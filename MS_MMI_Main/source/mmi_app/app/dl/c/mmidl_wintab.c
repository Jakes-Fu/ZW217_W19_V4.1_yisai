/****************************************************************************
** File Name:      mmidl_wintab.c                                            *
** Author:                                                                    *
** Date:           07/28/2009                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the window for Download            *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 07/2009        wancan.you        Create
** 
****************************************************************************/
#define _MMIDL_WINTAB_C_  

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_dl_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h" 
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipub.h"
#include "guiedit.h"
#include "guisoftkey.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "guimenu.h"
#include "guirichtext.h"
#include "guires.h"
#include "guidropdownlist.h"
#include "mmidisplay_data.h"
#include "mmidl_internal.h"
#include "mmidl_menutab.h"
#include "mmidl_text.h"
#include "mmidl_image.h"
#include "mmidl_id.h"
#include "mmidl_nv.h"
#include "mmifmm_export.h"
#include "mime_type.h"
#include "gui_ucs2b_converter.h"
#include "sfs.h"
#include "mmicc_export.h"
#include "guiform.h"
#ifdef JAVA_SUPPORT_IA
#include "mmijava_export.h"
#endif

#ifdef DL_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#ifdef MAINLCD_SIZE_176X220
#define MMIDL_CONTROL_V_MARGIN          (7)
#define MMIDL_LABEL_HEIGHT              (18)
#else
#define MMIDL_CONTROL_V_MARGIN          (10)
#define MMIDL_LABEL_HEIGHT              (24)
#endif

#define MMIDL_DOWNLOAD_AUTH_USER_LEN  20
#define MMIDL_DOWNLOAD_AUTH_PASSWORD_LEN  20

#define MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN  127
#define MMIDL_FILENAME_EXT_MAX_LEN  (5)
#define MMIDL_DOWNLOAD_AUTH_TITLE_LEFT  0
#define MMIDL_DOWNLOAD_AUTH_TITLE_TOP  0
#define MMIDL_DOWNLOAD_AUTH_TITLE_RIGHT  (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - MMIDL_DOWNLOAD_AUTH_TITLE_LEFT)
#define MMIDL_DOWNLOAD_AUTH_TITLE_BOTTOM  (MMIDL_DOWNLOAD_AUTH_TITLE_TOP + MMI_TITLE_HEIGHT - 1)

#define MMIDL_DOWNLOAD_AUTH_ACCOUNT_LEFT  (MMIDL_DOWNLOAD_AUTH_TITLE_LEFT + 8)
#define MMIDL_DOWNLOAD_AUTH_ACCOUNT_TOP  (MMIDL_DOWNLOAD_AUTH_TITLE_BOTTOM + 10)
#define MMIDL_DOWNLOAD_AUTH_ACCOUNT_RIGHT  (MMIDL_DOWNLOAD_AUTH_TITLE_RIGHT - 8)
#define MMIDL_DOWNLOAD_AUTH_ACCOUNT_BOTTOM  (MMIDL_DOWNLOAD_AUTH_ACCOUNT_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_LEFT  MMIDL_DOWNLOAD_AUTH_ACCOUNT_LEFT
#define MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_TOP  (MMIDL_DOWNLOAD_AUTH_ACCOUNT_BOTTOM + 10)
#define MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_RIGHT  MMIDL_DOWNLOAD_AUTH_ACCOUNT_RIGHT
#define MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_BOTTOM  (MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_AUTH_PASSWORD_LEFT  MMIDL_DOWNLOAD_AUTH_ACCOUNT_LEFT
#define MMIDL_DOWNLOAD_AUTH_PASSWORD_TOP  (MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_BOTTOM + 10)
#define MMIDL_DOWNLOAD_AUTH_PASSWORD_RIGHT  MMIDL_DOWNLOAD_AUTH_ACCOUNT_RIGHT
#define MMIDL_DOWNLOAD_AUTH_PASSWORD_BOTTOM  (MMIDL_DOWNLOAD_AUTH_PASSWORD_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_LEFT  MMIDL_DOWNLOAD_AUTH_ACCOUNT_LEFT
#define MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_TOP  (MMIDL_DOWNLOAD_AUTH_PASSWORD_BOTTOM + 10)
#define MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_RIGHT  MMIDL_DOWNLOAD_AUTH_ACCOUNT_RIGHT
#define MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_BOTTOM  (MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_SAVE_TITLE_LEFT  0
#define MMIDL_DOWNLOAD_SAVE_TITLE_TOP  0
#define MMIDL_DOWNLOAD_SAVE_TITLE_RIGHT  (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - MMIDL_DOWNLOAD_SAVE_TITLE_LEFT)
#define MMIDL_DOWNLOAD_SAVE_TITLE_BOTTOM  (MMIDL_DOWNLOAD_SAVE_TITLE_TOP + MMI_TITLE_HEIGHT - 1)

#define MMIDL_DOWNLOAD_SAVE_FILE_NAME_LEFT  (MMIDL_DOWNLOAD_SAVE_TITLE_LEFT + 8)
#define MMIDL_DOWNLOAD_SAVE_FILE_NAME_TOP  (MMIDL_DOWNLOAD_SAVE_TITLE_BOTTOM + 10)
#define MMIDL_DOWNLOAD_SAVE_FILE_NAME_RIGHT  (MMIDL_DOWNLOAD_SAVE_TITLE_RIGHT - 8)
#define MMIDL_DOWNLOAD_SAVE_FILE_NAME_BOTTOM  (MMIDL_DOWNLOAD_SAVE_FILE_NAME_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_LEFT  MMIDL_DOWNLOAD_SAVE_FILE_NAME_LEFT
#define MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_TOP  (MMIDL_DOWNLOAD_SAVE_FILE_NAME_BOTTOM + 10)
#define MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_RIGHT  MMIDL_DOWNLOAD_SAVE_FILE_NAME_RIGHT
#define MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_BOTTOM  (MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_SAVE_PATH_LEFT  MMIDL_DOWNLOAD_SAVE_FILE_NAME_LEFT
#define MMIDL_DOWNLOAD_SAVE_PATH_TOP  (MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_BOTTOM + 10)
#define MMIDL_DOWNLOAD_SAVE_PATH_RIGHT  MMIDL_DOWNLOAD_SAVE_FILE_NAME_RIGHT
#define MMIDL_DOWNLOAD_SAVE_PATH_BOTTOM  (MMIDL_DOWNLOAD_SAVE_PATH_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_SAVE_PATH_VALUE_LEFT  MMIDL_DOWNLOAD_SAVE_FILE_NAME_LEFT
#define MMIDL_DOWNLOAD_SAVE_PATH_VALUE_TOP  (MMIDL_DOWNLOAD_SAVE_PATH_BOTTOM + 10)
#define MMIDL_DOWNLOAD_SAVE_PATH_VALUE_RIGHT  MMIDL_DOWNLOAD_SAVE_FILE_NAME_RIGHT
#define MMIDL_DOWNLOAD_SAVE_PATH_VALUE_BOTTOM  (MMIDL_DOWNLOAD_SAVE_PATH_VALUE_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_LEFT           MMIDL_DOWNLOAD_SAVE_FILE_NAME_LEFT
#define MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_TOP            MMIDL_DOWNLOAD_SAVE_FILE_NAME_TOP
#define MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_RIGHT          MMIDL_DOWNLOAD_SAVE_FILE_NAME_RIGHT
#define MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_BOTTOM         MMIDL_DOWNLOAD_SAVE_FILE_NAME_BOTTOM

#define MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_VALUE_LEFT     MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_LEFT
#define MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_VALUE_TOP      MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_TOP
#define MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_VALUE_RIGHT    MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_RIGHT
#define MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_VALUE_BOTTOM   MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_BOTTOM

#define MMIDL_DOWNLOAD_SAVE_AS_PATH_LEFT                MMIDL_DOWNLOAD_SAVE_PATH_LEFT
#define MMIDL_DOWNLOAD_SAVE_AS_PATH_TOP                 MMIDL_DOWNLOAD_SAVE_PATH_TOP
#define MMIDL_DOWNLOAD_SAVE_AS_PATH_RIGHT               MMIDL_DOWNLOAD_SAVE_PATH_RIGHT
#define MMIDL_DOWNLOAD_SAVE_AS_PATH_BOTTOM              MMIDL_DOWNLOAD_SAVE_PATH_BOTTOM

#define MMIDL_DOWNLOAD_SAVE_AS_PATH_VALUE_LEFT          MMIDL_DOWNLOAD_SAVE_PATH_VALUE_LEFT
#define MMIDL_DOWNLOAD_SAVE_AS_PATH_VALUE_TOP           MMIDL_DOWNLOAD_SAVE_PATH_VALUE_TOP
#define MMIDL_DOWNLOAD_SAVE_AS_PATH_VALUE_RIGHT         MMIDL_DOWNLOAD_SAVE_PATH_VALUE_RIGHT
#define MMIDL_DOWNLOAD_SAVE_AS_PATH_VALUE_BOTTOM        MMIDL_DOWNLOAD_SAVE_PATH_VALUE_BOTTOM

#define MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_LEFT    10
#define MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_TOP     (MMI_TITLE_HEIGHT + 10)
#ifdef MAINLCD_SIZE_176X220
#define MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_RIGHT  (MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_LEFT + 60)
#define MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_BOTTOM  (MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_TOP + MMIDL_LABEL_HEIGHT - 1 + 3)// Adjust dropdown
#else
#define MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_RIGHT  (MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_LEFT + 80)
#define MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_BOTTOM  (MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_TOP + MMIDL_LABEL_HEIGHT - 1)
#endif

#define MMIDL_DOWNLOAD_SETTINGS_METHOD_LEFT  (MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_RIGHT + 10)
#define MMIDL_DOWNLOAD_SETTINGS_METHOD_TOP  MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_TOP
#define MMIDL_DOWNLOAD_SETTINGS_METHOD_RIGHT  (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - 10)
#define MMIDL_DOWNLOAD_SETTINGS_METHOD_BOTTOM  MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_BOTTOM

#define MMIDL_DOWNLOAD_SETTINGS_PATH_NAME_LEFT  MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_LEFT
#define MMIDL_DOWNLOAD_SETTINGS_PATH_NAME_TOP  (MMIDL_DOWNLOAD_SETTINGS_METHOD_BOTTOM + MMIDL_CONTROL_V_MARGIN)
#define MMIDL_DOWNLOAD_SETTINGS_PATH_NAME_RIGHT  MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_RIGHT

#ifdef MAINLCD_SIZE_176X220
#define MMIDL_DOWNLOAD_SETTINGS_PATH_NAME_BOTTOM  (MMIDL_DOWNLOAD_SETTINGS_PATH_NAME_TOP + MMIDL_LABEL_HEIGHT - 1 + 3)// Adjust dropdown
#else
#define MMIDL_DOWNLOAD_SETTINGS_PATH_NAME_BOTTOM  (MMIDL_DOWNLOAD_SETTINGS_PATH_NAME_TOP + MMIDL_LABEL_HEIGHT - 1)
#endif



#define MMIDL_DOWNLOAD_SETTINGS_PATH_LEFT  (MMIDL_DOWNLOAD_SETTINGS_PATH_NAME_RIGHT + 10)
#define MMIDL_DOWNLOAD_SETTINGS_PATH_TOP  MMIDL_DOWNLOAD_SETTINGS_PATH_NAME_TOP
#define MMIDL_DOWNLOAD_SETTINGS_PATH_RIGHT  MMIDL_DOWNLOAD_SETTINGS_METHOD_RIGHT
#define MMIDL_DOWNLOAD_SETTINGS_PATH_BOTTOM  MMIDL_DOWNLOAD_SETTINGS_PATH_NAME_BOTTOM

#define MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_LEFT       (MMIDL_DOWNLOAD_SETTINGS_METHOD_NAME_LEFT)
#define MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_TOP        (MMIDL_DOWNLOAD_SETTINGS_PATH_NAME_BOTTOM + MMIDL_CONTROL_V_MARGIN + 5)
#ifdef MAINLCD_SIZE_176X220
#define MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_RIGHT      (MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_LEFT + 30)
#else
#define MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_RIGHT      (MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_LEFT + 50)
#endif
#define MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_BOTTOM     (MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_LEFT      (MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_RIGHT + 3)
#define MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_TOP       (MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_TOP)
#define MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_RIGHT     (MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_RIGHT)
#define MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_BOTTOM    (MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_BOTTOM)

#define MMIDL_DOWNLOAD_SAVE_MUSIC_NAME_LEFT         (MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_LEFT)
#define MMIDL_DOWNLOAD_SAVE_MUSIC_NAME_TOP          (MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_BOTTOM + MMIDL_CONTROL_V_MARGIN)
#define MMIDL_DOWNLOAD_SAVE_MUSIC_NAME_RIGHT        (MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_RIGHT)
#define MMIDL_DOWNLOAD_SAVE_MUSIC_NAME_BOTTOM       (MMIDL_DOWNLOAD_SAVE_MUSIC_NAME_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_SAVE_MUSIC_VALUE_LEFT        (MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_LEFT)
#define MMIDL_DOWNLOAD_SAVE_MUSIC_VALUE_TOP         (MMIDL_DOWNLOAD_SAVE_MUSIC_NAME_TOP)
#define MMIDL_DOWNLOAD_SAVE_MUSIC_VALUE_RIGHT       (MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_RIGHT)
#define MMIDL_DOWNLOAD_SAVE_MUSIC_VALUE_BOTTOM      (MMIDL_DOWNLOAD_SAVE_MUSIC_NAME_BOTTOM)

#define MMIDL_DOWNLOAD_SAVE_VIDEO_NAME_LEFT         (MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_LEFT)
#define MMIDL_DOWNLOAD_SAVE_VIDEO_NAME_TOP          (MMIDL_DOWNLOAD_SAVE_MUSIC_NAME_BOTTOM + MMIDL_CONTROL_V_MARGIN)
#define MMIDL_DOWNLOAD_SAVE_VIDEO_NAME_RIGHT        (MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_RIGHT)
#define MMIDL_DOWNLOAD_SAVE_VIDEO_NAME_BOTTOM       (MMIDL_DOWNLOAD_SAVE_VIDEO_NAME_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_SAVE_VIDEO_VALUE_LEFT        (MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_LEFT)
#define MMIDL_DOWNLOAD_SAVE_VIDEO_VALUE_TOP         (MMIDL_DOWNLOAD_SAVE_VIDEO_NAME_TOP)
#define MMIDL_DOWNLOAD_SAVE_VIDEO_VALUE_RIGHT       (MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_RIGHT)
#define MMIDL_DOWNLOAD_SAVE_VIDEO_VALUE_BOTTOM      (MMIDL_DOWNLOAD_SAVE_VIDEO_NAME_BOTTOM)

#define MMIDL_DOWNLOAD_SAVE_OTHERS_NAME_LEFT           (MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_LEFT)
#define MMIDL_DOWNLOAD_SAVE_OTHERS_NAME_TOP            (MMIDL_DOWNLOAD_SAVE_VIDEO_VALUE_BOTTOM + MMIDL_CONTROL_V_MARGIN)
#define MMIDL_DOWNLOAD_SAVE_OTHERS_NAME_RIGHT          (MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_RIGHT)
#define MMIDL_DOWNLOAD_SAVE_OTHERS_NAME_BOTTOM         (MMIDL_DOWNLOAD_SAVE_OTHERS_NAME_TOP + MMIDL_LABEL_HEIGHT - 1)

#define MMIDL_DOWNLOAD_SAVE_OTHERS_VALUE_LEFT          (MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_LEFT)
#define MMIDL_DOWNLOAD_SAVE_OTHERS_VALUE_TOP           (MMIDL_DOWNLOAD_SAVE_OTHERS_NAME_TOP)
#define MMIDL_DOWNLOAD_SAVE_OTHERS_VALUE_RIGHT         (MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_RIGHT)
#define MMIDL_DOWNLOAD_SAVE_OTHERS_VALUE_BOTTOM        (MMIDL_DOWNLOAD_SAVE_OTHERS_NAME_BOTTOM)

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

#ifndef MMI_GUI_STYLE_TYPICAL
typedef enum
{
    MMIDL_MOTHOD_TYPE_APPOSITE = 1,
    MMIDL_MOTHOD_TYPE_QUEUE,

    MMIDL_MOTHOD_TYPE_MAX
}MMIDL_MOTHOD_TYPE_E;

typedef enum
{
    MMIDL_PATH_TYPE_DEFAULT = 1,
    MMIDL_PATH_TYPE_CUSTOMIZE,

    MMIDL_PATH_TYPE_MAX
}MMIDL_PATH_TYPE_E;

typedef void (*SETTING_ITEM_FUNC)(void);
#endif

LOCAL uint16 s_dev_item_index = MMI_DEVICE_UDISK;
/**--------------------------------------------------------------------------*
 **                          EXTERN DECLARE                                *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle message of download Auth win
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadAuthWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : handle message of download auth input
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadAuthInputWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of download Save win
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadSaveWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of download Save win
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadSaveAsWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenFileCallBack(BOOLEAN is_success, FILEARRAY file_array);

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenSaveAsPathCallBack(BOOLEAN is_success, FILEARRAY file_array);

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenPicturePathCallBack(BOOLEAN is_success, FILEARRAY file_array);

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenMusicPathCallBack(BOOLEAN is_success, FILEARRAY file_array);

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenVideoPathCallBack(BOOLEAN is_success, FILEARRAY file_array);

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenOtherPathCallBack(BOOLEAN is_success, FILEARRAY file_array);

/*****************************************************************************/
// 	Description : handle message of input download file name
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputDownloadFileName(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle message of download manager win
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadManagerWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : handle messages of download manager option menu win
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadManagerOptWinMsg(MMI_WIN_ID_T win_id,
                                                                                                          MMI_MESSAGE_ID_E msg_id,
                                                                                                          DPARAM param);

/*****************************************************************************/
//  Description : Set Download Manager Option Menu Gray
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void SetDownloadManagerOptionGray(MMI_WIN_ID_T win_id, uint32 task_index);

/*****************************************************************************/
//  Description : handle message of download task detail
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadTaskDetailWinMsg(MMI_WIN_ID_T win_id,
                                                                                                          MMI_MESSAGE_ID_E msg_id,
                                                                                                          DPARAM param);

/*****************************************************************************/
//  Description : handle message of download query
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadQueryWinMsg(MMI_WIN_ID_T win_id,
                                                                                                          MMI_MESSAGE_ID_E msg_id,
                                                                                                          DPARAM param);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQueryUseWlanWinMsg(MMI_WIN_ID_T win_id,
                                                                                                          MMI_MESSAGE_ID_E msg_id,
                                                                                                          DPARAM param);

/*****************************************************************************/
//  Description : handle message of download settings
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadSettingsWinMsg(MMI_WIN_ID_T win_id,
                                                                                                          MMI_MESSAGE_ID_E msg_id,
                                                                                                          DPARAM param);

/*****************************************************************************/
// 	Description : set Download settings param
//	Global resource dependence : 
//  Author: Jiaoyou.wu
//	Note: 
/*****************************************************************************/
LOCAL void SetDownloadPath(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : set Download settings param
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void SetDownloadSettings(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : save download settings param
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void SaveDownloadSettings(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : handle message of DD property
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDDPropertyWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : Set DD property info
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIDL_SetDDPropertyInfo(MMI_WIN_ID_T  win_id);

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void AppendTwoTextOneImageItem(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_1, MMI_TEXT_ID_T text_2, MMI_IMAGE_ID_T image_id, uint32 user_data);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void InsertTwoTextOneImageItem(MMI_CTRL_ID_T ctrl_id, uint16 index, MMI_TEXT_ID_T text_1, MMI_TEXT_ID_T text_2, MMI_IMAGE_ID_T image_id, uint32 user_data);

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void dlHandleMethodSet(void);

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void dlHandlePathSet(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DlHandleMethodSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DlHandlePathSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void DlCreateMethodSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void DlCreatePathSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
void DlCreateRadioListByTextId(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint16 node_num, MMI_TEXT_ID_T *text_id_array, uint32 *node_array, uint32 select_index);

/*****************************************************************************/
//  Description : Get List User data 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL uint32 DlGetListUserdata(MMI_CTRL_ID_T ctrl_id, uint16 index);
#endif

/*****************************************************************************/
//  Description : handle message of download manager win
//  Global resource dependence : none
//  Author: wancna.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadSelectDeviceWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/**--------------------------------------------------------------------------*
**                         WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/
//the window for download auth
WINDOW_TABLE(MMIDL_DOWNLOAD_AUTH_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDownloadAuthWinMsg),    
    WIN_ID(MMIDL_DOWNLOAD_AUTH_WIN_ID),
//    WIN_STATUSBAR,
    WIN_TITLE(TXT_DL_DOWNLOAD_AUTH),
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    #endif /* MMI_PDA_SUPPORT */
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
    //Account Label
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MMIDL_DOWNLOAD_AUTH_ACCOUNT_CTRL_ID),
    //Account Value Label
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_CTRL_ID),
    //Password Label
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MMIDL_DOWNLOAD_AUTH_PASSWORD_CTRL_ID),
    //Password Value Label
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_CTRL_ID),
    END_WIN
};

//the window for download auth input
WINDOW_TABLE(MMIDL_DOWNLOAD_AUTH_INPUT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDownloadAuthInputWinMsg),    
    WIN_TITLE(TXT_NULL),
    WIN_ID(MMIDL_DOWNLOAD_AUTH_INPUT_WIN_ID),
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    #endif /* MMI_PDA_SUPPORT */
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    
    CREATE_EDIT_TEXT_CTRL(0,MMIDL_DOWNLOAD_AUTH_INPUT_CTRL_ID),
        
    END_WIN
};

//the window for download Save
WINDOW_TABLE(MMIDL_DOWNLOAD_SAVE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDownloadSaveWinMsg),    
    WIN_ID(MMIDL_DOWNLOAD_SAVE_WIN_ID),
//    WIN_STATUSBAR,
    WIN_TITLE(STXT_SAVE),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(STXT_SAVE, TXT_EDIT, STXT_RETURN),
#endif /* MMI_PDA_SUPPORT */
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIDL_DOWNLOAD_SAVE_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIDL_DOWNLOAD_SAVE_FILE_FORM_CTRL_ID, MMIDL_DOWNLOAD_SAVE_FORM_CTRL_ID),
    //Filename Label
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIDL_DOWNLOAD_SAVE_FILE_NAME_CTRL_ID,MMIDL_DOWNLOAD_SAVE_FILE_FORM_CTRL_ID),
    //Filename Value Label
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_CTRL_ID,MMIDL_DOWNLOAD_SAVE_FILE_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIDL_DOWNLOAD_SAVE_PATH_FORM_CTRL_ID, MMIDL_DOWNLOAD_SAVE_FORM_CTRL_ID),
    //Path Label
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIDL_DOWNLOAD_SAVE_PATH_CTRL_ID,MMIDL_DOWNLOAD_SAVE_PATH_FORM_CTRL_ID),
    //Path Value Label
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIDL_DOWNLOAD_SAVE_PATH_VALUE_CTRL_ID,MMIDL_DOWNLOAD_SAVE_PATH_FORM_CTRL_ID),
    END_WIN
};

//the window for download Save As
WINDOW_TABLE(MMIDL_DOWNLOAD_SAVE_AS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDownloadSaveAsWinMsg),    
    WIN_ID(MMIDL_DOWNLOAD_SAVE_AS_WIN_ID),
//    WIN_STATUSBAR,
    WIN_TITLE(STXT_SAVE),
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    #endif /* MMI_PDA_SUPPORT */
    WIN_SOFTKEY(STXT_SAVE, TXT_EDIT, STXT_RETURN),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIDL_DOWNLOAD_SAVE_AS_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIDL_DOWNLOAD_SAVE_AS_FILE_FORM_CTRL_ID, MMIDL_DOWNLOAD_SAVE_AS_FORM_CTRL_ID),
    //Filename Label
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_CTRL_ID,MMIDL_DOWNLOAD_SAVE_AS_FILE_FORM_CTRL_ID),
    //Filename Value Label
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_VALUE_CTRL_ID,MMIDL_DOWNLOAD_SAVE_AS_FILE_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIDL_DOWNLOAD_SAVE_AS_PATH_FORM_CTRL_ID, MMIDL_DOWNLOAD_SAVE_AS_FORM_CTRL_ID),
    //Path Label
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIDL_DOWNLOAD_SAVE_AS_PATH_CTRL_ID,MMIDL_DOWNLOAD_SAVE_AS_PATH_FORM_CTRL_ID),
    //Path Value Label
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIDL_DOWNLOAD_SAVE_AS_PATH_VALUE_CTRL_ID,MMIDL_DOWNLOAD_SAVE_AS_PATH_FORM_CTRL_ID),

    END_WIN
};

//the window for input download file name
WINDOW_TABLE(MMIDL_DOWNLOAD_INPUT_FILENAME_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleInputDownloadFileName),
    WIN_ID(MMIDL_DOWNLOAD_INPUT_FILENAME_WIN_ID),
//    WIN_STATUSBAR,
    WIN_TITLE(TXT_DL_DOWNLOAD_INPUT_FILENAME),
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    #endif /* MMI_PDA_SUPPORT */
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
    CREATE_EDIT_TEXT_CTRL(0,MMIDL_DOWNLOAD_INPUT_FILENAME_CTRL_ID),
    END_WIN
};

//the window for download manager
WINDOW_TABLE(MMIDL_DOWNLOAD_MANAGER_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDownloadManagerWinMsg),
    WIN_ID(MMIDL_DOWNLOAD_MANAGER_WIN_ID),
//    WIN_STATUSBAR,
    WIN_TITLE(TXT_DL_DOWNLOAD_MANAGER),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIDL_DOWNLOAD_MANAGER_LIST_CTRL_ID),
    END_WIN
};

//the window for download manager
WINDOW_TABLE(MMIDL_DOWNLOAD_SELECT_DEVICE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDownloadSelectDeviceWinMsg),
    WIN_ID(MMIDL_DOWNLOAD_SELECT_DEVICE_WIN_ID),
        //    WIN_STATUSBAR,
    WIN_TITLE(STXT_STORE_MEDIUM),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIDL_DOWNLOAD_SELECT_DEVICE_LIST_CTRL_ID),
    END_WIN
};

//the window for download manager option menu
WINDOW_TABLE(MMIDL_DOWNLOAD_MANAGER_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDownloadManagerOptWinMsg),
    WIN_ID(MMIDL_DOWNLOAD_MANAGER_OPT_WIN_ID),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_DL_MANAGET_OPT, MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID),
    END_WIN
};

//the window for download task detail
WINDOW_TABLE(MMIDL_DOWNLOAD_TASK_DETAIL_WIN_TAB) = 
{
    WIN_TITLE(STXT_DETAIL),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32)HandleDownloadTaskDetailWinMsg),    
    WIN_ID(MMIDL_DOWNLOAD_TASK_DETAIL_WIN_ID),

    CREATE_RICHTEXT_CTRL(MMIDL_DOWNLOAD_TASK_DETAIL_CTRL_ID),

    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),

    END_WIN
};

//the window for download query
WINDOW_TABLE(MMIDL_DOWNLOAD_QUERY_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDownloadQueryWinMsg),    
    WIN_ID(MMIDL_DOWNLOAD_TASK_DETAIL_WIN_ID),
    WIN_STYLE(WS_HAS_TRANSPARENT),

    END_WIN
};   
    
//the window for download query
WINDOW_TABLE(MMIDL_DOWNLOAD_SETTINGS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDownloadSettingsWinMsg),    
    WIN_ID(MMIDL_DOWNLOAD_SETTINGS_WIN_ID),
    WIN_TITLE(STXT_MAIN_SETTING),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIDL_DOWNLOAD_SETTINGS_FORM_CTRL_ID),

#ifndef MMI_GUI_STYLE_TYPICAL
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIDL_DOWNLOAD_SETTINGS_LIST_CTRL_ID, MMIDL_DOWNLOAD_SETTINGS_FORM_CTRL_ID),
#else
    //download method
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIDL_DOWNLOAD_SETTINGS_METHOD_FORM_CTRL_ID,MMIDL_DOWNLOAD_SETTINGS_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIDL_DOWNLOAD_SETTINGS_METHOD_LABEL_CTRL_ID,MMIDL_DOWNLOAD_SETTINGS_METHOD_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIDL_DOWNLOAD_SETTINGS_METHOD_LIST_CTRL_ID,MMIDL_DOWNLOAD_SETTINGS_METHOD_FORM_CTRL_ID),

    //download path
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIDL_DOWNLOAD_SETTINGS_PATH_FORM_CTRL_ID,MMIDL_DOWNLOAD_SETTINGS_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIDL_DOWNLOAD_SETTINGS_PATH_LABEL_CTRL_ID,MMIDL_DOWNLOAD_SETTINGS_PATH_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIDL_DOWNLOAD_SETTINGS_PATH_LIST_CTRL_ID,MMIDL_DOWNLOAD_SETTINGS_PATH_FORM_CTRL_ID),

#endif
    //Picture Label
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIDL_DOWNLOAD_SAVE_PICTURE_FORM_CTRL_ID,MMIDL_DOWNLOAD_SETTINGS_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_CTRL_ID,MMIDL_DOWNLOAD_SAVE_PICTURE_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_CTRL_ID,MMIDL_DOWNLOAD_SAVE_PICTURE_FORM_CTRL_ID),

    //Music Label
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIDL_DOWNLOAD_SAVE_MUSIC_FORM_CTRL_ID,MMIDL_DOWNLOAD_SETTINGS_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIDL_DOWNLOAD_SAVE_MUSIC_NAME_CTRL_ID,MMIDL_DOWNLOAD_SAVE_MUSIC_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMIDL_DOWNLOAD_SAVE_MUSIC_VALUE_CTRL_ID,MMIDL_DOWNLOAD_SAVE_MUSIC_FORM_CTRL_ID),

    //Video Label
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIDL_DOWNLOAD_SAVE_VIDEO_FORM_CTRL_ID,MMIDL_DOWNLOAD_SETTINGS_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIDL_DOWNLOAD_SAVE_VIDEO_NAME_CTRL_ID,MMIDL_DOWNLOAD_SAVE_VIDEO_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMIDL_DOWNLOAD_SAVE_VIDEO_VALUE_CTRL_ID,MMIDL_DOWNLOAD_SAVE_VIDEO_FORM_CTRL_ID),

    //Others Label
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIDL_DOWNLOAD_SAVE_OTHERS_FORM_CTRL_ID,MMIDL_DOWNLOAD_SETTINGS_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIDL_DOWNLOAD_SAVE_OTHERS_NAME_CTRL_ID,MMIDL_DOWNLOAD_SAVE_OTHERS_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMIDL_DOWNLOAD_SAVE_OTHERS_VALUE_CTRL_ID,MMIDL_DOWNLOAD_SAVE_OTHERS_FORM_CTRL_ID),

    WIN_SOFTKEY(STXT_SAVE,TXT_NULL,STXT_RETURN),

    END_WIN
};

//the window for DD property
WINDOW_TABLE(MMIDL_DD_PROPERTY_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDDPropertyWinMsg),
    WIN_TITLE(STXT_DETAIL),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
    WIN_ID(MMIDL_DD_PROPERTY_WIN_ID),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    CREATE_RICHTEXT_CTRL(MMIDL_DD_PROPERTY_RICHTEXT_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#ifndef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE(MMIDL_METHOD_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) DlHandleMethodSetWinMsg), 
    WIN_ID(MMIDL_METHOD_SET_WIN_ID),
    WIN_TITLE(TXT_DL_SETTINGS_METHOD),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIDL_DOWNLOAD_SETTINGS_METHOD_MENU_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIDL_PATH_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32) DlHandlePathSetWinMsg), 
    WIN_ID(MMIDL_PATH_SET_WIN_ID),
    WIN_TITLE(TXT_DL_SETTINGS_PATH),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIDL_DOWNLOAD_SETTINGS_PATH_MENU_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Open Download Authorize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadAuthWin(uint32 task_id)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();

    win_table_create.applet_handle = MMIDL_HANDLE(download_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIDL_DOWNLOAD_AUTH_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)task_id;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Open Download Authorize Input
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadAuthInputWin(MMI_CTRL_ID_T ctrl_id)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();

    win_table_create.applet_handle = MMIDL_HANDLE(download_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIDL_DOWNLOAD_AUTH_INPUT_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)ctrl_id;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Open Download Save Win
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadSaveWin(uint32 task_id)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();

    win_table_create.applet_handle = MMIDL_HANDLE(download_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIDL_DOWNLOAD_SAVE_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)task_id;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Create page property win
//  Global resource dependence : 
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDDPropertyWin(MMIDL_OMA_DD_INFO_T *dd_info_ptr)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();
    MMIDL_OMA_DD_INFO_T * dd_info = PNULL;

    dd_info = SCI_ALLOCA(sizeof(MMIDL_OMA_DD_INFO_T));

    if (PNULL == dd_info)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_MEMCPY(dd_info, dd_info_ptr, sizeof(MMIDL_OMA_DD_INFO_T));

    win_table_create.applet_handle = MMIDL_HANDLE(download_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIDL_DD_PROPERTY_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)dd_info;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Open Download Save As Win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadSaveAsWin(MMIDL_DOWNLOAD_INFO_T *download_info_ptr)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMIDL_DOWNLOAD_SAVE_AS_INFO_T  *save_as_info = PNULL;
    uint16 url_len = 0;

    if (PNULL == download_info_ptr || PNULL == download_info_ptr->url || PNULL == download_info_ptr->buf)
    {
        return MMI_RESULT_FALSE;
    }

    url_len = (uint16)strlen((const char *)download_info_ptr->url);
    save_as_info = SCI_ALLOCA(sizeof(MMIDL_DOWNLOAD_SAVE_AS_INFO_T));

    if (PNULL == save_as_info)
    {
        //SCI_TRACE_LOW:"mmidl_wintab.c:MMIDL_OpenDownloadSaveAsWin() save_as_info is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_679_112_2_18_2_13_38_161,(uint8*)"");
        return MMI_RESULT_TRUE;
    }

    SCI_MEMSET(save_as_info, 0x0, sizeof(MMIDL_DOWNLOAD_SAVE_AS_INFO_T));

    if (download_info_ptr->buf_len > 0)
    {
        save_as_info->buf = SCI_ALLOCA(download_info_ptr->buf_len);
        if (PNULL == save_as_info->buf)
        {
            SCI_FREE(save_as_info);
            save_as_info = PNULL;
            //SCI_TRACE_LOW:"mmidl_wintab.c:MMIDL_OpenDownloadSaveAsWin() save_as_info->buf is NULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_692_112_2_18_2_13_38_162,(uint8*)"");
            return MMI_RESULT_TRUE;
        }
        save_as_info->buf_len = download_info_ptr->buf_len;
        SCI_MEMCPY(save_as_info->buf, download_info_ptr->buf, save_as_info->buf_len);
    }
    if (url_len > 0)
    {
        save_as_info->url = SCI_ALLOCA(url_len + 1);
        if (PNULL ==save_as_info->url)
        {
            SCI_FREE(save_as_info->buf);
            save_as_info->buf = PNULL;
            SCI_FREE(save_as_info);
            save_as_info = PNULL;
            //SCI_TRACE_LOW:"mmidl_wintab.c:MMIDL_OpenDownloadSaveAsWin() save_as_info->url is NULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_707_112_2_18_2_13_38_163,(uint8*)"");
            return MMI_RESULT_TRUE;
        }
        SCI_MEMSET(save_as_info->url, 0x0, (url_len + 1));
        SCI_MEMCPY(save_as_info->url, download_info_ptr->url, url_len);
    }

    save_as_info->mime_type = download_info_ptr->mime_type;
    win_table_create.applet_handle = MMK_GetFirstAppletHandle();
    win_table_create.win_table_ptr = MMIDL_DOWNLOAD_SAVE_AS_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)save_as_info;

    MMK_CreateWinTable(&win_table_create);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Open Download Input filename
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenInputFileNameWin(MMI_HANDLE_T label_handle)
{
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();

    return MMIDL_OpenInputFileNameWinEx(MMIDL_HANDLE(download_applet), label_handle);
}

/*****************************************************************************/
//  Description : Open Download Input filename
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenInputFileNameWinEx(MMI_HANDLE_T applet_handle, MMI_HANDLE_T label_handle)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = applet_handle;
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIDL_DOWNLOAD_INPUT_FILENAME_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)label_handle;
    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Open Download Manager
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadManagerWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();
    MMI_HANDLE_T win_handle = PNULL;

    if (PNULL == download_applet)
    {
        return MMI_RESULT_FALSE;
    }

    win_handle = MMK_GetWinHandle(MMIDL_HANDLE(download_applet), MMIDL_DOWNLOAD_MANAGER_WIN_ID);

    if (MMK_IsOpenWin(win_handle))
    {
        MMK_WinGrabFocus(win_handle);
    }
    else
    {
        win_table_create.applet_handle = MMIDL_HANDLE(download_applet);
        win_table_create.parent_win_handle = 0;
        win_table_create.win_table_ptr = MMIDL_DOWNLOAD_MANAGER_WIN_TAB;

        MMK_CreateWinTable(&win_table_create);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Open Download Manager Option
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadManagerOptionWin(uint32 task_index)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();

    win_table_create.applet_handle = MMIDL_HANDLE(download_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIDL_DOWNLOAD_MANAGER_OPT_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)task_index;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Open Download Task Detail
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadTaskDetailWin(uint32 task_index)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();

    win_table_create.applet_handle = MMIDL_HANDLE(download_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIDL_DOWNLOAD_TASK_DETAIL_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)task_index;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Open Download Query
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadQueryWin(uint32 task_index)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();

    win_table_create.applet_handle = MMIDL_HANDLE(download_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIDL_DOWNLOAD_QUERY_WIN_TAB;
    win_table_create.add_data_ptr = (ADD_DATA)task_index;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Open Download Settings
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIDL_OpenDownloadSettingsWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();

    win_table_create.applet_handle = MMIDL_HANDLE(download_applet);
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIDL_DOWNLOAD_SETTINGS_WIN_TAB;

    MMK_CreateWinTable(&win_table_create);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle message of download Auth win
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadAuthWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_RECT_T account_value_rect = GUI_CreateRect(MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_LEFT,
                                                                                    MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_TOP,
                                                                                    MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_RIGHT,
                                                                                    MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_BOTTOM);
    GUI_RECT_T password_value_rect = GUI_CreateRect(MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_LEFT,
                                                                                        MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_TOP,
                                                                                        MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_RIGHT,
                                                                                        MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_BOTTOM);

    GUI_POINT_T point = {0};
    GUI_POINT_T dis_point = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    MMI_CTRL_ID_T active_ctrl_id = 0;
    GUI_RECT_T focus_rect = {0};
    MMI_HANDLE_T account_handle = 0;
    MMI_HANDLE_T password_handle = 0;
    MMI_STRING_T account_value_string = {0};
    MMI_STRING_T password_value_string = {0};
    MMI_HANDLE_T account_value_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_CTRL_ID);
    MMI_HANDLE_T password_value_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_CTRL_ID);
    uint32 task_id = (uint32)MMK_GetWinAddDataPtr(win_id);    

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    //SCI_TRACE_LOW:"mmidl_wintab.c:HandleDownloadAuthWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_908_112_2_18_2_13_38_165,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
        	GUI_RECT_T lab_rect = {0};
            
            lab_rect.left = MMIDL_DOWNLOAD_AUTH_ACCOUNT_LEFT;
            lab_rect.top = MMIDL_DOWNLOAD_AUTH_ACCOUNT_TOP;
            lab_rect.right = MMIDL_DOWNLOAD_AUTH_ACCOUNT_RIGHT;
            lab_rect.bottom = MMIDL_DOWNLOAD_AUTH_ACCOUNT_BOTTOM;

			GUILABEL_SetRect(MMIDL_DOWNLOAD_AUTH_ACCOUNT_CTRL_ID, &lab_rect, FALSE);
        }                                    

        {
        	GUI_RECT_T lab_rect = {0};
            lab_rect.left = MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_LEFT;
            lab_rect.top = MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_TOP;
            lab_rect.right = MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_RIGHT;
            lab_rect.bottom = MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_BOTTOM;
			GUILABEL_SetRect(MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_CTRL_ID, &lab_rect, FALSE);
        }                                    
                                    
        {
        	GUI_RECT_T lab_rect = {0};

            lab_rect.left = MMIDL_DOWNLOAD_AUTH_PASSWORD_LEFT;
            lab_rect.top = MMIDL_DOWNLOAD_AUTH_PASSWORD_TOP;
            lab_rect.right = MMIDL_DOWNLOAD_AUTH_PASSWORD_RIGHT;
            lab_rect.bottom = MMIDL_DOWNLOAD_AUTH_PASSWORD_BOTTOM;

			GUILABEL_SetRect(MMIDL_DOWNLOAD_AUTH_PASSWORD_CTRL_ID, &lab_rect, FALSE);
        }                                     
        {
        	GUI_RECT_T lab_rect = {0};

            lab_rect.left = MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_LEFT;
            lab_rect.top = MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_TOP;
            lab_rect.right = MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_RIGHT;
            lab_rect.bottom = MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_BOTTOM;

			GUILABEL_SetRect(MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_CTRL_ID, &lab_rect, FALSE);
        }                                    

        account_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_AUTH_ACCOUNT_CTRL_ID);
        password_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_AUTH_PASSWORD_CTRL_ID);

        GUILABEL_SetTextById(account_handle, TXT_ACCOUNT, FALSE);
        GUILABEL_SetTextById(password_handle, TXT_COMMON_PASSWORD, FALSE);
        
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_EDIT, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, account_value_handle);        
        break;

    case MSG_FULL_PAINT:
        dis_point.x = MMIDL_DOWNLOAD_AUTH_TITLE_LEFT;
        dis_point.y = MMIDL_DOWNLOAD_AUTH_TITLE_BOTTOM + 1;
        GUIRES_DisplayImg(&dis_point,
                                        PNULL,
                                        PNULL,
                                        win_id,
                                        IMAGE_COMMON_BG,
                                        &lcd_dev_info);
        
        active_ctrl_id = MMK_GetActiveCtrl(win_id);

        LCD_DrawRect(&lcd_dev_info, account_value_rect, MMI_DARK_GRAY_COLOR);
        LCD_DrawRect(&lcd_dev_info, password_value_rect, MMI_DARK_GRAY_COLOR);

        if (account_value_handle == active_ctrl_id)
        {
            focus_rect = GUI_CreateRect((uint16)(account_value_rect.left - 1),
                                                        (uint16)(account_value_rect.top - 1),
                                                        (uint16)(account_value_rect.right + 1),
                                                        (uint16)(account_value_rect.bottom + 1));

            GUILABEL_SetBackgroundColor(account_value_handle, MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
            GUILABEL_SetBackgroundColor(password_value_handle, 0);
        }
        else
        {
            focus_rect = GUI_CreateRect((uint16)(password_value_rect.left - 1),
                                                        (uint16)(password_value_rect.top - 1),
                                                        (uint16)(password_value_rect.right + 1),
                                                        (uint16)(password_value_rect.bottom + 1));

            GUILABEL_SetBackgroundColor(account_value_handle, 0);
            GUILABEL_SetBackgroundColor(password_value_handle, MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT));
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        if (GUI_PointIsInRect(point, account_value_rect))
        {
            active_ctrl_id = account_value_handle;
            MMIDL_OpenDownloadAuthInputWin(MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_CTRL_ID);
        }
        else if(GUI_PointIsInRect(point, password_value_rect))
        {
            
            active_ctrl_id = password_value_handle;
            MMIDL_OpenDownloadAuthInputWin(MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_CTRL_ID);
        }

        if (0 != active_ctrl_id)
        {
            MMK_SetAtvCtrl(win_id, active_ctrl_id);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_UP:
    case MSG_APP_DOWN:
        active_ctrl_id = MMK_GetActiveCtrl(win_id);

        if (account_value_handle == active_ctrl_id)
        {
            active_ctrl_id = password_value_handle;
        }
        else
        {
            active_ctrl_id = account_value_handle;
        }
        MMK_SetAtvCtrl(win_id, active_ctrl_id);
        
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        active_ctrl_id = MMK_GetActiveCtrl(win_id);

        if (account_value_handle == active_ctrl_id)
        {
            MMIDL_OpenDownloadAuthInputWin(MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_CTRL_ID);
        }
        else
        {
            MMIDL_OpenDownloadAuthInputWin(MMIDL_DOWNLOAD_AUTH_PASSWORD_VALUE_CTRL_ID);
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        GUILABEL_GetText(account_value_handle, &account_value_string);
        GUILABEL_GetText(password_value_handle, &password_value_string);

        if (0 == account_value_string.wstr_len)
        {
            MMIPUB_OpenAlertWarningWin(TXT_DL_DOWNLOAD_AUTH_ACCOUNT_EMPTY);
        }
        else if (0 == password_value_string.wstr_len)
        {
            MMIPUB_OpenAlertWarningWin(TXT_DL_DOWNLOAD_AUTH_PASSWORD_EMPTY);
        }
        else
        {
            MMIDL_AuthResponse(task_id, &account_value_string, &password_value_string);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIDL_StopDownload(task_id, FALSE);
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        //SCI_TRACE_LOW:"mmidl_wintab.c:HandleDownloadAuthWinMsg() msg_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1084_112_2_18_2_13_38_166,(uint8*)"d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : handle message of download auth input
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadAuthInputWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_AUTH_INPUT_CTRL_ID);
    MMI_HANDLE_T label_ctrl_id = (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_HANDLE_T label_handle = 0;
    MMI_STRING_T label_string = {0};
    MMI_STRING_T edit_string = {0};
    DOWNLOAD_APPLET_T *applet = PNULL;
    MMI_HANDLE_T win_handle = PNULL;

    applet = MMIDL_APPLETINSTANCE();
    win_handle = MMK_GetWinHandle(MMIDL_HANDLE(applet), MMIDL_DOWNLOAD_AUTH_WIN_ID);
    label_handle = MMK_GetCtrlHandleByWin(win_handle, label_ctrl_id);

    //SCI_TRACE_LOW:"mmidl_wintab.c:HandleDownloadAuthInputWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1111_112_2_18_2_13_38_167,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (MMIDL_DOWNLOAD_AUTH_ACCOUNT_VALUE_CTRL_ID == label_ctrl_id)
        {
            GUIWIN_SetTitleTextId(win_id, TXT_ACCOUNT, FALSE);
            GUIEDIT_SetTextMaxLen(ctrl_handle, MMIDL_DOWNLOAD_AUTH_USER_LEN, MMIDL_DOWNLOAD_AUTH_USER_LEN);//TODO
        }
        else
        {
            GUIWIN_SetTitleTextId(win_id, TXT_COMMON_PASSWORD, FALSE);
            GUIEDIT_SetTextMaxLen(ctrl_handle, MMIDL_DOWNLOAD_AUTH_PASSWORD_LEN, MMIDL_DOWNLOAD_AUTH_PASSWORD_LEN);//TODO
        }

        GUIEDIT_SetIm(ctrl_handle, GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);//lint !e655

        GUILABEL_GetText(label_handle, &label_string);
        GUIEDIT_SetString(ctrl_handle, label_string.wstr_ptr, label_string.wstr_len);

        GUIEDIT_SetSoftkey(
                                        ctrl_handle,
                                        0,
                                        0,
                                        TXT_COMMON_OK,
                                        TXT_COMMON_OK, 
                                        PNULL
                                        );

        GUIEDIT_SetSoftkey(
                                        ctrl_handle,
                                        2,
                                        0,
                                        STXT_RETURN,
                                        TXT_DELETE,
                                        PNULL
                                        );

        MMK_SetAtvCtrl(win_id, ctrl_handle);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIEDIT_GetString(ctrl_handle, &edit_string);
        
        label_string.wstr_len = edit_string.wstr_len;
        label_string.wstr_ptr = edit_string.wstr_ptr;
        GUILABEL_SetText(label_handle, &label_string, FALSE);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenFileCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
    uint32 file_num = 0;
    FILEARRAY_DATA_T file_array_data = {0};
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"Download_OpenFileCallBack download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1191_112_2_18_2_13_39_168,(uint8*)"");

        if (PNULL != file_array)
        {
            MMIAPIFILEARRAY_Destroy(&file_array);
            file_array = PNULL;
        }

        return;
    }

    //SCI_TRACE_LOW:"Download_OpenFileCallBack is_success = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1202_112_2_18_2_13_39_169,(uint8*)"d", is_success);

    if (is_success)
    {
        file_num = MMIAPIFILEARRAY_GetArraySize(file_array);

        //SCI_TRACE_LOW:"Download_OpenFileCallBack, file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1208_112_2_18_2_13_39_170,(uint8*)"d", file_num);

        if (file_num > 0 && (MMIAPIFILEARRAY_Read(file_array, 0, &file_array_data)))
        {
            //read user selected dir
            //SCI_TRACE_LOW:"Download_OpenFileCallBack pathlen = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1213_112_2_18_2_13_39_171,(uint8*)"d", file_array_data.name_len);

            SCI_MEMSET(download_instance->save_path, 0, sizeof(download_instance->save_path));

            download_instance->save_path_len = file_array_data.name_len;

            MMI_WSTRNCPY(download_instance->save_path,
                                    MMIFILE_FULL_PATH_MAX_LEN,
                                    file_array_data.filename,
                                    file_array_data.name_len,
                                    file_array_data.name_len);
        }
    }

    //释放列表
    if (PNULL != file_array)
    {
        MMIAPIFILEARRAY_Destroy(&file_array);
        file_array = PNULL;
    }
}

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenLabelCallBack(BOOLEAN is_success, FILEARRAY file_array, MMI_HANDLE_T ctrl_handle)
{
    uint32 file_num = 0;
    FILEARRAY_DATA_T file_array_data = {0};
    MMI_STRING_T path_str = {0};
    MMI_STRING_T dsp_str = {0};
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar dsp_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};

    //SCI_TRACE_LOW:"Download_OpenLabelCallBack is_success = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1248_112_2_18_2_13_39_172,(uint8*)"d", is_success);

    if (is_success)
    {
        file_num = MMIAPIFILEARRAY_GetArraySize(file_array);

        //SCI_TRACE_LOW:"Download_OpenLabelCallBack, file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1254_112_2_18_2_13_39_173,(uint8*)"d", file_num);

        if (file_num > 0 && ( MMIAPIFILEARRAY_Read(file_array, 0, &file_array_data)))
        {
            //read user selected dir

            path_str.wstr_len         = file_array_data.name_len;

            MMI_WSTRNCPY(path, MMIFILE_FULL_PATH_MAX_LEN, file_array_data.filename, file_array_data.name_len, file_array_data.name_len);
            path_str.wstr_ptr = path;
            if (PNULL != ctrl_handle)
            {
                dsp_str.wstr_ptr = dsp_path;
                dsp_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
                MMIDL_GetDisplayPathText(&path_str, dsp_str.wstr_ptr, &dsp_str.wstr_len);
                GUILABEL_SetText(ctrl_handle, &dsp_str, FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
                {
                    MMI_HANDLE_T win_handle = PNULL;
                    DOWNLOAD_APPLET_T *applet = PNULL;

                    applet = MMIDL_APPLETINSTANCE();
                    if (PNULL != applet)
                    {
                        win_handle = MMK_GetWinHandle(MMIDL_HANDLE(applet), MMIDL_DOWNLOAD_SETTINGS_WIN_ID);
                        if (PNULL != win_handle)
                        {
                            SaveDownloadSettings(win_handle);
                        }
                    }
                }
#endif
            }
        }
    }

    //释放列表
    if (PNULL != file_array)
    {
        MMIAPIFILEARRAY_Destroy(&file_array);
        file_array = PNULL;
    }
}

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenSaveAsPathCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
    MMI_HANDLE_T win_handle = PNULL;
    MMI_HANDLE_T label_handle = PNULL;
    MMI_HANDLE_T applet = PNULL;

    applet = MMK_GetFirstAppletHandle();
    if (PNULL == applet)
    {
        return;
    }
    win_handle = MMK_GetWinHandle(applet, MMIDL_DOWNLOAD_SAVE_AS_WIN_ID);
    label_handle = MMK_GetCtrlHandleByWin(win_handle, MMIDL_DOWNLOAD_SAVE_AS_PATH_VALUE_CTRL_ID);

    Download_OpenLabelCallBack(is_success, file_array, label_handle);
}

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenPicturePathCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
    DOWNLOAD_APPLET_T *applet = PNULL;
    MMI_HANDLE_T win_handle = PNULL;
    MMI_HANDLE_T label_handle = PNULL;

    applet = MMIDL_APPLETINSTANCE();
    if (PNULL == applet)
    {
        return;
    }
    win_handle = MMK_GetWinHandle(MMIDL_HANDLE(applet), MMIDL_DOWNLOAD_SETTINGS_WIN_ID);
    label_handle = MMK_GetCtrlHandleByWin(win_handle, MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_CTRL_ID);

    Download_OpenLabelCallBack(is_success, file_array, label_handle);
}

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenMusicPathCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
    DOWNLOAD_APPLET_T *applet = PNULL;
    MMI_HANDLE_T win_handle = PNULL;
    MMI_HANDLE_T label_handle = PNULL;

    applet = MMIDL_APPLETINSTANCE();
    if (PNULL == applet)
    {
        return;
    }
    win_handle = MMK_GetWinHandle(MMIDL_HANDLE(applet), MMIDL_DOWNLOAD_SETTINGS_WIN_ID);
    label_handle = MMK_GetCtrlHandleByWin(win_handle, MMIDL_DOWNLOAD_SAVE_MUSIC_VALUE_CTRL_ID);

    Download_OpenLabelCallBack(is_success, file_array, label_handle);
}

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenVideoPathCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
    DOWNLOAD_APPLET_T *applet = PNULL;
    MMI_HANDLE_T win_handle = PNULL;
    MMI_HANDLE_T label_handle = PNULL;

    applet = MMIDL_APPLETINSTANCE();
    if (PNULL == applet)
    {
        return;
    }
    win_handle = MMK_GetWinHandle(MMIDL_HANDLE(applet), MMIDL_DOWNLOAD_SETTINGS_WIN_ID);
    label_handle = MMK_GetCtrlHandleByWin(win_handle, MMIDL_DOWNLOAD_SAVE_VIDEO_VALUE_CTRL_ID);

    Download_OpenLabelCallBack(is_success, file_array, label_handle);
}

/*****************************************************************************/
// 	Description : Download Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Download_OpenOtherPathCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
    DOWNLOAD_APPLET_T *applet = PNULL;
    MMI_HANDLE_T win_handle = PNULL;
    MMI_HANDLE_T label_handle = PNULL;

    applet = MMIDL_APPLETINSTANCE();
    if (PNULL == applet)
    {
        return;
    }
    win_handle = MMK_GetWinHandle(MMIDL_HANDLE(applet), MMIDL_DOWNLOAD_SETTINGS_WIN_ID);
    label_handle = MMK_GetCtrlHandleByWin(win_handle, MMIDL_DOWNLOAD_SAVE_OTHERS_VALUE_CTRL_ID);

    Download_OpenLabelCallBack(is_success, file_array, label_handle);
}

/*****************************************************************************/
//  Description : handle message of download Save win
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadSaveWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMI_HANDLE_T file_name_handle = 0;
    MMI_HANDLE_T path_handle = 0;
    MMI_STRING_T file_name_value_string = {0};
    MMI_STRING_T path_value_string = {0};
    MMI_HANDLE_T file_name_value_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_FILE_NAME_VALUE_CTRL_ID);
    MMI_HANDLE_T path_value_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_PATH_VALUE_CTRL_ID);
    uint32 task_id = (uint32)MMK_GetWinAddDataPtr(win_id);    
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();
    uint8 *url_ptr = PNULL;
    uint8 *http_header = PNULL;
    uint32 mime_type = 0;
    MMIDL_DOWNLOAD_TYPE download_type = MMIDL_DOWNLOAD_TYPE_MAX;
    wchar full_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};  
    uint16 full_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar file_name[MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN + 1] = {0};
    wchar file_name_ext[MMIDL_FILENAME_EXT_MAX_LEN + 1] = {0};
    uint8 *temp_file_name_ext = PNULL;
    int32 temp_len = 0;
    uint16 file_name_len = 0;
    uint16 file_name_len_ext = 0;
    BOOLEAN is_only_storage = FALSE;

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"HandleDownloadSaveWinMsg download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1423_112_2_18_2_13_39_174,(uint8*)"");

        return MMI_RESULT_FALSE;
    }

    url_ptr = download_instance->download_task[task_id].download_info.url;
    http_header = download_instance->download_task[task_id].download_info.http_header;
    mime_type = download_instance->download_task[task_id].download_info.mime_type;

    if ((mime_type >= MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE) && (mime_type <= MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML))
    {
        is_only_storage = TRUE;
    }

    //SCI_TRACE_LOW:"HandleDownloadSaveWinMsg mime_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1432_112_2_18_2_13_39_175,(uint8*)"d",download_instance->download_task[task_id].download_info.mime_type);
    //SCI_TRACE_LOW:"mmidl_wintab.c:HandleDownloadSaveWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1433_112_2_18_2_13_39_176,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:     
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(MMIDL_DOWNLOAD_SAVE_FILE_FORM_CTRL_ID, GUIFORM_TYPE_TP);
        GUIFORM_SetType(MMIDL_DOWNLOAD_SAVE_PATH_FORM_CTRL_ID, GUIFORM_TYPE_TP);
#endif
        GUIFORM_SetStyle(MMIDL_DOWNLOAD_SAVE_FILE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMIDL_DOWNLOAD_SAVE_PATH_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        file_name_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_FILE_NAME_CTRL_ID);
        path_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_PATH_CTRL_ID);

        GUILABEL_SetIcon(path_value_handle, IMAGE_DL_FOLDER_ICON);
        GUILABEL_SetTextById(file_name_handle, TXT_INPUT_FILE_NAME, FALSE);
        GUILABEL_SetTextById(path_handle, TXT_COMM_FILE_SAVE_TO, FALSE);

        //set default filename
        SCI_MEMSET(full_name, 0x00, sizeof(full_name));
        MMIDL_GetDefaultFileName(full_name, full_name_len, url_ptr, http_header, mime_type);

        MMIDL_SplitFileName(full_name, file_name, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN, file_name_ext, MMIDL_FILENAME_EXT_MAX_LEN);
        SCI_MEMSET(full_name, 0x00, sizeof(full_name));
        file_name_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)file_name);
        file_name_len_ext = (uint16)MMIAPICOM_Wstrlen((const wchar *)file_name_ext);
        MMIAPICOM_Wstrncpy(full_name, file_name, MIN(file_name_len, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN));
        MMIAPICOM_Wstrncpy(full_name + MIN(file_name_len, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN), 
                            file_name_ext, 
                            MIN(file_name_len_ext, MMIDL_FILENAME_EXT_MAX_LEN));

        file_name_value_string.wstr_ptr = (wchar *)full_name;
        file_name_value_string.wstr_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)full_name);

        GUILABEL_SetText(file_name_value_handle, &file_name_value_string, FALSE);

        //Get mime_type from ext first
        temp_len = MMIAPICOM_Wstrlen(file_name_ext) - 1;/* Do not convert the DOT */
        if (temp_len > 0)
        {
            uint16 temp_name_ex_len = 0;
            temp_name_ex_len = (uint16)(temp_len * 3 + 1); /* 0 <= temp_len <= 4 */
            temp_file_name_ext = SCI_ALLOCA(temp_name_ex_len * sizeof(uint8));

            SCI_MEMSET(temp_file_name_ext, 0x00, (temp_name_ex_len * sizeof(uint8)));
            GUI_WstrToUTF8(temp_file_name_ext, temp_name_ex_len, file_name_ext + 1, (uint16)(MMIAPICOM_Wstrlen(file_name_ext) - 1));

            mime_type = mime_ext_to_uint((const char *)temp_file_name_ext);
            if (MIME_TYPE_UNKNOWN == mime_type)
            {
                mime_type = download_instance->download_task[task_id].download_info.mime_type;
#ifdef BROWSER_SUPPORT_NETFRONT
                download_type = MMIDL_GetDownloadType(MMIAPIDL_NFMimeTypeToDLmimeType(mime_type), TRUE);
#else
                download_type = MMIDL_GetDownloadType(MMIAPIDL_NFMimeTypeToDLmimeType(mime_type), FALSE);
#endif
            }
            else
            {
                download_instance->download_task[task_id].download_info.mime_type = mime_type;
                download_type = MMIDL_GetDownloadType(mime_type, FALSE);
            }
            SCI_FREE(temp_file_name_ext);
        }
        else
        {
            mime_type = download_instance->download_task[task_id].download_info.mime_type;
#ifdef BROWSER_SUPPORT_NETFRONT
            download_type = MMIDL_GetDownloadType(MMIAPIDL_NFMimeTypeToDLmimeType(mime_type), TRUE);
#else
            download_type = MMIDL_GetDownloadType(MMIAPIDL_NFMimeTypeToDLmimeType(mime_type), FALSE);
#endif
        }
        //SCI_TRACE_LOW:"HandleDownloadSaveWinMsg mime_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1493_112_2_18_2_13_39_177,(uint8*)"d",download_instance->download_task[task_id].download_info.mime_type);

        //set save path
        SCI_MEMSET(full_name, 0x00, sizeof(full_name));
        full_name_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetDownloadPath(download_type, full_name, &full_name_len);

        //save path to buffer
        download_instance->save_path_len = full_name_len;
        SCI_MEMSET(download_instance->save_path, 0x00, sizeof(download_instance->save_path));
        MMI_WSTRNCPY(download_instance->save_path,
                                MMIFILE_FULL_PATH_MAX_LEN,
                                full_name,
                                full_name_len,
                                full_name_len);

        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_EDIT, STXT_RETURN, FALSE);

        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)))
        {
            s_dev_item_index = MMI_DEVICE_SDCARD;
        }

        MMK_SetAtvCtrl(win_id, file_name_value_handle);        
        break;

    case MSG_FULL_PAINT:
        active_ctrl_id = MMK_GetActiveCtrl(win_id);

        //set path display string
        path_value_string.wstr_ptr = (wchar *)download_instance->save_path;
        path_value_string.wstr_len = (uint16)download_instance->save_path_len;

        {
            if (is_only_storage)
            {
                MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;         
                MMI_TEXT_ID_T  dev_text_id = 0;                 

                dev_text_id = MMIAPIFMM_GetDeviceName(s_dev_item_index);
                MMI_GetLabelTextByLang(dev_text_id, &path_value_string);
                
                switch (s_dev_item_index)
                {
                case MMI_DEVICE_UDISK:
					download_instance->save_path[0] = L'D';
                    break;
                    
                case MMI_DEVICE_SDCARD:
					download_instance->save_path[0] = L'E';
                    break;
                    
#ifdef DUAL_TCARD_SUPPORT
                case MMI_DEVICE_SDCARD_1:
					download_instance->save_path[0] = L'F';
                    break;
#endif
                default:
                    break;
                }
            }
            else
            {
                MMIDL_GetDisplayPathText(&path_value_string,
                    full_name,
                    &full_name_len);

                path_value_string.wstr_ptr = (wchar *)full_name;
                path_value_string.wstr_len = full_name_len;
            }
        }

        GUILABEL_SetText(path_value_handle, &path_value_string, FALSE);

        if (file_name_value_handle == active_ctrl_id)
        {
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_EDIT, STXT_RETURN, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_SPECIFY, STXT_RETURN, FALSE);
        }
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T       ctrl_id = 0;

            if (PNULL != param)
            {
		        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (MSG_CTL_MIDSK == msg_id)
                {
                    ctrl_id = MMK_GetActiveCtrlId(win_id);
                }

                if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                else
                {
                    ctrl_id = MMK_GetActiveCtrl(win_id);

                    if (file_name_value_handle == ctrl_id)
                    {
                        MMIDL_OpenInputFileNameWin(file_name_value_handle);
                    }
                    else
                    {
                        if (is_only_storage)
                        {
                            MMK_CreateWin((uint32 *)MMIDL_DOWNLOAD_SELECT_DEVICE_WIN_TAB, PNULL);
                        }
                        else
                        {
                            MMIDL_AppointDownloadSavePath(download_instance->save_path, download_instance->save_path_len, Download_OpenFileCallBack);
                        }                        
                    }
                }
            }
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        GUILABEL_GetText(file_name_value_handle, &file_name_value_string);
        path_value_string.wstr_ptr = (wchar *)download_instance->save_path;
        path_value_string.wstr_len = (uint16)download_instance->save_path_len;
        if (MMIAPICOM_FileNameIsValid(&file_name_value_string))
        {
            if (MMIDL_CombineDownloadFile(&path_value_string,
                                                            &file_name_value_string,
                                                            full_name,
                                                            &full_name_len,
                                                            TRUE
                                                            ))
            {
                wchar temp_file[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
                uint16 file_len = MMIAPICOM_Wstrlen(full_name);
                uint16 tmp_ext_len = strlen(MMIDL_TMPFILE_EXT);

                SCI_MEMSET(temp_file, 0x00, sizeof(temp_file));
                MMI_WSTRNCPY(temp_file,
                                        MMIFILE_FULL_PATH_MAX_LEN,
                                        full_name,
                                        file_len,
                                        file_len);

                SCI_MEMSET(temp_file + file_len - tmp_ext_len, 0x00, (tmp_ext_len * sizeof(wchar)));//去掉.tmp

                if (!MMIAPIFMM_IsFileExist(full_name, full_name_len) && !MMIAPIFMM_IsFileExist(temp_file, MMIAPICOM_Wstrlen(temp_file)))
                {
                    int32 task_index = 0;

                    //Save Task Info
                    task_index = MMIDL_SaveDownloadTask(download_instance->downoad_manager, url_ptr, full_name, mime_type);
                    //SCI_TRACE_LOW:"HandleDownloadSaveWinMsg task_index=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1590_112_2_18_2_13_39_178,(uint8*)"d", task_index);
                    if (-1 != task_index)
                    {
                        download_instance->download_task[task_id].manager_index = task_index;
                        if (download_instance->download_task[task_id].is_oma_dl)
                        {
#ifdef OMADL_SUPPORT
                            ODM_DOWNLOAD_CONFIRMATION_PARAM_T  cnf_param = {0};

                            cnf_param.is_user_allowed = TRUE;
                            cnf_param.session_id = download_instance->download_task[task_id].session_id;
                            download_instance->downoad_manager->task_info[task_index].status = MMIDL_DOWNLOAD_DOWNLOADING;
                            cnf_param.file_name_len = full_name_len;
                            SCI_MEMCPY(cnf_param.file_name, full_name, sizeof(full_name));
                            ODM_DownloadConfirmationResponse(&cnf_param);
                            MMK_CloseWin(MMK_GetWinHandle(MMIDL_HANDLE(MMIDL_APPLETINSTANCE()), MMIPUB_ALERT_WIN_ID));
#endif
                        }
                        else
                        {
                            //Start Http
                            MMIDL_StartHttpRequest(task_id);
                        }

                        MMIDL_DLTaskListSetFocusItem((uint16)task_index);
                    }
                    else
                    {
                        if (download_instance->download_task[task_id].is_oma_dl)
                        {
#ifdef OMADL_SUPPORT
                            ODM_DOWNLOAD_CONFIRMATION_PARAM_T  cnf_param = {0};

                            cnf_param.is_user_allowed = FALSE;
                            ODM_DownloadConfirmationResponse(&cnf_param);
                            MMK_CloseWin(MMK_GetWinHandle(MMIDL_HANDLE(MMIDL_APPLETINSTANCE()), MMIPUB_ALERT_WIN_ID));
#endif
                        }
                        MMIDL_DestoryDownloadTask(task_id, download_instance);
                        MMIPUB_OpenAlertWarningWin( TXT_DL_TASK_FULL);
                    }
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin( TXT_DC_NAMEREPEAT);
                }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin( TXT_DL_PATH_TOO_LONG);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_DC_NAMEINVALID);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if (!download_instance->download_task[task_id].is_oma_dl) /* destory task in window OMA query*/
        {
#ifdef JAVA_SUPPORT_IA
            if(MIME_TYPE_APPLICATION_JAVA_JAR == download_instance->download_task[task_id].download_info.mime_type)
            {
                MMIAPIJAVA_StopOTAInstall();
            }
#endif
            MMIDL_DestoryDownloadTask(task_id, download_instance);
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        s_dev_item_index = 0;
        break;

    case MSG_APP_RED:
        if (download_instance->download_task[task_id].is_oma_dl)
        {
#ifdef OMADL_SUPPORT
            ODM_DOWNLOAD_CONFIRMATION_PARAM_T  cnf_param = {0};

            cnf_param.is_user_allowed = FALSE;
            ODM_DownloadConfirmationResponse(&cnf_param);
#endif
        }

        MMIDL_DestoryDownloadTask(task_id, download_instance);
        recode = MMI_RESULT_FALSE;
        break;

    default:
        //SCI_TRACE_LOW:"mmidl_wintab.c:HandleDownloadSaveWinMsg() msg_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1683_112_2_18_2_13_40_179,(uint8*)"d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle message of download Save As win
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadSaveAsWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMIDL_DOWNLOAD_TYPE download_type = MMIDL_DOWNLOAD_TYPE_MAX;
    MMI_HANDLE_T file_name_handle = 0;
    MMI_HANDLE_T path_handle = 0;
    MMI_STRING_T file_name_value_string = {0};
    MMI_STRING_T path_value_string = {0};
    MMI_HANDLE_T file_name_value_handle = PNULL;
    MMI_HANDLE_T path_value_handle = PNULL; 
    MMIDL_DOWNLOAD_SAVE_AS_INFO_T  *save_as_info = PNULL;
    wchar full_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar file_name[MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN + 1] = {0};
    wchar file_name_ext[MMIDL_FILENAME_EXT_MAX_LEN + 1] = {0};
    uint8 *temp_file_name_ext = PNULL;
    int32 temp_len = 0;
    SFS_ERROR_E error = SFS_NO_ERROR;
    uint32 mime_type = MIME_TYPE_UNKNOWN;
    uint32 bytes_written = 0;
    MMIFILE_HANDLE file_handle = PNULL;
    MMI_STRING_T path_str = {0};
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0}; 
    uint16 file_name_len = 0;
    uint16 file_name_len_ext = 0;

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    file_name_value_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_VALUE_CTRL_ID);
    path_value_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_AS_PATH_VALUE_CTRL_ID);  

    save_as_info = (MMIDL_DOWNLOAD_SAVE_AS_INFO_T *)MMK_GetWinAddDataPtr(win_id);

    //SCI_TRACE_LOW:"mmidl_wintab.c:HandleDownloadSaveAsWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1730_112_2_18_2_13_40_180,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMIDL_DOWNLOAD_SAVE_AS_FILE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMIDL_DOWNLOAD_SAVE_AS_PATH_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        file_name_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_AS_FILE_NAME_CTRL_ID);
        path_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_AS_PATH_CTRL_ID);

        GUILABEL_SetTextById(file_name_handle, TXT_INPUT_FILE_NAME, FALSE);
        GUILABEL_SetTextById(path_handle, TXT_COMM_FILE_SAVE_TO, FALSE);

        SCI_MEMSET(full_name, 0x00, sizeof(full_name));
        MMIDL_GetDefaultFileName(full_name, full_name_len, save_as_info->url, PNULL, save_as_info->mime_type);

        MMIDL_SplitFileName(full_name, file_name, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN, file_name_ext, MMIDL_FILENAME_EXT_MAX_LEN);
        SCI_MEMSET(full_name, 0x00, sizeof(full_name));
        file_name_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)file_name);
        file_name_len_ext = (uint16)MMIAPICOM_Wstrlen((const wchar *)file_name_ext);
        MMIAPICOM_Wstrncpy(full_name, file_name, MIN(file_name_len, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN));
        MMIAPICOM_Wstrncpy(full_name + MIN(file_name_len, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN), 
                            file_name_ext, 
                            MIN(file_name_len_ext, MMIDL_FILENAME_EXT_MAX_LEN));

        file_name_value_string.wstr_ptr = (wchar *)full_name;
        file_name_value_string.wstr_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)full_name);

        GUILABEL_SetText(file_name_value_handle, &file_name_value_string, FALSE);

        temp_len = MMIAPICOM_Wstrlen(file_name_ext) - 1;
        if (temp_len > 0) 
        {
            uint16 temp_name_ex_len = 0;

            temp_name_ex_len = (uint16)(temp_len * 3 + 1);
            temp_file_name_ext = SCI_ALLOCA(temp_name_ex_len * sizeof(uint8));

            SCI_MEMSET(temp_file_name_ext, 0x00, (temp_name_ex_len * sizeof(uint8)));
            GUI_WstrToUTF8(temp_file_name_ext, temp_name_ex_len, (file_name_ext + 1), (uint16)(MMIAPICOM_Wstrlen(file_name_ext) - 1));

            mime_type = mime_ext_to_uint((const char *)temp_file_name_ext);
            if (MIME_TYPE_UNKNOWN == mime_type)
            {
                mime_type = save_as_info->mime_type;
#ifdef BROWSER_SUPPORT_NETFRONT
                download_type = MMIDL_GetDownloadType(MMIAPIDL_NFMimeTypeToDLmimeType(mime_type), TRUE);
#else
                download_type = MMIDL_GetDownloadType(MMIAPIDL_NFMimeTypeToDLmimeType(mime_type), FALSE);
#endif
            }
            else
            {
                download_type = MMIDL_GetDownloadType(mime_type, FALSE);
            }
            SCI_FREE(temp_file_name_ext);
        }
        else
        {
            mime_type = save_as_info->mime_type;
#ifdef BROWSER_SUPPORT_NETFRONT
            download_type = MMIDL_GetDownloadType(MMIAPIDL_NFMimeTypeToDLmimeType(mime_type), TRUE);
#else
            download_type = MMIDL_GetDownloadType(MMIAPIDL_NFMimeTypeToDLmimeType(mime_type), FALSE);
#endif
        }

        SCI_MEMSET(full_name, 0x00, sizeof(full_name));
        full_name_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetDownloadPath(download_type, full_name, &full_name_len);
        file_name_value_string.wstr_ptr = (wchar *)full_name;
        file_name_value_string.wstr_len = full_name_len;

        path_str.wstr_ptr = path;
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetDisplayPathText(&file_name_value_string, path_str.wstr_ptr, &path_str.wstr_len);
        GUILABEL_SetText(path_value_handle, &path_str, FALSE);
        GUILABEL_SetIcon(path_value_handle, IMAGE_DL_FOLDER_ICON);
        MMK_SetAtvCtrl(win_id, file_name_value_handle);
        break;

    case MSG_FULL_PAINT:
        active_ctrl_id = MMK_GetActiveCtrl(win_id);
        if (file_name_value_handle == active_ctrl_id)
        {
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_EDIT, STXT_RETURN, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_SPECIFY, STXT_RETURN, FALSE);
        }
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        active_ctrl_id = MMK_GetActiveCtrl(win_id);

        if (file_name_value_handle == active_ctrl_id)
        {
            MMIDL_OpenInputFileNameWinEx(MMK_GetFirstAppletHandle(), file_name_value_handle);
        }
        else
        {
            GUILABEL_GetText(path_value_handle, &path_value_string);
            path_str.wstr_ptr = path;
            path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
            MMIDL_GetRealPathText(&path_value_string, path_str.wstr_ptr, &path_str.wstr_len);
            MMIDL_AppointDownloadSavePath(path_str.wstr_ptr, path_str.wstr_len, Download_OpenSaveAsPathCallBack);
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        GUILABEL_GetText(file_name_value_handle, &file_name_value_string);
        GUILABEL_GetText(path_value_handle, &path_value_string);
        path_str.wstr_ptr = path;
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetRealPathText(&path_value_string, path_str.wstr_ptr, &path_str.wstr_len);
        
        if (MMIAPICOM_FileNameIsValid(&file_name_value_string))
        {
            if (MMIDL_CombineDownloadFile(&path_str,
                                                            &file_name_value_string,
                                                            full_name,
                                                            &full_name_len,
                                                            FALSE
                                                            ))
            {
                if (!MMIAPIFMM_IsFileExist(full_name, full_name_len))
                {
                    MMK_CloseWin(win_id);
                    file_handle = SFS_CreateFile(full_name, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS,0 , 0);/*lint !e655*/
                    if( SFS_INVALID_HANDLE != file_handle )
                    {
                        error = MMIAPIFMM_WriteFile(file_handle, save_as_info->buf, save_as_info->buf_len, &bytes_written, PNULL);
                        if (SFS_NO_ERROR == error)
                        {
                            MMIAPIFMM_CloseFile(file_handle);
                            MMIPUB_OpenAlertSuccessWin( TXT_COMM_SAVE_SUCCESS);
                        }
                        else
                        {
                            MMIAPIFMM_CloseFile(file_handle);
                            SFS_DeleteFile(full_name,PNULL);
                            MMIPUB_OpenAlertWarningWin( TXT_COMM_NO_SPACE);
                        }
                    } 
                    else 
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_ERROR);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin( TXT_DC_NAMEREPEAT);
                }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin( TXT_DL_PATH_TOO_LONG);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_DC_NAMEINVALID);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != save_as_info->buf)
        {
            SCI_FREE(save_as_info->buf);
        }
        if (PNULL != save_as_info->url)
        {
            SCI_FREE(save_as_info->url);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : handle message of input download file name
//	Global resource dependence : none
//  Author: wancan.you
//	Note: Do Not use applet instance in this func 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputDownloadFileName(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_HANDLE_T ctrl_handle = PNULL;
    MMI_HANDLE_T label_handle =  PNULL;
    MMI_STRING_T name_str = {0};
    wchar file_name[MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN + 1] = {0};
    static wchar file_name_ext[MMIDL_FILENAME_EXT_MAX_LEN + 1] = {0};
    wchar full_name[MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN + MMIDL_FILENAME_EXT_MAX_LEN + 1] = {0};  
    uint16 file_name_len = 0;

    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_INPUT_FILENAME_CTRL_ID);
    label_handle = (MMI_HANDLE_T)MMK_GetWinAddDataPtr(win_id);

    if (PNULL == label_handle)
    {
        return MMI_RESULT_FALSE;
    }

    //SCI_TRACE_LOW:"mmidl_wintab.c:HandleInputDownloadFileName() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_1942_112_2_18_2_13_40_181,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetTextMaxLen(ctrl_handle, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN);//TODO
        SCI_MEMSET(file_name_ext, 0x00, sizeof(file_name_ext));
        GUILABEL_GetText(label_handle, &name_str);
        MMIDL_SplitFileName(name_str.wstr_ptr, file_name, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN, file_name_ext, MMIDL_FILENAME_EXT_MAX_LEN);
        // set file name
        file_name_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)file_name);
        if (file_name_len > 0)
        {
            GUIEDIT_SetString(ctrl_handle, file_name, file_name_len);
        }

        //更改softkey显示
        GUIEDIT_SetSoftkey(
                                        ctrl_handle,
                                        0,
                                        1,
                                        TXT_NULL,
                                        TXT_COMMON_OK,
                                        PNULL
                                        );

        GUIEDIT_SetSoftkey(
                                        ctrl_handle,
                                        2,
                                        1,
                                        STXT_RETURN,
                                        TXT_DELETE,
                                        PNULL
                                        );
        
        MMK_SetAtvCtrl(win_id, ctrl_handle);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_SK_SKB_ICON_PRESSED:
        GUIEDIT_GetString(ctrl_handle, &name_str);
        if (0 == name_str.wstr_len)
        {
            break;
        }

        MMIAPICOM_Wstrncpy(full_name, name_str.wstr_ptr, MIN(name_str.wstr_len, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN));
        file_name_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)file_name_ext);
        MMIAPICOM_Wstrncpy(full_name + MIN(name_str.wstr_len, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN), 
                            file_name_ext, 
                            MIN(file_name_len, MMIDL_FILENAME_EXT_MAX_LEN));
        name_str.wstr_ptr = full_name;
        name_str.wstr_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)full_name);
        name_str.wstr_len = (uint16)MIN(name_str.wstr_len, MMIDL_DOWNLOAD_DOWNLOAD_NAME_LEN + MMIDL_FILENAME_EXT_MAX_LEN);
        if (!MMIAPICOM_FileNameIsValid(&name_str))  
        {
            MMIPUB_OpenAlertWarningWin( TXT_DC_NAMEINVALID);
            break;
        }

        GUILABEL_SetText(label_handle, &name_str, FALSE);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : handle message of download manager win
//  Global resource dependence : none
//  Author: wancna.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadSelectDeviceWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIDL_DOWNLOAD_SELECT_DEVICE_LIST_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMIAPIFMM_GetAllAvarialeDiskNum(), FALSE );//max item s2
        MMIAPISET_AppendListItemByTextId(TXT_COMMON_UDISK, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#ifndef DUAL_TCARD_SUPPORT
        MMIAPISET_AppendListItemByTextId(TXT_COMMON_SD_CARD, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#else
        MMIAPISET_AppendListItemByTextId(TXT_COMMON_SD_CARD0, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId(TXT_COMMON_SD_CARD1, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#endif
        GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, 0); 

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        s_dev_item_index = GUILIST_GetCurItemIndex(ctrl_id) + 1;
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_APP_MENU:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle message of download manager win
//  Global resource dependence : none
//  Author: wancna.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadManagerWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_MANAGER_LIST_CTRL_ID);
    uint32 task_index = 0;
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = (MMIDL_DOWNLOAD_MANAGER_T *)MMIDL_GetDownloadManager();

    //SCI_TRACE_LOW:"mmidl_wintab.c:HandleDownloadManagerWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_2037_112_2_18_2_13_40_182,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_handle);
        break;

    case MSG_FULL_PAINT:
        if (PNULL != dl_manager && dl_manager->total_task_num > 0)
        {            
            GUILIST_SetTitleIndexType(ctrl_handle, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType(ctrl_handle, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }
        MMIDL_AppendDLTaskListItem(win_id, ctrl_handle);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        if (!MMIDL_IsNeedDownLoadTask(MMIDL_GetDownloadManager()))
        {
            MMIDL_StopDownloadApplet();
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_APP_MENU:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        task_index = MMIDL_GetDownloadManagerTaskIndex(GUILIST_GetCurItemIndex(ctrl_handle));

        MMIDL_OpenDownloadManagerOptionWin(task_index);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
// Description : update download manager Win
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC void MMIDL_UpdateDownloadManagerWin(void)
{
    MMI_HANDLE_T win_handle = PNULL;

    win_handle = MMIDL_GetDownloadManagerWinHandle();

    if (MMK_IsFocusWin(win_handle))
    {
        MMK_SendMsg(win_handle, MSG_FULL_PAINT, PNULL);
    }
}

/*****************************************************************************/
// Description : Get manager win handle
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIDL_GetDownloadManagerWinHandle(void)
{
    DOWNLOAD_APPLET_T *applet = PNULL;
    MMI_HANDLE_T win_handle = PNULL;

    applet = MMIDL_APPLETINSTANCE();
    if (PNULL != applet)
    {
        win_handle = MMK_GetWinHandle(MMIDL_HANDLE(applet), MMIDL_DOWNLOAD_MANAGER_WIN_ID);
    }
    return win_handle;
}

/*****************************************************************************/
//  Description : handle messages of download manager option menu win
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadManagerOptWinMsg(MMI_WIN_ID_T win_id,
                                                                                                          MMI_MESSAGE_ID_E msg_id,
                                                                                                          DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_MENU_ID_T menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    uint32 task_index = (uint32)MMK_GetWinAddDataPtr(win_id);
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;
    dl_manager = MMIDL_GetDownloadManager();
    if(PNULL == dl_manager)
    {
        return recode;
    }
    //SCI_TRACE_LOW:"mmidl_wintab.c:HandleDownloadManagerOptWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_2138_112_2_18_2_13_40_183,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetDownloadManagerOptionGray(win_id, task_index);
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID));
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:  
        GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID),&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_DL_MANAGER_OPT_PAUSE:
            MMIDL_PauseDownload(dl_manager, task_index);
            MMK_CloseWin(win_id);
            break;

        case ID_DL_MANAGER_OPT_RESUME:
            MMIDL_ResumeDownload(dl_manager, task_index);
            MMK_CloseWin(win_id);
            break;

        case ID_DL_MANAGER_OPT_REDOWNLOAD:
            MMIPUB_OpenQueryWinByTextId(TXT_DL_QURERY_RE_DOWNLOAD, IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;

        case ID_DL_MANAGER_OPT_DEL:
            MMIPUB_OpenQueryWinByTextId(TXT_COMMON_CONFIRM_DELETE, IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;

        case ID_DL_MANAGER_OPT_DEL_ALL:
            MMIPUB_OpenQueryWinByTextId(TXT_DL_QURERY_DEL_ALL_TASK, IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;

        case ID_DL_MANAGER_OPT_DETAIL:
            MMIDL_OpenDownloadTaskDetailWin(task_index);
            MMK_CloseWin(win_id);
            break;

        case ID_DL_MANAGER_OPT_FILE_EXPLORER:
            MMIDL_OpenFileExplorer(task_index);
            MMK_CloseWin(win_id);
            break;

        case ID_DL_MANAGER_OPT_SETTINGS:
            MMIDL_OpenDownloadSettingsWin();
            MMK_CloseWin(win_id);
            break;

        default:
            //SCI_TRACE_LOW:"HandleDownloadManagerOptWinMsg() not handled menu"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_2193_112_2_18_2_13_41_184,(uint8*)"");
            break;
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_OK:
        GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID),&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_DL_MANAGER_OPT_REDOWNLOAD:
            MMIDL_DeleteDownloadTask(task_index);
            MMIDL_ReDownload(dl_manager, task_index);
            break;

        case ID_DL_MANAGER_OPT_DEL:
            MMIDL_DeleteDownloadManagerTask(dl_manager, task_index, FALSE);
            MMIDL_DeleteDownloadTask(task_index);
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            break;

        case ID_DL_MANAGER_OPT_DEL_ALL:
            MMIDL_DeleteDownloadManagerTask(dl_manager, 0, TRUE);
            MMIDL_StopDownload(0, TRUE);
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            break;

        default:
            break;
        }
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
#if 0
        // NOTE: added by hua.fang
        // grayed property is set in each menu object, now, menu object is
        // destroyed, we need not set FALSE here.
        //
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_PAUSE, FALSE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_RESUME, FALSE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_REDOWNLOAD, FALSE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_DEL, FALSE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_DEL_ALL, FALSE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_DETAIL, FALSE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_FILE_EXPLORER, FALSE);
#endif
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Set Download Manager Option Menu Gray
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void SetDownloadManagerOptionGray(MMI_WIN_ID_T win_id, uint32 task_index)
{
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;
    uint32 total_task_num = 0;
    dl_manager = MMIDL_GetDownloadManager();
    
    if (task_index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    total_task_num = MMIDL_GetDownloadManagerTotalTaskNum(dl_manager);
    //SCI_TRACE_LOW:"SetDownloadManagerOptionGray index = %d, total_task_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_2277_112_2_18_2_13_41_185,(uint8*)"dd", task_index, total_task_num);

    MMIDL_GetManagerTaskInfo(dl_manager, task_index, &task_info);
    //SCI_TRACE_LOW:"SetDownloadManagerOptionGray status= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_2280_112_2_18_2_13_41_186,(uint8*)"d", task_info.status);

    switch (task_info.status)
    {
    case MMIDL_DOWNLOAD_DOWNLOADING:
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_RESUME, TRUE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_REDOWNLOAD, TRUE);
        break;

    case MMIDL_DOWNLOAD_DOWNLOADED:
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_PAUSE, TRUE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_RESUME, TRUE);
        break;

    case MMIDL_DOWNLOAD_FAILED:
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_PAUSE, TRUE);
        break;

    case MMIDL_DOWNLOAD_WAITING:
    case MMIDL_DOWNLOAD_WAITING_HTTP:
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_PAUSE, TRUE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_RESUME, TRUE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_REDOWNLOAD, TRUE);
        break;

    case MMIDL_DOWNLOAD_PAUSED:
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_PAUSE, TRUE);
        break;

    default:
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_PAUSE, TRUE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_RESUME, TRUE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_REDOWNLOAD, TRUE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_DEL, TRUE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_DEL_ALL, TRUE);
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_DETAIL, TRUE);
        break;
    }

    if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        GUIMENU_SetItemGrayed(MMIDL_DOWNLOAD_MANAGER_OPT_MENU_CTRL_ID, MENU_DL_MANAGET_OPT, ID_DL_MANAGER_OPT_FILE_EXPLORER, TRUE);
    }
}

/*****************************************************************************/
//  Description : handle message of download task detail
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadTaskDetailWinMsg(MMI_WIN_ID_T win_id,
                                                                                                          MMI_MESSAGE_ID_E msg_id,
                                                                                                          DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 task_index = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIDL_AppendDLTaskDetail(win_id, MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_TASK_DETAIL_CTRL_ID), task_index);
            MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_TASK_DETAIL_CTRL_ID));
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        break;
        
    default:
        //SCI_TRACE_LOW:"HandleDownloadTaskDetailWinMsg() msg_id=0x%04x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_2358_112_2_18_2_13_41_187,(uint8*)"d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQueryUseWlanWinMsg(MMI_WIN_ID_T win_id,
                                                                                                          MMI_MESSAGE_ID_E msg_id,
                                                                                                          DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(win_id);
        MMIDL_HandlePDP_Change();
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : handle message of download query
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadQueryWinMsg(MMI_WIN_ID_T win_id,
                                                                                                          MMI_MESSAGE_ID_E msg_id,
                                                                                                          DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 task_index = (uint32)MMK_GetWinAddDataPtr(win_id);

    //SCI_TRACE_LOW:"mmidl_wintab.c:HandleDownloadQueryWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_2498_112_2_18_2_13_41_189,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_HANDLE_T mgr_win_handle = MMIDL_GetDownloadManagerWinHandle();

            if (PNULL != mgr_win_handle)
            {
                MMK_SendMsg(mgr_win_handle, MSG_LOSE_FOCUS, PNULL); /* mmk MSG 问题对应 */
            }
            MMIPUB_OpenQueryWinByTextId(TXT_DL_TASK_REPEAT, IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMIDL_DeleteDownloadTask(task_index);
        MMIDL_ReDownload(MMIDL_GetDownloadManager(), task_index);
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
        
    default:
        //SCI_TRACE_LOW:"HandleDownloadQueryWinMsg() msg_id=0x%04x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_2527_112_2_18_2_13_41_190,(uint8*)"d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle message of download settings
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadSettingsWinMsg(MMI_WIN_ID_T win_id,
                                                                                                          MMI_MESSAGE_ID_E msg_id,
                                                                                                          DPARAM param)
{   
    MMI_STRING_T path_str = {0};
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIDL_OpenFileCallBack OpenFileCallBack = PNULL;

    //SCI_TRACE_LOW:"mmidl_wintab.c:HandleDownloadQueryWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_WINTAB_2547_112_2_18_2_13_41_191,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set download settings
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_LIST_CTRL_ID));
#else
        SetDownloadSettings(win_id);
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_METHOD_LIST_CTRL_ID));
#endif
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_FULL_PAINT:
        SetDownloadSettings(win_id);
        break;
#endif

#ifdef MMI_GUI_STYLE_TYPICAL
    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        SetDownloadPath(win_id);
        break;
#endif

    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef MMI_GUI_STYLE_TYPICAL
        SaveDownloadSettings(win_id);
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
            {
                MMI_HANDLE_T active_ctrl = MMK_GetActiveCtrl(win_id);
                MMI_HANDLE_T pic_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_CTRL_ID);
                MMI_HANDLE_T music_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_MUSIC_VALUE_CTRL_ID);
                MMI_HANDLE_T video_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_VIDEO_VALUE_CTRL_ID);
                MMI_HANDLE_T others_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_OTHERS_VALUE_CTRL_ID);
                MMI_STRING_T real_path_str = {0};
                wchar real_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};

                if (pic_value_ctrl == active_ctrl)
                {
                    OpenFileCallBack = Download_OpenPicturePathCallBack;
                }
                else if (music_value_ctrl == active_ctrl)
                {
                    OpenFileCallBack = Download_OpenMusicPathCallBack;
                }
                else if (video_value_ctrl == active_ctrl)
                {
                    OpenFileCallBack = Download_OpenVideoPathCallBack;
                }
                else if (others_value_ctrl == active_ctrl)
                {
                    OpenFileCallBack = Download_OpenOtherPathCallBack;
                }
                else
                {
#ifndef MMI_GUI_STYLE_TYPICAL
                    uint32              index = 0;
                    SETTING_ITEM_FUNC   item_func = PNULL;

                    index = GUILIST_GetCurItemIndex(active_ctrl);
                    item_func = (SETTING_ITEM_FUNC)DlGetListUserdata(active_ctrl, index);

                    if (PNULL != item_func)
                    {
                        (*item_func)();
                        break;
                    }
#else
                    OpenFileCallBack = PNULL;
#endif
                }
                if (PNULL != OpenFileCallBack)
                {
                    real_path_str.wstr_ptr = real_path;
                    real_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
                    GUILABEL_GetText(active_ctrl, &path_str);
                    MMIDL_GetRealPathText(&path_str, real_path_str.wstr_ptr, &real_path_str.wstr_len);
                    MMIDL_AppointDownloadSavePath(real_path_str.wstr_ptr, real_path_str.wstr_len, OpenFileCallBack);
                }
            }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_NOTIFY_GET_ACTIVE:
        {
            MMI_HANDLE_T active_ctrl = MMK_GetActiveCtrl(win_id);
#ifndef MMI_GUI_STYLE_TYPICAL
            MMI_HANDLE_T list_ctrl =  MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_LIST_CTRL_ID);

            if (list_ctrl == active_ctrl)
            {
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_OK, STXT_SELECT, STXT_RETURN, TRUE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_OK, TXT_SPECIFY, STXT_RETURN, TRUE);
            }
#else
            MMI_HANDLE_T method_ctrl =  MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_METHOD_LIST_CTRL_ID);
            MMI_HANDLE_T path_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_PATH_LIST_CTRL_ID);

            if (path_ctrl == active_ctrl || method_ctrl == active_ctrl)
            {
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, STXT_SELECT, STXT_RETURN, TRUE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_SPECIFY, STXT_RETURN, TRUE);
            }
#endif
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : set Download settings param
//	Global resource dependence : 
//  Author: Jiaoyou.wu
//	Note: 
/*****************************************************************************/
LOCAL void SetDownloadPath(MMI_WIN_ID_T win_id)
{
    MMI_HANDLE_T form_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_FORM_CTRL_ID);
    MMI_HANDLE_T pic_form_ctrl =  MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_PICTURE_FORM_CTRL_ID);
    MMI_HANDLE_T pic_name_ctrl =  MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_PICTURE_NAME_CTRL_ID);
    MMI_HANDLE_T pic_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_CTRL_ID);
    MMI_HANDLE_T music_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_MUSIC_FORM_CTRL_ID);
    MMI_HANDLE_T music_name_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_MUSIC_NAME_CTRL_ID);
    MMI_HANDLE_T music_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_MUSIC_VALUE_CTRL_ID);
    MMI_HANDLE_T video_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_VIDEO_FORM_CTRL_ID);
    MMI_HANDLE_T video_name_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_VIDEO_NAME_CTRL_ID);
    MMI_HANDLE_T video_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_VIDEO_VALUE_CTRL_ID);
    MMI_HANDLE_T others_form_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_OTHERS_FORM_CTRL_ID);
    MMI_HANDLE_T others_name_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_OTHERS_NAME_CTRL_ID);
    MMI_HANDLE_T others_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_OTHERS_VALUE_CTRL_ID);
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_HANDLE_T path_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_PATH_LIST_CTRL_ID);
#endif
    MMI_HANDLE_T active_ctrl = MMK_GetActiveCtrl(win_id);
    MMI_STRING_T path_str = {0};
    MMI_STRING_T dsp_path_str = {0};
    uint16 cur_index = 0;
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar dsp_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
#ifndef MMI_GUI_STYLE_TYPICAL
    MMIDL_DOWNLOAD_PATH_TYPE path_type = MMIDL_DOWNLOAD_PATH_DEFAULT;
#endif

    path_str.wstr_ptr = path;
    path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    dsp_path_str.wstr_ptr = dsp_path;
    dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;


    //set name
    GUILABEL_SetTextById(pic_name_ctrl, TXT_COMMON_PIC, FALSE);
    GUILABEL_SetTextById(music_name_ctrl, TXT_COMMON_MUSIC, FALSE);
    GUILABEL_SetTextById(video_name_ctrl, TXT_VIDEO, FALSE);
    GUILABEL_SetTextById(others_name_ctrl, TXT_TOOLS_OTHERS, FALSE);

#ifndef MMI_GUI_STYLE_TYPICAL
    MMIDL_ReadDownloadPathType(&path_type);
    cur_index = path_type;
#else
    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(path_list_ctrl);
#endif

    //Set value
    if (MMIDL_DOWNLOAD_PATH_CUSTOM == (MMIDL_DOWNLOAD_PATH_TYPE)cur_index)
    {
        GUILABEL_SetIcon(pic_value_ctrl, IMAGE_DL_FOLDER_ICON);
        GUIFORM_SetChildDisplay(form_handle, pic_form_ctrl, GUIFORM_CHILD_DISP_NORMAL);

        GUILABEL_SetIcon(music_value_ctrl, IMAGE_DL_FOLDER_ICON);
        GUIFORM_SetChildDisplay(form_handle, music_form_ctrl, GUIFORM_CHILD_DISP_NORMAL);

        GUILABEL_SetIcon(video_value_ctrl, IMAGE_DL_FOLDER_ICON);
        GUIFORM_SetChildDisplay(form_handle, video_form_ctrl, GUIFORM_CHILD_DISP_NORMAL);

        GUILABEL_SetIcon(others_value_ctrl, IMAGE_DL_FOLDER_ICON);
        GUIFORM_SetChildDisplay(form_handle, others_form_ctrl, GUIFORM_CHILD_DISP_NORMAL);

        SCI_MEMSET(path, 0x0, sizeof(path));
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetCustomizedDownloadPath(MMIDL_DOWNLOAD_PICTURE_TYPE, path_str.wstr_ptr, &path_str.wstr_len);
        MMIDL_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
        GUILABEL_SetText(pic_value_ctrl, &dsp_path_str, FALSE);

        SCI_MEMSET(path, 0x0, sizeof(path));
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetCustomizedDownloadPath(MMIDL_DOWNLOAD_AUDIO_TYPE, path_str.wstr_ptr, &path_str.wstr_len);
        MMIDL_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
        GUILABEL_SetText(music_value_ctrl, &dsp_path_str, FALSE);

        SCI_MEMSET(path, 0x0, sizeof(path));
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetCustomizedDownloadPath(MMIDL_DOWNLOAD_VIDEO_TYPE, path_str.wstr_ptr, &path_str.wstr_len);
        MMIDL_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
        GUILABEL_SetText(video_value_ctrl, &dsp_path_str, FALSE);

        SCI_MEMSET(path, 0x0, sizeof(path));
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetCustomizedDownloadPath(MMIDL_DOWNLOAD_OTHER_TYPE, path_str.wstr_ptr, &path_str.wstr_len);
        MMIDL_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
        GUILABEL_SetText(others_value_ctrl, &dsp_path_str, FALSE);
    }
    else
    {
#ifdef MMI_GUI_STYLE_TYPICAL
		GUIFORM_CHILD_DISPLAY_E  display_type = GUIFORM_CHILD_DISP_NORMAL;
#else
		GUIFORM_CHILD_DISPLAY_E  display_type = GUIFORM_CHILD_DISP_HIDE;
#endif
        //set value
        GUILABEL_SetIcon(pic_value_ctrl, IMAGE_DL_FOLDER_ICON_DISABLED);
        GUIFORM_SetChildDisplay(form_handle, pic_form_ctrl, display_type);

        GUILABEL_SetIcon(music_value_ctrl, IMAGE_DL_FOLDER_ICON_DISABLED);
        GUIFORM_SetChildDisplay(form_handle, music_form_ctrl, display_type);

        GUILABEL_SetIcon(video_value_ctrl, IMAGE_DL_FOLDER_ICON_DISABLED);
        GUIFORM_SetChildDisplay(form_handle, video_form_ctrl, display_type);

        GUILABEL_SetIcon(others_value_ctrl, IMAGE_DL_FOLDER_ICON_DISABLED);
        GUIFORM_SetChildDisplay(form_handle, others_form_ctrl, display_type);
        
        SCI_MEMSET(path, 0x0, sizeof(path));
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetDefaultDownloadPath(MMIDL_DOWNLOAD_PICTURE_TYPE, path_str.wstr_ptr, &path_str.wstr_len);
        MMIDL_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
        GUILABEL_SetText(pic_value_ctrl, &dsp_path_str, FALSE);

        SCI_MEMSET(path, 0x0, sizeof(path));
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetDefaultDownloadPath(MMIDL_DOWNLOAD_AUDIO_TYPE, path_str.wstr_ptr, &path_str.wstr_len);
        MMIDL_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
        GUILABEL_SetText(music_value_ctrl, &dsp_path_str, FALSE);

        SCI_MEMSET(path, 0x0, sizeof(path));
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetDefaultDownloadPath(MMIDL_DOWNLOAD_VIDEO_TYPE, path_str.wstr_ptr, &path_str.wstr_len);
        MMIDL_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
        GUILABEL_SetText(video_value_ctrl, &dsp_path_str, FALSE);

        SCI_MEMSET(path, 0x0, sizeof(path));
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIDL_GetDefaultDownloadPath(MMIDL_DOWNLOAD_OTHER_TYPE, path_str.wstr_ptr, &path_str.wstr_len);
        MMIDL_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
        GUILABEL_SetText(others_value_ctrl, &dsp_path_str, FALSE);
    }
    
    if (pic_value_ctrl == active_ctrl || music_value_ctrl == active_ctrl || video_value_ctrl == active_ctrl || others_value_ctrl == active_ctrl)
    {
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OK, TXT_SPECIFY, STXT_RETURN, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_SPECIFY, STXT_RETURN, FALSE);
#endif

    }
    else
    {
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OK, STXT_SELECT, STXT_RETURN, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, STXT_SELECT, STXT_RETURN, FALSE);
#endif

    }
}

/*****************************************************************************/
// 	Description : set Download settings param
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
LOCAL void SetDownloadSettings(MMI_WIN_ID_T win_id)
{
#ifdef MMI_GUI_STYLE_TYPICAL
    uint16 cur_index = 0;
    uint32 i = 0;
    MMI_STRING_T method_item[MMIDL_DOWNLOAD_METHOD_TYPE_MAX] = {0};
    MMI_STRING_T path_item[MMIDL_DOWNLOAD_PATH_TYPE_MAX] = {0};
#endif
    MMI_TEXT_ID_T method_text[MMIDL_DOWNLOAD_METHOD_TYPE_MAX] = {TXT_DL_SETTINGS_METHOD_APPOSITE, TXT_DL_SETTINGS_METHOD_QUEUE};
    MMI_TEXT_ID_T path_text[MMIDL_DOWNLOAD_PATH_TYPE_MAX] = {TXT_DL_SETTINGS_PATH_DEFAULT, TXT_EXPORT_LOC_CUSTOM};
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_HANDLE_T method_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_METHOD_LIST_CTRL_ID);
    MMI_HANDLE_T method_name_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_METHOD_LABEL_CTRL_ID);
    MMI_HANDLE_T path_name_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_PATH_LABEL_CTRL_ID);
    MMI_HANDLE_T path_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_PATH_LIST_CTRL_ID);
#endif
    MMIDL_DOWNLOAD_METHOD_TYPE method_type = MMIDL_DOWNLOAD_METHOD_APPOSITE;
    MMIDL_DOWNLOAD_PATH_TYPE path_type = MMIDL_DOWNLOAD_PATH_DEFAULT;

#ifdef MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetStyle(MMIDL_DOWNLOAD_SETTINGS_METHOD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIDL_DOWNLOAD_SETTINGS_PATH_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
    GUIFORM_SetStyle(MMIDL_DOWNLOAD_SAVE_PICTURE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIDL_DOWNLOAD_SAVE_MUSIC_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIDL_DOWNLOAD_SAVE_VIDEO_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIDL_DOWNLOAD_SAVE_OTHERS_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMIDL_ReadDownloadMethodType(&method_type);
    MMIDL_ReadDownloadPathType(&path_type);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetType(MMIDL_DOWNLOAD_SETTINGS_FORM_CTRL_ID, GUIFORM_TYPE_TP);

    GUILIST_SetMaxItem(MMIDL_DOWNLOAD_SETTINGS_LIST_CTRL_ID, 2, FALSE);

    GUILIST_SetSlideState(MMIDL_DOWNLOAD_SETTINGS_LIST_CTRL_ID, FALSE); 

    /* Method */
    AppendTwoTextOneImageItem(MMIDL_DOWNLOAD_SETTINGS_LIST_CTRL_ID, TXT_DL_SETTINGS_METHOD, method_text[method_type], IMAGE_COMMON_RIGHT_ARROW, (uint32)dlHandleMethodSet);

    /* Path */
    AppendTwoTextOneImageItem(MMIDL_DOWNLOAD_SETTINGS_LIST_CTRL_ID, TXT_DL_SETTINGS_PATH, path_text[path_type], IMAGE_COMMON_RIGHT_ARROW, (uint32)dlHandlePathSet);
#else
    //set method name
    GUILABEL_SetTextById(method_name_ctrl, TXT_DL_SETTINGS_METHOD, FALSE);

    //set path name
    GUILABEL_SetTextById(path_name_ctrl, TXT_DL_SETTINGS_PATH, FALSE);

    //get method item
    for (i = 0; i < MMIDL_DOWNLOAD_METHOD_TYPE_MAX; i++)
    {
        MMI_GetLabelTextByLang(method_text[i], &method_item[i]);
    }

    //set method drop down list
    GUIDROPDOWNLIST_AppendItemArray(method_list_ctrl, method_item, MMIDL_DOWNLOAD_METHOD_TYPE_MAX);
    
    //get current method index
    cur_index = (uint16)method_type;

    //set select method item
    GUIDROPDOWNLIST_SetCurItemIndex(method_list_ctrl, cur_index);

    //get path item
    for (i = 0; i < MMIDL_DOWNLOAD_PATH_TYPE_MAX; i++)
    {
        MMI_GetLabelTextByLang(path_text[i], &path_item[i]);
    }

    //set path drop down list
    GUIDROPDOWNLIST_AppendItemArray(path_list_ctrl, path_item, MMIDL_DOWNLOAD_PATH_TYPE_MAX);

    //get current path index
    cur_index = (uint16)path_type;

    //set select path item
    GUIDROPDOWNLIST_SetCurItemIndex(path_list_ctrl, cur_index);
#endif

    SetDownloadPath(win_id);
}

/*****************************************************************************/
// 	Description : save download settings param
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void SaveDownloadSettings(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T dsp_path_str = {0};
    MMI_STRING_T path_str = {0};
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_HANDLE_T method_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_METHOD_LIST_CTRL_ID);
    MMI_HANDLE_T path_list_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_PATH_LIST_CTRL_ID);
#endif
    MMI_HANDLE_T pic_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_PICTURE_VALUE_CTRL_ID);
    MMI_HANDLE_T music_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_MUSIC_VALUE_CTRL_ID);
    MMI_HANDLE_T video_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_VIDEO_VALUE_CTRL_ID);
    MMI_HANDLE_T others_value_ctrl = MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SAVE_OTHERS_VALUE_CTRL_ID);
#ifdef MMI_GUI_STYLE_TYPICAL
    MMIDL_DOWNLOAD_METHOD_TYPE old_download_method = MMIDL_DOWNLOAD_METHOD_APPOSITE;
    MMIDL_DOWNLOAD_METHOD_TYPE download_method = MMIDL_DOWNLOAD_METHOD_APPOSITE;
    MMIDL_DOWNLOAD_PATH_TYPE download_path = MMIDL_DOWNLOAD_PATH_DEFAULT;

    MMIDL_ReadDownloadMethodType(&old_download_method);

    download_method = (MMIDL_DOWNLOAD_METHOD_TYPE)GUIDROPDOWNLIST_GetCurItemIndex(method_list_ctrl);

    //save download method
    MMIDL_WriteDownloadMethodType(download_method);

    MMIDL_SetMultiTaskNum();

    if (MMIDL_DOWNLOAD_METHOD_QUEUE == old_download_method && MMIDL_DOWNLOAD_METHOD_APPOSITE == download_method)
    {
        MMIDL_CheckNextDownloadTask(MMIDL_GetDownloadManager());
    }

    //save download path
    download_path = (MMIDL_DOWNLOAD_PATH_TYPE) GUIDROPDOWNLIST_GetCurItemIndex(path_list_ctrl);
    MMIDL_WriteDownloadPathType(download_path);
#endif

    path_str.wstr_ptr = path;

    path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    GUILABEL_GetText(pic_value_ctrl, &dsp_path_str);
    MMIDL_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);
    MMIDL_WriteDownloadPicturePath(path_str.wstr_ptr);

    SCI_MEMSET(path, 0x0, sizeof(path));
    path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    GUILABEL_GetText(music_value_ctrl, &dsp_path_str);
    MMIDL_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);
    MMIDL_WriteDownloadMusicPath(path_str.wstr_ptr);

    SCI_MEMSET(path, 0x0, sizeof(path));
    path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    GUILABEL_GetText(video_value_ctrl, &dsp_path_str);
    MMIDL_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);
    MMIDL_WriteDownloadVideoPath(path_str.wstr_ptr);

    SCI_MEMSET(path, 0x0, sizeof(path));
    path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    GUILABEL_GetText(others_value_ctrl, &dsp_path_str);
    MMIDL_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);
    MMIDL_WriteDownloadOthersPath(path_str.wstr_ptr);
}

/*****************************************************************************/
//  Description : Set DL Task Focus Item 
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_DLTaskListSetFocusItem(uint16 focus_item)
{
    MMI_HANDLE_T mgr_win_handle = PNULL;
    MMI_HANDLE_T list_ctrl_handle = PNULL;

    mgr_win_handle = MMIDL_GetDownloadManagerWinHandle();
    if (PNULL != mgr_win_handle)
    {
        list_ctrl_handle = MMK_GetCtrlHandleByWin(mgr_win_handle, MMIDL_DOWNLOAD_MANAGER_LIST_CTRL_ID);
        if (PNULL != list_ctrl_handle)
        {
            if (focus_item >= GUILIST_GetTotalItemNum(list_ctrl_handle))
            {
                MMIDL_AppendDLTaskListItem(mgr_win_handle, list_ctrl_handle); /* Reload List */
            }
            GUILIST_SetCurItemIndex(list_ctrl_handle, focus_item);
        }
    }
}

/*****************************************************************************/
//  Description : Tidy all windows of DL
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDL_TidyExistedWin(void)
{
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_WIN_ID_T win_id = MMIDL_WIN_ID_START;
    MMI_HANDLE_T win_handle = PNULL;

    if (PNULL != download_applet)
    {
        applet_handle = MMIDL_HANDLE(download_applet);

        for (win_id = MMIDL_WIN_ID_START + 1; win_id <  MMIDL_MAX_WIN_ID; win_id++)
        {
            win_handle = MMK_GetWinHandle(applet_handle, win_id);
            if (MMK_IsOpenWin(win_handle))
            {
                MMK_CloseWin(win_handle);
            }
        }
    }
}


/*****************************************************************************/
//  Description : Set DD property info
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIDL_SetDDPropertyInfo(MMI_WIN_ID_T  win_id)
{
    MMIDL_OMA_DD_INFO_T *dd_info_ptr = PNULL;
    MMI_HANDLE_T ctrl_handle = PNULL;
    MMI_STRING_T string = {0};

    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIDL_DD_PROPERTY_RICHTEXT_CTRL_ID);
    dd_info_ptr = (MMIDL_OMA_DD_INFO_T *)MMK_GetWinAddDataPtr(win_id);

    //name
    MMI_GetLabelTextByLang(TXT_DL_DD_MO_NAME, &string);
    MMIDL_RichText_AddItem(ctrl_handle, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    string.wstr_ptr = dd_info_ptr->dd_content.name;
    string.wstr_len = MMIAPICOM_Wstrlen(dd_info_ptr->dd_content.name);
    MMIDL_RichText_AddItem(ctrl_handle, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    //vendor
    MMI_GetLabelTextByLang(TXT_DL_DD_MO_VENDOR, &string);
    MMIDL_RichText_AddItem(ctrl_handle, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    string.wstr_ptr = dd_info_ptr->dd_content.vendor;
    string.wstr_len = MMIAPICOM_Wstrlen(dd_info_ptr->dd_content.vendor);
    MMIDL_RichText_AddItem(ctrl_handle, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    //description
    MMI_GetLabelTextByLang(TXT_DL_DD_MO_DESCRIPTION, &string);
    MMIDL_RichText_AddItem(ctrl_handle, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    string.wstr_ptr = dd_info_ptr->dd_content.description;
    string.wstr_len = MMIAPICOM_Wstrlen(dd_info_ptr->dd_content.description);
    MMIDL_RichText_AddItem(ctrl_handle, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
}

/*****************************************************************************/
//  Description : handle message of DD property
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDDPropertyWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDL_SetDDPropertyInfo(win_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            MMIDL_OMA_DD_INFO_T *dd_info_ptr = PNULL;

            dd_info_ptr = (MMIDL_OMA_DD_INFO_T *)MMK_GetWinAddDataPtr(win_id);
            if (PNULL != dd_info_ptr)
            {
                MMIDL_OpenDownloadSaveWin(dd_info_ptr->task_id);
            }
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_RED:
        {
#ifdef OMADL_SUPPORT
            ODM_DOWNLOAD_CONFIRMATION_PARAM_T  cnf_param = {0};
            MMIDL_OMA_DD_INFO_T *dd_info_ptr = PNULL;

            dd_info_ptr = (MMIDL_OMA_DD_INFO_T *)MMK_GetWinAddDataPtr(win_id);
            cnf_param.is_user_allowed = FALSE;
            ODM_DownloadConfirmationResponse(&cnf_param);
            if (PNULL != dd_info_ptr)
            {
                MMIDL_DestoryDownloadTask(dd_info_ptr->task_id, &(((DOWNLOAD_APPLET_T *)MMIDL_APPLETINSTANCE())->download_instance));
            }

            recode = MMI_RESULT_FALSE;
#endif
        }
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T       ctrl_id = 0;

            if (PNULL != param)
            {
		        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (MSG_CTL_MIDSK == msg_id)
                {
                    ctrl_id = MMK_GetActiveCtrlId(win_id);
                }

                if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
        {
            MMIDL_OMA_DD_INFO_T *dd_info_ptr = PNULL;

            dd_info_ptr = (MMIDL_OMA_DD_INFO_T *)MMK_GetWinAddDataPtr(win_id);

            SCI_FREE(dd_info_ptr);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_OpenPDPChangeQueryWin(void)
{
    MMIDL_DOWNLOAD_INSTANCE_T *dl_instance = PNULL;

    dl_instance =  MMIDL_INSTANCE();

    if (PNULL != dl_instance && PNULL != dl_instance->downoad_manager && MMIDL_IsNeedDownLoadTask(dl_instance->downoad_manager))
    {
        MMIPUB_OpenQueryWinByTextId(TXT_WLAN_QUERY, IMAGE_PUBWIN_QUERY, PNULL, HandleQueryUseWlanWinMsg);
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_OpenPDPDeactiveQueryWin(void)
{
    MMIDL_DOWNLOAD_INSTANCE_T *dl_instance = PNULL;

    dl_instance =  MMIDL_INSTANCE();

    if (PNULL != dl_instance && MMIPDP_INTERFACE_WIFI == dl_instance->pdp_info.bearer_type )
    {
        if (MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
        {
            MMIDL_HandlePDP_Change();
        }
    }
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void AppendTwoTextOneImageItem(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_1, MMI_TEXT_ID_T text_2, MMI_IMAGE_ID_T image_id, uint32 user_data)
{
    InsertTwoTextOneImageItem(ctrl_id, GUILIST_GetTotalItemNum(ctrl_id), text_1, text_2, image_id, user_data);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void InsertTwoTextOneImageItem(MMI_CTRL_ID_T ctrl_id, uint16 index, MMI_TEXT_ID_T text_1, MMI_TEXT_ID_T text_2, MMI_IMAGE_ID_T image_id, uint32 user_data)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    if(TXT_NULL == text_2)
    {
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    }
    else
    {
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    }    

    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    if(TXT_NULL != text_2)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[2].item_data.text_id = text_2;
    }

    if (IMAGE_NULL != image_id)
    {
        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.text_id = image_id;
    }

    if (!GUILIST_InsertItem(ctrl_id, &item_t, index))
    {
        SCI_TRACE_LOW("InsertTwoTextOneImageItem failed");
    }
}


/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void dlHandleMethodSet(void)
{
    MMK_CreatePubListWin((uint32 *)MMIDL_METHOD_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL void dlHandlePathSet(void)
{
    MMK_CreatePubListWin((uint32 *)MMIDL_PATH_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DlHandleMethodSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        DlCreateMethodSetMenu(win_id, MMIDL_DOWNLOAD_SETTINGS_METHOD_MENU_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_METHOD_MENU_CTRL_ID));
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint16                      index = 0;
            MMIDL_DOWNLOAD_METHOD_TYPE  method_type = MMIDL_DOWNLOAD_METHOD_APPOSITE;

            MMIDL_ReadDownloadMethodType(&method_type);

            index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_METHOD_MENU_CTRL_ID));
            if (index != method_type)
            {
                MMIDL_WriteDownloadMethodType((MMIDL_DOWNLOAD_METHOD_TYPE)index);
            }

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DlHandlePathSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        DlCreatePathSetMenu(win_id, MMIDL_DOWNLOAD_SETTINGS_PATH_MENU_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_PATH_MENU_CTRL_ID));
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint16                       index = 0;
            MMIDL_DOWNLOAD_PATH_TYPE     path_type = MMIDL_DOWNLOAD_PATH_DEFAULT;

            MMIDL_ReadDownloadPathType(&path_type);

            index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIDL_DOWNLOAD_SETTINGS_PATH_MENU_CTRL_ID));
            if (index != path_type)
            {
                MMI_HANDLE_T win_handle = PNULL;
                DOWNLOAD_APPLET_T *applet = PNULL;

                MMIDL_WriteDownloadPathType((MMIDL_DOWNLOAD_PATH_TYPE)index);

                applet = MMIDL_APPLETINSTANCE();
                if (PNULL != applet)
                {
                    win_handle = MMK_GetWinHandle(MMIDL_HANDLE(applet), MMIDL_DOWNLOAD_SETTINGS_WIN_ID);
                    if (PNULL != win_handle)
                    {
                        SaveDownloadSettings(win_handle);
                    }
                }
            }

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void DlCreateMethodSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       method_text[] = {TXT_DL_SETTINGS_METHOD_APPOSITE, TXT_DL_SETTINGS_METHOD_QUEUE};
    uint32              node_index[] = {MMIDL_MOTHOD_TYPE_APPOSITE, MMIDL_MOTHOD_TYPE_QUEUE};
    uint16              select_index = 0;
    uint32              num = 0;
    MMIDL_DOWNLOAD_METHOD_TYPE method_type = MMIDL_DOWNLOAD_METHOD_APPOSITE;

    MMIDL_ReadDownloadMethodType(&method_type);

    num = sizeof(node_index) / sizeof(node_index[0]);

    select_index = method_type;

    DlCreateRadioListByTextId(win_id, ctrl_id, num, method_text, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void DlCreatePathSetMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_TEXT_ID_T       path_text[] = {TXT_DL_SETTINGS_PATH_DEFAULT, TXT_EXPORT_LOC_CUSTOM};
    uint32              node_index[] = {MMIDL_PATH_TYPE_DEFAULT, MMIDL_PATH_TYPE_CUSTOMIZE};
    uint16              select_index = 0;
    uint32              num = 0;
    MMIDL_DOWNLOAD_PATH_TYPE path_type = MMIDL_DOWNLOAD_PATH_DEFAULT;

    MMIDL_ReadDownloadPathType(&path_type);

    num = sizeof(node_index) / sizeof(node_index[0]);

    select_index = path_type;

    DlCreateRadioListByTextId(win_id, ctrl_id, num, path_text, node_index, select_index);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
void DlCreateRadioListByTextId(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint16 node_num, MMI_TEXT_ID_T *text_id_array, uint32 *node_array, uint32 select_index)
{
    GUILIST_ITEM_T          item_t = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};
    uint16                  index = {0};

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;

    GUILIST_SetMaxItem(ctrl_id, node_num, FALSE);

    for (index = 0; index < node_num; index++)
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_id_array[index];
        item_t.user_data                            = node_array[index];

        if (!GUILIST_AppendItem(ctrl_id, &item_t))
        {
            SCI_TRACE_LOW("DlCreateRadioListByTextId failed");
        }
    }

    GUILIST_SetSelectedItem(ctrl_id, select_index, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id, select_index);
}

/*****************************************************************************/
//  Description : Get List User data 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL uint32 DlGetListUserdata(MMI_CTRL_ID_T ctrl_id, uint16 index)
{
    uint32 user_data = 0;
    
    GUILIST_GetItemData(ctrl_id, index, (uint32*)(&user_data));
    
    return user_data;
}

#endif

#endif //#ifdef DL_SUPPORT


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:52 2012*/ //IGNORE9527


/*Edit by script, ignore 10 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527

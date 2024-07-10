/*****************************************************************************
** File Name:      mmifmm_mainwin.c                                          *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/

#define _MMIFMM_MAINWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_mainwin.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmifmm_menutable.h"
#include "guitab.h"
#include "mmipub.h"
#include "mmifmm_image.h"
#include "mmisd_export.h"
#include "mmifmm_app.h"
#include "mmifmm_mem.h"
#include "mmifmm_comfunc.h"
#include "mmiudisk_export.h"
#include "mmieng_export.h"
#include "mmiset_export.h"
#include "mmifmm_renamewin.h"
#include "mmi_autotest.h"
//#include "mmi_drmfilemgr.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmifmm_interface.h"
#include "mmifmm_pic.h"
#include "mmifmm_deletewin.h"
#include "mmi_pubmenuwin.h"
#include "mmivc_common.h"
#include "mmivcard_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmisms_id.h"
#if defined MMI_VCARD_SUPPORT
#include "mmisms_export.h"
#endif
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_export.h"
#endif
#include "mmifmm_internal.h"
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmivp_text.h"
#endif
#include "mmifmm_sd.h"
#include "mmifmm_autotest.h"
#include "mmifmmsrv_api.h"
#include "mmialarm_text.h"
#include "mmi_appmsg.h"
#include "window_parse.h"
#include "gui_ucs2b_converter.h"
#ifdef  VIDEOTHUMBNAIL_SUPPORT
#include "mmifmm_vthumbnail.h"
#include "mmivp_export.h"
#endif
#include "mmifmm_comwin.h"
#ifdef CALENDAR_SUPPORT
#include "mmischedule_export.h"
#endif
#include "mmidisplay_data.h"
#ifndef MMI_FMM_TITLE_BUTTON_SUPPORT
#include "guictrl_api.h"
#endif
#include "guires.h"
#ifdef MCARE_V31_SUPPORT
#include "Mcare_Interface.h"
#endif
#ifndef MMI_PDA_SUPPORT
#include "guisoftkey.h"
#ifdef MMI_AUDIO_PLAYER_SUPPORT
#include "mmialarm_export.h"
#endif
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

typedef enum
{
    MMIFMM_APPEND_LIST_ITEM_TYPE,          //append
    MMIFMM_REPLACE_LIST_ITEM_TYPE,         //replace
    MMIFMM_NEED_LIST_ITEM_DATA_TYPE,       //need data
    MMIFMM_NEED_LIST_ITEM_CONTENT_TYPE    //need content
} MMIFMM_LIST_ITEM_TYPE_E;

typedef enum
{
    MMIFMM_NORMAL_TAB_TYPE,
    MMIFMM_OTHER_TAB_TYPE
} MMIFMM_TAB_TYPE_E;
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    wchar               file_name[MMIFMM_PATHNAME_LEN +1];
    uint16              file_len;
    uint32              win_id;
    uint32              s_suffix_type;
    BOOLEAN             is_need_display;
    BOOLEAN             user_filename_valid;
    BOOLEAN             is_protect_checked;
    MMIFILE_DEVICE_E    s_cur_dev;
    MMIFMM_PATH_INFO_T  patch_info;
    MMIFILE_DEVICE_E    s_device_table[MMI_DEVICE_NUM];
    MMIFMM_SOFTKEY_TEXT_T sk_text_id;
    MMIFMM_OPENWIN_CALLBACK_T callback;
} MMIFMM_OPEN_FILE_PATH_INFO_T;


typedef struct
{
    MMIFMM_DATA_INFO_T * s_fmm_list_data_ptr;
    MMIFMM_PATH_INFO_T * s_fmm_current_path_ptr;
    MMIFMM_OPEN_FILE_PATH_INFO_T * s_file_path_info_ptr;
} MMIFMM_MAIN_WIN_DATA_T;

typedef struct
{
    MMIFMM_MAIN_WIN_DATA_T  *main_d_ptr;
    MMIFMM_PATH_INFO_T      *s_fmm_path_ptr;
    MMIFMM_PICTURE_INDEX_DATA_T    *s_pic_index_data_ptr;
    wchar                   *s_list_file_name_ptr;
    wchar                   s_list_file_name_len;
    MMI_CTRL_ID_T           list_ctrl_id; //list ctrl id
    MMI_CTRL_ID_T           tab_ctrl_id; //tab ctrl id
    MMI_CTRL_ID_T           com_ctrl_id;  //softkey ctrl_id
    MMIFMM_FILE_TYPE_E      cur_file_type; //current file type
} MMIFMM_VIEW_WIN_DATA_T;
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

#ifdef MMI_PDA_SUPPORT
LOCAL MMI_IMAGE_ID_T g_s_focustab_icon[] =
{
    IMAGE_FMM_UDISK_FOCUSED,
    IMAGE_FMM_SDCARD_FOCUSED,
    IMAGE_FMM_SDCARD1_FOCUSED,
};

LOCAL MMI_IMAGE_ID_T g_s_releasetab_icon[] =
{
    IMAGE_FMM_UDISK_RELEASED,
    IMAGE_FMM_SDCARD_RELEASED,
    IMAGE_FMM_SDCARD1_RELEASED,
};
#if defined MMI_ISTYLE_SUPPORT
LOCAL MMI_IMAGE_ID_T g_s_focustab_istyle_icon[] =
{
    ISTYLE_IMAGE_FMM_UDISK_FOCUSED,
    ISTYLE_IMAGE_FMM_SDCARD_FOCUSED,
    ISTYLE_IMAGE_FMM_SDCARD1_FOCUSED,
};

LOCAL MMI_IMAGE_ID_T g_s_releasetab_istyle_icon[] =
{
    ISTYLE_IMAGE_FMM_UDISK_RELEASED,
    ISTYLE_IMAGE_FMM_SDCARD_RELEASED,
    ISTYLE_IMAGE_FMM_SDCARD1_RELEASED,
};
#endif
LOCAL const uint32 s_fmm_folder_menu_root_item[][2] =
{
    {ID_FMM_DELETE, TXT_DELETE},//folder delete
    {ID_FMM_DETAIL, STXT_DETAIL},//folder detail
    {ID_FMM_RENAME, TXT_COMMON_RENAME},//folder rename
    {ID_FMM_COPY, TXT_COMMON_COPY},////folder copy
#ifdef MMI_FMM_MOVE_SUPPORT
    {ID_FMM_MOVE, TXT_COMMON_CUT},
#endif
};
LOCAL const uint32 s_fmm_music_menu_root_item[][2] =
{
    {ID_FMM_DELETE, TXT_DELETE},
#ifdef MMI_AUDIO_PLAYER_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    {ID_FMM_SIM1_RING, TXT_FMM_SET_RING_SIM1_SELECT_CALL},
    {ID_FMM_SIM2_RING, TXT_FMM_SET_RING_SIM2_SELECT_CALL},
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    {
        ID_FMM_SIM3_RING, TXT_FMM_SET_RING_SIM3_SELECT_CALL
    },
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    {
        ID_FMM_SIM4_RING, TXT_FMM_SET_RING_SIM4_SELECT_CALL
    },
#endif
#else
    {ID_FMM_RING, TXT_COMMON_SET_CALL_RING},
#endif
#endif
#ifdef BLUETOOTH_SUPPORT
    {ID_FMM_BLUETOOTH, TXT_COMMON_SEND_BY_BT},
#endif
#ifdef MMS_SUPPORT
    {ID_FMM_MMS, TXT_COMM_MMS_SEND},
#endif
    {ID_FMM_DETAIL, STXT_DETAIL},
    {ID_FMM_RENAME, TXT_COMMON_RENAME},
    {ID_FMM_COPY, TXT_COMMON_COPY},
#ifdef MMI_FMM_MOVE_SUPPORT
    {ID_FMM_MOVE, TXT_COMMON_CUT},
#endif
};
LOCAL const uint32 s_fmm_picture_menu_root_item[][2] =
{
    {ID_FMM_WALLPAPER, TXT_COMMON_SETAS_WALLPAPER},
    {ID_FMM_PB, TXT_FMM_PHOTO_SET_PB},
    {ID_FMM_POWER_ON_SET, TXT_SET_AS_POWER_ON_ANIMATION},//TXT_FMM_POWER_ON_ANIMATION
    {ID_FMM_POWER_OFF_SET, TXT_SET_AS_POWER_OFF_ANIMATION},//TXT_FMM_POWER_OFF_ANIMATION
#ifdef MMS_SUPPORT
    {ID_FMM_MMS, TXT_COMM_MMS_SEND},
#endif
#ifdef BLUETOOTH_SUPPORT
    {ID_FMM_BLUETOOTH, TXT_COMMON_SEND_BY_BT},
#endif
    {ID_FMM_DELETE, TXT_DELETE},
    {ID_FMM_RENAME, TXT_COMMON_RENAME},
    {ID_FMM_PREVIEW, STXT_PREVIEW},
    {ID_FMM_DETAIL, STXT_DETAIL},
    {ID_FMM_COPY, TXT_COMMON_COPY},
#ifdef MMI_FMM_MOVE_SUPPORT
    {ID_FMM_MOVE, TXT_COMMON_CUT},
#endif
//     {ID_FMM_PHOTO_SET_PB,TXT_FMM_PHOTO_SET_PB},
};

LOCAL const uint32 s_fmm_movie_menu_root_item[][2] =
{
    {ID_FMM_DELETE, TXT_DELETE},
#ifdef SCREENSAVER_SUPPORT
    {ID_FMM_SCEENSAVE, TXT_SET_AS_SCREENSAVE},//TXT_FMM_SCREENSAVE
#endif
    {ID_FMM_POWER_ON_SET, TXT_SET_AS_POWER_ON_ANIMATION},//TXT_FMM_POWER_ON_ANIMATION
    {ID_FMM_POWER_OFF_SET, TXT_SET_AS_POWER_OFF_ANIMATION},//TXT_FMM_POWER_OFF_ANIMATION
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    {ID_FMM_VIDEO_WALLPAPER, TXT_VP_PDA_VIDEO_WALLPAPER},
#endif
#ifdef BLUETOOTH_SUPPORT
    {ID_FMM_BLUETOOTH, TXT_COMMON_SEND_BY_BT},
#endif
#ifdef MMS_SUPPORT
    {ID_FMM_MMS, TXT_COMM_MMS_SEND},
#endif
    {ID_FMM_DETAIL, STXT_DETAIL},
    {ID_FMM_RENAME, TXT_COMMON_RENAME},
    {ID_FMM_COPY, TXT_COMMON_COPY},
#ifdef MMI_FMM_MOVE_SUPPORT
    {ID_FMM_MOVE, TXT_COMMON_CUT},
#endif
};

LOCAL const uint32 s_fmm_vcard_menu_root_item[][2] =
{
    {ID_FMM_DELETE, TXT_DELETE},
    {ID_FMM_LOAD_VCARD, TXT_COMMON_IMPORT},
#ifdef BLUETOOTH_SUPPORT
    {ID_FMM_BLUETOOTH, TXT_COMMON_SEND_BY_BT},
#endif
#ifdef MMS_SUPPORT
    {ID_FMM_MMS, TXT_COMM_MMS_SEND},
#endif
    {ID_FMM_SMS, TXT_NOTEPAD_SEND_BY_SMS},
    {ID_FMM_DETAIL, STXT_DETAIL},
    {ID_FMM_RENAME, TXT_COMMON_RENAME},
    {ID_FMM_COPY, TXT_COMMON_COPY},
#ifdef MMI_FMM_MOVE_SUPPORT
    {ID_FMM_MOVE, TXT_COMMON_CUT},
#endif
};

LOCAL const uint32 s_fmm_vcalendar_menu_root_item[][2] =
{
    {ID_FMM_DELETE, TXT_DELETE},
#ifdef MMI_VCALENDAR_SUPPORT
    {ID_FMM_SAVE_VCALENDAR, TXT_SAVE_CALENDAR},
#endif
#ifdef BLUETOOTH_SUPPORT
    {ID_FMM_BLUETOOTH, TXT_COMMON_SEND_BY_BT},
#endif
#ifdef MMS_SUPPORT
    {ID_FMM_MMS, TXT_COMM_MMS_SEND},
#endif
    {ID_FMM_VCALENDAR_SMS, TXT_NOTEPAD_SEND_BY_SMS},

    {ID_FMM_DETAIL, STXT_DETAIL},
    {ID_FMM_RENAME, TXT_COMMON_RENAME},
    {ID_FMM_COPY, TXT_COMMON_COPY},
#ifdef MMI_FMM_MOVE_SUPPORT
    {ID_FMM_MOVE, TXT_COMMON_CUT},
#endif
};

LOCAL const uint32 s_fmm_other_menu_root_item[][2] =
{
    {ID_FMM_DELETE, TXT_DELETE},
#ifdef BLUETOOTH_SUPPORT
    {ID_FMM_BLUETOOTH, TXT_COMMON_SEND_BY_BT},
#endif
#ifdef MMS_SUPPORT
    {ID_FMM_MMS, TXT_COMM_MMS_SEND},
#endif
    {ID_FMM_DETAIL, STXT_DETAIL},
    {ID_FMM_RENAME, TXT_COMMON_RENAME},
    {ID_FMM_COPY, TXT_COMMON_COPY},
#ifdef MMI_FMM_MOVE_SUPPORT
    {ID_FMM_MOVE, TXT_COMMON_CUT},
#endif
};

LOCAL const uint32 s_fmm_file_menu_root_item[][2] =
{
    {ID_FMM_DELETE, TXT_DELETE},
#ifdef BLUETOOTH_SUPPORT
    {ID_FMM_BLUETOOTH, TXT_COMMON_SEND_BY_BT},
#endif
#ifdef MMS_SUPPORT
    {ID_FMM_MMS, TXT_COMM_MMS_SEND},
#endif
    {ID_FMM_DETAIL, STXT_DETAIL},
    {ID_FMM_RENAME, TXT_COMMON_RENAME},
    {ID_FMM_COPY, TXT_COMMON_COPY},
#ifdef MMI_FMM_MOVE_SUPPORT
    {ID_FMM_MOVE, TXT_COMMON_CUT},
#endif
};

LOCAL const uint32 s_fmm_manage_menu_root_item[][2] =
{
#ifdef SCREENSAVER_SUPPORT
    {ID_FMM_SCEENSAVE, TXT_COMMON_SCREENSAVE},
#endif
    {ID_FMM_POWER_ON_SET, TXT_COMMON_POWER_ON_ANIMATION},
    {ID_FMM_POWER_OFF_SET, TXT_COMMON_POWER_OFF_ANIMATION},
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    {ID_FMM_VIDEO_WALLPAPER, TXT_VIDEO_WALLPAPER},
#endif
};

LOCAL const uint32 s_fmm_marked_menu_root_item[][2] =
{
#ifdef BLUETOOTH_SUPPORT
    {ID_FMM_BLUETOOTH, TXT_COMMON_SEND_BY_BT},
#endif
    {ID_FMM_DELETE, TXT_DELETE},
    {ID_FMM_COPY, TXT_COMMON_COPY},
#ifdef MMI_FMM_MOVE_SUPPORT
    {ID_FMM_MOVE, TXT_COMMON_CUT},
#endif
};

#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create exploer windows
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateExplorerWin (MMIFMM_MAIN_WIN_DATA_T *main_win_p);

/*****************************************************************************/
//  Description : handle fmm main window message
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmMainWinMsg (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

/*****************************************************************************/
//  Description : HandleUdiskChildWindow
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUdiskChildWindow (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

/*****************************************************************************/
// 	Description : validate privacy fmm doc
//	Global resource dependence :
//  Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL void FMM_ValidatePrivacyFmmDoc (BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : update file list
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void UpdateFileList (MMI_WIN_ID_T win_id, BOOLEAN need_fullpiant);


/*****************************************************************************/
//  Description : Initialize device path
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint16 FmmInitDevicePath (
    MMIFILE_DEVICE_E      device,   // [IN]
    wchar               *path_ptr   //[IN]/[OUT]
);

/*****************************************************************************/
//  Description : fmm check memory card state
//  Global resource dependence : none
//  Author: jian.ma
//  Note:TRUE
/*****************************************************************************/
LOCAL BOOLEAN FmmCheckMemoryCardState (void);

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MainWinOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MainWinCloseCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListViewWinOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListViewWinCloseCtrlFunc (MMI_WIN_ID_T win_id);

#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
/*****************************************************************************/
//  Description :SetTitleLabelParam
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void SetTitleLabelParam (MMIFMM_PATH_INFO_T  *current_path_ptr);

/*****************************************************************************/
//  Description :title button call back
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E   TitleButtonCallBack (MMI_HANDLE_T ctrl_handle);
#endif

/*****************************************************************************/
//  Description : handle list append data
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FmmHandleListAppendData (
    MMI_WIN_ID_T         win_id,
    DPARAM param,
    MMI_HANDLE_T ctrl_handle,
    MMIFMM_LIST_ITEM_TYPE_E type
);

/*****************************************************************************/
//  Description : 保存当前设备的路径
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void SaveCurrentPath (
    MMIFMM_PATH_INFO_T     *path_ptr ,
    MMIFMM_PATH_INFO_T     *current_path_ptr,
    uint16                 index
);
/*****************************************************************************/
//  Description : 更新列表数据
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateListData (
    MMI_WIN_ID_T           win_id,
    MMI_CTRL_ID_T          ctrl_id,
    MMIFMM_READ_TYPE_E     type,
    MMI_TEXT_ID_T          sfk_left_id,
    BOOLEAN                is_empty
);

/*****************************************************************************/
//  Description : get file index when open win
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint16 GetFileIndex (
    MMIFMM_DATA_INFO_T     *list_data_ptr,
    const wchar            *file_name_ptr,
    uint16                 file_name_len
);

/*****************************************************************************/
//  Description : handle mark item function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleMarkItem (
    MMI_WIN_ID_T           win_id,
    DPARAM  param
);

/*****************************************************************************/
//  Description : handle cancel option function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleCancelOpt (MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : get next index after creating a folder;
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetNextIndex (
    MMIFMM_DATA_INFO_T     *list_data_ptr,
    const wchar                 *folder_name_ptr,
    uint16                 folder_name_len
);



/*****************************************************************************/
//  Description : 更新重命名数据
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateRenameData (
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T        ctrl_id,
    //MMIFMM_DATA_INFO_T   *list_data_ptr ,
    const wchar               *new_filename_ptr,
    MMI_TEXT_ID_T        sfk_left_id
);
/*****************************************************************************/
//  Description : 添加列表list的item
//  Global resource dependence :
//  Author: robert.lu
//  Note:  保存文件的类型
/*****************************************************************************/
LOCAL void AppendTwoLineAnimTextAndTextListItem (
    MMI_WIN_ID_T            win_id,
    MMI_CTRL_ID_T           ctrl_id,
    MMI_TEXT_ID_T           left_softkey_id,
    MMIFMM_FILE_TYPE_E      file_type,
    const wchar*            filename,
    uint16                  filename_len,
    uint32                  filesize,
    BOOLEAN                 path_is_valid,
    MMIFMM_PATH_INFO_T      *current_path_ptr,
    uint16                  item_index,
    uint16                  item_content_index,
    MMIFMM_LIST_ITEM_TYPE_E type,
    BOOLEAN                 is_two_line
);

/*****************************************************************************/
//  Description : get up path info
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN EnterUpPath (wchar * path_ptr, uint16 *path_len);

/*****************************************************************************/
//  Description : get first latter by name
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint16 GetFirstLatterByName (const wchar *filename);


/*****************************************************************************/
// Description : update valid path
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
LOCAL BOOLEAN FmmUpdateValidPath (MMI_WIN_ID_T win_id , BOOLEAN is_enter_root);


/*****************************************************************************/
//  Description : handle mark Operate
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandlePDAMarkOperate (
    MMI_WIN_ID_T        win_id
);

/*****************************************************************************/
//  Description : handle menu cancel all mark
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuCancelAllMark (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMI_CTRL_ID_T         ctrl_id
);

/*****************************************************************************/
//  Description : handle list item
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleListItem (
    MMI_WIN_ID_T win_id
    //MMIFMM_DATA_INFO_T         *list_data_ptr,
    //MMIFMM_PATH_INFO_T         *current_path_ptr,
    //uint32                     ctrl_id
);

#ifdef MMI_AUDIO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void ValidatePrivacyPlayMusicFile (void);
#endif

#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void ValidatePrivacyPlayMovieFile (void);
#endif

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void ValidatePrivacyShowTxtContent (BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : 添加列表list的item
//  Global resource dependence :
//  Author: robert.lu
//  Note:  保存文件的类型
/*****************************************************************************/
LOCAL void FmmAppendListItem (//当前索引
    MMI_WIN_ID_T         win_id,
    MMI_CTRL_ID_T        ctrl_id,
    MMI_TEXT_ID_T        left_softkey_id,
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMIFMM_PATH_INFO_T   * current_path_ptr
);

/*****************************************************************************/
//  Description : handle menu mark all
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuMarkAll (
    MMIFMM_DATA_INFO_T  *list_data_ptr,
    MMI_CTRL_ID_T       ctrl_id
);

/*****************************************************************************/
//  Description : setup the data of the picture index
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SetupPicIndexData (
    MMI_WIN_ID_T         win_id,
    MMI_CTRL_ID_T                ctrl_id,
    MMIFMM_DATA_INFO_T           *list_data_ptr,     // [IN]
    MMIFMM_PICTURE_INDEX_DATA_T  *pic_index_data_ptr // [OUT]
);

/*****************************************************************************/
//  Description : handle mark item function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleCreatePopMenuOpt (
    MMI_WIN_ID_T         win_id,
    uint32 index
);




/*****************************************************************************/
//  Description : get sub folder max file path len
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMenuOption (MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : handle pub menu win ok handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuOKHandler (MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : handle pub menu win cancel handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuCancelHandler (MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuInitHandler (MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : handle pub menu win close handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuCloseHandler (MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : handle menu mark
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuMark (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMI_CTRL_ID_T         ctrl_id
);

/*****************************************************************************/
//  Description : handle menu cancel mark
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuCancelMark (
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMI_CTRL_ID_T        ctrl_id
);

/*****************************************************************************/
//  Description : enablel menu item
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void DisableInvalidMenuItem (
    MMI_WIN_ID_T          win_id,
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMIFMM_PATH_INFO_T    *current_path_ptr,
    MMI_MENU_GROUP_ID_T   group_id,
    MMI_CTRL_ID_T         list_ctrl_id,
    MMI_CTRL_ID_T         menu_ctrl_id
);
/*****************************************************************************/
//  Description : set win title
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetWinTitle (
    MMIFMM_TAB_TYPE_E    type,
    MMI_WIN_ID_T         win_id,
    MMI_CTRL_ID_T        ctrl_id,
    MMI_CTRL_ID_T        tab_ctrl_id,
    MMIFMM_PATH_INFO_T   *current_path_ptr
);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle pda menu mark
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandlePDAMenuMark (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMI_CTRL_ID_T         ctrl_id,
    MMI_WIN_ID_T          win_id
);


/*****************************************************************************/
//  Description : dea long ok menu
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void DealWithUdiskLongOkMenu (
    MMI_WIN_ID_T win_id,
    //MMIFMM_DATA_INFO_T *list_data_ptr,
    uint16 index
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: maryxiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIFMM_isMenuAvailable (uint32 menu_id);

/**************************************************************************************************/
// Description:
// Date:
// Author:maryxiao
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleFmmLongPopUpMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);
/*****************************************************************************/
//  Description : IsMarkItemContainFolders
//  Global resource dependence :
//  Author: juan.wu
//  Note:判断mark的item中是否含有文件夹,有，返回TRUE
/*****************************************************************************/
LOCAL BOOLEAN IsMarkItemContainFolders (void);


#endif
/**************************************************************************************************/
// Description:import vcard call back function
// Date:
// Author:
// Note:
//*************************************************************************************************/
LOCAL void  Importvcard_Callback (MMIPB_HANDLE_T handle, MMIPB_ERROR_E error);

/*****************************************************************************/
//  Description : wait move window message
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImportVcardWaitWinMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
// Description : 是否选中的是同一种文件类型文件
// Global resource dependence :
// Author: renyi.hu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSelectedSameFileType (MMIFMM_DATA_INFO_T    *list_data_ptr,
                                      MMIFMM_FILE_TYPE_E     type
                                     );

WINDOW_TABLE (MMIFMM_MAIN_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC ( (uint32) HandleFmmMainWinMsg),
    WIN_ID (MMIFMM_MAIN_WIN_ID),
//fixed by liyan.zhu for bugzilla 2781
#if (defined (MMI_GUI_STYLE_TYPICAL)||defined (MMI_GUI_STYLE_MINISCREEN))
    CREATE_TAB_CTRL (GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL, MMIFMM_TAB_CTRL_ID),
#else
    CREATE_TAB_CTRL (GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL | GUITAB_STATE_SINGLE_LINE, MMIFMM_TAB_CTRL_ID),
#endif
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the child window for udisk
WINDOW_TABLE (MMIFMM_UDISK_CHILD_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC ( (uint32) HandleUdiskChildWindow),
    WIN_ID (MMIFMM_UDISK_CHILD_WIN_ID),
//  WIN_TITLE(TXT_NULL),
#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
    CREATE_FORM_CTRL (GUIFORM_LAYOUT_ORDER, MMIFMM_UDISK_FORM_CTRL_ID),
    CHILD_FORM_CTRL (FALSE, GUIFORM_LAYOUT_SBS, MMIFMM_UDISK_TITLE_BUTTON_FORM_CTRL_ID, MMIFMM_UDISK_FORM_CTRL_ID),
    CHILD_LABEL_CTRL (GUILABEL_ALIGN_MIDDLE, FALSE, MMIFMM_UDISK_TITLE_LABEL_CTRL_ID, MMIFMM_UDISK_TITLE_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL (TRUE, IMAGE_COMMON_BACK, MMIFMM_UDISK_TITLE_BUTTON_CTRL_ID, MMIFMM_UDISK_TITLE_BUTTON_FORM_CTRL_ID),
    CHILD_LIST_CTRL (TRUE, GUILIST_TEXTLIST_E, MMIFMM_UDISK_LISTBOX_CTRL_ID, MMIFMM_UDISK_FORM_CTRL_ID),
#ifdef GUIF_SOFTKEY_PDASTYLE
    CHILD_SOFTKEY_CTRL (TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIFMM_UDISK_FORM_CTRL_ID),
#else
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
#else
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL (GUILIST_TEXTLIST_E, MMIFMM_UDISK_LISTBOX_CTRL_ID),
#endif
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE (MMIFMM_LONG_POPUP_OPT_MENU_WIN_TAB) =
{
    WIN_FUNC ( (uint32) HandleFmmLongPopUpMsg),
    WIN_ID (MMIFMM_LONG_POPUP_OPT_MENU_WIN_ID),
    WIN_STYLE (WS_HAS_TRANSPARENT),
    END_WIN
};
#endif
/*****************************************************************************/
//  Description : create exploer windows
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CreateExplorerWin (MMIFMM_MAIN_WIN_DATA_T *main_win_p)
{
	//DPARAM param = SCI_ALLOC_APPZ(sizeof(uint8)*(MMIFILE_FILE_NAME_MAX_LEN + 1));	
    //ATC_ExtCmd(param);
    MMK_CreateParentWin ( (uint32 *) MMIFMM_MAIN_WIN_TAB, (ADD_DATA) main_win_p);
    MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);

    return TRUE;
}
/*****************************************************************************/
//  Description : set preview picture anim control param
//  Global resource dependence :
//  Author: liyan.zhu
//  Note: CR191213
/*****************************************************************************/
BOOLEAN MMIAPIFMM_IsPreviewPicSucc (
									wchar          *full_path_wstr_ptr,//in
									uint16         full_path_wstr_len//in:字节数
									)

{
	GUI_RECT_T              icon_rect       = {0,0,100,100};
	IGUICTRL_T              *ctrl_ptr       = PNULL;
	GUIANIM_RESULT_E        anim_result     = GUIANIM_RESULT_FAIL;
	GUIANIM_CTRL_INFO_T     ctrl_info       = {0};
	GUIANIM_DISPLAY_INFO_T  display_info    = {0};
	GUIANIM_FILE_INFO_T     file_info	    = {0};
	MMI_CONTROL_CREATE_T    create          = {0};
	GUIANIM_INIT_DATA_T     init_data       = {0};
	BOOLEAN                 result          = FALSE;
	
	init_data.both_rect.h_rect = init_data.both_rect.v_rect = icon_rect;
	create.ctrl_id = 10; //useless ctrl id just for malloc a new ctrl_ptr
	create.guid    = SPRD_GUI_ANIM_ID;
	create.parent_win_handle = MMK_ConvertIdToHandle(MMIFMM_UDISK_CHILD_WIN_ID);
	create.parent_ctrl_handle = 0;
	create.init_data_ptr = &init_data;
	
	ctrl_ptr = MMK_CreateControl(&create);
	
	//set anim param
	ctrl_info.is_ctrl_id = FALSE;
	ctrl_info.ctrl_id    = 0;
	ctrl_info.ctrl_ptr   = ctrl_ptr;
	
	display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
	display_info.is_play_once      = FALSE;
	display_info.is_syn_decode     = TRUE;
	display_info.is_update         = FALSE;
	display_info.is_disp_one_frame = TRUE;	
	
	file_info.full_path_wstr_ptr = full_path_wstr_ptr;
    file_info.full_path_wstr_len = full_path_wstr_len;

	anim_result = GUIANIM_SetParam(&ctrl_info,
		PNULL,
		&file_info,
		&display_info
		);
	
	MMK_DestroyControl(create.ctrl_id);
	
    if ( (GUIANIM_RESULT_SUCC == anim_result) ||
		(GUIANIM_RESULT_WAIT == anim_result))
    {
        result = TRUE;
    }
	
    return (result);
}
/*****************************************************************************/
//  Description : handle fmm main window message
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmMainWinMsg (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T    str_text = {0};
    uint16          cur_tab_index = 0;
    uint16          sel_tab_index = 0;
    uint32          base_index = 0;
    MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_UDISK;
    char            get_label_name[MMIFILE_NEW_DEVICE_NAME_MAX_LEN*3+1] = {0};
    wchar           buffer_name[MMIFMM_FULL_FILENAME_LEN+1] = {0};
    BOOLEAN         get_label_res = FALSE;
    MMI_TEXT_ID_T g_s_tabstr[] =
    {
        TXT_COMMON_UDISK,
#ifndef DUAL_TCARD_SUPPORT
		TXT_COMMON_SD_CARD,
#else
		TXT_COMMON_SD_CARD0,
		TXT_COMMON_SD_CARD1,
#endif
    };

    MMIFMM_MAIN_WIN_DATA_T* main_d_ptr = (MMIFMM_MAIN_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    //SCI_TRACE_LOW:"MMIFMM: HandleFmmMainWinMsg, win_id =%d, msg_id = %d."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3048_112_2_18_2_20_41_369, (uint8*) "dd", win_id, msg_id);

    //UNUSED_PARAM(param);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        if (MMI_RESULT_FALSE == MainWinOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        main_d_ptr = (MMIFMM_MAIN_WIN_DATA_T*) MMK_GetWinUserData (win_id);

        if (PNULL == main_d_ptr)
        {
            MMK_CloseWin (win_id);
            break;
        }

        MMK_SetAtvCtrl (win_id, MMIFMM_TAB_CTRL_ID);
        GUITAB_SetMaxItemNum (MMIFMM_TAB_CTRL_ID, MMI_DEVICE_NUM);

        //add window
        base_index = fmm_dev;

        for (; fmm_dev < MMI_DEVICE_NUM; fmm_dev++)
        {
            if (MMIAPIFMM_GetDeviceTypeStatus (fmm_dev)
#ifdef MSDC_CARD_SUPPORT
                    || MMIAPISD_IsSDNeedFormat (fmm_dev)
#endif
               )
            {
                if (MMI_DEVICE_UDISK < fmm_dev && fmm_dev < MMI_DEVICE_NUM)
                {
                    get_label_res = MMIAPIFMM_GetDeviceLabel (MMIAPIFMM_GetDevicePath (fmm_dev), get_label_name);
                }

                if (get_label_res)
                {
                    uint16 temp_wstr_len = 0;//pclint bug 2099193
                    SCI_MEMSET (buffer_name, 0x00, (sizeof (wchar) * (MMIFMM_FULL_FILENAME_LEN + 1)));
                    //CR161453
                    GUI_OtherCodeToWstr(buffer_name,MMIFMM_FULL_FILENAME_LEN,GUI_GetCodeType(get_label_name,strlen (get_label_name)),(uint8 *)get_label_name,strlen(get_label_name));
                    //GUI_UTF8ToWstr (buffer_name, MMIFILE_NEW_DEVICE_NAME_MAX_LEN, (uint8 *) get_label_name, strlen (get_label_name));
                    //GUI_OtherCodeToWstr(buffer_name,MMIFILE_NEW_DEVICE_NAME_MAX_LEN, GUI_CHAR_CODE_ANSI,  (uint8 *)get_label_name,strlen(get_label_name));
                    str_text.wstr_ptr = buffer_name;
                    temp_wstr_len = MMIAPICOM_Wstrlen (buffer_name);
                    str_text.wstr_len = MIN(temp_wstr_len,MMIFILE_NEW_DEVICE_NAME_MAX_LEN);
                }
                else
                {
                    MMI_GetLabelTextByLang (g_s_tabstr[fmm_dev - base_index], &str_text);/*lint !e661*/
                }

                //MMI_GetLabelTextByLang(g_s_tabstr[fmm_dev - base_index], &str_text);

#ifdef MMI_PDA_SUPPORT
                {
                    MMI_IMAGE_ID_T *focustab_icon_ptr = PNULL;
                    MMI_IMAGE_ID_T *releasetab_icon_ptr = PNULL;
#if defined MMI_ISTYLE_SUPPORT
                    if (MMITHEME_IsIstyle())
                    {
                        focustab_icon_ptr = g_s_focustab_istyle_icon;
                        releasetab_icon_ptr = g_s_releasetab_istyle_icon;
                    }
                    else
#endif						
                    {
                        focustab_icon_ptr = g_s_focustab_icon;
                        releasetab_icon_ptr = g_s_releasetab_icon;
                    }

                    GUITAB_AppendSimpleItem (
                        MMIFMM_TAB_CTRL_ID,
                        &str_text,
                        focustab_icon_ptr[fmm_dev - base_index],
                        releasetab_icon_ptr[fmm_dev - base_index]
                    );

                }
#else
                GUITAB_AppendSimpleItem (
                    MMIFMM_TAB_CTRL_ID,
                    &str_text,
                    IMAGE_FMM_UDISK_FOCUSED + fmm_dev - base_index,
                    IMAGE_FMM_UDISK_RELEASED + fmm_dev - base_index
                );
#endif
                GUITAB_AddChildWindow (
                    MMIFMM_TAB_CTRL_ID,
                    MMIFMM_UDISK_CHILD_WIN_ID,
                    cur_tab_index);

                if (fmm_dev == main_d_ptr->s_file_path_info_ptr->s_cur_dev)
                {
                    sel_tab_index = cur_tab_index;

                }

                main_d_ptr->s_file_path_info_ptr->s_device_table[cur_tab_index] = fmm_dev;
                cur_tab_index++	;
                //continue;
            }

            //SCI_TRACE_LOW("HandleFmmMainWinMsg: the status is:%d, need format:%d", MMIAPIFMM_GetDeviceTypeStatus(fmm_dev), MMIAPISD_IsSDNeedFormat(fmm_dev));
        }
        //双卡时index是否对应的判断
        if(FALSE == GUITAB_SetCurSel(MMIFMM_TAB_CTRL_ID, sel_tab_index))
        {
            GUITAB_SetCurSel(MMIFMM_TAB_CTRL_ID,cur_tab_index);
        }
        MMK_CreateChildWin (
            MMIFMM_MAIN_WIN_ID,
            (uint32*) MMIFMM_UDISK_CHILD_WIN_TAB,
            (ADD_DATA) main_d_ptr);
#ifdef MSDC_CARD_SUPPORT

        if (MMIAPISD_IsSDNeedFormat (main_d_ptr->s_file_path_info_ptr->s_cur_dev))
        {
            //MMIFMM_FmmMemoryFormat(main_d_ptr->s_file_path_info_ptr->s_cur_dev, TXT_IS_FORMAT_ERROR_SDCARD);
            MMI_TEXT_ID_T alert_text_id = TXT_IS_FORMAT_ERROR_SDCARD;

            MMIFMM_FmmMemoryFormat (main_d_ptr->s_file_path_info_ptr->s_cur_dev, alert_text_id);
        }

#endif
        MMK_SendMsg (MMIFMM_UDISK_CHILD_WIN_ID, MSG_FMM_UPDATE_LIST_DATA, PNULL);

        //update current path
        if (main_d_ptr->s_file_path_info_ptr->user_filename_valid)
        {
            UpdateFileList (main_d_ptr->s_file_path_info_ptr->win_id, FALSE);
        }

        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_LCD_SWITCH:
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseParentWin (win_id);
        break;

    case MSG_FMM_DELETE_END_CNF:
        MMK_PostMsg (MMIFMM_UDISK_CHILD_WIN_ID, MSG_FMM_DELETE_END_CNF, PNULL, 0);

        break;
    case MSG_FMM_COPY_END_CNF:
        MMK_PostMsg (MMIFMM_UDISK_CHILD_WIN_ID, MSG_FMM_COPY_END_CNF, PNULL, 0);

        break;

    case MSG_FMM_MOVE_END_CNF:
        MMK_PostMsg (MMIFMM_UDISK_CHILD_WIN_ID, MSG_FMM_MOVE_END_CNF, PNULL, 0);
        break;

    case MSG_CLOSE_WINDOW:
        MMIFMM_FreeFileInfo (main_d_ptr->s_fmm_list_data_ptr, TRUE);

        //MMIFMM_SetDisplayFileType(MMIFMM_FILE_ALL);
#ifdef  VIDEOTHUMBNAIL_SUPPORT
        MMIFMM_VideoThumbnailClose();
#endif
        MainWinCloseCtrlFunc (win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : HandleUdiskChildWindow
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUdiskChildWindow (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{

    MMI_RESULT_E               result = MMI_RESULT_TRUE;
    uint16                     max_num = 0;
    uint16                     index = 0;
    uint32                      cur_tab = 0;
    uint16                     img_width = 0;
    MMI_CTRL_ID_T              list_ctrl_id = MMIFMM_UDISK_LISTBOX_CTRL_ID;
    MMIFILE_DEVICE_E           base_dev  = MMI_DEVICE_UDISK;
#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    GUIFORM_CHILD_WIDTH_T  child_width = {0};
    GUI_BG_T               bg_info  = {0};
    uint16                 hor_space = 0;
    uint16                 ver_space = 0;
    uint16                 back_button_w = 0;
    uint16                 back_button_h = 0;
#else
    GUI_BOTH_RECT_T            list_rect = {0};
#endif /* MMI_PDA_SUPPORT */
    MMIFMM_VIEW_WIN_DATA_T* view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);/*lint !e64 */
    //BOOLEAN                    is_update = FALSE;
    //SCI_TRACE_LOW:"MMIFMM: HandleUdiskChildWindow, win_id =0x%x, msg_id = 0x%x."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_9191_112_2_18_2_20_54_488, (uint8*) "dd", win_id, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        if (MMI_RESULT_FALSE == ListViewWinOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

        if (PNULL == view_win_d)
        {
            MMK_CloseWin (win_id);
            break;
        }

        cur_tab = GUITAB_GetCurSel (MMIFMM_TAB_CTRL_ID);

#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIRES_GetImgWidthHeight (&back_button_w, &back_button_h, IMAGE_COMMON_BACK, win_id);
        GUIFORM_IsSlide (MMIFMM_UDISK_FORM_CTRL_ID, FALSE);
        GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildHeight (MMIFMM_UDISK_FORM_CTRL_ID, list_ctrl_id, &child_height);
        GUIFORM_PermitChildBg (MMIFMM_UDISK_FORM_CTRL_ID, FALSE);
        GUIFORM_PermitChildBorder (MMIFMM_UDISK_FORM_CTRL_ID, FALSE);
        GUIFORM_SetSpace (MMIFMM_UDISK_FORM_CTRL_ID, &hor_space, &ver_space);
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = back_button_w;
        GUIFORM_SetChildWidth (MMIFMM_UDISK_TITLE_BUTTON_FORM_CTRL_ID, MMIFMM_UDISK_TITLE_BUTTON_CTRL_ID, &child_width);
        GUIBUTTON_SetCallBackFuncExt (MMIFMM_UDISK_TITLE_BUTTON_CTRL_ID, TitleButtonCallBack);
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id = IMAGE_COMMON_BACK_HIGHLIGHT;

        //GUIFORM_PermitChildBg(MMIFMM_UDISK_TITLE_BUTTON_FORM_CTRL_ID, TRUE);
        GUIBUTTON_SetPressedBg (MMIFMM_UDISK_TITLE_BUTTON_CTRL_ID, &bg_info);
        GUIBUTTON_SetRunSheen (MMIFMM_UDISK_TITLE_BUTTON_CTRL_ID, FALSE);
        //GUIFORM_SetBg(MMIFMM_UDISK_TITLE_BUTTON_FORM_CTRL_ID, &bg_info);
#else
        list_rect = MMITHEME_GetWinClientBothRect (win_id);
#endif

        if (cur_tab < MMI_DEVICE_NUM)
        {
#ifndef MMI_FMM_TITLE_BUTTON_SUPPORT
            GUIAPICTRL_SetBothRect (list_ctrl_id, &list_rect);
#endif
            // 设置参数
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.leftsoft_id, 
				TXT_NULL, view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.rightsoft_id, FALSE);
			GUIWIN_SeSoftkeytButtonIconId(win_id, view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.middlesoft_id, 1, TRUE);
#else 
			GUIWIN_SetSoftkeyTextId(win_id,  view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.leftsoft_id,
				                     view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.middlesoft_id,
                                     view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.rightsoft_id, FALSE);
#endif
            MMK_SetAtvCtrl (win_id, list_ctrl_id);
        }

        GUIRES_GetImgWidthHeight (&img_width, PNULL, IMAGE_TAB_SELECT2, win_id);
//        GUITAB_SetItemWidth(MMIFMM_TAB_CTRL_ID, img_width);
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        FmmHandleListAppendData (win_id, param, list_ctrl_id, MMIFMM_NEED_LIST_ITEM_DATA_TYPE);
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        FmmHandleListAppendData (win_id, param, list_ctrl_id, MMIFMM_NEED_LIST_ITEM_CONTENT_TYPE);
        break;

    case MSG_GET_FOCUS:
#ifdef  VIDEOTHUMBNAIL_SUPPORT

        if (MMIFMM_IsDefaultVideoFolder (view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len))
        {
            if (MMIAPIVP_IsThumbnailOpen())
            {
                FmmVideoThumbnailUpdateIcon(view_win_d->main_d_ptr->s_fmm_list_data_ptr,view_win_d->main_d_ptr->s_fmm_current_path_ptr);
                MMIAPIVP_ThumbnailResume();
            }
            else
            {
                MMIFMM_FmmVideoThumbnailLoadingMovieList (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr);
            }
        }

#endif
		//bug72513,处理DRM版权的即时反馈情况
#ifdef DRM_SUPPORT
		if (PNULL != view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[GUILIST_GetCurItemIndex(list_ctrl_id)])
		{//初次进入fmm时防止assert
			if (MMIFMM_FILE_TYPE_DRM == view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[GUILIST_GetCurItemIndex(list_ctrl_id)]->type)
			{
				MMK_SendMsg(win_id, MSG_FMM_LIST_REPLACE_ITEM_DATA, PNULL);
			}
		}
#endif
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState (win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMK_SetAtvCtrl (win_id, list_ctrl_id);
        break;
    case MSG_LOSE_FOCUS:
#ifdef VIDEOTHUMBNAIL_SUPPORT

        if (MMIFMM_IsDefaultVideoFolder (view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len))
        {
            MMIAPIVP_ThumbnailClose();
        }

#endif
        break;
    case MSG_CTL_TAB_SWITCH:
        //tab窗口切换，切换到另外的存储设备
        GUIRES_GetImgWidthHeight (&img_width, PNULL, IMAGE_TAB_SELECT2, win_id);

        SCI_MEMSET (&view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev],
                    0, sizeof (view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev]));/*lint !e656 */
        SaveCurrentPath (&view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev],/*lint !e656 */
                         view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (list_ctrl_id));/*lint !e656 */

        cur_tab = GUITAB_GetCurSel (MMIFMM_TAB_CTRL_ID);
        //if(cur_tab >= MMI_DEVICE_NUM)
        //{
        //    SCI_TRACE_LOW("[MMIFMM] HandleUdiskChildWindow MSG_CTL_TAB_SWITCH cur_tab %d !", cur_tab);
        //	break;
        //}
        view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev =  view_win_d->main_d_ptr->s_file_path_info_ptr->s_device_table[cur_tab];

        if (view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev >= MMI_DEVICE_NUM)
        {
            //SCI_TRACE_LOW:"[MMIFMM] HandleUdiskChildWindow MSG_CTL_TAB_SWITCH cur_tab %d ,view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev %d!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_9284_112_2_18_2_20_54_489, (uint8*) "dd", cur_tab, view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev);
            break;
        }

        SCI_MEMSET (view_win_d->main_d_ptr->s_fmm_current_path_ptr, 0, sizeof (MMIFMM_PATH_INFO_T));

        if (0 == MMIAPICOM_Wstrlen (view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev].pathname)) /*lint !e656 */
        {
            // 第一进入某个存储卡
            view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len = FmmInitDevicePath (view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname);
            view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth     = 1;

        }
        else
        {
            // 恢复上次保存的路径
            view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth = view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev].path_depth;/*lint !e656 *///(uint16)view_win_d->s_fmm_path_ptr->device[view_win_d->s_fmm_path_ptr->current_device].depth;
            view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len = view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev].pathname_len;/*lint !e656 *///view_win_d->s_fmm_path_ptr->device[view_win_d->s_fmm_path_ptr->current_device].path[view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth -1].pathname_len;
            MMIAPICOM_Wstrncpy (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname,
                                view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev].pathname,/*lint !e656 */
                                view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len);
        }

        MMIAPICOM_Wstrncpy (view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len);
        view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len = view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len;
        view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_depth = view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth;

        //首先判断存储卡格式是否支持
        if (!MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev), view_win_d->main_d_ptr->s_file_path_info_ptr->s_device_table[cur_tab]))
        {
#ifdef MSDC_CARD_SUPPORT

            if (MMIAPISD_IsSDNeedFormat (view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev))
            {
                //格式错误，提示是否格式化

                MMIFMM_FmmMemoryFormat (view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev, TXT_IS_FORMAT_ERROR_SDCARD);
                break;
            }
            else
#endif
            {
                //其他错误
                MMIPUB_OpenAlertWinByTextId (PNULL, MMIAPIFMM_GetDeviceName (view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev), TXT_COMMON_FORMAT_ERROR, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }

        // 更新数据
        MMIFMM_RemoveListData (list_ctrl_id);
        {
            //  MMK_PostMsg(MMIFMM_UDISK_CHILD_WIN_ID, MSG_FMM_UPDATE_LIST_DATA, PNULL, 0);
            UpdateListData (win_id, list_ctrl_id,
                            MMIFMM_READ_TYPE_FILE_DIR,
                            view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.leftsoft_id, TRUE);

            if (!view_win_d->main_d_ptr->s_file_path_info_ptr->user_filename_valid)
            {
                index = view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev].reserved_pos;/*lint !e656 *///(uint16)view_win_d->s_fmm_path_ptr->device[view_win_d->s_fmm_path_ptr->current_device].path[view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth -1].reserved_pos;
            }
            else
            {
                index = GetFileIndex (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_file_path_info_ptr->file_name, view_win_d->main_d_ptr->s_file_path_info_ptr->file_len);
                view_win_d->main_d_ptr->s_file_path_info_ptr->user_filename_valid = FALSE;
            }

            //重新设置光标的位置
            GUILIST_SetCurItemIndex (list_ctrl_id, index);
            MMK_UpdateScreen();
            MMIPUB_CloseAlertWin();
        }

#ifdef MMI_PDA_SUPPORT
        //每次切换tab时，都让softkey隐藏起来,并标志出对应标志信息
        GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
        break;

        //case MSG_FULL_PAINT:
        //  break;

    case MSG_CTL_LIST_MARK_ITEM:
        HandleMarkItem (win_id, param);
        break;
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        {
            DealWithUdiskLongOkMenu (win_id, GUILIST_GetCurItemIndex (list_ctrl_id));
        }

        break;
#endif

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        //SCI_TRACE_LOW:"MMIFMM: HandleUdiskChildWindow, call relative menu!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_9374_112_2_18_2_20_55_490, (uint8*) "");
        HandleCreatePopMenuOpt (win_id, GUILIST_GetCurItemIndex (list_ctrl_id));
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        HandleCancelOpt (win_id, list_ctrl_id);
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_KEYLONG_CANCEL:
        MMK_CloseParentWin (MMIFMM_MAIN_WIN_ID);
        break;
#endif
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
        {
            //MMI_CTRL_ID_T       ctrl_id = 0;

            if (PNULL != param)
            {
                view_win_d->com_ctrl_id = ( (MMI_NOTIFY_T*) param)->src_id;

                if (MMIFMM_UDISK_TITLE_LABEL_CTRL_ID == view_win_d->com_ctrl_id)
                {
                    break;
                }

                HandlePDAMarkOperate (win_id);
            }
        }
#else
        HandleListItem (win_id);//view_win_d->main_d_ptr->s_fmm_list_data_ptr,view_win_d->main_d_ptr->s_fmm_current_path_ptr,list_ctrl_id);
#endif
        break;
#endif

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:

        if (MMIFMM_FILE_STATE_UNCHECKED == view_win_d->main_d_ptr->s_fmm_list_data_ptr->checked)
        {
            HandleListItem (win_id);//view_win_d->main_d_ptr->s_fmm_list_data_ptr,view_win_d->main_d_ptr->s_fmm_current_path_ptr,list_ctrl_id);
        }
        else
        {
            max_num = GUILIST_GetSelectedItemIndex (list_ctrl_id, PNULL, 0);

            if (0 == max_num)
            {
                GUILIST_SetTextListMarkable (list_ctrl_id, FALSE);
            }
        }

        break;
    case MSG_FMM_NEW_FOLDER_CNF:
        // 更新数据
        UpdateListData (win_id, list_ctrl_id, MMIFMM_READ_TYPE_FILE_DIR, STXT_OPTION, TRUE);

        // 获得光标的位置
        index = GetNextIndex (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->s_list_file_name_ptr, view_win_d->s_list_file_name_len);
        view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev].reserved_pos = index;/*lint !e656 */
        //view_win_d->s_fmm_path_ptr->device[view_win_d->s_fmm_path_ptr->current_device].path[view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth -1].reserved_pos = index;

        //重新设置光标的位置
        GUILIST_SetCurItemIndex (list_ctrl_id, index);
        MMK_UpdateScreen();
        break;

    case MSG_FMM_RENAME_END_CNF:

        if (MMK_IsOpenWin (MMIFMM_PICTURE_PREVIEW_WIN_ID))
        {
            //关闭图像浏览窗口
            MMK_CloseWin (MMIFMM_PICTURE_PREVIEW_WIN_ID);
        }

        {
            MMIFMM_RENAME_RETURN_T *rename_param = (MMIFMM_RENAME_RETURN_T *) param;
            SCI_MEMSET (view_win_d->s_list_file_name_ptr, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) * sizeof (wchar)));
            MMIAPICOM_Wstrncpy (view_win_d->s_list_file_name_ptr, rename_param->file_name_ptr, MIN (rename_param->file_name_len, MMIFMM_FULL_FILENAME_LEN));
            // 更新数据
            UpdateRenameData (win_id, list_ctrl_id, view_win_d->s_list_file_name_ptr, STXT_OPTION);
			SCI_TRACE_BUF(" mmifmm_mainwin.c rename_param->file_name_ptr",rename_param->file_name_ptr, rename_param->file_name_len);
			SCI_TRACE_BUF(" mmifmm_mainwin.c view_win_d->s_list_file_name_ptr",view_win_d->s_list_file_name_ptr,rename_param->file_name_len);
			//MMK_PostMsg (win_id, MSG_FMM_UPDATE_LIST_DATA, PNULL, 0);
        }

        // 刷新当前窗口
        if (MMK_GetFocusWinId() == win_id)
        {
            MMK_PostMsg (win_id, MSG_FULL_PAINT, PNULL, 0);
        }

        break;

    case MSG_FMM_DELETE_END_CNF:

        // 更新数据
        if (MMK_IsOpenWin (MMIFMM_PICTURE_PREVIEW_WIN_ID))
        {
            //关闭图像浏览窗口
            MMK_CloseWin (MMIFMM_PICTURE_PREVIEW_WIN_ID);
        }

        UpdateListData (win_id, list_ctrl_id, MMIFMM_READ_TYPE_FILE_DIR, STXT_OPTION, TRUE);

        //重新设置光标的位置
      //  index = view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev].reserved_pos;/*lint !e656 */
        //index =(uint16)view_win_d->s_fmm_path_ptr->device[view_win_d->s_fmm_path_ptr->current_device].path[view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth -1].reserved_pos;
        max_num = GUILIST_GetTotalItemNum (list_ctrl_id);
		index   = GUILIST_GetCurItemIndex(list_ctrl_id);

		 
        if (index < max_num)
        {
            GUILIST_SetCurItemIndex (list_ctrl_id, index);
        }
        else if (0 < max_num)
        {
            GUILIST_SetCurItemIndex (list_ctrl_id, (uint16) (max_num - 1));
        }
        else
        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  
				view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.leftsoft_id, 
				TXT_NULL, 
				view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.rightsoft_id,
				FALSE);
			GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
			GUIWIN_SetSoftkeyTextId(win_id,  
				view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.leftsoft_id, 
				view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.middlesoft_id, 
				view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.rightsoft_id,
				FALSE);
#endif

        }

#ifdef MMI_PDA_SUPPORT
        {
            GUIFORM_CHILD_DISPLAY_E  display_type = GUIFORM_CHILD_DISP_NORMAL;
            GUIFORM_GetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);

            if (GUIFORM_CHILD_DISP_NORMAL == display_type)
            {
                GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                GUILIST_SetTextListMarkable (MMIFMM_UDISK_LISTBOX_CTRL_ID, FALSE);//cr134402
                HandleMenuCancelAllMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, MMIFMM_UDISK_LISTBOX_CTRL_ID);
            }
        }
#endif
        MMK_UpdateScreen();
        break;

    case MSG_FMM_SEARCH_CNF:
        // 更新数据
        view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_is_valid = TRUE;

        UpdateListData (win_id, list_ctrl_id, MMIFMM_READ_TYPE_FILE_DIR, STXT_OPTION, FALSE);

        view_win_d->main_d_ptr->s_fmm_list_data_ptr->state = MMIFMM_FMI_STATE_LOOKUP_FILE;
        MMK_UpdateScreen();
        MMK_CloseWin (MMIFMM_FIND_FILE_WIN_ID);
        MMIPUB_CloseAlertWin();
        break;

    case MSG_FMM_SORT_END_CNF:
        // 更新数据
        UpdateListData (win_id, list_ctrl_id, MMIFMM_READ_TYPE_FILE_DIR, STXT_OPTION, FALSE);

        MMK_UpdateScreen();
        break;

    case MSG_FMM_COPY_END_CNF:

        if (MMIFMM_FILE_STATE_UNCHECKED != view_win_d->main_d_ptr->s_fmm_list_data_ptr->checked)
        {
            uint32  i = 0;

            //去掉标志状态
            for (i = 0; i < (uint16) (view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num); i++)
            {
                view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[i]->state = MMIFMM_FILE_STATE_UNCHECKED;
            }

            view_win_d->main_d_ptr->s_fmm_list_data_ptr->checked = MMIFMM_FILE_STATE_UNCHECKED;
            view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num = 0;
            GUILIST_SetTextListMarkable (list_ctrl_id, FALSE);
        }

        // 更新数据
        UpdateListData (win_id, list_ctrl_id, MMIFMM_READ_TYPE_FILE_DIR, STXT_OPTION, TRUE);

        // 获得光标的位置
        index = (uint16) view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev].reserved_pos;/*lint !e656 */
        //index = (uint16)view_win_d->s_fmm_path_ptr->device[view_win_d->s_fmm_path_ptr->current_device].path[view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth -1].reserved_pos;

        //重新设置光标的位置
        GUILIST_SetCurItemIndex (list_ctrl_id, index);
#ifdef MMI_PDA_SUPPORT
        {
            GUIFORM_CHILD_DISPLAY_E  display_type = GUIFORM_CHILD_DISP_NORMAL;
            GUIFORM_GetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);

            if (GUIFORM_CHILD_DISP_NORMAL == display_type)
            {
                GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                GUILIST_SetTextListMarkable (MMIFMM_UDISK_LISTBOX_CTRL_ID, FALSE);//cr134402
                HandleMenuCancelAllMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, MMIFMM_UDISK_LISTBOX_CTRL_ID);
            }
        }
#endif
        MMK_UpdateScreen();
        MMK_CloseWin (MMIFMM_FILE_WAITING_WIN_ID);
        break;

    case MSG_FMM_MOVE_END_CNF:

        if (MMIFMM_FILE_STATE_UNCHECKED != view_win_d->main_d_ptr->s_fmm_list_data_ptr->checked)
        {
            uint32  i = 0;

            //去掉标志状态
            for (i = 0; i < (uint16) (view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num); i++)
            {
                view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[i]->state = MMIFMM_FILE_STATE_UNCHECKED;
            }

            view_win_d->main_d_ptr->s_fmm_list_data_ptr->checked = MMIFMM_FILE_STATE_UNCHECKED;
            view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num = 0;
            GUILIST_SetTextListMarkable (list_ctrl_id, FALSE);
        }

        // 更新数据
        UpdateListData (win_id, list_ctrl_id, MMIFMM_READ_TYPE_FILE_DIR, STXT_OPTION, TRUE);

        //重新设置光标的位置
        GUILIST_SetCurItemIndex (list_ctrl_id, 0);
#ifdef MMI_PDA_SUPPORT
        {
            GUIFORM_CHILD_DISPLAY_E display_type = GUIFORM_CHILD_DISP_NORMAL;
            GUIFORM_GetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);

            if (GUIFORM_CHILD_DISP_NORMAL == display_type && !MMIFMM_IsCopyFromMove())
            {
                GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                GUILIST_SetTextListMarkable (list_ctrl_id, FALSE);
                HandleMenuCancelAllMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, list_ctrl_id);
            }
        }
#endif
        MMK_UpdateScreen();
        //MMK_CloseWin(MMIFMM_FILE_WAITING_WIN_ID);
        break;

    case MSG_FMM_UPDATE_LIST_DATA:
        // 更新数据

        UpdateListData (win_id, list_ctrl_id, MMIFMM_READ_TYPE_FILE_DIR,
                        view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.leftsoft_id, TRUE);

        if (!view_win_d->main_d_ptr->s_file_path_info_ptr->user_filename_valid)
        {
            index = (uint16) view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev].reserved_pos;/*lint !e656 */
        }
        else
        {
            index = GetFileIndex (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_file_path_info_ptr->file_name, view_win_d->main_d_ptr->s_file_path_info_ptr->file_len);
            view_win_d->main_d_ptr->s_file_path_info_ptr->user_filename_valid = FALSE;
        }

        //重新设置光标的位置
        GUILIST_SetCurItemIndex (list_ctrl_id, index);
        GUILIST_SetTopItemIndex (list_ctrl_id, index);

        if (view_win_d->main_d_ptr->s_file_path_info_ptr->is_need_display)
        {
            view_win_d->main_d_ptr->s_file_path_info_ptr->is_need_display = FALSE;
            HandleListItem (win_id);
            //HandleListItem(view_win_d->main_d_ptr->s_fmm_list_data_ptr,view_win_d->main_d_ptr->s_fmm_current_path_ptr,list_ctrl_id);
        }

        if (param && MMK_IsFocusWin (win_id))
        {
            MMK_UpdateScreen();
        }

        MMIPUB_CloseAlertWin();
        break;
		//bug21172 (22001)处理DRM版权的即时反馈情况并时聚焦
#ifdef DRM_SUPPORT
	case MSG_FMM_LIST_REPLACE_ITEM_DATA://bug63821
		{
			if (MMIFMM_FILE_TYPE_DRM == view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[GUILIST_GetCurItemIndex(list_ctrl_id)]->type)
			{//只对DRM文件进行处理
				GUILIST_ITEM_T       *replace_item_t_ptr    =  PNULL;
				GUILIST_ITEM_DATA_T  replace_item_data = {0};
				uint16 replace_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
				
                replace_item_t_ptr = SCI_ALLOC_APPZ(sizeof(GUILIST_ITEM_T));
				if(PNULL != replace_item_t_ptr)
				{
					SCI_MEMCPY(replace_item_t_ptr,GUILIST_GetItemPtrByIndex(list_ctrl_id,replace_item_index),sizeof(GUILIST_ITEM_T));
					//获得当前时间下的DRM文件的版权情况(反映在前面图标上)
					replace_item_data.item_content[0].item_data.image_id = MMIFMM_GetIconIdByFileName(view_win_d->main_d_ptr->s_file_path_info_ptr->file_name, MMIAPICOM_Wstrlen(view_win_d->main_d_ptr->s_file_path_info_ptr->file_name));
					if (PNULL != replace_item_t_ptr->item_data_ptr)
					{//用于替换drm item前面的图标，但不更新其他内容
						if(replace_item_data.item_content[0].item_data.image_id!=replace_item_t_ptr->item_data_ptr->item_content[0].item_data.image_id)
						{
							replace_item_t_ptr->item_data_ptr->item_content[0].item_data.image_id = replace_item_data.item_content[0].item_data.image_id;
							GUILIST_ReplaceItem(list_ctrl_id,replace_item_t_ptr,replace_item_index);
						}
					}
					SCI_FREE(replace_item_t_ptr);
					replace_item_t_ptr = PNULL;
				}
			}
		}
   	break;
#endif
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP:
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_PREPAGE:
    case MSG_CTL_LIST_NXTPAGE:
        {
            view_win_d->main_d_ptr->s_file_path_info_ptr->file_len = MMIFMM_FmmGetFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, index, view_win_d->main_d_ptr->s_file_path_info_ptr->file_name, MMIFMM_PATHNAME_LEN);
            Atest_FMMTrace (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, list_ctrl_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        {
            ListViewWinCloseCtrlFunc (win_id);
            result = MMI_RESULT_FALSE;
        }
        break;
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
#ifdef  VIDEOTHUMBNAIL_SUPPORT
        {
            GUILIST_SLIDE_STATE_E  state = * (GUILIST_SLIDE_STATE_E*) param;
            BOOLEAN  is_videofolder = MMIFMM_IsDefaultVideoFolder (view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len);
            //SCI_TRACE_LOW:"MMIFMM: HandleUdiskChildWindow, is_videofolder:%d,state:%d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_9639_112_2_18_2_20_55_491, (uint8*) "dd", is_videofolder, state);

            if (is_videofolder)
            {
                MMIFMM_VideoThumnailChangeState (state, view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr);
            }
        }
#endif
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    switch (msg_id)
    {
    case MSG_FMM_NEW_FOLDER_CNF:
    case MSG_FMM_RENAME_END_CNF:
    case MSG_FMM_DELETE_END_CNF:
    case MSG_FMM_COPY_END_CNF:
        MMIFMM_UpdateEventInd();
        break;
    default :
        break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : update file list
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void UpdateFileList (MMI_WIN_ID_T win_id, BOOLEAN need_fullpiant)
{
    MMI_TEXT_ID_T alert_text_id = TXT_NULL;
    MMIFMM_MAIN_WIN_DATA_T* main_d_ptr = (MMIFMM_MAIN_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL == main_d_ptr || PNULL == main_d_ptr->s_fmm_current_path_ptr)
    {
        SCI_TRACE_LOW ("OpenFmmMainExplorer:PNULL == s_fmm_current_path_ptr");
        return;
    }

    if (MMIFMM_PATH_DEPTH < main_d_ptr->s_file_path_info_ptr->patch_info.path_depth
            || (main_d_ptr->s_file_path_info_ptr->patch_info.pathname_len) > MMIFMM_PATHNAME_LEN)
    {
        if (MMIFMM_PATH_DEPTH < main_d_ptr->s_file_path_info_ptr->patch_info.path_depth)
        {
            alert_text_id = TXT_MAX_DEPTH;
        }
        else
        {
            alert_text_id = TXT_COM_FILE_NAME_TOO_LONG;
        }

        //路径已到最到深度
        MMIPUB_OpenAlertWaitingWin (alert_text_id);
    }
    else
    {
        main_d_ptr->s_fmm_current_path_ptr->path_depth = main_d_ptr->s_file_path_info_ptr->patch_info.path_depth;

        MMIAPICOM_Wstrncpy (main_d_ptr->s_fmm_current_path_ptr->pathname,
                            main_d_ptr->s_file_path_info_ptr->patch_info.pathname,
                            main_d_ptr->s_file_path_info_ptr->patch_info.pathname_len);

        main_d_ptr->s_fmm_current_path_ptr->pathname_len = main_d_ptr->s_file_path_info_ptr->patch_info.pathname_len;
        main_d_ptr->s_fmm_current_path_ptr->pathname[main_d_ptr->s_fmm_current_path_ptr->pathname_len] = 0;

        MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
        MMK_SendMsg (win_id, MSG_FMM_UPDATE_LIST_DATA, (ADD_DATA) need_fullpiant);
    }
}


/*****************************************************************************/
//  Description : open any fmm win with callback function
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_OpenExplorerWin (MMIFMM_OPENWIN_INFO_T * openwin_info)
{
    BOOLEAN result = FALSE;

    if (PNULL == openwin_info)
    {
        return result;
    }
    else
    {
        MMIFILE_DEVICE_E fs_dev = MMI_DEVICE_NUM;

        if (MMIAPIENG_GetIQModeStatus())
        {
            MMIPUB_OpenAlertWarningWin (TXT_IQ_DATA_PROCESSING);
            return FALSE;
        }

        if (MMIAPIUDISK_UdiskIsRun())
        {
            //u disk runing
            MMIPUB_OpenAlertWarningWin (TXT_COMMON_UDISK_USING);
            return FALSE;
        }

        fs_dev = MMIAPIFMM_GetFirstValidDevice();
		
        if (MMI_DEVICE_NUM == fs_dev)
        {
            //no valid device, get first exist devcie
            fs_dev = MMIAPIFMM_GetFirstExistDevice();
        }

        if (MMI_DEVICE_NUM > fs_dev)
        {
            MMIFMM_MAIN_WIN_DATA_T *main_win_p = PNULL;
            MMIAPIFMM_InitFolders();
            //close previous windows
            MMK_CloseWin (MMIFMM_MAIN_WIN_ID);


            main_win_p = SCI_ALLOC_APPZ (sizeof (MMIFMM_MAIN_WIN_DATA_T));

            main_win_p->s_fmm_current_path_ptr = SCI_ALLOC_APPZ (sizeof (MMIFMM_PATH_INFO_T));

            if (PNULL == main_win_p->s_fmm_current_path_ptr)
            {
                SCI_TRACE_LOW ("OpenFmmMainExplorer:PNULL == s_fmm_current_path_ptr");
                return FALSE;
            }

            main_win_p->s_fmm_list_data_ptr = SCI_ALLOC_APPZ (sizeof (MMIFMM_DATA_INFO_T));

            if (PNULL == main_win_p->s_fmm_list_data_ptr)
            {
                SCI_FREE (main_win_p->s_fmm_current_path_ptr);
                SCI_TRACE_LOW ("OpenFmmMainExplorer:PNULL == main_win_p->s_fmm_list_data_ptr!");
                return FALSE;
            }

            main_win_p->s_file_path_info_ptr = (MMIFMM_OPEN_FILE_PATH_INFO_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_OPEN_FILE_PATH_INFO_T));

            main_win_p->s_file_path_info_ptr->is_protect_checked = openwin_info->privacy_protect;
            main_win_p->s_file_path_info_ptr->s_suffix_type = openwin_info->find_suffix_type;

            main_win_p->s_file_path_info_ptr->sk_text_id = openwin_info->sk_text_id;
            if (PNULL != openwin_info->path_ptr && 0 != openwin_info->path_len
                    && (MMIAPIFMM_IsFolderExist (openwin_info->path_ptr, openwin_info->path_len)
                        || MMIAPIFMM_IsFileExist (openwin_info->path_ptr, openwin_info->path_len)
                       )
               )
            {
                MMIAPICOM_Wstrncpy (main_win_p->s_file_path_info_ptr->patch_info.pathname, openwin_info->path_ptr, openwin_info->path_len);
                main_win_p->s_file_path_info_ptr->patch_info.pathname_len = openwin_info->path_len;

                MMIAPICOM_Wstrncpy (main_win_p->s_fmm_current_path_ptr->pathname, openwin_info->path_ptr, openwin_info->path_len);
                main_win_p->s_fmm_current_path_ptr->pathname_len = openwin_info->path_len;

                if (PNULL != openwin_info->filename && 0 != openwin_info->file_len)
                {
                    MMIAPICOM_Wstrncpy (main_win_p->s_file_path_info_ptr->file_name, openwin_info->filename, openwin_info->file_len);
                    main_win_p->s_file_path_info_ptr->file_len = openwin_info->file_len;
                }

                if (openwin_info->file_len > 0)
                {
                    main_win_p->s_file_path_info_ptr->is_need_display = openwin_info->is_need_display;
                    main_win_p->s_file_path_info_ptr->user_filename_valid = TRUE;
                }
                else
                {
                    main_win_p->s_file_path_info_ptr->is_need_display = FALSE;
                    main_win_p->s_file_path_info_ptr->user_filename_valid = FALSE;
                }

                main_win_p->s_file_path_info_ptr->patch_info.path_depth = MMIFMM_GetPathDepth (main_win_p->s_file_path_info_ptr->patch_info.pathname, main_win_p->s_file_path_info_ptr->patch_info.pathname_len);

                if (MMIAPIFMM_GetDeviceStatus (openwin_info->path_ptr, MMIFILE_DEVICE_LEN))
                {
                    //current path device exist
                    fs_dev = MMIAPIFMM_GetDeviceTypeByPath (openwin_info->path_ptr, MMIFILE_DEVICE_LEN);
                }
                else
                {
                    FmmInitDevicePath (fs_dev, main_win_p->s_fmm_current_path_ptr->pathname);
                }

                main_win_p->s_file_path_info_ptr->s_cur_dev = fs_dev;
            }
            else
            {
                main_win_p->s_file_path_info_ptr->patch_info.path_depth = 1;
                main_win_p->s_file_path_info_ptr->s_cur_dev = fs_dev;
                FmmInitDevicePath (fs_dev, main_win_p->s_fmm_current_path_ptr->pathname);
            }

            main_win_p->s_fmm_current_path_ptr->pathname_len = (uint16) MMIAPICOM_Wstrlen (main_win_p->s_fmm_current_path_ptr->pathname);
            main_win_p->s_fmm_current_path_ptr->path_depth = MMIFMM_GetPathDepth (main_win_p->s_fmm_current_path_ptr->pathname, main_win_p->s_fmm_current_path_ptr->pathname_len);

            if (FmmCheckMemoryCardState())
            {
                if (!main_win_p->s_file_path_info_ptr->is_protect_checked && MMIAPISET_IsOpenPrivacyProtect (MMISET_PROTECT_MYDOC))
                {
                    MMIAPISET_ValidatePrivacy (MMISET_PROTECT_MYDOC, FMM_ValidatePrivacyFmmDoc, (ADD_DATA) main_win_p, sizeof (MMIFMM_MAIN_WIN_DATA_T));
                    SCI_FREE(main_win_p);
                }
                else
                {
                    CreateExplorerWin (main_win_p);
                }
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin (TXT_PLEASE_INSERT_SD);
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : validate privacy fmm doc
//	Global resource dependence :
//  Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL void FMM_ValidatePrivacyFmmDoc (BOOLEAN is_permitted, DPARAM param_ptr)
{
    if (is_permitted)
    {
        MMIFMM_MAIN_WIN_DATA_T *callback_d = (MMIFMM_MAIN_WIN_DATA_T *) param_ptr;
        MMIFMM_MAIN_WIN_DATA_T* main_win_p = PNULL;
        main_win_p = SCI_ALLOC_APPZ (sizeof (MMIFMM_MAIN_WIN_DATA_T));
        SCI_MEMCPY(main_win_p,callback_d,sizeof (MMIFMM_MAIN_WIN_DATA_T));
        main_win_p->s_file_path_info_ptr->is_protect_checked = TRUE;
        CreateExplorerWin (main_win_p);
    }
}

/*****************************************************************************/
//  Description : Initialize device path
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint16 FmmInitDevicePath (
    MMIFILE_DEVICE_E      device,     // [IN]
    wchar               *path_ptr   //[IN]/[OUT]
)
{
    uint16 i  = 0;
    //SCI_ASSERT(PNULL != path_ptr);

    //SCI_TRACE_LOW:"MMIFMM: FmmInitDevicePath, device =%d."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3204_112_2_18_2_20_41_370, (uint8*) "d", device);

    if (device < MMI_DEVICE_NUM && (PNULL != path_ptr))
    {
        MMIAPICOM_Wstrncpy (path_ptr, MMIAPIFMM_GetDevicePath (device), MMIAPIFMM_GetDevicePathLen (device));
        i++;
        path_ptr[i] = MMIFILE_COLON;
        i++;
        path_ptr[i] = 0;
    }

    return (i);
}


/*****************************************************************************/
//  Description : fmm check memory card state
//  Global resource dependence : none
//  Author: jian.ma
//  Note:TRUE
/*****************************************************************************/
LOCAL BOOLEAN FmmCheckMemoryCardState (void)
{
    BOOLEAN              result = FALSE;

    if (MMI_DEVICE_NUM <= MMIAPIFMM_GetFirstExistDevice())
    {
        //no devcie exist
        MMIPUB_OpenAlertWarningWin (TXT_NO_SD_CARD_ALERT);
        return result;
    }
    else
    {
        result = TRUE;
    }

    //SCI_TRACE_LOW:"FmmCheckMemoryCardState result = %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7425_112_2_18_2_20_50_463, (uint8*) "d", result);
    return result;
}
/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MainWinOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_MAIN_WIN_DATA_T *main_win_d = (MMIFMM_MAIN_WIN_DATA_T*) MMK_GetWinAddDataPtr (win_id);

    MMK_SetWinUserData (win_id, (void *) main_win_d);

    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MainWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_MAIN_WIN_DATA_T *win_data_p = (MMIFMM_MAIN_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != win_data_p)
    {
        if (PNULL != win_data_p->s_fmm_list_data_ptr)
        {
            SCI_FREE (win_data_p->s_fmm_list_data_ptr);
        }

        if (PNULL != win_data_p->s_fmm_current_path_ptr)
        {
            SCI_FREE (win_data_p->s_fmm_current_path_ptr);
        }

        if (PNULL != win_data_p->s_file_path_info_ptr)
        {
            SCI_FREE (win_data_p->s_file_path_info_ptr);
        }

        SCI_FREE (win_data_p);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListViewWinOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_MAIN_WIN_DATA_T *main_win_d = (MMIFMM_MAIN_WIN_DATA_T*) MMK_GetWinAddDataPtr (win_id);
    MMIFMM_VIEW_WIN_DATA_T *view_win_d = PNULL;

    if (PNULL != main_win_d)
    {
        view_win_d = (MMIFMM_VIEW_WIN_DATA_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_VIEW_WIN_DATA_T));
        view_win_d->main_d_ptr = main_win_d;

        view_win_d->s_fmm_path_ptr = SCI_ALLOC_APPZ (sizeof (MMIFMM_PATH_INFO_T) * (MMI_DEVICE_NUM - 1));

        if (PNULL == view_win_d->s_fmm_path_ptr)
        {
            SCI_FREE (view_win_d);
            SCI_TRACE_LOW ("HandleUdiskChildWindow PNULL == view_win_d->s_fmm_path_ptr");
            return MMI_RESULT_FALSE;
        }

        view_win_d->s_list_file_name_ptr = SCI_ALLOC_APPZ (sizeof (wchar) * (MMIFMM_FILENAME_LEN + 1));
        view_win_d->list_ctrl_id = MMIFMM_UDISK_LISTBOX_CTRL_ID;
        view_win_d->com_ctrl_id = 0;
        view_win_d->tab_ctrl_id = MMIFMM_TAB_CTRL_ID;
    }

    MMK_SetWinUserData (win_id, (void *) view_win_d);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListViewWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_VIEW_WIN_DATA_T *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != view_win_d)
    {
        if (PNULL != view_win_d->s_list_file_name_ptr)
        {
            SCI_FREE (view_win_d->s_list_file_name_ptr);
        }

        if (PNULL != view_win_d->s_fmm_path_ptr)
        {
            SCI_FREE (view_win_d->s_fmm_path_ptr);
        }

        if (PNULL != view_win_d->s_pic_index_data_ptr)
        {
            SCI_FREE (view_win_d->s_pic_index_data_ptr);
        }

        SCI_FREE (view_win_d);
    }

    return MMI_RESULT_TRUE;
}


#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
/*****************************************************************************/
//  Description :SetTitleLabelParam
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void SetTitleLabelParam (MMIFMM_PATH_INFO_T  *current_path_ptr)
{
    MMI_CTRL_ID_T       label_ctrl_id = MMIFMM_UDISK_TITLE_LABEL_CTRL_ID;
    MMI_STRING_T        title_string = {0};
    //wchar               title1[MMIFMM_WIN_TITLE_MAX_LEN + 1] = {0};
	 wchar               *title1_ptr = PNULL;

    if (PNULL == current_path_ptr)
    {
        return;
    }
	
	if(0 == current_path_ptr->pathname_len)
	{
		SCI_TRACE_LOW("[mmifmm_wintab.c][SetTitleLabelParam]:current_path_ptr->pathname_len = 0");
		return;
	}
	
	title1_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN + 1));	
	if (PNULL == title1_ptr)	
	{		
		SCI_TRACE_LOW("[mmifmm_wintab.c][SetTitleLabelParam]:PNULL == title1_ptr");
		return;	
	}   

    if (current_path_ptr->path_depth == 1)
    {
        MMIAPICOM_Wstrncpy (title1_ptr, current_path_ptr->pathname, current_path_ptr->pathname_len);
        MMIAPICOM_Wstrcat (title1_ptr, L"\\");
        title_string.wstr_ptr = title1_ptr;
        title_string.wstr_len = current_path_ptr->pathname_len + 1;
    }
    else
    {
		MMIFMM_ComputTitleShow(0,&title_string,current_path_ptr->pathname,title1_ptr);
#if 0
        uint16              i = 0;
        uint16              title_len = 0;
        GUISTR_STYLE_T      font_style = {0};
        GUISTR_STATE_T      state      = GUISTR_STATE_ALIGN | GUISTR_STATE_ANGLE;
        uint16              str_width = 0;
        uint16              label_width = 0;
        uint16              logic_width = 0;
        uint16              logic_height = 0;
        uint16              back_button_w = 0;
        uint16              back_button_h = 0;
        font_style.font = MMI_DEFAULT_TEXT_FONT;
        title_string.wstr_ptr = current_path_ptr->pathname;
        title_string.wstr_len = current_path_ptr->pathname_len;
        GUILCD_GetLogicWidthHeight (GUI_MAIN_LCD_ID, &logic_width, &logic_height);
        str_width = GUISTR_GetStringWidth (&font_style, &title_string, state);
        GUIRES_GetImgWidthHeight (&back_button_w, &back_button_h, IMAGE_COMMON_BACK, MMIFMM_UDISK_CHILD_WIN_ID);
        label_width = logic_width - back_button_w - MMIFMM_TITLE_LABEL_MARGIN;

        if (str_width > label_width)
        {
            MMIAPICOM_Wstrncpy (title1, MMIFMM_ELLIPSIS, MMIFMM_ELLIPSIS_LEN);
            i = current_path_ptr->pathname_len + MMIFMM_ELLIPSIS_LEN - MMIFMM_WIN_TITLE_MAX_LEN;
            title_len = MMIFMM_WIN_TITLE_MAX_LEN - MMIFMM_ELLIPSIS_LEN;

            MMIAPICOM_Wstrncpy (&title1[MMIFMM_ELLIPSIS_LEN], &current_path_ptr->pathname[i],  title_len);

            title_string.wstr_ptr = title1;
            title_string.wstr_len = (uint16) (title_len + MMIFMM_ELLIPSIS_LEN);
        }
#endif
    }

    GUILABEL_SetText (label_ctrl_id, &title_string, FALSE);
	if (PNULL != title1_ptr)	/*lint !e774*/
	{		
		SCI_Free(title1_ptr);
		title1_ptr = PNULL;	
	}
}
/*****************************************************************************/
//  Description :title button call back
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E   TitleButtonCallBack (MMI_HANDLE_T ctrl_handle)
{
    HandleCancelOpt (MMIFMM_UDISK_CHILD_WIN_ID, MMIFMM_UDISK_LISTBOX_CTRL_ID);
    //HandleCancelOpt(MMIFMM_UDISK_CHILD_WIN_ID,MMIFMM_UDISK_LISTBOX_CTRL_ID);
    return MMI_RESULT_TRUE;
}
#endif

/*****************************************************************************/
//  Description : handle list append data
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FmmHandleListAppendData (
    MMI_WIN_ID_T         win_id,
    DPARAM param,
    MMI_HANDLE_T ctrl_handle,
    MMIFMM_LIST_ITEM_TYPE_E type
)
{
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = PNULL;
    uint16 index = 0;
    MMIFMM_VIEW_WIN_DATA_T *view_win_d = PNULL;
    MMI_HANDLE_T list_handle = MMK_ConvertIdToHandle (ctrl_handle);
	//添加PICTHUMBNAIL_SUPPORT控制
    BOOLEAN is_two_line      = FALSE;
#ifdef  VIDEOTHUMBNAIL_SUPPORT 
    BOOLEAN  is_videotwo_line = FALSE;
#endif

    view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != view_win_d)
    {
#ifdef MMI_FMM_PICTHUMBNAIL_SUPPORT 
		is_two_line = MMIFMM_IsDefaultPicFolder(view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len);
#endif
#ifdef  VIDEOTHUMBNAIL_SUPPORT
        is_videotwo_line = MMIFMM_IsDefaultVideoFolder (view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len);
        is_two_line = is_two_line || is_videotwo_line;
#endif

        if (MMIFMM_NEED_LIST_ITEM_DATA_TYPE == type)
        {
            need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*) param;

            //SCI_ASSERT( PNULL != need_item_data_ptr );
            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMIFMM:[FmmHandleListAppendData]PNULL == need_item_data_ptr"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11876_112_2_18_2_21_0_534, (uint8*) "");
                return;
            }

            index = need_item_data_ptr->item_index;
        }
        else if (MMIFMM_NEED_LIST_ITEM_CONTENT_TYPE == type)
        {
            need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*) param;

            //SCI_ASSERT( PNULL != need_item_content_ptr );
            if (PNULL == need_item_content_ptr)
            {
                //SCI_TRACE_LOW:"MMIFMM:[FmmHandleListAppendData]PNULL == need_item_content_ptr"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11887_112_2_18_2_21_0_535, (uint8*) "");
                return;
            }

            index = need_item_content_ptr->item_index;
            //s_is_picfolder = MMIFMM_IsDefaultPicFolder(view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname,view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len);
        }
        else
        {
            //SCI_ASSERT(0);
            //SCI_TRACE_LOW:"MMIFMM:[FmmHandleListAppendData] type = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11897_112_2_18_2_21_0_536, (uint8*) "d", type);
            return;
        }

        if (index < view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num)
        {
            AppendTwoLineAnimTextAndTextListItem (win_id,
                                                  list_handle,
                                                  view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.leftsoft_id,
                                                  view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->type,
                                                  view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr,
                                                  view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len,
                                                  view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->file_size,
                                                  view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_is_valid,
                                                  view_win_d->main_d_ptr->s_fmm_current_path_ptr,
                                                  index,
                                                  0,
                                                  type,
                                                  is_two_line);
        }
        else
        {
            //SCI_TRACE_LOW:"FmmHandleListAppendData: index = %d file_num = %d,folder_num = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11918_112_2_18_2_21_0_537, (uint8*) "ddd", index, view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num, view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num);
        }
    }
}

/*****************************************************************************/
//  Description : 保存当前设备的路径
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void SaveCurrentPath (
    MMIFMM_PATH_INFO_T     *path_ptr ,
    MMIFMM_PATH_INFO_T      *current_path_ptr,
    uint16                  index
)
{
    //uint16              path_depth = 0;
    //MMIFILE_DEVICE_E     device = MMI_DEVICE_UDISK;

    //SCI_TRACE_LOW:"MMIFMM: SaveCurrentPath."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2224_112_2_18_2_20_39_340, (uint8*) "");

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[SaveCurrentPath]:PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2227_112_2_18_2_20_39_341, (uint8*) "");
        return;
    }

    if (PNULL == path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[SaveCurrentPath]:PNULL == path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2232_112_2_18_2_20_39_342, (uint8*) "");
        return;
    }


    if (current_path_ptr->path_depth > 0)
    {
        path_ptr->path_depth = current_path_ptr->path_depth;

        if (path_ptr->path_depth > 0)
        {
            path_ptr->reserved_pos = index;
            MMIAPICOM_Wstrncpy (path_ptr->pathname, current_path_ptr->pathname, current_path_ptr->pathname_len);
            path_ptr->pathname_len = current_path_ptr->pathname_len;
        }
    }
}

/*****************************************************************************/
//  Description : 清除列表数据
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_RemoveListData (MMI_CTRL_ID_T ctrl_id)
{
    if (GUILIST_GetTotalItemNum (ctrl_id) > 0)
    {
        if (!MMIFMM_IsListMultiSel())
        {
            GUILIST_SetTextListMarkable (ctrl_id, FALSE);
        }

        GUILIST_RemoveAllItems (ctrl_id);
    }
}


/*****************************************************************************/
//  Description : 更新列表数据
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateListData (
    MMI_WIN_ID_T           win_id,
    MMI_CTRL_ID_T          ctrl_id,
    MMIFMM_READ_TYPE_E     type,
    MMI_TEXT_ID_T          sfk_left_id,
    BOOLEAN                is_empty
)
{
    uint16      max_num = 0; 

    MMIFMM_SORT_TYPE_E  sort = MMIFMM_SORT_TYPE_NAME;
    MMIFMM_VIEW_WIN_DATA_T *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);
    //SCI_TRACE_LOW:"MMIFMM: UpdateListData, win_id =%d, ctrl_id = %d, type = %d."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2368_112_2_18_2_20_39_347, (uint8*) "ddd", win_id, ctrl_id, type);

    if (PNULL == view_win_d)
    {
        //SCI_TRACE_LOW:"MMIFMM:[UpdateListData]:param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2373_112_2_18_2_20_39_348, (uint8*) "");
        return;
    }

    if (view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth <= 1)
    {
        GUIFORM_SetChildDisplay (MMIFMM_UDISK_TITLE_BUTTON_FORM_CTRL_ID, MMIFMM_UDISK_TITLE_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {
        GUIFORM_SetChildDisplay (MMIFMM_UDISK_TITLE_BUTTON_FORM_CTRL_ID, MMIFMM_UDISK_TITLE_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    }

    max_num = GUILIST_GetTotalItemNum (ctrl_id);

    if (max_num > 0)
    {
        GUILIST_SetTextListMarkable (ctrl_id, FALSE);
        GUILIST_RemoveAllItems (ctrl_id);
    }

    //保存排序类型
    sort = MMIFMM_GeFileSortType();

    if (is_empty)
    {
        FmmUpdateValidPath (win_id, FALSE);

        ///MMIFMM_FreeFileInfoBuffer(FALSE);
        MMIFMM_FreeFileInfo (view_win_d->main_d_ptr->s_fmm_list_data_ptr, FALSE);
        SCI_MEMSET (view_win_d->main_d_ptr->s_fmm_list_data_ptr , 0, sizeof (MMIFMM_DATA_INFO_T));

        MMIAPICOM_Wstrncpy (view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len);
        view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len = view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len;
        view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_depth = view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth;

        view_win_d->main_d_ptr->s_fmm_list_data_ptr->sort = sort;

        // 读当前路径数据
        max_num = MMIFMM_GetCurrentPathFile (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_list_data_ptr);
    }
    else
    {
        max_num = (uint16) (view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num);
    }

    GUILIST_SetMaxItem (ctrl_id, max_num, TRUE);
    // 增加到listbox 中
    FmmAppendListItem (win_id, ctrl_id,  sfk_left_id, view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr);
#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
    SetTitleLabelParam (view_win_d->main_d_ptr->s_fmm_current_path_ptr);
#else
    SetWinTitle (MMIFMM_NORMAL_TAB_TYPE, win_id, ctrl_id, MMIFMM_TAB_CTRL_ID, view_win_d->main_d_ptr->s_fmm_current_path_ptr);
#endif

    if (max_num > 0)
    {
        Atest_FMMTrace (
            view_win_d->main_d_ptr->s_fmm_list_data_ptr,
            view_win_d->main_d_ptr->s_fmm_current_path_ptr,
            ctrl_id);
    }
    else
    {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
		GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);   
#endif 
    }
}



/*****************************************************************************/
//  Description : get file index when open win
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint16 GetFileIndex (
    MMIFMM_DATA_INFO_T     *list_data_ptr,
    const wchar            *file_name_ptr,
    uint16                 file_name_len
)
{
    uint16      i = 0;
    int32       result = 0;

    //SCI_TRACE_LOW:"MMIFMM: GetFileIndex."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10368_112_2_18_2_20_57_509, (uint8*) "");

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[GetFileIndex]PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10373_112_2_18_2_20_57_510, (uint8*) "");
        return i;
    }

    if (PNULL == file_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[GetFileIndex]PNULL == file_name_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10379_112_2_18_2_20_57_511, (uint8*) "");
        return i;
    }

    for (i = 0; i < list_data_ptr->file_num + list_data_ptr->folder_num; i++)
    {
        result = MMIAPICOM_CompareTwoWstr (list_data_ptr->data[i]->filename_ptr,
                                           list_data_ptr->data[i]->filename_len,
                                           file_name_ptr,
                                           file_name_len
                                          );

        if (0 == result)
        {
            break;
        }
    }

    if (i >= list_data_ptr->file_num + list_data_ptr->folder_num)
    {
        i = 0;
    }

    return (i);
}

/*****************************************************************************/
//  Description : handle mark item function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleMarkItem (MMI_WIN_ID_T win_id, DPARAM param)
{
    GUILIST_MARK_ITEM_T* list_param_ptr = PNULL;
    MMIFMM_VIEW_WIN_DATA_T *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);
    list_param_ptr = (GUILIST_MARK_ITEM_T *) param;

    if (PNULL != view_win_d && PNULL != list_param_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM: HandleMarkItem, MSG_CTL_LIST_MARK_ITEM, index = %d, mark = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11044_112_2_18_2_20_58_521, (uint8*) "dd", list_param_ptr->item_idx, list_param_ptr->had_maked);

        if (list_param_ptr->had_maked)
        {
            if (MMIFMM_FILE_STATE_UNCHECKED == view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[list_param_ptr->item_idx]->state)
            {
                view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num++;
                view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[list_param_ptr->item_idx]->state = MMIFMM_FILE_STATE_CHECKED;

                // 如果标记全部，需要修改总的状态
                if (view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num == (view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num))
                {
                    view_win_d->main_d_ptr->s_fmm_list_data_ptr->checked = MMIFMM_FILE_STATE_ALL_CHECKED;
#ifdef MMI_PDA_SUPPORT
                    GUISOFTKEY_SetTextId (MMIFMM_UDISK_CHILD_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK, TXT_DELETE, STXT_CANCEL, TRUE);
#endif
                }
                else
                {
                    view_win_d->main_d_ptr->s_fmm_list_data_ptr->checked = MMIFMM_FILE_STATE_CHECKED;
                }
            }
            else
            {
                //SCI_TRACE_LOW:"MMIFMM: HandleSDChildWindow, %d state  error!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11067_112_2_18_2_20_58_522, (uint8*) "d", list_param_ptr->item_idx);
            }
        }
        else
        {
            if (MMIFMM_FILE_STATE_CHECKED == view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[list_param_ptr->item_idx]->state)
            {
                view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[list_param_ptr->item_idx]->state = MMIFMM_FILE_STATE_UNCHECKED;

                // 如果是全选状态，需要改为选择状态
                if (view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num == (view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num))
                {
                    view_win_d->main_d_ptr->s_fmm_list_data_ptr->checked = MMIFMM_FILE_STATE_CHECKED;
#ifdef MMI_PDA_SUPPORT
                    GUISOFTKEY_SetTextId (MMIFMM_UDISK_CHILD_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
#endif
                }

                view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num--;

                // 如果标记数为零，关闭标记状态
#ifndef MMI_PDA_SUPPORT

                if (0 == view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num)
                {
                    view_win_d->main_d_ptr->s_fmm_list_data_ptr->checked = MMIFMM_FILE_STATE_UNCHECKED;
                    GUILIST_SetTextListMarkable (list_param_ptr->ctrl_id, FALSE);
#ifdef MMI_PDA_SUPPORT
                    GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif

                }

#endif
            }
            else
            {
                //SCI_TRACE_LOW:"MMIFMM: HandleMarkItem, %d state  error!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11101_112_2_18_2_20_58_523, (uint8*) "d", list_param_ptr->item_idx);
            }
        }
    }
}



/*****************************************************************************/
//  Description : handle cancel option function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleCancelOpt (MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN is_flush = FALSE;
    uint16  file_name_len = 0;
    uint16  slash_index  = 0;
    MMIFMM_VIEW_WIN_DATA_T *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);
#ifdef CMCC_UI_STYLE

    //add for newms162182
    if (MMIFMM_MUSIC_ALL == MMIFMM_GetDisplayFileType() ||
            MMIFMM_MOVIE_ALL == MMIFMM_GetDisplayFileType())
    {
        MMK_CloseParentWin (MMIFMM_MAIN_WIN_ID);
        return;
    }

#endif

    if (PNULL == view_win_d)
    {
        return;
    }

    if (PNULL != view_win_d->main_d_ptr->s_file_path_info_ptr->callback.callback_can)
    {
        view_win_d->main_d_ptr->s_file_path_info_ptr->callback.callback_can (view_win_d->main_d_ptr->s_fmm_list_data_ptr, GUILIST_GetCurItemIndex (ctrl_id));
    }
    else
    {
        GUILIST_SetTextListMarkable (ctrl_id, FALSE);

        // 如果查找文件状态，返回到上一个浏览状态
        if (MMIFMM_FMI_STATE_LOOKUP_FILE == view_win_d->main_d_ptr->s_fmm_list_data_ptr->state)
        {
            MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);

            // 更新数据
            is_flush = TRUE;
            MMK_PostMsg (win_id, MSG_FMM_UPDATE_LIST_DATA, (DPARAM) &is_flush, sizeof (BOOLEAN));
            view_win_d->main_d_ptr->s_fmm_list_data_ptr->state = MMIFMM_FMI_STATE_BROWSER;
            return;
        }

        // 如果当前路径不是根目录
        if (view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth > 1)
        {
            //SCI_TRACE_LOW:"MMIFMM: HandleSDChildWindow, return previous directory!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11557_112_2_18_2_20_59_524, (uint8*) "");
            // 返回到上一级路径
            slash_index = MMIFMM_GetFinallySlashIndex (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len);

            if (slash_index < view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len)
            {
                file_name_len = MMIAPICOM_Wstrlen (&view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname[slash_index +1]);
                SCI_MEMSET (view_win_d->main_d_ptr->s_file_path_info_ptr->file_name, 0x00, sizeof (view_win_d->main_d_ptr->s_file_path_info_ptr->file_name));
                view_win_d->main_d_ptr->s_file_path_info_ptr->file_len = MIN (MMIFMM_PATHNAME_LEN, file_name_len);
                MMIAPICOM_Wstrncpy (view_win_d->main_d_ptr->s_file_path_info_ptr->file_name, &view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname[slash_index +1], view_win_d->main_d_ptr->s_file_path_info_ptr->file_len);
                view_win_d->main_d_ptr->s_file_path_info_ptr->user_filename_valid = TRUE;
            }

            //s_fmm_path_ptr->device[s_fmm_path_ptr->current_device].path[view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth -1].reserved_pos = 0;
            view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth--;
            EnterUpPath (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, &view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len);
            MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);

            // 更新数据
            is_flush = TRUE;
            MMK_PostMsg (win_id, MSG_FMM_UPDATE_LIST_DATA, (DPARAM) &is_flush, sizeof (BOOLEAN));
        }
        // 根目录，关闭窗口
        else
        {
            MMK_CloseParentWin (MMIFMM_MAIN_WIN_ID);
        }
    }

#ifdef MMI_PDA_SUPPORT
    GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
}


/*****************************************************************************/
//  Description : get next index after creating a folder;
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetNextIndex (
    MMIFMM_DATA_INFO_T     *list_data_ptr,
    const wchar                 *folder_name_ptr,
    uint16                 folder_name_len
)
{
    uint16      i = 0;
    int32       result = 0;

    //SCI_TRACE_LOW:"MMIFMM: GetNextIndex."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2908_112_2_18_2_20_40_363, (uint8*) "");

    //SCI_ASSERT(PNULL != list_data_ptr);
    //SCI_ASSERT(PNULL != folder_name_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[GetNextIndex] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2913_112_2_18_2_20_40_364, (uint8*) "");
        return i;
    }

    if (PNULL == folder_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[GetNextIndex] PNULL == folder_name_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2918_112_2_18_2_20_40_365, (uint8*) "");
        return i;
    }

    for (i = 0; i < list_data_ptr->folder_num; i++)
    {
        result = MMIAPICOM_CompareTwoWstr (list_data_ptr->data[i]->filename_ptr,
                                           list_data_ptr->data[i]->filename_len,
                                           folder_name_ptr,
                                           folder_name_len
                                          );

        if (0 == result)
        {
            break;
        }
    }

    if (i >= list_data_ptr->folder_num)
    {
        i = 0;
    }

    return (i);
}


/*****************************************************************************/
//  Description : 更新重命名数据
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateRenameData (
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T        ctrl_id,
    //MMIFMM_DATA_INFO_T   *list_data_ptr ,
    const wchar          *new_filename_ptr,
    MMI_TEXT_ID_T        sfk_left_id
)
{
    uint16                   index = 0;
    uint16                   new_filename_len = 0;
    uint16                   file_len = 0;
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = PNULL;
    //添加PICTHUMBNAIL_SUPPORT控制
    BOOLEAN                  is_two_line = FALSE;
#ifdef  VIDEOTHUMBNAIL_SUPPORT     
    BOOLEAN                  is_videotwo_line = FALSE;
#endif
    view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != view_win_d)
    {
#ifdef MMI_FMM_PICTHUMBNAIL_SUPPORT //添加PICTHUMBNAIL_SUPPORT控制
        is_two_line = MMIFMM_IsDefaultPicFolder(view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len);
#endif
#ifdef  VIDEOTHUMBNAIL_SUPPORT
        is_videotwo_line = MMIFMM_IsDefaultVideoFolder (view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len);
        is_two_line = is_two_line || is_videotwo_line;
#endif

        //SCI_TRACE_LOW:"MMIFMM: UpdateRenameData, ctrl_id = %d, new_filename_len = %d."
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2270_112_2_18_2_20_39_343, (uint8*) "dd",  ctrl_id, new_filename_len);

        if (PNULL == view_win_d->main_d_ptr->s_fmm_list_data_ptr)
        {
            //SCI_TRACE_LOW:"MMIFMM:[UpdateRenameData]:PNULL == view_win_d->main_d_ptr->s_fmm_list_data_ptr"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2275_112_2_18_2_20_39_344, (uint8*) "");
            return;
        }

        if (PNULL == new_filename_ptr)
        {
            //SCI_TRACE_LOW:"MMIFMM:[UpdateRenameData]:PNULL == new_filename_ptr"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2280_112_2_18_2_20_39_345, (uint8*) "");
            return;
        }

        new_filename_len = (uint16) MMIAPICOM_Wstrlen ( (wchar *) new_filename_ptr);

        //SCI_ASSERT( new_filename_len > 0 && new_filename_len <= MMIFMM_FULL_FILENAME_LEN );
        if ( (new_filename_len == 0) || (MMIFMM_FULL_FILENAME_LEN < new_filename_len))
        {
            //SCI_TRACE_LOW:"MMIFMM:[UpdateRenameData]:new_filename_len = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2287_112_2_18_2_20_39_346, (uint8*) "d", new_filename_len);
            return;
        }

        // 修改数据内容
        index = GUILIST_GetCurItemIndex (ctrl_id);
        SCI_MEMSET (view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr,  0, ( (view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len) * sizeof (wchar)));

        if (view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_is_valid)
        {
            file_len = new_filename_len;
        }
        else
        {
            uint16 i = MMIFMM_GetFinallySlashIndex (new_filename_ptr, (new_filename_len - 1));
            file_len = (uint16) (new_filename_len - 1 - i);
        }

        if (file_len != view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len)
        {
            SCI_FREE (view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr);
            view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr = SCI_ALLOCA (sizeof (wchar) * file_len);

            if (PNULL == view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr)
            {
                return;
            }

            SCI_MEMSET (view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr, 0x00, (sizeof (wchar) *file_len));
            view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len = file_len;
        }
        else
        {
            SCI_MEMSET (view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr, 0x00, (sizeof (wchar) *view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len));
        }

        if (view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_is_valid)
        {
            MMI_WSTRNCPY (view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr,
                          view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len,
                          new_filename_ptr, new_filename_len, new_filename_len);
        }
        else
        {
            MMI_WSTRNCPY (view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr,
                          view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len,
                          &new_filename_ptr[new_filename_len-file_len],
                          view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len,
                          view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len);
        }

        AppendTwoLineAnimTextAndTextListItem (win_id, ctrl_id,
                                              sfk_left_id,
                                              view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->type,
                                              view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr,
                                              view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len,
                                              view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->file_size,
                                              view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_is_valid,
                                              view_win_d->main_d_ptr->s_fmm_current_path_ptr,
                                              index,
                                              0,
                                              MMIFMM_REPLACE_LIST_ITEM_TYPE,
                                              is_two_line);
    }
}

/*****************************************************************************/
//  Description : add data to list item
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void AppendTwoLineAnimTextAndTextListItem (
    MMI_WIN_ID_T            win_id,
    MMI_CTRL_ID_T           ctrl_id,
    MMI_TEXT_ID_T           left_softkey_id,
    MMIFMM_FILE_TYPE_E      file_type,
    const wchar*            filename,
    uint16                  filename_len,
    uint32                  filesize,
    BOOLEAN                 path_is_valid,
    MMIFMM_PATH_INFO_T      *current_path_ptr,
    uint16                  item_index,
    uint16                  item_content_index,
    MMIFMM_LIST_ITEM_TYPE_E type,
    BOOLEAN                 is_two_line
)
{
    GUILIST_ITEM_T       item_t    =  {0};
    GUILIST_ITEM_DATA_T  item_data = {0};/*lint !e64*/
    uint16          list_name_len  = 0;
    uint16          full_name_len  = 0;
    //uint32          time           = 0;
    //uint32          size           = 0;
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    GUIANIM_FILE_INFO_T anim_path = {0};
    wchar  list_file_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
#ifdef  VIDEOTHUMBNAIL_SUPPORT
    wchar  thumbnail_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
    uint16  thumbnail_name_len = 0;
#endif
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL == view_win_d)
    {
        SCI_TRACE_LOW ("MMIFMM AppendTwoLineAnimTextAndTextListItem PNULL == view_win_d!");
        return;
    }

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;

    if (MMIFMM_FILE_TYPE_FOLDER == file_type)
    {
        //folder one text
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    }
	else if(is_two_line && (MMIFMM_FILE_TYPE_PICTURE == file_type
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
		|| MMIFMM_FILE_TYPE_MOVIE == file_type
#endif
#ifdef DRM_SUPPORT
		||MMIFMM_FILE_TYPE_DRM == file_type
#endif
		))
	{ //双行情形：pic缩略图，movie缩略图，DRM
		item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT;
	}
    else
    {
        //单行显示
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;
    }

    item_t.first_letter = GetFirstLatterByName (filename);

    if (type == MMIFMM_APPEND_LIST_ITEM_TYPE)
    {
        GUILIST_AppendItem (ctrl_id, &item_t);
    }
    else if (type == MMIFMM_REPLACE_LIST_ITEM_TYPE)
    {
        GUILIST_ReplaceItem (ctrl_id, &item_t, item_index);
    }
    else
    {
        item_t.item_data_ptr = &item_data;
        item_data.softkey_id[0] = left_softkey_id;
        item_data.softkey_id[1] = view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.middlesoft_id;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        if (TXT_NULL == item_data.softkey_id[1])
        {
            item_data.softkey_id[1] = IMAGE_NULL;
        }
#endif
#ifdef JAVA_SUPPORT

        if (MMIFMM_FILE_TYPE_JAVA == file_type)
        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_LOCALINSTALL_ICON;
#else
			item_data.softkey_id[1] = TXT_COM_LOCALINSTALL;
#endif
        }

#endif
        item_data.softkey_id[2] = view_win_d->main_d_ptr->s_file_path_info_ptr->sk_text_id.rightsoft_id;

        SCI_MEMSET (view_win_d->s_list_file_name_ptr, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) * sizeof (wchar)));

        /* full path */
        if (path_is_valid)
        {
            uint16  index = MMIFMM_GetFinallySlashIndex (filename, (filename_len - 1));
            list_name_len = (uint16) (filename_len - 1 - index);
            MMIAPICOM_Wstrncpy (list_file_name,  &filename[index + 1], list_name_len);

            full_name_len = filename_len;
            MMIAPICOM_Wstrncpy (view_win_d->s_list_file_name_ptr, filename, full_name_len);
        }
        else
        {
            list_name_len = filename_len;
            MMIAPICOM_Wstrncpy (list_file_name, filename, list_name_len);
            full_name_len = (uint16) MMIFMM_CombineToFileName (current_path_ptr->pathname, current_path_ptr->pathname_len,
                            filename, filename_len,
                            view_win_d->s_list_file_name_ptr, (uint16) MMIFMM_FULL_FILENAME_LEN);
        }

        /* icon */
        if (is_two_line && MMIFMM_FILE_TYPE_PICTURE == file_type)
        {
                //双行显示，并且是图片文件，则显示缩略图
                item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;

                //only support syn now
                item_data.item_content[0].item_data.anim_path_ptr = &anim_path;
                anim_path.full_path_wstr_ptr = view_win_d->s_list_file_name_ptr;
                anim_path.full_path_wstr_len = full_name_len;
        }

#ifdef  VIDEOTHUMBNAIL_SUPPORT
        else if (is_two_line && MMIFMM_FILE_TYPE_MOVIE == file_type)
        {
            MMIFMM_FmmVideoThumbnailGetPathname (thumbnail_name, &thumbnail_name_len, view_win_d->s_list_file_name_ptr);

            if (!MMIAPIFMM_IsFileExist (thumbnail_name, thumbnail_name_len))
            {
// 				MMIFILE_HANDLE file_handle = MMIAPIFMM_CreateFile(thumbnail_name, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
// 				if(SFS_INVALID_HANDLE != file_handle)
// 				{
// 					MMIAPIFMM_WriteFile(file_handle, kmv_infor_t.item_thumbnail.item_buffer_ptr,
// 						kmv_infor_t.item_thumbnail.item_size, &bytes_write, NULL);
// 					MMIAPIFMM_CloseFile(file_handle);
// 				}

                item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[0].item_data.image_id = MMIAPIFMM_GetIconIdByFileType (file_type, MMI_DEVICE_UDISK);
                //SCI_TRACE_LOW:"[VP]: AppendTwoLineAnimTextAndTextListItem FmmVideoThumbnailAdd  !"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3386_112_2_18_2_20_41_371, (uint8*) "");
                MMIFMM_VideoThumbnailAdd (view_win_d->s_list_file_name_ptr);
            }
            else
            {
                //双行显示，并且是图片文件，则显示缩略图
                item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;

                //only support syn now
                item_data.item_content[0].item_data.anim_path_ptr = &anim_path;
                anim_path.full_path_wstr_ptr = thumbnail_name;
                anim_path.full_path_wstr_len = thumbnail_name_len;
            }
        }

#endif
#ifdef DRM_SUPPORT
        else if (is_two_line && MMIFMM_FILE_TYPE_DRM == file_type)
        {
            MMI_IMAGE_ID_T  icon_id = IMAGE_NULL;
            MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;

            file_handle = MMIAPIFMM_CreateFile (view_win_d->s_list_file_name_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);

            if (SFS_INVALID_HANDLE != file_handle)
            {
                //is drm picture
                if (MMIFMM_FILE_TYPE_PICTURE == MMIAPIDRM_GetMediaFileType (file_handle, PNULL))
                {
                    //is it has right, here should be picture
                    if (MMIAPIDRM_IsRightsValid (file_handle, PNULL, TRUE, DRM_PERMISSION_DISPLAY))
                    {
                        icon_id = IMAGE_DRM_PIC_UNLOCK_ICON;
                    }
                    else
                    {
                        icon_id = IMAGE_DRM_PIC_LOCK_ICON;
                    }
                }

#ifdef  VIDEOTHUMBNAIL_SUPPORT
                else if (MMIFMM_FILE_TYPE_MOVIE == MMIAPIDRM_GetMediaFileType (file_handle, PNULL))
                {
                    //is it has right, here should be movie
                    if (MMIAPIDRM_IsRightsValid (file_handle, PNULL, TRUE, DRM_PERMISSION_PLAY))
                    {
                        icon_id = IMAGE_DRM_MOVIE_UNLOCK_ICON;
                    }
                    else
                    {
                        icon_id = IMAGE_DRM_MOVIE_LOCK_ICON;
                    }
                }

#endif
                else
                {
                    icon_id = IMAGE_DRM_PIC_UNKNOW_ICON;
                }
            }
            else
            {
                icon_id = IMAGE_DRM_PIC_UNKNOW_ICON;
            }

            MMIAPIFMM_CloseFile (file_handle);

            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = icon_id;
        }

#endif
//        else if(is_two_line && MMIFMM_FILE_TYPE_FOLDER == file_type)
//        {
//            item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
//            item_data.item_content[0].item_data.image_id = IMAGE_FMM_FOLDER_2L_ICON;
//        }
        else
        {
            item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
#ifdef DRM_SUPPORT

            if (MMIFMM_FILE_TYPE_DRM == file_type)
            {
                item_data.item_content[0].item_data.image_id = MMIFMM_GetIconIdByFileName (view_win_d->s_list_file_name_ptr, full_name_len);
            }
            else
            {
                item_data.item_content[0].item_data.image_id = MMIAPIFMM_GetIconIdByFileType (file_type, MMI_DEVICE_UDISK);
            }

#else
            item_data.item_content[0].item_data.image_id = MMIAPIFMM_GetIconIdByFileType (file_type, MMI_DEVICE_UDISK);
#endif


        }

        /* text 1 */
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

        item_data.item_content[1].item_data.text_buffer.wstr_len = list_name_len;

        item_data.item_content[1].item_data.text_buffer.wstr_ptr = list_file_name;

        if (MMIFMM_FILE_TYPE_FOLDER != file_type)
        {
            /* text 2 */
            //MMIFMM_GetFileInfo(view_win_d->s_list_file_name_ptr, full_name_len, &time, &size );

            item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

            //item_data.item_content[2].item_data.text_buffer.is_ucs2 = FALSE;
            MMIAPIFMM_GetFileSizeString (filesize,
                                         temp_wstr,
                                         MMIFMM_SIZE_STR_LEN,
                                         TRUE);

            item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
            item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (temp_wstr);
        }

        if (type == MMIFMM_NEED_LIST_ITEM_DATA_TYPE)
        {
            GUILIST_SetItemData (ctrl_id, &item_data, item_index);
        }
        else if (type == MMIFMM_NEED_LIST_ITEM_CONTENT_TYPE)
        {
            GUILIST_SetItemContent (ctrl_id, &item_data.item_content[item_content_index], item_index, item_content_index);
        }
    }
}

/*****************************************************************************/
//  Description : get first latter by name
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint16 GetFirstLatterByName (const wchar *filename)
{
    wchar upper_buf [2] = {0};
    upper_buf[0] = filename[0];
    upper_buf[1] = 0;
    MMIAPICOM_Wstrupper (&upper_buf[0]);
    return upper_buf[0];
}



/*****************************************************************************/
//  Description : get up path info
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN EnterUpPath (wchar * path_ptr, uint16 *path_len)
{
    BOOLEAN  result = FALSE;
    uint16 *path = PNULL;
    int32   i = 0;

    if (PNULL == path_ptr || 0 == *path_len)
    {
        return result;
    }

    path = path_ptr;

    for (i = *path_len; i > 0 ; i--)
    {
        if (path[i] == MMIFILE_SLASH)
        {
            path[i] = 0x00;
            break;
        }

        path[i] = 0x00;
    }

    if (i != 0)
    {
        *path_len = (uint16) i;
        result = TRUE;
    }

    return result;
}





/*****************************************************************************/
// Description : update valid path
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
LOCAL BOOLEAN FmmUpdateValidPath (MMI_WIN_ID_T win_id , BOOLEAN is_enter_root)
{
    BOOLEAN result = FALSE;
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL == view_win_d)
    {
        SCI_TRACE_LOW ("MMIFMM FmmUpdateValidPath PNULL == view_win_d!");
        return FALSE;
    }

    if (1 < view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth)
    {
        if (is_enter_root) //需要退到root 目录
        {
            while (view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth != 1)
            {
                EnterUpPath (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, &view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len);
                view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth--;
            }

            result = TRUE;
        }
        else
        {   //查找有效目录
            while (view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth != 1)
            {
                if (!MMIAPIFMM_IsFolderExist (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len))
                {
                    //不是有效路径，返回上层目录继续查找确认
                    EnterUpPath (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, &view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len);
                    view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth--;
                }
                else //找到有效目录退出
                {
                    break;
                }
            }

            result = TRUE;
        }
    }
    else //已经是根目录
    {
        result = TRUE;
    }

    return result;
}



/*****************************************************************************/
//  Description : handle mark Operate
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandlePDAMarkOperate (
    MMI_WIN_ID_T        win_id
)
{
    uint16          mark_num = 0;
    //MMI_WIN_ID_T    win_id = MMIFMM_UDISK_CHILD_WIN_ID;
    //MMI_CTRL_ID_T   list_ctrl_id = MMIFMM_UDISK_LISTBOX_CTRL_ID;
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL == view_win_d)
    {
        SCI_TRACE_LOW ("MMIFMM HandlePDAMarkOperate PNULL == view_win_d!");
        return;
    }

    if (view_win_d->com_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
    {
        if (MMIFMM_FILE_STATE_ALL_CHECKED == view_win_d->main_d_ptr->s_fmm_list_data_ptr->checked) //如果已经全部选中
        {
            HandleMenuCancelAllMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->list_ctrl_id);
        }
        else//全部标记
        {
            HandleMenuMarkAll (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->list_ctrl_id);
        }

        MMK_PostMsg (win_id, MSG_FULL_PAINT, PNULL, 0);
    }
    else if (view_win_d->com_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
    {
        GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        HandleMenuCancelAllMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->list_ctrl_id);
        MMK_PostMsg (win_id, MSG_FULL_PAINT, PNULL, 0);
    }
    else if (view_win_d->com_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
    {
        mark_num = GUILIST_GetSelectedItemIndex (view_win_d->list_ctrl_id, PNULL, 0);

        if (0 == mark_num)
        {
            MMIPUB_OpenAlertWarningWin (TXT_PLS_SELECT_ONE_ITEM);
        }
        else
        {
            MMIFMM_DeleteSelectFile (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->list_ctrl_id, view_win_d->main_d_ptr->s_fmm_current_path_ptr);
        }
    }
    else
    {
        HandleListItem (win_id);
        //HandleListItem(view_win_d->main_d_ptr->s_fmm_list_data_ptr,view_win_d->main_d_ptr->s_fmm_current_path_ptr,view_win_d->list_ctrl_id);
    }
}


/*****************************************************************************/
//  Description : handle menu cancel all mark
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuCancelAllMark (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMI_CTRL_ID_T         ctrl_id
)
{
    uint16   i  = 0;
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_DISPLAY_E display_type = GUIFORM_CHILD_DISP_NORMAL;
#endif

    //SCI_TRACE_LOW:"MMIFMM: HandleMenuCancelAllMark."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3683_112_2_18_2_20_42_378, (uint8*) "");

    list_data_ptr->mark_num = 0;

    for (i = 0; i < (list_data_ptr->file_num + list_data_ptr->folder_num); i++)
    {
        list_data_ptr->data[i]->state = MMIFMM_FILE_STATE_UNCHECKED;
        GUILIST_SetSelectedItem (ctrl_id, i, FALSE);
    }

    list_data_ptr->checked = MMIFMM_FILE_STATE_UNCHECKED;

    // 取消listbox 为标记
#ifdef MMI_PDA_SUPPORT
    GUISOFTKEY_SetTextId (MMIFMM_UDISK_CHILD_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE);
    GUIFORM_GetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
    GUIWIN_SetTitleButtonState (MMIFMM_UDISK_CHILD_WIN_ID, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, TRUE);

    if (GUIFORM_CHILD_DISP_HIDE == display_type)
#endif
    {
        GUILIST_SetTextListMarkable (ctrl_id, FALSE);
    }
}



/*****************************************************************************/
//  Description : 处理list item
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleListItem (
    MMI_WIN_ID_T win_id
    //MMIFMM_DATA_INFO_T         *list_data_ptr,
    //MMIFMM_PATH_INFO_T         *current_path_ptr,
    //uint32                     ctrl_id
)
{
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
    IGUICTRL_T              *ctrl_ptr = PNULL;
    //MMI_WIN_ID_T            win_id = 0;
    MMI_TEXT_ID_T           alert_text_id = TXT_NULL;
    uint16                  index = 0;
#ifdef JAVA_SUPPORT
#ifdef JAVA_SUPPORT_SUN
    wchar                   *file_name_ptr = PNULL;
#endif
#endif
    uint16                  file_name_len = 0;
    BOOLEAN                 is_flush = FALSE;
	
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    //SCI_ASSERT(PNULL != list_data_ptr);
    //SCI_ASSERT(PNULL != current_path_ptr);
    if (PNULL == view_win_d || PNULL == view_win_d->main_d_ptr->s_fmm_list_data_ptr)
    {
        SCI_TRACE_LOW ("MMIFMM HandleListItem PNULL == view_win_d!");
        return;
    }

    if (PNULL == view_win_d->main_d_ptr->s_fmm_current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[HandleListItem]:NULL == view_win_d->main_d_ptr->s_fmm_current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2563_112_2_18_2_20_40_353, (uint8*) "");
        return;
    }

    //SCI_ASSERT(PNULL != path_ptr);

    if (PNULL != view_win_d->main_d_ptr->s_file_path_info_ptr->callback.callback_web)
    {
        view_win_d->main_d_ptr->s_file_path_info_ptr->callback.callback_web (view_win_d->main_d_ptr->s_fmm_list_data_ptr, GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id));
    }
    else
    {
        if (0 == (view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num))
        {
            // empty folder, return
            return;
        }

        index = GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id);
        //SCI_TRACE_LOW:"MMIFMM: HandleListItem , ctrl_id = %d, index = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2581_112_2_18_2_20_40_354, (uint8*) "dd", view_win_d->list_ctrl_id, index);

        ctrl_ptr = MMK_GetCtrlPtr (view_win_d->list_ctrl_id);

        //SCI_ASSERT(PNULL != ctrl_ptr);
        if (PNULL == ctrl_ptr)
        {
            //SCI_TRACE_LOW:"MMIFMM:[HandleListItem]:NULL == ctrl_ptr"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2587_112_2_18_2_20_40_355, (uint8*) "");
            return;
        }

        //win_id = MMK_GetWinHandleByCtrl( view_win_d->list_ctrl_id );

        // 如果是文件夹，进入文件夹
        if (MMIFMM_FILE_TYPE_FOLDER == view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->type)
        {   // 保存当前的路径
			
            if (view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_is_valid)
            {
                file_name_len = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len;
            }
            else
            {
                file_name_len = (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len + view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len + 1);
            }

            if (MMIFMM_PATH_DEPTH == view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth || file_name_len >= MMIFMM_PATHNAME_LEN)
            {
                //路径已到最到深度
                //SCI_TRACE_LOW:"MMIFMM: HandleListItem, Max depth!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2606_112_2_18_2_20_40_356, (uint8*) "");

                if (MMIFMM_PATH_DEPTH == view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth)
                {
                    alert_text_id = TXT_MAX_DEPTH;
                }
                else
                {
                    alert_text_id = TXT_COM_FILE_NAME_TOO_LONG;
                }

                MMIPUB_OpenAlertWarningWin (alert_text_id);
            }
            else
            {
		     	MMIFILE_DEVICE_E           base_dev  = MMI_DEVICE_UDISK;//CR149894
                //SCI_TRACE_LOW:"MMIFMM: HandleListItem, enter subdir!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2619_112_2_18_2_20_40_357, (uint8*) "");
                //SaveCurrentPath(path_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, index);

                // 设置当前路径
                if (view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_is_valid)
                {
                    MMIAPICOM_Wstrncpy (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr, view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len);
                    view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len;
                    view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth =  MMIFMM_GetFullPathDepth (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len);
                    view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth++;
                }
                else
                {
                    view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth++;

                    //SCI_ASSERT((view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len + view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len +1)<= MMIFMM_PATHNAME_LEN );
                    if (MMIFMM_PATHNAME_LEN < (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len + view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len + 1))
                    {
                        //SCI_TRACE_LOW:"MMIFMM:[HandleListItem]: total_pathname_len = %d"
                        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2636_112_2_18_2_20_40_358, (uint8*) "d", (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len + view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len + 1));
                        return;
                    }

                    view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname[view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len] = MMIFILE_SLASH; // 增加" \"
                    MMIAPICOM_Wstrncpy (&view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname[view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len+1], view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr, view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len);
                    view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len = (uint16) (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len + view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len + 1);
                    view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname[view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len] = 0;

                    MMIAPICOM_Wstrncpy (view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len);
                    view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname_len = view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len;
                    view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_depth = view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth;
                }

                //path_ptr->device[path_ptr->current_device].path[view_win_d->main_d_ptr->s_fmm_current_path_ptr->path_depth -1].reserved_pos = 0;

                MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
#ifdef  VIDEOTHUMBNAIL_SUPPORT
                FmmVideoThumbnail_Init (view_win_d->main_d_ptr->s_fmm_current_path_ptr);
#endif
				view_win_d->s_fmm_path_ptr[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - base_dev].reserved_pos = 0;//CR149894
                // 更新数据
                is_flush = TRUE;
                MMK_PostMsg (win_id, MSG_FMM_UPDATE_LIST_DATA, (DPARAM) &is_flush, sizeof (BOOLEAN));
            }
        }
        //  如果是文件，根据文件的后缀，查找播放器程序
        else
        {
            //SCI_TRACE_LOW:"MMIFMM: HandleListItem, call relative application!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2662_112_2_18_2_20_40_359, (uint8*) "");
            file_type =  view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->type;
            if (0 == view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->file_size)
            {
                MMIPUB_OpenAlertWarningWin (TXT_EMPTY_FILE);
                return;
            }
#ifdef DRM_SUPPORT
            //  如果是DRM文件，进一步分析是何种媒体文件
            {
                DRM_PERMISSION_MODE_E drm_permission = DRM_PERMISSION_NONE;
                DRMFILE_PRE_CHECK_STATUS_E pre_check_drmfile_status = DRMFILE_PRE_CHECK_NORMAL;

                if (MMIFMM_FILE_TYPE_DRM == file_type)
                {
                    MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, index, view_win_d->s_list_file_name_ptr, MMIFMM_FULL_FILENAME_LEN);
                    file_type = MMIAPIDRM_GetMediaFileType (SFS_INVALID_HANDLE, view_win_d->s_list_file_name_ptr);

                    switch (file_type)
                    {
                    case MMIFMM_FILE_TYPE_PICTURE:
                    case  MMIFMM_FILE_TYPE_EBOOK:
                        drm_permission = DRM_PERMISSION_DISPLAY;
                        break;

                    case MMIFMM_FILE_TYPE_MUSIC:
                    case MMIFMM_FILE_TYPE_MOVIE:
                        drm_permission = DRM_PERMISSION_PLAY;
                        break;

                    case MMIFMM_FILE_TYPE_JAVA:
                        drm_permission = DRM_PERMISSION_EXECUTE;
                        break;

                    default:
                        break;
                    }

                    if (DRM_PERMISSION_NONE == drm_permission)
                    {
                        MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
                        return;
                    }

                    pre_check_drmfile_status = MMIAPIDRM_PreCheckFileStatus (view_win_d->s_list_file_name_ptr, drm_permission);

                    if (DRMFILE_PRE_CHECK_NORMAL != pre_check_drmfile_status)
                    {
                        if (DRMFILE_PRE_CHECK_NO_RIGHTS == pre_check_drmfile_status)
                        {
                            //如果是无效的，则需要提示guilist去刷新本行
                            //MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
                            MMIAPIFMM_UpdateListIconData (view_win_d->list_ctrl_id, index, view_win_d->main_d_ptr->s_fmm_list_data_ptr->pathname, view_win_d->s_list_file_name_ptr);
                        }

                        return;
                    }
                }
            }
#endif

            switch (file_type)
            {
            case MMIFMM_FILE_TYPE_PICTURE:
                // 生成图像文件数据
                //分配的内存在preview窗口中释放
                view_win_d->s_pic_index_data_ptr = SCI_ALLOC_APPZ (sizeof (MMIFMM_PICTURE_INDEX_DATA_T));

                if (PNULL == view_win_d->s_pic_index_data_ptr)
                {
                    SCI_TRACE_LOW ("MMIFMM:HandleListItem PNULL == view_win_d->s_pic_index_data_ptr");
                    return;
                }

                SetupPicIndexData (win_id, view_win_d->list_ctrl_id, view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->s_pic_index_data_ptr);
                // 打开图像浏览窗口
                MMIFMM_PicPreview (view_win_d->list_ctrl_id, view_win_d->s_pic_index_data_ptr);
                break;

            case MMIFMM_FILE_TYPE_MUSIC:
#ifdef MMI_AUDIO_PLAYER_SUPPORT

                if (MMIAPISET_IsOpenPrivacyProtect (MMISET_PROTECT_AUDIO_PLAYER))
                {
                    MMIAPISET_ValidatePrivacyAppEntry (MMISET_PROTECT_AUDIO_PLAYER, ValidatePrivacyPlayMusicFile);
                }
                else
#endif
                {
                    // 播放音乐
                    MMIFMM_FmmPlayMusicFile (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id), GUILIST_GetTotalItemNum (view_win_d->list_ctrl_id));
                }

                break;
#ifdef VIDEO_PLAYER_SUPPORT
            case MMIFMM_FILE_TYPE_MOVIE:
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION

                if (MMIAPISET_IsOpenPrivacyProtect (MMISET_PROTECT_VIDEO_PLAYER))
                {
                    MMIAPISET_ValidatePrivacyAppEntry (MMISET_PROTECT_VIDEO_PLAYER, ValidatePrivacyPlayMovieFile);
                }
                else
#endif
                {
                    // 播放影像
                    MMIFMM_FmmPlayMovieFile (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id));
                }

#endif
                break;

#ifdef EBOOK_SUPPORT
            case MMIFMM_FILE_TYPE_EBOOK:
                // 显示电子书的内容
                index = GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id);
                SCI_MEMSET (view_win_d->s_list_file_name_ptr, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) * sizeof (wchar)));
                MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, PNULL, index, view_win_d->s_list_file_name_ptr, MMIFMM_FULL_FILENAME_LEN);

                if (MMIAPISET_IsOpenPrivacyProtect (MMISET_PROTECT_EBOOK) && !view_win_d->main_d_ptr->s_file_path_info_ptr->is_protect_checked)
                {
                    MMIAPISET_ValidatePrivacy (MMISET_PROTECT_EBOOK, ValidatePrivacyShowTxtContent, (DPARAM) view_win_d->s_list_file_name_ptr, sizeof(wchar)*( MMIFMM_FULL_FILENAME_LEN + 1 ));//coverity 12987
                }
                else
                {
                    //MMIFMM_CombineFullFileName(view_win_d->main_d_ptr->s_fmm_list_data_ptr,view_win_d->main_d_ptr->s_fmm_current_path_ptr,index,view_win_d->s_list_file_name_ptr,MMIFMM_FULL_FILENAME_LEN);
                    MMIFMM_ShowTxtContent (view_win_d->s_list_file_name_ptr);
                }

                break;
#endif
#if defined MMI_VCARD_SUPPORT
            case MMIFMM_FILE_TYPE_VCARD:
                // 打开VCARD文件
                // MMIFMM_FmmOpenVcardFile(view_win_d->main_d_ptr->s_fmm_list_data_ptr,view_win_d->main_d_ptr->s_fmm_current_path_ptr,GUILIST_GetCurItemIndex(view_win_d->list_ctrl_id));
                index = GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id);
                SCI_MEMSET (view_win_d->s_list_file_name_ptr, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) * sizeof (wchar)));

                MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, PNULL, index, view_win_d->s_list_file_name_ptr, MMIFMM_FULL_FILENAME_LEN);
                MMIFMM_ShowTxtContent (view_win_d->s_list_file_name_ptr);
                break;
#endif
#ifdef MMI_VCALENDAR_SUPPORT
            case MMIFMM_FILE_TYPE_VCALENDAR:
                // 打开VCARD文件
                MMIFMM_FmmOpenVcalendarFile (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id));
                break;
#endif
#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
            case MMIFMM_FILE_TYPE_FONT:
                MMIFMM_FmmOpenFont (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id));
                break;
#endif

#ifdef DYNAMIC_MODULE_SUPPORT
            case MMIFMM_FILE_TYPE_SZIP:
                MMIFMM_FmmOpenSzipFile (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id));
                break;
#endif
#ifdef JAVA_SUPPORT
            case MMIFMM_FILE_TYPE_JAVA:
#ifdef JAVA_SUPPORT_SUN //需要统一接口

                if (view_win_d->main_d_ptr->s_fmm_list_data_ptr->path_is_valid && (view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len > view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len + 1))
                {
                    file_name_ptr = & (view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_ptr[view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len + 1]);
                    file_name_len = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len - view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len;
                }
                else
                {
                    file_name_ptr = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]filename_ptr;
                    file_name_len = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->filename_len;
                }

                if (0 == view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->file_size)
                {
                    MMIPUB_OpenAlertWarningWin (TXT_EMPTY_FILE);

                }
                else
                {
                    MMIAPIJAVA_InstallFromFilesystem (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname_len,
                                                      file_name_ptr, file_name_len);
                }

#elif defined(JAVA_SUPPORT_IA)
                {
                    MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, index, view_win_d->s_list_file_name_ptr, MMIFMM_FULL_FILENAME_LEN);
                    MMIAPIJAVA_InstallFromFilesystem (view_win_d->s_list_file_name_ptr, MMIAPICOM_Wstrlen (view_win_d->s_list_file_name_ptr));
                }
#elif JAVA_SUPPORT_MYRIAD
                {
                    MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, index, view_win_d->s_list_file_name_ptr, MMIFMM_FULL_FILENAME_LEN);
                    MMIAPIJAVA_Install (view_win_d->s_list_file_name_ptr, MMIAPICOM_Wstrlen (view_win_d->s_list_file_name_ptr));
                }

#endif
                break;
#endif
#ifdef QBTHEME_SUPPORT
            case MMIFMM_FILE_TYPE_THEME:
                index = GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id);
                SCI_MEMSET (view_win_d->s_list_file_name_ptr, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) * sizeof (wchar)));

                MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, PNULL, index, view_win_d->s_list_file_name_ptr, MMIFMM_FULL_FILENAME_LEN);
                MMIAPIQBTHEME_Install (view_win_d->s_list_file_name_ptr);
                break;
#endif
            case MMIFMM_FILE_TYPE_BROWSER:
#ifdef BROWSER_SUPPORT_NETFRONT
                MMIFMM_FmmOpenWWWFile (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id));
                break;
#endif

#ifdef BROWSER_SUPPORT
            case  MMIFMM_FILE_TYPE_URL:
                MMIFMM_FmmOpenUrlFile(view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id));
                break;
#endif

            default:
                // 提示格式不支持
                MMIPUB_OpenAlertWarningWin (TXT_COMMON_NO_SUPPORT);
                break;
            }
        }
    }
}

#ifdef MMI_AUDIO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void ValidatePrivacyPlayMusicFile (void)
{
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);

    if (PNULL != view_win_d)
    {
        MMIFMM_FmmPlayMusicFile (
            view_win_d->main_d_ptr->s_fmm_list_data_ptr,
            view_win_d->main_d_ptr->s_fmm_current_path_ptr,
            GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id),
            GUILIST_GetTotalItemNum (view_win_d->list_ctrl_id));
    }
}
#endif

#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void ValidatePrivacyPlayMovieFile (void)
{
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);

    if (PNULL != view_win_d)
    {

        MMIFMM_FmmPlayMovieFile (
            view_win_d->main_d_ptr->s_fmm_list_data_ptr,
            view_win_d->main_d_ptr->s_fmm_current_path_ptr,
            GUILIST_GetCurItemIndex (view_win_d->list_ctrl_id));
    }
}
#endif

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void ValidatePrivacyShowTxtContent (BOOLEAN is_permitted, DPARAM param_ptr)
{
    wchar *full_path_name_ptr = PNULL;

    if (is_permitted)
    {
        if (PNULL != param_ptr)
        {
            full_path_name_ptr = param_ptr;
        }

        MMIFMM_ShowTxtContent (full_path_name_ptr);
    }
}



/*****************************************************************************/
//  Description : 添加列表list的item
//  Global resource dependence :
//  Author: robert.lu
//  Note:  保存文件的类型
/*****************************************************************************/
LOCAL void FmmAppendListItem (//当前索引
    MMI_WIN_ID_T win_id ,
    MMI_CTRL_ID_T        ctrl_id,
    MMI_TEXT_ID_T        left_softkey_id,
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMIFMM_PATH_INFO_T   *current_path_ptr
)
{
    uint16 i = 0;
    uint32 max_num = 0;
	//添加PICTHUMBNAIL_SUPPORT控制
    BOOLEAN is_two_line = FALSE;
#ifdef  VIDEOTHUMBNAIL_SUPPORT     
    BOOLEAN                  is_videotwo_line = FALSE;
#endif
	MMI_HANDLE_T list_handle = MMK_ConvertIdToHandle (ctrl_id);

#ifdef MMI_FMM_PICTHUMBNAIL_SUPPORT 
	is_two_line = MMIFMM_IsDefaultPicFolder(list_data_ptr->pathname,list_data_ptr->pathname_len);
#endif
#ifdef  VIDEOTHUMBNAIL_SUPPORT
    is_videotwo_line  = MMIFMM_IsDefaultVideoFolder (list_data_ptr->pathname, list_data_ptr->pathname_len);
    is_two_line = is_two_line || is_videotwo_line;
#endif

    max_num = MIN (MMIFMM_FILE_NUM, list_data_ptr->folder_num + list_data_ptr->file_num);

    for (i = 0; i < max_num; i++)
    {
        AppendTwoLineAnimTextAndTextListItem (win_id, list_handle,
                                              left_softkey_id,
                                              list_data_ptr->data[i]->type,
                                              list_data_ptr->data[i]->filename_ptr,
                                              list_data_ptr->data[i]->filename_len,
                                              list_data_ptr->data[i]->file_size,
                                              list_data_ptr->path_is_valid,
                                              current_path_ptr,
                                              i,
                                              0,
                                              MMIFMM_APPEND_LIST_ITEM_TYPE,
                                              is_two_line
                                             );
    }
}



/*****************************************************************************/
//  Description : handle menu mark all
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuMarkAll (
    MMIFMM_DATA_INFO_T  *list_data_ptr,
    MMI_CTRL_ID_T       ctrl_id
)
{
    uint16   i  = 0;

    //SCI_TRACE_LOW:"MMIFMM: HandleMenuMarkAll."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3640_112_2_18_2_20_42_376, (uint8*) "");

    //SCI_ASSERT(PNULL != list_data_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[HandleMenuMarkAll]PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3644_112_2_18_2_20_42_377, (uint8*) "");
        return;
    }

    if (0 == list_data_ptr->mark_num)
    {
        GUILIST_SetTextListMarkable (ctrl_id, TRUE);
        GUILIST_SetMaxSelectedItem (ctrl_id, (uint16) (list_data_ptr->file_num + list_data_ptr->folder_num));
    }

    list_data_ptr->mark_num = (uint16) (list_data_ptr->file_num + list_data_ptr->folder_num);

    for (i = 0; i < (list_data_ptr->file_num + list_data_ptr->folder_num); i++)
    {
        list_data_ptr->data[i]->state = MMIFMM_FILE_STATE_CHECKED;

        // 设置listbox 为标记
        GUILIST_SetSelectedItem (ctrl_id, i, TRUE);
    }

    //CR161515
    // 判断是外部模块通过filter访问fmm还是从主入口main访问（）区别在于前者mark all是部分文件而不是全部文件
    if ((MMIFMM_FILE_ALL != MMIFMM_GetDisplayFileType())||(MMIFMM_FMI_STATE_LOOKUP_FILE == list_data_ptr->state))
    {
        list_data_ptr->checked = MMIFMM_FILE_STATE_CHECKED;
    }
    else
    {
        list_data_ptr->checked = MMIFMM_FILE_STATE_ALL_CHECKED;
    }
#ifdef MMI_PDA_SUPPORT
    GUISOFTKEY_SetTextId (MMIFMM_UDISK_CHILD_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK, TXT_DELETE, STXT_CANCEL, FALSE);
#endif
}



/*****************************************************************************/
//  Description : setup the data of the picture index
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SetupPicIndexData (
    MMI_WIN_ID_T         win_id,
    MMI_CTRL_ID_T                ctrl_id,
    MMIFMM_DATA_INFO_T           *list_data_ptr,     // [IN]
    MMIFMM_PICTURE_INDEX_DATA_T  *pic_index_data_ptr // [OUT]
)
{
    int16      i = 0;
    uint16      pic_num = 0;
    MMIFMM_FILE_TYPE_E file_type = MMIFMM_FILE_TYPE_NORMAL;
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);
    //SCI_TRACE_LOW:"MMIFMM: SetupPicIndexData."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2956_112_2_18_2_20_40_366, (uint8*) "");

    if (PNULL == view_win_d)
    {
        SCI_TRACE_LOW ("MMIFMM SetupPicIndexData PNULL == view_win_d!");
        return pic_num;
    }

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[SetupPicIndexData] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2961_112_2_18_2_20_40_367, (uint8*) "");
        return pic_num;
    }

    if (PNULL == pic_index_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[SetupPicIndexData] PNULL == pic_index_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_2966_112_2_18_2_20_41_368, (uint8*) "");
        return pic_num;
    }

    pic_num = 0;
    SCI_MEMSET (pic_index_data_ptr , 0, sizeof (MMIFMM_PICTURE_INDEX_DATA_T));

    for (i = 0; i < (list_data_ptr->folder_num + list_data_ptr->file_num); i++)
    {
        file_type = list_data_ptr->data[i]->type;

#ifdef DRM_SUPPORT

        if (MMIFMM_FILE_TYPE_DRM == file_type)
        {
            //需要把DRM有效的图片文件选进所谓的预览图片数组区
            MMIFILE_HANDLE file_handle = 0;

            MMIFMM_CombineFullFileName (list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, i, view_win_d->s_list_file_name_ptr, MMIFMM_FULL_FILENAME_LEN);

            file_handle = SFS_CreateFile (view_win_d->s_list_file_name_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);

            if (SFS_INVALID_HANDLE != file_handle)
            {
                if (MMIAPIDRM_IsRightsValid (file_handle, PNULL, TRUE, DRM_PERMISSION_DISPLAY))
                {
                    //如果是图片类型，则下面自动会把此DRM文件加进数组
                    file_type = MMIAPIDRM_GetMediaFileType (file_handle, PNULL);
                }
            }

            SFS_CloseFile (file_handle);
        }

#endif

        // 查找图像文件
        if (MMIFMM_FILE_TYPE_PICTURE == file_type)
        {
            pic_index_data_ptr->list_index[pic_num] = i;

            // 保留当前位置
            if (GUILIST_GetCurItemIndex (ctrl_id) == i)
            {
                pic_index_data_ptr->current_pos  = pic_num;
            }

            pic_num++;
        }
    }

    pic_index_data_ptr->total_num = pic_num;

    return (pic_num);
}


/*****************************************************************************/
//  Description : handle mark item function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleCreatePopMenuOpt (
    MMI_WIN_ID_T         win_id,
    uint32 index
)
{
    //MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_DISPLAY_E  display_type = GUIFORM_CHILD_DISP_NORMAL;
#endif
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL == view_win_d)
    {
        SCI_TRACE_LOW ("MMIFMM HandleCreatePopMenuOpt PNULL == view_win_d!");
        return;
    }

    if (PNULL != view_win_d->main_d_ptr->s_file_path_info_ptr->callback.callback_ok)
    {
        view_win_d->main_d_ptr->s_file_path_info_ptr->callback.callback_ok (view_win_d->main_d_ptr->s_fmm_list_data_ptr, index);
    }
    else
    {
        if (0 == (view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num))
        {
            //MMK_CreateWin((uint32 *)MMIFMM_EMPTY_FOLDER_MENU_WIN_TAB,PNULL);
            MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_EMPTY_FOLDER_OPT, MMIFMM_EMPTY_FOLDER_MENU_WIN_ID, MMIFMM_EMPTY_FOLDER_MENU_CTRL_ID);
            return;
        }
        else
        {
            view_win_d->cur_file_type = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->type;

#ifdef DRM_SUPPORT

            if (MMIFMM_FILE_TYPE_DRM == view_win_d->cur_file_type)
            {
                //需要根据DRM文件的媒体类型来选择定制的菜单选项
                MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
                DRM_PERMISSION_MODE_E drm_permission = DRM_PERMISSION_NONE;

                MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, index, view_win_d->s_list_file_name_ptr, MMIFMM_FULL_FILENAME_LEN);

                file_handle = SFS_CreateFile (view_win_d->s_list_file_name_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);

                //如果此DRM文件有效且符合下列枚举类型，则依据原始类型显示菜单项
                if (SFS_INVALID_HANDLE != file_handle)
                {
                    drm_permission = MMIAPIDRM_GetFilePermission (file_handle, PNULL);

                    if (MMIAPIDRM_IsRightsValid (file_handle, PNULL, TRUE, drm_permission))
                    {
                        view_win_d->cur_file_type = MMIAPIDRM_GetMediaFileType (file_handle, PNULL);
                    }
                }

                SFS_CloseFile (file_handle);
            }

#endif

#ifdef MMI_PDA_SUPPORT
            GUIFORM_GetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);

            if (GUIFORM_CHILD_DISP_NORMAL == display_type)
            {
                //MMK_CreateWin((uint32 *)MMIFMM_MARK_OPT_MENU_WIN_TAB,PNULL);
                MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_MARK_LIST_OPT, MMIFMM_MRAK_OPT_MENU_WIN_ID, MMIFMM_MARK_OPT_MENU_CTRL_ID);
            }
            else
#endif
            {
                switch (view_win_d->cur_file_type)
                {
                case MMIFMM_FILE_TYPE_PICTURE:
                    MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_PICTURE_LIST_OPT, MMIFMM_PIC_OPT_MENU_WIN_ID, MMIFMM_PIC_OPT_MENU_CTRL_ID);
                    break;

                case MMIFMM_FILE_TYPE_MUSIC:	
                    MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_MUSIC_LIST_OPT, MMIFMM_MUSIC_OPT_MENU_WIN_ID, MMIFMM_MUSIC_OPT_MENU_CTRL_ID);
					{//非pda上增加alarm设置功能
#ifndef MMI_PDA_SUPPORT
#ifdef MMI_AUDIO_PLAYER_SUPPORT
						MMI_STRING_T                stk_str = {0};
						wchar	str_content[MMIACC_EVENT_TEXT_MAX_LEN+1] = {0};	
						uint16 i = 0;
						
						for(i=0;i<ALM_ALARM_NUM;i++)
						{
							stk_str.wstr_ptr = str_content;
							stk_str.wstr_len= MMIACC_EVENT_TEXT_MAX_LEN;
							MMIAPIALM_GetAlarmName(i,&stk_str);
							GUIMENU_SetStaticItem(MMIFMM_MUSIC_OPT_MENU_CTRL_ID,MENU_FMM_ALARM_SELECT_OPT,ID_FMM_ALARM1_RING+i,&stk_str,0);
						}
#endif
#endif
					}
					break;

                case MMIFMM_FILE_TYPE_FOLDER:
                    MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_NORMAL_LIST_OPT, MMIFMM_NORMAL_OPT_MENU_WIN_ID, MMIFMM_NORMAL_OPT_MENU_CTRL_ID);
                    break;

                case MMIFMM_FILE_TYPE_MOVIE:
                    MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_MOVIE_LIST_OPT, MMIFMM_MOVIE_OPT_MENU_WIN_ID, MMIFMM_MOVIE_OPT_MENU_CTRL_ID);
                    break;
#if defined MMI_VCARD_SUPPORT
                case MMIFMM_FILE_TYPE_VCARD:
                    MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_VCARD_LIST_OPT, MMIFMM_VCARD_OPT_MENU_WIN_ID, MMIFMM_VCARD_OPT_MENU_CTRL_ID);
                    break;
#endif
#ifdef MMI_VCALENDAR_SUPPORT
                case MMIFMM_FILE_TYPE_VCALENDAR:
                    MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_VCALENDAR_LIST_OPT, MMIFMM_VCALENDAR_OPT_MENU_WIN_ID, MMIFMM_VCALENDAR_OPT_MENU_CTRL_ID);
                    break;
#endif
#ifdef EBOOK_SUPPORT
                case MMIFMM_FILE_TYPE_EBOOK:
#endif
                case MMIFMM_FILE_TYPE_FONT:
                case MMIFMM_FILE_TYPE_SZIP:
                case MMIFMM_FILE_TYPE_BROWSER:
#ifdef BROWSER_SUPPORT
                case  MMIFMM_FILE_TYPE_URL:
#endif
                    MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_OTHER_LIST_OPT, MMIFMM_OTHER_OPT_MENU_WIN_ID, MMIFMM_OTHER_OPT_MENU_CTRL_ID);
                    break;

                default:
                    MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_NORMAL_FILE_LIST_OPT, MMIFMM_NORMAL_OPT_MENU_WIN_ID, MMIFMM_NORMAL_OPT_MENU_CTRL_ID);
                    break;
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : handle pub menu win ok handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuOKHandler (MMIPUB_MENU_NOTIFY_T *param)
{
    if (PNULL != param)
    {
        HandleMenuOption (param);
    }
}

/*****************************************************************************/
//  Description : handle pub menu win cancel handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuCancelHandler (MMIPUB_MENU_NOTIFY_T *param)
{
    if (PNULL != param)
    {
        HandleMenuOption (param);
    }
}

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuInitHandler (MMIPUB_MENU_NOTIFY_T *param)
{
    if (PNULL != param)
    {
        MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (param->dst_handle);

        if (PNULL != view_win_d && PNULL != view_win_d->main_d_ptr)
        {
            if (0 == view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num)
            {
                GUIMENU_POP_SEL_INFO_T      pop_item_info = {0};
                MMIFILE_DEVICE_E            device_type = MMI_DEVICE_UDISK;
                BOOLEAN                     udisk_status =  FALSE;
                MMI_MENU_ID_T               menu_id = 0 ;

                MMIAPICOM_FreeGrayed (param->win_id);

                for (device_type = MMI_DEVICE_UDISK; device_type < MMI_DEVICE_NUM; device_type++)
                {
#ifdef MSDC_CARD_SUPPORT
                    udisk_status = (MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (device_type), MMIAPIFMM_GetDevicePathLen (device_type)) || MMIAPISD_IsSDNeedFormat (device_type));
#else
                    udisk_status = MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (device_type), MMIAPIFMM_GetDevicePathLen (device_type));
#endif
                    MMIAPICOM_EnableGrayed (param->win_id, MENU_FMM_MEMORY_OPT, ID_FMM_UDISK_MEMORY + device_type - MMI_DEVICE_UDISK, (BOOLEAN) !udisk_status);
                    MMIAPICOM_EnableGrayed (param->win_id, MENU_FMM_MEMORY_OPT, ID_FMM_UDISK_FORMAT + device_type - MMI_DEVICE_UDISK, (BOOLEAN) !udisk_status);
                    MMIAPICOM_EnableGrayed (param->win_id, MENU_FMM_MEMORY_OPT, ID_FMM_MEMCARD_RENAME + device_type - MMI_DEVICE_SDCARD, (BOOLEAN) !udisk_status);
				}

                //设置当前的排序类型
                switch (MMIFMM_GeFileSortType())
                {
                case MMIFMM_SORT_TYPE_NAME:
                    menu_id = ID_FMM_NAME;
                    break;

                case MMIFMM_SORT_TYPE_TIME:
                    menu_id = ID_FMM_DATE;
                    break;

                case MMIFMM_SORT_TYPE_SIZE:
                    menu_id = ID_FMM_SIZE;
                    break;

                case MMIFMM_SORT_TYPE_TYPE:
                    menu_id = ID_FMM_TYPE;
                    break;

                default:
                    menu_id = ID_FMM_NAME;
                    break;
                }

                pop_item_info.is_static = TRUE;
                pop_item_info.ctrl_id = param->ctrl_id;
                pop_item_info.menu_id = menu_id;
                pop_item_info.group_id = MENU_FMM_SORT_OPT;

                GUIMENU_SetPopItemStatus (TRUE, &pop_item_info);
            }
            else
            {
                GUIMENU_GetId(param->ctrl_id,&param->group_id,&param->menu_id);
                DisableInvalidMenuItem (param->win_id,
                                        view_win_d->main_d_ptr->s_fmm_list_data_ptr,
                                        view_win_d->main_d_ptr->s_fmm_current_path_ptr,
                                        param->group_id,
                                        view_win_d->list_ctrl_id,
                                        param->ctrl_id
                                       );
            }
        }
    }

}

/*****************************************************************************/
//  Description : handle pub menu win close handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuCloseHandler (MMIPUB_MENU_NOTIFY_T *param)
{
    if (PNULL != param)
    {
        MMIAPICOM_FreeGrayed (param->win_id);
    }
}

/*****************************************************************************/
//  Description : open menu pub win
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_OpenMeunPubWin (
    MMI_WIN_ID_T         dst_win_id,
    MMI_MENU_GROUP_ID_T  menu_group_id,
    MMI_WIN_ID_T         menu_win_id,
    MMI_CTRL_ID_T        menu_ctrl_id
)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = menu_group_id;
    menuparam.win_id = menu_win_id;
    menuparam.ctrl_id = menu_ctrl_id;

    menuparam.dst_handle = dst_win_id;
    menuparam.func.OnMenuOk = HandlePubMenuOKHandler;
    menuparam.func.OnMenuCancel = HandlePubMenuCancelHandler;
    menuparam.func.OnMenuInit = HandlePubMenuInitHandler;
    menuparam.func.OnMenuDestory = HandlePubMenuCloseHandler;
    MMIPUB_OpenPubMenuWin (&menuparam);
}




/*****************************************************************************/
//  Description : get sub folder max file path len
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMenuOption (MMIPUB_MENU_NOTIFY_T *param)
{
    BOOLEAN                 result = FALSE;
    MMI_CTRL_ID_T           sel_ctrl_id = MMIFMM_UDISK_LISTBOX_CTRL_ID;
    uint32                  cur_tab_index = 0;
    MMI_WIN_ID_T            cur_child_win_id = MMIFMM_UDISK_CHILD_WIN_ID;
    MMI_MENU_GROUP_ID_T     group_id = 0;
    MMI_MENU_ID_T           menu_id = 0;
#if defined (MMI_VCALENDAR_SUPPORT) || defined(MMI_VCARD_SUPPORT)
    static MMIVC_SMS_SEND_TYPE_E  send_type = MMIVC_SMS_SEND_NORMAL;
    MMI_TEXT_ID_T           result_text = TXT_NULL;
#endif
#if defined MMI_VCARD_SUPPORT
    VCARD_ERROR_E           vcard_result = VCARD_NO_ERROR;
#endif

#ifdef MMI_VCALENDAR_SUPPORT
    MMIVCAL_ERROR_TYPE_E vcalendar_result = MMIVCAL_NO_ERROR;
#endif
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = PNULL;

    if (PNULL == param)
    {
        return FALSE;
    }

    view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);

    if (PNULL == view_win_d || PNULL == view_win_d->main_d_ptr)
    {
        SCI_TRACE_LOW ("MMIFMM HandleMenuOption PNULL == view_win_d!");
        return FALSE;
    }

    if (PNULL == view_win_d->main_d_ptr->s_fmm_current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[HandleMenuOption]PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10714_112_2_18_2_20_58_520, (uint8*) "");
        return FALSE;
    }

    cur_tab_index = GUITAB_GetCurSel (view_win_d->tab_ctrl_id);
    sel_ctrl_id = view_win_d->list_ctrl_id;
    cur_child_win_id = param->dst_handle;

    if (0 == param->group_id && 0 == param->menu_id)
    {
        menu_id = GUIMENU_GetCurNodeId (param->ctrl_id);
    }
    else
    {
        GUIMENU_GetId (param->ctrl_id, &group_id, &menu_id);
    }

    switch (menu_id)
    {
    case ID_FMM_NEW_FOLDER:
        view_win_d->s_list_file_name_len = 0;

        //以下内存分配后在udisk win中释放
        if (PNULL != view_win_d->s_list_file_name_ptr)
        {
            SCI_FREE (view_win_d->s_list_file_name_ptr);
            view_win_d->s_list_file_name_ptr = PNULL;
        }

        view_win_d->s_list_file_name_ptr = SCI_ALLOC_APPZ (sizeof (wchar) * (MMIFMM_FILENAME_LEN + 1));

        if (PNULL == view_win_d->s_list_file_name_ptr)
        {
            SCI_TRACE_LOW ("MMIFMM:HandleMenuOption PNULL == s_list_file_name_ptr");
            return FALSE;
        }

        MMIFMM_FmmCreateFolder (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->s_list_file_name_ptr, &view_win_d->s_list_file_name_len, MMIFMM_UDISK_CHILD_WIN_ID, MSG_FMM_NEW_FOLDER_CNF);
        break;
    case ID_FMM_SEND:
#if defined (BLUETOOTH_SUPPORT)
    case ID_FMM_BLUETOOTH:
        MMIFMM_FmmBtSendFile (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (sel_ctrl_id));
#endif
        break;
    case  ID_FMM_DELETE:
        MMIFMM_DeleteSelectFile (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id, view_win_d->main_d_ptr->s_fmm_current_path_ptr);
        break;

    case  ID_FMM_RENAME:
        MMIFMM_FmmRenameFile (GUILIST_GetCurItemIndex (sel_ctrl_id), cur_child_win_id, MSG_FMM_RENAME_END_CNF, view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->s_list_file_name_ptr);
        
		break;

    case  ID_FMM_COPY:
        MMIFMM_CopySelectFileToFolder (view_win_d->main_d_ptr->s_fmm_list_data_ptr, GUILIST_GetCurItemIndex (sel_ctrl_id), view_win_d->main_d_ptr->s_fmm_current_path_ptr);
        break;

#ifdef MMI_FMM_MOVE_SUPPORT
    case ID_FMM_MOVE:
        MMIFMM_MoveSlectFileToFolder (view_win_d->main_d_ptr->s_fmm_list_data_ptr, GUILIST_GetCurItemIndex (sel_ctrl_id), view_win_d->main_d_ptr->s_fmm_current_path_ptr);
        break;
#endif

    case  ID_FMM_DETAIL:
        MMIFMM_FmmFileDetail (GUILIST_GetCurItemIndex (sel_ctrl_id), view_win_d->main_d_ptr->s_fmm_list_data_ptr);
        break;

    case ID_FMM_NAME:
        MMIFMM_FmmFileDataSort (param->dst_handle, view_win_d->main_d_ptr->s_fmm_list_data_ptr, MMIFMM_SORT_TYPE_NAME);
        break;

    case ID_FMM_DATE:
        MMIFMM_FmmFileDataSort (param->dst_handle, view_win_d->main_d_ptr->s_fmm_list_data_ptr, MMIFMM_SORT_TYPE_TIME);
        break;

    case ID_FMM_SIZE:
        MMIFMM_FmmFileDataSort (param->dst_handle, view_win_d->main_d_ptr->s_fmm_list_data_ptr, MMIFMM_SORT_TYPE_SIZE);
        break;

    case ID_FMM_TYPE:
        MMIFMM_FmmFileDataSort (param->dst_handle, view_win_d->main_d_ptr->s_fmm_list_data_ptr, MMIFMM_SORT_TYPE_TYPE);
        break;
//fixed by liyan.zhu for bugzilla 2760 2012-05-22 fmm search support
#ifdef MMI_FMM_SEARCH_SUPPORT
    case  ID_FMM_SEARCH:
        MMIFMM_FmmOpenFindFileWin (view_win_d->main_d_ptr->s_fmm_current_path_ptr->pathname, view_win_d->main_d_ptr->s_fmm_list_data_ptr, MMIFMM_FILE_NUM, (uint32)  cur_child_win_id, MSG_FMM_SEARCH_CNF);
        break;
#endif
#ifdef MMI_PDA_SUPPORT
    case  ID_FMM_MARK_OPT:
        HandlePDAMenuMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id, param->dst_handle);
        break;
#endif

    case  ID_FMM_SUB_MARK:
        HandleMenuMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id);
        break;

    case  ID_FMM_CANCEL_MARK:
        HandleMenuCancelMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id);
        break;

    case  ID_FMM_MARK_ALL:
        HandleMenuMarkAll (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id);
        break;

    case  ID_FMM_CANCEL_ALL_MARK:
        HandleMenuCancelAllMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id);
        break;

    case ID_FMM_PREVIEW:
        // 生成图像文件数据
        HandleMenuCancelAllMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id);

        //分配的内存在preview窗口中释放
        if (PNULL == view_win_d->s_pic_index_data_ptr)
        {
            view_win_d->s_pic_index_data_ptr = SCI_ALLOC_APPZ (sizeof (MMIFMM_PICTURE_INDEX_DATA_T));
        }

        if (PNULL == view_win_d->s_pic_index_data_ptr)
        {
            SCI_TRACE_LOW ("MMIFMM:HandleMenuOption PNULL == view_win_d->s_pic_index_data_ptr");
            return FALSE;
        }

        SetupPicIndexData (param->dst_handle, sel_ctrl_id, view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->s_pic_index_data_ptr);
        // 打开图像浏览窗口
        MMIFMM_PicPreview (sel_ctrl_id, view_win_d->s_pic_index_data_ptr);
        break;
#if defined (MMS_SUPPORT)
    case ID_FMM_MMS://CR162740
		if (0 == view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num)
		{//没有mark发送当前文件
			MMIFMM_FmmSendMMS (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (sel_ctrl_id));
		}
		else if (1 == view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num)
		{//有mark发送标记的文件。注：只能mark1个发送mms，多个不能发送。
			uint16 file_i = 0;
			while(file_i < view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num+view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num)
			{
				if(MMIFMM_FILE_STATE_UNCHECKED != view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[file_i]->state)
				{//找到一个立马返回。
					MMIFMM_FmmSendMMS (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, file_i);
					break;
				}
				file_i++;
			}
		}

        
        break;
#endif
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    case ID_FMM_RING:
        MMIFMM_FmmSetCallRing (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (sel_ctrl_id)
			, MN_DUAL_SYS_1
#ifndef MMI_PDA_SUPPORT
			,MMIMULTIM_SET_CALL_RING
#endif
        );
		break;

    case ID_FMM_SIM1_RING:
    case ID_FMM_SIM2_RING:
    case ID_FMM_SIM3_RING:
    case ID_FMM_SIM4_RING:
        //FMM_SIM1/SIM2/SIM3/SIM4_RING handle seperately for multisim flow!!
        MMIFMM_FmmSetCallRing(view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr
			, GUILIST_GetCurItemIndex (sel_ctrl_id), (MN_DUAL_SYS_E) (menu_id - ID_FMM_SIM1_RING)
#ifndef MMI_PDA_SUPPORT
			,MMIMULTIM_SET_CALL_RING
#endif
			);
		    break;
	case ID_FMM_ALARM1_RING:
	case ID_FMM_ALARM1_RING+1:
    case ID_FMM_ALARM1_RING+2:
//#if (ALM_ALARM_NUM > 3)//ndef MMI_ALARM_MINI_LOW_COST
//	case ID_FMM_ALARM1_RING+3:
//	case ID_FMM_ALARM1_RING+4:
//#if (ALM_ALARM_NUM > 5)//ndef ALARM_SUPPORT_LOW_MEMORY
//	case ID_FMM_ALARM1_RING+5:
//	case ID_FMM_ALARM1_RING+6:
//	case ID_FMM_ALARM1_RING+7:
//	case ID_FMM_ALARM1_RING+8:
//	case ID_FMM_ALARM1_RING+9:
//#endif
//#endif
		MMIFMM_FmmSetCallRing(view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr
			, GUILIST_GetCurItemIndex (sel_ctrl_id), (MN_DUAL_SYS_E) (menu_id-ID_FMM_ALARM1_RING)
#ifndef MMI_PDA_SUPPORT
			,MMIMULTIM_SET_ALARM_RING
#endif
			);

		break;
#endif

#ifdef SCREENSAVER_SUPPORT
    case ID_FMM_SCEENSAVE:
        MMIFMM_FmmSetAsScreenSaver (GUILIST_GetCurItemIndex (sel_ctrl_id), view_win_d->main_d_ptr->s_fmm_list_data_ptr);
        break;
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    case ID_FMM_VIDEO_WALLPAPER:
        MMIFMM_FmmSetAsVideoWallpaper (GUILIST_GetCurItemIndex (sel_ctrl_id), view_win_d->main_d_ptr->s_fmm_list_data_ptr);
        break;
#endif
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
    case ID_FMM_POWER_ON_SET:
        MMIFMM_FmmSetAsPowerOnOffAnim (GUILIST_GetCurItemIndex (sel_ctrl_id), view_win_d->main_d_ptr->s_fmm_list_data_ptr, TRUE);
        break;
    case ID_FMM_POWER_OFF_SET:
        MMIFMM_FmmSetAsPowerOnOffAnim (GUILIST_GetCurItemIndex (sel_ctrl_id), view_win_d->main_d_ptr->s_fmm_list_data_ptr, FALSE);
        break;
#endif
#ifndef FLASH_SUPPORT
    case ID_FMM_WALLPAPER:
        MMIFMM_FmmSetWallpaper (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (sel_ctrl_id));
        break;
#endif
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    case ID_FMM_PB:
        MMIFMM_FmmSetPB (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (sel_ctrl_id));
        break;
#endif
#ifdef MMI_VCALENDAR_SUPPORT
    case ID_FMM_VCALENDAR_SMS:
        MMIAPISMS_ClearDestNum();
        vcalendar_result = MMIFMM_GetVcalendarFileData (view_win_d->main_d_ptr->s_fmm_list_data_ptr,
                           view_win_d->main_d_ptr->s_fmm_current_path_ptr,
                           GUILIST_GetCurItemIndex (sel_ctrl_id));

        if (MMIVCAL_NO_ERROR == vcalendar_result)
        {
            send_type = MMIVC_SMS_SEND_VCALENDAR;
            MMIAPISMS_EnterSendOptionMenu((uint32)send_type);/*lint !e746 !e718*/
        }
        else
        {   //提示错误信息
            switch (vcalendar_result)
            {
            case MMIVCAL_IS_EMPTY:
                result_text = TXT_EMPTY_FILE;
                break;
            case MMIVCAL_IS_TOOBIG:
                result_text = TXT_FILESIZE_TOOBIG;
                break;
            case MMIVCAL_FILE_ERROR:
                result_text = TXT_ERROR;
                break;
            default:
                result_text = TXT_ERROR;
                break;
            }

            MMIPUB_OpenAlertWarningWin (result_text);
            MMIVCAL_VcalBuildExit();
        }

        break;
#endif
#ifdef MMI_VCARD_SUPPORT
    case ID_FMM_LOAD_VCARD:
        {
            wchar *file_name_ptr = PNULL;
            uint32 file_name_len = 0;
            uint16 index = 0;

            file_name_ptr = (wchar*) SCI_ALLOC_APP (sizeof (wchar) * (MMIFMM_FULL_FILENAME_LEN + 1));

            if (PNULL == file_name_ptr)
            {
                break;
            }

            index = GUILIST_GetCurItemIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID);
            file_name_len = MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, index, file_name_ptr, MMIFMM_FULL_FILENAME_LEN + 1);

            //MMIPB_LoadVCardFile(file_name_ptr,file_name_len);
            if (MMIPB_ERROR_PENDING == MMIAPIPB_ImportVcardAsyn (MMIAPIPB_CreateHandle(), file_name_ptr, Importvcard_Callback))
            {
                MMIFMM_OpenComWaitWin (HandleImportVcardWaitWinMsg);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId (PNULL, TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_FAIL, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }

            SCI_FREE (file_name_ptr);
        }
        break;
#endif
#ifdef MMI_VCALENDAR_SUPPORT
    case ID_FMM_SAVE_VCALENDAR:
        {
            FILEARRAY file_array = MMIAPISCH_CreateFileArray();
            FILEARRAY_DATA_T   *data_ptr = PNULL;
            int num = view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num;
            int i = 0;
            wchar *file_name_ptr = PNULL;
            uint32 file_name_len = 0;
            uint16 index = 0;

            data_ptr = (FILEARRAY_DATA_T*) SCI_ALLOC_APP (sizeof (FILEARRAY_DATA_T));

            if (PNULL == data_ptr)
            {
                if (PNULL != file_array)
                {
                    MMIAPIFILEARRAY_Destroy (&file_array);
                    file_array = PNULL;
                }

                break;
            }

            file_name_ptr = (wchar*) SCI_ALLOC_APP (sizeof (wchar) * (MMIFMM_FULL_FILENAME_LEN + 1));

            if (PNULL == file_name_ptr)
            {
                SCI_FREE (data_ptr);
                data_ptr = PNULL;

                if (PNULL != file_array)
                {
                    MMIAPIFILEARRAY_Destroy (&file_array);
                    file_array = PNULL;
                }

                break;
            }

            if (PNULL != file_array)
            {
                if (MMIFMM_FILE_STATE_UNCHECKED == view_win_d->main_d_ptr->s_fmm_list_data_ptr->checked)
                {
                    index = GUILIST_GetCurItemIndex (sel_ctrl_id);
                    SCI_MEMSET (data_ptr, 0, sizeof (FILEARRAY_DATA_T));
                    file_name_len = MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, index, file_name_ptr, MMIFMM_FULL_FILENAME_LEN + 1);
                    data_ptr->name_len = file_name_len;
                    MMIAPICOM_Wstrncpy (data_ptr->filename, file_name_ptr, data_ptr->name_len);
                    data_ptr->size = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->file_size;
                    data_ptr->type = FILEARRAY_TYPE_FILE;
                    MMIAPIFILEARRAY_Add (file_array, data_ptr);
                }
                else
                {
                    for (i = 0; i < num; i++)
                    {
                        if ( (MMIFMM_FILE_STATE_CHECKED == view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[i]->state))
                        {
                            SCI_MEMSET (data_ptr, 0, sizeof (FILEARRAY_DATA_T));
                            file_name_len = MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, i, file_name_ptr, MMIFMM_FULL_FILENAME_LEN + 1);
                            data_ptr->name_len = file_name_len;
                            MMIAPICOM_Wstrncpy (data_ptr->filename, file_name_ptr, data_ptr->name_len);
                            data_ptr->size = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[i]->file_size;
                            data_ptr->type = FILEARRAY_TYPE_FILE;
                            MMIAPIFILEARRAY_Add (file_array, data_ptr);
                        }
                    }
                }
                MMIAPISCH_LoadVcalendar (file_array);
            }

            SCI_FREE (data_ptr);
            data_ptr = PNULL;

            SCI_FREE (file_name_ptr);
            file_name_ptr = PNULL;
        }
        break;
#endif
#if defined MMI_VCARD_SUPPORT
    case ID_FMM_SMS:
        MMIAPISMS_ClearDestNum();
		{//CR202146 
			uint32 send_index = 0;
			if (0 == view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num)
			{//没有mark发送当前文件
				send_index = GUILIST_GetCurItemIndex (sel_ctrl_id);
			}
			else if (1 == view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num)
			{//有mark发送标记的文件。注：只能mark1个发送sms，多个不能发送。
				uint16 file_i = 0;
				while(file_i < view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num+view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num)
				{
					if(MMIFMM_FILE_STATE_UNCHECKED != view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[file_i]->state)
					{//找到一个立马返回。
						send_index = file_i;
						break;
					}
					file_i++;
				}
			}
			vcard_result = MMIFMM_GetVcardFileData (view_win_d->main_d_ptr->s_fmm_list_data_ptr,
				view_win_d->main_d_ptr->s_fmm_current_path_ptr,
                                                send_index);
		}     

        if (VCARD_NO_ERROR == vcard_result)
        {          
            send_type = MMIVC_SMS_SEND_VCARD;
            MMIAPISMS_EnterSendOptionMenu((uint32)send_type);/*lint !e746 !e718*/
        }
        else
        {   //提示错误信息
            switch (vcard_result)
            {
            case VCARD_EMPTY_FILE:
                result_text = TXT_EMPTY_FILE;
                break;
            case VCARD_TOBIG_FILE:
                result_text = TXT_FILESIZE_TOOBIG;
                break;
            case VCARD_FILE_ERROR:
                result_text = TXT_ERROR;
                break;
            default:
                result_text = TXT_ERROR;
                break;
            }

            MMIPUB_OpenAlertWarningWin (result_text);
        }

        break;
#endif
#ifdef MMI_VCALENDAR_SUPPORT
    case  ID_FMM_EMAIL:
        break;
#endif
    case ID_FMM_MEMCARD_RENAME:
            MMIFMM_EnterMemCardReNameWin (MMI_DEVICE_SDCARD);
        break;
#ifdef DUAL_TCARD_SUPPORT
	 case ID_FMM_MEMCARD1_RENAME:
		    MMIFMM_EnterMemCardReNameWin (MMI_DEVICE_SDCARD_1);
		 break;
#endif
        
    default:
        if (menu_id >= ID_FMM_UDISK_MEMORY && menu_id <= ID_FMM_MEMCARD_MEMORY_NUM)
        {
            //memory status menu
            //MMK_CloseWin( param->dst_handle);
            MMIFMM_FmmMemoryDetail ( (uint32) MMI_DEVICE_UDISK + (menu_id - ID_FMM_UDISK_MEMORY));
        }
        else if (menu_id >= ID_FMM_UDISK_FORMAT && menu_id <= ID_FMM_MEMCARD_FORMAT_NUM)
        {
            MMI_TEXT_ID_T alert_text_id = TXT_IS_FORMAT_SDCARD;
#ifdef TCARD_PB_SUPPORT

            if(MMIAPIPB_IsTCardContactExist()
               &&((MMI_DEVICE_UDISK + (menu_id - ID_FMM_UDISK_FORMAT)) == MMIAPIFMM_GetFirstValidSD())
                )
            {

                alert_text_id = TXT_FMM_FORMAT_TCARD_CONTACT_ALERT;
            }

#endif
#ifdef MCARE_V31_SUPPORT

            if (MCareV31_Exit_Confirm())
            {
                return FALSE;	///具体的return值不同，和添加出的函数相关
            }

#endif
            //format memory menu
            {
                MMIFMM_FmmMemoryFormat( (uint32) MMI_DEVICE_UDISK + (menu_id - ID_FMM_UDISK_FORMAT), alert_text_id);
            }
        }

        //SCI_ASSERT(0);
        break;
    }

    result = TRUE;
    return TRUE;
}


/*****************************************************************************/
//  Description : handle menu option function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_HandleMenuOpt (MMIFMM_OPTION_TYPE_E type)
{

    MMI_CTRL_ID_T sel_ctrl_id = MMIFMM_UDISK_LISTBOX_CTRL_ID;
    MMI_WIN_ID_T cur_child_win_id = MMIFMM_UDISK_CHILD_WIN_ID;
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);

    if (PNULL == view_win_d)
    {
        SCI_TRACE_LOW ("MMIFMM MMIFMM_HandleMenuOpt PNULL == view_win_d!");
        return;
    }

    switch (type)
    {
#if defined (BLUETOOTH_SUPPORT)
    case MMUFMM_BT_SEND_OPT:
        MMIFMM_FmmBtSendFile (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, GUILIST_GetCurItemIndex (sel_ctrl_id));
        break;
#endif
    case MMIFMM_DELETE_OPT:
        MMIFMM_DeleteSelectFile (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id, view_win_d->main_d_ptr->s_fmm_current_path_ptr);
        break;
    case MMIFMM_DETAIL_OPT:
        MMIFMM_FmmFileDetail (GUILIST_GetCurItemIndex (sel_ctrl_id), view_win_d->main_d_ptr->s_fmm_list_data_ptr);
        break;
    case MMIFMM_SUB_MARK_OPT:
        HandleMenuMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id);
        break;
    case MMIFMM_CANCEL_MARK_OPT:
        HandleMenuCancelMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id);
        break;
    case MMIFMM_MARK_ALL_OPT:
        HandleMenuMarkAll (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id);
        break;
    case MMIFMM_CANCEL_ALL_MARK_OPT:
        HandleMenuCancelAllMark (view_win_d->main_d_ptr->s_fmm_list_data_ptr, sel_ctrl_id);
        break;
    case MMIFMM_RENAME_OPT:
        MMIFMM_FmmRenameFile (GUILIST_GetCurItemIndex (sel_ctrl_id), cur_child_win_id, MSG_FMM_RENAME_END_CNF, view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->s_list_file_name_ptr);
        break;
    case MMIFMM_COPY_OPT:
        MMIFMM_CopySelectFileToFolder (view_win_d->main_d_ptr->s_fmm_list_data_ptr, GUILIST_GetCurItemIndex (sel_ctrl_id), view_win_d->main_d_ptr->s_fmm_current_path_ptr);
        break;
    default:
        //SCI_ASSERT(0);
        //SCI_TRACE_LOW:"MMIFMM:[MMIAPIFMM_HandleMenuOpt] type = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11636_112_2_18_2_20_59_525, (uint8*) "d", type);
        break;
    }

}


/*****************************************************************************/
//  Description : handle menu mark
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuMark (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMI_CTRL_ID_T         ctrl_id
)
{
    uint16   current_index = 0;

    //SCI_TRACE_LOW:"MMIFMM: HandleMenuMark."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3554_112_2_18_2_20_42_372, (uint8*) "");

    //SCI_ASSERT(PNULL != list_data_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[HandleMenuMark]PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3558_112_2_18_2_20_42_373, (uint8*) "");
        return;
    }

    current_index = GUILIST_GetCurItemIndex (ctrl_id);
    list_data_ptr->data[current_index]->state = MMIFMM_FILE_STATE_CHECKED;

    if (0 == list_data_ptr->mark_num)
    {
        GUILIST_SetTextListMarkable (ctrl_id, TRUE);
        GUILIST_SetMaxSelectedItem (ctrl_id, (uint16) (list_data_ptr->file_num + list_data_ptr->folder_num));
    }

    list_data_ptr->mark_num++;

	//CR161515
	// 判断是外部模块通过filter访问fmm还是从主入口main访问（）区别在于前者mark all是部分文件而不是全部文件
	if ((MMIFMM_FILE_ALL != MMIFMM_GetDisplayFileType())||(MMIFMM_FMI_STATE_LOOKUP_FILE == list_data_ptr->state))
	{
		list_data_ptr->checked = MMIFMM_FILE_STATE_CHECKED;
	}
	else
	{
		if (list_data_ptr->mark_num == list_data_ptr->file_num + list_data_ptr->folder_num)
		{
			list_data_ptr->checked = MMIFMM_FILE_STATE_ALL_CHECKED;
		}
		else
		{
			list_data_ptr->checked = MMIFMM_FILE_STATE_CHECKED;
		}
	}

    // 设置listbox 为标记
    GUILIST_SetSelectedItem (ctrl_id, current_index, TRUE);
}


/*****************************************************************************/
//  Description : handle menu cancel mark
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuCancelMark (
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMI_CTRL_ID_T        ctrl_id
)
{
    uint16   current_index = 0;

    //SCI_TRACE_LOW:"MMIFMM: HandleMenuCancelMark."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3595_112_2_18_2_20_42_374, (uint8*) "");

    //SCI_ASSERT(PNULL != list_data_ptr);
    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[HandleMenuCancelMark]PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3599_112_2_18_2_20_42_375, (uint8*) "");
        return;
    }

    current_index = GUILIST_GetCurItemIndex (ctrl_id);

    if (list_data_ptr->data[current_index]->state == MMIFMM_FILE_STATE_CHECKED)
    {
        list_data_ptr->data[current_index]->state = MMIFMM_FILE_STATE_UNCHECKED;
        list_data_ptr->mark_num--;
    }

    // 取消listbox 为标记
    GUILIST_SetSelectedItem (ctrl_id, current_index, FALSE);
#ifdef MMI_PDA_SUPPORT
    GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif

    if (0 == list_data_ptr->mark_num)
    {
        list_data_ptr->checked = MMIFMM_FILE_STATE_UNCHECKED;
        GUILIST_SetTextListMarkable (ctrl_id, FALSE);
    }
    else if (list_data_ptr->file_num + list_data_ptr->folder_num == list_data_ptr->mark_num)
    {
        list_data_ptr->checked = MMIFMM_FILE_STATE_ALL_CHECKED;
    }
    else
    {
        list_data_ptr->checked = MMIFMM_FILE_STATE_CHECKED;
    }
}


/*****************************************************************************/
//  Description : enablel menu item
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void DisableInvalidMenuItem (
    MMI_WIN_ID_T          win_id,
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMIFMM_PATH_INFO_T    *current_path_ptr,
    MMI_MENU_GROUP_ID_T   group_id,
    MMI_CTRL_ID_T         list_ctrl_id,
    MMI_CTRL_ID_T         menu_ctrl_id
)
{
    uint16                  current_index = 0;
    MMIFMM_FOLDER_TYPE_E    folder_type = MMIFMM_FOLDER_TYPE_NORMAL;
    MMI_MENU_GROUP_ID_T     mark_group_id = MENU_FMM_MARK_OPT;
#ifndef MMI_GUI_STYLE_MINISCREEN
    MMI_MENU_GROUP_ID_T     edit_group_id = MENU_FMM_LIST_EDIT_OPT;
#else
    MMI_MENU_GROUP_ID_T     edit_group_id = group_id;
#endif
    MMI_MENU_GROUP_ID_T     sort_group_id = MENU_FMM_SORT_OPT;
    MMI_MENU_GROUP_ID_T     mem_group_id  = MENU_FMM_MEMORY_OPT;
    MMI_MENU_ID_T           menu_id = 0;
    MMIFILE_DEVICE_E        device_type = MMI_DEVICE_UDISK;
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
    BOOLEAN                 udisk_status = 0;
    BOOLEAN                 is_include_path = FALSE;
    uint32 					file_type = 0;
#ifdef DRM_SUPPORT
    wchar 					file_name[MMIFMM_FILE_FILE_NAME_MAX_LEN + 1] = {0};
#endif
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);
    //SCI_TRACE_LOW:"MMIFMM: EnablelMenuItem."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3733_112_2_18_2_20_42_379, (uint8*) "");

    if (PNULL == view_win_d)
    {
        SCI_TRACE_LOW ("MMIFMM DisableInvalidMenuItem PNULL == view_win_d!");
        return;
    }

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[DisableInvalidMenuItem] PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3737_112_2_18_2_20_42_380, (uint8*) "");
        return;
    }

    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[DisableInvalidMenuItem] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_3743_112_2_18_2_20_42_381, (uint8*) "");
        return;
    }

    current_index = GUILIST_GetCurItemIndex (list_ctrl_id);

    if (list_data_ptr->path_is_valid)
    {
        is_include_path = TRUE;
    }

    folder_type =  MMIAPIFMM_GetFolderType (list_data_ptr->data[current_index]->filename_ptr,
                                            list_data_ptr->data[current_index]->filename_len, is_include_path);

    //MMIAPIUDISK_GetAllDeviceStatus(&device_status[MMI_DEVICE_UDISK],&device_status[MMI_DEVICE_SDCARD]);
    MMIAPICOM_FreeGrayed (win_id);

    for (device_type = MMI_DEVICE_UDISK; device_type < MMI_DEVICE_NUM; device_type++)
    {
        udisk_status = MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (device_type), MMIAPIFMM_GetDevicePathLen (device_type));
        MMIAPICOM_EnableGrayed (win_id, mem_group_id, ID_FMM_UDISK_MEMORY + device_type - MMI_DEVICE_UDISK, (BOOLEAN) !udisk_status);
#ifdef MSDC_CARD_SUPPORT
        udisk_status = udisk_status | MMIAPISD_IsSDNeedFormat (device_type);
#endif
        MMIAPICOM_EnableGrayed (win_id, mem_group_id, ID_FMM_UDISK_FORMAT + device_type - MMI_DEVICE_UDISK, (BOOLEAN) !udisk_status);
		MMIAPICOM_EnableGrayed(win_id, mem_group_id,ID_FMM_MEMCARD_RENAME + device_type - MMI_DEVICE_SDCARD, (BOOLEAN)!udisk_status); 
   }
//memcard rename 分开
//     device_table = view_win_d->main_d_ptr->s_file_path_info_ptr->s_device_table[GUITAB_GetCurSel (MMIFMM_TAB_CTRL_ID) ];
// 
//     if (MMI_DEVICE_UDISK == device_table)
//     {
//         GUIMENU_SetItemVisible (MMIFMM_NORMAL_OPT_MENU_CTRL_ID, MENU_FMM_MEMORY_OPT, ID_FMM_MEMCARD_RENAME, FALSE);
//     }
//     else
//     {
//         GUIMENU_SetItemVisible (MMIFMM_NORMAL_OPT_MENU_CTRL_ID, MENU_FMM_MEMORY_OPT, ID_FMM_MEMCARD_RENAME, TRUE);
//     }

    //CR161515
    // 判断是外部模块通过filter访问fmm还是从主入口main访问（）区别在于前者mark all是部分文件而不是全部文件
    if ((MMIFMM_FILE_ALL != MMIFMM_GetDisplayFileType())||(MMIFMM_FMI_STATE_LOOKUP_FILE == list_data_ptr->state))
    {//假mark all状态（存在一些没有被过滤出来的其他类型文件），但表面的options需要显示的如同真的一样。
        if (list_data_ptr->mark_num == list_data_ptr->file_num + list_data_ptr->folder_num)
        {
            list_data_ptr->checked = MMIFMM_FILE_STATE_ALL_CHECKED;
        }
    }
    switch (list_data_ptr->checked)
    {
    case MMIFMM_FILE_STATE_UNCHECKED:

        if (MMIFMM_FILE_STATE_UNCHECKED == list_data_ptr->data[current_index]->state)
        {
            MMIAPICOM_EnableGrayed (win_id, mark_group_id, ID_FMM_CANCEL_MARK, TRUE);
        }
        else if (MMIFMM_FILE_STATE_CHECKED == list_data_ptr->data[current_index]->state)
        {
            MMIAPICOM_EnableGrayed (win_id, mark_group_id, ID_FMM_SUB_MARK, TRUE);
        }

        if (list_data_ptr->mark_num == 0)
        {
            MMIAPICOM_EnableGrayed (win_id, mark_group_id, ID_FMM_CANCEL_ALL_MARK, TRUE);
        }

        file_type = list_data_ptr->data[current_index]->type;
#ifdef DRM_SUPPORT

        if (MMIFMM_FILE_TYPE_DRM ==  file_type)
        {
            MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, current_index , file_name, MMIFMM_FILE_FILE_NAME_MAX_LEN);
            file_type = MMIAPIDRM_GetMediaFileType (SFS_INVALID_HANDLE, file_name);
        }

#endif

        switch (group_id)
        {
        case MENU_FMM_PICTURE_LIST_OPT :

            if (MMIFMM_FILE_TYPE_PICTURE != file_type)
            {
				MMIAPICOM_EnableGrayed(win_id,group_id, ID_FMM_PIC_MANAGER, TRUE);
                //MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_WALLPAPER, TRUE);
                //MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_PB, TRUE);
            }

            break;

        case MENU_FMM_MUSIC_LIST_OPT :

            if (MMIFMM_FILE_TYPE_MUSIC != file_type)
            {
                MMIAPICOM_EnableGrayed (win_id, group_id,  ID_FMM_MUSIC_MANAGER, TRUE);
            }

            break;
            //case MENU_FMM_VCARD_LIST_OPT:
        case MENU_FMM_MOVIE_LIST_OPT :

            if (MMIFMM_FILE_TYPE_MOVIE != file_type)
            {
                MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_MANAGER, TRUE);
            }

            break;

        case MENU_FMM_NORMAL_LIST_OPT :

            if (MMIFMM_FMI_STATE_LOOKUP_FILE == list_data_ptr->state)
            {
                MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_NEW_FOLDER, TRUE);
            }

            break;

        default:
            break;
        }

        break;

    case MMIFMM_FILE_STATE_CHECKED:

        if (MMIFMM_FILE_STATE_CHECKED == list_data_ptr->data[current_index]->state)
        {
            MMIAPICOM_EnableGrayed (win_id, mark_group_id, ID_FMM_SUB_MARK, TRUE);
        }
        else
        {
            MMIAPICOM_EnableGrayed (win_id, mark_group_id, ID_FMM_CANCEL_MARK, TRUE);
        }

        if (list_data_ptr->mark_num == 0)
        {
            MMIAPICOM_EnableGrayed (win_id, mark_group_id, ID_FMM_CANCEL_ALL_MARK, TRUE);
        }

        //MMIAPICOM_EnableGrayed(win_id,group_id, ID_FMM_DELETE, TRUE);
        {
            MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_DELETE, FALSE);
        }
        // 标记状态，只能处理相关的操作
		if(list_data_ptr->mark_num > 1)//CR162740
		{
#if defined (MMS_SUPPORT) && defined (BLUETOOTH_SUPPORT)
			MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SEND_OPT, ID_FMM_MMS, TRUE);
#if defined MMI_VCARD_SUPPORT//CR202074
            MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SEND_VCARD_OPT, ID_FMM_MMS, TRUE);
			MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SEND_VCARD_OPT, ID_FMM_SMS, TRUE);
#endif
#else
            MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_MMS, TRUE);
#endif

		}
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_POWER_ON_SET, TRUE);
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_POWER_OFF_SET, TRUE);
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_NEW_FOLDER, TRUE);
        MMIAPICOM_EnableGrayed (win_id, edit_group_id, ID_FMM_RENAME, TRUE);
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_DETAIL, TRUE);
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_SORT, TRUE);
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_SEARCH, TRUE);
        MMIAPICOM_EnableGrayed  (win_id, MENU_FMM_MOVIE_LIST_OPT, ID_FMM_MANAGER, TRUE);                
#ifdef SCREENSAVER_SUPPORT
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_MANAGE_OPT, ID_FMM_SCEENSAVE,TRUE);
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_MANAGE_OPT, ID_FMM_VIDEO_WALLPAPER,TRUE);
#endif
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_MANAGE_OPT, ID_FMM_POWER_ON_SET,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_MANAGE_OPT, ID_FMM_POWER_OFF_SET,TRUE);
#endif
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_MUSIC_LIST_OPT, ID_FMM_RING,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SIM_SELECT_OPT, ID_FMM_SIM1_RING,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SIM_SELECT_OPT, ID_FMM_SIM2_RING,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SIM_SELECT_OPT, ID_FMM_SIM3_RING,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SIM_SELECT_OPT, ID_FMM_SIM4_RING,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_PICTURE_LIST_OPT, ID_FMM_WALLPAPER,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_PICTURE_LIST_OPT, ID_FMM_PREVIEW,TRUE);
        
//         if (MMIFMM_FILE_TYPE_FOLDER != list_data_ptr->data[current_index]->type)
//         {
//             MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_SEND, TRUE);
//         }

        switch (group_id)
        {
        case MENU_FMM_NORMAL_LIST_OPT :
        case MENU_FMM_OTHER_LIST_OPT :
            //MMIAPICOM_EnableGrayed(win_id,group_id, ID_FMM_SEARCH, TRUE);
            break;

        case MENU_FMM_PICTURE_LIST_OPT :
			MMIAPICOM_EnableGrayed(win_id,group_id, ID_FMM_PIC_MANAGER, TRUE);
            //MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_WALLPAPER, TRUE);
            //MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_PB, TRUE);
            MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_PREVIEW, TRUE);
            break;

        case MENU_FMM_MUSIC_LIST_OPT :
            MMIAPICOM_EnableGrayed (win_id, group_id,  ID_FMM_MUSIC_MANAGER, TRUE);
            break;
        case MENU_FMM_MOVIE_LIST_OPT :
            MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_MANAGER, TRUE);
            //lint -fallthrough
            break;
#if defined MMI_VCARD_SUPPORT
        case MENU_FMM_VCARD_LIST_OPT :
            break;
#endif
#ifdef MMI_VCALENDAR_SUPPORT
        case MENU_FMM_VCALENDAR_LIST_OPT :

            if (!IsSelectedSameFileType (list_data_ptr, MMIFMM_FILE_TYPE_VCALENDAR))
            {
                MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_SAVE_VCALENDAR, TRUE);
            }

            break;
#endif
        default:
            break;
        }

        break;

    case MMIFMM_FILE_STATE_ALL_CHECKED:
        // 标记all状态，如下相关的操作不操作
        MMIAPICOM_EnableGrayed (win_id, mark_group_id, ID_FMM_SUB_MARK, TRUE);
        MMIAPICOM_EnableGrayed (win_id, mark_group_id, ID_FMM_MARK_ALL, TRUE);
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_NEW_FOLDER, TRUE);
        MMIAPICOM_EnableGrayed (win_id, edit_group_id, ID_FMM_RENAME, TRUE);
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_DETAIL, TRUE);
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_SORT, TRUE);
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_SEARCH, TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_MOVIE_LIST_OPT, ID_FMM_MANAGER, TRUE);                
#ifdef SCREENSAVER_SUPPORT
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_MANAGE_OPT, ID_FMM_SCEENSAVE,TRUE);
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_MANAGE_OPT, ID_FMM_VIDEO_WALLPAPER,TRUE);
#endif
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_MANAGE_OPT, ID_FMM_POWER_ON_SET,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_MANAGE_OPT, ID_FMM_POWER_OFF_SET,TRUE);
#endif
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_MUSIC_LIST_OPT, ID_FMM_RING,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SIM_SELECT_OPT, ID_FMM_SIM1_RING,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SIM_SELECT_OPT, ID_FMM_SIM2_RING,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SIM_SELECT_OPT, ID_FMM_SIM3_RING,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SIM_SELECT_OPT, ID_FMM_SIM4_RING,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_PICTURE_LIST_OPT, ID_FMM_WALLPAPER,TRUE);
        MMIAPICOM_EnableGrayed (win_id, MENU_FMM_PICTURE_LIST_OPT, ID_FMM_PREVIEW,TRUE);
//         //除文件夹其他文件send都不能操作
//         if (MMIFMM_FILE_TYPE_FOLDER != list_data_ptr->data[current_index]->type)
//         {
//             MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_SEND, TRUE);
//         }

        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_DELETE, FALSE);
		if(list_data_ptr->mark_num > 1)//CR162740
		{
#if defined (MMS_SUPPORT) && defined (BLUETOOTH_SUPPORT)
			MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SEND_OPT, ID_FMM_MMS, TRUE);
#if defined MMI_VCARD_SUPPORT//CR202074
            MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SEND_VCARD_OPT, ID_FMM_MMS, TRUE);
			MMIAPICOM_EnableGrayed (win_id, MENU_FMM_SEND_VCARD_OPT, ID_FMM_SMS, TRUE);
#endif
#else
            MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_MMS, TRUE);
#endif
		}
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_POWER_ON_SET, TRUE);
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_POWER_OFF_SET, TRUE);

        switch (group_id)
        {
        case MENU_FMM_NORMAL_LIST_OPT :
        case MENU_FMM_OTHER_LIST_OPT :
            //MMIAPICOM_EnableGrayed(win_id,group_id, ID_FMM_SEARCH, TRUE);
            break;
        case MENU_FMM_PICTURE_LIST_OPT :
            //MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_WALLPAPER, TRUE);
            //MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_PB, TRUE);
			MMIAPICOM_EnableGrayed(win_id,group_id, ID_FMM_PIC_MANAGER, TRUE);
            MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_PREVIEW, TRUE);
            break;
        case MENU_FMM_MUSIC_LIST_OPT :
            MMIAPICOM_EnableGrayed (win_id, group_id,  ID_FMM_MUSIC_MANAGER, TRUE);
            break;
        case MENU_FMM_MOVIE_LIST_OPT :
            MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_MANAGER, TRUE);
            //lint -fallthrough
            //case MENU_FMM_VCARD_LIST_OPT :
            break;
#ifdef MMI_VCALENDAR_SUPPORT
        case MENU_FMM_VCALENDAR_LIST_OPT :

            if (!IsSelectedSameFileType (list_data_ptr, MMIFMM_FILE_TYPE_VCALENDAR))
            {
                MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_SAVE_VCALENDAR, TRUE);
            }

            break;
#endif
        default:
            break;
        }
		//CR161515
		// 判断是外部模块通过filter访问fmm还是从主入口main访问（）区别在于前者mark all是部分文件而不是全部文件
		if ((MMIFMM_FILE_ALL != MMIFMM_GetDisplayFileType())||(MMIFMM_FMI_STATE_LOOKUP_FILE == list_data_ptr->state))
		{//假mark all状态（存在一些没有被过滤出来的其他类型文件），但表面的options需要显示的如同真的一样。
			//但之后仍然需要将list的状态置回来
			list_data_ptr->checked = MMIFMM_FILE_STATE_CHECKED;
		}
        break;

    default:
        //SCI_PASSERT(0,(("list_data_ptr->checked = %d"),list_data_ptr->checked));
        //SCI_TRACE_LOW:"MMIFMM:[DisableInvalidMenuItem] list_data_ptr->checked = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_4002_112_2_18_2_20_43_382, (uint8*) "d", list_data_ptr->checked);
        break;
    }

    if (list_data_ptr->path_is_valid)
    {
        //in search result
        MMIAPICOM_EnableGrayed (win_id, group_id, ID_FMM_NEW_FOLDER, TRUE);
       // MMIAPICOM_EnableGrayed (win_id, edit_group_id, ID_FMM_RENAME, TRUE);
    }

    //设置当前的排序类型
    switch (list_data_ptr->sort)
    {
    case MMIFMM_SORT_TYPE_NAME:
        menu_id = ID_FMM_NAME;
        break;

    case MMIFMM_SORT_TYPE_TIME:
        menu_id = ID_FMM_DATE;
        break;

    case MMIFMM_SORT_TYPE_SIZE:
        menu_id = ID_FMM_SIZE;
        break;

    case MMIFMM_SORT_TYPE_TYPE:
        menu_id = ID_FMM_TYPE;
        break;

    default:
        menu_id = ID_FMM_NAME;
        break;
    }

    pop_item_info.is_static = TRUE;
    pop_item_info.ctrl_id = menu_ctrl_id;
    pop_item_info.menu_id = menu_id;
    pop_item_info.group_id = sort_group_id;

    GUIMENU_SetPopItemStatus (TRUE, &pop_item_info);
}


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle pda menu mark
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandlePDAMenuMark (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMI_CTRL_ID_T         ctrl_id,
    MMI_WIN_ID_T          win_id
)
{

    //SCI_TRACE_LOW:"MMIFMM: HandlePDAMenuMark."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13611_112_2_18_2_21_4_562, (uint8*) "");

    if (PNULL == list_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[HandlePDAMenuMark]PNULL == list_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13614_112_2_18_2_21_4_563, (uint8*) "");
        return;
    }

    if (0 == list_data_ptr->mark_num)
    {
        GUILIST_SetTextListMarkable (ctrl_id, TRUE);
        //for cr:NEWMS00128611,每次按下“标记”都重新设置text
        GUISOFTKEY_SetTextId (MMIFMM_UDISK_CHILD_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE);
        GUILIST_SetMaxSelectedItem (ctrl_id, (uint16) (list_data_ptr->file_num + list_data_ptr->folder_num));
    }

    GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    //GUIWIN_SetTitleButtonState(MMIFMM_UDISK_CHILD_WIN_ID, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
    GUILIST_SetAllSelected (ctrl_id, FALSE);
}






/*****************************************************************************/
//  Description : dea long ok menu
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void DealWithUdiskLongOkMenu (
    MMI_WIN_ID_T win_id,
    //MMIFMM_DATA_INFO_T *list_data_ptr,
    uint16 index
)
{
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
    uint32                  i = 0;
    uint32                  item_num = 0;
    uint16                  nodex_index = 0;
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    const GUILIST_ITEM_T*         item_ptr = PNULL;
    MMI_STRING_T            title_str = {0};
    uint32                  menu_ctrl_id = MMIFMM_FOLDER_OPT_CTRL_ID;
    uint8                   begin = 0;
#ifdef DRM_SUPPORT
    wchar                   full_path_name[MMIFMM_FILE_FILE_NAME_MAX_LEN] = {0};
#endif
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL == view_win_d
            || (PNULL == view_win_d->main_d_ptr->s_fmm_list_data_ptr 
            || PNULL == view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index])
       )
    {
        SCI_TRACE_LOW ("MMIFMM DealWithUdiskLongOkMenu PNULL == view_win_d!");
        return;
    }

    file_type = view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->type;
#ifdef DRM_SUPPORT

    if (MMIFMM_FILE_TYPE_DRM == file_type)
    {
        //需要根据DRM文件的媒体类型来选择定制的菜单选项
        MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
        DRM_PERMISSION_MODE_E drm_permission = DRM_PERMISSION_NONE;

        MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr, view_win_d->main_d_ptr->s_fmm_current_path_ptr, index, full_path_name, MMIFMM_FULL_FILENAME_LEN);

        file_handle = SFS_CreateFile (full_path_name, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);

        //如果此DRM文件有效且符合下列枚举类型，则依据原始类型显示菜单项
        if (SFS_INVALID_HANDLE != file_handle)
        {
            drm_permission = MMIAPIDRM_GetFilePermission (file_handle, PNULL);

            if (MMIAPIDRM_IsRightsValid (file_handle, PNULL, TRUE, drm_permission))
            {
                file_type = MMIAPIDRM_GetMediaFileType (file_handle, PNULL);
            }
        }

        SFS_CloseFile (file_handle);
    }

#endif

    if (0 < view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num) //NEWMS00124942
    {
        item_num = sizeof (s_fmm_marked_menu_root_item) / sizeof (*s_fmm_marked_menu_root_item);
        nodex_index = 0;
        menu_ctrl_id = MMIFMM_MARKED_OPT_CTRL_ID;
        MMK_CreateWin ( (uint32 *) MMIFMM_LONG_POPUP_OPT_MENU_WIN_TAB, (ADD_DATA) menu_ctrl_id);
        GUIMENU_CreatDynamic (PNULL, MMIFMM_LONG_POPUP_OPT_MENU_WIN_ID, MMIFMM_MARKED_OPT_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);

        if (IsMarkItemContainFolders())
        {
            begin = 1;
        }

        for (i = begin; i < item_num; i++)
        {
            MMI_GetLabelTextByLang (s_fmm_marked_menu_root_item[i][1], &kstring);
            node_item.item_text_ptr = &kstring;
            GUIMENU_InsertNode (nodex_index, s_fmm_marked_menu_root_item[i][0], 0, &node_item, MMIFMM_MARKED_OPT_CTRL_ID);
            nodex_index ++;
        }

        MMI_GetLabelTextByLang (TXT_PLS_SELECT_APPLICATION, &title_str);
        GUIMENU_SetMenuTitle (&title_str, menu_ctrl_id);
    }
    else
    {
        switch (file_type)
        {
        case MMIFMM_FILE_TYPE_PICTURE:
            item_num = sizeof (s_fmm_picture_menu_root_item) / sizeof (*s_fmm_picture_menu_root_item);
            nodex_index = 0;
            menu_ctrl_id = MMIFMM_PICTURE_OPT_CTRL_ID;
            MMK_CreateWin ( (uint32 *) MMIFMM_LONG_POPUP_OPT_MENU_WIN_TAB, (ADD_DATA) menu_ctrl_id);
            GUIMENU_CreatDynamic (PNULL, MMIFMM_LONG_POPUP_OPT_MENU_WIN_ID, MMIFMM_PICTURE_OPT_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);

            for (i = 0; i < item_num; i++)
            {
                if (!MMIFMM_isMenuAvailable (s_fmm_picture_menu_root_item[i][0]))
                {
                    continue;
                }

                MMI_GetLabelTextByLang (s_fmm_picture_menu_root_item[i][1], &kstring);
                node_item.item_text_ptr = &kstring;
                GUIMENU_InsertNode (nodex_index, s_fmm_picture_menu_root_item[i][0], 0, &node_item, MMIFMM_PICTURE_OPT_CTRL_ID);
                nodex_index ++;
            }

            item_ptr = GUILIST_GetItemPtrByIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID, index);/*lint !e605 */
            title_str = item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
            GUIMENU_SetMenuTitle (&title_str, MMIFMM_PICTURE_OPT_CTRL_ID);
            break;

        case MMIFMM_FILE_TYPE_MUSIC:
            item_num = sizeof (s_fmm_music_menu_root_item) / sizeof (*s_fmm_music_menu_root_item);
            nodex_index = 0;
            menu_ctrl_id = MMIFMM_MUSIC_OPT_CTRL_ID;
            MMK_CreateWin ( (uint32 *) MMIFMM_LONG_POPUP_OPT_MENU_WIN_TAB, (ADD_DATA) menu_ctrl_id);
            GUIMENU_CreatDynamic (PNULL, MMIFMM_LONG_POPUP_OPT_MENU_WIN_ID, MMIFMM_MUSIC_OPT_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);

            for (i = 0; i < item_num; i++)
            {
                if (!MMIFMM_isMenuAvailable (s_fmm_music_menu_root_item[i][0]))
                {
                    continue;
                }

                MMI_GetLabelTextByLang (s_fmm_music_menu_root_item[i][1], &kstring);
                node_item.item_text_ptr = &kstring;
                GUIMENU_InsertNode (nodex_index, s_fmm_music_menu_root_item[i][0], 0, &node_item, MMIFMM_MUSIC_OPT_CTRL_ID);
                nodex_index ++;
            }

            item_ptr = GUILIST_GetItemPtrByIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID, index);/*lint !e605 */
            title_str = item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
            GUIMENU_SetMenuTitle (&title_str, MMIFMM_MUSIC_OPT_CTRL_ID);
            break;

        case MMIFMM_FILE_TYPE_FOLDER:

            item_num = sizeof (s_fmm_folder_menu_root_item) / sizeof (*s_fmm_folder_menu_root_item);
            nodex_index = 0;
            menu_ctrl_id = MMIFMM_FOLDER_OPT_CTRL_ID;
            MMK_CreateWin ( (uint32 *) MMIFMM_LONG_POPUP_OPT_MENU_WIN_TAB, (ADD_DATA) menu_ctrl_id);
            GUIMENU_CreatDynamic (PNULL, MMIFMM_LONG_POPUP_OPT_MENU_WIN_ID, MMIFMM_FOLDER_OPT_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);

            for (i = 0; i < item_num; i++)
            {
                if (!MMIFMM_isMenuAvailable (s_fmm_folder_menu_root_item[i][0]))
                {
                    continue;
                }

                MMI_GetLabelTextByLang (s_fmm_folder_menu_root_item[i][1], &kstring);
                node_item.item_text_ptr = &kstring;
                GUIMENU_InsertNode (nodex_index, s_fmm_folder_menu_root_item[i][0], 0, &node_item, MMIFMM_FOLDER_OPT_CTRL_ID);
                nodex_index ++;
            }

            item_ptr = GUILIST_GetItemPtrByIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID, index);/*lint !e605 */
            title_str = item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
            GUIMENU_SetMenuTitle (&title_str, MMIFMM_FOLDER_OPT_CTRL_ID);


            break;

        case MMIFMM_FILE_TYPE_MOVIE:
            item_num = sizeof (s_fmm_movie_menu_root_item) / sizeof (*s_fmm_movie_menu_root_item);
            nodex_index = 0;
            menu_ctrl_id = MMIFMM_MOVIE_OPT_CTRL_ID;
            MMK_CreateWin ( (uint32 *) MMIFMM_LONG_POPUP_OPT_MENU_WIN_TAB, (ADD_DATA) menu_ctrl_id);
            GUIMENU_CreatDynamic (PNULL, MMIFMM_LONG_POPUP_OPT_MENU_WIN_ID, MMIFMM_MOVIE_OPT_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);

            for (i = 0; i < item_num; i++)
            {
                if (!MMIFMM_isMenuAvailable (s_fmm_movie_menu_root_item[i][0]))
                {
                    continue;
                }

                MMI_GetLabelTextByLang (s_fmm_movie_menu_root_item[i][1], &kstring);
                node_item.item_text_ptr = &kstring;
                GUIMENU_InsertNode (nodex_index, s_fmm_movie_menu_root_item[i][0], 0, &node_item, MMIFMM_MOVIE_OPT_CTRL_ID);
                nodex_index ++;
            }

            item_ptr = GUILIST_GetItemPtrByIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID, index);/*lint !e605 */
            title_str = item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
            GUIMENU_SetMenuTitle (&title_str, MMIFMM_MOVIE_OPT_CTRL_ID);
            break;
#if defined MMI_VCARD_SUPPORT
        case MMIFMM_FILE_TYPE_VCARD:
            item_num = sizeof (s_fmm_vcard_menu_root_item) / sizeof (*s_fmm_vcard_menu_root_item);
            nodex_index = 0;
            menu_ctrl_id = MMIFMM_VCARD_OPT_CTRL_ID;
            MMK_CreateWin ( (uint32 *) MMIFMM_LONG_POPUP_OPT_MENU_WIN_TAB, (ADD_DATA) menu_ctrl_id);
            GUIMENU_CreatDynamic (PNULL, MMIFMM_LONG_POPUP_OPT_MENU_WIN_ID, MMIFMM_VCARD_OPT_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);

            for (i = 0; i < item_num; i++)
            {
                if (!MMIFMM_isMenuAvailable (s_fmm_vcard_menu_root_item[i][0]))
                {
                    continue;
                }

                MMI_GetLabelTextByLang (s_fmm_vcard_menu_root_item[i][1], &kstring);
                node_item.item_text_ptr = &kstring;
                GUIMENU_InsertNode (nodex_index, s_fmm_vcard_menu_root_item[i][0], 0, &node_item, MMIFMM_VCARD_OPT_CTRL_ID);
                nodex_index ++;
            }

            item_ptr = GUILIST_GetItemPtrByIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID, index);/*lint !e605 */
            title_str = item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
            GUIMENU_SetMenuTitle (&title_str, MMIFMM_VCARD_OPT_CTRL_ID);
            break;
#endif
#ifdef MMI_VCALENDAR_SUPPORT
        case MMIFMM_FILE_TYPE_VCALENDAR:
            item_num = sizeof (s_fmm_vcalendar_menu_root_item) / sizeof (*s_fmm_vcalendar_menu_root_item);
            nodex_index = 0;
            menu_ctrl_id = MMIFMM_VCALENDAR_OPT_CTRL_ID;
            MMK_CreateWin ( (uint32 *) MMIFMM_LONG_POPUP_OPT_MENU_WIN_TAB, (ADD_DATA) menu_ctrl_id);
            GUIMENU_CreatDynamic (PNULL, MMIFMM_LONG_POPUP_OPT_MENU_WIN_ID, MMIFMM_VCALENDAR_OPT_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);

            for (i = 0; i < item_num; i++)
            {
                if (!MMIFMM_isMenuAvailable (s_fmm_vcalendar_menu_root_item[i][0]))
                {
                    continue;
                }

                MMI_GetLabelTextByLang (s_fmm_vcalendar_menu_root_item[i][1], &kstring);
                node_item.item_text_ptr = &kstring;
                GUIMENU_InsertNode (nodex_index, s_fmm_vcalendar_menu_root_item[i][0], 0, &node_item, MMIFMM_VCALENDAR_OPT_CTRL_ID);
                nodex_index ++;
            }

            item_ptr = GUILIST_GetItemPtrByIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID, index);/*lint !e605 */
            title_str = item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
            GUIMENU_SetMenuTitle (&title_str, MMIFMM_VCALENDAR_OPT_CTRL_ID);
            break;
#endif
#ifdef EBOOK_SUPPORT
        case MMIFMM_FILE_TYPE_EBOOK:
#endif
        case MMIFMM_FILE_TYPE_FONT:
        case MMIFMM_FILE_TYPE_SZIP:
        case MMIFMM_FILE_TYPE_BROWSER:
#ifdef BROWSER_SUPPORT
        case  MMIFMM_FILE_TYPE_URL:
#endif
            item_num = sizeof (s_fmm_other_menu_root_item) / sizeof (*s_fmm_other_menu_root_item);
            nodex_index = 0;
            menu_ctrl_id = MMIFMM_OTHER_OPT_CTRL_ID;
            MMK_CreateWin ( (uint32 *) MMIFMM_LONG_POPUP_OPT_MENU_WIN_TAB, (ADD_DATA) menu_ctrl_id);
            GUIMENU_CreatDynamic (PNULL, MMIFMM_LONG_POPUP_OPT_MENU_WIN_ID, MMIFMM_OTHER_OPT_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);

            for (i = 0; i < item_num; i++)
            {
                if (!MMIFMM_isMenuAvailable (s_fmm_other_menu_root_item[i][0]))
                {
                    continue;
                }

                MMI_GetLabelTextByLang (s_fmm_other_menu_root_item[i][1], &kstring);
                node_item.item_text_ptr = &kstring;
                GUIMENU_InsertNode (nodex_index, s_fmm_other_menu_root_item[i][0], 0, &node_item, MMIFMM_OTHER_OPT_CTRL_ID);
                nodex_index ++;
            }

            item_ptr = GUILIST_GetItemPtrByIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID, index);/*lint !e605 */
            title_str = item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
            GUIMENU_SetMenuTitle (&title_str, MMIFMM_OTHER_OPT_CTRL_ID);
            break;

        default:
            item_num = sizeof (s_fmm_file_menu_root_item) / sizeof (*s_fmm_file_menu_root_item);
            nodex_index = 0;
            menu_ctrl_id = MMIFMM_FILE_OPT_CTRL_ID;
            MMK_CreateWin ( (uint32 *) MMIFMM_LONG_POPUP_OPT_MENU_WIN_TAB, (ADD_DATA) menu_ctrl_id);
            GUIMENU_CreatDynamic (PNULL, MMIFMM_LONG_POPUP_OPT_MENU_WIN_ID, MMIFMM_FILE_OPT_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);

            for (i = 0; i < item_num; i++)
            {
                if (!MMIFMM_isMenuAvailable (s_fmm_file_menu_root_item[i][0]))
                {
                    continue;
                }

                MMI_GetLabelTextByLang (s_fmm_file_menu_root_item[i][1], &kstring);
                node_item.item_text_ptr = &kstring;
                GUIMENU_InsertNode (nodex_index, s_fmm_file_menu_root_item[i][0], 0, &node_item, MMIFMM_FILE_OPT_CTRL_ID);
                nodex_index ++;
            }

            item_ptr = GUILIST_GetItemPtrByIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID, index);/*lint !e605 */
            title_str = item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
            GUIMENU_SetMenuTitle (&title_str, MMIFMM_FILE_OPT_CTRL_ID);
            break;
        }
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: maryxiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIFMM_isMenuAvailable (uint32 menu_id)
{
    BOOLEAN ret = TRUE;

    if ( (menu_id == ID_FMM_DETAIL) || (menu_id == ID_FMM_RENAME))
    {
        if (GUILIST_GetSelectedItemIndex (MMIFMM_UDISK_LISTBOX_CTRL_ID, PNULL, 0) > 0)
        {
            ret = FALSE;
        }

    }

    return ret;
}

/**************************************************************************************************/
// Description:HandleFmmLongPopUpMsg
// Date:
// Author:
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleFmmLongPopUpMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E                result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id  = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        ctrl_id = (MMI_CTRL_ID_T) MMK_GetWinAddDataPtr (win_id);
        MMK_SetAtvCtrl (win_id, ctrl_id);
        break;

    case MSG_CTL_OK:
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMIPUB_MENU_NOTIFY_T param1 = {0};
            param1.win_id = win_id;
            param1.dst_handle = MMIFMM_UDISK_CHILD_WIN_ID;
            param1.group_id = 0;
            param1.menu_id = 0;//GUIMENU_GetCurNodeId(ctrl_id);
            param1.ctrl_id = (MMI_CTRL_ID_T) MMK_GetWinAddDataPtr (win_id);
            HandleMenuOption (&param1);
            MMK_CloseWin (win_id);
        }
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin (win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}
/*****************************************************************************/
//  Description : IsMarkItemContainFolders
//  Global resource dependence :
//  Author: juan.wu
//  Note:判断mark的item中是否含有文件夹,有，返回TRUE
/*****************************************************************************/
LOCAL BOOLEAN IsMarkItemContainFolders (void)
{
    uint16                  index = 0;
    BOOLEAN                 result = FALSE;

    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);

    if (PNULL == view_win_d || 0 == view_win_d->main_d_ptr->s_fmm_list_data_ptr->mark_num)
    {
        SCI_TRACE_LOW ("MMIFMM IsMarkItemContainFolders PNULL == view_win_d!");
        return result;
    }

    for (index = 0; index < (uint16) (view_win_d->main_d_ptr->s_fmm_list_data_ptr->file_num + view_win_d->main_d_ptr->s_fmm_list_data_ptr->folder_num); index++)
    {
        if (MMIFMM_FILE_STATE_UNCHECKED != view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->state)
        {
            if (MMIFMM_FILE_TYPE_FOLDER == view_win_d->main_d_ptr->s_fmm_list_data_ptr->data[index]->type)
            {
                result = TRUE;
                break;
            }
        }
    }

    return result;
}
#endif



/*****************************************************************************/
// Description : update file list
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_UpdateFmmList (uint32 device, BOOLEAN is_enter_root)
{
    BOOLEAN                 need_fullpiant = TRUE;
    uint32                  cur_tab_index = 0;
    MMIFILE_DEVICE_E        device_table = MMI_DEVICE_SDCARD;
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);
    //SCI_TRACE_LOW:"MMIAPIFMM_UpdateFmmList device = %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_12027_112_2_18_2_21_0_539, (uint8*) "d", device);

    if (PNULL == view_win_d)
    {
        SCI_TRACE_LOW ("MMIFMM MMIFMM_UpdateFmmList PNULL == view_win_d!");
        return;
    }

    MMK_CloseWin (MMIFMM_PICTURE_PREVIEW_WIN_ID);
    MMK_CloseWin (MMIFMM_PIC_PREVIEW_OPT_MENU_WIN_ID);

    if (MMK_IsOpenWin (MMIFMM_UDISK_CHILD_WIN_ID))
    {
        cur_tab_index = GUITAB_GetCurSel (MMIFMM_TAB_CTRL_ID);

        if (cur_tab_index < MMI_DEVICE_NUM)
        {
            device_table = view_win_d->main_d_ptr->s_file_path_info_ptr->s_device_table[cur_tab_index];
        }

        if (device_table == (MMIFILE_DEVICE_E) device)
        {
            FmmUpdateValidPath (MMIFMM_UDISK_CHILD_WIN_ID, is_enter_root);
            MMK_SendMsg (MMIFMM_UDISK_CHILD_WIN_ID, MSG_FMM_UPDATE_LIST_DATA, (ADD_DATA) need_fullpiant);
        }
    }
}



/*****************************************************************************/
//  Description : get file name
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIFMM_GetPicName (
    uint16  item_index,     //in:
    uint16  *name_len_ptr   //in/out:
)
{
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);

    if (PNULL == view_win_d || PNULL == name_len_ptr)
    {
        SCI_TRACE_LOW ("MMIFMM MMIFMM_GetPicName PNULL == view_win_d!");
        return PNULL;
    }

    *name_len_ptr = (uint16) MMIFMM_CombineFullFileName (view_win_d->main_d_ptr->s_fmm_list_data_ptr,
                    view_win_d->main_d_ptr->s_fmm_current_path_ptr,
                    item_index,
                    view_win_d->s_list_file_name_ptr,
                    MMIFMM_FULL_FILENAME_LEN);

    return (view_win_d->s_list_file_name_ptr);
}



/*****************************************************************************/
//  Description :MMIFMM_GetPicIndexData
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_PICTURE_INDEX_DATA_T  ** MMIFMM_GetPicIndexData (void)
{
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);

    if (PNULL != view_win_d)
    {
        return &view_win_d->s_pic_index_data_ptr;
    }
    else
    {
        return PNULL;
    }
}

/**************************************************************************************************/
// Description:import vcard call back function
// Date:
// Author:
// Note:
//*************************************************************************************************/
LOCAL void  Importvcard_Callback (MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIAPIPB_CloseHandle (handle);

    if (MMIPB_ERROR_NO_SPACE == error)
    {
        MMIPUB_OpenAlertWinByTextId (PNULL, TXT_NO_SPACE, TXT_NULL, IMAGE_PUBWIN_FAIL, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
    }
    else if (MMIPB_ERROR_SUCCESS != error
             && MMIPB_ERROR_NO_ENTRY != error)
    {
        //import fail
        MMIPUB_OpenAlertWinByTextId (PNULL, TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_FAIL, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
    }

    MMK_CloseWin (MMIFMM_FILE_WAITING_WIN_ID);
}



/*****************************************************************************/
//  Description : wait move window message
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImportVcardWaitWinMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        // 不处理
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin (win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMIAPIPB_AbortPB();
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    }

    return (result);
}


#ifdef MMI_SYNCTOOL_SUPPORT
/*****************************************************************************/
//  Description : get current path
//  Global resource dependence :
//  Author: jian.ma
//  Note: only for auto test
/*****************************************************************************/
PUBLIC MMIFMM_PATH_INFO_T *MMIFMM_GetCurrentPath (void)
{
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);

    if (PNULL != view_win_d)
    {
        return  view_win_d->main_d_ptr->s_fmm_current_path_ptr;
    }
    else
    {
        return PNULL;
    }
}
#endif

/*****************************************************************************/
//  Description :if is FmmMainWinOpen
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsFmmMainWinOpen (void)
{
    return MMK_IsOpenWin (MMIFMM_MAIN_WIN_ID);
}


/*****************************************************************************/
//  Description : get display file type
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIFMM_GetDisplayFileType (void)
{
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);

    if (PNULL == view_win_d)
    {
        return 0;
    }

    return view_win_d->main_d_ptr->s_file_path_info_ptr->s_suffix_type;
}


/*****************************************************************************/
//  Description : get select tartget path
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_OpenTartgetWin (MMIFMM_OpenFileCallBack callback)
{

    MMIFMM_FILTER_T           filter       = {0};
    FILEARRAY                 ret_array    = MMIAPIFILEARRAY_Create();
    wchar                     root_folder[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    uint16                    device_len = 0;
    MMIFMM_OPEN_LAYER_WIN_INFO_T open_layer_info = {0};
    MMIFMM_VIEW_WIN_DATA_T   *view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);

    if (PNULL != view_win_d && view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev < MMI_DEVICE_NUM) //jian
    {
        SCI_MEMCPY (filter.filter_str, "*", sizeof ("*"));
        if(view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev < MMI_DEVICE_UDISK)//cid609708
        {
            return;
        }
        device_len = MMIAPIFMM_GetDevicePathLen (view_win_d->main_d_ptr->s_file_path_info_ptr->s_device_table[view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev - MMI_DEVICE_UDISK]);/*lint !e656 */ //jian
        MMIAPICOM_Wstrncpy (root_folder, MMIAPIFMM_GetDevicePath (view_win_d->main_d_ptr->s_file_path_info_ptr->s_cur_dev), device_len);//jian
        root_folder[device_len] = MMIFILE_COLON;
        open_layer_info.path_ptr = root_folder;
        open_layer_info.path_len = MMIAPICOM_Wstrlen (root_folder);
        open_layer_info.filter_ptr = &filter;
        open_layer_info.func_type = FUNC_FIND_FOLDER;
        open_layer_info.is_multi_sel = FALSE;
        open_layer_info.handle_func = PNULL;
        open_layer_info.ret_array = ret_array;
        open_layer_info.callback = callback;
        open_layer_info.is_internal_call = TRUE;
        open_layer_info.is_sel_folder = TRUE;


//        if(FMM_OPENWIN_FILE_NOT_EXIST == MMIFMM_OpenFileWinByLayerInternal(root_folder,
//            MMIAPICOM_Wstrlen(root_folder),   &filter, FUNC_FIND_FOLDER, FALSE, PNULL,
//             ret_array, callback, TRUE, TRUE))
        if (FMM_OPENWIN_FILE_NOT_EXIST == MMIAPIFMM_OpenFileWinByLayerEx (&open_layer_info))
        {
            MMIAPIFILEARRAY_Destroy (&ret_array);
        }
    }
}

/*****************************************************************************/
// Description : 是否选中的是同一种文件类型文件
// Global resource dependence :
// Author: renyi.hu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSelectedSameFileType (MMIFMM_DATA_INFO_T    *list_data_ptr,
                                      MMIFMM_FILE_TYPE_E     type
                                     )
{
    int i = 0;
    int num = list_data_ptr->file_num + list_data_ptr->folder_num;
    BOOLEAN result = FALSE;

    for (i = 0; i < num; i++)
    {
        if ( (MMIFMM_FILE_STATE_CHECKED == list_data_ptr->data[i]->state))
        {
            if (type == list_data_ptr->data[i]->type)
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;
                break;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : set tab title
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SetWinTitle (BOOLEAN is_normal_type,
                                MMI_WIN_ID_T win_id,
                                MMI_CTRL_ID_T tab_ctrl_id,
                                wchar * current_path,
                                uint16  current_path_len)
{
    MMIFMM_TAB_TYPE_E type = MMIFMM_NORMAL_TAB_TYPE;
    MMIFMM_PATH_INFO_T current_path_info = {0};

    if (0 == current_path_len)
    {
        current_path_info.pathname_len = MMIAPICOM_Wstrlen (current_path);
    }
    else
    {
        current_path_info.pathname_len = current_path_len;
    }

    MMIAPICOM_Wstrncpy (current_path_info.pathname, current_path, current_path_info.pathname_len);
    current_path_info.path_depth = MMIFMM_GetPathDepth (current_path_info.pathname, current_path_info.pathname_len);

    if (!is_normal_type)
    {
        type = MMIFMM_OTHER_TAB_TYPE;
    }

    SetWinTitle (type, win_id, 0, tab_ctrl_id, &current_path_info);
}

/*****************************************************************************/
//  Description : set win title
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetWinTitle (
    MMIFMM_TAB_TYPE_E    type,
    MMI_WIN_ID_T         win_id,
    MMI_CTRL_ID_T        ctrl_id,
    MMI_CTRL_ID_T        tab_ctrl_id,
    MMIFMM_PATH_INFO_T   *current_path_ptr
)
{
    MMI_STRING_T        title_string = {0};
    //wchar               title1[MMIFMM_WIN_TITLE_MAX_LEN + 1] = {0};
	wchar               *title1_ptr = PNULL;   
    //uint16              title_len = 0;
    //MMI_CTRL_ID_T       list_ctrl_id = MMIFMM_UDISK_LISTBOX_CTRL_ID;
    MMIFILE_DEVICE_E    device = MMI_DEVICE_UDISK;
    uint32              base_index = 0;

    char                get_label_name[MMIFILE_NEW_DEVICE_NAME_MAX_LEN*3+1] = {0};
    wchar               buffer_name[MMIFMM_FULL_FILENAME_LEN+1] = {0};
    BOOLEAN             get_label_res = FALSE;

    //SCI_ASSERT(PNULL != current_path_ptr);
    if (PNULL == current_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM: [SetWinTitle] PNULL == current_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10110_112_2_18_2_20_56_503, (uint8*) "");
        return;
    }
    if(0 == current_path_ptr->pathname_len)
    {
        SCI_TRACE_LOW("[mmifmm_wintab.c]SetWinTitle:current_path_ptr->pathname_len = 0");
        return;
    }
    //coverity 12704
    //SCI_TRACE_LOW:"MMIFMM: SetWinTitle, win_id = %d current_path_ptr->path_depth = %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10106_112_2_18_2_20_56_502, (uint8*) "dd", win_id, current_path_ptr->path_depth);
    /*
    if (MMIFMM_OTHER_TAB_TYPE == type)
    {
        list_ctrl_id = MMIFMM_PATH_UDISK_LISTBOX_CTRL_ID;
    }*/
    title1_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN + 1));
    if(PNULL == title1_ptr)//判断是否申请成功
    {
        SCI_TRACE_LOW("[mmifmm_wintab.c] SetWinTitle PNULL == title1_ptr");
        return;
    }

    device = MMIAPIFMM_GetDeviceTypeByPath (current_path_ptr->pathname, 1);
    //SCI_TRACE_LOW:"MMIFMM: [SetWinTitle] device = %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10118_112_2_18_2_20_56_504, (uint8*) "d", device);

    //set current tab title
    if (1 >= current_path_ptr->path_depth)
    {
        //root path, set device name as title
        //MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(device), &title_string);
        if (MMI_DEVICE_UDISK < device && device < MMI_DEVICE_NUM)
        {
            get_label_res = MMIAPIFMM_GetDeviceLabel (MMIAPIFMM_GetDevicePath (device), get_label_name);
        }

        if (get_label_res)
        {
            SCI_MEMSET (buffer_name, 0x00, (sizeof (wchar) * (MMIFMM_FULL_FILENAME_LEN  + 1)));
			//CR161453
			GUI_OtherCodeToWstr(buffer_name,MMIFMM_FULL_FILENAME_LEN,GUI_GetCodeType(get_label_name,strlen (get_label_name)),(uint8 *)get_label_name,strlen(get_label_name));
            //GUI_UTF8ToWstr (buffer_name, MMIFILE_NEW_DEVICE_NAME_MAX_LEN, (uint8 *) get_label_name, strlen (get_label_name));
            //GUI_OtherCodeToWstr( buffer_name,MMIFILE_NEW_DEVICE_NAME_MAX_LEN, GUI_CHAR_CODE_ANSI,  (uint8 *)get_label_name,strlen(get_label_name));
            title_string.wstr_ptr = buffer_name;
            title_string.wstr_len = MIN(MMIAPICOM_Wstrlen (buffer_name),MMIFILE_NEW_DEVICE_NAME_MAX_LEN);
        }
        else
        {
            MMI_GetLabelTextByLang (MMIAPIFMM_GetDeviceName (device), &title_string);
        }
    }
    else
    {

        //not root path, set path name as title
        if (current_path_ptr->pathname_len <= MMIFMM_WIN_TITLE_MAX_LEN)
        {

            title_string.wstr_ptr = current_path_ptr->pathname;
            title_string.wstr_len = current_path_ptr->pathname_len;
        }
        else
        {
			MMIFMM_ComputTitleShow(tab_ctrl_id,&title_string,current_path_ptr->pathname,title1_ptr);
#if 0
            //CR NEWMS00213280
            MMIAPICOM_Wstrncpy (title1, MMIFMM_ELLIPSIS, MMIFMM_ELLIPSIS_LEN);
            i = current_path_ptr->pathname_len + MMIFMM_ELLIPSIS_LEN - MMIFMM_WIN_TITLE_MAX_LEN;
            title_len = MMIFMM_WIN_TITLE_MAX_LEN - MMIFMM_ELLIPSIS_LEN;

            MMIAPICOM_Wstrncpy (&title1[MMIFMM_ELLIPSIS_LEN], &current_path_ptr->pathname[i],  title_len);

            title_string.wstr_ptr = title1;
            title_string.wstr_len = (uint16) (title_len + MMIFMM_ELLIPSIS_LEN);
#endif
        }
    }

    base_index = MMI_DEVICE_UDISK;
#ifdef MMI_PDA_SUPPORT
    {
        MMI_IMAGE_ID_T *focustab_icon_ptr = PNULL;
        MMI_IMAGE_ID_T *releasetab_icon_ptr = PNULL;
#if defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())
        {
            focustab_icon_ptr = g_s_focustab_istyle_icon;
            releasetab_icon_ptr = g_s_releasetab_istyle_icon;
        }
        else
#endif			
        {
            focustab_icon_ptr = g_s_focustab_icon;
            releasetab_icon_ptr = g_s_releasetab_icon;
        }

        GUITAB_SetSimpleItem (tab_ctrl_id,
                              &title_string,
                              focustab_icon_ptr[device - base_index],
                              releasetab_icon_ptr[device - base_index],
                              GUITAB_GetCurSel (tab_ctrl_id));
    }
#else
    GUITAB_SetSimpleItem (tab_ctrl_id,
                          &title_string,
                          IMAGE_FMM_UDISK_FOCUSED + device - base_index,
                          IMAGE_FMM_UDISK_RELEASED + device - base_index,
                          GUITAB_GetCurSel (tab_ctrl_id));
#endif
    if (PNULL != title1_ptr) /*lint !e774*/
    {
        SCI_Free(title1_ptr);
        title1_ptr = PNULL;
    }
}
#ifdef  VIDEOTHUMBNAIL_SUPPORT
/*****************************************************************************/
//  Description : VIDEO文件夹支持缩略图功能实时刷新 ICONLIST列表
//  Global resource dependence :
//  Author:liyan.zhu
//  Note:ForCR139456 重构产生的后遗症
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumbnailUpdateIcon(void)
{
	MMIFMM_VIEW_WIN_DATA_T* view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_UDISK_CHILD_WIN_ID);
	FmmVideoThumbnailUpdateIcon(view_win_d->main_d_ptr->s_fmm_list_data_ptr,view_win_d->main_d_ptr->s_fmm_current_path_ptr);  
}
#endif

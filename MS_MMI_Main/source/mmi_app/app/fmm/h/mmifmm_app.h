/*****************************************************************************
** File Name:      mmifmm_app.h                                              *
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

#ifndef _MMIFMM_APP_H_
#define _MMIFMM_APP_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
#if defined MMI_VCARD_SUPPORT
#include "mmivcard_export.h"
#endif
#include "mmifmm_interface.h"
#include "mmivcalendar.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIFMM_FILE_SUFFIX_MAX_LEN          7

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : preview current video file
// Global resource dependence :
// Author:  murphy.xie
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_PreviewVideo (wchar *full_path_name_ptr);

/*****************************************************************************/
//  Description : open search pic preview win
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SearchOpenPicturePreviewWin (
    uint32 index,
    FILEARRAY   file_arra
);

/*****************************************************************************/
//  Description : get file sort type
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_SORT_TYPE_E MMIFMM_GeFileSortType (void);

/*****************************************************************************/
//  Description : set file sort type
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SetFileSortType (MMIFMM_SORT_TYPE_E sort_type);


/*****************************************************************************/
//  Description : get select tartget path
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_GetSelectTargetPath (MMIFMM_OpenFileCallBack callback);

/*****************************************************************************/
// Description : memory detail
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmMemoryDetail (uint32 device);

/*****************************************************************************/
//  Description : show txt context
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  void  MMIFMM_ShowTxtContent (wchar *full_path_name_ptr);

/*****************************************************************************/
//  Description : local open fmm  main win
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_OpenFmmMainExplorer (
    const wchar * path_ptr,
    uint16   path_len,
    const wchar * filename,
    uint16   file_len,
    BOOLEAN is_need_display,
    BOOLEAN is_protect_checked,
    uint32  find_suffix_type
);

/*****************************************************************************/
// 	Description : indication to other module that file has updated
//	Global resource dependence : none
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMIFMM_UpdateEventInd (void);

/*****************************************************************************/
//  Description : play music file
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmPlayMusicFile (
    MMIFMM_DATA_INFO_T      *list_data_ptr,
    MMIFMM_PATH_INFO_T      * current_path_ptr,
    uint16                               cur_index,
    uint16				        total_num
);

/*****************************************************************************/
//  Description : play movie file
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmPlayMovieFile (
    MMIFMM_DATA_INFO_T      *list_data_ptr,
    MMIFMM_PATH_INFO_T      * current_path_ptr,
    uint16              index
);

#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
/*****************************************************************************/
//  Description : open szip file
//  Global resource dependence :
//  Author: james.zhang
//  Note: 2007-6-12
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenFont (
    MMIFMM_DATA_INFO_T               *list_data_ptr,
    MMIFMM_PATH_INFO_T               * current_path_ptr,
    uint16     					 index
);
#endif



#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
//  Description : set szip name
//  Global resource dependence :
//  Author: james.zhang
//  Note: 2007-6-12
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenSzipFile (
    MMIFMM_DATA_INFO_T               *list_data_ptr,
    MMIFMM_PATH_INFO_T               * current_path_ptr,
    uint16     					 index
);
#endif

#if defined MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : open vcard file
//  Global resource dependence :
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC VCARD_ERROR_E MMIFMM_GetVcardFileData (
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMIFMM_PATH_INFO_T   *current_path_ptr,
    uint16     					 index);
#endif
#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIFMM_GetVcalendarFileData (
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMIFMM_PATH_INFO_T   *current_path_ptr,
    uint16     					 index);
#endif

/*****************************************************************************/
// Description : 在给定路径下创建目录，需要输入目录名字
// Global resource dependence :
// Author:    Ming.Song 2007-4-17
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmCreateFolder (
    wchar  *full_path_ptr,      // 全路径
    wchar  *new_folder_ptr,     // 新文件夹
    uint16  *new_folder_len_ptr, // 新文件夹的长度
    uint32  win_id,              // 创建文件夹后通知的窗口
    uint32  msg_id               // 通知的消息
);

/*****************************************************************************/
//  Description : sort
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmFileDataSort (
    uint32  win_id,
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMIFMM_SORT_TYPE_E     sort
);

/*****************************************************************************/
//  Description : open vcard file
//  Global resource dependence :
//  Author: ming.song
//  Note: 2007-6-12
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenVcardFile (
    MMIFMM_DATA_INFO_T               *list_data_ptr,
    MMIFMM_PATH_INFO_T               * current_path_ptr,
    uint16     					 index
);

#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
//  Description : open vcalendar file
//  Global resource dependence :
//  Author: ming.song
//  Note: 2007-6-12
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenVcalendarFile (
    MMIFMM_DATA_INFO_T               *list_data_ptr,
    MMIFMM_PATH_INFO_T               * current_path_ptr,
    uint16     					 index
);
#endif
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
//  Description : open szip file
//  Global resource dependence :
//  Author: james.zhang
//  Note: 2007-6-12
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenSzipFile (
    MMIFMM_DATA_INFO_T               *list_data_ptr,
    MMIFMM_PATH_INFO_T               * current_path_ptr,
    uint16     					 index
);
#endif
#ifdef BROWSER_SUPPORT_NETFRONT
/*****************************************************************************/
//  Description : open www file
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenWWWFile (
    MMIFMM_DATA_INFO_T               *list_data_ptr,
    MMIFMM_PATH_INFO_T               * current_path_ptr,
    uint16     					 index
);
#endif

#ifdef KURO_SUPPORT
/*****************************************************************************/
//  Description : play kur music file
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmPlaykurFile (
    MMIFMM_DATA_INFO_T      *list_data_ptr,
    MMIFMM_PATH_INFO_T      * current_path_ptr,
    uint16     					 index
);
#endif
#if defined (BLUETOOTH_SUPPORT)
/*****************************************************************************/
//  Description : BT send files
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmBtSendFile (
    MMIFMM_DATA_INFO_T     *list_data_ptr,
    MMIFMM_PATH_INFO_T     *current_path_ptr,
    uint16     					 index
);
#endif
/*****************************************************************************/
//  Description : 详情
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmFileDetail (
    uint16     					 index ,
    MMIFMM_DATA_INFO_T     *list_data_ptr
);
/*****************************************************************************/
//  Description : 重命名
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmRenameFile (
    uint16     					 index ,
    MMI_WIN_ID_T           win_id,
    MMI_MESSAGE_ID_E       msg_id,
    MMIFMM_DATA_INFO_T     *list_data_ptr,
    wchar*                 full_file_name_ptr
);

#if defined (MMS_SUPPORT)
/*****************************************************************************/
//  Description : send MMS
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSendMMS (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMIFMM_PATH_INFO_T    *current_path_ptr,
    uint16     					 index
);
#endif
#ifdef MMI_FMM_SEARCH_SUPPORT
/*****************************************************************************/
// Description : 在给定路径下查找文件，需要输入文件名字
// Global resource dependence :
// Author:    Ming.Song 2007-4-12
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenFindFileWin (
    wchar                *find_path_ptr,
    MMIFMM_DATA_INFO_T    *result_data_ptr,
    uint16                max_result_file_num,
    uint32                win_id,
    uint32                msg_id
);
#endif
#ifdef MMI_AUDIO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : set call ring
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetCallRing (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMIFMM_PATH_INFO_T    *current_path_ptr,
    uint16     					 index ,
    MN_DUAL_SYS_E         dual_sys
#ifndef MMI_PDA_SUPPORT
	,
	MMIFMM_SET_RING_TYPE_E   ring_type
#endif
);
#endif




#ifdef SCREENSAVER_SUPPORT
/*****************************************************************************/
//  Description : Set as screen saver
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetAsScreenSaver (
    uint16     					 index ,
    MMIFMM_DATA_INFO_T     *list_data_ptr
);

#endif

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
/*****************************************************************************/
//  Description : Set as screen saver
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetAsVideoWallpaper (
    uint16     					 index ,
    MMIFMM_DATA_INFO_T     *list_data_ptr
);
#endif

/*****************************************************************************/
//  Description : FmmSetAsPowerOnOffAnim
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetAsPowerOnOffAnim (
    uint16     					 index ,
    MMIFMM_DATA_INFO_T     *list_data_ptr,
    BOOLEAN     is_power_on_off     //TRUE: power on, FALSE: power off
);


/*****************************************************************************/
//  Description : append tab ctrl
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_AppendTabCtrl(
                                MMI_CTRL_ID_T ctrl_id,
                                uint32 base_index,
                                MMIFILE_DEVICE_E fmm_dev);

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : set as pb contact icon
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetPB (
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMIFMM_PATH_INFO_T   *current_path_ptr,
    uint16     					 index
);
#endif
/*****************************************************************************/
//  Description : set wallpaper
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmSetWallpaper (
    MMIFMM_DATA_INFO_T   *list_data_ptr,
    MMIFMM_PATH_INFO_T   *current_path_ptr,
    uint16     					 index
);

/*****************************************************************************/
//  Description : copy file menu function
//  Global resource dependence :
//  Author: xinrui.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_CopySelectFileToFolder (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    uint16     					 index ,
    MMIFMM_PATH_INFO_T*   current_path_ptr
);

/*****************************************************************************/
//  Description : 移动文件或目录
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_MoveSlectFileToFolder (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    uint16     					 index ,
    MMIFMM_PATH_INFO_T    *current_path_ptr
);


/*****************************************************************************/
//  Description : 读当前目录中的文件数据，
//  Global resource dependence :
//  Author: robert.lu
//  Note: 文件保存，没有实现
/*****************************************************************************/
PUBLIC uint16 MMIFMM_GetCurrentPathFile (// 所读文件和目录的总数
    const wchar          *full_path_ptr,     // 全路径,查找条件
    MMIFMM_DATA_INFO_T   *data_ptr         // 输出数据
);


/*****************************************************************************/
//  Description : get icon id by file name
//  Global resource dependence :
//  Author: ying.xu
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIFMM_GetIconIdByFileName (
    const wchar        *file_name,
    uint16     file_name_len
);


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 通过给定的字符，查出当前支持的后缀类型
/*****************************************************************************/
PUBLIC void MMIFMM_GetSuffixTypeInfo (char *filter_str, uint32 *suffix_type_ptr);

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
                                uint16  current_path_len);

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : open url file by browser
//  Global resource dependence :
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmOpenUrlFile(
                MMIFMM_DATA_INFO_T  *list_data_ptr,
                MMIFMM_PATH_INFO_T  * current_path_ptr,
                uint16  index
                );
#endif

#endif

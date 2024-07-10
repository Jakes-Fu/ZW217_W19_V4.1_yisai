/*****************************************************************************
** File Name:      mmifmm_export.h                                                  *
** Author:                                                                   *
** Date:           06/04/2007                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:     file manager modue                                                                              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2007       Robert           Create
******************************************************************************/
/*******************************************************************************
** Full Path Format: 
**      full path = device + ':\' + directory + '\' + filename
**      eg:  "D:\picture\abc.jpg"
**      "D" is device name of Udisk, SD card is represented by "E"
**      "picture" is directory name
**      "abc.jpg" is filename
**      use ":", "\" to seperate device, directory, filename (don't use '/')
**
** 注:
** 1. 文件名长度、文件夹名长度、设备名长度、全路径名长度都以unicode个数
******************************************************************************/

#ifndef _MMIFMM_EXPORT_H_
#define _MMIFMM_EXPORT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sfs.h"
#include "mmk_type.h"
#include "guianim.h"
#include "mime_type.h"
//#include "mmi_filetask.h"
#include "mmi_custom_define.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_nv.h"
#include "ffs.h"
#if defined(DRM_SUPPORT)
#include "drm_api.h"
#include "mmifmm_drm.h"
#endif
#include "mmi_filemgr.h"
#include "mmifilearray_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmifmmsrv_api.h"
#include "mmifmm.h"
#include "mmifmm_interface.h"
#include "mmifmm_lsearch.h"
#include "mmifmm_srv.h"
#include "mmisrvfmm_export.h"

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
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
#ifdef DRM_SUPPORT
typedef struct
{
    uint16 cons_mode_limit;             //受限的消费模式如count, interval, time
    uint16 method_level_limit;          //受限的DRM文件类型如fl, cd, sd
    BOOLEAN is_rights_valid;            //是否只可选版权有效的文件    
}MMIFMM_DRM_LIMIT_TYPE_T;               //在做查找文件时的最后选择时，设置应用限制的类型
#endif


/**--------------------------------------------------------------------------*
**                         GLOBAL VALUE DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : initialize FMM module
//  Global resource dependence : none
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitModule(void);

/*****************************************************************************/
//  Description : open FMM
//  Global resource dependence : none
//  Author: Robert.Lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenExplorer(void);

/*****************************************************************************/
//  Description : open any fmm win
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenExplorerExt(
                                      const wchar* path_ptr,
                                      uint16   path_len,
                                      const wchar * filename,
                                      uint16   file_len,
                                      BOOLEAN is_need_display,
                                      uint32  find_suffix_type
                                      );

/*****************************************************************************/
//  Description : open any fmm win 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenExplorerWithoutProtect(
                                      const wchar * path_ptr,
                                      uint16   path_len,
                                      const wchar * filename,
                                      uint16   file_len,
                                      BOOLEAN is_need_display,
                                      uint32  find_suffix_type
                                      );

/*****************************************************************************/
//  Description : open any fmm win with callback function
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenExplorerWin(MMIFMM_OPENWIN_INFO_T * openwin_info);

/*****************************************************************************/
//  Description : handle menu option function
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_HandleMenuOpt(MMIFMM_OPTION_TYPE_E type);


/*****************************************************************************/
//  Description : 读当前目录中的文件数据，
//  Global resource dependence :
//  Author: robert.lu
//  Note: 文件保存，没有实现
/*****************************************************************************/
PUBLIC uint16 MMIAPIFMM_GetCurrentPathFile(// 所读文件和目录的总数
                                           const wchar          *full_path_ptr,     // 全路径,查找条件
                                           //MMIFMM_READ_TYPE_E   type,               // 类型
                                           MMIFMM_DATA_INFO_T   *data_ptr         // 输出数据
                                           //uint16               *output_filename_ptr// 输出数据的文件名
                                           );
/*****************************************************************************/
//  Description : get file type
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFMM_GetFileType(
                                                const wchar      *suffix_name_ptr,
                                                uint16      suffix_name_len
                                                );

/*****************************************************************************/
//  Description : convert file type to fmm file type
//  Global resource dependence : 
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIAPIFMM_ConvertFileType(
                                                const wchar      *suffix_name_ptr,
                                                uint16      suffix_name_len
                                                );

/*****************************************************************************/
//  Description : convert file type to movie type
//  Global resource dependence : 
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC MMICOM_VIDEO_TYPE_E MMIAPIFMM_ConvertMovieFileType(
                                                 const wchar    *suffix_name_ptr,
                                                  uint16    suffix_name_len
                                                  );
/*****************************************************************************/
//  Description : convert file type to music type
//  Global resource dependence : 
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIFMM_ConvertMusicFileType(
                                                  const wchar    *suffix_name_ptr,
                                                  uint16    suffix_name_len
                                                  );

/*****************************************************************************/
//  Description : get the type of folder
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FOLDER_TYPE_E MMIAPIFMM_GetFolderType(
                                                    const wchar      *path_ptr,
                                                    uint16      path_len,
                                                    BOOLEAN     is_include_path
                                                    );

/*****************************************************************************/
//  Description : sort
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_FileDataSort(
                                  MMIFMM_DATA_INFO_T    *list_data_ptr,
                                  MMIFMM_SORT_TYPE_E    sort
                                  );

/*****************************************************************************/
//  Description : stop current operation when sd plug
//  Global resource dependence : 
//  Author:robert.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_StopOperaSDPlug(void);

                                       
/*****************************************************************************/
//  Description : handle copy task callback
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CopyCallBack(
                         uint16     result
                         );

/*****************************************************************************/
//  Description : handle copy task callback
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CopyFileExistInform(void);

/*****************************************************************************/
//  Description : set preview picture anim control param
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetPreviewPicParam(
                                     BOOLEAN        is_update,          //in:是否立即刷新
                                     wchar          *full_path_wstr_ptr,//in
                                     uint16         full_path_wstr_len, //in:字节数
                                     MMI_CTRL_ID_T  ctrl_id
                                     );

/*****************************************************************************/
//  Description : 停止复制/删除等文件操作
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_ExitFileOption(void);

/*****************************************************************************/
//  Description :是否在文件复制/删除操作
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsFileOption(void);

/*****************************************************************************/
//  Description : 更新当前的文件列表
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_UpdateCurrentFileLIst(void);

/*****************************************************************************/
//  Description : get dev info and check if have enough space
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsDevEnoughSpace(uint32 size,MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : get pic data for pic detail info
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetPictureWidthHeight(
                                        const wchar       *full_path_ptr,
                                        const uint16      full_path_len,
                                        uint32            file_size,
                                        uint16            *width_ptr,
                                        uint16            *height_ptr
                                        );
/*****************************************************************************/
//  Description : initialize Multim
//  Global resource dependence : none
//  Author: Liqing.Peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitFolders(void);

/*****************************************************************************/
//  Description : initialize NV about multim wallpaper
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitWallPaper(void);

/*****************************************************************************/
//  Description : 设置图片为墙纸
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetPictureToWallpaper(
                                           BOOLEAN      is_pre_wallpaper,   //是否是恢复前一张墙纸
                                           const wchar  *full_path_ptr,
                                           uint16       full_path_len
                                           );

/*****************************************************************************/
//  Description : 设置图片为墙纸,如果有图片裁剪，并格式支持，则先进入图片裁剪
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetPictureToWallpaperEx(
                                           BOOLEAN      is_pre_wallpaper,   //是否是恢复前一张墙纸
                                           const wchar  *full_path_ptr,
                                           uint16       full_path_len
                                           );
/*****************************************************************************/
//  Description : 设置图片为开关机动画/屏保?如果有图片裁剪，并格式支持，则先进入图片裁剪
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note: copy MMIAPIFMM_SetPictureToWallpaperEx
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetPictureToPowerAndSaverEx(
													 //BOOLEAN      is_pre_wallpaper,   //是否是恢复前一张墙纸
													 const wchar  *full_path_ptr,
													 uint16       full_path_len
													 );
/*****************************************************************************/
//  Description : FmmSetAsPowerOnOffAnim
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetAsPowerOnOffAnim (
                                            BOOLEAN     is_power_on_off,
                                            const wchar  *full_path_ptr,
                                            uint16       full_path_len
                                            );

/*****************************************************************************/
//  Description : set win title
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetFileNameWinTitle(
                                  MMI_WIN_ID_T    win_id, 
                                  const wchar     *file_name_ptr,
                                  uint16          file_name_len,
                                  BOOLEAN         is_refresh
                                  );
#ifdef MMI_AUDIO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : set music file to ring
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetMusicToRing(
                                MMIFMM_SET_RING_TYPE_E   ring_type,
                                DPARAM                      param,
                                uint16                      *full_name_ptr,
                                uint16                      full_name_len
                                );
#endif
/*****************************************************************************/
//  Description : is file wallpaper file
//  Global resource dependence : none
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsWallPaperFile(
                                  const wchar *full_path_ptr,
                                  uint16    full_path_len
                                  );

/*****************************************************************************/
//  Description : change wallpaper file name
//  Global resource dependence : none
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_ChangeWallPaperFileName(
                                      const wchar *full_path_ptr,
                                      uint16    full_path_len
                                      );

/*****************************************************************************/
//  Description : write wallpaper file info NV
//  Global resource dependence : none
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_WriteWallPaperNV(
                                       MMIMULTIM_AS_WALLPAPER_INFO  *wallpaper_file_ptr
                                       );

/*****************************************************************************/
//  Description : set wallpaper,close waiting window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CloseWaitSetWallpaper(
                                               GUIANIM_RESULT_E     result
                                               );

/*****************************************************************************/
//  Description : 设置图片为墙纸,处理请稍候消息
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_HandleWallpaperWaitMsg(void);

/*****************************************************************************/
// 	Description : reset pic wallpaper setting
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_ResetWallpaperSetting(void);

/*****************************************************************************/
// 	Description : set wallpaper,open waiting window
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenWaitSetWallpaper(void);
/*****************************************************************************/
// 	Description : get wallpaper information
//	Global resource dependence : none
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_GetWallPaperInfo (MMIMULTIM_AS_WALLPAPER_INFO *wallpaper_file_ptr);

/*****************************************************************************/
//Description : Set default system's more_wallpaperNV informatioin;
//Global resource dependence : 
//Author: Glen.Li
//Note: for "more pictures"
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetDefaultMoreWallpaperNV(void);

/*****************************************************************************/
//Description : Get more_wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note: for "more pictures"
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetMoreWallpaperNVByStyle(MMIFMM_AS_WALLPAPER_INFO* wallpaper_info_ptr);

/*****************************************************************************/
//Description : Set more_wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note: for "more pictures"
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetMoreWallpaperNVByStyle(MMIFMM_AS_WALLPAPER_INFO* wallpaper_info_ptr);
//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
// 	Description : 产生随机的文件名，并且设置.
//	Global resource dependence : 
//  Author:liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetRandomNameEditbox(
                                   const uint8              *prefix_ptr, //in    
                                   MMI_CTRL_ID_T            ctrl_id     //in
                                   );
#endif
/*****************************************************************************/
//  Description :if is FmmMainWinOpen
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsFmmMainWinOpen( void );


/*****************************************************************************/
// Description : update file list
// Global resource dependence : 
// Author:  jian.ma
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIFMM_UpdateFmmList(uint32 device,BOOLEAN is_enter_root);

/*****************************************************************************/
// Description : show file error type  
// Global resource dependence : 
// Author:  
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIFMM_PromptFileErrorType(MMI_WIN_ID_T alert_win_id, MMIFILE_ERROR_E fs_error, MMIFILE_DEVICE_E filedev);

/*****************************************************************************/
//  Description : check memory card status
//  Return: 
//  Global resource dependence : 
//  Author: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CheckMemoryCard(MMIFILE_DEVICE_E file_dev);

/*****************************************************************************/
//  Description : handle del task callback
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
void MMIAPIFMM_DelCallBack(uint16     result);

/*****************************************************************************/
//  Description : get file info form fullpath
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetFileInfoFormFullPath(
                                      const wchar *full_file_name,       //in
                                      const uint32 filename_len,          //in
                                      MMIFILE_FILE_INFO_T *file_info//out
                                      );

/*****************************************************************************/
//  Description : init system folder
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitSystemFolder(
                                     const wchar   *device_ptr,
                                     uint16  device_len,
                                     const wchar   *dir_ptr,
                                     uint16  dir_len
                                     );

/*****************************************************************************/
// 	Description: get the prefer disk
//	Global resource dependence:
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetDefaultDisk(void);

/*****************************************************************************/
// 	Description: get the number of all available disks
//	Global resource dependence:
//  Author: yaye.jiang
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIFMM_GetAllAvarialeDiskNum(void);

/*****************************************************************************/
// 	Description: is enough space
//	Global resource dependence:
//  Author: aoke.hu
//	Note:新增参数，获取满足空间要求的盘符.有转储需求的模块可以使用，比如拍照，录像，录音等
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsEnoughSpace(
                                     MMIFMM_STORE_TYPE_E store_type,
                                     MMIFILE_DEVICE_E prefer_devie,
                                     uint32 min_free_size,
                                     MMIFILE_DEVICE_E *device_ptr  //[out]
                                     );

/*****************************************************************************/
//  Description : get system file full path for read
//  Author:aoke.hu
//  Note: check from MMI_DEVICE_SYSTEM
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForReadEx(
                                                 BOOLEAN        is_fix_store,           //[IN]
                                                 BOOLEAN        is_from_cdisk,          //[IN]
                                                 const wchar    *path_name_ptr,         //[IN]
                                                 uint16         path_name_len,          //[IN]                                                 
                                                 const wchar    *file_name_ptr,         //[IN]
                                                 uint16         file_name_len,          //[IN]
                                                 wchar          *full_path_name_ptr,    //[OUT]
                                                 uint16         *full_path_len_ptr      //[OUT]                                                 
                                                 );

/*****************************************************************************/
//  Description : get system file full path for write
//  Author:aoke.hu
//  Note: check from MMI_DEVICE_SYSTEM
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForWriteEx(
                                                  BOOLEAN        is_fix_store,          //[IN]TRUE, is fixed. FALSE, is portable
                                                  BOOLEAN        is_from_cdisk,         //[IN]
                                                  const wchar    *path_name_ptr,        //[IN]
                                                  uint16         path_name_len,         //[IN]                                                 
                                                  const wchar    *file_name_ptr,        //[IN]
                                                  uint16         file_name_len,         //[IN]
                                                  uint32         min_free_size,         //[IN]
                                                  wchar          *full_path_name_ptr,   //[OUT]
                                                  uint16         *full_path_len_ptr     //[OUT]                                                 
                                                  );

/*****************************************************************************/
//  Description : get system file full path for read
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForRead(
                                                 BOOLEAN        is_fix_store,           //[IN]
                                                 const wchar    *path_name_ptr,         //[IN]
                                                 uint16         path_name_len,          //[IN]                                                 
                                                 const wchar    *file_name_ptr,         //[IN]
                                                 uint16         file_name_len,          //[IN]
                                                 wchar          *full_path_name_ptr,    //[OUT]
                                                 uint16         *full_path_len_ptr      //[OUT]                                                 
                                                 );

/*****************************************************************************/
//  Description : get system file full path for write
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForWrite(
                                                  BOOLEAN        is_fix_store,          //[IN]TRUE, is fixed. FALSE, is portable
                                                  const wchar    *path_name_ptr,        //[IN]
                                                  uint16         path_name_len,         //[IN]                                                 
                                                  const wchar    *file_name_ptr,        //[IN]
                                                  uint16         file_name_len,         //[IN]
                                                  uint32         min_free_size,         //[IN]
                                                  wchar          *full_path_name_ptr,   //[OUT]
                                                  uint16         *full_path_len_ptr     //[OUT]                                                 
                                                  );

/*****************************************************************************/
//  Description : create sysfile dir, if success return TRUE, else return FALSE
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CreateSysFileDir(
                                        MMIFILE_DEVICE_E    dev_type,               //[IN]
                                        const wchar     *path_name_ptr,         //[IN]
                                        uint16          path_name_len,          //[IN]
                                        MMIFILE_ERROR_E *file_err_ptr           //[OUT] can be null
                                        );

/*****************************************************************************/
//  Description : Combine Sys Path
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CombineSysPath(
                         MMIFILE_DEVICE_E    dev_type,              //[IN]
                         const wchar    *path_name_ptr,         //[IN]
                         uint16         path_name_len,          //[IN]
                         const wchar    *file_name_ptr,         //[IN]
                         uint16         file_name_len,          //[IN]
                         wchar          *full_path_name_ptr,    //[OUT]
                         uint16         *full_path_len_ptr      //[OUT]
                         );

/*****************************************************************************/
// Description : show file error type  
// Global resource dependence : 
// Author:  
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIFMM_PreviewPicture(wchar *full_path_name_ptr);

/*****************************************************************************/
// Description : preview current video file
// Global resource dependence : 
// Author:  murphy.xie
// RETRUN: 
// Note:   
/*****************************************************************************/
//PUBLIC void MMIAPIFMM_PreviewVideo(wchar *full_path_name_ptr);


/*****************************************************************************/
// Description : copy next  
// Global resource dependence : 
// Author:  
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CopyNext(void);

/*****************************************************************************/
// Description : setting file detail
// Global resource dependence : 
// Author: YING.XU
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetFolderDetail(
                                 MMIFMM_DETAIL_DATA_T  *detail_data,
                                 MMI_CTRL_ID_T              ctrl_id,
                                 MMI_WIN_ID_T            win_id
                                 );


//fixed by liyan.zhu for bugzilla 2760 2012-05-22 fmm search support
#ifdef MMI_FMM_SEARCH_SUPPORT
/*****************************************************************************/
// Description : 搜索串是否有效
// Global resource dependence : 
// Author: Ming.Song
// Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_FmmFindStringIsValid(
                                   MMI_STRING_T   *file_name_ptr  //in:
                                   );
#endif
//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
// Description : 打开文件列表窗口
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/ 
PUBLIC void MMIAPIFMM_OpenFMMMainWin(void);

/*****************************************************************************/
// Description : close文件列表窗口
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/ 
PUBLIC void MMIAPIFMM_CloseFMMMainWin(void);
#endif

/*****************************************************************************/
// Description : 文件详情
// Global resource dependence : 
// Author:aoke.hu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIFMM_DetailFileData(
                                     MMIFMM_DETAIL_DATA_T* detail_data  // 数据
                                     );

//fixed by liyan.zhu for bugzilla 2760
#if defined(DRM_SUPPORT)
/*****************************************************************************/
//  Description : convert mime type
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIFMM_ConvertMimeType(
                                                MIME_TYPE_E mime_type
                                                );

/*****************************************************************************/
// Description : add drm file detail
// Global resource dependence : 
// Author:haiwu.chen
// Note: 由于协议目前只能给出单个版权，所以目前实现单个版权的详情显示
/*****************************************************************************/
PUBLIC void MMIAPIFMM_AddDrmFileDetail(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri,
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 是否可以预览，如果不能预览，则需要提示下载
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_UpdateListIconData(
    MMI_CTRL_ID_T ctrl_id, 
    uint16 index,
    const wchar *file_dir_ptr,              //文件所在文件夹名称
    const wchar *file_path_ptr              //文件全路径
);

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 本函数用于更新选择文件夹中的图标
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_UpdateSelectListIconData(
    wchar *file_path_ptr,
    uint16 file_path_len
);

/*****************************************************************************/
//  Description : create drm pop menu
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIFMM_DrmOpenDownloadMenuWin(wchar *full_path);

/*****************************************************************************/
//  Description : get drm icon id by full path
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIFMM_GetDrmIconId(uint16 *file_path_ptr);
#endif

/*****************************************************************************/
// Description : open file  
// Global resource dependence : 
// Author: xiaoming.ren
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenFile(wchar *full_path_name_ptr);

/*****************************************************************************/
//  Description : MMIAPIFMM_ShowTxtContent
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC  void  MMIAPIFMM_ShowTxtContent(wchar *full_path_name_ptr);


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////// 以下接口部分是从mmi_filemgr.h移过来的      /////////////
///////////////////今后的对外接口都只能放在mmifmm_export.h///////////////
///////////////////**************************************************//////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
//#if 1
/*****************************************************************************/
//  Description : split full pathname to filepath and filename/dir
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 将输入的全路径（full_path_ptr）分解成路径名（full_path_ptr）+文件名（file_name_ptr）
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SplitLastName(uint16* full_path_ptr,             //[in],[out]
                                     uint16* full_path_len_ptr,         //[in],[out]
                                     uint16* file_name_ptr,             //[out]
                                     uint16* file_name_len_ptr          //[out]
                                     );

/*****************************************************************************/
//  Description : split full pathname to device, dir, filename
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SplitFullPath(      //支持多级目录
                              const wchar      *full_path_ptr,     //in
                              uint16           full_path_len,      //in, unicode个数
                              wchar            *device_ptr,        //out, device_xxx_ptr, dir_xxx_ptr, file_name_xxx_ptr 可以是NULL, 但是至少有一个不是NULL
                              uint16           *device_len_ptr,    //out, unicode个数
                              wchar            *dir_ptr,           //out
                              uint16           *dir_len_ptr,       //out, unicode个数
                              wchar            *file_name_ptr,     //out
                              uint16           *file_name_len_ptr  //out, unicode个数
                              );

/*****************************************************************************/
// 	Description : split full path name to path,name and suffix
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SplitFullPathExt(
                                     const wchar     *full_path_ptr, //in
                                     uint16          full_path_len,  //in
                                     wchar           *path_ptr,      //in/out:may PNULL
                                     uint16          *path_len_ptr,  //in/out:may PNULL,path_ptr不为空时,此处必须填max len
                                     wchar           *name_ptr,      //in/out:may PNULL,don't include suffix
                                     uint16          *name_len_ptr,  //in/out:may PNULL,name_ptr不为空时,此处必须填max len
                                     wchar           *suffix_ptr,    //in/out:may PNULL,include dot
                                     uint16          *suffix_len_ptr //in/out:may PNULL,suffix_ptr不为空时,此处必须填max len
                                     );

/*****************************************************************************/
//  Description : combine full pathname from device, file type, filename
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CombineFullPath(    //支持多级目录
                              const wchar     *device_ptr,        //in
                              uint16           device_len,         //in, unicode个数
                              const wchar     *dir_ptr,           //in
                              uint16           dir_len,            //in, unicode个数
                              const wchar     *file_name_ptr,     //in, file_name_ptr 可以是NULL
                              uint16           file_name_len,      //in, , unicode个数
                              wchar           *full_path_ptr,     //out
                              uint16           *full_path_len_ptr  //out, unicode个数
                              );

/*****************************************************************************/
//  Description : combin absolut path according to root, dir, name
//  Global resource dependence :                                
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIFMM_CombineFullPathEx(
                                          wchar * abs_path_ptr,   // [out] the absolute path name.
                                          uint32 abs_path_len,    // [in]  the buffer length of abs_path_ptr.
                                          MMIFILE_DEVICE_E file_dev,  // [in]  the file dev type.
                                          const wchar* dir_ptr,  // [in]  the directory name
                                          //                    BOOLEAN dir_is_ucs2,    // [in]  whether the directory is in ucs2
                                          const wchar* name_ptr//, // [in]  the file name 
                                          //                    BOOLEAN name_is_ucs2    // [in]  whether the file is in ucs2
                                          );
/*****************************************************************************/
//  Description : split file name to two part: name and suffix
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SplitFileName(
                              const wchar      *file_name_ptr,     //in
                              uint16           file_name_len,      //in, unicode个数
                              wchar            *name_ptr,          //out, name_xxx_ptr, suffix_xxx_ptr 可以是NULL, 但是至少有一个不是NULL
                              uint16           *name_len_ptr,      //out, unicode个数
                              wchar            *suffix_ptr,        //out
                              uint16           *suffix_len_ptr     //out, unicode个数
                              );

/*****************************************************************************/
//  Description : get file size string
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_GetFileSizeString(
                             uint32  size,
                             wchar*  str_ptr,
                             uint16  wstr_len,
							 BOOLEAN is_rounding
                             );

/*****************************************************************************/
//  Description : Init file system
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitFileTasks(void);


/*****************************************************************************/
//  Description : Init file system
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIAPIFMM_RegisterDevice(
                                const wchar *device_ptr,    //in
                                uint16      device_len      //in, unicode个数
                                );

/*****************************************************************************/
//  Description : unregister device 
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_UnRegisterDevice(
                                const wchar *device_ptr,    //in
                                uint16      device_len      //in, unicode个数
                                );


/*****************************************************************************/
//  Description : get device status, ok or not ok
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_FormatDevice(
                            const wchar *device_ptr,    //in
                            uint16      device_len,     //in, unicode个数
                            MMIFILE_DEVICE_FORMAT_E     format  //in
                            );

/*****************************************************************************/
//  Description : format device asyn
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_FormatDeviceAsyn(
                                 const wchar                *device_ptr,        //in
                                 uint16                     device_len,         //in, unicode个数
                                 MMIFILE_DEVICE_FORMAT_E    format,             //in
                                 MMI_WIN_ID_T               win_id,             //in
                                 MMI_MESSAGE_ID_E           msg_id              //in
                                );

/*****************************************************************************/
//  Description : get device status, ok or not ok
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetDeviceStatus(
                                const wchar *device_ptr,    //in
                                uint16      device_len      //in, unicode个数
                                );

/*****************************************************************************/
//  Description : set sd status
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetDeviceTypeStatus(MMIFILE_DEVICE_E memory_type);

/*****************************************************************************/
//  Description : 根据设备类型返回给设备的TEXT ID
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceName(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : 根据设备类型返回没有该设备的字符串
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetAlertTextIDByDev(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : 根据设备类型返回给设备容量的Text id
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceMemStatusText(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : 根据设备类型返回该设备的复制字符串的Text id
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceCopyText(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : Callback
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIAPIFMM_GetDevicePath(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : Callback
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIFMM_GetDevicePathLen(MMIFILE_DEVICE_E device_type);
//
///*****************************************************************************/
////  Description : Callback
////  Global resource dependence : 
////  Author:
////  Note: 
///*****************************************************************************/
// PUBLIC wchar*  MMIFILE_GetRootDir(MMIFILE_DEVICE_E file_dev);

/*****************************************************************************/
//  Description : Callback
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
// PUBLIC wchar* MMIFILE_GetPathByFileDevice(MMIFILE_DEVICE_E file_dev);

/*****************************************************************************/
//  Description : Callback
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
// PUBLIC uint16 MMIFILE_GetPathLenByFileDevice(MMIFILE_DEVICE_E file_dev);

/*****************************************************************************/
//  Description : get file device type by sd card type
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
// PUBLIC FILE_DEV_E_T MMIFILE_GetFileDeviceType(MMIFILE_DEVICE_E sd_type);

/*****************************************************************************/
//  Description : get sd card type type by file device
//  Global resource dependence : 
//  Author: gang.tong
//  Note:
/*****************************************************************************/ 
// PUBLIC MMIFILE_DEVICE_E MMIFILE_GetDeviceTypeByFileDev(FILE_DEV_E_T fs_dev);



/*****************************************************************************/
//  Description : get file device type by sd card type
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
// PUBLIC FILE_DEV_E_T MMIFILE_GetFileDeviceByFilter(uint8 file_filter);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
// PUBLIC uint8 MMIFILE_GetFileFilterByDevice(FILE_DEV_E_T file_dev);

/*****************************************************************************/
//  Description : 根据用户选择的设备类型，返回最适合的设备类型
//  Global resource dependence : 
//  Author: 
//  Note: 如果用户选择的设备剩余空间满足最小空间要求，返回用户设置；
//        如果用户选择的设备剩余空间不满足满足最小空间要求，返回到一个最大剩余空间的设备类型
//        如果U盘和所有的存储卡都不存在，返回默认的sd卡
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_GetSuitableFileDev(
												  MMIFILE_DEVICE_E user_sel_dev,//IN: 
												   uint32 min_free_space,//IN:
												   MMIFILE_DEVICE_E *suitable_dev_ptr//OUT:
												   );
											
/*****************************************************************************/
//  Description : create directory
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CreateDir(
                          const wchar      *full_path_ptr,     //in
                          uint16           full_path_len       //in, unicode个数
                          );

/*****************************************************************************/
//  Description : create directory with RO or HIDDEN attr
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetAttr(
                               const wchar       *full_path_ptr,     //in
                               uint16            full_path_len,       //in, 双字节为单位
                               BOOLEAN           is_hidden,         //in, 是否为隐藏属性 
                               BOOLEAN           is_ro,             //in, 是否为只读属性 
                               BOOLEAN           is_system,         //in, 是否为系统属性
                               BOOLEAN           is_arch           //in, 是否为存档属性
                               );

/*****************************************************************************/
//  Description : get file information
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetFileInfo(
                            const wchar             *full_path_ptr,     //in, 全文件名
                            uint16                  full_path_len,      //in, 全文件名长度, unicode个数
                            uint32                  *file_size_ptr,     //out,文件大小
							SFS_DATE_T				*modify_date,
							SFS_TIME_T				*modify_time
                            );


/*****************************************************************************/
//  Description : check whether file is exist
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsFileExist(
                            const wchar     *full_path_ptr,     //in
                            uint16          full_path_len       //in, unicode个数
                            );

/*****************************************************************************/
//  Description : check whether folder is exist
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsFolderExist(
                            const wchar     *full_path_ptr,     //in
                            uint16          full_path_len       //in, unicode个数
                            );

/*****************************************************************************/
//  Description : get device free space
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetDeviceFreeSpace(
                                    const wchar *device_ptr,        //in
                                    uint16      device_len,         //in, unicode个数
                                    uint32      *free_high_ptr,     //out
                                    uint32      *free_low_ptr       //out
                                    );

/*****************************************************************************/
//  Description : get device used space
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetDeviceUsedSpace(
                                    const wchar *device_ptr,        //in
                                    uint16      device_len,         //in, unicode个数
                                    uint32      *used_high_ptr,     //out
                                    uint32      *used_low_ptr       //out
                                    );

/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_RenameFile(
                            const wchar     *full_path_ptr,     //in
                            uint16          full_path_len,      //in, unicode个数
                            wchar          *new_file_name_ptr, //in
                            uint16          new_file_name_len   //in, unicode个数
                            );

/*****************************************************************************/
//  Description : check device name whether it is Udisk
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetDeviceTypeByPath(
                              const wchar *device_ptr,    //in
                              uint16    device_len      //in, 双字节为单位
                              );

/*****************************************************************************/
//  Description : read file data syn, file must exist
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_ReadFilesDataSyn(
                                const wchar             *full_path_ptr,     //in
                                uint16                  full_path_len,      //in, unicode个数
                                uint8                   *buf_ptr,           //out
                                uint32                  to_read_size        //in
                                );

/*****************************************************************************/
//  Description : write file data syn, create file always and will delete file
//                existing
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_WriteFilesDataSyn(
                                  const wchar            *full_path_ptr,     //in
                                  uint16                  full_path_len,      //in, unicode个数
                                  const uint8             *buf_ptr,           //in
                                  uint32                  to_write_size       //in
                                  );

/*****************************************************************************/
//  Description : delete file syn
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_DeleteFileSyn(
                              const wchar                  *full_path_ptr,     //in
                              uint16                  full_path_len       //in, unicode个数
                              );

/*****************************************************************************/
//  Description : copy file
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_CopyFile(const wchar *sour_filename, const wchar * dest_filename);
//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : read file data by offset syn, file must exist
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_ReadFileDataByOffsetSyn(
                                        const wchar             *full_path_ptr,     //in
                                        uint16                  full_path_len,      //in, unicode个数
                                        uint8                   *buf_ptr,           //out
                                        uint32                  to_read_size,       //in
                                        uint32                  offset              //in
                                        );
#endif
//fixed by liyan.zhu for bugzilla 2760
//#ifdef PIC_EDITOR_SUPPORT
/*****************************************************************************/
//  Description : read file data asyn
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_ReadFilesDataAsyn(
                                  const wchar             *full_path_ptr,   //in
                                  uint16                  full_path_len,    //in, unicode个数
                                  uint8                   *buf_ptr,         //out
                                  uint32                  to_read_size,     //in
                                  SFS_HANDLE              *file_handle_ptr, //in/out
                                  MMI_WIN_ID_T            win_id,           //in:往窗口发消息,0则往控件发消息
                                  MMI_CTRL_ID_T           ctrl_id,          //in:往控件发消息,0则往窗口发消息
                                  MMI_MESSAGE_ID_E        msg_id            //in
                                  );
//#endif
//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : read file data asyn with file handle
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_ReadFilesDataAsynEx(
    MMIFILE_HANDLE          file_handle,        //in
    uint8                   *buf_ptr,           //out
    uint32                  to_read_size,       //in
    MMI_WIN_ID_T            win_id,             //in:往窗口发消息,0则往控件发消息
    MMI_CTRL_ID_T           ctrl_id,            //in:往控件发消息,0则往窗口发消息
    MMI_MESSAGE_ID_E        msg_id              //in
);
#endif

/*****************************************************************************/
//  Description : write file data asyn
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_WriteFilesDataAsyn(
                                   const wchar             *full_path_ptr,     //in
                                   uint16                  full_path_len,      //in, unicode个数
                                   uint8                   *buf_ptr,           //in
                                   uint32                  to_write_size,      //in
                                   MMI_WIN_ID_T            win_id,             //in, 异步写文件数据结束后,需要发消息通知的 win id
                                   MMI_MESSAGE_ID_E        msg_id              //in, 异步写文件数据结束后,需要发送的消息, 带一个BOOLEAN的参数
                                   );

//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : read file data asyn with file handle
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_ReadFilesDataAsynEx(
    MMIFILE_HANDLE          file_handle,        //in
    uint8                   *buf_ptr,           //out
    uint32                  to_read_size,       //in
    MMI_WIN_ID_T            win_id,             //in:往窗口发消息,0则往控件发消息
    MMI_CTRL_ID_T           ctrl_id,            //in:往控件发消息,0则往窗口发消息
    MMI_MESSAGE_ID_E        msg_id              //in
);

/*****************************************************************************/
//  Description : write file data by offset asyn, file must exist
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_WriteFileDataByOffsetAsyn(
                                          const wchar             *full_path_ptr,     //in
                                          uint16                  full_path_len,      //in, unicode个数
                                          uint8                   *buf_ptr,           //in
                                          uint32                  to_write_size,      //in
                                          uint32                  offset,             //in
                                          MMI_WIN_ID_T            win_id,             //in
                                          MMI_MESSAGE_ID_E        msg_id              //in
                                          );
#endif
/*****************************************************************************/
//  Description : delete file asyn
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_DeleteFileAsyn(
                               const wchar             *full_path_ptr,     //in
                               uint16                  full_path_len,      //in, unicode个数
                               MMI_WIN_ID_T            win_id,             //in, 异步删除文件结束后,需要发消息通知的 win id
                               MMI_MESSAGE_ID_E        msg_id              //in, 异步删除文件结束后,需要发送的消息, 带一个BOOLEAN的参数
                               );

/*****************************************************************************/
//  Description : asyn operation confirm
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC void MMIAPIFMM_OperationAsynCnf(
                              void                  *param_ptr, 
                              MMI_MESSAGE_ID_E      callback_msg_id
                              );

/*****************************************************************************/
//  Description :start copy one file
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CopyOneFile(
                            const wchar                 *src_full_path,
                            uint16                      src_path_len,       //in, unicode个数
                            const wchar                 *dest_full_path,
                            uint16                      dest_path_len,      //in, unicode个数
                            MMI_WIN_ID_T                win_id,
                            MMI_MESSAGE_ID_E            msg_id
                            );

/*****************************************************************************/
//  Description : read file data in copy file, can't be used alone
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CopyReadSrc(
                            SFS_ERROR_E     error
                            );
//裁剪RAM之s_copy_info
#if 0
/*****************************************************************************/
//  Description : write file data in copy file, can't be used alone
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CopyWriteDest(
                              SFS_ERROR_E   error
                              );
#endif
/*****************************************************************************/
//  Description : is file end
//  Global resource dependence:
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_IsEndOfFile(  //TRUE: cur position is the end of the file.
                                   SFS_HANDLE handle
                                   );

/*****************************************************************************/
//  Description : Get a char from handle
//  Global resource dependence : 
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/ 
PUBLIC SFS_ERROR_E MMIAPIFMM_GetChar(
                                   SFS_HANDLE handle, //[in]
                                   void*  char_ptr //[out]
                                   );

/*****************************************************************************/
//  Description : create file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_HANDLE MMIAPIFMM_CreateFile(
                                    const wchar *file_name, 
                                    uint32 access_mode,
                                    uint32 share_mode,  //Must be NULL,File System have not realize it 
                                    uint32 file_attri       //Must be NULL,File System have not realize it 
                                    );

/*****************************************************************************/
//  Description : create file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_CreateFileAsyn(
                                    const wchar *file_name, 
                                    uint32 access_mode,
                                    uint32 share_mode,  //Must be NULL,File System have not realize it 
                                    uint32 file_attri,     //Must be NULL,File System have not realize it 
									MMI_WIN_ID_T            win_id,   //in
									MMI_MESSAGE_ID_E        msg_id,    //in
									MMIFILE_HANDLE        *file_handle_ptr//OUT
                                    );

/*****************************************************************************/
//  Description : delete file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_DeleteFile(
                                    const wchar *name,
                                    MMIFILE_OVERLAPPED_T *overlapped_ptr
                                    );

/*****************************************************************************/
//  Description : create directiory
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_CreateDirectory(
                                        const wchar *path
                                        );

/*****************************************************************************/
//  Description : delete all files in folder
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_DeleteTreeEx(
	const wchar *pathName,
	const wchar *fileName,
    BOOLEAN is_del_dir);

/*****************************************************************************/
//  Description : 同步删除某一文件夹的子文件和子文件夹(子文件夹中存在文件)
//  Parameter: [In] pathName  //文件夹所在的路径
//             [In] fileName  //文件夹名称
//             [In] is_del_dir  //是否将此文件夹也删除
//             [Return] 接口调用返回值
//  Author: zirui.li
//  Note:与MMIAPIFMM_DeleteTreeEx的区别在于此接口可以将有文件的子文件夹删除
/*****************************************************************************/
MMIFILE_ERROR_E MMIAPIFMM_DeleteAllTreeEx (
    const wchar *pathName,
    const wchar *fileName,
    BOOLEAN is_del_dir);

/*****************************************************************************/
//  Description : delete folder and it's files in it, only can delete it's sub empty folder
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_DeleteTree(
	const wchar *pathName,
	const wchar *fileName);

/*****************************************************************************/
//  Description : delete diretory
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_DeleteDirectory(
                                        const wchar *path
                                        );

/*****************************************************************************/
//  Description : find first file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_HANDLE MMIAPIFMM_FindFirstFile(
                                        const wchar *find, 
                                        uint16 find_len,
                                        MMIFILE_FIND_DATA_T *fd
                                        );

/*****************************************************************************/
//  Description : find nezt file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_FindNextFile(
                                        MMIFILE_HANDLE file_handle, 
                                        MMIFILE_FIND_DATA_T *fd
                                        );

/*****************************************************************************/
//  Description : find close
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_FindClose(
                                    MMIFILE_HANDLE file_handle
                                    );

/*****************************************************************************/
//  Description : get file size
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC uint32 MMIAPIFMM_GetFileSize(
                            MMIFILE_HANDLE file_handle
                            );

/*****************************************************************************/
//  Description : set file size
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_SetFileSize(
                                    MMIFILE_HANDLE file_handle, 
                                    uint32 size
                                    );

/*****************************************************************************/
//  Description : read file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_ReadFile(
                                MMIFILE_HANDLE file_handle,
                                void *buffer,
                                uint32 bytes_to_read,
                                uint32 *bytes_read,
                                MMIFILE_OVERLAPPED_T *overlapped
                                );

/*****************************************************************************/
//  Description : write file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_WriteFile(
                                MMIFILE_HANDLE file_handle,
                                const void *buffer,
                                uint32 bytes_to_write,
                                uint32 *bytes_written,
                                MMIFILE_OVERLAPPED_T *overlapped
                                );

/*****************************************************************************/
//  Description : set file pointer
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC uint32 MMIAPIFMM_SetFilePointer(
                                MMIFILE_HANDLE file_handle,
                                int32 offset,
                                uint32 origin
                                );

/*****************************************************************************/
//  Description : get file pointer
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC int32 MMIAPIFMM_GetFilePointer(
                                MMIFILE_HANDLE file_handle,
                                uint32 origin
                                );

/*****************************************************************************/
//  Description : close file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_CloseFile(
                                    MMIFILE_HANDLE file_handle
                                    );
/*****************************************************************************/
//  Description : check and get validate device
//  Global resource dependence : 
//  Author: gang.tong
//  Note: 指定设备类型无效时，依次从SD卡向UDISK查找设备
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetValidateDevice(MMIFILE_DEVICE_E original_device);

/*****************************************************************************/
//  Description : get file path len
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
//PUBLIC uint32 MMIAPIFILE_GetFilePathLen( const wchar * pWString );

/*****************************************************************************/
//  Description : Callback
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_Callback( uint16     result);

/*****************************************************************************/
//  Description : append file
//  Global resource dependence : 
//  Author: 
//  Note: append append_file_name from append_postion to base_file_name,
//        if is_delete_append_file is TRUE, after append, the append_file must be deleted
//  Return: return file result
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E  MMIAPIFMM_AppendFile(
                                    wchar *base_file_name,
									wchar *append_file_name,
									uint32 append_postion,
									BOOLEAN is_delete_append_file,
									uint32  *appended_size_ptr
                                    );
                                    
/*****************************************************************************/
//  Description : 返回剩余空间较大的device
//  Global resource dependence : 
//  Author: charlie.wang
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetMaxFreeDeviceType(void);

/*****************************************************************************/
//  Description : 返回第一个有效的device
//  Global resource dependence : 
//  Author: 
//  Note:如果没有有效的返回FS_INVALID
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetFirstValidDevice(void);

/*****************************************************************************/
//  Description : 返回第一个有效的sd card device type 
//  Global resource dependence : 
//  Author: 
//  Note:如果没有有效的返回MMI_DEVICE_NUM
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetFirstValidSD(void);

/*****************************************************************************/
//  Description : 返回第一个存在的device，这个device可能是无效的
//  Global resource dependence : 
//  Author: 
//  Note:如果没有存在的返回FS_INVALID
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetFirstExistDevice(void);

/*****************************************************************************/
//  Description : 根据设备类型返回给设备剩余容量的Text id
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceFreeSpaceText(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : 根据设备类型返回该设备的图标
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_IMAGE_ID_T  MMIAPIFMM_GetOSDImageIDByDeviceType(MMIFILE_DEVICE_E device_type);
/*****************************************************************************/
//  Description : has exist file when move
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_MoveFileExist(void);

/*****************************************************************************/
//  Description : 根据设备类型返回该设备的图标
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_IMAGE_ID_T  MMIAPIFMM_GetStorageIcon(MMIFILE_DEVICE_E device_type);
//#endif
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : is drm file by suffix name
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 通过后缀检查是不是drm文件(此接口待废弃，最好使用MMIAPIDRM_IsDRMFile接口)
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsDrmFileBySuffix(
    wchar *file_name_ptr, //in:
    uint16  file_name_len   //in:
);
#endif

/*****************************************************************************/
// 	Description : get total and used space of t/sd card
//	Global resource dependence :
//  Author:zhewei.tang
//	Note:05_31
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetTotalAndUsedInExternalMem(
            uint32     *used_space_low ,
            uint32    *used_space_high ,
            uint32    *total_space_high ,
            uint32    *total_space_low
    );

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////// 以下接口部分是从mmifmm_interface.h移过来的      /////////////
///////////////////今后的对外接口都只能放在mmifmm_export.h///////////////
///////////////////**************************************************//////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
//  Description : get icon id by fmm data info
//  Global resource dependence :
//  Author: james.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIFMM_GetIconIdByFileType( MMIFMM_FILE_TYPE_E  file_type, MMIFILE_DEVICE_E device);


/*****************************************************************************/
//  Description : play music file 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: add is_consume para
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_PlayMusicFileEx(
                        MMISET_ALL_RING_TYPE_E         ring_type,
                        BOOLEAN                        is_a2dp,
                        BOOLEAN                        is_consume,
                        const wchar                    *full_path_ptr,
                        uint16                         full_path_len,
                        uint32                         play_times,
                        uint32                         play_offset,
                        uint32                         vol,
                        MMISET_RING_CALLBACK_PFUNC    call_back
                        );

/*****************************************************************************/
//  Description : play music file 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_PlayMusicFile(
                        MMISET_ALL_RING_TYPE_E         ring_type,
                        BOOLEAN                        is_a2dp,
                        const wchar                    *full_path_ptr,
                        uint16                         full_path_len,
                        uint32                         play_times,
                        uint32                         play_offset,
                        uint32                         vol,
                        MMISET_RING_CALLBACK_PFUNC    call_back
                        );

/*****************************************************************************/
//  Description : close select music window
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CloseSelectFileWin(void);

/*****************************************************************************/
//  Description : close select music window
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIAPIFMM_CloseSelectMusicWin(void);

/*****************************************************************************/
//  Description : close select movie window
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIAPIFMM_CloseSelectMovieWin(void);

/*****************************************************************************/
//  Description :
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetOpenFileWinCurFile(
                                       FILEARRAY_DATA_T *f_array_data
                                       );

/*****************************************************************************/
//  Description : 把搜索类型转化成字符串形式
//  Global resource dependence : 
//  Author:liqingpeng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_GetFilerInfo(MMIFMM_FILTER_T* filter,uint32 suffix_type);

/*****************************************************************************/
//  Description :
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetOpenFileWinSoftkey(
                                       MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                       MMI_TEXT_ID_T   middlesoft_id, // the middle softkey id
                                       MMI_TEXT_ID_T   rightsoft_id   // the right softkey id
                                       );

/*****************************************************************************/
//  Description : get icon id by file name
//  Global resource dependence :
//  Author: james.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIAPIFMM_GetFileTypeByFileName(
                                                       const wchar  *file_name,
                                                       uint16       file_name_len
                                                       );
#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : open music file
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenMusicFileList(void);

/*****************************************************************************/
//  Description : open movie file
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenMovieFileList(void);
#endif //CMCC_UI_STYLE

/*****************************************************************************/
//  Description : help other modual to close win when sd plug
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CloseOptionWin(void);
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function  Declare                                                     */
/* Description:  Hanlde at cmd for fmm auto test cases                                                           */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#ifdef MMI_SYNCTOOL_SUPPORT
/*****************************************************************************/
//  Description : create folders in same path
//  Global resource dependence : s_fmm_current_path
//  Parameter: folders number
//  Author: shuyan.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Atest_FMMCreateFolderHori(uint16 hori_num);

/*****************************************************************************/
//  Description : create folders across path
//  Global resource dependence : s_fmm_current_path
//  Parameter: folders number
//  Author: shuyan.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Atest_FMMCreateFolderVerti(uint16 Veri_num);
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    End of Atest  Function  Declare                                                       */
/* Description:  Hanlde at cmd for fmm auto test cases                                                           */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#endif

//#ifdef DRM_SUPPORT
///*****************************************************************************/
////  Description : set the select drm file limit type
////  Global resource dependence : 
////  Author: haiwu.chen
////  Note: 由用户首先设置一个查找文件窗口的后缀形式，
////  然后针对DRM如果还有一些限制，则会在选中后根据用户的相关设置进行提示
////  注意本函数跟在选择窗口的后面才会起作用，如MMIAPIFMM_OpenSelectMovieWin等
////  这函数只针对DRM起作用 值设置成:MMIFMM_DRMFILE_LIMIT_COUNT，
////  MMIFMM_DRMFILE_LIMIT_INTERVAL, MMIFMM_DRMFILE_LIMIT_INTERVAL and so on
///*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIFMM_SetSelDrmLimitValue(MMIFMM_DRM_LIMIT_TYPE_T limit_value);
// #endif

/*****************************************************************************/
//  Description : move file call back
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_MoveCallBack(uint16 result);
/*****************************************************************************/
//  Description : copy is from move
//  Global resource dependence : none
//  Author: juan.wu
//  Note:如果从move呼叫copy,则为TRUE
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsCopyFromMove(void);
/*****************************************************************************/
//  Description : has exist file when move
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_MoveFileExist(void);
                                
 /*****************************************************************************/
//  Description : select a pic from fmm
//  Global resource dependence : 
//  Author: robert.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenSelectPictureWin(MMIFMM_SELECT_INFO_T *select_info);

/*****************************************************************************/
//  Description : select a pic from fmm
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenSelectAllFileWin(MMIFMM_SELECT_INFO_T *select_info);


/*****************************************************************************/
//  Description : select a music from fmm
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenSelectMusicWin(MMIFMM_SELECT_INFO_T *select_info);


/*****************************************************************************/
//  Description : select a movie from fmm
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenSelectMovieWin(MMIFMM_SELECT_INFO_T *select_info);

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : set the select drm file limit value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsSelDrmConstraintFile(
    uint16 *filename, 
    uint16 name_len, 
    MMIFMM_DRM_LIMIT_TYPE_T limit_value
);

/*****************************************************************************/
//  Description : set the select drm file limit type
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 由用户首先设置一个查找文件窗口的后缀形式，
//  然后针对DRM如果还有一些限制，则会在选中后根据用户的相关设置进行提示
//  注意本函数跟在选择窗口的后面才会起作用，如MMIAPIFMM_OpenSelectMovieWin等
//  这函数只针对DRM起作用 值设置成:MMIFMM_DRMFILE_LIMIT_COUNT，
//  MMIFMM_DRMFILE_LIMIT_INTERVAL, MMIFMM_DRMFILE_LIMIT_INTERVAL and so on
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetSelDrmLimitValue(MMIFMM_DRM_LIMIT_TYPE_T limit_value);
#endif

#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
/*****************************************************************************/
//  Description : select a font from fmm
//  Global resource dependence : 
//  Author: james.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenSelectFontWin(MMIFMM_SELECT_INFO_T *select_info);
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIFMM_ConvertFileType					 MMIAPIFMM_ConvertFileType

#define MMIFMM_ConvertMovieFileType	 		 MMIAPIFMM_ConvertMovieFileType

#define MMIFMM_ConvertMusicFileType 	 		 MMIAPIFMM_ConvertMusicFileType

#define MMIFMM_CopyCallBack		 			 MMIAPIFMM_CopyCallBack

#define MMIFMM_CopyFileExistInform 				 MMIAPIFMM_CopyFileExistInform

#define MMIFMM_UpdateCurrentFileLIst	 		 MMIAPIFMM_UpdateCurrentFileLIst

#define MMIFMM_IsDevEnoughSpace			 	 MMIAPIFMM_IsDevEnoughSpace

#define MMIFMM_GetPictureWidthHeight	 		 MMIAPIFMM_GetPictureWidthHeight

#define MMIAPIMULTIM_Init				 		 MMIAPIFMM_InitFolders

#define MMIAPIMULTIM_InitWallPaper		 		 MMIAPIFMM_InitWallPaper

#define MMIAPIMULTIM_SetPictureToWallpaper		 MMIAPIFMM_SetPictureToWallpaper

#define MMIAPIMULTIM_SetPictureToWallpaperEx	 MMIAPIFMM_SetPictureToWallpaperEx

#define MMIAPIMULTIM_SetPictureToPowerAndSaverEx  MMIAPIFMM_SetPictureToPowerAndSaverEx

#ifdef MMI_AUDIO_PLAYER_SUPPORT
#define MMIAPIMULTIM_SetMusicToRing			 MMIAPIFMM_SetMusicToRing
#endif
#define MMIAPIMULTIM_IsWallPaperFile			 MMIAPIFMM_IsWallPaperFile

#define MMIFMM_ChangeWallPaperFileName		 MMIAPIFMM_ChangeWallPaperFileName

#define MMIAPIMULTIM_CloseWaitSetWallpaper		 MMIAPIFMM_CloseWaitSetWallpaper

#define MMIAPIMULTIM_HandleWallpaperWaitMsg	 MMIAPIFMM_HandleWallpaperWaitMsg

#define MMIAPIMULTIM_OpenWaitSetWallpaper		 MMIAPIFMM_OpenWaitSetWallpaper

#define MMIMULTIM_GetWallPaperInfo			 MMIAPIFMM_GetWallPaperInfo

//fixed by liyan.zhu for bugzilla 2760
#if 0
#define MMIMULTIM_SetRandomNameEditbox		 MMIAPIFMM_SetRandomNameEditbox
#endif

//#define MMIFMM_PromptFileErrorType				 MMIAPIFMM_PromptFileErrorType

#define MMIFMM_DelCallBack					 MMIAPIFMM_DelCallBack

#define MMIFILE_InitSystemFolder				 MMIAPIFMM_InitSystemFolder

#define MMIFILE_GetDefaultDisk					 MMIAPIFMM_GetDefaultDisk

#define MMIFILE_IsEnoughSpace					 MMIAPIFMM_IsEnoughSpace

#define MMIFILE_GetSysFileFullPathForReadEx		 MMIAPIFMM_GetSysFileFullPathForReadEx

#define MMIFILE_GetSysFileFullPathForWriteEx	         MMIAPIFMM_GetSysFileFullPathForWriteEx

#define MMIFILE_GetSysFileFullPathForRead		 MMIAPIFMM_GetSysFileFullPathForRead

#define MMIFILE_GetSysFileFullPathForWrite		 MMIAPIFMM_GetSysFileFullPathForWrite

#define MMIFILE_CreateSysFileDir				 MMIAPIFMM_CreateSysFileDir

#define MMIFILE_CombineSysPath				MMIAPIFMM_CombineSysPath

#define MMIFMM_PreviewPicture					 MMIAPIFMM_PreviewPicture

//#define MMIFMM_PreviewVideo					 MMIAPIFMM_PreviewVideo

//#define MMIFMM_SetFolderDetail					 MMIAPIFMM_SetFolderDetail

//#define MMIFMM_SetFolderDetailCNF				 MMIAPIFMM_SetFolderDetailCNF

//#define MMIFMM_GetDrmIconId 					 MMIAPIFMM_GetDrmIconId

#define MMIFMM_UpdateListIconData				 MMIAPIFMM_UpdateListIconData

#define MMIFMM_UpdateSelectListIconData		 MMIAPIFMM_UpdateSelectListIconData

//////////////////////////////////define for mmi_filemgr.h   ////////////////////////////////////////

#define MMIFILE_SplitLastName					 MMIAPIFMM_SplitLastName

#define MMIFILE_SplitFullPath 					 MMIAPIFMM_SplitFullPath

#define MMIFILE_SplitFullPathExt				         MMIAPIFMM_SplitFullPathExt

#define MMIFILE_CombineFullPath				 MMIAPIFMM_CombineFullPath

#define MMIFILE_CombineFullPathEx				 MMIAPIFMM_CombineFullPathEx

#define MMIFILE_SplitFileName					 MMIAPIFMM_SplitFileName

#define MMIFMM_GetFileSizeString				 MMIAPIFMM_GetFileSizeString

#define MMIFILE_Init							 MMIAPIFMM_InitFileTasks

#define MMIFILE_RegisterDevice					 MMIAPIFMM_RegisterDevice

#define MMIFILE_UnRegisterDevice				 MMIAPIFMM_UnRegisterDevice

#define MMIFILE_FormatDevice					 MMIAPIFMM_FormatDevice

#define MMIFILE_FormatDeviceAsyn				 MMIAPIFMM_FormatDeviceAsyn

#define MMIFILE_GetDeviceStatus				 MMIAPIFMM_GetDeviceStatus

#define MMIAPISD_GetStatus					 MMIAPIFMM_GetDeviceTypeStatus

#define MMIFILE_GetDeviceName					 MMIAPIFMM_GetDeviceName

#define MMIFILE_GetAlertTextIDByDev			 MMIAPIFMM_GetAlertTextIDByDev

#define MMIFILE_GetDeviceMemStatusText		 MMIAPIFMM_GetDeviceMemStatusText

#define MMIFILE_GetDeviceCopyText				 MMIAPIFMM_GetDeviceCopyText

#define MMIFILE_GetDevicePath					 MMIAPIFMM_GetDevicePath

#define MMIFILE_GetDevicePathLen				 MMIAPIFMM_GetDevicePathLen

#define MMIFILE_GetSuitableFileDev				 MMIAPIFMM_GetSuitableFileDev

#define MMIFILE_CreateDir						 MMIAPIFMM_CreateDir

#define MMIFILE_SetAttr						 MMIAPIFMM_SetAttr

#define MMIFILE_GetFileInfo						 MMIAPIFMM_GetFileInfo

//#define MMIFILE_IsFileExist						 MMIAPIFMM_IsFileExist

//#define MMIFILE_IsFolderExist					 MMIAPIFMM_IsFolderExist

#define MMIFILE_GetDeviceFreeSpace				 MMIAPIFMM_GetDeviceFreeSpace

#define MMIFILE_GetDeviceUsedSpace				 MMIAPIFMM_GetDeviceUsedSpace

#define MMIFILE_RenameFile					 MMIAPIFMM_RenameFile

#define MMIFILE_GetDeviceTypeByPath			 MMIAPIFMM_GetDeviceTypeByPath

#define MMIFILE_ReadFilesDataSyn				 MMIAPIFMM_ReadFilesDataSyn

#define MMIFILE_WriteFilesDataSyn				 MMIAPIFMM_WriteFilesDataSyn

#define MMIFILE_DeleteFileSyn					 MMIAPIFMM_DeleteFileSyn.

#define MMIFILE_CopyFile						 MMIAPIFMM_CopyFile
//fixed by liyan.zhu for bugzilla 2760
#if 0
#define MMIFILE_ReadFileDataByOffsetSyn			 MMIAPIFMM_ReadFileDataByOffsetSyn
#endif
//fixed by liyan.zhu for bugzilla 2760
//#ifdef PIC_EDITOR_SUPPORT
#define MMIFILE_ReadFilesDataAsyn				 MMIAPIFMM_ReadFilesDataAsyn
//#endif
//fixed by liyan.zhu for bugzilla 2760
#if 0
#define MMIFILE_ReadFilesDataAsynEx			 MMIAPIFMM_ReadFilesDataAsynEx
#endif
#define MMIFILE_WriteFilesDataAsyn				 MMIAPIFMM_WriteFilesDataAsyn

//fixed by liyan.zhu for bugzilla 2760
#if 0
#define MMIFILE_ReadFilesDataAsynEx			 MMIAPIFMM_ReadFilesDataAsynEx

#define MMIFILE_WriteFileDataByOffsetAsyn		 MMIAPIFMM_WriteFileDataByOffsetAsyn
#endif
#define MMIFILE_DeleteFileAsyn					 MMIAPIFMM_DeleteFileAsyn

#define MMIFILE_OperationAsynCnf				 MMIAPIFMM_OperationAsynCnf

#define MMIFILE_CopyOneFile					 MMIAPIFMM_CopyOneFile

#define MMIFILE_CopyReadSrc					 MMIAPIFMM_CopyReadSrc
//裁剪RAM之s_copy_info
#if 0
#define MMIFILE_CopyWriteDest					 MMIAPIFMM_CopyWriteDest
#endif
#define MMIFILE_IsEndOfFile					 MMIAPIFMM_IsEndOfFile

#define MMIFILE_GetChar						 MMIAPIFMM_GetChar

//#define MMIFILE_CreateFile						 MMIAPIFMM_CreateFile

#define MMIFILE_CreateFileAsyn					 MMIAPIFMM_CreateFileAsyn

//#define MMIFILE_DeleteFile						 MMIAPIFMM_DeleteFile

#define MMIFILE_CreateDirectory					 MMIAPIFMM_CreateDirectory

#define MMIFILE_DeleteTreeEx					 MMIAPIFMM_DeleteTreeEx

#define MMIFILE_DeleteTree						 MMIAPIFMM_DeleteTree

//#define MMIFILE_DeleteDirectory					 MMIAPIFMM_DeleteDirectory

//#define MMIFILE_FindFirstFile					 MMIAPIFMM_FindFirstFile

//#define MMIFILE_FindNextFile					 MMIAPIFMM_FindNextFile

//#define MMIFILE_FindClose						 MMIAPIFMM_FindClose

//#define MMIFILE_GetFileSize						 MMIAPIFMM_GetFileSize

#define MMIFILE_SetFileSize						 MMIAPIFMM_SetFileSize

//#define MMIFILE_ReadFile						 MMIAPIFMM_ReadFile

//#define MMIFILE_WriteFile						 MMIAPIFMM_WriteFile

#define MMIFILE_SetFilePointer					 MMIAPIFMM_SetFilePointer

#define MMIFILE_GetFilePointer					 MMIAPIFMM_GetFilePointer

//#define MMIFILE_CloseFile						 MMIAPIFMM_CloseFile

#define MMIFILE_GetValidateDevice				 MMIAPIFMM_GetValidateDevice

#define MMIAPIFILE_Callback					 MMIAPIFMM_Callback

#define MMIFILE_AppendFile						 MMIAPIFMM_AppendFile

#define MMIFILE_GetMaxFreeDeviceType			 MMIAPIFMM_GetMaxFreeDeviceType

#define MMIFILE_GetFirstValidDevice				 MMIAPIFMM_GetFirstValidDevice

#define MMIFILE_GetFirstValidSD					 MMIAPIFMM_GetFirstValidSD

#define MMIFILE_GetFirstExistDevice				 MMIAPIFMM_GetFirstExistDevice

#define MMIFILE_GetDeviceFreeSpaceText			 MMIAPIFMM_GetDeviceFreeSpaceText

#define MMIFILE_GetOSDImageIDByDeviceType	 MMIAPIFMM_GetOSDImageIDByDeviceType

#define MMIFILE_GetStorageIcon					 MMIAPIFMM_GetStorageIcon

#ifdef DRM_SUPPORT
#define MMIFILE_IsDrmFileBySuffix				 MMIAPIFMM_IsDrmFileBySuffix
#endif

//////////////////////////////////      mmifmm_interface.h    ///////////////////////////////////

#define MMIFMM_GetIconIdByFileType				 MMIAPIFMM_GetIconIdByFileType

#define MMIFMM_OpenFileWinByLayer				 MMIAPIFMM_OpenFileWinByLayer

#define MMIFMM_SearchFileInPath				 MMIAPIFMM_SearchFileInPath

#define MMIFMM_SearchFileInPathEx				 MMIAPIFMM_SearchFileInPathEx

#define MMIFMM_SearchFileInPathBySort			 MMIAPIFMM_SearchFileInPathBySort

//#define MMIFMM_SearchFileCallBack			      MMIAPIFMM_SearchFileCallBack

#define MMIFMM_GetOpenFileWinCurFile			 MMIAPIFMM_GetOpenFileWinCurFile
 
#define MMIFMM_GetFilerInfo					 MMIAPIFMM_GetFilerInfo

#define MMIFMM_SetOpenFileWinSoftkey			 MMIAPIFMM_SetOpenFileWinSoftkey

#define MMIFMM_GetFileTypeByFileName			 MMIAPIFMM_GetFileTypeByFileName

#define FILE_OpenMatchedItem					 MMIAPIFMM_OpenMatchedItem

#define FILE_SetMatchItemData					 MMIAPIFMM_SetMatchItemData

#define MMIAPIFILE_StartLocalSearch				 MMIAPIFMM_StartLocalSearch

#define MMIAPIFILE_FreeLocalSearchResource		 MMIAPIFMM_FreeLocalSearchResource

#define MMIAPIFILE_GetFileSearchMatchedNum		 MMIAPIFMM_GetFileSearchMatchedNum

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

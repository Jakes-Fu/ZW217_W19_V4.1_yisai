/*****************************************************************************
** File Name:      mmi_drmfilemgr.h                                          *
** Author:                                                                   *
** Date:           02/23/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe drm file common operation   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/23/2011     haiwu.chen       Create                                    *
******************************************************************************/


#ifndef _MMI_DRMFILEMGR_H_
#define _MMI_DRMFILEMGR_H_


#if defined(DRM_SUPPORT)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sfs.h"
#include "drm_api.h"
#include "mmk_type.h"
#include "ffs.h"
#include "mmifmm_export.h"

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
#define MMIDRMFILE_HANDLE           SFS_HANDLE
#define MMIDRMFILE_OVERLAPPED_T     SFS_OVERLAPPED_T
#define MMIDRMFILE_ERROR_E          SFS_ERROR_E

#define MMIDRMFILE_RO_DIR           (g_mmidrm_ro_dir)   //name: "drm_ro"
#define MMIDRMFILE_RO_DIR_LEN       6

/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
typedef struct 
{
    uint8 tm_sec;     // secondsafter the minute - [0,59] 
    uint8 tm_min;     // minutesafter the hour - [0,59] 
    uint8 tm_hour;    // hours since midnight - [0,23] 
    uint8 tm_mday;    // day of the month - [1,31] 
    uint8 tm_mon;     // months since January - [0,11] 
    uint8 tm_year;    // years since [0~?]
}MMIDRMFILE_LEFT_TM_T;


typedef struct
{
    BOOLEAN is_syn_time;            //是否已经同步网络时间, 1:非初次使用，0:初次使用
    uint32 sys_time_value;           //同步时间差 = (本地时间 - 网络时间);单位:秒
    DRM_NET_TIME_HANDLE_TYPE_E net_time_type; //net time handle type
    
}MMIDRM_NITZ_TYPE_T;



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*与协议相关的DRM操作*/

/*****************************************************************************/
//  Description : check for drm file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsDRMFile(
    MMIDRMFILE_HANDLE file_handle
);

/*****************************************************************************/
//  Description : check for drm file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsDRMFileByPath(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri
);
    
/*****************************************************************************/
//  Description : check rights valid
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: permission: 
//  DRM_PERMISSION_PLAY--->mp3,mp4 application
//  DRM_PERMISSION_DISPLAY--->pic
//  DRM_PERMISSION_EXECUTE--->java
//  DRM_PERMISSION_PRINT--->no application
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRightsValid(
    MMIDRMFILE_HANDLE file_handle,
    DRM_PERMISSION_MODE_E permission    //哪种类型的文件对应那一种打开方式
);
    
/*****************************************************************************/
//  Description : check rights valid
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: permission see about "MMIDRM_IsRightsValid"
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRightsValidByPath(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri,
    DRM_PERMISSION_MODE_E permission    //哪种类型的文件对应那一种打开方式
);

/*****************************************************************************/
//  Description : check rights valid
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 注:此方法只给fmm用，因为FMM在罗列时，不必去关心是哪一种permission类型
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRsValidNoCarePer(
    MMIDRMFILE_HANDLE file_handle
);

/*****************************************************************************/
//  Description : check rights valid
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 注:此方法只给fmm用，因为FMM在罗列时，不必去关心是哪一种permission类型
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRsValidNoCarePerByPath(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri
);
    
/*****************************************************************************/
//  Description : check rights valid by rights
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRightsValidByRights(
    MMIDRMFILE_HANDLE file_handle,
    DRM_RIGHTS_T *drm_rights_ptr,           
    DRM_PERMISSION_MODE_E permission        //哪种类型的文件对应那一种打开方式
);

/*****************************************************************************/
//  Description : check the rights file is expired
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: permission 用户必须传入正确的permission,例如，picture: display
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRightsExpired(
    MMIDRMFILE_HANDLE file_handle,           
    DRM_PERMISSION_MODE_E permission
);

/*****************************************************************************/
//  Description : check the rights file is expired
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRightsExpiredByPath(
    const uint16 *file_name,
    uint32 access_mode,  
    uint32 share_mode, 
    uint32 file_attri,
    DRM_PERMISSION_MODE_E permission
);

/*****************************************************************************/
//  Description : check rights valid
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 注:此方法只给fmm用，因为FMM在罗列时，不必去关心是哪一种permission类型
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRsValidNoCarePer(
    MMIDRMFILE_HANDLE file_handle
);

/*****************************************************************************/
//  Description : check rights valid
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 注:此方法只给fmm用，因为FMM在罗列时，不必去关心是哪一种permission类型
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRsValidNoCarePerByPath(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri
);
    
/*****************************************************************************/
//  Description : check whether the file can be forwarded
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsCanForward(
    MMIDRMFILE_HANDLE file_handle
);
    
/*****************************************************************************/
//  Description : check whether the file can be forwarded
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsCanForwardByPath(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri
);
    
/*****************************************************************************/
//  Description : check whether the file can be forwarded
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsCanForwardByRights(
    DRM_RIGHTS_T *rights_ptr
);

/*****************************************************************************/
//  Description : get drm rights info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC MMIDRMFILE_ERROR_E MMIDRM_GetRightsinfo(
    MMIDRMFILE_HANDLE file_handle,
    DRM_RIGHTS_T *rights_ptr
);

/*****************************************************************************/
//  Description : get drm rights info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC MMIDRMFILE_ERROR_E MMIDRM_GetRightsinfoByPath(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri,
    DRM_RIGHTS_T *rights_ptr
);

/*****************************************************************************/
//  Description : consume drm rights
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_ConsumeRights(
    MMIDRMFILE_HANDLE file_handle
);

/*****************************************************************************/
//  Description : consume drm rights
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_ConsumeRightsByPath(
    const uint16 *file_name  
);

/*****************************************************************************/
//  Description : get drm file permission by mine tye
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 会根据文件媒体类型来获取文件的permission
/*****************************************************************************/
PUBLIC DRM_PERMISSION_MODE_E MMIDRM_GetFilePermission(
    MMIDRMFILE_HANDLE file_handle       //[in] handle priority if not null
);

/*****************************************************************************/
//  Description : get drm file permission by mine tye
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 会根据文件媒体类型来获取文件的permission
/*****************************************************************************/
PUBLIC DRM_PERMISSION_MODE_E MMIDRM_GetFilePermissionByPath(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri
);

/*****************************************************************************/
//  Description : get drm rights Constraint info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_GetRightsConstraint(
    MMIDRMFILE_HANDLE file_handle,
    DRM_CONSTRAINT_T *rights_cons_ptr
);

/*****************************************************************************/
//  Description : get drm rights interval time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIDRM_GetDateIntervalTime(
    MMIDRMFILE_HANDLE file_handle,
    DRM_RIGHTS_INTERVAL_T *interval_ptr
);

/*****************************************************************************/
// Description : get time left by interval
// Global resource dependence : 
// Author:haiwu.chen
// Note: 通过interval得到剩余时间的整形值
/*****************************************************************************/
PUBLIC uint32 MMIDRM_GetTimeLeft(
    const DRM_CONSTRAINT_T *right_cons_ptr
);

/*****************************************************************************/
// Description : translator left time to year type
// Global resource dependence : 
// Author:haiwu.chen
// Note: 通过整数换算成剩余几年等，和当前时间比
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_TransLeftTimeToYearType(
    uint32 left_time,
    MMIDRMFILE_LEFT_TM_T *left_time_ptr
);

/*****************************************************************************/
//  Description : get the type of drm file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC DRM_METHOD_LEVEL_E MMIDRM_GetDRMFileType(
    MMIDRMFILE_HANDLE file_handle
);

/*****************************************************************************/
//  Description : get the type of drm file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC DRM_METHOD_LEVEL_E MMIDRM_GetDRMFileTypeByPath(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri
);

/*****************************************************************************/
//  Description : get the type of drm file mime
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDRM_GetDRMFileMimeType(
    MMIDRMFILE_HANDLE file_handle
);

/*****************************************************************************/
//  Description : get the type of drm file mime
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDRM_GetDRMFileMimeTypeByPath(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri
);

/*****************************************************************************/
//  Description : get media file type
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIDRM_GetMediaFileType(
    MMIDRMFILE_HANDLE file_handle
);

/*****************************************************************************/
//  Description : get media file type
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIDRM_GetMediaFileTypeByPath(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri
);

/*****************************************************************************/
//  Description : download drm rights file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_DownloadRightsFile(
    MMIDRMFILE_HANDLE file_handle
);

/*****************************************************************************/
//  Description : download drm rights file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_DownloadRightsFileByPath(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri
);

/*****************************************************************************/
//  Description : handle the download drm and rights files
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC DRM_ERROR_E MMIDRM_InstallFileReq(
    DRM_INSTALL_FILE_T  *file_handle_ptr
);

/*****************************************************************************/
//  Description : init agent
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: sychronization network time and save the path of rights files
/*****************************************************************************/
PUBLIC void MMIDRM_InitAgent(void);

/*****************************************************************************/
//  Description : delete agent
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: pair with MMIDRM_InitAgent
/*****************************************************************************/
PUBLIC void MMIDRM_DeInitAgent(void);

/*****************************************************************************/
//  Description : create drm ro dir if it's not exist
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_CreateDrmRoDir(void);

/*****************************************************************************/
//  Description : get drm ro dir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIDRM_GetDrmRoDir(uint16* full_dir);

/*与应用相关的操作*/

/*****************************************************************************/
//  Description : delete drm_tmp directory
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: path is only
/*****************************************************************************/
PUBLIC void MMIDRM_DelDrmTmpDir(void);

/*****************************************************************************/
//  Description : Can the file set to call ring?
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 前提是DRM文件
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_CanDrmFileSetToWSPC(
    uint16 *file_path_ptr, 
    uint16 file_path_len,
    MMIFMM_FILE_TYPE_E file_type,
    DRM_PERMISSION_MODE_E permission
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/


#ifdef   __cplusplus
    }
#endif

#endif  //#if defined(DRM_SUPPORT)

#endif  //_MMI_DRMFILEMGR_H_

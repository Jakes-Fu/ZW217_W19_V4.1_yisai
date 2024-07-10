/****************************************************************************
** File Name:      mmidrm_export.h                                          *
** Author:         haiwu.chen                                               *
** Date:           03/29/2011                                               *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the interface of drm op    *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/29/2011     haiwu.chen        Create                                 *
****************************************************************************/
#ifndef  _DRM_EXPORT_H_    
#define  _DRM_EXPORT_H_   


#if defined(DRM_SUPPORT)

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sfs.h"
#include "drm_api.h"
#include "mmk_type.h"
#include "ffs.h"
#include "mmifmm_export.h"
#include "mmi_drmfilemgr.h"
#include "mmidrm.h"
#include "mmidrm_image.h"
#include "mmidrm_text.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
// drm file setting the type
typedef enum
{
    DRMFILE_SETTING_NONE,        
    DRMFILE_SETTING_WALLPAPER,
    DRMFILE_SETTING_SCREENSAVER,
    DRMFILE_SETTING_POWERONOFFANIM,
    DRMFILE_SETTING_CALLRING,
    DRMFILE_SETTING_MAXNUM
    
}DRMFILE_SETTING_TYPE;

// drm pre-check status value
typedef enum
{
    DRMFILE_PRE_CHECK_NORMAL,           //一切正常
    DRMFILE_PRE_CHECK_INVALID_PARAM,    //无效参数   
    DRMFILE_PRE_CHECK_INVALID_HANDLE,   //无效句柄
    DRMFILE_PRE_CHECK_NO_SYS_NITZ,      //网络时间未同步
    DRMFILE_PRE_CHECK_NO_RIGHTS,        //版权过期
    DRMFILE_PRE_CHECK_MAXNUM
    
}DRMFILE_PRE_CHECK_STATUS_E;     

extern const wchar g_mmidrm_tmp_dir[];

#define MMIDRMFILE_SPTMP_DIR            (g_mmidrm_tmp_dir)   //name: "drm_sptmp"
#define MMIDRMFILE_SPTMP_DIR_LEN        9

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*与协议相关的DRM操作*/

//注意本文的处理函数传参均有一特色，希望在开头就讲清楚明白。
//DRM涉及到文件处理，均希望用户传入文件句柄，而不必在这里再去打开，关闭影响效率
//但是为了大家方便，也提供带路径的参数方式，比如参数列表中有:handle, file_name
//实现时均会优先处理文件句柄，如果文件句柄为null，则会去处理file_name参数
//另:如果有传入版权之类的，优先级则比handle高

/*****************************************************************************/
//  Description : drm module init
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_InitModule(void);

/*****************************************************************************/
//  Description : init drm module nv
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIDRM_RegNv(void);

/*****************************************************************************/
//  Description : drm init
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_Init(void);

/*****************************************************************************/
//  Description : check for drm file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 如果有file_handle传入，则优先处理，否则处理file_name
//  return: True: is drm file
//          False: is not drm file
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_IsDRMFile(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : check the rights file is expired
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: permission 用户必须传入正确的permission,例如，picture: display
//        如果有file_handle传入，则优先处理，否则处理file_name
//  return: True: drm file is expired
//          False: drm file is not expired
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_IsRightsExpired(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name,            //[in] if file_handle is null, handle path
    DRM_PERMISSION_MODE_E permission
);
    
/*****************************************************************************/
//  Description : check whether the file can be forwarded
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 如果没有rights_ptr传入，则优先处理，次之file_handle,最后处理file_name
//  return: True: drm file can forward
//          False: drm file can not forward
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_IsCanForward(
    DRM_RIGHTS_T *rights_ptr,           //[in] handle priority if rights is not null
    MMIDRMFILE_HANDLE file_handle,      //[in] handle secondly if not null
    const uint16 *file_name             //[in] handle last if not null
);
    
/*****************************************************************************/
//  Description : check rights valid by rights
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_IsRightsValid(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name,            //[in] if file_handle is null, handle path
    BOOLEAN is_care_permission,         //[in] 大部分应用都应该关心，然后传入permission
    DRM_PERMISSION_MODE_E permission    //[in] 哪种类型的文件对应那一种打开方式
);

/*****************************************************************************/
//  Description : get drm rights info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 如果有file_handle传入，则优先处理，否则处理file_name
/*****************************************************************************/
PUBLIC MMIDRMFILE_ERROR_E MMIAPIDRM_GetRightsinfo(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name,            //[in] if file_handle is null, handle path
    DRM_RIGHTS_T *rights_ptr            //[out] get the rights
);

/*****************************************************************************/
//  Description : get drm file permission by mine tye
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 会根据文件媒体类型来获取文件的permission
/*****************************************************************************/
PUBLIC DRM_PERMISSION_MODE_E MMIAPIDRM_GetFilePermission(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : consume drm rights
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_ConsumeRights(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : get drm rights Constraint info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_GetRightsConstraint(
    MMIDRMFILE_HANDLE file_handle,      //[in] 
    DRM_CONSTRAINT_T *rights_cons_ptr   //[out] 
);

/*****************************************************************************/
//  Description : get drm rights interval time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIAPIDRM_GetDateIntervalTime(
    MMIDRMFILE_HANDLE file_handle,      //[in]
    DRM_RIGHTS_INTERVAL_T *interval_ptr //[out]
);

/*****************************************************************************/
// Description : get time left by interval
// Global resource dependence : 
// Author:haiwu.chen
// Note: 通过interval得到剩余时间的整形值
/*****************************************************************************/
PUBLIC uint32 MMIAPIDRM_GetTimeLeft(
    const DRM_CONSTRAINT_T *right_cons_ptr
);

/*****************************************************************************/
// Description : translator left time to year type
// Global resource dependence : 
// Author:haiwu.chen
// Note: 通过整数换算成剩余几年等，和当前时间比
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_TransLeftTimeToYearType(
    uint32 left_time,
    MMIDRMFILE_LEFT_TM_T *left_time_ptr
);

/*****************************************************************************/
//  Description : get the type of drm file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC DRM_METHOD_LEVEL_E MMIAPIDRM_GetDRMFileType(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : get the type of drm file mime
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIDRM_GetDRMFileMimeType(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : get media file type
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIAPIDRM_GetMediaFileType(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : download drm rights file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_DownloadRightsFile(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : set encrypt
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIAPIDRM_SDSetEncrypt(
    MMIDRMFILE_HANDLE file_handle, 
    BOOLEAN is_encrpt_Read
);

/*****************************************************************************/
//  Description : handle the download drm and rights files
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC DRM_ERROR_E MMIAPIDRM_InstallFileReq(
    DRM_INSTALL_FILE_T  *file_handle_ptr
);

/*****************************************************************************/
//  Description : delete agent
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: pair with MMIDRM_InitAgent
/*****************************************************************************/
PUBLIC void MMIAPIDRM_DeInitAgent(void);


/*与应用相关的操作*/

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 在使用DRM文件之前，统一一并检查文件状态，
//  注意如果返回值为非DRMFILE_PRE_CHECK_NORMAL，则原流程原则上应该stop
/*****************************************************************************/
PUBLIC DRMFILE_PRE_CHECK_STATUS_E MMIAPIDRM_PreCheckFileStatus(
    wchar *file_path_ptr,                   //in
    DRM_PERMISSION_MODE_E permmison         //in
);

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 在使用DRM文件之前，统一一并检查文件状态，
//  SD则提示请求并返回TRUE，其它值对应状态并返回FALSE。   适用于自行提示的应用使用（SD下载版权公用）
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_PreCheckFileStatusEX(
                                      wchar *file_path_ptr,                   //in
                                      DRM_PERMISSION_MODE_E permmison,         //in
                                      DRMFILE_PRE_CHECK_STATUS_E *drm_status_ptr   //out
                                                               );

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 在使用DRM文件之前，统一一并检查文件状态，
//  注意如果返回值为非DRMFILE_PRE_CHECK_NORMAL，则原流程原则上应该stop
/*****************************************************************************/
PUBLIC DRMFILE_PRE_CHECK_STATUS_E MMIAPIDRM_PreCheckFileStatusExForMMS(
                                      wchar *file_path_ptr,                   //in
                                      DRM_PERMISSION_MODE_E permmison,         //in
                                      BOOLEAN is_alert_win
                                                               );

/*****************************************************************************/
//  Description : Can the file set to all the type,include Wallpaper,ScreenSaver,CallRing
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 此文件是否可以被设置成手机的壁纸、屏保、来电铃声、开关机动画等所有类型
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_CanDrmFileSetToWSPC(
    uint16 *file_path_ptr, 
    uint16 file_path_len,
    DRMFILE_SETTING_TYPE setting_type
);

/*****************************************************************************/
//  Description : convert mime type
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIDRM_ConvertAudioMimeType(
                            MIME_TYPE_E mime_type
                            );

/*****************************************************************************/
//  Description : is the network time syn
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_IsSynNitzTime(void);

/*****************************************************************************/
//  Description : init nv MMINV_SET_NETWORK_TIME_NUM
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_InitNetworkTime(void);

/*****************************************************************************/
//  Description : get the result of local time minus network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_GetNetworkTime(MMIDRM_NITZ_TYPE_T *drm_nitz_ptr);

/*****************************************************************************/
//  Description : set the result of local time minus network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_SetNetworkTime(MMIDRM_NITZ_TYPE_T drm_nitz);

/*****************************************************************************/
//  Description : get the global nitz_time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDRM_GetDrmNitzTime(MMIDRM_NITZ_TYPE_T *nitz_time_ptr);

/*****************************************************************************/
//  Description : set the global nitz_time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDRM_SetDrmNitzTime(MMIDRM_NITZ_TYPE_T nitz_time);

/*****************************************************************************/
//  Description : read network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 开机后，读取NV的网络时间，并设置全局值
/*****************************************************************************/
PUBLIC void MMIAPIDRM_ReadDrmNitzTime(void);

/*****************************************************************************/
//  Description : update net work time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 开机的时候，检查本手机出厂后是否同步过网络时间一次，如果没有，请更新
//      网络时间不是随便乱更新的，一定要检查一下，是否有必要，不然就是浪费资源
/*****************************************************************************/
PUBLIC void MMIAPIDRM_UpdateNetworkTime(void);

/*****************************************************************************/
//  Description : clear net work time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 网络时间被人更新了，则需要重置一下，置我们关心的flag及清除差值
/*****************************************************************************/
PUBLIC void MMIAPIDRM_ResetNetworkTime(void);

/*****************************************************************************/
//  Description : restore factory network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 恢复出厂设置，则需要把标志位置0，表示手机出厂后从未同步过
/*****************************************************************************/
PUBLIC void MMIAPIDRM_RestoreFactoryNetworkTime(void);

/*****************************************************************************/
//  Description : alter network time when system time is changed
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 维护本地时间差值，当系统时间被修改了
/*****************************************************************************/
PUBLIC void MMIAPIDRM_AlterNetworkTime(
    uint32 old_time,
    uint32 new_time
);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif

#endif  //#if defined(DRM_SUPPORT)

#endif /* _DRM_EXPORT_H_ */

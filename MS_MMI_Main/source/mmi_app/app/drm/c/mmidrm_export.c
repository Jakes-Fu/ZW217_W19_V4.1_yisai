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
#include "mmi_app_drm_trc.h"
#if defined(DRM_SUPPORT)

#if defined(__cplusplus)
extern "C" {
#endif


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32	
#include "std_header.h"
#endif
#include "mmidrm_export.h"
#include "mmi_image.h"
#include "mmipub.h"
#ifdef MMI_UPDATE_TIME_SUPPORT
//#include "mmiset.h"
#include "mmiset_export.h"
#endif
#include "mmisrvaud_api.h"


MMIDRM_NITZ_TYPE_T g_drm_nitz = {0};

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : drm module init
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_InitModule(void)
{
    MMIDRM_RegNv();
}

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
)
{
    BOOLEAN is_ret = FALSE;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_IsDRMFile(file_handle);
    }
    else if (PNULL != file_name)
    {
        is_ret = MMIDRM_IsDRMFileByPath(file_name, 
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_IsDRMFile: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_70_112_2_18_2_14_43_79,(uint8*)"");
        is_ret = FALSE;
    }

    return is_ret;
}

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
)
{
    BOOLEAN is_ret = FALSE;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_IsRightsExpired(file_handle, permission);
    }
    else if (PNULL != file_name)
    {
        is_ret = MMIDRM_IsRightsExpiredByPath(file_name, 
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0, permission);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_IsRightsExpired: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_103_112_2_18_2_14_44_80,(uint8*)"");
        is_ret = FALSE;
    }

    return is_ret;
}
    
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
)
{
    BOOLEAN is_ret = FALSE;
    
    if (PNULL != rights_ptr)
    {
        is_ret = MMIDRM_IsCanForwardByRights(rights_ptr);
    }
    else if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_IsCanForward(file_handle);
    }
    else if (PNULL != file_name)
    {
        is_ret = MMIDRM_IsCanForwardByPath(file_name, 
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_IsCanForward: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_139_112_2_18_2_14_44_81,(uint8*)"");
        is_ret = FALSE;
    }

    return is_ret;
}
    
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
)
{
    BOOLEAN is_ret = FALSE;
    
    if (is_care_permission)
    {
        if (SFS_INVALID_HANDLE != file_handle)
        {
            is_ret = MMIDRM_IsRightsValid(file_handle, permission);
        }
        else if (PNULL != file_name)
        {
            is_ret = MMIDRM_IsRightsValidByPath(file_name, 
                SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0, permission);
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_IsRightsValid: invalid param!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_172_112_2_18_2_14_44_82,(uint8*)"");
            is_ret = FALSE;
        }
    }
    else 
    {
        if (SFS_INVALID_HANDLE != file_handle)
        {
            is_ret = MMIDRM_IsRsValidNoCarePer(file_handle);
        }
        else if (PNULL != file_name)
        {
            is_ret = MMIDRM_IsRsValidNoCarePerByPath(file_name, 
                SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_IsRightsValid: invalid param!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_189_112_2_18_2_14_44_83,(uint8*)"");
            is_ret = FALSE;
        }
    }

    return is_ret;
}

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
)
{
    MMIDRMFILE_ERROR_E is_ret = SFS_ERROR_INVALID_PARAM;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_GetRightsinfo(file_handle, rights_ptr);
    }
    else if (PNULL != file_name)
    {
        is_ret = MMIDRM_GetRightsinfoByPath(file_name, 
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0, rights_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_GetRightsinfo: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_220_112_2_18_2_14_44_84,(uint8*)"");
        is_ret = SFS_ERROR_INVALID_PARAM;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : get drm file permission by mine tye
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 会根据文件媒体类型来获取文件的permission
/*****************************************************************************/
PUBLIC DRM_PERMISSION_MODE_E MMIAPIDRM_GetFilePermission(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
)
{
    DRM_PERMISSION_MODE_E is_ret = DRM_PERMISSION_NONE;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_GetFilePermission(file_handle);
    }
    else if (PNULL != file_name)
    {
        is_ret = MMIDRM_GetFilePermissionByPath(file_name, 
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_GetFilePermission: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_249_112_2_18_2_14_44_85,(uint8*)"");
        is_ret = DRM_PERMISSION_NONE;//SFS_ERROR_INVALID_PARAM;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : consume drm rights
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_ConsumeRights(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
)
{
    BOOLEAN is_ret = FALSE;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_ConsumeRights(file_handle);
    }
    else if (PNULL != file_name)
    {
        is_ret = MMIDRM_ConsumeRightsByPath(file_name);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_ConsumeRights: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_277_112_2_18_2_14_44_86,(uint8*)"");
        is_ret = FALSE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : get drm rights Constraint info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_GetRightsConstraint(
    MMIDRMFILE_HANDLE file_handle,      //[in] 
    DRM_CONSTRAINT_T *rights_cons_ptr   //[out] 
)
{
    BOOLEAN is_ret = FALSE;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_GetRightsConstraint(file_handle, rights_cons_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_GetRightsConstraint: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_301_112_2_18_2_14_44_87,(uint8*)"");
        is_ret = FALSE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : get drm rights interval time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIAPIDRM_GetDateIntervalTime(
    MMIDRMFILE_HANDLE file_handle,      //[in]
    DRM_RIGHTS_INTERVAL_T *interval_ptr //[out]
)
{
    SFS_ERROR_E is_ret = SFS_NO_ERROR;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_GetDateIntervalTime(file_handle, interval_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_GetDateIntervalTime: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_325_112_2_18_2_14_44_88,(uint8*)"");
        is_ret = SFS_ERROR_INVALID_PARAM;
    }

    return is_ret;
}

/*****************************************************************************/
// Description : get time left by interval
// Global resource dependence : 
// Author:haiwu.chen
// Note: 通过interval得到剩余时间的整形值
/*****************************************************************************/
PUBLIC uint32 MMIAPIDRM_GetTimeLeft(
    const DRM_CONSTRAINT_T *right_cons_ptr
)
{
    return MMIDRM_GetTimeLeft(right_cons_ptr);
}

/*****************************************************************************/
// Description : translator left time to year type
// Global resource dependence : 
// Author:haiwu.chen
// Note: 通过整数换算成剩余几年等，和当前时间比
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_TransLeftTimeToYearType(
    uint32 left_time,
    MMIDRMFILE_LEFT_TM_T *left_time_ptr
)
{
    return MMIDRM_TransLeftTimeToYearType(left_time, left_time_ptr);
}

/*****************************************************************************/
//  Description : get the type of drm file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC DRM_METHOD_LEVEL_E MMIAPIDRM_GetDRMFileType(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
)
{
    DRM_METHOD_LEVEL_E is_ret = DRM_LEVEL_INVALID;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_GetDRMFileType(file_handle);
    }
    else if (PNULL != file_name)
    {
        is_ret = MMIDRM_GetDRMFileTypeByPath(file_name, 
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_GetDRMFileType: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_377_112_2_18_2_14_44_89,(uint8*)"");
        is_ret = DRM_LEVEL_INVALID;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : get the type of drm file mime
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIDRM_GetDRMFileMimeType(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
)
{
    uint32 is_ret = 0;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_GetDRMFileMimeType(file_handle);
    }
    else if (PNULL != file_name)
    {
        is_ret = MMIDRM_GetDRMFileMimeTypeByPath(file_name, 
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_GetDRMFileMimeType: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_406_112_2_18_2_14_44_90,(uint8*)"");
        is_ret = 0;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : get media file type
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIAPIDRM_GetMediaFileType(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
)
{
    MMIFMM_FILE_TYPE_E is_ret = MMIFMM_FILE_TYPE_NUM;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_GetMediaFileType(file_handle);
    }
    else if (PNULL != file_name)
    {
        is_ret = MMIDRM_GetMediaFileTypeByPath(file_name, 
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_GetMediaFileType: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_435_112_2_18_2_14_45_91,(uint8*)"");
        is_ret = MMIFMM_FILE_TYPE_NUM;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : download drm rights file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_DownloadRightsFile(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
)
{
    BOOLEAN is_ret = FALSE;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_DownloadRightsFile(file_handle);
    }
    else if (PNULL != file_name)
    {
        is_ret = MMIDRM_DownloadRightsFileByPath(file_name, 
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_DownloadRightsFile: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_464_112_2_18_2_14_45_92,(uint8*)"");
        is_ret = FALSE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : set encrypt
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIAPIDRM_SDSetEncrypt(
    MMIDRMFILE_HANDLE file_handle, 
    BOOLEAN is_encrpt_Read
)
{
    return DRM_SDSetEncrypt(file_handle, is_encrpt_Read);
}

/*****************************************************************************/
//  Description : handle the download drm and rights files
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC DRM_ERROR_E MMIAPIDRM_InstallFileReq(
    DRM_INSTALL_FILE_T  *file_handle_ptr
)
{
    return MMIDRM_InstallFileReq(file_handle_ptr);
}

/*****************************************************************************/
//  Description : delete agent
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: pair with MMIDRM_InitAgent
/*****************************************************************************/
PUBLIC void MMIAPIDRM_DeInitAgent(void)
{
    MMIDRM_DeInitAgent();
}

/*****************************************************************************/
//  Description : is the network time syn
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_IsSynNitzTime(void)
{
    return g_drm_nitz.is_syn_time;
}

/*****************************************************************************/
//  Description : get the global nitz_time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDRM_GetDrmNitzTime(MMIDRM_NITZ_TYPE_T *nitz_time_ptr)
{
    if (PNULL != nitz_time_ptr)
    {
        nitz_time_ptr->net_time_type = g_drm_nitz.net_time_type;
        nitz_time_ptr->is_syn_time = g_drm_nitz.is_syn_time;
        nitz_time_ptr->sys_time_value = g_drm_nitz.sys_time_value;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_DownloadRightsFile: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_526_112_2_18_2_14_45_93,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : delete agent
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: pair with MMIDRM_InitAgent
/*****************************************************************************/
PUBLIC void MMIAPIDRM_SetDrmNitzTime(MMIDRM_NITZ_TYPE_T nitz_time)
{
    g_drm_nitz.net_time_type = nitz_time.net_time_type;
    g_drm_nitz.is_syn_time = nitz_time.is_syn_time;
    g_drm_nitz.sys_time_value = nitz_time.sys_time_value;
}

/*****************************************************************************/
//  Description : read network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 开机后，读取NV的网络时间，并设置全局值
/*****************************************************************************/
PUBLIC void MMIAPIDRM_ReadDrmNitzTime(void)
{
    MMIDRM_NITZ_TYPE_T nitz_type = {0};
    
    //读取NV
    MMIAPIDRM_GetNetworkTime(&nitz_type);

    //设置全局变量值
    MMIAPIDRM_SetDrmNitzTime(nitz_type);
}

/*****************************************************************************/
//  Description : update net work time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 开机的时候，检查本手机出厂后是否同步过网络时间一次，如果没有，请更新
//      网络时间不是随便乱更新的，一定要检查一下，是否有必要，不然就是浪费资源
/*****************************************************************************/
PUBLIC void MMIAPIDRM_UpdateNetworkTime()
{
    MMIDRM_NITZ_TYPE_T nitz_type = {0};

    MMIAPIDRM_GetDrmNitzTime(&nitz_type);
    
    if (!nitz_type.is_syn_time)
    {
        //update network time
#ifdef MMI_UPDATE_TIME_SUPPORT
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_UpdateNetworkTime: begin to auto update network time!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_571_112_2_18_2_14_45_94,(uint8*)"");
        //MMIAPISET_StartManualUpdateSystemTime();
#else
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_UpdateNetworkTime: not support auto update network time!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_574_112_2_18_2_14_45_95,(uint8*)"");
#endif
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_UpdateNetworkTime: the phone is already update network time!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_579_112_2_18_2_14_45_96,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : clear net work time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 网络时间被人更新了，则需要重置一下，置我们关心的flag及清除差值
/*****************************************************************************/
PUBLIC void MMIAPIDRM_ResetNetworkTime()
{
    //网络时间更新成功且修改了本地时间，此时需要更新一下DRM维护的时间差值等
    
    MMIDRM_NITZ_TYPE_T nitz_type = {0};

    nitz_type.is_syn_time = TRUE;
    nitz_type.sys_time_value = 0;
    nitz_type.net_time_type = 0;
    
    //记录nv
    MMIAPIDRM_SetNetworkTime(nitz_type);

    //修改全局变量值
    MMIAPIDRM_SetDrmNitzTime(nitz_type);
}

/*****************************************************************************/
//  Description : restore factory network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 恢复出厂设置，则需要把标志位置0，表示手机出厂后从未同步过
/*****************************************************************************/
PUBLIC void MMIAPIDRM_RestoreFactoryNetworkTime(void)
{
    //手机恢复出厂设置，清0
    
    MMIDRM_NITZ_TYPE_T nitz_type = {0};

    nitz_type.is_syn_time = FALSE;
    nitz_type.sys_time_value = 0;
    nitz_type.net_time_type = 0;
    
    //记录nv
    MMIAPIDRM_SetNetworkTime(nitz_type);

    //修改全局变量值
    MMIAPIDRM_SetDrmNitzTime(nitz_type);
}

/*****************************************************************************/
//  Description : alter network time when system time is changed
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 维护本地时间差值，当系统时间被修改了
/*****************************************************************************/
PUBLIC void MMIAPIDRM_AlterNetworkTime(
    uint32 old_time,
    uint32 new_time
)
{
    MMIDRM_NITZ_TYPE_T nitz_type = {0};
    uint32 time_minus = 0;

    MMIAPIDRM_GetDrmNitzTime(&nitz_type);

    //如果手机时间还未有同步，则不必更新时间差值
    if (nitz_type.is_syn_time)
    {
        if (new_time >= old_time)
        {
            time_minus = new_time - old_time;
            if (DRM_NET_TIME_ADD == nitz_type.net_time_type)
            {
                time_minus += nitz_type.sys_time_value;
            }
            else if (DRM_NET_TIME_REDUCE == nitz_type.net_time_type)
            {
                if (time_minus > nitz_type.sys_time_value)
                {
                    nitz_type.net_time_type = DRM_NET_TIME_ADD;
                    time_minus -= nitz_type.sys_time_value;
                }
                else
                {
                    time_minus = nitz_type.sys_time_value - time_minus;
                }
            }
            else
            {
                nitz_type.net_time_type = DRM_NET_TIME_ADD;
            }
        }
        else
        {
            time_minus = old_time - new_time;
            if (DRM_NET_TIME_ADD == nitz_type.net_time_type)
            {
                if (time_minus > nitz_type.sys_time_value)
                {
                    time_minus = time_minus - nitz_type.sys_time_value;
                    nitz_type.net_time_type = DRM_NET_TIME_REDUCE;
                }
                else
                {
                    time_minus = nitz_type.sys_time_value - time_minus;
                }
            }
            else if (DRM_NET_TIME_REDUCE == nitz_type.net_time_type)
            {
                time_minus += nitz_type.sys_time_value;
            }
            else
            {
                nitz_type.net_time_type = DRM_NET_TIME_REDUCE;
            }
        }
        
        //time_minus = (int64)((int64)new_time - (int64)old_time);

        nitz_type.sys_time_value = time_minus;
    
        //记录nv
        MMIAPIDRM_SetNetworkTime(nitz_type);

        //修改全局变量值
        MMIAPIDRM_SetDrmNitzTime(nitz_type);
        
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_AlterNetworkTime: alter network time, newtime-oldtime=%d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_701_112_2_18_2_14_45_97,(uint8*)"d", time_minus);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_AlterNetworkTime: the phone's nitz time is not update, so don't alter!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_705_112_2_18_2_14_45_98,(uint8*)"");
    }
}




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
                                                               )
{    
    DRMFILE_PRE_CHECK_STATUS_E  ret_value = DRMFILE_PRE_CHECK_INVALID_PARAM;
    MMIFILE_HANDLE              file_handle = 0;
    BOOLEAN                     is_valid = FALSE;
    BOOLEAN                     is_net_sys_time = TRUE;
    BOOLEAN                     is_drm_file = TRUE;
    DRM_METHOD_LEVEL_E          method_type = DRM_LEVEL_INVALID;
    wchar                       *drmfile_name_ptr = PNULL;
    DRM_RIGHTS_T                drm_rights = {0};
    DRM_CONSTRAINT_T            *rights_cons_ptr = PNULL;
    
    if (PNULL == file_path_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_PreCheckFileStatus: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_735_112_2_18_2_14_45_99,(uint8*)"");
        return ret_value;
    }
    
    file_handle = SFS_CreateFile(file_path_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        if (!MMIAPIDRM_IsDRMFile(file_handle, PNULL))
        {
            ret_value = DRMFILE_PRE_CHECK_NORMAL;
            is_drm_file = FALSE;
        }
        else
        {
            is_valid = MMIAPIDRM_IsRightsValid(file_handle, PNULL, TRUE, permmison);
            if (is_valid)
            {
                ret_value = DRMFILE_PRE_CHECK_NORMAL;
            }
            else
            {
                MMIAPIDRM_GetRightsinfo(file_handle, PNULL, &drm_rights);
                rights_cons_ptr = &(drm_rights.constraint);
                method_type = drm_rights.method_level;
                
                if (0 != (rights_cons_ptr->type & DRM_CONSTRAINT_INTERVAL)
                    || 0 != (rights_cons_ptr->type & DRM_CONSTRAINT_DATETIME))
                {
                    if (!MMIAPIDRM_IsSynNitzTime())
                    {
                        //未同步网络时间
                        ret_value = DRMFILE_PRE_CHECK_NO_SYS_NITZ;
                        MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SYS_NITZ);
                        is_net_sys_time = FALSE;
                    }
                    else
                    {
                        //版本无效
                        ret_value = DRMFILE_PRE_CHECK_NO_RIGHTS;
                    }
                }
                else
                {
                    //版本无效
                    ret_value = DRMFILE_PRE_CHECK_NO_RIGHTS;
                }
            }
        }
    }
    else
    {
        ret_value = DRMFILE_PRE_CHECK_INVALID_HANDLE;
    }
    
    SFS_CloseFile(file_handle);
    
    if (is_drm_file && is_net_sys_time && !is_valid)
    {
        switch (method_type)
        {
        case DRM_LEVEL_SD:
            //alloc memory
            drmfile_name_ptr = SCI_ALLOC_APPZ((MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
            SCI_MEMSET(drmfile_name_ptr, 0x00, (MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
            
            //set send param
            MMIAPICOM_Wstrcpy(drmfile_name_ptr, file_path_ptr);
            
            MMIPUB_OpenQueryWinByTextIdEx(
                MMK_GetFirstAppletHandle(),
                TXT_DRM_COPYRIGHTS_INVLIAD_DOWNLOAD_RO,
                IMAGE_PUBWIN_QUERY,
                PNULL,
                MMIAPIDRM_HandleDrmDLRoQueryMsg,
                (uint32)drmfile_name_ptr);
            break;
            
        default:
            MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE);
            break;
            
        }        
    }
    
    return ret_value;
}

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
                                                               )
{
    DRMFILE_PRE_CHECK_STATUS_E  ret_value = DRMFILE_PRE_CHECK_INVALID_PARAM;
    MMIFILE_HANDLE              file_handle = 0;
    BOOLEAN                     is_valid = FALSE;
    BOOLEAN                     is_net_sys_time = TRUE;
    BOOLEAN                     is_drm_file = TRUE;
    DRM_METHOD_LEVEL_E          method_type = DRM_LEVEL_INVALID;
    wchar                       *drmfile_name_ptr = PNULL;
    DRM_RIGHTS_T                drm_rights = {0};
    DRM_CONSTRAINT_T            *rights_cons_ptr = PNULL;
    
    if (PNULL == file_path_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_PreCheckFileStatus: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_846_112_2_18_2_14_46_100,(uint8*)"");
        return ret_value;
    }
    
    file_handle = SFS_CreateFile(file_path_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        if (!MMIAPIDRM_IsDRMFile(file_handle, PNULL))
        {
            ret_value = DRMFILE_PRE_CHECK_NORMAL;
            is_drm_file = FALSE;
        }
        else
        {
            is_valid = MMIAPIDRM_IsRightsValid(file_handle, PNULL, TRUE, permmison);
            if (is_valid)
            {
                ret_value = DRMFILE_PRE_CHECK_NORMAL;
            }
            else
            {
                MMIAPIDRM_GetRightsinfo(file_handle, PNULL, &drm_rights);
                rights_cons_ptr = &(drm_rights.constraint);
                method_type = drm_rights.method_level;
                
                if (0 != (rights_cons_ptr->type & DRM_CONSTRAINT_INTERVAL)
                    || 0 != (rights_cons_ptr->type & DRM_CONSTRAINT_DATETIME))
                {
                    if (!MMIAPIDRM_IsSynNitzTime())
                    {
                        //未同步网络时间
                        ret_value = DRMFILE_PRE_CHECK_NO_SYS_NITZ;
                        MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SYS_NITZ);
                        is_net_sys_time = FALSE;
                    }
                    else
                    {
                        //版本无效
                        ret_value = DRMFILE_PRE_CHECK_NO_RIGHTS;
                    }
                }
                else
                {
                    //版本无效
                    ret_value = DRMFILE_PRE_CHECK_NO_RIGHTS;
                }
            }
        }
    }
    else
    {
        ret_value = DRMFILE_PRE_CHECK_INVALID_HANDLE;
    }
    
    SFS_CloseFile(file_handle);
    
    if (is_drm_file && is_net_sys_time && !is_valid && is_alert_win)
    {
        switch (method_type)
        {
        case DRM_LEVEL_SD:
            //alloc memory
            drmfile_name_ptr = SCI_ALLOC_APPZ((MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
            SCI_MEMSET(drmfile_name_ptr, 0x00, (MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
            
            //set send param
            MMIAPICOM_Wstrcpy(drmfile_name_ptr, file_path_ptr);
            
            MMIPUB_OpenQueryWinByTextIdEx(
                MMK_GetFirstAppletHandle(),
                TXT_DRM_COPYRIGHTS_INVLIAD_DOWNLOAD_RO,
                IMAGE_PUBWIN_QUERY,
                PNULL,
                MMIAPIDRM_HandleDrmDLRoQueryMsg,
                (uint32)drmfile_name_ptr);
            break;
            
        default:
            MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE);
            break;
            
        }        
    }
    
    return ret_value;
}

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
                                                               )
{ 
    BOOLEAN                     ret_value = FALSE;
    MMIFILE_HANDLE              file_handle = 0;
    BOOLEAN                     is_valid = FALSE;
    BOOLEAN                     is_net_sys_time = TRUE;
    BOOLEAN                     is_drm_file = TRUE;
    DRM_METHOD_LEVEL_E          method_type = DRM_LEVEL_INVALID;
    wchar                       *drmfile_name_ptr = PNULL;
    DRM_RIGHTS_T                drm_rights = {0};
    DRM_CONSTRAINT_T            *rights_cons_ptr = PNULL;
    
    if (PNULL == file_path_ptr || PNULL == drm_status_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_PreCheckFileStatusEX: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_957_112_2_18_2_14_46_101,(uint8*)"");
        return ret_value;
    }
    
    file_handle = SFS_CreateFile(file_path_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        if (!MMIAPIDRM_IsDRMFile(file_handle, PNULL))
        {
            *drm_status_ptr = DRMFILE_PRE_CHECK_NORMAL;
            is_drm_file = FALSE;
        }
        else
        {
            is_valid = MMIAPIDRM_IsRightsValid(file_handle, PNULL, TRUE, permmison);
            if (is_valid)
            {
                *drm_status_ptr = DRMFILE_PRE_CHECK_NORMAL;
            }
            else
            {
                MMIAPIDRM_GetRightsinfo(file_handle, PNULL, &drm_rights);
                rights_cons_ptr = &(drm_rights.constraint);
                method_type = drm_rights.method_level;
                
                if (0 != (rights_cons_ptr->type & DRM_CONSTRAINT_INTERVAL)
                    || 0 != (rights_cons_ptr->type & DRM_CONSTRAINT_DATETIME))
                {
                    if (!MMIAPIDRM_IsSynNitzTime())
                    {
                        //未同步网络时间
                        *drm_status_ptr = DRMFILE_PRE_CHECK_NO_SYS_NITZ;
                        is_net_sys_time = FALSE;
                    }
                    else
                    {
                        //版本无效
                        *drm_status_ptr = DRMFILE_PRE_CHECK_NO_RIGHTS;
                    }
                }
                else
                {
                    //版本无效
                    *drm_status_ptr = DRMFILE_PRE_CHECK_NO_RIGHTS;
                }
            }
        }
    }
    else
    {
        *drm_status_ptr = DRMFILE_PRE_CHECK_INVALID_HANDLE;
    }
    
    SFS_CloseFile(file_handle);
    
    if (is_drm_file && is_net_sys_time && !is_valid)
    {
        switch (method_type)
        {
        case DRM_LEVEL_SD:
            //alloc memory
            drmfile_name_ptr = SCI_ALLOC_APPZ((MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
            SCI_MEMSET(drmfile_name_ptr, 0x00, (MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
            
            //set send param
            MMIAPICOM_Wstrcpy(drmfile_name_ptr, file_path_ptr);
            
            MMIPUB_OpenQueryWinByTextIdEx(
                MMK_GetFirstAppletHandle(),
                TXT_DRM_COPYRIGHTS_INVLIAD_DOWNLOAD_RO,
                IMAGE_PUBWIN_QUERY,
                PNULL,
                MMIAPIDRM_HandleDrmDLRoQueryMsg,
                (uint32)drmfile_name_ptr);
            ret_value = TRUE;
            break;
            
        default:
            break;
            
        }        
    }
    
    return ret_value;
}


//*****************************************************************************/
//  Description : Can the file set to all the type,include Wallpaper,ScreenSaver,CallRing
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 此文件是否可以被设置成手机的壁纸、屏保、来电铃声、开关机动画等所有类型
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_CanDrmFileSetToWSPC(
    uint16 *file_path_ptr, 
    uint16 file_path_len,
    DRMFILE_SETTING_TYPE setting_type
)
{
    BOOLEAN is_ret = TRUE;

    if (PNULL == file_path_ptr
        || 0 == file_path_len
        || DRMFILE_SETTING_NONE == setting_type)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_CanDrmFileSetToAllType: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_1062_112_2_18_2_14_46_102,(uint8*)"");
        return is_ret;
    }
    
    if (!MMIAPIFMM_IsDrmFileBySuffix(file_path_ptr, file_path_len))
    {
        //如果不是drm文件，则应该直接退回之前的逻辑，交由用户自己控制
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_CanDrmFileSetToAllType: not drm file suffix!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_EXPORT_1069_112_2_18_2_14_46_103,(uint8*)"");
        return is_ret;
    }

    switch (setting_type)
    {
        case DRMFILE_SETTING_WALLPAPER:
            is_ret = MMIDRM_CanDrmFileSetToWSPC(file_path_ptr, file_path_len, 
                MMIFMM_FILE_TYPE_PICTURE, DRM_PERMISSION_DISPLAY);
            break;
            
        case DRMFILE_SETTING_SCREENSAVER:
            is_ret = MMIDRM_CanDrmFileSetToWSPC(file_path_ptr, file_path_len, 
                MMIFMM_FILE_TYPE_MOVIE, DRM_PERMISSION_PLAY);
            break;
            
        case DRMFILE_SETTING_POWERONOFFANIM:
            is_ret = MMIDRM_CanDrmFileSetToWSPC(file_path_ptr, file_path_len, 
                MMIFMM_FILE_TYPE_MOVIE, DRM_PERMISSION_PLAY);
            break;
            
        case DRMFILE_SETTING_CALLRING:
            is_ret = MMIDRM_CanDrmFileSetToWSPC(file_path_ptr, file_path_len, 
                MMIFMM_FILE_TYPE_MUSIC, DRM_PERMISSION_PLAY);
            break;
            
        default:
            break;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : convert mime type
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIDRM_ConvertAudioMimeType(
                            MIME_TYPE_E mime_type
                            )
{
    MMISRVAUD_RING_FMT_E drm_audio_type = MMISRVAUD_RING_FMT_MAX;

    switch (mime_type)
    {
    case MIME_TYPE_AUDIO_MID:
    case MIME_TYPE_AUDIO_MIDI:
    case MIME_TYPE_AUDIO_X_MIDI:
    case MIME_TYPE_AUDIO_SP_MIDI:
#ifdef MIDI_SUPPORT
        drm_audio_type = MMISRVAUD_RING_FMT_MIDI;
#endif
        break;
        
    case MIME_TYPE_AUDIO_MPEG:
#ifdef MP3_SUPPORT
        drm_audio_type = MMISRVAUD_RING_FMT_MP3;
#endif
        break;

    case MIME_TYPE_AUDIO_X_AAC:
#ifdef AAC_SUPPORT
        drm_audio_type = MMISRVAUD_RING_FMT_AAC;
#endif
        break;        
        
    case MIME_TYPE_AUDIO_X_WAV:
#ifdef WAV_SUPPORT
        drm_audio_type = MMISRVAUD_RING_FMT_WAVE;
#endif
        break;
        
    case MIME_TYPE_AUDIO_AMR:
#ifdef AMR_SUPPORT
        drm_audio_type = MMISRVAUD_RING_FMT_AMR;
#endif
        break;

    default:
        break;

    }
    return drm_audio_type;
}


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


#if defined(__cplusplus)
}
#endif

#endif  //#if defined(DRM_SUPPORT)


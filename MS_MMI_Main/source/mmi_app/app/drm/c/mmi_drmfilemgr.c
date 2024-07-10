/*****************************************************************************
** File Name:      mmi_drmfilemgr.c                                          *
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

#include "mmi_app_drm_trc.h"
#if defined(DRM_SUPPORT)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_drmfilemgr.h"
#include "mmifmm_export.h"
#include "sfs.h"
#include "mmicom_time.h"
#include "mmidrm_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif

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
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/
const wchar g_mmidrm_ro_dir[] = {'D', 'r', 'm', '_', 'r', 'o', 0};//drm_ro
const wchar g_mmidrm_tmp_dir[] = {'D', 'r', 'm', '_', 's', 'p', 't', 'm', 'p', 0};//drm_sptmp

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNettimeSysn(DRM_NETWORK_TIME_T *nettime_ptr);
    
/*****************************************************************************/
//  Description : check rights valid
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsRightsValid(
    MMIDRMFILE_HANDLE file_handle,
    BOOLEAN care_permission,
    DRM_PERMISSION_MODE_E permission
);

/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DECLARE                        *
 **--------------------------------------------------------------------------*/ 

/*****************************************************************************/
//  Description : check for drm file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsDRMFile(
    MMIDRMFILE_HANDLE file_handle
)
{
    BOOLEAN is_ret = FALSE;
        
    if (SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsDRMFile: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_78_112_2_18_2_14_35_0,(uint8*)"");
        return FALSE;
    }

    is_ret = DRM_IsDRMFile(file_handle);

    return is_ret;
}

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
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    BOOLEAN is_ret = FALSE;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsDRMFileByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_103_112_2_18_2_14_35_1,(uint8*)"");
        return FALSE; 
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, access_mode, share_mode, file_attri);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_IsDRMFile(file_handle);
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    return is_ret;
}
    
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
)
{
    BOOLEAN is_ret = FALSE;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = IsRightsValid(file_handle, TRUE, permission);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsRightsValid: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_140_112_2_18_2_14_35_2,(uint8*)"");
    }
    
    return is_ret;
}
    
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
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    BOOLEAN is_ret = FALSE;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsRightsValidByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_163_112_2_18_2_14_35_3,(uint8*)"");
        return FALSE; 
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, access_mode, share_mode, file_attri);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = IsRightsValid(file_handle, TRUE, permission);
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    return is_ret;
}

/*****************************************************************************/
//  Description : check rights valid
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 注:此方法只给fmm用，因为FMM在罗列时，不必去关心是哪一种permission类型
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRsValidNoCarePer(
    MMIDRMFILE_HANDLE file_handle
)
{
    BOOLEAN is_ret = FALSE;
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = IsRightsValid(file_handle, FALSE, DRM_PERMISSION_DISPLAY);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsRsValidNoCarePer: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_195_112_2_18_2_14_35_4,(uint8*)"");
    }
    
    return is_ret;
}

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
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    BOOLEAN is_ret = FALSE;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsRsValidNoCarePerByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_217_112_2_18_2_14_35_5,(uint8*)"");
        return FALSE; 
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, access_mode, share_mode, file_attri);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = IsRightsValid(file_handle, FALSE, DRM_PERMISSION_DISPLAY);
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    return is_ret;
}
    
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
)
{
    BOOLEAN is_ret = FALSE;

    if (PNULL == file_handle
        || PNULL == drm_rights_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsRightsValidByRights: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_248_112_2_18_2_14_36_6,(uint8*)"");
        return is_ret;
    }
    
    if (!DRM_IsRightsExpired(file_handle, permission))
    {
        is_ret = TRUE;
    }

    return is_ret;
}
    
/*****************************************************************************/
//  Description : check the rights file is expired
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRightsExpired(
    MMIDRMFILE_HANDLE file_handle,
    DRM_PERMISSION_MODE_E permission
)
{
    BOOLEAN is_ret = TRUE;

    if (SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsRightsExpired: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_273_112_2_18_2_14_36_7,(uint8*)"");
        return is_ret;
    }

    //rights file whether expired
    is_ret = DRM_IsRightsExpired(file_handle, permission);
    
    return is_ret;
}

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
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    BOOLEAN is_ret = TRUE;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsRightsExpiredByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_300_112_2_18_2_14_36_8,(uint8*)"");
        return FALSE; 
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, access_mode, share_mode, file_attri);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_IsRightsExpired(file_handle, permission);
    }
    
    MMIAPIFMM_CloseFile(file_handle);
    
    return is_ret;
}

/*****************************************************************************/
//  Description : check the rights file is expired
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsRightsExpiredByRights(
    MMIDRMFILE_HANDLE file_handle,
    DRM_RIGHTS_T *rights_ptr
)
{
    BOOLEAN is_ret = TRUE;

    if (SFS_INVALID_HANDLE == file_handle 
        || PNULL == rights_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsRightsExpiredByRights: valid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_330_112_2_18_2_14_36_9,(uint8*)"");
        return is_ret;
    }

    //rights file whether expired
    is_ret = MMIDRM_IsRightsExpired(file_handle, rights_ptr->permission);

    return is_ret;
}

/*****************************************************************************/
//  Description : check whether the file can be forwarded
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsCanForward(
    MMIDRMFILE_HANDLE file_handle
)
{
    BOOLEAN is_ret = FALSE;    
    DRM_RIGHTS_T drm_right = {0};
    SFS_ERROR_E error = SFS_NO_ERROR;

    if (SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsCanForward: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_354_112_2_18_2_14_36_10,(uint8*)"");
        return is_ret;
    }

    if(!MMIDRM_IsDRMFile(file_handle))
    {
        //非drm文件，可以转发
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsCanForward: this isn't drm file, return %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_361_112_2_18_2_14_36_11,(uint8*)"d", is_ret);
        is_ret = TRUE;
    }
    else
    {
        error = DRM_GetRightsInfo(file_handle, &drm_right);

        if (SFS_NO_ERROR != error)
        {
            //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsCanForward: get rights error = %d!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_370_112_2_18_2_14_36_12,(uint8*)"d", error);
            is_ret = FALSE;
        }
        else
        {
            //rights file whether forward
            is_ret = (DRM_LEVEL_SD == drm_right.method_level);
        }
    }    

    return is_ret;
}
    
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
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    BOOLEAN is_ret = FALSE;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsCanForwardByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_399_112_2_18_2_14_36_13,(uint8*)"");
        return FALSE; 
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, access_mode, share_mode, file_attri);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_IsCanForward(file_handle);
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    return is_ret;
}
    
/*****************************************************************************/
//  Description : check whether the file can be forwarded
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_IsCanForwardByRights(
    DRM_RIGHTS_T *rights_ptr
)
{
    BOOLEAN is_ret = FALSE;

    if (PNULL == rights_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_IsCanForwardByRights: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_427_112_2_18_2_14_36_14,(uint8*)"");
        return is_ret;
    }

    //rights file whether forward
    return (DRM_LEVEL_SD == rights_ptr->method_level);
}

/*****************************************************************************/
//  Description : get drm rights info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC MMIDRMFILE_ERROR_E MMIDRM_GetRightsinfo(
    MMIDRMFILE_HANDLE file_handle,
    DRM_RIGHTS_T *rights_ptr
)
{
    MMIDRMFILE_ERROR_E error = SFS_ERROR_INVALID_PARAM;

    if (PNULL == file_handle 
        || PNULL == rights_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetRightsinfo: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_449_112_2_18_2_14_36_15,(uint8*)"");
        return error;
    }

    error = DRM_GetRightsInfo(file_handle, rights_ptr);
    
    return error;
}

/*****************************************************************************/
//  Description : consume drm rights
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_ConsumeRights(
    MMIDRMFILE_HANDLE file_handle
)
{
    BOOLEAN is_ret = FALSE;
    
    if (SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_ConsumeRights: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_470_112_2_18_2_14_36_16,(uint8*)"");
        return is_ret;
    }

    is_ret = DRM_ConsumeRights(file_handle);
    
    return is_ret;
}

/*****************************************************************************/
//  Description : consume drm rights
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_ConsumeRightsByPath(
    const uint16 *file_name  
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    BOOLEAN is_ret = FALSE;
    uint16 file_name_len = 0;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_ConsumeRightsByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_493_112_2_18_2_14_36_17,(uint8*)"");
        return is_ret;
    }

    file_name_len = MMIAPICOM_Wstrlen(file_name);
    if (!MMIAPIFMM_IsDrmFileBySuffix(file_name, file_name_len))	/*lint !e605 */
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_ConsumeRightsByPath: not drm file suffix!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_500_112_2_18_2_14_36_18,(uint8*)"");
        is_ret = FALSE;
    }
    else
    {
        file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    
        if (SFS_INVALID_HANDLE != file_handle)
        {
            is_ret = MMIDRM_ConsumeRights(file_handle);
        }

        MMIAPIFMM_CloseFile(file_handle);
    }
    
    return is_ret;
}

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
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    MMIDRMFILE_ERROR_E is_ret = SFS_ERROR_INVALID_PARAM;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetRightsinfoByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_535_112_2_18_2_14_36_19,(uint8*)"");
        return is_ret;
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, access_mode, share_mode, file_attri);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_GetRightsinfo(file_handle, rights_ptr);
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    return is_ret;
}

/*****************************************************************************/
//  Description : get drm file permission by mine tye
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 会根据文件媒体类型来获取文件的permission
/*****************************************************************************/
PUBLIC DRM_PERMISSION_MODE_E MMIDRM_GetFilePermission(
    MMIDRMFILE_HANDLE file_handle       //[in] handle priority if not null
)
{
    DRM_PERMISSION_MODE_E drm_permission = DRM_PERMISSION_NONE;
    MMIFMM_FILE_TYPE_E file_type = MMIFMM_FILE_TYPE_NORMAL;

    if (PNULL == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetFilePermission: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_564_112_2_18_2_14_36_20,(uint8*)"");
        return drm_permission;
    }    

    file_type = MMIDRM_GetMediaFileType(file_handle);
    switch(file_type)
    {
    case MMIFMM_FILE_TYPE_PICTURE:       
    case MMIFMM_FILE_TYPE_EBOOK: 
#if defined MMI_VCARD_SUPPORT
    case MMIFMM_FILE_TYPE_VCARD: 
#endif
        drm_permission = DRM_PERMISSION_DISPLAY;
        break;
        
    case MMIFMM_FILE_TYPE_MUSIC :  
    case MMIFMM_FILE_TYPE_MOVIE :               
        drm_permission = DRM_PERMISSION_PLAY;
        break;                   
        
    case MMIFMM_FILE_TYPE_JAVA :               
        drm_permission = DRM_PERMISSION_EXECUTE;
        break;     
        
    default:
        break;
    }
    return drm_permission;
}

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
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    DRM_PERMISSION_MODE_E drm_permission = DRM_PERMISSION_NONE;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetFilePermissionByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_610_112_2_18_2_14_36_21,(uint8*)"");
        return drm_permission;
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, access_mode, share_mode, file_attri);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        drm_permission = MMIDRM_GetFilePermission(file_handle);
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    return drm_permission;
}

/*****************************************************************************/
//  Description : get drm rights Constraint info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_GetRightsConstraint(
    MMIDRMFILE_HANDLE file_handle,
    DRM_CONSTRAINT_T *rights_cons_ptr
)
{
    BOOLEAN is_ret = FALSE;

    if (SFS_INVALID_HANDLE == file_handle 
        || PNULL == rights_cons_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetRightsConstraint: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_640_112_2_18_2_14_36_22,(uint8*)"");
        return is_ret;
    }

    is_ret = DRM_GetRightsConstraint(file_handle, rights_cons_ptr);

    return is_ret;
}

/*****************************************************************************/
//  Description : get drm rights interval time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIDRM_GetDateIntervalTime(
    MMIDRMFILE_HANDLE file_handle,
    DRM_RIGHTS_INTERVAL_T *interval_ptr
)
{
    SFS_ERROR_E is_ret = SFS_NO_ERROR;

    if (SFS_INVALID_HANDLE == file_handle 
        || PNULL == interval_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetDateIntervalTime: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_663_112_2_18_2_14_36_23,(uint8*)"");
        return is_ret;
    }

    is_ret = DRM_GetDateTimeFormInterval(file_handle, interval_ptr);

    return is_ret;
}

/*****************************************************************************/
// Description : get time left by interval
// Global resource dependence : 
// Author:haiwu.chen
// Note: 通过interval得到剩余时间的整形值
/*****************************************************************************/
PUBLIC uint32 MMIDRM_GetTimeLeft(
    const DRM_CONSTRAINT_T *right_cons_ptr
)
{
    uint32 left_time = 0;
    uint32 cur_time = 0;

    if (PNULL == right_cons_ptr)
    {
        return 0;
    }

    //有版权，但是从未使用过
    if (0 == right_cons_ptr->start_interval ||
        0 == right_cons_ptr->end_interval)
    {
        left_time = right_cons_ptr->interval;

        return left_time;
    }

    cur_time = MMIAPICOM_GetCurTime();

    //错误情形
    if (cur_time < right_cons_ptr->start_interval
        || cur_time > right_cons_ptr->end_interval)
    {
        return 0;
    }
    
    if (cur_time - right_cons_ptr->start_interval > right_cons_ptr->interval
        || right_cons_ptr->end_interval - cur_time > right_cons_ptr->interval)
    {
        return 0;
    }

    left_time = right_cons_ptr->end_interval - cur_time;    

    return left_time;
}

/*****************************************************************************/
// Description : translator left time to year type
// Global resource dependence : 
// Author:haiwu.chen
// Note: 通过整数换算成剩余几年等，和当前时间比
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_TransLeftTimeToYearType(
    uint32 left_time,
    MMIDRMFILE_LEFT_TM_T *left_time_ptr
)
{
    uint32 day = 0;
    uint32 rem = 0;
    SCI_DATE_T sys_date = {0};
    uint32 month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint32 month_day = 30;

    if (0 == left_time
        || PNULL == left_time_ptr)
    {
        return FALSE;
    }    
    
    left_time_ptr->tm_sec = left_time % 60;
    rem = left_time / 60;
    left_time_ptr->tm_min = rem % 60;
    rem /= 60;
    left_time_ptr->tm_hour = rem %24;
    day  = rem / 24;
    
    TM_GetSysDate(&sys_date);

    if (2 == sys_date.mon && MMIAPICOM_IsLeapYear(sys_date.year))
    {
        month_day = 29;
    }
    else
    {
        month_day = month_days[sys_date.mon - 1];
    }    

    if(day < month_day)
    {
        left_time_ptr->tm_year = 0;
        left_time_ptr->tm_mon = 0;
        left_time_ptr->tm_mday = day;
    }
    else
    {
        left_time_ptr->tm_year = MMIAPICOM_DayToYear(&day) - MMICOM_DATE_MIN_YEAR;
        left_time_ptr->tm_mon = MMIAPICOM_DayToMonth(MMIAPICOM_IsLeapYear(sys_date.year),&day);
        left_time_ptr->tm_mday = day;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : get the type of drm file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC DRM_METHOD_LEVEL_E MMIDRM_GetDRMFileType(
    MMIDRMFILE_HANDLE file_handle
)
{
    DRM_METHOD_LEVEL_E method_level = DRM_LEVEL_INVALID;
    
    if (SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetDRMFileType: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_783_112_2_18_2_14_37_24,(uint8*)"");
        return method_level;
    }

    method_level = DRM_GetDRMFileType(file_handle);

    return method_level;
}

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
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    DRM_METHOD_LEVEL_E is_ret = DRM_LEVEL_INVALID;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetDRMFileTypeByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_808_112_2_18_2_14_37_25,(uint8*)"");
        return is_ret;
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, access_mode, share_mode, file_attri);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_GetDRMFileType(file_handle);
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    return is_ret;
}

/*****************************************************************************/
//  Description : get the type of drm file mime
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDRM_GetDRMFileMimeType(
    MMIDRMFILE_HANDLE file_handle
)
{
    DRM_RIGHTS_T drm_right =  {0};
    SFS_ERROR_E error = SFS_NO_ERROR;

    if (SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetDRMFileMimeType: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_837_112_2_18_2_14_37_26,(uint8*)"");
        return 0;
    }

    SCI_MEMSET(&drm_right, 0, sizeof(DRM_RIGHTS_T));
    error = DRM_GetRightsInfo(file_handle, &drm_right);
    
    //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetMediaFileType: Get drm mime type is error=%d, mime_type=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_844_112_2_18_2_14_37_27,(uint8*)"dd", error, drm_right.mime_type);

    return drm_right.mime_type;
}

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
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    uint32 is_ret = 0;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetDRMFileMimeTypeByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_865_112_2_18_2_14_37_28,(uint8*)"");
        return is_ret;
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, access_mode, share_mode, file_attri);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_GetDRMFileMimeType(file_handle);
    }
    
    MMIAPIFMM_CloseFile(file_handle);
    
    return is_ret;
}

/*****************************************************************************/
//  Description : get media file type
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIDRM_GetMediaFileType(
    MMIDRMFILE_HANDLE file_handle
)
{
    DRM_RIGHTS_T drm_right =  {0};
    MMIFMM_FILE_TYPE_E media_type = MMIFMM_FILE_TYPE_NUM;
    SFS_ERROR_E error = SFS_NO_ERROR;

    if (SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetMediaFileType: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_895_112_2_18_2_14_37_29,(uint8*)"");
        return media_type;
    }

    SCI_MEMSET(&drm_right, 0, sizeof(DRM_RIGHTS_T));
    error = DRM_GetRightsInfo(file_handle, &drm_right);

    if (SFS_NO_ERROR == error)
    {
        //根据drm_right.mime_type得到media file type
        media_type = MMIFMM_ConvertMimeType(drm_right.mime_type);
    }

    //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetMediaFileType: Get drm media type is error=%d, media_type=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_909_112_2_18_2_14_37_30,(uint8*)"dd",error, media_type);

    return media_type;
}

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
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    MMIFMM_FILE_TYPE_E is_ret = MMIFMM_FILE_TYPE_NUM;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_GetMediaFileTypeByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_930_112_2_18_2_14_37_31,(uint8*)"");
        return is_ret;
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, access_mode, share_mode, file_attri);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_GetMediaFileType(file_handle);
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    return is_ret;
}

/*****************************************************************************/
//  Description : download drm rights file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_DownloadRightsFile(
    MMIDRMFILE_HANDLE file_handle
)
{
    BOOLEAN is_ret = FALSE;
    DRM_RIGHTS_T rights_info = {0};

    if (SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_DownloadRightsFile: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_959_112_2_18_2_14_37_32,(uint8*)"");
        return FALSE;
    }

    SCI_MEMSET(&rights_info, 0, sizeof(DRM_RIGHTS_T));
    MMIDRM_GetRightsinfo(file_handle, &rights_info);

#ifdef BROWSER_SUPPORT
    {
        MMIBROWSER_ENTRY_PARAM entry_param = {0};
        MMIBROWSER_ERROR_E  entry_result = MMIBROWSER_SUCC;
            
        entry_param.type = MMIBROWSER_ACCESS_URL;
        entry_param.is_use_wifi =  FALSE;
        entry_param.dual_sys = MN_DUAL_SYS_MAX;
        entry_param.url_ptr = rights_info.rights_issuer;
        entry_param.user_agent_ptr = PNULL;

        entry_result = MMIAPIBROWSER_Entry(&entry_param);
        
        is_ret = (MMIBROWSER_SUCC == entry_result)? TRUE : FALSE;
    }
#endif

    return is_ret;
}

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
)
{
    MMIDRMFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    BOOLEAN is_ret = FALSE;

    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_DownloadRightsFileByPath: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1002_112_2_18_2_14_37_33,(uint8*)"");
        return is_ret;
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, access_mode, share_mode, file_attri);
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        is_ret = MMIDRM_DownloadRightsFile(file_handle);
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    return is_ret;
}

/*****************************************************************************/
//  Description : handle the download drm and rights files
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC DRM_ERROR_E MMIDRM_InstallFileReq(
    DRM_INSTALL_FILE_T  *file_handle_ptr
)
{
    DRM_ERROR_E result = DRM_DH_FAILED;

    if (PNULL == file_handle_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_InstallFileReq: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1030_112_2_18_2_14_37_34,(uint8*)"");
        return result;
    }

    result = DRM_InstallFileReq(file_handle_ptr);

    //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_InstallFileReq: install request is=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1036_112_2_18_2_14_37_35,(uint8*)"d", result);

    return result;
}

/*****************************************************************************/
//  Description : init agent
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: sychronization network time or save the path of rights files
/*****************************************************************************/
PUBLIC void MMIDRM_InitAgent(void)
{
    DRM_INIT_AGENT_T init_agent = {0};
    uint16 full_path_name[DRM_RIGHTS_DIR_LEN+2] = {0};
	
    MMIDRM_CreateDrmRoDir();

    SCI_MEMSET(&init_agent, 0, sizeof(DRM_INIT_AGENT_T));
    init_agent.get_network_time_func = GetNettimeSysn;	
    MMIDRM_GetDrmRoDir(full_path_name);
    init_agent.rights_path_ptr = full_path_name;
	
    //开机时清除WEB下载下来的临时文件
    MMIDRM_DelDrmTmpDir();
	
    //可以分开设置，只要有一项不空，即可以设置
    DRM_InitAgent(&init_agent);	/*lint !e605 */
    
    return;
}

/*****************************************************************************/
//  Description : delete agent
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: pair with MMIDRM_InitAgent
/*****************************************************************************/
PUBLIC void MMIDRM_DeInitAgent(void)
{
    DRM_DeInitAgent();    
}

/*****************************************************************************/
//  Description : create drm ro dir if it's not exist
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: path is only
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_CreateDrmRoDir(void)
{
    BOOLEAN is_result = FALSE;
    uint16  full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
//    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN + 2;
    BOOLEAN is_hidden = TRUE;
    BOOLEAN is_system = TRUE;

    //检查文件夹是否存在D:如果没有，则重建并设隐藏
    SCI_MEMSET(full_path_name, 0, MMIFILE_FULL_PATH_MAX_LEN+2);
    MMIAPICOM_StrToWstr((const uint8*)DDISK_RIGHTS_DIR, full_path_name);
    if(!MMIAPIFMM_IsFolderExist(full_path_name, DRM_RIGHTS_DIR_LEN))
    {
        if (MMIAPIFMM_CreateDir(full_path_name, DRM_RIGHTS_DIR_LEN))
        {
            //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_CreateDrmRoDir: create d_drm_ro path success."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1094_112_2_18_2_14_37_36,(uint8*)"");
            MMIAPIFMM_SetAttr(full_path_name, DRM_RIGHTS_DIR_LEN, is_hidden, FALSE, is_system, FALSE);
        }
        
        is_result = TRUE;
    }
    
    //同上放在e:
    SCI_MEMSET(full_path_name, 0, MMIFILE_FULL_PATH_MAX_LEN+2);
    MMIAPICOM_StrToWstr((const uint8*)EDISK_RIGHTS_DIR, full_path_name);
    if(!MMIAPIFMM_IsFolderExist(full_path_name, DRM_RIGHTS_DIR_LEN))
    {
        if (MMIAPIFMM_CreateDir(full_path_name, DRM_RIGHTS_DIR_LEN))
        {
            //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_CreateDrmRoDir: create e_drm_ro path success."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1108_112_2_18_2_14_37_37,(uint8*)"");
            MMIAPIFMM_SetAttr(full_path_name, DRM_RIGHTS_DIR_LEN, is_hidden, FALSE, is_system, FALSE);
        }
        
        is_result = TRUE;
    }

    //同上放在f:
    SCI_MEMSET(full_path_name, 0, MMIFILE_FULL_PATH_MAX_LEN+2);
    MMIAPICOM_StrToWstr((const uint8*)FDISK_RIGHTS_DIR, full_path_name);
    if(!MMIAPIFMM_IsFolderExist(full_path_name, DRM_RIGHTS_DIR_LEN))
    {
        if (MMIAPIFMM_CreateDir(full_path_name, DRM_RIGHTS_DIR_LEN))
        {
            //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_CreateDrmRoDir: create f_drm_ro path success."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1122_112_2_18_2_14_37_38,(uint8*)"");
            MMIAPIFMM_SetAttr(full_path_name, DRM_RIGHTS_DIR_LEN, is_hidden, FALSE, is_system, FALSE);
        }
        
        is_result = TRUE;
    }

    return is_result;
}


/*****************************************************************************/
//  Description : get drm ro dir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIDRM_GetDrmRoDir(uint16* full_dir)
{
    MMIFILE_DEVICE_E file_dev_type = MMI_DEVICE_SYSTEM;

    file_dev_type = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
    //if no exist valid device, use MMI_DEVICE_UDISK as default
    if( MMI_DEVICE_NUM == file_dev_type)
    {
        file_dev_type = MMI_DEVICE_UDISK;
    }
    switch (file_dev_type)
    {
        case MMI_DEVICE_UDISK:
            MMIAPICOM_StrToWstr((const uint8*)DDISK_RIGHTS_DIR, full_dir);
            break;
        case MMI_DEVICE_SDCARD:
            MMIAPICOM_StrToWstr((const uint8*)EDISK_RIGHTS_DIR, full_dir);
            break;
#ifdef DUAL_TCARD_SUPPORT
        case MMI_DEVICE_SDCARD_1:
            MMIAPICOM_StrToWstr((const uint8*)FDISK_RIGHTS_DIR, full_dir);
            break;
#endif
        default:
            MMIAPICOM_StrToWstr((const uint8*)DDISK_RIGHTS_DIR, full_dir);
            break;
    }

    return;
}

/*****************************************************************************/
//  Description : delete drm_tmp directory
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: path is only
/*****************************************************************************/
PUBLIC void MMIDRM_DelDrmTmpDir(void)
{
    wchar path_d[] = L"d:";
    wchar path_e[] = L"e:";
    wchar path_f[] = L"f:";

    MMIAPIFMM_DeleteTree(path_d, g_mmidrm_tmp_dir);
    MMIAPIFMM_DeleteTree(path_e, g_mmidrm_tmp_dir);
    MMIAPIFMM_DeleteTree(path_f, g_mmidrm_tmp_dir);
}


/*与应用相关的操作*/


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
)
{
    /*本函数实现，目前的逻辑是，只要是图片类型且有版权且非count类型，就可以设置成来电铃声*/
    BOOLEAN is_ret = FALSE;
    MMIFILE_HANDLE file_handle = 0;
    DRM_RIGHTS_T drm_rights = {0};

    if (PNULL == file_path_ptr
        || 0 == file_path_len)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_CanDrmFileSetCallRing: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1203_112_2_18_2_14_37_39,(uint8*)"");
        return FALSE;
    }
    
    file_handle = MMIAPIFMM_CreateFile(file_path_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);

    if (SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_CanDrmFileSetCallRing create file error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1211_112_2_18_2_14_38_40,(uint8*)"");
        is_ret = FALSE;
    }
    else
    {
        //is not drm file
        if (!MMIDRM_IsDRMFile(file_handle))
        {
            is_ret = TRUE;
        }
        else
        {
            //is drm file
            SCI_MEMSET(&drm_rights, 0, sizeof(drm_rights));
            MMIDRM_GetRightsinfo(file_handle, &drm_rights);
			
            if (!MMIDRM_IsRightsValid(file_handle, permission))
            {
                is_ret = FALSE;
            }
            else
            {
            
                if (file_type == MMIFMM_ConvertMimeType(drm_rights.mime_type))
                {
                    //is music, 需求要求，次数限制的不可能作为壁纸
                    if (0 != (drm_rights.constraint.type & DRM_CONSTRAINT_COUNT))
                    {
                        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_CanDrmFileSetCallRing: drm file is count cons.!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1239_112_2_18_2_14_38_41,(uint8*)"");
                        is_ret = FALSE;
                    }
                    else
                    {
                        is_ret = TRUE;
                    }
                }
                else
                {
                    //is not music
                    is_ret = FALSE;
                }
            }
        }
    }


    MMIAPIFMM_CloseFile(file_handle);

    return is_ret;
}


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNettimeSysn(DRM_NETWORK_TIME_T *nettime_ptr)
{
    //目前一直返回true，表示同步时间过了
    MMIDRM_NITZ_TYPE_T nitz_time = {0};

    if (PNULL == nettime_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] GetNettimeSysn: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1277_112_2_18_2_14_38_42,(uint8*)"");
        return FALSE;
    }

    MMIAPIDRM_GetDrmNitzTime(&nitz_time);

#if 0    //测试嘛，先置1吧
    nitz_time.is_syn_time = TRUE;
    nitz_time.sys_time_value = 0;
#endif

    nettime_ptr->is_nettime = nitz_time.is_syn_time;
    nettime_ptr->different_nettime = nitz_time.sys_time_value;
    nettime_ptr->net_time_hanle_type = nitz_time.net_time_type;
    
    return TRUE;
}
    
/*****************************************************************************/
//  Description : check rights valid
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsRightsValid(
    MMIDRMFILE_HANDLE file_handle,
    BOOLEAN care_permission,
    DRM_PERMISSION_MODE_E permission
)
{
    BOOLEAN is_ret = FALSE;    
    DRM_RIGHTS_T drm_right = {0};
    SFS_ERROR_E error = SFS_NO_ERROR;

    if (SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIDRM] IsRightsValid: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1311_112_2_18_2_14_38_43,(uint8*)"");
        return is_ret;
    }

    if (!DRM_IsDRMFile(file_handle))
    {
        //SCI_TRACE_LOW:"[MMIDRM] IsRightsValid: this isn't drm file!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1317_112_2_18_2_14_38_44,(uint8*)"");
        return is_ret;
    }    

    SCI_MEMSET(&drm_right, 0, sizeof(DRM_RIGHTS_T));
    error = DRM_GetRightsInfo(file_handle, &drm_right);

    if (SFS_NO_ERROR != error)
    {
        //SCI_TRACE_LOW:"[MMIDRM] IsRightsValid: can not get rights!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1326_112_2_18_2_14_38_45,(uint8*)"");
        return is_ret;
    }

    if (!care_permission)
    {
        //not care
        permission = drm_right.permission;
    }

    //rights file is exist and unexpired
    if (!DRM_IsRightsExpired(file_handle, permission))
    {
        is_ret = TRUE;
    }

    //SCI_TRACE_LOW:"[MMIDRM] IsRightsValid return = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_DRMFILEMGR_1342_112_2_18_2_14_38_46,(uint8*)"d", is_ret);

    return is_ret;
}


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  //#if defined(DRM_ENABLE)


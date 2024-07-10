/*************************************************************************
** File Name:      mmiota_file.c                                        *
** Author:         minghu.mao                                           *
** Date:           2009/08/10                                           *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:     This file defines the function ota file             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2009/08/10    minghu.mao       Create.                              *
*************************************************************************/
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_otapush_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_type.h"

#include "mmifmm_export.h"
#include "mmisd_export.h"
#include "guifont.h"

#include "mmiota_main.h"
#ifdef MMI_OTA_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
const wchar g_mmiota_dir_wstr[] = { 'M', 'M', 'S', 0 };

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description :check if file is exist
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIOTA_CheckFileExist(const uint8 *file)
{
    wchar    ucs2_name_arr[OTA_UCS2_NAME_MAX_LEN]        = {0};
    wchar  cmsmms_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16  cmsmms_full_path_len                         = MMIFILE_FULL_PATH_MAX_LEN;
    wchar*    device_name_ptr                            = PNULL;
    uint16    device_name_len                            = 0;
    
    if ( PNULL == file )
    {
        return FALSE;
    }
    else if(!file[0])
    {
        return FALSE;
    }
    
    GUI_GBToWstr(ucs2_name_arr, (uint8 *)file, (uint16)strlen((char *)file));
    
    /** check device exist **/
    device_name_ptr = MMIOTA_GetDeviceName(&device_name_len);
    if(device_name_ptr==PNULL)
    {
        return FALSE;
    }
    if(MMIAPIFMM_CombineFullPath(
        (wchar*)device_name_ptr, device_name_len,
        (wchar*)MMIOTA_UCS2_DIR, MMIOTA_UCS2_DIR_LEN,
        ucs2_name_arr, 
        (uint16)MMIAPICOM_Wstrlen( ucs2_name_arr), //in bytes
        cmsmms_full_path,  //[out]
        &cmsmms_full_path_len      //[out]
        ))
    {
        return MMIAPIFMM_IsFileExist(
            cmsmms_full_path,     //in
            cmsmms_full_path_len       //in
            );
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :get device name
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC wchar *MMIOTA_GetDeviceName(uint16 *device_name_len)
{
    MMIFILE_DEVICE_E cur_file_type= MMI_DEVICE_NUM;
    *device_name_len = 0;

    cur_file_type = MMIOTA_GetCurrentFFS();
    *device_name_len = MMIAPIFMM_GetDevicePathLen((cur_file_type));
    return (MMIAPIFMM_GetDevicePath(cur_file_type));
}

/*****************************************************************************/
//  Description :get current file system
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIOTA_GetCurrentFFS( void )
{
    return MMIAPIFMM_GetFirstValidDevice();
}

/*****************************************************************************/
//  Description :create file dir
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIOTA_FilecCreateDir(const char *path)
{
    wchar    ucs2_name_arr[OTA_UCS2_NAME_MAX_LEN+1]      = {0};
    wchar  cmsmms_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};//sd:/music/*.mp3, udisk:/music/*.mp3
    uint16  cmsmms_full_path_len                         = MMIFILE_FULL_PATH_MAX_LEN;
    wchar *device_name_ptr                               = PNULL;
    uint16	device_name_len                              = 0;
    
    if ( PNULL == path)
    {
        return     SFS_ERROR_INVALID_PARAM;
    }
    device_name_ptr = MMIOTA_GetDeviceName(&device_name_len);
    if(PNULL == device_name_ptr)
    {
        return SFS_ERROR_NOT_EXIST;
    }
    GUI_GBToWstr(ucs2_name_arr, (uint8 *)path, (uint16)strlen(path));    
    
    //保存到隐藏分区
    if(MMIAPIFMM_CombineFullPath(
        device_name_ptr, device_name_len,
        ucs2_name_arr, 
        (wchar)MMIAPICOM_Wstrlen( ucs2_name_arr),
        PNULL, 
        PNULL, //in bytes
        cmsmms_full_path,  //[out]
        &cmsmms_full_path_len      //[out]
        ))
        
    {
        return MMIAPIFMM_CreateDirectory((const wchar *)cmsmms_full_path);
    }
    else
    {
        return SFS_ERROR_NOT_EXIST;
    }
}

/*****************************************************************************/
//  Description :file create
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIOTA_FilecCreate(const uint8 *name, uint32 mode)
{
    wchar  ucs2_name_arr[OTA_UCS2_NAME_MAX_LEN+1]        = {0};
    wchar  cmsmms_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16  cmsmms_full_path_len                         = MMIFILE_FULL_PATH_MAX_LEN;
    wchar*    device_name_ptr                            = PNULL;
    uint16    device_name_len                            = 0;

    if (PNULL == name)
    {
        return SFS_INVALID_HANDLE;
    }    
    /** check device exist **/
    device_name_ptr = MMIOTA_GetDeviceName(&device_name_len);
    if(device_name_ptr==PNULL)
    {
        return SFS_INVALID_HANDLE;
    }
    GUI_GBToWstr(ucs2_name_arr, (uint8 *)name, (uint16)strlen((char *)name));
    
    if(MMIAPIFMM_CombineFullPath(
        (wchar*)device_name_ptr, device_name_len,
        (uint16*)MMIOTA_UCS2_DIR, MMIOTA_UCS2_DIR_LEN,
        (wchar*)ucs2_name_arr, 
        (uint16)(MMIAPICOM_Wstrlen( ucs2_name_arr)), //in bytes
        cmsmms_full_path,          //[out]
        &cmsmms_full_path_len      //[out]
        ))
    {
        return MMIAPIFMM_CreateFile(
            (const wchar *)cmsmms_full_path, 
            mode,
            NULL,    //Must be NULL,File System have not realize it 
            NULL    //Must be NULL,File System have not realize it 
            );
    }
    else
    {
        return FALSE;    
    }
}

/*****************************************************************************/
//  Description :file close
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIOTA_FileClose(MMIFILE_HANDLE handle)
{
    if ( SFS_INVALID_HANDLE == handle)
    {
        return SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        return MMIAPIFMM_CloseFile(handle);
    }
}

/*****************************************************************************/
//  Description :file write
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIOTA_FileWrite(MMIFILE_HANDLE handle, 
                                        uint8 *buf,  
                                        uint32 len, 
                                        uint32 *transmitted, 
                                        MMIFILE_OVERLAPPED_T *overlapped_ptr
                                        )
{
    if ( ( PNULL == buf) || (0 == len) || (0 == transmitted))
    {
        return  SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        return MMIAPIFMM_WriteFile(handle, (const void *)buf, len, transmitted, overlapped_ptr);
    }
}

/*****************************************************************************/
//  Description :file delete
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIOTA_FileDelete(const wchar* ucs2_name_ptr, MMIFILE_OVERLAPPED_T *overlapped_ptr)
{
    wchar  cmsmms_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16  cmsmms_full_path_len                         = MMIFILE_FULL_PATH_MAX_LEN;
    wchar*    device_name_ptr                            = PNULL;
    uint16    device_name_len                            = 0;
    
    if ( PNULL == ucs2_name_ptr)
    {
        return  SFS_ERROR_INVALID_PARAM;
    }
    
    /** check device exist **/
    device_name_ptr = MMIOTA_GetDeviceName(&device_name_len);
    if(device_name_ptr==PNULL)
    {
        return SFS_ERROR_NOT_EXIST;
    }
    if(MMIAPIFMM_CombineFullPath(
        (wchar*)device_name_ptr, device_name_len,
        (uint16*)MMIOTA_UCS2_DIR, MMIOTA_UCS2_DIR_LEN,
        (wchar*)ucs2_name_ptr, 
        (uint16)MMIAPICOM_Wstrlen( ucs2_name_ptr), //双字节
        cmsmms_full_path,  //[out]
        &cmsmms_full_path_len      //[out]
        ))
    {
        return MMIAPIFMM_DeleteFile((const wchar *)cmsmms_full_path, (MMIFILE_OVERLAPPED_T *)overlapped_ptr);
    }
    else
    {
        return SFS_ERROR_INVALID_PARAM;
    }
}

/*****************************************************************************/
//  Description :file read
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIOTA_FileRead(MMIFILE_HANDLE handle, 
                                       uint8 *buf, 
                                       uint32 len, 
                                       uint32 *transmitted, 
                                       MMIFILE_OVERLAPPED_T *overlapped_ptr
                                       )
{
    if ( (  PNULL == buf) || (0 == len) || (0 == transmitted))
    {
        return  SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        return MMIAPIFMM_ReadFile(handle, (void *)buf, len, transmitted, overlapped_ptr);
    }
}

/*****************************************************************************/
//  Description :file get length
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIOTA_FileGetLength(MMIFILE_HANDLE handle)
{
    if ( SFS_INVALID_HANDLE == handle)
    {
        return SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        return MMIAPIFMM_GetFileSize(handle);
    }
}

/*****************************************************************************/
//  Description :is enough space to save ota file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIOTA_IsEnoughSpace(uint32 size)
{
    MMIFILE_DEVICE_E dev_type = MMI_DEVICE_NUM;
    return MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, MMIOTA_GetCurrentFFS(), size, &dev_type);
}
#endif






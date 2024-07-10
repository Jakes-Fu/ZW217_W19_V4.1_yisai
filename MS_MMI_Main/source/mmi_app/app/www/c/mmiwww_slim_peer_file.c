/******************************************************************************
** File Name:      mmiwww_slim_peer_file.c                                    *
** Author:                                                                    *
** Date:           06/15/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the slim file peer for WWW   *
**                 Browser                                                    *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 06/2009        wancan.you        Create                                    *
******************************************************************************/
#define _MMIWWW_SLIM_PEER_FILE_C_  

/**--------------------------------------------------------------------------*
 **                         Dependencies                                     *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "gui_ucs2b_converter.h"
#include "mmiwww_slim_peer.h"
#include "mmifmm_export.h"
#include "mmisd_export.h"
#include "mmiwww_bookmark.h"

#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define SLIM_DEFAULT_MODE          0

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#define MMIWWW_WWW_DIR  (g_mmiwww_dir_str)
#define MMIWWW_WWW_DIR_LEN  3 //in the unit of uint8

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
const wchar g_mmiwww_dir_str[] = { 'w', 'w', 'w', 0 };//www

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get device name
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL wchar *slimFile_GetDeviceName(uint16 *device_name_len);

/*****************************************************************************/
//  Description : Check whether the path is absolute or not
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL slim_bool slimFile_IsAbsolutePath(wchar *file_ptr);

/**--------------------------------------------------------------------------*
 **                         Constant Variables                               *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get device name
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL wchar *slimFile_GetDeviceName(uint16 *device_name_len)
{
    MMIFILE_DEVICE_E file_type = MMI_DEVICE_NUM;

    *device_name_len = 0;
	
	
	for(file_type = MMI_DEVICE_SYSTEM; file_type < MMI_DEVICE_NUM; file_type++)
	{
		if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(file_type), MMIAPIFMM_GetDevicePathLen(file_type)))
		{
			*device_name_len=MMIAPIFMM_GetDevicePathLen(file_type);
			return (uint16*)MMIAPIFMM_GetDevicePath(file_type);
		}
	}

	return PNULL;
}

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC slim_bool slimFile_CombineFullPath(
                                         slim_char  *in_fname,
                                         wchar      *full_path_ptr,     //out
                                         uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                         )
{
    uint16 length = 0, w_length = 0;
    wchar *wstr_ptr = PNULL;
    wchar* device_name_ptr = PNULL;
    uint16 device_name_len = 0;
    slim_bool ret = slim_true;

    device_name_ptr = slimFile_GetDeviceName(&device_name_len);

    if (PNULL == device_name_ptr || 0 == device_name_len)
    {
        return FALSE;
    }

    length = (uint16)strlen((char *)in_fname);
    wstr_ptr = SCI_ALLOCA((length + 1) * sizeof(wchar));

    if (PNULL == wstr_ptr)
    {
        return slim_false;
    }

    SCI_MEMSET(wstr_ptr, 0x00, ((length + 1) * sizeof(wchar)));
    
    w_length = GUI_UTF8ToWstr(wstr_ptr, length, (const uint8*)in_fname, length);
    if (slimFile_IsAbsolutePath(wstr_ptr))
    {
        length = MMIAPICOM_Wstrlen(wstr_ptr);
        MMIAPICOM_Wstrncpy(full_path_ptr, wstr_ptr, MIN(length, MMIFILE_FULL_PATH_MAX_LEN));
    }
    else
    {
        if (!MMIAPIFMM_CombineFullPath(device_name_ptr, device_name_len,
                                                    (wchar*)MMIWWW_WWW_DIR, MMIWWW_WWW_DIR_LEN,
                                                    wstr_ptr, w_length,
                                                    full_path_ptr, full_path_len_ptr))
        {
            ret = slim_false;
        }
    }

    SCI_FREE(wstr_ptr);

    return ret;
}

/*****************************************************************************/
// Description : Opens file
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int slimFileOpenPeer(slim_char *in_fname, slim_int in_flag)
{
    slim_int      ret = SLIM_E_GENERIC;
    SFS_HANDLE    handle = SFS_INVALID_HANDLE;
    uint32        access, create = 0;
    slim_bool     is_exist = FALSE;
    wchar         full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};  
    uint16        full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    // 转换文件名
    if (PNULL == in_fname)
    {
        return SLIM_E_GENERIC;
    }

    PEER_DP(("wv_peer_file: slimFileOpenPeer(), file name: %s  mode=%d", in_fname, in_flag));

    if ('/' == *in_fname)
    {
        in_fname ++;
    }

    if (!slimFile_CombineFullPath(in_fname, full_path, &full_path_len))
    {
        return ret;
    }

    // 文件是否存在
    is_exist = MMIAPIFMM_IsFileExist(full_path, full_path_len);

    // 设置文件打开模式
    access = 0;
    create = 0;
    if (in_flag & SLIM_STREAM_READ)
    {
        access |= SFS_MODE_READ;
    }
    if (in_flag & SLIM_STREAM_WRITE)
    {
        access |= SFS_MODE_WRITE;
    }
    switch (in_flag & (SLIM_STREAM_CREATE | SLIM_STREAM_TRUNCATE | SLIM_STREAM_EXCLUSIVE))
    {
      case SLIM_DEFAULT_MODE:
        if (is_exist)
        {
            create = SFS_MODE_OPEN_EXISTING;
        }
        else
        {
            create = SFS_MODE_OPEN_ALWAYS;
        }        
        break;

      case SLIM_STREAM_CREATE:
        if (is_exist)
        {
            create = SFS_MODE_OPEN_ALWAYS;
        }
        else
        {
            create = SFS_MODE_CREATE_NEW;
        }
        break;        

      case SLIM_STREAM_TRUNCATE:
        handle = SFS_INVALID_HANDLE;
        if (is_exist)
        {
            create = SFS_MODE_CREATE_ALWAYS;
            PEER_DP(("wv_peer_file: slimFileOpenPeer(),access =%d,create = %d", access,create));
            handle = MMIAPIFMM_CreateFile(full_path, access|create, NULL, NULL);

            if (SFS_INVALID_HANDLE != handle) 
            {
                ret = (slim_int)handle;
            } 
            else 
            {
                ret = SLIM_E_GENERIC;
            }                
        }
        else
        {
             ret = SLIM_E_GENERIC;
        }
        return ret;

      case SLIM_STREAM_CREATE | SLIM_STREAM_TRUNCATE:
        create = SFS_MODE_CREATE_ALWAYS;
        break;

      case SLIM_STREAM_CREATE | SLIM_STREAM_EXCLUSIVE:
      case SLIM_STREAM_CREATE | SLIM_STREAM_TRUNCATE | SLIM_STREAM_EXCLUSIVE:
        create = SFS_MODE_CREATE_NEW;
        break;

      default:
        create = 0;
        break;
    }

    PEER_DP(("wv_peer_file: slimFileOpenPeer() access =%d create = %d", access, create));
    handle = MMIAPIFMM_CreateFile(full_path, access|create, NULL, NULL);

    if (SFS_INVALID_HANDLE != handle) 
    {
        PEER_DP(("wv_peer_file: slimFileOpenPeer(), succeed"));
        ret = (slim_int)handle;
    }
    else 
    {
        PEER_DP(("wv_peer_file: slimFileOpenPeer(), error!!"));
        ret = SLIM_E_GENERIC;
    }

    return ret;
}

/*****************************************************************************/
// Description : Closes file
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimFileClosePeer(slim_int in_desc)
{
    SFS_ERROR_E    error = SFS_NO_ERROR;

    PEER_DP(("slimFileClosePeer() in_desc=%d", in_desc));
    error = MMIAPIFMM_CloseFile((SFS_HANDLE)in_desc);

    if (error == SFS_NO_ERROR)
    {
        PEER_DP(("slimFileClosePeer() success"));
    }
    else
    {
        PEER_DP(("slimFileClosePeer() failed"));
    }

    return;
}

/*****************************************************************************/
// Description : Reads file
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int slimFileReadPeer(slim_int in_desc, void *out_buf, slim_int in_len)
{
    SFS_ERROR_E    error = SFS_NO_ERROR;
    uint32         bytes_read = 0;

    if (in_desc <= 0 || PNULL == out_buf)
    {
        return SLIM_E_GENERIC;
    }

    PEER_DP(("slimFileReadPeer() in_desc=%d len=%d", in_desc, in_len));

    if (0 == in_len)
    {
        return SLIM_E_OK;
    }
    error = MMIAPIFMM_ReadFile((SFS_HANDLE)in_desc, out_buf, in_len, &bytes_read, PNULL);
    if (SFS_NO_ERROR == error)
    {
        PEER_DP(("slimFileReadPeer() success bytes_read=%d", bytes_read));
        return bytes_read;
    }
    else
    {
        PEER_DP(("slimFileReadPeer() failed !!error code: %d", error));
        return SLIM_E_GENERIC;
    }
}

/*****************************************************************************/
// Description : Writes to file
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int slimFileWritePeer(slim_int in_desc, void *in_buf, slim_int in_len)
{
    uint32         bytes_written = 0;
    SFS_ERROR_E    error = SFS_NO_ERROR;
    slim_int       ret_code = 0;
    
    if (in_desc <= 0 || PNULL == in_buf)
    {
        return SLIM_E_GENERIC;
    }

    PEER_DP(("slimFileWritePeer() fp=%d len=%d", in_desc, in_len ));

    if (0 == in_len)
    {
        return ret_code;
    }

    error = MMIAPIFMM_WriteFile((SFS_HANDLE)in_desc, in_buf, in_len, &bytes_written, PNULL);
    if (SFS_NO_ERROR == error)
    {
        PEER_DP(("slimFileWritePeer() success bytes_written=d", bytes_written));

        return bytes_written;
    }
    else
    {
        PEER_DP(("slimFileWritePeer() failed !!error code: %d",error));
        return SLIM_E_GENERIC;
    }
}

/*****************************************************************************/
// Description : Deletes file
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int slimFileDeletePeer(slim_char *in_fname)
{
    SFS_ERROR_E    error = SFS_NO_ERROR;
    slim_int       ret_code = SLIM_E_GENERIC;
    wchar          full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};  
    uint16         full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    if (PNULL == in_fname)
    {
        return SLIM_E_GENERIC;
    }

    PEER_DP(("slimFileDeletePeer() fname = %s", in_fname));

    if (!slimFile_CombineFullPath(in_fname, full_path, &full_path_len))
    {
        return ret_code;
    }

    error = MMIAPIFMM_DeleteFile(full_path, PNULL);

    if (SFS_NO_ERROR != error)
    {
        PEER_DP(("slimFileDeletePeer() failed !!error_code:%d", error));
        ret_code = SLIM_E_GENERIC;
    }

    return ret_code;
}

/*****************************************************************************/
// Description : Moves file pointer
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int slimFileSeekPeer(slim_int in_desc, slim_int in_offset, slim_int in_origin)
{
    SFS_ERROR_E    error = SFS_NO_ERROR;
    uint32         how = 0;
    slim_int       ret_code = SLIM_E_OK;

    PEER_DP(("slimFileSeekPeer() fp=%d offset=%d, seek=%d",in_desc,in_offset,in_origin));
    switch (in_origin)
    {
    case SLIM_SEEK_SET:
        how = SFS_SEEK_BEGIN;
        break;

    case SLIM_SEEK_CUR:
        how = SFS_SEEK_CUR;
        break;

    case SLIM_SEEK_END:
        how = SFS_SEEK_END;
        break;

    default:
        PEER_DP(("slimFileSeekPeer() wrong seek mode"));
        return SLIM_E_GENERIC;
    }
    error = (SFS_ERROR_E)MMIAPIFMM_SetFilePointer((SFS_HANDLE)in_desc, in_offset, how);

    if (SFS_NO_ERROR != error)
    {
        PEER_DP(("slimFileSeekPeer() failed !!error_code:%d", error));
        ret_code = SLIM_E_GENERIC;
    }
    else
    {
        ret_code = MMIAPIFMM_GetFilePointer((SFS_HANDLE)in_desc, SFS_SEEK_BEGIN);
    }

    return ret_code;
}

/*****************************************************************************/
// Description : Get file Size
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int slimFileSizePeer(slim_char *in_fname)
{
    // Not used in Browser 
    return 0;
}

/*****************************************************************************/
//  Description : Clean Netftont Data Info
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 清空书签、历史记录、保存网页...
/*****************************************************************************/
PUBLIC void MMIWWW_CleanNetfrontData(void)
{
    wchar* device_name_ptr = PNULL;
    uint16 device_name_len = 0;
    wchar* path_name_ptr = PNULL;
    wchar colon[] = {':', '0'};
    uint16 colon_len = 1;

    MMIWWW_CleanBookmarkData(TRUE);

    device_name_ptr = slimFile_GetDeviceName(&device_name_len);

    if (PNULL != device_name_ptr)
    {
        path_name_ptr = SCI_ALLOCA((device_name_len + colon_len + 1)*sizeof(wchar));

        if (PNULL == path_name_ptr)
        {
            return;
        }

        SCI_MEMSET(path_name_ptr, 0x00, ((device_name_len + colon_len + 1)*sizeof(wchar)));

        MMI_WSTRNCPY(path_name_ptr,
                                (device_name_len + colon_len + 1),
                                device_name_ptr,
                                device_name_len,
                                device_name_len
                                );

        MMI_WSTRNCPY(path_name_ptr + device_name_len,
                                (colon_len + 1),
                                colon,
                                colon_len,
                                colon_len
                                );

        MMIAPIFMM_DeleteTree((const uint16 * )path_name_ptr, (const uint16 *)MMIWWW_WWW_DIR);

        SCI_FREE(path_name_ptr);
    }
}

/*****************************************************************************/
//  Description : Check whether the path is absolute or not
//  Global resource dependence : 
//  Author:wancan.you
//  Note: TRUE like "C:" "D:"
/*****************************************************************************/
LOCAL slim_bool slimFile_IsAbsolutePath(wchar *file_ptr)
{
    slim_bool ret = slim_false;

    if (MMIFILE_COLON == *(file_ptr + 1))
    {
        //Todo:more check
        ret = slim_true;
    }

    return ret;
}
#endif //#ifdef BROWSER_SUPPORT_NETFRONT

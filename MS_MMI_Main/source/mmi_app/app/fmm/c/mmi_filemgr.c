/*****************************************************************************
** File Name:      mmi_filemgr.c                                             *
** Author:                                                                   *
** Date:           08/07/2006                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mmi file manage             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2006       liqing.peng       Create
******************************************************************************/

/*lint -save -e18*/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#ifdef WIN32
#include "std_header.h"
#include "XSR_Partition.h"
#endif
#include "os_api.h"
#include "mmk_type.h"
#ifndef WIN32
#include "scm_config.h"
#endif
#include "mmifmm_export.h"
//#include "mmi_id.h"
#include "mmi_appmsg.h"
#include "mmk_msg.h"
#include "mmk_app.h"
#include "block_mem.h"
#include "mmitheme_anim.h"
#include "sfs.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_filetask.h"
#include "mmisd_export.h"
#include "mmi_text.h"
#include "mmifmm_text.h"
#include "mmifmm_image.h"
#include "mmimp3_text.h"
#include "mmi_image.h"
#include "mmipub.h"
#include "mmifilearray_export.h"
#include "mmidc_camera_image.h"
#include "scm_api.h"
#include "mmisd_internal.h"/*lint !e1752*/
#include "mmiudisk_export.h"

#ifdef MCARE_V31_SUPPORT
#include "Mmiacc_id.h"
#include "Mmifmm_id.h"
#include "McfInterface.h"
#include "Mcare_Interface.h"
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
#define MMIFILE_COPY_BUFFER_SIZE        20480   //20k
#define MMIFILE_PATH_OFFSET             4       // (设备+ :)共四个字节


    /**--------------------------------------------------------------------------*
     **                         TYPE DEFINITION                                  *
     **--------------------------------------------------------------------------*/


//用于Copy文件函数
    typedef struct
    {
        wchar                   src_full_path[MMIFILE_FULL_PATH_MAX_LEN+2];
        uint16                  src_path_len;
        wchar                   dest_full_path[MMIFILE_FULL_PATH_MAX_LEN+2];
        uint16                  dest_path_len;

        SFS_HANDLE              src_handle;      //原文件的句柄
        SFS_HANDLE              dest_handle;     //目标文件的句柄
        uint32                  copy_size;       //已经copy的文件长度
        uint32                  file_size;       //源文件大小
        uint8                   *buf_ptr;        //用于copy的100K buffer

        MMI_MESSAGE_ID_E        msg_id;          //copy结束后返回的msg_id
        MMI_WIN_ID_T            win_id;          //copy结束后要返回给的win_id
    } MMIFILE_COPY_INFO_T;




    /**--------------------------------------------------------------------------*
     **                         LOCAL VARIABLE DEFINITION                        *
     **--------------------------------------------------------------------------*/
#ifdef MMI_FILEMGR_COPY // for 217 ram cut down
    static MMIFILE_COPY_INFO_T        s_copy_info = {0};
#endif
    LOCAL BOOLEAN       s_is_sdready    = FALSE;    //SD卡是否插入并且已经注册到文件系统，即SD卡是否已经可以使用了
//LOCAL BOOLEAN       s_is_sdformate = FALSE;
    /**--------------------------------------------------------------------------*
     **                         EXTERNAL FUNCTION DECLARE                        *
     **--------------------------------------------------------------------------*/
    const wchar g_file_device_udisk[]    = { 'D', 0 };// L"D"
    const wchar g_file_device_sdcard[]   = { 'E', 0 };
    const wchar g_file_device_sdcard1[]  = { 'F', 0 };

#ifndef MMI_FMM_MINI_SUPPORT
    const wchar g_file_device_sdcard2[]  = { 'G', 0 };
    const wchar g_file_device_sdcard3[]  = { 'H', 0 };
    const wchar g_file_device_sdcard4[]  = { 'I', 0 };
#endif
    const wchar g_file_device_sysname[]  = { 'C', 0 };
    /*---------------------------------------------------------------------------*/
    /*                          LOCAL FUNCTION DECLARE                           */
    /*---------------------------------------------------------------------------*/

    /*****************************************************************************/
// 	Description: get available device
//	Global resource dependence:
//  Author: bin.ji
//	Note:
    /*****************************************************************************/
    LOCAL MMIFILE_DEVICE_E GetAvailableDevice (
        MMIFMM_STORE_TYPE_E store_type,
        MMIFILE_DEVICE_E prefer_devie,
        uint32 min_free_size
    );
//fixed by liyan.zhu for bugzilla 2760
#if 0
    /*****************************************************************************/
//  Description : store find data
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL BOOLEAN StoreFindFileData (
        SFS_FIND_DATA_T         *find_data_ptr,     //in
        uint16                  *device_ptr,        //in
        uint16                  device_len,         //in
        uint16                  *dir_ptr,           //in
        uint16                  dir_len,            //in
        MMIFILE_FILE_INFO_T     *file_info_ptr      //out
    );

    /*****************************************************************************/
//  Description : read file data asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void ReadFileDataAsynCallback (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    );
#endif
    /*****************************************************************************/
//  Description : write file data asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void WriteFileDataAsynCallback (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    );

    /*****************************************************************************/
//  Description : delete file asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void DeleteFileAsynCallback (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    );

    /*****************************************************************************/
//  Description : create file asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void CreateFileAsynCallback (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    );

    /*****************************************************************************/
//  Description : callback of write file data in copy file
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void CallBackCopyWriteDest (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    );

    /*****************************************************************************/
//  Description : callback of read file data in copy file
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void CallBackCopyReadSrc (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    );

    /*****************************************************************************/
//  Description : format device asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void FormatDeviceAsynCallBack (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    );

    /**--------------------------------------------------------------------------*
     **                         Function Implement                               *
     **--------------------------------------------------------------------------*/
    /*****************************************************************************/
//  Description : split full pathname to filepath and filename/dir
//  Global resource dependence :
//  Author:aoke.hu
//  Note: 将输入的全路径（full_path_ptr）分解成路径名（full_path_ptr）+文件名（file_name_ptr）. 自动加上结尾符
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_SplitLastName (uint16* full_path_ptr,            //[in],[out]
                                            uint16* full_path_len_ptr,         //[in],[out]
                                            uint16* file_name_ptr,             //[out]
                                            uint16* file_name_len_ptr          //[out]
                                           )
    {
        uint32 i = 0;
        uint32 k = 0;
        uint32 pos = 0;

        if (PNULL == full_path_ptr || PNULL == full_path_len_ptr || PNULL == file_name_ptr || PNULL == file_name_len_ptr)
        {
            return FALSE;
        }

        file_name_ptr[0] = 0;

        while (i < *full_path_len_ptr)
        {
            if (
                ( (uint16) ('/') != full_path_ptr[i])
                && ( (uint16) ('\\') != full_path_ptr[i])
            )
            {
                file_name_ptr[k++] = full_path_ptr[i++];
            }
            else
            {
                pos = i++;
                k = 0;
                file_name_ptr[0] = 0;
            }
        }

        full_path_ptr[pos] = 0;
        file_name_ptr[k] = 0;

        *full_path_len_ptr = pos;
        *file_name_len_ptr = k;

        return TRUE;
    }




    /*****************************************************************************/
//  Description : split full pathname to device, dir, filename
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_SplitFullPath (     //支持多级目录
        const wchar      *full_path_ptr,     //in
        uint16           full_path_len,      //in, 双字节为单位
        wchar            *device_ptr,        //out, one of device_ptr, dir_ptr and file_name_ptr should be NOT NULL
        uint16           *device_len_ptr,    //in, 双字节为单位
        wchar            *dir_ptr,           //out
        uint16           *dir_len_ptr,       //in, 双字节为单位
        wchar            *file_name_ptr,     //out
        uint16           *file_name_len_ptr  //in, 双字节为单位
    )
    {
        uint16      wchar_value = 0;
        uint16       i = 0;
        int16       j = 0;


        if (0 == full_path_len || MMIFILE_FULL_PATH_MAX_LEN < full_path_len)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_SplitFullPath ERROR! full_path_len = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_281_112_2_18_2_19_47_0, (uint8*) "d", full_path_len);
            return FALSE;
        }

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len && MMIFILE_FULL_PATH_MAX_LEN >= full_path_len);
        //SCI_ASSERT(NULL != device_ptr || NULL != dir_ptr || NULL != file_name_ptr);
        if (PNULL == full_path_ptr)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_SplitFullPath param ERROR PNULL == full_path_ptr! "
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_289_112_2_18_2_19_47_1, (uint8*) "");
            return FALSE;
        }

        if (PNULL == device_ptr && PNULL == dir_ptr && PNULL == file_name_ptr)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_SplitFullPath param ERROR! "
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_295_112_2_18_2_19_47_2, (uint8*) "");
            return FALSE;
        }

        //get Device name
        for (i = 0; i < full_path_len; i++)
        {
            wchar_value = * (full_path_ptr + i);

            if (MMIFILE_COLON == wchar_value)
            {
                break;
            }
        }

        if (0 < i && i < full_path_len)
        {
            if (NULL != device_ptr && NULL != device_len_ptr)
            {
                MMI_WSTRNCPY (device_ptr, i, full_path_ptr, i, i);

                *device_len_ptr = i;
            }
        }
        else
        {
            //":" not found
            //SCI_TRACE_LOW:"MMIAPIFMM_SplitFullPath, ':' not found!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_321_112_2_18_2_19_47_3, (uint8*) "");
            return FALSE;
        }


        //after device name, must be "\"
        i = (uint16) (i + 1);

        if (i < full_path_len)
        {
            wchar_value = * (full_path_ptr + i);
        }

        if (i >= full_path_len || MMIFILE_SLASH != wchar_value)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_SplitFullPath, '\' not found after device!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_335_112_2_18_2_19_47_4, (uint8*) "");
            return FALSE;
        }


        //get file name
        for (j = (uint16) (full_path_len - 1); j >= 0; j--)
        {
            wchar_value = * (full_path_ptr + j);

            if (MMIFILE_SLASH == wchar_value)
            {
                break;
            }
        }

        if ( (uint16) (j + 1) <= full_path_len && j > 0)
        {
            if (NULL != file_name_ptr && NULL != file_name_len_ptr)
            {
                MMI_WSTRNCPY (file_name_ptr, (uint16) (full_path_len - j - 1),
                              full_path_ptr + j + 1, (uint16) (full_path_len - j - 1), (uint16) (full_path_len - j - 1));

                *file_name_len_ptr = (uint16) (full_path_len - j - 1);
            }
        }
        else
        {
            //"\" not found
            return FALSE;
        }


        //get dir name
        if (j > (i + 1) && j < full_path_len)
        {
            if (NULL != dir_ptr && NULL != dir_len_ptr)
            {
                MMI_WSTRNCPY (dir_ptr, (uint16) (j - i - 1),
                              (full_path_ptr + i + 1), (uint16) (j - i - 1), (uint16) (j - i - 1));

                *dir_len_ptr = (uint16) (j - i - 1);
            }
        }
        else
        {
            // 需要考虑根目录, robert.lu , 2007.4.28
            if (NULL != dir_ptr && NULL != dir_len_ptr)
            {
                *dir_ptr = MMIFILE_SLASH;
                *dir_len_ptr = 1;
            }
        }


        return TRUE;
    }

    /*****************************************************************************/
// 	Description : split full path name to path,name and suffix
//	Global resource dependence :
//  Author: Jassmine
//	Note:
    /*****************************************************************************/
    PUBLIC void MMIAPIFMM_SplitFullPathExt (
        const wchar     *full_path_ptr, //in
        uint16          full_path_len,  //in
        wchar           *path_ptr,      //in/out:may PNULL
        uint16          *path_len_ptr,  //in/out:may PNULL,path_ptr不为空时,此处必须填max len
        wchar           *name_ptr,      //in/out:may PNULL,don't include suffix
        uint16          *name_len_ptr,  //in/out:may PNULL,name_ptr不为空时,此处必须填max len
        wchar           *suffix_ptr,    //in/out:may PNULL,include dot
        uint16          *suffix_len_ptr //in/out:may PNULL,suffix_ptr不为空时,此处必须填max len
    )
    {
        uint16      suffix_len = 0;
        uint16      name_len = 0;
        uint16      path_len = 0;
        int32       i = 0;
        wchar       wchar_value = 0;

        //SCI_ASSERT(PNULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        if ( (PNULL == full_path_ptr) || (0 == full_path_len))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_SplitFullPathExt param ERROR!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_417_112_2_18_2_19_47_5, (uint8*) "");
            return;
        }

        //get suffix and len
        for (i = full_path_len - 1; i >= 0; i--)
        {
            wchar_value = * (full_path_ptr + i);

            if (MMIFILE_SLASH == wchar_value)
            {
                //no suffix name
                break;
            }

            if (MMIFILE_DOT == wchar_value)
            {
                suffix_len = (uint16) (full_path_len - i);

                if (PNULL != suffix_ptr)
                {
                    //SCI_ASSERT(PNULL != suffix_len_ptr);
                    if (PNULL == suffix_len_ptr)
                    {
                        //SCI_TRACE_LOW:"MMIAPIFMM_SplitFullPathExt ERROR! PNULL == suffix_len_ptr"
                        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_438_112_2_18_2_19_47_6, (uint8*) "");
                        return;
                    }

                    MMI_WSTRNCPY (suffix_ptr, *suffix_len_ptr,
                                  full_path_ptr + i, suffix_len,
                                  suffix_len);
                }

                if (PNULL != suffix_len_ptr)
                {
                    *suffix_len_ptr = suffix_len;
                }

                break;
            }
        }

        //get name and len
        for (; i >= 0; i--)
        {
            wchar_value = * (full_path_ptr + i);

            if (MMIFILE_SLASH == wchar_value)
            {
                name_len = (uint16) (full_path_len - 1 - i - suffix_len);

                if (PNULL != name_ptr)
                {
                    //SCI_ASSERT(PNULL != name_len_ptr);
                    if (PNULL == name_len_ptr)
                    {
                        //SCI_TRACE_LOW:"MMIAPIFMM_SplitFullPathExt ERROR! PNULL != name_ptr,PNULL == name_len_ptr"
                        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_465_112_2_18_2_19_47_7, (uint8*) "");
                        return;
                    }

                    MMI_WSTRNCPY (name_ptr, *name_len_ptr,
                                  full_path_ptr + i + 1, name_len,
                                  name_len);
                }

                if (PNULL != name_len_ptr)
                {
                    *name_len_ptr = name_len;
                }

                break;
            }
        }

        //get path and len
        path_len = (uint16) (full_path_len - suffix_len - name_len);

        if (PNULL != path_ptr)
        {
            //SCI_ASSERT(PNULL != path_len_ptr);
            if (PNULL == path_len_ptr)
            {
                //SCI_TRACE_LOW:"MMIAPIFMM_SplitFullPathExt ERROR! PNULL != path_ptr,PNULL == path_len_ptr"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_487_112_2_18_2_19_47_8, (uint8*) "");
                return;
            }

            MMI_WSTRNCPY (path_ptr, *path_len_ptr,
                          full_path_ptr, path_len,
                          path_len);
        }

        if (PNULL != path_len_ptr)
        {
            *path_len_ptr = path_len;
        }
    }


    /*****************************************************************************/
//  Description : check wether file is this folder's sub file
//  Global resource dependence :
//  Author:
//  Note:
//  Return: TRUE:file_name_ptr is in path_name_ptr; FALSE:file_name_ptr isnot in path_name_ptr;
    /*****************************************************************************/
    BOOLEAN MMIFILE_FileIsInThisFolder (
        const wchar      *path_name_ptr,     //in
        uint16           path_name_len,      //in, 双字节为单位
        const wchar      *file_name_ptr,     //IN
        uint16           file_name_len  //in, 双字节为单位
    )
    {
//    uint16      wchar_value = 0;
        int16       i = 0;
//    int16       j = 0;


        if (0 == path_name_len || 0 == file_name_len)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_SplitFullPath ERROR! full_path_len = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_520_112_2_18_2_19_48_9, (uint8*) "d", path_name_len);
            return FALSE;
        }

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len && MMIFILE_FULL_PATH_MAX_LEN >= full_path_len);
        //SCI_ASSERT(NULL != device_ptr || NULL != dir_ptr || NULL != file_name_ptr);
        if (PNULL == path_name_ptr)
        {
            //SCI_TRACE_LOW:"MMIFILE_FileIsInThisFolder param ERROR PNULL == full_path_ptr! "
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_528_112_2_18_2_19_48_10, (uint8*) "");
            return FALSE;
        }

        if (PNULL == file_name_ptr)
        {
            //SCI_TRACE_LOW:"MMIFILE_FileIsInThisFolder param ERROR! "
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_534_112_2_18_2_19_48_11, (uint8*) "");
            return FALSE;
        }

        if (path_name_len >= file_name_len)
        {
            return FALSE;
        }

        //找文件的最后一个"\"
        for (i = file_name_len - 1; i > 0; i--)
        {
            if (file_name_ptr[i] == MMIFILE_SLASH)
            {
                break;
            }
        }

        if (i != path_name_len)
        {
            //file_name_ptr的上一个级文件夹和path_name_ptr长度不同
            return FALSE;
        }

        if (MMIAPICOM_Wstrncmp (path_name_ptr, file_name_ptr, path_name_len))
        {
            //file_name_ptr的上一个级文件夹和path_name_ptr不相同
            return FALSE;
        }

        return TRUE;
    }

    /*****************************************************************************/
//  Description : combin absolut path according to root, dir, name
//  Global resource dependence :
//  Author: gang.tong
//  Note:
    /*****************************************************************************/
    PUBLIC uint16 MMIAPIFMM_CombineFullPathEx (
        wchar * abs_path_ptr,   // [out] the absolute path name.
        uint32 abs_path_len,    // [in]  the buffer length of abs_path_ptr.
        MMIFILE_DEVICE_E file_dev,  // [in]  the file dev type.
        const wchar* dir_ptr,  // [in]  the directory name
        //                    BOOLEAN dir_is_ucs2,    // [in]  whether the directory is in ucs2
        const wchar* name_ptr//, // [in]  the file name
        //                    BOOLEAN name_is_ucs2    // [in]  whether the file is in ucs2
    )
    {
        uint16 abs_path_total_len = abs_path_len;
        //wchar dev_arr[MMIFILE_DEVICE_NAME_MAX_LEN+1] = {0};
        //uint16  dev_len = 0;
        wchar  *dev_path_ptr = PNULL;
        uint16  dev_path_len = 0;
        //wchar dir_arr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
        //uint16 dir_len = 0;
        //wchar name_arr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
        uint16 name_len = 0;

        //SCI_PASSERT(PNULL != abs_path_ptr,("[MMIMPEG4]: MMIAPIMPEG4_CombineFullPath() param error"));
        if (PNULL == abs_path_ptr)
        {
            //SCI_TRACE_LOW:"[MMIFMM]: MMIAPIFMM_CombineFullPathEx() param error,PNULL == abs_path_ptr!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_590_112_2_18_2_19_48_12, (uint8*) "");
            return 0;
        }

        SCI_MEMSET (abs_path_ptr, 0x00, (abs_path_len*sizeof (wchar)));


        /** convert file dev **/
//
//     if(FS_UDISK == file_dev)
//     {
//         MMIAPICOM_Wstrncpy(dev_arr, MMIFILE_DEVICE_UDISK, MMIFILE_DEVICE_UDISK_LEN);
//         dev_len = MMIFILE_DEVICE_UDISK_LEN;
//     }
//     else if(FS_MMC == file_dev)
//     {
//         MMIAPICOM_Wstrncpy(dev_arr, MMIFILE_DEVICE_SDCARD, MMIFILE_DEVICE_SDCARD_LEN);
//         dev_len = MMIFILE_DEVICE_SDCARD_LEN;
//     }
//     else
//     {
//         SCI_PASSERT(FALSE,("[MMI MPEG4]: file_dev = %d,error", file_dev));
//     }

        dev_path_ptr = MMIAPIFMM_GetDevicePath (file_dev);
        dev_path_len = MMIAPIFMM_GetDevicePathLen (file_dev);
//     dir_len = MMIAPICOM_Wstrlen( dir_ptr );
//     MMIAPICOM_Wstrncpy( dir_arr, dir_ptr, dir_len );

        /** convert name **/
        if (PNULL == name_ptr)
        {
            name_len = 0;
        }
        else
        {
            name_len = MMIAPICOM_Wstrlen (name_ptr);
        }

//     else
//     {
//         name_len = MMIAPICOM_Wstrlen( name_ptr );
//         MMIAPICOM_Wstrncpy(name_arr, name_ptr, name_len);
//     }
        //SCI_TRACE_LOW("[MMI MPEG4]: dev_len = %d, dir_len = %d, name_len = %d", dev_len, dir_len, name_len);
        if (0 == name_len)
        {
            MMIAPIFMM_CombineFullPath (dev_path_ptr, dev_path_len, dir_ptr, MMIAPICOM_Wstrlen (dir_ptr), PNULL, 0, abs_path_ptr, &abs_path_total_len);
        }
        else
        {
            MMIAPIFMM_CombineFullPath (dev_path_ptr, dev_path_len, dir_ptr, MMIAPICOM_Wstrlen (dir_ptr), name_ptr, name_len, abs_path_ptr, &abs_path_total_len);
        }

        abs_path_total_len = abs_path_total_len;

        //SCI_ASSERT(abs_path_total_len <= abs_path_len);
        if (abs_path_total_len > abs_path_len)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_CombineFullPathEx ERROR! abs_path_total_len = %d,abs_path_len = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_646_112_2_18_2_19_48_13, (uint8*) "dd", abs_path_total_len, abs_path_len);
            return abs_path_len;
        }

        return abs_path_total_len;
    }

    /*****************************************************************************/
//  Description : combine full pathname from device, file type, filename
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_CombineFullPath (   //支持多级目录
        const wchar     *device_ptr,        //in
        uint16           device_len,         //in, 双字节为单位
        const wchar     *dir_ptr,           //in
        uint16           dir_len,            //in, 双字节为单位
        const wchar     *file_name_ptr,     //in, can be NULL
        uint16           file_name_len,      //in, 双字节为单位
        wchar           *full_path_ptr,     //out
        uint16           *full_path_len_ptr  //in/out, 双字节为单位
    )
    {
        uint16      *wchar_ptr = NULL;
        uint32      j = 0;
        uint16     full_path_len = 0;
        uint16     not_cut_file_len = 0;
        uint16      merge_len = 0;

        //modified by liyan.zhu for c++ test
        //uint16      cut_len = 0;
        //uint16      suffix_name[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
        //uint16      suffix_len = 0;
        //SCI_ASSERT(NULL != device_ptr);
        //SCI_ASSERT(0 < device_len);
        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(NULL != full_path_len_ptr);
        if ( (PNULL == device_ptr) || (0 == device_len) || (PNULL == full_path_ptr) || (PNULL == full_path_len_ptr))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_CombineFullPath para error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_682_112_2_18_2_19_48_14, (uint8*) "");
            return FALSE;
        }

        full_path_len = *full_path_len_ptr;
        not_cut_file_len = file_name_len;
        //检查full_path_len长度是否足够
        merge_len = (uint16) (device_len + 1);

//    if (NULL != dir_ptr && 0 < dir_len)
//    {
//        merge_len = (uint16)(merge_len + dir_len + 1);
//    }
//
//    if (NULL != file_name_ptr && 0 < file_name_len)
//    {
//        merge_len = (uint16)(merge_len +file_name_len + 1);
//     }

//    if (full_path_len < merge_len)
//    {
//        //SCI_ASSERT(0);
//        SCI_TRACE_LOW("full path len = %d merge_len =%d",full_path_len,merge_len);
//        return FALSE;
//     }

        *full_path_len_ptr = 0;

        //copy device to full path
        if (merge_len > full_path_len)
        {
            device_len = full_path_len - 1;
        }

        *full_path_len_ptr = device_len;
        MMI_WSTRNCPY (full_path_ptr, device_len, device_ptr, device_len, device_len);

        j = device_len;
        wchar_ptr = (uint16 *) (full_path_ptr + j);
        *wchar_ptr = MMIFILE_COLON;

        j = j + 1;

        //copy dir to full path
        if (NULL != dir_ptr && 0 < dir_len)
        {
            if (! (MMIFILE_SLASH == *dir_ptr && 1 == dir_len)) //不是根目录
            {
                if (j + 1 > full_path_len)
                {
                    *full_path_len_ptr = (uint16) j;
                    return FALSE;
                }

                wchar_ptr = (uint16 *) (full_path_ptr + j);
                *wchar_ptr = MMIFILE_SLASH;
                j = j + 1;

                if (j + dir_len > full_path_len)
                {
                    dir_len = full_path_len - j;
                }

                MMI_WSTRNCPY (full_path_ptr + j, dir_len, dir_ptr, dir_len, dir_len);

                j = j + dir_len;
            }
        }


        //copy file name to full path
        if (NULL != file_name_ptr && 0 < file_name_len)
        {
            uint16      cut_len = 0;
            uint16      suffix_name[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
            uint16      suffix_len = 0;

            if (j + 1 > full_path_len)
            {
                *full_path_len_ptr = (uint16) j;
                return FALSE;
            }

            wchar_ptr = (uint16 *) (full_path_ptr + j);
            *wchar_ptr = MMIFILE_SLASH;
            j = j + 1;

            suffix_len = MMIFILE_FILE_NAME_MAX_LEN;
            MMIAPIFMM_SplitFileName (file_name_ptr, file_name_len, PNULL, PNULL, suffix_name, &suffix_len);

            if (j + file_name_len > full_path_len)
            {
                if (suffix_len + j >= full_path_len)
                {
                    //only suffix name
                    cut_len = file_name_len + j - full_path_len;
                }
                else
                {
                    file_name_len = full_path_len - j - suffix_len - 1;
                    MMI_WSTRNCPY (full_path_ptr + j, file_name_len, file_name_ptr, file_name_len, file_name_len);
                    j = j + file_name_len;

                    if (j + 1 > full_path_len)
                    {
                        *full_path_len_ptr = (uint16) j;
                        return FALSE;
                    }

                    wchar_ptr = (uint16 *) (full_path_ptr + j);
                    *wchar_ptr = MMIFILE_DOT;
                    j = j + 1;
                    cut_len = not_cut_file_len - suffix_len;
                }

                file_name_len = full_path_len - j;
            }

            MMI_WSTRNCPY (full_path_ptr + j, file_name_len, & (file_name_ptr[cut_len]), file_name_len, file_name_len);

            j = j + file_name_len;
        }


        //SCI_ASSERT(0 < j && full_path_len >= j);
        if (full_path_len < j)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_CombineFullPath error! j = %d, full_path_len = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_795_112_2_18_2_19_48_15, (uint8*) "dd", j, full_path_len);
            return FALSE;
        }

        *full_path_len_ptr = (uint16) j;

        return TRUE;
    }

    /*****************************************************************************/
//  Description : split file name to two part: name and suffix
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_SplitFileName (
        const wchar      *file_name_ptr,     //in
        uint16           file_name_len,      //in, 双字节为单位
        wchar            *name_ptr,          //out, one of name_ptr and suffix_ptr can be NULL
        uint16           *name_len_ptr,      //out, 双字节为单位
        wchar            *suffix_ptr,        //out
        uint16           *suffix_len_ptr     //in/out, 双字节为单位, in表示后缀名的最大长度
    )
    {
        BOOLEAN     result = FALSE;
        uint16      wchar_value = 0;
        uint16      suffix_len = 0;
        int32      i = 0;
        BOOLEAN     is_dot_file = FALSE ;//".mp3 ...."

        //SCI_ASSERT(NULL != file_name_ptr);
        //SCI_ASSERT(NULL != name_ptr || NULL != suffix_ptr);
        if (NULL == file_name_ptr || 0 == file_name_len)
        {
            return FALSE;
        }

        //find DOT in file_name_ptr
        for (i = file_name_len - 1; i >= 0; i--)
        {
            wchar_value = * (file_name_ptr + i);

            if (MMIFILE_SLASH == wchar_value)
            {
                //this file no suffix name
                break;
            }

            if (MMIFILE_DOT == wchar_value)
            {
                is_dot_file = TRUE;
                suffix_len = file_name_len - i - 1;
                break;
            }
        }

        // i为0， 表示没有后缀
        if (!is_dot_file)
        {
            i = file_name_len;
        }
        else if (suffix_len > *suffix_len_ptr)
        {
            //suffix name too long, short it.
            i =  file_name_len - *suffix_len_ptr - 1;
        }

        if (PNULL != suffix_len_ptr)
        {
            //reset suffix len
            *suffix_len_ptr = 0;
        }

        if ( (0 < i && i <= file_name_len) || is_dot_file)
        {
            if (NULL != name_ptr && NULL != name_len_ptr)
            {
                MMI_WSTRNCPY (name_ptr, i,
                              file_name_ptr, i, i);

                *name_len_ptr = (uint16) i;
            }

            if (NULL != suffix_ptr && NULL != suffix_len_ptr && i < file_name_len - 1)
            {
                MMI_WSTRNCPY (suffix_ptr, file_name_len - i - 1,
                              file_name_ptr + i + 1, file_name_len - i - 1, file_name_len - i - 1);

                *suffix_len_ptr = (uint16) (file_name_len - i - 1);
            }

            result = TRUE;
        }

        return result;
    }


    /*****************************************************************************/
//  Description : get file size string
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIAPIFMM_GetFileSizeString (
        uint32  size,
        wchar*  str_ptr,
        uint16  wstr_len,
        BOOLEAN is_rounding
    )
    {
        uint32 kb_size = 1 << 10;
        uint32 mb_size = 1 << 20;
        uint32 gb_size = 1 << 30;

        char *temp_str_ptr = PNULL;

        if (str_ptr == PNULL)
        {
            return;
        }

        temp_str_ptr = (char *) SCI_ALLOCA (MAX ( (wstr_len + 1), (MMIFMM_SIZE_STR_LEN + 1)));

        if (temp_str_ptr == PNULL)
        {
            return;
        }

        SCI_MEMSET (temp_str_ptr, 0x00, MAX ( (wstr_len + 1), (MMIFMM_SIZE_STR_LEN + 1)));

        if (is_rounding)
        {
            //取整，四舍五入
            if (size > kb_size  && size < mb_size)
            {
                size += kb_size / 2;
            }
            else if (size > mb_size && (size < gb_size))
            {
                size += mb_size / 2;
            }
            else if (size > gb_size)
            {
                size += gb_size / 2;
            }
        }

        /* B */
        if (size < kb_size)
        {
            sprintf (temp_str_ptr, "%ldB", (int32) size);
        }
        /* KB */
        else if (size < mb_size)
        {
            if (is_rounding)
            {
                sprintf (temp_str_ptr, "%ldK", (int32) (size / kb_size));/*lint !e737 */
            }
            else
            {
                sprintf (temp_str_ptr, "%ld.%ldK", (int32) (size / kb_size), (int32) (size % kb_size) / (kb_size / 10));/*lint !e737 !e573*/
            }
        }
        /* MB */
        else if (size < gb_size)
        {
            if (is_rounding)
            {
                sprintf (temp_str_ptr, "%ldM", (int32) (size / mb_size));/*lint !e737 */
            }
            else
            {
                sprintf (temp_str_ptr, "%ld.%ldM", (int32) (size / mb_size), (int32) (size % mb_size) / (mb_size / 10));/*lint !e737 !e573*/
            }
        }
        /* GB */
        else
        {
            if (is_rounding)
            {
                sprintf (temp_str_ptr, "%ldG", (int32) (size / gb_size));/*lint !e737 */
            }
            else
            {
                sprintf (temp_str_ptr, "%ld.%ldG", (int32) (size / gb_size), (int32) (size % gb_size) / (gb_size / 10));/*lint !e737 !e573*/
            }
        }

        MMI_STRNTOWSTR (str_ptr, wstr_len, (uint8*) temp_str_ptr, MMIFMM_SIZE_STR_LEN, MIN (strlen (temp_str_ptr), wstr_len));/*lint !e666*/

        SCI_FREE (temp_str_ptr);
    }


#ifdef WIN32

#define STL_UDISK_FS_PART "D"
#define STL_SD_DEVICE_NAME "E"

//SFS thread definition
#define   FFS_STACK_SIZE            (1024*8)  //4096  
    uint32    FFS_THREAD_ADDR[FFS_STACK_SIZE >> 2];
#define   FFS_QUEUE_NUM             10
    uint32    FFS_QUEUE_ADDR[FFS_QUEUE_NUM * SCI_QUEUE_ITEM_SIZE];

//ctrlpan thread definition
#define   CTRLPAN_STACK_SIZE            (1024*8)  //4096  
    uint32    CTRLPAN_THREAD_ADDR[CTRLPAN_STACK_SIZE >> 2];
#define   CTRLPAN_QUEUE_NUM             10
    uint32    CTRLPAN_QUEUE_ADDR[CTRLPAN_QUEUE_NUM * SCI_QUEUE_ITEM_SIZE];

//contrl panel task ID
#define CTRLPAN (50-1)

// Create static thread macro.
#define CREATE_REF_STATIC_THREAD(NAME, THREAD_ENTRY, PRIO) \
    SCI_CreateStaticThread( NAME, "T_"#NAME, THREAD_ENTRY, 0, 0, \
                            NAME##_THREAD_ADDR, NAME##_STACK_SIZE, PRIO, SCI_PREEMPT, \
                            "Q_"#NAME, NAME##_QUEUE_ADDR, NAME##_QUEUE_NUM, SCI_AUTO_START);

    extern void FFS_Task (uint32 argc, void * argv);
    extern void CtrlPan_Task (uint32 argc, void * argv);

#endif
    /*****************************************************************************/
//  Description : Init file system
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    void MMIAPIFMM_InitFileTasks (void)
    {
#ifdef WIN32
        CREATE_REF_STATIC_THREAD (FFS,     FFS_Task, (SCI_APP_PRI (31) - 1));
        CREATE_REF_STATIC_THREAD (CTRLPAN, CtrlPan_Task, (SCI_APP_PRI (31) - 1));
#endif
        //start file task
        APPFILE_CreateTask();

    }

    /*****************************************************************************/
//  Description : register device
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    SFS_ERROR_E MMIAPIFMM_RegisterDevice (
        const wchar *device_ptr,    //in
        uint16      device_len      //in, 双字节为单位
    )
    {
        //BOOLEAN                 result = FALSE;
        SFS_ERROR_E error = SFS_ERROR_NONE;
        SFS_DEVICE_FORMAT_E     format = SFS_UNKNOWN_FORMAT;
#ifdef WIN32
        BOOLEAN                 reg_ok = TRUE ;
#endif
        MMIFILE_DEVICE_E         device_type = MMI_DEVICE_UDISK;
        SCM_SLOT_NAME_E         slot_name = SCM_SLOT_0;

        //SCI_ASSERT(NULL != device_ptr);
        //SCI_ASSERT(0 < device_len);
        if ( (PNULL == device_ptr) || (0 == device_len))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_RegisterDevice param error !"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1046_112_2_18_2_19_49_16, (uint8*) "");
            return SFS_ERROR_INVALID_PARAM;
        }

        if (MMIAPIFMM_GetDeviceStatus (device_ptr, device_len)) //设备已经注册了
        {
            //result = TRUE;
            error = SFS_ERROR_NONE;
        }

//#ifdef MSDC_CARD_SUPPORT
        else //设备还未注册
        {
            device_type = MMIAPIFMM_GetDeviceTypeByPath (device_ptr, device_len);

#ifdef MSDC_CARD_SUPPORT
            if ( (device_type > MMI_DEVICE_UDISK) && (device_type < MMI_DEVICE_NUM))
            {
                //sd status not ready, may be not register,
                slot_name = MMISD_GetSLOTName (device_type);           //注册sd卡格式
#ifndef WIN32

                if (TRUE == SCM_BSD_REG (slot_name)) /*lint !e774*/  //BSD注册成功
#else
                if (reg_ok)
#endif
                {
                    error = SFS_RegisterDevice (device_ptr, &format);

                    //if(device_type == MMI_DEVICE_SDCARD_1) SCI_PASSERT(0, ("error %d device_ptr[0] 0x%x device_ptr[1] 0x%x ", error, device_ptr[0], device_ptr[1]));
                    if (SFS_ERROR_NONE == error)
                        //设备注册成功
                    {
                        s_is_sdready = TRUE;
                        return SFS_ERROR_NONE;
                    }
                    else    //设备注册失败
                    {
                        s_is_sdready = FALSE;
#ifndef WIN32
                        SCM_BSD_UNREG (slot_name);
#endif
                        return error;
                    }
                }
                else    //BSD注册失败
                {
                    s_is_sdready = FALSE;
                    return SFS_ERROR_DEVICE;    //BSD注册失败暂时借用SFS_ERROR_DEVICE
                }
            }
            else    //注册未知格式
#endif
            {
                error = SFS_RegisterDevice (device_ptr, &format);

                if (SFS_ERROR_NONE == error)
                {
                    return SFS_ERROR_NONE;
                }
                else
                {
                    return error;
                }
            }

        }

//#endif

        return error;

    }


    /*****************************************************************************/
//  Description : unregister device
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_UnRegisterDevice (
        const wchar      *device_ptr,    //in
        uint16      device_len      //in, 双字节为单位
    )
    {
        BOOLEAN                 result = FALSE;
        MMIFILE_DEVICE_E         device_type = MMI_DEVICE_UDISK;
        SCM_SLOT_NAME_E         slot_name = SCM_SLOT_0;

        //SCI_ASSERT(NULL != device_ptr);
        //SCI_ASSERT(0 < device_len);
        if ( (PNULL == device_ptr) || (0 == device_len))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_UnRegisterDevice param ERROR!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1130_112_2_18_2_19_49_17, (uint8*) "");
            return FALSE;
        }

        if (FALSE == MMIAPIFMM_GetDeviceStatus (device_ptr, device_len))
        {
            result = TRUE;
        }

//#ifdef MSDC_CARD_SUPPORT
        else //if(0 == MMIAPICOM_CompareTwoWstr( MMIFILE_DEVICE_SDCARD,MMIFILE_DEVICE_SDCARD_LEN,device_ptr,device_len))
        {
            if (SFS_ERROR_NONE == SFS_UnRegisterDevice (device_ptr))
            {
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"MMIAPIFMM_UnRegisterDevice unregister device error"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1147_112_2_18_2_19_49_18, (uint8*) "");
                result = FALSE;
            }

            device_type = MMIAPIFMM_GetDeviceTypeByPath (device_ptr, device_len);
#ifdef MSDC_CARD_SUPPORT
            if ( (device_type > MMI_DEVICE_UDISK) && (device_type < MMI_DEVICE_NUM))
            {
#ifndef WIN32
                slot_name = MMISD_GetSLOTName (device_type);
                SCM_BSD_UNREG (slot_name);
#endif
            }
#endif

            s_is_sdready = FALSE;
        }

//#endif
        return result;
    }


    /*****************************************************************************/
//  Description : format device syn
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_FormatDevice (
        const wchar      *device_ptr,    //in
        uint16      device_len,     //in, 双字节为单位
        MMIFILE_DEVICE_FORMAT_E     format  //in
    )
    {
//    BOOLEAN                 result = FALSE;
        SFS_DEVICE_FORMAT_E     fs_format = SFS_FAT12_FORMAT;
        MMIFILE_DEVICE_E         device_type = MMI_DEVICE_UDISK;
        SCM_SLOT_NAME_E         slot_name = SCM_SLOT_0;

        //SCI_ASSERT(NULL != device_ptr);
        //SCI_ASSERT(0 < device_len);
        //SCI_PASSERT(0, ("device_ptr 0x%x", *device_ptr));
        if ( (PNULL == device_ptr) || (0 == device_len))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_FormatDevice param ERROR!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1186_112_2_18_2_19_49_19, (uint8*) "");
            return FALSE;
        }

        switch (format)
        {
        case MMIFILE_FORMAT_FAT12:
            fs_format = SFS_FAT12_FORMAT;
            break;

        case MMIFILE_FORMAT_FAT16:
            fs_format = SFS_FAT16_FORMAT;
            break;

        case MMIFILE_FORMAT_FAT32:
            fs_format = SFS_FAT32_FORMAT;
            break;

        case MMIFILE_FORMAT_AUTO:
            fs_format = SFS_AUTO_FORMAT;
            break;

        default:
            //SCI_ASSERT(0);
            //SCI_TRACE_LOW:"MMIAPIFMM_FormatDevice ERROR! fs_format = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1210_112_2_18_2_19_49_20, (uint8*) "d", fs_format);
            break;
        }

        device_type = MMIAPIFMM_GetDeviceTypeByPath (device_ptr, device_len);
#ifdef MSDC_CARD_SUPPORT

        if (device_type > MMI_DEVICE_UDISK && (device_type < MMI_DEVICE_NUM))
        {
            slot_name = MMISD_GetSLOTName (device_type);

            if (FALSE == MMIAPIFMM_GetDeviceStatus (device_ptr, device_len))
            {
#ifndef WIN32

                if (FALSE == SCM_BSD_REG (slot_name))
#endif
                {
                    s_is_sdready = FALSE;
                    return FALSE;
                }
            }

            if (SFS_ERROR_NONE == SFS_Format (device_ptr, fs_format, NULL))
            {
                s_is_sdready = TRUE;
                return TRUE;
            }
            else
            {
#ifndef WIN32
                SCM_BSD_UNREG (slot_name);
#endif
                s_is_sdready = FALSE;
                return FALSE;
            }
        }
        else
#endif
        {
            if (SFS_ERROR_NONE == SFS_Format (device_ptr, fs_format, NULL))
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }

//  return result;

    }


    /*****************************************************************************/
//  Description : format device asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void FormatDeviceAsynCallBack (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    )
    {
        BOOLEAN     result = FALSE;
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;
        MMIFILE_DEVICE_E  device_type = MMI_DEVICE_UDISK;

        //SCI_ASSERT(NULL != irp_param);
        if (PNULL == irp_param)
        {
            //SCI_TRACE_LOW:"FormatDeviceAsynCallBack param error:PNULL == irp_param"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1277_112_2_18_2_19_49_21, (uint8*) "");
            return;
        }

        callback_ptr = (MMIFILE_ASYN_CALLBACK_T*) irp_param;

        //SCI_TRACE_LOW:"MMIAPIFMM_FormatDeviceAsyn: format device callback, error = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1282_112_2_18_2_19_49_22, (uint8*) "d", error);
        device_type = callback_ptr->device_type;

        if (SFS_ERROR_NONE == error)
        {
            result = TRUE;
        }

#ifdef MSDC_CARD_SUPPORT

        if (MMI_DEVICE_UDISK < device_type && device_type < MMI_DEVICE_NUM)
        {
            s_is_sdready = result;
#ifndef WIN32

            if (!s_is_sdready)
            {
                SCM_BSD_UNREG (MMISD_GetSLOTName (device_type));
            }

#endif
        }

#endif

//    MMK_PostMsg(callback_ptr->win_id, callback_ptr->msg_id, (DPARAM)&result,sizeof(result));
        MMIAPICOM_OtherTaskToMMI (callback_ptr->win_id, callback_ptr->msg_id, (DPARAM) &result, sizeof (result));
        SCI_FREE (callback_ptr);

//    MMIAPICOM_TriggerMMITask();
    }

    /*****************************************************************************/
//  Description : format device asyn
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/

    BOOLEAN MMIAPIFMM_FormatDeviceAsyn (
        const wchar                *device_ptr,        //in
        uint16                     device_len,         //in, 双字节为单位
        MMIFILE_DEVICE_FORMAT_E    format,             //in
        MMI_WIN_ID_T               win_id,             //in
        MMI_MESSAGE_ID_E           msg_id              //in
    )
    {
//    BOOLEAN                     result          = FALSE;
        SFS_ERROR_E                 error           = SFS_ERROR_NONE;
        SFS_OVERLAPPED_T            overlapped      = {0};
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr   = NULL;
        SFS_DEVICE_FORMAT_E         fs_format       = SFS_FAT12_FORMAT;
        MMIFILE_DEVICE_E             device_type = MMI_DEVICE_UDISK;
        SCM_SLOT_NAME_E             slot_name = SCM_SLOT_0;

        //SCI_ASSERT(NULL != device_ptr);
        //SCI_ASSERT(0 < device_len);
        if ( (PNULL == device_ptr) || (0 == device_len))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_FormatDeviceAsyn param ERROR!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1333_112_2_18_2_19_49_23, (uint8*) "");
            return FALSE;
        }

        switch (format)
        {
        case MMIFILE_FORMAT_FAT12:
            fs_format = SFS_FAT12_FORMAT;
            break;

        case MMIFILE_FORMAT_FAT16:
            fs_format = SFS_FAT16_FORMAT;
            break;

        case MMIFILE_FORMAT_FAT32:
            fs_format = SFS_FAT32_FORMAT;
            break;
        case MMIFILE_FORMAT_AUTO:
            fs_format = SFS_AUTO_FORMAT;
            break;
        default:
            //SCI_ASSERT(0);
            //SCI_TRACE_LOW:"MMIFMM:MMIAPIFMM_FormatDeviceAsyn format = %d is error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1355_112_2_18_2_19_49_24, (uint8*) "d", format);
            break;
        }

        device_type = MMIAPIFMM_GetDeviceTypeByPath (device_ptr, device_len);
#ifdef MCARE_V31_SUPPORT

        if (device_type == MMI_DEVICE_UDISK && McfIF_IsUsePublicDrv()) /*lint !e718 !e18*/
        {
            if (MCareV31_Exit_Confirm())
            {
                MMK_CloseWin (MMISDCARD_FORMATTING_WIN_ID);
                MMK_CloseWin (MMIFMM_FILE_WAITING_WIN_ID);
                return TRUE;
            }
        }
        else if (device_type == MMI_DEVICE_SDCARD && !McfIF_IsUsePublicDrv())
        {
            if (MCareV31_Exit_Confirm())
            {
                MMK_CloseWin (MMISDCARD_FORMATTING_WIN_ID);
                MMK_CloseWin (MMIFMM_FILE_WAITING_WIN_ID);
                return TRUE;
            }
        }

        //modified by liyan.zhu for c++ test
        //else
        //{

        //}
#endif

        callback_ptr = (MMIFILE_ASYN_CALLBACK_T *) SCI_ALLOCA (sizeof (MMIFILE_ASYN_CALLBACK_T));

        //SCI_ASSERT(NULL != callback_ptr);
        if (PNULL == callback_ptr)
        {
            //SCI_TRACE_LOW:"MMIFMM:MMIAPIFMM_FormatDeviceAsyn PNULL == callback_ptr is error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1389_112_2_18_2_19_49_25, (uint8*) "");
            return FALSE;
        }

        callback_ptr->handle = NULL;
        callback_ptr->msg_id = msg_id;
        callback_ptr->win_id = win_id;
        callback_ptr->device_type = device_type;


        overlapped.param = (uint32) callback_ptr;
        overlapped.complete_route = FormatDeviceAsynCallBack;
#ifdef MSDC_CARD_SUPPORT

        if (device_type > MMI_DEVICE_UDISK && (device_type < MMI_DEVICE_NUM))
        {
            slot_name = MMISD_GetSLOTName (device_type);

            if (FALSE == MMIAPIFMM_GetDeviceStatus (device_ptr, device_len))
            {
#ifndef WIN32

                if (FALSE == SCM_BSD_REG (slot_name))
                {
                    s_is_sdready = FALSE;
                    SCI_FREE (callback_ptr);
                    return FALSE;
                }

#endif
            }

            s_is_sdready = FALSE;
            error = SFS_Format (device_ptr, fs_format, &overlapped);

            if ( (SFS_ERROR_NONE  == error) || (SFS_ERROR_IO_PENDING == error))
            {
                return TRUE;
            }
            else
            {
#ifndef WIN32
                SCM_BSD_UNREG (slot_name);
#endif
                s_is_sdready = FALSE;
                SCI_FREE (callback_ptr);
                return FALSE;
            }
        }
        else
#endif
        {
            error = SFS_Format (device_ptr, fs_format, &overlapped);

            if ( (SFS_ERROR_NONE  == error) || (SFS_ERROR_IO_PENDING == error))
            {
                return TRUE;
            }
            else
            {
                SCI_FREE (callback_ptr);
                return FALSE;
            }
        }


    }


    /*****************************************************************************/
//  Description : get device status, ok or not ok
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_GetDeviceStatus (
        const wchar *device_ptr,    //in
        uint16      device_len      //in, 双字节为单位
    )
    {

        BOOLEAN     result = FALSE;

        //SCI_ASSERT(NULL != device_ptr);
        //SCI_ASSERT(0 < device_len);
        if ( (PNULL == device_ptr) || (0 == device_len))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_GetDeviceStatus param ERROR!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1468_112_2_18_2_19_49_26, (uint8*) "");
            return FALSE;
        }

        if (SFS_ERROR_NONE == SFS_GetDeviceStatus (device_ptr))
        {

            result = TRUE;
        }

        return result;

    }

    /*****************************************************************************/
//  Description : set sd status
//  Global resource dependence :
//  Author: louis.wei
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_GetDeviceTypeStatus (MMIFILE_DEVICE_E memory_type)
    {
        BOOLEAN device_status = FALSE;

        //UDisk
        if ( (MMI_DEVICE_UDISK == memory_type || MMI_DEVICE_SYSTEM == memory_type) && MMIAPIUDISK_UdiskIsRun())
        {
            return device_status;
        }

        //SD
        if (MMIAPISD_IsCardLogOn() && MMI_DEVICE_SDCARD == memory_type) //cr244919
        {
            return device_status;
        }

        device_status =  MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (memory_type), MMIAPIFMM_GetDevicePathLen (memory_type));

        return device_status;
    }

    /*****************************************************************************/
//  Description : create directory
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_CreateDir (
        const wchar      *full_path_ptr,     //in
        uint16           full_path_len       //in, 双字节为单位
    )
    {
        BOOLEAN         result = FALSE;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        if ( (PNULL == full_path_ptr) || (0 == full_path_len))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_CreateDir param ERROR!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1522_112_2_18_2_19_50_27, (uint8*) "");
            return FALSE;
        }

        if (SFS_ERROR_NONE == SFS_CreateDirectory (full_path_ptr))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_CreateDir: create dir success"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1528_112_2_18_2_19_50_28, (uint8*) "");
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_CreateDir: create dir fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1533_112_2_18_2_19_50_29, (uint8*) "");
        }

        return result;
    }

    /*****************************************************************************/
//  Description : create directory with RO or HIDDEN attr
//  Global resource dependence :
//  Author:aoke.hu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_SetAttr (
        const wchar       *full_path_ptr,     //in
        uint16            full_path_len,       //in, 双字节为单位
        BOOLEAN           is_hidden,         //in, 是否为隐藏属性
        BOOLEAN           is_ro,             //in, 是否为只读属性
        BOOLEAN           is_system,         //in, 是否为系统属性
        BOOLEAN           is_arch           //in, 是否为存档属性
    )
    {
        wchar           *file_name_ptr = PNULL;
        wchar           *path_name_ptr = PNULL;
        uint16          file_name_len = full_path_len;
        wchar           attr[MMIFILE_FILE_ATTR_LEN] = {0};

        if (NULL == full_path_ptr || 0 == full_path_len)
        {
            return FALSE;
        }

        if (is_hidden || is_ro || is_system || is_arch)
        {
            file_name_ptr = (wchar *) SCI_ALLOCA ( (file_name_len + 1) * sizeof (wchar));
            path_name_ptr = (wchar *) SCI_ALLOCA ( (full_path_len + 1) * sizeof (wchar));

            if (file_name_ptr == PNULL || path_name_ptr == PNULL)
            {
                //SCI_TRACE_LOW:"[MMIFILE] SetAttr NO memory !"
                SCI_FREE (file_name_ptr);
                SCI_FREE (path_name_ptr);
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1568_112_2_18_2_19_50_30, (uint8*) "");
                return FALSE;
            }

            SCI_MEMSET (file_name_ptr, 0x00, ( (file_name_len + 1) *sizeof (wchar)));
            SCI_MEMSET (path_name_ptr, 0x00, ( (full_path_len + 1) *sizeof (wchar)));
            MMIAPICOM_Wstrncpy (path_name_ptr, full_path_ptr, full_path_len);
            MMIAPIFMM_SplitLastName (path_name_ptr, &full_path_len, file_name_ptr, &file_name_len);
            MMIAPICOM_Wstrcpy (attr, L"/SA:");

            if (is_hidden)
            {
                MMIAPICOM_Wstrcat (attr, L"+H");
            }

            if (is_ro)
            {
                MMIAPICOM_Wstrcat (attr, L"+R");
            }

            if (is_system)
            {
                MMIAPICOM_Wstrcat (attr, L"+S");
            }

            if (is_arch)
            {
                MMIAPICOM_Wstrcat (attr, L"+A");
            }

            SFS_SetAttrAsyn (path_name_ptr, file_name_ptr, attr, 0);
            SCI_FREE (file_name_ptr);
            SCI_FREE (path_name_ptr);
        }

        return TRUE;
    }


    /*****************************************************************************/
//  Description : get file information
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_GetFileInfo (
        const wchar             *full_path_ptr,     //in
        uint16                  full_path_len,      //in, 双字节为单位
        uint32                  *file_size_ptr,     //out
        SFS_DATE_T				*modify_date,
        SFS_TIME_T				*modify_time
    )
    {
        BOOLEAN         result = FALSE;
        SFS_HANDLE      sfs_handle = 0;
        SFS_FIND_DATA_T find_data = {0};

        //SCI_ASSERT(NULL != full_path_ptr);
        if (PNULL == full_path_ptr)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_GetFileInfo ERROR:PNULL == full_path_ptr"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1620_112_2_18_2_19_50_31, (uint8*) "");
            return FALSE;
        }

        if (0 < full_path_len)
        {
            sfs_handle = MMIAPIFMM_FindFirstFile (full_path_ptr, full_path_len, &find_data);

            if (SFS_INVALID_HANDLE != sfs_handle)
            {
                if (! (SFS_ATTR_DIR&find_data.attr))
                {
                    if (NULL != file_size_ptr)
                    {
                        *file_size_ptr = find_data.length;
                    }

                    if (NULL != modify_date)
                    {
                        *modify_date = find_data.modify_Date;
                    }

                    if (NULL != modify_time)
                    {
                        *modify_time = find_data.modify_time;
                    }

                    result = TRUE;
                }
            }

            SFS_FindClose (sfs_handle);
        }

        return result;
    }

    /*****************************************************************************/
//  Description : check whether file is exist
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_IsFileExist (
        const wchar *full_path_ptr,     //in
        uint16       full_path_len       //in, 双字节为单位
    )
    {
        return MMIFILE_IsFileExist (full_path_ptr, full_path_len);
    }


    /*****************************************************************************/
//  Description : check whether folder is exist
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_IsFolderExist (
        const wchar     *full_path_ptr,     //in
        uint16              full_path_len       //in, 双字节为单位
    )
    {
        return MMIFILE_IsFolderExist (full_path_ptr, full_path_len);
    }

    /*****************************************************************************/
//  Description : check whether file is exist
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIFMM_IsFilesInFolder (
        const wchar *full_path_ptr,     //in
        uint16       full_path_len ,      //in, 双字节为单位
        const wchar *filter//IN:查询字符串
    )
    {
        BOOLEAN         result = FALSE;
        SFS_HANDLE      sfs_handle = 0;
        SFS_FIND_DATA_T find_data = {0};
        uint16          ctlStr[] = L"/A:-NF-H-S";

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        if (PNULL == full_path_ptr || PNULL == filter)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_IsFileExist error:PNULL == full_path_ptr!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1700_112_2_18_2_19_50_32, (uint8*) "");
            return FALSE;
        }

        if (//0 == full_path_len ||
            0 == MMIAPICOM_Wstrlen (full_path_ptr))
        {
            result = FALSE;
        }
        else
        {
            if (SFS_NO_ERROR == SFS_FindFirstFileCtrl (full_path_ptr, filter, ctlStr, &sfs_handle, &find_data, PNULL))
            {
                result = TRUE;
            }

            if (NULL != sfs_handle)
            {
                SFS_FindClose (sfs_handle);
            }
        }

        //SCI_TRACE_LOW:"MMIFMM_IsFilesInFolder: result = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1721_112_2_18_2_19_50_33, (uint8*) "d", result);

        return result;
    }



    /*****************************************************************************/
//  Description : get device free space
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_GetDeviceFreeSpace (
        const wchar *device_ptr,        //in
        uint16      device_len,         //in, 双字节为单位
        uint32      *free_high_ptr,     //out
        uint32      *free_low_ptr       //out
    )
    {
        BOOLEAN     result = FALSE;

        //SCI_ASSERT(NULL != device_ptr);
        //SCI_ASSERT(0 < device_len && MMIFILE_DEVICE_NAME_MAX_LEN >= device_len);
        //SCI_ASSERT(NULL != free_high_ptr);
        //SCI_ASSERT(NULL != free_low_ptr);
        if ( (PNULL == device_ptr) || (0 == device_len) || (MMIFILE_DEVICE_NAME_MAX_LEN < device_len)
                || (PNULL == free_high_ptr) || (PNULL == free_low_ptr))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_GetDeviceFreeSpace param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1748_112_2_18_2_19_50_34, (uint8*) "");
            return FALSE;
        }

        if (SFS_ERROR_NONE == SFS_GetDeviceFreeSpace (device_ptr, free_high_ptr, free_low_ptr))
        {
            result = TRUE;
        }

        return result;
    }


    /*****************************************************************************/
//  Description : get device used space
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_GetDeviceUsedSpace (
        const wchar *device_ptr,        //in
        uint16      device_len,         //in, 双字节为单位
        uint32      *used_high_ptr,     //out
        uint32      *used_low_ptr       //out
    )
    {
        BOOLEAN     result = FALSE;

        //SCI_ASSERT(NULL != device_ptr);
        //SCI_ASSERT(0 < device_len && MMIFILE_DEVICE_NAME_MAX_LEN >= device_len);
        //SCI_ASSERT(NULL != used_high_ptr);
        //SCI_ASSERT(NULL != used_low_ptr);
        if ( (PNULL == device_ptr) || (0 == device_len) || (MMIFILE_DEVICE_NAME_MAX_LEN < device_len)
                || (PNULL == used_high_ptr) || (PNULL == used_low_ptr))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_GetDeviceUsedSpace error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1781_112_2_18_2_19_50_35, (uint8*) "");
            return FALSE;
        }

        if (SFS_ERROR_NONE == SFS_GetDeviceUsedSpace (device_ptr, used_high_ptr, used_low_ptr))
        {
            result = TRUE;
        }

        return result;
    }


    /*****************************************************************************/
//  Description : rename file
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_RenameFile (
        const wchar     *full_path_ptr,     //in
        uint16          full_path_len,      //in, 双字节为单位
        wchar           *new_file_name_ptr, //in
        uint16          new_file_name_len   //in, 双字节为单位
    )
    {
        BOOLEAN         result = FALSE;
        wchar           device_name[MMIFILE_DEVICE_NAME_MAX_LEN+2] = {0};
        uint16          device_name_len = 0;
        wchar           dir_name[MMIFILE_DIR_NAME_MAX_LEN+2] = {0};
        uint16          dir_name_len = 0;
        wchar           new_full_path[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
        uint16          new_full_path_len = MMIFILE_FULL_PATH_MAX_LEN + 2;

        if (NULL == full_path_ptr
                || (0 == full_path_len || MMIFILE_FULL_PATH_MAX_LEN < full_path_len)
                || (NULL == new_file_name_ptr)
                || (0 == new_file_name_len || MMIFILE_FILE_NAME_MAX_LEN < new_file_name_len)
           )
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_RenameFile full_path_len %d new_file_name_len %d !!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1819_112_2_18_2_19_50_36, (uint8*) "dd", full_path_len, new_file_name_len);
            return FALSE;
        }

        if (MMIAPIFMM_SplitFullPath (full_path_ptr, full_path_len,
                                     device_name, &device_name_len,
                                     dir_name, &dir_name_len,
                                     NULL, NULL))
        {
            if (MMIAPIFMM_CombineFullPath (device_name, device_name_len,
                                           dir_name, dir_name_len,
                                           new_file_name_ptr, new_file_name_len,
                                           new_full_path, &new_full_path_len))
            {
                if (SFS_ERROR_NONE == SFS_RenameFile (full_path_ptr, new_full_path, NULL))
                {
                    result = TRUE;
                }
            }
        }

        return result;
    }
    /*****************************************************************************/
//  Description : set device label
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_SetDeviceLabel (
        const wchar   *device_path,     //in
        char          *label_name,      //in
        uint8          label_name_len
    )
    {
        BOOLEAN         result = FALSE;

        if (PNULL == device_path || (PNULL == label_name)
                || (0 == label_name_len || label_name_len > MMIFILE_NEW_DEVICE_NAME_MAX_LEN*3)
           )
        {
            SCI_TRACE_LOW ("MMIAPIFMM_SetDeviceLabel MMIAPIFMM_RenameDevice param error!");
            return FALSE;
        }

        if (SFS_ERROR_NONE == SFS_SetDeviceLabel (device_path, (uint8 *) label_name, PNULL))
        {
            result = TRUE;
        }

        return result;
    }
    /*****************************************************************************/
//  Description : get device label
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_GetDeviceLabel (
        const wchar   *device_path,     //in
        char          *label_name       //out
    )
    {
        BOOLEAN         result = FALSE;

        if (PNULL == device_path || (PNULL == label_name))
        {
            SCI_TRACE_LOW ("MMIAPIFMM_GetDeviceName MMIAPIFMM_RenameDevice param error!");
            return FALSE;
        }

        if (SFS_ERROR_NONE == SFS_GetDeviceLabel (device_path, (uint8 *) label_name, PNULL))
        {
            result = TRUE;
        }

        return result;
    }
    /*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:liqing.peng
//  Note: 若没有匹配到合适的DEV，返回无效的DEV， caller需要对返回值进行有效性判断
    /*****************************************************************************/
    MMIFILE_DEVICE_E MMIAPIFMM_GetDeviceTypeByPath (
        const wchar *device_ptr,    //in
        uint16    device_len      //in, 双字节为单位
    )
    {
        MMIFILE_DEVICE_E  device_type = MMI_DEVICE_NUM;

        if (device_ptr != PNULL && (device_len > 0))
        {
            if (device_ptr[device_len -1] == MMIFILE_SLASH)
            {
                //减"\\"
                device_len--;
            }

            if ( (device_len > 0) && (device_ptr[device_len -1] == MMIFILE_COLON))
            {
                //减":"
                device_len--;
            }

            for (device_type = MMI_DEVICE_SYSTEM; device_type < MMI_DEVICE_NUM; device_type++)
            {
                if (!MMIAPICOM_CompareTwoWstrExt (device_ptr,
                                                  device_len,
                                                  MMIAPIFMM_GetDevicePath (device_type),
                                                  MMIAPIFMM_GetDevicePathLen (device_type),
                                                  FALSE))
                {
                    break;
                }
            }
        }

        return device_type;

    }
//fixed by liyan.zhu for bugzilla 2760
#if 0
    /*****************************************************************************/
//  Description : store find data
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL BOOLEAN StoreFindFileData (
        SFS_FIND_DATA_T         *find_data_ptr,     //in
        uint16                  *device_ptr,        //in
        uint16                  device_len,         //in
        uint16                  *dir_ptr,           //in
        uint16                  dir_len,            //in
        MMIFILE_FILE_INFO_T     *file_info_ptr      //out
    )
    {
        BOOLEAN     result = FALSE;
        uint16      max_name_len = 0;

        //SCI_ASSERT(NULL != find_data_ptr);
        //SCI_ASSERT(NULL != device_ptr);
        //SCI_ASSERT(0 < device_len);
        //SCI_ASSERT(NULL != dir_ptr);
        //SCI_ASSERT(0 < dir_len);
        //SCI_ASSERT(NULL != file_info_ptr);
        if ( (PNULL == find_data_ptr) || (PNULL == device_ptr) || (0 == device_len)
                || (PNULL == dir_ptr) || (0 == dir_len) || (PNULL == file_info_ptr))
        {
            //SCI_TRACE_LOW:"StoreFindFileData param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1907_112_2_18_2_19_50_37, (uint8*) "");
            return FALSE;
        }

        file_info_ptr->create_time = MMIAPICOM_TansferSFSTime (find_data_ptr->modify_Date, find_data_ptr->modify_time);
        file_info_ptr->file_size = find_data_ptr->length;

        file_info_ptr->file_name_len = (uint16) MMIAPICOM_Wstrlen (find_data_ptr->name);

        //max_name_len = MMIFILE_FILE_NAME_MAX_LEN - d:\Music\ length
        max_name_len = (uint16) (MMIFILE_FILE_NAME_MAX_LEN - device_len - 2 - dir_len - 1);

        if (max_name_len >= file_info_ptr->file_name_len)
        {
            file_info_ptr->file_name_len = file_info_ptr->file_name_len;

            MMI_WSTRNCPY (file_info_ptr->file_name, MMIFILE_FILE_NAME_MAX_LEN,
                          find_data_ptr->name, file_info_ptr->file_name_len, file_info_ptr->file_name_len);

            file_info_ptr->device_name_len = device_len;
            MMI_WSTRNCPY (file_info_ptr->device_name, MMIFILE_DEVICE_NAME_MAX_LEN,
                          device_ptr, device_len, device_len);

            file_info_ptr->dir_name_len = dir_len;
            MMI_WSTRNCPY (file_info_ptr->dir_name, MMIFILE_DIR_NAME_MAX_LEN,
                          dir_ptr, dir_len, dir_len);

            result = TRUE;
        }

        return result;
    }

    /*****************************************************************************/
//  Description : find files in directory by key word
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    uint16 MMIFMM_FindAndGetFiles (        //return find files number
        const wchar             *full_path_ptr,     //in
        uint16                  full_path_len,      //in, 双字节为单位
        MMIFILE_FILE_INFO_T     file_list[],        //out
        uint16                  file_list_size      //in, file_list buf size
    )
    {
        wchar           device_name[MMIFILE_DEVICE_NAME_MAX_LEN+2] = {0};
        uint16          device_name_len = 0;
        wchar           dir_name[MMIFILE_DIR_NAME_MAX_LEN+2] = {0};
        uint16          dir_name_len = 0;
        SFS_HANDLE      sfs_handle = 0;
        SFS_FIND_DATA_T find_data = {0};
        uint16          index = 0;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        //SCI_ASSERT(NULL != file_list);
        //SCI_ASSERT(0 < file_list_size);
        if ( (PNULL == full_path_ptr) || (0 == full_path_len) || (PNULL == file_list) || (0 == file_list_size))
        {
            //SCI_TRACE_LOW:"MMIFMM_FindAndGetFiles param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1964_112_2_18_2_19_50_38, (uint8*) "");
            return index;
        }

        if (MMIAPIFMM_SplitFullPath (full_path_ptr, full_path_len,
                                     device_name, &device_name_len,
                                     dir_name, &dir_name_len,
                                     NULL, NULL))
        {

            sfs_handle = MMIAPIFMM_FindFirstFile (full_path_ptr, full_path_len, &find_data);

            if (SFS_INVALID_HANDLE != sfs_handle)
            {
                //SCI_TRACE_LOW:"MMIFMM_FindAndGetFiles: find first success"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_1978_112_2_18_2_19_50_39, (uint8*) "");

                if (! (SFS_ATTR_DIR&find_data.attr))
                {
                    if (StoreFindFileData (&find_data,
                                           device_name,
                                           device_name_len,
                                           dir_name,
                                           dir_name_len,
                                           &file_list[index]))
                    {
                        index++;
                    }
                }

                SCI_MEMSET (&find_data, 0, sizeof (find_data));

                while (index < file_list_size && SFS_ERROR_NONE == SFS_FindNextFile (sfs_handle, &find_data))
                {
                    if (! (SFS_ATTR_DIR&find_data.attr))
                    {
                        if (StoreFindFileData (&find_data,
                                               device_name,
                                               device_name_len,
                                               dir_name,
                                               dir_name_len,
                                               &file_list[index]))
                        {
                            index++;
                        }
                    }

                    SCI_MEMSET (&find_data, 0, sizeof (find_data));
                }
            }

            SFS_FindClose (sfs_handle);
        }

        //SCI_TRACE_LOW:"MMIFMM_FindAndGetFiles: find file number = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2016_112_2_18_2_19_51_40, (uint8*) "d", index);

        return index;
    }
#endif

    /*****************************************************************************/
//  Description : read file data syn, file must exist
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_ReadFilesDataSyn (
        const wchar             *full_path_ptr,     //in
        uint16                  full_path_len,      //in, 双字节为单位
        uint8                   *buf_ptr,           //out
        uint32                  to_read_size        //in
    )
    {
        BOOLEAN         result = FALSE;
        SFS_HANDLE      sfs_handle = 0;
        uint32          read_size = 0;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        //SCI_ASSERT(NULL != buf_ptr);
        if ( (PNULL == full_path_ptr) || (0 == full_path_len) || (PNULL == buf_ptr))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataSyn param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2042_112_2_18_2_19_51_41, (uint8*) "");
            return FALSE;
        }

        if (0 == to_read_size)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataSyn: read file 0"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2048_112_2_18_2_19_51_42, (uint8*) "");
            return TRUE;
        }

        sfs_handle = SFS_CreateFile (full_path_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

        if (SFS_INVALID_HANDLE != sfs_handle)
        {
            if (SFS_ERROR_NONE == SFS_ReadFile (sfs_handle, buf_ptr, to_read_size, &read_size, NULL))
            {
                result = TRUE;
            }
        }

        SFS_CloseFile (sfs_handle);

        if (result)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataSyn: return success"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2066_112_2_18_2_19_51_43, (uint8*) "");
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataSyn: return fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2070_112_2_18_2_19_51_44, (uint8*) "");
        }

        return result;
    }

//fixed by liyan.zhu for bugzilla 2760
#if 0
    /*****************************************************************************/
//  Description : read file data syn with file handle
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIFILE_ReadFilesDataSynEx (
        MMIFILE_HANDLE          file_handle,        //in
        uint8                   *buf_ptr,           //out
        uint32                  to_read_size        //in
    )
    {
        BOOLEAN         result = FALSE;
        SFS_HANDLE      sfs_handle = file_handle;
        uint32          read_size = 0;

        if (NULL == buf_ptr)
        {
            return result;
        }

        if (0 == to_read_size)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataAsynEx: read file 0"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2097_112_2_18_2_19_51_45, (uint8*) "");
            return TRUE;
        }

        if (SFS_INVALID_HANDLE == sfs_handle)
        {
            return FALSE;
        }

        if (SFS_ERROR_NONE == SFS_ReadFile (sfs_handle, buf_ptr, to_read_size, &read_size, NULL))
        {
            result = TRUE;
        }

        if (result)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataAsynEx: return success"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2113_112_2_18_2_19_51_46, (uint8*) "");
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataAsynEx: return fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2117_112_2_18_2_19_51_47, (uint8*) "");
        }

        return result;
    }
#endif

    /*****************************************************************************/
//  Description : write file data syn, create file always and will delete file
//                existing
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_WriteFilesDataSyn (
        const uint16            *full_path_ptr,     //in
        uint16                  full_path_len,      //in, 双字节为单位
        const uint8             *buf_ptr,           //in
        uint32                  to_write_size       //in
    )
    {
        BOOLEAN         result = FALSE;
        SFS_HANDLE      sfs_handle = 0;
        uint32          write_size = to_write_size;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(NULL != buf_ptr);
        //SCI_ASSERT(0 < to_write_size);
        if ( (PNULL == full_path_ptr) || (PNULL == buf_ptr) || (0 == to_write_size))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_WriteFilesDataSyn param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2144_112_2_18_2_19_51_48, (uint8*) "");
            return FALSE;
        }

        sfs_handle = SFS_CreateFile (full_path_ptr, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/

        if (SFS_INVALID_HANDLE != sfs_handle)
        {
            if (SFS_ERROR_NONE == SFS_WriteFile (sfs_handle, (const void *) buf_ptr, to_write_size, &write_size, NULL))
            {
                result = TRUE;
            }
        }

        SFS_CloseFile (sfs_handle);

        return result;
    }


    /*****************************************************************************/
//  Description : delete file syn
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_DeleteFileSyn (
        const wchar             *full_path_ptr,     //in
        uint16                  full_path_len       //in, 双字节为单位
    )
    {
        BOOLEAN     result = FALSE;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        if ( (PNULL == full_path_ptr) || (0 == full_path_len))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_DeleteFileSyn param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2179_112_2_18_2_19_51_49, (uint8*) "");
            return FALSE;
        }

        if (SFS_ERROR_NONE == SFS_DeleteFile (full_path_ptr, NULL))
        {
            result = TRUE;
        }

        return result;
    }

    /*****************************************************************************/
//  Description : copy file
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_CopyFile (const wchar *sour_filename, const wchar * dest_filename)
    {
        MMIFILE_ERROR_E copy_ret = SFS_NO_ERROR;
        MMIFILE_HANDLE  sour_file_handle = PNULL;
        MMIFILE_HANDLE  dest_file_handle = PNULL;
        uint8           *data_ptr = PNULL;
        uint32          copy_size = MMIFILE_COPY_BUFFER_SIZE;

        if (PNULL == sour_filename || PNULL == dest_filename)
        {
            //SCI_TRACE_LOW:"[MMIFMM] MMIAPIFMM_CopyFile invalid file name !"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2205_112_2_18_2_19_51_50, (uint8*) "");
            return SFS_ERROR_INVALID_PARAM;
        }

        data_ptr = (uint8 *) SCI_ALLOCA (MMIFILE_COPY_BUFFER_SIZE);

        if (PNULL == data_ptr)
        {
            //SCI_TRACE_LOW:"[MMIFMM] MMIAPIFMM_CopyFile no memory!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2212_112_2_18_2_19_51_51, (uint8*) "");
            return SFS_ERROR_NO_SPACE;
        }

        sour_file_handle = MMIAPIFMM_CreateFile (sour_filename,
                           SFS_MODE_READ | SFS_MODE_OPEN_EXISTING,/*lint !e655*/
                           0,
                           0);
        dest_file_handle = MMIAPIFMM_CreateFile (dest_filename,
                           SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS,/*lint !e655*/
                           0,
                           0);

        if (SFS_INVALID_HANDLE == sour_file_handle || SFS_INVALID_HANDLE == dest_file_handle)
        {
            //SCI_TRACE_LOW:"[MMIFMM] MMIAPIFMM_CopyFile open file error !"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2226_112_2_18_2_19_51_52, (uint8*) "");
            SCI_FREE (data_ptr);
            MMIAPIFMM_CloseFile (sour_file_handle);
            MMIAPIFMM_CloseFile (dest_file_handle);
            return SFS_ERROR_ACCESS;
        }

        //start copy
        //first read source file to data_ptr
        //copy_size = MMIAPIFMM_GetFileSize(sour_file_handle);
        while (copy_size >= MMIFILE_COPY_BUFFER_SIZE)
        {
            copy_ret = MMIAPIFMM_ReadFile (sour_file_handle, data_ptr, MMIFILE_COPY_BUFFER_SIZE, &copy_size, PNULL);

            if (SFS_ERROR_NONE != copy_ret)
            {
                //SCI_TRACE_LOW:"[MMIFILE] MMIAPIFMM_CopyFile MMIAPIFMM_ReadFile copy_ret %d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2240_112_2_18_2_19_51_53, (uint8*) "d", copy_ret);
                break;
            }

            copy_ret = MMIAPIFMM_WriteFile (dest_file_handle, data_ptr, copy_size, &copy_size, PNULL);

            if (SFS_ERROR_NONE != copy_ret)
            {
                //SCI_TRACE_LOW:"[MMIFILE] MMIAPIFMM_CopyFile MMIAPIFMM_WriteFile copy_ret %d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2246_112_2_18_2_19_51_54, (uint8*) "d", copy_ret);
                break;
            }
        }

        SCI_FREE (data_ptr);
        MMIAPIFMM_CloseFile (sour_file_handle);
        MMIAPIFMM_CloseFile (dest_file_handle);
        //SCI_TRACE_LOW:"[MMIFILE] MMIAPIFMM_CopyFile copy_ret %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2253_112_2_18_2_19_51_55, (uint8*) "d", copy_ret);
        return copy_ret;
    }
    /*****************************************************************************/
//  Description : read file data asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void ReadFileDataAsynCallback (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    )
    {
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;

        //SCI_ASSERT(NULL != irp_param);
        if (PNULL == irp_param)
        {
            //SCI_TRACE_LOW:"ReadFileDataAsynCallback param error:PNULL == irp_param"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2434_112_2_18_2_19_51_66, (uint8*) "");
            return;
        }

        callback_ptr = (MMIFILE_ASYN_CALLBACK_T*) irp_param;
        callback_ptr->error = error;

//    MMK_PostMsg(VIRTUAL_WIN_ID, MSG_FILE_READFILE_ASYN_CALLBACK, (DPARAM)callback_ptr, sizeof(MMIFILE_ASYN_CALLBACK_T));
        MMIAPICOM_OtherTaskToMMI (VIRTUAL_WIN_ID, MSG_FILE_READFILE_ASYN_CALLBACK, (DPARAM) callback_ptr, sizeof (MMIFILE_ASYN_CALLBACK_T));

        SCI_FREE (callback_ptr);

//    MMIAPICOM_TriggerMMITask();
    }

    /*****************************************************************************/
//  Description : read file data asyn
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_ReadFilesDataAsyn (
        const wchar             *full_path_ptr,   //in
        uint16                  full_path_len,    //in, 双字节为单位
        uint8                   *buf_ptr,         //out
        uint32                  to_read_size,     //in
        SFS_HANDLE              *file_handle_ptr, //in/out
        MMI_WIN_ID_T            win_id,           //in:往窗口发消息,0则往控件发消息
        MMI_CTRL_ID_T           ctrl_id,          //in:往控件发消息,0则往窗口发消息
        MMI_MESSAGE_ID_E        msg_id            //in
    )
    {
        BOOLEAN             result = FALSE;
        SFS_ERROR_E         sfs_error = SFS_ERROR_NONE;
        SFS_HANDLE          sfs_handle = 0;
        uint32              read_size = 0;
        SFS_OVERLAPPED_T    overLapped = {0};
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;
        IGUICTRL_T               *ctrl_ptr = PNULL;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        //SCI_ASSERT(NULL != buf_ptr);
        //SCI_ASSERT((0 != win_id) || (0 != ctrl_id));
        //SCI_ASSERT(0 != msg_id);
        if ( (PNULL == full_path_ptr) || (0 == full_path_len) || (PNULL == buf_ptr))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataAsyn param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2478_112_2_18_2_19_51_67, (uint8*) "");
            return FALSE;
        }

        if ( ( (0 == win_id) && (0 == ctrl_id)) || (0 == msg_id))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataAsyn param error: win_id = %d,ctrl_id = %d,msg_id = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2483_112_2_18_2_19_51_68, (uint8*) "ddd", win_id, ctrl_id, msg_id);
            return FALSE;
        }

        if (0 == to_read_size)
        {
            result = TRUE;

            if (0 != win_id)
            {
                MMK_PostMsg (win_id, msg_id, &result, sizeof (BOOLEAN));
            }
            else
            {
                ctrl_ptr = MMK_GetCtrlPtr (ctrl_id);

                if (PNULL != ctrl_ptr)
                {
                    MMK_PostMsg (/*(PWND)ctrl_ptr*/ctrl_id, msg_id, &result, sizeof (BOOLEAN));
                }
            }

            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataAsyn: read file 0"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2503_112_2_18_2_19_52_69, (uint8*) "");
            return TRUE;
        }

        sfs_handle = SFS_CreateFile (full_path_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

        if (SFS_INVALID_HANDLE != sfs_handle)
        {
            callback_ptr = (MMIFILE_ASYN_CALLBACK_T *) SCI_ALLOCA (sizeof (MMIFILE_ASYN_CALLBACK_T));

            //SCI_ASSERT(NULL != callback_ptr);
            if (PNULL == callback_ptr)
            {
                //SCI_TRACE_LOW:"MMIFMM:MMIAPIFMM_ReadFilesDataAsyn PNULL == callback_ptr is error!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2515_112_2_18_2_19_52_70, (uint8*) "");
                return FALSE;
            }

            callback_ptr->handle = sfs_handle;
            callback_ptr->msg_id = msg_id;
            callback_ptr->win_id = win_id;
            callback_ptr->ctrl_id = ctrl_id;

            overLapped.param = (SFS_PARAM) callback_ptr;
            overLapped.complete_route = ReadFileDataAsynCallback;

            sfs_error = SFS_ReadFile (sfs_handle, buf_ptr, to_read_size, &read_size, &overLapped);

            if (SFS_ERROR_NONE == sfs_error || SFS_ERROR_IO_PENDING == sfs_error)
            {
                //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataAsyn: read file success"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2529_112_2_18_2_19_52_71, (uint8*) "");
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataAsyn: read file fail"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2534_112_2_18_2_19_52_72, (uint8*) "");

                SFS_CloseFile (sfs_handle);
                SCI_FREE (callback_ptr);
            }
        }

        //set return file handle
        if (PNULL != file_handle_ptr)
        {
            *file_handle_ptr = sfs_handle;
        }

        return result;
    }

//fixed by liyan.zhu for bugzilla 2760
#if 0
    /*****************************************************************************/
//  Description : read file data by offset syn, file must exist
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_ReadFileDataByOffsetSyn (
        const wchar             *full_path_ptr,     //in
        uint16                  full_path_len,      //in, 双字节为单位
        uint8                   *buf_ptr,           //out
        uint32                  to_read_size,       //in
        uint32                  offset              //in
    )
    {
        BOOLEAN         result = FALSE;
        SFS_HANDLE      sfs_handle = 0;
        uint32          read_size = 0;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        //SCI_ASSERT(NULL != buf_ptr);
        if ( (PNULL == full_path_ptr) || (0 == full_path_len) || (PNULL == buf_ptr))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFileDataByOffsetSyn param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2277_112_2_18_2_19_51_56, (uint8*) "");
            return FALSE;
        }

        if (0 == to_read_size)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFileDataByOffsetSyn: read file 0"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2282_112_2_18_2_19_51_57, (uint8*) "");
            return TRUE;
        }

        sfs_handle = SFS_CreateFile (full_path_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

        if (SFS_INVALID_HANDLE != sfs_handle)
        {
            if (SFS_ERROR_NONE == SFS_SetFilePointer (sfs_handle, offset, SFS_SEEK_BEGIN)) /*lint !e747*/
            {
                if (SFS_ERROR_NONE == SFS_ReadFile (sfs_handle, buf_ptr, to_read_size, &read_size, NULL))
                {
                    result = TRUE;
                }
            }
        }

        SFS_CloseFile (sfs_handle);

        if (result)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFileDataByOffsetSyn: read file data success"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2303_112_2_18_2_19_51_58, (uint8*) "");
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFileDataByOffsetSyn: read file data fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2307_112_2_18_2_19_51_59, (uint8*) "");
        }

        return result;
    }

    /*****************************************************************************/
//  Description : read file data asyn with file handle
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIFMM_ReadFileDataByOffsetSynEx (
        MMIFILE_HANDLE          file_handle,        //in
        uint8                   *buf_ptr,           //out
        uint32                  to_read_size,       //in
        uint32                  offset              //in
    )
    {
        BOOLEAN         result = FALSE;
        SFS_HANDLE      sfs_handle = file_handle;
        uint32          read_size = 0;

        if (NULL == buf_ptr)
        {
            return result;
        }

        if (0 == to_read_size)
        {
            //SCI_TRACE_LOW:"MMIFMM_ReadFileDataByOffsetSynEx: read file 0"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2335_112_2_18_2_19_51_60, (uint8*) "");
            return TRUE;
        }

        if (SFS_INVALID_HANDLE == sfs_handle)
        {
            return FALSE;
        }

        if (SFS_ERROR_NONE == SFS_SetFilePointer (sfs_handle, offset, SFS_SEEK_BEGIN))	/*lint !e747 */
        {
            if (SFS_ERROR_NONE == SFS_ReadFile (sfs_handle, buf_ptr, to_read_size, &read_size, NULL))
            {
                result = TRUE;
            }
        }

        if (result)
        {
            //SCI_TRACE_LOW:"MMIFMM_ReadFileDataByOffsetSynEx: read file data success"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2354_112_2_18_2_19_51_61, (uint8*) "");
        }
        else
        {
            //SCI_TRACE_LOW:"MMIFMM_ReadFileDataByOffsetSynEx: read file data fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2358_112_2_18_2_19_51_62, (uint8*) "");
        }

        return result;
    }

    /*****************************************************************************/
//  Description : write file data by offset syn, file must exist
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIFILE_WriteFileDataByOffsetSyn (
        const uint16            *full_path_ptr,     //in
        uint16                  full_path_len,      //in, 双字节为单位
        const uint8             *buf_ptr,           //in
        uint32                  to_write_size,      //in
        uint32                  offset              //in
    )
    {
        BOOLEAN         result = FALSE;
        SFS_HANDLE      sfs_handle = 0;
        uint32          write_size = to_write_size;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        //SCI_ASSERT(NULL != buf_ptr);
        //SCI_ASSERT(0 < to_write_size);
        if ( (PNULL == full_path_ptr) || (0 == full_path_len) || (PNULL == buf_ptr) || (0 == to_write_size))
        {
            //SCI_TRACE_LOW:"MMIFILE_WriteFileDataByOffsetSyn param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2386_112_2_18_2_19_51_63, (uint8*) "");
            return FALSE;
        }

        sfs_handle = SFS_CreateFile (full_path_ptr, SFS_MODE_WRITE | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

        if (SFS_INVALID_HANDLE != sfs_handle)
        {
            if (SFS_ERROR_NONE == SFS_SetFilePointer (sfs_handle, offset, SFS_SEEK_BEGIN)) /*lint !e747*/
            {
                if (SFS_ERROR_NONE == SFS_WriteFile (sfs_handle, (const void *) buf_ptr, to_write_size, &write_size, NULL))
                {
                    result = TRUE;
                }
            }
        }

        SFS_CloseFile (sfs_handle);

        if (result)
        {
            //SCI_TRACE_LOW:"MMIFILE_WriteFileDataByOffsetSyn: write file data success"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2407_112_2_18_2_19_51_64, (uint8*) "");
        }
        else
        {
            //SCI_TRACE_LOW:"MMIFILE_WriteFileDataByOffsetSyn: write file data fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2411_112_2_18_2_19_51_65, (uint8*) "");
        }

        return result;
    }


    /*****************************************************************************/
//  Description : read file data asyn with file handle
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_ReadFilesDataAsynEx (
        MMIFILE_HANDLE          file_handle,        //in
        uint8                   *buf_ptr,           //out
        uint32                  to_read_size,       //in
        MMI_WIN_ID_T            win_id,             //in:往窗口发消息,0则往控件发消息
        MMI_CTRL_ID_T           ctrl_id,            //in:往控件发消息,0则往窗口发消息
        MMI_MESSAGE_ID_E        msg_id              //in
    )
    {
        BOOLEAN             result = FALSE;
        SFS_ERROR_E         sfs_error = SFS_ERROR_NONE;
        SFS_HANDLE          sfs_handle = file_handle;
        uint32              read_size = 0;
        SFS_OVERLAPPED_T    overLapped = {0};
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;
        IGUICTRL_T               *ctrl_ptr = PNULL;

        if (NULL == buf_ptr)
        {
            return result;
        }

        if ( (0 == win_id) && (0 == ctrl_id))
        {
            return result;
        }

        if (0 == msg_id)
        {
            return result;
        }

        if (0 == to_read_size)
        {
            result = TRUE;

            if (0 != win_id)
            {
                MMK_PostMsg (win_id, msg_id, &result, sizeof (BOOLEAN));
            }
            else
            {
                ctrl_ptr = MMK_GetCtrlPtr (ctrl_id);

                if (PNULL != ctrl_ptr)
                {
                    MMK_PostMsg (/*(PWND)ctrl_ptr*/ctrl_id, msg_id, &result, sizeof (BOOLEAN));
                }
            }

            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataAsynEx: read file 0"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2600_112_2_18_2_19_52_73, (uint8*) "");
            return TRUE;
        }

        if (SFS_INVALID_HANDLE != sfs_handle)
        {
            callback_ptr = (MMIFILE_ASYN_CALLBACK_T *) SCI_ALLOCA (sizeof (MMIFILE_ASYN_CALLBACK_T));
            //SCI_ASSERT(NULL != callback_ptr);
            callback_ptr->handle = sfs_handle;
            callback_ptr->msg_id = msg_id;
            callback_ptr->win_id = win_id;
            callback_ptr->ctrl_id = ctrl_id;

            overLapped.param = (SFS_PARAM) callback_ptr;
            overLapped.complete_route = ReadFileDataAsynCallback;

            sfs_error = SFS_ReadFile (sfs_handle, buf_ptr, to_read_size, &read_size, &overLapped);

            if (SFS_ERROR_NONE == sfs_error || SFS_ERROR_IO_PENDING == sfs_error)
            {
                //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataAsynEx: read file success"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2619_112_2_18_2_19_52_74, (uint8*) "");
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"MMIAPIFMM_ReadFilesDataAsynEx: read file fail"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2624_112_2_18_2_19_52_75, (uint8*) "");
                SCI_FREE (callback_ptr);
            }
        }

        return result;
    }
#endif

    /*****************************************************************************/
//  Description : write file data asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void WriteFileDataAsynCallback (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    )
    {
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;

        //SCI_ASSERT(NULL != irp_param);
        if (PNULL == irp_param)
        {
            //SCI_TRACE_LOW:"WriteFileDataAsynCallback param error:PNULL == irp_param"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2649_112_2_18_2_19_52_76, (uint8*) "");
            return;
        }

        //SCI_TRACE_LOW:"WriteFileDataAsynCallback: sfs error = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2652_112_2_18_2_19_52_77, (uint8*) "d", error);

        callback_ptr = (MMIFILE_ASYN_CALLBACK_T*) irp_param;
        callback_ptr->error = error;

//   MMK_PostMsg(VIRTUAL_WIN_ID, MSG_FILE_WRITEFILE_ASYN_CALLBACK, (DPARAM)callback_ptr, sizeof(MMIFILE_ASYN_CALLBACK_T));
        MMIAPICOM_OtherTaskToMMI (VIRTUAL_WIN_ID, MSG_FILE_WRITEFILE_ASYN_CALLBACK, (DPARAM) callback_ptr, sizeof (MMIFILE_ASYN_CALLBACK_T));

        SCI_FREE (callback_ptr);

//    MMIAPICOM_TriggerMMITask();
    }

    /*****************************************************************************/
//  Description : write file data asyn
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_WriteFilesDataAsyn (
        const wchar             *full_path_ptr,     //in
        uint16                  full_path_len,      //in, 双字节为单位
        uint8                   *buf_ptr,           //in
        uint32                  to_write_size,      //in
        MMI_WIN_ID_T            win_id,             //in
        MMI_MESSAGE_ID_E        msg_id              //in
    )
    {
        BOOLEAN             result = FALSE;
        SFS_HANDLE          sfs_handle = 0;
        uint32              write_size = to_write_size;
        SFS_OVERLAPPED_T    overLapped = {0};
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;
        SFS_ERROR_E         write_return = SFS_ERROR_NONE;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        //SCI_ASSERT(NULL != buf_ptr);
        //SCI_ASSERT(0 < to_write_size);
        //SCI_ASSERT(0 != win_id);
        //SCI_ASSERT(0 != msg_id);
        if ( (PNULL == full_path_ptr) || (0 == full_path_len) || (PNULL == buf_ptr) || (0 == to_write_size))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_WriteFilesDataAsyn param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2693_112_2_18_2_19_52_78, (uint8*) "");
            return FALSE;
        }

        if ( (0 == win_id) || (0 == msg_id))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_WriteFilesDataAsyn param error: win_id = %d,msg_id = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2698_112_2_18_2_19_52_79, (uint8*) "dd", win_id, msg_id);
            return FALSE;
        }

        sfs_handle = SFS_CreateFile (full_path_ptr, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/

        if (SFS_INVALID_HANDLE != sfs_handle)
        {
            callback_ptr = (MMIFILE_ASYN_CALLBACK_T *) SCI_ALLOCA (sizeof (MMIFILE_ASYN_CALLBACK_T));

            //SCI_ASSERT(NULL != callback_ptr);
            if (PNULL == callback_ptr)
            {
                //SCI_TRACE_LOW:"MMIAPIFMM_WriteFilesDataAsyn PNULL == callback_ptr is error!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2710_112_2_18_2_19_52_80, (uint8*) "");
                return FALSE;
            }

            callback_ptr->handle = sfs_handle;
            callback_ptr->msg_id = msg_id;
            callback_ptr->win_id = win_id;

            overLapped.param = (uint32) callback_ptr;
            overLapped.complete_route = WriteFileDataAsynCallback;

            write_return = SFS_WriteFile (sfs_handle, buf_ptr, to_write_size, &write_size, &overLapped);

            if (SFS_ERROR_NONE == write_return || SFS_ERROR_IO_PENDING == write_return)
            {
                //SCI_TRACE_LOW:"MMIAPIFMM_WriteFilesDataAsyn: write file success"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2723_112_2_18_2_19_52_81, (uint8*) "");
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"MMIAPIFMM_WriteFilesDataAsyn: write file fail"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2728_112_2_18_2_19_52_82, (uint8*) "");
                SFS_CloseFile (sfs_handle);
                SCI_FREE (callback_ptr);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_WriteFilesDataAsyn: create file fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2735_112_2_18_2_19_52_83, (uint8*) "");
        }

        return result;
    }
//fixed by liyan.zhu for bugzilla 2760
#if 0
    /*****************************************************************************/
//  Description : read file data by offset asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void ReadFileDataByOffsetAsynCallback (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    )
    {
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;

        //SCI_ASSERT(NULL != irp_param);
        if (PNULL == irp_param)
        {
            //SCI_TRACE_LOW:"ReadFileDataByOffsetAsynCallback param error:PNULL == irp_param"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2757_112_2_18_2_19_52_84, (uint8*) "");
            return;
        }

        callback_ptr = (MMIFILE_ASYN_CALLBACK_T*) irp_param;
        callback_ptr->error = error;

//    MMK_PostMsg(VIRTUAL_WIN_ID, MSG_FILE_READFILE_BYOFFSET_ASYN_CALLBACK, (DPARAM)callback_ptr, sizeof(MMIFILE_ASYN_CALLBACK_T));
        MMIAPICOM_OtherTaskToMMI (VIRTUAL_WIN_ID, MSG_FILE_READFILE_BYOFFSET_ASYN_CALLBACK, (DPARAM) callback_ptr, sizeof (MMIFILE_ASYN_CALLBACK_T));

        SCI_FREE (callback_ptr);

//    MMIAPICOM_TriggerMMITask();
    }

    /*****************************************************************************/
//  Description : read file data by offset asyn, file must exist
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIFILE_ReadFileDataByOffsetAsyn (
        const wchar             *full_path_ptr,     //in
        uint16                  full_path_len,      //in, 双字节为单位
        uint8                   *buf_ptr,           //out
        uint32                  to_read_size,       //in
        uint32                  offset,             //in
        MMI_WIN_ID_T            win_id,             //in
        MMI_MESSAGE_ID_E        msg_id              //in
    )
    {
        BOOLEAN             result = FALSE;
        SFS_ERROR_E         sfs_error = SFS_ERROR_NONE;
        SFS_HANDLE          sfs_handle = 0;
        uint32              read_size = 0;
        SFS_OVERLAPPED_T    overLapped = {0};
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        //SCI_ASSERT(NULL != buf_ptr);
        //SCI_ASSERT(0 != win_id);
        //SCI_ASSERT(0 != msg_id);
        if ( (PNULL == full_path_ptr) || (0 == full_path_len) || (PNULL == buf_ptr))
        {
            //SCI_TRACE_LOW:"MMIFILE_ReadFileDataByOffsetAsyn param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2800_112_2_18_2_19_52_85, (uint8*) "");
            return FALSE;
        }

        if ( (0 == win_id) || (0 == msg_id))
        {
            //SCI_TRACE_LOW:"MMIFILE_ReadFileDataByOffsetAsyn param error:win_id = %d,msg_id = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2805_112_2_18_2_19_52_86, (uint8*) "dd", win_id, msg_id);
            return FALSE;
        }

        if (0 == to_read_size)
        {
            result = TRUE;
            MMK_PostMsg (win_id, msg_id, &result, sizeof (BOOLEAN));

            //SCI_TRACE_LOW:"MMIFILE_ReadFileDataByOffsetAsyn: read file 0"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2814_112_2_18_2_19_52_87, (uint8*) "");
            return TRUE;
        }

        sfs_handle = SFS_CreateFile (full_path_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

        if (SFS_INVALID_HANDLE != sfs_handle)
        {
            if (SFS_ERROR_NONE == SFS_SetFilePointer (sfs_handle, offset, SFS_SEEK_BEGIN)) /*lint !e747*/
            {
                callback_ptr = (MMIFILE_ASYN_CALLBACK_T *) SCI_ALLOCA (sizeof (MMIFILE_ASYN_CALLBACK_T));

                //SCI_ASSERT(NULL != callback_ptr);
                if (PNULL == callback_ptr)
                {
                    //SCI_TRACE_LOW:"MMIFILE_ReadFileDataByOffsetAsyn PNULL == callback_ptr is error!"
                    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2828_112_2_18_2_19_52_88, (uint8*) "");
                    return FALSE;
                }

                callback_ptr->handle = sfs_handle;
                callback_ptr->msg_id = msg_id;
                callback_ptr->win_id = win_id;

                overLapped.param = (SFS_PARAM) callback_ptr;
                overLapped.complete_route = ReadFileDataByOffsetAsynCallback;

                sfs_error = SFS_ReadFile (sfs_handle, buf_ptr, to_read_size, &read_size, &overLapped);

                if (SFS_ERROR_NONE == sfs_error || SFS_ERROR_IO_PENDING == sfs_error)
                {
                    result = TRUE;
                }
                else
                {
                    SFS_CloseFile (sfs_handle);
                    SCI_FREE (callback_ptr);
                }
            }
            else
            {
                SFS_CloseFile (sfs_handle);
            }
        }

        if (result)
        {
            //SCI_TRACE_LOW:"MMIFILE_ReadFileDataByOffsetAsyn: return success"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2857_112_2_18_2_19_52_89, (uint8*) "");
        }
        else
        {
            //SCI_TRACE_LOW:"MMIFILE_ReadFileDataByOffsetAsyn: return fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2861_112_2_18_2_19_52_90, (uint8*) "");
        }

        return result;
    }

    /*****************************************************************************/
//  Description : read file data asyn with file handle
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIFILE_ReadFileDataByOffsetAsynEx (
        MMIFILE_HANDLE          file_handle,        //in
        uint8                   *buf_ptr,           //out
        uint32                  to_read_size,       //in
        uint32                  offset,             //in
        MMI_WIN_ID_T            win_id,             //in
        MMI_MESSAGE_ID_E        msg_id              //in
    )
    {
        BOOLEAN             result = FALSE;
        SFS_ERROR_E         sfs_error = SFS_ERROR_NONE;
        SFS_HANDLE          sfs_handle = file_handle;
        uint32              read_size = 0;
        SFS_OVERLAPPED_T    overLapped = {0};
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;

        if (NULL == buf_ptr)
        {
            return result;
        }

        if (0 == win_id)
        {
            return result;
        }

        if (0 == msg_id)
        {
            return result;
        }

//    if (SFS_INVALID_HANDLE == sfs_handle)
//    {
//        return result;
//    }

        if (0 == to_read_size)
        {
            result = TRUE;
            MMK_PostMsg (win_id, msg_id, &result, sizeof (BOOLEAN));

            //SCI_TRACE_LOW:"MMIFILE_ReadFileDataByOffsetAsynEx: read file 0"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2910_112_2_18_2_19_52_91, (uint8*) "");
            return TRUE;
        }

        if (SFS_INVALID_HANDLE == sfs_handle)
        {
            return FALSE;
        }

        if (SFS_ERROR_NONE == SFS_SetFilePointer (sfs_handle, offset, SFS_SEEK_BEGIN))	/*lint !e747 */
        {
            callback_ptr = (MMIFILE_ASYN_CALLBACK_T *) SCI_ALLOCA (sizeof (MMIFILE_ASYN_CALLBACK_T));
            //SCI_ASSERT(NULL != callback_ptr);
            callback_ptr->handle = sfs_handle;
            callback_ptr->msg_id = msg_id;
            callback_ptr->win_id = win_id;

            overLapped.param = (SFS_PARAM) callback_ptr;
            overLapped.complete_route = ReadFileDataByOffsetAsynCallback;

            sfs_error = SFS_ReadFile (sfs_handle, buf_ptr, to_read_size, &read_size, &overLapped);

            if (SFS_ERROR_NONE == sfs_error || SFS_ERROR_IO_PENDING == sfs_error)
            {
                result = TRUE;
            }
            else
            {
                SCI_FREE (callback_ptr);
            }
        }

        if (result)
        {
            //SCI_TRACE_LOW:"MMIFILE_ReadFileDataByOffsetAsynEx: return success"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2943_112_2_18_2_19_52_92, (uint8*) "");
        }
        else
        {
            //SCI_TRACE_LOW:"MMIFILE_ReadFileDataByOffsetAsynEx: return fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2947_112_2_18_2_19_52_93, (uint8*) "");
        }

        return result;
    }

    /*****************************************************************************/
//  Description : write file data by offset asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void WriteFileDataByOffsetAsynCallback (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    )
    {
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;

        //SCI_ASSERT(NULL != irp_param);
        if (PNULL == irp_param)
        {
            //SCI_TRACE_LOW:"WriteFileDataByOffsetAsynCallback param error:PNULL == irp_param"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2969_112_2_18_2_19_52_94, (uint8*) "");
            return;
        }

        //SCI_TRACE_LOW:"WriteFileDataAsynCallback: sfs error = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_2973_112_2_18_2_19_52_95, (uint8*) "d", error);

        callback_ptr = (MMIFILE_ASYN_CALLBACK_T*) irp_param;
        callback_ptr->error = error;

//    MMK_PostMsg(VIRTUAL_WIN_ID, MSG_FILE_WRITEFILE_BYOFFSET_ASYN_CALLBACK, (DPARAM)callback_ptr, sizeof(MMIFILE_ASYN_CALLBACK_T));
        MMIAPICOM_OtherTaskToMMI (VIRTUAL_WIN_ID, MSG_FILE_WRITEFILE_BYOFFSET_ASYN_CALLBACK, (DPARAM) callback_ptr, sizeof (MMIFILE_ASYN_CALLBACK_T));
        SCI_FREE (callback_ptr);

//    MMIAPICOM_TriggerMMITask();
    }

    /*****************************************************************************/
//  Description : write file data by offset asyn, file must exist
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_WriteFileDataByOffsetAsyn (
        const wchar             *full_path_ptr,     //in
        uint16                  full_path_len,      //in, 双字节为单位
        uint8                   *buf_ptr,           //in
        uint32                  to_write_size,      //in
        uint32                  offset,             //in
        MMI_WIN_ID_T            win_id,             //in
        MMI_MESSAGE_ID_E        msg_id              //in
    )
    {
        BOOLEAN             result = FALSE;
        SFS_HANDLE          sfs_handle = 0;
        uint32              write_size = to_write_size;
        SFS_OVERLAPPED_T    overLapped = {0};
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;
        SFS_ERROR_E         write_return = SFS_ERROR_NONE;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        //SCI_ASSERT(NULL != buf_ptr);
        //SCI_ASSERT(0 < to_write_size);
        //SCI_ASSERT(0 != win_id);
        //SCI_ASSERT(0 != msg_id);
        if ( (PNULL == full_path_ptr) || (0 == full_path_len) || (PNULL == buf_ptr) || (0 == to_write_size))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_WriteFileDataByOffsetAsyn param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3014_112_2_18_2_19_53_96, (uint8*) "");
            return FALSE;
        }

        if ( (0 == win_id) || (0 == msg_id))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_WriteFileDataByOffsetAsyn param error: win_id = %d,msg_id = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3019_112_2_18_2_19_53_97, (uint8*) "dd", win_id, msg_id);
            return FALSE;
        }

        sfs_handle = SFS_CreateFile (full_path_ptr, SFS_MODE_WRITE | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

        if (SFS_INVALID_HANDLE != sfs_handle)
        {
            if (SFS_ERROR_NONE == SFS_SetFilePointer (sfs_handle, offset, SFS_SEEK_BEGIN)) /*lint !e747*/
            {
                callback_ptr = (MMIFILE_ASYN_CALLBACK_T *) SCI_ALLOCA (sizeof (MMIFILE_ASYN_CALLBACK_T));

                //SCI_ASSERT(NULL != callback_ptr);
                if (PNULL == callback_ptr)
                {
                    //SCI_TRACE_LOW:"[MMIFMM]:MMIAPIFMM_WriteFileDataByOffsetAsyn PNULL == callback_ptr error!"
                    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3033_112_2_18_2_19_53_98, (uint8*) "");
                    return FALSE;
                }

                callback_ptr->handle = sfs_handle;
                callback_ptr->msg_id = msg_id;
                callback_ptr->win_id = win_id;

                overLapped.param = (uint32) callback_ptr;
                overLapped.complete_route = WriteFileDataByOffsetAsynCallback;

                write_return = SFS_WriteFile (sfs_handle, buf_ptr, to_write_size, &write_size, &overLapped);

                if (SFS_ERROR_NONE == write_return || SFS_ERROR_IO_PENDING == write_return)
                {
                    result = TRUE;
                }
                else
                {
                    SFS_CloseFile (sfs_handle);
                    SCI_FREE (callback_ptr);
                }
            }
            else
            {
                SFS_CloseFile (sfs_handle);
            }
        }

        if (result)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_WriteFileDataByOffsetAsyn: return success"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3062_112_2_18_2_19_53_99, (uint8*) "");
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_WriteFileDataByOffsetAsyn: return fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3066_112_2_18_2_19_53_100, (uint8*) "");
        }

        return result;
    }
#endif

    /*****************************************************************************/
//  Description : asyn operation confirm
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    void MMIAPIFMM_OperationAsynCnf (
        void                  *param_ptr,
        MMI_MESSAGE_ID_E      callback_msg_id
    )
    {
        BOOLEAN                     result = FALSE;
        IGUICTRL_T               *ctrl_ptr = PNULL;
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;

        //SCI_ASSERT(NULL != param_ptr);
        if (PNULL == param_ptr)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_OperationAsynCnf param error: PNULL == param_ptr"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3089_112_2_18_2_19_53_101, (uint8*) "");
            return;
        }

        callback_ptr = (MMIFILE_ASYN_CALLBACK_T*) param_ptr;

        if (SFS_ERROR_NONE == callback_ptr->error)
        {
            result = TRUE;
        }

        SFS_CloseFile (callback_ptr->handle);

        if (0 != callback_ptr->win_id)
        {
            MMK_SendMsg (callback_ptr->win_id, callback_ptr->msg_id, &result);
        }
        else
        {
            ctrl_ptr = MMK_GetCtrlPtr (callback_ptr->ctrl_id);

            if (PNULL != ctrl_ptr)
            {
                MMK_SendMsg (callback_ptr->ctrl_id, callback_ptr->msg_id, &result);
            }
        }

        //SCI_TRACE_LOW:"MMIAPIFMM_OperationAsynCnf: result = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3114_112_2_18_2_19_53_102, (uint8*) "d", result);
    }


    /*****************************************************************************/
//  Description : delete file asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void DeleteFileAsynCallback (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    )
    {
        //BOOLEAN                     result = FALSE;
        MMIFILE_ASYN_CALLBACK_T                  *sig_ptr = PNULL;
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;

        //SCI_ASSERT(NULL != irp_param);
        if (PNULL == irp_param)
        {
            //SCI_TRACE_LOW:"DeleteFileAsynCallback param error:PNULL == irp_param"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3136_112_2_18_2_19_53_103, (uint8*) "");
            return;
        }


        //SCI_TRACE_LOW:"DeleteFileAsynCallback: error = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3141_112_2_18_2_19_53_104, (uint8*) "d", error);
        callback_ptr = (MMIFILE_ASYN_CALLBACK_T*) irp_param;

        MmiCreateSignal (MSG_FS_DELETE_CNF, sizeof (MMIFILE_ASYN_CALLBACK_T), (MmiSignalS**)&sig_ptr);

        //the xSignalHeaderRec in irp_param is not correct
        SCI_MEMCPY((void*)((uint32)sig_ptr + sizeof(xSignalHeaderRec)), (void*)((uint32)callback_ptr + sizeof(xSignalHeaderRec)), sizeof(MMIFILE_ASYN_CALLBACK_T) - sizeof(xSignalHeaderRec));
        sig_ptr->SignalSize = sizeof (MMIFILE_ASYN_CALLBACK_T);
        sig_ptr->Sender = P_APP;
        sig_ptr->error = error;
        MMI_SEND_SIGNAL (sig_ptr, P_APP)
        //MMI_SendSignal_2APP(callback_ptr->msg_id, (uint16)callback_ptr->msg_id);
        //MMK_PostMsg(callback_ptr->win_id, callback_ptr->msg_id, &result, sizeof(result));
        SCI_FREE (callback_ptr);

        //MMIAPICOM_TriggerMMITask();
    }


    /*****************************************************************************/
//  Description : create file asyn callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void CreateFileAsynCallback (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    )
    {
        BOOLEAN                     result = FALSE;
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;

        //SCI_ASSERT(NULL != irp_param);
        if (PNULL == irp_param)
        {
            //SCI_TRACE_LOW:"CreateFileAsynCallback param error:PNULL == irp_param"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3175_112_2_18_2_19_53_105, (uint8*) "");
            return;
        }

        callback_ptr = (MMIFILE_ASYN_CALLBACK_T*) irp_param;

        //SCI_TRACE_LOW:"CreateFileAsynCallback: error = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3181_112_2_18_2_19_53_106, (uint8*) "d", error);

        if (SFS_ERROR_NONE == error)
        {
            result = TRUE;
        }

//    MMK_PostMsg(callback_ptr->win_id, callback_ptr->msg_id, &result, sizeof(result));
        MMIAPICOM_OtherTaskToMMI (callback_ptr->win_id, callback_ptr->msg_id, (DPARAM) &result, sizeof (result));
        SCI_FREE (callback_ptr);

//    MMIAPICOM_TriggerMMITask();
    }

    /*****************************************************************************/
//  Description : delete file asyn
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_DeleteFileAsyn (
        const wchar             *full_path_ptr,     //in
        uint16                  full_path_len,      //in, 双字节为单位
        MMI_WIN_ID_T            win_id,             //in
        MMI_MESSAGE_ID_E        msg_id              //in
    )
    {
        BOOLEAN                     result = FALSE;
        SFS_ERROR_E                 sfs_error = SFS_ERROR_NONE;
        SFS_OVERLAPPED_T            overLapped = {0};
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        //SCI_ASSERT(0 != win_id);
        //SCI_ASSERT(0 != msg_id);

        if ( (PNULL == full_path_ptr) || (0 == full_path_len))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_DeleteFileAsyn param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3217_112_2_18_2_19_53_107, (uint8*) "");
            return FALSE;
        }

        if ( (0 == win_id) || (0 == msg_id))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_DeleteFileAsyn param error: win_id = %d,msg_id = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3222_112_2_18_2_19_53_108, (uint8*) "dd", win_id, msg_id);
            return FALSE;
        }

        //delete the miniature file
        if (MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_GetFileTypeByFileName (full_path_ptr, full_path_len))
        {
            GUIANIM_DeleteMiniatureFile (full_path_ptr, full_path_len);
        }

        callback_ptr = (MMIFILE_ASYN_CALLBACK_T *) SCI_ALLOCA (sizeof (MMIFILE_ASYN_CALLBACK_T));

        //SCI_ASSERT(NULL != callback_ptr);
        if (PNULL == callback_ptr)
        {
            //SCI_TRACE_LOW:"MMIFMM:MMIAPIFMM_DeleteFileAsyn PNULL == callback_ptr error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3236_112_2_18_2_19_53_109, (uint8*) "");
            return FALSE;
        }

        callback_ptr->handle = 0;
        callback_ptr->msg_id = msg_id;
        callback_ptr->win_id = win_id;

        overLapped.param = (uint32) callback_ptr;
        overLapped.complete_route = DeleteFileAsynCallback;

        sfs_error = SFS_DeleteFile (full_path_ptr, &overLapped);

        if (SFS_ERROR_NONE == sfs_error || SFS_ERROR_IO_PENDING == sfs_error)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_DeleteFileAsyn: delete asyn success"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3249_112_2_18_2_19_53_110, (uint8*) "");

            result = TRUE;
        }
        else
        {
            SCI_FREE (callback_ptr);
        }

        return result;
    }

#ifdef MMI_FILEMGR_COPY //for 217 ram cutdown
    /*****************************************************************************/
//  Description :start copy one file
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_CopyOneFile (
        const wchar                 *src_full_path,
        uint16                      src_path_len,       //in, unicode个数
        const wchar                 *dest_full_path,
        uint16                      dest_path_len,      //in, unicode个数
        MMI_WIN_ID_T                win_id,
        MMI_MESSAGE_ID_E            msg_id
    )
    {
        BOOLEAN         result = FALSE;

        //SCI_ASSERT(NULL != src_full_path);
        //SCI_ASSERT(0 < src_path_len);
        //SCI_ASSERT(NULL != dest_full_path);
        //SCI_ASSERT(0 < dest_path_len);
        if ( (PNULL == src_full_path) || (0 == src_path_len) || (PNULL == dest_full_path) || (0 == dest_path_len))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_CopyOneFile param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3283_112_2_18_2_19_53_111, (uint8*) "");
            return FALSE;
        }

        SCI_MEMSET (&s_copy_info, 0, sizeof (s_copy_info));

        s_copy_info.src_path_len = src_path_len;
        MMI_WSTRNCPY (s_copy_info.src_full_path, MMIFILE_FULL_PATH_MAX_LEN,
                      src_full_path, src_path_len, src_path_len);
        s_copy_info.dest_path_len = dest_path_len;
        MMI_WSTRNCPY (s_copy_info.dest_full_path, MMIFILE_FULL_PATH_MAX_LEN,
                      dest_full_path, dest_path_len, dest_path_len);

        s_copy_info.copy_size = 0;
        MMIAPIFMM_GetFileInfo (src_full_path, src_path_len, &s_copy_info.file_size, NULL, NULL);
        s_copy_info.src_handle = SFS_INVALID_HANDLE;
        s_copy_info.dest_handle = SFS_INVALID_HANDLE;
        s_copy_info.buf_ptr = (uint8 *) SCI_ALLOCA (MMIFILE_COPY_BUFFER_SIZE);

        //SCI_ASSERT(NULL != s_copy_info.buf_ptr);
        if (PNULL == s_copy_info.buf_ptr)
        {
            //SCI_TRACE_LOW:"MMIFMM:MMIAPIFMM_CopyOneFile PNULL == s_copy_info.buf_ptr is error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3304_112_2_18_2_19_53_112, (uint8*) "");
            return FALSE;
        }

        s_copy_info.win_id = win_id;
        s_copy_info.msg_id = msg_id;


        s_copy_info.src_handle = SFS_CreateFile (s_copy_info.src_full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

        if (SFS_INVALID_HANDLE != s_copy_info.src_handle)
        {
            if (SFS_ERROR_NONE == SFS_GetFileSize (s_copy_info.src_handle, &s_copy_info.file_size))
            {
                s_copy_info.dest_handle = SFS_CreateFile (s_copy_info.dest_full_path, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/

                if (SFS_INVALID_HANDLE != s_copy_info.dest_handle)
                {
                    //SCI_TRACE_LOW:"MMIAPIFMM_CopyOneFile: create src file and dest file success"
                    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3320_112_2_18_2_19_53_113, (uint8*) "");

                    if (SFS_ERROR_NONE == SFS_SetFileSize (s_copy_info.dest_handle, s_copy_info.file_size))
                    {
                        if (MMIAPIFMM_CopyReadSrc (SFS_ERROR_NONE))
                        {
                            result = TRUE;
                        }
                    }
                }
            }
        }


        if (!result)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_CopyOneFile: create src file and dest file fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3336_112_2_18_2_19_53_114, (uint8*) "");

            if (NULL != s_copy_info.buf_ptr)
            {
                SCI_FREE (s_copy_info.buf_ptr);
                s_copy_info.buf_ptr = NULL;
            }

            if (SFS_INVALID_HANDLE != s_copy_info.src_handle)
            {
                SFS_CloseFile (s_copy_info.src_handle);
            }

            if (SFS_INVALID_HANDLE != s_copy_info.dest_handle)
            {
                SFS_CloseFile (s_copy_info.dest_handle);
            }
        }

        return result;
    }

    /*****************************************************************************/
//  Description : callback of read file data in copy file
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void CallBackCopyReadSrc (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    )
    {
//    MMK_PostMsg(VIRTUAL_WIN_ID, MSG_FILE_COPY_READ_CNF, (DPARAM)&error,sizeof(SFS_ERROR_E));
        MMIAPICOM_OtherTaskToMMI (VIRTUAL_WIN_ID, MSG_FILE_COPY_READ_CNF, (DPARAM) &error, sizeof (SFS_ERROR_E));
//    MMIAPICOM_TriggerMMITask();
    }

    /*****************************************************************************/
//  Description : read file data in copy file, can't be used alone
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_CopyReadSrc (
        SFS_ERROR_E     error
    )
    {
        BOOLEAN             result = FALSE;
        uint32              remainder_len = 0;
        uint32              to_read_len = 0;
        uint32              read_len = 0;
        SFS_ERROR_E         sfs_error = SFS_ERROR_NONE;
        SFS_OVERLAPPED_T    overLapped = {0};

        //whether write file sucess
        if (SFS_ERROR_NONE == error)
        {
            if (s_copy_info.file_size == s_copy_info.copy_size)
            {
                //copy finished and success
                SFS_CloseFile (s_copy_info.src_handle);
                SFS_CloseFile (s_copy_info.dest_handle);

                if (NULL != s_copy_info.buf_ptr)
                {
                    SCI_FREE (s_copy_info.buf_ptr);
                    s_copy_info.buf_ptr = NULL;
                }

                result = TRUE;
                MMK_PostMsg (s_copy_info.win_id, s_copy_info.msg_id, (DPARAM) &result, sizeof (BOOLEAN));

                //SCI_TRACE_LOW:"MMIAPIFMM_CopyReadSrc: copy one file finished with success"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3404_112_2_18_2_19_53_115, (uint8*) "");
            }
            else
            {
                //SCI_TRACE_LOW:"MMIAPIFMM_CopyReadSrc: have copied data size = %d, src file size = %d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3408_112_2_18_2_19_53_116, (uint8*) "dd", s_copy_info.copy_size, s_copy_info.file_size);

                if (MMK_IsOpenWin (s_copy_info.win_id)) //if window closed, copy operation stop with fail
                {
                    //read file size
                    remainder_len = s_copy_info.file_size - s_copy_info.copy_size;
                    to_read_len = (remainder_len > MMIFILE_COPY_BUFFER_SIZE) ? MMIFILE_COPY_BUFFER_SIZE : remainder_len;

                    //read ffs
                    overLapped.complete_route = CallBackCopyReadSrc;
                    overLapped.param = PNULL;
                    sfs_error = SFS_ReadFile (s_copy_info.src_handle, (void *) s_copy_info.buf_ptr, to_read_len, &read_len, &overLapped);

                    if (SFS_ERROR_NONE == sfs_error || SFS_ERROR_IO_PENDING == sfs_error)
                    {
                        result = TRUE;
                    }
                }
            }
        }

        if (!result)
        {
            //copy file fail, close src file, close dest file, free buffer
            //SCI_TRACE_LOW:"MMIAPIFMM_CopyReadSrc:  fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3431_112_2_18_2_19_53_117, (uint8*) "");

            if (NULL != s_copy_info.buf_ptr)
            {
                SCI_FREE (s_copy_info.buf_ptr);
                s_copy_info.buf_ptr = NULL;
            }

            if (SFS_INVALID_HANDLE != s_copy_info.src_handle)
            {
                SFS_CloseFile (s_copy_info.src_handle);
            }

            if (SFS_INVALID_HANDLE != s_copy_info.dest_handle)
            {
                SFS_CloseFile (s_copy_info.dest_handle);
            }

            //delete unfinished file
            MMIAPIFMM_DeleteFileSyn (s_copy_info.dest_full_path, s_copy_info.dest_path_len);

            MMK_PostMsg (s_copy_info.win_id, s_copy_info.msg_id, (DPARAM) &result, sizeof (BOOLEAN));
        }

        return result;
    }



    /*****************************************************************************/
//  Description : callback of write file data in copy file
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    LOCAL void CallBackCopyWriteDest (
        SFS_ERROR_E   error,
        uint32        irp_param,
        uint32        param1,
        uint32        param2
    )
    {
//    MMK_PostMsg(VIRTUAL_WIN_ID, MSG_FILE_COPY_WRITE_CNF, (DPARAM)&error,sizeof(SFS_ERROR_NONE));
        MMIAPICOM_OtherTaskToMMI (VIRTUAL_WIN_ID, MSG_FILE_COPY_WRITE_CNF, (DPARAM) &error, sizeof (SFS_ERROR_E));

//    MMIAPICOM_TriggerMMITask();
    }

    /*****************************************************************************/
//  Description : write file data in copy file, can't be used alone
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIAPIFMM_CopyWriteDest (
        SFS_ERROR_E   error
    )
    {
        BOOLEAN             result          = FALSE;
        uint32              remainder_len   = 0;
        uint32              to_write_len    = 0;
        uint32              write_size      = 0;
        SFS_ERROR_E         sfs_error       = SFS_NO_ERROR;
        SFS_OVERLAPPED_T    overLapped      = {0};

        if (SFS_ERROR_NONE == error)
        {
            if (MMK_IsOpenWin (s_copy_info.win_id)) //if window closed, copy operation stop with fail
            {
                remainder_len = s_copy_info.file_size - s_copy_info.copy_size;
                to_write_len = (remainder_len > MMIFILE_COPY_BUFFER_SIZE) ? MMIFILE_COPY_BUFFER_SIZE : remainder_len;

                //SCI_TRACE_LOW:"MMIAPIFMM_CopyWriteDest: read src file success, data size = %d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3499_112_2_18_2_19_53_118, (uint8*) "d", to_write_len);

                overLapped.complete_route = CallBackCopyWriteDest;
                overLapped.param = PNULL;
                sfs_error = SFS_WriteFile (s_copy_info.dest_handle, s_copy_info.buf_ptr, to_write_len, &write_size, &overLapped);

                if ( (SFS_NO_ERROR == sfs_error) || (SFS_ERROR_IO_PENDING == sfs_error))
                {
                    s_copy_info.copy_size += to_write_len;
                    result = TRUE;
                }
            }
        }

        if (!result)
        {
            //copy file fail, close src file, close dest file, free buffer
            //SCI_TRACE_LOW:"MMIAPIFMM_CopyWriteDest:  fail"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3515_112_2_18_2_19_54_119, (uint8*) "");

            if (NULL != s_copy_info.buf_ptr)
            {
                SCI_FREE (s_copy_info.buf_ptr);
                s_copy_info.buf_ptr = NULL;
            }

            if (SFS_INVALID_HANDLE != s_copy_info.src_handle)
            {
                SFS_CloseFile (s_copy_info.src_handle);
            }

            if (SFS_INVALID_HANDLE != s_copy_info.dest_handle)
            {
                SFS_CloseFile (s_copy_info.dest_handle);
            }

            //delete unfinished file
            MMIAPIFMM_DeleteFileSyn (s_copy_info.dest_full_path, s_copy_info.dest_path_len);

            MMK_PostMsg (s_copy_info.win_id, s_copy_info.msg_id, (DPARAM) &result, sizeof (BOOLEAN));
        }

        return result;
    }

#endif
    /*****************************************************************************/
//  Description : create file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_HANDLE MMIAPIFMM_CreateFile (
        const wchar *file_name,
        uint32 access_mode,
        uint32 share_mode,  //Must be NULL,File System have not realize it
        uint32 file_attri       //Must be NULL,File System have not realize it
    )
    {
        return MMIFILE_CreateFile (file_name, access_mode, share_mode, file_attri);
    }

    /*****************************************************************************/
//  Description : delete folder and it's files in it, only can delete it's sub empty folder
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_DeleteTreeEx (
        const wchar *pathName,
        const wchar *fileName,
        BOOLEAN is_del_dir)
    {
        MMIFILE_ERROR_E     file_ret = SFS_ERROR_NONE;
        SFS_HANDLE          file_handle = PNULL;
        MMIFILE_FIND_DATA_T find_data = {0};
        wchar               *full_path_name_ptr = PNULL;
        uint16              ctlStr[] = L"/A:+NF";
        uint16              filter[] = L"*";
        uint16              slash_name[] = L"\\";
        uint16              tree_len = 0;
        uint16              file_len = 0;

        if (PNULL == pathName || PNULL == fileName)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_DeleteTree pathName 0x%x , fileName 0x%x"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3576_112_2_18_2_19_54_120, (uint8*) "dd", pathName, fileName);
            return SFS_ERROR_INVALID_PARAM;
        }

        full_path_name_ptr = (wchar *) SCI_ALLOCA (sizeof (wchar) * (MMIFILE_FULL_PATH_MAX_LEN + 1));

        if (PNULL == full_path_name_ptr)
        {
            //no memory
            return SFS_ERROR_INVALID_PARAM;
        }

        SCI_MEMSET (full_path_name_ptr, 0x00, sizeof (wchar) * (MMIFILE_FULL_PATH_MAX_LEN + 1));
        tree_len = MMIAPICOM_Wstrlen (pathName);

        if (tree_len < MMIFILE_FULL_PATH_MAX_LEN)
        {
            //combine root name
            MMIAPICOM_Wstrncpy (full_path_name_ptr, pathName,  tree_len);
        }

        tree_len += 1;

        if (tree_len < MMIFILE_FULL_PATH_MAX_LEN)
        {
            //combine "\"
            MMIAPICOM_Wstrncat (full_path_name_ptr, slash_name,  1);
        }

        tree_len += MMIAPICOM_Wstrlen (fileName);

        if (tree_len < MMIFILE_FULL_PATH_MAX_LEN)
        {
            //combine folder name
            MMIAPICOM_Wstrncat (full_path_name_ptr, fileName,  MMIAPICOM_Wstrlen (fileName));
        }

        if (tree_len >= MMIFILE_FULL_PATH_MAX_LEN)
        {
            //MMIAPIFMM_FindClose(file_handle);
            SCI_FREE (full_path_name_ptr);
            return SFS_ERROR_INVALID_PARAM;
        }

        file_ret = SFS_FindFirstFileCtrl (full_path_name_ptr , filter, ctlStr, &file_handle, &find_data, PNULL);

        if (SFS_INVALID_HANDLE != file_handle && (SFS_ERROR_NONE == file_ret))
        {
            do
            {
                //find one sub folder or file to delete it
                SCI_MEMSET (&full_path_name_ptr[tree_len], 0x00, (sizeof (wchar) * (MMIFILE_FULL_PATH_MAX_LEN + 1 - tree_len)));
                file_len = MMIAPICOM_Wstrlen (find_data.name);

                if (file_len + tree_len + 1 <= MMIFILE_FULL_PATH_MAX_LEN)
                {
                    MMIAPICOM_Wstrncat (full_path_name_ptr, slash_name,  1);
                    MMIAPICOM_Wstrncat (&full_path_name_ptr[tree_len], find_data.name,  file_len);

                    if ( (find_data.attr & SFS_ATTR_DIR) > 0)
                    {
                        //folder
                        file_ret = SFS_DeleteDirectory (full_path_name_ptr);
                    }
                    else
                    {
                        //file
                        file_ret = SFS_DeleteFile (full_path_name_ptr, PNULL);
                    }
                }
            }
            while ( (SFS_NO_ERROR == MMIAPIFMM_FindNextFile (file_handle, &find_data)));
        }

        MMIAPIFMM_FindClose (file_handle);

        if (is_del_dir)
        {
            //delete folder itself
            full_path_name_ptr[tree_len] = 0;
            file_ret = SFS_DeleteDirectory (full_path_name_ptr);
        }

        SCI_FREE (full_path_name_ptr);
        return file_ret;
    }

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
    BOOLEAN is_del_dir)
{
    MMIFILE_ERROR_E     file_ret = SFS_ERROR_NONE;
    SFS_HANDLE          file_handle = PNULL;
    MMIFILE_FIND_DATA_T find_data = {0};
    wchar               *full_path_name_ptr = PNULL;
    uint16              ctlStr[] = L"/A:+NF";
    uint16              filter[] = L"*";
    uint16              slash_name[] = L"\\";
    uint16              tree_len = 0;
    uint16              file_len = 0;

    if (PNULL == pathName || PNULL == fileName)
    {
        SCI_TRACE_LOW("MMIAPIFMM_DeleteAllTreeEx pathName:0x%x, fileName:0x%x", pathName, fileName);
        return SFS_ERROR_INVALID_PARAM;
    }

    full_path_name_ptr = (wchar *) SCI_ALLOCA (sizeof (wchar) * (MMIFILE_FULL_PATH_MAX_LEN + 1));

    if (PNULL == full_path_name_ptr)
    {
        //no memory
        return SFS_ERROR_INVALID_PARAM;
    }

    SCI_MEMSET (full_path_name_ptr, 0x00, sizeof (wchar) * (MMIFILE_FULL_PATH_MAX_LEN + 1));
    tree_len = MMIAPICOM_Wstrlen (pathName);

    if (tree_len < MMIFILE_FULL_PATH_MAX_LEN)
    {
        //combine root name
        MMIAPICOM_Wstrncpy (full_path_name_ptr, pathName,  tree_len);
    }

    tree_len += 1;

    if (tree_len < MMIFILE_FULL_PATH_MAX_LEN)
    {
        //combine "\"
        MMIAPICOM_Wstrncat (full_path_name_ptr, slash_name,  1);
    }

    tree_len += MMIAPICOM_Wstrlen (fileName);

    if (tree_len < MMIFILE_FULL_PATH_MAX_LEN)
    {
        //combine folder name
        MMIAPICOM_Wstrncat (full_path_name_ptr, fileName,  MMIAPICOM_Wstrlen (fileName));
    }

    if (tree_len >= MMIFILE_FULL_PATH_MAX_LEN)
    {
        //MMIAPIFMM_FindClose(file_handle);
        SCI_FREE (full_path_name_ptr);
        return SFS_ERROR_INVALID_PARAM;
    }

    file_ret = SFS_FindFirstFileCtrl (full_path_name_ptr , filter, ctlStr, &file_handle, &find_data, PNULL);

    if (SFS_INVALID_HANDLE != file_handle && (SFS_ERROR_NONE == file_ret))
    {
        do
        {
            //find one sub folder or file to delete it
            SCI_MEMSET (&full_path_name_ptr[tree_len], 0x00, (sizeof (wchar) * (MMIFILE_FULL_PATH_MAX_LEN + 1 - tree_len)));
            file_len = MMIAPICOM_Wstrlen (find_data.name);

            if (file_len + tree_len + 1 <= MMIFILE_FULL_PATH_MAX_LEN)
            {
                if ( (find_data.attr & SFS_ATTR_DIR) > 0)
                {
                    MMIFILE_ERROR_E error = 0;
                    SCI_TRACE_LOW("[MMIAPIFMM_DeleteAllTreeEx]folder");
                    //folder
                    error = MMIAPIFMM_DeleteTreeEx(full_path_name_ptr, find_data.name, TRUE);
                    SCI_TRACE_LOW("[MMIAPIFMM_DeleteAllTreeEx]del folder result:%d", error);
                }
                else
                {
                    //file
                    SCI_TRACE_LOW("[MMIAPIFMM_DeleteAllTreeEx]file");
                    MMIAPICOM_Wstrncat (full_path_name_ptr, slash_name,  1);
                    MMIAPICOM_Wstrncat (&full_path_name_ptr[tree_len], find_data.name,  file_len);
                    file_ret = SFS_DeleteFile (full_path_name_ptr, PNULL);
                    SCI_TRACE_LOW("[MMIAPIFMM_DeleteAllTreeEx]del file result:%d", file_ret);
                }
            }
        }
        while ( (SFS_NO_ERROR == MMIAPIFMM_FindNextFile (file_handle, &find_data)));
    }

    MMIAPIFMM_FindClose (file_handle);

    if (is_del_dir)
    {
        //delete folder itself
        full_path_name_ptr[tree_len] = 0;
        file_ret = SFS_DeleteDirectory (full_path_name_ptr);
    }

    SCI_FREE (full_path_name_ptr);
    return file_ret;
}

    /*****************************************************************************/
//  Description : delete folder and it's files in it, only can delete it's sub empty folder
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_DeleteTree (
        const wchar *pathName,
        const wchar *fileName)
    {
        return MMIAPIFMM_DeleteTreeEx (pathName, fileName, TRUE);
    }

    /*****************************************************************************/
//  Description : create file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_CreateFileAsyn (
        const wchar *file_name,
        uint32 access_mode,
        uint32 share_mode,  //Must be NULL,File System have not realize it
        uint32 file_attri ,      //Must be NULL,File System have not realize it
        MMI_WIN_ID_T            win_id,   //in
        MMI_MESSAGE_ID_E        msg_id,    //in
        MMIFILE_HANDLE        *file_handle_ptr//OUT
    )
    {
        uint16    path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
        uint16    path_len = 0;
        uint16    full_path_len = 0;
        SFS_ERROR_E ret_val = SFS_ERROR_INVALID_PARAM;
        SFS_OVERLAPPED_T            overLapped = {0};
        MMIFILE_ASYN_CALLBACK_T     *callback_ptr = NULL;

        //SCI_ASSERT(PNULL != file_name);
        if (PNULL == file_name)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_CreateFileAsyn param error: PNULL == file_name"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3687_112_2_18_2_19_54_121, (uint8*) "");
            return SFS_ERROR_INVALID_PARAM;
        }


        full_path_len = MMIAPICOM_Wstrlen (file_name);
        MMIAPICOM_GetFilePath (file_name, full_path_len, path_name, &path_len);

        if (0 ==  full_path_len)
        {
            ret_val = SFS_ERROR_INVALID_PARAM;
        }
        else
        {
            if (full_path_len >= path_len + 2)
            {
                if ( (0 != msg_id) && (0 != win_id))
                {
                    callback_ptr = (MMIFILE_ASYN_CALLBACK_T *) SCI_ALLOCA (sizeof (MMIFILE_ASYN_CALLBACK_T));

                    //SCI_ASSERT(NULL != callback_ptr);/* assert verified */
                    if (PNULL == callback_ptr)
                    {
                        //SCI_TRACE_LOW:"MMIFMM:MMIAPIFMM_CreateFileAsyn PNULL == callback_ptr is error!"
                        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3709_112_2_18_2_19_54_122, (uint8*) "");
                        return SFS_ERROR_NO_ENOUGH_RESOURCE;
                    }

                    callback_ptr->handle = 0;
                    callback_ptr->msg_id = msg_id;
                    callback_ptr->win_id = win_id;

                    overLapped.param = (uint32) callback_ptr;
                    overLapped.complete_route = CreateFileAsynCallback;
                    ret_val = SFS_CreateFileAsyn (path_name, & (file_name[path_len + 1]), access_mode, share_mode, file_attri, file_handle_ptr, &overLapped);

                }
                else
                {
                    ret_val = SFS_CreateFileAsyn (path_name, & (file_name[path_len + 1]), access_mode, share_mode, file_attri, file_handle_ptr, PNULL);

                }
            }
        }

        return ret_val;
    }

    /*****************************************************************************/
//  Description : delete file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_DeleteFile (
        const wchar *name,
        MMIFILE_OVERLAPPED_T *overlapped_ptr
    )
    {

        return MMIFILE_DeleteFile (name, overlapped_ptr);
    }

    /*****************************************************************************/
//  Description : create directiory
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_CreateDirectory (
        const wchar *path
    )
    {
        MMIFILE_ERROR_E error = SFS_NO_ERROR;

        //SCI_ASSERT(PNULL != path);
        if (PNULL == path)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_CreateDirectory param error:PNULL == path"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3757_112_2_18_2_19_54_123, (uint8*) "");
            return SFS_ERROR_INVALID_PARAM;
        }

        error = SFS_CreateDirectory (path);

        return error;
    }

    /*****************************************************************************/
//  Description : delete diretory
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_DeleteDirectory (
        const wchar *path
    )
    {
        return MMIFILE_DeleteDirectory (path);
    }

    /*****************************************************************************/
//  Description : find first file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_HANDLE MMIAPIFMM_FindFirstFile (
        const wchar *find,
        uint16 find_len,
        MMIFILE_FIND_DATA_T *fd
    )
    {
        return MMIFILE_FindFirstFile (find, find_len, fd);
    }

    /*****************************************************************************/
//  Description : find nezt file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_FindNextFile (
        MMIFILE_HANDLE file_handle,
        MMIFILE_FIND_DATA_T *fd
    )
    {
        return MMIFILE_FindNextFile (file_handle, fd);
    }

    /*****************************************************************************/
//  Description : find close
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_FindClose (
        MMIFILE_HANDLE file_handle
    )
    {
        return MMIFILE_FindClose (file_handle);
    }

    /*****************************************************************************/
//  Description : get file size
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    uint32 MMIAPIFMM_GetFileSize (
        MMIFILE_HANDLE file_handle
    )
    {
        return MMIFILE_GetFileSize (file_handle);
    }

    /*****************************************************************************/
//  Description : set file size
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_SetFileSize (
        MMIFILE_HANDLE file_handle,
        uint32 size
    )
    {
        MMIFILE_ERROR_E error = SFS_NO_ERROR;

        //SCI_ASSERT(PNULL != file_handle);
        if (PNULL == file_handle)
        {
            //SCI_TRACE_LOW:"MMIFILE_SetFileSize param error: PNULL == file_handle"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3838_112_2_18_2_19_54_124, (uint8*) "");
            return SFS_ERROR_INVALID_PARAM;
        }

        error = SFS_SetFileSize (file_handle, size);

        return error;
    }

    /*****************************************************************************/
//  Description : read file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_ReadFile (
        MMIFILE_HANDLE file_handle,
        void *buffer,
        uint32 bytes_to_read,
        uint32 *bytes_read,
        MMIFILE_OVERLAPPED_T *overlapped
    )
    {
        return MMIFILE_ReadFile (file_handle, buffer, bytes_to_read, bytes_read, overlapped);
    }

    /*****************************************************************************/
//  Description : write file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_WriteFile (
        MMIFILE_HANDLE file_handle,
        const void *buffer,
        uint32 bytes_to_write,
        uint32 *bytes_written,
        MMIFILE_OVERLAPPED_T *overlapped
    )
    {
        return MMIFILE_WriteFile (file_handle, buffer, bytes_to_write, bytes_written, overlapped);
    }

    /*****************************************************************************/
//  Description : set file pointer
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    uint32 MMIAPIFMM_SetFilePointer (
        MMIFILE_HANDLE file_handle,
        int32 offset,
        uint32 origin
    )
    {
        uint32 len = 0;

        //SCI_ASSERT(PNULL != file_handle);
        if (PNULL == file_handle)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_SetFilePointer param error:PNULL == file_handle"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3892_112_2_18_2_19_54_125, (uint8*) "");
            return len;
        }

        len = SFS_SetFilePointer (
                  file_handle,
                  offset,         /*lint !e747*/
                  origin
              );

        return len;
    }

    /*****************************************************************************/
//  Description : get file pointer
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    int32 MMIAPIFMM_GetFilePointer (
        MMIFILE_HANDLE file_handle,
        uint32 origin
    )
    {
        int32 len = 0;

        //SCI_ASSERT(PNULL != file_handle);
        if (PNULL == file_handle)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_GetFilePointer param error:PNULL == file_handle"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3919_112_2_18_2_19_54_126, (uint8*) "");
            return -1;
        }

        if (SFS_NO_ERROR != SFS_GetFilePointer (file_handle, origin, &len))
        {
            len = -1;
        }

        return len;
    }

    /*****************************************************************************/
//  Description : close file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIAPIFMM_CloseFile (
        MMIFILE_HANDLE file_handle
    )
    {
        return MMIFILE_CloseFile (file_handle);
    }

    /*****************************************************************************/
//  Description : is file end
//  Global resource dependence:
//  Author: haiyang.hu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_IsEndOfFile ( //TRUE: cur position is the end of the file.
        SFS_HANDLE handle
    )
    {
        int32 cur_pos = 0;
        uint32 file_total_size = 0;

        if (SFS_NO_ERROR == SFS_GetFilePointer (
                    handle,
                    SFS_SEEK_BEGIN,
                    &cur_pos
                )
           )
        {
            if (SFS_ERROR_NONE != SFS_GetFileSize (handle, &file_total_size))
            {
                file_total_size = 0;
            }

            if (cur_pos >= (int32) file_total_size)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }

        }
        else
        {
            //SCI_TRACE_LOW:"mmi_filemgr.c, MMIAPIFMM_IsEndOfFile(), cur_pos = %X, file_total_size = %X"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_3976_112_2_18_2_19_54_127, (uint8*) "dd", cur_pos, file_total_size);
            return FALSE;
        }
    }

    /*****************************************************************************/
//  Description : Get a char from handle
//  Global resource dependence :
//  Author: haiyang.hu
//  Note:
    /*****************************************************************************/
    PUBLIC SFS_ERROR_E MMIAPIFMM_GetChar (
        SFS_HANDLE handle, //[in]
        void*  char_ptr //[out]
    )
    {
        uint32      size_readed = 0;
        SFS_ERROR_E read_err = SFS_NO_ERROR;

        read_err = SFS_ReadFile (
                       handle,
                       (void *) (char_ptr),
                       1,
                       &size_readed,
                       PNULL
                   );

        return read_err;
    }

    /*****************************************************************************/
//  Description : check and get validate device
//  Global resource dependence :
//  Author: gang.tong
//  Note: 指定设备类型无效时，依次从SD卡向UDISK查找设备
    /*****************************************************************************/
    MMIFILE_DEVICE_E MMIAPIFMM_GetValidateDevice (MMIFILE_DEVICE_E original_device)
    {
        MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;

        if (!MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (original_device), MMIAPIFMM_GetDevicePathLen (original_device)))
        {
            for (device_type = MMI_DEVICE_NUM - 1; device_type >= MMI_DEVICE_UDISK; device_type--)
            {
                if (MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (device_type), MMIAPIFMM_GetDevicePathLen (device_type)))
                {
                    break;
                }
            }

            if (MMI_DEVICE_SYSTEM == device_type)
            {
                //未找到合适设备
                device_type = MMI_DEVICE_NUM;
            }
        }
        else
        {
            device_type = original_device;
        }

        //SCI_TRACE_LOW:"[MMI FILE]: MMIAPIFMM_GetValidateDevice, ret_dev %x"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_4033_112_2_18_2_19_55_128, (uint8*) "d" , device_type);
        return device_type;
    }

    /*****************************************************************************/
//  Description : Callback
//  Global resource dependence :
//  Author:jian.ma
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIAPIFMM_Callback (uint16     result)
    {
        //SCI_TRACE_LOW:"[MMIFILE]: MMIAPIFMM_Callback result = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_4043_112_2_18_2_19_55_129, (uint8*) "d", result);

        if (SFS_NO_ERROR == result)
        {
            MMIPUB_OpenAlertSuccessWin (TXT_SUCCESS);
        }
        else if (SFS_ERROR_NOT_EXIST == result)
        {
            MMIPUB_OpenAlertFailWin (TXT_REJECT);
        }
        else
        {
            MMIPUB_OpenAlertFailWin (TXT_ERROR);
        }

        //APPFILE_ExitTask();
    }

    /*****************************************************************************/
//  Description : set sd device status
//  Author:apple.zhang
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIFILE_SetDeviceSDStatus (BOOLEAN is_ready)
    {
        s_is_sdready = is_ready;
        return TRUE;
    }

/*****************************************************************************/
//  Description : get sd device status
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFILE_GetDeviceSDStatus(void)
{
    return s_is_sdready;
}

//===========================

    /*****************************************************************************/
// 	Description: get the prefer disk
//	Global resource dependence:
//  Author: bin.ji
//	Note:
    /*****************************************************************************/
    PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetDefaultDisk (void)
    {
        //MMIFILE_DEVICE_E dev_type = MMI_DEVICE_NUM;

        MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;

        for (device_type = MMI_DEVICE_NUM - 1; device_type >= MMI_DEVICE_UDISK; device_type--)
        {
            if (MMIAPIFMM_GetDeviceStatus ( (uint16 *) MMIAPIFMM_GetDevicePath (device_type), MMIAPIFMM_GetDevicePathLen (device_type)))
            {
                break;
            }
        }

        if (device_type < MMI_DEVICE_UDISK)
        {
#ifdef NANDBOOT_SUPPORT
            device_type = MMI_DEVICE_UDISK;
#else
            device_type = MMI_DEVICE_SDCARD;
#endif
        }

        //SCI_TRACE_LOW:"[mmi_filemgr.c]:MMIAPIFMM_GetDefaultDisk dev_type=%d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_4094_112_2_18_2_19_55_130, (uint8*) "d", device_type);
        return device_type;

    }

    /*****************************************************************************/
// 	Description: get the number of all available disks
//	Global resource dependence:
//  Author: yaye.jiang
//	Note:
    /*****************************************************************************/
    PUBLIC uint8 MMIAPIFMM_GetAllAvarialeDiskNum (void)
    {
        MMIFILE_DEVICE_E	device_type				= 0;
        uint8 				count					= 0;

        for (device_type = MMI_DEVICE_UDISK; device_type < MMI_DEVICE_NUM; device_type++)
        {
            if (MMIAPIFMM_GetDeviceStatus ( (uint16 *) MMIAPIFMM_GetDevicePath (device_type), MMIAPIFMM_GetDevicePathLen (device_type)))
            {
                count++;
            }
        }

        return count;
    }

    /*****************************************************************************/
// 	Description: get available device
//	Global resource dependence:
//  Author: bin.ji
//	Note:
    /*****************************************************************************/
    LOCAL MMIFILE_DEVICE_E GetAvailableDevice (
        MMIFMM_STORE_TYPE_E store_type,
        MMIFILE_DEVICE_E  prefer_devie,
        uint32 min_free_size
    )
    {
        MMIFILE_DEVICE_E dev_type = MMI_DEVICE_NUM;
        uint32 free_high_word = 0;
        uint32 free_low_word = 0;
        //wchar*	device_ptr = PNULL;
        //uint16	device_len = 0;

        switch (store_type)
        {
        case MMIFMM_STORE_TYPE_FIXED:
            MMIAPIFMM_GetDeviceFreeSpace (
                (uint16 *) MMIAPIFMM_GetDevicePath (prefer_devie),
                MMIAPIFMM_GetDevicePathLen (prefer_devie),
                &free_high_word,
                &free_low_word
            );

            if (0 < free_high_word || free_low_word >= min_free_size)
            {
                dev_type = prefer_devie;
            }

            break;

        case MMIFMM_STORE_TYPE_PREFER:

            if (SFS_NO_ERROR != MMIAPIFMM_GetSuitableFileDev (prefer_devie, min_free_size, &dev_type))
            {

                dev_type = MMI_DEVICE_NUM;
            }

            break;

        default:
            break;
        }

        //SCI_TRACE_LOW:"[mmi_filemgr.c]:GetAvailableDevice available device=%d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_4143_112_2_18_2_19_55_131, (uint8*) "d", dev_type);

        return dev_type;
    }

    /*****************************************************************************/
// 	Description: is enough space
//	Global resource dependence:
//  Author: aoke.hu
//	Note:新增参数，获取满足空间要求的盘符.有转储需求的模块可以使用，比如拍照，录像，录音等
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_IsEnoughSpace (
        MMIFMM_STORE_TYPE_E store_type,
        MMIFILE_DEVICE_E prefer_devie,
        uint32 min_free_size,
        MMIFILE_DEVICE_E *device_ptr  //[out]
    )
    {
        MMIFILE_DEVICE_E dev_type = MMI_DEVICE_NUM;
        BOOLEAN return_val = FALSE;

        dev_type = GetAvailableDevice (store_type, prefer_devie, min_free_size);

        if (MMI_DEVICE_NUM != dev_type)
        {
            if (PNULL != device_ptr)
            {
                *device_ptr = dev_type;
            }

            return_val = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"[mmi_filemgr.c]:MMIAPIFMM_IsEnoughSpace is not enough space!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_4173_112_2_18_2_19_55_132, (uint8*) "");
        }

        return return_val;
    }


    /*****************************************************************************/
//  Description : get system file full path for read
//  Author:bin.ji
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForRead (
        BOOLEAN        is_fix_store,           //[IN]
        const wchar    *path_name_ptr,         //[IN]
        uint16         path_name_len,          //[IN]
        const wchar    *file_name_ptr,         //[IN]
        uint16         file_name_len,          //[IN]
        wchar          *full_path_name_ptr,    //[OUT]
        uint16         *full_path_len_ptr      //[OUT]
    )
    {
        return  MMIAPIFMM_GetSysFileFullPathForReadEx (is_fix_store,
                FALSE,
                path_name_ptr,
                path_name_len,
                file_name_ptr,
                file_name_len,
                full_path_name_ptr,
                full_path_len_ptr
                                                      );
    }

    /*****************************************************************************/
//  Description : get system file full path for write
//  Author:bin.ji
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForWrite (
        BOOLEAN        is_fix_store,          //[IN]TRUE, is fixed. FALSE, is portable
        const wchar    *path_name_ptr,        //[IN]
        uint16         path_name_len,         //[IN]
        const wchar    *file_name_ptr,        //[IN]
        uint16         file_name_len,         //[IN]
        uint32         min_free_size,         //[IN]
        wchar          *full_path_name_ptr,   //[OUT]
        uint16         *full_path_len_ptr     //[OUT]
    )
    {
        return MMIAPIFMM_GetSysFileFullPathForWriteEx (is_fix_store,
                FALSE,
                path_name_ptr,
                path_name_len,
                file_name_ptr,
                file_name_len,
                min_free_size,
                full_path_name_ptr,
                full_path_len_ptr
                                                      );
    }

    /*****************************************************************************/
//  Description : get system file full path for read
//  Author:aoke.hu
//  Note: 指定C disk时先确认C, 否则由E到D查找
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForReadEx (
        BOOLEAN        is_fix_store,           //[IN]
        BOOLEAN        is_from_cdisk,          //[IN]
        const wchar    *path_name_ptr,         //[IN]
        uint16         path_name_len,          //[IN]
        const wchar    *file_name_ptr,         //[IN]
        uint16         file_name_len,          //[IN]
        wchar          *full_path_name_ptr,    //[OUT]
        uint16         *full_path_len_ptr      //[OUT]
    )
    {
        wchar   *dst_path_name_ptr = PNULL;
        uint16  dst_path_name_len = 0;
        wchar   syspath_name_ptr[] = {MMIMULTIM_DIR_SYSTEM_CHAR, '\\'};
        uint16  syspath_name_len = (MMIMULTIM_DIR_SYSTEM_LEN + 1);
        BOOLEAN result_val = FALSE;
        MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
        MMIFILE_DEVICE_E first_valid_dev = MMI_DEVICE_NUM;

        dst_path_name_len = (path_name_len + syspath_name_len);
        dst_path_name_ptr = (wchar *) SCI_ALLOCA (sizeof (wchar) * dst_path_name_len);

        if (PNULL == dst_path_name_ptr)
        {
            return FALSE;
        }

        SCI_MEMSET (dst_path_name_ptr, 0x00, sizeof (wchar) *dst_path_name_len);

        MMI_WSTRNCPY (dst_path_name_ptr, dst_path_name_len, syspath_name_ptr, syspath_name_len, syspath_name_len);
        MMI_WSTRNCPY (dst_path_name_ptr + syspath_name_len, path_name_len, path_name_ptr, path_name_len, path_name_len);

        if (is_from_cdisk)
        {
            //check C disk first
            device_type = MMI_DEVICE_SYSTEM;

            if (MMIAPIFMM_GetDeviceStatus ( (uint16 *) MMIAPIFMM_GetDevicePath (device_type), MMIAPIFMM_GetDevicePathLen (device_type)))
            {
                result_val = MMIAPIFMM_CombineFullPath (
                                 (wchar*) MMIAPIFMM_GetDevicePath (device_type),
                                 MMIAPIFMM_GetDevicePathLen (device_type),
                                 dst_path_name_ptr,
                                 dst_path_name_len,
                                 file_name_ptr,
                                 file_name_len,
                                 full_path_name_ptr,
                                 full_path_len_ptr
                             );

                if (MMI_DEVICE_NUM == first_valid_dev) /*lint !e774*/
                {
                    first_valid_dev = device_type;
                }

                if (result_val && MMIAPIFMM_IsFileExist (full_path_name_ptr, *full_path_len_ptr))
                {
                    SCI_FREE (dst_path_name_ptr);
                    return TRUE;
                }
                else
                {
                    result_val = FALSE;
                }
            }
        }

        //check from E to D
        for (device_type = MMI_DEVICE_NUM - 1; device_type >= MMI_DEVICE_UDISK; device_type--)
        {
            if (MMIAPIFMM_GetDeviceStatus ( (uint16 *) MMIAPIFMM_GetDevicePath (device_type), MMIAPIFMM_GetDevicePathLen (device_type)))
            {
                result_val = MMIAPIFMM_CombineFullPath (
                                 (wchar*) MMIAPIFMM_GetDevicePath (device_type),
                                 MMIAPIFMM_GetDevicePathLen (device_type),
                                 dst_path_name_ptr,
                                 dst_path_name_len,
                                 file_name_ptr,
                                 file_name_len,
                                 full_path_name_ptr,
                                 full_path_len_ptr
                             );

                if (MMI_DEVICE_NUM == first_valid_dev)
                {
                    first_valid_dev = device_type;
                }

                if (result_val && MMIAPIFMM_IsFileExist (full_path_name_ptr, *full_path_len_ptr))
                {
                    SCI_FREE (dst_path_name_ptr);
                    return TRUE;
                }
                else
                {
                    result_val = FALSE;
                }
            }
        }

        if (!result_val)
        {
            //must return a valid full path

            if (MMI_DEVICE_NUM == first_valid_dev)
            {
                first_valid_dev = MMI_DEVICE_SDCARD;
            }

            result_val = MMIAPIFMM_CombineFullPath (
                             MMIAPIFMM_GetDevicePath ( (first_valid_dev)),
                             MMIAPIFMM_GetDevicePathLen ( (first_valid_dev)),
                             dst_path_name_ptr,
                             dst_path_name_len,
                             file_name_ptr,
                             file_name_len,
                             full_path_name_ptr,
                             full_path_len_ptr
                         );
        }

        SCI_FREE (dst_path_name_ptr);

        return result_val;
    }


    /*****************************************************************************/
//  Description : get system file full path for write
//  Author:aoke.hu
//  Note: check from MMI_DEVICE_SYSTEM
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForWriteEx (
        BOOLEAN        is_fix_store,          //[IN]TRUE, is fixed. FALSE, is portable
        BOOLEAN        is_from_cdisk,         //[IN]
        const wchar    *path_name_ptr,        //[IN]
        uint16         path_name_len,         //[IN]
        const wchar    *file_name_ptr,        //[IN]
        uint16         file_name_len,         //[IN]
        uint32         min_free_size,         //[IN]
        wchar          *full_path_name_ptr,   //[OUT]
        uint16         *full_path_len_ptr     //[OUT]
    )
    {
        wchar   *dst_path_name_ptr = PNULL;
        uint16  dst_path_name_len = 0;
        wchar   syspath_name_ptr[] = {MMIMULTIM_DIR_SYSTEM_CHAR, '\\'};
        uint16  syspath_name_len = (MMIMULTIM_DIR_SYSTEM_LEN + 1);
        BOOLEAN result_val = FALSE;
        MMIFILE_DEVICE_E device_type = MMI_DEVICE_NUM;
        MMIFILE_DEVICE_E dev_type = MMI_DEVICE_NUM;

        dst_path_name_len = (path_name_len + syspath_name_len);
        dst_path_name_ptr = (wchar *) SCI_ALLOCA (sizeof (wchar) * dst_path_name_len);

        if (PNULL == dst_path_name_ptr)
        {
            return result_val;
        }

        SCI_MEMSET (dst_path_name_ptr, 0, (sizeof (wchar) *dst_path_name_len));
        MMI_WSTRNCPY (dst_path_name_ptr, dst_path_name_len, syspath_name_ptr, syspath_name_len, syspath_name_len);
        MMI_WSTRNCPY (dst_path_name_ptr + syspath_name_len, path_name_len, path_name_ptr, path_name_len, path_name_len);

        if (is_from_cdisk)
        {
            device_type = MMI_DEVICE_SYSTEM;
        }
        else
        {
            device_type = MMI_DEVICE_NUM - 1;
        }

        if (SFS_ERROR_NONE == MMIAPIFMM_GetSuitableFileDev (device_type, min_free_size, &dev_type))
        {
            result_val = MMIAPIFMM_CombineFullPath (
                             MMIAPIFMM_GetDevicePath (dev_type),
                             MMIAPIFMM_GetDevicePathLen (dev_type),
                             dst_path_name_ptr,
                             dst_path_name_len,
                             file_name_ptr,
                             file_name_len,
                             full_path_name_ptr,
                             full_path_len_ptr
                         );
        }

        SCI_FREE (dst_path_name_ptr);

        return result_val;
    }

/*****************************************************************************/
//  Description : get system file full path for read
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForReadUdiskPrefer(
                                                 BOOLEAN        is_fix_store,           //[IN]
                                                 BOOLEAN        is_from_cdisk,          //[IN]
                                                 const wchar    *path_name_ptr,         //[IN]
                                                 uint16         path_name_len,          //[IN]                                                 
                                                 const wchar    *file_name_ptr,         //[IN]
                                                 uint16         file_name_len,          //[IN]
                                                 wchar          *full_path_name_ptr,    //[OUT]
                                                 uint16         *full_path_len_ptr      //[OUT]                                                 
                                                 )
{
    wchar   *dst_path_name_ptr = PNULL;
    uint16  dst_path_name_len = 0;
    wchar   syspath_name_ptr[] = {MMIMULTIM_DIR_SYSTEM_CHAR, '\\'};
    uint16  syspath_name_len = (MMIMULTIM_DIR_SYSTEM_LEN+1);
    BOOLEAN result_val = FALSE;
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E first_valid_dev = MMI_DEVICE_NUM;

    dst_path_name_len = (path_name_len + syspath_name_len);
    dst_path_name_ptr = (wchar *)SCI_ALLOCA(sizeof(wchar)*dst_path_name_len);
    if (PNULL == dst_path_name_ptr)
    {
        return FALSE;
    }
    SCI_MEMSET(dst_path_name_ptr, 0x00, sizeof(wchar)*dst_path_name_len);

    MMI_WSTRNCPY(dst_path_name_ptr, dst_path_name_len, syspath_name_ptr, syspath_name_len, syspath_name_len);
    MMI_WSTRNCPY(dst_path_name_ptr+syspath_name_len, path_name_len, path_name_ptr, path_name_len, path_name_len);

    if (is_from_cdisk)
    {
        //check C disk first
        device_type = MMI_DEVICE_SYSTEM;

        if (MMIAPIFMM_GetDeviceStatus((uint16 *)MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
        {
            result_val = MMIAPIFMM_CombineFullPath(
                (wchar*)MMIAPIFMM_GetDevicePath(device_type), 
                MMIAPIFMM_GetDevicePathLen(device_type),
                dst_path_name_ptr, 
                dst_path_name_len,
                file_name_ptr, 
                file_name_len,
                full_path_name_ptr,
                full_path_len_ptr
                );
            
            if (MMI_DEVICE_NUM == first_valid_dev)/*lint !e774*/
            {
                first_valid_dev = device_type;
            }
            
            if (result_val && MMIAPIFMM_IsFileExist(full_path_name_ptr,*full_path_len_ptr))
            {
                SCI_FREE(dst_path_name_ptr);
                return TRUE;
            }
            else
            {
                result_val = FALSE;
            }
        }
    }

    if (is_fix_store)
    {
        device_type = MMI_DEVICE_UDISK;

        if (MMIAPIFMM_GetDeviceStatus((uint16 *)MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
        {
            result_val = MMIAPIFMM_CombineFullPath(
                (wchar*)MMIAPIFMM_GetDevicePath(device_type), 
                MMIAPIFMM_GetDevicePathLen(device_type),
                dst_path_name_ptr, 
                dst_path_name_len,
                file_name_ptr, 
                file_name_len,
                full_path_name_ptr,
                full_path_len_ptr
                );
        } 

        SCI_FREE(dst_path_name_ptr);

        return result_val;
    }
    else
    {
        //check from D to E
        for(device_type = MMI_DEVICE_UDISK; device_type < MMI_DEVICE_NUM; device_type++)
        {
            if (MMIAPIFMM_GetDeviceStatus((uint16 *)MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
            {
                result_val = MMIAPIFMM_CombineFullPath(
                    (wchar*)MMIAPIFMM_GetDevicePath(device_type), 
                    MMIAPIFMM_GetDevicePathLen(device_type),
                    dst_path_name_ptr, 
                    dst_path_name_len,
                    file_name_ptr, 
                    file_name_len,
                    full_path_name_ptr,
                    full_path_len_ptr
                    );
                
                if (MMI_DEVICE_NUM == first_valid_dev)
                {
                    first_valid_dev = device_type;
                }
                
                if (result_val && MMIAPIFMM_IsFileExist(full_path_name_ptr,*full_path_len_ptr))
                {
                    SCI_FREE(dst_path_name_ptr);
                    return TRUE;
                }
                else
                {
                    result_val = FALSE;
                }
            } 
        }
    }
    
    if (!result_val)
    {
        //must return a valid full path
        if (MMI_DEVICE_NUM == first_valid_dev)
        {
            first_valid_dev = MMI_DEVICE_UDISK;
        }
        
        result_val = MMIAPIFMM_CombineFullPath(
            MMIAPIFMM_GetDevicePath((first_valid_dev)), 
            MMIAPIFMM_GetDevicePathLen((first_valid_dev)),
            dst_path_name_ptr, 
            dst_path_name_len,
            file_name_ptr, 
            file_name_len,
            full_path_name_ptr,
            full_path_len_ptr
            );   
    }
    
    SCI_FREE(dst_path_name_ptr);
    
    return result_val;
}

/*****************************************************************************/
//  Description : get system file full path for write
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForWriteUdiskPrefer(
                                                  BOOLEAN        is_fix_store,          //[IN]TRUE, is fixed. FALSE, is portable
                                                  BOOLEAN        is_from_cdisk,         //[IN]
                                                  const wchar    *path_name_ptr,        //[IN]
                                                  uint16         path_name_len,         //[IN]                                                 
                                                  const wchar    *file_name_ptr,        //[IN]
                                                  uint16         file_name_len,         //[IN]
                                                  uint32         min_free_size,         //[IN]
                                                  wchar          *full_path_name_ptr,   //[OUT]
                                                  uint16         *full_path_len_ptr     //[OUT]                                                 
                                                  )
{
    wchar   *dst_path_name_ptr = PNULL;
    uint16  dst_path_name_len = 0;
    wchar   syspath_name_ptr[] = {MMIMULTIM_DIR_SYSTEM_CHAR, '\\'};
    uint16  syspath_name_len = (MMIMULTIM_DIR_SYSTEM_LEN+1);
    BOOLEAN result_val = FALSE;
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_NUM;
    MMIFILE_DEVICE_E dev_type = MMI_DEVICE_NUM;
    
    dst_path_name_len = (path_name_len + syspath_name_len);
    dst_path_name_ptr = (wchar *)SCI_ALLOCA(sizeof(wchar)*dst_path_name_len);
    if (PNULL == dst_path_name_ptr)
    {
        return result_val;
    }
    SCI_MEMSET(dst_path_name_ptr,0,(sizeof(wchar)*dst_path_name_len));
    MMI_WSTRNCPY(dst_path_name_ptr, dst_path_name_len, syspath_name_ptr, syspath_name_len, syspath_name_len);
    MMI_WSTRNCPY(dst_path_name_ptr+syspath_name_len, path_name_len, path_name_ptr, path_name_len, path_name_len);
    
    if (is_from_cdisk)
    {
        device_type = MMI_DEVICE_SYSTEM;
    }
    else
    {
        device_type = MMI_DEVICE_UDISK;
    }
    
    if (SFS_ERROR_NONE == MMIAPIFMM_GetSuitableFileDev(device_type, min_free_size, &dev_type))
    {
        result_val = MMIAPIFMM_CombineFullPath(
            MMIAPIFMM_GetDevicePath(dev_type), 
            MMIAPIFMM_GetDevicePathLen(dev_type),
            dst_path_name_ptr, 
            dst_path_name_len,
            file_name_ptr, 
            file_name_len,
            full_path_name_ptr,
            full_path_len_ptr
            );
    }
    
    SCI_FREE(dst_path_name_ptr);    
    
    return result_val;
}

    /*****************************************************************************/
//  Description : create sysfile dir, if success return TRUE, else return FALSE
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_CreateSysFileDir (
        MMIFILE_DEVICE_E    dev_type,               //[IN]
        const wchar     *path_name_ptr,         //[IN]
        uint16          path_name_len,          //[IN]
        MMIFILE_ERROR_E *file_err_ptr           //[OUT] can be null
    )
    {
        BOOLEAN         return_val = TRUE;
        wchar           *device_ptr = PNULL;
        uint16          device_len = 0;
        wchar           *dst_path_name_ptr = PNULL;
        uint16          dst_path_name_len = 0;
        wchar           syspath_name_ptr[] = {MMIMULTIM_DIR_SYSTEM_CHAR, '\\'};
        uint16          syspath_name_len = (MMIMULTIM_DIR_SYSTEM_LEN + 1);
        wchar           full_path_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
        uint16          full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIFILE_ERROR_E file_err = SFS_ERROR_NONE;

        dst_path_name_len = (path_name_len + syspath_name_len);
        dst_path_name_ptr = (wchar*) SCI_ALLOCA (sizeof (wchar) * dst_path_name_len);

        if (PNULL == dst_path_name_ptr)
        {
            return FALSE;
        }

        SCI_MEMSET (dst_path_name_ptr, 0, (sizeof (wchar) *dst_path_name_len));

        MMI_WSTRNCPY (dst_path_name_ptr, dst_path_name_len, syspath_name_ptr, syspath_name_len, syspath_name_len);
        MMI_WSTRNCPY (dst_path_name_ptr + syspath_name_len, path_name_len, path_name_ptr, path_name_len, path_name_len);

//     if(FS_UDISK == dev_type)
//     {
//         device_ptr = (wchar*)MMIFILE_DEVICE_UDISK;
//         device_len = MMIFILE_DEVICE_UDISK_LEN;
//     }
//     else if(FS_MMC == dev_type)
//     {
//         device_ptr = (wchar*)MMIFILE_DEVICE_SDCARD;
//         device_len = MMIFILE_DEVICE_SDCARD_LEN;
//     }
//     else

        device_ptr = MMIAPIFMM_GetDevicePath ( (dev_type));
        device_len = MMIAPIFMM_GetDevicePathLen ( (dev_type));

        if (device_ptr == PNULL)
        {

            file_err = SFS_ERROR_INVALID_PARAM;
            return_val = FALSE;
        }

        if (return_val)
        {
            MMIAPIFMM_CombineFullPath (
                device_ptr,
                device_len,
                dst_path_name_ptr,
                dst_path_name_len,
                PNULL,
                0,
                full_path_name,
                &full_path_len
            );

            if (!MMIAPIFMM_IsFolderExist (full_path_name, full_path_len))
            {
                //SCI_TRACE_LOW:"[mmi_filemgr.c]:MMIAPIFMM_CreateSysFileDir sysfile folder not exist!!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_4471_112_2_18_2_19_55_133, (uint8*) "");
                MMIAPIFMM_InitSystemFolder (device_ptr, device_len,
                                            MMIMULTIM_DIR_SYSTEM, MMIMULTIM_DIR_SYSTEM_LEN);

                file_err = SFS_CreateDirectory (full_path_name);

                if (SFS_ERROR_NONE != file_err)
                {
                    //SCI_TRACE_LOW:"MMIAPIFMM_CreateSysFileDir: create dir fail"
                    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_4478_112_2_18_2_19_55_134, (uint8*) "");
                    return_val = FALSE;
                }
            }
        }

        SCI_FREE (dst_path_name_ptr);

        if (PNULL != file_err_ptr)
        {
            *file_err_ptr = file_err;
        }

        return return_val;

    }

    /*****************************************************************************/
//  Description : Combine Sys Path
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_CombineSysPath (
        MMIFILE_DEVICE_E    dev_type,              //[IN]
        const wchar    *path_name_ptr,         //[IN]
        uint16         path_name_len,          //[IN]
        const wchar    *file_name_ptr,         //[IN]
        uint16         file_name_len,          //[IN]
        wchar          *full_path_name_ptr,    //[OUT]
        uint16         *full_path_len_ptr      //[OUT]
    )
    {
        wchar   *dst_path_name_ptr = PNULL;
        uint16  dst_path_name_len = 0;
        wchar   syspath_name_ptr[] = {MMIMULTIM_DIR_SYSTEM_CHAR, '\\'};
        uint16  syspath_name_len = (MMIMULTIM_DIR_SYSTEM_LEN + 1);
        wchar*	device_ptr = PNULL;
        uint16	device_len = 0;
        BOOLEAN result_val = TRUE;

        dst_path_name_len = (path_name_len + syspath_name_len);
        dst_path_name_ptr = (wchar *) SCI_ALLOCA (sizeof (wchar) * dst_path_name_len);

        if (PNULL == dst_path_name_ptr)
        {
            return FALSE;
        }

        SCI_MEMSET (dst_path_name_ptr, 0x00, sizeof (wchar) *dst_path_name_len);

        MMI_WSTRNCPY (dst_path_name_ptr, dst_path_name_len, syspath_name_ptr, syspath_name_len, syspath_name_len);
        MMI_WSTRNCPY (dst_path_name_ptr + syspath_name_len, path_name_len, path_name_ptr, path_name_len, path_name_len);

        device_ptr = MMIAPIFMM_GetDevicePath (dev_type);
        device_len = MMIAPIFMM_GetDevicePathLen (dev_type);

        if (PNULL == device_ptr)
        {
            result_val = FALSE;
        }

        if (result_val)
        {
            result_val = MMIAPIFMM_CombineFullPath (
                             device_ptr,
                             device_len,
                             dst_path_name_ptr,
                             dst_path_name_len,
                             file_name_ptr,
                             file_name_len,
                             full_path_name_ptr,
                             full_path_len_ptr
                         );
        }

        SCI_FREE (dst_path_name_ptr);

        return result_val;

    }

    /*****************************************************************************/
//  Description : append file
//  Global resource dependence :
//  Author:
//  Note: append append_file_name from append_postion to base_file_name,
//        if is_delete_append_file is TRUE, after append, the append_file must be deleted
//  Return: return file result
    /*****************************************************************************/
    MMIFILE_ERROR_E  MMIAPIFMM_AppendFile (
        wchar *base_file_name,
        wchar *append_file_name,
        uint32 append_postion,
        BOOLEAN is_delete_append_file,
        uint32  *appended_size_ptr
    )
    {
        MMIFILE_HANDLE  base_file_handle = PNULL;
        MMIFILE_HANDLE  append_file_handle = PNULL;
        MMIFILE_ERROR_E file_ret = SFS_ERROR_SYSTEM;
        uint32          file_size = 0;
        uint32          rw_size = 0;
        uint32          write_size = 0;
        uint32          total_write_size = 0;
        uint8           *rw_buf_ptr = PNULL;

        if (PNULL == base_file_name || PNULL == append_file_name)
        {
            //SCI_TRACE_LOW:"[MMIFILE] MMIAPIFMM_AppendFile file name invalid !"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_4594_112_2_18_2_19_56_135, (uint8*) "");
            return 0;
        }

        //open source file to append
        base_file_handle = SFS_CreateFile (base_file_name, SFS_MODE_WRITE | SFS_MODE_APPEND, NULL, NULL);/*lint !e655*/

        if (SFS_INVALID_HANDLE != base_file_handle)
        {
            //open file to read
            append_file_handle = SFS_CreateFile (append_file_name, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

            if (SFS_INVALID_HANDLE != append_file_handle)
            {
                file_ret = SFS_GetFileSize (append_file_handle, &file_size);

                if (SFS_ERROR_NONE == file_ret)
                {
                    if (append_postion < file_size)
                    {
                        //has data to append
                        if (append_postion > 0)
                        {
                            SFS_SetFilePointer (append_file_handle, append_postion, SFS_SEEK_BEGIN);/*lint !e747*/
                        }

                        rw_size = MIN ( (file_size - append_postion), MMIFILE_COPY_BUFFER_SIZE);
                        rw_buf_ptr = (uint8 *) SCI_ALLOC_APP (rw_size);
                        file_size = (file_size - append_postion);
                        total_write_size = 0;

                        if (PNULL != rw_buf_ptr)
                        {
                            //append data to file
                            while (total_write_size < file_size)
                            {
                                file_ret = SFS_ReadFile (append_file_handle, rw_buf_ptr, rw_size, &write_size, PNULL);

                                if (SFS_ERROR_NONE == file_ret)
                                {
                                    total_write_size += write_size;

                                    if ( (rw_size > write_size || total_write_size >= file_size)
                                            && is_delete_append_file)
                                    {
                                        //read file end,delete append file
                                        MMIAPIFMM_CloseFile (append_file_handle);
                                        SFS_DeleteFile (append_file_name, PNULL);
                                        append_file_handle = PNULL;
                                    }

                                    //append read data
                                    file_ret = SFS_WriteFile (base_file_handle, rw_buf_ptr, write_size, &write_size, PNULL);

                                    if (SFS_ERROR_NONE != file_ret)
                                    {
                                        break;
                                    }
                                }
                                else
                                {
                                    //read file error, or no data to read
                                    if (is_delete_append_file)
                                    {
                                        //delete append file
                                        MMIAPIFMM_CloseFile (append_file_handle);
                                        SFS_DeleteFile (append_file_name, PNULL);
                                        append_file_handle = PNULL;
                                    }

                                    break;
                                }

                            }//while(write_size < file_size)

                            SCI_FREE (rw_buf_ptr);
                        }//if(PNULL != rw_buf_ptr)
                    }
                }

                MMIAPIFMM_CloseFile (append_file_handle);
            }

            MMIAPIFMM_CloseFile (base_file_handle);
        }

        if (PNULL != appended_size_ptr)
        {
            *appended_size_ptr = total_write_size;
        }

        return file_ret;
    }

    /*****************************************************************************/
//  Description : Callback
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC wchar* MMIAPIFMM_GetDevicePath (MMIFILE_DEVICE_E device_type)
    {
        wchar *path = PNULL;

        switch (device_type)
        {
        case MMI_DEVICE_SYSTEM:
            path = g_file_device_sysname;/*lint !e605*/
            break;
        case MMI_DEVICE_UDISK:
            path = g_file_device_udisk;/*lint !e605*/
            break;
        case MMI_DEVICE_SDCARD:
            path = g_file_device_sdcard;/*lint !e605*/
            break;
        case MMI_DEVICE_SDCARD_1:
            path = g_file_device_sdcard1;/*lint !e605*/
            break;
        default:
            break;
        }

        return path;
    }


    /*****************************************************************************/
//  Description : Callback
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC uint16 MMIAPIFMM_GetDevicePathLen (MMIFILE_DEVICE_E device_type)
    {
        uint16 path_len = 0;

        switch (device_type)
        {
        case MMI_DEVICE_SYSTEM:
            path_len = MMIAPICOM_Wstrlen (g_file_device_sysname);
            break;
        case MMI_DEVICE_UDISK:
            path_len = MMIAPICOM_Wstrlen (g_file_device_udisk);
            break;
        case MMI_DEVICE_SDCARD:
            path_len = MMIAPICOM_Wstrlen (g_file_device_sdcard);
            break;
        case MMI_DEVICE_SDCARD_1:
            path_len = MMIAPICOM_Wstrlen (g_file_device_sdcard1);
            break;
        default:
            break;
        }

        return path_len;
    }

    /*****************************************************************************/
//  Description : 根据用户选择的设备类型，返回最适合的设备类型
//  Global resource dependence :
//  Author:
//  Note: 如果用户选择的设备剩余空间满足最小空间要求，返回用户设置；
//        如果用户选择的设备剩余空间不满足满足最小空间要求，返回到一个最大剩余空间的设备类型
//        如果U盘和所有的存储卡都不存在，返回默认的UDISK
    /*****************************************************************************/
    PUBLIC MMIFILE_ERROR_E MMIAPIFMM_GetSuitableFileDev (MMIFILE_DEVICE_E user_sel_dev,//IN:
            uint32 min_free_space,//IN:
            MMIFILE_DEVICE_E *suitable_dev_ptr//OUT:
                                                        )
    {
        uint32 free_space_high = 0;
        uint32 free_space_low = 0;
        uint32 cur_free_space_high = 0;
        uint32 cur_free_space_low = 0;
        //MMIFILE_DEVICE_E suit_disk = MMI_DEVICE_NUM;
        MMIFILE_DEVICE_E sd_type = MMI_DEVICE_SDCARD;
        MMIFILE_DEVICE_E max_freespace_sd = MMI_DEVICE_SDCARD;
        MMIFILE_ERROR_E file_ret = SFS_ERROR_NONE;

        BOOLEAN         is_exist_valid = FALSE;
        BOOLEAN         user_sel_result = TRUE;

        if (suitable_dev_ptr == PNULL)
        {
            return SFS_ERROR_INVALID_PARAM;
        }

        sd_type = user_sel_dev;//MMIFILE_GetDeviceTypeByFileDev(user_sel_dev);

        user_sel_result = MMIAPIFMM_GetDeviceFreeSpace ( (uint16 *) MMIAPIFMM_GetDevicePath (sd_type),
                          MMIAPIFMM_GetDevicePathLen (sd_type),
                          &free_space_high,
                          &free_space_low);

        if (user_sel_result)
        {
            if (0 < free_space_high || free_space_low > min_free_space)
            {
                *suitable_dev_ptr = user_sel_dev;
                file_ret = SFS_ERROR_NONE;
                SCI_TRACE_LOW ("MMIAPIFMM_GetSuitableFileDev user_dev is ok!");
                return file_ret;
            }
        }
        else
        {
            free_space_high = 0;
            free_space_low  = 0;
        }

        //else
        {
            //user setting device space not enoght, find a large space memory
            for (sd_type = MMI_DEVICE_UDISK; sd_type < MMI_DEVICE_NUM; sd_type++)
            {
                if (!MMIAPIFMM_GetDeviceTypeStatus (sd_type))
                {
                    continue;
                }

                file_ret = SFS_GetDeviceFreeSpace ( (uint16 *) MMIAPIFMM_GetDevicePath (sd_type),
                                                    &cur_free_space_high,
                                                    &cur_free_space_low);

                if (file_ret == SFS_ERROR_NONE)
                {
                    is_exist_valid = TRUE;

                    if (cur_free_space_high > free_space_high //高位大于肯定大于
                            || ( (cur_free_space_high == free_space_high) //高位相等，低位大于
                                 && (cur_free_space_low > free_space_low))
                       )
                    {
                        free_space_high = cur_free_space_high;
                        free_space_low = cur_free_space_low;
                        max_freespace_sd = sd_type;
                    }
                }

            }

            if (is_exist_valid)
            {
                file_ret = SFS_ERROR_NONE;
            }

            if (0 == free_space_high && (0 == free_space_low))
            {
                //都满，或者都不存在
                file_ret = SFS_ERROR_NO_SPACE;
                *suitable_dev_ptr  = MMIAPIFMM_GetFirstValidDevice();

            }
            else
            {
                //返回剩余空间最大的存储设备
                if (0 == free_space_high && (free_space_low < min_free_space))
                {
                    //所有的存储设备剩余空间都不足
                    file_ret = SFS_ERROR_NO_SPACE;
                }

                if (MMIAPIFMM_GetDeviceTypeStatus (max_freespace_sd))
                {
                    *suitable_dev_ptr  = max_freespace_sd;
                }
                else
                {
                    *suitable_dev_ptr  = MMIAPIFMM_GetFirstValidDevice();
                }
            }
        }
        //SCI_TRACE_LOW:"[MMIFILE] MMIAPIFMM_GetSuitableFileDev user_sel_dev %d min_free_space %d return dev %d file_ret %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_4820_112_2_18_2_19_56_136, (uint8*) "dddd", user_sel_dev, min_free_space, *suitable_dev_ptr, file_ret);

        if (SFS_ERROR_INVALID_PARAM == file_ret)
        {
            file_ret = SFS_ERROR_DEVICE;
        }

        return file_ret;
    }

    /*****************************************************************************/
//  Description : 根据设备类型返回给设备的TEXT ID
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceName (MMIFILE_DEVICE_E device_type)
    {
        MMI_TEXT_ID_T  text_id = TXT_NULL;

        switch (device_type)
        {
        case MMI_DEVICE_UDISK:
            text_id = TXT_COMMON_UDISK;
            break;
        case MMI_DEVICE_SDCARD:
#ifndef DUAL_TCARD_SUPPORT
			text_id = TXT_COMMON_SD_CARD;
#else
			text_id = TXT_COMMON_SD_CARD0;
#endif
            break;
        case MMI_DEVICE_SDCARD_1:
            text_id = TXT_COMMON_SD_CARD1;
            break;
        default:
            break;
        }

        return text_id;
    }

    /*****************************************************************************/
//  Description : 根据设备类型返回没有该设备的字符串
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetAlertTextIDByDev (MMIFILE_DEVICE_E device_type)
    {
        MMI_TEXT_ID_T  text_id = TXT_NULL;

        switch (device_type)
        {
        case MMI_DEVICE_UDISK:
            text_id = TXT_UDISK_NO_EXIST;
            break;
        case MMI_DEVICE_SDCARD:
            text_id = TXT_NO_SD_CARD_ALERT;
            break;
        case MMI_DEVICE_SDCARD_1:
            text_id = TXT_NO_SD_CARD_ALERT;
            break;
        default:
            break;
        }

        return text_id;
    }

    /*****************************************************************************/
//  Description : 根据设备类型返回给设备容量的Text id
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceMemStatusText (MMIFILE_DEVICE_E device_type)
    {
        MMI_TEXT_ID_T  text_id = TXT_NULL;

        switch (device_type)
        {
        case MMI_DEVICE_UDISK:
            text_id = TXT_COMMON_UDISK_MEMORY;
            break;
        case MMI_DEVICE_SDCARD:
#ifndef DUAL_TCARD_SUPPORT
			text_id = TXT_COMMON_SD_MEMORY;
#else
			text_id = TXT_COMMON_SD0_MEMORY;
#endif
            break;
        case MMI_DEVICE_SDCARD_1:
            text_id = TXT_COMMON_SD1_MEMORY;
            break;
        default:
            break;
        }

        return text_id;
    }

    /*****************************************************************************/
//  Description : 根据设备类型返回该设备的复制字符串的Text id
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceCopyText (MMIFILE_DEVICE_E device_type)
    {
        MMI_TEXT_ID_T  text_id = TXT_NULL;
#ifdef MMI_AUDIO_PLAYER_SUPPORT

        switch (device_type)
        {
        case MMI_DEVICE_UDISK:
            text_id = TXT_MP3_LIST_COPY_TO_UDISK;
            break;
        case MMI_DEVICE_SDCARD:
#ifndef DUAL_TCARD_SUPPORT
			text_id = TXT_MP3_LIST_COPY_TO_SDCARD;
#else
			text_id = TXT_COPY_TO_SD_CARD0;
#endif
            break;
        case MMI_DEVICE_SDCARD_1:
            text_id = TXT_COPY_TO_SD_CARD1;
            break;
        default:
            break;
        }

#endif
        return text_id;
    }
    /*****************************************************************************/
//  Description : 返回剩余空间较大的device
//  Global resource dependence :
//  Author: charlie.wang
//  Note:
    /*****************************************************************************/
    PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetMaxFreeDeviceType (void)
    {
        uint32          free_space_high     = 0;
        uint32          free_space_low      = 0;
        uint32          cur_free_space_high = 0;
        uint32          cur_free_space_low  = 0;
        MMIFILE_ERROR_E file_ret            = SFS_ERROR_NONE;
        MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_SDCARD;
        MMIFILE_DEVICE_E  max_free_device     = MMI_DEVICE_UDISK;

        for (sd_type = MMI_DEVICE_UDISK; sd_type < MMI_DEVICE_NUM; sd_type++)
        {
            if (!MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
            {
                continue;
            }

            file_ret = SFS_GetDeviceFreeSpace ( (uint16 *) MMIAPIFMM_GetDevicePath (sd_type),
                                                &cur_free_space_high,
                                                &cur_free_space_low);

            if (file_ret == SFS_ERROR_NONE)
            {
                if (cur_free_space_high > free_space_high //高位大于肯定大于
                        || ( (cur_free_space_high == free_space_high) //高位相等，低位大于
                             && (cur_free_space_low > free_space_low))
                   )
                {
                    free_space_high = cur_free_space_high;
                    free_space_low  = cur_free_space_low;
                    max_free_device = sd_type;
                }
            }
        }

        return max_free_device;
    }

    /*****************************************************************************/
//  Description : 返回第一个有效的device
//  Global resource dependence :
//  Author:
//  Note:如果没有有效的返回FS_INVALID
    /*****************************************************************************/
    PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetFirstValidDevice (void)
    {
        MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_NUM - 1;

        BOOLEAN      dev_ok  = FALSE;

        do
        {
            //需要调用以下接口，因为有对CARDLOG等的判断
            if (MMIAPIFMM_GetDeviceTypeStatus (fmm_dev))
            {
                dev_ok = TRUE;
                break;
            }
        }
        while (--fmm_dev >= MMI_DEVICE_UDISK);

        if (!dev_ok)
        {
            fmm_dev = MMI_DEVICE_NUM;
        }

        return fmm_dev;
    }

    /*****************************************************************************/
//  Description : 返回第一个存在的device，这个device可能是无效的
//  Global resource dependence :
//  Author:
//  Note:如果没有存在的返回FS_INVALID
    /*****************************************************************************/
    PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetFirstExistDevice (void)
    {
        MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_NUM - 1;
        BOOLEAN      dev_ok  = FALSE;

        do
        {
            //需要调用以下接口，因为有对CARDLOG等的判断
            if (MMIAPIFMM_GetDeviceTypeStatus ( (fmm_dev)))
            {
                dev_ok = TRUE;
                break;
            }

#ifdef MSDC_CARD_SUPPORT
            else if (MMIAPISD_IsSDNeedFormat ( (fmm_dev)))
            {
                dev_ok = TRUE;
                break;
            }

#endif
        }
        while (--fmm_dev >= MMI_DEVICE_UDISK);

        if (!dev_ok)
        {
            fmm_dev = MMI_DEVICE_NUM;
        }

        return fmm_dev;
    }

    /*****************************************************************************/
//  Description : 返回第一个有效的sd card device type
//  Global resource dependence :
//  Author:
//  Note:如果没有有效的返回MMI_DEVICE_NUM
    /*****************************************************************************/
    PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetFirstValidSD (void)
    {
        MMIFILE_DEVICE_E device = MMI_DEVICE_SDCARD;
        //BOOLEAN      dev_ok  = FALSE;

        for (device = MMI_DEVICE_SDCARD; device < MMI_DEVICE_NUM; device++)
        {
            if (MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (device), MMIAPIFMM_GetDevicePathLen (device)))
            {
                break;
            }
        }

        return device;
    }

    /*****************************************************************************/
//  Description : 根据设备类型返回给设备剩余容量的Text id
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceFreeSpaceText (MMIFILE_DEVICE_E device_type)
    {
        MMI_TEXT_ID_T  text_id = TXT_NULL;

        switch (device_type)
        {
        case MMI_DEVICE_UDISK:
            text_id = TXT_UDISK_NO_USED_SPACE;
            break;
        case MMI_DEVICE_SDCARD:
            text_id = TXT_SD_NO_USED_SPACE;
            break;
        case MMI_DEVICE_SDCARD_1:
            text_id = TXT_COMMON_SD1_FREESPACE;
            break;
        default:
            break;
        }

        return text_id;
    }

    /*****************************************************************************/
//  Description : 根据设备类型返回该设备的图标
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC MMI_IMAGE_ID_T  MMIAPIFMM_GetOSDImageIDByDeviceType (MMIFILE_DEVICE_E device_type)
    {
        MMI_IMAGE_ID_T  img_id = IMAGE_NULL;

#ifdef CAMERA_SUPPORT
#ifndef MMI_RES_LOW_COST

        switch (device_type)
        {
        case MMI_DEVICE_UDISK:
            img_id = IMG_UDISK_TIP;
            break;
        case MMI_DEVICE_SDCARD:
            img_id = IMG_SD_TIP;
            break;
#ifdef DUAL_TCARD_SUPPORT
        case MMI_DEVICE_SDCARD_1:
            img_id = IMG_SD2_TIP;
            break;
#endif
        default:
            break;
        }

#endif
#endif

        return img_id;
    }

    /*****************************************************************************/
//  Description : 根据设备类型返回该设备的图标
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC MMI_IMAGE_ID_T  MMIAPIFMM_GetStorageIcon (MMIFILE_DEVICE_E device_type)
    {
        MMI_IMAGE_ID_T  img_id = IMAGE_NULL;

#ifdef CAMERA_SUPPORT
#ifdef MMIDC_DESKTOP_ICON_BAR

        switch (device_type)
        {
        case MMI_DEVICE_UDISK:
            img_id = IMG_ICON_UDISK;
            break;
        case MMI_DEVICE_SDCARD:
            img_id = IMG_ICON_SD;
            break;
#ifdef DUAL_TCARD_SUPPORT
        case MMI_DEVICE_SDCARD_1:
            img_id = IMG_ICON_SD2;
            break;
#endif
        default:
            img_id = IMG_ICON_NO_STORE;
            break;
        }

#endif
#endif

        return img_id;
    }



#ifdef DRM_SUPPORT
    /*****************************************************************************/
//  Description : is drm file by suffix name
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 通过后缀检查是不是drm文件(此接口待废弃，最好使用MMIAPIDRM_IsDRMFile接口)
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIFMM_IsDrmFileBySuffix (
        wchar *file_name_ptr, //in:
        uint16  file_name_len   //in:
    )
    {
        BOOLEAN     result = FALSE;
        wchar       suffix[MMIFMM_FILE_FULL_PATH_MAX_LEN+1] = {0};
        const wchar suffix_dm[] = {'.', 'd', 'm', 0};
        const wchar suffix_dcf[] = {'.', 'd', 'c', 'f', 0};
        uint16      suffix_len = MMIFMM_FILE_FULL_PATH_MAX_LEN;
        uint16      dm_len = 0;
        uint16      dcf_len = 0;

        //split file suffix
        MMIAPIFMM_SplitFullPathExt (file_name_ptr,
                                    file_name_len,
                                    PNULL, PNULL,
                                    PNULL, PNULL,
                                    suffix, &suffix_len);

        //get dm and dcf suffix len
        dm_len  = (uint16) MMIAPICOM_Wstrlen (suffix_dm);
        dcf_len = (uint16) MMIAPICOM_Wstrlen (suffix_dcf);

        if ( (0 == MMIAPICOM_CompareTwoWstrExt (suffix, suffix_len, suffix_dm, dm_len, FALSE)) ||
                (0 == MMIAPICOM_CompareTwoWstrExt (suffix, suffix_len, suffix_dcf, dcf_len, FALSE)))
        {
            result = TRUE;
        }

        return (result);
    }
#endif


    /*****************************************************************************/
//  Description : create file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_HANDLE MMIFILE_CreateFile (
        const wchar *file_name,
        uint32 access_mode,
        uint32 share_mode,  //Must be NULL,File System have not realize it
        uint32 file_attri       //Must be NULL,File System have not realize it
    )
    {
        MMIFILE_HANDLE handle = 0;

        //SCI_ASSERT(PNULL != file_name);
        if (PNULL == file_name)
        {
            //SCI_TRACE_LOW:"MMIFILE_CreateFile param error: PNULL == file_name"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5194_112_2_18_2_19_57_137, (uint8*) "");
            return SFS_INVALID_HANDLE;
        }

        if (//0 == full_path_len ||
            0 == MMIAPICOM_Wstrlen (file_name))
        {
            handle = SFS_INVALID_HANDLE;
        }
        else
        {
            handle = SFS_CreateFile (
                         file_name,
                         access_mode,
                         share_mode,
                         file_attri
                     );
        }

        return handle;
    }

    /*****************************************************************************/
//  Description : find nezt file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIFILE_FindNextFile (
        MMIFILE_HANDLE file_handle,
        MMIFILE_FIND_DATA_T *fd
    )
    {
        MMIFILE_ERROR_E error = SFS_NO_ERROR;

        //SCI_ASSERT(PNULL != file_handle);
        //SCI_ASSERT(PNULL != fd);
        if ( (PNULL == file_handle) || (PNULL == fd))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_FindNextFile param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5230_112_2_18_2_19_57_138, (uint8*) "");
            return SFS_ERROR_INVALID_PARAM;
        }

        error = SFS_FindNextFile (file_handle, fd);

        return error;
    }

    /*****************************************************************************/
//  Description : delete file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIFILE_DeleteFile (
        const wchar *name,
        MMIFILE_OVERLAPPED_T *overlapped_ptr
    )
    {
        MMIFILE_ERROR_E error = SFS_NO_ERROR;

        //SCI_ASSERT(PNULL != name);
        if (PNULL == name)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_DeleteFile param error:PNULL == name"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5253_112_2_18_2_19_57_139, (uint8*) "");
            return SFS_ERROR_INVALID_PARAM;
        }

        error = SFS_DeleteFile (
                    name,
                    overlapped_ptr
                );

        return error;
    }

    /*****************************************************************************/
//  Description : delete diretory
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIFILE_DeleteDirectory (
        const wchar *path
    )
    {
        MMIFILE_ERROR_E error = SFS_NO_ERROR;

        //SCI_ASSERT(PNULL != path);
        if (PNULL == path)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_DeleteDirectory param error:PNULL == path"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5278_112_2_18_2_19_57_140, (uint8*) "");
            return SFS_ERROR_INVALID_PARAM;
        }

        error = SFS_DeleteDirectory (path);

        return error;
    }

    /*****************************************************************************/
//  Description : find first file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_HANDLE MMIFILE_FindFirstFile (
        const wchar *find,
        uint16 find_len,
        MMIFILE_FIND_DATA_T *fd
    )
    {
        MMIFILE_HANDLE handle = 0;

        //SCI_ASSERT(PNULL != find);
        //SCI_ASSERT(PNULL != fd);
        if ( (PNULL == find) || (PNULL == fd))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_FindFirstFile param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5303_112_2_18_2_19_57_141, (uint8*) "");
            return SFS_INVALID_HANDLE;
        }

        if (MMIFILE_FULL_PATH_MAX_LEN >= find_len)
        {
            handle = SFS_FindFirstFile (find, fd);
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_FindFirstFile find_len = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5312_112_2_18_2_19_57_142, (uint8*) "d", find_len);
        }

        return handle;
    }

    /*****************************************************************************/
//  Description : find close
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIFILE_FindClose (
        MMIFILE_HANDLE file_handle
    )
    {
        MMIFILE_ERROR_E error = SFS_NO_ERROR;

        if (PNULL != file_handle)
        {
            error = SFS_FindClose (file_handle);
        }

        return error;
    }

    /*****************************************************************************/
//  Description : get file size
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    uint32 MMIFILE_GetFileSize (
        MMIFILE_HANDLE file_handle
    )
    {
        uint32 len = 0;

        //SCI_ASSERT(PNULL != file_handle);
        if (PNULL == file_handle)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_GetFileSize param error: PNULL == file_handle"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5348_112_2_18_2_19_57_143, (uint8*) "");
            return len;
        }

        if (SFS_ERROR_NONE != SFS_GetFileSize (file_handle, &len))
        {
            len = 0;
        }

        return len;
    }

    /*****************************************************************************/
//  Description : read file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIFILE_ReadFile (
        MMIFILE_HANDLE file_handle,
        void *buffer,
        uint32 bytes_to_read,
        uint32 *bytes_read,
        MMIFILE_OVERLAPPED_T *overlapped
    )
    {
        MMIFILE_ERROR_E error = SFS_NO_ERROR;

        //SCI_ASSERT(PNULL != file_handle);
        //SCI_ASSERT(PNULL != buffer);
        //SCI_ASSERT(PNULL != bytes_read);
        if ( (PNULL == file_handle) || (PNULL == buffer) || (PNULL == bytes_read))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_ReadFile param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5379_112_2_18_2_19_57_144, (uint8*) "");
            return SFS_ERROR_INVALID_PARAM;
        }

        error = SFS_ReadFile (
                    file_handle,
                    buffer,
                    bytes_to_read,
                    bytes_read,
                    overlapped
                );

        return error;
    }

    /*****************************************************************************/
//  Description : write file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIFILE_WriteFile (
        MMIFILE_HANDLE file_handle,
        const void *buffer,
        uint32 bytes_to_write,
        uint32 *bytes_written,
        MMIFILE_OVERLAPPED_T *overlapped
    )
    {
        MMIFILE_ERROR_E error = SFS_NO_ERROR;

        //SCI_ASSERT(PNULL != file_handle);
        //SCI_ASSERT(PNULL != buffer);
        //SCI_ASSERT(PNULL != bytes_written);

        if ( (PNULL == file_handle) || (PNULL == buffer) || (PNULL == bytes_written))
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_WriteFile param error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5414_112_2_18_2_19_57_145, (uint8*) "");
            return SFS_ERROR_INVALID_PARAM;
        }

        error = SFS_WriteFile (
                    file_handle,
                    buffer,
                    bytes_to_write,
                    bytes_written,
                    overlapped
                );

        return error;
    }

    /*****************************************************************************/
//  Description : close file
//  Global resource dependence :
//  Author: songbin.zeng
//  Note:
    /*****************************************************************************/
    MMIFILE_ERROR_E MMIFILE_CloseFile (
        MMIFILE_HANDLE file_handle
    )
    {
        MMIFILE_ERROR_E error = SFS_NO_ERROR;

        if (PNULL != file_handle)
        {
            error = SFS_CloseFile (file_handle);
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_CloseFile file_handle %d !"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5445_112_2_18_2_19_57_146, (uint8*) "d", file_handle);
        }

        return error;
    }

    /*****************************************************************************/
//  Description : check whether file is exist
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIFILE_IsFileExist (
        const wchar *full_path_ptr,     //in
        uint16       full_path_len       //in, 双字节为单位
    )
    {
        BOOLEAN         result = FALSE;
        SFS_HANDLE      sfs_handle = 0;
        SFS_FIND_DATA_T find_data = {0};

        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);
        if (PNULL == full_path_ptr)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_IsFileExist error:PNULL == full_path_ptr!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5467_112_2_18_2_19_57_147, (uint8*) "");
            return FALSE;
        }

        if (//0 == full_path_len ||
            0 == MMIAPICOM_Wstrlen (full_path_ptr))
        {
            result = FALSE;
        }
        else
        {
            sfs_handle = MMIAPIFMM_FindFirstFile (full_path_ptr, full_path_len, &find_data);

            //SCI_TRACE_LOW:"MMIAPIFMM_IsFileExist: find first sfs_handle = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5480_112_2_18_2_19_57_148, (uint8*) "d", sfs_handle);

            if (SFS_INVALID_HANDLE != sfs_handle)
            {
                if (! (SFS_ATTR_DIR&find_data.attr))
                {
                    result = TRUE;
                }
            }

            SFS_FindClose (sfs_handle);
        }

        //SCI_TRACE_LOW:"MMIAPIFMM_IsFileExist: result = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5493_112_2_18_2_19_57_149, (uint8*) "d", result);

        return result;
    }

    /*****************************************************************************/
//  Description : check whether folder is exist
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
    /*****************************************************************************/
    BOOLEAN MMIFILE_IsFolderExist (
        const wchar     *full_path_ptr,     //in
        uint16              full_path_len       //in, 双字节为单位
    )
    {
        BOOLEAN         result = FALSE;
        SFS_HANDLE      sfs_handle = 0;
        SFS_FIND_DATA_T find_data = {0};
        MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
        //SCI_ASSERT(NULL != full_path_ptr);
        //SCI_ASSERT(0 < full_path_len);

        if (PNULL == full_path_ptr)
        {
            //SCI_TRACE_LOW:"MMIAPIFMM_IsFolderExist error:PNULL == full_path_ptr!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5516_112_2_18_2_19_57_150, (uint8*) "");
            return FALSE;
        }

        if (//0 == full_path_len ||
            0 == MMIAPICOM_Wstrlen (full_path_ptr))
        {
            result = FALSE;
        }
        else
        {
            device = MMIAPIFMM_GetDeviceTypeByPath (full_path_ptr, full_path_len);

            if (device < MMI_DEVICE_NUM)
            {
                if (MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (device), MMIAPIFMM_GetDevicePathLen (device)))
                {
                    result = TRUE;
                }
            }
            else
            {
                //normal folder
                sfs_handle = MMIAPIFMM_FindFirstFile (full_path_ptr, full_path_len, &find_data);

                //SCI_TRACE_LOW:"MMIAPIFMM_IsFolderExist: find first sfs_handle = %d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5540_112_2_18_2_19_58_151, (uint8*) "d", sfs_handle);

                if (SFS_INVALID_HANDLE != sfs_handle)
                {
                    if (SFS_ATTR_DIR&find_data.attr)
                    {
                        result = TRUE;
                    }
                }

                SFS_FindClose (sfs_handle);
            }
        }

        //SCI_TRACE_LOW:"MMIAPIFMM_IsFolderExist: result = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMI_FILEMGR_5553_112_2_18_2_19_58_152, (uint8*) "d", result);

        return result;

    }

    /*****************************************************************************/
//  Description : get free space in percent
//  Global resource dependence :
//  Author:juan.wu
//  Note: change byte to kb for caculate
    /*****************************************************************************/
    PUBLIC int32 MMIAPIFMM_GetFreeSpaceInPercent (const wchar *device_ptr)
    {
        uint32 free_high     = 0;
        uint32 free_low      = 0;
        uint32 used_high     = 0;
        uint32 used_low      = 0;
        uint32 total_high    = 0;
        uint32 total_low     = 0;
        int32  free_percent  = 0;
        uint32 total_kb      = 0;
        uint32 free_kb       = 0;
        uint32 kb_size       = 1 << 10;
        uint32 mb_size       = 1 << 20;

        SFS_GetDeviceUsedSpace (device_ptr, &used_high, &used_low);
        SFS_GetDeviceFreeSpace (device_ptr, &free_high, &free_low);

        total_high = used_high + free_high;
        total_low  = used_low  + free_low;

        if (0 == total_high && 0 == total_low)
        {
            SCI_TRACE_LOW ("MMIAPIFMM_GetFreeSpaceInPercent error total = 0");
            return -1;
        }

        total_kb = total_high * 4 * mb_size + total_low / kb_size;
        free_kb = free_high * 4 * mb_size   + free_low / kb_size;

        free_percent = (free_kb * 100) / total_kb;
        return free_percent;
    }

#ifdef _SW_ZDT_PRODUCT_
#ifdef FILE_LOG_SUPPORT
#define MMI_WAP_FILE_TEST 1
#else
#define MMI_WAP_FILE_TEST 0
#endif

#if MMI_WAP_FILE_TEST
#ifdef SRAM_SIZE_64MBIT
#define FILE_TEST_LOG_LEN 10240 
#define FILE_TEST_AT_LEN   20480
#define FILE_TEST_SOCKET_LEN 10240
#define FILE_TEST_BUF_LEN 51200
#define FILE_TEST_OVER_LEN 1024
#else
#define FILE_TEST_LOG_LEN 20480 
#define FILE_TEST_AT_LEN   2048
#define FILE_TEST_SOCKET_LEN 2048
#define FILE_TEST_BUF_LEN 20480
#define FILE_TEST_OVER_LEN 1024
#endif
static BOOLEAN             s_is_need_time = FALSE;
static BOOLEAN             s_is_need_hex = FALSE;

static uint8*  temp_log_msg = PNULL;
static uint32  temp_log_len = 0;
static uint8*  temp_sock_msg = PNULL;
static uint32  temp_sock_len = 0;
static uint8*  temp_at_msg = PNULL;
static uint32  temp_at_len = 0;
static uint8*  temp_log_buf_msg = PNULL;
static uint32  temp_log_buf_len = 0;
#endif


	BOOLEAN  UserFile_Delete( char * path,
								 char * filename
								)
	{	
		wchar	full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
		uint16	full_path_len = 0;
		wchar * file_dev_ptr;
		uint16	 tmp_len = 0;
		uint16 i = 0;
	
		if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD);
		}
		else if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
		} 
		else
		{
			return FALSE;
		}	
		
		for(i = 0; i < tmp_len; i++)
		{
			full_path[full_path_len++] = file_dev_ptr[i];
		}
			
		full_path[full_path_len++] = ':';
		full_path[full_path_len++] = '\\';
		tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
		full_path_len += tmp_len;
		full_path[full_path_len++] = '\\';
		tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
		full_path_len += tmp_len;
		   
		if(MMIFILE_IsFileExist(full_path,full_path_len))
		{
			MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
			return TRUE;
		}
	
		return FALSE;
		
	}
	
BOOLEAN  UserFile_DeleteU( char * path,
	                      	 char * filename
							)
{	
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;

    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    } 
    else
    {
        return FALSE;
    }   
    
    for(i = 0; i < tmp_len; i++)
    {
        full_path[full_path_len++] = file_dev_ptr[i];
    }
        
    full_path[full_path_len++] = ':';
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
    full_path_len += tmp_len;
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
    full_path_len += tmp_len;
       
    if(MMIFILE_IsFileExist(full_path,full_path_len))
    {
        MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
        return TRUE;
    }

    return FALSE;
	
}

	FFS_ERROR_E  UserFile_Write(
									char * path,
							char * filename,
							uint8 * data_ptr,
							uint32	data_len
						  )
	{
		wchar	full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
		uint16	full_path_len = 0;
		wchar * file_dev_ptr;
		uint16	 tmp_len = 0;
		uint16 i = 0;
		uint32 transbyte = 0;
		MMIFILE_HANDLE file_handle = 0;
		MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
		BOOLEAN ret = FALSE;
		if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD);
		}
		else if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
		}
		else
		{
			return FALSE;
		}	
			
		 for(i = 0; i < tmp_len; i++)
		 {
			 full_path[full_path_len++] = file_dev_ptr[i];
		 }
			
		 full_path[full_path_len++] = ':';
		 full_path[full_path_len++] = '\\';
		 tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
		 full_path_len += tmp_len;
		 full_path[full_path_len++] = '\\';
		 tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
		 full_path_len += tmp_len;
			
		file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
		if(SFS_INVALID_HANDLE != file_handle)
		{
			file_ret = MMIFILE_WriteFile(
				file_handle, 
				data_ptr, 
				data_len, 
				&transbyte, 
				NULL
				);
			
			if(SFS_ERROR_NONE == file_ret)
			{
				ret = TRUE;
			}
			MMIFILE_CloseFile(file_handle);
		}
	
		return ret;
		
	}
	
FFS_ERROR_E  UserFile_WriteU(
                                char * path,
                        char * filename,
						uint8 * data_ptr,
                        uint32  data_len
					  )
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    BOOLEAN ret = FALSE;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;
        
	file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
	if(SFS_INVALID_HANDLE != file_handle)
	{
		file_ret = MMIFILE_WriteFile(
			file_handle, 
			data_ptr, 
			data_len, 
			&transbyte, 
			NULL
			);
        
		if(SFS_ERROR_NONE == file_ret)
		{
			ret = TRUE;
		}
		MMIFILE_CloseFile(file_handle);
	}

    return ret;
	
}
	FFS_ERROR_E  UserFile_WriteExt(
									   char * path,
								char * filename,
								uint8  is_new,
								uint8 * data_ptr,
								uint32	data_len
							 )
	{
		wchar	full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
		uint16	full_path_len = 0;
		wchar * file_dev_ptr;
		uint16	 tmp_len = 0;
		uint16 i = 0;
		uint32 transbyte = 0;
		MMIFILE_HANDLE file_handle = 0;
		MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
		BOOLEAN ret = FALSE;
	
		if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD);
		}
		else  if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
		}	 
		else
		{
			return FALSE;
		}
		
		for(i = 0; i < tmp_len; i++)
		{
			full_path[full_path_len++] = file_dev_ptr[i];
		}
	
		full_path[full_path_len++] = ':';
		full_path[full_path_len++] = '\\';
		tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
		full_path_len += tmp_len;
		full_path[full_path_len++] = '\\';
		tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
		full_path_len += tmp_len;
	
		if(is_new)
		{
			file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
		}
		else
		{
			if(MMIFILE_IsFileExist(full_path, full_path_len))
			{
				file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_APPEND, NULL, NULL);
			}
			else
			{
				file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
			}
		}
		
		if(SFS_INVALID_HANDLE != file_handle)
		{
			file_ret = MMIFILE_WriteFile(
				file_handle, 
				data_ptr, 
				data_len, 
				&transbyte, 
				NULL
				);
	
			if(SFS_ERROR_NONE == file_ret)
			{
				ret = TRUE;
			}
			MMIFILE_CloseFile(file_handle);
		}
	
		return ret;
		
	}
	
FFS_ERROR_E  UserFile_WriteExtU(
                                   char * path,
                            char * filename,
                            uint8  is_new,
							uint8 * data_ptr,
                            uint32  data_len
						 )
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    BOOLEAN ret = FALSE;

    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }    
    else
    {
        return FALSE;
    }
    
    for(i = 0; i < tmp_len; i++)
    {
        full_path[full_path_len++] = file_dev_ptr[i];
    }

    full_path[full_path_len++] = ':';
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
    full_path_len += tmp_len;
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
    full_path_len += tmp_len;

    if(is_new)
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    }
    else
    {
        if(MMIFILE_IsFileExist(full_path, full_path_len))
        {
            file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_APPEND, NULL, NULL);
        }
        else
        {
            file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
        }
    }
    
    if(SFS_INVALID_HANDLE != file_handle)
    {
        file_ret = MMIFILE_WriteFile(
        	file_handle, 
        	data_ptr, 
        	data_len, 
        	&transbyte, 
        	NULL
        	);

        if(SFS_ERROR_NONE == file_ret)
        {
        	ret = TRUE;
        }
        MMIFILE_CloseFile(file_handle);
    }

    return ret;
	
}
	FFS_ERROR_E  UserFile_ReadExt(
									   char * path,
							 char * filename,
							 void *buffer,
							 uint32 bytes_to_read,
							 uint32 *bytes_read,
							 MMIFILE_OVERLAPPED_T *overlapped
						   )
	{
		FFS_ERROR_E error = FFS_NO_ERROR;
		wchar	full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
		uint16	full_path_len = 0;
		wchar * file_dev_ptr;
		uint16	 tmp_len = 0;
		uint16 i = 0;
		uint32 transbyte = 0;
		MMIFILE_HANDLE file_handle = 0;
		MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
		
		if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD);
		}
		else  if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
		}
		else
		{
			return FALSE;
		}	
		  
		for(i = 0; i < tmp_len; i++)
		{
			full_path[full_path_len++] = file_dev_ptr[i];
		}
			
		full_path[full_path_len++] = ':';
		full_path[full_path_len++] = '\\';
		tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
		full_path_len += tmp_len;
		full_path[full_path_len++] = '\\';
		tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
		full_path_len += tmp_len;
			
		if(MMIFILE_IsFileExist(full_path, full_path_len))
		{
			file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_OPEN_EXISTING |SFS_MODE_READ, NULL, NULL);
		}
		else
		{
			return SFS_ERROR_NOT_EXIST;
		}
	
		if(SFS_INVALID_HANDLE != file_handle)
		{
			file_ret = MMIFILE_ReadFile(
								  file_handle, 
								  buffer, 
								  bytes_to_read, 
								  bytes_read, 
								  overlapped
								 );
			
			if(SFS_ERROR_NONE != file_ret)
			{
				error = FFS_ERROR_INVALID_PARAM;
			}
			MMIFILE_CloseFile(file_handle);
		}
	
		return error;
	}
	
	
	SFS_ERROR_E  UserFile_RemoveDirFile(
								   
										 char * path
										)
	{
		SFS_HANDLE	hFind	 = PNULL;
		SFS_FIND_DATA_T sfsfind 			= {0};
		SFS_ERROR_E 	result				= SFS_NO_ERROR;
		wchar	full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
		uint16	full_path_len = 0;
		wchar * file_dev_ptr;
		uint16	 tmp_len = 0;
		uint16 i = 0;
		BOOLEAN ret = FALSE;
	
		if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD);
		}
		else if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
		}
		else
		{
			return FALSE;
		}	
		  
		for(i = 0; i < tmp_len; i++)
		{
			full_path[full_path_len++] = file_dev_ptr[i];
		}
	
		full_path[full_path_len++] = ':';
		full_path[full_path_len++] = '\\';
		tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
		full_path_len += tmp_len;		 
		if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
		{
			full_path[full_path_len++] = 0x5c;
		}
	
		full_path[full_path_len] = 0x2a;
		full_path[full_path_len+1] = 0;
	
		hFind  = SFS_FindFirstFile(full_path,&sfsfind );
	
		if( hFind == PNULL)
		{
			return SFS_NO_ERROR;
		}
	
		do
		{
			uint32 uNameLen = 0;
			uNameLen = MMIAPICOM_Wstrlen( (wchar *)sfsfind.name );
			SCI_MEMCPY(&full_path[full_path_len], sfsfind.name, MIN(uNameLen*2+2, MMIFILE_FULL_PATH_MAX_LEN));
			full_path[full_path_len+uNameLen] = '\0';
	
			// 是目录
			if( sfsfind.attr & SFS_ATTR_DIR )
			{
			}
			else
			{
				//删除文件
				result = MMIFILE_DeleteFile(full_path, PNULL);
	
				if( SFS_NO_ERROR != result )
				{
					SFS_FindClose(hFind);
					return result;
				}
			}
	
			result = SFS_FindNextFile(hFind, &sfsfind);
	
		}while(SFS_NO_ERROR == result);
	
		SFS_FindClose(hFind);
	
		return SFS_NO_ERROR;
	}
	
#ifdef MCARE_V31_SUPPORT
	BOOLEAN  UserFile_DeleteQQ( char * path,
								 char * filename
								)
	{	
		wchar	full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
		uint16	full_path_len = 0;
		wchar * file_dev_ptr;
		uint16	 tmp_len = 0;
		uint16 i = 0;
	
		if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD);
		}
		else
		{
			return FALSE;
		}	
		
		for(i = 0; i < tmp_len; i++)
		{
			full_path[full_path_len++] = file_dev_ptr[i];
		}
			
		full_path[full_path_len++] = ':';
		full_path[full_path_len++] = '\\';
		tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
		full_path_len += tmp_len;
		full_path[full_path_len++] = '\\';
		tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
		full_path_len += tmp_len;
		   
		if(MMIFILE_IsFileExist(full_path,full_path_len))
		{
			MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
			return TRUE;
		}
	
		return FALSE;
		
	}
	FFS_ERROR_E  UserFile_WriteQQ(
									char * path,
							char * filename,
							uint8 * data_ptr,
							uint32	data_len
						  )
	{
		wchar	full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
		uint16	full_path_len = 0;
		wchar * file_dev_ptr;
		uint16	 tmp_len = 0;
		uint16	 path_len = 0;
		uint16 i = 0;
		uint32 transbyte = 0;
		MMIFILE_HANDLE file_handle = 0;
		MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
		BOOLEAN ret = FALSE;
		
		if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD)))
		{
			file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD);
			tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD);
		}
		else
		{
			return FALSE;
		}	
			
		 for(i = 0; i < tmp_len; i++)
		 {
			 full_path[full_path_len++] = file_dev_ptr[i];
		 }
			
		 full_path[full_path_len++] = ':';
		 full_path[full_path_len++] = '\\';
		 tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
		 full_path_len += tmp_len;
		 path_len = full_path_len;
		 full_path[full_path_len++] = '\\';
		 tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
		 full_path_len += tmp_len;
	
		 if(MMIFILE_IsFileExist(full_path,full_path_len))
		 {
			return FALSE;
		 }
	
		file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
		if(SFS_INVALID_HANDLE != file_handle)
		{
				 uint16 dwAttri = 0;
			file_ret = MMIFILE_WriteFile(
				file_handle, 
				data_ptr, 
				data_len, 
				&transbyte, 
				NULL
				);
			
			if(SFS_ERROR_NONE == file_ret)
			{
				ret = TRUE;
			}
			MMIFILE_CloseFile(file_handle);
            #if 0
				dwAttri |= SFS_ATTR_HIDDEN;
				SFS_SetAttr(full_path,	dwAttri);
				dwAttri = 0;
            #endif
				full_path[path_len] = 0;
				if (SFS_NO_ERROR == SFS_GetAttr(full_path, &dwAttri))
				{
						DWORD dwRes = 0;
						dwAttri &= (~SFS_ATTR_DIR);
						dwAttri |= SFS_ATTR_HIDDEN;
						dwRes = SFS_SetAttr(full_path,	dwAttri);
				}
		}
		
		  return ret;
		
	}
#endif

void TraceUser_Init(void)
{
#if MMI_WAP_FILE_TEST
    if(temp_log_msg == PNULL)
    {
        temp_log_msg = SCI_ALLOC_APP(FILE_TEST_LOG_LEN);
    }
    if(temp_sock_msg == PNULL)
    {
        temp_sock_msg = SCI_ALLOC_APP(FILE_TEST_SOCKET_LEN);
    }
    if(temp_at_msg == PNULL)
    {
        temp_at_msg = SCI_ALLOC_APP(FILE_TEST_AT_LEN);
    }
    if(temp_log_buf_msg == PNULL)
    {
        temp_log_buf_msg = SCI_ALLOC_APP(FILE_TEST_BUF_LEN);
    }
#endif
}

void Trace_Need_Time(BOOLEAN need_time)
{
    #if MMI_WAP_FILE_TEST
    s_is_need_time = need_time;
    #endif
}

void Trace_Need_Hex(BOOLEAN need_hex)
{
#if MMI_WAP_FILE_TEST
    s_is_need_hex = need_hex;
#endif
}

static uint8 Nibble2HexChar(uint8 bNibble)
{
    uint8  bRtn = '0';

    if(bNibble <= 9)
        bRtn = bNibble + '0';
    else if(bNibble <= 0x0F)
    {
        switch(bNibble)
        {
            case 10:
                bRtn = 'A';
                break;
            case 11:
                bRtn = 'B';
                break;
            case 12:
                bRtn = 'C';
                break;
            case 13:
                bRtn = 'D';
                break;
            case 14:
                bRtn = 'E';
                break;
            case 15:
                bRtn = 'F';
                break;
        }
    }
    return(bRtn);
}

uint32 Debug_Hex_Log(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len)
{
    uint8 low = 0;
    uint8 high = 0;
    uint32 i = 0;
    uint32 j = 0;

    if((len*2) > dest_len)
    {
        len = dest_len/2;
    }
    for(i = 0; i < len; i++)
    {
        high = buf[i]>>4;
        dest_buf[j++] = (char)Nibble2HexChar(high);
        low = buf[i] & 0x0F;
        dest_buf[j++] = (char)Nibble2HexChar(low);
    }
    dest_buf[j] = 0;
    return j;
}

void Trace_Log_Data(const char *  buf, uint32 buf_len)
{
    #if MMI_WAP_FILE_TEST
    if(temp_log_msg != PNULL)
    {
        if((temp_log_len + buf_len) >  (FILE_TEST_LOG_LEN-FILE_TEST_OVER_LEN))
        {
            MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_SAVE, 0,0);
        }
        
        if((temp_log_len + buf_len) >= FILE_TEST_LOG_LEN)
        {
            buf_len = FILE_TEST_LOG_LEN-temp_log_len;
        }
        
        SCI_MEMCPY(temp_log_msg+temp_log_len,buf,buf_len);
        temp_log_len += buf_len;
    }
    #endif
}

void Trace_Log_Print(const char *pszFmt, ...)
{
    #if MMI_WAP_FILE_TEST
    if(temp_log_msg != PNULL)
    {
        char buf[1024];
        uint32 tmp_len = 0;
        SCI_TIME_T          time;
        SCI_DATE_T      date;
        char		        timestr[40] = {0};
        uint16 time_len = 0;
        va_list argptr;
        va_start(argptr, pszFmt);
       
        if(1)
        {
            TM_GetSysDate(&date);
            TM_GetSysTime(&time);
            sprintf((char*)timestr,"%ld:", SCI_GetTickCount());
            time_len = strlen(timestr);
            Trace_Log_Data(timestr,time_len);
        }
        
          if (vsprintf(buf,pszFmt, argptr) >= 0) {
            SCI_TRACE_LOW(buf);
            tmp_len = strlen(buf);
            buf[tmp_len++] = '\r';
            buf[tmp_len++] = '\n';
            buf[tmp_len] = 0;
            Trace_Log_Data(buf,tmp_len);
          }
       va_end(argptr);
    }
#endif
}

void Trace_Log_QQ_Data(const char *  buf, uint32 buf_len)
{
    #if MMI_WAP_FILE_TEST
    if(temp_sock_msg != PNULL)
    {
        if((temp_sock_len + buf_len) >  (FILE_TEST_SOCKET_LEN-FILE_TEST_OVER_LEN))
        {
            MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_SOCKET_SAVE, 0,0);
        }
        
        if((temp_sock_len + buf_len) >= FILE_TEST_SOCKET_LEN)
        {
            buf_len = FILE_TEST_SOCKET_LEN-temp_sock_len;
        }
        
        SCI_MEMCPY(temp_sock_msg+temp_sock_len,buf,buf_len);
        temp_sock_len += buf_len;
    }
    #endif
}

void Trace_Log_QQ_Print(const char *pszFmt, ...)
{
    #if MMI_WAP_FILE_TEST
    if(temp_sock_msg != PNULL)
    {
       char buf[512];
       uint32 tmp_len = 0;
        SCI_TIME_T          time;
        SCI_DATE_T      date;
        char		        timestr[40] = {0};
        uint16 time_len = 0;
        static BOOLEAN is_over_save = FALSE;
        va_list argptr;
        va_start(argptr, pszFmt);
        
        if(0)
        {
            sprintf((char*)timestr,"%ld: ", SCI_GetTickCount());
            time_len = strlen(timestr);
            Trace_Log_QQ_Data(timestr,time_len);
        }

          if (vsprintf(buf,pszFmt, argptr) >= 0) {
            SCI_TRACE_LOW(buf);
            tmp_len = strlen(buf);
            buf[tmp_len++] = '\r';
            buf[tmp_len++] = '\n';
            Trace_Log_QQ_Data(buf,tmp_len);
          }
       va_end(argptr);
    }
    #endif
}

void  Trace_Log_Buf_Data(const char *  buf, uint32 buf_len)
{
    #if MMI_WAP_FILE_TEST
    if(temp_log_buf_msg != PNULL)
    {
        if((temp_log_buf_len + buf_len) >  (FILE_TEST_BUF_LEN-FILE_TEST_OVER_LEN))
        {
            MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_BUF_SAVE, 0,0);
        }
        
        if((temp_log_buf_len + buf_len) >= FILE_TEST_BUF_LEN)
        {
            buf_len = FILE_TEST_BUF_LEN-temp_log_buf_len;
        }
        
        SCI_MEMCPY(temp_log_buf_msg+temp_log_buf_len,buf,buf_len);
        temp_log_buf_len += buf_len;
    }
    #endif
}


void Trace_Log_Buf_Print(const char *pszFmt, ...)
{
    #if MMI_WAP_FILE_TEST
    if(temp_log_buf_msg != PNULL)
    {
       char buf[1024];
       uint32 tmp_len = 0;
        SCI_TIME_T          time;
        SCI_DATE_T      date;
        char		        timestr[100] = {0};
        uint16 time_len = 0;
        static BOOLEAN is_over_save = FALSE;
        va_list argptr;
        va_start(argptr, pszFmt);
        TM_GetSysDate(&date);
        TM_GetSysTime(&time);
        sprintf(timestr,"%02d%02d%02d-%02d%02d%02d: ",(date.year%2000),date.mon,date.mday,time.hour,time.min,time.sec);
        //sprintf((char*)timestr,"%ld: ", SCI_GetTickCount());
        time_len = strlen(timestr);
        Trace_Log_Buf_Data(timestr,time_len);

          if (vsprintf(buf,pszFmt, argptr) >= 0) {
            SCI_TRACE_LOW(buf);
            tmp_len = strlen(buf);
            buf[tmp_len++] = '\r';
            buf[tmp_len++] = '\n';
            Trace_Log_Buf_Data(buf,tmp_len);
          }
       va_end(argptr);
    }
#endif
}


void Trace_Log_AT_Data(const char *  buf, uint32 buf_len)
{
    #if MMI_WAP_FILE_TEST
    if(temp_at_msg != PNULL)
    {
        uint32 tmp_len = 0;

        if(s_is_need_hex)
        {
            tmp_len = (buf_len*2);
        }
        else
        {
            tmp_len = buf_len;
        }   
        if((temp_at_len + tmp_len) >  (FILE_TEST_AT_LEN-FILE_TEST_OVER_LEN))
        {
            MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_AT_SAVE, 0,0);
        }
        
        if((temp_at_len + tmp_len) >= FILE_TEST_AT_LEN)
        {
            tmp_len = FILE_TEST_AT_LEN-temp_at_len;
        }
        
        if(s_is_need_hex)
        {
            tmp_len = Debug_Hex_Log((uint8 *)buf,buf_len,temp_at_msg+temp_at_len,tmp_len);
        }
        else
        {
            SCI_MEMCPY(temp_at_msg+temp_at_len,buf,buf_len);
        }   
        temp_at_len += tmp_len;
    }
    #endif
}

void Trace_Log_AT_Print(const char *pszFmt, ...)
{
    #if MMI_WAP_FILE_TEST
    if(temp_at_msg != PNULL)
    {
        char buf[600] = {0};
        uint32 tmp_len = 0;
        SCI_TIME_T          time;
        SCI_DATE_T      date;
        char		        timestr[40] = {0};
        uint16 time_len = 0;
        static BOOLEAN is_over_save = FALSE;
        va_list argptr;
        va_start(argptr, pszFmt);
        
        {
            sprintf((char*)timestr,"%ld: ", SCI_GetTickCount());
            time_len = strlen(timestr);
            Trace_Log_AT_Data(timestr,time_len);
        }
        
        if (vsprintf(buf,pszFmt, argptr) >= 0) 
        {
            tmp_len = strlen(buf);
            buf[tmp_len++] = '\r';
            buf[tmp_len++] = '\n';
            buf[tmp_len] = 0;
            //SCI_TRACE_LOW("%s",buf);
            Trace_Log_AT_Data(buf,tmp_len);
        }
        va_end(argptr);
    }
    #endif
}


void  Trace_BUF_Save(void)
{
    #if MMI_WAP_FILE_TEST
    if(temp_log_buf_msg != PNULL)
    {
        char tmp_char[200] = {0};
        SCI_TIME_T          time;
        SCI_DATE_T      date;
        if(temp_log_buf_len != 0)
        {
            TM_GetSysDate(&date);
            TM_GetSysTime(&time);
            sprintf((char*)tmp_char,"GSM_BUF_%02d%02d_%02d%02d%02d.log", date.mon,date.mday,time.hour,time.min,time.sec);
            UserFile_Delete("TEST",tmp_char);
            UserFile_Write("TEST",tmp_char,temp_log_buf_msg,temp_log_buf_len);
            temp_log_buf_len = 0;
        }
    }
    #endif
}

void Trace_Log_Save(void)
{
    #if MMI_WAP_FILE_TEST
    SCI_TRACE_LOW("Trace_Log_Save Ptr=0x%x,Len=%ld ",temp_log_msg,temp_log_len);
    if(temp_log_msg != PNULL)
    {
        char tmp_char[200] = {0};
        SCI_TIME_T          time;
        SCI_DATE_T      date;
        if(temp_log_len != 0)
        {
            TM_GetSysDate(&date);
            TM_GetSysTime(&time);
            sprintf((char*)tmp_char,"GSM_LOG_%02d%02d_%02d%02d%02d.log", date.mon,date.mday,time.hour,time.min,time.sec);
            UserFile_Delete("TEST",tmp_char);
            UserFile_Write("TEST",tmp_char,temp_log_msg,temp_log_len);
            temp_log_len = 0;
        }
    }
    #endif
}


void Trace_AT_Save(void)
{
    #if MMI_WAP_FILE_TEST
    if(temp_at_msg != PNULL)
    {
        char tmp_char[200] = {0};
        SCI_TIME_T          time;
        SCI_DATE_T      date;
        if(temp_at_len != 0)
        {
            TM_GetSysDate(&date);
            TM_GetSysTime(&time);
            sprintf((char*)tmp_char,"GSM_AT_%02d%02d_%02d%02d%02d.log", date.mon,date.mday,time.hour,time.min,time.sec);
            UserFile_Delete("TEST",tmp_char);
            UserFile_Write("TEST",tmp_char,temp_at_msg,temp_at_len);
            temp_at_len = 0;
        }
    }
    #endif
}

void Trace_Socket_Save(void)
{
    #if MMI_WAP_FILE_TEST
    if(temp_sock_msg != PNULL)
    {
        char tmp_char[200] = {0};
        SCI_TIME_T          time;
        SCI_DATE_T      date;
        if(temp_sock_len != 0)
        {
            TM_GetSysDate(&date);
            TM_GetSysTime(&time);
            sprintf((char*)tmp_char,"GSM_QQ_%02d%02d_%02d%02d%02d.log", date.mon,date.mday,time.hour,time.min,time.sec);
            UserFile_Delete("TEST",tmp_char);
            UserFile_Write("TEST",tmp_char,temp_sock_msg,temp_sock_len);
            temp_sock_len = 0;
        }
    }
    #endif
}
#endif

    /**--------------------------------------------------------------------------*
     **                         Compiler Flag                                    *
     **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/*lint -restore*/

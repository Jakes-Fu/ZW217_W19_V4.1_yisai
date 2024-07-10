/****************************************************************************************
** File Name:      mmibrowser_bookmark.c                                                *
** Author:         juan.zhang                                                           *
** Date:           2009.2.11                                                            *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This file is for bookmark operation                                  *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.2        juan.zhang         Create                                              *
**                                                                                      *
*****************************************************************************************/


/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_browser_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
//#include "sfs.h"
#include "mn_type.h"
#include "mmibrowser_bookmark.h"
#include "mmibrowser_wintable.h"
#include "mmibrowser_nv.h"
#ifndef MMIDORADO_BOOKMARK_BY_NV
#include "mmifmm_export.h"
#endif
#include "mmi_resource.h"
#include "mmibrowser_setting.h"
#include "mmibrowser_func.h"
#include "browser_text.h"
#ifdef MMI_SSL_CERT_SUPPORT 
#include "mmifilearray_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define	MMIBRW_ALLOC		SCI_ALLOCA
#define	MMIBRW_FREE			SCI_FREE

#ifndef MMIDORADO_BOOKMARK_BY_NV
//#define MMIBRW_BROWSER_DIR  (s_browser_dir_str) //"Browser"
//#define MMIBRW_BROWSER_DIR_LEN  7 //in the unit of uint8
#define MMIBRW_BROWSER_ROOT_PATH  (s_browser_bk_root_path) //"Browser"
#define MMIBRW_BROWSER_ROOT_PATH_LEN  16 
#define MMIBRW_BOOKMARK_FILE_MAX_LEN (3*1024)
#endif

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifndef MMIDORADO_BOOKMARK_BY_NV
LOCAL const wchar s_dorado_bk_file_suffix[]   = {'.' , 'u', 'r', 'l', 0 };
//const wchar s_browser_dir_str[]   = {'b' , 'r', 'o', 'w', 's', 'e', 'r', 0 };

LOCAL MMIBRW_BOOKMARK_T *s_bookmark_info_ptr = PNULL;
LOCAL MMIFMM_FILTER_T s_bookmark_filter = {{"*.url"}};
LOCAL const wchar s_bookmark_url_flag[] = { '[', 'I', 'n', 't', 'e','r','n','e', 't', 'S', 'h', 'o', 'r', 't', 'c', 'u', 't',']', 0x0D, 0x0A, 'U', 'R','L', '=', 0};//"[InternetShortcut]\x0d\x0aURL=";
LOCAL wchar s_browser_path[MMIFMM_FILE_FULL_PATH_MAX_LEN + 1] = {0};
LOCAL uint16 s_browser_path_len = MMIFMM_FILE_FULL_PATH_MAX_LEN;
LOCAL MMIFILE_DEVICE_E s_browser_default_dev = MMI_DEVICE_NUM;
const wchar s_browser_bk_root_path[]   = {'b' , 'r', 'o', 'w', 's', 'e', 'r', '\\', 'b','o','o','k','m','a','r','k',0 };

LOCAL MMIFMM_FUNC_TYPE_E   s_load_fmm_func_type = FUNC_FIND_NULL;

#else/*MMIDORADO_BOOKMARK_BY_NV*/
LOCAL MMIBRW_BOOKMARK_T *s_bookmark_info_ptr = PNULL;

#endif/*MMIDORADO_BOOKMARK_BY_NV*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
#if 0
/*****************************************************************************/
//  Description : BookMark Initialize
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void BrwInitializeBookmarkFileArray(void);
#endif

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwRenameFile(
                            const wchar     *full_path_ptr,     //in
                            uint16          full_path_len,      //in, 双字节为单位
                            wchar           *new_file_name_ptr, //in
                            uint16          new_file_name_len,   //in, 双字节为单位
                            wchar       *new_full_path_ptr,     //out
                            uint16          *new_full_path_len_ptr      //out, 双字节为单位
                            );

/*****************************************************************************/
// Description : Gets Url (UTF8)
// Global resource dependence :
// Author: fen.xie
// Note: need free "out_url_ptr" outside
/*****************************************************************************/
LOCAL uint32 BrwGetBookmarkUrl(uint16 in_index, //IN
                            uint8 **out_url_ptr//IN/OUT:need free outside
                            );

/*****************************************************************************/
//  Description : MMIBROWSER_BookmarkGetRootPath
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_BookmarkGetRootPath(
                    wchar           *full_path_ptr,     //out
                    uint16           *full_path_len_ptr,  //in/out, 双字节为单位
                    MMIFILE_DEVICE_E *dev_ptr
                    );
                    
/*****************************************************************************/
// Description : find the array index of file array  by filename
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL int32 BrwFindArrayIndexByFilename(
                    FILEARRAY   file_array,
                    wchar *full_file_name_ptr,
                    uint16 full_file_name_len,
                    VIRTUALARRAY_TYPE_E type
                    );

#else
LOCAL void BookmarkReadInfo(BOOLEAN is_force);
LOCAL void BookmarkOverwriteInfo(void);
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkCombinePath(
                                     wchar           *path_ptr,      //in/out:may PNULL
                                     uint16          path_len,  //in/out:may PNULL,path_ptr不为空时,此处必须填max len
                                     wchar           *name_ptr,      //in/out:may PNULL,don't include suffix
                                     uint16          name_len,  //in/out:may PNULL,name_ptr不为空时,此处必须填max len
                                     const wchar    *suffix_ptr,    //in/out:may PNULL,include dot
                                     uint16          suffix_len, //in/out:may PNULL,suffix_ptr不为空时,此处必须填max len
                                     wchar     *full_path_ptr, //out
                                     uint16          *full_path_len_ptr  //out
                                     )
{
    wchar      *wchar_ptr = PNULL;
    uint16  temp_len = 0;
    uint16  full_len = 0;

    if (PNULL == path_ptr || PNULL == name_ptr || 0 == path_len || 0 == name_len || PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        return FALSE;
    }

    full_len = path_len + 1 + name_len;
    if ((suffix_ptr != PNULL) && (suffix_len > 0))
    {
        full_len += (1 + suffix_len);
    }
    if (full_len > *full_path_len_ptr)//error
    {
        return FALSE;
    }
    
    MMI_WSTRNCPY(full_path_ptr, *full_path_len_ptr , path_ptr, path_len, path_len);
    temp_len += path_len;
    wchar_ptr = (uint16 *)(full_path_ptr + temp_len);

    *wchar_ptr = MMIFILE_SLASH;
    temp_len += 1;
    
    MMI_WSTRNCPY(full_path_ptr + temp_len, *full_path_len_ptr , name_ptr, name_len, name_len);
    temp_len += name_len;

    if ((suffix_ptr != PNULL) && (suffix_len > 0))
    {
        wchar_ptr = (uint16 *)(full_path_ptr + temp_len);
        *wchar_ptr = MMIFILE_DOT;
        temp_len += 1;
        
        MMI_WSTRNCPY(full_path_ptr + temp_len, *full_path_len_ptr , suffix_ptr, suffix_len, suffix_len);
        temp_len += suffix_len;
    }

    *full_path_len_ptr = temp_len;
        
    return TRUE;
}


#ifdef MMI_BROWSER_MINI_SUPPORT
/****************************************************************************/
//Description : free mem, add for low memory project
//Global resource dependence : 
//Author: bown.zhang
//Note:	
/****************************************************************************/
PUBLIC void MMIBRW_FreeBookmark(void)
{
    if (s_bookmark_info_ptr != PNULL)
    {
        SCI_FREE(s_bookmark_info_ptr);
        s_bookmark_info_ptr = PNULL;
    }
}
#endif

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwRenameFile(
                            const wchar     *full_path_ptr,     //in
                            uint16          full_path_len,      //in, 双字节为单位
                            wchar           *new_file_name_ptr, //in
                            uint16          new_file_name_len,   //in, 双字节为单位
                            wchar       *new_full_path_ptr,     //out
                            uint16          *new_full_path_len_ptr      //out, 双字节为单位
                            )
{
    BOOLEAN         result = FALSE;
    wchar           device_name[MMIFILE_DEVICE_NAME_MAX_LEN+2] = {0};
    uint16          device_name_len = 0;
    wchar           dir_name[MMIFILE_DIR_NAME_MAX_LEN+2] = {0};
    uint16          dir_name_len = 0;

    if((PNULL == full_path_ptr )
        || (0 == full_path_len || MMIFILE_FULL_PATH_MAX_LEN < full_path_len)
        || (PNULL == new_file_name_ptr)
        || (0 == new_file_name_len || MMIFILE_FILE_NAME_MAX_LEN < new_file_name_len)
        || (PNULL == new_full_path_ptr )
        || (PNULL == new_full_path_len_ptr )
        || (0 == *new_full_path_len_ptr || MMIFILE_FULL_PATH_MAX_LEN < *new_full_path_len_ptr)
        )
    {
        //SCI_TRACE_LOW:"MMIBrw RenameFile full_path_len %d new_file_name_len %d !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_200_112_2_18_2_5_38_0,(uint8*)"dd", full_path_len, new_file_name_len);
        return FALSE;
    }
        
    if(MMIAPIFMM_SplitFullPath(full_path_ptr, full_path_len,
                             device_name, &device_name_len,
                             dir_name, &dir_name_len,
                             NULL, NULL))
    {
        if(MMIAPIFMM_CombineFullPath(device_name, device_name_len,
                                   dir_name, dir_name_len,
                                   new_file_name_ptr, new_file_name_len,
                                   new_full_path_ptr, new_full_path_len_ptr))
        {
            if(SFS_ERROR_NONE == SFS_RenameFile(full_path_ptr, new_full_path_ptr, NULL))
            {
                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
// Description : Gets Url (UTF8)
// Global resource dependence :
// Author: fen.xie
// Note: need free "out_url_ptr" outside
/*****************************************************************************/
LOCAL uint32 BrwGetBookmarkUrl(uint16 in_index, //IN
                            uint8 **out_url_ptr//IN/OUT:need free outside
                            )
{
    FILEARRAY_DATA_T array_item = {0};
    uint8 * buf_ptr = PNULL;
    uint32 url_len = 0;
    uint8 * url_buf_ptr = PNULL;
    uint8 * temp_buf_ptr = PNULL;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();

    if (PNULL == out_url_ptr)
    {
        return 0;
    }

    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
    {
        if (array_item.size > MMIBRW_BOOKMARK_FILE_MAX_LEN)/*size需要限制大小,避免出错时申请过大内存*/
        {
            SCI_TRACE_LOW("[BRW]W BrwGetBookmarkUrl:array_item.size =%d", array_item.size);
            return 0;
        }
    
        buf_ptr = SCI_ALLOCAZ(array_item.size);
        if (PNULL == buf_ptr)
        {
            SCI_TRACE_LOW("[BRW]W BrwGetBookmarkUrl:buf_ptr ALLOC FAIL");
            return 0;
        }
        
        if(MMIAPIFMM_ReadFilesDataSyn((const wchar *)array_item.filename, array_item.name_len, buf_ptr, array_item.size))
        {
            temp_buf_ptr = MMIAPIBROWSER_GetUrlByFile(buf_ptr, array_item.size, &url_len);
            if ((0 != url_len) && (PNULL != temp_buf_ptr))
            {
                url_buf_ptr = SCI_ALLOCAZ(url_len + 1);
                if (PNULL == url_buf_ptr)
                {
                    SCI_FREE(buf_ptr);
                    return 0;
                }
                //SCI_MEMSET(url_buf_ptr, 0x00, (url_len + 1));
                SCI_MEMCPY(url_buf_ptr, temp_buf_ptr, url_len);
            }
        }
    }
    
    if (PNULL != buf_ptr)
    {
        SCI_FREE(buf_ptr);
    }

    *out_url_ptr = url_buf_ptr;
    return url_len;
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : MMIBRW_Bookmark Split Path
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkSplitPath(
                            const wchar     *full_path_ptr,     //in
                            uint16          full_path_len,      //in, 双字节为单位
                            wchar           *pre_path_ptr, //IN/OUT
                            uint16          *pre_path_len_ptr,   //IN/OUT, 双字节为单位
                            wchar            *file_name_ptr,     //IN/OUT
                            uint16           *file_name_len_ptr,  //IN/OUT, 双字节为单位
                            BOOLEAN     is_with_suffix  //IN
                            )
{
    BOOLEAN         result = FALSE;
    wchar           device_name[MMIFILE_DEVICE_NAME_MAX_LEN+2] = {0};
    uint16          device_name_len = 0;
    wchar           dir_name[MMIFILE_DIR_NAME_MAX_LEN+2] = {0};
    uint16          dir_name_len = 0;

    if((PNULL == full_path_ptr )
        || (0 == full_path_len || MMIFILE_FULL_PATH_MAX_LEN < full_path_len)
        //|| (PNULL == pre_path_ptr)
        //|| (PNULL == pre_path_len_ptr)
        //|| (0 == *pre_path_len_ptr || MMIFILE_FULL_PATH_MAX_LEN < *pre_path_len_ptr)
        //|| (PNULL == file_name_ptr )
        //|| (PNULL == file_name_len_ptr )
        //|| (0 == *file_name_len_ptr || MMIFILE_FULL_PATH_MAX_LEN < *file_name_len_ptr)
        )
    {
        return FALSE;
    }

    if (is_with_suffix)
    {
        if(MMIAPIFMM_SplitFullPath(full_path_ptr, full_path_len,
                             device_name, &device_name_len,
                             dir_name, &dir_name_len,
                             file_name_ptr, file_name_len_ptr))
         {
            result = TRUE;
         }
    }
    else
    {
        MMIAPIFMM_SplitFullPathExt(full_path_ptr, full_path_len,
                             pre_path_ptr, pre_path_len_ptr,
                             file_name_ptr, file_name_len_ptr, 
                             PNULL, PNULL);
    }

   if (result)
    {
        if(MMIAPIFMM_CombineFullPath(device_name, device_name_len,
                                   dir_name, dir_name_len,
                                   PNULL, PNULL,
                                   pre_path_ptr, pre_path_len_ptr))
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
   }

    return result;
}
#endif /* Reduce Code size */

#if 0
/*****************************************************************************/
//  Description : Check whether the path is absolute or not
//  Global resource dependence : 
//  Author:wancan.you
//  Note: TRUE like "C:" "D:"
/*****************************************************************************/
LOCAL BOOLEAN  BrwIsAbsolutePath(wchar *file_ptr)
{
    BOOLEAN ret = FALSE;

    if (MMIFILE_COLON == *(file_ptr + 1))
    {
        //Todo:more check
        ret = TRUE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN BrwCombineFullPath(
                                         char  *in_fname,
                                         wchar      *full_path_ptr,     //out
                                         uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                         )
{
    uint16 length = 0, w_length = 0;
    wchar *wstr_ptr = PNULL;
    wchar* device_name_ptr = PNULL;
    uint16 device_name_len = 0;
    BOOLEAN ret = TRUE;

    MMIBROWSER_GetBrwSysDevName(&device_name_ptr, &device_name_len);

    if (PNULL == device_name_ptr || 0 == device_name_len)
    {
        return FALSE;
    }

    length = (uint16)strlen((char *)in_fname);
    wstr_ptr = SCI_ALLOCA((length + 1) * sizeof(wchar));

    if (PNULL == wstr_ptr)
    {
        return FALSE;
    }

    SCI_MEMSET(wstr_ptr, 0x00, ((length + 1) * sizeof(wchar)));
    
    w_length = GUI_UTF8ToWstr(wstr_ptr, length, (const uint8*)in_fname, length);
    if (BrwIsAbsolutePath(wstr_ptr))
    {
        length = MMIAPICOM_Wstrlen(wstr_ptr);
        MMIAPICOM_Wstrncpy(full_path_ptr, wstr_ptr, MIN(length, MMIFILE_FULL_PATH_MAX_LEN));
    }
    else
    {
        if (!MMIAPIFMM_CombineFullPath(device_name_ptr, device_name_len,
                                                    (wchar*)MMIBRW_BROWSER_DIR, MMIBRW_BROWSER_DIR_LEN,
                                                    wstr_ptr, w_length,
                                                    full_path_ptr, full_path_len_ptr))
        {
            ret = FALSE;
        }
    }

    SCI_FREE(wstr_ptr);
    
    return ret;
}
#endif

/*****************************************************************************/
//  Description : get bookmark default root path
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN BrwGetDefaultRootPath(
                wchar      *full_path_ptr,     //out
                uint16     *full_path_len_ptr  //in/out, 双字节为单位
                )
{
    BOOLEAN ret = TRUE;

    if ((PNULL == full_path_ptr) || (PNULL == full_path_len_ptr) || (0 == *full_path_len_ptr))
    {
        return FALSE;
    }

    MMI_WSTRNCPY(full_path_ptr, *full_path_len_ptr, 
        s_browser_path, s_browser_path_len,
        MIN(s_browser_path_len, *full_path_len_ptr));
        
    *full_path_len_ptr = MMIAPICOM_Wstrlen(full_path_ptr);
    return ret;
}

/*****************************************************************************/
// Description : update the total bookmark number
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIBRW_UpdateBookmarkTotalNum(void)
{
    uint32 file_num = 0;
    wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    FILEARRAY   file_array = {0};
    FILEARRAY   temp_file_array = {0};
    FILEARRAY_DATA_T temp_file_data = {0};
    uint32 root_folder_num = 0;
    uint32 root_file_num = 0;
    uint32 total_file_num = 0;
    int32 index = 0;

    //BrwCombineFullPath((char*)MMIBRW_BOOKMARK_FILE_DIR, full_path_name, &full_path_name_len);
    BrwGetDefaultRootPath(full_path_name, &full_path_name_len);
    
    //根目录下的书签文件
    file_array = MMIAPIFILEARRAY_Create();    
    MMIAPIFMM_SearchFileInPath(full_path_name, full_path_name_len,
            &s_bookmark_filter,
            FALSE,
            FUNC_FIND_FILE,
            file_array,
            0,
            0);
    
    root_file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
    MMIAPIFILEARRAY_Destroy(&file_array);
    total_file_num += root_file_num;

    //开始遍历文件夹中的书签文件
    if (total_file_num < MMIBRW_BOOKMARK_MAX_ITEM)
    {
        file_array = MMIAPIFILEARRAY_Create();
        MMIAPIFMM_SearchFileInPath(full_path_name, full_path_name_len,
                &s_bookmark_filter,
                FALSE,
                FUNC_FIND_FOLDER,
                file_array,
                0,
                0);
        root_folder_num = (uint32)MMIAPIFILEARRAY_GetArraySize(file_array);
        root_folder_num = MIN(MMIBRW_BOOKMARK_DIR_MAX_NUM, root_folder_num);

#ifdef MMI_BROWSER_MINI_SUPPORT
        if(PNULL == s_bookmark_info_ptr)
        {
            MMIBRW_BookmarkInit();
        }
#endif
        s_bookmark_info_ptr->cur_folder_num = root_folder_num;

        for(index = 0; (uint32)index < root_folder_num; index++)
        {
            SCI_MEMSET(&temp_file_data, 0x00, sizeof(temp_file_data));
            MMIAPIFILEARRAY_Read(file_array, index, &temp_file_data);
            
            temp_file_array = MMIAPIFILEARRAY_Create();
            MMIAPIFMM_SearchFileInPath(temp_file_data.filename, temp_file_data.name_len,
                    &s_bookmark_filter,
                    FALSE,
                    FUNC_FIND_FILE,
                    temp_file_array,
                    0,
                    0);
            
            file_num = MMIAPIFILEARRAY_GetArraySize(temp_file_array);
            MMIAPIFILEARRAY_Destroy(&temp_file_array);
            total_file_num += file_num;
            
            if (total_file_num >= MMIBRW_BOOKMARK_MAX_ITEM)
            {
                total_file_num = MMIBRW_BOOKMARK_MAX_ITEM;
                break;
            }

        }
        MMIAPIFILEARRAY_Destroy(&file_array);
    
    }
    else
    {
        total_file_num = MMIBRW_BOOKMARK_MAX_ITEM;
    }
    //SCI_TRACE_LOW:"MMIBRW_UpdateBookmarkTotalNum bookmark_total_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_708_112_2_18_2_5_39_1,(uint8*)"d", total_file_num);

    return total_file_num;
}

/*****************************************************************************/
//  Description : BookMark Initialize for start phone
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_BookmarkInit(void)
{
    wchar path_arr[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 path_len = 0;
    
    SCI_MEMSET(s_browser_path, 0x00, sizeof(s_browser_path));    
    s_browser_path_len = MMIFMM_FILE_FULL_PATH_MAX_LEN;
    s_browser_default_dev = MMI_DEVICE_NUM;
    MMIBROWSER_BookmarkGetRootPath(s_browser_path, &s_browser_path_len, &s_browser_default_dev);//init
    
    if (PNULL == s_bookmark_info_ptr)
    {
        s_bookmark_info_ptr = MMIBRW_ALLOC(sizeof(MMIBRW_BOOKMARK_T));
        
        if (PNULL != s_bookmark_info_ptr)
        {
            SCI_MEMSET(s_bookmark_info_ptr, 0x00, sizeof(MMIBRW_BOOKMARK_T));
            
            if ((MMI_DEVICE_NUM > s_browser_default_dev) && (0 < s_browser_path_len))//normal
            {
                path_len = MMIFILE_FULL_PATH_MAX_LEN;
                //BrwCombineFullPath((char *)MMIBRW_BOOKMARK_FILE_DIR, path_arr, &path_len);
                BrwGetDefaultRootPath(path_arr, &path_len);

                s_bookmark_info_ptr->total = MMIBRW_UpdateBookmarkTotalNum();
                s_bookmark_info_ptr->file_array_step = 0;
                s_bookmark_info_ptr->full_path_len = MIN(path_len, MMIFILE_FULL_PATH_MAX_LEN);
                MMI_WSTRNCPY((wchar *)s_bookmark_info_ptr->full_path, 
                                            MMIFILE_FULL_PATH_MAX_LEN,  
                                            path_arr, 
                                            s_bookmark_info_ptr->full_path_len, 
                                            s_bookmark_info_ptr->full_path_len);
                
                //SCI_TRACE_LOW:"[BRW]MMIBRW_BookmarkInit bookmark_num=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_750_112_2_18_2_5_39_2,(uint8*)"d", s_bookmark_info_ptr->total);
            }
            else
            {
                //SCI_TRACE_LOW:"[BRW]W MMIBRW_BookmarkInit s_browser_default_dev=%d, s_browser_path_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_755_112_2_18_2_5_39_3,(uint8*)"dd", s_browser_default_dev, s_browser_path_len);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[BRW]W MMIBRW_BookmarkInit ALLOC FAIL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_760_112_2_18_2_5_39_4,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[BRW]MMIBRW_BookmarkInit Inited!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_765_112_2_18_2_5_39_5,(uint8*)"");
    }   
    
    if ((MMI_DEVICE_NUM <= s_browser_default_dev) ||(0 == s_browser_path_len))
    {
        MMIBROWSER_SetBookmarkLoaded(FALSE);//set bookmark is not loaded,if no stotage
    }
    else if ((PNULL != s_bookmark_info_ptr) && (!MMIBROWSER_GetIsBookmarkLoaded()))
    {
        if (MMIAPIFMM_IsFolderExist(s_browser_path, s_browser_path_len))//the folder is exit, don't set default bookmarks again
        {
            //SCI_TRACE_LOW:"[BRW]MMIBRW_BookmarkInit Folder is Exist!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_776_112_2_18_2_5_39_6,(uint8*)"");
            MMIBROWSER_SetBookmarkLoaded(TRUE);
        }
        else
        {
            MMIBRW_SetDefaultBookmark();/*if bookmark is not loaded,set default*/
        }
    }

}

/*****************************************************************************/
//  Description : BookMark Initialize after the phone is start up(for protect)
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_BookmarkInitAfterStartup(void)
{
    if (!MMIBROWSER_GetIsBookmarkLoaded())//without storage sometime(plug out.....)
    {    
        wchar path_arr[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
        uint16 path_len = 0;

        SCI_MEMSET(s_browser_path, 0x00, sizeof(s_browser_path));    
        s_browser_path_len = MMIFMM_FILE_FULL_PATH_MAX_LEN;
        s_browser_default_dev = MMI_DEVICE_NUM;
        MMIBROWSER_BookmarkGetRootPath(s_browser_path, &s_browser_path_len, &s_browser_default_dev);//init

#ifdef MMI_BROWSER_MINI_SUPPORT
        if(PNULL == s_bookmark_info_ptr)
        {
            MMIBRW_BookmarkInit();
        }
#endif
        if (PNULL != s_bookmark_info_ptr)//update the date also
        {
            SCI_MEMSET(s_bookmark_info_ptr, 0x00, sizeof(MMIBRW_BOOKMARK_T));
            
            if ((MMI_DEVICE_NUM > s_browser_default_dev) && (0 < s_browser_path_len))//normal
            {
                path_len = MMIFILE_FULL_PATH_MAX_LEN;
                //BrwCombineFullPath((char *)MMIBRW_BOOKMARK_FILE_DIR, path_arr, &path_len);
                BrwGetDefaultRootPath(path_arr, &path_len);

                s_bookmark_info_ptr->total = MMIBRW_UpdateBookmarkTotalNum();
                s_bookmark_info_ptr->file_array_step = 0;
                s_bookmark_info_ptr->full_path_len = MIN(path_len, MMIFILE_FULL_PATH_MAX_LEN);
                MMI_WSTRNCPY((wchar *)s_bookmark_info_ptr->full_path, 
                                            MMIFILE_FULL_PATH_MAX_LEN,  
                                            path_arr, 
                                            s_bookmark_info_ptr->full_path_len, 
                                            s_bookmark_info_ptr->full_path_len);
                
                //SCI_TRACE_LOW:"[BRW]MMIBRW_BookmarkInitAfterStartup bookmark_num=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_822_112_2_18_2_5_39_7,(uint8*)"d", s_bookmark_info_ptr->total);

                if (MMIAPIFMM_IsFolderExist(s_browser_path, s_browser_path_len))//the folder is exit, don't set default bookmarks again
                {
                    //SCI_TRACE_LOW:"[BRW]MMIBRW_BookmarkInitAfterStartup IsFolderExist"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_826_112_2_18_2_5_39_8,(uint8*)"");
                    MMIBROWSER_SetBookmarkLoaded(TRUE);
                }
                else
                {
                    MMIBRW_SetDefaultBookmark();/*if bookmark is not loaded,set default*/
                }
            }
            else
            {
                //SCI_TRACE_LOW:"[BRW]W MMIBRW_BookmarkInitAfterStartup s_browser_default_dev=%d, s_browser_path_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_837_112_2_18_2_5_39_9,(uint8*)"dd", s_browser_default_dev, s_browser_path_len);
            }
            
        }
        else//abort
        {
            //SCI_TRACE_LOW:"[BRW]W MMIBRW_BookmarkInitAfterStartup s_bookmark_info_ptr PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_843_112_2_18_2_5_39_10,(uint8*)"");
        }
    }
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : BookMark EXIT
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_BookmarkExit(void)
{
    if (PNULL != s_bookmark_info_ptr)//protect
    {
        SCI_FREE(s_bookmark_info_ptr);
    }
}

/*****************************************************************************/
//  Description : BookMark finalize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_Bookmarkfinalize(void)
{
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    if(PNULL != bookmark_info_ptr->file_array)
    {
        MMIAPIFILEARRAY_Destroy(&(bookmark_info_ptr->file_array));
    }

    SCI_MEMSET(bookmark_info_ptr, 0x00, sizeof(MMIBRW_BOOKMARK_T));
}
#endif /* Reduce Code size */
#endif

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
// Description : Gets title of specified title entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC FILEARRAY_TYPE_E MMIBRW_GetBookmarkTitle(
                                            uint16 in_index, //in
                                            MMI_STRING_T *out_title//in/out:out_title->wstr_ptr need free outside
                                            )
{
    FILEARRAY_DATA_T array_item = {0};
    wchar *wstr_ptr = PNULL;
    uint32 wstr_byte_len = 0;
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    FILEARRAY_TYPE_E type = FILEARRAY_TYPE_NULL;

    if (PNULL == out_title)
    {
        return type;
    }

    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));

    if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
    {
        if (FILEARRAY_TYPE_FOLDER == array_item.type)
        {
            MMIAPIFMM_SplitFullPath(
                array_item.filename, array_item.name_len, 
                PNULL, PNULL,
                PNULL, PNULL,
                file_name, &file_name_len
            );
        }
        else
        {
            MMIAPIFMM_SplitFullPathExt(
                array_item.filename, array_item.name_len, 
                PNULL, PNULL,
                file_name, &file_name_len,
                PNULL, PNULL);
        }
        
        wstr_byte_len = (((uint32)(file_name_len) + 1) * sizeof(wchar));
        wstr_ptr = SCI_ALLOCA(wstr_byte_len);//free outside
        if (PNULL == wstr_ptr)
        {
            return type;
        }
        SCI_MEMSET(wstr_ptr, 0x00, wstr_byte_len);
        
        MMI_WSTRNCPY(wstr_ptr, file_name_len, file_name, file_name_len, file_name_len);
        //MMI_MEMCPY(wstr_ptr, wstr_byte_len, file_name, wstr_byte_len, wstr_byte_len);
        
        out_title->wstr_len = file_name_len;
        out_title->wstr_ptr = wstr_ptr;
        
        type = array_item.type;
    }

    return type;
}
#else
/*****************************************************************************/
// Description : Gets title of specified title entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_GetBookmarkTitle(
                                uint16 in_index, //in
                                MMI_STRING_T *out_title//in/out
                                )
{    
    MMIBRW_BOOKMARK_ITEM_T* item_ptr = PNULL;

    item_ptr = MMIBRW_BookmarkGetItem(in_index);
    out_title->wstr_ptr = item_ptr->title_arr;
    out_title->wstr_len = item_ptr->title_len;

    return TRUE;
}
#endif

/*****************************************************************************/
// Description : Gets Url of specified title entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_GetBookmarkUrl(
                            uint16 in_index, //in
                            MMI_STRING_T *out_url//in/out:out_url->wstr_ptr need free outside
                            )
{
#ifndef MMIDORADO_BOOKMARK_BY_NV
    FILEARRAY_DATA_T array_item = {0};
    wchar *wstr_ptr = PNULL;
    uint8 * buf_ptr = PNULL;
    uint32 url_len = 0;
    uint8 * url_buf_ptr = PNULL;
    BOOLEAN result = FALSE;
	
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();

    if (PNULL == out_url)
    {
        return FALSE;
    }

    SCI_MEMSET(out_url, 0x00, sizeof(MMI_STRING_T));
    
    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
    {
        if (array_item.size > MMIBRW_BOOKMARK_FILE_MAX_LEN)/*size需要限制大小,避免出错时申请过大内存*/
        {
            SCI_TRACE_LOW("[BRW]W MMIBRW_GetBookmarkUrl:array_item.size =%d", array_item.size);
            return 0;
        }    

        buf_ptr = SCI_ALLOCAZ(array_item.size + 1);
        if (PNULL == buf_ptr)
        {
            return FALSE;
        }
        
        if(MMIAPIFMM_ReadFilesDataSyn((const wchar *)array_item.filename, array_item.name_len, buf_ptr, array_item.size))
        {
            url_buf_ptr = MMIAPIBROWSER_GetUrlByFile(buf_ptr, array_item.size, &url_len);
            if ((url_buf_ptr != PNULL) && (url_len > 0))
            {
                wstr_ptr = SCI_ALLOCA((url_len+1) * sizeof(wchar));//free outside
                if (PNULL == wstr_ptr)
                {
                    SCI_FREE(buf_ptr);
                    return FALSE;
                }
                SCI_MEMSET(wstr_ptr, 0x00, ((url_len+1) * sizeof(wchar)));
                
                out_url->wstr_len = GUI_UTF8ToWstr(wstr_ptr, (uint16)url_len, url_buf_ptr, (uint16)url_len);
                out_url->wstr_ptr = wstr_ptr;
                result = TRUE;
            }
        }
    }

    if (PNULL != buf_ptr)
    {
        SCI_FREE(buf_ptr);
    }
    
    return result;
#else
    MMIBRW_BOOKMARK_ITEM_T* item_ptr = PNULL;
    BOOLEAN result = FALSE;

    item_ptr = MMIBRW_BookmarkGetItem(in_index);
    out_url->wstr_len = SCI_STRLEN(item_ptr->url_ptr);

    if (out_url->wstr_len > 0)
    {
        out_url->wstr_ptr = SCI_ALLOCA((out_url->wstr_len+1) * sizeof(wchar));//free outside
        if (PNULL != out_url->wstr_ptr)
        {
            SCI_MEMSET(out_url->wstr_ptr, 0x00, ((out_url->wstr_len+1) * sizeof(wchar)));
            MMI_STRNTOWSTR(out_url->wstr_ptr, out_url->wstr_len, (uint8*)(item_ptr->url_ptr), out_url->wstr_len, out_url->wstr_len);
            result = TRUE;
        }
    }

    return result;
#endif
}

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
// Description : load new file array(load 根目录或cur_array_index对应的文件夹的file)
// Global resource dependence :
// Author: xie.fen
// Note: fmm_func_type :1 FUNC_FIND_NULL表示使用默认的搜索方式
//          root:FUNC_FIND_FILE_AND_FOLDER  other:FUNC_FIND_FILE
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_LoadBookmarkFileArray(
                        MMIBRW_BOOKMARK_STEP_TYPE_E step_type,//in
                        uint16 cur_array_index,//in:load 根目录时无效
                        MMIFMM_FUNC_TYPE_E   fmm_func_type,//in
                        MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                        MMI_MESSAGE_ID_E    msg_id
                        )
{
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    FILEARRAY_DATA_T f_array_data = {0};
    BOOLEAN is_get_path_result = FALSE;
    //目前filearray接口尚未实现指定排序
    //FILEARRAY_SORT_E sort_type = FILEARRAY_SORT_TIME_DESCEND;
    //BOOLEAN is_need_sort =  FALSE;

    //init
    SCI_MEMSET(bookmark_info_ptr->full_path, 0x00, sizeof(bookmark_info_ptr->full_path));
    bookmark_info_ptr->full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    //get new file path
    switch(step_type)
    {
    case BOOKMARK_STEP_TYPE_ROOT:
    case BOOKMARK_STEP_TYPE_PRE:/*目前只支持一级目录,待扩展*/
        //是否需增加重复判断
        bookmark_info_ptr->file_array_step = 0;
        //BrwCombineFullPath((char *)MMIBRW_BOOKMARK_FILE_DIR, bookmark_info_ptr->full_path, &(bookmark_info_ptr->full_path_len));
        BrwGetDefaultRootPath(bookmark_info_ptr->full_path, &(bookmark_info_ptr->full_path_len));
        is_get_path_result = TRUE;
       break;
        
    case BOOKMARK_STEP_TYPE_NEXT:
        if (MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, cur_array_index, &f_array_data))
        {
            if (FILEARRAY_TYPE_FOLDER == f_array_data.type)
            {
                bookmark_info_ptr->file_array_step ++;
                bookmark_info_ptr->full_path_len = f_array_data.name_len;

                MMI_WSTRNCPY(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len,
                                f_array_data.filename, f_array_data.name_len,
                                f_array_data.name_len);
                
                is_get_path_result = TRUE;
            }
        }
        break;
                
     default:
        break;
    }

    if (!is_get_path_result)//error, do nothing
    {
        return FALSE;
    }

    //begin to load new path 
    if( PNULL != bookmark_info_ptr->file_array)
    {
        MMIAPIFILEARRAY_Destroy(&(bookmark_info_ptr->file_array));
    }
    
    bookmark_info_ptr->file_array = MMIAPIFILEARRAY_Create();

    if (FUNC_FIND_NULL == fmm_func_type)/*通用方式*/
    {
        if (0 == bookmark_info_ptr->file_array_step)/*第一级路径需要搜索文件夹*/
        {
            fmm_func_type = FUNC_FIND_FILE_AND_FOLDER;
        }
        else
        {
            //is_need_sort = TRUE;    //目前filearray接口尚未实现指定排序
            fmm_func_type = FUNC_FIND_FILE;
        }
    }
    
    s_load_fmm_func_type = fmm_func_type;
    
    MMIAPIFMM_SearchFileInPath(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len,
            &s_bookmark_filter,
            FALSE,
            fmm_func_type,
            bookmark_info_ptr->file_array,
            win_id,
            msg_id);

    if (0 == win_id && 0 == msg_id)
    {
        //采用同步方式
        MMIBROWSER_LoadFinish();    
    }
    else
    {
        //异步
    }
    return TRUE;
}

/*****************************************************************************/
// Description : reload file array
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_ReloadBookmarkFileArray(
                        const wchar *path_ptr,
                        uint16 path_len,
                        MMIFMM_FUNC_TYPE_E fmm_func_type
                        )
{
    uint16 num = 0;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    uint16 folder_num = 0;

    if( PNULL != bookmark_info_ptr->file_array)
    {
        MMIAPIFILEARRAY_Destroy(&(bookmark_info_ptr->file_array));
    }
    
    bookmark_info_ptr->file_array = MMIAPIFILEARRAY_Create();

    if (FUNC_FIND_NULL == fmm_func_type)/*通用方式*/
    {
        if (0 == bookmark_info_ptr->file_array_step)/*第一级路径需要搜索文件夹*/
        {
            fmm_func_type = FUNC_FIND_FILE_AND_FOLDER;
        }
        else
        {
            //is_need_sort = TRUE;    //目前filearray接口尚未实现指定排序
            fmm_func_type = FUNC_FIND_FILE;
        }
    }

    MMIAPIFMM_SearchFileInPath(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len,
            &s_bookmark_filter,
            FALSE,
            fmm_func_type,
            bookmark_info_ptr->file_array,
            0,
            0);
    MMIAPIFILEARRAY_Sort(bookmark_info_ptr->file_array, FILEARRAY_SORT_TYPE_ASCEND);
    num = (uint32)MMIAPIFILEARRAY_GetArraySize(bookmark_info_ptr->file_array);
    //SCI_TRACE_LOW:"[BRW]MMIBRW_ReloadBookmarkFileArray num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1153_112_2_18_2_5_40_11,(uint8*)"d", num);

    if ((0 == bookmark_info_ptr->file_array_step) && (FUNC_FIND_FILE_AND_FOLDER == fmm_func_type))//MS00250307
    {
        FILEARRAY folder_file_array = 0;
        folder_file_array = MMIAPIFILEARRAY_Create();
        MMIAPIFMM_SearchFileInPath(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len,
            &s_bookmark_filter,
            FALSE,
            FUNC_FIND_FOLDER,
            folder_file_array,
            0,
            0);
        folder_num = (uint32)MMIAPIFILEARRAY_GetArraySize(folder_file_array);
        MMIAPIFILEARRAY_Destroy(&folder_file_array);

        //SCI_TRACE_LOW:"[BRW]MMIBRW_ReloadBookmarkFileArray folder_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1169_112_2_18_2_5_40_12,(uint8*)"d", folder_num);
        
        folder_num = MIN(MMIBRW_BOOKMARK_DIR_MAX_NUM, folder_num);//protect
        if (folder_num > num)//protect
        {
            folder_num = num;
        }
        bookmark_info_ptr->cur_folder_num = folder_num;

    }
    if ((num - folder_num) > MMIBRW_BOOKMARK_MAX_ITEM)
    {
        num = (MMIBRW_BOOKMARK_MAX_ITEM + folder_num);
    }

    bookmark_info_ptr->cur_index_num = num;

    return TRUE;
}
#endif

/*****************************************************************************/
// Description : get the total bookmark number
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBRW_BookmarkGetCount(void)
{
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();

    SCI_TRACE_LOW("MMIBRW_BookmarkGetCount result = %d", bookmark_info_ptr->total);

    return bookmark_info_ptr->total;
    
#else
    if (s_bookmark_info_ptr != PNULL)
    {
        return s_bookmark_info_ptr->item_count;
    }
    else
    {
        SCI_TRACE_LOW("[BRW]MMIBRW_BookmarkGetCount ERROR");
        return 0;
    }
#endif
}

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
// Description : get index num of current bookmark path(include the "..." index)
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIBRW_GetBookmarkNumber(void)
{
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();

    return bookmark_info_ptr->cur_index_num;
}

/*****************************************************************************/
// Description : get index num of current bookmark path(include the "..." index)
// Global resource dependence :
// Author: dave.ruan
// Note: 
/*****************************************************************************/
PUBLIC MMIBRW_BOOKMARK_T *MMIBRW_GetBookmark(void)
{
#ifdef MMI_BROWSER_MINI_SUPPORT
     /*change mem alloc to after start up*/
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = PNULL;
    if(PNULL == s_bookmark_info_ptr)
    {
        MMIBRW_BookmarkInit();
    }
    bookmark_info_ptr = s_bookmark_info_ptr;
#else
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = s_bookmark_info_ptr;
#endif
    return bookmark_info_ptr;
}
#endif

/*****************************************************************************/
// Description : Access network by bookmark
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIBRW_BookmarkGotoURL(uint16 in_index)
{
    char* url_ptr = PNULL;
    BOOLEAN result = FALSE;
#ifdef MMIDORADO_BOOKMARK_BY_NV
    MMIBRW_BOOKMARK_ITEM_T* item_ptr = PNULL;
#endif

#ifndef MMIDORADO_BOOKMARK_BY_NV
    BrwGetBookmarkUrl(in_index, (uint8 **)&url_ptr);
#else
    item_ptr = MMIBRW_BookmarkGetItem(in_index);
    url_ptr = item_ptr->url_ptr;
#endif

    if (Brw_IsIncludeInvalidUrl((uint8*)url_ptr))
    {
        MMIBROWSER_AlertUrlInvalid();//alert the url is invalid
    }
    else
    {
        result = TRUE;
        MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_URL, (uint8*)url_ptr, PNULL);
    }

    SCI_TRACE_LOW("MMIBRW_BookmarkGotoURL:url_ptr= %s", url_ptr);

#ifndef MMIDORADO_BOOKMARK_BY_NV
    if (url_ptr != PNULL)
    {
        SCI_FREE(url_ptr);
    }
#endif

    return result;
}

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
// Description : Adds a new bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_AddBookmark(MMI_STRING_T in_url_str, //IN
                                            MMI_STRING_T in_title_str, //IN
                                            BOOLEAN in_protect, //IN
                                            uint16 *out_index_ptr,//OUT:new index
                                            wchar *full_path_ptr,//IN:(PNULL: add in current catelog)
                                            BOOLEAN is_need_update_array
                                            )
{
    FILEARRAY_DATA_T array_item = {0};
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_HANDLE file_handle = 0;
    uint8 *url_buf = PNULL;
    uint32 bytes_to_write = 0;
    uint32 bytes_written = 0;
    BOOLEAN result = FALSE;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    MMI_STRING_T string_1 ={0};
    MMI_STRING_T string_dest ={0};
    uint32 dest_wstr_byte_len = 0;
    SFS_DATE_T  modify_date = {0};
    SFS_TIME_T  modify_time = {0};
    MMIFILE_ERROR_E sfs_error = SFS_ERROR_NONE;

    if (PNULL == in_title_str.wstr_ptr)
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBRW_AddBookmark:wstr_ptr PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1283_112_2_18_2_5_40_14,(uint8*)"");
        return FALSE;
    }
    
    //create new bookmark file's name
    if (PNULL == full_path_ptr)
    {
        MMIBRW_BookmarkCombinePath(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len,
            in_title_str.wstr_ptr, in_title_str.wstr_len,
            s_dorado_bk_file_suffix + 1, MMIAPICOM_Wstrlen(s_dorado_bk_file_suffix) -1, 
            file_name, &file_name_len);
    }
    else
    {
        uint16 full_path_len = MMIAPICOM_Wstrlen(full_path_ptr);

        MMIBRW_BookmarkCombinePath(full_path_ptr, full_path_len,
            in_title_str.wstr_ptr, in_title_str.wstr_len,
            s_dorado_bk_file_suffix + 1, MMIAPICOM_Wstrlen(s_dorado_bk_file_suffix) -1, 
            file_name, &file_name_len);
    }

    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_CREATE_NEW, 0, 0);

    do
    {
        if (file_handle != 0)
        {
            //url flag
            string_1.wstr_ptr = (wchar*)s_bookmark_url_flag;
            string_1.wstr_len = MMIAPICOM_Wstrlen(s_bookmark_url_flag);
            
            //url content
            string_dest.wstr_len = string_1.wstr_len + in_url_str.wstr_len;
            dest_wstr_byte_len = (string_dest.wstr_len + 1)*sizeof(wchar);
            
            string_dest.wstr_ptr = SCI_ALLOCA(dest_wstr_byte_len);
            if (PNULL == string_dest.wstr_ptr)
            {
                //SCI_TRACE_LOW:"[BRW]W MMIBRW_AddBookmark:string_dest ALLOC FAIL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1322_112_2_18_2_5_40_15,(uint8*)"");
                break;
            }
            SCI_MEMSET(string_dest.wstr_ptr, 0x00, dest_wstr_byte_len);
                    
            MMIAPICOM_MergeTwoString( &string_1,&in_url_str, &string_dest, string_dest.wstr_len);
            
            bytes_to_write = string_dest.wstr_len;//不允许多余数据写入文件中
            url_buf = SCI_ALLOCA(bytes_to_write + 1);
            if (PNULL == url_buf)
            {
                SCI_FREE(string_dest.wstr_ptr);
                //SCI_TRACE_LOW:"[BRW]W MMIBRW_AddBookmark:url_buf ALLOC FAIL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1334_112_2_18_2_5_40_16,(uint8*)"");
                break;
            }
            SCI_MEMSET(url_buf, 0x00, (bytes_to_write + 1));
            
            MMIAPICOM_WstrToStr(string_dest.wstr_ptr, (uint8 *)url_buf);
            
            SCI_FREE(string_dest.wstr_ptr);

            sfs_error = MMIAPIFMM_WriteFile(file_handle, url_buf, bytes_to_write, &bytes_written, PNULL);
            if (SFS_ERROR_NONE == sfs_error)
            {
                result = TRUE;
            }
            else
            {
                //FALSE
            }            
            //SCI_TRACE_LOW:"[BRW]W MMIBRW_AddBookmark:write file sfs_error =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1352_112_2_18_2_5_40_17,(uint8*)"d", sfs_error);
            SCI_FREE(url_buf);
        }
        else
        {
            //FALSE
        }
    }while(0);

    MMIAPIFMM_CloseFile(file_handle);
    if (!result)
    {
        MMIAPIFMM_DeleteFile(file_name, PNULL);
        return FALSE;
    }

    //add file successfull        
    if (is_need_update_array && (PNULL != bookmark_info_ptr->file_array))
    {
        MMIAPICOM_Wstrncpy(array_item.filename, file_name, file_name_len);
        array_item.name_len = file_name_len;
        MMIAPIFMM_GetFileInfo(array_item.filename, array_item.name_len, 
                            &array_item.size, &modify_date, &modify_time);
        
        array_item.modify_date.mday = modify_date.mday;
        array_item.modify_date.mon = modify_date.mon;
        array_item.modify_date.year = modify_date.year;
        
        array_item.modify_time.sec = modify_time.sec;
        array_item.modify_time.min = modify_time.min;
        array_item.modify_time.hour = modify_time.hour;

        array_item.type = FILEARRAY_TYPE_FILE;
        
        if (MMIAPIFILEARRAY_Add(bookmark_info_ptr->file_array, &array_item))
        {
            bookmark_info_ptr->total++;
            bookmark_info_ptr->cur_index_num++;
            
            if (out_index_ptr != PNULL)
            {
                int32   new_index = 0;
                MMIAPIFILEARRAY_Sort(bookmark_info_ptr->file_array, FILEARRAY_SORT_TYPE_ASCEND);//need to sort first
                new_index = BrwFindArrayIndexByFilename(bookmark_info_ptr->file_array, array_item.filename, array_item.name_len, FILEARRAY_TYPE_FILE);
                *out_index_ptr = (uint16)new_index;
            }
        }
        else//fail add to file array
        {
            MMIAPIFMM_DeleteFile(file_name, PNULL);
            result = FALSE;
            //SCI_TRACE_LOW:"[BRW]W MMIBRW_AddBookmark:MMIFILEARRAY_Add fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1404_112_2_18_2_5_40_18,(uint8*)"");
        }
        
    }
    else
    {
        bookmark_info_ptr->total++;
    }
    
    return result;
}

/*****************************************************************************/
// Description : Adds a new bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_AddBookmarkFolder (MMI_STRING_T in_title_str, BOOLEAN in_protect, uint16 *out_index_ptr)
{
    FILEARRAY_DATA_T array_item = {0};
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN result = FALSE;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();

    if ((PNULL == bookmark_info_ptr->file_array) || (PNULL == in_title_str.wstr_ptr))
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBRW_AddBookmarkFolder:in_title_str.wstr_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1430_112_2_18_2_5_40_19,(uint8*)"d", in_title_str.wstr_ptr);
        return FALSE;
    }
    
    //create new bookmark file's name
    MMIBRW_BookmarkCombinePath(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len,
        in_title_str.wstr_ptr, in_title_str.wstr_len,
        PNULL, PNULL, 
        file_name, &file_name_len);

    if (MMIAPIFMM_CreateDir(file_name, file_name_len))
    {
        MMIAPICOM_Wstrncpy(array_item.filename, file_name, file_name_len);
        array_item.name_len = file_name_len;
        
        TM_GetSysDate(&array_item.modify_date);
        TM_GetSysTime(&array_item.modify_time);
        array_item.type = FILEARRAY_TYPE_FOLDER;

        if (MMIAPIFILEARRAY_Add(bookmark_info_ptr->file_array, &array_item))
        {
            bookmark_info_ptr->cur_index_num++;
            bookmark_info_ptr->cur_folder_num++;
            
            if (out_index_ptr != PNULL)
            {
                int32   new_index = 0;
                MMIAPIFILEARRAY_Sort(bookmark_info_ptr->file_array, FILEARRAY_SORT_TYPE_ASCEND);//need to sort first
                new_index = BrwFindArrayIndexByFilename(bookmark_info_ptr->file_array, array_item.filename, array_item.name_len, FILEARRAY_TYPE_FOLDER);
                *out_index_ptr = (uint16)new_index;
            }

            result = TRUE;
        }
        else//fail
        {
            MMIAPIFMM_DeleteDirectory(file_name);
            //SCI_TRACE_LOW:"[BRW]W MMIBRW_AddBookmarkFolder:MMIFILEARRAY_Add fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1468_112_2_18_2_5_40_20,(uint8*)"");
        }
    }
    else
    {
        //fail
        //SCI_TRACE_LOW:"[BRW]W MMIBRW_AddBookmarkFolder:MMIFILE_CreateDir fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1474_112_2_18_2_5_40_21,(uint8*)"");
    }
    
    return result;
}


/*****************************************************************************/
// Description : Changes the title of specified bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_ChangeBookmarkTitle(uint16 in_index, MMI_STRING_T in_title_str, uint16 *out_index_ptr)
{
    FILEARRAY_DATA_T array_item = {0};
    wchar *dest_wstr_ptr = PNULL;
    uint32 dest_wstr_byte_len = 0;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    wchar           new_full_path[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16          new_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMI_STRING_T string_suffix ={0};
    MMI_STRING_T string_dest ={0};
    BOOLEAN result =FALSE;

    if ((PNULL == bookmark_info_ptr->file_array) || (PNULL == in_title_str.wstr_ptr))
    {
        //SCI_TRACE_LOW:"[BRW]MMIBRW_ChangeBookmarkTitle:in_title_str.wstr_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1499_112_2_18_2_5_40_22,(uint8*)"d", in_title_str.wstr_ptr);
        return FALSE;
    }

    if (PNULL != out_index_ptr)
    {
        *out_index_ptr = in_index;//the time isn't modify, so the index will not be modifide
    }

    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    if (MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
    {
            
        if (FILEARRAY_TYPE_FILE == array_item.type)
        {
            string_suffix.wstr_ptr = (wchar*)s_dorado_bk_file_suffix;
            string_suffix.wstr_len = MMIAPICOM_Wstrlen(s_dorado_bk_file_suffix);
         
            string_dest.wstr_len = in_title_str.wstr_len + string_suffix.wstr_len;
            dest_wstr_byte_len = (string_dest.wstr_len + 1)*sizeof(wchar);
            
            dest_wstr_ptr = SCI_ALLOCA(dest_wstr_byte_len);
            if (PNULL == dest_wstr_ptr)
            {
                return FALSE;
            }
            SCI_MEMSET(dest_wstr_ptr, 0x00, dest_wstr_byte_len);
            
            string_dest.wstr_ptr = dest_wstr_ptr;
            
            MMIAPICOM_MergeTwoString(&in_title_str, &string_suffix, &string_dest, string_dest.wstr_len);
            
            result = BrwRenameFile(array_item.filename, array_item.name_len,
                        string_dest.wstr_ptr, string_dest.wstr_len,
                        new_full_path, &new_full_path_len);
            
            SCI_FREE(dest_wstr_ptr);
        }
        else if (MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
        {
            
           result = BrwRenameFile(array_item.filename, array_item.name_len,
                    in_title_str.wstr_ptr, in_title_str.wstr_len,
                    new_full_path, &new_full_path_len);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[BRW]MMIBRW_ChangeBookmarkTitle:MMIFILEARRAY_Read fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1547_112_2_18_2_5_40_23,(uint8*)"");
    }
     
    //rename success, update the filearray
    if (result)
    {
        FILEARRAY_DATA_T new_array_item = {0};

        SCI_MEMCPY(&new_array_item, &array_item, sizeof(FILEARRAY_DATA_T));
        
        SCI_MEMSET(new_array_item.filename, 0, sizeof(new_array_item.filename));
        new_array_item.name_len = MIN(MMIFILE_FULL_PATH_MAX_LEN, new_full_path_len);
        MMI_WSTRNCPY(new_array_item.filename, MMIFILE_FULL_PATH_MAX_LEN, new_full_path, new_full_path_len, new_array_item.name_len);
        
        result = MMIAPIFILEARRAY_Modify(bookmark_info_ptr->file_array, (uint32)in_index, &new_array_item);
        if (!result)//update fail ,so rename back
        {
            SFS_RenameFile(new_full_path,  array_item.filename, PNULL);
        }
        
        //SCI_TRACE_LOW:"[BRW]MMIBRW_ChangeBookmarkTitle:MMIFILEARRAY_Modify result =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1567_112_2_18_2_5_40_24,(uint8*)"d", result);
    }

    return result;
}

/*****************************************************************************/
// Description : Changes the url of specified bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_ChangeBookmarkUrl(uint16 in_index, MMI_STRING_T in_url_str, uint16 *out_index_ptr)
{
    FILEARRAY_DATA_T array_item = {0};
    uint8 *url_buf = PNULL;
    uint32 bytes_to_write = 0;
    BOOLEAN result = FALSE;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    MMI_STRING_T string_prefix ={0};
    MMI_STRING_T string_dest ={0};
    wchar *dest_wstr_ptr = PNULL;
    uint32 dest_wstr_byte_len = 0;
    SFS_DATE_T  modify_date = {0};
    SFS_TIME_T  modify_time = {0};
    
    if ((PNULL == bookmark_info_ptr->file_array) || (PNULL == in_url_str.wstr_ptr))
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBRW_ChangeBookmarkUrl:in_url_str.wstr_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1593_112_2_18_2_5_40_25,(uint8*)"d", in_url_str.wstr_ptr);
        return FALSE;
    }

    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    //SCI_TRACE_LOW:"[BRW]MMIBRW_ChangeBookmarkUrl:in_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1598_112_2_18_2_5_40_26,(uint8*)"d", in_index);

    if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
    {
        string_prefix.wstr_ptr = (wchar*)s_bookmark_url_flag;
        string_prefix.wstr_len = MMIAPICOM_Wstrlen(s_bookmark_url_flag);
     
        string_dest.wstr_len = in_url_str.wstr_len + string_prefix.wstr_len;
        dest_wstr_byte_len = (string_dest.wstr_len + 1)*sizeof(wchar);
        
        dest_wstr_ptr = SCI_ALLOCA(dest_wstr_byte_len);
        if (PNULL == dest_wstr_ptr)
        {
            //SCI_TRACE_LOW:"[BRW]W MMIBRW_ChangeBookmarkUrl:dest_wstr_ptr alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1611_112_2_18_2_5_40_27,(uint8*)"");
            return FALSE;
        }
        SCI_MEMSET(dest_wstr_ptr, 0x00, dest_wstr_byte_len);
        
        string_dest.wstr_ptr = dest_wstr_ptr;
        
        MMIAPICOM_MergeTwoString(&string_prefix, &in_url_str, &string_dest, string_dest.wstr_len);
        
        bytes_to_write = string_dest.wstr_len;
        url_buf = SCI_ALLOCA(bytes_to_write + 1);
        if (PNULL == url_buf)
        {
            SCI_FREE(dest_wstr_ptr);
            //SCI_TRACE_LOW:"[BRW]W MMIBRW_ChangeBookmarkUrl:url_buf alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1625_112_2_18_2_5_40_28,(uint8*)"");
            return FALSE;
        }
        SCI_MEMSET(url_buf, 0x00, (bytes_to_write + 1));
        MMIAPICOM_WstrToStr(string_dest.wstr_ptr, (uint8 *)url_buf);
        //SCI_TRACE_LOW:"[BRW]MMIBRW_ChangeBookmarkUrl buf=%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1630_112_2_18_2_5_40_29,(uint8*)"s", url_buf + string_prefix.wstr_len);
        SCI_FREE(dest_wstr_ptr);

        if (MMIAPIFMM_WriteFilesDataSyn(array_item.filename, array_item.name_len, url_buf, bytes_to_write))
        {
            result = TRUE;
        }
        //SCI_TRACE_LOW:"[BRW]MMIBRW_ChangeBookmarkUrl:write file data result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1637_112_2_18_2_5_40_30,(uint8*)"d", result);
        
        SCI_FREE(url_buf);
        url_buf = PNULL;
    }
    else
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBRW_ChangeBookmarkUrl: MMIFILEARRAY_Read fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1644_112_2_18_2_5_41_31,(uint8*)"");
    }

    //update
    if (result && (PNULL != bookmark_info_ptr->file_array))
    {
        MMIAPIFMM_GetFileInfo(array_item.filename, array_item.name_len, &(array_item.size), &modify_date, &modify_time);
        array_item.modify_date.mday = modify_date.mday;
        array_item.modify_date.mon = modify_date.mon;
        array_item.modify_date.year = modify_date.year;
        
        array_item.modify_time.sec = modify_time.sec;
        array_item.modify_time.min = modify_time.min;
        array_item.modify_time.hour = modify_time.hour;
        result = MMIAPIFILEARRAY_Modify(bookmark_info_ptr->file_array, (uint32)in_index, &array_item);
        
        if (out_index_ptr != PNULL)
        {
            int32   new_index = 0;
            MMIAPIFILEARRAY_Sort(bookmark_info_ptr->file_array, FILEARRAY_SORT_TYPE_ASCEND);//need to sort first
            new_index = BrwFindArrayIndexByFilename(bookmark_info_ptr->file_array, array_item.filename, array_item.name_len, FILEARRAY_TYPE_FILE);
            *out_index_ptr = (uint16)new_index;
        }
        
        //SCI_TRACE_LOW:"[BRW]MMIBRW_ChangeBookmarkUrl:MMIFILEARRAY_Modify result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1668_112_2_18_2_5_41_32,(uint8*)"d", result);
    }

    return result;
}
#endif

/*****************************************************************************/
// Description : Deletes bookmark entry specified by in_index
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_RemoveBookmark(uint16 in_index , BOOLEAN is_for_delete_all)
{
#ifndef MMIDORADO_BOOKMARK_BY_NV
    FILEARRAY_DATA_T array_item = {0};
    BOOLEAN result = FALSE;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32 files_num = 0;
    FILEARRAY file_array = PNULL;

    if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
    {
        switch (array_item.type)
        {
        case FILEARRAY_TYPE_FILE:
            if (SFS_NO_ERROR == MMIAPIFMM_DeleteFile((const wchar *)array_item.filename,  PNULL))
            {
                if (!is_for_delete_all)
                {
                    result = MMIAPIFILEARRAY_Delete(bookmark_info_ptr->file_array, (uint32)in_index);
                }
                else
                {
                    result = TRUE;
                }
                if (0 < bookmark_info_ptr->cur_index_num)
                {
                    bookmark_info_ptr->cur_index_num--;
                }

                if (0 < bookmark_info_ptr->total)
                {
                    bookmark_info_ptr->total--;
                }
            }
            break;

        case FILEARRAY_TYPE_FOLDER:
            //需要先得到dir下的files,然后一一删除
            MMIAPIFMM_SplitFullPath(
                        array_item.filename, array_item.name_len, 
                        PNULL, PNULL,
                        PNULL, PNULL,
                        file_name, &file_name_len
                        );
            
            file_array = MMIAPIFILEARRAY_Create();
            MMIAPIFMM_SearchFileInPath(array_item.filename, array_item.name_len,
                    &s_bookmark_filter,
                    FALSE,
                    FUNC_FIND_FILE,
                    file_array,
                    0,
                    0);
            
            files_num = MMIAPIFILEARRAY_GetArraySize(file_array);
            MMIAPIFILEARRAY_Destroy(&file_array);
            
            if (SFS_NO_ERROR == MMIAPIFMM_DeleteTree(bookmark_info_ptr->full_path , file_name))
            {
                if (!is_for_delete_all)
                {
                    result = MMIAPIFILEARRAY_Delete(bookmark_info_ptr->file_array, (uint32)in_index);
                }
                else
                {
                    result = TRUE;
                }
                if (0 < bookmark_info_ptr->cur_index_num)
                {
                    bookmark_info_ptr->cur_index_num--;
                    bookmark_info_ptr->cur_folder_num--;
                }

                if (files_num <= bookmark_info_ptr->total)
                {
                    bookmark_info_ptr->total -= files_num;
                }
            }
            break;
            
        default:
            break;
        }

    }        

    SCI_TRACE_LOW("MMIBRW_RemoveBookmark in_index = %d, is_for_delete_all = %d, type = %d", in_index, is_for_delete_all, array_item.type);

    return result;
#else
    if (is_for_delete_all)
    {
        return MMIBRW_RemoveAllBookmark();
    }

#ifdef MMI_BROWSER_MINI_SUPPORT
    if(PNULL == s_bookmark_info_ptr)
    {
        MMIBRW_BookmarkInit();
    }
#endif
    if(in_index >= s_bookmark_info_ptr->item_count)
    {
        return FALSE;
    }
    
    if (in_index != (s_bookmark_info_ptr->item_count -1))
    {
        SCI_MEMCPY(
                &s_bookmark_info_ptr->item_info[in_index],
                &s_bookmark_info_ptr->item_info[in_index + 1],
                (s_bookmark_info_ptr->item_count - in_index -1)*sizeof(MMIBRW_BOOKMARK_ITEM_T)
                );
    }
    SCI_MEMSET(&s_bookmark_info_ptr->item_info[s_bookmark_info_ptr->item_count-1],0,sizeof(MMIBRW_BOOKMARK_ITEM_T));
    s_bookmark_info_ptr->item_count--;
    BookmarkOverwriteInfo();
    
    return TRUE;
#endif
}

/*****************************************************************************/
// Description : Deletes all bookmark include file and folder,but only in root
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_RemoveAllBookmark(void)
{
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    int32 index = 0;
    BOOLEAN result = FALSE;
    uint16 total_cur_num = bookmark_info_ptr->cur_index_num;

    for (index = 0; index < total_cur_num; index++)//注意:只用于root，故cur_index_num此时就是root下的文件总数
    {
        result = MMIBRW_RemoveBookmark(index, TRUE);       
    }
    //update
    MMIBRW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL, 0, 0);
    bookmark_info_ptr->total = MMIBRW_UpdateBookmarkTotalNum();
    if (bookmark_info_ptr->total > 0)
    {
        result = FALSE;
    }

    SCI_TRACE_LOW("MMIBRW_RemoveAllBookmark result = %d", result);
    return result;
#else
#ifdef MMI_BROWSER_MINI_SUPPORT
    if(PNULL == s_bookmark_info_ptr)
    {
        MMIBRW_BookmarkInit();
    }
#endif
    SCI_MEMSET(s_bookmark_info_ptr, 0, sizeof(MMIBRW_BOOKMARK_T));
    BookmarkOverwriteInfo();
    return TRUE;
#endif
}

#ifndef MMIDORADO_BOOKMARK_BY_NV
#if 0
/*****************************************************************************/
//  Description : BookMark Initialize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void BrwInitializeBookmarkFileArray(void)
{
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();

    if (PNULL == bookmark_info_ptr->file_array)
    {
        MMIBRW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL, 0, 0);
    }

}
#endif

/*****************************************************************************/
// Description : Deletes all bookmark in folder,but do Not delete folder
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_RemoveAllBookmarkOfSubFolder(void)
{
    BOOLEAN ret = FALSE;
    wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar folder[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    int32 i = 0;
    int16 path_len = 0;

    if (PNULL == bookmark_info_ptr )
    {
        return FALSE;
    }
    path_len = MMIAPICOM_Wstrlen(bookmark_info_ptr->full_path);

    for (i = path_len; i > 0 ; i--)
    {
        if (bookmark_info_ptr->full_path[i] == '\\')
        {
            MMI_WSTRNCPY(full_path_name, MMIFILE_FULL_PATH_MAX_LEN, bookmark_info_ptr->full_path, MMIFILE_FULL_PATH_MAX_LEN, i);
            MMI_WSTRNCPY(folder, MMIFILE_FULL_PATH_MAX_LEN, bookmark_info_ptr->full_path + i + 1, MMIFILE_FULL_PATH_MAX_LEN, (path_len - 1 - i));
            break;
        }
    }

    if (0 < i)
    {
        if (SFS_NO_ERROR == MMIAPIFMM_DeleteTreeEx(full_path_name , folder, FALSE))
        {
            ret = TRUE;
        }
    }

    MMIBRW_ReloadBookmarkFileArray(bookmark_info_ptr->full_path, path_len, FUNC_FIND_FILE);

    SCI_TRACE_LOW("MMIBRW_RemoveAllBookmarkOfSubFolder ret = %d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : Clean bookmark Data Info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 清空书签
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_CleanBookmarkData(BOOLEAN in_force)
{
    BOOLEAN result = TRUE;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar folder_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 folder_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    FILEARRAY   root_file_array = {0};
    FILEARRAY_DATA_T array_data = {0};
    uint32 root_num = 0;
    uint32 index = 0;

    //BrwCombineFullPath((char *)MMIBRW_BOOKMARK_FILE_DIR, full_path_name, &full_path_name_len);
    BrwGetDefaultRootPath(full_path_name, &full_path_name_len);
    //根目录下
    root_file_array = MMIAPIFILEARRAY_Create();    

    if (PNULL == root_file_array)
    {
        //SCI_TRACE_LOW:"MMIBRW_CleanBookmarkData root_file_array == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1876_112_2_18_2_5_41_33,(uint8*)"");
        return FALSE;
    }
    
    MMIAPIFMM_SearchFileInPath(full_path_name, full_path_name_len,
            &s_bookmark_filter,
            FALSE,
            FUNC_FIND_FILE_AND_FOLDER,
            root_file_array,
            0,
            0);
    
    root_num = MMIAPIFILEARRAY_GetArraySize(root_file_array);

    for(index = 0; index < root_num; index++)
    {
        SCI_MEMSET(&array_data, 0x00, sizeof(array_data));
        MMIAPIFILEARRAY_Read(root_file_array, index, &array_data);

        if (FILEARRAY_TYPE_FILE == array_data.type)
        {
            if (SFS_NO_ERROR != MMIAPIFMM_DeleteFile((const wchar *)array_data.filename,  PNULL))
            {
                result = FALSE;
                //break;
            }
        }
        else if (FILEARRAY_TYPE_FOLDER == array_data.type)
        {
            SCI_MEMSET(folder_name, 0x00, sizeof(folder_name));

            //需要先得到dir下的files,然后一一删除
            MMIAPIFMM_SplitFullPath(
                        array_data.filename, array_data.name_len, 
                        PNULL, PNULL,
                        PNULL, PNULL,
                        folder_name, &folder_name_len
                        );
            if (SFS_NO_ERROR != MMIAPIFMM_DeleteTree(full_path_name , folder_name))
            {
                result = FALSE;
                //break;
            }
        }

    }
    /*clean data,reset bookmark infor */
    if (PNULL != bookmark_info_ptr)
    {
        SCI_MEMSET(bookmark_info_ptr, 0x00, sizeof(MMIBRW_BOOKMARK_T));
        //BrwCombineFullPath((char *)MMIBRW_BOOKMARK_FILE_DIR, full_path_name, &full_path_name_len);
        BrwGetDefaultRootPath(full_path_name, &full_path_name_len);
        
        bookmark_info_ptr->total = 0;
        bookmark_info_ptr->file_array_step = 0;
        bookmark_info_ptr->full_path_len = full_path_name_len;
        MMI_WSTRNCPY((wchar *)bookmark_info_ptr->full_path, 
                                    MMIFILE_FULL_PATH_MAX_LEN,  
                                    full_path_name, 
                                    bookmark_info_ptr->full_path_len, 
                                    bookmark_info_ptr->full_path_len);
        
        //SCI_TRACE_LOW:"[BRW]MMIBRW_CleanBookmarkData bookmark_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1938_112_2_18_2_5_41_34,(uint8*)"d", bookmark_info_ptr->total);
    }
    
    MMIAPIFILEARRAY_Destroy(&root_file_array);
    
    //SCI_TRACE_LOW:"MMIBRW_CleanBookmarkData result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_1943_112_2_18_2_5_41_35,(uint8*)"d", result);

    return result;
}
#endif

/*****************************************************************************/
//  Description : Set default bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_SetDefaultBookmark(void)
{
#ifndef MMIDORADO_BOOKMARK_BY_NV
#ifdef IM_SIMP_CHINESE_SUPPORT//just for chinese for temp 
    //int32 arrary_num = 0;
    int32 i = 0;
    BOOLEAN ret = FALSE;
    MMI_STRING_T url_str = {0};
    MMI_STRING_T title_str = {0};
    BOOLEAN is_res_data_exist = FALSE;
    MMIRES_BOOKMARK_INFO_T res_info = {0};

    //初始化后先删除所有的bookmark
    MMIBRW_CleanBookmarkData(TRUE);

    is_res_data_exist = MMIRES_LoadBookmarkFromRes(&res_info);

    if (is_res_data_exist && PNULL != res_info.res_item_ptr)
    {
        res_info.res_num = MIN(res_info.res_num, MMIBRW_BOOKMARK_MAX_ITEM);
        
        for (i = 0; i < res_info.res_num; i++)
        {
            title_str.wstr_len = res_info.res_item_ptr[i].name_len;
            title_str.wstr_ptr = res_info.res_item_ptr[i].name;
            url_str.wstr_len = res_info.res_item_ptr[i].url_len;
            url_str.wstr_ptr = SCI_ALLOCA((url_str.wstr_len + 1) * sizeof(wchar));
            if (PNULL != url_str.wstr_ptr)
            {
                SCI_MEMSET(url_str.wstr_ptr, 0x00, ((url_str.wstr_len + 1) * sizeof(wchar)));
                GUI_GBToWstr(url_str.wstr_ptr, (const uint8*)res_info.res_item_ptr[i].url, url_str.wstr_len);
                ret = MMIBRW_AddBookmark(url_str, title_str, TRUE, PNULL, PNULL, FALSE);
                
                SCI_FREE(url_str.wstr_ptr);
          
                //SCI_TRACE_LOW:"MMIBRW_SetDefaultBookmark ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_2015_112_2_18_2_5_41_36,(uint8*)"d", ret);
            }
            else
            {
                MMIRES_UnloadBookmarkInfo(&res_info);
            
                //SCI_TRACE_LOW:"MMIBRW_SetDefaultBookmark NO memory"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_2021_112_2_18_2_5_41_37,(uint8*)"");
                return;
            }
        }

        /*load bookmark success,remember it*/
        if(ret)
        {
            MMIBROWSER_SetBookmarkLoaded(TRUE);
        }

        MMIRES_UnloadBookmarkInfo(&res_info);
    }
    else
    {
        MMIBROWSER_SetBookmarkLoaded(TRUE);
    }

#else/*IM_SIMP_CHINESE_SUPPORT*/
    MMIBRW_CleanBookmarkData(TRUE);
    MMIBROWSER_SetBookmarkLoaded(TRUE);
#endif/*IM_SIMP_CHINESE_SUPPORT*/

#else/*MMIDORADO_BOOKMARK_BY_NV*/
    BookmarkReadInfo(TRUE);
#endif/*MMIDORADO_BOOKMARK_BY_NV*/

#ifdef BROWSER_START_PAGE_THEME1
    MMIBROWSER_UpdateBkListToRoot();
#endif
}
#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
// Description : Find duplicated Bookmark
// Global resource dependence :
// Author: fen.xie
// Note: negative number  means there is no duplicated bookmark
/*****************************************************************************/
PUBLIC int32 MMIBRW_FindDuplicatedBookmarkIndex(wchar *cur_path_ptr, 
                    uint16 cur_path_len, 
                    uint16 path_step, 
                    wchar *new_name_ptr)
{
    int32 bm_num = 0;
    int32 bm_index = -1;
    int32 i = 0;
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16   file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    FILEARRAY_DATA_T f_array_data = {0};
    FILEARRAY   file_array = {0};
    MMIFMM_FUNC_TYPE_E  func_type = FUNC_FIND_NULL;
    
    file_array = MMIAPIFILEARRAY_Create();

    if (file_array != PNULL)
    {
        //get new file's name
        MMIBRW_BookmarkCombinePath(cur_path_ptr, cur_path_len, 
            new_name_ptr, MMIAPICOM_Wstrlen(new_name_ptr), 
            s_dorado_bk_file_suffix + 1, MMIAPICOM_Wstrlen(s_dorado_bk_file_suffix) -1, 
            file_name, &file_name_len);
		{
			//多重文件夹下‘/’‘\’调整
			uint i = 0;
			while (i<file_name_len)
			{
				if ('/' == file_name[i])
				{
					file_name[i] = '\\';
				}
				++i;
			}
		}
    
        func_type=  (path_step > 0)? FUNC_FIND_FILE: FUNC_FIND_FILE_AND_FOLDER;
        MMIAPIFMM_SearchFileInPath(cur_path_ptr,
                cur_path_len,
                &s_bookmark_filter,
                TRUE,
                func_type,
                file_array,
                0,
                0);
        bm_num = MMIAPIFILEARRAY_GetArraySize(file_array);

        MMIAPICOM_Wstrlower(file_name);
        MMIAPIFILEARRAY_Sort(file_array, FILEARRAY_SORT_TYPE_ASCEND);
        if (PNULL != new_name_ptr && 0 != MMIAPICOM_Wstrlen(new_name_ptr))
        {
            for (i = 0; i < bm_num; i++)
            {
                SCI_MEMSET(&f_array_data, 0x00, sizeof(f_array_data));
                MMIAPIFILEARRAY_Read(file_array, i, &f_array_data);                    
                MMIAPICOM_Wstrlower(f_array_data.filename);
                
                if ((FILEARRAY_TYPE_FILE == f_array_data.type) && (0 == MMIAPICOM_Wstrcmp(f_array_data.filename, file_name)))
                {
                    bm_index = i;
                    break;
                }
            }
        }
        
        MMIAPIFILEARRAY_Destroy(&file_array);
    }

    SCI_TRACE_LOW("MMIBRW_FindDuplicatedBookmarkIndex bm_index = %d", bm_index);

    return bm_index;
}

/*****************************************************************************/
// Description : Find duplicated Bookmark dir
// Global resource dependence :
// Author: fen.xie
// Note: negative number  means there is no duplicated bookmark
/*****************************************************************************/
PUBLIC int32 MMIBRW_FindDuplicatedBookmarkDirIndex(wchar *cur_path_ptr, 
                    uint16 cur_path_len, 
                    uint16 cur_path_num, 
                    wchar *new_name_ptr)
{
    int32 bm_num = 0;
    int32 bm_index = -1;
    int32 i = 0;
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16   file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar   tmp_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    
    FILEARRAY_DATA_T f_array_data = {0};
    FILEARRAY   file_array = {0};
    
    MMI_WSTRNCPY(file_name, file_name_len,
                                cur_path_ptr, cur_path_len,
                                MIN(cur_path_len, file_name_len));
    file_name_len = MMIAPICOM_Wstrlen(file_name);

    file_array = MMIAPIFILEARRAY_Create();

    MMIAPIFMM_SearchFileInPath(file_name,
            file_name_len,
            &s_bookmark_filter,
            FALSE,
            FUNC_FIND_FOLDER,
            file_array,
            0,
            0);
    MMIAPIFILEARRAY_Sort(file_array, FILEARRAY_SORT_TYPE_ASCEND);
    bm_num = MMIAPIFILEARRAY_GetArraySize(file_array);

    if (PNULL != new_name_ptr && 0 != MMIAPICOM_Wstrlen(new_name_ptr))
    {
  
        SCI_MEMSET(&tmp_name, 0x00, sizeof(tmp_name));
        MMIAPICOM_Wstrncpy( tmp_name, new_name_ptr, MMIAPICOM_Wstrlen(new_name_ptr) );
        MMIAPICOM_Wstrlower(tmp_name);
    
        for (i = 0; i < bm_num; i++)
        {
            SCI_MEMSET(&f_array_data, 0x00, sizeof(f_array_data));
            SCI_MEMSET(&file_name, 0x00, sizeof(file_name));
            file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
            MMIAPIFILEARRAY_Read(file_array, i, &f_array_data);
            MMIAPIFMM_SplitFullPath(
                    f_array_data.filename, 
                    f_array_data.name_len, 
                    PNULL, PNULL,
                    PNULL, PNULL,
                    file_name, &file_name_len
                    );
            MMIAPICOM_Wstrlower(file_name);
            if (0 == MMIAPICOM_Wstrcmp(file_name, tmp_name))
            {
                bm_index = i;
                break;
            }
        }
    }

    MMIAPIFILEARRAY_Destroy(&file_array);

    SCI_TRACE_LOW("MMIBRW_FindDuplicatedBookmarkDirIndex bm_index = %d", bm_index);

    return bm_index;
}

/*****************************************************************************/
//  Description : Bookmark Is In Root
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkIsInRoot(void)
{
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    BOOLEAN result = FALSE;
    if (0 == bookmark_info_ptr->file_array_step)
    {
        result = TRUE;
    }

    SCI_TRACE_LOW("MMIBRW_BookmarkIsInRoot result = %d", result);

    return result;
}

/*****************************************************************************/
//  Description : Bookmark Get Folder Num
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIBRW_BookmarkGetFolderNum(BOOLEAN is_by_filearray)
{
    uint32 num = 0;

    if (is_by_filearray)
    {
        FILEARRAY file_array = PNULL;
        wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
        uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
        
        file_array = MMIAPIFILEARRAY_Create();
        
        //BrwCombineFullPath((char *)MMIBRW_BOOKMARK_FILE_DIR, file_name, &file_name_len);
        BrwGetDefaultRootPath(file_name, &file_name_len);
        
        MMIAPIFMM_SearchFileInPath(file_name, file_name_len,
                &s_bookmark_filter,
                FALSE,
                FUNC_FIND_FOLDER,
                file_array,
                0,
                0);

        num = MMIAPIFILEARRAY_GetArraySize(file_array);

        MMIAPIFILEARRAY_Destroy(&file_array);
    }
    else
    {
        MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
        num = bookmark_info_ptr->cur_folder_num;
    }
    
    num = MIN(MMIBRW_BOOKMARK_DIR_MAX_NUM, num);
    
    return num;
} 

/*****************************************************************************/
//  Description : Bookmark Get Folder Num
//  Global resource dependence : 
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkGetFolderName(FILEARRAY cur_file_array,
                uint16 fodler_index,  //folder index of all folder
                MMI_STRING_T *out_folder_name//in/out:out_folder_name->wstr_ptr need free outside
                )
{
    FILEARRAY_DATA_T array_item = {0};
    FILEARRAY file_array = PNULL;
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint16  wstr_byte_len = 0;
    wchar *wstr_ptr = PNULL;
    FILEARRAY temp_file_array = PNULL;
    BOOLEAN result = FALSE;

    if (PNULL== cur_file_array)
    {
        temp_file_array = MMIAPIFILEARRAY_Create();
    
        //BrwCombineFullPath((char *)MMIBRW_BOOKMARK_FILE_DIR, file_name, &file_name_len);
        BrwGetDefaultRootPath(file_name, &file_name_len);
        
        MMIAPIFMM_SearchFileInPath(file_name, file_name_len,
                &s_bookmark_filter,
                FALSE,
                FUNC_FIND_FOLDER,
                temp_file_array,
                0,
                0);
        
        /*find in order to get the right index*/
        MMIAPIFILEARRAY_Sort(temp_file_array, FILEARRAY_SORT_TYPE_ASCEND);
        file_array = temp_file_array;
    }
    else
    {
        file_array = cur_file_array;
    }

    do
    {
        SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
        if(MMIAPIFILEARRAY_Read(file_array, (uint32)fodler_index, &array_item ))
        {
            MMIAPIFMM_SplitFullPath(
                array_item.filename, array_item.name_len, 
                PNULL, PNULL,
                PNULL, PNULL,
                file_name, &file_name_len
            );
            
            wstr_byte_len = (((uint32)(file_name_len) + 1) * sizeof(wchar));
            wstr_ptr = SCI_ALLOCA(wstr_byte_len);//free outside
            if (PNULL == wstr_ptr)
            {
                break;
            }
            SCI_MEMSET(wstr_ptr, 0x00, wstr_byte_len);
            
            MMI_WSTRNCPY(wstr_ptr, file_name_len, file_name, file_name_len, file_name_len);
            
            out_folder_name->wstr_len = file_name_len;
            out_folder_name->wstr_ptr = wstr_ptr;

            result = TRUE;
        } 
    }
    while(0);
    
    if (temp_file_array != PNULL)
    {
        MMIAPIFILEARRAY_Destroy(&temp_file_array);
    }
    
    return result;
} 

/*****************************************************************************/
//  Description : Bookmark Get Folder Num
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkGetFolderNameArray(FILEARRAY cur_file_array,
                    MMI_STRING_T **out_folder_name_pptr,//[in/out]:*out_folder_name_pptr and out_folder_name_pptr[i]->wstr_ptr need free outside
                    uint16  *folder_total_num_ptr//[in/out]
                    )
{
    FILEARRAY file_array = PNULL;
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    FILEARRAY temp_file_array = PNULL;
    uint16  folder_total_num = 0;
    uint32  loop = 0;
    BOOLEAN result = TRUE;

    if ((PNULL == out_folder_name_pptr) && (PNULL == folder_total_num_ptr))
    {
        return FALSE;
    }

    if (PNULL== cur_file_array)
    {
        temp_file_array = MMIAPIFILEARRAY_Create();
        BrwGetDefaultRootPath(file_name, &file_name_len);
        MMIAPIFMM_SearchFileInPath(file_name, file_name_len,
                &s_bookmark_filter,
                FALSE,
                FUNC_FIND_FOLDER,
                temp_file_array,
                0,
                0);
        
        /*find in order to get the right index*/
        MMIAPIFILEARRAY_Sort(temp_file_array, FILEARRAY_SORT_TYPE_ASCEND);
		file_array = temp_file_array;
    }
    else
    {
        file_array = cur_file_array;
    }

    folder_total_num= MMIAPIFILEARRAY_GetArraySize(file_array);
    folder_total_num = MIN(MMIBRW_BOOKMARK_DIR_MAX_NUM, folder_total_num);//protect

    if (out_folder_name_pptr != PNULL)
    {
        do
        {
            if (folder_total_num > 0)
            {
                *out_folder_name_pptr = SCI_ALLOCA((folder_total_num + 1) * sizeof(MMI_STRING_T));
                if (PNULL == *out_folder_name_pptr)
                {
                    result = FALSE;
                    break;
                }
                SCI_MEMSET(*out_folder_name_pptr, 0x00, ((folder_total_num + 1) * sizeof(MMI_STRING_T)));
                for(loop = 0; loop < folder_total_num; loop++)
                {
                    MMI_STRING_T    *folder_name_ptr = (*out_folder_name_pptr) + loop;
                    
                    if (folder_name_ptr != PNULL)/*lint !e774*/
                    {
                        MMIBRW_BookmarkGetFolderName(file_array, loop, folder_name_ptr);
                    }
                }
            }
        }
        while(0);
    }
    
    if (temp_file_array != PNULL)
    {
        MMIAPIFILEARRAY_Destroy(&temp_file_array);
    }
    
    if (folder_total_num_ptr != PNULL)
    {
        *folder_total_num_ptr = folder_total_num;
    }
    
    return result;
} 

/*****************************************************************************/
//  Description : check if the default bookmark is load,and record in NV 
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsBookmarkLoaded(void)
{
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
    BOOLEAN is_bookmark_load = FALSE;
    
    MMINV_READ(MMINV_BROWSER_IS_BOOKMARK_LOAD, &is_bookmark_load, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        is_bookmark_load = FALSE;

        MMINV_WRITE( MMINV_BROWSER_IS_BOOKMARK_LOAD, &is_bookmark_load);
    }
    
    return is_bookmark_load;
}

/*****************************************************************************/
//  Description : check if the default bookmark is load,and record in NV 
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetBookmarkLoaded(BOOLEAN is_load)
{
    MMINV_WRITE( MMINV_BROWSER_IS_BOOKMARK_LOAD, &is_load);
}
/*****************************************************************************/
//  Description : Bookmark Get Folder
//  Global resource dependence : 
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkGetFolder(
                                                                            uint16 fodler_index,  //folder index of all folder
                                                                            wchar *full_path_ptr,
                                                                            uint16 *full_path_len_ptr,
                                                                            FILEARRAY cur_file_array
                                                                            )
{
    FILEARRAY_DATA_T array_item = {0};
    FILEARRAY temp_file_array = PNULL;
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    FILEARRAY file_array = PNULL;

    if ((PNULL == full_path_ptr) || (PNULL == full_path_len_ptr) || (0 == *full_path_len_ptr))
    {
        return FALSE;
    }

    if (PNULL== cur_file_array)
    {
        temp_file_array = MMIAPIFILEARRAY_Create();
        
        //BrwCombineFullPath((char *)MMIBRW_BOOKMARK_FILE_DIR, file_name, &file_name_len);
        BrwGetDefaultRootPath(file_name, &file_name_len);
        
        MMIAPIFMM_SearchFileInPath(file_name, file_name_len,
                &s_bookmark_filter,
                FALSE,
                FUNC_FIND_FOLDER,
                temp_file_array,
                0,
                0);
        
        /*find in order to get the right index*/
        MMIAPIFILEARRAY_Sort(temp_file_array, FILEARRAY_SORT_TYPE_ASCEND);
        file_array = temp_file_array;
    }
    else
    {
        file_array = cur_file_array;
    }
   
    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    
    if (MMIAPIFILEARRAY_Read(file_array, (uint32)fodler_index, &array_item))
    {
        *full_path_len_ptr = MIN(*full_path_len_ptr, array_item.name_len);

        MMI_WSTRNCPY(full_path_ptr, *full_path_len_ptr,
                                array_item.filename, array_item.name_len, *full_path_len_ptr);
    }
    else
    {
        *full_path_len_ptr = MIN(*full_path_len_ptr, file_name_len);

        MMI_WSTRNCPY(full_path_ptr, *full_path_len_ptr,
                                file_name, file_name_len, *full_path_len_ptr);
    }

    if (PNULL != temp_file_array)
    {
        MMIAPIFILEARRAY_Destroy(&temp_file_array);
    }
    
    return TRUE;
} 


/*****************************************************************************/
//  Description : BookMark Initialize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_BookmarkGetRootPath(
                    wchar           *full_path_ptr,     //out
                    uint16           *full_path_len_ptr,  //in/out, 双字节为单位
                    MMIFILE_DEVICE_E *dev_ptr
                    )
{
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    BOOLEAN result = FALSE;
    const wchar    *path_name_ptr = (wchar*)MMIBRW_BROWSER_ROOT_PATH;
    uint16  path_name_len = MMIBRW_BROWSER_ROOT_PATH_LEN;                                     

#ifdef NANDBOOT_SUPPORT
    dev = MMI_DEVICE_UDISK;
#else
    dev = MMIAPIFMM_GetFirstValidSD();

    if (MMI_DEVICE_NUM > dev)
#endif
    {
        wchar   *dst_path_name_ptr = PNULL;
        uint16  dst_path_name_len = 0;
        wchar   syspath_name_ptr[] = {MMIMULTIM_DIR_SYSTEM_CHAR, '\\'};
        uint16  syspath_name_len = (MMIMULTIM_DIR_SYSTEM_LEN+1);

        dst_path_name_len = (path_name_len + syspath_name_len);
        dst_path_name_ptr = (wchar *)SCI_ALLOCAZ(sizeof(wchar) * (dst_path_name_len+1));
        if (dst_path_name_ptr != PNULL)
        {
            MMI_WSTRNCPY(dst_path_name_ptr, dst_path_name_len, syspath_name_ptr, syspath_name_len, syspath_name_len);
            MMI_WSTRNCPY(dst_path_name_ptr+syspath_name_len, path_name_len, path_name_ptr, path_name_len, path_name_len);

            result = MMIAPIFMM_CombineFullPath(MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev), dst_path_name_ptr, dst_path_name_len, PNULL, 0, full_path_ptr, full_path_len_ptr);   
        
            SCI_FREE(dst_path_name_ptr);    
        }
        else
        {
            //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_BookmarkGetRootPath:dst_path_name_ptr alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_2597_112_2_18_2_5_42_40,(uint8*)"");
        }
    }

    if (result)
    {
        if (PNULL != dev_ptr)
        {
            *dev_ptr = dev;
        }
    }

    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_BookmarkGetRootPath result =%d, dev =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_2611_112_2_18_2_5_42_41,(uint8*)"dd", result, dev);
    return result;
}

/*****************************************************************************/
//  Description : handle plug out sd card for browser bookmark
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_BKHandleSDCardPlugOut(void)
{
    if ((MMI_DEVICE_UDISK < s_browser_default_dev) || (MMI_DEVICE_NUM > s_browser_default_dev))
    {
        if (!MMIAPIFMM_GetDeviceTypeStatus(s_browser_default_dev))
        {
            //mean: the storage of bookmar is invalid
            s_browser_default_dev = MMI_DEVICE_NUM;//reset
            MMIBROWSER_SetBookmarkLoaded(FALSE);
        }
    }
}
/*****************************************************************************/
//  Description : handle browser when format the storage 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_BKHandleStorageFormat(void)
{
#if 0
    if ((MMI_DEVICE_UDISK < s_browser_default_dev) || (MMI_DEVICE_NUM > s_browser_default_dev))
    {
        if (dev_type == s_browser_default_dev)
        {
            //update the num info
            s_bookmark_info_ptr->cur_folder_num = 0;
            s_bookmark_info_ptr->total = MMIBRW_UpdateBookmarkTotalNum();
        }
    }
#endif
#ifdef MMI_BROWSER_MINI_SUPPORT
    if(PNULL == s_bookmark_info_ptr)
    {
        MMIBRW_BookmarkInit();
    }
#endif
    //update the num info
    s_bookmark_info_ptr->cur_folder_num = 0;
    s_bookmark_info_ptr->total = MMIBRW_UpdateBookmarkTotalNum();
}
/*****************************************************************************/
// Description : after load the file array for bookmark, update the bookmark info
// Global resource dependence :s_load_fmm_func_type
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_LoadFinish(void)
{
    uint16 num = 0;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    uint16 folder_num = 0;

    //采用同步方式
    MMIAPIFILEARRAY_Sort(bookmark_info_ptr->file_array, FILEARRAY_SORT_TYPE_ASCEND);
    num = (uint32)MMIAPIFILEARRAY_GetArraySize(bookmark_info_ptr->file_array);
    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_LoadFinish num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_2666_112_2_18_2_5_43_42,(uint8*)"d", num);

    if ((0 == bookmark_info_ptr->file_array_step) && (FUNC_FIND_FILE_AND_FOLDER == s_load_fmm_func_type))//MS00250307
    {
        FILEARRAY folder_file_array = PNULL;

        folder_file_array = MMIAPIFILEARRAY_Create();
        MMIAPIFMM_SearchFileInPath(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len,
            &s_bookmark_filter,
            FALSE,
            FUNC_FIND_FOLDER,
            folder_file_array,
            0,
            0);
        folder_num = (uint32)MMIAPIFILEARRAY_GetArraySize(folder_file_array);
        MMIAPIFILEARRAY_Destroy(&folder_file_array);
        
        //SCI_TRACE_LOW:"[BRW]MMIBROWSER_LoadFinish folder_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_BOOKMARK_2683_112_2_18_2_5_43_43,(uint8*)"d", folder_num);
        
        folder_num = MIN(MMIBRW_BOOKMARK_DIR_MAX_NUM, folder_num);//protect
        if (folder_num > num)//protect
        {
            folder_num = num;
        }
        bookmark_info_ptr->cur_folder_num = folder_num;
    }

    if ((num - folder_num) >= MMIBRW_BOOKMARK_MAX_ITEM)
    {
        num = (MMIBRW_BOOKMARK_MAX_ITEM + folder_num);
    }

    bookmark_info_ptr->cur_index_num = num;

    s_load_fmm_func_type = FUNC_FIND_NULL;
    return TRUE;
}

/*****************************************************************************/
// Description : find the array index of file array  by filename
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL int32 BrwFindArrayIndexByFilename(
                    FILEARRAY   file_array,
                    wchar *full_file_name_ptr,
                    uint16 full_file_name_len,
                    VIRTUALARRAY_TYPE_E type
                    )
{
    uint32 total_array_size = 0;
    uint32 bm_index = 0;
    uint32 i = 0;
    FILEARRAY_DATA_T f_array_data = {0};

    if ((PNULL == file_array) || (PNULL == full_file_name_ptr) || (0 == full_file_name_len))
    {
        return -1;//error
    }
    
    total_array_size = MMIAPIFILEARRAY_GetArraySize(file_array);
    for (i = 0; i < total_array_size; i++)
    {
        SCI_MEMSET(&f_array_data, 0x00, sizeof(f_array_data));
        MMIAPIFILEARRAY_Read(file_array, i, &f_array_data);                    
        if (type == f_array_data.type)
        {
            //MMIAPICOM_Wstrlower(f_array_data.filename);
            if (0 == MMIAPICOM_Wstrcmp(f_array_data.filename, full_file_name_ptr))
            {
                bm_index = i;
                break;
            }
        }
    }
        
    return bm_index;
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
// Description : Find duplicated Bookmark
// Global resource dependence :
// Author: fen.xie
// Note: negative number  means there is no duplicated bookmark
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_IsBKFolderExist(wchar *cur_path_ptr, 
                    uint16 cur_path_len, 
                    wchar *new_name_ptr,
                    uint16 new_name_len
                    )
{
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;

    MMIBRW_BookmarkCombinePath(
        cur_path_ptr,cur_path_len,
        new_name_ptr, new_name_len,
        PNULL, PNULL,
        file_name, &file_name_len
        );

    return MMIAPIFMM_IsFolderExist(file_name, file_name_len);
    
}
#endif /* Reduce Code size */

/*****************************************************************************/
// Description : Changes the title and url of specified bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_ChangeBookmarkTitleAndUrl(uint16 in_index, 
                MMI_STRING_T in_title_str,
                MMI_STRING_T in_url_str, 
                uint16 *out_index_ptr)
{
    FILEARRAY_DATA_T array_item = {0};
    wchar *dest_wstr_ptr = PNULL;
    uint32 dest_wstr_byte_len = 0;
    MMIBRW_BOOKMARK_T *bookmark_info_ptr = MMIBRW_GetBookmark();
    wchar           new_full_path[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16          new_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMI_STRING_T string_suffix ={0};
    MMI_STRING_T string_dest ={0};
    BOOLEAN result =FALSE;

    if ((PNULL == bookmark_info_ptr->file_array) || (PNULL == in_title_str.wstr_ptr))
    {
        return FALSE;
    }

    if (PNULL != out_index_ptr)
    {
        *out_index_ptr = in_index;//the time isn't modify, so the index will not be modifide
    }

    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    if (MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
    {
            
        if (FILEARRAY_TYPE_FILE == array_item.type)
        {
            string_suffix.wstr_ptr = (wchar*)s_dorado_bk_file_suffix;
            string_suffix.wstr_len = MMIAPICOM_Wstrlen(s_dorado_bk_file_suffix);
         
            string_dest.wstr_len = in_title_str.wstr_len + string_suffix.wstr_len;
            dest_wstr_byte_len = (string_dest.wstr_len + 1)*sizeof(wchar);
            
            dest_wstr_ptr = SCI_ALLOCA(dest_wstr_byte_len);
            if (PNULL == dest_wstr_ptr)
            {
                return FALSE;
            }
            SCI_MEMSET(dest_wstr_ptr, 0x00, dest_wstr_byte_len);
            
            string_dest.wstr_ptr = dest_wstr_ptr;
            
            MMIAPICOM_MergeTwoString(&in_title_str, &string_suffix, &string_dest, string_dest.wstr_len);
            
            result = BrwRenameFile(array_item.filename, array_item.name_len,
                        string_dest.wstr_ptr, string_dest.wstr_len,
                        new_full_path, &new_full_path_len);
            
            SCI_FREE(dest_wstr_ptr);
        }
        else if (MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
        {
            
           result = BrwRenameFile(array_item.filename, array_item.name_len,
                    in_title_str.wstr_ptr, in_title_str.wstr_len,
                    new_full_path, &new_full_path_len);
        }
    }
    else
    {
        return FALSE;
    }
     
    //rename success, update the filearray
    if (result)
    {
        FILEARRAY_DATA_T new_array_item = {0};

        SCI_MEMCPY(&new_array_item, &array_item, sizeof(FILEARRAY_DATA_T));
        
        SCI_MEMSET(new_array_item.filename, 0, sizeof(new_array_item.filename));
        new_array_item.name_len = MIN(MMIFILE_FULL_PATH_MAX_LEN, new_full_path_len);
        MMI_WSTRNCPY(new_array_item.filename, MMIFILE_FULL_PATH_MAX_LEN, new_full_path, new_full_path_len, new_array_item.name_len);
        
        result = MMIAPIFILEARRAY_Modify(bookmark_info_ptr->file_array, (uint32)in_index, &new_array_item);
        if (result)//begin to change url
        {
            result = MMIBRW_ChangeBookmarkUrl(in_index, in_url_str, out_index_ptr);
        }
        
        if (!result)
        {
            SFS_RenameFile(new_full_path,  array_item.filename, PNULL);//update fail ,so rename back
        }
    }

    return result;
}


/*****************************************************************************/
// Description : get the file suffix name of bookmark
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC const wchar* MMIBRW_GetBkSuffixName(void)
{
    return s_dorado_bk_file_suffix + 1;
}

#else

/*****************************************************************************/
//  Description : Set default bookmark to s_bookmark_info_ptr
//  Global resource dependence : none
//  Author: fen.xie
//  Note:s_bookmark_info_ptr
/*****************************************************************************/
LOCAL void SetDefaultBookmark(void)
{
#ifdef IM_SIMP_CHINESE_SUPPORT//just for chinese for temp 
    //int32 arrary_num = 0;
    int32 i = 0;
    BOOLEAN is_res_data_exist = FALSE;
    MMIRES_BOOKMARK_INFO_T res_info = {0};

    is_res_data_exist = MMIRES_LoadBookmarkFromRes(&res_info);

    if (is_res_data_exist && PNULL != res_info.res_item_ptr)
    {
        res_info.res_num = MIN(res_info.res_num, MMIBRW_BOOKMARK_MAX_ITEM);
        
#ifdef MMI_BROWSER_MINI_SUPPORT
        if(PNULL == s_bookmark_info_ptr)
        {
            MMIBRW_BookmarkInit();
        }
#endif
        for (i = 0; i < res_info.res_num; i++)
        {
            MMIBRW_BOOKMARK_ITEM_T  *item_info_ptr = &(s_bookmark_info_ptr->item_info[i]);
            
            item_info_ptr->title_len = MIN(res_info.res_item_ptr[i].name_len,MMIBRW_BOOKMARK_MAX_TITLE_LENGTH);
            MMI_WSTRNCPY(item_info_ptr->title_arr, MMIBRW_BOOKMARK_MAX_TITLE_LENGTH,
                res_info.res_item_ptr[i].name, res_info.res_item_ptr[i].name_len,
                res_info.res_item_ptr[i].name_len);
            
            SCI_MEMCPY(item_info_ptr->url_ptr, res_info.res_item_ptr[i].url, MIN(MMIBRW_BOOKMARK_MAX_URL_LENGTH, res_info.res_item_ptr[i].url_len));
        }
        s_bookmark_info_ptr->item_count = i;
        
        /*load bookmark success,remember it*/
        MMIRES_UnloadBookmarkInfo(&res_info);
    }
#else
    if (s_bookmark_info_ptr != PNULL)
    {
        SCI_MEMSET(s_bookmark_info_ptr, 0, sizeof(MMIBRW_BOOKMARK_T));
    }
#endif
}

LOCAL void BookmarkReadInfo(BOOLEAN is_force)
{
    MN_RETURN_RESULT_E result = MN_RETURN_FAILURE;

    if(PNULL == s_bookmark_info_ptr)
    {
        return;
    }

    if (!is_force)
    {
        MMINV_READ(MMINV_BROWSER_BOOKMARK_INFO, s_bookmark_info_ptr, result);
    }
    
    if (MN_RETURN_SUCCESS != result )
    {
        SetDefaultBookmark();
        MMINV_WRITE(MMINV_BROWSER_BOOKMARK_INFO, s_bookmark_info_ptr);
    }
}

LOCAL void BookmarkOverwriteInfo(void)
{
    if (s_bookmark_info_ptr != PNULL)
    {
        MMINV_WRITE(MMINV_BROWSER_BOOKMARK_INFO, s_bookmark_info_ptr);
    }
}

/****************************************************************************/
//Description : bookmark initiation
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC void MMIBRW_BookmarkInit(void)
{
    if (PNULL == s_bookmark_info_ptr)
    {
        s_bookmark_info_ptr = MMIBRW_ALLOC(sizeof(MMIBRW_BOOKMARK_T));
    }
    
    if (s_bookmark_info_ptr != PNULL)
    {
        SCI_MEMSET(s_bookmark_info_ptr, 0, sizeof(MMIBRW_BOOKMARK_T));
        BookmarkReadInfo(FALSE);
    }
    else
    {
        SCI_TRACE_LOW("[BRW]MMIBRW_BookmarkInit ALLOC FAIL");
    }
}

/****************************************************************************/
//Description : Get the indicated index node info 
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC MMIBRW_BOOKMARK_ITEM_T* MMIBRW_BookmarkGetItem(uint32 item_index)
{
#ifdef MMI_BROWSER_MINI_SUPPORT
        if(PNULL == s_bookmark_info_ptr)
        {
            MMIBRW_BookmarkInit();
        }
#endif
    if (item_index < s_bookmark_info_ptr->item_count)
    {
        return &s_bookmark_info_ptr->item_info[item_index];
    }
    else
    {
        SCI_TRACE_LOW("[BRW]MMIBRW_BookmarkGetItem ERROR:index=%d, icount=%d", item_index, s_bookmark_info_ptr->item_count);
        return PNULL;
    }
}

/****************************************************************************/
//Description : To add a bookmark item
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkAddItem(
                MMIBRW_BOOKMARK_ITEM_T	*item_info_ptr
                )
{
#ifdef MMI_BROWSER_MINI_SUPPORT
    if(PNULL == s_bookmark_info_ptr)
    {
        MMIBRW_BookmarkInit();
    }
#endif
    if ((PNULL == item_info_ptr) || (s_bookmark_info_ptr->item_count >= MMIBRW_BOOKMARK_MAX_ITEM))
    {
        return FALSE;
    }

    SCI_MEMCPY(
            &s_bookmark_info_ptr->item_info[s_bookmark_info_ptr->item_count],
            item_info_ptr,
            sizeof(MMIBRW_BOOKMARK_ITEM_T)
            );
    s_bookmark_info_ptr->item_count++;
    BookmarkOverwriteInfo();
    return TRUE;
}

/****************************************************************************/
//Description : To update the bookmark info for the indecated node
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkUpdateItem(
                uint32					item_index,
                MMIBRW_BOOKMARK_ITEM_T *item_info_ptr
                )
{
#ifdef MMI_BROWSER_MINI_SUPPORT
    if(PNULL == s_bookmark_info_ptr)
    {
        MMIBRW_BookmarkInit();
    }
#endif
    if ((item_index >= s_bookmark_info_ptr->item_count) || (PNULL == item_info_ptr))
    {
        return FALSE;
    }
    SCI_MEMCPY(
            &s_bookmark_info_ptr->item_info[item_index],
            item_info_ptr,
            sizeof(MMIBRW_BOOKMARK_ITEM_T)
            );
    BookmarkOverwriteInfo();
    return TRUE;
}

#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


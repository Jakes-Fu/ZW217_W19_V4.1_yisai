/****************************************************************************
** File Name:      MMIWWW_BOOKMARK.c                                            *
** Author:                                                                    *
** Date:            06/09/2010                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the netfront for WWW Browser            *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 06/2010        fen.xie        Create
** 
****************************************************************************/
#define _MMIWWW_BOOKMARK_C_  

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_www_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmi_theme.h"
#include "mmiwww_export.h"
#include "mmiwww_internal.h"
#include "mmiwww_netfront.h"
#include "mmi_position.h"
#include "gui_ucs2b_converter.h"
#include "mmi_nv.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiwww_func.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmifmm_export.h"

#include "mmiwww_bookmark.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                          EXTERN DECLARE                                *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMIFMM_FILTER_T s_bookmark_filter = {{"*.url"}};
const wchar g_bookmark_file_suffix[]   = {'.' , 'u', 'r', 'l', 0 };
LOCAL const wchar s_bookmark_url_flag[] = { '[', 'I', 'n', 't', 'e','r','n','e', 't', 'S', 'h', 'o', 'r', 't', 'c', 'u', 't',']', 0x0D, 0x0A, 'U', 'R','L', '=', 0};//"[InternetShortcut]\x0d\x0aURL=";;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : BookMark Initialize
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void InitializeBookmarkFileArray(void);

/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RenameFile(
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
LOCAL uint32 GetBookmarkUrl(uint16 in_index, //IN
                            uint8 **out_url_ptr//IN/OUT:need free outside
                            );

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RenameFile(
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
        //SCI_TRACE_LOW:"RenameFile full_path_len %d new_file_name_len %d !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_159_112_2_18_3_11_29_27,(uint8*)"dd", full_path_len, new_file_name_len);
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
//  Description : rename file
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_Bookmark_SplitPath(
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
        || (PNULL == pre_path_ptr)
        || (PNULL == pre_path_len_ptr)
        || (0 == *pre_path_len_ptr || MMIFILE_FULL_PATH_MAX_LEN < *pre_path_len_ptr)
        || (PNULL == file_name_ptr )
        || (PNULL == file_name_len_ptr )
        || (0 == *file_name_len_ptr || MMIFILE_FULL_PATH_MAX_LEN < *file_name_len_ptr)
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

/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_BookmarkCombinePath(
                                     wchar           *path_ptr,      //in/out:may PNULL
                                     uint16          path_len,  //in/out:may PNULL,path_ptr不为空时,此处必须填max len
                                     wchar           *name_ptr,      //in/out:may PNULL,don't include suffix
                                     uint16          name_len,  //in/out:may PNULL,name_ptr不为空时,此处必须填max len
                                     wchar           *suffix_ptr,    //in/out:may PNULL,include dot
                                     uint16          suffix_len, //in/out:may PNULL,suffix_ptr不为空时,此处必须填max len
                                     wchar     *full_path_ptr, //out
                                     uint16          *full_path_len_ptr  //out
                                     )
{
    BOOLEAN         result = FALSE;
    wchar      *wchar_ptr = PNULL;
    uint16  temp_len = 0;

    if (PNULL == path_ptr || PNULL == name_ptr || 0 == path_len || 0 == name_len || PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        return result;
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
        
    return result;
}

/*****************************************************************************/
// Description : Gets Url (UTF8)
// Global resource dependence :
// Author: fen.xie
// Note: need free "out_url_ptr" outside
/*****************************************************************************/
LOCAL uint32 GetBookmarkUrl(uint16 in_index, //IN
                            uint8 **out_url_ptr//IN/OUT:need free outside
                            )
{
    FILEARRAY_DATA_T array_item = {0};
    uint8 * buf_ptr = PNULL;
    uint32 url_len = 0;
    uint8 * url_buf_ptr = PNULL;
    uint8 * temp_buf_ptr = PNULL;
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();

    if (PNULL == out_url_ptr)
    {
        return 0;
    }

    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
    {
        buf_ptr = SCI_ALLOCAZ(array_item.size);/*size需要限制大小? 是否需考虑到异步?*/
        if (PNULL == buf_ptr)
        {
            return 0;
        }
        //SCI_MEMSET(buf_ptr, 0x00, array_item.size);
        
        if(MMIAPIFMM_ReadFilesDataSyn((const wchar *)array_item.filename, array_item.name_len, buf_ptr, array_item.size))
        {
            temp_buf_ptr = MMIAPIBROWSER_GetUrlByFile(buf_ptr, array_item.size, &url_len);
            if ((0 != url_len) && (PNULL != temp_buf_ptr))
            {
                url_buf_ptr = SCI_ALLOC_APP(url_len + 1);
                if (PNULL == url_buf_ptr)
                {
                    SCI_FREE(buf_ptr);
                    return 0;
                }
                SCI_MEMSET(url_buf_ptr, 0x00, (url_len + 1));
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
/*****************************************************************************/
// Description : update the total bookmark number
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_UpdateBookmarkTotalNum(void)
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

    slimFile_CombineFullPath((slim_char *)BRSCFG_FAVORITES, full_path_name, &full_path_name_len);

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
    if (total_file_num < MMIWWW_MAX_BOOKMARK_NUM)
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

        root_folder_num = (root_folder_num < MMIWWW_MAX_BOOKMARK_DIR_NUM)? root_folder_num: MMIWWW_MAX_BOOKMARK_DIR_NUM;
        
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
            
            if (total_file_num >= MMIWWW_MAX_BOOKMARK_NUM)
            {
                total_file_num = MMIWWW_MAX_BOOKMARK_NUM;
                break;
            }

        }
        MMIAPIFILEARRAY_Destroy(&file_array);
    
    }
    else
    {
        total_file_num = MMIWWW_MAX_BOOKMARK_NUM;
    }
    //SCI_TRACE_LOW:"MMIWWW_UpdateBookmarkTotalNum bookmark_total_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_553_112_2_18_3_11_30_28,(uint8*)"d", total_file_num);

    return total_file_num;
}



/*****************************************************************************/
//  Description : BookMark Initialize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_Bookmark_Initialize(MMIWWW_INSTANCE_T *self)
{
    uint32 file_num = 0;
    wchar *path_ptr = self->bookmark_info.full_path;
    uint16 *path_len_ptr = &(self->bookmark_info.full_path_len);
    
    SCI_MEMSET(&(self->bookmark_info), 0x00, sizeof((self->bookmark_info)));
    *path_len_ptr = MMIFILE_FULL_PATH_MAX_LEN;
    slimFile_CombineFullPath((slim_char *)BRSCFG_FAVORITES, path_ptr, path_len_ptr);

    self->bookmark_info.total = MMIWWW_UpdateBookmarkTotalNum();
    self->bookmark_info.file_array_step = 0;
    
    //SCI_TRACE_LOW:"MMIWWW_Bookmark_Initialize bookmark_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_577_112_2_18_3_11_30_29,(uint8*)"d", file_num);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : BookMark finalize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Bookmark_finalize(void)
{
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
    if(PNULL != bookmark_info_ptr->file_array)
    {
        MMIAPIFILEARRAY_Destroy(&(bookmark_info_ptr->file_array));
    }

    SCI_MEMSET(bookmark_info_ptr, 0x00, sizeof(WWW_BOOKMARK_T));
}
/*****************************************************************************/
// Description : Gets title of specified title entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC FILEARRAY_TYPE_E MMIWWW_GetBookmarkTitle(
                                            uint16 in_index, //in
                                            MMI_STRING_T *out_title//in/out:out_title->wstr_ptr need free outside
                                            )
{
    FILEARRAY_DATA_T array_item = {0};
    wchar *wstr_ptr = PNULL;
    uint32 wstr_byte_len = 0;
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
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

/*****************************************************************************/
// Description : Gets Url of specified title entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetBookmarkUrl(
                            uint16 in_index, //in
                            MMI_STRING_T *out_url//in/out:out_url->wstr_ptr need free outside
                            )
{
    FILEARRAY_DATA_T array_item = {0};
    wchar *wstr_ptr = PNULL;
    uint8 * buf_ptr = PNULL;
    uint32 url_len = 0;
    uint8 * url_buf_ptr = PNULL;
    BOOLEAN result = FALSE;
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();

    if (PNULL == out_url)
    {
        return FALSE;
    }
    
    SCI_MEMSET(out_url, 0x00, sizeof(MMI_STRING_T));
    
    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
    {
        buf_ptr = SCI_ALLOCAZ(array_item.size + 1);/*size需要限制大小? 是否需考虑到异步?*/
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
}

/*****************************************************************************/
// Description : load new file array(load 根目录或cur_array_index对应的文件夹的file)
// Global resource dependence :
// Author: xie.fen
// Note: fmm_func_type :1 FUNC_FIND_NULL表示使用默认的搜索方式
//          root:FUNC_FIND_FILE_AND_FOLDER  other:FUNC_FIND_FILE
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_LoadBookmarkFileArray(
                        WWW_BOOKMARK_STEP_TYPE_E step_type,//in
                        uint16 cur_array_index,//in:load 根目录时无效
                        MMIFMM_FUNC_TYPE_E   fmm_func_type//in
                        )
{
    uint16 num = 0;
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
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
        slimFile_CombineFullPath((slim_char *)BRSCFG_FAVORITES, bookmark_info_ptr->full_path, &(bookmark_info_ptr->full_path_len));
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

    MMIAPIFMM_SearchFileInPath(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len,
            &s_bookmark_filter,
            FALSE,
            fmm_func_type,
            bookmark_info_ptr->file_array,
            0,
            0);

    MMIAPIFILEARRAY_Sort(bookmark_info_ptr->file_array, FILEARRAY_SORT_TYPE_ASCEND);

//    MMIAPIFILEARRAY_Sort(bookmark_info_ptr->file_array, FILEARRAY_SORT_TIME_DESCEND);
    
    num = (uint32)MMIAPIFILEARRAY_GetArraySize(bookmark_info_ptr->file_array);

    if (1 == bookmark_info_ptr->file_array_step)//非根目录下,需增加额外的"..." 项,用于返回上一级
    {
        num += 1;
    }
    if (num >= MMIWWW_MAX_BOOKMARK_NUM)
    {
        num = MMIWWW_MAX_BOOKMARK_NUM;
    }

    bookmark_info_ptr->cur_index_num = num;

    return TRUE;
}

/*****************************************************************************/
// Description : reload file array
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_ReloadBookmarkFileArray(
                        const wchar *path_ptr,
                        uint16 path_len,
                        MMIFMM_FUNC_TYPE_E fmm_func_type
                        )
{
    uint16 num = 0;
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();

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

    if (1 == bookmark_info_ptr->file_array_step)//非根目录下,需增加额外的"..." 项,用于返回上一级
    {
        num += 1;
    }
    if (num >= MMIWWW_MAX_BOOKMARK_NUM)
    {
        num = MMIWWW_MAX_BOOKMARK_NUM;
    }

    bookmark_info_ptr->cur_index_num = num;

    return TRUE;
}

/*****************************************************************************/
// Description : get the total bookmark number
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWWW_GetBookmarkTotalNumber(void)
{
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();

    return bookmark_info_ptr->total;
}

/*****************************************************************************/
// Description : get index num of current bookmark path(include the "..." index)
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetBookmarkNumber(void)
{
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();

    return bookmark_info_ptr->cur_index_num;
}

/*****************************************************************************/
// Description : Adds a new bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_AddBookmark (MMI_STRING_T in_url_str, //IN
                                            MMI_STRING_T in_title_str, //IN
                                            BOOLEAN in_protect, //IN
                                            uint16 *out_index//OUT:new index
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
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
    MMI_STRING_T string_1 ={0};
    MMI_STRING_T string_dest ={0};
    uint32 dest_wstr_byte_len = 0;
    SFS_DATE_T  modify_date = {0};
    SFS_TIME_T  modify_time = {0};

    //InitializeBookmarkFileArray();//the default path of new bookmark file is root

    //create new bookmark file's name
    MMIWWW_BookmarkCombinePath(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len,
        in_title_str.wstr_ptr, in_title_str.wstr_len,
        (wchar*)g_bookmark_file_suffix + 1, MMIAPICOM_Wstrlen(g_bookmark_file_suffix) -1, 
        file_name, &file_name_len);

    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_CREATE_NEW, 0, 0);

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
            return FALSE;
        }
        SCI_MEMSET(string_dest.wstr_ptr, 0x00, dest_wstr_byte_len);
                
        MMIAPICOM_MergeTwoString( &string_1,&in_url_str, &string_dest, string_dest.wstr_len);
        
        bytes_to_write = string_dest.wstr_len;//不允许多余数据写入文件中
        url_buf = SCI_ALLOCA(bytes_to_write + 1);
        if (PNULL == url_buf)
        {
            SCI_FREE(string_dest.wstr_ptr);
            return FALSE;
        }
        SCI_MEMSET(url_buf, 0x00, (bytes_to_write + 1));
        
        MMIAPICOM_WstrToStr(string_dest.wstr_ptr, (uint8 *)url_buf);
        
        SCI_FREE(string_dest.wstr_ptr);
        
        if (SFS_ERROR_NONE == MMIAPIFMM_WriteFile(file_handle, url_buf, bytes_to_write, &bytes_written, PNULL))
        {
            result = TRUE;
        }
        SCI_FREE(url_buf);
        url_buf = PNULL;

        MMIAPIFMM_CloseFile(file_handle);
    }
    else
    {
        return FALSE;
    }

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
    
    MMIAPIFILEARRAY_Add(bookmark_info_ptr->file_array, &array_item);

    bookmark_info_ptr->cur_index_num++;
    bookmark_info_ptr->total++;
    
    *out_index = bookmark_info_ptr->cur_index_num-1;
    return result;
}

/*****************************************************************************/
// Description : Adds a new bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_AddBookmarkFolder (MMI_STRING_T in_title_str, slim_bool in_protect, uint16 *out_index)
{
    FILEARRAY_DATA_T array_item = {0};
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN result = FALSE;
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();

    //InitializeBookmarkFileArray();

    if ((PNULL == out_index) || (PNULL == bookmark_info_ptr->file_array))//do nothing
    {
        return result;
    }
    
    //create new bookmark file's name
    MMIWWW_BookmarkCombinePath(bookmark_info_ptr->full_path, bookmark_info_ptr->full_path_len,
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
        
        MMIAPIFILEARRAY_Add(bookmark_info_ptr->file_array, &array_item);
       
        bookmark_info_ptr->cur_index_num++;

        *out_index = bookmark_info_ptr->cur_index_num -1;

        result = TRUE;
    }
    
    return result;
}


/*****************************************************************************/
// Description : Changes the title of specified bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_ChangeBookmarkTitle(uint16 in_index, MMI_STRING_T in_title_str, uint16 *out_index)
{
    FILEARRAY_DATA_T array_item = {0};
    wchar *dest_wstr_ptr = PNULL;
    uint32 dest_wstr_byte_len = 0;
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
    wchar           new_full_path[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16          new_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMI_STRING_T string_suffix ={0};
    MMI_STRING_T string_dest ={0};

    if (PNULL == in_title_str.wstr_ptr || PNULL == out_index)
    {
        return 0;
    }
    
    *out_index = in_index;

    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    if (MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
    {
            
        if (FILEARRAY_TYPE_FILE == array_item.type)
        {
            string_suffix.wstr_ptr = (wchar*)g_bookmark_file_suffix;
            string_suffix.wstr_len = MMIAPICOM_Wstrlen(g_bookmark_file_suffix);
         
            string_dest.wstr_len = in_title_str.wstr_len + string_suffix.wstr_len;
            dest_wstr_byte_len = (string_dest.wstr_len + 1)*sizeof(wchar);
            
            dest_wstr_ptr = SCI_ALLOCA(dest_wstr_byte_len);
            if (PNULL == dest_wstr_ptr)
            {
                return 0;
            }
            SCI_MEMSET(dest_wstr_ptr, 0x00, dest_wstr_byte_len);
            
            string_dest.wstr_ptr = dest_wstr_ptr;
            
            MMIAPICOM_MergeTwoString(&in_title_str, &string_suffix, &string_dest, string_dest.wstr_len);
            
            RenameFile(array_item.filename, array_item.name_len,
                        string_dest.wstr_ptr, string_dest.wstr_len,
                        new_full_path, &new_full_path_len);
            
            SCI_FREE(dest_wstr_ptr);
        }
        else if (MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
        {
            
           RenameFile(array_item.filename, array_item.name_len,
                    in_title_str.wstr_ptr, in_title_str.wstr_len,
                    new_full_path, &new_full_path_len);
        }
    }
    else
    {
        return 0;
    }
     
    //update the filearray
    SCI_MEMSET(array_item.filename, 0, sizeof(array_item.filename));
    MMI_WSTRNCPY(array_item.filename, FILEARRAY_FILENAME_MAX_LEN, new_full_path, new_full_path_len, new_full_path_len );
    array_item.name_len = new_full_path_len;

    return MMIAPIFILEARRAY_Modify(bookmark_info_ptr->file_array, (uint32)in_index, &array_item);
}

/*****************************************************************************/
// Description : Changes the url of specified bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_ChangeBookmarkUrl(uint16 in_index, MMI_STRING_T in_url_str)
{
    FILEARRAY_DATA_T array_item = {0};
    uint8 *url_buf = PNULL;
    uint32 bytes_to_write = 0;
    BOOLEAN result = FALSE;
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
    MMI_STRING_T string_prefix ={0};
    MMI_STRING_T string_dest ={0};
    wchar *dest_wstr_ptr = PNULL;
    uint32 dest_wstr_byte_len = 0;
    SFS_DATE_T  modify_date = {0};
    SFS_TIME_T  modify_time = {0};

    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    //SCI_TRACE_LOW:"MMIWWW_ChangeBookmarkUrl in_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1157_112_2_18_3_11_31_30,(uint8*)"d", in_index);

    if(MMIAPIFILEARRAY_Read(bookmark_info_ptr->file_array, (uint32)in_index, &array_item ))
    {
        string_prefix.wstr_ptr = (wchar*)s_bookmark_url_flag;
        string_prefix.wstr_len = MMIAPICOM_Wstrlen(s_bookmark_url_flag);
     
        string_dest.wstr_len = in_url_str.wstr_len + string_prefix.wstr_len;
        dest_wstr_byte_len = (string_dest.wstr_len + 1)*sizeof(wchar);
        
        dest_wstr_ptr = SCI_ALLOCA(dest_wstr_byte_len);
        if (PNULL == dest_wstr_ptr)
        {
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
            return FALSE;
        }
        SCI_MEMSET(url_buf, 0x00, (bytes_to_write + 1));
        MMIAPICOM_WstrToStr(string_dest.wstr_ptr, (uint8 *)url_buf);
        //SCI_TRACE_LOW:"MMIWWW_ChangeBookmarkUrl buf=%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1187_112_2_18_3_11_31_31,(uint8*)"s", url_buf + string_prefix.wstr_len);
        SCI_FREE(dest_wstr_ptr);

        if (MMIAPIFMM_WriteFilesDataSyn(array_item.filename, array_item.name_len, url_buf, bytes_to_write))
        {
            result = TRUE;
            //SCI_TRACE_LOW:"MMIWWW_ChangeBookmarkUrl result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1193_112_2_18_3_11_31_32,(uint8*)"d", result);
        }
        SCI_FREE(url_buf);
        url_buf = PNULL;
    }

    //update
    MMIAPIFMM_GetFileInfo(array_item.filename, array_item.name_len, &(array_item.size), &modify_date, &modify_time);
    array_item.modify_date.mday = modify_date.mday;
    array_item.modify_date.mon = modify_date.mon;
    array_item.modify_date.year = modify_date.year;
    
    array_item.modify_time.sec = modify_time.sec;
    array_item.modify_time.min = modify_time.min;
    array_item.modify_time.hour = modify_time.hour;

    return MMIAPIFILEARRAY_Modify(bookmark_info_ptr->file_array, (uint32)in_index, &array_item);
}

/*****************************************************************************/
// Description : Deletes bookmark entry specified by in_index
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_RemoveBookmark(uint16 in_index , BOOLEAN is_for_delete_all)
{
    FILEARRAY_DATA_T array_item = {0};
    BOOLEAN result = FALSE;
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
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
                bookmark_info_ptr->cur_index_num--;
                bookmark_info_ptr->total--;
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
                bookmark_info_ptr->cur_index_num--;
                bookmark_info_ptr->total -= files_num;
            }
            break;
            
        default:
            break;
        }

    }        

    return result;
}

/*****************************************************************************/
// Description : Deletes all bookmark include file and folder,but only in root
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_RemoveAllBookmark(BOOLEAN in_force)
{
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
    int32 index = 0;
    BOOLEAN result = FALSE;
    uint16 total_cur_num = bookmark_info_ptr->cur_index_num;

    for (index = 0; index < total_cur_num; index++)//注意:只用于root，故cur_index_num此时就是root下的文件总数
    {
        result = MMIWWW_RemoveBookmark(index, TRUE);       
    }

    MMIWWW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL);
    //bookmark_info_ptr->total = 0;

    return result;
}

/*****************************************************************************/
// Description : Deletes all bookmark in folder,but do Not delete folder
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_RemoveAllBookmarkOfSubFolder(void)
{
    BOOLEAN ret = FALSE;
    wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar folder[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
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

    MMIWWW_ReloadBookmarkFileArray(bookmark_info_ptr->full_path, path_len, FUNC_FIND_FILE);

    return ret;
}

/*****************************************************************************/
// Description : Access network by bookmark
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Bookmark_GotoURL(uint16 in_index)
{
    char* url_ptr = PNULL;
    uint32 url_len = 0;

    url_len = GetBookmarkUrl(in_index, (uint8 **)&url_ptr);
    if (url_len > 0)
    {
        MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, (char *)url_ptr);
    }

    //SCI_TRACE_LOW:"MMIWWW_Bookmark_GotoURL: url_len=%d, url_ptr= %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1370_112_2_18_3_11_32_33,(uint8*)"ds",url_len,url_ptr);

    if (url_ptr != PNULL)
    {
        SCI_FREE(url_ptr);
    }
}
/*****************************************************************************/
//  Description : BookMark Initialize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void InitializeBookmarkFileArray(void)
{
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();

    if (PNULL == bookmark_info_ptr->file_array)
    {
        MMIWWW_LoadBookmarkFileArray(BOOKMARK_STEP_TYPE_ROOT, 0, FUNC_FIND_NULL);
    }

}

/*****************************************************************************/
// Description : Swap Bookmark
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: bookmark will be de destroyed if FALSE return
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_SwapBookmark(int32 index_1, int32 index_2)
{
    int32 bm_num = 0;
    BOOLEAN ret = FALSE;
    uint8 *title_ptr = PNULL;
    uint8 *url_ptr = PNULL;
    slim_int index = 0;
    NFBrowserMem nf_mem = {0};/*lint !e64*/
    slim_int in_self = MMIWWW_GETBROWSER;

    do
    {
        bm_num = MMIWWW_GetBookmarkNumber();
        if (index_1 >= bm_num || index_2 >= bm_num)
        {
            //SCI_TRACE_LOW:"MMIWWW_SwapBookmark Bad Param"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1411_112_2_18_3_11_32_34,(uint8*)"");
            break;
        }

        // Title of index_1
        SCI_MEMSET(&nf_mem, 0x00, sizeof(NFBrowserMem));
        ret = (BOOLEAN)NFBrowser_GetBookmarkTitle(in_self, index_1, &nf_mem);
        if (!ret)
        {
            //SCI_TRACE_LOW:"MMIWWW_SwapBookmark get %d's title failed "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1420_112_2_18_3_11_32_35,(uint8*)"d", index_1);
            break;
        }

        title_ptr = SCI_ALLOCA(((uint32)nf_mem.fDataSize * sizeof(uint8)));
        if (PNULL == title_ptr)
        {
            //SCI_TRACE_LOW:"MMIWWW_SwapBookmark Title No mem "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1427_112_2_18_3_11_32_36,(uint8*)"");
            NFBrowser_ReleaseSlimMem(&nf_mem);
            break;
        }
        SCI_MEMSET(title_ptr, 0, nf_mem.fDataSize);
        strncpy((char *)title_ptr, (char *)nf_mem.fData, nf_mem.fDataSize);
        NFBrowser_ReleaseSlimMem(&nf_mem);

        // URL of index_1
        SCI_MEMSET(&nf_mem, 0x00, sizeof(NFBrowserMem));
        ret = (BOOLEAN)NFBrowser_GetBookmarkUrl(in_self, index_1, &nf_mem);
        if (!ret)
        {
            //SCI_TRACE_LOW:"MMIWWW_SwapBookmark get %d's url failed "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1440_112_2_18_3_11_32_37,(uint8*)"d", index_1);
            break;
        }

        url_ptr = SCI_ALLOCA(((uint32)nf_mem.fDataSize * sizeof(uint8)));
        if (PNULL == url_ptr)
        {
            //SCI_TRACE_LOW:"MMIWWW_SwapBookmark Url No mem "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1447_112_2_18_3_11_32_38,(uint8*)"");
            NFBrowser_ReleaseSlimMem(&nf_mem);
            break;
        }
        SCI_MEMSET(url_ptr, 0, nf_mem.fDataSize);
        strncpy((char *)url_ptr, (char *)nf_mem.fData, nf_mem.fDataSize);
        NFBrowser_ReleaseSlimMem(&nf_mem);

        // Replace title of index_1 by index_2's
        SCI_MEMSET(&nf_mem, 0x00, sizeof(NFBrowserMem));
        ret = (BOOLEAN)NFBrowser_GetBookmarkTitle(in_self, index_2, &nf_mem);
        if (!ret)
        {
            //SCI_TRACE_LOW:"MMIWWW_SwapBookmark get %d's title failed "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1460_112_2_18_3_11_32_39,(uint8*)"d", index_2);
            break;
        }

        NFBrowser_ChangeBookmarkTitle(in_self, index_1, nf_mem.fData , &index);
        //SCI_TRACE_LOW:"MMIWWW_SwapBookmark index_1 = %d, index = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1465_112_2_18_3_11_32_40,(uint8*)"dd", index_1, index);
        NFBrowser_ReleaseSlimMem(&nf_mem);
 
        // Replace Url of index_1 by index_2's
        SCI_MEMSET(&nf_mem, 0x00, sizeof(NFBrowserMem));
        ret = (BOOLEAN)NFBrowser_GetBookmarkUrl(in_self, index_2, &nf_mem);
        if (!ret)
        {
            //SCI_TRACE_LOW:"MMIWWW_SwapBookmark get %d's url failed "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1473_112_2_18_3_11_32_41,(uint8*)"d", index_2);
            break;
        }

        NFBrowser_ChangeBookmarkUrl(in_self, index_1, nf_mem.fData);
        NFBrowser_ReleaseSlimMem(&nf_mem);

        // Replace title of index_2 by index_1's
        NFBrowser_ChangeBookmarkTitle(in_self, index_2, title_ptr , &index);
        //SCI_TRACE_LOW:"MMIWWW_SwapBookmark index_2 = %d, index = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1482_112_2_18_3_11_32_42,(uint8*)"dd", index_2, index);

        // Replace Url of index_2 by index_1's
        NFBrowser_ChangeBookmarkUrl(in_self, index_2, url_ptr);
        SCI_FREE(title_ptr);
        SCI_FREE(url_ptr);
        return TRUE;
    }
    while (0);

    if (PNULL != title_ptr)
    {
        SCI_FREE(title_ptr);
    }
    if (PNULL != url_ptr)
    {
        SCI_FREE(url_ptr);
    }

    return FALSE;
}

/*****************************************************************************/
// Description : Find duplicated Bookmark
// Global resource dependence :
// Author: fen.xie
// Note: negative number  means there is no duplicated bookmark
/*****************************************************************************/
PUBLIC int32 MMIWWW_FindDuplicatedBookmarkIndex(wchar *cur_path_ptr, 
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
        MMIWWW_BookmarkCombinePath(cur_path_ptr, cur_path_len, 
            new_name_ptr, MMIAPICOM_Wstrlen(new_name_ptr), 
            (wchar*)g_bookmark_file_suffix + 1, MMIAPICOM_Wstrlen(g_bookmark_file_suffix) -1, 
            file_name, &file_name_len);
    
        func_type=  (path_step > 0)? FUNC_FIND_FILE: FUNC_FIND_FILE_AND_FOLDER;
        MMIAPIFMM_SearchFileInPath(cur_path_ptr,
                cur_path_len,
                &s_bookmark_filter,
                FALSE,
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

    return bm_index;
}

/*****************************************************************************/
// Description : Find duplicated Bookmark dir
// Global resource dependence :
// Author: fen.xie
// Note: negative number  means there is no duplicated bookmark
/*****************************************************************************/
PUBLIC int32 MMIWWW_FindDuplicatedBookmarkDirIndex(wchar *cur_path_ptr, 
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

    return bm_index;
}

/*****************************************************************************/
//  Description : BookMark Initialize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_BookmarkIsInRoot(void)
{
    WWW_BOOKMARK_T *bookmark_info_ptr = MMIWWW_BOOKMARK();
    BOOLEAN result = FALSE;
    if (0 == bookmark_info_ptr->file_array_step)
    {
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : BookMark Initialize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWWW_BookmarkGetFolderNum(void)
{
    FILEARRAY file_array = PNULL;
    uint32 num = 0;
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;

    file_array = MMIAPIFILEARRAY_Create();
    
    slimFile_CombineFullPath((slim_char *)BRSCFG_FAVORITES, file_name, &file_name_len);

    MMIAPIFMM_SearchFileInPath(file_name, file_name_len,
            &s_bookmark_filter,
            FALSE,
            FUNC_FIND_FOLDER,
            file_array,
            0,
            0);

    num = MMIAPIFILEARRAY_GetArraySize(file_array);

    MMIAPIFILEARRAY_Destroy(&file_array);
    
    return num;
}

/*****************************************************************************/
//  Description : Clean bookmark Data Info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 清空书签
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_CleanBookmarkData(BOOLEAN in_force)
{
    BOOLEAN result = TRUE;
    wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar folder_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 folder_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    FILEARRAY   root_file_array = {0};
    FILEARRAY_DATA_T array_data = {0};
    uint32 root_num = 0;
    uint32 index = 0;

    slimFile_CombineFullPath((slim_char *)BRSCFG_FAVORITES, full_path_name, &full_path_name_len);

    //根目录下
    root_file_array = MMIAPIFILEARRAY_Create();    

    if (PNULL == root_file_array)
    {
        //SCI_TRACE_LOW:"MMIWWW_CleanBookmarkData root_file_array == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1708_112_2_18_3_11_32_43,(uint8*)"");
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
    MMIAPIFILEARRAY_Destroy(&root_file_array);
    
    //SCI_TRACE_LOW:"MMIWWW_CleanBookmarkData result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_BOOKMARK_1754_112_2_18_3_11_33_44,(uint8*)"d", result);

    return result;

}

/*****************************************************************************
** File Name:      mmimemo_func.c                                          *
** Author:          fen.xie                                                    *
** Date:            05/08/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the memo             *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_memo_trc.h"
#ifdef MMI_MEMO_SUPPORT
#include "mmk_app.h"
#include "mmifmm_export.h"
#include "mmimemo_func.h"
#include "gui_ucs2b_converter.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMIMEMO_FILENAME_MAX_LEN        30
#define MMIMEMO_FILEHEAD_BOM_LEN        3  //Byte Order Mark Length
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef struct _MMIMEMO_DATA_T_
{
    uint32 total;//total
    //uint32 cur_index_num;//current bookmark list 's total items num
    FILEARRAY file_array;
}MMIMEMO_DATA_T;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MMIFMM_FILTER_T s_memo_filter = {{"*.txt"}};
MMIMEMO_DATA_T s_memo_info = {0};
LOCAL const uint8  s_memo_bom_flag[MMIMEMO_FILEHEAD_BOM_LEN] = {0xEF, 0xBB, 0xBF};
LOCAL BOOLEAN s_save_file_no_space = FALSE;
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
const wchar g_memo_file_suffix[]   = {'.' , 't', 'x', 't', 0 };
const wchar g_memo_dir[]   = { 'M', 'e', 'm', 'o',  0};

//LOCAL const char s_memo_prefix_name[]   = "MEMO_";
//LOCAL const char s_memo_prefix_widget_name[]   = "MEMO_WIDGET_";

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : combile path
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MemoCombinePath(
                                     wchar           *path_ptr,      //in/out:may PNULL
                                     uint16          path_len,  //in/out:may PNULL,path_ptr不为空时,此处必须填max len
                                     wchar           *name_ptr,      //in/out:may PNULL,don't include suffix
                                     uint16          name_len,  //in/out:may PNULL,name_ptr不为空时,此处必须填max len
                                     wchar           *suffix_ptr,    //in/out:may PNULL,include dot
                                     uint16          suffix_len, //in/out:may PNULL,suffix_ptr不为空时,此处必须填max len
                                     wchar     *full_path_ptr, //out
                                     uint16          *full_path_len_ptr  //out
                                     );
                        
/**--------------------------------------------------------------------------*
 **                         WINDOW DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : MMIMEMO_Init
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMEMO_Init(
                        MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                        MMI_MESSAGE_ID_E    msg_id)
{
    MMIMEMO_LoadFileArray(win_id, msg_id);
}

/*****************************************************************************/
//  Description : MMIMEMO_DeleteUserDataInPhone
//  Global resource dependence : 
//  Author: leon.wang
//  Note:   delete all memo file stored in phone space  
/*****************************************************************************/
PUBLIC void MMIMEMO_DeleteUserDataInPhone(void)
{
    //删除所有Phone/memo下存储的备忘录
    MMIFILE_DEVICE_E    dev = MMI_DEVICE_UDISK;
    wchar       path[MMIFMM_FILENAME_LEN+1] = {0};
    uint16      path_length = MMIFMM_FILENAME_LEN;
    
    MMIAPIFMM_CombineFullPath(
        MMIAPIFMM_GetDevicePath(dev), 
        MMIAPIFMM_GetDevicePathLen(dev),
        MMIMEMO_DIR, 
        MMIAPICOM_Wstrlen(MMIMEMO_DIR),
        PNULL, 
        NULL,
        path,
        &path_length
        );

    MMIAPIFMM_DeleteDirectory(path);
}
/*****************************************************************************/
// Description : load the file array for memo, and init the s_memo_info
// Global resource dependence :s_memo_info
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_LoadFileArray(
                        MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                        MMI_MESSAGE_ID_E    msg_id)
{
    wchar    path[MMIFMM_FILENAME_LEN+1]={0};
    uint16    path_length = MMIFMM_FILENAME_LEN;
    MMIFILE_DEVICE_E  dev    = MMI_DEVICE_SDCARD;

    if( PNULL != s_memo_info.file_array)
    {
        MMIAPIFILEARRAY_Destroy(&(s_memo_info.file_array));
    }
    SCI_MEMSET(&s_memo_info, 0x00, sizeof(s_memo_info));

    s_memo_info.file_array = MMIAPIFILEARRAY_Create();

     //bug160098 
     //下面是原有的主干版本，对于大屏幕工程也需要遍历存储设备，保证同时能够显示不同存储设备的memo记录
     //故在此确保小屏、大屏处理一样
/*
#ifndef MMI_GUI_STYLE_MINISCREEN  //这是原有的主干版本，也许需要更改为else的样子
    dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
#ifdef CMCC_UI_STYLE
    dev = MMI_DEVICE_UDISK;
#endif

    MMIAPIFMM_CombineFullPath(
        MMIAPIFMM_GetDevicePath(dev), 
        MMIAPIFMM_GetDevicePathLen(dev),
        MMIMEMO_DIR, 
        MMIAPICOM_Wstrlen(MMIMEMO_DIR),
        PNULL, 
        NULL,
        path,
        &path_length
        );
    MMIAPIFMM_SearchFileInPath(path, path_length,
            &s_memo_filter,
            FALSE,
            FUNC_FIND_FILE,
            s_memo_info.file_array,
            win_id,
            msg_id);

#else
*/
    //NEWMS00211194  leon.wang  2012.8.2
    //保证能够同时显示不同存储位置的备忘录，大屏幕工程也可以考虑这种方法；
    //从phone开始读取memo记录，之后再读取T卡, 
    for(dev = MMI_DEVICE_UDISK; dev <= MMI_DEVICE_NUM -1; dev++)
    {
       if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev)))
       {
           MMIAPIFMM_CombineFullPath(
                MMIAPIFMM_GetDevicePath(dev), 
                MMIAPIFMM_GetDevicePathLen(dev),
                MMIMEMO_DIR, 
                MMIAPICOM_Wstrlen(MMIMEMO_DIR),
                PNULL, 
                NULL,
                path,
                &path_length
                );
            MMIAPIFMM_SearchFileInPath(path, path_length,
                    &s_memo_filter,
                    FALSE,
                    FUNC_FIND_FILE,
                    s_memo_info.file_array,
                    0,
                    0); //使用同步方式，保证多次搜索的结果能够同时表现出来；
        }
    }
    MMK_PostMsg(win_id, msg_id, PNULL, 0);
//#endif

    if (0 == win_id && 0 == msg_id)
    {
        //采用同步方式
        MMIAPIFILEARRAY_Sort(s_memo_info.file_array, FILEARRAY_SORT_TIME_DESCEND);  
        s_memo_info.total = MMIAPIFILEARRAY_GetArraySize(s_memo_info.file_array);
    }
    else
    {
        //handle in the msg of the win id
    }

    return TRUE;
}

/*****************************************************************************/
// Description : after load the file array for memo, update the s_memo_info
// Global resource dependence :s_memo_info
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_LoadFinish(void)
{
    MMIAPIFILEARRAY_Sort(s_memo_info.file_array,FILEARRAY_SORT_TIME_DESCEND);
    s_memo_info.total = MMIAPIFILEARRAY_GetArraySize(s_memo_info.file_array);
    return TRUE;
}
/*****************************************************************************/
// Description : destory the file array for memo
// Global resource dependence :s_memo_info
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_DestroyFileArray(void)
{
    if( PNULL != s_memo_info.file_array)
    {
        MMIAPIFILEARRAY_Destroy(&(s_memo_info.file_array));
    }
    SCI_MEMSET(&s_memo_info, 0x00, sizeof(s_memo_info));
    
    return TRUE;    
}

/*****************************************************************************/
// Description : get the total num of memo
// Global resource dependence :s_memo_info
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMEMO_GetTotalNum(void)
{
    return s_memo_info.total;
}

/*****************************************************************************/
// Description : Get the content of one memo
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_GetContent(
                            uint16 in_index, //in
                            MMI_STRING_T *out_content_ptr,//in/out:out_content->wstr_ptr need free outside
                            uint32  max_wstr_len, //in,允许的wstr最大长度 
                            SCI_DATE_T  *modify_date_ptr,   //日期
                            SCI_TIME_T  *modify_time_ptr    //时间
                            )
{
    FILEARRAY_DATA_T array_item = {0};
    wchar   *wstr_ptr = PNULL;
    uint8   *buf_ptr = PNULL;
    BOOLEAN result = FALSE;
    BOOLEAN is_little_endian = FALSE;
    BOOLEAN is_need_adjust = FALSE;
//  uint32  content_size = 0;
    uint32  max_content_size = 3*MMIMEMO_CONTENT_MAX_LEN + MMIMEMO_FILEHEAD_BOM_LEN;
   
    if (PNULL == out_content_ptr)
    {
        return FALSE;
    }
    
    if (PNULL == out_content_ptr->wstr_ptr)
    {
        return FALSE;
    }
//  SCI_MEMSET(out_content_ptr, 0x00, sizeof(MMI_STRING_T));
    
    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    if(MMIAPIFILEARRAY_Read(s_memo_info.file_array, (uint32)in_index, &array_item ))
    {
        if (modify_date_ptr != PNULL)
        {
            SCI_MEMCPY(modify_date_ptr, &array_item.modify_date, sizeof(SCI_DATE_T));
        }
        if (modify_time_ptr != PNULL)
        {
            SCI_MEMCPY(modify_time_ptr, &array_item.modify_time, sizeof(SCI_TIME_T));
        }
  
        array_item.size = (array_item.size < max_content_size)? array_item.size: max_content_size;//protect
    
        buf_ptr = SCI_ALLOCA(array_item.size + 1);
        if (PNULL == buf_ptr)
        {
            //SCI_TRACE_LOW:"[MEMO]MMIMEMO_GetContent: buf_ptr ALLOC FAIL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMEMO_FUNC_216_112_2_18_2_32_41_0,(uint8*)"");
            return FALSE;
        }
        SCI_MEMSET(buf_ptr, 0x00, array_item.size + 1);
        
        if(MMIAPIFMM_ReadFilesDataSyn((const wchar *)array_item.filename, array_item.name_len, buf_ptr, array_item.size))
        {
			if (array_item.size <= MMIMEMO_FILEHEAD_BOM_LEN)
			{
				SCI_FREE(buf_ptr);
				//SCI_TRACE_LOW:"[MEMO]MMIMEMO_GetContent: buf_ptr error!1"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMEMO_FUNC_226_112_2_18_2_32_41_1,(uint8*)"");
				return FALSE;      
 			}

//			content_size = array_item.size - MMIMEMO_FILEHEAD_BOM_LEN;
// 			wstr_ptr = SCI_ALLOC_APP(content_size + 1); //free outside and with end pos
// 			if(PNULL == wstr_ptr)
// 			{
// 				SCI_FREE(buf_ptr);
// 				return FALSE;
// 			}
// 
// 			SCI_MEMSET(wstr_ptr, 0x00, content_size + 1);
			GUI_UTF8ToWstr(out_content_ptr->wstr_ptr, 
						 max_wstr_len, 
						(uint8*)(buf_ptr + MMIMEMO_FILEHEAD_BOM_LEN), 
						strlen((uint8*)(buf_ptr + MMIMEMO_FILEHEAD_BOM_LEN)));

            //out_content_ptr->wstr_ptr = wstr_ptr;
            out_content_ptr->wstr_len = MMIAPICOM_Wstrlen(out_content_ptr->wstr_ptr);
            result = TRUE;

        }
    }

    if (PNULL != buf_ptr)
    {
        SCI_FREE(buf_ptr);
    }
    
    return result;
}
/*****************************************************************************/
// Description : Get the file name of one memo
// Global resource dependence :
// Author: guodong.liu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_GetFileName(
                            uint16 in_index, //in
                            wchar *out_filename_ptr,//in/out:out_content->wstr_ptr need free outside
                            uint32  max_wstr_len //in,允许的wstr最大长度 
                            )
{
    FILEARRAY_DATA_T array_item = {0};
    uint8   *buf_ptr = PNULL;
    BOOLEAN result = FALSE;
    uint32  max_content_size = max_wstr_len;
   
    if (PNULL == out_filename_ptr)
    {
        return FALSE;
    }
    
  
    SCI_MEMSET(&array_item, 0x00, sizeof(FILEARRAY_DATA_T));
    if(MMIAPIFILEARRAY_Read(s_memo_info.file_array, (uint32)in_index, &array_item ))
    {  
        array_item.size = (array_item.size < max_content_size)? array_item.size: max_content_size;//protect
    
        buf_ptr = SCI_ALLOCA(array_item.size + 1);
        if (PNULL == buf_ptr)
        {
            //SCI_TRACE_LOW:"[MEMO]MMIMEMO_GetContent: buf_ptr ALLOC FAIL!"
            return FALSE;
        }
        SCI_MEMSET(buf_ptr, 0x00, array_item.size + 1);
        
        if(MMIAPIFMM_ReadFilesDataSyn((const wchar *)array_item.filename, array_item.name_len, buf_ptr, array_item.size))
        {
			MMIAPICOM_Wstrncpy(out_filename_ptr, array_item.filename, MMIAPICOM_Wstrlen(array_item.filename));
            result = TRUE;
        }
    }

    if (PNULL != buf_ptr)
    {
        SCI_FREE(buf_ptr);
    }
    
    return result;
}

/*****************************************************************************/
// Description : Deletes one memo specified by in_index
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_DelOne(uint16 in_index)
{
    FILEARRAY_DATA_T array_item = {0};
    BOOLEAN result = FALSE;

    if(MMIAPIFILEARRAY_Read(s_memo_info.file_array, (uint32)in_index, &array_item ))
    {
        if (SFS_NO_ERROR == MMIAPIFMM_DeleteFile((const wchar *)array_item.filename,  PNULL))
        {
            result = MMIAPIFILEARRAY_Delete(s_memo_info.file_array, (uint32)in_index);
            
            if (0 < s_memo_info.total)
            {
                s_memo_info.total--;
            }
        }
    }

    return result;
}

/*****************************************************************************/
// Description : Adds a new bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN MemoCreateFileName(MMI_STRING_T *name_str_ptr, //[IN/OUT]
                                            BOOLEAN in_for_widget_memo//IN
                                            )
{
    SCI_DATE_T date = {0};
    SCI_TIME_T time = {0};
    char    name[MMIMEMO_FILENAME_MAX_LEN+1] = {0};

    if (PNULL == name_str_ptr)
    {
        return FALSE;
    }

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
  
    sprintf(name, "MEMO_%04d%02d%02d_%02d%02d%02d%02d", 
        date.year, date.mon, date.mday, time.hour, time.min, time.sec,rand());

    name_str_ptr->wstr_len = SCI_STRLEN(name);
    name_str_ptr->wstr_ptr = SCI_ALLOC((name_str_ptr->wstr_len + 1) * sizeof(wchar));
    if (name_str_ptr->wstr_ptr != PNULL)
    {
        SCI_MEMSET(name_str_ptr->wstr_ptr, 0x00, ((name_str_ptr->wstr_len + 1) * sizeof(wchar)));
        MMI_STRNTOWSTR(name_str_ptr->wstr_ptr, name_str_ptr->wstr_len, 
            (uint8*)name, name_str_ptr->wstr_len, 
            name_str_ptr->wstr_len);
        return TRUE;            
    }
    else
    {
        name_str_ptr->wstr_len = 0;
        return FALSE;
    }

    
}


/*****************************************************************************/
// Description : Adds a new memo
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_AddOne(MMI_STRING_T in_url_str, //IN
                                            BOOLEAN in_for_widget_memo, //IN
                                            uint16 *out_index//OUT:new index
                                            )
{
    FILEARRAY_DATA_T array_item = {0};
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_HANDLE file_handle = 0;
    //uint8 *url_buf = PNULL;
    uint32 bytes_to_write = 0;
    uint32 bytes_written = 0;
    BOOLEAN result = FALSE;
    BOOLEAN is_exit =FALSE;
    //MMI_STRING_T string_1 ={0};
    //MMI_STRING_T string_dest ={0};
    //uint32 dest_wstr_byte_len = 0;
    SFS_DATE_T  modify_date = {0};
    SFS_TIME_T  modify_time = {0};
    wchar    path[MMIFMM_FILENAME_LEN+1]={0};
    uint16    path_length = MMIFMM_FILENAME_LEN;
    MMIFILE_DEVICE_E  dev    = MMI_DEVICE_SDCARD;
    uint8 *				char_content_ptr = PNULL;
    uint16				text_content_len = 0;
	SFS_ERROR_E			sfs_error = SFS_ERROR_NONE;
    
    //create new bookmark file's name
	if(!MMIMEMO_IsContentEdit())
    {
        //get the title from the modify time
        MMI_STRING_T string_name ={0};

        MemoCreateFileName(&string_name, in_for_widget_memo);
        dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
#ifdef CMCC_UI_STYLE
        dev = MMI_DEVICE_UDISK;
#endif
        MMIAPIFMM_CombineFullPath(
            MMIAPIFMM_GetDevicePath(dev), 
            MMIAPIFMM_GetDevicePathLen(dev),
            MMIMEMO_DIR, 
            MMIAPICOM_Wstrlen(MMIMEMO_DIR),
            PNULL, 
            NULL,
            path,
            &path_length
            );
        
        MemoCombinePath(path, path_length,
            string_name.wstr_ptr, string_name.wstr_len,
            (wchar*)g_memo_file_suffix + 1, MMIAPICOM_Wstrlen(g_memo_file_suffix) -1, 
            file_name, &file_name_len);

        if (string_name.wstr_ptr != PNULL)
        {
            SCI_FREE(string_name.wstr_ptr);
        }
    }
	else
	{
	 MMIAPICOM_Wstrncpy(file_name, MMIMEMO_GetEdittext_Ptr(), MMIAPICOM_Wstrlen(MMIMEMO_GetEdittext_Ptr()));
	}
    s_save_file_no_space = FALSE;
    is_exit = MMIAPIFMM_IsFileExist(file_name, MMIAPICOM_Wstrlen(file_name));
    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_CREATE_NEW, 0, 0);
	SCI_TRACE_LOW("MMIMEMO_AddOne is_exit: %d, file_handle:%x", is_exit, file_handle);
    if (file_handle != 0)
    {
        sfs_error = MMIAPIFMM_WriteFile(file_handle, &s_memo_bom_flag, MMIMEMO_FILEHEAD_BOM_LEN, &bytes_written, PNULL);
		SCI_TRACE_LOW("MMIMEMO_AddOne sfs_error1 : %d", sfs_error);
		if(SFS_ERROR_NONE == sfs_error)
		{
			text_content_len = (in_url_str.wstr_len * 3) * sizeof(uint8);
			char_content_ptr = SCI_ALLOC_APP(text_content_len + 1);
			if (PNULL == char_content_ptr)
			{
				MMIAPIFMM_CloseFile(file_handle);
				return FALSE;
			}

			SCI_MEMSET(char_content_ptr, 0x00, text_content_len + 1);
			GUI_WstrToUTF8(char_content_ptr, text_content_len, in_url_str.wstr_ptr, in_url_str.wstr_len);
			sfs_error = MMIAPIFMM_WriteFile(file_handle, char_content_ptr, text_content_len, &bytes_written, PNULL);
			SCI_TRACE_LOW("MMIMEMO_AddOne  sfs_error2 :%d", sfs_error);
			if (SFS_ERROR_NONE == sfs_error)
			{
				result = TRUE;
			}
			else if (SFS_ERROR_NO_SPACE == sfs_error)
			{
				s_save_file_no_space = TRUE;
				result = FALSE;
			}
		}
		else if (SFS_ERROR_NO_SPACE == sfs_error)
		{
			s_save_file_no_space = TRUE;
			result = FALSE;
		}
		SCI_TRACE_LOW("MMIMEMO_AddOne  sfs_error3 :%d, result: %d", sfs_error, result);
        MMIAPIFMM_CloseFile(file_handle);
		
		if (PNULL != char_content_ptr)
		{
			SCI_FREE(char_content_ptr);
			char_content_ptr = PNULL;
		}
    }
    else
    {
        return FALSE;
    }

    if (result)
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
        
        MMIAPIFILEARRAY_Add(s_memo_info.file_array, &array_item);

        s_memo_info.total++;
    }
    else if(!is_exit)//NEWMS00135487
    {
        MMIAPIFMM_DeleteFile(file_name, PNULL);
    }
	SCI_TRACE_LOW("MMIMEMO_AddOne  result = %d, s_save_file_no_space = %d", result, s_save_file_no_space);
    //*out_index = bookmark_info_ptr->cur_index_num-1;
    return result;
}

/*****************************************************************************/
//  Description : combile path
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MemoCombinePath(
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
//  Description : MMIMEMO_IsNoSpace
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 是否为空间不足导致的保存失败
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_IsNoSpace(void)
{
    return s_save_file_no_space;
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

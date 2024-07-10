/****************************************************************************
** File Name:      mmiebook_file.c                              
** Author:         songbin.zeng                                                        
** Date:           2006/06/9 
** Copyright:      2006 Spreadrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to read/write text file
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 06/6/2006         songbin.zeng          Create
** 
****************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_ebook_trc.h"
#ifdef EBOOK_SUPPORT
#include "window_parse.h"
#include "guifont.h"
#include "gui_ucs2b_converter.h"
#include "mmiacc.h"
#include "block_mem.h"
#include "mmifmm_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guicommon.h"
#include "mmisd_export.h"

#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif

//#include "mmiebook.h"
#include "mmiebook_internal.h"
#include "mmiebook_file.h"

/**--------------------------------------------------------------------------*
 **                         Macro Definition                                 *
 **--------------------------------------------------------------------------*/
#define MMIEBOOK_LATEST_LIST_FILE_NAME_UCS2              (s_mmiebook_latest_list_file_name_wstr)//"\x00\x65\x00\x62\x00\x6F\x00\x6F\x00\x6B\x00\x2E\x00\x64\x00\x61\x00\x74\x00\x00"
#define MMIEBOOK_LATEST_LIST_FILE_NAME_UCS2_LEN          14

#define MMIEBOOK_BOOKMARK_FILE_SUFFIX_NAME_UCS2             (s_mmiebook_bookmark_file_suffix_name_wstr)//"\x00\x65\x00\x62\x00\x6F\x00\x6F\x00\x6B\x00\x2E\x00\x64\x00\x61\x00\x74\x00\x00"
#define MMIEBOOK_BOOKMARK_FILE_SUFFIX_NAME_UCS2_LEN          4

#define MMIEBOOK_CHECK_SIZE  1000
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
PUBLIC MMIEBOOK_ALL_DEV_FILE_BRIEF_T    g_ebook_all_dev_file_brief = {{MMI_DEVICE_UDISK, 
                                                                       MMI_DEVICE_UDISK,
                                                                       MMI_DEVICE_UDISK,
                                                                       MMI_DEVICE_UDISK,
                                                                       MMI_DEVICE_UDISK,
                                                                       MMI_DEVICE_UDISK,
                                                                       MMI_DEVICE_UDISK,
                                                                       MMI_DEVICE_UDISK,
                                                                       MMI_DEVICE_UDISK,
                                                                       MMI_DEVICE_UDISK}, {0}, 0, 0, NULL};       //所有存储设备中的电子书简略信息
/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//add by rongjin, fixed bug 21540, 070723        

LOCAL MMIEBOOK_FILE_BRIEF_T*		s_ebook_file_brief = PNULL;

//end add
LOCAL MMIFILE_HANDLE                           s_ebook_show_file_handle = PNULL;   //当前阅读的电子书句柄
LOCAL MMIFILE_HANDLE                           s_ebook_latest_list_file_handle = PNULL;//最近阅读的电子书列表文件句柄
//

//const wchar g_mmiebook_dir_wstr[] = { 'E', 'b', 'o', 'o', 'k', 0 };
LOCAL const wchar s_mmiebook_latest_list_file_name_wstr[] = { 'e', 'b', 'o', 'o', 'k', '_', 'l', 'a', 's', 't', '.','l', 's', 't',  0 };// 

LOCAL MMIFILE_HANDLE s_ebook_bookmark_file_handle = PNULL;//书签文件句柄
LOCAL const wchar s_mmiebook_bookmark_file_suffix_name_wstr[] = {'.','B','M','K',0};//书签文件的后缀名

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                  *
**-------------------------------------------------------------------------- */

/*****************************************************************************/
//  Description : 获取文件操作方式
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
LOCAL uint32 GetFileOptMode(const char *mode);


/**--------------------------------------------------------------------------*
**                         PUBLIC FUNCTION DEFINITION                        *
**--------------------------------------------------------------------------*/
extern void MMIAPICOM_GetFileNameAndSuffix( 
                                           const wchar  *ucs_full_name_ptr,     //IN
                                           wchar        *ucs_suffix_ptr,        //OUT
                                           wchar        *ucs_name_ptr           //OUT
                                           );
/*****************************************************************************/
//  Description : 创建所有存储设备电子书目录
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIAPIEBOOK_CreateAllDevEbookDir(void)
{
    int32 i = 0;
    MMIFILE_ERROR_E error = SFS_NO_ERROR;
    SCI_TRACE_LOW("ebook_time:MMIAPIEBOOK_CreateAllDevEbookDir  start=%d",SCI_GetTickCount());
    //获取设备列表
    MMIEBOOK_GetDevInfo();
    //可能有几个存储设备
    for (i = 0; i < g_ebook_all_dev_file_brief.dev_num; i++)
    {
        error |= MMIEBOOK_CreateEbookDir(g_ebook_all_dev_file_brief.dev_tab[i]);/*lint !e655*/
    }
    SCI_TRACE_LOW("ebook_time:MMIAPIEBOOK_CreateAllDevEbookDir  end=%d",SCI_GetTickCount());
    return error;
}

/*****************************************************************************/
//  Description : Get text file type
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetTextFileType(//type
                                        MMIFILE_HANDLE file_handle, // in
                                        GUI_CHAR_CODE_TYPE_E *file_type // out
                                        )
{
    GUI_CHAR_CODE_TYPE_E type = GUI_CHAR_CODE_ANSI;
    uint32 len = 1024;
    MMIFILE_ERROR_E file_read_error = SFS_NO_ERROR;
    uint8*          data_str_ptr = PNULL;
    //SCI_ASSERT(PNULL != file_handle);/*assert verified*/
    if(NULL == file_handle)
    {
        return FALSE;
    }
    SCI_TRACE_LOW("ebook_time:MMIEBOOK_GetTextFileType  start=%d",SCI_GetTickCount());
    data_str_ptr = (uint8 *)SCI_ALLOCA(len + 1);
    if(PNULL == data_str_ptr)
    {
        return FALSE;
    }           
    MMIAPIFMM_SetFilePointer(file_handle, 0, MMIFILE_SEEK_BEGIN);	
    file_read_error = MMIAPIFMM_ReadFile(file_handle, data_str_ptr, len, &len, PNULL);
    if (file_read_error != SFS_NO_ERROR)
    {
        SCI_FREE(data_str_ptr);
        return FALSE;
    }
    
    type = GUI_GetCodeType(data_str_ptr, len);
    if(GUI_CHAR_CODE_ANSI == type)
    {
        if(MMIEBOOK_CODING_TYPE_BIG5 == MMIAPIEBOOK_GetCodingType())
        {
            type = GUI_CHAR_CODE_ANSI_BIG5;
        }
    }
    else if(GUI_CHAR_CODE_ANSI_BIG5 == type)
    {
        if(MMIEBOOK_CODING_TYPE_GB == MMIAPIEBOOK_GetCodingType())
        {
            type = GUI_CHAR_CODE_ANSI;
        }
    }
    *file_type = type;
    SCI_FREE(data_str_ptr);
    SCI_TRACE_LOW("ebook_time:MMIEBOOK_GetTextFileType  end=%d",SCI_GetTickCount());
    return TRUE;
}

/*****************************************************************************/
//  Description : 释放文件所需资源
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_FreeEbookFileRes(void)
{
	//释放简略信息内存

	g_ebook_all_dev_file_brief.total_num = 0;

	//add by rongjin, fixed bug 21540, 070723        

    MMIEBOOK_DestroyFileBrief();
	MMIEBOOK_SetFilelistupdate(FALSE);
	//add end
}

/*****************************************************************************/
//  Description : 在指定设备上产生电子书目录
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIEBOOK_CreateEbookDir(MMIFILE_DEVICE_E dev)
{
    MMIFILE_ERROR_E     error = SFS_ERROR_NONE;
    BOOLEAN             isCreateDir = FALSE;
    wchar              full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
	

    if(MMIEBOOK_GetFileFullPathUCS2
                                                 (
                                                 dev,
                                                MMIMULTIM_DIR_EBOOK, 
                                                MMIAPICOM_Wstrlen(MMIMULTIM_DIR_EBOOK), 
                                                NULL, 
                                                0, 
                                                full_path_name, 
                                                &full_path_len
                                                ))
       
   { 
        if(MMIAPIFMM_IsFolderExist(full_path_name,  full_path_len))
        {
            return SFS_ERROR_HAS_EXIST;
        }
        isCreateDir = MMIAPIFMM_CreateDir(full_path_name, full_path_len);
	    
        if (isCreateDir)
        {
            error = SFS_ERROR_NONE;
        }
        else
        {
            error = SFS_ERROR_HAS_EXIST;
        }
    }
    return error;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:  rongjin
//  Note:
//  Return:
/*****************************************************************************/
#if 0
PUBLIC void  MMIEBOOK_SetTxtBrief(void)
{    
	SCI_ASSERT(s_ebook_file_brief != PNULL);/*assert verified*/
	s_ebook_file_brief->name_len = (uint16)MIN(MMIEBOOK_TXT_NAME_MAX_LEN, s_ebook_file_brief->name_len);
	s_ebook_file_brief->detail_id = -1;
}
#endif
/*****************************************************************************/
//  Description : 搜索文件设备
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:设备总数
/*****************************************************************************/
PUBLIC uint16 MMIEBOOK_GetDevInfo(void)
{
    MMIFILE_DEVICE_E dev = MMI_DEVICE_SDCARD;
    
	g_ebook_all_dev_file_brief.dev_num = 0;

    do
    {
        if(MMIAPIFMM_GetDeviceTypeStatus(dev))
        {
            g_ebook_all_dev_file_brief.dev_tab[g_ebook_all_dev_file_brief.dev_num] = dev;
            g_ebook_all_dev_file_brief.dev_num++;
        }
    }while(++dev < MMI_DEVICE_NUM);

    if(MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_UDISK))
    {
        g_ebook_all_dev_file_brief.dev_tab[g_ebook_all_dev_file_brief.dev_num] = MMI_DEVICE_UDISK;
        g_ebook_all_dev_file_brief.dev_num++;
    }
    //SCI_PASSERT(g_ebook_all_dev_file_brief.dev_num < MMIEBOOK_DEV_TAB_MAX_LEN, ("MMIEBOOK_GetDevInfo: g_ebook_all_dev_file_brief.dev_num is %d",g_ebook_all_dev_file_brief.dev_num));/*assert verified*/
   
    return g_ebook_all_dev_file_brief.dev_num;
}

/*****************************************************************************/
//  Description : 获取正在阅读的电子书句柄
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:电子书句柄
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIEBOOK_GetShowFileHandle(void)
{
    //SCI_ASSERT(PNULL != s_ebook_show_file_handle);/*assert verified*/
    return s_ebook_show_file_handle;
}

/*****************************************************************************/
//  Description : 阅读的电子书是否打开了
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->打开了
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsShowFileOpen(MMIFILE_DEVICE_E dev)
{
    if (s_ebook_show_file_handle != PNULL && dev != MMI_DEVICE_NUM)
    {
        return TRUE;
    }
    return FALSE; 
}

/*****************************************************************************/
//  Description : 关闭正在阅读的电子书
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CloseShowFile(void)
{
    BOOLEAN flag = TRUE;
    MMIEBOOK_FFS_fclose(s_ebook_show_file_handle);
    s_ebook_show_file_handle = PNULL;
    return flag;
}

/*****************************************************************************/
//  Description : 打开电子书列表中选择的电子书
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC MMIEBOOK_OPEN_FILE_ERROR_E MMIEBOOK_OpenShowFile(
                                     uint16    cur_selection//IN: 列表中的ID
                                     )
{       
    MMIEBOOK_FILE_BRIEF_T * brief_ptr = PNULL;
    wchar                  full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    //uint16                  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32                  opt_mode = 0;
    MMIEBOOK_OPEN_FILE_ERROR_E	result =  MMIEBOOK_OPEN_FILE_NO_ERROR;
    
    brief_ptr = s_ebook_file_brief;
    SCI_TRACE_LOW("ebook_time:MMIEBOOK_OpenShowFile  start=%d",SCI_GetTickCount());
    if (s_ebook_file_brief->full_name_len > 0)
    {
        MMI_WSTRNCPY(
            full_path,
            MMIFILE_FULL_PATH_MAX_LEN,
            s_ebook_file_brief->full_name_wstr, 
            s_ebook_file_brief->full_name_len,
            s_ebook_file_brief->full_name_len
            );
        result = MMIEBOOK_OPEN_FILE_NO_ERROR;
    }
    else
    {
       /* if(MMIEBOOK_GetFileFullPathUCS2
		              (
                      brief_ptr->file_dev, 
                      MMIMULTIM_DIR_EBOOK, 
                      MMIAPICOM_Wstrlen(MMIMULTIM_DIR_EBOOK),		
                      brief_ptr->name_wstr,
                      brief_ptr->name_len,
                      full_path, 
                      &full_path_len
                      ))  
	        {
	            result = MMIEBOOK_OPEN_FILE_NO_ERROR;
	        }
	        else*/
	        //{
	            result = MMIEBOOK_OPEN_FILE_ERROR;
	        //}    
    }

    opt_mode = GetFileOptMode("r");
    //打开指定文件
    if(MMIEBOOK_OPEN_FILE_NO_ERROR == result)
    {
        s_ebook_show_file_handle = MMIAPIFMM_CreateFile(
            full_path, 
            opt_mode, 
            PNULL, 
            PNULL
            );
        
        if (PNULL != s_ebook_show_file_handle)
        {
#ifdef DRM_SUPPORT
            BOOLEAN is_drm_file = FALSE;
            is_drm_file = MMIAPIDRM_IsDRMFile(s_ebook_show_file_handle, PNULL);
            if (is_drm_file)
            {
                if (!MMIAPIDRM_IsRightsValid(s_ebook_show_file_handle, 
                PNULL, TRUE, DRM_PERMISSION_DISPLAY))
                {
                    //SCI_TRACE_LOW:"MMIEBOOK_OpenShowFile invalid rights."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_374_112_2_18_2_15_14_4,(uint8*)"");
                    result = MMIEBOOK_OPEN_FILE_NO_RIGHTS;
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIEBOOK_OpenShowFile consume rights."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_379_112_2_18_2_15_14_5,(uint8*)"");
                    MMIAPIDRM_ConsumeRights(s_ebook_show_file_handle, PNULL);
                    result = MMIEBOOK_OPEN_FILE_NO_ERROR;
                }
            }
            else
            {
                result = MMIEBOOK_OPEN_FILE_NO_ERROR;
            }
#else
            result = MMIEBOOK_OPEN_FILE_NO_ERROR;
#endif
        }
        else
        {
            //SCI_TRACE_LOW:"MMIEBOOK_OpenShowFile open fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_394_112_2_18_2_15_14_6,(uint8*)"");
            result = MMIEBOOK_OPEN_FILE_ERROR;
        }
    }
    else
    {
		
        result = MMIEBOOK_OPEN_FILE_ERROR;
    }
    SCI_TRACE_LOW("ebook_time:MMIEBOOK_OpenShowFile  end=%d",SCI_GetTickCount());
    return result;
}

/*****************************************************************************/
//  Description : 根据文件设备类型获取设备名
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetDevNameByType(
								   MMIFILE_DEVICE_E file_dev,
								   wchar* ucs2_dev_name_ptr,
								   uint16* ucs2_dev_name_len_ptr
								   )
{
    BOOLEAN ret=FALSE;
    //SCI_ASSERT(PNULL != ucs2_dev_name_ptr);/*assert verified*/
    //SCI_ASSERT(PNULL != ucs2_dev_name_len_ptr);/*assert verified*/
    if(PNULL == ucs2_dev_name_ptr||PNULL == ucs2_dev_name_len_ptr)
    {
        return FALSE;
    }

    if(file_dev < MMI_DEVICE_NUM)
    {
        MMI_WSTRNCPY(ucs2_dev_name_ptr, 
                 MMIAPIFMM_GetDevicePathLen(file_dev), 
                 MMIAPIFMM_GetDevicePath(file_dev),
                 MMIAPIFMM_GetDevicePathLen(file_dev), 
                 MMIAPIFMM_GetDevicePathLen(file_dev));
	    *ucs2_dev_name_len_ptr = MMIAPIFMM_GetDevicePathLen(file_dev);
		 ret = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIEBOOK_GetDevNameByType file_dev=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_436_112_2_18_2_15_14_7,(uint8*)"d",file_dev);
        ret = FALSE;
    }
   
    return ret;
}

/*****************************************************************************/
//  Description : 获取文件全路径
//	Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetFileFullPathUCS2(
									  MMIFILE_DEVICE_E        file_dev,//in
                                                              const wchar     *ucs2_dir_ptr,           //in
                                                              uint16           ucs2_dir_len,            //in
                                                              const wchar      *ucs2_file_name_ptr,     //in, can be NULL
                                                              uint16           ucs2_file_name_len,      //in
                                                              wchar            *ucs2_full_path_ptr,     //out
                                                              uint16           *ucs2_full_path_len_ptr  //out
									  )
{
    wchar              dev_name[MMIFILE_DEVICE_NAME_MAX_LEN +1] = {0};
    uint16              dev_name_len = 0;
	
    //SCI_ASSERT(PNULL != ucs2_full_path_ptr);/*assert verified*/
    //SCI_ASSERT(PNULL != ucs2_full_path_len_ptr);/*assert verified*/
    if(PNULL == ucs2_full_path_ptr||PNULL == ucs2_full_path_len_ptr)
    {
        return FALSE;
    }
    
    if(!MMIEBOOK_GetDevNameByType(file_dev, dev_name, &dev_name_len))
    {
        return FALSE;
    }
	
    if( MMIAPIFMM_CombineFullPath(
		dev_name, 
		dev_name_len, 
		ucs2_dir_ptr, 
		ucs2_dir_len, 
		ucs2_file_name_ptr, 
		ucs2_file_name_len, 
		ucs2_full_path_ptr, 
		ucs2_full_path_len_ptr
		))
    	{
		*ucs2_full_path_len_ptr = (*ucs2_full_path_len_ptr) ;
		return TRUE;
    	}
	else
	{
		return FALSE;
	}
    //SCI_TRACE_BUF("ucs2_full_path:%x", ucs2_full_path_ptr, *ucs2_full_path_len_ptr);
}

/*****************************************************************************/
//  Description : 获取文件操作方式
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
LOCAL uint32 GetFileOptMode(const char *mode)
{
    uint32 opt = 0;
    
    //SCI_ASSERT(PNULL != mode);/*assert verified*/
    if(PNULL == mode)
    {
        return opt;
    }
    
    if(0 == MMIAPICOM_Stricmp((const uint8 *)mode, (const uint8 *)"r"))
	{
        opt = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ;/*lint !e655*/
	}
    else if (0 == MMIAPICOM_Stricmp((const uint8 *)mode, (const uint8 *)"r+"))
	{
        opt = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ | SFS_MODE_WRITE;/*lint !e655*/
	}
    else if (0 == MMIAPICOM_Stricmp((const uint8 *)mode, (const uint8 *)"w"))
	{
        opt = SFS_MODE_OPEN_ALWAYS | SFS_MODE_WRITE;/*lint !e655*/
	}
    else if (0 == MMIAPICOM_Stricmp((const uint8 *)mode, (const uint8 *)"w+"))
	{
        opt = SFS_MODE_CREATE_ALWAYS | SFS_MODE_READ | SFS_MODE_WRITE;/*lint !e655*/
	}
    else if (0 == MMIAPICOM_Stricmp((const uint8 *)mode, (const uint8 *)"a"))
	{
        opt = SFS_MODE_APPEND | SFS_MODE_WRITE;/*lint !e655*/
	}
    else if (0 == MMIAPICOM_Stricmp((const uint8 *)mode, (const uint8 *)"a+"))
	{
        opt = SFS_MODE_APPEND | SFS_MODE_WRITE | SFS_MODE_READ ;/*lint !e655*/
	}
    else
	{
	    //SCI_TRACE_LOW:"GetFileOptMode error: mode is %s"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_533_112_2_18_2_15_14_8,(uint8*)"s",mode);
        //SCI_PASSERT(0, ("GetFileOptMode: mode is %s",mode));/*assert verified*/	
	}
	
    return opt;
}

/*****************************************************************************/
//  Description : Sets the current position of a file pointer
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC int MMIEBOOK_FFS_fseek(
						   MMIFILE_HANDLE  file_handle,    //文件
						   int32           offset,
						   uint32          origin      //0，1，2 分别表示从当前位置，文件结尾，文件开始
						   )
{
    int       result = 0;
    uint32    file_size;
    int32     current;
    
    //SCI_ASSERT(NULL != file_handle); /*assert verified*/   
    if(PNULL == file_handle)
    {
        return result;
    }
    
    file_size = MMIAPIFMM_GetFileSize(file_handle);

    // appended by guanyong.zhang. cr97506
    // 使得offset为有效的offset
    switch (origin)
    {
    case MMIFILE_SEEK_BEGIN:
        if (offset < 0)
        {
            offset = 0;
        }
        else if (offset > (int32)file_size)
        {
            offset = file_size;
        }
        break;

    case MMIFILE_SEEK_CUR:
        current = MMIAPIFMM_GetFilePointer(file_handle, MMIFILE_SEEK_BEGIN);

        if (offset >= 0)
        {
            if ((current + offset)  >(int32) file_size)
            {
                offset = (int32)file_size - current;
            }
        }
        else
        {
            if ((offset * -1) > current)
            {
                offset = current * -1;
            }
        }
        break;

    case MMIFILE_SEEK_END:
        if (offset > 0)
        {
            offset = 0;
        }
        else if ((offset * -1) > (int32)file_size)
        {
            offset = (int32)file_size * (-1);
        }
        break;

    default:
        break;
    }
        
    result = MMIAPIFMM_SetFilePointer(file_handle, offset, origin);

    return result;          
}

/*****************************************************************************/
//  Description : write file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_FFS_fwrite(
							   const void * buffer, 
							   uint32 size, 
							   uint32 count, 
							   MMIFILE_HANDLE file_handle
							   )
{
    uint32 len = 0;
    BOOLEAN result = FALSE;
    
    //SCI_ASSERT(NULL != file_handle);/*assert verified*/
    //SCI_ASSERT(NULL != buffer);/*assert verified*/
    //SCI_PASSERT(1 == count, ("MMIEBOOK_FFS_fwrite: count is %d",count));/*assert verified*/	
    if(NULL == file_handle ||NULL == buffer ||1 != count)
    {
    	return FALSE;
    }

    if(SFS_ERROR_NONE == MMIAPIFMM_WriteFile(file_handle, buffer, size, &len, NULL))
    {
        //SCI_PASSERT(size == len, ("MMIEBOOK_FFS_fwrite: size is %d,len is %d",size,len));/*assert verified*/	
        //SCI_TRACE_LOW:"MMIEBOOK_FFS_fwrite: size is %d,len is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_641_112_2_18_2_15_14_9,(uint8*)"dd",size,len);
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description :read file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIEBOOK_FFS_fread(
							  void * buffer, 
							  uint32 size, 
							  uint32 count, 
							  MMIFILE_HANDLE file_handle
							  )
{
    uint32 len = 0;
	
    //SCI_ASSERT(NULL != file_handle);/*assert verified*/
    //SCI_ASSERT(NULL != buffer);/*assert verified*/
	if(NULL != file_handle && NULL != buffer)
	{
	    MMIAPIFMM_ReadFile(
			file_handle, 
			buffer, 
			size * count, 
			&len, 
			NULL
			);
	}
	return len;
}

/*****************************************************************************/
//  Description : close file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_FFS_fclose(
							 MMIFILE_HANDLE file_handle
							 )
{
    if (PNULL != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle); 
    }
    else
    {
        //SCI_TRACE_LOW:"MMIEBOOK_FFS_fclose file_handle = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_689_112_2_18_2_15_14_10,(uint8*)"");
    }

    return;
}

/*****************************************************************************/
//  Description : return s_ebook_file_brief
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC MMIEBOOK_FILE_BRIEF_T* MMIEBOOK_GetFileBrief(void)
{
    return s_ebook_file_brief;
}

/*****************************************************************************/
//  Description :初始化s_ebook_file_brief
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_InitFileBrief(uint8 file_dev,const wchar* name_ptr, uint32 name_length, uint32 file_size, const wchar* full_name_ptr, uint32 full_name_length)
{
    MMIFILE_DEVICE_E dev = (MMIFILE_DEVICE_E)file_dev;
    
    if(PNULL != s_ebook_file_brief)
    {
        SCI_FREE(s_ebook_file_brief);
    }

    //为文件简略信息分配空间
    s_ebook_file_brief = SCI_ALLOCA(sizeof(MMIEBOOK_FILE_BRIEF_T));
    if(PNULL == s_ebook_file_brief)
    {
        //SCI_TRACE_LOW:"MMIEBOOK_FILE MMIEBOOK_InitFileBrief s_ebook_file_brief out of memory"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_722_112_2_18_2_15_14_11,(uint8*)"");
        return FALSE;
    }
    
    //SCI_ASSERT(s_ebook_file_brief != PNULL);/*assert verified*/
    SCI_MEMSET(s_ebook_file_brief, 0, sizeof(MMIEBOOK_FILE_BRIEF_T));

    s_ebook_file_brief->file_dev = dev;

    //MMI_WSTRNCPY(s_ebook_file_brief->name_wstr,MMIEBOOK_TXT_NAME_MAX_LEN,name_ptr,name_length,name_length);
    //s_ebook_file_brief->name_len = (uint16)MIN(MMIEBOOK_TXT_NAME_MAX_LEN, name_length);

    if (full_name_length > 0)
    {
        MMI_WSTRNCPY(s_ebook_file_brief->full_name_wstr,MMIEBOOK_FULLNAME_MAX_LEN,full_name_ptr,full_name_length,full_name_length);
    }
    s_ebook_file_brief->full_name_len = (uint16)MIN(MMIEBOOK_TXT_NAME_MAX_LEN, full_name_length);
    //s_ebook_file_brief->detail_id = -1;	

#ifdef DRM_SUPPORT
    {
        MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
        file_handle = MMIAPIFMM_CreateFile(s_ebook_file_brief->full_name_wstr, 
            SFS_MODE_OPEN_EXISTING|SFS_MODE_READ, 0, 0);
        s_ebook_file_brief->file_size = MMIAPIFMM_GetFileSize(file_handle);
        MMIAPIFMM_CloseFile(file_handle);
    }
#else
    s_ebook_file_brief->file_size = file_size;
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : alloc s_ebook_file_brief
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_CreateFileBrief( void )
{
    if (PNULL == s_ebook_file_brief)
    {
        s_ebook_file_brief = SCI_ALLOC_APPZ(sizeof(MMIEBOOK_FILE_BRIEF_T));
        //SCI_ASSERT(s_ebook_file_brief != PNULL);/*assert verified*/
    }

    SCI_MEMSET(s_ebook_file_brief, 0, sizeof(MMIEBOOK_FILE_BRIEF_T));
}

/*****************************************************************************/
//  Description : free s_ebook_file_brief
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_DestroyFileBrief( void )
{
    if (PNULL != s_ebook_file_brief)
    {
        SCI_FREE(s_ebook_file_brief);
        s_ebook_file_brief=PNULL;
    }
}

/*最近阅读的电子书列表文件操作相关接口,begin*/
/*****************************************************************************/
//  Description : 打开最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_OpenLatestListFile(MMIFILE_DEVICE_E dev, const char * mode)
{
    MMIFILE_HANDLE          file_handle = PNULL;
    wchar                   full_path[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    uint16                  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32                  opt = 0;
        
    //SCI_ASSERT(mode != PNULL);/*assert verified*/
    if(PNULL == mode)
    {
        return FALSE;
    }
    
    opt = GetFileOptMode(mode);
    if( MMIEBOOK_GetFileFullPathUCS2
              (
	    dev, 
	    MMIMULTIM_DIR_EBOOK, 
	    MMIAPICOM_Wstrlen(MMIMULTIM_DIR_EBOOK),
	    MMIEBOOK_LATEST_LIST_FILE_NAME_UCS2, 
	    MMIEBOOK_LATEST_LIST_FILE_NAME_UCS2_LEN,
	    full_path, 
	    &full_path_len
	    ))
    {
	    file_handle = MMIAPIFMM_CreateFile(
		    full_path, 
		    opt, 
		    PNULL, 
		    PNULL
		    ); 

	    if (SFS_INVALID_HANDLE == file_handle )
	    {
	        //SCI_TRACE_LOW:"MMIEBOOK_OpenLatestListFile file_handle is SFS_INVALID_HANDLE"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_826_112_2_18_2_15_15_12,(uint8*)"");
	        return FALSE;
	    }
	    
	    //SCI_ASSERT(PNULL == s_ebook_latest_list_file_handle);/*assert verified*/
	    //SCI_ASSERT(PNULL != file_handle);/*assert verified*/
	    
	    s_ebook_latest_list_file_handle = file_handle;
	    
	    return TRUE;
    }
    else
    {
   	    return FALSE;
    }
}

/*****************************************************************************/
//  Description : 创建最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CreateLatestListFile(MMIFILE_DEVICE_E dev)
{
//     MMIFILE_HANDLE          file_handle = PNULL;
//     wchar                   full_path[MMIFILE_FULL_PATH_MAX_LEN] = {0};
//     uint16                  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
	BOOLEAN                 write_ok = FALSE;
	
    if (MMIEBOOK_OpenLatestListFile(dev,"w+"))
    {
        //写数据  	
        write_ok = MMIEBOOK_WriteLatestListFile(//写文件版本信息
            MMIEBOOK_GetLatestListFileHandle(),
            MMIEBOOK_LATEST_LIST_FILE_VER_ID,
            MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME
            );

        if (write_ok)
        {
            MMIEBOOK_LATEST_LIST_T*    latest_list_ptr = PNULL;//存储了最近10本电子书的信息

	        latest_list_ptr = SCI_ALLOC_APP(sizeof(MMIEBOOK_LATEST_LIST_T));
            //SCI_ASSERT(PNULL != latest_list_ptr);/*assert verified*/
			if(PNULL != latest_list_ptr)
			{
	            SCI_MEMSET(latest_list_ptr, 0, sizeof(MMIEBOOK_LATEST_LIST_T));

	            write_ok = MMIEBOOK_WriteLatestListFile(//初始化最近阅读的电子书列表信息
	                MMIEBOOK_GetLatestListFileHandle(),
	                MMIEBOOK_LATEST_LIST_FILE_INFO,
	                latest_list_ptr
	                );

		        SCI_FREE(latest_list_ptr);
		        latest_list_ptr = PNULL;
			}
			else
			{
				write_ok = FALSE;
			}
        }
        //已经完成
        MMIEBOOK_CloseLatestListFile();
    }

    return write_ok;
}

/*****************************************************************************/
//  Description : 最近阅读的电子书列表文件是否打开
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE:已打开 FALSE:未打开
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsLatestListFileOpen(void)
{
    if (s_ebook_latest_list_file_handle != PNULL)
    {
        return TRUE;
    }
    return FALSE;   
}
/*****************************************************************************/
//  Description : 得到指定设备上最近阅读的电子书列表文件的句柄
//  Global resource dependence : 
//  Author: zhaohui
//  Note:文件已经打开
//  Return:文件句柄
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIEBOOK_GetLatestListFileHandle(void)
{
    return s_ebook_latest_list_file_handle;
}
/*****************************************************************************/
//  Description : 关闭最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CloseLatestListFile(void)
{
    BOOLEAN flag = TRUE;
    
	MMIEBOOK_FFS_fclose(s_ebook_latest_list_file_handle);
	s_ebook_latest_list_file_handle = PNULL;
    
    return flag;
}

/*****************************************************************************/
//  Description : 读取最近阅读的电子书列表
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_ReadLatestListFile(
										   MMIFILE_HANDLE file_handle,
										   MMIEBOOK_LATEST_LIST_FILE_OPT_E type,//IN: 读哪一部分
										   void *param
										   )
{
    int ret = 0;
    uint32 str_len = 0;
    BOOLEAN result = TRUE;
	
    //SCI_ASSERT(PNULL != file_handle);/*assert verified*/
    //SCI_ASSERT(PNULL != param);/*assert verified*/
    if(PNULL == file_handle ||PNULL == param)
    {
    	return FALSE;
    }
	
    //SCI_TRACE_LOW:"MMIEBOOK_ReadLatestListFile:  type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_954_112_2_18_2_15_15_13,(uint8*)"d",type);
    switch(type)
    {
    case MMIEBOOK_LATEST_LIST_FILE_VER_ID://文件版本信息
        {
            uint16 *ver_id_ptr  = PNULL;
            
            ver_id_ptr = (uint16 *)param;
            
            ret = MMIEBOOK_FFS_fseek(file_handle, 0, MMIFILE_SEEK_BEGIN);
			
			if (0 == ret )
			{
				str_len = MMIEBOOK_FFS_fread(ver_id_ptr, MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME_LEN, 1, file_handle);
				if (str_len != MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME_LEN)
				{
					//SCI_TRACE_LOW:"MMIEBOOK_ReadLatestListFile FILE_VER_ID fail ! str_len is %d"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_970_112_2_18_2_15_15_14,(uint8*)"d",str_len);
					result = FALSE;
				}
			}
			else
			{
				//SCI_TRACE_LOW:"MMIEBOOK_ReadLatestListFile: MMIEBOOK_FFS_fseek error ret is %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_976_112_2_18_2_15_15_15,(uint8*)"d",ret);
				result = FALSE;
			}
			
        }   
        break;
		
    case MMIEBOOK_LATEST_LIST_FILE_INFO://最近阅读的电子书列表信息
        {
            MMIEBOOK_LATEST_LIST_T *latest_list_ptr = PNULL;
            
            latest_list_ptr = (MMIEBOOK_LATEST_LIST_T* )param;
            
            ret = MMIEBOOK_FFS_fseek(file_handle, MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME_LEN, MMIFILE_SEEK_BEGIN);            

            if (0 == ret )
            {
                str_len = MMIEBOOK_FFS_fread(latest_list_ptr, sizeof(MMIEBOOK_LATEST_LIST_T), 1, file_handle);
                if (str_len != sizeof(MMIEBOOK_LATEST_LIST_T))
                {
                    //SCI_TRACE_LOW:"MMIEBOOK_ReadLatestListFile FILE_INFO fail ! str_len is %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_996_112_2_18_2_15_15_16,(uint8*)"d",str_len);
                    result = FALSE;
                }
                else if(latest_list_ptr->total_item_num>0)//NEWMS00132985 在查询最近文件的时候，查看电子书是否还在，不在的话从最近文件中删除
                {
                    int32 index = 0;
                    int32 i = 0;
                    for(index= (latest_list_ptr->total_item_num-1); index>=0; index--)/*lint !e64 */
                    {
                        if (!MMIAPIFMM_IsFileExist(latest_list_ptr->item_brief[index].full_name_wstr,  latest_list_ptr->item_brief[index].full_name_len))
                        {
                            
                            SCI_MEMSET(&latest_list_ptr->item_brief[index], 0X0, sizeof(MMIEBOOK_FILE_BRIEF_T));
                            for(i=index; i<latest_list_ptr->total_item_num-1; i++)/*lint !e574 !e737*/
                            {
                                SCI_MEMCPY(&latest_list_ptr->item_brief[i],&latest_list_ptr->item_brief[i+1], sizeof(MMIEBOOK_FILE_BRIEF_T))
                            }
                            latest_list_ptr->total_item_num--;
                            MMIEBOOK_WriteLatestListFile(
			                        MMIEBOOK_GetLatestListFileHandle(),
			                        MMIEBOOK_LATEST_LIST_FILE_INFO,
			                        latest_list_ptr
                                    );

                        }
                    }
                }
            }
            else
            {
                //SCI_TRACE_LOW:"MMIEBOOK_ReadLatestListFile: MMIEBOOK_FFS_fseek error ret is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1026_112_2_18_2_15_15_17,(uint8*)"d",ret);
                result = FALSE;
            }

        }
        break;

    default:
        result = FALSE;
        break;
    }
    return result;         
}
                              
/*****************************************************************************/
//  Description : 读取最近阅读的电子书列表
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_WriteLatestListFile(
											MMIFILE_HANDLE file_handle,
											MMIEBOOK_LATEST_LIST_FILE_OPT_E type,//IN: 读哪一部分
											void *param
											)
{
    int ret = 0;
    BOOLEAN result = TRUE;
	
    //SCI_ASSERT(PNULL != file_handle);/*assert verified*/
    //SCI_ASSERT(PNULL != param);/*assert verified*/
	if(NULL == file_handle ||PNULL== param)
	{
		return FALSE;
	}
	
    //SCI_TRACE_LOW:"MMIEBOOK_WriteLatestListFile type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1061_112_2_18_2_15_15_18,(uint8*)"d",type);
    switch(type)
    {
    case MMIEBOOK_LATEST_LIST_FILE_VER_ID://文件版本信息
        {
            uint16 *ver_id_ptr  = PNULL;
            
            ver_id_ptr = (uint16 *)param;
            
            ret = MMIEBOOK_FFS_fseek(file_handle, 0, MMIFILE_SEEK_BEGIN);
			if (0 == ret )
			{
				result = MMIEBOOK_FFS_fwrite(ver_id_ptr, MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME_LEN, 1, file_handle);				
			}
			else
			{
				//SCI_TRACE_LOW:"MMIEBOOK_WriteLatestListFile: MMIEBOOK_FFS_fseek error ret is %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1077_112_2_18_2_15_15_19,(uint8*)"d",ret);
				result = FALSE;
			}
        }   
        break;
		
    case MMIEBOOK_LATEST_LIST_FILE_INFO://最近阅读的电子书列表信息
        {
            MMIEBOOK_LATEST_LIST_T *latest_list_ptr = PNULL;
            
            latest_list_ptr = (MMIEBOOK_LATEST_LIST_T *)param;
            
            ret = MMIEBOOK_FFS_fseek(file_handle, MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME_LEN, MMIFILE_SEEK_BEGIN);            
            if (0 == ret)
			{
				result = MMIEBOOK_FFS_fwrite(latest_list_ptr, sizeof(MMIEBOOK_LATEST_LIST_T), 1, file_handle);
			}
			else
			{
				//SCI_TRACE_LOW:"MMIEBOOK_WriteLatestListFile: MMIEBOOK_FFS_fseek error ret is %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1096_112_2_18_2_15_15_20,(uint8*)"d",ret);
				result = FALSE;
			}
        }   
        break;
		
    default:
        result = FALSE;
        break;
    }
	
    //SCI_TRACE_LOW:"MMIEBOOK_WriteLatestListFile result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1107_112_2_18_2_15_15_21,(uint8*)"d",result);
    return result;         
}
                                        
/*****************************************************************************/
//  Description : 指定设备是否有最近阅读的电子书列表文件是否存在且是可用的(即版本一致),
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->可用的
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsLatestListFileOK(MMIFILE_DEVICE_E dev)
{
    BOOLEAN result = FALSE;
    
    if (MMIEBOOK_IsExistLatestListFile(dev))
    {
        if (MMIEBOOK_IsLatestListFileVersionVaild(dev))
        {
            result = TRUE;
        }
        else
        {
            //删除老的文件
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 指定设备是否有详情文件存在
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->存在
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsExistLatestListFile(MMIFILE_DEVICE_E dev)
{
    wchar               full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN+1;
//     MMIFILE_FIND_DATA_T  find_data = {0};
    BOOLEAN             isExist = FALSE;
    
    if(MMIEBOOK_GetFileFullPathUCS2(
		dev,
		MMIMULTIM_DIR_EBOOK,
		MMIAPICOM_Wstrlen(MMIMULTIM_DIR_EBOOK),
		MMIEBOOK_LATEST_LIST_FILE_NAME_UCS2,
		MMIEBOOK_LATEST_LIST_FILE_NAME_UCS2_LEN,
		full_path,
		&full_path_len
		))

    {
        isExist = MMIAPIFMM_IsFileExist((const wchar *)full_path, full_path_len);
    }
    
    return isExist;
}

/*****************************************************************************/
//  Description : 最近阅读的电子书列表文件是否与当前软件版本一致
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->版本一致
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsLatestListFileVersionVaild(MMIFILE_DEVICE_E dev)
{
    BOOLEAN     result = FALSE;
    uint8       ver_info[MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME_LEN + 1] = {0};      

    if (MMIEBOOK_OpenLatestListFile(dev, "r+"))
    {
        //读版本信息，判断版本是否一致，若不一致则删除
        MMIEBOOK_ReadLatestListFile(
            MMIEBOOK_GetLatestListFileHandle(),
            MMIEBOOK_LATEST_LIST_FILE_VER_ID,
            ver_info /*lint !e64*/
            );/*lint !e64*/  
        //若此时读写错误,也会进入下面"不一致,删除"的处理
    
        MMIEBOOK_CloseLatestListFile();
    
        if ((0 != strncmp((char*)ver_info,(char*)MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME,MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME_LEN)))
        {
            //打开失败,或版本不一致，删除
            MMIEBOOK_DeleteLatestListFile(dev);
            //SCI_TRACE_LOW:"MMIEBOOK_IsLatestListFileVersionVaild version is wrong"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1190_112_2_18_2_15_15_22,(uint8*)"");
        }
        else
        {
            result = TRUE;
        }
    }

    //SCI_TRACE_LOW:"MMIEBOOK_IsLatestListFileVersionVaild result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1198_112_2_18_2_15_15_23,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : 删除最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_DeleteLatestListFile(
                    MMIFILE_DEVICE_E dev
                    )
{
    wchar                   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16                  full_path_len = MMIFILE_FULL_PATH_MAX_LEN+1;
    BOOLEAN                 result = FALSE;

    if (MMIEBOOK_GetFileFullPathUCS2(
        dev, 
        MMIMULTIM_DIR_EBOOK, 
        MMIAPICOM_Wstrlen(MMIMULTIM_DIR_EBOOK),
        MMIEBOOK_LATEST_LIST_FILE_NAME_UCS2, 
        MMIEBOOK_LATEST_LIST_FILE_NAME_UCS2_LEN,
        full_path, 
        &full_path_len
        ))
    {
	    if (MMIAPIFMM_DeleteFileSyn(
	            full_path, 
	            full_path_len
	            ))
	    {
	        s_ebook_latest_list_file_handle = NULL;
	        result = TRUE;
	    }
    }
    return result;
}

/*最近阅读的电子书列表文件操作相关接口,end*/

/*****************************************************************************/
//  Description : 判断指定的文件时候在使用和已经被打开
//  Global resource dependence : 
//  Author: 
//  Parameter: full_path_name_ptr[IN]:指定的文件名
//             full_path_name_len[IN]:指定文件的全路径名长度
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
#if 0
PUBLIC BOOLEAN MMIAPIEBOOK_IsFileUsed(const wchar *full_path_name_ptr,
									  const uint16 full_path_name_len
                                     )
{
    wchar    full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16   full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN  result = FALSE;

    if(PNULL == s_ebook_show_file_handle || PNULL == s_ebook_file_brief || PNULL == full_path_name_ptr || 0 ==full_path_name_len)
	{
		//no file opened, or not specify file

		//SCI_TRACE_LOW:"MMIAPIEBOOK_IsFileUsed  %d, %d ,%d , %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1261_112_2_18_2_15_15_24,(uint8*)"dddd", s_ebook_show_file_handle,s_ebook_file_brief,full_path_name_ptr,full_path_name_len);
		return FALSE;
	}
	
	if (s_ebook_file_brief->full_name_len > 0)
	{

		if(!MMIAPICOM_Wstrcmp(full_path_name_ptr, s_ebook_file_brief->full_name_wstr))
		{
			result = TRUE;
		}
	}
	else
	{
		/*result = MMIEBOOK_GetFileFullPathUCS2
			(
			s_ebook_file_brief->file_dev, 
			MMIMULTIM_DIR_EBOOK, 
			MMIAPICOM_Wstrlen(MMIMULTIM_DIR_EBOOK),		
			s_ebook_file_brief->name_wstr,
			s_ebook_file_brief->name_len,
			full_path, 
			&full_path_len
			);
		if(result && (!MMIAPICOM_Wstrcmp(full_path_name_ptr, full_path)))
		{
			result = TRUE;
		}*/
	}
	
	//SCI_TRACE_LOW:"MMIAPIEBOOK_IsFileUsed  result %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1291_112_2_18_2_15_16_25,(uint8*)"d", result);
	return result;
}
#endif
//书签文件部分
/*****************************************************************************/
//  Description : 打开电子书书签文件
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_OpenBookmarkFile(
                                         wchar* full_path,  //in
                                         const char* mode   //in
                                         )
{
    MMIFILE_HANDLE          file_handle = PNULL;
    uint32                  opt = 0;
        
    //SCI_ASSERT(mode != PNULL);/*assert verified*/
    if(mode == PNULL)
    {
        return FALSE;
    }
    
    if(s_ebook_bookmark_file_handle)
    {
       return TRUE;
    }
    opt = GetFileOptMode(mode);

    file_handle = MMIAPIFMM_CreateFile(
		full_path, 
		opt, 
		PNULL, 
		PNULL
		);
	    
    if (SFS_INVALID_HANDLE == file_handle )
    {
        //SCI_TRACE_LOW:"Bookmark File open fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1330_112_2_18_2_15_16_26,(uint8*)"");
        return FALSE;
    }
    
    s_ebook_bookmark_file_handle = file_handle;
	    
    return TRUE;
}

/*****************************************************************************/
//  Description : 电子书书签文件是否打开
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE:已打开 FALSE:未打开
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsBookmarkFileOpen(void)
{
    if (s_ebook_bookmark_file_handle != PNULL)
    {
        return TRUE;
    }
    return FALSE;   
}

/*****************************************************************************/
//  Description : 得到指定设备上最近阅读的电子书列表文件的句柄
//  Global resource dependence : 
//  Author: zhaohui
//  Note:文件已经打开
//  Return:文件句柄
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIEBOOK_GetBookmarkFileHandle(void)
{
    return s_ebook_bookmark_file_handle;
}

/*****************************************************************************/
//  Description : 关闭最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CloseBookmarkFile(void)
{
    BOOLEAN flag = TRUE;
	wchar              bookmark_file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
//     uint16             bookmark_file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN; 
	MMIEBOOK_FILE_BRIEF_T* ebook_show_file_brief_ptr=PNULL;
    
	MMIEBOOK_FFS_fclose(s_ebook_bookmark_file_handle);
	s_ebook_bookmark_file_handle = PNULL;

	//设置书签文件为隐藏属性
	ebook_show_file_brief_ptr = MMIEBOOK_GetFileBrief();
    MMIEBOOK_GetBookmarkFileName(ebook_show_file_brief_ptr->full_name_wstr,bookmark_file_full_path);
    
    if (MMIAPIFMM_IsFileExist(bookmark_file_full_path,MMIAPICOM_Wstrlen(bookmark_file_full_path)))
    {
    	MMIAPIFMM_SetAttr(bookmark_file_full_path, MMIAPICOM_Wstrlen(bookmark_file_full_path), TRUE, FALSE, FALSE, FALSE);
    }
    
    return flag;
}

/*****************************************************************************/
//  Description : 读取电子书书签文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_ReadBookmarkFile(
											MMIFILE_HANDLE file_handle,         //in
											MMIEBOOK_BOOKMARK_FILE_OPT_E type,  //IN,读哪一部分
                                            void *param                         //in,写入的内容,
                                            )
{
    int ret = 0;
    uint32 str_len = 0;
    BOOLEAN result = TRUE;
	
    //SCI_ASSERT(PNULL != file_handle);/*assert verified*/
    //SCI_ASSERT(PNULL != param);/*assert verified*/
	if(PNULL == file_handle ||PNULL == param )
	{
		return FALSE;
	}
	
//	SCI_TRACE_LOW("MMIEBOOK_ReadLatestListFile:  type is %d",type);    
    switch (type)
    {
        case MMIEBOOK_BOOKMARK_FILE_CUR_READ_OFFSET:
            {
                int32* cur_read_offset_ptr = PNULL;

                //SCI_ASSERT(PNULL != param);/*assert verified*/
                cur_read_offset_ptr = (int32*)param;
                ret = MMIEBOOK_FFS_fseek(file_handle, 0, MMIFILE_SEEK_BEGIN);            
                if (0 == ret)
                {
                    str_len = MMIEBOOK_FFS_fread(cur_read_offset_ptr, sizeof(int32), 1, file_handle);
                    if (str_len != sizeof(int32))
                    {
                        //SCI_TRACE_LOW:"MMIEBOOK_ReadBookmarkFile FILE_INFO fail ! str_len is %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1427_112_2_18_2_15_16_27,(uint8*)"d",str_len);
                        result = FALSE;
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIEBOOK_ReadBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1433_112_2_18_2_15_16_28,(uint8*)"d",ret);
                    result = FALSE;
                }
            }
            break;

        case MMIEBOOK_BOOKMARK_FILE_INFO:
            {
                MMIEBOOK_BOOKMARK_T* bookmark_info_ptr = PNULL;

                //SCI_ASSERT(PNULL != param);/*assert verified*/
                bookmark_info_ptr = (MMIEBOOK_BOOKMARK_T*)param;

                ret = MMIEBOOK_FFS_fseek(file_handle, sizeof(int32), MMIFILE_SEEK_BEGIN);            
                if (0 == ret)
                {
                    str_len = MMIEBOOK_FFS_fread(bookmark_info_ptr, sizeof(MMIEBOOK_BOOKMARK_T), 1, file_handle);
                    if (str_len != sizeof(MMIEBOOK_BOOKMARK_T))
                    {
                        //SCI_TRACE_LOW:"MMIEBOOK_ReadBookmarkFile FILE_INFO fail ! str_len is %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1452_112_2_18_2_15_16_29,(uint8*)"d",str_len);
                        result = FALSE;
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIEBOOK_ReadBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1458_112_2_18_2_15_16_30,(uint8*)"d",ret);
                    result = FALSE;
                }
            }
            break;

        default:
            result = FALSE;
            break;
    }
    


    return result;         
}
                              
/*****************************************************************************/
//  Description : 写电子书书签文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_WriteBookmarkFile(
											MMIFILE_HANDLE file_handle,         //in
											MMIEBOOK_BOOKMARK_FILE_OPT_E type,  //IN,读哪一部分
                                            void *param                         //in,写入的内容,
											)
{
    int ret = 0;
    BOOLEAN result = TRUE;

	//不要乱用assert!!!
    //SCI_ASSERT(PNULL != file_handle);/*assert verified*/
    //SCI_ASSERT(PNULL != param);/*assert verified*/

	if(PNULL == file_handle ||PNULL == param )
	{
		return FALSE;
	}
	
//    SCI_TRACE_LOW("MMIEBOOK_WriteLatestListFile type is %d",type);   

    switch (type)
    {
        case MMIEBOOK_BOOKMARK_FILE_CUR_READ_OFFSET:
            {
                int32* cur_read_offset_ptr = PNULL;

                //SCI_ASSERT(PNULL != param);/*assert verified*/
                cur_read_offset_ptr = (int32*)param;
                ret = MMIEBOOK_FFS_fseek(file_handle, 0, MMIFILE_SEEK_BEGIN);            
                if (0 == ret)
                {
                    result = MMIEBOOK_FFS_fwrite(cur_read_offset_ptr, sizeof(int32), 1, file_handle);
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIEBOOK_WriteBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1514_112_2_18_2_15_16_31,(uint8*)"d",ret);
                    result = FALSE;
                }
            }
            break;

        case MMIEBOOK_BOOKMARK_FILE_INFO:
            {
                MMIEBOOK_BOOKMARK_T* bookmark_info_ptr = PNULL;

                //SCI_ASSERT(PNULL != param);/*assert verified*/
                bookmark_info_ptr = (MMIEBOOK_BOOKMARK_T*)param;
                
                ret = MMIEBOOK_FFS_fseek(file_handle, sizeof(int32), MMIFILE_SEEK_BEGIN);            
                if (0 == ret)
                {
                    result = MMIEBOOK_FFS_fwrite(bookmark_info_ptr, sizeof(MMIEBOOK_BOOKMARK_T), 1, file_handle);
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIEBOOK_WriteBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1534_112_2_18_2_15_16_32,(uint8*)"d",ret);
                    result = FALSE;
                }
            }
            break;

        default:
            result = FALSE;
            break;
    }
            
    //SCI_TRACE_LOW:"MMIEBOOK_WriteBookmarkFile result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_FILE_1545_112_2_18_2_15_16_33,(uint8*)"d",result);
    return result;         
}

/*****************************************************************************/
//  Description : MMIEBOOK_GetBookmarkFileName
//  Global resource dependence :
//   Author: hui.zhao
//  Note:   
/*****************************************************************************/
PUBLIC void MMIEBOOK_GetBookmarkFileName(
    wchar* src_full_path_ptr,    //in                       
    wchar* des_full_path_ptr    //out
)
{
    wchar                   ucs_name_ptr[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16                  ucs_name_len = 0;

    //SCI_ASSERT(PNULL != src_full_path_ptr);/*assert verified*/
    //SCI_ASSERT(PNULL != des_full_path_ptr);/*assert verified*/
    if(src_full_path_ptr == PNULL ||des_full_path_ptr == PNULL )
    {
        return;
    } 
    
    MMIAPICOM_GetFileNameAndSuffix(src_full_path_ptr,PNULL,ucs_name_ptr);
    ucs_name_len = MMIAPICOM_Wstrlen( ucs_name_ptr);
    
	MMI_WSTRNCPY(des_full_path_ptr,MMIEBOOK_TXT_NAME_MAX_LEN,ucs_name_ptr,
     ucs_name_len,ucs_name_len);
  	MMI_WSTRNCPY(des_full_path_ptr + ucs_name_len,MMIEBOOK_TXT_NAME_MAX_LEN,MMIEBOOK_BOOKMARK_FILE_SUFFIX_NAME_UCS2,
       MMIEBOOK_BOOKMARK_FILE_SUFFIX_NAME_UCS2_LEN,MMIEBOOK_BOOKMARK_FILE_SUFFIX_NAME_UCS2_LEN);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//   Author: renyi.hu
//  Note:   
/*****************************************************************************/
/*PUBLIC BOOLEAN MMIEBOOK_IsExistZero(
					    MMIFILE_HANDLE file_handle,                          
					    uint32 file_size ,
					    GUI_CHAR_CODE_TYPE_E type
					)
{
	BOOLEAN result = FALSE;
    uint32 len = 0;
    MMIFILE_ERROR_E file_read_error = SFS_NO_ERROR;
	uint32 check_size=0;
	uint32 offset=0;
	uint8* buff=PNULL;
	uint32  i=0;
	
    if(type == GUI_CHAR_CODE_ANSI ||type == GUI_CHAR_CODE_ANSI_BIG5)
	{
		if(file_size<=MMIEBOOK_CHECK_SIZE)
		{
			check_size=file_size;
		}
		else
		{
			check_size=MMIEBOOK_CHECK_SIZE;
		}
		offset=MMIAPIFMM_SetFilePointer(file_handle, 0, MMIFILE_SEEK_BEGIN);

		buff=SCI_ALLOC_APP(check_size+1);
		if(buff==PNULL)
		{
			result = TRUE;
		}
		else
		{
			SCI_MEMSET(buff,0,check_size+1);

			file_read_error = MMIAPIFMM_ReadFile(file_handle, buff, check_size, &len, PNULL);

		    if (file_read_error != SFS_NO_ERROR)
		    {
		        result = TRUE;
		    }
			else
			{
				for(i=0;i<len;i++)
				{
					if(buff[i]==0)
					{
						result = TRUE;
						break;
					}
				}
			}
			
			if((!result) && check_size<file_size)
			{
				offset=MMIAPIFMM_SetFilePointer(file_handle, file_size-check_size, MMIFILE_SEEK_BEGIN);
				file_read_error = MMIAPIFMM_ReadFile(file_handle, buff, check_size, &len, PNULL);
				if (file_read_error != SFS_NO_ERROR)
			    {
			        result = TRUE;
			    }
				else
				{
					for(i=0;i<len;i++)
					{
						if(buff[i]==0)
						{
							result = TRUE;
							break;
						}
					}
				}
			}
		}
		
		if(buff!=PNULL)
		{
			SCI_FREE(buff);
		}
	}
	else//其他编码目前还不需要判断，以后需要时添加
	{
	}

	return result;
}*/

/*****************************************************************************/
//  Description : MMIEBOOK_isEbookTypeFile
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_isEbookTypeFile(const wchar* name_ptr, uint32 name_length)
{
    uint16              suffix_len                                = MMIFMM_FILENAME_LEN;
    wchar              *suffix_wstr_ptr                           = PNULL;
    MMIFMM_FILE_TYPE_E file_type=MMIFMM_FILE_TYPE_NORMAL;
    
    suffix_wstr_ptr = SCI_ALLOCA((MMIFMM_FILENAME_LEN + 1) * sizeof(wchar));
    if (PNULL == suffix_wstr_ptr)
    {
        return  FALSE;
    }

    SCI_MEMSET(suffix_wstr_ptr, 0x00, (MMIFMM_FILENAME_LEN + 1) * sizeof(wchar));

    MMIAPIFMM_SplitFileName(name_ptr,name_length, PNULL, PNULL, suffix_wstr_ptr, &suffix_len);

    file_type = MMIAPIFMM_ConvertFileType(suffix_wstr_ptr, suffix_len);

    SCI_FREE(suffix_wstr_ptr);
    if(MMIFMM_FILE_TYPE_EBOOK==file_type)
    {
        return TRUE;
    }
    
    return  FALSE;
}
#endif//ENG_SUPPORT

/*****************************************************************
 **  File Name: 	brw_plinterface.c 							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:在这个文件中实现同平台相关的接口,brw的其它文件  *
 **              统一使用这里的接口                              *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
**  DATE			NAME			DESCRIPTION				     *
**  07/04/2009		yuzhi.chen      Create. 					 *
*****************************************************************/
#include "mmi_app_browser_trc.h"
#include "brw_plinterface.h"
#include "mmifmm_export.h"
#include "mmicom_time.h"//时间相关的接口
//#include "block_mem_def.h"
//#include "block_mem.h"
#include "mmibrowser_dorado_api.h"
#include "ctype.h"
#include "ui_layer.h"

#define	BROWSER_POOL_NAME	"BRW_POOL"
//#define BRW_MEM_DEBUG
#define SLASH_WCHAR 0x002f	//'/'
#define BACKSLASH_WCHAR 0x005c //'\'
#define BRW_MAX_FILE_PATH_LEN SFS_MAX_PATH
#define BRW_FILE_COPY_EXCHANGE_BUF_SIZE 20480

#ifdef BRW_MEM_DEBUG
typedef struct mem_check {
    const char *filename;
    int line;
    int size;
    void *address;
}MEM_INFO_T;

LOCAL MEM_INFO_T g_mem_alloc_arr[2000] = {0};
static SCI_MUTEX_PTR    s_mem_debug_mutex = 0;
#endif

#define DLMALLOC_SUPPORT//MOCOR_MPALLOC_SUPPORT

#ifdef MOCOR_MPALLOC_SUPPORT
    MPOOL_HANDLE g_browser_mem_handle = 0;
#elif defined DLMALLOC_SUPPORT
    #include "cfl_dlmalloc.h"
    CFL_MSPACE_HANDLE g_browser_mem_handle = 0;
#endif 
#ifndef PLATFORM_SC6530
LOCAL void *s_dorado_memory_ptr = PNULL;/*the block memory that alloc by dorado browser*/
#endif


#ifdef BRW_MEM_DEBUG
LOCAL void *BRWMem_malloc_debug(int size, const char *pfile, int line);
LOCAL void BRWMem_add_push_node(MEM_INFO_T *new_mem);
LOCAL void BRWMem_delete_push_node(void *address);
LOCAL void BRWMem_free_debug(void *ptr,char *pfile, int line);
PUBLIC void BRWMem_print_mem(void);
#endif

#if 0
LOCAL BOOLEAN SplitFullPath(
						 uint16 *full_path_ptr,//[in]
						 uint16 path_max_len, //[in]
						 uint16 name_max_len, //[in]
						 uint16 *path_ptr,//[out]
						 uint16 *name_ptr//[out]
						 );
#endif

extern int stricmp (const char *a, const char *b);
extern int strnicmp (const char *a, const char *b, size_t c);
//extern BOOLEAN MMIAPIFMM_GetDeviceTypeStatus(MMIFILE_DEVICE_E memory_type);

/*
 *	desc:check if folder is exist
 *  param:full_path_ptr  full path folder string
 *        full_path_len  length of path
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWPL_IsFolderExist(const uint16 *full_path_ptr,uint16 full_path_len)
{
    if(full_path_ptr!=PNULL && 0!=full_path_len)
    {
        return MMIAPIFMM_IsFolderExist(full_path_ptr,full_path_len); 
    }
    else
    {
        //BRWDebug_Printf:"BRWPL_IsFolderExist,full_path_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_86_112_2_18_2_7_22_743,(uint8*)"d",full_path_len);
        return FALSE;
    }    
}

PUBLIC BRWPL_FFS_HANDLE BRWPL_CreatFile(const uint16 *full_path_ptr)
{
	BRWPL_FFS_HANDLE ffs_handler=0;

	ffs_handler = SFS_CreateFile(full_path_ptr, (uint32)(SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS), NULL, NULL);
	return ffs_handler;
}

#if 0
LOCAL BOOLEAN SplitFullPath(
						 uint16 *full_path_ptr,//[in]
						 uint16 path_max_len, //[in]
						 uint16 name_max_len, //[in]
						 uint16 *path_ptr,//[out]
						 uint16 *name_ptr//[out]
						 )
{
	uint16 *wch_ptr = 0;
	uint32 len = 0;
	uint32 output_len = (path_max_len>name_max_len)?name_max_len:path_max_len;

	if(0==full_path_ptr || 0==path_ptr || 0==name_ptr || 0==output_len)
	{
		return FALSE;
	}

	len = BRW_WStrlen(full_path_ptr);
	if(len>output_len)
	{
		//BRWDebug_Printf:"SplitFullPath, err full_path_ptr length err"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_119_112_2_18_2_7_22_744,(uint8*)"");
		return FALSE;
	}
	wch_ptr = full_path_ptr+BRW_WStrlen(full_path_ptr)-1;
	while(len!=0)
	{
		if(*wch_ptr==SLASH_WCHAR || *wch_ptr==BACKSLASH_WCHAR)
		{
			break;
		}
		else
		{
			len--;
			wch_ptr--;
		}
	}
	if(len==0)
	{
		//BRWDebug_Printf:"SplitFullPath, err full_path_ptr path=0"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_137_112_2_18_2_7_22_745,(uint8*)"");
		return FALSE;
	}
	BRW_Memcpy(path_ptr,full_path_ptr,((len-1)*sizeof(uint16)));
	BRW_WstrCat(name_ptr,(wch_ptr+1));
	return TRUE;
}
#endif

PUBLIC BOOLEAN BRWPL_FileCopy(
							  uint16 *src_full_path_ptr, 
							  uint16 *dest_full_path_ptr
							  )
{
    uint8               *exchange_buf_ptr = 0;
    uint32              buffer_len = 0;
    uint32              src_file_size = 0;
    uint32              unread_file_size = 0;
    SFS_HANDLE          src_file_handle = 0;
    SFS_HANDLE          copyed_file_handle = 0;
    uint32              read_len = 0;
    uint32              write_len = 0;
    BOOLEAN             result = FALSE;

    if(src_full_path_ptr==0 || dest_full_path_ptr==0)
    {
        goto COPY_FAIL;
    }
    
    if(!BRWPL_GetFileSize(src_full_path_ptr, BRW_WStrlen(src_full_path_ptr), &src_file_size))
    {
        //SCI_TRACE_LOW:"BRWPL_FileCopy err BRWPL_GetFileSize failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_166_112_2_18_2_7_22_746,(uint8*)"");
        goto COPY_FAIL;
    }
    if(src_file_size == 0)
    {   
        //SCI_TRACE_LOW:"BRWPL_FileCopy err src_file_size=0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_171_112_2_18_2_7_22_747,(uint8*)"");
        goto COPY_FAIL;
    }
    //SCI_TRACE_LOW:"BRWPL_FileCopy src_file_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_174_112_2_18_2_7_22_748,(uint8*)"d",src_file_size);

    src_file_handle = SFS_CreateFile(src_full_path_ptr, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), PNULL, PNULL);
    if(src_file_handle==0)
    {
        //SCI_TRACE_LOW:"BRWPL_FileCopy err creat src_file_handle fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_179_112_2_18_2_7_22_749,(uint8*)"");
        goto COPY_FAIL;
    }
    copyed_file_handle = SFS_CreateFile(dest_full_path_ptr, (uint32)(SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS), PNULL, PNULL);
    if(copyed_file_handle==0)
    {
        //SCI_TRACE_LOW:"BRWPL_FileCopy err creat copyed_file_handle fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_185_112_2_18_2_7_22_750,(uint8*)"");
        goto COPY_FAIL;
    }

    unread_file_size = src_file_size;
    while(unread_file_size!=0)
    {
        if(unread_file_size>BRW_FILE_COPY_EXCHANGE_BUF_SIZE)
        {
            buffer_len = BRW_FILE_COPY_EXCHANGE_BUF_SIZE;
        }
        else
        {
            buffer_len = unread_file_size;
        }
        exchange_buf_ptr = BRWMem_Alloc(buffer_len);
        if(exchange_buf_ptr == 0)
        {
            //SCI_TRACE_LOW:"BRWPL_FileCopy  err alloc exchange buffer failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_203_112_2_18_2_7_22_751,(uint8*)"");
            goto COPY_FAIL;
        }
        if(SFS_NO_ERROR!=SFS_ReadFile(src_file_handle, exchange_buf_ptr, buffer_len, &read_len, 0))
        {
            //SCI_TRACE_LOW:"BRWPL_FileCopy err SFS_ReadFile fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_208_112_2_18_2_7_22_752,(uint8*)"");
            goto COPY_FAIL;
        }
        if(read_len==0)
        {
            //SCI_TRACE_LOW:"BRWPL_FileCopy err read_len=0!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_213_112_2_18_2_7_22_753,(uint8*)"");
            goto COPY_FAIL;
        }
        unread_file_size -= read_len;
        if(SFS_NO_ERROR!=SFS_WriteFile(copyed_file_handle,exchange_buf_ptr,read_len,&write_len,0))
        {
            //SCI_TRACE_LOW:"BRWPL_FileCopy err SFS_ReadFile fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_219_112_2_18_2_7_22_754,(uint8*)"");
            goto COPY_FAIL;
        }
        BRWMem_Free(exchange_buf_ptr);
        exchange_buf_ptr = 0;
    }
    result = TRUE;
COPY_FAIL:
    if(src_file_handle!=0)
    {
        SFS_CloseFile(src_file_handle);
    }
    if(copyed_file_handle!=0)
    {
        SFS_CloseFile(copyed_file_handle);
    }
    if(exchange_buf_ptr!=0)
    {
        BRWMem_Free(exchange_buf_ptr);
        exchange_buf_ptr = 0;
    }
    if(!result)
    {
        BRWPL_DelFileSyn(dest_full_path_ptr,BRW_WStrlen(dest_full_path_ptr));
    }
    return result;
}

/*
 *	desc:create folder
 *  param:full_path_ptr  full path folder string
 *        full_path_len  length of path
 *  return:TRUE if success else FALSE
 */
PUBLIC BOOLEAN BRWPL_CreateDir(const uint16 *full_path_ptr,uint16 full_path_len)
{
    if(full_path_ptr!=PNULL && 0!=full_path_len)
    {
        return MMIAPIFMM_CreateDir(full_path_ptr,full_path_len);
    }
    else
    {
        //BRWDebug_Printf:"BRWPL_CreateDir:full_path_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_260_112_2_18_2_7_22_755,(uint8*)"d",full_path_len);
        return FALSE;
    }
}
PUBLIC BOOLEAN BRWPL_DeleteDir(const uint16 *folder_path_ptr,const uint16 *folder_name_ptr)
{
    SFS_HANDLE      sfs_handle = 0;
    uint16          *full_path_ptr = 0;
    uint16          *find_path_ptr = 0;
    SFS_FIND_DATA_T fd = {0};

    if(folder_path_ptr!=PNULL)
    {
        full_path_ptr = BRWMem_Alloc(SFS_MAX_PATH*2);
        if(full_path_ptr==0)
        {
            //SCI_TRACE_LOW:"brw BRWPL_DeleteDir full_path_ptr fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_276_112_2_18_2_7_22_756,(uint8*)"");
            return FALSE;
        }
        find_path_ptr = BRWMem_Alloc(SFS_MAX_PATH*2);
        if(find_path_ptr==0)
        {
            //SCI_TRACE_LOW:"brw BRWPL_DeleteDir find_path_ptr fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_282_112_2_18_2_7_22_757,(uint8*)"");
            BRWMem_Free(full_path_ptr);
            return FALSE;
        }
        SCI_MEMSET(full_path_ptr, 0, SFS_MAX_PATH*2);
        SCI_MEMSET(find_path_ptr, 0, SFS_MAX_PATH*2);

        BRW_WstrCat(full_path_ptr,folder_path_ptr);
        BRWPL_WstrStrnCatWithAsciiStr(full_path_ptr,BRW_FILE_SLASH,SFS_MAX_PATH);
        BRW_WstrCat(full_path_ptr,folder_name_ptr);

        BRW_WstrCat(find_path_ptr,full_path_ptr);
        BRWPL_WstrStrnCatWithAsciiStr(find_path_ptr,BRW_FILE_SLASH,SFS_MAX_PATH);
        BRWPL_WstrStrnCatWithAsciiStr(find_path_ptr,"*",SFS_MAX_PATH);
        sfs_handle = SFS_FindFirstFile(find_path_ptr,&fd);
        if(sfs_handle!=0)
        {
            do
            {
                if((fd.attr & SFS_ATTR_DIR) > 0)
                {
                    //delete folder recursion
                    if(!BRWPL_DeleteDir(full_path_ptr, fd.name))
                    {
                        //SCI_TRACE_LOW:"brw BRWPL_DeleteDir BRWPL_DeleteDir fail!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_306_112_2_18_2_7_22_758,(uint8*)"");
                        SFS_FindClose(sfs_handle);
                        BRWMem_Free(full_path_ptr);
                        BRWMem_Free(find_path_ptr);
                        return FALSE;                   
                    }
                }
                else //delete file find
                {
                    uint16  *temp_path_ptr = BRWMem_Alloc(SFS_MAX_PATH*2);
                    if(temp_path_ptr == 0)
                    {
                        //SCI_TRACE_LOW:"brw BRWPL_DeleteDir temp_path_ptr fail!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_318_112_2_18_2_7_22_759,(uint8*)"");
                        SFS_FindClose(sfs_handle);
                        BRWMem_Free(full_path_ptr);
                        BRWMem_Free(find_path_ptr);
                        return FALSE;
                    }
                    SCI_MEMSET(temp_path_ptr, 0, SFS_MAX_PATH*2);
                    BRW_WstrCat(temp_path_ptr,full_path_ptr);
                    BRWPL_WstrStrnCatWithAsciiStr(temp_path_ptr,BRW_FILE_SLASH,SFS_MAX_PATH);
                    BRW_WstrCat(temp_path_ptr,fd.name);
                    if(SFS_NO_ERROR != SFS_DeleteFile(temp_path_ptr, 0))
                    {
                        //SCI_TRACE_LOW:"brw BRWPL_DeleteDir SFS_DeleteFile fail!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_330_112_2_18_2_7_22_760,(uint8*)"");
                        SFS_FindClose(sfs_handle);
                        BRWMem_Free(full_path_ptr);
                        BRWMem_Free(find_path_ptr);
                        BRWMem_Free(temp_path_ptr);
                        return FALSE;                   
                    }
                    BRWMem_Free(temp_path_ptr);
                }
            }while(SFS_NO_ERROR == SFS_FindNextFile(sfs_handle, &fd));
            SFS_FindClose(sfs_handle);
            //delete the folder input
            if(SFS_NO_ERROR !=SFS_DeleteDirectory(full_path_ptr))
            {
                SFS_FindClose(sfs_handle);
                BRWMem_Free(full_path_ptr);
                BRWMem_Free(find_path_ptr);
                return FALSE;
           }
        }
        else //empty folder, delete folder directly
        {
            SFS_FindClose(sfs_handle);
            if(SFS_NO_ERROR !=SFS_DeleteDirectory(full_path_ptr))
            {
                SFS_FindClose(sfs_handle);
                BRWMem_Free(full_path_ptr);
                BRWMem_Free(find_path_ptr);
                return FALSE;
           }
        }
        BRWMem_Free(full_path_ptr);
        BRWMem_Free(find_path_ptr);
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

PUBLIC BRWPL_FFS_HANDLE BRWPL_WriteFileAsync(
									   uint16 *full_path_ptr,
									   uint8 *buf_ptr,
									   uint32 write_size,
									   BRWPL_FFS_OVERLAPPED_T *overlapped_ptr
									   )
{
	BRWPL_FFS_HANDLE sfs_handler = 0;
	uint32 bytes_written = 0;
	SFS_ERROR_E	result = 0;

	if(0==full_path_ptr||0==buf_ptr||0==overlapped_ptr)
	{
		return 0;
	}
	sfs_handler = SFS_CreateFile(full_path_ptr, (uint32)(SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS), NULL, NULL);
	if(0==sfs_handler)
	{
		return 0;
	}
	result = SFS_WriteFile(sfs_handler,buf_ptr,write_size,&bytes_written,overlapped_ptr);
	if(SFS_ERROR_NONE==result||SFS_ERROR_IO_PENDING==result)
	{
		return sfs_handler;
	}
	else
	{
		SFS_CloseFile(sfs_handler);
		return 0;
	}
}
PUBLIC BOOLEAN BRWPL_CloseFile(BRWPL_FFS_HANDLE ffs_handler)
{
	if(0==ffs_handler)
	{
		return FALSE;
	}
	SFS_CloseFile(ffs_handler);
	return TRUE;
}
PUBLIC BOOLEAN BRWPL_FindClose(BRWPL_FFS_HANDLE ffs_handler)
{
	if(0==ffs_handler)
	{
		return FALSE;
	}
	SFS_FindClose(ffs_handler);
	return TRUE;
}
PUBLIC BOOLEAN BRWPL_AppendWriteFileSync(
										 BRWPL_FFS_HANDLE ffs_handler,
										 uint8 *buf_ptr,
										 uint32 write_size,
										 uint32 offset
										 )
{
	uint32 bytes_written = 0;
	SFS_ERROR_E result = 0;

	if(0==buf_ptr||0==ffs_handler)
	{
		return FALSE;
	}
	if(SFS_ERROR_NONE != SFS_SetFilePointer(ffs_handler, (__int64)offset, SFS_SEEK_BEGIN))
	{
		return 0;
	}
	result = SFS_WriteFile(ffs_handler,buf_ptr,write_size,&bytes_written,0);
	if(SFS_ERROR_NONE==result||SFS_ERROR_IO_PENDING==result)
	{
		return TRUE;
	}
	else
	{
		//BRWDebug_Printf:"BRWPL_AppendWriteFileSync err:%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_445_112_2_18_2_7_23_761,(uint8*)"d", result);
		return FALSE;
	}
}
PUBLIC BRWPL_FFS_HANDLE BRWPL_AppendWriteFileAsync(
									   uint16 *full_path_ptr,
									   uint8 *buf_ptr,
									   uint32 write_size,
									   uint32 offset,
									   BRWPL_FFS_OVERLAPPED_T *overlapped_ptr,
									   SFS_ERROR_E  *sfs_error_ptr
									   )
{
	BRWPL_FFS_HANDLE sfs_handler = 0;
	uint32 bytes_written = 0;
	SFS_ERROR_E	result = 0;

	if(0==full_path_ptr||0==buf_ptr||0==overlapped_ptr)
	{
		return 0;
	}
	sfs_handler = SFS_CreateFile(full_path_ptr, (uint32)(SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING), NULL, NULL);
	if(0==sfs_handler)
	{
		return 0;
	}
	if(SFS_ERROR_NONE != SFS_SetFilePointer(sfs_handler, (__int64)offset, SFS_SEEK_BEGIN))
	{
		return 0;
	}
	result = SFS_WriteFile(sfs_handler,buf_ptr,write_size,&bytes_written,overlapped_ptr);

        if (sfs_error_ptr != PNULL)
        {
            *sfs_error_ptr = result;
        }
    
	if(SFS_ERROR_NONE==result||SFS_ERROR_IO_PENDING==result)
	{
		return sfs_handler;
	}
	else
	{
		SFS_CloseFile(sfs_handler);
		return 0;
	}
}
PUBLIC BRWPL_FFS_HANDLE BRWPL_ReadFileAsync(
										const uint16 *full_path_ptr,
										uint8 *buf_ptr,
										uint32 bytes_to_read,
										BRWPL_FFS_OVERLAPPED_T *overlapped_ptr
									  )
{
	uint32 bytes_read=0;
	BRWPL_FFS_HANDLE ffs_handler = 0;
	SFS_ERROR_E	result = 0;

	if(0==full_path_ptr||0==buf_ptr||0==bytes_to_read||0==overlapped_ptr)
	{
		return 0;
	}
	ffs_handler = SFS_CreateFile(full_path_ptr,(uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING),0,0);
	if(0==ffs_handler)
	{
		return 0;
	}
	result = SFS_ReadFile(ffs_handler,buf_ptr,bytes_to_read,&bytes_read,overlapped_ptr);
	if(SFS_ERROR_NONE==result||SFS_ERROR_IO_PENDING==result)
	{
		return ffs_handler;
	}
	else
	{
		SFS_CloseFile(ffs_handler);
		return 0;
	}
}
/*
 *	desc:check if file is exist
 *  param:full_path_ptr  full path 
 *        full_path_len  length of path
 *  return:TRUE if success else FALSE
 */
PUBLIC BOOLEAN BRWPL_IsFileExist(const uint16 *full_path_ptr,uint16 full_path_len)
{
    if(full_path_ptr!=PNULL && 0!=full_path_len)
    {
        return MMIAPIFMM_IsFileExist(full_path_ptr,full_path_len);
    }
    else
    {
        //BRWDebug_Printf:"BRWPL_IsFileExist,full_path_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_529_112_2_18_2_7_23_762,(uint8*)"d",full_path_len);
        return FALSE;
    }
}

PUBLIC BRWPL_FFS_HANDLE BRWPL_FfsFindFirst(const uint16 *find, BRWPL_FFS_FIND_DATA_T *fd)
{
	if(0==find||0==fd)
	{
		return 0;
	}
	else
	{
		return SFS_FindFirstFile(find, fd);
	}
}

PUBLIC BOOLEAN BRWPL_FfsFindNext(BRWPL_FFS_HANDLE ffs_handler,BRWPL_FFS_FIND_DATA_T *fd)
{
	if(0==ffs_handler||0==fd)
	{
		return FALSE;
	}
	else
	{
		if(SFS_NO_ERROR!=SFS_FindNextFile(ffs_handler,fd))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/*
 *	desc:get size of file
 *  param:full_path_ptr  full path 
 *        full_path_len  length of path
 *        size_ptr[output]   size of file
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWPL_GetFileSize(const uint16 *full_path_ptr,uint16 full_path_len,uint32* size_ptr)
{
    if(full_path_ptr!=PNULL && 0!=full_path_len && size_ptr!=PNULL)
    {
        return MMIAPIFMM_GetFileInfo(full_path_ptr,full_path_len,size_ptr,PNULL,PNULL);
    }
    else
    {
        //BRWDebug_Printf:"BRWPL_GetFileSize,full_path_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_575_112_2_18_2_7_23_763,(uint8*)"d",full_path_len);
        return FALSE;
    }
}

PUBLIC BOOLEAN BRWPL_RenameFile(uint16 *old_full_name_ptr, uint16 *new_full_name_ptr)
{
	if(0==old_full_name_ptr || 0==new_full_name_ptr)
	{
		return FALSE;
	}
	return (SFS_ERROR_NONE==SFS_RenameFile(old_full_name_ptr, new_full_name_ptr, NULL));
}
/*
 *	desc:read file(syn operation)
 *  param:full_path_ptr  full path folder string
 *        full_path_len  length of path
 *        buf_ptr   buffer to store data
 *        buf_max_len  max length of buffer
 *        read_len_ptr[output]  real length readed 
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWPL_ReadFileSyn(const uint16 *full_path_ptr,uint16 full_path_len,uint8* buf_ptr,
                                    uint32 buf_max_len,uint32* read_len_ptr)
{
    BOOLEAN ret = FALSE;
    if(full_path_ptr!=PNULL && 0!=full_path_len && buf_ptr!=PNULL && buf_max_len!=0)
    {
        SFS_HANDLE      sfs_handle = 0;
        sfs_handle = SFS_CreateFile(full_path_ptr, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), PNULL, PNULL);
        if(SFS_INVALID_HANDLE != sfs_handle)
        {
             if(SFS_ERROR_NONE == SFS_ReadFile(sfs_handle, buf_ptr, buf_max_len, read_len_ptr, PNULL))
             {
                 ret = TRUE;
             }
        }
        SFS_CloseFile(sfs_handle);
    }
    else
    {
        //BRWDebug_Printf:"BRWPL_ReadFileSyn,full_path_len=%d,buf_ptr=0x%x,buf_max_len=%d,read_len_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_615_112_2_18_2_7_23_764,(uint8*)"dddd",full_path_len,buf_ptr,buf_max_len,read_len_ptr);
    }
    return ret;
}
/*
 *	desc:write buffer to file(syn operation)
 *  param:full_path_ptr  full path folder string
 *        full_path_len  length of path
 *        buf_ptr   address of data to write
 *        to_write_len  the length of data want to write
 *  return:SFS_ERROR_E
 */
PUBLIC SFS_ERROR_E BRWPL_WriteFileSyn(const uint16 *full_path_ptr,uint16 full_path_len,const uint8* buf_ptr,
                                  uint32 to_write_len)
{
    SFS_ERROR_E sfs_error = SFS_ERROR_NONE;
    SFS_HANDLE      sfs_handle = 0;
    uint32          bytes_written = 0;
 
    if(full_path_ptr!=PNULL && 0!=full_path_len && buf_ptr!=PNULL && to_write_len!=0)
    {
        sfs_handle = SFS_CreateFile(full_path_ptr, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/
        sfs_error = MMIAPIFMM_WriteFile(sfs_handle, (const void *)buf_ptr, to_write_len, &bytes_written, NULL);
        SFS_CloseFile(sfs_handle);
        return sfs_error;
    }
    else
    {
        //BRWDebug_Printf:"BRWPL_WriteFileSyn,full_path_len=%d,to_write_len=%d,buf_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_635_112_2_18_2_7_23_765,(uint8*)"ddd",full_path_len,to_write_len,buf_ptr);
        return SFS_ERROR_INVALID_PARAM;
    }
}
/*
 *	desc:delete file(syn operation)
 *  param:full_path_ptr  full path folder string
 *        full_path_len  length of path
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWPL_DelFileSyn(const uint16 *full_path_ptr,uint16 full_path_len)
{
    return MMIAPIFMM_DeleteFileSyn(full_path_ptr,full_path_len);    
}
/*
desc:function like windows printf
*/
//void BRWDebug_Printf(const char* arg, ...)
//{
//    SCI_TRACE_LOW(arg);
//}
/*
 *	desc:function for print warning
 */
void BRWDebug_PrintWarning(const char* arg, ...)
{
    //SCI_TRACE_LOW:"[BRW]---waning-----"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_658_112_2_18_2_7_23_766,(uint8*)"");
    SCI_TRACE_LOW(arg);
    //assert when develop period
    //BRWDebug_Assert(0);
}
/*
 *	desc: function like memcpy,if src or dest null,no assert
 */
PUBLIC void BRW_Memcpy(void *dest, const void *src, uint32 size)
{
	if(0==size)
	{
		return;
	}
    if(dest && src)
    {
        memcpy(dest,src,size);
    }
    else
    {
        //BRWDebug_Printf:"BRW_Memcpy dest=%x,src=%x,size=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_675_112_2_18_2_7_23_767,(uint8*)"ddd",dest,src,size);
    }
}
/*
 *	desc: function like memset,if dest null or size equal 0,no assert
 */
PUBLIC void BRW_Memset(void*dest,uint8 value,uint32 size)
{
    if(dest && (size>0))
    {
        memset(dest,value,size);
    }
    else
    {
        //BRWDebug_Printf:"BRW_Memset:dest=%x,size=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_688_112_2_18_2_7_23_768,(uint8*)"dd",dest,size);
    }
}
/*
 *	function like memcmp
 */
PUBLIC uint32 BRW_Memcmp(const void * src_ptr, const void* * dest_ptr, uint32 count)
{
    return memcmp(src_ptr,dest_ptr,count);
}
/*
 *	get unicode char len
 */
PUBLIC uint32 BRW_WStrlen( const uint16* str)
{
    const uint16* ustr = str;
    
    if( PNULL == str || PNULL == ustr )
    {        
        //BRWDebug_Printf:"BRW_WSTRLEN str null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_705_112_2_18_2_7_23_769,(uint8*)"");
        return 0;
    }

	while(*ustr++)
	{
	
	}

	return( (uint32)(ustr - str - 1) );
}

PUBLIC wchar *BRW_WstrCat(
	wchar *dest_ptr,		
	const wchar *src_ptr )
{
	uint32 i = 0, j = 0;
    

    if ((PNULL == dest_ptr) || (PNULL == src_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BRW_WstrCat :dest_ptr =0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_726_112_2_18_2_7_23_770,(uint8*)"d", dest_ptr);
        return PNULL;
    } 
    
    while(0 != dest_ptr[j] ) 
    {
        j++;
    }

    while( 0 != src_ptr[i] ) 
    {
        dest_ptr[j] = src_ptr[i];
        i++,j++;
    }

    dest_ptr[j] = '\0';

    return dest_ptr;
}
/*
 *	desc:get length of null terminated string,if string null,no assert
 */
PUBLIC uint32 BRW_Strlen(const uint8 *string)
{
    if(string)
    {
        return strlen((char*)string);
    }
    else
    {
        //BRWDebug_Printf:"BRW_Strlen,string null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_755_112_2_18_2_7_23_771,(uint8*)"");
        return 0;
    }
}
/*
 *	desc:function like strcpy,if src or dest null,no assert
 */
PUBLIC uint8* BRW_Strcpy(uint8 *dst, const uint8 *src)
{
    if(dst && src)
    {
        return (uint8*)strcpy((char*)dst,(const char*)src);
    }
    else
    {
        //BRWDebug_Printf:"BRW_Strcpy,dst=%x,src=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_769_112_2_18_2_7_23_772,(uint8*)"dd",dst,src);
        return PNULL;
    }
}
/*
 *	desc:function like strcat,if src or dest null,no assert
 */
PUBLIC uint8* BRW_Strcat(uint8 *dst, const uint8 *src)
{
    if(dst && src)
    {
        return (uint8*)strcat((char*)dst,(const char*)src);
    }
    else
    {
        //BRWDebug_Printf:"BRW_Strcpy,dst=%x,src=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_783_112_2_18_2_7_23_773,(uint8*)"dd",dst,src);
        return PNULL;
    }
}
/*
 *	desc:function like strncpy,if src or dest null,no assert
 */
PUBLIC uint8 *BRW_Strncpy( uint8 *dst, const uint8 *src, uint32 len)
{
    if(dst && src )
    {
        return (uint8*)strncpy((char*)dst,(const char*)src,len);
    }
    else
    {
        //BRWDebug_Printf:"BRW_Strncpy,dst=%x,src=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_797_112_2_18_2_7_23_774,(uint8*)"dd",dst,src);
        return PNULL;
    }
}
/*
 *	desc:function like strcmp,if string or str null ,no assert
 */
PUBLIC uint32 BRW_Strcmp(const uint8 * string, const uint8 * str)
{
    if (!string || !str) {
        //BRWDebug_Printf:"BRW_Strcmp,string=%x,str=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_806_112_2_18_2_7_23_775,(uint8*)"dd",string,str);
        return 1;
    }    
    return strcmp((const char*)string, (const char*)str);
}
/*
 *	desc:function like strncmp,if string or str null ,no assert
 */
PUBLIC uint32 BRW_Strncmp(const uint8 * string, const uint8 * str,uint32 size)
{
    if (!string || !str) {
        //BRWDebug_Printf:"BRW_Strcmp,string=%x,str=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_816_112_2_18_2_7_23_776,(uint8*)"dd",string,str);
        return 1;
    }    
    return strncmp((const char*)string, (const char*)str,size);
}
/*
 *	desc:function like stricmp,if string or str null ,no assert
 */
PUBLIC uint32 BRW_Stricmp(const uint8 * string, const uint8 * str)
{
    if (!string || !str) {
        //BRWDebug_Printf:"BRW_Stricmp,string=%x,str=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_826_112_2_18_2_7_23_777,(uint8*)"dd",string,str);
        return 1;
    }
    return stricmp((char *)string, (char *)str);/*lint !e718*/
}
/*
 *	desc:function like strnicmp,if string or str null ,no assert
 */
PUBLIC uint32 BRW_Strnicmp(const uint8 * string, const uint8 * str, uint32 size)
{
    if (!string || !str) {
        //BRWDebug_Printf:"BRW_Strnicmp,string=%x,str=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_836_112_2_18_2_7_23_778,(uint8*)"dd",string,str);
        return 1;
    }
    return strnicmp((char *)string, (char *)str, size);    /*lint !e718*/
}
/*
 *	desc:function like strstr,if string or str null ,no assert
 */
PUBLIC uint8* BRW_Strstr(const uint8 *string, const uint8 *str)
{
    if (string && str) {
        return (uint8*)strstr((char*)string, (char*)str);
    }
    //BRWDebug_Printf:"BRW_Strstr,string=%x,str=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_848_112_2_18_2_7_23_779,(uint8*)"dd",string,str);
    return PNULL;
}
/*desc: find s2 in s1, case insensitive*/
PUBLIC const uint8 * BRW_CaseStrstr (const uint8 * s1, const uint8 * s2)
{
    const uint8 * ptr = s1;
    
    if (!s1 || !s2 || !*s2) return s1;
    
    while (*ptr) {
        if (toupper(*ptr) == toupper(*s2)) {
            const uint8 * cur1 = ptr + 1;
            const uint8 * cur2 = s2 + 1;
            while (*cur1 && *cur2 && toupper(*cur1)==toupper(*cur2)) {
                cur1++;
                cur2++;
            }
            if (!*cur2) return ptr;
        }
        ptr++;
    }
    return NULL;
}
/*
 *	desc:function like strch,if string null ,no assert
 */
PUBLIC uint8* BRW_Strchr(const uint8 *str, uint8 c)
{
    if(str)
    {
        return (uint8*)strchr((char*)str,(char)c);
    }
    //BRWDebug_Printf:"BRW_Strchr,str=%x,c=%c"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_879_112_2_18_2_7_23_780,(uint8*)"dc",str,c);
    return PNULL;
}
/*
 *	desc:function like atoi,if str null,no assert
 */
PUBLIC uint32 BRW_Atoi(const uint8 *str)
{
     if(!str)
     {
         //BRWDebug_Printf:"BRW_Atoi,invalid param"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_888_112_2_18_2_7_23_781,(uint8*)"");
        return 0;
     }
    return atoi((char*)str);
}
/*
 *	desc:check if long_str's tail equal with tail,consider case difference
 */
PUBLIC uint32  BRW_TailStrcmp(const uint8 *long_str, const uint8* tail)
{
    uint32 long_str_len= 0;
    uint32 tail_len =0;
    if(!long_str || !tail)
    {
        //BRWDebug_Printf:"BRW_TailStricmp,long_str=%x,tail=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_901_112_2_18_2_7_23_782,(uint8*)"dd",long_str,tail);
        return 0;
    }
    long_str_len = BRW_Strlen(long_str);
    tail_len = BRW_Strlen(tail);
    if (tail_len < long_str_len)
    {
        long_str += (long_str_len - tail_len);
    }

    return BRW_Strcmp(tail, long_str);
}
/*
 *	desc:compare long_str's tail with tail,ingore case difference
 */
PUBLIC uint32  BRW_TailStricmp(const uint8 *long_str, const uint8* tail)
{
    uint32 long_str_len= 0;
    uint32 tail_len =0;
    if(!long_str || !tail)
    {
        //BRWDebug_Printf:"BRW_TailStricmp,long_str=%x,tail=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_921_112_2_18_2_7_23_783,(uint8*)"dd",long_str,tail);
        return 0;
    }
    long_str_len = BRW_Strlen(long_str);
    tail_len = BRW_Strlen(tail);
    if (tail_len < long_str_len)
    {
        long_str += (long_str_len - tail_len);
    }

    return BRW_Stricmp(tail, long_str);
}

PUBLIC void BRWMem_DeleteMemHandle(void)
{
#ifdef MOCOR_MPALLOC_SUPPORT
    uint32 delete_result = 0;
#endif

#ifdef BRW_MEM_DEBUG
    BRWMem_print_mem();
    return;
#endif

    if(0 != g_browser_mem_handle)
    {
#ifdef MOCOR_MPALLOC_SUPPORT
#ifdef WIN32
//        SCI_DumpAllocatedMemInfo();
#endif
        delete_result = SCI_DeleteMemPool(g_browser_mem_handle, TRUE);
        if(SCI_SUCCESS != delete_result)
        {
            //BRWDebug_Printf:"[BRW]W BRWMem_DeleteMemHandle: delete mem handle fail,err:%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_954_112_2_18_2_7_24_784,(uint8*)"d",delete_result);
        }
#else
#ifndef PLATFORM_SC6530
#ifdef WIN32
        {
            CFL_MspaceInfo_T info = {0};

            CFL_MspaceGetInfo(g_browser_mem_handle, &info);
        }
//        SCI_DumpAllocatedMemInfo();
#endif
        CFL_DestroyMspace(g_browser_mem_handle);
        SCI_FREE(s_dorado_memory_ptr);
        s_dorado_memory_ptr = PNULL;
#else//PLATFORM_SC6530
        CFL_DestroyMspaceEx(g_browser_mem_handle);
#endif
#endif
        g_browser_mem_handle = 0;
    }
    else
    {
        //BRWDebug_Printf:"BRWMem_DeleteMemHandle:Mem Handle has been deleted!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_978_112_2_18_2_7_24_785,(uint8*)"");
    }
}
PUBLIC BOOLEAN BRWMem_CreatMemHandle(void)
{    
#ifndef PLATFORM_SC6530
    void    *block_buffer_ptr = PNULL;
    uint32  block_buff_size = 0;
#endif
#ifdef MOCOR_MPALLOC_SUPPORT
    APP_MEM_CFG_T   mem_cfg = {0};
#endif

    //SCI_TRACE_LOW:"BRWMem_CreatMemHandle: size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_1007_112_2_18_2_7_24_786,(uint8*)"d", MMIDORADO_MEMORY_SIZE);
    if(0 == g_browser_mem_handle)
    {       
#ifdef PLATFORM_SC6530
        if(g_browser_mem_handle != 0)
        {
            CFL_DestroyMspaceEx(g_browser_mem_handle);
			g_browser_mem_handle = 0;        
        }
#else
        if (s_dorado_memory_ptr != PNULL)//protect
        {
            SCI_FREE(s_dorado_memory_ptr);
            s_dorado_memory_ptr = PNULL;
        }
        block_buff_size = MMIDORADO_MEMORY_SIZE;  //return memory size.
        block_buffer_ptr = SCI_ALLOCA(block_buff_size);
		
        if (block_buffer_ptr != PNULL)
        {
            //memset(block_buffer_ptr, block_buff_size, 0);
            SCI_MEMSET(block_buffer_ptr, 0, block_buff_size);//converity
        }
        else
        {
            BRWDebug_PrintWarning("BRWMem_CreatMemHandle: ALLOC fail!");
            return FALSE;
        }		
        s_dorado_memory_ptr = block_buffer_ptr;
#endif    
         
#ifdef  DLMALLOC_SUPPORT
#ifdef PLATFORM_SC6530
        g_browser_mem_handle = CFL_CreateMspaceEx(MMIDORADO_MEMORY_GRANULARITY, 
                                                  MMIDORADO_MEMORY_SIZE/MMIDORADO_MEMORY_GRANULARITY);
#else
        g_browser_mem_handle = CFL_CreateMspace(block_buffer_ptr, block_buff_size);
#endif    
#elif defined MOCOR_MPALLOC_SUPPORT
        mem_cfg.min_pool_size = 4;
        mem_cfg.pool_space_size =  0;//block_buff_size/2;
        mem_cfg.heap_mem_watermark = 1024;//@juan temp
        g_browser_mem_handle = SCI_CreateMemPool(BROWSER_POOL_NAME, block_buffer_ptr, block_buff_size, &mem_cfg);
#endif   
        
        if(0 == g_browser_mem_handle)
        {
#ifndef PLATFORM_SC6530
            SCI_FREE(s_dorado_memory_ptr);
            s_dorado_memory_ptr = PNULL;
#endif
            BRWDebug_PrintWarning("BRWMem_CreatMemHandle: creat mem handle fail!");
            return FALSE;
        }  

    }
    else
    {
        //BRWDebug_Printf:"[BRW]BRWMem_CreatMemHandle:Mem Handle has been initiated!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_1045_112_2_18_2_7_24_787,(uint8*)"");
    }

    return TRUE;
}

#ifdef BRW_MEM_DEBUG
LOCAL void *BRWMem_malloc_debug(int size, const char *pfile, int line)
{
    MEM_INFO_T n = {0};
    void *new_ptr = 0;
    int len =  0;

#ifdef MOCOR_MPALLOC_SUPPORT
    new_ptr = SCI_MemPoolMalloc(size,g_browser_mem_handle,pfile,line);
#else
#ifndef PLATFORM_SC6530
    new_ptr = CFL_MspaceMallocEx(g_browser_mem_handle, size);
#else
    new_ptr = CFL_MspaceMalloc(g_browser_mem_handle, size);
#endif
#endif
	//SCI_TRACE_LOW("BRWMem_malloc_debug:%x",new_ptr);
    if(0!=new_ptr)
    {
		n.address = new_ptr;
		n.filename = pfile;
		n.line = line;
		n.size = size;
		BRWMem_add_push_node(&n);
	}
	return new_ptr;
}

LOCAL void BRWMem_delete_push_node(void *address)
{
	int count = 0;
	
	if(address!=0)
	{
		while(count<2000)
		{
			if(g_mem_alloc_arr[count].address == address)
			{
				BRW_Memset(&g_mem_alloc_arr[count],0,sizeof(MEM_INFO_T));
				break;
			}
			else
			{
				count++;
			}
		}
	}
}

LOCAL void BRWMem_free_debug(void *ptr,char *pfile, int line)
{
    void* address= 0;

	//SCI_TRACE_LOW("BRWMem_free_debug:%x,%s,%d",ptr,pfile,line);
    if(ptr) {
        address = ptr;
        BRWMem_delete_push_node(address);
#ifdef MOCOR_MPALLOC_SUPPORT
        SCI_MemPoolFree(ptr);
#else
#ifdef PLATFORM_SC6530
        CFL_MspaceFreeEx(g_browser_mem_handle, ptr);    
#else
        CFL_MspaceFree(g_browser_mem_handle, ptr);    
#endif      
#endif
    }
}

LOCAL void BRWMem_add_push_node(MEM_INFO_T *new_mem)
{
    int count =  0;
    
	if (s_mem_debug_mutex == 0)
    {
        s_mem_debug_mutex = SCI_CreateMutex(
                            "BRW_MEM_DEBUG",
                            SCI_NO_INHERIT
                            );
    }
	SCI_GetMutex(s_mem_debug_mutex,SCI_WAIT_FOREVER);
	while(count<2000)
	{
		if(g_mem_alloc_arr[count].address == 0)
		{
			break;
		}
		else
		{
			count++;
		}
	}
	if(count<2000)
	{
		BRW_Memcpy(&g_mem_alloc_arr[count],new_mem,sizeof(MEM_INFO_T));
	}
	SCI_PutMutex(s_mem_debug_mutex);
}

PUBLIC void BRWMem_print_mem(void)
{
	int count = 0;

    //BRWDebug_Printf:"BRW MEM leak print start:\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_1146_112_2_18_2_7_24_788,(uint8*)"");
    while(count<2000){
		if(g_mem_alloc_arr[count].address!=0)
		{
			//SCI_TRACE_LOW:"%d %s/%d 0x%x"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_1151_112_2_18_2_7_24_789,(uint8*)"dsdd",g_mem_alloc_arr[count].size,g_mem_alloc_arr[count].filename,g_mem_alloc_arr[count].line,g_mem_alloc_arr[count].address);
		}
		count++;
    }
	//BRW_Memset(g_mem_alloc_arr,0,2000*sizeof(MEM_INFO_T));
	//BRWDebug_Printf:"BRW MEM leak print end:\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_1156_112_2_18_2_7_24_790,(uint8*)"");
}
#endif

PUBLIC BOOLEAN BRWMem_InitMem(void)
{
    BOOLEAN result = TRUE;
    
    if (0 == g_browser_mem_handle)
    {
#if 0//@fen.xie    
        if(NOT_USE != BL_GetStatus((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_WAP))
        {
            BRWDebug_PrintWarning("BRWMem_InitMem, Mem Init Fail!");
            result = FALSE;
        }
        else
#endif            
        {
            result = BRWMem_CreatMemHandle();
        }
    }
    
    return result;
}
/*
 *	desc:malloc memory
 *  param: size  size want to allocate
 *         file  name of file which call this function
 *         line  line num where call this function
 *  return:memory address if success,else return PNULL
 */
PUBLIC void * BRWMem_MallocEx(uint32 size,const char* file,uint32 line)
{
    void    *new_ptr=PNULL;
    
    if (0 == size)
    {
        BRWDebug_PrintWarning("BRWMem_MallocEx,size=0");
        return PNULL;
    }
    
    if (0 == g_browser_mem_handle)
    {
        BRWDebug_PrintWarning("BRWMem_MallocEx,g_browser_mem_handle null!");
        return PNULL;

    }
    
#ifndef BRW_MEM_DEBUG
#ifdef MOCOR_MPALLOC_SUPPORT
        new_ptr = SCI_MemPoolMalloc(size,g_browser_mem_handle,file,line);
#else
#ifdef PLATFORM_SC6530
        new_ptr = CFL_MspaceMallocEx(g_browser_mem_handle, size+1);
#else
        new_ptr = CFL_MspaceMalloc(g_browser_mem_handle, size+1);
#endif
#endif

#else
        new_ptr = BRWMem_malloc_debug((int)size,file,line);
#endif

    if (PNULL == new_ptr)
    {
        //SCI_TRACE_LOW:"BRWMem_MallocEx,malloc failed,size:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_1217_112_2_18_2_7_24_791,(uint8*)"d",size);
#ifdef BRW_MEM_DEBUG
        //		BRWMem_print_mem();
#endif
    }
    
    return new_ptr;
}
/*
 *	desc:free memory
 *  param: mem_ptr  memory address want to free
 *  return:void
 */
PUBLIC void   BRWMem_FreeEx(void* mem_ptr,char *pfile, int line)
{
    if (mem_ptr != PNULL)
    {
#ifndef BRW_MEM_DEBUG

#ifdef MOCOR_MPALLOC_SUPPORT
        SCI_MemPoolFree(mem_ptr);
#else
#ifdef PLATFORM_SC6530
        CFL_MspaceFreeEx(g_browser_mem_handle, mem_ptr);  
#else
        CFL_MspaceFree(g_browser_mem_handle, mem_ptr);  
#endif
#endif
#else
        BRWMem_free_debug(mem_ptr,pfile, line);
#endif
        mem_ptr = PNULL;
    }    
}

/*
 *	desc:realloc memory
 *  param: mem_ptr  the former alloced memory address
  *        new_size new alloc size
 *  return:void
 */
PUBLIC void *BRWMem_ReallocEx(void* mem_ptr, int new_size, char *pfile, int line)
{
#ifndef BRW_MEM_DEBUG
#ifdef MOCOR_MPALLOC_SUPPORT
    SCI_ASSERT(0);/*assert verified*/
#else
#ifdef PLATFORM_SC6530
    return CFL_MspaceReallocEx(g_browser_mem_handle, mem_ptr, new_size);  
#else
    return CFL_MspaceRealloc(g_browser_mem_handle, mem_ptr, new_size);  
#endif
#endif
#else
    SCI_ASSERT(0);/*assert verified*/
#endif

}

/************************************************************************/
/*                   时间相关的接口                                     */
/************************************************************************/
/*
 *	desc: get current system time,just like windows time_t
 */
PUBLIC uint32 BRWPL_GetCurTime(void)
{
    return MMIAPICOM_GetCurTime();
}

/*
 *	desc:convert asc str to unicode wchar
 */
PUBLIC wchar* BRWPL_StrToWstr(const char* src_ptr,wchar* dst_ptr)
{
    wchar *return_wstr = dst_ptr;
    if(PNULL==dst_ptr)
    {
        BRWDebug_PrintWarning("BRWPL_StrToWstr:dst_ptr null");
        return PNULL;
    }
    if( src_ptr == PNULL )
    {
        return return_wstr;
    }

    while( '\0' != ( *dst_ptr++ = *src_ptr++ ) )    /* copy string */
    {
    }

    return return_wstr;    
}
/*
 *	desc:convert unicode wchar to asc str
 */
PUBLIC char* BRWPL_WstrToStr(const wchar* src_ptr,char* dst_ptr)
{
    char *return_str = dst_ptr;

    if(PNULL==dst_ptr)
    {
        //BRWDebug_Printf:"BRWPL_WstrToStr:dst_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_1306_112_2_18_2_7_24_792,(uint8*)"");
        return PNULL;
    }
    if( src_ptr == PNULL )
    {
        return return_str;
    }

    while( !(*src_ptr & 0xFF00) && '\0' != (*dst_ptr++ = (char)*src_ptr++) )
    {
    }

    return return_str;
}

PUBLIC wchar *BRWPL_WstrStrnCatWithAsciiStr(
	wchar *dest_ptr,	
	const char *src_ptr,
	uint32 dest_len)
{
	uint32 i = 0, j = 0;
    
    if(PNULL == dest_ptr || PNULL == src_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BRWPL_WstrStrnCatWithAsciiStr :dest_ptr =0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_1331_112_2_18_2_7_24_793,(uint8*)"d", dest_ptr);
        return PNULL;
    }
    
    while( 0 != dest_ptr[j] )
    {
        j++;
    }

    while( 0 != src_ptr[i] && j<dest_len-1) 
    {
        dest_ptr[j] = src_ptr[i];
        i++,j++;
    }

    dest_ptr[j] = '\0';

    return dest_ptr;
}

/*
 *	desc:获取存放浏览器系统文件的设备名(unicode格式)
 *  param:device_name_len_ptr[out]  设备名长度
 *  return:unicode格式的设备名
 */
PUBLIC wchar* BRWPL_GetBrwSysDevName(uint16 *device_name_len_ptr)
{
#ifdef NANDBOOT_SUPPORT
    MMIFILE_DEVICE_E dev = MMI_DEVICE_SYSTEM;
#else
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;   //Modify it for not tcard,select udisk as storage device.(bugs 363699)
#endif
    uint16 device_name_len = 0;
    wchar *device_ptr = PNULL;

    do
    {
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev)))
        {
            device_name_len = MMIAPIFMM_GetDevicePathLen(dev);
            device_ptr = MMIAPIFMM_GetDevicePath(dev);
            break;
        }
    }while(++dev < MMI_DEVICE_NUM);

    if (PNULL != device_name_len_ptr)
    {
        *device_name_len_ptr = device_name_len;
    }

    return device_ptr;
}

/*
 *	desc:获取存放浏览器系统文件的设备名(unicode格式)
 *  param:device_name_len_ptr[out]  设备名长度
 *  return:unicode格式的设备名
 */
PUBLIC wchar* BRWPL_GetBrwCookieDevName(uint16 *device_name_len_ptr)
{
 //因cookie的存储需求为先SD卡->U盘->Ram，因此项目的NANDBOOT_SUPPORT为TRUE，因此，再这就不区分是不是NANDBOOT.
//#ifdef NANDBOOT_SUPPORT
//    MMIFILE_DEVICE_E dev = MMI_DEVICE_SYSTEM;
//#else
    MMIFILE_DEVICE_E dev = MMI_DEVICE_SDCARD;
//#endif
    uint16 device_name_len = 0;
    wchar *device_ptr = PNULL;
#if 0//NANDBOOT_SUPPORT
    do
    {
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev)))
        {
            device_name_len = MMIAPIFMM_GetDevicePathLen(dev);
            device_ptr = MMIAPIFMM_GetDevicePath(dev);
            break;
        }
    }while(++dev < MMI_DEVICE_NUM);
#else
    do
    {
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev)))
        {
            device_name_len = MMIAPIFMM_GetDevicePathLen(dev);
            device_ptr = MMIAPIFMM_GetDevicePath(dev);
            break;
        }
	    else if(MMI_DEVICE_SYSTEM == dev)//fixed for coverity:29815
    	{
	      SCI_TRACE_LOW("[MMI]BRWPL_GetBrwCookieDevName failed");
	      break;
	    }
    }while(--dev >= MMI_DEVICE_SYSTEM);
#endif
    if (PNULL != device_name_len_ptr)
    {
        *device_name_len_ptr = device_name_len;
    }
    return device_ptr;
}
/*
 *	desc:check and get validate device,without system device(C:)
 *	notes:for MS00254277
 *  param:device_name_len_ptr[out]  设备名长度
 *  return:unicode格式的设备名
 */
PUBLIC wchar* BRWPL_GetValidateDevName(uint16 *device_name_len_ptr)
{
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    uint16 device_name_len = 0;
    wchar *device_ptr = PNULL;

#ifdef NANDBOOT_SUPPORT
    dev = MMIAPIFMM_GetFirstValidDevice();
#else
    dev = MMIAPIFMM_GetFirstValidDevice(); //Modify it for not tcard,select udisk as storage device.(bugs 363699)
#endif

    if (dev < MMI_DEVICE_NUM)
    {
        device_name_len = MMIAPIFMM_GetDevicePathLen(dev);
        device_ptr = MMIAPIFMM_GetDevicePath(dev);
    }

    if (PNULL != device_name_len_ptr)
    {
        *device_name_len_ptr = device_name_len;
    }

    return device_ptr;
}

/*
 *	desc:get browser system file full path for write
 *	notes:
 *  param:
 *  return:result
 */
PUBLIC BOOLEAN BRWPL_GetValidDevSysBrwRootForWrite(
                wchar           *full_path_ptr,     //out
                uint16           *full_path_len_ptr,  //in/out, 双字节为单位
                uint32 *dev_ptr   //in/out,can be null
                )
{
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    BOOLEAN result = FALSE;
    wchar   unicode_dir_name[BRW_ROOT_DIR_LEN+1] = {0};
    const wchar    *path_name_ptr = PNULL;
    uint16  path_name_len = 0;                                     

    BRWPL_StrToWstr(BRW_ROOT_DIR, unicode_dir_name);

    path_name_ptr = unicode_dir_name;
    path_name_len = BRW_ROOT_DIR_LEN;                                     

#ifdef NANDBOOT_SUPPORT
    dev = MMIAPIFMM_GetFirstValidDevice();
    if (MMI_DEVICE_NUM > dev)
    {
        result = MMIAPIFMM_GetSysFileFullPathForWrite(TRUE, 
                        path_name_ptr, path_name_len,
                        PNULL, 0,
                        0, 
                        full_path_ptr, full_path_len_ptr
                        );
    }
#else//NEWMS00176308
    {
        dev = MMIAPIFMM_GetFirstValidSD();

        if (MMI_DEVICE_NUM > dev)
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

                result = MMIAPIFMM_CombineFullPath(
                    MMIAPIFMM_GetDevicePath(dev), 
                    MMIAPIFMM_GetDevicePathLen(dev),
                    dst_path_name_ptr, 
                    dst_path_name_len,
                    PNULL, 0,
                    full_path_ptr,
                    full_path_len_ptr
                    );   
                
                SCI_FREE(dst_path_name_ptr);    
            }
            else
            {
                //SCI_TRACE_LOW:"[BRW]W BRWPL_GetValidDevSysBrwRootForWrite:dst_path_name_ptr alloc fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_1479_112_2_18_2_7_25_794,(uint8*)"");
            }
        }
    }    
#endif

    if (result)
    {
        if (PNULL != dev_ptr)
        {
            *dev_ptr = dev;
        }
    }

    //SCI_TRACE_LOW:"[BRW]BRWPL_GetValidDevSysBrwRoot result =%d, dev =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_PLINTERFACE_1493_112_2_18_2_7_25_795,(uint8*)"dd", result, dev);
    return result;
}

/*
 *	desc:combine pathname with two name of dir_ptr and filename
 *  param:device_ptr  unicode格式设备名称
 *        device_len  设备长度,双字节单位
 *        dir_ptr unicode格式路径
 *        dir_len 路径长度
 *        file_name_ptr unicode格式文件名,可以为空
 *        file_name_len 文件名长度,双字节单位
 *        full_path_ptr unicode格式全路径
 *        full_path_len_ptr[in&out]  传入文件名buf最大大小,传出全路径最大长度
 */
PUBLIC BOOLEAN BRWPL_CombinePath(
                              const wchar     *device_ptr, 
                              uint16           device_len, 
                              const wchar     *dir_ptr,
                              uint16           dir_len,
                              const wchar     *file_name_ptr,
                              uint16           file_name_len,
                              wchar           *full_path_ptr,
                              uint16           *full_path_len_ptr
                              )
{
    return MMIAPIFMM_CombineFullPath(device_ptr,device_len,dir_ptr,dir_len,
                                        file_name_ptr,file_name_len,
                                        full_path_ptr,full_path_len_ptr);
}



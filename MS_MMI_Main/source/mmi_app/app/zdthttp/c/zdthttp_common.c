
#include "zdthttp_common.h"


BOOLEAN ZDT_HTTP_File_Save(const char * file_name,uint8  * data_ptr, uint32 data_len,BOOLEAN is_add)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    BOOLEAN ret = FALSE;


    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    if(is_add)
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
    else
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
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

BOOLEAN ZDT_HTTP_File_Remove(const char * file_name)
{	
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;
       
    if(MMIFILE_IsFileExist(full_path,full_path_len))
    {
        MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
        return TRUE;
    }

    return FALSE;
}

BOOLEAN ZDT_HTTP_File_Read(const char * file_name,uint8  * data_ptr, uint32  data_len, uint32 * read_len)
{
    BOOLEAN ret = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    
    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;
    
    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_OPEN_EXISTING |SFS_MODE_READ, NULL, NULL);
    }
    else
    {
        return FALSE;
    }

    if(SFS_INVALID_HANDLE != file_handle)
    {
    	file_ret = MMIFILE_ReadFile(
                              file_handle, 
                              data_ptr, 
                              data_len, 
                              read_len, 
                              PNULL
                             );
        
    	if(SFS_ERROR_NONE == file_ret)
    	{
    		ret = TRUE;
    	}
    	MMIFILE_CloseFile(file_handle);
    }

    return ret;
}

BOOLEAN ZDT_HTTP_File_Exsit(const char * file_name)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    
    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
        return TRUE;
    }
    
    return FALSE;
}

BOOLEAN ZDT_HTTP_Dir_Exsit(const char * dir_path)
{
    SFS_HANDLE	hFind    = PNULL;
    SFS_FIND_DATA_T sfsfind             = {0};
    BOOLEAN		result              = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    uint16 i = 0;
    BOOLEAN ret = FALSE;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)dir_path, SCI_STRLEN(dir_path));
    full_path_len = tmp_len;        
    if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
    {
        full_path[full_path_len++] = 0x5c;
    }

    full_path[full_path_len] = 0x2a;
    full_path[full_path_len+1] = 0;

    hFind  = SFS_FindFirstFile(full_path,&sfsfind );

    if( hFind == PNULL)
    {
        return FALSE;
    }

    SFS_FindClose(hFind);

    return TRUE;
}

BOOLEAN ZDT_HTTP_File_RemoveDirFile(const char * dir_path)
{
    SFS_HANDLE	hFind    = PNULL;
    SFS_FIND_DATA_T sfsfind             = {0};
    BOOLEAN		result              = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    uint16 i = 0;
    BOOLEAN ret = FALSE;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)dir_path, SCI_STRLEN(dir_path));
    full_path_len = tmp_len;        
    if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
    {
        full_path[full_path_len++] = 0x5c;
    }

    full_path[full_path_len] = 0x2a;
    full_path[full_path_len+1] = 0;

    hFind  = SFS_FindFirstFile(full_path,&sfsfind );

    if( hFind == PNULL)
    {
        return TRUE;
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
                return FALSE;
            }
        }

        result = SFS_FindNextFile(hFind, &sfsfind);

    }while(SFS_NO_ERROR == result);

    SFS_FindClose(hFind);

    return TRUE;
}

MMIFILE_HANDLE  ZDT_HTTP_File_Open(const char * file_name,BOOLEAN is_read)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    MMIFILE_HANDLE file_handle = 0;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    if(is_read)
    {
        if(MMIFILE_IsFileExist(full_path, full_path_len))
        {
            file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_OPEN_EXISTING |SFS_MODE_READ, NULL, NULL);
        }
        else
        {
            return SFS_INVALID_HANDLE;
        }
    }
    else
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    }
    
    return file_handle;
	
}

uint32  ZDT_HTTP_File_Write(MMIFILE_HANDLE file_handle,uint8  * data_ptr, uint32 data_len)
{
    uint16 j = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    uint32 transbyte = 0;
    uint32 all_transbyte = 0;
    BOOLEAN ret = TRUE;
    if(file_handle == SFS_INVALID_HANDLE)
    {
        return 0;
    }

    do
    {
        transbyte = 0;
        file_ret = MMIFILE_WriteFile(
        	file_handle, 
        	data_ptr, 
        	data_len, 
        	&transbyte, 
        	NULL
        	);
        
        if(SFS_ERROR_NONE != file_ret)
        {
            ret = FALSE;
            break;
        }
        all_transbyte += transbyte;
    }while(transbyte != 0 && all_transbyte < data_len);
    

    return all_transbyte;
	
}

void  ZDT_HTTP_File_Close(MMIFILE_HANDLE file_handle)
{
    if(file_handle != SFS_INVALID_HANDLE)
    {
        MMIFILE_CloseFile(file_handle);
    }
    return;
	
}

uint32  ZDT_HTTP_File_GetSize(const char * file_name)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    MMIFILE_HANDLE file_handle = 0;
    uint32   file_size          = 0;
    
     tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
     full_path_len = tmp_len;

     file_handle = MMIAPIFMM_CreateFile(full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != file_handle) 
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        MMIAPIFMM_CloseFile(file_handle);
    }
    return file_size;
}

BOOLEAN  ZDT_HTTP_File_Rename(const char * file_name, const char * new_filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar   new_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  new_full_path_len = 0;
    uint16   tmp_len = 0;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    tmp_len = GUI_GBToWstr(new_full_path, (const uint8*)new_filename, SCI_STRLEN(new_filename));
    new_full_path_len = tmp_len;

    return MMIAPIFMM_RenameFile(full_path,full_path_len,new_full_path,new_full_path_len);
}

BOOLEAN ZDT_HTTP_MemFull(BOOLEAN is_tf, uint32 need_len)
{
    uint32          free_space_high     = 0;
    uint32          free_space_low      = 0;
    uint32          cur_free_space_high = 0;
    uint32          cur_free_space_low  = 0;
    MMIFILE_ERROR_E file_ret            = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_UDISK;

    if(is_tf)
    {
       sd_type = MMI_DEVICE_SDCARD;
    }
    free_space_low = need_len+204800;
    if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
    {
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
                return FALSE;
            }
        }
    }
        
    return TRUE;
}


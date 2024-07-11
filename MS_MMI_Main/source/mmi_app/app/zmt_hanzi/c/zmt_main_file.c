#include "zmt_main_file.h"

PUBLIC BOOLEAN zmt_tfcard_exist(void)
{
    MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_SDCARD;
    
    if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
    {
		SCI_TRACE_LOW("dsl %s exit = %d", __FUNCTION__, TRUE);
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint32 zmt_tfcard_get_free_kb(void)
{
	MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_SDCARD;
	uint32 space_low2 = 0, space_high2 = 0;
	uint32 account=0;

	if(MMIFILE_GetDeviceStatus(MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
	{
		MMIFILE_GetDeviceFreeSpace((uint16 *)MMIAPIFMM_GetDevicePath(sd_type) , MMIAPIFMM_GetDevicePathLen(sd_type) ,&space_high2,&space_low2);
		SCI_TRACE_LOW("dsl %s space_high2 = %u", __FUNCTION__, space_high2);
		SCI_TRACE_LOW("dsl %s space_low2 = %u", __FUNCTION__, space_low2);
		account = ((space_high2<<22) + (space_low2>>10));

	}
	SCI_TRACE_LOW("dsl %s account = %uM", __FUNCTION__, account/1024);
	return account;
}


PUBLIC uint32 zmt_tfcard_get_used_kb(void)
{
	MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_SDCARD;
	uint32 space_low2 = 0, space_high2 = 0;
	uint32 account=0;
	if(MMIFILE_GetDeviceStatus(MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
	{
		MMIFILE_GetDeviceUsedSpace((uint16 *)MMIAPIFMM_GetDevicePath(sd_type) , MMIAPIFMM_GetDevicePathLen(sd_type) ,&space_high2,&space_low2);
		SCI_TRACE_LOW("dsl %s space_high2 = %u", __FUNCTION__, space_high2);
		SCI_TRACE_LOW("dsl %s space_low2 = %u", __FUNCTION__, space_low2);
		account = ((space_high2<<22) + (space_low2>>10));

	}
	SCI_TRACE_LOW("dsl %s account = %uM", __FUNCTION__, account/1024);
	return account;
}

uint32  zmt_file_get_size(const char * file_name)
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

PUBLIC BOOLEAN zmt_file_save(const char * file_name,uint8  * data_ptr, uint32 data_len,BOOLEAN is_add)
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

PUBLIC BOOLEAN zmt_file_read(const char * file_name,uint8  * data_ptr, uint32  data_len, uint32 * read_len)
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

uint32  zmt_lrc_file_get_size(const char * file_name)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    MMIFILE_HANDLE file_handle = 0;
    uint32   file_size          = 0;
    
     tmp_len = GUI_UTF8ToWstr(full_path, MMIFILE_FULL_PATH_MAX_LEN+1, (const uint8*)file_name, SCI_STRLEN(file_name));
     full_path_len = tmp_len;

     file_handle = MMIAPIFMM_CreateFile(full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != file_handle) 
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        MMIAPIFMM_CloseFile(file_handle);
    }
    return file_size;
}

PUBLIC BOOLEAN zmt_lrc_file_read(const char * file_name,uint8  * data_ptr, uint32  data_len, uint32 * read_len)
{
    BOOLEAN ret = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    
    tmp_len = GUI_UTF8ToWstr(full_path, MMIFILE_FULL_PATH_MAX_LEN+1, (const uint8*)file_name, SCI_STRLEN(file_name));
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

PUBLIC BOOLEAN zmt_file_exist(char * file_path)
{
	BOOLEAN exist = FALSE;
    wchar   file_str[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
	uint16 file_length = 0;

	GUI_UTF8ToWstr(file_str, MMIFILE_FULL_PATH_MAX_LEN+1, file_path, strlen(file_path));
	//MMIAPICOM_StrToWstr(file_path, file_str);
	file_length = MMIAPICOM_Wstrlen(file_str);
	exist = MMIFILE_IsFileExist(file_str, file_length);
	SCI_TRACE_LOW("dsl_file %s: exist = %d, file_str = %s, file_length = %d\n", __FUNCTION__, exist, file_path, file_length);
	
	return exist;
}

PUBLIC BOOLEAN zmt_file_delete(char * file_path)
{
    	wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
	uint16  full_path_len = 0;
	full_path_len = GUI_GBToWstr(full_path, file_path, strlen(file_path));

	 if(MMIFILE_IsFileExist(full_path, full_path_len))
    	 {
        	if(MMIFILE_DeleteFile((const uint16 *)full_path,PNULL) == SFS_NO_ERROR)
        	{
        		SCI_TRACE_LOW("dsl_file %s: delete file %s success", __FUNCTION__, file_path);
			return TRUE;
		}
    	 }
	return FALSE;
}

PUBLIC BOOLEAN zmt_directory_delete(char * file_path)
{
	wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
	uint16  full_path_len = 0;
	full_path_len = GUI_GBToWstr(full_path, file_path, strlen(file_path));
	 if(MMIFILE_IsFolderExist(full_path, full_path_len))
    	 {
        	if(MMIFILE_DeleteDirectory((const wchar *)full_path) == SFS_NO_ERROR)
        	{
        		SCI_TRACE_LOW("dsl_file %s: delete directory %s success", __FUNCTION__, file_path);
			return TRUE;
		}
    	 }
	return FALSE;
}

PUBLIC void zmt_file_data_add(void * data, uint32 size, char * file_path)
{
	SCI_TRACE_LOW("dsl_file %s: add file %s\n", __FUNCTION__, file_path);

	zmt_file_save(file_path,data,size,TRUE);

	SCI_TRACE_LOW("dsl_file %s: add file end %s\n", __FUNCTION__, file_path);
}

PUBLIC void zmt_file_data_write(void * data, uint32 size, char * file_path)
{
	SCI_TRACE_LOW("dsl_file %s: write file %s\n", __FUNCTION__, file_path);

	zmt_file_save(file_path,data,size,FALSE);

	SCI_TRACE_LOW("dsl_file %s: write file end %s\n", __FUNCTION__, file_path);
}

PUBLIC void * zmt_file_data_read(char * file_path,uint32 * data_size)
{
    uint32 file_len = 0;
	uint8 * tmp_buf = PNULL;
	SCI_TRACE_LOW("dsl_file %s:read file start %s\n", __FUNCTION__, file_path);

	file_len = zmt_file_get_size(file_path);

	SCI_TRACE_LOW("dsl_file %s:file_len %d\n", __FUNCTION__, file_len);

	if(file_len<=0)
	{
		return tmp_buf;
	}

	tmp_buf = (uint8 * )SCI_ALLOC_APPZ(file_len);
	zmt_file_read(file_path, tmp_buf, file_len,data_size);

	SCI_TRACE_LOW("dsl_file %s:read file end %s\n", __FUNCTION__, file_path);
	return tmp_buf;
}

PUBLIC void * zmt_lrc_file_data_read(char * file_path,uint32 * data_size)
{
    uint32 file_len = 0;
	uint8 * tmp_buf = PNULL;
	SCI_TRACE_LOW("dsl_file %s:read file start %s\n", __FUNCTION__, file_path);

	file_len = zmt_lrc_file_get_size(file_path);

	SCI_TRACE_LOW("dsl_file %s:file_len %d\n", __FUNCTION__, file_len);

	if(file_len<=0)
	{
		return tmp_buf;
	}

	tmp_buf = (uint8 * )SCI_ALLOC_APPZ(file_len);
	zmt_lrc_file_read(file_path, tmp_buf, file_len,data_size);

	SCI_TRACE_LOW("dsl_file %s:read file end %s\n", __FUNCTION__, file_path);
	return tmp_buf;
}

PUBLIC BOOLEAN zmt_dir_exsit(char * file_path)
{
    BOOLEAN exist = FALSE;
    wchar   file_str[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
	uint16 file_length = 0;
	
	GUI_GBToWstr( file_str, file_path, strlen(file_path));
	file_length = MMIAPICOM_Wstrlen(file_str);
	exist = MMIFILE_IsFolderExist(file_str, file_length);
	SCI_TRACE_LOW("dsl_file %s: exist = %d, dir_str = %s, dir_length = %d\n", __FUNCTION__, exist, file_path, file_length);

    return exist;
}

PUBLIC BOOLEAN zmt_dir_creat(char * file_path)
{
    BOOLEAN err = FALSE;
    wchar   file_str[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
	GUI_GBToWstr( file_str, file_path, strlen(file_path));
	err = MMIAPIFMM_CreateDirectory(file_str);
	SCI_TRACE_LOW("dsl_file %s: err = %d, dir_str = %s\n", __FUNCTION__, err, file_path);
    return !err;
}
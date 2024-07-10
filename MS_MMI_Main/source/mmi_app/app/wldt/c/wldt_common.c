
#include "wldt_common.h"

BOOLEAN WLDT_SIM_Exsit(void)
{
    uint32 sim_ok_num = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;

    sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);
    if(sim_ok_num ==0)
    {
        return FALSE;
    }
    return TRUE;
}

uint32  WLDT_Get_DayTimeSecond(void)
{
    uint32 daysecond;
    SCI_TIME_T          time;
    TM_GetSysTime(&time);
    daysecond = (time.hour* 3600) + (time.min* 60) + time.sec;
    return daysecond;
}

uint32  WLDT_Get_TimeSecond(void)
{
    SCI_TM_T sys_time = {0};
    uint32   second = 0;

    TM_GetTime(&sys_time);
    second = MMIAPICOM_Tm2Second(sys_time.tm_sec, sys_time.tm_min, sys_time.tm_hour, sys_time.tm_mday, sys_time.tm_mon, sys_time.tm_year);

    return second;
}

#if 1
BOOLEAN  WLDT_DBFile_Write(char * path,char * filename, uint8 * data_ptr, uint32  data_len)
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
     
     if(MMIFILE_IsFileExist(full_path,full_path_len))
     {
         MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
     }
        
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

BOOLEAN  WLDT_DBFile_Exist(char * path, char * filename)
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
     
     return MMIFILE_IsFileExist(full_path,full_path_len);	
}

uint32  WLDT_DBFile_GetSize(char * path, char * filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    MMIFILE_HANDLE file_handle = 0;
    uint32   file_size          = 0;
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

     file_handle = MMIAPIFMM_CreateFile(full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != file_handle) 
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        MMIAPIFMM_CloseFile(file_handle);
    }
    return file_size;	
}

BOOLEAN  WLDT_DBFile_Read(char * path, char * filename, void *buffer, uint32 bytes_to_read, uint32 *bytes_read)
{
    BOOLEAN ret = FALSE;
    FFS_ERROR_E error = FFS_NO_ERROR;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    
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
                              buffer, 
                              bytes_to_read, 
                              bytes_read, 
                              NULL
                             );
        
        if(SFS_ERROR_NONE != file_ret)
        {
            error = FFS_ERROR_INVALID_PARAM;
        }
        else
        {
            ret = TRUE;
        }
        MMIFILE_CloseFile(file_handle);
    }

    return ret;
}

BOOLEAN  WLDT_DBFile_RemoveFile(char * path, char * filename)
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
     }
     return TRUE;	
}

BOOLEAN  WLDT_DBFile_RemoveDir(char * path	)
{
    BOOLEAN ret = FALSE;
    SFS_HANDLE	hFind    = PNULL;
    SFS_FIND_DATA_T sfsfind             = {0};
    SFS_ERROR_E		result              = SFS_NO_ERROR;
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
#endif


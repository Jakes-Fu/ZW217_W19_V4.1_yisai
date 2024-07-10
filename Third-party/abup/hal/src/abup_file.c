#include "abup_typedef.h"
#include "abup_debug.h"
#include "sfs.h"
#include <stdio.h>
#include <stdarg.h>

#include "nvitem.h"
//#include "mmi_filemgr.h"
static ABUP_BOOL bAppend = ABUP_FALSE;

#ifdef ABUP_FOTA_SUPPORT_TCARD
const abup_uint16      s_abup_disk_name[2] = {'E', 0};

#define ABUP_ROOT_FOLDER		L"E:\\@fota_abup"

#define ABUP_DELTAFILE_PATH  	L"E:\\@fota_abup\\abupfota.delta"
#define ABUP_DELTAFILE_PATH12 	L"E:\\@fota_abup\\abupfota12.delta"
#define ABUP_DELTAFILE_PATH21 	L"E:\\@fota_abup\\abupfota21.delta"

#define ABUPVERSION 			L"E:\\@fota_abup\\abupUpState.txt"
#define ABUPDETAID 			    L"E:\\@fota_abup\\abupDetaId.txt"
#define ABUP_BACKUP_PATH  		L"E:\\@fota_abup\\backup.txt"
#define ABUP_LOGIN_PATH 		L"E:\\@fota_abup\\login.txt"

#else
const abup_uint16      s_abup_disk_name[2] = {'D', 0};

#define ABUP_ROOT_FOLDER		L"D:\\@fota_abup"

#define ABUP_DELTAFILE_PATH  	L"D:\\@fota_abup\\abupfota.delta"
#define ABUP_DELTAFILE_PATH12 	L"D:\\@fota_abup\\abupfota12.delta"
#define ABUP_DELTAFILE_PATH21 	L"D:\\@fota_abup\\abupfota21.delta"

#define ABUPVERSION 			L"D:\\@fota_abup\\abupUpState.txt"
#define ABUPDETAID 				L"D:\\@fota_abup\\abupDetaId.txt"
#define ABUP_BACKUP_PATH  		L"D:\\@fota_abup\\backup.txt"
#define ABUP_LOGIN_PATH 		L"D:\\@fota_abup\\login.txt"

#endif

ABUP_BOOL abup_get_fota_drv_size(abup_uint64* total_size, abup_uint64* free_size)
{
    uint32 high_word = 0;
    uint32 low_word = 0;
    int rv = 0;

    rv = SFS_GetDeviceFreeSpace((uint16*)s_abup_disk_name, &high_word, &low_word);

    if (SFS_NO_ERROR != rv)
        return -1;
    *free_size = low_word;

    rv = SFS_GetDeviceUsedSpace((uint16*)s_abup_disk_name, &high_word, &low_word);
    
    if (SFS_NO_ERROR != rv)
        return -1;
    
    *total_size = low_word + *free_size;
    return 0;
}


abup_wchar* abup_get_delta_file_path(void)
{
	return ABUP_DELTAFILE_PATH;
}
abup_wchar* abup_get_delta_file_path12(void)
{
	return ABUP_DELTAFILE_PATH12;
}
abup_wchar* abup_get_delta_file_path21(void)
{
	return ABUP_DELTAFILE_PATH21;
}
abup_wchar* abup_get_backup_file_path(void)
{
	return ABUP_BACKUP_PATH;
}
abup_wchar* abup_get_version_file_path(void)
{
	return ABUPVERSION;
}

abup_wchar* abup_get_login_file_path(void)
{
	return ABUP_LOGIN_PATH;
}

abup_wchar* abup_get_delta_id_path(void)
{
	return ABUPDETAID;
}

abup_wchar* abup_get_abup_root_path(void)
{
	return ABUP_ROOT_FOLDER;
}

#ifdef FOTA_SUPPORT_ABUP_NVRAM

void abup_set_append(void)
{
	bAppend = ABUP_TRUE;
}

ABUP_BOOL abup_get_append(void)
{
	return bAppend;
}

void abup_reset_append(void)
{
	bAppend = ABUP_FALSE;
}

abup_int32 abup_is_nvitem(abup_wchar *file_path)
{
	abup_DebugPrint("abup: %s ,line = %d file_path = %s",__FUNCTION__,__LINE__,file_path);
	if(abup_wstrcmp(abup_get_version_file_path(), file_path) == 0)
	{
		return MN_NV_ABUP_VERSION;
	}
	else if(abup_wstrcmp(abup_get_login_file_path(), file_path) == 0)
	{
		return MN_NV_ABUP_LOGIN;
	}
	else if(abup_wstrcmp(abup_get_delta_id_path(), file_path) == 0)
	{
		return MN_NV_ABUP_BLOCK;
	}

	return E_ABUP_FAILURE;
}

abup_int32 abup_is_nvitem_hand(abup_int32 hand)
{
	abup_DebugPrint("abup: %s ,line = %d hand = %d",__FUNCTION__,__LINE__,hand);
	if(hand == MN_NV_ABUP_VERSION || hand == MN_NV_ABUP_LOGIN || hand == MN_NV_ABUP_BLOCK)
	{
		return E_ABUP_SUCCESS;
	}

	return E_ABUP_FAILURE;
}

abup_int32 abup_reset_nvitem(ABUP_NV_ITEM_ID itemid)
{
	abup_int32 ret=0;
	
	switch(itemid)
	{
		case MN_NV_ABUP_VERSION:
			{
				ABUP_VERSION_STRUCT version={0};
				version.state = 0;
				abup_DebugPrint("abup: %s ,line = %d",__FUNCTION__,__LINE__);
				if(0 == EFS_NvitemWrite(MN_NV_ABUP_VERSION, sizeof(ABUP_VERSION_STRUCT), (uint8 *)&version, FALSE))
				{
					return E_ABUP_SUCCESS;
				}
			}
			break;
		case MN_NV_ABUP_LOGIN:
			{
				ABUP_LOGIN_STRUCT login = {0};
				login.size = 0;
				abup_DebugPrint("abup: %s ,line = %d",__FUNCTION__,__LINE__);
				if(0 == EFS_NvitemWrite(MN_NV_ABUP_LOGIN, sizeof(ABUP_LOGIN_STRUCT), (uint8 *)&login, FALSE))
				{
					return E_ABUP_SUCCESS;
				}
			}
			break;
		case MN_NV_ABUP_BLOCK:
			{
				ABUP_BLOCK_STRUCT block = {0};
				block.size = 0;
				abup_DebugPrint("abup: %s ,line = %d",__FUNCTION__,__LINE__);
				if(0 == EFS_NvitemWrite(MN_NV_ABUP_BLOCK, sizeof(ABUP_BLOCK_STRUCT), (uint8 *)&block, FALSE))
				{
					return E_ABUP_SUCCESS;
				}
			}
			break;
		default:
			return E_ABUP_FAILURE;
	}
	return E_ABUP_FAILURE;
}

abup_int32 abup_check_nvitem(ABUP_NV_ITEM_ID itemid)
{
	abup_int32 ret=0;
	
	switch(itemid)
	{
		case MN_NV_ABUP_VERSION:
			{
				ABUP_VERSION_STRUCT version={0};
				
				if(NVERR_NONE !=EFS_NvitemRead(MN_NV_ABUP_VERSION, sizeof(ABUP_VERSION_STRUCT), (uint8 *)&version))
				{
					return E_ABUP_FAILURE;
				}
				abup_DebugPrint("abup: %s ,line = %d version.state =%d",__FUNCTION__,__LINE__,version.state);
				return version.state == 0? E_ABUP_FAILURE : version.state;
			}
			break;
		case MN_NV_ABUP_LOGIN:
			{
				ABUP_LOGIN_STRUCT login = {0};
				login.size = 0;
				
				if(NVERR_NONE !=EFS_NvitemRead(MN_NV_ABUP_LOGIN, sizeof(ABUP_LOGIN_STRUCT), (uint8 *)&login))
				{
					return E_ABUP_FAILURE;
				}
				abup_DebugPrint("abup: %s ,line = %d,login.size = %d",__FUNCTION__,__LINE__,login.size);
				return login.size == 0? E_ABUP_FAILURE : login.size;
			}
			break;
		case MN_NV_ABUP_BLOCK:
			{
				ABUP_BLOCK_STRUCT block = {0};
				block.size = 0;
				
				if(NVERR_NONE !=EFS_NvitemRead(MN_NV_ABUP_BLOCK, sizeof(ABUP_BLOCK_STRUCT), (uint8 *)&block))
				{
					return E_ABUP_FAILURE;
				}
				abup_DebugPrint("abup: %s ,line = %d,block.size=%d",__FUNCTION__,__LINE__,block.size);
				return block.size == 0? E_ABUP_FAILURE : block.size;
			}
			break;
		default:
			return E_ABUP_FAILURE;
	}
	return E_ABUP_FAILURE;
}

abup_int32 abup_write_item(abup_int32 file_handle,abup_char *inBuff)
{
	abup_int32 ret=0;
	abup_DebugPrint("abup: %s ,line = %d file_handle = %d",__FUNCTION__,__LINE__,file_handle);
	switch(file_handle)
	{
		case MN_NV_ABUP_VERSION:
			{
				ABUP_VERSION_STRUCT version={0};
				//version.state = ABUP_TRUE;
				abup_DebugPrint("abup: %s ,line = %d",__FUNCTION__,__LINE__);
				if(abup_get_append() == ABUP_TRUE)
				{
					abup_reset_append();
					if(0 != EFS_NvitemRead(MN_NV_ABUP_VERSION, sizeof(ABUP_VERSION_STRUCT), (uint8 *)&version))
					{
						return E_ABUP_FAILURE;
					}
					version.state+=abup_strlen(inBuff);
					abup_strcat((abup_char *)version.versionbuff, inBuff);
					version.state = ABUP_TRUE;
					if(0 != EFS_NvitemWrite(MN_NV_ABUP_VERSION, sizeof(ABUP_VERSION_STRUCT), (uint8 *)&version, FALSE))
					{
						return E_ABUP_FAILURE;
					}
					else
					{
						return E_ABUP_SUCCESS;
					}
				}
				else
				{
					version.state=abup_strlen(inBuff);
					abup_strcpy(version.versionbuff, inBuff);
					if(0 != EFS_NvitemWrite(MN_NV_ABUP_VERSION, sizeof(ABUP_VERSION_STRUCT), (uint8 *)&version, FALSE))
					{
						return E_ABUP_FAILURE;
					}
					else
					{
						return E_ABUP_SUCCESS;
					}
				}
			}
			break;
		case MN_NV_ABUP_LOGIN:
			{
				ABUP_LOGIN_STRUCT login = {0};
				login.size = 0;
				abup_DebugPrint("abup: %s ,line = %d",__FUNCTION__,__LINE__);
				abup_strcpy(login.data, inBuff);
				login.size+=abup_strlen(inBuff);
				if(0 != EFS_NvitemWrite(MN_NV_ABUP_LOGIN, sizeof(ABUP_LOGIN_STRUCT), (uint8 *)&login, FALSE))
				{
					return E_ABUP_FAILURE;
				}
				else
				{
					return E_ABUP_SUCCESS;
				}
			}
			break;
		case MN_NV_ABUP_BLOCK:
			{
				ABUP_BLOCK_STRUCT block = {0};
				block.size = 0;
				abup_DebugPrint("abup: %s ,line = %d",__FUNCTION__,__LINE__);
				abup_strcpy(block.block_str, inBuff);
				block.size+=abup_strlen(inBuff);
				if(0 != EFS_NvitemWrite(MN_NV_ABUP_BLOCK, sizeof(ABUP_BLOCK_STRUCT), (uint8 *)&block, FALSE))
				{
					return E_ABUP_FAILURE;
				}
				else
				{
					return E_ABUP_SUCCESS;
				}
			}
			break;
		default:
			return E_ABUP_FAILURE;
	}
	return E_ABUP_FAILURE;
}

abup_int32 abup_read_item(abup_int32 file_handle,abup_char *outBuff)
{
	abup_int32 ret=0;
	
	switch(file_handle)
	{
		case MN_NV_ABUP_VERSION:
			{
				ABUP_VERSION_STRUCT version={0};
				abup_DebugPrint("abup: %s ,line = %d",__FUNCTION__,__LINE__);
				if(NVERR_NONE !=EFS_NvitemRead(MN_NV_ABUP_VERSION, sizeof(ABUP_VERSION_STRUCT), (uint8 *)&version))
				{
					return E_ABUP_FAILURE;
				}

				if(version.state == ABUP_FALSE)
				{
					return E_ABUP_FAILURE;
				}
				else
				{
					abup_strcpy(outBuff, version.versionbuff);
					return E_ABUP_SUCCESS;
				}
			}
			break;
		case MN_NV_ABUP_LOGIN:
			{
				ABUP_LOGIN_STRUCT login = {0};
				abup_DebugPrint("abup: %s ,line = %d",__FUNCTION__,__LINE__);
				if(NVERR_NONE !=EFS_NvitemRead(MN_NV_ABUP_LOGIN, sizeof(ABUP_LOGIN_STRUCT), (uint8 *)&login))
				{
					return E_ABUP_FAILURE;
				}
				
				if(login.size== 0)
				{
					return E_ABUP_FAILURE;
				}
				else
				{
					abup_strcpy(outBuff, login.data);
					return E_ABUP_SUCCESS;
				}
			}
			break;
		case MN_NV_ABUP_BLOCK:
			{
				ABUP_BLOCK_STRUCT block = {0};
				abup_DebugPrint("abup: %s ,line = %d",__FUNCTION__,__LINE__);
				if(NVERR_NONE !=EFS_NvitemRead(MN_NV_ABUP_BLOCK, sizeof(ABUP_BLOCK_STRUCT), (uint8 *)&block))
				{
					return E_ABUP_FAILURE;
				}
				if(block.size== 0)
				{
					return E_ABUP_FAILURE;
				}
				else
				{
					abup_strcpy(outBuff, block.block_str);
					return E_ABUP_SUCCESS;
				}
			}
			break;
		default:
			return E_ABUP_FAILURE;
	}
	return E_ABUP_FAILURE;
}
#endif

void abup_reset_ipaddr(void)
{
	ABUP_IP_STRUCT ip = {0};
	ip.addr = 0 ;

    if(NVERR_NONE != EFS_NvitemWrite(MN_NV_ABUP_IP, sizeof(ABUP_IP_STRUCT), (uint8 *)&ip, FALSE))
    {
        abup_DebugPrint("abup: %s ,line = %d write failed",__FUNCTION__,__LINE__);
    }
}

abup_uint32 abup_get_ipaddr(void)
{
	ABUP_IP_STRUCT ip = {0};
	
    if(NVERR_NONE ==EFS_NvitemRead(MN_NV_ABUP_IP, sizeof(ABUP_IP_STRUCT), (uint8 *)&ip))
    {
    	abup_DebugPrint("abup: %s ,line = %d ip.addr = 0x%x",__FUNCTION__,__LINE__,ip.addr);
        if (ip.addr == 0xFFFFFFFF)
		{
			abup_reset_ipaddr();
		}
		else
		{
			return ip.addr;
		}
    }
    
    abup_DebugPrint("abup: abup get ipaddr fail");
    return 0;
}

void abup_set_ipaddr( abup_uint32 ipaddr)
{
	ABUP_IP_STRUCT ip = {0};
	ip.addr = ipaddr;
	abup_DebugPrint("abup: %s ,line = %d ip.addr = 0x%x",__FUNCTION__,__LINE__,ip.addr);
    if(NVERR_NONE != EFS_NvitemWrite(MN_NV_ABUP_IP, sizeof(ABUP_IP_STRUCT), (uint8 *)&ip, FALSE))
    {
        abup_DebugPrint("abup: %s ,line = %d write failed",__FUNCTION__,__LINE__);
    }	
}


ABUP_BOOL abup_fs_pathorfile_exist(const abup_wchar *path, ABUP_BOOL pathorfile)
{
    abup_int32 handle=-1;
	abup_uint32 nFlag=0;

	abup_DebugPrint("abup_fs_pathorfile_exist: flag:%s",(pathorfile==ABUP_TRUE)?"folder":"file");

	if(pathorfile)
	{
		SFS_FIND_DATA_T find_data = {0};

		
		abup_DebugPrint("abup_fs_pathorfile_exist: a");

		//normal folder
		handle = SFS_FindFirstFile(path, &find_data);

		abup_DebugPrint("abup_fs_pathorfile_exist: b");
		
		if(handle != 0)//SFS_INVALID_HANDLE
		{
			abup_DebugPrint("abup_fs_pathorfile_exist: b1");
			if(SFS_ATTR_DIR&find_data.attr)
			{
				abup_DebugPrint("abup_fs_pathorfile_exist: b2");
				SFS_FindClose(handle);
				return ABUP_TRUE;
			}
		}

		abup_DebugPrint("abup_fs_pathorfile_exist: c");
		
		SFS_FindClose(handle);
		return ABUP_FALSE;		
	}
	else
	{
		nFlag = SFS_MODE_READ | SFS_MODE_OPEN_EXISTING;

		abup_DebugPrint("abup_fs_pathorfile_exist: x");
#ifdef FOTA_SUPPORT_ABUP_NVRAM
{
		abup_int32 hand = 0;
		hand = abup_is_nvitem(path);
		if(hand > 0)
		{
			if(abup_check_nvitem(hand) > 0)
			{
				abup_DebugPrint("abup: %s start,line = %d result = ABUP_TRUE",__FUNCTION__,__LINE__);
				return ABUP_TRUE;
			}
			else
			{
				abup_DebugPrint("abup: %s start,line = %d result = ABUP_FALSE",__FUNCTION__,__LINE__);
				return ABUP_FALSE;
			}
		}
}		
#endif		
		//success: none-zero, fail: zero
		handle = SFS_CreateFile(path,nFlag,0,0);

	    if (handle != 0)
	    {
	    	abup_DebugPrint("abup_fs_pathorfile_exist: y");
			
			SFS_CloseFile(handle);
	        return ABUP_TRUE;
	    }
		else
	        return ABUP_FALSE;

		abup_DebugPrint("abup_fs_pathorfile_exist: z");
	}
}

abup_int32 Abup_Open_File(abup_wchar *file_path,abup_uint32 flag)
{
	abup_int32 result=-1;
	abup_uint32 nFlag=0;
	abup_uint32 regstate = (abup_uint32)SFS_ERROR_NO_SPACE;//SFS_ERROR_E
	SFS_DEVICE_FORMAT_E format = SFS_UNKNOWN_FORMAT;
#ifdef FOTA_SUPPORT_abup_NVRAM	
	abup_int32 nhand = 0;
	nhand = abup_is_nvitem(file_path);
	abup_DebugPrint("abup: %s start,line = %d,nhand = %d",__FUNCTION__,__LINE__,nhand);
	if(nhand > 0)
	{
		if(flag & ABUP_CREATE || flag & ABUP_TRUNCATE)
		{
			return nhand;
		}
		else
		{
			result = abup_check_nvitem(nhand);
			abup_DebugPrint("abup: %s start,line = %d,nhand = %d,result=%d",__FUNCTION__,__LINE__,nhand,result);
			if(result > 0)
			{
				return nhand;
			}
			return -1;
		}
	}

	abup_DebugPrint("abup: %s start,line = %d",__FUNCTION__,__LINE__);
#endif	
	if((ABUP_READONLY & flag) == ABUP_READONLY)
	{
		nFlag = SFS_MODE_READ | SFS_MODE_OPEN_EXISTING;
	}
	else if(((ABUP_READWRITE & flag) == ABUP_READWRITE) || ((ABUP_APPEND&flag) == ABUP_APPEND) || 
			((ABUP_TRUNCATE & flag) == ABUP_TRUNCATE))
	{
		//nFlag = SFS_MODE_READ | SFS_MODE_WRITE;
		nFlag = SFS_MODE_WRITE;
		
		if((ABUP_TRUNCATE & flag) == ABUP_TRUNCATE)
			nFlag |= SFS_MODE_CREATE_ALWAYS;
		else if((ABUP_CREATE & flag) == ABUP_CREATE)
			nFlag |= SFS_MODE_CREATE_NEW;
	}
	else 
	{ 
		abup_DebugPrint("Abup_Open_File: bad flag, flag = %d", flag);

		return -1;
	}

	//check if fs can access, if not, so format
	regstate = (abup_uint32)SFS_GetDeviceStatus(s_abup_disk_name);
	abup_DebugPrint("abup: SFS_GetDeviceStatus result:%d", regstate);
    if(regstate != SFS_ERROR_NONE)
    {
    	//registing...
		regstate = (abup_uint32)SFS_RegisterDevice(s_abup_disk_name, &format);
		if(regstate != SFS_ERROR_NONE)
		{
			abup_DebugPrint("abup: reg disk:%s, state:%d, fmt:%d", s_abup_disk_name,regstate, format);
			//return -1;
			regstate = (abup_uint32)SFS_Format(s_abup_disk_name, SFS_AUTO_FORMAT, NULL);
			//abup_DebugPrint("abup: format result:%d", regstate);
			if(regstate != SFS_ERROR_NONE)
				abup_DebugPrint("abup: format fail!,result:%d", regstate);
			else
				 abup_DebugPrint("abup: format OK");
			
		}
		else
			abup_DebugPrint("abup: reg disk:%s, OK",s_abup_disk_name);
    	
		regstate = (abup_uint32)SFS_GetDeviceStatus(s_abup_disk_name);
		abup_DebugPrint("abup: SFS_GetDeviceStatus2 result:%d", regstate);
    }

	abup_DebugPrint("abup: disk status PASS, can access");
	
	if(!abup_fs_pathorfile_exist(abup_get_abup_root_path(), 1))
	{
		abup_DebugPrint("Abup_Open_File: create root folder");
		//SFS_ERROR_NONE: Success
		result = SFS_CreateDirectory(abup_get_abup_root_path());
		if(result != SFS_ERROR_NONE)
		{
			abup_DebugPrint("Abup_Open_File: create fail!!!");
			return -1;
		}
		//else
		//	Abup_Close_File(result);
	}

	abup_DebugPrint("Abup_Open_File: nFlag = %d", nFlag);

	//success: none-zero, fail: zero
	result = SFS_CreateFile(file_path,nFlag,0,0);

	abup_DebugPrint("Abup_Open_File: result:%d", result);
	
	if(result == 0)
		result = -1;

	return result; //HANDLE
		
}

abup_int32 Abup_Seek_File(abup_int32 file_handle,abup_uint32 offset,abup_int32 origin)
{
	
	abup_int32 result=0;	
	abup_int32 whence=0;	
#ifdef FOTA_SUPPORT_ABUP_NVRAM		
	if(abup_is_nvitem_hand(file_handle) == E_ABUP_SUCCESS)
	{
		return E_ABUP_SUCCESS;
	}
#endif	
	switch(origin)
	{
		case ABUP_SEEK_START:
			whence = SFS_SEEK_BEGIN;
			break;
		case ABUP_SEEK_END:
			whence = SFS_SEEK_END;
			break;
		case ABUP_SEEK_CURRENT:
			whence = SFS_SEEK_CUR;
			break;
		default:
			return E_ABUP_FAILURE;
	}
	//SFS_ERROR_NONE: Success
	result = SFS_SetFilePointer(file_handle,offset,whence);

	abup_DebugPrint("Abup_Seek_File: file_handle=%d,result = %d\r\n",file_handle,result);

	if(result != SFS_NO_ERROR)
		return E_ABUP_FAILURE;	
	else
		return E_ABUP_SUCCESS;
	
}

abup_int32 Abup_Remove_File(abup_wchar *file_path)
{
	abup_int32 ret=0;

#ifdef FOTA_SUPPORT_ABUP_NVRAM		
{
	abup_int32 nhand = 0;
	nhand = abup_is_nvitem(file_path);
	if(nhand > 0)
	{
		return abup_reset_nvitem(nhand);
	}
}	
#endif
	if(abup_fs_pathorfile_exist((const abup_wchar *)file_path,0) == ABUP_FALSE)
		return E_ABUP_FAILURE;

	//SFS_ERROR_NONE: Success
	ret = SFS_DeleteFile(file_path, NULL);

	if(ret == SFS_ERROR_NONE)
		return E_ABUP_SUCCESS;
	else 
		return E_ABUP_FAILURE;
	
	
}

abup_int32 Abup_Write_File(abup_int32 file_handle,abup_char *inBuff,abup_uint32 len,abup_uint32 *write_len)
{
	abup_int32 ret = 0;

	if((inBuff==NULL) || (write_len==NULL) || (len==0))
	{
		abup_DebugPrint("Abup_Write_File: ptr NULL or len 0 FAIL!");
		return 0;
	}
#ifdef FOTA_SUPPORT_ABUP_NVRAM		
	if(abup_is_nvitem_hand(file_handle) == E_ABUP_SUCCESS)
	{

		ret = abup_write_item(file_handle,inBuff);
		if(ret == 0)
		{
			*write_len = len;
		}
		return ret;
	}
#endif	
	//SFS_ERROR_NONE: Success
	ret = SFS_WriteFile(file_handle,inBuff,len,write_len,NULL);

	abup_DebugPrint("Abup_Write_File ret=%d" , ret);

	if(ret != SFS_ERROR_NONE)
	{
		ret = -1;
	}
	else
	{
		ret = SFS_FlushFile(file_handle,NULL);
		abup_DebugPrint("Abup_Write_File flush ret=%d" , ret);
	}

	return ret;
}

abup_int32 Abup_Read_File(abup_int32 file_handle,abup_char *outBuff,abup_uint32 len,abup_uint32 *write_len)
{
	abup_int32 ret = -1;

	if((outBuff==NULL) || (write_len==NULL))
	{
		abup_DebugPrint("Abup_Read_File: ptr FAIL!");
		return -1;
	}
#ifdef FOTA_SUPPORT_ABUP_NVRAM		
	abup_DebugPrint("abup: %s ,line = %d file_handle =%d",__FUNCTION__,__LINE__,file_handle);
	if(abup_is_nvitem_hand(file_handle) == E_ABUP_SUCCESS)
	{

		ret = abup_read_item(file_handle,outBuff);
		if(ret == 0)
		{
			*write_len = len;
		}
		return ret;
	}
#endif	
	//SFS_ERROR_NONE: Success
	ret = SFS_ReadFile(file_handle,outBuff,len,write_len,NULL);

	abup_DebugPrint("Abup_Read_File ret=%d" , ret);

	if(ret != SFS_ERROR_NONE)
		ret = -1;
	
	return ret;
}

abup_int32 abup_Get_File_Size(abup_int32 file_handle,abup_uint32 *file_size)
{
	abup_int32 ret = -1;

	if(file_size==NULL)
	{
		abup_DebugPrint("abup_Get_File_Size: size ptr FAIL!");
		return 0;
	}
#ifdef FOTA_SUPPORT_ABUP_NVRAM		
	if(abup_is_nvitem_hand(file_handle) == E_ABUP_SUCCESS)
	{
		abup_int32 result = abup_check_nvitem(file_handle);
		if(result > 0)
		{
			*file_size = result;
			return 0;
		}
		
		return -1;
	}
#endif	
	//SFS_ERROR_NONE: Success
	ret = SFS_GetFileSize(file_handle, file_size);
	if(ret != SFS_ERROR_NONE)
	{
		*file_size = 0;
		ret = -1;
	}

	abup_DebugPrint("abup_Get_File_Size: ret:%d", ret);

	return ret;
}

void Abup_Close_File(abup_int32 file_handle)
{
	abup_int32 ret = -1;
#ifdef FOTA_SUPPORT_ABUP_NVRAM		
	if(E_ABUP_SUCCESS == abup_is_nvitem_hand(file_handle))
	{
		return;
	}
#endif	
	ret = SFS_CloseFile(file_handle);
	abup_DebugPrint("Abup_Close_File: handle:%d, ret:%d", file_handle, ret);	
}

ABUP_BOOL abup_init_fs(void)
{
	abup_int32 result=-1;
	abup_uint32 nFlag=0;
	abup_uint32 regstate = (abup_uint32)SFS_ERROR_NO_SPACE;//SFS_ERROR_E
	SFS_DEVICE_FORMAT_E format = SFS_UNKNOWN_FORMAT;

	//check if fs can access, if not, so format
	regstate = (abup_uint32)SFS_GetDeviceStatus(s_abup_disk_name);
	abup_DebugPrint("abup: SFS_GetDeviceStatus result:%d", regstate);
    if(regstate != SFS_ERROR_NONE)
    {
    	//registing...
		regstate = (abup_uint32)SFS_RegisterDevice(s_abup_disk_name, &format);
		if(regstate != SFS_ERROR_NONE)
		{
			abup_DebugPrint("abup: reg disk:%s, state:%d, fmt:%d", s_abup_disk_name,regstate, format);
			regstate = (abup_uint32)SFS_Format(s_abup_disk_name, SFS_AUTO_FORMAT, NULL);
			//abup_DebugPrint("abup: format result:%d", regstate);
			if(regstate != SFS_ERROR_NONE)
			{
				abup_DebugPrint("abup: format fail!,result:%d", regstate);
				return ABUP_FALSE;
			}
			else
				 abup_DebugPrint("abup: format OK");
			
		}
		else
			abup_DebugPrint("abup: reg disk:%s, OK",s_abup_disk_name);
    	
		regstate = (abup_uint32)SFS_GetDeviceStatus(s_abup_disk_name);
		abup_DebugPrint("abup: SFS_GetDeviceStatus2 result:%d", regstate);
    }

	abup_DebugPrint("abup: disk FAT access status PASS");
	
	return ABUP_TRUE;		
}




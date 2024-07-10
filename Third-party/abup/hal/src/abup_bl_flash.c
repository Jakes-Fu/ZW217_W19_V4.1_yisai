#include "abup_typedef.h"
#include "abup_define.h"
#include "flash_drv.h"
#include "abup_bl_flash.h"
#include "abup_bl_main.h"
#include "ff.h"
#include "flash_drvapi.h"
#include "ffconf.h"
#include "tf_str.h"

const uint16 g_abup_dir_wstr[] = { '@', 'f', 'o','t', 'a','_', 'a' ,'b', 'u', 'p',0 };

#define ABUP_BL_DEFAULT_DIR             		  (g_abup_dir_wstr)

static TCHAR lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
static TCHAR g_bl_deltafile_path[_MAX_LFN]={0};
static TCHAR g_bl_olddata_path[]={'@','f','o','t','a','_','a','b','u','p','\\','o','l','d','d','a','t','a','.','b','i','n',0};

abup_uint32 abup_bl_flash_delta_base(void);
abup_uint32 abup_bl_flash_delta_size(void);
FIL abup_fp;

LOCAL int32 abup_findfile (const TCHAR* path, const TCHAR* dst_name, TCHAR* file_path);
void abup_Close_File(abup_int32 file_handle);
abup_int32 abup_Remove_File(abup_wchar *file_path);

extern const NOR_FLASH_CONFIG_T s_platform_patitiion_config;

abup_uint32 abup_bl_flash_block_size(void)
{
	return s_platform_patitiion_config.sect_size;
}

abup_uint32 abup_bl_flash_backup_size(void)
{
	return s_platform_patitiion_config.sect_size;
}

abup_uint32 abup_bl_flash_backup_base(void)
{
	return s_platform_patitiion_config.abup_fota_app_addr + s_platform_patitiion_config.abup_fota_app_size - s_platform_patitiion_config.sect_size * 2;
}

abup_uint32 abup_bl_flash_delta_base(void)
{
	return s_platform_patitiion_config.abup_fota_app_addr;
}

abup_uint32 abup_bl_flash_delta_size(void)
{
	return s_platform_patitiion_config.abup_fota_app_size - s_platform_patitiion_config.sect_size * 2;
}

abup_uint32 abup_bl_flash_runningnv_base(void)
{
	return s_platform_patitiion_config.efs_start_addr;
}

abup_uint32 abup_bl_flag_base(void)
{
	return s_platform_patitiion_config.abup_fota_app_addr + s_platform_patitiion_config.abup_fota_app_size - s_platform_patitiion_config.sect_size;
}

abup_uint32 abup_bl_flag_size(void)
{
	return s_platform_patitiion_config.sect_size;
}

abup_uint32 AbupGetFlashDiskSize(void)
{
	return abup_bl_flash_delta_size();
}

abup_uint32 abup_bl_read_block(abup_uint8* dest, abup_uint32 start, abup_uint32 size)
{
	abup_uint32 read_size = 0;

	read_size=(abup_uint32)FDL_FlashRead(start, dest, size);
	//abup_bl_debug_print(NULL, "abup read block: addr:0x%x,size:%d,act size:%d,result:%s", \
	return read_size;
}

abup_uint32 abup_bl_write_block(abup_uint8* src, abup_uint32 start, abup_uint32 size)
{
	abup_uint32 writen_size = 0;

	if (start % s_platform_patitiion_config.sect_size == 0)
	{
		FDL_FlashErase(start, s_platform_patitiion_config.sect_size);
	}

	FDL_FlashWriteEnable(start, 1);
	writen_size = (abup_uint32)FDL_FlashWriteArea(start, src, size); //NOTE: src must be from ram, not in nor
	//abup_bl_debug_print(NULL, "abup write block: addr:0x%x,size:%d,act size:%d,result:%s", \
	return writen_size;
}

static FATFS abup_fs;

abup_int32 abup_fat_init(void)
{
	FRESULT result;
	uint32  i,j,errno;
    abup_int32 fp = 0;

#ifdef ABUP_FOTA_SUPPORT_TCARD
	for(i=0; i<2; i++)
	{
		result = f_mount(i,&abup_fs);
        abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_fat_init %d ret=%d",i, result);
		errno = abup_findfile(ABUP_BL_DEFAULT_DIR, _T("abupfota.delta"), g_bl_deltafile_path);
		if(0 == errno)
		{        
			break;
		}
	}
    abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_fat_init pacfilename = %s",g_bl_deltafile_path);
	fp = abup_Open_File(g_bl_deltafile_path, ABUP_READONLY);
    if(!fp)
    {
		abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_fat_init fail");
        return E_ABUP_FAILURE;
    }
    abup_Close_File(fp);
    return E_ABUP_SUCCESS;
#else
    return E_ABUP_SUCCESS;
#endif
}

abup_uint32 abup_bl_read_flash(abup_uint32 addr,abup_uint8* data_ptr, abup_uint32 len)
{
	abup_uint32 rom_offset = abup_bl_flash_delta_base();
	abup_uint32 read_len=0;
	read_len = (abup_uint32)abup_bl_read_block(data_ptr, rom_offset+addr, len);

	abup_bl_debug_print(NULL, "abup read flash: base:%x,offset:0x%x,len:%d,act len:%d,result:%s", \
			rom_offset, addr, len, read_len, (read_len==len)?"OK":"FAIL");	 

	if(read_len == len)
		return 0;       //success
	else
		return 1;		//fail
}

abup_uint32 abup_bl_erase_delata(void)
{
	abup_uint32 rom_offset = abup_bl_flash_delta_base();
	abup_uint32 ret=0, idx=0,blocknum=0,blocksize=0;

	blocksize = abup_bl_flash_block_size();
	if(blocksize < 0)
		return 0;

	blocknum = abup_bl_flash_delta_size() / blocksize;

	for(idx=0;idx<blocknum;idx++)
	{
		ret = FDL_FlashErase(rom_offset+blocksize*idx, blocksize);
		//abup_bl_debug_print(NULL,"abup_bl_erase_delata: addr:0x%x,size:0x%x,ret:%d", \
			//(rom_offset+blocksize*idx), blocksize, ret);
	}

	return ret;
}

void abup_bl_erase_backup_region(void)
{
	abup_uint32 backup_base=abup_bl_flash_backup_base();
	abup_uint32 blocksize=abup_bl_flash_block_size();
	abup_uint32 ret=0;

	//abup_bl_debug_print(NULL,"abup_bl_erase_backup_region: base:0x%x, size:0x%x", backup_base, blocksize);
	if((backup_base>0) && (blocksize>0))
	{
		ret = FDL_FlashErase(backup_base, blocksize);
		//abup_bl_debug_print(NULL,"abup_bl_erase_backup_region: ret:%d", ret);
	}
}

void abup_bl_erase_running_nv(void)
{
	abup_uint32 base=abup_bl_flash_runningnv_base();
	abup_uint32 ret=0;
	abup_uint8 i = 0;

	//abup_bl_debug_print(NULL,"abup_bl_erase_backup_region: base:0x%x, size:0x%x", backup_base, blocksize);
	for (i = 0; i < 5; i++)
	{
		ret = FDL_FlashErase(base + i * abup_bl_flash_block_size(), abup_bl_flash_block_size());
		//abup_bl_debug_print(NULL,"abup_bl_erase_backup_region: ret:%d", ret);
	}
}

abup_int32 abup_bl_write_backup_region(abup_uint8* data_ptr, abup_uint32 len)
{
	abup_uint32 backup_base=abup_bl_flash_backup_base();
	abup_int32 ret=0;

	//abup_bl_debug_print(NULL,"abup_bl_write_backup_region: base:0x%x, size:0x%x", backup_base, len);
	if((backup_base>0) && (len>0))
	{
		ret = abup_bl_write_block(data_ptr, backup_base, len);
		//abup_bl_debug_print(NULL,"abup_bl_write_backup_region: ret:%d", ret);
	}
	if(ret == len)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

abup_int32 abup_bl_read_backup_region(abup_uint8* data_ptr, abup_uint32 len)
{
	abup_uint32 backup_base=abup_bl_flash_backup_base();
	abup_int32 ret=0;

	abup_bl_debug_print(NULL,"abup_bl_read_backup_region: base:0x%x, size:0x%x", backup_base, len);
	if(data_ptr && (backup_base>0) && (len>0))
	{
		ret = abup_bl_read_block(data_ptr, backup_base, len);
		abup_bl_debug_print(NULL,"abup_bl_read_backup_region: ret:%d", ret);
	}
	if(ret == len)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

abup_wchar* abup_get_delta_file_path(void)
{
	return g_bl_deltafile_path;
}

abup_wchar* abup_get_old_data_path(void)
{
	return g_bl_olddata_path;
}

void abup_bl_remove_delata(void)
{
    abup_Remove_File(abup_get_delta_file_path());
    abup_Remove_File(abup_get_old_data_path());
}

abup_int32 abup_Open_File(abup_wchar *file_path, abup_uint32 flag)
{
#ifdef ABUP_FOTA_SUPPORT_TCARD
	FRESULT result;
	BYTE mode = flag;

	result = f_open(&abup_fp, file_path, mode);
	if (FR_OK != result)
	{
		abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_Open_File fail.%d", result);
		return NULL;
	}
    abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_Open_File success size = %d", abup_fp.fsize);
	return (abup_int32)&abup_fp;
#else
	return 0;
#endif
}

abup_int32 abup_Seek_File(abup_int32 file_handle,abup_uint32 offset,abup_int32 origin)
{
#ifdef ABUP_FOTA_SUPPORT_TCARD
	FRESULT result;

	abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_Seek_File file_handle=%d, offset=%d, origin=%d", file_handle, offset, origin);
	switch(origin)
	{
		case ABUP_SEEK_START:
			break;
		case ABUP_SEEK_END:
		case ABUP_SEEK_CURRENT:
		default:
			return E_ABUP_FAILURE;
	}

	if (file_handle != NULL)
	{
		result = f_lseek((FIL *)file_handle, offset);
	}

	return ((result == FR_OK)? E_ABUP_SUCCESS: E_ABUP_FAILURE);
#else
	return E_ABUP_SUCCESS;
#endif
}

abup_int32 abup_Remove_File(abup_wchar *file_path)
{
#ifdef ABUP_FOTA_SUPPORT_TCARD
	FRESULT ret;
    ret = f_unlink(file_path);
	abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_Remove_File.result = %d",ret);
#endif
	return E_ABUP_SUCCESS;
}

abup_int32 abup_Write_File(abup_int32 file_handle,abup_char *inBuff,abup_uint32 len,abup_uint32 *write_len)
{
#ifdef ABUP_FOTA_SUPPORT_TCARD
	UINT b_write = 0;

	abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_Write_File file_handle=%d, len=%d", file_handle, len);
	if (file_handle != NULL)
	{
		abup_uint32 i = 0, num = 0;

		if (len % 512 == 0)
		{
			num = len / 512;
		}
		else
		{
			num = len / 512 + 1;
		}
		*write_len = 0;
		for (i = 0; i < num; i++)
		{
			if (i == num - 1)
			{
				f_write((FIL *)file_handle, inBuff + i * 512, len % 512, &b_write);
			}
			else
			{
				f_write((FIL *)file_handle, inBuff + i * 512, 512, &b_write);
			}
			*write_len += (abup_uint32)b_write;
		}
		abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_Write_File write_len=%d", *write_len);
	}
	
	return 0;
#else
	return 0;
#endif
}

abup_int32 abup_Read_File(abup_int32 file_handle,abup_char *outBuff,abup_uint32 len,abup_uint32 *read_len)
{
#ifdef ABUP_FOTA_SUPPORT_TCARD
	FRESULT ret;
	UINT b_read = 0;

	abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_Read_File file_handle=%d, len=%d", file_handle, len);
	if (file_handle != NULL)
	{
		ret = f_read((FIL *)file_handle, outBuff, len, &b_read);
		*read_len = (abup_uint32)b_read;
		abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_Read_File ret=%d, read_len=%d", ret, *read_len);
	}
	
	return ret;
#else
	return 0;
#endif
}

abup_int32 abup_Get_File_Size(abup_int32 file_handle,abup_uint32 *file_size)
{
#ifdef ABUP_FOTA_SUPPORT_TCARD
	FIL *fp = NULL;

	if (file_handle != NULL)
	{
		fp = (FIL *)file_handle;
		return (abup_int32)fp->fsize;
	}
#endif

	return 0;
}

void abup_Close_File(abup_int32 file_handle)
{
#ifdef ABUP_FOTA_SUPPORT_TCARD
	FRESULT result;

	if (file_handle != NULL)
	{
		result = f_close((FIL *)file_handle);
	}

	abup_bl_debug_print(ABUP_LOG_DEBUG,"abup_Close_File.file_handle=%d,result=%d", file_handle, result);
#endif
}

LOCAL int32 abup_findfile (const TCHAR* path, const TCHAR* dst_name, TCHAR* file_path)
{
#ifdef ABUP_FOTA_SUPPORT_TCARD
	FRESULT res;
	FILINFO fno;
	DIR dir;
	int i;
	TCHAR *fn;
	int find_counts = 0;
	int ret = 0;
	TCHAR temp_fn[_MAX_LFN] = {0};    
	TCHAR file_pac_fn[_MAX_LFN] = {0};

#if _USE_LFN
	fno.lfname = lfn;
	fno.lfsize = sizeof(lfn);
#endif

	res = f_opendir(&dir, path);
	if (res == FR_OK) 
    {   
		i = TF_Load_Wstrlen(path);
		for (;;) 
		{
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0)
			{
				break;
			}
			if (fno.fname[0] == '.')
			{
				continue;
			}
#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			fn = fno.fname;
#endif
			if (fno.fattrib & AM_DIR) 
			{
				continue;
			}
			else
			{
				memset(temp_fn,0,sizeof(temp_fn));
				TF_Load_Wstrcpy(temp_fn,fn);
				TF_Load_Wstrtolower(temp_fn);

				if(TF_Load_Wstrstr(temp_fn, dst_name))
				{                 
					find_counts++;
					TF_Load_Wstrcpy(file_pac_fn, fn);
				}                
			}
		}
	}

	if(0 == find_counts)
	{
		ret = -1;//ERR_OPEN_FILE;
	}
	else if(find_counts > 1)
	{
		ret = -2;//ERR_TOO_MANY_FILE;
	}
	else
	{
		TF_Load_Wstrcpy(file_path, path);
		TF_Load_Wstrcat(file_path, _T("\\"));
		TF_Load_Wstrcat(file_path, file_pac_fn);
	}
	return ret;
#else
	return 0;
#endif
}


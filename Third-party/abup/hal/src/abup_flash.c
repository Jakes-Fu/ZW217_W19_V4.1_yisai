#include "abup_typedef.h"
#include "abup_define.h"

#include "abup_flash.h"
#include "os_api.h"
#include "flash_drvapi.h"

static ABUP_FLASH_PARTATIONT_T s_abup_flash_info={0};

#ifdef FOTA_SUPPORT_ABUP
#ifndef WIN32
abup_extern abup_int32 abup_write_buffer(abup_uint8* data_ptr, abup_uint32 len);
abup_extern abup_uint32 abup_erase_disk(void);
abup_extern void abup_delete_header(void);
abup_extern abup_int32  abup_read_buffer(abup_uint32 addr, abup_uint8* data_ptr, abup_uint32 len);
#else
abup_int32 abup_write_buffer(abup_uint8* data_ptr, abup_uint32 len){return 0;}
abup_uint32 abup_erase_disk(void){return 	0;}
void abup_delete_header(void){}
abup_int32  abup_read_buffer(abup_uint32 addr, abup_uint8* data_ptr, abup_uint32 len){return 0;}
#endif
#endif


void abup_get_flash_info(NOR_FLASH_CONFIG_PTR pspeccfg)
{
	ABUP_FLASH_PARTATIONT_T *flash_info = &s_abup_flash_info;
	if(flash_info)
	{
		NOR_FLASH_CONFIG_PTR flash_info_in_ps = pspeccfg;
					
		if (flash_info_in_ps != NULL)
		{
			flash_info->bank_num = flash_info_in_ps->bank_num;
			flash_info->sect_num = flash_info_in_ps->sect_num;
			flash_info->sect_size = flash_info_in_ps->sect_size;
		}

		if(flash_info_in_ps != NULL)
		{		
			//flash_info->NorBootloaderAdress = flash_info_in_ps->NorBootloaderAdress;
			//flash_info->NorBootloaderSize = flash_info_in_ps->NorBootloaderSize;
			
			flash_info->PsAdress = flash_info_in_ps->ps_addr;
			//flash_info->PSSize = flash_info_in_ps->PSSize;

			flash_info->Spload_Addr = flash_info_in_ps->abup_fota_app_addr;
			flash_info->SploadSize = flash_info_in_ps->abup_fota_app_size;

			flash_info->ResAdress = flash_info_in_ps->mmi_res;
			//flash_info->ResSize = flash_info_in_ps->ResSize;
			
			flash_info->NvAdress = flash_info_in_ps->fixnv_addr;
			//flash_info->NvSize = flash_info_in_ps->NvSize;
			
			flash_info->EfsAdress = flash_info_in_ps->efs_start_addr;
			//flash_info->EfsSize = flash_info_in_ps->EfsSize;

			flash_info->UmemAdress = flash_info_in_ps->umem_addr;
			flash_info->UmemSize = flash_info_in_ps->umem_size;

			flash_info->FlashSize = flash_info_in_ps->flash_size;

		}

	}
}

abup_uint32 abup_flash_block_size(void)
{
	return s_abup_flash_info.sect_size; //ABUP_BLOCK_SIZE;
}

abup_uint32 abup_flash_backup_size(void)
{
	return s_abup_flash_info.sect_size;
}

abup_uint32 abup_flash_backup_base(void)
{
	return s_abup_flash_info.Spload_Addr + s_abup_flash_info.SploadSize - abup_flash_backup_size();
}

abup_uint32 abup_flash_delta_base(void)
{
	return s_abup_flash_info.Spload_Addr;
}

abup_uint32 abup_flash_delta_size(void)
{
	return s_abup_flash_info.SploadSize - abup_flash_backup_size();
}

abup_uint32 abupGetFlashDiskSize(void)
{
	//abup_uint32 block_size=0;

	//block_size = s_abup_flash_info.FlashSize;
	
#if 1
	return abup_flash_delta_size();
#else
	return 0;	
#endif	
}

abup_uint32 abup_get_rom_len(void)
{
	return (abup_uint32)s_abup_flash_info.PSSize; ///////////NOR_BOOTING_NOR_FS_BASE_ADDRESS;
}

void abup_flash_get_partation(void)
{	
	//abup_bl_get_flash_info(&s_abup_flash_info);
	abup_DebugPrint("abup: \r\n-----flash partation info:----->");
	abup_DebugPrint("abup sect_num:%d, sect_size:0x%x", s_abup_flash_info.sect_num, s_abup_flash_info.sect_size);
	//abup_DebugPrint("abup NorBootloaderAdress:0x%x, NorBootloaderSize:0x%x", s_abup_flash_info.NorBootloaderAdress, s_abup_flash_info.NorBootloaderSize);
	abup_DebugPrint("abup PsAdress:0x%x, PsSize:0x%x", s_abup_flash_info.PsAdress, s_abup_flash_info.PSSize);
	abup_DebugPrint("abup ResAdress:0x%x, ResSize:0x%x", s_abup_flash_info.ResAdress, s_abup_flash_info.ResSize);
	abup_DebugPrint("abup NvAdress:0x%x, NvSize:0x%x", s_abup_flash_info.NvAdress, s_abup_flash_info.NvSize);
	abup_DebugPrint("abup EfsAdress:0x%x, EfsSize:0x%x", s_abup_flash_info.EfsAdress, s_abup_flash_info.EfsSize);
	abup_DebugPrint("abup UmemAdress:0x%x, UmemSize:0x%x", s_abup_flash_info.UmemAdress, s_abup_flash_info.UmemSize);
	abup_DebugPrint("abup Spload_Addr:0x%x, SploadSize:0x%x", s_abup_flash_info.Spload_Addr, abup_flash_delta_size());
	abup_DebugPrint("abup FlashSize:0x%x", s_abup_flash_info.FlashSize);
	abup_DebugPrint("abup Delta Base:0x%x, Size:0x%x", abup_flash_delta_base(), abup_flash_delta_size());
	abup_DebugPrint("abup Backup Base:0x%x, Size:0x%x", abup_flash_backup_base(), abup_flash_backup_size());
	abup_DebugPrint("\r\n-----abup flash partation info <-----\r\n");
}


ABUP_BOOL AbupUseFlash(void)
{
#ifdef ABUP_FOTA_SUPPORT_TCARD
	return ABUP_FALSE;
#else
	return ABUP_TRUE;
#endif
}

abup_uint32 AbupGetBlockSize(void)
{
	return s_abup_flash_info.sect_size; //ABUP_BLOCK_SIZE;
}

abup_int32 abup_write_backup_flash(abup_uint32 start,abup_uint8* data_ptr, abup_uint32 len)
{
	abup_int32 result;
	ABUP_BOOL ret=ABUP_FALSE;

	abup_uint32 base=abup_flash_backup_base();
	abup_uint32 maxsize=abup_flash_backup_size() ;
	abup_uint32 addr=0, tolen=len;

	if((!data_ptr) || (len<=0))
		return 0;
	 
#ifdef FOTA_SUPPORT_ABUP	
	addr = base + start;

	abup_DebugPrint("abup_write_backup_flash: addr:0x%x, , len:%d ********",addr, len);
	ret = FLASH_Write(addr, data_ptr, len);
	if(ret == ABUP_TRUE)
	{
		//return len;
		return 0; //write success
	}
	else
	{
		abup_DebugPrint("abup_write_flash: write error!");
		return 1;
	}
#else
	 return 0;
#endif
}

abup_int32 abup_read_backup_flash(abup_uint32 addr,abup_uint8* data_ptr, abup_uint32 len)
{
	 abup_int32 result;
	 ABUP_BOOL ret=ABUP_FALSE;
	 
	 abup_uint32 base = abup_flash_backup_base();
	 //abup_uint32 maxsize=abup_flash_delta_size();

	if((!data_ptr) || (len<=0) || (base<=0))
	{
		abup_DebugPrint("abup_read_backup_flash: error!base:0x%x,len:%d,buf:0x%x",base, len, data_ptr);
		return 1;
	}

	if((addr+len) > abup_flash_backup_size())
	{
		abup_DebugPrint("abup_read_backup_flash: error!overlap offset+len:0x%x,delta maxsize:%d", (addr+len), abup_flash_delta_size());
		return 2;
	}
	
#ifdef FOTA_SUPPORT_ABUP
   	ret = FLASH_Read(base+addr, data_ptr, len);
	if(ret == ABUP_TRUE)
		return 0; //read success
	else
	{
		abup_DebugPrint("abup_read_backup_flash: error!base:0x%x,offset:0x%x,len:%d",base, addr, len);
		return 3;
	}
#else
	return 0;
#endif
}


abup_int32 abup_write_flash(abup_uint32 start,abup_uint8* data_ptr, abup_uint32 len)
{
	abup_int32 result;
	ABUP_BOOL ret=ABUP_FALSE;

	abup_uint32 base=abup_flash_delta_base();
	abup_uint32 maxsize=abup_flash_delta_size() ;
	abup_uint32 addr=0, tolen=len;

	if((!data_ptr) || (len<=0))
		return 0;
	 
#ifdef FOTA_SUPPORT_ABUP	
	addr = base + start;
	if((start+len) > maxsize)
		tolen = maxsize - start;

	abup_DebugPrint("abup_write_flash: addr:0x%x, tolen:%d, len:%d",addr, tolen, len);
	if(tolen > 0)
	{
		ret = FLASH_Write(addr, data_ptr, len);
		if(ret == ABUP_TRUE)
		{
			//return len;
			return 0; //write success
		}
		else
		{
			abup_DebugPrint("abup_write_flash: write error!");
			return 1;
		}
	}
	else
	{
		abup_DebugPrint("abup_write_flash: no data write error!");
		return 2;
	}
#else
	 return 0;
#endif
}

abup_int32 abup_read_flash(abup_uint32 addr,abup_uint8* data_ptr, abup_uint32 len)
{
	abup_int32 result;
	ABUP_BOOL ret=ABUP_FALSE;	 
	abup_uint32 base = abup_flash_delta_base();

	abup_DebugPrint("abup_read_flash:11111------->base:%d",base);
	if((!data_ptr) || (len<=0) || (base<=0))
	{
		abup_DebugPrint("abup_read_flash: error!base:0x%x,len:%d,buf:0x%x",base, len, data_ptr);
		return 1;
	}
	if((addr+len) > abup_flash_delta_size())
	{
		abup_DebugPrint("abup_read_flash: error!overlap offset+len:0x%x,delta maxsize:%d", (addr+len), abup_flash_delta_size());
		return 2;
	}
#ifdef FOTA_SUPPORT_ABUP
	ret = FLASH_Read(base+addr, data_ptr, len);
	abup_DebugPrint("abup_read_flash:4444------->");

	if(ret == ABUP_TRUE){
		return 0; //read success
	}
	else{
		abup_DebugPrint("abup_read_flash: error!base:0x%x,offset:0x%x,len:%d",base, addr, len);
		return 3;
	}
#else
	return 0;
#endif
}

abup_uint32 abup_init_flash(void)
{
	abup_uint32 rom_offset = abup_flash_delta_base();
	abup_uint32 idx=0,blocknum=0,blocksize=0;
	ABUP_BOOL ret=ABUP_FALSE;
	 
	abup_DebugPrint("abup: rom_offset:0x%x", rom_offset);

#ifdef FOTA_SUPPORT_ABUP
	blocksize = abup_flash_block_size();
	if(blocksize <= 0) //&& (blocksize >= )
	{
		abup_DebugPrint("abup: blocksize error!blocksize:%d", blocksize);
		return 0;
	}

	blocknum = abup_flash_delta_size() / blocksize;

	abup_DebugPrint("abup_init_flash: blocknum:%d, blocksize:%d", blocknum, blocksize);

	for(idx=0;idx<blocknum;idx++)
	{
		ret = FLASH_Erase(rom_offset+blocksize*idx);
		if(ret == ABUP_FALSE)
		{
			abup_DebugPrint("abup_init_flash: erase error!idx/num:%d/%d", idx, blocknum);
			break;
		}
	}

	if((ret==ABUP_TRUE) && (idx>=blocknum))
	{
		abup_DebugPrint("abup_init_flash: Erase OK]");
		return 0; ////erase success
	}
	else
	{
		abup_DebugPrint("abup_init_flash: Erase fail!");
		return 1;
	}
#else
   return 0;		
#endif
}

void abup_erase_delata(void)
{
	abup_uint32 base = abup_flash_delta_base();
	ABUP_BOOL ret=ABUP_FALSE;

#ifdef FOTA_SUPPORT_abup
	ret = FLASH_Erase(base);
	if(ret == ABUP_FALSE)
		abup_DebugPrint("abup_init_flash: erase error!offset:0x%x", base);
	else
		abup_DebugPrint("abup_init_flash: ERASE HDR PASS[0x%x]", base);
#else
#endif
}


void abup_erase_backup(void)
{
	abup_uint32 base = abup_flash_backup_base();
	ABUP_BOOL ret=ABUP_FALSE;
#ifdef FOTA_SUPPORT_ABUP
	ret = FLASH_Erase(base);
	if(ret == ABUP_FALSE)
	abup_DebugPrint("abup_init_flash: erase error!offset:0x%x", base);
	else
	abup_DebugPrint("abup_init_flash: ERASE HDR PASS[0x%x]", base);
#endif
}

abup_uint32 abup_addr_to_block(abup_uint32 addr)
{
	abup_uint32 blocksize=0;
	blocksize = abup_flash_block_size();
#ifdef FOTA_SUPPORT_ABUP	
	return 	(addr / blocksize);
#endif
}

void abup_init_block(abup_uint32 block_idx)
{
	abup_uint32 rom_offset = abup_flash_delta_base();
	abup_uint32 blocksize=0;
	blocksize = abup_flash_block_size();
#ifdef FOTA_SUPPORT_ABUP	
	FLASH_Erase(rom_offset+blocksize*block_idx);
#endif
}
 


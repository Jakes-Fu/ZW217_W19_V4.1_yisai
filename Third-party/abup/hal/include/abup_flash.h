#ifndef _ABUP_FLASH_H_
#define _ABUP_FLASH_H_

typedef struct ABUP_FLASH_PARTATION
{
    abup_uint32 bank_num;
    abup_uint32 sect_num;
    abup_uint32 sect_size;

    abup_uint32 ProdinfoAdress;
    abup_uint32 FlashSize;
	
    abup_uint32 SploadSize;
    abup_uint32 Spload_Addr; 
	
    abup_uint32 NvAdress;
    abup_uint32 NvSize;
		
    abup_uint32 ResAdress;
    abup_uint32 ResSize; 
	
    abup_uint32 EfsAdress;
    abup_uint32 EfsSize;
	
    abup_uint32 UmemAdress;
    abup_uint32 UmemSize;

    abup_uint32 PSSize; 
    abup_uint32 PsAdress;
	
    abup_uint32 NorBootloaderSize;           
    abup_uint32 NorBootloaderAdress;	
} ABUP_FLASH_PARTATIONT_T,*ABUP_FLASH_PARTATION_PTR;


 

abup_extern abup_int32 abup_write_flash(abup_uint32 start,abup_uint8* data_ptr, abup_uint32 len);
abup_extern ABUP_BOOL AbupUseFlash(void);

abup_extern  abup_int32 abup_read_flash(abup_uint32 addr,abup_uint8* data_ptr, abup_uint32 len);
abup_extern  abup_uint32 abup_addr_to_block(abup_uint32 addr);

abup_extern  abup_uint32 abup_init_flash(void);
abup_extern  void abup_init_block(abup_uint32 block_idx);
abup_extern void abup_erase_delata(void);
abup_extern void abup_erase_backup(void);

abup_extern abup_uint32 AbupGetBlockSize(void);
#endif


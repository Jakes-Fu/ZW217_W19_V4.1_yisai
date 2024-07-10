#include "sci_types.h"
#include "secure_write_flash.h"
#define NOR_FLASH_SECTOR_SIZE (1024*64)
PUBLIC void SecureBootWriteFlash(uint8 *start_addr, uint8 *temp_buffer)
{
	FLASH_UnlockSecureOnly(start_addr);
	FLASH_EraseSecureOnly(start_addr);
	FLASH_WriteSecureOnly((start_addr), temp_buffer, NOR_FLASH_SECTOR_SIZE);
}

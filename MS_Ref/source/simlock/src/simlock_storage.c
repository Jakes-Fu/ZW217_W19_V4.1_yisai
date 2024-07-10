/******************************************************************************
 ** File Name:      simlock_storage.c                     *
 ** Author:                                            *
 ** Date:           01/26/2016                                                *
 ** Copyright:          All Rights Reserved.   *                                    *
 ** Description:    simlock security stoarge                                                      *
 ** 
 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date                  01.26.2016                   *
 ** version              v2  
 ** Description         added Functions for reading simlock storage.
 **---------------------------------------------------------------------------*
 **
 ******************************************************************************/

#include "simlock_storage.h"
#include "flash_drvapi.h"

static SIMLOCK_STORAGE_STR *storage_transaction_buffer=NULL;
static uint32 storage_transation_start=0;
extern FLASH_GetSimlockDataAddr(void);

BOOLEAN simlock_storage_read(void* buffer,
                      uint32 stor_offset, uint32 stor_length,
                      uint32 offset, uint32 length) 
{
	 BOOLEAN ret=FALSE;
	 uint32 simlock_addr;
	 if(stor_length != STOR_LEN_ANY) {
		if(!(offset+length <= stor_length)){
			return FALSE;
		}
	 }
			
	 if(storage_transaction_buffer) 
	 {
		memcpy(buffer, storage_transaction_buffer+stor_offset+offset, length);
		ret = TRUE;
	 } 
	 else 
	 {
		simlock_addr=FLASH_GetSimlockDataAddr();
		ret = FLASH_Read(simlock_addr+stor_offset+offset, buffer, length);
	 }
	
	 return ret;
}
BOOLEAN simlock_storage_write(void* buffer,
                      uint32 stor_offset, uint32 stor_length,
                      uint32 offset, uint32 length) 
{
	 BOOLEAN ret=FALSE;
	 uint32 simlock_addr;
	 uint8* temp_buffer;
	 
	 if(stor_length != STOR_LEN_ANY){
		if(!(offset+length <= stor_length)){
			return FALSE;
		}
	 }
	 if(storage_transaction_buffer) {
		memcpy(storage_transaction_buffer+stor_offset+offset, buffer, length);
		ret = TRUE;
	 }else{
		temp_buffer = SCI_ALLOC(sizeof(SIMLOCK_STORAGE_STR));
		if(!temp_buffer){
		    return FALSE;
		}
		simlock_addr=FLASH_GetSimlockDataAddr();
		memcpy((void*)temp_buffer,(void*)simlock_addr,sizeof(SIMLOCK_STORAGE_STR));
		memcpy((uint8*)temp_buffer+stor_offset+offset,buffer,length);
		FLASH_Erase(simlock_addr);
		ret = FLASH_Write(simlock_addr,(uint8*)temp_buffer, sizeof(SIMLOCK_STORAGE_STR));
		SCI_FREE(temp_buffer);
	 }
	 return ret;
}
BOOLEAN storage_start_transaction(void)
{

       BOOLEAN ret=FALSE;
	 uint32 simlock_addr;
	if(storage_transaction_buffer)
	{
		// Transaction already started
		return FALSE;
	}

	storage_transaction_buffer = SCI_ALLOC(sizeof(SIMLOCK_STORAGE_STR));
	if(!storage_transaction_buffer)
	{
		return FALSE;
	}
      simlock_addr=FLASH_GetSimlockDataAddr();
	if(!FLASH_Read(simlock_addr, storage_transaction_buffer, sizeof(SIMLOCK_STORAGE_STR)))
	{
		SCI_FREE(storage_transaction_buffer);
		storage_transaction_buffer=NULL;
		return FALSE;
	}

	return TRUE;
}

uint32 storage_commit(void)
{
	
	BOOLEAN ret=FALSE;
	uint32 simlock_addr;
	if(!storage_transaction_buffer)
	{
		// Transaction wasn't started
		return FALSE;
	}
	simlock_addr=FLASH_GetSimlockDataAddr();
			
	FLASH_Erase(simlock_addr);
	ret = FLASH_Write(simlock_addr,(uint8*)storage_transaction_buffer, sizeof(SIMLOCK_STORAGE_STR));

	if(!ret)
	{
		return FALSE;
	}

	SCI_FREE(storage_transaction_buffer);
	storage_transaction_buffer=NULL;
	return TRUE;
}


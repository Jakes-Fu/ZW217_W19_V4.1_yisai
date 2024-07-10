/******************************************************************************
 ** File Name:      bsc_device.c                                                    *
 ** Author:                                                       *
 ** DATE:                                                         *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 bsc data. It manages bootloader write and read.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           	NAME             	DESCRIPTION                               *
 ** 										                                  *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "bsc_debug.h"
#include "bsc_device.h"
#include "nv_item_id.h"
#include "bsc.h"
#include "bsd.h"
#include <string.h>
//#include "nfc_drv.h"
//#include "nand_spec.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/******************************************************************************
 * Static Global Variables
 ******************************************************************************/
#define				BSC_BUF_SIZE	0x10000
LOCAL uint8			BSCDataBuf[BSC_BUF_SIZE];//Global variable to store Custom data at the time of BSCD_Open
LOCAL uint32 mHashLength = 0;

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define HASH_LENGTH_OFFSET			0x24//Added to get hash length offset
#else
#define HASH_LENGTH_OFFSET			0x20//Added to get hash length offset
#endif

#define HASH_LENGTH_SIZE			4
#define BOOTLOADER_HEADER_OFFSET   32
#define NOR_BOOTLOADER_ADDR		0x0

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    
// Global resource dependence : 
// Author :         
// Note :
/*****************************************************************************/
LOCAL __inline BSC_ERROR_E BSCD_Read_Boot();
LOCAL __inline BSC_ERROR_E  BSCD_Erase_Boot();
LOCAL BSC_ERROR_E BSCD_Head_Data_Compare(uint32 Size, const void *pDat);
extern uint16 crc16(uint16 crc, uint8 const *buffer, uint32 len);
LOCAL unsigned short _CheckSum (const unsigned int *src, int len);

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
 /*****************************************************************************/
// Description :    Open Bsc device,Get bootloader data from flash.
// PUBLIC resource dependence : 
// Author :         
// Note :
/*****************************************************************************/
PUBLIC BSC_ERROR_E BSCD_Open(void)
{
	if(BSCERR_NONE != BSCD_Read_BSC())
	{
		BSC_ERR_TRACE(("[BSC:BSCD_Open] BSCD_Read_Boot Read FALSE\r\n"));
		return  BSCERR_SYSTEM;
	}
		BSC_INF_TRACE(("[BSC:BSCD_Open] Read BSC data Successed!\r\n"));
	return BSCERR_NONE;
}

 /*****************************************************************************/
// Description :    Get bootloader data from flash.
// PUBLIC resource dependence : 
// Author :
// Note :
/*****************************************************************************/
LOCAL __inline BSC_ERROR_E BSCD_Read_BSC() 
{
	BSC_ERROR_E err;

	memset(BSCDataBuf,0,BSC_BUF_SIZE);
	memcpy((uint8 *)BSCDataBuf,(uint8 *)NOR_BOOTLOADER_ADDR,BSC_BUF_SIZE);

	memcpy(&mHashLength,HASH_LENGTH_OFFSET,HASH_LENGTH_SIZE);

	return BSCERR_NONE;
		
}

 /*****************************************************************************/
// Description :    check Sum.
// Local resource dependence : 
// Author :
// Note :
/*****************************************************************************/
LOCAL unsigned short _CheckSum (const unsigned int *src, int len)
{
    unsigned int   sum = 0;
    unsigned short *src_short_ptr = PNULL;

    while (len > 3)
    {
        sum += *src++;
        len -= 4;
    }

    src_short_ptr = (unsigned short *) src;

    if (0 != (len&0x2))
    {
        sum += * (src_short_ptr);
        src_short_ptr++;
    }

    if (0 != (len&0x1))
    {
        sum += * ( (unsigned char *) (src_short_ptr));
    }

    sum  = (sum >> 16) + (sum & 0x0FFFF);
    sum += (sum >> 16);

    return (unsigned short) (~sum);
}

LOCAL __inline BSC_ERROR_E BSCD_Write_BSC()
{
	BSC_ERROR_E ret = BSCERR_NONE;
	uint32 sector_size;
	uint16 sector_num;
	uint16 file_system_sector_num;

	HAL_GetFlashInfo(&sector_size, &sector_num, &file_system_sector_num);	

	HAL_UnlockSector(NOR_BOOTLOADER_ADDR);
	
	if(!HAL_FlashWrite(NOR_BOOTLOADER_ADDR,BSCDataBuf,sector_size))//BSC_BUF_SIZE
	{
		ret = BSCERR_SYSTEM;
	}
	
	HAL_LockSector(NOR_BOOTLOADER_ADDR);

	return ret;
}

 /*****************************************************************************/
// Description :    Erase flash bootloader data.
// Local resource dependence : 
// Author :
// Note :
/*****************************************************************************/
LOCAL __inline BSC_ERROR_E  BSCD_Erase_BSC()
{
	HAL_UnlockSector(NOR_BOOTLOADER_ADDR);

	if(TRUE != HAL_EraseSector(NOR_BOOTLOADER_ADDR))		
	{
		return BSCERR_SYSTEM;
	}

	HAL_LockSector(NOR_BOOTLOADER_ADDR);
	
	return BSCERR_NONE;
}

 PUBLIC uint32 BSCD_GetHashLength(void)	
 {
	return mHashLength;
 }

 /*****************************************************************************/
// Description :    Bsc device Close.
// PUBLIC resource dependence : 
// Author :
// Note :
/*****************************************************************************/
PUBLIC void BSCD_Close(void)
{		

}

 /*****************************************************************************/
// Description :    Read data from Bsc device.
// PUBLIC resource dependence : 
// Author :
// Note :
/*****************************************************************************/
PUBLIC BSC_ERROR_E BSCD_Read(uint16 ItemID, uint32 Size, const void *pDat)
{
	BSC_ERROR_E ret = BSCERR_NONE;
	
	BSC_LOG_TRACE(("[BSC:BSCD_Read] IN (ItemID = %u, Size = %u, pDat = 0x%x)\r\n",
		ItemID, Size, pDat));

	if((ItemID < BSC_MIN_ID) || (ItemID > BSC_MAX_ID))
	{
		return BSCERR_INVALID_PARAM;
	}
	if(BSC_CUSTOM_DATA == ItemID)
	{
		if(Size > CUSTOM_DATA_SIZE)
			return BSCERR_INVALID_PARAM;
		memcpy((uint8 *)pDat,(uint8 *)(BSCDataBuf + CUSTOM_DATA_ADDR),Size);
	}
	else if(BSC_BOOT_DATA == ItemID)
	{
		memcpy((uint8 *)pDat,(volatile uint8 *)NOR_BOOTLOADER_ADDR,Size);
	}
	else
	{
		ret = BSCERR_INVALID_PARAM;
	}

	return ret;
}

 /*****************************************************************************/
// Description :    Write data to Bsc device.
// PUBLIC resource dependence : 
// Author :
// Note :
/*****************************************************************************/
PUBLIC BSC_ERROR_E BSCD_Write(uint16 ItemID, uint32 Size, const void *pDat)
{
	uint8	j;
	uint8	temp_flag = 0;

	if((ItemID < BSC_MIN_ID) || (ItemID > BSC_MAX_ID))
	{
		return BSCERR_INVALID_PARAM;
	}
	
	if(BSC_CUSTOM_DATA == ItemID)
	{
		if(Size > CUSTOM_DATA_SIZE)
			return BSCERR_INVALID_PARAM;
		memcpy(BSCDataBuf + CUSTOM_DATA_ADDR,pDat,Size);
	}
	else
	{
		return BSCERR_INVALID_PARAM;
	}

	for (j=0; j<3; j++)
	{   
		if(BSCERR_NONE != BSCD_Erase_BSC())
		{
			BSC_ERR_TRACE(("[BSC:BSCD_Write] Erase BSC data failed.\r\n"));
			return BSCERR_SYSTEM;
		}

		if(BSCERR_NONE != BSCD_Write_BSC())
		{
			BSC_ERR_TRACE(("[BSC:BSCD_Write] Write BSC data failed.\r\n"));
			return BSCERR_SYSTEM;
		}

		temp_flag = memcmp((volatile uint8 *)NOR_BOOTLOADER_ADDR,BSCDataBuf,mHashLength<<2);

		if(0 != temp_flag)
		{
			BSC_LOG_TRACE(("[BSC:BSCD_Write] Write failed\r\n"));
		}
		else
		{
			break;
		}		
	}

	if(3 == j)return BSCERR_INCOMPATIBLE;

	BSC_LOG_TRACE(("[BSC:BSCD_Write] Write OK\r\n"));
	
	return BSCERR_NONE;
	
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif //


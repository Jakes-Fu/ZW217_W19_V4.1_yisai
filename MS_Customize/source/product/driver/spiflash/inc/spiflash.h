/******************************************************************************
 ** File Name:      spiflash.h                                                 *
 ** Author:         Fei.Zhang                                                 *
 ** DATE:           01/15/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of SPI Flash       *
 ******************************************************************************/
#ifndef _SPIFLASH_H_
#define _SPIFLASH_H_

//#include "flash.h"   /* basic type  */
#include "sci_types.h"
#include "flash.h"
#include "spiflash_spec.h" 

#define SPI_MODE            0x0
#define QPI_MODE            0x1

#ifdef SECURE_BOOT_SUPPORT
#define KERNEL_HEADER_SIZE		1024		//size of header in kernel added for storing secure boot data
#define FLASH_CONFIG_OFF		0x28		//Flash type configuration offset in Bootloader
#endif

#define SPIFLASH_CS0                0
#define SPIFLASH_CS1                1

#define SPIFLASH_MAGIC_FIRST    (0x4D414748UL)    // MAGH
#define SPIFLASH_MAGIC_SECOND   (0x5346432EUL) // SFC.
#define SPIFLASH_MAGIC_END      (0x5441494CUL)    // TAIL
#define SPIFLASH_INVAILD_DATA   (~0x0UL)

typedef struct SPIFLASH_TYPE_CONFIG_Tag
{
  uint32   magic_first;
  uint32   magic_second; 
  uint32   cs0_MID;
  uint32   cs0_DID1;
  uint32   cs0_DID2;
  uint32   cs0_desity;
  uint32   cs1_MID;
  uint32   cs1_DID1;
  uint32   cs1_DID2;
  uint32   cs1_desity;
  uint32   reserved;
  uint32   magic_end;  
}SPIFLASH_TYPE_CONFIG_T;

/*****************************************************************************/
// Description :    This function is used to read manufacturer ID and device ID
// Global resource dependence :
// Author :         Fei.Zhang
// Note :           
/*****************************************************************************/
PUBLIC BOOLEAN SPIFLASH_ReadID(int cs, uint8 *pMID, uint8 *pDID1, uint8 *pDID2);

PUBLIC BOOLEAN SPIFLASH_GetID(uint cs, uint8 *pMID, uint8 *pDID1, uint8 *pDID2);

PUBLIC void SPIFLASH_ResetQPIFlash(int cs);

PUBLIC SPIFLASH_SPEC_T *SPIFLASH_SPEC_Get(uint16 nMID, uint16 nDID1, uint16 nDID2);

PUBLIC SPIFLASH_TYPE_CONFIG_T*  SPIFLASH_GetFlashType(void);

PUBLIC BOOLEAN  SPIFLASH_WriteWord(uint32 addr, uint16 data);

PUBLIC uint32 SPIFLASH_LockRegion(uint32 addr, uint32 size);

PUBLIC void SPIFLASH_ResetRegister(int cs);
PUBLIC uint32 SPIFLASH_GetFlashTotalSize(void);


#endif    //_SPI_DRV_H_


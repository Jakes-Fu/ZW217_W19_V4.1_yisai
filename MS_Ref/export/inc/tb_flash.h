#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
//#include "flash.h"
#ifndef _TB_FLASH_H_
#define _TB_FLASH_H_
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
extern uint16 FLASH_FastSearchWord (
    uint32    addr,         // the start address that will be read
    uint32      end_addr,
    uint16      cmpdata   /* compare data: maybe 0xfffe to kick out 0xffff(empty sector), maybe 0x1000,to find the first available data <=0x1000*/
);

#define HAL_INVALID_FLASH_ADDR      0xFFFFFFFF  //FLASH_INVALID_ADDR
#define HAL_INVALID_SECTOR          0xFFFF //FLASH_INVALID_SECTOR

#define HAL_InitFlash               FLASH_Init
#define HAL_CloseFlash              FLASH_Close
#define HAL_GetFlashInfo            FLASH_GetDeviceInfo
#define HAL_AddrToSector            FLASH_AddrToSector
#define HAL_SectorToAddr            FLASH_SectorToAddr
#define HAL_ReadWord                FLASH_ReadWord
#define HAL_ReadDWord				FLASH_ReadDWord
#define HAL_FlashRead               FLASH_Read
#define HAL_WriteWord               FLASH_WriteWord
#define HAL_FlashWrite              FLASH_Write
#define HAL_FlashCopy               FLASH_Copy
#define HAL_CheckRevisable          FLASH_CheckRevisable
#define HAL_IsEraseCompleted        FLASH_IsEraseCompleted
#define HAL_EraseSector             FLASH_Erase
#define HAL_FlashSuspend            FLASH_Suspend
#define HAL_FlashResume             FLASH_Resume
#define HAL_UnlockSector            FLASH_Unlock
#define HAL_LockSector              FLASH_Lock
#define HAL_GetFixedNvitemAddr      FLASH_GetFixedNvitemAddr
#define HAL_IsSimulate              FLASH_IsSimulate
#define HAL_GetFotaNvitemAddr      FLASH_GetFotaNvitemAddr

#define HAL_SetSuspendFlag          FLASH_SetSuspendFlag
#define HAL_FormatFlash             FLASH_Format
#define HAL_GetMMIResPtr            FLASH_GetMMIResPtr
#define HAL_FastSearchWord          FLASH_FastSearchWord

//HAL_GetNVMAddress just used in TDPS_UEIT now.
#define HAL_GetNVMAddress           FLASH_GetNVAddress
#define HAL_MMI_FlashWrite          mmi_FLASH_Write   /*add for ueit mmi simulator only, name???*/
//
// @lin.liu.(2003-12-04). CR: MS5446
// added some function to support check sum feature
//
#define HAL_IsEmptySpace            FLASH_IsEmptySpace
#define HAL_GetCheckSum             FLASH_GetCheckSum
#define HAL_ReadWordByCount         FLASH_ReadWordByCount
//@ End added. CR:5446

/**********************************************************************
// Description:
//      Init the flash device.
// Global resource dependence :
// Author:
//        Lin.liu
// Note:
***********************************************************************/
BOOLEAN HAL_InitFlash (void);

/**********************************************************************
// Description:
//      Close the flash device.
// Global resource dependence :
// Author:
//      Lin.liu
// Note:
***********************************************************************/
void HAL_CloseFlash (void);

/**********************************************************************
// Description:
//      Get Infomation of flash device.
// Global resource dependence :
// Author:
//      Lin.liu
// Input:
//      sector_size_ptr : pointer to the VAR that received the sector size.
//      sector_num_ptr  : pointer to the VAR that received the sector number.
// Note:
***********************************************************************/
void HAL_GetFlashInfo (
    uint32 *sector_size_ptr,
    uint16 *sector_num_ptr,
    uint16 *file_system_sector_num_ptr);  /* output the file system sector num. */

/**********************************************************************
// Description:
//      Get the no. of the sector which contain the address.
// Global resource dependence :
// Author:
//      Lin.liu
// Return:
//      The no. of the sector.(If the addr is out of range, return 0xFFFF)
// Note:
***********************************************************************/
uint16 HAL_AddrToSector (uint32 addr);

/**********************************************************************
// Description:
//      Convert the given sector to physical address.
// Global resource dependence :
// Author:
//        Lin.liu
// Input:
//      sector_no : the no. of the given sector.
// Return :
//      the base address of the given sector.(if the no. is invalid, return 0xFFFFFFFF)
// Note:
***********************************************************************/
uint32 HAL_SectorToAddr (uint16  sector_no);

/**********************************************************************
// Description:
//      Read one word from the flash
// Global resource dependence :
// Author:
//      Lin.liu
// Input :
//      addr : the address.
// Return :
//      the word has been read.
// Note:
***********************************************************************/
uint16 HAL_ReadWord (uint32 addr);


/**********************************************************************
// Description:
//      Read one word from the flash
// Global resource dependence :
// Author:
//      Lin.liu
// Input :
//      addr : the address.
// Return :
//      the Dword has been read.
// Note:
***********************************************************************/
uint32 HAL_ReadDword(uint32 addr);

/**********************************************************************
// Description:
//      Read the given bytes from flash.
// Global resource dependence :
// Author:
//      Lin.liu
// Input:
//      addr : the address of the given area.
//      buf  : the buffer that store content.
//      len  : the number of bytes need be read.
// Return:
//      TRUE : If the operation is successful and all bytes has been read.
// Note:
//
***********************************************************************/
BOOLEAN HAL_FlashRead (uint32 addr, uint8 *buf, uint32 read_len);

/**********************************************************************
// Description:
//      Write one word to flash.
// Global resource dependence :
// Author:
//      Lin.liu
// Input:
//      addr  : The address that will be written.
//      data  : content.
// Return:
//      TRUE  : if the operation is successful.
// Note:
//      addr must be even.
***********************************************************************/
BOOLEAN HAL_WriteWord (uint32 addr, uint16 data);

/**********************************************************************
// Description:
//      Write content to the given area.
// Global resource dependence :
// Author:
//      Lin.liu
// Input:
//      addr : the address of the given area.
//      buf  : the buffer that contain content.
//      len  : the number of bytes of the buffer.
// Return:
//      TRUE : If the operation is successful and all bytes are written to flash.
// Note:
//
***********************************************************************/
BOOLEAN HAL_FlashWrite (uint32 addr, const uint8 *buf, uint32 len);

/**********************************************************************
// Description:
//      Copy data from one area to another.
// Global resource dependence :
// Author:
//        Lin.liu
// Input:
//      dest : the address of dest area.
//      sour : the address of sour area.
//      len  : the number of bytes that need be moved.
// Return:
//      TRUE : If all bytes have been moved successfully.
// Note:
//      The dest and sour both are word boundary.
***********************************************************************/
BOOLEAN HAL_FlashCopy (uint32 dest, uint32 sour, uint32 len);

/**********************************************************************
// Description:
//      Check can write the given data to the area?
// Global resource dependence :
// Author:
//      Lin.liu
// Input:
//      addr : the address of the given area.
//      buf  : the buffer that contain the compare content.
//      len  : the number of bytes in buffer.
// Output:
//      TRUE : If we can program directly.
//      FALSE:
// Note:
//      Because program can only change the bit one way(eg. '1' --> '0').
//      and erase can change bit from '0' to '1', but need change all bit
//      in the whole sector.
***********************************************************************/
BOOLEAN HAL_CheckRevisable (uint32 addr, const uint8 *buf, uint32 len);

/**********************************************************************
// Description:
//      Check erase operation has been completed?
// Global resource dependence :
// Author:
//      Lin.liu
// Return:
//      TRUE : Has completed.  False : Not.
// Note:
//      This function used in Multi-Task entironment.
***********************************************************************/
BOOLEAN HAL_IsEraseCompleted (void);

/**********************************************************************
// Description:
//      Erase a sector.
// Global resource dependence :
// Author:
//      Lin.liu
// Input:
//      sector_base : the base address of the given sector.
// Return:
//      TRUE  : The erase operation successfully.
//      FALSE : The operation failed.
// Note:
//      This operation maybe need 1second or more.
***********************************************************************/
BOOLEAN HAL_EraseSector (uint32 sector_base);

/**********************************************************************
// Description:
//      Suspend the program/erase operation. so can read from flash.
// Global resource dependence :
// Author:
//      Lin.liu
// Note:
***********************************************************************/

void HAL_FlashSuspend (void);

/**********************************************************************
// Description:
//      Resume the operation if has been suspended previous.
// Global resource dependence :
// Author:
//        Lin.liu
// Note:
***********************************************************************/
void HAL_FlashResume (void);

/**********************************************************************
// Description:
//      Unlock the given sector, and program/erase being permited.
// Global resource dependence :
// Author:
//      Lin.liu
// Input:
//      sector_addr : the address that within the given sector.
// Note:
***********************************************************************/
void HAL_UnlockSector (uint32 sector_addr);

/**********************************************************************
// Description:
//      Lock the given sector, and program/erase is not permited.
// Global resource dependence :
// Author:
//      Lin.liu
// Input:
//      sector_addr : the address that within the given sector.
// Note:
***********************************************************************/
void HAL_LockSector (uint32  sector_addr);


/**********************************************************************
// Description:
//      Get address of classmark data.
// Global resource dependence :
// Author:
//      Lin.liu
// Return :
//      The address of first byte (classmark data).
// Note:
***********************************************************************/
void HAL_GetFixedNvitemAddr (uint32 *start, uint32 *end);
void HAL_GetFotaNvitemAddr (uint32 *start, uint32 *end);


/*****************************************************************************/
//  Description :
//      Check use SRAM for simulate ?
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
BOOLEAN HAL_IsSimulate (void);


//
// @lin.liu.(2003-12-04). CR: MS5446
// added some function to support check sum feature
//

/*****************************************************************************/
//  Description :
//      Check the content of the given area are all 0xFFFF
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
BOOLEAN HAL_IsEmptySpace (uint32  start_address, uint32  end_address);


/*****************************************************************************/
//  Description :
//      calculate the check sum
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
uint16 HAL_GetCheckSum (uint32 start_address, uint16 count);

/*****************************************************************************/
//  Description :
//      Check use SRAM for simulate ?
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void HAL_ReadWordByCount (uint32 start_address,  uint16 *ptr, uint32 count);

//@End Added. CR:5446

//
// HAL_FormatFlash.
// This function should be called after disable interrupt and task switch.
// Maybe need a long time( 3s --> 35s )
//
// Input:
//     param_1 should be equal to ~param_2
//
BOOLEAN HAL_FormatFlash (uint32 param_1, uint32 param_2);

/*****************************************************************************/
//  Description :
//      Set the flag if the erasing/programming sector be in code bank
//      .
//  Global resource dependence :
//      flash_bank_num
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
void HAL_SetSuspendFlag (BOOLEAN flag);

//  Description :
//      Get MMI res address.
//      .
//  Global resource dependence :
//
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/

uint32 HAL_GetMMIResPtr (void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


#endif // TB_FLASH_H







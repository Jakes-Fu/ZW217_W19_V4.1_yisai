/******************************************************************************
 ** File Name:      tf_spiflash.h                                             *
 ** Author:         yijun.yang                                             *
 ** DATE:           04/07/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 SPI flash Low-level drvier.                             * 
 **                 This driver support ALL SPI flash                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/07/2011     yijun.yang       Poring from spiflash.c of main code.      *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifndef __TF_SPIFLASH_H__
#define __TF_SPIFLASH_H__

#if 0
#include "sci_api.h"
#include "sfc_reg_v0.h"
#include "flash.h"
#include "spiflash.h"

#include "tf_drv.h"
#include "tf_sfc_drv.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif

#if 0
/**---------------------------------------------------------------------------*
 **              variable declare                                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variable                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
PUBLIC void SPIFLASH_EnterSPIRead(void);

PUBLIC void SPIFLASH_EnterSPI4Read(void);

PUBLIC void SPIFLASH_SetReadPara(uint8 para);

PUBLIC void SPIFLASH_ResetRegister(void);

PUBLIC void SPIFLASH_ResetQPIFlash(void);

/*****************************************************************************/
// Description :    This function is used to init flash
// Global resource dependence :
// Author :
// Note :           
/*****************************************************************************/  
PUBLIC int SPIFLASH_Init(void);
/*****************************************************************************/
// Description :    This function is used to read manufacturer ID and device ID
// Global resource dependence :
// Author :
// Note :           
/*****************************************************************************/
PUBLIC BOOLEAN SPIFLASH_ReadID(uint8 *pMID, uint8 *pDID1, uint8 *pDID2);

/*****************************************************************************/
// Description :    buffer write function
// Global resource dependence :
// s_flash_sector_size
// Author :
// Parameter:
// Note :           
/*****************************************************************************/
PUBLIC BOOLEAN SPIFLASH_WriteByte(    // If the data being written to flash, return TRUE.
    uint32       addr,              // the start address that will be written.
    const uint8 * buf,             // Pointer to the buffer containing the data to be written.
    uint32       counter);           // number of byte to write.

/*****************************************************************************/
// Description :    buffer write function
// Global resource dependence :
// s_flash_sector_size
// Author :
// Parameter:
// Note :           
/*****************************************************************************/
PUBLIC BOOLEAN SPIFLASH_WriteBuffer(    // If the data being written to flash, return TRUE.
    uint32       addr,              // the start address that will be written.
    const uint8 * buf,             // Pointer to the buffer containing the data to be written.
    uint32       counter);           // number of byte to write.

/*****************************************************************************/
//     Description :
//      The function write word to flash.
//
//    Global resource dependence :
//      None
//  Author:
//    Note:
//      The address MUST be word boundary.
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written 
//      is not correct.
/*****************************************************************************/
PUBLIC BOOLEAN  SPIFLASH_WriteWord(
    uint32      addr,
    uint16      data);


/*****************************************************************************/
//     Description :
//      The function write data to flash.
//
//    Global resource dependence :
//      None
//
//  Author:
//    Note:
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written 
//      is not correct.
/*****************************************************************************/
PUBLIC BOOLEAN SPIFLASH_Write(    // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8      * buf,   // Pointer to the buffer containing the data to be written.
    uint32       len);   // number of bytes to write.

// enter_read/exit_read should be called with pair
//
PUBLIC void SPIFLASH_EnterRead( void );

PUBLIC void SPIFLASH_ExitRead( void );

/*****************************************************************************/
//     Description :
//      Read word(two byte) from the given address.
//
//    Global resource dependence :
//      None
//  Author:
//    Note:
//      the address MUST be word boundary.
/*****************************************************************************/
uint16 SPIFLASH_ReadWord(  // The word that has read from the given address.
    uint32 addr);        // the address that will be read.

/*****************************************************************************/
//     Description :
//      The function reads up to size bytes from the device and stores them in buffer.
//
//    Global resource dependence :
//      None
//  Author:
//    Note:
//      
/*****************************************************************************/
BOOLEAN SPIFLASH_Read(      // If read successfully, return true, else false;
    uint32    addr,      // the start address that will be read
    uint8   * buf,       // store the data that being read to this buffer.
    uint32    read_len);  // the count of bytes that will be read.

/*****************************************************************************/
//     Description :
//      Erase the 4KB sector that contain the address.
//
//    Global resource dependence :
//      None
//
//  Author:
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//      
/*****************************************************************************/
PUBLIC BOOLEAN SPIFLASH_EraseSetor(
    uint32 addr);

PUBLIC BOOLEAN SPIFLASH_EraseMultSetor(uint32 addr, uint32 sec_num);

/*****************************************************************************/
//     Description :
//      Erase the 32K Block that contain the address.
//
//    Global resource dependence :
//      None
//
//  Author:
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//      
/*****************************************************************************/
PUBLIC BOOLEAN SPIFLASH_EraseBlock_32KB(
    uint32 addr);

/*****************************************************************************/
//     Description :
//      Erase the 64K Block that contain the address.
//
//    Global resource dependence :
//      None
//
//  Author:
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//      
/*****************************************************************************/
PUBLIC BOOLEAN SPIFLASH_EraseBlock_64KB(
    uint32 addr);

/*****************************************************************************/
//     Description :
//      Erase the whole chip.
//
//    Global resource dependence :
//      None
//
//  Author:
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//      
/*****************************************************************************/
PUBLIC BOOLEAN SPIFLASH_EraseChip(void);

/*****************************************************************************/
//     Description :
//      Erase the sector that contain the address.
//
//    Global resource dependence :
//      None
//
//  Author:
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//      
/*****************************************************************************/
BOOLEAN SPIFLASH_Erase(
    uint32 addr);

PUBLIC uint32 TF_Write_Area(uint32 addr, char *buf, uint32 size);

PUBLIC int TF_GetSecsize(uint32 start_addr);

int flash_erase_block(uint32 addr, uint32 block_size);

uint32 TF_Erase(uint32 start_addr,  uint32 size);

/**********************************************
* flash_read_word
***********************************************/
uint16 TF_Read_Word( uint32 addr );

/**********************************************
* flash_read
***********************************************/
uint32 TF_Read(uint32 addr, uint8 * buf, uint32 read_len);

uint32 TF_Write_Enable(uint32 start_addr, uint32  enable);

/*********************************************************************/
//  Discription: This func will used to do flash init
//  Global resource dependence: none 
//  Author: 
//  Note:
/*********************************************************************/
PUBLIC  BOOLEAN  TF_Init(void);
#endif
#define  TF_FlashWriteEnable      FDL_FlashWriteEnable
#define TF_FlashInit                      SPIFLASH_HWInit
#define TF_FlashRead                    FDL_FlashRead
#define TF_Read_Word                  FDL_FlashReadWord
#define TF_Write_Word                  FDL_FlashWriteWord
#define TF_FlashErase                   FDL_FlashErase//SFC_Erase
#define TF_GetSecsize                   FDL_FlashCheckSectorSize
#define TF_FlashWriteArea            FDL_FlashWriteArea

#define TF_DebugTrace                   FDL_SendTrace

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif  /* #ifndef __TF_SPIFLASH_H__ */


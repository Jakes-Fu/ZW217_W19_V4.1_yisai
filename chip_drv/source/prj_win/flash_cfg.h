/******************************************************************************
 ** File Name:      flash_cfg.h                                               *
 ** Author:         Daniel.ding                                               *
 ** DATE:           2006-04-19                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the memory config                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/19/2006     Daniel.ding      Modify it for SC6800                      *
 *****************************************************************************/
#ifndef _FLASH_CFG_H_
#define _FLASH_CFG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
extern const uint32  FLASH_FileSystemNum;
extern const uint32  FLASH_SectorNum;
extern const uint32  FLASH_SectorSize;
extern const uint32  FLASH_StartAddrr;
extern const uint32  FLASH_BankAddr;
extern const uint32  FLASH_NV_BankAddr;
extern const uint32  FLASH_Size;

extern const uint32  FLASH_FixedNvitemAddr;
extern const uint32  FLASH_ProductInfoAddr;

// The offset of the sram that be used to simulator
extern const uint32  FLASH_EmuOffsetAddr;

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif


#endif  // MEM_CFG_H
/******************************************************************************
 ** File Name:     flash_drv.h                                                 *
 ** Description:                                                              *
 **        norflash operations                                                *
 ** Author:         Younger.yang                                              *
 ** DATE:           11/23/2006                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/23/2006      Younger.yang       Create for Jiuyang                     *
 ******************************************************************************/

#ifndef _TF_DRV_NOR_H_
#define _TF_DRV_NOR_H_

#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Macro defines
 **---------------------------------------------------------------------------*/


#define  BIG_ENDIAN    1


#define SR_0      0x1
#define SR_1      0x2
#define SR_2      0x4
#define SR_3      0x8
#define SR_4      0x10
#define SR_5      0x20
#define SR_6      0x40
#define SR_7      0x80

/**---------------------------------------------------------------------------*
 **                         Functions
 **---------------------------------------------------------------------------*/
#define TF_FlashInit                      flash_init
#define TF_FlashRead                     FDL_FlashRead
#define TF_Read_Word                  FDL_FlashReadWord
#define TF_Write_Word                  FDL_FlashWriteWord
#define TF_FlashErase                   FDL_FlashErase
#define TF_GetSecsize                   FDL_FlashCheckSectorSize
#define TF_FlashWriteArea           FDL_FlashWriteArea

#define TF_DebugTrace                   FDL_SendTrace

#endif	//_TF_DRV_NOR_H_

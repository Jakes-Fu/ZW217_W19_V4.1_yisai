/******************************************************************************
 ** File Name:     flash_cfg.h                                                *
 ** Description:                                                              *
 **        norflash configuration                                             *
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


#include "sci_types.h"


/**---------------------------------------------------------------------------*
 **                         Macro defines
 **---------------------------------------------------------------------------*/

//Flash type define
#define FLASH_AMD_ID         0x1
#define FLASH_ST_ID          0x20
#define FLASH_INTEL_ID       0x89
#define FLASH_SAMSUNG_ID     0xEC         //@David.Jia 2006.3.13 cr43532
#define FLASH_SST_ID         0xBF
#define FLASH_TSB_ID        0x98
#define FLASH_CFEON_ID       0x7F

#define TOP_BOOT_FLAG        0x1
#define BOTTOM_BOOT_FLAG     0x2

/**--------------------------------------------------------------------------*
 **                         Data Structures                                  *
 **--------------------------------------------------------------------------*/

typedef uint32 (*FLAGH_GET_SECTOR_SIZE) (uint32 bootflag, uint32 addr);


//norflash info
typedef struct _norflash_info
{
    uint32  manufacturer_id;
    uint32  device_id;
    uint32  extend_id;

    uint32  writebufferlen;  //if the norflash support write buffer,set the write buffer len according to norflash's spec.
    uint32  bootflag;  //the norflash support bottom boot or top boot or bottom and top boot or single size sector.

    FLAGH_GET_SECTOR_SIZE get_sector_size;  //get the sector size
} NORFLASH_INFO_T;
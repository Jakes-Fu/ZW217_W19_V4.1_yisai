/******************************************************************************
 ** File Name:     spiflash_cfg.c                                              *
 ** Description:														 	 *
 ** This file contains spiflash config info                                  *
 ** Author:         Guangqiao.she                                            	 *
 ** DATE:           07/2022                                              *
 ** Copyright:      2022 UNISOC, Incoporated. All Rights Reserved.       *
 ** Description:                                                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                      *
 ** -------------------------------------------------------------------------*
 ** DATE           NAME          	   DESCRIPTION                              *
 **07/2022      Guangqiao.she      Create									 *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "spiflash.h"
#include "mem_cfg.h"
//#include "emc_drv.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

#define _NULL 0xFF

static struct SPIFLASH_ExtCfg cfg_GD25LE128E = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25LQ256 = {SPIFLASH_VOLTAGE_1V8, 0x02000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25LQ128 = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25LQ64B = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25LQ32B = {SPIFLASH_VOLTAGE_1V8, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25LQ16 = {SPIFLASH_VOLTAGE_1V8, 0x00200000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25Q64B = {SPIFLASH_VOLTAGE_3V, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25Q32B = {SPIFLASH_VOLTAGE_3V, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_N25W128 = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};

static struct SPIFLASH_ExtCfg cfg_N25W064 = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_N25W032 = {SPIFLASH_VOLTAGE_1V8, 0x00400000,0,0,0,0,0};

static struct SPIFLASH_ExtCfg cfg_MX25U3235E = {SPIFLASH_VOLTAGE_1V8, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_MX25U6435E = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_MX25U12835E = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q128JW = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q128FW = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q64FW = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q32FV = {SPIFLASH_VOLTAGE_3V, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q32JW = {SPIFLASH_VOLTAGE_1V8, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q64FVSTIM = {SPIFLASH_VOLTAGE_3V, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q128FV = {SPIFLASH_VOLTAGE_3V, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q256FV = {SPIFLASH_VOLTAGE_3V, 0x02000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_F25D64QA = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_FM25M128A = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_FM25M64C = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_FM25M64A = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_FM25Q32A = {SPIFLASH_VOLTAGE_3V, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_FM25M32A = {SPIFLASH_VOLTAGE_1V8, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_BG25Q128AL = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};

#ifdef MULTI_FLASH_SUPPORT //FLASH_SIZE_128MBITX32MBIT

#pragma arm section rodata = "SPIFLASH_CFG_TABLE_DETAIL"
PUBLIC  const NOR_FLASH_CONFIG_T  s_platform_patitiion_config =
{
    1,            				/*single-bank*/
    RUNNIN_NV_SECTOR_COUNT,       /*runningNV/EFS sector number*/
    0,            				/* abandon */
    FLASH_SECTOR_SIZE,  		/*sector size*/
    PHY_FLASH_START_ADDR,   	            /*start address*/
    RUNNIN_NV_BASE_ADDR,        /*runningNV/EFS start address*/
    PHY_FLASH_SIZE,   		    /*flash size*/
    FIXED_NV_BASE_ADDR, //FIXED_NV_BASE_ADDR,         /*fixed nv start address*/
    PRODUCT_NV_BASE_ADDR,       /*product info start address*/
    MMI_RES_BASE_ADDR,         /*resource start address*/
    MMI_RESOURCE_SIZE,
    UDISK_BASE_ADDR,            /*U Disk start address	*/
    UDISK_SIZE,                 /*U Disk Size*/
    -1,
   SIM_LOCK_BASE_ADDR,    //simlock Start Addr
#ifdef NOR_BOOTLOADER_SUPPORT
    AP_BASE_ADDR,     //AP Start Addr
#endif
    OPERATOR_DATA_BASE_ADDR,  // operator_data_addr
    OPERATOR_DATA_SIZE,            // operator_data_size

    CP_BASE_ADDR, //cp_addr
    CP_SIZE, //cp_size
    CONFIG_NV_BASE_ADDR,  // config_nv_addr
    CONFIG_NV_SIZE,            // config_nv_size

    CRYSTAL_AGING_ADDR,
    CRYSTAL_AGING_SIZE,
    IMS_TOGGEL_CFG_ADDR,
    IMS_TOGGEL_CFG_SIZE,
#if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
    FOTA_BASE_ADDR,
    FOTA_SIZE
    FOTA_BOOTLOADER_ADDR,
    FOTA_BOOTLOADER_SIZE
#endif

};//lint !e570

PUBLIC  const NOR_FLASH_CONFIG_T  s_platform_patitiion_config_B =
{
    1,            				/*single-bank*/
    RUNNIN_NV_SECTOR_COUNT,       /*runningNV/EFS sector number*/
    0,            				/* abandon */
    FLASH_SECTOR_SIZE,  		/*sector size*/
    CP_BASE_ADDR,   	            /*start address*/
    RUNNIN_NV_BASE_ADDR,        /*runningNV/EFS start address*/
    PHY_FLASH_SIZE_B,   		    /*flash size*/
    FIXED_NV_BASE_ADDR,         /*fixed nv start address*/
    PRODUCT_NV_BASE_ADDR,       /*product info start address*/
    MMI_RES_BASE_ADDR,          /*resource start address*/
   MMI_RESOURCE_SIZE,
    UDISK_BASE_ADDR,           /*U Disk start address	*/
    UDISK_SIZE,                 /*U Disk Size*/
    -1,                         /*blbin*/
	SIM_LOCK_BASE_ADDR,    //simlock Start Addr
#ifdef NOR_BOOTLOADER_SUPPORT
     -1,     //AP Start Addr
 #endif
    OPERATOR_DATA_BASE_ADDR,
    OPERATOR_DATA_SIZE,
    CP_BASE_ADDR, //cp_addr
    CP_SIZE, //cp_size
    CONFIG_NV_BASE_ADDR,  // config_nv_addr
    CONFIG_NV_SIZE,            // config_nv_size
    CRYSTAL_AGING_ADDR,
    CRYSTAL_AGING_SIZE,
    IMS_TOGGEL_CFG_ADDR,
    IMS_TOGGEL_CFG_SIZE,

#if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
    FOTA_BASE_ADDR,
    FOTA_SIZE,
    FOTA_BOOTLOADER_ADDR,
    FOTA_BOOTLOADER_SIZE
#endif

};//lint !e570

#elif FLASH_SIZE_128MBIT

#pragma arm section rodata = "SPIFLASH_CFG_TABLE_DETAIL"
PUBLIC  const NOR_FLASH_CONFIG_T  s_platform_patitiion_config =
{
    1,            				/*single-bank*/
    RUNNIN_NV_SECTOR_COUNT,       /*runningNV/EFS sector number*/
    0,            				/* abandon */
    FLASH_SECTOR_SIZE,  		/*sector size*/
    PHY_FLASH_START_ADDR,   	            /*start address*/
    RUNNIN_NV_BASE_ADDR,        /*runningNV/EFS start address*/
    PHY_FLASH_SIZE,   		    /*flash size*/
    FIXED_NV_BASE_ADDR, //FIXED_NV_BASE_ADDR,         /*fixed nv start address*/
    PRODUCT_NV_BASE_ADDR,       /*product info start address*/
    MMI_RES_BASE_ADDR,          /*resource start address*/

    MMI_RESOURCE_SIZE,

    UDISK_BASE_ADDR,           /*U Disk start address	*/
    UDISK_SIZE,                 /*U Disk Size*/
    -1,                         /*blbin*/
   SIM_LOCK_BASE_ADDR,    //simlock Start Addr
#ifdef NOR_BOOTLOADER_SUPPORT
    AP_BASE_ADDR,     //PS Start Addr
 #endif
    OPERATOR_DATA_BASE_ADDR,  // operator_data_addr
    OPERATOR_DATA_SIZE,            // operator_data_size


    CP_BASE_ADDR, //cp_addr
    CP_SIZE, //cp_size
    CONFIG_NV_BASE_ADDR,  // config_nv_addr
    CONFIG_NV_SIZE,            // config_nv_size

    CRYSTAL_AGING_ADDR,
    CRYSTAL_AGING_SIZE,
    IMS_TOGGEL_CFG_ADDR,
    IMS_TOGGEL_CFG_SIZE,
#if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
    FOTA_BASE_ADDR,
    FOTA_SIZE,
    FOTA_BOOTLOADER_ADDR,
    FOTA_BOOTLOADER_SIZE
#endif
};//lint !e570
#else
#error please add FLASH_SIZE_XXX macor here
#endif


#ifdef FLASH_SIZE_128MBITX32MBIT
#define FLASH_LOGIC_PATITION_B   &s_platform_patitiion_config_B
#else
#define FLASH_LOGIC_PATITION_B   (NULL)
#endif

#define FLASH_LOGIC_PATITION   &s_platform_patitiion_config


#pragma arm section rodata = "SPIFLASH_CFG_TABLE"

LOCAL const SPIFLASH_SPEC_T SpiFlashSpec[] = {
    /***************************************************************************/
    /* nMID, nDID1, nDID2, nQEPos, nWIPPos, EnterQPI, ExitQPI, cmd_4pp, cmd_4read, suspend, resume, reset_support, QPI_support, spiflash_cfg, pExtcfg*/
    /***************************************************************************/
    { 0xC8, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LE128E},//GD25LE128E
    { 0xC8, 0x60, 0x19, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ256},//GD25LQ256
    //{ 0xC8, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ128},//GD25LQ128
    { 0xC8, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ128},//GD25LQ128
    { 0xC8, 0x60, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION_B, &cfg_GD25LQ64B},//GD25LQ64B
    { 0xC8, 0x60, 0x16, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ32B},//GD25LQ32B
    { 0xC8, 0x60, 0x15, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ16},//GD25LQ16

    { 0xC8, 0x40, 0x17, 0x09, 0x00, _NULL, _NULL, 0x32, 0xEB, 0x75, 0x7A, FALSE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25Q64B},//GD25Q64B
    { 0xC8, 0x40, 0x16, 0x09, 0x00, _NULL, _NULL, 0x32, 0xEB, 0x75, 0x7A, FALSE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25Q32B},//GD25Q32B

  //  { 0x2C, 0xCB, 0x18, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_N25W128},//N25W128
  //  { 0x2C, 0xCB, 0x17, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_N25W064},//N25W064
   // { 0x2C, 0xCB, 0x16, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_N25W032},//N25W032
    { 0x2C, 0xCB, 0x18, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION_B, &cfg_N25W128},//N25W128
    { 0x2C, 0xCB, 0x17, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION_B, &cfg_N25W064},//N25W064
    { 0x2C, 0xCB, 0x16, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION_B, &cfg_N25W032},//N25W032

    { 0xC2, 0x25, 0x38, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_MX25U12835E},//MX25U12835E
    { 0xC2, 0x25, 0x37, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_MX25U6435E},//MX25U6435E
    { 0xC2, 0x25, 0x36, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_MX25U3235E},//MX25U3235E

    { 0xE0, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_BG25Q128AL},//BG25Q128AL

    { 0xEF, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q128JW},//W25Q128JW
    { 0xEF, 0x80, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q128JW},//W25Q128JW
//    { 0xEF, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q128FW},//W25Q128FW
    { 0xEF, 0x60, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q64FW},//W25Q64FW
    { 0xEF, 0x40, 0x16, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q32FV},//W25Q32FV
    { 0xEF, 0x60, 0x16, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION_B, &cfg_W25Q32JW},//W25Q32JW
    { 0xEF, 0x40, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q64FVSTIM},//W25Q64FVSTIM
    { 0xEF, 0x40, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q128FV},//W25Q128FV
    { 0xEF, 0x40, 0x19, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q256FV},//W25Q256FV

    { 0xF8, 0x32, 0x16, 0x09, 0x00, _NULL, _NULL, 0x38, 0xEB, 0x75, 0x7A, FALSE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25Q32A},//FM25Q32, SC2112C-V
    { 0xF8, 0x42, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M128A},//FM25M128A
    { 0xF8, 0x43, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M64C},//FM25M64C
    { 0xF8, 0x42, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M64A},//FM25M64A
    { 0xF8, 0x42, 0x16, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M32A},//FM25M32

    { 0x8C, 0x25, 0x37, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_F25D64QA},//F25D64QA

    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, FALSE, FALSE, NULL, NULL}
};

#pragma arm section rodata = "DFILE_DOWNLOAD_CFG"
const DFILE_CONFIG_T  s_download_file_config =
{
   DFILE_MAGIC_FIRST,
   DFILE_MAGIC_SECOND,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_MAGIC_END
};
#pragma arm section rodata


#pragma arm section rodata = "DFILE_DOWNLOAD_CFG_B"
const DFILE_CONFIG_T  s_download_file_config_B =
{
   DFILE_MAGIC_FIRST,
   DFILE_MAGIC_SECOND,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_MAGIC_END
};

#pragma arm section rodata


#pragma arm section rodata = "SPIFLASH_TYPE_CFG"
const SPIFLASH_TYPE_CONFIG_T  s_spiflash_type_config =
{
   SPIFLASH_MAGIC_FIRST,
   SPIFLASH_MAGIC_SECOND,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_MAGIC_END
};

#pragma arm section rodata


#pragma arm section

#pragma arm section code = "SPIFLASH_CFG_CODE"
SPIFLASH_SPEC_T *SPIFLASH_SPEC_Get(uint16 nMID, uint16 nDID1, uint16 nDID2)
{
   uint32 i;

   for(i=0; SpiFlashSpec[i].nMID!=0x0; i++)
   {
       if((nDID1 == SpiFlashSpec[i].nDID1) && (nMID == SpiFlashSpec[i].nMID) && (nDID2 == SpiFlashSpec[i].nDID2))
       {
           //Find Nand Type
           return (SPIFLASH_SPEC_T *)&SpiFlashSpec[i];
       }
   }
   //Not find
   return NULL;
}
#pragma arm section code


/*****************************************************************************/
//  Description :
//      Get the DFILE relative information of ps/res bin files.
//  Global resource dependence :
//  Input  : None
//  Output : None
//  Return : the pointer for the relative information of ps/bin files
//  Author :
//  Note   : Only for the combination of the bin files
//
/*****************************************************************************/
PUBLIC DFILE_CONFIG_T*  SPIFLASH_GetDFileConfig(void)
{
   if( (DFILE_MAGIC_FIRST == s_download_file_config.magic_first) &&
 	   (DFILE_MAGIC_SECOND == s_download_file_config.magic_second) &&
 	   (DFILE_MAGIC_END == s_download_file_config.magic_end))
   	{
        return (DFILE_CONFIG_T* )&s_download_file_config;
    }
    else
    {
    	return NULL;
    }
}



/*****************************************************************************/
//  Description :
//      Get the DFILE relative information of cp/nv/UMEM/operator bin files.
//  Global resource dependence :
//  Input  : None
//  Output : None
//  Return : the pointer for the relative information of cp/nv/UMEM/operator  files
//  Author :
//  Note   : Only for the combination of the bin files
//
/*****************************************************************************/
PUBLIC DFILE_CONFIG_T*  SPIFLASH_GetDFileConfigForFlashB(void)
{
   if( (DFILE_MAGIC_FIRST == s_download_file_config_B.magic_first) &&
 	   (DFILE_MAGIC_SECOND == s_download_file_config_B.magic_second) &&
 	   (DFILE_MAGIC_END == s_download_file_config_B.magic_end))
   	{
        return (DFILE_CONFIG_T* )&s_download_file_config_B;
    }
    else
    {
    	return NULL;
    }
}


PUBLIC SPIFLASH_TYPE_CONFIG_T*  SPIFLASH_GetFlashType(void)
{
//#ifdef SECURE_BOOT_SUPPORT
#if 0
	SPIFLASH_TYPE_CONFIG_T* ptr = NULL;
	ptr = ((SPIFLASH_TYPE_CONFIG_T*)FLASH_CONFIG_OFF);//In Bootloader, Flash Type Configuration is stored at FLASH_CONFIG_OFF offset

	if( (SPIFLASH_MAGIC_FIRST == ptr->magic_first) &&
		(SPIFLASH_MAGIC_SECOND == ptr->magic_second) &&
	 	(SPIFLASH_MAGIC_END == ptr->magic_end))
	{
		return ptr;
	}
	else
	{
		return NULL;
	}
#else
   if( (SPIFLASH_MAGIC_FIRST == s_spiflash_type_config.magic_first) &&
 	   (SPIFLASH_MAGIC_SECOND == s_spiflash_type_config.magic_second) &&
 	   (SPIFLASH_MAGIC_END == s_spiflash_type_config.magic_end))
   	{
        return (SPIFLASH_TYPE_CONFIG_T* )&s_spiflash_type_config;
    }
    else
    {
    	return NULL;
    }
#endif
}



uint32 SPIFLASH_GetOperatorDataAddress(void)
{
   return s_platform_patitiion_config.operator_data_addr;
}


uint32 SPIFLASH_GetOperatorDataSize(void)
{
   return s_platform_patitiion_config.operator_data_size;
}

uint32 SPIFLASH_GetCrystalAgingAddress(void)
{
   return s_platform_patitiion_config.crystal_aging_addr;
}


uint32 SPIFLASH_GetCrystalAgingSize(void)
{
   return s_platform_patitiion_config.crystal_aging_size;
}

uint32 SPIFLASH_GetImsToggelCfgAddress(void)
{
   return s_platform_patitiion_config.ims_toggel_cfg_addr;
}


uint32 SPIFLASH_GetImsToggelCfgSize(void)
{
   return s_platform_patitiion_config.ims_toggel_cfg_size;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// end flash_cfg.c

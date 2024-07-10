/******************************************************************************
 ** File Name:      mem_cfg.h                                                *
 ** Author:         guangqiao.she                                                 *
 ** DATE:           2022.06                                                *
 ** Copyright:      2022  UNISOC Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the memory config                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/29/2022    guangqiao.she         Create.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif
//*********** WARNING: PLEASE CONSULT SPREADTRUM SUPPORT ENGINEER BEFORE MODIFYING THIS FILE !***********

/************************************************************************
*************************************************************************
*************************************************************************
************************this is for 6121E 16MB RAM layout*********************
*************************************************************************
---------------------------------------------------------
region                      start address       size
---------------------------------------------------------
|memory index list            0x80000000       0x1000
---------------------------------------------------------
|CP assert reg                0x80001000       0x800
---------------------------------------------------------
|CP NV                        0x80001800       0x12800
---------------------------------------------------------
|LTE RF Calib                 0x80014000       0x10000
---------------------------------------------------------
|RISCV bin                    0x80024000       0xB000
---------------------------------------------------------
|ap cp SHMEM for IPC          0x8002F000       0x111000
---------------------------------------------------------
|ap cp SHMEM for audio        0x80140000       0x1000
---------------------------------------------------------
|CP                           0x80141000       0x93F000
---------------------------------------------------------
|AP                           0x80A80000       0x580000
---------------------------------------------------------


*************************************************************************
************************this is for 6121E 64KB AP ROM layout******************
*************************************************************************
---------------------------------------------------------
region                  start address   size
---------------------------------------------------------
---------------------------------------------------------
|AP ROM                  0x00000000     0x10000
---------------------------------------------------------

*************************************************************************
************************this is for 6121E 16KB AP IRAM layout******************
*************************************************************************
---------------------------------------------------------
region                  start address   size
---------------------------------------------------------
---------------------------------------------------------
|AP IRAM                0x00100000      0x4000
---------------------------------------------------------

*************************************************************************
************************this is for 6121E 16KB CP IRAM layout******************
*************************************************************************
---------------------------------------------------------
region                  start address   size
---------------------------------------------------------
---------------------------------------------------------
|CP IRAM                0x10100000       0x34000
---------------------------------------------------------

*************************************************************************
***********************this is for 6121E 80KB AON IRAM layout******************
*************************************************************************
---------------------------------------------------------
region                  start address       size
---------------------------------------------------------
---------------------------------------------------------
|AON IRAM for AP            0x50800000      0x2000
---------------------------------------------------------
| IPC Control info          0x50802000      0xF00
---------------------------------------------------------
|CP log control info        0x50802F00      0x100
---------------------------------------------------------
|AON IRAM for CP            0x50803000      0x11000
---------------------------------------------------------

******************************************************************************
******************************************************************************
******************************************************************************
*/


// PSRAM
#define PHY_RAM_BASE_ADDR   0x80000000
#define MEM_RAM_SIZE                   0x1000000
#define MEM_RAM_END_ADDR        (PHY_RAM_BASE_ADDR+MEM_RAM_SIZE)

#define  MMU_TLB_L1_SIZE    (16 * 1024)    //L1 section size is 1MB, 4Bytes describe 1MB£¬16KB describe the 4GB space
#define  MMU_TLB_L2_SIZE    (48 * 1024)    //L2 page size is 4KB, 4Bytes describe 4KB£¬48KB describe the 48MB space
#define  MMU_TLB_SIZE        (MMU_TLB_L1_SIZE+MMU_TLB_L2_SIZE)

#define RAM_CFG_MEM_CFG_LIST_OFFSET     (0x00000000)
#define RAM_CFG_CP_ASS_REG_OFFSET               (0x00001000)
#define RAM_CFG_CP_NV_OFFSET                         (0x00001800)
#define RAM_CFG_RF_CALIB_OFFSET             (0x00014000)
#define RAM_CFG_RSICV_BIN_OFFSET                (0x00024000)
#define RAM_CFG_IPC_SHMEM_OFFSET                (0x0002F000)
#define RAM_CFG_AUDIO_SHMEM_OFFSET              (0x00140000)
#define RAM_CFG_CP_OFFSET                                (0x00141000)
#define RAM_CFG_AP_OFFSET                                (0x00A00000)
#define RAM_CFG_MMU_OFFSET                       (MEM_RAM_SIZE - MMU_TLB_SIZE)
#define RAM_CFG_END_OFFSET                               (MEM_RAM_SIZE)

#define MEM_RAM_MEM_CFG_LIST_SIZE           (RAM_CFG_CP_ASS_REG_OFFSET -RAM_CFG_MEM_CFG_LIST_OFFSET)
#define MEM_RAM_CP_ASS_REG_SIZE             (RAM_CFG_CP_NV_OFFSET -RAM_CFG_CP_ASS_REG_OFFSET)
#define MEM_RAM_CP_NV_SIZE                              (RAM_CFG_RF_CALIB_OFFSET - RAM_CFG_CP_NV_OFFSET)
#define MEM_RAM_RF_CALIB_SIZE                   (RAM_CFG_RSICV_BIN_OFFSET - RAM_CFG_RF_CALIB_OFFSET)
#define MEM_RAM_RSICV_BIN_SIZE              (RAM_CFG_IPC_SHMEM_OFFSET - RAM_CFG_RSICV_BIN_OFFSET)
#define MEM_RAM_IPC_SHMEM_SIZE              (RAM_CFG_AUDIO_SHMEM_OFFSET -RAM_CFG_IPC_SHMEM_OFFSET)
#define MEM_RAM_AUDIO_SHMEM_SIZE                (RAM_CFG_CP_OFFSET -RAM_CFG_AUDIO_SHMEM_OFFSET)
#define MEM_RAM_CP_SIZE                              (RAM_CFG_AP_OFFSET - RAM_CFG_CP_OFFSET)
#define MEM_RAM_AP_SIZE                              (RAM_CFG_MMU_OFFSET - RAM_CFG_AP_OFFSET)
#define MEM_RAM_MMU_SIZE                                 (RAM_CFG_END_OFFSET - RAM_CFG_MMU_OFFSET)

#define MEM_RAM_MEM_CFG_LIST_BASE_ADDR      (PHY_RAM_BASE_ADDR+RAM_CFG_MEM_CFG_LIST_OFFSET)
#define MEM_RAM_CP_ASS_BASE_ADDR                (PHY_RAM_BASE_ADDR+RAM_CFG_CP_ASS_REG_OFFSET)
#define MEM_RAM_CP_NV_BASE_ADDR                      (PHY_RAM_BASE_ADDR+RAM_CFG_CP_NV_OFFSET)
#define MEM_RAM_RF_CALIB_BASE_ADDR              (PHY_RAM_BASE_ADDR+RAM_CFG_RF_CALIB_OFFSET)
#define MEM_RAM_RSICV_BIN_BASE_ADDR             (PHY_RAM_BASE_ADDR+RAM_CFG_RSICV_BIN_OFFSET)
#define MEM_RAM_IPC_SHMEM_BASE_ADDR             (PHY_RAM_BASE_ADDR+RAM_CFG_IPC_SHMEM_OFFSET)
#define MEM_RAM_AUDIO_SHMEM_BASE_ADDR                (PHY_RAM_BASE_ADDR+RAM_CFG_AUDIO_SHMEM_OFFSET)
#define MEM_RAM_CP_BASE_ADDR                                 (PHY_RAM_BASE_ADDR+RAM_CFG_CP_OFFSET)
#define MEM_RAM_AP_BASE_ADDR                  (PHY_RAM_BASE_ADDR+RAM_CFG_AP_OFFSET)
#define MEM_RAM_MMU_BASE_ADDR                          (PHY_RAM_BASE_ADDR+RAM_CFG_MMU_OFFSET)

#define MEM_RAM_AP_END_ADDR                  (MEM_RAM_AP_BASE_ADDR+MEM_RAM_AP_SIZE)


// AP IROM
#define PHY_IROM_BASE_ADDR       0x00000000
#define MEM_IROM_SIZE                   0x10000
#define MEM_IROM_END_ADDR        (PHY_IROM_BASE_ADDR+MEM_IROM_SIZE)

// AP SYSTEM RAM
#define PHY_IRAM_BASE_ADDR       0x00100000
#define MEM_IRAM_SIZE                   0x4000
#define MEM_IRAM_END_ADDR        (PHY_IRAM_BASE_ADDR+MEM_IRAM_SIZE)

// CP_ROM
#define MEM_CP_ROM_BASE_ADDR                 (0x10000000)
#define MEM_CP_ROM_SIZE                      (0x80000)
#define MEM_CP_ROM_END_ADDR                 (MEM_CP_ROM_BASE_ADDR+MEM_CP_ROM_SIZE)

// CP SYSTEM RAM
#define PHY_CP_IRAM_BASE_ADDR       (0x10100000)
#define MEM_CP_IRAM_SIZE                   (0x34000)
#define MEM_CP_IRAM_END_ADDR        (PHY_IRAM_BASE_ADDR+MEM_IRAM_SIZE)


//AON IRAM
#define PHY_AON_IRAM_BASE_ADDR       0x50800000
#define MEM_AON_IRAM_SIZE                   0x14000
#define MEM_AON_IRAM_END_ADDR        (PHY_AON_IRAM_BASE_ADDR+MEM_AON_IRAM_SIZE)

#define AON_IRAM_CFG_AP_OFFSET      (0x00000000)
#define AON_IRAM_CFG_IPC_OFFSET     (0x00002000)  // ipc control information
#define AON_IRAM_CFG_CP_LOG_OFFSET      (0x00002F00) // cp log buffer control header infor
#define AON_IRAM_CFG_CP_OFFSET      (0x00003000)
#define AON_IRAM_CFG_END_OFFSET     (MEM_AON_IRAM_SIZE)

#define AON_IRAM_CFG_AP_SIZE    (AON_IRAM_CFG_IPC_OFFSET-AON_IRAM_CFG_AP_OFFSET)
#define AON_IRAM_CFG_IPC_SIZE       (AON_IRAM_CFG_CP_LOG_OFFSET-AON_IRAM_CFG_IPC_OFFSET)
#define AON_IRAM_CFG_CP_LOG_SIZE    (AON_IRAM_CFG_CP_OFFSET-AON_IRAM_CFG_CP_LOG_OFFSET)
#define AON_IRAM_CFG_CP_SIZE    (AON_IRAM_CFG_END_OFFSET-AON_IRAM_CFG_CP_LOG_OFFSET)


#define AON_IRAM_CFG_AP_BASE_ADDR       (PHY_AON_IRAM_BASE_ADDR+AON_IRAM_CFG_AP_OFFSET)
#define AON_IRAM_CFG_IPC_BASE_ADDR      (PHY_AON_IRAM_BASE_ADDR+AON_IRAM_CFG_IPC_OFFSET)
#define AON_IRAM_CFG_CP_LOG_BASE_ADDR       (PHY_AON_IRAM_BASE_ADDR+AON_IRAM_CFG_CP_LOG_OFFSET)
#define AON_IRAM_CFG_CP_BASE_ADDR       (PHY_AON_IRAM_BASE_ADDR+AON_IRAM_CFG_CP_OFFSET)
#define AON_IRAM_CFG_END_BASE_ADDR      (PHY_AON_IRAM_BASE_ADDR+AON_IRAM_CFG_END_OFFSET)




/************************************************************************
******************************Flash LayeOut*******************************
*************************************************************************/
#define  PHY_FLASH_START_ADDR           0x60000000
#define  PHY_FLASH_SIZE                 0x01000000
#define  FLASH_SECTOR_SIZE              0x00010000   //Block size
#define  PHY_FLASH_END_ADDR             (PHY_FLASH_START_ADDR+PHY_FLASH_SIZE)

#define  PHY_FLASH_START_ADDR_B         0x70000000
#ifdef FLASH_SIZE_128MBITX32MBIT
#define  PHY_FLASH_SIZE_B               0x00400000
#endif
#define  FLASH_SECTOR_SIZE_B            0x00010000   //Block size
#define  PHY_FLASH_END_ADDR_B           (PHY_FLASH_START_ADDR_B+PHY_FLASH_SIZE_B)

//#if defined(BZP_SUPPORT)
#define VIR_IMAGE_START_ADDR            (0xD0000000)
#define VIR_IMAGE_SIZE                  (PHY_FLASH_SIZE)
//#endif




#ifdef FLASH_SIZE_128MBIT

/***********Flash LayeOut******************

***************************************************************
************ ONLY  FLASH A PARTITION (16MB) Start*****************
***************************************************************
---------------------------------------------------------
region                   start address      size(Byte)
---------------------------------------------------------
---------------------------------------------------------
|bootloader                   0x60000000   0x00010000
---------------------------------------------------------
|AP                           0x60010000   0x006D0000 //110
---------------------------------------------------------
|FOTA bootload                0x606E0000   0x00030000 //3
---------------------------------------------------------
|FOTA                         0x60710000   0x000A0000 //10
---------------------------------------------------------
|mmi resource                 0x607B0000   0x003C0000 //60
---------------------------------------------------------
|CP                           0x60B70000   0x00200000 //32
---------------------------------------------------------
|IMS Toggle                   0x60D70000   0x00010000
---------------------------------------------------------
|Crystal aging                0x60D80000   0x00020000
---------------------------------------------------------
|ConfigNV                     0x60DA0000   0x00010000
---------------------------------------------------------
|Operator Data(IMS)           0x60DB0000   0x00010000
---------------------------------------------------------
|fix NV                       0x60DC0000   0x0002FC00
---------------------------------------------------------
|production info              0x60DEFC00   0x00000400
---------------------------------------------------------
|fixd NV backup               0x60DF0000   0x00030000 //3
---------------------------------------------------------
|running nv                   0x60E20000   0x00090000 //9
---------------------------------------------------------
|UMEM                         0x60EB0000   0x00150000 //21
---------------------------------------------------------
|Flash A End                  0x61000000
---------------------------------------------------------
***************************************************************
*************** ONLY  FLASH A PARTITION (16MB) End***************
***************************************************************/

#define  UMEM_SECTOR_NUM            21
//when PRELOAD_SUPPORT is true and preload img to UMEM PARTITION. 
//keep the "IMG_SIZE"(in \make\preload_app\preload_app.mk) matching with the UDISK_SIZE 
#ifdef  UMEM_SUPPORT
#define UDISK_SIZE                    (UMEM_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define UDISK_BASE_ADDR       (PHY_FLASH_END_ADDR - UDISK_SIZE)
#else
#define UDISK_SIZE                     (0)
#define UDISK_BASE_ADDR        (-1)
#endif

//include  fix nv bak sector
#define RUNNIN_NV_SECTOR_NUM       12
//include umem sector
#define RUNNIN_NV_SECTOR_COUNT      (UMEM_SECTOR_NUM + RUNNIN_NV_SECTOR_NUM)
#define RUNNIN_NV_SIZE              (RUNNIN_NV_SECTOR_COUNT*FLASH_SECTOR_SIZE)
#define RUNNIN_NV_BASE_ADDR         (PHY_FLASH_END_ADDR - RUNNIN_NV_SIZE)

#define PRODUCT_NV_SZE              0x400
#define FIXNV_SECTOR_NUM            3
#define FIXED_NV_SIZE               0x2FC00    //191KB
#define FIXED_NV_BASE_ADDR          (RUNNIN_NV_BASE_ADDR - FIXNV_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define PRODUCT_NV_BASE_ADDR        (FIXED_NV_BASE_ADDR + FIXED_NV_SIZE)

#define OPERATOR_DATA_SECTOR_NUM    1
#define OPERATOR_DATA_SIZE          (OPERATOR_DATA_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define OPERATOR_DATA_BASE_ADDR     (FIXED_NV_BASE_ADDR-OPERATOR_DATA_SIZE)

#define CONFIG_NV_SECTOR_NUM        1
#define CONFIG_NV_SIZE              (CONFIG_NV_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define CONFIG_NV_BASE_ADDR         (OPERATOR_DATA_BASE_ADDR-CONFIG_NV_SIZE)

#define SIM_LOCK_SECTOR_NUM         (0)
#define SIM_LOCK_SIZE               (SIM_LOCK_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define SIM_LOCK_BASE_ADDR          (CONFIG_NV_BASE_ADDR - SIM_LOCK_SIZE)

#define CRYSTAL_AGING_SECTOR_NUM     2
#define CRYSTAL_AGING_SIZE          (CRYSTAL_AGING_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define CRYSTAL_AGING_ADDR          (SIM_LOCK_BASE_ADDR-CRYSTAL_AGING_SIZE)

#define IMS_TOGGEL_CFG_SECTOR_NUM    1
#define IMS_TOGGEL_CFG_SIZE         (IMS_TOGGEL_CFG_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define IMS_TOGGEL_CFG_ADDR         (CRYSTAL_AGING_ADDR-IMS_TOGGEL_CFG_SIZE)

#if defined(GPS_SUPPORT)
#define CP_SECTOR_NUM                38
#else
#define CP_SECTOR_NUM                32//38//32
#endif
#define CP_SIZE                      (CP_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define CP_BASE_ADDR                 (IMS_TOGGEL_CFG_ADDR-CP_SIZE)

#ifdef _SW_ZDT_PRODUCT_
#define MMI_RES_SECTOR_NUM          60	//68//58
#else
#define MMI_RES_SECTOR_NUM          43
#endif
#define MMI_RESOURCE_SIZE           (MMI_RES_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define MMI_RES_BASE_ADDR           (CP_BASE_ADDR - MMI_RESOURCE_SIZE)

#if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
#define  FOTA_SECTOR_NUM            10
#define  FOTA_BOOTLOADER_SECTOR_NUM     3
#else
#define  FOTA_SECTOR_NUM            0
#define  FOTA_BOOTLOADER_SECTOR_NUM     0
#endif
#define FOTA_SIZE                   (FOTA_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define FOTA_BASE_ADDR          (MMI_RES_BASE_ADDR - FOTA_SIZE)

#ifdef FOTA_SUPPORT_CMIOT
#define FOTA_SIZE                       (FOTA_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define FOTA_BASE_ADDR                  (MMI_RES_BASE_ADDR - FOTA_SIZE)
#define FOTA_BOOTLOADER_SIZE            (FOTA_BOOTLOADER_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define FOTA_BOOTLOADER_ADDR            (FOTA_BASE_ADDR-FOTA_BOOTLOADER_SIZE)
#endif

//#define  AP_SECTOR_NUM        100
#define  AP_BASE_ADDR       (0x60010000)  //(BOOTLOADER_BASE_ADDR+BOOTLOADER_SIZE)
#define  AP_SIZE                (FOTA_BASE_ADDR - AP_BASE_ADDR)

#define  BOOTLOADER_SECTOR_NUM  1
#define  BOOTLOADER_SIZE            (BOOTLOADER_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define  BOOTLOADER_BASE_ADDR   PHY_FLASH_START_ADDR


#endif

#define IMAGE_ENTRY_ADDR  (AP_BASE_ADDR)
#define IMAGE_SIZE                  (AP_SIZE)

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


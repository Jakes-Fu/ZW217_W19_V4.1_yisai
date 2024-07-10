/******************************************************************************
 ** File Name:     flash_cfg.c                                                *
 ** Description:                                      *
 ** This file contains flash/sram config info ,for emc, for platform application, for access code     *
 ** Author:         Younger.yang                                              *
 ** DATE:           11/23/2006                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/23/2006      Younger.yang       Create                         *
 ** 12/16/2008    hyy       Modify                          *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "flash.h"
#include "emc_drv.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines
 **---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Data Structures                                  *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Constant DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         CUSTOMERIZE DEFINITION                           *
 **--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------  *
 **                          cfg struct 1 :config flash/sram to emc             *
 **---------------------------------------------------------------------------  */

//*********** WARNING: CUSTOMER SHOULD NOT MODIFY THE CONTENTS OF THIS FILE !***********

//LOCAL const NORMCP_CONFIG_T g_normcp_S29WS128P_cfg[] =
//{
//    { EMC_CS0, EMC_MEM_FLASH,   EMC_READ_BURST_MODE, EMC_LEN_CONTINUOUS, EMC_WRITE_NORMAL_MODE,   EMC_LEN_SINGLE, PNULL},/*lint !e651*/
//    { EMC_CS1, EMC_MEM_SRAM,  EMC_READ_BURST_MODE, EMC_LEN_CONTINUOUS, EMC_WRITE_BURST_MODE,  EMC_LEN_CONTINUOUS, PNULL},
//    { EMC_CS_MAX,0,0,0,0,0,0}//end line
//};

LOCAL const NORMCP_CONFIG_T g_normcp_burst_common_emc_cfg[] =
{
    { EMC_CS0, EMC_MEM_FLASH,   EMC_READ_BURST_MODE, EMC_LEN_CONTINUOUS, EMC_WRITE_NORMAL_MODE,   EMC_LEN_SINGLE, PNULL},/*lint !e651*/
    { EMC_CS1, EMC_MEM_SRAM,  EMC_READ_BURST_MODE, EMC_LEN_CONTINUOUS, EMC_WRITE_BURST_MODE,  EMC_LEN_CONTINUOUS, PNULL},
    { EMC_CS_MAX,0,0,0,0,0,0}//end line
};

LOCAL const NORMCP_CONFIG_T g_normcp_256M_burst_common_emc_cfg[] =
{
    { EMC_CS0, EMC_MEM_FLASH,   EMC_READ_BURST_MODE, EMC_LEN_CONTINUOUS, EMC_WRITE_NORMAL_MODE,   EMC_LEN_SINGLE, PNULL},/*lint !e651*/
    { EMC_CS1, EMC_MEM_SRAM,  EMC_READ_BURST_MODE, EMC_LEN_CONTINUOUS, EMC_WRITE_BURST_MODE,  EMC_LEN_CONTINUOUS, PNULL},
    { EMC_CS_MAX,0,0,0,0,0,0}//end line
};

LOCAL const NORMCP_CONFIG_T g_normcp_K5L2731CAM_emc_cfg[] =
{
    { EMC_CS0, EMC_MEM_FLASH,   EMC_READ_PAGE_MODE, EMC_LEN_8, EMC_WRITE_NORMAL_MODE,   EMC_LEN_SINGLE, PNULL},/*lint !e651*/
    { EMC_CS1, EMC_MEM_SRAM,  EMC_READ_PAGE_MODE, EMC_LEN_4, EMC_WRITE_NORMAL_MODE,   EMC_LEN_SINGLE, PNULL},
    { EMC_CS_MAX,0,0,0,0,0,0}//end line
};

LOCAL const NORMCP_CONFIG_T g_normcp_TSB_TV00570002_emc_cfg[] =
{
    { EMC_CS0, EMC_MEM_FLASH,   EMC_READ_PAGE_MODE, EMC_LEN_8, EMC_WRITE_NORMAL_MODE,   EMC_LEN_SINGLE, PNULL},/*lint !e651*/
    { EMC_CS1, EMC_MEM_SRAM,  EMC_READ_PAGE_MODE, EMC_LEN_8, EMC_WRITE_NORMAL_MODE,   EMC_LEN_SINGLE, PNULL},
    { EMC_CS_MAX,0,0,0,0,0,0}//end line
};

LOCAL const NORMCP_CONFIG_T g_normcp_k5l2833ata_af66_emc_cfg[] =
{
    { EMC_CS0, EMC_MEM_FLASH,   EMC_READ_BURST_MODE,  EMC_LEN_16, EMC_WRITE_NORMAL_MODE,  EMC_LEN_SINGLE, PNULL},/*lint !e651*/
    { EMC_CS1, EMC_MEM_SRAM,  EMC_READ_BURST_MODE,  EMC_LEN_32, EMC_WRITE_BURST_MODE, EMC_LEN_32,   PNULL},
    { EMC_CS_MAX,0,0,0,0,0,0}//end line
};
LOCAL const NORMCP_CONFIG_T g_normcp_page_len8_common_cfg[] =
{
    { EMC_CS0, EMC_MEM_FLASH, EMC_READ_PAGE_MODE,   EMC_LEN_8, EMC_WRITE_NORMAL_MODE, EMC_LEN_SINGLE,  PNULL},/*lint !e651*/
    { EMC_CS1, EMC_MEM_SRAM , EMC_READ_PAGE_MODE, EMC_LEN_8, EMC_WRITE_NORMAL_MODE, EMC_LEN_SINGLE,  PNULL},
    { EMC_CS_MAX,0,0,0,0,0,0}//end line
};


LOCAL const NORMCP_CONFIG_T g_normcp_CFEON_EN71GL128B0_emc_cfg[] =
{
    { EMC_CS0, EMC_MEM_FLASH,     EMC_READ_PAGE_MODE, EMC_LEN_8, EMC_WRITE_NORMAL_MODE,   EMC_LEN_SINGLE, PNULL},/*lint !e651*/
    { EMC_CS1, EMC_MEM_SRAM,    EMC_READ_PAGE_MODE, EMC_LEN_4, EMC_WRITE_NORMAL_MODE,   EMC_LEN_SINGLE, PNULL},
    { EMC_CS_MAX,0,0,0,0,0,0}//end line
};

LOCAL const NORMCP_CONFIG_T g_normcp_m36l0t70x0t3_emc_cfg[] =
{
    { EMC_CS0, EMC_MEM_FLASH, EMC_READ_PAGE_MODE,   EMC_LEN_4, EMC_WRITE_NORMAL_MODE, EMC_LEN_SINGLE,  PNULL},/*lint !e651*/
    { EMC_CS1, EMC_MEM_SRAM , EMC_READ_PAGE_MODE, EMC_LEN_8, EMC_WRITE_NORMAL_MODE, EMC_LEN_SINGLE,  PNULL},
    { EMC_CS_MAX,0,0,0,0,0,0}//end line
};
LOCAL const NORMCP_CONFIG_T g_normcp_Memocom_2832B_emc_cfg[] =
{
    { EMC_CS0, EMC_MEM_FLASH,   EMC_READ_PAGE_MODE, EMC_LEN_8, EMC_WRITE_NORMAL_MODE,   EMC_LEN_SINGLE, PNULL},/*lint !e651*/
    { EMC_CS1, EMC_MEM_SRAM,  EMC_READ_PAGE_MODE, EMC_LEN_8, EMC_WRITE_NORMAL_MODE,   EMC_LEN_SINGLE, PNULL},
    { EMC_CS_MAX,0,0,0,0,0,0}//end line
};
/**---------------------------------------------------------------------------  *
 **                          cfg struct 2:config flash  space usage                 *
 **---------------------------------------------------------------------------  */
#pragma arm section rodata = "FLASH_CFG_TABLE_DETAIL"
LOCAL const NOR_FLASH_CONFIG_T s_flash_128Mbit_128Kbyte_cfg =
{
    1,          //4-bank
    14,          //(prodinfo_addr-efs_start_addr)/sect_size
    0,          //file system sector num
    0x00020000, //sector size
    0x00000000, //start address
    0x00E00000, //the bank managed by EFS
    0x01000000, //flash size
    0x00DE0000, //fixed nv address
    0x00FE0000, //product info, the last sector
#ifdef STONE_IMAGE_SUPPORT
    FLASH_INVALID_ADDR,
#else                     
    0x00900000, //MMI_res
#endif    
    0x00FC0000,  //uabin
    0x00020000,  //upbin
    FLASH_INVALID_ADDR   //blbin
};

LOCAL const NOR_FLASH_CONFIG_T s_flash_128Mbit_64Kbyte_cfg =
{
    1,          //4-bank
    10,         //(udisk base assress-efs_start_addr)/sect_size
    0,          //file system sector num
    0x00010000, //sector size
    0x00000000, //start address     
    0x00F30000, //the bank managed by EFS
    0x01000000, //flash size
    0x00F20000, //fixed nv address
    0x00FF0000, //product info, the last sector
#ifdef STONE_IMAGE_SUPPORT
    FLASH_INVALID_ADDR,
#else         
    0x00A00000,  //MMI_res
#endif    
    0x00FD0000,  //uabin
    0x00020000,  //upbin
    -1   //blbin

};

/************ SC6800H Phone Config***************/
/*|---- Product Info(128KB)-------|*/
/*|---- UDISK(7040KB)      -------|*/
/*|---- RUNNING NV(1092KB) -------|*/
/*|---- FIXED NV(128KB)    -------|*/
/*|---- Resource (7MB)     -------|*/
/*|---- Code  (16MB)       -------|*/
/************************************************/
LOCAL const NOR_FLASH_CONFIG_T s_flash_256Mbit_128Kbyte_cfg =
{
    1,          //4-bank
    15,         //(udisk base assress-efs_start_addr)/sect_size
    0,          //file system sector num
    0x00020000, //sector size
    0x00000000, //start address     
    0x01B20000, //the bank managed by EFS
    0x02000000, //flash size
    0x01B00000, //fixed nv address
    0x01FE0000, //product info, the last sector
#ifdef STONE_IMAGE_SUPPORT
    FLASH_INVALID_ADDR,
#else         
    0x01000000, //MMI_res
#endif    
    0x01D00000, ////udisk base address
    0x002C0000, //udisk size
    0x01FC0000  //blbin
};

LOCAL const NOR_FLASH_CONFIG_T s_flash_128Mbit_256Kbyte_M18_cfg =
{
    1,          //4-bank
    2,          //(prodinfo_addr-efs_start_addr)/sect_size/2 M18
    0,          //file system sector num
    0x00040000, //sector size
    0x00000000, //start address
    0x00EC0000, //the bank managed by EFS
    0x01000000, //flash size
    0x00E80000, //fixed nv address
    0x00FC0000, //product info, the last sector
#ifdef STONE_IMAGE_SUPPORT
    FLASH_INVALID_ADDR,
#else         
    0x00980000, //MMI_res
#endif    
    FLASH_INVALID_ADDR,  //uabin
    FLASH_INVALID_ADDR,  //upbin
    FLASH_INVALID_ADDR   //blbin
};

LOCAL const NOR_FLASH_CONFIG_T s_flash_128Mbit_256Kbyte_cfg =
{
    1,          //4-bank
    4,          //(prodinfo_addr-efs_start_addr)/sect_size
    0,          //file system sector num
    0x00040000, //sector size
    0x00000000, //start address
    0x00EC0000, //the bank managed by EFS
    0x01000000, //flash size
    0x00E80000, //fixed nv address
    0x00FC0000, //product info, the last sector
#ifdef STONE_IMAGE_SUPPORT
    FLASH_INVALID_ADDR,
#else         
    0x00980000, //MMI_res
#endif    
    FLASH_INVALID_ADDR,  //uabin
    FLASH_INVALID_ADDR,  //upbin
    FLASH_INVALID_ADDR   //blbin
};
LOCAL const NOR_FLASH_CONFIG_T s_flash_64Mbit_64Kbyte_cfg =
{
    1,          //4-bank                  //OK, default, don't care
    3,          //(prodinfo_addr-efs_start_addr)/sect_size
    0,          //file system sector num
    0x00010000, //sector size
    0x00000000, //start address
    0x007C0000, //the bank managed by EFS
    0x00800000, //flash size
    0x007B0000, //fixed nv address
    0x007F0000, //product info, the last sector
#ifdef STONE_IMAGE_SUPPORT
    FLASH_INVALID_ADDR,
#else         
    0x005B0000,  //MMI_res
#endif    
    FLASH_INVALID_ADDR,  //uabin
    FLASH_INVALID_ADDR,  //upbin
    FLASH_INVALID_ADDR   //blbin
};
#pragma arm section rodata

/**---------------------------------------------------------------------------  *
 **                          describe info and cfg struct info assembled in this entity         *
 **                          For code access                                *
 **---------------------------------------------------------------------------  */
#pragma arm section rodata = "FLASH_CFG_TABLE"
LOCAL const NORMCP_ENTITY_T g_normcp_entity_table[] =
{
   { (NORMCP_SPEC_PTR)&g_normcp_k5n2866ate_bq12_spec,    	(NORMCP_CONFIG_PTR)g_normcp_k5l2833ata_af66_emc_cfg, 	(NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_64Kbyte_cfg},                      
    { (NORMCP_SPEC_PTR)&g_normcp_k5n2833atm_aq11_spec,  	(NORMCP_CONFIG_PTR)g_normcp_k5l2833ata_af66_emc_cfg,		(NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_64Kbyte_cfg},
	{ (NORMCP_SPEC_PTR)&g_normcp_M36L0T70x0T3_spec,         (NORMCP_CONFIG_PTR)g_normcp_m36l0t70x0t3_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_CFEON_EN71GL128B0_spec,    (NORMCP_CONFIG_PTR)g_normcp_CFEON_EN71GL128B0_emc_cfg,  (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_PF38F3040L0YUQ3_spec,      (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_s71ws128pc0hf3sr_spec,     (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_S71PL127N_spec,            (NORMCP_CONFIG_PTR)g_normcp_page_len8_common_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_256Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_K5L2731CAM_spec,           (NORMCP_CONFIG_PTR)g_normcp_K5L2731CAM_emc_cfg,         (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_64Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_K5L6331CAA_spec,           (NORMCP_CONFIG_PTR)g_normcp_K5L2731CAM_emc_cfg,         (NOR_FLASH_CONFIG_PTR)&s_flash_64Mbit_64Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_TSB_TV00570002_spec,       (NORMCP_CONFIG_PTR)g_normcp_TSB_TV00570002_emc_cfg,     (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_TSB_TV00560002_spec,       (NORMCP_CONFIG_PTR)g_normcp_TSB_TV00570002_emc_cfg,     (NOR_FLASH_CONFIG_PTR)&s_flash_64Mbit_64Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_TSB_TV00560003_spec,       (NORMCP_CONFIG_PTR)g_normcp_TSB_TV00570002_emc_cfg,     (NOR_FLASH_CONFIG_PTR)&s_flash_64Mbit_64Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_k5l2833ata_af66_spec,      (NORMCP_CONFIG_PTR)g_normcp_k5l2833ata_af66_emc_cfg,    (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_64Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_m36l0r7050u3_spec,         (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_m36l0r7050l3_spec,         (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_m36l0r7050t4zaq_spec,      (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_m36l0r7050t4zaq_spec_1,    (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_m36l0r7050b4zaq_spec,      (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_m36l0r7050b4zaq_spec_1,    (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_k5n2833atm_aq11_spec,      (NORMCP_CONFIG_PTR)g_normcp_k5l2833ata_af66_emc_cfg,    (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_64Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_k5n2833abm_aq11_spec,      (NORMCP_CONFIG_PTR)g_normcp_k5l2833ata_af66_emc_cfg,    (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_64Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_PF38F3040L0YUQ3_B_spec,    (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg}, 
    { (NORMCP_SPEC_PTR)&g_normcp_PF38F4050L0YUQ3_spec,      (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_256Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_PF38F4050L0YUQ3_B_spec,    (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_256Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_PF38F3050M0Y3Q_spec,       (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_256Kbyte_M18_cfg},  
    { (NORMCP_SPEC_PTR)&g_normcp_PF38F3050M0Y0YE_spec,      (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_256Kbyte_M18_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_m36l0r8060u0_spec,         (NORMCP_CONFIG_PTR)g_normcp_256M_burst_common_emc_cfg,  (NOR_FLASH_CONFIG_PTR)&s_flash_256Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_K5L5666CAM_spec,           (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_256Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_k5n5629atb_spec,           (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_256Mbit_128Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_k5n2866ate_bq12_spec,      (NORMCP_CONFIG_PTR)g_normcp_k5l2833ata_af66_emc_cfg,    (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_64Kbyte_cfg},
    { (NORMCP_SPEC_PTR)&g_normcp_CFEON_EN71NS128C0_spec,    (NORMCP_CONFIG_PTR)g_normcp_burst_common_emc_cfg,       (NOR_FLASH_CONFIG_PTR)&s_flash_128Mbit_128Kbyte_cfg},
};

#pragma arm section rodata
/**---------------------------------------------------------------------------  *
 **          describe download file configuration in stone image                *
 **                              For image download                             *
 **---------------------------------------------------------------------------  */
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
   DFILE_MAGIC_END	
};

#pragma arm section rodata
/*****************************************************************************/
//  Description :
//  By manu_id,devc_id and extend_id,find correct g_normcp_entity_table item.
//  Global resource dependence :
//     None
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
#pragma arm section code = "FLASH_CFG_CODE"
PUBLIC NORMCP_ENTITY_PTR NORMCP_GetEntityTab(uint32 *entity_cnt)
{
  *entity_cnt = sizeof(g_normcp_entity_table)/sizeof(g_normcp_entity_table[0]);
  return (NORMCP_ENTITY_PTR)g_normcp_entity_table;
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


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// end flash_cfg.c

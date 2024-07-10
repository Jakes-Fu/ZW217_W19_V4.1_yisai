/******************************************************************************
 ** File Name:      emc_drv.c                                                 *
 ** Author:         younger.yang&Nick.zhao                                    *
 ** DATE:           11/26/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the operation about memory interface.   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                                                     *
 ** 11/26/2006     Younger.yang     Create.                                                         *
 ** 12/17/2008   Yiyue.He               Modify this file's orientation to emc-drv logic layer       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "os_api.h"
#include "sci_types.h"
#include "chip_plf_export.h"
#include "flash.h"
#include "emc_drvapi.h"
#include "normcp_spec.h"
#include "emc_reg_if.h"
#include "dma_drv_internal.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif


LOCAL NORMCP_ENTITY_PTR     s_normcp_entity_ptr = {0};

LOCAL EMC_REGINFO_T s_emc_reginfo[EMC_CS_MAX];/*lint -esym(551,s_emc_reginfo)*/  



/*****************************************************************************/
//  Description :
//      1.call FLASH_ReadID to get manufacture ID and device ID
//      2.call GetNorMCP_Entity to get normcp configuration from user table
//      3.call FLASH_InitHAL to init flash HAL
//  Global resource dependence :
//      s_normcp_entity_ptr
//      g_normcp_spec_ptr
//      g_Nor_Config_ptr
//      g_Nor_Driver_ptr
//
//  Author:
//      Younger.yang
//  Note:
//      code must be located at SRAM or internal-RAM.
//      must be called before EMC_Timing_cfg()
/*****************************************************************************/
#ifdef  FLASH_TYPE_SPI
extern PUBLIC void EMC_pSRAM_init(void);
#endif

PUBLIC void EMC_Init (void)
{
#ifndef FLASH_TYPE_SPI
    uint32 nMID = 0, nDevID = 0, nExtID = 0;
#endif

    //@set emc default value
#if !defined (PLATFORM_SC6800H) && !defined (PLATFORM_SC6530) && !defined(PLATFORM_SC6531EFM)
    EMC_Reg_SetToDef();
#endif

#ifndef FLASH_TYPE_SPI
    //@Find correct mem info and cfg info
    //Read ID from flash
    DISABLE_CACHE
    FLASH_ReadID (&nMID, &nDevID, &nExtID);
    ENABLE_CACHE
		

    //Get normcp total entity info from user table
    s_normcp_entity_ptr = NORMCP_GetEntity (nMID, nDevID, nExtID, NULL);

    SCI_ASSERT (s_normcp_entity_ptr);/*assert verified*/

    //@init flash by cfg info
    FLASH_InitHAL (s_normcp_entity_ptr);

    EMC_Reg_If_Init (s_emc_reginfo);

    //@save exprot info into local info
    SCI_MEMSET (s_emc_reginfo, 0, (sizeof (s_emc_reginfo)));

    {
        NORMCP_CONFIG_PTR emc_cs_cfg_ptr;
        emc_cs_cfg_ptr = s_normcp_entity_ptr->normcp_config_ptr;

        while (emc_cs_cfg_ptr->cs_num <  EMC_CS_MAX)
        {
            EMC_RegMemoryInfo (emc_cs_cfg_ptr->cs_num, & (emc_cs_cfg_ptr->cs_mode), s_normcp_entity_ptr->normcp_spec_ptr);
            ++emc_cs_cfg_ptr;
        }
    }

#else
	EMC_pSRAM_init();
#endif
}

PUBLIC void EMC_GetNorFlash_Partition( NOR_FLASH_CONFIG_PTR   * nor_flash_cfg_ptr)
{
     *nor_flash_cfg_ptr = s_normcp_entity_ptr->nor_flash_cfg_ptr;
}

PUBLIC void EMC_RegMemoryInfo (EMC_CS_NUM_E cs_num, EMC_MODE_CFG_PTR    emc_cs_cfg_ptr, EMC_TIMING_PTR emc_cs_mem_info_ptr)
{
    s_emc_reginfo[cs_num].emc_cs = cs_num;

    s_emc_reginfo[cs_num].emc_cs_cfg_ptr = emc_cs_cfg_ptr;

    s_emc_reginfo[cs_num].emc_cs_mem_info_ptr = emc_cs_mem_info_ptr;

    s_emc_reginfo[cs_num].reg_flag       = EMC_CS_REG_INIT_FLAG;
}



/*****************************************************************************/
//  Description :
//  Set All EMC CS Parameter To Default.
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
PUBLIC void EMC_Close (void)
{
    DISABLE_CACHE
    EMC_Reg_SetToDef();
}



/*****************************************************************************/
//  Description:    This function is used to set EMC reg value according different ahb clk.
//  Global resource dependence :
//  Author:          younger.yang
//  Note:
/*****************************************************************************/
PUBLIC void EMC_Timing_chng (uint32 ahb_clk)
{
    DMA_HAL_SetPause (SCI_TRUE);

    EMC_Reg_cfg (ahb_clk);

    DMA_HAL_SetPause (SCI_FALSE);
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

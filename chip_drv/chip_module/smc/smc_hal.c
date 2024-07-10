/******************************************************************************
 ** File Name:      smc_hal.c                                                 *
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
#include "smc_drvapi.h"
#include "smc_phy.h"
#include "flash.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif


PUBLIC void EMC_Init (void)
{
	SMC_Init();
}

PUBLIC void EMC_GetNorFlash_Partition( NOR_FLASH_CONFIG_PTR   * nor_flash_cfg_ptr)
{
}

/*****************************************************************************/
//  Description:    This function is used to set EMC reg value according different ahb clk.
//  Global resource dependence :
//  Author:          younger.yang
//  Note:
/*****************************************************************************/
PUBLIC void EMC_Timing_chng (uint32 ahb_clk)
{
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

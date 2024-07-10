/******************************************************************************
 ** File Name:      smc_drvapi.h                                                                                   *
 ** Author:         lin.liu                                                                                         *
 ** DATE:           17/12/2008                                                                                  *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.                                   *
 ** Description:    EMC drv public type and exprot interface declaration                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 27/11/2001     Lin.liu          Create.                                   *
 ** 17/12/2008    hyy       Modify
 ******************************************************************************/
#ifndef _SMC_DRVAPI_H_
#define _SMC_DRVAPI_H_

#include "flash.h"
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


/*Since MMU Config Flash Code region to read only, then before send flash
  command at code region should close MMU and Cache*/
#define DISABLE_CACHE   MMU_CacheOff(FALSE, TRUE); \
    MMU_AlignFaultDisable(); \
    MMU_UseNorDOM();

#define ENABLE_CACHE     MMU_CacheOn(FALSE, TRUE); \
    MMU_AlignFaultEnable(); \
    MMU_UseDefDOM();



/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
PUBLIC void EMC_Init (void);

PUBLIC void EMC_GetNorFlash_Partition(NOR_FLASH_CONFIG_PTR   * nor_flash_cfg_ptr);

PUBLIC void EMC_Timing_chng (uint32 ahb_clk);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _EMC_DRVAPI_H_

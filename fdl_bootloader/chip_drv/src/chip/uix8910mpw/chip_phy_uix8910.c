/******************************************************************************
 ** File Name:      chip_phy_sc6531efm.c                                          *
 ** Author:                                                                   *
 ** DATE:           01/27/2016                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic information on chip.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/27/2016                      Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "chip_plf_export.h"
#include "chip_drvapi.h"

#if defined(FPGA_SUPPORT)
#define   FPGA_USE_INTER_DDR      /*KU095 please open the macro*/
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/*****************************************************************************/
//  Description:    The function is used to get the chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         
//  Note:           It must be called before any hardware related settings
/*****************************************************************************/
PUBLIC CHIP_TYPE_E CHIP_GetChipType (void)
{
    return UIX8910;
}

/*****************************************************************************/
// Description :    This function is used get the chip type ex
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC CHIP_TYPE_EX_E CHIP_GetChipTypeEx (void)
{
    return CHIP_TYPE_EX_MAX;
}

/*****************************************************************************/
// Description :    This function is used to init chip.
// Global resource dependence :
// Author :         
// Note :           
/*****************************************************************************/
PUBLIC void Chip_Init(void)
{
    hwcfg_swcfg_init();
    clock_init();
 #if !defined(FPGA_USE_INTER_DDR)
    ddr_init();
 #endif
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

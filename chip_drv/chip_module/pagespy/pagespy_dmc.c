/******************************************************************************
 ** File Name:     pagespy_dmc_phy_uix8910.c                                              *
 ** Author:         jiajiang.lu                                                   *
 ** DATE:           3/5/2019                                                 *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Analog to Digital Module.                                 *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 3/5/2019     jiajiang.lu      Create.                                   *
 **                                                                      *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "pagespy_dmc_phy.h"

/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
PUBLIC void PAGESPY_HAL_ProtectAddrWMonitorStart(uint8 id, uint32 startAddr, uint32 endAddr)
{
    uint8  page_ch;

    page_ch = halb_PagePhyNumGet(id);
    hal_PageProtectAddrSetup(page_ch, HAL_PAGE_WRITE_HIT_TRIGGER, startAddr, endAddr);
}

PUBLIC void PAGESPY_HAL_ProtectAddrWMonitorStop(uint8 id)
{
    uint8  page_ch;

    page_ch = halb_PagePhyNumGet(id);
    hal_PageStop(page_ch);
}


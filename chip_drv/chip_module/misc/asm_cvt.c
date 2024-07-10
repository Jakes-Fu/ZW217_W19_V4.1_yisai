/**************************************************************************

  Copyright (C) 2001 SPREADTRUM COMMUNICATION CO.
  All rights reserved.

  This document contains proprietary information blonging to
  SPREADTRUM. Passing on and copying this document, use and
  communication of its contents is not permitted without prior
  writed authorization.

***************************************************************************/

/***************************************************************************

  Reversion information:
  File Name:    asm_cvt.c
  Version:      0.1
  Author:       Raislin.Kong
  Date:         2003.12
  Description:  this file only define const variable used in ARM asm as const
                variable that may change for different chip
***************************************************************************/
/***************************************************************************

                Edit Histroy
  Date          Name            Description
  --------------------------------------------------------------------------
  2003.12        Raislin.Kong    Create
***************************************************************************/
#include "os_api.h"
//#include "mem_prod.h"
#include "chip_plf_export.h"


const uint32    g_exception_mode_flag = 0x1F1F ;

// Define system timer priority
#if defined(PLATFORM_SC6800H)
PUBLIC const uint32    g_sys_timer_pri = 0x04;/*lint -e765 */
#else
PUBLIC const uint32    g_sys_timer_pri = 0x02;/*lint -e765 */
#endif

// Define watchdog related const
#if defined(PLATFORM_SC8800G)

const uint32    g_hw_reset_addr = ANA_HWRST_STATUS;

//analog die reg. timluo_g
//adi interface
const uint32    g_adi_arm_rd_cmd_add = ADI_ARM_RD_CMD;
const uint32    g_adi_rd_data_add = ADI_RD_DATA;

#elif defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H) 

const uint32    g_hw_reset_addr = GR_HWRST;
#elif  defined(PLATFORM_SC6530)
const uint32    g_hw_reset_addr = ANA_HW_RST0;

//analog die reg. timluo_g
//adi interface
const uint32    g_adi_arm_rd_cmd_add = ADI_ARM_RD_CMD;
const uint32    g_adi_rd_data_add = ADI_ARM_RD_DATA;

#elif defined(PLATFORM_SC6531EFM)
//const uint32    g_hw_reset_addr = ANA_POR_RST_MONITOR;
const uint32    g_hw_reset_addr = APB_MISC_CTL2;

//analog die reg.
//adi interface
const uint32    g_adi_arm_rd_cmd_add = ADI_ARM_RD_CMD;
const uint32    g_adi_rd_data_add = ADI_ARM_RD_DATA;

#elif defined(PLATFORM_UWS6121E)
//const uint32    g_hw_reset_addr = ANA_POR_RST_MONITOR;
const uint32    g_hw_reset_addr = 0x81000000;  //´ò×® APB_MISC_CTL2;

//analog die reg.
//adi interface
//const uint32    g_adi_arm_rd_cmd_add = ADI_ARM_RD_CMD;
const uint32    g_adi_arm_rd_cmd_add = REG_ADI_MST_BASE + 0x28;
const uint32    g_adi_rd_data_add = REG_ADI_MST_BASE + 0x2C;
#endif


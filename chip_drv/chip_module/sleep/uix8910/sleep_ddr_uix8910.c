/******************************************************************************
 ** File Name:      sleep_phy_nix8910.c                                       *
 ** Author:         bo.tang                                                   *
 ** DATE:           07/23/2018                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for time compensate of the bug for DSP.       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/23/2018     bo.tang     		Create.                                   *

******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *

**---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "../sleep_phy.h"
#include "sleep_phy_sc6531efm.h"
#include "task_monitor.h"
#include "pin_sleep.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "fiq_drvapi.h"
//#include "dal_audio.h"
//#include "ldo_drvapi.h"
//#include "../../tpc/r4p0/tpc_reg_r4p0.h"


#include "dmc400_asm_arm.h"
#include "cp_pwrctrl_asm_arm.h"
#include "cp_sysreg_asm_arm.h"

#ifdef LPS_CTRL_TYPE_PSRAM
#include "psram_phy_asm_arm.h"
#include "tgt_ap_psram_config.h"
#else
#include "lpddr_phy_asm_arm.h"
#include "tgt_ap_lpddr2_config.h"
#endif
#include "sleep.h"


//PUBLIC volatile struct ddr_save_pattern ddr_param[DDR_PARAM_ARRAY_LEN];

//#define DDR_PARAM_ARRAY_LEN 120
//LOCAL const struct ddr_save_pattern ddr_param[DDR_PARAM_ARRAY_LEN];
//PUBLIC volatile struct ddr_save_pattern ddr_param[DDR_PARAM_ARRAY_LEN];

#if 0
/* 
  * save ddr corresponding registers, pad registers is save in aon area,
  * so no need to save. 
  */
PUBLIC void rda_pm_lpddr2_regs_save(void)
{
	uint32 save_addr = (uint32)(&ddr_param[DDR_PARAM_ARRAY_LEN-1]);
	struct ddr_save_pattern *p_save = (struct ddr_save_pattern *)save_addr;
	int32 i = 0;
#ifdef TGT_AP_MEM_TYPE_PSRAM
	uint32 addr_base = REG_PSRAM_PHY_BASE;
#else
	uint32 addr_base = REG_LPDDR_PHY_BASE;
#endif
	uint32 save_reg_num = ARRAY_SIZE(phy_off_val) + ARRAY_SIZE(dmc_off_val);
	DDR_PARAM_SAVE_ADDR = save_addr;

	
	if (save_reg_num >= DDR_PARAM_ARRAY_LEN) {
		SCI_TRACE("The saved register number is more than max(%d)",
		       DDR_PARAM_ARRAY_LEN);
		return;
	}

	writel(DDR_DMC_PHY_SAVE_MAGIC, (void *)&p_save->offs);
	writel(addr_base, (void *)&p_save->val);
	p_save--;

	for (; i < ARRAY_SIZE(phy_off_val); i++) {
		writel(addr_base + phy_off_val[i].offs,
		       (void *)&p_save->offs);
		writel(phy_off_val[i].val, (void *)&p_save->val);
		p_save--;
	}

	for (i = 0; i < ARRAY_SIZE(dmc_off_val); i++) {
		writel(REG_DMC_CTRL_BASE + dmc_off_val[i].offs,
		       (void *)&p_save->offs);
		writel(dmc_off_val[i].val, (void *)&p_save->val);
		p_save--;
	}

	writel(REG_DMC_CTRL_BASE + REG_DMC_CTRL_MEMC_CMD, (void *)&p_save->offs);
	writel(DMC400_MEMC_CMD_GO, (void *)&p_save->val);
	p_save--;

	writel(REG_PWRCTRL_BASE + REG_PWRCTRL_DDR_HOLD_CTRL,
	       (void *)&p_save->offs);
	writel(0, (void *)&p_save->val);
	p_save--;

	writel(REG_SYSREG_BASE + REG_SYSREG_DDR_SLP_WAIT_NUMBER, (void *)&p_save->offs);
	writel(CP_SYSREG_N(0xc8), (void *)&p_save->val);
	p_save--;

	writel(REG_SYSREG_BASE + REG_SYSREG_DDR_SLP_CTRL_ENABLE, (void *)&p_save->offs);
	writel(CP_SYSREG_ENABLE, (void *)&p_save->val);
	p_save--;

	writel(DDR_DMC_PHY_SAVE_END, (void *)&p_save->offs);
}
#endif




#include "sci_types.h"
#include "global_macros.h"
#include "analog_reg_8910m.h"
#include "dmc400_asm_arm.h"
#include "dmc400.h"
#include "cp_pwrctrl.h"
#include "sys_ctrl.h"
#include "tgt_ap_clock_config.h"
#ifdef TGT_AP_MEM_TYPE_PSRAM
#include "psram_phy_asm_arm.h"
#include "tgt_ap_psram_config.h"
#else
#include "lpddr_phy_asm_arm.h"
#include "tgt_ap_lpddr2_config.h"
#endif
#include "lpddr_phy.h"
#include "psram_phy.h"
#include "cp_sysreg_asm_arm.h"
#include "lvds.h"
#include "cp_clkrst.h"
#include "cp_sysreg.h"

#define REGS32(x)   (*((volatile uint32 *)(x)))

#if !defined(TGT_AP_MEM_TYPE_PSRAM) && !defined(FPGA_SUPPORT_UIX8910)
static uint32 ddr_pll_train_ac(void)
{
	uint32 dll_lock, dll_cnt, dll_status;
	//disable dll to clear status
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ac &= ~LPDDR_PHY_RF_DLL_EN_AC;
	//drf_dll_clk_mode_f0 : 0 - clk_dmc_2x_fg; 1 - clk_dmc_fg
	hwp_lpddrPhy->lpddr_drf_cfg_dll_mode_f0 &= ~LPDDR_PHY_DRF_DLL_CLK_MODE_F0;
	//drf_dll_half_mode_f0 : 1 - dll lock cnt = 1/2 T of dl_in
	hwp_lpddrPhy->lpddr_drf_cfg_dll_mode_f0 |= LPDDR_PHY_DRF_DLL_HALF_MODE_F0;

	hwp_lpddrPhy->lpddr_rf_cfg_dll_ac &= ~LPDDR_PHY_RF_DLL_LOCK_WAIT_AC(0xF);
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ac |= LPDDR_PHY_RF_DLL_LOCK_WAIT_AC(6);

	hwp_lpddrPhy->lpddr_rf_cfg_dll_ac &= ~LPDDR_PHY_RF_DLL_PD_CNT_AC(0xF);
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ac |= LPDDR_PHY_RF_DLL_PD_CNT_AC(6);

	//enable dll
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ac |= LPDDR_PHY_RF_DLL_EN_AC;
	//wait dll lock
	dll_lock = 0;
	dll_cnt = 0;
	do {
		dll_status = hwp_lpddrPhy->lpddr_rfdll_status_dll_ac;
		dll_lock = dll_status & LPDDR_PHY_RFDLL_LOCKED_AC;
		dll_cnt = LPDDR_PHY_RFDLL_CNT_AC(dll_status);
	} while((dll_lock == 0) && (dll_cnt != 127));
	rprintf("LPDDR AC slice dll lock : %d\n", dll_lock);
	rprintf("LPDDR AC slice dll cnt : 0x%x\n", dll_cnt);

	return dll_cnt;
}

static uint32 ddr_pll_train_ds0(void)
{
	uint32 dll_lock, dll_cnt, dll_status;
	//disable dll to clear status
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds0 &= ~LPDDR_PHY_RF_DLL_EN_DS0;
	//drf_dll_clk_mode_f0 : 0 - clk_dmc_2x_fg; 1 - clk_dmc_fg
	hwp_lpddrPhy->lpddr_drf_cfg_dll_mode_f0 &= ~LPDDR_PHY_DRF_DLL_CLK_MODE_F0;
	//drf_dll_half_mode_f0 : 1 - dll lock cnt = 1/2 T of dl_in
	hwp_lpddrPhy->lpddr_drf_cfg_dll_mode_f0 |= LPDDR_PHY_DRF_DLL_HALF_MODE_F0;

	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds0 &= ~LPDDR_PHY_RF_DLL_LOCK_WAIT_DS0(0xF);
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds0 |= LPDDR_PHY_RF_DLL_LOCK_WAIT_DS0(6);

	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds0 &= ~LPDDR_PHY_RF_DLL_PD_CNT_DS0(0xF);
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds0 |= LPDDR_PHY_RF_DLL_PD_CNT_DS0(6);

	//enable dll
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds0 |= LPDDR_PHY_RF_DLL_EN_DS0;
	//wait dll lock
	dll_lock = 0;
	dll_cnt = 0;
	do {
		dll_status = hwp_lpddrPhy->lpddr_rfdll_status_dll_ds0;
		dll_lock = dll_status & LPDDR_PHY_RFDLL_LOCKED_DS0;
		dll_cnt = LPDDR_PHY_RFDLL_CNT_AC(dll_status);
	} while((dll_lock == 0) && (dll_cnt != 127));
	rprintf("LPDDR data slice 0 dll lock : %d\n", dll_lock);
	rprintf("LPDDR data slice 0 dll cnt : 0x%x\n", dll_cnt);

	return dll_cnt;
}

static uint32 ddr_pll_train_ds1(void)
{
	uint32 dll_lock, dll_cnt, dll_status;
	//disable dll to clear status
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds1 &= ~LPDDR_PHY_RF_DLL_EN_DS1;
	//drf_dll_clk_mode_f0 : 0 - clk_dmc_2x_fg; 1 - clk_dmc_fg
	hwp_lpddrPhy->lpddr_drf_cfg_dll_mode_f0 &= ~LPDDR_PHY_DRF_DLL_CLK_MODE_F0;
	//drf_dll_half_mode_f0 : 1 - dll lock cnt = 1/2 T of dl_in
	hwp_lpddrPhy->lpddr_drf_cfg_dll_mode_f0 |= LPDDR_PHY_DRF_DLL_HALF_MODE_F0;

	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds1 &= ~LPDDR_PHY_RF_DLL_LOCK_WAIT_DS1(0xF);
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds1 |= LPDDR_PHY_RF_DLL_LOCK_WAIT_DS1(6);

	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds1 &= ~LPDDR_PHY_RF_DLL_PD_CNT_DS1(0xF);
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds1 |= LPDDR_PHY_RF_DLL_PD_CNT_DS1(6);

	//enable dll
	hwp_lpddrPhy->lpddr_rf_cfg_dll_ds1 |= LPDDR_PHY_RF_DLL_EN_DS1;
	//wait dll lock
	dll_lock = 0;
	dll_cnt = 0;
	do {
		dll_status = hwp_lpddrPhy->lpddr_rfdll_status_dll_ds1;
		dll_lock = dll_status & LPDDR_PHY_RFDLL_LOCKED_DS1;
		dll_cnt = LPDDR_PHY_RFDLL_CNT_AC(dll_status);
	} while((dll_lock == 0) && (dll_cnt != 127));
	rprintf("LPDDR data slice 1 dll lock : %d\n", dll_lock);
	rprintf("LPDDR data slice 1 dll cnt : 0x%x\n", dll_cnt);

	return dll_cnt;
}
#endif

#ifdef TGT_AP_MEM_TYPE_PSRAM
static uint32 psram_pll_train_ads0(void)
{
	uint32 dll_lock, dll_cnt, dll_status;
	//disable dll to clear status
	hwp_psramPhy->psram_rf_cfg_dll_ads0 &= ~PSRAM_PHY_RF_DLL_EN_ADS0;
	//drf_dll_clk_mode_f0 : 0 - clk_dmc_2x_fg; 1 - clk_dmc_fg
	hwp_psramPhy->psram_drf_cfg_dll_mode_f0 &= ~PSRAM_PHY_DRF_DLL_CLK_MODE_F0;
	//drf_dll_half_mode_f0 : 1 - dll lock cnt = 1/2 T of dl_in
	hwp_psramPhy->psram_drf_cfg_dll_mode_f0 |= PSRAM_PHY_DRF_DLL_HALF_MODE_F0;

	hwp_psramPhy->psram_rf_cfg_dll_ads0 &= ~PSRAM_PHY_RF_DLL_LOCK_WAIT_ADS0(0xF);
	hwp_psramPhy->psram_rf_cfg_dll_ads0 |= PSRAM_PHY_RF_DLL_LOCK_WAIT_ADS0(6);

	hwp_psramPhy->psram_rf_cfg_dll_ads0 &= ~PSRAM_PHY_RF_DLL_PD_CNT_ADS0(0xF);
	hwp_psramPhy->psram_rf_cfg_dll_ads0 |= PSRAM_PHY_RF_DLL_PD_CNT_ADS0(6);

	//enable dll
	hwp_psramPhy->psram_rf_cfg_dll_ads0 |= PSRAM_PHY_RF_DLL_EN_ADS0;
	//wait dll lock
	dll_lock = 0;
	dll_cnt = 0;
	do {
		dll_status = hwp_psramPhy->psram_rfdll_status_dll_ads0;
		dll_lock = dll_status & PSRAM_PHY_RFDLL_LOCKED_ADS0;
		dll_cnt = PSRAM_PHY_RFDLL_CNT_ADS0(dll_status);
	} while((dll_lock == 0) && (dll_cnt != 255));
	rprintf("PSRAM data slice 0 dll lock : %d\n", dll_lock);
	rprintf("PSRAM data slice 0 dll cnt : 0x%x\n", dll_cnt);

	return dll_cnt;
}

static uint32 psram_pll_train_ads1(void)
{
	uint32 dll_lock, dll_cnt, dll_status;
	//disable dll to clear status
	hwp_psramPhy->psram_rf_cfg_dll_ads1 &= ~PSRAM_PHY_RF_DLL_EN_ADS1;
	//drf_dll_clk_mode_f0 : 0 - clk_dmc_2x_fg; 1 - clk_dmc_fg
	hwp_psramPhy->psram_drf_cfg_dll_mode_f0 &= ~PSRAM_PHY_DRF_DLL_CLK_MODE_F0;
	//drf_dll_half_mode_f0 : 1 - dll lock cnt = 1/2 T of dl_in
	hwp_psramPhy->psram_drf_cfg_dll_mode_f0 |= PSRAM_PHY_DRF_DLL_HALF_MODE_F0;

	hwp_psramPhy->psram_rf_cfg_dll_ads1 &= ~PSRAM_PHY_RF_DLL_LOCK_WAIT_ADS1(0xF);
	hwp_psramPhy->psram_rf_cfg_dll_ads1 |= PSRAM_PHY_RF_DLL_LOCK_WAIT_ADS1(6);

	hwp_psramPhy->psram_rf_cfg_dll_ads1 &= ~PSRAM_PHY_RF_DLL_PD_CNT_ADS1(0xF);
	hwp_psramPhy->psram_rf_cfg_dll_ads1 |= PSRAM_PHY_RF_DLL_PD_CNT_ADS1(6);

	//enable dll
	hwp_psramPhy->psram_rf_cfg_dll_ads1 |= PSRAM_PHY_RF_DLL_EN_ADS1;
	//wait dll lock
	dll_lock = 0;
	dll_cnt = 0;
	do {
		dll_status = hwp_psramPhy->psram_rfdll_status_dll_ads1;
		dll_lock = dll_status & PSRAM_PHY_RFDLL_LOCKED_ADS1;
		dll_cnt = PSRAM_PHY_RFDLL_CNT_ADS1(dll_status);
	} while((dll_lock == 0) && (dll_cnt != 255));
	rprintf("PSRAM data slice 1 dll lock : %d\n", dll_lock);
	rprintf("PSRAM data slice 1 dll cnt : 0x%x\n", dll_cnt);

	return dll_cnt;
}

#endif

static void ddr_phy_config(uint32 flag)
{
#ifdef TGT_AP_MEM_TYPE_PSRAM
	uint32 addr_base = REG_PSRAM_PHY_BASE;
#else
	uint32 addr_base = REG_LPDDR_PHY_BASE;
#endif
	int i;

#if !defined(TGT_AP_MEM_TYPE_PSRAM) && !defined(FPGA_SUPPORT_UIX8910)
	uint32 dll_cnt_ac, dll_cnt_ds0, dll_cnt_ds1;
	//enable phy
	hwp_lpddrPhy->lpddr_rf_cfg_phy &= ~LPDDR_PHY_RF_PHY_INIT_COMPLETE;
	hwp_lpddrPhy->lpddr_rf_cfg_phy |= LPDDR_PHY_RF_PHY_EN;
	//enable fg clk for dll train
	hwp_lpddrPhy->lpddr_rf_cfg_clock_gate |= LPDDR_PHY_RF_CLK_GATE_EN;
	hwp_lpddrPhy->lpddr_rf_cfg_clock_gate |= LPDDR_PHY_RF_CLK_GATE_FG_EN;

	dll_cnt_ac = ddr_pll_train_ac();
	dll_cnt_ds0 = ddr_pll_train_ds0();
	dll_cnt_ds1 = ddr_pll_train_ds1();
#endif

#ifdef TGT_AP_MEM_TYPE_PSRAM

	uint32 dll_cnt_ds0, dll_cnt_ds1;
	//enable phy
	hwp_psramPhy->psram_rf_cfg_phy &= ~PSRAM_PHY_RF_PHY_INIT_COMPLETE;
	hwp_psramPhy->psram_rf_cfg_phy |= PSRAM_PHY_RF_PHY_EN;
	//enable fg clk for dll train
	hwp_psramPhy->psram_rf_cfg_clock_gate |= PSRAM_PHY_RF_CLK_GATE_EN;
	hwp_psramPhy->psram_rf_cfg_clock_gate |= PSRAM_PHY_RF_CLK_GATE_FG_EN;

	dll_cnt_ds0 = psram_pll_train_ads0();
	dll_cnt_ds1 = psram_pll_train_ads1();
#endif

	for (i = 0; i < ARRAY_SIZE(phy_off_val); i++) {
		writel(phy_off_val[i].val,
				(void *)(addr_base + phy_off_val[i].offs));
		rprintf("phy reg %8x = %8x\n", addr_base + phy_off_val[i].offs,
				phy_off_val[i].val);
	}

#if !defined(TGT_AP_MEM_TYPE_PSRAM) && !defined(FPGA_SUPPORT_UIX8910)
	hwp_lpddrPhy->lpddr_rf_cfg_dll_dl_0_wr_ac = dll_cnt_ac;
	hwp_lpddrPhy->lpddr_rf_cfg_dll_dl_0_wr_ds0 = dll_cnt_ds0;
	hwp_lpddrPhy->lpddr_rf_cfg_dll_dl_1_wr_ds0 = dll_cnt_ds0 - 10;
	hwp_lpddrPhy->lpddr_rf_cfg_dll_dl_2_wr_ds0 = dll_cnt_ds0 - 10;
	hwp_lpddrPhy->lpddr_rf_cfg_dll_dl_3_wr_ds0 = dll_cnt_ds0;
	hwp_lpddrPhy->lpddr_rf_cfg_dll_dl_0_wr_ds1 = dll_cnt_ds1;
	hwp_lpddrPhy->lpddr_rf_cfg_dll_dl_1_wr_ds1 = dll_cnt_ds1 - 10;
	hwp_lpddrPhy->lpddr_rf_cfg_dll_dl_2_wr_ds1 = dll_cnt_ds1 - 10;
	hwp_lpddrPhy->lpddr_rf_cfg_dll_dl_3_wr_ds1 = dll_cnt_ds1;
#endif

#ifdef TGT_AP_MEM_TYPE_PSRAM
	hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads0 = dll_cnt_ds0;
	hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = dll_cnt_ds0 - 10;
	hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = dll_cnt_ds0 - 10;
	hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = dll_cnt_ds0;
	hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads1 = dll_cnt_ds1;
	hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads1 = dll_cnt_ds1 - 10;
	hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads1 = dll_cnt_ds1 - 10;
	hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads1 = dll_cnt_ds1;
#endif
}

#ifndef TGT_AP_MEM_TYPE_PSRAM
static void ddr_pad_config(void)
{
#if defined(FPGA_SUPPORT_UIX8910)
	// disable PSRAM pad
	hwp_analogReg->psram_pad_cfg = 0x0;
	hwp_lpddrPhy->io_rf_psram_pad_en_cfg = 0x0;

	// enable LPDDR pad
	hwp_lpddrPhy->io_rf_ddr_pad_cfg =
	    (LPDDR_PHY_DDR_LP2C | LPDDR_PHY_DDR_LP2D);

	// disable ddr holdg
	hwp_pwrctrl->DDR_HOLD_CTRL = 0;
#else
	// disable PSRAM pad
	hwp_analogReg->psram_pad_cfg = 0x0;
	hwp_lpddrPhy->io_rf_psram_pad_en_cfg = 0x0;
	hwp_lpddrPhy->io_rf_psram_drv_cfg	 = 0x0;
	hwp_lpddrPhy->io_rf_psram_pull_cfg	 = 0x0;
	hwp_lpddrPhy->io_rf_psram_reserved   = 0x0;

	// enable LPDDR pad
	hwp_lpddrPhy->io_rf_ddr_pad_cfg	  = 0x0;
	hwp_lpddrPhy->io_rf_ddr_rpull_cfg = 0x1;
	hwp_lpddrPhy->io_rf_ddr_vref_cfg  = (LPDDR_PHY_DDR_VREF_EN_INT
						| LPDDR_PHY_DDR_SEL_INT(32)
						| LPDDR_PHY_DDR_SEL_PAD(32));
	hwp_lpddrPhy->io_rf_ddr_zq_cfg	= LPDDR_PHY_DDR_CAL_ZQ_PD;
#endif
}
#else
static void psram_pad_config(void)
{
    uint32 val;

#if defined(FPGA_SUPPORT_UIX8910)
	unsigned int rdata;

	// disable LPDDR pad
	hwp_analogReg->ddr_pad_cfg = 0x0;
	hwp_lpddrPhy->io_rf_ddr_pad_cfg = 0x0;

	// enable PSRAM pad
	rdata = hwp_lpddrPhy->io_rf_psram_drv_cfg;
	hwp_lpddrPhy->io_rf_psram_drv_cfg = rdata & (~LPDDR_PHY_PSRAM_FIX_READ0
		& LPDDR_PHY_PSRAM_FIX_READ0);
	hwp_lpddrPhy->io_rf_psram_pad_en_cfg = LPDDR_PHY_PSRAM_PAD_CLKN_EN;

	// disable ddr holdg
	hwp_pwrctrl->DDR_HOLD_CTRL = 0x0;
#else
	// disable LPDDR pad
	hwp_lpddrPhy->io_rf_ddr_pad_cfg = 0x0;

	// drive strengh
#if 0
	hwp_lpddrPhy->io_rf_psram_drv_cfg = LPDDR_PHY_PSRAM_DRVN(0x10)
					| LPDDR_PHY_PSRAM_DRVP(0x10)
					| LPDDR_PHY_PSRAM_SLEWRATE(0x0);
#else
	hwp_lpddrPhy->io_rf_psram_drv_cfg = LPDDR_PHY_PSRAM_DRVN(0x4)
					| LPDDR_PHY_PSRAM_DRVP(0x4)
					| LPDDR_PHY_PSRAM_SLEWRATE(0x0);
#endif
	// pull up/down
	hwp_lpddrPhy->io_rf_psram_pull_cfg = LPDDR_PHY_PSRAM_DQS_PULL0_BIT(1)
					| LPDDR_PHY_PSRAM_DQ_PULL0_BIT(0)
					| LPDDR_PHY_PSRAM_DM_PULL1_BIT(1)
					| LPDDR_PHY_PSRAM_CLKN_PULL1_BIT(1)
					| LPDDR_PHY_PSRAM_CLK_PULL0_BIT(1)
					| LPDDR_PHY_PSRAM_CEN_PULL1_BIT(1);
	// enable psram IO
	hwp_lpddrPhy->io_rf_psram_pad_en_cfg = LPDDR_PHY_PSRAM_PAD_CLKN_EN;

	// psram_drf_t_chp_rd.drf_t_cpu_rd_optm = 1'b1
	val = REGS32(0x8801D14);
	val |= 0x10;
	REGS32(0x8801D14) = val;

	// psram_drf_t_rddata_valid_early.drf_t_rddata_valid_early = 2'b11
	REGS32(0x8801D2c) = 0x2;

 	// psram_drf_t_rddata_vld_sync.drf_t_rddata_vld_sync = 2
	REGS32(0x8801D24) = 0x2;
#endif
}
#endif /* #ifndef TGT_AP_MEM_TYPE_PSRAM */

#define SYS_QOS_REG_BASE		REG_SYS_CTRL_BASE

#define SYS_CLK_AON_AHB_ENABLE		0x98
#define SYS_QOS_REG_WCN_A5_GGE		0x238
#define SYS_QOS_REG_AXIDMA_CPA5_F8	0x23c
#define SYS_QOS_REG_LCDC_LZMA_GOUDA	0x240
#define SYS_QOS_REG_LTE_USB         0x244

static void ddr_dmc400_qos_config(void)
{
    uint32 addr, val;
    uint32 base = SYS_QOS_REG_BASE;
    uint32 tmp;

    // system register AXIDMA, CPA5, F8
    addr = base + SYS_QOS_REG_AXIDMA_CPA5_F8;
    val = readl(addr);
    val &= ~( SYS_CTRL_AXIDMA_ARQOS(0x1f)
    | SYS_CTRL_AXIDMA_AWQOS(0x1f)
    | SYS_CTRL_CP_A5_ARQOS(0x1f)
    | SYS_CTRL_CP_A5_AWQOS(0x1f)
    | SYS_CTRL_F8_ARQOS(0x1f)
    | SYS_CTRL_F8_AWQOS(0x1f));

    val |=  ( SYS_CTRL_AXIDMA_ARQOS(0x1)	// AXIDMA read
    | SYS_CTRL_AXIDMA_AWQOS(0X1)	// AXIDMA write
    | SYS_CTRL_CP_A5_ARQOS(0x7)	// CP	read
    | SYS_CTRL_CP_A5_AWQOS(0x7)	// CP	write
    | SYS_CTRL_F8_ARQOS(0x5)	// F8	read
    | SYS_CTRL_F8_AWQOS(0x5));	// F8	write
    writel(val, addr);
    rprintf("sys reg %8x = %8x\n", addr, val);

    // system register AP, GGE, WCN
    addr = base + SYS_QOS_REG_WCN_A5_GGE;
    val = readl(addr);
    val &= ~( SYS_CTRL_GGE_ARQOS(0x1f)
    | SYS_CTRL_GGE_AWQOS(0x1f)
    | SYS_CTRL_A5_ARQOS(0x1f)
    | SYS_CTRL_A5_AWQOS(0x1f));

    val |=  ( SYS_CTRL_GGE_ARQOS(0x6)	// GGE	read
    | SYS_CTRL_GGE_AWQOS(0x6)	// GGE	write
    | SYS_CTRL_A5_ARQOS(0x3)	// AP	read
    | SYS_CTRL_A5_AWQOS(0x3));	// AP	write
    writel(val, addr);
    rprintf("sys reg %8x = %8x\n", addr, val);

    writel(SYS_CTRL_ENABLE_AON_AHB_CLK_ID_BB_SYSCTRL, base + SYS_CLK_AON_AHB_ENABLE);
    hwp_clkrst->CLKEN_BB_SYSCTRL_SET = CP_CLKRST_CLKEN_BB_SYSCTRLSET(1);
    hwp_sysreg->ZSP_QOS &= ~(CP_SYSREG_AWQOS_ZSP_AXIDMA(15) | CP_SYSREG_ARQOS_ZSP_AXIDMA(15));
    hwp_sysreg->ZSP_QOS |= (CP_SYSREG_AWQOS_ZSP_AXIDMA(4) | CP_SYSREG_ARQOS_ZSP_AXIDMA(4));
    rprintf("sys reg %8x = %8x\n", &hwp_sysreg->ZSP_QOS, hwp_sysreg->ZSP_QOS);
	
    // system register LTE, USB
    addr = base + SYS_QOS_REG_LTE_USB;
    val = readl(addr);
    val &= ~( SYS_CTRL_LTE_ARQOS(0x1f)
    | SYS_CTRL_LTE_AWQOS(0x1f)
    | SYS_CTRL_USB_ARQOS(0x1f)
    | SYS_CTRL_USB_AWQOS(0x1f));

    val |=  ( SYS_CTRL_LTE_ARQOS(0x7)	// LTE	read
    | SYS_CTRL_LTE_AWQOS(0x7)	// LTE	write
    | SYS_CTRL_USB_ARQOS(0x2)	// USB	read
    | SYS_CTRL_USB_AWQOS(0x2));	// USB	write
    writel(val, addr);
    rprintf("sys reg %8x = %8x\n", addr, val);

    addr = REG_DMC_CTRL_BASE + REG_DMC_CTRL_QOS0_CONTROL;
#if defined(DDR_TYPE_DDR) || defined(CHIP_VER_UIS8910A)
    val = (0x0 << 8) | 0x0;
#else
    val = (0xf << 8) | 0x0;
#endif
    writel(val, addr);
    rprintf("qos reg %8x = %8x\n", addr, val);

    // DMC400 qos1_control
    addr = REG_DMC_CTRL_BASE + REG_DMC_CTRL_QOS1_CONTROL;
#if defined(DDR_TYPE_DDR) || defined(CHIP_VER_UIS8910A)
    val = (0x0 << 8) | 0x1;
#else
    val = (0xf << 8) | 0x1;
#endif
    writel(val, addr);
    rprintf("qos reg %8x = %8x\n", addr, val);

    // DMC400 qos2_control
    addr = REG_DMC_CTRL_BASE + REG_DMC_CTRL_QOS2_CONTROL;
#if defined(DDR_TYPE_DDR) || defined(CHIP_VER_UIS8910A)
    val = (0x0 << 8) | 0x2;
#else
    val = (0xf << 8) | 0x2;
#endif
    writel(val, addr);
    rprintf("qos reg %8x = %8x\n", addr, val);

    // DMC400 qos3_control
    addr = REG_DMC_CTRL_BASE + REG_DMC_CTRL_QOS3_CONTROL;
#if defined(DDR_TYPE_DDR) || defined(CHIP_VER_UIS8910A)
    val = (0x0 << 8) | 0x3;
#else
    val = (0xf << 8) | 0x3;
#endif
    writel(val, addr);
    rprintf("qos reg %8x = %8x\n", addr, val);

    // DMC400 qos4_control
    addr = REG_DMC_CTRL_BASE + REG_DMC_CTRL_QOS4_CONTROL;
#if defined(DDR_TYPE_DDR) || defined(CHIP_VER_UIS8910A) 
        val = (0x0 << 8) | 0x4; //CP: priority = 4, timeout = 1
#else
        val = (0xf << 8) | 0x4; //CP: priority = 4, timeout = 1
#endif
    writel(val, addr);
    rprintf("qos reg %8x = %8x\n", addr, val);

    // DMC400 qos5_control
    addr = REG_DMC_CTRL_BASE + REG_DMC_CTRL_QOS5_CONTROL;
#if defined(DDR_TYPE_DDR) || defined(CHIP_VER_UIS8910A) 
    val = (0x0 << 8) | 0x5;
#else
    val = (0xf << 8) | 0x5;
#endif
    writel(val, addr);
    rprintf("qos reg %8x = %8x\n", addr, val);

    // DMC400 qos6_control
    addr = REG_DMC_CTRL_BASE + REG_DMC_CTRL_QOS6_CONTROL;
#if defined(DDR_TYPE_DDR) || defined(CHIP_VER_UIS8910A)
    val = (0x0 << 8) | 0x6;
#else
    val = (0xf << 8) | 0x6;
#endif
    writel(val, addr);
    rprintf("qos reg %8x = %8x\n", addr, val);

    // DMC400 qos7_control
    addr = REG_DMC_CTRL_BASE + REG_DMC_CTRL_QOS7_CONTROL;    
#if defined(DDR_TYPE_DDR) || defined(CHIP_VER_UIS8910A)
    val = (0x0 << 8) | 0x7;
#else
    val = (0xf << 8) | 0x7;
#endif
    writel(val, addr);
    rprintf("qos reg %8x = %8x\n", addr, val);

#if 0
    // DMC400 turnaround priority
    addr = REG_DMC_CTRL_BASE + REG_DMC_CTRL_TURNAROUND_PRIORITY;
    val = (0xf << 8) | 0x3;//limit = 0xf, priority = 3
    writel(val, addr);
    rprintf("qos reg %8x = %8x\n", addr, val);

    // DMC400 hit priority
    addr = REG_DMC_CTRL_BASE + REG_DMC_CTRL_HIT_PRIORITY;
    val = (0xf << 8) | 0x3;//limit = 0xf, priority = 3
    writel(val, addr);
    rprintf("qos reg %8x = %8x\n", addr, val);
#endif

    hwp_dmcCtrl->hit_priority = 0x41;
    hwp_dmcCtrl->turnaround_priority = 0x43;
}

static void ddr_power_up_config(void);

static void ddr_dmc400_config(void)
{
	/* NOTE: memc_cmd is DMC400_MEMC_CMD_CONFIG by default after reset
	 * when DMC400 is in CONFIG mode, write memc_cmd as CONFIG will assert pslverr
	 * hwp_dmcCtrl->memc_cmd = DMC400_MEMC_CMD_CONFIG;
	 */
	int i;
	unsigned int base = REG_DMC_CTRL_BASE, status;

	status = readl(base + REG_DMC_CTRL_MEMC_STATUS);
	if (status != DMC400_MEMC_CMD_CONFIG) {
		writel(DMC400_MEMC_CMD_CONFIG, base + REG_DMC_CTRL_MEMC_CMD);
		udelay(10);
		status = readl(base + REG_DMC_CTRL_MEMC_STATUS);
	}
	rprintf("dmc status1: %d\n", status);

	for (i = 0; i < ARRAY_SIZE(dmc_off_val); i++) {
		if ((dmc_off_val[i].offs == REG_DMC_CTRL_MEMC_CMD)
			&& (dmc_off_val[i].val == DMC400_MEMC_CMD_GO))
			continue;

		writel(dmc_off_val[i].val, (void *)(base + dmc_off_val[i].offs));
		rprintf("dmc reg %8x = %8x\n", base + dmc_off_val[i].offs,
			dmc_off_val[i].val);
	}
	ddr_dmc400_qos_config();
	ddr_power_up_config();

	writel(DMC400_MEMC_CMD_GO, base + REG_DMC_CTRL_MEMC_CMD);
	udelay(10);
	rprintf("dmc status2:%d\n", readl(base + REG_DMC_CTRL_MEMC_STATUS));
}

static void ddr_power_up_config(void)
{
	int i;
	unsigned int base = REG_DMC_CTRL_BASE;

	for (i = 0; i < ARRAY_SIZE(dmc_cmd_off_val); i++) {
		if (dmc_cmd_off_val[i].offs != UDELAY_SYMBOL)
			rprintf("dircmd %8x = %8x\n",
				base + dmc_cmd_off_val[i].offs,
				dmc_cmd_off_val[i].val);
	}

	for (i = 0; i < ARRAY_SIZE(dmc_cmd_off_val); i++) {
		if (dmc_cmd_off_val[i].offs == UDELAY_SYMBOL)
			udelay(dmc_cmd_off_val[i].val);
		else
			writel(dmc_cmd_off_val[i].val,
					(void *)(REG_DMC_CTRL_BASE + dmc_cmd_off_val[i].offs));
	}
}

static void ddr_auto_sleep_config(void)
{
	unsigned int reg_base = REG_SYSREG_BASE;

	writel(CP_SYSREG_N(0xc8), reg_base + REG_SYSREG_DDR_SLP_WAIT_NUMBER);
	writel(CP_SYSREG_ENABLE, reg_base + REG_SYSREG_DDR_SLP_CTRL_ENABLE);
	rprintf("ddr auto sleep enable! \n");
}

static void lvds_config(void)
{
	unsigned int reg_base = REG_LVDS_BASE;

	writel(LVDS_LVDS_TX_HZ_ENABLE(0x1f), reg_base + 0x134);
	writel(LVDS_LVDS_RX_PU_DIFF2CMOS(0), reg_base + 0x124);
	rprintf("lvds disable! \n");
}

int ddr_init(void)
{
	uint32 flags = 0;

#ifndef TGT_AP_MEM_TYPE_PSRAM
	rprintf("Init LPDDR2 ...\n");
#else
	rprintf("Init PSRAM ...\n");
#endif

	ddr_phy_config(flags);

#ifndef TGT_AP_MEM_TYPE_PSRAM
	ddr_pad_config();
#else
	psram_pad_config();
#endif

	ddr_dmc400_config();

#ifndef TGT_AP_MEM_TYPE_PSRAM
#ifndef FPGA_SUPPORT_UIX8910
	ddr_auto_sleep_config();
#endif
#endif
	rprintf("Done\n");

#ifndef FPGA_SUPPORT_UIX8910
	lvds_config();
#endif
	return 0;
}

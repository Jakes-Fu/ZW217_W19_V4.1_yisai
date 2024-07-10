#include "sci_types.h"
#include "sys_ctrl.h"
#include "global_macros.h"
#include "analog_reg.h"
#include "clk_div.h"
#include "cp_clkrst.h"
#include "idle.h"
#include "tgt_ap_clock_config.h"
#include "timer_drvapi.h"
//#include "arm_uart.h"
#include "uart_reg_uws6121e.h"


#include "ap_clk.h"     //w217
#include "aon_clk.h"    //w217
#include "ap_apb.h"     //w217
#include "idle_lps.h"   //w217

/*
static void prvFlashClkInit(void) //后续这个东西需要只放到FDL1或2初始化
{
    REG_AP_CLK_CGM_SPIFLASH1_SEL_CFG_T CFG;
	REG_AP_APB_CLK_AP_EN0_T clken;
    REG_AP_APB_CFG_CLK_SPIFLASH1_T ap_clk;

	REG_SPI_FLASH_SPI_CONFIG_T SPI_CFG;

	REG_IOMUX_PAD_M_SPI_D_3_T iocfg_d3;
    REG_IOMUX_PAD_M_SPI_D_2_T iocfg_d2;
    REG_IOMUX_PAD_M_SPI_D_1_T iocfg_d1;
    REG_IOMUX_PAD_M_SPI_D_0_T iocfg_d0;
    REG_IOMUX_PAD_M_SPI_CS_T iocfg_cs;
    REG_IOMUX_PAD_M_SPI_CLK_T iocfg_cLK;

	CFG.v = 0;
	clken.v = hwp_apApb->clk_ap_en0;

	ap_clk.v = 0;
    ap_clk.b.spiflash1_freq = 0xc;
    hwp_apApb->cfg_clk_spiflash1 = ap_clk.v;

    clken.b.clk_en_spiflash1_fr = 1;
    hwp_apApb->clk_ap_en0 = clken.v;
    CFG.b.cgm_spiflash1_sel = 0x4;
    hwp_apClk->cgm_spiflash1_sel_cfg = CFG.v;

	SPI_CFG.v = 0;
    SPI_CFG.b.clk_divider = 0x2;
    SPI_CFG.b.sample_delay = 0x2;
    SPI_CFG.b.quad_mode = 1;
    hwp_spiFlash->spi_config = SPI_CFG.v;

	iocfg_d3.v = 0;
    iocfg_d3.b.dslp_en = 63;
    iocfg_d3.b.drv = 4;
    hwp_iomux->pad_m_spi_d_3 = iocfg_d3.v;

	iocfg_d2.v = 0;
    iocfg_d2.b.dslp_en = 63;
    iocfg_d2.b.drv = 4;
    hwp_iomux->pad_m_spi_d_2 = iocfg_d2.v;

	iocfg_d1.v = 0;
    iocfg_d1.b.dslp_en = 63;
    iocfg_d1.b.drv = 4;
    hwp_iomux->pad_m_spi_d_1 = iocfg_d1.v;

	iocfg_d0.v = 0;
    iocfg_d0.b.dslp_en = 63;
    iocfg_d0.b.drv = 4;
    hwp_iomux->pad_m_spi_d_0 = iocfg_d0.v;

	iocfg_cs.v = 0;
    iocfg_cs.b.dslp_en = 63;
    iocfg_cs.b.drv = 4;
    hwp_iomux->pad_m_spi_cs = iocfg_cs.v;

	iocfg_cLK.v = 0;
    iocfg_cLK.b.dslp_en = 63;
    iocfg_cLK.b.drv = 4;
    hwp_iomux->pad_m_spi_clk = iocfg_cLK.v;
}

*/
#pragma arm section code = "CLOCK_INIT_AONRAM"

void clock_init(void)
{

	REG_AP_CLK_CGM_AP_A5_SEL_CFG_T AP_CLK_CGM_AP_A5_SEL_CFG;
	REG_AON_CLK_CGM_AON_AHB_SEL_CFG_T AON_CLK_CGM_AON_AHB_SEL_CFG;
	REG_AP_CLK_CGM_AP_A5_DIV_CFG_T AP_CLK_CGM_AP_A5_DIV_CFG;
	REG_AP_CLK_CGM_AP_BUS_DIV_CFG_T AP_CLK_CGM_AP_BUS_DIV_CFG;
	REG_AON_CLK_CGM_AON_AHB_DIV_CFG_T AON_CLK_CGM_AON_AHB_DIV_CFG;
	REG_IDLE_LPS_AP_AWK_EN_T ap_awk_en;
	REG_IDLE_LPS_AP_AWK_EN1_T ap_awk_en1;
	REG_IDLE_LPS_AP_INTEN_T IDLE_LPS_AP_INTEN;

	// choose apll500M, aon defalut HW ctrl
	AP_CLK_CGM_AP_A5_SEL_CFG.b.cgm_ap_a5_sel = 5;
	hwp_apClk->cgm_ap_a5_sel_cfg = AP_CLK_CGM_AP_A5_SEL_CFG.v;

	// choose apll200M
	AON_CLK_CGM_AON_AHB_SEL_CFG.b.cgm_aon_ahb_sel = 7;
	hwp_aonClk->cgm_aon_ahb_sel_cfg = AON_CLK_CGM_AON_AHB_SEL_CFG.v;

	AP_CLK_CGM_AP_A5_DIV_CFG.b.cgm_ap_a5_div = 0;
	hwp_apClk->cgm_ap_a5_div_cfg = AP_CLK_CGM_AP_A5_DIV_CFG.v;

	AP_CLK_CGM_AP_BUS_DIV_CFG.b.cgm_ap_bus_div = 1;
	hwp_apClk->cgm_ap_bus_div_cfg = AP_CLK_CGM_AP_BUS_DIV_CFG.v;

	AON_CLK_CGM_AON_AHB_DIV_CFG.b.cgm_aon_ahb_div = 0;
	hwp_aonClk->cgm_aon_ahb_div_cfg = AON_CLK_CGM_AON_AHB_DIV_CFG.v;


    // close debug function, use clk_mode and clk_en control
    hwp_apApb->cgm_gate_auto_sel0 = 0xffffffff;
    hwp_apApb->cgm_gate_auto_sel1 = 0xffffffff;
    hwp_apApb->cgm_gate_auto_sel2 = 0xffffffff;
    hwp_apApb->cgm_gate_auto_sel3 = 0xffffffff;

    hwp_apApb->clk_ap_mode0 = 0; //clk_core auto gating
    hwp_apApb->clk_ap_mode1 = 0; //ip clk auto gating
    hwp_apApb->clk_ap_mode2 = 0;
    hwp_apApb->clk_ap_en1 = 0;
    hwp_apApb->clk_ap_en2 = 0;

    // debug host(current uart4 be used to debug log func)
    //REGT_FIELD_CHANGE(hwp_iomux->debug_host_tx, REG_IOMUX_DEBUG_HOST_TX_T, func_sel, 2);
    //REGT_FIELD_CHANGE(hwp_iomux->debug_host_rx, REG_IOMUX_DEBUG_HOST_RX_T, func_sel, 2);

    // enable wakeup sources
	ap_awk_en.b.ap_awk0_en = 1; // pmic
	ap_awk_en.b.ap_awk1_en = 1; // uart1
	ap_awk_en.b.ap_awk2_en = 1; // keypad
	ap_awk_en.b.ap_awk3_en = 1; // gpio1
	ap_awk_en.b.ap_awk4_en = 1; // gpt1
	ap_awk_en.b.ap_awk5_en = 1; // uart1_rx
	ap_awk_en.b.ap_awk6_en = 1; // mailbox_ap
	ap_awk_en.b.ap_awk7_en = 1; // mailbox_cp
	// ap_awk_en.b.ap_awk8_en = 1; // uart2
	// ap_awk_en.b.ap_awk9_en = 1; // uart3
	ap_awk_en.b.ap_awk10_en = 1; // gpio2
	ap_awk_en.b.ap_awk11_en = 1; // gpt2_irq0
	ap_awk_en.b.ap_awk12_en = 1; // gpt2_irq1
	ap_awk_en.b.ap_awk13_en = 1; // gpt2_irq2
	// ap_awk_en.b.ap_awk14_en = 1; // uart2_rx
	// ap_awk_en.b.ap_awk15_en = 1; // uart3_rx
	ap_awk_en.b.ap_awk16_en = 1; // usb
	ap_awk_en.b.ap_awk17_en = 1; // spi2
	ap_awk_en.b.ap_awk19_en = 1; // rtc_timer
	ap_awk_en.b.ap_p1_awk_en = 1;
	// ap_awk_en.b.ap_t1_awk_en = 1;
	// ap_awk_en.b.ap_t2_awk_en = 1;
	// ap_awk_en.b.ap_t3_awk_en = 1;
	// ap_awk_en.b.ap_t4_awk_en = 1;
	// ap_awk_en.b.ap_t5_awk_en = 1;
	// ap_awk_en.b.ap_t6_awk_en = 1;
    hwp_idleLps->ap_awk_en_set = ap_awk_en.v;

	ap_awk_en1.b.ap_t7_awk_en = 1;
	ap_awk_en1.b.ap_t8_awk_en = 1;
	ap_awk_en1.b.ap_t9_awk_en = 1;
	hwp_idleLps->ap_awk_en1_set = ap_awk_en1.v;

	IDLE_LPS_AP_INTEN.b.ap_sys_awk_irq_to_ap_en = 1;
	hwp_idleLps->ap_inten = IDLE_LPS_AP_INTEN.v;


    // hwp_idleTimer->slow_sys_clk_sel_hwen = 1;
    // hwp_idleTimer->slow_clk_sel_hwen = 1;

    //prvFlashClkInit();  //需要放在别处(如nor_fdl1或nor_fdl12)调用
/*
	//原8910代码中没有mailbox和硬件自旋锁相关逻辑，暂不处理(这个hwp_mailbox,8910是放在kernel的phase1阶段进行的初始化)
    halHwspinlockAcquireInCritical(HAL_HWSPINLOCK_ID_CPSLEEP);
    for (uintptr_t address = (uintptr_t)&hwp_mailbox->sysmail0;
         address <= (uintptr_t)&hwp_mailbox->sysmail95; address += 4)
        *(volatile unsigned *)address = 0;
    halHwspinlockReleaseInCritical(HAL_HWSPINLOCK_ID_CPSLEEP);
*/

}

#pragma arm section code

void udelay(uint32 usec)
{
	int i;

	for (i = 0; i < usec; i++);
}




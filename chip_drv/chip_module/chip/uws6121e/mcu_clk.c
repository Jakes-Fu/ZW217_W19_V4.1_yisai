#include "ap_clk.h"
#include "aon_clk.h"
#include "idle_lps.h"
#include "ap_apb.h"

int clock_init(void)
{
	REG_AP_CLK_CGM_AP_A5_SEL_CFG_T ap_a5_sel_cfg;
    REG_AON_CLK_CGM_AON_AHB_SEL_CFG_T aon_ahb_sel_cfg;
    REG_AP_CLK_CGM_AP_A5_DIV_CFG_T ap_a5_div_cfg;
    REG_AP_CLK_CGM_AP_BUS_DIV_CFG_T ap_bus_div_cfg;
    REG_AON_CLK_CGM_AON_AHB_DIV_CFG_T aon_ahb_div;
    REG_IOMUX_DEBUG_HOST_TX_T host_tx;
    REG_IOMUX_DEBUG_HOST_RX_T host_rx;
    REG_IDLE_LPS_AP_INTEN_T ap_sys_awk_irq_to_ap_en_cfg;
    REG_IDLE_LPS_AP_AWK_EN_T ap_awk_en;
    REG_IDLE_LPS_AP_AWK_EN1_T ap_awk_en1;

    // choose apll500M, aon defalut HW ctrl
    ap_a5_sel_cfg.v = hwp_apClk->cgm_ap_a5_sel_cfg;
    ap_a5_sel_cfg.b.cgm_ap_a5_sel = 5;
    hwp_apClk->cgm_ap_a5_sel_cfg = ap_a5_sel_cfg.v;

    // choose apll200M
    aon_ahb_sel_cfg.v = hwp_aonClk->cgm_aon_ahb_sel_cfg;
    aon_ahb_sel_cfg.b.cgm_aon_ahb_sel = 7;
    hwp_aonClk->cgm_aon_ahb_sel_cfg = aon_ahb_sel_cfg.v;

    ap_a5_div_cfg.v = hwp_apClk->cgm_ap_a5_div_cfg;
    ap_a5_div_cfg.b.cgm_ap_a5_div = 0;
    hwp_apClk->cgm_ap_a5_div_cfg = ap_a5_div_cfg.v;

    ap_bus_div_cfg.v = hwp_apClk->cgm_ap_bus_div_cfg;
    ap_bus_div_cfg.b.cgm_ap_bus_div = 1;
    hwp_apClk->cgm_ap_bus_div_cfg = ap_bus_div_cfg.v;

    aon_ahb_div.v = hwp_aonClk->cgm_aon_ahb_div_cfg;
    aon_ahb_div.b.cgm_aon_ahb_div = 0;
    hwp_aonClk->cgm_aon_ahb_div_cfg = aon_ahb_div.v;

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

    // enable wakeup sources
    ap_awk_en.b.ap_awk0_en = 1; // pmic
    ap_awk_en.b.ap_awk1_en = 1; // uart1
    ap_awk_en.b.ap_awk2_en = 1; // keypad
    ap_awk_en.b.ap_awk3_en = 1; // gpio1
    ap_awk_en.b.ap_awk4_en = 1; // gpt1
    ap_awk_en.b.ap_awk5_en = 1; // uart1_rx
    ap_awk_en.b.ap_awk6_en = 1; // mailbox_ap
    ap_awk_en.b.ap_awk7_en = 1; // mailbox_cp
    // .b.ap_awk8_en = 1, // uart2
    // .b.ap_awk9_en = 1, // uart3
    ap_awk_en.b.ap_awk10_en = 1; // gpio2
    ap_awk_en.b.ap_awk11_en = 1; // gpt2_irq0
    ap_awk_en.b.ap_awk12_en = 1; // gpt2_irq1
    ap_awk_en.b.ap_awk13_en = 1; // gpt2_irq2
    // .b.ap_awk14_en = 1, // uart2_rx
    // .b.ap_awk15_en = 1, // uart3_rx
    ap_awk_en.b.ap_awk16_en = 1; // usb
    ap_awk_en.b.ap_awk17_en = 1; // spi2
    ap_awk_en.b.ap_awk19_en = 1; // rtc_timer
    ap_awk_en.b.ap_p1_awk_en = 1;
    // .b.ap_t1_awk_en = 1,
    // .b.ap_t2_awk_en = 1,
    // .b.ap_t3_awk_en = 1,
    // .b.ap_t4_awk_en = 1,
    // .b.ap_t5_awk_en = 1,
    // .b.ap_t6_awk_en = 1,
    hwp_idleLps->ap_awk_en_set = ap_awk_en.v;

    ap_awk_en1.b.ap_t7_awk_en = 1;
    ap_awk_en1.b.ap_t8_awk_en = 1;
    ap_awk_en1.b.ap_t9_awk_en = 1;
    hwp_idleLps->ap_awk_en1_set = ap_awk_en1.v;

    //REGT_FIELD_CHANGE(hwp_idleLps->ap_inten, REG_IDLE_LPS_AP_INTEN_T, ap_sys_awk_irq_to_ap_en, 1);
    ap_sys_awk_irq_to_ap_en_cfg.v = hwp_idleLps->ap_inten;
    ap_sys_awk_irq_to_ap_en_cfg.b.ap_sys_awk_irq_to_ap_en = 1;
    hwp_idleLps->ap_inten = ap_sys_awk_irq_to_ap_en_cfg.v;
    // hwp_idleTimer->slow_sys_clk_sel_hwen = 1;
    // hwp_idleTimer->slow_clk_sel_hwen = 1;
}



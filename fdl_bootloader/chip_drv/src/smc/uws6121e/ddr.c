#if 0
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
#endif

#include "sci_types.h"
#include "global_macros.h"
//----------------- w217 Code -----------------------------------------------------

#include "pwrctrl.h"            //w217
#include "pub_apb.h"            //w217
#include "lps_apb.h"            //w217
#include "dmc400.h"             //w217
#include "sys_ctrl_6121e.h"     //w217
#include "timer_ap.h"           //w217
#include "psram_phy.h"          //w217
#include "ap_apb.h"             //w217
#include "idle_lps.h"           //w217

#define MR0 (0)
#define MR1 (1)
#define MR2 (2)
#define MR3 (3)
#define MR4 (4)
#define MR5 (5)
#define MR6 (6)
#define MR7 (7)
#define MR8 (8)
#define DMC_MR_WRITE_CMD(mr, val) (DMC400_DIRECT_CMD_MRS | ((val) << 8) | (mr))
#define DMC_MR_READ_CMD(mr, val) (DMC400_DIRECT_CMD_MRR | (mr))

#define QOS_AON 1
#define QOS_AP_AXIDMA 1
#define QOS_EMMC 1
#define QOS_LZMA 1
#define QOS_APA5 2
#define QOS_CE 2
#define QOS_USB 2
#define QOS_MED 2
#define QOS_CPIFC 3
#define QOS_APIFC 3
#define QOS_GOUDA 3
#define QOS_GNSS 4
#define QOS_CP_AXIDMA 5
#define QOS_F8 6
#define QOS_CPA5 7
#define WRITE_PRIO 3
#define READ_FILL_PRIO 3


#define __APS_64__
//#define CONFIG_USE_PSRAM
//#define CONFIG_PSRAM_LP_HALF_SLEEP

extern void rprintf(const char *fmt, ...);
extern void Time_DelayUS(unsigned int us);

#if 0
static void prvDelayUS(uint32_t us)
{
    uint32_t start;
	uint32_t ticks;
	start = hwp_timer2->hwtimer_curval_l;
    ticks = us*2;  //timer freq = 2000000HZ  0.5us + 1
    while(!(((unsigned)((hwp_timer2->hwtimer_curval_l) - start)) > ticks))
    {
	}
}
#endif

static void pll_cpst0(void)
{
    unsigned int rdata;
    unsigned int locked = 0;
    unsigned int dll_cnt = 0;
    int i = 0;
    int m;

    hwp_psramPhy->psram_drf_train_cfg = 0x1;
    hwp_psramPhy->psram_drf_cfg_dll_mode_f0 = 0x2;
    hwp_psramPhy->psram_rf_cfg_dll_ads0 = PSRAM_PHY_RF_DLL_LOCK_WAIT_ADS0(10) | PSRAM_PHY_RF_DLL_PD_CNT_ADS0(5) | PSRAM_PHY_RF_DL_CPST_THR_ADS0(8) | PSRAM_PHY_RF_DLL_EN_ADS0 | PSRAM_PHY_RF_DL_CPST_AUTO_REF_EN_ADS0 | PSRAM_PHY_RF_DL_CPST_EN_ADS0;
    hwp_psramPhy->psram_rf_cfg_dll_ads1 = 0x400;
    while ((locked == 0) && (dll_cnt < 0xff) && (i < 1000))
    {
        for (m = 0; m < 20; m++)
        {
             __asm
		    {
		        nop
		    }
        }
        rdata = hwp_psramPhy->psram_rfdll_status_dll_ads0;
        locked = rdata & PSRAM_PHY_RFDLL_LOCKED_ADS0;
        dll_cnt = rdata & PSRAM_PHY_RFDLL_CNT_ADS0(0xff);
        i++;
    }
    if (locked == PSRAM_PHY_RFDLL_LOCKED_ADS0)
    {
        //rprintf("PSRAM adslice0 DLL locked!" );
    }
    else if (dll_cnt == 0xff)
    {
        //rprintf("PSRAM adslice0 DLL saturated!" );
    }
    else
    {
        //rprintf("PSRAM adslice0 DLL saturated!" );
    }
    Time_DelayUS(5);
    hwp_psramPhy->psram_rf_cfg_dll_ads0 = 0x0;
}

static void pll_cpst1(void)
{
    unsigned int rdata;
    unsigned int locked = 0;
    unsigned int dll_cnt = 0;
    int i = 0;
    int m;

    hwp_psramPhy->psram_drf_train_cfg = 0x1;
    hwp_psramPhy->psram_drf_cfg_dll_mode_f0 = 0x2;
    hwp_psramPhy->psram_rf_cfg_dll_ads1 = PSRAM_PHY_RF_DLL_LOCK_WAIT_ADS1(10) | PSRAM_PHY_RF_DLL_PD_CNT_ADS1(5) | PSRAM_PHY_RF_DL_CPST_THR_ADS1(8) | PSRAM_PHY_RF_DLL_EN_ADS1 | PSRAM_PHY_RF_DL_CPST_AUTO_REF_EN_ADS1 | PSRAM_PHY_RF_DL_CPST_EN_ADS1;
    while ((locked == 0) && (dll_cnt < 0xff) && (i < 1000))
    {
        for (m = 0; m < 20; m++)
        {
            __asm
		    {
		        nop
		    }
        }
        rdata = hwp_psramPhy->psram_rfdll_status_dll_ads1;
        locked = rdata & PSRAM_PHY_RFDLL_LOCKED_ADS1;
        dll_cnt = rdata & PSRAM_PHY_RFDLL_CNT_ADS1(0xff);
        i++;
    }
    if (locked == PSRAM_PHY_RFDLL_LOCKED_ADS1)
    {
        //rprintf("PSRAM adslice1 DLL locked!" );
    }
    else if (dll_cnt == 0xff)
    {
        //rprintf("PSRAM adslice1 DLL saturated!" );
    }
    else
    {
        //rprintf("PSRAM adslice1 DLL saturated!" );
    }
    Time_DelayUS(5);
    hwp_psramPhy->psram_rf_cfg_dll_ads1 = 0x0;
}


/////aps 64m/32m////////////////////
static void prvPsramPhyConfig(void)
{
    //rprintf("START APS32  PHY INIT" );
    hwp_psramPhy->psram_rf_cfg_phy = 0x1; //rf_phy_en=1,rf_phy_init_complete.
    hwp_psramPhy->psram_drf_cfg = 0x1;    //select clk phase
    //ads0
    //#if definedCONFIG_POST_SIM
    //hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads0=0x80000008;
    //#else
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_rf_cfg_psram_type = 0x42;
#endif
    hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads0 = 0x80000008;
    //#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = 0x80080008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = 0x83190008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = 0x00000002;
#endif
#if defined(__APS_32__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = 0x80050008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = 0x831f0008;
    //#endif
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = 0x00000000;
#endif
#if defined(__APS_64__)

    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = 0x80000008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = 0x80000008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = 0x00000000; //0527

#endif
    //#if definedCONFIG_POST_SIM
    //hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0=0x83060008;
    //#else
    hwp_psramPhy->psram_rf_cfg_dll_dl_4_wr_ads0 = 0x1c08;
    hwp_psramPhy->psram_rf_cfg_dll_dl_5_wr_ads0 = 0x1409180a;
    hwp_psramPhy->psram_rf_cfg_dll_dl_6_wr_ads0 = 0x9001a;
    hwp_psramPhy->psram_rf_cfg_dll_dl_7_wr_ads0 = 0xa090a13;
    hwp_psramPhy->psram_rf_cfg_dll_dl_8_wr_ads0 = 0x61e1e11;
#if defined(__APS_32__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads0 = 0x1f0d0a;
#endif
#if defined(__APS_64__) || defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads0 = 0xb0c0a;
#endif
    //ads1
    //#if definedCONFIG_POST_SIM
    //hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads1=0x80000008;
    //#else
    hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads1 = 0x80000008;
    //#endif
#if defined(__APS_32__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads1 = 0x80080008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads1 = 0x801b0008;
#endif
#if defined(__APS_64__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads1 = 0x80000008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads1 = 0x80000008;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads1 = 0x80170008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads1 = 0x831f0008;
#endif
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads1 = 0x00000000;
    hwp_psramPhy->psram_rf_cfg_dll_dl_4_wr_ads1 = 0x1;
    hwp_psramPhy->psram_rf_cfg_dll_dl_5_wr_ads1 = 0x140b0e02;
    hwp_psramPhy->psram_rf_cfg_dll_dl_6_wr_ads1 = 0xd0c0901;
#if defined(__APS_32__) || defined(__aps__64__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_7_wr_ads1 = 0x13161514;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_7_wr_ads1 = 0xa090a13;
#endif
    hwp_psramPhy->psram_rf_cfg_dll_dl_8_wr_ads1 = 0x8171c13;
    hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads1 = 0x1f0d0a;
    //shift_sel16
    hwp_psramPhy->psram_drf_cfg_reg_sel = 0x0;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f0 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f1 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f2 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f3 = 0x8;

    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f0 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f1 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f2 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f3 = 0x8;

    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f0 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f1 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f2 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f3 = 0x10;

    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f0 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f1 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f2 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f3 = 0x10;

    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f0 = 0x2;
    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f1 = 0x2;
    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f2 = 0x2;
    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f3 = 0x2;

    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f0 = 0x1;
    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f1 = 0x1;
    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f2 = 0x1;
    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f3 = 0x1;

    hwp_psramPhy->psram_drf_format_control = 0x1;
    hwp_psramPhy->psram_drf_t_rcd = 0x6;
#if defined(__APS_128__) || defined(__APS_256__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_drf_t_phywrlat = 0x0;
#endif
    hwp_psramPhy->psram_drf_t_rddata_en = 0x6;
#if defined(__APS_32__) || defined(__APS_64__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_drf_t_cph_rd = 0x2;
#endif
    hwp_psramPhy->psram_drf_t_rddata_late = 0x2;
    hwp_psramPhy->psram_drf_t_rddata_valid_early = 0x3;
#if defined(__APS_32__) || defined(__APS_64__) || defined(__WB955__)
    hwp_psramPhy->psram_drf_t_cph_wr = 0x61e6;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_drf_t_cph_wr = 0x6;
    hwp_psramPhy->psram_drf_t_cph_rd = 0x5;
#endif
    hwp_psramPhy->psram_drf_t_data_oe_ext = 0x11;
    hwp_psramPhy->psram_drf_t_dqs_oe_ext = 0x1;
    hwp_psramPhy->psram_drf_t_xphs = 0xc;
    hwp_psramPhy->psram_rf_cfg_clock_gate = 0x1f;

#if definedCONFIG_POST_SIM
    hwp_psramPhy->io_rf_psram_drv_cfg = 0xa0;
#endif
    hwp_psramPhy->io_rf_psram_drv_cfg = 0x1009;
    hwp_psramPhy->psram_rf_cfg_phy = 0x3; //rf_phy_en=1,rf_phy_init_complete.
}


static void prvPsramPor(void)
{
    //rprintf("START APS32 POR INIT" );
    //*(volatile unsigned int*) REG_PWRCTRL_PSRAM_HOLD_CTRL = 0;
    hwp_pwrctrl->psram_hold_ctrl = 0;
    //TODO:t_PU>=150us
    //prvDelayUS(15000);//according to tb define the delay function
#if defined(__APS_32__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_drf_t_cph_rd = 0x6;
#endif
    Time_DelayUS(5);                 //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    hwp_dmcCtrl->direct_cmd = 0x1000003f; //no
    //TODO t_RST>2us
    Time_DelayUS(5);
#if defined(__APS_32__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    Time_DelayUS(5);                 //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    hwp_dmcCtrl->direct_cmd = 0x1000003f;
    //TODO t_RST>2us
    Time_DelayUS(5);
    hwp_psramPhy->psram_drf_t_cph_rd = 0x2;
    Time_DelayUS(5);
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10001000;
    Time_DelayUS(5);
#endif
#if defined(__APS_32__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_dmcCtrl->direct_cmd = 0x10001900;
#endif
#if defined(__APS_64__)
    hwp_dmcCtrl->direct_cmd = 0x10001a00;
#endif
#if defined(__APS_32__) || defined(__APS_64__)
    Time_DelayUS(5);
    hwp_dmcCtrl->direct_cmd = 0x10008004;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10002004;
    Time_DelayUS(10);
    hwp_dmcCtrl->direct_cmd = 0x10004708;
#endif
    Time_DelayUS(5);
    //*(volatile unsigned int*) REG_PWRCTRL_PSRAM_HOLD_CTRL = 1;
    hwp_pwrctrl->psram_hold_ctrl = 1; //no
}


static void prvPsramDmc400Config(void)
{
    //rprintf("START APS32  DMC INIT" );
    hwp_dmcCtrl->format_control = 0x11000101;
    hwp_dmcCtrl->address_control = 0x20201;
    hwp_dmcCtrl->decode_control = 0x20;
    hwp_dmcCtrl->t_refi = 0x00000208;
    hwp_dmcCtrl->t_rfc = 0x00040004; // TODO
    hwp_dmcCtrl->t_mrr = 0x00000002;
    hwp_dmcCtrl->t_mrw = 0x00000005;
    hwp_dmcCtrl->t_rcd = 0x00000006;
    hwp_dmcCtrl->t_ras = 0x00000011;
    hwp_dmcCtrl->t_rp = 0x00000004;
    hwp_dmcCtrl->t_rpall = 0x00000004;
    hwp_dmcCtrl->t_rrd = 0x00000004;
    hwp_dmcCtrl->t_faw = 0x00000014;
    hwp_dmcCtrl->read_latency = 0x0000000e;  // TODO
    hwp_dmcCtrl->t_rtr = 0x00000006;         // TODO
    hwp_dmcCtrl->t_rtw = 0x0000000c;         // TODO
    hwp_dmcCtrl->t_rtp = 0x00000008;         // TODO
    hwp_dmcCtrl->write_latency = 0x0000000a; // TODO
    hwp_dmcCtrl->t_wr = 0x0000000c;
    hwp_dmcCtrl->t_wtr = 0x00090009; //initial version
    //hwp_dmcCtrl->t_wtr                          =0x00090012;
    hwp_dmcCtrl->t_wtw = 0x000c000c;
    hwp_dmcCtrl->t_eckd = 0x0000000b; // TODO
    hwp_dmcCtrl->t_xckd = 0x0000000b; // TODO
    hwp_dmcCtrl->t_ep = 0x00000002;   // TODO
    hwp_dmcCtrl->t_xp = 0x00030003;
    hwp_dmcCtrl->t_esr = 0x00000002; // TODO
    hwp_dmcCtrl->t_xsr = 0x00040004;
    hwp_dmcCtrl->t_srckd = 0x0000000b;     // TODO
    hwp_dmcCtrl->t_cksrd = 0x0000000b;     // TODO
    hwp_dmcCtrl->t_rddata_en = 0x00000006; // RL=6
    //hwp_dmcCtrl->t_rddata_en                    =0x00000007; // RL=6
#if defined(__APS_32__) || defined(__APS_64__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_dmcCtrl->t_phywrlat = 0x00000103; // tDQSS=1(0.75-1.25) + WL=3
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->t_phywrlat = 0x00000108; // tDQSS=1(0.75-1.25) + WL=3
#endif
        // DMC400 : 0x8 - memc_cmd
        // 2:0 - memc_cmd.
        //       0b000 : CONFIGURE
        //       0b001 : SLEEP
        //       0b010 : PAUSE
        //       0b011 : GO
        //       0b100 : INVALIDATE
    // hwp_dmcCtrl->memc_cmd = 0x3; // GO
    Time_DelayUS(5);
}


static void prvDmc400QosConfig(void)
{
    REG_AP_APB_CFG_QOS0_T cfg_qos_apa5_ce_emmc_lzma;
	REG_AP_APB_CFG_QOS1_T cfg_gd_usb_ce_apifc_aon;
	REG_AP_APB_CFG_QOS2_T cfg_med_apaxidma;
	REG_SYS_CTRL_CFG_AON_QOS_T aon_qos;
	REG_DMC400_WRITE_PRIORITY_CONTROL_T write_priority_control;
	REG_DMC400_READ_PRIORITY_CONTROL_T read_priority_control;

	cfg_qos_apa5_ce_emmc_lzma.v = 0;
	cfg_qos_apa5_ce_emmc_lzma.b.ap_a5_arqos = QOS_APA5;
	cfg_qos_apa5_ce_emmc_lzma.b.ap_a5_awqos = QOS_APA5;
	cfg_qos_apa5_ce_emmc_lzma.b.ce_arqos = QOS_CE;
	cfg_qos_apa5_ce_emmc_lzma.b.ce_awqos = QOS_CE;
	cfg_qos_apa5_ce_emmc_lzma.b.emmc_arqos = QOS_EMMC;
	cfg_qos_apa5_ce_emmc_lzma.b.emmc_awqos = QOS_EMMC;
	cfg_qos_apa5_ce_emmc_lzma.b.lzma_arqos = QOS_LZMA;
	cfg_qos_apa5_ce_emmc_lzma.b.lzma_awqos = QOS_LZMA;
    hwp_apApb->cfg_qos0 = cfg_qos_apa5_ce_emmc_lzma.v;

	cfg_gd_usb_ce_apifc_aon.v = 0;
	cfg_gd_usb_ce_apifc_aon.b.gouda_arqos = QOS_GOUDA;
	cfg_gd_usb_ce_apifc_aon.b.gouda_awqos = QOS_GOUDA;
	cfg_gd_usb_ce_apifc_aon.b.usb_awqos = QOS_USB;
	cfg_gd_usb_ce_apifc_aon.b.usb_awqos = QOS_USB;
	cfg_gd_usb_ce_apifc_aon.b.ap_ifc_arqos = QOS_APIFC;
	cfg_gd_usb_ce_apifc_aon.b.ap_ifc_awqos = QOS_APIFC;
	hwp_apApb->cfg_qos1 = cfg_gd_usb_ce_apifc_aon.v;

	cfg_med_apaxidma.v = 0;
	cfg_med_apaxidma.b.med_arqos = QOS_MED;
	cfg_med_apaxidma.b.med_awqos = QOS_MED;
	cfg_med_apaxidma.b.ap_axidma_arqos = QOS_AP_AXIDMA;
	cfg_med_apaxidma.b.ap_axidma_awqos = QOS_AP_AXIDMA;
    hwp_apApb->cfg_qos2 = cfg_med_apaxidma.v;

#if 0
    REG_CP_GLB_SYSCTRL01_T cfg_cpa5_f8_cpaxidma = {
        .b.awqos_cp_a5 = QOS_CPA5,
        .b.arqos_cp_a5 = QOS_CPA5,
        .b.awqos_f8 = QOS_F8,
        .b.arqos_f8 = QOS_F8,
        .b.awqos_axidma = QOS_CP_AXIDMA,
        .b.arqos_axidma = QOS_CP_AXIDMA,
        .b.awqos_cp_ifc = QOS_CPIFC,
        .b.arqos_cp_ifc = QOS_CPIFC,
    };
    hwp_cpGlb->sysctrl01 = cfg_cpa5_f8_cpaxidma.v;
#endif

	aon_qos.v = 0;
	aon_qos.b.awqos_aon = QOS_AON;
	aon_qos.b.arqos_aon = QOS_AON;
	hwp_sysCtrl->cfg_aon_qos = aon_qos.v;


#if 0
    REG_GNSS_SYS_GNSS_AXI_QOS_T gnss_qos = {
        .b.gnss_arqos = QOS_GNSS,
        .b.gnss_awqos = QOS_GNSS,
    };
    hwp_gnssSys->gnss_axi_qos = gnss_qos.v;
#endif

    hwp_dmcCtrl->qos0_control = 0xf00;
    hwp_dmcCtrl->qos1_control = 0xe01;
    hwp_dmcCtrl->qos2_control = 0xd02;
    hwp_dmcCtrl->qos3_control = 0xc03;
    hwp_dmcCtrl->qos4_control = 0xb04;
    hwp_dmcCtrl->qos5_control = 0xa05;
    hwp_dmcCtrl->qos6_control = 0x906;
    hwp_dmcCtrl->qos7_control = 0x807;
    hwp_dmcCtrl->qos8_control = 0x708;
    hwp_dmcCtrl->qos9_control = 0x608;
    hwp_dmcCtrl->qos10_control = 0x50a;
    hwp_dmcCtrl->qos11_control = 0x40b;
    hwp_dmcCtrl->qos12_control = 0x30c;
    hwp_dmcCtrl->qos13_control = 0x20d;
    hwp_dmcCtrl->qos14_control = 0x10e;
    hwp_dmcCtrl->qos15_control = 0x00f;

	write_priority_control.v = 0;
	write_priority_control.b.write_threshold_en = 1;
	write_priority_control.b.write_fill_priority_1_16ths = 0;
	write_priority_control.b.write_fill_priority_2_16ths = 0;
	write_priority_control.b.write_fill_priority_3_16ths = 0;
	write_priority_control.b.write_fill_priority_4_16ths = 0;
	write_priority_control.b.write_fill_priority_5_16ths = 0;
	write_priority_control.b.write_fill_priority_6_16ths = 0;
	write_priority_control.b.write_fill_priority_7_16ths = 0;
	hwp_dmcCtrl->write_priority_control = write_priority_control.v;

	read_priority_control.v = 0;
	read_priority_control.b.read_escalation = 1;
	read_priority_control.b.read_in_burst_prioritisation = 1;
	read_priority_control.b.read_fill_priority_1_16ths = 0;
	read_priority_control.b.read_fill_priority_2_16ths = 0;
	read_priority_control.b.read_fill_priority_3_16ths = 0;
	read_priority_control.b.read_fill_priority_4_16ths = 0;
	read_priority_control.b.read_fill_priority_5_16ths = 0;
	read_priority_control.b.read_fill_priority_6_16ths = 0;
	read_priority_control.b.read_fill_priority_7_16ths = 0;
	hwp_dmcCtrl->read_priority_control = read_priority_control.v;
}


int ddr_init(void)
{

	REG_PWRCTRL_PUB_PWR_CTRL_T PWRCTRL_PUB_PWR_CTRL;

	PWRCTRL_PUB_PWR_CTRL.v = 0;
	PWRCTRL_PUB_PWR_CTRL.b.ap_pol = 1;
	hwp_pwrctrl->pub_pwr_ctrl_clr = PWRCTRL_PUB_PWR_CTRL.v;

	//wait stable
	while(!(hwp_pwrctrl->pub_pwr_stat == 3))
	{
		__asm
	    {
			nop
			nop
			nop
			nop
	    }
	}

#if defined(CONFIG_USE_PSRAM) && defined(CONFIG_PSRAM_LP_HALF_SLEEP)
    hwp_pubApb->slp_ctrl |= 0x80000;
    hwp_lpsApb->cfg_psram_half_slp |= 1;
#endif
    hwp_pubApb->cgm_psram = 0xa2;
    hwp_pubApb->slp_ctrl &= 0xFFFBF100;
    // DMC config for PSRAM
    hwp_pwrctrl->psram_hold_ctrl = 1;
    // hwp_dmcCtrl->memc_cmd = 0x0;
    if ((hwp_dmcCtrl->memc_status & 0x3) == 3)
    {
        hwp_dmcCtrl->memc_cmd = 0x0;
    }
    prvDmc400QosConfig();
    prvPsramDmc400Config();
    hwp_dmcCtrl->memc_cmd = 0x3;
    hwp_pwrctrl->psram_hold_ctrl = 0;
    // PSRAM PHY config
    prvPsramPhyConfig();
    hwp_psramPhy->psram_rf_cfg_phy = 0x0;
    pll_cpst0();
    Time_DelayUS(10);
    pll_cpst1();
    Time_DelayUS(10);
    hwp_psramPhy->psram_rf_cfg_phy = 0x3;
    hwp_pwrctrl->psram_hold_ctrl = 0;
    // PSRAM PAD config
    //psram_pad_config();
    hwp_dmcCtrl->memc_cmd = 0x0;
    // Power up PSRAM with global reset
    prvPsramPor(); //FOR APSRAM64
                   //psram_power_up_with_grst();
    hwp_dmcCtrl->memc_cmd = 0x3;
    //*(volatile unsigned int*) REG_PWRCTRL_PSRAM_HOLD_CTRL = 0;
    //rdata = *(volatile unsigned int*)0x5160300c;
    //*(volatile unsigned int*)0x5160300c = rdata | 0x40100; //for psram slp enable
    hwp_pwrctrl->psram_hold_ctrl = 0;
    hwp_pubApb->slp_ctrl |= 0x40F00;
	hwp_idleLps->lps_res8 = (hwp_idleLps->lps_res8 | 0x1);

	return 0;
}









#if 0
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

#endif


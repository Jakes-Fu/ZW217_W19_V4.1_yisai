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

/*
��ע:���ļ���δ������룬�Ⱥ���lowpower��Ҫ�õ�ʱ�ٵ���
*/

#include "sci_types.h"
#include "global_macros.h"
//----------------- w217 Code -----------------------------------------------------

#include "pwrctrl.h"  //w217
#include "pub_apb.h"  //w217
#include "lps_apb.h"  //w217
#include "dmc400.h"   //w217
#include "sys_ctrl.h" //w217
#include "timer_ap.h" //w217
#include "psram_phy.h" //w217
#include "ap_apb.h" //w217
#include "idle_lps.h" //w217

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

#pragma arm section code = "DDR_INIT_AONRAM"
static void prvDelayUS(unsigned int us)
{
    unsigned int start;
	unsigned int ticks;
	start = hwp_timer2->hwtimer_curval_l;
    ticks = us*2;  //timer freq = 2000000HZ  0.5us + 1
    while(!(((unsigned)((hwp_timer2->hwtimer_curval_l) - start)) > ticks))
    {
	}
}

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
    prvDelayUS(5);
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
    prvDelayUS(5);
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

void prvPsramWarmPor(void)
{
    //rprintf("START APS32 POR INIT" );
    //*(volatile unsigned int*) REG_PWRCTRL_PSRAM_HOLD_CTRL = 0;
    hwp_pwrctrl->psram_hold_ctrl = 0;
    //TODO:t_PU>=150us
    //prvDelayUS(15000);//according to tb define the delay function
#if defined(__APS_32__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_drf_t_cph_rd = 0x6;
#endif
    prvDelayUS(5);                 //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    //hwp_dmcCtrl->direct_cmd = 0x60000000;
    //hwp_dmcCtrl->direct_cmd = 0x1000003f;//no
    //TODO t_RST>2us
    prvDelayUS(5);
#if defined(__APS_32__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    prvDelayUS(5);                 //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    hwp_dmcCtrl->direct_cmd = 0x1000003f;
    //TODO t_RST>2us
    prvDelayUS(5);
    hwp_psramPhy->psram_drf_t_cph_rd = 0x2;
    prvDelayUS(5);
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10001000;
    prvDelayUS(5);
#endif
#if defined(__APS_32__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_dmcCtrl->direct_cmd = 0x10001900;
#endif

#if defined(__APS_32__) || defined(__APS_64__)
    prvDelayUS(5);
    hwp_dmcCtrl->direct_cmd = 0x10008004;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10002004;
    prvDelayUS(10);
    hwp_dmcCtrl->direct_cmd = 0x10004708;
#endif
    prvDelayUS(5);
    //*(volatile unsigned int*) REG_PWRCTRL_PSRAM_HOLD_CTRL = 1;
    hwp_pwrctrl->psram_hold_ctrl = 1; //no
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
    prvDelayUS(5);                 //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    hwp_dmcCtrl->direct_cmd = 0x1000003f; //no
    //TODO t_RST>2us
    prvDelayUS(5);
#if defined(__APS_32__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    prvDelayUS(5);                 //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    hwp_dmcCtrl->direct_cmd = 0x1000003f;
    //TODO t_RST>2us
    prvDelayUS(5);
    hwp_psramPhy->psram_drf_t_cph_rd = 0x2;
    prvDelayUS(5);
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10001000;
    prvDelayUS(5);
#endif
#if defined(__APS_32__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_dmcCtrl->direct_cmd = 0x10001900;
#endif
#if defined(__APS_64__)
    hwp_dmcCtrl->direct_cmd = 0x10001a00;
#endif
#if defined(__APS_32__) || defined(__APS_64__)
    prvDelayUS(5);
    hwp_dmcCtrl->direct_cmd = 0x10008004;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10002004;
    prvDelayUS(10);
    hwp_dmcCtrl->direct_cmd = 0x10004708;
#endif
    prvDelayUS(5);
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
    prvDelayUS(5);
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

void ddr_init(void)
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
    prvDelayUS(10);
    pll_cpst1();
    prvDelayUS(10);
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
}

void prvPsramWarmInit(void)
{
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
    prvDelayUS(10);
    pll_cpst1();
    prvDelayUS(10);
    hwp_psramPhy->psram_rf_cfg_phy = 0x3;
    hwp_pwrctrl->psram_hold_ctrl = 0;
    // PSRAM PAD config
    //psram_pad_config();

#ifdef CONFIG_PSRAM_LP_HALF_SLEEP
    hwp_psramPhy->psram_rf_cfg_phy_iomux_oe_wr_ads0 = 0x00110000;  // rf_phy_io_csn_oe_ads0=1,  rf_phy_io_clk_oe_ads0=1
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00100000; // rf_phy_io_csn_out_ads0=1, rf_phy_io_clk_out_ads0=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_sel_wr_ads0 = 0x00110000; // rf_phy_io_csn_sel_ads0=1, rf_phy_io_clk_sel_ads0=1
    // select iomux for software ads1
    hwp_psramPhy->psram_rf_cfg_phy_iomux_oe_wr_ads1 = 0x00110000;  // rf_phy_io_csn_oe_ads1=1,  rf_phy_io_clk_oe_ads1=1
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00100000; // rf_phy_io_csn_out_ads1=1, rf_phy_io_clk_out_ads1=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_sel_wr_ads1 = 0x00110000; // rf_phy_io_csn_sel_ads1=1, rf_phy_io_clk_sel_ads1=1
    // gate clock
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00100000; // rf_phy_io_csn_out_ads0=1, rf_phy_io_clk_out_ads0=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00100000; // rf_phy_io_csn_out_ads1=1, rf_phy_io_clk_out_ads1=0
    // exit Half Sleep mode
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00000000; // rf_phy_io_csn_out_ads0=0, rf_phy_io_clk_out_ads0=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00000000; // rf_phy_io_csn_out_ads1=0, rf_phy_io_clk_out_ads1=0
    prvDelayUS(1);                                                 //weak up ce low hold time (60-2000ns)
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00100000; // rf_phy_io_csn_out_ads0=1, rf_phy_io_clk_out_ads0=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00100000; // rf_phy_io_csn_out_ads1=1, rf_phy_io_clk_out_ads1=0

    // release clock
    prvDelayUS(150);                                               //(ce low to first posedge clk > 150us)
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00100000; // rf_phy_io_csn_out_ads0=1, rf_phy_io_clk_out_ads0=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00100000; // rf_phy_io_csn_out_ads1=1, rf_phy_io_clk_out_ads1=0

    // select iomux for hardware
    hwp_psramPhy->psram_rf_cfg_phy_iomux_sel_wr_ads0 = 0x00000000; // rf_phy_io_csn_sel_ads010, rf_phy_io_clk_sel_ads010
    hwp_psramPhy->psram_rf_cfg_phy_iomux_oe_wr_ads0 = 0x00000000;  // rf_phy_io_csn_oe_ads010,  rf_phy_io_clk_oe_ads010
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00000000; // rf_phy_io_csn_out_ads010, rf_phy_io_clk_out_ads010
    hwp_psramPhy->psram_rf_cfg_phy_iomux_sel_wr_ads1 = 0x00000000; // rf_phy_io_csn_sel_ads010, rf_phy_io_clk_sel_ads010
    hwp_psramPhy->psram_rf_cfg_phy_iomux_oe_wr_ads1 = 0x00000000;  // rf_phy_io_csn_oe_ads010,  rf_phy_io_clk_oe_ads010
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00000000; // rf_phy_io_csn_out_ads010, rf_phy_io_clk_out_ads010
                                                                   //#500us;

    if ((hwp_dmcCtrl->memc_status & 0x3) == 3)
    {
        hwp_dmcCtrl->memc_cmd = 0x0;
    }
    // Power up PSRAM with global reset
    prvPsramWarmPor(); //FOR APSRAM64
    //psram_power_up_with_grst();
    hwp_dmcCtrl->memc_cmd = 0x3;

    prvDelayUS(150);
    //*(volatile unsigned int*) REG_PWRCTRL_PSRAM_HOLD_CTRL = 0;
    //rdata = *(volatile unsigned int*)0x5160300c;
    //*(volatile unsigned int*)0x5160300c = rdata | 0x40100; //for psram slp enable
    hwp_pwrctrl->psram_hold_ctrl = 0;
#endif
    hwp_pubApb->slp_ctrl |= 0x40F00;
}

#pragma arm section code


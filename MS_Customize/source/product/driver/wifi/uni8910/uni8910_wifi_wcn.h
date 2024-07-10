/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _WCN_WLAN_H_
#define _WCN_WLAN_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'wcn_wlan'."
#endif

#include "sci_types.h"
#include "sci_api.h"
#include "sci_log.h"
#include "os_api.h"
// =============================================================================
//  MACROS
// =============================================================================
//#define hwp_wcnRfIf  hwp_rf_if



//typedef unsigned char bool;

typedef volatile unsigned int REG32;

#define true 1
#define false 0

#ifndef WIN32
#ifdef MIPSGCC
#define KSEG0(addr)     ( (addr) | 0x80000000 )
#define KSEG1(addr)     ( (addr) | 0xA0000000 )
#else
#define KSEG0(addr)     (addr)
#define KSEG1(addr)     (addr)
#endif
#define REG_ACCESS_ADDRESS(addr)    KSEG1(addr)
#else
extern void* KSEG0(int addr);
extern void* KSEG1(int addr);
#endif
// ============================================================================
// WCN_BT_MODEM_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef volatile struct
{
    REG32  bypass_ctrl;            //0x00000000
    REG32 sw_swap_dccal;          //0x00000004
    REG32 dem750_afc_freq;        //0x00000008
    REG32 dpsk_gfsk_tx_ctrl;      //0x0000000C
    REG32 tx_gain_ctrl;           //0x00000010
    REG32 rssi_out1;              //0x00000014
    REG32 sw_ctrl;                //0x00000018
    REG32 adcclk_sw_ctrl;         //0x0000001C
    REG32 dpsk_demod_sw;          //0x00000020
    REG32 min_phase_err;          //0x00000024
    REG32 afc_ctrl;               //0x00000028
    REG32 dpsk_gfsk_smp_th;       //0x0000002C
    REG32 dpsk_gfsk_smp_th_1;     //0x00000030
    REG32 gfsk_smp_ref_ctrl;      //0x00000034
    REG32 gfsk_smp_ref_ctrl_1;    //0x00000038
    REG32 rateconv_ctrl1;         //0x0000003C
    REG32 rateconv_ctrl2;         //0x00000040
    REG32 demod_smp_ctrl;         //0x00000044
    REG32 agc_ctrl;               //0x00000048
    REG32 agc_th_ctrl1;           //0x0000004C
    REG32 agc_th_ctrl2;           //0x00000050
    REG32 agc_ctrl1;              //0x00000054
    REG32 agc_ctrl2;              //0x00000058
    REG32 agc_dgc_ctrl;           //0x0000005C
    REG32 agc_dccal_ctrl;         //0x00000060
    REG32 rx_dccal_ctrl;          //0x00000064
    REG32 rx_dcofst_inuse;        //0x00000068
    REG32 tx_dc_calib;            //0x0000006C
    REG32 tx_fix_i_dcofst;        //0x00000070
    REG32 tx_fix_q_dcofst;        //0x00000074
    REG32 tx_i_dcofst_inuse;      //0x00000078
    REG32 tx_q_dcofst_inuse;      //0x0000007C
    REG32 rssi_gain_ctrl1;        //0x00000080
    REG32 rssi_gain_ctrl2;        //0x00000084
    REG32 rssi_gain_ctrl3;        //0x00000088
    REG32 rssi_gain_ctrl4;        //0x0000008C
    REG32 rssi_gain_ctrl5;        //0x00000090
    REG32 rssi_gain_ctrl6;        //0x00000094
    REG32 rssi_gain_ctrl7;        //0x00000098
    REG32 rssi_gain_ctrl8;        //0x0000009C
    REG32 modem_tpd_sel;          //0x000000A0
    REG32 demod_smp_th_ctrl;      //0x000000A4
    REG32 newpacket_byte4;        //0x000000A8
    REG32 newpacket_byte3;        //0x000000AC
    REG32 newpacket_byte2;        //0x000000B0
    REG32 newpacket_byte1;        //0x000000B4
    REG32 gfsk_mod_ctrl;          //0x000000B8
    REG32 gfsk_mod_ref_ctrl;      //0x000000BC
    REG32 sym_dly_ctrl;           //0x000000C0
    REG32 rssi_out2;              //0x000000C4
    REG32 trx_clk_ctrl;           //0x000000C8
    REG32 iq_swap_ctrl;           //0x000000CC
    REG32 gfsk_sync_ctrl;         //0x000000D0
    REG32 gfsk_demod_ctrl;        //0x000000D4
    REG32 gfsk_mod_idx;           //0x000000D8
    REG32 dpsk_gfsk_misc_ctrl;    //0x000000DC
    REG32 modem_dbm_sel;          //0x000000E0
    REG32 gfsk_mod_idx_le;        //0x000000E4
    REG32 newpacket_byte4_inuse;  //0x000000E8
    REG32 newpacket_byte3_inuse;  //0x000000EC
    REG32 newpacket_byte2_inuse;  //0x000000F0
    REG32 newpacket_byte1_inuse;  //0x000000F4
    REG32 le_mode_ctrl1;          //0x000000F8
    REG32 le_mode_ctrl2;          //0x000000FC
    REG32 le_mode_ctrl3;          //0x00000100
    REG32 le_mode_ctrl4;          //0x00000104
    REG32 le_mode_ctrl5;          //0x00000108
    REG32 rf_ctrl;                //0x0000010C
    REG32 tx_q_im;                //0x00000110
    REG32 tx_i_im;                //0x00000114
    REG32 tx_am_p0;               //0x00000118
    REG32 tx_am_p1;               //0x0000011C
    REG32 tx_am_p2;               //0x00000120
    REG32 tx_am_p3;               //0x00000124
    REG32 tx_am_p4;               //0x00000128
    REG32 tx_am_p5;               //0x0000012C
    REG32 tx_am_p6;               //0x00000130
    REG32 tx_am_p7;               //0x00000134
    REG32 tx_am_p8;               //0x00000138
    REG32 tx_am_p9;               //0x0000013C
    REG32 tx_am_p10;              //0x00000140
    REG32 tx_am_p11;              //0x00000144
    REG32 tx_am_p12;              //0x00000148
    REG32 tx_am_p13;              //0x0000014C
    REG32 tx_am_p14;              //0x00000150
    REG32 tx_am_p15;              //0x00000154
    REG32 tx_am_p16;              //0x00000158
    REG32 tx_pm_p0;               //0x0000015C
    REG32 tx_pm_p1;               //0x00000160
    REG32 tx_pm_p2;               //0x00000164
    REG32 tx_pm_p3;               //0x00000168
    REG32 tx_pm_p4;               //0x0000016C
    REG32 tx_pm_p5;               //0x00000170
    REG32 tx_pm_p6;               //0x00000174
    REG32 tx_pm_p7;               //0x00000178
    REG32 tx_pm_p8;               //0x0000017C
    REG32 tx_pm_p9;               //0x00000180
    REG32 tx_pm_p10;              //0x00000184
    REG32 tx_pm_p11;              //0x00000188
    REG32 tx_pm_p12;              //0x0000018C
    REG32 tx_pm_p13;              //0x00000190
    REG32 tx_pm_p14;              //0x00000194
    REG32 tx_pm_p15;              //0x00000198
    REG32 tx_pm_p16;              //0x0000019C
    REG32 notch_coef;             //0x000001A0
    REG32 adapt_edr3_demod;       //0x000001A4
    REG32 adapt_edr3_thresh;      //0x000001A8
    REG32 tx_auto_gain1_gfsk;     //0x000001AC
    REG32 tx_auto_gain0_gfsk;     //0x000001B0
    REG32 tx_auto_gain1_gfsk_edr; //0x000001B4
    REG32 tx_auto_gain0_gfsk_edr; //0x000001B8
    REG32 tx_auto_gain1_dpsk;     //0x000001BC
    REG32 tx_auto_gain0_dpsk;     //0x000001C0
    REG32 gfsk_mod_eql_gain;      //0x000001C4
    REG32 tx_lfp_delay_ctrl;      //0x000001C8
    REG32 tx_hfp_delay;           //0x000001CC
    REG32 tx_polar_mode_ctl;      //0x000001D0
    REG32 tx_apf_num_b1;          //0x000001D4
    REG32 tx_apf_num_b2;          //0x000001D8
    REG32 tx_apf_num_b3;          //0x000001DC
    REG32 tx_apf_num_b4;          //0x000001E0
    REG32 tx_apf_den_a2;          //0x000001E4
    REG32 tx_apf_den_a3;          //0x000001E8
    REG32 tx_apf_den_a4;          //0x000001EC
    REG32 adapt_edr3_cci;         //0x000001F0
} HWP_WCN_BT_MODEM_T;
#define REG_WCN_MODEM_BASE 0x15006000
#define hwp_wcnModem ((HWP_WCN_BT_MODEM_T *)REG_ACCESS_ADDRESS(REG_WCN_MODEM_BASE))


typedef volatile struct
{
    REG32 rwbtcnt;               //0x00000000
    REG32 version;               //0x00000004
    REG32 rwbtconf;              //0x00000008
    REG32 intcntl;               //0x0000000C
    REG32 intstat;               //0x00000010
    REG32 intrawstat;            //0x00000014
    REG32 intack;                //0x00000018
    REG32 slotclk;               //0x0000001C
    REG32 finetimecnt;           //0x00000020
    REG32 abtraincnt;            //0x00000024
    REG32 edrcntl_nwinsize;      //0x00000028
    REG32 et_currentrxdescptr;   //0x0000002C
    REG32 deepslcntl;            //0x00000030
    REG32 deepslwkup;            //0x00000034
    REG32 deepslstat;            //0x00000038
    REG32 enbpreset;             //0x0000003C
    REG32 finecntcorr;           //0x00000040
    REG32 clkncntcorr;           //0x00000044
    REG32 tokencntl;             //0x00000048
    REG32 Reserved_0000004C;     //0x0000004C
    REG32 diagcntl;              //0x00000050
    REG32 diagstat;              //0x00000054
    REG32 debugaddmax;           //0x00000058
    REG32 debugaddmin;           //0x0000005C
    REG32 errortypestat;         //0x00000060
    REG32 swprofiling;           //0x00000064
    REG32 Reserved_00000068[2];  //0x00000068
    REG32 radiocntl0;            //0x00000070
    REG32 radiocntl1;            //0x00000074
    REG32 radiocntl2;            //0x00000078
    REG32 radiocntl3;            //0x0000007C
    REG32 radiopwrupdn;          //0x00000080
    REG32 Reserved_00000084[3];  //0x00000084
    REG32 txmicval;              //0x00000090
    REG32 rxmicval;              //0x00000094
    REG32 e0ptr;                 //0x00000098
    REG32 Reserved_0000009C;     //0x0000009C
    REG32 rftestcntl;            //0x000000A0
    REG32 rftestfreq;            //0x000000A4
    REG32 Reserved_000000A8[2];  //0x000000A8
    REG32 timgencntl;            //0x000000B0
    REG32 grosstimtgt;           //0x000000B4
    REG32 finetimtgt;            //0x000000B8
    REG32 sketclkn;              //0x000000BC
    REG32 pcacntl0;              //0x000000C0
    REG32 pcacntl1;              //0x000000C4
    REG32 pcastat;               //0x000000C8
    REG32 Reserved_000000CC[29]; //0x000000CC
    REG32 escochancntl0;         //0x00000140
    REG32 escomutecntl0;         //0x00000144
    REG32 escocurrenttxptr0;     //0x00000148
    REG32 escocurrentrxptr0;     //0x0000014C
    REG32 escoltcntl0;           //0x00000150
    REG32 escotrcntl0;           //0x00000154
    REG32 escodaycnt0;           //0x00000158
    REG32 Reserved_0000015C;     //0x0000015C
    REG32 escochancntl1;         //0x00000160
    REG32 escomutecntl1;         //0x00000164
    REG32 escocurrenttxptr1;     //0x00000168
    REG32 escocurrentrxptr1;     //0x0000016C
    REG32 escoltcntl1;           //0x00000170
    REG32 escotrcntl1;           //0x00000174
    REG32 escodaycnt1;           //0x00000178
    REG32 Reserved_0000017C;     //0x0000017C
    REG32 escochancntl2;         //0x00000180
    REG32 escomutecntl2;         //0x00000184
    REG32 escocurrenttxptr2;     //0x00000188
    REG32 escocurrentrxptr2;     //0x0000018C
    REG32 escoltcntl2;           //0x00000190
    REG32 escotrcntl2;           //0x00000194
    REG32 escodaycnt2;           //0x00000198
    REG32 Reserved_0000019C;     //0x0000019C
    REG32 audiocntl0;            //0x000001A0
    REG32 audiocntl1;            //0x000001A4
    REG32 audiocntl2;            //0x000001A8
    REG32 Reserved_000001AC;     //0x000001AC
    REG32 pcmgencntl;            //0x000001B0
    REG32 pcmphyscntl0;          //0x000001B4
    REG32 pcmphyscntl1;          //0x000001B8
    REG32 pcmpadding;            //0x000001BC
    REG32 pcmpllcntl0;           //0x000001C0
    REG32 pcmpllcntl1;           //0x000001C4
    REG32 pcmpllcntl2;           //0x000001C8
    REG32 pcmsourceptr;          //0x000001CC
    REG32 pcmsinkptr;            //0x000001D0
    REG32 Reserved_000001D4[3];  //0x000001D4
    REG32 bredrprioscharb;       //0x000001E0
    REG32 plcaddr01;             //0x000001E4
    REG32 plcaddr23;             //0x000001E8
    REG32 plcaddr45;             //0x000001EC
    REG32 plcaddr67;             //0x000001F0
    REG32 plcaddr89;             //0x000001F4
    REG32 plcaddrb;              //0x000001F8
    REG32 plcconf;               //0x000001FC
} HWP_WCN_BT_LINK_T;
#define REG_WCN_BT_REG_BASE 0x14100000
#define hwp_wcnBtReg ((HWP_WCN_BT_LINK_T *)REG_ACCESS_ADDRESS(REG_WCN_BT_REG_BASE))

typedef volatile struct
{
    REG32 Reserved_00000000;     //0x00000000
    REG32 phy_reg_bank_addr;     //0x00000004
    REG32 phy_reg_offset_addr;   //0x00000008
    REG32 phy_reg_write_data;    //0x0000000C
    REG32 phy_reg_read_data;     //0x00000010
    REG32 protocol_version;      //0x00000014
    REG32 type;                  //0x00000018
    REG32 sub_type;              //0x0000001C
    REG32 desaddr_l;             //0x00000020
    REG32 desaddr_h;             //0x00000024
    REG32 srcaddr_l;             //0x00000028
    REG32 srcaddr_h;             //0x0000002C
    REG32 bssidaddr_l;           //0x00000030
    REG32 bssidaddr_h;           //0x00000034
    REG32 seqcontrol;            //0x00000038
    REG32 rssival;               //0x0000003C
    REG32 Reserved_00000040[16]; //0x00000040
    REG32 config_reg;            //0x00000080
    REG32 datardyint;            //0x00000084
    REG32 phy_sel_set0;          //0x00000088
    REG32 phy_sel_set2;          //0x0000008C
    REG32 phy_sel_set3;          //0x00000090
    REG32 phy_sel_set4;          //0x00000094
    REG32 phy_sel_set5;          //0x00000098
    REG32 phy_sel_set6;          //0x0000009C
    REG32 phy_sel_set7;          //0x000000A0
    REG32 phy_sel_set8;          //0x000000A4
} HWP_WCN_WLAN_T;
#define REG_WCN_WLAN_BASE 0x15007000
#define hwp_wcnWlan ((HWP_WCN_WLAN_T *)REG_ACCESS_ADDRESS(REG_WCN_WLAN_BASE))

typedef union {
    REG32 v;
    struct
    {
        REG32 wf_tune : 1;         // [0]
        REG32 wf_chn : 4;          // [4:1]
        REG32 wf_freq_mode : 1;    // [5]
        REG32 wf_freq_direct : 17; // [22:6]
        REG32 __31_23 : 9;
    } b;
} REG_WCN_RF_IF_WF_CONTROL_T;

//config_reg
typedef union {
    REG32 v;
    struct
    {
        REG32 apb_clear : 1; // [0]
        REG32 apb_hold : 1;  // [1]
        REG32 __7_2 : 6;
        REG32 subtype_config_reg : 4; // [11:8]
        REG32 type_config_reg : 2;    // [13:12]
        REG32 __15_14 : 2;
        REG32 phyrx_en_b : 1; // [16]
        REG32 __31_17 : 15;
    } b;
} REG_WCN_WLAN_CONFIG_REG_T;

//datardyint
typedef union {
    REG32 v;
    struct
    {
        REG32 datardyint : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_WCN_WLAN_DATARDYINT_T;


typedef volatile struct
{
    REG32                          CLKSEL;                       //0x00000000
    REG32                          CLKSEL_SET;                   //0x00000004
    REG32                          CLKSEL_CLR;                   //0x00000008
    REG32                          CLKDIV_ZSP;                   //0x0000000C
    REG32                          CLKDIV_LTE;                   //0x00000010
    REG32                          CLKEN_BB_SYSCTRL;             //0x00000014
    REG32                          CLKEN_BB_SYSCTRL_SET;         //0x00000018
    REG32                          CLKEN_BB_SYSCTRL_CLR;         //0x0000001C
    REG32                          CLKEN_ZSP;                    //0x00000020
    REG32                          CLKEN_ZSP_SET;                //0x00000024
    REG32                          CLKEN_ZSP_CLR;                //0x00000028
    REG32                          CLKEN_LTE;                    //0x0000002C
    REG32                          CLKEN_LTE_SET;                //0x00000030
    REG32                          CLKEN_LTE_CLR;                //0x00000034
    REG32                          CLKEN_ZSPCORE_MODE;           //0x00000038
    REG32                          CLKEN_ZSP_AXIDMA_MODE;        //0x0000003C
    REG32                          RSTCTRL_BB_SYSCTRL;           //0x00000040
    REG32                          RSTCTRL_BB_SYSCTRL_SET;       //0x00000044
    REG32                          RSTCTRL_BB_SYSCTRL_CLR;       //0x00000048
    REG32                          RSTCTRL_ZSP;                  //0x0000004C
    REG32                          RSTCTRL_ZSP_SET;              //0x00000050
    REG32                          RSTCTRL_ZSP_CLR;              //0x00000054
    REG32                          RSTCTRL_LTE;                  //0x00000058
    REG32                          RSTCTRL_LTE_SET;              //0x0000005C
    REG32                          RSTCTRL_LTE_CLR;              //0x00000060
    REG32                          ZSP_SOFT_RST;                 //0x00000064
    REG32                          LTE_SOFT_RST;                 //0x00000068
    REG32                          ZSP_AXILPCNT;                 //0x0000006C
    REG32                          ZSP_BUSLPMC;                  //0x00000070
    REG32                          ZSP_BUSLPMC_SET;              //0x00000074
    REG32                          ZSP_BUSLPMC_CLR;              //0x00000078
    REG32                          ZSP_BUSFORCELPMC;             //0x0000007C
    REG32                          ZSP_BUSFORCELPMC_SET;         //0x00000080
    REG32                          ZSP_BUSFORCELPMC_CLR;         //0x00000084
    REG32                          CLKEN_MAILBOX_MODE;           //0x00000088
} HWP_CP_CLKRST_T;

#if defined(REG_ADDRESS_FOR_GGE)
#define REG_CLKRST_BASE            0x05081000
#else
#define REG_CLKRST_BASE            0x50081000
#endif
#define hwp_clkrst                 ((HWP_CP_CLKRST_T*) REG_ACCESS_ADDRESS(REG_CLKRST_BASE))


typedef volatile struct
{
    REG32                          PWR_HWEN;                     //0x00000000
    REG32                          AP_PWR_CTRL;                  //0x00000004
    REG32                          ZSP_PWR_CTRL;                 //0x00000008
    REG32                          LTE_PWR_CTRL;                 //0x0000000C
    REG32                          GGE_PWR_CTRL;                 //0x00000010
    REG32                          RF_PWR_CTRL;                  //0x00000014
    REG32                          BTFM_PWR_CTRL;                //0x00000018
    REG32                          AON_LP_PWR_CTRL;              //0x0000001C
    REG32                          AP_PWR_STAT;                  //0x00000020
    REG32                          ZSP_PWR_STAT;                 //0x00000024
    REG32                          LTE_PWR_STAT;                 //0x00000028
    REG32                          GGE_PWR_STAT;                 //0x0000002C
    REG32                          RF_PWR_STAT;                  //0x00000030
    REG32                          BTFM_PWR_STAT;                //0x00000034
    REG32                          AON_LP_PWR_STAT;              //0x00000038
    REG32                          STATE_DELAY;                  //0x0000003C
    REG32                          PREPGC_DELAY;                 //0x00000040
    REG32                          ALLPGC_DELAY;                 //0x00000044
    REG32                          DDR_HOLD_CTRL;                //0x00000048
    REG32                          ZSP_PD_POLL;                  //0x0000004C
    REG32                          ZSP_PD_POLL_SET;              //0x00000050
    REG32                          ZSP_PD_POLL_CLR;              //0x00000054
    REG32 Reserved_00000058[14];                //0x00000058
    REG32                          WCN_LPS;                      //0x00000090
    REG32                          ARM_SLP_REQ_SW;               //0x00000094
    REG32                          ARM_SLP_ACK;                  //0x00000098
    REG32                          ARM_SLP_REQ_HWEN;             //0x0000009C
    REG32                          ZSP_SLP_REQ_SW;               //0x000000A0
    REG32                          ZSP_SLP_ACK;                  //0x000000A4
    REG32                          ZSP_SLP_REQ_HWEN;             //0x000000A8
    REG32                          DDR_SLP_REQ_SW;               //0x000000AC
    REG32                          DDR_SLP_ACK;                  //0x000000B0
    REG32                          DDR_SLP_REQ_HWEN;             //0x000000B4
    REG32                          TIMEOUT_FLAG;                 //0x000000B8
    REG32                          POWER_STATE;                  //0x000000BC
    REG32                          PWRCTRL_MODE;                 //0x000000C0
    REG32                          PWRCTRL_SW;                   //0x000000C4
    REG32                          PWRCTRL_SW_SET;               //0x000000C8
    REG32                          PWRCTRL_SW_CLR;               //0x000000CC
    REG32                          PWRCTRL_SW1;                  //0x000000D0
    REG32                          PWRCTRL_SW1_SET;              //0x000000D4
    REG32                          PWRCTRL_SW1_CLR;              //0x000000D8
} HWP_CP_PWRCTRL_T;
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_PWRCTRL_BASE           0x050A0000
#else
#define REG_PWRCTRL_BASE           0x500A0000
#endif
#define hwp_pwrctrl                ((HWP_CP_PWRCTRL_T*) REG_ACCESS_ADDRESS(REG_PWRCTRL_BASE))

#define CP_PWRCTRL_BTFM_STABLE     (1<<1)
#define SYS_CTRL_CFG_PLL_AHB_BTFM_DIV_UPDATE (1<<4)
#define CP_PWRCTRL_BTFM_PON        (1<<1)
#define CP_PWRCTRL_BTFM_POFF       (1<<0)
typedef volatile struct
{
    /// <strong>This register is used to Lock and Unlock the protected
    /// registers.</strong>
    REG32                          REG_DBG;                      //0x00000000
    /// This register is protected.
    REG32                          SYS_AXI_RST_SET;              //0x00000004
    REG32                          SYS_AXI_RST_CLR;              //0x00000008
    /// This register is protected.
    REG32                          SYS_AHB_RST_SET;              //0x0000000C
    REG32                          SYS_AHB_RST_CLR;              //0x00000010
    /// This register is protected.
    REG32                          AP_APB_RST_SET;               //0x00000014
    REG32                          AP_APB_RST_CLR;               //0x00000018
    /// This register is protected.
    REG32                          AIF_APB_RST_SET;              //0x0000001C
    REG32                          AIF_APB_RST_CLR;              //0x00000020
    /// This register is protected.
    REG32                          AON_AHB_RST_SET;              //0x00000024
    REG32                          AON_AHB_RST_CLR;              //0x00000028
    /// This register is protected.
    REG32                          AON_APB_RST_SET;              //0x0000002C
    REG32                          AON_APB_RST_CLR;              //0x00000030
    /// This register is protected.
    REG32                          RF_AHB_RST_SET;               //0x00000034
    REG32                          RF_AHB_RST_CLR;               //0x00000038
    /// This register is protected.
    REG32                          RF_APB_RST_SET;               //0x0000003C
    REG32                          RF_APB_RST_CLR;               //0x00000040
    /// This register is protected.
    REG32                          APCPU_RST_SET;                //0x00000044
    REG32                          APCPU_RST_CLR;                //0x00000048
    /// This register is protected.
    REG32                          CPCPU_RST_SET;                //0x0000004C
    REG32                          CPCPU_RST_CLR;                //0x00000050
    /// This register is protected.
    REG32                          BBLTE_RST_SET;                //0x00000054
    REG32                          BBLTE_RST_CLR;                //0x00000058
    /// This register is protected.
    REG32                          OTHERS_RST_SET;               //0x0000005C
    REG32                          OTHERS_RST_CLR;               //0x00000060
    REG32                          CLK_SYS_AXI_MODE;             //0x00000064
    REG32                          CLK_SYS_AXI_ENABLE;           //0x00000068
    /// This register is protected.
    REG32                          CLK_SYS_AXI_DISABLE;          //0x0000006C
    REG32                          CLK_SYS_AHB_MODE;             //0x00000070
    REG32                          CLK_SYS_AHB_ENABLE;           //0x00000074
    /// This register is protected.
    REG32                          CLK_SYS_AHB_DISABLE;          //0x00000078
    REG32                          CLK_AP_APB_MODE;              //0x0000007C
    REG32                          CLK_AP_APB_ENABLE;            //0x00000080
    /// This register is protected.
    REG32                          CLK_AP_APB_DISABLE;           //0x00000084
    REG32                          CLK_AIF_APB_MODE;             //0x00000088
    REG32                          CLK_AIF_APB_ENABLE;           //0x0000008C
    /// This register is protected.
    REG32                          CLK_AIF_APB_DISABLE;          //0x00000090
    REG32                          CLK_AON_AHB_MODE;             //0x00000094
    REG32                          CLK_AON_AHB_ENABLE;           //0x00000098
    /// This register is protected.
    REG32                          CLK_AON_AHB_DISABLE;          //0x0000009C
    REG32                          CLK_AON_APB_MODE;             //0x000000A0
    REG32                          CLK_AON_APB_ENABLE;           //0x000000A4
    /// This register is protected.
    REG32                          CLK_AON_APB_DISABLE;          //0x000000A8
    REG32                          CLK_RF_AHB_MODE;              //0x000000AC
    REG32                          CLK_RF_AHB_ENABLE;            //0x000000B0
    /// This register is protected.
    REG32                          CLK_RF_AHB_DISABLE;           //0x000000B4
    REG32                          CLK_RF_APB_MODE;              //0x000000B8
    REG32                          CLK_RF_APB_ENABLE;            //0x000000BC
    /// This register is protected.
    REG32                          CLK_RF_APB_DISABLE;           //0x000000C0
    REG32                          CLK_OTHERS_MODE;              //0x000000C4
    REG32                          CLK_OTHERS_ENABLE;            //0x000000C8
    /// This register is protected.
    REG32                          CLK_OTHERS_DISABLE;           //0x000000CC
    REG32                          CLK_OTHERS_1_MODE;            //0x000000D0
    REG32                          CLK_OTHERS_1_ENABLE;          //0x000000D4
    /// This register is protected.
    REG32                          CLK_OTHERS_1_DISABLE;         //0x000000D8
    /// Register protected by Write_Unlocked_H.
    REG32                          Pll_Ctrl;                     //0x000000DC
    /// This register is protected.
    REG32                          Sel_Clock;                    //0x000000E0
    REG32 Reserved_000000E4;                    //0x000000E4
    /// This register is protected.
    REG32                          Cfg_Clk_Out;                  //0x000000E8
    REG32 Reserved_000000EC;                    //0x000000EC
    REG32                          Cfg_Clk_AudioBCK1_Div;        //0x000000F0
    REG32                          Cfg_Clk_AudioBCK2_Div;        //0x000000F4
    REG32                          Cfg_Clk_Uart[4];              //0x000000F8
    REG32                          Cfg_Clk_PWM;                  //0x00000108
    REG32                          Cfg_Clk_Dbg_Div;              //0x0000010C
    REG32                          Cfg_Clk_Camera_Out;           //0x00000110
    REG32                          Reset_Cause;                  //0x00000114
    /// This register is protected.
    REG32                          WakeUp;                       //0x00000118
    /// This register is protected.
    REG32                          Ignore_Charger;               //0x0000011C
    REG32 Reserved_00000120[2];                 //0x00000120
    REG32                          CFG_PLL_SPIFLASH_DIV;         //0x00000128
    REG32                          CFG_PLL_SPIFLASH1_DIV;        //0x0000012C
    REG32                          CFG_PLL_MEM_BRIDGE_DIV;       //0x00000130
    REG32                          CFG_DBG_CLK_SOURCE_SEL;       //0x00000134
    REG32                          CFG_CLK_494M_LTE_DIV;         //0x00000138
    REG32                          CFG_PLL_ISP_DIV;              //0x0000013C
    REG32                          CFG_PLL_PIX_DIV;              //0x00000140
    REG32                          CFG_PLL_SYS_AXI_DIV;          //0x00000144
    REG32                          CFG_PLL_SYS_AHB_APB_DIV;      //0x00000148
    REG32                          CFG_PLL_SYS_AHB_BTFM_DIV;     //0x0000014C
    REG32                          CFG_PLL_CSI_DIV;              //0x00000150
    REG32                          CFG_PLL_SYS_SPIFLASH_DIV;     //0x00000154
    REG32                          CFG_PLL_CP_DIV;               //0x00000158
    REG32                          CFG_PLL_AP_CPU_DIV;           //0x0000015C
    REG32                          CFG_AP_CPU_ACLKEN_DIV;        //0x00000160
    REG32                          CFG_AP_CPU_DBGEN_DIV;         //0x00000164
    REG32                          CFG_PLL_CP_CPU_DIV;           //0x00000168
    REG32                          CFG_CP_CPU_ACLKEN_DIV;        //0x0000016C
    REG32                          CFG_CP_CPU_DBGEN_DIV;         //0x00000170
    REG32                          CFG_TRNG_CLKEN_DIV;           //0x00000174
    REG32                          CFG_CORESIGHT_DIV;            //0x00000178
    REG32                          CFG_USB11_48M_DIV;            //0x0000017C
    REG32                          CFG_PLL_USB;                  //0x00000180
    REG32                          CFG_VAD_DIV;                  //0x00000184
    /// This register is ahb master protect cfg.
    REG32                          Cfg_HMPROT;                   //0x00000188
    /// This register is cq memory cfg.
    REG32                          Cfg_MEM_CQ;                   //0x0000018C
    /// This register is a5_top_wrap/axidma/cp_a5_top/f8/gea3_wrap/lzma/sys_imem
    /// mem cfg.
    REG32                          Cfg_ARM_SYS_CFG_MEM;          //0x00000190
    /// This register is audio mem cfg.
    REG32                          Cfg_AUDIO_CFG_MEM;            //0x00000194
    /// This register is lcd/gouda mem cfg.
    REG32                          Cfg_LCD_CFG_MEM;              //0x00000198
    /// This register is camera mem cfg.
    REG32                          Cfg_CAM_CFG_MEM;              //0x0000019C
    /// This register is peri(sdmmc/uart/usbc etc.) mem cfg.
    REG32                          Cfg_PERI_CFG_MEM;             //0x000001A0
    /// This register is aon sys mem cfg.
    REG32                          Cfg_AON_SYS_CFG_MEM;          //0x000001A4
    /// This register is rf sys mem cfg.
    REG32                          Cfg_RF_SYS_CFG_MEM;           //0x000001A8
    /// This register is coresight mem cfg.
    REG32                          Cfg_CORESIGHT_CFG_MEM;        //0x000001AC
    /// This register is vad mem cfg.
    REG32                          Cfg_VAD_CFG_MEM;              //0x000001B0
    /// This register is for audio i2s mux ,aif load_position etc. config.
    REG32                          Cfg_AIF_CFG;                  //0x000001B4
    /// This register is limit_en_spi,,clk_freq cfg.
    REG32                          Cfg_MISC_CFG;                 //0x000001B8
    /// This register is misc cfg.
    REG32                          Cfg_MISC1_CFG;                //0x000001BC
    /// This register set lp related config.
    REG32                          Cfg_FORCE_LP_MODE_LP;         //0x000001C0
    REG32 Reserved_000001C4[4];                 //0x000001C4
    /// This register is reserved.
    REG32                          Cfg_Reserve;                  //0x000001D4
    /// This register is reserved.
    REG32                          Cfg_Reserve1;                 //0x000001D8
    /// This register is reserved.
    REG32                          Cfg_Reserve2;                 //0x000001DC
    /// This register is reserved.
    REG32                          Cfg_Reserve3;                 //0x000001E0
    /// This register is reserved.
    REG32                          Cfg_Reserve4;                 //0x000001E4
    /// This register is reserved.
    REG32                          Cfg_Reserve5;                 //0x000001E8
    /// This register is reserved.
    REG32                          Cfg_Reserve6;                 //0x000001EC
    /// This register is reserved.
    REG32                          Cfg_Reserve7;                 //0x000001F0
    /// This register is reserved.
    REG32                          Cfg_Reserve8;                 //0x000001F4
    /// This register is reserved.
    REG32                          Cfg_Reserve9;                 //0x000001F8
    /// This register is reserved.
    REG32                          Cfg_Reserve10;                //0x000001FC
    /// This register is reserved.
    REG32                          Cfg_Reserve11;                //0x00000200
    /// This register is reserved.
    REG32                          Cfg_Reserve12;                //0x00000204
    /// This register is reserved.
    REG32                          Cfg_Reserve13;                //0x00000208
    /// This register is reserved.
    REG32                          Cfg_Reserve14;                //0x0000020C
    /// This register is reserved.
    REG32                          Cfg_Reserve15;                //0x00000210
    /// This register is reserved.
    REG32                          Cfg_Reserve16;                //0x00000214
    /// This register is reserved.
    REG32                          Cfg_Reserve1_in;              //0x00000218
    /// This register is reserved.
    REG32                          Cfg_Reserve2_in;              //0x0000021C
    /// This register is reserved.
    REG32                          Cfg_Reserve3_in;              //0x00000220
    /// This register is reserved.
    REG32                          Cfg_Reserve4_in;              //0x00000224
    /// This register is reserved.
    REG32                          Cfg_Reserve5_in;              //0x00000228
    /// This register is reserved.
    REG32                          Cfg_Reserve6_in;              //0x0000022C
    /// This register is reserved.
    REG32                          Cfg_Reserve7_in;              //0x00000230
    /// This register is for CHIP_ID(METAL_ID[11:0],BOND_ID[15:12]),PROD[31:16]
    REG32                          Cfg_CHIP_PROD_ID;             //0x00000234
    /// This register is for BUS QOS config.
    REG32                          Cfg_QOS_WCN_A5_GGE;           //0x00000238
    /// This register is for BUS QOS config.
    REG32                          Cfg_QOS_AXIDMA_CPA5_F8;       //0x0000023C
    /// This register is for BUS QOS config.
    REG32                          Cfg_QOS_LCDC_LZMA_GOUDA;      //0x00000240
    /// This register is for BUS QOS config.
    REG32                          Cfg_QOS_LTE_USB;              //0x00000244
    /// This register is for merge mem awqos/arqos QOS config.
    REG32                          Cfg_QOS_MERGE_MEM;            //0x00000248
    /// This register is for bcpu break point debug.
    REG32                          Cfg_BCPU_DBG_BKP;             //0x0000024C
} HWP_SYS_CTRL_T;
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_SYS_CTRL_BASE          0x05100000
#else
#define REG_SYS_CTRL_BASE          0x50100000
#endif
#define hwp_sysCtrl                ((HWP_SYS_CTRL_T*) REG_ACCESS_ADDRESS(REG_SYS_CTRL_BASE))


typedef struct
{
    REG32                          revision; /*0x0  , RO   , 0x00000003, No description.               */
    REG32                          sys_control; /*0x4  , RW   , 0x00000061, No description.               */
    REG32                          bt_control; /*0x8  , RW   , 0x00000027, No description.               */
    REG32                          wf_control; /*0xc  , RW   , 0x00000002, No description.               */
    REG32                          fm_control; /*0x10 , RW   , 0x00004808, No description.               */
    REG32                          inter_freq_setting; /*0x14 , RW   , 0x008002F6, No description.               */
    REG32                          bandgap_setting; /*0x18 , RW   , 0x00000001, No description.               */
    REG32                          lna_rmx_setting; /*0x1c , RW   , 0x00810058, No description.               */
    REG32                          pga_setting0; /*0x20 , RW   , 0x00001800, No description.               */
    REG32                          pga_setting1; /*0x24 , RW   , 0x0006C222, No description.               */
    REG32                          rxflt_setting; /*0x28 , RW   , 0x03361112, No description.               */
    REG32                          adc_setting0; /*0x2c , RW   , 0x00000620, No description.               */
    REG32                          adc_setting1; /*0x30 , RW   , 0x00000000, No description.               */
    REG32                          bt_dac_setting; /*0x34 , RW   , 0x0001008A, No description.               */
    REG32                          bt_txrf_setting; /*0x38 , RW   , 0x1E777777, No description.               */
    REG32                 fm_dsp_setting; /*0x3c , RW   , 0x00000000, No description.               */
    REG32                    vco_setting; /*0x40 , RW   , 0x0196C838, No description.               */
    REG32                   pll_setting0; /*0x44 , RW   , 0x2221319C, No description.               */
    REG32                   pll_setting1; /*0x48 , RW   , 0x221DC8AB, No description.               */
    REG32                   pll_setting2; /*0x4c , RW   , 0x01000022, No description.               */
    REG32                     pll_status; /*0x50 , RW   , 0x00000000, No description.               */
    REG32                 adpll_setting0; /*0x54 , RW   , 0x2B110908, No description.               */
    REG32                 adpll_setting1; /*0x58 , RW   , 0x00000000, No description.               */
    REG32                   adpll_status; /*0x5c , RW   , 0x00000000, No description.               */
    REG32                   mdll_setting; /*0x60 , RW   , 0x000061C5, No description.               */
    REG32               pll_sdm_setting0; /*0x64 , RW   , 0x00144840, No description.               */
    REG32               pll_sdm_setting1; /*0x68 , RW   , 0x2EF13B14, No description.               */
    REG32             adpll_sdm_setting0; /*0x6c , RW   , 0x00000906, No description.               */
    REG32             adpll_sdm_setting1; /*0x70 , RW   , 0x00000000, No description.               */
    REG32             adpll_sdm_setting2; /*0x74 , RW   , 0x00000000, No description.               */
    REG32             rxflt_cal_setting0; /*0x78 , RW   , 0x04B90070, No description.               */
    REG32             rxflt_cal_setting1; /*0x7c , RW   , 0x02EE8080, No description.               */
    REG32             rxflt_cal_setting2; /*0x80 , RW   , 0x472B2B27, No description.               */
    REG32             rxflt_cal_setting3; /*0x84 , RW   , 0x03480348, No description.               */
    REG32            vco_pkd_cal_setting; /*0x88 , RW   , 0x00010860, No description.               */
    REG32               pll_cal_setting0; /*0x8c , RW   , 0x00900048, No description.               */
    REG32               pll_cal_setting1; /*0x90 , RW   , 0x00000000, No description.               */
    REG32                adc_cal_setting; /*0x94 , RW   , 0x00000000, No description.               */
    REG32                 cal_dr_setting; /*0x98 , RW   , 0x00000000, No description.               */
    REG32                     cal_status; /*0x9c , RO   , 0x00000000, No description.               */
    REG32                   cal_results0; /*0xa0 , RO   , 0x3FC08080, No description.               */
    REG32                   cal_results1; /*0xa4 , RO   , 0x00000200, No description.               */
    REG32                       power_dr; /*0xa8 , RW   , 0x00000000, No description.               */
    REG32                      power_reg; /*0xac , RW   , 0x00000000, No description.               */
    REG32                   power_status; /*0xb0 , RO   , 0x00000000, No description.               */
    REG32                bt_gain_table_0; /*0xb4 , RW   , 0x00000000, No description.               */
    REG32                bt_gain_table_1; /*0xb8 , RW   , 0x00002000, No description.               */
    REG32                bt_gain_table_2; /*0xbc , RW   , 0x00002010, No description.               */
    REG32                bt_gain_table_3; /*0xc0 , RW   , 0x00002020, No description.               */
    REG32                bt_gain_table_4; /*0xc4 , RW   , 0x00002030, No description.               */
    REG32                bt_gain_table_5; /*0xc8 , RW   , 0x0000A030, No description.               */
    REG32                bt_gain_table_6; /*0xcc , RW   , 0x0000A040, No description.               */
    REG32                bt_gain_table_7; /*0xd0 , RW   , 0x0000A041, No description.               */
    REG32                bt_gain_table_8; /*0xd4 , RW   , 0x0001A041, No description.               */
    REG32                bt_gain_table_9; /*0xd8 , RW   , 0x0001A043, No description.               */
    REG32                bt_gain_table_a; /*0xdc , RW   , 0x0001A843, No description.               */
    REG32                bt_gain_table_b; /*0xe0 , RW   , 0x0001A847, No description.               */
    REG32                bt_gain_table_c; /*0xe4 , RW   , 0x0001B047, No description.               */
    REG32                bt_gain_table_d; /*0xe8 , RW   , 0x0001B04B, No description.               */
    REG32                bt_gain_table_e; /*0xec , RW   , 0x0001B04F, No description.               */
    REG32                bt_gain_table_f; /*0xf0 , RW   , 0x0001D04F, No description.               */
    REG32                wf_gain_table_0; /*0xf4 , RW   , 0x00000000, No description.               */
    REG32                wf_gain_table_1; /*0xf8 , RW   , 0x00002000, No description.               */
    REG32                wf_gain_table_2; /*0xfc , RW   , 0x00002010, No description.               */
    REG32                wf_gain_table_3; /*0x100, RW   , 0x00002020, No description.               */
    REG32                wf_gain_table_4; /*0x104, RW   , 0x00002030, No description.               */
    REG32                wf_gain_table_5; /*0x108, RW   , 0x0000A030, No description.               */
    REG32                wf_gain_table_6; /*0x10c, RW   , 0x0000A040, No description.               */
    REG32                wf_gain_table_7; /*0x110, RW   , 0x0000A041, No description.               */
    REG32                wf_gain_table_8; /*0x114, RW   , 0x0001A041, No description.               */
    REG32                wf_gain_table_9; /*0x118, RW   , 0x0001A043, No description.               */
    REG32                wf_gain_table_a; /*0x11c, RW   , 0x0001A843, No description.               */
    REG32                wf_gain_table_b; /*0x120, RW   , 0x0001A847, No description.               */
    REG32                wf_gain_table_c; /*0x124, RW   , 0x0001B047, No description.               */
    REG32                wf_gain_table_d; /*0x128, RW   , 0x0001B04B, No description.               */
    REG32                wf_gain_table_e; /*0x12c, RW   , 0x0001B04F, No description.               */
    REG32                wf_gain_table_f; /*0x130, RW   , 0x0001D04F, No description.               */
    REG32             bt_tx_gain_table_0; /*0x134, RW   , 0x00000000, No description.               */
    REG32             bt_tx_gain_table_1; /*0x138, RW   , 0x01150315, No description.               */
    REG32             bt_tx_gain_table_2; /*0x13c, RW   , 0x06150715, No description.               */
    REG32             bt_tx_gain_table_3; /*0x140, RW   , 0x0B150F15, No description.               */
    REG32                fm_gain_table_0; /*0x144, RW   , 0x10201030, No description.               */
    REG32                fm_gain_table_1; /*0x148, RW   , 0x104010C0, No description.               */
    REG32                fm_gain_table_2; /*0x14c, RW   , 0x1AC01B40, No description.               */
    REG32                fm_gain_table_3; /*0x150, RW   , 0x1D401DC0, No description.               */
    REG32                fm_gain_table_4; /*0x154, RW   , 0x27C027D0, No description.               */
    REG32                fm_gain_table_5; /*0x158, RW   , 0x27E027F0, No description.               */
    REG32                fm_gain_table_6; /*0x15c, RW   , 0x2FF42FF8, No description.               */
    REG32                fm_gain_table_7; /*0x160, RW   , 0x2FFC2FFE, No description.               */
    REG32            direct_gain_setting; /*0x164, RW   , 0x000F0F0F, No description.               */
    REG32          bt_rxflt_cal_bit_reg0; /*0x168, RW   , 0x80808080, No description.               */
    REG32          bt_rxflt_cal_bit_reg1; /*0x16c, RW   , 0x80808080, No description.               */
    REG32          bt_rxflt_cal_bit_reg2; /*0x170, RW   , 0x80808080, No description.               */
    REG32          bt_rxflt_cal_bit_reg3; /*0x174, RW   , 0x80808080, No description.               */
    REG32          bt_rxflt_cal_bit_reg4; /*0x178, RW   , 0x80808080, No description.               */
    REG32          bt_rxflt_cal_bit_reg5; /*0x17c, RW   , 0x80808080, No description.               */
    REG32          bt_rxflt_cal_bit_reg6; /*0x180, RW   , 0x80808080, No description.               */
    REG32          bt_rxflt_cal_bit_reg7; /*0x184, RW   , 0x80808080, No description.               */
    REG32          bt_rxflt_cal_bit_reg8; /*0x188, RW   , 0x80808080, No description.               */
    REG32          bt_rxflt_cal_bit_reg9; /*0x18c, RW   , 0x80808080, No description.               */
    REG32          bt_rxflt_cal_bit_rega; /*0x190, RW   , 0x80808080, No description.               */
    REG32          bt_rxflt_cal_bit_regb; /*0x194, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_reg0; /*0x198, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_reg1; /*0x19c, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_reg2; /*0x1a0, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_reg3; /*0x1a4, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_reg4; /*0x1a8, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_reg5; /*0x1ac, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_reg6; /*0x1b0, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_reg7; /*0x1b4, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_reg8; /*0x1b8, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_reg9; /*0x1bc, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_rega; /*0x1c0, RW   , 0x80808080, No description.               */
    REG32          wf_rxflt_cal_bit_regb; /*0x1c4, RW   , 0x80808080, No description.               */
    REG32          fm_rxflt_cal_bit_reg0; /*0x1c8, RW   , 0x80808080, No description.               */
    REG32          fm_rxflt_cal_bit_reg1; /*0x1cc, RW   , 0x80808080, No description.               */
    REG32          fm_rxflt_cal_bit_reg2; /*0x1d0, RW   , 0x80808080, No description.               */
    REG32          fm_rxflt_cal_bit_reg3; /*0x1d4, RW   , 0x80808080, No description.               */
    REG32          fm_rxflt_cal_bit_reg4; /*0x1d8, RW   , 0x80808080, No description.               */
    REG32          fm_rxflt_cal_bit_reg5; /*0x1dc, RW   , 0x80808080, No description.               */
    REG32          fm_rxflt_cal_bit_reg6; /*0x1e0, RW   , 0x80808080, No description.               */
    REG32          fm_rxflt_cal_bit_reg7; /*0x1e4, RW   , 0x80808080, No description.               */
    REG32                       test_buf; /*0x1e8, RW   , 0x00000000, No description.               */
    REG32                   ana_reserved; /*0x1ec, RW   , 0x00F0FF00, No description.               */
    REG32                   dig_reserved; /*0x1f0, RW   , 0x0000FF00, No description.               */
    REG32                        new_add; /*0x1f4, RW   , 0x00000020, No description.               */
} T_HWP_WCN_RF_IF_T;
#define REG_WCN_RF_IF_BASE      0x15005000
#define hwp_wcnRfIf           ((T_HWP_WCN_RF_IF_T*)REG_ACCESS_ADDRESS(REG_WCN_RF_IF_BASE))

typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_ahb_btfm_div : 4; // [3:0]
        REG32 cfg_pll_ahb_btfm_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_PLL_SYS_AHB_BTFM_DIV_T;


#endif

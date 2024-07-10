/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
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


#ifndef _CP_CLKRST_H_
#define _CP_CLKRST_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'cp_clkrst'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// CP_CLKRST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_CLKRST_BASE            0x05081000
#else
#define REG_CLKRST_BASE            0x50081000
#endif

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
    REG32                          CLKEN_LTE_INTF;               //0x0000008C
    REG32                          CLKEN_LTE_INTF_SET;           //0x00000090
    REG32                          CLKEN_LTE_INTF_CLR;           //0x00000094
    REG32                          LTE_AUTOGATE_MODE;            //0x00000098
    REG32                          LTE_AUTOGATE_EN;              //0x0000009C
    REG32                          LTE_AUTOGATE_EN_SET;          //0x000000A0
    REG32                          LTE_AUTOGATE_EN_CLR;          //0x000000A4
    REG32                          LTE_AUTOGATE_DELAY_NUM;       //0x000000A8
} HWP_CP_CLKRST_T;

#define hwp_clkrst                 ((HWP_CP_CLKRST_T*) REG_ACCESS_ADDRESS(REG_CLKRST_BASE))


//CLKSEL
typedef union {
    REG32 v;
    struct {
        REG32 slow_sel_122m : 1; // [0]
        REG32 slow_sel_480m : 1; // [1]
        REG32 bb_sysctrl_wd_sel : 1; // [2]
        REG32 zsp_wd_sel : 1; // [3]
        REG32 idle_h_sel : 2; // [5:4]
        REG32 __31_6 : 26;
    } b;
} REG_CP_CLKRST_CLKSEL_T;

//CLKSEL_SET
typedef union {
    REG32 v;
    struct {
        REG32 clkselset : 5; // [4:0], write set
        REG32 __31_5 : 27;
    } b;
} REG_CP_CLKRST_CLKSEL_SET_T;

//CLKSEL_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clkselclear : 5; // [4:0], write clear
        REG32 __31_5 : 27;
    } b;
} REG_CP_CLKRST_CLKSEL_CLR_T;

//CLKDIV_ZSP
typedef union {
    REG32 v;
    struct {
        REG32 zsp_cd : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CP_CLKRST_CLKDIV_ZSP_T;

//CLKDIV_LTE
typedef union {
    REG32 v;
    struct {
        REG32 lte_cd : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CP_CLKRST_CLKDIV_LTE_T;

//CLKEN_BB_SYSCTRL
typedef union {
    REG32 v;
    struct {
        REG32 clken_sysreg : 1; // [0]
        REG32 clken_pwrctrl : 1; // [1]
        REG32 clken_mailbox : 1; // [2]
        REG32 clken_idle : 1; // [3]
        REG32 clken_cp_wd : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_CP_CLKRST_CLKEN_BB_SYSCTRL_T;

//CLKEN_BB_SYSCTRL_SET
typedef union {
    REG32 v;
    struct {
        REG32 clken_bb_sysctrlset : 5; // [4:0], write set
        REG32 __31_5 : 27;
    } b;
} REG_CP_CLKRST_CLKEN_BB_SYSCTRL_SET_T;

//CLKEN_BB_SYSCTRL_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clken_bb_sysctrlclear : 5; // [4:0], write clear
        REG32 __31_5 : 27;
    } b;
} REG_CP_CLKRST_CLKEN_BB_SYSCTRL_CLR_T;

//CLKEN_ZSP
typedef union {
    REG32 v;
    struct {
        REG32 clken_zspcore : 1; // [0]
        REG32 clken_axidma : 1; // [1]
        REG32 clken_zirq : 1; // [2]
        REG32 clken_busmon : 1; // [3]
        REG32 clken_wd : 1; // [4]
        REG32 clken_aud_dft : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_CP_CLKRST_CLKEN_ZSP_T;

//CLKEN_ZSP_SET
typedef union {
    REG32 v;
    struct {
        REG32 clken_zspset : 6; // [5:0], write set
        REG32 __31_6 : 26;
    } b;
} REG_CP_CLKRST_CLKEN_ZSP_SET_T;

//CLKEN_ZSP_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clken_zspclear : 6; // [5:0], write clear
        REG32 __31_6 : 26;
    } b;
} REG_CP_CLKRST_CLKEN_ZSP_CLR_T;

//CLKEN_LTE
typedef union {
    REG32 v;
    struct {
        REG32 clken_txrx : 1; // [0]
        REG32 clken_coeff : 1; // [1]
        REG32 clken_ldtc : 1; // [2]
        REG32 clken_ldtc1 : 1; // [3]
        REG32 clken_measpwr : 1; // [4]
        REG32 clken_iddet : 1; // [5]
        REG32 clken_otdoa : 1; // [6]
        REG32 clken_uldft : 1; // [7]
        REG32 clken_pusch : 1; // [8]
        REG32 clken_csirs : 1; // [9]
        REG32 clken_dlfft : 1; // [10]
        REG32 clken_rfad : 1; // [11]
        REG32 clken_rxcapt : 1; // [12]
        REG32 __31_13 : 19;
    } b;
} REG_CP_CLKRST_CLKEN_LTE_T;

//CLKEN_LTE_SET
typedef union {
    REG32 v;
    struct {
        REG32 clken_lteset : 13; // [12:0], write set
        REG32 __31_13 : 19;
    } b;
} REG_CP_CLKRST_CLKEN_LTE_SET_T;

//CLKEN_LTE_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clken_lteclear : 13; // [12:0], write clear
        REG32 __31_13 : 19;
    } b;
} REG_CP_CLKRST_CLKEN_LTE_CLR_T;

//CLKEN_ZSPCORE_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_CLKRST_CLKEN_ZSPCORE_MODE_T;

//CLKEN_ZSP_AXIDMA_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_CLKRST_CLKEN_ZSP_AXIDMA_MODE_T;

//RSTCTRL_BB_SYSCTRL
typedef union {
    REG32 v;
    struct {
        REG32 rstctrl_sysreg : 1; // [0]
        REG32 rstctrl_pwrctrl : 1; // [1]
        REG32 rstctrl_mailbox : 1; // [2]
        REG32 rstctrl_idle : 1; // [3]
        REG32 rstctrl_cp_wd : 1; // [4]
        REG32 rstctrl_monitor : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_CP_CLKRST_RSTCTRL_BB_SYSCTRL_T;

//RSTCTRL_BB_SYSCTRL_SET
typedef union {
    REG32 v;
    struct {
        REG32 rstctrl_bb_sysctrlset : 6; // [5:0], write set
        REG32 __31_6 : 26;
    } b;
} REG_CP_CLKRST_RSTCTRL_BB_SYSCTRL_SET_T;

//RSTCTRL_BB_SYSCTRL_CLR
typedef union {
    REG32 v;
    struct {
        REG32 rstctrl_bb_sysctrlclear : 6; // [5:0], write clear
        REG32 __31_6 : 26;
    } b;
} REG_CP_CLKRST_RSTCTRL_BB_SYSCTRL_CLR_T;

//RSTCTRL_ZSP
typedef union {
    REG32 v;
    struct {
        REG32 rstctrl_zspcore : 1; // [0]
        REG32 rstctrl_axidma : 1; // [1]
        REG32 rstctrl_zirq : 1; // [2]
        REG32 rstctrl_busmon : 1; // [3]
        REG32 rstctrl_wd : 1; // [4]
        REG32 rstctrl_aud_dft : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_CP_CLKRST_RSTCTRL_ZSP_T;

//RSTCTRL_ZSP_SET
typedef union {
    REG32 v;
    struct {
        REG32 rstctrl_zspset : 6; // [5:0], write set
        REG32 __31_6 : 26;
    } b;
} REG_CP_CLKRST_RSTCTRL_ZSP_SET_T;

//RSTCTRL_ZSP_CLR
typedef union {
    REG32 v;
    struct {
        REG32 rstctrl_zspclear : 6; // [5:0], write clear
        REG32 __31_6 : 26;
    } b;
} REG_CP_CLKRST_RSTCTRL_ZSP_CLR_T;

//RSTCTRL_LTE
typedef union {
    REG32 v;
    struct {
        REG32 rstctrl_txrx_tx : 1; // [0]
        REG32 rstctrl_txrx_rx : 1; // [1]
        REG32 rstctrl_coeff : 1; // [2]
        REG32 rstctrl_ldtc : 1; // [3]
        REG32 rstctrl_ldtc1 : 1; // [4]
        REG32 rstctrl_measpwr : 1; // [5]
        REG32 rstctrl_iddet : 1; // [6]
        REG32 rstctrl_otdoa : 1; // [7]
        REG32 rstctrl_uldft : 1; // [8]
        REG32 rstctrl_pusch : 1; // [9]
        REG32 rstctrl_csirs : 1; // [10]
        REG32 rstctrl_dlfft : 1; // [11]
        REG32 rstctrl_rfad : 1; // [12]
        REG32 rstctrl_rxcapt : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_CP_CLKRST_RSTCTRL_LTE_T;

//RSTCTRL_LTE_SET
typedef union {
    REG32 v;
    struct {
        REG32 rstctrl_lteset : 14; // [13:0], write set
        REG32 __31_14 : 18;
    } b;
} REG_CP_CLKRST_RSTCTRL_LTE_SET_T;

//RSTCTRL_LTE_CLR
typedef union {
    REG32 v;
    struct {
        REG32 rstctrl_lteclear : 14; // [13:0], write clear
        REG32 __31_14 : 18;
    } b;
} REG_CP_CLKRST_RSTCTRL_LTE_CLR_T;

//ZSP_SOFT_RST
typedef union {
    REG32 v;
    struct {
        REG32 zsp_soft_rst_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_CLKRST_ZSP_SOFT_RST_T;

//LTE_SOFT_RST
typedef union {
    REG32 v;
    struct {
        REG32 lte_soft_rst_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_CLKRST_LTE_SOFT_RST_T;

//ZSP_AXILPCNT
typedef union {
    REG32 v;
    struct {
        REG32 cnt : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_CLKRST_ZSP_AXILPCNT_T;

//ZSP_BUSLPMC
typedef union {
    REG32 v;
    struct {
        REG32 buslpmc_sw1 : 1; // [0]
        REG32 buslpmc_sw2 : 1; // [1]
        REG32 buslpmc_phy : 1; // [2]
        REG32 buslpmc_zspcore : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_CP_CLKRST_ZSP_BUSLPMC_T;

//ZSP_BUSLPMC_SET
typedef union {
    REG32 v;
    struct {
        REG32 zsp_buslpmcset : 4; // [3:0], write set
        REG32 __31_4 : 28;
    } b;
} REG_CP_CLKRST_ZSP_BUSLPMC_SET_T;

//ZSP_BUSLPMC_CLR
typedef union {
    REG32 v;
    struct {
        REG32 zsp_buslpmcclear : 4; // [3:0], write clear
        REG32 __31_4 : 28;
    } b;
} REG_CP_CLKRST_ZSP_BUSLPMC_CLR_T;

//ZSP_BUSFORCELPMC
typedef union {
    REG32 v;
    struct {
        REG32 busforcelpmc_sw1 : 1; // [0]
        REG32 busforcelpmc_sw2 : 1; // [1]
        REG32 busforcelpmc_phy : 1; // [2]
        REG32 busforcelpmc_zspcore : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_CP_CLKRST_ZSP_BUSFORCELPMC_T;

//ZSP_BUSFORCELPMC_SET
typedef union {
    REG32 v;
    struct {
        REG32 zsp_busforcelpmcset : 4; // [3:0], write set
        REG32 __31_4 : 28;
    } b;
} REG_CP_CLKRST_ZSP_BUSFORCELPMC_SET_T;

//ZSP_BUSFORCELPMC_CLR
typedef union {
    REG32 v;
    struct {
        REG32 zsp_busforcelpmcclear : 4; // [3:0], write clear
        REG32 __31_4 : 28;
    } b;
} REG_CP_CLKRST_ZSP_BUSFORCELPMC_CLR_T;

//CLKEN_MAILBOX_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_CLKRST_CLKEN_MAILBOX_MODE_T;

//CLKEN_LTE_INTF
typedef union {
    REG32 v;
    struct {
        REG32 clken_txrx_intf : 1; // [0]
        REG32 clken_coeff_intf : 1; // [1]
        REG32 clken_ldtc_intf : 1; // [2]
        REG32 clken_ldtc1_intf : 1; // [3]
        REG32 clken_measpwr_intf : 1; // [4]
        REG32 clken_iddet_intf : 1; // [5]
        REG32 clken_otdoa_intf : 1; // [6]
        REG32 clken_uldft_intf : 1; // [7]
        REG32 clken_pusch_intf : 1; // [8]
        REG32 clken_csirs_intf : 1; // [9]
        REG32 clken_dlfft_intf : 1; // [10]
        REG32 clken_rfad_intf : 1; // [11]
        REG32 clken_rxcapt_intf : 1; // [12]
        REG32 __31_13 : 19;
    } b;
} REG_CP_CLKRST_CLKEN_LTE_INTF_T;

//CLKEN_LTE_INTF_SET
typedef union {
    REG32 v;
    struct {
        REG32 clken_lte_intfset : 13; // [12:0], write set
        REG32 __31_13 : 19;
    } b;
} REG_CP_CLKRST_CLKEN_LTE_INTF_SET_T;

//CLKEN_LTE_INTF_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clken_lte_intfclear : 13; // [12:0], write clear
        REG32 __31_13 : 19;
    } b;
} REG_CP_CLKRST_CLKEN_LTE_INTF_CLR_T;

//LTE_AUTOGATE_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_CLKRST_LTE_AUTOGATE_MODE_T;

//LTE_AUTOGATE_EN
typedef union {
    REG32 v;
    struct {
        REG32 txrx_func_autogate_en : 1; // [0]
        REG32 coeff_func_autogate_en : 1; // [1]
        REG32 ldtc_func_autogate_en : 1; // [2]
        REG32 ldtc1_func_autogate_en : 1; // [3]
        REG32 measpwr_func_autogate_en : 1; // [4]
        REG32 iddet_func_autogate_en : 1; // [5]
        REG32 otdoa_func_autogate_en : 1; // [6]
        REG32 uldft_func_autogate_en : 1; // [7]
        REG32 pusch_func_autogate_en : 1; // [8]
        REG32 csirs_func_autogate_en : 1; // [9]
        REG32 dlfft_func_autogate_en : 1; // [10]
        REG32 txrx_intf_autogate_en : 1; // [11]
        REG32 coeff_intf_autogate_en : 1; // [12]
        REG32 ldtc_intf_autogate_en : 1; // [13]
        REG32 ldtc1_intf_autogate_en : 1; // [14]
        REG32 measpwr_intf_autogate_en : 1; // [15]
        REG32 iddet_intf_autogate_en : 1; // [16]
        REG32 otdoa_intf_autogate_en : 1; // [17]
        REG32 uldft_intf_autogate_en : 1; // [18]
        REG32 pusch_intf_autogate_en : 1; // [19]
        REG32 csirs_intf_autogate_en : 1; // [20]
        REG32 dlfft_intf_autogate_en : 1; // [21]
        REG32 __23_22 : 2;
        REG32 downlink_func_autogate_en : 1; // [24]
        REG32 uplink_func_autogate_en : 1; // [25]
        REG32 downlink_intf_autogate_en : 1; // [26]
        REG32 uplink_intf_autogate_en : 1; // [27]
        REG32 __31_28 : 4;
    } b;
} REG_CP_CLKRST_LTE_AUTOGATE_EN_T;

//LTE_AUTOGATE_EN_SET
typedef union {
    REG32 v;
    struct {
        REG32 clken_lte_intfset : 28; // [27:0], write set
        REG32 __31_28 : 4;
    } b;
} REG_CP_CLKRST_LTE_AUTOGATE_EN_SET_T;

//LTE_AUTOGATE_EN_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clken_lte_intfclear : 28; // [27:0], write clear
        REG32 __31_28 : 4;
    } b;
} REG_CP_CLKRST_LTE_AUTOGATE_EN_CLR_T;

//LTE_AUTOGATE_DELAY_NUM
typedef union {
    REG32 v;
    struct {
        REG32 delaycounternumber : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CP_CLKRST_LTE_AUTOGATE_DELAY_NUM_T;


//CLKSEL
#define CP_CLKRST_RESERVED_0(n)    (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_IDLE_H_SEL(n)    (((n)&3)<<4)
#define CP_CLKRST_ZSP_WD_SEL       (1<<3)
#define CP_CLKRST_BB_SYSCTRL_WD_SEL (1<<2)
#define CP_CLKRST_SLOW_SEL_480M    (1<<1)
#define CP_CLKRST_SLOW_SEL_122M    (1<<0)

//CLKSEL_SET
#define CP_CLKRST_RESERVED_0_X1(n) (((n)&0x7FFFFFF)<<5)
#define CP_CLKRST_CLKSELSET(n)     (((n)&31)<<0)

//CLKSEL_CLR
//#define CP_CLKRST_RESERVED_0_X1(n) (((n)&0x7FFFFFF)<<5)
#define CP_CLKRST_CLKSELCLEAR(n)   (((n)&31)<<0)

//CLKDIV_ZSP
#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0xFFFFFFF)<<4)
#define CP_CLKRST_ZSP_CD(n)        (((n)&15)<<0)

//CLKDIV_LTE
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0xFFFFFFF)<<4)
#define CP_CLKRST_LTE_CD(n)        (((n)&15)<<0)

//CLKEN_BB_SYSCTRL
//#define CP_CLKRST_RESERVED_0_X1(n) (((n)&0x7FFFFFF)<<5)
#define CP_CLKRST_CLKEN_CP_WD      (1<<4)
#define CP_CLKRST_CLKEN_IDLE       (1<<3)
#define CP_CLKRST_CLKEN_MAILBOX    (1<<2)
#define CP_CLKRST_CLKEN_PWRCTRL    (1<<1)
#define CP_CLKRST_CLKEN_SYSREG     (1<<0)

//CLKEN_BB_SYSCTRL_SET
//#define CP_CLKRST_RESERVED_0_X1(n) (((n)&0x7FFFFFF)<<5)
#define CP_CLKRST_CLKEN_BB_SYSCTRLSET(n) (((n)&31)<<0)

//CLKEN_BB_SYSCTRL_CLR
//#define CP_CLKRST_RESERVED_0_X1(n) (((n)&0x7FFFFFF)<<5)
#define CP_CLKRST_CLKEN_BB_SYSCTRLCLEAR(n) (((n)&31)<<0)

//CLKEN_ZSP
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_CLKEN_AUD_DFT    (1<<5)
#define CP_CLKRST_CLKEN_WD         (1<<4)
#define CP_CLKRST_CLKEN_BUSMON     (1<<3)
#define CP_CLKRST_CLKEN_ZIRQ       (1<<2)
#define CP_CLKRST_CLKEN_AXIDMA     (1<<1)
#define CP_CLKRST_CLKEN_ZSPCORE    (1<<0)

//CLKEN_ZSP_SET
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_CLKEN_ZSPSET(n)  (((n)&0x3F)<<0)

//CLKEN_ZSP_CLR
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_CLKEN_ZSPCLEAR(n) (((n)&0x3F)<<0)

//CLKEN_LTE
#define CP_CLKRST_RESERVED_0_X3(n) (((n)&0x7FFFF)<<13)
#define CP_CLKRST_CLKEN_RXCAPT     (1<<12)
#define CP_CLKRST_CLKEN_RFAD       (1<<11)
#define CP_CLKRST_CLKEN_DLFFT      (1<<10)
#define CP_CLKRST_CLKEN_CSIRS      (1<<9)
#define CP_CLKRST_CLKEN_PUSCH      (1<<8)
#define CP_CLKRST_CLKEN_ULDFT      (1<<7)
#define CP_CLKRST_CLKEN_OTDOA      (1<<6)
#define CP_CLKRST_CLKEN_IDDET      (1<<5)
#define CP_CLKRST_CLKEN_MEASPWR    (1<<4)
#define CP_CLKRST_CLKEN_LDTC1      (1<<3)
#define CP_CLKRST_CLKEN_LDTC       (1<<2)
#define CP_CLKRST_CLKEN_COEFF      (1<<1)
#define CP_CLKRST_CLKEN_TXRX       (1<<0)

//CLKEN_LTE_SET
//#define CP_CLKRST_RESERVED_0_X3(n) (((n)&0x7FFFF)<<13)
#define CP_CLKRST_CLKEN_LTESET(n)  (((n)&0x1FFF)<<0)

//CLKEN_LTE_CLR
//#define CP_CLKRST_RESERVED_0_X3(n) (((n)&0x7FFFF)<<13)
#define CP_CLKRST_CLKEN_LTECLEAR(n) (((n)&0x1FFF)<<0)

//CLKEN_ZSPCORE_MODE
#define CP_CLKRST_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_CLKRST_MODE             (1<<0)

//CLKEN_ZSP_AXIDMA_MODE
//#define CP_CLKRST_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
//#define CP_CLKRST_MODE           (1<<0)

//RSTCTRL_BB_SYSCTRL
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_MONITOR  (1<<5)
#define CP_CLKRST_RSTCTRL_CP_WD    (1<<4)
#define CP_CLKRST_RSTCTRL_IDLE     (1<<3)
#define CP_CLKRST_RSTCTRL_MAILBOX  (1<<2)
#define CP_CLKRST_RSTCTRL_PWRCTRL  (1<<1)
#define CP_CLKRST_RSTCTRL_SYSREG   (1<<0)

//RSTCTRL_BB_SYSCTRL_SET
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_BB_SYSCTRLSET(n) (((n)&0x3F)<<0)

//RSTCTRL_BB_SYSCTRL_CLR
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_BB_SYSCTRLCLEAR(n) (((n)&0x3F)<<0)

//RSTCTRL_ZSP
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_AUD_DFT  (1<<5)
#define CP_CLKRST_RSTCTRL_WD       (1<<4)
#define CP_CLKRST_RSTCTRL_BUSMON   (1<<3)
#define CP_CLKRST_RSTCTRL_ZIRQ     (1<<2)
#define CP_CLKRST_RSTCTRL_AXIDMA   (1<<1)
#define CP_CLKRST_RSTCTRL_ZSPCORE  (1<<0)

//RSTCTRL_ZSP_SET
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_ZSPSET(n) (((n)&0x3F)<<0)

//RSTCTRL_ZSP_CLR
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_ZSPCLEAR(n) (((n)&0x3F)<<0)

//RSTCTRL_LTE
#define CP_CLKRST_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
#define CP_CLKRST_RSTCTRL_RXCAPT   (1<<13)
#define CP_CLKRST_RSTCTRL_RFAD     (1<<12)
#define CP_CLKRST_RSTCTRL_DLFFT    (1<<11)
#define CP_CLKRST_RSTCTRL_CSIRS    (1<<10)
#define CP_CLKRST_RSTCTRL_PUSCH    (1<<9)
#define CP_CLKRST_RSTCTRL_ULDFT    (1<<8)
#define CP_CLKRST_RSTCTRL_OTDOA    (1<<7)
#define CP_CLKRST_RSTCTRL_IDDET    (1<<6)
#define CP_CLKRST_RSTCTRL_MEASPWR  (1<<5)
#define CP_CLKRST_RSTCTRL_LDTC1    (1<<4)
#define CP_CLKRST_RSTCTRL_LDTC     (1<<3)
#define CP_CLKRST_RSTCTRL_COEFF    (1<<2)
#define CP_CLKRST_RSTCTRL_TXRX_RX  (1<<1)
#define CP_CLKRST_RSTCTRL_TXRX_TX  (1<<0)

//RSTCTRL_LTE_SET
//#define CP_CLKRST_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
#define CP_CLKRST_RSTCTRL_LTESET(n) (((n)&0x3FFF)<<0)

//RSTCTRL_LTE_CLR
//#define CP_CLKRST_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
#define CP_CLKRST_RSTCTRL_LTECLEAR(n) (((n)&0x3FFF)<<0)

//ZSP_SOFT_RST
//#define CP_CLKRST_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_CLKRST_ZSP_SOFT_RST_CTRL (1<<0)

//LTE_SOFT_RST
//#define CP_CLKRST_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_CLKRST_LTE_SOFT_RST_CTRL (1<<0)

//ZSP_AXILPCNT
#define CP_CLKRST_RESERVED_0_X6(n) (((n)&0xFFFF)<<16)
#define CP_CLKRST_CNT(n)           (((n)&0xFFFF)<<0)

//ZSP_BUSLPMC
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0xFFFFFFF)<<4)
#define CP_CLKRST_BUSLPMC_ZSPCORE  (1<<3)
#define CP_CLKRST_BUSLPMC_PHY      (1<<2)
#define CP_CLKRST_BUSLPMC_SW2      (1<<1)
#define CP_CLKRST_BUSLPMC_SW1      (1<<0)

//ZSP_BUSLPMC_SET
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0xFFFFFFF)<<4)
#define CP_CLKRST_ZSP_BUSLPMCSET(n) (((n)&15)<<0)

//ZSP_BUSLPMC_CLR
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0xFFFFFFF)<<4)
#define CP_CLKRST_ZSP_BUSLPMCCLEAR(n) (((n)&15)<<0)

//ZSP_BUSFORCELPMC
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0xFFFFFFF)<<4)
#define CP_CLKRST_BUSFORCELPMC_ZSPCORE (1<<3)
#define CP_CLKRST_BUSFORCELPMC_PHY (1<<2)
#define CP_CLKRST_BUSFORCELPMC_SW2 (1<<1)
#define CP_CLKRST_BUSFORCELPMC_SW1 (1<<0)

//ZSP_BUSFORCELPMC_SET
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0xFFFFFFF)<<4)
#define CP_CLKRST_ZSP_BUSFORCELPMCSET(n) (((n)&15)<<0)

//ZSP_BUSFORCELPMC_CLR
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0xFFFFFFF)<<4)
#define CP_CLKRST_ZSP_BUSFORCELPMCCLEAR(n) (((n)&15)<<0)

//CLKEN_MAILBOX_MODE
//#define CP_CLKRST_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
//#define CP_CLKRST_MODE           (1<<0)

//CLKEN_LTE_INTF
//#define CP_CLKRST_RESERVED_0_X3(n) (((n)&0x7FFFF)<<13)
#define CP_CLKRST_CLKEN_RXCAPT_INTF (1<<12)
#define CP_CLKRST_CLKEN_RFAD_INTF  (1<<11)
#define CP_CLKRST_CLKEN_DLFFT_INTF (1<<10)
#define CP_CLKRST_CLKEN_CSIRS_INTF (1<<9)
#define CP_CLKRST_CLKEN_PUSCH_INTF (1<<8)
#define CP_CLKRST_CLKEN_ULDFT_INTF (1<<7)
#define CP_CLKRST_CLKEN_OTDOA_INTF (1<<6)
#define CP_CLKRST_CLKEN_IDDET_INTF (1<<5)
#define CP_CLKRST_CLKEN_MEASPWR_INTF (1<<4)
#define CP_CLKRST_CLKEN_LDTC1_INTF (1<<3)
#define CP_CLKRST_CLKEN_LDTC_INTF  (1<<2)
#define CP_CLKRST_CLKEN_COEFF_INTF (1<<1)
#define CP_CLKRST_CLKEN_TXRX_INTF  (1<<0)

//CLKEN_LTE_INTF_SET
//#define CP_CLKRST_RESERVED_0_X3(n) (((n)&0x7FFFF)<<13)
#define CP_CLKRST_CLKEN_LTE_INTFSET(n) (((n)&0x1FFF)<<0)

//CLKEN_LTE_INTF_CLR
//#define CP_CLKRST_RESERVED_0_X3(n) (((n)&0x7FFFF)<<13)
#define CP_CLKRST_CLKEN_LTE_INTFCLEAR(n) (((n)&0x1FFF)<<0)

//LTE_AUTOGATE_MODE
//#define CP_CLKRST_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
//#define CP_CLKRST_MODE           (1<<0)

//LTE_AUTOGATE_EN
#define CP_CLKRST_RESERVED_0_X7(n) (((n)&15)<<28)
#define CP_CLKRST_UPLINK_INTF_AUTOGATE_EN (1<<27)
#define CP_CLKRST_DOWNLINK_INTF_AUTOGATE_EN (1<<26)
#define CP_CLKRST_UPLINK_FUNC_AUTOGATE_EN (1<<25)
#define CP_CLKRST_DOWNLINK_FUNC_AUTOGATE_EN (1<<24)
#define CP_CLKRST_RESERVED_1       (1<<23)
#define CP_CLKRST_RESERVED_2       (1<<22)
#define CP_CLKRST_DLFFT_INTF_AUTOGATE_EN (1<<21)
#define CP_CLKRST_CSIRS_INTF_AUTOGATE_EN (1<<20)
#define CP_CLKRST_PUSCH_INTF_AUTOGATE_EN (1<<19)
#define CP_CLKRST_ULDFT_INTF_AUTOGATE_EN (1<<18)
#define CP_CLKRST_OTDOA_INTF_AUTOGATE_EN (1<<17)
#define CP_CLKRST_IDDET_INTF_AUTOGATE_EN (1<<16)
#define CP_CLKRST_MEASPWR_INTF_AUTOGATE_EN (1<<15)
#define CP_CLKRST_LDTC1_INTF_AUTOGATE_EN (1<<14)
#define CP_CLKRST_LDTC_INTF_AUTOGATE_EN (1<<13)
#define CP_CLKRST_COEFF_INTF_AUTOGATE_EN (1<<12)
#define CP_CLKRST_TXRX_INTF_AUTOGATE_EN (1<<11)
#define CP_CLKRST_DLFFT_FUNC_AUTOGATE_EN (1<<10)
#define CP_CLKRST_CSIRS_FUNC_AUTOGATE_EN (1<<9)
#define CP_CLKRST_PUSCH_FUNC_AUTOGATE_EN (1<<8)
#define CP_CLKRST_ULDFT_FUNC_AUTOGATE_EN (1<<7)
#define CP_CLKRST_OTDOA_FUNC_AUTOGATE_EN (1<<6)
#define CP_CLKRST_IDDET_FUNC_AUTOGATE_EN (1<<5)
#define CP_CLKRST_MEASPWR_FUNC_AUTOGATE_EN (1<<4)
#define CP_CLKRST_LDTC1_FUNC_AUTOGATE_EN (1<<3)
#define CP_CLKRST_LDTC_FUNC_AUTOGATE_EN (1<<2)
#define CP_CLKRST_COEFF_FUNC_AUTOGATE_EN (1<<1)
#define CP_CLKRST_TXRX_FUNC_AUTOGATE_EN (1<<0)

//LTE_AUTOGATE_EN_SET
//#define CP_CLKRST_RESERVED_0_X7(n) (((n)&15)<<28)
#define CP_CLKRST_CLKEN_LTE_INTFSET_X1(n) (((n)&0xFFFFFFF)<<0)

//LTE_AUTOGATE_EN_CLR
//#define CP_CLKRST_RESERVED_0_X7(n) (((n)&15)<<28)
#define CP_CLKRST_CLKEN_LTE_INTFCLEAR_X1(n) (((n)&0xFFFFFFF)<<0)

//LTE_AUTOGATE_DELAY_NUM
#define CP_CLKRST_RESERVED_0_X8(n) (((n)&0xFFFFFF)<<8)
#define CP_CLKRST_DELAYCOUNTERNUMBER(n) (((n)&0xFF)<<0)





#endif


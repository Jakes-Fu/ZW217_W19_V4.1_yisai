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


#ifndef _PSRAM_PHY_H_
#define _PSRAM_PHY_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'psram_phy'."
#endif

#include "global_macros.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// PSRAM_PHY_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_PSRAM_PHY_BASE         0x08801800

typedef volatile struct
{
    REG32                          psram_rf_cfg_phy;             //0x00000000
    REG32                          psram_rf_cfg_clock_gate;      //0x00000004
    REG32                          psram_rf_cfg_lpi;             //0x00000008
    REG32 Reserved_0000000C[61];                //0x0000000C
    REG32                          psram_rfdll_cfg_dll;          //0x00000100
    REG32                          psram_rfdll_status_cpst_idle; //0x00000104
    REG32                          psram_rf_status_phy_data_in;  //0x00000108
    REG32 Reserved_0000010C[61];                //0x0000010C
    REG32                          psram_rf_cfg_dll_ads0;        //0x00000200
    REG32                          psram_rfdll_status_dll_ads0;  //0x00000204
    REG32                          psram_rf_cfg_dll_dl_0_wr_ads0; //0x00000208
    REG32                          psram_rf_cfg_dll_dl_1_wr_ads0; //0x0000020C
    REG32                          psram_rf_cfg_dll_dl_2_wr_ads0; //0x00000210
    REG32                          psram_rf_cfg_dll_dl_3_wr_ads0; //0x00000214
    REG32                          psram_rf_cfg_dll_dl_4_wr_ads0; //0x00000218
    REG32                          psram_rf_cfg_dll_dl_5_wr_ads0; //0x0000021C
    REG32                          psram_rf_cfg_dll_dl_6_wr_ads0; //0x00000220
    REG32                          psram_rf_cfg_dll_dl_7_wr_ads0; //0x00000224
    REG32                          psram_rf_cfg_dll_dl_8_wr_ads0; //0x00000228
    REG32                          psram_rf_cfg_dll_dl_9_wr_ads0; //0x0000022C
    REG32                          psram_rfdll_status_max_cnt_ads0; //0x00000230
    REG32                          psram_rfdll_status_min_cnt_ads0; //0x00000234
    REG32                          psram_rf_cfg_phy_iomux_sel_wr_ads0; //0x00000238
    REG32                          psram_rf_cfg_phy_iomux_ie_wr_ads0; //0x0000023C
    REG32                          psram_rf_cfg_phy_iomux_oe_wr_ads0; //0x00000240
    REG32                          psram_rf_cfg_phy_iomux_out_wr_ads0; //0x00000244
    REG32 Reserved_00000248[46];                //0x00000248
    REG32                          psram_rf_cfg_dll_ads1;        //0x00000300
    REG32                          psram_rfdll_status_dll_ads1;  //0x00000304
    REG32                          psram_rf_cfg_dll_dl_0_wr_ads1; //0x00000308
    REG32                          psram_rf_cfg_dll_dl_1_wr_ads1; //0x0000030C
    REG32                          psram_rf_cfg_dll_dl_2_wr_ads1; //0x00000310
    REG32                          psram_rf_cfg_dll_dl_3_wr_ads1; //0x00000314
    REG32                          psram_rf_cfg_dll_dl_4_wr_ads1; //0x00000318
    REG32                          psram_rf_cfg_dll_dl_5_wr_ads1; //0x0000031C
    REG32                          psram_rf_cfg_dll_dl_6_wr_ads1; //0x00000320
    REG32                          psram_rf_cfg_dll_dl_7_wr_ads1; //0x00000324
    REG32                          psram_rf_cfg_dll_dl_8_wr_ads1; //0x00000328
    REG32                          psram_rf_cfg_dll_dl_9_wr_ads1; //0x0000032C
    REG32                          psram_rfdll_status_max_cnt_ads1; //0x00000330
    REG32                          psram_rfdll_status_min_cnt_ads1; //0x00000334
    REG32                          psram_rf_cfg_phy_iomux_sel_wr_ads1; //0x00000338
    REG32                          psram_rf_cfg_phy_iomux_ie_wr_ads1; //0x0000033C
    REG32                          psram_rf_cfg_phy_iomux_oe_wr_ads1; //0x00000340
    REG32                          psram_rf_cfg_phy_iomux_out_wr_ads1; //0x00000344
    REG32 Reserved_00000348[46];                //0x00000348
    REG32                          psram_drf_cfg;                //0x00000400
    REG32                          psram_drf_cfg_reg_sel;        //0x00000404
    REG32                          psram_drf_cfg_dqs_ie_sel_f0;  //0x00000408
    REG32                          psram_drf_cfg_dqs_oe_sel_f0;  //0x0000040C
    REG32                          psram_drf_cfg_dqs_out_sel_f0; //0x00000410
    REG32                          psram_drf_cfg_dqs_gate_sel_f0; //0x00000414
    REG32                          psram_drf_cfg_data_ie_sel_f0; //0x00000418
    REG32                          psram_drf_cfg_data_oe_sel_f0; //0x0000041C
    REG32                          psram_drf_cfg_dqs_ie_sel_f1;  //0x00000420
    REG32                          psram_drf_cfg_dqs_oe_sel_f1;  //0x00000424
    REG32                          psram_drf_cfg_dqs_out_sel_f1; //0x00000428
    REG32                          psram_drf_cfg_dqs_gate_sel_f1; //0x0000042C
    REG32                          psram_drf_cfg_data_ie_sel_f1; //0x00000430
    REG32                          psram_drf_cfg_data_oe_sel_f1; //0x00000434
    REG32                          psram_drf_cfg_dqs_ie_sel_f2;  //0x00000438
    REG32                          psram_drf_cfg_dqs_oe_sel_f2;  //0x0000043C
    REG32                          psram_drf_cfg_dqs_out_sel_f2; //0x00000440
    REG32                          psram_drf_cfg_dqs_gate_sel_f2; //0x00000444
    REG32                          psram_drf_cfg_data_ie_sel_f2; //0x00000448
    REG32                          psram_drf_cfg_data_oe_sel_f2; //0x0000044C
    REG32                          psram_drf_cfg_dqs_ie_sel_f3;  //0x00000450
    REG32                          psram_drf_cfg_dqs_oe_sel_f3;  //0x00000454
    REG32                          psram_drf_cfg_dqs_out_sel_f3; //0x00000458
    REG32                          psram_drf_cfg_dqs_gate_sel_f3; //0x0000045C
    REG32                          psram_drf_cfg_data_ie_sel_f3; //0x00000460
    REG32                          psram_drf_cfg_data_oe_sel_f3; //0x00000464
    REG32                          psram_drf_cfg_dll_mode_f0;    //0x00000468
    REG32                          psram_drf_cfg_dll_cnt_f0;     //0x0000046C
    REG32                          psram_drf_cfg_dll_mode_f1;    //0x00000470
    REG32                          psram_drf_cfg_dll_cnt_f1;     //0x00000474
    REG32                          psram_drf_cfg_dll_mode_f2;    //0x00000478
    REG32                          psram_drf_cfg_dll_cnt_f2;     //0x0000047C
    REG32                          psram_drf_cfg_dll_mode_f3;    //0x00000480
    REG32                          psram_drf_cfg_dll_cnt_f3;     //0x00000484
    REG32 Reserved_00000488[30];                //0x00000488
    REG32                          psram_drf_format_control;     //0x00000500
    REG32                          psram_drf_t_rcd;              //0x00000504
    REG32                          psram_drf_t_rddata_en;        //0x00000508
    REG32                          psram_drf_t_phywrlat;         //0x0000050C
    REG32                          psram_drf_t_cph_wr;           //0x00000510
    REG32                          psram_drf_t_cph_rd;           //0x00000514
    REG32                          psram_drf_t_data_oe_ext;      //0x00000518
    REG32                          psram_drf_t_dqs_oe_ext;       //0x0000051C
    REG32                          psram_drf_t_xphs;             //0x00000520
    REG32                          psram_drf_t_rddata_vld_sync;  //0x00000524
    REG32                          psram_drf_t_rddata_late;      //0x00000528
    REG32 Reserved_0000052C[53];                //0x0000052C
    REG32                          psram_drf_train_cfg;          //0x00000600
    REG32                          psram_drf_mr_data_en;         //0x00000604
    REG32                          psram_drf_mr_data_0;          //0x00000608
    REG32                          psram_drf_mr_data_1;          //0x0000060C
    REG32 Reserved_00000610[60];                //0x00000610
    REG32                          psram_rf_irq_ctrl;            //0x00000700
    REG32                          psram_rf_irq_status_clr;      //0x00000704
    REG32                          psram_rf_irq_status;          //0x00000708
    REG32                          psram_rf_irq_cnt_clr;         //0x0000070C
    REG32                          psram_rf_irq_cnt_dll_unlock_ads0; //0x00000710
    REG32                          psram_rf_irq_cnt_dll_unlock_ads1; //0x00000714
} HWP_PSRAM_PHY_T;

#define hwp_psramPhy               ((HWP_PSRAM_PHY_T*) REG_ACCESS_ADDRESS(REG_PSRAM_PHY_BASE))


//psram_rf_cfg_phy
#define PSRAM_PHY_RF_PHY_INIT_COMPLETE (1<<1)
#define PSRAM_PHY_RF_PHY_EN        (1<<0)

//psram_rf_cfg_clock_gate
#define PSRAM_PHY_RF_CLK_GATE_AG_RD_EN (1<<4)
#define PSRAM_PHY_RF_CLK_GATE_AG_WR_EN (1<<3)
#define PSRAM_PHY_RF_CLK_GATE_AG_EN (1<<2)
#define PSRAM_PHY_RF_CLK_GATE_FG_EN (1<<1)
#define PSRAM_PHY_RF_CLK_GATE_EN   (1<<0)

//psram_rf_cfg_lpi
#define PSRAM_PHY_RF_CWAKEUP_S0    (1<<2)
#define PSRAM_PHY_RF_CWAKEUP_M0    (1<<1)
#define PSRAM_PHY_RF_LPI_SEL_M0    (1<<0)

//psram_rfdll_cfg_dll
#define PSRAM_PHY_RFDLL_RESET      (1<<0)

//psram_rfdll_status_cpst_idle
#define PSRAM_PHY_RFDL_CPST_ST_IDLE_ADS1 (1<<1)
#define PSRAM_PHY_RFDL_CPST_ST_IDLE_ADS0 (1<<0)

//psram_rf_status_phy_data_in
#define PSRAM_PHY_RF_PHY_DATA_IN(n) (((n)&0xFFFF)<<0)

//psram_rf_cfg_dll_ads0
#define PSRAM_PHY_RF_DLL_LOCK_WAIT_ADS0(n) (((n)&15)<<28)
#define PSRAM_PHY_RF_DLL_AUTO_ERR_CLR_EN_ADS0 (1<<27)
#define PSRAM_PHY_RF_DLL_PD_CNT_ADS0(n) (((n)&7)<<24)
#define PSRAM_PHY_RF_DL_CPST_THR_ADS0(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RF_DLL_EN_ADS0   (1<<15)
#define PSRAM_PHY_RF_DLL_CLK_SEL_ADS0 (1<<14)
#define PSRAM_PHY_RF_DLL_ERR_CLR_ADS0 (1<<13)
#define PSRAM_PHY_RF_DL_CPST_AUTO_REF_EN_ADS0 (1<<12)
#define PSRAM_PHY_RF_DL_CPST_START_ADS0 (1<<11)
#define PSRAM_PHY_RF_DL_CPST_EN_ADS0 (1<<10)
#define PSRAM_PHY_RF_DLL_AUTO_CLR_EN_ADS0 (1<<9)
#define PSRAM_PHY_RF_DLL_CLR_ADS0  (1<<8)

//psram_rfdll_status_dll_ads0
#define PSRAM_PHY_RFDLL_ERROR_ADS0 (1<<29)
#define PSRAM_PHY_RFDLL_LOCKED_ADS0 (1<<28)
#define PSRAM_PHY_RFDLL_ST_ADS0(n) (((n)&7)<<25)
#define PSRAM_PHY_RFDL_CPST_ST_ADS0 (1<<24)
#define PSRAM_PHY_RFDLL_CNT_ADS0(n) (((n)&0xFF)<<0)

//psram_rf_cfg_dll_dl_0_wr_ads0
#define PSRAM_PHY_RF_CLKWR_DL_CPST_EN_ADS0 (1<<31)
#define PSRAM_PHY_RF_CLKWR_DL_CPST_MINUS_ADS0 (1<<30)
#define PSRAM_PHY_RF_CLKWR_QTR_DL_CPST_OFFSET_ADS0(n) (((n)&3)<<28)
#define PSRAM_PHY_RFDL_CLKWR_QTR_CNT_ADS0(n) (((n)&3)<<26)
#define PSRAM_PHY_RF_CLKWR_QTR_DL_SEL_ADS0(n) (((n)&3)<<24)
#define PSRAM_PHY_RF_CLKWR_RAW_DL_CPST_OFFSET_ADS0(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDL_CLKWR_RAW_CNT_ADS0(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RF_CLKWR_RAW_DL_SEL_ADS0(n) (((n)&0xFF)<<0)

//psram_rf_cfg_dll_dl_1_wr_ads0
#define PSRAM_PHY_RF_DQS_IN_POS_DL_CPST_EN_ADS0 (1<<31)
#define PSRAM_PHY_RF_DQS_IN_POS_DL_CPST_MINUS_ADS0 (1<<30)
#define PSRAM_PHY_RF_DQS_IN_POS_QTR_DL_CPST_OFFSET_ADS0(n) (((n)&3)<<28)
#define PSRAM_PHY_RFDL_DQS_IN_POS_QTR_CNT_ADS0(n) (((n)&3)<<26)
#define PSRAM_PHY_RF_DQS_IN_POS_QTR_DL_SEL_ADS0(n) (((n)&3)<<24)
#define PSRAM_PHY_RF_DQS_IN_POS_RAW_DL_CPST_OFFSET_ADS0(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDL_DQS_IN_POS_RAW_CNT_ADS0(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RF_DQS_IN_POS_RAW_DL_SEL_ADS0(n) (((n)&0xFF)<<0)

//psram_rf_cfg_dll_dl_2_wr_ads0
#define PSRAM_PHY_RF_DQS_IN_NEG_DL_CPST_EN_ADS0 (1<<31)
#define PSRAM_PHY_RF_DQS_IN_NEG_DL_CPST_MINUS_ADS0 (1<<30)
#define PSRAM_PHY_RF_DQS_IN_NEG_QTR_DL_CPST_OFFSET_ADS0(n) (((n)&3)<<28)
#define PSRAM_PHY_RFDL_DQS_IN_NEG_QTR_CNT_ADS0(n) (((n)&3)<<26)
#define PSRAM_PHY_RF_DQS_IN_NEG_QTR_DL_SEL_ADS0(n) (((n)&3)<<24)
#define PSRAM_PHY_RF_DQS_IN_NEG_RAW_DL_CPST_OFFSET_ADS0(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDL_DQS_IN_NEG_RAW_CNT_ADS0(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RF_DQS_IN_NEG_RAW_DL_SEL_ADS0(n) (((n)&0xFF)<<0)

//psram_rf_cfg_dll_dl_3_wr_ads0
#define PSRAM_PHY_RF_DQS_GATE_DL_CPST_EN_ADS0 (1<<31)
#define PSRAM_PHY_RF_DQS_GATE_DL_CPST_MINUS_ADS0 (1<<30)
#define PSRAM_PHY_RF_DQS_GATE_QTR_DL_CPST_OFFSET_ADS0(n) (((n)&3)<<28)
#define PSRAM_PHY_RFDL_DQS_GATE_QTR_CNT_ADS0(n) (((n)&3)<<26)
#define PSRAM_PHY_RF_DQS_GATE_QTR_DL_SEL_ADS0(n) (((n)&3)<<24)
#define PSRAM_PHY_RF_DQS_GATE_RAW_DL_CPST_OFFSET_ADS0(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDL_DQS_GATE_RAW_CNT_ADS0(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RF_DQS_GATE_RAW_DL_SEL_ADS0(n) (((n)&0xFF)<<0)

//psram_rf_cfg_dll_dl_4_wr_ads0
#define PSRAM_PHY_RF_DLY_OUT_CEN_DL_SEL_ADS0(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_OUT_CLK_DL_SEL_ADS0(n) (((n)&31)<<0)

//psram_rf_cfg_dll_dl_5_wr_ads0
#define PSRAM_PHY_RF_DLY_OUT_D3_DL_SEL_ADS0(n) (((n)&31)<<24)
#define PSRAM_PHY_RF_DLY_OUT_D2_DL_SEL_ADS0(n) (((n)&31)<<16)
#define PSRAM_PHY_RF_DLY_OUT_D1_DL_SEL_ADS0(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_OUT_D0_DL_SEL_ADS0(n) (((n)&31)<<0)

//psram_rf_cfg_dll_dl_6_wr_ads0
#define PSRAM_PHY_RF_DLY_OUT_D7_DL_SEL_ADS0(n) (((n)&31)<<24)
#define PSRAM_PHY_RF_DLY_OUT_D6_DL_SEL_ADS0(n) (((n)&31)<<16)
#define PSRAM_PHY_RF_DLY_OUT_D5_DL_SEL_ADS0(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_OUT_D4_DL_SEL_ADS0(n) (((n)&31)<<0)

//psram_rf_cfg_dll_dl_7_wr_ads0
#define PSRAM_PHY_RF_DLY_IN_D3_DL_SEL_ADS0(n) (((n)&31)<<24)
#define PSRAM_PHY_RF_DLY_IN_D2_DL_SEL_ADS0(n) (((n)&31)<<16)
#define PSRAM_PHY_RF_DLY_IN_D1_DL_SEL_ADS0(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_IN_D0_DL_SEL_ADS0(n) (((n)&31)<<0)

//psram_rf_cfg_dll_dl_8_wr_ads0
#define PSRAM_PHY_RF_DLY_IN_D7_DL_SEL_ADS0(n) (((n)&31)<<24)
#define PSRAM_PHY_RF_DLY_IN_D6_DL_SEL_ADS0(n) (((n)&31)<<16)
#define PSRAM_PHY_RF_DLY_IN_D5_DL_SEL_ADS0(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_IN_D4_DL_SEL_ADS0(n) (((n)&31)<<0)

//psram_rf_cfg_dll_dl_9_wr_ads0
#define PSRAM_PHY_RF_DLY_IN_DQS_DL_SEL_ADS0(n) (((n)&31)<<16)
#define PSRAM_PHY_RF_DLY_OUT_DQM_DL_SEL_ADS0(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_OUT_DQS_DL_SEL_ADS0(n) (((n)&31)<<0)

//psram_rfdll_status_max_cnt_ads0
#define PSRAM_PHY_RFDLL_MAX_CNT_F3_ADS0(n) (((n)&0xFF)<<24)
#define PSRAM_PHY_RFDLL_MAX_CNT_F2_ADS0(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDLL_MAX_CNT_F1_ADS0(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RFDLL_MAX_CNT_F0_ADS0(n) (((n)&0xFF)<<0)

//psram_rfdll_status_min_cnt_ads0
#define PSRAM_PHY_RFDLL_MIN_CNT_F3_ADS0(n) (((n)&0xFF)<<24)
#define PSRAM_PHY_RFDLL_MIN_CNT_F2_ADS0(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDLL_MIN_CNT_F1_ADS0(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RFDLL_MIN_CNT_F0_ADS0(n) (((n)&0xFF)<<0)

//psram_rf_cfg_phy_iomux_sel_wr_ads0
#define PSRAM_PHY_RF_PHY_IO_CSN_SEL_ADS0 (1<<20)
#define PSRAM_PHY_RF_PHY_IO_CLK_SEL_ADS0 (1<<16)
#define PSRAM_PHY_RF_PHY_IO_DQS_SEL_ADS0 (1<<9)
#define PSRAM_PHY_RF_PHY_IO_DQM_SEL_ADS0 (1<<8)
#define PSRAM_PHY_RF_PHY_IO_D7_SEL_ADS0 (1<<7)
#define PSRAM_PHY_RF_PHY_IO_D6_SEL_ADS0 (1<<6)
#define PSRAM_PHY_RF_PHY_IO_D5_SEL_ADS0 (1<<5)
#define PSRAM_PHY_RF_PHY_IO_D4_SEL_ADS0 (1<<4)
#define PSRAM_PHY_RF_PHY_IO_D3_SEL_ADS0 (1<<3)
#define PSRAM_PHY_RF_PHY_IO_D2_SEL_ADS0 (1<<2)
#define PSRAM_PHY_RF_PHY_IO_D1_SEL_ADS0 (1<<1)
#define PSRAM_PHY_RF_PHY_IO_D0_SEL_ADS0 (1<<0)

//psram_rf_cfg_phy_iomux_ie_wr_ads0
#define PSRAM_PHY_RF_PHY_IO_CSN_IE_ADS0 (1<<20)
#define PSRAM_PHY_RF_PHY_IO_CLK_IE_ADS0 (1<<16)
#define PSRAM_PHY_RF_PHY_IO_DQS_IE_ADS0 (1<<9)
#define PSRAM_PHY_RF_PHY_IO_DQM_IE_ADS0 (1<<8)
#define PSRAM_PHY_RF_PHY_IO_D7_IE_ADS0 (1<<7)
#define PSRAM_PHY_RF_PHY_IO_D6_IE_ADS0 (1<<6)
#define PSRAM_PHY_RF_PHY_IO_D5_IE_ADS0 (1<<5)
#define PSRAM_PHY_RF_PHY_IO_D4_IE_ADS0 (1<<4)
#define PSRAM_PHY_RF_PHY_IO_D3_IE_ADS0 (1<<3)
#define PSRAM_PHY_RF_PHY_IO_D2_IE_ADS0 (1<<2)
#define PSRAM_PHY_RF_PHY_IO_D1_IE_ADS0 (1<<1)
#define PSRAM_PHY_RF_PHY_IO_D0_IE_ADS0 (1<<0)

//psram_rf_cfg_phy_iomux_oe_wr_ads0
#define PSRAM_PHY_RF_PHY_IO_CSN_OE_ADS0 (1<<20)
#define PSRAM_PHY_RF_PHY_IO_CLK_OE_ADS0 (1<<16)
#define PSRAM_PHY_RF_PHY_IO_DQS_OE_ADS0 (1<<9)
#define PSRAM_PHY_RF_PHY_IO_DQM_OE_ADS0 (1<<8)
#define PSRAM_PHY_RF_PHY_IO_D7_OE_ADS0 (1<<7)
#define PSRAM_PHY_RF_PHY_IO_D6_OE_ADS0 (1<<6)
#define PSRAM_PHY_RF_PHY_IO_D5_OE_ADS0 (1<<5)
#define PSRAM_PHY_RF_PHY_IO_D4_OE_ADS0 (1<<4)
#define PSRAM_PHY_RF_PHY_IO_D3_OE_ADS0 (1<<3)
#define PSRAM_PHY_RF_PHY_IO_D2_OE_ADS0 (1<<2)
#define PSRAM_PHY_RF_PHY_IO_D1_OE_ADS0 (1<<1)
#define PSRAM_PHY_RF_PHY_IO_D0_OE_ADS0 (1<<0)

//psram_rf_cfg_phy_iomux_out_wr_ads0
#define PSRAM_PHY_RF_PHY_IO_CSN_OUT_ADS0 (1<<20)
#define PSRAM_PHY_RF_PHY_IO_CLK_OUT_ADS0 (1<<16)
#define PSRAM_PHY_RF_PHY_IO_DQS_OUT_ADS0 (1<<9)
#define PSRAM_PHY_RF_PHY_IO_DQM_OUT_ADS0 (1<<8)
#define PSRAM_PHY_RF_PHY_IO_D7_OUT_ADS0 (1<<7)
#define PSRAM_PHY_RF_PHY_IO_D6_OUT_ADS0 (1<<6)
#define PSRAM_PHY_RF_PHY_IO_D5_OUT_ADS0 (1<<5)
#define PSRAM_PHY_RF_PHY_IO_D4_OUT_ADS0 (1<<4)
#define PSRAM_PHY_RF_PHY_IO_D3_OUT_ADS0 (1<<3)
#define PSRAM_PHY_RF_PHY_IO_D2_OUT_ADS0 (1<<2)
#define PSRAM_PHY_RF_PHY_IO_D1_OUT_ADS0 (1<<1)
#define PSRAM_PHY_RF_PHY_IO_D0_OUT_ADS0 (1<<0)

//psram_rf_cfg_dll_ads1
#define PSRAM_PHY_RF_DLL_LOCK_WAIT_ADS1(n) (((n)&15)<<28)
#define PSRAM_PHY_RF_DLL_AUTO_ERR_CLR_EN_ADS1 (1<<27)
#define PSRAM_PHY_RF_DLL_PD_CNT_ADS1(n) (((n)&7)<<24)
#define PSRAM_PHY_RF_DL_CPST_THR_ADS1(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RF_DLL_EN_ADS1   (1<<15)
#define PSRAM_PHY_RF_DLL_CLK_SEL_ADS1 (1<<14)
#define PSRAM_PHY_RF_DLL_ERR_CLR_ADS1 (1<<13)
#define PSRAM_PHY_RF_DL_CPST_AUTO_REF_EN_ADS1 (1<<12)
#define PSRAM_PHY_RF_DL_CPST_START_ADS1 (1<<11)
#define PSRAM_PHY_RF_DL_CPST_EN_ADS1 (1<<10)
#define PSRAM_PHY_RF_DLL_AUTO_CLR_EN_ADS1 (1<<9)
#define PSRAM_PHY_RF_DLL_CLR_ADS1  (1<<8)

//psram_rfdll_status_dll_ads1
#define PSRAM_PHY_RFDLL_ERROR_ADS1 (1<<29)
#define PSRAM_PHY_RFDLL_LOCKED_ADS1 (1<<28)
#define PSRAM_PHY_RFDLL_ST_ADS1(n) (((n)&7)<<25)
#define PSRAM_PHY_RFDL_CPST_ST_ADS1 (1<<24)
#define PSRAM_PHY_RFDLL_CNT_ADS1(n) (((n)&0xFF)<<0)

//psram_rf_cfg_dll_dl_0_wr_ads1
#define PSRAM_PHY_RF_CLKWR_DL_CPST_EN_ADS1 (1<<31)
#define PSRAM_PHY_RF_CLKWR_DL_CPST_MINUS_ADS1 (1<<30)
#define PSRAM_PHY_RF_CLKWR_QTR_DL_CPST_OFFSET_ADS1(n) (((n)&3)<<28)
#define PSRAM_PHY_RFDL_CLKWR_QTR_CNT_ADS1(n) (((n)&3)<<26)
#define PSRAM_PHY_RF_CLKWR_QTR_DL_SEL_ADS1(n) (((n)&3)<<24)
#define PSRAM_PHY_RF_CLKWR_RAW_DL_CPST_OFFSET_ADS1(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDL_CLKWR_RAW_CNT_ADS1(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RF_CLKWR_RAW_DL_SEL_ADS1(n) (((n)&0xFF)<<0)

//psram_rf_cfg_dll_dl_1_wr_ads1
#define PSRAM_PHY_RF_DQS_IN_POS_DL_CPST_EN_ADS1 (1<<31)
#define PSRAM_PHY_RF_DQS_IN_POS_DL_CPST_MINUS_ADS1 (1<<30)
#define PSRAM_PHY_RF_DQS_IN_POS_QTR_DL_CPST_OFFSET_ADS1(n) (((n)&3)<<28)
#define PSRAM_PHY_RFDL_DQS_IN_POS_QTR_CNT_ADS1(n) (((n)&3)<<26)
#define PSRAM_PHY_RF_DQS_IN_POS_QTR_DL_SEL_ADS1(n) (((n)&3)<<24)
#define PSRAM_PHY_RF_DQS_IN_POS_RAW_DL_CPST_OFFSET_ADS1(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDL_DQS_IN_POS_RAW_CNT_ADS1(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RF_DQS_IN_POS_RAW_DL_SEL_ADS1(n) (((n)&0xFF)<<0)

//psram_rf_cfg_dll_dl_2_wr_ads1
#define PSRAM_PHY_RF_DQS_IN_NEG_DL_CPST_EN_ADS1 (1<<31)
#define PSRAM_PHY_RF_DQS_IN_NEG_DL_CPST_MINUS_ADS1 (1<<30)
#define PSRAM_PHY_RF_DQS_IN_NEG_QTR_DL_CPST_OFFSET_ADS1(n) (((n)&3)<<28)
#define PSRAM_PHY_RFDL_DQS_IN_NEG_QTR_CNT_ADS1(n) (((n)&3)<<26)
#define PSRAM_PHY_RF_DQS_IN_NEG_QTR_DL_SEL_ADS1(n) (((n)&3)<<24)
#define PSRAM_PHY_RF_DQS_IN_NEG_RAW_DL_CPST_OFFSET_ADS1(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDL_DQS_IN_NEG_RAW_CNT_ADS1(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RF_DQS_IN_NEG_RAW_DL_SEL_ADS1(n) (((n)&0xFF)<<0)

//psram_rf_cfg_dll_dl_3_wr_ads1
#define PSRAM_PHY_RF_DQS_GATE_DL_CPST_EN_ADS1 (1<<31)
#define PSRAM_PHY_RF_DQS_GATE_DL_CPST_MINUS_ADS1 (1<<30)
#define PSRAM_PHY_RF_DQS_GATE_QTR_DL_CPST_OFFSET_ADS1(n) (((n)&3)<<28)
#define PSRAM_PHY_RFDL_DQS_GATE_QTR_CNT_ADS1(n) (((n)&3)<<26)
#define PSRAM_PHY_RF_DQS_GATE_QTR_DL_SEL_ADS1(n) (((n)&3)<<24)
#define PSRAM_PHY_RF_DQS_GATE_RAW_DL_CPST_OFFSET_ADS1(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDL_DQS_GATE_RAW_CNT_ADS1(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RF_DQS_GATE_RAW_DL_SEL_ADS1(n) (((n)&0xFF)<<0)

//psram_rf_cfg_dll_dl_4_wr_ads1
#define PSRAM_PHY_RF_DLY_OUT_CEN_DL_SEL_ADS1(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_OUT_CLK_DL_SEL_ADS1(n) (((n)&31)<<0)

//psram_rf_cfg_dll_dl_5_wr_ads1
#define PSRAM_PHY_RF_DLY_OUT_D3_DL_SEL_ADS1(n) (((n)&31)<<24)
#define PSRAM_PHY_RF_DLY_OUT_D2_DL_SEL_ADS1(n) (((n)&31)<<16)
#define PSRAM_PHY_RF_DLY_OUT_D1_DL_SEL_ADS1(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_OUT_D0_DL_SEL_ADS1(n) (((n)&31)<<0)

//psram_rf_cfg_dll_dl_6_wr_ads1
#define PSRAM_PHY_RF_DLY_OUT_D7_DL_SEL_ADS1(n) (((n)&31)<<24)
#define PSRAM_PHY_RF_DLY_OUT_D6_DL_SEL_ADS1(n) (((n)&31)<<16)
#define PSRAM_PHY_RF_DLY_OUT_D5_DL_SEL_ADS1(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_OUT_D4_DL_SEL_ADS1(n) (((n)&31)<<0)

//psram_rf_cfg_dll_dl_7_wr_ads1
#define PSRAM_PHY_RF_DLY_IN_D3_DL_SEL_ADS1(n) (((n)&31)<<24)
#define PSRAM_PHY_RF_DLY_IN_D2_DL_SEL_ADS1(n) (((n)&31)<<16)
#define PSRAM_PHY_RF_DLY_IN_D1_DL_SEL_ADS1(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_IN_D0_DL_SEL_ADS1(n) (((n)&31)<<0)

//psram_rf_cfg_dll_dl_8_wr_ads1
#define PSRAM_PHY_RF_DLY_IN_D7_DL_SEL_ADS1(n) (((n)&31)<<24)
#define PSRAM_PHY_RF_DLY_IN_D6_DL_SEL_ADS1(n) (((n)&31)<<16)
#define PSRAM_PHY_RF_DLY_IN_D5_DL_SEL_ADS1(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_IN_D4_DL_SEL_ADS1(n) (((n)&31)<<0)

//psram_rf_cfg_dll_dl_9_wr_ads1
#define PSRAM_PHY_RF_DLY_IN_DQS_DL_SEL_ADS1(n) (((n)&31)<<16)
#define PSRAM_PHY_RF_DLY_OUT_DQM_DL_SEL_ADS1(n) (((n)&31)<<8)
#define PSRAM_PHY_RF_DLY_OUT_DQS_DL_SEL_ADS1(n) (((n)&31)<<0)

//psram_rfdll_status_max_cnt_ads1
#define PSRAM_PHY_RFDLL_MAX_CNT_F3_ADS1(n) (((n)&0xFF)<<24)
#define PSRAM_PHY_RFDLL_MAX_CNT_F2_ADS1(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDLL_MAX_CNT_F1_ADS1(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RFDLL_MAX_CNT_F0_ADS1(n) (((n)&0xFF)<<0)

//psram_rfdll_status_min_cnt_ads1
#define PSRAM_PHY_RFDLL_MIN_CNT_F3_ADS1(n) (((n)&0xFF)<<24)
#define PSRAM_PHY_RFDLL_MIN_CNT_F2_ADS1(n) (((n)&0xFF)<<16)
#define PSRAM_PHY_RFDLL_MIN_CNT_F1_ADS1(n) (((n)&0xFF)<<8)
#define PSRAM_PHY_RFDLL_MIN_CNT_F0_ADS1(n) (((n)&0xFF)<<0)

//psram_rf_cfg_phy_iomux_sel_wr_ads1
#define PSRAM_PHY_RF_PHY_IO_CSN_SEL_ADS1 (1<<20)
#define PSRAM_PHY_RF_PHY_IO_CLK_SEL_ADS1 (1<<16)
#define PSRAM_PHY_RF_PHY_IO_DQS_SEL_ADS1 (1<<9)
#define PSRAM_PHY_RF_PHY_IO_DQM_SEL_ADS1 (1<<8)
#define PSRAM_PHY_RF_PHY_IO_D7_SEL_ADS1 (1<<7)
#define PSRAM_PHY_RF_PHY_IO_D6_SEL_ADS1 (1<<6)
#define PSRAM_PHY_RF_PHY_IO_D5_SEL_ADS1 (1<<5)
#define PSRAM_PHY_RF_PHY_IO_D4_SEL_ADS1 (1<<4)
#define PSRAM_PHY_RF_PHY_IO_D3_SEL_ADS1 (1<<3)
#define PSRAM_PHY_RF_PHY_IO_D2_SEL_ADS1 (1<<2)
#define PSRAM_PHY_RF_PHY_IO_D1_SEL_ADS1 (1<<1)
#define PSRAM_PHY_RF_PHY_IO_D0_SEL_ADS1 (1<<0)

//psram_rf_cfg_phy_iomux_ie_wr_ads1
#define PSRAM_PHY_RF_PHY_IO_CSN_IE_ADS1 (1<<20)
#define PSRAM_PHY_RF_PHY_IO_CLK_IE_ADS1 (1<<16)
#define PSRAM_PHY_RF_PHY_IO_DQS_IE_ADS1 (1<<9)
#define PSRAM_PHY_RF_PHY_IO_DQM_IE_ADS1 (1<<8)
#define PSRAM_PHY_RF_PHY_IO_D7_IE_ADS1 (1<<7)
#define PSRAM_PHY_RF_PHY_IO_D6_IE_ADS1 (1<<6)
#define PSRAM_PHY_RF_PHY_IO_D5_IE_ADS1 (1<<5)
#define PSRAM_PHY_RF_PHY_IO_D4_IE_ADS1 (1<<4)
#define PSRAM_PHY_RF_PHY_IO_D3_IE_ADS1 (1<<3)
#define PSRAM_PHY_RF_PHY_IO_D2_IE_ADS1 (1<<2)
#define PSRAM_PHY_RF_PHY_IO_D1_IE_ADS1 (1<<1)
#define PSRAM_PHY_RF_PHY_IO_D0_IE_ADS1 (1<<0)

//psram_rf_cfg_phy_iomux_oe_wr_ads1
#define PSRAM_PHY_RF_PHY_IO_CSN_OE_ADS1 (1<<20)
#define PSRAM_PHY_RF_PHY_IO_CLK_OE_ADS1 (1<<16)
#define PSRAM_PHY_RF_PHY_IO_DQS_OE_ADS1 (1<<9)
#define PSRAM_PHY_RF_PHY_IO_DQM_OE_ADS1 (1<<8)
#define PSRAM_PHY_RF_PHY_IO_D7_OE_ADS1 (1<<7)
#define PSRAM_PHY_RF_PHY_IO_D6_OE_ADS1 (1<<6)
#define PSRAM_PHY_RF_PHY_IO_D5_OE_ADS1 (1<<5)
#define PSRAM_PHY_RF_PHY_IO_D4_OE_ADS1 (1<<4)
#define PSRAM_PHY_RF_PHY_IO_D3_OE_ADS1 (1<<3)
#define PSRAM_PHY_RF_PHY_IO_D2_OE_ADS1 (1<<2)
#define PSRAM_PHY_RF_PHY_IO_D1_OE_ADS1 (1<<1)
#define PSRAM_PHY_RF_PHY_IO_D0_OE_ADS1 (1<<0)

//psram_rf_cfg_phy_iomux_out_wr_ads1
#define PSRAM_PHY_RF_PHY_IO_CSN_OUT_ADS1 (1<<20)
#define PSRAM_PHY_RF_PHY_IO_CLK_OUT_ADS1 (1<<16)
#define PSRAM_PHY_RF_PHY_IO_DQS_OUT_ADS1 (1<<9)
#define PSRAM_PHY_RF_PHY_IO_DQM_OUT_ADS1 (1<<8)
#define PSRAM_PHY_RF_PHY_IO_D7_OUT_ADS1 (1<<7)
#define PSRAM_PHY_RF_PHY_IO_D6_OUT_ADS1 (1<<6)
#define PSRAM_PHY_RF_PHY_IO_D5_OUT_ADS1 (1<<5)
#define PSRAM_PHY_RF_PHY_IO_D4_OUT_ADS1 (1<<4)
#define PSRAM_PHY_RF_PHY_IO_D3_OUT_ADS1 (1<<3)
#define PSRAM_PHY_RF_PHY_IO_D2_OUT_ADS1 (1<<2)
#define PSRAM_PHY_RF_PHY_IO_D1_OUT_ADS1 (1<<1)
#define PSRAM_PHY_RF_PHY_IO_D0_OUT_ADS1 (1<<0)

//psram_drf_cfg
#define PSRAM_PHY_DRF_CLKDMEM_OUT_SEL (1<<0)

//psram_drf_cfg_reg_sel
#define PSRAM_PHY_DRF_REG_SEL(n)   (((n)&3)<<0)

//psram_drf_cfg_dqs_ie_sel_f0
#define PSRAM_PHY_DRF_DQS_IE_SEL_F0(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_oe_sel_f0
#define PSRAM_PHY_DRF_DQS_OE_SEL_F0(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_out_sel_f0
#define PSRAM_PHY_DRF_DQS_OUT_SEL_F0(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_gate_sel_f0
#define PSRAM_PHY_DRF_DQS_GATE_SEL_F0(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_data_ie_sel_f0
#define PSRAM_PHY_DRF_DATA_IE_SEL_F0(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_data_oe_sel_f0
#define PSRAM_PHY_DRF_DATA_OE_SEL_F0(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_ie_sel_f1
#define PSRAM_PHY_DRF_DQS_IE_SEL_F1(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_oe_sel_f1
#define PSRAM_PHY_DRF_DQS_OE_SEL_F1(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_out_sel_f1
#define PSRAM_PHY_DRF_DQS_OUT_SEL_F1(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_gate_sel_f1
#define PSRAM_PHY_DRF_DQS_GATE_SEL_F1(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_data_ie_sel_f1
#define PSRAM_PHY_DRF_DATA_IE_SEL_F1(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_data_oe_sel_f1
#define PSRAM_PHY_DRF_DATA_OE_SEL_F1(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_ie_sel_f2
#define PSRAM_PHY_DRF_DQS_IE_SEL_F2(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_oe_sel_f2
#define PSRAM_PHY_DRF_DQS_OE_SEL_F2(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_out_sel_f2
#define PSRAM_PHY_DRF_DQS_OUT_SEL_F2(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_gate_sel_f2
#define PSRAM_PHY_DRF_DQS_GATE_SEL_F2(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_data_ie_sel_f2
#define PSRAM_PHY_DRF_DATA_IE_SEL_F2(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_data_oe_sel_f2
#define PSRAM_PHY_DRF_DATA_OE_SEL_F2(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_ie_sel_f3
#define PSRAM_PHY_DRF_DQS_IE_SEL_F3(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_oe_sel_f3
#define PSRAM_PHY_DRF_DQS_OE_SEL_F3(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_out_sel_f3
#define PSRAM_PHY_DRF_DQS_OUT_SEL_F3(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dqs_gate_sel_f3
#define PSRAM_PHY_DRF_DQS_GATE_SEL_F3(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_data_ie_sel_f3
#define PSRAM_PHY_DRF_DATA_IE_SEL_F3(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_data_oe_sel_f3
#define PSRAM_PHY_DRF_DATA_OE_SEL_F3(n) (((n)&0xFFFF)<<0)

//psram_drf_cfg_dll_mode_f0
#define PSRAM_PHY_DRF_DLL_SATU_MODE_F0 (1<<2)
#define PSRAM_PHY_DRF_DLL_HALF_MODE_F0 (1<<1)
#define PSRAM_PHY_DRF_DLL_CLK_MODE_F0 (1<<0)

//psram_drf_cfg_dll_cnt_f0
#define PSRAM_PHY_DRF_DLL_AUTO_CNT_F0(n) (((n)&0x3FF)<<20)
#define PSRAM_PHY_DRF_DLL_SATU_CNT_F0(n) (((n)&0x3FF)<<10)
#define PSRAM_PHY_DRF_DLL_INIT_CNT_F0(n) (((n)&0x3FF)<<0)

//psram_drf_cfg_dll_mode_f1
#define PSRAM_PHY_DRF_DLL_SATU_MODE_F1 (1<<2)
#define PSRAM_PHY_DRF_DLL_HALF_MODE_F1 (1<<1)
#define PSRAM_PHY_DRF_DLL_CLK_MODE_F1 (1<<0)

//psram_drf_cfg_dll_cnt_f1
#define PSRAM_PHY_DRF_DLL_AUTO_CNT_F1(n) (((n)&0x3FF)<<20)
#define PSRAM_PHY_DRF_DLL_SATU_CNT_F1(n) (((n)&0x3FF)<<10)
#define PSRAM_PHY_DRF_DLL_INIT_CNT_F1(n) (((n)&0x3FF)<<0)

//psram_drf_cfg_dll_mode_f2
#define PSRAM_PHY_DRF_DLL_SATU_MODE_F2 (1<<2)
#define PSRAM_PHY_DRF_DLL_HALF_MODE_F2 (1<<1)
#define PSRAM_PHY_DRF_DLL_CLK_MODE_F2 (1<<0)

//psram_drf_cfg_dll_cnt_f2
#define PSRAM_PHY_DRF_DLL_AUTO_CNT_F2(n) (((n)&0x3FF)<<20)
#define PSRAM_PHY_DRF_DLL_SATU_CNT_F2(n) (((n)&0x3FF)<<10)
#define PSRAM_PHY_DRF_DLL_INIT_CNT_F2(n) (((n)&0x3FF)<<0)

//psram_drf_cfg_dll_mode_f3
#define PSRAM_PHY_DRF_DLL_SATU_MODE_F3 (1<<2)
#define PSRAM_PHY_DRF_DLL_HALF_MODE_F3 (1<<1)
#define PSRAM_PHY_DRF_DLL_CLK_MODE_F3 (1<<0)

//psram_drf_cfg_dll_cnt_f3
#define PSRAM_PHY_DRF_DLL_AUTO_CNT_F3(n) (((n)&0x3FF)<<20)
#define PSRAM_PHY_DRF_DLL_SATU_CNT_F3(n) (((n)&0x3FF)<<10)
#define PSRAM_PHY_DRF_DLL_INIT_CNT_F3(n) (((n)&0x3FF)<<0)

//psram_drf_format_control
#define PSRAM_PHY_DRF_MEMORY_BURST(n) (((n)&3)<<0)

//psram_drf_t_rcd
#define PSRAM_PHY_DRF_T_RCD(n)     (((n)&15)<<0)

//psram_drf_t_rddata_en
#define PSRAM_PHY_DRF_T_RDDATA_EN(n) (((n)&15)<<0)

//psram_drf_t_phywrlat
#define PSRAM_PHY_DRF_T_PHYWRLAT(n) (((n)&15)<<0)

//psram_drf_t_cph_wr
#define PSRAM_PHY_DRF_T_CPH_WR(n)  (((n)&15)<<0)

//psram_drf_t_cph_rd
#define PSRAM_PHY_DRF_T_CPH_RD(n)  (((n)&7)<<0)

//psram_drf_t_data_oe_ext
#define PSRAM_PHY_DRF_T_DATA_OE_CMD_EXT(n) (((n)&15)<<4)
#define PSRAM_PHY_DRF_T_DATA_OE_WDATA_EXT(n) (((n)&15)<<0)

//psram_drf_t_dqs_oe_ext
#define PSRAM_PHY_DRF_T_DQS_OE_EXT(n) (((n)&15)<<0)

//psram_drf_t_xphs
#define PSRAM_PHY_DRF_T_XPHS(n)    (((n)&31)<<0)

//psram_drf_t_rddata_vld_sync
#define PSRAM_PHY_DRF_T_RDDATA_VLD_SYNC(n) (((n)&7)<<0)

//psram_drf_t_rddata_late
#define PSRAM_PHY_DRF_T_RDDATA_LATE(n) (((n)&31)<<0)

//psram_drf_train_cfg
#define PSRAM_PHY_DRF_DMC_RDLVL_GATE_EN (1<<21)
#define PSRAM_PHY_DRF_PHY_RDLVL_GATE_EN (1<<20)
#define PSRAM_PHY_DRF_DMC_RDLVL_EN (1<<17)
#define PSRAM_PHY_DRF_PHY_RDLVL_EN (1<<16)
#define PSRAM_PHY_DRF_DMC_WRLVL_EN (1<<13)
#define PSRAM_PHY_DRF_PHY_WRLVL_EN (1<<12)
#define PSRAM_PHY_DRF_PHYUPD_TYPE_3(n) (((n)&3)<<10)
#define PSRAM_PHY_DRF_PHYUPD_TYPE_2(n) (((n)&3)<<8)
#define PSRAM_PHY_DRF_PHYUPD_TYPE_1(n) (((n)&3)<<6)
#define PSRAM_PHY_DRF_PHYUPD_TYPE_0(n) (((n)&3)<<4)
#define PSRAM_PHY_DRF_PHYUPD_TYPE_SEL(n) (((n)&3)<<1)
#define PSRAM_PHY_DRF_PHYUPD_EN    (1<<0)

//psram_drf_mr_data_en
#define PSRAM_PHY_DRF_MR_DATA_EN   (1<<0)

//psram_drf_mr_data_0
#define PSRAM_PHY_DRF_MR_DATA_0(n) (((n)&0xFFFFFFFF)<<0)

//psram_drf_mr_data_1
#define PSRAM_PHY_DRF_MR_DATA_1(n) (((n)&0xFFFFFFFF)<<0)

//psram_rf_irq_ctrl
#define PSRAM_PHY_RF_IRQ_EN_DISC_RD_ADS1 (1<<20)
#define PSRAM_PHY_RF_IRQ_EN_DISC_WR_ADS1 (1<<19)
#define PSRAM_PHY_RF_IRQ_EN_DISC_MRR_ADS1 (1<<18)
#define PSRAM_PHY_RF_IRQ_EN_DISC_MRW_ADS1 (1<<17)
#define PSRAM_PHY_RF_IRQ_EN_DISC_RST_ADS1 (1<<16)
#define PSRAM_PHY_RF_IRQ_EN_DISC_RD_ADS0 (1<<12)
#define PSRAM_PHY_RF_IRQ_EN_DISC_WR_ADS0 (1<<11)
#define PSRAM_PHY_RF_IRQ_EN_DISC_MRR_ADS0 (1<<10)
#define PSRAM_PHY_RF_IRQ_EN_DISC_MRW_ADS0 (1<<9)
#define PSRAM_PHY_RF_IRQ_EN_DISC_RST_ADS0 (1<<8)
#define PSRAM_PHY_RF_IRQ_EN_RDDATA_TIMEOUT_ADS1 (1<<5)
#define PSRAM_PHY_RF_IRQ_EN_RDDATA_TIMEOUT_ADS0 (1<<4)
#define PSRAM_PHY_RF_IRQ_EN_DLL_UNLOCK_ADS1 (1<<1)
#define PSRAM_PHY_RF_IRQ_EN_DLL_UNLOCK_ADS0 (1<<0)

//psram_rf_irq_status_clr
#define PSRAM_PHY_RF_IRQ_ST_CLR_DISC_RD_ADS1 (1<<20)
#define PSRAM_PHY_RF_IRQ_ST_CLR_DISC_WR_ADS1 (1<<19)
#define PSRAM_PHY_RF_IRQ_ST_CLR_DISC_MRR_ADS1 (1<<18)
#define PSRAM_PHY_RF_IRQ_ST_CLR_DISC_MRW_ADS1 (1<<17)
#define PSRAM_PHY_RF_IRQ_ST_CLR_DISC_RST_ADS1 (1<<16)
#define PSRAM_PHY_RF_IRQ_ST_CLR_DISC_RD_ADS0 (1<<12)
#define PSRAM_PHY_RF_IRQ_ST_CLR_DISC_WR_ADS0 (1<<11)
#define PSRAM_PHY_RF_IRQ_ST_CLR_DISC_MRR_ADS0 (1<<10)
#define PSRAM_PHY_RF_IRQ_ST_CLR_DISC_MRW_ADS0 (1<<9)
#define PSRAM_PHY_RF_IRQ_ST_CLR_DISC_RST_ADS0 (1<<8)
#define PSRAM_PHY_RF_IRQ_ST_CLR_RDDATA_TIMEOUT_ADS1 (1<<5)
#define PSRAM_PHY_RF_IRQ_ST_CLR_RDDATA_TIMEOUT_ADS0 (1<<4)
#define PSRAM_PHY_RF_IRQ_ST_CLR_DLL_UNLOCK_ADS1 (1<<1)
#define PSRAM_PHY_RF_IRQ_ST_CLR_DLL_UNLOCK_ADS0 (1<<0)

//psram_rf_irq_status
#define PSRAM_PHY_RF_IRQ_ST_DISC_RD_ADS1 (1<<20)
#define PSRAM_PHY_RF_IRQ_ST_DISC_WR_ADS1 (1<<19)
#define PSRAM_PHY_RF_IRQ_ST_DISC_MRR_ADS1 (1<<18)
#define PSRAM_PHY_RF_IRQ_ST_DISC_MRW_ADS1 (1<<17)
#define PSRAM_PHY_RF_IRQ_ST_DISC_RST_ADS1 (1<<16)
#define PSRAM_PHY_RF_IRQ_ST_DISC_RD_ADS0 (1<<12)
#define PSRAM_PHY_RF_IRQ_ST_DISC_WR_ADS0 (1<<11)
#define PSRAM_PHY_RF_IRQ_ST_DISC_MRR_ADS0 (1<<10)
#define PSRAM_PHY_RF_IRQ_ST_DISC_MRW_ADS0 (1<<9)
#define PSRAM_PHY_RF_IRQ_ST_DISC_RST_ADS0 (1<<8)
#define PSRAM_PHY_RF_IRQ_ST_RDDATA_TIMEOUT_ADS1 (1<<5)
#define PSRAM_PHY_RF_IRQ_ST_RDDATA_TIMEOUT_ADS0 (1<<4)
#define PSRAM_PHY_RF_IRQ_ST_DLL_UNLOCK_ADS1 (1<<1)
#define PSRAM_PHY_RF_IRQ_ST_DLL_UNLOCK_ADS0 (1<<0)

//psram_rf_irq_cnt_clr
#define PSRAM_PHY_RF_IRQ_CNT_CLR_DLL_UNLOCK_ADS1 (1<<1)
#define PSRAM_PHY_RF_IRQ_CNT_CLR_DLL_UNLOCK_ADS0 (1<<0)

//psram_rf_irq_cnt_dll_unlock_ads0
#define PSRAM_PHY_RF_IRQ_CNT_OVERFLOW_DLL_UNLOCK_ADS0 (1<<31)
#define PSRAM_PHY_RF_IRQ_CNT_DLL_UNLOCK_ADS0(n) (((n)&0x7FFFFFFF)<<0)

//psram_rf_irq_cnt_dll_unlock_ads1
#define PSRAM_PHY_RF_IRQ_CNT_OVERFLOW_DLL_UNLOCK_ADS1 (1<<31)
#define PSRAM_PHY_RF_IRQ_CNT_DLL_UNLOCK_ADS1(n) (((n)&0x7FFFFFFF)<<0)





#endif


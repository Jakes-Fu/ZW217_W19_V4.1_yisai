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
    REG32                          psram_drf_t_rddata_valid_early; //0x0000052C
    REG32 Reserved_00000530[52];                //0x00000530
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
typedef union {
    REG32 v;
    struct {
        REG32 rf_phy_en : 1; // [0]
        REG32 rf_phy_init_complete : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_PHY_T;

//psram_rf_cfg_clock_gate
typedef union {
    REG32 v;
    struct {
        REG32 rf_clk_gate_en : 1; // [0]
        REG32 rf_clk_gate_fg_en : 1; // [1]
        REG32 rf_clk_gate_ag_en : 1; // [2]
        REG32 rf_clk_gate_ag_wr_en : 1; // [3]
        REG32 rf_clk_gate_ag_rd_en : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_CLOCK_GATE_T;

//psram_rf_cfg_lpi
typedef union {
    REG32 v;
    struct {
        REG32 rf_lpi_sel_m0 : 1; // [0]
        REG32 rf_cwakeup_m0 : 1; // [1]
        REG32 rf_cwakeup_s0 : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_LPI_T;

//psram_rfdll_cfg_dll
typedef union {
    REG32 v;
    struct {
        REG32 rfdll_reset : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_PSRAM_PHY_PSRAM_RFDLL_CFG_DLL_T;

//psram_rfdll_status_cpst_idle
typedef union {
    REG32 v;
    struct {
        REG32 rfdl_cpst_st_idle_ads0 : 1; // [0], read only
        REG32 rfdl_cpst_st_idle_ads1 : 1; // [1], read only
        REG32 __31_2 : 30;
    } b;
} REG_PSRAM_PHY_PSRAM_RFDLL_STATUS_CPST_IDLE_T;

//psram_rf_status_phy_data_in
typedef union {
    REG32 v;
    struct {
        REG32 rf_phy_data_in : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_STATUS_PHY_DATA_IN_T;

//psram_rf_cfg_dll_ads0
typedef union {
    REG32 v;
    struct {
        REG32 __7_0 : 8;
        REG32 rf_dll_clr_ads0 : 1; // [8]
        REG32 rf_dll_auto_clr_en_ads0 : 1; // [9]
        REG32 rf_dl_cpst_en_ads0 : 1; // [10]
        REG32 rf_dl_cpst_start_ads0 : 1; // [11]
        REG32 rf_dl_cpst_auto_ref_en_ads0 : 1; // [12]
        REG32 rf_dll_err_clr_ads0 : 1; // [13]
        REG32 rf_dll_clk_sel_ads0 : 1; // [14]
        REG32 rf_dll_en_ads0 : 1; // [15]
        REG32 rf_dl_cpst_thr_ads0 : 8; // [23:16]
        REG32 rf_dll_pd_cnt_ads0 : 3; // [26:24]
        REG32 rf_dll_auto_err_clr_en_ads0 : 1; // [27]
        REG32 rf_dll_lock_wait_ads0 : 4; // [31:28]
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_ADS0_T;

//psram_rfdll_status_dll_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rfdll_cnt_ads0 : 8; // [7:0], read only
        REG32 __23_8 : 16;
        REG32 rfdl_cpst_st_ads0 : 1; // [24], read only
        REG32 rfdll_st_ads0 : 3; // [27:25], read only
        REG32 rfdll_locked_ads0 : 1; // [28], read only
        REG32 rfdll_error_ads0 : 1; // [29], read only
        REG32 __31_30 : 2;
    } b;
} REG_PSRAM_PHY_PSRAM_RFDLL_STATUS_DLL_ADS0_T;

//psram_rf_cfg_dll_dl_0_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_clkwr_raw_dl_sel_ads0 : 8; // [7:0]
        REG32 rfdl_clkwr_raw_cnt_ads0 : 8; // [15:8], read only
        REG32 rf_clkwr_raw_dl_cpst_offset_ads0 : 8; // [23:16]
        REG32 rf_clkwr_qtr_dl_sel_ads0 : 2; // [25:24]
        REG32 rfdl_clkwr_qtr_cnt_ads0 : 2; // [27:26], read only
        REG32 rf_clkwr_qtr_dl_cpst_offset_ads0 : 2; // [29:28]
        REG32 rf_clkwr_dl_cpst_minus_ads0 : 1; // [30]
        REG32 rf_clkwr_dl_cpst_en_ads0 : 1; // [31]
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_0_WR_ADS0_T;

//psram_rf_cfg_dll_dl_1_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_dqs_in_pos_raw_dl_sel_ads0 : 8; // [7:0]
        REG32 rfdl_dqs_in_pos_raw_cnt_ads0 : 8; // [15:8], read only
        REG32 rf_dqs_in_pos_raw_dl_cpst_offset_ads0 : 8; // [23:16]
        REG32 rf_dqs_in_pos_qtr_dl_sel_ads0 : 2; // [25:24]
        REG32 rfdl_dqs_in_pos_qtr_cnt_ads0 : 2; // [27:26], read only
        REG32 rf_dqs_in_pos_qtr_dl_cpst_offset_ads0 : 2; // [29:28]
        REG32 rf_dqs_in_pos_dl_cpst_minus_ads0 : 1; // [30]
        REG32 rf_dqs_in_pos_dl_cpst_en_ads0 : 1; // [31]
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_1_WR_ADS0_T;

//psram_rf_cfg_dll_dl_2_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_dqs_in_neg_raw_dl_sel_ads0 : 8; // [7:0]
        REG32 rfdl_dqs_in_neg_raw_cnt_ads0 : 8; // [15:8], read only
        REG32 rf_dqs_in_neg_raw_dl_cpst_offset_ads0 : 8; // [23:16]
        REG32 rf_dqs_in_neg_qtr_dl_sel_ads0 : 2; // [25:24]
        REG32 rfdl_dqs_in_neg_qtr_cnt_ads0 : 2; // [27:26], read only
        REG32 rf_dqs_in_neg_qtr_dl_cpst_offset_ads0 : 2; // [29:28]
        REG32 rf_dqs_in_neg_dl_cpst_minus_ads0 : 1; // [30]
        REG32 rf_dqs_in_neg_dl_cpst_en_ads0 : 1; // [31]
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_2_WR_ADS0_T;

//psram_rf_cfg_dll_dl_3_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_dqs_gate_raw_dl_sel_ads0 : 8; // [7:0]
        REG32 rfdl_dqs_gate_raw_cnt_ads0 : 8; // [15:8], read only
        REG32 rf_dqs_gate_raw_dl_cpst_offset_ads0 : 8; // [23:16]
        REG32 rf_dqs_gate_qtr_dl_sel_ads0 : 2; // [25:24]
        REG32 rfdl_dqs_gate_qtr_cnt_ads0 : 2; // [27:26], read only
        REG32 rf_dqs_gate_qtr_dl_cpst_offset_ads0 : 2; // [29:28]
        REG32 rf_dqs_gate_dl_cpst_minus_ads0 : 1; // [30]
        REG32 rf_dqs_gate_dl_cpst_en_ads0 : 1; // [31]
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_3_WR_ADS0_T;

//psram_rf_cfg_dll_dl_4_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_out_clk_dl_sel_ads0 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_out_cen_dl_sel_ads0 : 5; // [12:8]
        REG32 __31_13 : 19;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_4_WR_ADS0_T;

//psram_rf_cfg_dll_dl_5_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_out_d0_dl_sel_ads0 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_out_d1_dl_sel_ads0 : 5; // [12:8]
        REG32 __15_13 : 3;
        REG32 rf_dly_out_d2_dl_sel_ads0 : 5; // [20:16]
        REG32 __23_21 : 3;
        REG32 rf_dly_out_d3_dl_sel_ads0 : 5; // [28:24]
        REG32 __31_29 : 3;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_5_WR_ADS0_T;

//psram_rf_cfg_dll_dl_6_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_out_d4_dl_sel_ads0 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_out_d5_dl_sel_ads0 : 5; // [12:8]
        REG32 __15_13 : 3;
        REG32 rf_dly_out_d6_dl_sel_ads0 : 5; // [20:16]
        REG32 __23_21 : 3;
        REG32 rf_dly_out_d7_dl_sel_ads0 : 5; // [28:24]
        REG32 __31_29 : 3;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_6_WR_ADS0_T;

//psram_rf_cfg_dll_dl_7_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_in_d0_dl_sel_ads0 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_in_d1_dl_sel_ads0 : 5; // [12:8]
        REG32 __15_13 : 3;
        REG32 rf_dly_in_d2_dl_sel_ads0 : 5; // [20:16]
        REG32 __23_21 : 3;
        REG32 rf_dly_in_d3_dl_sel_ads0 : 5; // [28:24]
        REG32 __31_29 : 3;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_7_WR_ADS0_T;

//psram_rf_cfg_dll_dl_8_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_in_d4_dl_sel_ads0 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_in_d5_dl_sel_ads0 : 5; // [12:8]
        REG32 __15_13 : 3;
        REG32 rf_dly_in_d6_dl_sel_ads0 : 5; // [20:16]
        REG32 __23_21 : 3;
        REG32 rf_dly_in_d7_dl_sel_ads0 : 5; // [28:24]
        REG32 __31_29 : 3;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_8_WR_ADS0_T;

//psram_rf_cfg_dll_dl_9_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_out_dqs_dl_sel_ads0 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_out_dqm_dl_sel_ads0 : 5; // [12:8]
        REG32 __15_13 : 3;
        REG32 rf_dly_in_dqs_dl_sel_ads0 : 5; // [20:16]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_9_WR_ADS0_T;

//psram_rfdll_status_max_cnt_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rfdll_max_cnt_f0_ads0 : 8; // [7:0], read only
        REG32 rfdll_max_cnt_f1_ads0 : 8; // [15:8], read only
        REG32 rfdll_max_cnt_f2_ads0 : 8; // [23:16], read only
        REG32 rfdll_max_cnt_f3_ads0 : 8; // [31:24], read only
    } b;
} REG_PSRAM_PHY_PSRAM_RFDLL_STATUS_MAX_CNT_ADS0_T;

//psram_rfdll_status_min_cnt_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rfdll_min_cnt_f0_ads0 : 8; // [7:0], read only
        REG32 rfdll_min_cnt_f1_ads0 : 8; // [15:8], read only
        REG32 rfdll_min_cnt_f2_ads0 : 8; // [23:16], read only
        REG32 rfdll_min_cnt_f3_ads0 : 8; // [31:24], read only
    } b;
} REG_PSRAM_PHY_PSRAM_RFDLL_STATUS_MIN_CNT_ADS0_T;

//psram_rf_cfg_phy_iomux_sel_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_phy_io_d0_sel_ads0 : 1; // [0]
        REG32 rf_phy_io_d1_sel_ads0 : 1; // [1]
        REG32 rf_phy_io_d2_sel_ads0 : 1; // [2]
        REG32 rf_phy_io_d3_sel_ads0 : 1; // [3]
        REG32 rf_phy_io_d4_sel_ads0 : 1; // [4]
        REG32 rf_phy_io_d5_sel_ads0 : 1; // [5]
        REG32 rf_phy_io_d6_sel_ads0 : 1; // [6]
        REG32 rf_phy_io_d7_sel_ads0 : 1; // [7]
        REG32 rf_phy_io_dqm_sel_ads0 : 1; // [8]
        REG32 rf_phy_io_dqs_sel_ads0 : 1; // [9]
        REG32 __15_10 : 6;
        REG32 rf_phy_io_clk_sel_ads0 : 1; // [16]
        REG32 __19_17 : 3;
        REG32 rf_phy_io_csn_sel_ads0 : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_PHY_IOMUX_SEL_WR_ADS0_T;

//psram_rf_cfg_phy_iomux_ie_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_phy_io_d0_ie_ads0 : 1; // [0]
        REG32 rf_phy_io_d1_ie_ads0 : 1; // [1]
        REG32 rf_phy_io_d2_ie_ads0 : 1; // [2]
        REG32 rf_phy_io_d3_ie_ads0 : 1; // [3]
        REG32 rf_phy_io_d4_ie_ads0 : 1; // [4]
        REG32 rf_phy_io_d5_ie_ads0 : 1; // [5]
        REG32 rf_phy_io_d6_ie_ads0 : 1; // [6]
        REG32 rf_phy_io_d7_ie_ads0 : 1; // [7]
        REG32 rf_phy_io_dqm_ie_ads0 : 1; // [8]
        REG32 rf_phy_io_dqs_ie_ads0 : 1; // [9]
        REG32 __15_10 : 6;
        REG32 rf_phy_io_clk_ie_ads0 : 1; // [16]
        REG32 __19_17 : 3;
        REG32 rf_phy_io_csn_ie_ads0 : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_PHY_IOMUX_IE_WR_ADS0_T;

//psram_rf_cfg_phy_iomux_oe_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_phy_io_d0_oe_ads0 : 1; // [0]
        REG32 rf_phy_io_d1_oe_ads0 : 1; // [1]
        REG32 rf_phy_io_d2_oe_ads0 : 1; // [2]
        REG32 rf_phy_io_d3_oe_ads0 : 1; // [3]
        REG32 rf_phy_io_d4_oe_ads0 : 1; // [4]
        REG32 rf_phy_io_d5_oe_ads0 : 1; // [5]
        REG32 rf_phy_io_d6_oe_ads0 : 1; // [6]
        REG32 rf_phy_io_d7_oe_ads0 : 1; // [7]
        REG32 rf_phy_io_dqm_oe_ads0 : 1; // [8]
        REG32 rf_phy_io_dqs_oe_ads0 : 1; // [9]
        REG32 __15_10 : 6;
        REG32 rf_phy_io_clk_oe_ads0 : 1; // [16]
        REG32 __19_17 : 3;
        REG32 rf_phy_io_csn_oe_ads0 : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_PHY_IOMUX_OE_WR_ADS0_T;

//psram_rf_cfg_phy_iomux_out_wr_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_phy_io_d0_out_ads0 : 1; // [0]
        REG32 rf_phy_io_d1_out_ads0 : 1; // [1]
        REG32 rf_phy_io_d2_out_ads0 : 1; // [2]
        REG32 rf_phy_io_d3_out_ads0 : 1; // [3]
        REG32 rf_phy_io_d4_out_ads0 : 1; // [4]
        REG32 rf_phy_io_d5_out_ads0 : 1; // [5]
        REG32 rf_phy_io_d6_out_ads0 : 1; // [6]
        REG32 rf_phy_io_d7_out_ads0 : 1; // [7]
        REG32 rf_phy_io_dqm_out_ads0 : 1; // [8]
        REG32 rf_phy_io_dqs_out_ads0 : 1; // [9]
        REG32 __15_10 : 6;
        REG32 rf_phy_io_clk_out_ads0 : 1; // [16]
        REG32 __19_17 : 3;
        REG32 rf_phy_io_csn_out_ads0 : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_PHY_IOMUX_OUT_WR_ADS0_T;

//psram_rf_cfg_dll_ads1
typedef union {
    REG32 v;
    struct {
        REG32 __7_0 : 8;
        REG32 rf_dll_clr_ads1 : 1; // [8]
        REG32 rf_dll_auto_clr_en_ads1 : 1; // [9]
        REG32 rf_dl_cpst_en_ads1 : 1; // [10]
        REG32 rf_dl_cpst_start_ads1 : 1; // [11]
        REG32 rf_dl_cpst_auto_ref_en_ads1 : 1; // [12]
        REG32 rf_dll_err_clr_ads1 : 1; // [13]
        REG32 rf_dll_clk_sel_ads1 : 1; // [14]
        REG32 rf_dll_en_ads1 : 1; // [15]
        REG32 rf_dl_cpst_thr_ads1 : 8; // [23:16]
        REG32 rf_dll_pd_cnt_ads1 : 3; // [26:24]
        REG32 rf_dll_auto_err_clr_en_ads1 : 1; // [27]
        REG32 rf_dll_lock_wait_ads1 : 4; // [31:28]
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_ADS1_T;

//psram_rfdll_status_dll_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rfdll_cnt_ads1 : 8; // [7:0], read only
        REG32 __23_8 : 16;
        REG32 rfdl_cpst_st_ads1 : 1; // [24], read only
        REG32 rfdll_st_ads1 : 3; // [27:25], read only
        REG32 rfdll_locked_ads1 : 1; // [28], read only
        REG32 rfdll_error_ads1 : 1; // [29], read only
        REG32 __31_30 : 2;
    } b;
} REG_PSRAM_PHY_PSRAM_RFDLL_STATUS_DLL_ADS1_T;

//psram_rf_cfg_dll_dl_0_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_clkwr_raw_dl_sel_ads1 : 8; // [7:0]
        REG32 rfdl_clkwr_raw_cnt_ads1 : 8; // [15:8], read only
        REG32 rf_clkwr_raw_dl_cpst_offset_ads1 : 8; // [23:16]
        REG32 rf_clkwr_qtr_dl_sel_ads1 : 2; // [25:24]
        REG32 rfdl_clkwr_qtr_cnt_ads1 : 2; // [27:26], read only
        REG32 rf_clkwr_qtr_dl_cpst_offset_ads1 : 2; // [29:28]
        REG32 rf_clkwr_dl_cpst_minus_ads1 : 1; // [30]
        REG32 rf_clkwr_dl_cpst_en_ads1 : 1; // [31]
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_0_WR_ADS1_T;

//psram_rf_cfg_dll_dl_1_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_dqs_in_pos_raw_dl_sel_ads1 : 8; // [7:0]
        REG32 rfdl_dqs_in_pos_raw_cnt_ads1 : 8; // [15:8], read only
        REG32 rf_dqs_in_pos_raw_dl_cpst_offset_ads1 : 8; // [23:16]
        REG32 rf_dqs_in_pos_qtr_dl_sel_ads1 : 2; // [25:24]
        REG32 rfdl_dqs_in_pos_qtr_cnt_ads1 : 2; // [27:26], read only
        REG32 rf_dqs_in_pos_qtr_dl_cpst_offset_ads1 : 2; // [29:28]
        REG32 rf_dqs_in_pos_dl_cpst_minus_ads1 : 1; // [30]
        REG32 rf_dqs_in_pos_dl_cpst_en_ads1 : 1; // [31]
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_1_WR_ADS1_T;

//psram_rf_cfg_dll_dl_2_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_dqs_in_neg_raw_dl_sel_ads1 : 8; // [7:0]
        REG32 rfdl_dqs_in_neg_raw_cnt_ads1 : 8; // [15:8], read only
        REG32 rf_dqs_in_neg_raw_dl_cpst_offset_ads1 : 8; // [23:16]
        REG32 rf_dqs_in_neg_qtr_dl_sel_ads1 : 2; // [25:24]
        REG32 rfdl_dqs_in_neg_qtr_cnt_ads1 : 2; // [27:26], read only
        REG32 rf_dqs_in_neg_qtr_dl_cpst_offset_ads1 : 2; // [29:28]
        REG32 rf_dqs_in_neg_dl_cpst_minus_ads1 : 1; // [30]
        REG32 rf_dqs_in_neg_dl_cpst_en_ads1 : 1; // [31]
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_2_WR_ADS1_T;

//psram_rf_cfg_dll_dl_3_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_dqs_gate_raw_dl_sel_ads1 : 8; // [7:0]
        REG32 rfdl_dqs_gate_raw_cnt_ads1 : 8; // [15:8], read only
        REG32 rf_dqs_gate_raw_dl_cpst_offset_ads1 : 8; // [23:16]
        REG32 rf_dqs_gate_qtr_dl_sel_ads1 : 2; // [25:24]
        REG32 rfdl_dqs_gate_qtr_cnt_ads1 : 2; // [27:26], read only
        REG32 rf_dqs_gate_qtr_dl_cpst_offset_ads1 : 2; // [29:28]
        REG32 rf_dqs_gate_dl_cpst_minus_ads1 : 1; // [30]
        REG32 rf_dqs_gate_dl_cpst_en_ads1 : 1; // [31]
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_3_WR_ADS1_T;

//psram_rf_cfg_dll_dl_4_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_out_clk_dl_sel_ads1 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_out_cen_dl_sel_ads1 : 5; // [12:8]
        REG32 __31_13 : 19;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_4_WR_ADS1_T;

//psram_rf_cfg_dll_dl_5_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_out_d0_dl_sel_ads1 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_out_d1_dl_sel_ads1 : 5; // [12:8]
        REG32 __15_13 : 3;
        REG32 rf_dly_out_d2_dl_sel_ads1 : 5; // [20:16]
        REG32 __23_21 : 3;
        REG32 rf_dly_out_d3_dl_sel_ads1 : 5; // [28:24]
        REG32 __31_29 : 3;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_5_WR_ADS1_T;

//psram_rf_cfg_dll_dl_6_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_out_d4_dl_sel_ads1 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_out_d5_dl_sel_ads1 : 5; // [12:8]
        REG32 __15_13 : 3;
        REG32 rf_dly_out_d6_dl_sel_ads1 : 5; // [20:16]
        REG32 __23_21 : 3;
        REG32 rf_dly_out_d7_dl_sel_ads1 : 5; // [28:24]
        REG32 __31_29 : 3;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_6_WR_ADS1_T;

//psram_rf_cfg_dll_dl_7_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_in_d0_dl_sel_ads1 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_in_d1_dl_sel_ads1 : 5; // [12:8]
        REG32 __15_13 : 3;
        REG32 rf_dly_in_d2_dl_sel_ads1 : 5; // [20:16]
        REG32 __23_21 : 3;
        REG32 rf_dly_in_d3_dl_sel_ads1 : 5; // [28:24]
        REG32 __31_29 : 3;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_7_WR_ADS1_T;

//psram_rf_cfg_dll_dl_8_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_in_d4_dl_sel_ads1 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_in_d5_dl_sel_ads1 : 5; // [12:8]
        REG32 __15_13 : 3;
        REG32 rf_dly_in_d6_dl_sel_ads1 : 5; // [20:16]
        REG32 __23_21 : 3;
        REG32 rf_dly_in_d7_dl_sel_ads1 : 5; // [28:24]
        REG32 __31_29 : 3;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_8_WR_ADS1_T;

//psram_rf_cfg_dll_dl_9_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_dly_out_dqs_dl_sel_ads1 : 5; // [4:0]
        REG32 __7_5 : 3;
        REG32 rf_dly_out_dqm_dl_sel_ads1 : 5; // [12:8]
        REG32 __15_13 : 3;
        REG32 rf_dly_in_dqs_dl_sel_ads1 : 5; // [20:16]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_DLL_DL_9_WR_ADS1_T;

//psram_rfdll_status_max_cnt_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rfdll_max_cnt_f0_ads1 : 8; // [7:0], read only
        REG32 rfdll_max_cnt_f1_ads1 : 8; // [15:8], read only
        REG32 rfdll_max_cnt_f2_ads1 : 8; // [23:16], read only
        REG32 rfdll_max_cnt_f3_ads1 : 8; // [31:24], read only
    } b;
} REG_PSRAM_PHY_PSRAM_RFDLL_STATUS_MAX_CNT_ADS1_T;

//psram_rfdll_status_min_cnt_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rfdll_min_cnt_f0_ads1 : 8; // [7:0], read only
        REG32 rfdll_min_cnt_f1_ads1 : 8; // [15:8], read only
        REG32 rfdll_min_cnt_f2_ads1 : 8; // [23:16], read only
        REG32 rfdll_min_cnt_f3_ads1 : 8; // [31:24], read only
    } b;
} REG_PSRAM_PHY_PSRAM_RFDLL_STATUS_MIN_CNT_ADS1_T;

//psram_rf_cfg_phy_iomux_sel_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_phy_io_d0_sel_ads1 : 1; // [0]
        REG32 rf_phy_io_d1_sel_ads1 : 1; // [1]
        REG32 rf_phy_io_d2_sel_ads1 : 1; // [2]
        REG32 rf_phy_io_d3_sel_ads1 : 1; // [3]
        REG32 rf_phy_io_d4_sel_ads1 : 1; // [4]
        REG32 rf_phy_io_d5_sel_ads1 : 1; // [5]
        REG32 rf_phy_io_d6_sel_ads1 : 1; // [6]
        REG32 rf_phy_io_d7_sel_ads1 : 1; // [7]
        REG32 rf_phy_io_dqm_sel_ads1 : 1; // [8]
        REG32 rf_phy_io_dqs_sel_ads1 : 1; // [9]
        REG32 __15_10 : 6;
        REG32 rf_phy_io_clk_sel_ads1 : 1; // [16]
        REG32 __19_17 : 3;
        REG32 rf_phy_io_csn_sel_ads1 : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_PHY_IOMUX_SEL_WR_ADS1_T;

//psram_rf_cfg_phy_iomux_ie_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_phy_io_d0_ie_ads1 : 1; // [0]
        REG32 rf_phy_io_d1_ie_ads1 : 1; // [1]
        REG32 rf_phy_io_d2_ie_ads1 : 1; // [2]
        REG32 rf_phy_io_d3_ie_ads1 : 1; // [3]
        REG32 rf_phy_io_d4_ie_ads1 : 1; // [4]
        REG32 rf_phy_io_d5_ie_ads1 : 1; // [5]
        REG32 rf_phy_io_d6_ie_ads1 : 1; // [6]
        REG32 rf_phy_io_d7_ie_ads1 : 1; // [7]
        REG32 rf_phy_io_dqm_ie_ads1 : 1; // [8]
        REG32 rf_phy_io_dqs_ie_ads1 : 1; // [9]
        REG32 __15_10 : 6;
        REG32 rf_phy_io_clk_ie_ads1 : 1; // [16]
        REG32 __19_17 : 3;
        REG32 rf_phy_io_csn_ie_ads1 : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_PHY_IOMUX_IE_WR_ADS1_T;

//psram_rf_cfg_phy_iomux_oe_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_phy_io_d0_oe_ads1 : 1; // [0]
        REG32 rf_phy_io_d1_oe_ads1 : 1; // [1]
        REG32 rf_phy_io_d2_oe_ads1 : 1; // [2]
        REG32 rf_phy_io_d3_oe_ads1 : 1; // [3]
        REG32 rf_phy_io_d4_oe_ads1 : 1; // [4]
        REG32 rf_phy_io_d5_oe_ads1 : 1; // [5]
        REG32 rf_phy_io_d6_oe_ads1 : 1; // [6]
        REG32 rf_phy_io_d7_oe_ads1 : 1; // [7]
        REG32 rf_phy_io_dqm_oe_ads1 : 1; // [8]
        REG32 rf_phy_io_dqs_oe_ads1 : 1; // [9]
        REG32 __15_10 : 6;
        REG32 rf_phy_io_clk_oe_ads1 : 1; // [16]
        REG32 __19_17 : 3;
        REG32 rf_phy_io_csn_oe_ads1 : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_PHY_IOMUX_OE_WR_ADS1_T;

//psram_rf_cfg_phy_iomux_out_wr_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_phy_io_d0_out_ads1 : 1; // [0]
        REG32 rf_phy_io_d1_out_ads1 : 1; // [1]
        REG32 rf_phy_io_d2_out_ads1 : 1; // [2]
        REG32 rf_phy_io_d3_out_ads1 : 1; // [3]
        REG32 rf_phy_io_d4_out_ads1 : 1; // [4]
        REG32 rf_phy_io_d5_out_ads1 : 1; // [5]
        REG32 rf_phy_io_d6_out_ads1 : 1; // [6]
        REG32 rf_phy_io_d7_out_ads1 : 1; // [7]
        REG32 rf_phy_io_dqm_out_ads1 : 1; // [8]
        REG32 rf_phy_io_dqs_out_ads1 : 1; // [9]
        REG32 __15_10 : 6;
        REG32 rf_phy_io_clk_out_ads1 : 1; // [16]
        REG32 __19_17 : 3;
        REG32 rf_phy_io_csn_out_ads1 : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_CFG_PHY_IOMUX_OUT_WR_ADS1_T;

//psram_drf_cfg
typedef union {
    REG32 v;
    struct {
        REG32 drf_clkdmem_out_sel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_T;

//psram_drf_cfg_reg_sel
typedef union {
    REG32 v;
    struct {
        REG32 drf_reg_sel : 2; // [1:0]
        REG32 __31_2 : 30;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_REG_SEL_T;

//psram_drf_cfg_dqs_ie_sel_f0
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_ie_sel_f0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_IE_SEL_F0_T;

//psram_drf_cfg_dqs_oe_sel_f0
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_oe_sel_f0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_OE_SEL_F0_T;

//psram_drf_cfg_dqs_out_sel_f0
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_out_sel_f0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_OUT_SEL_F0_T;

//psram_drf_cfg_dqs_gate_sel_f0
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_gate_sel_f0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_GATE_SEL_F0_T;

//psram_drf_cfg_data_ie_sel_f0
typedef union {
    REG32 v;
    struct {
        REG32 drf_data_ie_sel_f0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DATA_IE_SEL_F0_T;

//psram_drf_cfg_data_oe_sel_f0
typedef union {
    REG32 v;
    struct {
        REG32 drf_data_oe_sel_f0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DATA_OE_SEL_F0_T;

//psram_drf_cfg_dqs_ie_sel_f1
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_ie_sel_f1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_IE_SEL_F1_T;

//psram_drf_cfg_dqs_oe_sel_f1
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_oe_sel_f1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_OE_SEL_F1_T;

//psram_drf_cfg_dqs_out_sel_f1
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_out_sel_f1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_OUT_SEL_F1_T;

//psram_drf_cfg_dqs_gate_sel_f1
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_gate_sel_f1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_GATE_SEL_F1_T;

//psram_drf_cfg_data_ie_sel_f1
typedef union {
    REG32 v;
    struct {
        REG32 drf_data_ie_sel_f1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DATA_IE_SEL_F1_T;

//psram_drf_cfg_data_oe_sel_f1
typedef union {
    REG32 v;
    struct {
        REG32 drf_data_oe_sel_f1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DATA_OE_SEL_F1_T;

//psram_drf_cfg_dqs_ie_sel_f2
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_ie_sel_f2 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_IE_SEL_F2_T;

//psram_drf_cfg_dqs_oe_sel_f2
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_oe_sel_f2 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_OE_SEL_F2_T;

//psram_drf_cfg_dqs_out_sel_f2
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_out_sel_f2 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_OUT_SEL_F2_T;

//psram_drf_cfg_dqs_gate_sel_f2
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_gate_sel_f2 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_GATE_SEL_F2_T;

//psram_drf_cfg_data_ie_sel_f2
typedef union {
    REG32 v;
    struct {
        REG32 drf_data_ie_sel_f2 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DATA_IE_SEL_F2_T;

//psram_drf_cfg_data_oe_sel_f2
typedef union {
    REG32 v;
    struct {
        REG32 drf_data_oe_sel_f2 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DATA_OE_SEL_F2_T;

//psram_drf_cfg_dqs_ie_sel_f3
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_ie_sel_f3 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_IE_SEL_F3_T;

//psram_drf_cfg_dqs_oe_sel_f3
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_oe_sel_f3 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_OE_SEL_F3_T;

//psram_drf_cfg_dqs_out_sel_f3
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_out_sel_f3 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_OUT_SEL_F3_T;

//psram_drf_cfg_dqs_gate_sel_f3
typedef union {
    REG32 v;
    struct {
        REG32 drf_dqs_gate_sel_f3 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DQS_GATE_SEL_F3_T;

//psram_drf_cfg_data_ie_sel_f3
typedef union {
    REG32 v;
    struct {
        REG32 drf_data_ie_sel_f3 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DATA_IE_SEL_F3_T;

//psram_drf_cfg_data_oe_sel_f3
typedef union {
    REG32 v;
    struct {
        REG32 drf_data_oe_sel_f3 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DATA_OE_SEL_F3_T;

//psram_drf_cfg_dll_mode_f0
typedef union {
    REG32 v;
    struct {
        REG32 drf_dll_clk_mode_f0 : 1; // [0]
        REG32 drf_dll_half_mode_f0 : 1; // [1]
        REG32 drf_dll_satu_mode_f0 : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DLL_MODE_F0_T;

//psram_drf_cfg_dll_cnt_f0
typedef union {
    REG32 v;
    struct {
        REG32 drf_dll_init_cnt_f0 : 10; // [9:0]
        REG32 drf_dll_satu_cnt_f0 : 10; // [19:10]
        REG32 drf_dll_auto_cnt_f0 : 10; // [29:20]
        REG32 __31_30 : 2;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DLL_CNT_F0_T;

//psram_drf_cfg_dll_mode_f1
typedef union {
    REG32 v;
    struct {
        REG32 drf_dll_clk_mode_f1 : 1; // [0]
        REG32 drf_dll_half_mode_f1 : 1; // [1]
        REG32 drf_dll_satu_mode_f1 : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DLL_MODE_F1_T;

//psram_drf_cfg_dll_cnt_f1
typedef union {
    REG32 v;
    struct {
        REG32 drf_dll_init_cnt_f1 : 10; // [9:0]
        REG32 drf_dll_satu_cnt_f1 : 10; // [19:10]
        REG32 drf_dll_auto_cnt_f1 : 10; // [29:20]
        REG32 __31_30 : 2;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DLL_CNT_F1_T;

//psram_drf_cfg_dll_mode_f2
typedef union {
    REG32 v;
    struct {
        REG32 drf_dll_clk_mode_f2 : 1; // [0]
        REG32 drf_dll_half_mode_f2 : 1; // [1]
        REG32 drf_dll_satu_mode_f2 : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DLL_MODE_F2_T;

//psram_drf_cfg_dll_cnt_f2
typedef union {
    REG32 v;
    struct {
        REG32 drf_dll_init_cnt_f2 : 10; // [9:0]
        REG32 drf_dll_satu_cnt_f2 : 10; // [19:10]
        REG32 drf_dll_auto_cnt_f2 : 10; // [29:20]
        REG32 __31_30 : 2;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DLL_CNT_F2_T;

//psram_drf_cfg_dll_mode_f3
typedef union {
    REG32 v;
    struct {
        REG32 drf_dll_clk_mode_f3 : 1; // [0]
        REG32 drf_dll_half_mode_f3 : 1; // [1]
        REG32 drf_dll_satu_mode_f3 : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DLL_MODE_F3_T;

//psram_drf_cfg_dll_cnt_f3
typedef union {
    REG32 v;
    struct {
        REG32 drf_dll_init_cnt_f3 : 10; // [9:0]
        REG32 drf_dll_satu_cnt_f3 : 10; // [19:10]
        REG32 drf_dll_auto_cnt_f3 : 10; // [29:20]
        REG32 __31_30 : 2;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_CFG_DLL_CNT_F3_T;

//psram_drf_format_control
typedef union {
    REG32 v;
    struct {
        REG32 drf_memory_burst : 2; // [1:0]
        REG32 __31_2 : 30;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_FORMAT_CONTROL_T;

//psram_drf_t_rcd
typedef union {
    REG32 v;
    struct {
        REG32 drf_t_rcd : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_T_RCD_T;

//psram_drf_t_rddata_en
typedef union {
    REG32 v;
    struct {
        REG32 drf_t_rddata_en : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_T_RDDATA_EN_T;

//psram_drf_t_phywrlat
typedef union {
    REG32 v;
    struct {
        REG32 drf_t_phywrlat : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_T_PHYWRLAT_T;

//psram_drf_t_cph_wr
typedef union {
    REG32 v;
    struct {
        REG32 drf_t_cph_wr : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_T_CPH_WR_T;

//psram_drf_t_cph_rd
typedef union {
    REG32 v;
    struct {
        REG32 drf_t_cph_rd : 3; // [2:0]
        REG32 __3_3 : 1;
        REG32 drf_t_cph_rd_optm : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_T_CPH_RD_T;

//psram_drf_t_data_oe_ext
typedef union {
    REG32 v;
    struct {
        REG32 drf_t_data_oe_wdata_ext : 4; // [3:0]
        REG32 drf_t_data_oe_cmd_ext : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_T_DATA_OE_EXT_T;

//psram_drf_t_dqs_oe_ext
typedef union {
    REG32 v;
    struct {
        REG32 drf_t_dqs_oe_ext : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_T_DQS_OE_EXT_T;

//psram_drf_t_xphs
typedef union {
    REG32 v;
    struct {
        REG32 drf_t_xphs : 5; // [4:0]
        REG32 __31_5 : 27;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_T_XPHS_T;

//psram_drf_t_rddata_vld_sync
typedef union {
    REG32 v;
    struct {
        REG32 drf_t_rddata_vld_sync : 3; // [2:0]
        REG32 __31_3 : 29;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_T_RDDATA_VLD_SYNC_T;

//psram_drf_t_rddata_late
typedef union {
    REG32 v;
    struct {
        REG32 drf_t_rddata_late : 5; // [4:0]
        REG32 __31_5 : 27;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_T_RDDATA_LATE_T;

//psram_drf_t_rddata_valid_early
typedef union {
    REG32 v;
    struct {
        REG32 drf_t_rddata_valid_early : 2; // [1:0]
        REG32 __31_2 : 30;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_T_RDDATA_VALID_EARLY_T;

//psram_drf_train_cfg
typedef union {
    REG32 v;
    struct {
        REG32 drf_phyupd_en : 1; // [0]
        REG32 drf_phyupd_type_sel : 2; // [2:1]
        REG32 __3_3 : 1;
        REG32 drf_phyupd_type_0 : 2; // [5:4]
        REG32 drf_phyupd_type_1 : 2; // [7:6]
        REG32 drf_phyupd_type_2 : 2; // [9:8]
        REG32 drf_phyupd_type_3 : 2; // [11:10]
        REG32 drf_phy_wrlvl_en : 1; // [12]
        REG32 drf_dmc_wrlvl_en : 1; // [13]
        REG32 __15_14 : 2;
        REG32 drf_phy_rdlvl_en : 1; // [16]
        REG32 drf_dmc_rdlvl_en : 1; // [17]
        REG32 __19_18 : 2;
        REG32 drf_phy_rdlvl_gate_en : 1; // [20]
        REG32 drf_dmc_rdlvl_gate_en : 1; // [21]
        REG32 __31_22 : 10;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_TRAIN_CFG_T;

//psram_drf_mr_data_en
typedef union {
    REG32 v;
    struct {
        REG32 drf_mr_data_en : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_PSRAM_PHY_PSRAM_DRF_MR_DATA_EN_T;

//psram_rf_irq_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 rf_irq_en_dll_unlock_ads0 : 1; // [0]
        REG32 rf_irq_en_dll_unlock_ads1 : 1; // [1]
        REG32 __3_2 : 2;
        REG32 rf_irq_en_rddata_timeout_ads0 : 1; // [4]
        REG32 rf_irq_en_rddata_timeout_ads1 : 1; // [5]
        REG32 __7_6 : 2;
        REG32 rf_irq_en_disc_rst_ads0 : 1; // [8]
        REG32 rf_irq_en_disc_mrw_ads0 : 1; // [9]
        REG32 rf_irq_en_disc_mrr_ads0 : 1; // [10]
        REG32 rf_irq_en_disc_wr_ads0 : 1; // [11]
        REG32 rf_irq_en_disc_rd_ads0 : 1; // [12]
        REG32 __15_13 : 3;
        REG32 rf_irq_en_disc_rst_ads1 : 1; // [16]
        REG32 rf_irq_en_disc_mrw_ads1 : 1; // [17]
        REG32 rf_irq_en_disc_mrr_ads1 : 1; // [18]
        REG32 rf_irq_en_disc_wr_ads1 : 1; // [19]
        REG32 rf_irq_en_disc_rd_ads1 : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_IRQ_CTRL_T;

//psram_rf_irq_status_clr
typedef union {
    REG32 v;
    struct {
        REG32 rf_irq_st_clr_dll_unlock_ads0 : 1; // [0]
        REG32 rf_irq_st_clr_dll_unlock_ads1 : 1; // [1]
        REG32 __3_2 : 2;
        REG32 rf_irq_st_clr_rddata_timeout_ads0 : 1; // [4]
        REG32 rf_irq_st_clr_rddata_timeout_ads1 : 1; // [5]
        REG32 __7_6 : 2;
        REG32 rf_irq_st_clr_disc_rst_ads0 : 1; // [8]
        REG32 rf_irq_st_clr_disc_mrw_ads0 : 1; // [9]
        REG32 rf_irq_st_clr_disc_mrr_ads0 : 1; // [10]
        REG32 rf_irq_st_clr_disc_wr_ads0 : 1; // [11]
        REG32 rf_irq_st_clr_disc_rd_ads0 : 1; // [12]
        REG32 __15_13 : 3;
        REG32 rf_irq_st_clr_disc_rst_ads1 : 1; // [16]
        REG32 rf_irq_st_clr_disc_mrw_ads1 : 1; // [17]
        REG32 rf_irq_st_clr_disc_mrr_ads1 : 1; // [18]
        REG32 rf_irq_st_clr_disc_wr_ads1 : 1; // [19]
        REG32 rf_irq_st_clr_disc_rd_ads1 : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_IRQ_STATUS_CLR_T;

//psram_rf_irq_status
typedef union {
    REG32 v;
    struct {
        REG32 rf_irq_st_dll_unlock_ads0 : 1; // [0], read only
        REG32 rf_irq_st_dll_unlock_ads1 : 1; // [1], read only
        REG32 __3_2 : 2;
        REG32 rf_irq_st_rddata_timeout_ads0 : 1; // [4], read only
        REG32 rf_irq_st_rddata_timeout_ads1 : 1; // [5], read only
        REG32 __7_6 : 2;
        REG32 rf_irq_st_disc_rst_ads0 : 1; // [8], read only
        REG32 rf_irq_st_disc_mrw_ads0 : 1; // [9], read only
        REG32 rf_irq_st_disc_mrr_ads0 : 1; // [10], read only
        REG32 rf_irq_st_disc_wr_ads0 : 1; // [11], read only
        REG32 rf_irq_st_disc_rd_ads0 : 1; // [12], read only
        REG32 __15_13 : 3;
        REG32 rf_irq_st_disc_rst_ads1 : 1; // [16], read only
        REG32 rf_irq_st_disc_mrw_ads1 : 1; // [17], read only
        REG32 rf_irq_st_disc_mrr_ads1 : 1; // [18], read only
        REG32 rf_irq_st_disc_wr_ads1 : 1; // [19], read only
        REG32 rf_irq_st_disc_rd_ads1 : 1; // [20], read only
        REG32 __31_21 : 11;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_IRQ_STATUS_T;

//psram_rf_irq_cnt_clr
typedef union {
    REG32 v;
    struct {
        REG32 rf_irq_cnt_clr_dll_unlock_ads0 : 1; // [0]
        REG32 rf_irq_cnt_clr_dll_unlock_ads1 : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_PSRAM_PHY_PSRAM_RF_IRQ_CNT_CLR_T;

//psram_rf_irq_cnt_dll_unlock_ads0
typedef union {
    REG32 v;
    struct {
        REG32 rf_irq_cnt_dll_unlock_ads0 : 31; // [30:0], read only
        REG32 rf_irq_cnt_overflow_dll_unlock_ads0 : 1; // [31], read only
    } b;
} REG_PSRAM_PHY_PSRAM_RF_IRQ_CNT_DLL_UNLOCK_ADS0_T;

//psram_rf_irq_cnt_dll_unlock_ads1
typedef union {
    REG32 v;
    struct {
        REG32 rf_irq_cnt_dll_unlock_ads1 : 31; // [30:0], read only
        REG32 rf_irq_cnt_overflow_dll_unlock_ads1 : 1; // [31], read only
    } b;
} REG_PSRAM_PHY_PSRAM_RF_IRQ_CNT_DLL_UNLOCK_ADS1_T;


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
#define PSRAM_PHY_DRF_T_CPH_RD_OPTM (1<<4)
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

//psram_drf_t_rddata_valid_early
#define PSRAM_PHY_DRF_T_RDDATA_VALID_EARLY(n) (((n)&3)<<0)

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


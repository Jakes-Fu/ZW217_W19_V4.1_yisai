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


#ifndef _LPDDR_PHY_H_
#define _LPDDR_PHY_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'lpddr_phy'."
#endif

#include "global_macros.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// LPDDR_PHY_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_LPDDR_PHY_BASE         0x08801000

typedef volatile struct
{
    REG32                          lpddr_rf_cfg_phy;             //0x00000000
    REG32                          lpddr_rf_cfg_clock_gate;      //0x00000004
    REG32                          lpddr_rf_cfg_sample_resync;   //0x00000008
    REG32                          lpddr_rf_cfg_lpi;             //0x0000000C
    REG32 Reserved_00000010[60];                //0x00000010
    REG32                          lpddr_rf_cfg_ext;             //0x00000100
    REG32 Reserved_00000104[63];                //0x00000104
    REG32                          lpddr_rfdll_cfg_dll;          //0x00000200
    REG32                          lpddr_rfdll_status_cpst_idle; //0x00000204
    REG32                          lpddr_rf_cfg_dll_ac;          //0x00000208
    REG32                          lpddr_rfdll_status_dll_ac;    //0x0000020C
    REG32                          lpddr_rf_cfg_dll_dl_0_wr_ac;  //0x00000210
    REG32                          lpddr_rf_cfg_dll_dl_1_wr_ac;  //0x00000214
    REG32                          lpddr_rf_cfg_dll_dl_2_wr_ac;  //0x00000218
    REG32                          lpddr_rf_cfg_dll_dl_3_wr_ac;  //0x0000021C
    REG32                          lpddr_rf_cfg_dll_dl_4_wr_ac;  //0x00000220
    REG32                          lpddr_rfdll_status_max_cnt_ac; //0x00000224
    REG32                          lpddr_rfdll_status_min_cnt_ac; //0x00000228
    REG32                          lpddr_rf_cfg_phy_iomux_sel_wr_ac; //0x0000022C
    REG32                          lpddr_rf_cfg_phy_iomux_ie_wr_ac; //0x00000230
    REG32                          lpddr_rf_cfg_phy_iomux_oe_wr_ac; //0x00000234
    REG32                          lpddr_rf_cfg_phy_iomux_out_wr_ac; //0x00000238
    REG32                          lpddr_rf_cfg_dll_ds0;         //0x0000023C
    REG32                          lpddr_rfdll_status_dll_ds0;   //0x00000240
    REG32                          lpddr_rf_cfg_dll_dl_0_wr_ds0; //0x00000244
    REG32                          lpddr_rf_cfg_dll_dl_1_wr_ds0; //0x00000248
    REG32                          lpddr_rf_cfg_dll_dl_2_wr_ds0; //0x0000024C
    REG32                          lpddr_rf_cfg_dll_dl_3_wr_ds0; //0x00000250
    REG32                          lpddr_rf_cfg_dll_dl_4_wr_ds0; //0x00000254
    REG32                          lpddr_rf_cfg_dll_dl_5_wr_ds0; //0x00000258
    REG32                          lpddr_rf_cfg_dll_dl_6_wr_ds0; //0x0000025C
    REG32                          lpddr_rf_cfg_dll_dl_7_wr_ds0; //0x00000260
    REG32                          lpddr_rf_cfg_dll_dl_8_wr_ds0; //0x00000264
    REG32                          lpddr_rfdll_status_max_cnt_ds0; //0x00000268
    REG32                          lpddr_rfdll_status_min_cnt_ds0; //0x0000026C
    REG32                          lpddr_rf_cfg_phy_iomux_sel_wr_ds0; //0x00000270
    REG32                          lpddr_rf_cfg_phy_iomux_ie_wr_ds0; //0x00000274
    REG32                          lpddr_rf_cfg_phy_iomux_oe_wr_ds0; //0x00000278
    REG32                          lpddr_rf_cfg_phy_iomux_out_wr_ds0; //0x0000027C
    REG32                          lpddr_rf_cfg_dll_ds1;         //0x00000280
    REG32                          lpddr_rfdll_status_dll_ds1;   //0x00000284
    REG32                          lpddr_rf_cfg_dll_dl_0_wr_ds1; //0x00000288
    REG32                          lpddr_rf_cfg_dll_dl_1_wr_ds1; //0x0000028C
    REG32                          lpddr_rf_cfg_dll_dl_2_wr_ds1; //0x00000290
    REG32                          lpddr_rf_cfg_dll_dl_3_wr_ds1; //0x00000294
    REG32                          lpddr_rf_cfg_dll_dl_4_wr_ds1; //0x00000298
    REG32                          lpddr_rf_cfg_dll_dl_5_wr_ds1; //0x0000029C
    REG32                          lpddr_rf_cfg_dll_dl_6_wr_ds1; //0x000002A0
    REG32                          lpddr_rf_cfg_dll_dl_7_wr_ds1; //0x000002A4
    REG32                          lpddr_rf_cfg_dll_dl_8_wr_ds1; //0x000002A8
    REG32                          lpddr_rfdll_status_max_cnt_ds1; //0x000002AC
    REG32                          lpddr_rfdll_status_min_cnt_ds1; //0x000002B0
    REG32                          lpddr_rf_cfg_phy_iomux_sel_wr_ds1; //0x000002B4
    REG32                          lpddr_rf_cfg_phy_iomux_ie_wr_ds1; //0x000002B8
    REG32                          lpddr_rf_cfg_phy_iomux_oe_wr_ds1; //0x000002BC
    REG32                          lpddr_rf_cfg_phy_iomux_out_wr_ds1; //0x000002C0
    REG32                          lpddr_rf_status_phy_data_in;  //0x000002C4
    REG32 Reserved_000002C8[78];                //0x000002C8
    REG32                          lpddr_drf_cfg;                //0x00000400
    REG32                          lpddr_drf_cfg_reg_sel;        //0x00000404
    REG32                          lpddr_drf_cfg_dqs_ie_sel_f0;  //0x00000408
    REG32                          lpddr_drf_cfg_dqs_oe_sel_f0;  //0x0000040C
    REG32                          lpddr_drf_cfg_dqs_out_sel_f0; //0x00000410
    REG32                          lpddr_drf_cfg_dqs_gate_sel_f0; //0x00000414
    REG32                          lpddr_drf_cfg_data_ie_sel_f0; //0x00000418
    REG32                          lpddr_drf_cfg_data_oe_sel_f0; //0x0000041C
    REG32                          lpddr_drf_cfg_read_en_sel_f0; //0x00000420
    REG32                          lpddr_drf_cfg_dqs_ie_sel_f1;  //0x00000424
    REG32                          lpddr_drf_cfg_dqs_oe_sel_f1;  //0x00000428
    REG32                          lpddr_drf_cfg_dqs_out_sel_f1; //0x0000042C
    REG32                          lpddr_drf_cfg_dqs_gate_sel_f1; //0x00000430
    REG32                          lpddr_drf_cfg_data_ie_sel_f1; //0x00000434
    REG32                          lpddr_drf_cfg_data_oe_sel_f1; //0x00000438
    REG32                          lpddr_drf_cfg_read_en_sel_f1; //0x0000043C
    REG32                          lpddr_drf_cfg_dqs_ie_sel_f2;  //0x00000440
    REG32                          lpddr_drf_cfg_dqs_oe_sel_f2;  //0x00000444
    REG32                          lpddr_drf_cfg_dqs_out_sel_f2; //0x00000448
    REG32                          lpddr_drf_cfg_dqs_gate_sel_f2; //0x0000044C
    REG32                          lpddr_drf_cfg_data_ie_sel_f2; //0x00000450
    REG32                          lpddr_drf_cfg_data_oe_sel_f2; //0x00000454
    REG32                          lpddr_drf_cfg_read_en_sel_f2; //0x00000458
    REG32                          lpddr_drf_cfg_dqs_ie_sel_f3;  //0x0000045C
    REG32                          lpddr_drf_cfg_dqs_oe_sel_f3;  //0x00000460
    REG32                          lpddr_drf_cfg_dqs_out_sel_f3; //0x00000464
    REG32                          lpddr_drf_cfg_dqs_gate_sel_f3; //0x00000468
    REG32                          lpddr_drf_cfg_data_ie_sel_f3; //0x0000046C
    REG32                          lpddr_drf_cfg_data_oe_sel_f3; //0x00000470
    REG32                          lpddr_drf_cfg_read_en_sel_f3; //0x00000474
    REG32                          lpddr_drf_cfg_dll_mode_f0;    //0x00000478
    REG32                          lpddr_drf_cfg_dll_cnt_f0;     //0x0000047C
    REG32                          lpddr_drf_cfg_dll_mode_f1;    //0x00000480
    REG32                          lpddr_drf_cfg_dll_cnt_f1;     //0x00000484
    REG32                          lpddr_drf_cfg_dll_mode_f2;    //0x00000488
    REG32                          lpddr_drf_cfg_dll_cnt_f2;     //0x0000048C
    REG32                          lpddr_drf_cfg_dll_mode_f3;    //0x00000490
    REG32                          lpddr_drf_cfg_dll_cnt_f3;     //0x00000494
    REG32 Reserved_00000498[26];                //0x00000498
    REG32                          lpddr_drf_train_cfg;          //0x00000500
    REG32                          lpddr_drf_mr_data_0;          //0x00000504
    REG32                          lpddr_drf_mr_data_1;          //0x00000508
    REG32 Reserved_0000050C[61];                //0x0000050C
    REG32                          lpddr_rf_irq_ctrl;            //0x00000600
    REG32                          lpddr_rf_irq_status_clr;      //0x00000604
    REG32                          lpddr_rf_irq_status;          //0x00000608
    REG32                          lpddr_rf_irq_cnt_clr;         //0x0000060C
    REG32                          lpddr_rf_irq_cnt_dll_unlock_ac; //0x00000610
    REG32                          lpddr_rf_irq_cnt_dll_unlock_ds0; //0x00000614
    REG32                          lpddr_rf_irq_cnt_dll_unlock_ds1; //0x00000618
} HWP_LPDDR_PHY_T;

#define hwp_lpddrPhy               ((HWP_LPDDR_PHY_T*) REG_ACCESS_ADDRESS(REG_LPDDR_PHY_BASE))


//lpddr_rf_cfg_phy
#define LPDDR_PHY_RF_PHY_INIT_COMPLETE (1<<1)
#define LPDDR_PHY_RF_PHY_EN        (1<<0)

//lpddr_rf_cfg_clock_gate
#define LPDDR_PHY_RF_CLK_GATE_AG_RD_EN (1<<4)
#define LPDDR_PHY_RF_CLK_GATE_AG_WR_EN (1<<3)
#define LPDDR_PHY_RF_CLK_GATE_AG_EN (1<<2)
#define LPDDR_PHY_RF_CLK_GATE_FG_EN (1<<1)
#define LPDDR_PHY_RF_CLK_GATE_EN   (1<<0)

//lpddr_rf_cfg_sample_resync
#define LPDDR_PHY_RF_DATA_RESYNC_SEL(n) (((n)&0xFFFF)<<16)
#define LPDDR_PHY_RF_SAMPLE_RESYNC_AUTO_EN (1<<1)
#define LPDDR_PHY_RF_SAMPLE_RESYNC (1<<0)

//lpddr_rf_cfg_lpi
#define LPDDR_PHY_RF_CWAKEUP_S0    (1<<2)
#define LPDDR_PHY_RF_CWAKEUP_M0    (1<<1)
#define LPDDR_PHY_RF_LPI_SEL_M0    (1<<0)

//lpddr_rf_cfg_ext
#define LPDDR_PHY_RF_DATA_RESYNC_EXT(n) (((n)&15)<<20)
#define LPDDR_PHY_RF_DQS_GATE_EXT(n) (((n)&15)<<16)
#define LPDDR_PHY_RF_DQS_IE_EXT(n) (((n)&15)<<12)
#define LPDDR_PHY_RF_DQS_OE_EXT(n) (((n)&15)<<8)
#define LPDDR_PHY_RF_DATA_IE_EXT(n) (((n)&15)<<4)
#define LPDDR_PHY_RF_DATA_OE_EXT(n) (((n)&15)<<0)

//lpddr_rfdll_cfg_dll
#define LPDDR_PHY_RFDLL_RESET      (1<<0)

//lpddr_rfdll_status_cpst_idle
#define LPDDR_PHY_RFDL_CPST_ST_IDLE_DS1 (1<<2)
#define LPDDR_PHY_RFDL_CPST_ST_IDLE_DS0 (1<<1)
#define LPDDR_PHY_RFDL_CPST_ST_IDLE_AC (1<<0)

//lpddr_rf_cfg_dll_ac
#define LPDDR_PHY_RF_DLL_LOCK_WAIT_AC(n) (((n)&15)<<28)
#define LPDDR_PHY_RF_DLL_AUTO_ERR_CLR_EN_AC (1<<27)
#define LPDDR_PHY_RF_DLL_PD_CNT_AC(n) (((n)&7)<<24)
#define LPDDR_PHY_RF_DL_CPST_THR_AC(n) (((n)&0xFF)<<16)
#define LPDDR_PHY_RF_DLL_EN_AC     (1<<15)
#define LPDDR_PHY_RF_DLL_CLK_SEL_AC (1<<14)
#define LPDDR_PHY_RF_DLL_ERR_CLR_AC (1<<13)
#define LPDDR_PHY_RF_DL_CPST_AUTO_REF_EN_AC (1<<12)
#define LPDDR_PHY_RF_DL_CPST_START_AC (1<<11)
#define LPDDR_PHY_RF_DL_CPST_EN_AC (1<<10)
#define LPDDR_PHY_RF_DLL_AUTO_CLR_EN_AC (1<<9)
#define LPDDR_PHY_RF_DLL_CLR_AC    (1<<8)

//lpddr_rfdll_status_dll_ac
#define LPDDR_PHY_RFDLL_ERROR_AC   (1<<29)
#define LPDDR_PHY_RFDLL_LOCKED_AC  (1<<28)
#define LPDDR_PHY_RFDLL_ST_AC(n)   (((n)&7)<<25)
#define LPDDR_PHY_RFDL_CPST_ST_AC  (1<<24)
#define LPDDR_PHY_RFDLL_CNT_AC(n)  (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_0_wr_ac
#define LPDDR_PHY_RF_CLKWR_DL_CPST_EN_AC (1<<31)
#define LPDDR_PHY_RF_CLKWR_DL_CPST_MINUS_AC (1<<30)
#define LPDDR_PHY_RF_CLKWR_QTR_DL_CPST_OFFSET_AC(n) (((n)&3)<<28)
#define LPDDR_PHY_RFDL_CLKWR_QTR_CNT_AC(n) (((n)&3)<<26)
#define LPDDR_PHY_RF_CLKWR_QTR_DL_SEL_AC(n) (((n)&3)<<24)
#define LPDDR_PHY_RF_CLKWR_RAW_DL_CPST_OFFSET_AC(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDL_CLKWR_RAW_CNT_AC(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RF_CLKWR_RAW_DL_SEL_AC(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_1_wr_ac
#define LPDDR_PHY_RF_DLY_OUT_A3_DL_SEL(n) (((n)&31)<<24)
#define LPDDR_PHY_RF_DLY_OUT_A2_DL_SEL(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_OUT_A1_DL_SEL(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_OUT_A0_DL_SEL(n) (((n)&31)<<0)

//lpddr_rf_cfg_dll_dl_2_wr_ac
#define LPDDR_PHY_RF_DLY_OUT_A7_DL_SEL(n) (((n)&31)<<24)
#define LPDDR_PHY_RF_DLY_OUT_A6_DL_SEL(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_OUT_A5_DL_SEL(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_OUT_A4_DL_SEL(n) (((n)&31)<<0)

//lpddr_rf_cfg_dll_dl_3_wr_ac
#define LPDDR_PHY_RF_DLY_OUT_A9_DL_SEL(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_OUT_A8_DL_SEL(n) (((n)&31)<<0)

//lpddr_rf_cfg_dll_dl_4_wr_ac
#define LPDDR_PHY_RF_DLY_OUT_CLK_DL_SEL(n) (((n)&31)<<24)
#define LPDDR_PHY_RF_DLY_OUT_CKE_DL_SEL(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_OUT_CSN_DL_SEL(n) (((n)&31)<<0)

//lpddr_rfdll_status_max_cnt_ac
#define LPDDR_PHY_RFDLL_MAX_CNT_F3_AC(n) (((n)&0x7F)<<24)
#define LPDDR_PHY_RFDLL_MAX_CNT_F2_AC(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDLL_MAX_CNT_F1_AC(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RFDLL_MAX_CNT_F0_AC(n) (((n)&0x7F)<<0)

//lpddr_rfdll_status_min_cnt_ac
#define LPDDR_PHY_RFDLL_MIN_CNT_F3_AC(n) (((n)&0x7F)<<24)
#define LPDDR_PHY_RFDLL_MIN_CNT_F2_AC(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDLL_MIN_CNT_F1_AC(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RFDLL_MIN_CNT_F0_AC(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_phy_iomux_sel_wr_ac
#define LPDDR_PHY_RF_PHY_IO_CKE1_SEL_AC (1<<21)
#define LPDDR_PHY_RF_PHY_IO_CKE0_SEL_AC (1<<20)
#define LPDDR_PHY_RF_PHY_IO_CSN_SEL_AC (1<<16)
#define LPDDR_PHY_RF_PHY_IO_A9_SEL_AC (1<<9)
#define LPDDR_PHY_RF_PHY_IO_A8_SEL_AC (1<<8)
#define LPDDR_PHY_RF_PHY_IO_A7_SEL_AC (1<<7)
#define LPDDR_PHY_RF_PHY_IO_A6_SEL_AC (1<<6)
#define LPDDR_PHY_RF_PHY_IO_A5_SEL_AC (1<<5)
#define LPDDR_PHY_RF_PHY_IO_A4_SEL_AC (1<<4)
#define LPDDR_PHY_RF_PHY_IO_A3_SEL_AC (1<<3)
#define LPDDR_PHY_RF_PHY_IO_A2_SEL_AC (1<<2)
#define LPDDR_PHY_RF_PHY_IO_A1_SEL_AC (1<<1)
#define LPDDR_PHY_RF_PHY_IO_A0_SEL_AC (1<<0)

//lpddr_rf_cfg_phy_iomux_ie_wr_ac
#define LPDDR_PHY_RF_PHY_IO_CKE1_IE_AC (1<<21)
#define LPDDR_PHY_RF_PHY_IO_CKE0_IE_AC (1<<20)
#define LPDDR_PHY_RF_PHY_IO_CSN_IE_AC (1<<16)
#define LPDDR_PHY_RF_PHY_IO_A9_IE_AC (1<<9)
#define LPDDR_PHY_RF_PHY_IO_A8_IE_AC (1<<8)
#define LPDDR_PHY_RF_PHY_IO_A7_IE_AC (1<<7)
#define LPDDR_PHY_RF_PHY_IO_A6_IE_AC (1<<6)
#define LPDDR_PHY_RF_PHY_IO_A5_IE_AC (1<<5)
#define LPDDR_PHY_RF_PHY_IO_A4_IE_AC (1<<4)
#define LPDDR_PHY_RF_PHY_IO_A3_IE_AC (1<<3)
#define LPDDR_PHY_RF_PHY_IO_A2_IE_AC (1<<2)
#define LPDDR_PHY_RF_PHY_IO_A1_IE_AC (1<<1)
#define LPDDR_PHY_RF_PHY_IO_A0_IE_AC (1<<0)

//lpddr_rf_cfg_phy_iomux_oe_wr_ac
#define LPDDR_PHY_RF_PHY_IO_CKE1_OE_AC (1<<21)
#define LPDDR_PHY_RF_PHY_IO_CKE0_OE_AC (1<<20)
#define LPDDR_PHY_RF_PHY_IO_CSN_OE_AC (1<<16)
#define LPDDR_PHY_RF_PHY_IO_A9_OE_AC (1<<9)
#define LPDDR_PHY_RF_PHY_IO_A8_OE_AC (1<<8)
#define LPDDR_PHY_RF_PHY_IO_A7_OE_AC (1<<7)
#define LPDDR_PHY_RF_PHY_IO_A6_OE_AC (1<<6)
#define LPDDR_PHY_RF_PHY_IO_A5_OE_AC (1<<5)
#define LPDDR_PHY_RF_PHY_IO_A4_OE_AC (1<<4)
#define LPDDR_PHY_RF_PHY_IO_A3_OE_AC (1<<3)
#define LPDDR_PHY_RF_PHY_IO_A2_OE_AC (1<<2)
#define LPDDR_PHY_RF_PHY_IO_A1_OE_AC (1<<1)
#define LPDDR_PHY_RF_PHY_IO_A0_OE_AC (1<<0)

//lpddr_rf_cfg_phy_iomux_out_wr_ac
#define LPDDR_PHY_RF_PHY_IO_CKE1_OUT_AC (1<<21)
#define LPDDR_PHY_RF_PHY_IO_CKE0_OUT_AC (1<<20)
#define LPDDR_PHY_RF_PHY_IO_CSN_OUT_AC (1<<16)
#define LPDDR_PHY_RF_PHY_IO_A9_OUT_AC (1<<9)
#define LPDDR_PHY_RF_PHY_IO_A8_OUT_AC (1<<8)
#define LPDDR_PHY_RF_PHY_IO_A7_OUT_AC (1<<7)
#define LPDDR_PHY_RF_PHY_IO_A6_OUT_AC (1<<6)
#define LPDDR_PHY_RF_PHY_IO_A5_OUT_AC (1<<5)
#define LPDDR_PHY_RF_PHY_IO_A4_OUT_AC (1<<4)
#define LPDDR_PHY_RF_PHY_IO_A3_OUT_AC (1<<3)
#define LPDDR_PHY_RF_PHY_IO_A2_OUT_AC (1<<2)
#define LPDDR_PHY_RF_PHY_IO_A1_OUT_AC (1<<1)
#define LPDDR_PHY_RF_PHY_IO_A0_OUT_AC (1<<0)

//lpddr_rf_cfg_dll_ds0
#define LPDDR_PHY_RF_DLL_LOCK_WAIT_DS0(n) (((n)&15)<<28)
#define LPDDR_PHY_RF_DLL_AUTO_ERR_CLR_EN_DS0 (1<<27)
#define LPDDR_PHY_RF_DLL_PD_CNT_DS0(n) (((n)&7)<<24)
#define LPDDR_PHY_RF_DL_CPST_THR_DS0(n) (((n)&0xFF)<<16)
#define LPDDR_PHY_RF_DLL_EN_DS0    (1<<15)
#define LPDDR_PHY_RF_DLL_CLK_SEL_DS0 (1<<14)
#define LPDDR_PHY_RF_DLL_ERR_CLR_DS0 (1<<13)
#define LPDDR_PHY_RF_DL_CPST_AUTO_REF_EN_DS0 (1<<12)
#define LPDDR_PHY_RF_DL_CPST_START_DS0 (1<<11)
#define LPDDR_PHY_RF_DL_CPST_EN_DS0 (1<<10)
#define LPDDR_PHY_RF_DLL_AUTO_CLR_EN_DS0 (1<<9)
#define LPDDR_PHY_RF_DLL_CLR_DS0   (1<<8)

//lpddr_rfdll_status_dll_ds0
#define LPDDR_PHY_RFDLL_ERROR_DS0  (1<<29)
#define LPDDR_PHY_RFDLL_LOCKED_DS0 (1<<28)
#define LPDDR_PHY_RFDLL_ST_DS0(n)  (((n)&7)<<25)
#define LPDDR_PHY_RFDL_CPST_ST_DS0 (1<<24)
#define LPDDR_PHY_RFDLL_CNT_DS0(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_0_wr_ds0
#define LPDDR_PHY_RF_CLKWR_DL_CPST_EN_DS0 (1<<31)
#define LPDDR_PHY_RF_CLKWR_DL_CPST_MINUS_DS0 (1<<30)
#define LPDDR_PHY_RF_CLKWR_QTR_DL_CPST_OFFSET_DS0(n) (((n)&3)<<28)
#define LPDDR_PHY_RFDL_CLKWR_QTR_CNT_DS0(n) (((n)&3)<<26)
#define LPDDR_PHY_RF_CLKWR_QTR_DL_SEL_DS0(n) (((n)&3)<<24)
#define LPDDR_PHY_RF_CLKWR_RAW_DL_CPST_OFFSET_DS0(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDL_CLKWR_RAW_CNT_DS0(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RF_CLKWR_RAW_DL_SEL_DS0(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_1_wr_ds0
#define LPDDR_PHY_RF_DQS_IN_POS_DL_CPST_EN_DS0 (1<<31)
#define LPDDR_PHY_RF_DQS_IN_POS_DL_CPST_MINUS_DS0 (1<<30)
#define LPDDR_PHY_RF_DQS_IN_POS_QTR_DL_CPST_OFFSET_DS0(n) (((n)&3)<<28)
#define LPDDR_PHY_RFDL_DQS_IN_POS_QTR_CNT_DS0(n) (((n)&3)<<26)
#define LPDDR_PHY_RF_DQS_IN_POS_QTR_DL_SEL_DS0(n) (((n)&3)<<24)
#define LPDDR_PHY_RF_DQS_IN_POS_RAW_DL_CPST_OFFSET_DS0(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDL_DQS_IN_POS_RAW_CNT_DS0(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RF_DQS_IN_POS_RAW_DL_SEL_DS0(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_2_wr_ds0
#define LPDDR_PHY_RF_DQS_IN_NEG_DL_CPST_EN_DS0 (1<<31)
#define LPDDR_PHY_RF_DQS_IN_NEG_DL_CPST_MINUS_DS0 (1<<30)
#define LPDDR_PHY_RF_DQS_IN_NEG_QTR_DL_CPST_OFFSET_DS0(n) (((n)&3)<<28)
#define LPDDR_PHY_RFDL_DQS_IN_NEG_QTR_CNT_DS0(n) (((n)&3)<<26)
#define LPDDR_PHY_RF_DQS_IN_NEG_QTR_DL_SEL_DS0(n) (((n)&3)<<24)
#define LPDDR_PHY_RF_DQS_IN_NEG_RAW_DL_CPST_OFFSET_DS0(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDL_DQS_IN_NEG_RAW_CNT_DS0(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RF_DQS_IN_NEG_RAW_DL_SEL_DS0(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_3_wr_ds0
#define LPDDR_PHY_RF_DQS_GATE_DL_CPST_EN_DS0 (1<<31)
#define LPDDR_PHY_RF_DQS_GATE_DL_CPST_MINUS_DS0 (1<<30)
#define LPDDR_PHY_RF_DQS_GATE_QTR_DL_CPST_OFFSET_DS0(n) (((n)&3)<<28)
#define LPDDR_PHY_RFDL_DQS_GATE_QTR_CNT_DS0(n) (((n)&3)<<26)
#define LPDDR_PHY_RF_DQS_GATE_QTR_DL_SEL_DS0(n) (((n)&3)<<24)
#define LPDDR_PHY_RF_DQS_GATE_RAW_DL_CPST_OFFSET_DS0(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDL_DQS_GATE_RAW_CNT_DS0(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RF_DQS_GATE_RAW_DL_SEL_DS0(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_4_wr_ds0
#define LPDDR_PHY_RF_DLY_OUT_D3_DL_SEL_DS0(n) (((n)&31)<<24)
#define LPDDR_PHY_RF_DLY_OUT_D2_DL_SEL_DS0(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_OUT_D1_DL_SEL_DS0(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_OUT_D0_DL_SEL_DS0(n) (((n)&31)<<0)

//lpddr_rf_cfg_dll_dl_5_wr_ds0
#define LPDDR_PHY_RF_DLY_OUT_D7_DL_SEL_DS0(n) (((n)&31)<<24)
#define LPDDR_PHY_RF_DLY_OUT_D6_DL_SEL_DS0(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_OUT_D5_DL_SEL_DS0(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_OUT_D4_DL_SEL_DS0(n) (((n)&31)<<0)

//lpddr_rf_cfg_dll_dl_6_wr_ds0
#define LPDDR_PHY_RF_DLY_IN_D3_DL_SEL_DS0(n) (((n)&31)<<24)
#define LPDDR_PHY_RF_DLY_IN_D2_DL_SEL_DS0(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_IN_D1_DL_SEL_DS0(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_IN_D0_DL_SEL_DS0(n) (((n)&31)<<0)

//lpddr_rf_cfg_dll_dl_7_wr_ds0
#define LPDDR_PHY_RF_DLY_IN_D7_DL_SEL_DS0(n) (((n)&31)<<24)
#define LPDDR_PHY_RF_DLY_IN_D6_DL_SEL_DS0(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_IN_D5_DL_SEL_DS0(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_IN_D4_DL_SEL_DS0(n) (((n)&31)<<0)

//lpddr_rf_cfg_dll_dl_8_wr_ds0
#define LPDDR_PHY_RF_DLY_IN_DQS_DL_SEL_DS0(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_OUT_DQM_DL_SEL_DS0(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_OUT_DQS_DL_SEL_DS0(n) (((n)&31)<<0)

//lpddr_rfdll_status_max_cnt_ds0
#define LPDDR_PHY_RFDLL_MAX_CNT_F3_DS0(n) (((n)&0x7F)<<24)
#define LPDDR_PHY_RFDLL_MAX_CNT_F2_DS0(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDLL_MAX_CNT_F1_DS0(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RFDLL_MAX_CNT_F0_DS0(n) (((n)&0x7F)<<0)

//lpddr_rfdll_status_min_cnt_ds0
#define LPDDR_PHY_RFDLL_MIN_CNT_F3_DS0(n) (((n)&0x7F)<<24)
#define LPDDR_PHY_RFDLL_MIN_CNT_F2_DS0(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDLL_MIN_CNT_F1_DS0(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RFDLL_MIN_CNT_F0_DS0(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_phy_iomux_sel_wr_ds0
#define LPDDR_PHY_RF_PHY_IO_DQS_SEL_DS0 (1<<9)
#define LPDDR_PHY_RF_PHY_IO_DQM_SEL_DS0 (1<<8)
#define LPDDR_PHY_RF_PHY_IO_D7_SEL_DS0 (1<<7)
#define LPDDR_PHY_RF_PHY_IO_D6_SEL_DS0 (1<<6)
#define LPDDR_PHY_RF_PHY_IO_D5_SEL_DS0 (1<<5)
#define LPDDR_PHY_RF_PHY_IO_D4_SEL_DS0 (1<<4)
#define LPDDR_PHY_RF_PHY_IO_D3_SEL_DS0 (1<<3)
#define LPDDR_PHY_RF_PHY_IO_D2_SEL_DS0 (1<<2)
#define LPDDR_PHY_RF_PHY_IO_D1_SEL_DS0 (1<<1)
#define LPDDR_PHY_RF_PHY_IO_D0_SEL_DS0 (1<<0)

//lpddr_rf_cfg_phy_iomux_ie_wr_ds0
#define LPDDR_PHY_RF_PHY_IO_DQS_IE_DS0 (1<<9)
#define LPDDR_PHY_RF_PHY_IO_DQM_IE_DS0 (1<<8)
#define LPDDR_PHY_RF_PHY_IO_D7_IE_DS0 (1<<7)
#define LPDDR_PHY_RF_PHY_IO_D6_IE_DS0 (1<<6)
#define LPDDR_PHY_RF_PHY_IO_D5_IE_DS0 (1<<5)
#define LPDDR_PHY_RF_PHY_IO_D4_IE_DS0 (1<<4)
#define LPDDR_PHY_RF_PHY_IO_D3_IE_DS0 (1<<3)
#define LPDDR_PHY_RF_PHY_IO_D2_IE_DS0 (1<<2)
#define LPDDR_PHY_RF_PHY_IO_D1_IE_DS0 (1<<1)
#define LPDDR_PHY_RF_PHY_IO_D0_IE_DS0 (1<<0)

//lpddr_rf_cfg_phy_iomux_oe_wr_ds0
#define LPDDR_PHY_RF_PHY_IO_DQS_OE_DS0 (1<<9)
#define LPDDR_PHY_RF_PHY_IO_DQM_OE_DS0 (1<<8)
#define LPDDR_PHY_RF_PHY_IO_D7_OE_DS0 (1<<7)
#define LPDDR_PHY_RF_PHY_IO_D6_OE_DS0 (1<<6)
#define LPDDR_PHY_RF_PHY_IO_D5_OE_DS0 (1<<5)
#define LPDDR_PHY_RF_PHY_IO_D4_OE_DS0 (1<<4)
#define LPDDR_PHY_RF_PHY_IO_D3_OE_DS0 (1<<3)
#define LPDDR_PHY_RF_PHY_IO_D2_OE_DS0 (1<<2)
#define LPDDR_PHY_RF_PHY_IO_D1_OE_DS0 (1<<1)
#define LPDDR_PHY_RF_PHY_IO_D0_OE_DS0 (1<<0)

//lpddr_rf_cfg_phy_iomux_out_wr_ds0
#define LPDDR_PHY_RF_PHY_IO_DQS_OUT_DS0 (1<<9)
#define LPDDR_PHY_RF_PHY_IO_DQM_OUT_DS0 (1<<8)
#define LPDDR_PHY_RF_PHY_IO_D7_OUT_DS0 (1<<7)
#define LPDDR_PHY_RF_PHY_IO_D6_OUT_DS0 (1<<6)
#define LPDDR_PHY_RF_PHY_IO_D5_OUT_DS0 (1<<5)
#define LPDDR_PHY_RF_PHY_IO_D4_OUT_DS0 (1<<4)
#define LPDDR_PHY_RF_PHY_IO_D3_OUT_DS0 (1<<3)
#define LPDDR_PHY_RF_PHY_IO_D2_OUT_DS0 (1<<2)
#define LPDDR_PHY_RF_PHY_IO_D1_OUT_DS0 (1<<1)
#define LPDDR_PHY_RF_PHY_IO_D0_OUT_DS0 (1<<0)

//lpddr_rf_cfg_dll_ds1
#define LPDDR_PHY_RF_DLL_LOCK_WAIT_DS1(n) (((n)&15)<<28)
#define LPDDR_PHY_RF_DLL_AUTO_ERR_CLR_EN_DS1 (1<<27)
#define LPDDR_PHY_RF_DLL_PD_CNT_DS1(n) (((n)&7)<<24)
#define LPDDR_PHY_RF_DL_CPST_THR_DS1(n) (((n)&0xFF)<<16)
#define LPDDR_PHY_RF_DLL_EN_DS1    (1<<15)
#define LPDDR_PHY_RF_DLL_CLK_SEL_DS1 (1<<14)
#define LPDDR_PHY_RF_DLL_ERR_CLR_DS1 (1<<13)
#define LPDDR_PHY_RF_DL_CPST_AUTO_REF_EN_DS1 (1<<12)
#define LPDDR_PHY_RF_DL_CPST_START_DS1 (1<<11)
#define LPDDR_PHY_RF_DL_CPST_EN_DS1 (1<<10)
#define LPDDR_PHY_RF_DLL_AUTO_CLR_EN_DS1 (1<<9)
#define LPDDR_PHY_RF_DLL_CLR_DS1   (1<<8)

//lpddr_rfdll_status_dll_ds1
#define LPDDR_PHY_RFDLL_ERROR_DS1  (1<<29)
#define LPDDR_PHY_RFDLL_LOCKED_DS1 (1<<28)
#define LPDDR_PHY_RFDLL_ST_DS1(n)  (((n)&7)<<25)
#define LPDDR_PHY_RFDL_CPST_ST_DS1 (1<<24)
#define LPDDR_PHY_RFDLL_CNT_DS1(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_0_wr_ds1
#define LPDDR_PHY_RF_CLKWR_DL_CPST_EN_DS1 (1<<31)
#define LPDDR_PHY_RF_CLKWR_DL_CPST_MINUS_DS1 (1<<30)
#define LPDDR_PHY_RF_CLKWR_QTR_DL_CPST_OFFSET_DS1(n) (((n)&3)<<28)
#define LPDDR_PHY_RFDL_CLKWR_QTR_CNT_DS1(n) (((n)&3)<<26)
#define LPDDR_PHY_RF_CLKWR_QTR_DL_SEL_DS1(n) (((n)&3)<<24)
#define LPDDR_PHY_RF_CLKWR_RAW_DL_CPST_OFFSET_DS1(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDL_CLKWR_RAW_CNT_DS1(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RF_CLKWR_RAW_DL_SEL_DS1(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_1_wr_ds1
#define LPDDR_PHY_RF_DQS_IN_POS_DL_CPST_EN_DS1 (1<<31)
#define LPDDR_PHY_RF_DQS_IN_POS_DL_CPST_MINUS_DS1 (1<<30)
#define LPDDR_PHY_RF_DQS_IN_POS_QTR_DL_CPST_OFFSET_DS1(n) (((n)&3)<<28)
#define LPDDR_PHY_RFDL_DQS_IN_POS_QTR_CNT_DS1(n) (((n)&3)<<26)
#define LPDDR_PHY_RF_DQS_IN_POS_QTR_DL_SEL_DS1(n) (((n)&3)<<24)
#define LPDDR_PHY_RF_DQS_IN_POS_RAW_DL_CPST_OFFSET_DS1(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDL_DQS_IN_POS_RAW_CNT_DS1(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RF_DQS_IN_POS_RAW_DL_SEL_DS1(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_2_wr_ds1
#define LPDDR_PHY_RF_DQS_IN_NEG_DL_CPST_EN_DS1 (1<<31)
#define LPDDR_PHY_RF_DQS_IN_NEG_DL_CPST_MINUS_DS1 (1<<30)
#define LPDDR_PHY_RF_DQS_IN_NEG_QTR_DL_CPST_OFFSET_DS1(n) (((n)&3)<<28)
#define LPDDR_PHY_RFDL_DQS_IN_NEG_QTR_CNT_DS1(n) (((n)&3)<<26)
#define LPDDR_PHY_RF_DQS_IN_NEG_QTR_DL_SEL_DS1(n) (((n)&3)<<24)
#define LPDDR_PHY_RF_DQS_IN_NEG_RAW_DL_CPST_OFFSET_DS1(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDL_DQS_IN_NEG_RAW_CNT_DS1(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RF_DQS_IN_NEG_RAW_DL_SEL_DS1(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_3_wr_ds1
#define LPDDR_PHY_RF_DQS_GATE_DL_CPST_EN_DS1 (1<<31)
#define LPDDR_PHY_RF_DQS_GATE_DL_CPST_MINUS_DS1 (1<<30)
#define LPDDR_PHY_RF_DQS_GATE_QTR_DL_CPST_OFFSET_DS1(n) (((n)&3)<<28)
#define LPDDR_PHY_RFDL_DQS_GATE_QTR_CNT_DS1(n) (((n)&3)<<26)
#define LPDDR_PHY_RF_DQS_GATE_QTR_DL_SEL_DS1(n) (((n)&3)<<24)
#define LPDDR_PHY_RF_DQS_GATE_RAW_DL_CPST_OFFSET_DS1(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDL_DQS_GATE_RAW_CNT_DS1(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RF_DQS_GATE_RAW_DL_SEL_DS1(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_dll_dl_4_wr_ds1
#define LPDDR_PHY_RF_DLY_OUT_D3_DL_SEL_DS1(n) (((n)&31)<<24)
#define LPDDR_PHY_RF_DLY_OUT_D2_DL_SEL_DS1(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_OUT_D1_DL_SEL_DS1(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_OUT_D0_DL_SEL_DS1(n) (((n)&31)<<0)

//lpddr_rf_cfg_dll_dl_5_wr_ds1
#define LPDDR_PHY_RF_DLY_OUT_D7_DL_SEL_DS1(n) (((n)&31)<<24)
#define LPDDR_PHY_RF_DLY_OUT_D6_DL_SEL_DS1(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_OUT_D5_DL_SEL_DS1(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_OUT_D4_DL_SEL_DS1(n) (((n)&31)<<0)

//lpddr_rf_cfg_dll_dl_6_wr_ds1
#define LPDDR_PHY_RF_DLY_IN_D3_DL_SEL_DS1(n) (((n)&31)<<24)
#define LPDDR_PHY_RF_DLY_IN_D2_DL_SEL_DS1(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_IN_D1_DL_SEL_DS1(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_IN_D0_DL_SEL_DS1(n) (((n)&31)<<0)

//lpddr_rf_cfg_dll_dl_7_wr_ds1
#define LPDDR_PHY_RF_DLY_IN_D7_DL_SEL_DS1(n) (((n)&31)<<24)
#define LPDDR_PHY_RF_DLY_IN_D6_DL_SEL_DS1(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_IN_D5_DL_SEL_DS1(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_IN_D4_DL_SEL_DS1(n) (((n)&31)<<0)

//lpddr_rf_cfg_dll_dl_8_wr_ds1
#define LPDDR_PHY_RF_DLY_IN_DQS_DL_SEL_DS1(n) (((n)&31)<<16)
#define LPDDR_PHY_RF_DLY_OUT_DQM_DL_SEL_DS1(n) (((n)&31)<<8)
#define LPDDR_PHY_RF_DLY_OUT_DQS_DL_SEL_DS1(n) (((n)&31)<<0)

//lpddr_rfdll_status_max_cnt_ds1
#define LPDDR_PHY_RFDLL_MAX_CNT_F3_DS1(n) (((n)&0x7F)<<24)
#define LPDDR_PHY_RFDLL_MAX_CNT_F2_DS1(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDLL_MAX_CNT_F1_DS1(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RFDLL_MAX_CNT_F0_DS1(n) (((n)&0x7F)<<0)

//lpddr_rfdll_status_min_cnt_ds1
#define LPDDR_PHY_RFDLL_MIN_CNT_F3_DS1(n) (((n)&0x7F)<<24)
#define LPDDR_PHY_RFDLL_MIN_CNT_F2_DS1(n) (((n)&0x7F)<<16)
#define LPDDR_PHY_RFDLL_MIN_CNT_F1_DS1(n) (((n)&0x7F)<<8)
#define LPDDR_PHY_RFDLL_MIN_CNT_F0_DS1(n) (((n)&0x7F)<<0)

//lpddr_rf_cfg_phy_iomux_sel_wr_ds1
#define LPDDR_PHY_RF_PHY_IO_DQS_SEL_DS1 (1<<9)
#define LPDDR_PHY_RF_PHY_IO_DQM_SEL_DS1 (1<<8)
#define LPDDR_PHY_RF_PHY_IO_D7_SEL_DS1 (1<<7)
#define LPDDR_PHY_RF_PHY_IO_D6_SEL_DS1 (1<<6)
#define LPDDR_PHY_RF_PHY_IO_D5_SEL_DS1 (1<<5)
#define LPDDR_PHY_RF_PHY_IO_D4_SEL_DS1 (1<<4)
#define LPDDR_PHY_RF_PHY_IO_D3_SEL_DS1 (1<<3)
#define LPDDR_PHY_RF_PHY_IO_D2_SEL_DS1 (1<<2)
#define LPDDR_PHY_RF_PHY_IO_D1_SEL_DS1 (1<<1)
#define LPDDR_PHY_RF_PHY_IO_D0_SEL_DS1 (1<<0)

//lpddr_rf_cfg_phy_iomux_ie_wr_ds1
#define LPDDR_PHY_RF_PHY_IO_DQS_IE_DS1 (1<<9)
#define LPDDR_PHY_RF_PHY_IO_DQM_IE_DS1 (1<<8)
#define LPDDR_PHY_RF_PHY_IO_D7_IE_DS1 (1<<7)
#define LPDDR_PHY_RF_PHY_IO_D6_IE_DS1 (1<<6)
#define LPDDR_PHY_RF_PHY_IO_D5_IE_DS1 (1<<5)
#define LPDDR_PHY_RF_PHY_IO_D4_IE_DS1 (1<<4)
#define LPDDR_PHY_RF_PHY_IO_D3_IE_DS1 (1<<3)
#define LPDDR_PHY_RF_PHY_IO_D2_IE_DS1 (1<<2)
#define LPDDR_PHY_RF_PHY_IO_D1_IE_DS1 (1<<1)
#define LPDDR_PHY_RF_PHY_IO_D0_IE_DS1 (1<<0)

//lpddr_rf_cfg_phy_iomux_oe_wr_ds1
#define LPDDR_PHY_RF_PHY_IO_DQS_OE_DS1 (1<<9)
#define LPDDR_PHY_RF_PHY_IO_DQM_OE_DS1 (1<<8)
#define LPDDR_PHY_RF_PHY_IO_D7_OE_DS1 (1<<7)
#define LPDDR_PHY_RF_PHY_IO_D6_OE_DS1 (1<<6)
#define LPDDR_PHY_RF_PHY_IO_D5_OE_DS1 (1<<5)
#define LPDDR_PHY_RF_PHY_IO_D4_OE_DS1 (1<<4)
#define LPDDR_PHY_RF_PHY_IO_D3_OE_DS1 (1<<3)
#define LPDDR_PHY_RF_PHY_IO_D2_OE_DS1 (1<<2)
#define LPDDR_PHY_RF_PHY_IO_D1_OE_DS1 (1<<1)
#define LPDDR_PHY_RF_PHY_IO_D0_OE_DS1 (1<<0)

//lpddr_rf_cfg_phy_iomux_out_wr_ds1
#define LPDDR_PHY_RF_PHY_IO_DQS_OUT_DS1 (1<<9)
#define LPDDR_PHY_RF_PHY_IO_DQM_OUT_DS1 (1<<8)
#define LPDDR_PHY_RF_PHY_IO_D7_OUT_DS1 (1<<7)
#define LPDDR_PHY_RF_PHY_IO_D6_OUT_DS1 (1<<6)
#define LPDDR_PHY_RF_PHY_IO_D5_OUT_DS1 (1<<5)
#define LPDDR_PHY_RF_PHY_IO_D4_OUT_DS1 (1<<4)
#define LPDDR_PHY_RF_PHY_IO_D3_OUT_DS1 (1<<3)
#define LPDDR_PHY_RF_PHY_IO_D2_OUT_DS1 (1<<2)
#define LPDDR_PHY_RF_PHY_IO_D1_OUT_DS1 (1<<1)
#define LPDDR_PHY_RF_PHY_IO_D0_OUT_DS1 (1<<0)

//lpddr_rf_status_phy_data_in
#define LPDDR_PHY_RF_PHY_DATA_IN(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg
#define LPDDR_PHY_DRF_CLKDMEM_OUT_SEL (1<<0)

//lpddr_drf_cfg_reg_sel
#define LPDDR_PHY_DRF_REG_SEL(n)   (((n)&3)<<0)

//lpddr_drf_cfg_dqs_ie_sel_f0
#define LPDDR_PHY_DRF_DQS_IE_SEL_F0(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_oe_sel_f0
#define LPDDR_PHY_DRF_DQS_OE_SEL_F0(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_out_sel_f0
#define LPDDR_PHY_DRF_DQS_OUT_SEL_F0(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_gate_sel_f0
#define LPDDR_PHY_DRF_DQS_GATE_SEL_F0(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_data_ie_sel_f0
#define LPDDR_PHY_DRF_DATA_IE_SEL_F0(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_data_oe_sel_f0
#define LPDDR_PHY_DRF_DATA_OE_SEL_F0(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_read_en_sel_f0
#define LPDDR_PHY_DRF_READ_EN_SEL_F0(n) (((n)&15)<<0)

//lpddr_drf_cfg_dqs_ie_sel_f1
#define LPDDR_PHY_DRF_DQS_IE_SEL_F1(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_oe_sel_f1
#define LPDDR_PHY_DRF_DQS_OE_SEL_F1(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_out_sel_f1
#define LPDDR_PHY_DRF_DQS_OUT_SEL_F1(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_gate_sel_f1
#define LPDDR_PHY_DRF_DQS_GATE_SEL_F1(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_data_ie_sel_f1
#define LPDDR_PHY_DRF_DATA_IE_SEL_F1(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_data_oe_sel_f1
#define LPDDR_PHY_DRF_DATA_OE_SEL_F1(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_read_en_sel_f1
#define LPDDR_PHY_DRF_READ_EN_SEL_F1(n) (((n)&15)<<0)

//lpddr_drf_cfg_dqs_ie_sel_f2
#define LPDDR_PHY_DRF_DQS_IE_SEL_F2(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_oe_sel_f2
#define LPDDR_PHY_DRF_DQS_OE_SEL_F2(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_out_sel_f2
#define LPDDR_PHY_DRF_DQS_OUT_SEL_F2(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_gate_sel_f2
#define LPDDR_PHY_DRF_DQS_GATE_SEL_F2(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_data_ie_sel_f2
#define LPDDR_PHY_DRF_DATA_IE_SEL_F2(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_data_oe_sel_f2
#define LPDDR_PHY_DRF_DATA_OE_SEL_F2(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_read_en_sel_f2
#define LPDDR_PHY_DRF_READ_EN_SEL_F2(n) (((n)&15)<<0)

//lpddr_drf_cfg_dqs_ie_sel_f3
#define LPDDR_PHY_DRF_DQS_IE_SEL_F3(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_oe_sel_f3
#define LPDDR_PHY_DRF_DQS_OE_SEL_F3(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_out_sel_f3
#define LPDDR_PHY_DRF_DQS_OUT_SEL_F3(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_dqs_gate_sel_f3
#define LPDDR_PHY_DRF_DQS_GATE_SEL_F3(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_data_ie_sel_f3
#define LPDDR_PHY_DRF_DATA_IE_SEL_F3(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_data_oe_sel_f3
#define LPDDR_PHY_DRF_DATA_OE_SEL_F3(n) (((n)&0xFFFF)<<0)

//lpddr_drf_cfg_read_en_sel_f3
#define LPDDR_PHY_DRF_READ_EN_SEL_F3(n) (((n)&15)<<0)

//lpddr_drf_cfg_dll_mode_f0
#define LPDDR_PHY_DRF_DLL_SATU_MODE_F0 (1<<2)
#define LPDDR_PHY_DRF_DLL_HALF_MODE_F0 (1<<1)
#define LPDDR_PHY_DRF_DLL_CLK_MODE_F0 (1<<0)

//lpddr_drf_cfg_dll_cnt_f0
#define LPDDR_PHY_DRF_DLL_AUTO_CNT_F0(n) (((n)&0x3FF)<<20)
#define LPDDR_PHY_DRF_DLL_SATU_CNT_F0(n) (((n)&0x3FF)<<10)
#define LPDDR_PHY_DRF_DLL_INIT_CNT_F0(n) (((n)&0x3FF)<<0)

//lpddr_drf_cfg_dll_mode_f1
#define LPDDR_PHY_DRF_DLL_SATU_MODE_F1 (1<<2)
#define LPDDR_PHY_DRF_DLL_HALF_MODE_F1 (1<<1)
#define LPDDR_PHY_DRF_DLL_CLK_MODE_F1 (1<<0)

//lpddr_drf_cfg_dll_cnt_f1
#define LPDDR_PHY_DRF_DLL_AUTO_CNT_F1(n) (((n)&0x3FF)<<20)
#define LPDDR_PHY_DRF_DLL_SATU_CNT_F1(n) (((n)&0x3FF)<<10)
#define LPDDR_PHY_DRF_DLL_INIT_CNT_F1(n) (((n)&0x3FF)<<0)

//lpddr_drf_cfg_dll_mode_f2
#define LPDDR_PHY_DRF_DLL_SATU_MODE_F2 (1<<2)
#define LPDDR_PHY_DRF_DLL_HALF_MODE_F2 (1<<1)
#define LPDDR_PHY_DRF_DLL_CLK_MODE_F2 (1<<0)

//lpddr_drf_cfg_dll_cnt_f2
#define LPDDR_PHY_DRF_DLL_AUTO_CNT_F2(n) (((n)&0x3FF)<<20)
#define LPDDR_PHY_DRF_DLL_SATU_CNT_F2(n) (((n)&0x3FF)<<10)
#define LPDDR_PHY_DRF_DLL_INIT_CNT_F2(n) (((n)&0x3FF)<<0)

//lpddr_drf_cfg_dll_mode_f3
#define LPDDR_PHY_DRF_DLL_SATU_MODE_F3 (1<<2)
#define LPDDR_PHY_DRF_DLL_HALF_MODE_F3 (1<<1)
#define LPDDR_PHY_DRF_DLL_CLK_MODE_F3 (1<<0)

//lpddr_drf_cfg_dll_cnt_f3
#define LPDDR_PHY_DRF_DLL_AUTO_CNT_F3(n) (((n)&0x3FF)<<20)
#define LPDDR_PHY_DRF_DLL_SATU_CNT_F3(n) (((n)&0x3FF)<<10)
#define LPDDR_PHY_DRF_DLL_INIT_CNT_F3(n) (((n)&0x3FF)<<0)

//lpddr_drf_train_cfg
#define LPDDR_PHY_DRF_DMC_RDLVL_GATE_EN (1<<21)
#define LPDDR_PHY_DRF_PHY_RDLVL_GATE_EN (1<<20)
#define LPDDR_PHY_DRF_DMC_RDLVL_EN (1<<17)
#define LPDDR_PHY_DRF_PHY_RDLVL_EN (1<<16)
#define LPDDR_PHY_DRF_DMC_WRLVL_EN (1<<13)
#define LPDDR_PHY_DRF_PHY_WRLVL_EN (1<<12)
#define LPDDR_PHY_DRF_PHYUPD_TYPE_3(n) (((n)&3)<<10)
#define LPDDR_PHY_DRF_PHYUPD_TYPE_2(n) (((n)&3)<<8)
#define LPDDR_PHY_DRF_PHYUPD_TYPE_1(n) (((n)&3)<<6)
#define LPDDR_PHY_DRF_PHYUPD_TYPE_0(n) (((n)&3)<<4)
#define LPDDR_PHY_DRF_PHYUPD_TYPE_SEL(n) (((n)&3)<<1)
#define LPDDR_PHY_DRF_PHYUPD_EN    (1<<0)

//lpddr_drf_mr_data_0
#define LPDDR_PHY_DRF_MR_DATA_0(n) (((n)&0xFFFFFFFF)<<0)

//lpddr_drf_mr_data_1
#define LPDDR_PHY_DRF_MR_DATA_1(n) (((n)&0xFFFFFFFF)<<0)

//lpddr_rf_irq_ctrl
#define LPDDR_PHY_RF_IRQ_EN_DLL_UNLOCK_DS1 (1<<2)
#define LPDDR_PHY_RF_IRQ_EN_DLL_UNLOCK_DS0 (1<<1)
#define LPDDR_PHY_RF_IRQ_EN_DLL_UNLOCK_AC (1<<0)

//lpddr_rf_irq_status_clr
#define LPDDR_PHY_RF_IRQ_ST_CLR_DLL_UNLOCK_DS1 (1<<2)
#define LPDDR_PHY_RF_IRQ_ST_CLR_DLL_UNLOCK_DS0 (1<<1)
#define LPDDR_PHY_RF_IRQ_ST_CLR_DLL_UNLOCK_AC (1<<0)

//lpddr_rf_irq_status
#define LPDDR_PHY_RF_IRQ_ST_DLL_UNLOCK_DS1 (1<<2)
#define LPDDR_PHY_RF_IRQ_ST_DLL_UNLOCK_DS0 (1<<1)
#define LPDDR_PHY_RF_IRQ_ST_DLL_UNLOCK_AC (1<<0)

//lpddr_rf_irq_cnt_clr
#define LPDDR_PHY_RF_IRQ_CNT_CLR_DLL_UNLOCK_DS1 (1<<2)
#define LPDDR_PHY_RF_IRQ_CNT_CLR_DLL_UNLOCK_DS0 (1<<1)
#define LPDDR_PHY_RF_IRQ_CNT_CLR_DLL_UNLOCK_AC (1<<0)

//lpddr_rf_irq_cnt_dll_unlock_ac
#define LPDDR_PHY_RF_IRQ_CNT_OVERFLOW_DLL_UNLOCK_AC (1<<31)
#define LPDDR_PHY_RF_IRQ_CNT_DLL_UNLOCK_AC(n) (((n)&0x7FFFFFFF)<<0)

//lpddr_rf_irq_cnt_dll_unlock_ds0
#define LPDDR_PHY_RF_IRQ_CNT_OVERFLOW_DLL_UNLOCK_DS0 (1<<31)
#define LPDDR_PHY_RF_IRQ_CNT_DLL_UNLOCK_DS0(n) (((n)&0x7FFFFFFF)<<0)

//lpddr_rf_irq_cnt_dll_unlock_ds1
#define LPDDR_PHY_RF_IRQ_CNT_OVERFLOW_DLL_UNLOCK_DS1 (1<<31)
#define LPDDR_PHY_RF_IRQ_CNT_DLL_UNLOCK_DS1(n) (((n)&0x7FFFFFFF)<<0)





#endif


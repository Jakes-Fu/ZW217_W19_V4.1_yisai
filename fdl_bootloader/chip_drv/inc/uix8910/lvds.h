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


#ifndef _LVDS_H_
#define _LVDS_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'lvds'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// LVDS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_LVDS_BASE              0x05101000
#else
#define REG_LVDS_BASE              0x50101000
#endif

typedef volatile struct
{
    REG32                          DLHSSB_EN;                    //0x00000000
    REG32                          ULHSSB_EN;                    //0x00000004
    REG32                          DLFIFO_CLR;                   //0x00000008
    REG32                          ULFIFO_CLR;                   //0x0000000C
    REG32                          SYNCWORD10B_D;                //0x00000010
    REG32                          SYNCWORD10B_S;                //0x00000014
    REG32                          REVERSE_MODE_EN;              //0x00000018
    REG32 Reserved_0000001C;                    //0x0000001C
    REG32                          SYNCIDX0;                     //0x00000020
    REG32                          SYNCIDX1;                     //0x00000024
    REG32 Reserved_00000028[2];                 //0x00000028
    REG32                          RX_BYTE_TRAINING_OK;          //0x00000030
    REG32                          RX_BIT_TRAINING_OK;           //0x00000034
    REG32                          ULRDY;                        //0x00000038
    REG32                          LEN_PAYLOAD;                  //0x0000003C
    REG32                          LEN_SYNC;                     //0x00000040
    REG32                          FIX_PATTERN;                  //0x00000044
    REG32                          PATTERN0;                     //0x00000048
    REG32                          PATTERN1;                     //0x0000004C
    REG32 Reserved_00000050[2];                 //0x00000050
    REG32                          SAMPLE_WIDTH;                 //0x00000058
    REG32                          DOWNSAMPLE;                   //0x0000005C
    REG32                          TX_BIT_TRAINING_OK;           //0x00000060
    REG32                          TX_BYTE_TRAINING_OK;          //0x00000064
    REG32                          LATCH_WORD0_0;                //0x00000068
    REG32                          LATCH_WORD0_1;                //0x0000006C
    REG32                          LATCH_WORD1_0;                //0x00000070
    REG32                          LATCH_WORD1_1;                //0x00000074
    REG32                          STATE_MONITOR;                //0x00000078
    REG32 Reserved_0000007C[3];                 //0x0000007C
    REG32                          RES_LVDS_BB;                  //0x00000088
    REG32                          CLOCK_LVDS;                   //0x0000008C
    REG32 Reserved_00000090;                    //0x00000090
    REG32                          LATCH;                        //0x00000094
    REG32                          HEADER_CONFIG;                //0x00000098
    REG32                          RX_STATE_STATUS;              //0x0000009C
    REG32                          TX_STATE_STATUS;              //0x000000A0
    REG32                          RX_PLL_STABLE_TIME;           //0x000000A4
    REG32                          TX_PLL_STABLE_TIME;           //0x000000A8
    REG32                          INTERRUPT;                    //0x000000AC
    REG32                          INTERRUPT_CTRL;               //0x000000B0
    REG32                          LANE0_BIST_RESULT;            //0x000000B4
    REG32                          LANE1_BIST_RESULT;            //0x000000B8
    REG32 Reserved_000000BC[17];                //0x000000BC
    REG32                          LVDS_RESET;                   //0x00000100
    REG32                          CLK_BAND;                     //0x00000104
    REG32                          PHASE_SEL;                    //0x00000108
    REG32                          RX_DATA_PNSW;                 //0x0000010C
    REG32                          RX_DL8P_SEL;                  //0x00000110
    REG32 Reserved_00000114[3];                 //0x00000114
    REG32                          RX_DL128P_SEL;                //0x00000120
    REG32                          RX_PU_DIFF2CMOS;              //0x00000124
    REG32                          RX_VCOM_SEL;                  //0x00000128
    REG32                          TX_DL8P_SEL;                  //0x0000012C
    REG32                          TX_DL128P_SEL;                //0x00000130
    REG32                          TX_HZ_ENABLE;                 //0x00000134
    REG32                          TX_IREF_BIT;                  //0x00000138
    REG32                          TX_VCM_BIT;                   //0x0000013C
    REG32                          TX_VDM_BIT;                   //0x00000140
    REG32                          REG_RES;                      //0x00000144
    REG32                          PLL_REFDIV2_ENABLE;           //0x00000148
    REG32                          PLL_CLK_RSTB;                 //0x0000014C
    REG32 Reserved_00000150[3];                 //0x00000150
    REG32                          PLL_DIN;                      //0x0000015C
    REG32                          PLL_PCON;                     //0x00000160
    REG32                          PLL_REFMULTI2_ENABLE;         //0x00000164
    REG32 Reserved_00000168[6];                 //0x00000168
    REG32                          PLL_VREG_BIT;                 //0x00000180
    REG32                          PU_PLL;                       //0x00000184
    REG32                          PLL_LOCK;                     //0x00000188
    REG32                          PLL_REG0;                     //0x0000018C
    REG32                          PLL_REG1;                     //0x00000190
    REG32                          LVDS_MONITOR_SELECT;          //0x00000194
    REG32                          LANE1_PATTERN0;               //0x00000198
    REG32                          LANE1_PATTERN1;               //0x0000019C
    REG32                          LVDS2DFE_LATCH_REG_0;         //0x000001A0
    REG32                          LVDS2DFE_LATCH_REG_1;         //0x000001A4
    REG32                          TOPBIST_CONTROL;              //0x000001A8
} HWP_LVDS_T;

#define hwp_lvds                   ((HWP_LVDS_T*) REG_ACCESS_ADDRESS(REG_LVDS_BASE))


//DLHSSB_EN
typedef union {
    REG32 v;
    struct {
        REG32 config_dlhssb_en : 2; // [1:0]
        REG32 __31_2 : 30;
    } b;
} REG_LVDS_DLHSSB_EN_T;

//ULHSSB_EN
typedef union {
    REG32 v;
    struct {
        REG32 config_ulhssb_en : 2; // [1:0]
        REG32 __31_2 : 30;
    } b;
} REG_LVDS_ULHSSB_EN_T;

//DLFIFO_CLR
typedef union {
    REG32 v;
    struct {
        REG32 config_dlfifo_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_DLFIFO_CLR_T;

//ULFIFO_CLR
typedef union {
    REG32 v;
    struct {
        REG32 config_ulfifo_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_ULFIFO_CLR_T;

//SYNCWORD10B_D
typedef union {
    REG32 v;
    struct {
        REG32 config_syncword10b_d : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_LVDS_SYNCWORD10B_D_T;

//SYNCWORD10B_S
typedef union {
    REG32 v;
    struct {
        REG32 config_syncword10b_s : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_LVDS_SYNCWORD10B_S_T;

//REVERSE_MODE_EN
typedef union {
    REG32 v;
    struct {
        REG32 config_reverse_mode_en : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_REVERSE_MODE_EN_T;

//SYNCIDX0
typedef union {
    REG32 v;
    struct {
        REG32 config_sync_idx0 : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_LVDS_SYNCIDX0_T;

//SYNCIDX1
typedef union {
    REG32 v;
    struct {
        REG32 config_sync_idx1 : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_LVDS_SYNCIDX1_T;

//RX_BYTE_TRAINING_OK
typedef union {
    REG32 v;
    struct {
        REG32 config_rx_byte_training_ok : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_RX_BYTE_TRAINING_OK_T;

//RX_BIT_TRAINING_OK
typedef union {
    REG32 v;
    struct {
        REG32 config_rx_bit_training_ok : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_RX_BIT_TRAINING_OK_T;

//ULRDY
typedef union {
    REG32 v;
    struct {
        REG32 config_ul_rdy : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_ULRDY_T;

//LEN_PAYLOAD
typedef union {
    REG32 v;
    struct {
        REG32 config_len_payload : 10; // [9:0]
        REG32 __31_10 : 22;
    } b;
} REG_LVDS_LEN_PAYLOAD_T;

//LEN_SYNC
typedef union {
    REG32 v;
    struct {
        REG32 config_len_sync : 10; // [9:0]
        REG32 __31_10 : 22;
    } b;
} REG_LVDS_LEN_SYNC_T;

//FIX_PATTERN
typedef union {
    REG32 v;
    struct {
        REG32 config_fix_pattern : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_FIX_PATTERN_T;

//SAMPLE_WIDTH
typedef union {
    REG32 v;
    struct {
        REG32 config_sample_width : 2; // [1:0]
        REG32 __31_2 : 30;
    } b;
} REG_LVDS_SAMPLE_WIDTH_T;

//DOWNSAMPLE
typedef union {
    REG32 v;
    struct {
        REG32 config_1x2x : 2; // [1:0]
        REG32 config_mt_en : 2; // [3:2]
        REG32 config_rate_cnt_rx : 2; // [5:4]
        REG32 config_rate_cnt_tx : 2; // [7:6]
        REG32 config_mode_mt_rx : 2; // [9:8]
        REG32 config_mode_mt_tx : 2; // [11:10]
        REG32 __31_12 : 20;
    } b;
} REG_LVDS_DOWNSAMPLE_T;

//TX_BIT_TRAINING_OK
typedef union {
    REG32 v;
    struct {
        REG32 config_tx_bit_training_ok : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_TX_BIT_TRAINING_OK_T;

//TX_BYTE_TRAINING_OK
typedef union {
    REG32 v;
    struct {
        REG32 config_tx_byte_training_ok : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_TX_BYTE_TRAINING_OK_T;

//STATE_MONITOR
typedef union {
    REG32 v;
    struct {
        REG32 src_lane0_mstate : 3; // [2:0], read only
        REG32 src_lane0_state : 3; // [5:3], read only
        REG32 src_lane1_mstate : 3; // [8:6], read only
        REG32 src_lane1_state : 3; // [11:9], read only
        REG32 dest_lane0_mstate : 3; // [14:12], read only
        REG32 dest_lane0_state : 3; // [17:15], read only
        REG32 dest_lane1_mstate : 3; // [20:18], read only
        REG32 dest_lane1_state : 3; // [23:21], read only
        REG32 dest_lane0_fstate : 3; // [26:24], read only
        REG32 dest_lane1_fstate : 3; // [29:27], read only
        REG32 __31_30 : 2;
    } b;
} REG_LVDS_STATE_MONITOR_T;

//RES_LVDS_BB
typedef union {
    REG32 v;
    struct {
        REG32 config_res_lvds_bb : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LVDS_RES_LVDS_BB_T;

//CLOCK_LVDS
typedef union {
    REG32 v;
    struct {
        REG32 config_clock_lvds : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LVDS_CLOCK_LVDS_T;

//LATCH
typedef union {
    REG32 v;
    struct {
        REG32 config_latch : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_LATCH_T;

//HEADER_CONFIG
typedef union {
    REG32 v;
    struct {
        REG32 config_header_len : 3; // [2:0]
        REG32 config_header : 8; // [10:3]
        REG32 __31_11 : 21;
    } b;
} REG_LVDS_HEADER_CONFIG_T;

//RX_STATE_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 lvds_rx_state_machine : 6; // [5:0], read only
        REG32 __31_6 : 26;
    } b;
} REG_LVDS_RX_STATE_STATUS_T;

//TX_STATE_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 lvds_tx_state_machine : 6; // [5:0], read only
        REG32 __31_6 : 26;
    } b;
} REG_LVDS_TX_STATE_STATUS_T;

//RX_PLL_STABLE_TIME
typedef union {
    REG32 v;
    struct {
        REG32 lvds_rx_pll_stable_time : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LVDS_RX_PLL_STABLE_TIME_T;

//TX_PLL_STABLE_TIME
typedef union {
    REG32 v;
    struct {
        REG32 lvds_tx_pll_stable_time : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LVDS_TX_PLL_STABLE_TIME_T;

//INTERRUPT
typedef union {
    REG32 v;
    struct {
        REG32 lvds_interrupt : 1; // [0], read only
        REG32 lvds_interrupt_raw : 1; // [1], read only
        REG32 __31_2 : 30;
    } b;
} REG_LVDS_INTERRUPT_T;

//INTERRUPT_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 lvds_interrupt_mask : 1; // [0]
        REG32 lvds_interrupt_source_sel : 3; // [3:1]
        REG32 lvds_interrupt_clear : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_LVDS_INTERRUPT_CTRL_T;

//LANE0_BIST_RESULT
typedef union {
    REG32 v;
    struct {
        REG32 bist_fail_ind_lane0 : 1; // [0], read only
        REG32 bist_error_cnt_lane0 : 16; // [16:1], read only
        REG32 __31_17 : 15;
    } b;
} REG_LVDS_LANE0_BIST_RESULT_T;

//LANE1_BIST_RESULT
typedef union {
    REG32 v;
    struct {
        REG32 bist_fail_ind_lane1 : 1; // [0], read only
        REG32 bist_error_cnt_lane1 : 16; // [16:1], read only
        REG32 __31_17 : 15;
    } b;
} REG_LVDS_LANE1_BIST_RESULT_T;

//LVDS_RESET
typedef union {
    REG32 v;
    struct {
        REG32 lvds_tx_rstn : 1; // [0]
        REG32 lvds_rx_rstn : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_LVDS_LVDS_RESET_T;

//CLK_BAND
typedef union {
    REG32 v;
    struct {
        REG32 lvds_clk_band : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_LVDS_CLK_BAND_T;

//PHASE_SEL
typedef union {
    REG32 v;
    struct {
        REG32 lvds_phase_sel : 3; // [2:0]
        REG32 __31_3 : 29;
    } b;
} REG_LVDS_PHASE_SEL_T;

//RX_DATA_PNSW
typedef union {
    REG32 v;
    struct {
        REG32 lvds_rx_data_pnsw : 2; // [1:0]
        REG32 __31_2 : 30;
    } b;
} REG_LVDS_RX_DATA_PNSW_T;

//RX_DL128P_SEL
typedef union {
    REG32 v;
    struct {
        REG32 lvds_rx_dl128p_sel : 14; // [13:0]
        REG32 __31_14 : 18;
    } b;
} REG_LVDS_RX_DL128P_SEL_T;

//RX_PU_DIFF2CMOS
typedef union {
    REG32 v;
    struct {
        REG32 lvds_rx_pu_diff2cmos : 5; // [4:0]
        REG32 __31_5 : 27;
    } b;
} REG_LVDS_RX_PU_DIFF2CMOS_T;

//RX_VCOM_SEL
typedef union {
    REG32 v;
    struct {
        REG32 lvds_rx_vcom_sel : 5; // [4:0]
        REG32 __31_5 : 27;
    } b;
} REG_LVDS_RX_VCOM_SEL_T;

//TX_DL8P_SEL
typedef union {
    REG32 v;
    struct {
        REG32 lvds_tx_dl8p_sel : 15; // [14:0]
        REG32 __31_15 : 17;
    } b;
} REG_LVDS_TX_DL8P_SEL_T;

//TX_DL128P_SEL
typedef union {
    REG32 v;
    struct {
        REG32 lvds_tx_dl128p_sel : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_LVDS_TX_DL128P_SEL_T;

//TX_HZ_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 lvds_tx_hz_enable : 5; // [4:0]
        REG32 __31_5 : 27;
    } b;
} REG_LVDS_TX_HZ_ENABLE_T;

//TX_IREF_BIT
typedef union {
    REG32 v;
    struct {
        REG32 lvds_tx_iref_bit : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_LVDS_TX_IREF_BIT_T;

//TX_VCM_BIT
typedef union {
    REG32 v;
    struct {
        REG32 lvds_tx_vcm_bit : 3; // [2:0]
        REG32 __31_3 : 29;
    } b;
} REG_LVDS_TX_VCM_BIT_T;

//TX_VDM_BIT
typedef union {
    REG32 v;
    struct {
        REG32 lvds_tx_vdm_bit : 3; // [2:0]
        REG32 __31_3 : 29;
    } b;
} REG_LVDS_TX_VDM_BIT_T;

//PLL_REFDIV2_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 lvds_pll_refdiv2_enable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_PLL_REFDIV2_ENABLE_T;

//PLL_CLK_RSTB
typedef union {
    REG32 v;
    struct {
        REG32 lvds_pll_clk_rstb : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_PLL_CLK_RSTB_T;

//PLL_DIN
typedef union {
    REG32 v;
    struct {
        REG32 lvds_pll_din : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_LVDS_PLL_DIN_T;

//PLL_PCON
typedef union {
    REG32 v;
    struct {
        REG32 lvds_pll_pcon : 3; // [2:0]
        REG32 __31_3 : 29;
    } b;
} REG_LVDS_PLL_PCON_T;

//PLL_REFMULTI2_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 lvds_pll_refmulti2_en : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_PLL_REFMULTI2_ENABLE_T;

//PLL_VREG_BIT
typedef union {
    REG32 v;
    struct {
        REG32 lvds_pll_vreg_bit : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_LVDS_PLL_VREG_BIT_T;

//PU_PLL
typedef union {
    REG32 v;
    struct {
        REG32 lvds_pu_pll : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_PU_PLL_T;

//PLL_LOCK
typedef union {
    REG32 v;
    struct {
        REG32 lvds_pll_lock : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_LVDS_PLL_LOCK_T;

//PLL_REG0
typedef union {
    REG32 v;
    struct {
        REG32 lvds_pll_reg0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LVDS_PLL_REG0_T;

//PLL_REG1
typedef union {
    REG32 v;
    struct {
        REG32 lvds_pll_reg1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LVDS_PLL_REG1_T;

//LVDS_MONITOR_SELECT
typedef union {
    REG32 v;
    struct {
        REG32 lvds_monitor_sel : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_LVDS_LVDS_MONITOR_SELECT_T;

//TOPBIST_CONTROL
typedef union {
    REG32 v;
    struct {
        REG32 err_cnt : 16; // [15:0], read only
        REG32 err_flag : 1; // [16], read only
        REG32 top2lvds_bist_en : 1; // [17]
        REG32 __31_18 : 14;
    } b;
} REG_LVDS_TOPBIST_CONTROL_T;


//DLHSSB_EN
#define LVDS_DLHSSB_EN_RESERVED_0(n) (((n)&0x3FFFFFFF)<<2)
#define LVDS_CONFIG_DLHSSB_EN(n)   (((n)&3)<<0)

//ULHSSB_EN
#define LVDS_ULHSSB_EN_RESERVED_0(n) (((n)&0x3FFFFFFF)<<2)
#define LVDS_CONFIG_ULHSSB_EN(n)   (((n)&3)<<0)

//DLFIFO_CLR
#define LVDS_DLFIFO_CLR_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_DLFIFO_CLR     (1<<0)

//ULFIFO_CLR
#define LVDS_ULFIFO_CLR_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_ULFIFO_CLR     (1<<0)

//SYNCWORD10B_D
#define LVDS_SYNCWORD10B_D_RESERVED_0(n) (((n)&0xFFFFFF)<<8)
#define LVDS_CONFIG_SYNCWORD10B_D(n) (((n)&0xFF)<<0)

//SYNCWORD10B_S
#define LVDS_SYNCWORD10B_S_RESERVED_0(n) (((n)&0xFFFFFF)<<8)
#define LVDS_CONFIG_SYNCWORD10B_S(n) (((n)&0xFF)<<0)

//REVERSE_MODE_EN
#define LVDS_REVERSE_MODE_EN_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_REVERSE_MODE_EN (1<<0)

//SYNCIDX0
#define LVDS_SYNCIDX0_RESERVED_0(n) (((n)&0xFFFFFFF)<<4)
#define LVDS_CONFIG_SYNC_IDX0(n)   (((n)&15)<<0)

//SYNCIDX1
#define LVDS_SYNCIDX1_RESERVED_0(n) (((n)&0xFFFFFFF)<<4)
#define LVDS_CONFIG_SYNC_IDX1(n)   (((n)&15)<<0)

//RX_BYTE_TRAINING_OK
#define LVDS_RX_BYTE_TRAINING_OK_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_RX_BYTE_TRAINING_OK (1<<0)

//RX_BIT_TRAINING_OK
#define LVDS_RX_BIT_TRAINING_OK_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_RX_BIT_TRAINING_OK (1<<0)

//ULRDY
#define LVDS_ULRDY_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_UL_RDY         (1<<0)

//LEN_PAYLOAD
#define LVDS_LEN_PAYLOAD_RESERVED_0(n) (((n)&0x3FFFFF)<<10)
#define LVDS_CONFIG_LEN_PAYLOAD(n) (((n)&0x3FF)<<0)

//LEN_SYNC
#define LVDS_LEN_SYNC_RESERVED_0(n) (((n)&0x3FFFFF)<<10)
#define LVDS_CONFIG_LEN_SYNC(n)    (((n)&0x3FF)<<0)

//FIX_PATTERN
#define LVDS_FIX_PATTERN_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_FIX_PATTERN    (1<<0)

//PATTERN0
#define LVDS_CONFIG_PATTERN0(n)    (((n)&0xFFFFFFFF)<<0)

//PATTERN1
#define LVDS_CONFIG_PATTERN1(n)    (((n)&0xFFFFFFFF)<<0)

//SAMPLE_WIDTH
#define LVDS_SAMPLE_WIDTH_RESERVED_0(n) (((n)&0x3FFFFFFF)<<2)
#define LVDS_CONFIG_SAMPLE_WIDTH(n) (((n)&3)<<0)

//DOWNSAMPLE
#define LVDS_DOWNSAMPLE_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define LVDS_CONFIG_MODE_MT_TX(n)  (((n)&3)<<10)
#define LVDS_CONFIG_MODE_MT_RX(n)  (((n)&3)<<8)
#define LVDS_CONFIG_RATE_CNT_TX(n) (((n)&3)<<6)
#define LVDS_CONFIG_RATE_CNT_RX(n) (((n)&3)<<4)
#define LVDS_CONFIG_MT_EN(n)       (((n)&3)<<2)
#define LVDS_CONFIG_1X2X(n)        (((n)&3)<<0)

//TX_BIT_TRAINING_OK
#define LVDS_TX_BIT_TRAINING_OK_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_TX_BIT_TRAINING_OK (1<<0)

//TX_BYTE_TRAINING_OK
#define LVDS_TX_BYTE_TRAINING_OK_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_TX_BYTE_TRAINING_OK (1<<0)

//LATCH_WORD0_0
#define LVDS_REG_LATCH_WORD0_0(n)  (((n)&0xFFFFFFFF)<<0)

//LATCH_WORD0_1
#define LVDS_REG_LATCH_WORD0_1(n)  (((n)&0xFFFFFFFF)<<0)

//LATCH_WORD1_0
#define LVDS_REG_LATCH_WORD1_0(n)  (((n)&0xFFFFFFFF)<<0)

//LATCH_WORD1_1
#define LVDS_REG_LATCH_WORD1_1(n)  (((n)&0xFFFFFFFF)<<0)

//STATE_MONITOR
#define LVDS_STATE_MONITOR_RESERVED_0(n) (((n)&3)<<30)
#define LVDS_DEST_LANE1_FSTATE(n)  (((n)&7)<<27)
#define LVDS_DEST_LANE0_FSTATE(n)  (((n)&7)<<24)
#define LVDS_DEST_LANE1_STATE(n)   (((n)&7)<<21)
#define LVDS_DEST_LANE1_MSTATE(n)  (((n)&7)<<18)
#define LVDS_DEST_LANE0_STATE(n)   (((n)&7)<<15)
#define LVDS_DEST_LANE0_MSTATE(n)  (((n)&7)<<12)
#define LVDS_SRC_LANE1_STATE(n)    (((n)&7)<<9)
#define LVDS_SRC_LANE1_MSTATE(n)   (((n)&7)<<6)
#define LVDS_SRC_LANE0_STATE(n)    (((n)&7)<<3)
#define LVDS_SRC_LANE0_MSTATE(n)   (((n)&7)<<0)

//RES_LVDS_BB
#define LVDS_RES_LVDS_BB_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define LVDS_CONFIG_RES_LVDS_BB(n) (((n)&0xFFFF)<<0)

//CLOCK_LVDS
#define LVDS_CLOCK_LVDS_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define LVDS_CONFIG_CLOCK_LVDS(n)  (((n)&0xFFFF)<<0)

//LATCH
#define LVDS_LATCH_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_LATCH          (1<<0)

//HEADER_CONFIG
#define LVDS_HEADER_CONFIG_RESERVED_0(n) (((n)&0x1FFFFF)<<11)
#define LVDS_CONFIG_HEADER(n)      (((n)&0xFF)<<3)
#define LVDS_CONFIG_HEADER_LEN(n)  (((n)&7)<<0)

//RX_STATE_STATUS
#define LVDS_RX_STATE_STATUS_RESERVED_0(n) (((n)&0x3FFFFFF)<<6)
#define LVDS_LVDS_RX_STATE_MACHINE(n) (((n)&0x3F)<<0)

//TX_STATE_STATUS
#define LVDS_TX_STATE_STATUS_RESERVED_0(n) (((n)&0x3FFFFFF)<<6)
#define LVDS_LVDS_TX_STATE_MACHINE(n) (((n)&0x3F)<<0)

//RX_PLL_STABLE_TIME
#define LVDS_RX_PLL_STABLE_TIME_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define LVDS_LVDS_RX_PLL_STABLE_TIME(n) (((n)&0xFFFF)<<0)

//TX_PLL_STABLE_TIME
#define LVDS_TX_PLL_STABLE_TIME_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define LVDS_LVDS_TX_PLL_STABLE_TIME(n) (((n)&0xFFFF)<<0)

//INTERRUPT
#define LVDS_INTERRUPT_RESERVED_0(n) (((n)&0x3FFFFFFF)<<2)
#define LVDS_LVDS_INTERRUPT_RAW    (1<<1)
#define LVDS_LVDS_INTERRUPT        (1<<0)

//INTERRUPT_CTRL
#define LVDS_INTERRUPT_CTRL_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define LVDS_LVDS_INTERRUPT_CLEAR  (1<<4)
#define LVDS_LVDS_INTERRUPT_SOURCE_SEL(n) (((n)&7)<<1)
#define LVDS_LVDS_INTERRUPT_MASK   (1<<0)

//LANE0_BIST_RESULT
#define LVDS_LANE0_BIST_RESULT_RESERVED_0(n) (((n)&0x7FFF)<<17)
#define LVDS_BIST_ERROR_CNT_LANE0(n) (((n)&0xFFFF)<<1)
#define LVDS_BIST_FAIL_IND_LANE0   (1<<0)

//LANE1_BIST_RESULT
#define LVDS_LANE1_BIST_RESULT_RESERVED_0(n) (((n)&0x7FFF)<<17)
#define LVDS_BIST_ERROR_CNT_LANE1(n) (((n)&0xFFFF)<<1)
#define LVDS_BIST_FAIL_IND_LANE1   (1<<0)

//LVDS_RESET
#define LVDS_LVDS_RESET_RESERVED_0(n) (((n)&0x3FFFFFFF)<<2)
#define LVDS_LVDS_RX_RSTN          (1<<1)
#define LVDS_LVDS_TX_RSTN          (1<<0)

//CLK_BAND
#define LVDS_CLK_BAND_RESERVED_0(n) (((n)&0xFFFFFFF)<<4)
#define LVDS_LVDS_CLK_BAND(n)      (((n)&15)<<0)

//PHASE_SEL
#define LVDS_PHASE_SEL_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define LVDS_LVDS_PHASE_SEL(n)     (((n)&7)<<0)

//RX_DATA_PNSW
#define LVDS_RX_DATA_PNSW_RESERVED_0(n) (((n)&0x3FFFFFFF)<<2)
#define LVDS_LVDS_RX_DATA_PNSW(n)  (((n)&3)<<0)

//RX_DL8P_SEL
#define LVDS_LVDS_RX_DL8P_SEL(n)   (((n)&0xFFFFFFFF)<<0)

//RX_DL128P_SEL
#define LVDS_RX_DL128P_SEL_RESERVED_0(n) (((n)&0x3FFFF)<<14)
#define LVDS_LVDS_RX_DL128P_SEL(n) (((n)&0x3FFF)<<0)

//RX_PU_DIFF2CMOS
#define LVDS_RX_PU_DIFF2CMOS_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define LVDS_LVDS_RX_PU_DIFF2CMOS(n) (((n)&31)<<0)

//RX_VCOM_SEL
#define LVDS_RX_VCOM_SEL_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define LVDS_LVDS_RX_VCOM_SEL(n)   (((n)&31)<<0)

//TX_DL8P_SEL
#define LVDS_TX_DL8P_SEL_RESERVED_0(n) (((n)&0x1FFFF)<<15)
#define LVDS_LVDS_TX_DL8P_SEL(n)   (((n)&0x7FFF)<<0)

//TX_DL128P_SEL
#define LVDS_TX_DL128P_SEL_RESERVED_0(n) (((n)&0x1FFFFFF)<<7)
#define LVDS_LVDS_TX_DL128P_SEL(n) (((n)&0x7F)<<0)

//TX_HZ_ENABLE
#define LVDS_TX_HZ_ENABLE_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define LVDS_LVDS_TX_HZ_ENABLE(n)  (((n)&31)<<0)

//TX_IREF_BIT
#define LVDS_TX_IREF_BIT_RESERVED_0(n) (((n)&0xFFFFFFF)<<4)
#define LVDS_LVDS_TX_IREF_BIT(n)   (((n)&15)<<0)

//TX_VCM_BIT
#define LVDS_TX_VCM_BIT_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define LVDS_LVDS_TX_VCM_BIT(n)    (((n)&7)<<0)

//TX_VDM_BIT
#define LVDS_TX_VDM_BIT_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define LVDS_LVDS_TX_VDM_BIT(n)    (((n)&7)<<0)

//REG_RES
#define LVDS_LVDS_REG_RES(n)       (((n)&0xFFFFFFFF)<<0)

//PLL_REFDIV2_ENABLE
#define LVDS_PLL_REFDIV2_ENABLE_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_LVDS_PLL_REFDIV2_ENABLE (1<<0)

//PLL_CLK_RSTB
#define LVDS_PLL_CLK_RSTB_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_LVDS_PLL_CLK_RSTB     (1<<0)

//PLL_DIN
#define LVDS_PLL_DIN_RESERVED_0(n) (((n)&0x1FFFFFF)<<7)
#define LVDS_LVDS_PLL_DIN(n)       (((n)&0x7F)<<0)

//PLL_PCON
#define LVDS_PLL_PCON_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define LVDS_LVDS_PLL_PCON(n)      (((n)&7)<<0)

//PLL_REFMULTI2_ENABLE
#define LVDS_PLL_REFMULTI2_ENABLE_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_LVDS_PLL_REFMULTI2_EN (1<<0)

//PLL_VREG_BIT
#define LVDS_PLL_VREG_BIT_RESERVED_0(n) (((n)&0xFFFFFFF)<<4)
#define LVDS_LVDS_PLL_VREG_BIT(n)  (((n)&15)<<0)

//PU_PLL
#define LVDS_PU_PLL_RESERVED_0(n)  (((n)&0x7FFFFFFF)<<1)
#define LVDS_LVDS_PU_PLL           (1<<0)

//PLL_LOCK
#define LVDS_PLL_LOCK_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define LVDS_LVDS_PLL_LOCK         (1<<0)

//PLL_REG0
#define LVDS_PLL_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define LVDS_LVDS_PLL_REG0(n)      (((n)&0xFFFF)<<0)

//PLL_REG1
#define LVDS_PLL_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define LVDS_LVDS_PLL_REG1(n)      (((n)&0xFFFF)<<0)

//LVDS_MONITOR_SELECT
#define LVDS_LVDS_MONITOR_SELECT_RESERVED_0(n) (((n)&0xFFFFFFF)<<4)
#define LVDS_LVDS_MONITOR_SEL(n)   (((n)&15)<<0)

//LANE1_PATTERN0
#define LVDS_CONFIG_LANE1_PATTERN0(n) (((n)&0xFFFFFFFF)<<0)

//LANE1_PATTERN1
#define LVDS_CONFIG_LANE1_PATTERN1(n) (((n)&0xFFFFFFFF)<<0)

//LVDS2DFE_LATCH_REG_0
#define LVDS_LVDS2DFE_LATCH_REG0(n) (((n)&0xFFFFFFFF)<<0)

//LVDS2DFE_LATCH_REG_1
#define LVDS_LVDS2DFE_LATCH_REG1(n) (((n)&0xFFFFFFFF)<<0)

//TOPBIST_CONTROL
#define LVDS_TOPBIST_CONTROL_RESERVED_0(n) (((n)&0x3FFF)<<18)
#define LVDS_TOP2LVDS_BIST_EN      (1<<17)
#define LVDS_ERR_FLAG              (1<<16)
#define LVDS_ERR_CNT(n)            (((n)&0xFFFF)<<0)





#endif


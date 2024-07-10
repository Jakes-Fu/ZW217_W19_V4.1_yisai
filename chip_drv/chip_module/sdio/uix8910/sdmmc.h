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


#ifndef _SDMMC_H_
#define _SDMMC_H_
#include "global_macros.h"
#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'sdmmc'."
#endif

typedef volatile unsigned int REG32;


// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SDMMC_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_SDMMC_BASE             0x08810000
#define REG_SDMMC2_BASE            0x08811000

typedef volatile struct
{
    REG32                          apbi_ctrl_sdmmc;              //0x00000000
    REG32 Reserved_00000004;                    //0x00000004
    REG32                          APBI_FIFO_TxRx;               //0x00000008
    REG32 Reserved_0000000C[509];               //0x0000000C
    REG32                          SDMMC_CONFIG;                 //0x00000800
    REG32                          SDMMC_STATUS;                 //0x00000804
    REG32                          SDMMC_CMD_INDEX;              //0x00000808
    REG32                          SDMMC_CMD_ARG;                //0x0000080C
    REG32                          SDMMC_RESP_INDEX;             //0x00000810
    REG32                          SDMMC_RESP_ARG3;              //0x00000814
    REG32                          SDMMC_RESP_ARG2;              //0x00000818
    REG32                          SDMMC_RESP_ARG1;              //0x0000081C
    REG32                          SDMMC_RESP_ARG0;              //0x00000820
    REG32                          SDMMC_DATA_WIDTH;         //0x00000824
    REG32                          SDMMC_BLOCK_SIZE;         //0x00000828
    REG32                          SDMMC_BLOCK_CNT;          //0x0000082C
    REG32                          SDMMC_INT_STATUS;             //0x00000830
    REG32                          SDMMC_INT_MASK;               //0x00000834
    REG32                          SDMMC_INT_CLEAR;              //0x00000838
    REG32                          SDMMC_TRANS_SPEED;        //0x0000083C
    REG32                          SDMMC_MCLK_ADJUST;        //0x00000840
} HWP_SDMMC_T;

#define hwp_sdmmc                  ((HWP_SDMMC_T*) REG_ACCESS_ADDRESS(REG_SDMMC_BASE))
#define hwp_sdmmc2                 ((HWP_SDMMC_T*) REG_ACCESS_ADDRESS(REG_SDMMC2_BASE))


//apbi_ctrl_sdmmc
typedef union {
    REG32 v;
    struct {
        REG32 l_endian : 3; // [2:0]
        REG32 soft_rst_l : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_SDMMC_APBI_CTRL_SDMMC_T;

//APBI_FIFO_TxRx
typedef union {
    REG32 v;
    struct {
        REG32 data_in : 32; // [31:0]
        REG32 data_out : 32; // [31:0], read only
    } b;
} REG_SDMMC_APBI_FIFO_TXRX_T;

//SDMMC_CONFIG
typedef union {
    REG32 v;
    struct {
        REG32 sdmmc_sendcmd : 1; // [0]
        REG32 sdmmc_suspend : 1; // [1]
        REG32 __3_2 : 2;
        REG32 rsp_en : 1; // [4]
        REG32 rsp_sel : 2; // [6:5]
        REG32 __7_7 : 1;
        REG32 rd_wt_en : 1; // [8]
        REG32 rd_wt_sel : 1; // [9]
        REG32 s_m_sel : 1; // [10]
        REG32 __15_11 : 5;
        REG32 bit_16 : 1; // [16]
        REG32 __31_17 : 15;
    } b;
} REG_SDMMC_SDMMC_CONFIG_T;

//SDMMC_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 not_sdmmc_over : 1; // [0], read only
        REG32 busy : 1; // [1], read only
        REG32 dl_busy : 1; // [2], read only
        REG32 suspend : 1; // [3], read only
        REG32 __7_4 : 4;
        REG32 rsp_error : 1; // [8], read only
        REG32 no_rsp_error : 1; // [9], read only
        REG32 __11_10 : 2;
        REG32 crc_status : 3; // [14:12], read only
        REG32 __15_15 : 1;
        REG32 data_error : 8; // [23:16], read only
        REG32 dat3_val : 1; // [24], read only
        REG32 __31_25 : 7;
    } b;
} REG_SDMMC_SDMMC_STATUS_T;

//SDMMC_CMD_INDEX
typedef union {
    REG32 v;
    struct {
        REG32 command : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_SDMMC_SDMMC_CMD_INDEX_T;

//SDMMC_RESP_INDEX
typedef union {
    REG32 v;
    struct {
        REG32 response : 6; // [5:0], read only
        REG32 __31_6 : 26;
    } b;
} REG_SDMMC_SDMMC_RESP_INDEX_T;

//SDMMC_DATA_WIDTH_REG
typedef union {
    REG32 v;
    struct {
        REG32 sdmmc_data_width : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_SDMMC_SDMMC_DATA_WIDTH_REG_T;

//SDMMC_BLOCK_SIZE_REG
typedef union {
    REG32 v;
    struct {
        REG32 sdmmc_block_size : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_SDMMC_SDMMC_BLOCK_SIZE_REG_T;

//SDMMC_BLOCK_CNT_REG
typedef union {
    REG32 v;
    struct {
        REG32 sdmmc_block_cnt : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_SDMMC_SDMMC_BLOCK_CNT_REG_T;

//SDMMC_INT_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 no_rsp_int : 1; // [0], read only
        REG32 rsp_err_int : 1; // [1], read only
        REG32 rd_err_int : 1; // [2], read only
        REG32 wr_err_int : 1; // [3], read only
        REG32 dat_over_int : 1; // [4], read only
        REG32 txdma_done_int : 1; // [5], read only
        REG32 rxdma_done_int : 1; // [6], read only
        REG32 __7_7 : 1;
        REG32 no_rsp_sc : 1; // [8], read only
        REG32 rsp_err_sc : 1; // [9], read only
        REG32 rd_err_sc : 1; // [10], read only
        REG32 wr_err_sc : 1; // [11], read only
        REG32 dat_over_sc : 1; // [12], read only
        REG32 txdma_done_sc : 1; // [13], read only
        REG32 rxdma_done_sc : 1; // [14], read only
        REG32 __31_15 : 17;
    } b;
} REG_SDMMC_SDMMC_INT_STATUS_T;

//SDMMC_INT_MASK
typedef union {
    REG32 v;
    struct {
        REG32 no_rsp_mk : 1; // [0]
        REG32 rsp_err_mk : 1; // [1]
        REG32 rd_err_mk : 1; // [2]
        REG32 wr_err_mk : 1; // [3]
        REG32 dat_over_mk : 1; // [4]
        REG32 txdma_done_mk : 1; // [5]
        REG32 rxdma_done_mk : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_SDMMC_SDMMC_INT_MASK_T;

//SDMMC_INT_CLEAR
typedef union {
    REG32 v;
    struct {
        REG32 no_rsp_cl : 1; // [0]
        REG32 rsp_err_cl : 1; // [1]
        REG32 rd_err_cl : 1; // [2]
        REG32 wr_err_cl : 1; // [3]
        REG32 dat_over_cl : 1; // [4]
        REG32 txdma_done_cl : 1; // [5]
        REG32 rxdma_done_cl : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_SDMMC_SDMMC_INT_CLEAR_T;

//SDMMC_TRANS_SPEED_REG
typedef union {
    REG32 v;
    struct {
        REG32 sdmmc_trans_speed : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_SDMMC_SDMMC_TRANS_SPEED_REG_T;

//SDMMC_MCLK_ADJUST_REG
typedef union {
    REG32 v;
    struct {
        REG32 sdmmc_mclk_adjust : 4; // [3:0]
        REG32 clk_inv : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SDMMC_SDMMC_MCLK_ADJUST_REG_T;


//apbi_ctrl_sdmmc
#define SDMMC_L_ENDIAN(n)          (((n)&7)<<0)
#define SDMMC_L_ENDIAN_MASK        (7<<0)
#define SDMMC_L_ENDIAN_SHIFT       (0)
#define SDMMC_SOFT_RST_L           (1<<3)
#define SDMMC_SOFT_RST_L_MASK      (1<<3)
#define SDMMC_SOFT_RST_L_SHIFT     (3)

//APBI_FIFO_TxRx
#define SDMMC_DATA_IN(n)           (((n)&0xFFFFFFFF)<<0)
#define SDMMC_DATA_IN_MASK         (0xFFFFFFFF<<0)
#define SDMMC_DATA_IN_SHIFT        (0)
#define SDMMC_DATA_OUT(n)          (((n)&0xFFFFFFFF)<<0)
#define SDMMC_DATA_OUT_MASK        (0xFFFFFFFF<<0)
#define SDMMC_DATA_OUT_SHIFT       (0)

//SDMMC_CONFIG
#define SDMMC_SDMMC_SENDCMD        (1<<0)
#define SDMMC_SDMMC_SENDCMD_MASK   (1<<0)
#define SDMMC_SDMMC_SENDCMD_SHIFT  (0)
#define SDMMC_SDMMC_SUSPEND        (1<<1)
#define SDMMC_SDMMC_SUSPEND_MASK   (1<<1)
#define SDMMC_SDMMC_SUSPEND_SHIFT  (1)
#define SDMMC_RSP_EN               (1<<4)
#define SDMMC_RSP_EN_MASK          (1<<4)
#define SDMMC_RSP_EN_SHIFT         (4)
#define SDMMC_RSP_SEL(n)           (((n)&3)<<5)
#define SDMMC_RSP_SEL_MASK         (3<<5)
#define SDMMC_RSP_SEL_SHIFT        (5)
#define SDMMC_RSP_SEL_R2           (2<<5)
#define SDMMC_RSP_SEL_R3           (1<<5)
#define SDMMC_RSP_SEL_OTHER        (0<<5)
#define SDMMC_RSP_SEL_V_R2         (2)
#define SDMMC_RSP_SEL_V_R3         (1)
#define SDMMC_RSP_SEL_V_OTHER      (0)
#define SDMMC_RD_WT_EN             (1<<8)
#define SDMMC_RD_WT_EN_MASK        (1<<8)
#define SDMMC_RD_WT_EN_SHIFT       (8)
#define SDMMC_RD_WT_SEL            (1<<9)
#define SDMMC_RD_WT_SEL_MASK       (1<<9)
#define SDMMC_RD_WT_SEL_SHIFT      (9)
#define SDMMC_RD_WT_SEL_READ       (0<<9)
#define SDMMC_RD_WT_SEL_WRITE      (1<<9)
#define SDMMC_RD_WT_SEL_V_READ     (0)
#define SDMMC_RD_WT_SEL_V_WRITE    (1)
#define SDMMC_S_M_SEL              (1<<10)
#define SDMMC_S_M_SEL_MASK         (1<<10)
#define SDMMC_S_M_SEL_SHIFT        (10)
#define SDMMC_S_M_SEL_SIMPLE       (0<<10)
#define SDMMC_S_M_SEL_MULTIPLE     (1<<10)
#define SDMMC_S_M_SEL_V_SIMPLE     (0)
#define SDMMC_S_M_SEL_V_MULTIPLE   (1)
#define SDMMC_BIT_16               (1<<16)
#define SDMMC_AUTO_FLAG_EN          (1<<16)     /*原来定义*/
#define SDMMC_BIT_16_MASK          (1<<16)
#define SDMMC_BIT_16_SHIFT         (16)

//SDMMC_STATUS
#define SDMMC_NOT_SDMMC_OVER       (1<<0)
#define SDMMC_NOT_SDMMC_OVER_MASK  (1<<0)
#define SDMMC_NOT_SDMMC_OVER_SHIFT (0)
#define SDMMC_BUSY                 (1<<1)
#define SDMMC_BUSY_MASK            (1<<1)
#define SDMMC_BUSY_SHIFT           (1)
#define SDMMC_DL_BUSY              (1<<2)
#define SDMMC_DL_BUSY_MASK         (1<<2)
#define SDMMC_DL_BUSY_SHIFT        (2)
#define SDMMC_SUSPEND              (1<<3)
#define SDMMC_SUSPEND_MASK         (1<<3)
#define SDMMC_SUSPEND_SHIFT        (3)
#define SDMMC_RSP_ERROR            (1<<8)
#define SDMMC_RSP_ERROR_MASK       (1<<8)
#define SDMMC_RSP_ERROR_SHIFT      (8)
#define SDMMC_NO_RSP_ERROR         (1<<9)
#define SDMMC_NO_RSP_ERROR_MASK    (1<<9)
#define SDMMC_NO_RSP_ERROR_SHIFT   (9)
#define SDMMC_CRC_STATUS(n)        (((n)&7)<<12)
#define SDMMC_CRC_STATUS_MASK      (7<<12)
#define SDMMC_CRC_STATUS_SHIFT     (12)
#define SDMMC_DATA_ERROR(n)        (((n)&0xFF)<<16)
#define SDMMC_DATA_ERROR_MASK      (0xFF<<16)
#define SDMMC_DATA_ERROR_SHIFT     (16)
#define SDMMC_DAT3_VAL             (1<<24)
#define SDMMC_DAT3_VAL_MASK        (1<<24)
#define SDMMC_DAT3_VAL_SHIFT       (24)

//SDMMC_CMD_INDEX
#define SDMMC_COMMAND(n)           (((n)&0x3F)<<0)
#define SDMMC_COMMAND_MASK         (0x3F<<0)
#define SDMMC_COMMAND_SHIFT        (0)

//SDMMC_CMD_ARG
#define SDMMC_ARGUMENT(n)          (((n)&0xFFFFFFFF)<<0)
#define SDMMC_ARGUMENT_MASK        (0xFFFFFFFF<<0)
#define SDMMC_ARGUMENT_SHIFT       (0)

//SDMMC_RESP_INDEX
#define SDMMC_RESPONSE(n)          (((n)&0x3F)<<0)
#define SDMMC_RESPONSE_MASK        (0x3F<<0)
#define SDMMC_RESPONSE_SHIFT       (0)

//SDMMC_RESP_ARG3
#define SDMMC_ARGUMENT3(n)         (((n)&0xFFFFFFFF)<<0)
#define SDMMC_ARGUMENT3_MASK       (0xFFFFFFFF<<0)
#define SDMMC_ARGUMENT3_SHIFT      (0)

//SDMMC_RESP_ARG2
#define SDMMC_ARGUMENT2(n)         (((n)&0xFFFFFFFF)<<0)
#define SDMMC_ARGUMENT2_MASK       (0xFFFFFFFF<<0)
#define SDMMC_ARGUMENT2_SHIFT      (0)

//SDMMC_RESP_ARG1
#define SDMMC_ARGUMENT1(n)         (((n)&0xFFFFFFFF)<<0)
#define SDMMC_ARGUMENT1_MASK       (0xFFFFFFFF<<0)
#define SDMMC_ARGUMENT1_SHIFT      (0)

//SDMMC_RESP_ARG0
#define SDMMC_ARGUMENT0(n)         (((n)&0xFFFFFFFF)<<0)
#define SDMMC_ARGUMENT0_MASK       (0xFFFFFFFF<<0)
#define SDMMC_ARGUMENT0_SHIFT      (0)

//SDMMC_DATA_WIDTH_REG
#define SDMMC_SDMMC_DATA_WIDTH(n)  (((n)&15)<<0)
#define SDMMC_SDMMC_DATA_WIDTH_MASK (15<<0)
#define SDMMC_SDMMC_DATA_WIDTH_SHIFT (0)

//SDMMC_BLOCK_SIZE_REG
#define SDMMC_SDMMC_BLOCK_SIZE(n)  (((n)&15)<<0)
#define SDMMC_SDMMC_BLOCK_SIZE_MASK (15<<0)
#define SDMMC_SDMMC_BLOCK_SIZE_SHIFT (0)

//SDMMC_BLOCK_CNT_REG
#define SDMMC_SDMMC_BLOCK_CNT(n)   (((n)&0xFFFF)<<0)
#define SDMMC_SDMMC_BLOCK_CNT_MASK (0xFFFF<<0)
#define SDMMC_SDMMC_BLOCK_CNT_SHIFT (0)

//SDMMC_INT_STATUS
#define SDMMC_NO_RSP_INT           (1<<0)
#define SDMMC_NO_RSP_INT_MASK      (1<<0)
#define SDMMC_NO_RSP_INT_SHIFT     (0)
#define SDMMC_RSP_ERR_INT          (1<<1)
#define SDMMC_RSP_ERR_INT_MASK     (1<<1)
#define SDMMC_RSP_ERR_INT_SHIFT    (1)
#define SDMMC_RD_ERR_INT           (1<<2)
#define SDMMC_RD_ERR_INT_MASK      (1<<2)
#define SDMMC_RD_ERR_INT_SHIFT     (2)
#define SDMMC_WR_ERR_INT           (1<<3)
#define SDMMC_WR_ERR_INT_MASK      (1<<3)
#define SDMMC_WR_ERR_INT_SHIFT     (3)
#define SDMMC_DAT_OVER_INT         (1<<4)
#define SDMMC_DAT_OVER_INT_MASK    (1<<4)
#define SDMMC_DAT_OVER_INT_SHIFT   (4)
#define SDMMC_TXDMA_DONE_INT       (1<<5)
#define SDMMC_TXDMA_DONE_INT_MASK  (1<<5)
#define SDMMC_TXDMA_DONE_INT_SHIFT (5)
#define SDMMC_RXDMA_DONE_INT       (1<<6)
#define SDMMC_RXDMA_DONE_INT_MASK  (1<<6)
#define SDMMC_RXDMA_DONE_INT_SHIFT (6)
#define SDMMC_NO_RSP_SC            (1<<8)
#define SDMMC_NO_RSP_SC_MASK       (1<<8)
#define SDMMC_NO_RSP_SC_SHIFT      (8)
#define SDMMC_RSP_ERR_SC           (1<<9)
#define SDMMC_RSP_ERR_SC_MASK      (1<<9)
#define SDMMC_RSP_ERR_SC_SHIFT     (9)
#define SDMMC_RD_ERR_SC            (1<<10)
#define SDMMC_RD_ERR_SC_MASK       (1<<10)
#define SDMMC_RD_ERR_SC_SHIFT      (10)
#define SDMMC_WR_ERR_SC            (1<<11)
#define SDMMC_WR_ERR_SC_MASK       (1<<11)
#define SDMMC_WR_ERR_SC_SHIFT      (11)
#define SDMMC_DAT_OVER_SC          (1<<12)
#define SDMMC_DAT_OVER_SC_MASK     (1<<12)
#define SDMMC_DAT_OVER_SC_SHIFT    (12)
#define SDMMC_TXDMA_DONE_SC        (1<<13)
#define SDMMC_TXDMA_DONE_SC_MASK   (1<<13)
#define SDMMC_TXDMA_DONE_SC_SHIFT  (13)
#define SDMMC_RXDMA_DONE_SC        (1<<14)
#define SDMMC_RXDMA_DONE_SC_MASK   (1<<14)
#define SDMMC_RXDMA_DONE_SC_SHIFT  (14)

//SDMMC_INT_MASK
#define SDMMC_NO_RSP_MK            (1<<0)
#define SDMMC_NO_RSP_MK_MASK       (1<<0)
#define SDMMC_NO_RSP_MK_SHIFT      (0)
#define SDMMC_RSP_ERR_MK           (1<<1)
#define SDMMC_RSP_ERR_MK_MASK      (1<<1)
#define SDMMC_RSP_ERR_MK_SHIFT     (1)
#define SDMMC_RD_ERR_MK            (1<<2)
#define SDMMC_RD_ERR_MK_MASK       (1<<2)
#define SDMMC_RD_ERR_MK_SHIFT      (2)
#define SDMMC_WR_ERR_MK            (1<<3)
#define SDMMC_WR_ERR_MK_MASK       (1<<3)
#define SDMMC_WR_ERR_MK_SHIFT      (3)
#define SDMMC_DAT_OVER_MK          (1<<4)
#define SDMMC_DAT_OVER_MK_MASK     (1<<4)
#define SDMMC_DAT_OVER_MK_SHIFT    (4)
#define SDMMC_TXDMA_DONE_MK        (1<<5)
#define SDMMC_TXDMA_DONE_MK_MASK   (1<<5)
#define SDMMC_TXDMA_DONE_MK_SHIFT  (5)
#define SDMMC_RXDMA_DONE_MK        (1<<6)
#define SDMMC_RXDMA_DONE_MK_MASK   (1<<6)
#define SDMMC_RXDMA_DONE_MK_SHIFT  (6)

//SDMMC_INT_CLEAR
#define SDMMC_NO_RSP_CL            (1<<0)
#define SDMMC_NO_RSP_CL_MASK       (1<<0)
#define SDMMC_NO_RSP_CL_SHIFT      (0)
#define SDMMC_RSP_ERR_CL           (1<<1)
#define SDMMC_RSP_ERR_CL_MASK      (1<<1)
#define SDMMC_RSP_ERR_CL_SHIFT     (1)
#define SDMMC_RD_ERR_CL            (1<<2)
#define SDMMC_RD_ERR_CL_MASK       (1<<2)
#define SDMMC_RD_ERR_CL_SHIFT      (2)
#define SDMMC_WR_ERR_CL            (1<<3)
#define SDMMC_WR_ERR_CL_MASK       (1<<3)
#define SDMMC_WR_ERR_CL_SHIFT      (3)
#define SDMMC_DAT_OVER_CL          (1<<4)
#define SDMMC_DAT_OVER_CL_MASK     (1<<4)
#define SDMMC_DAT_OVER_CL_SHIFT    (4)
#define SDMMC_TXDMA_DONE_CL        (1<<5)
#define SDMMC_TXDMA_DONE_CL_MASK   (1<<5)
#define SDMMC_TXDMA_DONE_CL_SHIFT  (5)
#define SDMMC_RXDMA_DONE_CL        (1<<6)
#define SDMMC_RXDMA_DONE_CL_MASK   (1<<6)
#define SDMMC_RXDMA_DONE_CL_SHIFT  (6)

//SDMMC_TRANS_SPEED_REG
#define SDMMC_SDMMC_TRANS_SPEED(n) (((n)&0xFF)<<0)
#define SDMMC_SDMMC_TRANS_SPEED_MASK (0xFF<<0)
#define SDMMC_SDMMC_TRANS_SPEED_SHIFT (0)

//SDMMC_MCLK_ADJUST_REG
#define SDMMC_SDMMC_MCLK_ADJUST(n) (((n)&15)<<0)
#define SDMMC_SDMMC_MCLK_ADJUST_MASK (15<<0)
#define SDMMC_SDMMC_MCLK_ADJUST_SHIFT (0)
#define SDMMC_CLK_INV              (1<<4)
#define SDMMC_CLK_INV_MASK         (1<<4)
#define SDMMC_CLK_INV_SHIFT        (4)



#define SDMMC_AUTO_CMD12_EN  (1<<16)

#endif


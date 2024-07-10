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


#ifndef _SCI_H_
#define _SCI_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'sci'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SCI_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_SCI1_BASE              0x08814000
#define REG_SCI2_BASE              0x08815000

typedef volatile struct
{
    REG32                          Sci_config;                   //0x00000000
    REG32                          Status;                       //0x00000004
    REG32                          Data;                         //0x00000008
    REG32                          ClkDiv_REG;                   //0x0000000C
    REG32                          RxCnt_REG;                    //0x00000010
    REG32                          Times;                        //0x00000014
    REG32                          Ch_Filt_REG;                  //0x00000018
    /// clear RX/TX FIFO
    REG32                          dbg;                          //0x0000001C
    /// This register is a <b>READ ONLY</b> register that returns the logical
    /// <b>and</b> of the SCI_INT_STATUS register and the SCI_INT_MASK. If any
    /// of these bits is '1', the SCI module will generate an interrupt. Bits
    /// 21:16 return the <u>status</u> of the interrupt which is the interrupt
    /// state before the mask is applied. These bits should only be used for
    /// debugging.
    REG32                          Int_Cause;                    //0x00000020
    /// This is a WRITE ONLY register that is used to clear an SCI interrupt.
    /// Write a '1' to the interrupt that is to be cleared. Writing '0' has no
    /// effect.
    REG32                          Int_Clr;                      //0x00000024
    /// This register is READ/WRITE register that enables the desired interrupt.
    /// A '1' in a bit position indicates that the corresponding interrupt is
    /// enabled and if the interrupt occurs, the SCI will generate a hardware
    /// interrupt.
    REG32                          Int_Mask;                     //0x00000028
    REG32                          PA_Clk_Stop_En;               //0x0000002C
    REG32                          PA_Status;                    //0x00000030
} HWP_SCI_T;

#define hwp_sci1                   ((HWP_SCI_T*) REG_ACCESS_ADDRESS(REG_SCI1_BASE))
#define hwp_sci2                   ((HWP_SCI_T*) REG_ACCESS_ADDRESS(REG_SCI2_BASE))


//Sci_config
typedef union {
    REG32 v;
    struct {
        REG32 enable : 1; // [0]
        REG32 parity : 1; // [1]
        REG32 perf : 1; // [2]
        REG32 filter_disable : 1; // [3]
        REG32 clockstop : 1; // [4]
        REG32 autostop_en_h : 1; // [5]
        REG32 msbh_lsbl : 1; // [6]
        REG32 lli : 1; // [7]
        REG32 pegen_len : 1; // [8]
        REG32 parity_en : 1; // [9]
        REG32 stop_level : 1; // [10]
        REG32 __15_11 : 5;
        REG32 arg_h : 1; // [16]
        REG32 afd_en_h : 1; // [17]
        REG32 tx_resend_en_h : 1; // [18]
        REG32 __19_19 : 1;
        REG32 reset : 1; // [20]
        REG32 dly_sel : 1; // [21]
        REG32 in_avg_en : 1; // [22]
        REG32 __23_23 : 1;
        REG32 par_chk_offset : 6; // [29:24]
        REG32 __31_30 : 2;
    } b;
} REG_SCI_SCI_CONFIG_T;

//Status
typedef union {
    REG32 v;
    struct {
        REG32 rxdata_rdy : 1; // [0], read only
        REG32 tx_fifo_rdy : 1; // [1], read only
        REG32 format_det : 1; // [2], read only
        REG32 arg_det : 1; // [3], read only
        REG32 reset_det : 1; // [4], read only
        REG32 clk_rdy_h : 1; // [5], read only
        REG32 clk_off : 1; // [6], read only
        REG32 __7_7 : 1;
        REG32 rx_err : 1; // [8], read only
        REG32 tx_err : 1; // [9], read only
        REG32 rxoverflow : 1; // [10], read only
        REG32 txoverflow : 1; // [11], read only
        REG32 __29_12 : 18;
        REG32 autostop_state : 2; // [31:30], read only
    } b;
} REG_SCI_STATUS_T;

//Data
typedef union {
    REG32 v;
    struct {
        REG32 data_in : 8; // [7:0]
        REG32 data_out : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_SCI_DATA_T;

//ClkDiv_REG
typedef union {
    REG32 v;
    struct {
        REG32 clkdiv : 9; // [8:0]
        REG32 baud_x8_en : 1; // [9]
        REG32 rx_clk_cnt_limit : 5; // [14:10]
        REG32 clk_tst : 1; // [15]
        REG32 clkdiv_16 : 8; // [23:16]
        REG32 maindiv : 6; // [29:24]
        REG32 clk_out_inv : 1; // [30]
        REG32 clk_inv : 1; // [31]
    } b;
} REG_SCI_CLKDIV_REG_T;

//RxCnt_REG
typedef union {
    REG32 v;
    struct {
        REG32 rxcnt : 10; // [9:0]
        REG32 __30_10 : 21;
        REG32 clk_persist : 1; // [31]
    } b;
} REG_SCI_RXCNT_REG_T;

//Times
typedef union {
    REG32 v;
    struct {
        REG32 chguard : 8; // [7:0]
        REG32 turnaroundguard : 4; // [11:8]
        REG32 __15_12 : 4;
        REG32 wi : 8; // [23:16]
        REG32 tx_pert : 8; // [31:24]
    } b;
} REG_SCI_TIMES_T;

//Ch_Filt_REG
typedef union {
    REG32 v;
    struct {
        REG32 ch_filt : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_SCI_CH_FILT_REG_T;

//dbg
typedef union {
    REG32 v;
    struct {
        REG32 fifo_rx_clr : 1; // [0]
        REG32 fifo_tx_clr : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_SCI_DBG_T;

//Int_Cause
typedef union {
    REG32 v;
    struct {
        REG32 rx_done : 1; // [0], read only
        REG32 rx_half : 1; // [1], read only
        REG32 wwt_timeout : 1; // [2], read only
        REG32 extra_rx : 1; // [3], read only
        REG32 resend_ovfl : 1; // [4], read only
        REG32 arg_end : 1; // [5], read only
        REG32 sci_dma_tx_done : 1; // [6], read only
        REG32 sci_dma_rx_done : 1; // [7], read only
        REG32 __31_8 : 24;
    } b;
} REG_SCI_INT_CAUSE_T;

//Int_Clr
typedef union {
    REG32 v;
    struct {
        REG32 rx_done : 1; // [0], write clear
        REG32 rx_half : 1; // [1], write clear
        REG32 wwt_timeout : 1; // [2], write clear
        REG32 extra_rx : 1; // [3], write clear
        REG32 resend_ovfl : 1; // [4], write clear
        REG32 arg_end : 1; // [5], write clear
        REG32 sci_dma_tx_done : 1; // [6], write clear
        REG32 sci_dma_rx_done : 1; // [7], write clear
        REG32 __31_8 : 24;
    } b;
} REG_SCI_INT_CLR_T;

//Int_Mask
typedef union {
    REG32 v;
    struct {
        REG32 rx_done : 1; // [0]
        REG32 rx_half : 1; // [1]
        REG32 wwt_timeout : 1; // [2]
        REG32 extra_rx : 1; // [3]
        REG32 resend_ovfl : 1; // [4]
        REG32 arg_end : 1; // [5]
        REG32 sci_dma_tx_done : 1; // [6]
        REG32 sci_dma_rx_done : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_SCI_INT_MASK_T;


//Sci_config
#define SCI_ENABLE                 (1<<0)
#define SCI_PARITY_MASK            (1<<1)
#define SCI_PARITY_SHIFT           (1)
#define SCI_PARITY_EVEN_PARITY     (0<<1)
#define SCI_PARITY_ODD_PARITY      (1<<1)
#define SCI_PARITY_V_EVEN_PARITY   (0)
#define SCI_PARITY_V_ODD_PARITY    (1)
#define SCI_PERF                   (1<<2)
#define SCI_FILTER_DISABLE         (1<<3)
#define SCI_CLOCKSTOP              (1<<4)
#define SCI_AUTOSTOP_EN_H          (1<<5)
#define SCI_MSBH_LSBL              (1<<6)
#define SCI_LLI                    (1<<7)
#define SCI_PEGEN_LEN              (1<<8)
#define SCI_PARITY_EN              (1<<9)
#define SCI_STOP_LEVEL             (1<<10)
#define SCI_ARG_H                  (1<<16)
#define SCI_AFD_EN_H               (1<<17)
#define SCI_TX_RESEND_EN_H         (1<<18)
#define SCI_RESET                  (1<<20)
#define SCI_DLY_SEL                (1<<21)
#define SCI_IN_AVG_EN              (1<<22)
#define SCI_PAR_CHK_OFFSET(n)      (((n)&0x3F)<<24)
//#define SCI_RESERVED(n)          (((n)&3)<<30)

//Status
#define SCI_RXDATA_RDY             (1<<0)
#define SCI_TX_FIFO_RDY            (1<<1)
#define SCI_FORMAT_DET             (1<<2)
#define SCI_ARG_DET                (1<<3)
#define SCI_RESET_DET              (1<<4)
#define SCI_CLK_RDY_H              (1<<5)
#define SCI_CLK_OFF                (1<<6)
#define SCI_RX_ERR                 (1<<8)
#define SCI_TX_ERR                 (1<<9)
#define SCI_RXOVERFLOW             (1<<10)
#define SCI_TXOVERFLOW             (1<<11)
#define SCI_AUTOSTOP_STATE_MASK    (3<<30)
#define SCI_AUTOSTOP_STATE_SHIFT   (30)
#define SCI_AUTOSTOP_STATE_STARTUP_PHASE (0<<30)
#define SCI_AUTOSTOP_STATE_AUTO_ON (1<<30)
#define SCI_AUTOSTOP_STATE_SHUTDOWN_PHASE (2<<30)
#define SCI_AUTOSTOP_STATE_CLOCK_OFF (3<<30)
#define SCI_AUTOSTOP_STATE_V_STARTUP_PHASE (0)
#define SCI_AUTOSTOP_STATE_V_AUTO_ON (1)
#define SCI_AUTOSTOP_STATE_V_SHUTDOWN_PHASE (2)
#define SCI_AUTOSTOP_STATE_V_CLOCK_OFF (3)

//Data
#define SCI_DATA_IN(n)             (((n)&0xFF)<<0)
#define SCI_DATA_OUT(n)            (((n)&0xFF)<<0)

//ClkDiv_REG
#define SCI_CLKDIV(n)              (((n)&0x1FF)<<0)
#define SCI_BAUD_X8_EN             (1<<9)
#define SCI_RX_CLK_CNT_LIMIT(n)    (((n)&31)<<10)
#define SCI_CLK_TST                (1<<15)
#define SCI_CLKDIV_16(n)           (((n)&0xFF)<<16)
#define SCI_MAINDIV(n)             (((n)&0x3F)<<24)
#define SCI_MAINDIV_MASK           (0x3F<<24)
#define SCI_CLK_OUT_INV            (1<<30)
#define SCI_CLK_INV                (1<<31)

//RxCnt_REG
#define SCI_RXCNT(n)               (((n)&0x3FF)<<0)
#define SCI_CLK_PERSIST            (1<<31)

//Times
#define SCI_CHGUARD(n)             (((n)&0xFF)<<0)
#define SCI_CHGUARD_MASK           (0xFF<<0)
#define SCI_CHGUARD_SHIFT          (0)
#define SCI_TURNAROUNDGUARD(n)     (((n)&15)<<8)
#define SCI_TURNAROUNDGUARD_MASK   (15<<8)
#define SCI_TURNAROUNDGUARD_SHIFT  (8)
#define SCI_WI(n)                  (((n)&0xFF)<<16)
#define SCI_WI_MASK                (0xFF<<16)
#define SCI_WI_SHIFT               (16)
#define SCI_TX_PERT(n)             (((n)&0xFF)<<24)

//Ch_Filt_REG
#define SCI_CH_FILT(n)             (((n)&0xFF)<<0)

//dbg
#define SCI_FIFO_RX_CLR            (1<<0)
#define SCI_FIFO_TX_CLR            (1<<1)

//Int_Cause
#define SCI_RX_DONE                (1<<0)
#define SCI_RX_HALF                (1<<1)
#define SCI_WWT_TIMEOUT            (1<<2)
#define SCI_EXTRA_RX               (1<<3)
#define SCI_RESEND_OVFL            (1<<4)
#define SCI_ARG_END                (1<<5)
#define SCI_SCI_DMA_TX_DONE        (1<<6)
#define SCI_SCI_DMA_RX_DONE        (1<<7)

//Int_Clr
//#define SCI_RX_DONE              (1<<0)
//#define SCI_RX_HALF              (1<<1)
//#define SCI_WWT_TIMEOUT          (1<<2)
//#define SCI_EXTRA_RX             (1<<3)
//#define SCI_RESEND_OVFL          (1<<4)
//#define SCI_ARG_END              (1<<5)
//#define SCI_SCI_DMA_TX_DONE      (1<<6)
//#define SCI_SCI_DMA_RX_DONE      (1<<7)

//Int_Mask
//#define SCI_RX_DONE              (1<<0)
//#define SCI_RX_HALF              (1<<1)
//#define SCI_WWT_TIMEOUT          (1<<2)
//#define SCI_EXTRA_RX             (1<<3)
//#define SCI_RESEND_OVFL          (1<<4)
//#define SCI_ARG_END              (1<<5)
//#define SCI_SCI_DMA_TX_DONE      (1<<6)
//#define SCI_SCI_DMA_RX_DONE      (1<<7)





#endif


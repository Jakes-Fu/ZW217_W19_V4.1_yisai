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


#ifndef _ARM_UART_H_
#define _ARM_UART_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'arm_uart'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// ARM_UART_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_UART2_BASE             0x08817000
#define REG_UART3_BASE             0x08818000
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_UART1_BASE             0x05098000
#else
#define REG_UART1_BASE             0x50098000
#endif
#define REG_UART4_BASE             0x08816000

typedef volatile struct
{
    REG32                          uart_tx;                      //0x00000000
    REG32                          uart_rx;                      //0x00000004
    REG32                          uart_baud;                    //0x00000008
    REG32                          uart_conf;                    //0x0000000C
    REG32                          uart_rxtrig;                  //0x00000010
    REG32                          uart_txtrig;                  //0x00000014
    REG32                          uart_delay;                   //0x00000018
    REG32                          uart_status;                  //0x0000001C
    REG32                          uart_rxfifo_stat;            //0x00000020
    REG32                          uart_txfifo_stat;            //0x00000024
    REG32                          uart_rxfifo_hdlc;             //0x00000028
    REG32                          uart_at_status;               //0x0000002C
} HWP_ARM_UART_T;

#define hwp_uart2                  ((HWP_ARM_UART_T*) REG_ACCESS_ADDRESS(REG_UART2_BASE))
#define hwp_uart3                  ((HWP_ARM_UART_T*) REG_ACCESS_ADDRESS(REG_UART3_BASE))
#define hwp_uart1                  ((HWP_ARM_UART_T*) REG_ACCESS_ADDRESS(REG_UART1_BASE))


//UART_TX
typedef union {
    REG32 v;
    struct {
        REG32 tx_data : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_ARM_UART_UART_TX_T;

//UART_RX
typedef union {
    REG32 v;
    struct {
        REG32 rx_data : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_ARM_UART_UART_RX_T;

//UART_BAUD
typedef union {
    REG32 v;
    struct {
        REG32 baud_div : 16; // [15:0]
        REG32 baud_const : 4; // [19:16]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_UART_UART_BAUD_T;

//UART_CONF
typedef union {
    REG32 v;
    struct {
        REG32 check : 1; // [0]
        REG32 parity : 1; // [1]
        REG32 stop_bit : 1; // [2]
        REG32 st_check : 1; // [3]
        REG32 rx_ie : 1; // [4]
        REG32 tx_ie : 1; // [5]
        REG32 tout_ie : 1; // [6]
        REG32 hwfc : 1; // [7]
        REG32 rx_trig_hwfc : 1; // [8]
        REG32 tout_hwfc : 1; // [9]
        REG32 hdlc : 1; // [10]
        REG32 frm_stp : 1; // [11]
        REG32 trail : 1; // [12]
        REG32 txrst : 1; // [13]
        REG32 rxrst : 1; // [14]
        REG32 at_parity_en : 1; // [15]
        REG32 at_parity_sel : 2; // [17:16]
        REG32 at_sync_char_sel : 1; // [18]
        REG32 at_verify_2byte : 1; // [19]
        REG32 at_div_mode : 1; // [20]
        REG32 at_enable : 1; // [21]
        REG32 __31_22 : 10;
    } b;
} REG_ARM_UART_UART_CONF_T;

//UART_RXTRIG
typedef union {
    REG32 v;
    struct {
        REG32 rx_trig : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_ARM_UART_UART_RXTRIG_T;

//UART_TXTRIG
typedef union {
    REG32 v;
    struct {
        REG32 tx_trig : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_ARM_UART_UART_TXTRIG_T;

//UART_DELAY
typedef union {
    REG32 v;
    struct {
        REG32 toutcnt : 8; // [7:0]
        REG32 two_tx_delay : 4; // [11:8]
        REG32 __31_12 : 20;
    } b;
} REG_ARM_UART_UART_DELAY_T;

//UART_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 tx_int : 1; // [0], write clear
        REG32 rx_int : 1; // [1], write clear
        REG32 timeout_int : 1; // [2], write clear
        REG32 frame_int : 1; // [3], write clear
        REG32 p_error : 1; // [4], write clear
        REG32 st_error : 1; // [5], write clear
        REG32 cts : 1; // [6], write clear
        REG32 rts : 1; // [7], write clear
        REG32 __31_8 : 24;
    } b;
} REG_ARM_UART_UART_STATUS_T;

//UART_RXFIFO_STAT
typedef union {
    REG32 v;
    struct {
        REG32 rx_fifo_cnt : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_ARM_UART_UART_RXFIFO_STAT_T;

//UART_TXFIFO_STAT
typedef union {
    REG32 v;
    struct {
        REG32 tx_fifo_cnt : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_ARM_UART_UART_TXFIFO_STAT_T;

//UART_RXFIFO_HDLC
typedef union {
    REG32 v;
    struct {
        REG32 hdlc_rxfifo_cnt : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_ARM_UART_UART_RXFIFO_HDLC_T;

//UART_AT_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 at_baud_div : 16; // [15:0], read only
        REG32 character_miscompare : 1; // [16], read only
        REG32 auto_baud_locked : 1; // [17], read only
        REG32 __31_18 : 14;
    } b;
} REG_ARM_UART_UART_AT_STATUS_T;


//UART_TX
#define ARM_UART_RESERVED_0(n)     (((n)&0xFFFFFF)<<8)
#define ARM_UART_TX_DATA(n)        (((n)&0xFF)<<0)

//UART_RX
//#define ARM_UART_RESERVED_0(n)   (((n)&0xFFFFFF)<<8)
#define ARM_UART_RX_DATA(n)        (((n)&0xFF)<<0)

//UART_BAUD
#define ARM_UART_RESERVED_0_X1(n)  (((n)&0xFFF)<<20)
#define ARM_UART_BAUD_CONST(n)     (((n)&15)<<16)
#define ARM_UART_BAUD_DIV(n)       (((n)&0xFFFF)<<0)

//UART_CONF
#define ARM_UART_RESERVED_0_X2(n)  (((n)&0x3FF)<<22)
#define ARM_UART_AT_ENABLE         (1<<21)
#define ARM_UART_AT_DIV_MODE       (1<<20)
#define ARM_UART_AT_VERIFY_2BYTE   (1<<19)
#define ARM_UART_AT_SYNC_CHAR_SEL  (1<<18)
#define ARM_UART_AT_PARITY_SEL(n)  (((n)&3)<<16)
#define ARM_UART_AT_PARITY_EN      (1<<15)
#define ARM_UART_RXRST             (1<<14)
#define ARM_UART_TXRST             (1<<13)
#define ARM_UART_TRAIL             (1<<12)
#define ARM_UART_FRM_STP           (1<<11)
#define ARM_UART_HDLC              (1<<10)
#define ARM_UART_TOUT_HWFC         (1<<9)
#define ARM_UART_RX_TRIG_HWFC      (1<<8)
#define ARM_UART_HWFC              (1<<7)
#define ARM_UART_TOUT_IE           (1<<6)
#define ARM_UART_TX_IE             (1<<5)
#define ARM_UART_RX_IE             (1<<4)
#define ARM_UART_ST_CHECK          (1<<3)
#define ARM_UART_STOP_BIT          (1<<2)
#define ARM_UART_PARITY            (1<<1)
#define ARM_UART_CHECK             (1<<0)

//UART_RXTRIG
//#define ARM_UART_RESERVED_0(n)   (((n)&0xFFFFFF)<<8)
#define ARM_UART_RX_TRIG(n)        (((n)&0xFF)<<0)

//UART_TXTRIG
//#define ARM_UART_RESERVED_0(n)   (((n)&0xFFFFFF)<<8)
#define ARM_UART_TX_TRIG(n)        (((n)&0xFF)<<0)

//UART_DELAY
#define ARM_UART_RESERVED_0_X3(n)  (((n)&0xFFFFF)<<12)
#define ARM_UART_TWO_TX_DELAY(n)   (((n)&15)<<8)
#define ARM_UART_TOUTCNT(n)        (((n)&0xFF)<<0)

//UART_STATUS
//#define ARM_UART_RESERVED_0(n)   (((n)&0xFFFFFF)<<8)
#define ARM_UART_RTS               (1<<7)
#define ARM_UART_CTS               (1<<6)
#define ARM_UART_ST_ERROR          (1<<5)
#define ARM_UART_P_ERROR           (1<<4)
#define ARM_UART_FRAME_INT         (1<<3)
#define ARM_UART_TIMEOUT_INT       (1<<2)
#define ARM_UART_RX_INT            (1<<1)
#define ARM_UART_TX_INT            (1<<0)

//UART_RXFIFO_STAT
//#define ARM_UART_RESERVED_0(n)   (((n)&0xFFFFFF)<<8)
#define ARM_UART_RX_FIFO_CNT(n)    (((n)&0xFF)<<0)

//UART_TXFIFO_STAT
//#define ARM_UART_RESERVED_0(n)   (((n)&0xFFFFFF)<<8)
#define ARM_UART_TX_FIFO_CNT(n)    (((n)&0xFF)<<0)

//UART_RXFIFO_HDLC
//#define ARM_UART_RESERVED_0(n)   (((n)&0xFFFFFF)<<8)
#define ARM_UART_HDLC_RXFIFO_CNT(n) (((n)&0xFF)<<0)

//UART_AT_STATUS
#define ARM_UART_RESERVED_0_X4(n)  (((n)&0x3FFF)<<18)
#define ARM_UART_AUTO_BAUD_LOCKED  (1<<17)
#define ARM_UART_CHARACTER_MISCOMPARE (1<<16)
#define ARM_UART_AT_BAUD_DIV(n)    (((n)&0xFFFF)<<0)



//baudrate register has 21 bits: (2exp21 - 2) / 2
#define BAUDRATE_MAX       2097151
#define ARM_UART_TXD       0x0000  //Write data to this address initiates a character transmission through tx fifo.


#endif


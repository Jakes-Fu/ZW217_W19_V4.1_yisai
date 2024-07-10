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
#define REG_UART1_BASE             0x08816000
#define REG_UART2_BASE             0x08817000
#define REG_UART3_BASE             0x08818000

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
} HWP_ARM_UART_T;

#define hwp_uart1                  ((HWP_ARM_UART_T*) REG_ACCESS_ADDRESS(REG_UART1_BASE))
#define hwp_uart2                  ((HWP_ARM_UART_T*) REG_ACCESS_ADDRESS(REG_UART2_BASE))
#define hwp_uart3                  ((HWP_ARM_UART_T*) REG_ACCESS_ADDRESS(REG_UART3_BASE))


//uart_tx
#define ARM_UART_TX_DATA(n)        (((n)&0xFF)<<0)

//uart_rx
#define ARM_UART_RX_DATA(n)        (((n)&0xFF)<<0)

//uart_baud
#define ARM_UART_BAUD(n)           (((n)&0x3FFF)<<0)

//uart_conf
#define ARM_UART_CHECK_CHECK_DISABLE (0<<0)
#define ARM_UART_CHECK_CHECK_ENABLE (1<<0)
#define ARM_UART_PARITY_EVEN_PARITY (0<<1)
#define ARM_UART_PARITY_ODD_PARITY (1<<1)
#define ARM_UART_STOP_BIT_1BIT_STOP (0<<2)
#define ARM_UART_STOP_BIT_2BIT_STOP (1<<2)
#define ARM_UART_ST_CHECK_ST_CHECK_DISABLE (0<<3)
#define ARM_UART_ST_CHECK_ST_CHECK_ENABLE (1<<3)
#define ARM_UART_RX_IE_RX_IE_DISABLE (0<<4)
#define ARM_UART_RX_IE_RX_IE_ENABLE (1<<4)
#define ARM_UART_TX_IE_TX_IE_DISABLE (0<<5)
#define ARM_UART_TX_IE_TX_IE_ENABLE (1<<5)
#define ARM_UART_TOUT_IE_TOUT_IE_DISABLE (0<<6)
#define ARM_UART_TOUT_IE_TOUT_IE_ENABLE (1<<6)
#define ARM_UART_HWFC_HWFC_DISABLE (0<<7)
#define ARM_UART_HWFC_HWFC_ENABLE  (1<<7)
#define ARM_UART_RX_TRIG_HWFC_RX_TRIG_HWFC_DISABLE (0<<8)
#define ARM_UART_RX_TRIG_HWFC_RX_TRIG_HWFC_ENABLE (1<<8)
#define ARM_UART_TOUT_HWFC_TOUT_HWFC_DISABLE (0<<9)
#define ARM_UART_TOUT_HWFC_TOUT_HWFC_ENABLE (1<<9)
#define ARM_UART_HDLC_HDLC_DISABLE (0<<10)
#define ARM_UART_HDLC_HDLC_ENABLE  (1<<10)
#define ARM_UART_FRM_STP_FRM_STP_DISABLE (0<<11)
#define ARM_UART_FRM_STP_FRM_STP_ENABLE (1<<11)
#define ARM_UART_TRAIL_ARM_TRAIL   (0<<12)
#define ARM_UART_TRAIL_DMA_TRAIL   (1<<12)
#define ARM_UART_TXRST_TX_FIFO_NO_RESET (0<<13)
#define ARM_UART_TXRST_TX_FIFO_RESET (1<<13)
#define ARM_UART_RXRST_RX_FIFO_NO_RESET (0<<14)
#define ARM_UART_RXRST_RX_FIFO_RESET (1<<14)

//uart_rxtrig
#define ARM_UART_RX_TRIG(n)        (((n)&0xFF)<<0)

//uart_txtrig
#define ARM_UART_TX_TRIG(n)        (((n)&0xFF)<<0)

//uart_delay
#define ARM_UART_TOUTCNT(n)        (((n)&0xFF)<<0)
#define ARM_UART_TWO_TX_DELAY(n)   (((n)&15)<<8)

//uart_status
#define ARM_UART_TX_INT            (1<<0)
#define ARM_UART_RX_INT            (1<<1)
#define ARM_UART_TIMEOUT_INT       (1<<2)
#define ARM_UART_FRAME_INT         (1<<3)
#define ARM_UART_P_ERROR           (1<<4)
#define ARM_UART_ST_ERROR          (1<<5)
#define ARM_UART_CTS               (1<<6)
#define ARM_UART_RTS               (1<<7)

//uart_rxfifo_stat
#define ARM_UART_RX_FIFO_CNT(n)    (((n)&0xFF)<<0)

//uart_txfifo_stat
#define ARM_UART_TX_FIFO_CNT(n)    (((n)&0xFF)<<0)

//uart_rxfifo_hdlc
#define ARM_UART_HDLC_RXFIFO_CNT(n) (((n)&0xFF)<<0)




//baudrate register has 21 bits: (2exp21 - 2) / 2
#define BAUDRATE_MAX       2097151
#define ARM_UART_TXD       0x0000  //Write data to this address initiates a character transmission through tx fifo.


#endif


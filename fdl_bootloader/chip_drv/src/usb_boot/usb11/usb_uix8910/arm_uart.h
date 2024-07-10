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

#include "global_macros.h"

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
    REG32                          UART_TX;                      //0x00000000
    REG32                          UART_RX;                      //0x00000004
    REG32                          UART_BAUD;                    //0x00000008
    REG32                          UART_CONF;                    //0x0000000C
    REG32                          UART_RXTRIG;                  //0x00000010
    REG32                          UART_TXTRIG;                  //0x00000014
    REG32                          UART_DELAY;                   //0x00000018
    REG32                          UART_STATUS;                  //0x0000001C
    REG32                          UART_RXFIFO_STAT;             //0x00000020
    REG32                          UART_TXFIFO_STAT;             //0x00000024
    REG32                          UART_RXFIFO_HDLC;             //0x00000028
    REG32                          UART_AT_STATUS;               //0x0000002C
} HWP_ARM_UART_T;

#define hwp_uart1                  ((HWP_ARM_UART_T*) REG_ACCESS_ADDRESS(REG_UART1_BASE))
#define hwp_uart2                  ((HWP_ARM_UART_T*) REG_ACCESS_ADDRESS(REG_UART2_BASE))
#define hwp_uart3                  ((HWP_ARM_UART_T*) REG_ACCESS_ADDRESS(REG_UART3_BASE))


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





#endif


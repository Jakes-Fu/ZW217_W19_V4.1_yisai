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

#include "uix8910_module_ver_extern.h"

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
} HWP_CP_CLKRST_T;

#define hwp_clkrst                 ((HWP_CP_CLKRST_T*) REG_ACCESS_ADDRESS(REG_CLKRST_BASE))


//CLKSEL
#define CP_CLKRST_RESERVED_0(n)    (((n)&0x7FFFFFF)<<5)
#define CP_CLKRST_IDLE_H_SEL       (1<<4)
#define CP_CLKRST_ZSP_WD_SEL       (1<<3)
#define CP_CLKRST_BB_SYSCTRL_WD_SEL (1<<2)
#define CP_CLKRST_SLOW_SEL_480M    (1<<1)
#define CP_CLKRST_SLOW_SEL_122M    (1<<0)

//CLKSEL_SET
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x7FFFFFF)<<5)
#define CP_CLKRST_CLKSELSET(n)     (((n)&31)<<0)

//CLKSEL_CLR
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x7FFFFFF)<<5)
#define CP_CLKRST_CLKSELCLEAR(n)   (((n)&31)<<0)

//CLKDIV_ZSP
#define CP_CLKRST_RESERVED_0_X1(n) (((n)&0xFFFFFFF)<<4)
#define CP_CLKRST_ZSP_CD(n)        (((n)&15)<<0)

//CLKDIV_LTE
//#define CP_CLKRST_RESERVED_0_X1(n) (((n)&0xFFFFFFF)<<4)
#define CP_CLKRST_LTE_CD(n)        (((n)&15)<<0)

//CLKEN_BB_SYSCTRL
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x7FFFFFF)<<5)
#define CP_CLKRST_CLKEN_CP_WD      (1<<4)
#define CP_CLKRST_CLKEN_IDLE       (1<<3)
#define CP_CLKRST_CLKEN_MAILBOX    (1<<2)
#define CP_CLKRST_CLKEN_PWRCTRL    (1<<1)
#define CP_CLKRST_CLKEN_SYSREG     (1<<0)

//CLKEN_BB_SYSCTRL_SET
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x7FFFFFF)<<5)
#define CP_CLKRST_CLKEN_BB_SYSCTRLSET(n) (((n)&31)<<0)

//CLKEN_BB_SYSCTRL_CLR
//#define CP_CLKRST_RESERVED_0(n)  (((n)&0x7FFFFFF)<<5)
#define CP_CLKRST_CLKEN_BB_SYSCTRLCLEAR(n) (((n)&31)<<0)

//CLKEN_ZSP
#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_CLKEN_UART       (1<<5)
#define CP_CLKRST_CLKEN_WD         (1<<4)
#define CP_CLKRST_CLKEN_BUSMON     (1<<3)
#define CP_CLKRST_CLKEN_ZIRQ       (1<<2)
#define CP_CLKRST_CLKEN_AXIDMA     (1<<1)
#define CP_CLKRST_CLKEN_ZSPCORE    (1<<0)

//CLKEN_ZSP_SET
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_CLKEN_ZSPSET(n)  (((n)&0x3F)<<0)

//CLKEN_ZSP_CLR
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_CLKEN_ZSPCLEAR(n) (((n)&0x3F)<<0)

//CLKEN_LTE
#define CP_CLKRST_RESERVED_0_X3(n) (((n)&0xFFFFF)<<12)
#define CP_CLKRST_CLKEN_RFAD       (1<<11)
#define CP_CLKRST_CLKEN_DLFFT      (1<<10)
#define CP_CLKRST_CLKEN_CSIRS      (1<<9)
#define CP_CLKRST_CLKEN_PUSCH      (1<<8)
#define CP_CLKRST_CLKEN_ULDFT      (1<<7)
#define CP_CLKRST_CLKEN_NSSCORR    (1<<6)
#define CP_CLKRST_CLKEN_IDDET      (1<<5)
#define CP_CLKRST_CLKEN_MEASPWR    (1<<4)
#define CP_CLKRST_CLKEN_LDTC1      (1<<3)
#define CP_CLKRST_CLKEN_LDTC       (1<<2)
#define CP_CLKRST_CLKEN_COEFF      (1<<1)
#define CP_CLKRST_CLKEN_TXRX       (1<<0)

//CLKEN_LTE_SET
//#define CP_CLKRST_RESERVED_0_X3(n) (((n)&0xFFFFF)<<12)
#define CP_CLKRST_CLKEN_LTESET(n)  (((n)&0xFFF)<<0)

//CLKEN_LTE_CLR
//#define CP_CLKRST_RESERVED_0_X3(n) (((n)&0xFFFFF)<<12)
#define CP_CLKRST_CLKEN_LTECLEAR(n) (((n)&0xFFF)<<0)

//CLKEN_ZSPCORE_MODE
#define CP_CLKRST_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_CLKRST_MODE             (1<<0)

//CLKEN_ZSP_AXIDMA_MODE
//#define CP_CLKRST_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
//#define CP_CLKRST_MODE           (1<<0)

//RSTCTRL_BB_SYSCTRL
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_MONITOR  (1<<5)
#define CP_CLKRST_RSTCTRL_CP_WD    (1<<4)
#define CP_CLKRST_RSTCTRL_IDLE     (1<<3)
#define CP_CLKRST_RSTCTRL_MAILBOX  (1<<2)
#define CP_CLKRST_RSTCTRL_PWRCTRL  (1<<1)
#define CP_CLKRST_RSTCTRL_SYSREG   (1<<0)

//RSTCTRL_BB_SYSCTRL_SET
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_BB_SYSCTRLSET(n) (((n)&0x3F)<<0)

//RSTCTRL_BB_SYSCTRL_CLR
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_BB_SYSCTRLCLEAR(n) (((n)&0x3F)<<0)

//RSTCTRL_ZSP
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_UART     (1<<5)
#define CP_CLKRST_RSTCTRL_WD       (1<<4)
#define CP_CLKRST_RSTCTRL_BUSMON   (1<<3)
#define CP_CLKRST_RSTCTRL_ZIRQ     (1<<2)
#define CP_CLKRST_RSTCTRL_AXIDMA   (1<<1)
#define CP_CLKRST_RSTCTRL_ZSPCORE  (1<<0)

//RSTCTRL_ZSP_SET
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_ZSPSET(n) (((n)&0x3F)<<0)

//RSTCTRL_ZSP_CLR
//#define CP_CLKRST_RESERVED_0_X2(n) (((n)&0x3FFFFFF)<<6)
#define CP_CLKRST_RSTCTRL_ZSPCLEAR(n) (((n)&0x3F)<<0)

//RSTCTRL_LTE
#define CP_CLKRST_RESERVED_0_X5(n) (((n)&0x7FFFF)<<13)
#define CP_CLKRST_RSTCTRL_RFAD     (1<<12)
#define CP_CLKRST_RSTCTRL_DLFFT    (1<<11)
#define CP_CLKRST_RSTCTRL_CSIRS    (1<<10)
#define CP_CLKRST_RSTCTRL_PUSCH    (1<<9)
#define CP_CLKRST_RSTCTRL_ULDFT    (1<<8)
#define CP_CLKRST_RSTCTRL_NSSCORR  (1<<7)
#define CP_CLKRST_RSTCTRL_IDDET    (1<<6)
#define CP_CLKRST_RSTCTRL_MEASPWR  (1<<5)
#define CP_CLKRST_RSTCTRL_LDTC1    (1<<4)
#define CP_CLKRST_RSTCTRL_LDTC     (1<<3)
#define CP_CLKRST_RSTCTRL_COEFF    (1<<2)
#define CP_CLKRST_RSTCTRL_TXRX_RX  (1<<1)
#define CP_CLKRST_RSTCTRL_TXRX_TX  (1<<0)

//RSTCTRL_LTE_SET
//#define CP_CLKRST_RESERVED_0_X5(n) (((n)&0x7FFFF)<<13)
#define CP_CLKRST_RSTCTRL_LTESET(n) (((n)&0x1FFF)<<0)

//RSTCTRL_LTE_CLR
//#define CP_CLKRST_RESERVED_0_X5(n) (((n)&0x7FFFF)<<13)
#define CP_CLKRST_RSTCTRL_LTECLEAR(n) (((n)&0x1FFF)<<0)

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
#define CP_CLKRST_RESERVED_0_X7(n) (((n)&0x1FFFFFFF)<<3)
#define CP_CLKRST_BUSLPMC_PHY      (1<<2)
#define CP_CLKRST_BUSLPMC_SW2      (1<<1)
#define CP_CLKRST_BUSLPMC_SW1      (1<<0)

//ZSP_BUSLPMC_SET
//#define CP_CLKRST_RESERVED_0_X7(n) (((n)&0x1FFFFFFF)<<3)
#define CP_CLKRST_ZSP_BUSLPMCSET(n) (((n)&7)<<0)

//ZSP_BUSLPMC_CLR
//#define CP_CLKRST_RESERVED_0_X7(n) (((n)&0x1FFFFFFF)<<3)
#define CP_CLKRST_ZSP_BUSLPMCCLEAR(n) (((n)&7)<<0)

//ZSP_BUSFORCELPMC
//#define CP_CLKRST_RESERVED_0_X7(n) (((n)&0x1FFFFFFF)<<3)
#define CP_CLKRST_BUSFORCELPMC_PHY (1<<2)
#define CP_CLKRST_BUSFORCELPMC_SW2 (1<<1)
#define CP_CLKRST_BUSFORCELPMC_SW1 (1<<0)

//ZSP_BUSFORCELPMC_SET
//#define CP_CLKRST_RESERVED_0_X7(n) (((n)&0x1FFFFFFF)<<3)
#define CP_CLKRST_ZSP_BUSFORCELPMCSET(n) (((n)&7)<<0)

//ZSP_BUSFORCELPMC_CLR
//#define CP_CLKRST_RESERVED_0_X7(n) (((n)&0x1FFFFFFF)<<3)
#define CP_CLKRST_ZSP_BUSFORCELPMCCLEAR(n) (((n)&7)<<0)

//CLKEN_MAILBOX_MODE
//#define CP_CLKRST_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
//#define CP_CLKRST_MODE           (1<<0)





#endif


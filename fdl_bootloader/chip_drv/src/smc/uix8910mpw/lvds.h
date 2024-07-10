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

#include "global_macros.h"

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
    REG32                          SYNCIDX2;                     //0x00000028
    REG32                          SYNCIDX3;                     //0x0000002C
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
    REG32                          LATCH_WORD2_0;                //0x00000078
    REG32                          LATCH_WORD2_1;                //0x0000007C
    REG32                          LATCH_WORD3_0;                //0x00000080
    REG32                          LATCH_WORD3_1;                //0x00000084
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
    REG32 Reserved_000000B4[19];                //0x000000B4
    REG32                          LVDS_RESET;                   //0x00000100
    REG32                          CLK_BAND;                     //0x00000104
    REG32                          PHASE_SEL;                    //0x00000108
    REG32                          RX_DATA_PNSW;                 //0x0000010C
    REG32                          RX_DL8P_SEL_0;                //0x00000110
    REG32                          RX_DL8P_SEL_1;                //0x00000114
    REG32 Reserved_00000118[2];                 //0x00000118
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
} HWP_LVDS_T;

#define hwp_lvds                   ((HWP_LVDS_T*) REG_ACCESS_ADDRESS(REG_LVDS_BASE))


//DLHSSB_EN
#define LVDS_RESERVED_0(n)         (((n)&0xFFFFFFF)<<4)
#define LVDS_CONFIG_DLHSSB_EN(n)   (((n)&15)<<0)

//ULHSSB_EN
//#define LVDS_RESERVED_0(n)       (((n)&0xFFFFFFF)<<4)
#define LVDS_CONFIG_ULHSSB_EN(n)   (((n)&15)<<0)

//DLFIFO_CLR
#define LVDS_RESERVED_0_X1(n)      (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_DLFIFO_CLR     (1<<0)

//ULFIFO_CLR
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_ULFIFO_CLR     (1<<0)

//SYNCWORD10B_D
#define LVDS_RESERVED_0_X2(n)      (((n)&0xFFFFFF)<<8)
#define LVDS_CONFIG_SYNCWORD10B_D(n) (((n)&0xFF)<<0)

//SYNCWORD10B_S
//#define LVDS_RESERVED_0_X2(n)    (((n)&0xFFFFFF)<<8)
#define LVDS_CONFIG_SYNCWORD10B_S(n) (((n)&0xFF)<<0)

//REVERSE_MODE_EN
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_REVERSE_MODE_EN (1<<0)

//SYNCIDX0
//#define LVDS_RESERVED_0(n)       (((n)&0xFFFFFFF)<<4)
#define LVDS_CONFIG_SYNC_IDX0(n)   (((n)&15)<<0)

//SYNCIDX1
//#define LVDS_RESERVED_0(n)       (((n)&0xFFFFFFF)<<4)
#define LVDS_CONFIG_SYNC_IDX1(n)   (((n)&15)<<0)

//SYNCIDX2
//#define LVDS_RESERVED_0(n)       (((n)&0xFFFFFFF)<<4)
#define LVDS_CONFIG_SYNC_IDX2(n)   (((n)&15)<<0)

//SYNCIDX3
//#define LVDS_RESERVED_0(n)       (((n)&0xFFFFFFF)<<4)
#define LVDS_CONFIG_SYNC_IDX3(n)   (((n)&15)<<0)

//RX_BYTE_TRAINING_OK
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_RX_BYTE_TRAINING_OK (1<<0)

//RX_BIT_TRAINING_OK
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_RX_BIT_TRAINING_OK (1<<0)

//ULRDY
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_UL_RDY         (1<<0)

//LEN_PAYLOAD
#define LVDS_RESERVED_0_X3(n)      (((n)&0x3FFFFF)<<10)
#define LVDS_CONFIG_LEN_PAYLOAD(n) (((n)&0x3FF)<<0)

//LEN_SYNC
//#define LVDS_RESERVED_0_X3(n)    (((n)&0x3FFFFF)<<10)
#define LVDS_CONFIG_LEN_SYNC(n)    (((n)&0x3FF)<<0)

//FIX_PATTERN
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_FIX_PATTERN    (1<<0)

//PATTERN0
#define LVDS_CONFIG_PATTERN0(n)    (((n)&0xFFFFFFFF)<<0)

//PATTERN1
#define LVDS_CONFIG_PATTERN1(n)    (((n)&0xFFFFFFFF)<<0)

//SAMPLE_WIDTH
#define LVDS_RESERVED_0_X4(n)      (((n)&0x3FFFFFFF)<<2)
#define LVDS_CONFIG_SAMPLE_WIDTH(n) (((n)&3)<<0)

//DOWNSAMPLE
//#define LVDS_RESERVED_0_X4(n)    (((n)&0x3FFFFFFF)<<2)
#define LVDS_CONFIG_1X2X(n)        (((n)&3)<<0)

//TX_BIT_TRAINING_OK
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_TX_BIT_TRAINING_OK (1<<0)

//TX_BYTE_TRAINING_OK
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_TX_BYTE_TRAINING_OK (1<<0)

//LATCH_WORD0_0
#define LVDS_REG_LATCH_WORD0_0(n)  (((n)&0xFFFFFFFF)<<0)

//LATCH_WORD0_1
#define LVDS_REG_LATCH_WORD0_1(n)  (((n)&0xFFFFFFFF)<<0)

//LATCH_WORD1_0
#define LVDS_REG_LATCH_WORD1_0(n)  (((n)&0xFFFFFFFF)<<0)

//LATCH_WORD1_1
#define LVDS_REG_LATCH_WORD1_1(n)  (((n)&0xFFFFFFFF)<<0)

//LATCH_WORD2_0
#define LVDS_REG_LATCH_WORD2_0(n)  (((n)&0xFFFFFFFF)<<0)

//LATCH_WORD2_1
#define LVDS_REG_LATCH_WORD2_1(n)  (((n)&0xFFFFFFFF)<<0)

//LATCH_WORD3_0
#define LVDS_REG_LATCH_WORD3_0(n)  (((n)&0xFFFFFFFF)<<0)

//LATCH_WORD3_1
#define LVDS_REG_LATCH_WORD3_1(n)  (((n)&0xFFFFFFFF)<<0)

//RES_LVDS_BB
#define LVDS_RESERVED_0_X5(n)      (((n)&0xFFFF)<<16)
#define LVDS_CONFIG_RES_LVDS_BB(n) (((n)&0xFFFF)<<0)

//CLOCK_LVDS
//#define LVDS_RESERVED_0_X5(n)    (((n)&0xFFFF)<<16)
#define LVDS_CONFIG_CLOCK_LVDS(n)  (((n)&0xFFFF)<<0)

//LATCH
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_CONFIG_LATCH          (1<<0)

//HEADER_CONFIG
#define LVDS_RESERVED_0_X6(n)      (((n)&0x1FFFFF)<<11)
#define LVDS_CONFIG_HEADER(n)      (((n)&0xFF)<<3)
#define LVDS_CONFIG_HEADER_LEN(n)  (((n)&7)<<0)

//RX_STATE_STATUS
#define LVDS_RESERVED_0_X7(n)      (((n)&0x3FFFFFF)<<6)
#define LVDS_LVDS_RX_STATE_MACHINE(n) (((n)&0x3F)<<0)

//TX_STATE_STATUS
//#define LVDS_RESERVED_0_X7(n)    (((n)&0x3FFFFFF)<<6)
#define LVDS_LVDS_TX_STATE_MACHINE(n) (((n)&0x3F)<<0)

//RX_PLL_STABLE_TIME
//#define LVDS_RESERVED_0_X5(n)    (((n)&0xFFFF)<<16)
#define LVDS_LVDS_RX_PLL_STABLE_TIME(n) (((n)&0xFFFF)<<0)

//TX_PLL_STABLE_TIME
//#define LVDS_RESERVED_0_X5(n)    (((n)&0xFFFF)<<16)
#define LVDS_LVDS_TX_PLL_STABLE_TIME(n) (((n)&0xFFFF)<<0)

//INTERRUPT
//#define LVDS_RESERVED_0_X4(n)    (((n)&0x3FFFFFFF)<<2)
#define LVDS_LVDS_INTERRUPT_RAW    (1<<1)
#define LVDS_LVDS_INTERRUPT        (1<<0)

//INTERRUPT_CTRL
#define LVDS_RESERVED_0_X8(n)      (((n)&0x7FFFFFF)<<5)
#define LVDS_LVDS_INTERRUPT_CLEAR  (1<<4)
#define LVDS_LVDS_INTERRUPT_SOURCE_SEL(n) (((n)&7)<<1)
#define LVDS_LVDS_INTERRUPT_MASK   (1<<0)

//LVDS_RESET
//#define LVDS_RESERVED_0_X4(n)    (((n)&0x3FFFFFFF)<<2)
#define LVDS_LVDS_RX_RSTN          (1<<1)
#define LVDS_LVDS_TX_RSTN          (1<<0)

//CLK_BAND
//#define LVDS_RESERVED_0(n)       (((n)&0xFFFFFFF)<<4)
#define LVDS_LVDS_CLK_BAND(n)      (((n)&15)<<0)

//PHASE_SEL
#define LVDS_RESERVED_0_X9(n)      (((n)&0x1FFFFFFF)<<3)
#define LVDS_LVDS_PHASE_SEL(n)     (((n)&7)<<0)

//RX_DATA_PNSW
//#define LVDS_RESERVED_0(n)       (((n)&0xFFFFFFF)<<4)
#define LVDS_LVDS_RX_DATA_PNSW(n)  (((n)&15)<<0)

//RX_DL8P_SEL_0
#define LVDS_LVDS_RX_DL8P_SEL_0(n) (((n)&0xFFFFFFFF)<<0)

//RX_DL8P_SEL_1
#define LVDS_LVDS_RX_DL8P_SEL_1(n) (((n)&0xFFFFFFFF)<<0)

//RX_DL128P_SEL
#define LVDS_RESERVED_0_X10(n)     (((n)&0x3FFFF)<<14)
#define LVDS_LVDS_RX_DL128P_SEL(n) (((n)&0x3FFF)<<0)

//RX_PU_DIFF2CMOS
//#define LVDS_RESERVED_0_X8(n)    (((n)&0x7FFFFFF)<<5)
#define LVDS_LVDS_RX_PU_DIFF2CMOS(n) (((n)&31)<<0)

//RX_VCOM_SEL
//#define LVDS_RESERVED_0_X8(n)    (((n)&0x7FFFFFF)<<5)
#define LVDS_LVDS_RX_VCOM_SEL(n)   (((n)&31)<<0)

//TX_DL8P_SEL
#define LVDS_RESERVED_0_X11(n)     (((n)&0x1FFFF)<<15)
#define LVDS_LVDS_TX_DL8P_SEL(n)   (((n)&0x7FFF)<<0)

//TX_DL128P_SEL
#define LVDS_RESERVED_0_X12(n)     (((n)&0x1FFFFFF)<<7)
#define LVDS_LVDS_TX_DL128P_SEL(n) (((n)&0x7F)<<0)

//TX_HZ_ENABLE
//#define LVDS_RESERVED_0_X8(n)    (((n)&0x7FFFFFF)<<5)
#define LVDS_LVDS_TX_HZ_ENABLE(n)  (((n)&31)<<0)

//TX_IREF_BIT
//#define LVDS_RESERVED_0(n)       (((n)&0xFFFFFFF)<<4)
#define LVDS_LVDS_TX_IREF_BIT(n)   (((n)&15)<<0)

//TX_VCM_BIT
//#define LVDS_RESERVED_0_X9(n)    (((n)&0x1FFFFFFF)<<3)
#define LVDS_LVDS_TX_VCM_BIT(n)    (((n)&7)<<0)

//TX_VDM_BIT
//#define LVDS_RESERVED_0_X9(n)    (((n)&0x1FFFFFFF)<<3)
#define LVDS_LVDS_TX_VDM_BIT(n)    (((n)&7)<<0)

//REG_RES
#define LVDS_LVDS_REG_RES(n)       (((n)&0xFFFFFFFF)<<0)

//PLL_REFDIV2_ENABLE
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_LVDS_PLL_REFDIV2_ENABLE (1<<0)

//PLL_CLK_RSTB
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_LVDS_PLL_CLK_RSTB     (1<<0)

//PLL_DIN
//#define LVDS_RESERVED_0_X12(n)   (((n)&0x1FFFFFF)<<7)
#define LVDS_LVDS_PLL_DIN(n)       (((n)&0x7F)<<0)

//PLL_PCON
//#define LVDS_RESERVED_0_X9(n)    (((n)&0x1FFFFFFF)<<3)
#define LVDS_LVDS_PLL_PCON(n)      (((n)&7)<<0)

//PLL_REFMULTI2_ENABLE
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_LVDS_PLL_REFMULTI2_EN (1<<0)

//PLL_VREG_BIT
//#define LVDS_RESERVED_0(n)       (((n)&0xFFFFFFF)<<4)
#define LVDS_LVDS_PLL_VREG_BIT(n)  (((n)&15)<<0)

//PU_PLL
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_LVDS_PU_PLL           (1<<0)

//PLL_LOCK
//#define LVDS_RESERVED_0_X1(n)    (((n)&0x7FFFFFFF)<<1)
#define LVDS_LVDS_PLL_LOCK         (1<<0)

//PLL_REG0
//#define LVDS_RESERVED_0_X5(n)    (((n)&0xFFFF)<<16)
#define LVDS_LVDS_PLL_REG0(n)      (((n)&0xFFFF)<<0)

//PLL_REG1
//#define LVDS_RESERVED_0_X5(n)    (((n)&0xFFFF)<<16)
#define LVDS_LVDS_PLL_REG1(n)      (((n)&0xFFFF)<<0)





#endif


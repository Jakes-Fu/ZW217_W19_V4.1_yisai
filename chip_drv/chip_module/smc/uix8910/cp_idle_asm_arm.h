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


#ifndef _CP_IDLE_ASM_ARM_H_
#define _CP_IDLE_ASM_ARM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !



//==============================================================================
// cp_idle
//------------------------------------------------------------------------------
///
//==============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_IDLE_BASE              0x05090000
#else
#define REG_IDLE_BASE              0x50090000
#endif

#define REG_IDLE_IDL_CTRL_SYS1        0x00000000
#define REG_IDLE_IDL_CTRL_SYS2        0x00000004
#define REG_IDLE_IDL_M_TIMER          0x0000000C
#define REG_IDLE_IDL_WCN_EN           0x00000010
#define REG_IDLE_IDL_CTRL_TIMER       0x00000014
#define REG_IDLE_IDL_M2_SYS           0x00000018
#define REG_IDLE_IDL_TC_START         0x0000001C
#define REG_IDLE_IDL_TC_END           0x00000020
#define REG_IDLE_IDL_AWK_TIMER        0x00000024
#define REG_IDLE_GSM_LP_PU_DONE       0x00000028
#define REG_IDLE_IDL_STA              0x00000044
#define REG_IDLE_IDL_H_CTRL           0x00000048
#define REG_IDLE_IDL_H_VAL            0x0000004C
#define REG_IDLE_IDL_H_GSM            0x00000050
#define REG_IDLE_IDL_H_LTEM           0x00000054
#define REG_IDLE_IDL_AWK_EN           0x00000058
#define REG_IDLE_IDL_AWK_ST           0x0000005C
#define REG_IDLE_IDL_AWK_SELF         0x00000060
#define REG_IDLE_IDL_OSW1_EN          0x00000064
#define REG_IDLE_IDL_OSW1_CONT        0x00000068
#define REG_IDLE_IDL_FN_GSM           0x0000006C
#define REG_IDLE_IDL_FN_LTEM1         0x00000070
#define REG_IDLE_IDL_FN_LTEM2         0x00000074
#define REG_IDLE_IDL_LTEM_RFL         0x00000078
#define REG_IDLE_IDL_LTEM_SFL         0x0000007C
#define REG_IDLE_IDL_SIG_EN           0x00000080
#define REG_IDLE_IDL_SIG_TIMER        0x00000084
#define REG_IDLE_IDL_32K_REF          0x00000088
#define REG_IDLE_IDL_INTEN            0x00000090
#define REG_IDLE_IDL_INTEN_SET        0x00000094
#define REG_IDLE_IDL_INTEN_CLR        0x00000098
#define REG_IDLE_IDLE_INT_STA         0x0000009C
#define REG_IDLE_LTEM1_CFSR_HFN       0x000000AC
#define REG_IDLE_LTEM1_CFSR_FN        0x000000B0
#define REG_IDLE_LTEM1_CFSRS          0x000000B4
#define REG_IDLE_LTEM1_CFSR_RDH       0x000000B8
#define REG_IDLE_LTEM1_CFSR_RDL       0x000000BC
#define REG_IDLE_LTEM1_FRAMC          0x000000C0
#define REG_IDLE_LTEM1_FRAML          0x000000C4
#define REG_IDLE_LTEM1_FRAMLS         0x000000C8
#define REG_IDLE_LTEM1_CFSR_TPH       0x000000CC
#define REG_IDLE_LTEM1_CFSR_TPL       0x000000D0
#define REG_IDLE_LTEM1_FRAMC_TP       0x000000D4
#define REG_IDLE_LTEM2_CFSR_HFN       0x000000D8
#define REG_IDLE_LTEM2_CFSR_FN        0x000000DC
#define REG_IDLE_LTEM2_CFSRS          0x000000E0
#define REG_IDLE_LTEM2_CFSR_RDH       0x000000E4
#define REG_IDLE_LTEM2_CFSR_RDL       0x000000E8
#define REG_IDLE_LTEM2_FRAMC          0x000000EC
#define REG_IDLE_LTEM2_FRAML          0x000000F0
#define REG_IDLE_LTEM2_FRAMLS         0x000000F4
#define REG_IDLE_LTEM2_CFSR_TPH       0x000000F8
#define REG_IDLE_LTEM2_CFSR_TPL       0x000000FC
#define REG_IDLE_LTEM2_FRAMC_TP       0x00000100
#define REG_IDLE_GSM_CFSR             0x00000104
#define REG_IDLE_GSM_CFSRS            0x00000108
#define REG_IDLE_GSM_CFSRO            0x0000010C
#define REG_IDLE_LTEM1_FHL            0x00000110
#define REG_IDLE_LTEM1_FLL            0x00000114
#define REG_IDLE_LTEM1_FCL            0x00000118
#define REG_IDLE_LTEM2_FHL            0x0000011C
#define REG_IDLE_LTEM2_FLL            0x00000120
#define REG_IDLE_LTEM2_FCL            0x00000124
#define REG_IDLE_GSM_FL               0x00000128
#define REG_IDLE_GSM_FCL              0x0000012C
#define REG_IDLE_TPCTRL               0x00000130
#define REG_IDLE_LAYOUTT              0x00000134
#define REG_IDLE_LAYOUTCTRL           0x00000138
#define REG_IDLE_LTEM1_FINT_DLY1      0x0000013C
#define REG_IDLE_LTEM1_FINT_DLY2      0x00000140
#define REG_IDLE_LTEM2_FINT_DLY1      0x00000144
#define REG_IDLE_LTEM2_FINT_DLY2      0x00000148
#define REG_IDLE_FINT_EN              0x0000014C
#define REG_IDLE_TIMER_EN             0x00000150
#define REG_IDLE_IDLE_FRAME_STA       0x00000154
#define REG_IDLE_IDLE_FRAME_LTEM1     0x00000158
#define REG_IDLE_IDLE_FRAME_LTEM2     0x0000015C
#define REG_IDLE_IDLE_FRAME_GSM       0x00000160
#define REG_IDLE_IDLE_FRAME_LTE       0x00000164
#define REG_IDLE_IDLE_FRAME_LTE_CONF    0x00000168
#define REG_IDLE_LTEM_REF_FN          0x0000016C
#define REG_IDLE_LTEM_REF_FNL         0x00000170
#define REG_IDLE_LTEM_REF_FCL         0x00000174
#define REG_IDLE_REF_32K_FNL          0x00000178
#define REG_IDLE_LTEM_REF_FC          0x0000017C
#define REG_IDLE_GSM_FRAML            0x00000180
#define REG_IDLE_IDL_OSW2_EN          0x00000184
#define REG_IDLE_IDL_OSW2_CONT        0x00000188
#define REG_IDLE_IDLE_FRAMC_GSM       0x0000018C
#define REG_IDLE_LTEM1_FINT_DLY3      0x00000190
#define REG_IDLE_LTEM2_FINT_DLY3      0x00000194

//IDL_CTRL_SYS1
#define CP_IDLE_RESERVED_0(n)      (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_IDCT_CTRL_SYS1     (1<<0)

//IDL_CTRL_SYS2
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_IDCT_CTRL_SYS2     (1<<0)

//IDL_M_TIMER
#define CP_IDLE_IDCT_M_SYS(n)      (((n)&0xFFFFFFFF)<<0)

//IDL_WCN_EN
#define CP_IDLE_RESERVED_0_X1(n)   (((n)&0x3FFFFFFF)<<2)
#define CP_IDLE_IDL_WCN_RES_VAL    (1<<1)
#define CP_IDLE_IDL_WCN_EN_IDL_WCN_EN (1<<0)

//IDL_CTRL_TIMER
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_IDCT_CTRL_TIMER    (1<<0)

//IDL_M2_SYS
#define CP_IDLE_M1_SYS(n)          (((n)&0xFFFF)<<16)
#define CP_IDLE_M2_SYS(n)          (((n)&0xFFFF)<<0)

//IDL_TC_START
//#define CP_IDLE_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_IDLE_TC_START_MOD(n)    (((n)&3)<<0)

//IDL_TC_END
#define CP_IDLE_RESERVED_0_X2(n)   (((n)&0x7FF)<<21)
#define CP_IDLE_TC_END_FRAMC(n)    (((n)&0x1FFFF)<<4)
#define CP_IDLE_RESERVED_1(n)      (((n)&3)<<2)
#define CP_IDLE_TC_END_MOD(n)      (((n)&3)<<0)

//IDL_AWK_TIMER
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_WAKE_TIMER         (1<<0)

//GSM_LP_PU_DONE
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_LP_PU_DONE         (1<<0)

//IDL_STA
#define CP_IDLE_RESERVED_0_X3(n)   (((n)&0x7FFFFFF)<<5)
#define CP_IDLE_H_STAT             (1<<4)
#define CP_IDLE_IDLE_LTEM2_TIMER_STAT (1<<3)
#define CP_IDLE_IDLE_LTEM1_TIMER_STAT (1<<2)
#define CP_IDLE_IDLE_GSM_TIMER_STAT (1<<1)
#define CP_IDLE_IDLE_SYS_STAT      (1<<0)

//IDL_H_CTRL
#define CP_IDLE_RESERVED1_0(n)     (((n)&0x1FFFFFF)<<7)
#define CP_IDLE_H_RUN_TIME(n)      (((n)&15)<<3)
#define CP_IDLE_H_AUTO_EN          (1<<2)
#define CP_IDLE_H_CTRL_EN          (1<<1)
#define CP_IDLE_RESERVED2_1        (1<<0)

//IDL_H_VAL
#define CP_IDLE_RESERVED_0_X4(n)   (((n)&31)<<27)
#define CP_IDLE_H_VALUE(n)         (((n)&0x7FFFFFF)<<0)

//IDL_H_GSM
//#define CP_IDLE_RESERVED_0_X4(n) (((n)&31)<<27)
//#define CP_IDLE_H_VALUE(n)       (((n)&0x7FFFFFF)<<0)

//IDL_H_LTEM
//#define CP_IDLE_RESERVED_0_X4(n) (((n)&31)<<27)
//#define CP_IDLE_H_VALUE(n)       (((n)&0x7FFFFFF)<<0)

//IDL_AWK_EN
#define CP_IDLE_RESERVED_0_X5(n)   (((n)&0x7FFF)<<17)
#define CP_IDLE_AWK_SELF_EN        (1<<16)
#define CP_IDLE_RESERVED_1_X1(n)   (((n)&7)<<13)
#define CP_IDLE_AWK_OSW2_EN        (1<<12)
#define CP_IDLE_RESERVED_2(n)      (((n)&7)<<9)
#define CP_IDLE_AWK_OSW1_EN        (1<<8)
#define CP_IDLE_AWK7_EN            (1<<7)
#define CP_IDLE_AWK6_EN            (1<<6)
#define CP_IDLE_AWK5_EN            (1<<5)
#define CP_IDLE_AWK4_EN            (1<<4)
#define CP_IDLE_AWK3_EN            (1<<3)
#define CP_IDLE_AWK2_EN            (1<<2)
#define CP_IDLE_AWK1_EN            (1<<1)
#define CP_IDLE_AWK0_EN            (1<<0)

//IDL_AWK_ST
#define CP_IDLE_RESERVED_0_X6(n)   (((n)&3)<<30)
#define CP_IDLE_THR_STA            (1<<29)
#define CP_IDLE_POW_STA            (1<<28)
#define CP_IDLE_RESERVED_1_X2(n)   (((n)&7)<<25)
#define CP_IDLE_IDLE_STAT          (1<<24)
#define CP_IDLE_RESERVED_2_X1(n)   (((n)&7)<<21)
#define CP_IDLE_AWK_UP_STAT        (1<<20)
#define CP_IDLE_RESERVED_3(n)      (((n)&7)<<17)
#define CP_IDLE_AWK_SELF_STAT      (1<<16)
#define CP_IDLE_RESERVED_4(n)      (((n)&7)<<13)
#define CP_IDLE_AWK_OSW2_STAT      (1<<12)
#define CP_IDLE_RESERVED_5(n)      (((n)&7)<<9)
#define CP_IDLE_AWK_OSW1_STAT      (1<<8)
#define CP_IDLE_AWK7_AWK_STAT      (1<<7)
#define CP_IDLE_AWK6_AWK_STAT      (1<<6)
#define CP_IDLE_AWK5_AWK_STAT      (1<<5)
#define CP_IDLE_AWK4_AWK_STAT      (1<<4)
#define CP_IDLE_AWK3_AWK_STAT      (1<<3)
#define CP_IDLE_AWK2_AWK_STAT      (1<<2)
#define CP_IDLE_AWK1_AWK_STAT      (1<<1)
#define CP_IDLE_AWK0_AWK_STAT      (1<<0)

//IDL_AWK_SELF
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_WAKE_SELF          (1<<0)

//IDL_OSW1_EN
#define CP_IDLE_OSW1_EN            (1<<31)
#define CP_IDLE_OSW1_TIME(n)       (((n)&0x7FFFFFFF)<<0)

//IDL_OSW1_CONT
#define CP_IDLE_OSW1_COUNT(n)      (((n)&0xFFFFFFFF)<<0)

//IDL_FN_GSM
#define CP_IDLE_IDFN_GSM(n)        (((n)&0xFFFFFFFF)<<0)

//IDL_FN_LTEM1
#define CP_IDLE_IDFN_RAD_LTEM(n)   (((n)&0xFFFFFFF)<<4)
#define CP_IDLE_IDFN_SUB_LTEM(n)   (((n)&15)<<0)

//IDL_FN_LTEM2
//#define CP_IDLE_IDFN_RAD_LTEM(n) (((n)&0xFFFFFFF)<<4)
//#define CP_IDLE_IDFN_SUB_LTEM(n) (((n)&15)<<0)

//IDL_LTEM_RFL
//#define CP_IDLE_RESERVED_0_X2(n) (((n)&0x7FF)<<21)
#define CP_IDLE_LTEM_IDLE_RADIOFRAME_PARAMETER(n) (((n)&0x1FFFFF)<<0)

//IDL_LTEM_SFL
//#define CP_IDLE_RESERVED_0_X5(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_LTEM_IDLE_FRAME_PARAMETER(n) (((n)&0x1FFFF)<<0)

//IDL_SIG_EN
#define CP_IDLE_RESERVED_0_X7(n)   (((n)&0xFFFFFFF)<<4)
#define CP_IDLE_IDLE_CG_EN         (1<<3)
#define CP_IDLE_PD_PLL_EN          (1<<2)
#define CP_IDLE_PD_XTAL_EN         (1<<1)
#define CP_IDLE_CHIP_PD_EN         (1<<0)

//IDL_SIG_TIMER
#define CP_IDLE_T4(n)              (((n)&0xFF)<<24)
#define CP_IDLE_T3(n)              (((n)&0xFF)<<16)
#define CP_IDLE_T2(n)              (((n)&0xFF)<<8)
#define CP_IDLE_T1(n)              (((n)&0xFF)<<0)

//IDL_32K_REF
#define CP_IDLE_REK_REF(n)         (((n)&0xFFFFFFFF)<<0)

//IDL_INTEN
#define CP_IDLE_RESERVED_0_X8(n)   (((n)&0x3FF)<<22)
#define CP_IDLE_LTEM2_FRAME3_IRQ_EN (1<<21)
#define CP_IDLE_LTEM1_FRAME3_IRQ_EN (1<<20)
#define CP_IDLE_OSW2_IRQ_AP        (1<<19)
#define CP_IDLE_TIMER_AWK_IRQ_AP   (1<<18)
#define CP_IDLE_PU_REACH_IRQ_AP    (1<<17)
#define CP_IDLE_SYS_WAK_IRQ_AP     (1<<16)
#define CP_IDLE_PU_REACH_IRQ_CP    (1<<15)
#define CP_IDLE_TIMER_AWK_IRQ_CP   (1<<14)
#define CP_IDLE_GSM_FRAME_IRQ_EN   (1<<13)
#define CP_IDLE_TC_END_IRQ         (1<<12)
#define CP_IDLE_TC_START_IRQ       (1<<11)
#define CP_IDLE_RESERVED_1_X3      (1<<10)
#define CP_IDLE_OSW1_IRQ           (1<<9)
#define CP_IDLE_TSTAMP_IRQ         (1<<8)
#define CP_IDLE_IDLE_FRAME_IRQ     (1<<7)
#define CP_IDLE_IDLE_H_IRQ         (1<<6)
#define CP_IDLE_RESERVED_2_X2      (1<<5)
#define CP_IDLE_LAYOUT_IRQ_EN      (1<<4)
#define CP_IDLE_LTEM2_FRAME2_IRQ_EN (1<<3)
#define CP_IDLE_LTEM2_FRAME1_IRQ_EN (1<<2)
#define CP_IDLE_LTEM1_FRAME2_IRQ_EN (1<<1)
#define CP_IDLE_LTEM1_FRAME1_IRQ_EN (1<<0)

//IDL_INTEN_SET
//#define CP_IDLE_RESERVED_0_X8(n) (((n)&0x3FF)<<22)
#define CP_IDLE_INT_EN_SET(n)      (((n)&0x3FFFFF)<<0)

//IDL_INTEN_CLR
//#define CP_IDLE_RESERVED_0_X8(n) (((n)&0x3FF)<<22)
#define CP_IDLE_INT_EN_CLR(n)      (((n)&0x3FFFFF)<<0)

//IDLE_INT_STA
//#define CP_IDLE_RESERVED_0_X8(n) (((n)&0x3FF)<<22)
#define CP_IDLE_LTEM2_FRAME3_IRQ_STATE (1<<21)
#define CP_IDLE_LTEM1_FRAME3_IRQ_STATE (1<<20)
//#define CP_IDLE_OSW2_IRQ_AP      (1<<19)
//#define CP_IDLE_TIMER_AWK_IRQ_AP (1<<18)
//#define CP_IDLE_PU_REACH_IRQ_AP  (1<<17)
//#define CP_IDLE_SYS_WAK_IRQ_AP   (1<<16)
//#define CP_IDLE_PU_REACH_IRQ_CP  (1<<15)
#define CP_IDLE_TIMER_AWK_IRQ_STATE (1<<14)
#define CP_IDLE_GSM_FRAME_IRQ_STATE (1<<13)
#define CP_IDLE_TC_END_IRQ_STATE   (1<<12)
#define CP_IDLE_TC_START_IRQ_STATE (1<<11)
//#define CP_IDLE_RESERVED_1_X3    (1<<10)
#define CP_IDLE_OSW1_IRQ_STATE     (1<<9)
#define CP_IDLE_TSTAMP_IRQ_STATE   (1<<8)
#define CP_IDLE_IDLE_FRAME_IRQ_STATE (1<<7)
#define CP_IDLE_IDLE_H_IRQ_STATE   (1<<6)
//#define CP_IDLE_RESERVED_2_X2    (1<<5)
#define CP_IDLE_LAYOUT_IRQ_STATE   (1<<4)
#define CP_IDLE_LTEM2_FRAME2_IRQ_STATE (1<<3)
#define CP_IDLE_LTEM2_FRAME1_IRQ_STATE (1<<2)
#define CP_IDLE_LTEM1_FRAME2_IRQ_STATE (1<<1)
#define CP_IDLE_LTEM1_FRAME1_IRQ_STATE (1<<0)

//LTEM1_CFSR_HFN
//#define CP_IDLE_RESERVED_0_X8(n) (((n)&0x3FF)<<22)
#define CP_IDLE_LTEM_CFSR_HFN(n)   (((n)&0x3FFFFF)<<0)

//LTEM1_CFSR_FN
#define CP_IDLE_RESERVED_0_X9(n)   (((n)&0x3FFFF)<<14)
#define CP_IDLE_LTEM_CFSR_RAD(n)   (((n)&0x3FF)<<4)
#define CP_IDLE_LTEM_CFSR_SUB(n)   (((n)&15)<<0)

//LTEM1_CFSRS
#define CP_IDLE_RESERVED_0_X10(n)  (((n)&0x7F)<<25)
#define CP_IDLE_ADJUST_DIRECT      (1<<24)
#define CP_IDLE_LTEM_CFSRS(n)      (((n)&0xFFFFFF)<<0)

//LTEM1_CFSR_RDH
//#define CP_IDLE_RESERVED_0_X8(n) (((n)&0x3FF)<<22)
#define CP_IDLE_LTEM_CFSR_RDH(n)   (((n)&0x3FFFFF)<<0)

//LTEM1_CFSR_RDL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
#define CP_IDLE_LTEM_CFSR_RDL_RAD(n) (((n)&0x3FF)<<4)
#define CP_IDLE_LTEM_CFSR_RDL_SUB(n) (((n)&15)<<0)

//LTEM1_FRAMC
#define CP_IDLE_RESERVED_0_X11(n)  (((n)&0xFFFF)<<16)
#define CP_IDLE_LFRAMC(n)          (((n)&0xFFFF)<<0)

//LTEM1_FRAML
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
#define CP_IDLE_LFRAML(n)          (((n)&0xFFFF)<<0)

//LTEM1_FRAMLS
//#define CP_IDLE_RESERVED_0_X5(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_ACTIVE_TIME        (1<<16)
#define CP_IDLE_LFRAMLS(n)         (((n)&0xFFFF)<<0)

//LTEM1_CFSR_TPH
//#define CP_IDLE_RESERVED_0_X8(n) (((n)&0x3FF)<<22)
#define CP_IDLE_LTEM_CFSR_TPH(n)   (((n)&0x3FFFFF)<<0)

//LTEM1_CFSR_TPL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
#define CP_IDLE_LTEM_CFSR_TPL(n)   (((n)&0x3FFF)<<0)

//LTEM1_FRAMC_TP
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
#define CP_IDLE_LTEM_FRAMC_TP(n)   (((n)&0xFFFF)<<0)

//LTEM2_CFSR_HFN
//#define CP_IDLE_RESERVED_0_X8(n) (((n)&0x3FF)<<22)
//#define CP_IDLE_LTEM_CFSR_HFN(n) (((n)&0x3FFFFF)<<0)

//LTEM2_CFSR_FN
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
//#define CP_IDLE_LTEM_CFSR_RAD(n) (((n)&0x3FF)<<4)
//#define CP_IDLE_LTEM_CFSR_SUB(n) (((n)&15)<<0)

//LTEM2_CFSRS
//#define CP_IDLE_RESERVED_0_X10(n) (((n)&0x7F)<<25)
//#define CP_IDLE_ADJUST_DIRECT    (1<<24)
//#define CP_IDLE_LTEM_CFSRS(n)    (((n)&0xFFFFFF)<<0)

//LTEM2_CFSR_RDH
//#define CP_IDLE_RESERVED_0_X8(n) (((n)&0x3FF)<<22)
//#define CP_IDLE_LTEM_CFSR_RDH(n) (((n)&0x3FFFFF)<<0)

//LTEM2_CFSR_RDL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
//#define CP_IDLE_LTEM_CFSR_RDL_RAD(n) (((n)&0x3FF)<<4)
//#define CP_IDLE_LTEM_CFSR_RDL_SUB(n) (((n)&15)<<0)

//LTEM2_FRAMC
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_LFRAMC(n)        (((n)&0xFFFF)<<0)

//LTEM2_FRAML
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_LFRAML(n)        (((n)&0xFFFF)<<0)

//LTEM2_FRAMLS
//#define CP_IDLE_RESERVED_0_X5(n) (((n)&0x7FFF)<<17)
//#define CP_IDLE_ACTIVE_TIME      (1<<16)
//#define CP_IDLE_LFRAMLS(n)       (((n)&0xFFFF)<<0)

//LTEM2_CFSR_TPH
//#define CP_IDLE_RESERVED_0_X8(n) (((n)&0x3FF)<<22)
//#define CP_IDLE_LTEM_CFSR_TPH(n) (((n)&0x3FFFFF)<<0)

//LTEM2_CFSR_TPL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
//#define CP_IDLE_LTEM_CFSR_TPL(n) (((n)&0x3FFF)<<0)

//LTEM2_FRAMC_TP
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_LTEM_FRAMC_TP(n) (((n)&0xFFFF)<<0)

//GSM_CFSR
#define CP_IDLE_RESERVED_0_X12(n)  (((n)&0xFF)<<24)
#define CP_IDLE_GSM_CFSR_GSM_CFSR(n) (((n)&0xFFFFFF)<<0)

//GSM_CFSRS
//#define CP_IDLE_RESERVED_0_X10(n) (((n)&0x7F)<<25)
//#define CP_IDLE_ADJUST_DIRECT    (1<<24)
#define CP_IDLE_GSM_CFSRS_GSM_CFSRS(n) (((n)&0xFFFFFF)<<0)

//GSM_CFSRO
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFF)<<24)
#define CP_IDLE_GSM_CFSR_OVERFLOW(n) (((n)&0xFFFFFF)<<0)

//LTEM1_FHL
//#define CP_IDLE_RESERVED_0_X8(n) (((n)&0x3FF)<<22)
#define CP_IDLE_LTEM_FHL(n)        (((n)&0x3FFFFF)<<0)

//LTEM1_FLL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
#define CP_IDLE_LTEM_FLL(n)        (((n)&0x3FFF)<<0)

//LTEM1_FCL
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
#define CP_IDLE_LTEM_FCL(n)        (((n)&0xFFFF)<<0)

//LTEM2_FHL
//#define CP_IDLE_RESERVED_0_X8(n) (((n)&0x3FF)<<22)
//#define CP_IDLE_LTEM_FHL(n)      (((n)&0x3FFFFF)<<0)

//LTEM2_FLL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
//#define CP_IDLE_LTEM_FLL(n)      (((n)&0x3FFF)<<0)

//LTEM2_FCL
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_LTEM_FCL(n)      (((n)&0xFFFF)<<0)

//GSM_FL
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFF)<<24)
#define CP_IDLE_FL(n)              (((n)&0xFFFFFF)<<0)

//GSM_FCL
//#define CP_IDLE_RESERVED_0_X5(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_GSM_FCL_GSM_FCL(n) (((n)&0x1FFFF)<<0)

//TPCTRL
#define CP_IDLE_RESERVED1_0_X1(n)  (((n)&0x3FFFFF)<<10)
#define CP_IDLE_MOD_SEL(n)         (((n)&3)<<8)
#define CP_IDLE_RESERVED2_1_X1(n)  (((n)&3)<<6)
#define CP_IDLE_INNER_CONFG(n)     (((n)&3)<<4)
#define CP_IDLE_RESERVED3_2(n)     (((n)&7)<<1)
#define CP_IDLE_INNER_CTRL         (1<<0)

//LAYOUTT
#define CP_IDLE_LAYOUTT(n)         (((n)&0xFFFFFFFF)<<0)

//LAYOUTCTRL
#define CP_IDLE_RESERVED1_0_X2(n)  (((n)&0x1FF)<<23)
#define CP_IDLE_CHIP_COUNT(n)      (((n)&0x7FFF)<<8)
#define CP_IDLE_RESERVED2_1_X2(n)  (((n)&0x3F)<<2)
#define CP_IDLE_TIMER_SELECT       (1<<1)
#define CP_IDLE_ENABLE             (1<<0)

//LTEM1_FINT_DLY1
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
#define CP_IDLE_DELAY_TIME(n)      (((n)&0xFFFF)<<0)

//LTEM1_FINT_DLY2
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_DELAY_TIME(n)    (((n)&0xFFFF)<<0)

//LTEM2_FINT_DLY1
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_DELAY_TIME(n)    (((n)&0xFFFF)<<0)

//LTEM2_FINT_DLY2
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_DELAY_TIME(n)    (((n)&0xFFFF)<<0)

//FINT_EN
#define CP_IDLE_RESERVED1_0_X3(n)  (((n)&0x3FF)<<22)
#define CP_IDLE_LTE_M2_FINT_ENABLE(n) (((n)&0x3FF)<<12)
#define CP_IDLE_RESERVED2_1_X3(n)  (((n)&3)<<10)
#define CP_IDLE_LTE_M1_FINT_ENABLE(n) (((n)&0x3FF)<<0)

//TIMER_EN
//#define CP_IDLE_RESERVED_0_X7(n) (((n)&0xFFFFFFF)<<4)
#define CP_IDLE_LTEM_TIMER_ENABLE  (1<<3)
#define CP_IDLE_GSM_TIMER_ENABLE   (1<<2)
#define CP_IDLE_LTE_M2_TIMER_ENABLE (1<<1)
#define CP_IDLE_LTE_M1_TIMER_ENABLE (1<<0)

//IDLE_FRAME_STA
//#define CP_IDLE_RESERVED_0_X7(n) (((n)&0xFFFFFFF)<<4)
#define CP_IDLE_LTE_M_FRAME_STATE  (1<<3)
#define CP_IDLE_GSM_FRAME_STATE    (1<<2)
#define CP_IDLE_LTE_M2_FRAME_STATE (1<<1)
#define CP_IDLE_LTE_M1_FRAME_STATE (1<<0)

//IDLE_FRAME_LTEM1
#define CP_IDLE_RESERVED1_0_X4(n)  (((n)&0x7F)<<25)
#define CP_IDLE_FRAME_CONF         (1<<24)
#define CP_IDLE_RESERVED2_1_X4(n)  (((n)&3)<<22)
#define CP_IDLE_FRAME_CFSR(n)      (((n)&0x3FFFFF)<<0)

//IDLE_FRAME_LTEM2
//#define CP_IDLE_RESERVED1_0_X4(n) (((n)&0x7F)<<25)
//#define CP_IDLE_FRAME_CONF       (1<<24)
//#define CP_IDLE_RESERVED2_1_X4(n) (((n)&3)<<22)
//#define CP_IDLE_FRAME_CFSR(n)    (((n)&0x3FFFFF)<<0)

//IDLE_FRAME_GSM
//#define CP_IDLE_RESERVED_0_X10(n) (((n)&0x7F)<<25)
//#define CP_IDLE_FRAME_CONF       (1<<24)
#define CP_IDLE_FRAME_CFSR_X1(n)   (((n)&0xFFFFFF)<<0)

//IDLE_FRAME_LTE
#define CP_IDLE_FRAME_REF_CFSR(n)  (((n)&0xFFFFFFFF)<<0)

//IDLE_FRAME_LTE_CONF
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_FRAME_REF_LTE_CONF (1<<0)

//LTEM_REF_FN
#define CP_IDLE_LTEM_REF_FN_LTEM_REF_FN(n) (((n)&0xFFFFFFFF)<<0)

//LTEM_REF_FNL
#define CP_IDLE_REF_LTEM_FNL(n)    (((n)&0xFFFFFFFF)<<0)

//LTEM_REF_FCL
#define CP_IDLE_RESERVED_0_X13(n)  (((n)&0x1FFFF)<<15)
#define CP_IDLE_REF_LTEM_FCL(n)    (((n)&0x7FFF)<<0)

//REF_32K_FNL
#define CP_IDLE_REF_32K_FNL_REF_32K_FNL(n) (((n)&0xFFFFFFFF)<<0)

//LTEM_REF_FC
//#define CP_IDLE_RESERVED_0_X13(n) (((n)&0x1FFFF)<<15)
#define CP_IDLE_LTEM_REF_FC_LTEM_REF_FC(n) (((n)&0x7FFF)<<0)

//GSM_FRAML
//#define CP_IDLE_RESERVED_0_X5(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_GSM_FRAML(n)       (((n)&0x1FFFF)<<0)

//IDL_OSW2_EN
#define CP_IDLE_OSW2_EN            (1<<31)
#define CP_IDLE_OSW2_TIME(n)       (((n)&0x7FFFFFFF)<<0)

//IDL_OSW2_CONT
#define CP_IDLE_OSW2_COUNT(n)      (((n)&0xFFFFFFFF)<<0)

//IDLE_FRAMC_GSM
//#define CP_IDLE_RESERVED_0_X5(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_FRAMC_CFSR(n)      (((n)&0x1FFFF)<<0)

//LTEM1_FINT_DLY3
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_DELAY_TIME(n)    (((n)&0xFFFF)<<0)

//LTEM2_FINT_DLY3
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_DELAY_TIME(n)    (((n)&0xFFFF)<<0)




#endif

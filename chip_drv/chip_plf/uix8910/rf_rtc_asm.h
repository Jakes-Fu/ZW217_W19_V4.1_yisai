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


#ifndef _RF_RTC_ASM_H_
#define _RF_RTC_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'rf_rtc'."
#endif



//==============================================================================
// rf_rtc
//------------------------------------------------------------------------------
///
//==============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_RF_RTC_BASE            0x05034000
#else
#define REG_RF_RTC_BASE            0x50034000
#endif

#define REG_RF_RTC_BASE_HI         BASE_HI(REG_RF_RTC_BASE)
#define REG_RF_RTC_BASE_LO         BASE_LO(REG_RF_RTC_BASE)

#define REG_RF_RTC_REG_00_REG      REG_RF_RTC_BASE_LO + 0x000000A0
#define REG_RF_RTC_REG_0C_REG      REG_RF_RTC_BASE_LO + 0x000000AC
#define REG_RF_RTC_REG_10_REG      REG_RF_RTC_BASE_LO + 0x000000B0
#define REG_RF_RTC_REG_14_REG      REG_RF_RTC_BASE_LO + 0x000000B4
#define REG_RF_RTC_REG_18_REG      REG_RF_RTC_BASE_LO + 0x000000B8
#define REG_RF_RTC_REG_1C_REG      REG_RF_RTC_BASE_LO + 0x000000BC
#define REG_RF_RTC_REG_C0_REG      REG_RF_RTC_BASE_LO + 0x000000C0
#define REG_RF_RTC_REG_C4_REG      REG_RF_RTC_BASE_LO + 0x000000C4
#define REG_RF_RTC_REG_C8_REG      REG_RF_RTC_BASE_LO + 0x000000C8
#define REG_RF_RTC_REG_CC_REG      REG_RF_RTC_BASE_LO + 0x000000CC
#define REG_RF_RTC_REG_D0_REG      REG_RF_RTC_BASE_LO + 0x000000D0
#define REG_RF_RTC_REG_D4_REG      REG_RF_RTC_BASE_LO + 0x000000D4
#define REG_RF_RTC_REG_D8_REG      REG_RF_RTC_BASE_LO + 0x000000D8

//REG_00_REG
#define RF_RTC_REG_A0_BIT(n)       (((n)&0xFF)<<8)
#define RF_RTC_REG_A1_BIT(n)       (((n)&0xFF)<<0)

//REG_0c_REG
#define RF_RTC_REG_A6_BIT(n)       (((n)&0xFF)<<8)
#define RF_RTC_REG_A7_BIT(n)       (((n)&0xFF)<<0)

//REG_10_REG
#define RF_RTC_REG_A8_BIT(n)       (((n)&0xFF)<<8)
#define RF_RTC_REG_A9_BIT(n)       (((n)&0xFF)<<0)

//REG_14_REG
#define RF_RTC_REG_AA_BIT(n)       (((n)&0xFF)<<8)
#define RF_RTC_REG_AB_BIT(n)       (((n)&0xFF)<<0)

//REG_18_REG
#define RF_RTC_REG_AC_BIT(n)       (((n)&0xFF)<<8)
#define RF_RTC_REG_AD_BIT(n)       (((n)&0xFF)<<0)

//REG_1c_REG
#define RF_RTC_REG_AE_BIT(n)       (((n)&0xFF)<<8)
#define RF_RTC_REG_AF_BIT(n)       (((n)&0xFF)<<0)

//REG_c0_REG
#define RF_RTC_RESERVED_0          (1<<15)
#define RF_RTC_ENABLE_CLK_6P5M     (1<<14)
#define RF_RTC_DIG_AFC_BIT_REG(n)  (((n)&0x3FFF)<<0)

//REG_c4_REG
#define RF_RTC_XTAL26M_PLLS2_EN    (1<<15)
#define RF_RTC_XTAL26M_PLLS1_EN    (1<<14)
#define RF_RTC_XTAL26M_TSXADC_EN   (1<<13)
#define RF_RTC_XTAL_REG_BIT(n)     (((n)&15)<<9)
#define RF_RTC_XTAL26M_PWADC_EN    (1<<8)
#define RF_RTC_XTAL_OSC_IBIT_L(n)  (((n)&15)<<4)
#define RF_RTC_XTAL_OSC_IBIT_N(n)  (((n)&15)<<0)

//REG_c8_REG
#define RF_RTC_RESERVED_0_X1(n)    (((n)&15)<<12)
#define RF_RTC_XTAL26M_PLLCAL_EN   (1<<11)
#define RF_RTC_XTAL26M_TS_EN       (1<<10)
#define RF_RTC_XTAL_FIXI_BIT_L(n)  (((n)&31)<<5)
#define RF_RTC_XTAL_FIXI_BIT_N(n)  (((n)&31)<<0)

//REG_cc_REG
//#define RF_RTC_RESERVED_0        (1<<15)
#define RF_RTC_XTAL26M_PMIC_EN     (1<<14)
#define RF_RTC_XTAL26M_INTERFACE_EN (1<<13)
#define RF_RTC_XDRV_PMIC_POWER_BIT(n) (((n)&7)<<10)
#define RF_RTC_XDRV_AUX2_POWER_BIT(n) (((n)&7)<<7)
#define RF_RTC_XDRV_AUX1_POWER_BIT(n) (((n)&7)<<4)
#define RF_RTC_XDRV_REG_BIT(n)     (((n)&15)<<0)

//REG_d0_REG
#define RF_RTC_XTAL_HSEL_L         (1<<15)
#define RF_RTC_XTAL_CAPBANK_BIT_L(n) (((n)&0x7F)<<8)
#define RF_RTC_XTAL_HSEL_N         (1<<7)
#define RF_RTC_XTAL_CAPBANK_BIT_N(n) (((n)&0x7F)<<0)

//REG_d4_REG
#define RF_RTC_RTC_RESER_L(n)      (((n)&0xFFFF)<<0)

//REG_d8_REG
#define RF_RTC_RTC_RESER_N(n)      (((n)&0xFFFF)<<0)




#endif

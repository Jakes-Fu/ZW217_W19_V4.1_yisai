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


#ifndef _RF_RTC_H_
#define _RF_RTC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'rf_rtc'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// RF_RTC_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_RF_RTC_BASE            0x05034000
#else
#define REG_RF_RTC_BASE            0x50034000
#endif

typedef volatile struct
{
    REG32 Reserved_00000000[40];                //0x00000000
    REG32                          REG_00_REG;                   //0x000000A0
    REG32 Reserved_000000A4[2];                 //0x000000A4
    REG32                          REG_0c_REG;                   //0x000000AC
    REG32                          REG_10_REG;                   //0x000000B0
    REG32                          REG_14_REG;                   //0x000000B4
    REG32                          REG_18_REG;                   //0x000000B8
    REG32                          REG_1c_REG;                   //0x000000BC
    REG32                          REG_c0_REG;                   //0x000000C0
    REG32                          REG_c4_REG;                   //0x000000C4
    REG32                          REG_c8_REG;                   //0x000000C8
    REG32                          REG_cc_REG;                   //0x000000CC
    REG32                          REG_d0_REG;                   //0x000000D0
    REG32                          REG_d4_REG;                   //0x000000D4
    REG32                          REG_d8_REG;                   //0x000000D8
} HWP_RF_RTC_T;

#define hwp_rfRtc                  ((HWP_RF_RTC_T*) REG_ACCESS_ADDRESS(REG_RF_RTC_BASE))


//REG_00_REG
typedef union {
    REG32 v;
    struct {
        REG32 reg_a1_bit : 8; // [7:0]
        REG32 reg_a0_bit : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_RTC_REG_00_REG_T;

//REG_0c_REG
typedef union {
    REG32 v;
    struct {
        REG32 reg_a7_bit : 8; // [7:0]
        REG32 reg_a6_bit : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_RTC_REG_0C_REG_T;

//REG_10_REG
typedef union {
    REG32 v;
    struct {
        REG32 reg_a9_bit : 8; // [7:0]
        REG32 reg_a8_bit : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_RTC_REG_10_REG_T;

//REG_14_REG
typedef union {
    REG32 v;
    struct {
        REG32 reg_ab_bit : 8; // [7:0]
        REG32 reg_aa_bit : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_RTC_REG_14_REG_T;

//REG_18_REG
typedef union {
    REG32 v;
    struct {
        REG32 reg_ad_bit : 8; // [7:0]
        REG32 reg_ac_bit : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_RTC_REG_18_REG_T;

//REG_1c_REG
typedef union {
    REG32 v;
    struct {
        REG32 reg_af_bit : 8; // [7:0]
        REG32 reg_ae_bit : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_RTC_REG_1C_REG_T;

//REG_c0_REG
typedef union {
    REG32 v;
    struct {
        REG32 dig_afc_bit_reg : 14; // [13:0]
        REG32 enable_clk_6p5m : 1; // [14]
        REG32 __31_15 : 17;
    } b;
} REG_RF_RTC_REG_C0_REG_T;

//REG_c4_REG
typedef union {
    REG32 v;
    struct {
        REG32 xtal_osc_ibit_n : 4; // [3:0]
        REG32 xtal_osc_ibit_l : 4; // [7:4]
        REG32 xtal26m_pwadc_en : 1; // [8]
        REG32 xtal_reg_bit : 4; // [12:9]
        REG32 xtal26m_tsxadc_en : 1; // [13]
        REG32 xtal26m_plls1_en : 1; // [14]
        REG32 xtal26m_plls2_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RF_RTC_REG_C4_REG_T;

//REG_c8_REG
typedef union {
    REG32 v;
    struct {
        REG32 xtal_fixi_bit_n : 5; // [4:0]
        REG32 xtal_fixi_bit_l : 5; // [9:5]
        REG32 xtal26m_ts_en : 1; // [10]
        REG32 xtal26m_pllcal_en : 1; // [11]
        REG32 __31_12 : 20;
    } b;
} REG_RF_RTC_REG_C8_REG_T;

//REG_cc_REG
typedef union {
    REG32 v;
    struct {
        REG32 xdrv_reg_bit : 4; // [3:0]
        REG32 xdrv_aux1_power_bit : 3; // [6:4]
        REG32 xdrv_aux2_power_bit : 3; // [9:7]
        REG32 xdrv_pmic_power_bit : 3; // [12:10]
        REG32 xtal26m_interface_en : 1; // [13]
        REG32 xtal26m_pmic_en : 1; // [14]
        REG32 __31_15 : 17;
    } b;
} REG_RF_RTC_REG_CC_REG_T;

//REG_d0_REG
typedef union {
    REG32 v;
    struct {
        REG32 xtal_capbank_bit_n : 7; // [6:0]
        REG32 xtal_hsel_n : 1; // [7]
        REG32 xtal_capbank_bit_l : 7; // [14:8]
        REG32 xtal_hsel_l : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RF_RTC_REG_D0_REG_T;

//REG_d4_REG
typedef union {
    REG32 v;
    struct {
        REG32 rtc_reser_l : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_RTC_REG_D4_REG_T;

//REG_d8_REG
typedef union {
    REG32 v;
    struct {
        REG32 rtc_reser_n : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_RTC_REG_D8_REG_T;


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


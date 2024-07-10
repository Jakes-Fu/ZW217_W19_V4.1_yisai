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


#ifndef _RDA2720M_BLTC_H_
#define _RDA2720M_BLTC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'rda2720m_bltc'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// RDA2720M_BLTC_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_RDA2720M_BLTC_BASE     0x180

typedef volatile struct
{
    REG32                          BLTC_CTRL;                    //0x00000000
    REG32                          BLTC_R_PRESCL;                //0x00000004
    REG32                          BLTC_R_DUTY;                  //0x00000008
    REG32                          BLTC_R_CURVE0;                //0x0000000C
    REG32                          BLTC_R_CURVE1;                //0x00000010
    REG32                          BLTC_G_PRESCL;                //0x00000014
    REG32                          BLTC_G_DUTY;                  //0x00000018
    REG32                          BLTC_G_CURVE0;                //0x0000001C
    REG32                          BLTC_G_CURVE1;                //0x00000020
    REG32                          BLTC_B_PRESCL;                //0x00000024
    REG32                          BLTC_B_DUTY;                  //0x00000028
    REG32                          BLTC_B_CURVE0;                //0x0000002C
    REG32                          BLTC_B_CURVE1;                //0x00000030
    REG32                          BLTC_STS;                     //0x00000034
    REG32                          RG_RGB_V0;                    //0x00000038
    REG32                          RG_RGB_V1;                    //0x0000003C
    REG32                          RG_RGB_V2;                    //0x00000040
    REG32                          RG_RGB_V3;                    //0x00000044
    REG32                          BLTC_WLED_PRESCL;             //0x00000048
    REG32                          BLTC_WLED_DUTY;               //0x0000004C
    REG32                          BLTC_WLED_CURVE0;             //0x00000050
    REG32                          BLTC_WLED_CURVE1;             //0x00000054
    REG32                          BLTC_PD_CTRL;                 //0x00000058
    REG32                          BLTC_VERSION;                 //0x0000005C
} HWP_RDA2720M_BLTC_T;

#define hwp_rda2720mBltc           ((HWP_RDA2720M_BLTC_T*) REG_ACCESS_ADDRESS(REG_RDA2720M_BLTC_BASE))


//BLTC_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 r_run : 1; // [0]
        REG32 r_type : 1; // [1]
        REG32 r_sel : 1; // [2]
        REG32 r_sw : 1; // [3]
        REG32 g_run : 1; // [4]
        REG32 g_type : 1; // [5]
        REG32 g_sel : 1; // [6]
        REG32 g_sw : 1; // [7]
        REG32 b_run : 1; // [8]
        REG32 b_type : 1; // [9]
        REG32 b_sel : 1; // [10]
        REG32 b_sw : 1; // [11]
        REG32 wled_run : 1; // [12]
        REG32 wled_type : 1; // [13]
        REG32 wled_sel : 1; // [14]
        REG32 wled_sw : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_BLTC_BLTC_CTRL_T;

//BLTC_R_PRESCL
typedef union {
    REG32 v;
    struct {
        REG32 prescl : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_RDA2720M_BLTC_BLTC_R_PRESCL_T;

//BLTC_R_DUTY
typedef union {
    REG32 v;
    struct {
        REG32 mod : 8; // [7:0]
        REG32 duty : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_BLTC_BLTC_R_DUTY_T;

//BLTC_R_CURVE0
typedef union {
    REG32 v;
    struct {
        REG32 trise : 6; // [5:0]
        REG32 __7_6 : 2;
        REG32 tfall : 6; // [13:8]
        REG32 __31_14 : 18;
    } b;
} REG_RDA2720M_BLTC_BLTC_R_CURVE0_T;

//BLTC_R_CURVE1
typedef union {
    REG32 v;
    struct {
        REG32 thigh : 8; // [7:0]
        REG32 tlow : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_BLTC_BLTC_R_CURVE1_T;

//BLTC_G_PRESCL
typedef union {
    REG32 v;
    struct {
        REG32 prescl : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_RDA2720M_BLTC_BLTC_G_PRESCL_T;

//BLTC_G_DUTY
typedef union {
    REG32 v;
    struct {
        REG32 mod : 8; // [7:0]
        REG32 duty : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_BLTC_BLTC_G_DUTY_T;

//BLTC_G_CURVE0
typedef union {
    REG32 v;
    struct {
        REG32 trise : 6; // [5:0]
        REG32 __7_6 : 2;
        REG32 tfall : 6; // [13:8]
        REG32 __31_14 : 18;
    } b;
} REG_RDA2720M_BLTC_BLTC_G_CURVE0_T;

//BLTC_G_CURVE1
typedef union {
    REG32 v;
    struct {
        REG32 thigh : 8; // [7:0]
        REG32 tlow : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_BLTC_BLTC_G_CURVE1_T;

//BLTC_B_PRESCL
typedef union {
    REG32 v;
    struct {
        REG32 prescl : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_RDA2720M_BLTC_BLTC_B_PRESCL_T;

//BLTC_B_DUTY
typedef union {
    REG32 v;
    struct {
        REG32 mod : 8; // [7:0]
        REG32 duty : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_BLTC_BLTC_B_DUTY_T;

//BLTC_B_CURVE0
typedef union {
    REG32 v;
    struct {
        REG32 trise : 6; // [5:0]
        REG32 __7_6 : 2;
        REG32 tfall : 6; // [13:8]
        REG32 __31_14 : 18;
    } b;
} REG_RDA2720M_BLTC_BLTC_B_CURVE0_T;

//BLTC_B_CURVE1
typedef union {
    REG32 v;
    struct {
        REG32 thigh : 8; // [7:0]
        REG32 tlow : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_BLTC_BLTC_B_CURVE1_T;

//BLTC_STS
typedef union {
    REG32 v;
    struct {
        REG32 bltc_r_busy : 1; // [0], read only
        REG32 bltc_g_busy : 1; // [1], read only
        REG32 bltc_b_busy : 1; // [2], read only
        REG32 bltc_wled_busy : 1; // [3], read only
        REG32 __31_4 : 28;
    } b;
} REG_RDA2720M_BLTC_BLTC_STS_T;

//RG_RGB_V0
typedef union {
    REG32 v;
    struct {
        REG32 rg_rgb_v0 : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_BLTC_RG_RGB_V0_T;

//RG_RGB_V1
typedef union {
    REG32 v;
    struct {
        REG32 rg_rgb_v1 : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_BLTC_RG_RGB_V1_T;

//RG_RGB_V2
typedef union {
    REG32 v;
    struct {
        REG32 rg_rgb_v2 : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_BLTC_RG_RGB_V2_T;

//RG_RGB_V3
typedef union {
    REG32 v;
    struct {
        REG32 rg_rgb_v3 : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_BLTC_RG_RGB_V3_T;

//BLTC_WLED_PRESCL
typedef union {
    REG32 v;
    struct {
        REG32 prescl : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_RDA2720M_BLTC_BLTC_WLED_PRESCL_T;

//BLTC_WLED_DUTY
typedef union {
    REG32 v;
    struct {
        REG32 mod : 8; // [7:0]
        REG32 duty : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_BLTC_BLTC_WLED_DUTY_T;

//BLTC_WLED_CURVE0
typedef union {
    REG32 v;
    struct {
        REG32 trise : 6; // [5:0]
        REG32 __7_6 : 2;
        REG32 tfall : 6; // [13:8]
        REG32 __31_14 : 18;
    } b;
} REG_RDA2720M_BLTC_BLTC_WLED_CURVE0_T;

//BLTC_WLED_CURVE1
typedef union {
    REG32 v;
    struct {
        REG32 thigh : 8; // [7:0]
        REG32 tlow : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_BLTC_BLTC_WLED_CURVE1_T;

//BLTC_PD_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 sw_pd : 1; // [0]
        REG32 hw_pd : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_RDA2720M_BLTC_BLTC_PD_CTRL_T;

//BLTC_VERSION
typedef union {
    REG32 v;
    struct {
        REG32 bltc_version : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_BLTC_BLTC_VERSION_T;


//BLTC_CTRL
#define RDA2720M_BLTC_RESERVED_15  (1<<31)
#define RDA2720M_BLTC_RESERVED_14  (1<<30)
#define RDA2720M_BLTC_RESERVED_13  (1<<29)
#define RDA2720M_BLTC_RESERVED_12  (1<<28)
#define RDA2720M_BLTC_RESERVED_11  (1<<27)
#define RDA2720M_BLTC_RESERVED_10  (1<<26)
#define RDA2720M_BLTC_RESERVED_9   (1<<25)
#define RDA2720M_BLTC_RESERVED_8   (1<<24)
#define RDA2720M_BLTC_RESERVED_7   (1<<23)
#define RDA2720M_BLTC_RESERVED_6   (1<<22)
#define RDA2720M_BLTC_RESERVED_5   (1<<21)
#define RDA2720M_BLTC_RESERVED_4   (1<<20)
#define RDA2720M_BLTC_RESERVED_3   (1<<19)
#define RDA2720M_BLTC_RESERVED_2   (1<<18)
#define RDA2720M_BLTC_RESERVED_1   (1<<17)
#define RDA2720M_BLTC_RESERVED_0   (1<<16)
#define RDA2720M_BLTC_WLED_SW      (1<<15)
#define RDA2720M_BLTC_WLED_SEL     (1<<14)
#define RDA2720M_BLTC_WLED_TYPE    (1<<13)
#define RDA2720M_BLTC_WLED_RUN     (1<<12)
#define RDA2720M_BLTC_B_SW         (1<<11)
#define RDA2720M_BLTC_B_SEL        (1<<10)
#define RDA2720M_BLTC_B_TYPE       (1<<9)
#define RDA2720M_BLTC_B_RUN        (1<<8)
#define RDA2720M_BLTC_G_SW         (1<<7)
#define RDA2720M_BLTC_G_SEL        (1<<6)
#define RDA2720M_BLTC_G_TYPE       (1<<5)
#define RDA2720M_BLTC_G_RUN        (1<<4)
#define RDA2720M_BLTC_R_SW         (1<<3)
#define RDA2720M_BLTC_R_SEL        (1<<2)
#define RDA2720M_BLTC_R_TYPE       (1<<1)
#define RDA2720M_BLTC_R_RUN        (1<<0)

//BLTC_R_PRESCL
#define RDA2720M_BLTC_RESERVED_23  (1<<31)
#define RDA2720M_BLTC_RESERVED_22  (1<<30)
#define RDA2720M_BLTC_RESERVED_21  (1<<29)
#define RDA2720M_BLTC_RESERVED_20  (1<<28)
#define RDA2720M_BLTC_RESERVED_19  (1<<27)
#define RDA2720M_BLTC_RESERVED_18  (1<<26)
#define RDA2720M_BLTC_RESERVED_17  (1<<25)
#define RDA2720M_BLTC_RESERVED_16  (1<<24)
#define RDA2720M_BLTC_RESERVED_15_X1 (1<<23)
#define RDA2720M_BLTC_RESERVED_14_X1 (1<<22)
#define RDA2720M_BLTC_RESERVED_13_X1 (1<<21)
#define RDA2720M_BLTC_RESERVED_12_X1 (1<<20)
#define RDA2720M_BLTC_RESERVED_11_X1 (1<<19)
#define RDA2720M_BLTC_RESERVED_10_X1 (1<<18)
#define RDA2720M_BLTC_RESERVED_9_X1 (1<<17)
#define RDA2720M_BLTC_RESERVED_8_X1 (1<<16)
#define RDA2720M_BLTC_RESERVED_7_X1 (1<<15)
#define RDA2720M_BLTC_RESERVED_6_X1 (1<<14)
#define RDA2720M_BLTC_RESERVED_5_X1 (1<<13)
#define RDA2720M_BLTC_RESERVED_4_X1 (1<<12)
#define RDA2720M_BLTC_RESERVED_3_X1 (1<<11)
#define RDA2720M_BLTC_RESERVED_2_X1 (1<<10)
#define RDA2720M_BLTC_RESERVED_1_X1 (1<<9)
#define RDA2720M_BLTC_RESERVED_0_X1 (1<<8)
#define RDA2720M_BLTC_PRESCL(n)    (((n)&0xFF)<<0)

//BLTC_R_DUTY
//#define RDA2720M_BLTC_RESERVED_15 (1<<31)
//#define RDA2720M_BLTC_RESERVED_14 (1<<30)
//#define RDA2720M_BLTC_RESERVED_13 (1<<29)
//#define RDA2720M_BLTC_RESERVED_12 (1<<28)
//#define RDA2720M_BLTC_RESERVED_11 (1<<27)
//#define RDA2720M_BLTC_RESERVED_10 (1<<26)
//#define RDA2720M_BLTC_RESERVED_9 (1<<25)
//#define RDA2720M_BLTC_RESERVED_8 (1<<24)
//#define RDA2720M_BLTC_RESERVED_7 (1<<23)
//#define RDA2720M_BLTC_RESERVED_6 (1<<22)
//#define RDA2720M_BLTC_RESERVED_5 (1<<21)
//#define RDA2720M_BLTC_RESERVED_4 (1<<20)
//#define RDA2720M_BLTC_RESERVED_3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_2 (1<<18)
//#define RDA2720M_BLTC_RESERVED_1 (1<<17)
//#define RDA2720M_BLTC_RESERVED_0 (1<<16)
#define RDA2720M_BLTC_DUTY(n)      (((n)&0xFF)<<8)
#define RDA2720M_BLTC_MOD(n)       (((n)&0xFF)<<0)

//BLTC_R_CURVE0
#define RDA2720M_BLTC_RESERVED_19_X1 (1<<31)
#define RDA2720M_BLTC_RESERVED_18_X1 (1<<30)
#define RDA2720M_BLTC_RESERVED_17_X1 (1<<29)
#define RDA2720M_BLTC_RESERVED_16_X1 (1<<28)
#define RDA2720M_BLTC_RESERVED_15_X2 (1<<27)
#define RDA2720M_BLTC_RESERVED_14_X2 (1<<26)
#define RDA2720M_BLTC_RESERVED_13_X2 (1<<25)
#define RDA2720M_BLTC_RESERVED_12_X2 (1<<24)
#define RDA2720M_BLTC_RESERVED_11_X2 (1<<23)
#define RDA2720M_BLTC_RESERVED_10_X2 (1<<22)
#define RDA2720M_BLTC_RESERVED_9_X2 (1<<21)
#define RDA2720M_BLTC_RESERVED_8_X2 (1<<20)
#define RDA2720M_BLTC_RESERVED_7_X2 (1<<19)
#define RDA2720M_BLTC_RESERVED_6_X2 (1<<18)
#define RDA2720M_BLTC_RESERVED_5_X2 (1<<17)
#define RDA2720M_BLTC_RESERVED_4_X2 (1<<16)
#define RDA2720M_BLTC_RESERVED_3_X2 (1<<15)
#define RDA2720M_BLTC_RESERVED_2_X2 (1<<14)
#define RDA2720M_BLTC_TFALL(n)     (((n)&0x3F)<<8)
#define RDA2720M_BLTC_RESERVED_1_X2 (1<<7)
#define RDA2720M_BLTC_RESERVED_0_X2 (1<<6)
#define RDA2720M_BLTC_TRISE(n)     (((n)&0x3F)<<0)

//BLTC_R_CURVE1
//#define RDA2720M_BLTC_RESERVED_15 (1<<31)
//#define RDA2720M_BLTC_RESERVED_14 (1<<30)
//#define RDA2720M_BLTC_RESERVED_13 (1<<29)
//#define RDA2720M_BLTC_RESERVED_12 (1<<28)
//#define RDA2720M_BLTC_RESERVED_11 (1<<27)
//#define RDA2720M_BLTC_RESERVED_10 (1<<26)
//#define RDA2720M_BLTC_RESERVED_9 (1<<25)
//#define RDA2720M_BLTC_RESERVED_8 (1<<24)
//#define RDA2720M_BLTC_RESERVED_7 (1<<23)
//#define RDA2720M_BLTC_RESERVED_6 (1<<22)
//#define RDA2720M_BLTC_RESERVED_5 (1<<21)
//#define RDA2720M_BLTC_RESERVED_4 (1<<20)
//#define RDA2720M_BLTC_RESERVED_3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_2 (1<<18)
//#define RDA2720M_BLTC_RESERVED_1 (1<<17)
//#define RDA2720M_BLTC_RESERVED_0 (1<<16)
#define RDA2720M_BLTC_TLOW(n)      (((n)&0xFF)<<8)
#define RDA2720M_BLTC_THIGH(n)     (((n)&0xFF)<<0)

//BLTC_G_PRESCL
//#define RDA2720M_BLTC_RESERVED_23 (1<<31)
//#define RDA2720M_BLTC_RESERVED_22 (1<<30)
//#define RDA2720M_BLTC_RESERVED_21 (1<<29)
//#define RDA2720M_BLTC_RESERVED_20 (1<<28)
//#define RDA2720M_BLTC_RESERVED_19 (1<<27)
//#define RDA2720M_BLTC_RESERVED_18 (1<<26)
//#define RDA2720M_BLTC_RESERVED_17 (1<<25)
//#define RDA2720M_BLTC_RESERVED_16 (1<<24)
//#define RDA2720M_BLTC_RESERVED_15_X1 (1<<23)
//#define RDA2720M_BLTC_RESERVED_14_X1 (1<<22)
//#define RDA2720M_BLTC_RESERVED_13_X1 (1<<21)
//#define RDA2720M_BLTC_RESERVED_12_X1 (1<<20)
//#define RDA2720M_BLTC_RESERVED_11_X1 (1<<19)
//#define RDA2720M_BLTC_RESERVED_10_X1 (1<<18)
//#define RDA2720M_BLTC_RESERVED_9_X1 (1<<17)
//#define RDA2720M_BLTC_RESERVED_8_X1 (1<<16)
//#define RDA2720M_BLTC_RESERVED_7_X1 (1<<15)
//#define RDA2720M_BLTC_RESERVED_6_X1 (1<<14)
//#define RDA2720M_BLTC_RESERVED_5_X1 (1<<13)
//#define RDA2720M_BLTC_RESERVED_4_X1 (1<<12)
//#define RDA2720M_BLTC_RESERVED_3_X1 (1<<11)
//#define RDA2720M_BLTC_RESERVED_2_X1 (1<<10)
//#define RDA2720M_BLTC_RESERVED_1_X1 (1<<9)
//#define RDA2720M_BLTC_RESERVED_0_X1 (1<<8)
//#define RDA2720M_BLTC_PRESCL(n)  (((n)&0xFF)<<0)

//BLTC_G_DUTY
//#define RDA2720M_BLTC_RESERVED_15 (1<<31)
//#define RDA2720M_BLTC_RESERVED_14 (1<<30)
//#define RDA2720M_BLTC_RESERVED_13 (1<<29)
//#define RDA2720M_BLTC_RESERVED_12 (1<<28)
//#define RDA2720M_BLTC_RESERVED_11 (1<<27)
//#define RDA2720M_BLTC_RESERVED_10 (1<<26)
//#define RDA2720M_BLTC_RESERVED_9 (1<<25)
//#define RDA2720M_BLTC_RESERVED_8 (1<<24)
//#define RDA2720M_BLTC_RESERVED_7 (1<<23)
//#define RDA2720M_BLTC_RESERVED_6 (1<<22)
//#define RDA2720M_BLTC_RESERVED_5 (1<<21)
//#define RDA2720M_BLTC_RESERVED_4 (1<<20)
//#define RDA2720M_BLTC_RESERVED_3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_2 (1<<18)
//#define RDA2720M_BLTC_RESERVED_1 (1<<17)
//#define RDA2720M_BLTC_RESERVED_0 (1<<16)
//#define RDA2720M_BLTC_DUTY(n)    (((n)&0xFF)<<8)
//#define RDA2720M_BLTC_MOD(n)     (((n)&0xFF)<<0)

//BLTC_G_CURVE0
//#define RDA2720M_BLTC_RESERVED_19_X1 (1<<31)
//#define RDA2720M_BLTC_RESERVED_18_X1 (1<<30)
//#define RDA2720M_BLTC_RESERVED_17_X1 (1<<29)
//#define RDA2720M_BLTC_RESERVED_16_X1 (1<<28)
//#define RDA2720M_BLTC_RESERVED_15_X2 (1<<27)
//#define RDA2720M_BLTC_RESERVED_14_X2 (1<<26)
//#define RDA2720M_BLTC_RESERVED_13_X2 (1<<25)
//#define RDA2720M_BLTC_RESERVED_12_X2 (1<<24)
//#define RDA2720M_BLTC_RESERVED_11_X2 (1<<23)
//#define RDA2720M_BLTC_RESERVED_10_X2 (1<<22)
//#define RDA2720M_BLTC_RESERVED_9_X2 (1<<21)
//#define RDA2720M_BLTC_RESERVED_8_X2 (1<<20)
//#define RDA2720M_BLTC_RESERVED_7_X2 (1<<19)
//#define RDA2720M_BLTC_RESERVED_6_X2 (1<<18)
//#define RDA2720M_BLTC_RESERVED_5_X2 (1<<17)
//#define RDA2720M_BLTC_RESERVED_4_X2 (1<<16)
//#define RDA2720M_BLTC_RESERVED_3_X2 (1<<15)
//#define RDA2720M_BLTC_RESERVED_2_X2 (1<<14)
//#define RDA2720M_BLTC_TFALL(n)   (((n)&0x3F)<<8)
//#define RDA2720M_BLTC_RESERVED_1_X2 (1<<7)
//#define RDA2720M_BLTC_RESERVED_0_X2 (1<<6)
//#define RDA2720M_BLTC_TRISE(n)   (((n)&0x3F)<<0)

//BLTC_G_CURVE1
//#define RDA2720M_BLTC_RESERVED_15 (1<<31)
//#define RDA2720M_BLTC_RESERVED_14 (1<<30)
//#define RDA2720M_BLTC_RESERVED_13 (1<<29)
//#define RDA2720M_BLTC_RESERVED_12 (1<<28)
//#define RDA2720M_BLTC_RESERVED_11 (1<<27)
//#define RDA2720M_BLTC_RESERVED_10 (1<<26)
//#define RDA2720M_BLTC_RESERVED_9 (1<<25)
//#define RDA2720M_BLTC_RESERVED_8 (1<<24)
//#define RDA2720M_BLTC_RESERVED_7 (1<<23)
//#define RDA2720M_BLTC_RESERVED_6 (1<<22)
//#define RDA2720M_BLTC_RESERVED_5 (1<<21)
//#define RDA2720M_BLTC_RESERVED_4 (1<<20)
//#define RDA2720M_BLTC_RESERVED_3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_2 (1<<18)
//#define RDA2720M_BLTC_RESERVED_1 (1<<17)
//#define RDA2720M_BLTC_RESERVED_0 (1<<16)
//#define RDA2720M_BLTC_TLOW(n)    (((n)&0xFF)<<8)
//#define RDA2720M_BLTC_THIGH(n)   (((n)&0xFF)<<0)

//BLTC_B_PRESCL
#define RDA2720M_BLTC_RESERVED_16_X2 (1<<31)
#define RDA2720M_BLTC_RESERVED_15_X3 (1<<30)
#define RDA2720M_BLTC_RESERVED_14_X3 (1<<29)
#define RDA2720M_BLTC_RESERVED_13_X3 (1<<28)
#define RDA2720M_BLTC_RESERVED_12_X3 (1<<27)
#define RDA2720M_BLTC_RESERVED_11_X3 (1<<26)
#define RDA2720M_BLTC_RESERVED_10_X3 (1<<25)
#define RDA2720M_BLTC_RESERVED_9_X3 (1<<24)
#define RDA2720M_BLTC_RESERVED_8_X3 (1<<23)
#define RDA2720M_BLTC_RESERVED_7_X3 (1<<22)
#define RDA2720M_BLTC_RESERVED_6_X3 (1<<21)
#define RDA2720M_BLTC_RESERVED_5_X3 (1<<20)
#define RDA2720M_BLTC_RESERVED_4_X3 (1<<19)
#define RDA2720M_BLTC_RESERVED_3_X3 (1<<18)
#define RDA2720M_BLTC_RESERVED_2_X3 (1<<17)
#define RDA2720M_BLTC_RESERVED_1_X3 (1<<16)
#define RDA2720M_BLTC_BLTC_B_PRESCL_RESERVED_0(n) (((n)&0xFF)<<8)
//#define RDA2720M_BLTC_PRESCL(n)  (((n)&0xFF)<<0)

//BLTC_B_DUTY
//#define RDA2720M_BLTC_RESERVED_15 (1<<31)
//#define RDA2720M_BLTC_RESERVED_14 (1<<30)
//#define RDA2720M_BLTC_RESERVED_13 (1<<29)
//#define RDA2720M_BLTC_RESERVED_12 (1<<28)
//#define RDA2720M_BLTC_RESERVED_11 (1<<27)
//#define RDA2720M_BLTC_RESERVED_10 (1<<26)
//#define RDA2720M_BLTC_RESERVED_9 (1<<25)
//#define RDA2720M_BLTC_RESERVED_8 (1<<24)
//#define RDA2720M_BLTC_RESERVED_7 (1<<23)
//#define RDA2720M_BLTC_RESERVED_6 (1<<22)
//#define RDA2720M_BLTC_RESERVED_5 (1<<21)
//#define RDA2720M_BLTC_RESERVED_4 (1<<20)
//#define RDA2720M_BLTC_RESERVED_3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_2 (1<<18)
//#define RDA2720M_BLTC_RESERVED_1 (1<<17)
//#define RDA2720M_BLTC_RESERVED_0 (1<<16)
//#define RDA2720M_BLTC_DUTY(n)    (((n)&0xFF)<<8)
//#define RDA2720M_BLTC_MOD(n)     (((n)&0xFF)<<0)

//BLTC_B_CURVE0
#define RDA2720M_BLTC_RESERVED_17_X2 (1<<31)
#define RDA2720M_BLTC_RESERVED_16_X3 (1<<30)
#define RDA2720M_BLTC_RESERVED_15_X4 (1<<29)
#define RDA2720M_BLTC_RESERVED_14_X4 (1<<28)
#define RDA2720M_BLTC_RESERVED_13_X4 (1<<27)
#define RDA2720M_BLTC_RESERVED_12_X4 (1<<26)
#define RDA2720M_BLTC_RESERVED_11_X4 (1<<25)
#define RDA2720M_BLTC_RESERVED_10_X4 (1<<24)
#define RDA2720M_BLTC_RESERVED_9_X4 (1<<23)
#define RDA2720M_BLTC_RESERVED_8_X4 (1<<22)
#define RDA2720M_BLTC_RESERVED_7_X4 (1<<21)
#define RDA2720M_BLTC_RESERVED_6_X4 (1<<20)
#define RDA2720M_BLTC_RESERVED_5_X4 (1<<19)
#define RDA2720M_BLTC_RESERVED_4_X4 (1<<18)
#define RDA2720M_BLTC_RESERVED_3_X4 (1<<17)
#define RDA2720M_BLTC_RESERVED_2_X4 (1<<16)
#define RDA2720M_BLTC_BLTC_B_CURVE0_RESERVED_0(n) (((n)&3)<<14)
//#define RDA2720M_BLTC_TFALL(n)   (((n)&0x3F)<<8)
#define RDA2720M_BLTC_BLTC_B_CURVE0_RESERVED_1(n) (((n)&3)<<6)
//#define RDA2720M_BLTC_TRISE(n)   (((n)&0x3F)<<0)

//BLTC_B_CURVE1
//#define RDA2720M_BLTC_RESERVED_15 (1<<31)
//#define RDA2720M_BLTC_RESERVED_14 (1<<30)
//#define RDA2720M_BLTC_RESERVED_13 (1<<29)
//#define RDA2720M_BLTC_RESERVED_12 (1<<28)
//#define RDA2720M_BLTC_RESERVED_11 (1<<27)
//#define RDA2720M_BLTC_RESERVED_10 (1<<26)
//#define RDA2720M_BLTC_RESERVED_9 (1<<25)
//#define RDA2720M_BLTC_RESERVED_8 (1<<24)
//#define RDA2720M_BLTC_RESERVED_7 (1<<23)
//#define RDA2720M_BLTC_RESERVED_6 (1<<22)
//#define RDA2720M_BLTC_RESERVED_5 (1<<21)
//#define RDA2720M_BLTC_RESERVED_4 (1<<20)
//#define RDA2720M_BLTC_RESERVED_3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_2 (1<<18)
//#define RDA2720M_BLTC_RESERVED_1 (1<<17)
//#define RDA2720M_BLTC_RESERVED_0 (1<<16)
//#define RDA2720M_BLTC_TLOW(n)    (((n)&0xFF)<<8)
//#define RDA2720M_BLTC_THIGH(n)   (((n)&0xFF)<<0)

//BLTC_STS
//#define RDA2720M_BLTC_RESERVED_16_X2 (1<<31)
//#define RDA2720M_BLTC_RESERVED_15_X3 (1<<30)
//#define RDA2720M_BLTC_RESERVED_14_X3 (1<<29)
//#define RDA2720M_BLTC_RESERVED_13_X3 (1<<28)
//#define RDA2720M_BLTC_RESERVED_12_X3 (1<<27)
//#define RDA2720M_BLTC_RESERVED_11_X3 (1<<26)
//#define RDA2720M_BLTC_RESERVED_10_X3 (1<<25)
//#define RDA2720M_BLTC_RESERVED_9_X3 (1<<24)
//#define RDA2720M_BLTC_RESERVED_8_X3 (1<<23)
//#define RDA2720M_BLTC_RESERVED_7_X3 (1<<22)
//#define RDA2720M_BLTC_RESERVED_6_X3 (1<<21)
//#define RDA2720M_BLTC_RESERVED_5_X3 (1<<20)
//#define RDA2720M_BLTC_RESERVED_4_X3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_3_X3 (1<<18)
//#define RDA2720M_BLTC_RESERVED_2_X3 (1<<17)
//#define RDA2720M_BLTC_RESERVED_1_X3 (1<<16)
#define RDA2720M_BLTC_BLTC_STS_RESERVED_0(n) (((n)&0xFFF)<<4)
#define RDA2720M_BLTC_BLTC_WLED_BUSY (1<<3)
#define RDA2720M_BLTC_BLTC_B_BUSY  (1<<2)
#define RDA2720M_BLTC_BLTC_G_BUSY  (1<<1)
#define RDA2720M_BLTC_BLTC_R_BUSY  (1<<0)

//RG_RGB_V0
//#define RDA2720M_BLTC_RESERVED_16_X2 (1<<31)
//#define RDA2720M_BLTC_RESERVED_15_X3 (1<<30)
//#define RDA2720M_BLTC_RESERVED_14_X3 (1<<29)
//#define RDA2720M_BLTC_RESERVED_13_X3 (1<<28)
//#define RDA2720M_BLTC_RESERVED_12_X3 (1<<27)
//#define RDA2720M_BLTC_RESERVED_11_X3 (1<<26)
//#define RDA2720M_BLTC_RESERVED_10_X3 (1<<25)
//#define RDA2720M_BLTC_RESERVED_9_X3 (1<<24)
//#define RDA2720M_BLTC_RESERVED_8_X3 (1<<23)
//#define RDA2720M_BLTC_RESERVED_7_X3 (1<<22)
//#define RDA2720M_BLTC_RESERVED_6_X3 (1<<21)
//#define RDA2720M_BLTC_RESERVED_5_X3 (1<<20)
//#define RDA2720M_BLTC_RESERVED_4_X3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_3_X3 (1<<18)
//#define RDA2720M_BLTC_RESERVED_2_X3 (1<<17)
//#define RDA2720M_BLTC_RESERVED_1_X3 (1<<16)
#define RDA2720M_BLTC_RG_RGB_V0_RESERVED_0(n) (((n)&0x3FF)<<6)
#define RDA2720M_BLTC_RG_RGB_V0(n) (((n)&0x3F)<<0)

//RG_RGB_V1
//#define RDA2720M_BLTC_RESERVED_16_X2 (1<<31)
//#define RDA2720M_BLTC_RESERVED_15_X3 (1<<30)
//#define RDA2720M_BLTC_RESERVED_14_X3 (1<<29)
//#define RDA2720M_BLTC_RESERVED_13_X3 (1<<28)
//#define RDA2720M_BLTC_RESERVED_12_X3 (1<<27)
//#define RDA2720M_BLTC_RESERVED_11_X3 (1<<26)
//#define RDA2720M_BLTC_RESERVED_10_X3 (1<<25)
//#define RDA2720M_BLTC_RESERVED_9_X3 (1<<24)
//#define RDA2720M_BLTC_RESERVED_8_X3 (1<<23)
//#define RDA2720M_BLTC_RESERVED_7_X3 (1<<22)
//#define RDA2720M_BLTC_RESERVED_6_X3 (1<<21)
//#define RDA2720M_BLTC_RESERVED_5_X3 (1<<20)
//#define RDA2720M_BLTC_RESERVED_4_X3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_3_X3 (1<<18)
//#define RDA2720M_BLTC_RESERVED_2_X3 (1<<17)
//#define RDA2720M_BLTC_RESERVED_1_X3 (1<<16)
#define RDA2720M_BLTC_RG_RGB_V1_RESERVED_0(n) (((n)&0x3FF)<<6)
#define RDA2720M_BLTC_RG_RGB_V1(n) (((n)&0x3F)<<0)

//RG_RGB_V2
//#define RDA2720M_BLTC_RESERVED_16_X2 (1<<31)
//#define RDA2720M_BLTC_RESERVED_15_X3 (1<<30)
//#define RDA2720M_BLTC_RESERVED_14_X3 (1<<29)
//#define RDA2720M_BLTC_RESERVED_13_X3 (1<<28)
//#define RDA2720M_BLTC_RESERVED_12_X3 (1<<27)
//#define RDA2720M_BLTC_RESERVED_11_X3 (1<<26)
//#define RDA2720M_BLTC_RESERVED_10_X3 (1<<25)
//#define RDA2720M_BLTC_RESERVED_9_X3 (1<<24)
//#define RDA2720M_BLTC_RESERVED_8_X3 (1<<23)
//#define RDA2720M_BLTC_RESERVED_7_X3 (1<<22)
//#define RDA2720M_BLTC_RESERVED_6_X3 (1<<21)
//#define RDA2720M_BLTC_RESERVED_5_X3 (1<<20)
//#define RDA2720M_BLTC_RESERVED_4_X3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_3_X3 (1<<18)
//#define RDA2720M_BLTC_RESERVED_2_X3 (1<<17)
//#define RDA2720M_BLTC_RESERVED_1_X3 (1<<16)
#define RDA2720M_BLTC_RG_RGB_V2_RESERVED_0(n) (((n)&0x3FF)<<6)
#define RDA2720M_BLTC_RG_RGB_V2(n) (((n)&0x3F)<<0)

//RG_RGB_V3
//#define RDA2720M_BLTC_RESERVED_16_X2 (1<<31)
//#define RDA2720M_BLTC_RESERVED_15_X3 (1<<30)
//#define RDA2720M_BLTC_RESERVED_14_X3 (1<<29)
//#define RDA2720M_BLTC_RESERVED_13_X3 (1<<28)
//#define RDA2720M_BLTC_RESERVED_12_X3 (1<<27)
//#define RDA2720M_BLTC_RESERVED_11_X3 (1<<26)
//#define RDA2720M_BLTC_RESERVED_10_X3 (1<<25)
//#define RDA2720M_BLTC_RESERVED_9_X3 (1<<24)
//#define RDA2720M_BLTC_RESERVED_8_X3 (1<<23)
//#define RDA2720M_BLTC_RESERVED_7_X3 (1<<22)
//#define RDA2720M_BLTC_RESERVED_6_X3 (1<<21)
//#define RDA2720M_BLTC_RESERVED_5_X3 (1<<20)
//#define RDA2720M_BLTC_RESERVED_4_X3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_3_X3 (1<<18)
//#define RDA2720M_BLTC_RESERVED_2_X3 (1<<17)
//#define RDA2720M_BLTC_RESERVED_1_X3 (1<<16)
#define RDA2720M_BLTC_RG_RGB_V3_RESERVED_0(n) (((n)&0x3FF)<<6)
#define RDA2720M_BLTC_RG_RGB_V3(n) (((n)&0x3F)<<0)

//BLTC_WLED_PRESCL
//#define RDA2720M_BLTC_RESERVED_23 (1<<31)
//#define RDA2720M_BLTC_RESERVED_22 (1<<30)
//#define RDA2720M_BLTC_RESERVED_21 (1<<29)
//#define RDA2720M_BLTC_RESERVED_20 (1<<28)
//#define RDA2720M_BLTC_RESERVED_19 (1<<27)
//#define RDA2720M_BLTC_RESERVED_18 (1<<26)
//#define RDA2720M_BLTC_RESERVED_17 (1<<25)
//#define RDA2720M_BLTC_RESERVED_16 (1<<24)
//#define RDA2720M_BLTC_RESERVED_15_X1 (1<<23)
//#define RDA2720M_BLTC_RESERVED_14_X1 (1<<22)
//#define RDA2720M_BLTC_RESERVED_13_X1 (1<<21)
//#define RDA2720M_BLTC_RESERVED_12_X1 (1<<20)
//#define RDA2720M_BLTC_RESERVED_11_X1 (1<<19)
//#define RDA2720M_BLTC_RESERVED_10_X1 (1<<18)
//#define RDA2720M_BLTC_RESERVED_9_X1 (1<<17)
//#define RDA2720M_BLTC_RESERVED_8_X1 (1<<16)
//#define RDA2720M_BLTC_RESERVED_7_X1 (1<<15)
//#define RDA2720M_BLTC_RESERVED_6_X1 (1<<14)
//#define RDA2720M_BLTC_RESERVED_5_X1 (1<<13)
//#define RDA2720M_BLTC_RESERVED_4_X1 (1<<12)
//#define RDA2720M_BLTC_RESERVED_3_X1 (1<<11)
//#define RDA2720M_BLTC_RESERVED_2_X1 (1<<10)
//#define RDA2720M_BLTC_RESERVED_1_X1 (1<<9)
//#define RDA2720M_BLTC_RESERVED_0_X1 (1<<8)
//#define RDA2720M_BLTC_PRESCL(n)  (((n)&0xFF)<<0)

//BLTC_WLED_DUTY
//#define RDA2720M_BLTC_RESERVED_15 (1<<31)
//#define RDA2720M_BLTC_RESERVED_14 (1<<30)
//#define RDA2720M_BLTC_RESERVED_13 (1<<29)
//#define RDA2720M_BLTC_RESERVED_12 (1<<28)
//#define RDA2720M_BLTC_RESERVED_11 (1<<27)
//#define RDA2720M_BLTC_RESERVED_10 (1<<26)
//#define RDA2720M_BLTC_RESERVED_9 (1<<25)
//#define RDA2720M_BLTC_RESERVED_8 (1<<24)
//#define RDA2720M_BLTC_RESERVED_7 (1<<23)
//#define RDA2720M_BLTC_RESERVED_6 (1<<22)
//#define RDA2720M_BLTC_RESERVED_5 (1<<21)
//#define RDA2720M_BLTC_RESERVED_4 (1<<20)
//#define RDA2720M_BLTC_RESERVED_3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_2 (1<<18)
//#define RDA2720M_BLTC_RESERVED_1 (1<<17)
//#define RDA2720M_BLTC_RESERVED_0 (1<<16)
//#define RDA2720M_BLTC_DUTY(n)    (((n)&0xFF)<<8)
//#define RDA2720M_BLTC_MOD(n)     (((n)&0xFF)<<0)

//BLTC_WLED_CURVE0
//#define RDA2720M_BLTC_RESERVED_19_X1 (1<<31)
//#define RDA2720M_BLTC_RESERVED_18_X1 (1<<30)
//#define RDA2720M_BLTC_RESERVED_17_X1 (1<<29)
//#define RDA2720M_BLTC_RESERVED_16_X1 (1<<28)
//#define RDA2720M_BLTC_RESERVED_15_X2 (1<<27)
//#define RDA2720M_BLTC_RESERVED_14_X2 (1<<26)
//#define RDA2720M_BLTC_RESERVED_13_X2 (1<<25)
//#define RDA2720M_BLTC_RESERVED_12_X2 (1<<24)
//#define RDA2720M_BLTC_RESERVED_11_X2 (1<<23)
//#define RDA2720M_BLTC_RESERVED_10_X2 (1<<22)
//#define RDA2720M_BLTC_RESERVED_9_X2 (1<<21)
//#define RDA2720M_BLTC_RESERVED_8_X2 (1<<20)
//#define RDA2720M_BLTC_RESERVED_7_X2 (1<<19)
//#define RDA2720M_BLTC_RESERVED_6_X2 (1<<18)
//#define RDA2720M_BLTC_RESERVED_5_X2 (1<<17)
//#define RDA2720M_BLTC_RESERVED_4_X2 (1<<16)
//#define RDA2720M_BLTC_RESERVED_3_X2 (1<<15)
//#define RDA2720M_BLTC_RESERVED_2_X2 (1<<14)
//#define RDA2720M_BLTC_TFALL(n)   (((n)&0x3F)<<8)
//#define RDA2720M_BLTC_RESERVED_1_X2 (1<<7)
//#define RDA2720M_BLTC_RESERVED_0_X2 (1<<6)
//#define RDA2720M_BLTC_TRISE(n)   (((n)&0x3F)<<0)

//BLTC_WLED_CURVE1
//#define RDA2720M_BLTC_RESERVED_15 (1<<31)
//#define RDA2720M_BLTC_RESERVED_14 (1<<30)
//#define RDA2720M_BLTC_RESERVED_13 (1<<29)
//#define RDA2720M_BLTC_RESERVED_12 (1<<28)
//#define RDA2720M_BLTC_RESERVED_11 (1<<27)
//#define RDA2720M_BLTC_RESERVED_10 (1<<26)
//#define RDA2720M_BLTC_RESERVED_9 (1<<25)
//#define RDA2720M_BLTC_RESERVED_8 (1<<24)
//#define RDA2720M_BLTC_RESERVED_7 (1<<23)
//#define RDA2720M_BLTC_RESERVED_6 (1<<22)
//#define RDA2720M_BLTC_RESERVED_5 (1<<21)
//#define RDA2720M_BLTC_RESERVED_4 (1<<20)
//#define RDA2720M_BLTC_RESERVED_3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_2 (1<<18)
//#define RDA2720M_BLTC_RESERVED_1 (1<<17)
//#define RDA2720M_BLTC_RESERVED_0 (1<<16)
//#define RDA2720M_BLTC_TLOW(n)    (((n)&0xFF)<<8)
//#define RDA2720M_BLTC_THIGH(n)   (((n)&0xFF)<<0)

//BLTC_PD_CTRL
#define RDA2720M_BLTC_RESERVED_29  (1<<31)
#define RDA2720M_BLTC_RESERVED_28  (1<<30)
#define RDA2720M_BLTC_RESERVED_27  (1<<29)
#define RDA2720M_BLTC_RESERVED_26  (1<<28)
#define RDA2720M_BLTC_RESERVED_25  (1<<27)
#define RDA2720M_BLTC_RESERVED_24  (1<<26)
#define RDA2720M_BLTC_RESERVED_23_X1 (1<<25)
#define RDA2720M_BLTC_RESERVED_22_X1 (1<<24)
#define RDA2720M_BLTC_RESERVED_21_X1 (1<<23)
#define RDA2720M_BLTC_RESERVED_20_X1 (1<<22)
#define RDA2720M_BLTC_RESERVED_19_X2 (1<<21)
#define RDA2720M_BLTC_RESERVED_18_X2 (1<<20)
#define RDA2720M_BLTC_RESERVED_17_X3 (1<<19)
#define RDA2720M_BLTC_RESERVED_16_X4 (1<<18)
#define RDA2720M_BLTC_RESERVED_15_X5 (1<<17)
#define RDA2720M_BLTC_RESERVED_14_X5 (1<<16)
#define RDA2720M_BLTC_RESERVED_13_X5 (1<<15)
#define RDA2720M_BLTC_RESERVED_12_X5 (1<<14)
#define RDA2720M_BLTC_RESERVED_11_X5 (1<<13)
#define RDA2720M_BLTC_RESERVED_10_X5 (1<<12)
#define RDA2720M_BLTC_RESERVED_9_X5 (1<<11)
#define RDA2720M_BLTC_RESERVED_8_X5 (1<<10)
#define RDA2720M_BLTC_RESERVED_7_X5 (1<<9)
#define RDA2720M_BLTC_RESERVED_6_X5 (1<<8)
#define RDA2720M_BLTC_RESERVED_5_X5 (1<<7)
#define RDA2720M_BLTC_RESERVED_4_X5 (1<<6)
#define RDA2720M_BLTC_RESERVED_3_X5 (1<<5)
#define RDA2720M_BLTC_RESERVED_2_X5 (1<<4)
#define RDA2720M_BLTC_RESERVED_1_X4 (1<<3)
#define RDA2720M_BLTC_RESERVED_0_X3 (1<<2)
#define RDA2720M_BLTC_HW_PD        (1<<1)
#define RDA2720M_BLTC_SW_PD        (1<<0)

//BLTC_VERSION
//#define RDA2720M_BLTC_RESERVED_15 (1<<31)
//#define RDA2720M_BLTC_RESERVED_14 (1<<30)
//#define RDA2720M_BLTC_RESERVED_13 (1<<29)
//#define RDA2720M_BLTC_RESERVED_12 (1<<28)
//#define RDA2720M_BLTC_RESERVED_11 (1<<27)
//#define RDA2720M_BLTC_RESERVED_10 (1<<26)
//#define RDA2720M_BLTC_RESERVED_9 (1<<25)
//#define RDA2720M_BLTC_RESERVED_8 (1<<24)
//#define RDA2720M_BLTC_RESERVED_7 (1<<23)
//#define RDA2720M_BLTC_RESERVED_6 (1<<22)
//#define RDA2720M_BLTC_RESERVED_5 (1<<21)
//#define RDA2720M_BLTC_RESERVED_4 (1<<20)
//#define RDA2720M_BLTC_RESERVED_3 (1<<19)
//#define RDA2720M_BLTC_RESERVED_2 (1<<18)
//#define RDA2720M_BLTC_RESERVED_1 (1<<17)
//#define RDA2720M_BLTC_RESERVED_0 (1<<16)
#define RDA2720M_BLTC_BLTC_VERSION(n) (((n)&0xFFFF)<<0)





#endif


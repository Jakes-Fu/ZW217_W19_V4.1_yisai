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


#ifndef _TSEN_ADC_H_
#define _TSEN_ADC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'tsen_adc'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// TSEN_ADC_TOP_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_TSEN_ADC_BASE          0x0503B000
#else
#define REG_TSEN_ADC_BASE          0x5003B000
#endif

typedef volatile struct
{
    REG32 Reserved_00000000;                    //0x00000000
    REG32 Reserved_00000004;                    //0x00000004
    REG32 Reserved_00000008;                    //0x00000008
    REG32                          tst_tsen_bist_test;           //0x0000000C
    REG32                          tst_tsen_bist_test1;          //0x00000010
    REG32                          tst_tsen_c0_test_res0;        //0x00000014
    REG32                          tst_tsen_c0_test_res1;        //0x00000018
    REG32                          tst_tsen_c0_test_res2;        //0x0000001C
    REG32                          tst_tsen_c0_test_res3;        //0x00000020
    REG32                          tst_tsen_c1_test_res0;        //0x00000024
    REG32                          tst_tsen_c1_test_res1;        //0x00000028
    REG32                          tst_tsen_c1_test_res2;        //0x0000002C
    REG32                          tst_tsen_c1_test_res3;        //0x00000030
    REG32                          tst_tsen_c2_test_res0;        //0x00000034
    REG32                          tst_tsen_c2_test_res1;        //0x00000038
    REG32                          tst_tsen_c2_test_res2;        //0x0000003C
    REG32                          tst_tsen_c2_test_res3;        //0x00000040
    REG32                          tst_tsen_c3_test_res0;        //0x00000044
    REG32                          tst_tsen_c3_test_res1;        //0x00000048
    REG32                          tst_tsen_c3_test_res2;        //0x0000004C
    REG32                          tst_tsen_c3_test_res3;        //0x00000050
} HWP_TSEN_ADC_TOP_T;

#define hwp_tsenAdc                ((HWP_TSEN_ADC_TOP_T*) REG_ACCESS_ADDRESS(REG_TSEN_ADC_BASE))


//tst_tsen_bist_test
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_bist_done : 1; // [0], read only
        REG32 tst_tsen_bist_cfg3 : 3; // [3:1]
        REG32 tst_tsen_bist_cfg2 : 3; // [6:4]
        REG32 tst_tsen_bist_cfg1 : 3; // [9:7]
        REG32 tst_tsen_bist_cfg0 : 3; // [12:10]
        REG32 tst_tsen_bist_bypass : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_BIST_TEST_T;

//tst_tsen_bist_test1
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_bit_en : 1; // [0]
        REG32 tst_tsen_bist_time_sel_cfg3 : 2; // [2:1]
        REG32 tst_tsen_bist_time_sel_cfg2 : 2; // [4:3]
        REG32 tst_tsen_bist_time_sel_cfg1 : 2; // [6:5]
        REG32 tst_tsen_bist_time_sel_cfg0 : 2; // [8:7]
        REG32 tst_tsen_bist_code_sel : 1; // [9]
        REG32 __31_10 : 22;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_BIST_TEST1_T;

//tst_tsen_c0_test_res0
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c0_res0 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C0_TEST_RES0_T;

//tst_tsen_c0_test_res1
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c0_res1 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C0_TEST_RES1_T;

//tst_tsen_c0_test_res2
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c0_res2 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C0_TEST_RES2_T;

//tst_tsen_c0_test_res3
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c0_res3 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C0_TEST_RES3_T;

//tst_tsen_c1_test_res0
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c1_res0 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C1_TEST_RES0_T;

//tst_tsen_c1_test_res1
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c1_res1 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C1_TEST_RES1_T;

//tst_tsen_c1_test_res2
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c1_res2 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C1_TEST_RES2_T;

//tst_tsen_c1_test_res3
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c1_res3 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C1_TEST_RES3_T;

//tst_tsen_c2_test_res0
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c2_res0 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C2_TEST_RES0_T;

//tst_tsen_c2_test_res1
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c2_res1 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C2_TEST_RES1_T;

//tst_tsen_c2_test_res2
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c2_res2 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C2_TEST_RES2_T;

//tst_tsen_c2_test_res3
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c2_res3 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C2_TEST_RES3_T;

//tst_tsen_c3_test_res0
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c3_res0 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C3_TEST_RES0_T;

//tst_tsen_c3_test_res1
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c3_res1 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C3_TEST_RES1_T;

//tst_tsen_c3_test_res2
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c3_res2 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C3_TEST_RES2_T;

//tst_tsen_c3_test_res3
typedef union {
    REG32 v;
    struct {
        REG32 tst_tsen_c3_res3 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_TSEN_ADC_TOP_TST_TSEN_C3_TEST_RES3_T;


//tst_tsen_bist_test
#define TSEN_ADC_TOP_TST_TSEN_BIST_BYPASS (1<<13)
#define TSEN_ADC_TOP_TST_TSEN_BIST_CFG0(n) (((n)&7)<<10)
#define TSEN_ADC_TOP_TST_TSEN_BIST_CFG1(n) (((n)&7)<<7)
#define TSEN_ADC_TOP_TST_TSEN_BIST_CFG2(n) (((n)&7)<<4)
#define TSEN_ADC_TOP_TST_TSEN_BIST_CFG3(n) (((n)&7)<<1)
#define TSEN_ADC_TOP_TST_TSEN_BIST_DONE (1<<0)

//tst_tsen_bist_test1
#define TSEN_ADC_TOP_TST_TSEN_BIST_CODE_SEL (1<<9)
#define TSEN_ADC_TOP_TST_TSEN_BIST_TIME_SEL_CFG0(n) (((n)&3)<<7)
#define TSEN_ADC_TOP_TST_TSEN_BIST_TIME_SEL_CFG1(n) (((n)&3)<<5)
#define TSEN_ADC_TOP_TST_TSEN_BIST_TIME_SEL_CFG2(n) (((n)&3)<<3)
#define TSEN_ADC_TOP_TST_TSEN_BIST_TIME_SEL_CFG3(n) (((n)&3)<<1)
#define TSEN_ADC_TOP_TST_TSEN_BIT_EN (1<<0)

//tst_tsen_c0_test_res0
#define TSEN_ADC_TOP_TST_TSEN_C0_RES0(n) (((n)&0xFFFF)<<0)

//tst_tsen_c0_test_res1
#define TSEN_ADC_TOP_TST_TSEN_C0_RES1(n) (((n)&0xFFFF)<<0)

//tst_tsen_c0_test_res2
#define TSEN_ADC_TOP_TST_TSEN_C0_RES2(n) (((n)&0xFFFF)<<0)

//tst_tsen_c0_test_res3
#define TSEN_ADC_TOP_TST_TSEN_C0_RES3(n) (((n)&0xFFFF)<<0)

//tst_tsen_c1_test_res0
#define TSEN_ADC_TOP_TST_TSEN_C1_RES0(n) (((n)&0xFFFF)<<0)

//tst_tsen_c1_test_res1
#define TSEN_ADC_TOP_TST_TSEN_C1_RES1(n) (((n)&0xFFFF)<<0)

//tst_tsen_c1_test_res2
#define TSEN_ADC_TOP_TST_TSEN_C1_RES2(n) (((n)&0xFFFF)<<0)

//tst_tsen_c1_test_res3
#define TSEN_ADC_TOP_TST_TSEN_C1_RES3(n) (((n)&0xFFFF)<<0)

//tst_tsen_c2_test_res0
#define TSEN_ADC_TOP_TST_TSEN_C2_RES0(n) (((n)&0xFFFF)<<0)

//tst_tsen_c2_test_res1
#define TSEN_ADC_TOP_TST_TSEN_C2_RES1(n) (((n)&0xFFFF)<<0)

//tst_tsen_c2_test_res2
#define TSEN_ADC_TOP_TST_TSEN_C2_RES2(n) (((n)&0xFFFF)<<0)

//tst_tsen_c2_test_res3
#define TSEN_ADC_TOP_TST_TSEN_C2_RES3(n) (((n)&0xFFFF)<<0)

//tst_tsen_c3_test_res0
#define TSEN_ADC_TOP_TST_TSEN_C3_RES0(n) (((n)&0xFFFF)<<0)

//tst_tsen_c3_test_res1
#define TSEN_ADC_TOP_TST_TSEN_C3_RES1(n) (((n)&0xFFFF)<<0)

//tst_tsen_c3_test_res2
#define TSEN_ADC_TOP_TST_TSEN_C3_RES2(n) (((n)&0xFFFF)<<0)

//tst_tsen_c3_test_res3
#define TSEN_ADC_TOP_TST_TSEN_C3_RES3(n) (((n)&0xFFFF)<<0)





#endif


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


#ifndef _WDT_H_
#define _WDT_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'wdt'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// WDT_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_WDT_BASE               0x00025000
#else
#define REG_WDT_BASE               0x40025000
#endif

typedef volatile struct
{
    REG32                          wdt_cvr0;                     //0x00000000
    REG32                          wdt_cvr1;                     //0x00000004
    REG32                          wdt_cr;                       //0x00000008
    REG32                          wdt_cmd;                      //0x0000000C
    REG32                          wdt_icr;                      //0x00000010
    REG32                          wdt_sr;                       //0x00000014
} HWP_WDT_T;

#define hwp_wdt                    ((HWP_WDT_T*) REG_ACCESS_ADDRESS(REG_WDT_BASE))


//wdt_cvr0
typedef union {
    REG32 v;
    struct {
        REG32 count_value_0 : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_WDT_WDT_CVR0_T;

//wdt_cvr1
typedef union {
    REG32 v;
    struct {
        REG32 count_value_1 : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_WDT_WDT_CVR1_T;

//wdt_cr
typedef union {
    REG32 v;
    struct {
        REG32 reset_length : 3; // [2:0]
        REG32 __3_3 : 1;
        REG32 mode : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_WDT_WDT_CR_T;

//wdt_cmd
typedef union {
    REG32 v;
    struct {
        REG32 cmd : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_WDT_WDT_CMD_T;

//wdt_icr
typedef union {
    REG32 v;
    struct {
        REG32 int_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_WDT_WDT_ICR_T;

//wdt_sr
typedef union {
    REG32 v;
    struct {
        REG32 int_assert : 1; // [0], read only
        REG32 wdt_active : 1; // [1], read only
        REG32 __31_2 : 30;
    } b;
} REG_WDT_WDT_SR_T;


//wdt_cvr0
#define WDT_COUNT_VALUE_0(n)       (((n)&0xFFFFFF)<<0)

//wdt_cvr1
#define WDT_COUNT_VALUE_1(n)       (((n)&0xFFFFFF)<<0)

//wdt_cr
#define WDT_MODE                   (1<<4)
#define WDT_RESET_LENGTH(n)        (((n)&7)<<0)

//wdt_cmd
#define WDT_CMD(n)                 (((n)&0xFF)<<0)

//wdt_icr
#define WDT_INT_CLR                (1<<0)

//wdt_sr
#define WDT_WDT_ACTIVE             (1<<1)
#define WDT_INT_ASSERT             (1<<0)





#endif


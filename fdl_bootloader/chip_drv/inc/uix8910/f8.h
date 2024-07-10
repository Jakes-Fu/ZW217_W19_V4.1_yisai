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


#ifndef _F8_H_
#define _F8_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'f8'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// F8_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_F8_BASE                0x09000000

typedef volatile struct
{
    REG32                          F8_CONF;                      //0x00000000
    REG32                          F8_GROUP_ADDR;                //0x00000004
    REG32                          F8_GROUP_CNT;                 //0x00000008
    REG32                          F8_STATUS;                    //0x0000000C
    REG32                          F9_CONF;                      //0x00000010
    REG32                          F9_GROUP_ADDR;                //0x00000014
    REG32                          F9_RESULT;                    //0x00000018
} HWP_F8_T;

#define hwp_f8                     ((HWP_F8_T*) REG_ACCESS_ADDRESS(REG_F8_BASE))


//F8_CONF
typedef union {
    REG32 v;
    struct {
        REG32 f8_start : 1; // [0]
        REG32 f8_irq_en : 1; // [1]
        REG32 f8_ar_sel : 2; // [3:2]
        REG32 __31_4 : 28;
    } b;
} REG_F8_F8_CONF_T;

//F8_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 f8_stat : 1; // [0], write clear
        REG32 f9_stat : 1; // [1], write clear
        REG32 __31_2 : 30;
    } b;
} REG_F8_F8_STATUS_T;

//F9_CONF
typedef union {
    REG32 v;
    struct {
        REG32 f9_start : 1; // [0]
        REG32 f9_irq_en : 1; // [1]
        REG32 f9_ar_sel : 2; // [3:2]
        REG32 __31_4 : 28;
    } b;
} REG_F8_F9_CONF_T;


//F8_CONF
#define F8_RESERVED_0(n)           (((n)&0xFFFFFFF)<<4)
#define F8_F8_AR_SEL(n)            (((n)&3)<<2)
#define F8_F8_IRQ_EN               (1<<1)
#define F8_F8_START                (1<<0)

//F8_GROUP_ADDR
#define F8_GROUP_ADDR(n)           (((n)&0xFFFFFFFF)<<0)

//F8_GROUP_CNT
//#define F8_GROUP_ADDR(n)         (((n)&0xFFFFFFFF)<<0)

//F8_STATUS
#define F8_RESERVED_0_X1(n)        (((n)&0x3FFFFFFF)<<2)
#define F8_F9_STAT                 (1<<1)
#define F8_F8_STAT                 (1<<0)

//F9_CONF
#define F8_RESERVED_0_X2(n)        (((n)&0xFFFFFF)<<8)
#define F8_F9_AR_SEL(n)            (((n)&3)<<2)
#define F8_F9_IRQ_EN               (1<<1)
#define F8_F9_START                (1<<0)
#define F8_RESERVED_1              (1<<4)
#define F8_RESERVED_2              (1<<5)
#define F8_RESERVED_3              (1<<6)
#define F8_RESERVED_4              (1<<7)

//F9_GROUP_ADDR
#define F8_F9_ADDR(n)              (((n)&0xFFFFFFFF)<<0)

//F9_RESULT
#define F8_F9_MAC(n)               (((n)&0xFFFFFFFF)<<0)





#endif


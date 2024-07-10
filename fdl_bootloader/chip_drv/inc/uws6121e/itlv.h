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


#ifndef _ITLV_H_
#define _ITLV_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'itlv'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// ITLV_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_ITLV_BASE              0x00002000
#else
#define REG_ITLV_BASE              0x40002000
#endif

typedef volatile struct
{
    REG32                          command;                      //0x00000000
    REG32                          status;                       //0x00000004
    REG32                          burst_base;                   //0x00000008
    REG32                          frame_base;                   //0x0000000C
    REG32                          int_status;                   //0x00000010
    REG32                          int_clear;                    //0x00000014
} HWP_ITLV_T;

#define hwp_itlv                   ((HWP_ITLV_T*) REG_ACCESS_ADDRESS(REG_ITLV_BASE))


//command
typedef union {
    REG32 v;
    struct {
        REG32 itlv_start : 1; // [0]
        REG32 ditlv_start : 1; // [1]
        REG32 __2_2 : 1;
        REG32 int_mask : 1; // [3]
        REG32 itlv_type : 4; // [7:4]
        REG32 burst_offset : 5; // [12:8]
        REG32 __19_13 : 7;
        REG32 nb_bits : 11; // [30:20]
        REG32 __31_31 : 1;
    } b;
} REG_ITLV_COMMAND_T;

//status
typedef union {
    REG32 v;
    struct {
        REG32 busy : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_ITLV_STATUS_T;

//burst_base
typedef union {
    REG32 v;
    struct {
        REG32 __3_0 : 4;
        REG32 burst_base : 11; // [14:4]
        REG32 __31_15 : 17;
    } b;
} REG_ITLV_BURST_BASE_T;

//frame_base
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 frame_base : 13; // [14:2]
        REG32 __31_15 : 17;
    } b;
} REG_ITLV_FRAME_BASE_T;

//int_status
typedef union {
    REG32 v;
    struct {
        REG32 it_cause : 1; // [0], read only
        REG32 __15_1 : 15;
        REG32 it_status : 1; // [16], read only
        REG32 __31_17 : 15;
    } b;
} REG_ITLV_INT_STATUS_T;

//int_clear
typedef union {
    REG32 v;
    struct {
        REG32 it_clear : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ITLV_INT_CLEAR_T;


//command
#define ITLV_NB_BITS(n)            (((n)&0x7FF)<<20)
#define ITLV_BURST_OFFSET(n)       (((n)&31)<<8)
#define ITLV_ITLV_TYPE(n)          (((n)&15)<<4)
#define ITLV_ITLV_TYPE_MASK        (15<<4)
#define ITLV_ITLV_TYPE_SHIFT       (4)
#define ITLV_ITLV_TYPE_TYPE_1A     (0<<4)
#define ITLV_ITLV_TYPE_TYPE_1B     (1<<4)
#define ITLV_ITLV_TYPE_TYPE_1C     (2<<4)
#define ITLV_ITLV_TYPE_TYPE_2A     (3<<4)
#define ITLV_ITLV_TYPE_TYPE_2B     (4<<4)
#define ITLV_ITLV_TYPE_TYPE_3      (5<<4)
#define ITLV_ITLV_TYPE_V_TYPE_1A   (0)
#define ITLV_ITLV_TYPE_V_TYPE_1B   (1)
#define ITLV_ITLV_TYPE_V_TYPE_1C   (2)
#define ITLV_ITLV_TYPE_V_TYPE_2A   (3)
#define ITLV_ITLV_TYPE_V_TYPE_2B   (4)
#define ITLV_ITLV_TYPE_V_TYPE_3    (5)
#define ITLV_INT_MASK              (1<<3)
#define ITLV_DITLV_START           (1<<1)
#define ITLV_ITLV_START            (1<<0)

//status
#define ITLV_BUSY                  (1<<0)

//burst_base
#define ITLV_BURST_BASE(n)         (((n)&0x7FF)<<4)

//frame_base
#define ITLV_FRAME_BASE(n)         (((n)&0x1FFF)<<2)

//int_status
#define ITLV_IT_STATUS             (1<<16)
#define ITLV_IT_CAUSE              (1<<0)

//int_clear
#define ITLV_IT_CLEAR              (1<<0)





#endif


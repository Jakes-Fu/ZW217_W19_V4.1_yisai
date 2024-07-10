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


#ifndef _EVITAC_H_
#define _EVITAC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'evitac'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================
#define EVITAC_START                            (1)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// EVITAC_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_EVITAC_BASE            0x00007000
#else
#define REG_EVITAC_BASE            0x40007000
#endif

typedef volatile struct
{
    REG32                          ctrl;                         //0x00000000
    REG32                          pbmml_addr;                   //0x00000004
    REG32                          pzfm_addr;                    //0x00000008
    REG32                          psmpl_addr;                   //0x0000000C
    REG32                          output_bits_addr;             //0x00000010
    REG32                          shift_status;                 //0x00000014
    REG32                          zfhist_history;               //0x00000018
} HWP_EVITAC_T;

#define hwp_evitac                 ((HWP_EVITAC_T*) REG_ACCESS_ADDRESS(REG_EVITAC_BASE))


//ctrl
typedef union {
    REG32 v;
    struct {
        REG32 psampl_inc : 8; // [7:0]
        REG32 nb_symbol : 8; // [15:8]
        REG32 nodemetric_his : 8; // [23:16]
        REG32 cmd : 8; // [31:24]
    } b;
} REG_EVITAC_CTRL_T;

//shift_status
typedef union {
    REG32 v;
    struct {
        REG32 status : 8; // [7:0]
        REG32 svshift : 8; // [15:8]
        REG32 bmshift : 8; // [23:16]
        REG32 __31_24 : 8;
    } b;
} REG_EVITAC_SHIFT_STATUS_T;


//ctrl
#define EVITAC_PSAMPL_INC(n)       (((n)&0xFF)<<0)
#define EVITAC_NB_SYMBOL(n)        (((n)&0xFF)<<8)
#define EVITAC_NODEMETRIC_HIS(n)   (((n)&0xFF)<<16)
#define EVITAC_CMD(n)              (((n)&0xFF)<<24)

//pbmml_addr
#define EVITAC_PBMML_ADDR(n)       (((n)&0xFFFFFFFF)<<0)

//pzfm_addr
#define EVITAC_PZFM_ADDR(n)        (((n)&0xFFFFFFFF)<<0)

//psmpl_addr
#define EVITAC_PSMPL_ADDR(n)       (((n)&0xFFFFFFFF)<<0)

//output_bits_addr
#define EVITAC_OUTPUT_BITS_ADDR(n) (((n)&0xFFFFFFFF)<<0)

//shift_status
#define EVITAC_STATUS(n)           (((n)&0xFF)<<0)
#define EVITAC_SVSHIFT(n)          (((n)&0xFF)<<8)
#define EVITAC_BMSHIFT(n)          (((n)&0xFF)<<16)

//zfhist_history
#define EVITAC_ZFHIST_HISTORY(n)   (((n)&0xFFFFFFFF)<<0)





#endif


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


#ifndef _XCOR_H_
#define _XCOR_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'xcor'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================
#define XCOR_SYMBOL_SIZE                        (VITAC_MULT_SIZE)
#define XCOR_SADDER_SIZE                        (XCOR_SYMBOL_SIZE+1 +6)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// XCOR_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_XCOR_BASE              0x00000000
#else
#define REG_XCOR_BASE              0x40000000
#endif

typedef volatile struct
{
    REG32                          command;                      //0x00000000
    REG32                          status;                       //0x00000004
    /// Multipurpose Data Register.
    /// Store Training Sequence in TSXC mode.
    /// Store SUM in DCOC 3rd pass mode.
    /// Store bit sequence in SREC mode.
    /// Store SUM in SPROC mode.
    /// Store I SUM in CHEST mode.
    /// Store R(k-1) in FCHXC mode.
    REG32                          hv0;                          //0x00000008
    /// Multipurpose Data Register.
    /// Store Training Sequence in TSXC mode.
    /// Store bit sequence in SREC mode.
    /// Store Q SUM in CHEST mode.
    REG32                          hv1;                          //0x0000000C
    /// Multipurpose Data Registers.
    /// D0 stores symbols/softvalues/channel taps depending on mode. Not
    /// readable.
    /// D1 stores decoded bits/IQ threshols/IQ Offsets/A terms depending on
    /// mode. Not readable.
    /// D2 (aka A1) serves as Rd address (decoded bits, A or B terms) / Wr
    /// address register (I or packed IQ results, Symbols) / event counter
    /// depending on mode.
    /// D3 (aka A2) serves as Wr address (Q results) / event counter depending
    /// on mode.
    /// D4 stores results from VITAC / extracted HardValues depending on mode.
    /// Not readable.
    /// D5 (aka A3) serves as Wr address (CQ results) Not readable.
    REG32                          data[6];                      //0x00000010
    REG32                          accuI;                        //0x00000028
    REG32                          accuQ;                        //0x0000002C
    /// Address 0 Register.
    /// Stores Rd address for symbols / SoftValues / A terms depending on mode.
    /// Auto increment/decrement/reset.
    REG32                          addr0;                        //0x00000030
    /// Multipurpose Data Edge Registers.
    REG32                          data_e[6];                    //0x00000034
} HWP_XCOR_T;

#define hwp_xcor                   ((HWP_XCOR_T*) REG_ACCESS_ADDRESS(REG_XCOR_BASE))


//command
typedef union {
    REG32 v;
    struct {
        REG32 biterr_en : 1; // [0]
        REG32 dco1p_en : 1; // [1]
        REG32 dco2p_en : 1; // [2]
        REG32 dco3p_en : 1; // [3]
        REG32 tscxc_en : 1; // [4]
        REG32 srec_en : 1; // [5]
        REG32 bext_en : 1; // [6]
        REG32 sproc_en : 1; // [7]
        REG32 chest_en : 1; // [8]
        REG32 fchxc_en : 1; // [9]
        REG32 sldwin_en : 1; // [10]
        REG32 it_mask : 1; // [11]
        REG32 pack_iq : 1; // [12]
        REG32 derotation_en : 1; // [13]
        REG32 __15_14 : 2;
        REG32 nb_iloop : 8; // [23:16]
        REG32 nb_symb : 8; // [31:24]
    } b;
} REG_XCOR_COMMAND_T;

//status
typedef union {
    REG32 v;
    struct {
        REG32 op_pending : 1; // [0], read only
        REG32 __15_1 : 15;
        REG32 it_cause : 1; // [16], read only
        REG32 __30_17 : 14;
        REG32 it_status : 1; // [31]
    } b;
} REG_XCOR_STATUS_T;

//accuI
typedef union {
    REG32 v;
    struct {
        REG32 accui : 21; // [20:0], read only
        REG32 __31_21 : 11;
    } b;
} REG_XCOR_ACCUI_T;

//accuQ
typedef union {
    REG32 v;
    struct {
        REG32 accuq : 21; // [20:0], read only
        REG32 __31_21 : 11;
    } b;
} REG_XCOR_ACCUQ_T;

//addr0
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 addr0 : 13; // [14:2]
        REG32 __31_15 : 17;
    } b;
} REG_XCOR_ADDR0_T;


//command
#define XCOR_BITERR_EN             (1<<0)
#define XCOR_DCO1P_EN              (1<<1)
#define XCOR_DCO2P_EN              (1<<2)
#define XCOR_DCO3P_EN              (1<<3)
#define XCOR_TSCXC_EN              (1<<4)
#define XCOR_SREC_EN               (1<<5)
#define XCOR_BEXT_EN               (1<<6)
#define XCOR_SPROC_EN              (1<<7)
#define XCOR_CHEST_EN              (1<<8)
#define XCOR_FCHXC_EN              (1<<9)
#define XCOR_SLDWIN_EN             (1<<10)
#define XCOR_IT_MASK               (1<<11)
#define XCOR_PACK_IQ               (1<<12)
#define XCOR_DEROTATION_EN         (1<<13)
#define XCOR_NB_ILOOP(n)           (((n)&0xFF)<<16)
#define XCOR_NB_SYMB(n)            (((n)&0xFF)<<24)

//status
#define XCOR_OP_PENDING            (1<<0)
#define XCOR_IT_CAUSE              (1<<16)
#define XCOR_IT_STATUS             (1<<31)

//hv0
#define XCOR_HV0(n)                (((n)&0xFFFFFFFF)<<0)

//hv1
#define XCOR_HV1(n)                (((n)&0xFFFFFFFF)<<0)

//data
#define XCOR_DATA(n)               (((n)&0xFFFFFFFF)<<0)

//accuI
#define XCOR_ACCUI(n)              (((n)&0x1FFFFF)<<0)

//accuQ
#define XCOR_ACCUQ(n)              (((n)&0x1FFFFF)<<0)

//addr0
#define XCOR_ADDR0(n)              (((n)&0x1FFF)<<2)

//data_e
//#define XCOR_DATA(n)             (((n)&0xFFFFFFFF)<<0)





#endif


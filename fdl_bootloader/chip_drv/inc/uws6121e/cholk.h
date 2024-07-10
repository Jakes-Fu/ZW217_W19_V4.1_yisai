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


#ifndef _CHOLK_H_
#define _CHOLK_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'cholk'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// CHOLK_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_CHOLK_BASE             0x00005000
#else
#define REG_CHOLK_BASE             0x40005000
#endif

typedef volatile struct
{
    REG32                          CHOLK_CTRL;                   //0x00000000
    REG32                          GAIN;                         //0x00000004
    REG32                          ITER_THRE;                    //0x00000008
    REG32                          MCOVA_BASE;                   //0x0000000C
    REG32                          MCE_BASE;                     //0x00000010
    REG32                          MCOEF_BASE;                   //0x00000014
    REG32                          NUMBER;                       //0x00000018
    REG32                          CHOLK_STATUS;                 //0x0000001C
    REG32                          eCHOLK_INT;                   //0x00000020
} HWP_CHOLK_T;

#define hwp_cholk                  ((HWP_CHOLK_T*) REG_ACCESS_ADDRESS(REG_CHOLK_BASE))


//CHOLK_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 cholk_on : 1; // [0]
        REG32 cholk_int_mask : 1; // [1]
        REG32 cholk_mode : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_CHOLK_CHOLK_CTRL_T;

//GAIN
typedef union {
    REG32 v;
    struct {
        REG32 resi_gain : 2; // [1:0]
        REG32 resi2_gain : 2; // [3:2]
        REG32 ogrs_gain : 2; // [5:4]
        REG32 oles1_gain : 2; // [7:6]
        REG32 oles2_gain : 2; // [9:8]
        REG32 coef_gain : 2; // [11:10]
        REG32 grad_gain : 2; // [13:12]
        REG32 gops_gain : 2; // [15:14]
        REG32 oles3_gain : 2; // [17:16]
        REG32 __31_18 : 14;
    } b;
} REG_CHOLK_GAIN_T;

//ITER_THRE
typedef union {
    REG32 v;
    struct {
        REG32 iter_thre1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CHOLK_ITER_THRE_T;

//MCOVA_BASE
typedef union {
    REG32 v;
    struct {
        REG32 mcova_base : 13; // [12:0]
        REG32 __31_13 : 19;
    } b;
} REG_CHOLK_MCOVA_BASE_T;

//MCE_BASE
typedef union {
    REG32 v;
    struct {
        REG32 mce_base : 13; // [12:0]
        REG32 __31_13 : 19;
    } b;
} REG_CHOLK_MCE_BASE_T;

//MCOEF_BASE
typedef union {
    REG32 v;
    struct {
        REG32 mcoef_base : 13; // [12:0]
        REG32 __31_13 : 19;
    } b;
} REG_CHOLK_MCOEF_BASE_T;

//NUMBER
typedef union {
    REG32 v;
    struct {
        REG32 row_number : 5; // [4:0]
        REG32 mcova_number : 9; // [13:5]
        REG32 iter_number : 5; // [18:14]
        REG32 __31_19 : 13;
    } b;
} REG_CHOLK_NUMBER_T;

//CHOLK_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 cholk_status : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CHOLK_CHOLK_STATUS_T;

//eCHOLK_INT
typedef union {
    REG32 v;
    struct {
        REG32 echolk_int_raw : 1; // [0]
        REG32 echolk_int_out : 1; // [1], read only
        REG32 __31_2 : 30;
    } b;
} REG_CHOLK_ECHOLK_INT_T;


//CHOLK_CTRL
#define CHOLK_CHOLK_ON             (1<<0)
#define CHOLK_CHOLK_INT_MASK       (1<<1)
#define CHOLK_CHOLK_MODE           (1<<2)

//GAIN
#define CHOLK_RESI_GAIN(n)         (((n)&3)<<0)
#define CHOLK_RESI2_GAIN(n)        (((n)&3)<<2)
#define CHOLK_OGRS_GAIN(n)         (((n)&3)<<4)
#define CHOLK_OLES1_GAIN(n)        (((n)&3)<<6)
#define CHOLK_OLES2_GAIN(n)        (((n)&3)<<8)
#define CHOLK_COEF_GAIN(n)         (((n)&3)<<10)
#define CHOLK_GRAD_GAIN(n)         (((n)&3)<<12)
#define CHOLK_GOPS_GAIN(n)         (((n)&3)<<14)
#define CHOLK_OLES3_GAIN(n)        (((n)&3)<<16)

//ITER_THRE
#define CHOLK_ITER_THRE1(n)        (((n)&0xFFFF)<<0)

//MCOVA_BASE
#define CHOLK_MCOVA_BASE(n)        (((n)&0x1FFF)<<0)

//MCE_BASE
#define CHOLK_MCE_BASE(n)          (((n)&0x1FFF)<<0)

//MCOEF_BASE
#define CHOLK_MCOEF_BASE(n)        (((n)&0x1FFF)<<0)

//NUMBER
#define CHOLK_ROW_NUMBER(n)        (((n)&31)<<0)
#define CHOLK_MCOVA_NUMBER(n)      (((n)&0x1FF)<<5)
#define CHOLK_ITER_NUMBER(n)       (((n)&31)<<14)

//CHOLK_STATUS
#define CHOLK_CHOLK_STATUS         (1<<0)

//eCHOLK_INT
#define CHOLK_ECHOLK_INT_RAW       (1<<0)
#define CHOLK_ECHOLK_INT_OUT       (1<<1)





#endif


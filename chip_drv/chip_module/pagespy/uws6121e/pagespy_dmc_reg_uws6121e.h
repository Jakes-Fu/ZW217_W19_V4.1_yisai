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


#ifndef _PAGESPY_DMC_H_
#define _PAGESPY_DMC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'pagespy_dmc'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
#define PAGESPY_DMC_NB_PAGE                     (8)
#define PAGESPY_DMC_AXI_ID_WIDTH                (9)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// PAGESPY_DMC_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_PAGESPY_DMC_BASE       0x08802000

typedef volatile struct
{
    struct
    {
        REG32                      MON_CTRL;                     //0x00000000
        REG32                      MON_CONF;                     //0x00000004
        REG32                      MON_START_ADDR;               //0x00000008
        REG32                      MON_END_ADDR;                 //0x0000000C
        REG32                      MON_INT_EN;                   //0x00000010
        REG32                      MON_INT_STATUS;               //0x00000014
        REG32                      MON_INT_CAUSE;                //0x00000018
        REG32                      MON_TIMER_TH;                 //0x0000001C
        REG32                      WRITE_BYTES_SR;               //0x00000020
        REG32                      READ_BYTES_SR;                //0x00000024
        REG32                      TIMER_COUNT;                  //0x00000028
        REG32                      MON_NUM_TH;                   //0x0000002C
        REG32                      HIT_INFO;                     //0x00000030
        REG32                      HIT_ADDR;                     //0x00000034
        REG32 Reserved_00000038;                //0x00000038
        REG32                      INT_VEC;                      //0x0000003C
    } page[PAGESPY_DMC_NB_PAGE];
} HWP_PAGESPY_DMC_T;

#define hwp_pagespyDmc             ((HWP_PAGESPY_DMC_T*) REG_ACCESS_ADDRESS(REG_PAGESPY_DMC_BASE))


//MON_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 mon_ctrl_sr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_PAGESPY_DMC_MON_CTRL_T;

//MON_CONF
typedef union {
    REG32 v;
    struct {
        REG32 mon_sysifsel_0 : 1; // [0]
        REG32 mon_sysifsel_1 : 1; // [1]
        REG32 mon_time_en : 1; // [2]
        REG32 mon_long_en : 1; // [3]
        REG32 mon_num_en : 1; // [4]
        REG32 mon_hit_en : 1; // [5]
        REG32 mon_hit_detectw : 1; // [6]
        REG32 mon_hit_detectr : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_PAGESPY_DMC_MON_CONF_T;

//MON_START_ADDR
typedef union {
    REG32 v;
    struct {
        REG32 mon_start_addr : 28; // [27:0]
        REG32 __31_28 : 4;
    } b;
} REG_PAGESPY_DMC_MON_START_ADDR_T;

//MON_END_ADDR
typedef union {
    REG32 v;
    struct {
        REG32 mon_end_addr : 28; // [27:0]
        REG32 __31_28 : 4;
    } b;
} REG_PAGESPY_DMC_MON_END_ADDR_T;

//MON_INT_EN
typedef union {
    REG32 v;
    struct {
        REG32 timer_int_en : 1; // [0]
        REG32 num_int_en : 1; // [1]
        REG32 hit_int_en : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_PAGESPY_DMC_MON_INT_EN_T;

//MON_INT_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 time_int_status : 1; // [0]
        REG32 long_int_status : 1; // [1]
        REG32 write_num_int_status : 1; // [2]
        REG32 read_num_int_status : 1; // [3]
        REG32 write_hit_int_status : 1; // [4]
        REG32 read_hit_int_status : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_PAGESPY_DMC_MON_INT_STATUS_T;

//MON_INT_CAUSE
typedef union {
    REG32 v;
    struct {
        REG32 time_int_cause : 1; // [0], read only
        REG32 long_int_cause : 1; // [1], read only
        REG32 write_num_int_cause : 1; // [2], read only
        REG32 read_num_int_cause : 1; // [3], read only
        REG32 write_hit_int_cause : 1; // [4], read only
        REG32 read_hit_int_cause : 1; // [5], read only
        REG32 __31_6 : 26;
    } b;
} REG_PAGESPY_DMC_MON_INT_CAUSE_T;

//HIT_INFO
typedef union {
    REG32 v;
    struct {
        REG32 hit_aburst_0 : 1; // [0], read only
        REG32 hit_aburst_1 : 1; // [1], read only
        REG32 hit_asize_0 : 1; // [2], read only
        REG32 hit_asize_1 : 1; // [3], read only
        REG32 hit_asize_2 : 1; // [4], read only
        REG32 hit_alen_0 : 1; // [5], read only
        REG32 hit_alen_1 : 1; // [6], read only
        REG32 hit_alen_2 : 1; // [7], read only
        REG32 hit_alen_3 : 1; // [8], read only
        REG32 hit_aid_0 : 1; // [9], read only
        REG32 hit_aid_1 : 1; // [10], read only
        REG32 hit_aid_2 : 1; // [11], read only
        REG32 hit_aid_3 : 1; // [12], read only
        REG32 hit_aid_4 : 1; // [13], read only
        REG32 hit_aid_5 : 1; // [14], read only
        REG32 hit_aid_6 : 1; // [15], read only
        REG32 hit_aid_7 : 1; // [16], read only
        REG32 hit_aid_8 : 1; // [17], read only
        REG32 __31_18 : 14;
    } b;
} REG_PAGESPY_DMC_HIT_INFO_T;

//INT_VEC
typedef union {
    REG32 v;
    struct {
        REG32 int_vec : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_PAGESPY_DMC_INT_VEC_T;


//MON_CTRL
#define PAGESPY_DMC_MON_CTRL_SR    (1<<0)

//MON_CONF
#define PAGESPY_DMC_MON_SYSIFSEL_0 (1<<0)
#define PAGESPY_DMC_MON_SYSIFSEL_1 (1<<1)
#define PAGESPY_DMC_MON_TIME_EN    (1<<2)
#define PAGESPY_DMC_MON_LONG_EN    (1<<3)
#define PAGESPY_DMC_MON_NUM_EN     (1<<4)
#define PAGESPY_DMC_MON_HIT_EN     (1<<5)
#define PAGESPY_DMC_MON_HIT_DETECTW (1<<6)
#define PAGESPY_DMC_MON_HIT_DETECTR (1<<7)
#define PAGESPY_DMC_MON_SYSIFSEL(n) (((n)&3)<<0)
#define PAGESPY_DMC_MON_SYSIFSEL_MASK (3<<0)
#define PAGESPY_DMC_MON_SYSIFSEL_SHIFT (0)
#define PAGESPY_DMC_MON_HIT_MODE(n) (((n)&3)<<6)
#define PAGESPY_DMC_MON_HIT_MODE_MASK (3<<6)
#define PAGESPY_DMC_MON_HIT_MODE_SHIFT (6)

//MON_START_ADDR
#define PAGESPY_DMC_MON_START_ADDR(n) (((n)&0xFFFFFFF)<<0)

//MON_END_ADDR
#define PAGESPY_DMC_MON_END_ADDR(n) (((n)&0xFFFFFFF)<<0)

//MON_INT_EN
#define PAGESPY_DMC_TIMER_INT_EN   (1<<0)
#define PAGESPY_DMC_NUM_INT_EN     (1<<1)
#define PAGESPY_DMC_HIT_INT_EN     (1<<2)

//MON_INT_STATUS
#define PAGESPY_DMC_TIME_INT_STATUS (1<<0)
#define PAGESPY_DMC_LONG_INT_STATUS (1<<1)
#define PAGESPY_DMC_WRITE_NUM_INT_STATUS (1<<2)
#define PAGESPY_DMC_READ_NUM_INT_STATUS (1<<3)
#define PAGESPY_DMC_WRITE_HIT_INT_STATUS (1<<4)
#define PAGESPY_DMC_READ_HIT_INT_STATUS (1<<5)

//MON_INT_CAUSE
#define PAGESPY_DMC_TIME_INT_CAUSE (1<<0)
#define PAGESPY_DMC_LONG_INT_CAUSE (1<<1)
#define PAGESPY_DMC_WRITE_NUM_INT_CAUSE (1<<2)
#define PAGESPY_DMC_READ_NUM_INT_CAUSE (1<<3)
#define PAGESPY_DMC_WRITE_HIT_INT_CAUSE (1<<4)
#define PAGESPY_DMC_READ_HIT_INT_CAUSE (1<<5)

//MON_TIMER_TH
#define PAGESPY_DMC_MON_TIMER_THRESHOLD(n) (((n)&0xFFFFFFFF)<<0)

//WRITE_BYTES_SR
#define PAGESPY_DMC_WRITE_BYTES_SR(n) (((n)&0xFFFFFFFF)<<0)

//READ_BYTES_SR
#define PAGESPY_DMC_READ_BYTES_SR(n) (((n)&0xFFFFFFFF)<<0)

//TIMER_COUNT
#define PAGESPY_DMC_TIMER_COUNT(n) (((n)&0xFFFFFFFF)<<0)

//MON_NUM_TH
#define PAGESPY_DMC_MON_NUM_THRESHOLD(n) (((n)&0xFFFFFFFF)<<0)

//HIT_INFO
#define PAGESPY_DMC_HIT_ABURST_0   (1<<0)
#define PAGESPY_DMC_HIT_ABURST_1   (1<<1)
#define PAGESPY_DMC_HIT_ASIZE_0    (1<<2)
#define PAGESPY_DMC_HIT_ASIZE_1    (1<<3)
#define PAGESPY_DMC_HIT_ASIZE_2    (1<<4)
#define PAGESPY_DMC_HIT_ALEN_0     (1<<5)
#define PAGESPY_DMC_HIT_ALEN_1     (1<<6)
#define PAGESPY_DMC_HIT_ALEN_2     (1<<7)
#define PAGESPY_DMC_HIT_ALEN_3     (1<<8)
#define PAGESPY_DMC_HIT_AID_0      (1<<9)
#define PAGESPY_DMC_HIT_AID_1      (1<<10)
#define PAGESPY_DMC_HIT_AID_2      (1<<11)
#define PAGESPY_DMC_HIT_AID_3      (1<<12)
#define PAGESPY_DMC_HIT_AID_4      (1<<13)
#define PAGESPY_DMC_HIT_AID_5      (1<<14)
#define PAGESPY_DMC_HIT_AID_6      (1<<15)
#define PAGESPY_DMC_HIT_AID_7      (1<<16)
#define PAGESPY_DMC_HIT_AID_8      (1<<17)
#define PAGESPY_DMC_HIT_ABURST(n)  (((n)&3)<<0)
#define PAGESPY_DMC_HIT_ABURST_MASK (3<<0)
#define PAGESPY_DMC_HIT_ABURST_SHIFT (0)
#define PAGESPY_DMC_HIT_ASIZE(n)   (((n)&7)<<2)
#define PAGESPY_DMC_HIT_ASIZE_MASK (7<<2)
#define PAGESPY_DMC_HIT_ASIZE_SHIFT (2)
#define PAGESPY_DMC_HIT_ALEN(n)    (((n)&15)<<5)
#define PAGESPY_DMC_HIT_ALEN_MASK  (15<<5)
#define PAGESPY_DMC_HIT_ALEN_SHIFT (5)
#define PAGESPY_DMC_HIT_AID(n)     (((n)&0x1FF)<<9)
#define PAGESPY_DMC_HIT_AID_MASK   (0x1FF<<9)
#define PAGESPY_DMC_HIT_AID_SHIFT  (9)

//HIT_ADDR
#define PAGESPY_DMC_HIT_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//INT_VEC
#define PAGESPY_DMC_INT_VEC(n)     (((n)&0xFFFF)<<0)





#endif


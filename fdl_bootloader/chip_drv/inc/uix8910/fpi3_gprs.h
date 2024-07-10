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


#ifndef _FPI3_GPRS_H_
#define _FPI3_GPRS_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'fpi3_gprs'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// FPI3_GPRS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_GEA3_BASE              0x09100000

typedef volatile struct
{
    REG32                          GPRS_CLC;                     //0x00000000
    REG32 Reserved_00000004;                    //0x00000004
    REG32                          GPRS_ID;                      //0x00000008
    REG32 Reserved_0000000C;                    //0x0000000C
    REG32                          GPRS_DATA;                    //0x00000010
    REG32                          GPRS_STAT;                    //0x00000014
    REG32                          GPRS_MAC;                     //0x00000018
    REG32 Reserved_0000001C;                    //0x0000001C
    REG32                          GPRS_BCCC;                    //0x00000020
    REG32                          GPRS_POLY;                    //0x00000024
    REG32                          GPRS_FCS;                     //0x00000028
    REG32                          GPRS_FRESH;                   //0x0000002C
    REG32                          GPRS_KC0;                     //0x00000030
    REG32                          GPRS_KC1;                     //0x00000034
    REG32                          GPRS_KC2;                     //0x00000038
    REG32                          GPRS_KC3;                     //0x0000003C
    REG32                          GPRS_INPUT;                   //0x00000040
    REG32                          GPRS_GEA3;                    //0x00000044
    REG32                          GPRS_LENGTH;                  //0x00000048
    REG32                          GPRS_CTRL;                    //0x0000004C
    REG32 Reserved_00000050[42];                //0x00000050
    REG32                          GPRS_SRC1;                    //0x000000F8
    REG32                          GPRS_SRC0;                    //0x000000FC
} HWP_FPI3_GPRS_T;

#define hwp_gea3                   ((HWP_FPI3_GPRS_T*) REG_ACCESS_ADDRESS(REG_GEA3_BASE))


//GPRS_CLC
typedef union {
    REG32 v;
    struct {
        REG32 disr : 1; // [0]
        REG32 diss : 1; // [1], read only
        REG32 spen : 1; // [2]
        REG32 edis : 1; // [3]
        REG32 sbwe : 1; // [4]
        REG32 fsoe : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_FPI3_GPRS_GPRS_CLC_T;

//GPRS_ID
typedef union {
    REG32 v;
    struct {
        REG32 rev_number : 8; // [7:0], read only
        REG32 __15_8 : 8;
        REG32 mod_number : 16; // [31:16], read only
    } b;
} REG_FPI3_GPRS_GPRS_ID_T;

//GPRS_STAT
typedef union {
    REG32 v;
    struct {
        REG32 rd : 6; // [5:0], read only
        REG32 __6_6 : 1;
        REG32 ufl : 1; // [7]
        REG32 wr : 6; // [13:8], read only
        REG32 __14_14 : 1;
        REG32 ofl : 1; // [15]
        REG32 __22_16 : 7;
        REG32 ufl_stat : 1; // [23], read only
        REG32 __30_24 : 7;
        REG32 ofl_stat : 1; // [31], read only
    } b;
} REG_FPI3_GPRS_GPRS_STAT_T;

//GPRS_BCCC
typedef union {
    REG32 v;
    struct {
        REG32 cnt_val : 11; // [10:0]
        REG32 __17_11 : 7;
        REG32 crc_bcctrl : 1; // [18]
        REG32 ciph_bcctrl : 1; // [19]
        REG32 __31_20 : 12;
    } b;
} REG_FPI3_GPRS_GPRS_BCCC_T;

//GPRS_GEA3
typedef union {
    REG32 v;
    struct {
        REG32 ce : 16; // [15:0]
        REG32 ca : 8; // [23:16]
        REG32 __25_24 : 2;
        REG32 cd : 1; // [26]
        REG32 cb : 5; // [31:27]
    } b;
} REG_FPI3_GPRS_GPRS_GEA3_T;

//GPRS_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 direction : 1; // [1]
        REG32 init : 1; // [1]
        REG32 crc_ctrl : 1; // [2]
        REG32 ciph_ctrl : 1; // [3]
        REG32 mode : 1; // [4]
        REG32 min_int : 1; // [5]
        REG32 int_en : 1; // [6]
        REG32 bufout_en : 1; // [7]
        REG32 bufin_en : 1; // [8]
        REG32 bc_en : 1; // [9]
        REG32 burstsize : 3; // [12:10]
        REG32 fifo_flush : 1; // [13]
        REG32 f9cal : 1; // [14]
        REG32 __15_15 : 1;
        REG32 init_stat : 1; // [16], read only
        REG32 __19_17 : 3;
        REG32 gea3_umts : 1; // [20]
        REG32 xor_dis : 1; // [21]
        REG32 int_stat : 1; // [22], read only
        REG32 __23_23 : 1;
        REG32 offset : 5; // [28:24]
        REG32 __29_29 : 1;
        REG32 f9cal_stat : 1; // [30], read only
        REG32 __31_31 : 1;
    } b;
} REG_FPI3_GPRS_GPRS_CTRL_T;

//GPRS_SRC1
typedef union {
    REG32 v;
    struct {
        REG32 __9_0 : 10;
        REG32 tos : 1; // [10]
        REG32 __11_11 : 1;
        REG32 sre : 1; // [12]
        REG32 srr : 1; // [13], read only
        REG32 clrr : 1; // [14]
        REG32 setr : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_FPI3_GPRS_GPRS_SRC1_T;

//GPRS_SRC0
typedef union {
    REG32 v;
    struct {
        REG32 __9_0 : 10;
        REG32 tos : 1; // [10]
        REG32 __11_11 : 1;
        REG32 sre : 1; // [12]
        REG32 srr : 1; // [13], read only
        REG32 clrr : 1; // [14]
        REG32 setr : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_FPI3_GPRS_GPRS_SRC0_T;


//GPRS_CLC
#define FPI3_GPRS_RESERVED_0(n)    (((n)&0x3FFFFFF)<<6)
#define FPI3_GPRS_FSOE             (1<<5)
#define FPI3_GPRS_SBWE             (1<<4)
#define FPI3_GPRS_EDIS             (1<<3)
#define FPI3_GPRS_SPEN             (1<<2)
#define FPI3_GPRS_DISS             (1<<1)
#define FPI3_GPRS_DISR             (1<<0)

//GPRS_ID
#define FPI3_GPRS_MOD_NUMBER(n)    (((n)&0xFFFF)<<16)
#define FPI3_GPRS_RESERVED_0_X1(n) (((n)&0xFF)<<8)
#define FPI3_GPRS_REV_NUMBER(n)    (((n)&0xFF)<<0)

//GPRS_DATA
#define FPI3_GPRS_DATA(n)          (((n)&0xFFFFFFFF)<<0)

//GPRS_STAT
#define FPI3_GPRS_OFL_STAT         (1<<31)
#define FPI3_GPRS_RESERVED_0_X2(n) (((n)&0x7F)<<24)
#define FPI3_GPRS_UFL_STAT         (1<<23)
#define FPI3_GPRS_RESERVED_1(n)    (((n)&0x7F)<<16)
#define FPI3_GPRS_OFL              (1<<15)
#define FPI3_GPRS_RESERVED_2       (1<<14)
#define FPI3_GPRS_WR(n)            (((n)&0x3F)<<8)
#define FPI3_GPRS_UFL              (1<<7)
#define FPI3_GPRS_RESERVED_3       (1<<6)
#define FPI3_GPRS_RD(n)            (((n)&0x3F)<<0)

//GPRS_MAC
#define FPI3_GPRS_MAC_I(n)         (((n)&0xFFFFFFFF)<<0)

//GPRS_BCCC
#define FPI3_GPRS_RESERVED_0_X3(n) (((n)&0xFFF)<<20)
#define FPI3_GPRS_CIPH_BCCTRL      (1<<19)
#define FPI3_GPRS_CRC_BCCTRL       (1<<18)
#define FPI3_GPRS_RESERVED_1_X1(n) (((n)&0x7F)<<11)
#define FPI3_GPRS_CNT_VAL(n)       (((n)&0x7FF)<<0)

//GPRS_POLY
#define FPI3_GPRS_POLYNOMIAL(n)    (((n)&0xFFFFFFFF)<<0)

//GPRS_FCS
#define FPI3_GPRS_FCS(n)           (((n)&0xFFFFFFFF)<<0)

//GPRS_FRESH
#define FPI3_GPRS_FRESH(n)         (((n)&0xFFFFFFFF)<<0)

//GPRS_KC0
#define FPI3_GPRS_KC0(n)           (((n)&0xFFFFFFFF)<<0)

//GPRS_KC1
#define FPI3_GPRS_KC1(n)           (((n)&0xFFFFFFFF)<<0)

//GPRS_KC2
#define FPI3_GPRS_KC2(n)           (((n)&0xFFFFFFFF)<<0)

//GPRS_KC3
#define FPI3_GPRS_KC3(n)           (((n)&0xFFFFFFFF)<<0)

//GPRS_INPUT
#define FPI3_GPRS_INPUT(n)         (((n)&0xFFFFFFFF)<<0)

//GPRS_GEA3
#define FPI3_GPRS_CB(n)            (((n)&31)<<27)
#define FPI3_GPRS_CD               (1<<26)
#define FPI3_GPRS_RESERVED_0_X4(n) (((n)&3)<<24)
#define FPI3_GPRS_CA(n)            (((n)&0xFF)<<16)
#define FPI3_GPRS_CE(n)            (((n)&0xFFFF)<<0)

//GPRS_LENGTH
#define FPI3_GPRS_LENGTH(n)        (((n)&0xFFFFFFFF)<<0)

//GPRS_CTRL
#define FPI3_GPRS_RESERVED_0_X5    (1<<31)
#define FPI3_GPRS_F9CAL_STAT       (1<<30)
#define FPI3_GPRS_RESERVED_1_X2    (1<<29)
#define FPI3_GPRS_OFFSET(n)        (((n)&31)<<24)
#define FPI3_GPRS_RESERVED_2_X1    (1<<23)
#define FPI3_GPRS_INT_STAT         (1<<22)
#define FPI3_GPRS_XOR_DIS          (1<<21)
#define FPI3_GPRS_GEA3_UMTS        (1<<20)
#define FPI3_GPRS_RESERVED_3_X1(n) (((n)&7)<<17)
#define FPI3_GPRS_INIT_STAT        (1<<16)
#define FPI3_GPRS_RESERVED_4       (1<<15)
#define FPI3_GPRS_F9CAL            (1<<14)
#define FPI3_GPRS_FIFO_FLUSH       (1<<13)
#define FPI3_GPRS_BURSTSIZE(n)     (((n)&7)<<10)
#define FPI3_GPRS_BC_EN            (1<<9)
#define FPI3_GPRS_BUFIN_EN         (1<<8)
#define FPI3_GPRS_BUFOUT_EN        (1<<7)
#define FPI3_GPRS_INT_EN           (1<<6)
#define FPI3_GPRS_MIN_INT          (1<<5)
#define FPI3_GPRS_MODE             (1<<4)
#define FPI3_GPRS_CIPH_CTRL        (1<<3)
#define FPI3_GPRS_CRC_CTRL         (1<<2)
#define FPI3_GPRS_DIRECTION        (1<<1)
#define FPI3_GPRS_INIT             (1<<1)
#define FPI3_GPRS_RESERVED_5       (1<<0)

//GPRS_SRC1
#define FPI3_GPRS_RESERVED_0_X6(n) (((n)&0xFFFF)<<16)
#define FPI3_GPRS_SETR             (1<<15)
#define FPI3_GPRS_CLRR             (1<<14)
#define FPI3_GPRS_SRR              (1<<13)
#define FPI3_GPRS_SRE              (1<<12)
#define FPI3_GPRS_RESERVED_1_X3    (1<<11)
#define FPI3_GPRS_TOS              (1<<10)
#define FPI3_GPRS_RESERVED_2_X2(n) (((n)&0x3FF)<<0)

//GPRS_SRC0
//#define FPI3_GPRS_RESERVED_0_X6(n) (((n)&0xFFFF)<<16)
//#define FPI3_GPRS_SETR           (1<<15)
//#define FPI3_GPRS_CLRR           (1<<14)
//#define FPI3_GPRS_SRR            (1<<13)
//#define FPI3_GPRS_SRE            (1<<12)
//#define FPI3_GPRS_RESERVED_1_X3  (1<<11)
//#define FPI3_GPRS_TOS            (1<<10)
//#define FPI3_GPRS_RESERVED_2_X2(n) (((n)&0x3FF)<<0)





#endif


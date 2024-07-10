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


#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'spinlock'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SPINLOCK_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_SPINLOCK_BASE          0x0510D000
#else
#define REG_SPINLOCK_BASE          0x5010D000
#endif

typedef volatile struct
{
    REG32                          chk_id;                       //0x00000000
    REG32                          recctrl;                      //0x00000004
    REG32                          ttlsts;                       //0x00000008
    REG32                          swflag0;                      //0x0000000C
    REG32                          swflag1;                      //0x00000010
    REG32                          swflag2;                      //0x00000014
    REG32                          swflag3;                      //0x00000018
    REG32 Reserved_0000001C[25];                //0x0000001C
    REG32                          mstid0;                       //0x00000080
    REG32                          mstid1;                       //0x00000084
    REG32                          mstid2;                       //0x00000088
    REG32                          mstid3;                       //0x0000008C
    REG32                          mstid4;                       //0x00000090
    REG32                          mstid5;                       //0x00000094
    REG32                          mstid6;                       //0x00000098
    REG32                          mstid7;                       //0x0000009C
    REG32                          mstid8;                       //0x000000A0
    REG32                          mstid9;                       //0x000000A4
    REG32                          mstid10;                      //0x000000A8
    REG32                          mstid11;                      //0x000000AC
    REG32                          mstid12;                      //0x000000B0
    REG32                          mstid13;                      //0x000000B4
    REG32                          mstid14;                      //0x000000B8
    REG32                          mstid15;                      //0x000000BC
    REG32                          mstid16;                      //0x000000C0
    REG32                          mstid17;                      //0x000000C4
    REG32                          mstid18;                      //0x000000C8
    REG32                          mstid19;                      //0x000000CC
    REG32                          mstid20;                      //0x000000D0
    REG32                          mstid21;                      //0x000000D4
    REG32                          mstid22;                      //0x000000D8
    REG32                          mstid23;                      //0x000000DC
    REG32                          mstid24;                      //0x000000E0
    REG32                          mstid25;                      //0x000000E4
    REG32                          mstid26;                      //0x000000E8
    REG32                          mstid27;                      //0x000000EC
    REG32                          mstid28;                      //0x000000F0
    REG32                          mstid29;                      //0x000000F4
    REG32                          mstid30;                      //0x000000F8
    REG32                          mstid31;                      //0x000000FC
    REG32 Reserved_00000100[448];               //0x00000100
    REG32                          locksts0;                     //0x00000800
    REG32                          locksts1;                     //0x00000804
    REG32                          locksts2;                     //0x00000808
    REG32                          locksts3;                     //0x0000080C
    REG32                          locksts4;                     //0x00000810
    REG32                          locksts5;                     //0x00000814
    REG32                          locksts6;                     //0x00000818
    REG32                          locksts7;                     //0x0000081C
    REG32                          locksts8;                     //0x00000820
    REG32                          locksts9;                     //0x00000824
    REG32                          locksts10;                    //0x00000828
    REG32                          locksts11;                    //0x0000082C
    REG32                          locksts12;                    //0x00000830
    REG32                          locksts13;                    //0x00000834
    REG32                          locksts14;                    //0x00000838
    REG32                          locksts15;                    //0x0000083C
    REG32                          locksts16;                    //0x00000840
    REG32                          locksts17;                    //0x00000844
    REG32                          locksts18;                    //0x00000848
    REG32                          locksts19;                    //0x0000084C
    REG32                          locksts20;                    //0x00000850
    REG32                          locksts21;                    //0x00000854
    REG32                          locksts22;                    //0x00000858
    REG32                          locksts23;                    //0x0000085C
    REG32                          locksts24;                    //0x00000860
    REG32                          locksts25;                    //0x00000864
    REG32                          locksts26;                    //0x00000868
    REG32                          locksts27;                    //0x0000086C
    REG32                          locksts28;                    //0x00000870
    REG32                          locksts29;                    //0x00000874
    REG32                          locksts30;                    //0x00000878
    REG32                          locksts31;                    //0x0000087C
    REG32 Reserved_00000880[479];               //0x00000880
    REG32                          verid;                        //0x00000FFC
} HWP_SPINLOCK_T;

#define hwp_spinlock               ((HWP_SPINLOCK_T*) REG_ACCESS_ADDRESS(REG_SPINLOCK_BASE))


//recctrl
typedef union {
    REG32 v;
    struct {
        REG32 rec_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_RECCTRL_T;

//locksts0
typedef union {
    REG32 v;
    struct {
        REG32 locksts0_locksts0 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS0_T;

//locksts1
typedef union {
    REG32 v;
    struct {
        REG32 locksts1_locksts1 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS1_T;

//locksts2
typedef union {
    REG32 v;
    struct {
        REG32 locksts2_locksts2 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS2_T;

//locksts3
typedef union {
    REG32 v;
    struct {
        REG32 locksts3_locksts3 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS3_T;

//locksts4
typedef union {
    REG32 v;
    struct {
        REG32 locksts4_locksts4 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS4_T;

//locksts5
typedef union {
    REG32 v;
    struct {
        REG32 locksts5_locksts5 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS5_T;

//locksts6
typedef union {
    REG32 v;
    struct {
        REG32 locksts6_locksts6 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS6_T;

//locksts7
typedef union {
    REG32 v;
    struct {
        REG32 locksts7_locksts7 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS7_T;

//locksts8
typedef union {
    REG32 v;
    struct {
        REG32 locksts8_locksts8 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS8_T;

//locksts9
typedef union {
    REG32 v;
    struct {
        REG32 locksts9_locksts9 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS9_T;

//locksts10
typedef union {
    REG32 v;
    struct {
        REG32 locksts10_locksts10 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS10_T;

//locksts11
typedef union {
    REG32 v;
    struct {
        REG32 locksts11_locksts11 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS11_T;

//locksts12
typedef union {
    REG32 v;
    struct {
        REG32 locksts12_locksts12 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS12_T;

//locksts13
typedef union {
    REG32 v;
    struct {
        REG32 locksts13_locksts13 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS13_T;

//locksts14
typedef union {
    REG32 v;
    struct {
        REG32 locksts14_locksts14 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS14_T;

//locksts15
typedef union {
    REG32 v;
    struct {
        REG32 locksts15_locksts15 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS15_T;

//locksts16
typedef union {
    REG32 v;
    struct {
        REG32 locksts16_locksts16 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS16_T;

//locksts17
typedef union {
    REG32 v;
    struct {
        REG32 locksts17_locksts17 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS17_T;

//locksts18
typedef union {
    REG32 v;
    struct {
        REG32 locksts18_locksts18 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS18_T;

//locksts19
typedef union {
    REG32 v;
    struct {
        REG32 locksts19_locksts19 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS19_T;

//locksts20
typedef union {
    REG32 v;
    struct {
        REG32 locksts20_locksts20 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS20_T;

//locksts21
typedef union {
    REG32 v;
    struct {
        REG32 locksts21_locksts21 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS21_T;

//locksts22
typedef union {
    REG32 v;
    struct {
        REG32 locksts22_locksts22 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS22_T;

//locksts23
typedef union {
    REG32 v;
    struct {
        REG32 locksts23_locksts23 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS23_T;

//locksts24
typedef union {
    REG32 v;
    struct {
        REG32 locksts24_locksts24 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS24_T;

//locksts25
typedef union {
    REG32 v;
    struct {
        REG32 locksts25_locksts25 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS25_T;

//locksts26
typedef union {
    REG32 v;
    struct {
        REG32 locksts26_locksts26 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS26_T;

//locksts27
typedef union {
    REG32 v;
    struct {
        REG32 locksts27_locksts27 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS27_T;

//locksts28
typedef union {
    REG32 v;
    struct {
        REG32 locksts28_locksts28 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS28_T;

//locksts29
typedef union {
    REG32 v;
    struct {
        REG32 locksts29_locksts29 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS29_T;

//locksts30
typedef union {
    REG32 v;
    struct {
        REG32 locksts30_locksts30 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS30_T;

//locksts31
typedef union {
    REG32 v;
    struct {
        REG32 locksts31_locksts31 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_SPINLOCK_LOCKSTS31_T;


//chk_id
#define SPINLOCK_CHK_ID_CHK_ID(n)  (((n)&0xFFFFFFFF)<<0)

//recctrl
#define SPINLOCK_RESERVED_0(n)     (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_REC_CTRL          (1<<0)

//ttlsts
#define SPINLOCK_LOCK(n)           (((n)&0xFFFFFFFF)<<0)

//swflag0
#define SPINLOCK_SWFLAG0_SWFLAG0(n) (((n)&0xFFFFFFFF)<<0)

//swflag1
#define SPINLOCK_SWFLAG1_SWFLAG1(n) (((n)&0xFFFFFFFF)<<0)

//swflag2
#define SPINLOCK_SWFLAG2_SWFLAG2(n) (((n)&0xFFFFFFFF)<<0)

//swflag3
#define SPINLOCK_SWFLAG3_SWFLAG3(n) (((n)&0xFFFFFFFF)<<0)

//mstid0
#define SPINLOCK_MSTID0_MSTID0(n)  (((n)&0xFFFFFFFF)<<0)

//mstid1
#define SPINLOCK_MSTID1_MSTID1(n)  (((n)&0xFFFFFFFF)<<0)

//mstid2
#define SPINLOCK_MSTID2_MSTID2(n)  (((n)&0xFFFFFFFF)<<0)

//mstid3
#define SPINLOCK_MSTID3_MSTID3(n)  (((n)&0xFFFFFFFF)<<0)

//mstid4
#define SPINLOCK_MSTID4_MSTID4(n)  (((n)&0xFFFFFFFF)<<0)

//mstid5
#define SPINLOCK_MSTID5_MSTID5(n)  (((n)&0xFFFFFFFF)<<0)

//mstid6
#define SPINLOCK_MSTID6_MSTID6(n)  (((n)&0xFFFFFFFF)<<0)

//mstid7
#define SPINLOCK_MSTID7_MSTID7(n)  (((n)&0xFFFFFFFF)<<0)

//mstid8
#define SPINLOCK_MSTID8_MSTID8(n)  (((n)&0xFFFFFFFF)<<0)

//mstid9
#define SPINLOCK_MSTID9_MSTID9(n)  (((n)&0xFFFFFFFF)<<0)

//mstid10
#define SPINLOCK_MSTID10_MSTID10(n) (((n)&0xFFFFFFFF)<<0)

//mstid11
#define SPINLOCK_MSTID11_MSTID11(n) (((n)&0xFFFFFFFF)<<0)

//mstid12
#define SPINLOCK_MSTID12_MSTID12(n) (((n)&0xFFFFFFFF)<<0)

//mstid13
#define SPINLOCK_MSTID13_MSTID13(n) (((n)&0xFFFFFFFF)<<0)

//mstid14
#define SPINLOCK_MSTID14_MSTID14(n) (((n)&0xFFFFFFFF)<<0)

//mstid15
#define SPINLOCK_MSTID15_MSTID15(n) (((n)&0xFFFFFFFF)<<0)

//mstid16
#define SPINLOCK_MSTID16_MSTID16(n) (((n)&0xFFFFFFFF)<<0)

//mstid17
#define SPINLOCK_MSTID17_MSTID17(n) (((n)&0xFFFFFFFF)<<0)

//mstid18
#define SPINLOCK_MSTID18_MSTID18(n) (((n)&0xFFFFFFFF)<<0)

//mstid19
#define SPINLOCK_MSTID19_MSTID19(n) (((n)&0xFFFFFFFF)<<0)

//mstid20
#define SPINLOCK_MSTID20_MSTID20(n) (((n)&0xFFFFFFFF)<<0)

//mstid21
#define SPINLOCK_MSTID21_MSTID21(n) (((n)&0xFFFFFFFF)<<0)

//mstid22
#define SPINLOCK_MSTID22_MSTID22(n) (((n)&0xFFFFFFFF)<<0)

//mstid23
#define SPINLOCK_MSTID23_MSTID23(n) (((n)&0xFFFFFFFF)<<0)

//mstid24
#define SPINLOCK_MSTID24_MSTID24(n) (((n)&0xFFFFFFFF)<<0)

//mstid25
#define SPINLOCK_MSTID25_MSTID25(n) (((n)&0xFFFFFFFF)<<0)

//mstid26
#define SPINLOCK_MSTID26_MSTID26(n) (((n)&0xFFFFFFFF)<<0)

//mstid27
#define SPINLOCK_MSTID27_MSTID27(n) (((n)&0xFFFFFFFF)<<0)

//mstid28
#define SPINLOCK_MSTID28_MSTID28(n) (((n)&0xFFFFFFFF)<<0)

//mstid29
#define SPINLOCK_MSTID29_MSTID29(n) (((n)&0xFFFFFFFF)<<0)

//mstid30
#define SPINLOCK_MSTID30_MSTID30(n) (((n)&0xFFFFFFFF)<<0)

//mstid31
#define SPINLOCK_MSTID31_MSTID31(n) (((n)&0xFFFFFFFF)<<0)

//locksts0
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS0_LOCKSTS0 (1<<0)

//locksts1
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS1_LOCKSTS1 (1<<0)

//locksts2
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS2_LOCKSTS2 (1<<0)

//locksts3
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS3_LOCKSTS3 (1<<0)

//locksts4
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS4_LOCKSTS4 (1<<0)

//locksts5
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS5_LOCKSTS5 (1<<0)

//locksts6
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS6_LOCKSTS6 (1<<0)

//locksts7
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS7_LOCKSTS7 (1<<0)

//locksts8
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS8_LOCKSTS8 (1<<0)

//locksts9
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS9_LOCKSTS9 (1<<0)

//locksts10
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS10_LOCKSTS10 (1<<0)

//locksts11
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS11_LOCKSTS11 (1<<0)

//locksts12
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS12_LOCKSTS12 (1<<0)

//locksts13
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS13_LOCKSTS13 (1<<0)

//locksts14
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS14_LOCKSTS14 (1<<0)

//locksts15
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS15_LOCKSTS15 (1<<0)

//locksts16
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS16_LOCKSTS16 (1<<0)

//locksts17
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS17_LOCKSTS17 (1<<0)

//locksts18
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS18_LOCKSTS18 (1<<0)

//locksts19
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS19_LOCKSTS19 (1<<0)

//locksts20
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS20_LOCKSTS20 (1<<0)

//locksts21
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS21_LOCKSTS21 (1<<0)

//locksts22
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS22_LOCKSTS22 (1<<0)

//locksts23
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS23_LOCKSTS23 (1<<0)

//locksts24
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS24_LOCKSTS24 (1<<0)

//locksts25
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS25_LOCKSTS25 (1<<0)

//locksts26
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS26_LOCKSTS26 (1<<0)

//locksts27
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS27_LOCKSTS27 (1<<0)

//locksts28
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS28_LOCKSTS28 (1<<0)

//locksts29
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS29_LOCKSTS29 (1<<0)

//locksts30
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS30_LOCKSTS30 (1<<0)

//locksts31
//#define SPINLOCK_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define SPINLOCK_LOCKSTS31_LOCKSTS31 (1<<0)

//verid
#define SPINLOCK_VERID_VERID(n)    (((n)&0xFFFFFFFF)<<0)





#endif


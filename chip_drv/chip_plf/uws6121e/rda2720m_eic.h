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


#ifndef _RDA2720M_EIC_H_
#define _RDA2720M_EIC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'rda2720m_eic'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// RDA2720M_EIC_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_RDA2720M_EIC_BASE      0x500

typedef volatile struct
{
    REG32                          EICDATA;                      //0x00000000
    REG32                          EICDMSK;                      //0x00000004
    REG32                          RESERVED1;                    //0x00000008
    REG32                          RESERVED2;                    //0x0000000C
    REG32                          RESERVED3;                    //0x00000010
    REG32                          EICIEV;                       //0x00000014
    REG32                          EICIE;                        //0x00000018
    REG32                          EICRIS;                       //0x0000001C
    REG32                          EICMIS;                       //0x00000020
    REG32                          EICIC;                        //0x00000024
    REG32                          EICTRIG;                      //0x00000028
    REG32                          RESERVED4;                    //0x0000002C
    REG32                          RESERVED5;                    //0x00000030
    REG32                          RESERVED6;                    //0x00000034
    REG32                          RESERVED7;                    //0x00000038
    REG32                          RESERVED8;                    //0x0000003C
    REG32                          EIC0CTRL;                     //0x00000040
    REG32                          EIC1CTRL;                     //0x00000044
    REG32                          EIC2CTRL;                     //0x00000048
    REG32                          EIC3CTRL;                     //0x0000004C
    REG32                          EIC4CTRL;                     //0x00000050
    REG32                          EIC5CTRL;                     //0x00000054
    REG32                          EIC6CTRL;                     //0x00000058
    REG32                          EIC7CTRL;                     //0x0000005C
    REG32                          EIC8CTRL;                     //0x00000060
    REG32                          EIC9CTRL;                     //0x00000064
    REG32                          EIC10CTRL;                    //0x00000068
    REG32                          EIC11CTRL;                    //0x0000006C
    REG32                          EIC12CTRL;                    //0x00000070
    REG32                          EIC13CTRL;                    //0x00000074
    REG32                          EIC14CTRL;                    //0x00000078
    REG32                          EIC15CTRL;                    //0x0000007C
} HWP_RDA2720M_EIC_T;

#define hwp_rda2720mEic            ((HWP_RDA2720M_EIC_T*) REG_ACCESS_ADDRESS(REG_RDA2720M_EIC_BASE))


//EICDATA
typedef union {
    REG32 v;
    struct {
        REG32 eicdata : 13; // [12:0], read only
        REG32 __31_13 : 19;
    } b;
} REG_RDA2720M_EIC_EICDATA_T;

//EICDMSK
typedef union {
    REG32 v;
    struct {
        REG32 eicdmsk : 13; // [12:0]
        REG32 __31_13 : 19;
    } b;
} REG_RDA2720M_EIC_EICDMSK_T;

//EICIEV
typedef union {
    REG32 v;
    struct {
        REG32 eiciev : 13; // [12:0]
        REG32 __31_13 : 19;
    } b;
} REG_RDA2720M_EIC_EICIEV_T;

//EICIE
typedef union {
    REG32 v;
    struct {
        REG32 eicie : 13; // [12:0]
        REG32 __31_13 : 19;
    } b;
} REG_RDA2720M_EIC_EICIE_T;

//EICRIS
typedef union {
    REG32 v;
    struct {
        REG32 eicris : 13; // [12:0], read only
        REG32 __31_13 : 19;
    } b;
} REG_RDA2720M_EIC_EICRIS_T;

//EICMIS
typedef union {
    REG32 v;
    struct {
        REG32 eicmis : 13; // [12:0], read only
        REG32 __31_13 : 19;
    } b;
} REG_RDA2720M_EIC_EICMIS_T;

//EICIC
typedef union {
    REG32 v;
    struct {
        REG32 eicic : 13; // [12:0]
        REG32 __31_13 : 19;
    } b;
} REG_RDA2720M_EIC_EICIC_T;

//EICTRIG
typedef union {
    REG32 v;
    struct {
        REG32 eictrig : 13; // [12:0]
        REG32 __31_13 : 19;
    } b;
} REG_RDA2720M_EIC_EICTRIG_T;

//EIC0CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC0CTRL_T;

//EIC1CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC1CTRL_T;

//EIC2CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC2CTRL_T;

//EIC3CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC3CTRL_T;

//EIC4CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC4CTRL_T;

//EIC5CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC5CTRL_T;

//EIC6CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC6CTRL_T;

//EIC7CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC7CTRL_T;

//EIC8CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC8CTRL_T;

//EIC9CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC9CTRL_T;

//EIC10CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC10CTRL_T;

//EIC11CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC11CTRL_T;

//EIC12CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dbnc_cnt : 12; // [11:0]
        REG32 __13_12 : 2;
        REG32 dbnc_en : 1; // [14]
        REG32 force_clk_dbnc : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_EIC_EIC12CTRL_T;


//EICDATA
#define RDA2720M_EIC_EICDATA_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define RDA2720M_EIC_EICDATA(n)    (((n)&0x1FFF)<<0)

//EICDMSK
#define RDA2720M_EIC_EICDMSK_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define RDA2720M_EIC_EICDMSK(n)    (((n)&0x1FFF)<<0)

//RESERVED1
#define RDA2720M_EIC_RESERVED1_RESERVED_0(n) (((n)&0xFFFFFFFF)<<0)

//RESERVED2
#define RDA2720M_EIC_RESERVED2_RESERVED_0(n) (((n)&0xFFFFFFFF)<<0)

//RESERVED3
#define RDA2720M_EIC_RESERVED3_RESERVED_0(n) (((n)&0xFFFFFFFF)<<0)

//EICIEV
#define RDA2720M_EIC_EICIEV_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define RDA2720M_EIC_EICIEV(n)     (((n)&0x1FFF)<<0)

//EICIE
#define RDA2720M_EIC_EICIE_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define RDA2720M_EIC_EICIE(n)      (((n)&0x1FFF)<<0)

//EICRIS
#define RDA2720M_EIC_EICRIS_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define RDA2720M_EIC_EICRIS(n)     (((n)&0x1FFF)<<0)

//EICMIS
#define RDA2720M_EIC_EICMIS_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define RDA2720M_EIC_EICMIS(n)     (((n)&0x1FFF)<<0)

//EICIC
#define RDA2720M_EIC_EICIC_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define RDA2720M_EIC_EICIC(n)      (((n)&0x1FFF)<<0)

//EICTRIG
#define RDA2720M_EIC_EICTRIG_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define RDA2720M_EIC_EICTRIG(n)    (((n)&0x1FFF)<<0)

//RESERVED4
#define RDA2720M_EIC_RESERVED4_RESERVED_0(n) (((n)&0xFFFFFFFF)<<0)

//RESERVED5
#define RDA2720M_EIC_RESERVED5_RESERVED_0(n) (((n)&0xFFFFFFFF)<<0)

//RESERVED6
#define RDA2720M_EIC_RESERVED6_RESERVED_0(n) (((n)&0xFFFFFFFF)<<0)

//RESERVED7
#define RDA2720M_EIC_RESERVED7_RESERVED_0(n) (((n)&0xFFFFFFFF)<<0)

//RESERVED8
#define RDA2720M_EIC_RESERVED8_RESERVED_0(n) (((n)&0xFFFFFFFF)<<0)

//EIC0CTRL
#define RDA2720M_EIC_EIC0CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
#define RDA2720M_EIC_DBNC_EN       (1<<14)
#define RDA2720M_EIC_EIC0CTRL_RESERVED_1(n) (((n)&3)<<12)
#define RDA2720M_EIC_DBNC_CNT(n)   (((n)&0xFFF)<<0)

//EIC1CTRL
#define RDA2720M_EIC_EIC1CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC1CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)

//EIC2CTRL
#define RDA2720M_EIC_EIC2CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC2CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)

//EIC3CTRL
#define RDA2720M_EIC_EIC3CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC3CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)

//EIC4CTRL
#define RDA2720M_EIC_EIC4CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC4CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)

//EIC5CTRL
#define RDA2720M_EIC_EIC5CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC5CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)

//EIC6CTRL
#define RDA2720M_EIC_EIC6CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC6CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)

//EIC7CTRL
#define RDA2720M_EIC_EIC7CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC7CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)

//EIC8CTRL
#define RDA2720M_EIC_EIC8CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC8CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)

//EIC9CTRL
#define RDA2720M_EIC_EIC9CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC9CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)

//EIC10CTRL
#define RDA2720M_EIC_EIC10CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC10CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)

//EIC11CTRL
#define RDA2720M_EIC_EIC11CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC11CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)

//EIC12CTRL
#define RDA2720M_EIC_EIC12CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define RDA2720M_EIC_FORCE_CLK_DBNC (1<<15)
//#define RDA2720M_EIC_DBNC_EN     (1<<14)
#define RDA2720M_EIC_EIC12CTRL_RESERVED_1(n) (((n)&3)<<12)
//#define RDA2720M_EIC_DBNC_CNT(n) (((n)&0xFFF)<<0)





#endif


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


#ifndef _INT_REG_UWS6121E_H_
#define _INT_REG_UWS6121E_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'gic400_reg'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// GIC400_REG_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_GIC400_REG_BASE        0x00800000

typedef volatile struct
{
    REG32 Reserved_00000000[1024];              //0x00000000
    REG32                          GICD_CTRL;                    //0x00001000
    REG32                          GICD_TYPER;                   //0x00001004
    REG32                          GICD_IDDR;                    //0x00001008
    REG32 Reserved_0000100C[29];                //0x0000100C
    REG32                          GICD_IGROUPRn[4];             //0x00001080
    REG32 Reserved_00001090[28];                //0x00001090
    REG32                          GICD_ISENABLER0;              //0x00001100
    REG32                          GICD_ISENABLER1;              //0x00001104
    REG32                          GICD_ISENABLER2;              //0x00001108
    REG32                          GICD_ISENABLER3;              //0x0000110C
    REG32 Reserved_00001110[28];                //0x00001110
    REG32                          GICD_ICENABLER0;              //0x00001180
    REG32                          GICD_ICENABLER1;              //0x00001184
    REG32                          GICD_ICENABLER2;              //0x00001188
    REG32                          GICD_ICENABLER3;              //0x0000118C
    REG32 Reserved_00001190[28];                //0x00001190
    REG32                          GICD_ISPENDRn[4];             //0x00001200
    REG32 Reserved_00001210[28];                //0x00001210
    REG32                          GICD_ICPENDRn[4];             //0x00001280
    REG32 Reserved_00001290[28];                //0x00001290
    REG32                          GICD_ISACTIVERn[4];           //0x00001300
    REG32 Reserved_00001310[28];                //0x00001310
    REG32                          GICD_ICACTIVERn[4];           //0x00001380
    REG32 Reserved_00001390[28];                //0x00001390
    REG32                          GICD_IPRIORITYRn[32];         //0x00001400
    REG32 Reserved_00001480[224];               //0x00001480
    REG32                          GICD_ITARGETSR0;              //0x00001800
    REG32                          GICD_ITARGETSR1;              //0x00001804
    REG32                          GICD_ITARGETSR2;              //0x00001808
    REG32                          GICD_ITARGETSR3;              //0x0000180C
    REG32                          GICD_ITARGETSR4;              //0x00001810
    REG32                          GICD_ITARGETSR5;              //0x00001814
    REG32                          GICD_ITARGETSR6;              //0x00001818
    REG32                          GICD_ITARGETSR7;              //0x0000181C
    REG32                          GICD_ITARGETSR8;              //0x00001820
    REG32                          GICD_ITARGETSR9;              //0x00001824
    REG32                          GICD_ITARGETSR10;             //0x00001828
    REG32                          GICD_ITARGETSR11;             //0x0000182C
    REG32                          GICD_ITARGETSR12;             //0x00001830
    REG32                          GICD_ITARGETSR13;             //0x00001834
    REG32                          GICD_ITARGETSR14;             //0x00001838
    REG32                          GICD_ITARGETSR15;             //0x0000183C
    REG32                          GICD_ITARGETSR16;             //0x00001840
    REG32                          GICD_ITARGETSR17;             //0x00001844
    REG32                          GICD_ITARGETSR18;             //0x00001848
    REG32                          GICD_ITARGETSR19;             //0x0000184C
    REG32                          GICD_ITARGETSR20;             //0x00001850
    REG32                          GICD_ITARGETSR21;             //0x00001854
    REG32                          GICD_ITARGETSR22;             //0x00001858
    REG32                          GICD_ITARGETSR23;             //0x0000185C
    REG32                          GICD_ITARGETSR24;             //0x00001860
    REG32                          GICD_ITARGETSR25;             //0x00001864
    REG32                          GICD_ITARGETSR26;             //0x00001868
    REG32                          GICD_ITARGETSR27;             //0x0000186C
    REG32                          GICD_ITARGETSR28;             //0x00001870
    REG32                          GICD_ITARGETSR29;             //0x00001874
    REG32                          GICD_ITARGETSR30;             //0x00001878
    REG32                          GICD_ITARGETSR31;             //0x0000187C
    REG32 Reserved_00001880[224];               //0x00001880
    REG32                          GICD_ICFGR0;                  //0x00001C00
    REG32                          GICD_ICFGR1;                  //0x00001C04
    REG32                          GICD_ICFGR2;                  //0x00001C08
    REG32                          GICD_ICFGR3;                  //0x00001C0C
    REG32                          GICD_ICFGR4;                  //0x00001C10
    REG32                          GICD_ICFGR5;                  //0x00001C14
    REG32                          GICD_ICFGR6;                  //0x00001C18
    REG32                          GICD_ICFGR7;                  //0x00001C1C
    REG32 Reserved_00001C20[56];                //0x00001C20
    REG32                          GICD_PPISR;                   //0x00001D00
    REG32                          GICD_SPISRn[3];               //0x00001D04
    REG32 Reserved_00001D10[60];                //0x00001D10
    REG32                          GICD_NSACRn[8];               //0x00001E00
    REG32 Reserved_00001E20[56];                //0x00001E20
    REG32                          GICD_SGIR;                    //0x00001F00
    REG32 Reserved_00001F04[3];                 //0x00001F04
    REG32                          GICD_CPENDSGIRn[4];           //0x00001F10
    REG32                          GICD_SPENDSGIRn[4];           //0x00001F20
    REG32 Reserved_00001F30[52];                //0x00001F30
    REG32                          GICC_CTRL;                    //0x00002000
    REG32                          GICC_PMR;                     //0x00002004
    REG32                          GICC_BPR;                     //0x00002008
    REG32                          GICC_IAR;                     //0x0000200C
    REG32                          GICC_EOIR;                    //0x00002010
    REG32                          GICC_RPR;                     //0x00002014
    REG32                          GICC_HPPIR;                   //0x00002018
    REG32                          GICC_ABPR;                    //0x0000201C
    REG32                          GICC_AIAR;                    //0x00002020
    REG32                          GICC_AEOIR;                   //0x00002024
    REG32                          GICC_AHPPIR;                  //0x00002028
    REG32 Reserved_0000202C[41];                //0x0000202C
    REG32                          GICC_APRn;                    //0x000020D0
    REG32 Reserved_000020D4[3];                 //0x000020D4
    REG32                          GICC_NSAPRn;                  //0x000020E0
    REG32 Reserved_000020E4[6];                 //0x000020E4
    REG32                          GICC_IIDR;                    //0x000020FC
    REG32 Reserved_00002100[960];               //0x00002100
    REG32                          GICC_DIR;                     //0x00003000
} HWP_GIC400_REG_T;

#define hwp_gic400Reg              ((HWP_GIC400_REG_T*) REG_ACCESS_ADDRESS(REG_GIC400_REG_BASE))


//GICD_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 enablegrp0 : 1; // [0]
        REG32 enablegrp1 : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_GIC400_REG_GICD_CTRL_T;

//GICD_TYPER
typedef union {
    REG32 v;
    struct {
        REG32 itlinesnumber : 5; // [4:0], read only
        REG32 cpunumber : 3; // [7:5], read only
        REG32 __9_8 : 2;
        REG32 securityextn : 1; // [10], read only
        REG32 lspi : 5; // [15:11], read only
        REG32 __31_16 : 16;
    } b;
} REG_GIC400_REG_GICD_TYPER_T;

//GICD_IDDR
typedef union {
    REG32 v;
    struct {
        REG32 implementer : 12; // [11:0], read only
        REG32 revision : 4; // [15:12], read only
        REG32 variant : 4; // [19:16], read only
        REG32 __23_20 : 4;
        REG32 productid : 8; // [31:24], read only
    } b;
} REG_GIC400_REG_GICD_IDDR_T;

//GICD_PPISR
typedef union {
    REG32 v;
    struct {
        REG32 __8_0 : 9;
        REG32 ppi_status : 7; // [15:9], read only
        REG32 __31_16 : 16;
    } b;
} REG_GIC400_REG_GICD_PPISR_T;

//GICD_SGIR
typedef union {
    REG32 v;
    struct {
        REG32 sgiintid : 4; // [3:0]
        REG32 __14_4 : 11;
        REG32 nsatt : 1; // [15]
        REG32 cputargetlist : 8; // [23:16]
        REG32 targetlistfilter : 2; // [25:24]
        REG32 __31_26 : 6;
    } b;
} REG_GIC400_REG_GICD_SGIR_T;

//GICC_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 enablegrp0 : 1; // [0]
        REG32 enablegrp1 : 1; // [1]
        REG32 ackctl : 1; // [2]
        REG32 fiqen : 1; // [3]
        REG32 cbpr : 1; // [4]
        REG32 fiqbypdisgrp0 : 1; // [5]
        REG32 irqbypdisgrp0 : 1; // [6]
        REG32 fiqbypdisgrp1 : 1; // [7]
        REG32 irqbypdisgrp1 : 1; // [8]
        REG32 eoimodes : 1; // [9]
        REG32 eoimodens : 1; // [10]
        REG32 __31_11 : 21;
    } b;
} REG_GIC400_REG_GICC_CTRL_T;

//GICC_PMR
typedef union {
    REG32 v;
    struct {
        REG32 priority : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_GIC400_REG_GICC_PMR_T;

//GICC_BPR
typedef union {
    REG32 v;
    struct {
        REG32 binary_point : 3; // [2:0]
        REG32 __31_3 : 29;
    } b;
} REG_GIC400_REG_GICC_BPR_T;

//GICC_IAR
typedef union {
    REG32 v;
    struct {
        REG32 interrupt_id : 10; // [9:0], read only
        REG32 cpuid : 3; // [12:10], read only
        REG32 __31_13 : 19;
    } b;
} REG_GIC400_REG_GICC_IAR_T;

//GICC_EOIR
typedef union {
    REG32 v;
    struct {
        REG32 eoiintid : 10; // [9:0]
        REG32 cpuid : 3; // [12:10]
        REG32 __31_13 : 19;
    } b;
} REG_GIC400_REG_GICC_EOIR_T;

//GICC_RPR
typedef union {
    REG32 v;
    struct {
        REG32 priority : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_GIC400_REG_GICC_RPR_T;

//GICC_HPPIR
typedef union {
    REG32 v;
    struct {
        REG32 pendintid : 10; // [9:0], read only
        REG32 cpuid : 3; // [12:10], read only
        REG32 __31_13 : 19;
    } b;
} REG_GIC400_REG_GICC_HPPIR_T;

//GICC_ABPR
typedef union {
    REG32 v;
    struct {
        REG32 binary_point : 3; // [2:0]
        REG32 __31_3 : 29;
    } b;
} REG_GIC400_REG_GICC_ABPR_T;

//GICC_AIAR
typedef union {
    REG32 v;
    struct {
        REG32 interrupt_id : 10; // [9:0], read only
        REG32 cpuid : 3; // [12:10], read only
        REG32 __31_13 : 19;
    } b;
} REG_GIC400_REG_GICC_AIAR_T;

//GICC_AEOIR
typedef union {
    REG32 v;
    struct {
        REG32 interrupt_id : 10; // [9:0]
        REG32 cpuid : 3; // [12:10]
        REG32 __31_13 : 19;
    } b;
} REG_GIC400_REG_GICC_AEOIR_T;

//GICC_AHPPIR
typedef union {
    REG32 v;
    struct {
        REG32 pendintid : 10; // [9:0], read only
        REG32 cpuid : 3; // [12:10], read only
        REG32 __31_13 : 19;
    } b;
} REG_GIC400_REG_GICC_AHPPIR_T;

//GICC_IIDR
typedef union {
    REG32 v;
    struct {
        REG32 implementer : 12; // [11:0], read only
        REG32 revision : 4; // [15:12], read only
        REG32 architecture_version : 4; // [19:16], read only
        REG32 __23_20 : 4;
        REG32 productid : 8; // [31:24], read only
    } b;
} REG_GIC400_REG_GICC_IIDR_T;

//GICC_DIR
typedef union {
    REG32 v;
    struct {
        REG32 interrupt_id : 10; // [9:0]
        REG32 cpuid : 3; // [12:10]
        REG32 __31_13 : 19;
    } b;
} REG_GIC400_REG_GICC_DIR_T;


//GICD_CTRL
#define GIC400_REG_GICD_CTRL_RESERVED_0(n) (((n)&0x3FFFFFFF)<<2)
#define GIC400_REG_GICD_CTRL_RESERVED_0_MASK (0x3FFFFFFF<<2)
#define GIC400_REG_GICD_CTRL_RESERVED_0_SHIFT (2)
#define GIC400_REG_ENABLEGRP1      (1<<1)
#define GIC400_REG_ENABLEGRP1_MASK (1<<1)
#define GIC400_REG_ENABLEGRP1_SHIFT (1)
#define GIC400_REG_ENABLEGRP0      (1<<0)
#define GIC400_REG_ENABLEGRP0_MASK (1<<0)
#define GIC400_REG_ENABLEGRP0_SHIFT (0)

//GICD_TYPER
#define GIC400_REG_GICD_TYPER_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define GIC400_REG_GICD_TYPER_RESERVED_0_MASK (0xFFFF<<16)
#define GIC400_REG_GICD_TYPER_RESERVED_0_SHIFT (16)
#define GIC400_REG_LSPI(n)         (((n)&31)<<11)
#define GIC400_REG_LSPI_MASK       (31<<11)
#define GIC400_REG_LSPI_SHIFT      (11)
#define GIC400_REG_SECURITYEXTN    (1<<10)
#define GIC400_REG_SECURITYEXTN_MASK (1<<10)
#define GIC400_REG_SECURITYEXTN_SHIFT (10)
#define GIC400_REG_GICD_TYPER_RESERVED_1(n) (((n)&3)<<8)
#define GIC400_REG_GICD_TYPER_RESERVED_1_MASK (3<<8)
#define GIC400_REG_GICD_TYPER_RESERVED_1_SHIFT (8)
#define GIC400_REG_CPUNUMBER(n)    (((n)&7)<<5)
#define GIC400_REG_CPUNUMBER_MASK  (7<<5)
#define GIC400_REG_CPUNUMBER_SHIFT (5)
#define GIC400_REG_ITLINESNUMBER(n) (((n)&31)<<0)
#define GIC400_REG_ITLINESNUMBER_MASK (31<<0)
#define GIC400_REG_ITLINESNUMBER_SHIFT (0)

//GICD_IDDR
#define GIC400_REG_PRODUCTID(n)    (((n)&0xFF)<<24)
#define GIC400_REG_PRODUCTID_MASK  (0xFF<<24)
#define GIC400_REG_PRODUCTID_SHIFT (24)
#define GIC400_REG_GICD_IDDR_RESERVED_0(n) (((n)&15)<<20)
#define GIC400_REG_GICD_IDDR_RESERVED_0_MASK (15<<20)
#define GIC400_REG_GICD_IDDR_RESERVED_0_SHIFT (20)
#define GIC400_REG_VARIANT(n)      (((n)&15)<<16)
#define GIC400_REG_VARIANT_MASK    (15<<16)
#define GIC400_REG_VARIANT_SHIFT   (16)
#define GIC400_REG_REVISION(n)     (((n)&15)<<12)
#define GIC400_REG_REVISION_MASK   (15<<12)
#define GIC400_REG_REVISION_SHIFT  (12)
#define GIC400_REG_IMPLEMENTER(n)  (((n)&0xFFF)<<0)
#define GIC400_REG_IMPLEMENTER_MASK (0xFFF<<0)
#define GIC400_REG_IMPLEMENTER_SHIFT (0)

//GICD_IGROUPRn
#define GIC400_REG_GROUP_STATUS_BITS(n) (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_GROUP_STATUS_BITS_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_GROUP_STATUS_BITS_SHIFT (0)

//GICD_ISENABLER0
#define GIC400_REG_SET_ENABLE_BITS(n) (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_SET_ENABLE_BITS_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_SET_ENABLE_BITS_SHIFT (0)

//GICD_ISENABLER1
//#define GIC400_REG_SET_ENABLE_BITS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_SET_ENABLE_BITS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_SET_ENABLE_BITS_SHIFT (0)

//GICD_ISENABLER2
//#define GIC400_REG_SET_ENABLE_BITS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_SET_ENABLE_BITS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_SET_ENABLE_BITS_SHIFT (0)

//GICD_ISENABLER3
//#define GIC400_REG_SET_ENABLE_BITS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_SET_ENABLE_BITS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_SET_ENABLE_BITS_SHIFT (0)

//GICD_ICENABLER0
#define GIC400_REG_CLEAR_ENABLE_BITS(n) (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_CLEAR_ENABLE_BITS_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_CLEAR_ENABLE_BITS_SHIFT (0)

//GICD_ICENABLER1
//#define GIC400_REG_CLEAR_ENABLE_BITS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CLEAR_ENABLE_BITS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CLEAR_ENABLE_BITS_SHIFT (0)

//GICD_ICENABLER2
//#define GIC400_REG_CLEAR_ENABLE_BITS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CLEAR_ENABLE_BITS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CLEAR_ENABLE_BITS_SHIFT (0)

//GICD_ICENABLER3
//#define GIC400_REG_CLEAR_ENABLE_BITS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CLEAR_ENABLE_BITS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CLEAR_ENABLE_BITS_SHIFT (0)

//GICD_ISPENDRn
#define GIC400_REG_SET_PENDING_BITS(n) (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_SET_PENDING_BITS_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_SET_PENDING_BITS_SHIFT (0)

//GICD_ICPENDRn
#define GIC400_REG_CLEAR_PENDING_BITS(n) (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_CLEAR_PENDING_BITS_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_CLEAR_PENDING_BITS_SHIFT (0)

//GICD_ISACTIVERn
#define GIC400_REG_SET_ACTIVE_BITS(n) (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_SET_ACTIVE_BITS_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_SET_ACTIVE_BITS_SHIFT (0)

//GICD_ICACTIVERn
#define GIC400_REG_CLEAR_ACTIVE_BITS(n) (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_CLEAR_ACTIVE_BITS_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_CLEAR_ACTIVE_BITS_SHIFT (0)

//GICD_IPRIORITYRn
#define GIC400_REG_PRIORITY(n)     (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_PRIORITY_MASK   (0xFFFFFFFF<<0)
#define GIC400_REG_PRIORITY_SHIFT  (0)

//GICD_ITARGETSR0
#define GIC400_REG_CPU_TARGETS(n)  (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR1
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR2
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR3
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR4
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR5
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR6
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR7
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR8
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR9
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR10
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR11
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR12
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR13
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR14
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR15
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR16
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR17
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR18
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR19
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR20
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR21
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR22
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR23
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR24
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR25
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR26
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR27
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR28
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR29
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR30
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ITARGETSR31
//#define GIC400_REG_CPU_TARGETS(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_CPU_TARGETS_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_CPU_TARGETS_SHIFT (0)

//GICD_ICFGR0
#define GIC400_REG_INT_CONFIG(n)   (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_INT_CONFIG_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_INT_CONFIG_SHIFT (0)

//GICD_ICFGR1
//#define GIC400_REG_INT_CONFIG(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_INT_CONFIG_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_INT_CONFIG_SHIFT (0)

//GICD_ICFGR2
//#define GIC400_REG_INT_CONFIG(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_INT_CONFIG_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_INT_CONFIG_SHIFT (0)

//GICD_ICFGR3
//#define GIC400_REG_INT_CONFIG(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_INT_CONFIG_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_INT_CONFIG_SHIFT (0)

//GICD_ICFGR4
//#define GIC400_REG_INT_CONFIG(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_INT_CONFIG_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_INT_CONFIG_SHIFT (0)

//GICD_ICFGR5
//#define GIC400_REG_INT_CONFIG(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_INT_CONFIG_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_INT_CONFIG_SHIFT (0)

//GICD_ICFGR6
//#define GIC400_REG_INT_CONFIG(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_INT_CONFIG_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_INT_CONFIG_SHIFT (0)

//GICD_ICFGR7
//#define GIC400_REG_INT_CONFIG(n) (((n)&0xFFFFFFFF)<<0)
//#define GIC400_REG_INT_CONFIG_MASK (0xFFFFFFFF<<0)
//#define GIC400_REG_INT_CONFIG_SHIFT (0)

//GICD_PPISR
#define GIC400_REG_GICD_PPISR_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define GIC400_REG_GICD_PPISR_RESERVED_0_MASK (0xFFFF<<16)
#define GIC400_REG_GICD_PPISR_RESERVED_0_SHIFT (16)
#define GIC400_REG_PPI_STATUS(n)   (((n)&0x7F)<<9)
#define GIC400_REG_PPI_STATUS_MASK (0x7F<<9)
#define GIC400_REG_PPI_STATUS_SHIFT (9)
#define GIC400_REG_GICD_PPISR_RESERVED_1(n) (((n)&0x1FF)<<0)
#define GIC400_REG_GICD_PPISR_RESERVED_1_MASK (0x1FF<<0)
#define GIC400_REG_GICD_PPISR_RESERVED_1_SHIFT (0)

//GICD_SPISRn
#define GIC400_REG_SPIS_STATUS(n)  (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_SPIS_STATUS_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_SPIS_STATUS_SHIFT (0)

//GICD_NSACRn
#define GIC400_REG_NS_ACCESS(n)    (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_NS_ACCESS_MASK  (0xFFFFFFFF<<0)
#define GIC400_REG_NS_ACCESS_SHIFT (0)

//GICD_SGIR
#define GIC400_REG_GICD_SGIR_RESERVED_0(n) (((n)&0x3F)<<26)
#define GIC400_REG_GICD_SGIR_RESERVED_0_MASK (0x3F<<26)
#define GIC400_REG_GICD_SGIR_RESERVED_0_SHIFT (26)
#define GIC400_REG_TARGETLISTFILTER(n) (((n)&3)<<24)
#define GIC400_REG_TARGETLISTFILTER_MASK (3<<24)
#define GIC400_REG_TARGETLISTFILTER_SHIFT (24)
#define GIC400_REG_CPUTARGETLIST(n) (((n)&0xFF)<<16)
#define GIC400_REG_CPUTARGETLIST_MASK (0xFF<<16)
#define GIC400_REG_CPUTARGETLIST_SHIFT (16)
#define GIC400_REG_NSATT           (1<<15)
#define GIC400_REG_NSATT_MASK      (1<<15)
#define GIC400_REG_NSATT_SHIFT     (15)
#define GIC400_REG_GICD_SGIR_RESERVED_1(n) (((n)&0x7FF)<<4)
#define GIC400_REG_GICD_SGIR_RESERVED_1_MASK (0x7FF<<4)
#define GIC400_REG_GICD_SGIR_RESERVED_1_SHIFT (4)
#define GIC400_REG_SGIINTID(n)     (((n)&15)<<0)
#define GIC400_REG_SGIINTID_MASK   (15<<0)
#define GIC400_REG_SGIINTID_SHIFT  (0)

//GICD_CPENDSGIRn
#define GIC400_REG_SGI_CLEAR_PENDING(n) (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_SGI_CLEAR_PENDING_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_SGI_CLEAR_PENDING_SHIFT (0)

//GICD_SPENDSGIRn
#define GIC400_REG_SGI_SET_PENDING(n) (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_SGI_SET_PENDING_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_SGI_SET_PENDING_SHIFT (0)

//GICC_CTRL
#define GIC400_REG_GICC_CTRL_RESERVED_0(n) (((n)&0x1FFFFF)<<11)
#define GIC400_REG_GICC_CTRL_RESERVED_0_MASK (0x1FFFFF<<11)
#define GIC400_REG_GICC_CTRL_RESERVED_0_SHIFT (11)
#define GIC400_REG_EOIMODENS       (1<<10)
#define GIC400_REG_EOIMODENS_MASK  (1<<10)
#define GIC400_REG_EOIMODENS_SHIFT (10)
#define GIC400_REG_EOIMODES        (1<<9)
#define GIC400_REG_EOIMODES_MASK   (1<<9)
#define GIC400_REG_EOIMODES_SHIFT  (9)
#define GIC400_REG_IRQBYPDISGRP1   (1<<8)
#define GIC400_REG_IRQBYPDISGRP1_MASK (1<<8)
#define GIC400_REG_IRQBYPDISGRP1_SHIFT (8)
#define GIC400_REG_FIQBYPDISGRP1   (1<<7)
#define GIC400_REG_FIQBYPDISGRP1_MASK (1<<7)
#define GIC400_REG_FIQBYPDISGRP1_SHIFT (7)
#define GIC400_REG_IRQBYPDISGRP0   (1<<6)
#define GIC400_REG_IRQBYPDISGRP0_MASK (1<<6)
#define GIC400_REG_IRQBYPDISGRP0_SHIFT (6)
#define GIC400_REG_FIQBYPDISGRP0   (1<<5)
#define GIC400_REG_FIQBYPDISGRP0_MASK (1<<5)
#define GIC400_REG_FIQBYPDISGRP0_SHIFT (5)
#define GIC400_REG_CBPR            (1<<4)
#define GIC400_REG_CBPR_MASK       (1<<4)
#define GIC400_REG_CBPR_SHIFT      (4)
#define GIC400_REG_FIQEN           (1<<3)
#define GIC400_REG_FIQEN_MASK      (1<<3)
#define GIC400_REG_FIQEN_SHIFT     (3)
#define GIC400_REG_ACKCTL          (1<<2)
#define GIC400_REG_ACKCTL_MASK     (1<<2)
#define GIC400_REG_ACKCTL_SHIFT    (2)
//#define GIC400_REG_ENABLEGRP1    (1<<1)
//#define GIC400_REG_ENABLEGRP1_MASK (1<<1)
//#define GIC400_REG_ENABLEGRP1_SHIFT (1)
//#define GIC400_REG_ENABLEGRP0    (1<<0)
//#define GIC400_REG_ENABLEGRP0_MASK (1<<0)
//#define GIC400_REG_ENABLEGRP0_SHIFT (0)

//GICC_PMR
#define GIC400_REG_GICC_PMR_RESERVED_0(n) (((n)&0xFFFFFF)<<8)
#define GIC400_REG_GICC_PMR_RESERVED_0_MASK (0xFFFFFF<<8)
#define GIC400_REG_GICC_PMR_RESERVED_0_SHIFT (8)
#define GIC400_REG_PRIORITY_X1(n)  (((n)&0xFF)<<0)
#define GIC400_REG_PRIORITY_MASK_X1 (0xFF<<0)
//#define GIC400_REG_PRIORITY_SHIFT (0)

//GICC_BPR
#define GIC400_REG_GICC_BPR_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define GIC400_REG_GICC_BPR_RESERVED_0_MASK (0x1FFFFFFF<<3)
#define GIC400_REG_GICC_BPR_RESERVED_0_SHIFT (3)
#define GIC400_REG_BINARY_POINT(n) (((n)&7)<<0)
#define GIC400_REG_BINARY_POINT_MASK (7<<0)
#define GIC400_REG_BINARY_POINT_SHIFT (0)

//GICC_IAR
#define GIC400_REG_GICC_IAR_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define GIC400_REG_GICC_IAR_RESERVED_0_MASK (0x7FFFF<<13)
#define GIC400_REG_GICC_IAR_RESERVED_0_SHIFT (13)
#define GIC400_REG_CPUID(n)        (((n)&7)<<10)
#define GIC400_REG_CPUID_MASK      (7<<10)
#define GIC400_REG_CPUID_SHIFT     (10)
#define GIC400_REG_INTERRUPT_ID(n) (((n)&0x3FF)<<0)
#define GIC400_REG_INTERRUPT_ID_MASK (0x3FF<<0)
#define GIC400_REG_INTERRUPT_ID_SHIFT (0)

//GICC_EOIR
#define GIC400_REG_GICC_EOIR_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define GIC400_REG_GICC_EOIR_RESERVED_0_MASK (0x7FFFF<<13)
#define GIC400_REG_GICC_EOIR_RESERVED_0_SHIFT (13)
//#define GIC400_REG_CPUID(n)      (((n)&7)<<10)
//#define GIC400_REG_CPUID_MASK    (7<<10)
//#define GIC400_REG_CPUID_SHIFT   (10)
#define GIC400_REG_EOIINTID(n)     (((n)&0x3FF)<<0)
#define GIC400_REG_EOIINTID_MASK   (0x3FF<<0)
#define GIC400_REG_EOIINTID_SHIFT  (0)

//GICC_RPR
#define GIC400_REG_GICC_RPR_RESERVED_0(n) (((n)&0xFFFFFF)<<8)
#define GIC400_REG_GICC_RPR_RESERVED_0_MASK (0xFFFFFF<<8)
#define GIC400_REG_GICC_RPR_RESERVED_0_SHIFT (8)
//#define GIC400_REG_PRIORITY_X1(n) (((n)&0xFF)<<0)
//#define GIC400_REG_PRIORITY_MASK_X1 (0xFF<<0)
//#define GIC400_REG_PRIORITY_SHIFT (0)

//GICC_HPPIR
#define GIC400_REG_GICC_HPPIR_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define GIC400_REG_GICC_HPPIR_RESERVED_0_MASK (0x7FFFF<<13)
#define GIC400_REG_GICC_HPPIR_RESERVED_0_SHIFT (13)
//#define GIC400_REG_CPUID(n)      (((n)&7)<<10)
//#define GIC400_REG_CPUID_MASK    (7<<10)
//#define GIC400_REG_CPUID_SHIFT   (10)
#define GIC400_REG_PENDINTID(n)    (((n)&0x3FF)<<0)
#define GIC400_REG_PENDINTID_MASK  (0x3FF<<0)
#define GIC400_REG_PENDINTID_SHIFT (0)

//GICC_ABPR
#define GIC400_REG_GICC_ABPR_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define GIC400_REG_GICC_ABPR_RESERVED_0_MASK (0x1FFFFFFF<<3)
#define GIC400_REG_GICC_ABPR_RESERVED_0_SHIFT (3)
//#define GIC400_REG_BINARY_POINT(n) (((n)&7)<<0)
//#define GIC400_REG_BINARY_POINT_MASK (7<<0)
//#define GIC400_REG_BINARY_POINT_SHIFT (0)

//GICC_AIAR
#define GIC400_REG_GICC_AIAR_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define GIC400_REG_GICC_AIAR_RESERVED_0_MASK (0x7FFFF<<13)
#define GIC400_REG_GICC_AIAR_RESERVED_0_SHIFT (13)
//#define GIC400_REG_CPUID(n)      (((n)&7)<<10)
//#define GIC400_REG_CPUID_MASK    (7<<10)
//#define GIC400_REG_CPUID_SHIFT   (10)
//#define GIC400_REG_INTERRUPT_ID(n) (((n)&0x3FF)<<0)
//#define GIC400_REG_INTERRUPT_ID_MASK (0x3FF<<0)
//#define GIC400_REG_INTERRUPT_ID_SHIFT (0)

//GICC_AEOIR
#define GIC400_REG_GICC_AEOIR_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define GIC400_REG_GICC_AEOIR_RESERVED_0_MASK (0x7FFFF<<13)
#define GIC400_REG_GICC_AEOIR_RESERVED_0_SHIFT (13)
//#define GIC400_REG_CPUID(n)      (((n)&7)<<10)
//#define GIC400_REG_CPUID_MASK    (7<<10)
//#define GIC400_REG_CPUID_SHIFT   (10)
//#define GIC400_REG_INTERRUPT_ID(n) (((n)&0x3FF)<<0)
//#define GIC400_REG_INTERRUPT_ID_MASK (0x3FF<<0)
//#define GIC400_REG_INTERRUPT_ID_SHIFT (0)

//GICC_AHPPIR
#define GIC400_REG_GICC_AHPPIR_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define GIC400_REG_GICC_AHPPIR_RESERVED_0_MASK (0x7FFFF<<13)
#define GIC400_REG_GICC_AHPPIR_RESERVED_0_SHIFT (13)
//#define GIC400_REG_CPUID(n)      (((n)&7)<<10)
//#define GIC400_REG_CPUID_MASK    (7<<10)
//#define GIC400_REG_CPUID_SHIFT   (10)
//#define GIC400_REG_PENDINTID(n)  (((n)&0x3FF)<<0)
//#define GIC400_REG_PENDINTID_MASK (0x3FF<<0)
//#define GIC400_REG_PENDINTID_SHIFT (0)

//GICC_APRn
#define GIC400_REG_ACTIVE_PRIORITY(n) (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_ACTIVE_PRIORITY_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_ACTIVE_PRIORITY_SHIFT (0)

//GICC_NSAPRn
#define GIC400_REG_NS_ACTIVE_PRIORITY(n) (((n)&0xFFFFFFFF)<<0)
#define GIC400_REG_NS_ACTIVE_PRIORITY_MASK (0xFFFFFFFF<<0)
#define GIC400_REG_NS_ACTIVE_PRIORITY_SHIFT (0)

//GICC_IIDR
//#define GIC400_REG_PRODUCTID(n)  (((n)&0xFF)<<24)
//#define GIC400_REG_PRODUCTID_MASK (0xFF<<24)
//#define GIC400_REG_PRODUCTID_SHIFT (24)
#define GIC400_REG_GICC_IIDR_RESERVED_0(n) (((n)&15)<<20)
#define GIC400_REG_GICC_IIDR_RESERVED_0_MASK (15<<20)
#define GIC400_REG_GICC_IIDR_RESERVED_0_SHIFT (20)
#define GIC400_REG_ARCHITECTURE_VERSION(n) (((n)&15)<<16)
#define GIC400_REG_ARCHITECTURE_VERSION_MASK (15<<16)
#define GIC400_REG_ARCHITECTURE_VERSION_SHIFT (16)
//#define GIC400_REG_REVISION(n)   (((n)&15)<<12)
//#define GIC400_REG_REVISION_MASK (15<<12)
//#define GIC400_REG_REVISION_SHIFT (12)
//#define GIC400_REG_IMPLEMENTER(n) (((n)&0xFFF)<<0)
//#define GIC400_REG_IMPLEMENTER_MASK (0xFFF<<0)
//#define GIC400_REG_IMPLEMENTER_SHIFT (0)

//GICC_DIR
#define GIC400_REG_GICC_DIR_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define GIC400_REG_GICC_DIR_RESERVED_0_MASK (0x7FFFF<<13)
#define GIC400_REG_GICC_DIR_RESERVED_0_SHIFT (13)
//#define GIC400_REG_CPUID(n)      (((n)&7)<<10)
//#define GIC400_REG_CPUID_MASK    (7<<10)
//#define GIC400_REG_CPUID_SHIFT   (10)
//#define GIC400_REG_INTERRUPT_ID(n) (((n)&0x3FF)<<0)
//#define GIC400_REG_INTERRUPT_ID_MASK (0x3FF<<0)
//#define GIC400_REG_INTERRUPT_ID_SHIFT (0)





#endif


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


#ifndef _SYS_AXI_CFG_H_
#define _SYS_AXI_CFG_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'sys_axi_cfg'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SYS_AXI_CFG_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_SYS_AXI_CFG_BASE       0x08300000

typedef volatile struct
{
    struct
    {
        REG32                      remap;                        //0x00000000
        REG32 Reserved_00000004;                //0x00000004
        REG32                      security[64];                 //0x00000008
        REG32 Reserved_00000108[958];           //0x00000108
    } Address_Region;
    struct
    {
        REG32 Reserved_00000000[1012];          //0x00000000
        REG32                      Peripheral_ID4;               //0x00001FD0
        REG32                      Peripheral_ID5;               //0x00001FD4
        REG32                      Peripheral_ID6;               //0x00001FD8
        REG32                      Peripheral_ID7;               //0x00001FDC
        REG32                      Peripheral_ID0;               //0x00001FE0
        REG32                      Peripheral_ID1;               //0x00001FE4
        REG32                      Peripheral_ID2;               //0x00001FE8
        REG32                      Peripheral_ID3;               //0x00001FEC
        REG32                      Component_ID0;                //0x00001FF0
        REG32                      Component_ID1;                //0x00001FF4
        REG32                      Component_ID2;                //0x00001FF8
        REG32                      Component_ID3;                //0x00001FFC
    } Periperal_ID;
} HWP_SYS_AXI_CFG_T;

#define hwp_sysAxiCfg              ((HWP_SYS_AXI_CFG_T*) REG_ACCESS_ADDRESS(REG_SYS_AXI_CFG_BASE))


//security
typedef union {
    REG32 v;
    struct {
        REG32 secure : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_SYS_AXI_CFG_SECURITY_T;

//Peripheral_ID0
typedef union {
    REG32 v;
    struct {
        REG32 part_number_7_0_ : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_SYS_AXI_CFG_PERIPHERAL_ID0_T;

//Peripheral_ID1
typedef union {
    REG32 v;
    struct {
        REG32 part_number_11_8_ : 4; // [3:0], read only
        REG32 jep106_3_0_ : 4; // [7:4], read only
        REG32 __31_8 : 24;
    } b;
} REG_SYS_AXI_CFG_PERIPHERAL_ID1_T;

//Peripheral_ID2
typedef union {
    REG32 v;
    struct {
        REG32 jep106_6_4_ : 4; // [3:0], read only
        REG32 __31_4 : 28;
    } b;
} REG_SYS_AXI_CFG_PERIPHERAL_ID2_T;


//security
#define SYS_AXI_CFG_SECURE(n)      (((n)&0xFFFF)<<0)

//Peripheral_ID0
#define SYS_AXI_CFG_PART_NUMBER_7_0_(n) (((n)&0xFF)<<0)

//Peripheral_ID1
#define SYS_AXI_CFG_PART_NUMBER_11_8_(n) (((n)&15)<<0)
#define SYS_AXI_CFG_JEP106_3_0_(n) (((n)&15)<<4)

//Peripheral_ID2
#define SYS_AXI_CFG_JEP106_6_4_(n) (((n)&15)<<0)





#endif


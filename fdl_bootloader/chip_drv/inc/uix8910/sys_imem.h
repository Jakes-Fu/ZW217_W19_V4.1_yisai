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


#ifndef _SYS_IMEM_H_
#define _SYS_IMEM_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'sys_imem'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
#define NB_ROM_PATCH                            (8)
#define INT_ROM_SIZE                            (64*1024)
#define INT_SRAM_SIZE                           (64*1024)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SYS_IMEM_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_SYS_IMEM_BASE          0x08804000

typedef volatile struct
{
    struct
    {
        REG32                      patch_pagespy_start;          //0x00000000
        REG32                      patch_pagespy_end;            //0x00000004
    } patch_pagespy_ctrl[NB_ROM_PATCH/2];
    struct
    {
        REG32                      pagespy_info0;                //0x00000020
        REG32                      pagespy_info1;                //0x00000024
    } pagespy_info[NB_ROM_PATCH/2];
} HWP_SYS_IMEM_T;

#define hwp_sysImem                ((HWP_SYS_IMEM_T*) REG_ACCESS_ADDRESS(REG_SYS_IMEM_BASE))


//patch_pagespy_start
typedef union {
    REG32 v;
    struct {
        REG32 patch_addr_pagespy_start_addr : 28; // [27:0]
        REG32 pagespy_detectw : 1; // [28]
        REG32 pagespy_detectr : 1; // [29]
        REG32 pagespy_enable : 1; // [30]
        REG32 patch_valid : 1; // [31]
    } b;
} REG_SYS_IMEM_PATCH_PAGESPY_START_T;

//patch_pagespy_end
typedef union {
    REG32 v;
    struct {
        REG32 patch_addr_pagespy_end_addr : 28; // [27:0]
        REG32 __30_28 : 3;
        REG32 patch_valid : 1; // [31]
    } b;
} REG_SYS_IMEM_PATCH_PAGESPY_END_T;

//pagespy_info0
typedef union {
    REG32 v;
    struct {
        REG32 pagespy_aid : 9; // [8:0], read only
        REG32 pagespy_hitw : 1; // [9], read only
        REG32 pagespy_hitr : 1; // [10], read only
        REG32 pagespy_status : 1; // [11], read only
        REG32 __31_12 : 20;
    } b;
} REG_SYS_IMEM_PAGESPY_INFO0_T;


//patch_pagespy_start
#define SYS_IMEM_PATCH_VALID       (1<<31)
#define SYS_IMEM_PAGESPY_ENABLE    (1<<30)
#define SYS_IMEM_PAGESPY_DETECTR   (1<<29)
#define SYS_IMEM_PAGESPY_DETECTW   (1<<28)
#define SYS_IMEM_PATCH_ADDR_PAGESPY_START_ADDR(n) (((n)&0xFFFFFFF)<<0)

//patch_pagespy_end
//#define SYS_IMEM_PATCH_VALID     (1<<31)
#define SYS_IMEM_PATCH_ADDR_PAGESPY_END_ADDR(n) (((n)&0xFFFFFFF)<<0)

//pagespy_info0
#define SYS_IMEM_PAGESPY_STATUS    (1<<11)
#define SYS_IMEM_PAGESPY_HITR      (1<<10)
#define SYS_IMEM_PAGESPY_HITW      (1<<9)
#define SYS_IMEM_PAGESPY_AID(n)    (((n)&0x1FF)<<0)

//pagespy_info1
#define SYS_IMEM_PAGESPY_ADDR(n)   (((n)&0xFFFFFFFF)<<0)



// ============================================================================
// INT_ROM_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_INT_ROM_BASE           0x00000000

typedef volatile struct
{
    UINT8 boot_rom[65536];                      //0x00000000
} HWP_INT_ROM_T;

#define hwp_intRom                 ((HWP_INT_ROM_T*) REG_ACCESS_ADDRESS(REG_INT_ROM_BASE))





// ============================================================================
// INT_SRAM_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_INT_SRAM_BASE          0x00800000

typedef volatile struct
{
    UINT8 int_sram[65536];                      //0x00000000
} HWP_INT_SRAM_T;

#define hwp_intSram                ((HWP_INT_SRAM_T*) REG_ACCESS_ADDRESS(REG_INT_SRAM_BASE))







#endif


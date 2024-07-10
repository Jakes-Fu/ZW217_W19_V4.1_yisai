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


#ifndef _LPDDR_PSRAM_H_
#define _LPDDR_PSRAM_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'lpddr_psram'."
#endif

#include "global_macros.h"

// =============================================================================
//  MACROS
// =============================================================================
#define LPDDR_MEM_SIZE                          (0X04000000)
#define PSRAM_MEM_SIZE                          (0X01000000)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// LPDDR_MEM_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_LPDDR_MEM_BASE         0x08000000
#else
#define REG_LPDDR_MEM_BASE         0x80000000
#endif

typedef volatile struct
{
    UINT8 lpddr_mem[67108864];                  //0x00000000
} HWP_LPDDR_MEM_T;

#define hwp_lpddrMem               ((HWP_LPDDR_MEM_T*) REG_ACCESS_ADDRESS(REG_LPDDR_MEM_BASE))





// ============================================================================
// PSRAM_MEM_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_PSRAM_MEM_BASE         0x08000000
#else
#define REG_PSRAM_MEM_BASE         0x80000000
#endif

typedef volatile struct
{
    UINT8 psram_mem[16777216];                  //0x00000000
} HWP_PSRAM_MEM_T;

#define hwp_psramMem               ((HWP_PSRAM_MEM_T*) REG_ACCESS_ADDRESS(REG_PSRAM_MEM_BASE))







#endif


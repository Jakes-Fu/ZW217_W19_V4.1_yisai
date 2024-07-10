/******************************************************************************
 ** File Name:    mmu_vm_config.h                                                       *
 ** Author:                                                                   *
 ** DATE:         8/16/2010                                                   *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 08/16/2010    Steve.Zhan      Create.                                     *
 *****************************************************************************/
#ifndef _MMU_VM_CONFIG_H_
#define _MMU_VM_CONFIG_H_

#if 0
#define DP_USE_COARSE_PTE
#define DP_USE_SMALL_PAGE
#else
#define DP_USE_FINE_PTE
#define DP_USE_TINY_PAGE
#endif


#define DP_USE_FIFO   0

/* hash of virtual space */
#if 0
/* 256 slots */
#define HASH_TABLE_SHIFT        (8)
#else
/* 512 slots */
#define HASH_TABLE_SHIFT        (9)
#endif


#define PAGE_TABLE_SIZE         (0x00020000)     /* 128KB DP Partition Max-size is 28M */

#endif //_MMU_VM_CONFIG_H_




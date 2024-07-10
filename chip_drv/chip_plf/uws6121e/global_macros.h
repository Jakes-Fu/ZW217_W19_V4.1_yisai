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


#ifndef _GLOBAL_MACROS_H_
#define _GLOBAL_MACROS_H_



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================




#ifndef WIN32
#ifdef MIPSGCC
#define KSEG0(addr)     ( (addr) | 0x80000000 )
#define KSEG1(addr)     ( (addr) | 0xA0000000 )
#else
#define KSEG0(addr)     (addr)
#define KSEG1(addr)     (addr)
#endif
#define REG_ACCESS_ADDRESS(addr)    KSEG1(addr)
#else
extern void* KSEG0(int addr);
extern void* KSEG1(int addr);
#endif


/* Define access cached or uncached */
#define MEM_ACCESS_CACHED(addr)     ((UINT32*)((UINT32)(addr)&0xdfffffff))
#define MEM_ACCESS_UNCACHED(addr)   ((UINT32*)((UINT32)(addr)|0x20000000))

/* Register access for assembly */
#ifdef MIPSGCC
#define BASE_HI(val) (((0xa0000000 | val) & 0xffff8000) + (val & 0x8000))
#define BASE_LO(val) (((val) & 0x7fff) - (val & 0x8000))
#else
#define BASE_HI(val) (((val) & 0xffff8000) | (val & 0x8000))
#define BASE_LO(val) (((val) & 0x7fff) - (val & 0x8000))
#endif

/* to extract bitfield from register value */
#define GET_BITFIELD(dword, bitfield) (((dword) & (bitfield ## _MASK)) >> (bitfield ## _SHIFT))

#define EXP2(n) (1<<(n))
//#define DUMP_DATA
//#define DUMP_FCCH_DATA

#define readl( _reg_ )		(*(volatile unsigned int *)(_reg_))
#define writel( _val_, _reg_ )	(*((volatile unsigned int *)(_reg_)) = (_val_))

#define ARRAY_SIZE(x)		(sizeof(x) / sizeof((x)[0]))

#endif


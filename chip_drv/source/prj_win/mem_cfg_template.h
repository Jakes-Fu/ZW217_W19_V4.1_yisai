/******************************************************************************
 ** File Name:      mem_cfg_typical.c                                         *
 ** Author:         Lin.liu                                                   *
 ** DATE:           2004-04-19                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the memory config of typical memory     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/19/2004     Lin.liu          Create.                                   *
 ******************************************************************************/
#ifndef _MEM_CFG_TEMPLATE_H_
#define _MEM_CFG_TEMPLATE_H_
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "arm_reg.h"
//#include "sc6600b_reg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

// Flash Config

#ifdef FLASH_AMD_128M
const uint32  FLASH_SectorNum       = 95;       // From 0x80_0000 ==> 0xDE_FFFF
const uint32  FLASH_FileSystemNum   = 80;
const uint32  FLASH_SectorSize      = 0x10000;  // 64K
const uint32  FLASH_StartAddrr      = 0x0;
const uint32  FLASH_BankAddr        = 0x0800000; // the bank managed by EFS
const uint32  FLASH_Size            = 0x1000000;

const uint32  FLASH_FixedNvitemAddr = 0x07F0000;
const uint32  FLASH_ProductInfoAddr = 0x0FFE000; // the last 8K ( for some flash has the 8K sector)

// The offset of the sram that be used to simulator
const uint32  FLASH_EmuOffsetAddr    = 0x81000;
#endif // FLASH_AMD_128M

#ifdef FLASH_AMD_64M
const uint32  FLASH_SectorNum       = 15;       // From 0x70_0000 ==> 0x7E_FFFF
const uint32  FLASH_FileSystemNum   = 9;
const uint32  FLASH_SectorSize      = 0x10000;  // 64K
const uint32  FLASH_StartAddrr      = 0x0;
const uint32  FLASH_BankAddr        = 0x700000; // the bank managed by EFS
const uint32  FLASH_Size            = 0x800000;

const uint32  FLASH_FixedNvitemAddr = 0x6F0000;
const uint32  FLASH_ProductInfoAddr = 0x7FE000; // the last 8K ( for some flash has the 8K sector)

// The offset of the sram that be used to simulator
const uint32  FLASH_EmuOffsetAddr    = 0x81000;
#endif // FLASH_AMD_64M

#ifdef FLASH_AMD_32M
const uint32  FLASH_SectorNum       = 7;        // From 0x38_0000 ==> 0x3E_FFFF
const uint32  FLASH_FileSystemNum   = 2;
const uint32  FLASH_SectorSize      = 0x10000;  // 64K
const uint32  FLASH_StartAddrr      = 0x0;
const uint32  FLASH_BankAddr        = 0x380000; // the bank managed by EFS
const uint32  FLASH_Size            = 0x400000;

const uint32  FLASH_FixedNvitemAddr = 0x370000;
const uint32  FLASH_ProductInfoAddr = 0x3FE000;

// The offset of the sram that be used to simulator
const uint32  FLASH_EmuOffsetAddr    = 0x81000;
#endif //FLASH_AMD_32M


//
// SRAM config
//
#ifdef SRAM_512K
const uint32 g_fiq_stack_size        = 512;
const uint32 g_irq_stack_size        = 2048;
const uint32 g_undefine_stack_size   = 512;
const uint32 g_abort_stack_size      = 8;
const uint32 g_sys_stack_size        = 8;
const uint32 g_timer_stack_size      = 2048;

//the system memory allocation define,the allocation map defeine is different when
//chip,module and the flash chip used in the module is different for the production
//                                                  // Typical Define
const uint32 const_TestPointerOffset = 0x7DC00;     // = 0xFDC00   // 6408
// = 0x1FDC00  // 6416
// = 0x7DC00   // 3204

const uint32 const_StackLimitOffset  = 0x7DBFC;     // = 0xFDBFC   // 6408
// = 0x1FDBFC  // 6416
// = 0x7DBFC   // 3204

const uint32 const_RAM_LimitOffset  = 0x7C000;      // = 0xFC000   // 6408
// = 0x1FC000  // 6416
// = 0x7C000   // 3204

const uint32    g_svc_stack_limit_add = EXTM_XCSN1_START + 0x7DBFC;//StackLimitOffset
//the ZW and ZI data should not excced the RAM LIMIT,and RAM after RAM_LIMIT
//is used as stack of exceptions as FIQ,IRQ,ABORT and so on
const uint32    g_mem_limit = EXTM_XCSN1_START + const_RAM_LimitOffset;

const uint32    g_sram_size = 512 * 1024;

#endif //SRAM_512K

#ifdef SRAM_1M
const uint32 g_fiq_stack_size        = 512;
const uint32 g_irq_stack_size        = 2048;
const uint32 g_undefine_stack_size   = 512;
const uint32 g_abort_stack_size      = 8;
const uint32 g_sys_stack_size        = 8;
const uint32 g_timer_stack_size      = 2048;

//the system memory allocation define,the allocation map defeine is different when
//chip,module and the flash chip used in the module is different for the production
//                                                  // Typical Define
const uint32 const_TestPointerOffset = 0xFDC00;     // = 0xFDC00   // 6408
// = 0x1FDC00  // 6416
// = 0x7DC00   // 3204

const uint32 const_StackLimitOffset  = 0xFDBFC;     // = 0xFDBFC   // 6408
// = 0x1FDBFC  // 6416
// = 0x7DBFC   // 3204

const uint32 const_RAM_LimitOffset  = 0xFC000;      // = 0xFC000   // 6408
// = 0x1FC000  // 6416
// = 0x7C000   // 3204
const uint32    g_svc_stack_limit_add = EXTM_XCSN1_START + const_StackLimitOffset;
//the ZW and ZI data should not excced the RAM LIMIT,and RAM after RAM_LIMIT
//is used as stack of exceptions as FIQ,IRQ,ABORT and so on
const uint32    g_mem_limit = EXTM_XCSN1_START + 0xFC000;//const_RAM_LimitOffset

const uint32    g_sram_size = 1024 * 1024;

#endif //SRAM_1M

#ifdef SRAM_2M
const uint32 g_fiq_stack_size        = 512;
const uint32 g_irq_stack_size        = 2048;
const uint32 g_undefine_stack_size   = 512;
const uint32 g_abort_stack_size      = 8;
const uint32 g_sys_stack_size        = 8;
const uint32 g_timer_stack_size      = 2048;

//the system memory allocation define,the allocation map defeine is different when
//chip,module and the flash chip used in the module is different for the production
//                                                  // Typical Define
const uint32 const_TestPointerOffset = 0x1FDC00;    // = 0xFDC00   // 6408
// = 0x1FDC00  // 6416
// = 0x7DC00   // 3204

const uint32 const_StackLimitOffset  = 0x1FDBFC;    // = 0xFDBFC   // 6408
// = 0x1FDBFC  // 6416
// = 0x7DBFC   // 3204

const uint32 const_RAM_LimitOffset  = 0x1FC000;     // = 0xFC000   // 6408
// = 0x1FC000  // 6416
// = 0x7C000   // 3204

const uint32    g_svc_stack_limit_add = EXTM_XCSN1_START + 0x1FDBFC;//StackLimitOffset
//the ZW and ZI data should not excced the RAM LIMIT,and RAM after RAM_LIMIT
//is used as stack of exceptions as FIQ,IRQ,ABORT and so on
const uint32    g_mem_limit = EXTM_XCSN1_START + 0x1FC000;//const_RAM_LimitOffset

const uint32    g_sram_size = 1024 * 1024 * 2;
#endif //SRAM_2M
// Memory type    6416

#ifdef SRAM_4M
const uint32 g_fiq_stack_size        = 512;
const uint32 g_irq_stack_size        = 2048;
const uint32 g_undefine_stack_size   = 512;
const uint32 g_abort_stack_size      = 8;
const uint32 g_sys_stack_size        = 8;
const uint32 g_timer_stack_size      = 2048;

//the system memory allocation define,the allocation map defeine is different when
//chip,module and the flash chip used in the module is different for the production
//                                                  // Typical Define
const uint32 const_TestPointerOffset = 0x3FDC00;    // = 0xFDC00   // 6408
// = 0x1FDC00  // 6416
// = 0x7DC00   // 3204
// = 0x3FDC00  // AM49PDL127BH

const uint32 const_StackLimitOffset  = 0x3FDBFC;    // = 0xFDBFC   // 6408
// = 0x1FDBFC  // 6416
// = 0x7DBFC   // 3204
// = 0x3FDC00  // AM49PDL127BH

const uint32 const_RAM_LimitOffset  = 0x3FC000;     // = 0xFC000   // 6408
// = 0x1FC000  // 6416
// = 0x7C000   // 3204
// = 0x3FDC00  // AM49PDL127BH

const uint32    g_svc_stack_limit_add = EXTM_XCSN1_START + 0x3FDBFC;//StackLimitOffset
//the ZW and ZI data should not excced the RAM LIMIT,and RAM after RAM_LIMIT
//is used as stack of exceptions as FIQ,IRQ,ABORT and so on
const uint32    g_mem_limit = EXTM_XCSN1_START + 0x3FC000;//const_RAM_LimitOffset

const uint32    g_sram_size = 1024 * 1024 * 4;
#endif //SRAM_4M

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

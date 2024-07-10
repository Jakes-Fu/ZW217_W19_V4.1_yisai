/******************************************************************************
 ** File Name:      mmi_utility_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:261
#ifndef _MMI_UTILITY_TRC_H_
#define _MMI_UTILITY_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI_CACHE_152_112_2_18_3_25_6_0 "@@Cache count = %d."
#define MMI_LIVEWALLPAPER_UTIL_723_112_2_18_3_25_11_1 "@@GetLwResImgDataByOffset:offset = 0x%x, res_handle=0x%x, sfs_err = %d\n"
#define MMI_MEM_94_112_2_18_3_25_12_2 "[M]:heap_start_addr = 0x%x"
#define MMI_MEM_95_112_2_18_3_25_12_3 "[M]:heap_fragment = 0x%x"
#define MMI_MEM_96_112_2_18_3_25_12_4 "[M]:heap_available_size = 0x%x"
#define MMI_MEM_97_112_2_18_3_25_12_5 "[M]:heap_search = 0x%x"
#define MMI_MEM_100_112_2_18_3_25_12_6 "[M]:fragment = %d"
#define MMI_MEM_101_112_2_18_3_25_12_7 "[M]:cur_ptr  = 0x%x"
#define MMI_MEM_102_112_2_18_3_25_12_8 "[M]:state = 0x%x"
#define MMI_MEM_103_112_2_18_3_25_12_9 "[M]:realy memory = 0x%x"
#define MMI_MEM_104_112_2_18_3_25_12_10 "[M]:realy memory2 = 0x%x"
#define MMI_MEM_105_112_2_18_3_25_12_11 "[M]:next_ptr = 0x%x"
#define MMI_MEM_356_112_2_18_3_25_12_12 "[M]:s_mem_manage_id = %d"
#define MMI_MEM_671_112_2_18_3_25_13_13 "[M]:HeapPoolAllocate :info_ptr = 0x%x, size = %d, return memory_ptr = 0x%x\n"
#define MMI_MEM_679_112_2_18_3_25_13_14 "[M]:MMIAPIMEM_Malloc: size = %d, return = 0x%x\n"
#define MMI_MEM_687_112_2_18_3_25_13_15 "[M]:MMIAPIMEM_Malloc again: size = %d, return = 0x%x\n"
#define MMI_MEM_693_112_2_18_3_25_13_16 "[M]:malloc failed, no memory"
#define MMI_MEM_817_112_2_18_3_25_13_17 "[M]: free memory_ptr = 0x%x"
#define MMI_MEM_836_112_2_18_3_25_13_18 "[M]:free pointer failed: 0x%x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_UTILITY_TRC)
TRACE_MSG(MMI_CACHE_152_112_2_18_3_25_6_0,"@@Cache count = %d.")
TRACE_MSG(MMI_LIVEWALLPAPER_UTIL_723_112_2_18_3_25_11_1,"@@GetLwResImgDataByOffset:offset = 0x%x, res_handle=0x%x, sfs_err = %d\n")
TRACE_MSG(MMI_MEM_94_112_2_18_3_25_12_2,"[M]:heap_start_addr = 0x%x")
TRACE_MSG(MMI_MEM_95_112_2_18_3_25_12_3,"[M]:heap_fragment = 0x%x")
TRACE_MSG(MMI_MEM_96_112_2_18_3_25_12_4,"[M]:heap_available_size = 0x%x")
TRACE_MSG(MMI_MEM_97_112_2_18_3_25_12_5,"[M]:heap_search = 0x%x")
TRACE_MSG(MMI_MEM_100_112_2_18_3_25_12_6,"[M]:fragment = %d")
TRACE_MSG(MMI_MEM_101_112_2_18_3_25_12_7,"[M]:cur_ptr  = 0x%x")
TRACE_MSG(MMI_MEM_102_112_2_18_3_25_12_8,"[M]:state = 0x%x")
TRACE_MSG(MMI_MEM_103_112_2_18_3_25_12_9,"[M]:realy memory = 0x%x")
TRACE_MSG(MMI_MEM_104_112_2_18_3_25_12_10,"[M]:realy memory2 = 0x%x")
TRACE_MSG(MMI_MEM_105_112_2_18_3_25_12_11,"[M]:next_ptr = 0x%x")
TRACE_MSG(MMI_MEM_356_112_2_18_3_25_12_12,"[M]:s_mem_manage_id = %d")
TRACE_MSG(MMI_MEM_671_112_2_18_3_25_13_13,"[M]:HeapPoolAllocate :info_ptr = 0x%x, size = %d, return memory_ptr = 0x%x\n")
TRACE_MSG(MMI_MEM_679_112_2_18_3_25_13_14,"[M]:MMIAPIMEM_Malloc: size = %d, return = 0x%x\n")
TRACE_MSG(MMI_MEM_687_112_2_18_3_25_13_15,"[M]:MMIAPIMEM_Malloc again: size = %d, return = 0x%x\n")
TRACE_MSG(MMI_MEM_693_112_2_18_3_25_13_16,"[M]:malloc failed, no memory")
TRACE_MSG(MMI_MEM_817_112_2_18_3_25_13_17,"[M]: free memory_ptr = 0x%x")
TRACE_MSG(MMI_MEM_836_112_2_18_3_25_13_18,"[M]:free pointer failed: 0x%x")
END_TRACE_MAP(MMI_UTILITY_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_UTILITY_TRC_H_


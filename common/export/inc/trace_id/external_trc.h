/******************************************************************************
 ** File Name:      external_trc.h                                         *
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
//trace_id:11
#ifndef _EXTERNAL_TRC_H_
#define _EXTERNAL_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define FAKE_MEMORY_38_112_2_18_0_22_31_0 "LIBC_PoolFinaliz."
#define FAKE_MEMORY_65_112_2_18_0_22_31_1 "LIBC_PoolInital, s_libc_pool_handle=%d."
#define FAKE_MEMORY_97_112_2_18_0_22_31_2 "SPLIBC_malloc, pool is full, size=%d."
#define SQLITE3_VFS_ADAPTER_47_112_2_18_0_26_10_3 "CLOSE   %-3d\n"
#define SQLITE3_VFS_ADAPTER_69_112_2_18_0_26_10_4 "READ %d \n"
#define SQLITE3_VFS_ADAPTER_106_112_2_18_0_26_11_5 "WRITE %d \n"
#define SQLITE3_VFS_ADAPTER_129_112_2_18_0_26_11_6 "TRUNCATE %d %lld\n"
#define SQLITE3_VFS_ADAPTER_149_112_2_18_0_26_11_7 "SYNC %d \n"
#define SQLITE3_VFS_ADAPTER_314_112_2_18_0_26_11_8 "OPEN want %d\n"
#define SQLITE3_VFS_ADAPTER_319_112_2_18_0_26_11_9 "OPEN read/write\n"
#define SQLITE3_VFS_ADAPTER_324_112_2_18_0_26_11_10 "OPEN read only\n"
#define SQLITE3_VFS_ADAPTER_329_112_2_18_0_26_11_11 "OPEN open new/create\n"
#define SQLITE3_VFS_ADAPTER_332_112_2_18_0_26_11_12 "OPEN open existing\n"
#define SQLITE3_VFS_ADAPTER_337_112_2_18_0_26_11_13 "OPEN share read/write\n"
#define SQLITE3_VFS_ADAPTER_340_112_2_18_0_26_11_14 "OPEN share read only\n"
#define SQLITE3_VFS_ADAPTER_345_112_2_18_0_26_11_15 "OPEN hidden/delete on close file attributes\n"
#define SQLITE3_VFS_ADAPTER_349_112_2_18_0_26_11_16 "OPEN normal file attribute\n"
#define SQLITE3_VFS_ADAPTER_366_112_2_18_0_26_11_17 "OPEN Invalid handle rc=%d: zName=%s,Mode=%#lx\n"
#define SQLITE3_VFS_ADAPTER_370_112_2_18_0_26_11_18 "OPEN %d Invalid handlen"
#define SQLITE3_VFS_ADAPTER_389_112_2_18_0_26_11_19 "OPEN %d pOutFlags=%d\n"
#define SQLITE3_VFS_ADAPTER_421_112_2_18_0_26_11_20 "DELETE \"%s\"\n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(EXTERNAL_TRC)
TRACE_MSG(FAKE_MEMORY_38_112_2_18_0_22_31_0,"LIBC_PoolFinaliz.")
TRACE_MSG(FAKE_MEMORY_65_112_2_18_0_22_31_1,"LIBC_PoolInital, s_libc_pool_handle=%d.")
TRACE_MSG(FAKE_MEMORY_97_112_2_18_0_22_31_2,"SPLIBC_malloc, pool is full, size=%d.")
TRACE_MSG(SQLITE3_VFS_ADAPTER_47_112_2_18_0_26_10_3,"CLOSE   %-3d\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_69_112_2_18_0_26_10_4,"READ %d \n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_106_112_2_18_0_26_11_5,"WRITE %d \n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_129_112_2_18_0_26_11_6,"TRUNCATE %d %lld\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_149_112_2_18_0_26_11_7,"SYNC %d \n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_314_112_2_18_0_26_11_8,"OPEN want %d\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_319_112_2_18_0_26_11_9,"OPEN read/write\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_324_112_2_18_0_26_11_10,"OPEN read only\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_329_112_2_18_0_26_11_11,"OPEN open new/create\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_332_112_2_18_0_26_11_12,"OPEN open existing\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_337_112_2_18_0_26_11_13,"OPEN share read/write\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_340_112_2_18_0_26_11_14,"OPEN share read only\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_345_112_2_18_0_26_11_15,"OPEN hidden/delete on close file attributes\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_349_112_2_18_0_26_11_16,"OPEN normal file attribute\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_366_112_2_18_0_26_11_17,"OPEN Invalid handle rc=%d: zName=%s,Mode=%#lx\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_370_112_2_18_0_26_11_18,"OPEN %d Invalid handlen")
TRACE_MSG(SQLITE3_VFS_ADAPTER_389_112_2_18_0_26_11_19,"OPEN %d pOutFlags=%d\n")
TRACE_MSG(SQLITE3_VFS_ADAPTER_421_112_2_18_0_26_11_20,"DELETE \"%s\"\n")
END_TRACE_MAP(EXTERNAL_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _EXTERNAL_TRC_H_


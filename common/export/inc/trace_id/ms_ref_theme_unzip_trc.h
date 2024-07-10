/******************************************************************************
 ** File Name:      ms_ref_theme_unzip_trc.h                                         *
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
//trace_id:71
#ifndef _MS_REF_THEME_UNZIP_TRC_H_
#define _MS_REF_THEME_UNZIP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define THEME_GZIP_101_112_2_18_1_54_38_0 "THEME_UNZIP: Not zip flag: 0x%x-0x%x"
#define THEME_GZIP_129_112_2_18_1_54_38_1 "THEME_UNZIP: Open compressed file failed"
#define THEME_GZIP_135_112_2_18_1_54_38_2 "THEME_UNZIP: Get compressed file size failed"
#define THEME_GZIP_141_112_2_18_1_54_38_3 "THEME_UNZIP: Path length is 0"
#define THEME_GZIP_157_112_2_18_1_54_38_4 "THEME_UNZIP: Create output file failed"
#define THEME_GZIP_166_112_2_18_1_54_38_5 "THEME_UNZIP: Work function return error"
#define THEME_GZIP_176_112_2_18_1_54_38_6 "THEME_UNZIP: Report-processed: %d"
#define THEME_GZIP_187_112_2_18_1_54_38_7 "THEME_UNZIP: processed: %d"
#define THEME_GZIP_194_112_2_18_1_54_38_8 "THEME_UNZIP: processed: %d"
#define THEME_GZIP_207_112_2_18_1_54_38_9 "THEME_UNZIP: Get g_theme_method error, value %d"
#define THEME_GZIP_215_112_2_18_1_54_38_10 "THEME_UNZIP: Create output file failed"
#define THEME_GZIP_239_112_2_18_1_54_38_11 "THEME_UNZIP: Allocate memory failed: theme_entry_param"
#define THEME_GZIP_245_112_2_18_1_54_38_12 "THEME_UNZIP: Allocate memory failed: g_theme_inbuf"
#define THEME_GZIP_251_112_2_18_1_54_38_13 "THEME_UNZIP: Allocate memory failed: g_theme_window"
#define THEME_GZIP_273_112_2_18_1_54_38_14 "THEME_UNZIP: Create thread %d"
#define THEME_UNZIP_45_112_2_18_1_54_40_15 "THEME_UNZIP: Error zip method--%d"
#define THEME_UNZIP_76_112_2_18_1_54_40_16 "THEME_UNZIP: Create dir failed, error code %d"
#define THEME_UNZIP_107_112_2_18_1_54_40_17 "THEME_UNZIP: Zip file didn't contain file name or name too long"
#define THEME_UNZIP_134_112_2_18_1_54_40_18 "THEME_UNZIP: Out of memory"
#define THEME_UNZIP_139_112_2_18_1_54_41_19 "THEME_UNZIP: Invalid compressed data--format violated"
#define THEME_UNZIP_146_112_2_18_1_54_41_20 "THEME_UNZIP: Internal error, invalid method"
#define THEME_UNZIP_153_112_2_18_1_54_41_21 "THEME_UNZIP: Invalid compressed data--crc error"
#define THEME_UNZIP_159_112_2_18_1_54_41_22 "THEME_UNZIP: Invalid compressed data--length error"
#define THEME_UTIL_58_112_2_18_1_54_41_23 "THEME_UNZIP: Create base dir failed, error code %d"
#define THEME_UTIL_120_112_2_18_1_54_41_24 "THEME_UNZIP: Read compressed error, error code %d"
#define THEME_UTIL_125_112_2_18_1_54_41_25 "THEME_UNZIP: Read compressed file complete"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_THEME_UNZIP_TRC)
TRACE_MSG(THEME_GZIP_101_112_2_18_1_54_38_0,"THEME_UNZIP: Not zip flag: 0x%x-0x%x")
TRACE_MSG(THEME_GZIP_129_112_2_18_1_54_38_1,"THEME_UNZIP: Open compressed file failed")
TRACE_MSG(THEME_GZIP_135_112_2_18_1_54_38_2,"THEME_UNZIP: Get compressed file size failed")
TRACE_MSG(THEME_GZIP_141_112_2_18_1_54_38_3,"THEME_UNZIP: Path length is 0")
TRACE_MSG(THEME_GZIP_157_112_2_18_1_54_38_4,"THEME_UNZIP: Create output file failed")
TRACE_MSG(THEME_GZIP_166_112_2_18_1_54_38_5,"THEME_UNZIP: Work function return error")
TRACE_MSG(THEME_GZIP_176_112_2_18_1_54_38_6,"THEME_UNZIP: Report-processed: %d")
TRACE_MSG(THEME_GZIP_187_112_2_18_1_54_38_7,"THEME_UNZIP: processed: %d")
TRACE_MSG(THEME_GZIP_194_112_2_18_1_54_38_8,"THEME_UNZIP: processed: %d")
TRACE_MSG(THEME_GZIP_207_112_2_18_1_54_38_9,"THEME_UNZIP: Get g_theme_method error, value %d")
TRACE_MSG(THEME_GZIP_215_112_2_18_1_54_38_10,"THEME_UNZIP: Create output file failed")
TRACE_MSG(THEME_GZIP_239_112_2_18_1_54_38_11,"THEME_UNZIP: Allocate memory failed: theme_entry_param")
TRACE_MSG(THEME_GZIP_245_112_2_18_1_54_38_12,"THEME_UNZIP: Allocate memory failed: g_theme_inbuf")
TRACE_MSG(THEME_GZIP_251_112_2_18_1_54_38_13,"THEME_UNZIP: Allocate memory failed: g_theme_window")
TRACE_MSG(THEME_GZIP_273_112_2_18_1_54_38_14,"THEME_UNZIP: Create thread %d")
TRACE_MSG(THEME_UNZIP_45_112_2_18_1_54_40_15,"THEME_UNZIP: Error zip method--%d")
TRACE_MSG(THEME_UNZIP_76_112_2_18_1_54_40_16,"THEME_UNZIP: Create dir failed, error code %d")
TRACE_MSG(THEME_UNZIP_107_112_2_18_1_54_40_17,"THEME_UNZIP: Zip file didn't contain file name or name too long")
TRACE_MSG(THEME_UNZIP_134_112_2_18_1_54_40_18,"THEME_UNZIP: Out of memory")
TRACE_MSG(THEME_UNZIP_139_112_2_18_1_54_41_19,"THEME_UNZIP: Invalid compressed data--format violated")
TRACE_MSG(THEME_UNZIP_146_112_2_18_1_54_41_20,"THEME_UNZIP: Internal error, invalid method")
TRACE_MSG(THEME_UNZIP_153_112_2_18_1_54_41_21,"THEME_UNZIP: Invalid compressed data--crc error")
TRACE_MSG(THEME_UNZIP_159_112_2_18_1_54_41_22,"THEME_UNZIP: Invalid compressed data--length error")
TRACE_MSG(THEME_UTIL_58_112_2_18_1_54_41_23,"THEME_UNZIP: Create base dir failed, error code %d")
TRACE_MSG(THEME_UTIL_120_112_2_18_1_54_41_24,"THEME_UNZIP: Read compressed error, error code %d")
TRACE_MSG(THEME_UTIL_125_112_2_18_1_54_41_25,"THEME_UNZIP: Read compressed file complete")
END_TRACE_MAP(MS_REF_THEME_UNZIP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_THEME_UNZIP_TRC_H_


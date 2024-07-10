/******************************************************************************
 ** File Name:      ms_ref_gzip_trc.h                                         *
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
//trace_id:50
#ifndef _MS_REF_GZIP_TRC_H_
#define _MS_REF_GZIP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define IMG_GZIP_DEFLATE_637_112_2_18_1_32_3_0 "deflate_fast:%c"
#define IMG_GZIP_DEFLATE_739_112_2_18_1_32_3_1 "deflate: %c"
#define IMG_GZIP_GZIP_395_112_2_18_1_32_4_2 "IMGGZIP_Decode: size error! real size %d, source size %d"
#define IMG_GZIP_GZIP_428_112_2_18_1_32_4_3 "dest size %d  error %d"
#define IMG_GZIP_GZIP_559_112_2_18_1_32_4_4 "magic[0]=0x%x"
#define IMG_GZIP_GZIP_560_112_2_18_1_32_4_5 "magic[1]=0x%x"
#define IMG_GZIP_GZIP_562_112_2_18_1_32_4_6 "inputbuf[0]=0x%x"
#define IMG_GZIP_GZIP_563_112_2_18_1_32_4_7 "inputbuf[1]=0x%x"
#define IMG_GZIP_GZIP_564_112_2_18_1_32_4_8 "inputbuf[2]=0x%x"
#define IMG_GZIP_GZIP_565_112_2_18_1_32_4_9 "inputbuf[3]=0x%x"
#define IMG_GZIP_GZIP_567_112_2_18_1_32_4_10 "inptr=0x%x"
#define IMG_GZIP_GZIP_575_112_2_18_1_32_4_11 "magic[1]=0x%x"
#define IMG_GZIP_GZIP_580_112_2_18_1_32_4_12 "method=0x%x"
#define IMG_GZIP_GZIP_583_112_2_18_1_32_4_13 "unknown method %d"
#define IMG_GZIP_GZIP_592_112_2_18_1_32_4_14 "encrypted -- get newer version of gzip!"
#define IMG_GZIP_GZIP_598_112_2_18_1_32_4_15 "multi-part gzip file -- get newer version of gzip"
#define IMG_GZIP_GZIP_605_112_2_18_1_32_4_16 "get newer version of gzip"
#define IMG_GZIP_GZIP_636_112_2_18_1_32_4_17 "extra field of %u bytes ignored"
#define IMG_GZIP_GZIP_685_112_2_18_1_32_5_18 "PKZIP_MAGIC method=0x%x"
#define IMG_GZIP_GZIP_716_112_2_18_1_32_5_19 "not in gzip format"
#define IMG_GZIP_GZIP_720_112_2_18_1_32_5_20 "decompression OK, trailing garbage ignored"
#define IMG_GZIP_GZIP_850_112_2_18_1_32_5_21 "inbuf=0x%x,outbuf=0x%x,d_buf=0x%x,window=0x%x,tab_prefix1=0x%x"
#define IMG_GZIP_LZW_21_112_2_18_1_32_7_22 "output in compress .Z format not supported"
#define IMG_GZIP_UNLZW_234_112_2_18_1_32_12_23 "warning, unknown flags 0x%x"
#define IMG_GZIP_UNLZW_246_112_2_18_1_32_12_24 "compressed with %d bits, can only handle %d bits"
#define IMG_GZIP_UNZIP_130_112_2_18_1_32_14_25 "GZIP:ERROR,invalid compressed data--format violated"
#define IMG_GZIP_UNZIP_140_112_2_18_1_32_14_26 "GZIP:ERROR,unzip:len %ld, siz %d"
#define IMG_GZIP_UNZIP_141_112_2_18_1_32_14_27 "GZIP:ERROR,invalid compressed data--length mismatch"
#define IMG_GZIP_UNZIP_156_112_2_18_1_32_14_28 "GZIP:ERROR,internal error, invalid method"
#define IMG_GZIP_UNZIP_187_112_2_18_1_32_14_29 "GZIP:ERROR,invalid compressed data--crc error"
#define IMG_GZIP_UNZIP_192_112_2_18_1_32_14_30 "GZIP:ERROR,invalid compressed data--length error"
#define IMG_GZIP_UNZIP_202_112_2_18_1_32_14_31 "has more than one entry--rest ignored"
#define IMG_GZIP_UNZIP_208_112_2_18_1_32_14_32 "has more than one entry -- unchanged"
#define IMG_GZIP_UTIL_383_112_2_18_1_32_15_33 "error %s"
#define IMG_GZIP_UTIL_395_112_2_18_1_32_15_34 "warning: %s%s"
#define IMG_GZIP_UTIL_408_112_2_18_1_32_15_35 "unexpected end of file"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_GZIP_TRC)
TRACE_MSG(IMG_GZIP_DEFLATE_637_112_2_18_1_32_3_0,"deflate_fast:%c")
TRACE_MSG(IMG_GZIP_DEFLATE_739_112_2_18_1_32_3_1,"deflate: %c")
TRACE_MSG(IMG_GZIP_GZIP_395_112_2_18_1_32_4_2,"IMGGZIP_Decode: size error! real size %d, source size %d")
TRACE_MSG(IMG_GZIP_GZIP_428_112_2_18_1_32_4_3,"dest size %d  error %d")
TRACE_MSG(IMG_GZIP_GZIP_559_112_2_18_1_32_4_4,"magic[0]=0x%x")
TRACE_MSG(IMG_GZIP_GZIP_560_112_2_18_1_32_4_5,"magic[1]=0x%x")
TRACE_MSG(IMG_GZIP_GZIP_562_112_2_18_1_32_4_6,"inputbuf[0]=0x%x")
TRACE_MSG(IMG_GZIP_GZIP_563_112_2_18_1_32_4_7,"inputbuf[1]=0x%x")
TRACE_MSG(IMG_GZIP_GZIP_564_112_2_18_1_32_4_8,"inputbuf[2]=0x%x")
TRACE_MSG(IMG_GZIP_GZIP_565_112_2_18_1_32_4_9,"inputbuf[3]=0x%x")
TRACE_MSG(IMG_GZIP_GZIP_567_112_2_18_1_32_4_10,"inptr=0x%x")
TRACE_MSG(IMG_GZIP_GZIP_575_112_2_18_1_32_4_11,"magic[1]=0x%x")
TRACE_MSG(IMG_GZIP_GZIP_580_112_2_18_1_32_4_12,"method=0x%x")
TRACE_MSG(IMG_GZIP_GZIP_583_112_2_18_1_32_4_13,"unknown method %d")
TRACE_MSG(IMG_GZIP_GZIP_592_112_2_18_1_32_4_14,"encrypted -- get newer version of gzip!")
TRACE_MSG(IMG_GZIP_GZIP_598_112_2_18_1_32_4_15,"multi-part gzip file -- get newer version of gzip")
TRACE_MSG(IMG_GZIP_GZIP_605_112_2_18_1_32_4_16,"get newer version of gzip")
TRACE_MSG(IMG_GZIP_GZIP_636_112_2_18_1_32_4_17,"extra field of %u bytes ignored")
TRACE_MSG(IMG_GZIP_GZIP_685_112_2_18_1_32_5_18,"PKZIP_MAGIC method=0x%x")
TRACE_MSG(IMG_GZIP_GZIP_716_112_2_18_1_32_5_19,"not in gzip format")
TRACE_MSG(IMG_GZIP_GZIP_720_112_2_18_1_32_5_20,"decompression OK, trailing garbage ignored")
TRACE_MSG(IMG_GZIP_GZIP_850_112_2_18_1_32_5_21,"inbuf=0x%x,outbuf=0x%x,d_buf=0x%x,window=0x%x,tab_prefix1=0x%x")
TRACE_MSG(IMG_GZIP_LZW_21_112_2_18_1_32_7_22,"output in compress .Z format not supported")
TRACE_MSG(IMG_GZIP_UNLZW_234_112_2_18_1_32_12_23,"warning, unknown flags 0x%x")
TRACE_MSG(IMG_GZIP_UNLZW_246_112_2_18_1_32_12_24,"compressed with %d bits, can only handle %d bits")
TRACE_MSG(IMG_GZIP_UNZIP_130_112_2_18_1_32_14_25,"GZIP:ERROR,invalid compressed data--format violated")
TRACE_MSG(IMG_GZIP_UNZIP_140_112_2_18_1_32_14_26,"GZIP:ERROR,unzip:len %ld, siz %d")
TRACE_MSG(IMG_GZIP_UNZIP_141_112_2_18_1_32_14_27,"GZIP:ERROR,invalid compressed data--length mismatch")
TRACE_MSG(IMG_GZIP_UNZIP_156_112_2_18_1_32_14_28,"GZIP:ERROR,internal error, invalid method")
TRACE_MSG(IMG_GZIP_UNZIP_187_112_2_18_1_32_14_29,"GZIP:ERROR,invalid compressed data--crc error")
TRACE_MSG(IMG_GZIP_UNZIP_192_112_2_18_1_32_14_30,"GZIP:ERROR,invalid compressed data--length error")
TRACE_MSG(IMG_GZIP_UNZIP_202_112_2_18_1_32_14_31,"has more than one entry--rest ignored")
TRACE_MSG(IMG_GZIP_UNZIP_208_112_2_18_1_32_14_32,"has more than one entry -- unchanged")
TRACE_MSG(IMG_GZIP_UTIL_383_112_2_18_1_32_15_33,"error %s")
TRACE_MSG(IMG_GZIP_UTIL_395_112_2_18_1_32_15_34,"warning: %s%s")
TRACE_MSG(IMG_GZIP_UTIL_408_112_2_18_1_32_15_35,"unexpected end of file")
END_TRACE_MAP(MS_REF_GZIP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_GZIP_TRC_H_


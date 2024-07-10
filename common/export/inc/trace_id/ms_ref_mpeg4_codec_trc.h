/******************************************************************************
 ** File Name:      ms_ref_mpeg4_codec_trc.h                                         *
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
//trace_id:58
#ifndef _MS_REF_MPEG4_CODEC_TRC_H_
#define _MS_REF_MPEG4_CODEC_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MP4DEC_HEADER_1317_112_2_18_1_46_57_0 "end of sequence!\n"
#define MP4DEC_HEADER_1334_112_2_18_1_46_57_1 "This is not a legal H.263 bitstream\n"
#define MP4DEC_HEADER_1348_112_2_18_1_46_57_2 "This is not a legal H.263 bitstream\n"
#define MP4DEC_BITSTREAM_133_112_2_18_1_47_25_3 "Mp4Dec_VerifyBitstrm,BIT[31:37]= 0x%x"
#define MP4DEC_MALLOC_59_112_2_18_1_48_11_4 "Mp4Dec_ExtraMemAlloc failed,required=%d,total=%d,used=%d"
#define MP4DEC_MALLOC_46_112_2_18_1_48_52_5 "Mp4Dec_ExtraMemAlloc failed,required=%d,total=%d,used=%d"
#define MP4DEC_MALLOC_74_112_2_18_1_48_52_6 "Mp4Dec_ExtraMemAlloc_64WordAlign failed,required=%d,total=%d,used=%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_MPEG4_CODEC_TRC)
TRACE_MSG(MP4DEC_HEADER_1317_112_2_18_1_46_57_0,"end of sequence!\n")
TRACE_MSG(MP4DEC_HEADER_1334_112_2_18_1_46_57_1,"This is not a legal H.263 bitstream\n")
TRACE_MSG(MP4DEC_HEADER_1348_112_2_18_1_46_57_2,"This is not a legal H.263 bitstream\n")
TRACE_MSG(MP4DEC_BITSTREAM_133_112_2_18_1_47_25_3,"Mp4Dec_VerifyBitstrm,BIT[31:37]= 0x%x")
TRACE_MSG(MP4DEC_MALLOC_59_112_2_18_1_48_11_4,"Mp4Dec_ExtraMemAlloc failed,required=%d,total=%d,used=%d")
TRACE_MSG(MP4DEC_MALLOC_46_112_2_18_1_48_52_5,"Mp4Dec_ExtraMemAlloc failed,required=%d,total=%d,used=%d")
TRACE_MSG(MP4DEC_MALLOC_74_112_2_18_1_48_52_6,"Mp4Dec_ExtraMemAlloc_64WordAlign failed,required=%d,total=%d,used=%d")
END_TRACE_MAP(MS_REF_MPEG4_CODEC_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_MPEG4_CODEC_TRC_H_


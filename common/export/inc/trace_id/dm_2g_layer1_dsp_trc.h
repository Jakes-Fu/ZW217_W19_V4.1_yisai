/******************************************************************************
 ** File Name:      dm_2g_layer1_dsp_trc.h                                         *
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
//trace_id:9
#ifndef _DM_2G_LAYER1_DSP_TRC_H_
#define _DM_2G_LAYER1_DSP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define DSP_OPERATE_1254_112_2_17_23_57_2_0 "0x8B000038 value is %x"
#define LAYER1_DSP_DOWNLOAD_193_112_2_18_0_3_1_1 "L1:dsp code has inited!"
#define LAYER1_DSP_DOWNLOAD_208_112_2_18_0_3_1_2 "L1:Use dsp code in NV!!, chip type 0x%x"
#define LAYER1_DSP_DOWNLOAD_209_112_2_18_0_3_1_3 "codesize = %d"
#define LAYER1_DSP_DOWNLOAD_214_112_2_18_0_3_1_4 "L1:Use dsp_pdata.c!!, chip type 0x%x"
#define LAYER1_DSP_DOWNLOAD_277_112_2_18_0_3_2_5 "dsp code has inited!"
#define LAYER1_DSP_DOWNLOAD_334_112_2_18_0_3_2_6 "SCI_InitLayer1 cmd 002 \n"
#define LAYER1_DSP_DOWNLOAD_371_112_2_18_0_3_2_7 "SCI_InitLayer1 cmd 003 \n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(DM_2G_LAYER1_DSP_TRC)
TRACE_MSG(DSP_OPERATE_1254_112_2_17_23_57_2_0,"0x8B000038 value is %x")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_193_112_2_18_0_3_1_1,"L1:dsp code has inited!")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_208_112_2_18_0_3_1_2,"L1:Use dsp code in NV!!, chip type 0x%x")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_209_112_2_18_0_3_1_3,"codesize = %d")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_214_112_2_18_0_3_1_4,"L1:Use dsp_pdata.c!!, chip type 0x%x")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_277_112_2_18_0_3_2_5,"dsp code has inited!")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_334_112_2_18_0_3_2_6,"SCI_InitLayer1 cmd 002 \n")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_371_112_2_18_0_3_2_7,"SCI_InitLayer1 cmd 003 \n")
END_TRACE_MAP(DM_2G_LAYER1_DSP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _DM_2G_LAYER1_DSP_TRC_H_


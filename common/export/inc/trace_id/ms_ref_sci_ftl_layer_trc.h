/******************************************************************************
 ** File Name:      ms_ref_sci_ftl_layer_trc.h                                         *
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
//trace_id:65
#ifndef _MS_REF_SCI_FTL_LAYER_TRC_H_
#define _MS_REF_SCI_FTL_LAYER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define SCI_FTLMNT_ENGIN_56_112_2_18_1_52_20_0 "[SCI_FTL_MntEngin:_ftl_read failed 0x%x"
#define SCI_FTLMNT_ENGIN_74_112_2_18_1_52_20_1 "[SCI_FTL_MntEngin:_ftl_write failed 0x%x"
#define SCI_FTLMNT_ENGIN_106_112_2_18_1_52_20_2 "[SCI_FTL_MntEngin:_ftl_erase failed 0x%x"
#define SCI_FTLMNT_ENGIN_252_112_2_18_1_52_21_3 "SCI_FTL_MntEngin: badBlk-->rpBlk"
#define SCI_FTLMNT_ENGIN_255_112_2_18_1_52_21_4 "SCI_FTL_MntEngin: none-->none"
#define SCI_FTLMNT_ENGIN_260_112_2_18_1_52_21_5 "SCI_FTL_MntEngin: %d-->%d"
#define SCI_FTLMNT_ENGIN_262_112_2_18_1_52_21_6 "SCI_FTL_MntEngin: printf End total = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_SCI_FTL_LAYER_TRC)
TRACE_MSG(SCI_FTLMNT_ENGIN_56_112_2_18_1_52_20_0,"[SCI_FTL_MntEngin:_ftl_read failed 0x%x")
TRACE_MSG(SCI_FTLMNT_ENGIN_74_112_2_18_1_52_20_1,"[SCI_FTL_MntEngin:_ftl_write failed 0x%x")
TRACE_MSG(SCI_FTLMNT_ENGIN_106_112_2_18_1_52_20_2,"[SCI_FTL_MntEngin:_ftl_erase failed 0x%x")
TRACE_MSG(SCI_FTLMNT_ENGIN_252_112_2_18_1_52_21_3,"SCI_FTL_MntEngin: badBlk-->rpBlk")
TRACE_MSG(SCI_FTLMNT_ENGIN_255_112_2_18_1_52_21_4,"SCI_FTL_MntEngin: none-->none")
TRACE_MSG(SCI_FTLMNT_ENGIN_260_112_2_18_1_52_21_5,"SCI_FTL_MntEngin: %d-->%d")
TRACE_MSG(SCI_FTLMNT_ENGIN_262_112_2_18_1_52_21_6,"SCI_FTL_MntEngin: printf End total = %d")
END_TRACE_MAP(MS_REF_SCI_FTL_LAYER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_SCI_FTL_LAYER_TRC_H_


/******************************************************************************
 ** File Name:      ms_ref_scm_trc.h                                         *
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
//trace_id:66
#ifndef _MS_REF_SCM_TRC_H_
#define _MS_REF_SCM_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define SCM_FUNC_165_112_2_18_1_52_23_0 "SCM _SendEvtToClient slot 0x%x evt 0x%x"
#define SCM_FUNC_201_112_2_18_1_52_23_1 "SCM Slot 0x%x sleep"
#define SCM_FUNC_234_112_2_18_1_52_23_2 "SCM _SCM_ReInit slot 0x%x"
#define SCM_FUNC_246_112_2_18_1_52_23_3 "SCM _SCM_ErrProc, slot 0x%x cmd = %x"
#define SCM_FUNC_252_112_2_18_1_52_23_4 "SCM _SCM_ErrProc : Error process succuss"
#define SCM_FUNC_269_112_2_18_1_52_23_5 "SCM _SCM_ErrProc : Error process fail"
#define SCM_FUNC_278_112_2_18_1_52_23_6 "SCM _SCM_ErrProc : Card has been removed from slot"
#define SCM_FUNC_295_112_2_18_1_52_23_7 "SCM _SCM_RegSlot"
#define SCM_FUNC_372_112_2_18_1_52_23_8 "SCM _SCM_UnRegSlot"
#define SCM_FUNC_409_112_2_18_1_52_23_9 "SCM _SCM_RegEvent slot 0x%x, evt 0x%x"
#define SCM_FUNC_464_112_2_18_1_52_24_10 "SCM _SCM_GetSlotStatus slot 0x%x"
#define SCM_FUNC_486_112_2_18_1_52_24_11 "SCM _SCM_SetService slot 0x%x,service 0x%x, onoff = 0x%x"
#define SCM_FUNC_489_112_2_18_1_52_24_12 "SCM _SCM_SetService invalid slot id."
#define SCM_FUNC_500_112_2_18_1_52_24_13 "SCM invalid service id"
#define SCM_FUNC_510_112_2_18_1_52_24_14 "SCM _SCM_PlugIn"
#define SCM_FUNC_545_112_2_18_1_52_24_15 "SCM _SCM_PlugOut"
#define SCM_FUNC_577_112_2_18_1_52_24_16 "SCM invalid plug event slot:0x%x"
#define SCM_FUNC_580_112_2_18_1_52_24_17 "SCM plug evt 0x%x"
#define SCM_FUNC_638_112_2_18_1_52_24_18 "SCM _SCM_IOCTL : Slot is empty state"
#define SCM_FUNC_645_112_2_18_1_52_24_19 "SCM _SCM_IOCTL : Slot is error"
#define SCM_TASK_44_112_2_18_1_52_25_20 "SCM Pwr scan"
#define SCM_TASK_49_112_2_18_1_52_25_21 "SCM some slot wake up"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_SCM_TRC)
TRACE_MSG(SCM_FUNC_165_112_2_18_1_52_23_0,"SCM _SendEvtToClient slot 0x%x evt 0x%x")
TRACE_MSG(SCM_FUNC_201_112_2_18_1_52_23_1,"SCM Slot 0x%x sleep")
TRACE_MSG(SCM_FUNC_234_112_2_18_1_52_23_2,"SCM _SCM_ReInit slot 0x%x")
TRACE_MSG(SCM_FUNC_246_112_2_18_1_52_23_3,"SCM _SCM_ErrProc, slot 0x%x cmd = %x")
TRACE_MSG(SCM_FUNC_252_112_2_18_1_52_23_4,"SCM _SCM_ErrProc : Error process succuss")
TRACE_MSG(SCM_FUNC_269_112_2_18_1_52_23_5,"SCM _SCM_ErrProc : Error process fail")
TRACE_MSG(SCM_FUNC_278_112_2_18_1_52_23_6,"SCM _SCM_ErrProc : Card has been removed from slot")
TRACE_MSG(SCM_FUNC_295_112_2_18_1_52_23_7,"SCM _SCM_RegSlot")
TRACE_MSG(SCM_FUNC_372_112_2_18_1_52_23_8,"SCM _SCM_UnRegSlot")
TRACE_MSG(SCM_FUNC_409_112_2_18_1_52_23_9,"SCM _SCM_RegEvent slot 0x%x, evt 0x%x")
TRACE_MSG(SCM_FUNC_464_112_2_18_1_52_24_10,"SCM _SCM_GetSlotStatus slot 0x%x")
TRACE_MSG(SCM_FUNC_486_112_2_18_1_52_24_11,"SCM _SCM_SetService slot 0x%x,service 0x%x, onoff = 0x%x")
TRACE_MSG(SCM_FUNC_489_112_2_18_1_52_24_12,"SCM _SCM_SetService invalid slot id.")
TRACE_MSG(SCM_FUNC_500_112_2_18_1_52_24_13,"SCM invalid service id")
TRACE_MSG(SCM_FUNC_510_112_2_18_1_52_24_14,"SCM _SCM_PlugIn")
TRACE_MSG(SCM_FUNC_545_112_2_18_1_52_24_15,"SCM _SCM_PlugOut")
TRACE_MSG(SCM_FUNC_577_112_2_18_1_52_24_16,"SCM invalid plug event slot:0x%x")
TRACE_MSG(SCM_FUNC_580_112_2_18_1_52_24_17,"SCM plug evt 0x%x")
TRACE_MSG(SCM_FUNC_638_112_2_18_1_52_24_18,"SCM _SCM_IOCTL : Slot is empty state")
TRACE_MSG(SCM_FUNC_645_112_2_18_1_52_24_19,"SCM _SCM_IOCTL : Slot is error")
TRACE_MSG(SCM_TASK_44_112_2_18_1_52_25_20,"SCM Pwr scan")
TRACE_MSG(SCM_TASK_49_112_2_18_1_52_25_21,"SCM some slot wake up")
END_TRACE_MAP(MS_REF_SCM_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_SCM_TRC_H_


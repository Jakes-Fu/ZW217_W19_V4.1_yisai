/******************************************************************************
 ** File Name:      ms_ref_xsr_layer_trc.h                                         *
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
//trace_id:81
#ifndef _MS_REF_XSR_LAYER_TRC_H_
#define _MS_REF_XSR_LAYER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define COMPACTION_TASK_60_112_2_18_1_57_9_0 "[XSR_Port:register_part] Index %d registered\r\n"
#define COMPACTION_TASK_68_112_2_18_1_57_9_1 "[XSR_Port:trigger_comp] Wake up compaction task\r\n"
#define COMPACTION_TASK_79_112_2_18_1_57_9_2 "[XSR_Port:STL_compaction_task] Waked-up to do compaction\r\n"
#define COMPACTION_TASK_81_112_2_18_1_57_9_3 "[XSR_Port:STL_compaction_task] All compacitons done. To sleep\r\n"
#define COMPACTION_TASK_97_112_2_18_1_57_9_4 "[XSR_Port:_do_compaction] Compact %d (0x%x)rn"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_XSR_LAYER_TRC)
TRACE_MSG(COMPACTION_TASK_60_112_2_18_1_57_9_0,"[XSR_Port:register_part] Index %d registered\r\n")
TRACE_MSG(COMPACTION_TASK_68_112_2_18_1_57_9_1,"[XSR_Port:trigger_comp] Wake up compaction task\r\n")
TRACE_MSG(COMPACTION_TASK_79_112_2_18_1_57_9_2,"[XSR_Port:STL_compaction_task] Waked-up to do compaction\r\n")
TRACE_MSG(COMPACTION_TASK_81_112_2_18_1_57_9_3,"[XSR_Port:STL_compaction_task] All compacitons done. To sleep\r\n")
TRACE_MSG(COMPACTION_TASK_97_112_2_18_1_57_9_4,"[XSR_Port:_do_compaction] Compact %d (0x%x)rn")
END_TRACE_MAP(MS_REF_XSR_LAYER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_XSR_LAYER_TRC_H_


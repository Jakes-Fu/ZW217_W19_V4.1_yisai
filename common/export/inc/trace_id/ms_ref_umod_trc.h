/******************************************************************************
 ** File Name:      ms_ref_umod_trc.h                                         *
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
//trace_id:75
#ifndef _MS_REF_UMOD_TRC_H_
#define _MS_REF_UMOD_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define UMOD_HANDLER_149_112_2_18_1_55_12_0 "UMOD_handler.c ep1 intoken ack timeout"
#define UMOD_HANDLER_199_112_2_18_1_55_12_1 "UMOD_handler.c ep2 intoken ack timeout"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_UMOD_TRC)
TRACE_MSG(UMOD_HANDLER_149_112_2_18_1_55_12_0,"UMOD_handler.c ep1 intoken ack timeout")
TRACE_MSG(UMOD_HANDLER_199_112_2_18_1_55_12_1,"UMOD_handler.c ep2 intoken ack timeout")
END_TRACE_MAP(MS_REF_UMOD_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_UMOD_TRC_H_


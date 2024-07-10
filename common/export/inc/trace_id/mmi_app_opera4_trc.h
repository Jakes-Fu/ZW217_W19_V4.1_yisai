/******************************************************************************
 ** File Name:      mmi_app_opera4_trc.h                                         *
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
//trace_id:245
#ifndef _MMI_APP_OPERA4_TRC_H_
#define _MMI_APP_OPERA4_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define OPDEVDEBUG_93_112_2_18_12_21_28_0 "%s"
#define OPDEVDEBUG_98_112_2_18_12_21_28_1 "%s"
#define OPDEVMEMORY_22_112_2_18_12_21_37_2 "op:OD_MemoryCreatePool minsize=%d default_size=%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_OPERA4_TRC)
TRACE_MSG(OPDEVDEBUG_93_112_2_18_12_21_28_0,"%s")
TRACE_MSG(OPDEVDEBUG_98_112_2_18_12_21_28_1,"%s")
TRACE_MSG(OPDEVMEMORY_22_112_2_18_12_21_37_2,"op:OD_MemoryCreatePool minsize=%d default_size=%d")
END_TRACE_MAP(MMI_APP_OPERA4_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_OPERA4_TRC_H_


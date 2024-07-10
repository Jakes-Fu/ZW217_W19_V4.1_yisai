/******************************************************************************
 ** File Name:      ms_ref_rotation_trc.h                                         *
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
//trace_id:62
#ifndef _MS_REF_ROTATION_TRC_H_
#define _MS_REF_ROTATION_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define ROTATION_APP_361_112_2_18_1_49_44_0 "Rotation: the param ptr is null error"
#define ROTATION_APP_372_112_2_18_1_49_44_1 "Rotation: the add not algin error"
#define ROTATION_APP_378_112_2_18_1_49_44_2 "Rotation: data for error : %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_ROTATION_TRC)
TRACE_MSG(ROTATION_APP_361_112_2_18_1_49_44_0,"Rotation: the param ptr is null error")
TRACE_MSG(ROTATION_APP_372_112_2_18_1_49_44_1,"Rotation: the add not algin error")
TRACE_MSG(ROTATION_APP_378_112_2_18_1_49_44_2,"Rotation: data for error : %d")
END_TRACE_MAP(MS_REF_ROTATION_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_ROTATION_TRC_H_


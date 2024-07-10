/******************************************************************************
 ** File Name:      ms_ref_disk_trc.h                                         *
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
//trace_id:43
#ifndef _MS_REF_DISK_TRC_H_
#define _MS_REF_DISK_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define DISK_API_234_112_2_18_1_24_39_0 "DISK_Create Handle:0x%x,start:0x%X,size:0x%X\r\n"
#define DISK_API_235_112_2_18_1_24_39_1 "DISK  Name:%s,type:0x%X, type:0x%X\r\n"
#define DISK_API_255_112_2_18_1_24_39_2 "DISK_Delete Handle:0x%x"
#define DISK_API_259_112_2_18_1_24_39_3 "DISK_Delete Remove:ret 0x%x, suc:0x%x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_DISK_TRC)
TRACE_MSG(DISK_API_234_112_2_18_1_24_39_0,"DISK_Create Handle:0x%x,start:0x%X,size:0x%X\r\n")
TRACE_MSG(DISK_API_235_112_2_18_1_24_39_1,"DISK  Name:%s,type:0x%X, type:0x%X\r\n")
TRACE_MSG(DISK_API_255_112_2_18_1_24_39_2,"DISK_Delete Handle:0x%x")
TRACE_MSG(DISK_API_259_112_2_18_1_24_39_3,"DISK_Delete Remove:ret 0x%x, suc:0x%x")
END_TRACE_MAP(MS_REF_DISK_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_DISK_TRC_H_


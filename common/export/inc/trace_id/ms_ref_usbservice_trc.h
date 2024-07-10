/******************************************************************************
 ** File Name:      ms_ref_usbservice_trc.h                                         *
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
//trace_id:77
#ifndef _MS_REF_USBSERVICE_TRC_H_
#define _MS_REF_USBSERVICE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define USBSERVICE_MAIN_214_112_2_18_1_55_27_0 "\nUSB_MainThread signal error\n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_USBSERVICE_TRC)
TRACE_MSG(USBSERVICE_MAIN_214_112_2_18_1_55_27_0,"\nUSB_MainThread signal error\n")
END_TRACE_MAP(MS_REF_USBSERVICE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_USBSERVICE_TRC_H_


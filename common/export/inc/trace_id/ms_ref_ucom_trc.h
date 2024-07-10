/******************************************************************************
 ** File Name:      ms_ref_ucom_trc.h                                         *
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
//trace_id:73
#ifndef _MS_REF_UCOM_TRC_H_
#define _MS_REF_UCOM_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define UCOM_DRV_91_112_2_18_1_54_53_0 "\nUCOM_GetDevDescriptor\n"
#define UCOM_DRV_244_112_2_18_1_54_53_1 "\nUCOM_VendorCommand\n"
#define UCOM_DRV_263_112_2_18_1_54_53_2 "\nUCOM_ClassCommand 0x%x 0x%x\n"
#define UCOM_MAIN_150_112_2_18_1_54_54_3 "ucom send data %d recv data %d  in 2s"
#define UCOM_MAIN_369_112_2_18_1_54_55_4 "device_id = %d buffer 0x%x length %d \n"
#define UCOM_UBOT_STORAGE_168_112_2_18_1_55_0_5 "\nUBOT_ProcCBW() err\n"
#define UCOM_UBOT_STORAGE_178_112_2_18_1_55_0_6 "UCOM_UBOT UPM CBD Err is 0x%x"
#define UCOM_UBOT_STORAGE_226_112_2_18_1_55_0_7 "UCOM_UBOT CBD_0x%x_NOREG_FUCTION"
#define UCOM_UBOT_STORAGE_243_112_2_18_1_55_0_8 "UCOM_UBOT UBOT_CBD_UNDEFINED_CMD is 0x%x"
#define UCOM_UBOT_STORAGE_290_112_2_18_1_55_0_9 "UCOM_UBOT cbw cmd write error %x"
#define UCOM_UBOT_STORAGE_321_112_2_18_1_55_0_10 "UCOM_UBOT_ProcCSW"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_UCOM_TRC)
TRACE_MSG(UCOM_DRV_91_112_2_18_1_54_53_0,"\nUCOM_GetDevDescriptor\n")
TRACE_MSG(UCOM_DRV_244_112_2_18_1_54_53_1,"\nUCOM_VendorCommand\n")
TRACE_MSG(UCOM_DRV_263_112_2_18_1_54_53_2,"\nUCOM_ClassCommand 0x%x 0x%x\n")
TRACE_MSG(UCOM_MAIN_150_112_2_18_1_54_54_3,"ucom send data %d recv data %d  in 2s")
TRACE_MSG(UCOM_MAIN_369_112_2_18_1_54_55_4,"device_id = %d buffer 0x%x length %d \n")
TRACE_MSG(UCOM_UBOT_STORAGE_168_112_2_18_1_55_0_5,"\nUBOT_ProcCBW() err\n")
TRACE_MSG(UCOM_UBOT_STORAGE_178_112_2_18_1_55_0_6,"UCOM_UBOT UPM CBD Err is 0x%x")
TRACE_MSG(UCOM_UBOT_STORAGE_226_112_2_18_1_55_0_7,"UCOM_UBOT CBD_0x%x_NOREG_FUCTION")
TRACE_MSG(UCOM_UBOT_STORAGE_243_112_2_18_1_55_0_8,"UCOM_UBOT UBOT_CBD_UNDEFINED_CMD is 0x%x")
TRACE_MSG(UCOM_UBOT_STORAGE_290_112_2_18_1_55_0_9,"UCOM_UBOT cbw cmd write error %x")
TRACE_MSG(UCOM_UBOT_STORAGE_321_112_2_18_1_55_0_10,"UCOM_UBOT_ProcCSW")
END_TRACE_MAP(MS_REF_UCOM_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_UCOM_TRC_H_


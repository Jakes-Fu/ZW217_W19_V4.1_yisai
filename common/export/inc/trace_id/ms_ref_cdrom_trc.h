/******************************************************************************
 ** File Name:      ms_ref_cdrom_trc.h                                         *
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
//trace_id:40
#ifndef _MS_REF_CDROM_TRC_H_
#define _MS_REF_CDROM_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define CDROM_CONFIG_169_112_2_18_1_8_1_0 "CDROM_CustomerConfig State=FALSE"
#define CDROM_CONFIG_182_112_2_18_1_8_1_1 "CDROM_CustomerConfig no device exist"
#define CDROM_DRV_73_112_2_18_1_8_1_2 "\nUCOM_UBOT_ResetCache\n"
#define CDROM_DRV_132_112_2_18_1_8_2_3 "\nUCOM_UBOT_WriteCache\n"
#define CDROM_DRV_149_112_2_18_1_8_2_4 "\nUCOM_UBOT_SendCache\n"
#define CDROM_DRV_172_112_2_18_1_8_2_5 "UCOM_UBOT  send data %d start\r\n"
#define CDROM_DRV_256_112_2_18_1_8_2_6 "\nUCOM_UBOT_ReadDataFromBSD\n"
#define CDROM_DRV_312_112_2_18_1_8_2_7 "\nUCOM_UBOT_WriteDataToBSD\n"
#define CDROM_DRV_337_112_2_18_1_8_2_8 "UCOM_UBOT donot connect"
#define CDROM_MAIN_80_112_2_18_1_8_2_9 "CDROM get maxlun error"
#define CDROM_STORAGE_168_112_2_18_1_8_6_10 "\nUBOT_ProcCBW() err\n"
#define CDROM_STORAGE_178_112_2_18_1_8_6_11 "CDROM UPM CBD Err is 0x%x"
#define CDROM_STORAGE_226_112_2_18_1_8_6_12 "CDROM CBD_0x%x_NOREG_FUCTION"
#define CDROM_STORAGE_243_112_2_18_1_8_6_13 "CDROM UBOT_CBD_UNDEFINED_CMD is 0x%x"
#define CDROM_STORAGE_290_112_2_18_1_8_6_14 "CDROM cbw cmd write error %x"
#define CDROM_STORAGE_321_112_2_18_1_8_6_15 "CDROM_ProcCSW"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_CDROM_TRC)
TRACE_MSG(CDROM_CONFIG_169_112_2_18_1_8_1_0,"CDROM_CustomerConfig State=FALSE")
TRACE_MSG(CDROM_CONFIG_182_112_2_18_1_8_1_1,"CDROM_CustomerConfig no device exist")
TRACE_MSG(CDROM_DRV_73_112_2_18_1_8_1_2,"\nUCOM_UBOT_ResetCache\n")
TRACE_MSG(CDROM_DRV_132_112_2_18_1_8_2_3,"\nUCOM_UBOT_WriteCache\n")
TRACE_MSG(CDROM_DRV_149_112_2_18_1_8_2_4,"\nUCOM_UBOT_SendCache\n")
TRACE_MSG(CDROM_DRV_172_112_2_18_1_8_2_5,"UCOM_UBOT  send data %d start\r\n")
TRACE_MSG(CDROM_DRV_256_112_2_18_1_8_2_6,"\nUCOM_UBOT_ReadDataFromBSD\n")
TRACE_MSG(CDROM_DRV_312_112_2_18_1_8_2_7,"\nUCOM_UBOT_WriteDataToBSD\n")
TRACE_MSG(CDROM_DRV_337_112_2_18_1_8_2_8,"UCOM_UBOT donot connect")
TRACE_MSG(CDROM_MAIN_80_112_2_18_1_8_2_9,"CDROM get maxlun error")
TRACE_MSG(CDROM_STORAGE_168_112_2_18_1_8_6_10,"\nUBOT_ProcCBW() err\n")
TRACE_MSG(CDROM_STORAGE_178_112_2_18_1_8_6_11,"CDROM UPM CBD Err is 0x%x")
TRACE_MSG(CDROM_STORAGE_226_112_2_18_1_8_6_12,"CDROM CBD_0x%x_NOREG_FUCTION")
TRACE_MSG(CDROM_STORAGE_243_112_2_18_1_8_6_13,"CDROM UBOT_CBD_UNDEFINED_CMD is 0x%x")
TRACE_MSG(CDROM_STORAGE_290_112_2_18_1_8_6_14,"CDROM cbw cmd write error %x")
TRACE_MSG(CDROM_STORAGE_321_112_2_18_1_8_6_15,"CDROM_ProcCSW")
END_TRACE_MAP(MS_REF_CDROM_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_CDROM_TRC_H_


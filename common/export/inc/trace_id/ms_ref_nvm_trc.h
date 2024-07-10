/******************************************************************************
 ** File Name:      ms_ref_nvm_trc.h                                         *
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
//trace_id:61
#ifndef _MS_REF_NVM_TRC_H_
#define _MS_REF_NVM_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define FOTA_OPERATION_42_112_2_18_1_49_30_0 "[FOTA_Open] IN \r\n"
#define FOTA_OPERATION_47_112_2_18_1_49_30_1 "[FOTA_Open] BSD_Open failed(0x%x)\r\n"
#define FOTA_OPERATION_53_112_2_18_1_49_30_2 "[FOTA_Open] BSD_GetDeviceByHandle failed(0x%x)\r\n"
#define FOTA_OPERATION_58_112_2_18_1_49_30_3 "[FOTA_Open] :total sector %d  sector_size %d\n"
#define FOTA_OPERATION_76_112_2_18_1_49_30_4 "[FOTA_Erase] IN \r\n"
#define FOTA_OPERATION_91_112_2_18_1_49_30_5 "[FOTA_read] IN \r\n"
#define FOTA_OPERATION_138_112_2_18_1_49_30_6 "[FOTA_Write] IN 0x%x \r\n"
#define FOTA_OPERATION_201_112_2_18_1_49_31_7 "[FOTA_Write] OUT 0x%x \r\n"
#define FOTA_OPERATION_207_112_2_18_1_49_31_8 "[NV:NVD_Close] IN ()\r\n"
#define FOTA_OPERATION_267_112_2_18_1_49_31_9 "[FOTA_SetUpdateFlag] BSD_GetDeviceByHandle failed(0x%x)\r\n"
#define FOTA_OPERATION_298_112_2_18_1_49_31_10 "[FOTA_SetUpdateFlag] BSD_GetDeviceByHandle failed(0x%x)\r\n"
#define FOTA_OPERATION_318_112_2_18_1_49_31_11 "[FOTA_SetUpdateFlag] BSD_GetDeviceByHandle failed(0x%x)\r\n"
#define FOTA_OPERATION_330_112_2_18_1_49_31_12 "Unknow operation id!!!\n"
#define FOTA_OPERATION_347_112_2_18_1_49_31_13 "device name %s start %d  offset %d"
#define FOTA_OPERATION_349_112_2_18_1_49_31_14 "[FOTA_SetUpdateFlag] BSD_GetDeviceByHandle failed(0x%x)\r\n"
#define FOTA_OPERATION_363_112_2_18_1_49_31_15 "Invalid FOTA flag0 0x%08x"
#define FOTA_OPERATION_370_112_2_18_1_49_31_16 "Invalid FOTA flag1 0x%08x"
#define FOTA_OPERATION_377_112_2_18_1_49_31_17 "Update success  flag1 0x%08x"
#define FOTA_OPERATION_382_112_2_18_1_49_31_18 "Update failure  flag1 0x%08x"
#define NV_DEVICE_865_112_2_18_1_49_34_19 "Create item %d addr = %d"
#define NVPARAM_882_112_2_18_1_49_40_20 "[NVITEM:NVParam_GetTimestampOfNVPart]fail to alloc devhandle\n"
#define NVPARAM_888_112_2_18_1_49_40_21 "[NVITEM:NVParam_GetTimestampOfNVPart]fail to open dev\n"
#define NVPARAM_895_112_2_18_1_49_40_22 "[NVITEM:NVParam_GetTimestampOfNVPart]fail to read dev\n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_NVM_TRC)
TRACE_MSG(FOTA_OPERATION_42_112_2_18_1_49_30_0,"[FOTA_Open] IN \r\n")
TRACE_MSG(FOTA_OPERATION_47_112_2_18_1_49_30_1,"[FOTA_Open] BSD_Open failed(0x%x)\r\n")
TRACE_MSG(FOTA_OPERATION_53_112_2_18_1_49_30_2,"[FOTA_Open] BSD_GetDeviceByHandle failed(0x%x)\r\n")
TRACE_MSG(FOTA_OPERATION_58_112_2_18_1_49_30_3,"[FOTA_Open] :total sector %d  sector_size %d\n")
TRACE_MSG(FOTA_OPERATION_76_112_2_18_1_49_30_4,"[FOTA_Erase] IN \r\n")
TRACE_MSG(FOTA_OPERATION_91_112_2_18_1_49_30_5,"[FOTA_read] IN \r\n")
TRACE_MSG(FOTA_OPERATION_138_112_2_18_1_49_30_6,"[FOTA_Write] IN 0x%x \r\n")
TRACE_MSG(FOTA_OPERATION_201_112_2_18_1_49_31_7,"[FOTA_Write] OUT 0x%x \r\n")
TRACE_MSG(FOTA_OPERATION_207_112_2_18_1_49_31_8,"[NV:NVD_Close] IN ()\r\n")
TRACE_MSG(FOTA_OPERATION_267_112_2_18_1_49_31_9,"[FOTA_SetUpdateFlag] BSD_GetDeviceByHandle failed(0x%x)\r\n")
TRACE_MSG(FOTA_OPERATION_298_112_2_18_1_49_31_10,"[FOTA_SetUpdateFlag] BSD_GetDeviceByHandle failed(0x%x)\r\n")
TRACE_MSG(FOTA_OPERATION_318_112_2_18_1_49_31_11,"[FOTA_SetUpdateFlag] BSD_GetDeviceByHandle failed(0x%x)\r\n")
TRACE_MSG(FOTA_OPERATION_330_112_2_18_1_49_31_12,"Unknow operation id!!!\n")
TRACE_MSG(FOTA_OPERATION_347_112_2_18_1_49_31_13,"device name %s start %d  offset %d")
TRACE_MSG(FOTA_OPERATION_349_112_2_18_1_49_31_14,"[FOTA_SetUpdateFlag] BSD_GetDeviceByHandle failed(0x%x)\r\n")
TRACE_MSG(FOTA_OPERATION_363_112_2_18_1_49_31_15,"Invalid FOTA flag0 0x%08x")
TRACE_MSG(FOTA_OPERATION_370_112_2_18_1_49_31_16,"Invalid FOTA flag1 0x%08x")
TRACE_MSG(FOTA_OPERATION_377_112_2_18_1_49_31_17,"Update success  flag1 0x%08x")
TRACE_MSG(FOTA_OPERATION_382_112_2_18_1_49_31_18,"Update failure  flag1 0x%08x")
TRACE_MSG(NV_DEVICE_865_112_2_18_1_49_34_19,"Create item %d addr = %d")
TRACE_MSG(NVPARAM_882_112_2_18_1_49_40_20,"[NVITEM:NVParam_GetTimestampOfNVPart]fail to alloc devhandle\n")
TRACE_MSG(NVPARAM_888_112_2_18_1_49_40_21,"[NVITEM:NVParam_GetTimestampOfNVPart]fail to open dev\n")
TRACE_MSG(NVPARAM_895_112_2_18_1_49_40_22,"[NVITEM:NVParam_GetTimestampOfNVPart]fail to read dev\n")
END_TRACE_MAP(MS_REF_NVM_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_NVM_TRC_H_


/******************************************************************************
 ** File Name:      ms_ref_upcc_trc.h                                         *
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
//trace_id:76
#ifndef _MS_REF_UPCC_TRC_H_
#define _MS_REF_UPCC_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define UPCC_CAMERA_145_112_2_18_1_55_16_0 "UPCC_camera.c:SetVideoStartFlag:set %d."
#define UPCC_CAMERA_194_112_2_18_1_55_16_1 "UPCC_NoticeUpcc: Drop one frame !!"
#define UPCC_CAMERA_242_112_2_18_1_55_16_2 "UPCC_camera.c:UPCC_SetALLSensorEffect:set contrast fail."
#define UPCC_CAMERA_249_112_2_18_1_55_16_3 "UPCC_camera.c:UPCC_SetALLSensorEffect:set brightness fail."
#define UPCC_CAMERA_256_112_2_18_1_55_16_4 "UPCC_camera.c:UPCC_SetALLSensorEffect:set effect fail."
#define UPCC_CAMERA_260_112_2_18_1_55_16_5 "UPCC_camera.c:UPCC_SetALLSensorEffect:success."
#define UPCC_CAMERA_357_112_2_18_1_55_16_6 "UPCC_camera.c:UPCC_SetVideoStartFlagByHost:set %d."
#define UPCC_CAMERA_397_112_2_18_1_55_16_7 "upcc_camera:UPCC_InitPCCamera"
#define UPCC_CAMERA_417_112_2_18_1_55_16_8 "UPCC_StartVideoStreaming: Open DC fail !!"
#define UPCC_CAMERA_437_112_2_18_1_55_16_9 "UPCC_camera.c:UPCC_StopVideoStreaming fail."
#define UPCC_CAMERA_444_112_2_18_1_55_16_10 "UPCC_camera.c:DC_Close fail."
#define UPCC_CAMERA_476_112_2_18_1_55_16_11 "-- UPCC_StartVideoStreaming(): Already run. ---"
#define UPCC_CAMERA_485_112_2_18_1_55_16_12 "UPCC_StartVideoStreaming: Open DC fail !!"
#define UPCC_CAMERA_495_112_2_18_1_55_16_13 "---------UPCC_StartVideoStreaming()--------"
#define UPCC_CAMERA_507_112_2_18_1_55_16_14 "UPCC_StartVideoStreaming:  DCAMERA_DATA_TYPE_YUV"
#define UPCC_CAMERA_512_112_2_18_1_55_16_15 "UPCC_StartVideoStreaming:  DCAMERA_DATA_TYPE_JPEG"
#define UPCC_CAMERA_517_112_2_18_1_55_16_16 "UPCC_StartVideoStreaming:  DCAMERA_DATA_TYPE_JPEG"
#define UPCC_CAMERA_550_112_2_18_1_55_16_17 "UPCC_camera.c:UPCC_SetALLSensorEffect wrong."
#define UPCC_CAMERA_569_112_2_18_1_55_16_18 "UPCC_camera.c:UPCC_StopVideoStreaming:video is not start."
#define UPCC_CAMERA_579_112_2_18_1_55_16_19 "UPCC_camera.c:UPCC_StopVideoStreaming success."
#define UPCC_CAMERA_595_112_2_18_1_55_16_20 "UPCC_camera.c:UPCC_SetBrightnessValue:invalid value."
#define UPCC_CAMERA_599_112_2_18_1_55_17_21 "UPCC_camera.c:UPCC_SetBrightnessValue:value = %d"
#define UPCC_CAMERA_608_112_2_18_1_55_17_22 "UPCC_camera.c:UPCC_SetBrightnessValue fail."
#define UPCC_CAMERA_646_112_2_18_1_55_17_23 "UPCC_camer.c:UPCC_GetBrightnessValue:invalid request"
#define UPCC_CAMERA_664_112_2_18_1_55_17_24 "UPCC_camera.c:UPCC_SetContrastValue:invalid value."
#define UPCC_CAMERA_668_112_2_18_1_55_17_25 "UPCC_camera.c:UPCC_SetContrastValue:value = %d"
#define UPCC_CAMERA_676_112_2_18_1_55_17_26 "UPCC_camera.c:UPCC_SetContrastValue fail."
#define UPCC_CAMERA_714_112_2_18_1_55_17_27 "UPCC_camer.c:UPCC_GetContrastValue:invalid request"
#define UPCC_CAMERA_731_112_2_18_1_55_17_28 "UPCC_camera.c:UPCC_SetHueValue:invalid value."
#define UPCC_CAMERA_735_112_2_18_1_55_17_29 "UPCC_camera.c:UPCC_SetHueValue:value = %d"
#define UPCC_CAMERA_743_112_2_18_1_55_17_30 "UPCC_camera.c:UPCC_SetHueValue fail."
#define UPCC_CAMERA_778_112_2_18_1_55_17_31 "UPCC_camera.c:UPCC_SetFrameSizePattern:invalid value."
#define UPCC_CAMERA_782_112_2_18_1_55_17_32 "UPCC_camera.c:UPCC_SetFrameSizePattern:value = %d"
#define UPCC_CAMERA_808_112_2_18_1_55_17_33 "UPCC_camera.c:UPCC_SetFrameRate:invalid value."
#define UPCC_CAMERA_812_112_2_18_1_55_17_34 "UPCC_camera.c:UPCC_SetFrameRate:value = %d"
#define UPCC_CAMERA_820_112_2_18_1_55_17_35 "UPCC_camera.c:UPCC_SetFrameRate fail."
#define UPCC_DRV_873_112_2_18_1_55_20_36 "UPCC_SetCacheLockFlag: Get event Timer out!!"
#define UPCC_DRV_972_112_2_18_1_55_20_37 "UPCC_drv.c:UPCC_MakeCacheData:the cache is locked."
#define UPCC_DRV_1093_112_2_18_1_55_21_38 "UPCC_drv.c:UPCC_SendCache:fail."
#define UPCC_DRV_1097_112_2_18_1_55_21_39 "UPCC_SendCache: size = 0x%x, buf_addr = 0x%x"
#define UPCC_DRV_1135_112_2_18_1_55_21_40 "UPCC_drv.c:UPCC_MakeCacheData:the cache is locked."
#define UPCC_DRV_1180_112_2_18_1_55_21_41 "UPCC_drv.c:UPCC_SendAFrame:send cache fail."
#define UPCC_MAIN_241_112_2_18_1_55_22_42 "\ncom_bind() alloc ctrl req fail\n"
#define UPCC_REQUEST_279_112_2_18_1_55_23_43 "UPCC_request.c:HandleBrightness:set cur value fail."
#define UPCC_REQUEST_300_112_2_18_1_55_23_44 "UPCC_request.c:HandleBrightness:invalid request"
#define UPCC_REQUEST_323_112_2_18_1_55_23_45 "UPCC_request.c:HandleContrast:set cur value fail."
#define UPCC_REQUEST_342_112_2_18_1_55_23_46 "UPCC_request.c:HandleContrast:invalid request"
#define UPCC_REQUEST_365_112_2_18_1_55_23_47 "UPCC_request.c:HandleHue:set cur value fail."
#define UPCC_REQUEST_381_112_2_18_1_55_23_48 "UPCC_request.c:HandleHue:invalid request"
#define UPCC_REQUEST_417_112_2_18_1_55_23_49 "UPCC_request.c:HandleFrameSizePattern:invalid request"
#define UPCC_REQUEST_453_112_2_18_1_55_23_50 "UPCC_request.c:HandleFrameRate:invalid request"
#define UPCC_REQUEST_469_112_2_18_1_55_24_51 "------HandleVideoStreamingControl(start=%d)------"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_UPCC_TRC)
TRACE_MSG(UPCC_CAMERA_145_112_2_18_1_55_16_0,"UPCC_camera.c:SetVideoStartFlag:set %d.")
TRACE_MSG(UPCC_CAMERA_194_112_2_18_1_55_16_1,"UPCC_NoticeUpcc: Drop one frame !!")
TRACE_MSG(UPCC_CAMERA_242_112_2_18_1_55_16_2,"UPCC_camera.c:UPCC_SetALLSensorEffect:set contrast fail.")
TRACE_MSG(UPCC_CAMERA_249_112_2_18_1_55_16_3,"UPCC_camera.c:UPCC_SetALLSensorEffect:set brightness fail.")
TRACE_MSG(UPCC_CAMERA_256_112_2_18_1_55_16_4,"UPCC_camera.c:UPCC_SetALLSensorEffect:set effect fail.")
TRACE_MSG(UPCC_CAMERA_260_112_2_18_1_55_16_5,"UPCC_camera.c:UPCC_SetALLSensorEffect:success.")
TRACE_MSG(UPCC_CAMERA_357_112_2_18_1_55_16_6,"UPCC_camera.c:UPCC_SetVideoStartFlagByHost:set %d.")
TRACE_MSG(UPCC_CAMERA_397_112_2_18_1_55_16_7,"upcc_camera:UPCC_InitPCCamera")
TRACE_MSG(UPCC_CAMERA_417_112_2_18_1_55_16_8,"UPCC_StartVideoStreaming: Open DC fail !!")
TRACE_MSG(UPCC_CAMERA_437_112_2_18_1_55_16_9,"UPCC_camera.c:UPCC_StopVideoStreaming fail.")
TRACE_MSG(UPCC_CAMERA_444_112_2_18_1_55_16_10,"UPCC_camera.c:DC_Close fail.")
TRACE_MSG(UPCC_CAMERA_476_112_2_18_1_55_16_11,"-- UPCC_StartVideoStreaming(): Already run. ---")
TRACE_MSG(UPCC_CAMERA_485_112_2_18_1_55_16_12,"UPCC_StartVideoStreaming: Open DC fail !!")
TRACE_MSG(UPCC_CAMERA_495_112_2_18_1_55_16_13,"---------UPCC_StartVideoStreaming()--------")
TRACE_MSG(UPCC_CAMERA_507_112_2_18_1_55_16_14,"UPCC_StartVideoStreaming:  DCAMERA_DATA_TYPE_YUV")
TRACE_MSG(UPCC_CAMERA_512_112_2_18_1_55_16_15,"UPCC_StartVideoStreaming:  DCAMERA_DATA_TYPE_JPEG")
TRACE_MSG(UPCC_CAMERA_517_112_2_18_1_55_16_16,"UPCC_StartVideoStreaming:  DCAMERA_DATA_TYPE_JPEG")
TRACE_MSG(UPCC_CAMERA_550_112_2_18_1_55_16_17,"UPCC_camera.c:UPCC_SetALLSensorEffect wrong.")
TRACE_MSG(UPCC_CAMERA_569_112_2_18_1_55_16_18,"UPCC_camera.c:UPCC_StopVideoStreaming:video is not start.")
TRACE_MSG(UPCC_CAMERA_579_112_2_18_1_55_16_19,"UPCC_camera.c:UPCC_StopVideoStreaming success.")
TRACE_MSG(UPCC_CAMERA_595_112_2_18_1_55_16_20,"UPCC_camera.c:UPCC_SetBrightnessValue:invalid value.")
TRACE_MSG(UPCC_CAMERA_599_112_2_18_1_55_17_21,"UPCC_camera.c:UPCC_SetBrightnessValue:value = %d")
TRACE_MSG(UPCC_CAMERA_608_112_2_18_1_55_17_22,"UPCC_camera.c:UPCC_SetBrightnessValue fail.")
TRACE_MSG(UPCC_CAMERA_646_112_2_18_1_55_17_23,"UPCC_camer.c:UPCC_GetBrightnessValue:invalid request")
TRACE_MSG(UPCC_CAMERA_664_112_2_18_1_55_17_24,"UPCC_camera.c:UPCC_SetContrastValue:invalid value.")
TRACE_MSG(UPCC_CAMERA_668_112_2_18_1_55_17_25,"UPCC_camera.c:UPCC_SetContrastValue:value = %d")
TRACE_MSG(UPCC_CAMERA_676_112_2_18_1_55_17_26,"UPCC_camera.c:UPCC_SetContrastValue fail.")
TRACE_MSG(UPCC_CAMERA_714_112_2_18_1_55_17_27,"UPCC_camer.c:UPCC_GetContrastValue:invalid request")
TRACE_MSG(UPCC_CAMERA_731_112_2_18_1_55_17_28,"UPCC_camera.c:UPCC_SetHueValue:invalid value.")
TRACE_MSG(UPCC_CAMERA_735_112_2_18_1_55_17_29,"UPCC_camera.c:UPCC_SetHueValue:value = %d")
TRACE_MSG(UPCC_CAMERA_743_112_2_18_1_55_17_30,"UPCC_camera.c:UPCC_SetHueValue fail.")
TRACE_MSG(UPCC_CAMERA_778_112_2_18_1_55_17_31,"UPCC_camera.c:UPCC_SetFrameSizePattern:invalid value.")
TRACE_MSG(UPCC_CAMERA_782_112_2_18_1_55_17_32,"UPCC_camera.c:UPCC_SetFrameSizePattern:value = %d")
TRACE_MSG(UPCC_CAMERA_808_112_2_18_1_55_17_33,"UPCC_camera.c:UPCC_SetFrameRate:invalid value.")
TRACE_MSG(UPCC_CAMERA_812_112_2_18_1_55_17_34,"UPCC_camera.c:UPCC_SetFrameRate:value = %d")
TRACE_MSG(UPCC_CAMERA_820_112_2_18_1_55_17_35,"UPCC_camera.c:UPCC_SetFrameRate fail.")
TRACE_MSG(UPCC_DRV_873_112_2_18_1_55_20_36,"UPCC_SetCacheLockFlag: Get event Timer out!!")
TRACE_MSG(UPCC_DRV_972_112_2_18_1_55_20_37,"UPCC_drv.c:UPCC_MakeCacheData:the cache is locked.")
TRACE_MSG(UPCC_DRV_1093_112_2_18_1_55_21_38,"UPCC_drv.c:UPCC_SendCache:fail.")
TRACE_MSG(UPCC_DRV_1097_112_2_18_1_55_21_39,"UPCC_SendCache: size = 0x%x, buf_addr = 0x%x")
TRACE_MSG(UPCC_DRV_1135_112_2_18_1_55_21_40,"UPCC_drv.c:UPCC_MakeCacheData:the cache is locked.")
TRACE_MSG(UPCC_DRV_1180_112_2_18_1_55_21_41,"UPCC_drv.c:UPCC_SendAFrame:send cache fail.")
TRACE_MSG(UPCC_MAIN_241_112_2_18_1_55_22_42,"\ncom_bind() alloc ctrl req fail\n")
TRACE_MSG(UPCC_REQUEST_279_112_2_18_1_55_23_43,"UPCC_request.c:HandleBrightness:set cur value fail.")
TRACE_MSG(UPCC_REQUEST_300_112_2_18_1_55_23_44,"UPCC_request.c:HandleBrightness:invalid request")
TRACE_MSG(UPCC_REQUEST_323_112_2_18_1_55_23_45,"UPCC_request.c:HandleContrast:set cur value fail.")
TRACE_MSG(UPCC_REQUEST_342_112_2_18_1_55_23_46,"UPCC_request.c:HandleContrast:invalid request")
TRACE_MSG(UPCC_REQUEST_365_112_2_18_1_55_23_47,"UPCC_request.c:HandleHue:set cur value fail.")
TRACE_MSG(UPCC_REQUEST_381_112_2_18_1_55_23_48,"UPCC_request.c:HandleHue:invalid request")
TRACE_MSG(UPCC_REQUEST_417_112_2_18_1_55_23_49,"UPCC_request.c:HandleFrameSizePattern:invalid request")
TRACE_MSG(UPCC_REQUEST_453_112_2_18_1_55_23_50,"UPCC_request.c:HandleFrameRate:invalid request")
TRACE_MSG(UPCC_REQUEST_469_112_2_18_1_55_24_51,"------HandleVideoStreamingControl(start=%d)------")
END_TRACE_MAP(MS_REF_UPCC_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_UPCC_TRC_H_


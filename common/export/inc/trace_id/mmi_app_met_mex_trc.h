/******************************************************************************
 ** File Name:      mmi_app_met_mex_trc.h                                         *
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
//trace_id:193
#ifndef _MMI_APP_MET_MEX_TRC_H_
#define _MMI_APP_MET_MEX_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MET_MEX_AQ_217_112_2_18_2_35_52_0 "Met_aq trace met_aq_malloc failed "
#define MET_MEX_AQ_238_112_2_18_2_35_52_1 "Met_aq trace  succeed "
#define MET_MEX_AQ_LIB_1042_112_2_18_2_35_55_2 "%s number %s, block_status %d"
#define MET_MEX_AQ_LIB_1086_112_2_18_2_35_55_3 "%s number %s, block_status %d"
#define MET_MEX_WINTAB_1763_112_2_18_2_36_0_4 "HandleMexSIMSelectWinMsg dual_sys = %d"
#define MET_MEX_WINTAB_1803_112_2_18_2_36_1_5 "Mex_Get_Select_sim,s_dual_sys=%d"
#define MEXCONFIG_10376_112_2_18_2_36_29_6 "MexPhs_RemovePhbEntry result = %d "
#define MEXCONFIG_12814_112_2_18_2_36_34_7 "MEX_CONFIG MexConfig_DeleteSramPool :delete sram before MexConfig_CreateSramPool"
#define MEXINTERFACE_2738_112_2_18_2_36_41_8 "crazy Gsesor CB status none"
#define MEXINTERFACE_2768_112_2_18_2_36_41_9 "crazy Gsesor ax = %d,ay = %d, az = %d,r = %d"
#define MEXINTERFACE_2842_112_2_18_2_36_41_10 "MexPhs_MotionSensor_Start Gsensor win_handle = %d"
#define MEXINTERFACE_2845_112_2_18_2_36_41_11 "MexPhs_MotionSensor_Start GSensor ret = %d"
#define MEXINTERFACE_5086_112_2_18_2_36_46_12 "MexInterface_HandlePsAndRefMsg Msg id=%x"
#define MEXINTERFACE_5090_112_2_18_2_36_46_13 "MexInterface_HandlePsAndRefMsg is not need process"
#define MEXINTERFACE_5100_112_2_18_2_36_46_14 "MexInterface_HandlePsAndRefMsg activate result=%d"
#define MEXINTERFACE_5134_112_2_18_2_36_46_15 "MexInterface_HandlePsAndRefMsg deactive cnf,result=%d detach=%d"
#define MEXINTERFACE_5144_112_2_18_2_36_46_16 "Met deactive ind,result=%d detach=%d"
#define MEXINTERFACE_5168_112_2_18_2_36_46_17 "[MEX]:MexInterface_HandlePDPMsg MMIPDP_ACTIVE_CNF result=%d"
#define MEXINTERFACE_5172_112_2_18_2_36_46_18 "[MEX]:pdp active success!"
#define MEXINTERFACE_5192_112_2_18_2_36_46_19 "[MEX]:pdp active failed!"
#define MEXINTERFACE_5202_112_2_18_2_36_46_20 "[MEX]:pdp active failed in else logic"
#define MEXINTERFACE_5212_112_2_18_2_36_46_21 "[MEX]:MexInterface_HandlePDPMsg: MMIPDP_DEACTIVE_IND"
#define MEXINTERFACE_5535_112_2_18_2_36_47_22 "MexInterface_SetGprsReq Activate err=%d"
#define MEXINTERFACE_5540_112_2_18_2_36_47_23 "MexInterface_SetGprsReq Reset PDP err=%d"
#define MEXINTERFACE_9233_112_2_18_2_36_54_24 "MMIBT_OpenFileCallBack, Enter,is_success = %d!"
#define MEXINTERFACE_9237_112_2_18_2_36_54_25 "MMIBT_OpenFileCallBack, file_num = %d"
#define MEXINTERFACE_11197_112_2_18_2_36_59_26 "[MMIFILE] SetAttr NO memory !"
#define MEXINTERFACE_11800_112_2_18_2_37_0_27 "arm play, AUDIO_CreateStreamBufferHandle, create handle fail!"
#define MEXINTERFACE_12143_112_2_18_2_37_1_28 "GetImageDimension BMP"
#define MEXINTERFACE_12149_112_2_18_2_37_1_29 "GetImageDimension width;%d height:%d"
#define MEXINTERFACE_12154_112_2_18_2_37_1_30 "GetImageDimension PNG"
#define MEXINTERFACE_12160_112_2_18_2_37_1_31 "GetImageDimension width;%d height:%d"
#define MEXINTERFACE_12165_112_2_18_2_37_1_32 "GetImageDimension GIF"
#define MEXINTERFACE_12171_112_2_18_2_37_1_33 "GetImageDimension width;%d height:%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_MET_MEX_TRC)
TRACE_MSG(MET_MEX_AQ_217_112_2_18_2_35_52_0,"Met_aq trace met_aq_malloc failed ")
TRACE_MSG(MET_MEX_AQ_238_112_2_18_2_35_52_1,"Met_aq trace  succeed ")
TRACE_MSG(MET_MEX_AQ_LIB_1042_112_2_18_2_35_55_2,"%s number %s, block_status %d")
TRACE_MSG(MET_MEX_AQ_LIB_1086_112_2_18_2_35_55_3,"%s number %s, block_status %d")
TRACE_MSG(MET_MEX_WINTAB_1763_112_2_18_2_36_0_4,"HandleMexSIMSelectWinMsg dual_sys = %d")
TRACE_MSG(MET_MEX_WINTAB_1803_112_2_18_2_36_1_5,"Mex_Get_Select_sim,s_dual_sys=%d")
TRACE_MSG(MEXCONFIG_10376_112_2_18_2_36_29_6,"MexPhs_RemovePhbEntry result = %d ")
TRACE_MSG(MEXCONFIG_12814_112_2_18_2_36_34_7,"MEX_CONFIG MexConfig_DeleteSramPool :delete sram before MexConfig_CreateSramPool")
TRACE_MSG(MEXINTERFACE_2738_112_2_18_2_36_41_8,"crazy Gsesor CB status none")
TRACE_MSG(MEXINTERFACE_2768_112_2_18_2_36_41_9,"crazy Gsesor ax = %d,ay = %d, az = %d,r = %d")
TRACE_MSG(MEXINTERFACE_2842_112_2_18_2_36_41_10,"MexPhs_MotionSensor_Start Gsensor win_handle = %d")
TRACE_MSG(MEXINTERFACE_2845_112_2_18_2_36_41_11,"MexPhs_MotionSensor_Start GSensor ret = %d")
TRACE_MSG(MEXINTERFACE_5086_112_2_18_2_36_46_12,"MexInterface_HandlePsAndRefMsg Msg id=%x")
TRACE_MSG(MEXINTERFACE_5090_112_2_18_2_36_46_13,"MexInterface_HandlePsAndRefMsg is not need process")
TRACE_MSG(MEXINTERFACE_5100_112_2_18_2_36_46_14,"MexInterface_HandlePsAndRefMsg activate result=%d")
TRACE_MSG(MEXINTERFACE_5134_112_2_18_2_36_46_15,"MexInterface_HandlePsAndRefMsg deactive cnf,result=%d detach=%d")
TRACE_MSG(MEXINTERFACE_5144_112_2_18_2_36_46_16,"Met deactive ind,result=%d detach=%d")
TRACE_MSG(MEXINTERFACE_5168_112_2_18_2_36_46_17,"[MEX]:MexInterface_HandlePDPMsg MMIPDP_ACTIVE_CNF result=%d")
TRACE_MSG(MEXINTERFACE_5172_112_2_18_2_36_46_18,"[MEX]:pdp active success!")
TRACE_MSG(MEXINTERFACE_5192_112_2_18_2_36_46_19,"[MEX]:pdp active failed!")
TRACE_MSG(MEXINTERFACE_5202_112_2_18_2_36_46_20,"[MEX]:pdp active failed in else logic")
TRACE_MSG(MEXINTERFACE_5212_112_2_18_2_36_46_21,"[MEX]:MexInterface_HandlePDPMsg: MMIPDP_DEACTIVE_IND")
TRACE_MSG(MEXINTERFACE_5535_112_2_18_2_36_47_22,"MexInterface_SetGprsReq Activate err=%d")
TRACE_MSG(MEXINTERFACE_5540_112_2_18_2_36_47_23,"MexInterface_SetGprsReq Reset PDP err=%d")
TRACE_MSG(MEXINTERFACE_9233_112_2_18_2_36_54_24,"MMIBT_OpenFileCallBack, Enter,is_success = %d!")
TRACE_MSG(MEXINTERFACE_9237_112_2_18_2_36_54_25,"MMIBT_OpenFileCallBack, file_num = %d")
TRACE_MSG(MEXINTERFACE_11197_112_2_18_2_36_59_26,"[MMIFILE] SetAttr NO memory !")
TRACE_MSG(MEXINTERFACE_11800_112_2_18_2_37_0_27,"arm play, AUDIO_CreateStreamBufferHandle, create handle fail!")
TRACE_MSG(MEXINTERFACE_12143_112_2_18_2_37_1_28,"GetImageDimension BMP")
TRACE_MSG(MEXINTERFACE_12149_112_2_18_2_37_1_29,"GetImageDimension width;%d height:%d")
TRACE_MSG(MEXINTERFACE_12154_112_2_18_2_37_1_30,"GetImageDimension PNG")
TRACE_MSG(MEXINTERFACE_12160_112_2_18_2_37_1_31,"GetImageDimension width;%d height:%d")
TRACE_MSG(MEXINTERFACE_12165_112_2_18_2_37_1_32,"GetImageDimension GIF")
TRACE_MSG(MEXINTERFACE_12171_112_2_18_2_37_1_33,"GetImageDimension width;%d height:%d")
END_TRACE_MAP(MMI_APP_MET_MEX_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_MET_MEX_TRC_H_


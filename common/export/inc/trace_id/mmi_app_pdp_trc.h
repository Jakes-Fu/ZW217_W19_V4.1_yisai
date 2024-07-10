/******************************************************************************
 ** File Name:      mmi_app_pdp_trc.h                                         *
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
//trace_id:207
#ifndef _MMI_APP_PDP_TRC_H_
#define _MMI_APP_PDP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIPDP_API_291_112_2_18_2_44_48_0 "PdpStringCopy, ALLOC FAIL"
#define MMIPDP_API_563_112_2_18_2_44_49_1 "PDP MANAGER callback,app_handler:%d,msg:%d"
#define MMIPDP_API_598_112_2_18_2_44_49_2 "Callback, ALLOC FAIL"
#define MMIPDP_API_651_112_2_18_2_44_49_3 "CreatMsg, MMIPDP_ACTIVE_REQ ALLOC FAIL"
#define MMIPDP_API_667_112_2_18_2_44_49_4 "CreatMsg, MMIPDP_DEACTIVE_REQ ALLOC FAIL"
#define MMIPDP_API_683_112_2_18_2_44_49_5 "CreatMsg, MMIPDP_PS_ACTIVE_CNF ALLOC FAIL"
#define MMIPDP_API_697_112_2_18_2_44_49_6 "CreatMsg, msg_id=%d ALLOC FAIL"
#define MMIPDP_API_713_112_2_18_2_44_49_7 "CreatMsg, msg_id=%d ALLOC FAIL"
#define MMIPDP_API_767_112_2_18_2_44_49_8 "CreatFsmInstance, ALLOC FAIL"
#define MMIPDP_API_1160_112_2_18_2_44_50_9 "PDP FsmStateTrans: cur:%d, trans to:%d"
#define MMIPDP_API_1180_112_2_18_2_44_50_10 "PDP MANAGER FsmDeactived,msg_id:%d,link_id:%d"
#define MMIPDP_API_1201_112_2_18_2_44_50_11 "DEACTIVED, got wrong msg"
#define MMIPDP_API_1223_112_2_18_2_44_50_12 "PDP MANAGER FsmActived,msg_id:%d,link_id:%d"
#define MMIPDP_API_1411_112_2_18_2_44_51_13 "ACTIVED, got wrong msg"
#define MMIPDP_API_1434_112_2_18_2_44_51_14 "PDP MANAGER FsmDeactiving,msg_id:%d,link_id:%d"
#define MMIPDP_API_1533_112_2_18_2_44_51_15 "DEACTIVING, got wrong msg"
#define MMIPDP_API_1558_112_2_18_2_44_51_16 "PDP MANAGER FsmActiving,msg_id:%d,link_id:%d"
#define MMIPDP_API_1703_112_2_18_2_44_51_17 "PDP MANAGER Active fail, ps err=%d"
#define MMIPDP_API_1736_112_2_18_2_44_51_18 "ACTIVING, got wrong msg"
#define MMIPDP_API_1840_112_2_18_2_44_52_19 "PDP MANAGER HandlePdpPsMsg,APP_MN_ACTIVATE_PDP_CONTEXT_CNF apn sim same!"
#define MMIPDP_API_1850_112_2_18_2_44_52_20 "PDP MANAGER HandlePdpPsMsg,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,result:%d,nsapi:%d,pdp_id:%d"
#define MMIPDP_API_1852_112_2_18_2_44_52_21 "PDP MANAGER HandlePdpPsMsg,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,link_id:%d"
#define MMIPDP_API_1866_112_2_18_2_44_52_22 "APP_MN_ACTIVATE_PDP_CONTEXT_CNF err link_id=0"
#define MMIPDP_API_1883_112_2_18_2_44_52_23 "PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,pdp_num:%d,dual_sys:%d"
#define MMIPDP_API_1889_112_2_18_2_44_52_24 "APP_MN_DEACTIVATE_PDP_CONTEXT_CNF deactive wifi"
#define MMIPDP_API_1914_112_2_18_2_44_52_25 "PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,pdp_id:%d"
#define MMIPDP_API_1916_112_2_18_2_44_52_26 "PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,link_id:%d"
#define MMIPDP_API_1927_112_2_18_2_44_52_27 "APP_MN_DEACTIVATE_PDP_CONTEXT_CNF err link_id=0"
#define MMIPDP_API_1950_112_2_18_2_44_52_28 "[YKTEST] pdp detatch!!!"
#define MMIPDP_API_1982_112_2_18_2_44_52_29 "PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_IND,pdp_num:%d"
#define MMIPDP_API_1995_112_2_18_2_44_52_30 "APP_MN_DEACTIVATE_PDP_CONTEXT_IND err link_id=0"
#define MMIPDP_API_2085_112_2_18_2_44_52_31 "PDP MANAGER MMIPDP_SetLinkPdpId,link_id:%d,pdp_id:%d"
#define MMIPDP_API_2126_112_2_18_2_44_52_32 "mmipdp error %s: CFL_ListCreat failed!"
#define MMIPDP_API_2154_112_2_18_2_44_52_33 "PDP MANAGER MMIPDP_HandleTimer: deactive timer,link_id:%d"
#define MMIPDP_API_2169_112_2_18_2_44_52_34 "PDP MANAGER MMIPDP_HandleTimer: reactive timer,link_id:%d"
#define MMIPDP_API_2179_112_2_18_2_44_52_35 "PDP MANAGER MMIPDP_HandleTimer: reactive timer,wrong state"
#define MMIPDP_API_2200_112_2_18_2_44_52_36 "PDP MANAGER MMIPDP_HandleTimer: active timer,pending list"
#define MMIPDP_API_2201_112_2_18_2_44_52_37 "PDP MANAGER Active timeout callback, cannot get link resouce, other higher priority APP is using"
#define MMIPDP_API_2222_112_2_18_2_44_52_38 "PDP MANAGER MMIPDP_HandleTimer: active timer,using list"
#define MMIPDP_API_2223_112_2_18_2_44_52_39 "PDP MANAGER Active timeout callback, no response from PS"
#define MMIPDP_API_2250_112_2_18_2_44_52_40 "!!!!!! app_handler = %d,find_data = %d"
#define MMIPDP_API_2320_112_2_18_2_44_53_41 "mmipdp error %s: pstAppRegInfo is PNULL!"
#define MMIPDP_API_2332_112_2_18_2_44_53_42 "mmipdp error %s: handle_msg_callback is PNULL!app_handler is 0x%x"
#define MMIPDP_API_2335_112_2_18_2_44_53_43 "mmipdp info %s: app_handler=%p,nsapi=0x%x,result=%d"
#define MMIPDP_API_2337_112_2_18_2_44_53_44 "mmipdp info %s: handle_msg_callback exit"
#define MMIPDP_API_2355_112_2_18_2_44_53_45 "mmipdp error %s: loop is overlap!"
#define MMIPDP_API_2358_112_2_18_2_44_53_46 "mmipdp info %s: final, i is %d!"
#define MMIPDP_API_2397_112_2_18_2_44_53_47 "mmipdp error %s: pstAppRegInfo is PNULL!"
#define MMIPDP_API_2409_112_2_18_2_44_53_48 "mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d"
#define MMIPDP_API_2412_112_2_18_2_44_53_49 "mmipdp info %s: handle_msg_callback entry %p"
#define MMIPDP_API_2414_112_2_18_2_44_53_50 "mmipdp info %s: handle_msg_callback exit"
#define MMIPDP_API_2422_112_2_18_2_44_53_51 "mmipdp error %s: loop is overlap!"
#define MMIPDP_API_2463_112_2_18_2_44_53_52 "mmipdp error %s: pstAppRegInfo is PNULL!"
#define MMIPDP_API_2475_112_2_18_2_44_53_53 "mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d"
#define MMIPDP_API_2478_112_2_18_2_44_53_54 "mmipdp info %s: handle_msg_callback entry %p"
#define MMIPDP_API_2480_112_2_18_2_44_53_55 "mmipdp info %s: handle_msg_callback exit"
#define MMIPDP_API_2488_112_2_18_2_44_53_56 "mmipdp error %s: loop is overlap!"
#define MMIPDP_API_2522_112_2_18_2_44_53_57 "mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d"
#define MMIPDP_API_2525_112_2_18_2_44_53_58 "mmipdp info %s: handle_msg_callback entry %p"
#define MMIPDP_API_2527_112_2_18_2_44_53_59 "mmipdp info %s: handle_msg_callback exit"
#define MMIPDP_API_2564_112_2_18_2_44_53_60 "mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d"
#define MMIPDP_API_2567_112_2_18_2_44_53_61 "mmipdp info %s: handle_msg_callback entry %p"
#define MMIPDP_API_2569_112_2_18_2_44_53_62 "mmipdp info %s: handle_msg_callback exit"
#define MMIPDP_API_2599_112_2_18_2_44_53_63 "mmipdp error %s: pstWifiAppInfo is PNULL!"
#define MMIPDP_API_2605_112_2_18_2_44_53_64 "mmipdp error %s: dwWifiRegAppList is zeros!"
#define MMIPDP_API_2612_112_2_18_2_44_53_65 "mmipdp info %s: CFL_ListInsertNode!"
#define MMIPDP_API_2617_112_2_18_2_44_53_66 "mmipdp info %s: CFL_ListUpdateNodeData!"
#define MMIPDP_API_2653_112_2_18_2_44_53_67 "mmipdp error %s: mmipdp_WifiRegApp is abnormal"
#define MMIPDP_API_2660_112_2_18_2_44_53_68 "mmipdp info %s: mmipdp_WifiRegApp is abnormal"
#define MMIPDP_API_2671_112_2_18_2_44_53_69 "mmipdp info %s: wifi already actived"
#define MMIPDP_API_2686_112_2_18_2_44_53_70 "mmipdp info %s: call wifi list_win"
#define MMIPDP_API_2698_112_2_18_2_44_53_71 "mmipdp error %s: app active info is wrong"
#define MMIPDP_API_2719_112_2_18_2_44_53_72 "mmipdp info %s: app_handler=%d deactive Wifi!"
#define MMIPDP_API_2753_112_2_18_2_44_53_73 "mmipdp error %s: pstAppRegInfo is PNULL!"
#define MMIPDP_API_2776_112_2_18_2_44_53_74 "mmipdp error MMIAPIPDP_Active: app_info_ptr is PNULL"
#define MMIPDP_API_2780_112_2_18_2_44_53_75 "mmipdp info MMIAPIPDP_Active: priority:%d, app_handler:0x%x, apn:%s, dual_sys:%d"
#define MMIPDP_API_2784_112_2_18_2_44_53_76 "mmipdp error MMIAPIPDP_Active: handle_msg_callback is PNULL!"
#define MMIPDP_API_2791_112_2_18_2_44_53_77 "mmipdp error MMIAPIPDP_Active: GPRS FDN is forbbiden!"
#define MMIPDP_API_2800_112_2_18_2_44_54_78 "mmipdp info %s: Request WiFi interface"
#define MMIPDP_API_2807_112_2_18_2_44_54_79 "PDP MANAGER MMIAPISET_GetSwitchRatState() = %d!"
#define MMIPDP_API_2817_112_2_18_2_44_54_80 "mmipdp error call is being"
#define MMIPDP_API_2825_112_2_18_2_44_54_81 "mmipdp error MMIAPIPDP_Active: GPRS apn_ptr is PNULL!"
#define MMIPDP_API_2831_112_2_18_2_44_54_82 "mmipdp error MMIAPIPDP_Active: GPRS dual_sys is wrong!"
#define MMIPDP_API_2839_112_2_18_2_44_54_83 "PDP MANAGER MMIAPIPDP_Active,priority:MMIPDP_RESULT_AT_IS_ON"
#define MMIPDP_API_2853_112_2_18_2_44_54_84 "PDP MANAGER MMIAPIPDP_Active,MMICONNECTION_SETTING_DATA_ROAMING_DISABLE"
#define MMIPDP_API_2864_112_2_18_2_44_54_85 "PDP MANAGER MMIAPIPDP_Active,MMIPDP_RESULT_DIFFER_DUALSYS"
#define MMIPDP_API_2872_112_2_18_2_44_54_86 "PDP MANAGER MMIAPIPDP_Active, GprsForce Detach!"
#define MMIPDP_API_2896_112_2_18_2_44_54_87 "PDP MANAGER MMIAPIPDP_Active, the app already exist in pending list."
#define MMIPDP_API_2927_112_2_18_2_44_54_88 "PDP MANAGER MMIAPIPDP_Active,fsm busy"
#define MMIPDP_API_2933_112_2_18_2_44_54_89 "PDP MANAGER MMIAPIPDP_Active, no link resource, add to pending list"
#define MMIPDP_API_2963_112_2_18_2_44_54_90 "PDP MANAGER MMIAPIPDP_Deactive,app_handler:%d"
#define MMIPDP_API_2974_112_2_18_2_44_54_91 "mmipdp error %s: pstAppRegInfo is PNULL!"
#define MMIPDP_API_2996_112_2_18_2_44_54_92 "PDP MANAGER MMIAPIPDP_Deactive, the app is in the pending list."
#define MMIPDP_API_3018_112_2_18_2_44_54_93 "PDP MANAGER MMIAPIPDP_Deactive, the app is not using any link resource!"
#define MMIPDP_API_3031_112_2_18_2_44_54_94 "PDP MANAGER MMIAPIPDP_Deactive,fsm busy"
#define MMIPDP_API_3078_112_2_18_2_44_54_95 "PDP MANAGER MMIAPIPDP_ReleaseAll is called, all app are deactived or quit activing"
#define MMIPDP_API_3118_112_2_18_2_44_54_96 "mmipdp info %s: %d wifi Link state is %d!"
#define MMIPDP_API_3149_112_2_18_2_44_54_97 "MMIAPIPDP_GetFlowrate nv read error"
#define MMIPDP_API_3318_112_2_18_2_44_55_98 "MMIAPIPDP_SubscribeEvent, ALLOC FAIL"
#define MMIPDP_API_3348_112_2_18_2_44_55_99 "PDP MANAGER, reset pdp dual_sys=%d"
#define MMIPDP_API_3388_112_2_18_2_44_55_100 "PDP MANAGER MMIAPIPDP_ResetPdp is called, all app are deactived or quit activing"
#define MMIPDP_API_3393_112_2_18_2_44_55_101 "PDP MANAGER MMIAPIPDP_ResetPdp dual_sys=%d, %d, break!"
#define MMIPDP_API_3426_112_2_18_2_44_55_102 "HandlePdpEvent : %d, dual_sys:%d"
#define MMIPDP_API_3498_112_2_18_2_44_55_103 "MMIPDP_SetGPRSStatus out"
#define MMIPDP_API_3505_112_2_18_2_44_55_104 "MMIPDP_SetGPRSStatus out"
#define MMIPDP_API_3530_112_2_18_2_44_55_105 "MMIAPIPDP_PsIsActivedPdpLinkExist Not Pdp Link Now!"
#define MMIPDP_API_3564_112_2_18_2_44_55_106 "MMIAPIPDP_PsIsPdpLinkExist Not Pdp Link Now!"
#define MMIPDP_API_3597_112_2_18_2_44_55_107 "MMIAPIPDP_GetPdpLinkSIM Not Pdp Link Now!"
#define MMIPDP_API_3693_112_2_18_2_44_55_108 "MMIAPIPDP_IsPermitActivePdp gprs_fdn_gateway_string_ptr i =%d"
#define MMIPDP_PS_ADAP_69_112_2_18_2_44_57_109 "PDP MANAGER SetQos pdp_id:%d"
#define MMIPDP_PS_ADAP_76_112_2_18_2_44_57_110 "PDP MANAGER SetQos MNGPRS_SetQosProfileEx FAIL,%d"
#define MMIPDP_PS_ADAP_82_112_2_18_2_44_57_111 "PDP MANAGER SetQos MNGPRS_SetExtReqQosProfileEx FAIL,%d"
#define MMIPDP_PS_ADAP_131_112_2_18_2_44_57_112 "PDP MANAGER MMIPDP_PsActivePdp RequestPdpId fail"
#define MMIPDP_PS_ADAP_142_112_2_18_2_44_57_113 "PDP MANAGER MMIPDP_PsActivePdp set qos fail"
#define MMIPDP_PS_ADAP_149_112_2_18_2_44_57_114 "PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextEx FAIL,%d"
#define MMIPDP_PS_ADAP_164_112_2_18_2_44_57_115 "PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextPcoEx FAIL,%d"
#define MMIPDP_PS_ADAP_175_112_2_18_2_44_57_116 "PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextPcoEx FAIL,%d"
#define MMIPDP_PS_ADAP_189_112_2_18_2_44_57_117 "PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_ActivatePdpContextEx FAIL:%d"
#define MMIPDP_PS_ADAP_228_112_2_18_2_44_57_118 "PDP MANAGER RequestPdpId fail pdp_id:%d"
#define MMIPDP_PS_ADAP_231_112_2_18_2_44_57_119 "PDP MANAGER RequestPdpId pdp_id:%d"
#define MMIPDP_PS_ADAP_277_112_2_18_2_44_57_120 "PDP MANAGER MMIPDP_PsDeactivePdp MMIPDP_CreatTimer:%d"
#define MMIPDP_PS_ADAP_280_112_2_18_2_44_57_121 "PDP MANAGER MMIPDP_PsDeactivePdp MNGPRS_DeactivatePdpContextEx fail"
#define MMIPDP_PS_ADAP_319_112_2_18_2_44_57_122 "PDP MANAGER MMIPDP_PsGprsAttach, input error"
#define MMIPDP_PS_ADAP_359_112_2_18_2_44_57_123 "PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, input error"
#define MMIPDP_PS_ADAP_380_112_2_18_2_44_57_124 "PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, pdp_context_arr_ptr error"
#define MMIPDP_PS_ADAP_386_112_2_18_2_44_57_125 "PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, MNGPRS_ReadPdpContextStateEx error"
#define MMIPDP_PS_ADAP_389_112_2_18_2_44_57_126 "PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, is_exist:%d"
#define MMIPDP_PS_ADAP_410_112_2_18_2_44_57_127 "PDP MANAGER MMIPDP_PsIsPdpLinkExist, input error"
#define MMIPDP_PS_ADAP_431_112_2_18_2_44_57_128 "PDP MANAGER MMIPDP_PsIsPdpLinkExist, pdp_context_arr_ptr error"
#define MMIPDP_PS_ADAP_437_112_2_18_2_44_57_129 "PDP MANAGER MMIPDP_PsIsPdpLinkExist, MNGPRS_ReadPdpContextStateEx error"
#define MMIPDP_PS_ADAP_440_112_2_18_2_44_57_130 "PDP MANAGER MMIPDP_PsIsPdpLinkExist, is_exist:%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_PDP_TRC)
TRACE_MSG(MMIPDP_API_291_112_2_18_2_44_48_0,"PdpStringCopy, ALLOC FAIL")
TRACE_MSG(MMIPDP_API_563_112_2_18_2_44_49_1,"PDP MANAGER callback,app_handler:%d,msg:%d")
TRACE_MSG(MMIPDP_API_598_112_2_18_2_44_49_2,"Callback, ALLOC FAIL")
TRACE_MSG(MMIPDP_API_651_112_2_18_2_44_49_3,"CreatMsg, MMIPDP_ACTIVE_REQ ALLOC FAIL")
TRACE_MSG(MMIPDP_API_667_112_2_18_2_44_49_4,"CreatMsg, MMIPDP_DEACTIVE_REQ ALLOC FAIL")
TRACE_MSG(MMIPDP_API_683_112_2_18_2_44_49_5,"CreatMsg, MMIPDP_PS_ACTIVE_CNF ALLOC FAIL")
TRACE_MSG(MMIPDP_API_697_112_2_18_2_44_49_6,"CreatMsg, msg_id=%d ALLOC FAIL")
TRACE_MSG(MMIPDP_API_713_112_2_18_2_44_49_7,"CreatMsg, msg_id=%d ALLOC FAIL")
TRACE_MSG(MMIPDP_API_767_112_2_18_2_44_49_8,"CreatFsmInstance, ALLOC FAIL")
TRACE_MSG(MMIPDP_API_1160_112_2_18_2_44_50_9,"PDP FsmStateTrans: cur:%d, trans to:%d")
TRACE_MSG(MMIPDP_API_1180_112_2_18_2_44_50_10,"PDP MANAGER FsmDeactived,msg_id:%d,link_id:%d")
TRACE_MSG(MMIPDP_API_1201_112_2_18_2_44_50_11,"DEACTIVED, got wrong msg")
TRACE_MSG(MMIPDP_API_1223_112_2_18_2_44_50_12,"PDP MANAGER FsmActived,msg_id:%d,link_id:%d")
TRACE_MSG(MMIPDP_API_1411_112_2_18_2_44_51_13,"ACTIVED, got wrong msg")
TRACE_MSG(MMIPDP_API_1434_112_2_18_2_44_51_14,"PDP MANAGER FsmDeactiving,msg_id:%d,link_id:%d")
TRACE_MSG(MMIPDP_API_1533_112_2_18_2_44_51_15,"DEACTIVING, got wrong msg")
TRACE_MSG(MMIPDP_API_1558_112_2_18_2_44_51_16,"PDP MANAGER FsmActiving,msg_id:%d,link_id:%d")
TRACE_MSG(MMIPDP_API_1703_112_2_18_2_44_51_17,"PDP MANAGER Active fail, ps err=%d")
TRACE_MSG(MMIPDP_API_1736_112_2_18_2_44_51_18,"ACTIVING, got wrong msg")
TRACE_MSG(MMIPDP_API_1840_112_2_18_2_44_52_19,"PDP MANAGER HandlePdpPsMsg,APP_MN_ACTIVATE_PDP_CONTEXT_CNF apn sim same!")
TRACE_MSG(MMIPDP_API_1850_112_2_18_2_44_52_20,"PDP MANAGER HandlePdpPsMsg,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,result:%d,nsapi:%d,pdp_id:%d")
TRACE_MSG(MMIPDP_API_1852_112_2_18_2_44_52_21,"PDP MANAGER HandlePdpPsMsg,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,link_id:%d")
TRACE_MSG(MMIPDP_API_1866_112_2_18_2_44_52_22,"APP_MN_ACTIVATE_PDP_CONTEXT_CNF err link_id=0")
TRACE_MSG(MMIPDP_API_1883_112_2_18_2_44_52_23,"PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,pdp_num:%d,dual_sys:%d")
TRACE_MSG(MMIPDP_API_1889_112_2_18_2_44_52_24,"APP_MN_DEACTIVATE_PDP_CONTEXT_CNF deactive wifi")
TRACE_MSG(MMIPDP_API_1914_112_2_18_2_44_52_25,"PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,pdp_id:%d")
TRACE_MSG(MMIPDP_API_1916_112_2_18_2_44_52_26,"PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,link_id:%d")
TRACE_MSG(MMIPDP_API_1927_112_2_18_2_44_52_27,"APP_MN_DEACTIVATE_PDP_CONTEXT_CNF err link_id=0")
TRACE_MSG(MMIPDP_API_1950_112_2_18_2_44_52_28,"[YKTEST] pdp detatch!!!")
TRACE_MSG(MMIPDP_API_1982_112_2_18_2_44_52_29,"PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_IND,pdp_num:%d")
TRACE_MSG(MMIPDP_API_1995_112_2_18_2_44_52_30,"APP_MN_DEACTIVATE_PDP_CONTEXT_IND err link_id=0")
TRACE_MSG(MMIPDP_API_2085_112_2_18_2_44_52_31,"PDP MANAGER MMIPDP_SetLinkPdpId,link_id:%d,pdp_id:%d")
TRACE_MSG(MMIPDP_API_2126_112_2_18_2_44_52_32,"mmipdp error %s: CFL_ListCreat failed!")
TRACE_MSG(MMIPDP_API_2154_112_2_18_2_44_52_33,"PDP MANAGER MMIPDP_HandleTimer: deactive timer,link_id:%d")
TRACE_MSG(MMIPDP_API_2169_112_2_18_2_44_52_34,"PDP MANAGER MMIPDP_HandleTimer: reactive timer,link_id:%d")
TRACE_MSG(MMIPDP_API_2179_112_2_18_2_44_52_35,"PDP MANAGER MMIPDP_HandleTimer: reactive timer,wrong state")
TRACE_MSG(MMIPDP_API_2200_112_2_18_2_44_52_36,"PDP MANAGER MMIPDP_HandleTimer: active timer,pending list")
TRACE_MSG(MMIPDP_API_2201_112_2_18_2_44_52_37,"PDP MANAGER Active timeout callback, cannot get link resouce, other higher priority APP is using")
TRACE_MSG(MMIPDP_API_2222_112_2_18_2_44_52_38,"PDP MANAGER MMIPDP_HandleTimer: active timer,using list")
TRACE_MSG(MMIPDP_API_2223_112_2_18_2_44_52_39,"PDP MANAGER Active timeout callback, no response from PS")
TRACE_MSG(MMIPDP_API_2250_112_2_18_2_44_52_40,"!!!!!! app_handler = %d,find_data = %d")
TRACE_MSG(MMIPDP_API_2320_112_2_18_2_44_53_41,"mmipdp error %s: pstAppRegInfo is PNULL!")
TRACE_MSG(MMIPDP_API_2332_112_2_18_2_44_53_42,"mmipdp error %s: handle_msg_callback is PNULL!app_handler is 0x%x")
TRACE_MSG(MMIPDP_API_2335_112_2_18_2_44_53_43,"mmipdp info %s: app_handler=%p,nsapi=0x%x,result=%d")
TRACE_MSG(MMIPDP_API_2337_112_2_18_2_44_53_44,"mmipdp info %s: handle_msg_callback exit")
TRACE_MSG(MMIPDP_API_2355_112_2_18_2_44_53_45,"mmipdp error %s: loop is overlap!")
TRACE_MSG(MMIPDP_API_2358_112_2_18_2_44_53_46,"mmipdp info %s: final, i is %d!")
TRACE_MSG(MMIPDP_API_2397_112_2_18_2_44_53_47,"mmipdp error %s: pstAppRegInfo is PNULL!")
TRACE_MSG(MMIPDP_API_2409_112_2_18_2_44_53_48,"mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d")
TRACE_MSG(MMIPDP_API_2412_112_2_18_2_44_53_49,"mmipdp info %s: handle_msg_callback entry %p")
TRACE_MSG(MMIPDP_API_2414_112_2_18_2_44_53_50,"mmipdp info %s: handle_msg_callback exit")
TRACE_MSG(MMIPDP_API_2422_112_2_18_2_44_53_51,"mmipdp error %s: loop is overlap!")
TRACE_MSG(MMIPDP_API_2463_112_2_18_2_44_53_52,"mmipdp error %s: pstAppRegInfo is PNULL!")
TRACE_MSG(MMIPDP_API_2475_112_2_18_2_44_53_53,"mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d")
TRACE_MSG(MMIPDP_API_2478_112_2_18_2_44_53_54,"mmipdp info %s: handle_msg_callback entry %p")
TRACE_MSG(MMIPDP_API_2480_112_2_18_2_44_53_55,"mmipdp info %s: handle_msg_callback exit")
TRACE_MSG(MMIPDP_API_2488_112_2_18_2_44_53_56,"mmipdp error %s: loop is overlap!")
TRACE_MSG(MMIPDP_API_2522_112_2_18_2_44_53_57,"mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d")
TRACE_MSG(MMIPDP_API_2525_112_2_18_2_44_53_58,"mmipdp info %s: handle_msg_callback entry %p")
TRACE_MSG(MMIPDP_API_2527_112_2_18_2_44_53_59,"mmipdp info %s: handle_msg_callback exit")
TRACE_MSG(MMIPDP_API_2564_112_2_18_2_44_53_60,"mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d")
TRACE_MSG(MMIPDP_API_2567_112_2_18_2_44_53_61,"mmipdp info %s: handle_msg_callback entry %p")
TRACE_MSG(MMIPDP_API_2569_112_2_18_2_44_53_62,"mmipdp info %s: handle_msg_callback exit")
TRACE_MSG(MMIPDP_API_2599_112_2_18_2_44_53_63,"mmipdp error %s: pstWifiAppInfo is PNULL!")
TRACE_MSG(MMIPDP_API_2605_112_2_18_2_44_53_64,"mmipdp error %s: dwWifiRegAppList is zeros!")
TRACE_MSG(MMIPDP_API_2612_112_2_18_2_44_53_65,"mmipdp info %s: CFL_ListInsertNode!")
TRACE_MSG(MMIPDP_API_2617_112_2_18_2_44_53_66,"mmipdp info %s: CFL_ListUpdateNodeData!")
TRACE_MSG(MMIPDP_API_2653_112_2_18_2_44_53_67,"mmipdp error %s: mmipdp_WifiRegApp is abnormal")
TRACE_MSG(MMIPDP_API_2660_112_2_18_2_44_53_68,"mmipdp info %s: mmipdp_WifiRegApp is abnormal")
TRACE_MSG(MMIPDP_API_2671_112_2_18_2_44_53_69,"mmipdp info %s: wifi already actived")
TRACE_MSG(MMIPDP_API_2686_112_2_18_2_44_53_70,"mmipdp info %s: call wifi list_win")
TRACE_MSG(MMIPDP_API_2698_112_2_18_2_44_53_71,"mmipdp error %s: app active info is wrong")
TRACE_MSG(MMIPDP_API_2719_112_2_18_2_44_53_72,"mmipdp info %s: app_handler=%d deactive Wifi!")
TRACE_MSG(MMIPDP_API_2753_112_2_18_2_44_53_73,"mmipdp error %s: pstAppRegInfo is PNULL!")
TRACE_MSG(MMIPDP_API_2776_112_2_18_2_44_53_74,"mmipdp error MMIAPIPDP_Active: app_info_ptr is PNULL")
TRACE_MSG(MMIPDP_API_2780_112_2_18_2_44_53_75,"mmipdp info MMIAPIPDP_Active: priority:%d, app_handler:0x%x, apn:%s, dual_sys:%d")
TRACE_MSG(MMIPDP_API_2784_112_2_18_2_44_53_76,"mmipdp error MMIAPIPDP_Active: handle_msg_callback is PNULL!")
TRACE_MSG(MMIPDP_API_2791_112_2_18_2_44_53_77,"mmipdp error MMIAPIPDP_Active: GPRS FDN is forbbiden!")
TRACE_MSG(MMIPDP_API_2800_112_2_18_2_44_54_78,"mmipdp info %s: Request WiFi interface")
TRACE_MSG(MMIPDP_API_2807_112_2_18_2_44_54_79,"PDP MANAGER MMIAPISET_GetSwitchRatState() = %d!")
TRACE_MSG(MMIPDP_API_2817_112_2_18_2_44_54_80,"mmipdp error call is being")
TRACE_MSG(MMIPDP_API_2825_112_2_18_2_44_54_81,"mmipdp error MMIAPIPDP_Active: GPRS apn_ptr is PNULL!")
TRACE_MSG(MMIPDP_API_2831_112_2_18_2_44_54_82,"mmipdp error MMIAPIPDP_Active: GPRS dual_sys is wrong!")
TRACE_MSG(MMIPDP_API_2839_112_2_18_2_44_54_83,"PDP MANAGER MMIAPIPDP_Active,priority:MMIPDP_RESULT_AT_IS_ON")
TRACE_MSG(MMIPDP_API_2853_112_2_18_2_44_54_84,"PDP MANAGER MMIAPIPDP_Active,MMICONNECTION_SETTING_DATA_ROAMING_DISABLE")
TRACE_MSG(MMIPDP_API_2864_112_2_18_2_44_54_85,"PDP MANAGER MMIAPIPDP_Active,MMIPDP_RESULT_DIFFER_DUALSYS")
TRACE_MSG(MMIPDP_API_2872_112_2_18_2_44_54_86,"PDP MANAGER MMIAPIPDP_Active, GprsForce Detach!")
TRACE_MSG(MMIPDP_API_2896_112_2_18_2_44_54_87,"PDP MANAGER MMIAPIPDP_Active, the app already exist in pending list.")
TRACE_MSG(MMIPDP_API_2927_112_2_18_2_44_54_88,"PDP MANAGER MMIAPIPDP_Active,fsm busy")
TRACE_MSG(MMIPDP_API_2933_112_2_18_2_44_54_89,"PDP MANAGER MMIAPIPDP_Active, no link resource, add to pending list")
TRACE_MSG(MMIPDP_API_2963_112_2_18_2_44_54_90,"PDP MANAGER MMIAPIPDP_Deactive,app_handler:%d")
TRACE_MSG(MMIPDP_API_2974_112_2_18_2_44_54_91,"mmipdp error %s: pstAppRegInfo is PNULL!")
TRACE_MSG(MMIPDP_API_2996_112_2_18_2_44_54_92,"PDP MANAGER MMIAPIPDP_Deactive, the app is in the pending list.")
TRACE_MSG(MMIPDP_API_3018_112_2_18_2_44_54_93,"PDP MANAGER MMIAPIPDP_Deactive, the app is not using any link resource!")
TRACE_MSG(MMIPDP_API_3031_112_2_18_2_44_54_94,"PDP MANAGER MMIAPIPDP_Deactive,fsm busy")
TRACE_MSG(MMIPDP_API_3078_112_2_18_2_44_54_95,"PDP MANAGER MMIAPIPDP_ReleaseAll is called, all app are deactived or quit activing")
TRACE_MSG(MMIPDP_API_3118_112_2_18_2_44_54_96,"mmipdp info %s: %d wifi Link state is %d!")
TRACE_MSG(MMIPDP_API_3149_112_2_18_2_44_54_97,"MMIAPIPDP_GetFlowrate nv read error")
TRACE_MSG(MMIPDP_API_3318_112_2_18_2_44_55_98,"MMIAPIPDP_SubscribeEvent, ALLOC FAIL")
TRACE_MSG(MMIPDP_API_3348_112_2_18_2_44_55_99,"PDP MANAGER, reset pdp dual_sys=%d")
TRACE_MSG(MMIPDP_API_3388_112_2_18_2_44_55_100,"PDP MANAGER MMIAPIPDP_ResetPdp is called, all app are deactived or quit activing")
TRACE_MSG(MMIPDP_API_3393_112_2_18_2_44_55_101,"PDP MANAGER MMIAPIPDP_ResetPdp dual_sys=%d, %d, break!")
TRACE_MSG(MMIPDP_API_3426_112_2_18_2_44_55_102,"HandlePdpEvent : %d, dual_sys:%d")
TRACE_MSG(MMIPDP_API_3498_112_2_18_2_44_55_103,"MMIPDP_SetGPRSStatus out")
TRACE_MSG(MMIPDP_API_3505_112_2_18_2_44_55_104,"MMIPDP_SetGPRSStatus out")
TRACE_MSG(MMIPDP_API_3530_112_2_18_2_44_55_105,"MMIAPIPDP_PsIsActivedPdpLinkExist Not Pdp Link Now!")
TRACE_MSG(MMIPDP_API_3564_112_2_18_2_44_55_106,"MMIAPIPDP_PsIsPdpLinkExist Not Pdp Link Now!")
TRACE_MSG(MMIPDP_API_3597_112_2_18_2_44_55_107,"MMIAPIPDP_GetPdpLinkSIM Not Pdp Link Now!")
TRACE_MSG(MMIPDP_API_3693_112_2_18_2_44_55_108,"MMIAPIPDP_IsPermitActivePdp gprs_fdn_gateway_string_ptr i =%d")
TRACE_MSG(MMIPDP_PS_ADAP_69_112_2_18_2_44_57_109,"PDP MANAGER SetQos pdp_id:%d")
TRACE_MSG(MMIPDP_PS_ADAP_76_112_2_18_2_44_57_110,"PDP MANAGER SetQos MNGPRS_SetQosProfileEx FAIL,%d")
TRACE_MSG(MMIPDP_PS_ADAP_82_112_2_18_2_44_57_111,"PDP MANAGER SetQos MNGPRS_SetExtReqQosProfileEx FAIL,%d")
TRACE_MSG(MMIPDP_PS_ADAP_131_112_2_18_2_44_57_112,"PDP MANAGER MMIPDP_PsActivePdp RequestPdpId fail")
TRACE_MSG(MMIPDP_PS_ADAP_142_112_2_18_2_44_57_113,"PDP MANAGER MMIPDP_PsActivePdp set qos fail")
TRACE_MSG(MMIPDP_PS_ADAP_149_112_2_18_2_44_57_114,"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextEx FAIL,%d")
TRACE_MSG(MMIPDP_PS_ADAP_164_112_2_18_2_44_57_115,"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextPcoEx FAIL,%d")
TRACE_MSG(MMIPDP_PS_ADAP_175_112_2_18_2_44_57_116,"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextPcoEx FAIL,%d")
TRACE_MSG(MMIPDP_PS_ADAP_189_112_2_18_2_44_57_117,"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_ActivatePdpContextEx FAIL:%d")
TRACE_MSG(MMIPDP_PS_ADAP_228_112_2_18_2_44_57_118,"PDP MANAGER RequestPdpId fail pdp_id:%d")
TRACE_MSG(MMIPDP_PS_ADAP_231_112_2_18_2_44_57_119,"PDP MANAGER RequestPdpId pdp_id:%d")
TRACE_MSG(MMIPDP_PS_ADAP_277_112_2_18_2_44_57_120,"PDP MANAGER MMIPDP_PsDeactivePdp MMIPDP_CreatTimer:%d")
TRACE_MSG(MMIPDP_PS_ADAP_280_112_2_18_2_44_57_121,"PDP MANAGER MMIPDP_PsDeactivePdp MNGPRS_DeactivatePdpContextEx fail")
TRACE_MSG(MMIPDP_PS_ADAP_319_112_2_18_2_44_57_122,"PDP MANAGER MMIPDP_PsGprsAttach, input error")
TRACE_MSG(MMIPDP_PS_ADAP_359_112_2_18_2_44_57_123,"PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, input error")
TRACE_MSG(MMIPDP_PS_ADAP_380_112_2_18_2_44_57_124,"PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, pdp_context_arr_ptr error")
TRACE_MSG(MMIPDP_PS_ADAP_386_112_2_18_2_44_57_125,"PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, MNGPRS_ReadPdpContextStateEx error")
TRACE_MSG(MMIPDP_PS_ADAP_389_112_2_18_2_44_57_126,"PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, is_exist:%d")
TRACE_MSG(MMIPDP_PS_ADAP_410_112_2_18_2_44_57_127,"PDP MANAGER MMIPDP_PsIsPdpLinkExist, input error")
TRACE_MSG(MMIPDP_PS_ADAP_431_112_2_18_2_44_57_128,"PDP MANAGER MMIPDP_PsIsPdpLinkExist, pdp_context_arr_ptr error")
TRACE_MSG(MMIPDP_PS_ADAP_437_112_2_18_2_44_57_129,"PDP MANAGER MMIPDP_PsIsPdpLinkExist, MNGPRS_ReadPdpContextStateEx error")
TRACE_MSG(MMIPDP_PS_ADAP_440_112_2_18_2_44_57_130,"PDP MANAGER MMIPDP_PsIsPdpLinkExist, is_exist:%d")
END_TRACE_MAP(MMI_APP_PDP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_PDP_TRC_H_


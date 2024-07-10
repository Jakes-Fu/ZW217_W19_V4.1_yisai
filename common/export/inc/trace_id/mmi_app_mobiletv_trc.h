/******************************************************************************
 ** File Name:      mmi_app_mobiletv_trc.h                                         *
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
//trace_id:198
#ifndef _MMI_APP_MOBILETV_TRC_H_
#define _MMI_APP_MOBILETV_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MBBMS_API_DUMMY_42_112_2_18_2_39_24_0 "MBBMS GBA: set callback function\n"
#define MBBMS_API_DUMMY_48_112_2_18_2_39_24_1 "MBBMS GBA: clean callback function\n"
#define MBBMS_API_DUMMY_53_112_2_18_2_39_24_2 "MBBMS GBA: set net id to [%d]\n"
#define MBBMS_API_DUMMY_314_112_2_18_2_39_24_3 "MBBMS_Free_Service_Request_Ptr ... \n"
#define MBBMS_API_DUMMY_329_112_2_18_2_39_24_4 "MBBMS_Free_Service_Request_Ptr END \n"
#define MBBMS_API_DUMMY_344_112_2_18_2_39_24_5 "MBBMS_Free_Service_Request_Res_Ptr ...\n"
#define MBBMS_API_DUMMY_370_112_2_18_2_39_24_6 "MBBMS_Free_Service_Request_Res_Ptr END \n"
#define MMIMBBMS_MAIN_678_112_2_18_2_39_27_7 "MMIMBBMS_Copy2AnyURI src_ptr:0x%x dst_ptr:0x%x"
#define MMIMBBMS_MAIN_693_112_2_18_2_39_27_8 "MMIMBBMS_Copy2AnyURI alloc memory fail"
#define MMIMBBMS_MAIN_725_112_2_18_2_39_27_9 "MMIMBBMS_SetFlowInfo MMIMBBMS_FLOW_MAX <= flow_type"
#define MMIMBBMS_MAIN_731_112_2_18_2_39_27_10 "MMIMBBMS_SetFlowInfo MMIMBBMS_FLOW_STATE_MAX <= status"
#define MMIMBBMS_MAIN_758_112_2_18_2_39_27_11 "MMIMBBMS_StopFlow curr flow status=%d,curr_flow type =%d"
#define MMIMBBMS_MAIN_781_112_2_18_2_39_27_12 "MMIMBBMS_StopFlow MTVSERVICE_StopParseServiceGuide ret = %d"
#define MMIMBBMS_MAIN_799_112_2_18_2_39_27_13 "MMIMBBMS_StopFlow MTVSERVICE_StopParseServiceGuide ret = %d"
#define MMIMBBMS_MAIN_817_112_2_18_2_39_27_14 "MMIMBBMS_StopFlow MTVSERVICE_StopParseESG ret = %d"
#define MMIMBBMS_MAIN_835_112_2_18_2_39_27_15 "MMIMBBMS_StopFlow MTVSERVICE_DeactiveMBBMS ret = %d"
#define MMIMBBMS_MAIN_856_112_2_18_2_39_27_16 "MMIMBBMS_StopFlow MTVSERVICE_Stop ret = %d"
#define MMIMBBMS_MAIN_881_112_2_18_2_39_27_17 "MMIMBBMS_StopFlow MTVSERVICE_MBMSStopSubUpdate ret = %d"
#define MMIMBBMS_MAIN_899_112_2_18_2_39_27_18 "MMIMBBMS_StopFlow MTVSERVICE_MBMSStopServiceRequest ret = %d"
#define MMIMBBMS_MAIN_918_112_2_18_2_39_28_19 "MMIMBBMS_StopFlow MTVSERVICE_MBMSStopUnsubRequest ret = %d"
#define MMIMBBMS_MAIN_936_112_2_18_2_39_28_20 "MMIMBBMS_StopFlow MTVSERVICE_MBMSStopGBA ret = %d"
#define MMIMBBMS_MAIN_954_112_2_18_2_39_28_21 "MMIMBBMS_StopFlow MTVSERVICE_MBMSStopAccountQuiry ret = %d"
#define MMIMBBMS_MAIN_983_112_2_18_2_39_28_22 "MMIMBBMS_SetSubUpdateType MMIMBBMS_SUB_UPDATE_FROM_TYPE_MAX <= sub_update_type"
#define MMIMBBMS_MAIN_987_112_2_18_2_39_28_23 "MMIMBBMS_SetSubUpdateType sub_update_type = %d\n"
#define MMIMBBMS_MAIN_1008_112_2_18_2_39_28_24 "MMIMBBMS_SetGBAOperationType MMIMBBMS_GBA_MRK_FROM_TYPE_MAX <= gba_operation_type"
#define MMIMBBMS_MAIN_1031_112_2_18_2_39_28_25 "MMIMBBMS_SetSubscribeFromType  type >= MMIMBBMS_SUBSCRIBE_FROM_TYPE_MAX"
#define MMIMBBMS_MAIN_1055_112_2_18_2_39_28_26 "MMIMBBMS_SetPlayFromType MMIMBBMS_PLAY_FROM_MAX <= play_from_type"
#define MMIMBBMS_MAIN_1094_112_2_18_2_39_28_27 "MMIMBBMS_Init"
#define MMIMBBMS_MAIN_1160_112_2_18_2_39_28_28 "MMIMBBMS_ActivePDPContext  active_info_ptr is null"
#define MMIMBBMS_MAIN_1182_112_2_18_2_39_28_29 "MMIMBBMS_ActivePDPContext:apn=%s,username=%s,psw=%s,dual_sys=%d"
#define MMIMBBMS_MAIN_1191_112_2_18_2_39_28_30 "MMIMBBMS_ActivePDPContext  MMIAPIPDP_Active failed!"
#define MMIMBBMS_MAIN_1218_112_2_18_2_39_28_31 "MMIMBBMS_DeactivePDPContext ret=%d\n"
#define MMIMBBMS_MAIN_1235_112_2_18_2_39_28_32 "MMIMBBMS_HandlePDPMsg msg_ptr:0x%x mtvservice_pdp_callback:0x%x"
#define MMIMBBMS_MAIN_1241_112_2_18_2_39_28_33 "MMIMBBMS_HandlePDPMsg:mbbms is not open,ignore msg =%d\n"
#define MMIMBBMS_MAIN_1245_112_2_18_2_39_28_34 "MMIMBBMS_HandlePDPMsg: msg_id=%d, result=%d\n"
#define MMIMBBMS_MAIN_1250_112_2_18_2_39_28_35 "MMIMBBMS_HandlePDPMsg: ignore MMIPDP_DEACTIVE_CNF msg!"
#define MMIMBBMS_MAIN_1289_112_2_18_2_39_28_36 "MMIMBBMS_StartGBAOperation:card not support mtv\n"
#define MMIMBBMS_MAIN_1298_112_2_18_2_39_28_37 "MMIMBBMS_StartGBAOperation:gba is ready\n"
#define MMIMBBMS_MAIN_1303_112_2_18_2_39_28_38 "MMIMBBMS_StartGBAOperation:start gba init process start!\n"
#define MMIMBBMS_MAIN_1307_112_2_18_2_39_28_39 "MMIMBBMS_StartGBAOperation flow status is not MMIMBBMS_FLOW_STATE_IDLE"
#define MMIMBBMS_MAIN_1320_112_2_18_2_39_28_40 "MMIMBBMS_StartGBAOperation gba from type =%d\n"
#define MMIMBBMS_MAIN_1326_112_2_18_2_39_28_41 "MMIMBBMS_StartGBAOperation  MTVSERVICE_StartGBA return fail "
#define MMIMBBMS_MAIN_1341_112_2_18_2_39_28_42 "[MMIMBBMS]: MMIMBBMS_Play freq_no = %d service_id = %d"
#define MMIMBBMS_MAIN_1365_112_2_18_2_39_28_43 "[MMIMBBMS]: MMIMBBMS_Play MMIMTV_MAIN_WIN_ID is in focus"
#define MMIMBBMS_MAIN_1398_112_2_18_2_39_29_44 "[MMIMBBMS]: MMIMBBMS_Play  MMK_WinGrabFocus(MMIMTV_MAIN_WIN_ID)"
#define MMIMBBMS_MAIN_1413_112_2_18_2_39_29_45 "[MMIMBBMS]: MMIMBBMS_Play  MMIMTV_OpenMTVPlayer()"
#define MMIMBBMS_MAIN_1432_112_2_18_2_39_29_46 "MMIMBBMS_StartManualUpdateSG from type=%d service_mode=%d"
#define MMIMBBMS_MAIN_1436_112_2_18_2_39_29_47 "MMIMBBMS_StartManualUpdateSG flow status is not MMIMBBMS_FLOW_STATE_IDLE"
#define MMIMBBMS_MAIN_1451_112_2_18_2_39_29_48 "MMIMBBMS_StartManualUpdateSG gateway =%s,port=%d\n"
#define MMIMBBMS_MAIN_1459_112_2_18_2_39_29_49 "MMIMBBMS_StartManualUpdateSG connection setting failed wap_ret=%d, sg_ret=%d"
#define MMIMBBMS_MAIN_1476_112_2_18_2_39_29_50 "MMIMBBMS_StartManualUpdateSG MTVSERVICE_SetSGAreaName name_len=%d error_code = %d"
#define MMIMBBMS_MAIN_1482_112_2_18_2_39_29_51 "MMIMBBMS_StartManualUpdateSG  MTVSERVICE_ManualUpdateSG return fail"
#define MMIMBBMS_MAIN_1492_112_2_18_2_39_29_52 "MMIMBBMS_StartManualUpdateSG  MTVSERVICE_ManualUpdateSG return fail"
#define MMIMBBMS_MAIN_1532_112_2_18_2_39_29_53 "MMIMBBMS_GetServiceWapSetting wap_addr =%s ,ret_val  =%d"
#define MMIMBBMS_MAIN_1577_112_2_18_2_39_29_54 "MMIMBBMS_GetNAFSettingStatus naf addr =%s ,ret_val  =%d"
#define MMIMBBMS_MAIN_1604_112_2_18_2_39_29_55 "MMIMBBMS_HandleGBAOperationSignal sig_data_ptr is null"
#define MMIMBBMS_MAIN_1609_112_2_18_2_39_29_56 "MMIMBBMS_HandleGBAOperationSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_1613_112_2_18_2_39_29_57 "MMIMBBMS_HandleGBAOperationSignal:  cur flow is stopping"
#define MMIMBBMS_MAIN_1639_112_2_18_2_39_29_58 "MMIMBBMS_HandleGBAOperationSignal subtype MBBMS_SERVICE_GBA"
#define MMIMBBMS_MAIN_1675_112_2_18_2_39_29_59 "MMIMBBMS_HandleGBAOperationSignal subtype MBBMS_SERVICE_PDP_ACTIVE"
#define MMIMBBMS_MAIN_1679_112_2_18_2_39_29_60 "MMIMBBMS_HandleGBAOperationSignal  subtype error!"
#define MMIMBBMS_MAIN_1685_112_2_18_2_39_29_61 "MMIMBBMS_HandleGBAOperationSignal  cur flow status error!"
#define MMIMBBMS_MAIN_1700_112_2_18_2_39_29_62 "MMIMBBMS_HandleStopGBAOperationSignal sig_data_ptr is null"
#define MMIMBBMS_MAIN_1705_112_2_18_2_39_29_63 "MMIMBBMS_HandleStopGBAOperationSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_1731_112_2_18_2_39_29_64 "MMIMBBMS_EnterIntoMainWindow\n"
#define MMIMBBMS_MAIN_1761_112_2_18_2_39_29_65 "MMIMBBMS_HandleNeedUpdateSubscription operation_type =%d"
#define MMIMBBMS_MAIN_1827_112_2_18_2_39_30_66 "HandleSubscribeMainFlowFinish sig_data_ptr is null"
#define MMIMBBMS_MAIN_1831_112_2_18_2_39_30_67 "[MMIMBBMS]HandleSubscribeMainFlowFinish:curr_status= %d"
#define MMIMBBMS_MAIN_1840_112_2_18_2_39_30_68 "[MMIMBBMS]HandleSubscribeMainFlowFinish:Subscrible successfully!\n"
#define MMIMBBMS_MAIN_1857_112_2_18_2_39_30_69 "HandleSubscribeMainFlowFinish alloc mem fail"
#define MMIMBBMS_MAIN_1973_112_2_18_2_39_30_70 "MMIMBBMS HandleSubscribeMainFlowFinish:Subscrible  failed !\n"
#define MMIMBBMS_MAIN_2011_112_2_18_2_39_30_71 "MMIMBBMS_HandleSubscribeSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_2016_112_2_18_2_39_30_72 "MMIMBBMS_HandleSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_2020_112_2_18_2_39_30_73 "MMIMBBMS_HandleSubscribeSignal:  cur flow is stopping"
#define MMIMBBMS_MAIN_2065_112_2_18_2_39_30_74 "MMIMBBMS_HandleSubscribeSignal subtype MBBMS_SERVICE_SUBSCRIBE"
#define MMIMBBMS_MAIN_2070_112_2_18_2_39_30_75 "MMIMBBMS_HandleSubscribeSignal subtype MBBMS_SERVICE_PDP_ACTIVE"
#define MMIMBBMS_MAIN_2074_112_2_18_2_39_30_76 "MMIMBBMS_HandleSubscribeSignal subtype MBBMS_SERVICE_GBA"
#define MMIMBBMS_MAIN_2081_112_2_18_2_39_30_77 "MMIMBBMS_HandleSubscribeSignal  subtype error!"
#define MMIMBBMS_MAIN_2087_112_2_18_2_39_30_78 "MMIMBBMS_HandleSubscribeSignal  cur flow status error!"
#define MMIMBBMS_MAIN_2102_112_2_18_2_39_30_79 "MMIMBBMS_HandleStopSubscribeSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_2107_112_2_18_2_39_30_80 "MMIMBBMS_HandleStopSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_2143_112_2_18_2_39_30_81 "HandleUnsubscribeMainFlowFinish: sig_data_ptr is null"
#define MMIMBBMS_MAIN_2147_112_2_18_2_39_30_82 "[MMIMBBMS]HandleUnsubscribeMainFlowFinish:start"
#define MMIMBBMS_MAIN_2173_112_2_18_2_39_31_83 "[MMIMBBMS]HandleUnsubscribeMainFlowFinish purchase_table_info_ptr = 0x%x\n"
#define MMIMBBMS_MAIN_2189_112_2_18_2_39_31_84 "[MMIMBBMS]HandleUnsubscribeMainFlowFinish anyURI_len = %d\n"
#define MMIMBBMS_MAIN_2207_112_2_18_2_39_31_85 "[MMIMBBMS]HandleUnsubscribeMainFlowFinish purchase_item_error_code =%dn"
#define MMIMBBMS_MAIN_2231_112_2_18_2_39_31_86 "MMIMBBMS_HandleUnSubscribeSignal:UnSubscrible successfully!\n"
#define MMIMBBMS_MAIN_2245_112_2_18_2_39_31_87 "MMIMBBMS_HandleUnSubscribeSignal alloc mem fail"
#define MMIMBBMS_MAIN_2332_112_2_18_2_39_31_88 "[MMIMBBMS]HandleUnsubscribeMainFlowFinish: unsubscrible failed!\n"
#define MMIMBBMS_MAIN_2369_112_2_18_2_39_31_89 "MMIMBBMS_HandleUnSubscribeSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_2374_112_2_18_2_39_31_90 "MMIMBBMS_HandleUnSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_2378_112_2_18_2_39_31_91 "MMIMBBMS_HandleUnSubscribeSignal:  cur flow is stopping"
#define MMIMBBMS_MAIN_2418_112_2_18_2_39_31_92 "MMIMBBMS_HandleUnSubscribeSignal subtype MBBMS_SERVICE_UNSUBSCRIBE"
#define MMIMBBMS_MAIN_2423_112_2_18_2_39_31_93 "MMIMBBMS_HandleUnSubscribeSignal subtype MBBMS_SERVICE_PDP_ACTIVE"
#define MMIMBBMS_MAIN_2427_112_2_18_2_39_31_94 "MMIMBBMS_HandleUnSubscribeSignal subtype MBBMS_SERVICE_GBA"
#define MMIMBBMS_MAIN_2434_112_2_18_2_39_31_95 "MMIMBBMS_HandleUnSubscribeSignal subtype error!"
#define MMIMBBMS_MAIN_2440_112_2_18_2_39_31_96 "MMIMBBMS_HandleUnSubscribeSignal  cur flow status error!"
#define MMIMBBMS_MAIN_2455_112_2_18_2_39_31_97 "MMIMBBMS_HandleStopUnSubscribeSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_2460_112_2_18_2_39_31_98 "MMIMBBMS_HandleStopUnSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_2488_112_2_18_2_39_31_99 "MMIMBBMS_HandleAccountInquirySignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_2493_112_2_18_2_39_31_100 "MMIMBBMS_HandleAccountInquirySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_2497_112_2_18_2_39_31_101 "MMIMBBMS_HandleAccountInquirySignal:  cur flow is stopping"
#define MMIMBBMS_MAIN_2525_112_2_18_2_39_31_102 "MMIMBBMS_HandleAccountInquirySignal subtype MBBMS_SERVICE_ACCOUNT_QUIRY"
#define MMIMBBMS_MAIN_2567_112_2_18_2_39_31_103 "MMIMBBMS_HandleAccountInquirySignal subtype MBBMS_SERVICE_PDP_ACTIVE"
#define MMIMBBMS_MAIN_2571_112_2_18_2_39_31_104 "MMIMBBMS_HandleAccountInquirySignal subtype error!"
#define MMIMBBMS_MAIN_2577_112_2_18_2_39_31_105 "MMIMBBMS_HandleAccountInquirySignal  cur flow status error!"
#define MMIMBBMS_MAIN_2591_112_2_18_2_39_31_106 "MMIMBBMS_HandleStopAccountInquirySignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_2596_112_2_18_2_39_31_107 "MMIMBBMS_HandleStopAccountInquirySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_2627_112_2_18_2_39_31_108 "MMIMBBMS_GetNameFromNameList: name_list_head is null"
#define MMIMBBMS_MAIN_2676_112_2_18_2_39_32_109 "MMIMBBMS_GetPurchaseNameByIndex: purchaseindex_list_head is null"
#define MMIMBBMS_MAIN_2746_112_2_18_2_39_32_110 "MMIMBBMS_GetPurchasePriceByIndex: purchaseindex_list_head is null"
#define MMIMBBMS_MAIN_2774_112_2_18_2_39_32_111 "MMIMBBMS_GetPurchasePriceByIndex not find price info according to currency type"
#define MMIMBBMS_MAIN_2782_112_2_18_2_39_32_112 "MMIMBBMS_GetPurchasePriceByIndex.....get node failed!\n"
#define MMIMBBMS_MAIN_2804_112_2_18_2_39_32_113 "MMIMBBMS_GetPurchaseOrderStatusByIndex: purchaseindex_list_head is null"
#define MMIMBBMS_MAIN_2810_112_2_18_2_39_32_114 "MMIMBBMS_GetPurchaseOrderStatusByIndex: index error\n"
#define MMIMBBMS_MAIN_2823_112_2_18_2_39_32_115 "[MMIMBBMS] Get purchase item order status=%d\n"
#define MMIMBBMS_MAIN_2851_112_2_18_2_39_32_116 "MMIMBBMS_DisplayAllPurchaseItem:Get Purchase Item failed\n"
#define MMIMBBMS_MAIN_2860_112_2_18_2_39_32_117 "---no purchaseitem\n"
#define MMIMBBMS_MAIN_2868_112_2_18_2_39_32_118 "MMIMBBMS_DisplayAllPurchaseItem total purchase num=%d\n"
#define MMIMBBMS_MAIN_2902_112_2_18_2_39_32_119 "MMIMBBMS_DisplayAllPurchaseItem purchase name is null\n"
#define MMIMBBMS_MAIN_2925_112_2_18_2_39_32_120 "MMIMBBMS_DisplayAllPurchaseItem purchase_display_info.period = %d, purchase_display_info.scription_type = %d\n"
#define MMIMBBMS_MAIN_2932_112_2_18_2_39_32_121 "MMIMBBMS_DisplayAllPurchaseItem complete add purchase item\n"
#define MMIMBBMS_MAIN_2947_112_2_18_2_39_32_122 "MMIMBBMS_IsLocalPurchaseItem: pGlobalPurchaseItemID is null"
#define MMIMBBMS_MAIN_3014_112_2_18_2_39_32_123 "MMIMBBMS_SetCurrPurchaseDisplayInfo:Get purchase item index error\n"
#define MMIMBBMS_MAIN_3031_112_2_18_2_39_32_124 "MMIMBBMS_SetCurrPurchaseDisplayInfo:purchase_name_ptr is null\n"
#define MMIMBBMS_MAIN_3121_112_2_18_2_39_32_125 "MMIMBBMS_DisplayServiceInPurchase: temp is null"
#define MMIMBBMS_MAIN_3135_112_2_18_2_39_32_126 "MMIMBBMS_DisplayServiceInPurchase service.NameList =0x%x, len=%dn"
#define MMIMBBMS_MAIN_3171_112_2_18_2_39_33_127 "MMIMBBMS_DisplayContentInPurchase: temp is null"
#define MMIMBBMS_MAIN_3239_112_2_18_2_39_33_128 "MMIMBBMS_DisplayServiceOrContentInPurchase:Get purchase item index error\n"
#define MMIMBBMS_MAIN_3275_112_2_18_2_39_33_129 "MMIMBBMS_DisplayServiceOrContentInPurchase purchase mbbms_string_description1.wstr_len =%d\n"
#define MMIMBBMS_MAIN_3282_112_2_18_2_39_33_130 "MMIMBBMS_DisplayServiceOrContentInPurchase purchase mbbms_string_description2.wstr_len =%d\n"
#define MMIMBBMS_MAIN_3287_112_2_18_2_39_33_131 "MMIMBBMS_DisplayServiceOrContentInPurchase purchase mbbms_string_description.wstr_len =%d\n"
#define MMIMBBMS_MAIN_3364_112_2_18_2_39_33_132 "MBBMSSG_GetPurchaseItemIndex:Get purchase item index error\n"
#define MMIMBBMS_MAIN_3375_112_2_18_2_39_33_133 "MMIMBBMS_SubscriblePurchaseItem: get purchase info error\n"
#define MMIMBBMS_MAIN_3382_112_2_18_2_39_33_134 "MMIMBBMS_SubscriblePurchaseItem: s_subscrible_request_ptr  is not null\n"
#define MMIMBBMS_MAIN_3391_112_2_18_2_39_33_135 "MMIMBBMS_SubscriblePurchaseItem: s_subscrible_request_ptr alloc fail"
#define MMIMBBMS_MAIN_3400_112_2_18_2_39_33_136 "MMIMBBMS_SubscriblePurchaseItem: data_ref_ptr alloc fail"
#define MMIMBBMS_MAIN_3427_112_2_18_2_39_33_137 "MBBMSSG_GetGlobalPurchaseItemtId gloabl_id=%s"
#define MMIMBBMS_MAIN_3431_112_2_18_2_39_33_138 "MBBMS_Service_Request return = %d"
#define MMIMBBMS_MAIN_3435_112_2_18_2_39_33_139 "MMIMBBMS_SubscriblePurchaseItem:Service Request error\n"
#define MMIMBBMS_MAIN_3462_112_2_18_2_39_33_140 " MMIMBBMS_SDandOpenCondition OK!"
#define MMIMBBMS_MAIN_3537_112_2_18_2_39_33_141 "MMIMBBMS_HandleManualUpdateSGSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_3542_112_2_18_2_39_33_142 "MMIMBBMS_HandleManualUpdateSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_3546_112_2_18_2_39_33_143 "MMIMBBMS_HandleManualUpdateSGSignal:  cur flow is stopping"
#define MMIMBBMS_MAIN_3563_112_2_18_2_39_33_144 "MMIMBBMS_HandleManualUpdateSGSignal  cur_service_mode=%d"
#define MMIMBBMS_MAIN_3572_112_2_18_2_39_34_145 "MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_MANUAL_SG"
#define MMIMBBMS_MAIN_3589_112_2_18_2_39_34_146 "MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_UPDATE_SG"
#define MMIMBBMS_MAIN_3603_112_2_18_2_39_34_147 "MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_PARSE_SG"
#define MMIMBBMS_MAIN_3635_112_2_18_2_39_34_148 "MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_UPDATE_TABLE"
#define MMIMBBMS_MAIN_3671_112_2_18_2_39_34_149 "MMIMBBMS_HandleManualUpdateSGSignal subtype CMMB_SEARCH_PROG"
#define MMIMBBMS_MAIN_3676_112_2_18_2_39_34_150 "MMIMBBMS_HandleActiveMBBMSSignal  subtype error!"
#define MMIMBBMS_MAIN_3682_112_2_18_2_39_34_151 "MMIMBBMS_HandleManualUpdateSGSignal  cur flow status error!"
#define MMIMBBMS_MAIN_3696_112_2_18_2_39_34_152 "MMIMBBMS_HandleStopManualUpdateSGSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_3701_112_2_18_2_39_34_153 "MMIMBBMS_HandleStopManualUpdateSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_3723_112_2_18_2_39_34_154 "MMIMBBMS_HandleParseSGSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_3728_112_2_18_2_39_34_155 "MMIMBBMS_HandleParseSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_3735_112_2_18_2_39_34_156 "MMIMBBMS_HandleParseSGSignal:  cur flow is stopping"
#define MMIMBBMS_MAIN_3784_112_2_18_2_39_34_157 "MMIMBBMS_HandleParseSGSignal cur flow status error!"
#define MMIMBBMS_MAIN_3800_112_2_18_2_39_34_158 "MMIMBBMS_HandleStopParseSGSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_3805_112_2_18_2_39_34_159 "MMIMBBMS_HandleStopParseSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_3826_112_2_18_2_39_34_160 "MMIMBBMS_HandleParseESGSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_3831_112_2_18_2_39_34_161 "MMIMBBMS_HandleParseESGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_3838_112_2_18_2_39_34_162 "MMIMBBMS_HandleParseESGSignal:  cur flow is stopping"
#define MMIMBBMS_MAIN_3860_112_2_18_2_39_34_163 "MMIMBBMS_HandleParseESGSignal:  subtype MBBMS_SERVICE_PARSE_ESG"
#define MMIMBBMS_MAIN_3872_112_2_18_2_39_34_164 "MMIMBBMS_HandleParseESGSignal cur flow status error!"
#define MMIMBBMS_MAIN_3888_112_2_18_2_39_34_165 "MMIMBBMS_HandleStopParseESGSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_3893_112_2_18_2_39_34_166 "MMIMBBMS_HandleStopParseESGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_3921_112_2_18_2_39_34_167 "MMIMBBMS_HandleActiveMBBMSSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_3926_112_2_18_2_39_34_168 "MMIMBBMS_HandleActiveMBBMSSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_3932_112_2_18_2_39_34_169 "MMIMBBMS_HandleActiveMBBMSSignal:  cur flow is stopping"
#define MMIMBBMS_MAIN_3957_112_2_18_2_39_34_170 "MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_UPDATE_SG/MBBMS_SERVICE_PARSE_SG"
#define MMIMBBMS_MAIN_3991_112_2_18_2_39_34_171 "MMIMBBMS_HandleActiveMBBMSSignal  subtype result error!"
#define MMIMBBMS_MAIN_3996_112_2_18_2_39_34_172 "MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_UPDATE_TABLE"
#define MMIMBBMS_MAIN_4034_112_2_18_2_39_35_173 "MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_GBA"
#define MMIMBBMS_MAIN_4058_112_2_18_2_39_35_174 "MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_ACTIVE"
#define MMIMBBMS_MAIN_4069_112_2_18_2_39_35_175 "MMIMBBMS_HandleActiveMBBMSSignal  subtype error!"
#define MMIMBBMS_MAIN_4075_112_2_18_2_39_35_176 "MMIMBBMS_HandleActiveMBBMSSignal  cur flow status error!"
#define MMIMBBMS_MAIN_4089_112_2_18_2_39_35_177 "MMIMBBMS_HandleStopActiveMBBMSSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_4094_112_2_18_2_39_35_178 "MMIMBBMS_HandleStopActiveMBBMSSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_4111_112_2_18_2_39_35_179 "MMIMBBMS: MBBMSActiveFailed"
#define MMIMBBMS_MAIN_4131_112_2_18_2_39_35_180 "MMIMBBMS_HandleUpdateSubTableSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_4136_112_2_18_2_39_35_181 "MMIMBBMS_HandleUpdateSubTableSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_4140_112_2_18_2_39_35_182 "MMIMBBMS_HandleUpdateSubTableSignal:  cur flow is stopping"
#define MMIMBBMS_MAIN_4168_112_2_18_2_39_35_183 "MMIMBBMS_HandleUpdateSubTableSignal subtype MBBMS_SERVICE_UPDATE_TABLE"
#define MMIMBBMS_MAIN_4202_112_2_18_2_39_35_184 "MMIMBBMS_HandleUpdateSubTableSignal subtype MBBMS_SERVICE_PDP_ACTIVE"
#define MMIMBBMS_MAIN_4206_112_2_18_2_39_35_185 "MMIMBBMS_HandleActiveMBBMSSignal  subtype error!"
#define MMIMBBMS_MAIN_4212_112_2_18_2_39_35_186 "MMIMBBMS_HandleUpdateSubTableSignal  cur flow status error!"
#define MMIMBBMS_MAIN_4227_112_2_18_2_39_35_187 "MMIMBBMS_HandleStopUpdateSubTableSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_4232_112_2_18_2_39_35_188 "MMIMBBMS_HandleStopUpdateSubTableSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_4258_112_2_18_2_39_35_189 "MMIMBBMS_HandleStartPlaySignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_4263_112_2_18_2_39_35_190 "MMIMBBMS_HandleStartPlaySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_4278_112_2_18_2_39_35_191 "MMIMBBMS_HandleStartPlaySignal is not exiting while MMIMBBMS_FLOW_STATE_STOPPING"
#define MMIMBBMS_MAIN_4290_112_2_18_2_39_35_192 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PDP_ACTIVE"
#define MMIMBBMS_MAIN_4294_112_2_18_2_39_35_193 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_GBA"
#define MMIMBBMS_MAIN_4298_112_2_18_2_39_35_194 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_GETMSK"
#define MMIMBBMS_MAIN_4302_112_2_18_2_39_35_195 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY"
#define MMIMBBMS_MAIN_4311_112_2_18_2_39_35_196 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_RSSI_CHANGED_IND"
#define MMIMBBMS_MAIN_4316_112_2_18_2_39_35_197 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY_ERROR_IND"
#define MMIMBBMS_MAIN_4321_112_2_18_2_39_35_198 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY_AV_IND"
#define MMIMBBMS_MAIN_4326_112_2_18_2_39_35_199 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_EMERGENCY_BROADCAST_IND"
#define MMIMBBMS_MAIN_4331_112_2_18_2_39_35_200 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_CA_NOT_GRANTED_IND"
#define MMIMBBMS_MAIN_4336_112_2_18_2_39_35_201 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY_DRA_IND"
#define MMIMBBMS_MAIN_4341_112_2_18_2_39_35_202 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_BAD_SIGNAL_IND"
#define MMIMBBMS_MAIN_4346_112_2_18_2_39_35_203 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_SIGNAL_RECOVERED_IND"
#define MMIMBBMS_MAIN_4351_112_2_18_2_39_35_204 "MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_SIGNAL_RELOCKING_IND"
#define MMIMBBMS_MAIN_4356_112_2_18_2_39_35_205 "MMIMBBMS_HandleStartPlaySignal  subtype ignored!"
#define MMIMBBMS_MAIN_4409_112_2_18_2_39_35_206 "MMIMBBMS HandlePlayAck result = %d"
#define MMIMBBMS_MAIN_4466_112_2_18_2_39_36_207 "MMIMBBMS HandlePlayErrorIndMsg"
#define MMIMBBMS_MAIN_4493_112_2_18_2_39_36_208 "MMIMBBMS HandleRssiChangedMsg rssi_value = %d"
#define MMIMBBMS_MAIN_4518_112_2_18_2_39_36_209 "MMIMBBMS_HandleStartPlaySignal subtype av_ind  video_ind=%d, audio_ind=%d"
#define MMIMBBMS_MAIN_4548_112_2_18_2_39_36_210 "MMIMBBMS HandleEmergencyBroadcastMsg"
#define MMIMBBMS_MAIN_4660_112_2_18_2_39_36_211 "MMIMBBMS HandleCANotGrantedMsg"
#define MMIMBBMS_MAIN_4685_112_2_18_2_39_36_212 "MMIMBBMS HandleDRAIndMsg"
#define MMIMBBMS_MAIN_4699_112_2_18_2_39_36_213 "MMIMBBMS HandleBadSignalMsg"
#define MMIMBBMS_MAIN_4724_112_2_18_2_39_36_214 "MMIMBBMS HandleSignalRecoveredMsg"
#define MMIMBBMS_MAIN_4749_112_2_18_2_39_36_215 "MMIMBBMS HandleRelockingMsg"
#define MMIMBBMS_MAIN_4766_112_2_18_2_39_36_216 "MMIMBBMS_HandleStopPlaySignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_4771_112_2_18_2_39_36_217 "MMIMBBMS_HandleStopPlaySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_4816_112_2_18_2_39_36_218 "MMIMBBMS_HandleSGGetComplex: err_code = %d"
#define MMIMBBMS_MAIN_4855_112_2_18_2_39_37_219 "MMIMBBMS_HandleSGStopComplex: err_code = %d"
#define MMIMBBMS_MAIN_4878_112_2_18_2_39_37_220 "MMIMBBMS_HandleMSKupdateSignal: sig_data_ptr is null"
#define MMIMBBMS_MAIN_4883_112_2_18_2_39_37_221 "MMIMBBMS_HandleMSKupdateSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_MAIN_4905_112_2_18_2_39_37_222 "MMIMBBMS_IsServiceEncrypted Service encryption status =%d\n"
#define MMIMBBMS_MAIN_4910_112_2_18_2_39_37_223 "MMIMBBMS_IsServiceEncrypted Service encryption status = FALSE\n"
#define MMIMBBMS_MAIN_4937_112_2_18_2_39_37_224 "MMIMBBMS_IsContentEncrypted content encryted status  =%d\n"
#define MMIMBBMS_MAIN_4955_112_2_18_2_39_37_225 "MMIMBBMS_IsPurchaseItemOrdered error_code=%d,is_ordered=%d"
#define MMIMBBMS_MAIN_4985_112_2_18_2_39_37_226 "MMIMBBMS_IsServiceOrdered get purchase result=%d\n"
#define MMIMBBMS_MAIN_4990_112_2_18_2_39_37_227 "MMIMBBMS_IsServiceOrdered:not found purchase which contain this service\n"
#define MMIMBBMS_MAIN_4999_112_2_18_2_39_37_228 "MMIMBBMS_IsServiceOrdered purchase_itemlist_head_ptr=0x%x\n"
#define MMIMBBMS_MAIN_5003_112_2_18_2_39_37_229 "MMIMBBMS_IsServiceOrdered purchase_index_ptr is null"
#define MMIMBBMS_MAIN_5010_112_2_18_2_39_37_230 "MMIMBBMS_IsServiceOrdered :Purchase ordered\n"
#define MMIMBBMS_MAIN_5020_112_2_18_2_39_37_231 "MMIMBBMS_IsServiceOrdered:Get Purchase Item by sevice failded\n"
#define MMIMBBMS_MAIN_5026_112_2_18_2_39_37_232 "MMIMBBMS_IsServiceOrdered:%d"
#define MMIMBBMS_MAIN_5111_112_2_18_2_39_37_233 "MMIMBBMS_PlayService is  encrypted but free,then play direct\n"
#define MMIMBBMS_MAIN_5116_112_2_18_2_39_37_234 "MMIMBBMS_PlayService is  encrypted but not free\n"
#define MMIMBBMS_MAIN_5122_112_2_18_2_39_37_235 "MMIMBBMS_PlayService:service is orderd and then check gba \n"
#define MMIMBBMS_MAIN_5127_112_2_18_2_39_37_236 "MMIMBBMS_PlayService:service is not orderd"
#define MMIMBBMS_MAIN_5133_112_2_18_2_39_37_237 "MMIMBBMS_OpenSubscriblePromptWin MMIMTV_MAIN_WIN_ID is focus"
#define MMIMBBMS_MAIN_5149_112_2_18_2_39_37_238 "MMIMBBMS_PlayService: service is not encrypted and play directly\n"
#define MMIMBBMS_MAIN_5181_112_2_18_2_39_37_239 "MMIMBBMS HandleContentAlertWinMsg close win"
#define MMIMBBMS_MAIN_5209_112_2_18_2_39_37_240 "MMIMBBMS_PlayContent get content  error\n"
#define MMIMBBMS_MAIN_5236_112_2_18_2_39_37_241 "MMIMBBMS_PlayContent is  encrypted but free,then play direct\n"
#define MMIMBBMS_MAIN_5265_112_2_18_2_39_37_242 "MMIMBBMS_PlayService is  encrypted but not free\n"
#define MMIMBBMS_MAIN_5271_112_2_18_2_39_37_243 "MMIMBBMS_PlayContent:service is orderd and then check gba \n"
#define MMIMBBMS_MAIN_5300_112_2_18_2_39_37_244 "MMIMBBMS_PlayContent:service is not orderd"
#define MMIMBBMS_MAIN_5306_112_2_18_2_39_37_245 "MMIMBBMS_OpenSubscriblePromptWin MMIMTV_MAIN_WIN_ID is focus"
#define MMIMBBMS_MAIN_5322_112_2_18_2_39_37_246 "MMIMBBMS_PlayContent: service is not encrypted and play directly\n"
#define MMIMBBMS_MAIN_5382_112_2_18_2_39_38_247 "MMIMBBMS_EnterPlay service cannot play audio service!"
#define MMIMBBMS_MAIN_5403_112_2_18_2_39_38_248 "MMIMBBMS_EnterPlay content cannot play audio service!"
#define MMIMBBMS_MAIN_5417_112_2_18_2_39_38_249 "MMIMBBMS_EnterCMMBModePlay  frequency_no=%d, service_id=%d"
#define MMIMBBMS_MAIN_5421_112_2_18_2_39_38_250 "MMIMBBMS_EnterCMMBModePlay prog_t_ptr is null"
#define MMIMBBMS_MAIN_5444_112_2_18_2_39_38_251 "MMIMBBMS_EnterCMMBModePlay cannot play audio service!"
#define MMIMBBMS_MAIN_5467_112_2_18_2_39_38_252 "MMIMBBMS_GetNAFAddr naf_addr:0x%d, naf_addr_len:0x%x, port:0x%x"
#define MMIMBBMS_MAIN_5475_112_2_18_2_39_38_253 "MMIMBBMS_GetNAFAddr error_code =%d, service_num=%d\n"
#define MMIMBBMS_MAIN_5489_112_2_18_2_39_38_254 "MMIMBBMS_GetNAFAddr service_index_ptr is null"
#define MMIMBBMS_MAIN_5501_112_2_18_2_39_38_255 "MMIMBBMS_GetNAFAddr get access\n"
#define MMIMBBMS_MAIN_5522_112_2_18_2_39_38_256 "MMIMBBMS_GetNAFAddr temp =%s\n"
#define MMIMBBMS_MAIN_5544_112_2_18_2_39_38_257 "MMIMBBMS_GetNAFAddr doesn't find"
#define MMIMBBMS_MAIN_5559_112_2_18_2_39_38_258 "MMIMBBMS_GetNAFAddr naf addr =%s\n"
#define MMIMBBMS_MAIN_5615_112_2_18_2_39_38_259 "MMIMBBMS_EnterMBBMS both sd and usdisk doesn't exist\n"
#define MMIMBBMS_MAIN_5624_112_2_18_2_39_38_260 "MMIMBBMS_EnterMBBMS udisk no space  K\n"
#define MMIMBBMS_MAIN_5711_112_2_18_2_39_38_261 "MMIMBBMS_Init: init_ptr is null"
#define MMIMBBMS_MAIN_5724_112_2_18_2_39_38_262 "MMIMBBMS_Init: no u disk and sd card\n"
#define MMIMBBMS_MAIN_5769_112_2_18_2_39_38_263 "MMIMBBMS_InitProcess enter  TickCount=%d"
#define MMIMBBMS_MAIN_5848_112_2_18_2_39_39_264 "MMIMBBMS HandleStartUpWinMsg wchar welcome_info_len=%d"
#define MMIMBBMS_MAIN_5896_112_2_18_2_39_39_265 "MMIMBBMS_OpenStartUpWin MSG_OPEN_WINDOW TickCount=%d"
#define MMIMBBMS_MAIN_5911_112_2_18_2_39_39_266 "MMIMBBMS_InitProcess MTVSERVICE_Init  before TickCount=%d"
#define MMIMBBMS_MAIN_5919_112_2_18_2_39_39_267 "MMIMBBMS_InitProcess MTVSERVICE_Init  after TickCount=%d"
#define MMIMBBMS_MAIN_5935_112_2_18_2_39_39_268 "MMIMBBMS_OpenStartUpWin MSG_FULL_PAINT TickCount=%d"
#define MMIMBBMS_MAIN_5954_112_2_18_2_39_39_269 "[MMIMBBMS]: HandleStartUpWinMsg abnormal colsed!"
#define MMIMBBMS_MAIN_5958_112_2_18_2_39_39_270 "[MMIMBBMS]: close mbbms init window\n"
#define MMIMBBMS_MAIN_5979_112_2_18_2_39_39_271 "MMIMBBMS_ExitMBBMS start"
#define MMIMBBMS_MAIN_5983_112_2_18_2_39_39_272 "MMIMBBMS_ExitMBBMS app is already closed"
#define MMIMBBMS_MAIN_5994_112_2_18_2_39_39_273 "MMIMBBMS_ExitMBBMS no service is working\n"
#define MMIMBBMS_MAIN_6030_112_2_18_2_39_39_274 "[MMIMTV]: NotifyEventCallback dual_sys=%d, notify_event=%d, param=%d"
#define MMIMBBMS_MAIN_6082_112_2_18_2_39_39_275 "MMIMBBMS_GetSettingInfo notify_type_ptr:0x%x, nerwork_index_ptr:0x%x, sg_ptr:0x%x, naf_ptr:0x%x"
#define MMIMBBMS_MAIN_6097_112_2_18_2_39_39_276 "MMIMBBMS_GetSettingInfo sg_domain_info = %s "
#define MMIMBBMS_MAIN_6131_112_2_18_2_39_39_277 "MMIMBBMS_InitContentNotifyInfo no alarm content file"
#define MMIMBBMS_MAIN_6148_112_2_18_2_39_39_278 "MMIMBBMS_InitContentNotifyInfo %d-%d-%d %d:%d"
#define MMIMBBMS_MAIN_6159_112_2_18_2_39_39_279 "MMIMBBMS_InitContentNotifyInfo  %d:%d:%d"
#define MMIMBBMS_MAIN_6199_112_2_18_2_39_39_280 "MMIMBBMS_InitContentNotifyInfo %d-%d-%d %d:%d"
#define MMIMBBMS_MAIN_6217_112_2_18_2_39_39_281 "MMIMBBMS_InitContentNotifyInfo %d-%d-%d %d:%d"
#define MMIMBBMS_MAIN_6219_112_2_18_2_39_39_282 "MMIMBBMS_InitContentNotifyInfo before update cursor->event_id = %d"
#define MMIMBBMS_MAIN_6236_112_2_18_2_39_39_283 "MMIMBBMS_InitContentNotifyInfo after update cursor->event_id = %d"
#define MMIMBBMS_MAIN_6333_112_2_18_2_39_40_284 "MMIMBBMS_UpdateAllContentAlarm %d-%d-%d %d:%d"
#define MMIMBBMS_MAIN_6351_112_2_18_2_39_40_285 "MMIMBBMS_UpdateAllContentAlarm %d-%d-%d %d:%d"
#define MMIMBBMS_MAIN_6353_112_2_18_2_39_40_286 "before update cursor->event_id = %d"
#define MMIMBBMS_MAIN_6365_112_2_18_2_39_40_287 "after update cursor->event_id = %d"
#define MMIMBBMS_MAIN_6466_112_2_18_2_39_40_288 "MMIMBBMS_RegisterContentAlarm pContentInfo is null"
#define MMIMBBMS_MAIN_6532_112_2_18_2_39_40_289 "MMIMBBMS_RegisterContentAlarm %d-%d-%d %d:%d remindertime = %d event_id = %d"
#define MMIMBBMS_MAIN_6564_112_2_18_2_39_40_290 "MMIMBBMS_GetNewRemindTime pSrcTime:0x%x, pDstTime:0x%x"
#define MMIMBBMS_MAIN_6633_112_2_18_2_39_40_291 "MMIMBBMS_InitSettingInfo read nv failed\n"
#define MMIMBBMS_MAIN_6682_112_2_18_2_39_40_292 "MMIMBBMS_EnterPlayprocess curr flow status=%d,curr_flow type =%d"
#define MMIMBBMS_MAIN_6685_112_2_18_2_39_40_293 "MMIMBBMS_EnterPlayprocess is not permit play!"
#define MMIMBBMS_MAIN_6710_112_2_18_2_39_40_294 "[MMIMBBMS]:MMIMBBMS_EnterPlayprocess   frequency_no = %d service_id = %d uri:%s"
#define MMIMBBMS_MAIN_6732_112_2_18_2_39_40_295 "[MMIMBBMS]:MMIMBBMS_EnterPlayprocess frequency_no = %d service_id = %d uri:%s"
#define MMIMBBMS_MAIN_6758_112_2_18_2_39_40_296 "MMIMBBMS_EnterPlayprocess cmmb service mode!"
#define MMIMBBMS_MAIN_6787_112_2_18_2_39_40_297 "MMIMBBMS_StartSubscribeProcess\n"
#define MMIMBBMS_MAIN_6790_112_2_18_2_39_40_298 "MMIMBBMS_StartSubscribeProcess flow status is not MMIMBBMS_FLOW_STATE_IDLE"
#define MMIMBBMS_MAIN_6814_112_2_18_2_39_40_299 "MMIMBBMS_StartUnSubscribeProcess\n"
#define MMIMBBMS_MAIN_6817_112_2_18_2_39_40_300 "MMIMBBMS_StartUnSubscribeProcess flow status is not MMIMBBMS_FLOW_STATE_IDLE"
#define MMIMBBMS_MAIN_6841_112_2_18_2_39_41_301 "MMIMBBMS_StartUpdateSubscriptionTable from type=%d\n"
#define MMIMBBMS_MAIN_6845_112_2_18_2_39_41_302 "MMIMBBMS_StartUpdateSubscriptionTable flow status is not MMIMBBMS_FLOW_STATE_IDLE"
#define MMIMBBMS_MAIN_6851_112_2_18_2_39_41_303 "MMIMBBMS_StartUpdateSubscriptionTable connection setting error \n"
#define MMIMBBMS_MAIN_6861_112_2_18_2_39_41_304 "MMIMBBMS_StartUpdateSubscriptionTable  MTVSERVICE_SubUpdate return fail "
#define MMIMBBMS_MAIN_6934_112_2_18_2_39_41_305 "MMIMBBMS_StartPlay  frequency_no=%d, service_id=%d cur_flow_type=%d cur_flow_status=%d cmmb_status=%d"
#define MMIMBBMS_MAIN_6944_112_2_18_2_39_41_306 "MMIMBBMS_StartPlay  MTVSERVICE_Play return = %d"
#define MMIMBBMS_MAIN_6963_112_2_18_2_39_41_307 "MMIMBBMS_StopPlay  MTVSERVICE_Stop return fail "
#define MMIMBBMS_MAIN_6980_112_2_18_2_39_41_308 "MMIMBBMS_StartPlay  status error in flow MMIMBBMS_FLOW_PLAY"
#define MMIMBBMS_MAIN_6986_112_2_18_2_39_41_309 "MMIMBBMS_StartPlay  flow type and status error!"
#define MMIMBBMS_MAIN_7000_112_2_18_2_39_41_310 "MMIMBBMS_StopPlay  cur_flow_status=%d, cur_flow_type=%d cmmb_status=%d"
#define MMIMBBMS_MAIN_7023_112_2_18_2_39_41_311 "MMIMBBMS_StopPlay  MTVSERVICE_Stop return = %d"
#define MMIMBBMS_MAIN_7043_112_2_18_2_39_41_312 "MMIMBBMS_StopPlay  cmmb stop complete!"
#define MMIMBBMS_MAIN_7060_112_2_18_2_39_41_313 "MMIMBBMS_QuickStopPlay  cur_flow_status=%d, cur_flow_type=%d cmmb_status=%d"
#define MMIMBBMS_MAIN_7077_112_2_18_2_39_41_314 "MMIMBBMS_QuickStopPlay  complete!"
#define MMIMBBMS_MAIN_7100_112_2_18_2_39_41_315 "MMIMBBMS_StartParseSG process from type=%d MMIMBBMS_TickCount=%d"
#define MMIMBBMS_MAIN_7125_112_2_18_2_39_41_316 "MMIMBBMS_StartParseESG main_service_count = %d"
#define MMIMBBMS_MAIN_7138_112_2_18_2_39_41_317 "MMIMBBMS_StartParseESG flow status is not MMIMBBMS_FLOW_STATE_IDLE"
#define MMIMBBMS_MAIN_7146_112_2_18_2_39_41_318 "MMIMBBMS_StartParseESG net_id = %d"
#define MMIMBBMS_MAIN_7164_112_2_18_2_39_41_319 "MMIMBBMS_StartActiveMBBMS"
#define MMIMBBMS_MAIN_7168_112_2_18_2_39_41_320 "MMIMBBMS_StartActiveMBBMS flow status is not MMIMBBMS_FLOW_STATE_IDLE"
#define MMIMBBMS_MAIN_7175_112_2_18_2_39_41_321 "MMIMBBMS_StartActiveMBBMS MTVSERVICE_ActiveMBBMS return fail"
#define MMIMBBMS_MAIN_7249_112_2_18_2_39_41_322 "MMIMBBMS_GetSubscriptionUpdateStatus = %d\n"
#define MMIMBBMS_MAIN_7267_112_2_18_2_39_41_323 "MMIMBBMS_IsContentExpired pIndex is null"
#define MMIMBBMS_MAIN_7281_112_2_18_2_39_41_324 "MMIMBBMS_IsContentExpired content start time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d"
#define MMIMBBMS_MAIN_7290_112_2_18_2_39_41_325 "MMIMBBMS_IsContentExpired content end time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d"
#define MMIMBBMS_MAIN_7351_112_2_18_2_39_42_326 "MMIMBBMS_IsContentExpired ret =%d"
#define MMIMBBMS_MAIN_7369_112_2_18_2_39_42_327 "MMIMBBMS_CheckContentPlayTime pIndex is null"
#define MMIMBBMS_MAIN_7375_112_2_18_2_39_42_328 "MMIMBBMS_CheckContentPlayTime SG_ERROR_NONE!=MBBMSSG_GetContent"
#define MMIMBBMS_MAIN_7385_112_2_18_2_39_42_329 "MMIMBBMS_CheckContentPlayTime content start time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d"
#define MMIMBBMS_MAIN_7393_112_2_18_2_39_42_330 "MMIMBBMS_CheckContentPlayTime content end time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d"
#define MMIMBBMS_MAIN_7423_112_2_18_2_39_42_331 "MMIMBBMS_CheckContentPlayTime ret =%d"
#define MMIMBBMS_MAIN_7435_112_2_18_2_39_42_332 "MMIMBBMS_Clear enter\n"
#define MMIMBBMS_MAIN_7483_112_2_18_2_39_42_333 "MMIMBBMS SGSearchFailed sg search from type =%d\n"
#define MMIMBBMS_MAIN_7517_112_2_18_2_39_42_334 "MMIMBBMS: SGParseOK!"
#define MMIMBBMS_MAIN_7521_112_2_18_2_39_42_335 "MMIMBBMS SGParseOK: mbbms exiting"
#define MMIMBBMS_MAIN_7570_112_2_18_2_39_42_336 "MMIMBBMS: SGParseFailed"
#define MMIMBBMS_MAIN_7614_112_2_18_2_39_42_337 "MMIMBBMS GBAOperationFailed GBA operation type =%d\n"
#define MMIMBBMS_MAIN_7632_112_2_18_2_39_42_338 "MMIMBBMS GBAOperationFailed:GBA operation type error!"
#define MMIMBBMS_MAIN_7650_112_2_18_2_39_42_339 "MMIMBBMS GBAOperationOK:Operation type =%d\n"
#define MMIMBBMS_MAIN_7671_112_2_18_2_39_42_340 "MMIMBBMS SubTableUpateOK subscription type=%d\n"
#define MMIMBBMS_MAIN_7701_112_2_18_2_39_42_341 "MMIMBBMS SubTableUpateOK:subscription update type error\n "
#define MMIMBBMS_MAIN_7718_112_2_18_2_39_42_342 "MMIMBBMS SubTableUpateFailed from type =%d\n"
#define MMIMBBMS_MAIN_7736_112_2_18_2_39_42_343 "MMIMBBMS SubTableUpateOK:subscription update type error\n "
#define MMIMBBMS_MAIN_7747_112_2_18_2_39_42_344 "MMIMBBMS_StartAccoutInqueryProcess"
#define MMIMBBMS_MAIN_7751_112_2_18_2_39_42_345 "MMIMBBMS flow status is not MMIMBBMS_FLOW_STATE_IDLE"
#define MMIMBBMS_MAIN_7761_112_2_18_2_39_42_346 "MMIMBBMS_StartAccoutInquiryProcess  MMIMBBMS_AccoutInquiry return fail "
#define MMIMBBMS_MAIN_7785_112_2_18_2_39_42_347 "MMIMBBMS_AccoutInquiry: s_account_inquiry_request_ptr is  not null\n"
#define MMIMBBMS_MAIN_7793_112_2_18_2_39_42_348 "MMIMBBMS_AccoutInquiry: s_account_inquiry_request_ptr alloc fail"
#define MMIMBBMS_MAIN_7803_112_2_18_2_39_42_349 "MMIMBBMS_AccoutInquiry: inquiry type =%d \n"
#define MMIMBBMS_MAIN_7806_112_2_18_2_39_42_350 "MBBMS_Account_Inquiry return = %d"
#define MMIMBBMS_MAIN_7947_112_2_18_2_39_43_351 "MMIMBBMS: MMIMBBMS_UpdateWelcomeInfo welcome_str_ptr = 0x%x, str len =%d"
#define MMIMBBMS_MAIN_7960_112_2_18_2_39_43_352 "MMIMBBMS_UpdateWelcomeInfo byte1 = 0x%x, byte2 =0x%x ,byte3= 0x%x"
#define MMIMBBMS_MAIN_7975_112_2_18_2_39_43_353 "MMIMBBMS_GetInitStatus = %d\n"
#define MMIMBBMS_MAIN_7996_112_2_18_2_39_43_354 "MMIMBBMS_GetRemindTime index error =%d"
#define MMIMBBMS_MAIN_8076_112_2_18_2_39_43_355 "MMIMBBMS ResetCurServiceInfo  cur service_id =%d"
#define MMIMBBMS_MAIN_8090_112_2_18_2_39_43_356 "MMIMBBMS_PreExitCallback"
#define MMIMBBMS_MAIN_8119_112_2_18_2_39_43_357 "MMIMBBMS_IsServiceAudioTyle servicetype = %d"
#define MMIMBBMS_MAIN_8145_112_2_18_2_39_43_358 "MMIMBBMS_IsCurrentContentAudioTyle"
#define MMIMBBMS_MAIN_8190_112_2_18_2_39_43_359 "MMIMBBMS_IsCurrentServiceAudioTyle : %d"
#define MMIMBBMS_MAIN_8219_112_2_18_2_39_43_360 "MMIMBBMS_IsMBBMSAppFocous is_focus = %d"
#define MMIMBBMS_MAIN_8239_112_2_18_2_39_43_361 "MMIMBBMS_SetServiceMode mode = %d"
#define MMIMBBMS_MAIN_8315_112_2_18_2_39_43_362 "MMIMBBMS SendSignalToTask sig_to_mmi_ptr alloc fail"
#define MMIMBBMS_MAIN_8353_112_2_18_2_39_44_363 "MMIMBBMS_DisplayAllPurchasedItem  --  account_inquiry_res_ptr = NULL.\n"
#define MMIMBBMS_MAIN_8361_112_2_18_2_39_44_364 "---MMIMBBMS_DisplayAllPurchasedItem no purchaseitem\n"
#define MMIMBBMS_MAIN_8367_112_2_18_2_39_44_365 "MMIMBBMS_DisplayAllPurchasedItem total purchase num=%d\n"
#define MMIMBBMS_MAIN_8376_112_2_18_2_39_44_366 "---MMIMBBMS_DisplayAllPurchasedItem purchase_table_info_ptr = PNULL\n"
#define MMIMBBMS_MAIN_8395_112_2_18_2_39_44_367 "MMIMBBMS_DisplayAllPurchasedItem anyURI_len = %d\n"
#define MMIMBBMS_MAIN_8413_112_2_18_2_39_44_368 "MMIMBBMS_DisplayAllPurchasedItem purchase_item_error_code =%d, purchase_detail_error_code =%dn"
#define MMIMBBMS_MAIN_8428_112_2_18_2_39_44_369 "MMIMBBMS_DisplayAllPurchasedItem purchase name is null\n"
#define MMIMBBMS_MAIN_8497_112_2_18_2_39_44_370 "MMIMBBMS_DisplayServiceOrContentInPurchasedItem  --  account_inquiry_res_ptr = NULL.\n"
#define MMIMBBMS_MAIN_8510_112_2_18_2_39_44_371 "---MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase_table_info_ptr = PNULL\n"
#define MMIMBBMS_MAIN_8545_112_2_18_2_39_44_372 "MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase_item_error_code =%d, purchase_detail_error_code =%dn"
#define MMIMBBMS_MAIN_8553_112_2_18_2_39_44_373 "MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase_item_ptr=0x%x, purchase_detail_ptr=0x%x.n"
#define MMIMBBMS_MAIN_8563_112_2_18_2_39_44_374 "MMIMBBMS_DisplayServiceOrContentInPurchasedItem null in local and account inquiry return.\n"
#define MMIMBBMS_MAIN_8569_112_2_18_2_39_44_375 "MMIMBBMS_DisplayServiceOrContentInPurchasedItem  --  purchase_item_ptr->NameList = NULL.\n"
#define MMIMBBMS_MAIN_8579_112_2_18_2_39_44_376 "MMIMBBMS_DisplayServiceOrContentInPurchasedItem ServiceIdRefList =0x%x, len=%dn"
#define MMIMBBMS_MAIN_8590_112_2_18_2_39_44_377 "MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase description description_str_len =%d\n"
#define MMIMBBMS_MAIN_8595_112_2_18_2_39_44_378 "MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase mbbms_string_description1.wstr_len =%d\n"
#define MMIMBBMS_MAIN_8602_112_2_18_2_39_44_379 "MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase mbbms_string_description2.wstr_len =%d\n"
#define MMIMBBMS_MAIN_8607_112_2_18_2_39_44_380 "MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase mbbms_string_description.wstr_len =%d\n"
#define MMIMBBMS_MAIN_8634_112_2_18_2_39_44_381 "MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase name is null\n"
#define MMIMBBMS_MAIN_8716_112_2_18_2_39_45_382 "MMIMBBMS_SetCurrPurchasedItemDisplayInfo  --  account_inquiry_res_ptr = NULL.\n"
#define MMIMBBMS_MAIN_8731_112_2_18_2_39_45_383 "---MMIMBBMS_SetCurrPurchasedItemDisplayInfo purchase_table_info_ptr = PNULL\n"
#define MMIMBBMS_MAIN_8739_112_2_18_2_39_45_384 "MMIMBBMS_SetCurrPurchasedItemDisplayInfo purchase_item_ptr=0x%x, purchase_detail_ptr=0x%x.n"
#define MMIMBBMS_MAIN_8768_112_2_18_2_39_45_385 "MMIMBBMS_SetCurrPurchasedItemDisplayInfo purchase_item_error_code =%d, purchase_detail_error_code =%dn"
#define MMIMBBMS_MAIN_8873_112_2_18_2_39_45_386 "MMIMBBMS GetTextIdByReturnCode invalid err_code = %d"
#define MMIMBBMS_MAIN_8897_112_2_18_2_39_45_387 "MMIMBBMS CheckDescriptionResValid  type=%d"
#define MMIMBBMS_MAIN_8948_112_2_18_2_39_45_388 "MMIMBBMS CheckDescriptionResValid Input type error!  type=%d"
#define MMIMBBMS_MAIN_8977_112_2_18_2_39_45_389 "MMIMBBMS DisplayErrorTipByDescription len=%d, min_len=%d, real_len=%d"
#define MMIMBBMS_MAIN_9005_112_2_18_2_39_45_390 "MMIMBBMS CreateBgOSDLayer is_bg_layer_open = %d"
#define MMIMBBMS_MAIN_9009_112_2_18_2_39_45_391 "MMIMBBMS CreateBgOSDLayer player win is not focus!"
#define MMIMBBMS_MAIN_9015_112_2_18_2_39_45_392 "MMIMBBMS CreateBgOSDLayer multi-layer not support!"
#define MMIMBBMS_MAIN_9020_112_2_18_2_39_45_393 "MMIMBBMS CreateBgOSDLayer Logic lcd_width=%d, lcd_height=%d"
#define MMIMBBMS_MAIN_9040_112_2_18_2_39_45_394 "MMIMBBMS CreateBgOSDLayer UILAYER_CreateLayer return error:%d"
#define MMIMBBMS_MAIN_9085_112_2_18_2_39_46_395 "MMIMBBMS ReleaseBgOSDLayer is_bg_layer_open=%d"
#define MMIMBBMS_MAIN_9110_112_2_18_2_39_46_396 "MMIMBBMS_SetUpdateCityName: wstr_len=%d utf8_len=%d"
#define MMIMBBMS_MAIN_9162_112_2_18_2_39_46_397 "MMIMBBMS_UnSubscriblePurchaseItemNew account_inquiry_res_ptr = PNULL\n"
#define MMIMBBMS_MAIN_9171_112_2_18_2_39_46_398 "---MMIMBBMS_UnSubscriblePurchaseItemNew purchase_table_info_ptr = PNULL\n"
#define MMIMBBMS_MAIN_9177_112_2_18_2_39_46_399 "MMIMBBMS_UnSubscriblePurchaseItemNew: s_uns_subscrible_request_ptr  is not null\n"
#define MMIMBBMS_MAIN_9186_112_2_18_2_39_46_400 "MMIMBBMS_UnSubscriblePurchaseItemNew s_uns_subscrible_request_ptr alloc fail"
#define MMIMBBMS_MAIN_9195_112_2_18_2_39_46_401 "MMIMBBMS_UnSubscriblePurchaseItemNew data_ref_ptr alloc fail"
#define MMIMBBMS_MAIN_9209_112_2_18_2_39_46_402 "MMIMBBMS_UnSubscriblePurchaseItemNew MBBMS_Unsubscribe_Request return = %d"
#define MMIMBBMS_MAIN_9212_112_2_18_2_39_46_403 "MMIMBBMS_UnSubscriblePurchaseItemNew: service request error\n"
#define MMIMBBMS_MAIN_9264_112_2_18_2_39_46_404 "MMIMBBMS_GetPurchasedItemByIndex  --  index error.\n"
#define MMIMBBMS_MAIN_9278_112_2_18_2_39_46_405 "MMIMBBMS_GetPurchasedItemByIndex  --  purchase_cur  NULL.\n"
#define MMIMBBMS_MAIN_9294_112_2_18_2_39_46_406 "MMIMBBMS_CheckSGDD read nv from init failed\n"
#define MMIMBBMS_MAIN_9303_112_2_18_2_39_46_407 "MMIMBBMS_CheckSGDD nv old url = %s, len = %d"
#define MMIMBBMS_MAIN_9304_112_2_18_2_39_46_408 "MMIMBBMS_CheckSGDD nv new url = %s, len = %d"
#define MMIMBBMS_MAIN_9331_112_2_18_2_39_46_409 "MMIMBBMS_SetCurSelectSim dual_sys error!"
#define MMIMBBMS_MAIN_9334_112_2_18_2_39_46_410 "MMIMBBMS_SetCurSelectSim, dual_sys = %d"
#define MMIMBBMS_WINTAB_1497_112_2_18_2_39_49_411 "MMIMBBMS_AddPurchaseItemToListboxCtrl purchase_display_info_ptr.period = %d, purchase_display_info_ptr.scription_type = %d\n"
#define MMIMBBMS_WINTAB_1532_112_2_18_2_39_49_412 "MMIMBBMS_AddPurchaseItemToListboxCtrl purchase_display_info_ptr.period = %d\n"
#define MMIMBBMS_WINTAB_1544_112_2_18_2_39_49_413 "MMIMBBMS_AddPurchaseItemToListboxCtrl str_period[0] = 0x%x, mbbms_string_unit.wstr_len = %d \n"
#define MMIMBBMS_WINTAB_1810_112_2_18_2_39_50_414 "MMIMBBMS_HandleSearchMainMsgWin genre.string_len = %d"
#define MMIMBBMS_WINTAB_1825_112_2_18_2_39_50_415 "MMIMBBMS_HandleSearchMainMsgWin start MBBMSSG_GetComplexInfo"
#define MMIMBBMS_WINTAB_1832_112_2_18_2_39_50_416 "MMIMBBMS_HandleSearchMainMsgWin is not MMIMBBMS_FLOW_STATE_IDLE"
#define MMIMBBMS_WINTAB_1984_112_2_18_2_39_50_417 "HandleSearchResultMainWinMsg()  msg_id=%d"
#define MMIMBBMS_WINTAB_2045_112_2_18_2_39_50_418 "HandleSearchResultMainWinMsg() MSG_CTL_LIST_NEED_ITEM_DATA msg_id=%d"
#define MMIMBBMS_WINTAB_2048_112_2_18_2_39_50_419 "MMIMBBMS HandleSearchResultMainWinMsg need_item_data_ptr is null"
#define MMIMBBMS_WINTAB_2219_112_2_18_2_39_51_420 "pContent = 0x%x"
#define MMIMBBMS_WINTAB_2435_112_2_18_2_39_51_421 "SCI_ALLOC_APP FOR effect_item FAIL!"
#define MMIMBBMS_WINTAB_2568_112_2_18_2_39_51_422 "HandleMbbmsSettingWinMsg() msg_id=%d"
#define MMIMBBMS_WINTAB_2787_112_2_18_2_39_52_423 "---MMIMBBMS_OpenPurchaseOrderQueryWin purchase_table_info_ptr = 0x%x\n"
#define MMIMBBMS_WINTAB_2803_112_2_18_2_39_52_424 "MMIMBBMS_OpenPurchaseOrderQueryWin anyURI_len = %d\n"
#define MMIMBBMS_WINTAB_2821_112_2_18_2_39_52_425 "MMIMBBMS_OpenPurchaseOrderQueryWin purchase_item_error_code =%dn"
#define MMIMBBMS_WINTAB_2856_112_2_18_2_39_52_426 "MMIMBBMS_OpenPurchaseOrderQueryWin:Get Purchase Item failed\n"
#define MMIMBBMS_WINTAB_3232_112_2_18_2_39_53_427 "MMIMBBMS_GetGlobalIndexFromTwoIndex:Get Purchase Item failed\n"
#define MMIMBBMS_WINTAB_3242_112_2_18_2_39_53_428 "MMIMBBMS_GetGlobalIndexFromTwoIndex ---no purchaseitem\n"
#define MMIMBBMS_WINTAB_3247_112_2_18_2_39_53_429 "MMIMBBMS_GetGlobalIndexFromTwoIndex total purchase num=%d\n"
#define MMIMBBMS_WINTAB_3304_112_2_18_2_39_53_430 "MMIMBBMS_HandlePurchaseItemDetailWinMsg() enter:msg_id=%d"
#define MMIMBBMS_WINTAB_3345_112_2_18_2_39_53_431 "MMIMBBMS_HandlePurchaseItemDetailWinMsg() msg_id=%d"
#define MMIMBBMS_WINTAB_4200_112_2_18_2_39_55_432 "MBBMSSG_GetContentIndex errcode = %d"
#define MMIMBBMS_WINTAB_4213_112_2_18_2_39_55_433 "MBBMSSG_GetContentIndex list_potr = PNULL"
#define MMIMBBMS_WINTAB_4237_112_2_18_2_39_55_434 "pURI_index is pnull"
#define MMIMBBMS_WINTAB_4311_112_2_18_2_39_55_435 "MMIMBBMS_LoadListItem purDetailItem.PriceInfo.period = %d"
#define MMIMBBMS_WINTAB_4385_112_2_18_2_39_55_436 "MMIMBBMS_LoadListItem date year:%d, month:%d day:%d"
#define MMIMBBMS_WINTAB_4388_112_2_18_2_39_55_437 "MMIMBBMS_LoadListItem net_id=%d, service_id=%d CMMB_GetAllProg return=%d, NumOfProg=%d"
#define MMIMBBMS_WINTAB_4438_112_2_18_2_39_55_438 "MMIMBBMS_GetServiceIndex errcode = %d"
#define MMIMBBMS_WINTAB_4447_112_2_18_2_39_55_439 "MMIMBBMS_GetServiceNodeByIdx: only have head node"
#define MMIMBBMS_WINTAB_4494_112_2_18_2_39_55_440 "MBBMSSG_GetContentIndex errcode = %d"
#define MMIMBBMS_WINTAB_4503_112_2_18_2_39_55_441 "MMIMBBMS_GetServiceNodeByIdx: only have head node"
#define MMIMBBMS_WINTAB_4829_112_2_18_2_39_56_442 "MMIMBBMS_HandleProgramDetailWinMsg() enter:msg_id=%d"
#define MMIMBBMS_WINTAB_4864_112_2_18_2_39_56_443 "MBBMSSG_GetContent errcode = %d"
#define MMIMBBMS_WINTAB_4904_112_2_18_2_39_56_444 "MBBMSSG_GetPreviewdata errcode = %d"
#define MMIMBBMS_WINTAB_4925_112_2_18_2_39_56_445 "MMIMBBMS_HandleProgramDetailWinMsg filename[%d] = 0x%x, len = %d"
#define MMIMBBMS_WINTAB_5374_112_2_18_2_39_57_446 "HandleMainInterfaceTabWinMsg MSG_CLOSE_WINDOW\n "
#define MMIMBBMS_WINTAB_5938_112_2_18_2_39_58_447 "MMIMBBMS_OpenMainInterfaceWin enter MMIMBBMS_TickCount=%d"
#define MMIMBBMS_WINTAB_5942_112_2_18_2_39_58_448 "MMIMBBMS_OpenMainInterfaceWin signal \n"
#define MMIMBBMS_WINTAB_5966_112_2_18_2_39_58_449 "MMIMBBMS_OpenMainInterfaceWin end MMIMBBMS_TickCount=%d"
#define MMIMBBMS_WINTAB_6178_112_2_18_2_39_59_450 "[MMIMBBMS]: CreateAllServiceList  list_num = %d"
#define MMIMBBMS_WINTAB_6185_112_2_18_2_39_59_451 "[MMIMBBMS]: CreateAllServiceList pinfo_node alloc fail"
#define MMIMBBMS_WINTAB_6227_112_2_18_2_39_59_452 "[MMIMBBMS]:MBBMSSG_GetGlobalServiceId is error = %d"
#define MMIMBBMS_WINTAB_6286_112_2_18_2_39_59_453 "CreateFavoriteServiceList:Get Service Index error\n"
#define MMIMBBMS_WINTAB_6291_112_2_18_2_39_59_454 "[MMIMTV]: CreateFavoriteServiceList  list_num = %d"
#define MMIMBBMS_WINTAB_6312_112_2_18_2_39_59_455 "[MMIMTV]: CreateFavoriteServiceList MBBMSSG_GetService err = %d"
#define MMIMBBMS_WINTAB_6370_112_2_18_2_40_0_456 "[MMIMTV]: CreateAlarmContentList  list_num = %d"
#define MMIMBBMS_WINTAB_6391_112_2_18_2_40_0_457 "[MMIMTV]: CreateAlarmContentList MBBMSSG_GetContent ERR = %d"
#define MMIMBBMS_WINTAB_6450_112_2_18_2_40_0_458 "[MMIMBBMS]: AddServiceItemToListboxCtrl node_ptr is null"
#define MMIMBBMS_WINTAB_6548_112_2_18_2_40_0_459 "[MMIMBBMS]: AddContentItemToListboxCtrl node_ptr is null"
#define MMIMBBMS_WINTAB_6645_112_2_18_2_40_0_460 "MBBMSSG_GetService errcode = %d"
#define MMIMBBMS_WINTAB_6726_112_2_18_2_40_0_461 "MBBMSSG_GetContentIndex errcode = %d"
#define MMIMBBMS_WINTAB_6739_112_2_18_2_40_0_462 "pURI_index is pnull"
#define MMIMBBMS_WINTAB_6805_112_2_18_2_40_0_463 "[MMIMBBMS] GetInfoFromService bRight=%d, bMax=%d, bMin=%d"
#define MMIMBBMS_WINTAB_7306_112_2_18_2_40_1_464 "MMIMBBMS_RefreshMainInterface: FALSE!"
#define MMIMBBMS_WINTAB_7547_112_2_18_2_40_2_465 "MMIMBBMS_PlayServiceByIndex:Get s_main_service_count=%d, index=%d, s_main_service_index=%d"
#define MMIMBBMS_WINTAB_7556_112_2_18_2_40_2_466 "MMIMBBMS_PlayServiceByIndex cmmb_play_status=%d"
#define MMIMBBMS_WINTAB_7609_112_2_18_2_40_2_467 "MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service Index error\n"
#define MMIMBBMS_WINTAB_7617_112_2_18_2_40_2_468 "MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service NODE error\n"
#define MMIMBBMS_WINTAB_7625_112_2_18_2_40_2_469 "MMIMBBMS_AddCurPlayServiceIntoFavoriteList:have added into favorite list\n"
#define MMIMBBMS_WINTAB_7655_112_2_18_2_40_2_470 "MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service Index error\n"
#define MMIMBBMS_WINTAB_7663_112_2_18_2_40_2_471 "MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service NODE error\n"
#define MMIMBBMS_WINTAB_7671_112_2_18_2_40_2_472 "MMIMBBMS_DelCurPlayServiceIntoFavoriteList:have not in favorite list\n"
#define MMIMBBMS_WINTAB_7701_112_2_18_2_40_2_473 "MMIMBBMS_IsCurPlayServiceInFavoriteList:Get Service Index error\n"
#define MMIMBBMS_WINTAB_7709_112_2_18_2_40_2_474 "MMIMBBMS_IsCurPlayServiceInFavoriteList:Get Service NODE error\n"
#define MMIMBBMS_WINTAB_7753_112_2_18_2_40_2_475 "MBBMSSG_GetService errcode = %d"
#define MMIMBBMS_WINTAB_7805_112_2_18_2_40_3_476 "MMIMBBMS_GetCurrServiceName error! index=%  total_count=%d"
#define MMIMBBMS_WINTAB_7837_112_2_18_2_40_3_477 "MMIMBBMS_OpenStartUpWin TickCount=%d"
#define MMIMBBMS_WINTAB_7882_112_2_18_2_40_3_478 "MMIMBBMS HandleWatchConnectingWinMsg msg MSG_APP_MBBMS_PLAY_CONTENT"
#define MMIMBBMS_WINTAB_8212_112_2_18_2_40_4_479 "[MMIMTV]: MBBMS_AddAllServiceToDropdownList  alloc fail"
#define MMIMBBMS_WINTAB_8228_112_2_18_2_40_4_480 "[MMIMTV]: CreateAllServiceList  list_num = %d"
#define MMIMBBMS_WINTAB_8235_112_2_18_2_40_4_481 "[MMIMTV]: MBBMS_AddAllServiceToDropdownList  alloc fail"
#define MMIMBBMS_WINTAB_8250_112_2_18_2_40_4_482 "MBBMSSG_GetService errcode = %d"
#define MMIMBBMS_WINTAB_8265_112_2_18_2_40_4_483 "wstr_len = %d"
#define MMIMBBMS_WINTAB_8274_112_2_18_2_40_4_484 "index = %d"
#define MMIMBBMS_WINTAB_8520_112_2_18_2_40_5_485 "MMIMBBMS_ContentNotifyCallback: "
#define MMIMBBMS_WINTAB_8524_112_2_18_2_40_5_486 "MMIMBBMS_ContentNotifyCallback: RESTART_BY_ALARM"
#define MMIMBBMS_WINTAB_8541_112_2_18_2_40_5_487 "MMIMBBMS_ContentNotifyCallback: time_remainder is error = %d"
#define MMIMBBMS_WINTAB_8553_112_2_18_2_40_5_488 "MMIMBBMS_ContentNotifyCallback: remindertime = %d systime.tm_min = %d  realtime.tm_min = %d"
#define MMIMBBMS_WINTAB_8563_112_2_18_2_40_5_489 "MMIMBBMS_ContentNotifyCallback: match content id = %s"
#define MMIMBBMS_WINTAB_8710_112_2_18_2_40_5_490 "MMIMBBMS_AddPurchaseDetailTail: scription_type=%d"
#define MMIMBBMS_WINTAB_8729_112_2_18_2_40_5_491 "MMIMBBMS_AddPurchaseDetailTail.....purchase_display_info->period = %d\n"
#define MMIMBBMS_WINTAB_8786_112_2_18_2_40_6_492 "MMIMBBMS_AddPurchaseDetailTail: price scription_type=%d"
#define MMIMBBMS_WINTAB_8832_112_2_18_2_40_6_493 "MMIMBBMS_IsCurrServiceInAllTabEncrypted: %d"
#define MMIMBBMS_WINTAB_8837_112_2_18_2_40_6_494 "MMIMBBMS_IsCurrServiceInAllTabEncrypted error!  index=%  total_count=%d"
#define MMIMBBMS_WINTAB_8872_112_2_18_2_40_6_495 "MMIMBBMS_IsCurrServiceInFavoriteTabEncrypted:%d"
#define MMIMBBMS_WINTAB_8910_112_2_18_2_40_6_496 "MMIMBBMS_IsCurrContentInFavoriteTabEncrypted:%d"
#define MMIMBBMS_WINTAB_8985_112_2_18_2_40_6_497 "[MMIMBBMS]: CreateEBList eb_list_number = %d"
#define MMIMBBMS_WINTAB_9453_112_2_18_2_40_7_498 "HandleSimSelectWinMsg, item_data=%d"
#define MMIMBBMS_WINTAB_9537_112_2_18_2_40_8_499 "sg_addr = %s sg_domain_info = %s "
#define MMIMBBMS_WINTAB_9561_112_2_18_2_40_8_500 "MMIMBBMS_HandleStartTimer timer_master_query = %d"
#define MMIMBBMS_WINTAB_9565_112_2_18_2_40_8_501 "MMIMBBMS_HandleStartTimer app is closed!"
#define MMIMBBMS_WINTAB_9648_112_2_18_2_40_8_502 "[MMIMBBMS]:MMIMBBMS_GetServiceIndexByServiceID MMIMBBMS_GetServiceIndex error!"
#define MMIMBBMS_WINTAB_9654_112_2_18_2_40_8_503 "[MMIMBBMS]: MMIMBBMS_GetServiceIndexByServiceID  list_num = %d"
#define MMIMBBMS_WINTAB_9658_112_2_18_2_40_8_504 "[MMIMBBMS]:MMIMBBMS_GetServiceIndexByServiceID listnum = %d "
#define MMIMBBMS_WINTAB_9703_112_2_18_2_40_8_505 "MMIMBBMS_EnterPlayFromAllServiceTab:Get Service Index error\n"
#define MMIMBBMS_WINTAB_9734_112_2_18_2_40_8_506 "MMIMBBMS_EnterPlayFromAllServiceTab: error enter MMIMBBMS_EnterCMMBModePlay"
#define MMIMBBMS_WINTAB_9829_112_2_18_2_40_9_507 "MMIMBBMS_EnterOpenOrPlayFromProgramList: error enter MMIMBBMS_EnterCMMBModePlay"
#define MMIMBBMS_WINTAB_9910_112_2_18_2_40_9_508 "MMIMBBMS_EnterOpenOrPlayFromProgramDetail enter play\n"
#define MMIMBBMS_WINTAB_9938_112_2_18_2_40_9_509 "MMIMBBMS_EnterOpenOrPlayFromSearchResult enter play\n"
#define MMIMBBMS_WINTAB_9966_112_2_18_2_40_9_510 "MMIMBBMS_EnterOpenOrPlayFromProgramDetail enter play\n"
#define MMIMBBMS_WINTAB_9986_112_2_18_2_40_9_511 "MMIMBBMS_OpenSubscriptionUpdateQueryWin\n"
#define MMIMBBMS_WINTAB_10003_112_2_18_2_40_9_512 "MMIMBBMS_OpenSubscriptionUpdateQueryWinByTextPtr\n"
#define MMIMBBMS_WINTAB_10092_112_2_18_2_40_9_513 "MMIMBBMS_OpenSwitchToMBBMSQueryWin  cur_flow_status=%d, cur_flow_type=%d"
#define MMIMBBMS_WINTAB_10217_112_2_18_2_40_9_514 "StartSwitchToMBBMSProcess enter"
#define MMIMBBMS_WINTAB_10224_112_2_18_2_40_9_515 "StartSwitchToMBBMSProcess cur_status=%d, cur_flow_type=%d"
#define MMIMBBMS_WINTAB_10263_112_2_18_2_40_10_516 "HandleSwitchToMBBMSTimer"
#define MMIMBBMS_WINTAB_10267_112_2_18_2_40_10_517 "HandleSwitchToMBBMSTimer app is closed!"
#define MMIMBBMS_WINTAB_10419_112_2_18_2_40_10_518 "MMIMBBMS_EnterPlayOrOpen s_object_type=%d"
#define MMIMBBMS_WINTAB_10420_112_2_18_2_40_10_519 "MMIMBBMS_TickCount MMIMBBMS_EnterPlayOrOpen enter = %d"
#define MMIMBBMS_WINTAB_10493_112_2_18_2_40_10_520 "MMIMBBMS_OpenAccountInquiryRequestWin"
#define MMIMBBMS_WINTAB_10496_112_2_18_2_40_10_521 "MMIMBBMS_OpenAccountInquiryRequestWin MMIMBBMS_SDandOpenCondition return FALSE"
#define MMIMBBMS_WINTAB_10576_112_2_18_2_40_10_522 "MMIMBBMS_OpenSubscribeUnsubscribeCancelWaitWin %d"
#define MMIMBBMS_WINTAB_10688_112_2_18_2_40_11_523 "MMIMBBMS_DisplayInitWaitingWin:init status=%d\n"
#define MMIMBBMS_WINTAB_10786_112_2_18_2_40_11_524 "MMIMBBMS_AddServiceItemToTabCtrl service_count = %d large than GUITAB_MAX_ITEM_NUM"
#define MMIMBBMS_WINTAB_10873_112_2_18_2_40_11_525 "MMIMBBMS_HandleUpdateMainWinMsg  MTV_ID_MBBMS_AUTO_UPDATE_MENU"
#define MMIMBBMS_WINTAB_10878_112_2_18_2_40_11_526 "MMIMBBMS_HandleUpdateMainWinMsg  MTV_ID_MBBMS_CITY_UPDATE_MENU"
#define MMIMBBMS_WINTAB_10887_112_2_18_2_40_11_527 "MMIMBBMS_HandleUpdateMainWinMsg  ID ERROR!!!!!!!!!!!!!!"
#define MMIMBBMS_WINTAB_10952_112_2_18_2_40_11_528 "MMIMBBMS_HandleUpdateFromCityNameWinMsg()  start city update"
#define MMIMBBMS_WINTAB_10994_112_2_18_2_40_11_529 "MMIMBBMS_GetProgramList s_program_list_ptr alloc fail"
#define MMIMBBMS_WINTAB_11122_112_2_18_2_40_12_530 "[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex content year=%d, month=%d, day=%d, hour=%d, minutes=%d "
#define MMIMBBMS_WINTAB_11132_112_2_18_2_40_12_531 "[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex SYSTEM hour=%d, minutes=%d "
#define MMIMBBMS_WINTAB_11138_112_2_18_2_40_12_532 "[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex SYSTEM year=%d, month=%d, day=%d "
#define MMIMBBMS_WINTAB_11152_112_2_18_2_40_12_533 "[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex bRight=%d "
#define MMIMBBMS_WINTAB_11163_112_2_18_2_40_12_534 "[MMIMBBMS] MMIMBBMS_SetIfEnterSubFromPlay ret=%d "
#define MMIMBBMS_WINTAB_11173_112_2_18_2_40_12_535 "[MMIMBBMS] MMIMBBMS_GetIfEnterSubFromPlay ret=%d "
#define MMIMBBMS_WINTAB_11202_112_2_18_2_40_12_536 "MBBMS_AddAllContentTypeToDropdownListNew genre_ptr = 0x%x, total_num = %d"
#define MMIMBBMS_WINTAB_11207_112_2_18_2_40_12_537 "MBBMS_AddAllContentTypeToDropdownListNew item_data_ptr = PNULL. \n "
#define MMIMBBMS_WINTAB_11230_112_2_18_2_40_12_538 "MBBMS_AddAllContentTypeToDropdownListNew content_type_string_ptr = 0x%x, string_len = %d"
#define MMIMTV_DATA_149_112_2_18_2_40_12_539 "MMIMTV_NewDataData s_data_data_ptr is null"
#define MMIMTV_DATA_269_112_2_18_2_40_13_540 "[MMIMTV]: MMIMTV_DeleteServiceNode find service node error!"
#define MMIMTV_DATA_373_112_2_18_2_40_13_541 "[MMIMTV]: MMIMTV_DeleteNetNode, find net node error!"
#define MMIMTV_DATA_508_112_2_18_2_40_13_542 "[MMIMTV]: MMIMTV_GetNetNodeViaIndex, index = %d"
#define MMIMTV_DATA_669_112_2_18_2_40_14_543 "[MMIMTV]: MMIMTV_SetPlayingNetServiceNodeViaID, net_id=%d, service_id=%d"
#define MMIMTV_DATA_889_112_2_18_2_40_14_544 "[MMIMTV]: MMIMTV_AddEBNode alloc memory error!"
#define MMIMTV_DATA_931_112_2_18_2_40_14_545 "[MMIMTV]: MMIMTV_DeleteEBNode find eb node error!"
#define MMIMTV_DATA_1018_112_2_18_2_40_14_546 "[MMIMTV]: MMIMTV_GetUnreadEBMsgNum, num = %d"
#define MMIMTV_DATA_1170_112_2_18_2_40_15_547 "[MMIMTV]: MMIMTV_ReadEBFile service_head_ptr not null! "
#define MMIMTV_DATA_1191_112_2_18_2_40_15_548 "[MMIMTV]: MMIMTV_ReadEBFile create file error! "
#define MMIMTV_DATA_1197_112_2_18_2_40_15_549 "[MMIMTV]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_1212_112_2_18_2_40_15_550 "[MMIMTV]: EB file version error! "
#define MMIMTV_DATA_1223_112_2_18_2_40_15_551 "[MMIMTV]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_1227_112_2_18_2_40_15_552 "[MMIMTV]: MMIMTV_ReadEBFile total list num = %d"
#define MMIMTV_DATA_1231_112_2_18_2_40_15_553 "[MMIMTV]: MMIAPIFMM_SetFilePointer error! "
#define MMIMTV_DATA_1251_112_2_18_2_40_15_554 "[MMIMTV]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_1258_112_2_18_2_40_15_555 "[MMIMTV]: MMIAPIFMM_SetFilePointer error! "
#define MMIMTV_DATA_1297_112_2_18_2_40_15_556 "[MMIMTV]: MMIMTV_WriteEBFile create file error! "
#define MMIMTV_DATA_1365_112_2_18_2_40_15_557 "[MMIMTV]: MMIMTV_ReadNSFile open file error! "
#define MMIMTV_DATA_1371_112_2_18_2_40_15_558 "[MMIMTV]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_1377_112_2_18_2_40_15_559 "[MMIMTV]: NS file version error! "
#define MMIMTV_DATA_1401_112_2_18_2_40_15_560 "[MMIMTV]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_1409_112_2_18_2_40_15_561 "[MMIMTV]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_1454_112_2_18_2_40_15_562 "[MMIMTV]: MMIMTV_WriteNSFile create file error! "
#define MMIMTV_DATA_1680_112_2_18_2_40_16_563 "[MMIMTV]: MMIMTV_SetESGPath path: %s"
#define MMIMTV_DATA_2295_112_2_18_2_40_17_564 "MMIMTV_Mtvstring2Unicode length==0"
#define MMIMTV_DATA_2385_112_2_18_2_40_17_565 "MMIMTV_BMPToYUV422 ENTER"
#define MMIMTV_DATA_2389_112_2_18_2_40_17_566 "MMIMTV_BMPToYUV422 height=%d, width=%d"
#define MMIMTV_DATA_2395_112_2_18_2_40_17_567 "MMIMTV_BMPToYUV422: error, please check image type!"
#define MMIMTV_DATA_2499_112_2_18_2_40_18_568 "[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteServiceFile create file error! "
#define MMIMTV_DATA_2506_112_2_18_2_40_18_569 "[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_2522_112_2_18_2_40_18_570 "[MMIMTV MMIMBBMS]: Favorite Service file version error! "
#define MMIMTV_DATA_2533_112_2_18_2_40_18_571 "[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_2537_112_2_18_2_40_18_572 "[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteServiceFile total list num = %d"
#define MMIMTV_DATA_2541_112_2_18_2_40_18_573 "[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! "
#define MMIMTV_DATA_2561_112_2_18_2_40_18_574 "[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_2568_112_2_18_2_40_18_575 "[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! "
#define MMIMTV_DATA_2614_112_2_18_2_40_18_576 "[MMIMTV MMIMBBMS]: MMIMBBMS_WriteFavoriteServiceFile create file error! "
#define MMIMTV_DATA_2671_112_2_18_2_40_18_577 "[MMIMTV]: MMIMBBMS_AddFavoriteServiceNode alloc memory error! "
#define MMIMTV_DATA_2826_112_2_18_2_40_18_578 "[MMIMTV]: MMIMBBMS_GetFavoriteServiceListNum  num = %d"
#define MMIMTV_DATA_2940_112_2_18_2_40_18_579 "[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteContentFile create file error! "
#define MMIMTV_DATA_2948_112_2_18_2_40_18_580 "[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_2965_112_2_18_2_40_19_581 "[MMIMTV MMIMBBMS]: Favorite Content file version error! "
#define MMIMTV_DATA_2976_112_2_18_2_40_19_582 "[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_2980_112_2_18_2_40_19_583 "[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteContentFile total list num = %d"
#define MMIMTV_DATA_2984_112_2_18_2_40_19_584 "[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! "
#define MMIMTV_DATA_3005_112_2_18_2_40_19_585 "[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
#define MMIMTV_DATA_3012_112_2_18_2_40_19_586 "[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! "
#define MMIMTV_DATA_3058_112_2_18_2_40_19_587 "[MMIMTV MMIMBBMS]: MMIMBBMS_WriteFavoriteContentFile create file error! "
#define MMIMTV_DATA_3115_112_2_18_2_40_19_588 "[MMIMTV MMIMBBMS]: MMIMBBMS_AddAlarmContentNode alloc memory error! "
#define MMIMTV_DATA_3152_112_2_18_2_40_19_589 "[MMIMTV MMIMBBMS]: MMIMBBMS_DelAlarmContentNode find node error! "
#define MMIMTV_DATA_3303_112_2_18_2_40_19_590 "[MMIMTV]: MMIMBBMS_GetAlarmContentListNum  num = %d"
#define MMIMTV_DATA_3355_112_2_18_2_40_19_591 "[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile net_id = %d, service_id = %d"
#define MMIMTV_DATA_3394_112_2_18_2_40_19_592 "[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile write file in E disk error! "
#define MMIMTV_DATA_3397_112_2_18_2_40_19_593 "[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile create file in E disk ok! "
#define MMIMTV_DATA_3409_112_2_18_2_40_19_594 "[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile write file in D disk error! "
#define MMIMTV_DATA_3412_112_2_18_2_40_19_595 "[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile create file in D disk error! "
#define MMIMTV_EXPORT_95_112_2_18_2_40_20_596 "[MMIMTV]: MMIAPIMTV_OpenMTVPlayer"
#define MMIMTV_EXPORT_107_112_2_18_2_40_20_597 "[MMIMTV]: MMIAPIMTV_OpenMTVPlayer call running =%d"
#define MMIMTV_EXPORT_117_112_2_18_2_40_20_598 "[MMIMTV]: MMIAPIMTV_OpenMTVPlayer app is opened!"
#define MMIMTV_EXPORT_207_112_2_18_2_40_20_599 "[MMIMTV]:MMIAPIMTV_Exit"
#define MMIMTV_EXPORT_226_112_2_18_2_40_20_600 "[MMIMTV]:MMIAPIMTV_VTExit "
#define MMIMTV_MAIN_860_112_2_18_2_40_23_601 "[MMIMTV]: MMIMTV_EnterMTV"
#define MMIMTV_MAIN_877_112_2_18_2_40_23_602 "[MMIMTV]: MMIMTV_ExitMTV  current play status = %d"
#define MMIMTV_MAIN_885_112_2_18_2_40_23_603 "[MMIMBBMS]: MMIMBBMS_CloseCMMB MTV WIN is opened!"
#define MMIMTV_MAIN_986_112_2_18_2_40_23_604 "MMIMTV_HandleTPMsg point.x = %d, point.y = %d"
#define MMIMTV_MAIN_1129_112_2_18_2_40_23_605 "[MMIMTV]: MMIMTV_HandleFocusMsg, msg_id = 0x%x, s_focus_osd = 0x%x"
#define MMIMTV_MAIN_1207_112_2_18_2_40_23_606 "[MMIMTV]: MMIMTV_HandlePSMsg: msg_id = 0x%x"
#define MMIMTV_MAIN_1213_112_2_18_2_40_23_607 "[MMIMTV]: MMIMTV_HandlePSMsg app is not open!"
#define MMIMTV_MAIN_1221_112_2_18_2_40_23_608 "[MMIMTV]: received MTV_RSSI_CHANGED msg"
#define MMIMTV_MAIN_1234_112_2_18_2_40_23_609 "[MMIMTV]: received MTV_STIMI_SEARCH_PROG_END_CNF msg"
#define MMIMTV_MAIN_1258_112_2_18_2_40_23_610 "[MMIMTV]: MTVSTIMI_GetNetworkInfoTab network_number = %d."
#define MMIMTV_MAIN_1308_112_2_18_2_40_24_611 "[MMIMTV]: received MTV_STIMI_SEARCH_PROG_ERR_IND msg"
#define MMIMTV_MAIN_1322_112_2_18_2_40_24_612 "[MMIMTV]: received MTV_STIMI_SEARCH_ESG_END_CNF msg"
#define MMIMTV_MAIN_1334_112_2_18_2_40_24_613 "[MMIMTV]: received MTV_STIMI_SEARCH_ESG_ERR_IND msg"
#define MMIMTV_MAIN_1347_112_2_18_2_40_24_614 "[MMIMTV]: received MTV_ESG_PARSE_RESULT_IND  CMMB_ParseESG retrun: %d"
#define MMIMTV_MAIN_1353_112_2_18_2_40_24_615 "[MMIMTV]: received MSG_MTV_ESG_SUC_CNF msg"
#define MMIMTV_MAIN_1360_112_2_18_2_40_24_616 "[MMIMTV]: received MSG_MTV_ESG_ERR_IND msg"
#define MMIMTV_MAIN_1371_112_2_18_2_40_24_617 "[MMIMTV]: received MTV_STIMI_EMERGENCY_BROADCAST_IND msg"
#define MMIMTV_MAIN_1373_112_2_18_2_40_24_618 "[MMIMTV]: received MTV_STIMI_EMERGENCY_BROADCAST_IND msg sig_data->data_type = %d"
#define MMIMTV_MAIN_1457_112_2_18_2_40_24_619 "[MMIMTV]: received MTV_STIMI_PROGRAM_HINT_IND msg"
#define MMIMTV_MAIN_1471_112_2_18_2_40_24_620 "[MMIMTV]: received MTV_STIMI_CTLTAB_UPDATE_IND msg"
#define MMIMTV_MAIN_1475_112_2_18_2_40_24_621 "[MMIMTV]: received MTV_STIMI_RCV_CTLTAB_ERR_IND msg"
#define MMIMTV_MAIN_1479_112_2_18_2_40_24_622 "[MMIMTV]: received MTV_PLAY_ACK msg"
#define MMIMTV_MAIN_1489_112_2_18_2_40_24_623 "[MMIMTV]: received MTV_PLAY_ACK msg  MMIMTV_PLAY_TYPE_NETWORK"
#define MMIMTV_MAIN_1496_112_2_18_2_40_24_624 "[MMIMTV]: received MTV_PLAY_ACK msg  MMIMTV_PLAY_TYPE_RECORD"
#define MMIMTV_MAIN_1513_112_2_18_2_40_24_625 "[MMIMTV]: received MTV_PLAY_ERROR msg"
#define MMIMTV_MAIN_1534_112_2_18_2_40_24_626 "[MMIMTV]: received MTV_PLAY_ERROR_IND msg"
#define MMIMTV_MAIN_1554_112_2_18_2_40_24_627 "[MMIMTV]: received MTV_BAD_SIGNAL msg"
#define MMIMTV_MAIN_1574_112_2_18_2_40_24_628 "[MMIMTV]: received MTV_SIGNAL_RECOVERED msg"
#define MMIMTV_MAIN_1599_112_2_18_2_40_24_629 "[MMIMTV]: received MTV_RELOCKING msg"
#define MMIMTV_MAIN_1618_112_2_18_2_40_24_630 " [MMIMTV]: received MTV_PLAY_AV_IND msg video ind=%d, audio ind=%d"
#define MMIMTV_MAIN_1623_112_2_18_2_40_24_631 "[MMIMTV]: received MTV_PLAY_DRA_IND msg"
#define MMIMTV_MAIN_1638_112_2_18_2_40_24_632 "[MMIMTV]: received MTV_CA_NOT_GRANTED_IND msg"
#define MMIMTV_MAIN_1660_112_2_18_2_40_24_633 "[MMIMTV]: received MTV_CA_CARD_NOT_PRESENT_IND msg"
#define MMIMTV_MAIN_1680_112_2_18_2_40_24_634 "[MMIMTV]: received MTV_STOP_ACK msg"
#define MMIMTV_MAIN_1708_112_2_18_2_40_24_635 "[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_PLAY_NEXT"
#define MMIMTV_MAIN_1718_112_2_18_2_40_24_636 "[MMIMTV]: MTV_STOP_ACK PlayNetwork error"
#define MMIMTV_MAIN_1729_112_2_18_2_40_24_637 "[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_PLAY_RECORD"
#define MMIMTV_MAIN_1735_112_2_18_2_40_24_638 "[MMIMTV]: PlayStream error"
#define MMIMTV_MAIN_1752_112_2_18_2_40_24_639 "[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_SLIDE"
#define MMIMTV_MAIN_1759_112_2_18_2_40_24_640 "[MMIMTV]: SetStreamPos set_pos_result = %d"
#define MMIMTV_MAIN_1764_112_2_18_2_40_24_641 "[MMIMTV]: PlayStream error"
#define MMIMTV_MAIN_1782_112_2_18_2_40_25_642 "[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_SEARCH search type = %d"
#define MMIMTV_MAIN_1789_112_2_18_2_40_25_643 "[MMIMTV]: received MTV_STOP_ACK CMMB_SearchProgram"
#define MMIMTV_MAIN_1794_112_2_18_2_40_25_644 "[MMIMTV]: CMMB_SearchProgram error"
#define MMIMTV_MAIN_1803_112_2_18_2_40_25_645 "[MMIMTV]: received MTV_STOP_ACK CMMB_SearchCenterFrq"
#define MMIMTV_MAIN_1818_112_2_18_2_40_25_646 "[MMIMTV]: CMMB_SearchCenterFrq error"
#define MMIMTV_MAIN_1828_112_2_18_2_40_25_647 "[MMIMTV]: received MTV_STOP_ACK CMMB_SearchESGData"
#define MMIMTV_MAIN_1840_112_2_18_2_40_25_648 "[MMIMTV]: StartESGSearch stimi_ret = %d"
#define MMIMTV_MAIN_1859_112_2_18_2_40_25_649 "[MMIMTV]: received MTV_STOP_ACK  MTV_PLAY_STREAM_END !"
#define MMIMTV_MAIN_1870_112_2_18_2_40_25_650 "[MMIMTV]: MMIMTV_HandlePSMsg MTV_STOP_ERROR !"
#define MMIMTV_MAIN_1883_112_2_18_2_40_25_651 "[MMIMTV]: received MTV_CA_IND msg"
#define MMIMTV_MAIN_1887_112_2_18_2_40_25_652 "[MMIMTV]: received MTV_PLAY_STREAM_END msg"
#define MMIMTV_MAIN_1892_112_2_18_2_40_25_653 "[MMIMTV]: Stop error"
#define MMIMTV_MAIN_1904_112_2_18_2_40_25_654 "[MMIMTV]: received MTV_PLAY_TIME_IND msg"
#define MMIMTV_MAIN_1920_112_2_18_2_40_25_655 "[MMIMTV]: s_mtv_current_para.play_type %d is error!"
#define MMIMTV_MAIN_1925_112_2_18_2_40_25_656 "[MMIMTV]: received MTV_RECORD_FILE_SAVED msg"
#define MMIMTV_MAIN_1939_112_2_18_2_40_25_657 "[MMIMTV]: received MTV_RECORD_FILE_TOOSMALL msg"
#define MMIMTV_MAIN_1952_112_2_18_2_40_25_658 "[MMIMTV]: received MTV_DISK_WRITE_ERROR msg"
#define MMIMTV_MAIN_1973_112_2_18_2_40_25_659 "[MMIMTV]: received MTV_DTL_ERROR msg"
#define MMIMTV_MAIN_1982_112_2_18_2_40_25_660 "[MMIMTV]: received MBBMS_SERVICE_PDP_ACTIVE msg"
#define MMIMTV_MAIN_1997_112_2_18_2_40_25_661 "[MMIMTV]: received MBBMS_SERVICE_PDP_DEACTIVE msg"
#define MMIMTV_MAIN_2002_112_2_18_2_40_25_662 "[MMIMTV]: received MBBMS_SERVICE_PARSE_SG msg"
#define MMIMTV_MAIN_2007_112_2_18_2_40_25_663 "[MMIMTV]: received MBBMS_SERVICE_STOP_PARSE_SG msg"
#define MMIMTV_MAIN_2012_112_2_18_2_40_25_664 "[MMIMTV]: received MBBMS_SERVICE_PARSE_ESG msg"
#define MMIMTV_MAIN_2017_112_2_18_2_40_25_665 "[MMIMTV]: received MBBMS_SERVICE_STOP_PARSE_ESG msg"
#define MMIMTV_MAIN_2022_112_2_18_2_40_25_666 "[MMIMTV]: received MBBMS_SERVICE_ACTIVE msg"
#define MMIMTV_MAIN_2027_112_2_18_2_40_25_667 "[MMIMTV]: received MBBMS_SERVICE_STOP_ACTIVE msg"
#define MMIMTV_MAIN_2032_112_2_18_2_40_25_668 "[MMIMTV]: received MBBMS_SERVICE_UPDATE_TABLE msg"
#define MMIMTV_MAIN_2037_112_2_18_2_40_25_669 "[MMIMTV]: received MBBMS_SERVICE_STOP_UPDATE_TABLE msg"
#define MMIMTV_MAIN_2042_112_2_18_2_40_25_670 "[MMIMTV]: received MBBMS_SERVICE_GBA msg"
#define MMIMTV_MAIN_2047_112_2_18_2_40_25_671 "[MMIMTV]: received MBBMS_SERVICE_STOP_GBA msg"
#define MMIMTV_MAIN_2052_112_2_18_2_40_25_672 "[MMIMTV]: received MBBMS_SERVICE_PLAY msg"
#define MMIMTV_MAIN_2057_112_2_18_2_40_25_673 "[MMIMTV]: received MBBMS_SERVICE_STOP msg"
#define MMIMTV_MAIN_2063_112_2_18_2_40_25_674 "[MMIMBBMS]:receive sg get complexe data!\n"
#define MMIMTV_MAIN_2067_112_2_18_2_40_25_675 "[MMIMBBMS]:receive sg stop getting complexe data!\n"
#define MMIMTV_MAIN_2072_112_2_18_2_40_25_676 "[MMIMTV]: received MBBMS_SERVICE_SUBSCRIBE msg"
#define MMIMTV_MAIN_2077_112_2_18_2_40_25_677 "[MMIMTV]: received MBBMS_SERVICE_STOP_SUBSCRIBE msg"
#define MMIMTV_MAIN_2082_112_2_18_2_40_25_678 "[MMIMTV]: received MBBMS_SERVICE_UNSUBSCRIBE msg"
#define MMIMTV_MAIN_2087_112_2_18_2_40_25_679 "[MMIMTV]: received MBBMS_SERVICE_STOP_UNSUBSCRIBE msg"
#define MMIMTV_MAIN_2092_112_2_18_2_40_25_680 "[MMIMTV]: received MBBMS_SERVICE_ACCOUNT_QUIRY msg"
#define MMIMTV_MAIN_2097_112_2_18_2_40_25_681 "[MMIMTV]: received MBBMS_SERVICE_STOP_ACCOUNT_QUIRY msg"
#define MMIMTV_MAIN_2102_112_2_18_2_40_25_682 "[MMIMTV]: received MBBMS_SERVICE_MANUAL_SG msg"
#define MMIMTV_MAIN_2107_112_2_18_2_40_25_683 "[MMIMTV]: received MBBMS_SERVICE_STOP_MANUAL_SG msg"
#define MMIMTV_MAIN_2112_112_2_18_2_40_25_684 "[MMIMTV]: received MBBMS_SERVICE_CA_NOT_GRANTED_IND msg"
#define MMIMTV_MAIN_2169_112_2_18_2_40_25_685 "[MMIMTV]: TurnUpVolume"
#define MMIMTV_MAIN_2218_112_2_18_2_40_25_686 "[MMIMTV]: TurnDownVolume"
#define MMIMTV_MAIN_2324_112_2_18_2_40_26_687 "[MMIMTV]: ReadNetCtrlTable error!"
#define MMIMTV_MAIN_2345_112_2_18_2_40_26_688 "[MMIMTV]: SetNetCtrlTableData"
#define MMIMTV_MAIN_2355_112_2_18_2_40_26_689 "[MMIMTV]: SetNetCtrlTableData net_id=%d data_len=%d"
#define MMIMTV_MAIN_2393_112_2_18_2_40_26_690 "[MMIMTV]: ParseNetworkInfoListData network_number = %d"
#define MMIMTV_MAIN_2403_112_2_18_2_40_26_691 "[MMIMTV]: ParseNetworkInfoListData network_id = %d"
#define MMIMTV_MAIN_2404_112_2_18_2_40_26_692 "[MMIMTV]: ParseNetworkInfoListData frequency_no = %d"
#define MMIMTV_MAIN_2406_112_2_18_2_40_26_693 "[MMIMTV]: ParseNetworkInfoListData center_frequency = %d"
#define MMIMTV_MAIN_2407_112_2_18_2_40_26_694 "[MMIMTV]: ParseNetworkInfoListData service_number = %d"
#define MMIMTV_MAIN_2409_112_2_18_2_40_26_695 "[MMIMTV]: ParseNetworkInfoListData code_table = %d"
#define MMIMTV_MAIN_2431_112_2_18_2_40_26_696 "[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_GB_2312 ucs_net_name_len = %d."
#define MMIMTV_MAIN_2445_112_2_18_2_40_26_697 "[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_UTF8 ucs_net_name_len = %d."
#define MMIMTV_MAIN_2476_112_2_18_2_40_26_698 "[MMIMTV]: ParseNetworkInfoListData network_id = %d"
#define MMIMTV_MAIN_2477_112_2_18_2_40_26_699 "[MMIMTV]: ParseNetworkInfoListData service_id = %d, service_type=%d"
#define MMIMTV_MAIN_2500_112_2_18_2_40_26_700 "[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_GB_2312 ucs_service_name_len = %d."
#define MMIMTV_MAIN_2514_112_2_18_2_40_26_701 "[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_UTF8 ucs_service_name_len = %d."
#define MMIMTV_MAIN_2559_112_2_18_2_40_26_702 "[MMIMTV]: ParseNetworkInfoListData network_id = %d"
#define MMIMTV_MAIN_2560_112_2_18_2_40_26_703 "[MMIMTV]: ParseNetworkInfoListData shortservice_id = %d, type=%d"
#define MMIMTV_MAIN_2583_112_2_18_2_40_26_704 "[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_GB_2312 ucs_shortservice_name_len = %d."
#define MMIMTV_MAIN_2597_112_2_18_2_40_26_705 "[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_UTF8 ucs_shortservice_name_len = %d."
#define MMIMTV_MAIN_2651_112_2_18_2_40_26_706 "[MMIMTV]: ParseProgramHintData"
#define MMIMTV_MAIN_2656_112_2_18_2_40_26_707 "[MMIMTV]: ParseProgramHintData prog_hintinfo->num = %d"
#define MMIMTV_MAIN_2657_112_2_18_2_40_26_708 "[MMIMTV]: ParseProgramHintData hint_len = %d"
#define MMIMTV_MAIN_2659_112_2_18_2_40_26_709 "[MMIMTV]: ParseProgramHintData MMI_MIN hint_len = %d"
#define MMIMTV_MAIN_2668_112_2_18_2_40_26_710 "[MMIMTV]: ParseProgramHintData ucs_len = %d"
#define MMIMTV_MAIN_2681_112_2_18_2_40_26_711 "[MMIMTV]: ParseProgramHintData ucs_len = %d"
#define MMIMTV_MAIN_2690_112_2_18_2_40_26_712 "[MMIMTV]: ParseProgramHintData code_char_set = %d"
#define MMIMTV_MAIN_2698_112_2_18_2_40_26_713 "[MMIMTV]: ParseProgramHintData hint_len = %d"
#define MMIMTV_MAIN_2700_112_2_18_2_40_26_714 "[MMIMTV]: ParseProgramHintData MMI_MIN hint_len = %d"
#define MMIMTV_MAIN_2708_112_2_18_2_40_26_715 "[MMIMTV]: ParseProgramHintData ucs_len = %d"
#define MMIMTV_MAIN_2721_112_2_18_2_40_26_716 "[MMIMTV]: ParseProgramHintData ucs_len = %d"
#define MMIMTV_MAIN_2730_112_2_18_2_40_26_717 "[MMIMTV]: ParseProgramHintData code_char_set = %d"
#define MMIMTV_MAIN_2754_112_2_18_2_40_26_718 "[MMIMTV]: ParseEmergencyBroadcastData sig_data->data_type = %d"
#define MMIMTV_MAIN_2761_112_2_18_2_40_27_719 "[MMIMTV]: ParseEmergencyBroadcastData origin text_len = %d"
#define MMIMTV_MAIN_2764_112_2_18_2_40_27_720 "[MMIMTV]: ParseEmergencyBroadcastData msg_len = %d,msg_level = %d,code_char_set=%d"
#define MMIMTV_MAIN_2769_112_2_18_2_40_27_721 "[MMIMTV]: ParseEmergencyBroadcastData node.eb_content[MMIMTV_EB_CONTENT_MAX] = %d,result_len = %d"
#define MMIMTV_MAIN_2815_112_2_18_2_40_27_722 "[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData sig_data->data_type = %d"
#define MMIMTV_MAIN_2822_112_2_18_2_40_27_723 "[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData origin text_len = %d"
#define MMIMTV_MAIN_2825_112_2_18_2_40_27_724 "[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData msg_len = %d,msg_level = %d,code_char_set=%d"
#define MMIMTV_MAIN_2830_112_2_18_2_40_27_725 "[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData node.eb_content[MMIMTV_EB_CONTENT_MAX] = %d,result_len = %d"
#define MMIMTV_MAIN_2868_112_2_18_2_40_27_726 "[MMIMTV]: PlayDefaultServiceAfterSearch"
#define MMIMTV_MAIN_2879_112_2_18_2_40_27_727 "[MMIMTV]: PlayDefaultServiceAfterSearch SaveLastServiceId"
#define MMIMTV_MAIN_2888_112_2_18_2_40_27_728 "[MMIMTV]:PlayDefaultServiceAfterSearch PlayNetwork error"
#define MMIMTV_MAIN_2896_112_2_18_2_40_27_729 "[MMIMTV]: PlayDefaultServiceAfterSearch net_id=%d, service_id=%d"
#define MMIMTV_MAIN_2903_112_2_18_2_40_27_730 "[MMIMTV]: PlayDefaultServiceAfterSearch play_status error!"
#define MMIMTV_MAIN_2909_112_2_18_2_40_27_731 "[MMIMTV]: PlayDefaultServiceAfterSearch service Failed !"
#define MMIMTV_MAIN_2915_112_2_18_2_40_27_732 "[MMIMTV]: PlayDefaultServiceAfterSearch network Failed !"
#define MMIMTV_MAIN_2928_112_2_18_2_40_27_733 "[MMIMTV]: ResumePlayServiceAfterSearch play_status = %d  net_id = %d service_id = %d"
#define MMIMTV_MAIN_2942_112_2_18_2_40_27_734 "[MMIMTV]:ResumePlayServiceAfterSearch PlayNetwork error"
#define MMIMTV_MAIN_2955_112_2_18_2_40_27_735 "[MMIMTV]:ResumePlayServiceAfterSearch play_status error"
#define MMIMTV_MAIN_3011_112_2_18_2_40_27_736 "[MMIMTV]: SwitchServicePlay current play_status = %d"
#define MMIMTV_MAIN_3027_112_2_18_2_40_27_737 "[MMIMTV]: Stop error"
#define MMIMTV_MAIN_3042_112_2_18_2_40_27_738 "[MMIMTV]: Switch Channel now play state is stopped"
#define MMIMTV_MAIN_3054_112_2_18_2_40_27_739 "[MMIMTV]: PlayNetwork error"
#define MMIMTV_MAIN_3072_112_2_18_2_40_27_740 "[MMIMBBMS]: StopServicePlay play_status = %d"
#define MMIMTV_MAIN_3079_112_2_18_2_40_27_741 "[MMIMTV]: Stop error"
#define MMIMTV_MAIN_3123_112_2_18_2_40_27_742 "[MMIMTV]: PlayRecordfile record fiel total time = %d"
#define MMIMTV_MAIN_3132_112_2_18_2_40_27_743 "[MMIMTV]: Stop error"
#define MMIMTV_MAIN_3144_112_2_18_2_40_27_744 "[MMIMTV]: PlayRecordfile now play state is stopped"
#define MMIMTV_MAIN_3150_112_2_18_2_40_27_745 "[MMIMTV]: PlayStream error"
#define MMIMTV_MAIN_3174_112_2_18_2_40_27_746 "[MMIMTV]: StartAutoSearch current_play_status = %d"
#define MMIMTV_MAIN_3194_112_2_18_2_40_27_747 "[MMIMTV]: Stop error"
#define MMIMTV_MAIN_3216_112_2_18_2_40_27_748 "[MMIMTV]: CMMB_SearchProgram error"
#define MMIMTV_MAIN_3235_112_2_18_2_40_27_749 "[MMIMTV]: StartFrequencySearch current_play_status = %d  frequency = %d"
#define MMIMTV_MAIN_3256_112_2_18_2_40_28_750 "[MMIMTV]: Stop error"
#define MMIMTV_MAIN_3286_112_2_18_2_40_28_751 "[MMIMTV]: CMMB_SearchCenterFrq error"
#define MMIMTV_MAIN_3305_112_2_18_2_40_28_752 "[MMIMTV]: StartESGSearch current_play_status = %d  net_id = %d"
#define MMIMTV_MAIN_3351_112_2_18_2_40_28_753 "[MMIMTV]: Stop error"
#define MMIMTV_MAIN_3367_112_2_18_2_40_28_754 "[MMIMTV]: StartESGSearch stimi_ret = %d"
#define MMIMTV_MAIN_3391_112_2_18_2_40_28_755 "[MMIMTV]: StartESGParse net_id = %d, parse_status = %d"
#define MMIMTV_MAIN_3401_112_2_18_2_40_28_756 "[MMIMTV]: StartESGParse no enough space"
#define MMIMTV_MAIN_3412_112_2_18_2_40_28_757 "[MMIMTV]: CMMB_ParseESG return = %d"
#define MMIMTV_MAIN_3423_112_2_18_2_40_28_758 "[MMIMTV]: ResetAndQuit !"
#define MMIMTV_MAIN_3471_112_2_18_2_40_28_759 "[MMIMTV]: ResetCMMBModule !"
#define MMIMTV_MAIN_3488_112_2_18_2_40_28_760 "[MMIMTV]: MTV_Initialize error"
#define MMIMTV_MAIN_3507_112_2_18_2_40_28_761 "[MMIMTV]: HandleInitializingSceneMsg, volume_value = 0x%x"
#define MMIMTV_MAIN_3637_112_2_18_2_40_28_762 "[MMIMTV]: OpenPlayNetworkErrScene net_id=%d"
#define MMIMTV_MAIN_3654_112_2_18_2_40_28_763 "[MMIMTV]: OpenPlayStreamErrScene"
#define MMIMTV_MAIN_3674_112_2_18_2_40_28_764 "[MMIMTV]: OpenSearchProgramErrScene"
#define MMIMTV_MAIN_3706_112_2_18_2_40_28_765 "[MMIMTV]: HandleNoneSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_3735_112_2_18_2_40_28_766 "[MMIMTV]: HandleWarningSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_3782_112_2_18_2_40_29_767 "[MMIMTV]: HandleInitializingSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_3803_112_2_18_2_40_29_768 "[MMIMTV]: HandleInitializingSceneMsg PlayNetwork error"
#define MMIMTV_MAIN_3858_112_2_18_2_40_29_769 "[MMIMTV]: HandleNoLastServiceSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_3877_112_2_18_2_40_29_770 "[MMIMTV]: CMMB_SearchProgram error"
#define MMIMTV_MAIN_3967_112_2_18_2_40_29_771 "[MMIMTV]: HandleIdleSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_4102_112_2_18_2_40_29_772 "[MMIMTV]: HandleIdleSceneMsg, num switch error! input_count %d"
#define MMIMTV_MAIN_4149_112_2_18_2_40_29_773 "[MMIMTV]: MMIMTV_TimeOutPlay"
#define MMIMTV_MAIN_4181_112_2_18_2_40_29_774 "[MMIMTV]: MMIMTV_NumSwitchPlay input_num = %d"
#define MMIMTV_MAIN_4185_112_2_18_2_40_29_775 "[MMIMTV]: MMIMTV_NumSwitchPlay !OSD_IDLE osd=%d"
#define MMIMTV_MAIN_4248_112_2_18_2_40_29_776 "[MMIMTV]: HandleShortcutIconSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_4397_112_2_18_2_40_30_777 "[MMIMTV]: HandleMainMenuSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_4568_112_2_18_2_40_30_778 "[MMIMTV]: HandleMainMenuSceneMsg:menu_id 0x%x is error!"
#define MMIMTV_MAIN_4597_112_2_18_2_40_30_779 "[MMIMTV]: HandleQuitQuerySceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_4636_112_2_18_2_40_30_780 "[MMIMTV]: HandleNetListSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_4714_112_2_18_2_40_30_781 "[MMIMTV]: HandleNetListOptSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_4772_112_2_18_2_40_31_782 "[MMIMTV]: HandleDelNetQuerySceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_4823_112_2_18_2_40_31_783 "[MMIMTV]: HandleServiceListSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_4959_112_2_18_2_40_31_784 "[MMIMTV]: HandleServiceListOptSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5073_112_2_18_2_40_31_785 "[MMIMTV]: HandleDelServiceQuerySceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5123_112_2_18_2_40_31_786 "[MMIMTV]: HandleSearchWaitingSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5131_112_2_18_2_40_31_787 "[MMIMTV]: Now stop search !"
#define MMIMTV_MAIN_5160_112_2_18_2_40_31_788 "[MMIMTV]: HandleSearchInvalidFrqSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5195_112_2_18_2_40_32_789 "[MMIMTV]: HandleSearchMenuSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5224_112_2_18_2_40_32_790 "[MMIMTV]: HandleSearchMenuSceneMsg:menu_id %d is error!"
#define MMIMTV_MAIN_5260_112_2_18_2_40_32_791 "[MMIMTV]: HandleManualSearchSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5274_112_2_18_2_40_32_792 "[MMIMTV]: HandleManualSearchSceneMsg, s_search_mode.center_freq = %d"
#define MMIMTV_MAIN_5314_112_2_18_2_40_32_793 "[MMIMTV]: HandleEBListSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5384_112_2_18_2_40_32_794 "[MMIMTV]: HandleEBListOptSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5462_112_2_18_2_40_32_795 "[MMIMTV]: HandleDelEBQuerySceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5515_112_2_18_2_40_32_796 "[MMIMTV]: HandleDelAllEBQuerySceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5561_112_2_18_2_40_32_797 "[MMIMTV]: HandleEBDetailSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5568_112_2_18_2_40_32_798 "[MMIMTV]: HandleEBDetailSceneMsg, index = %d"
#define MMIMTV_MAIN_5601_112_2_18_2_40_32_799 "[MMIMTV]: HandleEBDetailSceneMsg, s_source_osd = %d"
#define MMIMTV_MAIN_5682_112_2_18_2_40_33_800 "[MMIMTV]: HandleBookingRecordSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5726_112_2_18_2_40_33_801 "[MMIMTV]: HandleESGListSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5730_112_2_18_2_40_33_802 "[MMIMTV]: HandleESGListSceneMsg MMIMTV_OPEN_SCENE  parse_status = %d"
#define MMIMTV_MAIN_5761_112_2_18_2_40_33_803 "[MMIMTV]: HandleESGListSceneMsg NumOfProg = %d,ProgTotal=%d"
#define MMIMTV_MAIN_5767_112_2_18_2_40_33_804 "[MMIMTV]: HandleESGListSceneMsg, CMMB_GetEsgDateRange return: %d"
#define MMIMTV_MAIN_5897_112_2_18_2_40_33_805 "[MMIMTV]: HandleSearchESGQuerySceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5937_112_2_18_2_40_33_806 "[MMIMTV]: HandleSearchESGWaitingSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5945_112_2_18_2_40_33_807 "[MMIMTV]: Now stop search !"
#define MMIMTV_MAIN_5970_112_2_18_2_40_33_808 "[MMIMTV]: HandlePubSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_5976_112_2_18_2_40_33_809 "[MMIMTV]: HandlePubSceneMsg param is null"
#define MMIMTV_MAIN_6013_112_2_18_2_40_33_810 "[MMIMTV]: HandleToOtherAppSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6078_112_2_18_2_40_33_811 "[MMIMTV]: HandleSettingSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6199_112_2_18_2_40_34_812 "[MMIMTV]: HandleSettingAudioLanguageSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6204_112_2_18_2_40_34_813 "[MMIMTV]:HandleSettingAudioLanguageSceneMsg, play_status=%d"
#define MMIMTV_MAIN_6208_112_2_18_2_40_34_814 "[MMIMTV]:HandleSettingAudioLanguageSceneMsg audio_lang_count=%d"
#define MMIMTV_MAIN_6261_112_2_18_2_40_34_815 "HandleSettingAudioLanguageSceneMsg play_status = %d is wrong"
#define MMIMTV_MAIN_6287_112_2_18_2_40_34_816 "[MMIMTV]: HandleSettingSubtitleSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6340_112_2_18_2_40_34_817 "[MMIMTV]: HandleSettingAudioModeSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6380_112_2_18_2_40_34_818 "[MMIMTV]: HandleSettingZoomModeSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6422_112_2_18_2_40_34_819 "[MMIMTV]: HandleSettingScreenAdjustSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6459_112_2_18_2_40_34_820 "[MMIMTV]: HandleSettingRecordStoreSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6499_112_2_18_2_40_34_821 "[MMIMTV]: HandleSettingSnapshotStoreSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6551_112_2_18_2_40_34_822 "[MMIMTV]: HandleSettingTransparencySceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6600_112_2_18_2_40_34_823 "[MMIMTV]: HandleSettingDisplayTimeSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6680_112_2_18_2_40_35_824 "[MMIMTV]: HandleSettingBroadcastBGpicSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6725_112_2_18_2_40_35_825 "[MMIMTV]: HandleHelpSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6756_112_2_18_2_40_35_826 "[MMIMTV]: HandleRecordLoadingSceneMsg, msg_id = %d"
#define MMIMTV_MAIN_6789_112_2_18_2_40_35_827 "[MMIMTV]: HandleRecordPlaySceneMsg, msg_id = %d"
#define MMIMTV_MAIN_6800_112_2_18_2_40_35_828 "HandleRecordPlaySceneMsg now pause play"
#define MMIMTV_MAIN_6809_112_2_18_2_40_35_829 "HandleRecordPlaySceneMsg now resume play"
#define MMIMTV_MAIN_6813_112_2_18_2_40_35_830 "[MMIMTV]: Resume error"
#define MMIMTV_MAIN_6830_112_2_18_2_40_35_831 "[MMIMTV]: PlayStream error"
#define MMIMTV_MAIN_6861_112_2_18_2_40_35_832 "[MMIMTV]: Stop error"
#define MMIMTV_MAIN_6908_112_2_18_2_40_35_833 "[MMIMTV]: HandlePlayConnectingSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6944_112_2_18_2_40_35_834 "[MMIMTV]: HandleFavoriteServiceOptSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_6976_112_2_18_2_40_35_835 "[MMIMTV]: HandleReadEBQuerySceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_7017_112_2_18_2_40_35_836 "[MMIMTV]: HandleSubscribleQuerySceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_7068_112_2_18_2_40_35_837 "[MMIMTV]: HandleCMMBModeWarningSceneMsg, msg_id = 0x%x"
#define MMIMTV_MAIN_7171_112_2_18_2_40_36_838 "MMIMTV_GetCurrentAudioLanguage audio_language=%d"
#define MMIMTV_MAIN_7265_112_2_18_2_40_36_839 "AllocAndGetESGInfo networkID = %d, serviceID = %d"
#define MMIMTV_MAIN_7282_112_2_18_2_40_36_840 "[MMIMTV]: MMIMTV_StartRecord play_status = %d"
#define MMIMTV_MAIN_7329_112_2_18_2_40_36_841 "[MMIMTV]: MMIMTV_StopRecord"
#define MMIMTV_MAIN_7357_112_2_18_2_40_36_842 "[MMIMTV]: MMIMTV_PlayRecordCallBack"
#define MMIMTV_MAIN_7405_112_2_18_2_40_36_843 "[MMIMTV]: MMIMTV_SetFocusOSDScene scene >= OSD_ID_MAX"
#define MMIMTV_MAIN_7427_112_2_18_2_40_36_844 "[MMIMTV]: MMIMTV_CloseFocusScene s_focus_osd = %d"
#define MMIMTV_MAIN_7497_112_2_18_2_40_36_845 "[MMIMTV]: MMIMTV_SetOSDRestoreParam param = %d"
#define MMIMTV_MAIN_7763_112_2_18_2_40_37_846 "[MMIMTV]: SendSignalToTask yuv422_buffer_ptr alloc fail"
#define MMIMTV_MAIN_7778_112_2_18_2_40_37_847 "[MMIMTV]: SendSignalToTask BL_Malloc fail"
#define MMIMTV_MAIN_7799_112_2_18_2_40_37_848 "[MMIMTV]: MMIMTV_Snapshot ret: %d, jpeg_buf_ptr:0x%x, jpeg_buf_size : 0x%x "
#define MMIMTV_MAIN_7807_112_2_18_2_40_37_849 "[MMIMTV]: error: MMIMTV_Snapshot create file failed!"
#define MMIMTV_MAIN_7817_112_2_18_2_40_37_850 "[MMIMTV]: MMIMTV_Snapshot write file success"
#define MMIMTV_MAIN_7822_112_2_18_2_40_37_851 "[MMIMTV]: error:MMIMTV_Snapshot write file failed!"
#define MMIMTV_MAIN_7927_112_2_18_2_40_37_852 "[MMIMTV]: MMIMTV_CallBack MTV_ESG_PARSE_RESULT_IND p0 = %d, p1 = %d"
#define MMIMTV_MAIN_7939_112_2_18_2_40_37_853 "[MMIMTV]: MMIMTV_CallBack received MTV_STOP_ACK op_type = %d"
#define MMIMTV_MAIN_7969_112_2_18_2_40_37_854 "[MMIMTV]: SendSignalToTask alloc sig_to_mmi_ptr fail"
#define MMIMTV_MAIN_7987_112_2_18_2_40_37_855 "[MMIMTV]: SetIsReceivedStopACK"
#define MMIMTV_MAIN_8003_112_2_18_2_40_37_856 "[MMIMTV]: MMIMTV_InitCMMB"
#define MMIMTV_MAIN_8116_112_2_18_2_40_38_857 "MMIMTV_AddServiceItemToTabCtrl net == 0"
#define MMIMTV_MAIN_8125_112_2_18_2_40_38_858 "MMIMTV_AddServiceItemToTabCtrl service_count = %d large than GUITAB_MAX_ITEM_NUM"
#define MMIMTV_MAIN_8168_112_2_18_2_40_38_859 "[MMIMBBMS]: MMIMBBMS_CloseCMMB"
#define MMIMTV_OSD_DISPLAY_586_112_2_18_2_40_40_860 "[MMIMTV]: MMIMTV_NewOSDDisplayData"
#define MMIMTV_OSD_DISPLAY_1486_112_2_18_2_40_42_861 "[MMIMTV]: OKMuteIcon UnMute"
#define MMIMTV_OSD_DISPLAY_1507_112_2_18_2_40_42_862 "[MMIMTV]: OKMuteIcon Mute"
#define MMIMTV_OSD_DISPLAY_1563_112_2_18_2_40_42_863 "[MMIMTV]: OKRecordIcon s_focus_osd = %d"
#define MMIMTV_OSD_DISPLAY_1602_112_2_18_2_40_42_864 "[MMIMTV]: OKRecordIcon mbbms flow_type=%d, flow_status=%d"
#define MMIMTV_OSD_DISPLAY_1755_112_2_18_2_40_42_865 "[MMIMTV]: MMIMTV_HandleSystimer MMIMTV_SYSTIME_TIP_PUBWIN"
#define MMIMTV_OSD_DISPLAY_2751_112_2_18_2_40_44_866 "[MMIMTV]: MMIMTV_OpenNetListScene list_num = %d."
#define MMIMTV_OSD_DISPLAY_3019_112_2_18_2_40_45_867 "MMIMTV_OpenAudioLanguageScene lang_code_ptr = %x"
#define MMIMTV_OSD_DISPLAY_3039_112_2_18_2_40_45_868 "MMIMTV_OpenAudioLanguageScene lang_code = %x"
#define MMIMTV_OSD_DISPLAY_3542_112_2_18_2_40_46_869 "[MMIMTV]: MMIMTV_OpenEBListScene eb_list_number = %d  param = %d"
#define MMIMTV_OSD_DISPLAY_3741_112_2_18_2_40_46_870 "[MMIMTV]: CloseRecordListCallBack"
#define MMIMTV_OSD_DISPLAY_3853_112_2_18_2_40_47_871 "[MMIMTV]: MMIMTV_OpenEBListContentScene, param = %d"
#define MMIMTV_OSD_DISPLAY_4468_112_2_18_2_40_48_872 "[MMIMTV]: MMIMTV_OpenManualSearchInputScene, freq_num = %d"
#define MMIMTV_OSD_DISPLAY_4472_112_2_18_2_40_48_873 "[MMIMTV]: MMIMTV_OpenManualSearchInputScene, wstr_freq_len = %d"
#define MMIMTV_OSD_DISPLAY_4932_112_2_18_2_40_49_874 "[MMIMTV]: MMIMTV_OpenESGListScene, NumOfProg = %d"
#define MMIMTV_OSD_DISPLAY_5100_112_2_18_2_40_49_875 "[MMIMTV] MMIMTV_OpenSwitchServiceTab MMIMTV_EnableStb"
#define MMIMTV_OSD_DISPLAY_5214_112_2_18_2_40_49_876 "[MMIMBBMS]:mbbms welcome string len=%d"
#define MMIMTV_OSD_DISPLAY_5221_112_2_18_2_40_49_877 "MMIMTV_OpenPlayConnectingScene picture_ptr = 0x%x"
#define MMIMTV_OSD_DISPLAY_5239_112_2_18_2_40_49_878 "MMIMTV_OpenPlayConnectingScene filename[%d] = 0x%x, len = %d"
#define MMIMTV_OSD_DISPLAY_5578_112_2_18_2_40_50_879 "[MMIMTV] MMIMTV_StartStbDisableTimer if timer id = %d"
#define MMIMTV_OSD_DISPLAY_5580_112_2_18_2_40_50_880 "[MMIMTV] MMIMTV_StartStbDisableTimer else timer id = %d"
#define MMIMTV_OSD_DISPLAY_5592_112_2_18_2_40_50_881 "[MMIMTV] MMIMTV_StopStbDisableTimer timer id = %d"
#define MMIMTV_OSD_DISPLAY_5618_112_2_18_2_40_50_882 "[MMIMTV] MMIMTV_HandleStbDisableTimer type = %d"
#define MMIMTV_OSD_DISPLAY_5659_112_2_18_2_40_50_883 "[MMIMTV] MMIMTV_DisableStb ctrl_handle = 0x%x"
#define MMIMTV_OSD_DISPLAY_5677_112_2_18_2_40_50_884 "[MMIMTV] MMIMTV_EnableStb ctrl_handle = 0x%x"
#define MMIMTV_SIMU_542_112_2_18_2_40_51_885 "mtvesg_common.c.c:MMIMTV_FreeProg()"
#define MMIMTV_SIMU_609_112_2_18_2_40_51_886 "mtvesg_common.c.c:CMMB_FreeAllProg()"
#define MMIMTV_WINTAB_267_112_2_18_2_40_53_887 "[MMIMBBMS]: MSG_GET_FOCUS cannot play audio service!"
#define MMIMTV_WINTAB_275_112_2_18_2_40_53_888 "[MMIMBBMS]: MSG_GET_FOCUS is need play service "
#define MMIMTV_WINTAB_281_112_2_18_2_40_53_889 "[MMIMBBMS]: service_id is INVALID"
#define MMIMTV_WINTAB_311_112_2_18_2_40_53_890 "[MMIMBBMS]: MSG_GET_FOCUS is need resume play"
#define MMIMTV_WINTAB_546_112_2_18_2_40_53_891 "MMIMBBMS InitDisplayParam: w %d, h %d, angle %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_MOBILETV_TRC)
TRACE_MSG(MBBMS_API_DUMMY_42_112_2_18_2_39_24_0,"MBBMS GBA: set callback function\n")
TRACE_MSG(MBBMS_API_DUMMY_48_112_2_18_2_39_24_1,"MBBMS GBA: clean callback function\n")
TRACE_MSG(MBBMS_API_DUMMY_53_112_2_18_2_39_24_2,"MBBMS GBA: set net id to [%d]\n")
TRACE_MSG(MBBMS_API_DUMMY_314_112_2_18_2_39_24_3,"MBBMS_Free_Service_Request_Ptr ... \n")
TRACE_MSG(MBBMS_API_DUMMY_329_112_2_18_2_39_24_4,"MBBMS_Free_Service_Request_Ptr END \n")
TRACE_MSG(MBBMS_API_DUMMY_344_112_2_18_2_39_24_5,"MBBMS_Free_Service_Request_Res_Ptr ...\n")
TRACE_MSG(MBBMS_API_DUMMY_370_112_2_18_2_39_24_6,"MBBMS_Free_Service_Request_Res_Ptr END \n")
TRACE_MSG(MMIMBBMS_MAIN_678_112_2_18_2_39_27_7,"MMIMBBMS_Copy2AnyURI src_ptr:0x%x dst_ptr:0x%x")
TRACE_MSG(MMIMBBMS_MAIN_693_112_2_18_2_39_27_8,"MMIMBBMS_Copy2AnyURI alloc memory fail")
TRACE_MSG(MMIMBBMS_MAIN_725_112_2_18_2_39_27_9,"MMIMBBMS_SetFlowInfo MMIMBBMS_FLOW_MAX <= flow_type")
TRACE_MSG(MMIMBBMS_MAIN_731_112_2_18_2_39_27_10,"MMIMBBMS_SetFlowInfo MMIMBBMS_FLOW_STATE_MAX <= status")
TRACE_MSG(MMIMBBMS_MAIN_758_112_2_18_2_39_27_11,"MMIMBBMS_StopFlow curr flow status=%d,curr_flow type =%d")
TRACE_MSG(MMIMBBMS_MAIN_781_112_2_18_2_39_27_12,"MMIMBBMS_StopFlow MTVSERVICE_StopParseServiceGuide ret = %d")
TRACE_MSG(MMIMBBMS_MAIN_799_112_2_18_2_39_27_13,"MMIMBBMS_StopFlow MTVSERVICE_StopParseServiceGuide ret = %d")
TRACE_MSG(MMIMBBMS_MAIN_817_112_2_18_2_39_27_14,"MMIMBBMS_StopFlow MTVSERVICE_StopParseESG ret = %d")
TRACE_MSG(MMIMBBMS_MAIN_835_112_2_18_2_39_27_15,"MMIMBBMS_StopFlow MTVSERVICE_DeactiveMBBMS ret = %d")
TRACE_MSG(MMIMBBMS_MAIN_856_112_2_18_2_39_27_16,"MMIMBBMS_StopFlow MTVSERVICE_Stop ret = %d")
TRACE_MSG(MMIMBBMS_MAIN_881_112_2_18_2_39_27_17,"MMIMBBMS_StopFlow MTVSERVICE_MBMSStopSubUpdate ret = %d")
TRACE_MSG(MMIMBBMS_MAIN_899_112_2_18_2_39_27_18,"MMIMBBMS_StopFlow MTVSERVICE_MBMSStopServiceRequest ret = %d")
TRACE_MSG(MMIMBBMS_MAIN_918_112_2_18_2_39_28_19,"MMIMBBMS_StopFlow MTVSERVICE_MBMSStopUnsubRequest ret = %d")
TRACE_MSG(MMIMBBMS_MAIN_936_112_2_18_2_39_28_20,"MMIMBBMS_StopFlow MTVSERVICE_MBMSStopGBA ret = %d")
TRACE_MSG(MMIMBBMS_MAIN_954_112_2_18_2_39_28_21,"MMIMBBMS_StopFlow MTVSERVICE_MBMSStopAccountQuiry ret = %d")
TRACE_MSG(MMIMBBMS_MAIN_983_112_2_18_2_39_28_22,"MMIMBBMS_SetSubUpdateType MMIMBBMS_SUB_UPDATE_FROM_TYPE_MAX <= sub_update_type")
TRACE_MSG(MMIMBBMS_MAIN_987_112_2_18_2_39_28_23,"MMIMBBMS_SetSubUpdateType sub_update_type = %d\n")
TRACE_MSG(MMIMBBMS_MAIN_1008_112_2_18_2_39_28_24,"MMIMBBMS_SetGBAOperationType MMIMBBMS_GBA_MRK_FROM_TYPE_MAX <= gba_operation_type")
TRACE_MSG(MMIMBBMS_MAIN_1031_112_2_18_2_39_28_25,"MMIMBBMS_SetSubscribeFromType  type >= MMIMBBMS_SUBSCRIBE_FROM_TYPE_MAX")
TRACE_MSG(MMIMBBMS_MAIN_1055_112_2_18_2_39_28_26,"MMIMBBMS_SetPlayFromType MMIMBBMS_PLAY_FROM_MAX <= play_from_type")
TRACE_MSG(MMIMBBMS_MAIN_1094_112_2_18_2_39_28_27,"MMIMBBMS_Init")
TRACE_MSG(MMIMBBMS_MAIN_1160_112_2_18_2_39_28_28,"MMIMBBMS_ActivePDPContext  active_info_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_1182_112_2_18_2_39_28_29,"MMIMBBMS_ActivePDPContext:apn=%s,username=%s,psw=%s,dual_sys=%d")
TRACE_MSG(MMIMBBMS_MAIN_1191_112_2_18_2_39_28_30,"MMIMBBMS_ActivePDPContext  MMIAPIPDP_Active failed!")
TRACE_MSG(MMIMBBMS_MAIN_1218_112_2_18_2_39_28_31,"MMIMBBMS_DeactivePDPContext ret=%d\n")
TRACE_MSG(MMIMBBMS_MAIN_1235_112_2_18_2_39_28_32,"MMIMBBMS_HandlePDPMsg msg_ptr:0x%x mtvservice_pdp_callback:0x%x")
TRACE_MSG(MMIMBBMS_MAIN_1241_112_2_18_2_39_28_33,"MMIMBBMS_HandlePDPMsg:mbbms is not open,ignore msg =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_1245_112_2_18_2_39_28_34,"MMIMBBMS_HandlePDPMsg: msg_id=%d, result=%d\n")
TRACE_MSG(MMIMBBMS_MAIN_1250_112_2_18_2_39_28_35,"MMIMBBMS_HandlePDPMsg: ignore MMIPDP_DEACTIVE_CNF msg!")
TRACE_MSG(MMIMBBMS_MAIN_1289_112_2_18_2_39_28_36,"MMIMBBMS_StartGBAOperation:card not support mtv\n")
TRACE_MSG(MMIMBBMS_MAIN_1298_112_2_18_2_39_28_37,"MMIMBBMS_StartGBAOperation:gba is ready\n")
TRACE_MSG(MMIMBBMS_MAIN_1303_112_2_18_2_39_28_38,"MMIMBBMS_StartGBAOperation:start gba init process start!\n")
TRACE_MSG(MMIMBBMS_MAIN_1307_112_2_18_2_39_28_39,"MMIMBBMS_StartGBAOperation flow status is not MMIMBBMS_FLOW_STATE_IDLE")
TRACE_MSG(MMIMBBMS_MAIN_1320_112_2_18_2_39_28_40,"MMIMBBMS_StartGBAOperation gba from type =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_1326_112_2_18_2_39_28_41,"MMIMBBMS_StartGBAOperation  MTVSERVICE_StartGBA return fail ")
TRACE_MSG(MMIMBBMS_MAIN_1341_112_2_18_2_39_28_42,"[MMIMBBMS]: MMIMBBMS_Play freq_no = %d service_id = %d")
TRACE_MSG(MMIMBBMS_MAIN_1365_112_2_18_2_39_28_43,"[MMIMBBMS]: MMIMBBMS_Play MMIMTV_MAIN_WIN_ID is in focus")
TRACE_MSG(MMIMBBMS_MAIN_1398_112_2_18_2_39_29_44,"[MMIMBBMS]: MMIMBBMS_Play  MMK_WinGrabFocus(MMIMTV_MAIN_WIN_ID)")
TRACE_MSG(MMIMBBMS_MAIN_1413_112_2_18_2_39_29_45,"[MMIMBBMS]: MMIMBBMS_Play  MMIMTV_OpenMTVPlayer()")
TRACE_MSG(MMIMBBMS_MAIN_1432_112_2_18_2_39_29_46,"MMIMBBMS_StartManualUpdateSG from type=%d service_mode=%d")
TRACE_MSG(MMIMBBMS_MAIN_1436_112_2_18_2_39_29_47,"MMIMBBMS_StartManualUpdateSG flow status is not MMIMBBMS_FLOW_STATE_IDLE")
TRACE_MSG(MMIMBBMS_MAIN_1451_112_2_18_2_39_29_48,"MMIMBBMS_StartManualUpdateSG gateway =%s,port=%d\n")
TRACE_MSG(MMIMBBMS_MAIN_1459_112_2_18_2_39_29_49,"MMIMBBMS_StartManualUpdateSG connection setting failed wap_ret=%d, sg_ret=%d")
TRACE_MSG(MMIMBBMS_MAIN_1476_112_2_18_2_39_29_50,"MMIMBBMS_StartManualUpdateSG MTVSERVICE_SetSGAreaName name_len=%d error_code = %d")
TRACE_MSG(MMIMBBMS_MAIN_1482_112_2_18_2_39_29_51,"MMIMBBMS_StartManualUpdateSG  MTVSERVICE_ManualUpdateSG return fail")
TRACE_MSG(MMIMBBMS_MAIN_1492_112_2_18_2_39_29_52,"MMIMBBMS_StartManualUpdateSG  MTVSERVICE_ManualUpdateSG return fail")
TRACE_MSG(MMIMBBMS_MAIN_1532_112_2_18_2_39_29_53,"MMIMBBMS_GetServiceWapSetting wap_addr =%s ,ret_val  =%d")
TRACE_MSG(MMIMBBMS_MAIN_1577_112_2_18_2_39_29_54,"MMIMBBMS_GetNAFSettingStatus naf addr =%s ,ret_val  =%d")
TRACE_MSG(MMIMBBMS_MAIN_1604_112_2_18_2_39_29_55,"MMIMBBMS_HandleGBAOperationSignal sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_1609_112_2_18_2_39_29_56,"MMIMBBMS_HandleGBAOperationSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_1613_112_2_18_2_39_29_57,"MMIMBBMS_HandleGBAOperationSignal:  cur flow is stopping")
TRACE_MSG(MMIMBBMS_MAIN_1639_112_2_18_2_39_29_58,"MMIMBBMS_HandleGBAOperationSignal subtype MBBMS_SERVICE_GBA")
TRACE_MSG(MMIMBBMS_MAIN_1675_112_2_18_2_39_29_59,"MMIMBBMS_HandleGBAOperationSignal subtype MBBMS_SERVICE_PDP_ACTIVE")
TRACE_MSG(MMIMBBMS_MAIN_1679_112_2_18_2_39_29_60,"MMIMBBMS_HandleGBAOperationSignal  subtype error!")
TRACE_MSG(MMIMBBMS_MAIN_1685_112_2_18_2_39_29_61,"MMIMBBMS_HandleGBAOperationSignal  cur flow status error!")
TRACE_MSG(MMIMBBMS_MAIN_1700_112_2_18_2_39_29_62,"MMIMBBMS_HandleStopGBAOperationSignal sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_1705_112_2_18_2_39_29_63,"MMIMBBMS_HandleStopGBAOperationSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_1731_112_2_18_2_39_29_64,"MMIMBBMS_EnterIntoMainWindow\n")
TRACE_MSG(MMIMBBMS_MAIN_1761_112_2_18_2_39_29_65,"MMIMBBMS_HandleNeedUpdateSubscription operation_type =%d")
TRACE_MSG(MMIMBBMS_MAIN_1827_112_2_18_2_39_30_66,"HandleSubscribeMainFlowFinish sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_1831_112_2_18_2_39_30_67,"[MMIMBBMS]HandleSubscribeMainFlowFinish:curr_status= %d")
TRACE_MSG(MMIMBBMS_MAIN_1840_112_2_18_2_39_30_68,"[MMIMBBMS]HandleSubscribeMainFlowFinish:Subscrible successfully!\n")
TRACE_MSG(MMIMBBMS_MAIN_1857_112_2_18_2_39_30_69,"HandleSubscribeMainFlowFinish alloc mem fail")
TRACE_MSG(MMIMBBMS_MAIN_1973_112_2_18_2_39_30_70,"MMIMBBMS HandleSubscribeMainFlowFinish:Subscrible  failed !\n")
TRACE_MSG(MMIMBBMS_MAIN_2011_112_2_18_2_39_30_71,"MMIMBBMS_HandleSubscribeSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_2016_112_2_18_2_39_30_72,"MMIMBBMS_HandleSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_2020_112_2_18_2_39_30_73,"MMIMBBMS_HandleSubscribeSignal:  cur flow is stopping")
TRACE_MSG(MMIMBBMS_MAIN_2065_112_2_18_2_39_30_74,"MMIMBBMS_HandleSubscribeSignal subtype MBBMS_SERVICE_SUBSCRIBE")
TRACE_MSG(MMIMBBMS_MAIN_2070_112_2_18_2_39_30_75,"MMIMBBMS_HandleSubscribeSignal subtype MBBMS_SERVICE_PDP_ACTIVE")
TRACE_MSG(MMIMBBMS_MAIN_2074_112_2_18_2_39_30_76,"MMIMBBMS_HandleSubscribeSignal subtype MBBMS_SERVICE_GBA")
TRACE_MSG(MMIMBBMS_MAIN_2081_112_2_18_2_39_30_77,"MMIMBBMS_HandleSubscribeSignal  subtype error!")
TRACE_MSG(MMIMBBMS_MAIN_2087_112_2_18_2_39_30_78,"MMIMBBMS_HandleSubscribeSignal  cur flow status error!")
TRACE_MSG(MMIMBBMS_MAIN_2102_112_2_18_2_39_30_79,"MMIMBBMS_HandleStopSubscribeSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_2107_112_2_18_2_39_30_80,"MMIMBBMS_HandleStopSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_2143_112_2_18_2_39_30_81,"HandleUnsubscribeMainFlowFinish: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_2147_112_2_18_2_39_30_82,"[MMIMBBMS]HandleUnsubscribeMainFlowFinish:start")
TRACE_MSG(MMIMBBMS_MAIN_2173_112_2_18_2_39_31_83,"[MMIMBBMS]HandleUnsubscribeMainFlowFinish purchase_table_info_ptr = 0x%x\n")
TRACE_MSG(MMIMBBMS_MAIN_2189_112_2_18_2_39_31_84,"[MMIMBBMS]HandleUnsubscribeMainFlowFinish anyURI_len = %d\n")
TRACE_MSG(MMIMBBMS_MAIN_2207_112_2_18_2_39_31_85,"[MMIMBBMS]HandleUnsubscribeMainFlowFinish purchase_item_error_code =%dn")
TRACE_MSG(MMIMBBMS_MAIN_2231_112_2_18_2_39_31_86,"MMIMBBMS_HandleUnSubscribeSignal:UnSubscrible successfully!\n")
TRACE_MSG(MMIMBBMS_MAIN_2245_112_2_18_2_39_31_87,"MMIMBBMS_HandleUnSubscribeSignal alloc mem fail")
TRACE_MSG(MMIMBBMS_MAIN_2332_112_2_18_2_39_31_88,"[MMIMBBMS]HandleUnsubscribeMainFlowFinish: unsubscrible failed!\n")
TRACE_MSG(MMIMBBMS_MAIN_2369_112_2_18_2_39_31_89,"MMIMBBMS_HandleUnSubscribeSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_2374_112_2_18_2_39_31_90,"MMIMBBMS_HandleUnSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_2378_112_2_18_2_39_31_91,"MMIMBBMS_HandleUnSubscribeSignal:  cur flow is stopping")
TRACE_MSG(MMIMBBMS_MAIN_2418_112_2_18_2_39_31_92,"MMIMBBMS_HandleUnSubscribeSignal subtype MBBMS_SERVICE_UNSUBSCRIBE")
TRACE_MSG(MMIMBBMS_MAIN_2423_112_2_18_2_39_31_93,"MMIMBBMS_HandleUnSubscribeSignal subtype MBBMS_SERVICE_PDP_ACTIVE")
TRACE_MSG(MMIMBBMS_MAIN_2427_112_2_18_2_39_31_94,"MMIMBBMS_HandleUnSubscribeSignal subtype MBBMS_SERVICE_GBA")
TRACE_MSG(MMIMBBMS_MAIN_2434_112_2_18_2_39_31_95,"MMIMBBMS_HandleUnSubscribeSignal subtype error!")
TRACE_MSG(MMIMBBMS_MAIN_2440_112_2_18_2_39_31_96,"MMIMBBMS_HandleUnSubscribeSignal  cur flow status error!")
TRACE_MSG(MMIMBBMS_MAIN_2455_112_2_18_2_39_31_97,"MMIMBBMS_HandleStopUnSubscribeSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_2460_112_2_18_2_39_31_98,"MMIMBBMS_HandleStopUnSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_2488_112_2_18_2_39_31_99,"MMIMBBMS_HandleAccountInquirySignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_2493_112_2_18_2_39_31_100,"MMIMBBMS_HandleAccountInquirySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_2497_112_2_18_2_39_31_101,"MMIMBBMS_HandleAccountInquirySignal:  cur flow is stopping")
TRACE_MSG(MMIMBBMS_MAIN_2525_112_2_18_2_39_31_102,"MMIMBBMS_HandleAccountInquirySignal subtype MBBMS_SERVICE_ACCOUNT_QUIRY")
TRACE_MSG(MMIMBBMS_MAIN_2567_112_2_18_2_39_31_103,"MMIMBBMS_HandleAccountInquirySignal subtype MBBMS_SERVICE_PDP_ACTIVE")
TRACE_MSG(MMIMBBMS_MAIN_2571_112_2_18_2_39_31_104,"MMIMBBMS_HandleAccountInquirySignal subtype error!")
TRACE_MSG(MMIMBBMS_MAIN_2577_112_2_18_2_39_31_105,"MMIMBBMS_HandleAccountInquirySignal  cur flow status error!")
TRACE_MSG(MMIMBBMS_MAIN_2591_112_2_18_2_39_31_106,"MMIMBBMS_HandleStopAccountInquirySignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_2596_112_2_18_2_39_31_107,"MMIMBBMS_HandleStopAccountInquirySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_2627_112_2_18_2_39_31_108,"MMIMBBMS_GetNameFromNameList: name_list_head is null")
TRACE_MSG(MMIMBBMS_MAIN_2676_112_2_18_2_39_32_109,"MMIMBBMS_GetPurchaseNameByIndex: purchaseindex_list_head is null")
TRACE_MSG(MMIMBBMS_MAIN_2746_112_2_18_2_39_32_110,"MMIMBBMS_GetPurchasePriceByIndex: purchaseindex_list_head is null")
TRACE_MSG(MMIMBBMS_MAIN_2774_112_2_18_2_39_32_111,"MMIMBBMS_GetPurchasePriceByIndex not find price info according to currency type")
TRACE_MSG(MMIMBBMS_MAIN_2782_112_2_18_2_39_32_112,"MMIMBBMS_GetPurchasePriceByIndex.....get node failed!\n")
TRACE_MSG(MMIMBBMS_MAIN_2804_112_2_18_2_39_32_113,"MMIMBBMS_GetPurchaseOrderStatusByIndex: purchaseindex_list_head is null")
TRACE_MSG(MMIMBBMS_MAIN_2810_112_2_18_2_39_32_114,"MMIMBBMS_GetPurchaseOrderStatusByIndex: index error\n")
TRACE_MSG(MMIMBBMS_MAIN_2823_112_2_18_2_39_32_115,"[MMIMBBMS] Get purchase item order status=%d\n")
TRACE_MSG(MMIMBBMS_MAIN_2851_112_2_18_2_39_32_116,"MMIMBBMS_DisplayAllPurchaseItem:Get Purchase Item failed\n")
TRACE_MSG(MMIMBBMS_MAIN_2860_112_2_18_2_39_32_117,"---no purchaseitem\n")
TRACE_MSG(MMIMBBMS_MAIN_2868_112_2_18_2_39_32_118,"MMIMBBMS_DisplayAllPurchaseItem total purchase num=%d\n")
TRACE_MSG(MMIMBBMS_MAIN_2902_112_2_18_2_39_32_119,"MMIMBBMS_DisplayAllPurchaseItem purchase name is null\n")
TRACE_MSG(MMIMBBMS_MAIN_2925_112_2_18_2_39_32_120,"MMIMBBMS_DisplayAllPurchaseItem purchase_display_info.period = %d, purchase_display_info.scription_type = %d\n")
TRACE_MSG(MMIMBBMS_MAIN_2932_112_2_18_2_39_32_121,"MMIMBBMS_DisplayAllPurchaseItem complete add purchase item\n")
TRACE_MSG(MMIMBBMS_MAIN_2947_112_2_18_2_39_32_122,"MMIMBBMS_IsLocalPurchaseItem: pGlobalPurchaseItemID is null")
TRACE_MSG(MMIMBBMS_MAIN_3014_112_2_18_2_39_32_123,"MMIMBBMS_SetCurrPurchaseDisplayInfo:Get purchase item index error\n")
TRACE_MSG(MMIMBBMS_MAIN_3031_112_2_18_2_39_32_124,"MMIMBBMS_SetCurrPurchaseDisplayInfo:purchase_name_ptr is null\n")
TRACE_MSG(MMIMBBMS_MAIN_3121_112_2_18_2_39_32_125,"MMIMBBMS_DisplayServiceInPurchase: temp is null")
TRACE_MSG(MMIMBBMS_MAIN_3135_112_2_18_2_39_32_126,"MMIMBBMS_DisplayServiceInPurchase service.NameList =0x%x, len=%dn")
TRACE_MSG(MMIMBBMS_MAIN_3171_112_2_18_2_39_33_127,"MMIMBBMS_DisplayContentInPurchase: temp is null")
TRACE_MSG(MMIMBBMS_MAIN_3239_112_2_18_2_39_33_128,"MMIMBBMS_DisplayServiceOrContentInPurchase:Get purchase item index error\n")
TRACE_MSG(MMIMBBMS_MAIN_3275_112_2_18_2_39_33_129,"MMIMBBMS_DisplayServiceOrContentInPurchase purchase mbbms_string_description1.wstr_len =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_3282_112_2_18_2_39_33_130,"MMIMBBMS_DisplayServiceOrContentInPurchase purchase mbbms_string_description2.wstr_len =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_3287_112_2_18_2_39_33_131,"MMIMBBMS_DisplayServiceOrContentInPurchase purchase mbbms_string_description.wstr_len =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_3364_112_2_18_2_39_33_132,"MBBMSSG_GetPurchaseItemIndex:Get purchase item index error\n")
TRACE_MSG(MMIMBBMS_MAIN_3375_112_2_18_2_39_33_133,"MMIMBBMS_SubscriblePurchaseItem: get purchase info error\n")
TRACE_MSG(MMIMBBMS_MAIN_3382_112_2_18_2_39_33_134,"MMIMBBMS_SubscriblePurchaseItem: s_subscrible_request_ptr  is not null\n")
TRACE_MSG(MMIMBBMS_MAIN_3391_112_2_18_2_39_33_135,"MMIMBBMS_SubscriblePurchaseItem: s_subscrible_request_ptr alloc fail")
TRACE_MSG(MMIMBBMS_MAIN_3400_112_2_18_2_39_33_136,"MMIMBBMS_SubscriblePurchaseItem: data_ref_ptr alloc fail")
TRACE_MSG(MMIMBBMS_MAIN_3427_112_2_18_2_39_33_137,"MBBMSSG_GetGlobalPurchaseItemtId gloabl_id=%s")
TRACE_MSG(MMIMBBMS_MAIN_3431_112_2_18_2_39_33_138,"MBBMS_Service_Request return = %d")
TRACE_MSG(MMIMBBMS_MAIN_3435_112_2_18_2_39_33_139,"MMIMBBMS_SubscriblePurchaseItem:Service Request error\n")
TRACE_MSG(MMIMBBMS_MAIN_3462_112_2_18_2_39_33_140," MMIMBBMS_SDandOpenCondition OK!")
TRACE_MSG(MMIMBBMS_MAIN_3537_112_2_18_2_39_33_141,"MMIMBBMS_HandleManualUpdateSGSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_3542_112_2_18_2_39_33_142,"MMIMBBMS_HandleManualUpdateSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_3546_112_2_18_2_39_33_143,"MMIMBBMS_HandleManualUpdateSGSignal:  cur flow is stopping")
TRACE_MSG(MMIMBBMS_MAIN_3563_112_2_18_2_39_33_144,"MMIMBBMS_HandleManualUpdateSGSignal  cur_service_mode=%d")
TRACE_MSG(MMIMBBMS_MAIN_3572_112_2_18_2_39_34_145,"MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_MANUAL_SG")
TRACE_MSG(MMIMBBMS_MAIN_3589_112_2_18_2_39_34_146,"MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_UPDATE_SG")
TRACE_MSG(MMIMBBMS_MAIN_3603_112_2_18_2_39_34_147,"MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_PARSE_SG")
TRACE_MSG(MMIMBBMS_MAIN_3635_112_2_18_2_39_34_148,"MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_UPDATE_TABLE")
TRACE_MSG(MMIMBBMS_MAIN_3671_112_2_18_2_39_34_149,"MMIMBBMS_HandleManualUpdateSGSignal subtype CMMB_SEARCH_PROG")
TRACE_MSG(MMIMBBMS_MAIN_3676_112_2_18_2_39_34_150,"MMIMBBMS_HandleActiveMBBMSSignal  subtype error!")
TRACE_MSG(MMIMBBMS_MAIN_3682_112_2_18_2_39_34_151,"MMIMBBMS_HandleManualUpdateSGSignal  cur flow status error!")
TRACE_MSG(MMIMBBMS_MAIN_3696_112_2_18_2_39_34_152,"MMIMBBMS_HandleStopManualUpdateSGSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_3701_112_2_18_2_39_34_153,"MMIMBBMS_HandleStopManualUpdateSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_3723_112_2_18_2_39_34_154,"MMIMBBMS_HandleParseSGSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_3728_112_2_18_2_39_34_155,"MMIMBBMS_HandleParseSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_3735_112_2_18_2_39_34_156,"MMIMBBMS_HandleParseSGSignal:  cur flow is stopping")
TRACE_MSG(MMIMBBMS_MAIN_3784_112_2_18_2_39_34_157,"MMIMBBMS_HandleParseSGSignal cur flow status error!")
TRACE_MSG(MMIMBBMS_MAIN_3800_112_2_18_2_39_34_158,"MMIMBBMS_HandleStopParseSGSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_3805_112_2_18_2_39_34_159,"MMIMBBMS_HandleStopParseSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_3826_112_2_18_2_39_34_160,"MMIMBBMS_HandleParseESGSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_3831_112_2_18_2_39_34_161,"MMIMBBMS_HandleParseESGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_3838_112_2_18_2_39_34_162,"MMIMBBMS_HandleParseESGSignal:  cur flow is stopping")
TRACE_MSG(MMIMBBMS_MAIN_3860_112_2_18_2_39_34_163,"MMIMBBMS_HandleParseESGSignal:  subtype MBBMS_SERVICE_PARSE_ESG")
TRACE_MSG(MMIMBBMS_MAIN_3872_112_2_18_2_39_34_164,"MMIMBBMS_HandleParseESGSignal cur flow status error!")
TRACE_MSG(MMIMBBMS_MAIN_3888_112_2_18_2_39_34_165,"MMIMBBMS_HandleStopParseESGSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_3893_112_2_18_2_39_34_166,"MMIMBBMS_HandleStopParseESGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_3921_112_2_18_2_39_34_167,"MMIMBBMS_HandleActiveMBBMSSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_3926_112_2_18_2_39_34_168,"MMIMBBMS_HandleActiveMBBMSSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_3932_112_2_18_2_39_34_169,"MMIMBBMS_HandleActiveMBBMSSignal:  cur flow is stopping")
TRACE_MSG(MMIMBBMS_MAIN_3957_112_2_18_2_39_34_170,"MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_UPDATE_SG/MBBMS_SERVICE_PARSE_SG")
TRACE_MSG(MMIMBBMS_MAIN_3991_112_2_18_2_39_34_171,"MMIMBBMS_HandleActiveMBBMSSignal  subtype result error!")
TRACE_MSG(MMIMBBMS_MAIN_3996_112_2_18_2_39_34_172,"MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_UPDATE_TABLE")
TRACE_MSG(MMIMBBMS_MAIN_4034_112_2_18_2_39_35_173,"MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_GBA")
TRACE_MSG(MMIMBBMS_MAIN_4058_112_2_18_2_39_35_174,"MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_ACTIVE")
TRACE_MSG(MMIMBBMS_MAIN_4069_112_2_18_2_39_35_175,"MMIMBBMS_HandleActiveMBBMSSignal  subtype error!")
TRACE_MSG(MMIMBBMS_MAIN_4075_112_2_18_2_39_35_176,"MMIMBBMS_HandleActiveMBBMSSignal  cur flow status error!")
TRACE_MSG(MMIMBBMS_MAIN_4089_112_2_18_2_39_35_177,"MMIMBBMS_HandleStopActiveMBBMSSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_4094_112_2_18_2_39_35_178,"MMIMBBMS_HandleStopActiveMBBMSSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_4111_112_2_18_2_39_35_179,"MMIMBBMS: MBBMSActiveFailed")
TRACE_MSG(MMIMBBMS_MAIN_4131_112_2_18_2_39_35_180,"MMIMBBMS_HandleUpdateSubTableSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_4136_112_2_18_2_39_35_181,"MMIMBBMS_HandleUpdateSubTableSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_4140_112_2_18_2_39_35_182,"MMIMBBMS_HandleUpdateSubTableSignal:  cur flow is stopping")
TRACE_MSG(MMIMBBMS_MAIN_4168_112_2_18_2_39_35_183,"MMIMBBMS_HandleUpdateSubTableSignal subtype MBBMS_SERVICE_UPDATE_TABLE")
TRACE_MSG(MMIMBBMS_MAIN_4202_112_2_18_2_39_35_184,"MMIMBBMS_HandleUpdateSubTableSignal subtype MBBMS_SERVICE_PDP_ACTIVE")
TRACE_MSG(MMIMBBMS_MAIN_4206_112_2_18_2_39_35_185,"MMIMBBMS_HandleActiveMBBMSSignal  subtype error!")
TRACE_MSG(MMIMBBMS_MAIN_4212_112_2_18_2_39_35_186,"MMIMBBMS_HandleUpdateSubTableSignal  cur flow status error!")
TRACE_MSG(MMIMBBMS_MAIN_4227_112_2_18_2_39_35_187,"MMIMBBMS_HandleStopUpdateSubTableSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_4232_112_2_18_2_39_35_188,"MMIMBBMS_HandleStopUpdateSubTableSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_4258_112_2_18_2_39_35_189,"MMIMBBMS_HandleStartPlaySignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_4263_112_2_18_2_39_35_190,"MMIMBBMS_HandleStartPlaySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_4278_112_2_18_2_39_35_191,"MMIMBBMS_HandleStartPlaySignal is not exiting while MMIMBBMS_FLOW_STATE_STOPPING")
TRACE_MSG(MMIMBBMS_MAIN_4290_112_2_18_2_39_35_192,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PDP_ACTIVE")
TRACE_MSG(MMIMBBMS_MAIN_4294_112_2_18_2_39_35_193,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_GBA")
TRACE_MSG(MMIMBBMS_MAIN_4298_112_2_18_2_39_35_194,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_GETMSK")
TRACE_MSG(MMIMBBMS_MAIN_4302_112_2_18_2_39_35_195,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY")
TRACE_MSG(MMIMBBMS_MAIN_4311_112_2_18_2_39_35_196,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_RSSI_CHANGED_IND")
TRACE_MSG(MMIMBBMS_MAIN_4316_112_2_18_2_39_35_197,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY_ERROR_IND")
TRACE_MSG(MMIMBBMS_MAIN_4321_112_2_18_2_39_35_198,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY_AV_IND")
TRACE_MSG(MMIMBBMS_MAIN_4326_112_2_18_2_39_35_199,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_EMERGENCY_BROADCAST_IND")
TRACE_MSG(MMIMBBMS_MAIN_4331_112_2_18_2_39_35_200,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_CA_NOT_GRANTED_IND")
TRACE_MSG(MMIMBBMS_MAIN_4336_112_2_18_2_39_35_201,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY_DRA_IND")
TRACE_MSG(MMIMBBMS_MAIN_4341_112_2_18_2_39_35_202,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_BAD_SIGNAL_IND")
TRACE_MSG(MMIMBBMS_MAIN_4346_112_2_18_2_39_35_203,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_SIGNAL_RECOVERED_IND")
TRACE_MSG(MMIMBBMS_MAIN_4351_112_2_18_2_39_35_204,"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_SIGNAL_RELOCKING_IND")
TRACE_MSG(MMIMBBMS_MAIN_4356_112_2_18_2_39_35_205,"MMIMBBMS_HandleStartPlaySignal  subtype ignored!")
TRACE_MSG(MMIMBBMS_MAIN_4409_112_2_18_2_39_35_206,"MMIMBBMS HandlePlayAck result = %d")
TRACE_MSG(MMIMBBMS_MAIN_4466_112_2_18_2_39_36_207,"MMIMBBMS HandlePlayErrorIndMsg")
TRACE_MSG(MMIMBBMS_MAIN_4493_112_2_18_2_39_36_208,"MMIMBBMS HandleRssiChangedMsg rssi_value = %d")
TRACE_MSG(MMIMBBMS_MAIN_4518_112_2_18_2_39_36_209,"MMIMBBMS_HandleStartPlaySignal subtype av_ind  video_ind=%d, audio_ind=%d")
TRACE_MSG(MMIMBBMS_MAIN_4548_112_2_18_2_39_36_210,"MMIMBBMS HandleEmergencyBroadcastMsg")
TRACE_MSG(MMIMBBMS_MAIN_4660_112_2_18_2_39_36_211,"MMIMBBMS HandleCANotGrantedMsg")
TRACE_MSG(MMIMBBMS_MAIN_4685_112_2_18_2_39_36_212,"MMIMBBMS HandleDRAIndMsg")
TRACE_MSG(MMIMBBMS_MAIN_4699_112_2_18_2_39_36_213,"MMIMBBMS HandleBadSignalMsg")
TRACE_MSG(MMIMBBMS_MAIN_4724_112_2_18_2_39_36_214,"MMIMBBMS HandleSignalRecoveredMsg")
TRACE_MSG(MMIMBBMS_MAIN_4749_112_2_18_2_39_36_215,"MMIMBBMS HandleRelockingMsg")
TRACE_MSG(MMIMBBMS_MAIN_4766_112_2_18_2_39_36_216,"MMIMBBMS_HandleStopPlaySignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_4771_112_2_18_2_39_36_217,"MMIMBBMS_HandleStopPlaySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_4816_112_2_18_2_39_36_218,"MMIMBBMS_HandleSGGetComplex: err_code = %d")
TRACE_MSG(MMIMBBMS_MAIN_4855_112_2_18_2_39_37_219,"MMIMBBMS_HandleSGStopComplex: err_code = %d")
TRACE_MSG(MMIMBBMS_MAIN_4878_112_2_18_2_39_37_220,"MMIMBBMS_HandleMSKupdateSignal: sig_data_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_4883_112_2_18_2_39_37_221,"MMIMBBMS_HandleMSKupdateSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_MAIN_4905_112_2_18_2_39_37_222,"MMIMBBMS_IsServiceEncrypted Service encryption status =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_4910_112_2_18_2_39_37_223,"MMIMBBMS_IsServiceEncrypted Service encryption status = FALSE\n")
TRACE_MSG(MMIMBBMS_MAIN_4937_112_2_18_2_39_37_224,"MMIMBBMS_IsContentEncrypted content encryted status  =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_4955_112_2_18_2_39_37_225,"MMIMBBMS_IsPurchaseItemOrdered error_code=%d,is_ordered=%d")
TRACE_MSG(MMIMBBMS_MAIN_4985_112_2_18_2_39_37_226,"MMIMBBMS_IsServiceOrdered get purchase result=%d\n")
TRACE_MSG(MMIMBBMS_MAIN_4990_112_2_18_2_39_37_227,"MMIMBBMS_IsServiceOrdered:not found purchase which contain this service\n")
TRACE_MSG(MMIMBBMS_MAIN_4999_112_2_18_2_39_37_228,"MMIMBBMS_IsServiceOrdered purchase_itemlist_head_ptr=0x%x\n")
TRACE_MSG(MMIMBBMS_MAIN_5003_112_2_18_2_39_37_229,"MMIMBBMS_IsServiceOrdered purchase_index_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_5010_112_2_18_2_39_37_230,"MMIMBBMS_IsServiceOrdered :Purchase ordered\n")
TRACE_MSG(MMIMBBMS_MAIN_5020_112_2_18_2_39_37_231,"MMIMBBMS_IsServiceOrdered:Get Purchase Item by sevice failded\n")
TRACE_MSG(MMIMBBMS_MAIN_5026_112_2_18_2_39_37_232,"MMIMBBMS_IsServiceOrdered:%d")
TRACE_MSG(MMIMBBMS_MAIN_5111_112_2_18_2_39_37_233,"MMIMBBMS_PlayService is  encrypted but free,then play direct\n")
TRACE_MSG(MMIMBBMS_MAIN_5116_112_2_18_2_39_37_234,"MMIMBBMS_PlayService is  encrypted but not free\n")
TRACE_MSG(MMIMBBMS_MAIN_5122_112_2_18_2_39_37_235,"MMIMBBMS_PlayService:service is orderd and then check gba \n")
TRACE_MSG(MMIMBBMS_MAIN_5127_112_2_18_2_39_37_236,"MMIMBBMS_PlayService:service is not orderd")
TRACE_MSG(MMIMBBMS_MAIN_5133_112_2_18_2_39_37_237,"MMIMBBMS_OpenSubscriblePromptWin MMIMTV_MAIN_WIN_ID is focus")
TRACE_MSG(MMIMBBMS_MAIN_5149_112_2_18_2_39_37_238,"MMIMBBMS_PlayService: service is not encrypted and play directly\n")
TRACE_MSG(MMIMBBMS_MAIN_5181_112_2_18_2_39_37_239,"MMIMBBMS HandleContentAlertWinMsg close win")
TRACE_MSG(MMIMBBMS_MAIN_5209_112_2_18_2_39_37_240,"MMIMBBMS_PlayContent get content  error\n")
TRACE_MSG(MMIMBBMS_MAIN_5236_112_2_18_2_39_37_241,"MMIMBBMS_PlayContent is  encrypted but free,then play direct\n")
TRACE_MSG(MMIMBBMS_MAIN_5265_112_2_18_2_39_37_242,"MMIMBBMS_PlayService is  encrypted but not free\n")
TRACE_MSG(MMIMBBMS_MAIN_5271_112_2_18_2_39_37_243,"MMIMBBMS_PlayContent:service is orderd and then check gba \n")
TRACE_MSG(MMIMBBMS_MAIN_5300_112_2_18_2_39_37_244,"MMIMBBMS_PlayContent:service is not orderd")
TRACE_MSG(MMIMBBMS_MAIN_5306_112_2_18_2_39_37_245,"MMIMBBMS_OpenSubscriblePromptWin MMIMTV_MAIN_WIN_ID is focus")
TRACE_MSG(MMIMBBMS_MAIN_5322_112_2_18_2_39_37_246,"MMIMBBMS_PlayContent: service is not encrypted and play directly\n")
TRACE_MSG(MMIMBBMS_MAIN_5382_112_2_18_2_39_38_247,"MMIMBBMS_EnterPlay service cannot play audio service!")
TRACE_MSG(MMIMBBMS_MAIN_5403_112_2_18_2_39_38_248,"MMIMBBMS_EnterPlay content cannot play audio service!")
TRACE_MSG(MMIMBBMS_MAIN_5417_112_2_18_2_39_38_249,"MMIMBBMS_EnterCMMBModePlay  frequency_no=%d, service_id=%d")
TRACE_MSG(MMIMBBMS_MAIN_5421_112_2_18_2_39_38_250,"MMIMBBMS_EnterCMMBModePlay prog_t_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_5444_112_2_18_2_39_38_251,"MMIMBBMS_EnterCMMBModePlay cannot play audio service!")
TRACE_MSG(MMIMBBMS_MAIN_5467_112_2_18_2_39_38_252,"MMIMBBMS_GetNAFAddr naf_addr:0x%d, naf_addr_len:0x%x, port:0x%x")
TRACE_MSG(MMIMBBMS_MAIN_5475_112_2_18_2_39_38_253,"MMIMBBMS_GetNAFAddr error_code =%d, service_num=%d\n")
TRACE_MSG(MMIMBBMS_MAIN_5489_112_2_18_2_39_38_254,"MMIMBBMS_GetNAFAddr service_index_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_5501_112_2_18_2_39_38_255,"MMIMBBMS_GetNAFAddr get access\n")
TRACE_MSG(MMIMBBMS_MAIN_5522_112_2_18_2_39_38_256,"MMIMBBMS_GetNAFAddr temp =%s\n")
TRACE_MSG(MMIMBBMS_MAIN_5544_112_2_18_2_39_38_257,"MMIMBBMS_GetNAFAddr doesn't find")
TRACE_MSG(MMIMBBMS_MAIN_5559_112_2_18_2_39_38_258,"MMIMBBMS_GetNAFAddr naf addr =%s\n")
TRACE_MSG(MMIMBBMS_MAIN_5615_112_2_18_2_39_38_259,"MMIMBBMS_EnterMBBMS both sd and usdisk doesn't exist\n")
TRACE_MSG(MMIMBBMS_MAIN_5624_112_2_18_2_39_38_260,"MMIMBBMS_EnterMBBMS udisk no space  K\n")
TRACE_MSG(MMIMBBMS_MAIN_5711_112_2_18_2_39_38_261,"MMIMBBMS_Init: init_ptr is null")
TRACE_MSG(MMIMBBMS_MAIN_5724_112_2_18_2_39_38_262,"MMIMBBMS_Init: no u disk and sd card\n")
TRACE_MSG(MMIMBBMS_MAIN_5769_112_2_18_2_39_38_263,"MMIMBBMS_InitProcess enter  TickCount=%d")
TRACE_MSG(MMIMBBMS_MAIN_5848_112_2_18_2_39_39_264,"MMIMBBMS HandleStartUpWinMsg wchar welcome_info_len=%d")
TRACE_MSG(MMIMBBMS_MAIN_5896_112_2_18_2_39_39_265,"MMIMBBMS_OpenStartUpWin MSG_OPEN_WINDOW TickCount=%d")
TRACE_MSG(MMIMBBMS_MAIN_5911_112_2_18_2_39_39_266,"MMIMBBMS_InitProcess MTVSERVICE_Init  before TickCount=%d")
TRACE_MSG(MMIMBBMS_MAIN_5919_112_2_18_2_39_39_267,"MMIMBBMS_InitProcess MTVSERVICE_Init  after TickCount=%d")
TRACE_MSG(MMIMBBMS_MAIN_5935_112_2_18_2_39_39_268,"MMIMBBMS_OpenStartUpWin MSG_FULL_PAINT TickCount=%d")
TRACE_MSG(MMIMBBMS_MAIN_5954_112_2_18_2_39_39_269,"[MMIMBBMS]: HandleStartUpWinMsg abnormal colsed!")
TRACE_MSG(MMIMBBMS_MAIN_5958_112_2_18_2_39_39_270,"[MMIMBBMS]: close mbbms init window\n")
TRACE_MSG(MMIMBBMS_MAIN_5979_112_2_18_2_39_39_271,"MMIMBBMS_ExitMBBMS start")
TRACE_MSG(MMIMBBMS_MAIN_5983_112_2_18_2_39_39_272,"MMIMBBMS_ExitMBBMS app is already closed")
TRACE_MSG(MMIMBBMS_MAIN_5994_112_2_18_2_39_39_273,"MMIMBBMS_ExitMBBMS no service is working\n")
TRACE_MSG(MMIMBBMS_MAIN_6030_112_2_18_2_39_39_274,"[MMIMTV]: NotifyEventCallback dual_sys=%d, notify_event=%d, param=%d")
TRACE_MSG(MMIMBBMS_MAIN_6082_112_2_18_2_39_39_275,"MMIMBBMS_GetSettingInfo notify_type_ptr:0x%x, nerwork_index_ptr:0x%x, sg_ptr:0x%x, naf_ptr:0x%x")
TRACE_MSG(MMIMBBMS_MAIN_6097_112_2_18_2_39_39_276,"MMIMBBMS_GetSettingInfo sg_domain_info = %s ")
TRACE_MSG(MMIMBBMS_MAIN_6131_112_2_18_2_39_39_277,"MMIMBBMS_InitContentNotifyInfo no alarm content file")
TRACE_MSG(MMIMBBMS_MAIN_6148_112_2_18_2_39_39_278,"MMIMBBMS_InitContentNotifyInfo %d-%d-%d %d:%d")
TRACE_MSG(MMIMBBMS_MAIN_6159_112_2_18_2_39_39_279,"MMIMBBMS_InitContentNotifyInfo  %d:%d:%d")
TRACE_MSG(MMIMBBMS_MAIN_6199_112_2_18_2_39_39_280,"MMIMBBMS_InitContentNotifyInfo %d-%d-%d %d:%d")
TRACE_MSG(MMIMBBMS_MAIN_6217_112_2_18_2_39_39_281,"MMIMBBMS_InitContentNotifyInfo %d-%d-%d %d:%d")
TRACE_MSG(MMIMBBMS_MAIN_6219_112_2_18_2_39_39_282,"MMIMBBMS_InitContentNotifyInfo before update cursor->event_id = %d")
TRACE_MSG(MMIMBBMS_MAIN_6236_112_2_18_2_39_39_283,"MMIMBBMS_InitContentNotifyInfo after update cursor->event_id = %d")
TRACE_MSG(MMIMBBMS_MAIN_6333_112_2_18_2_39_40_284,"MMIMBBMS_UpdateAllContentAlarm %d-%d-%d %d:%d")
TRACE_MSG(MMIMBBMS_MAIN_6351_112_2_18_2_39_40_285,"MMIMBBMS_UpdateAllContentAlarm %d-%d-%d %d:%d")
TRACE_MSG(MMIMBBMS_MAIN_6353_112_2_18_2_39_40_286,"before update cursor->event_id = %d")
TRACE_MSG(MMIMBBMS_MAIN_6365_112_2_18_2_39_40_287,"after update cursor->event_id = %d")
TRACE_MSG(MMIMBBMS_MAIN_6466_112_2_18_2_39_40_288,"MMIMBBMS_RegisterContentAlarm pContentInfo is null")
TRACE_MSG(MMIMBBMS_MAIN_6532_112_2_18_2_39_40_289,"MMIMBBMS_RegisterContentAlarm %d-%d-%d %d:%d remindertime = %d event_id = %d")
TRACE_MSG(MMIMBBMS_MAIN_6564_112_2_18_2_39_40_290,"MMIMBBMS_GetNewRemindTime pSrcTime:0x%x, pDstTime:0x%x")
TRACE_MSG(MMIMBBMS_MAIN_6633_112_2_18_2_39_40_291,"MMIMBBMS_InitSettingInfo read nv failed\n")
TRACE_MSG(MMIMBBMS_MAIN_6682_112_2_18_2_39_40_292,"MMIMBBMS_EnterPlayprocess curr flow status=%d,curr_flow type =%d")
TRACE_MSG(MMIMBBMS_MAIN_6685_112_2_18_2_39_40_293,"MMIMBBMS_EnterPlayprocess is not permit play!")
TRACE_MSG(MMIMBBMS_MAIN_6710_112_2_18_2_39_40_294,"[MMIMBBMS]:MMIMBBMS_EnterPlayprocess   frequency_no = %d service_id = %d uri:%s")
TRACE_MSG(MMIMBBMS_MAIN_6732_112_2_18_2_39_40_295,"[MMIMBBMS]:MMIMBBMS_EnterPlayprocess frequency_no = %d service_id = %d uri:%s")
TRACE_MSG(MMIMBBMS_MAIN_6758_112_2_18_2_39_40_296,"MMIMBBMS_EnterPlayprocess cmmb service mode!")
TRACE_MSG(MMIMBBMS_MAIN_6787_112_2_18_2_39_40_297,"MMIMBBMS_StartSubscribeProcess\n")
TRACE_MSG(MMIMBBMS_MAIN_6790_112_2_18_2_39_40_298,"MMIMBBMS_StartSubscribeProcess flow status is not MMIMBBMS_FLOW_STATE_IDLE")
TRACE_MSG(MMIMBBMS_MAIN_6814_112_2_18_2_39_40_299,"MMIMBBMS_StartUnSubscribeProcess\n")
TRACE_MSG(MMIMBBMS_MAIN_6817_112_2_18_2_39_40_300,"MMIMBBMS_StartUnSubscribeProcess flow status is not MMIMBBMS_FLOW_STATE_IDLE")
TRACE_MSG(MMIMBBMS_MAIN_6841_112_2_18_2_39_41_301,"MMIMBBMS_StartUpdateSubscriptionTable from type=%d\n")
TRACE_MSG(MMIMBBMS_MAIN_6845_112_2_18_2_39_41_302,"MMIMBBMS_StartUpdateSubscriptionTable flow status is not MMIMBBMS_FLOW_STATE_IDLE")
TRACE_MSG(MMIMBBMS_MAIN_6851_112_2_18_2_39_41_303,"MMIMBBMS_StartUpdateSubscriptionTable connection setting error \n")
TRACE_MSG(MMIMBBMS_MAIN_6861_112_2_18_2_39_41_304,"MMIMBBMS_StartUpdateSubscriptionTable  MTVSERVICE_SubUpdate return fail ")
TRACE_MSG(MMIMBBMS_MAIN_6934_112_2_18_2_39_41_305,"MMIMBBMS_StartPlay  frequency_no=%d, service_id=%d cur_flow_type=%d cur_flow_status=%d cmmb_status=%d")
TRACE_MSG(MMIMBBMS_MAIN_6944_112_2_18_2_39_41_306,"MMIMBBMS_StartPlay  MTVSERVICE_Play return = %d")
TRACE_MSG(MMIMBBMS_MAIN_6963_112_2_18_2_39_41_307,"MMIMBBMS_StopPlay  MTVSERVICE_Stop return fail ")
TRACE_MSG(MMIMBBMS_MAIN_6980_112_2_18_2_39_41_308,"MMIMBBMS_StartPlay  status error in flow MMIMBBMS_FLOW_PLAY")
TRACE_MSG(MMIMBBMS_MAIN_6986_112_2_18_2_39_41_309,"MMIMBBMS_StartPlay  flow type and status error!")
TRACE_MSG(MMIMBBMS_MAIN_7000_112_2_18_2_39_41_310,"MMIMBBMS_StopPlay  cur_flow_status=%d, cur_flow_type=%d cmmb_status=%d")
TRACE_MSG(MMIMBBMS_MAIN_7023_112_2_18_2_39_41_311,"MMIMBBMS_StopPlay  MTVSERVICE_Stop return = %d")
TRACE_MSG(MMIMBBMS_MAIN_7043_112_2_18_2_39_41_312,"MMIMBBMS_StopPlay  cmmb stop complete!")
TRACE_MSG(MMIMBBMS_MAIN_7060_112_2_18_2_39_41_313,"MMIMBBMS_QuickStopPlay  cur_flow_status=%d, cur_flow_type=%d cmmb_status=%d")
TRACE_MSG(MMIMBBMS_MAIN_7077_112_2_18_2_39_41_314,"MMIMBBMS_QuickStopPlay  complete!")
TRACE_MSG(MMIMBBMS_MAIN_7100_112_2_18_2_39_41_315,"MMIMBBMS_StartParseSG process from type=%d MMIMBBMS_TickCount=%d")
TRACE_MSG(MMIMBBMS_MAIN_7125_112_2_18_2_39_41_316,"MMIMBBMS_StartParseESG main_service_count = %d")
TRACE_MSG(MMIMBBMS_MAIN_7138_112_2_18_2_39_41_317,"MMIMBBMS_StartParseESG flow status is not MMIMBBMS_FLOW_STATE_IDLE")
TRACE_MSG(MMIMBBMS_MAIN_7146_112_2_18_2_39_41_318,"MMIMBBMS_StartParseESG net_id = %d")
TRACE_MSG(MMIMBBMS_MAIN_7164_112_2_18_2_39_41_319,"MMIMBBMS_StartActiveMBBMS")
TRACE_MSG(MMIMBBMS_MAIN_7168_112_2_18_2_39_41_320,"MMIMBBMS_StartActiveMBBMS flow status is not MMIMBBMS_FLOW_STATE_IDLE")
TRACE_MSG(MMIMBBMS_MAIN_7175_112_2_18_2_39_41_321,"MMIMBBMS_StartActiveMBBMS MTVSERVICE_ActiveMBBMS return fail")
TRACE_MSG(MMIMBBMS_MAIN_7249_112_2_18_2_39_41_322,"MMIMBBMS_GetSubscriptionUpdateStatus = %d\n")
TRACE_MSG(MMIMBBMS_MAIN_7267_112_2_18_2_39_41_323,"MMIMBBMS_IsContentExpired pIndex is null")
TRACE_MSG(MMIMBBMS_MAIN_7281_112_2_18_2_39_41_324,"MMIMBBMS_IsContentExpired content start time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d")
TRACE_MSG(MMIMBBMS_MAIN_7290_112_2_18_2_39_41_325,"MMIMBBMS_IsContentExpired content end time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d")
TRACE_MSG(MMIMBBMS_MAIN_7351_112_2_18_2_39_42_326,"MMIMBBMS_IsContentExpired ret =%d")
TRACE_MSG(MMIMBBMS_MAIN_7369_112_2_18_2_39_42_327,"MMIMBBMS_CheckContentPlayTime pIndex is null")
TRACE_MSG(MMIMBBMS_MAIN_7375_112_2_18_2_39_42_328,"MMIMBBMS_CheckContentPlayTime SG_ERROR_NONE!=MBBMSSG_GetContent")
TRACE_MSG(MMIMBBMS_MAIN_7385_112_2_18_2_39_42_329,"MMIMBBMS_CheckContentPlayTime content start time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d")
TRACE_MSG(MMIMBBMS_MAIN_7393_112_2_18_2_39_42_330,"MMIMBBMS_CheckContentPlayTime content end time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d")
TRACE_MSG(MMIMBBMS_MAIN_7423_112_2_18_2_39_42_331,"MMIMBBMS_CheckContentPlayTime ret =%d")
TRACE_MSG(MMIMBBMS_MAIN_7435_112_2_18_2_39_42_332,"MMIMBBMS_Clear enter\n")
TRACE_MSG(MMIMBBMS_MAIN_7483_112_2_18_2_39_42_333,"MMIMBBMS SGSearchFailed sg search from type =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_7517_112_2_18_2_39_42_334,"MMIMBBMS: SGParseOK!")
TRACE_MSG(MMIMBBMS_MAIN_7521_112_2_18_2_39_42_335,"MMIMBBMS SGParseOK: mbbms exiting")
TRACE_MSG(MMIMBBMS_MAIN_7570_112_2_18_2_39_42_336,"MMIMBBMS: SGParseFailed")
TRACE_MSG(MMIMBBMS_MAIN_7614_112_2_18_2_39_42_337,"MMIMBBMS GBAOperationFailed GBA operation type =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_7632_112_2_18_2_39_42_338,"MMIMBBMS GBAOperationFailed:GBA operation type error!")
TRACE_MSG(MMIMBBMS_MAIN_7650_112_2_18_2_39_42_339,"MMIMBBMS GBAOperationOK:Operation type =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_7671_112_2_18_2_39_42_340,"MMIMBBMS SubTableUpateOK subscription type=%d\n")
TRACE_MSG(MMIMBBMS_MAIN_7701_112_2_18_2_39_42_341,"MMIMBBMS SubTableUpateOK:subscription update type error\n ")
TRACE_MSG(MMIMBBMS_MAIN_7718_112_2_18_2_39_42_342,"MMIMBBMS SubTableUpateFailed from type =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_7736_112_2_18_2_39_42_343,"MMIMBBMS SubTableUpateOK:subscription update type error\n ")
TRACE_MSG(MMIMBBMS_MAIN_7747_112_2_18_2_39_42_344,"MMIMBBMS_StartAccoutInqueryProcess")
TRACE_MSG(MMIMBBMS_MAIN_7751_112_2_18_2_39_42_345,"MMIMBBMS flow status is not MMIMBBMS_FLOW_STATE_IDLE")
TRACE_MSG(MMIMBBMS_MAIN_7761_112_2_18_2_39_42_346,"MMIMBBMS_StartAccoutInquiryProcess  MMIMBBMS_AccoutInquiry return fail ")
TRACE_MSG(MMIMBBMS_MAIN_7785_112_2_18_2_39_42_347,"MMIMBBMS_AccoutInquiry: s_account_inquiry_request_ptr is  not null\n")
TRACE_MSG(MMIMBBMS_MAIN_7793_112_2_18_2_39_42_348,"MMIMBBMS_AccoutInquiry: s_account_inquiry_request_ptr alloc fail")
TRACE_MSG(MMIMBBMS_MAIN_7803_112_2_18_2_39_42_349,"MMIMBBMS_AccoutInquiry: inquiry type =%d \n")
TRACE_MSG(MMIMBBMS_MAIN_7806_112_2_18_2_39_42_350,"MBBMS_Account_Inquiry return = %d")
TRACE_MSG(MMIMBBMS_MAIN_7947_112_2_18_2_39_43_351,"MMIMBBMS: MMIMBBMS_UpdateWelcomeInfo welcome_str_ptr = 0x%x, str len =%d")
TRACE_MSG(MMIMBBMS_MAIN_7960_112_2_18_2_39_43_352,"MMIMBBMS_UpdateWelcomeInfo byte1 = 0x%x, byte2 =0x%x ,byte3= 0x%x")
TRACE_MSG(MMIMBBMS_MAIN_7975_112_2_18_2_39_43_353,"MMIMBBMS_GetInitStatus = %d\n")
TRACE_MSG(MMIMBBMS_MAIN_7996_112_2_18_2_39_43_354,"MMIMBBMS_GetRemindTime index error =%d")
TRACE_MSG(MMIMBBMS_MAIN_8076_112_2_18_2_39_43_355,"MMIMBBMS ResetCurServiceInfo  cur service_id =%d")
TRACE_MSG(MMIMBBMS_MAIN_8090_112_2_18_2_39_43_356,"MMIMBBMS_PreExitCallback")
TRACE_MSG(MMIMBBMS_MAIN_8119_112_2_18_2_39_43_357,"MMIMBBMS_IsServiceAudioTyle servicetype = %d")
TRACE_MSG(MMIMBBMS_MAIN_8145_112_2_18_2_39_43_358,"MMIMBBMS_IsCurrentContentAudioTyle")
TRACE_MSG(MMIMBBMS_MAIN_8190_112_2_18_2_39_43_359,"MMIMBBMS_IsCurrentServiceAudioTyle : %d")
TRACE_MSG(MMIMBBMS_MAIN_8219_112_2_18_2_39_43_360,"MMIMBBMS_IsMBBMSAppFocous is_focus = %d")
TRACE_MSG(MMIMBBMS_MAIN_8239_112_2_18_2_39_43_361,"MMIMBBMS_SetServiceMode mode = %d")
TRACE_MSG(MMIMBBMS_MAIN_8315_112_2_18_2_39_43_362,"MMIMBBMS SendSignalToTask sig_to_mmi_ptr alloc fail")
TRACE_MSG(MMIMBBMS_MAIN_8353_112_2_18_2_39_44_363,"MMIMBBMS_DisplayAllPurchasedItem  --  account_inquiry_res_ptr = NULL.\n")
TRACE_MSG(MMIMBBMS_MAIN_8361_112_2_18_2_39_44_364,"---MMIMBBMS_DisplayAllPurchasedItem no purchaseitem\n")
TRACE_MSG(MMIMBBMS_MAIN_8367_112_2_18_2_39_44_365,"MMIMBBMS_DisplayAllPurchasedItem total purchase num=%d\n")
TRACE_MSG(MMIMBBMS_MAIN_8376_112_2_18_2_39_44_366,"---MMIMBBMS_DisplayAllPurchasedItem purchase_table_info_ptr = PNULL\n")
TRACE_MSG(MMIMBBMS_MAIN_8395_112_2_18_2_39_44_367,"MMIMBBMS_DisplayAllPurchasedItem anyURI_len = %d\n")
TRACE_MSG(MMIMBBMS_MAIN_8413_112_2_18_2_39_44_368,"MMIMBBMS_DisplayAllPurchasedItem purchase_item_error_code =%d, purchase_detail_error_code =%dn")
TRACE_MSG(MMIMBBMS_MAIN_8428_112_2_18_2_39_44_369,"MMIMBBMS_DisplayAllPurchasedItem purchase name is null\n")
TRACE_MSG(MMIMBBMS_MAIN_8497_112_2_18_2_39_44_370,"MMIMBBMS_DisplayServiceOrContentInPurchasedItem  --  account_inquiry_res_ptr = NULL.\n")
TRACE_MSG(MMIMBBMS_MAIN_8510_112_2_18_2_39_44_371,"---MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase_table_info_ptr = PNULL\n")
TRACE_MSG(MMIMBBMS_MAIN_8545_112_2_18_2_39_44_372,"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase_item_error_code =%d, purchase_detail_error_code =%dn")
TRACE_MSG(MMIMBBMS_MAIN_8553_112_2_18_2_39_44_373,"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase_item_ptr=0x%x, purchase_detail_ptr=0x%x.n")
TRACE_MSG(MMIMBBMS_MAIN_8563_112_2_18_2_39_44_374,"MMIMBBMS_DisplayServiceOrContentInPurchasedItem null in local and account inquiry return.\n")
TRACE_MSG(MMIMBBMS_MAIN_8569_112_2_18_2_39_44_375,"MMIMBBMS_DisplayServiceOrContentInPurchasedItem  --  purchase_item_ptr->NameList = NULL.\n")
TRACE_MSG(MMIMBBMS_MAIN_8579_112_2_18_2_39_44_376,"MMIMBBMS_DisplayServiceOrContentInPurchasedItem ServiceIdRefList =0x%x, len=%dn")
TRACE_MSG(MMIMBBMS_MAIN_8590_112_2_18_2_39_44_377,"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase description description_str_len =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_8595_112_2_18_2_39_44_378,"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase mbbms_string_description1.wstr_len =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_8602_112_2_18_2_39_44_379,"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase mbbms_string_description2.wstr_len =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_8607_112_2_18_2_39_44_380,"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase mbbms_string_description.wstr_len =%d\n")
TRACE_MSG(MMIMBBMS_MAIN_8634_112_2_18_2_39_44_381,"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase name is null\n")
TRACE_MSG(MMIMBBMS_MAIN_8716_112_2_18_2_39_45_382,"MMIMBBMS_SetCurrPurchasedItemDisplayInfo  --  account_inquiry_res_ptr = NULL.\n")
TRACE_MSG(MMIMBBMS_MAIN_8731_112_2_18_2_39_45_383,"---MMIMBBMS_SetCurrPurchasedItemDisplayInfo purchase_table_info_ptr = PNULL\n")
TRACE_MSG(MMIMBBMS_MAIN_8739_112_2_18_2_39_45_384,"MMIMBBMS_SetCurrPurchasedItemDisplayInfo purchase_item_ptr=0x%x, purchase_detail_ptr=0x%x.n")
TRACE_MSG(MMIMBBMS_MAIN_8768_112_2_18_2_39_45_385,"MMIMBBMS_SetCurrPurchasedItemDisplayInfo purchase_item_error_code =%d, purchase_detail_error_code =%dn")
TRACE_MSG(MMIMBBMS_MAIN_8873_112_2_18_2_39_45_386,"MMIMBBMS GetTextIdByReturnCode invalid err_code = %d")
TRACE_MSG(MMIMBBMS_MAIN_8897_112_2_18_2_39_45_387,"MMIMBBMS CheckDescriptionResValid  type=%d")
TRACE_MSG(MMIMBBMS_MAIN_8948_112_2_18_2_39_45_388,"MMIMBBMS CheckDescriptionResValid Input type error!  type=%d")
TRACE_MSG(MMIMBBMS_MAIN_8977_112_2_18_2_39_45_389,"MMIMBBMS DisplayErrorTipByDescription len=%d, min_len=%d, real_len=%d")
TRACE_MSG(MMIMBBMS_MAIN_9005_112_2_18_2_39_45_390,"MMIMBBMS CreateBgOSDLayer is_bg_layer_open = %d")
TRACE_MSG(MMIMBBMS_MAIN_9009_112_2_18_2_39_45_391,"MMIMBBMS CreateBgOSDLayer player win is not focus!")
TRACE_MSG(MMIMBBMS_MAIN_9015_112_2_18_2_39_45_392,"MMIMBBMS CreateBgOSDLayer multi-layer not support!")
TRACE_MSG(MMIMBBMS_MAIN_9020_112_2_18_2_39_45_393,"MMIMBBMS CreateBgOSDLayer Logic lcd_width=%d, lcd_height=%d")
TRACE_MSG(MMIMBBMS_MAIN_9040_112_2_18_2_39_45_394,"MMIMBBMS CreateBgOSDLayer UILAYER_CreateLayer return error:%d")
TRACE_MSG(MMIMBBMS_MAIN_9085_112_2_18_2_39_46_395,"MMIMBBMS ReleaseBgOSDLayer is_bg_layer_open=%d")
TRACE_MSG(MMIMBBMS_MAIN_9110_112_2_18_2_39_46_396,"MMIMBBMS_SetUpdateCityName: wstr_len=%d utf8_len=%d")
TRACE_MSG(MMIMBBMS_MAIN_9162_112_2_18_2_39_46_397,"MMIMBBMS_UnSubscriblePurchaseItemNew account_inquiry_res_ptr = PNULL\n")
TRACE_MSG(MMIMBBMS_MAIN_9171_112_2_18_2_39_46_398,"---MMIMBBMS_UnSubscriblePurchaseItemNew purchase_table_info_ptr = PNULL\n")
TRACE_MSG(MMIMBBMS_MAIN_9177_112_2_18_2_39_46_399,"MMIMBBMS_UnSubscriblePurchaseItemNew: s_uns_subscrible_request_ptr  is not null\n")
TRACE_MSG(MMIMBBMS_MAIN_9186_112_2_18_2_39_46_400,"MMIMBBMS_UnSubscriblePurchaseItemNew s_uns_subscrible_request_ptr alloc fail")
TRACE_MSG(MMIMBBMS_MAIN_9195_112_2_18_2_39_46_401,"MMIMBBMS_UnSubscriblePurchaseItemNew data_ref_ptr alloc fail")
TRACE_MSG(MMIMBBMS_MAIN_9209_112_2_18_2_39_46_402,"MMIMBBMS_UnSubscriblePurchaseItemNew MBBMS_Unsubscribe_Request return = %d")
TRACE_MSG(MMIMBBMS_MAIN_9212_112_2_18_2_39_46_403,"MMIMBBMS_UnSubscriblePurchaseItemNew: service request error\n")
TRACE_MSG(MMIMBBMS_MAIN_9264_112_2_18_2_39_46_404,"MMIMBBMS_GetPurchasedItemByIndex  --  index error.\n")
TRACE_MSG(MMIMBBMS_MAIN_9278_112_2_18_2_39_46_405,"MMIMBBMS_GetPurchasedItemByIndex  --  purchase_cur  NULL.\n")
TRACE_MSG(MMIMBBMS_MAIN_9294_112_2_18_2_39_46_406,"MMIMBBMS_CheckSGDD read nv from init failed\n")
TRACE_MSG(MMIMBBMS_MAIN_9303_112_2_18_2_39_46_407,"MMIMBBMS_CheckSGDD nv old url = %s, len = %d")
TRACE_MSG(MMIMBBMS_MAIN_9304_112_2_18_2_39_46_408,"MMIMBBMS_CheckSGDD nv new url = %s, len = %d")
TRACE_MSG(MMIMBBMS_MAIN_9331_112_2_18_2_39_46_409,"MMIMBBMS_SetCurSelectSim dual_sys error!")
TRACE_MSG(MMIMBBMS_MAIN_9334_112_2_18_2_39_46_410,"MMIMBBMS_SetCurSelectSim, dual_sys = %d")
TRACE_MSG(MMIMBBMS_WINTAB_1497_112_2_18_2_39_49_411,"MMIMBBMS_AddPurchaseItemToListboxCtrl purchase_display_info_ptr.period = %d, purchase_display_info_ptr.scription_type = %d\n")
TRACE_MSG(MMIMBBMS_WINTAB_1532_112_2_18_2_39_49_412,"MMIMBBMS_AddPurchaseItemToListboxCtrl purchase_display_info_ptr.period = %d\n")
TRACE_MSG(MMIMBBMS_WINTAB_1544_112_2_18_2_39_49_413,"MMIMBBMS_AddPurchaseItemToListboxCtrl str_period[0] = 0x%x, mbbms_string_unit.wstr_len = %d \n")
TRACE_MSG(MMIMBBMS_WINTAB_1810_112_2_18_2_39_50_414,"MMIMBBMS_HandleSearchMainMsgWin genre.string_len = %d")
TRACE_MSG(MMIMBBMS_WINTAB_1825_112_2_18_2_39_50_415,"MMIMBBMS_HandleSearchMainMsgWin start MBBMSSG_GetComplexInfo")
TRACE_MSG(MMIMBBMS_WINTAB_1832_112_2_18_2_39_50_416,"MMIMBBMS_HandleSearchMainMsgWin is not MMIMBBMS_FLOW_STATE_IDLE")
TRACE_MSG(MMIMBBMS_WINTAB_1984_112_2_18_2_39_50_417,"HandleSearchResultMainWinMsg()  msg_id=%d")
TRACE_MSG(MMIMBBMS_WINTAB_2045_112_2_18_2_39_50_418,"HandleSearchResultMainWinMsg() MSG_CTL_LIST_NEED_ITEM_DATA msg_id=%d")
TRACE_MSG(MMIMBBMS_WINTAB_2048_112_2_18_2_39_50_419,"MMIMBBMS HandleSearchResultMainWinMsg need_item_data_ptr is null")
TRACE_MSG(MMIMBBMS_WINTAB_2219_112_2_18_2_39_51_420,"pContent = 0x%x")
TRACE_MSG(MMIMBBMS_WINTAB_2435_112_2_18_2_39_51_421,"SCI_ALLOC_APP FOR effect_item FAIL!")
TRACE_MSG(MMIMBBMS_WINTAB_2568_112_2_18_2_39_51_422,"HandleMbbmsSettingWinMsg() msg_id=%d")
TRACE_MSG(MMIMBBMS_WINTAB_2787_112_2_18_2_39_52_423,"---MMIMBBMS_OpenPurchaseOrderQueryWin purchase_table_info_ptr = 0x%x\n")
TRACE_MSG(MMIMBBMS_WINTAB_2803_112_2_18_2_39_52_424,"MMIMBBMS_OpenPurchaseOrderQueryWin anyURI_len = %d\n")
TRACE_MSG(MMIMBBMS_WINTAB_2821_112_2_18_2_39_52_425,"MMIMBBMS_OpenPurchaseOrderQueryWin purchase_item_error_code =%dn")
TRACE_MSG(MMIMBBMS_WINTAB_2856_112_2_18_2_39_52_426,"MMIMBBMS_OpenPurchaseOrderQueryWin:Get Purchase Item failed\n")
TRACE_MSG(MMIMBBMS_WINTAB_3232_112_2_18_2_39_53_427,"MMIMBBMS_GetGlobalIndexFromTwoIndex:Get Purchase Item failed\n")
TRACE_MSG(MMIMBBMS_WINTAB_3242_112_2_18_2_39_53_428,"MMIMBBMS_GetGlobalIndexFromTwoIndex ---no purchaseitem\n")
TRACE_MSG(MMIMBBMS_WINTAB_3247_112_2_18_2_39_53_429,"MMIMBBMS_GetGlobalIndexFromTwoIndex total purchase num=%d\n")
TRACE_MSG(MMIMBBMS_WINTAB_3304_112_2_18_2_39_53_430,"MMIMBBMS_HandlePurchaseItemDetailWinMsg() enter:msg_id=%d")
TRACE_MSG(MMIMBBMS_WINTAB_3345_112_2_18_2_39_53_431,"MMIMBBMS_HandlePurchaseItemDetailWinMsg() msg_id=%d")
TRACE_MSG(MMIMBBMS_WINTAB_4200_112_2_18_2_39_55_432,"MBBMSSG_GetContentIndex errcode = %d")
TRACE_MSG(MMIMBBMS_WINTAB_4213_112_2_18_2_39_55_433,"MBBMSSG_GetContentIndex list_potr = PNULL")
TRACE_MSG(MMIMBBMS_WINTAB_4237_112_2_18_2_39_55_434,"pURI_index is pnull")
TRACE_MSG(MMIMBBMS_WINTAB_4311_112_2_18_2_39_55_435,"MMIMBBMS_LoadListItem purDetailItem.PriceInfo.period = %d")
TRACE_MSG(MMIMBBMS_WINTAB_4385_112_2_18_2_39_55_436,"MMIMBBMS_LoadListItem date year:%d, month:%d day:%d")
TRACE_MSG(MMIMBBMS_WINTAB_4388_112_2_18_2_39_55_437,"MMIMBBMS_LoadListItem net_id=%d, service_id=%d CMMB_GetAllProg return=%d, NumOfProg=%d")
TRACE_MSG(MMIMBBMS_WINTAB_4438_112_2_18_2_39_55_438,"MMIMBBMS_GetServiceIndex errcode = %d")
TRACE_MSG(MMIMBBMS_WINTAB_4447_112_2_18_2_39_55_439,"MMIMBBMS_GetServiceNodeByIdx: only have head node")
TRACE_MSG(MMIMBBMS_WINTAB_4494_112_2_18_2_39_55_440,"MBBMSSG_GetContentIndex errcode = %d")
TRACE_MSG(MMIMBBMS_WINTAB_4503_112_2_18_2_39_55_441,"MMIMBBMS_GetServiceNodeByIdx: only have head node")
TRACE_MSG(MMIMBBMS_WINTAB_4829_112_2_18_2_39_56_442,"MMIMBBMS_HandleProgramDetailWinMsg() enter:msg_id=%d")
TRACE_MSG(MMIMBBMS_WINTAB_4864_112_2_18_2_39_56_443,"MBBMSSG_GetContent errcode = %d")
TRACE_MSG(MMIMBBMS_WINTAB_4904_112_2_18_2_39_56_444,"MBBMSSG_GetPreviewdata errcode = %d")
TRACE_MSG(MMIMBBMS_WINTAB_4925_112_2_18_2_39_56_445,"MMIMBBMS_HandleProgramDetailWinMsg filename[%d] = 0x%x, len = %d")
TRACE_MSG(MMIMBBMS_WINTAB_5374_112_2_18_2_39_57_446,"HandleMainInterfaceTabWinMsg MSG_CLOSE_WINDOW\n ")
TRACE_MSG(MMIMBBMS_WINTAB_5938_112_2_18_2_39_58_447,"MMIMBBMS_OpenMainInterfaceWin enter MMIMBBMS_TickCount=%d")
TRACE_MSG(MMIMBBMS_WINTAB_5942_112_2_18_2_39_58_448,"MMIMBBMS_OpenMainInterfaceWin signal \n")
TRACE_MSG(MMIMBBMS_WINTAB_5966_112_2_18_2_39_58_449,"MMIMBBMS_OpenMainInterfaceWin end MMIMBBMS_TickCount=%d")
TRACE_MSG(MMIMBBMS_WINTAB_6178_112_2_18_2_39_59_450,"[MMIMBBMS]: CreateAllServiceList  list_num = %d")
TRACE_MSG(MMIMBBMS_WINTAB_6185_112_2_18_2_39_59_451,"[MMIMBBMS]: CreateAllServiceList pinfo_node alloc fail")
TRACE_MSG(MMIMBBMS_WINTAB_6227_112_2_18_2_39_59_452,"[MMIMBBMS]:MBBMSSG_GetGlobalServiceId is error = %d")
TRACE_MSG(MMIMBBMS_WINTAB_6286_112_2_18_2_39_59_453,"CreateFavoriteServiceList:Get Service Index error\n")
TRACE_MSG(MMIMBBMS_WINTAB_6291_112_2_18_2_39_59_454,"[MMIMTV]: CreateFavoriteServiceList  list_num = %d")
TRACE_MSG(MMIMBBMS_WINTAB_6312_112_2_18_2_39_59_455,"[MMIMTV]: CreateFavoriteServiceList MBBMSSG_GetService err = %d")
TRACE_MSG(MMIMBBMS_WINTAB_6370_112_2_18_2_40_0_456,"[MMIMTV]: CreateAlarmContentList  list_num = %d")
TRACE_MSG(MMIMBBMS_WINTAB_6391_112_2_18_2_40_0_457,"[MMIMTV]: CreateAlarmContentList MBBMSSG_GetContent ERR = %d")
TRACE_MSG(MMIMBBMS_WINTAB_6450_112_2_18_2_40_0_458,"[MMIMBBMS]: AddServiceItemToListboxCtrl node_ptr is null")
TRACE_MSG(MMIMBBMS_WINTAB_6548_112_2_18_2_40_0_459,"[MMIMBBMS]: AddContentItemToListboxCtrl node_ptr is null")
TRACE_MSG(MMIMBBMS_WINTAB_6645_112_2_18_2_40_0_460,"MBBMSSG_GetService errcode = %d")
TRACE_MSG(MMIMBBMS_WINTAB_6726_112_2_18_2_40_0_461,"MBBMSSG_GetContentIndex errcode = %d")
TRACE_MSG(MMIMBBMS_WINTAB_6739_112_2_18_2_40_0_462,"pURI_index is pnull")
TRACE_MSG(MMIMBBMS_WINTAB_6805_112_2_18_2_40_0_463,"[MMIMBBMS] GetInfoFromService bRight=%d, bMax=%d, bMin=%d")
TRACE_MSG(MMIMBBMS_WINTAB_7306_112_2_18_2_40_1_464,"MMIMBBMS_RefreshMainInterface: FALSE!")
TRACE_MSG(MMIMBBMS_WINTAB_7547_112_2_18_2_40_2_465,"MMIMBBMS_PlayServiceByIndex:Get s_main_service_count=%d, index=%d, s_main_service_index=%d")
TRACE_MSG(MMIMBBMS_WINTAB_7556_112_2_18_2_40_2_466,"MMIMBBMS_PlayServiceByIndex cmmb_play_status=%d")
TRACE_MSG(MMIMBBMS_WINTAB_7609_112_2_18_2_40_2_467,"MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service Index error\n")
TRACE_MSG(MMIMBBMS_WINTAB_7617_112_2_18_2_40_2_468,"MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service NODE error\n")
TRACE_MSG(MMIMBBMS_WINTAB_7625_112_2_18_2_40_2_469,"MMIMBBMS_AddCurPlayServiceIntoFavoriteList:have added into favorite list\n")
TRACE_MSG(MMIMBBMS_WINTAB_7655_112_2_18_2_40_2_470,"MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service Index error\n")
TRACE_MSG(MMIMBBMS_WINTAB_7663_112_2_18_2_40_2_471,"MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service NODE error\n")
TRACE_MSG(MMIMBBMS_WINTAB_7671_112_2_18_2_40_2_472,"MMIMBBMS_DelCurPlayServiceIntoFavoriteList:have not in favorite list\n")
TRACE_MSG(MMIMBBMS_WINTAB_7701_112_2_18_2_40_2_473,"MMIMBBMS_IsCurPlayServiceInFavoriteList:Get Service Index error\n")
TRACE_MSG(MMIMBBMS_WINTAB_7709_112_2_18_2_40_2_474,"MMIMBBMS_IsCurPlayServiceInFavoriteList:Get Service NODE error\n")
TRACE_MSG(MMIMBBMS_WINTAB_7753_112_2_18_2_40_2_475,"MBBMSSG_GetService errcode = %d")
TRACE_MSG(MMIMBBMS_WINTAB_7805_112_2_18_2_40_3_476,"MMIMBBMS_GetCurrServiceName error! index=%  total_count=%d")
TRACE_MSG(MMIMBBMS_WINTAB_7837_112_2_18_2_40_3_477,"MMIMBBMS_OpenStartUpWin TickCount=%d")
TRACE_MSG(MMIMBBMS_WINTAB_7882_112_2_18_2_40_3_478,"MMIMBBMS HandleWatchConnectingWinMsg msg MSG_APP_MBBMS_PLAY_CONTENT")
TRACE_MSG(MMIMBBMS_WINTAB_8212_112_2_18_2_40_4_479,"[MMIMTV]: MBBMS_AddAllServiceToDropdownList  alloc fail")
TRACE_MSG(MMIMBBMS_WINTAB_8228_112_2_18_2_40_4_480,"[MMIMTV]: CreateAllServiceList  list_num = %d")
TRACE_MSG(MMIMBBMS_WINTAB_8235_112_2_18_2_40_4_481,"[MMIMTV]: MBBMS_AddAllServiceToDropdownList  alloc fail")
TRACE_MSG(MMIMBBMS_WINTAB_8250_112_2_18_2_40_4_482,"MBBMSSG_GetService errcode = %d")
TRACE_MSG(MMIMBBMS_WINTAB_8265_112_2_18_2_40_4_483,"wstr_len = %d")
TRACE_MSG(MMIMBBMS_WINTAB_8274_112_2_18_2_40_4_484,"index = %d")
TRACE_MSG(MMIMBBMS_WINTAB_8520_112_2_18_2_40_5_485,"MMIMBBMS_ContentNotifyCallback: ")
TRACE_MSG(MMIMBBMS_WINTAB_8524_112_2_18_2_40_5_486,"MMIMBBMS_ContentNotifyCallback: RESTART_BY_ALARM")
TRACE_MSG(MMIMBBMS_WINTAB_8541_112_2_18_2_40_5_487,"MMIMBBMS_ContentNotifyCallback: time_remainder is error = %d")
TRACE_MSG(MMIMBBMS_WINTAB_8553_112_2_18_2_40_5_488,"MMIMBBMS_ContentNotifyCallback: remindertime = %d systime.tm_min = %d  realtime.tm_min = %d")
TRACE_MSG(MMIMBBMS_WINTAB_8563_112_2_18_2_40_5_489,"MMIMBBMS_ContentNotifyCallback: match content id = %s")
TRACE_MSG(MMIMBBMS_WINTAB_8710_112_2_18_2_40_5_490,"MMIMBBMS_AddPurchaseDetailTail: scription_type=%d")
TRACE_MSG(MMIMBBMS_WINTAB_8729_112_2_18_2_40_5_491,"MMIMBBMS_AddPurchaseDetailTail.....purchase_display_info->period = %d\n")
TRACE_MSG(MMIMBBMS_WINTAB_8786_112_2_18_2_40_6_492,"MMIMBBMS_AddPurchaseDetailTail: price scription_type=%d")
TRACE_MSG(MMIMBBMS_WINTAB_8832_112_2_18_2_40_6_493,"MMIMBBMS_IsCurrServiceInAllTabEncrypted: %d")
TRACE_MSG(MMIMBBMS_WINTAB_8837_112_2_18_2_40_6_494,"MMIMBBMS_IsCurrServiceInAllTabEncrypted error!  index=%  total_count=%d")
TRACE_MSG(MMIMBBMS_WINTAB_8872_112_2_18_2_40_6_495,"MMIMBBMS_IsCurrServiceInFavoriteTabEncrypted:%d")
TRACE_MSG(MMIMBBMS_WINTAB_8910_112_2_18_2_40_6_496,"MMIMBBMS_IsCurrContentInFavoriteTabEncrypted:%d")
TRACE_MSG(MMIMBBMS_WINTAB_8985_112_2_18_2_40_6_497,"[MMIMBBMS]: CreateEBList eb_list_number = %d")
TRACE_MSG(MMIMBBMS_WINTAB_9453_112_2_18_2_40_7_498,"HandleSimSelectWinMsg, item_data=%d")
TRACE_MSG(MMIMBBMS_WINTAB_9537_112_2_18_2_40_8_499,"sg_addr = %s sg_domain_info = %s ")
TRACE_MSG(MMIMBBMS_WINTAB_9561_112_2_18_2_40_8_500,"MMIMBBMS_HandleStartTimer timer_master_query = %d")
TRACE_MSG(MMIMBBMS_WINTAB_9565_112_2_18_2_40_8_501,"MMIMBBMS_HandleStartTimer app is closed!")
TRACE_MSG(MMIMBBMS_WINTAB_9648_112_2_18_2_40_8_502,"[MMIMBBMS]:MMIMBBMS_GetServiceIndexByServiceID MMIMBBMS_GetServiceIndex error!")
TRACE_MSG(MMIMBBMS_WINTAB_9654_112_2_18_2_40_8_503,"[MMIMBBMS]: MMIMBBMS_GetServiceIndexByServiceID  list_num = %d")
TRACE_MSG(MMIMBBMS_WINTAB_9658_112_2_18_2_40_8_504,"[MMIMBBMS]:MMIMBBMS_GetServiceIndexByServiceID listnum = %d ")
TRACE_MSG(MMIMBBMS_WINTAB_9703_112_2_18_2_40_8_505,"MMIMBBMS_EnterPlayFromAllServiceTab:Get Service Index error\n")
TRACE_MSG(MMIMBBMS_WINTAB_9734_112_2_18_2_40_8_506,"MMIMBBMS_EnterPlayFromAllServiceTab: error enter MMIMBBMS_EnterCMMBModePlay")
TRACE_MSG(MMIMBBMS_WINTAB_9829_112_2_18_2_40_9_507,"MMIMBBMS_EnterOpenOrPlayFromProgramList: error enter MMIMBBMS_EnterCMMBModePlay")
TRACE_MSG(MMIMBBMS_WINTAB_9910_112_2_18_2_40_9_508,"MMIMBBMS_EnterOpenOrPlayFromProgramDetail enter play\n")
TRACE_MSG(MMIMBBMS_WINTAB_9938_112_2_18_2_40_9_509,"MMIMBBMS_EnterOpenOrPlayFromSearchResult enter play\n")
TRACE_MSG(MMIMBBMS_WINTAB_9966_112_2_18_2_40_9_510,"MMIMBBMS_EnterOpenOrPlayFromProgramDetail enter play\n")
TRACE_MSG(MMIMBBMS_WINTAB_9986_112_2_18_2_40_9_511,"MMIMBBMS_OpenSubscriptionUpdateQueryWin\n")
TRACE_MSG(MMIMBBMS_WINTAB_10003_112_2_18_2_40_9_512,"MMIMBBMS_OpenSubscriptionUpdateQueryWinByTextPtr\n")
TRACE_MSG(MMIMBBMS_WINTAB_10092_112_2_18_2_40_9_513,"MMIMBBMS_OpenSwitchToMBBMSQueryWin  cur_flow_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_WINTAB_10217_112_2_18_2_40_9_514,"StartSwitchToMBBMSProcess enter")
TRACE_MSG(MMIMBBMS_WINTAB_10224_112_2_18_2_40_9_515,"StartSwitchToMBBMSProcess cur_status=%d, cur_flow_type=%d")
TRACE_MSG(MMIMBBMS_WINTAB_10263_112_2_18_2_40_10_516,"HandleSwitchToMBBMSTimer")
TRACE_MSG(MMIMBBMS_WINTAB_10267_112_2_18_2_40_10_517,"HandleSwitchToMBBMSTimer app is closed!")
TRACE_MSG(MMIMBBMS_WINTAB_10419_112_2_18_2_40_10_518,"MMIMBBMS_EnterPlayOrOpen s_object_type=%d")
TRACE_MSG(MMIMBBMS_WINTAB_10420_112_2_18_2_40_10_519,"MMIMBBMS_TickCount MMIMBBMS_EnterPlayOrOpen enter = %d")
TRACE_MSG(MMIMBBMS_WINTAB_10493_112_2_18_2_40_10_520,"MMIMBBMS_OpenAccountInquiryRequestWin")
TRACE_MSG(MMIMBBMS_WINTAB_10496_112_2_18_2_40_10_521,"MMIMBBMS_OpenAccountInquiryRequestWin MMIMBBMS_SDandOpenCondition return FALSE")
TRACE_MSG(MMIMBBMS_WINTAB_10576_112_2_18_2_40_10_522,"MMIMBBMS_OpenSubscribeUnsubscribeCancelWaitWin %d")
TRACE_MSG(MMIMBBMS_WINTAB_10688_112_2_18_2_40_11_523,"MMIMBBMS_DisplayInitWaitingWin:init status=%d\n")
TRACE_MSG(MMIMBBMS_WINTAB_10786_112_2_18_2_40_11_524,"MMIMBBMS_AddServiceItemToTabCtrl service_count = %d large than GUITAB_MAX_ITEM_NUM")
TRACE_MSG(MMIMBBMS_WINTAB_10873_112_2_18_2_40_11_525,"MMIMBBMS_HandleUpdateMainWinMsg  MTV_ID_MBBMS_AUTO_UPDATE_MENU")
TRACE_MSG(MMIMBBMS_WINTAB_10878_112_2_18_2_40_11_526,"MMIMBBMS_HandleUpdateMainWinMsg  MTV_ID_MBBMS_CITY_UPDATE_MENU")
TRACE_MSG(MMIMBBMS_WINTAB_10887_112_2_18_2_40_11_527,"MMIMBBMS_HandleUpdateMainWinMsg  ID ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIMBBMS_WINTAB_10952_112_2_18_2_40_11_528,"MMIMBBMS_HandleUpdateFromCityNameWinMsg()  start city update")
TRACE_MSG(MMIMBBMS_WINTAB_10994_112_2_18_2_40_11_529,"MMIMBBMS_GetProgramList s_program_list_ptr alloc fail")
TRACE_MSG(MMIMBBMS_WINTAB_11122_112_2_18_2_40_12_530,"[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex content year=%d, month=%d, day=%d, hour=%d, minutes=%d ")
TRACE_MSG(MMIMBBMS_WINTAB_11132_112_2_18_2_40_12_531,"[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex SYSTEM hour=%d, minutes=%d ")
TRACE_MSG(MMIMBBMS_WINTAB_11138_112_2_18_2_40_12_532,"[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex SYSTEM year=%d, month=%d, day=%d ")
TRACE_MSG(MMIMBBMS_WINTAB_11152_112_2_18_2_40_12_533,"[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex bRight=%d ")
TRACE_MSG(MMIMBBMS_WINTAB_11163_112_2_18_2_40_12_534,"[MMIMBBMS] MMIMBBMS_SetIfEnterSubFromPlay ret=%d ")
TRACE_MSG(MMIMBBMS_WINTAB_11173_112_2_18_2_40_12_535,"[MMIMBBMS] MMIMBBMS_GetIfEnterSubFromPlay ret=%d ")
TRACE_MSG(MMIMBBMS_WINTAB_11202_112_2_18_2_40_12_536,"MBBMS_AddAllContentTypeToDropdownListNew genre_ptr = 0x%x, total_num = %d")
TRACE_MSG(MMIMBBMS_WINTAB_11207_112_2_18_2_40_12_537,"MBBMS_AddAllContentTypeToDropdownListNew item_data_ptr = PNULL. \n ")
TRACE_MSG(MMIMBBMS_WINTAB_11230_112_2_18_2_40_12_538,"MBBMS_AddAllContentTypeToDropdownListNew content_type_string_ptr = 0x%x, string_len = %d")
TRACE_MSG(MMIMTV_DATA_149_112_2_18_2_40_12_539,"MMIMTV_NewDataData s_data_data_ptr is null")
TRACE_MSG(MMIMTV_DATA_269_112_2_18_2_40_13_540,"[MMIMTV]: MMIMTV_DeleteServiceNode find service node error!")
TRACE_MSG(MMIMTV_DATA_373_112_2_18_2_40_13_541,"[MMIMTV]: MMIMTV_DeleteNetNode, find net node error!")
TRACE_MSG(MMIMTV_DATA_508_112_2_18_2_40_13_542,"[MMIMTV]: MMIMTV_GetNetNodeViaIndex, index = %d")
TRACE_MSG(MMIMTV_DATA_669_112_2_18_2_40_14_543,"[MMIMTV]: MMIMTV_SetPlayingNetServiceNodeViaID, net_id=%d, service_id=%d")
TRACE_MSG(MMIMTV_DATA_889_112_2_18_2_40_14_544,"[MMIMTV]: MMIMTV_AddEBNode alloc memory error!")
TRACE_MSG(MMIMTV_DATA_931_112_2_18_2_40_14_545,"[MMIMTV]: MMIMTV_DeleteEBNode find eb node error!")
TRACE_MSG(MMIMTV_DATA_1018_112_2_18_2_40_14_546,"[MMIMTV]: MMIMTV_GetUnreadEBMsgNum, num = %d")
TRACE_MSG(MMIMTV_DATA_1170_112_2_18_2_40_15_547,"[MMIMTV]: MMIMTV_ReadEBFile service_head_ptr not null! ")
TRACE_MSG(MMIMTV_DATA_1191_112_2_18_2_40_15_548,"[MMIMTV]: MMIMTV_ReadEBFile create file error! ")
TRACE_MSG(MMIMTV_DATA_1197_112_2_18_2_40_15_549,"[MMIMTV]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_1212_112_2_18_2_40_15_550,"[MMIMTV]: EB file version error! ")
TRACE_MSG(MMIMTV_DATA_1223_112_2_18_2_40_15_551,"[MMIMTV]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_1227_112_2_18_2_40_15_552,"[MMIMTV]: MMIMTV_ReadEBFile total list num = %d")
TRACE_MSG(MMIMTV_DATA_1231_112_2_18_2_40_15_553,"[MMIMTV]: MMIAPIFMM_SetFilePointer error! ")
TRACE_MSG(MMIMTV_DATA_1251_112_2_18_2_40_15_554,"[MMIMTV]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_1258_112_2_18_2_40_15_555,"[MMIMTV]: MMIAPIFMM_SetFilePointer error! ")
TRACE_MSG(MMIMTV_DATA_1297_112_2_18_2_40_15_556,"[MMIMTV]: MMIMTV_WriteEBFile create file error! ")
TRACE_MSG(MMIMTV_DATA_1365_112_2_18_2_40_15_557,"[MMIMTV]: MMIMTV_ReadNSFile open file error! ")
TRACE_MSG(MMIMTV_DATA_1371_112_2_18_2_40_15_558,"[MMIMTV]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_1377_112_2_18_2_40_15_559,"[MMIMTV]: NS file version error! ")
TRACE_MSG(MMIMTV_DATA_1401_112_2_18_2_40_15_560,"[MMIMTV]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_1409_112_2_18_2_40_15_561,"[MMIMTV]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_1454_112_2_18_2_40_15_562,"[MMIMTV]: MMIMTV_WriteNSFile create file error! ")
TRACE_MSG(MMIMTV_DATA_1680_112_2_18_2_40_16_563,"[MMIMTV]: MMIMTV_SetESGPath path: %s")
TRACE_MSG(MMIMTV_DATA_2295_112_2_18_2_40_17_564,"MMIMTV_Mtvstring2Unicode length==0")
TRACE_MSG(MMIMTV_DATA_2385_112_2_18_2_40_17_565,"MMIMTV_BMPToYUV422 ENTER")
TRACE_MSG(MMIMTV_DATA_2389_112_2_18_2_40_17_566,"MMIMTV_BMPToYUV422 height=%d, width=%d")
TRACE_MSG(MMIMTV_DATA_2395_112_2_18_2_40_17_567,"MMIMTV_BMPToYUV422: error, please check image type!")
TRACE_MSG(MMIMTV_DATA_2499_112_2_18_2_40_18_568,"[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteServiceFile create file error! ")
TRACE_MSG(MMIMTV_DATA_2506_112_2_18_2_40_18_569,"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_2522_112_2_18_2_40_18_570,"[MMIMTV MMIMBBMS]: Favorite Service file version error! ")
TRACE_MSG(MMIMTV_DATA_2533_112_2_18_2_40_18_571,"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_2537_112_2_18_2_40_18_572,"[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteServiceFile total list num = %d")
TRACE_MSG(MMIMTV_DATA_2541_112_2_18_2_40_18_573,"[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! ")
TRACE_MSG(MMIMTV_DATA_2561_112_2_18_2_40_18_574,"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_2568_112_2_18_2_40_18_575,"[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! ")
TRACE_MSG(MMIMTV_DATA_2614_112_2_18_2_40_18_576,"[MMIMTV MMIMBBMS]: MMIMBBMS_WriteFavoriteServiceFile create file error! ")
TRACE_MSG(MMIMTV_DATA_2671_112_2_18_2_40_18_577,"[MMIMTV]: MMIMBBMS_AddFavoriteServiceNode alloc memory error! ")
TRACE_MSG(MMIMTV_DATA_2826_112_2_18_2_40_18_578,"[MMIMTV]: MMIMBBMS_GetFavoriteServiceListNum  num = %d")
TRACE_MSG(MMIMTV_DATA_2940_112_2_18_2_40_18_579,"[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteContentFile create file error! ")
TRACE_MSG(MMIMTV_DATA_2948_112_2_18_2_40_18_580,"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_2965_112_2_18_2_40_19_581,"[MMIMTV MMIMBBMS]: Favorite Content file version error! ")
TRACE_MSG(MMIMTV_DATA_2976_112_2_18_2_40_19_582,"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_2980_112_2_18_2_40_19_583,"[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteContentFile total list num = %d")
TRACE_MSG(MMIMTV_DATA_2984_112_2_18_2_40_19_584,"[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! ")
TRACE_MSG(MMIMTV_DATA_3005_112_2_18_2_40_19_585,"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! ")
TRACE_MSG(MMIMTV_DATA_3012_112_2_18_2_40_19_586,"[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! ")
TRACE_MSG(MMIMTV_DATA_3058_112_2_18_2_40_19_587,"[MMIMTV MMIMBBMS]: MMIMBBMS_WriteFavoriteContentFile create file error! ")
TRACE_MSG(MMIMTV_DATA_3115_112_2_18_2_40_19_588,"[MMIMTV MMIMBBMS]: MMIMBBMS_AddAlarmContentNode alloc memory error! ")
TRACE_MSG(MMIMTV_DATA_3152_112_2_18_2_40_19_589,"[MMIMTV MMIMBBMS]: MMIMBBMS_DelAlarmContentNode find node error! ")
TRACE_MSG(MMIMTV_DATA_3303_112_2_18_2_40_19_590,"[MMIMTV]: MMIMBBMS_GetAlarmContentListNum  num = %d")
TRACE_MSG(MMIMTV_DATA_3355_112_2_18_2_40_19_591,"[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile net_id = %d, service_id = %d")
TRACE_MSG(MMIMTV_DATA_3394_112_2_18_2_40_19_592,"[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile write file in E disk error! ")
TRACE_MSG(MMIMTV_DATA_3397_112_2_18_2_40_19_593,"[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile create file in E disk ok! ")
TRACE_MSG(MMIMTV_DATA_3409_112_2_18_2_40_19_594,"[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile write file in D disk error! ")
TRACE_MSG(MMIMTV_DATA_3412_112_2_18_2_40_19_595,"[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile create file in D disk error! ")
TRACE_MSG(MMIMTV_EXPORT_95_112_2_18_2_40_20_596,"[MMIMTV]: MMIAPIMTV_OpenMTVPlayer")
TRACE_MSG(MMIMTV_EXPORT_107_112_2_18_2_40_20_597,"[MMIMTV]: MMIAPIMTV_OpenMTVPlayer call running =%d")
TRACE_MSG(MMIMTV_EXPORT_117_112_2_18_2_40_20_598,"[MMIMTV]: MMIAPIMTV_OpenMTVPlayer app is opened!")
TRACE_MSG(MMIMTV_EXPORT_207_112_2_18_2_40_20_599,"[MMIMTV]:MMIAPIMTV_Exit")
TRACE_MSG(MMIMTV_EXPORT_226_112_2_18_2_40_20_600,"[MMIMTV]:MMIAPIMTV_VTExit ")
TRACE_MSG(MMIMTV_MAIN_860_112_2_18_2_40_23_601,"[MMIMTV]: MMIMTV_EnterMTV")
TRACE_MSG(MMIMTV_MAIN_877_112_2_18_2_40_23_602,"[MMIMTV]: MMIMTV_ExitMTV  current play status = %d")
TRACE_MSG(MMIMTV_MAIN_885_112_2_18_2_40_23_603,"[MMIMBBMS]: MMIMBBMS_CloseCMMB MTV WIN is opened!")
TRACE_MSG(MMIMTV_MAIN_986_112_2_18_2_40_23_604,"MMIMTV_HandleTPMsg point.x = %d, point.y = %d")
TRACE_MSG(MMIMTV_MAIN_1129_112_2_18_2_40_23_605,"[MMIMTV]: MMIMTV_HandleFocusMsg, msg_id = 0x%x, s_focus_osd = 0x%x")
TRACE_MSG(MMIMTV_MAIN_1207_112_2_18_2_40_23_606,"[MMIMTV]: MMIMTV_HandlePSMsg: msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_1213_112_2_18_2_40_23_607,"[MMIMTV]: MMIMTV_HandlePSMsg app is not open!")
TRACE_MSG(MMIMTV_MAIN_1221_112_2_18_2_40_23_608,"[MMIMTV]: received MTV_RSSI_CHANGED msg")
TRACE_MSG(MMIMTV_MAIN_1234_112_2_18_2_40_23_609,"[MMIMTV]: received MTV_STIMI_SEARCH_PROG_END_CNF msg")
TRACE_MSG(MMIMTV_MAIN_1258_112_2_18_2_40_23_610,"[MMIMTV]: MTVSTIMI_GetNetworkInfoTab network_number = %d.")
TRACE_MSG(MMIMTV_MAIN_1308_112_2_18_2_40_24_611,"[MMIMTV]: received MTV_STIMI_SEARCH_PROG_ERR_IND msg")
TRACE_MSG(MMIMTV_MAIN_1322_112_2_18_2_40_24_612,"[MMIMTV]: received MTV_STIMI_SEARCH_ESG_END_CNF msg")
TRACE_MSG(MMIMTV_MAIN_1334_112_2_18_2_40_24_613,"[MMIMTV]: received MTV_STIMI_SEARCH_ESG_ERR_IND msg")
TRACE_MSG(MMIMTV_MAIN_1347_112_2_18_2_40_24_614,"[MMIMTV]: received MTV_ESG_PARSE_RESULT_IND  CMMB_ParseESG retrun: %d")
TRACE_MSG(MMIMTV_MAIN_1353_112_2_18_2_40_24_615,"[MMIMTV]: received MSG_MTV_ESG_SUC_CNF msg")
TRACE_MSG(MMIMTV_MAIN_1360_112_2_18_2_40_24_616,"[MMIMTV]: received MSG_MTV_ESG_ERR_IND msg")
TRACE_MSG(MMIMTV_MAIN_1371_112_2_18_2_40_24_617,"[MMIMTV]: received MTV_STIMI_EMERGENCY_BROADCAST_IND msg")
TRACE_MSG(MMIMTV_MAIN_1373_112_2_18_2_40_24_618,"[MMIMTV]: received MTV_STIMI_EMERGENCY_BROADCAST_IND msg sig_data->data_type = %d")
TRACE_MSG(MMIMTV_MAIN_1457_112_2_18_2_40_24_619,"[MMIMTV]: received MTV_STIMI_PROGRAM_HINT_IND msg")
TRACE_MSG(MMIMTV_MAIN_1471_112_2_18_2_40_24_620,"[MMIMTV]: received MTV_STIMI_CTLTAB_UPDATE_IND msg")
TRACE_MSG(MMIMTV_MAIN_1475_112_2_18_2_40_24_621,"[MMIMTV]: received MTV_STIMI_RCV_CTLTAB_ERR_IND msg")
TRACE_MSG(MMIMTV_MAIN_1479_112_2_18_2_40_24_622,"[MMIMTV]: received MTV_PLAY_ACK msg")
TRACE_MSG(MMIMTV_MAIN_1489_112_2_18_2_40_24_623,"[MMIMTV]: received MTV_PLAY_ACK msg  MMIMTV_PLAY_TYPE_NETWORK")
TRACE_MSG(MMIMTV_MAIN_1496_112_2_18_2_40_24_624,"[MMIMTV]: received MTV_PLAY_ACK msg  MMIMTV_PLAY_TYPE_RECORD")
TRACE_MSG(MMIMTV_MAIN_1513_112_2_18_2_40_24_625,"[MMIMTV]: received MTV_PLAY_ERROR msg")
TRACE_MSG(MMIMTV_MAIN_1534_112_2_18_2_40_24_626,"[MMIMTV]: received MTV_PLAY_ERROR_IND msg")
TRACE_MSG(MMIMTV_MAIN_1554_112_2_18_2_40_24_627,"[MMIMTV]: received MTV_BAD_SIGNAL msg")
TRACE_MSG(MMIMTV_MAIN_1574_112_2_18_2_40_24_628,"[MMIMTV]: received MTV_SIGNAL_RECOVERED msg")
TRACE_MSG(MMIMTV_MAIN_1599_112_2_18_2_40_24_629,"[MMIMTV]: received MTV_RELOCKING msg")
TRACE_MSG(MMIMTV_MAIN_1618_112_2_18_2_40_24_630," [MMIMTV]: received MTV_PLAY_AV_IND msg video ind=%d, audio ind=%d")
TRACE_MSG(MMIMTV_MAIN_1623_112_2_18_2_40_24_631,"[MMIMTV]: received MTV_PLAY_DRA_IND msg")
TRACE_MSG(MMIMTV_MAIN_1638_112_2_18_2_40_24_632,"[MMIMTV]: received MTV_CA_NOT_GRANTED_IND msg")
TRACE_MSG(MMIMTV_MAIN_1660_112_2_18_2_40_24_633,"[MMIMTV]: received MTV_CA_CARD_NOT_PRESENT_IND msg")
TRACE_MSG(MMIMTV_MAIN_1680_112_2_18_2_40_24_634,"[MMIMTV]: received MTV_STOP_ACK msg")
TRACE_MSG(MMIMTV_MAIN_1708_112_2_18_2_40_24_635,"[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_PLAY_NEXT")
TRACE_MSG(MMIMTV_MAIN_1718_112_2_18_2_40_24_636,"[MMIMTV]: MTV_STOP_ACK PlayNetwork error")
TRACE_MSG(MMIMTV_MAIN_1729_112_2_18_2_40_24_637,"[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_PLAY_RECORD")
TRACE_MSG(MMIMTV_MAIN_1735_112_2_18_2_40_24_638,"[MMIMTV]: PlayStream error")
TRACE_MSG(MMIMTV_MAIN_1752_112_2_18_2_40_24_639,"[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_SLIDE")
TRACE_MSG(MMIMTV_MAIN_1759_112_2_18_2_40_24_640,"[MMIMTV]: SetStreamPos set_pos_result = %d")
TRACE_MSG(MMIMTV_MAIN_1764_112_2_18_2_40_24_641,"[MMIMTV]: PlayStream error")
TRACE_MSG(MMIMTV_MAIN_1782_112_2_18_2_40_25_642,"[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_SEARCH search type = %d")
TRACE_MSG(MMIMTV_MAIN_1789_112_2_18_2_40_25_643,"[MMIMTV]: received MTV_STOP_ACK CMMB_SearchProgram")
TRACE_MSG(MMIMTV_MAIN_1794_112_2_18_2_40_25_644,"[MMIMTV]: CMMB_SearchProgram error")
TRACE_MSG(MMIMTV_MAIN_1803_112_2_18_2_40_25_645,"[MMIMTV]: received MTV_STOP_ACK CMMB_SearchCenterFrq")
TRACE_MSG(MMIMTV_MAIN_1818_112_2_18_2_40_25_646,"[MMIMTV]: CMMB_SearchCenterFrq error")
TRACE_MSG(MMIMTV_MAIN_1828_112_2_18_2_40_25_647,"[MMIMTV]: received MTV_STOP_ACK CMMB_SearchESGData")
TRACE_MSG(MMIMTV_MAIN_1840_112_2_18_2_40_25_648,"[MMIMTV]: StartESGSearch stimi_ret = %d")
TRACE_MSG(MMIMTV_MAIN_1859_112_2_18_2_40_25_649,"[MMIMTV]: received MTV_STOP_ACK  MTV_PLAY_STREAM_END !")
TRACE_MSG(MMIMTV_MAIN_1870_112_2_18_2_40_25_650,"[MMIMTV]: MMIMTV_HandlePSMsg MTV_STOP_ERROR !")
TRACE_MSG(MMIMTV_MAIN_1883_112_2_18_2_40_25_651,"[MMIMTV]: received MTV_CA_IND msg")
TRACE_MSG(MMIMTV_MAIN_1887_112_2_18_2_40_25_652,"[MMIMTV]: received MTV_PLAY_STREAM_END msg")
TRACE_MSG(MMIMTV_MAIN_1892_112_2_18_2_40_25_653,"[MMIMTV]: Stop error")
TRACE_MSG(MMIMTV_MAIN_1904_112_2_18_2_40_25_654,"[MMIMTV]: received MTV_PLAY_TIME_IND msg")
TRACE_MSG(MMIMTV_MAIN_1920_112_2_18_2_40_25_655,"[MMIMTV]: s_mtv_current_para.play_type %d is error!")
TRACE_MSG(MMIMTV_MAIN_1925_112_2_18_2_40_25_656,"[MMIMTV]: received MTV_RECORD_FILE_SAVED msg")
TRACE_MSG(MMIMTV_MAIN_1939_112_2_18_2_40_25_657,"[MMIMTV]: received MTV_RECORD_FILE_TOOSMALL msg")
TRACE_MSG(MMIMTV_MAIN_1952_112_2_18_2_40_25_658,"[MMIMTV]: received MTV_DISK_WRITE_ERROR msg")
TRACE_MSG(MMIMTV_MAIN_1973_112_2_18_2_40_25_659,"[MMIMTV]: received MTV_DTL_ERROR msg")
TRACE_MSG(MMIMTV_MAIN_1982_112_2_18_2_40_25_660,"[MMIMTV]: received MBBMS_SERVICE_PDP_ACTIVE msg")
TRACE_MSG(MMIMTV_MAIN_1997_112_2_18_2_40_25_661,"[MMIMTV]: received MBBMS_SERVICE_PDP_DEACTIVE msg")
TRACE_MSG(MMIMTV_MAIN_2002_112_2_18_2_40_25_662,"[MMIMTV]: received MBBMS_SERVICE_PARSE_SG msg")
TRACE_MSG(MMIMTV_MAIN_2007_112_2_18_2_40_25_663,"[MMIMTV]: received MBBMS_SERVICE_STOP_PARSE_SG msg")
TRACE_MSG(MMIMTV_MAIN_2012_112_2_18_2_40_25_664,"[MMIMTV]: received MBBMS_SERVICE_PARSE_ESG msg")
TRACE_MSG(MMIMTV_MAIN_2017_112_2_18_2_40_25_665,"[MMIMTV]: received MBBMS_SERVICE_STOP_PARSE_ESG msg")
TRACE_MSG(MMIMTV_MAIN_2022_112_2_18_2_40_25_666,"[MMIMTV]: received MBBMS_SERVICE_ACTIVE msg")
TRACE_MSG(MMIMTV_MAIN_2027_112_2_18_2_40_25_667,"[MMIMTV]: received MBBMS_SERVICE_STOP_ACTIVE msg")
TRACE_MSG(MMIMTV_MAIN_2032_112_2_18_2_40_25_668,"[MMIMTV]: received MBBMS_SERVICE_UPDATE_TABLE msg")
TRACE_MSG(MMIMTV_MAIN_2037_112_2_18_2_40_25_669,"[MMIMTV]: received MBBMS_SERVICE_STOP_UPDATE_TABLE msg")
TRACE_MSG(MMIMTV_MAIN_2042_112_2_18_2_40_25_670,"[MMIMTV]: received MBBMS_SERVICE_GBA msg")
TRACE_MSG(MMIMTV_MAIN_2047_112_2_18_2_40_25_671,"[MMIMTV]: received MBBMS_SERVICE_STOP_GBA msg")
TRACE_MSG(MMIMTV_MAIN_2052_112_2_18_2_40_25_672,"[MMIMTV]: received MBBMS_SERVICE_PLAY msg")
TRACE_MSG(MMIMTV_MAIN_2057_112_2_18_2_40_25_673,"[MMIMTV]: received MBBMS_SERVICE_STOP msg")
TRACE_MSG(MMIMTV_MAIN_2063_112_2_18_2_40_25_674,"[MMIMBBMS]:receive sg get complexe data!\n")
TRACE_MSG(MMIMTV_MAIN_2067_112_2_18_2_40_25_675,"[MMIMBBMS]:receive sg stop getting complexe data!\n")
TRACE_MSG(MMIMTV_MAIN_2072_112_2_18_2_40_25_676,"[MMIMTV]: received MBBMS_SERVICE_SUBSCRIBE msg")
TRACE_MSG(MMIMTV_MAIN_2077_112_2_18_2_40_25_677,"[MMIMTV]: received MBBMS_SERVICE_STOP_SUBSCRIBE msg")
TRACE_MSG(MMIMTV_MAIN_2082_112_2_18_2_40_25_678,"[MMIMTV]: received MBBMS_SERVICE_UNSUBSCRIBE msg")
TRACE_MSG(MMIMTV_MAIN_2087_112_2_18_2_40_25_679,"[MMIMTV]: received MBBMS_SERVICE_STOP_UNSUBSCRIBE msg")
TRACE_MSG(MMIMTV_MAIN_2092_112_2_18_2_40_25_680,"[MMIMTV]: received MBBMS_SERVICE_ACCOUNT_QUIRY msg")
TRACE_MSG(MMIMTV_MAIN_2097_112_2_18_2_40_25_681,"[MMIMTV]: received MBBMS_SERVICE_STOP_ACCOUNT_QUIRY msg")
TRACE_MSG(MMIMTV_MAIN_2102_112_2_18_2_40_25_682,"[MMIMTV]: received MBBMS_SERVICE_MANUAL_SG msg")
TRACE_MSG(MMIMTV_MAIN_2107_112_2_18_2_40_25_683,"[MMIMTV]: received MBBMS_SERVICE_STOP_MANUAL_SG msg")
TRACE_MSG(MMIMTV_MAIN_2112_112_2_18_2_40_25_684,"[MMIMTV]: received MBBMS_SERVICE_CA_NOT_GRANTED_IND msg")
TRACE_MSG(MMIMTV_MAIN_2169_112_2_18_2_40_25_685,"[MMIMTV]: TurnUpVolume")
TRACE_MSG(MMIMTV_MAIN_2218_112_2_18_2_40_25_686,"[MMIMTV]: TurnDownVolume")
TRACE_MSG(MMIMTV_MAIN_2324_112_2_18_2_40_26_687,"[MMIMTV]: ReadNetCtrlTable error!")
TRACE_MSG(MMIMTV_MAIN_2345_112_2_18_2_40_26_688,"[MMIMTV]: SetNetCtrlTableData")
TRACE_MSG(MMIMTV_MAIN_2355_112_2_18_2_40_26_689,"[MMIMTV]: SetNetCtrlTableData net_id=%d data_len=%d")
TRACE_MSG(MMIMTV_MAIN_2393_112_2_18_2_40_26_690,"[MMIMTV]: ParseNetworkInfoListData network_number = %d")
TRACE_MSG(MMIMTV_MAIN_2403_112_2_18_2_40_26_691,"[MMIMTV]: ParseNetworkInfoListData network_id = %d")
TRACE_MSG(MMIMTV_MAIN_2404_112_2_18_2_40_26_692,"[MMIMTV]: ParseNetworkInfoListData frequency_no = %d")
TRACE_MSG(MMIMTV_MAIN_2406_112_2_18_2_40_26_693,"[MMIMTV]: ParseNetworkInfoListData center_frequency = %d")
TRACE_MSG(MMIMTV_MAIN_2407_112_2_18_2_40_26_694,"[MMIMTV]: ParseNetworkInfoListData service_number = %d")
TRACE_MSG(MMIMTV_MAIN_2409_112_2_18_2_40_26_695,"[MMIMTV]: ParseNetworkInfoListData code_table = %d")
TRACE_MSG(MMIMTV_MAIN_2431_112_2_18_2_40_26_696,"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_GB_2312 ucs_net_name_len = %d.")
TRACE_MSG(MMIMTV_MAIN_2445_112_2_18_2_40_26_697,"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_UTF8 ucs_net_name_len = %d.")
TRACE_MSG(MMIMTV_MAIN_2476_112_2_18_2_40_26_698,"[MMIMTV]: ParseNetworkInfoListData network_id = %d")
TRACE_MSG(MMIMTV_MAIN_2477_112_2_18_2_40_26_699,"[MMIMTV]: ParseNetworkInfoListData service_id = %d, service_type=%d")
TRACE_MSG(MMIMTV_MAIN_2500_112_2_18_2_40_26_700,"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_GB_2312 ucs_service_name_len = %d.")
TRACE_MSG(MMIMTV_MAIN_2514_112_2_18_2_40_26_701,"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_UTF8 ucs_service_name_len = %d.")
TRACE_MSG(MMIMTV_MAIN_2559_112_2_18_2_40_26_702,"[MMIMTV]: ParseNetworkInfoListData network_id = %d")
TRACE_MSG(MMIMTV_MAIN_2560_112_2_18_2_40_26_703,"[MMIMTV]: ParseNetworkInfoListData shortservice_id = %d, type=%d")
TRACE_MSG(MMIMTV_MAIN_2583_112_2_18_2_40_26_704,"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_GB_2312 ucs_shortservice_name_len = %d.")
TRACE_MSG(MMIMTV_MAIN_2597_112_2_18_2_40_26_705,"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_UTF8 ucs_shortservice_name_len = %d.")
TRACE_MSG(MMIMTV_MAIN_2651_112_2_18_2_40_26_706,"[MMIMTV]: ParseProgramHintData")
TRACE_MSG(MMIMTV_MAIN_2656_112_2_18_2_40_26_707,"[MMIMTV]: ParseProgramHintData prog_hintinfo->num = %d")
TRACE_MSG(MMIMTV_MAIN_2657_112_2_18_2_40_26_708,"[MMIMTV]: ParseProgramHintData hint_len = %d")
TRACE_MSG(MMIMTV_MAIN_2659_112_2_18_2_40_26_709,"[MMIMTV]: ParseProgramHintData MMI_MIN hint_len = %d")
TRACE_MSG(MMIMTV_MAIN_2668_112_2_18_2_40_26_710,"[MMIMTV]: ParseProgramHintData ucs_len = %d")
TRACE_MSG(MMIMTV_MAIN_2681_112_2_18_2_40_26_711,"[MMIMTV]: ParseProgramHintData ucs_len = %d")
TRACE_MSG(MMIMTV_MAIN_2690_112_2_18_2_40_26_712,"[MMIMTV]: ParseProgramHintData code_char_set = %d")
TRACE_MSG(MMIMTV_MAIN_2698_112_2_18_2_40_26_713,"[MMIMTV]: ParseProgramHintData hint_len = %d")
TRACE_MSG(MMIMTV_MAIN_2700_112_2_18_2_40_26_714,"[MMIMTV]: ParseProgramHintData MMI_MIN hint_len = %d")
TRACE_MSG(MMIMTV_MAIN_2708_112_2_18_2_40_26_715,"[MMIMTV]: ParseProgramHintData ucs_len = %d")
TRACE_MSG(MMIMTV_MAIN_2721_112_2_18_2_40_26_716,"[MMIMTV]: ParseProgramHintData ucs_len = %d")
TRACE_MSG(MMIMTV_MAIN_2730_112_2_18_2_40_26_717,"[MMIMTV]: ParseProgramHintData code_char_set = %d")
TRACE_MSG(MMIMTV_MAIN_2754_112_2_18_2_40_26_718,"[MMIMTV]: ParseEmergencyBroadcastData sig_data->data_type = %d")
TRACE_MSG(MMIMTV_MAIN_2761_112_2_18_2_40_27_719,"[MMIMTV]: ParseEmergencyBroadcastData origin text_len = %d")
TRACE_MSG(MMIMTV_MAIN_2764_112_2_18_2_40_27_720,"[MMIMTV]: ParseEmergencyBroadcastData msg_len = %d,msg_level = %d,code_char_set=%d")
TRACE_MSG(MMIMTV_MAIN_2769_112_2_18_2_40_27_721,"[MMIMTV]: ParseEmergencyBroadcastData node.eb_content[MMIMTV_EB_CONTENT_MAX] = %d,result_len = %d")
TRACE_MSG(MMIMTV_MAIN_2815_112_2_18_2_40_27_722,"[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData sig_data->data_type = %d")
TRACE_MSG(MMIMTV_MAIN_2822_112_2_18_2_40_27_723,"[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData origin text_len = %d")
TRACE_MSG(MMIMTV_MAIN_2825_112_2_18_2_40_27_724,"[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData msg_len = %d,msg_level = %d,code_char_set=%d")
TRACE_MSG(MMIMTV_MAIN_2830_112_2_18_2_40_27_725,"[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData node.eb_content[MMIMTV_EB_CONTENT_MAX] = %d,result_len = %d")
TRACE_MSG(MMIMTV_MAIN_2868_112_2_18_2_40_27_726,"[MMIMTV]: PlayDefaultServiceAfterSearch")
TRACE_MSG(MMIMTV_MAIN_2879_112_2_18_2_40_27_727,"[MMIMTV]: PlayDefaultServiceAfterSearch SaveLastServiceId")
TRACE_MSG(MMIMTV_MAIN_2888_112_2_18_2_40_27_728,"[MMIMTV]:PlayDefaultServiceAfterSearch PlayNetwork error")
TRACE_MSG(MMIMTV_MAIN_2896_112_2_18_2_40_27_729,"[MMIMTV]: PlayDefaultServiceAfterSearch net_id=%d, service_id=%d")
TRACE_MSG(MMIMTV_MAIN_2903_112_2_18_2_40_27_730,"[MMIMTV]: PlayDefaultServiceAfterSearch play_status error!")
TRACE_MSG(MMIMTV_MAIN_2909_112_2_18_2_40_27_731,"[MMIMTV]: PlayDefaultServiceAfterSearch service Failed !")
TRACE_MSG(MMIMTV_MAIN_2915_112_2_18_2_40_27_732,"[MMIMTV]: PlayDefaultServiceAfterSearch network Failed !")
TRACE_MSG(MMIMTV_MAIN_2928_112_2_18_2_40_27_733,"[MMIMTV]: ResumePlayServiceAfterSearch play_status = %d  net_id = %d service_id = %d")
TRACE_MSG(MMIMTV_MAIN_2942_112_2_18_2_40_27_734,"[MMIMTV]:ResumePlayServiceAfterSearch PlayNetwork error")
TRACE_MSG(MMIMTV_MAIN_2955_112_2_18_2_40_27_735,"[MMIMTV]:ResumePlayServiceAfterSearch play_status error")
TRACE_MSG(MMIMTV_MAIN_3011_112_2_18_2_40_27_736,"[MMIMTV]: SwitchServicePlay current play_status = %d")
TRACE_MSG(MMIMTV_MAIN_3027_112_2_18_2_40_27_737,"[MMIMTV]: Stop error")
TRACE_MSG(MMIMTV_MAIN_3042_112_2_18_2_40_27_738,"[MMIMTV]: Switch Channel now play state is stopped")
TRACE_MSG(MMIMTV_MAIN_3054_112_2_18_2_40_27_739,"[MMIMTV]: PlayNetwork error")
TRACE_MSG(MMIMTV_MAIN_3072_112_2_18_2_40_27_740,"[MMIMBBMS]: StopServicePlay play_status = %d")
TRACE_MSG(MMIMTV_MAIN_3079_112_2_18_2_40_27_741,"[MMIMTV]: Stop error")
TRACE_MSG(MMIMTV_MAIN_3123_112_2_18_2_40_27_742,"[MMIMTV]: PlayRecordfile record fiel total time = %d")
TRACE_MSG(MMIMTV_MAIN_3132_112_2_18_2_40_27_743,"[MMIMTV]: Stop error")
TRACE_MSG(MMIMTV_MAIN_3144_112_2_18_2_40_27_744,"[MMIMTV]: PlayRecordfile now play state is stopped")
TRACE_MSG(MMIMTV_MAIN_3150_112_2_18_2_40_27_745,"[MMIMTV]: PlayStream error")
TRACE_MSG(MMIMTV_MAIN_3174_112_2_18_2_40_27_746,"[MMIMTV]: StartAutoSearch current_play_status = %d")
TRACE_MSG(MMIMTV_MAIN_3194_112_2_18_2_40_27_747,"[MMIMTV]: Stop error")
TRACE_MSG(MMIMTV_MAIN_3216_112_2_18_2_40_27_748,"[MMIMTV]: CMMB_SearchProgram error")
TRACE_MSG(MMIMTV_MAIN_3235_112_2_18_2_40_27_749,"[MMIMTV]: StartFrequencySearch current_play_status = %d  frequency = %d")
TRACE_MSG(MMIMTV_MAIN_3256_112_2_18_2_40_28_750,"[MMIMTV]: Stop error")
TRACE_MSG(MMIMTV_MAIN_3286_112_2_18_2_40_28_751,"[MMIMTV]: CMMB_SearchCenterFrq error")
TRACE_MSG(MMIMTV_MAIN_3305_112_2_18_2_40_28_752,"[MMIMTV]: StartESGSearch current_play_status = %d  net_id = %d")
TRACE_MSG(MMIMTV_MAIN_3351_112_2_18_2_40_28_753,"[MMIMTV]: Stop error")
TRACE_MSG(MMIMTV_MAIN_3367_112_2_18_2_40_28_754,"[MMIMTV]: StartESGSearch stimi_ret = %d")
TRACE_MSG(MMIMTV_MAIN_3391_112_2_18_2_40_28_755,"[MMIMTV]: StartESGParse net_id = %d, parse_status = %d")
TRACE_MSG(MMIMTV_MAIN_3401_112_2_18_2_40_28_756,"[MMIMTV]: StartESGParse no enough space")
TRACE_MSG(MMIMTV_MAIN_3412_112_2_18_2_40_28_757,"[MMIMTV]: CMMB_ParseESG return = %d")
TRACE_MSG(MMIMTV_MAIN_3423_112_2_18_2_40_28_758,"[MMIMTV]: ResetAndQuit !")
TRACE_MSG(MMIMTV_MAIN_3471_112_2_18_2_40_28_759,"[MMIMTV]: ResetCMMBModule !")
TRACE_MSG(MMIMTV_MAIN_3488_112_2_18_2_40_28_760,"[MMIMTV]: MTV_Initialize error")
TRACE_MSG(MMIMTV_MAIN_3507_112_2_18_2_40_28_761,"[MMIMTV]: HandleInitializingSceneMsg, volume_value = 0x%x")
TRACE_MSG(MMIMTV_MAIN_3637_112_2_18_2_40_28_762,"[MMIMTV]: OpenPlayNetworkErrScene net_id=%d")
TRACE_MSG(MMIMTV_MAIN_3654_112_2_18_2_40_28_763,"[MMIMTV]: OpenPlayStreamErrScene")
TRACE_MSG(MMIMTV_MAIN_3674_112_2_18_2_40_28_764,"[MMIMTV]: OpenSearchProgramErrScene")
TRACE_MSG(MMIMTV_MAIN_3706_112_2_18_2_40_28_765,"[MMIMTV]: HandleNoneSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_3735_112_2_18_2_40_28_766,"[MMIMTV]: HandleWarningSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_3782_112_2_18_2_40_29_767,"[MMIMTV]: HandleInitializingSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_3803_112_2_18_2_40_29_768,"[MMIMTV]: HandleInitializingSceneMsg PlayNetwork error")
TRACE_MSG(MMIMTV_MAIN_3858_112_2_18_2_40_29_769,"[MMIMTV]: HandleNoLastServiceSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_3877_112_2_18_2_40_29_770,"[MMIMTV]: CMMB_SearchProgram error")
TRACE_MSG(MMIMTV_MAIN_3967_112_2_18_2_40_29_771,"[MMIMTV]: HandleIdleSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_4102_112_2_18_2_40_29_772,"[MMIMTV]: HandleIdleSceneMsg, num switch error! input_count %d")
TRACE_MSG(MMIMTV_MAIN_4149_112_2_18_2_40_29_773,"[MMIMTV]: MMIMTV_TimeOutPlay")
TRACE_MSG(MMIMTV_MAIN_4181_112_2_18_2_40_29_774,"[MMIMTV]: MMIMTV_NumSwitchPlay input_num = %d")
TRACE_MSG(MMIMTV_MAIN_4185_112_2_18_2_40_29_775,"[MMIMTV]: MMIMTV_NumSwitchPlay !OSD_IDLE osd=%d")
TRACE_MSG(MMIMTV_MAIN_4248_112_2_18_2_40_29_776,"[MMIMTV]: HandleShortcutIconSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_4397_112_2_18_2_40_30_777,"[MMIMTV]: HandleMainMenuSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_4568_112_2_18_2_40_30_778,"[MMIMTV]: HandleMainMenuSceneMsg:menu_id 0x%x is error!")
TRACE_MSG(MMIMTV_MAIN_4597_112_2_18_2_40_30_779,"[MMIMTV]: HandleQuitQuerySceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_4636_112_2_18_2_40_30_780,"[MMIMTV]: HandleNetListSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_4714_112_2_18_2_40_30_781,"[MMIMTV]: HandleNetListOptSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_4772_112_2_18_2_40_31_782,"[MMIMTV]: HandleDelNetQuerySceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_4823_112_2_18_2_40_31_783,"[MMIMTV]: HandleServiceListSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_4959_112_2_18_2_40_31_784,"[MMIMTV]: HandleServiceListOptSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5073_112_2_18_2_40_31_785,"[MMIMTV]: HandleDelServiceQuerySceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5123_112_2_18_2_40_31_786,"[MMIMTV]: HandleSearchWaitingSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5131_112_2_18_2_40_31_787,"[MMIMTV]: Now stop search !")
TRACE_MSG(MMIMTV_MAIN_5160_112_2_18_2_40_31_788,"[MMIMTV]: HandleSearchInvalidFrqSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5195_112_2_18_2_40_32_789,"[MMIMTV]: HandleSearchMenuSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5224_112_2_18_2_40_32_790,"[MMIMTV]: HandleSearchMenuSceneMsg:menu_id %d is error!")
TRACE_MSG(MMIMTV_MAIN_5260_112_2_18_2_40_32_791,"[MMIMTV]: HandleManualSearchSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5274_112_2_18_2_40_32_792,"[MMIMTV]: HandleManualSearchSceneMsg, s_search_mode.center_freq = %d")
TRACE_MSG(MMIMTV_MAIN_5314_112_2_18_2_40_32_793,"[MMIMTV]: HandleEBListSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5384_112_2_18_2_40_32_794,"[MMIMTV]: HandleEBListOptSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5462_112_2_18_2_40_32_795,"[MMIMTV]: HandleDelEBQuerySceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5515_112_2_18_2_40_32_796,"[MMIMTV]: HandleDelAllEBQuerySceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5561_112_2_18_2_40_32_797,"[MMIMTV]: HandleEBDetailSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5568_112_2_18_2_40_32_798,"[MMIMTV]: HandleEBDetailSceneMsg, index = %d")
TRACE_MSG(MMIMTV_MAIN_5601_112_2_18_2_40_32_799,"[MMIMTV]: HandleEBDetailSceneMsg, s_source_osd = %d")
TRACE_MSG(MMIMTV_MAIN_5682_112_2_18_2_40_33_800,"[MMIMTV]: HandleBookingRecordSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5726_112_2_18_2_40_33_801,"[MMIMTV]: HandleESGListSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5730_112_2_18_2_40_33_802,"[MMIMTV]: HandleESGListSceneMsg MMIMTV_OPEN_SCENE  parse_status = %d")
TRACE_MSG(MMIMTV_MAIN_5761_112_2_18_2_40_33_803,"[MMIMTV]: HandleESGListSceneMsg NumOfProg = %d,ProgTotal=%d")
TRACE_MSG(MMIMTV_MAIN_5767_112_2_18_2_40_33_804,"[MMIMTV]: HandleESGListSceneMsg, CMMB_GetEsgDateRange return: %d")
TRACE_MSG(MMIMTV_MAIN_5897_112_2_18_2_40_33_805,"[MMIMTV]: HandleSearchESGQuerySceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5937_112_2_18_2_40_33_806,"[MMIMTV]: HandleSearchESGWaitingSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5945_112_2_18_2_40_33_807,"[MMIMTV]: Now stop search !")
TRACE_MSG(MMIMTV_MAIN_5970_112_2_18_2_40_33_808,"[MMIMTV]: HandlePubSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_5976_112_2_18_2_40_33_809,"[MMIMTV]: HandlePubSceneMsg param is null")
TRACE_MSG(MMIMTV_MAIN_6013_112_2_18_2_40_33_810,"[MMIMTV]: HandleToOtherAppSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6078_112_2_18_2_40_33_811,"[MMIMTV]: HandleSettingSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6199_112_2_18_2_40_34_812,"[MMIMTV]: HandleSettingAudioLanguageSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6204_112_2_18_2_40_34_813,"[MMIMTV]:HandleSettingAudioLanguageSceneMsg, play_status=%d")
TRACE_MSG(MMIMTV_MAIN_6208_112_2_18_2_40_34_814,"[MMIMTV]:HandleSettingAudioLanguageSceneMsg audio_lang_count=%d")
TRACE_MSG(MMIMTV_MAIN_6261_112_2_18_2_40_34_815,"HandleSettingAudioLanguageSceneMsg play_status = %d is wrong")
TRACE_MSG(MMIMTV_MAIN_6287_112_2_18_2_40_34_816,"[MMIMTV]: HandleSettingSubtitleSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6340_112_2_18_2_40_34_817,"[MMIMTV]: HandleSettingAudioModeSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6380_112_2_18_2_40_34_818,"[MMIMTV]: HandleSettingZoomModeSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6422_112_2_18_2_40_34_819,"[MMIMTV]: HandleSettingScreenAdjustSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6459_112_2_18_2_40_34_820,"[MMIMTV]: HandleSettingRecordStoreSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6499_112_2_18_2_40_34_821,"[MMIMTV]: HandleSettingSnapshotStoreSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6551_112_2_18_2_40_34_822,"[MMIMTV]: HandleSettingTransparencySceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6600_112_2_18_2_40_34_823,"[MMIMTV]: HandleSettingDisplayTimeSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6680_112_2_18_2_40_35_824,"[MMIMTV]: HandleSettingBroadcastBGpicSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6725_112_2_18_2_40_35_825,"[MMIMTV]: HandleHelpSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6756_112_2_18_2_40_35_826,"[MMIMTV]: HandleRecordLoadingSceneMsg, msg_id = %d")
TRACE_MSG(MMIMTV_MAIN_6789_112_2_18_2_40_35_827,"[MMIMTV]: HandleRecordPlaySceneMsg, msg_id = %d")
TRACE_MSG(MMIMTV_MAIN_6800_112_2_18_2_40_35_828,"HandleRecordPlaySceneMsg now pause play")
TRACE_MSG(MMIMTV_MAIN_6809_112_2_18_2_40_35_829,"HandleRecordPlaySceneMsg now resume play")
TRACE_MSG(MMIMTV_MAIN_6813_112_2_18_2_40_35_830,"[MMIMTV]: Resume error")
TRACE_MSG(MMIMTV_MAIN_6830_112_2_18_2_40_35_831,"[MMIMTV]: PlayStream error")
TRACE_MSG(MMIMTV_MAIN_6861_112_2_18_2_40_35_832,"[MMIMTV]: Stop error")
TRACE_MSG(MMIMTV_MAIN_6908_112_2_18_2_40_35_833,"[MMIMTV]: HandlePlayConnectingSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6944_112_2_18_2_40_35_834,"[MMIMTV]: HandleFavoriteServiceOptSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_6976_112_2_18_2_40_35_835,"[MMIMTV]: HandleReadEBQuerySceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_7017_112_2_18_2_40_35_836,"[MMIMTV]: HandleSubscribleQuerySceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_7068_112_2_18_2_40_35_837,"[MMIMTV]: HandleCMMBModeWarningSceneMsg, msg_id = 0x%x")
TRACE_MSG(MMIMTV_MAIN_7171_112_2_18_2_40_36_838,"MMIMTV_GetCurrentAudioLanguage audio_language=%d")
TRACE_MSG(MMIMTV_MAIN_7265_112_2_18_2_40_36_839,"AllocAndGetESGInfo networkID = %d, serviceID = %d")
TRACE_MSG(MMIMTV_MAIN_7282_112_2_18_2_40_36_840,"[MMIMTV]: MMIMTV_StartRecord play_status = %d")
TRACE_MSG(MMIMTV_MAIN_7329_112_2_18_2_40_36_841,"[MMIMTV]: MMIMTV_StopRecord")
TRACE_MSG(MMIMTV_MAIN_7357_112_2_18_2_40_36_842,"[MMIMTV]: MMIMTV_PlayRecordCallBack")
TRACE_MSG(MMIMTV_MAIN_7405_112_2_18_2_40_36_843,"[MMIMTV]: MMIMTV_SetFocusOSDScene scene >= OSD_ID_MAX")
TRACE_MSG(MMIMTV_MAIN_7427_112_2_18_2_40_36_844,"[MMIMTV]: MMIMTV_CloseFocusScene s_focus_osd = %d")
TRACE_MSG(MMIMTV_MAIN_7497_112_2_18_2_40_36_845,"[MMIMTV]: MMIMTV_SetOSDRestoreParam param = %d")
TRACE_MSG(MMIMTV_MAIN_7763_112_2_18_2_40_37_846,"[MMIMTV]: SendSignalToTask yuv422_buffer_ptr alloc fail")
TRACE_MSG(MMIMTV_MAIN_7778_112_2_18_2_40_37_847,"[MMIMTV]: SendSignalToTask BL_Malloc fail")
TRACE_MSG(MMIMTV_MAIN_7799_112_2_18_2_40_37_848,"[MMIMTV]: MMIMTV_Snapshot ret: %d, jpeg_buf_ptr:0x%x, jpeg_buf_size : 0x%x ")
TRACE_MSG(MMIMTV_MAIN_7807_112_2_18_2_40_37_849,"[MMIMTV]: error: MMIMTV_Snapshot create file failed!")
TRACE_MSG(MMIMTV_MAIN_7817_112_2_18_2_40_37_850,"[MMIMTV]: MMIMTV_Snapshot write file success")
TRACE_MSG(MMIMTV_MAIN_7822_112_2_18_2_40_37_851,"[MMIMTV]: error:MMIMTV_Snapshot write file failed!")
TRACE_MSG(MMIMTV_MAIN_7927_112_2_18_2_40_37_852,"[MMIMTV]: MMIMTV_CallBack MTV_ESG_PARSE_RESULT_IND p0 = %d, p1 = %d")
TRACE_MSG(MMIMTV_MAIN_7939_112_2_18_2_40_37_853,"[MMIMTV]: MMIMTV_CallBack received MTV_STOP_ACK op_type = %d")
TRACE_MSG(MMIMTV_MAIN_7969_112_2_18_2_40_37_854,"[MMIMTV]: SendSignalToTask alloc sig_to_mmi_ptr fail")
TRACE_MSG(MMIMTV_MAIN_7987_112_2_18_2_40_37_855,"[MMIMTV]: SetIsReceivedStopACK")
TRACE_MSG(MMIMTV_MAIN_8003_112_2_18_2_40_37_856,"[MMIMTV]: MMIMTV_InitCMMB")
TRACE_MSG(MMIMTV_MAIN_8116_112_2_18_2_40_38_857,"MMIMTV_AddServiceItemToTabCtrl net == 0")
TRACE_MSG(MMIMTV_MAIN_8125_112_2_18_2_40_38_858,"MMIMTV_AddServiceItemToTabCtrl service_count = %d large than GUITAB_MAX_ITEM_NUM")
TRACE_MSG(MMIMTV_MAIN_8168_112_2_18_2_40_38_859,"[MMIMBBMS]: MMIMBBMS_CloseCMMB")
TRACE_MSG(MMIMTV_OSD_DISPLAY_586_112_2_18_2_40_40_860,"[MMIMTV]: MMIMTV_NewOSDDisplayData")
TRACE_MSG(MMIMTV_OSD_DISPLAY_1486_112_2_18_2_40_42_861,"[MMIMTV]: OKMuteIcon UnMute")
TRACE_MSG(MMIMTV_OSD_DISPLAY_1507_112_2_18_2_40_42_862,"[MMIMTV]: OKMuteIcon Mute")
TRACE_MSG(MMIMTV_OSD_DISPLAY_1563_112_2_18_2_40_42_863,"[MMIMTV]: OKRecordIcon s_focus_osd = %d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_1602_112_2_18_2_40_42_864,"[MMIMTV]: OKRecordIcon mbbms flow_type=%d, flow_status=%d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_1755_112_2_18_2_40_42_865,"[MMIMTV]: MMIMTV_HandleSystimer MMIMTV_SYSTIME_TIP_PUBWIN")
TRACE_MSG(MMIMTV_OSD_DISPLAY_2751_112_2_18_2_40_44_866,"[MMIMTV]: MMIMTV_OpenNetListScene list_num = %d.")
TRACE_MSG(MMIMTV_OSD_DISPLAY_3019_112_2_18_2_40_45_867,"MMIMTV_OpenAudioLanguageScene lang_code_ptr = %x")
TRACE_MSG(MMIMTV_OSD_DISPLAY_3039_112_2_18_2_40_45_868,"MMIMTV_OpenAudioLanguageScene lang_code = %x")
TRACE_MSG(MMIMTV_OSD_DISPLAY_3542_112_2_18_2_40_46_869,"[MMIMTV]: MMIMTV_OpenEBListScene eb_list_number = %d  param = %d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_3741_112_2_18_2_40_46_870,"[MMIMTV]: CloseRecordListCallBack")
TRACE_MSG(MMIMTV_OSD_DISPLAY_3853_112_2_18_2_40_47_871,"[MMIMTV]: MMIMTV_OpenEBListContentScene, param = %d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_4468_112_2_18_2_40_48_872,"[MMIMTV]: MMIMTV_OpenManualSearchInputScene, freq_num = %d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_4472_112_2_18_2_40_48_873,"[MMIMTV]: MMIMTV_OpenManualSearchInputScene, wstr_freq_len = %d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_4932_112_2_18_2_40_49_874,"[MMIMTV]: MMIMTV_OpenESGListScene, NumOfProg = %d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_5100_112_2_18_2_40_49_875,"[MMIMTV] MMIMTV_OpenSwitchServiceTab MMIMTV_EnableStb")
TRACE_MSG(MMIMTV_OSD_DISPLAY_5214_112_2_18_2_40_49_876,"[MMIMBBMS]:mbbms welcome string len=%d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_5221_112_2_18_2_40_49_877,"MMIMTV_OpenPlayConnectingScene picture_ptr = 0x%x")
TRACE_MSG(MMIMTV_OSD_DISPLAY_5239_112_2_18_2_40_49_878,"MMIMTV_OpenPlayConnectingScene filename[%d] = 0x%x, len = %d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_5578_112_2_18_2_40_50_879,"[MMIMTV] MMIMTV_StartStbDisableTimer if timer id = %d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_5580_112_2_18_2_40_50_880,"[MMIMTV] MMIMTV_StartStbDisableTimer else timer id = %d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_5592_112_2_18_2_40_50_881,"[MMIMTV] MMIMTV_StopStbDisableTimer timer id = %d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_5618_112_2_18_2_40_50_882,"[MMIMTV] MMIMTV_HandleStbDisableTimer type = %d")
TRACE_MSG(MMIMTV_OSD_DISPLAY_5659_112_2_18_2_40_50_883,"[MMIMTV] MMIMTV_DisableStb ctrl_handle = 0x%x")
TRACE_MSG(MMIMTV_OSD_DISPLAY_5677_112_2_18_2_40_50_884,"[MMIMTV] MMIMTV_EnableStb ctrl_handle = 0x%x")
TRACE_MSG(MMIMTV_SIMU_542_112_2_18_2_40_51_885,"mtvesg_common.c.c:MMIMTV_FreeProg()")
TRACE_MSG(MMIMTV_SIMU_609_112_2_18_2_40_51_886,"mtvesg_common.c.c:CMMB_FreeAllProg()")
TRACE_MSG(MMIMTV_WINTAB_267_112_2_18_2_40_53_887,"[MMIMBBMS]: MSG_GET_FOCUS cannot play audio service!")
TRACE_MSG(MMIMTV_WINTAB_275_112_2_18_2_40_53_888,"[MMIMBBMS]: MSG_GET_FOCUS is need play service ")
TRACE_MSG(MMIMTV_WINTAB_281_112_2_18_2_40_53_889,"[MMIMBBMS]: service_id is INVALID")
TRACE_MSG(MMIMTV_WINTAB_311_112_2_18_2_40_53_890,"[MMIMBBMS]: MSG_GET_FOCUS is need resume play")
TRACE_MSG(MMIMTV_WINTAB_546_112_2_18_2_40_53_891,"MMIMBBMS InitDisplayParam: w %d, h %d, angle %d")
END_TRACE_MAP(MMI_APP_MOBILETV_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_MOBILETV_TRC_H_


/******************************************************************************
 ** File Name:      mmi_app_bt_trc.h                                         *
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
//trace_id:157
#ifndef _MMI_APP_BT_TRC_H_
#define _MMI_APP_BT_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIBT_A2DP_112_112_2_18_2_7_43_0 "[MMIBT] MMIBT_IsA2dpReadyForPlay: bt_status = %d, result = %d!"
#define MMIBT_A2DP_130_112_2_18_2_7_43_1 "[MMIBT] MMIBT_IsA2dpPaused: bt_status = %d, result = %d!"
#define MMIBT_A2DP_144_112_2_18_2_7_43_2 "[MMIBT]MMIBT_A2dpPause enter"
#define MMIBT_A2DP_148_112_2_18_2_7_43_3 "[MMIBT]Bt_A2dpPause: A2DP_PauseDevice is called!"
#define MMIBT_A2DP_153_112_2_18_2_7_43_4 "[MMIBT]Bt_A2dpPause: call A2DP_PauseDevice fail!"
#define MMIBT_A2DP_170_112_2_18_2_7_43_5 "[MMIBT]: MMIBT_A2dpStopDevice"
#define MMIBT_A2DP_182_112_2_18_2_7_43_6 "[MMIBT]:MMIBT_A2dpStopDevice: A2DP_CloseDevice is called!"
#define MMIBT_A2DP_187_112_2_18_2_7_43_7 "[MMIBT]:MMIBT_A2dpStopDevice: call A2DP_CloseDevice fail!"
#define MMIBT_A2DP_215_112_2_18_2_7_43_8 "[MMIBT]: MMIBT_A2dpResumeDevice"
#define MMIBT_A2DP_223_112_2_18_2_7_43_9 "[MMIBT]MMIBT_A2dpResumeDevice: A2DP_ResumeDevice is called!"
#define MMIBT_A2DP_229_112_2_18_2_7_43_10 "[MMIBT]MMIBT_A2dpResumeDevice: call A2DP_ResumeDevice fail!"
#define MMIBT_A2DP_252_112_2_18_2_7_43_11 "[MMIBT]MMIBT_A2dpOpenDevice: A2DP_OpenDevice is called!"
#define MMIBT_A2DP_256_112_2_18_2_7_43_12 "[MMIBT]MMIBT_A2dpOpenDevice: A2DP_OpenDevice failed !"
#define MMIBT_A2DP_277_112_2_18_2_7_43_13 "MMIBT_GetCurrentA2dpStatus PNULL == addr_ptr!"
#define MMIBT_A2DP_477_112_2_18_2_7_44_14 "[MMIBT]mmibt_a2dp.c:MMIBT_OpenA2dpHeadset:call A2DP_ConnectDevice:result is %d"
#define MMIBT_A2DP_494_112_2_18_2_7_44_15 "[MMIBT]mmibt_a2dp.c:MMIBT_CloseA2dpHeadset:call A2DP_DisConnectDevice:result is %d"
#define MMIBT_A2DP_535_112_2_18_2_7_44_16 "[MMIBT]MMIBT_GetActiveA2DPDevice:state = %d"
#define MMIBT_APP_71_112_2_18_2_7_44_17 "[MMIBT] MMIAPIBT_SpecifyHeadset headset_type %d"
#define MMIBT_APP_121_112_2_18_2_7_45_18 "[MMIBT]MMIAPIBT_GetActiveBTHeadset result %d s_specified_headset %d"
#define MMIBT_APP_151_112_2_18_2_7_45_19 "[MMIBT] MMIAPIBT_GetActiveHeadsetType type %d"
#define MMIBT_APP_192_112_2_18_2_7_45_20 "[MMIBT]: MMIBT_PauseDevice a2dp_app_type = %d, s_a2dp_app_info.app_type = %d"
#define MMIBT_APP_263_112_2_18_2_7_45_21 "MMIBT_OpenDevice result %d"
#define MMIBT_APP_278_112_2_18_2_7_45_22 "[MMIBT]: MMIBT_ResumeDevice a2dp_app_type = %d, s_a2dp_app_info.app_type = %d"
#define MMIBT_APP_285_112_2_18_2_7_45_23 "MMIBT_ResumeDevice s_process_func_ptr PNULL !"
#define MMIBT_APP_311_112_2_18_2_7_45_24 "MMIBT_ResumeDevice action_result %d"
#define MMIBT_APP_325_112_2_18_2_7_45_25 "[MMIBT]: MMIBT_StopDevice"
#define MMIBT_APP_329_112_2_18_2_7_45_26 "MMIBT_StopDevice s_process_func_ptr PNULL !"
#define MMIBT_APP_334_112_2_18_2_7_45_27 "[MMIBT]MMIBT_StopDevice result %d"
#define MMIBT_APP_355_112_2_18_2_7_45_28 "[MMIBT]MMIBT_GetDeviceStatus s_device_status %d"
#define MMIBT_APP_365_112_2_18_2_7_45_29 "[MMIBT]MMIBT_SetDeviceStatus audio_status %d"
#define MMIBT_APP_377_112_2_18_2_7_45_30 "[MMIBT]MMIBT_OpenDeviceCnf:  status = %d"
#define MMIBT_APP_385_112_2_18_2_7_45_31 "[MMIBT]MMIBT_OpenDeviceCnf: error!!!!!!  "
#define MMIBT_APP_405_112_2_18_2_7_45_32 "[MMIBT]MMIBT_ResumeDeviceCnf:  status = %d,act_init = %d "
#define MMIBT_APP_431_112_2_18_2_7_45_33 "[MMIBT]MMIBT_PauseDeviceCnf:  pause cnf status is %d"
#define MMIBT_APP_446_112_2_18_2_7_45_34 "[MMIBT]MMIBT_PauseDeviceCnf: pause audio fail,close audio service"
#define MMIBT_APP_456_112_2_18_2_7_45_35 "[MMIBT]MMIBT_CloseDeviceCnf:  status = %d"
#define MMIBT_APP_464_112_2_18_2_7_45_36 "[MMIBT]MMIBT_CloseDeviceCnf:  error!!!!!!"
#define MMIBT_APP_498_112_2_18_2_7_45_37 "[MMIBT]HFG_OpenDevice result %d"
#define MMIBT_APP_515_112_2_18_2_7_45_38 "[MMIBT]HFG_StopDevice result %d"
#define MMIBT_APP_535_112_2_18_2_7_46_39 "[MMIBT]HFG_ResumeDevice result %d"
#define MMIBT_APP_553_112_2_18_2_7_46_40 "[MMIBT]HFG_PauseDevice result %d"
#define MMIBT_APP_572_112_2_18_2_7_46_41 "MMIAPIBT_GetActiveHeadsetName PNULL == headset_name_ptr!"
#define MMIBT_APP_616_112_2_18_2_7_46_42 "[MMIBT] MMIAPIBT_GetActiveHeadsetName result = %d, *name_len_ptr = %d"
#define MMIBT_APP_734_112_2_18_2_7_46_43 "MMIBT_GetDeviceStatus s_device_status %d"
#define MMIBT_APP_744_112_2_18_2_7_46_44 "MMIBT_SetDeviceStatus audio_status %d"
#define MMIBT_APP_893_112_2_18_2_7_46_45 "s_action_queue.action[%d] %d"
#define MMIBT_APP_906_112_2_18_2_7_46_46 "MMIBT_RemoveAction_FromQueue s_action_queue.action_num %d"
#define MMIBT_APP_922_112_2_18_2_7_46_47 "MMIBT_In_ActionQueue queue full  %d"
#define MMIBT_APP_949_112_2_18_2_7_46_48 "MMIBT_In_ActionQueue act %d"
#define MMIBT_APP_957_112_2_18_2_7_46_49 "[MMIBT] In_ActionQueue act %d ret %d"
#define MMIBT_APP_986_112_2_18_2_7_46_50 "[MMIBT] Out_ActionQueue act %d"
#define MMIBT_APP_1004_112_2_18_2_7_47_51 "MMIBT_IsActionQueueEmpty ret %d"
#define MMIBT_APP_1063_112_2_18_2_7_47_52 "MMIBT Bt_SetA2dpBusyFlag %d"
#define MMIBT_APP_1073_112_2_18_2_7_47_53 "MMIBT_DeviceIsBusy %d"
#define MMIBT_EXPORT_732_112_2_18_2_7_48_54 "MMIAPIBT_SendMultiFile PNULL != send_file_info_ptr"
#define MMIBT_EXPORT_793_112_2_18_2_7_48_55 "[&&&]MMIBT_SendNextFileCallBack start,cursendfileindex =%d,send_file_num =%d"
#define MMIBT_EXPORT_894_112_2_18_2_7_49_56 "MMIAPIBT_GetActivatedHS PNULL == addr!"
#define MMIBT_EXPORT_1039_112_2_18_2_7_49_57 "MMIBT_OutgoingCall :Call the function: BT_StartAlert"
#define MMIBT_EXPORT_1063_112_2_18_2_7_49_58 "MMIBT_IncomingCall  :Call the function: BT_StartRing"
#define MMIBT_EXPORT_1126_112_2_18_2_7_49_59 "mmibt.c: MMIBT_SendSignal:sig_id=%d"
#define MMIBT_EXPORT_1154_112_2_18_2_7_49_60 "AppHandleBTMsg: msg_id = %d"
#define MMIBT_EXPORT_1159_112_2_18_2_7_49_61 "AppHandleBTMsg: MMI_BT_ME_ON_CNF"
#define MMIBT_EXPORT_1164_112_2_18_2_7_49_62 "AppHandleBTMsg:MMI_BT_ME_OFF_CNF"
#define MMIBT_EXPORT_1169_112_2_18_2_7_49_63 "AppHandleBTMsg:MMI_BT_INQUIRY_CANCEL"
#define MMIBT_EXPORT_1174_112_2_18_2_7_49_64 "AppHandleBTMsg:MMI_BT_VISIBILE_CNF"
#define MMIBT_EXPORT_1179_112_2_18_2_7_49_65 "AppHandleBTMsg:MMI_BT_HIDDEN_CNF"
#define MMIBT_EXPORT_1184_112_2_18_2_7_49_66 "AppHandleBTMsg: MMI_BT_INQ_DEV_NAME"
#define MMIBT_EXPORT_1189_112_2_18_2_7_49_67 "MMI_BT_INQ_COMP_CNF"
#define MMIBT_EXPORT_1195_112_2_18_2_7_49_68 "MMI_BT_DEV_PIN_REQ"
#define MMIBT_EXPORT_1200_112_2_18_2_7_49_69 "MMI_BT_DEV_PAIR_COMPLETE"
#define MMIBT_EXPORT_1205_112_2_18_2_7_49_70 "MMI_BT_DEV_PAIR_COMPLETE_IND"
#define MMIBT_EXPORT_1209_112_2_18_2_7_49_71 "MMI_BT_DELETE_DEVICE_RES"
#define MMIBT_EXPORT_1215_112_2_18_2_7_49_72 "MMI_BT_HFAG_SLC_CONNECTED"
#define MMIBT_EXPORT_1219_112_2_18_2_7_49_73 "MMI_BT_HFAG_SLC_CONNECTED_IND"
#define MMIBT_EXPORT_1223_112_2_18_2_7_49_74 "MMI_BT_HFAG_SLC_DISCONNECTED"
#define MMIBT_EXPORT_1228_112_2_18_2_7_49_75 "MMI_BT_CALL1_CONNECTED"
#define MMIBT_EXPORT_1233_112_2_18_2_7_49_76 "MMI_BT_HFU_CALL_HANGUP"
#define MMIBT_EXPORT_1238_112_2_18_2_7_49_77 "MMI_BT_HFAG_DIAL_NUMBER"
#define MMIBT_EXPORT_1243_112_2_18_2_7_49_78 "MMI_BT_HFAG_DIAL_MEMORY"
#define MMIBT_EXPORT_1252_112_2_18_2_7_49_79 "MMI_BT_HFU_CALL_HANGUP"
#define MMIBT_EXPORT_1258_112_2_18_2_7_49_80 "MMI_BT_HFAG_REDAIL_CALL"
#define MMIBT_EXPORT_1264_112_2_18_2_7_49_81 "MMI_BT_HFAG_HFU_HOLD"
#define MMIBT_EXPORT_1268_112_2_18_2_7_49_82 "MMI_BT_HFU_DTMF_NUMBER"
#define MMIBT_EXPORT_1271_112_2_18_2_7_49_83 "MMI_BT_HFU_DTMF_NUMBER char 0x%x"
#define MMIBT_EXPORT_1277_112_2_18_2_7_49_84 "MMI_BT_HFAG_AUDIO_CONNECTED"
#define MMIBT_EXPORT_1282_112_2_18_2_7_49_85 "MMI_BT_HFAG_AUDIO_DISCONNECTED"
#define MMIBT_EXPORT_1287_112_2_18_2_7_49_86 "MMI_BT_HFU_STATUS_REQ"
#define MMIBT_EXPORT_1293_112_2_18_2_7_49_87 "MMI_BT_HS_SPK_VOL_CHANGE"
#define MMIBT_EXPORT_1298_112_2_18_2_7_49_88 "MMI_BT_HFU_CLCC"
#define MMIBT_EXPORT_1307_112_2_18_2_7_50_89 "MMI_BT_DUN_LINK_REQ"
#define MMIBT_EXPORT_1311_112_2_18_2_7_50_90 "MMI_BT_DUN_LINK_SUC"
#define MMIBT_EXPORT_1315_112_2_18_2_7_50_91 "MMI_BT_DUN_LINK_CLOSE"
#define MMIBT_EXPORT_1324_112_2_18_2_7_50_92 "MMI_BT_OPC_CONNECT_RESULT"
#define MMIBT_EXPORT_1330_112_2_18_2_7_50_93 "MMI_BT_OPC_SERVER_ABORT"
#define MMIBT_EXPORT_1334_112_2_18_2_7_50_94 "MMI_BT_SEND_CANCELED"
#define MMIBT_EXPORT_1338_112_2_18_2_7_50_95 "MMI_BT_RECEIVE_CANCELED"
#define MMIBT_EXPORT_1342_112_2_18_2_7_50_96 "MMI_BT_PACKET_SENT"
#define MMIBT_EXPORT_1347_112_2_18_2_7_50_97 "MMI_BT_OPC_SEND_RES"
#define MMIBT_EXPORT_1353_112_2_18_2_7_50_98 "MMI_BT_FTS_GET_REQ"
#define MMIBT_EXPORT_1358_112_2_18_2_7_50_99 "MMI_BT_FTS_SHARED_FOLDER_UPDATE"
#define MMIBT_EXPORT_1363_112_2_18_2_7_50_100 "MMI_BT_OPS_CONNECT_REQ"
#define MMIBT_EXPORT_1369_112_2_18_2_7_50_101 "MMI_BT_OPS_PUT_REQ!"
#define MMIBT_EXPORT_1375_112_2_18_2_7_50_102 "Opp/FTP server client abort!"
#define MMIBT_EXPORT_1383_112_2_18_2_7_50_103 "OPS/FTS_DISCONNECT_IND"
#define MMIBT_EXPORT_1387_112_2_18_2_7_50_104 "OPC_DISCONNECT_IND"
#define MMIBT_EXPORT_1392_112_2_18_2_7_50_105 "MMI_BT_PACKET_RECEIVED"
#define MMIBT_EXPORT_1397_112_2_18_2_7_50_106 "MMI_BT_RECEIVE_FILE_RES"
#define MMIBT_EXPORT_1402_112_2_18_2_7_50_107 "Ftp server put request!"
#define MMIBT_EXPORT_1406_112_2_18_2_7_50_108 "MMI_BT_FTS_DEL_REQ!"
#define MMIBT_EXPORT_1411_112_2_18_2_7_50_109 "MMI_BT_FTS_DEL_FAIL!"
#define MMIBT_EXPORT_1415_112_2_18_2_7_50_110 "MMI_BT_FTS_ADD_FOLDER_REQ!"
#define MMIBT_EXPORT_1420_112_2_18_2_7_50_111 "MMI_BT_FTS_CONNECT_REQ"
#define MMIBT_EXPORT_1426_112_2_18_2_7_50_112 "MMI_BT_OPEN_DEVICE_CNF"
#define MMIBT_EXPORT_1431_112_2_18_2_7_50_113 "MMI_BT_RESUME_DEVICE_CNF"
#define MMIBT_EXPORT_1436_112_2_18_2_7_50_114 "MMI_BT_SUSPEND_DEVICE_CNF"
#define MMIBT_EXPORT_1441_112_2_18_2_7_50_115 "MMI_BT_CLOSE_DEVICE_CNF"
#define MMIBT_EXPORT_1446_112_2_18_2_7_50_116 "MMI_BT_PLAY_RESUNME_DEVICE_CNF"
#define MMIBT_EXPORT_1450_112_2_18_2_7_50_117 "MMI_BT_PAUSE_STOP_DEVICE_CNF"
#define MMIBT_EXPORT_1457_112_2_18_2_7_50_118 "MMI_BT_A2DP_OPERATION_BACKWARD"
#define MMIBT_EXPORT_1462_112_2_18_2_7_50_119 "MMI_BT_A2DP_OPERATION_FORWARD"
#define MMIBT_EXPORT_1467_112_2_18_2_7_50_120 "MMI_BT_A2DP_OPERATION_VOL_UP"
#define MMIBT_EXPORT_1472_112_2_18_2_7_50_121 "MMI_BT_A2DP_OPERATION_VOL_DOWN"
#define MMIBT_EXPORT_1477_112_2_18_2_7_50_122 "MMI_BT_A2DP_OPERATION_PLAY"
#define MMIBT_EXPORT_1482_112_2_18_2_7_50_123 "MMI_BT_A2DP_OPERATION_STOP"
#define MMIBT_EXPORT_1487_112_2_18_2_7_50_124 "MMI_BT_A2DP_OPERATION_PAUSE"
#define MMIBT_EXPORT_1492_112_2_18_2_7_50_125 "MMI_BT_A2DP_START_STREAM_IND"
#define MMIBT_EXPORT_1497_112_2_18_2_7_50_126 "MMI_BT_A2DP_SUSPEND_STREAM_IND"
#define MMIBT_EXPORT_1502_112_2_18_2_7_50_127 "MMI_BT_A2DP_RESUME_STREAM_IND"
#define MMIBT_EXPORT_1507_112_2_18_2_7_50_128 "MMI_BT_A2DP_CONNECTED"
#define MMIBT_EXPORT_1513_112_2_18_2_7_50_129 "MMI_BT_A2DP_ABORT_IND"
#define MMIBT_EXPORT_1518_112_2_18_2_7_50_130 "MMI_BT_A2DP_CONNECTED_IND"
#define MMIBT_EXPORT_1522_112_2_18_2_7_50_131 "MMI_BT_A2DP_DISCONNECTED"
#define MMIBT_EXPORT_1530_112_2_18_2_7_50_132 "MMI_BT   default"
#define MMIBT_EXPORT_1637_112_2_18_2_7_50_133 "MMIBT_CallBackFunc:restart_condition=%d"
#define MMIBT_EXPORT_1655_112_2_18_2_7_50_134 "MMIBT_CallBackFunc, msg_id = %d"
#define MMIBT_EXPORT_1660_112_2_18_2_7_50_135 "Bluetooth power on success.\n"
#define MMIBT_EXPORT_1666_112_2_18_2_7_50_136 "Bluetooth power off success.\n"
#define MMIBT_EXPORT_1672_112_2_18_2_7_50_137 "Bluetooth found 1 device: "
#define MMIBT_EXPORT_1678_112_2_18_2_7_50_138 "Bluetooth got the address and the name of the device.\n"
#define MMIBT_EXPORT_1684_112_2_18_2_7_50_139 "Bluetooth inquiry completed\n"
#define MMIBT_EXPORT_1690_112_2_18_2_7_50_140 "Bluetooth inquiry is cancelled.\n"
#define MMIBT_EXPORT_1696_112_2_18_2_7_50_141 "Bluetooth set visible success.\n"
#define MMIBT_EXPORT_1702_112_2_18_2_7_50_142 "Bluetooth set invisible success.\n"
#define MMIBT_EXPORT_1708_112_2_18_2_7_50_143 "Bluetooth input pin request"
#define MMIBT_EXPORT_1714_112_2_18_2_7_50_144 "Bluetooth pairing success.\n"
#define MMIBT_EXPORT_1720_112_2_18_2_7_50_145 "Bluetooth earphone pairing success.\n"
#define MMIBT_EXPORT_1725_112_2_18_2_7_50_146 "Bluetooth got delete paired device result.\n"
#define MMIBT_EXPORT_1731_112_2_18_2_7_50_147 "Bluetooth read service result.\n"
#define MMIBT_EXPORT_1737_112_2_18_2_7_50_148 "Bluetooth headset service connected.\n"
#define MMIBT_EXPORT_1743_112_2_18_2_7_50_149 "IND Bluetooth headset service connected.\n"
#define MMIBT_EXPORT_1748_112_2_18_2_7_50_150 "Bluetooth hand free service disconnected.\n"
#define MMIBT_EXPORT_1754_112_2_18_2_7_50_151 "MMI_BT_PLAY_RESUNME_DEVICE_CNF.\n"
#define MMIBT_EXPORT_1770_112_2_18_2_7_50_152 "MMI_BT_PAUSE_STOP_DEVICE_CNF.\n"
#define MMIBT_EXPORT_1785_112_2_18_2_7_50_153 "Bluetooth ID_STATUS_HFAG_HFU_CALL_ANSWER.\n"
#define MMIBT_EXPORT_1791_112_2_18_2_7_51_154 "Bluetooth ID_STATUS_HF_CALL1_HUNGUP.\n"
#define MMIBT_EXPORT_1797_112_2_18_2_7_51_155 "Bluetooth ID_STATUS_HFAG_HFU_DIAL_NUMBER.\n"
#define MMIBT_EXPORT_1803_112_2_18_2_7_51_156 "Bluetooth ID_STATUS_HFAG_HFU_DIAL_MEMORY.\n"
#define MMIBT_EXPORT_1809_112_2_18_2_7_51_157 "Bluetooth ID_STATUS_HFAG_HFU_REDIAL.\n"
#define MMIBT_EXPORT_1815_112_2_18_2_7_51_158 "Bluetooth ID_STATUS_HFAG_HFU_HOLD.\n"
#define MMIBT_EXPORT_1820_112_2_18_2_7_51_159 "Bluetooth ID_STATUS_HFAG_HFU_DTMF_NUMBER.\n"
#define MMIBT_EXPORT_1825_112_2_18_2_7_51_160 "Bluetooth ID_STATUS_HFAG_AUDIO_CONNECTED.\n"
#define MMIBT_EXPORT_1831_112_2_18_2_7_51_161 "Bluetooth ID_STATUS_HFAG_AUDIO_DISCONNECTED.\n"
#define MMIBT_EXPORT_1837_112_2_18_2_7_51_162 "Bluetooth ID_STATUS_HFAG_HFU_STATUS_REQ.\n"
#define MMIBT_EXPORT_1843_112_2_18_2_7_51_163 "Bluetooth ID_STATUS_HS_MIC_VOL_CHANGE.\n"
#define MMIBT_EXPORT_1849_112_2_18_2_7_51_164 "Bluetooth ID_STATUS_HS_SPK_VOL_CHANGE.\n"
#define MMIBT_EXPORT_1855_112_2_18_2_7_51_165 "Bluetooth ID_STATUS_HFAG_HFU_CLCC.\n"
#define MMIBT_EXPORT_1864_112_2_18_2_7_51_166 "Bluetooth Opc connect result.\n"
#define MMIBT_EXPORT_1870_112_2_18_2_7_51_167 "Bluetooth Opc server abort ind.\n"
#define MMIBT_EXPORT_1876_112_2_18_2_7_51_168 "Bluetooth Opc put abort ind.\n"
#define MMIBT_EXPORT_1882_112_2_18_2_7_51_169 "Bluetooth ID_STATUS_OPC_PUT_OBJECT.\n"
#define MMIBT_EXPORT_1888_112_2_18_2_7_51_170 "Bluetooth ID_STATUS_OPC_PUT_OBJECT.\n"
#define MMIBT_EXPORT_1895_112_2_18_2_7_51_171 "Bluetooth ID_STATUS_OPC_DISCONNECT_IND"
#define MMIBT_EXPORT_1904_112_2_18_2_7_51_172 "Bluetooth ID_STATUS_DUN_LINK_REQ.\n"
#define MMIBT_EXPORT_1909_112_2_18_2_7_51_173 "Bluetooth ID_STATUS_DG_LINK_STATUS_RES.\n"
#define MMIBT_EXPORT_1914_112_2_18_2_7_51_174 "Bluetooth ID_STATUS_DG_LINK_CLOSE_IND.\n"
#define MMIBT_EXPORT_1921_112_2_18_2_7_51_175 "Bluetooth ID_STATUS_FTS_GET_REQ.\n"
#define MMIBT_EXPORT_1928_112_2_18_2_7_51_176 "Bluetooth ID_STATUS_FTS_UPDATE_ROOT"
#define MMIBT_EXPORT_1933_112_2_18_2_7_51_177 "Bluetooth ID_STATUS_FTC_CONNECT_RES.\n"
#define MMIBT_EXPORT_1939_112_2_18_2_7_51_178 "Bluetooth ID_STATUS_FTC_SERVER_ABORT.\n"
#define MMIBT_EXPORT_1945_112_2_18_2_7_51_179 "Bluetooth ID_STATUS_FTC_PUT_ABORT.\n"
#define MMIBT_EXPORT_1951_112_2_18_2_7_51_180 "Bluetooth ID_STATUS_FTC_PUT_OBJECT.\n"
#define MMIBT_EXPORT_1957_112_2_18_2_7_51_181 "Bluetooth ID_STATUS_FTC_PUT_FINAL.\n"
#define MMIBT_EXPORT_1965_112_2_18_2_7_51_182 "Bluetooth ID_STATUS_OPC_PUT_OBJECT.\n"
#define MMIBT_EXPORT_1971_112_2_18_2_7_51_183 "Bluetooth ID_STATUS_OPS_CLIENT_ABORT.\n"
#define MMIBT_EXPORT_1977_112_2_18_2_7_51_184 "Bluetooth ID_STATUS_OPS_PUT_ABORT.\n"
#define MMIBT_EXPORT_1983_112_2_18_2_7_51_185 "Bluetooth ID_STATUS_OPS_PUT_OBJECT.\n"
#define MMIBT_EXPORT_1989_112_2_18_2_7_51_186 "Bluetooth ID_STATUS_OPS_PUT_FINAL.\n"
#define MMIBT_EXPORT_1996_112_2_18_2_7_51_187 "Bluetooth ID_STATUS_OPS_DISCONNECT_IND.\n"
#define MMIBT_EXPORT_2005_112_2_18_2_7_51_188 "Bluetooth ID_STATUS_OPC_PUT_OBJECT.\n"
#define MMIBT_EXPORT_2011_112_2_18_2_7_51_189 "Bluetooth ID_STATUS_FTS_CONNECT_REQ.\n"
#define MMIBT_EXPORT_2017_112_2_18_2_7_51_190 "Bluetooth ID_STATUS_OPS_CONNECT_REQ.\n"
#define MMIBT_EXPORT_2023_112_2_18_2_7_51_191 "Bluetooth ID_STATUS_FTS_CLIENT_ABORT.\n"
#define MMIBT_EXPORT_2029_112_2_18_2_7_51_192 "Bluetooth ID_STATUS_FTS_PUT_ABORT.\n"
#define MMIBT_EXPORT_2035_112_2_18_2_7_51_193 "Bluetooth ID_STATUS_FTS_PUT_OBJECT.\n"
#define MMIBT_EXPORT_2041_112_2_18_2_7_51_194 "Bluetooth ID_STATUS_FTS_PUT_FINAL.\n"
#define MMIBT_EXPORT_2046_112_2_18_2_7_51_195 "Bluetooth ID_STATUS_FTS_GET_ABORT.\n"
#define MMIBT_EXPORT_2051_112_2_18_2_7_51_196 "Bluetooth ID_STATUS_FTS_GET_FINAL.\n"
#define MMIBT_EXPORT_2056_112_2_18_2_7_51_197 "Bluetooth ID_STATUS_FTS_GET_OBJECT.\n"
#define MMIBT_EXPORT_2062_112_2_18_2_7_51_198 "Bluetooth ID_STATUS_FTS_DEL_REQ.\n"
#define MMIBT_EXPORT_2067_112_2_18_2_7_51_199 "Bluetooth ID_STATUS_FTS_DEL_BUSY.\n"
#define MMIBT_EXPORT_2072_112_2_18_2_7_51_200 "Bluetooth ID_STATUS_FTS_ADD_FOLDER_REQ.\n"
#define MMIBT_EXPORT_2079_112_2_18_2_7_51_201 "Bluetooth ID_STATUS_FTS_DISCONNECT_IND.\n"
#define MMIBT_EXPORT_2086_112_2_18_2_7_51_202 "Bluetooth signal unsolvable 0x%x.\n"
#define MMIBT_EXPORT_2113_112_2_18_2_7_51_203 "MMIBT_A2DPCallBackFunc:restart_condition=%d"
#define MMIBT_EXPORT_2131_112_2_18_2_7_51_204 "MMIBT_A2DPCallBackFunc: pMsg->msg_id = %d"
#define MMIBT_EXPORT_2137_112_2_18_2_7_51_205 "Bluetooth ID_STATUS_AVRCP_OPERATION_ID_BACKWARD.\n"
#define MMIBT_EXPORT_2143_112_2_18_2_7_51_206 "Bluetooth ID_STATUS_AVRCP_OPERATION_ID_FORWARD.\n"
#define MMIBT_EXPORT_2149_112_2_18_2_7_51_207 "Bluetooth ID_STATUS_AVRCP_OPERATION_ID_VOL_UP.\n"
#define MMIBT_EXPORT_2155_112_2_18_2_7_51_208 "Bluetooth ID_STATUS_AVRCP_OPERATION_ID_VOL_DOWN.\n"
#define MMIBT_EXPORT_2161_112_2_18_2_7_51_209 "Bluetooth ID_STATUS_AVRCP_OPERATION_ID_PLAY.\n"
#define MMIBT_EXPORT_2167_112_2_18_2_7_51_210 "Bluetooth ID_STATUS_AVRCP_OPERATION_ID_STOP.\n"
#define MMIBT_EXPORT_2173_112_2_18_2_7_51_211 "Bluetooth ID_STATUS_AVRCP_OPERATION_ID_PAUSE.\n"
#define MMIBT_EXPORT_2179_112_2_18_2_7_51_212 "Bluetooth ID_STATUS_AVRCP_CONNECT_COMPLETE.\n"
#define MMIBT_EXPORT_2185_112_2_18_2_7_51_213 "Bluetooth ID_STATUS_AVRCP_DISCONNECT_COMPLETE.\n"
#define MMIBT_EXPORT_2193_112_2_18_2_7_51_214 "bluetooth A2DP SRC config strem"
#define MMIBT_EXPORT_2199_112_2_18_2_7_51_215 "bluetooth A2DP SRC open strem"
#define MMIBT_EXPORT_2203_112_2_18_2_7_51_216 "Bluetooth ID_STATUS_A2DP_SRC_START_STREAM.\n"
#define MMIBT_EXPORT_2209_112_2_18_2_7_51_217 "Bluetooth ID_STATUS_A2DP_SRC_RESUME_STREAM.\n"
#define MMIBT_EXPORT_2215_112_2_18_2_7_51_218 "Bluetooth ID_STATUS_A2DP_SRC_SUSPEND_STREAM.\n"
#define MMIBT_EXPORT_2221_112_2_18_2_7_51_219 "Bluetooth ID_STATUS_A2DP_SRC_CLOSE_STREAM.\n"
#define MMIBT_EXPORT_2227_112_2_18_2_7_51_220 "Bluetooth ID_STATUS_A2DP_CONNECTED.\n"
#define MMIBT_EXPORT_2233_112_2_18_2_7_51_221 "IND Bluetooth ID_STATUS_A2DP_CONNECTED.\n"
#define MMIBT_EXPORT_2239_112_2_18_2_7_51_222 "Bluetooth ID_STATUS_A2DP_DISCONNECTED.\n"
#define MMIBT_EXPORT_2245_112_2_18_2_7_51_223 "Bluetooth ID_STATUS_A2DP_SNK_START_STREAM.\n"
#define MMIBT_EXPORT_2251_112_2_18_2_7_51_224 "Bluetooth ID_STATUS_A2DP_SNK_SUSPEND_STREAM.\n"
#define MMIBT_EXPORT_2257_112_2_18_2_7_51_225 "Bluetooth ID_STATUS_A2DP_ABORT_CFM.\n"
#define MMIBT_EXPORT_2263_112_2_18_2_7_51_226 "Bluetooth ID_STATUS_A2DP_ABORT_IND.\n"
#define MMIBT_EXPORT_2269_112_2_18_2_7_51_227 "Bluetooth ID_STATUS_A2DP_SNK_RESUME_STREAM.\n"
#define MMIBT_EXPORT_2276_112_2_18_2_7_52_228 "Bluetooth signal unsolvable 0x%x.\n"
#define MMIBT_EXPORT_2301_112_2_18_2_7_52_229 "MMIAPIBT_GetAddress PNULL == addr_ptr!"
#define MMIBT_EXPORT_2610_112_2_18_2_7_52_230 "MMIAPIBT_SetOperatorName name <%s>"
#define MMIBT_EXPORT_2627_112_2_18_2_7_52_231 "MMIAPIBT_SetCallInfo idx %d dir %d call_status %d mode %d mpty %d"
#define MMIBT_EXPORT_2628_112_2_18_2_7_52_232 "MMIAPIBT_SetCallInfo number  %s"
#define MMIBT_EXPORT_2644_112_2_18_2_7_52_233 "MMIAPIBT_SetNetWorkRoamStatus roam %d"
#define MMIBT_EXPORT_2671_112_2_18_2_7_52_234 "MMIAPIBT_SetMyPhoneNumber num_buf %s"
#define MMIBT_EXPORT_2682_112_2_18_2_7_52_235 "MMIAPIBT_SetCallSuccess "
#define MMIBT_EXPORT_2694_112_2_18_2_7_52_236 "MMIAPIBT_SetCallFail "
#define MMIBT_EXPORT_2708_112_2_18_2_7_52_237 "MMIAPIBT_Service_Indicator cmd_value %d is_in_net %d"
#define MMIBT_WINTAB_1606_112_2_18_2_8_2_238 "MMIBT_UpdateFtpSharedFolder PNULL == root_folder_ptr!"
#define MMIBT_WINTAB_1633_112_2_18_2_8_2_239 "MMIBT_IsDeviceActivedEarphone PNULL == addr_ptr!"
#define MMIBT_WINTAB_1638_112_2_18_2_8_2_240 " MMIBT_IsDeviceActivedEarphone: return value is %d"
#define MMIBT_WINTAB_3882_112_2_18_2_8_8_241 "MMIBT_SendNextFile PNULL == filepath_name!"
#define MMIBT_WINTAB_3885_112_2_18_2_8_8_242 "[&&&]MMIBT_SendNextFile start,file_size = %d"
#define MMIBT_WINTAB_3917_112_2_18_2_8_8_243 "MMIBT_SendFile PNULL == filepath_name!"
#define MMIBT_WINTAB_4000_112_2_18_2_8_8_244 "SetIsDelWallpaperFile: Set del file is wallpaper = %d"
#define MMIBT_WINTAB_4010_112_2_18_2_8_8_245 "GetIsDelWallpaperFile: Get file is wallpaper = %d"
#define MMIBT_WINTAB_4030_112_2_18_2_8_8_246 "MMIBT_FtsDelReq PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_4130_112_2_18_2_8_8_247 "MMIBT_ConnectDeviceResCnf PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_4161_112_2_18_2_8_8_248 "[&&&]MMIBT_ConnectDeviceResCnf"
#define MMIBT_WINTAB_4396_112_2_18_2_8_9_249 "[&&&]HandleFileTransferWindow Status = %d"
#define MMIBT_WINTAB_4464_112_2_18_2_8_9_250 "[&&&]HandleSendConnectWindow"
#define MMIBT_WINTAB_4542_112_2_18_2_8_9_251 "AssignReceiveFileCfg service_type %d "
#define MMIBT_WINTAB_4763_112_2_18_2_8_9_252 "MMIBT_DeleteDeviceCnf PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_4766_112_2_18_2_8_9_253 "MMIBT_DeleteDeviceCnf: The action result is %d"
#define MMIBT_WINTAB_4974_112_2_18_2_8_10_254 "[MMIBT] DeletePairedDevice, close MMIBT_DELETE_DEVICE_WAIT_WIN_ID"
#define MMIBT_WINTAB_4981_112_2_18_2_8_10_255 "[MMIBT] DeletePairedDevice is_all %d delete_result %d is_deleted %d"
#define MMIBT_WINTAB_5042_112_2_18_2_8_10_256 "[&&&]s_bt_file_incoming = %d"
#define MMIBT_WINTAB_5405_112_2_18_2_8_11_257 "MMIBT_GetFtpReq PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_5454_112_2_18_2_8_11_258 "MMIBT_GetInputPinReq PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_5470_112_2_18_2_8_11_259 "MMIBT_GetInputPinReq in pairing !!"
#define MMIBT_WINTAB_5479_112_2_18_2_8_11_260 "MMIBT_GetInputPinReq in repairing !!"
#define MMIBT_WINTAB_5487_112_2_18_2_8_11_261 "MMIBT_GetInputPinReq in Cacelling !!"
#define MMIBT_WINTAB_5565_112_2_18_2_8_11_262 "MMIBT_OpenIncomingFileWin PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_5576_112_2_18_2_8_11_263 "MMIBT_OpenIncomingFileWin:name_len is %d"
#define MMIBT_WINTAB_5692_112_2_18_2_8_11_264 "MMIBT_GetBtOnOffCnf status %d current_opertion %d"
#define MMIBT_WINTAB_5854_112_2_18_2_8_11_265 "MMIBT_GetVisibleCnf PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_5939_112_2_18_2_8_12_266 "MMIBT_GetInqDevName in calling or alarming !"
#define MMIBT_WINTAB_5947_112_2_18_2_8_12_267 "MMIBT_GetInqDevName status %d !"
#define MMIBT_WINTAB_5952_112_2_18_2_8_12_268 "MMIBT_GetInqDevName alerady exist s_inquired_device_num %d !"
#define MMIBT_WINTAB_6084_112_2_18_2_8_12_269 "MMIBT_GetPairComCnf msg_body_ptr->status %d"
#define MMIBT_WINTAB_6264_112_2_18_2_8_13_270 "MMIBT_OpenHeadsetCnf PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_6281_112_2_18_2_8_13_271 "MMIBT_OpenHeadsetCnf address = %x, %x, %x, %x, %x, %x"
#define MMIBT_WINTAB_6286_112_2_18_2_8_13_272 "MMIBT_OpenHeadsetCnf msg_body_ptr->status %d"
#define MMIBT_WINTAB_6324_112_2_18_2_8_13_273 "MMIBT MMIBT_OpenHeadsetCnf enter, TXT_BT_HEADSET_OPENED"
#define MMIBT_WINTAB_6371_112_2_18_2_8_13_274 "MMIBT_CloseHeadsetCnf PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_6374_112_2_18_2_8_13_275 "MMIBT_CloseHeadsetCnf is_a2dp %d msg_body_ptr->status %d"
#define MMIBT_WINTAB_6387_112_2_18_2_8_13_276 "MMIBT_CloseHeadsetCnf address = %x, %x, %x, %x, %x, %x"
#define MMIBT_WINTAB_6549_112_2_18_2_8_13_277 "MMIBT_HfagDiaMemory index=%d, memory_indicat=%d"
#define MMIBT_WINTAB_6625_112_2_18_2_8_13_278 "MMIBT_AudioConnected PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_6655_112_2_18_2_8_13_279 "MMIBT_AudioDisconnected PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_6786_112_2_18_2_8_14_280 "MMIBT_GetPacketReceivedInd: the received data is = %d"
#define MMIBT_WINTAB_6797_112_2_18_2_8_14_281 "MMIBT_GetPacketSentInd s_transfer_file_info.transfer_file_size %d transfer_info.transmitted_count %d"
#define MMIBT_WINTAB_6824_112_2_18_2_8_14_282 "MMIBT_GetPacketReceivedInd: the received data is = %d"
#define MMIBT_WINTAB_6903_112_2_18_2_8_14_283 "MMIBT_SendFileResInd PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_6948_112_2_18_2_8_14_284 "[&&&]MMIBT_SendFileFailedInd"
#define MMIBT_WINTAB_7007_112_2_18_2_8_14_285 "MMIBT_ReceiveFileResInd PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_7067_112_2_18_2_8_14_286 "MMIBT_ReceiveFileFail s_transfer_file_info.server_type %d"
#define MMIBT_WINTAB_7151_112_2_18_2_8_14_287 "[&&&]DoOperation start,MMIBT_CancelFileTransfer"
#define MMIBT_WINTAB_7185_112_2_18_2_8_14_288 "MMIBT_CancelSearchCnf  status is = %d"
#define MMIBT_WINTAB_7254_112_2_18_2_8_15_289 "MMIBT_GetIsDeviceVisible: visible status is = %d"
#define MMIBT_WINTAB_7573_112_2_18_2_8_15_290 "MMIBT_GetPairedDevList PNULL == paired_dev_ptr!"
#define MMIBT_WINTAB_7774_112_2_18_2_8_16_291 "SetFileSaveLocation: location info is = %d, return_value is = %d "
#define MMIBT_WINTAB_7816_112_2_18_2_8_16_292 "MMIBT_IsPhoneReady is_ready %d"
#define MMIBT_WINTAB_7852_112_2_18_2_8_16_293 "[MMIBT] MMIBT_GetOpertor root_opt %d, current_opt %d"
#define MMIBT_WINTAB_7889_112_2_18_2_8_16_294 "CreateListMenu window already opened !"
#define MMIBT_WINTAB_7999_112_2_18_2_8_16_295 "[MMIBT]MMIBT_OpenBluetooth set_on %d ret_val %d"
#define MMIBT_WINTAB_8078_112_2_18_2_8_16_296 "bt is_file_type = %d"
#define MMIBT_WINTAB_8087_112_2_18_2_8_16_297 "bt file_handle = %d"
#define MMIBT_WINTAB_8097_112_2_18_2_8_16_298 "bt SFS_ERROR_E = %d"
#define MMIBT_WINTAB_8123_112_2_18_2_8_16_299 "[MMIBT] HandleQueryWinMsg win_id 0x%x, msg_id 0x%x s_current_query_num %d"
#define MMIBT_WINTAB_8162_112_2_18_2_8_16_300 "OpenQueryWin MSG_APP_OK current_opc %d "
#define MMIBT_WINTAB_8175_112_2_18_2_8_16_301 "MMIBT file name %s"
#define MMIBT_WINTAB_8285_112_2_18_2_8_17_302 "DoOperation current_opt %d is_poweron_first %d"
#define MMIBT_WINTAB_8288_112_2_18_2_8_17_303 "DoOperation in calling or alarming ! current_opt %d be cancelled"
#define MMIBT_WINTAB_8372_112_2_18_2_8_17_304 "[&&&]DoOperation start,OPC_OPP_SEND"
#define MMIBT_WINTAB_8551_112_2_18_2_8_17_305 "[&&&]DoOperation start,CancelOperation MMIBT_OPC_OPP_SEND"
#define MMIBT_WINTAB_8621_112_2_18_2_8_18_306 "CreateList window already opened !"
#define MMIBT_WINTAB_8902_112_2_18_2_8_18_307 "[MMIBT]BtPauseBGPlay: TYPE %d "
#define MMIBT_WINTAB_8919_112_2_18_2_8_18_308 "BtResumeBGPlay type %d"
#define MMIBT_WINTAB_8943_112_2_18_2_8_18_309 "[MMIBT]BtResumeBGPlay: TYPE %d ret %d exist %d"
#define MMIBT_WINTAB_8961_112_2_18_2_8_18_310 "DeviceIsPaired PNULL == addr!"
#define MMIBT_WINTAB_8976_112_2_18_2_8_18_311 "mmibt.c: DeviceIsPaired device number is %d ret %d"
#define MMIBT_WINTAB_9016_112_2_18_2_8_18_312 "MMIBT_DeviceIsConnectedHeadset ret %d"
#define MMIBT_WINTAB_9039_112_2_18_2_8_18_313 "BT_FTSGetState ftp_status %d"
#define MMIBT_WINTAB_9046_112_2_18_2_8_18_314 "DeviceIsConnectedFTP ret %d"
#define MMIBT_WINTAB_9077_112_2_18_2_8_18_315 "MMIBT_OpenedHeadsetIsExist exist %d"
#define MMIBT_WINTAB_9149_112_2_18_2_8_19_316 "HeadsetIsClosed ret %d"
#define MMIBT_WINTAB_9445_112_2_18_2_8_19_317 "GetDeviceListIcon PNULL == dev!"
#define MMIBT_WINTAB_9449_112_2_18_2_8_19_318 "BT_GetDeviceType dev_type %d"
#define MMIBT_WINTAB_9500_112_2_18_2_8_19_319 "MMIBT_OpenFileCallBack, Enter,is_success = %d!"
#define MMIBT_WINTAB_9504_112_2_18_2_8_19_320 "MMIBT_OpenFileCallBack, file_num = %d"
#define MMIBT_WINTAB_9631_112_2_18_2_8_20_321 "GetFileInfo alloc fail !"
#define MMIBT_WINTAB_9731_112_2_18_2_8_20_322 "MMIBT_OpenDUNWin fail PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_9734_112_2_18_2_8_20_323 "MMIBT_OpenDUNWin msg_body_ptr->status %d"
#define MMIBT_WINTAB_9754_112_2_18_2_8_20_324 "UART selected SIO_UserPortBusy"
#define MMIBT_WINTAB_9763_112_2_18_2_8_20_325 "UART selected"
#define MMIBT_WINTAB_9771_112_2_18_2_8_20_326 "MMIAPIPHONE_GetSimStatus not ready"
#define MMIBT_WINTAB_9776_112_2_18_2_8_20_327 "MMIAPIBT_OpenDUNWin error"
#define MMIBT_WINTAB_9794_112_2_18_2_8_20_328 "MMIBT_DUNOpenedInd PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_9797_112_2_18_2_8_20_329 "MMIBT_DUNOpenedInd msg_body_ptr->status %d"
#define MMIBT_WINTAB_9837_112_2_18_2_8_20_330 "MMIBT_DUNClosedInd PNULL == msg_body_ptr!"
#define MMIBT_WINTAB_9840_112_2_18_2_8_20_331 "MMIBT_DUNClosedInd msg_body_ptr->status %d"
#define MMIBT_WINTAB_9888_112_2_18_2_8_20_332 "HandleConnectDUNOrNotMsg Cancel"
#define MMIBT_WINTAB_9900_112_2_18_2_8_20_333 "HandleConnectDUNOrNotMsg Accept"
#define MMIBT_WINTAB_10081_112_2_18_2_8_21_334 "UpdateCurrentFileList enter"
#define MMIBT_WINTAB_10130_112_2_18_2_8_21_335 "UpdateCurrentPictureViewList"
#define MMIBT_WINTAB_10155_112_2_18_2_8_21_336 "MMIBT_Reset enter"
#define MMIBT_WINTAB_10227_112_2_18_2_8_21_337 "Create_TextLabel window already opened !"
#define MMIBT_WINTAB_10341_112_2_18_2_8_21_338 "Get_Transfer_DeviceName string invalid !"
#define MMIBT_WINTAB_10357_112_2_18_2_8_21_339 "file_name string invalid !"
#define MMIBT_WINTAB_10550_112_2_18_2_8_22_340 "MMIBT_OPC_SAVE_LOCATION cur_select_item %d"
#define MMIBT_WINTAB_10608_112_2_18_2_8_22_341 "Is_PermitOpenFile result %d s_previous_win_id 0x%x "
#define MMIBT_WINTAB_10694_112_2_18_2_8_22_342 "CloseHeadset enter"
#define MMIBT_WINTAB_10737_112_2_18_2_8_22_343 "OpenHeadset enter"
#define MMIBT_WINTAB_10745_112_2_18_2_8_22_344 "OpenHeadset address = %x, %x, %x, %x, %x, %x"
#define MMIBT_WINTAB_10746_112_2_18_2_8_22_345 "OpenHeadset src_addr = %x, %x, %x, %x, %x, %x"
#define MMIBT_WINTAB_10769_112_2_18_2_8_22_346 "MMIBT LOCAL void OpenHeadset enter, TXT_BT_HEADSET_OPENED"
#define MMIBT_WINTAB_10854_112_2_18_2_8_22_347 "HeadsetOpenIsPermit in calling or alarming !"
#define MMIBT_WINTAB_10865_112_2_18_2_8_22_348 "HeadsetOpenIsPermit exist alrady opened headset !"
#define MMIBT_WINTAB_10875_112_2_18_2_8_22_349 "HeadsetOpenIsPermit in call !"
#define MMIBT_WINTAB_10887_112_2_18_2_8_22_350 "HeadsetOpenIsPermit mp3 pause or playing !"
#define MMIBT_WINTAB_10893_112_2_18_2_8_22_351 "HeadsetOpenIsPermit kuro pause or playing  !"
#define MMIBT_WINTAB_10900_112_2_18_2_8_22_352 "HeadsetOpenIsPermit ok"
#define MMIBT_WINTAB_11013_112_2_18_2_8_22_353 "MMIBT_GetPairComIND msg_body_ptr->status %d"
#define MMIBT_WINTAB_11086_112_2_18_2_8_23_354 "MMIBT_OpenHeadsetConfirmIND msg_body_ptr is null"
#define MMIBT_WINTAB_11101_112_2_18_2_8_23_355 "MMIBT_OpenHeadsetConfirmIND address = %x, %x, %x, %x, %x, %x"
#define MMIBT_WINTAB_11106_112_2_18_2_8_23_356 "MMIBT_OpenHeadsetConfirmIND msg_body_ptr->status %d"
#define MMIBT_WINTAB_11122_112_2_18_2_8_23_357 "MMIBT MMIBT_OpenHeadsetConfirmIND enter, TXT_BT_HEADSET_OPENED"
#define MMIBT_WINTAB_11160_112_2_18_2_8_23_358 "MMIBT_ClccInd msg_body %d"
#define MMIBT_WINTAB_11237_112_2_18_2_8_23_359 "mmibt_wintab.c: start to install drm!"
#define MMIBT_WINTAB_11243_112_2_18_2_8_23_360 "mmibt_wintab.c: file not exist!"
#define MMIBT_WINTAB_11309_112_2_18_2_8_23_361 "MMIBT_OpenFMMDirectory: result =%d"
#define MMIBT_WINTAB_11325_112_2_18_2_8_23_362 "MMIBT_SetFileLocationCallBack, Enter,is_success = %d!"
#define MMIBT_WINTAB_11329_112_2_18_2_8_23_363 "MMIBT_SetFileLocationCallBack, file_num = %d"
#define MMIBT_WINTAB_11475_112_2_18_2_8_23_364 "[&&&]MMIBT_StopBTTransferFile"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_BT_TRC)
TRACE_MSG(MMIBT_A2DP_112_112_2_18_2_7_43_0,"[MMIBT] MMIBT_IsA2dpReadyForPlay: bt_status = %d, result = %d!")
TRACE_MSG(MMIBT_A2DP_130_112_2_18_2_7_43_1,"[MMIBT] MMIBT_IsA2dpPaused: bt_status = %d, result = %d!")
TRACE_MSG(MMIBT_A2DP_144_112_2_18_2_7_43_2,"[MMIBT]MMIBT_A2dpPause enter")
TRACE_MSG(MMIBT_A2DP_148_112_2_18_2_7_43_3,"[MMIBT]Bt_A2dpPause: A2DP_PauseDevice is called!")
TRACE_MSG(MMIBT_A2DP_153_112_2_18_2_7_43_4,"[MMIBT]Bt_A2dpPause: call A2DP_PauseDevice fail!")
TRACE_MSG(MMIBT_A2DP_170_112_2_18_2_7_43_5,"[MMIBT]: MMIBT_A2dpStopDevice")
TRACE_MSG(MMIBT_A2DP_182_112_2_18_2_7_43_6,"[MMIBT]:MMIBT_A2dpStopDevice: A2DP_CloseDevice is called!")
TRACE_MSG(MMIBT_A2DP_187_112_2_18_2_7_43_7,"[MMIBT]:MMIBT_A2dpStopDevice: call A2DP_CloseDevice fail!")
TRACE_MSG(MMIBT_A2DP_215_112_2_18_2_7_43_8,"[MMIBT]: MMIBT_A2dpResumeDevice")
TRACE_MSG(MMIBT_A2DP_223_112_2_18_2_7_43_9,"[MMIBT]MMIBT_A2dpResumeDevice: A2DP_ResumeDevice is called!")
TRACE_MSG(MMIBT_A2DP_229_112_2_18_2_7_43_10,"[MMIBT]MMIBT_A2dpResumeDevice: call A2DP_ResumeDevice fail!")
TRACE_MSG(MMIBT_A2DP_252_112_2_18_2_7_43_11,"[MMIBT]MMIBT_A2dpOpenDevice: A2DP_OpenDevice is called!")
TRACE_MSG(MMIBT_A2DP_256_112_2_18_2_7_43_12,"[MMIBT]MMIBT_A2dpOpenDevice: A2DP_OpenDevice failed !")
TRACE_MSG(MMIBT_A2DP_277_112_2_18_2_7_43_13,"MMIBT_GetCurrentA2dpStatus PNULL == addr_ptr!")
TRACE_MSG(MMIBT_A2DP_477_112_2_18_2_7_44_14,"[MMIBT]mmibt_a2dp.c:MMIBT_OpenA2dpHeadset:call A2DP_ConnectDevice:result is %d")
TRACE_MSG(MMIBT_A2DP_494_112_2_18_2_7_44_15,"[MMIBT]mmibt_a2dp.c:MMIBT_CloseA2dpHeadset:call A2DP_DisConnectDevice:result is %d")
TRACE_MSG(MMIBT_A2DP_535_112_2_18_2_7_44_16,"[MMIBT]MMIBT_GetActiveA2DPDevice:state = %d")
TRACE_MSG(MMIBT_APP_71_112_2_18_2_7_44_17,"[MMIBT] MMIAPIBT_SpecifyHeadset headset_type %d")
TRACE_MSG(MMIBT_APP_121_112_2_18_2_7_45_18,"[MMIBT]MMIAPIBT_GetActiveBTHeadset result %d s_specified_headset %d")
TRACE_MSG(MMIBT_APP_151_112_2_18_2_7_45_19,"[MMIBT] MMIAPIBT_GetActiveHeadsetType type %d")
TRACE_MSG(MMIBT_APP_192_112_2_18_2_7_45_20,"[MMIBT]: MMIBT_PauseDevice a2dp_app_type = %d, s_a2dp_app_info.app_type = %d")
TRACE_MSG(MMIBT_APP_263_112_2_18_2_7_45_21,"MMIBT_OpenDevice result %d")
TRACE_MSG(MMIBT_APP_278_112_2_18_2_7_45_22,"[MMIBT]: MMIBT_ResumeDevice a2dp_app_type = %d, s_a2dp_app_info.app_type = %d")
TRACE_MSG(MMIBT_APP_285_112_2_18_2_7_45_23,"MMIBT_ResumeDevice s_process_func_ptr PNULL !")
TRACE_MSG(MMIBT_APP_311_112_2_18_2_7_45_24,"MMIBT_ResumeDevice action_result %d")
TRACE_MSG(MMIBT_APP_325_112_2_18_2_7_45_25,"[MMIBT]: MMIBT_StopDevice")
TRACE_MSG(MMIBT_APP_329_112_2_18_2_7_45_26,"MMIBT_StopDevice s_process_func_ptr PNULL !")
TRACE_MSG(MMIBT_APP_334_112_2_18_2_7_45_27,"[MMIBT]MMIBT_StopDevice result %d")
TRACE_MSG(MMIBT_APP_355_112_2_18_2_7_45_28,"[MMIBT]MMIBT_GetDeviceStatus s_device_status %d")
TRACE_MSG(MMIBT_APP_365_112_2_18_2_7_45_29,"[MMIBT]MMIBT_SetDeviceStatus audio_status %d")
TRACE_MSG(MMIBT_APP_377_112_2_18_2_7_45_30,"[MMIBT]MMIBT_OpenDeviceCnf:  status = %d")
TRACE_MSG(MMIBT_APP_385_112_2_18_2_7_45_31,"[MMIBT]MMIBT_OpenDeviceCnf: error!!!!!!  ")
TRACE_MSG(MMIBT_APP_405_112_2_18_2_7_45_32,"[MMIBT]MMIBT_ResumeDeviceCnf:  status = %d,act_init = %d ")
TRACE_MSG(MMIBT_APP_431_112_2_18_2_7_45_33,"[MMIBT]MMIBT_PauseDeviceCnf:  pause cnf status is %d")
TRACE_MSG(MMIBT_APP_446_112_2_18_2_7_45_34,"[MMIBT]MMIBT_PauseDeviceCnf: pause audio fail,close audio service")
TRACE_MSG(MMIBT_APP_456_112_2_18_2_7_45_35,"[MMIBT]MMIBT_CloseDeviceCnf:  status = %d")
TRACE_MSG(MMIBT_APP_464_112_2_18_2_7_45_36,"[MMIBT]MMIBT_CloseDeviceCnf:  error!!!!!!")
TRACE_MSG(MMIBT_APP_498_112_2_18_2_7_45_37,"[MMIBT]HFG_OpenDevice result %d")
TRACE_MSG(MMIBT_APP_515_112_2_18_2_7_45_38,"[MMIBT]HFG_StopDevice result %d")
TRACE_MSG(MMIBT_APP_535_112_2_18_2_7_46_39,"[MMIBT]HFG_ResumeDevice result %d")
TRACE_MSG(MMIBT_APP_553_112_2_18_2_7_46_40,"[MMIBT]HFG_PauseDevice result %d")
TRACE_MSG(MMIBT_APP_572_112_2_18_2_7_46_41,"MMIAPIBT_GetActiveHeadsetName PNULL == headset_name_ptr!")
TRACE_MSG(MMIBT_APP_616_112_2_18_2_7_46_42,"[MMIBT] MMIAPIBT_GetActiveHeadsetName result = %d, *name_len_ptr = %d")
TRACE_MSG(MMIBT_APP_734_112_2_18_2_7_46_43,"MMIBT_GetDeviceStatus s_device_status %d")
TRACE_MSG(MMIBT_APP_744_112_2_18_2_7_46_44,"MMIBT_SetDeviceStatus audio_status %d")
TRACE_MSG(MMIBT_APP_893_112_2_18_2_7_46_45,"s_action_queue.action[%d] %d")
TRACE_MSG(MMIBT_APP_906_112_2_18_2_7_46_46,"MMIBT_RemoveAction_FromQueue s_action_queue.action_num %d")
TRACE_MSG(MMIBT_APP_922_112_2_18_2_7_46_47,"MMIBT_In_ActionQueue queue full  %d")
TRACE_MSG(MMIBT_APP_949_112_2_18_2_7_46_48,"MMIBT_In_ActionQueue act %d")
TRACE_MSG(MMIBT_APP_957_112_2_18_2_7_46_49,"[MMIBT] In_ActionQueue act %d ret %d")
TRACE_MSG(MMIBT_APP_986_112_2_18_2_7_46_50,"[MMIBT] Out_ActionQueue act %d")
TRACE_MSG(MMIBT_APP_1004_112_2_18_2_7_47_51,"MMIBT_IsActionQueueEmpty ret %d")
TRACE_MSG(MMIBT_APP_1063_112_2_18_2_7_47_52,"MMIBT Bt_SetA2dpBusyFlag %d")
TRACE_MSG(MMIBT_APP_1073_112_2_18_2_7_47_53,"MMIBT_DeviceIsBusy %d")
TRACE_MSG(MMIBT_EXPORT_732_112_2_18_2_7_48_54,"MMIAPIBT_SendMultiFile PNULL != send_file_info_ptr")
TRACE_MSG(MMIBT_EXPORT_793_112_2_18_2_7_48_55,"[&&&]MMIBT_SendNextFileCallBack start,cursendfileindex =%d,send_file_num =%d")
TRACE_MSG(MMIBT_EXPORT_894_112_2_18_2_7_49_56,"MMIAPIBT_GetActivatedHS PNULL == addr!")
TRACE_MSG(MMIBT_EXPORT_1039_112_2_18_2_7_49_57,"MMIBT_OutgoingCall :Call the function: BT_StartAlert")
TRACE_MSG(MMIBT_EXPORT_1063_112_2_18_2_7_49_58,"MMIBT_IncomingCall  :Call the function: BT_StartRing")
TRACE_MSG(MMIBT_EXPORT_1126_112_2_18_2_7_49_59,"mmibt.c: MMIBT_SendSignal:sig_id=%d")
TRACE_MSG(MMIBT_EXPORT_1154_112_2_18_2_7_49_60,"AppHandleBTMsg: msg_id = %d")
TRACE_MSG(MMIBT_EXPORT_1159_112_2_18_2_7_49_61,"AppHandleBTMsg: MMI_BT_ME_ON_CNF")
TRACE_MSG(MMIBT_EXPORT_1164_112_2_18_2_7_49_62,"AppHandleBTMsg:MMI_BT_ME_OFF_CNF")
TRACE_MSG(MMIBT_EXPORT_1169_112_2_18_2_7_49_63,"AppHandleBTMsg:MMI_BT_INQUIRY_CANCEL")
TRACE_MSG(MMIBT_EXPORT_1174_112_2_18_2_7_49_64,"AppHandleBTMsg:MMI_BT_VISIBILE_CNF")
TRACE_MSG(MMIBT_EXPORT_1179_112_2_18_2_7_49_65,"AppHandleBTMsg:MMI_BT_HIDDEN_CNF")
TRACE_MSG(MMIBT_EXPORT_1184_112_2_18_2_7_49_66,"AppHandleBTMsg: MMI_BT_INQ_DEV_NAME")
TRACE_MSG(MMIBT_EXPORT_1189_112_2_18_2_7_49_67,"MMI_BT_INQ_COMP_CNF")
TRACE_MSG(MMIBT_EXPORT_1195_112_2_18_2_7_49_68,"MMI_BT_DEV_PIN_REQ")
TRACE_MSG(MMIBT_EXPORT_1200_112_2_18_2_7_49_69,"MMI_BT_DEV_PAIR_COMPLETE")
TRACE_MSG(MMIBT_EXPORT_1205_112_2_18_2_7_49_70,"MMI_BT_DEV_PAIR_COMPLETE_IND")
TRACE_MSG(MMIBT_EXPORT_1209_112_2_18_2_7_49_71,"MMI_BT_DELETE_DEVICE_RES")
TRACE_MSG(MMIBT_EXPORT_1215_112_2_18_2_7_49_72,"MMI_BT_HFAG_SLC_CONNECTED")
TRACE_MSG(MMIBT_EXPORT_1219_112_2_18_2_7_49_73,"MMI_BT_HFAG_SLC_CONNECTED_IND")
TRACE_MSG(MMIBT_EXPORT_1223_112_2_18_2_7_49_74,"MMI_BT_HFAG_SLC_DISCONNECTED")
TRACE_MSG(MMIBT_EXPORT_1228_112_2_18_2_7_49_75,"MMI_BT_CALL1_CONNECTED")
TRACE_MSG(MMIBT_EXPORT_1233_112_2_18_2_7_49_76,"MMI_BT_HFU_CALL_HANGUP")
TRACE_MSG(MMIBT_EXPORT_1238_112_2_18_2_7_49_77,"MMI_BT_HFAG_DIAL_NUMBER")
TRACE_MSG(MMIBT_EXPORT_1243_112_2_18_2_7_49_78,"MMI_BT_HFAG_DIAL_MEMORY")
TRACE_MSG(MMIBT_EXPORT_1252_112_2_18_2_7_49_79,"MMI_BT_HFU_CALL_HANGUP")
TRACE_MSG(MMIBT_EXPORT_1258_112_2_18_2_7_49_80,"MMI_BT_HFAG_REDAIL_CALL")
TRACE_MSG(MMIBT_EXPORT_1264_112_2_18_2_7_49_81,"MMI_BT_HFAG_HFU_HOLD")
TRACE_MSG(MMIBT_EXPORT_1268_112_2_18_2_7_49_82,"MMI_BT_HFU_DTMF_NUMBER")
TRACE_MSG(MMIBT_EXPORT_1271_112_2_18_2_7_49_83,"MMI_BT_HFU_DTMF_NUMBER char 0x%x")
TRACE_MSG(MMIBT_EXPORT_1277_112_2_18_2_7_49_84,"MMI_BT_HFAG_AUDIO_CONNECTED")
TRACE_MSG(MMIBT_EXPORT_1282_112_2_18_2_7_49_85,"MMI_BT_HFAG_AUDIO_DISCONNECTED")
TRACE_MSG(MMIBT_EXPORT_1287_112_2_18_2_7_49_86,"MMI_BT_HFU_STATUS_REQ")
TRACE_MSG(MMIBT_EXPORT_1293_112_2_18_2_7_49_87,"MMI_BT_HS_SPK_VOL_CHANGE")
TRACE_MSG(MMIBT_EXPORT_1298_112_2_18_2_7_49_88,"MMI_BT_HFU_CLCC")
TRACE_MSG(MMIBT_EXPORT_1307_112_2_18_2_7_50_89,"MMI_BT_DUN_LINK_REQ")
TRACE_MSG(MMIBT_EXPORT_1311_112_2_18_2_7_50_90,"MMI_BT_DUN_LINK_SUC")
TRACE_MSG(MMIBT_EXPORT_1315_112_2_18_2_7_50_91,"MMI_BT_DUN_LINK_CLOSE")
TRACE_MSG(MMIBT_EXPORT_1324_112_2_18_2_7_50_92,"MMI_BT_OPC_CONNECT_RESULT")
TRACE_MSG(MMIBT_EXPORT_1330_112_2_18_2_7_50_93,"MMI_BT_OPC_SERVER_ABORT")
TRACE_MSG(MMIBT_EXPORT_1334_112_2_18_2_7_50_94,"MMI_BT_SEND_CANCELED")
TRACE_MSG(MMIBT_EXPORT_1338_112_2_18_2_7_50_95,"MMI_BT_RECEIVE_CANCELED")
TRACE_MSG(MMIBT_EXPORT_1342_112_2_18_2_7_50_96,"MMI_BT_PACKET_SENT")
TRACE_MSG(MMIBT_EXPORT_1347_112_2_18_2_7_50_97,"MMI_BT_OPC_SEND_RES")
TRACE_MSG(MMIBT_EXPORT_1353_112_2_18_2_7_50_98,"MMI_BT_FTS_GET_REQ")
TRACE_MSG(MMIBT_EXPORT_1358_112_2_18_2_7_50_99,"MMI_BT_FTS_SHARED_FOLDER_UPDATE")
TRACE_MSG(MMIBT_EXPORT_1363_112_2_18_2_7_50_100,"MMI_BT_OPS_CONNECT_REQ")
TRACE_MSG(MMIBT_EXPORT_1369_112_2_18_2_7_50_101,"Opp server put request!")
TRACE_MSG(MMIBT_EXPORT_1375_112_2_18_2_7_50_102,"Opp/FTP server client abort!")
TRACE_MSG(MMIBT_EXPORT_1383_112_2_18_2_7_50_103,"OPS/FTS_DISCONNECT_IND")
TRACE_MSG(MMIBT_EXPORT_1387_112_2_18_2_7_50_104,"OPC_DISCONNECT_IND")
TRACE_MSG(MMIBT_EXPORT_1392_112_2_18_2_7_50_105,"MMI_BT_PACKET_RECEIVED")
TRACE_MSG(MMIBT_EXPORT_1397_112_2_18_2_7_50_106,"MMI_BT_RECEIVE_FILE_RES")
TRACE_MSG(MMIBT_EXPORT_1402_112_2_18_2_7_50_107,"Ftp server put request!")
TRACE_MSG(MMIBT_EXPORT_1406_112_2_18_2_7_50_108,"MMI_BT_FTS_DEL_REQ!")
TRACE_MSG(MMIBT_EXPORT_1411_112_2_18_2_7_50_109,"MMI_BT_FTS_DEL_FAIL!")
TRACE_MSG(MMIBT_EXPORT_1415_112_2_18_2_7_50_110,"MMI_BT_FTS_ADD_FOLDER_REQ!")
TRACE_MSG(MMIBT_EXPORT_1420_112_2_18_2_7_50_111,"MMI_BT_FTS_CONNECT_REQ")
TRACE_MSG(MMIBT_EXPORT_1426_112_2_18_2_7_50_112,"MMI_BT_OPEN_DEVICE_CNF")
TRACE_MSG(MMIBT_EXPORT_1431_112_2_18_2_7_50_113,"MMI_BT_RESUME_DEVICE_CNF")
TRACE_MSG(MMIBT_EXPORT_1436_112_2_18_2_7_50_114,"MMI_BT_SUSPEND_DEVICE_CNF")
TRACE_MSG(MMIBT_EXPORT_1441_112_2_18_2_7_50_115,"MMI_BT_CLOSE_DEVICE_CNF")
TRACE_MSG(MMIBT_EXPORT_1446_112_2_18_2_7_50_116,"MMI_BT_PLAY_RESUNME_DEVICE_CNF")
TRACE_MSG(MMIBT_EXPORT_1450_112_2_18_2_7_50_117,"MMI_BT_PAUSE_STOP_DEVICE_CNF")
TRACE_MSG(MMIBT_EXPORT_1457_112_2_18_2_7_50_118,"MMI_BT_A2DP_OPERATION_BACKWARD")
TRACE_MSG(MMIBT_EXPORT_1462_112_2_18_2_7_50_119,"MMI_BT_A2DP_OPERATION_FORWARD")
TRACE_MSG(MMIBT_EXPORT_1467_112_2_18_2_7_50_120,"MMI_BT_A2DP_OPERATION_VOL_UP")
TRACE_MSG(MMIBT_EXPORT_1472_112_2_18_2_7_50_121,"MMI_BT_A2DP_OPERATION_VOL_DOWN")
TRACE_MSG(MMIBT_EXPORT_1477_112_2_18_2_7_50_122,"MMI_BT_A2DP_OPERATION_PLAY")
TRACE_MSG(MMIBT_EXPORT_1482_112_2_18_2_7_50_123,"MMI_BT_A2DP_OPERATION_STOP")
TRACE_MSG(MMIBT_EXPORT_1487_112_2_18_2_7_50_124,"MMI_BT_A2DP_OPERATION_PAUSE")
TRACE_MSG(MMIBT_EXPORT_1492_112_2_18_2_7_50_125,"MMI_BT_A2DP_START_STREAM_IND")
TRACE_MSG(MMIBT_EXPORT_1497_112_2_18_2_7_50_126,"MMI_BT_A2DP_SUSPEND_STREAM_IND")
TRACE_MSG(MMIBT_EXPORT_1502_112_2_18_2_7_50_127,"MMI_BT_A2DP_RESUME_STREAM_IND")
TRACE_MSG(MMIBT_EXPORT_1507_112_2_18_2_7_50_128,"MMI_BT_A2DP_CONNECTED")
TRACE_MSG(MMIBT_EXPORT_1513_112_2_18_2_7_50_129,"MMI_BT_A2DP_ABORT_IND")
TRACE_MSG(MMIBT_EXPORT_1518_112_2_18_2_7_50_130,"MMI_BT_A2DP_CONNECTED_IND")
TRACE_MSG(MMIBT_EXPORT_1522_112_2_18_2_7_50_131,"MMI_BT_A2DP_DISCONNECTED")
TRACE_MSG(MMIBT_EXPORT_1530_112_2_18_2_7_50_132,"MMI_BT   default")
TRACE_MSG(MMIBT_EXPORT_1637_112_2_18_2_7_50_133,"MMIBT_CallBackFunc:restart_condition=%d")
TRACE_MSG(MMIBT_EXPORT_1655_112_2_18_2_7_50_134,"MMIBT_CallBackFunc, msg_id = %d")
TRACE_MSG(MMIBT_EXPORT_1660_112_2_18_2_7_50_135,"Bluetooth power on success.\n")
TRACE_MSG(MMIBT_EXPORT_1666_112_2_18_2_7_50_136,"Bluetooth power off success.\n")
TRACE_MSG(MMIBT_EXPORT_1672_112_2_18_2_7_50_137,"Bluetooth found 1 device: ")
TRACE_MSG(MMIBT_EXPORT_1678_112_2_18_2_7_50_138,"Bluetooth got the address and the name of the device.\n")
TRACE_MSG(MMIBT_EXPORT_1684_112_2_18_2_7_50_139,"Bluetooth inquiry completed\n")
TRACE_MSG(MMIBT_EXPORT_1690_112_2_18_2_7_50_140,"Bluetooth inquiry is cancelled.\n")
TRACE_MSG(MMIBT_EXPORT_1696_112_2_18_2_7_50_141,"Bluetooth set visible success.\n")
TRACE_MSG(MMIBT_EXPORT_1702_112_2_18_2_7_50_142,"Bluetooth set invisible success.\n")
TRACE_MSG(MMIBT_EXPORT_1708_112_2_18_2_7_50_143,"Bluetooth input pin request")
TRACE_MSG(MMIBT_EXPORT_1714_112_2_18_2_7_50_144,"Bluetooth pairing success.\n")
TRACE_MSG(MMIBT_EXPORT_1720_112_2_18_2_7_50_145,"Bluetooth earphone pairing success.\n")
TRACE_MSG(MMIBT_EXPORT_1725_112_2_18_2_7_50_146,"Bluetooth got delete paired device result.\n")
TRACE_MSG(MMIBT_EXPORT_1731_112_2_18_2_7_50_147,"Bluetooth read service result.\n")
TRACE_MSG(MMIBT_EXPORT_1737_112_2_18_2_7_50_148,"Bluetooth headset service connected.\n")
TRACE_MSG(MMIBT_EXPORT_1743_112_2_18_2_7_50_149,"IND Bluetooth headset service connected.\n")
TRACE_MSG(MMIBT_EXPORT_1748_112_2_18_2_7_50_150,"Bluetooth hand free service disconnected.\n")
TRACE_MSG(MMIBT_EXPORT_1754_112_2_18_2_7_50_151,"MMI_BT_PLAY_RESUNME_DEVICE_CNF.\n")
TRACE_MSG(MMIBT_EXPORT_1770_112_2_18_2_7_50_152,"MMI_BT_PAUSE_STOP_DEVICE_CNF.\n")
TRACE_MSG(MMIBT_EXPORT_1785_112_2_18_2_7_50_153,"Bluetooth ID_STATUS_HFAG_HFU_CALL_ANSWER.\n")
TRACE_MSG(MMIBT_EXPORT_1791_112_2_18_2_7_51_154,"Bluetooth ID_STATUS_HF_CALL1_HUNGUP.\n")
TRACE_MSG(MMIBT_EXPORT_1797_112_2_18_2_7_51_155,"Bluetooth ID_STATUS_HFAG_HFU_DIAL_NUMBER.\n")
TRACE_MSG(MMIBT_EXPORT_1803_112_2_18_2_7_51_156,"Bluetooth ID_STATUS_HFAG_HFU_DIAL_MEMORY.\n")
TRACE_MSG(MMIBT_EXPORT_1809_112_2_18_2_7_51_157,"Bluetooth ID_STATUS_HFAG_HFU_REDIAL.\n")
TRACE_MSG(MMIBT_EXPORT_1815_112_2_18_2_7_51_158,"Bluetooth ID_STATUS_HFAG_HFU_HOLD.\n")
TRACE_MSG(MMIBT_EXPORT_1820_112_2_18_2_7_51_159,"Bluetooth ID_STATUS_HFAG_HFU_DTMF_NUMBER.\n")
TRACE_MSG(MMIBT_EXPORT_1825_112_2_18_2_7_51_160,"Bluetooth ID_STATUS_HFAG_AUDIO_CONNECTED.\n")
TRACE_MSG(MMIBT_EXPORT_1831_112_2_18_2_7_51_161,"Bluetooth ID_STATUS_HFAG_AUDIO_DISCONNECTED.\n")
TRACE_MSG(MMIBT_EXPORT_1837_112_2_18_2_7_51_162,"Bluetooth ID_STATUS_HFAG_HFU_STATUS_REQ.\n")
TRACE_MSG(MMIBT_EXPORT_1843_112_2_18_2_7_51_163,"Bluetooth ID_STATUS_HS_MIC_VOL_CHANGE.\n")
TRACE_MSG(MMIBT_EXPORT_1849_112_2_18_2_7_51_164,"Bluetooth ID_STATUS_HS_SPK_VOL_CHANGE.\n")
TRACE_MSG(MMIBT_EXPORT_1855_112_2_18_2_7_51_165,"Bluetooth ID_STATUS_HFAG_HFU_CLCC.\n")
TRACE_MSG(MMIBT_EXPORT_1864_112_2_18_2_7_51_166,"Bluetooth Opc connect result.\n")
TRACE_MSG(MMIBT_EXPORT_1870_112_2_18_2_7_51_167,"Bluetooth Opc server abort ind.\n")
TRACE_MSG(MMIBT_EXPORT_1876_112_2_18_2_7_51_168,"Bluetooth Opc put abort ind.\n")
TRACE_MSG(MMIBT_EXPORT_1882_112_2_18_2_7_51_169,"Bluetooth ID_STATUS_OPC_PUT_OBJECT.\n")
TRACE_MSG(MMIBT_EXPORT_1888_112_2_18_2_7_51_170,"Bluetooth ID_STATUS_OPC_PUT_OBJECT.\n")
TRACE_MSG(MMIBT_EXPORT_1895_112_2_18_2_7_51_171,"Bluetooth ID_STATUS_OPC_DISCONNECT_IND")
TRACE_MSG(MMIBT_EXPORT_1904_112_2_18_2_7_51_172,"Bluetooth ID_STATUS_DUN_LINK_REQ.\n")
TRACE_MSG(MMIBT_EXPORT_1909_112_2_18_2_7_51_173,"Bluetooth ID_STATUS_DG_LINK_STATUS_RES.\n")
TRACE_MSG(MMIBT_EXPORT_1914_112_2_18_2_7_51_174,"Bluetooth ID_STATUS_DG_LINK_CLOSE_IND.\n")
TRACE_MSG(MMIBT_EXPORT_1921_112_2_18_2_7_51_175,"Bluetooth ID_STATUS_FTS_GET_REQ.\n")
TRACE_MSG(MMIBT_EXPORT_1928_112_2_18_2_7_51_176,"Bluetooth ID_STATUS_FTS_UPDATE_ROOT")
TRACE_MSG(MMIBT_EXPORT_1933_112_2_18_2_7_51_177,"Bluetooth ID_STATUS_FTC_CONNECT_RES.\n")
TRACE_MSG(MMIBT_EXPORT_1939_112_2_18_2_7_51_178,"Bluetooth ID_STATUS_FTC_SERVER_ABORT.\n")
TRACE_MSG(MMIBT_EXPORT_1945_112_2_18_2_7_51_179,"Bluetooth ID_STATUS_FTC_PUT_ABORT.\n")
TRACE_MSG(MMIBT_EXPORT_1951_112_2_18_2_7_51_180,"Bluetooth ID_STATUS_FTC_PUT_OBJECT.\n")
TRACE_MSG(MMIBT_EXPORT_1957_112_2_18_2_7_51_181,"Bluetooth ID_STATUS_FTC_PUT_FINAL.\n")
TRACE_MSG(MMIBT_EXPORT_1965_112_2_18_2_7_51_182,"Bluetooth ID_STATUS_OPC_PUT_OBJECT.\n")
TRACE_MSG(MMIBT_EXPORT_1971_112_2_18_2_7_51_183,"Bluetooth ID_STATUS_OPS_CLIENT_ABORT.\n")
TRACE_MSG(MMIBT_EXPORT_1977_112_2_18_2_7_51_184,"Bluetooth ID_STATUS_OPS_PUT_ABORT.\n")
TRACE_MSG(MMIBT_EXPORT_1983_112_2_18_2_7_51_185,"Bluetooth ID_STATUS_OPS_PUT_OBJECT.\n")
TRACE_MSG(MMIBT_EXPORT_1989_112_2_18_2_7_51_186,"Bluetooth ID_STATUS_OPS_PUT_FINAL.\n")
TRACE_MSG(MMIBT_EXPORT_1996_112_2_18_2_7_51_187,"Bluetooth ID_STATUS_OPS_DISCONNECT_IND.\n")
TRACE_MSG(MMIBT_EXPORT_2005_112_2_18_2_7_51_188,"Bluetooth ID_STATUS_OPC_PUT_OBJECT.\n")
TRACE_MSG(MMIBT_EXPORT_2011_112_2_18_2_7_51_189,"Bluetooth ID_STATUS_FTS_CONNECT_REQ.\n")
TRACE_MSG(MMIBT_EXPORT_2017_112_2_18_2_7_51_190,"Bluetooth ID_STATUS_OPS_CONNECT_REQ.\n")
TRACE_MSG(MMIBT_EXPORT_2023_112_2_18_2_7_51_191,"Bluetooth ID_STATUS_FTS_CLIENT_ABORT.\n")
TRACE_MSG(MMIBT_EXPORT_2029_112_2_18_2_7_51_192,"Bluetooth ID_STATUS_FTS_PUT_ABORT.\n")
TRACE_MSG(MMIBT_EXPORT_2035_112_2_18_2_7_51_193,"Bluetooth ID_STATUS_FTS_PUT_OBJECT.\n")
TRACE_MSG(MMIBT_EXPORT_2041_112_2_18_2_7_51_194,"Bluetooth ID_STATUS_FTS_PUT_FINAL.\n")
TRACE_MSG(MMIBT_EXPORT_2046_112_2_18_2_7_51_195,"Bluetooth ID_STATUS_FTS_GET_ABORT.\n")
TRACE_MSG(MMIBT_EXPORT_2051_112_2_18_2_7_51_196,"Bluetooth ID_STATUS_FTS_GET_FINAL.\n")
TRACE_MSG(MMIBT_EXPORT_2056_112_2_18_2_7_51_197,"Bluetooth ID_STATUS_FTS_GET_OBJECT.\n")
TRACE_MSG(MMIBT_EXPORT_2062_112_2_18_2_7_51_198,"Bluetooth ID_STATUS_FTS_DEL_REQ.\n")
TRACE_MSG(MMIBT_EXPORT_2067_112_2_18_2_7_51_199,"Bluetooth ID_STATUS_FTS_DEL_BUSY.\n")
TRACE_MSG(MMIBT_EXPORT_2072_112_2_18_2_7_51_200,"Bluetooth ID_STATUS_FTS_ADD_FOLDER_REQ.\n")
TRACE_MSG(MMIBT_EXPORT_2079_112_2_18_2_7_51_201,"Bluetooth ID_STATUS_FTS_DISCONNECT_IND.\n")
TRACE_MSG(MMIBT_EXPORT_2086_112_2_18_2_7_51_202,"Bluetooth signal unsolvable 0x%x.\n")
TRACE_MSG(MMIBT_EXPORT_2113_112_2_18_2_7_51_203,"MMIBT_A2DPCallBackFunc:restart_condition=%d")
TRACE_MSG(MMIBT_EXPORT_2131_112_2_18_2_7_51_204,"MMIBT_A2DPCallBackFunc: pMsg->msg_id = %d")
TRACE_MSG(MMIBT_EXPORT_2137_112_2_18_2_7_51_205,"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_BACKWARD.\n")
TRACE_MSG(MMIBT_EXPORT_2143_112_2_18_2_7_51_206,"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_FORWARD.\n")
TRACE_MSG(MMIBT_EXPORT_2149_112_2_18_2_7_51_207,"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_VOL_UP.\n")
TRACE_MSG(MMIBT_EXPORT_2155_112_2_18_2_7_51_208,"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_VOL_DOWN.\n")
TRACE_MSG(MMIBT_EXPORT_2161_112_2_18_2_7_51_209,"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_PLAY.\n")
TRACE_MSG(MMIBT_EXPORT_2167_112_2_18_2_7_51_210,"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_STOP.\n")
TRACE_MSG(MMIBT_EXPORT_2173_112_2_18_2_7_51_211,"Bluetooth ID_STATUS_AVRCP_OPERATION_ID_PAUSE.\n")
TRACE_MSG(MMIBT_EXPORT_2179_112_2_18_2_7_51_212,"Bluetooth ID_STATUS_AVRCP_CONNECT_COMPLETE.\n")
TRACE_MSG(MMIBT_EXPORT_2185_112_2_18_2_7_51_213,"Bluetooth ID_STATUS_AVRCP_DISCONNECT_COMPLETE.\n")
TRACE_MSG(MMIBT_EXPORT_2193_112_2_18_2_7_51_214,"bluetooth A2DP SRC config strem")
TRACE_MSG(MMIBT_EXPORT_2199_112_2_18_2_7_51_215,"bluetooth A2DP SRC open strem")
TRACE_MSG(MMIBT_EXPORT_2203_112_2_18_2_7_51_216,"Bluetooth ID_STATUS_A2DP_SRC_START_STREAM.\n")
TRACE_MSG(MMIBT_EXPORT_2209_112_2_18_2_7_51_217,"Bluetooth ID_STATUS_A2DP_SRC_RESUME_STREAM.\n")
TRACE_MSG(MMIBT_EXPORT_2215_112_2_18_2_7_51_218,"Bluetooth ID_STATUS_A2DP_SRC_SUSPEND_STREAM.\n")
TRACE_MSG(MMIBT_EXPORT_2221_112_2_18_2_7_51_219,"Bluetooth ID_STATUS_A2DP_SRC_CLOSE_STREAM.\n")
TRACE_MSG(MMIBT_EXPORT_2227_112_2_18_2_7_51_220,"Bluetooth ID_STATUS_A2DP_CONNECTED.\n")
TRACE_MSG(MMIBT_EXPORT_2233_112_2_18_2_7_51_221,"IND Bluetooth ID_STATUS_A2DP_CONNECTED.\n")
TRACE_MSG(MMIBT_EXPORT_2239_112_2_18_2_7_51_222,"Bluetooth ID_STATUS_A2DP_DISCONNECTED.\n")
TRACE_MSG(MMIBT_EXPORT_2245_112_2_18_2_7_51_223,"Bluetooth ID_STATUS_A2DP_SNK_START_STREAM.\n")
TRACE_MSG(MMIBT_EXPORT_2251_112_2_18_2_7_51_224,"Bluetooth ID_STATUS_A2DP_SNK_SUSPEND_STREAM.\n")
TRACE_MSG(MMIBT_EXPORT_2257_112_2_18_2_7_51_225,"Bluetooth ID_STATUS_A2DP_ABORT_CFM.\n")
TRACE_MSG(MMIBT_EXPORT_2263_112_2_18_2_7_51_226,"Bluetooth ID_STATUS_A2DP_ABORT_IND.\n")
TRACE_MSG(MMIBT_EXPORT_2269_112_2_18_2_7_51_227,"Bluetooth ID_STATUS_A2DP_SNK_RESUME_STREAM.\n")
TRACE_MSG(MMIBT_EXPORT_2276_112_2_18_2_7_52_228,"Bluetooth signal unsolvable 0x%x.\n")
TRACE_MSG(MMIBT_EXPORT_2301_112_2_18_2_7_52_229,"MMIAPIBT_GetAddress PNULL == addr_ptr!")
TRACE_MSG(MMIBT_EXPORT_2610_112_2_18_2_7_52_230,"MMIAPIBT_SetOperatorName name <%s>")
TRACE_MSG(MMIBT_EXPORT_2627_112_2_18_2_7_52_231,"MMIAPIBT_SetCallInfo idx %d dir %d call_status %d mode %d mpty %d")
TRACE_MSG(MMIBT_EXPORT_2628_112_2_18_2_7_52_232,"MMIAPIBT_SetCallInfo number  %s")
TRACE_MSG(MMIBT_EXPORT_2644_112_2_18_2_7_52_233,"MMIAPIBT_SetNetWorkRoamStatus roam %d")
TRACE_MSG(MMIBT_EXPORT_2671_112_2_18_2_7_52_234,"MMIAPIBT_SetMyPhoneNumber num_buf %s")
TRACE_MSG(MMIBT_EXPORT_2682_112_2_18_2_7_52_235,"MMIAPIBT_SetCallSuccess ")
TRACE_MSG(MMIBT_EXPORT_2694_112_2_18_2_7_52_236,"MMIAPIBT_SetCallFail ")
TRACE_MSG(MMIBT_EXPORT_2708_112_2_18_2_7_52_237,"MMIAPIBT_Service_Indicator cmd_value %d is_in_net %d")
TRACE_MSG(MMIBT_WINTAB_1606_112_2_18_2_8_2_238,"MMIBT_UpdateFtpSharedFolder PNULL == root_folder_ptr!")
TRACE_MSG(MMIBT_WINTAB_1633_112_2_18_2_8_2_239,"MMIBT_IsDeviceActivedEarphone PNULL == addr_ptr!")
TRACE_MSG(MMIBT_WINTAB_1638_112_2_18_2_8_2_240," MMIBT_IsDeviceActivedEarphone: return value is %d")
TRACE_MSG(MMIBT_WINTAB_3882_112_2_18_2_8_8_241,"MMIBT_SendNextFile PNULL == filepath_name!")
TRACE_MSG(MMIBT_WINTAB_3885_112_2_18_2_8_8_242,"[&&&]MMIBT_SendNextFile start,file_size = %d")
TRACE_MSG(MMIBT_WINTAB_3917_112_2_18_2_8_8_243,"MMIBT_SendFile PNULL == filepath_name!")
TRACE_MSG(MMIBT_WINTAB_4000_112_2_18_2_8_8_244,"SetIsDelWallpaperFile: Set del file is wallpaper = %d")
TRACE_MSG(MMIBT_WINTAB_4010_112_2_18_2_8_8_245,"GetIsDelWallpaperFile: Get file is wallpaper = %d")
TRACE_MSG(MMIBT_WINTAB_4030_112_2_18_2_8_8_246,"MMIBT_FtsDelReq PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_4130_112_2_18_2_8_8_247,"MMIBT_ConnectDeviceResCnf PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_4161_112_2_18_2_8_8_248,"[&&&]MMIBT_ConnectDeviceResCnf")
TRACE_MSG(MMIBT_WINTAB_4396_112_2_18_2_8_9_249,"[&&&]HandleFileTransferWindow Status = %d")
TRACE_MSG(MMIBT_WINTAB_4464_112_2_18_2_8_9_250,"[&&&]HandleSendConnectWindow")
TRACE_MSG(MMIBT_WINTAB_4542_112_2_18_2_8_9_251,"AssignReceiveFileCfg service_type %d ")
TRACE_MSG(MMIBT_WINTAB_4763_112_2_18_2_8_9_252,"MMIBT_DeleteDeviceCnf PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_4766_112_2_18_2_8_9_253,"MMIBT_DeleteDeviceCnf: The action result is %d")
TRACE_MSG(MMIBT_WINTAB_4974_112_2_18_2_8_10_254,"[MMIBT] DeletePairedDevice, close MMIBT_DELETE_DEVICE_WAIT_WIN_ID")
TRACE_MSG(MMIBT_WINTAB_4981_112_2_18_2_8_10_255,"[MMIBT] DeletePairedDevice is_all %d delete_result %d is_deleted %d")
TRACE_MSG(MMIBT_WINTAB_5042_112_2_18_2_8_10_256,"[&&&]s_bt_file_incoming = %d")
TRACE_MSG(MMIBT_WINTAB_5405_112_2_18_2_8_11_257,"MMIBT_GetFtpReq PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_5454_112_2_18_2_8_11_258,"MMIBT_GetInputPinReq PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_5470_112_2_18_2_8_11_259,"MMIBT_GetInputPinReq in pairing !!")
TRACE_MSG(MMIBT_WINTAB_5479_112_2_18_2_8_11_260,"MMIBT_GetInputPinReq in repairing !!")
TRACE_MSG(MMIBT_WINTAB_5487_112_2_18_2_8_11_261,"MMIBT_GetInputPinReq in Cacelling !!")
TRACE_MSG(MMIBT_WINTAB_5565_112_2_18_2_8_11_262,"MMIBT_OpenIncomingFileWin PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_5576_112_2_18_2_8_11_263,"MMIBT_OpenIncomingFileWin:name_len is %d")
TRACE_MSG(MMIBT_WINTAB_5692_112_2_18_2_8_11_264,"MMIBT_GetBtOnOffCnf status %d current_opertion %d")
TRACE_MSG(MMIBT_WINTAB_5854_112_2_18_2_8_11_265,"MMIBT_GetVisibleCnf PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_5939_112_2_18_2_8_12_266,"MMIBT_GetInqDevName in calling or alarming !")
TRACE_MSG(MMIBT_WINTAB_5947_112_2_18_2_8_12_267,"MMIBT_GetInqDevName status %d !")
TRACE_MSG(MMIBT_WINTAB_5952_112_2_18_2_8_12_268,"MMIBT_GetInqDevName alerady exist s_inquired_device_num %d !")
TRACE_MSG(MMIBT_WINTAB_6084_112_2_18_2_8_12_269,"MMIBT_GetPairComCnf msg_body_ptr->status %d")
TRACE_MSG(MMIBT_WINTAB_6264_112_2_18_2_8_13_270,"MMIBT_OpenHeadsetCnf PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_6281_112_2_18_2_8_13_271,"MMIBT_OpenHeadsetCnf address = %x, %x, %x, %x, %x, %x")
TRACE_MSG(MMIBT_WINTAB_6286_112_2_18_2_8_13_272,"MMIBT_OpenHeadsetCnf msg_body_ptr->status %d")
TRACE_MSG(MMIBT_WINTAB_6324_112_2_18_2_8_13_273,"MMIBT MMIBT_OpenHeadsetCnf enter, TXT_BT_HEADSET_OPENED")
TRACE_MSG(MMIBT_WINTAB_6371_112_2_18_2_8_13_274,"MMIBT_CloseHeadsetCnf PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_6374_112_2_18_2_8_13_275,"MMIBT_CloseHeadsetCnf is_a2dp %d msg_body_ptr->status %d")
TRACE_MSG(MMIBT_WINTAB_6387_112_2_18_2_8_13_276,"MMIBT_CloseHeadsetCnf address = %x, %x, %x, %x, %x, %x")
TRACE_MSG(MMIBT_WINTAB_6549_112_2_18_2_8_13_277,"MMIBT_HfagDiaMemory index=%d, memory_indicat=%d")
TRACE_MSG(MMIBT_WINTAB_6625_112_2_18_2_8_13_278,"MMIBT_AudioConnected PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_6655_112_2_18_2_8_13_279,"MMIBT_AudioDisconnected PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_6786_112_2_18_2_8_14_280,"MMIBT_GetPacketReceivedInd: the received data is = %d")
TRACE_MSG(MMIBT_WINTAB_6797_112_2_18_2_8_14_281,"MMIBT_GetPacketSentInd s_transfer_file_info.transfer_file_size %d transfer_info.transmitted_count %d")
TRACE_MSG(MMIBT_WINTAB_6824_112_2_18_2_8_14_282,"MMIBT_GetPacketReceivedInd: the received data is = %d")
TRACE_MSG(MMIBT_WINTAB_6903_112_2_18_2_8_14_283,"MMIBT_SendFileResInd PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_6948_112_2_18_2_8_14_284,"[&&&]MMIBT_SendFileFailedInd")
TRACE_MSG(MMIBT_WINTAB_7007_112_2_18_2_8_14_285,"MMIBT_ReceiveFileResInd PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_7067_112_2_18_2_8_14_286,"MMIBT_ReceiveFileFail s_transfer_file_info.server_type %d")
TRACE_MSG(MMIBT_WINTAB_7151_112_2_18_2_8_14_287,"[&&&]DoOperation start,MMIBT_CancelFileTransfer")
TRACE_MSG(MMIBT_WINTAB_7185_112_2_18_2_8_14_288,"MMIBT_CancelSearchCnf  status is = %d")
TRACE_MSG(MMIBT_WINTAB_7254_112_2_18_2_8_15_289,"MMIBT_GetIsDeviceVisible: visible status is = %d")
TRACE_MSG(MMIBT_WINTAB_7573_112_2_18_2_8_15_290,"MMIBT_GetPairedDevList PNULL == paired_dev_ptr!")
TRACE_MSG(MMIBT_WINTAB_7774_112_2_18_2_8_16_291,"SetFileSaveLocation: location info is = %d, return_value is = %d ")
TRACE_MSG(MMIBT_WINTAB_7816_112_2_18_2_8_16_292,"MMIBT_IsPhoneReady is_ready %d")
TRACE_MSG(MMIBT_WINTAB_7852_112_2_18_2_8_16_293,"[MMIBT] MMIBT_GetOpertor root_opt %d, current_opt %d")
TRACE_MSG(MMIBT_WINTAB_7889_112_2_18_2_8_16_294,"CreateListMenu window already opened !")
TRACE_MSG(MMIBT_WINTAB_7999_112_2_18_2_8_16_295,"[MMIBT]MMIBT_OpenBluetooth set_on %d ret_val %d")
TRACE_MSG(MMIBT_WINTAB_8078_112_2_18_2_8_16_296,"bt is_file_type = %d")
TRACE_MSG(MMIBT_WINTAB_8087_112_2_18_2_8_16_297,"bt file_handle = %d")
TRACE_MSG(MMIBT_WINTAB_8097_112_2_18_2_8_16_298,"bt SFS_ERROR_E = %d")
TRACE_MSG(MMIBT_WINTAB_8123_112_2_18_2_8_16_299,"[MMIBT] HandleQueryWinMsg win_id 0x%x, msg_id 0x%x s_current_query_num %d")
TRACE_MSG(MMIBT_WINTAB_8162_112_2_18_2_8_16_300,"OpenQueryWin MSG_APP_OK current_opc %d ")
TRACE_MSG(MMIBT_WINTAB_8175_112_2_18_2_8_16_301,"MMIBT file name %s")
TRACE_MSG(MMIBT_WINTAB_8285_112_2_18_2_8_17_302,"DoOperation current_opt %d is_poweron_first %d")
TRACE_MSG(MMIBT_WINTAB_8288_112_2_18_2_8_17_303,"DoOperation in calling or alarming ! current_opt %d be cancelled")
TRACE_MSG(MMIBT_WINTAB_8372_112_2_18_2_8_17_304,"[&&&]DoOperation start,OPC_OPP_SEND")
TRACE_MSG(MMIBT_WINTAB_8551_112_2_18_2_8_17_305,"[&&&]DoOperation start,CancelOperation MMIBT_OPC_OPP_SEND")
TRACE_MSG(MMIBT_WINTAB_8621_112_2_18_2_8_18_306,"CreateList window already opened !")
TRACE_MSG(MMIBT_WINTAB_8902_112_2_18_2_8_18_307,"[MMIBT]BtPauseBGPlay: TYPE %d ")
TRACE_MSG(MMIBT_WINTAB_8919_112_2_18_2_8_18_308,"BtResumeBGPlay type %d")
TRACE_MSG(MMIBT_WINTAB_8943_112_2_18_2_8_18_309,"[MMIBT]BtResumeBGPlay: TYPE %d ret %d exist %d")
TRACE_MSG(MMIBT_WINTAB_8961_112_2_18_2_8_18_310,"DeviceIsPaired PNULL == addr!")
TRACE_MSG(MMIBT_WINTAB_8976_112_2_18_2_8_18_311,"mmibt.c: DeviceIsPaired device number is %d ret %d")
TRACE_MSG(MMIBT_WINTAB_9016_112_2_18_2_8_18_312,"MMIBT_DeviceIsConnectedHeadset ret %d")
TRACE_MSG(MMIBT_WINTAB_9039_112_2_18_2_8_18_313,"BT_FTSGetState ftp_status %d")
TRACE_MSG(MMIBT_WINTAB_9046_112_2_18_2_8_18_314,"DeviceIsConnectedFTP ret %d")
TRACE_MSG(MMIBT_WINTAB_9077_112_2_18_2_8_18_315,"MMIBT_OpenedHeadsetIsExist exist %d")
TRACE_MSG(MMIBT_WINTAB_9149_112_2_18_2_8_19_316,"HeadsetIsClosed ret %d")
TRACE_MSG(MMIBT_WINTAB_9445_112_2_18_2_8_19_317,"GetDeviceListIcon PNULL == dev!")
TRACE_MSG(MMIBT_WINTAB_9449_112_2_18_2_8_19_318,"BT_GetDeviceType dev_type %d")
TRACE_MSG(MMIBT_WINTAB_9500_112_2_18_2_8_19_319,"MMIBT_OpenFileCallBack, Enter,is_success = %d!")
TRACE_MSG(MMIBT_WINTAB_9504_112_2_18_2_8_19_320,"MMIBT_OpenFileCallBack, file_num = %d")
TRACE_MSG(MMIBT_WINTAB_9631_112_2_18_2_8_20_321,"GetFileInfo alloc fail !")
TRACE_MSG(MMIBT_WINTAB_9731_112_2_18_2_8_20_322,"MMIBT_OpenDUNWin fail PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_9734_112_2_18_2_8_20_323,"MMIBT_OpenDUNWin msg_body_ptr->status %d")
TRACE_MSG(MMIBT_WINTAB_9754_112_2_18_2_8_20_324,"UART selected SIO_UserPortBusy")
TRACE_MSG(MMIBT_WINTAB_9763_112_2_18_2_8_20_325,"UART selected")
TRACE_MSG(MMIBT_WINTAB_9771_112_2_18_2_8_20_326,"MMIAPIPHONE_GetSimStatus not ready")
TRACE_MSG(MMIBT_WINTAB_9776_112_2_18_2_8_20_327,"MMIAPIBT_OpenDUNWin error")
TRACE_MSG(MMIBT_WINTAB_9794_112_2_18_2_8_20_328,"MMIBT_DUNOpenedInd PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_9797_112_2_18_2_8_20_329,"MMIBT_DUNOpenedInd msg_body_ptr->status %d")
TRACE_MSG(MMIBT_WINTAB_9837_112_2_18_2_8_20_330,"MMIBT_DUNClosedInd PNULL == msg_body_ptr!")
TRACE_MSG(MMIBT_WINTAB_9840_112_2_18_2_8_20_331,"MMIBT_DUNClosedInd msg_body_ptr->status %d")
TRACE_MSG(MMIBT_WINTAB_9888_112_2_18_2_8_20_332,"HandleConnectDUNOrNotMsg Cancel")
TRACE_MSG(MMIBT_WINTAB_9900_112_2_18_2_8_20_333,"HandleConnectDUNOrNotMsg Accept")
TRACE_MSG(MMIBT_WINTAB_10081_112_2_18_2_8_21_334,"UpdateCurrentFileList enter")
TRACE_MSG(MMIBT_WINTAB_10130_112_2_18_2_8_21_335,"UpdateCurrentPictureViewList")
TRACE_MSG(MMIBT_WINTAB_10155_112_2_18_2_8_21_336,"MMIBT_Reset enter")
TRACE_MSG(MMIBT_WINTAB_10227_112_2_18_2_8_21_337,"Create_TextLabel window already opened !")
TRACE_MSG(MMIBT_WINTAB_10341_112_2_18_2_8_21_338,"Get_Transfer_DeviceName string invalid !")
TRACE_MSG(MMIBT_WINTAB_10357_112_2_18_2_8_21_339,"file_name string invalid !")
TRACE_MSG(MMIBT_WINTAB_10550_112_2_18_2_8_22_340,"MMIBT_OPC_SAVE_LOCATION cur_select_item %d")
TRACE_MSG(MMIBT_WINTAB_10608_112_2_18_2_8_22_341,"Is_PermitOpenFile result %d s_previous_win_id 0x%x ")
TRACE_MSG(MMIBT_WINTAB_10694_112_2_18_2_8_22_342,"CloseHeadset enter")
TRACE_MSG(MMIBT_WINTAB_10737_112_2_18_2_8_22_343,"OpenHeadset enter")
TRACE_MSG(MMIBT_WINTAB_10745_112_2_18_2_8_22_344,"OpenHeadset address = %x, %x, %x, %x, %x, %x")
TRACE_MSG(MMIBT_WINTAB_10746_112_2_18_2_8_22_345,"OpenHeadset src_addr = %x, %x, %x, %x, %x, %x")
TRACE_MSG(MMIBT_WINTAB_10769_112_2_18_2_8_22_346,"MMIBT LOCAL void OpenHeadset enter, TXT_BT_HEADSET_OPENED")
TRACE_MSG(MMIBT_WINTAB_10854_112_2_18_2_8_22_347,"HeadsetOpenIsPermit in calling or alarming !")
TRACE_MSG(MMIBT_WINTAB_10865_112_2_18_2_8_22_348,"HeadsetOpenIsPermit exist alrady opened headset !")
TRACE_MSG(MMIBT_WINTAB_10875_112_2_18_2_8_22_349,"HeadsetOpenIsPermit in call !")
TRACE_MSG(MMIBT_WINTAB_10887_112_2_18_2_8_22_350,"HeadsetOpenIsPermit mp3 pause or playing !")
TRACE_MSG(MMIBT_WINTAB_10893_112_2_18_2_8_22_351,"HeadsetOpenIsPermit kuro pause or playing  !")
TRACE_MSG(MMIBT_WINTAB_10900_112_2_18_2_8_22_352,"HeadsetOpenIsPermit ok")
TRACE_MSG(MMIBT_WINTAB_11013_112_2_18_2_8_22_353,"MMIBT_GetPairComIND msg_body_ptr->status %d")
TRACE_MSG(MMIBT_WINTAB_11086_112_2_18_2_8_23_354,"MMIBT_OpenHeadsetConfirmIND msg_body_ptr is null")
TRACE_MSG(MMIBT_WINTAB_11101_112_2_18_2_8_23_355,"MMIBT_OpenHeadsetConfirmIND address = %x, %x, %x, %x, %x, %x")
TRACE_MSG(MMIBT_WINTAB_11106_112_2_18_2_8_23_356,"MMIBT_OpenHeadsetConfirmIND msg_body_ptr->status %d")
TRACE_MSG(MMIBT_WINTAB_11122_112_2_18_2_8_23_357,"MMIBT MMIBT_OpenHeadsetConfirmIND enter, TXT_BT_HEADSET_OPENED")
TRACE_MSG(MMIBT_WINTAB_11160_112_2_18_2_8_23_358,"MMIBT_ClccInd msg_body %d")
TRACE_MSG(MMIBT_WINTAB_11237_112_2_18_2_8_23_359,"mmibt_wintab.c: start to install drm!")
TRACE_MSG(MMIBT_WINTAB_11243_112_2_18_2_8_23_360,"mmibt_wintab.c: file not exist!")
TRACE_MSG(MMIBT_WINTAB_11309_112_2_18_2_8_23_361,"MMIBT_OpenFMMDirectory: result =%d")
TRACE_MSG(MMIBT_WINTAB_11325_112_2_18_2_8_23_362,"MMIBT_SetFileLocationCallBack, Enter,is_success = %d!")
TRACE_MSG(MMIBT_WINTAB_11329_112_2_18_2_8_23_363,"MMIBT_SetFileLocationCallBack, file_num = %d")
TRACE_MSG(MMIBT_WINTAB_11475_112_2_18_2_8_23_364,"[&&&]MMIBT_StopBTTransferFile")
END_TRACE_MAP(MMI_APP_BT_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_BT_TRC_H_


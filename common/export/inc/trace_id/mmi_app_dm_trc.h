/******************************************************************************
 ** File Name:      mmi_app_dm_trc.h                                         *
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
//trace_id:167
#ifndef _MMI_APP_DM_TRC_H_
#define _MMI_APP_DM_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIDM_APP_458_112_2_18_2_13_45_0 "MMIDM==> IsDmWapPush content[4]= 0x%x,content[5]= 0x%x,content[6]= 0x%x"
#define MMIDM_APP_468_112_2_18_2_13_45_1 "MMIDM==>IsDmWapPush sms[%d] = %x"
#define MMIDM_APP_476_112_2_18_2_13_45_2 "MMIDM==>IsDmWapPush session_id buf %s"
#define MMIDM_APP_481_112_2_18_2_13_45_3 "MMIDM==>IsDmWapPush alert_id  %d"
#define MMIDM_APP_511_112_2_18_2_13_45_4 "MMIDM==> MMIAPIDM_HandleSmsMtInd:"
#define MMIDM_APP_520_112_2_18_2_13_45_5 "MMIDM==> reply of self register sms,and write IMSI to NV success!"
#define MMIDM_APP_526_112_2_18_2_13_45_6 "MMIDM==> sms_is_wappush = %d"
#define MMIDM_APP_547_112_2_18_2_13_45_7 "MMIDM==> data[5]=0x%x,data[6]=0x%x,data[7]=0x%x"
#define MMIDM_APP_549_112_2_18_2_13_45_8 "MMIDM==> Start signal dm_ is_start len = %d"
#define MMIDM_APP_566_112_2_18_2_13_45_9 "MMIDM==> Start signal dm status = %d"
#define MMIDM_APP_626_112_2_18_2_13_46_10 "MMIDM==> DevRecvSRHandle: msg = %s"
#define MMIDM_APP_631_112_2_18_2_13_46_11 "MMIDM==> DevRecvSRHandle server_id=%s\n"
#define MMIDM_APP_639_112_2_18_2_13_46_12 "MMIDM==> DevRecvSRHandle---wrong SMS text received\n"
#define MMIDM_APP_665_112_2_18_2_13_46_13 "MMIDM==> ----------GetDmServerSmscNumber  dmSrvID= %s---------\n"
#define MMIDM_APP_668_112_2_18_2_13_46_14 "MMIDM==> ----------GetDmServerSmscNumber  dmSrvID= %s---------\n"
#define MMIDM_APP_692_112_2_18_2_13_46_15 "MMIDM==> --DevRecvSelfRegisterSMS--SRSMS=%s, SRSMSOK=%s--\n,SRSMSERROR=%s--\n"
#define MMIDM_APP_704_112_2_18_2_13_46_16 "MMIDM==> ---------the device register ok--------\n"
#define MMIDM_APP_713_112_2_18_2_13_46_17 "MMIDM==> ----------failed to register the device---\n"
#define MMIDM_APP_801_112_2_18_2_13_46_18 "MMIDM==> get is sr ready = %d"
#define MMIDM_APP_811_112_2_18_2_13_46_19 "MMIDM==> set is sr ready = %d"
#define MMIDM_APP_822_112_2_18_2_13_46_20 "MMIDM==> get test server type = %d"
#define MMIDM_APP_832_112_2_18_2_13_46_21 "MMIDM==> set test server type = %d"
#define MMIDM_APP_855_112_2_18_2_13_46_22 "MMIDM==> MMIDM_SendSelfRegisterSMS : sms=%d,net=%d"
#define MMIDM_APP_859_112_2_18_2_13_46_23 "MMIDM==> -------MMIDM_SendSelfRegisterSMS--------\n"
#define MMIDM_APP_862_112_2_18_2_13_46_24 "MMIDM==> --------SIM CARD NOT exist-------\n"
#define MMIDM_APP_867_112_2_18_2_13_46_25 "MMIDM==> --------SIM CARD NOT ready-------\n"
#define MMIDM_APP_872_112_2_18_2_13_46_26 "MMIDM==> --------NOT CMCC SIM CARD--------\n"
#define MMIDM_APP_885_112_2_18_2_13_46_27 "MMIDM==>--------the device have registered--------\n"
#define MMIDM_APP_891_112_2_18_2_13_46_28 "MMIDM==>--------the device have registered fail-----\n"
#define MMIDM_APP_902_112_2_18_2_13_46_29 "MMIDM==> ----------SRSMS devIMEI = %s---------\n"
#define MMIDM_APP_924_112_2_18_2_13_46_30 "MMIDM==> MMIDM_SendSelfRegisterSMS fail!"
#define MMIDM_APP_960_112_2_18_2_13_46_31 "MMIDM==> ----------SendSelfRegisterSms ---------\n"
#define MMIDM_APP_963_112_2_18_2_13_46_32 "MMIDM==>SR_Msg = %s \n"
#define MMIDM_APP_986_112_2_18_2_13_46_33 "MMIDM==> SendSelfRegisterSms length = %d \n"
#define MMIDM_APP_992_112_2_18_2_13_46_34 "MMIDM==> MMISMS_AppSendSmsForDm reg_times = %d send result =%d\n"
#define MMIDM_APP_1044_112_2_18_2_13_46_35 "MMIDM==>  in Param_init() enter"
#define MMIDM_APP_1133_112_2_18_2_13_47_36 "MMIDM==> in Param_init() enter lock=%d"
#define MMIDM_APP_1222_112_2_18_2_13_47_37 "MMIDM==> DM_ParamInit  alert_type %d"
#define MMIDM_APP_1223_112_2_18_2_13_47_38 "MMIDM==>  in devParam_init() end"
#define MMIDM_APP_1240_112_2_18_2_13_47_39 "MMIDM==> s_dm_debug_info.debug_mode = %d"
#define MMIDM_APP_1250_112_2_18_2_13_47_40 "MMIDM==> s_dm_debug_info.reg_times = %d"
#define MMIDM_APP_1271_112_2_18_2_13_47_41 "MMIDM==> s_dm_debug_info.dm_reg_switch = %d"
#define MMIDM_APP_1284_112_2_18_2_13_47_42 "MMIDM==>--enter into GetServePortNumber!!---\n"
#define MMIDM_APP_1334_112_2_18_2_13_47_43 "MMIDM==>--------DM_MVA IsNeedSelfRegister--------\n"
#define MMIDM_APP_1344_112_2_18_2_13_47_44 "MMIDM==>: sr switch on"
#define MMIDM_APP_1349_112_2_18_2_13_47_45 "MMIDM==>: sr switch off"
#define MMIDM_APP_1361_112_2_18_2_13_47_46 "MMIDM==> MMIDM_GetMan %s"
#define MMIDM_APP_1371_112_2_18_2_13_47_47 "MMIDM==> MMIDM_GetModel %s"
#define MMIDM_APP_1391_112_2_18_2_13_47_48 "MMIDM==> version %s"
#define MMIDM_APP_1562_112_2_18_2_13_48_49 "MMIDM==> MMIDM_IsDmRun = %d"
#define MMIDM_APP_1573_112_2_18_2_13_48_50 "MMIDM==> get dm state = %d"
#define MMIDM_APP_1584_112_2_18_2_13_48_51 "MMIDM==> set dm state = %d"
#define MMIDM_APP_1595_112_2_18_2_13_48_52 "MMIDM==> get dm session type = %d"
#define MMIDM_APP_1614_112_2_18_2_13_48_53 "MMIDM==> MMIDM_SendSigToDmTask task_id=0"
#define MMIDM_APP_1621_112_2_18_2_13_48_54 "MMIDM==> PNULL == signal_ptr"
#define MMIDM_APP_1638_112_2_18_2_13_48_55 "MMIDM==> PNULL=signal_ptr->content.msg_body"
#define MMIDM_APP_1649_112_2_18_2_13_48_56 "MMIDM==> SendSigToDmTask! event id =%d"
#define MMIDM_APP_1701_112_2_18_2_13_48_57 "MMIDM==> MMIDM_ReStartDmSession s_dm_need_restart = %d"
#define MMIDM_APP_1746_112_2_18_2_13_48_58 "MMIDM==> MMIDM_ReStartDmSession status = %d"
#define MMIDM_APP_1759_112_2_18_2_13_48_59 "MMIDM==> MMIDM_ReSetDmPushInfo "
#define MMIDM_APP_1771_112_2_18_2_13_48_60 "MMIDM==> MMIDM_HandDmRxChange  rx_level=%d s_dm_need_restart=%d"
#define MMIDM_APP_1806_112_2_18_2_13_48_61 "MMIDM==> MMIDM_SetIsNeedRestart  is_need_restart=%d"
#define MMIDM_APP_1816_112_2_18_2_13_48_62 "MMIDM==> MMIDM_GetIsNeedRestart  s_dm_need_restart=%d"
#define MMIDM_APP_1856_112_2_18_2_13_48_63 "MMIDM==> CreateDmTreeXmlFile MMIAPIFMM_DeleteFile error=%d"
#define MMIDM_APP_1877_112_2_18_2_13_48_64 "MMIDM==> tree.xml WriteFile succ!"
#define MMIDM_APP_1882_112_2_18_2_13_48_65 "MMIDM==> tree.xml WriteFile fail!"
#define MMIDM_APP_1973_112_2_18_2_13_49_66 "MMIDM==> s_dm_info.is_download_finish = %d"
#define MMIDM_APP_1992_112_2_18_2_13_49_67 "MMIDM==> s_dm_info.handset_lock = %d"
#define MMIDM_APP_2010_112_2_18_2_13_49_68 "MMIDM==> s_dm_debug_info.log_switch = %d"
#define MMIDM_APP_2028_112_2_18_2_13_49_69 "MMIDM==> s_dm_debug_info.fota_switch = %d"
#define MMIDM_APP_2045_112_2_18_2_13_49_70 "MMIDM==> s_dm_debug_info.tree_switch = %d"
#define MMIDM_APP_2055_112_2_18_2_13_49_71 "MMIDM==> idasdec_type = %d"
#define MMIDM_APP_2123_112_2_18_2_13_49_72 "MMIDM==>MMIDM_GetDmProfile"
#define MMIDM_APP_2148_112_2_18_2_13_49_73 "MMIDM==>MMIDM_SetDmProfile length = %d result=%d"
#define MMIDM_APP_2166_112_2_18_2_13_49_74 "MMIDM==> MMIDM_GetConnectInfo max_num = %d mmi_cmnet_connect_index =%d"
#define MMIDM_APP_2190_112_2_18_2_13_49_75 "MMIDM==>MMIDM_GetGPRSInfo type = %d con_type=%d"
#define MMIDM_APP_2194_112_2_18_2_13_49_76 "MMIDM==>MMIDM_GetGPRSInfo type PNULL == connect_datail_ptr"
#define MMIDM_APP_2242_112_2_18_2_13_49_77 "MMIDM==> MMIDM_GetGPRSInfo type=%d"
#define MMIDM_APP_2245_112_2_18_2_13_49_78 "MMIDM==> MMIDM_GetGPRSInfo string = %s"
#define MMIDM_APP_2271_112_2_18_2_13_49_79 "MMIDM==>MMIDM_SetGprsInfo type PNULL == connect_datail_ptr"
#define MMIDM_APP_2276_112_2_18_2_13_49_80 "MMIDM==>MMIDM_SetGprsInfo type = %d con_type = %d,len=%d,info=%s"
#define MMIDM_APP_2336_112_2_18_2_13_49_81 "MMIDM_SetGprsInfo type=%d"
#define MMIDM_APP_2369_112_2_18_2_13_49_82 "MMIDM==>DM_GetWAPInfo type = %d"
#define MMIDM_APP_2373_112_2_18_2_13_49_83 "MMIDM==>DM_GetWAPInfo type PNULL == connect_datail_ptr"
#define MMIDM_APP_2414_112_2_18_2_13_50_84 "MMIDM==> DM_GetWAPInfo type=%d"
#define MMIDM_APP_2417_112_2_18_2_13_50_85 "MMIDM==> DM_GetWAPInfo string = %s"
#define MMIDM_APP_2431_112_2_18_2_13_50_86 "MMIDM==>DM_SetWAPInfo type = %d info = %s length=%d"
#define MMIDM_APP_2494_112_2_18_2_13_50_87 "MMIDM==> DM_SetWAPInfo type=%d"
#define MMIDM_APP_2511_112_2_18_2_13_50_88 "MMIDM==>MMIDM_GetMMSCInfo type = %d"
#define MMIDM_APP_2531_112_2_18_2_13_50_89 "MMIDM==> MMIDM_GetMMSCInfo type=%d"
#define MMIDM_APP_2534_112_2_18_2_13_50_90 "MMIDM==> MMIDM_GetMMSCInfo string = %s"
#define MMIDM_APP_2553_112_2_18_2_13_50_91 "MMIDM==>MMIDM_SetMMSCInfo type = %d len=%d,info=%s"
#define MMIDM_APP_2576_112_2_18_2_13_50_92 "MMIDM==> MMIDM_SetMMSCInfo type=%d"
#define MMIDM_APP_2579_112_2_18_2_13_50_93 "MMIDM==> MMIDM_SetMMSCInfo string = %s"
#define MMIDM_APP_2592_112_2_18_2_13_50_94 "MMIDM==> MMIDM_GetPIMInfo type = %d"
#define MMIDM_APP_2616_112_2_18_2_13_50_95 "MMIDM==> MMIDM_GetPIMInfo type=%d"
#define MMIDM_APP_2619_112_2_18_2_13_50_96 "MMIDM==> MMIDM_GetPIMInfo string = %s"
#define MMIDM_APP_2634_112_2_18_2_13_50_97 "MMIDM==> MMIDM_GetPIMInfo type = %d"
#define MMIDM_APP_2663_112_2_18_2_13_50_98 "MMIDM==> MMIDM_SetPIMInfo type=%d"
#define MMIDM_APP_2668_112_2_18_2_13_50_99 "MMIDM==> MMIDM_SetPIMInfo string = %s"
#define MMIDM_APP_2683_112_2_18_2_13_50_100 "MMIDM==> MMIDM_GetStreamingInfo type = %d"
#define MMIDM_APP_2725_112_2_18_2_13_50_101 "MMIDM==> MMIDM_GetStreamingInfo type=%d"
#define MMIDM_APP_2729_112_2_18_2_13_50_102 "MMIDM==> MMIDM_GetStreamingInfo string = %s"
#define MMIDM_APP_2746_112_2_18_2_13_50_103 "MMIDM==> MMIDM_SetStreamingInfo type = %d"
#define MMIDM_APP_2784_112_2_18_2_13_50_104 "MMIDM==> MMIDM_SetStreamingInfo string = %s"
#define MMIDM_APP_2811_112_2_18_2_13_50_105 "MMIDM==> MMIDM_SetStreamingInfo type=%d"
#define MMIDM_APP_2817_112_2_18_2_13_50_106 "MMIDM==> MMIDM_SetStreamingInfo string = %s"
#define MMIDM_APP_2827_112_2_18_2_13_50_107 "MMIDM==> MMIDM_GetSrvAddURL srv_addr = %s"
#define MMIDM_APP_2839_112_2_18_2_13_50_108 "MMIDM==> MMIDM_GetLockStatusCode status"
#define MMIDM_APP_2866_112_2_18_2_13_50_109 "MMIDM==> MMIDM_HandleDmMessage 0x%x"
#define MMIDM_APP_2886_112_2_18_2_13_50_110 "MMIDM==> MSG_DM_SESSION_EXIT "
#define MMIDM_APP_2897_112_2_18_2_13_51_111 "MMIDM==> MMIDM_HandleDmMessage dm_sig->event_id=%d"
#define MMIDM_APP_2915_112_2_18_2_13_51_112 "MMIDM==> MMIDM_ResetFactorySetting "
#define MMIDM_APP_2939_112_2_18_2_13_51_113 "MMIDM==> MMIDM_ResetFactorySetting clear cl fail!"
#define MMIDM_APP_2982_112_2_18_2_13_51_114 "MMIDM==> MMIDM_GetDevFreeSpace free size = %d"
#define MMIDM_APP_3001_112_2_18_2_13_51_115 "MMIDM==> MMIDM_CheckDlSpaceEnouge result = %d"
#define MMIDM_APP_3018_112_2_18_2_13_51_116 "MMIDM==> MMIDM_InstallUpdatePackage succ!"
#define MMIDM_APP_3022_112_2_18_2_13_51_117 "MMIDM==> MMIDM_InstallUpdatePackage fail!"
#define MMIDM_APP_3047_112_2_18_2_13_51_118 "MMIDM==> MMIDM_CheckUpdateFile "
#define MMIDM_APP_3089_112_2_18_2_13_51_119 "MMIDM==> LoadFileToImage start"
#define MMIDM_APP_3099_112_2_18_2_13_51_120 "MMIDM==> LoadFileToImage MMIAPIFMM_IsFileExist"
#define MMIDM_APP_3104_112_2_18_2_13_51_121 "MMIDM==> LoadFileToImage sfs_handle=%d,fota_error=%d"
#define MMIDM_APP_3113_112_2_18_2_13_51_122 "MMIDM==> SFS_GetFileSize file_size=%d"
#define MMIDM_APP_3116_112_2_18_2_13_51_123 "MMIDM==> LoadFileToImage FOTA_GetSpaceSize()=%d"
#define MMIDM_APP_3124_112_2_18_2_13_51_124 "MMIDM==> LoadFileToImage FOTA_SetUpdateFlag fail!"
#define MMIDM_APP_3142_112_2_18_2_13_51_125 "MMIDM==> LoadFileToImage to_read_size=%d,bytes_read= %d,error=%d"
#define MMIDM_APP_3149_112_2_18_2_13_51_126 "MMIDM==> FOTA_ImageArea_Write fota_size =%d"
#define MMIDM_APP_3153_112_2_18_2_13_51_127 "MMIDM==> FOTA_ImageArea_Write fail fota_size=%d"
#define MMIDM_APP_3164_112_2_18_2_13_51_128 "MMIDM==> MMIAPIFMM_ReadFile fail!"
#define MMIDM_APP_3171_112_2_18_2_13_51_129 "MMIDM==> SFS_SetFilePointer fail!"
#define MMIDM_APP_3177_112_2_18_2_13_51_130 "MMIDM==> loop s_offset =%d,to_read_size=%d"
#define MMIDM_APP_3193_112_2_18_2_13_51_131 "MMIDM==> result = %d"
#define MMIDM_APP_3198_112_2_18_2_13_51_132 "MMIDM==> LoadFileToImage fota not switch on!"
#define MMIDM_APP_3220_112_2_18_2_13_51_133 "MMIDM==> fota state = %d"
#define MMIDM_APP_3230_112_2_18_2_13_51_134 "MMIDM==> fota state DM_FOTA_UPDATA_DONE"
#define MMIDM_APP_3252_112_2_18_2_13_51_135 "MMIDM==> MMIDM_SendFotaReport PLMN_status=%d,fota result = %d"
#define MMIDM_APP_3268_112_2_18_2_13_51_136 "MMIDM==> MMIDM_SendFotaReport "
#define MMIDM_APP_3274_112_2_18_2_13_51_137 "MMIDM==> MMIDM_SendFotaReport =%d"
#define MMIDM_APP_3279_112_2_18_2_13_51_138 "MMIDM==> plmn status=%d,fota result=%d"
#define MMIDM_APP_3310_112_2_18_2_13_51_139 "MMIDM==> MMIDM_ExtractAPPFile MMIAPIFMM_DeleteFile error= %d result= %d"
#define MMIDM_APP_3347_112_2_18_2_13_51_140 "MMIDM==> 2 read_size = %d,bytes_read=%d"
#define MMIDM_APP_3350_112_2_18_2_13_51_141 "MMIDM==> read sfspacket_handle fail 1 error=%d"
#define MMIDM_APP_3354_112_2_18_2_13_51_142 "MMIDM==> 3 bytes_read = %d,writelen=%d"
#define MMIDM_APP_3357_112_2_18_2_13_51_143 "MMIDM==> write sfsjad_handle fail 1 !error %d"
#define MMIDM_APP_3405_112_2_18_2_13_52_144 "MMIDM==> MMIDM_ExtractAppFile"
#define MMIDM_APP_3413_112_2_18_2_13_52_145 "MMIDM==> create sfspacket_handle fail !"
#define MMIDM_APP_3420_112_2_18_2_13_52_146 "MMIDM==> read sfspacket_handle fail !"
#define MMIDM_APP_3423_112_2_18_2_13_52_147 "MMIDM==> MMIDM_ExtractAppFile file_head=0x%x"
#define MMIDM_APP_3430_112_2_18_2_13_52_148 "MMIDM==> read sfspacket_handle fail !"
#define MMIDM_APP_3433_112_2_18_2_13_52_149 "MMIDM==> MMIDM_ExtractAppFile version=%d"
#define MMIDM_APP_3441_112_2_18_2_13_52_150 "MMIDM==> read sfspacket_handle fail !"
#define MMIDM_APP_3447_112_2_18_2_13_52_151 "MMIDM==> MMIDM_ExtractAppFile file_size=0x%x"
#define MMIDM_APP_3465_112_2_18_2_13_52_152 "MMIDM==> MMIDM_ExtractAppFile file_all_size=%d,jad size= %d,is_old_version %d"
#define MMIDM_APP_3489_112_2_18_2_13_52_153 "MMIDM==> create sfsjad_handle fail !"
#define MMIDM_APP_3502_112_2_18_2_13_52_154 "MMIDM==> create sfsjar_handle fail !"
#define MMIDM_APP_3520_112_2_18_2_13_52_155 "MMIDM==> app_java.all_size %d"
#define MMIDM_APP_3550_112_2_18_2_13_52_156 "MMIDM==> MMIDM_ExtractAppFile end"
#define MMIDM_APP_3555_112_2_18_2_13_52_157 "MMIDM==> File is not exist !"
#define MMIDM_APP_3615_112_2_18_2_13_52_158 "MMIDM==> MMIDM_ScomoInstall not support java!"
#define MMIDM_APP_3625_112_2_18_2_13_52_159 "MMIDM==> MMIDM_ScomoInstall fail!"
#define MMIDM_APP_3636_112_2_18_2_13_52_160 "MMIDM==> MMIDM_GetDlState state = %d "
#define MMIDM_APP_3646_112_2_18_2_13_52_161 "MMIDM==> MMIDM_SetDlState state = %d "
#define MMIDM_APP_3663_112_2_18_2_13_52_162 "MMIDM==> MMIDM_IsNeedDelayDMSession result= %d"
#define MMIDM_APP_3678_112_2_18_2_13_52_163 "MMIDM==> MMIDM_ScomoInstallCallback = %d"
#define MMIDM_APP_3686_112_2_18_2_13_52_164 "MMIDM==> install scomo dm result= %d"
#define MMIDM_APP_3690_112_2_18_2_13_52_165 "MMIDM==> install scomo dm fail!"
#define MMIDM_APP_3736_112_2_18_2_13_52_166 "MMIDM==> AddScomoIdToTab result=%d"
#define MMIDM_APP_3784_112_2_18_2_13_52_167 "MMIDM==> IsScomoApp result=%d"
#define MMIDM_APP_3793_112_2_18_2_13_52_168 "MMIDM==> MMIDM_ScomoRemoveCallback = %d"
#define MMIDM_APP_3806_112_2_18_2_13_52_169 "MMIDM==> remove scomo fail!"
#define MMIDM_APP_3824_112_2_18_2_13_52_170 "MMIDM==> MMIDM_CleanInstallFile "
#define MMIDM_APP_3843_112_2_18_2_13_52_171 "MMIDM==> error = %d"
#define MMIDM_APP_3865_112_2_18_2_13_53_172 "MMIDM==> MMIDM_DelDmFolderFiles error=%d"
#define MMIDM_APP_3889_112_2_18_2_13_53_173 "MMIDM==> MMIDM_GetResumeType type = %d"
#define MMIDM_APP_3929_112_2_18_2_13_53_174 "MMIDM==> MMIDM_SetBrokenType type = %d"
#define MMIDM_APP_3942_112_2_18_2_13_53_175 "MMIDM==> MMIDM_GetBrokemType broken_type = %d"
#define MMIDM_EXPORT_180_112_2_18_2_13_53_176 "MMIDM==> MMIAPIDM_HandleScomoCallback suiteID=%d"
#define MMIDM_INTERNAL_63_112_2_18_2_13_54_177 "MMIDM==>  MMIDM_Run "
#define MMIDM_INTERNAL_73_112_2_18_2_13_54_178 "MMIDM==>  MMIDM_Destroy "
#define MMIDM_INTERNAL_83_112_2_18_2_13_55_179 "MMIDM==>  MMIDM_SessionCancel "
#define MMIDM_INTERNAL_243_112_2_18_2_13_55_180 "MMIDM==> MMIDM_FUMO_SessionController_triggerDL pRootURI=%s"
#define MMIDM_INTERNAL_256_112_2_18_2_13_55_181 "MMIDM==> MMIDM_SCOMO_DP_SessionHandler_DL_trigger pRootURI %s"
#define MMIDM_INTERNAL_657_112_2_18_2_13_56_182 "MMIDM==>  MMIDM_Destroy "
#define MMIDM_INTERNAL_667_112_2_18_2_13_56_183 "MMIDM==>  MMIDM_SessionCancel "
#define MMIDM_PARSEXML_302_112_2_18_2_13_59_184 "MMIDM_setSessionId id %d"
#define MMIDM_PARSEXML_325_112_2_18_2_13_59_185 "MMIDM_GetTagStr str == PNULL"
#define MMIDM_PARSEXML_335_112_2_18_2_13_59_186 "MMIDM_GetTagStr the buf is too small %d"
#define MMIDM_PARSEXML_351_112_2_18_2_13_59_187 "MMIDM_setResUri %s"
#define MMIDM_PARSEXML_358_112_2_18_2_13_59_188 "MMIDM_setResUri the buf is too small %d"
#define MMIDM_PARSEXML_376_112_2_18_2_13_59_189 "MMIDM_getResUri %s"
#define MMIDM_PARSEXML_406_112_2_18_2_13_59_190 "MMIDM_CreateTag PNULL == buf"
#define MMIDM_PARSEXML_422_112_2_18_2_13_59_191 "MMIDM_CreateTag bufsize is too small %d"
#define MMIDM_PARSEXML_432_112_2_18_2_13_59_192 "MMIDM_CreateTag bufsize is too small %d"
#define MMIDM_PARSEXML_502_112_2_18_2_14_0_193 "MMIDM_GetCreddata PNULL == outbuf"
#define MMIDM_PARSEXML_539_112_2_18_2_14_0_194 "MMIDM_GetCreddata PNULL == outbuf"
#define MMIDM_PARSEXML_546_112_2_18_2_14_0_195 "MMIDM_GetCreddata after MMIDM_B64_Md51 %s"
#define MMIDM_PARSEXML_557_112_2_18_2_14_0_196 "MMIDM_GetCreddata  %s"
#define MMIDM_PARSEXML_560_112_2_18_2_14_0_197 "MMIDM_GetCreddata after MMIDM_B64_Md52 %s"
#define MMIDM_PARSEXML_579_112_2_18_2_14_0_198 "MMIDM_GetCreddata PNULL == outbuf"
#define MMIDM_PARSEXML_585_112_2_18_2_14_0_199 "MMIDM_GetCreddata s_g_nonce  %s"
#define MMIDM_PARSEXML_589_112_2_18_2_14_0_200 "MMIDM_GetCreddata nonece %s"
#define MMIDM_PARSEXML_597_112_2_18_2_14_0_201 "MMIDM_GetCreddata after  %s"
#define MMIDM_PARSEXML_604_112_2_18_2_14_0_202 "MMIDM_GetCreddata after md5 %s"
#define MMIDM_PARSEXML_606_112_2_18_2_14_0_203 "MMIDM_GetCreddata after b64 %s"
#define MMIDM_PARSEXML_623_112_2_18_2_14_0_204 "ENTER InitDMXMLHeader"
#define MMIDM_PARSEXML_629_112_2_18_2_14_0_205 "InitDMXMLHeader PNULL == headerbuf"
#define MMIDM_PARSEXML_639_112_2_18_2_14_0_206 "InitDMXMLHeader PNULL == buf"
#define MMIDM_PARSEXML_648_112_2_18_2_14_0_207 "InitDMXMLHeader PNULL == buf2"
#define MMIDM_PARSEXML_657_112_2_18_2_14_0_208 "InitDMXMLHeader PNULL== ptr"
#define MMIDM_PARSEXML_666_112_2_18_2_14_0_209 "InitDMXMLHeader PNULL== ptr->tagContent"
#define MMIDM_PARSEXML_773_112_2_18_2_14_0_210 "MMIDM creddata %s"
#define MMIDM_PARSEXML_867_112_2_18_2_14_0_211 "ENTER MMIDM_BuildAlert"
#define MMIDM_PARSEXML_871_112_2_18_2_14_0_212 "MMIDM_BuildAlert PNULL == alert_buf"
#define MMIDM_PARSEXML_879_112_2_18_2_14_0_213 "MMIDM_BuildItem PNULL == buf"
#define MMIDM_PARSEXML_888_112_2_18_2_14_0_214 "MMIDM_BuildItem PNULL == buf2"
#define MMIDM_PARSEXML_897_112_2_18_2_14_0_215 "MMIDM_BuildAlert PNULL== ptr"
#define MMIDM_PARSEXML_915_112_2_18_2_14_0_216 "MMIDM_BuildAlert buf size is too small buf2 %s"
#define MMIDM_PARSEXML_930_112_2_18_2_14_0_217 "MMIDM_BuildAlertbuf size is too small  buf2 %s"
#define MMIDM_PARSEXML_944_112_2_18_2_14_0_218 "MMIDM_BuildAlert PNULL==  ptr->tagContent"
#define MMIDM_PARSEXML_958_112_2_18_2_14_1_219 "MMIDM_BuildAlert alert_buf is too small buf_size %d"
#define MMIDM_PARSEXML_988_112_2_18_2_14_1_220 "LEAVE MMIDM_BuildAlert"
#define MMIDM_PARSEXML_1007_112_2_18_2_14_1_221 "ENTER MMIDM_BuildISource"
#define MMIDM_PARSEXML_1013_112_2_18_2_14_1_222 "MMIDM_BuildISource PNULL == buf"
#define MMIDM_PARSEXML_1022_112_2_18_2_14_1_223 "MMIDM_BuildISource PNULL == buf2"
#define MMIDM_PARSEXML_1044_112_2_18_2_14_1_224 "MMIDM_BuildIsource PNULL== ptr"
#define MMIDM_PARSEXML_1055_112_2_18_2_14_1_225 "MMIDM_BuildISource PNULL==  ptr->tagContent"
#define MMIDM_PARSEXML_1108_112_2_18_2_14_1_226 "ENTER MMIDM_BuildIMeta"
#define MMIDM_PARSEXML_1111_112_2_18_2_14_1_227 "MMIDM_BuildIMeta PNULL == meta_buf"
#define MMIDM_PARSEXML_1120_112_2_18_2_14_1_228 "MMIDM_BuildIMeta PNULL == buf"
#define MMIDM_PARSEXML_1129_112_2_18_2_14_1_229 "MMIDM_BuildIMeta PNULL == buf2"
#define MMIDM_PARSEXML_1155_112_2_18_2_14_1_230 "MMIDM_BuildIMeta PNULL== ptr"
#define MMIDM_PARSEXML_1165_112_2_18_2_14_1_231 "MMIDM_BuildIMeta PNULL==  ptr->tagContent"
#define MMIDM_PARSEXML_1217_112_2_18_2_14_1_232 "ENTER MMIDM_BuildItem"
#define MMIDM_PARSEXML_1220_112_2_18_2_14_1_233 "MMIDM_BuildItem PNULL == item_buf"
#define MMIDM_PARSEXML_1229_112_2_18_2_14_1_234 "MMIDM_BuildItem PNULL == buf"
#define MMIDM_PARSEXML_1238_112_2_18_2_14_1_235 "MMIDM_BuildItem PNULL == buf2"
#define MMIDM_PARSEXML_1253_112_2_18_2_14_1_236 "MMIDM_BuildItem buf size is too small"
#define MMIDM_PARSEXML_1266_112_2_18_2_14_1_237 "MMIDM_BuildItem buf size is too small"
#define MMIDM_PARSEXML_1276_112_2_18_2_14_1_238 "MMIDM_BuildItem PNULL== ptr"
#define MMIDM_PARSEXML_1287_112_2_18_2_14_1_239 "MMIDM_BuildItem PNULL==  ptr->tagContent"
#define MMIDM_PARSEXML_1344_112_2_18_2_14_1_240 "ENTER MMIDM_BuildReplace"
#define MMIDM_PARSEXML_1347_112_2_18_2_14_1_241 "MMIDM_BuildReplace PNULL == replace_buf"
#define MMIDM_PARSEXML_1357_112_2_18_2_14_1_242 "MMIDM_BuildReplace PNULL == buf"
#define MMIDM_PARSEXML_1366_112_2_18_2_14_1_243 "MMIDM_BuildReplace PNULL == buf2"
#define MMIDM_PARSEXML_1371_112_2_18_2_14_1_244 "MMIDM_BuildReplace after malloc buf"
#define MMIDM_PARSEXML_1385_112_2_18_2_14_1_245 "MMIDM_BuildReplace buf is too small"
#define MMIDM_PARSEXML_1397_112_2_18_2_14_1_246 "MMIDM_BuildReplace buf is too small"
#define MMIDM_PARSEXML_1405_112_2_18_2_14_1_247 "MMIDM_BuildReplace PNULL== ptr"
#define MMIDM_PARSEXML_1416_112_2_18_2_14_1_248 "MMIDM_BuildReplace PNULL==  ptr->tagContent"
#define MMIDM_PARSEXML_1485_112_2_18_2_14_2_249 "ENTER MMIDM_BuildISource"
#define MMIDM_PARSEXML_1492_112_2_18_2_14_2_250 "MMIDM_BuildISource PNULL == buf"
#define MMIDM_PARSEXML_1506_112_2_18_2_14_2_251 "MMIDM_BuildIChal PNULL== ptr"
#define MMIDM_PARSEXML_1517_112_2_18_2_14_2_252 "MMIDM_BuildIChal PNULL==  ptr->tagContent"
#define MMIDM_PARSEXML_1568_112_2_18_2_14_2_253 "ENTER MMIDM_BuildStatus"
#define MMIDM_PARSEXML_1574_112_2_18_2_14_2_254 "MMIDM_BuildStatus PNULL == status_buf"
#define MMIDM_PARSEXML_1583_112_2_18_2_14_2_255 "MMIDM_BuildStatus PNULL == buf"
#define MMIDM_PARSEXML_1592_112_2_18_2_14_2_256 "MMIDM_BuildReplace PNULL == buf2"
#define MMIDM_PARSEXML_1610_112_2_18_2_14_2_257 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1622_112_2_18_2_14_2_258 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1634_112_2_18_2_14_2_259 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1646_112_2_18_2_14_2_260 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1658_112_2_18_2_14_2_261 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1670_112_2_18_2_14_2_262 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1684_112_2_18_2_14_2_263 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1692_112_2_18_2_14_2_264 "MMIDM_BuildStatus PNULL== ptr"
#define MMIDM_PARSEXML_1703_112_2_18_2_14_2_265 "MMIDM_BuildStatus PNULL==  ptr->tagContent"
#define MMIDM_PARSEXML_1766_112_2_18_2_14_2_266 "ENTER MMIDM_BuildResult"
#define MMIDM_PARSEXML_1771_112_2_18_2_14_2_267 "MMIDM_BuildResult PNULL == result_buf"
#define MMIDM_PARSEXML_1780_112_2_18_2_14_2_268 "MMIDM_BuildResult  PNULL == buf"
#define MMIDM_PARSEXML_1789_112_2_18_2_14_2_269 "MMIDM_BuildResult PNULL == buf2"
#define MMIDM_PARSEXML_1807_112_2_18_2_14_2_270 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1819_112_2_18_2_14_2_271 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1831_112_2_18_2_14_2_272 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1843_112_2_18_2_14_2_273 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1856_112_2_18_2_14_2_274 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1869_112_2_18_2_14_2_275 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1881_112_2_18_2_14_2_276 "MMIDM_BuildStatus buf is too small"
#define MMIDM_PARSEXML_1889_112_2_18_2_14_2_277 "MMIDM_BuildReplace PNULL== ptr"
#define MMIDM_PARSEXML_1900_112_2_18_2_14_2_278 "MMIDM_BuildResult PNULL==  ptr->tagContent"
#define MMIDM_PARSEXML_1971_112_2_18_2_14_3_279 "ENTER MMIDM_CodecXmlBody"
#define MMIDM_PARSEXML_1977_112_2_18_2_14_3_280 "MMIDM_CodecXmlBody PNULL == bodybuf"
#define MMIDM_PARSEXML_1986_112_2_18_2_14_3_281 "MMIDM_BuildResult  PNULL == buf"
#define MMIDM_PARSEXML_1999_112_2_18_2_14_3_282 "MMIDM_CodecXmlBody PNULL== alert_tag"
#define MMIDM_PARSEXML_2015_112_2_18_2_14_3_283 "MMIDM_CodecXmlBody PNULL==  alert_tag->CmdId.tagContent"
#define MMIDM_PARSEXML_2028_112_2_18_2_14_3_284 "MMIDM_CodecXmlBody PNULL== alert_tag->DATA.tagContent"
#define MMIDM_PARSEXML_2043_112_2_18_2_14_3_285 "MMIDM_CodecXmlBody PNULL== replace_tag"
#define MMIDM_PARSEXML_2049_112_2_18_2_14_3_286 "MMIDM_CodecXmlBody after malloc replace_tag"
#define MMIDM_PARSEXML_2058_112_2_18_2_14_3_287 "MMIDM_CodecXmlBody PNULL==replace_tag->CmdId.tagContent"
#define MMIDM_PARSEXML_2065_112_2_18_2_14_3_288 "MMIDM_CodecXmlBody after TAG_CMDIDID"
#define MMIDM_PARSEXML_2070_112_2_18_2_14_3_289 "MMIDM_CodecXmlBody PNULL== item_tag"
#define MMIDM_PARSEXML_2080_112_2_18_2_14_3_290 "MMIDM_CodecXmlBody PNULL==item_tag->source.locuri.tagContent"
#define MMIDM_PARSEXML_2092_112_2_18_2_14_3_291 "MMIDM_CodecXmlBody PNULL==item_tag->data.tagContent"
#define MMIDM_PARSEXML_2105_112_2_18_2_14_3_292 "MMIDM_CodecXmlBody PNULL== item_tag"
#define MMIDM_PARSEXML_2115_112_2_18_2_14_3_293 "MMIDM_CodecXmlBody PNULL==item_tag->source.locuri.tagContent"
#define MMIDM_PARSEXML_2127_112_2_18_2_14_3_294 "MMIDM_CodecXmlBody PNULL==item_tag->data.tagContent"
#define MMIDM_PARSEXML_2141_112_2_18_2_14_3_295 "MMIDM_CodecXmlBody PNULL== item_tag"
#define MMIDM_PARSEXML_2151_112_2_18_2_14_3_296 "MMIDM_CodecXmlBody PNULL==item_tag->source.locuri.tagContent"
#define MMIDM_PARSEXML_2162_112_2_18_2_14_3_297 "MMIDM_CodecXmlBody PNULL==item_tag->data.tagContent"
#define MMIDM_PARSEXML_2175_112_2_18_2_14_3_298 "MMIDM_CodecXmlBody PNULL== item_tag"
#define MMIDM_PARSEXML_2185_112_2_18_2_14_3_299 "MMIDM_CodecXmlBody PNULL==item_tag->source.locuri.tagContent"
#define MMIDM_PARSEXML_2196_112_2_18_2_14_3_300 "MMIDM_CodecXmlBody PNULL==item_tag->data.tagContent"
#define MMIDM_PARSEXML_2208_112_2_18_2_14_3_301 "MMIDM_CodecXmlBody PNULL== item_tag"
#define MMIDM_PARSEXML_2213_112_2_18_2_14_3_302 "MMIDM_CodecXmlBody after ./DevInfo/DevId 0"
#define MMIDM_PARSEXML_2219_112_2_18_2_14_3_303 "MMIDM_CodecXmlBody PNULL==item_tag->source.locuri.tagContent"
#define MMIDM_PARSEXML_2230_112_2_18_2_14_3_304 "MMIDM_CodecXmlBody PNULL==item_tag->meta.format.tagContent"
#define MMIDM_PARSEXML_2240_112_2_18_2_14_3_305 "MMIDM_CodecXmlBody after ./DevInfo/DevId 1.5"
#define MMIDM_PARSEXML_2242_112_2_18_2_14_3_306 "MMIDM_CodecXmlBody after ./DevInfo/DevId 1.6"
#define MMIDM_PARSEXML_2245_112_2_18_2_14_3_307 "MMIDM_CodecXmlBody PNULL==item_tag->data.tagContent"
#define MMIDM_PARSEXML_2249_112_2_18_2_14_3_308 "MMIDM_CodecXmlBody after ./DevInfo/DevId 1.7"
#define MMIDM_PARSEXML_2251_112_2_18_2_14_3_309 "MMIDM_CodecXmlBody after ./DevInfo/DevId 1.8"
#define MMIDM_PARSEXML_2253_112_2_18_2_14_3_310 "MMIDM_CodecXmlBody after ./DevInfo/DevId 2"
#define MMIDM_PARSEXML_2259_112_2_18_2_14_3_311 "MMIDM_CodecXmlBody after ./DevInfo/DevId"
#define MMIDM_PARSEXML_2261_112_2_18_2_14_3_312 "MMIDM_CodecXmlBody begin to MMIDM_BuildReplace"
#define MMIDM_PARSEXML_2280_112_2_18_2_14_3_313 "MMIDM_CodecXmlBody buf is too small"
#define MMIDM_PARSEXML_2290_112_2_18_2_14_3_314 "MMIDM_CodecXmlBody buf is too small"
#define MMIDM_PARSEXML_2300_112_2_18_2_14_3_315 "MMIDM_CodecXmlBody buf is too small"
#define MMIDM_PARSEXML_2310_112_2_18_2_14_3_316 "MMIDM_CodecXmlBody buf is too small"
#define MMIDM_PARSEXML_2319_112_2_18_2_14_3_317 "MMIDM_CodecXmlBody PNULL== alert_tag"
#define MMIDM_PARSEXML_2333_112_2_18_2_14_3_318 "MMIDM_CodecXmlBody PNULL==  alert_tag->CmdId.tagContent"
#define MMIDM_PARSEXML_2346_112_2_18_2_14_3_319 "MMIDM_CodecXmlBody PNULL== alert_tag->DATA.tagContent"
#define MMIDM_PARSEXML_2362_112_2_18_2_14_3_320 "MMIDM_CodecXmlBody PNULL== ptr"
#define MMIDM_PARSEXML_2373_112_2_18_2_14_3_321 "MMIDM_CodecXmlBody PNULL==  ptr->tagContent"
#define MMIDM_PARSEXML_2424_112_2_18_2_14_4_322 "enter MMIDM_getNextXmlTagBuf"
#define MMIDM_PARSEXML_2427_112_2_18_2_14_4_323 "MMIDM_getNextXmlTagBuf PNULL == content"
#define MMIDM_PARSEXML_2493_112_2_18_2_14_4_324 "leave  MMIDM_getNextXmlTagBuf"
#define MMIDM_PARSEXML_2514_112_2_18_2_14_4_325 "ENTER MMIDM_generateXMLData"
#define MMIDM_PARSEXML_2521_112_2_18_2_14_4_326 "MMIDM_generateXMLData PNULL== bodybuf"
#define MMIDM_PARSEXML_2530_112_2_18_2_14_4_327 "MMIDM_generateXMLData PNULL== sendContent"
#define MMIDM_PARSEXML_2537_112_2_18_2_14_4_328 "MMIDM_generateXMLData HEADER"
#define MMIDM_PARSEXML_2539_112_2_18_2_14_4_329 "MMIDM_generateXMLData BODY"
#define MMIDM_PARSEXML_2545_112_2_18_2_14_4_330 "MMIDM_generateXMLData PNULL== ptr"
#define MMIDM_PARSEXML_2556_112_2_18_2_14_4_331 "MMIDM_generateXMLData PNULL==  ptr->tagContent"
#define MMIDM_PARSEXML_2631_112_2_18_2_14_4_332 "MMIDM_DealWithExecData PNULL== status_tag"
#define MMIDM_PARSEXML_2645_112_2_18_2_14_4_333 "MMIDM_DealWithExecData PNULL== status_tag->CmdId.tagContent"
#define MMIDM_PARSEXML_2658_112_2_18_2_14_4_334 "MMIDM_DealWithExecData PNULL==status_tag->msgRef.tagContent"
#define MMIDM_PARSEXML_2670_112_2_18_2_14_4_335 "MMIDM_DealWithExecData PNULL==  status_tag->cmdRef.tagContent"
#define MMIDM_PARSEXML_2682_112_2_18_2_14_4_336 "MMIDM_DealWithExecData PNULL== status_tag->targetRef.tagContent"
#define MMIDM_PARSEXML_2694_112_2_18_2_14_4_337 "MMIDM_DealWithExecData PNULL== status_tag->cmd.tagContent"
#define MMIDM_PARSEXML_2707_112_2_18_2_14_4_338 "MMIDM_DealWithExecData PNULL== status_tag->data.tagContent"
#define MMIDM_PARSEXML_2765_112_2_18_2_14_4_339 "ENTER MMIDM_ParseXMLStatus"
#define MMIDM_PARSEXML_2771_112_2_18_2_14_4_340 "MMIDM_ParseXMLStatus PNULL== bodybuf"
#define MMIDM_PARSEXML_2780_112_2_18_2_14_4_341 "MMIDM_ParseXMLExec PNULL== exec_buf"
#define MMIDM_PARSEXML_2788_112_2_18_2_14_4_342 "MMIDM_ParseXMLExec body_buf"
#define MMIDM_PARSEXML_2791_112_2_18_2_14_4_343 "MMIDM_ParseXMLStatus exec_buf"
#define MMIDM_PARSEXML_2811_112_2_18_2_14_4_344 "leave MMIDM_ParseXMLExec"
#define MMIDM_PARSEXML_2838_112_2_18_2_14_4_345 "ENTER MMIDM_DealWithStatusData"
#define MMIDM_PARSEXML_2841_112_2_18_2_14_4_346 "MMIDM_DealWithStatusData PNULL == status_buf"
#define MMIDM_PARSEXML_2858_112_2_18_2_14_4_347 "MMIDM_DealWithStatusData the cred status s_g_nonce %s"
#define MMIDM_PARSEXML_2873_112_2_18_2_14_4_348 "MMIDM_DealWithStatusData cred fail"
#define MMIDM_PARSEXML_2884_112_2_18_2_14_4_349 "MMIDM_DealWithStatusData PNULL== status_tag"
#define MMIDM_PARSEXML_2897_112_2_18_2_14_4_350 "MMIDM_DealWithStatusData PNULL== status_tag->CmdId.tagContent"
#define MMIDM_PARSEXML_2910_112_2_18_2_14_4_351 "MMIDM_DealWithStatusData PNULL==status_tag->msgRef.tagContent"
#define MMIDM_PARSEXML_2922_112_2_18_2_14_5_352 "MMIDM_DealWithStatusData PNULL==  status_tag->cmdRef.tagContent"
#define MMIDM_PARSEXML_2934_112_2_18_2_14_5_353 "MMIDM_DealWithStatusData PNULL== status_tag->targetRef.tagContent"
#define MMIDM_PARSEXML_2947_112_2_18_2_14_5_354 "MMIDM_DealWithStatusData PNULL== status_tag->targetRef.tagContent"
#define MMIDM_PARSEXML_2959_112_2_18_2_14_5_355 "MMIDM_DealWithStatusData PNULL== status_tag->cmd.tagContent"
#define MMIDM_PARSEXML_2973_112_2_18_2_14_5_356 "MMIDM_DealWithStatusData PNULL== status_tag->chal.meta.format.tagContent"
#define MMIDM_PARSEXML_2986_112_2_18_2_14_5_357 "MMIDM_DealWithStatusData PNULL== status_tag->chal.meta.type.tagContent"
#define MMIDM_PARSEXML_2999_112_2_18_2_14_5_358 "MMIDM_DealWithStatusData PNULL== status_tag->chal.meta.nextnonce.tagContent"
#define MMIDM_PARSEXML_3011_112_2_18_2_14_5_359 "MMIDM_DealWithStatusData PNULL== status_tag->data.tagContent"
#define MMIDM_PARSEXML_3038_112_2_18_2_14_5_360 "MMIDM_DealWithStatusData"
#define MMIDM_PARSEXML_3063_112_2_18_2_14_5_361 "ENTER MMIDM_ParseXMLStatus"
#define MMIDM_PARSEXML_3071_112_2_18_2_14_5_362 "MMIDM_ParseXMLStatus PNULL== bodybuf"
#define MMIDM_PARSEXML_3080_112_2_18_2_14_5_363 "MMIDM_ParseXMLStatus PNULL== status_buf"
#define MMIDM_PARSEXML_3088_112_2_18_2_14_5_364 "MMIDM_ParseXMLStatus body_buf"
#define MMIDM_PARSEXML_3091_112_2_18_2_14_5_365 "MMIDM_ParseXMLStatus status_buf"
#define MMIDM_PARSEXML_3137_112_2_18_2_14_5_366 "ENTER MMIDM_DealWithGetData"
#define MMIDM_PARSEXML_3144_112_2_18_2_14_5_367 "MMIDM_DealWithGetData PNULL== content"
#define MMIDM_PARSEXML_3159_112_2_18_2_14_5_368 "MMIDM_DealWithGetData PNULL== status_tag"
#define MMIDM_PARSEXML_3168_112_2_18_2_14_5_369 "MMIDM_DealWithGetData PNULL== result_tag"
#define MMIDM_PARSEXML_3178_112_2_18_2_14_5_370 "MMIDM_DealWithGetData PNULL== item_ptr"
#define MMIDM_PARSEXML_3197_112_2_18_2_14_5_371 "MMIDM_DealWithGetData PNULL== status_tag->CmdId.tagContent"
#define MMIDM_PARSEXML_3210_112_2_18_2_14_5_372 "MMIDM_DealWithGetData PNULL==status_tag->msgRef.tagContent"
#define MMIDM_PARSEXML_3222_112_2_18_2_14_5_373 "MMIDM_DealWithGetData PNULL==  status_tag->cmdRef.tagContent"
#define MMIDM_PARSEXML_3234_112_2_18_2_14_5_374 "MMIDM_DealWithGetData PNULL== status_tag->targetRef.tagContent"
#define MMIDM_PARSEXML_3246_112_2_18_2_14_5_375 "MMIDM_DealWithGetData PNULL== status_tag->cmd.tagContent"
#define MMIDM_PARSEXML_3260_112_2_18_2_14_5_376 "MMIDM_DealWithGetData PNULL== status_tag->data.tagContent"
#define MMIDM_PARSEXML_3272_112_2_18_2_14_5_377 "MMIDM_DealWithGetData, PNULL==read_func"
#define MMIDM_PARSEXML_3278_112_2_18_2_14_5_378 "MMIDM_DealWithGetData PNULL== status_tag->data.tagContent"
#define MMIDM_PARSEXML_3305_112_2_18_2_14_5_379 "MMIDM_DealWithGetData PNULL==result_tag->CmdId.tagContent"
#define MMIDM_PARSEXML_3318_112_2_18_2_14_5_380 "MMIDM_DealWithGetData PNULL==result_tag->msgRef.tagContent"
#define MMIDM_PARSEXML_3330_112_2_18_2_14_5_381 "MMIDM_DealWithGetData PNULL==result_tag->cmdRef.tagContent"
#define MMIDM_PARSEXML_3344_112_2_18_2_14_5_382 "MMIDM_DealWithGetData PNULL==item_ptr->source.locuri.tagContent"
#define MMIDM_PARSEXML_3360_112_2_18_2_14_5_383 "MMIDM_DealWithGetData PNULL==item_ptr->data.tagContent"
#define MMIDM_PARSEXML_3386_112_2_18_2_14_5_384 "MMIDM_DealWithGetData, PNULL==target_uri"
#define MMIDM_PARSEXML_3415_112_2_18_2_14_6_385 "ENTER MMIDM_ParseXMLGet"
#define MMIDM_PARSEXML_3423_112_2_18_2_14_6_386 "MMIDM_ParseXMLGet PNULL== bodybuf"
#define MMIDM_PARSEXML_3432_112_2_18_2_14_6_387 "MMIDM_ParseXMLGet PNULL== get_buf"
#define MMIDM_PARSEXML_3487_112_2_18_2_14_6_388 "ENTER MMIDM_DealWithReplaceData"
#define MMIDM_PARSEXML_3493_112_2_18_2_14_6_389 "MMIDM_DealWithReplaceData PNULL== content"
#define MMIDM_PARSEXML_3502_112_2_18_2_14_6_390 "MMIDM_DealWithReplaceData PNULL== data"
#define MMIDM_PARSEXML_3518_112_2_18_2_14_6_391 "MMIDM_DealWithReplaceData PNULL== status_tag"
#define MMIDM_PARSEXML_3533_112_2_18_2_14_6_392 "MMIDM_DealWithReplaceData PNULL==status_tag->CmdId.tagContent"
#define MMIDM_PARSEXML_3546_112_2_18_2_14_6_393 "MMIDM_DealWithReplaceData PNULL==status_tag->msgRef.tagContent"
#define MMIDM_PARSEXML_3558_112_2_18_2_14_6_394 "MMIDM_DealWithReplaceData PNULL==status_tag->cmdRef.tagContent"
#define MMIDM_PARSEXML_3570_112_2_18_2_14_6_395 "MMIDM_DealWithReplaceData PNULL==status_tag->targetRef.tagContent"
#define MMIDM_PARSEXML_3582_112_2_18_2_14_6_396 "MMIDM_DealWithReplaceData PNULL==status_tag->cmd.tagContent"
#define MMIDM_PARSEXML_3597_112_2_18_2_14_6_397 "MMIDM_DealWithReplaceData PNULL==status_tag->data.tagContent"
#define MMIDM_PARSEXML_3614_112_2_18_2_14_6_398 "MMIDM_DealWithGetData, PNULL==read_func"
#define MMIDM_PARSEXML_3620_112_2_18_2_14_6_399 "MMIDM_DealWithReplaceData PNULL==status_tag->data.tagContent"
#define MMIDM_PARSEXML_3672_112_2_18_2_14_6_400 "ENTER MMIDM_ParseXMLReplace"
#define MMIDM_PARSEXML_3679_112_2_18_2_14_6_401 "MMIDM_ParseXMLReplace PNULL== bodybuf"
#define MMIDM_PARSEXML_3688_112_2_18_2_14_6_402 "MMIDM_ParseXMLReplace PNULL== replace_buf"
#define MMIDM_PARSEXML_3731_112_2_18_2_14_6_403 "ENTER MMIDM_NotifyAlertResult"
#define MMIDM_PARSEXML_3735_112_2_18_2_14_6_404 "MMIDM_NotifyAlertResult PNULL == s_statusTag_cur"
#define MMIDM_PARSEXML_3742_112_2_18_2_14_6_405 "MMIDM_NotifyAlertResult PNULL==s_statusTag_cur->data.tagContent"
#define MMIDM_PARSEXML_3787_112_2_18_2_14_6_406 "ENTER MMIDM_DealWithAlertData"
#define MMIDM_PARSEXML_3795_112_2_18_2_14_6_407 "MMIDM_DealWithAlertData PNULL== item"
#define MMIDM_PARSEXML_3804_112_2_18_2_14_6_408 "MMIDM_DealWithAlertData PNULL== subdata"
#define MMIDM_PARSEXML_3816_112_2_18_2_14_6_409 "MMIDM_DealWithAlertData data"
#define MMIDM_PARSEXML_3844_112_2_18_2_14_6_410 "MMIDM_DealWithAlertData minimum display time %s"
#define MMIDM_PARSEXML_3860_112_2_18_2_14_6_411 "MMIDM_DealWithAlertData maximum display time %s"
#define MMIDM_PARSEXML_3864_112_2_18_2_14_6_412 "MMIDM_DealWithAlertData display content =%s"
#define MMIDM_PARSEXML_3871_112_2_18_2_14_6_413 "MMIDM_DealWithAlertData PNULL== status_tag"
#define MMIDM_PARSEXML_3885_112_2_18_2_14_6_414 "MMIDM_DealWithAlertData PNULL==status_tag->CmdId.tagContent"
#define MMIDM_PARSEXML_3899_112_2_18_2_14_6_415 "MMIDM_DealWithAlertData PNULL==status_tag->msgRef.tagContent"
#define MMIDM_PARSEXML_3912_112_2_18_2_14_7_416 "MMIDM_DealWithAlertData PNULL==status_tag->cmdRef.tagContent"
#define MMIDM_PARSEXML_3925_112_2_18_2_14_7_417 "MMIDM_DealWithAlertData PNULL==status_tag->cmd.tagContent"
#define MMIDM_PARSEXML_3983_112_2_18_2_14_7_418 "ENTER MMIDM_ParseXMLAlert"
#define MMIDM_PARSEXML_3990_112_2_18_2_14_7_419 "MMIDM_ParseXMLAlert PNULL== bodybuf"
#define MMIDM_PARSEXML_3999_112_2_18_2_14_7_420 "MMIDM_ParseXMLAlert PNULL== alert_buf"
#define MMIDM_PARSEXML_4181_112_2_18_2_14_7_421 "ENTER MMIDM_ReleaseXMLData"
#define MMIDM_PARSEXML_4225_112_2_18_2_14_7_422 "LEAVE MMIDM_ReleaseXMLData"
#define MMIDM_PARSEXML_4240_112_2_18_2_14_7_423 "enter MMIDM_ParseXMLData"
#define MMIDM_PARSEXML_4246_112_2_18_2_14_7_424 "MMIDM_ParseXMLData, the wrong sessionid %s"
#define MMIDM_PARSEXML_4254_112_2_18_2_14_7_425 "MMIDM_ParseXMLData, not the msg rely to our msg %s"
#define MMIDM_PARSEXML_4270_112_2_18_2_14_7_426 "enter MMIDM_ParseXMLData 1"
#define MMIDM_PARSEXML_4272_112_2_18_2_14_7_427 "enter MMIDM_ParseXMLData 2"
#define MMIDM_PARSEXML_4274_112_2_18_2_14_7_428 "enter MMIDM_ParseXMLData 3"
#define MMIDM_PARSEXML_4276_112_2_18_2_14_7_429 "enter MMIDM_ParseXMLData 4"
#define MMIDM_PARSEXML_4278_112_2_18_2_14_7_430 "enter MMIDM_ParseXMLData 5"
#define MMIDM_PARSEXML_4296_112_2_18_2_14_7_431 "MMIDM_SendDmData DM_CANCEL == MMIDM_GetDmState()"
#define MMIDM_PARSEXML_4302_112_2_18_2_14_7_432 "MMIDM_SendDmData PNULL== sendbuf"
#define MMIDM_PARSEXML_4346_112_2_18_2_14_7_433 "MMIDM_BeginDmTansport"
#define MMIDM_PARSEXML_4349_112_2_18_2_14_7_434 "MMIDM_BeginDmTansport DM_CANCEL == MMIDM_GetDmState()"
#define MMIDM_PARSEXML_4376_112_2_18_2_14_7_435 "MMIDM_ParseReceiveData NO PACKET TO SEND"
#define MMIDM_PARSEXML_4396_112_2_18_2_14_7_436 "MMIDM_ParseReceiveData DM_CANCEL == MMIDM_GetDmState()"
#define MMIDM_PARSEXML_4421_112_2_18_2_14_8_437 "mmidm %c%c%c%c%c%c"
#define MMIDM_PL_COMM_241_112_2_18_2_14_8_438 "MMIDM==> [RDM_COMMS_getDMSocketID_cbFunc] socketID = 0x%0x \r\n"
#define MMIDM_PL_COMM_256_112_2_18_2_14_8_439 "MMIDM==> [RDM_COMMS_GetDLSocketID_cbFunc] socketID = 0x%x \r\n"
#define MMIDM_PL_COMM_277_112_2_18_2_14_8_440 "MMIDM==> [RDM_COMMS_Construct_http] bodylen = %d\n"
#define MMIDM_PL_COMM_288_112_2_18_2_14_8_441 "MMIDM==> [RDM_COMMS_Construct_http] failed to extract host.\n"
#define MMIDM_PL_COMM_295_112_2_18_2_14_8_442 "MMIDM==> [RDM_COMMS_Construct_http] serverName = %s, port = %d\n"
#define MMIDM_PL_COMM_315_112_2_18_2_14_8_443 "MMIDM==> [RDM_COMMS_Construct_http] len=%d\n"
#define MMIDM_PL_COMM_316_112_2_18_2_14_8_444 "MMIDM==> [RDM_COMMS_Construct_http]Message too long to be hold.maxlen=%d\n"
#define MMIDM_PL_COMM_320_112_2_18_2_14_8_445 "MMIDM==> [RDM_COMMS_Construct_http] head lenght =%d\n"
#define MMIDM_PL_COMM_327_112_2_18_2_14_8_446 "MMIDM==> [RDM_COMMS_Construct_http] message constructed total len = %d \n"
#define MMIDM_PL_COMM_347_112_2_18_2_14_8_447 "MMIDM==> [findSubStringN] was called len= 0x%x,N= 0x%x,strN=0x%x\r\n"
#define MMIDM_PL_COMM_365_112_2_18_2_14_8_448 "MMIDM==> [findSubStringN]found the whole string,next =0x%x\r\n"
#define MMIDM_PL_COMM_430_112_2_18_2_14_9_449 "MMIDM==> [RDM_parse_syncml]  begin\r\n"
#define MMIDM_PL_COMM_455_112_2_18_2_14_9_450 "MMIDM==> [RDM_parse_syncml]  syncml_hdr_len=%d"
#define MMIDM_PL_COMM_468_112_2_18_2_14_9_451 "MMIDM==>RDM_parse_syncml  PNULL ==username_occ_ptr"
#define MMIDM_PL_COMM_480_112_2_18_2_14_9_452 "MMIDM==>RDM_parse_syncml  PNULL ==mac_occ_ptr"
#define MMIDM_PL_COMM_492_112_2_18_2_14_9_453 "MMIDM==>RDM_parse_syncml  PNULL ==algorithm_occ_ptr"
#define MMIDM_PL_COMM_536_112_2_18_2_14_9_454 "MMIDM==> [RDM_parse_syncml]3 RDM_ERR_MEMORY"
#define MMIDM_PL_COMM_548_112_2_18_2_14_9_455 "MMIDM==> [RDM_parse_syncml] s_loc_username =%s"
#define MMIDM_PL_COMM_550_112_2_18_2_14_9_456 "MMIDM==> [RDM_parse_syncml] s_loc_mac =%s"
#define MMIDM_PL_COMM_552_112_2_18_2_14_9_457 "MMIDM==> [RDM_parse_syncml] s_loc_algorithm =%s"
#define MMIDM_PL_COMM_565_112_2_18_2_14_9_458 "MMIDM==> [RDM_parse_syncml]  no mac\r\n"
#define MMIDM_PL_COMM_584_112_2_18_2_14_9_459 "MMIDM==>  VDM_Comm_PL_Conn_init "
#define MMIDM_PL_COMM_590_112_2_18_2_14_9_460 "MMIDM==>  VDM_Comm_PL_Conn_init result = %d "
#define MMIDM_PL_COMM_597_112_2_18_2_14_9_461 "MMIDM==>  VDM_Comm_PL_Conn_init result = %d "
#define MMIDM_PL_COMM_623_112_2_18_2_14_9_462 "MMIDM==>  VDM_Comm_PL_Conn_open "
#define MMIDM_PL_COMM_627_112_2_18_2_14_9_463 "MMIDM==>  VDM_Comm_PL_Conn_open PNULL == inURL"
#define MMIDM_PL_COMM_635_112_2_18_2_14_9_464 "MMIDM==>  VDM_Comm_PL_Conn_open:MMIDM_ActivePdpConnect not opened"
#define MMIDM_PL_COMM_643_112_2_18_2_14_9_465 "MMIDM==>  VDM_Comm_PL_Conn_open8:connection not opened to %s:%hu\n"
#define MMIDM_PL_COMM_648_112_2_18_2_14_9_466 "MMIDM==>  VDM_Comm_PL_Conn_open9 (connId = %d) returns 0x%x\n"
#define MMIDM_PL_COMM_673_112_2_18_2_14_9_467 "MMIDM==>  VDM_Comm_PL_Conn_send "
#define MMIDM_PL_COMM_679_112_2_18_2_14_9_468 "MMIDM==> VDM_Comm_PL_Conn_send success  actualSent = 0x%x"
#define MMIDM_PL_COMM_683_112_2_18_2_14_9_469 "MMIDM==>  VDM_Comm_PL_Conn_send :send attempt failed\n"
#define MMIDM_PL_COMM_687_112_2_18_2_14_9_470 "MMIDM==> VDM_Comm_PL_Conn_send (connId = %d) returns 0x%x\n"
#define MMIDM_PL_COMM_716_112_2_18_2_14_9_471 "MMIDM==>  VDM_Comm_PL_Conn_receive  inBufSize = %d"
#define MMIDM_PL_COMM_723_112_2_18_2_14_9_472 "MMIDM==>  VDM_Comm_PL_Conn_receive: recv attempt failed sock_errno =%d"
#define MMIDM_PL_COMM_729_112_2_18_2_14_9_473 "MMIDM==>  VDM_Comm_PL_Conn_receive: outDataLen = 0"
#define MMIDM_PL_COMM_737_112_2_18_2_14_9_474 "MMIDM==>  VDM_Comm_PL_Conn_receive notifyTransport left_data_len=%d"
#define MMIDM_PL_COMM_740_112_2_18_2_14_9_475 "MMIDM==>  VDM_Comm_PL_Conn_receive (connId = %d) returns 0x%x, *outDataLen=%d\n"
#define MMIDM_PL_COMM_757_112_2_18_2_14_9_476 "MMIDM==> VDM_Comm_PL_Conn_close (connId = %d) returns \n"
#define MMIDM_PL_COMM_767_112_2_18_2_14_9_477 "MMIDM==>  VDM_Comm_PL_Conn_term "
#define MMIDM_PL_COMM_775_112_2_18_2_14_9_478 "MMIDM==>  VDM_Comm_PL_Conn_term returns \n"
#define MMIDM_PL_COMM_830_112_2_18_2_14_9_479 "MMIDM==> VDM_Comm_PL_HTTP_init returns 0x%x\n"
#define MMIDM_PL_COMM_843_112_2_18_2_14_9_480 "MMIDM==>  VDM_Comm_PL_HTTP_init called (inAddrType=%s)\n"
#define MMIDM_PL_COMM_881_112_2_18_2_14_10_481 "MMIDM==> VDM_Comm_PL_HTTP_init called s_dm_observer=%x\n"
#define MMIDM_PL_COMM_883_112_2_18_2_14_10_482 "MMIDM==> VDM_Comm_PL_HTTP_init returns 0x%x\n"
#define MMIDM_PL_COMM_919_112_2_18_2_14_10_483 "MMIDM==> [VDM_Comm_PL_HTTP_open] begin, *outConnId = %d, addr = %s \r\n"
#define MMIDM_PL_COMM_920_112_2_18_2_14_10_484 "MMIDM==> [VDM_Comm_PL_HTTP_open]   inMimeType = %d, msgLen = %d,addr = %d \r\n"
#define MMIDM_PL_COMM_935_112_2_18_2_14_10_485 "MMIDM==> [VDM_Comm_PL_HTTP_open]   username %x = %s, mac %x = %s, algorithm %x = %s \r\n"
#define MMIDM_PL_COMM_950_112_2_18_2_14_10_486 "MMIDM==> [VDM_Comm_PL_HTTP_open] s_connect_info_ptr->server_url = %s\r\n"
#define MMIDM_PL_COMM_955_112_2_18_2_14_10_487 "MMIDM==> [VDM_Comm_PL_HTTP_open] ActivePdp fail!"
#define MMIDM_PL_COMM_964_112_2_18_2_14_10_488 "MMIDM==> [VDM_Comm_PL_HTTP_open] open fail!"
#define MMIDM_PL_COMM_999_112_2_18_2_14_10_489 "MMIDM==> VDM_Comm_PL_HTTP_send\r\n"
#define MMIDM_PL_COMM_1002_112_2_18_2_14_10_490 "MMIDM==> VDM_Comm_PL_HTTP_send total %d"
#define MMIDM_PL_COMM_1006_112_2_18_2_14_10_491 "MMIDM==> VDM_Comm_PL_HTTP_send sentLen %d"
#define MMIDM_PL_COMM_1021_112_2_18_2_14_10_492 "MMIDM==> VDM_Comm_PL_HTTP_send, total = %d, sentLen = %d"
#define MMIDM_PL_COMM_1071_112_2_18_2_14_10_493 "MMIDM==> [VDM_Comm_PL_HTTP_receive]  bufSize = %d, s_recvActualLen = %d\r\n"
#define MMIDM_PL_COMM_1075_112_2_18_2_14_10_494 "MMIDM==> [VDM_Comm_PL_HTTP_receive]  Error:RDM_ERR_BUFFER_OVERFLOW!\r\n"
#define MMIDM_PL_COMM_1083_112_2_18_2_14_10_495 "MMIDM==> [VDM_Comm_PL_HTTP_receive]  return error when cut the http header, t1_ptr=0x%x\r\n"
#define MMIDM_PL_COMM_1090_112_2_18_2_14_10_496 "MMIDM==> [VDM_Comm_PL_HTTP_receive]  LenHTTPbody=0x%x\r\n"
#define MMIDM_PL_COMM_1115_112_2_18_2_14_10_497 "MMIDM==> RDM_COMMS_recvMessage return ret =%d\r\n"
#define MMIDM_PL_COMM_1137_112_2_18_2_14_10_498 "MMIDM==> [VDM_Comm_PL_HTTP_receive]  bufSize = %d, s_recvActualLen = %d\r\n"
#define MMIDM_PL_COMM_1141_112_2_18_2_14_10_499 "MMIDM==> [VDM_Comm_PL_HTTP_receive]  Error:RDM_ERR_BUFFER_OVERFLOW!\r\n"
#define MMIDM_PL_COMM_1149_112_2_18_2_14_10_500 "MMIDM==> [VDM_Comm_PL_HTTP_receive]  return error when cut the http header, t1_ptr=0x%x\r\n"
#define MMIDM_PL_COMM_1156_112_2_18_2_14_10_501 "MMIDM==> [VDM_Comm_PL_HTTP_receive]  LenHTTPbody=0x%x\r\n"
#define MMIDM_PL_COMM_1165_112_2_18_2_14_10_502 "MMIDM==> RDM_COMMS_recvMessage return ret =%d\r\n"
#define MMIDM_PL_COMM_1183_112_2_18_2_14_10_503 "MMIDM==>  VDM_Comm_PL_HTTP_close (connId = %d) returns\n"
#define MMIDM_PL_COMM_1195_112_2_18_2_14_10_504 "MMIDM==>  VDM_Comm_PL_HTTP_term called\n"
#define MMIDM_PL_COMM_1232_112_2_18_2_14_10_505 "5"
#define MMIDM_PL_COMM_1239_112_2_18_2_14_10_506 "MMIDM==> VDM_Comm_PL_HTTP_term returns\n"
#define MMIDM_PL_COMM_1256_112_2_18_2_14_10_507 "MMIDM==> [RDM_Socket_Send], total_len = %d"
#define MMIDM_PL_COMM_1270_112_2_18_2_14_10_508 "MMIDM==> [RDM_Socket_Send], length = 0"
#define MMIDM_PL_COMM_1275_112_2_18_2_14_10_509 "MMIDM==> [RDM_Socket_Send], sent_len = %d"
#define MMIDM_PL_COMM_1315_112_2_18_2_14_10_510 "MMIDM==> [RDM_Socket_open] url = %s \r\n"
#define MMIDM_PL_COMM_1336_112_2_18_2_14_10_511 "MMIDM==> [RDM_Socket_Connect_Server] gv_Rdm_Socket = 0x%x \r\n"
#define MMIDM_PL_COMM_1352_112_2_18_2_14_10_512 "MMIDM==> RDM_Socket_Connect_Server, has http max_len= %d"
#define MMIDM_PL_COMM_1359_112_2_18_2_14_10_513 "MMIDM==> RDM_Socket_Connect_Server, has https max_len= %d"
#define MMIDM_PL_COMM_1364_112_2_18_2_14_10_514 "MMIDM==> RDM_Socket_Connect_Server, has not http and https\r\n"
#define MMIDM_PL_COMM_1368_112_2_18_2_14_11_515 "MMIDM==> RDM_Socket_Connect_Server, ServerAddress = %s\r\n"
#define MMIDM_PL_COMM_1375_112_2_18_2_14_11_516 "MMIDM==> RDM_Socket_Connect_Server, ServerAddress delete / = %s\r\n"
#define MMIDM_PL_COMM_1386_112_2_18_2_14_11_517 "MMIDM==> RDM_Socket_Connect_Server, ServerAddress delete : = %s, portNumber = %d\r\n"
#define MMIDM_PL_COMM_1402_112_2_18_2_14_11_518 "MMIDM==> RDM_Socket_Connect_Server no ip[%s],port=[%d]\r\n"
#define MMIDM_PL_COMM_1406_112_2_18_2_14_11_519 "MMIDM==> [RDM_Socket_Connect_Server], ret = %x\r\n"
#define MMIDM_PL_COMM_1416_112_2_18_2_14_11_520 "MMIDM==> [RDM_Socket_Create], socket_id = %x, netid = %drn"
#define MMIDM_PL_COMM_1433_112_2_18_2_14_11_521 "MMIDM==> [RDM_Socket_Create], iError = %x"
#define MMIDM_PL_COMM_1466_112_2_18_2_14_11_522 "MMIDM==> RDM_UrlIsAnIpAddress, url = %s\r\n"
#define MMIDM_PL_COMM_1467_112_2_18_2_14_11_523 "MMIDM==> RDM_UrlIsAnIpAddress, IPaddress = %s\r\n"
#define MMIDM_PL_COMM_1469_112_2_18_2_14_11_524 "MMIDM==> RDM_UrlIsAnIpAddress, v_len = %d\r\n"
#define MMIDM_PL_COMM_1475_112_2_18_2_14_11_525 "MMIDM==> RDM_UrlIsAnIpAddress ---url is a domain name\r\n"
#define MMIDM_PL_COMM_1479_112_2_18_2_14_11_526 "RDM_UrlIsAnIpAddress ---url is an dot decimal ip\r\n"
#define MMIDM_PL_COMM_1489_112_2_18_2_14_11_527 "MMIDM==> [RDM_Socket_Connect_by_ip], ip = %s,port = %d\r\n"
#define MMIDM_PL_COMM_1500_112_2_18_2_14_11_528 "[RDM_Socket_Connect_by_ip], ret = %d\r\n"
#define MMIDM_PL_COMM_1512_112_2_18_2_14_11_529 "MMIDM==> [RDM_Socket_Connect_by_name], server_name = %s,port = %d\r\n"
#define MMIDM_PL_COMM_1533_112_2_18_2_14_11_530 "[RDM_Socket_Connect_by_name], ret = %d\r\n"
#define MMIDM_PL_COMM_1539_112_2_18_2_14_11_531 "MMIDM==> VDM_PL_DD_field_value: inField=%s, inValue=%s"
#define MMIDM_PL_COMM_1553_112_2_18_2_14_11_532 "MMIDM==> VDM_HandleSocketMessage type 0x%x ,sock_id = 0x%x dm=0x%x,dl=0x%x"
#define MMIDM_PL_COMM_1557_112_2_18_2_14_11_533 "MMIDM==> SOCKET_STATE_READ"
#define MMIDM_PL_COMM_1570_112_2_18_2_14_11_534 "MMIDM==> SOCKET_STATE_WRITE"
#define MMIDM_PL_COMM_1577_112_2_18_2_14_11_535 "begin to write xml"
#define MMIDM_PL_COMM_1587_112_2_18_2_14_11_536 "MMIDM==> SOCKET_STATE_CONNECT"
#define MMIDM_PL_COMM_1595_112_2_18_2_14_11_537 "MMIDM_HandleSocketMessage DM_CANCEL == MMIDM_GetDmState()"
#define MMIDM_PL_COMM_1611_112_2_18_2_14_11_538 "MMIDM==> SOCKET_STATE_CLOSE"
#define MMIDM_PL_COMM_1672_112_2_18_2_14_11_539 "MMIDM==> RDM_isWholeMsgArrived: starting (0x%x, %d)\n"
#define MMIDM_PL_COMM_1676_112_2_18_2_14_11_540 "MMIDM==> RDM_isWholeMsgArrived: found EOM (0x%x)\n"
#define MMIDM_PL_COMM_1682_112_2_18_2_14_11_541 "MMIDM==> RDM_isWholeMsgArrived: found ContentLength (%d)\n"
#define MMIDM_PL_COMM_1685_112_2_18_2_14_11_542 "MMIDM==> RDM_isWholeMsgArrived: no ContentLength\n"
#define MMIDM_PL_COMM_1691_112_2_18_2_14_11_543 "MMIDM==> RDM_isWholeMsgArrived: inMsgPos %d"
#define MMIDM_PL_COMM_1695_112_2_18_2_14_11_544 "MMIDM==> RDM_isWholeMsgArrived: found Whole message\n"
#define MMIDM_PL_COMM_1714_112_2_18_2_14_11_545 "RDM_COMMS_Data_Arrive PNULL == buf"
#define MMIDM_PL_COMM_1721_112_2_18_2_14_11_546 "MMIDM==> RDM_COMMS_Data_Arrive socket_id=0x%x\n"
#define MMIDM_PL_COMM_1724_112_2_18_2_14_11_547 "MMIDM==> RDM_COMMS_Data_Arrive s_recvActualLen=%d\n"
#define MMIDM_PL_COMM_1747_112_2_18_2_14_11_548 "RDM_COMMS_Data_Arrive the receiver content"
#define MMIDM_PL_COMM_1830_112_2_18_2_14_11_549 "MMIDM==> MMIDM_BrokenDMSession dl socket session "
#define MMIDM_PL_COMM_1836_112_2_18_2_14_11_550 "MMIDM==> MMIDM_BrokenDMSession dm socket session "
#define MMIDM_PL_MMI_877_112_2_18_2_14_14_551 "MMIDM==> VDM_MMI_PL_init fail!"
#define MMIDM_PL_MMI_891_112_2_18_2_14_14_552 "MMIDM==> VDM_MMI_PL_term inInfoType "
#define MMIDM_PL_MMI_921_112_2_18_2_14_14_553 "MMIDM==> VDM_MMI_PL_infoMessage inInfoType=%d"
#define MMIDM_PL_MMI_948_112_2_18_2_14_14_554 "MMIDM==> VDM_MMI_PL_infoMessage inDefaultCommand = %d"
#define MMIDM_PL_MMI_951_112_2_18_2_14_14_555 "MMIDM==> VDM_MMI_PL_confirmationQuery alert_type %d"
#define MMIDM_PL_MMI_976_112_2_18_2_14_14_556 "MMIDM==> VDM_MMI_PL_inputQuery maxLength = %d"
#define MMIDM_PL_MMI_1002_112_2_18_2_14_14_557 "MMIDM==> VDM_MMI_PL_choiceListQuery isMultipleSelection = %d, itemsCount=%d"
#define MMIDM_PL_MMI_1090_112_2_18_2_14_14_558 "MMIDM==> VDM_MMI_PL_updateProgress inCurrentProgress = %d, inMaxProgress=%d"
#define MMIDM_PL_MMI_1124_112_2_18_2_14_14_559 "MMIDM==> DM_SessionStateChange inType=%d,inState=%d,inLastError=%d "
#define MMIDM_PL_MMI_1160_112_2_18_2_14_14_560 "MMIDM==> MMIDM_FreeAllMOInstance"
#define MMIDM_PL_MMI_1168_112_2_18_2_14_14_561 "MMIDM==> MMIDM_FreeAllMOInstance exit"
#define MMIDM_PL_MMI_1182_112_2_18_2_14_14_562 "MMIDM==> MMIDM_StartVDM start"
#define MMIDM_PL_MMI_1193_112_2_18_2_14_14_563 "MMIDM==> MMIDM_StartVDM alert_type %d"
#define MMIDM_PL_MMI_1208_112_2_18_2_14_14_564 "MMIDM==> Failed to create vDM: error 0x%x"
#define MMIDM_PL_MMI_1239_112_2_18_2_14_15_565 "MMIDM==> Failed to start vDM: error 0x%x"
#define MMIDM_PL_MMI_1247_112_2_18_2_14_15_566 "MMIDM==> Failed to register callbacks: error 0x%x"
#define MMIDM_PL_MMI_1255_112_2_18_2_14_15_567 "MMIDM==> Failed to initialize plug-ins: error 0x%x"
#define MMIDM_PL_MMI_1268_112_2_18_2_14_15_568 "MMIDM==> MMIDM_StarVDM Failed to trigger session: error 0x%x"
#define MMIDM_PL_MMI_1296_112_2_18_2_14_15_569 "MMIDM==> Failed to run vDM: error 0x%x"
#define MMIDM_PL_MMI_1300_112_2_18_2_14_15_570 "MMIDM==> MMIDM_StarVDM end"
#define MMIDM_PL_MMI_1311_112_2_18_2_14_15_571 "MMIDM==> MMIDM_CreateThread"
#define MMIDM_PL_MMI_1323_112_2_18_2_14_15_572 "MMIDM==> DM_registerCallbacks"
#define MMIDM_PL_MMI_1369_112_2_18_2_14_15_573 "MMIDM==> VDMC_PluginMng_init"
#define MMIDM_PL_MMI_1374_112_2_18_2_14_15_574 "MMIDM==> Failed to initialize FUMO plugin: error 0x%x"
#define MMIDM_PL_MMI_1381_112_2_18_2_14_15_575 "MMIDM==> Failed to initialize SCOMO plugin: error 0x%x"
#define MMIDM_PL_MMI_1388_112_2_18_2_14_15_576 "MMIDM==> Failed to initialize LAWMO plugin: error 0x%x"
#define MMIDM_PL_MMI_1403_112_2_18_2_14_15_577 "MMIDM==> FUMO_Plugin_init"
#define MMIDM_PL_MMI_1423_112_2_18_2_14_15_578 "MMIDM==> FUMO_Plugin_term"
#define MMIDM_PL_MMI_1440_112_2_18_2_14_15_579 "MMIDM==> DM_FumoConfirmUpdate"
#define MMIDM_PL_MMI_1457_112_2_18_2_14_15_580 "MMIDM==> DM_FumoExecuteUpdate"
#define MMIDM_PL_MMI_1472_112_2_18_2_14_15_581 "MMIDM==> SCOMO_Plugin_init"
#define MMIDM_PL_MMI_1530_112_2_18_2_14_15_582 "MMIDM==> DM_OnScomoConfirmInstallation inOperationPath %s"
#define MMIDM_PL_MMI_1549_112_2_18_2_14_15_583 "MMIDM==> DM_OnScomoConfirmInstallation not support java!"
#define MMIDM_PL_MMI_1555_112_2_18_2_14_15_584 "MMIDM==> DM_OnScomoConfirmInstallation MMIDM_ExtractAPPFile error !!"
#define MMIDM_PL_MMI_1577_112_2_18_2_14_15_585 "MMIDM==> DM_GetDeviceModel "
#define MMIDM_PL_MMI_1592_112_2_18_2_14_15_586 "MMIDM==> VDMC_GetVdmMan "
#define MMIDM_PL_MMI_1606_112_2_18_2_14_15_587 "MMIDM==> VDMC_GetVdmOEM "
#define MMIDM_PL_MMI_1620_112_2_18_2_14_15_588 "MMIDM==> VDMC_GetVdmLanguage "
#define MMIDM_PL_MMI_1636_112_2_18_2_14_15_589 "MMIDM==> VDMC_GetVdmMan "
#define MMIDM_PL_MMI_1651_112_2_18_2_14_15_590 "MMIDM==> VDMC_GetDevImei "
#define MMIDM_PL_MMI_1665_112_2_18_2_14_15_591 "MMIDM==> DM_GetDeviceFwV "
#define MMIDM_PL_MMI_1681_112_2_18_2_14_15_592 "MMIDM==> DM_GetDeviceSwV "
#define MMIDM_PL_MMI_1696_112_2_18_2_14_15_593 "MMIDM==> DM_GetDeviceHwV "
#define MMIDM_PL_MMI_1711_112_2_18_2_14_15_594 "MMIDM==> DM_GetStringNULL "
#define MMIDM_PL_MMI_1725_112_2_18_2_14_15_595 "MMIDM==> DM_GetDmProfile "
#define MMIDM_PL_MMI_1738_112_2_18_2_14_16_596 "MMIDM==> DM_SetDmProfile "
#define MMIDM_PL_MMI_1745_112_2_18_2_14_16_597 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_1759_112_2_18_2_14_16_598 "MMIDM==> DM_GetGprsCMNetAPN "
#define MMIDM_PL_MMI_1773_112_2_18_2_14_16_599 "MMIDM==> DM_SetGprsCMNetAPN "
#define MMIDM_PL_MMI_1780_112_2_18_2_14_16_600 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_1794_112_2_18_2_14_16_601 "MMIDM==> DM_GetGprsCMNetUsername "
#define MMIDM_PL_MMI_1807_112_2_18_2_14_16_602 "MMIDM==> DM_SetGprsCMNetUsername "
#define MMIDM_PL_MMI_1814_112_2_18_2_14_16_603 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_1828_112_2_18_2_14_16_604 "MMIDM==> DM_GetGprsCMNePw "
#define MMIDM_PL_MMI_1842_112_2_18_2_14_16_605 "MMIDM==> DM_SetGprsCMNetPw "
#define MMIDM_PL_MMI_1849_112_2_18_2_14_16_606 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_1863_112_2_18_2_14_16_607 "MMIDM==> DM_GetGprsCMNetProxyaddr "
#define MMIDM_PL_MMI_1875_112_2_18_2_14_16_608 "MMIDM==> DM_SetGprsCMNetProxyaddr "
#define MMIDM_PL_MMI_1882_112_2_18_2_14_16_609 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_1896_112_2_18_2_14_16_610 "MMIDM==> DM_GetGprsCMNetPortNum "
#define MMIDM_PL_MMI_1908_112_2_18_2_14_16_611 "MMIDM==> DM_SetGprsCMNetUsername "
#define MMIDM_PL_MMI_1915_112_2_18_2_14_16_612 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_1929_112_2_18_2_14_16_613 "MMIDM==> DM_GetGprsAPN "
#define MMIDM_PL_MMI_1941_112_2_18_2_14_16_614 "MMIDM==> DM_SetGprsAPN "
#define MMIDM_PL_MMI_1949_112_2_18_2_14_16_615 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_1962_112_2_18_2_14_16_616 "MMIDM==> DM_GetGprsUsername "
#define MMIDM_PL_MMI_1975_112_2_18_2_14_16_617 "MMIDM==> DM_SetGprsUsername "
#define MMIDM_PL_MMI_1983_112_2_18_2_14_16_618 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_1997_112_2_18_2_14_16_619 "MMIDM==> DM_GetGprsPw "
#define MMIDM_PL_MMI_2010_112_2_18_2_14_16_620 "MMIDM==> DM_SetGprsPw "
#define MMIDM_PL_MMI_2018_112_2_18_2_14_16_621 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2032_112_2_18_2_14_16_622 "MMIDM==> DM_GetGprsCMWapProxyaddr "
#define MMIDM_PL_MMI_2045_112_2_18_2_14_16_623 "MMIDM==> DM_SetGprsCMWapProxyaddr "
#define MMIDM_PL_MMI_2053_112_2_18_2_14_16_624 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2067_112_2_18_2_14_16_625 "MMIDM==> DM_GetGprsCMWapPortNum "
#define MMIDM_PL_MMI_2080_112_2_18_2_14_16_626 "MMIDM==> DM_SetGprsCMWapPortNum "
#define MMIDM_PL_MMI_2088_112_2_18_2_14_16_627 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2102_112_2_18_2_14_16_628 "MMIDM==> DM_GetMMSMmsc "
#define MMIDM_PL_MMI_2116_112_2_18_2_14_16_629 "MMIDM==> DM_SetMMSMmsc "
#define MMIDM_PL_MMI_2124_112_2_18_2_14_16_630 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2137_112_2_18_2_14_16_631 "MMIDM==> DM_GetMMSProfile "
#define MMIDM_PL_MMI_2150_112_2_18_2_14_16_632 "MMIDM==> DM_SetMMSProfile "
#define MMIDM_PL_MMI_2158_112_2_18_2_14_16_633 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2172_112_2_18_2_14_16_634 "MMIDM==> DM_GetWAPConprofile "
#define MMIDM_PL_MMI_2185_112_2_18_2_14_16_635 "MMIDM==> DM_SetWAPConprofile "
#define MMIDM_PL_MMI_2193_112_2_18_2_14_16_636 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2206_112_2_18_2_14_16_637 "MMIDM==> DM_GetWAPStarPage "
#define MMIDM_PL_MMI_2219_112_2_18_2_14_17_638 "MMIDM==> DM_SetWAPStarPage "
#define MMIDM_PL_MMI_2227_112_2_18_2_14_17_639 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2241_112_2_18_2_14_17_640 "MMIDM==> DM_GetPIMAddr "
#define MMIDM_PL_MMI_2254_112_2_18_2_14_17_641 "MMIDM==> DM_SetPIMAddr "
#define MMIDM_PL_MMI_2262_112_2_18_2_14_17_642 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2275_112_2_18_2_14_17_643 "MMIDM==> DM_GetPIMAddressBookURL "
#define MMIDM_PL_MMI_2288_112_2_18_2_14_17_644 "MMIDM==> DM_SetPIMAddressBookURL "
#define MMIDM_PL_MMI_2296_112_2_18_2_14_17_645 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2309_112_2_18_2_14_17_646 "MMIDM==> DM_GetPIMCalendarURL "
#define MMIDM_PL_MMI_2324_112_2_18_2_14_17_647 "MMIDM==> DM_GetSrvAddrURL "
#define MMIDM_PL_MMI_2335_112_2_18_2_14_17_648 "MMIDM==> DM_GetDeviceLockStatus"
#define MMIDM_PL_MMI_2348_112_2_18_2_14_17_649 "MMIDM==> DM_SetPIMCalendarURL "
#define MMIDM_PL_MMI_2356_112_2_18_2_14_17_650 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2370_112_2_18_2_14_17_651 "MMIDM==> DM_GetStreamingName "
#define MMIDM_PL_MMI_2383_112_2_18_2_14_17_652 "MMIDM==> DM_SetStreamingName "
#define MMIDM_PL_MMI_2391_112_2_18_2_14_17_653 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2405_112_2_18_2_14_17_654 "MMIDM==> DM_GetStreamingProxy "
#define MMIDM_PL_MMI_2417_112_2_18_2_14_17_655 "MMIDM==> DM_SetStreamingProxy "
#define MMIDM_PL_MMI_2425_112_2_18_2_14_17_656 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2439_112_2_18_2_14_17_657 "MMIDM==> DM_GetStreamingProxyPort "
#define MMIDM_PL_MMI_2451_112_2_18_2_14_17_658 "MMIDM==> DM_SetStreamingProxyPort "
#define MMIDM_PL_MMI_2459_112_2_18_2_14_17_659 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2472_112_2_18_2_14_17_660 "MMIDM==> DM_GetStreamingNapID "
#define MMIDM_PL_MMI_2484_112_2_18_2_14_17_661 "MMIDM==> DM_SetStreamingNapID "
#define MMIDM_PL_MMI_2492_112_2_18_2_14_17_662 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2505_112_2_18_2_14_17_663 "MMIDM==> DM_GetStreamingNetInfo "
#define MMIDM_PL_MMI_2517_112_2_18_2_14_17_664 "MMIDM==> DM_SetStreamingNetInfo "
#define MMIDM_PL_MMI_2525_112_2_18_2_14_17_665 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2538_112_2_18_2_14_17_666 "MMIDM==> DM_GetStreamingMinUDPPort "
#define MMIDM_PL_MMI_2550_112_2_18_2_14_17_667 "MMIDM==> DM_SetStreamingMinUDPPort "
#define MMIDM_PL_MMI_2558_112_2_18_2_14_17_668 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2571_112_2_18_2_14_17_669 "MMIDM==> DM_GetStreamingMaxUDPPort "
#define MMIDM_PL_MMI_2583_112_2_18_2_14_17_670 "MMIDM==> DM_SetStreamingName "
#define MMIDM_PL_MMI_2591_112_2_18_2_14_17_671 "MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
#define MMIDM_PL_MMI_2642_112_2_18_2_14_17_672 "MMIDM==>  DM_initiateSession type =%d"
#define MMIDM_PL_MMI_2698_112_2_18_2_14_17_673 "MMIDM==>  NIA_Plugin_handleAlert inUIMode =%d"
#define MMIDM_PL_MMI_2747_112_2_18_2_14_18_674 "MMIDM==> DM_DlResumeGetFunc %s"
#define MMIDM_PL_MMI_2751_112_2_18_2_14_18_675 "MMIDM==> DM_DlResumeGetFunc: Failed to allocate memory."
#define MMIDM_PL_MMI_2779_112_2_18_2_14_18_676 "MMIDM==>  DM_DlResumeGetFunc: Failed to open %s."
#define MMIDM_PL_MMI_2790_112_2_18_2_14_18_677 "MMIDM==>  DM_DlResumeGetFunc error =%d,read_size=%d,sizeof(s_vdm_resume) size=%d"
#define MMIDM_PL_MMI_2803_112_2_18_2_14_18_678 "MMIDM==>  DM_DlResumeGetFunc pResumeTag = %s, offset = %d."
#define MMIDM_PL_MMI_2846_112_2_18_2_14_18_679 "MMIDM==> DM_DLResumeSetFunc %s"
#define MMIDM_PL_MMI_2855_112_2_18_2_14_18_680 "MMIDM==>  DM_DLResumeSetFunc open file fail"
#define MMIDM_PL_MMI_2873_112_2_18_2_14_18_681 "MMIDM==>  DM_DLResumeSetFunc error =%d,write_size=%d,sizeof(s_vdm_resume) size=%d"
#define MMIDM_PL_MMI_2906_112_2_18_2_14_18_682 "MMIDM==> MMIDM_RemoveResumeFile "
#define MMIDM_PL_MMI_2922_112_2_18_2_14_18_683 "MMIDM==>  MMIDM_RemoveResumeFile: DL resume file %s result=%d"
#define MMIDM_PL_MMI_2932_112_2_18_2_14_18_684 "MMIDM==> MMIDM_DeleteAllResumeFile "
#define MMIDM_PL_MMI_2936_112_2_18_2_14_18_685 "MMIDM==> MMIDM_DeleteAllResumeFile "
#define MMIDM_PL_MMI_2992_112_2_18_2_14_18_686 "MMIDM==> DM_IsResumeFileExist "
#define MMIDM_PL_MMI_2999_112_2_18_2_14_18_687 "MMIDM==> %s"
#define MMIDM_PL_MMI_3036_112_2_18_2_14_18_688 "MMIDM==>  SendSignalToMMITask , sig_id is %x"
#define MMIDM_PL_MMI_3046_112_2_18_2_14_18_689 "MMIDM==>  DM_ConfirmPartiallyLockCB To do Lock operation"
#define MMIDM_PL_MMI_3058_112_2_18_2_14_18_690 "MMIDM==>  DM_ConfirmPartiallyLock do Lock operation"
#define MMIDM_PL_MMI_3069_112_2_18_2_14_18_691 "MMIDM==>  DM_ConfirmUnLockCB do UnLock operation"
#define MMIDM_PL_MMI_3081_112_2_18_2_14_18_692 "MMIDM==>  DM_ConfirmUnLock do UnLock operation"
#define MMIDM_PL_MMI_3092_112_2_18_2_14_18_693 "MMIDM==>  DM_ConfirmFactoryResetCB To do FactoryReset operation"
#define MMIDM_PL_MMI_3104_112_2_18_2_14_18_694 "MMIDM==>  DM_ConfirmFactoryReset To do FactoryReset operation"
#define MMIDM_PL_MMI_3131_112_2_18_2_14_18_695 "MMIDM==>  LAWMO_Plugin_init Enter, LAWMO_Plugin_init"
#define MMIDM_PL_MMI_3139_112_2_18_2_14_18_696 "MMIDM==>  LAWMO_Plugin_init = 0x%x"
#define MMIDM_PL_MMI_3158_112_2_18_2_14_18_697 "MMIDM==>  LAWMO_Plugin_term "
#define MMIDM_PL_MMI_3169_112_2_18_2_14_18_698 "MMIDM==>  DM_LawmoSetLockState: result= 0x%x"
#define MMIDM_PL_MMI_3197_112_2_18_2_14_18_699 "MMIDM==>  SCOMO confirmUninstall"
#define MMIDM_PL_MMI_3211_112_2_18_2_14_19_700 "MMIDM==>  DM_OnScomoReadyToRemove"
#define MMIDM_PL_MMI_3235_112_2_18_2_14_19_701 "MMIDM==> SCOMO_Plugin_initDCsall PNULL == pPlugin"
#define MMIDM_PL_MMI_3265_112_2_18_2_14_19_702 "MMIDM==>  SCOMO_Plugin_initDCsall name: %s"
#define MMIDM_PL_MMI_3283_112_2_18_2_14_19_703 "MMIDM==>  SCOMO_Plugin_initDCsall name"
#define MMIDM_PL_MMI_3297_112_2_18_2_14_19_704 "MMIDM==>  SCOMO_Plugin_initDCsall id: %s"
#define MMIDM_PL_MMI_3299_112_2_18_2_14_19_705 "MMIDM==>  SCOMO_Plugin_initDCsall name id=%d"
#define MMIDM_PL_MMI_3343_112_2_18_2_14_19_706 "MMIDM==> DM_ResumeScomoDLSess"
#define MMIDM_PL_MMI_3392_112_2_18_2_14_19_707 "MMIDM==>  DM_GetScomoResultCode: Unknow SCOMO result code!"
#define MMIDM_PL_MMI_3425_112_2_18_2_14_19_708 "MMIDM==> DM_SetDPInstallResult: result = %d"
#define MMIDM_PL_MMI_3429_112_2_18_2_14_19_709 "MMIDM==> DM_SetDPInstallResult PNULL"
#define MMIDM_PL_MMI_3437_112_2_18_2_14_19_710 "MMIDM==>  MMIDM_IU32toa"
#define MMIDM_PL_MMI_3443_112_2_18_2_14_19_711 "MMIDM==>  MMIDM_SCOMO_PL_INV_addComponent ret=0x%x"
#define MMIDM_PL_MMI_3452_112_2_18_2_14_19_712 "MMIDM==>  VDM_ERR_NODE_ALREADY_EXISTS del_ret =%d"
#define MMIDM_PL_MMI_3459_112_2_18_2_14_19_713 "MMIDM==>  VDM_ERR_NODE_ALREADY_EXISTS ret =%d"
#define MMIDM_PL_MMI_3463_112_2_18_2_14_19_714 "MMIDM==> DM_SetDPInstallResult Failed to add component to inventory! error = 0x%x"
#define MMIDM_PL_MMI_3469_112_2_18_2_14_19_715 "MMIDM==> DM_SetDPInstallResult added to inventory"
#define MMIDM_PL_MMI_3479_112_2_18_2_14_19_716 "MMIDM==> DM_SetDPInstallResult created instance"
#define MMIDM_PL_MMI_3486_112_2_18_2_14_19_717 "MMIDM==>  MMIDM_SCOMO_DP_setInstallationResult ret =0x%x"
#define MMIDM_PL_MMI_3492_112_2_18_2_14_19_718 "MMIDM==>  MMIDM_SCOMO_DP_setInstallationResult ret 1=0x%x"
#define MMIDM_PL_MMI_3509_112_2_18_2_14_19_719 "MMIDM==>  DM_RemoveDCFromInventory %d"
#define MMIDM_PL_MMI_3513_112_2_18_2_14_19_720 "MMIDM==>  DM_RemoveDCFromInventory PNULL"
#define MMIDM_PL_MMI_3529_112_2_18_2_14_19_721 "MMIDM==> DM_RemoveDCFromInventory: Selected DC has been removed.1"
#define MMIDM_PL_MMI_3538_112_2_18_2_14_19_722 "MMIDM==>  DM_RemoveDCFromInventory p=%x"
#define MMIDM_PL_MMI_3557_112_2_18_2_14_19_723 "MMIDM==> DM_RemoveDCFromInventory: Selected DC has been removed.2"
#define MMIDM_PL_MMI_3561_112_2_18_2_14_19_724 "MMIDM==> DM_RemoveDCFromInventory: Cannot find selected DC in inventory."
#define MMIDM_PL_MMI_3584_112_2_18_2_14_19_725 "MMIDM==>  SCOMO_Plugin_create"
#define MMIDM_PL_MMI_3597_112_2_18_2_14_19_726 "MMIDM==>  SCOMO_Plugin_term"
#define MMIDM_PL_MMI_3616_112_2_18_2_14_19_727 "MMIDM==> SCOMO_Plugin_term temp=%x"
#define MMIDM_PL_MMI_3622_112_2_18_2_14_19_728 "MMIDM==> SCOMO_Plugin_term destroy dp"
#define MMIDM_PL_MMI_3626_112_2_18_2_14_19_729 "MMIDM==>  SCOMO_Plugin_term instance->scomoDP instance->scomoDP %x"
#define MMIDM_PL_MMI_3630_112_2_18_2_14_19_730 "MMIDM==> SCOMO_Plugin_term 2"
#define MMIDM_PL_MMI_3633_112_2_18_2_14_19_731 "MMIDM==> SCOMO_Plugin_term 3"
#define MMIDM_PL_MMI_3637_112_2_18_2_14_19_732 "MMIDM==>  SCOMO_Plugin_term done"
#define MMIDM_PL_MMI_3646_112_2_18_2_14_19_733 "MMIDM==>  MMIDM_GetDPHeaderLength =%d"
#define MMIDM_PL_MMI_3656_112_2_18_2_14_19_734 "MMIDM==> downloadType = %d"
#define MMIDM_PL_MMI_3719_112_2_18_2_14_20_735 "MMIDM==> MMIDM_SetDPHeader PNULL == header"
#define MMIDM_PL_MMI_3753_112_2_18_2_14_20_736 "MMIDM==> MMIDM_IsBackgroupRun s_UIMode = %d result %d"
#define MMIDM_PL_MMI_3770_112_2_18_2_14_20_737 "MMIDM==> MMIDM_IsNeedDisplayProgresswin alert_type = %d result %d"
#define MMIDM_PL_OS_143_112_2_18_2_14_20_738 "MMIDM==> VDM_Client_PL_Dlpkg_allocNameFromId"
#define MMIDM_PL_OS_221_112_2_18_2_14_20_739 "MMIDM==> VDM_Client_PL_assertFail %s,%d, line=%ld"
#define MMIDM_PL_OS_254_112_2_18_2_14_20_740 "MMIDM==>VDM_Client_PL_Dlpkg_create PNULL == outDlpkgHandle"
#define MMIDM_PL_OS_259_112_2_18_2_14_20_741 "MMIDM==> VDM_Client_PL_Dlpkg_create: outDlpkgHandle = %s "
#define MMIDM_PL_OS_269_112_2_18_2_14_20_742 "MMIDM==> VDM_Client_PL_Dlpkg_create: len=%d, inURI= %s "
#define MMIDM_PL_OS_275_112_2_18_2_14_20_743 "MMIDM==> VDM_Client_PL_Dlpkg_create: inurl_ptr1= %s "
#define MMIDM_PL_OS_278_112_2_18_2_14_20_744 "MMIDM==> VDM_Client_PL_Dlpkg_create: inurl_ptr2= %s "
#define MMIDM_PL_OS_279_112_2_18_2_14_20_745 "MMIDM==> VDM_Client_PL_Dlpkg_create: file_name_ptr= %s "
#define MMIDM_PL_OS_300_112_2_18_2_14_21_746 "MMIDM==> VDM_Client_PL_Dlpkg_create: file_fullname_ptr= %s "
#define MMIDM_PL_OS_330_112_2_18_2_14_21_747 "MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: inOffset = %d, inDataSize=%d "
#define MMIDM_PL_OS_344_112_2_18_2_14_21_748 "MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: filename_ptr = %s"
#define MMIDM_PL_OS_356_112_2_18_2_14_21_749 "MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: error=%d "
#define MMIDM_PL_OS_363_112_2_18_2_14_21_750 "MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: 0 == file_handle "
#define MMIDM_PL_OS_370_112_2_18_2_14_21_751 "MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: SFS_SEEK error "
#define MMIDM_PL_OS_377_112_2_18_2_14_21_752 "MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: Write writelen %d"
#define MMIDM_PL_OS_383_112_2_18_2_14_21_753 "MMIDM==> VDM_Client_PL_Dlpkg_writeChunk writelen= %d ,result=0x%x"
#define MMIDM_PL_OS_407_112_2_18_2_14_21_754 "MMIDM==> VDM_Client_PL_Dlpkg_remove"
#define MMIDM_PL_OS_435_112_2_18_2_14_21_755 "MMIDM==> VDM_Client_PL_Dlpkg_remove result=%d"
#define MMIDM_PL_OS_449_112_2_18_2_14_21_756 "MMIDM==> VDM_Client_PL_Dlpkg_getMaxSize: result "
#define MMIDM_PL_OS_515_112_2_18_2_14_21_757 "MMIDM==> Reg_Save"
#define MMIDM_PL_OS_520_112_2_18_2_14_21_758 "MMIDM==> Reg_Save PNULL == filename_ptr"
#define MMIDM_PL_OS_535_112_2_18_2_14_21_759 "MMIDM==> Reg_Save error =%d,write_size=%d,sizeof(MMIDM_REG_T) size=%d"
#define MMIDM_PL_OS_541_112_2_18_2_14_21_760 "MMIDM==> Reg_Save open file fail"
#define MMIDM_PL_OS_564_112_2_18_2_14_21_761 "MMIDM==> Reg_Init"
#define MMIDM_PL_OS_572_112_2_18_2_14_21_762 "MMIDM==> Reg_Init PNULL == s_vdm_reg_ptr"
#define MMIDM_PL_OS_581_112_2_18_2_14_21_763 "MMIDM==> Reg_Init PNULL == filename_ptr"
#define MMIDM_PL_OS_596_112_2_18_2_14_21_764 "MMIDM==> Reg_init error =%d,read_size=%d,sizeof(MMIDM_REG_T) size=%d"
#define MMIDM_PL_OS_603_112_2_18_2_14_21_765 "MMIDM==> Reg_Init open file fail"
#define MMIDM_PL_OS_607_112_2_18_2_14_21_766 "MMIDM==> Reg_Init end"
#define MMIDM_PL_OS_619_112_2_18_2_14_21_767 "MMIDM==> Reg_Uninit s_vdm_reg_ptr=0x%x"
#define MMIDM_PL_OS_637_112_2_18_2_14_21_768 "MMIDM==> VDM_Client_PL_Registry_createKey "
#define MMIDM_PL_OS_659_112_2_18_2_14_21_769 "MMIDM==> VDM_Client_PL_Registry_setStringValue,inValueName=%s, inData=%s\n"
#define MMIDM_PL_OS_667_112_2_18_2_14_22_770 "MMIDM==> VDM_Client_PL_Registry_setStringValue,s=%s\n"
#define MMIDM_PL_OS_718_112_2_18_2_14_22_771 "MMIDM==> VDM_Client_PL_Registry_setStringValue,key_index=%d"
#define MMIDM_PL_OS_738_112_2_18_2_14_22_772 "MMIDM==> VDM_Client_PL_Registry_getStringValue,inKey=%s\n"
#define MMIDM_PL_OS_744_112_2_18_2_14_22_773 "MMIDM==> VDM_Client_PL_Registry_getStringValue,result=%x\n"
#define MMIDM_PL_OS_747_112_2_18_2_14_22_774 "MMIDM==> VDM_Client_PL_Registry_getStringValue,inValueName=%s\n"
#define MMIDM_PL_OS_756_112_2_18_2_14_22_775 "MMIDM==> VDM_Client_PL_Registry_getStringValue,result=%x, *ioLength=%d\n"
#define MMIDM_PL_OS_762_112_2_18_2_14_22_776 "MMIDM==> VDM_Client_PL_Registry_getStringValue,s=%s\n"
#define MMIDM_PL_OS_768_112_2_18_2_14_22_777 "MMIDM==> VDM_Client_PL_Registry_getStringValue,key_index = %d,reg_ptr->reg_val[key_index]0x%x\n"
#define MMIDM_PL_OS_771_112_2_18_2_14_22_778 "MMIDM==> VDM_Client_PL_Registry_getStringValue,key_index =%d,reg_ptr->reg_val[key_index]=%s\n"
#define MMIDM_PL_OS_773_112_2_18_2_14_22_779 "MMIDM==> VDM_Client_PL_Registry_getStringValue 11,*ioLength =%d,outLen=%d\n"
#define MMIDM_PL_OS_779_112_2_18_2_14_22_780 "MMIDM==> VDM_Client_PL_Registry_getStringValue 22,*ioLength =%d,outLen=%d\n"
#define MMIDM_PL_OS_791_112_2_18_2_14_22_781 "MMIDM==> VDM_Client_PL_Registry_getStringValue,result=%x"
#define MMIDM_PL_OS_810_112_2_18_2_14_22_782 "MMIDM==> VDM_Client_PL_Registry_setIntValue inKey=%s,inValueName=%s,inValue =%d"
#define MMIDM_PL_OS_814_112_2_18_2_14_22_783 "MMIDM==> VDM_Client_PL_Registry_setIntValue strValue =%s"
#define MMIDM_PL_OS_838_112_2_18_2_14_22_784 "MMIDM==> VDM_Client_PL_Registry_getIntValue "
#define MMIDM_PL_OS_840_112_2_18_2_14_22_785 "MMIDM==> VDM_Client_PL_Registry_getIntValue len=%d,strValue=%s"
#define MMIDM_PL_OS_849_112_2_18_2_14_22_786 "MMIDM==> VDM_Client_PL_Registry_getIntValue ret=%d,bSuccess=%d"
#define MMIDM_PL_OS_861_112_2_18_2_14_22_787 "MMIDM==> GetItemFilename inType = %d"
#define MMIDM_PL_OS_887_112_2_18_2_14_22_788 "MMIDM==> GetItemTmpFilename: inType = %d"
#define MMIDM_PL_OS_930_112_2_18_2_14_22_789 "MMIDM==> VDM_Client_PL_Storage_open inType=%d,inMode=%d"
#define MMIDM_PL_OS_936_112_2_18_2_14_22_790 "MMIDM==> VDM_Client_PL_Storage_open ! PNULL == storage_item_ptr"
#define MMIDM_PL_OS_947_112_2_18_2_14_22_791 "MMIDM==> VDM_Client_PL_Storage_open ! PNULL == filename_ptr"
#define MMIDM_PL_OS_957_112_2_18_2_14_22_792 "MMIDM==> VDM_Client_PL_Storage_open ! PNULL == tempfilename_ptr"
#define MMIDM_PL_OS_991_112_2_18_2_14_22_793 "MMIDM==> Cannot open file !"
#define MMIDM_PL_OS_1033_112_2_18_2_14_22_794 "MMIDM==> VDM_Client_PL_Storage_read PNULL ==stream_ptr"
#define MMIDM_PL_OS_1050_112_2_18_2_14_22_795 "MMIDM==> VDM_Client_PL_Storage_read VDM_ERR_STORAGE_READ"
#define MMIDM_PL_OS_1087_112_2_18_2_14_22_796 "MMIDM==> VDM_Client_PL_Storage_write VDM_ERR_STORAGE_WRITE 1"
#define MMIDM_PL_OS_1101_112_2_18_2_14_22_797 "MMIDM==> VDM_Client_PL_Storage_write VDM_ERR_STORAGE_WRITE 2"
#define MMIDM_PL_OS_1130_112_2_18_2_14_23_798 "MMIDM==> VDM_Client_PL_Storage_close "
#define MMIDM_PL_OS_1134_112_2_18_2_14_23_799 "MMIDM==> VDM_Client_PL_Storage_close VDM_ERR_STORAGE_WRITE"
#define MMIDM_PL_OS_1152_112_2_18_2_14_23_800 "MMIDM==> VDM_Client_PL_Storage_close filename_ptr"
#define MMIDM_PL_OS_1163_112_2_18_2_14_23_801 "MMIDM==> VDM_Client_PL_Storage_close tempfilename_ptr"
#define MMIDM_PL_OS_1199_112_2_18_2_14_23_802 "MMIDM==> VDM_Client_PL_Storage_delete inType=%d"
#define MMIDM_PL_OS_1202_112_2_18_2_14_23_803 "MMIDM==> VDM_Client_PL_Storage_delete PNULL == filename"
#define MMIDM_PL_OS_1209_112_2_18_2_14_23_804 "MMIDM==> VDM_Client_PL_Storage_delete PNULL == wfilename_ptr"
#define MMIDM_PL_OS_1219_112_2_18_2_14_23_805 "MMIDM==> VDM_Client_PL_Storage_delete Could not delete wfilename_ptr"
#define MMIDM_PL_OS_1226_112_2_18_2_14_23_806 "MMIDM==> VDM_Client_PL_Storage_delete PNULL == wtempfilename_ptr"
#define MMIDM_PL_OS_1242_112_2_18_2_14_23_807 "MMIDM==> VDM_Client_PL_Storage_delete Could not delete tempfilename_ptr"
#define MMIDM_PL_OS_1326_112_2_18_2_14_23_808 "MMIDM==> VDM_PL_Mutex_create "
#define MMIDM_PL_OS_1354_112_2_18_2_14_23_809 "MMIDM==> VDM_PL_Mutex_lock fail"
#define MMIDM_PL_OS_1377_112_2_18_2_14_23_810 "MMIDM==> VDM_PL_Mutex_unlock fail"
#define MMIDM_PL_OS_1397_112_2_18_2_14_23_811 "MMIDM==> VDM_PL_Mutex_close success"
#define MMIDM_PL_OS_1420_112_2_18_2_14_23_812 "MMIDM==> VDM_PL_Async_signal "
#define MMIDM_PL_OS_1435_112_2_18_2_14_23_813 "MMIDM==> VDM_PL_Async_yield "
#define MMIDM_PL_OS_1451_112_2_18_2_14_23_814 "MMIDM==> VDM_Client_PL_logInit "
#define MMIDM_PL_OS_1468_112_2_18_2_14_23_815 "MMIDM==> VDM_Client_PL_logTerm "
#define MMIDM_PL_OS_1508_112_2_18_2_14_23_816 "MMIDM==> VDMLIB:%s"
#define MMIDM_PL_OS_1536_112_2_18_2_14_23_817 "MMIDM==> VDMLIB:%s"
#define MMIDM_PL_OS_1549_112_2_18_2_14_23_818 "MMIDM==> VDMLIB:%s"
#define MMIDM_PL_OS_1561_112_2_18_2_14_23_819 "MMIDM==> VDMLIB:%s"
#define MMIDM_PL_OS_1582_112_2_18_2_14_23_820 "MMIDM==> 0x%x:%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x "
#define MMIDM_PL_OS_1585_112_2_18_2_14_23_821 "MMIDM==> %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
#define MMIDM_PL_OS_1598_112_2_18_2_14_23_822 "MMIDM==> VDM_Notify_PL_Task_Finished "
#define MMIDM_PL_STRING_196_112_2_18_2_14_24_823 "MMIDM VDM_PL_IU32toa PNULL == string"
#define MMIDM_TASK_82_112_2_18_2_14_25_824 "MMIDM==> HandleDMPDPMsg PNULL == msg_ptr"
#define MMIDM_TASK_85_112_2_18_2_14_25_825 "MMIDM==> HandleDMPDPMsg msg_id=%d"
#define MMIDM_TASK_111_112_2_18_2_14_25_826 "MMIDM==> MMIDM_GetPdpConnectType %d"
#define MMIDM_TASK_124_112_2_18_2_14_25_827 "MMIDM==> SetPdpConnectType CONNECT_UNKNOW"
#define MMIDM_TASK_139_112_2_18_2_14_25_828 "MMIDM==> SetPdpConnectType = %d"
#define MMIDM_TASK_154_112_2_18_2_14_25_829 "MMIDM==> MMIDM_ActivePdpContext dm_apn=%s"
#define MMIDM_TASK_174_112_2_18_2_14_25_830 "MMIDM==> MMIDM_ActivePdpContext fail"
#define MMIDM_TASK_191_112_2_18_2_14_26_831 "MMIDM==> MMIAPIPDP_Deactive fail!"
#define MMIDM_TASK_230_112_2_18_2_14_26_832 "MMIDM==> MMIDM_ActivePdpConnect TIME OUT"
#define MMIDM_TASK_248_112_2_18_2_14_26_833 "MMIDM==> pdp active result =%d"
#define MMIDM_TASK_279_112_2_18_2_14_26_834 "DM:task start!"
#define MMIDM_TASK_286_112_2_18_2_14_26_835 "MMIDM==> task cycle, signal code = 0x%x"
#define MMIDM_TASK_291_112_2_18_2_14_26_836 "MMIDM==> task DM_TASK_START_MESSAGE "
#define MMIDM_TASK_296_112_2_18_2_14_26_837 "MMIDM==> task DM_TASK_START_MESSAGE result=%d"
#define MMIDM_TASK_319_112_2_18_2_14_26_838 "MMIDM==> task DM_TASK_RUN_MESSAGE "
#define MMIDM_TASK_331_112_2_18_2_14_26_839 "MMIDM==> task DM_TASK_DM_CANCEL "
#define MMIDM_TASK_335_112_2_18_2_14_26_840 "MMIDM==> task DM_TASK_DM_CLOSE "
#define MMIDM_TASK_360_112_2_18_2_14_26_841 "MMIDM==> task DM_TASK_DM_EXIT "
#define MMIDM_TASK_380_112_2_18_2_14_26_842 "MMIDM==> task DM_TASK_EXIT_MESSAGE "
#define MMIDM_TASK_403_112_2_18_2_14_26_843 "MMIDM==> task SOCKET_READ_EVENT_IND "
#define MMIDM_TASK_408_112_2_18_2_14_26_844 "MMIDM==> task SOCKET_WRITE_EVENT_IND "
#define MMIDM_TASK_413_112_2_18_2_14_26_845 "MMIDM==> task SOCKET_CONNECT_EVENT_IND "
#define MMIDM_TASK_451_112_2_18_2_14_26_846 "MMIDM==> DM_CreateTask, task hasn't been closed!"
#define MMIDM_TASK_467_112_2_18_2_14_26_847 "MMIDM==> DM_CreateTask, task id = %d"
#define MMIDM_TASK_487_112_2_18_2_14_26_848 "MMIDM==> get pdp status = %d"
#define MMIDM_TASK_496_112_2_18_2_14_26_849 "MMIDM==> set pdp status = %d"
#define MMIDM_WINTAB_624_112_2_18_2_14_28_850 "MMIDM==> enter HandleDmDebugSettingMenuWinMsg "
#define MMIDM_WINTAB_886_112_2_18_2_14_28_851 "HandleDmSIMSelectWinMsg,item_data[%d] >= MMI_DUAL_SYS_MAX,ERROR!"
#define MMIDM_WINTAB_1109_112_2_18_2_14_29_852 "MMIDM==>  HandleDmMainMenuWinMsg: msg_id = 0x%x"
#define MMIDM_WINTAB_1162_112_2_18_2_14_29_853 "MMIDM==>  OpenDlAlerthandleWin  mode =%d"
#define MMIDM_WINTAB_1261_112_2_18_2_14_29_854 "MMIDM==> MMIDM_HandWapClose"
#define MMIDM_WINTAB_1279_112_2_18_2_14_29_855 "MMIDM==> MMIDM_HandDmEventClose"
#define MMIDM_WINTAB_1349_112_2_18_2_14_29_856 "MMIDM==>  PlayDmNotifyRing  dual_sys =%d"
#define MMIDM_WINTAB_1407_112_2_18_2_14_29_857 "MMIDM==>  ShowVersionInfo "
#define MMIDM_WINTAB_1504_112_2_18_2_14_30_858 "MMIDM==>  HandleDmSWQueryWin: msg_id = 0x%x"
#define MMIDM_WINTAB_1534_112_2_18_2_14_30_859 "MMIDM==> SoftWareUpdate "
#define MMIDM_WINTAB_1563_112_2_18_2_14_30_860 "MMIDM==> SoftWareUpdate alert_type = %d"
#define MMIDM_WINTAB_1591_112_2_18_2_14_30_861 "MMIDM==> SoftWareUpdate start"
#define MMIDM_WINTAB_1638_112_2_18_2_14_30_862 "MMIDM==>  HandleDmInstallQueryWin: msg_id = 0x%x"
#define MMIDM_WINTAB_1682_112_2_18_2_14_30_863 "MMIDM==>  HandleDmFumoUpdateSuccWin: msg_id = 0x%x"
#define MMIDM_WINTAB_1731_112_2_18_2_14_30_864 "MMIDM==> HandleWaitWinMsg, win_id =%d, msg_id = %d."
#define MMIDM_WINTAB_1801_112_2_18_2_14_30_865 "MMIDM==> MMIDM_ProgressUpdateCallBack inCurrentProgress=%d,inMaxProgress=%d"
#define MMIDM_WINTAB_1828_112_2_18_2_14_30_866 "MMIDM==> MMIDM_ProgressUpdateCallBack PNULL==data"
#define MMIDM_WINTAB_1892_112_2_18_2_14_30_867 "MMIDM==> MMIDM_ServerLockCallBack :LOCK"
#define MMIDM_WINTAB_1937_112_2_18_2_14_30_868 "MMIDM==> MMIDM_ServerUnlockCallBack: UNLOCK"
#define MMIDM_WINTAB_2012_112_2_18_2_14_31_869 "MMIDM==> HandleDMServerLockWinMsg: 0x%x"
#define MMIDM_WINTAB_2084_112_2_18_2_14_31_870 "MMIDM==>  HandleDmSessionChange %d"
#define MMIDM_WINTAB_2120_112_2_18_2_14_31_871 "MMIDM==>  HandleDmSessionChange PNULL==data"
#define MMIDM_WINTAB_2143_112_2_18_2_14_31_872 "MMIDM==>  HandleDlSessionChange %d"
#define MMIDM_WINTAB_2165_112_2_18_2_14_31_873 "MMIDM==>  HandleDlSessionChange PNULL==data"
#define MMIDM_WINTAB_2176_112_2_18_2_14_31_874 "MMIDM==>  MMIDM_SessionChangeCallBack PNULL == data"
#define MMIDM_WINTAB_2180_112_2_18_2_14_31_875 "MMIDM==>  MMIDM_SessionChangeCallBack inState = %d,inType = %d"
#define MMIDM_WINTAB_2206_112_2_18_2_14_31_876 "MMIDM==> MMIDM_NotifyAlertCallBack PNULL == data"
#define MMIDM_WINTAB_2320_112_2_18_2_14_31_877 "MMIDM==>  HandleDmAlertConfirmWin: msg_id = 0x%x"
#define MMIDM_WINTAB_2381_112_2_18_2_14_31_878 "MMIDM==>  HandleDmAlertDisplayWin msg_id = 0x%x"
#define MMIDM_WINTAB_2428_112_2_18_2_14_31_879 "MMIDM==>  MMIDM_HandleUserSelectOption alert_mode =%d msg_id=0x%x"
#define MMIDM_WINTAB_2464_112_2_18_2_14_31_880 "MMIDM==> HandleAlertPlDisplay %d"
#define MMIDM_WINTAB_2486_112_2_18_2_14_32_881 "MMIDM==> HandleAlertPlDisplay observer_ptr== PNULL"
#define MMIDM_WINTAB_2498_112_2_18_2_14_32_882 "MMIDM==> HandleAlertPlConfirm %d"
#define MMIDM_WINTAB_2536_112_2_18_2_14_32_883 "MMIDM==> HandleAlertPlConfirm observer_ptr== PNULL"
#define MMIDM_WINTAB_2553_112_2_18_2_14_32_884 "MMIDM==> HandleAlertNIAConfirm"
#define MMIDM_WINTAB_2579_112_2_18_2_14_32_885 "MMIDM==> HandleAlertFUMOConfirm"
#define MMIDM_WINTAB_2619_112_2_18_2_14_32_886 "MMIDM==> HandleAlertScomoConfirm"
#define MMIDM_WINTAB_2657_112_2_18_2_14_32_887 "MMIDM==> MMIDM_IsDmTimerId "
#define MMIDM_WINTAB_2697_112_2_18_2_14_32_888 "MMIDM==> MMIDM_HandleDmTimer %d"
#define MMIDM_WINTAB_2700_112_2_18_2_14_32_889 "MMIDM==> MMIDM_HandleDmTimer s_mmidm_fumo_update_timerid in"
#define MMIDM_WINTAB_2713_112_2_18_2_14_32_890 "MMIDM==> MMIDM_HandleDmTimer s_mmidm_run_timerid in"
#define MMIDM_WINTAB_2724_112_2_18_2_14_32_891 "MMIDM==> MMIDM_HandleDmTimer s_mmidm_no_signal_timerid in"
#define MMIDM_WINTAB_2747_112_2_18_2_14_32_892 "MMIDM==> MMIDM_HandleDmTimer s_mmidm_delay_timerid in"
#define MMIDM_WINTAB_2765_112_2_18_2_14_32_893 "MMIDM==> MMIDM_HandleDmTimer s_mmidm_dis_confim_timerid in"
#define MMIDM_WINTAB_2788_112_2_18_2_14_32_894 "MMIDM==> MMIDM_HandleDmTimer s_mmidm_fota_report_timerid in"
#define MMIDM_WINTAB_2798_112_2_18_2_14_32_895 "MMIDM==> MMIDM_HandleDmTimer s_mmidm_choice_check_timerid in"
#define MMIDM_WINTAB_2817_112_2_18_2_14_32_896 "MMIDM==> MMIDM_HandleDmTimer s_mmidm_socket_timerid in"
#define MMIDM_WINTAB_2852_112_2_18_2_14_32_897 "MMIDM==>  HandleDmResumeQueryWin: msg_id = 0x%x"
#define MMIDM_WINTAB_2891_112_2_18_2_14_32_898 "MMIDM==>  HandleReDmResumeQueryWin: msg_id = 0x%x"
#define MMIDM_WINTAB_2929_112_2_18_2_14_32_899 "MMIDM==> lock session\n"
#define MMIDM_WINTAB_2953_112_2_18_2_14_32_900 "MMIDM==> update session\n"
#define MMIDM_WINTAB_2973_112_2_18_2_14_32_901 "MMIDM==> CreateDispalyConfimCheckTimer timer=%d"
#define MMIDM_WINTAB_2988_112_2_18_2_14_33_902 "MMIDM==> CreateDispalyConfimCheckTimer timer_num=%d s_mmidm_dis_confim_timerid=%d"
#define MMIDM_WINTAB_3011_112_2_18_2_14_33_903 "MMIDM==> CreateChoiceListCheckTimer timer=%d"
#define MMIDM_WINTAB_3026_112_2_18_2_14_33_904 "MMIDM==> CreateDispalyConfimCheckTimer timer_num=%d s_mmidm_choice_check_timerid=%d"
#define MMIDM_WINTAB_3049_112_2_18_2_14_33_905 "MMIDM==> MMIDM_CreateDelayTimer timer=%d"
#define MMIDM_WINTAB_3220_112_2_18_2_14_33_906 "MMIDM==> OpenDlChoiceListQueryWin mode=%d"
#define MMIDM_WINTAB_3232_112_2_18_2_14_33_907 "MMIDM==> OpenDlChoiceListQueryWin error mode!"
#define MMIDM_WINTAB_3252_112_2_18_2_14_33_908 "MMIDM: HandleChiceListQueryWinMsg msg_id = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_DM_TRC)
TRACE_MSG(MMIDM_APP_458_112_2_18_2_13_45_0,"MMIDM==> IsDmWapPush content[4]= 0x%x,content[5]= 0x%x,content[6]= 0x%x")
TRACE_MSG(MMIDM_APP_468_112_2_18_2_13_45_1,"MMIDM==>IsDmWapPush sms[%d] = %x")
TRACE_MSG(MMIDM_APP_476_112_2_18_2_13_45_2,"MMIDM==>IsDmWapPush session_id buf %s")
TRACE_MSG(MMIDM_APP_481_112_2_18_2_13_45_3,"MMIDM==>IsDmWapPush alert_id  %d")
TRACE_MSG(MMIDM_APP_511_112_2_18_2_13_45_4,"MMIDM==> MMIAPIDM_HandleSmsMtInd:")
TRACE_MSG(MMIDM_APP_520_112_2_18_2_13_45_5,"MMIDM==> reply of self register sms,and write IMSI to NV success!")
TRACE_MSG(MMIDM_APP_526_112_2_18_2_13_45_6,"MMIDM==> sms_is_wappush = %d")
TRACE_MSG(MMIDM_APP_547_112_2_18_2_13_45_7,"MMIDM==> data[5]=0x%x,data[6]=0x%x,data[7]=0x%x")
TRACE_MSG(MMIDM_APP_549_112_2_18_2_13_45_8,"MMIDM==> Start signal dm_ is_start len = %d")
TRACE_MSG(MMIDM_APP_566_112_2_18_2_13_45_9,"MMIDM==> Start signal dm status = %d")
TRACE_MSG(MMIDM_APP_626_112_2_18_2_13_46_10,"MMIDM==> DevRecvSRHandle: msg = %s")
TRACE_MSG(MMIDM_APP_631_112_2_18_2_13_46_11,"MMIDM==> DevRecvSRHandle server_id=%s\n")
TRACE_MSG(MMIDM_APP_639_112_2_18_2_13_46_12,"MMIDM==> DevRecvSRHandle---wrong SMS text received\n")
TRACE_MSG(MMIDM_APP_665_112_2_18_2_13_46_13,"MMIDM==> ----------GetDmServerSmscNumber  dmSrvID= %s---------\n")
TRACE_MSG(MMIDM_APP_668_112_2_18_2_13_46_14,"MMIDM==> ----------GetDmServerSmscNumber  dmSrvID= %s---------\n")
TRACE_MSG(MMIDM_APP_692_112_2_18_2_13_46_15,"MMIDM==> --DevRecvSelfRegisterSMS--SRSMS=%s, SRSMSOK=%s--\n,SRSMSERROR=%s--\n")
TRACE_MSG(MMIDM_APP_704_112_2_18_2_13_46_16,"MMIDM==> ---------the device register ok--------\n")
TRACE_MSG(MMIDM_APP_713_112_2_18_2_13_46_17,"MMIDM==> ----------failed to register the device---\n")
TRACE_MSG(MMIDM_APP_801_112_2_18_2_13_46_18,"MMIDM==> get is sr ready = %d")
TRACE_MSG(MMIDM_APP_811_112_2_18_2_13_46_19,"MMIDM==> set is sr ready = %d")
TRACE_MSG(MMIDM_APP_822_112_2_18_2_13_46_20,"MMIDM==> get test server type = %d")
TRACE_MSG(MMIDM_APP_832_112_2_18_2_13_46_21,"MMIDM==> set test server type = %d")
TRACE_MSG(MMIDM_APP_855_112_2_18_2_13_46_22,"MMIDM==> MMIDM_SendSelfRegisterSMS : sms=%d,net=%d")
TRACE_MSG(MMIDM_APP_859_112_2_18_2_13_46_23,"MMIDM==> -------MMIDM_SendSelfRegisterSMS--------\n")
TRACE_MSG(MMIDM_APP_862_112_2_18_2_13_46_24,"MMIDM==> --------SIM CARD NOT exist-------\n")
TRACE_MSG(MMIDM_APP_867_112_2_18_2_13_46_25,"MMIDM==> --------SIM CARD NOT ready-------\n")
TRACE_MSG(MMIDM_APP_872_112_2_18_2_13_46_26,"MMIDM==> --------NOT CMCC SIM CARD--------\n")
TRACE_MSG(MMIDM_APP_885_112_2_18_2_13_46_27,"MMIDM==>--------the device have registered--------\n")
TRACE_MSG(MMIDM_APP_891_112_2_18_2_13_46_28,"MMIDM==>--------the device have registered fail-----\n")
TRACE_MSG(MMIDM_APP_902_112_2_18_2_13_46_29,"MMIDM==> ----------SRSMS devIMEI = %s---------\n")
TRACE_MSG(MMIDM_APP_924_112_2_18_2_13_46_30,"MMIDM==> MMIDM_SendSelfRegisterSMS fail!")
TRACE_MSG(MMIDM_APP_960_112_2_18_2_13_46_31,"MMIDM==> ----------SendSelfRegisterSms ---------\n")
TRACE_MSG(MMIDM_APP_963_112_2_18_2_13_46_32,"MMIDM==>SR_Msg = %s \n")
TRACE_MSG(MMIDM_APP_986_112_2_18_2_13_46_33,"MMIDM==> SendSelfRegisterSms length = %d \n")
TRACE_MSG(MMIDM_APP_992_112_2_18_2_13_46_34,"MMIDM==> MMISMS_AppSendSmsForDm reg_times = %d send result =%d\n")
TRACE_MSG(MMIDM_APP_1044_112_2_18_2_13_46_35,"MMIDM==>  in Param_init() enter")
TRACE_MSG(MMIDM_APP_1133_112_2_18_2_13_47_36,"MMIDM==> in Param_init() enter lock=%d")
TRACE_MSG(MMIDM_APP_1222_112_2_18_2_13_47_37,"MMIDM==> DM_ParamInit  alert_type %d")
TRACE_MSG(MMIDM_APP_1223_112_2_18_2_13_47_38,"MMIDM==>  in devParam_init() end")
TRACE_MSG(MMIDM_APP_1240_112_2_18_2_13_47_39,"MMIDM==> s_dm_debug_info.debug_mode = %d")
TRACE_MSG(MMIDM_APP_1250_112_2_18_2_13_47_40,"MMIDM==> s_dm_debug_info.reg_times = %d")
TRACE_MSG(MMIDM_APP_1271_112_2_18_2_13_47_41,"MMIDM==> s_dm_debug_info.dm_reg_switch = %d")
TRACE_MSG(MMIDM_APP_1284_112_2_18_2_13_47_42,"MMIDM==>--enter into GetServePortNumber!!---\n")
TRACE_MSG(MMIDM_APP_1334_112_2_18_2_13_47_43,"MMIDM==>--------DM_MVA IsNeedSelfRegister--------\n")
TRACE_MSG(MMIDM_APP_1344_112_2_18_2_13_47_44,"MMIDM==>: sr switch on")
TRACE_MSG(MMIDM_APP_1349_112_2_18_2_13_47_45,"MMIDM==>: sr switch off")
TRACE_MSG(MMIDM_APP_1361_112_2_18_2_13_47_46,"MMIDM==> MMIDM_GetMan %s")
TRACE_MSG(MMIDM_APP_1371_112_2_18_2_13_47_47,"MMIDM==> MMIDM_GetModel %s")
TRACE_MSG(MMIDM_APP_1391_112_2_18_2_13_47_48,"MMIDM==> version %s")
TRACE_MSG(MMIDM_APP_1562_112_2_18_2_13_48_49,"MMIDM==> MMIDM_IsDmRun = %d")
TRACE_MSG(MMIDM_APP_1573_112_2_18_2_13_48_50,"MMIDM==> get dm state = %d")
TRACE_MSG(MMIDM_APP_1584_112_2_18_2_13_48_51,"MMIDM==> set dm state = %d")
TRACE_MSG(MMIDM_APP_1595_112_2_18_2_13_48_52,"MMIDM==> get dm session type = %d")
TRACE_MSG(MMIDM_APP_1614_112_2_18_2_13_48_53,"MMIDM==> MMIDM_SendSigToDmTask task_id=0")
TRACE_MSG(MMIDM_APP_1621_112_2_18_2_13_48_54,"MMIDM==> PNULL == signal_ptr")
TRACE_MSG(MMIDM_APP_1638_112_2_18_2_13_48_55,"MMIDM==> PNULL=signal_ptr->content.msg_body")
TRACE_MSG(MMIDM_APP_1649_112_2_18_2_13_48_56,"MMIDM==> SendSigToDmTask! event id =%d")
TRACE_MSG(MMIDM_APP_1701_112_2_18_2_13_48_57,"MMIDM==> MMIDM_ReStartDmSession s_dm_need_restart = %d")
TRACE_MSG(MMIDM_APP_1746_112_2_18_2_13_48_58,"MMIDM==> MMIDM_ReStartDmSession status = %d")
TRACE_MSG(MMIDM_APP_1759_112_2_18_2_13_48_59,"MMIDM==> MMIDM_ReSetDmPushInfo ")
TRACE_MSG(MMIDM_APP_1771_112_2_18_2_13_48_60,"MMIDM==> MMIDM_HandDmRxChange  rx_level=%d s_dm_need_restart=%d")
TRACE_MSG(MMIDM_APP_1806_112_2_18_2_13_48_61,"MMIDM==> MMIDM_SetIsNeedRestart  is_need_restart=%d")
TRACE_MSG(MMIDM_APP_1816_112_2_18_2_13_48_62,"MMIDM==> MMIDM_GetIsNeedRestart  s_dm_need_restart=%d")
TRACE_MSG(MMIDM_APP_1856_112_2_18_2_13_48_63,"MMIDM==> CreateDmTreeXmlFile MMIAPIFMM_DeleteFile error=%d")
TRACE_MSG(MMIDM_APP_1877_112_2_18_2_13_48_64,"MMIDM==> tree.xml WriteFile succ!")
TRACE_MSG(MMIDM_APP_1882_112_2_18_2_13_48_65,"MMIDM==> tree.xml WriteFile fail!")
TRACE_MSG(MMIDM_APP_1973_112_2_18_2_13_49_66,"MMIDM==> s_dm_info.is_download_finish = %d")
TRACE_MSG(MMIDM_APP_1992_112_2_18_2_13_49_67,"MMIDM==> s_dm_info.handset_lock = %d")
TRACE_MSG(MMIDM_APP_2010_112_2_18_2_13_49_68,"MMIDM==> s_dm_debug_info.log_switch = %d")
TRACE_MSG(MMIDM_APP_2028_112_2_18_2_13_49_69,"MMIDM==> s_dm_debug_info.fota_switch = %d")
TRACE_MSG(MMIDM_APP_2045_112_2_18_2_13_49_70,"MMIDM==> s_dm_debug_info.tree_switch = %d")
TRACE_MSG(MMIDM_APP_2055_112_2_18_2_13_49_71,"MMIDM==> idasdec_type = %d")
TRACE_MSG(MMIDM_APP_2123_112_2_18_2_13_49_72,"MMIDM==>MMIDM_GetDmProfile")
TRACE_MSG(MMIDM_APP_2148_112_2_18_2_13_49_73,"MMIDM==>MMIDM_SetDmProfile length = %d result=%d")
TRACE_MSG(MMIDM_APP_2166_112_2_18_2_13_49_74,"MMIDM==> MMIDM_GetConnectInfo max_num = %d mmi_cmnet_connect_index =%d")
TRACE_MSG(MMIDM_APP_2190_112_2_18_2_13_49_75,"MMIDM==>MMIDM_GetGPRSInfo type = %d con_type=%d")
TRACE_MSG(MMIDM_APP_2194_112_2_18_2_13_49_76,"MMIDM==>MMIDM_GetGPRSInfo type PNULL == connect_datail_ptr")
TRACE_MSG(MMIDM_APP_2242_112_2_18_2_13_49_77,"MMIDM==> MMIDM_GetGPRSInfo type=%d")
TRACE_MSG(MMIDM_APP_2245_112_2_18_2_13_49_78,"MMIDM==> MMIDM_GetGPRSInfo string = %s")
TRACE_MSG(MMIDM_APP_2271_112_2_18_2_13_49_79,"MMIDM==>MMIDM_SetGprsInfo type PNULL == connect_datail_ptr")
TRACE_MSG(MMIDM_APP_2276_112_2_18_2_13_49_80,"MMIDM==>MMIDM_SetGprsInfo type = %d con_type = %d,len=%d,info=%s")
TRACE_MSG(MMIDM_APP_2336_112_2_18_2_13_49_81,"MMIDM_SetGprsInfo type=%d")
TRACE_MSG(MMIDM_APP_2369_112_2_18_2_13_49_82,"MMIDM==>DM_GetWAPInfo type = %d")
TRACE_MSG(MMIDM_APP_2373_112_2_18_2_13_49_83,"MMIDM==>DM_GetWAPInfo type PNULL == connect_datail_ptr")
TRACE_MSG(MMIDM_APP_2414_112_2_18_2_13_50_84,"MMIDM==> DM_GetWAPInfo type=%d")
TRACE_MSG(MMIDM_APP_2417_112_2_18_2_13_50_85,"MMIDM==> DM_GetWAPInfo string = %s")
TRACE_MSG(MMIDM_APP_2431_112_2_18_2_13_50_86,"MMIDM==>DM_SetWAPInfo type = %d info = %s length=%d")
TRACE_MSG(MMIDM_APP_2494_112_2_18_2_13_50_87,"MMIDM==> DM_SetWAPInfo type=%d")
TRACE_MSG(MMIDM_APP_2511_112_2_18_2_13_50_88,"MMIDM==>MMIDM_GetMMSCInfo type = %d")
TRACE_MSG(MMIDM_APP_2531_112_2_18_2_13_50_89,"MMIDM==> MMIDM_GetMMSCInfo type=%d")
TRACE_MSG(MMIDM_APP_2534_112_2_18_2_13_50_90,"MMIDM==> MMIDM_GetMMSCInfo string = %s")
TRACE_MSG(MMIDM_APP_2553_112_2_18_2_13_50_91,"MMIDM==>MMIDM_SetMMSCInfo type = %d len=%d,info=%s")
TRACE_MSG(MMIDM_APP_2576_112_2_18_2_13_50_92,"MMIDM==> MMIDM_SetMMSCInfo type=%d")
TRACE_MSG(MMIDM_APP_2579_112_2_18_2_13_50_93,"MMIDM==> MMIDM_SetMMSCInfo string = %s")
TRACE_MSG(MMIDM_APP_2592_112_2_18_2_13_50_94,"MMIDM==> MMIDM_GetPIMInfo type = %d")
TRACE_MSG(MMIDM_APP_2616_112_2_18_2_13_50_95,"MMIDM==> MMIDM_GetPIMInfo type=%d")
TRACE_MSG(MMIDM_APP_2619_112_2_18_2_13_50_96,"MMIDM==> MMIDM_GetPIMInfo string = %s")
TRACE_MSG(MMIDM_APP_2634_112_2_18_2_13_50_97,"MMIDM==> MMIDM_GetPIMInfo type = %d")
TRACE_MSG(MMIDM_APP_2663_112_2_18_2_13_50_98,"MMIDM==> MMIDM_SetPIMInfo type=%d")
TRACE_MSG(MMIDM_APP_2668_112_2_18_2_13_50_99,"MMIDM==> MMIDM_SetPIMInfo string = %s")
TRACE_MSG(MMIDM_APP_2683_112_2_18_2_13_50_100,"MMIDM==> MMIDM_GetStreamingInfo type = %d")
TRACE_MSG(MMIDM_APP_2725_112_2_18_2_13_50_101,"MMIDM==> MMIDM_GetStreamingInfo type=%d")
TRACE_MSG(MMIDM_APP_2729_112_2_18_2_13_50_102,"MMIDM==> MMIDM_GetStreamingInfo string = %s")
TRACE_MSG(MMIDM_APP_2746_112_2_18_2_13_50_103,"MMIDM==> MMIDM_SetStreamingInfo type = %d")
TRACE_MSG(MMIDM_APP_2784_112_2_18_2_13_50_104,"MMIDM==> MMIDM_SetStreamingInfo string = %s")
TRACE_MSG(MMIDM_APP_2811_112_2_18_2_13_50_105,"MMIDM==> MMIDM_SetStreamingInfo type=%d")
TRACE_MSG(MMIDM_APP_2817_112_2_18_2_13_50_106,"MMIDM==> MMIDM_SetStreamingInfo string = %s")
TRACE_MSG(MMIDM_APP_2827_112_2_18_2_13_50_107,"MMIDM==> MMIDM_GetSrvAddURL srv_addr = %s")
TRACE_MSG(MMIDM_APP_2839_112_2_18_2_13_50_108,"MMIDM==> MMIDM_GetLockStatusCode status")
TRACE_MSG(MMIDM_APP_2866_112_2_18_2_13_50_109,"MMIDM==> MMIDM_HandleDmMessage 0x%x")
TRACE_MSG(MMIDM_APP_2886_112_2_18_2_13_50_110,"MMIDM==> MSG_DM_SESSION_EXIT ")
TRACE_MSG(MMIDM_APP_2897_112_2_18_2_13_51_111,"MMIDM==> MMIDM_HandleDmMessage dm_sig->event_id=%d")
TRACE_MSG(MMIDM_APP_2915_112_2_18_2_13_51_112,"MMIDM==> MMIDM_ResetFactorySetting ")
TRACE_MSG(MMIDM_APP_2939_112_2_18_2_13_51_113,"MMIDM==> MMIDM_ResetFactorySetting clear cl fail!")
TRACE_MSG(MMIDM_APP_2982_112_2_18_2_13_51_114,"MMIDM==> MMIDM_GetDevFreeSpace free size = %d")
TRACE_MSG(MMIDM_APP_3001_112_2_18_2_13_51_115,"MMIDM==> MMIDM_CheckDlSpaceEnouge result = %d")
TRACE_MSG(MMIDM_APP_3018_112_2_18_2_13_51_116,"MMIDM==> MMIDM_InstallUpdatePackage succ!")
TRACE_MSG(MMIDM_APP_3022_112_2_18_2_13_51_117,"MMIDM==> MMIDM_InstallUpdatePackage fail!")
TRACE_MSG(MMIDM_APP_3047_112_2_18_2_13_51_118,"MMIDM==> MMIDM_CheckUpdateFile ")
TRACE_MSG(MMIDM_APP_3089_112_2_18_2_13_51_119,"MMIDM==> LoadFileToImage start")
TRACE_MSG(MMIDM_APP_3099_112_2_18_2_13_51_120,"MMIDM==> LoadFileToImage MMIAPIFMM_IsFileExist")
TRACE_MSG(MMIDM_APP_3104_112_2_18_2_13_51_121,"MMIDM==> LoadFileToImage sfs_handle=%d,fota_error=%d")
TRACE_MSG(MMIDM_APP_3113_112_2_18_2_13_51_122,"MMIDM==> SFS_GetFileSize file_size=%d")
TRACE_MSG(MMIDM_APP_3116_112_2_18_2_13_51_123,"MMIDM==> LoadFileToImage FOTA_GetSpaceSize()=%d")
TRACE_MSG(MMIDM_APP_3124_112_2_18_2_13_51_124,"MMIDM==> LoadFileToImage FOTA_SetUpdateFlag fail!")
TRACE_MSG(MMIDM_APP_3142_112_2_18_2_13_51_125,"MMIDM==> LoadFileToImage to_read_size=%d,bytes_read= %d,error=%d")
TRACE_MSG(MMIDM_APP_3149_112_2_18_2_13_51_126,"MMIDM==> FOTA_ImageArea_Write fota_size =%d")
TRACE_MSG(MMIDM_APP_3153_112_2_18_2_13_51_127,"MMIDM==> FOTA_ImageArea_Write fail fota_size=%d")
TRACE_MSG(MMIDM_APP_3164_112_2_18_2_13_51_128,"MMIDM==> MMIAPIFMM_ReadFile fail!")
TRACE_MSG(MMIDM_APP_3171_112_2_18_2_13_51_129,"MMIDM==> SFS_SetFilePointer fail!")
TRACE_MSG(MMIDM_APP_3177_112_2_18_2_13_51_130,"MMIDM==> loop s_offset =%d,to_read_size=%d")
TRACE_MSG(MMIDM_APP_3193_112_2_18_2_13_51_131,"MMIDM==> result = %d")
TRACE_MSG(MMIDM_APP_3198_112_2_18_2_13_51_132,"MMIDM==> LoadFileToImage fota not switch on!")
TRACE_MSG(MMIDM_APP_3220_112_2_18_2_13_51_133,"MMIDM==> fota state = %d")
TRACE_MSG(MMIDM_APP_3230_112_2_18_2_13_51_134,"MMIDM==> fota state DM_FOTA_UPDATA_DONE")
TRACE_MSG(MMIDM_APP_3252_112_2_18_2_13_51_135,"MMIDM==> MMIDM_SendFotaReport PLMN_status=%d,fota result = %d")
TRACE_MSG(MMIDM_APP_3268_112_2_18_2_13_51_136,"MMIDM==> MMIDM_SendFotaReport ")
TRACE_MSG(MMIDM_APP_3274_112_2_18_2_13_51_137,"MMIDM==> MMIDM_SendFotaReport =%d")
TRACE_MSG(MMIDM_APP_3279_112_2_18_2_13_51_138,"MMIDM==> plmn status=%d,fota result=%d")
TRACE_MSG(MMIDM_APP_3310_112_2_18_2_13_51_139,"MMIDM==> MMIDM_ExtractAPPFile MMIAPIFMM_DeleteFile error= %d result= %d")
TRACE_MSG(MMIDM_APP_3347_112_2_18_2_13_51_140,"MMIDM==> 2 read_size = %d,bytes_read=%d")
TRACE_MSG(MMIDM_APP_3350_112_2_18_2_13_51_141,"MMIDM==> read sfspacket_handle fail 1 error=%d")
TRACE_MSG(MMIDM_APP_3354_112_2_18_2_13_51_142,"MMIDM==> 3 bytes_read = %d,writelen=%d")
TRACE_MSG(MMIDM_APP_3357_112_2_18_2_13_51_143,"MMIDM==> write sfsjad_handle fail 1 !error %d")
TRACE_MSG(MMIDM_APP_3405_112_2_18_2_13_52_144,"MMIDM==> MMIDM_ExtractAppFile")
TRACE_MSG(MMIDM_APP_3413_112_2_18_2_13_52_145,"MMIDM==> create sfspacket_handle fail !")
TRACE_MSG(MMIDM_APP_3420_112_2_18_2_13_52_146,"MMIDM==> read sfspacket_handle fail !")
TRACE_MSG(MMIDM_APP_3423_112_2_18_2_13_52_147,"MMIDM==> MMIDM_ExtractAppFile file_head=0x%x")
TRACE_MSG(MMIDM_APP_3430_112_2_18_2_13_52_148,"MMIDM==> read sfspacket_handle fail !")
TRACE_MSG(MMIDM_APP_3433_112_2_18_2_13_52_149,"MMIDM==> MMIDM_ExtractAppFile version=%d")
TRACE_MSG(MMIDM_APP_3441_112_2_18_2_13_52_150,"MMIDM==> read sfspacket_handle fail !")
TRACE_MSG(MMIDM_APP_3447_112_2_18_2_13_52_151,"MMIDM==> MMIDM_ExtractAppFile file_size=0x%x")
TRACE_MSG(MMIDM_APP_3465_112_2_18_2_13_52_152,"MMIDM==> MMIDM_ExtractAppFile file_all_size=%d,jad size= %d,is_old_version %d")
TRACE_MSG(MMIDM_APP_3489_112_2_18_2_13_52_153,"MMIDM==> create sfsjad_handle fail !")
TRACE_MSG(MMIDM_APP_3502_112_2_18_2_13_52_154,"MMIDM==> create sfsjar_handle fail !")
TRACE_MSG(MMIDM_APP_3520_112_2_18_2_13_52_155,"MMIDM==> app_java.all_size %d")
TRACE_MSG(MMIDM_APP_3550_112_2_18_2_13_52_156,"MMIDM==> MMIDM_ExtractAppFile end")
TRACE_MSG(MMIDM_APP_3555_112_2_18_2_13_52_157,"MMIDM==> File is not exist !")
TRACE_MSG(MMIDM_APP_3615_112_2_18_2_13_52_158,"MMIDM==> MMIDM_ScomoInstall not support java!")
TRACE_MSG(MMIDM_APP_3625_112_2_18_2_13_52_159,"MMIDM==> MMIDM_ScomoInstall fail!")
TRACE_MSG(MMIDM_APP_3636_112_2_18_2_13_52_160,"MMIDM==> MMIDM_GetDlState state = %d ")
TRACE_MSG(MMIDM_APP_3646_112_2_18_2_13_52_161,"MMIDM==> MMIDM_SetDlState state = %d ")
TRACE_MSG(MMIDM_APP_3663_112_2_18_2_13_52_162,"MMIDM==> MMIDM_IsNeedDelayDMSession result= %d")
TRACE_MSG(MMIDM_APP_3678_112_2_18_2_13_52_163,"MMIDM==> MMIDM_ScomoInstallCallback = %d")
TRACE_MSG(MMIDM_APP_3686_112_2_18_2_13_52_164,"MMIDM==> install scomo dm result= %d")
TRACE_MSG(MMIDM_APP_3690_112_2_18_2_13_52_165,"MMIDM==> install scomo dm fail!")
TRACE_MSG(MMIDM_APP_3736_112_2_18_2_13_52_166,"MMIDM==> AddScomoIdToTab result=%d")
TRACE_MSG(MMIDM_APP_3784_112_2_18_2_13_52_167,"MMIDM==> IsScomoApp result=%d")
TRACE_MSG(MMIDM_APP_3793_112_2_18_2_13_52_168,"MMIDM==> MMIDM_ScomoRemoveCallback = %d")
TRACE_MSG(MMIDM_APP_3806_112_2_18_2_13_52_169,"MMIDM==> remove scomo fail!")
TRACE_MSG(MMIDM_APP_3824_112_2_18_2_13_52_170,"MMIDM==> MMIDM_CleanInstallFile ")
TRACE_MSG(MMIDM_APP_3843_112_2_18_2_13_52_171,"MMIDM==> error = %d")
TRACE_MSG(MMIDM_APP_3865_112_2_18_2_13_53_172,"MMIDM==> MMIDM_DelDmFolderFiles error=%d")
TRACE_MSG(MMIDM_APP_3889_112_2_18_2_13_53_173,"MMIDM==> MMIDM_GetResumeType type = %d")
TRACE_MSG(MMIDM_APP_3929_112_2_18_2_13_53_174,"MMIDM==> MMIDM_SetBrokenType type = %d")
TRACE_MSG(MMIDM_APP_3942_112_2_18_2_13_53_175,"MMIDM==> MMIDM_GetBrokemType broken_type = %d")
TRACE_MSG(MMIDM_EXPORT_180_112_2_18_2_13_53_176,"MMIDM==> MMIAPIDM_HandleScomoCallback suiteID=%d")
TRACE_MSG(MMIDM_INTERNAL_63_112_2_18_2_13_54_177,"MMIDM==>  MMIDM_Run ")
TRACE_MSG(MMIDM_INTERNAL_73_112_2_18_2_13_54_178,"MMIDM==>  MMIDM_Destroy ")
TRACE_MSG(MMIDM_INTERNAL_83_112_2_18_2_13_55_179,"MMIDM==>  MMIDM_SessionCancel ")
TRACE_MSG(MMIDM_INTERNAL_243_112_2_18_2_13_55_180,"MMIDM==> MMIDM_FUMO_SessionController_triggerDL pRootURI=%s")
TRACE_MSG(MMIDM_INTERNAL_256_112_2_18_2_13_55_181,"MMIDM==> MMIDM_SCOMO_DP_SessionHandler_DL_trigger pRootURI %s")
TRACE_MSG(MMIDM_INTERNAL_657_112_2_18_2_13_56_182,"MMIDM==>  MMIDM_Destroy ")
TRACE_MSG(MMIDM_INTERNAL_667_112_2_18_2_13_56_183,"MMIDM==>  MMIDM_SessionCancel ")
TRACE_MSG(MMIDM_PARSEXML_302_112_2_18_2_13_59_184,"MMIDM_setSessionId id %d")
TRACE_MSG(MMIDM_PARSEXML_325_112_2_18_2_13_59_185,"MMIDM_GetTagStr str == PNULL")
TRACE_MSG(MMIDM_PARSEXML_335_112_2_18_2_13_59_186,"MMIDM_GetTagStr the buf is too small %d")
TRACE_MSG(MMIDM_PARSEXML_351_112_2_18_2_13_59_187,"MMIDM_setResUri %s")
TRACE_MSG(MMIDM_PARSEXML_358_112_2_18_2_13_59_188,"MMIDM_setResUri the buf is too small %d")
TRACE_MSG(MMIDM_PARSEXML_376_112_2_18_2_13_59_189,"MMIDM_getResUri %s")
TRACE_MSG(MMIDM_PARSEXML_406_112_2_18_2_13_59_190,"MMIDM_CreateTag PNULL == buf")
TRACE_MSG(MMIDM_PARSEXML_422_112_2_18_2_13_59_191,"MMIDM_CreateTag bufsize is too small %d")
TRACE_MSG(MMIDM_PARSEXML_432_112_2_18_2_13_59_192,"MMIDM_CreateTag bufsize is too small %d")
TRACE_MSG(MMIDM_PARSEXML_502_112_2_18_2_14_0_193,"MMIDM_GetCreddata PNULL == outbuf")
TRACE_MSG(MMIDM_PARSEXML_539_112_2_18_2_14_0_194,"MMIDM_GetCreddata PNULL == outbuf")
TRACE_MSG(MMIDM_PARSEXML_546_112_2_18_2_14_0_195,"MMIDM_GetCreddata after MMIDM_B64_Md51 %s")
TRACE_MSG(MMIDM_PARSEXML_557_112_2_18_2_14_0_196,"MMIDM_GetCreddata  %s")
TRACE_MSG(MMIDM_PARSEXML_560_112_2_18_2_14_0_197,"MMIDM_GetCreddata after MMIDM_B64_Md52 %s")
TRACE_MSG(MMIDM_PARSEXML_579_112_2_18_2_14_0_198,"MMIDM_GetCreddata PNULL == outbuf")
TRACE_MSG(MMIDM_PARSEXML_585_112_2_18_2_14_0_199,"MMIDM_GetCreddata s_g_nonce  %s")
TRACE_MSG(MMIDM_PARSEXML_589_112_2_18_2_14_0_200,"MMIDM_GetCreddata nonece %s")
TRACE_MSG(MMIDM_PARSEXML_597_112_2_18_2_14_0_201,"MMIDM_GetCreddata after  %s")
TRACE_MSG(MMIDM_PARSEXML_604_112_2_18_2_14_0_202,"MMIDM_GetCreddata after md5 %s")
TRACE_MSG(MMIDM_PARSEXML_606_112_2_18_2_14_0_203,"MMIDM_GetCreddata after b64 %s")
TRACE_MSG(MMIDM_PARSEXML_623_112_2_18_2_14_0_204,"ENTER InitDMXMLHeader")
TRACE_MSG(MMIDM_PARSEXML_629_112_2_18_2_14_0_205,"InitDMXMLHeader PNULL == headerbuf")
TRACE_MSG(MMIDM_PARSEXML_639_112_2_18_2_14_0_206,"InitDMXMLHeader PNULL == buf")
TRACE_MSG(MMIDM_PARSEXML_648_112_2_18_2_14_0_207,"InitDMXMLHeader PNULL == buf2")
TRACE_MSG(MMIDM_PARSEXML_657_112_2_18_2_14_0_208,"InitDMXMLHeader PNULL== ptr")
TRACE_MSG(MMIDM_PARSEXML_666_112_2_18_2_14_0_209,"InitDMXMLHeader PNULL== ptr->tagContent")
TRACE_MSG(MMIDM_PARSEXML_773_112_2_18_2_14_0_210,"MMIDM creddata %s")
TRACE_MSG(MMIDM_PARSEXML_867_112_2_18_2_14_0_211,"ENTER MMIDM_BuildAlert")
TRACE_MSG(MMIDM_PARSEXML_871_112_2_18_2_14_0_212,"MMIDM_BuildAlert PNULL == alert_buf")
TRACE_MSG(MMIDM_PARSEXML_879_112_2_18_2_14_0_213,"MMIDM_BuildItem PNULL == buf")
TRACE_MSG(MMIDM_PARSEXML_888_112_2_18_2_14_0_214,"MMIDM_BuildItem PNULL == buf2")
TRACE_MSG(MMIDM_PARSEXML_897_112_2_18_2_14_0_215,"MMIDM_BuildAlert PNULL== ptr")
TRACE_MSG(MMIDM_PARSEXML_915_112_2_18_2_14_0_216,"MMIDM_BuildAlert buf size is too small buf2 %s")
TRACE_MSG(MMIDM_PARSEXML_930_112_2_18_2_14_0_217,"MMIDM_BuildAlertbuf size is too small  buf2 %s")
TRACE_MSG(MMIDM_PARSEXML_944_112_2_18_2_14_0_218,"MMIDM_BuildAlert PNULL==  ptr->tagContent")
TRACE_MSG(MMIDM_PARSEXML_958_112_2_18_2_14_1_219,"MMIDM_BuildAlert alert_buf is too small buf_size %d")
TRACE_MSG(MMIDM_PARSEXML_988_112_2_18_2_14_1_220,"LEAVE MMIDM_BuildAlert")
TRACE_MSG(MMIDM_PARSEXML_1007_112_2_18_2_14_1_221,"ENTER MMIDM_BuildISource")
TRACE_MSG(MMIDM_PARSEXML_1013_112_2_18_2_14_1_222,"MMIDM_BuildISource PNULL == buf")
TRACE_MSG(MMIDM_PARSEXML_1022_112_2_18_2_14_1_223,"MMIDM_BuildISource PNULL == buf2")
TRACE_MSG(MMIDM_PARSEXML_1044_112_2_18_2_14_1_224,"MMIDM_BuildIsource PNULL== ptr")
TRACE_MSG(MMIDM_PARSEXML_1055_112_2_18_2_14_1_225,"MMIDM_BuildISource PNULL==  ptr->tagContent")
TRACE_MSG(MMIDM_PARSEXML_1108_112_2_18_2_14_1_226,"ENTER MMIDM_BuildIMeta")
TRACE_MSG(MMIDM_PARSEXML_1111_112_2_18_2_14_1_227,"MMIDM_BuildIMeta PNULL == meta_buf")
TRACE_MSG(MMIDM_PARSEXML_1120_112_2_18_2_14_1_228,"MMIDM_BuildIMeta PNULL == buf")
TRACE_MSG(MMIDM_PARSEXML_1129_112_2_18_2_14_1_229,"MMIDM_BuildIMeta PNULL == buf2")
TRACE_MSG(MMIDM_PARSEXML_1155_112_2_18_2_14_1_230,"MMIDM_BuildIMeta PNULL== ptr")
TRACE_MSG(MMIDM_PARSEXML_1165_112_2_18_2_14_1_231,"MMIDM_BuildIMeta PNULL==  ptr->tagContent")
TRACE_MSG(MMIDM_PARSEXML_1217_112_2_18_2_14_1_232,"ENTER MMIDM_BuildItem")
TRACE_MSG(MMIDM_PARSEXML_1220_112_2_18_2_14_1_233,"MMIDM_BuildItem PNULL == item_buf")
TRACE_MSG(MMIDM_PARSEXML_1229_112_2_18_2_14_1_234,"MMIDM_BuildItem PNULL == buf")
TRACE_MSG(MMIDM_PARSEXML_1238_112_2_18_2_14_1_235,"MMIDM_BuildItem PNULL == buf2")
TRACE_MSG(MMIDM_PARSEXML_1253_112_2_18_2_14_1_236,"MMIDM_BuildItem buf size is too small")
TRACE_MSG(MMIDM_PARSEXML_1266_112_2_18_2_14_1_237,"MMIDM_BuildItem buf size is too small")
TRACE_MSG(MMIDM_PARSEXML_1276_112_2_18_2_14_1_238,"MMIDM_BuildItem PNULL== ptr")
TRACE_MSG(MMIDM_PARSEXML_1287_112_2_18_2_14_1_239,"MMIDM_BuildItem PNULL==  ptr->tagContent")
TRACE_MSG(MMIDM_PARSEXML_1344_112_2_18_2_14_1_240,"ENTER MMIDM_BuildReplace")
TRACE_MSG(MMIDM_PARSEXML_1347_112_2_18_2_14_1_241,"MMIDM_BuildReplace PNULL == replace_buf")
TRACE_MSG(MMIDM_PARSEXML_1357_112_2_18_2_14_1_242,"MMIDM_BuildReplace PNULL == buf")
TRACE_MSG(MMIDM_PARSEXML_1366_112_2_18_2_14_1_243,"MMIDM_BuildReplace PNULL == buf2")
TRACE_MSG(MMIDM_PARSEXML_1371_112_2_18_2_14_1_244,"MMIDM_BuildReplace after malloc buf")
TRACE_MSG(MMIDM_PARSEXML_1385_112_2_18_2_14_1_245,"MMIDM_BuildReplace buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1397_112_2_18_2_14_1_246,"MMIDM_BuildReplace buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1405_112_2_18_2_14_1_247,"MMIDM_BuildReplace PNULL== ptr")
TRACE_MSG(MMIDM_PARSEXML_1416_112_2_18_2_14_1_248,"MMIDM_BuildReplace PNULL==  ptr->tagContent")
TRACE_MSG(MMIDM_PARSEXML_1485_112_2_18_2_14_2_249,"ENTER MMIDM_BuildISource")
TRACE_MSG(MMIDM_PARSEXML_1492_112_2_18_2_14_2_250,"MMIDM_BuildISource PNULL == buf")
TRACE_MSG(MMIDM_PARSEXML_1506_112_2_18_2_14_2_251,"MMIDM_BuildIChal PNULL== ptr")
TRACE_MSG(MMIDM_PARSEXML_1517_112_2_18_2_14_2_252,"MMIDM_BuildIChal PNULL==  ptr->tagContent")
TRACE_MSG(MMIDM_PARSEXML_1568_112_2_18_2_14_2_253,"ENTER MMIDM_BuildStatus")
TRACE_MSG(MMIDM_PARSEXML_1574_112_2_18_2_14_2_254,"MMIDM_BuildStatus PNULL == status_buf")
TRACE_MSG(MMIDM_PARSEXML_1583_112_2_18_2_14_2_255,"MMIDM_BuildStatus PNULL == buf")
TRACE_MSG(MMIDM_PARSEXML_1592_112_2_18_2_14_2_256,"MMIDM_BuildReplace PNULL == buf2")
TRACE_MSG(MMIDM_PARSEXML_1610_112_2_18_2_14_2_257,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1622_112_2_18_2_14_2_258,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1634_112_2_18_2_14_2_259,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1646_112_2_18_2_14_2_260,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1658_112_2_18_2_14_2_261,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1670_112_2_18_2_14_2_262,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1684_112_2_18_2_14_2_263,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1692_112_2_18_2_14_2_264,"MMIDM_BuildStatus PNULL== ptr")
TRACE_MSG(MMIDM_PARSEXML_1703_112_2_18_2_14_2_265,"MMIDM_BuildStatus PNULL==  ptr->tagContent")
TRACE_MSG(MMIDM_PARSEXML_1766_112_2_18_2_14_2_266,"ENTER MMIDM_BuildResult")
TRACE_MSG(MMIDM_PARSEXML_1771_112_2_18_2_14_2_267,"MMIDM_BuildResult PNULL == result_buf")
TRACE_MSG(MMIDM_PARSEXML_1780_112_2_18_2_14_2_268,"MMIDM_BuildResult  PNULL == buf")
TRACE_MSG(MMIDM_PARSEXML_1789_112_2_18_2_14_2_269,"MMIDM_BuildResult PNULL == buf2")
TRACE_MSG(MMIDM_PARSEXML_1807_112_2_18_2_14_2_270,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1819_112_2_18_2_14_2_271,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1831_112_2_18_2_14_2_272,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1843_112_2_18_2_14_2_273,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1856_112_2_18_2_14_2_274,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1869_112_2_18_2_14_2_275,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1881_112_2_18_2_14_2_276,"MMIDM_BuildStatus buf is too small")
TRACE_MSG(MMIDM_PARSEXML_1889_112_2_18_2_14_2_277,"MMIDM_BuildReplace PNULL== ptr")
TRACE_MSG(MMIDM_PARSEXML_1900_112_2_18_2_14_2_278,"MMIDM_BuildResult PNULL==  ptr->tagContent")
TRACE_MSG(MMIDM_PARSEXML_1971_112_2_18_2_14_3_279,"ENTER MMIDM_CodecXmlBody")
TRACE_MSG(MMIDM_PARSEXML_1977_112_2_18_2_14_3_280,"MMIDM_CodecXmlBody PNULL == bodybuf")
TRACE_MSG(MMIDM_PARSEXML_1986_112_2_18_2_14_3_281,"MMIDM_BuildResult  PNULL == buf")
TRACE_MSG(MMIDM_PARSEXML_1999_112_2_18_2_14_3_282,"MMIDM_CodecXmlBody PNULL== alert_tag")
TRACE_MSG(MMIDM_PARSEXML_2015_112_2_18_2_14_3_283,"MMIDM_CodecXmlBody PNULL==  alert_tag->CmdId.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2028_112_2_18_2_14_3_284,"MMIDM_CodecXmlBody PNULL== alert_tag->DATA.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2043_112_2_18_2_14_3_285,"MMIDM_CodecXmlBody PNULL== replace_tag")
TRACE_MSG(MMIDM_PARSEXML_2049_112_2_18_2_14_3_286,"MMIDM_CodecXmlBody after malloc replace_tag")
TRACE_MSG(MMIDM_PARSEXML_2058_112_2_18_2_14_3_287,"MMIDM_CodecXmlBody PNULL==replace_tag->CmdId.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2065_112_2_18_2_14_3_288,"MMIDM_CodecXmlBody after TAG_CMDIDID")
TRACE_MSG(MMIDM_PARSEXML_2070_112_2_18_2_14_3_289,"MMIDM_CodecXmlBody PNULL== item_tag")
TRACE_MSG(MMIDM_PARSEXML_2080_112_2_18_2_14_3_290,"MMIDM_CodecXmlBody PNULL==item_tag->source.locuri.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2092_112_2_18_2_14_3_291,"MMIDM_CodecXmlBody PNULL==item_tag->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2105_112_2_18_2_14_3_292,"MMIDM_CodecXmlBody PNULL== item_tag")
TRACE_MSG(MMIDM_PARSEXML_2115_112_2_18_2_14_3_293,"MMIDM_CodecXmlBody PNULL==item_tag->source.locuri.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2127_112_2_18_2_14_3_294,"MMIDM_CodecXmlBody PNULL==item_tag->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2141_112_2_18_2_14_3_295,"MMIDM_CodecXmlBody PNULL== item_tag")
TRACE_MSG(MMIDM_PARSEXML_2151_112_2_18_2_14_3_296,"MMIDM_CodecXmlBody PNULL==item_tag->source.locuri.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2162_112_2_18_2_14_3_297,"MMIDM_CodecXmlBody PNULL==item_tag->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2175_112_2_18_2_14_3_298,"MMIDM_CodecXmlBody PNULL== item_tag")
TRACE_MSG(MMIDM_PARSEXML_2185_112_2_18_2_14_3_299,"MMIDM_CodecXmlBody PNULL==item_tag->source.locuri.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2196_112_2_18_2_14_3_300,"MMIDM_CodecXmlBody PNULL==item_tag->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2208_112_2_18_2_14_3_301,"MMIDM_CodecXmlBody PNULL== item_tag")
TRACE_MSG(MMIDM_PARSEXML_2213_112_2_18_2_14_3_302,"MMIDM_CodecXmlBody after ./DevInfo/DevId 0")
TRACE_MSG(MMIDM_PARSEXML_2219_112_2_18_2_14_3_303,"MMIDM_CodecXmlBody PNULL==item_tag->source.locuri.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2230_112_2_18_2_14_3_304,"MMIDM_CodecXmlBody PNULL==item_tag->meta.format.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2240_112_2_18_2_14_3_305,"MMIDM_CodecXmlBody after ./DevInfo/DevId 1.5")
TRACE_MSG(MMIDM_PARSEXML_2242_112_2_18_2_14_3_306,"MMIDM_CodecXmlBody after ./DevInfo/DevId 1.6")
TRACE_MSG(MMIDM_PARSEXML_2245_112_2_18_2_14_3_307,"MMIDM_CodecXmlBody PNULL==item_tag->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2249_112_2_18_2_14_3_308,"MMIDM_CodecXmlBody after ./DevInfo/DevId 1.7")
TRACE_MSG(MMIDM_PARSEXML_2251_112_2_18_2_14_3_309,"MMIDM_CodecXmlBody after ./DevInfo/DevId 1.8")
TRACE_MSG(MMIDM_PARSEXML_2253_112_2_18_2_14_3_310,"MMIDM_CodecXmlBody after ./DevInfo/DevId 2")
TRACE_MSG(MMIDM_PARSEXML_2259_112_2_18_2_14_3_311,"MMIDM_CodecXmlBody after ./DevInfo/DevId")
TRACE_MSG(MMIDM_PARSEXML_2261_112_2_18_2_14_3_312,"MMIDM_CodecXmlBody begin to MMIDM_BuildReplace")
TRACE_MSG(MMIDM_PARSEXML_2280_112_2_18_2_14_3_313,"MMIDM_CodecXmlBody buf is too small")
TRACE_MSG(MMIDM_PARSEXML_2290_112_2_18_2_14_3_314,"MMIDM_CodecXmlBody buf is too small")
TRACE_MSG(MMIDM_PARSEXML_2300_112_2_18_2_14_3_315,"MMIDM_CodecXmlBody buf is too small")
TRACE_MSG(MMIDM_PARSEXML_2310_112_2_18_2_14_3_316,"MMIDM_CodecXmlBody buf is too small")
TRACE_MSG(MMIDM_PARSEXML_2319_112_2_18_2_14_3_317,"MMIDM_CodecXmlBody PNULL== alert_tag")
TRACE_MSG(MMIDM_PARSEXML_2333_112_2_18_2_14_3_318,"MMIDM_CodecXmlBody PNULL==  alert_tag->CmdId.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2346_112_2_18_2_14_3_319,"MMIDM_CodecXmlBody PNULL== alert_tag->DATA.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2362_112_2_18_2_14_3_320,"MMIDM_CodecXmlBody PNULL== ptr")
TRACE_MSG(MMIDM_PARSEXML_2373_112_2_18_2_14_3_321,"MMIDM_CodecXmlBody PNULL==  ptr->tagContent")
TRACE_MSG(MMIDM_PARSEXML_2424_112_2_18_2_14_4_322,"enter MMIDM_getNextXmlTagBuf")
TRACE_MSG(MMIDM_PARSEXML_2427_112_2_18_2_14_4_323,"MMIDM_getNextXmlTagBuf PNULL == content")
TRACE_MSG(MMIDM_PARSEXML_2493_112_2_18_2_14_4_324,"leave  MMIDM_getNextXmlTagBuf")
TRACE_MSG(MMIDM_PARSEXML_2514_112_2_18_2_14_4_325,"ENTER MMIDM_generateXMLData")
TRACE_MSG(MMIDM_PARSEXML_2521_112_2_18_2_14_4_326,"MMIDM_generateXMLData PNULL== bodybuf")
TRACE_MSG(MMIDM_PARSEXML_2530_112_2_18_2_14_4_327,"MMIDM_generateXMLData PNULL== sendContent")
TRACE_MSG(MMIDM_PARSEXML_2537_112_2_18_2_14_4_328,"MMIDM_generateXMLData HEADER")
TRACE_MSG(MMIDM_PARSEXML_2539_112_2_18_2_14_4_329,"MMIDM_generateXMLData BODY")
TRACE_MSG(MMIDM_PARSEXML_2545_112_2_18_2_14_4_330,"MMIDM_generateXMLData PNULL== ptr")
TRACE_MSG(MMIDM_PARSEXML_2556_112_2_18_2_14_4_331,"MMIDM_generateXMLData PNULL==  ptr->tagContent")
TRACE_MSG(MMIDM_PARSEXML_2631_112_2_18_2_14_4_332,"MMIDM_DealWithExecData PNULL== status_tag")
TRACE_MSG(MMIDM_PARSEXML_2645_112_2_18_2_14_4_333,"MMIDM_DealWithExecData PNULL== status_tag->CmdId.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2658_112_2_18_2_14_4_334,"MMIDM_DealWithExecData PNULL==status_tag->msgRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2670_112_2_18_2_14_4_335,"MMIDM_DealWithExecData PNULL==  status_tag->cmdRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2682_112_2_18_2_14_4_336,"MMIDM_DealWithExecData PNULL== status_tag->targetRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2694_112_2_18_2_14_4_337,"MMIDM_DealWithExecData PNULL== status_tag->cmd.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2707_112_2_18_2_14_4_338,"MMIDM_DealWithExecData PNULL== status_tag->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2765_112_2_18_2_14_4_339,"ENTER MMIDM_ParseXMLStatus")
TRACE_MSG(MMIDM_PARSEXML_2771_112_2_18_2_14_4_340,"MMIDM_ParseXMLStatus PNULL== bodybuf")
TRACE_MSG(MMIDM_PARSEXML_2780_112_2_18_2_14_4_341,"MMIDM_ParseXMLExec PNULL== exec_buf")
TRACE_MSG(MMIDM_PARSEXML_2788_112_2_18_2_14_4_342,"MMIDM_ParseXMLExec body_buf")
TRACE_MSG(MMIDM_PARSEXML_2791_112_2_18_2_14_4_343,"MMIDM_ParseXMLStatus exec_buf")
TRACE_MSG(MMIDM_PARSEXML_2811_112_2_18_2_14_4_344,"leave MMIDM_ParseXMLExec")
TRACE_MSG(MMIDM_PARSEXML_2838_112_2_18_2_14_4_345,"ENTER MMIDM_DealWithStatusData")
TRACE_MSG(MMIDM_PARSEXML_2841_112_2_18_2_14_4_346,"MMIDM_DealWithStatusData PNULL == status_buf")
TRACE_MSG(MMIDM_PARSEXML_2858_112_2_18_2_14_4_347,"MMIDM_DealWithStatusData the cred status s_g_nonce %s")
TRACE_MSG(MMIDM_PARSEXML_2873_112_2_18_2_14_4_348,"MMIDM_DealWithStatusData cred fail")
TRACE_MSG(MMIDM_PARSEXML_2884_112_2_18_2_14_4_349,"MMIDM_DealWithStatusData PNULL== status_tag")
TRACE_MSG(MMIDM_PARSEXML_2897_112_2_18_2_14_4_350,"MMIDM_DealWithStatusData PNULL== status_tag->CmdId.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2910_112_2_18_2_14_4_351,"MMIDM_DealWithStatusData PNULL==status_tag->msgRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2922_112_2_18_2_14_5_352,"MMIDM_DealWithStatusData PNULL==  status_tag->cmdRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2934_112_2_18_2_14_5_353,"MMIDM_DealWithStatusData PNULL== status_tag->targetRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2947_112_2_18_2_14_5_354,"MMIDM_DealWithStatusData PNULL== status_tag->targetRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2959_112_2_18_2_14_5_355,"MMIDM_DealWithStatusData PNULL== status_tag->cmd.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2973_112_2_18_2_14_5_356,"MMIDM_DealWithStatusData PNULL== status_tag->chal.meta.format.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2986_112_2_18_2_14_5_357,"MMIDM_DealWithStatusData PNULL== status_tag->chal.meta.type.tagContent")
TRACE_MSG(MMIDM_PARSEXML_2999_112_2_18_2_14_5_358,"MMIDM_DealWithStatusData PNULL== status_tag->chal.meta.nextnonce.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3011_112_2_18_2_14_5_359,"MMIDM_DealWithStatusData PNULL== status_tag->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3038_112_2_18_2_14_5_360,"MMIDM_DealWithStatusData")
TRACE_MSG(MMIDM_PARSEXML_3063_112_2_18_2_14_5_361,"ENTER MMIDM_ParseXMLStatus")
TRACE_MSG(MMIDM_PARSEXML_3071_112_2_18_2_14_5_362,"MMIDM_ParseXMLStatus PNULL== bodybuf")
TRACE_MSG(MMIDM_PARSEXML_3080_112_2_18_2_14_5_363,"MMIDM_ParseXMLStatus PNULL== status_buf")
TRACE_MSG(MMIDM_PARSEXML_3088_112_2_18_2_14_5_364,"MMIDM_ParseXMLStatus body_buf")
TRACE_MSG(MMIDM_PARSEXML_3091_112_2_18_2_14_5_365,"MMIDM_ParseXMLStatus status_buf")
TRACE_MSG(MMIDM_PARSEXML_3137_112_2_18_2_14_5_366,"ENTER MMIDM_DealWithGetData")
TRACE_MSG(MMIDM_PARSEXML_3144_112_2_18_2_14_5_367,"MMIDM_DealWithGetData PNULL== content")
TRACE_MSG(MMIDM_PARSEXML_3159_112_2_18_2_14_5_368,"MMIDM_DealWithGetData PNULL== status_tag")
TRACE_MSG(MMIDM_PARSEXML_3168_112_2_18_2_14_5_369,"MMIDM_DealWithGetData PNULL== result_tag")
TRACE_MSG(MMIDM_PARSEXML_3178_112_2_18_2_14_5_370,"MMIDM_DealWithGetData PNULL== item_ptr")
TRACE_MSG(MMIDM_PARSEXML_3197_112_2_18_2_14_5_371,"MMIDM_DealWithGetData PNULL== status_tag->CmdId.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3210_112_2_18_2_14_5_372,"MMIDM_DealWithGetData PNULL==status_tag->msgRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3222_112_2_18_2_14_5_373,"MMIDM_DealWithGetData PNULL==  status_tag->cmdRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3234_112_2_18_2_14_5_374,"MMIDM_DealWithGetData PNULL== status_tag->targetRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3246_112_2_18_2_14_5_375,"MMIDM_DealWithGetData PNULL== status_tag->cmd.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3260_112_2_18_2_14_5_376,"MMIDM_DealWithGetData PNULL== status_tag->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3272_112_2_18_2_14_5_377,"MMIDM_DealWithGetData, PNULL==read_func")
TRACE_MSG(MMIDM_PARSEXML_3278_112_2_18_2_14_5_378,"MMIDM_DealWithGetData PNULL== status_tag->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3305_112_2_18_2_14_5_379,"MMIDM_DealWithGetData PNULL==result_tag->CmdId.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3318_112_2_18_2_14_5_380,"MMIDM_DealWithGetData PNULL==result_tag->msgRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3330_112_2_18_2_14_5_381,"MMIDM_DealWithGetData PNULL==result_tag->cmdRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3344_112_2_18_2_14_5_382,"MMIDM_DealWithGetData PNULL==item_ptr->source.locuri.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3360_112_2_18_2_14_5_383,"MMIDM_DealWithGetData PNULL==item_ptr->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3386_112_2_18_2_14_5_384,"MMIDM_DealWithGetData, PNULL==target_uri")
TRACE_MSG(MMIDM_PARSEXML_3415_112_2_18_2_14_6_385,"ENTER MMIDM_ParseXMLGet")
TRACE_MSG(MMIDM_PARSEXML_3423_112_2_18_2_14_6_386,"MMIDM_ParseXMLGet PNULL== bodybuf")
TRACE_MSG(MMIDM_PARSEXML_3432_112_2_18_2_14_6_387,"MMIDM_ParseXMLGet PNULL== get_buf")
TRACE_MSG(MMIDM_PARSEXML_3487_112_2_18_2_14_6_388,"ENTER MMIDM_DealWithReplaceData")
TRACE_MSG(MMIDM_PARSEXML_3493_112_2_18_2_14_6_389,"MMIDM_DealWithReplaceData PNULL== content")
TRACE_MSG(MMIDM_PARSEXML_3502_112_2_18_2_14_6_390,"MMIDM_DealWithReplaceData PNULL== data")
TRACE_MSG(MMIDM_PARSEXML_3518_112_2_18_2_14_6_391,"MMIDM_DealWithReplaceData PNULL== status_tag")
TRACE_MSG(MMIDM_PARSEXML_3533_112_2_18_2_14_6_392,"MMIDM_DealWithReplaceData PNULL==status_tag->CmdId.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3546_112_2_18_2_14_6_393,"MMIDM_DealWithReplaceData PNULL==status_tag->msgRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3558_112_2_18_2_14_6_394,"MMIDM_DealWithReplaceData PNULL==status_tag->cmdRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3570_112_2_18_2_14_6_395,"MMIDM_DealWithReplaceData PNULL==status_tag->targetRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3582_112_2_18_2_14_6_396,"MMIDM_DealWithReplaceData PNULL==status_tag->cmd.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3597_112_2_18_2_14_6_397,"MMIDM_DealWithReplaceData PNULL==status_tag->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3614_112_2_18_2_14_6_398,"MMIDM_DealWithGetData, PNULL==read_func")
TRACE_MSG(MMIDM_PARSEXML_3620_112_2_18_2_14_6_399,"MMIDM_DealWithReplaceData PNULL==status_tag->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3672_112_2_18_2_14_6_400,"ENTER MMIDM_ParseXMLReplace")
TRACE_MSG(MMIDM_PARSEXML_3679_112_2_18_2_14_6_401,"MMIDM_ParseXMLReplace PNULL== bodybuf")
TRACE_MSG(MMIDM_PARSEXML_3688_112_2_18_2_14_6_402,"MMIDM_ParseXMLReplace PNULL== replace_buf")
TRACE_MSG(MMIDM_PARSEXML_3731_112_2_18_2_14_6_403,"ENTER MMIDM_NotifyAlertResult")
TRACE_MSG(MMIDM_PARSEXML_3735_112_2_18_2_14_6_404,"MMIDM_NotifyAlertResult PNULL == s_statusTag_cur")
TRACE_MSG(MMIDM_PARSEXML_3742_112_2_18_2_14_6_405,"MMIDM_NotifyAlertResult PNULL==s_statusTag_cur->data.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3787_112_2_18_2_14_6_406,"ENTER MMIDM_DealWithAlertData")
TRACE_MSG(MMIDM_PARSEXML_3795_112_2_18_2_14_6_407,"MMIDM_DealWithAlertData PNULL== item")
TRACE_MSG(MMIDM_PARSEXML_3804_112_2_18_2_14_6_408,"MMIDM_DealWithAlertData PNULL== subdata")
TRACE_MSG(MMIDM_PARSEXML_3816_112_2_18_2_14_6_409,"MMIDM_DealWithAlertData data")
TRACE_MSG(MMIDM_PARSEXML_3844_112_2_18_2_14_6_410,"MMIDM_DealWithAlertData minimum display time %s")
TRACE_MSG(MMIDM_PARSEXML_3860_112_2_18_2_14_6_411,"MMIDM_DealWithAlertData maximum display time %s")
TRACE_MSG(MMIDM_PARSEXML_3864_112_2_18_2_14_6_412,"MMIDM_DealWithAlertData display content =%s")
TRACE_MSG(MMIDM_PARSEXML_3871_112_2_18_2_14_6_413,"MMIDM_DealWithAlertData PNULL== status_tag")
TRACE_MSG(MMIDM_PARSEXML_3885_112_2_18_2_14_6_414,"MMIDM_DealWithAlertData PNULL==status_tag->CmdId.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3899_112_2_18_2_14_6_415,"MMIDM_DealWithAlertData PNULL==status_tag->msgRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3912_112_2_18_2_14_7_416,"MMIDM_DealWithAlertData PNULL==status_tag->cmdRef.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3925_112_2_18_2_14_7_417,"MMIDM_DealWithAlertData PNULL==status_tag->cmd.tagContent")
TRACE_MSG(MMIDM_PARSEXML_3983_112_2_18_2_14_7_418,"ENTER MMIDM_ParseXMLAlert")
TRACE_MSG(MMIDM_PARSEXML_3990_112_2_18_2_14_7_419,"MMIDM_ParseXMLAlert PNULL== bodybuf")
TRACE_MSG(MMIDM_PARSEXML_3999_112_2_18_2_14_7_420,"MMIDM_ParseXMLAlert PNULL== alert_buf")
TRACE_MSG(MMIDM_PARSEXML_4181_112_2_18_2_14_7_421,"ENTER MMIDM_ReleaseXMLData")
TRACE_MSG(MMIDM_PARSEXML_4225_112_2_18_2_14_7_422,"LEAVE MMIDM_ReleaseXMLData")
TRACE_MSG(MMIDM_PARSEXML_4240_112_2_18_2_14_7_423,"enter MMIDM_ParseXMLData")
TRACE_MSG(MMIDM_PARSEXML_4246_112_2_18_2_14_7_424,"MMIDM_ParseXMLData, the wrong sessionid %s")
TRACE_MSG(MMIDM_PARSEXML_4254_112_2_18_2_14_7_425,"MMIDM_ParseXMLData, not the msg rely to our msg %s")
TRACE_MSG(MMIDM_PARSEXML_4270_112_2_18_2_14_7_426,"enter MMIDM_ParseXMLData 1")
TRACE_MSG(MMIDM_PARSEXML_4272_112_2_18_2_14_7_427,"enter MMIDM_ParseXMLData 2")
TRACE_MSG(MMIDM_PARSEXML_4274_112_2_18_2_14_7_428,"enter MMIDM_ParseXMLData 3")
TRACE_MSG(MMIDM_PARSEXML_4276_112_2_18_2_14_7_429,"enter MMIDM_ParseXMLData 4")
TRACE_MSG(MMIDM_PARSEXML_4278_112_2_18_2_14_7_430,"enter MMIDM_ParseXMLData 5")
TRACE_MSG(MMIDM_PARSEXML_4296_112_2_18_2_14_7_431,"MMIDM_SendDmData DM_CANCEL == MMIDM_GetDmState()")
TRACE_MSG(MMIDM_PARSEXML_4302_112_2_18_2_14_7_432,"MMIDM_SendDmData PNULL== sendbuf")
TRACE_MSG(MMIDM_PARSEXML_4346_112_2_18_2_14_7_433,"MMIDM_BeginDmTansport")
TRACE_MSG(MMIDM_PARSEXML_4349_112_2_18_2_14_7_434,"MMIDM_BeginDmTansport DM_CANCEL == MMIDM_GetDmState()")
TRACE_MSG(MMIDM_PARSEXML_4376_112_2_18_2_14_7_435,"MMIDM_ParseReceiveData NO PACKET TO SEND")
TRACE_MSG(MMIDM_PARSEXML_4396_112_2_18_2_14_7_436,"MMIDM_ParseReceiveData DM_CANCEL == MMIDM_GetDmState()")
TRACE_MSG(MMIDM_PARSEXML_4421_112_2_18_2_14_8_437,"mmidm %c%c%c%c%c%c")
TRACE_MSG(MMIDM_PL_COMM_241_112_2_18_2_14_8_438,"MMIDM==> [RDM_COMMS_getDMSocketID_cbFunc] socketID = 0x%0x \r\n")
TRACE_MSG(MMIDM_PL_COMM_256_112_2_18_2_14_8_439,"MMIDM==> [RDM_COMMS_GetDLSocketID_cbFunc] socketID = 0x%x \r\n")
TRACE_MSG(MMIDM_PL_COMM_277_112_2_18_2_14_8_440,"MMIDM==> [RDM_COMMS_Construct_http] bodylen = %d\n")
TRACE_MSG(MMIDM_PL_COMM_288_112_2_18_2_14_8_441,"MMIDM==> [RDM_COMMS_Construct_http] failed to extract host.\n")
TRACE_MSG(MMIDM_PL_COMM_295_112_2_18_2_14_8_442,"MMIDM==> [RDM_COMMS_Construct_http] serverName = %s, port = %d\n")
TRACE_MSG(MMIDM_PL_COMM_315_112_2_18_2_14_8_443,"MMIDM==> [RDM_COMMS_Construct_http] len=%d\n")
TRACE_MSG(MMIDM_PL_COMM_316_112_2_18_2_14_8_444,"MMIDM==> [RDM_COMMS_Construct_http]Message too long to be hold.maxlen=%d\n")
TRACE_MSG(MMIDM_PL_COMM_320_112_2_18_2_14_8_445,"MMIDM==> [RDM_COMMS_Construct_http] head lenght =%d\n")
TRACE_MSG(MMIDM_PL_COMM_327_112_2_18_2_14_8_446,"MMIDM==> [RDM_COMMS_Construct_http] message constructed total len = %d \n")
TRACE_MSG(MMIDM_PL_COMM_347_112_2_18_2_14_8_447,"MMIDM==> [findSubStringN] was called len= 0x%x,N= 0x%x,strN=0x%x\r\n")
TRACE_MSG(MMIDM_PL_COMM_365_112_2_18_2_14_8_448,"MMIDM==> [findSubStringN]found the whole string,next =0x%x\r\n")
TRACE_MSG(MMIDM_PL_COMM_430_112_2_18_2_14_9_449,"MMIDM==> [RDM_parse_syncml]  begin\r\n")
TRACE_MSG(MMIDM_PL_COMM_455_112_2_18_2_14_9_450,"MMIDM==> [RDM_parse_syncml]  syncml_hdr_len=%d")
TRACE_MSG(MMIDM_PL_COMM_468_112_2_18_2_14_9_451,"MMIDM==>RDM_parse_syncml  PNULL ==username_occ_ptr")
TRACE_MSG(MMIDM_PL_COMM_480_112_2_18_2_14_9_452,"MMIDM==>RDM_parse_syncml  PNULL ==mac_occ_ptr")
TRACE_MSG(MMIDM_PL_COMM_492_112_2_18_2_14_9_453,"MMIDM==>RDM_parse_syncml  PNULL ==algorithm_occ_ptr")
TRACE_MSG(MMIDM_PL_COMM_536_112_2_18_2_14_9_454,"MMIDM==> [RDM_parse_syncml]3 RDM_ERR_MEMORY")
TRACE_MSG(MMIDM_PL_COMM_548_112_2_18_2_14_9_455,"MMIDM==> [RDM_parse_syncml] s_loc_username =%s")
TRACE_MSG(MMIDM_PL_COMM_550_112_2_18_2_14_9_456,"MMIDM==> [RDM_parse_syncml] s_loc_mac =%s")
TRACE_MSG(MMIDM_PL_COMM_552_112_2_18_2_14_9_457,"MMIDM==> [RDM_parse_syncml] s_loc_algorithm =%s")
TRACE_MSG(MMIDM_PL_COMM_565_112_2_18_2_14_9_458,"MMIDM==> [RDM_parse_syncml]  no mac\r\n")
TRACE_MSG(MMIDM_PL_COMM_584_112_2_18_2_14_9_459,"MMIDM==>  VDM_Comm_PL_Conn_init ")
TRACE_MSG(MMIDM_PL_COMM_590_112_2_18_2_14_9_460,"MMIDM==>  VDM_Comm_PL_Conn_init result = %d ")
TRACE_MSG(MMIDM_PL_COMM_597_112_2_18_2_14_9_461,"MMIDM==>  VDM_Comm_PL_Conn_init result = %d ")
TRACE_MSG(MMIDM_PL_COMM_623_112_2_18_2_14_9_462,"MMIDM==>  VDM_Comm_PL_Conn_open ")
TRACE_MSG(MMIDM_PL_COMM_627_112_2_18_2_14_9_463,"MMIDM==>  VDM_Comm_PL_Conn_open PNULL == inURL")
TRACE_MSG(MMIDM_PL_COMM_635_112_2_18_2_14_9_464,"MMIDM==>  VDM_Comm_PL_Conn_open:MMIDM_ActivePdpConnect not opened")
TRACE_MSG(MMIDM_PL_COMM_643_112_2_18_2_14_9_465,"MMIDM==>  VDM_Comm_PL_Conn_open8:connection not opened to %s:%hu\n")
TRACE_MSG(MMIDM_PL_COMM_648_112_2_18_2_14_9_466,"MMIDM==>  VDM_Comm_PL_Conn_open9 (connId = %d) returns 0x%x\n")
TRACE_MSG(MMIDM_PL_COMM_673_112_2_18_2_14_9_467,"MMIDM==>  VDM_Comm_PL_Conn_send ")
TRACE_MSG(MMIDM_PL_COMM_679_112_2_18_2_14_9_468,"MMIDM==> VDM_Comm_PL_Conn_send success  actualSent = 0x%x")
TRACE_MSG(MMIDM_PL_COMM_683_112_2_18_2_14_9_469,"MMIDM==>  VDM_Comm_PL_Conn_send :send attempt failed\n")
TRACE_MSG(MMIDM_PL_COMM_687_112_2_18_2_14_9_470,"MMIDM==> VDM_Comm_PL_Conn_send (connId = %d) returns 0x%x\n")
TRACE_MSG(MMIDM_PL_COMM_716_112_2_18_2_14_9_471,"MMIDM==>  VDM_Comm_PL_Conn_receive  inBufSize = %d")
TRACE_MSG(MMIDM_PL_COMM_723_112_2_18_2_14_9_472,"MMIDM==>  VDM_Comm_PL_Conn_receive: recv attempt failed sock_errno =%d")
TRACE_MSG(MMIDM_PL_COMM_729_112_2_18_2_14_9_473,"MMIDM==>  VDM_Comm_PL_Conn_receive: outDataLen = 0")
TRACE_MSG(MMIDM_PL_COMM_737_112_2_18_2_14_9_474,"MMIDM==>  VDM_Comm_PL_Conn_receive notifyTransport left_data_len=%d")
TRACE_MSG(MMIDM_PL_COMM_740_112_2_18_2_14_9_475,"MMIDM==>  VDM_Comm_PL_Conn_receive (connId = %d) returns 0x%x, *outDataLen=%d\n")
TRACE_MSG(MMIDM_PL_COMM_757_112_2_18_2_14_9_476,"MMIDM==> VDM_Comm_PL_Conn_close (connId = %d) returns \n")
TRACE_MSG(MMIDM_PL_COMM_767_112_2_18_2_14_9_477,"MMIDM==>  VDM_Comm_PL_Conn_term ")
TRACE_MSG(MMIDM_PL_COMM_775_112_2_18_2_14_9_478,"MMIDM==>  VDM_Comm_PL_Conn_term returns \n")
TRACE_MSG(MMIDM_PL_COMM_830_112_2_18_2_14_9_479,"MMIDM==> VDM_Comm_PL_HTTP_init returns 0x%x\n")
TRACE_MSG(MMIDM_PL_COMM_843_112_2_18_2_14_9_480,"MMIDM==>  VDM_Comm_PL_HTTP_init called (inAddrType=%s)\n")
TRACE_MSG(MMIDM_PL_COMM_881_112_2_18_2_14_10_481,"MMIDM==> VDM_Comm_PL_HTTP_init called s_dm_observer=%x\n")
TRACE_MSG(MMIDM_PL_COMM_883_112_2_18_2_14_10_482,"MMIDM==> VDM_Comm_PL_HTTP_init returns 0x%x\n")
TRACE_MSG(MMIDM_PL_COMM_919_112_2_18_2_14_10_483,"MMIDM==> [VDM_Comm_PL_HTTP_open] begin, *outConnId = %d, addr = %s \r\n")
TRACE_MSG(MMIDM_PL_COMM_920_112_2_18_2_14_10_484,"MMIDM==> [VDM_Comm_PL_HTTP_open]   inMimeType = %d, msgLen = %d,addr = %d \r\n")
TRACE_MSG(MMIDM_PL_COMM_935_112_2_18_2_14_10_485,"MMIDM==> [VDM_Comm_PL_HTTP_open]   username %x = %s, mac %x = %s, algorithm %x = %s \r\n")
TRACE_MSG(MMIDM_PL_COMM_950_112_2_18_2_14_10_486,"MMIDM==> [VDM_Comm_PL_HTTP_open] s_connect_info_ptr->server_url = %s\r\n")
TRACE_MSG(MMIDM_PL_COMM_955_112_2_18_2_14_10_487,"MMIDM==> [VDM_Comm_PL_HTTP_open] ActivePdp fail!")
TRACE_MSG(MMIDM_PL_COMM_964_112_2_18_2_14_10_488,"MMIDM==> [VDM_Comm_PL_HTTP_open] open fail!")
TRACE_MSG(MMIDM_PL_COMM_999_112_2_18_2_14_10_489,"MMIDM==> VDM_Comm_PL_HTTP_send\r\n")
TRACE_MSG(MMIDM_PL_COMM_1002_112_2_18_2_14_10_490,"MMIDM==> VDM_Comm_PL_HTTP_send total %d")
TRACE_MSG(MMIDM_PL_COMM_1006_112_2_18_2_14_10_491,"MMIDM==> VDM_Comm_PL_HTTP_send sentLen %d")
TRACE_MSG(MMIDM_PL_COMM_1021_112_2_18_2_14_10_492,"MMIDM==> VDM_Comm_PL_HTTP_send, total = %d, sentLen = %d")
TRACE_MSG(MMIDM_PL_COMM_1071_112_2_18_2_14_10_493,"MMIDM==> [VDM_Comm_PL_HTTP_receive]  bufSize = %d, s_recvActualLen = %d\r\n")
TRACE_MSG(MMIDM_PL_COMM_1075_112_2_18_2_14_10_494,"MMIDM==> [VDM_Comm_PL_HTTP_receive]  Error:RDM_ERR_BUFFER_OVERFLOW!\r\n")
TRACE_MSG(MMIDM_PL_COMM_1083_112_2_18_2_14_10_495,"MMIDM==> [VDM_Comm_PL_HTTP_receive]  return error when cut the http header, t1_ptr=0x%x\r\n")
TRACE_MSG(MMIDM_PL_COMM_1090_112_2_18_2_14_10_496,"MMIDM==> [VDM_Comm_PL_HTTP_receive]  LenHTTPbody=0x%x\r\n")
TRACE_MSG(MMIDM_PL_COMM_1115_112_2_18_2_14_10_497,"MMIDM==> RDM_COMMS_recvMessage return ret =%d\r\n")
TRACE_MSG(MMIDM_PL_COMM_1137_112_2_18_2_14_10_498,"MMIDM==> [VDM_Comm_PL_HTTP_receive]  bufSize = %d, s_recvActualLen = %d\r\n")
TRACE_MSG(MMIDM_PL_COMM_1141_112_2_18_2_14_10_499,"MMIDM==> [VDM_Comm_PL_HTTP_receive]  Error:RDM_ERR_BUFFER_OVERFLOW!\r\n")
TRACE_MSG(MMIDM_PL_COMM_1149_112_2_18_2_14_10_500,"MMIDM==> [VDM_Comm_PL_HTTP_receive]  return error when cut the http header, t1_ptr=0x%x\r\n")
TRACE_MSG(MMIDM_PL_COMM_1156_112_2_18_2_14_10_501,"MMIDM==> [VDM_Comm_PL_HTTP_receive]  LenHTTPbody=0x%x\r\n")
TRACE_MSG(MMIDM_PL_COMM_1165_112_2_18_2_14_10_502,"MMIDM==> RDM_COMMS_recvMessage return ret =%d\r\n")
TRACE_MSG(MMIDM_PL_COMM_1183_112_2_18_2_14_10_503,"MMIDM==>  VDM_Comm_PL_HTTP_close (connId = %d) returns\n")
TRACE_MSG(MMIDM_PL_COMM_1195_112_2_18_2_14_10_504,"MMIDM==>  VDM_Comm_PL_HTTP_term called\n")
TRACE_MSG(MMIDM_PL_COMM_1232_112_2_18_2_14_10_505,"5")
TRACE_MSG(MMIDM_PL_COMM_1239_112_2_18_2_14_10_506,"MMIDM==> VDM_Comm_PL_HTTP_term returns\n")
TRACE_MSG(MMIDM_PL_COMM_1256_112_2_18_2_14_10_507,"MMIDM==> [RDM_Socket_Send], total_len = %d")
TRACE_MSG(MMIDM_PL_COMM_1270_112_2_18_2_14_10_508,"MMIDM==> [RDM_Socket_Send], length = 0")
TRACE_MSG(MMIDM_PL_COMM_1275_112_2_18_2_14_10_509,"MMIDM==> [RDM_Socket_Send], sent_len = %d")
TRACE_MSG(MMIDM_PL_COMM_1315_112_2_18_2_14_10_510,"MMIDM==> [RDM_Socket_open] url = %s \r\n")
TRACE_MSG(MMIDM_PL_COMM_1336_112_2_18_2_14_10_511,"MMIDM==> [RDM_Socket_Connect_Server] gv_Rdm_Socket = 0x%x \r\n")
TRACE_MSG(MMIDM_PL_COMM_1352_112_2_18_2_14_10_512,"MMIDM==> RDM_Socket_Connect_Server, has http max_len= %d")
TRACE_MSG(MMIDM_PL_COMM_1359_112_2_18_2_14_10_513,"MMIDM==> RDM_Socket_Connect_Server, has https max_len= %d")
TRACE_MSG(MMIDM_PL_COMM_1364_112_2_18_2_14_10_514,"MMIDM==> RDM_Socket_Connect_Server, has not http and https\r\n")
TRACE_MSG(MMIDM_PL_COMM_1368_112_2_18_2_14_11_515,"MMIDM==> RDM_Socket_Connect_Server, ServerAddress = %s\r\n")
TRACE_MSG(MMIDM_PL_COMM_1375_112_2_18_2_14_11_516,"MMIDM==> RDM_Socket_Connect_Server, ServerAddress delete / = %s\r\n")
TRACE_MSG(MMIDM_PL_COMM_1386_112_2_18_2_14_11_517,"MMIDM==> RDM_Socket_Connect_Server, ServerAddress delete : = %s, portNumber = %d\r\n")
TRACE_MSG(MMIDM_PL_COMM_1402_112_2_18_2_14_11_518,"MMIDM==> RDM_Socket_Connect_Server no ip[%s],port=[%d]\r\n")
TRACE_MSG(MMIDM_PL_COMM_1406_112_2_18_2_14_11_519,"MMIDM==> [RDM_Socket_Connect_Server], ret = %x\r\n")
TRACE_MSG(MMIDM_PL_COMM_1416_112_2_18_2_14_11_520,"MMIDM==> [RDM_Socket_Create], socket_id = %x, netid = %drn")
TRACE_MSG(MMIDM_PL_COMM_1433_112_2_18_2_14_11_521,"MMIDM==> [RDM_Socket_Create], iError = %x")
TRACE_MSG(MMIDM_PL_COMM_1466_112_2_18_2_14_11_522,"MMIDM==> RDM_UrlIsAnIpAddress, url = %s\r\n")
TRACE_MSG(MMIDM_PL_COMM_1467_112_2_18_2_14_11_523,"MMIDM==> RDM_UrlIsAnIpAddress, IPaddress = %s\r\n")
TRACE_MSG(MMIDM_PL_COMM_1469_112_2_18_2_14_11_524,"MMIDM==> RDM_UrlIsAnIpAddress, v_len = %d\r\n")
TRACE_MSG(MMIDM_PL_COMM_1475_112_2_18_2_14_11_525,"MMIDM==> RDM_UrlIsAnIpAddress ---url is a domain name\r\n")
TRACE_MSG(MMIDM_PL_COMM_1479_112_2_18_2_14_11_526,"RDM_UrlIsAnIpAddress ---url is an dot decimal ip\r\n")
TRACE_MSG(MMIDM_PL_COMM_1489_112_2_18_2_14_11_527,"MMIDM==> [RDM_Socket_Connect_by_ip], ip = %s,port = %d\r\n")
TRACE_MSG(MMIDM_PL_COMM_1500_112_2_18_2_14_11_528,"[RDM_Socket_Connect_by_ip], ret = %d\r\n")
TRACE_MSG(MMIDM_PL_COMM_1512_112_2_18_2_14_11_529,"MMIDM==> [RDM_Socket_Connect_by_name], server_name = %s,port = %d\r\n")
TRACE_MSG(MMIDM_PL_COMM_1533_112_2_18_2_14_11_530,"[RDM_Socket_Connect_by_name], ret = %d\r\n")
TRACE_MSG(MMIDM_PL_COMM_1539_112_2_18_2_14_11_531,"MMIDM==> VDM_PL_DD_field_value: inField=%s, inValue=%s")
TRACE_MSG(MMIDM_PL_COMM_1553_112_2_18_2_14_11_532,"MMIDM==> VDM_HandleSocketMessage type 0x%x ,sock_id = 0x%x dm=0x%x,dl=0x%x")
TRACE_MSG(MMIDM_PL_COMM_1557_112_2_18_2_14_11_533,"MMIDM==> SOCKET_STATE_READ")
TRACE_MSG(MMIDM_PL_COMM_1570_112_2_18_2_14_11_534,"MMIDM==> SOCKET_STATE_WRITE")
TRACE_MSG(MMIDM_PL_COMM_1577_112_2_18_2_14_11_535,"begin to write xml")
TRACE_MSG(MMIDM_PL_COMM_1587_112_2_18_2_14_11_536,"MMIDM==> SOCKET_STATE_CONNECT")
TRACE_MSG(MMIDM_PL_COMM_1595_112_2_18_2_14_11_537,"MMIDM_HandleSocketMessage DM_CANCEL == MMIDM_GetDmState()")
TRACE_MSG(MMIDM_PL_COMM_1611_112_2_18_2_14_11_538,"MMIDM==> SOCKET_STATE_CLOSE")
TRACE_MSG(MMIDM_PL_COMM_1672_112_2_18_2_14_11_539,"MMIDM==> RDM_isWholeMsgArrived: starting (0x%x, %d)\n")
TRACE_MSG(MMIDM_PL_COMM_1676_112_2_18_2_14_11_540,"MMIDM==> RDM_isWholeMsgArrived: found EOM (0x%x)\n")
TRACE_MSG(MMIDM_PL_COMM_1682_112_2_18_2_14_11_541,"MMIDM==> RDM_isWholeMsgArrived: found ContentLength (%d)\n")
TRACE_MSG(MMIDM_PL_COMM_1685_112_2_18_2_14_11_542,"MMIDM==> RDM_isWholeMsgArrived: no ContentLength\n")
TRACE_MSG(MMIDM_PL_COMM_1691_112_2_18_2_14_11_543,"MMIDM==> RDM_isWholeMsgArrived: inMsgPos %d")
TRACE_MSG(MMIDM_PL_COMM_1695_112_2_18_2_14_11_544,"MMIDM==> RDM_isWholeMsgArrived: found Whole message\n")
TRACE_MSG(MMIDM_PL_COMM_1714_112_2_18_2_14_11_545,"RDM_COMMS_Data_Arrive PNULL == buf")
TRACE_MSG(MMIDM_PL_COMM_1721_112_2_18_2_14_11_546,"MMIDM==> RDM_COMMS_Data_Arrive socket_id=0x%x\n")
TRACE_MSG(MMIDM_PL_COMM_1724_112_2_18_2_14_11_547,"MMIDM==> RDM_COMMS_Data_Arrive s_recvActualLen=%d\n")
TRACE_MSG(MMIDM_PL_COMM_1747_112_2_18_2_14_11_548,"RDM_COMMS_Data_Arrive the receiver content")
TRACE_MSG(MMIDM_PL_COMM_1830_112_2_18_2_14_11_549,"MMIDM==> MMIDM_BrokenDMSession dl socket session ")
TRACE_MSG(MMIDM_PL_COMM_1836_112_2_18_2_14_11_550,"MMIDM==> MMIDM_BrokenDMSession dm socket session ")
TRACE_MSG(MMIDM_PL_MMI_877_112_2_18_2_14_14_551,"MMIDM==> VDM_MMI_PL_init fail!")
TRACE_MSG(MMIDM_PL_MMI_891_112_2_18_2_14_14_552,"MMIDM==> VDM_MMI_PL_term inInfoType ")
TRACE_MSG(MMIDM_PL_MMI_921_112_2_18_2_14_14_553,"MMIDM==> VDM_MMI_PL_infoMessage inInfoType=%d")
TRACE_MSG(MMIDM_PL_MMI_948_112_2_18_2_14_14_554,"MMIDM==> VDM_MMI_PL_infoMessage inDefaultCommand = %d")
TRACE_MSG(MMIDM_PL_MMI_951_112_2_18_2_14_14_555,"MMIDM==> VDM_MMI_PL_confirmationQuery alert_type %d")
TRACE_MSG(MMIDM_PL_MMI_976_112_2_18_2_14_14_556,"MMIDM==> VDM_MMI_PL_inputQuery maxLength = %d")
TRACE_MSG(MMIDM_PL_MMI_1002_112_2_18_2_14_14_557,"MMIDM==> VDM_MMI_PL_choiceListQuery isMultipleSelection = %d, itemsCount=%d")
TRACE_MSG(MMIDM_PL_MMI_1090_112_2_18_2_14_14_558,"MMIDM==> VDM_MMI_PL_updateProgress inCurrentProgress = %d, inMaxProgress=%d")
TRACE_MSG(MMIDM_PL_MMI_1124_112_2_18_2_14_14_559,"MMIDM==> DM_SessionStateChange inType=%d,inState=%d,inLastError=%d ")
TRACE_MSG(MMIDM_PL_MMI_1160_112_2_18_2_14_14_560,"MMIDM==> MMIDM_FreeAllMOInstance")
TRACE_MSG(MMIDM_PL_MMI_1168_112_2_18_2_14_14_561,"MMIDM==> MMIDM_FreeAllMOInstance exit")
TRACE_MSG(MMIDM_PL_MMI_1182_112_2_18_2_14_14_562,"MMIDM==> MMIDM_StartVDM start")
TRACE_MSG(MMIDM_PL_MMI_1193_112_2_18_2_14_14_563,"MMIDM==> MMIDM_StartVDM alert_type %d")
TRACE_MSG(MMIDM_PL_MMI_1208_112_2_18_2_14_14_564,"MMIDM==> Failed to create vDM: error 0x%x")
TRACE_MSG(MMIDM_PL_MMI_1239_112_2_18_2_14_15_565,"MMIDM==> Failed to start vDM: error 0x%x")
TRACE_MSG(MMIDM_PL_MMI_1247_112_2_18_2_14_15_566,"MMIDM==> Failed to register callbacks: error 0x%x")
TRACE_MSG(MMIDM_PL_MMI_1255_112_2_18_2_14_15_567,"MMIDM==> Failed to initialize plug-ins: error 0x%x")
TRACE_MSG(MMIDM_PL_MMI_1268_112_2_18_2_14_15_568,"MMIDM==> MMIDM_StarVDM Failed to trigger session: error 0x%x")
TRACE_MSG(MMIDM_PL_MMI_1296_112_2_18_2_14_15_569,"MMIDM==> Failed to run vDM: error 0x%x")
TRACE_MSG(MMIDM_PL_MMI_1300_112_2_18_2_14_15_570,"MMIDM==> MMIDM_StarVDM end")
TRACE_MSG(MMIDM_PL_MMI_1311_112_2_18_2_14_15_571,"MMIDM==> MMIDM_CreateThread")
TRACE_MSG(MMIDM_PL_MMI_1323_112_2_18_2_14_15_572,"MMIDM==> DM_registerCallbacks")
TRACE_MSG(MMIDM_PL_MMI_1369_112_2_18_2_14_15_573,"MMIDM==> VDMC_PluginMng_init")
TRACE_MSG(MMIDM_PL_MMI_1374_112_2_18_2_14_15_574,"MMIDM==> Failed to initialize FUMO plugin: error 0x%x")
TRACE_MSG(MMIDM_PL_MMI_1381_112_2_18_2_14_15_575,"MMIDM==> Failed to initialize SCOMO plugin: error 0x%x")
TRACE_MSG(MMIDM_PL_MMI_1388_112_2_18_2_14_15_576,"MMIDM==> Failed to initialize LAWMO plugin: error 0x%x")
TRACE_MSG(MMIDM_PL_MMI_1403_112_2_18_2_14_15_577,"MMIDM==> FUMO_Plugin_init")
TRACE_MSG(MMIDM_PL_MMI_1423_112_2_18_2_14_15_578,"MMIDM==> FUMO_Plugin_term")
TRACE_MSG(MMIDM_PL_MMI_1440_112_2_18_2_14_15_579,"MMIDM==> DM_FumoConfirmUpdate")
TRACE_MSG(MMIDM_PL_MMI_1457_112_2_18_2_14_15_580,"MMIDM==> DM_FumoExecuteUpdate")
TRACE_MSG(MMIDM_PL_MMI_1472_112_2_18_2_14_15_581,"MMIDM==> SCOMO_Plugin_init")
TRACE_MSG(MMIDM_PL_MMI_1530_112_2_18_2_14_15_582,"MMIDM==> DM_OnScomoConfirmInstallation inOperationPath %s")
TRACE_MSG(MMIDM_PL_MMI_1549_112_2_18_2_14_15_583,"MMIDM==> DM_OnScomoConfirmInstallation not support java!")
TRACE_MSG(MMIDM_PL_MMI_1555_112_2_18_2_14_15_584,"MMIDM==> DM_OnScomoConfirmInstallation MMIDM_ExtractAPPFile error !!")
TRACE_MSG(MMIDM_PL_MMI_1577_112_2_18_2_14_15_585,"MMIDM==> DM_GetDeviceModel ")
TRACE_MSG(MMIDM_PL_MMI_1592_112_2_18_2_14_15_586,"MMIDM==> VDMC_GetVdmMan ")
TRACE_MSG(MMIDM_PL_MMI_1606_112_2_18_2_14_15_587,"MMIDM==> VDMC_GetVdmOEM ")
TRACE_MSG(MMIDM_PL_MMI_1620_112_2_18_2_14_15_588,"MMIDM==> VDMC_GetVdmLanguage ")
TRACE_MSG(MMIDM_PL_MMI_1636_112_2_18_2_14_15_589,"MMIDM==> VDMC_GetVdmMan ")
TRACE_MSG(MMIDM_PL_MMI_1651_112_2_18_2_14_15_590,"MMIDM==> VDMC_GetDevImei ")
TRACE_MSG(MMIDM_PL_MMI_1665_112_2_18_2_14_15_591,"MMIDM==> DM_GetDeviceFwV ")
TRACE_MSG(MMIDM_PL_MMI_1681_112_2_18_2_14_15_592,"MMIDM==> DM_GetDeviceSwV ")
TRACE_MSG(MMIDM_PL_MMI_1696_112_2_18_2_14_15_593,"MMIDM==> DM_GetDeviceHwV ")
TRACE_MSG(MMIDM_PL_MMI_1711_112_2_18_2_14_15_594,"MMIDM==> DM_GetStringNULL ")
TRACE_MSG(MMIDM_PL_MMI_1725_112_2_18_2_14_15_595,"MMIDM==> DM_GetDmProfile ")
TRACE_MSG(MMIDM_PL_MMI_1738_112_2_18_2_14_16_596,"MMIDM==> DM_SetDmProfile ")
TRACE_MSG(MMIDM_PL_MMI_1745_112_2_18_2_14_16_597,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_1759_112_2_18_2_14_16_598,"MMIDM==> DM_GetGprsCMNetAPN ")
TRACE_MSG(MMIDM_PL_MMI_1773_112_2_18_2_14_16_599,"MMIDM==> DM_SetGprsCMNetAPN ")
TRACE_MSG(MMIDM_PL_MMI_1780_112_2_18_2_14_16_600,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_1794_112_2_18_2_14_16_601,"MMIDM==> DM_GetGprsCMNetUsername ")
TRACE_MSG(MMIDM_PL_MMI_1807_112_2_18_2_14_16_602,"MMIDM==> DM_SetGprsCMNetUsername ")
TRACE_MSG(MMIDM_PL_MMI_1814_112_2_18_2_14_16_603,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_1828_112_2_18_2_14_16_604,"MMIDM==> DM_GetGprsCMNePw ")
TRACE_MSG(MMIDM_PL_MMI_1842_112_2_18_2_14_16_605,"MMIDM==> DM_SetGprsCMNetPw ")
TRACE_MSG(MMIDM_PL_MMI_1849_112_2_18_2_14_16_606,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_1863_112_2_18_2_14_16_607,"MMIDM==> DM_GetGprsCMNetProxyaddr ")
TRACE_MSG(MMIDM_PL_MMI_1875_112_2_18_2_14_16_608,"MMIDM==> DM_SetGprsCMNetProxyaddr ")
TRACE_MSG(MMIDM_PL_MMI_1882_112_2_18_2_14_16_609,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_1896_112_2_18_2_14_16_610,"MMIDM==> DM_GetGprsCMNetPortNum ")
TRACE_MSG(MMIDM_PL_MMI_1908_112_2_18_2_14_16_611,"MMIDM==> DM_SetGprsCMNetUsername ")
TRACE_MSG(MMIDM_PL_MMI_1915_112_2_18_2_14_16_612,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_1929_112_2_18_2_14_16_613,"MMIDM==> DM_GetGprsAPN ")
TRACE_MSG(MMIDM_PL_MMI_1941_112_2_18_2_14_16_614,"MMIDM==> DM_SetGprsAPN ")
TRACE_MSG(MMIDM_PL_MMI_1949_112_2_18_2_14_16_615,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_1962_112_2_18_2_14_16_616,"MMIDM==> DM_GetGprsUsername ")
TRACE_MSG(MMIDM_PL_MMI_1975_112_2_18_2_14_16_617,"MMIDM==> DM_SetGprsUsername ")
TRACE_MSG(MMIDM_PL_MMI_1983_112_2_18_2_14_16_618,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_1997_112_2_18_2_14_16_619,"MMIDM==> DM_GetGprsPw ")
TRACE_MSG(MMIDM_PL_MMI_2010_112_2_18_2_14_16_620,"MMIDM==> DM_SetGprsPw ")
TRACE_MSG(MMIDM_PL_MMI_2018_112_2_18_2_14_16_621,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2032_112_2_18_2_14_16_622,"MMIDM==> DM_GetGprsCMWapProxyaddr ")
TRACE_MSG(MMIDM_PL_MMI_2045_112_2_18_2_14_16_623,"MMIDM==> DM_SetGprsCMWapProxyaddr ")
TRACE_MSG(MMIDM_PL_MMI_2053_112_2_18_2_14_16_624,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2067_112_2_18_2_14_16_625,"MMIDM==> DM_GetGprsCMWapPortNum ")
TRACE_MSG(MMIDM_PL_MMI_2080_112_2_18_2_14_16_626,"MMIDM==> DM_SetGprsCMWapPortNum ")
TRACE_MSG(MMIDM_PL_MMI_2088_112_2_18_2_14_16_627,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2102_112_2_18_2_14_16_628,"MMIDM==> DM_GetMMSMmsc ")
TRACE_MSG(MMIDM_PL_MMI_2116_112_2_18_2_14_16_629,"MMIDM==> DM_SetMMSMmsc ")
TRACE_MSG(MMIDM_PL_MMI_2124_112_2_18_2_14_16_630,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2137_112_2_18_2_14_16_631,"MMIDM==> DM_GetMMSProfile ")
TRACE_MSG(MMIDM_PL_MMI_2150_112_2_18_2_14_16_632,"MMIDM==> DM_SetMMSProfile ")
TRACE_MSG(MMIDM_PL_MMI_2158_112_2_18_2_14_16_633,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2172_112_2_18_2_14_16_634,"MMIDM==> DM_GetWAPConprofile ")
TRACE_MSG(MMIDM_PL_MMI_2185_112_2_18_2_14_16_635,"MMIDM==> DM_SetWAPConprofile ")
TRACE_MSG(MMIDM_PL_MMI_2193_112_2_18_2_14_16_636,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2206_112_2_18_2_14_16_637,"MMIDM==> DM_GetWAPStarPage ")
TRACE_MSG(MMIDM_PL_MMI_2219_112_2_18_2_14_17_638,"MMIDM==> DM_SetWAPStarPage ")
TRACE_MSG(MMIDM_PL_MMI_2227_112_2_18_2_14_17_639,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2241_112_2_18_2_14_17_640,"MMIDM==> DM_GetPIMAddr ")
TRACE_MSG(MMIDM_PL_MMI_2254_112_2_18_2_14_17_641,"MMIDM==> DM_SetPIMAddr ")
TRACE_MSG(MMIDM_PL_MMI_2262_112_2_18_2_14_17_642,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2275_112_2_18_2_14_17_643,"MMIDM==> DM_GetPIMAddressBookURL ")
TRACE_MSG(MMIDM_PL_MMI_2288_112_2_18_2_14_17_644,"MMIDM==> DM_SetPIMAddressBookURL ")
TRACE_MSG(MMIDM_PL_MMI_2296_112_2_18_2_14_17_645,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2309_112_2_18_2_14_17_646,"MMIDM==> DM_GetPIMCalendarURL ")
TRACE_MSG(MMIDM_PL_MMI_2324_112_2_18_2_14_17_647,"MMIDM==> DM_GetSrvAddrURL ")
TRACE_MSG(MMIDM_PL_MMI_2335_112_2_18_2_14_17_648,"MMIDM==> DM_GetDeviceLockStatus")
TRACE_MSG(MMIDM_PL_MMI_2348_112_2_18_2_14_17_649,"MMIDM==> DM_SetPIMCalendarURL ")
TRACE_MSG(MMIDM_PL_MMI_2356_112_2_18_2_14_17_650,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2370_112_2_18_2_14_17_651,"MMIDM==> DM_GetStreamingName ")
TRACE_MSG(MMIDM_PL_MMI_2383_112_2_18_2_14_17_652,"MMIDM==> DM_SetStreamingName ")
TRACE_MSG(MMIDM_PL_MMI_2391_112_2_18_2_14_17_653,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2405_112_2_18_2_14_17_654,"MMIDM==> DM_GetStreamingProxy ")
TRACE_MSG(MMIDM_PL_MMI_2417_112_2_18_2_14_17_655,"MMIDM==> DM_SetStreamingProxy ")
TRACE_MSG(MMIDM_PL_MMI_2425_112_2_18_2_14_17_656,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2439_112_2_18_2_14_17_657,"MMIDM==> DM_GetStreamingProxyPort ")
TRACE_MSG(MMIDM_PL_MMI_2451_112_2_18_2_14_17_658,"MMIDM==> DM_SetStreamingProxyPort ")
TRACE_MSG(MMIDM_PL_MMI_2459_112_2_18_2_14_17_659,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2472_112_2_18_2_14_17_660,"MMIDM==> DM_GetStreamingNapID ")
TRACE_MSG(MMIDM_PL_MMI_2484_112_2_18_2_14_17_661,"MMIDM==> DM_SetStreamingNapID ")
TRACE_MSG(MMIDM_PL_MMI_2492_112_2_18_2_14_17_662,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2505_112_2_18_2_14_17_663,"MMIDM==> DM_GetStreamingNetInfo ")
TRACE_MSG(MMIDM_PL_MMI_2517_112_2_18_2_14_17_664,"MMIDM==> DM_SetStreamingNetInfo ")
TRACE_MSG(MMIDM_PL_MMI_2525_112_2_18_2_14_17_665,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2538_112_2_18_2_14_17_666,"MMIDM==> DM_GetStreamingMinUDPPort ")
TRACE_MSG(MMIDM_PL_MMI_2550_112_2_18_2_14_17_667,"MMIDM==> DM_SetStreamingMinUDPPort ")
TRACE_MSG(MMIDM_PL_MMI_2558_112_2_18_2_14_17_668,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2571_112_2_18_2_14_17_669,"MMIDM==> DM_GetStreamingMaxUDPPort ")
TRACE_MSG(MMIDM_PL_MMI_2583_112_2_18_2_14_17_670,"MMIDM==> DM_SetStreamingName ")
TRACE_MSG(MMIDM_PL_MMI_2591_112_2_18_2_14_17_671,"MMIDM==> VDM_ERR_BUFFER_OVERFLOW")
TRACE_MSG(MMIDM_PL_MMI_2642_112_2_18_2_14_17_672,"MMIDM==>  DM_initiateSession type =%d")
TRACE_MSG(MMIDM_PL_MMI_2698_112_2_18_2_14_17_673,"MMIDM==>  NIA_Plugin_handleAlert inUIMode =%d")
TRACE_MSG(MMIDM_PL_MMI_2747_112_2_18_2_14_18_674,"MMIDM==> DM_DlResumeGetFunc %s")
TRACE_MSG(MMIDM_PL_MMI_2751_112_2_18_2_14_18_675,"MMIDM==> DM_DlResumeGetFunc: Failed to allocate memory.")
TRACE_MSG(MMIDM_PL_MMI_2779_112_2_18_2_14_18_676,"MMIDM==>  DM_DlResumeGetFunc: Failed to open %s.")
TRACE_MSG(MMIDM_PL_MMI_2790_112_2_18_2_14_18_677,"MMIDM==>  DM_DlResumeGetFunc error =%d,read_size=%d,sizeof(s_vdm_resume) size=%d")
TRACE_MSG(MMIDM_PL_MMI_2803_112_2_18_2_14_18_678,"MMIDM==>  DM_DlResumeGetFunc pResumeTag = %s, offset = %d.")
TRACE_MSG(MMIDM_PL_MMI_2846_112_2_18_2_14_18_679,"MMIDM==> DM_DLResumeSetFunc %s")
TRACE_MSG(MMIDM_PL_MMI_2855_112_2_18_2_14_18_680,"MMIDM==>  DM_DLResumeSetFunc open file fail")
TRACE_MSG(MMIDM_PL_MMI_2873_112_2_18_2_14_18_681,"MMIDM==>  DM_DLResumeSetFunc error =%d,write_size=%d,sizeof(s_vdm_resume) size=%d")
TRACE_MSG(MMIDM_PL_MMI_2906_112_2_18_2_14_18_682,"MMIDM==> MMIDM_RemoveResumeFile ")
TRACE_MSG(MMIDM_PL_MMI_2922_112_2_18_2_14_18_683,"MMIDM==>  MMIDM_RemoveResumeFile: DL resume file %s result=%d")
TRACE_MSG(MMIDM_PL_MMI_2932_112_2_18_2_14_18_684,"MMIDM==> MMIDM_DeleteAllResumeFile ")
TRACE_MSG(MMIDM_PL_MMI_2936_112_2_18_2_14_18_685,"MMIDM==> MMIDM_DeleteAllResumeFile ")
TRACE_MSG(MMIDM_PL_MMI_2992_112_2_18_2_14_18_686,"MMIDM==> DM_IsResumeFileExist ")
TRACE_MSG(MMIDM_PL_MMI_2999_112_2_18_2_14_18_687,"MMIDM==> %s")
TRACE_MSG(MMIDM_PL_MMI_3036_112_2_18_2_14_18_688,"MMIDM==>  SendSignalToMMITask , sig_id is %x")
TRACE_MSG(MMIDM_PL_MMI_3046_112_2_18_2_14_18_689,"MMIDM==>  DM_ConfirmPartiallyLockCB To do Lock operation")
TRACE_MSG(MMIDM_PL_MMI_3058_112_2_18_2_14_18_690,"MMIDM==>  DM_ConfirmPartiallyLock do Lock operation")
TRACE_MSG(MMIDM_PL_MMI_3069_112_2_18_2_14_18_691,"MMIDM==>  DM_ConfirmUnLockCB do UnLock operation")
TRACE_MSG(MMIDM_PL_MMI_3081_112_2_18_2_14_18_692,"MMIDM==>  DM_ConfirmUnLock do UnLock operation")
TRACE_MSG(MMIDM_PL_MMI_3092_112_2_18_2_14_18_693,"MMIDM==>  DM_ConfirmFactoryResetCB To do FactoryReset operation")
TRACE_MSG(MMIDM_PL_MMI_3104_112_2_18_2_14_18_694,"MMIDM==>  DM_ConfirmFactoryReset To do FactoryReset operation")
TRACE_MSG(MMIDM_PL_MMI_3131_112_2_18_2_14_18_695,"MMIDM==>  LAWMO_Plugin_init Enter, LAWMO_Plugin_init")
TRACE_MSG(MMIDM_PL_MMI_3139_112_2_18_2_14_18_696,"MMIDM==>  LAWMO_Plugin_init = 0x%x")
TRACE_MSG(MMIDM_PL_MMI_3158_112_2_18_2_14_18_697,"MMIDM==>  LAWMO_Plugin_term ")
TRACE_MSG(MMIDM_PL_MMI_3169_112_2_18_2_14_18_698,"MMIDM==>  DM_LawmoSetLockState: result= 0x%x")
TRACE_MSG(MMIDM_PL_MMI_3197_112_2_18_2_14_18_699,"MMIDM==>  SCOMO confirmUninstall")
TRACE_MSG(MMIDM_PL_MMI_3211_112_2_18_2_14_19_700,"MMIDM==>  DM_OnScomoReadyToRemove")
TRACE_MSG(MMIDM_PL_MMI_3235_112_2_18_2_14_19_701,"MMIDM==> SCOMO_Plugin_initDCsall PNULL == pPlugin")
TRACE_MSG(MMIDM_PL_MMI_3265_112_2_18_2_14_19_702,"MMIDM==>  SCOMO_Plugin_initDCsall name: %s")
TRACE_MSG(MMIDM_PL_MMI_3283_112_2_18_2_14_19_703,"MMIDM==>  SCOMO_Plugin_initDCsall name")
TRACE_MSG(MMIDM_PL_MMI_3297_112_2_18_2_14_19_704,"MMIDM==>  SCOMO_Plugin_initDCsall id: %s")
TRACE_MSG(MMIDM_PL_MMI_3299_112_2_18_2_14_19_705,"MMIDM==>  SCOMO_Plugin_initDCsall name id=%d")
TRACE_MSG(MMIDM_PL_MMI_3343_112_2_18_2_14_19_706,"MMIDM==> DM_ResumeScomoDLSess")
TRACE_MSG(MMIDM_PL_MMI_3392_112_2_18_2_14_19_707,"MMIDM==>  DM_GetScomoResultCode: Unknow SCOMO result code!")
TRACE_MSG(MMIDM_PL_MMI_3425_112_2_18_2_14_19_708,"MMIDM==> DM_SetDPInstallResult: result = %d")
TRACE_MSG(MMIDM_PL_MMI_3429_112_2_18_2_14_19_709,"MMIDM==> DM_SetDPInstallResult PNULL")
TRACE_MSG(MMIDM_PL_MMI_3437_112_2_18_2_14_19_710,"MMIDM==>  MMIDM_IU32toa")
TRACE_MSG(MMIDM_PL_MMI_3443_112_2_18_2_14_19_711,"MMIDM==>  MMIDM_SCOMO_PL_INV_addComponent ret=0x%x")
TRACE_MSG(MMIDM_PL_MMI_3452_112_2_18_2_14_19_712,"MMIDM==>  VDM_ERR_NODE_ALREADY_EXISTS del_ret =%d")
TRACE_MSG(MMIDM_PL_MMI_3459_112_2_18_2_14_19_713,"MMIDM==>  VDM_ERR_NODE_ALREADY_EXISTS ret =%d")
TRACE_MSG(MMIDM_PL_MMI_3463_112_2_18_2_14_19_714,"MMIDM==> DM_SetDPInstallResult Failed to add component to inventory! error = 0x%x")
TRACE_MSG(MMIDM_PL_MMI_3469_112_2_18_2_14_19_715,"MMIDM==> DM_SetDPInstallResult added to inventory")
TRACE_MSG(MMIDM_PL_MMI_3479_112_2_18_2_14_19_716,"MMIDM==> DM_SetDPInstallResult created instance")
TRACE_MSG(MMIDM_PL_MMI_3486_112_2_18_2_14_19_717,"MMIDM==>  MMIDM_SCOMO_DP_setInstallationResult ret =0x%x")
TRACE_MSG(MMIDM_PL_MMI_3492_112_2_18_2_14_19_718,"MMIDM==>  MMIDM_SCOMO_DP_setInstallationResult ret 1=0x%x")
TRACE_MSG(MMIDM_PL_MMI_3509_112_2_18_2_14_19_719,"MMIDM==>  DM_RemoveDCFromInventory %d")
TRACE_MSG(MMIDM_PL_MMI_3513_112_2_18_2_14_19_720,"MMIDM==>  DM_RemoveDCFromInventory PNULL")
TRACE_MSG(MMIDM_PL_MMI_3529_112_2_18_2_14_19_721,"MMIDM==> DM_RemoveDCFromInventory: Selected DC has been removed.1")
TRACE_MSG(MMIDM_PL_MMI_3538_112_2_18_2_14_19_722,"MMIDM==>  DM_RemoveDCFromInventory p=%x")
TRACE_MSG(MMIDM_PL_MMI_3557_112_2_18_2_14_19_723,"MMIDM==> DM_RemoveDCFromInventory: Selected DC has been removed.2")
TRACE_MSG(MMIDM_PL_MMI_3561_112_2_18_2_14_19_724,"MMIDM==> DM_RemoveDCFromInventory: Cannot find selected DC in inventory.")
TRACE_MSG(MMIDM_PL_MMI_3584_112_2_18_2_14_19_725,"MMIDM==>  SCOMO_Plugin_create")
TRACE_MSG(MMIDM_PL_MMI_3597_112_2_18_2_14_19_726,"MMIDM==>  SCOMO_Plugin_term")
TRACE_MSG(MMIDM_PL_MMI_3616_112_2_18_2_14_19_727,"MMIDM==> SCOMO_Plugin_term temp=%x")
TRACE_MSG(MMIDM_PL_MMI_3622_112_2_18_2_14_19_728,"MMIDM==> SCOMO_Plugin_term destroy dp")
TRACE_MSG(MMIDM_PL_MMI_3626_112_2_18_2_14_19_729,"MMIDM==>  SCOMO_Plugin_term instance->scomoDP instance->scomoDP %x")
TRACE_MSG(MMIDM_PL_MMI_3630_112_2_18_2_14_19_730,"MMIDM==> SCOMO_Plugin_term 2")
TRACE_MSG(MMIDM_PL_MMI_3633_112_2_18_2_14_19_731,"MMIDM==> SCOMO_Plugin_term 3")
TRACE_MSG(MMIDM_PL_MMI_3637_112_2_18_2_14_19_732,"MMIDM==>  SCOMO_Plugin_term done")
TRACE_MSG(MMIDM_PL_MMI_3646_112_2_18_2_14_19_733,"MMIDM==>  MMIDM_GetDPHeaderLength =%d")
TRACE_MSG(MMIDM_PL_MMI_3656_112_2_18_2_14_19_734,"MMIDM==> downloadType = %d")
TRACE_MSG(MMIDM_PL_MMI_3719_112_2_18_2_14_20_735,"MMIDM==> MMIDM_SetDPHeader PNULL == header")
TRACE_MSG(MMIDM_PL_MMI_3753_112_2_18_2_14_20_736,"MMIDM==> MMIDM_IsBackgroupRun s_UIMode = %d result %d")
TRACE_MSG(MMIDM_PL_MMI_3770_112_2_18_2_14_20_737,"MMIDM==> MMIDM_IsNeedDisplayProgresswin alert_type = %d result %d")
TRACE_MSG(MMIDM_PL_OS_143_112_2_18_2_14_20_738,"MMIDM==> VDM_Client_PL_Dlpkg_allocNameFromId")
TRACE_MSG(MMIDM_PL_OS_221_112_2_18_2_14_20_739,"MMIDM==> VDM_Client_PL_assertFail %s,%d, line=%ld")
TRACE_MSG(MMIDM_PL_OS_254_112_2_18_2_14_20_740,"MMIDM==>VDM_Client_PL_Dlpkg_create PNULL == outDlpkgHandle")
TRACE_MSG(MMIDM_PL_OS_259_112_2_18_2_14_20_741,"MMIDM==> VDM_Client_PL_Dlpkg_create: outDlpkgHandle = %s ")
TRACE_MSG(MMIDM_PL_OS_269_112_2_18_2_14_20_742,"MMIDM==> VDM_Client_PL_Dlpkg_create: len=%d, inURI= %s ")
TRACE_MSG(MMIDM_PL_OS_275_112_2_18_2_14_20_743,"MMIDM==> VDM_Client_PL_Dlpkg_create: inurl_ptr1= %s ")
TRACE_MSG(MMIDM_PL_OS_278_112_2_18_2_14_20_744,"MMIDM==> VDM_Client_PL_Dlpkg_create: inurl_ptr2= %s ")
TRACE_MSG(MMIDM_PL_OS_279_112_2_18_2_14_20_745,"MMIDM==> VDM_Client_PL_Dlpkg_create: file_name_ptr= %s ")
TRACE_MSG(MMIDM_PL_OS_300_112_2_18_2_14_21_746,"MMIDM==> VDM_Client_PL_Dlpkg_create: file_fullname_ptr= %s ")
TRACE_MSG(MMIDM_PL_OS_330_112_2_18_2_14_21_747,"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: inOffset = %d, inDataSize=%d ")
TRACE_MSG(MMIDM_PL_OS_344_112_2_18_2_14_21_748,"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: filename_ptr = %s")
TRACE_MSG(MMIDM_PL_OS_356_112_2_18_2_14_21_749,"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: error=%d ")
TRACE_MSG(MMIDM_PL_OS_363_112_2_18_2_14_21_750,"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: 0 == file_handle ")
TRACE_MSG(MMIDM_PL_OS_370_112_2_18_2_14_21_751,"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: SFS_SEEK error ")
TRACE_MSG(MMIDM_PL_OS_377_112_2_18_2_14_21_752,"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk: Write writelen %d")
TRACE_MSG(MMIDM_PL_OS_383_112_2_18_2_14_21_753,"MMIDM==> VDM_Client_PL_Dlpkg_writeChunk writelen= %d ,result=0x%x")
TRACE_MSG(MMIDM_PL_OS_407_112_2_18_2_14_21_754,"MMIDM==> VDM_Client_PL_Dlpkg_remove")
TRACE_MSG(MMIDM_PL_OS_435_112_2_18_2_14_21_755,"MMIDM==> VDM_Client_PL_Dlpkg_remove result=%d")
TRACE_MSG(MMIDM_PL_OS_449_112_2_18_2_14_21_756,"MMIDM==> VDM_Client_PL_Dlpkg_getMaxSize: result ")
TRACE_MSG(MMIDM_PL_OS_515_112_2_18_2_14_21_757,"MMIDM==> Reg_Save")
TRACE_MSG(MMIDM_PL_OS_520_112_2_18_2_14_21_758,"MMIDM==> Reg_Save PNULL == filename_ptr")
TRACE_MSG(MMIDM_PL_OS_535_112_2_18_2_14_21_759,"MMIDM==> Reg_Save error =%d,write_size=%d,sizeof(MMIDM_REG_T) size=%d")
TRACE_MSG(MMIDM_PL_OS_541_112_2_18_2_14_21_760,"MMIDM==> Reg_Save open file fail")
TRACE_MSG(MMIDM_PL_OS_564_112_2_18_2_14_21_761,"MMIDM==> Reg_Init")
TRACE_MSG(MMIDM_PL_OS_572_112_2_18_2_14_21_762,"MMIDM==> Reg_Init PNULL == s_vdm_reg_ptr")
TRACE_MSG(MMIDM_PL_OS_581_112_2_18_2_14_21_763,"MMIDM==> Reg_Init PNULL == filename_ptr")
TRACE_MSG(MMIDM_PL_OS_596_112_2_18_2_14_21_764,"MMIDM==> Reg_init error =%d,read_size=%d,sizeof(MMIDM_REG_T) size=%d")
TRACE_MSG(MMIDM_PL_OS_603_112_2_18_2_14_21_765,"MMIDM==> Reg_Init open file fail")
TRACE_MSG(MMIDM_PL_OS_607_112_2_18_2_14_21_766,"MMIDM==> Reg_Init end")
TRACE_MSG(MMIDM_PL_OS_619_112_2_18_2_14_21_767,"MMIDM==> Reg_Uninit s_vdm_reg_ptr=0x%x")
TRACE_MSG(MMIDM_PL_OS_637_112_2_18_2_14_21_768,"MMIDM==> VDM_Client_PL_Registry_createKey ")
TRACE_MSG(MMIDM_PL_OS_659_112_2_18_2_14_21_769,"MMIDM==> VDM_Client_PL_Registry_setStringValue,inValueName=%s, inData=%s\n")
TRACE_MSG(MMIDM_PL_OS_667_112_2_18_2_14_22_770,"MMIDM==> VDM_Client_PL_Registry_setStringValue,s=%s\n")
TRACE_MSG(MMIDM_PL_OS_718_112_2_18_2_14_22_771,"MMIDM==> VDM_Client_PL_Registry_setStringValue,key_index=%d")
TRACE_MSG(MMIDM_PL_OS_738_112_2_18_2_14_22_772,"MMIDM==> VDM_Client_PL_Registry_getStringValue,inKey=%s\n")
TRACE_MSG(MMIDM_PL_OS_744_112_2_18_2_14_22_773,"MMIDM==> VDM_Client_PL_Registry_getStringValue,result=%x\n")
TRACE_MSG(MMIDM_PL_OS_747_112_2_18_2_14_22_774,"MMIDM==> VDM_Client_PL_Registry_getStringValue,inValueName=%s\n")
TRACE_MSG(MMIDM_PL_OS_756_112_2_18_2_14_22_775,"MMIDM==> VDM_Client_PL_Registry_getStringValue,result=%x, *ioLength=%d\n")
TRACE_MSG(MMIDM_PL_OS_762_112_2_18_2_14_22_776,"MMIDM==> VDM_Client_PL_Registry_getStringValue,s=%s\n")
TRACE_MSG(MMIDM_PL_OS_768_112_2_18_2_14_22_777,"MMIDM==> VDM_Client_PL_Registry_getStringValue,key_index = %d,reg_ptr->reg_val[key_index]0x%x\n")
TRACE_MSG(MMIDM_PL_OS_771_112_2_18_2_14_22_778,"MMIDM==> VDM_Client_PL_Registry_getStringValue,key_index =%d,reg_ptr->reg_val[key_index]=%s\n")
TRACE_MSG(MMIDM_PL_OS_773_112_2_18_2_14_22_779,"MMIDM==> VDM_Client_PL_Registry_getStringValue 11,*ioLength =%d,outLen=%d\n")
TRACE_MSG(MMIDM_PL_OS_779_112_2_18_2_14_22_780,"MMIDM==> VDM_Client_PL_Registry_getStringValue 22,*ioLength =%d,outLen=%d\n")
TRACE_MSG(MMIDM_PL_OS_791_112_2_18_2_14_22_781,"MMIDM==> VDM_Client_PL_Registry_getStringValue,result=%x")
TRACE_MSG(MMIDM_PL_OS_810_112_2_18_2_14_22_782,"MMIDM==> VDM_Client_PL_Registry_setIntValue inKey=%s,inValueName=%s,inValue =%d")
TRACE_MSG(MMIDM_PL_OS_814_112_2_18_2_14_22_783,"MMIDM==> VDM_Client_PL_Registry_setIntValue strValue =%s")
TRACE_MSG(MMIDM_PL_OS_838_112_2_18_2_14_22_784,"MMIDM==> VDM_Client_PL_Registry_getIntValue ")
TRACE_MSG(MMIDM_PL_OS_840_112_2_18_2_14_22_785,"MMIDM==> VDM_Client_PL_Registry_getIntValue len=%d,strValue=%s")
TRACE_MSG(MMIDM_PL_OS_849_112_2_18_2_14_22_786,"MMIDM==> VDM_Client_PL_Registry_getIntValue ret=%d,bSuccess=%d")
TRACE_MSG(MMIDM_PL_OS_861_112_2_18_2_14_22_787,"MMIDM==> GetItemFilename inType = %d")
TRACE_MSG(MMIDM_PL_OS_887_112_2_18_2_14_22_788,"MMIDM==> GetItemTmpFilename: inType = %d")
TRACE_MSG(MMIDM_PL_OS_930_112_2_18_2_14_22_789,"MMIDM==> VDM_Client_PL_Storage_open inType=%d,inMode=%d")
TRACE_MSG(MMIDM_PL_OS_936_112_2_18_2_14_22_790,"MMIDM==> VDM_Client_PL_Storage_open ! PNULL == storage_item_ptr")
TRACE_MSG(MMIDM_PL_OS_947_112_2_18_2_14_22_791,"MMIDM==> VDM_Client_PL_Storage_open ! PNULL == filename_ptr")
TRACE_MSG(MMIDM_PL_OS_957_112_2_18_2_14_22_792,"MMIDM==> VDM_Client_PL_Storage_open ! PNULL == tempfilename_ptr")
TRACE_MSG(MMIDM_PL_OS_991_112_2_18_2_14_22_793,"MMIDM==> Cannot open file !")
TRACE_MSG(MMIDM_PL_OS_1033_112_2_18_2_14_22_794,"MMIDM==> VDM_Client_PL_Storage_read PNULL ==stream_ptr")
TRACE_MSG(MMIDM_PL_OS_1050_112_2_18_2_14_22_795,"MMIDM==> VDM_Client_PL_Storage_read VDM_ERR_STORAGE_READ")
TRACE_MSG(MMIDM_PL_OS_1087_112_2_18_2_14_22_796,"MMIDM==> VDM_Client_PL_Storage_write VDM_ERR_STORAGE_WRITE 1")
TRACE_MSG(MMIDM_PL_OS_1101_112_2_18_2_14_22_797,"MMIDM==> VDM_Client_PL_Storage_write VDM_ERR_STORAGE_WRITE 2")
TRACE_MSG(MMIDM_PL_OS_1130_112_2_18_2_14_23_798,"MMIDM==> VDM_Client_PL_Storage_close ")
TRACE_MSG(MMIDM_PL_OS_1134_112_2_18_2_14_23_799,"MMIDM==> VDM_Client_PL_Storage_close VDM_ERR_STORAGE_WRITE")
TRACE_MSG(MMIDM_PL_OS_1152_112_2_18_2_14_23_800,"MMIDM==> VDM_Client_PL_Storage_close filename_ptr")
TRACE_MSG(MMIDM_PL_OS_1163_112_2_18_2_14_23_801,"MMIDM==> VDM_Client_PL_Storage_close tempfilename_ptr")
TRACE_MSG(MMIDM_PL_OS_1199_112_2_18_2_14_23_802,"MMIDM==> VDM_Client_PL_Storage_delete inType=%d")
TRACE_MSG(MMIDM_PL_OS_1202_112_2_18_2_14_23_803,"MMIDM==> VDM_Client_PL_Storage_delete PNULL == filename")
TRACE_MSG(MMIDM_PL_OS_1209_112_2_18_2_14_23_804,"MMIDM==> VDM_Client_PL_Storage_delete PNULL == wfilename_ptr")
TRACE_MSG(MMIDM_PL_OS_1219_112_2_18_2_14_23_805,"MMIDM==> VDM_Client_PL_Storage_delete Could not delete wfilename_ptr")
TRACE_MSG(MMIDM_PL_OS_1226_112_2_18_2_14_23_806,"MMIDM==> VDM_Client_PL_Storage_delete PNULL == wtempfilename_ptr")
TRACE_MSG(MMIDM_PL_OS_1242_112_2_18_2_14_23_807,"MMIDM==> VDM_Client_PL_Storage_delete Could not delete tempfilename_ptr")
TRACE_MSG(MMIDM_PL_OS_1326_112_2_18_2_14_23_808,"MMIDM==> VDM_PL_Mutex_create ")
TRACE_MSG(MMIDM_PL_OS_1354_112_2_18_2_14_23_809,"MMIDM==> VDM_PL_Mutex_lock fail")
TRACE_MSG(MMIDM_PL_OS_1377_112_2_18_2_14_23_810,"MMIDM==> VDM_PL_Mutex_unlock fail")
TRACE_MSG(MMIDM_PL_OS_1397_112_2_18_2_14_23_811,"MMIDM==> VDM_PL_Mutex_close success")
TRACE_MSG(MMIDM_PL_OS_1420_112_2_18_2_14_23_812,"MMIDM==> VDM_PL_Async_signal ")
TRACE_MSG(MMIDM_PL_OS_1435_112_2_18_2_14_23_813,"MMIDM==> VDM_PL_Async_yield ")
TRACE_MSG(MMIDM_PL_OS_1451_112_2_18_2_14_23_814,"MMIDM==> VDM_Client_PL_logInit ")
TRACE_MSG(MMIDM_PL_OS_1468_112_2_18_2_14_23_815,"MMIDM==> VDM_Client_PL_logTerm ")
TRACE_MSG(MMIDM_PL_OS_1508_112_2_18_2_14_23_816,"MMIDM==> VDMLIB:%s")
TRACE_MSG(MMIDM_PL_OS_1536_112_2_18_2_14_23_817,"MMIDM==> VDMLIB:%s")
TRACE_MSG(MMIDM_PL_OS_1549_112_2_18_2_14_23_818,"MMIDM==> VDMLIB:%s")
TRACE_MSG(MMIDM_PL_OS_1561_112_2_18_2_14_23_819,"MMIDM==> VDMLIB:%s")
TRACE_MSG(MMIDM_PL_OS_1582_112_2_18_2_14_23_820,"MMIDM==> 0x%x:%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x ")
TRACE_MSG(MMIDM_PL_OS_1585_112_2_18_2_14_23_821,"MMIDM==> %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c")
TRACE_MSG(MMIDM_PL_OS_1598_112_2_18_2_14_23_822,"MMIDM==> VDM_Notify_PL_Task_Finished ")
TRACE_MSG(MMIDM_PL_STRING_196_112_2_18_2_14_24_823,"MMIDM VDM_PL_IU32toa PNULL == string")
TRACE_MSG(MMIDM_TASK_82_112_2_18_2_14_25_824,"MMIDM==> HandleDMPDPMsg PNULL == msg_ptr")
TRACE_MSG(MMIDM_TASK_85_112_2_18_2_14_25_825,"MMIDM==> HandleDMPDPMsg msg_id=%d")
TRACE_MSG(MMIDM_TASK_111_112_2_18_2_14_25_826,"MMIDM==> MMIDM_GetPdpConnectType %d")
TRACE_MSG(MMIDM_TASK_124_112_2_18_2_14_25_827,"MMIDM==> SetPdpConnectType CONNECT_UNKNOW")
TRACE_MSG(MMIDM_TASK_139_112_2_18_2_14_25_828,"MMIDM==> SetPdpConnectType = %d")
TRACE_MSG(MMIDM_TASK_154_112_2_18_2_14_25_829,"MMIDM==> MMIDM_ActivePdpContext dm_apn=%s")
TRACE_MSG(MMIDM_TASK_174_112_2_18_2_14_25_830,"MMIDM==> MMIDM_ActivePdpContext fail")
TRACE_MSG(MMIDM_TASK_191_112_2_18_2_14_26_831,"MMIDM==> MMIAPIPDP_Deactive fail!")
TRACE_MSG(MMIDM_TASK_230_112_2_18_2_14_26_832,"MMIDM==> MMIDM_ActivePdpConnect TIME OUT")
TRACE_MSG(MMIDM_TASK_248_112_2_18_2_14_26_833,"MMIDM==> pdp active result =%d")
TRACE_MSG(MMIDM_TASK_279_112_2_18_2_14_26_834,"DM:task start!")
TRACE_MSG(MMIDM_TASK_286_112_2_18_2_14_26_835,"MMIDM==> task cycle, signal code = 0x%x")
TRACE_MSG(MMIDM_TASK_291_112_2_18_2_14_26_836,"MMIDM==> task DM_TASK_START_MESSAGE ")
TRACE_MSG(MMIDM_TASK_296_112_2_18_2_14_26_837,"MMIDM==> task DM_TASK_START_MESSAGE result=%d")
TRACE_MSG(MMIDM_TASK_319_112_2_18_2_14_26_838,"MMIDM==> task DM_TASK_RUN_MESSAGE ")
TRACE_MSG(MMIDM_TASK_331_112_2_18_2_14_26_839,"MMIDM==> task DM_TASK_DM_CANCEL ")
TRACE_MSG(MMIDM_TASK_335_112_2_18_2_14_26_840,"MMIDM==> task DM_TASK_DM_CLOSE ")
TRACE_MSG(MMIDM_TASK_360_112_2_18_2_14_26_841,"MMIDM==> task DM_TASK_DM_EXIT ")
TRACE_MSG(MMIDM_TASK_380_112_2_18_2_14_26_842,"MMIDM==> task DM_TASK_EXIT_MESSAGE ")
TRACE_MSG(MMIDM_TASK_403_112_2_18_2_14_26_843,"MMIDM==> task SOCKET_READ_EVENT_IND ")
TRACE_MSG(MMIDM_TASK_408_112_2_18_2_14_26_844,"MMIDM==> task SOCKET_WRITE_EVENT_IND ")
TRACE_MSG(MMIDM_TASK_413_112_2_18_2_14_26_845,"MMIDM==> task SOCKET_CONNECT_EVENT_IND ")
TRACE_MSG(MMIDM_TASK_451_112_2_18_2_14_26_846,"MMIDM==> DM_CreateTask, task hasn't been closed!")
TRACE_MSG(MMIDM_TASK_467_112_2_18_2_14_26_847,"MMIDM==> DM_CreateTask, task id = %d")
TRACE_MSG(MMIDM_TASK_487_112_2_18_2_14_26_848,"MMIDM==> get pdp status = %d")
TRACE_MSG(MMIDM_TASK_496_112_2_18_2_14_26_849,"MMIDM==> set pdp status = %d")
TRACE_MSG(MMIDM_WINTAB_624_112_2_18_2_14_28_850,"MMIDM==> enter HandleDmDebugSettingMenuWinMsg ")
TRACE_MSG(MMIDM_WINTAB_886_112_2_18_2_14_28_851,"HandleDmSIMSelectWinMsg,item_data[%d] >= MMI_DUAL_SYS_MAX,ERROR!")
TRACE_MSG(MMIDM_WINTAB_1109_112_2_18_2_14_29_852,"MMIDM==>  HandleDmMainMenuWinMsg: msg_id = 0x%x")
TRACE_MSG(MMIDM_WINTAB_1162_112_2_18_2_14_29_853,"MMIDM==>  OpenDlAlerthandleWin  mode =%d")
TRACE_MSG(MMIDM_WINTAB_1261_112_2_18_2_14_29_854,"MMIDM==> MMIDM_HandWapClose")
TRACE_MSG(MMIDM_WINTAB_1279_112_2_18_2_14_29_855,"MMIDM==> MMIDM_HandDmEventClose")
TRACE_MSG(MMIDM_WINTAB_1349_112_2_18_2_14_29_856,"MMIDM==>  PlayDmNotifyRing  dual_sys =%d")
TRACE_MSG(MMIDM_WINTAB_1407_112_2_18_2_14_29_857,"MMIDM==>  ShowVersionInfo ")
TRACE_MSG(MMIDM_WINTAB_1504_112_2_18_2_14_30_858,"MMIDM==>  HandleDmSWQueryWin: msg_id = 0x%x")
TRACE_MSG(MMIDM_WINTAB_1534_112_2_18_2_14_30_859,"MMIDM==> SoftWareUpdate ")
TRACE_MSG(MMIDM_WINTAB_1563_112_2_18_2_14_30_860,"MMIDM==> SoftWareUpdate alert_type = %d")
TRACE_MSG(MMIDM_WINTAB_1591_112_2_18_2_14_30_861,"MMIDM==> SoftWareUpdate start")
TRACE_MSG(MMIDM_WINTAB_1638_112_2_18_2_14_30_862,"MMIDM==>  HandleDmInstallQueryWin: msg_id = 0x%x")
TRACE_MSG(MMIDM_WINTAB_1682_112_2_18_2_14_30_863,"MMIDM==>  HandleDmFumoUpdateSuccWin: msg_id = 0x%x")
TRACE_MSG(MMIDM_WINTAB_1731_112_2_18_2_14_30_864,"MMIDM==> HandleWaitWinMsg, win_id =%d, msg_id = %d.")
TRACE_MSG(MMIDM_WINTAB_1801_112_2_18_2_14_30_865,"MMIDM==> MMIDM_ProgressUpdateCallBack inCurrentProgress=%d,inMaxProgress=%d")
TRACE_MSG(MMIDM_WINTAB_1828_112_2_18_2_14_30_866,"MMIDM==> MMIDM_ProgressUpdateCallBack PNULL==data")
TRACE_MSG(MMIDM_WINTAB_1892_112_2_18_2_14_30_867,"MMIDM==> MMIDM_ServerLockCallBack :LOCK")
TRACE_MSG(MMIDM_WINTAB_1937_112_2_18_2_14_30_868,"MMIDM==> MMIDM_ServerUnlockCallBack: UNLOCK")
TRACE_MSG(MMIDM_WINTAB_2012_112_2_18_2_14_31_869,"MMIDM==> HandleDMServerLockWinMsg: 0x%x")
TRACE_MSG(MMIDM_WINTAB_2084_112_2_18_2_14_31_870,"MMIDM==>  HandleDmSessionChange %d")
TRACE_MSG(MMIDM_WINTAB_2120_112_2_18_2_14_31_871,"MMIDM==>  HandleDmSessionChange PNULL==data")
TRACE_MSG(MMIDM_WINTAB_2143_112_2_18_2_14_31_872,"MMIDM==>  HandleDlSessionChange %d")
TRACE_MSG(MMIDM_WINTAB_2165_112_2_18_2_14_31_873,"MMIDM==>  HandleDlSessionChange PNULL==data")
TRACE_MSG(MMIDM_WINTAB_2176_112_2_18_2_14_31_874,"MMIDM==>  MMIDM_SessionChangeCallBack PNULL == data")
TRACE_MSG(MMIDM_WINTAB_2180_112_2_18_2_14_31_875,"MMIDM==>  MMIDM_SessionChangeCallBack inState = %d,inType = %d")
TRACE_MSG(MMIDM_WINTAB_2206_112_2_18_2_14_31_876,"MMIDM==> MMIDM_NotifyAlertCallBack PNULL == data")
TRACE_MSG(MMIDM_WINTAB_2320_112_2_18_2_14_31_877,"MMIDM==>  HandleDmAlertConfirmWin: msg_id = 0x%x")
TRACE_MSG(MMIDM_WINTAB_2381_112_2_18_2_14_31_878,"MMIDM==>  HandleDmAlertDisplayWin msg_id = 0x%x")
TRACE_MSG(MMIDM_WINTAB_2428_112_2_18_2_14_31_879,"MMIDM==>  MMIDM_HandleUserSelectOption alert_mode =%d msg_id=0x%x")
TRACE_MSG(MMIDM_WINTAB_2464_112_2_18_2_14_31_880,"MMIDM==> HandleAlertPlDisplay %d")
TRACE_MSG(MMIDM_WINTAB_2486_112_2_18_2_14_32_881,"MMIDM==> HandleAlertPlDisplay observer_ptr== PNULL")
TRACE_MSG(MMIDM_WINTAB_2498_112_2_18_2_14_32_882,"MMIDM==> HandleAlertPlConfirm %d")
TRACE_MSG(MMIDM_WINTAB_2536_112_2_18_2_14_32_883,"MMIDM==> HandleAlertPlConfirm observer_ptr== PNULL")
TRACE_MSG(MMIDM_WINTAB_2553_112_2_18_2_14_32_884,"MMIDM==> HandleAlertNIAConfirm")
TRACE_MSG(MMIDM_WINTAB_2579_112_2_18_2_14_32_885,"MMIDM==> HandleAlertFUMOConfirm")
TRACE_MSG(MMIDM_WINTAB_2619_112_2_18_2_14_32_886,"MMIDM==> HandleAlertScomoConfirm")
TRACE_MSG(MMIDM_WINTAB_2657_112_2_18_2_14_32_887,"MMIDM==> MMIDM_IsDmTimerId ")
TRACE_MSG(MMIDM_WINTAB_2697_112_2_18_2_14_32_888,"MMIDM==> MMIDM_HandleDmTimer %d")
TRACE_MSG(MMIDM_WINTAB_2700_112_2_18_2_14_32_889,"MMIDM==> MMIDM_HandleDmTimer s_mmidm_fumo_update_timerid in")
TRACE_MSG(MMIDM_WINTAB_2713_112_2_18_2_14_32_890,"MMIDM==> MMIDM_HandleDmTimer s_mmidm_run_timerid in")
TRACE_MSG(MMIDM_WINTAB_2724_112_2_18_2_14_32_891,"MMIDM==> MMIDM_HandleDmTimer s_mmidm_no_signal_timerid in")
TRACE_MSG(MMIDM_WINTAB_2747_112_2_18_2_14_32_892,"MMIDM==> MMIDM_HandleDmTimer s_mmidm_delay_timerid in")
TRACE_MSG(MMIDM_WINTAB_2765_112_2_18_2_14_32_893,"MMIDM==> MMIDM_HandleDmTimer s_mmidm_dis_confim_timerid in")
TRACE_MSG(MMIDM_WINTAB_2788_112_2_18_2_14_32_894,"MMIDM==> MMIDM_HandleDmTimer s_mmidm_fota_report_timerid in")
TRACE_MSG(MMIDM_WINTAB_2798_112_2_18_2_14_32_895,"MMIDM==> MMIDM_HandleDmTimer s_mmidm_choice_check_timerid in")
TRACE_MSG(MMIDM_WINTAB_2817_112_2_18_2_14_32_896,"MMIDM==> MMIDM_HandleDmTimer s_mmidm_socket_timerid in")
TRACE_MSG(MMIDM_WINTAB_2852_112_2_18_2_14_32_897,"MMIDM==>  HandleDmResumeQueryWin: msg_id = 0x%x")
TRACE_MSG(MMIDM_WINTAB_2891_112_2_18_2_14_32_898,"MMIDM==>  HandleReDmResumeQueryWin: msg_id = 0x%x")
TRACE_MSG(MMIDM_WINTAB_2929_112_2_18_2_14_32_899,"MMIDM==> lock session\n")
TRACE_MSG(MMIDM_WINTAB_2953_112_2_18_2_14_32_900,"MMIDM==> update session\n")
TRACE_MSG(MMIDM_WINTAB_2973_112_2_18_2_14_32_901,"MMIDM==> CreateDispalyConfimCheckTimer timer=%d")
TRACE_MSG(MMIDM_WINTAB_2988_112_2_18_2_14_33_902,"MMIDM==> CreateDispalyConfimCheckTimer timer_num=%d s_mmidm_dis_confim_timerid=%d")
TRACE_MSG(MMIDM_WINTAB_3011_112_2_18_2_14_33_903,"MMIDM==> CreateChoiceListCheckTimer timer=%d")
TRACE_MSG(MMIDM_WINTAB_3026_112_2_18_2_14_33_904,"MMIDM==> CreateDispalyConfimCheckTimer timer_num=%d s_mmidm_choice_check_timerid=%d")
TRACE_MSG(MMIDM_WINTAB_3049_112_2_18_2_14_33_905,"MMIDM==> MMIDM_CreateDelayTimer timer=%d")
TRACE_MSG(MMIDM_WINTAB_3220_112_2_18_2_14_33_906,"MMIDM==> OpenDlChoiceListQueryWin mode=%d")
TRACE_MSG(MMIDM_WINTAB_3232_112_2_18_2_14_33_907,"MMIDM==> OpenDlChoiceListQueryWin error mode!")
TRACE_MSG(MMIDM_WINTAB_3252_112_2_18_2_14_33_908,"MMIDM: HandleChiceListQueryWinMsg msg_id = %d")
END_TRACE_MAP(MMI_APP_DM_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_DM_TRC_H_


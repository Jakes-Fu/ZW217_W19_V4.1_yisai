/******************************************************************************
 ** File Name:      mmi_app_dl_trc.h                                         *
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
//trace_id:166
#ifndef _MMI_APP_DL_TRC_H_
#define _MMI_APP_DL_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIDL_617_112_2_18_2_13_19_0 "DL_Star mime_type = %d, is_chunk = %d"
#define MMIDL_618_112_2_18_2_13_19_1 "DL_Star status_code = %d, method = %d, buf_len = %d"
#define MMIDL_619_112_2_18_2_13_19_2 "DL_Star http_header = %x, url = %x, buf = %x"
#define MMIDL_848_112_2_18_2_13_19_3 "\"\"  MMIDL_SetFreq   \"#"
#define MMIDL_865_112_2_18_2_13_19_4 "\"\"  MMIDL_RestoreFreq   \"#"
#define MMIDL_899_112_2_18_2_13_19_5 "Download_CreateNew download_instance Null"
#define MMIDL_903_112_2_18_2_13_19_6 "Download_CreateNew mime_type=%d"
#define MMIDL_927_112_2_18_2_13_19_7 "Download_CreateNew Exceed Max Task!"
#define MMIDL_958_112_2_18_2_13_19_8 "SetDownloadAppletBGState is_bg=%d,is_freq_change=%d"
#define MMIDL_996_112_2_18_2_13_19_9 "MMIDL_StartHttpRequest task_id=%d"
#define MMIDL_1000_112_2_18_2_13_19_10 "MMIDL_StartHttpRequest download_instance Null"
#define MMIDL_1009_112_2_18_2_13_19_11 "MMIDL_StartHttpRequest Network Not Ready"
#define MMIDL_1022_112_2_18_2_13_19_12 "MMIDL_StartHttpRequest multi_task_num=%d,task num=%d"
#define MMIDL_1033_112_2_18_2_13_19_13 "MMIDL_StartHttpRequest No Memory"
#define MMIDL_1045_112_2_18_2_13_19_14 "MMIDL_StartHttpRequest gateway=%s, gateway_len=%d, port=%d, bearer_type=%d"
#define MMIDL_1090_112_2_18_2_13_20_15 "MMIDL_StartHttpRequest http_error=%d"
#define MMIDL_1109_112_2_18_2_13_20_16 "Download_CreateNewByIndex index=%d"
#define MMIDL_1113_112_2_18_2_13_20_17 "Download_CreateNewByIndex download_instance Null"
#define MMIDL_1137_112_2_18_2_13_20_18 "Download_Initialize"
#define MMIDL_1190_112_2_18_2_13_20_19 "MMIDL_GetDownloadInstance download applet not existed"
#define MMIDL_1211_112_2_18_2_13_20_20 "DownloadApplet_HandleEvent msg_id = 0x%04x"
#define MMIDL_1234_112_2_18_2_13_20_21 "DownloadApplet_HandleEvent type = %d"
#define MMIDL_1434_112_2_18_2_13_20_22 "DestoryDownloadTask task_id=%d"
#define MMIDL_1493_112_2_18_2_13_21_23 "HandleODM_Start_Download_Cnf Exceed Max Task!"
#define MMIDL_1510_112_2_18_2_13_21_24 "HandleODM_Start_Download_Cnf magic = %d session_id = %d ret = %d"
#define MMIDL_1551_112_2_18_2_13_21_25 "HandleODM_Download_Confirmation_Ind session id = %d"
#define MMIDL_1574_112_2_18_2_13_21_26 "HandleODM_Download_Confirmation_Ind Uri is incorrect"
#define MMIDL_1578_112_2_18_2_13_21_27 "HandleODM_Download_Confirmation_Ind task id = %d"
#define MMIDL_1584_112_2_18_2_13_21_28 "HandleODM_Download_Confirmation_Ind type = %s"
#define MMIDL_1585_112_2_18_2_13_21_29 "HandleODM_Download_Confirmation_Ind mime_type id = %d"
#define MMIDL_1633_112_2_18_2_13_21_30 "HandleODM_Download_Progress_Ind param =%d, session_id = %d, stage = %d"
#define MMIDL_1732_112_2_18_2_13_21_31 "HandleODM_Download_Error_Ind err_id = %d"
#define MMIDL_1742_112_2_18_2_13_21_32 "HandleODM_Download_Error_Ind manager_index=%d"
#define MMIDL_1748_112_2_18_2_13_21_33 "HandleODM_Download_Error_Ind task_info.url=%s"
#define MMIDL_1764_112_2_18_2_13_21_34 "HandleODM_Download_Error_Ind close MMIDL_DOWNLOAD_ALERT_WIN_ID"
#define MMIDL_1775_112_2_18_2_13_21_35 "MMISMS: PlayNewSMSRing call MMIAPICC_IsInCallConnected"
#define MMIDL_1958_112_2_18_2_13_22_36 "HandleODMMsg receive ODM_START_DOWNLOAD_CNF"
#define MMIDL_1963_112_2_18_2_13_22_37 "HandleODMMsg receive ODM_CANCEL_DOWNLOAD_CNF"
#define MMIDL_1968_112_2_18_2_13_22_38 "HandleODMMsg receive ODM_DOWNLOAD_CONFIRMATION_IND"
#define MMIDL_1973_112_2_18_2_13_22_39 "HandleODMMsg receive ODM_DOWNLOAD_AUTH_IND"
#define MMIDL_1978_112_2_18_2_13_22_40 "HandleODMMsg receive ODM_DOWNLOAD_PROGRESS_IND"
#define MMIDL_1983_112_2_18_2_13_22_41 "HandleODMMsg receive ODM_DOWNLOAD_SUCCESS_IND"
#define MMIDL_1988_112_2_18_2_13_22_42 "HandleODMMsg receive ODM_DOWNLOAD_ERROR_IND"
#define MMIDL_1993_112_2_18_2_13_22_43 "HandleODMMsg receive ODM_DOWNLOAD_GET_SESSION_LIST_CNF"
#define MMIDL_1998_112_2_18_2_13_22_44 "HandleODMMsg receive ODM_DOWNLOAD_GET_SESSION_INFO_CNF"
#define MMIDL_2003_112_2_18_2_13_22_45 "HandleODMMsg receive ODM_START_DOWNLOAD_CNF"
#define MMIDL_2008_112_2_18_2_13_22_46 "HandleODMMsg receive ODM_DESUBSCRIBE_INDICATION_CNF"
#define MMIDL_2013_112_2_18_2_13_22_47 "HandleODMMsg receive ODM_MANAGER_PROGRESS_IND"
#define MMIDL_2018_112_2_18_2_13_22_48 "HandleODMMsg receive ODM_MANAGER_SUCCESS_IND"
#define MMIDL_2023_112_2_18_2_13_22_49 "HandleODMMsg receive ODM_MANAGER_ERROR_IND"
#define MMIDL_2028_112_2_18_2_13_22_50 "HandleODMMsg Not handle msg_id = %x"
#define MMIDL_2165_112_2_18_2_13_22_51 "HandleHttpInitCnf not download task"
#define MMIDL_2174_112_2_18_2_13_22_52 "HandleHttpInitCnf not this download instance"
#define MMIDL_2186_112_2_18_2_13_22_53 "HandleHttpInitCnf no memory"
#define MMIDL_2422_112_2_18_2_13_23_54 "HandleHttpGetCnf mime_type is %d"
#define MMIDL_2552_112_2_18_2_13_23_55 "HandleHttpDataInd buf_len=%d, data_len=%d, cur_len=%d"
#define MMIDL_2622_112_2_18_2_13_23_56 "HandleHttpDataInd is_downloading=%d"
#define MMIDL_2632_112_2_18_2_13_23_57 "HandleHttpDataInd data_len=%d, file_handle=%d"
#define MMIDL_2669_112_2_18_2_13_23_58 "HandleHttpDataInd is_downloading=%d"
#define MMIDL_2722_112_2_18_2_13_24_59 "HandleHttpHeaderInd response_code=%d, content_len=%d"
#define MMIDL_2756_112_2_18_2_13_24_60 "HandleHttpHeaderInd space_high=%d, space_low=%d"
#define MMIDL_2771_112_2_18_2_13_24_61 "HandleHttpHeaderInd is_valid=%d"
#define MMIDL_3136_112_2_18_2_13_24_62 "MMIDL_AuthResponse download_instance Null"
#define MMIDL_3200_112_2_18_2_13_25_63 "HandleHttpMsg receive HTTP_SIG_INIT_CNF"
#define MMIDL_3205_112_2_18_2_13_25_64 "HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND"
#define MMIDL_3210_112_2_18_2_13_25_65 "HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND"
#define MMIDL_3215_112_2_18_2_13_25_66 "HandleHttpMsg receive HTTP_SIG_GET_CNF"
#define MMIDL_3220_112_2_18_2_13_25_67 "HandleHttpMsg receive HTTP_SIG_HEAD_CNF"
#define MMIDL_3225_112_2_18_2_13_25_68 "HandleHttpMsg receive HTTP_SIG_DATA_IND"
#define MMIDL_3230_112_2_18_2_13_25_69 "HandleHttpMsg receive HTTP_SIG_HEADER_IND"
#define MMIDL_3235_112_2_18_2_13_25_70 "HandleHttpMsg receive HTTP_SIG_ERROR_IND"
#define MMIDL_3240_112_2_18_2_13_25_71 "HandleHttpMsg receive HTTP_SIG_CLOSE_CNF"
#define MMIDL_3245_112_2_18_2_13_25_72 "HandleHttpMsg receive HTTP_SIG_CANCEL_CNF"
#define MMIDL_3250_112_2_18_2_13_25_73 "HandleHttpMsg receive HTTP_SIG_NET_PROG_IND"
#define MMIDL_3255_112_2_18_2_13_25_74 "HandleHttpMsg receive HTTP_SIG_REDIRECT_IND"
#define MMIDL_3260_112_2_18_2_13_25_75 "HandleHttpMsg receive HTTP_SIG_POST_CNF"
#define MMIDL_3266_112_2_18_2_13_25_76 "HandleHttpMsg receive HTTP_SIG_SSL_CERT_UNTRUST_IND"
#define MMIDL_3272_112_2_18_2_13_25_77 "HandleHttpMsg Not handle msg_id = %x"
#define MMIDL_3291_112_2_18_2_13_25_78 "MMIDL_DeleteDownloadTask download_instance Null"
#define MMIDL_3320_112_2_18_2_13_25_79 "MMIDL_SetDownloadManagerTaskIndex download_instance Null"
#define MMIDL_3342_112_2_18_2_13_25_80 "MMIDL_GetDownloadManagerTaskIndex download_instance Null"
#define MMIDL_3441_112_2_18_2_13_25_81 "MMIDL_StopDownload task_id=%d, is_all=%d"
#define MMIDL_3447_112_2_18_2_13_25_82 "MMIDL_StopDownload No Download Task"
#define MMIDL_3531_112_2_18_2_13_25_83 "MMIDL_StopDownloadApplet "
#define MMIDL_3536_112_2_18_2_13_25_84 "MMIDL_StopDownloadApplet No Download Task"
#define MMIDL_3587_112_2_18_2_13_25_85 "MMIDL_SetPDPState:old %d-new %d"
#define MMIDL_3602_112_2_18_2_13_25_86 "MMIDL_NetConnectEx bearer_type=%d"
#define MMIDL_3616_112_2_18_2_13_25_87 "MMIDL_NetConnectEx:MMIAPIPHONE_IsSimOk not OK"
#define MMIDL_3652_112_2_18_2_13_25_88 "MMIDL_NetConnectEx:MMIAPIPDP_Active fail"
#define MMIDL_3671_112_2_18_2_13_26_89 "MMIDL_NetConnect pdp_state=%d"
#define MMIDL_3696_112_2_18_2_13_26_90 "MMIDL_NetDisconnect"
#define MMIDL_3697_112_2_18_2_13_26_91 "MMIDL_Network  = %d"
#define MMIDL_3709_112_2_18_2_13_26_92 "MMIDL_NetDisconnect..."
#define MMIDL_3732_112_2_18_2_13_26_93 "HandleDLPsMsg() enter msg_id= %d"
#define MMIDL_3738_112_2_18_2_13_26_94 "HandleDLPsMsg: MMIPDP_ACTIVE_CNF"
#define MMIDL_3746_112_2_18_2_13_26_95 "HandleDLPsMsg: MMIPDP_DEACTIVE_CNF"
#define MMIDL_3754_112_2_18_2_13_26_96 "HandleDLPsMsg: MMIPDP_DEACTIVE_IND"
#define MMIDL_3762_112_2_18_2_13_26_97 "HandleDLPsMsg: MMIPDP_DEACTIVE_IND"
#define MMIDL_3786_112_2_18_2_13_26_98 "HandleDLPDPActiveCnf() msg_ptr->result=%d"
#define MMIDL_3787_112_2_18_2_13_26_99 "HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d"
#define MMIDL_3818_112_2_18_2_13_26_100 "HandleDLPDPDeactiveCnf() msg_ptr->result=%d"
#define MMIDL_3819_112_2_18_2_13_26_101 "HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d"
#define MMIDL_3847_112_2_18_2_13_26_102 "HandleDLPDPDeactiveInd() msg_ptr->result=%d"
#define MMIDL_3848_112_2_18_2_13_26_103 "HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d"
#define MMIDL_3880_112_2_18_2_13_26_104 "HandleDLPDPDeactiveInd() msg_ptr->result=%d"
#define MMIDL_3881_112_2_18_2_13_26_105 "HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d"
#define MMIDL_3902_112_2_18_2_13_26_106 "MMIDL_NetworkIsReady=%d"
#define MMIDL_3924_112_2_18_2_13_26_107 "DownloadHandleHttpWatingTaskId download_instance Null"
#define MMIDL_3928_112_2_18_2_13_26_108 "DownloadHandleHttpWatingTaskId task_id=%d, is_increase=%d, num=%d"
#define MMIDL_3977_112_2_18_2_13_26_109 "HandlePDPActiveSuccess download_instance Null"
#define MMIDL_3987_112_2_18_2_13_26_110 "HandlePDPActiveSuccess http_waiting_task_id_num=%d"
#define MMIDL_4016_112_2_18_2_13_26_111 "HandlePDPActiveFail download_instance Null"
#define MMIDL_4022_112_2_18_2_13_26_112 "HandlePDPActiveFail network not ready!"
#define MMIDL_4154_112_2_18_2_13_27_113 "MMIAPIDL_GetAudioTypeByNFMimeType mime_type=%d, dl_mime_type=%d"
#define MMIDL_4219_112_2_18_2_13_27_114 "MMIAPIDL_DeleteCompletedTask dl_instance exist status = %d"
#define MMIDL_4235_112_2_18_2_13_27_115 "MMIAPIDL_DeleteCompletedTask dl_instance Not exist status = %d"
#define MMIDL_4296_112_2_18_2_13_27_116 "MMIDL_HandlePDP_Change download_instance Null"
#define MMIDL_4348_112_2_18_2_13_27_117 "MMIAPIDL_HandleServiceChange bearer_type = %d"
#define MMIDL_4357_112_2_18_2_13_27_118 "MMIAPIDL_HandleServiceChange pdp_info.bearer_type = %d"
#define MMIDL_FILE_349_112_2_18_2_13_28_119 "MMIDL_CombineDownloadFilename_ptr->wstr_len = %d,  path_ptr->wstr_len = %d,is_combine_tmp_file=%d"
#define MMIDL_FILE_576_112_2_18_2_13_29_120 "GetDownloadFileByURL i=%d, rtn_url_ext=%s"
#define MMIDL_FILE_706_112_2_18_2_13_29_121 "GetMimeTypeByhttpHeader enter"
#define MMIDL_FILE_713_112_2_18_2_13_29_122 "GetMimeTypeByhttpHeader dest_str1=0x%x"
#define MMIDL_FILE_716_112_2_18_2_13_29_123 "GetMimeTypeByhttpHeader dest_str2=0x%x"
#define MMIDL_FILE_723_112_2_18_2_13_29_124 "GetMimeTypeByhttpHeader dest_str3=0x%x"
#define MMIDL_FILE_732_112_2_18_2_13_29_125 "GetMimeTypeByhttpHeader dest_len=%d"
#define MMIDL_FILE_738_112_2_18_2_13_29_126 "GetMimeTypeByhttpHeader dest_str=%s"
#define MMIDL_FILE_827_112_2_18_2_13_29_127 "MMIDL_GetDefaultFileName type = %d,dl_charset=%d"
#define MMIDL_FILE_863_112_2_18_2_13_29_128 "MMIDL_GetDownloadType mime_type=%d, is_nf_type=%d"
#define MMIDL_FILE_1042_112_2_18_2_13_30_129 "MMIDL_GetCustomizedDownloadPath len=%d"
#define MMIDL_FILE_1167_112_2_18_2_13_30_130 "MMIDL_CreateDownloadFile file not exist!"
#define MMIDL_FILE_1405_112_2_18_2_13_31_131 "GetCharsetByhttpHeader enter"
#define MMIDL_FILE_1422_112_2_18_2_13_31_132 "GetCharsetByhttpHeader content_type_len=%d"
#define MMIDL_FILE_1433_112_2_18_2_13_31_133 "GetCharsetByhttpHeader content_type_ptr=%s"
#define MMIDL_FILE_1444_112_2_18_2_13_31_134 "GetCharsetByhttpHeader dest_str1=%s"
#define MMIDL_FUNC_266_112_2_18_2_13_32_135 "MMIDL_DeleteDownloadManagerTask index=%d, is_all=%d"
#define MMIDL_FUNC_436_112_2_18_2_13_32_136 "MMIDL_ChangeDownloadTaskStatus index = %d, status = %d"
#define MMIDL_FUNC_567_112_2_18_2_13_32_137 "MMIDL_ChangeDownloadTaskFileName index = %d, error"
#define MMIDL_FUNC_586_112_2_18_2_13_32_138 "MMIDL_ChangeDownloadTaskTotalLen index = %d, total_len = %d"
#define MMIDL_FUNC_608_112_2_18_2_13_32_139 "MMIDL_ChangeDownloadTaskCurLen index = %d, cur_len = %d"
#define MMIDL_FUNC_627_112_2_18_2_13_32_140 "MMIDL_GetDownloadManagerTotalTaskNum total_task_num=%d"
#define MMIDL_FUNC_649_112_2_18_2_13_32_141 "MMIDL_GetUnUnusedManagerTaskId unused_id=%d"
#define MMIDL_FUNC_693_112_2_18_2_13_32_142 "MMIDL_IsSameDownloadDestUrl is_same_url=%d"
#define MMIDL_FUNC_735_112_2_18_2_13_33_143 "MMIDL_AppendDLTaskListItem invalid status = %d"
#define MMIDL_FUNC_921_112_2_18_2_13_33_144 "GetDownloadDisplayFilename full_path_len = %d"
#define MMIDL_FUNC_1197_112_2_18_2_13_33_145 "invalid device"
#define MMIDL_FUNC_1299_112_2_18_2_13_34_146 "MMIDL_SetPDPInfo download_instance Null"
#define MMIDL_FUNC_1319_112_2_18_2_13_34_147 "MMIDL_GetPDPInfo download_instance Null"
#define MMIDL_FUNC_1417_112_2_18_2_13_34_148 "MMIDL_NotifyDownloadComplete index=%d,is_success=%d,sound_notify= %d"
#define MMIDL_FUNC_1476_112_2_18_2_13_34_149 "MMIDL_NotifyDownloadComplete mime_type is %d"
#define MMIDL_FUNC_1491_112_2_18_2_13_34_150 "MMIDL_SetMultiTaskNum download_instance Null"
#define MMIDL_FUNC_1517_112_2_18_2_13_34_151 "MMIDL_SetMultiTaskNum download_instance Null"
#define MMIDL_FUNC_1534_112_2_18_2_13_34_152 "MMIDL_GetDownloadManager download_instance Null"
#define MMIDL_FUNC_1551_112_2_18_2_13_34_153 "MMIDL_SetDownloadManager download_instance Null"
#define MMIDL_FUNC_1587_112_2_18_2_13_34_154 "MMIDL_StartOMADownload enter"
#define MMIDL_FUNC_1595_112_2_18_2_13_34_155 "MMIDL_StartOMADownload dl_manager->task_info[%d].url=%s"
#define MMIDL_FUNC_1600_112_2_18_2_13_34_156 "MMIDL_StartOMADownload File_handle is NULL"
#define MMIDL_FUNC_1607_112_2_18_2_13_34_157 "MMIDL_StartOMADownload File size Incorrect"
#define MMIDL_FUNC_1614_112_2_18_2_13_34_158 "MMIDL_StartOMADownload No mem"
#define MMIDL_FUNC_1621_112_2_18_2_13_34_159 "MMIDL_StartOMADownload Read File error"
#define MMIDL_FUNC_1660_112_2_18_2_13_34_160 "MMIDL_StartOMADownload ODM_StartDownload Failed oma_ret = %d"
#define MMIDL_WINTAB_679_112_2_18_2_13_38_161 "mmidl_wintab.c:MMIDL_OpenDownloadSaveAsWin() save_as_info is NULL"
#define MMIDL_WINTAB_692_112_2_18_2_13_38_162 "mmidl_wintab.c:MMIDL_OpenDownloadSaveAsWin() save_as_info->buf is NULL"
#define MMIDL_WINTAB_707_112_2_18_2_13_38_163 "mmidl_wintab.c:MMIDL_OpenDownloadSaveAsWin() save_as_info->url is NULL"
#define MMIDL_WINTAB_830_112_2_18_2_13_38_164 "mmidl_wintab.c:MMIDL_OpenOMADownloadQueryWin No MEM"
#define MMIDL_WINTAB_908_112_2_18_2_13_38_165 "mmidl_wintab.c:HandleDownloadAuthWinMsg() msg_id=0x%04x"
#define MMIDL_WINTAB_1084_112_2_18_2_13_38_166 "mmidl_wintab.c:HandleDownloadAuthWinMsg() msg_id=%d"
#define MMIDL_WINTAB_1111_112_2_18_2_13_38_167 "mmidl_wintab.c:HandleDownloadAuthInputWinMsg() msg_id=0x%04x"
#define MMIDL_WINTAB_1191_112_2_18_2_13_39_168 "Download_OpenFileCallBack download_instance Null"
#define MMIDL_WINTAB_1202_112_2_18_2_13_39_169 "Download_OpenFileCallBack is_success = %d"
#define MMIDL_WINTAB_1208_112_2_18_2_13_39_170 "Download_OpenFileCallBack, file_num = %d"
#define MMIDL_WINTAB_1213_112_2_18_2_13_39_171 "Download_OpenFileCallBack pathlen = %d"
#define MMIDL_WINTAB_1248_112_2_18_2_13_39_172 "Download_OpenLabelCallBack is_success = %d"
#define MMIDL_WINTAB_1254_112_2_18_2_13_39_173 "Download_OpenLabelCallBack, file_num = %d"
#define MMIDL_WINTAB_1423_112_2_18_2_13_39_174 "HandleDownloadSaveWinMsg download_instance Null"
#define MMIDL_WINTAB_1432_112_2_18_2_13_39_175 "HandleDownloadSaveWinMsg mime_type is %d"
#define MMIDL_WINTAB_1433_112_2_18_2_13_39_176 "mmidl_wintab.c:HandleDownloadSaveWinMsg() msg_id=0x%04x"
#define MMIDL_WINTAB_1493_112_2_18_2_13_39_177 "HandleDownloadSaveWinMsg mime_type is %d"
#define MMIDL_WINTAB_1590_112_2_18_2_13_39_178 "HandleDownloadSaveWinMsg task_index=%d"
#define MMIDL_WINTAB_1683_112_2_18_2_13_40_179 "mmidl_wintab.c:HandleDownloadSaveWinMsg() msg_id=%d"
#define MMIDL_WINTAB_1730_112_2_18_2_13_40_180 "mmidl_wintab.c:HandleDownloadSaveAsWinMsg() msg_id=0x%04x"
#define MMIDL_WINTAB_1942_112_2_18_2_13_40_181 "mmidl_wintab.c:HandleInputDownloadFileName() msg_id=0x%04x"
#define MMIDL_WINTAB_2037_112_2_18_2_13_40_182 "mmidl_wintab.c:HandleDownloadManagerWinMsg() msg_id=0x%04x"
#define MMIDL_WINTAB_2138_112_2_18_2_13_40_183 "mmidl_wintab.c:HandleDownloadManagerOptWinMsg() msg_id=0x%04x"
#define MMIDL_WINTAB_2193_112_2_18_2_13_41_184 "HandleDownloadManagerOptWinMsg() not handled menu"
#define MMIDL_WINTAB_2277_112_2_18_2_13_41_185 "SetDownloadManagerOptionGray index = %d, total_task_num = %d"
#define MMIDL_WINTAB_2280_112_2_18_2_13_41_186 "SetDownloadManagerOptionGray status= %d"
#define MMIDL_WINTAB_2358_112_2_18_2_13_41_187 "HandleDownloadTaskDetailWinMsg() msg_id=0x%04x"
#define MMIDL_WINTAB_2385_112_2_18_2_13_41_188 "mmidl_wintab.c:HandleDownloadQueryWinMsg() msg_id=0x%04x"
#define MMIDL_WINTAB_2498_112_2_18_2_13_41_189 "mmidl_wintab.c:HandleDownloadQueryWinMsg() msg_id=0x%04x"
#define MMIDL_WINTAB_2527_112_2_18_2_13_41_190 "HandleDownloadQueryWinMsg() msg_id=0x%04x"
#define MMIDL_WINTAB_2547_112_2_18_2_13_41_191 "mmidl_wintab.c:HandleDownloadQueryWinMsg() msg_id=0x%04x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_DL_TRC)
TRACE_MSG(MMIDL_617_112_2_18_2_13_19_0,"DL_Star mime_type = %d, is_chunk = %d")
TRACE_MSG(MMIDL_618_112_2_18_2_13_19_1,"DL_Star status_code = %d, method = %d, buf_len = %d")
TRACE_MSG(MMIDL_619_112_2_18_2_13_19_2,"DL_Star http_header = %x, url = %x, buf = %x")
TRACE_MSG(MMIDL_848_112_2_18_2_13_19_3,"\"\"  MMIDL_SetFreq   \"#")
TRACE_MSG(MMIDL_865_112_2_18_2_13_19_4,"\"\"  MMIDL_RestoreFreq   \"#")
TRACE_MSG(MMIDL_899_112_2_18_2_13_19_5,"Download_CreateNew download_instance Null")
TRACE_MSG(MMIDL_903_112_2_18_2_13_19_6,"Download_CreateNew mime_type=%d")
TRACE_MSG(MMIDL_927_112_2_18_2_13_19_7,"Download_CreateNew Exceed Max Task!")
TRACE_MSG(MMIDL_958_112_2_18_2_13_19_8,"SetDownloadAppletBGState is_bg=%d,is_freq_change=%d")
TRACE_MSG(MMIDL_996_112_2_18_2_13_19_9,"MMIDL_StartHttpRequest task_id=%d")
TRACE_MSG(MMIDL_1000_112_2_18_2_13_19_10,"MMIDL_StartHttpRequest download_instance Null")
TRACE_MSG(MMIDL_1009_112_2_18_2_13_19_11,"MMIDL_StartHttpRequest Network Not Ready")
TRACE_MSG(MMIDL_1022_112_2_18_2_13_19_12,"MMIDL_StartHttpRequest multi_task_num=%d,task num=%d")
TRACE_MSG(MMIDL_1033_112_2_18_2_13_19_13,"MMIDL_StartHttpRequest No Memory")
TRACE_MSG(MMIDL_1045_112_2_18_2_13_19_14,"MMIDL_StartHttpRequest gateway=%s, gateway_len=%d, port=%d, bearer_type=%d")
TRACE_MSG(MMIDL_1090_112_2_18_2_13_20_15,"MMIDL_StartHttpRequest http_error=%d")
TRACE_MSG(MMIDL_1109_112_2_18_2_13_20_16,"Download_CreateNewByIndex index=%d")
TRACE_MSG(MMIDL_1113_112_2_18_2_13_20_17,"Download_CreateNewByIndex download_instance Null")
TRACE_MSG(MMIDL_1137_112_2_18_2_13_20_18,"Download_Initialize")
TRACE_MSG(MMIDL_1190_112_2_18_2_13_20_19,"MMIDL_GetDownloadInstance download applet not existed")
TRACE_MSG(MMIDL_1211_112_2_18_2_13_20_20,"DownloadApplet_HandleEvent msg_id = 0x%04x")
TRACE_MSG(MMIDL_1234_112_2_18_2_13_20_21,"DownloadApplet_HandleEvent type = %d")
TRACE_MSG(MMIDL_1434_112_2_18_2_13_20_22,"DestoryDownloadTask task_id=%d")
TRACE_MSG(MMIDL_1493_112_2_18_2_13_21_23,"HandleODM_Start_Download_Cnf Exceed Max Task!")
TRACE_MSG(MMIDL_1510_112_2_18_2_13_21_24,"HandleODM_Start_Download_Cnf magic = %d session_id = %d ret = %d")
TRACE_MSG(MMIDL_1551_112_2_18_2_13_21_25,"HandleODM_Download_Confirmation_Ind session id = %d")
TRACE_MSG(MMIDL_1574_112_2_18_2_13_21_26,"HandleODM_Download_Confirmation_Ind Uri is incorrect")
TRACE_MSG(MMIDL_1578_112_2_18_2_13_21_27,"HandleODM_Download_Confirmation_Ind task id = %d")
TRACE_MSG(MMIDL_1584_112_2_18_2_13_21_28,"HandleODM_Download_Confirmation_Ind type = %s")
TRACE_MSG(MMIDL_1585_112_2_18_2_13_21_29,"HandleODM_Download_Confirmation_Ind mime_type id = %d")
TRACE_MSG(MMIDL_1633_112_2_18_2_13_21_30,"HandleODM_Download_Progress_Ind param =%d, session_id = %d, stage = %d")
TRACE_MSG(MMIDL_1732_112_2_18_2_13_21_31,"HandleODM_Download_Error_Ind err_id = %d")
TRACE_MSG(MMIDL_1742_112_2_18_2_13_21_32,"HandleODM_Download_Error_Ind manager_index=%d")
TRACE_MSG(MMIDL_1748_112_2_18_2_13_21_33,"HandleODM_Download_Error_Ind task_info.url=%s")
TRACE_MSG(MMIDL_1764_112_2_18_2_13_21_34,"HandleODM_Download_Error_Ind close MMIDL_DOWNLOAD_ALERT_WIN_ID")
TRACE_MSG(MMIDL_1775_112_2_18_2_13_21_35,"MMISMS: PlayNewSMSRing call MMIAPICC_IsInCallConnected")
TRACE_MSG(MMIDL_1958_112_2_18_2_13_22_36,"HandleODMMsg receive ODM_START_DOWNLOAD_CNF")
TRACE_MSG(MMIDL_1963_112_2_18_2_13_22_37,"HandleODMMsg receive ODM_CANCEL_DOWNLOAD_CNF")
TRACE_MSG(MMIDL_1968_112_2_18_2_13_22_38,"HandleODMMsg receive ODM_DOWNLOAD_CONFIRMATION_IND")
TRACE_MSG(MMIDL_1973_112_2_18_2_13_22_39,"HandleODMMsg receive ODM_DOWNLOAD_AUTH_IND")
TRACE_MSG(MMIDL_1978_112_2_18_2_13_22_40,"HandleODMMsg receive ODM_DOWNLOAD_PROGRESS_IND")
TRACE_MSG(MMIDL_1983_112_2_18_2_13_22_41,"HandleODMMsg receive ODM_DOWNLOAD_SUCCESS_IND")
TRACE_MSG(MMIDL_1988_112_2_18_2_13_22_42,"HandleODMMsg receive ODM_DOWNLOAD_ERROR_IND")
TRACE_MSG(MMIDL_1993_112_2_18_2_13_22_43,"HandleODMMsg receive ODM_DOWNLOAD_GET_SESSION_LIST_CNF")
TRACE_MSG(MMIDL_1998_112_2_18_2_13_22_44,"HandleODMMsg receive ODM_DOWNLOAD_GET_SESSION_INFO_CNF")
TRACE_MSG(MMIDL_2003_112_2_18_2_13_22_45,"HandleODMMsg receive ODM_START_DOWNLOAD_CNF")
TRACE_MSG(MMIDL_2008_112_2_18_2_13_22_46,"HandleODMMsg receive ODM_DESUBSCRIBE_INDICATION_CNF")
TRACE_MSG(MMIDL_2013_112_2_18_2_13_22_47,"HandleODMMsg receive ODM_MANAGER_PROGRESS_IND")
TRACE_MSG(MMIDL_2018_112_2_18_2_13_22_48,"HandleODMMsg receive ODM_MANAGER_SUCCESS_IND")
TRACE_MSG(MMIDL_2023_112_2_18_2_13_22_49,"HandleODMMsg receive ODM_MANAGER_ERROR_IND")
TRACE_MSG(MMIDL_2028_112_2_18_2_13_22_50,"HandleODMMsg Not handle msg_id = %x")
TRACE_MSG(MMIDL_2165_112_2_18_2_13_22_51,"HandleHttpInitCnf not download task")
TRACE_MSG(MMIDL_2174_112_2_18_2_13_22_52,"HandleHttpInitCnf not this download instance")
TRACE_MSG(MMIDL_2186_112_2_18_2_13_22_53,"HandleHttpInitCnf no memory")
TRACE_MSG(MMIDL_2422_112_2_18_2_13_23_54,"HandleHttpGetCnf mime_type is %d")
TRACE_MSG(MMIDL_2552_112_2_18_2_13_23_55,"HandleHttpDataInd buf_len=%d, data_len=%d, cur_len=%d")
TRACE_MSG(MMIDL_2622_112_2_18_2_13_23_56,"HandleHttpDataInd is_downloading=%d")
TRACE_MSG(MMIDL_2632_112_2_18_2_13_23_57,"HandleHttpDataInd data_len=%d, file_handle=%d")
TRACE_MSG(MMIDL_2669_112_2_18_2_13_23_58,"HandleHttpDataInd is_downloading=%d")
TRACE_MSG(MMIDL_2722_112_2_18_2_13_24_59,"HandleHttpHeaderInd response_code=%d, content_len=%d")
TRACE_MSG(MMIDL_2756_112_2_18_2_13_24_60,"HandleHttpHeaderInd space_high=%d, space_low=%d")
TRACE_MSG(MMIDL_2771_112_2_18_2_13_24_61,"HandleHttpHeaderInd is_valid=%d")
TRACE_MSG(MMIDL_3136_112_2_18_2_13_24_62,"MMIDL_AuthResponse download_instance Null")
TRACE_MSG(MMIDL_3200_112_2_18_2_13_25_63,"HandleHttpMsg receive HTTP_SIG_INIT_CNF")
TRACE_MSG(MMIDL_3205_112_2_18_2_13_25_64,"HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND")
TRACE_MSG(MMIDL_3210_112_2_18_2_13_25_65,"HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND")
TRACE_MSG(MMIDL_3215_112_2_18_2_13_25_66,"HandleHttpMsg receive HTTP_SIG_GET_CNF")
TRACE_MSG(MMIDL_3220_112_2_18_2_13_25_67,"HandleHttpMsg receive HTTP_SIG_HEAD_CNF")
TRACE_MSG(MMIDL_3225_112_2_18_2_13_25_68,"HandleHttpMsg receive HTTP_SIG_DATA_IND")
TRACE_MSG(MMIDL_3230_112_2_18_2_13_25_69,"HandleHttpMsg receive HTTP_SIG_HEADER_IND")
TRACE_MSG(MMIDL_3235_112_2_18_2_13_25_70,"HandleHttpMsg receive HTTP_SIG_ERROR_IND")
TRACE_MSG(MMIDL_3240_112_2_18_2_13_25_71,"HandleHttpMsg receive HTTP_SIG_CLOSE_CNF")
TRACE_MSG(MMIDL_3245_112_2_18_2_13_25_72,"HandleHttpMsg receive HTTP_SIG_CANCEL_CNF")
TRACE_MSG(MMIDL_3250_112_2_18_2_13_25_73,"HandleHttpMsg receive HTTP_SIG_NET_PROG_IND")
TRACE_MSG(MMIDL_3255_112_2_18_2_13_25_74,"HandleHttpMsg receive HTTP_SIG_REDIRECT_IND")
TRACE_MSG(MMIDL_3260_112_2_18_2_13_25_75,"HandleHttpMsg receive HTTP_SIG_POST_CNF")
TRACE_MSG(MMIDL_3266_112_2_18_2_13_25_76,"HandleHttpMsg receive HTTP_SIG_SSL_CERT_UNTRUST_IND")
TRACE_MSG(MMIDL_3272_112_2_18_2_13_25_77,"HandleHttpMsg Not handle msg_id = %x")
TRACE_MSG(MMIDL_3291_112_2_18_2_13_25_78,"MMIDL_DeleteDownloadTask download_instance Null")
TRACE_MSG(MMIDL_3320_112_2_18_2_13_25_79,"MMIDL_SetDownloadManagerTaskIndex download_instance Null")
TRACE_MSG(MMIDL_3342_112_2_18_2_13_25_80,"MMIDL_GetDownloadManagerTaskIndex download_instance Null")
TRACE_MSG(MMIDL_3441_112_2_18_2_13_25_81,"MMIDL_StopDownload task_id=%d, is_all=%d")
TRACE_MSG(MMIDL_3447_112_2_18_2_13_25_82,"MMIDL_StopDownload No Download Task")
TRACE_MSG(MMIDL_3531_112_2_18_2_13_25_83,"MMIDL_StopDownloadApplet ")
TRACE_MSG(MMIDL_3536_112_2_18_2_13_25_84,"MMIDL_StopDownloadApplet No Download Task")
TRACE_MSG(MMIDL_3587_112_2_18_2_13_25_85,"MMIDL_SetPDPState:old %d-new %d")
TRACE_MSG(MMIDL_3602_112_2_18_2_13_25_86,"MMIDL_NetConnectEx bearer_type=%d")
TRACE_MSG(MMIDL_3616_112_2_18_2_13_25_87,"MMIDL_NetConnectEx:MMIAPIPHONE_IsSimOk not OK")
TRACE_MSG(MMIDL_3652_112_2_18_2_13_25_88,"MMIDL_NetConnectEx:MMIAPIPDP_Active fail")
TRACE_MSG(MMIDL_3671_112_2_18_2_13_26_89,"MMIDL_NetConnect pdp_state=%d")
TRACE_MSG(MMIDL_3696_112_2_18_2_13_26_90,"MMIDL_NetDisconnect")
TRACE_MSG(MMIDL_3697_112_2_18_2_13_26_91,"MMIDL_Network  = %d")
TRACE_MSG(MMIDL_3709_112_2_18_2_13_26_92,"MMIDL_NetDisconnect...")
TRACE_MSG(MMIDL_3732_112_2_18_2_13_26_93,"HandleDLPsMsg() enter msg_id= %d")
TRACE_MSG(MMIDL_3738_112_2_18_2_13_26_94,"HandleDLPsMsg: MMIPDP_ACTIVE_CNF")
TRACE_MSG(MMIDL_3746_112_2_18_2_13_26_95,"HandleDLPsMsg: MMIPDP_DEACTIVE_CNF")
TRACE_MSG(MMIDL_3754_112_2_18_2_13_26_96,"HandleDLPsMsg: MMIPDP_DEACTIVE_IND")
TRACE_MSG(MMIDL_3762_112_2_18_2_13_26_97,"HandleDLPsMsg: MMIPDP_DEACTIVE_IND")
TRACE_MSG(MMIDL_3786_112_2_18_2_13_26_98,"HandleDLPDPActiveCnf() msg_ptr->result=%d")
TRACE_MSG(MMIDL_3787_112_2_18_2_13_26_99,"HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d")
TRACE_MSG(MMIDL_3818_112_2_18_2_13_26_100,"HandleDLPDPDeactiveCnf() msg_ptr->result=%d")
TRACE_MSG(MMIDL_3819_112_2_18_2_13_26_101,"HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d")
TRACE_MSG(MMIDL_3847_112_2_18_2_13_26_102,"HandleDLPDPDeactiveInd() msg_ptr->result=%d")
TRACE_MSG(MMIDL_3848_112_2_18_2_13_26_103,"HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d")
TRACE_MSG(MMIDL_3880_112_2_18_2_13_26_104,"HandleDLPDPDeactiveInd() msg_ptr->result=%d")
TRACE_MSG(MMIDL_3881_112_2_18_2_13_26_105,"HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d")
TRACE_MSG(MMIDL_3902_112_2_18_2_13_26_106,"MMIDL_NetworkIsReady=%d")
TRACE_MSG(MMIDL_3924_112_2_18_2_13_26_107,"DownloadHandleHttpWatingTaskId download_instance Null")
TRACE_MSG(MMIDL_3928_112_2_18_2_13_26_108,"DownloadHandleHttpWatingTaskId task_id=%d, is_increase=%d, num=%d")
TRACE_MSG(MMIDL_3977_112_2_18_2_13_26_109,"HandlePDPActiveSuccess download_instance Null")
TRACE_MSG(MMIDL_3987_112_2_18_2_13_26_110,"HandlePDPActiveSuccess http_waiting_task_id_num=%d")
TRACE_MSG(MMIDL_4016_112_2_18_2_13_26_111,"HandlePDPActiveFail download_instance Null")
TRACE_MSG(MMIDL_4022_112_2_18_2_13_26_112,"HandlePDPActiveFail network not ready!")
TRACE_MSG(MMIDL_4154_112_2_18_2_13_27_113,"MMIAPIDL_GetAudioTypeByNFMimeType mime_type=%d, dl_mime_type=%d")
TRACE_MSG(MMIDL_4219_112_2_18_2_13_27_114,"MMIAPIDL_DeleteCompletedTask dl_instance exist status = %d")
TRACE_MSG(MMIDL_4235_112_2_18_2_13_27_115,"MMIAPIDL_DeleteCompletedTask dl_instance Not exist status = %d")
TRACE_MSG(MMIDL_4296_112_2_18_2_13_27_116,"MMIDL_HandlePDP_Change download_instance Null")
TRACE_MSG(MMIDL_4348_112_2_18_2_13_27_117,"MMIAPIDL_HandleServiceChange bearer_type = %d")
TRACE_MSG(MMIDL_4357_112_2_18_2_13_27_118,"MMIAPIDL_HandleServiceChange pdp_info.bearer_type = %d")
TRACE_MSG(MMIDL_FILE_349_112_2_18_2_13_28_119,"MMIDL_CombineDownloadFilename_ptr->wstr_len = %d,  path_ptr->wstr_len = %d,is_combine_tmp_file=%d")
TRACE_MSG(MMIDL_FILE_576_112_2_18_2_13_29_120,"GetDownloadFileByURL i=%d, rtn_url_ext=%s")
TRACE_MSG(MMIDL_FILE_706_112_2_18_2_13_29_121,"GetMimeTypeByhttpHeader enter")
TRACE_MSG(MMIDL_FILE_713_112_2_18_2_13_29_122,"GetMimeTypeByhttpHeader dest_str1=0x%x")
TRACE_MSG(MMIDL_FILE_716_112_2_18_2_13_29_123,"GetMimeTypeByhttpHeader dest_str2=0x%x")
TRACE_MSG(MMIDL_FILE_723_112_2_18_2_13_29_124,"GetMimeTypeByhttpHeader dest_str3=0x%x")
TRACE_MSG(MMIDL_FILE_732_112_2_18_2_13_29_125,"GetMimeTypeByhttpHeader dest_len=%d")
TRACE_MSG(MMIDL_FILE_738_112_2_18_2_13_29_126,"GetMimeTypeByhttpHeader dest_str=%s")
TRACE_MSG(MMIDL_FILE_827_112_2_18_2_13_29_127,"MMIDL_GetDefaultFileName type = %d,dl_charset=%d")
TRACE_MSG(MMIDL_FILE_863_112_2_18_2_13_29_128,"MMIDL_GetDownloadType mime_type=%d, is_nf_type=%d")
TRACE_MSG(MMIDL_FILE_1042_112_2_18_2_13_30_129,"MMIDL_GetCustomizedDownloadPath len=%d")
TRACE_MSG(MMIDL_FILE_1167_112_2_18_2_13_30_130,"MMIDL_CreateDownloadFile file not exist!")
TRACE_MSG(MMIDL_FILE_1405_112_2_18_2_13_31_131,"GetCharsetByhttpHeader enter")
TRACE_MSG(MMIDL_FILE_1422_112_2_18_2_13_31_132,"GetCharsetByhttpHeader content_type_len=%d")
TRACE_MSG(MMIDL_FILE_1433_112_2_18_2_13_31_133,"GetCharsetByhttpHeader content_type_ptr=%s")
TRACE_MSG(MMIDL_FILE_1444_112_2_18_2_13_31_134,"GetCharsetByhttpHeader dest_str1=%s")
TRACE_MSG(MMIDL_FUNC_266_112_2_18_2_13_32_135,"MMIDL_DeleteDownloadManagerTask index=%d, is_all=%d")
TRACE_MSG(MMIDL_FUNC_436_112_2_18_2_13_32_136,"MMIDL_ChangeDownloadTaskStatus index = %d, status = %d")
TRACE_MSG(MMIDL_FUNC_567_112_2_18_2_13_32_137,"MMIDL_ChangeDownloadTaskFileName index = %d, error")
TRACE_MSG(MMIDL_FUNC_586_112_2_18_2_13_32_138,"MMIDL_ChangeDownloadTaskTotalLen index = %d, total_len = %d")
TRACE_MSG(MMIDL_FUNC_608_112_2_18_2_13_32_139,"MMIDL_ChangeDownloadTaskCurLen index = %d, cur_len = %d")
TRACE_MSG(MMIDL_FUNC_627_112_2_18_2_13_32_140,"MMIDL_GetDownloadManagerTotalTaskNum total_task_num=%d")
TRACE_MSG(MMIDL_FUNC_649_112_2_18_2_13_32_141,"MMIDL_GetUnUnusedManagerTaskId unused_id=%d")
TRACE_MSG(MMIDL_FUNC_693_112_2_18_2_13_32_142,"MMIDL_IsSameDownloadDestUrl is_same_url=%d")
TRACE_MSG(MMIDL_FUNC_735_112_2_18_2_13_33_143,"MMIDL_AppendDLTaskListItem invalid status = %d")
TRACE_MSG(MMIDL_FUNC_921_112_2_18_2_13_33_144,"GetDownloadDisplayFilename full_path_len = %d")
TRACE_MSG(MMIDL_FUNC_1197_112_2_18_2_13_33_145,"invalid device")
TRACE_MSG(MMIDL_FUNC_1299_112_2_18_2_13_34_146,"MMIDL_SetPDPInfo download_instance Null")
TRACE_MSG(MMIDL_FUNC_1319_112_2_18_2_13_34_147,"MMIDL_GetPDPInfo download_instance Null")
TRACE_MSG(MMIDL_FUNC_1417_112_2_18_2_13_34_148,"MMIDL_NotifyDownloadComplete index=%d,is_success=%d,sound_notify= %d")
TRACE_MSG(MMIDL_FUNC_1476_112_2_18_2_13_34_149,"MMIDL_NotifyDownloadComplete mime_type is %d")
TRACE_MSG(MMIDL_FUNC_1491_112_2_18_2_13_34_150,"MMIDL_SetMultiTaskNum download_instance Null")
TRACE_MSG(MMIDL_FUNC_1517_112_2_18_2_13_34_151,"MMIDL_SetMultiTaskNum download_instance Null")
TRACE_MSG(MMIDL_FUNC_1534_112_2_18_2_13_34_152,"MMIDL_GetDownloadManager download_instance Null")
TRACE_MSG(MMIDL_FUNC_1551_112_2_18_2_13_34_153,"MMIDL_SetDownloadManager download_instance Null")
TRACE_MSG(MMIDL_FUNC_1587_112_2_18_2_13_34_154,"MMIDL_StartOMADownload enter")
TRACE_MSG(MMIDL_FUNC_1595_112_2_18_2_13_34_155,"MMIDL_StartOMADownload dl_manager->task_info[%d].url=%s")
TRACE_MSG(MMIDL_FUNC_1600_112_2_18_2_13_34_156,"MMIDL_StartOMADownload File_handle is NULL")
TRACE_MSG(MMIDL_FUNC_1607_112_2_18_2_13_34_157,"MMIDL_StartOMADownload File size Incorrect")
TRACE_MSG(MMIDL_FUNC_1614_112_2_18_2_13_34_158,"MMIDL_StartOMADownload No mem")
TRACE_MSG(MMIDL_FUNC_1621_112_2_18_2_13_34_159,"MMIDL_StartOMADownload Read File error")
TRACE_MSG(MMIDL_FUNC_1660_112_2_18_2_13_34_160,"MMIDL_StartOMADownload ODM_StartDownload Failed oma_ret = %d")
TRACE_MSG(MMIDL_WINTAB_679_112_2_18_2_13_38_161,"mmidl_wintab.c:MMIDL_OpenDownloadSaveAsWin() save_as_info is NULL")
TRACE_MSG(MMIDL_WINTAB_692_112_2_18_2_13_38_162,"mmidl_wintab.c:MMIDL_OpenDownloadSaveAsWin() save_as_info->buf is NULL")
TRACE_MSG(MMIDL_WINTAB_707_112_2_18_2_13_38_163,"mmidl_wintab.c:MMIDL_OpenDownloadSaveAsWin() save_as_info->url is NULL")
TRACE_MSG(MMIDL_WINTAB_830_112_2_18_2_13_38_164,"mmidl_wintab.c:MMIDL_OpenOMADownloadQueryWin No MEM")
TRACE_MSG(MMIDL_WINTAB_908_112_2_18_2_13_38_165,"mmidl_wintab.c:HandleDownloadAuthWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIDL_WINTAB_1084_112_2_18_2_13_38_166,"mmidl_wintab.c:HandleDownloadAuthWinMsg() msg_id=%d")
TRACE_MSG(MMIDL_WINTAB_1111_112_2_18_2_13_38_167,"mmidl_wintab.c:HandleDownloadAuthInputWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIDL_WINTAB_1191_112_2_18_2_13_39_168,"Download_OpenFileCallBack download_instance Null")
TRACE_MSG(MMIDL_WINTAB_1202_112_2_18_2_13_39_169,"Download_OpenFileCallBack is_success = %d")
TRACE_MSG(MMIDL_WINTAB_1208_112_2_18_2_13_39_170,"Download_OpenFileCallBack, file_num = %d")
TRACE_MSG(MMIDL_WINTAB_1213_112_2_18_2_13_39_171,"Download_OpenFileCallBack pathlen = %d")
TRACE_MSG(MMIDL_WINTAB_1248_112_2_18_2_13_39_172,"Download_OpenLabelCallBack is_success = %d")
TRACE_MSG(MMIDL_WINTAB_1254_112_2_18_2_13_39_173,"Download_OpenLabelCallBack, file_num = %d")
TRACE_MSG(MMIDL_WINTAB_1423_112_2_18_2_13_39_174,"HandleDownloadSaveWinMsg download_instance Null")
TRACE_MSG(MMIDL_WINTAB_1432_112_2_18_2_13_39_175,"HandleDownloadSaveWinMsg mime_type is %d")
TRACE_MSG(MMIDL_WINTAB_1433_112_2_18_2_13_39_176,"mmidl_wintab.c:HandleDownloadSaveWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIDL_WINTAB_1493_112_2_18_2_13_39_177,"HandleDownloadSaveWinMsg mime_type is %d")
TRACE_MSG(MMIDL_WINTAB_1590_112_2_18_2_13_39_178,"HandleDownloadSaveWinMsg task_index=%d")
TRACE_MSG(MMIDL_WINTAB_1683_112_2_18_2_13_40_179,"mmidl_wintab.c:HandleDownloadSaveWinMsg() msg_id=%d")
TRACE_MSG(MMIDL_WINTAB_1730_112_2_18_2_13_40_180,"mmidl_wintab.c:HandleDownloadSaveAsWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIDL_WINTAB_1942_112_2_18_2_13_40_181,"mmidl_wintab.c:HandleInputDownloadFileName() msg_id=0x%04x")
TRACE_MSG(MMIDL_WINTAB_2037_112_2_18_2_13_40_182,"mmidl_wintab.c:HandleDownloadManagerWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIDL_WINTAB_2138_112_2_18_2_13_40_183,"mmidl_wintab.c:HandleDownloadManagerOptWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIDL_WINTAB_2193_112_2_18_2_13_41_184,"HandleDownloadManagerOptWinMsg() not handled menu")
TRACE_MSG(MMIDL_WINTAB_2277_112_2_18_2_13_41_185,"SetDownloadManagerOptionGray index = %d, total_task_num = %d")
TRACE_MSG(MMIDL_WINTAB_2280_112_2_18_2_13_41_186,"SetDownloadManagerOptionGray status= %d")
TRACE_MSG(MMIDL_WINTAB_2358_112_2_18_2_13_41_187,"HandleDownloadTaskDetailWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIDL_WINTAB_2385_112_2_18_2_13_41_188,"mmidl_wintab.c:HandleDownloadQueryWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIDL_WINTAB_2498_112_2_18_2_13_41_189,"mmidl_wintab.c:HandleDownloadQueryWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIDL_WINTAB_2527_112_2_18_2_13_41_190,"HandleDownloadQueryWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIDL_WINTAB_2547_112_2_18_2_13_41_191,"mmidl_wintab.c:HandleDownloadQueryWinMsg() msg_id=0x%04x")
END_TRACE_MAP(MMI_APP_DL_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_DL_TRC_H_


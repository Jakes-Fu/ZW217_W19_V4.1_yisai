/******************************************************************************
 ** File Name:      mmi_app_browser_trc.h                                         *
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
//trace_id:155
#ifndef _MMI_APP_BROWSER_TRC_H_
#define _MMI_APP_BROWSER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIBROWSER_BOOKMARK_200_112_2_18_2_5_38_0 "MMIBrw RenameFile full_path_len %d new_file_name_len %d !!"
#define MMIBROWSER_BOOKMARK_708_112_2_18_2_5_39_1 "MMIBRW_UpdateBookmarkTotalNum bookmark_total_num=%d"
#define MMIBROWSER_BOOKMARK_750_112_2_18_2_5_39_2 "[BRW]MMIBRW_BookmarkInit bookmark_num=%d"
#define MMIBROWSER_BOOKMARK_755_112_2_18_2_5_39_3 "[BRW]W MMIBRW_BookmarkInit s_browser_default_dev=%d, s_browser_path_len=%d"
#define MMIBROWSER_BOOKMARK_760_112_2_18_2_5_39_4 "[BRW]W MMIBRW_BookmarkInit ALLOC FAIL"
#define MMIBROWSER_BOOKMARK_765_112_2_18_2_5_39_5 "[BRW]MMIBRW_BookmarkInit Inited!"
#define MMIBROWSER_BOOKMARK_776_112_2_18_2_5_39_6 "[BRW]MMIBRW_BookmarkInit Folder is Exist!"
#define MMIBROWSER_BOOKMARK_822_112_2_18_2_5_39_7 "[BRW]MMIBRW_BookmarkInitAfterStartup bookmark_num=%d"
#define MMIBROWSER_BOOKMARK_826_112_2_18_2_5_39_8 "[BRW]MMIBRW_BookmarkInitAfterStartup IsFolderExist"
#define MMIBROWSER_BOOKMARK_837_112_2_18_2_5_39_9 "[BRW]W MMIBRW_BookmarkInitAfterStartup s_browser_default_dev=%d, s_browser_path_len=%d"
#define MMIBROWSER_BOOKMARK_843_112_2_18_2_5_39_10 "[BRW]W MMIBRW_BookmarkInitAfterStartup s_bookmark_info_ptr PNULL"
#define MMIBROWSER_BOOKMARK_1153_112_2_18_2_5_40_11 "[BRW]MMIBRW_ReloadBookmarkFileArray num=%d"
#define MMIBROWSER_BOOKMARK_1169_112_2_18_2_5_40_12 "[BRW]MMIBRW_ReloadBookmarkFileArray folder_num=%d"
#define MMIBROWSER_BOOKMARK_1243_112_2_18_2_5_40_13 "MMIBRW_BookmarkGotoURL: url_len=%d, url_ptr= %s"
#define MMIBROWSER_BOOKMARK_1283_112_2_18_2_5_40_14 "[BRW]W MMIBRW_AddBookmark:wstr_ptr PNULL"
#define MMIBROWSER_BOOKMARK_1322_112_2_18_2_5_40_15 "[BRW]W MMIBRW_AddBookmark:string_dest ALLOC FAIL"
#define MMIBROWSER_BOOKMARK_1334_112_2_18_2_5_40_16 "[BRW]W MMIBRW_AddBookmark:url_buf ALLOC FAIL"
#define MMIBROWSER_BOOKMARK_1352_112_2_18_2_5_40_17 "[BRW]W MMIBRW_AddBookmark:write file sfs_error =%d"
#define MMIBROWSER_BOOKMARK_1404_112_2_18_2_5_40_18 "[BRW]W MMIBRW_AddBookmark:MMIFILEARRAY_Add fail!"
#define MMIBROWSER_BOOKMARK_1430_112_2_18_2_5_40_19 "[BRW]W MMIBRW_AddBookmarkFolder:in_title_str.wstr_ptr = 0x%x"
#define MMIBROWSER_BOOKMARK_1468_112_2_18_2_5_40_20 "[BRW]W MMIBRW_AddBookmarkFolder:MMIFILEARRAY_Add fail"
#define MMIBROWSER_BOOKMARK_1474_112_2_18_2_5_40_21 "[BRW]W MMIBRW_AddBookmarkFolder:MMIFILE_CreateDir fail"
#define MMIBROWSER_BOOKMARK_1499_112_2_18_2_5_40_22 "[BRW]MMIBRW_ChangeBookmarkTitle:in_title_str.wstr_ptr = 0x%x"
#define MMIBROWSER_BOOKMARK_1547_112_2_18_2_5_40_23 "[BRW]MMIBRW_ChangeBookmarkTitle:MMIFILEARRAY_Read fail"
#define MMIBROWSER_BOOKMARK_1567_112_2_18_2_5_40_24 "[BRW]MMIBRW_ChangeBookmarkTitle:MMIFILEARRAY_Modify result =%d"
#define MMIBROWSER_BOOKMARK_1593_112_2_18_2_5_40_25 "[BRW]W MMIBRW_ChangeBookmarkUrl:in_url_str.wstr_ptr = 0x%x"
#define MMIBROWSER_BOOKMARK_1598_112_2_18_2_5_40_26 "[BRW]MMIBRW_ChangeBookmarkUrl:in_index = %d"
#define MMIBROWSER_BOOKMARK_1611_112_2_18_2_5_40_27 "[BRW]W MMIBRW_ChangeBookmarkUrl:dest_wstr_ptr alloc fail"
#define MMIBROWSER_BOOKMARK_1625_112_2_18_2_5_40_28 "[BRW]W MMIBRW_ChangeBookmarkUrl:url_buf alloc fail"
#define MMIBROWSER_BOOKMARK_1630_112_2_18_2_5_40_29 "[BRW]MMIBRW_ChangeBookmarkUrl buf=%s"
#define MMIBROWSER_BOOKMARK_1637_112_2_18_2_5_40_30 "[BRW]MMIBRW_ChangeBookmarkUrl:write file data result = %d"
#define MMIBROWSER_BOOKMARK_1644_112_2_18_2_5_41_31 "[BRW]W MMIBRW_ChangeBookmarkUrl: MMIFILEARRAY_Read fail"
#define MMIBROWSER_BOOKMARK_1668_112_2_18_2_5_41_32 "[BRW]MMIBRW_ChangeBookmarkUrl:MMIFILEARRAY_Modify result = %d"
#define MMIBROWSER_BOOKMARK_1876_112_2_18_2_5_41_33 "MMIBRW_CleanBookmarkData root_file_array == PNULL"
#define MMIBROWSER_BOOKMARK_1938_112_2_18_2_5_41_34 "[BRW]MMIBRW_CleanBookmarkData bookmark_num=%d"
#define MMIBROWSER_BOOKMARK_1943_112_2_18_2_5_41_35 "MMIBRW_CleanBookmarkData result=%d"
#define MMIBROWSER_BOOKMARK_2015_112_2_18_2_5_41_36 "MMIBRW_SetDefaultBookmark ret = %d"
#define MMIBROWSER_BOOKMARK_2021_112_2_18_2_5_41_37 "MMIBRW_SetDefaultBookmark NO memory"
#define MMIBROWSER_BOOKMARK_2057_112_2_18_2_5_41_38 "MMIBRW_SetDefaultBookmark ret = %d"
#define MMIBROWSER_BOOKMARK_2061_112_2_18_2_5_41_39 "MMIBRW_SetDefaultBookmark NO memory"
#define MMIBROWSER_BOOKMARK_2597_112_2_18_2_5_42_40 "[BRW]W MMIBROWSER_BookmarkGetRootPath:dst_path_name_ptr alloc fail"
#define MMIBROWSER_BOOKMARK_2611_112_2_18_2_5_42_41 "[BRW]MMIBROWSER_BookmarkGetRootPath result =%d, dev =%d"
#define MMIBROWSER_BOOKMARK_2666_112_2_18_2_5_43_42 "[BRW]MMIBROWSER_LoadFinish num=%d"
#define MMIBROWSER_BOOKMARK_2683_112_2_18_2_5_43_43 "[BRW]MMIBROWSER_LoadFinish folder_num=%d"
#define MMIBROWSER_DORADO_API_95_112_2_18_2_5_43_44 "[MMIBROWSER]: HandleSIMPlugNotifyBrowserFunc dual_sys=%d, notify_event=%d"
#define MMIBROWSER_DORADO_API_280_112_2_18_2_5_44_45 "[BRW]MMIDORADO_Entry dual_sys = %d"
#define MMIBROWSER_DORADO_API_393_112_2_18_2_5_44_46 "[ATC:][FT:][BOOKMARK:]Atest_BROWSERBookmarkTitleInput  res = %d"
#define MMIBROWSER_DORADO_API_416_112_2_18_2_5_44_47 "[ATC:][FT:][BOOKMARK:]Atest_BROWSERBookmarkURLInput  res=%d"
#define MMIBROWSER_FUNC_195_112_2_18_2_5_45_48 "[BRW]W MMIBROWSER_Enter: Try later"
#define MMIBROWSER_FUNC_206_112_2_18_2_5_45_49 "[BRW]W MMIBROWSER_Init error"
#define MMIBROWSER_FUNC_215_112_2_18_2_5_45_50 "[BRW]MMIBROWSER_Enter:avtive_sim:%d"
#define MMIBROWSER_FUNC_219_112_2_18_2_5_45_51 "[BRW]MMIBROWSER_Enter sim mcc:%d, mnc:%d"
#define MMIBROWSER_FUNC_224_112_2_18_2_5_45_52 "[BRW]MMIBROWSER_Enter type = %d"
#define MMIBROWSER_FUNC_271_112_2_18_2_5_45_53 "[BRW]W MMIBROWSER_Init:InitMem error"
#define MMIBROWSER_FUNC_313_112_2_18_2_5_45_54 "[BRW]MMIBROWSER_Exit"
#define MMIBROWSER_FUNC_354_112_2_18_2_5_45_55 "[BRW]MMIBROWSER_Exit has done or doing"
#define MMIBROWSER_FUNC_368_112_2_18_2_5_45_56 "[BRW]MMIBROWSER_StartWeb:is_browser_inited = %d"
#define MMIBROWSER_FUNC_431_112_2_18_2_5_45_57 "[BRW]MMIBROWSER_ExitWeb"
#define MMIBROWSER_FUNC_450_112_2_18_2_5_45_58 "[BRW]MMIBROWSER_ExitWeb has done"
#define MMIBROWSER_FUNC_536_112_2_18_2_5_45_59 "[BRW]MMIBROWSER_SelectNetworkType:sim_num=%d,dual_sys=%d,is_use_wifi=%d"
#define MMIBROWSER_FUNC_618_112_2_18_2_5_46_60 "[BRW]W MMIBROWSER_Enter MMIAPICONNECTION_GetLinkSettingItemByIndex error"
#define MMIBROWSER_FUNC_660_112_2_18_2_5_46_61 "[BRW]W MMIBROWSER_SetHomepage result =%d"
#define MMIBROWSER_FUNC_759_112_2_18_2_5_46_62 "[BRW]W SetNumState2String alloc fail"
#define MMIBROWSER_FUNC_764_112_2_18_2_5_46_63 "[BRW]W SetNumState2String ERROR"
#define MMIBROWSER_FUNC_787_112_2_18_2_5_46_64 "[BRW]W MMIBROWSER_MergeTextAndNum error"
#define MMIBROWSER_FUNC_815_112_2_18_2_5_46_65 "[BRW]W MMIBROWSER_MergeTextAndNum alloc fail--text_str.wstr_len =%d"
#define MMIBROWSER_FUNC_889_112_2_18_2_5_46_66 "[BRW]W MMIBROWSER_FmmPathToDisplay param error"
#define MMIBROWSER_FUNC_909_112_2_18_2_5_46_67 "[BRW]W MMIBROWSER_FmmPathToDisplay err dev"
#define MMIBROWSER_FUNC_931_112_2_18_2_5_46_68 "[BRW]W MMIBROWSER_DisplayToFmmPath param error"
#define MMIBROWSER_FUNC_950_112_2_18_2_5_46_69 "[BRW]W MMIBROWSER_DisplayToFmmPath err dev"
#define MMIBROWSER_FUNC_1007_112_2_18_2_5_46_70 "MMIDL_CombineDownloadFilename_ptr->wstr_len = %d,  path_ptr->wstr_len = %d,is_combine_tmp_file=%d"
#define MMIBROWSER_FUNC_1071_112_2_18_2_5_47_71 "[BRW]MMIBROWSER BrowserDlGetDefaultMediaDir:path_ptr=0x%x,dir_path_len_ptr=0x%x"
#define MMIBROWSER_FUNC_1147_112_2_18_2_5_47_72 "[BRW]BrowserDlGetDeviceName file_type =%d"
#define MMIBROWSER_FUNC_1170_112_2_18_2_5_47_73 "[BRW]W MMIBROWSER_GetDefaultMediaFilePath full_path_ptr = 0x%x"
#define MMIBROWSER_FUNC_1265_112_2_18_2_5_47_74 "[BRW]MMIBROWSER_HandleZoom gesture_type = %d, is_need_zoom=%d"
#define MMIBROWSER_FUNC_1289_112_2_18_2_5_47_75 "[BRW]MMIBROWSER_GetBrwSysDevName dev =%d"
#define MMIBROWSER_FUNC_1313_112_2_18_2_5_47_76 "[BRW]W MMIBROWSER_GetCurPageUrlInfo error param"
#define MMIBROWSER_FUNC_1320_112_2_18_2_5_47_77 "[BRW]W MMIBROWSER_GetCurPageUrlInfo url_ptr alloc fails"
#define MMIBROWSER_FUNC_1357_112_2_18_2_5_47_78 "[BRW]W MMIBROWSER_GetCurPageUrlInfo wurl_buf_ptr alloc fails"
#define MMIBROWSER_FUNC_1389_112_2_18_2_5_47_79 "[BRW]MMIBROWSER_GetCurPageUrlInfo result =%d"
#define MMIBROWSER_FUNC_1432_112_2_18_2_5_47_80 "[BRW]W MMIBROWSER_GetCurAddrLabelInfo label_info_pptr null"
#define MMIBROWSER_FUNC_1439_112_2_18_2_5_47_81 "[BRW]W MMIBROWSER_GetCurAddrLabelInfo label_info_ptr alloc fails"
#define MMIBROWSER_FUNC_1883_112_2_18_2_5_48_82 "[BRW]GetMyNavDefaultInfo:PARAM null"
#define MMIBROWSER_FUNC_1989_112_2_18_2_5_49_83 "[BRW]W MMIBROWSER_GetIcoPath:dst_path_name_ptr alloc fail"
#define MMIBROWSER_FUNC_2002_112_2_18_2_5_49_84 "[BRW]MMIBROWSER_GetIcoPath result =%d, dev =%d"
#define MMIBROWSER_FUNC_2032_112_2_18_2_5_49_85 "[BRW W]MMIBROWSER_GetIcoFullFileName:FAIL"
#define MMIBROWSER_FUNC_2071_112_2_18_2_5_49_86 "[BRW]MMIBROWSER_DeleteIcoDir:sfs_err=%d"
#define MMIBROWSER_FUNC_2078_112_2_18_2_5_49_87 "[BRW]MMIBROWSER_DeleteIcoDir:result=%d"
#define MMIBROWSER_FUNC_2097_112_2_18_2_5_49_88 "[BRW]MMIBROWSER_DeleteIco sfs_err=%d"
#define MMIBROWSER_FUNC_2105_112_2_18_2_5_49_89 "[BRW]MMIBROWSER_DeleteIco:get name fail"
#define MMIBROWSER_SETTING_140_112_2_18_2_5_50_90 "[BRW]W MMIDORADO_GetNetSettingIndex dual_sys =%d"
#define MMIBROWSER_SETTING_317_112_2_18_2_5_51_91 "MMIBROWSER_Setting_ConnectionCallback,index=%d,dual_sys=%d"
#define MMIBROWSER_SETTING_583_112_2_18_2_5_51_92 "[BRW]MMIBROWSER_UpdateUrl ALLOC FAIL len =%d"
#define MMIBROWSER_SETTING_594_112_2_18_2_5_51_93 "MMIBROWSER_UpdateUrl error_code= %d"
#define MMIBROWSER_SETTING_619_112_2_18_2_5_51_94 "[BRW]MMIBROWSER_UpdateUserAgent ALLOC FAIL len =%d"
#define MMIBROWSER_SETTING_648_112_2_18_2_5_51_95 "[BRW]MMIBROWSER_UpdateUaProfile ALLOC FAIL len =%d"
#define MMIBROWSER_SETTING_679_112_2_18_2_5_51_96 "[BRW]MMIBROWSER_UpdateUserName ALLOC FAIL len =%d"
#define MMIBROWSER_SETTING_716_112_2_18_2_5_51_97 "[BRW]MMIBROWSER_UpdatePassword ALLOC FAIL len =%d"
#define MMIBROWSER_SETTING_1073_112_2_18_2_5_52_98 "MMIBROWSER_SetAnimateConnectCtrlState:state = %d,s_browser_context.animate_state = %d"
#define MMIBROWSER_SETTING_1226_112_2_18_2_5_53_99 "[BRW]W MMIBROWSER_UpdateFileNameReq next_url_ptr ALLOC Fail!!"
#define MMIBROWSER_SETTING_1304_112_2_18_2_5_53_100 "[BRW]MMIBROWSER_UpdateBookmarkEdit title ALLOC FAIL len =%d"
#define MMIBROWSER_SETTING_1325_112_2_18_2_5_53_101 "[BRW]MMIBROWSER_UpdateBookmarkEdit URL ALLOC FAIL len =%d"
#define MMIBROWSER_SETTING_1456_112_2_18_2_5_53_102 "[BRW] MMIBROWSER_UpdateSaveFile ALLOC FAIL LEN=%d"
#define MMIBROWSER_SETTING_1611_112_2_18_2_5_53_103 "brw MMIBROWSER_MIMEDownlaodCallBack mime=%d, url=%s"
#define MMIBROWSER_SETTING_1620_112_2_18_2_5_53_104 "\n MMIBROWSER MIME CALLBACK PARAM ERROR callback"
#define MMIBROWSER_SETTING_1625_112_2_18_2_5_53_105 "\n MMIBROWSER MIME CALLBACK PARAM ERROR param_ptr =%d"
#define MMIBROWSER_SETTING_1697_112_2_18_2_5_53_106 "BRW MMIBROWSER_SetIsUseWifi is_use_wifi = %d"
#define MMIBROWSER_SETTING_1732_112_2_18_2_5_54_107 "[BRW]MMIBROWSER_UpdateSnapshotFilename ALLOC FAIL len =%d"
#define MMIBROWSER_SETTING_1774_112_2_18_2_5_54_108 "[BRW]MMIBROWSER_SetEntryType entry_type = %d"
#define MMIBROWSER_SETTING_1794_112_2_18_2_5_54_109 "[BRW]MMIBROWSER_SetWebEntryType entry_type = %d"
#define MMIBROWSER_SETTING_1844_112_2_18_2_5_54_110 "[BRW]MMIBROWSER_UpdateEntryUrl ALLOC FAIL len =%d"
#define MMIBROWSER_SETTING_1855_112_2_18_2_5_54_111 "MMIBROWSER_UpdateEntryUrl error_code= %d"
#define MMIBROWSER_SETTING_1909_112_2_18_2_5_54_112 "[BRW]MMIBROWSER_UpdateSnapshotUrl ALLOC FAIL len =%d"
#define MMIBROWSER_SETTING_1922_112_2_18_2_5_54_113 "MMIBROWSER_UpdateSnapshotUrl error_code= %d"
#define MMIBROWSER_SETTING_1973_112_2_18_2_5_54_114 "[BRW]MMIBROWSER_UpdateSnapshotFilename ALLOC FAIL len =%d"
#define MMIBROWSER_SETTING_2017_112_2_18_2_5_54_115 "[BRW]MMIDORADO_SetConnectionSettings:connection_index=%d"
#define MMIBROWSER_WINTABLE_2035_112_2_18_2_5_58_116 "[BRW]W BrwSetDDPropertyInfo PNULL"
#define MMIBROWSER_WINTABLE_2256_112_2_18_2_5_59_117 "[BRW]W HandleOMADLQueryWinMsg OK PNULL"
#define MMIBROWSER_WINTABLE_2273_112_2_18_2_5_59_118 "[BRW]W HandleOMADLQueryWinMsg cancel PNULL"
#define MMIBROWSER_WINTABLE_2324_112_2_18_2_5_59_119 "[BRW]HandleControlDisplayReadyInd :title_len = %d"
#define MMIBROWSER_WINTABLE_2333_112_2_18_2_5_59_120 "[BRW]W display page title error, alloc PNULL!"
#define MMIBROWSER_WINTABLE_2340_112_2_18_2_5_59_121 "[BRW]W BRW_MSG_TYPE_DISPLAY_READY_IND:BRW_DisplayPage Error!"
#define MMIBROWSER_WINTABLE_2352_112_2_18_2_5_59_122 "MMIBROWSER need display TRUE"
#define MMIBROWSER_WINTABLE_2375_112_2_18_2_5_59_123 "[BRW]W HandleControlAuthReq:wrong param!"
#define MMIBROWSER_WINTABLE_2485_112_2_18_2_5_59_124 "[BRW]W HandleControlEnterFileNMInd:wrong param!"
#define MMIBROWSER_WINTABLE_2518_112_2_18_2_5_59_125 "[BRW]W HandleControlSslCertInd:wrong param!"
#define MMIBROWSER_WINTABLE_2537_112_2_18_2_5_59_126 "[BRW]W BHandleControlProgressInd:wrong param!"
#define MMIBROWSER_WINTABLE_2543_112_2_18_2_5_59_127 "HandleControlProgressInd:prg_ptr->total_data_len =%d"
#define MMIBROWSER_WINTABLE_2557_112_2_18_2_5_59_128 "BHandleControlProgressInd:receive_data_len = %d,total_data_len =%d"
#define MMIBROWSER_WINTABLE_2756_112_2_18_2_6_0_129 "[BRW]W HandleControlPageGetInd:wrong param!"
#define MMIBROWSER_WINTABLE_2803_112_2_18_2_6_0_130 "[BRW]W HandleControlMediaGetOneInd cur=%d,total=%d"
#define MMIBROWSER_WINTABLE_2820_112_2_18_2_6_0_131 "[BRW]HandleControlPageStopInd req_state =%d"
#define MMIBROWSER_WINTABLE_2872_112_2_18_2_6_0_132 "[BRW]W HandleControlOMADLConfirmReq:wrong param!"
#define MMIBROWSER_WINTABLE_2910_112_2_18_2_6_0_133 "[BRW]HandleControlMCReq"
#define MMIBROWSER_WINTABLE_2934_112_2_18_2_6_0_134 "[BRW]HandleControlSendDTMFReq"
#define MMIBROWSER_WINTABLE_3029_112_2_18_2_6_1_135 "[BRW]HandleControlSendSMSReq"
#define MMIBROWSER_WINTABLE_3035_112_2_18_2_6_1_136 "[BRW]HandleControlSendSmsReq content_wstr->wstr_ptr ALLOC Fail"
#define MMIBROWSER_WINTABLE_3041_112_2_18_2_6_1_137 "[BRW]HandleControlSendSmsReq:wrong param!"
#define MMIBROWSER_WINTABLE_3077_112_2_18_2_6_1_138 "[BRW]HandleControlSendMMSReq"
#define MMIBROWSER_WINTABLE_3088_112_2_18_2_6_1_139 "[BRW]HandleControlSendMmsReq:wrong param!"
#define MMIBROWSER_WINTABLE_3126_112_2_18_2_6_1_140 "[BRW]HandleControlSendMAILTOReq"
#define MMIBROWSER_WINTABLE_3135_112_2_18_2_6_1_141 "[BRW]HandleControlSendMailToReq temp_ptr ALLOC Fail"
#define MMIBROWSER_WINTABLE_3141_112_2_18_2_6_1_142 "[BRW]HandleControlSendMailToReq:wrong param!"
#define MMIBROWSER_WINTABLE_3320_112_2_18_2_6_1_143 "\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl SUCC"
#define MMIBROWSER_WINTABLE_3324_112_2_18_2_6_1_144 "\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl FAIL"
#define MMIBROWSER_WINTABLE_3415_112_2_18_2_6_2_145 "[BRW]MMIBROWSER_DisplayDownloadProgress GUI_IntersectRect FALSE"
#define MMIBROWSER_WINTABLE_3745_112_2_18_2_6_2_146 "\n MMIBROWSER start key timer\n"
#define MMIBROWSER_WINTABLE_3759_112_2_18_2_6_2_147 "\n MMIBROWSER stop key timer\n"
#define MMIBROWSER_WINTABLE_3869_112_2_18_2_6_2_148 "\n MMIBROWSER BrwHandleMainWinMsg BRW_InitInstance error"
#define MMIBROWSER_WINTABLE_3926_112_2_18_2_6_3_149 "BrwHandleMainOpenWinMsg: BRW_AccessPage error"
#define MMIBROWSER_WINTABLE_3971_112_2_18_2_6_3_150 "BrwHandleMainWinMsg:msg_id = 0x%x"
#define MMIBROWSER_WINTABLE_4105_112_2_18_2_6_3_151 "MMIBROWSER DISPLAY = %d"
#define MMIBROWSER_WINTABLE_4117_112_2_18_2_6_3_152 "[BRW]W BrwHandleMainWinMsg BRW_DisplayPage error instance=%d"
#define MMIBROWSER_WINTABLE_4127_112_2_18_2_6_3_153 "[BRW]W BrwHandleMainWinMsg BRW_RepaintPage error instance=%d"
#define MMIBROWSER_WINTABLE_4180_112_2_18_2_6_3_154 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 1"
#define MMIBROWSER_WINTABLE_4193_112_2_18_2_6_3_155 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 2"
#define MMIBROWSER_WINTABLE_4206_112_2_18_2_6_3_156 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 3"
#define MMIBROWSER_WINTABLE_4219_112_2_18_2_6_3_157 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 4"
#define MMIBROWSER_WINTABLE_4233_112_2_18_2_6_3_158 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 7"
#define MMIBROWSER_WINTABLE_4257_112_2_18_2_6_3_159 "\n MMIBROWSER BrwHandleMainWinMsg BRW_AccessPage error"
#define MMIBROWSER_WINTABLE_4281_112_2_18_2_6_3_160 "BrwHandleMainWinMsg:MSG_CTL_CANCEL BRW_StopPageReq error"
#define MMIBROWSER_WINTABLE_4353_112_2_18_2_6_3_161 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcLayoutTimer error"
#define MMIBROWSER_WINTABLE_4370_112_2_18_2_6_3_162 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 1"
#define MMIBROWSER_WINTABLE_4382_112_2_18_2_6_3_163 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 2"
#define MMIBROWSER_WINTABLE_4393_112_2_18_2_6_4_164 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 3"
#define MMIBROWSER_WINTABLE_4404_112_2_18_2_6_4_165 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 4"
#define MMIBROWSER_WINTABLE_4415_112_2_18_2_6_4_166 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 4"
#define MMIBROWSER_WINTABLE_4479_112_2_18_2_6_4_167 "[BRW]:MMIBROWSER_HandleZoom gesture_type =%d"
#define MMIBROWSER_WINTABLE_4528_112_2_18_2_6_4_168 "[BRW]MMIBROWSER_GetDefaultUserAgent mcc:%d, mnc:%d"
#define MMIBROWSER_WINTABLE_4545_112_2_18_2_6_4_169 "[BRW]MMIBROWSER_GetDefaultUserAgent PS is invalid"
#define MMIBROWSER_WINTABLE_4581_112_2_18_2_6_4_170 "\n MMIBROWSER MMIBROWSER_SetProfile MMIBROWSER_GetNVSetting error"
#define MMIBROWSER_WINTABLE_4626_112_2_18_2_6_4_171 "\n MMIBROWSER MMIBROWSER_SetProfile MMIAPICONNECTION_GetLinkSettingItemByIndex error"
#define MMIBROWSER_WINTABLE_5125_112_2_18_2_6_5_172 "[BRW] BrwHandleEditUrlWinMsg PNULL == need_item_data_ptr"
#define MMIBROWSER_WINTABLE_5221_112_2_18_2_6_5_173 "StopButtonCallBack BRW_AccessPage error"
#define MMIBROWSER_WINTABLE_5238_112_2_18_2_6_5_174 "StopButtonCallBack() BRW_StopPageReq error"
#define MMIBROWSER_WINTABLE_5357_112_2_18_2_6_6_175 "brw mmi get callback msg, id:%d"
#define MMIBROWSER_WINTABLE_5376_112_2_18_2_6_6_176 "[BRW]MMIBROWSER_OpenCCSelectWin error1"
#define MMIBROWSER_WINTABLE_5382_112_2_18_2_6_6_177 "[BRW]MMIBROWSER_OpenCCSelectWin invalid number"
#define MMIBROWSER_WINTABLE_5405_112_2_18_2_6_6_178 "BRW mmi cc enter selectwin  %s"
#define MMIBROWSER_WINTABLE_5430_112_2_18_2_6_6_179 "[BRW]MMIDORADO_HandleBrwControlMsg:brw_message_id:%d"
#define MMIBROWSER_WINTABLE_5435_112_2_18_2_6_6_180 "[BRW]MMIDORADO_HandleBrwControlMsg:MSG_BRW_STOP_BGSOUND"
#define MMIBROWSER_WINTABLE_5455_112_2_18_2_6_6_181 "[BRW]W MMIDORADO_HandleBrwControlMsg BRW_RepaintPage ERROR"
#define MMIBROWSER_WINTABLE_5616_112_2_18_2_6_6_182 "[BRW]BrwShowErrMsg error=%d"
#define MMIBROWSER_WINTABLE_5928_112_2_18_2_6_7_183 "BRW:MMIBROWSER_UpdateAddrLable url_ptr is PNULL!"
#define MMIBROWSER_WINTABLE_6127_112_2_18_2_6_7_184 "[BRW]BrwSIMSelectCallback sys_state[%d] = MMI_DUAL_SYS_MAX"
#define MMIBROWSER_WINTABLE_6301_112_2_18_2_6_7_185 "BrwHandleInputFileNameWinMsg suffix name len ==0, ERROR"
#define MMIBROWSER_WINTABLE_6394_112_2_18_2_6_8_186 "[BRW]BrwDlGetDefaultFileName is_exit_ext_name = %d"
#define MMIBROWSER_WINTABLE_6429_112_2_18_2_6_8_187 "BrwHandlePopMenuWinMsg:msg_id = 0x%x"
#define MMIBROWSER_WINTABLE_6443_112_2_18_2_6_8_188 "BrwHandlePopMenuWinMsg:menu_id = 0x%x"
#define MMIBROWSER_WINTABLE_6553_112_2_18_2_6_8_189 "ID_POPUP_MENU_STOP BRW_StopPageReq error"
#define MMIBROWSER_WINTABLE_6574_112_2_18_2_6_8_190 "MMIBROWSER forward error"
#define MMIBROWSER_WINTABLE_6585_112_2_18_2_6_8_191 "MMIBROWSER backward error"
#define MMIBROWSER_WINTABLE_6776_112_2_18_2_6_8_192 "\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCache fail"
#define MMIBROWSER_WINTABLE_6788_112_2_18_2_6_8_193 "\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCookie fail"
#define MMIBROWSER_WINTABLE_6817_112_2_18_2_6_8_194 "\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCache fail"
#define MMIBROWSER_WINTABLE_6823_112_2_18_2_6_8_195 "\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCookie fail"
#define MMIBROWSER_WINTABLE_7120_112_2_18_2_6_9_196 "BrwScrollBarCreate ::PNULL==rect_ptr"
#define MMIBROWSER_WINTABLE_7662_112_2_18_2_6_10_197 "MMIBROWSER_StopLoading: BRW_StopPageReq error"
#define MMIBROWSER_WINTABLE_7919_112_2_18_2_6_11_198 "[BRW]HandleSlide 1"
#define MMIBROWSER_WINTABLE_7930_112_2_18_2_6_11_199 "[BRW]HandleSlide 2"
#define MMIBROWSER_WINTABLE_8033_112_2_18_2_6_11_200 "MMIBROWSER MMIBROWSER_HandleTpPress BRW_ProcPenDownEvent error"
#define MMIBROWSER_WINTABLE_8091_112_2_18_2_6_11_201 "[BRW]MMIBROWSER_HandleTpPress BRW_ProcPenUpEvent error"
#define MMIBROWSER_WINTABLE_8106_112_2_18_2_6_11_202 "[BRW]MMIBROWSER_HandleTpPress fling_velocity =%d"
#define MMIBROWSER_WINTABLE_8180_112_2_18_2_6_11_203 "MMIBROWSER_HandleTpPress s_is_handle_move=%d, s_is_moving=%d"
#define MMIBROWSER_WINTABLE_8181_112_2_18_2_6_11_204 "MMIBROWSER_HandleTpPress s_move_x=%d, s_move_y=%d"
#define MMIBROWSER_WINTABLE_8199_112_2_18_2_6_11_205 "MMIBROWSER MMIBROWSER_HandleTpPress BRW_ProcPenDownEvent error"
#define MMIBROWSER_WINTABLE_8220_112_2_18_2_6_11_206 "MMIBROWSER MMIBROWSER_HandleTpPress BRW_ProcPenUpEvent error"
#define MMIBROWSER_WINTABLE_8275_112_2_18_2_6_11_207 "[BRW]W MMIBROWSER_EnterNewUrl url_ptr PNULL"
#define MMIBROWSER_WINTABLE_8329_112_2_18_2_6_12_208 "[BRW] BrwPrepareToDL error file_name_req_ptr = PNULL"
#define MMIBROWSER_WINTABLE_8512_112_2_18_2_6_12_209 "[BRW] BrwPrepareToDL generate file name fail"
#define MMIBROWSER_WINTABLE_8611_112_2_18_2_6_12_210 "[BRW] MMIBROWSER_DelInvalidDlFile"
#define MMIBROWSER_WINTABLE_8618_112_2_18_2_6_12_211 "[BRW] MMIBROWSER_DelInvalidDlFile result = %d"
#define MMIBROWSER_WINTABLE_8633_112_2_18_2_6_12_212 "[BRW]MMIBROWSER_StopDownload SUCC"
#define MMIBROWSER_WINTABLE_8637_112_2_18_2_6_12_213 "[BRW]MMIBROWSER_StopDownload FAIL"
#define MMIBROWSER_WINTABLE_8792_112_2_18_2_6_13_214 "\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl SUCC"
#define MMIBROWSER_WINTABLE_8796_112_2_18_2_6_13_215 "\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl FAIL"
#define MMIBROWSER_WINTABLE_8902_112_2_18_2_6_13_216 "\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl SUCC"
#define MMIBROWSER_WINTABLE_8906_112_2_18_2_6_13_217 "\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl FAIL"
#define MMIBROWSER_WINTABLE_9098_112_2_18_2_6_14_218 "[BRW]W SetDownloadFileNamePathWindow file_name_req_ptr PNULL"
#define MMIBROWSER_WINTABLE_9125_112_2_18_2_6_14_219 "[BRW]BrwOpenLabelCallBack is_success = %d"
#define MMIBROWSER_WINTABLE_9131_112_2_18_2_6_14_220 "[BRW]BrwOpenLabelCallBack, file_num = %d"
#define MMIBROWSER_WINTABLE_9148_112_2_18_2_6_14_221 "[BRW]BrwOpenLabelCallBack, file_type = %d"
#define MMIBROWSER_WINTABLE_9912_112_2_18_2_6_16_222 "BrwHandleStartPagePopMenuWinMsg:menu_id = 0x%x"
#define MMIBROWSER_WINTABLE_10037_112_2_18_2_6_16_223 "\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCache fail"
#define MMIBROWSER_WINTABLE_10049_112_2_18_2_6_16_224 "\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCookie fail"
#define MMIBROWSER_WINTABLE_10077_112_2_18_2_6_16_225 "\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCache fail"
#define MMIBROWSER_WINTABLE_10083_112_2_18_2_6_16_226 "\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCookie fail"
#define MMIBROWSER_WINTABLE_10313_112_2_18_2_6_17_227 "BrwHandleAlertWinMsg msg_id = %x"
#define MMIBROWSER_WINTABLE_11085_112_2_18_2_6_19_228 "nv_info.item_count=%d"
#define MMIBROWSER_WINTABLE_11472_112_2_18_2_6_19_229 "[BRW]AddTabsAndChildWindows:tab_child_type=%d"
#define MMIBROWSER_WINTABLE_11942_112_2_18_2_6_20_230 "[BRW]BrwHandleStartPageTheme1WinMsg:err cur_index = %d"
#define MMIBROWSER_WINTABLE_12196_112_2_18_2_6_21_231 "[BRW]OpenOneMyNavUrl:err cur_index = %d"
#define MMIBROWSER_WINTABLE_12787_112_2_18_2_6_22_232 "[BRW]BrwSetMyNavDetailWindow:cur_index=%d"
#define MMIBROWSER_WINTABLE_13095_112_2_18_2_6_23_233 "[BRW]BrwHandleStartPageTheme1WinMsg:err cur_index = %d"
#define MMIBROWSER_WINTABLE_13187_112_2_18_2_6_23_234 "MMIBROWSER_SetPublicProfile MMIBROWSER_GetNVSetting error"
#define MMIBROWSER_WINTABLE_13237_112_2_18_2_6_23_235 "[BRW]MMIBROWSER_SetProfile:use customized_connection"
#define MMIBROWSER_WINTABLE_13248_112_2_18_2_6_23_236 "[BRW]MMIBROWSER_SetProfile:index=%d"
#define MMIBROWSER_WINTABLE_13253_112_2_18_2_6_23_237 "[BRW]W MMIBROWSER_SetPublicProfile:GetLinkSettingItemByIndex error"
#define MMIBROWSER_WINTABLE_13495_112_2_18_2_6_23_238 "[BRW] HandleFixedInstanceControlMsg msg_id:%d"
#define MMIBROWSER_WINTABLE_13566_112_2_18_2_6_24_239 "[BRW]MMIBROWSER_HandleFixedCtrlMsg:message_id = %d"
#define MMIBROWSER_WINTABLE_13604_112_2_18_2_6_24_240 "BrwHandleFixHtmlWinMsg:msg_id = 0x%x"
#define MMIBROWSER_WINTABLE_13689_112_2_18_2_6_24_241 "[BRW]W BRW_DisplayPage error instance=%d"
#define MMIBROWSER_WINTABLE_13699_112_2_18_2_6_24_242 "[BRW]W BRW_DisplayPage error instance=%d"
#define MMIBROWSER_WINTABLE_13738_112_2_18_2_6_24_243 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 1"
#define MMIBROWSER_WINTABLE_13750_112_2_18_2_6_24_244 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 2"
#define MMIBROWSER_WINTABLE_13762_112_2_18_2_6_24_245 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 3"
#define MMIBROWSER_WINTABLE_13774_112_2_18_2_6_24_246 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 4"
#define MMIBROWSER_WINTABLE_13787_112_2_18_2_6_24_247 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 7"
#define MMIBROWSER_WINTABLE_13844_112_2_18_2_6_24_248 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcLayoutTimer error"
#define MMIBROWSER_WINTABLE_13860_112_2_18_2_6_24_249 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 1"
#define MMIBROWSER_WINTABLE_13871_112_2_18_2_6_24_250 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 2"
#define MMIBROWSER_WINTABLE_13881_112_2_18_2_6_24_251 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 3"
#define MMIBROWSER_WINTABLE_13891_112_2_18_2_6_24_252 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 4"
#define MMIBROWSER_WINTABLE_13901_112_2_18_2_6_24_253 "\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 4"
#define MMIBROWSER_WINTABLE_13982_112_2_18_2_6_25_254 "[WRE] Add2MainDelFunc  begin:"
#define MMIBROWSER_WINTABLE_14078_112_2_18_2_6_25_255 "[BRW]ID_POPUP_MENU_ADD_TO_MAINMENU: alloc dynamic_item_ptr return PNULL"
#define MMIBROWSER_WINTABLE_14132_112_2_18_2_6_25_256 "[BRW]ID_POPUP_MENU_ADD_TO_MAINMENU: alloc url_ptr return PNULL"
#define MMIBROWSER_WINTABLE_14134_112_2_18_2_6_25_257 "[BRW] ID_POPUP_MENU_ADD_TO_MAINMENU: ret=%d"
#define MMIBROWSER_WINTABLE_BOOKMARK_931_112_2_18_2_6_27_258 "BrwHandleBookmarkWinMsg msg_id= 0x%x"
#define MMIBROWSER_WINTABLE_BOOKMARK_1106_112_2_18_2_6_27_259 "BrwHandleBookmarkWinMsg() String is NULL"
#define MMIBROWSER_WINTABLE_BOOKMARK_1541_112_2_18_2_6_28_260 "[BRW]W MMIBROWSER get item error"
#define MMIBROWSER_WINTABLE_BOOKMARK_1728_112_2_18_2_6_29_261 "[BRW]BrwSetBookmarkEditWindow change_type=%d"
#define MMIBROWSER_WINTABLE_BOOKMARK_1870_112_2_18_2_6_29_262 "[BRW]W BrwSetBookmarkEditWindow MMIBRW_BM_CHANGE_ADD_URL err"
#define MMIBROWSER_WINTABLE_BOOKMARK_2245_112_2_18_2_6_30_263 "\n MMIBROWSER BrwQueryIsDeleteBookmark out_title.wstr_ptr error 1"
#define MMIBROWSER_WINTABLE_BOOKMARK_2254_112_2_18_2_6_30_264 "\n MMIBROWSER BrwQueryIsDeleteBookmark out_url.wstr_ptr error "
#define MMIBROWSER_WINTABLE_BOOKMARK_2289_112_2_18_2_6_30_265 "\n MMIBROWSER BrwQueryIsDeleteBookmark out_title.wstr_ptr error 2"
#define MMIBROWSER_WINTABLE_BOOKMARK_2327_112_2_18_2_6_30_266 "BrwQueryIsDeleteBookmark temp_wptr ALLOC ERROR!!!"
#define MMIBROWSER_WINTABLE_BOOKMARK_2503_112_2_18_2_6_30_267 "BrwDeleteMarkedBookmarkItem : MMIBRW_RemoveBookmark error;item_index = %d"
#define MMIBROWSER_WINTABLE_BOOKMARK_2929_112_2_18_2_6_31_268 "HandleBookmarkMoveWaitWinMsg  copy_result=%d"
#define MMIBROWSER_WINTABLE_BOOKMARK_2997_112_2_18_2_6_31_269 "MMIBRW_HandleMoveBookmark() msg_id=0x%04x"
#define MMIBROWSER_WINTABLE_BOOKMARK_3038_112_2_18_2_6_31_270 "[BRW]W MMIBRW_HandleMoveBookmark() MMIAPIFMM_DeleteFile FAIL"
#define MMIBROWSER_WINTABLE_BOOKMARK_3099_112_2_18_2_6_31_271 "[BRW]W BrwBookmarkMoveTo() PARAM ERROR"
#define MMIBROWSER_WINTABLE_BOOKMARK_3124_112_2_18_2_6_31_272 "[BRW]W BrwBookmarkMoveTo() result=%d"
#define MMIBROWSER_WINTABLE_BOOKMARK_3182_112_2_18_2_6_31_273 "[BRW]W BrwBookmarkBeginMoveTo 1"
#define MMIBROWSER_WINTABLE_BOOKMARK_3187_112_2_18_2_6_31_274 "[BRW]W BrwBookmarkBeginMoveTo 2"
#define MMIBROWSER_WINTABLE_BOOKMARK_3197_112_2_18_2_6_32_275 "[BRW]W BrwBookmarkBeginMoveTo 3"
#define MMIBROWSER_WINTABLE_BOOKMARK_3209_112_2_18_2_6_32_276 "[BRW]W BrwBookmarkBeginMoveTo 4"
#define MMIBROWSER_WINTABLE_BOOKMARK_3219_112_2_18_2_6_32_277 "[BRW]W BrwBookmarkBeginMoveTo 5"
#define MMIBROWSER_WINTABLE_BOOKMARK_3679_112_2_18_2_6_32_278 "[BRW]BrowserHanldeSaveInEditWin:is_save_success = %d"
#define MMIBROWSER_WINTABLE_BOOKMARK_3725_112_2_18_2_6_33_279 "HandleBKOpenMainWaitingWin, msg_id = 0x%x"
#define MMIBROWSER_WINTABLE_BOOKMARK_3767_112_2_18_2_6_33_280 "HandleBKOpenMainWaitingWin, msg_id = 0x%x end"
#define MMIBROWSER_WINTABLE_BOOKMARK_3846_112_2_18_2_6_33_281 "HandleBKOpenStepWaitingWin, msg_id = 0x%x"
#define MMIBROWSER_WINTABLE_BOOKMARK_3907_112_2_18_2_6_33_282 "HandleBKOpenStepWaitingWin, msg_id = 0x%x end"
#define MMIBROWSER_WINTABLE_BOOKMARK_4092_112_2_18_2_6_33_283 "HandleBKSaveWaitingWin, msg_id = 0x%x"
#define MMIBROWSER_WINTABLE_BOOKMARK_4107_112_2_18_2_6_33_284 "[BRW]HandleBKSaveWaitingWin:s_save_result = %d"
#define MMIBROWSER_WINTABLE_BOOKMARK_4206_112_2_18_2_6_34_285 "HandleBKOpenStepWaitingWin, msg_id = 0x%x end"
#define MMIBROWSER_WINTABLE_BOOKMARK_4306_112_2_18_2_6_34_286 "BrwHandleBookmarkPopUpWinMsg msg_id = %x"
#define MMIBROWSER_WINTABLE_BOOKMARK_4547_112_2_18_2_6_34_287 "BrwHandleBookmarkAlertWinMsg msg_id = %x"
#define MMIBROWSER_WINTABLE_HISTORY_290_112_2_18_2_6_36_288 "\n MMIBROWSER BrwHandleHistoryListWinMsg:malloc mem failed"
#define MMIBROWSER_WINTABLE_HISTORY_299_112_2_18_2_6_36_289 "\n MMIBROWSER BrwHandleHistoryListWinMsg BRW_VisitHisGetHisInfoByIndex error"
#define MMIBROWSER_WINTABLE_HISTORY_340_112_2_18_2_6_36_290 "[BRW]BrwHandleHistoryListWinMsg temp_visit_info_ptr = 0x%x"
#define MMIBROWSER_WINTABLE_HISTORY_440_112_2_18_2_6_36_291 "[BRW] BrwHandleHistoryMenuWinMsg menu_id = %d visit_info_ptr == PNULL"
#define MMIBROWSER_WINTABLE_HISTORY_477_112_2_18_2_6_36_292 "[BRW] ID_POPUP_MENU_HISTORY_DETAIL ALLOC FAIL"
#define MMIBROWSER_WINTABLE_HISTORY_664_112_2_18_2_6_36_293 "[BRW]BrwHandleHistoryDetailWinMsg MSG_OPEN_WINDOW visit_his_info_ptr == PNULL"
#define MMIBROWSER_WINTABLE_HISTORY_692_112_2_18_2_6_36_294 "[BRW]BrwHandleHistoryDetailWinMsg msg_id=%d, visit_his_info_ptr == PNULL"
#define MMIBROWSER_WINTABLE_HISTORY_781_112_2_18_2_6_37_295 "BrwHandleHistoryPopUpWinMsg msg_id = %x"
#define MMIBROWSER_WINTABLE_HISTORY_807_112_2_18_2_6_37_296 "BrwHandleHistoryPopUpWinMsg visit_info_ptr = %x"
#define MMIBROWSER_WINTABLE_HISTORY_829_112_2_18_2_6_37_297 "[BRW] ID_POPUP_MENU_HISTORY_DETAIL ALLOC FAIL"
#define MMIBROWSER_WINTABLE_HISTORY_1055_112_2_18_2_6_37_298 "BrwHandleHistoryAlertWinMsg msg_id = %x"
#define MMIBROWSER_WINTABLE_SEARCH_196_112_2_18_2_6_38_299 "[BRW]W MMIBROWSER_GetSearchEngine search_engine[err] = %d"
#define MMIBROWSER_WINTABLE_SEARCH_234_112_2_18_2_6_38_300 "[BRW]W MMIBROWSER_GetKeyInfo:key_data_ptr PNULL"
#define MMIBROWSER_WINTABLE_SEARCH_274_112_2_18_2_6_38_301 "[BRW]W MMIBROWSER_WriteKeyItem:in_url_ptr PNULL "
#define MMIBROWSER_WINTABLE_SEARCH_281_112_2_18_2_6_38_302 "[BRW]W MMIBROWSER_WriteKeyItem:key_ptr Alloc FAIL "
#define MMIBROWSER_WINTABLE_SEARCH_374_112_2_18_2_6_38_303 "[BRW]W MMIBROWSER_GetMatchKeyNum:key_ptr Alloc FAIL "
#define MMIBROWSER_WINTABLE_SEARCH_421_112_2_18_2_6_38_304 "[BRW]W MMIBROWSER_ListMatchKeyName:key_ptr Alloc FAIL "
#define MMIBROWSER_WINTABLE_SEARCH_439_112_2_18_2_6_38_305 "[BRW]W MMIBROWSER_ListMatchKeyName:match_key_ptr Alloc FAIL "
#define MMIBROWSER_WINTABLE_SEARCH_456_112_2_18_2_6_38_306 "[BRW]W MMIBROWSER_ListMatchKeyName:temp_input_key Alloc FAIL "
#define MMIBROWSER_WINTABLE_SEARCH_473_112_2_18_2_6_38_307 "[BRW]W MMIBROWSER_ListMatchKeyName:temp_key_ptr Alloc FAIL and i=%d"
#define MMIBROWSER_WINTABLE_SEARCH_496_112_2_18_2_6_38_308 "[BRW]W MMIBROWSER_ListMatchKeyName:match_key_ptr Alloc FAIL "
#define MMIBROWSER_WINTABLE_SEARCH_508_112_2_18_2_6_38_309 "[BRW]MMIBROWSER_ListMatchKeyName:no match "
#define MMIBROWSER_WINTABLE_SEARCH_546_112_2_18_2_6_39_310 "[BRW]MMIBROWSER_GenerateSearchWeb search_engine = %d"
#define MMIBROWSER_WINTABLE_SEARCH_598_112_2_18_2_6_39_311 "[BRW]W MMIBROWSER_GenerateSearchWeb url_ptr alloc"
#define MMIBROWSER_WINTABLE_SEARCH_606_112_2_18_2_6_39_312 "[BRW]W MMIBROWSER_GenerateSearchWeb str_ptr alloc"
#define MMIBROWSER_WINTABLE_SEARCH_611_112_2_18_2_6_39_313 "[BRW]W MMIBROWSER_GenerateSearchWeb PARAM ERROR"
#define MMIBROWSER_WINTABLE_SEARCH_1088_112_2_18_2_6_40_314 "[BRW]W BrwHandleSearchListWinMsg PNULL == need_item_data_ptr"
#define MMIBROWSER_WINTABLE_SEARCH_1095_112_2_18_2_6_40_315 "[BRW]W BrwHandleSearchListWinMsg:match_key_ptr = 0x%x"
#define MMIBROWSER_WINTABLE_SEARCH_1131_112_2_18_2_6_40_316 "[BRW]W BrwHandleSearchListWinMsg:cur_list_ptr PNULL"
#define MMIBROWSER_WINTABLE_SETTING_851_112_2_18_2_6_42_317 "[BRW]BrwExecuteSetting:reset customized_connection"
#define MMIBROWSER_WINTABLE_SETTING_877_112_2_18_2_6_42_318 "[BRW]W BrwExecuteSetting:BRW_InitInstance error"
#define MMIBROWSER_WINTABLE_SETTING_892_112_2_18_2_6_42_319 "[BRW]W BrwExecuteSetting MMIBROWSER_GetHomepageUrl ERROR"
#define MMIBROWSER_WINTABLE_SETTING_1213_112_2_18_2_6_43_320 "BrwHandleSettingHomepageTypeWinMsg msg_id = %x"
#define MMIBROWSER_WINTABLE_SETTING_1342_112_2_18_2_6_43_321 "BrwHandleSettingFontWinMsg msg_id = %x"
#define MMIBROWSER_WINTABLE_SETTING_1390_112_2_18_2_6_43_322 "BrwHandleSettingHomepageTypeWinMsg msg_id = %x"
#define MMIBROWSER_WINTABLE_SETTING_1753_112_2_18_2_6_44_323 "BrwHandleSetttingResetAlertWinMsg msg_id = %x"
#define MMIBROWSER_WINTABLE_SNAPSHOT_434_112_2_18_2_6_45_324 "\n MMIBROWSER get item error"
#define MMIBROWSER_WINTABLE_SNAPSHOT_589_112_2_18_2_6_45_325 "BrwHandleSnapshotListWinMsg ::  PNULL == need_item_data_ptr"
#define MMIBROWSER_WINTABLE_SNAPSHOT_597_112_2_18_2_6_45_326 "\n MMIBROWSER BrwHandleSnapshotListWinMsg BRW_SnapshotGetItemInfo error"
#define MMIBROWSER_WINTABLE_SNAPSHOT_995_112_2_18_2_6_46_327 "BrwHandleSnapshotSaveFileWinMsg  result_for_check = %d"
#define MMIBROWSER_WINTABLE_SNAPSHOT_1265_112_2_18_2_6_46_328 "\n BROWSER LILI TEST richtext 1"
#define MMIBROWSER_WINTABLE_SNAPSHOT_1269_112_2_18_2_6_46_329 "\n BROWSER LILI TEST richtext 2"
#define MMIBROWSER_WINTABLE_SNAPSHOT_1273_112_2_18_2_6_46_330 "\n BROWSER LILI TEST richtext 3"
#define MMIBROWSER_WINTABLE_SNAPSHOT_1543_112_2_18_2_6_47_331 "\n mmibrowser BrwHandleSnapshotPopMenuWinMsg BRW_SnapshotGetItemInfo error 1"
#define MMIBROWSER_WINTABLE_SNAPSHOT_1553_112_2_18_2_6_47_332 "\n mmibrowser BrwHandleSnapshotPopMenuWinMsg BRW_SnapshotGetItemInfo error 2"
#define MMIBROWSER_WINTABLE_SNAPSHOT_1713_112_2_18_2_6_47_333 "[BRW] BrwDeleteSelectedSnapshot FAIL 1"
#define MMIBROWSER_WINTABLE_SNAPSHOT_1730_112_2_18_2_6_47_334 "[BRW]BrwDeleteSelectedSnapshot FAIL 2"
#define MMIBROWSER_WINTABLE_SNAPSHOT_1741_112_2_18_2_6_47_335 "[BRW] BrwDeleteSelectedSnapshot ALLOC FAIL!"
#define MMIBROWSER_WINTABLE_SNAPSHOT_1825_112_2_18_2_6_48_336 "BrwQueryIsDeleteSnapshot temp_wptr ALLOC ERROR!!!"
#define MMIBROWSER_WINTABLE_SNAPSHOT_1989_112_2_18_2_6_48_337 "\n MMIBROWSER not change snapshot name"
#define MMIBROWSER_WINTABLE_SNAPSHOT_2130_112_2_18_2_6_48_338 "BrwHandlePageMemoPopUpWinMsg msg_id = %x"
#define MMIBROWSER_WINTABLE_SNAPSHOT_2157_112_2_18_2_6_48_339 "\n mmibrowser BrwHandleSnapshotPopMenuWinMsg item_info_ptr = %d"
#define MMIBROWSER_WINTABLE_SNAPSHOT_2352_112_2_18_2_6_49_340 "BrwHandleHistoryAlertWinMsg msg_id = %x"
#define BRW_CONTROL_208_112_2_18_2_6_49_341 "brw BrwSaveDataToFile create file error!"
#define BRW_CONTROL_234_112_2_18_2_6_49_342 "[BRW]BRW_SetParam,temp_profile_type=0x%x"
#define BRW_CONTROL_238_112_2_18_2_6_49_343 "[BRW]W BRW_SetPublicParam,profile=0x%x"
#define BRW_CONTROL_290_112_2_18_2_6_49_344 "[BRW]BRW_SetParam:set ua null!"
#define BRW_CONTROL_322_112_2_18_2_6_50_345 "[BRW]BRW_SetParam:set uaprof null!"
#define BRW_CONTROL_352_112_2_18_2_6_50_346 "[BRW]BRW_SetParam,proxy_addr_ptr null!"
#define BRW_CONTROL_370_112_2_18_2_6_50_347 "[BRW]BRW_SetParam,temp_profile_type=0x%x"
#define BRW_CONTROL_374_112_2_18_2_6_50_348 "[BRW]W BRW_SetParam,instance=0x%x,profile=0x%x"
#define BRW_CONTROL_410_112_2_18_2_6_50_349 "[BRW]W BRW_InitInstance,ERROR option_ptr = 0x%x"
#define BRW_CONTROL_460_112_2_18_2_6_50_350 "BRW_InitInstance:instance_id =0x%x"
#define BRW_CONTROL_472_112_2_18_2_6_50_351 "[BRW]BRW_DestroyInstance instance=0x%x"
#define BRW_CONTROL_504_112_2_18_2_6_50_352 "[BRW]W BRW_DestroyInstance brw_info_ptr PNULL"
#define BRW_CONTROL_577_112_2_18_2_6_50_353 "BRW_DisconnetNetwork"
#define BRW_CONTROL_661_112_2_18_2_6_50_354 "BRW_AccessPage, gwt fail:%s"
#define BRW_CONTROL_691_112_2_18_2_6_50_355 "brw alloc wap request in BRW_AccessPage:%x"
#define BRW_CONTROL_753_112_2_18_2_6_51_356 "BRW BRW_AccessPage init http session"
#define BRW_CONTROL_765_112_2_18_2_6_51_357 "BRW_AccessPage to ActiveNetwork"
#define BRW_CONTROL_884_112_2_18_2_6_51_358 "BRW_DisplayPage,brw_info_ptr null"
#define BRW_CONTROL_899_112_2_18_2_6_51_359 "BRW_DisplayPage,layout_handler null"
#define BRW_CONTROL_904_112_2_18_2_6_51_360 "BrwDisplayWebPage ret = %d"
#define BRW_CONTROL_932_112_2_18_2_6_51_361 "BRW_RepaintPage,layout_handler null"
#define BRW_CONTROL_937_112_2_18_2_6_51_362 "BRW_RepaintPage ret = %d"
#define BRW_CONTROL_952_112_2_18_2_6_51_363 "[BRW]BRW_StopPageReq"
#define BRW_CONTROL_963_112_2_18_2_6_51_364 "BRW_StopPageReq,wap_request_ptr null"
#define BRW_CONTROL_972_112_2_18_2_6_51_365 "BRW_StopPageReq,state =%d"
#define BRW_CONTROL_1021_112_2_18_2_6_51_366 "[BRW]BRW_StopDL"
#define BRW_CONTROL_1024_112_2_18_2_6_51_367 "[BRW]BRW_StopDL,brw_info_ptr=0x%x"
#define BRW_CONTROL_1030_112_2_18_2_6_51_368 "[BRW]BRW_StopDL,invalid param,wap_req_ptr=0x%x"
#define BRW_CONTROL_1046_112_2_18_2_6_51_369 "[BRW]BRW_CheckIfPageNull,brw_info_ptr 0"
#define BRW_CONTROL_1092_112_2_18_2_6_51_370 "BRW_ProcKeyEvent:brw_info_ptr null"
#define BRW_CONTROL_1206_112_2_18_2_6_52_371 "BRW_ProcKeyEvent,layout_handle invalid"
#define BRW_CONTROL_1231_112_2_18_2_6_52_372 "BRW_ProcPenDownEvent:brw_info_ptr null"
#define BRW_CONTROL_1256_112_2_18_2_6_52_373 "BRW_ProcPenDownEvent,layout_handle invalid"
#define BRW_CONTROL_1276_112_2_18_2_6_52_374 "BRW_ProcPenUpEvent:brw_info_ptr null"
#define BRW_CONTROL_1299_112_2_18_2_6_52_375 "BRW_ProcPenUpEvent,layout_handle invalid"
#define BRW_CONTROL_1322_112_2_18_2_6_52_376 "BRW_ProcNavEvent:brw_info_ptr null"
#define BRW_CONTROL_1329_112_2_18_2_6_52_377 "[BRW]BRW_ProcNavEvent:cur_uri_ptr null"
#define BRW_CONTROL_1351_112_2_18_2_6_52_378 "[BRW]BRW_ProcNavEvent:new_uri_ptr null"
#define BRW_CONTROL_1358_112_2_18_2_6_52_379 "BRW_ProcNavEvent,snapshot nav_event:%d"
#define BRW_CONTROL_1369_112_2_18_2_6_52_380 "[BRW]BRW_ProcNavEvent BrwIsUrlIsEquExceptFragment"
#define BRW_CONTROL_1382_112_2_18_2_6_52_381 "BRW_ProcNavEvent,card jumping,event:%d"
#define BRW_CONTROL_1503_112_2_18_2_6_52_382 "BRW_ProcScrollEvent:brw_info_ptr null"
#define BRW_CONTROL_1561_112_2_18_2_6_52_383 "BRW_ProcTpScrollEvent:brw_info_ptr null"
#define BRW_CONTROL_1634_112_2_18_2_6_52_384 "BRW_GetPageUrl:brw_info_ptr null"
#define BRW_CONTROL_1645_112_2_18_2_6_52_385 "BRW_GetPageUrl,cur_display_req_ptr null"
#define BRW_CONTROL_1648_112_2_18_2_6_53_386 "[brw] is snapshot"
#define BRW_CONTROL_1668_112_2_18_2_6_53_387 "BRW_GetPageUrl:url_pptr null"
#define BRW_CONTROL_1682_112_2_18_2_6_53_388 "BRW BrwProcInitHttpCnf"
#define BRW_CONTROL_1686_112_2_18_2_6_53_389 "BRW BrwProcInitHttpCnf brw_info_ptr is NULL"
#define BRW_CONTROL_1691_112_2_18_2_6_53_390 "BRW BrwProcInitHttpCnf success"
#define BRW_CONTROL_1695_112_2_18_2_6_53_391 "BRW BrwProcInitHttpCnf is_need_to_close_http"
#define BRW_CONTROL_1703_112_2_18_2_6_53_392 "BRW BrwProcInitHttpCnf 1"
#define BRW_CONTROL_1710_112_2_18_2_6_53_393 "BRW BrwProcInitHttpCnf 2"
#define BRW_CONTROL_1716_112_2_18_2_6_53_394 "BRW BrwProcInitHttpCnf 3"
#define BRW_CONTROL_1723_112_2_18_2_6_53_395 "BRW BrwProcInitHttpCnf 4"
#define BRW_CONTROL_1733_112_2_18_2_6_53_396 "[BRW]BrwProcErr,err=%d"
#define BRW_CONTROL_1774_112_2_18_2_6_53_397 "[BRW]BrwProcDownloadErr,err=%d"
#define BRW_CONTROL_1787_112_2_18_2_6_53_398 "[BRW]W BrwProcNoMem,brw_info_ptr=0x%x"
#define BRW_CONTROL_1874_112_2_18_2_6_53_399 "[BRW]W BrwStartNewInstance,param_ptr=0x%x!"
#define BRW_CONTROL_1898_112_2_18_2_6_53_400 "[BRW]W BrwStartNewPageReq,param_ptr=0x%x!"
#define BRW_CONTROL_1946_112_2_18_2_6_53_401 "brw alloc wap request in BrwStartNewPageReq:%x"
#define BRW_CONTROL_2021_112_2_18_2_6_53_402 "BrwStartNewPageReq to ActiveNetwork"
#define BRW_CONTROL_2153_112_2_18_2_6_54_403 "BrwAllocMediaObj media_obj_ptr = 0x%x"
#define BRW_CONTROL_2286_112_2_18_2_6_54_404 "[BRW]ProcPageContent"
#define BRW_CONTROL_2297_112_2_18_2_6_54_405 "[BRW]ProcPageContent,mime_type:%d"
#define BRW_CONTROL_2303_112_2_18_2_6_54_406 "[BRW]ProcPageContent,BrwDestroyLayoutCtrl: 0x%x"
#define BRW_CONTROL_2307_112_2_18_2_6_54_407 "[BRW]ProcPageContent,creat layout:%x"
#define BRW_CONTROL_2310_112_2_18_2_6_54_408 "[BRW]ProcPageContent,creat layout fail, mime_type:%d"
#define BRW_CONTROL_2320_112_2_18_2_6_54_409 "brw proc page, BrwProcWmlScript result:%d"
#define BRW_CONTROL_2360_112_2_18_2_6_54_410 "brw proc page, BrwParseWebPage result:%d"
#define BRW_CONTROL_2369_112_2_18_2_6_54_411 "brw proc page, BrwLayout result:%d"
#define BRW_CONTROL_2389_112_2_18_2_6_54_412 "brw ProcPageContent free cache"
#define BRW_CONTROL_2536_112_2_18_2_6_54_413 "[BRW]W brw ProcPageOK:req_ptr=0x%x,brw_info_ptr=0x%x"
#define BRW_CONTROL_2554_112_2_18_2_6_54_414 "[BRW]W ProcMediaContent:brw_info_ptr=0x%x,req_ptr=0x%x"
#define BRW_CONTROL_2564_112_2_18_2_6_54_415 "[BRW] ProcMediaContent:req_ptr->resp_buf_ptr=0x%x"
#define BRW_CONTROL_2565_112_2_18_2_6_54_416 "[BRW] ProcMediaContent:media_obj_ptr->media_req_type=%d"
#define BRW_CONTROL_2602_112_2_18_2_6_54_417 "[BRW]ProcMediaReq:BrwLayout failed"
#define BRW_CONTROL_2624_112_2_18_2_6_54_418 "[BRW]ProcMediaReq:MEDIA_REQ_TYPE_BGSOUND FALSE"
#define BRW_CONTROL_2634_112_2_18_2_6_54_419 "[BRW]ProcMediaReq:BrwLayout failed"
#define BRW_CONTROL_2643_112_2_18_2_6_54_420 "[BRW]ProcMediaReq:BrwLayout failed"
#define BRW_CONTROL_2652_112_2_18_2_6_54_421 "[BRW]ProcMediaReq:BrwLayout failed"
#define BRW_CONTROL_2707_112_2_18_2_6_55_422 "[BRW]W ProcMediaReq,layout_handle=0x%x"
#define BRW_CONTROL_2717_112_2_18_2_6_55_423 "[BRW]ProcMediaReq:media_obj_ptr = NULL!"
#define BRW_CONTROL_2764_112_2_18_2_6_55_424 "BRW_AccessPage:BrwSendHttpRequest failed!"
#define BRW_CONTROL_2787_112_2_18_2_6_55_425 "[BRW]W BrwStartNextMediaReq:brw_info_ptr=0x%x"
#define BRW_CONTROL_2847_112_2_18_2_6_55_426 "[BRW]W BrwStartMediaReq:brw_info_ptr=0x%x"
#define BRW_CONTROL_2865_112_2_18_2_6_55_427 "[BRW]W BrwRestartCurMediaReq:brw_info_ptr=0x%x"
#define BRW_CONTROL_3005_112_2_18_2_6_55_428 "[BRW]W BrwProcHttpReqDone:brw_info_ptr=0x%x,req_pptr=0x%x"
#define BRW_CONTROL_3015_112_2_18_2_6_55_429 "[BRW]BrwProcHttpReqDone"
#define BRW_CONTROL_3028_112_2_18_2_6_55_430 "[BRW]BrwProcHttpReqDone wap_req_ptr->http_req_ptr == 0"
#define BRW_CONTROL_3041_112_2_18_2_6_55_431 "[BRW]W BrwProcHttpReqDone BRWUtil_DecodeGzip result =%d"
#define BRW_CONTROL_3062_112_2_18_2_6_55_432 "BrwProcHttpReqDone page err"
#define BRW_CONTROL_3078_112_2_18_2_6_55_433 "BrwProcHttpReqDone, new page has been started"
#define BRW_CONTROL_3099_112_2_18_2_6_55_434 "[BRW]BrwProcHttpReqDone,777,%x,respbuf:%x"
#define BRW_CONTROL_3115_112_2_18_2_6_55_435 "[BRW]BrwProcHttpReqDone,BRW_STATE_DOWNLOADING:transfer_encoding = %d"
#define BRW_CONTROL_3211_112_2_18_2_6_56_436 "[BRW]BrwProcHttpReqDone:BRWUtil_DecodeGzip result =%d"
#define BRW_CONTROL_3259_112_2_18_2_6_56_437 "BrwProcCloseHttpCnf,brw_info_ptr=0x%x"
#define BRW_CONTROL_3314_112_2_18_2_6_56_438 "[BRW]W GetBrwInfoPtrByWapReqID,req_id=%d"
#define BRW_CONTROL_3326_112_2_18_2_6_56_439 "[BRW]W GetBrwInfoPtrByWapReqID,req_id=%d"
#define BRW_CONTROL_3344_112_2_18_2_6_56_440 "[BRW]W BrwGetBrwInfoPtrByDLSessionID,session_id=%d"
#define BRW_CONTROL_3356_112_2_18_2_6_56_441 "[BRW]W BrwGetBrwInfoPtrByDLSessionID,session_id=%d"
#define BRW_CONTROL_3373_112_2_18_2_6_56_442 "[BRW]W BrwGetBrwInfoPtrByHttpReq,http_req_ptr=0x%x"
#define BRW_CONTROL_3386_112_2_18_2_6_56_443 "[BRW]W BrwGetBrwInfoPtrByHttpReq,http_req_ptr=0x%x"
#define BRW_CONTROL_3417_112_2_18_2_6_56_444 "[BRW]W BrwGetWapReq,req_id=0x%x,brw_info_ptr=0x%x"
#define BRW_CONTROL_3461_112_2_18_2_6_56_445 "[BRW]W BrwGetWapReqByDLSessionID:brw_info_ptr=0x%x"
#define BRW_CONTROL_3479_112_2_18_2_6_56_446 "[BRW]W BrwGetWapReqByHttpReq:brw_info_ptr=0x%x"
#define BRW_CONTROL_3972_112_2_18_2_6_57_447 "brw alloc wap request in BrwBackupWapRequest:%x"
#define BRW_CONTROL_4031_112_2_18_2_6_57_448 "[BRW]BrwFreeWapReqStruct:is_cached_req!"
#define BRW_CONTROL_4034_112_2_18_2_6_57_449 "[BRW]free wap request:0x%x"
#define BRW_CONTROL_4096_112_2_18_2_6_57_450 "[BRW]BrwFreeWapReqStruct,BrwDestroyLayoutCtrl: 0x%x"
#define BRW_CONTROL_4139_112_2_18_2_6_57_451 "BRW FreeMediaObjStruct media_obj_info_ptr = 0x%x"
#define BRW_CONTROL_4264_112_2_18_2_6_58_452 "BRW HandleMsgCb,id:%d,result:%d,handler:%x"
#define BRW_CONTROL_4290_112_2_18_2_6_58_453 "BRW HandleMsgCb,MMIPDP_ACTIVE_CNF,nsapi:%d"
#define BRW_CONTROL_4322_112_2_18_2_6_58_454 "[BRW] ActiveNetwork is actived"
#define BRW_CONTROL_4329_112_2_18_2_6_58_455 "[BRW]W ActiveNetwork brw_info_ptr PNULL"
#define BRW_CONTROL_4345_112_2_18_2_6_58_456 "[BRW]W ActiveNetwork apn_ptr =0x%x, dualz_sys =%d"
#define BRW_CONTROL_4374_112_2_18_2_6_58_457 "[BRW] DeactiveNetwork brw_info_ptr is null"
#define BRW_CONTROL_4415_112_2_18_2_6_58_458 "[BRW]ProcNetLinkCnf,result=%d"
#define BRW_CONTROL_4429_112_2_18_2_6_58_459 "BRW ProcNetLinkCnf:brw_info_ptr null"
#define BRW_CONTROL_4496_112_2_18_2_6_58_460 "[BRW]ProcNetLinkDisConnInd,brw_info_ptr:0x%x"
#define BRW_CONTROL_4695_112_2_18_2_6_58_461 "[BRW]W BrwProcHttpErr,brw_info_ptr=0x%x"
#define BRW_CONTROL_4703_112_2_18_2_6_58_462 "[BRW]BrwProcHttpErr,state = 0x%x"
#define BRW_CONTROL_4715_112_2_18_2_6_59_463 "[BRW]BrwProcHttpErr retry"
#define BRW_CONTROL_4770_112_2_18_2_6_59_464 "[BRW]BrwProcHttpErr retry"
#define BRW_CONTROL_4785_112_2_18_2_6_59_465 "[BRW]BrwProcHttpErr:media_obj_ptr PNULL"
#define BRW_CONTROL_4805_112_2_18_2_6_59_466 "[BRW]W BrwCancelMediaReq,brw_info_ptr=0x%x,wap_req_ptr=0x%x"
#define BRW_CONTROL_4834_112_2_18_2_6_59_467 "[BRW]W BrwProcHttpAuth,brw_info_ptr=0x%x"
#define BRW_CONTROL_4900_112_2_18_2_6_59_468 "BrwFileCopyForWspDl brw_info_ptr=0"
#define BRW_CONTROL_4905_112_2_18_2_6_59_469 "BrwFileCopyForWspDl copy err"
#define BRW_CONTROL_4929_112_2_18_2_6_59_470 "BrwSetWspDlFileNm err, dl_file_name_ptr=0"
#define BRW_CONTROL_4940_112_2_18_2_6_59_471 "BrwSetWspDlFileNm brw_info_ptr=0"
#define BRW_CONTROL_4947_112_2_18_2_6_59_472 "[BRW]BrwSetWspDlFileNm,:transfer_encoding = %d"
#define BRW_CONTROL_4951_112_2_18_2_6_59_473 "[BRW]BrwSetWspDlFileNm,:is_gzip = %d"
#define BRW_CONTROL_4984_112_2_18_2_6_59_474 "[BRW]W BrwEnterFileNmCbf:brw_info_ptr=0x%x,wap_req_ptr=0x%x"
#define BRW_CONTROL_4996_112_2_18_2_6_59_475 "[BRW]BrwEnterFileNmCbf,file_name_len=%d"
#define BRW_CONTROL_5006_112_2_18_2_6_59_476 "[BRW]BrwSetWspDlFileNm,:transfer_encoding = %d"
#define BRW_CONTROL_5033_112_2_18_2_6_59_477 "[BRW]BrwEnterFileNmCbf,BrwSetHttpDLFileNm failed"
#define BRW_CONTROL_5045_112_2_18_2_6_59_478 "[BRW]BrwEnterFileNmCbf,BrwSetWspDlFileNm failed"
#define BRW_CONTROL_5067_112_2_18_2_6_59_479 "[BRW]W BrwEnterFileNmCbf:brw_info_ptr=0x%x,wap_req_ptr=0x%x"
#define BRW_CONTROL_5073_112_2_18_2_6_59_480 "[BRW]BrwEnterFileNmCbf,file_name_len=%d"
#define BRW_CONTROL_5117_112_2_18_2_6_59_481 "[BRW]BrwGetCachePath:result =%d"
#define BRW_CONTROL_5155_112_2_18_2_6_59_482 "[BRW]BrwGetCachePath:result =%d"
#define BRW_CONTROL_5180_112_2_18_2_6_59_483 "[BRW]BrwGetCookiePath:result =%d"
#define BRW_CONTROL_5195_112_2_18_2_6_59_484 "BRW BRW_ClearCookie instance =0"
#define BRW_CONTROL_5201_112_2_18_2_6_59_485 "BRW BRW_ClearCookie brw_info_ptr =0"
#define BRW_CONTROL_5261_112_2_18_2_7_0_486 "[BRW]BRW_ClearAllCache result_1 = %d"
#define BRW_CONTROL_5263_112_2_18_2_7_0_487 "[BRW]BRW_ClearAllCache result_2 = %d"
#define BRW_CONTROL_5283_112_2_18_2_7_0_488 "[BRW] BRW_ClearAllCookie result_1 = %d"
#define BRW_CONTROL_5285_112_2_18_2_7_0_489 "[BRW] BRW_ClearAllCookie result_2 = %d"
#define BRW_CONTROL_5307_112_2_18_2_7_0_490 "[BRW]W BrwOMADLConfirmCbf:brw_info_ptr=0x%x,wap_req_ptr=0x%x"
#define BRW_CONTROL_5313_112_2_18_2_7_0_491 "[BRW]BrwOMADLConfirmCbf,result =%d, file_name_ptr=0x%x"
#define BRW_CONTROL_5386_112_2_18_2_7_0_492 "[BRW]W BrwProcSSLCert,brw_info_ptr=0x%x"
#define BRW_CONTROL_5411_112_2_18_2_7_0_493 "BRW_SetScreenProfile:brw_info_ptr null"
#define BRW_CONTROL_5471_112_2_18_2_7_0_494 "BRW_SetScreenProfile:brw_info_ptr null"
#define BRW_CONTROL_5637_112_2_18_2_7_0_495 "GetCurDisplayLayout:brw_info_ptr null"
#define BRW_CONTROL_5644_112_2_18_2_7_0_496 "GetCurDisplayLayout,wap_request_ptr null"
#define BRW_CONTROL_5648_112_2_18_2_7_0_497 "GetCurDisplayLayout,get snapshot"
#define BRW_CONTROL_5836_112_2_18_2_7_1_498 "BrwFileCopyForWspDl brw_info_ptr=0"
#define BRW_CONTROL_5841_112_2_18_2_7_1_499 "[BRW]BrwDecodeFileForDl:decode_result = %d"
#define BRW_CONTROL_5933_112_2_18_2_7_1_500 "[BRW]W SetPublicProfile:malloc proxy addr failed,len=%d"
#define BRW_CONTROL_5956_112_2_18_2_7_1_501 "[BRW]W SetPublicProfile:malloc ua_prof failed,len=%d"
#define BRW_CONTROL_5984_112_2_18_2_7_1_502 "[BRW]W SetPublicProfile:malloc ua_prof failed,len=%d"
#define BRW_CONTROL_6172_112_2_18_2_7_1_503 "[BRW]StopAndFreeBGsound"
#define BRW_CONTROL_6176_112_2_18_2_7_1_504 "StopAndFreeBGsound,layout_handle=0x%x"
#define BRW_CONTROL_6197_112_2_18_2_7_1_505 "StopAndFreeBGsound,wap_request_ptr null"
#define BRW_CONTROL_6201_112_2_18_2_7_1_506 "StopAndFreeBGsound,get snapshot"
#define BRW_CONTROL_6212_112_2_18_2_7_1_507 "StopAndFreeBGsound,cur_layout_handle=0x%x"
#define BRW_CORE_ADAPTER_221_112_2_18_2_7_2_508 "Brw: timer=%dms start id=%d\n"
#define BRW_CORE_ADAPTER_244_112_2_18_2_7_2_509 "Brw: timer id=%d\n"
#define BRW_CORE_ADAPTER_323_112_2_18_2_7_2_510 "APP_Task Brw confirm WaitUserConfirm : begin init, time = %d"
#define BRW_CORE_ADAPTER_431_112_2_18_2_7_3_511 "HandleWmlInputBoxWinMsg ::PNULL ==input_ptr"
#define BRW_CORE_ADAPTER_453_112_2_18_2_7_3_512 "HandleWmlInputBoxWinMsg ::PNULL ==input_ptr"
#define BRW_CORE_ADAPTER_763_112_2_18_2_7_3_513 "BrwGraphicsDrawHLine ::PNULL ==region_ptr"
#define BRW_CORE_ADAPTER_808_112_2_18_2_7_3_514 "BrwGraphicsDrawVLine ::PNULL ==region_ptr"
#define BRW_CORE_ADAPTER_956_112_2_18_2_7_4_515 "BrwGraphicsDrawLine ::PNULL ==region_ptr"
#define BRW_CORE_ADAPTER_1221_112_2_18_2_7_4_516 "BrwGraphicsFillRect ::PNULL ==region_ptr"
#define BRW_CORE_ADAPTER_1269_112_2_18_2_7_4_517 "BrwFillLcdRect ::PNULL ==rect_ptr"
#define BRW_CORE_ADAPTER_1363_112_2_18_2_7_4_518 "BrwDisplayDownloadingImage ::PNULL ==unicode_ptr"
#define BRW_CORE_ADAPTER_1501_112_2_18_2_7_5_519 "BrwDisplayDownloadingImage ::PNULL ==rect_ptr"
#define BRW_CORE_ADAPTER_1560_112_2_18_2_7_5_520 "[BRW]BrwDisplayFailedImage"
#define BRW_CORE_ADAPTER_1565_112_2_18_2_7_5_521 "BrwDisplayFailedImage ::PNULL ==region_ptr"
#define BRW_CORE_ADAPTER_1690_112_2_18_2_7_5_522 "[BRW]BrwImageCalc:ret = %d, width =%d,height =%d"
#define BRW_CORE_ADAPTER_1717_112_2_18_2_7_5_523 "[BRW]W BrwImageDisplayEx input invalid!"
#define BRW_CORE_ADAPTER_1725_112_2_18_2_7_5_524 "[BRW]W BrwImageDisplayEx:target_width =%d, target_height =%d"
#define BRW_CORE_ADAPTER_1756_112_2_18_2_7_5_525 "[BRW]W BrwImageDisplayEx:info_ptr->mime_type =%d"
#define BRW_CORE_ADAPTER_1773_112_2_18_2_7_5_526 "[BRW]W BrwImageDisplayEx:alloc fail, in_param.target_buf_size =%d"
#define BRW_CORE_ADAPTER_1789_112_2_18_2_7_5_527 "[BRW]BrwImageDisplayEx:is_need_zoom_in = %d"
#define BRW_CORE_ADAPTER_1820_112_2_18_2_7_5_528 "[BRW]W BrwImageDisplayEx:ZOOM Alloc fail,zoom_in_data.dest_buf_size = %d"
#define BRW_CORE_ADAPTER_1850_112_2_18_2_7_5_529 "[BRW]W BrwImageDisplayEx:IMG_DEC_GetFrame  decode_ret= %d"
#define BRW_CORE_ADAPTER_1855_112_2_18_2_7_5_530 "[BRW]W BrwImageDisplayEx:IMG_DEC_Create decode_ret =%d"
#define BRW_CORE_ADAPTER_1861_112_2_18_2_7_5_531 "[BRW]W BrwImageDisplayEx:IMG_DEC_Destroy decode_ret =%d"
#define BRW_CORE_ADAPTER_1934_112_2_18_2_7_6_532 "[BRW]BrwImageDisplay:result = %d"
#define BRW_CORE_ADAPTER_1964_112_2_18_2_7_6_533 "WmlPreSelectListControl ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_1992_112_2_18_2_7_6_534 "SetupWmlListControl ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_2063_112_2_18_2_7_6_535 "WmlHandleSelectedFinish ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_2073_112_2_18_2_7_6_536 "WmlHandleSelectedFinish ::PNULL ==item_ptr"
#define BRW_CORE_ADAPTER_2129_112_2_18_2_7_6_537 "WmlHandleSelectWinMsg ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_2160_112_2_18_2_7_6_538 "WmlHandleSelectWinMsg ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_2180_112_2_18_2_7_6_539 "WmlHandleSelectWinMsg ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_2213_112_2_18_2_7_6_540 "XhtmlPreSelectListControl ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_2240_112_2_18_2_7_6_541 "SetupXhtmlListControl ::PNULL ==layout_ptr"
#define BRW_CORE_ADAPTER_2264_112_2_18_2_7_6_542 "SetupXhtmlListControl ::PNULL ==option_ptr"
#define BRW_CORE_ADAPTER_2314_112_2_18_2_7_6_543 "XhtmlHandleSelectWinMsg ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_2324_112_2_18_2_7_6_544 "XhtmlHandleSelectWinMsg ::PNULL ==item_ptr"
#define BRW_CORE_ADAPTER_2372_112_2_18_2_7_6_545 "[BRW]W XhtmlHandleSelectWinMsg ::PNULL==select_win_ptr"
#define BRW_CORE_ADAPTER_2388_112_2_18_2_7_7_546 "XhtmlHandleSelectWinMsg ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_2425_112_2_18_2_7_7_547 "XhtmlHandleSelectWinMsg ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_2451_112_2_18_2_7_7_548 "XhtmlHandleSelectWinMsg ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_2484_112_2_18_2_7_7_549 "ReadInputFormat ::PNULL ==method ||PNULL == input_mode_ptr"
#define BRW_CORE_ADAPTER_2547_112_2_18_2_7_7_550 "HandleWmlInputBoxWinMsg ::PNULL ==input_ptr"
#define BRW_CORE_ADAPTER_2600_112_2_18_2_7_7_551 "HandleWmlInputBoxWinMsg :MSG_CTL_OK:PNULL ==input_ptr"
#define BRW_CORE_ADAPTER_2638_112_2_18_2_7_7_552 "[BRW]W HandleXhtmlInputBoxWinMsg ::PNULL ==input_win_ptr"
#define BRW_CORE_ADAPTER_2683_112_2_18_2_7_7_553 "HandleXhtmlInputBoxWinMsg :MSG_CTL_OK:PNULL ==input_win_ptr"
#define BRW_CORE_ADAPTER_2751_112_2_18_2_7_7_554 "WmlLayoutCreateEditWindow ::PNULL ==input_ptr"
#define BRW_CORE_ADAPTER_2846_112_2_18_2_7_7_555 "[BRW]W XhtmlLayoutCreateEditWindow:param error, layout_ptr=0x%x"
#define BRW_CORE_ADAPTER_2852_112_2_18_2_7_7_556 "[BRW]W XhtmlLayoutCreateEditWindow ::input_win_ptr ALLOC FAIL"
#define BRW_CORE_ADAPTER_2878_112_2_18_2_7_7_557 "[BRW]W XhtmlLayoutCreateEditWindow:handle error"
#define BRW_CORE_ADAPTER_2950_112_2_18_2_7_8_558 "XhtmlOpenFormFileWinCallBack array size == 0"
#define BRW_CORE_ADAPTER_2955_112_2_18_2_7_8_559 "XhtmlOpenFormFileWinCallBack failed"
#define BRW_CORE_ADAPTER_3030_112_2_18_2_7_8_560 "XhtmlLayoutCreateEditWindow ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_3115_112_2_18_2_7_8_561 "XhtmlLayoutCreateSelectWindow ::PNULL ==select_ptr"
#define BRW_CORE_ADAPTER_3122_112_2_18_2_7_8_562 "[BRW]W XhtmlLayoutCreateSelectWindow ::select_win_ptr ALLOC FAIL"
#define BRW_CORE_ADAPTER_3139_112_2_18_2_7_8_563 "[BRW]W XhtmlLayoutCreateSelectWindow:handle error"
#define BRW_CORE_ADAPTER_3160_112_2_18_2_7_8_564 "[BRW]W XhtmlLayoutCreateSelectWindow:GUILIST_CreateListBox error"
#define BRW_CORE_ADAPTER_3195_112_2_18_2_7_8_565 "[BRW]W BrwPlayGif input invalid!"
#define BRW_CORE_ADAPTER_3203_112_2_18_2_7_8_566 "[BRW]W BrwPlayGif:target_width =%d, target_height =%d"
#define BRW_CORE_ADAPTER_3217_112_2_18_2_7_8_567 "[BRW]W BrwPlayGif:IMG_DEC_Create fail"
#define BRW_CORE_ADAPTER_3233_112_2_18_2_7_8_568 "[BRW]W BrwPlayGif:target_ptr alloc fail"
#define BRW_CORE_ADAPTER_3262_112_2_18_2_7_8_569 "[BRW]W BrwPlayGif:target_ptr alloc fail2"
#define BRW_CORE_ADAPTER_3277_112_2_18_2_7_8_570 "[BRW]BrwPlayGif:is_need_zoom_in = %d"
#define BRW_CORE_ADAPTER_3308_112_2_18_2_7_8_571 "[BRW]W BrwPlayGif:ZOOM Alloc fail,zoom_in_data.dest_buf_size = %d"
#define BRW_CORE_ADAPTER_3581_112_2_18_2_7_9_572 "BrwZoomImage zoom_in_ptr || zoom_out_ptr is null"
#define BRW_CORE_ADAPTER_3637_112_2_18_2_7_9_573 "[BRW]BrwZoomImage:zoom result is %d!"
#define BRW_CORE_ADAPTER_3671_112_2_18_2_7_9_574 "[BRW]BrwGetAudioTypeByMime mime_type=%d"
#define BRW_CORE_ADAPTER_3715_112_2_18_2_7_9_575 "[BRW]BrwPlayBgsoundCallBack, result=%d"
#define BRW_CORE_ADAPTER_3732_112_2_18_2_7_9_576 "[BRW]BrwPlayBGsound, loop=%d"
#define BRW_CORE_ADAPTER_3737_112_2_18_2_7_9_577 "[BRW]BrwPlayBGsound, play_times=%d, audio_type=%d"
#define BRW_CORE_ADAPTER_3754_112_2_18_2_7_9_578 "[BRW]BrwPlayBGsound, MMIAPISET_IsPermitPlayRing not!"
#define BRW_CORE_ADAPTER_3759_112_2_18_2_7_9_579 "[BRW]BrwPlayBGsound:ret=%d"
#define BRW_CORE_ADAPTER_3769_112_2_18_2_7_9_580 "[BRW]BrwStopBGsound"
#define BRW_DLINTERFACE_285_112_2_18_2_7_10_581 "[BRW]W DLGenFileName ::PNULL != file_name"
#define BRW_DLINTERFACE_299_112_2_18_2_7_10_582 "[BRW]W DLGenFileName:(ext_name_len=%d) > (BRW_DL_SUFFIX_MAX_LEN=%d)"
#define BRW_DLINTERFACE_305_112_2_18_2_7_10_583 "[BRW]W DLGenFileName:(ext_name_len=%d),(name_len=%d)"
#define BRW_DLINTERFACE_311_112_2_18_2_7_10_584 "[BRW]DLGenFileName:name_len=%d"
#define BRW_DLINTERFACE_515_112_2_18_2_7_11_585 "GetDownloadFileByURL i=%d, rtn_url_ext=%s"
#define BRW_DLINTERFACE_640_112_2_18_2_7_11_586 "[BRW]GetMimeTypeByhttpHeader mime_type =%d"
#define BRW_DLINTERFACE_660_112_2_18_2_7_11_587 "[BRW]W Brw_DLGetDefaultFileName:PNULL == file_name"
#define BRW_DLINTERFACE_666_112_2_18_2_7_11_588 "[BRW]Brw_DLGetDefaultFileName GetFileNameByContentDispositon TURE"
#define BRW_DLINTERFACE_674_112_2_18_2_7_11_589 "[BRW]Brw_DLGetDefaultFileName type =%d"
#define BRW_DLINTERFACE_689_112_2_18_2_7_11_590 "[BRW]Brw_DLGetDefaultFileName default_file_wname_ptr ALLOC Fail"
#define BRW_DLINTERFACE_694_112_2_18_2_7_11_591 "[BRW]Brw_DLGetDefaultFileName url_file_name_len err"
#define BRW_DLINTERFACE_752_112_2_18_2_7_11_592 "[BRW]BrwDLGetMimeTypeBySuffix mime_type = %d"
#define BRW_DLINTERFACE_772_112_2_18_2_7_11_593 "BrwGenerateDlTempName length exceed"
#define BRW_DLINTERFACE_794_112_2_18_2_7_11_594 "[BRW]W BrwStartDL:req_ptr=0x%x,brw_info_ptr=0x%x"
#define BRW_DLINTERFACE_800_112_2_18_2_7_11_595 "[BRW]BrwStartDL,:transfer_encoding = %d"
#define BRW_DLINTERFACE_874_112_2_18_2_7_11_596 "[BRW]BrwStartDL:expired_time = %d"
#define BRW_DLINTERFACE_878_112_2_18_2_7_11_597 "[BRW]BrwStartDL mime_type =%d"
#define BRW_DLINTERFACE_926_112_2_18_2_7_11_598 "[BRW]BrwStartDL req.mime_type =%d"
#define BRW_DLINTERFACE_941_112_2_18_2_7_11_599 "[BRW]W--BrwCancelDL:req_ptr=0x%x,brw_info_ptr=0x%x"
#define BRW_DLINTERFACE_956_112_2_18_2_7_11_600 "[BRW]W--BrwFreeDLResource:req_ptr=0x%x,brw_info_ptr=0x%x"
#define BRW_DLINTERFACE_968_112_2_18_2_7_11_601 "BrwFreeDLResource:isn't downloading state"
#define BRW_DLINTERFACE_990_112_2_18_2_7_11_602 "[BRW]W BrwProcDLSuccess:req_ptr=0x%x,brw_info_ptr=0x%x"
#define BRW_DLINTERFACE_1019_112_2_18_2_7_12_603 "[BRW]W BrwWspDownloadDataIn, invalid input,wap_req_ptr=%d,http_req_ptr=%d"
#define BRW_DLINTERFACE_1026_112_2_18_2_7_12_604 "[BRW]W BrwWspDownloadDataIn ERR:ffs_handler=%d,buf_ptr=0x%x"
#define BRW_DLINTERFACE_1055_112_2_18_2_7_12_605 "[BRW]W GetDrmSDExpiredTime http_header_ptr PNULL"
#define BRW_DLINTERFACE_1061_112_2_18_2_7_12_606 "[BRW]GetDrmSDExpiredTime dest_str1=%s"
#define BRW_DLINTERFACE_1080_112_2_18_2_7_12_607 "[BRW]W GetDrmSDExpiredTime ALLOC PNULL"
#define BRW_DLINTERFACE_1085_112_2_18_2_7_12_608 "[BRW]GetDrmSDExpiredTime expired_time=%d"
#define BRW_DLINTERFACE_1114_112_2_18_2_7_12_609 "[BRW]W BrwStartOMADL:req_ptr=0x%x,brw_info_ptr=0x%x"
#define BRW_DLINTERFACE_1125_112_2_18_2_7_12_610 "[BRW]W MMIDL_StartOMADownload file_name_ptr PNULL or file_name_len =%d"
#define BRW_DLINTERFACE_1172_112_2_18_2_7_12_611 "[BRW]W MMIDL_StartOMADownload ODM_StartDownload Failed oma_ret = %d"
#define BRW_DLINTERFACE_1201_112_2_18_2_7_12_612 "[BRW]W--BrwOMADLResp:wap_req_ptr=0x%x,brw_info_ptr=0x%x"
#define BRW_DLINTERFACE_1234_112_2_18_2_7_12_613 "[BRW]W BrwOMADLAuthResp:wap_req_ptr=0x%x,brw_info_ptr=0x%x"
#define BRW_DLINTERFACE_1238_112_2_18_2_7_12_614 "[BRW]BrwOMADLAuthResp:result = %d"
#define BRW_DLINTERFACE_1290_112_2_18_2_7_12_615 "[BRW]BrwOMADLAuthResp:cnf_ptr->result = %d"
#define BRW_DLINTERFACE_1404_112_2_18_2_7_12_616 "[BRW] HandleODMDlConfirmationInd mime_type[%d] =%d"
#define BRW_DLINTERFACE_1445_112_2_18_2_7_12_617 "[BRW] HandleODMDlConfirmationInd mime_type =%d"
#define BRW_DLINTERFACE_1447_112_2_18_2_7_12_618 "[BRW] HandleODMDlConfirmationInd drm_content_mime_type=%d"
#define BRW_DLINTERFACE_1481_112_2_18_2_7_12_619 "[BRW]W HandleODMDlConfirmationInd:session_id = 0x%x"
#define BRW_DLINTERFACE_1535_112_2_18_2_7_13_620 "[BRW]W HandleODMDlAuthInd:session_id = 0x%x"
#define BRW_DLINTERFACE_1559_112_2_18_2_7_13_621 "[BRW]HandleODMDlProgressInd:stage =%d"
#define BRW_DLINTERFACE_1596_112_2_18_2_7_13_622 "[BRW]W HandleODMDlProgressInd:session_id = 0x%x"
#define BRW_DLINTERFACE_1656_112_2_18_2_7_13_623 "[BRW]W HandleODMDlSuccessInd:session_id = 0x%x"
#define BRW_DLINTERFACE_1681_112_2_18_2_7_13_624 "HandleODMDlErrorInd error_no =%d"
#define BRW_DLINTERFACE_1700_112_2_18_2_7_13_625 "HandleODMDlErrorInd result =%d"
#define BRW_DLINTERFACE_1739_112_2_18_2_7_13_626 "[BRW]HandleODMDlHeaderInd:time_out = %d"
#define BRW_DLINTERFACE_1854_112_2_18_2_7_13_627 "[BRW]W BrwProcODMMsg param is PNULL msg_id = 0x%x"
#define BRW_DLINTERFACE_1862_112_2_18_2_7_13_628 "[BRW]BrwProcODMMsg receive ODM_START_DOWNLOAD_CNF"
#define BRW_DLINTERFACE_1867_112_2_18_2_7_13_629 "[BRW]BrwProcODMMsg receive ODM_CANCEL_DOWNLOAD_CNF"
#define BRW_DLINTERFACE_1872_112_2_18_2_7_13_630 "[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_CONFIRMATION_IND"
#define BRW_DLINTERFACE_1877_112_2_18_2_7_13_631 "[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_AUTH_IND"
#define BRW_DLINTERFACE_1882_112_2_18_2_7_13_632 "[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_PROGRESS_IND"
#define BRW_DLINTERFACE_1887_112_2_18_2_7_13_633 "[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_SUCCESS_IND"
#define BRW_DLINTERFACE_1892_112_2_18_2_7_13_634 "[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_ERROR_IND"
#define BRW_DLINTERFACE_1897_112_2_18_2_7_13_635 "[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_ERROR_IND"
#define BRW_DLINTERFACE_1903_112_2_18_2_7_13_636 "[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_GET_SESSION_LIST_CNF"
#define BRW_DLINTERFACE_1908_112_2_18_2_7_13_637 "[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_GET_SESSION_INFO_CNF"
#define BRW_DLINTERFACE_1913_112_2_18_2_7_13_638 "[BRW]BrwProcODMMsg receive ODM_START_DOWNLOAD_CNF"
#define BRW_DLINTERFACE_1918_112_2_18_2_7_13_639 "[BRW]BrwProcODMMsg receive ODM_DESUBSCRIBE_INDICATION_CNF"
#define BRW_DLINTERFACE_1923_112_2_18_2_7_13_640 "[BRW]BrwProcODMMsg receive ODM_MANAGER_PROGRESS_IND"
#define BRW_DLINTERFACE_1928_112_2_18_2_7_13_641 "[BRW]BrwProcODMMsg receive ODM_MANAGER_SUCCESS_IND"
#define BRW_DLINTERFACE_1933_112_2_18_2_7_13_642 "[BRW]BrwProcODMMsg receive ODM_MANAGER_ERROR_IND"
#define BRW_DLINTERFACE_1939_112_2_18_2_7_13_643 "[BRW]BrwProcODMMsg Not handle msg_id = 0x%x"
#define BRW_DLINTERFACE_1993_112_2_18_2_7_13_644 "[BRW]W BrwSetDlTempFilename err, alloc fail"
#define BRW_DLINTERFACE_2015_112_2_18_2_7_13_645 "[BRW]BrwDelDlTempFile:result = %d"
#define BRW_HISTORY_62_112_2_18_2_7_14_646 "BrwNavHisAddCache: cathed request error"
#define BRW_HISTORY_68_112_2_18_2_7_14_647 "BrwNavHisAddCache: error 2"
#define BRW_HISTORY_92_112_2_18_2_7_14_648 "brw BrwNavHisAddCache, free cathe"
#define BRW_HISTORY_124_112_2_18_2_7_14_649 "brw BrwNavClearCache free cathe"
#define BRW_HISTORY_183_112_2_18_2_7_14_650 "[BRW]NavHisFreeCurCacheReq,BrwDestroyLayoutCtrl: 0x%x"
#define BRW_HISTORY_186_112_2_18_2_7_14_651 "brw free wap request cathe:%x"
#define BRW_HISTORY_224_112_2_18_2_7_15_652 "BrwVisitHisAddItem:url len=%d"
#define BRW_HISTORY_230_112_2_18_2_7_15_653 "[BRW]BrwVisitHisAddItem:s_brw_visit_his_que_ptr null"
#define BRW_HISTORY_284_112_2_18_2_7_15_654 "[BRW]BRW_VisitHisDelItem:index =%d"
#define BRW_HISTORY_288_112_2_18_2_7_15_655 "[BRW]BRW_VisitHisDelItem ERROR:index =%d"
#define BRW_HISTORY_314_112_2_18_2_7_15_656 "[BRW]BRW_VisitHisDel:s_brw_visit_his_que_ptr null"
#define BRW_HISTORY_364_112_2_18_2_7_15_657 "BRW_VisitHisInit:read_len =%d"
#define BRW_HISTORY_397_112_2_18_2_7_15_658 "[BRW]W BRW_VisitHisGenIndexArr:index_arr_ptr=0x%x"
#define BRW_HISTORY_454_112_2_18_2_7_15_659 "[BRW]W BRW_VisitHisGetTotal:error"
#define BRW_HISTORY_476_112_2_18_2_7_15_660 "BRW]W BRW_VisitHisGetHisInfoByIndex:index=%d"
#define BRW_HISTORY_497_112_2_18_2_7_15_661 "brw FreeNavHisList free cathe"
#define BRW_HISTORY_548_112_2_18_2_7_15_662 "BrwNavHisAddItem,%s"
#define BRW_HISTORY_555_112_2_18_2_7_15_663 "[BRW]BrwNavHisAddItem:url_len=%d"
#define BRW_HISTORY_569_112_2_18_2_7_15_664 "[BRW]BrwNavHisAddItem:malloc failed"
#define BRW_HISTORY_595_112_2_18_2_7_16_665 "[BRW]BrwNavHisAddItem:malloc failed"
#define BRW_HISTORY_685_112_2_18_2_7_16_666 "BrwNavHisForward,total num:%d,cur:%s"
#define BRW_HISTORY_699_112_2_18_2_7_16_667 "BrwNavHisBackward,total num:%d,cur:%s"
#define BRW_HISTORY_799_112_2_18_2_7_16_668 "[BRW]BRWUtil_ReadFileSyn:device null"
#define BRW_HISTORY_819_112_2_18_2_7_16_669 "[BRW]BRW_ClearVisitHis result =%d"
#define BRW_HTTPINTERFACE_200_112_2_18_2_7_17_670 "BrwInitHttpSession"
#define BRW_HTTPINTERFACE_231_112_2_18_2_7_17_671 "BRW PASSWORD =%s"
#define BRW_HTTPINTERFACE_233_112_2_18_2_7_17_672 "BRW user name =%s"
#define BRW_HTTPINTERFACE_292_112_2_18_2_7_17_673 "BrwCloseHttpSession"
#define BRW_HTTPINTERFACE_303_112_2_18_2_7_17_674 "[BRW]BrwCloseHttpSession: unsupported protocol"
#define BRW_HTTPINTERFACE_353_112_2_18_2_7_17_675 "[BRW]BrwProcErrInd"
#define BRW_HTTPINTERFACE_365_112_2_18_2_7_17_676 "[BRW]BrwProcErrInd http_req_ptr null"
#define BRW_HTTPINTERFACE_394_112_2_18_2_7_17_677 "[BRW]W ProcRequestIDInd http_req_ptr null"
#define BRW_HTTPINTERFACE_400_112_2_18_2_7_17_678 "[BRW]ProcRequestIDInd request_id=0x%x, context_id=0x%x, app_instance=0x%x"
#define BRW_HTTPINTERFACE_411_112_2_18_2_7_17_679 "[BRW]ProcRequestIDInd profile.protocol =%d"
#define BRW_HTTPINTERFACE_415_112_2_18_2_7_17_680 "[BRWW ProcRequestIDInd brw_info_ptr null"
#define BRW_HTTPINTERFACE_438_112_2_18_2_7_17_681 "[BRW]ProcNetProgressInd http_req_ptr null"
#define BRW_HTTPINTERFACE_476_112_2_18_2_7_17_682 "[BRW]ProcRedirectInd http_req_ptr null"
#define BRW_HTTPINTERFACE_518_112_2_18_2_7_17_683 "[BRW]ProcNeedAuthInd"
#define BRW_HTTPINTERFACE_539_112_2_18_2_7_17_684 "[BRW]ProcNeedAuthInd http_req_ptr null"
#define BRW_HTTPINTERFACE_673_112_2_18_2_7_18_685 "[BRW]W BrwSendHttpRequest:file_name_len=%d"
#define BRW_HTTPINTERFACE_683_112_2_18_2_7_18_686 "SetPostRequestParam,buf:%x,len:%d"
#define BRW_HTTPINTERFACE_725_112_2_18_2_7_18_687 "[BRW]W SetPostRequestParam:file_name_len=%d"
#define BRW_HTTPINTERFACE_770_112_2_18_2_7_18_688 "[BRW]W BrwSendHttpRequest:param =0x%x!"
#define BRW_HTTPINTERFACE_780_112_2_18_2_7_18_689 "[BRW]BrwSendHttpRequest:BRWList_AppendObject req=0x%x!"
#define BRW_HTTPINTERFACE_784_112_2_18_2_7_18_690 "BrwSendHttpRequest:BRWList_AppendObject failed!"
#define BRW_HTTPINTERFACE_850_112_2_18_2_7_18_691 "[BRW]W BrwSendHttpRequest:unsupport method=%d!"
#define BRW_HTTPINTERFACE_858_112_2_18_2_7_18_692 "BrwSendHttpRequest:malloc req failed!"
#define BRW_HTTPINTERFACE_895_112_2_18_2_7_18_693 "BrwFreeHttpRequest:req null"
#define BRW_HTTPINTERFACE_911_112_2_18_2_7_18_694 "BrwFreeHttpRequest:req=0x%x"
#define BRW_HTTPINTERFACE_942_112_2_18_2_7_18_695 "[BRW] req_id:%d"
#define BRW_HTTPINTERFACE_953_112_2_18_2_7_18_696 "[BRW]wap_req"
#define BRW_HTTPINTERFACE_958_112_2_18_2_7_18_697 "[BRW]media"
#define BRW_HTTPINTERFACE_969_112_2_18_2_7_18_698 "ProcHeaderInd ind->rsp_header_info.response_code:%d"
#define BRW_HTTPINTERFACE_975_112_2_18_2_7_18_699 "ProcDataInd:wap_req_ptr null"
#define BRW_HTTPINTERFACE_985_112_2_18_2_7_18_700 "[BRW]ProcHeaderInd content_type_ptr = %s"
#define BRW_HTTPINTERFACE_988_112_2_18_2_7_18_701 "[BRW]ProcHeaderInd mime_type =%d"
#define BRW_HTTPINTERFACE_1021_112_2_18_2_7_18_702 "[BRW]ProcHeaderInd content_length =%d"
#define BRW_HTTPINTERFACE_1030_112_2_18_2_7_18_703 "[BRW]ProcHeaderInd mime_type(after) =%d, response_code= %d"
#define BRW_HTTPINTERFACE_1049_112_2_18_2_7_18_704 "[BRW]malloc charset failed"
#define BRW_HTTPINTERFACE_1131_112_2_18_2_7_19_705 "[BRW]ProcDataInd:http_req_ptr null"
#define BRW_HTTPINTERFACE_1137_112_2_18_2_7_19_706 "ProcDataInd:brw_info_ptr =0x%x"
#define BRW_HTTPINTERFACE_1144_112_2_18_2_7_19_707 "ProcDataInd:wap_req_ptr null"
#define BRW_HTTPINTERFACE_1172_112_2_18_2_7_19_708 "[BRW]ProcDataInd,now_len=%d,data_len=%d,max_len=%d"
#define BRW_HTTPINTERFACE_1237_112_2_18_2_7_19_709 "ProcGetCnf:brw_info_ptr =0x%x,http_req_ptr:%x"
#define BRW_HTTPINTERFACE_1267_112_2_18_2_7_19_710 "ProcGetCnf:brw_info_ptr =0x%x"
#define BRW_HTTPINTERFACE_1271_112_2_18_2_7_19_711 "brw ProcPostCnf cnf->result=%d"
#define BRW_HTTPINTERFACE_1464_112_2_18_2_7_19_712 "[BRW]W SendAuthRsp: unsupported protocol"
#define BRW_HTTPINTERFACE_1492_112_2_18_2_7_19_713 "[BRW]BrwHttpAuthResp:Cancel state =%d"
#define BRW_HTTPINTERFACE_1524_112_2_18_2_7_19_714 "[BRW]W BrwHttpAuthResp:brw_info_ptr null"
#define BRW_HTTPINTERFACE_1529_112_2_18_2_7_19_715 "[BRW]W BrwHttpAuthResp:http_req_ptr null"
#define BRW_HTTPINTERFACE_1560_112_2_18_2_7_20_716 "BrwClearCache error: unsupported protocol"
#define BRW_HTTPINTERFACE_1595_112_2_18_2_7_20_717 "BrwClearCookie error: unsupported protocol"
#define BRW_HTTPINTERFACE_1630_112_2_18_2_7_20_718 "[BRW]ProcSSLCertInd"
#define BRW_HTTPINTERFACE_1644_112_2_18_2_7_20_719 "[BRW]ProcSSLCertInd http_req_ptr null"
#define BRW_HTTPINTERFACE_1685_112_2_18_2_7_20_720 "[BRW]BrwHttpSSLCertResp: invalid protocol"
#define BRW_HTTPINTERFACE_1697_112_2_18_2_7_20_721 "[BRW]BrwHttpSSLCertResp:brw_info_ptr null"
#define BRW_HTTPINTERFACE_1716_112_2_18_2_7_20_722 "[BRW]W ProcHttpErr http err=%d"
#define BRW_HTTPINTERFACE_1776_112_2_18_2_7_20_723 "BRW ProcInitSessionCnf, the session inited has been destroyed!"
#define BRW_HTTPINTERFACE_1792_112_2_18_2_7_20_724 "BrwProcHttpMsg HTTP_SIG_INIT_CNF,context_id:%x"
#define BRW_HTTPINTERFACE_1807_112_2_18_2_7_20_725 "BrwProcHttpMsg HTTP_SIG_CLOSE_CNF"
#define BRW_HTTPINTERFACE_1828_112_2_18_2_7_20_726 "BrwProcHttpMsg HTTP_SIG_CANCEL_CNF"
#define BRW_HTTPINTERFACE_1843_112_2_18_2_7_20_727 "BrwProcHttpMsg HTTP_SIG_DATA_IND"
#define BRW_HTTPINTERFACE_1858_112_2_18_2_7_20_728 "BrwProcHttpMsg HTTP_SIG_ERROR_IND"
#define BRW_HTTPINTERFACE_1869_112_2_18_2_7_20_729 "BrwProcHttpMsg HTTP_SIG_GET_CNF,context_id:%x"
#define BRW_HTTPINTERFACE_1874_112_2_18_2_7_20_730 "BrwProcHttpMsg HTTP_SIG_GET_CNF"
#define BRW_HTTPINTERFACE_1889_112_2_18_2_7_20_731 "BrwProcHttpMsg HTTP_SIG_HEADER_IND"
#define BRW_HTTPINTERFACE_1904_112_2_18_2_7_20_732 "BrwProcHttpMsg HTTP_SIG_NEED_AUTH_IND"
#define BRW_HTTPINTERFACE_1936_112_2_18_2_7_20_733 "BrwProcHttpMsg HTTP_SIG_POST_CNF"
#define BRW_HTTPINTERFACE_1951_112_2_18_2_7_20_734 "BrwProcHttpMsg HTTP_SIG_REDIRECT_IND"
#define BRW_HTTPINTERFACE_1966_112_2_18_2_7_20_735 "BrwProcHttpMsg HTTP_SIG_REQUEST_ID_IND"
#define BRW_HTTPINTERFACE_1974_112_2_18_2_7_20_736 "BrwProcHttpMsg HTTP_SIG_SSL_CERT_UNTRUST_IND"
#define BRW_INPUT_62_112_2_18_2_7_21_737 "BRW_InputHisInit: data error i =%d,his_num=%d"
#define BRW_INPUT_65_112_2_18_2_7_21_738 "BRW_InputHisInit:read_len =%d"
#define BRW_INPUT_114_112_2_18_2_7_21_739 "[BRW]BRWUtil_ReadFileSyn:device null"
#define BRW_INPUT_134_112_2_18_2_7_21_740 "[BRW]BRW_ClearInputHis result =%d"
#define BRW_INPUT_192_112_2_18_2_7_21_741 "BrwInputHisAddItem:s_brw_input_his_ptr->his_num =%d"
#define BRW_INPUT_264_112_2_18_2_7_21_742 "BRW_GetMatchInputHisUrl:index =%d, his_num=%d"
#define BRW_PLINTERFACE_86_112_2_18_2_7_22_743 "BRWPL_IsFolderExist,full_path_len=%d"
#define BRW_PLINTERFACE_119_112_2_18_2_7_22_744 "SplitFullPath, err full_path_ptr length err"
#define BRW_PLINTERFACE_137_112_2_18_2_7_22_745 "SplitFullPath, err full_path_ptr path=0"
#define BRW_PLINTERFACE_166_112_2_18_2_7_22_746 "BRWPL_FileCopy err BRWPL_GetFileSize failed!"
#define BRW_PLINTERFACE_171_112_2_18_2_7_22_747 "BRWPL_FileCopy err src_file_size=0!"
#define BRW_PLINTERFACE_174_112_2_18_2_7_22_748 "BRWPL_FileCopy src_file_size=%d"
#define BRW_PLINTERFACE_179_112_2_18_2_7_22_749 "BRWPL_FileCopy err creat src_file_handle fail!"
#define BRW_PLINTERFACE_185_112_2_18_2_7_22_750 "BRWPL_FileCopy err creat copyed_file_handle fail!"
#define BRW_PLINTERFACE_203_112_2_18_2_7_22_751 "BRWPL_FileCopy  err alloc exchange buffer failed!"
#define BRW_PLINTERFACE_208_112_2_18_2_7_22_752 "BRWPL_FileCopy err SFS_ReadFile fail!"
#define BRW_PLINTERFACE_213_112_2_18_2_7_22_753 "BRWPL_FileCopy err read_len=0!"
#define BRW_PLINTERFACE_219_112_2_18_2_7_22_754 "BRWPL_FileCopy err SFS_ReadFile fail!"
#define BRW_PLINTERFACE_260_112_2_18_2_7_22_755 "BRWPL_CreateDir:full_path_len=%d"
#define BRW_PLINTERFACE_276_112_2_18_2_7_22_756 "brw BRWPL_DeleteDir full_path_ptr fail!"
#define BRW_PLINTERFACE_282_112_2_18_2_7_22_757 "brw BRWPL_DeleteDir find_path_ptr fail!"
#define BRW_PLINTERFACE_306_112_2_18_2_7_22_758 "brw BRWPL_DeleteDir BRWPL_DeleteDir fail!"
#define BRW_PLINTERFACE_318_112_2_18_2_7_22_759 "brw BRWPL_DeleteDir temp_path_ptr fail!"
#define BRW_PLINTERFACE_330_112_2_18_2_7_22_760 "brw BRWPL_DeleteDir SFS_DeleteFile fail!"
#define BRW_PLINTERFACE_445_112_2_18_2_7_23_761 "BRWPL_AppendWriteFileSync err:%d"
#define BRW_PLINTERFACE_529_112_2_18_2_7_23_762 "BRWPL_IsFileExist,full_path_len=%d"
#define BRW_PLINTERFACE_575_112_2_18_2_7_23_763 "BRWPL_GetFileSize,full_path_len=%d"
#define BRW_PLINTERFACE_615_112_2_18_2_7_23_764 "BRWPL_ReadFileSyn,full_path_len=%d,buf_ptr=0x%x,buf_max_len=%d,read_len_ptr=0x%x"
#define BRW_PLINTERFACE_635_112_2_18_2_7_23_765 "BRWPL_WriteFileSyn,full_path_len=%d,to_write_len=%d,buf_ptr=0x%x"
#define BRW_PLINTERFACE_658_112_2_18_2_7_23_766 "[BRW]---waning-----"
#define BRW_PLINTERFACE_675_112_2_18_2_7_23_767 "BRW_Memcpy dest=%x,src=%x,size=%x"
#define BRW_PLINTERFACE_688_112_2_18_2_7_23_768 "BRW_Memset:dest=%x,size=%x"
#define BRW_PLINTERFACE_705_112_2_18_2_7_23_769 "BRW_WSTRLEN str null"
#define BRW_PLINTERFACE_726_112_2_18_2_7_23_770 "[BRW]W BRW_WstrCat :dest_ptr =0x%x"
#define BRW_PLINTERFACE_755_112_2_18_2_7_23_771 "BRW_Strlen,string null!"
#define BRW_PLINTERFACE_769_112_2_18_2_7_23_772 "BRW_Strcpy,dst=%x,src=%x"
#define BRW_PLINTERFACE_783_112_2_18_2_7_23_773 "BRW_Strcpy,dst=%x,src=%x"
#define BRW_PLINTERFACE_797_112_2_18_2_7_23_774 "BRW_Strncpy,dst=%x,src=%x"
#define BRW_PLINTERFACE_806_112_2_18_2_7_23_775 "BRW_Strcmp,string=%x,str=%x"
#define BRW_PLINTERFACE_816_112_2_18_2_7_23_776 "BRW_Strcmp,string=%x,str=%x"
#define BRW_PLINTERFACE_826_112_2_18_2_7_23_777 "BRW_Stricmp,string=%x,str=%x"
#define BRW_PLINTERFACE_836_112_2_18_2_7_23_778 "BRW_Strnicmp,string=%x,str=%x"
#define BRW_PLINTERFACE_848_112_2_18_2_7_23_779 "BRW_Strstr,string=%x,str=%x"
#define BRW_PLINTERFACE_879_112_2_18_2_7_23_780 "BRW_Strchr,str=%x,c=%c"
#define BRW_PLINTERFACE_888_112_2_18_2_7_23_781 "BRW_Atoi,invalid param"
#define BRW_PLINTERFACE_901_112_2_18_2_7_23_782 "BRW_TailStricmp,long_str=%x,tail=%x"
#define BRW_PLINTERFACE_921_112_2_18_2_7_23_783 "BRW_TailStricmp,long_str=%x,tail=%x"
#define BRW_PLINTERFACE_954_112_2_18_2_7_24_784 "[BRW]W BRWMem_DeleteMemHandle: delete mem handle fail,err:%x"
#define BRW_PLINTERFACE_978_112_2_18_2_7_24_785 "BRWMem_DeleteMemHandle:Mem Handle has been deleted!"
#define BRW_PLINTERFACE_1007_112_2_18_2_7_24_786 "BRWMem_CreatMemHandle: size=%d"
#define BRW_PLINTERFACE_1045_112_2_18_2_7_24_787 "[BRW]BRWMem_CreatMemHandle:Mem Handle has been initiated!"
#define BRW_PLINTERFACE_1146_112_2_18_2_7_24_788 "BRW MEM leak print start:\n"
#define BRW_PLINTERFACE_1151_112_2_18_2_7_24_789 "%d %s/%d 0x%x"
#define BRW_PLINTERFACE_1156_112_2_18_2_7_24_790 "BRW MEM leak print end:\n"
#define BRW_PLINTERFACE_1217_112_2_18_2_7_24_791 "BRWMem_MallocEx,malloc failed,size:%d"
#define BRW_PLINTERFACE_1306_112_2_18_2_7_24_792 "BRWPL_WstrToStr:dst_ptr null"
#define BRW_PLINTERFACE_1331_112_2_18_2_7_24_793 "[BRW]W BRWPL_WstrStrnCatWithAsciiStr :dest_ptr =0x%x"
#define BRW_PLINTERFACE_1479_112_2_18_2_7_25_794 "[BRW]W BRWPL_GetValidDevSysBrwRootForWrite:dst_path_name_ptr alloc fail"
#define BRW_PLINTERFACE_1493_112_2_18_2_7_25_795 "[BRW]BRWPL_GetValidDevSysBrwRoot result =%d, dev =%d"
#define BRW_SNAPSHOT_272_112_2_18_2_7_25_796 "BrwSnapshotCreatFolder fail"
#define BRW_SNAPSHOT_459_112_2_18_2_7_26_797 "BRW BrwCallbackSaveResult, delete the folder fail when save is failed!"
#define BRW_SNAPSHOT_466_112_2_18_2_7_26_798 "BRW BrwCallbackSaveResult, delete the file fail when save is failed!"
#define BRW_SNAPSHOT_506_112_2_18_2_7_26_799 "[BRW]BRW_SnapshotHandleFfsMsg,msg_id:%d"
#define BRW_SNAPSHOT_541_112_2_18_2_7_26_800 "BRW_SNAPSHOT_FFS_PAGE_SAVE_CNF,is_succ:%d"
#define BRW_SNAPSHOT_571_112_2_18_2_7_26_801 "BRW_SNAPSHOT_FFS_CSS_SAVE_CNF,is_succ:%d"
#define BRW_SNAPSHOT_593_112_2_18_2_7_26_802 "BRW_SNAPSHOT_FFS_IMAGE_SAVE_CNF,is_succ:%d"
#define BRW_SNAPSHOT_604_112_2_18_2_7_26_803 "BRW_SNAPSHOT_FFS_IMAGE_SAVE_ONE_CNF,is_succ:%d"
#define BRW_SNAPSHOT_619_112_2_18_2_7_26_804 "BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF,is_succ:%d"
#define BRW_SNAPSHOT_626_112_2_18_2_7_26_805 "BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF: file_size = %d"
#define BRW_SNAPSHOT_705_112_2_18_2_7_26_806 "BRW_SNAPSHOT_FFS_IMAGE_OPEN_ONE_CNF,is_succ:%d"
#define BRW_SNAPSHOT_777_112_2_18_2_7_26_807 "brw snapshot open: %s"
#define BRW_SNAPSHOT_900_112_2_18_2_7_26_808 "[BRW]BrwParseOfflineWebPage,BrwDestroyLayoutCtrl: 0x%x"
#define BRW_SNAPSHOT_1249_112_2_18_2_7_27_809 "BRW_SaveCurWebPage,BrwGenerateCssInfo fail"
#define BRW_SNAPSHOT_1255_112_2_18_2_7_27_810 "BRW_SaveCurWebPage,BrwGenerateImgInfo fail"
#define BRW_SNAPSHOT_1261_112_2_18_2_7_27_811 "BRW_SaveCurWebPage,BrwSnapshotGenerateAllFileFolder fail"
#define BRW_SNAPSHOT_1267_112_2_18_2_7_27_812 "BRW_SaveCurWebPage,BrwFfsWritePageContent fail"
#define BRW_SNAPSHOT_1294_112_2_18_2_7_27_813 "BrwSnapshotDeleteWebPage, read file head len error"
#define BRW_SNAPSHOT_1299_112_2_18_2_7_27_814 "BrwSnapshotDeleteWebPage, error file_head_len==0"
#define BRW_SNAPSHOT_1305_112_2_18_2_7_27_815 "BrwSnapshotDeleteWebPage, file_head_buf_ptr alloc error"
#define BRW_SNAPSHOT_1311_112_2_18_2_7_27_816 "BrwSnapshotDeleteWebPage, read file head error"
#define BRW_SNAPSHOT_1358_112_2_18_2_7_27_817 "BrwSnapshotDeleteWebPage, no image folder"
#define BRW_SNAPSHOT_1428_112_2_18_2_7_28_818 "[BRW]BrwDestroyOpenInfo,BrwDestroyLayoutCtrl: 0x%x"
#define BRW_SNAPSHOT_1452_112_2_18_2_7_28_819 "BrwGetPageMimeType: type = %d"
#define BRW_SNAPSHOT_1510_112_2_18_2_7_28_820 "BrwGetPageUrl: charset_len = %d"
#define BRW_SNAPSHOT_1516_112_2_18_2_7_28_821 "BrwGetPageUrl: url_len = %d"
#define BRW_SNAPSHOT_1538_112_2_18_2_7_28_822 "BrwGetPageCharset: charset_len = %d"
#define BRW_SNAPSHOT_1589_112_2_18_2_7_28_823 "BRW_SnapshotParseOfflinePage no mem,g_snapshot_open_info.full_file_path_ptr"
#define BRW_SNAPSHOT_1623_112_2_18_2_7_28_824 "BRW_SnapshotOfflineBrowse,ERROR:file size=0"
#define BRW_SNAPSHOT_1631_112_2_18_2_7_28_825 "BRW_SnapshotOfflineBrowse,ERROR:alloc file buffer fail"
#define BRW_SNAPSHOT_1641_112_2_18_2_7_28_826 "BRW_SnapshotOfflineBrowse,ERROR:BRWPL_ReadFileAsync fail"
#define BRW_SNAPSHOT_1692_112_2_18_2_7_28_827 "BRW_SnapshotOfflineBrowse,ERROR:alloc file buffer fail"
#define BRW_SNAPSHOT_1703_112_2_18_2_7_28_828 "BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF: file_size = %d"
#define BRW_SNAPSHOT_2033_112_2_18_2_7_29_829 "BRW_SnapshotRenameFile,rename fail!"
#define BRW_SNAPSHOT_2118_112_2_18_2_7_29_830 "BRW_DisplayPage,brw_info_ptr null"
#define BRW_SNAPSHOT_2131_112_2_18_2_7_29_831 "BRW_GetPageImageUrl error: LAYOUT PNULL"
#define BRW_SNAPSHOT_2178_112_2_18_2_7_29_832 "BRW_DisplayPage,brw_info_ptr null"
#define BRW_SNAPSHOT_2191_112_2_18_2_7_29_833 "BRW_SavePageImage error: LAYOUT PNULL"
#define BRW_SNAPSHOT_2196_112_2_18_2_7_29_834 "BRW_SavePageImage error: BrwGetImgBuf error"
#define BRW_SNAPSHOT_2207_112_2_18_2_7_29_835 "BRW_SavePageImage error: BRWPL_WriteFileAsync error"
#define BRW_UTILITY_53_112_2_18_2_7_30_836 "BRWList_Construct PNULL == list_ptr"
#define BRW_UTILITY_203_112_2_18_2_7_30_837 "[BRW]W BRWList_RemoveObject,list_ptr=%x,object_ptr=%x"
#define BRW_UTILITY_329_112_2_18_2_7_30_838 "[BRW]BRWList_SearchObject list_ptr=%x,object_prt=%x,cmp=%x"
#define BRW_UTILITY_354_112_2_18_2_7_30_839 "[BRW]BRWList_SearchObject list_ptr=%x"
#define BRW_UTILITY_680_112_2_18_2_7_31_840 "[BRW]BRWUtil_GetDefaultPath:device null"
#define BRW_UTILITY_728_112_2_18_2_7_31_841 "[BRW]W BRWUtil_ReadFileSyn:file_name_ptr=0x%x,buf_ptr=0x%x"
#define BRW_UTILITY_734_112_2_18_2_7_31_842 "[BRW]BRWUtil_ReadFileSyn:device null"
#define BRW_UTILITY_784_112_2_18_2_7_31_843 "[BRW]W BRWUtil_ReadFileSyn:file_name_ptr=0x%x,buf_ptr=0x%x"
#define BRW_UTILITY_790_112_2_18_2_7_31_844 "[BRW]BRWUtil_ReadFileSyn:device null"
#define BRW_UTILITY_835_112_2_18_2_7_31_845 "[BRW]W BRWUtil_GetFileSize:file_name_ptr=0x%x,size_ptr=0x%x"
#define BRW_UTILITY_842_112_2_18_2_7_31_846 "[BRW]BRWUtil_GetFileSize:device null"
#define BRW_UTILITY_886_112_2_18_2_7_31_847 "[BRW]W BRWUtil_DelFileSyn:file_name_ptr=0x%x"
#define BRW_UTILITY_892_112_2_18_2_7_31_848 "[BRW]BRWUtil_DelFileSyn:device null"
#define BRW_UTILITY_932_112_2_18_2_7_31_849 "[BRW]BRWUtil_CreateDir:device null"
#define BRW_UTILITY_978_112_2_18_2_7_32_850 "[BRW]BRWUtil_DecodeGzip src_len =0x%x"
#define BRW_UTILITY_995_112_2_18_2_7_32_851 "[BRW]BRWUtil_DecodeGzip src_len =0x%x, dec_info.src_img_size=0x%x"
#define BRW_UTILITY_1083_112_2_18_2_7_32_852 "[BRW]W BRWUtil_DecodeGzipByFile:dest_file_name_ptr=0x%x;src_file_name_ptr=0x%x;"
#define BRW_UTILITY_1100_112_2_18_2_7_32_853 "BRWUtil_DecodeGzipByFile:decode_input.src_size = 0x%x"
#define BRW_UTILITY_1130_112_2_18_2_7_32_854 "BRWUtil_DecodeGzipByFile:decode_result =%d"
#define BRW_UTILITY_1178_112_2_18_2_7_32_855 "[BRW]BRWUtil_DecodeGzipRead:buf =0x%x, size =%d"
#define BRW_UTILITY_1185_112_2_18_2_7_32_856 "[BRW]BRWUtil_DecodeGzipRead:src_ffs_handler =0x%x, data_len =%d, readed =%d"
#define BRW_UTILITY_1193_112_2_18_2_7_32_857 "[BRW]BRWUtil_DecodeGzipRead:result =%d, bytes_readed =%d, to_read_size =%d"
#define BRW_UTILITY_1218_112_2_18_2_7_32_858 "[BRW]BRWUtil_DecodeGzipWrite:result =%d, cnt =%d, bytes_written =%d"
#define BRW_WTAI_62_112_2_18_2_7_32_859 "[BRW]W BrwWPMakeCall:phone_num_len=%d"
#define BRW_WTAI_86_112_2_18_2_7_32_860 "[BRW]W BrwWPSendDTMF:dtmf_len=%d"
#define BRW_WTAI_114_112_2_18_2_7_32_861 "[BRW]W BrwWPMakeCall:phone_num_len=%d"
#define BRW_WTAI_145_112_2_18_2_7_32_862 "[BRW]W BrwWPSendSms: phone_num_len=%d,sms_content_len=%d"
#define BRW_WTAI_151_112_2_18_2_7_33_863 "[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
#define BRW_WTAI_156_112_2_18_2_7_33_864 "[BRW]BrwGetWTAIInfo:err pb sms content length too long!len=%d"
#define BRW_WTAI_189_112_2_18_2_7_33_865 "[BRW]W BrwWPSendSmsTo: phone_num_len=%d,sms_content_len=%d"
#define BRW_WTAI_195_112_2_18_2_7_33_866 "[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
#define BRW_WTAI_200_112_2_18_2_7_33_867 "[BRW]BrwGetWTAIInfo:err pb sms content length too long!len=%d"
#define BRW_WTAI_230_112_2_18_2_7_33_868 "[BRW]W BrwWPSendMms:phone_num_len=%d"
#define BRW_WTAI_235_112_2_18_2_7_33_869 "[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
#define BRW_WTAI_259_112_2_18_2_7_33_870 "[BRW]W BrwWPSendMmsTo:phone_num_len=%d"
#define BRW_WTAI_264_112_2_18_2_7_33_871 "[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
#define BRW_WTAI_288_112_2_18_2_7_33_872 "[BRW]W BrwWPSendMail:phone_num_len=%d"
#define BRW_WTAI_293_112_2_18_2_7_33_873 "[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
#define BRW_WTAI_442_112_2_18_2_7_33_874 "[BRW]W BrwGetWTAIInfo:url_ptr=0x%x"
#define BRW_WTAI_500_112_2_18_2_7_33_875 "[BRW]BrwGetWTAIInfo:err email addr length too long!len=%d"
#define BRW_WTAI_541_112_2_18_2_7_33_876 "[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
#define BRW_WTAI_555_112_2_18_2_7_33_877 "[BRW]BrwGetWTAIInfo:err pb name length too long!len=%d"
#define BRW_WTAI_563_112_2_18_2_7_33_878 "[BRW]BrwGetWTAIInfo:not support"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_BROWSER_TRC)
TRACE_MSG(MMIBROWSER_BOOKMARK_200_112_2_18_2_5_38_0,"MMIBrw RenameFile full_path_len %d new_file_name_len %d !!")
TRACE_MSG(MMIBROWSER_BOOKMARK_708_112_2_18_2_5_39_1,"MMIBRW_UpdateBookmarkTotalNum bookmark_total_num=%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_750_112_2_18_2_5_39_2,"[BRW]MMIBRW_BookmarkInit bookmark_num=%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_755_112_2_18_2_5_39_3,"[BRW]W MMIBRW_BookmarkInit s_browser_default_dev=%d, s_browser_path_len=%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_760_112_2_18_2_5_39_4,"[BRW]W MMIBRW_BookmarkInit ALLOC FAIL")
TRACE_MSG(MMIBROWSER_BOOKMARK_765_112_2_18_2_5_39_5,"[BRW]MMIBRW_BookmarkInit Inited!")
TRACE_MSG(MMIBROWSER_BOOKMARK_776_112_2_18_2_5_39_6,"[BRW]MMIBRW_BookmarkInit Folder is Exist!")
TRACE_MSG(MMIBROWSER_BOOKMARK_822_112_2_18_2_5_39_7,"[BRW]MMIBRW_BookmarkInitAfterStartup bookmark_num=%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_826_112_2_18_2_5_39_8,"[BRW]MMIBRW_BookmarkInitAfterStartup IsFolderExist")
TRACE_MSG(MMIBROWSER_BOOKMARK_837_112_2_18_2_5_39_9,"[BRW]W MMIBRW_BookmarkInitAfterStartup s_browser_default_dev=%d, s_browser_path_len=%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_843_112_2_18_2_5_39_10,"[BRW]W MMIBRW_BookmarkInitAfterStartup s_bookmark_info_ptr PNULL")
TRACE_MSG(MMIBROWSER_BOOKMARK_1153_112_2_18_2_5_40_11,"[BRW]MMIBRW_ReloadBookmarkFileArray num=%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_1169_112_2_18_2_5_40_12,"[BRW]MMIBRW_ReloadBookmarkFileArray folder_num=%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_1243_112_2_18_2_5_40_13,"MMIBRW_BookmarkGotoURL: url_len=%d, url_ptr= %s")
TRACE_MSG(MMIBROWSER_BOOKMARK_1283_112_2_18_2_5_40_14,"[BRW]W MMIBRW_AddBookmark:wstr_ptr PNULL")
TRACE_MSG(MMIBROWSER_BOOKMARK_1322_112_2_18_2_5_40_15,"[BRW]W MMIBRW_AddBookmark:string_dest ALLOC FAIL")
TRACE_MSG(MMIBROWSER_BOOKMARK_1334_112_2_18_2_5_40_16,"[BRW]W MMIBRW_AddBookmark:url_buf ALLOC FAIL")
TRACE_MSG(MMIBROWSER_BOOKMARK_1352_112_2_18_2_5_40_17,"[BRW]W MMIBRW_AddBookmark:write file sfs_error =%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_1404_112_2_18_2_5_40_18,"[BRW]W MMIBRW_AddBookmark:MMIFILEARRAY_Add fail!")
TRACE_MSG(MMIBROWSER_BOOKMARK_1430_112_2_18_2_5_40_19,"[BRW]W MMIBRW_AddBookmarkFolder:in_title_str.wstr_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_BOOKMARK_1468_112_2_18_2_5_40_20,"[BRW]W MMIBRW_AddBookmarkFolder:MMIFILEARRAY_Add fail")
TRACE_MSG(MMIBROWSER_BOOKMARK_1474_112_2_18_2_5_40_21,"[BRW]W MMIBRW_AddBookmarkFolder:MMIFILE_CreateDir fail")
TRACE_MSG(MMIBROWSER_BOOKMARK_1499_112_2_18_2_5_40_22,"[BRW]MMIBRW_ChangeBookmarkTitle:in_title_str.wstr_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_BOOKMARK_1547_112_2_18_2_5_40_23,"[BRW]MMIBRW_ChangeBookmarkTitle:MMIFILEARRAY_Read fail")
TRACE_MSG(MMIBROWSER_BOOKMARK_1567_112_2_18_2_5_40_24,"[BRW]MMIBRW_ChangeBookmarkTitle:MMIFILEARRAY_Modify result =%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_1593_112_2_18_2_5_40_25,"[BRW]W MMIBRW_ChangeBookmarkUrl:in_url_str.wstr_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_BOOKMARK_1598_112_2_18_2_5_40_26,"[BRW]MMIBRW_ChangeBookmarkUrl:in_index = %d")
TRACE_MSG(MMIBROWSER_BOOKMARK_1611_112_2_18_2_5_40_27,"[BRW]W MMIBRW_ChangeBookmarkUrl:dest_wstr_ptr alloc fail")
TRACE_MSG(MMIBROWSER_BOOKMARK_1625_112_2_18_2_5_40_28,"[BRW]W MMIBRW_ChangeBookmarkUrl:url_buf alloc fail")
TRACE_MSG(MMIBROWSER_BOOKMARK_1630_112_2_18_2_5_40_29,"[BRW]MMIBRW_ChangeBookmarkUrl buf=%s")
TRACE_MSG(MMIBROWSER_BOOKMARK_1637_112_2_18_2_5_40_30,"[BRW]MMIBRW_ChangeBookmarkUrl:write file data result = %d")
TRACE_MSG(MMIBROWSER_BOOKMARK_1644_112_2_18_2_5_41_31,"[BRW]W MMIBRW_ChangeBookmarkUrl: MMIFILEARRAY_Read fail")
TRACE_MSG(MMIBROWSER_BOOKMARK_1668_112_2_18_2_5_41_32,"[BRW]MMIBRW_ChangeBookmarkUrl:MMIFILEARRAY_Modify result = %d")
TRACE_MSG(MMIBROWSER_BOOKMARK_1876_112_2_18_2_5_41_33,"MMIBRW_CleanBookmarkData root_file_array == PNULL")
TRACE_MSG(MMIBROWSER_BOOKMARK_1938_112_2_18_2_5_41_34,"[BRW]MMIBRW_CleanBookmarkData bookmark_num=%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_1943_112_2_18_2_5_41_35,"MMIBRW_CleanBookmarkData result=%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_2015_112_2_18_2_5_41_36,"MMIBRW_SetDefaultBookmark ret = %d")
TRACE_MSG(MMIBROWSER_BOOKMARK_2021_112_2_18_2_5_41_37,"MMIBRW_SetDefaultBookmark NO memory")
TRACE_MSG(MMIBROWSER_BOOKMARK_2057_112_2_18_2_5_41_38,"MMIBRW_SetDefaultBookmark ret = %d")
TRACE_MSG(MMIBROWSER_BOOKMARK_2061_112_2_18_2_5_41_39,"MMIBRW_SetDefaultBookmark NO memory")
TRACE_MSG(MMIBROWSER_BOOKMARK_2597_112_2_18_2_5_42_40,"[BRW]W MMIBROWSER_BookmarkGetRootPath:dst_path_name_ptr alloc fail")
TRACE_MSG(MMIBROWSER_BOOKMARK_2611_112_2_18_2_5_42_41,"[BRW]MMIBROWSER_BookmarkGetRootPath result =%d, dev =%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_2666_112_2_18_2_5_43_42,"[BRW]MMIBROWSER_LoadFinish num=%d")
TRACE_MSG(MMIBROWSER_BOOKMARK_2683_112_2_18_2_5_43_43,"[BRW]MMIBROWSER_LoadFinish folder_num=%d")
TRACE_MSG(MMIBROWSER_DORADO_API_95_112_2_18_2_5_43_44,"[MMIBROWSER]: HandleSIMPlugNotifyBrowserFunc dual_sys=%d, notify_event=%d")
TRACE_MSG(MMIBROWSER_DORADO_API_280_112_2_18_2_5_44_45,"[BRW]MMIDORADO_Entry dual_sys = %d")
TRACE_MSG(MMIBROWSER_DORADO_API_393_112_2_18_2_5_44_46,"[ATC:][FT:][BOOKMARK:]Atest_BROWSERBookmarkTitleInput  res = %d")
TRACE_MSG(MMIBROWSER_DORADO_API_416_112_2_18_2_5_44_47,"[ATC:][FT:][BOOKMARK:]Atest_BROWSERBookmarkURLInput  res=%d")
TRACE_MSG(MMIBROWSER_FUNC_195_112_2_18_2_5_45_48,"[BRW]W MMIBROWSER_Enter: Try later")
TRACE_MSG(MMIBROWSER_FUNC_206_112_2_18_2_5_45_49,"[BRW]W MMIBROWSER_Init error")
TRACE_MSG(MMIBROWSER_FUNC_215_112_2_18_2_5_45_50,"[BRW]MMIBROWSER_Enter:avtive_sim:%d")
TRACE_MSG(MMIBROWSER_FUNC_219_112_2_18_2_5_45_51,"[BRW]MMIBROWSER_Enter sim mcc:%d, mnc:%d")
TRACE_MSG(MMIBROWSER_FUNC_224_112_2_18_2_5_45_52,"[BRW]MMIBROWSER_Enter type = %d")
TRACE_MSG(MMIBROWSER_FUNC_271_112_2_18_2_5_45_53,"[BRW]W MMIBROWSER_Init:InitMem error")
TRACE_MSG(MMIBROWSER_FUNC_313_112_2_18_2_5_45_54,"[BRW]MMIBROWSER_Exit")
TRACE_MSG(MMIBROWSER_FUNC_354_112_2_18_2_5_45_55,"[BRW]MMIBROWSER_Exit has done or doing")
TRACE_MSG(MMIBROWSER_FUNC_368_112_2_18_2_5_45_56,"[BRW]MMIBROWSER_StartWeb:is_browser_inited = %d")
TRACE_MSG(MMIBROWSER_FUNC_431_112_2_18_2_5_45_57,"[BRW]MMIBROWSER_ExitWeb")
TRACE_MSG(MMIBROWSER_FUNC_450_112_2_18_2_5_45_58,"[BRW]MMIBROWSER_ExitWeb has done")
TRACE_MSG(MMIBROWSER_FUNC_536_112_2_18_2_5_45_59,"[BRW]MMIBROWSER_SelectNetworkType:sim_num=%d,dual_sys=%d,is_use_wifi=%d")
TRACE_MSG(MMIBROWSER_FUNC_618_112_2_18_2_5_46_60,"[BRW]W MMIBROWSER_Enter MMIAPICONNECTION_GetLinkSettingItemByIndex error")
TRACE_MSG(MMIBROWSER_FUNC_660_112_2_18_2_5_46_61,"[BRW]W MMIBROWSER_SetHomepage result =%d")
TRACE_MSG(MMIBROWSER_FUNC_759_112_2_18_2_5_46_62,"[BRW]W SetNumState2String alloc fail")
TRACE_MSG(MMIBROWSER_FUNC_764_112_2_18_2_5_46_63,"[BRW]W SetNumState2String ERROR")
TRACE_MSG(MMIBROWSER_FUNC_787_112_2_18_2_5_46_64,"[BRW]W MMIBROWSER_MergeTextAndNum error")
TRACE_MSG(MMIBROWSER_FUNC_815_112_2_18_2_5_46_65,"[BRW]W MMIBROWSER_MergeTextAndNum alloc fail--text_str.wstr_len =%d")
TRACE_MSG(MMIBROWSER_FUNC_889_112_2_18_2_5_46_66,"[BRW]W MMIBROWSER_FmmPathToDisplay param error")
TRACE_MSG(MMIBROWSER_FUNC_909_112_2_18_2_5_46_67,"[BRW]W MMIBROWSER_FmmPathToDisplay err dev")
TRACE_MSG(MMIBROWSER_FUNC_931_112_2_18_2_5_46_68,"[BRW]W MMIBROWSER_DisplayToFmmPath param error")
TRACE_MSG(MMIBROWSER_FUNC_950_112_2_18_2_5_46_69,"[BRW]W MMIBROWSER_DisplayToFmmPath err dev")
TRACE_MSG(MMIBROWSER_FUNC_1007_112_2_18_2_5_46_70,"MMIDL_CombineDownloadFilename_ptr->wstr_len = %d,  path_ptr->wstr_len = %d,is_combine_tmp_file=%d")
TRACE_MSG(MMIBROWSER_FUNC_1071_112_2_18_2_5_47_71,"[BRW]MMIBROWSER BrowserDlGetDefaultMediaDir:path_ptr=0x%x,dir_path_len_ptr=0x%x")
TRACE_MSG(MMIBROWSER_FUNC_1147_112_2_18_2_5_47_72,"[BRW]BrowserDlGetDeviceName file_type =%d")
TRACE_MSG(MMIBROWSER_FUNC_1170_112_2_18_2_5_47_73,"[BRW]W MMIBROWSER_GetDefaultMediaFilePath full_path_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_FUNC_1265_112_2_18_2_5_47_74,"[BRW]MMIBROWSER_HandleZoom gesture_type = %d, is_need_zoom=%d")
TRACE_MSG(MMIBROWSER_FUNC_1289_112_2_18_2_5_47_75,"[BRW]MMIBROWSER_GetBrwSysDevName dev =%d")
TRACE_MSG(MMIBROWSER_FUNC_1313_112_2_18_2_5_47_76,"[BRW]W MMIBROWSER_GetCurPageUrlInfo error param")
TRACE_MSG(MMIBROWSER_FUNC_1320_112_2_18_2_5_47_77,"[BRW]W MMIBROWSER_GetCurPageUrlInfo url_ptr alloc fails")
TRACE_MSG(MMIBROWSER_FUNC_1357_112_2_18_2_5_47_78,"[BRW]W MMIBROWSER_GetCurPageUrlInfo wurl_buf_ptr alloc fails")
TRACE_MSG(MMIBROWSER_FUNC_1389_112_2_18_2_5_47_79,"[BRW]MMIBROWSER_GetCurPageUrlInfo result =%d")
TRACE_MSG(MMIBROWSER_FUNC_1432_112_2_18_2_5_47_80,"[BRW]W MMIBROWSER_GetCurAddrLabelInfo label_info_pptr null")
TRACE_MSG(MMIBROWSER_FUNC_1439_112_2_18_2_5_47_81,"[BRW]W MMIBROWSER_GetCurAddrLabelInfo label_info_ptr alloc fails")
TRACE_MSG(MMIBROWSER_FUNC_1883_112_2_18_2_5_48_82,"[BRW]GetMyNavDefaultInfo:PARAM null")
TRACE_MSG(MMIBROWSER_FUNC_1989_112_2_18_2_5_49_83,"[BRW]W MMIBROWSER_GetIcoPath:dst_path_name_ptr alloc fail")
TRACE_MSG(MMIBROWSER_FUNC_2002_112_2_18_2_5_49_84,"[BRW]MMIBROWSER_GetIcoPath result =%d, dev =%d")
TRACE_MSG(MMIBROWSER_FUNC_2032_112_2_18_2_5_49_85,"[BRW W]MMIBROWSER_GetIcoFullFileName:FAIL")
TRACE_MSG(MMIBROWSER_FUNC_2071_112_2_18_2_5_49_86,"[BRW]MMIBROWSER_DeleteIcoDir:sfs_err=%d")
TRACE_MSG(MMIBROWSER_FUNC_2078_112_2_18_2_5_49_87,"[BRW]MMIBROWSER_DeleteIcoDir:result=%d")
TRACE_MSG(MMIBROWSER_FUNC_2097_112_2_18_2_5_49_88,"[BRW]MMIBROWSER_DeleteIco sfs_err=%d")
TRACE_MSG(MMIBROWSER_FUNC_2105_112_2_18_2_5_49_89,"[BRW]MMIBROWSER_DeleteIco:get name fail")
TRACE_MSG(MMIBROWSER_SETTING_140_112_2_18_2_5_50_90,"[BRW]W MMIDORADO_GetNetSettingIndex dual_sys =%d")
TRACE_MSG(MMIBROWSER_SETTING_317_112_2_18_2_5_51_91,"MMIBROWSER_Setting_ConnectionCallback,index=%d,dual_sys=%d")
TRACE_MSG(MMIBROWSER_SETTING_583_112_2_18_2_5_51_92,"[BRW]MMIBROWSER_UpdateUrl ALLOC FAIL len =%d")
TRACE_MSG(MMIBROWSER_SETTING_594_112_2_18_2_5_51_93,"MMIBROWSER_UpdateUrl error_code= %d")
TRACE_MSG(MMIBROWSER_SETTING_619_112_2_18_2_5_51_94,"[BRW]MMIBROWSER_UpdateUserAgent ALLOC FAIL len =%d")
TRACE_MSG(MMIBROWSER_SETTING_648_112_2_18_2_5_51_95,"[BRW]MMIBROWSER_UpdateUaProfile ALLOC FAIL len =%d")
TRACE_MSG(MMIBROWSER_SETTING_679_112_2_18_2_5_51_96,"[BRW]MMIBROWSER_UpdateUserName ALLOC FAIL len =%d")
TRACE_MSG(MMIBROWSER_SETTING_716_112_2_18_2_5_51_97,"[BRW]MMIBROWSER_UpdatePassword ALLOC FAIL len =%d")
TRACE_MSG(MMIBROWSER_SETTING_1073_112_2_18_2_5_52_98,"MMIBROWSER_SetAnimateConnectCtrlState:state = %d,s_browser_context.animate_state = %d")
TRACE_MSG(MMIBROWSER_SETTING_1226_112_2_18_2_5_53_99,"[BRW]W MMIBROWSER_UpdateFileNameReq next_url_ptr ALLOC Fail!!")
TRACE_MSG(MMIBROWSER_SETTING_1304_112_2_18_2_5_53_100,"[BRW]MMIBROWSER_UpdateBookmarkEdit title ALLOC FAIL len =%d")
TRACE_MSG(MMIBROWSER_SETTING_1325_112_2_18_2_5_53_101,"[BRW]MMIBROWSER_UpdateBookmarkEdit URL ALLOC FAIL len =%d")
TRACE_MSG(MMIBROWSER_SETTING_1456_112_2_18_2_5_53_102,"[BRW] MMIBROWSER_UpdateSaveFile ALLOC FAIL LEN=%d")
TRACE_MSG(MMIBROWSER_SETTING_1611_112_2_18_2_5_53_103,"brw MMIBROWSER_MIMEDownlaodCallBack mime=%d, url=%s")
TRACE_MSG(MMIBROWSER_SETTING_1620_112_2_18_2_5_53_104,"\n MMIBROWSER MIME CALLBACK PARAM ERROR callback")
TRACE_MSG(MMIBROWSER_SETTING_1625_112_2_18_2_5_53_105,"\n MMIBROWSER MIME CALLBACK PARAM ERROR param_ptr =%d")
TRACE_MSG(MMIBROWSER_SETTING_1697_112_2_18_2_5_53_106,"BRW MMIBROWSER_SetIsUseWifi is_use_wifi = %d")
TRACE_MSG(MMIBROWSER_SETTING_1732_112_2_18_2_5_54_107,"[BRW]MMIBROWSER_UpdateSnapshotFilename ALLOC FAIL len =%d")
TRACE_MSG(MMIBROWSER_SETTING_1774_112_2_18_2_5_54_108,"[BRW]MMIBROWSER_SetEntryType entry_type = %d")
TRACE_MSG(MMIBROWSER_SETTING_1794_112_2_18_2_5_54_109,"[BRW]MMIBROWSER_SetWebEntryType entry_type = %d")
TRACE_MSG(MMIBROWSER_SETTING_1844_112_2_18_2_5_54_110,"[BRW]MMIBROWSER_UpdateEntryUrl ALLOC FAIL len =%d")
TRACE_MSG(MMIBROWSER_SETTING_1855_112_2_18_2_5_54_111,"MMIBROWSER_UpdateEntryUrl error_code= %d")
TRACE_MSG(MMIBROWSER_SETTING_1909_112_2_18_2_5_54_112,"[BRW]MMIBROWSER_UpdateSnapshotUrl ALLOC FAIL len =%d")
TRACE_MSG(MMIBROWSER_SETTING_1922_112_2_18_2_5_54_113,"MMIBROWSER_UpdateSnapshotUrl error_code= %d")
TRACE_MSG(MMIBROWSER_SETTING_1973_112_2_18_2_5_54_114,"[BRW]MMIBROWSER_UpdateSnapshotFilename ALLOC FAIL len =%d")
TRACE_MSG(MMIBROWSER_SETTING_2017_112_2_18_2_5_54_115,"[BRW]MMIDORADO_SetConnectionSettings:connection_index=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_2035_112_2_18_2_5_58_116,"[BRW]W BrwSetDDPropertyInfo PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_2256_112_2_18_2_5_59_117,"[BRW]W HandleOMADLQueryWinMsg OK PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_2273_112_2_18_2_5_59_118,"[BRW]W HandleOMADLQueryWinMsg cancel PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_2324_112_2_18_2_5_59_119,"[BRW]HandleControlDisplayReadyInd :title_len = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_2333_112_2_18_2_5_59_120,"[BRW]W display page title error, alloc PNULL!")
TRACE_MSG(MMIBROWSER_WINTABLE_2340_112_2_18_2_5_59_121,"[BRW]W BRW_MSG_TYPE_DISPLAY_READY_IND:BRW_DisplayPage Error!")
TRACE_MSG(MMIBROWSER_WINTABLE_2352_112_2_18_2_5_59_122,"MMIBROWSER need display TRUE")
TRACE_MSG(MMIBROWSER_WINTABLE_2375_112_2_18_2_5_59_123,"[BRW]W HandleControlAuthReq:wrong param!")
TRACE_MSG(MMIBROWSER_WINTABLE_2485_112_2_18_2_5_59_124,"[BRW]W HandleControlEnterFileNMInd:wrong param!")
TRACE_MSG(MMIBROWSER_WINTABLE_2518_112_2_18_2_5_59_125,"[BRW]W HandleControlSslCertInd:wrong param!")
TRACE_MSG(MMIBROWSER_WINTABLE_2537_112_2_18_2_5_59_126,"[BRW]W BHandleControlProgressInd:wrong param!")
TRACE_MSG(MMIBROWSER_WINTABLE_2543_112_2_18_2_5_59_127,"HandleControlProgressInd:prg_ptr->total_data_len =%d")
TRACE_MSG(MMIBROWSER_WINTABLE_2557_112_2_18_2_5_59_128,"BHandleControlProgressInd:receive_data_len = %d,total_data_len =%d")
TRACE_MSG(MMIBROWSER_WINTABLE_2756_112_2_18_2_6_0_129,"[BRW]W HandleControlPageGetInd:wrong param!")
TRACE_MSG(MMIBROWSER_WINTABLE_2803_112_2_18_2_6_0_130,"[BRW]W HandleControlMediaGetOneInd cur=%d,total=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_2820_112_2_18_2_6_0_131,"[BRW]HandleControlPageStopInd req_state =%d")
TRACE_MSG(MMIBROWSER_WINTABLE_2872_112_2_18_2_6_0_132,"[BRW]W HandleControlOMADLConfirmReq:wrong param!")
TRACE_MSG(MMIBROWSER_WINTABLE_2910_112_2_18_2_6_0_133,"[BRW]HandleControlMCReq")
TRACE_MSG(MMIBROWSER_WINTABLE_2934_112_2_18_2_6_0_134,"[BRW]HandleControlSendDTMFReq")
TRACE_MSG(MMIBROWSER_WINTABLE_3029_112_2_18_2_6_1_135,"[BRW]HandleControlSendSMSReq")
TRACE_MSG(MMIBROWSER_WINTABLE_3035_112_2_18_2_6_1_136,"[BRW]HandleControlSendSmsReq content_wstr->wstr_ptr ALLOC Fail")
TRACE_MSG(MMIBROWSER_WINTABLE_3041_112_2_18_2_6_1_137,"[BRW]HandleControlSendSmsReq:wrong param!")
TRACE_MSG(MMIBROWSER_WINTABLE_3077_112_2_18_2_6_1_138,"[BRW]HandleControlSendMMSReq")
TRACE_MSG(MMIBROWSER_WINTABLE_3088_112_2_18_2_6_1_139,"[BRW]HandleControlSendMmsReq:wrong param!")
TRACE_MSG(MMIBROWSER_WINTABLE_3126_112_2_18_2_6_1_140,"[BRW]HandleControlSendMAILTOReq")
TRACE_MSG(MMIBROWSER_WINTABLE_3135_112_2_18_2_6_1_141,"[BRW]HandleControlSendMailToReq temp_ptr ALLOC Fail")
TRACE_MSG(MMIBROWSER_WINTABLE_3141_112_2_18_2_6_1_142,"[BRW]HandleControlSendMailToReq:wrong param!")
TRACE_MSG(MMIBROWSER_WINTABLE_3320_112_2_18_2_6_1_143,"\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl SUCC")
TRACE_MSG(MMIBROWSER_WINTABLE_3324_112_2_18_2_6_1_144,"\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl FAIL")
TRACE_MSG(MMIBROWSER_WINTABLE_3415_112_2_18_2_6_2_145,"[BRW]MMIBROWSER_DisplayDownloadProgress GUI_IntersectRect FALSE")
TRACE_MSG(MMIBROWSER_WINTABLE_3745_112_2_18_2_6_2_146,"\n MMIBROWSER start key timer\n")
TRACE_MSG(MMIBROWSER_WINTABLE_3759_112_2_18_2_6_2_147,"\n MMIBROWSER stop key timer\n")
TRACE_MSG(MMIBROWSER_WINTABLE_3869_112_2_18_2_6_2_148,"\n MMIBROWSER BrwHandleMainWinMsg BRW_InitInstance error")
TRACE_MSG(MMIBROWSER_WINTABLE_3926_112_2_18_2_6_3_149,"BrwHandleMainOpenWinMsg: BRW_AccessPage error")
TRACE_MSG(MMIBROWSER_WINTABLE_3971_112_2_18_2_6_3_150,"BrwHandleMainWinMsg:msg_id = 0x%x")
TRACE_MSG(MMIBROWSER_WINTABLE_4105_112_2_18_2_6_3_151,"MMIBROWSER DISPLAY = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_4117_112_2_18_2_6_3_152,"[BRW]W BrwHandleMainWinMsg BRW_DisplayPage error instance=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_4127_112_2_18_2_6_3_153,"[BRW]W BrwHandleMainWinMsg BRW_RepaintPage error instance=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_4180_112_2_18_2_6_3_154,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 1")
TRACE_MSG(MMIBROWSER_WINTABLE_4193_112_2_18_2_6_3_155,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 2")
TRACE_MSG(MMIBROWSER_WINTABLE_4206_112_2_18_2_6_3_156,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 3")
TRACE_MSG(MMIBROWSER_WINTABLE_4219_112_2_18_2_6_3_157,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 4")
TRACE_MSG(MMIBROWSER_WINTABLE_4233_112_2_18_2_6_3_158,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 7")
TRACE_MSG(MMIBROWSER_WINTABLE_4257_112_2_18_2_6_3_159,"\n MMIBROWSER BrwHandleMainWinMsg BRW_AccessPage error")
TRACE_MSG(MMIBROWSER_WINTABLE_4281_112_2_18_2_6_3_160,"BrwHandleMainWinMsg:MSG_CTL_CANCEL BRW_StopPageReq error")
TRACE_MSG(MMIBROWSER_WINTABLE_4353_112_2_18_2_6_3_161,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcLayoutTimer error")
TRACE_MSG(MMIBROWSER_WINTABLE_4370_112_2_18_2_6_3_162,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 1")
TRACE_MSG(MMIBROWSER_WINTABLE_4382_112_2_18_2_6_3_163,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 2")
TRACE_MSG(MMIBROWSER_WINTABLE_4393_112_2_18_2_6_4_164,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 3")
TRACE_MSG(MMIBROWSER_WINTABLE_4404_112_2_18_2_6_4_165,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 4")
TRACE_MSG(MMIBROWSER_WINTABLE_4415_112_2_18_2_6_4_166,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 4")
TRACE_MSG(MMIBROWSER_WINTABLE_4479_112_2_18_2_6_4_167,"[BRW]:MMIBROWSER_HandleZoom gesture_type =%d")
TRACE_MSG(MMIBROWSER_WINTABLE_4528_112_2_18_2_6_4_168,"[BRW]MMIBROWSER_GetDefaultUserAgent mcc:%d, mnc:%d")
TRACE_MSG(MMIBROWSER_WINTABLE_4545_112_2_18_2_6_4_169,"[BRW]MMIBROWSER_GetDefaultUserAgent PS is invalid")
TRACE_MSG(MMIBROWSER_WINTABLE_4581_112_2_18_2_6_4_170,"\n MMIBROWSER MMIBROWSER_SetProfile MMIBROWSER_GetNVSetting error")
TRACE_MSG(MMIBROWSER_WINTABLE_4626_112_2_18_2_6_4_171,"\n MMIBROWSER MMIBROWSER_SetProfile MMIAPICONNECTION_GetLinkSettingItemByIndex error")
TRACE_MSG(MMIBROWSER_WINTABLE_5125_112_2_18_2_6_5_172,"[BRW] BrwHandleEditUrlWinMsg PNULL == need_item_data_ptr")
TRACE_MSG(MMIBROWSER_WINTABLE_5221_112_2_18_2_6_5_173,"StopButtonCallBack BRW_AccessPage error")
TRACE_MSG(MMIBROWSER_WINTABLE_5238_112_2_18_2_6_5_174,"StopButtonCallBack() BRW_StopPageReq error")
TRACE_MSG(MMIBROWSER_WINTABLE_5357_112_2_18_2_6_6_175,"brw mmi get callback msg, id:%d")
TRACE_MSG(MMIBROWSER_WINTABLE_5376_112_2_18_2_6_6_176,"[BRW]MMIBROWSER_OpenCCSelectWin error1")
TRACE_MSG(MMIBROWSER_WINTABLE_5382_112_2_18_2_6_6_177,"[BRW]MMIBROWSER_OpenCCSelectWin invalid number")
TRACE_MSG(MMIBROWSER_WINTABLE_5405_112_2_18_2_6_6_178,"BRW mmi cc enter selectwin  %s")
TRACE_MSG(MMIBROWSER_WINTABLE_5430_112_2_18_2_6_6_179,"[BRW]MMIDORADO_HandleBrwControlMsg:brw_message_id:%d")
TRACE_MSG(MMIBROWSER_WINTABLE_5435_112_2_18_2_6_6_180,"[BRW]MMIDORADO_HandleBrwControlMsg:MSG_BRW_STOP_BGSOUND")
TRACE_MSG(MMIBROWSER_WINTABLE_5455_112_2_18_2_6_6_181,"[BRW]W MMIDORADO_HandleBrwControlMsg BRW_RepaintPage ERROR")
TRACE_MSG(MMIBROWSER_WINTABLE_5616_112_2_18_2_6_6_182,"[BRW]BrwShowErrMsg error=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_5928_112_2_18_2_6_7_183,"BRW:MMIBROWSER_UpdateAddrLable url_ptr is PNULL!")
TRACE_MSG(MMIBROWSER_WINTABLE_6127_112_2_18_2_6_7_184,"[BRW]BrwSIMSelectCallback sys_state[%d] = MMI_DUAL_SYS_MAX")
TRACE_MSG(MMIBROWSER_WINTABLE_6301_112_2_18_2_6_7_185,"BrwHandleInputFileNameWinMsg suffix name len ==0, ERROR")
TRACE_MSG(MMIBROWSER_WINTABLE_6394_112_2_18_2_6_8_186,"[BRW]BrwDlGetDefaultFileName is_exit_ext_name = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_6429_112_2_18_2_6_8_187,"BrwHandlePopMenuWinMsg:msg_id = 0x%x")
TRACE_MSG(MMIBROWSER_WINTABLE_6443_112_2_18_2_6_8_188,"BrwHandlePopMenuWinMsg:menu_id = 0x%x")
TRACE_MSG(MMIBROWSER_WINTABLE_6553_112_2_18_2_6_8_189,"ID_POPUP_MENU_STOP BRW_StopPageReq error")
TRACE_MSG(MMIBROWSER_WINTABLE_6574_112_2_18_2_6_8_190,"MMIBROWSER forward error")
TRACE_MSG(MMIBROWSER_WINTABLE_6585_112_2_18_2_6_8_191,"MMIBROWSER backward error")
TRACE_MSG(MMIBROWSER_WINTABLE_6776_112_2_18_2_6_8_192,"\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCache fail")
TRACE_MSG(MMIBROWSER_WINTABLE_6788_112_2_18_2_6_8_193,"\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCookie fail")
TRACE_MSG(MMIBROWSER_WINTABLE_6817_112_2_18_2_6_8_194,"\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCache fail")
TRACE_MSG(MMIBROWSER_WINTABLE_6823_112_2_18_2_6_8_195,"\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCookie fail")
TRACE_MSG(MMIBROWSER_WINTABLE_7120_112_2_18_2_6_9_196,"BrwScrollBarCreate ::PNULL==rect_ptr")
TRACE_MSG(MMIBROWSER_WINTABLE_7662_112_2_18_2_6_10_197,"MMIBROWSER_StopLoading: BRW_StopPageReq error")
TRACE_MSG(MMIBROWSER_WINTABLE_7919_112_2_18_2_6_11_198,"[BRW]HandleSlide 1")
TRACE_MSG(MMIBROWSER_WINTABLE_7930_112_2_18_2_6_11_199,"[BRW]HandleSlide 2")
TRACE_MSG(MMIBROWSER_WINTABLE_8033_112_2_18_2_6_11_200,"MMIBROWSER MMIBROWSER_HandleTpPress BRW_ProcPenDownEvent error")
TRACE_MSG(MMIBROWSER_WINTABLE_8091_112_2_18_2_6_11_201,"[BRW]MMIBROWSER_HandleTpPress BRW_ProcPenUpEvent error")
TRACE_MSG(MMIBROWSER_WINTABLE_8106_112_2_18_2_6_11_202,"[BRW]MMIBROWSER_HandleTpPress fling_velocity =%d")
TRACE_MSG(MMIBROWSER_WINTABLE_8180_112_2_18_2_6_11_203,"MMIBROWSER_HandleTpPress s_is_handle_move=%d, s_is_moving=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_8181_112_2_18_2_6_11_204,"MMIBROWSER_HandleTpPress s_move_x=%d, s_move_y=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_8199_112_2_18_2_6_11_205,"MMIBROWSER MMIBROWSER_HandleTpPress BRW_ProcPenDownEvent error")
TRACE_MSG(MMIBROWSER_WINTABLE_8220_112_2_18_2_6_11_206,"MMIBROWSER MMIBROWSER_HandleTpPress BRW_ProcPenUpEvent error")
TRACE_MSG(MMIBROWSER_WINTABLE_8275_112_2_18_2_6_11_207,"[BRW]W MMIBROWSER_EnterNewUrl url_ptr PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_8329_112_2_18_2_6_12_208,"[BRW] BrwPrepareToDL error file_name_req_ptr = PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_8512_112_2_18_2_6_12_209,"[BRW] BrwPrepareToDL generate file name fail")
TRACE_MSG(MMIBROWSER_WINTABLE_8611_112_2_18_2_6_12_210,"[BRW] MMIBROWSER_DelInvalidDlFile")
TRACE_MSG(MMIBROWSER_WINTABLE_8618_112_2_18_2_6_12_211,"[BRW] MMIBROWSER_DelInvalidDlFile result = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_8633_112_2_18_2_6_12_212,"[BRW]MMIBROWSER_StopDownload SUCC")
TRACE_MSG(MMIBROWSER_WINTABLE_8637_112_2_18_2_6_12_213,"[BRW]MMIBROWSER_StopDownload FAIL")
TRACE_MSG(MMIBROWSER_WINTABLE_8792_112_2_18_2_6_13_214,"\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl SUCC")
TRACE_MSG(MMIBROWSER_WINTABLE_8796_112_2_18_2_6_13_215,"\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl FAIL")
TRACE_MSG(MMIBROWSER_WINTABLE_8902_112_2_18_2_6_13_216,"\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl SUCC")
TRACE_MSG(MMIBROWSER_WINTABLE_8906_112_2_18_2_6_13_217,"\n MMIBROWSER BrwHandleDownladMediaFileWinMsg stop dl FAIL")
TRACE_MSG(MMIBROWSER_WINTABLE_9098_112_2_18_2_6_14_218,"[BRW]W SetDownloadFileNamePathWindow file_name_req_ptr PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_9125_112_2_18_2_6_14_219,"[BRW]BrwOpenLabelCallBack is_success = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_9131_112_2_18_2_6_14_220,"[BRW]BrwOpenLabelCallBack, file_num = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_9148_112_2_18_2_6_14_221,"[BRW]BrwOpenLabelCallBack, file_type = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_9912_112_2_18_2_6_16_222,"BrwHandleStartPagePopMenuWinMsg:menu_id = 0x%x")
TRACE_MSG(MMIBROWSER_WINTABLE_10037_112_2_18_2_6_16_223,"\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCache fail")
TRACE_MSG(MMIBROWSER_WINTABLE_10049_112_2_18_2_6_16_224,"\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCookie fail")
TRACE_MSG(MMIBROWSER_WINTABLE_10077_112_2_18_2_6_16_225,"\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCache fail")
TRACE_MSG(MMIBROWSER_WINTABLE_10083_112_2_18_2_6_16_226,"\n MMIBROWSER BrwHandlePopMenuWinMsg BRW_ClearCookie fail")
TRACE_MSG(MMIBROWSER_WINTABLE_10313_112_2_18_2_6_17_227,"BrwHandleAlertWinMsg msg_id = %x")
TRACE_MSG(MMIBROWSER_WINTABLE_11085_112_2_18_2_6_19_228,"nv_info.item_count=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_11472_112_2_18_2_6_19_229,"[BRW]AddTabsAndChildWindows:tab_child_type=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_11942_112_2_18_2_6_20_230,"[BRW]BrwHandleStartPageTheme1WinMsg:err cur_index = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_12196_112_2_18_2_6_21_231,"[BRW]OpenOneMyNavUrl:err cur_index = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_12787_112_2_18_2_6_22_232,"[BRW]BrwSetMyNavDetailWindow:cur_index=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_13095_112_2_18_2_6_23_233,"[BRW]BrwHandleStartPageTheme1WinMsg:err cur_index = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_13187_112_2_18_2_6_23_234,"MMIBROWSER_SetPublicProfile MMIBROWSER_GetNVSetting error")
TRACE_MSG(MMIBROWSER_WINTABLE_13237_112_2_18_2_6_23_235,"[BRW]MMIBROWSER_SetProfile:use customized_connection")
TRACE_MSG(MMIBROWSER_WINTABLE_13248_112_2_18_2_6_23_236,"[BRW]MMIBROWSER_SetProfile:index=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_13253_112_2_18_2_6_23_237,"[BRW]W MMIBROWSER_SetPublicProfile:GetLinkSettingItemByIndex error")
TRACE_MSG(MMIBROWSER_WINTABLE_13495_112_2_18_2_6_23_238,"[BRW] HandleFixedInstanceControlMsg msg_id:%d")
TRACE_MSG(MMIBROWSER_WINTABLE_13566_112_2_18_2_6_24_239,"[BRW]MMIBROWSER_HandleFixedCtrlMsg:message_id = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_13604_112_2_18_2_6_24_240,"BrwHandleFixHtmlWinMsg:msg_id = 0x%x")
TRACE_MSG(MMIBROWSER_WINTABLE_13689_112_2_18_2_6_24_241,"[BRW]W BRW_DisplayPage error instance=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_13699_112_2_18_2_6_24_242,"[BRW]W BRW_DisplayPage error instance=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_13738_112_2_18_2_6_24_243,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 1")
TRACE_MSG(MMIBROWSER_WINTABLE_13750_112_2_18_2_6_24_244,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 2")
TRACE_MSG(MMIBROWSER_WINTABLE_13762_112_2_18_2_6_24_245,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 3")
TRACE_MSG(MMIBROWSER_WINTABLE_13774_112_2_18_2_6_24_246,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 4")
TRACE_MSG(MMIBROWSER_WINTABLE_13787_112_2_18_2_6_24_247,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcKeyEvent error 7")
TRACE_MSG(MMIBROWSER_WINTABLE_13844_112_2_18_2_6_24_248,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcLayoutTimer error")
TRACE_MSG(MMIBROWSER_WINTABLE_13860_112_2_18_2_6_24_249,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 1")
TRACE_MSG(MMIBROWSER_WINTABLE_13871_112_2_18_2_6_24_250,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 2")
TRACE_MSG(MMIBROWSER_WINTABLE_13881_112_2_18_2_6_24_251,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 3")
TRACE_MSG(MMIBROWSER_WINTABLE_13891_112_2_18_2_6_24_252,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 4")
TRACE_MSG(MMIBROWSER_WINTABLE_13901_112_2_18_2_6_24_253,"\n MMIBROWSER BrwHandleMainWinMsg BRW_ProcScrollEvent error 4")
TRACE_MSG(MMIBROWSER_WINTABLE_13982_112_2_18_2_6_25_254,"[WRE] Add2MainDelFunc  begin:")
TRACE_MSG(MMIBROWSER_WINTABLE_14078_112_2_18_2_6_25_255,"[BRW]ID_POPUP_MENU_ADD_TO_MAINMENU: alloc dynamic_item_ptr return PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_14132_112_2_18_2_6_25_256,"[BRW]ID_POPUP_MENU_ADD_TO_MAINMENU: alloc url_ptr return PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_14134_112_2_18_2_6_25_257,"[BRW] ID_POPUP_MENU_ADD_TO_MAINMENU: ret=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_931_112_2_18_2_6_27_258,"BrwHandleBookmarkWinMsg msg_id= 0x%x")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_1106_112_2_18_2_6_27_259,"BrwHandleBookmarkWinMsg() String is NULL")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_1541_112_2_18_2_6_28_260,"[BRW]W MMIBROWSER get item error")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_1728_112_2_18_2_6_29_261,"[BRW]BrwSetBookmarkEditWindow change_type=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_1870_112_2_18_2_6_29_262,"[BRW]W BrwSetBookmarkEditWindow MMIBRW_BM_CHANGE_ADD_URL err")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_2245_112_2_18_2_6_30_263,"\n MMIBROWSER BrwQueryIsDeleteBookmark out_title.wstr_ptr error 1")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_2254_112_2_18_2_6_30_264,"\n MMIBROWSER BrwQueryIsDeleteBookmark out_url.wstr_ptr error ")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_2289_112_2_18_2_6_30_265,"\n MMIBROWSER BrwQueryIsDeleteBookmark out_title.wstr_ptr error 2")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_2327_112_2_18_2_6_30_266,"BrwQueryIsDeleteBookmark temp_wptr ALLOC ERROR!!!")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_2503_112_2_18_2_6_30_267,"BrwDeleteMarkedBookmarkItem : MMIBRW_RemoveBookmark error;item_index = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_2929_112_2_18_2_6_31_268,"HandleBookmarkMoveWaitWinMsg  copy_result=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_2997_112_2_18_2_6_31_269,"MMIBRW_HandleMoveBookmark() msg_id=0x%04x")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3038_112_2_18_2_6_31_270,"[BRW]W MMIBRW_HandleMoveBookmark() MMIAPIFMM_DeleteFile FAIL")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3099_112_2_18_2_6_31_271,"[BRW]W BrwBookmarkMoveTo() PARAM ERROR")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3124_112_2_18_2_6_31_272,"[BRW]W BrwBookmarkMoveTo() result=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3182_112_2_18_2_6_31_273,"[BRW]W BrwBookmarkBeginMoveTo 1")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3187_112_2_18_2_6_31_274,"[BRW]W BrwBookmarkBeginMoveTo 2")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3197_112_2_18_2_6_32_275,"[BRW]W BrwBookmarkBeginMoveTo 3")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3209_112_2_18_2_6_32_276,"[BRW]W BrwBookmarkBeginMoveTo 4")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3219_112_2_18_2_6_32_277,"[BRW]W BrwBookmarkBeginMoveTo 5")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3679_112_2_18_2_6_32_278,"[BRW]BrowserHanldeSaveInEditWin:is_save_success = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3725_112_2_18_2_6_33_279,"HandleBKOpenMainWaitingWin, msg_id = 0x%x")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3767_112_2_18_2_6_33_280,"HandleBKOpenMainWaitingWin, msg_id = 0x%x end")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3846_112_2_18_2_6_33_281,"HandleBKOpenStepWaitingWin, msg_id = 0x%x")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_3907_112_2_18_2_6_33_282,"HandleBKOpenStepWaitingWin, msg_id = 0x%x end")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_4092_112_2_18_2_6_33_283,"HandleBKSaveWaitingWin, msg_id = 0x%x")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_4107_112_2_18_2_6_33_284,"[BRW]HandleBKSaveWaitingWin:s_save_result = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_4206_112_2_18_2_6_34_285,"HandleBKOpenStepWaitingWin, msg_id = 0x%x end")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_4306_112_2_18_2_6_34_286,"BrwHandleBookmarkPopUpWinMsg msg_id = %x")
TRACE_MSG(MMIBROWSER_WINTABLE_BOOKMARK_4547_112_2_18_2_6_34_287,"BrwHandleBookmarkAlertWinMsg msg_id = %x")
TRACE_MSG(MMIBROWSER_WINTABLE_HISTORY_290_112_2_18_2_6_36_288,"\n MMIBROWSER BrwHandleHistoryListWinMsg:malloc mem failed")
TRACE_MSG(MMIBROWSER_WINTABLE_HISTORY_299_112_2_18_2_6_36_289,"\n MMIBROWSER BrwHandleHistoryListWinMsg BRW_VisitHisGetHisInfoByIndex error")
TRACE_MSG(MMIBROWSER_WINTABLE_HISTORY_340_112_2_18_2_6_36_290,"[BRW]BrwHandleHistoryListWinMsg temp_visit_info_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_WINTABLE_HISTORY_440_112_2_18_2_6_36_291,"[BRW] BrwHandleHistoryMenuWinMsg menu_id = %d visit_info_ptr == PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_HISTORY_477_112_2_18_2_6_36_292,"[BRW] ID_POPUP_MENU_HISTORY_DETAIL ALLOC FAIL")
TRACE_MSG(MMIBROWSER_WINTABLE_HISTORY_664_112_2_18_2_6_36_293,"[BRW]BrwHandleHistoryDetailWinMsg MSG_OPEN_WINDOW visit_his_info_ptr == PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_HISTORY_692_112_2_18_2_6_36_294,"[BRW]BrwHandleHistoryDetailWinMsg msg_id=%d, visit_his_info_ptr == PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_HISTORY_781_112_2_18_2_6_37_295,"BrwHandleHistoryPopUpWinMsg msg_id = %x")
TRACE_MSG(MMIBROWSER_WINTABLE_HISTORY_807_112_2_18_2_6_37_296,"BrwHandleHistoryPopUpWinMsg visit_info_ptr = %x")
TRACE_MSG(MMIBROWSER_WINTABLE_HISTORY_829_112_2_18_2_6_37_297,"[BRW] ID_POPUP_MENU_HISTORY_DETAIL ALLOC FAIL")
TRACE_MSG(MMIBROWSER_WINTABLE_HISTORY_1055_112_2_18_2_6_37_298,"BrwHandleHistoryAlertWinMsg msg_id = %x")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_196_112_2_18_2_6_38_299,"[BRW]W MMIBROWSER_GetSearchEngine search_engine[err] = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_234_112_2_18_2_6_38_300,"[BRW]W MMIBROWSER_GetKeyInfo:key_data_ptr PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_274_112_2_18_2_6_38_301,"[BRW]W MMIBROWSER_WriteKeyItem:in_url_ptr PNULL ")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_281_112_2_18_2_6_38_302,"[BRW]W MMIBROWSER_WriteKeyItem:key_ptr Alloc FAIL ")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_374_112_2_18_2_6_38_303,"[BRW]W MMIBROWSER_GetMatchKeyNum:key_ptr Alloc FAIL ")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_421_112_2_18_2_6_38_304,"[BRW]W MMIBROWSER_ListMatchKeyName:key_ptr Alloc FAIL ")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_439_112_2_18_2_6_38_305,"[BRW]W MMIBROWSER_ListMatchKeyName:match_key_ptr Alloc FAIL ")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_456_112_2_18_2_6_38_306,"[BRW]W MMIBROWSER_ListMatchKeyName:temp_input_key Alloc FAIL ")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_473_112_2_18_2_6_38_307,"[BRW]W MMIBROWSER_ListMatchKeyName:temp_key_ptr Alloc FAIL and i=%d")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_496_112_2_18_2_6_38_308,"[BRW]W MMIBROWSER_ListMatchKeyName:match_key_ptr Alloc FAIL ")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_508_112_2_18_2_6_38_309,"[BRW]MMIBROWSER_ListMatchKeyName:no match ")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_546_112_2_18_2_6_39_310,"[BRW]MMIBROWSER_GenerateSearchWeb search_engine = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_598_112_2_18_2_6_39_311,"[BRW]W MMIBROWSER_GenerateSearchWeb url_ptr alloc")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_606_112_2_18_2_6_39_312,"[BRW]W MMIBROWSER_GenerateSearchWeb str_ptr alloc")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_611_112_2_18_2_6_39_313,"[BRW]W MMIBROWSER_GenerateSearchWeb PARAM ERROR")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_1088_112_2_18_2_6_40_314,"[BRW]W BrwHandleSearchListWinMsg PNULL == need_item_data_ptr")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_1095_112_2_18_2_6_40_315,"[BRW]W BrwHandleSearchListWinMsg:match_key_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_WINTABLE_SEARCH_1131_112_2_18_2_6_40_316,"[BRW]W BrwHandleSearchListWinMsg:cur_list_ptr PNULL")
TRACE_MSG(MMIBROWSER_WINTABLE_SETTING_851_112_2_18_2_6_42_317,"[BRW]BrwExecuteSetting:reset customized_connection")
TRACE_MSG(MMIBROWSER_WINTABLE_SETTING_877_112_2_18_2_6_42_318,"[BRW]W BrwExecuteSetting:BRW_InitInstance error")
TRACE_MSG(MMIBROWSER_WINTABLE_SETTING_892_112_2_18_2_6_42_319,"[BRW]W BrwExecuteSetting MMIBROWSER_GetHomepageUrl ERROR")
TRACE_MSG(MMIBROWSER_WINTABLE_SETTING_1213_112_2_18_2_6_43_320,"BrwHandleSettingHomepageTypeWinMsg msg_id = %x")
TRACE_MSG(MMIBROWSER_WINTABLE_SETTING_1342_112_2_18_2_6_43_321,"BrwHandleSettingFontWinMsg msg_id = %x")
TRACE_MSG(MMIBROWSER_WINTABLE_SETTING_1390_112_2_18_2_6_43_322,"BrwHandleSettingHomepageTypeWinMsg msg_id = %x")
TRACE_MSG(MMIBROWSER_WINTABLE_SETTING_1753_112_2_18_2_6_44_323,"BrwHandleSetttingResetAlertWinMsg msg_id = %x")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_434_112_2_18_2_6_45_324,"\n MMIBROWSER get item error")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_589_112_2_18_2_6_45_325,"BrwHandleSnapshotListWinMsg ::  PNULL == need_item_data_ptr")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_597_112_2_18_2_6_45_326,"\n MMIBROWSER BrwHandleSnapshotListWinMsg BRW_SnapshotGetItemInfo error")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_995_112_2_18_2_6_46_327,"BrwHandleSnapshotSaveFileWinMsg  result_for_check = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_1265_112_2_18_2_6_46_328,"\n BROWSER LILI TEST richtext 1")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_1269_112_2_18_2_6_46_329,"\n BROWSER LILI TEST richtext 2")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_1273_112_2_18_2_6_46_330,"\n BROWSER LILI TEST richtext 3")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_1543_112_2_18_2_6_47_331,"\n mmibrowser BrwHandleSnapshotPopMenuWinMsg BRW_SnapshotGetItemInfo error 1")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_1553_112_2_18_2_6_47_332,"\n mmibrowser BrwHandleSnapshotPopMenuWinMsg BRW_SnapshotGetItemInfo error 2")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_1713_112_2_18_2_6_47_333,"[BRW] BrwDeleteSelectedSnapshot FAIL 1")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_1730_112_2_18_2_6_47_334,"[BRW]BrwDeleteSelectedSnapshot FAIL 2")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_1741_112_2_18_2_6_47_335,"[BRW] BrwDeleteSelectedSnapshot ALLOC FAIL!")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_1825_112_2_18_2_6_48_336,"BrwQueryIsDeleteSnapshot temp_wptr ALLOC ERROR!!!")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_1989_112_2_18_2_6_48_337,"\n MMIBROWSER not change snapshot name")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_2130_112_2_18_2_6_48_338,"BrwHandlePageMemoPopUpWinMsg msg_id = %x")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_2157_112_2_18_2_6_48_339,"\n mmibrowser BrwHandleSnapshotPopMenuWinMsg item_info_ptr = %d")
TRACE_MSG(MMIBROWSER_WINTABLE_SNAPSHOT_2352_112_2_18_2_6_49_340,"BrwHandleHistoryAlertWinMsg msg_id = %x")
TRACE_MSG(BRW_CONTROL_208_112_2_18_2_6_49_341,"brw BrwSaveDataToFile create file error!")
TRACE_MSG(BRW_CONTROL_234_112_2_18_2_6_49_342,"[BRW]BRW_SetParam,temp_profile_type=0x%x")
TRACE_MSG(BRW_CONTROL_238_112_2_18_2_6_49_343,"[BRW]W BRW_SetPublicParam,profile=0x%x")
TRACE_MSG(BRW_CONTROL_290_112_2_18_2_6_49_344,"[BRW]BRW_SetParam:set ua null!")
TRACE_MSG(BRW_CONTROL_322_112_2_18_2_6_50_345,"[BRW]BRW_SetParam:set uaprof null!")
TRACE_MSG(BRW_CONTROL_352_112_2_18_2_6_50_346,"[BRW]BRW_SetParam,proxy_addr_ptr null!")
TRACE_MSG(BRW_CONTROL_370_112_2_18_2_6_50_347,"[BRW]BRW_SetParam,temp_profile_type=0x%x")
TRACE_MSG(BRW_CONTROL_374_112_2_18_2_6_50_348,"[BRW]W BRW_SetParam,instance=0x%x,profile=0x%x")
TRACE_MSG(BRW_CONTROL_410_112_2_18_2_6_50_349,"[BRW]W BRW_InitInstance,ERROR option_ptr = 0x%x")
TRACE_MSG(BRW_CONTROL_460_112_2_18_2_6_50_350,"BRW_InitInstance:instance_id =0x%x")
TRACE_MSG(BRW_CONTROL_472_112_2_18_2_6_50_351,"[BRW]BRW_DestroyInstance instance=0x%x")
TRACE_MSG(BRW_CONTROL_504_112_2_18_2_6_50_352,"[BRW]W BRW_DestroyInstance brw_info_ptr PNULL")
TRACE_MSG(BRW_CONTROL_577_112_2_18_2_6_50_353,"BRW_DisconnetNetwork")
TRACE_MSG(BRW_CONTROL_661_112_2_18_2_6_50_354,"BRW_AccessPage, gwt fail:%s")
TRACE_MSG(BRW_CONTROL_691_112_2_18_2_6_50_355,"brw alloc wap request in BRW_AccessPage:%x")
TRACE_MSG(BRW_CONTROL_753_112_2_18_2_6_51_356,"BRW BRW_AccessPage init http session")
TRACE_MSG(BRW_CONTROL_765_112_2_18_2_6_51_357,"BRW_AccessPage to ActiveNetwork")
TRACE_MSG(BRW_CONTROL_884_112_2_18_2_6_51_358,"BRW_DisplayPage,brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_899_112_2_18_2_6_51_359,"BRW_DisplayPage,layout_handler null")
TRACE_MSG(BRW_CONTROL_904_112_2_18_2_6_51_360,"BrwDisplayWebPage ret = %d")
TRACE_MSG(BRW_CONTROL_932_112_2_18_2_6_51_361,"BRW_RepaintPage,layout_handler null")
TRACE_MSG(BRW_CONTROL_937_112_2_18_2_6_51_362,"BRW_RepaintPage ret = %d")
TRACE_MSG(BRW_CONTROL_952_112_2_18_2_6_51_363,"[BRW]BRW_StopPageReq")
TRACE_MSG(BRW_CONTROL_963_112_2_18_2_6_51_364,"BRW_StopPageReq,wap_request_ptr null")
TRACE_MSG(BRW_CONTROL_972_112_2_18_2_6_51_365,"BRW_StopPageReq,state =%d")
TRACE_MSG(BRW_CONTROL_1021_112_2_18_2_6_51_366,"[BRW]BRW_StopDL")
TRACE_MSG(BRW_CONTROL_1024_112_2_18_2_6_51_367,"[BRW]BRW_StopDL,brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_1030_112_2_18_2_6_51_368,"[BRW]BRW_StopDL,invalid param,wap_req_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_1046_112_2_18_2_6_51_369,"[BRW]BRW_CheckIfPageNull,brw_info_ptr 0")
TRACE_MSG(BRW_CONTROL_1092_112_2_18_2_6_51_370,"BRW_ProcKeyEvent:brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_1206_112_2_18_2_6_52_371,"BRW_ProcKeyEvent,layout_handle invalid")
TRACE_MSG(BRW_CONTROL_1231_112_2_18_2_6_52_372,"BRW_ProcPenDownEvent:brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_1256_112_2_18_2_6_52_373,"BRW_ProcPenDownEvent,layout_handle invalid")
TRACE_MSG(BRW_CONTROL_1276_112_2_18_2_6_52_374,"BRW_ProcPenUpEvent:brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_1299_112_2_18_2_6_52_375,"BRW_ProcPenUpEvent,layout_handle invalid")
TRACE_MSG(BRW_CONTROL_1322_112_2_18_2_6_52_376,"BRW_ProcNavEvent:brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_1329_112_2_18_2_6_52_377,"[BRW]BRW_ProcNavEvent:cur_uri_ptr null")
TRACE_MSG(BRW_CONTROL_1351_112_2_18_2_6_52_378,"[BRW]BRW_ProcNavEvent:new_uri_ptr null")
TRACE_MSG(BRW_CONTROL_1358_112_2_18_2_6_52_379,"BRW_ProcNavEvent,snapshot nav_event:%d")
TRACE_MSG(BRW_CONTROL_1369_112_2_18_2_6_52_380,"[BRW]BRW_ProcNavEvent BrwIsUrlIsEquExceptFragment")
TRACE_MSG(BRW_CONTROL_1382_112_2_18_2_6_52_381,"BRW_ProcNavEvent,card jumping,event:%d")
TRACE_MSG(BRW_CONTROL_1503_112_2_18_2_6_52_382,"BRW_ProcScrollEvent:brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_1561_112_2_18_2_6_52_383,"BRW_ProcTpScrollEvent:brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_1634_112_2_18_2_6_52_384,"BRW_GetPageUrl:brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_1645_112_2_18_2_6_52_385,"BRW_GetPageUrl,cur_display_req_ptr null")
TRACE_MSG(BRW_CONTROL_1648_112_2_18_2_6_53_386,"[brw] is snapshot")
TRACE_MSG(BRW_CONTROL_1668_112_2_18_2_6_53_387,"BRW_GetPageUrl:url_pptr null")
TRACE_MSG(BRW_CONTROL_1682_112_2_18_2_6_53_388,"BRW BrwProcInitHttpCnf")
TRACE_MSG(BRW_CONTROL_1686_112_2_18_2_6_53_389,"BRW BrwProcInitHttpCnf brw_info_ptr is NULL")
TRACE_MSG(BRW_CONTROL_1691_112_2_18_2_6_53_390,"BRW BrwProcInitHttpCnf success")
TRACE_MSG(BRW_CONTROL_1695_112_2_18_2_6_53_391,"BRW BrwProcInitHttpCnf is_need_to_close_http")
TRACE_MSG(BRW_CONTROL_1703_112_2_18_2_6_53_392,"BRW BrwProcInitHttpCnf 1")
TRACE_MSG(BRW_CONTROL_1710_112_2_18_2_6_53_393,"BRW BrwProcInitHttpCnf 2")
TRACE_MSG(BRW_CONTROL_1716_112_2_18_2_6_53_394,"BRW BrwProcInitHttpCnf 3")
TRACE_MSG(BRW_CONTROL_1723_112_2_18_2_6_53_395,"BRW BrwProcInitHttpCnf 4")
TRACE_MSG(BRW_CONTROL_1733_112_2_18_2_6_53_396,"[BRW]BrwProcErr,err=%d")
TRACE_MSG(BRW_CONTROL_1774_112_2_18_2_6_53_397,"[BRW]BrwProcDownloadErr,err=%d")
TRACE_MSG(BRW_CONTROL_1787_112_2_18_2_6_53_398,"[BRW]W BrwProcNoMem,brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_1874_112_2_18_2_6_53_399,"[BRW]W BrwStartNewInstance,param_ptr=0x%x!")
TRACE_MSG(BRW_CONTROL_1898_112_2_18_2_6_53_400,"[BRW]W BrwStartNewPageReq,param_ptr=0x%x!")
TRACE_MSG(BRW_CONTROL_1946_112_2_18_2_6_53_401,"brw alloc wap request in BrwStartNewPageReq:%x")
TRACE_MSG(BRW_CONTROL_2021_112_2_18_2_6_53_402,"BrwStartNewPageReq to ActiveNetwork")
TRACE_MSG(BRW_CONTROL_2153_112_2_18_2_6_54_403,"BrwAllocMediaObj media_obj_ptr = 0x%x")
TRACE_MSG(BRW_CONTROL_2286_112_2_18_2_6_54_404,"[BRW]ProcPageContent")
TRACE_MSG(BRW_CONTROL_2297_112_2_18_2_6_54_405,"[BRW]ProcPageContent,mime_type:%d")
TRACE_MSG(BRW_CONTROL_2303_112_2_18_2_6_54_406,"[BRW]ProcPageContent,BrwDestroyLayoutCtrl: 0x%x")
TRACE_MSG(BRW_CONTROL_2307_112_2_18_2_6_54_407,"[BRW]ProcPageContent,creat layout:%x")
TRACE_MSG(BRW_CONTROL_2310_112_2_18_2_6_54_408,"[BRW]ProcPageContent,creat layout fail, mime_type:%d")
TRACE_MSG(BRW_CONTROL_2320_112_2_18_2_6_54_409,"brw proc page, BrwProcWmlScript result:%d")
TRACE_MSG(BRW_CONTROL_2360_112_2_18_2_6_54_410,"brw proc page, BrwParseWebPage result:%d")
TRACE_MSG(BRW_CONTROL_2369_112_2_18_2_6_54_411,"brw proc page, BrwLayout result:%d")
TRACE_MSG(BRW_CONTROL_2389_112_2_18_2_6_54_412,"brw ProcPageContent free cache")
TRACE_MSG(BRW_CONTROL_2536_112_2_18_2_6_54_413,"[BRW]W brw ProcPageOK:req_ptr=0x%x,brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_2554_112_2_18_2_6_54_414,"[BRW]W ProcMediaContent:brw_info_ptr=0x%x,req_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_2564_112_2_18_2_6_54_415,"[BRW] ProcMediaContent:req_ptr->resp_buf_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_2565_112_2_18_2_6_54_416,"[BRW] ProcMediaContent:media_obj_ptr->media_req_type=%d")
TRACE_MSG(BRW_CONTROL_2602_112_2_18_2_6_54_417,"[BRW]ProcMediaReq:BrwLayout failed")
TRACE_MSG(BRW_CONTROL_2624_112_2_18_2_6_54_418,"[BRW]ProcMediaReq:MEDIA_REQ_TYPE_BGSOUND FALSE")
TRACE_MSG(BRW_CONTROL_2634_112_2_18_2_6_54_419,"[BRW]ProcMediaReq:BrwLayout failed")
TRACE_MSG(BRW_CONTROL_2643_112_2_18_2_6_54_420,"[BRW]ProcMediaReq:BrwLayout failed")
TRACE_MSG(BRW_CONTROL_2652_112_2_18_2_6_54_421,"[BRW]ProcMediaReq:BrwLayout failed")
TRACE_MSG(BRW_CONTROL_2707_112_2_18_2_6_55_422,"[BRW]W ProcMediaReq,layout_handle=0x%x")
TRACE_MSG(BRW_CONTROL_2717_112_2_18_2_6_55_423,"[BRW]ProcMediaReq:media_obj_ptr = NULL!")
TRACE_MSG(BRW_CONTROL_2764_112_2_18_2_6_55_424,"BRW_AccessPage:BrwSendHttpRequest failed!")
TRACE_MSG(BRW_CONTROL_2787_112_2_18_2_6_55_425,"[BRW]W BrwStartNextMediaReq:brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_2847_112_2_18_2_6_55_426,"[BRW]W BrwStartMediaReq:brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_2865_112_2_18_2_6_55_427,"[BRW]W BrwRestartCurMediaReq:brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_3005_112_2_18_2_6_55_428,"[BRW]W BrwProcHttpReqDone:brw_info_ptr=0x%x,req_pptr=0x%x")
TRACE_MSG(BRW_CONTROL_3015_112_2_18_2_6_55_429,"[BRW]BrwProcHttpReqDone")
TRACE_MSG(BRW_CONTROL_3028_112_2_18_2_6_55_430,"[BRW]BrwProcHttpReqDone wap_req_ptr->http_req_ptr == 0")
TRACE_MSG(BRW_CONTROL_3041_112_2_18_2_6_55_431,"[BRW]W BrwProcHttpReqDone BRWUtil_DecodeGzip result =%d")
TRACE_MSG(BRW_CONTROL_3062_112_2_18_2_6_55_432,"BrwProcHttpReqDone page err")
TRACE_MSG(BRW_CONTROL_3078_112_2_18_2_6_55_433,"BrwProcHttpReqDone, new page has been started")
TRACE_MSG(BRW_CONTROL_3099_112_2_18_2_6_55_434,"[BRW]BrwProcHttpReqDone,777,%x,respbuf:%x")
TRACE_MSG(BRW_CONTROL_3115_112_2_18_2_6_55_435,"[BRW]BrwProcHttpReqDone,BRW_STATE_DOWNLOADING:transfer_encoding = %d")
TRACE_MSG(BRW_CONTROL_3211_112_2_18_2_6_56_436,"[BRW]BrwProcHttpReqDone:BRWUtil_DecodeGzip result =%d")
TRACE_MSG(BRW_CONTROL_3259_112_2_18_2_6_56_437,"BrwProcCloseHttpCnf,brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_3314_112_2_18_2_6_56_438,"[BRW]W GetBrwInfoPtrByWapReqID,req_id=%d")
TRACE_MSG(BRW_CONTROL_3326_112_2_18_2_6_56_439,"[BRW]W GetBrwInfoPtrByWapReqID,req_id=%d")
TRACE_MSG(BRW_CONTROL_3344_112_2_18_2_6_56_440,"[BRW]W BrwGetBrwInfoPtrByDLSessionID,session_id=%d")
TRACE_MSG(BRW_CONTROL_3356_112_2_18_2_6_56_441,"[BRW]W BrwGetBrwInfoPtrByDLSessionID,session_id=%d")
TRACE_MSG(BRW_CONTROL_3373_112_2_18_2_6_56_442,"[BRW]W BrwGetBrwInfoPtrByHttpReq,http_req_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_3386_112_2_18_2_6_56_443,"[BRW]W BrwGetBrwInfoPtrByHttpReq,http_req_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_3417_112_2_18_2_6_56_444,"[BRW]W BrwGetWapReq,req_id=0x%x,brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_3461_112_2_18_2_6_56_445,"[BRW]W BrwGetWapReqByDLSessionID:brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_3479_112_2_18_2_6_56_446,"[BRW]W BrwGetWapReqByHttpReq:brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_3972_112_2_18_2_6_57_447,"brw alloc wap request in BrwBackupWapRequest:%x")
TRACE_MSG(BRW_CONTROL_4031_112_2_18_2_6_57_448,"[BRW]BrwFreeWapReqStruct:is_cached_req!")
TRACE_MSG(BRW_CONTROL_4034_112_2_18_2_6_57_449,"[BRW]free wap request:0x%x")
TRACE_MSG(BRW_CONTROL_4096_112_2_18_2_6_57_450,"[BRW]BrwFreeWapReqStruct,BrwDestroyLayoutCtrl: 0x%x")
TRACE_MSG(BRW_CONTROL_4139_112_2_18_2_6_57_451,"BRW FreeMediaObjStruct media_obj_info_ptr = 0x%x")
TRACE_MSG(BRW_CONTROL_4264_112_2_18_2_6_58_452,"BRW HandleMsgCb,id:%d,result:%d,handler:%x")
TRACE_MSG(BRW_CONTROL_4290_112_2_18_2_6_58_453,"BRW HandleMsgCb,MMIPDP_ACTIVE_CNF,nsapi:%d")
TRACE_MSG(BRW_CONTROL_4322_112_2_18_2_6_58_454,"[BRW] ActiveNetwork is actived")
TRACE_MSG(BRW_CONTROL_4329_112_2_18_2_6_58_455,"[BRW]W ActiveNetwork brw_info_ptr PNULL")
TRACE_MSG(BRW_CONTROL_4345_112_2_18_2_6_58_456,"[BRW]W ActiveNetwork apn_ptr =0x%x, dualz_sys =%d")
TRACE_MSG(BRW_CONTROL_4374_112_2_18_2_6_58_457,"[BRW] DeactiveNetwork brw_info_ptr is null")
TRACE_MSG(BRW_CONTROL_4415_112_2_18_2_6_58_458,"[BRW]ProcNetLinkCnf,result=%d")
TRACE_MSG(BRW_CONTROL_4429_112_2_18_2_6_58_459,"BRW ProcNetLinkCnf:brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_4496_112_2_18_2_6_58_460,"[BRW]ProcNetLinkDisConnInd,brw_info_ptr:0x%x")
TRACE_MSG(BRW_CONTROL_4695_112_2_18_2_6_58_461,"[BRW]W BrwProcHttpErr,brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_4703_112_2_18_2_6_58_462,"[BRW]BrwProcHttpErr,state = 0x%x")
TRACE_MSG(BRW_CONTROL_4715_112_2_18_2_6_59_463,"[BRW]BrwProcHttpErr retry")
TRACE_MSG(BRW_CONTROL_4770_112_2_18_2_6_59_464,"[BRW]BrwProcHttpErr retry")
TRACE_MSG(BRW_CONTROL_4785_112_2_18_2_6_59_465,"[BRW]BrwProcHttpErr:media_obj_ptr PNULL")
TRACE_MSG(BRW_CONTROL_4805_112_2_18_2_6_59_466,"[BRW]W BrwCancelMediaReq,brw_info_ptr=0x%x,wap_req_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_4834_112_2_18_2_6_59_467,"[BRW]W BrwProcHttpAuth,brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_4900_112_2_18_2_6_59_468,"BrwFileCopyForWspDl brw_info_ptr=0")
TRACE_MSG(BRW_CONTROL_4905_112_2_18_2_6_59_469,"BrwFileCopyForWspDl copy err")
TRACE_MSG(BRW_CONTROL_4929_112_2_18_2_6_59_470,"BrwSetWspDlFileNm err, dl_file_name_ptr=0")
TRACE_MSG(BRW_CONTROL_4940_112_2_18_2_6_59_471,"BrwSetWspDlFileNm brw_info_ptr=0")
TRACE_MSG(BRW_CONTROL_4947_112_2_18_2_6_59_472,"[BRW]BrwSetWspDlFileNm,:transfer_encoding = %d")
TRACE_MSG(BRW_CONTROL_4951_112_2_18_2_6_59_473,"[BRW]BrwSetWspDlFileNm,:is_gzip = %d")
TRACE_MSG(BRW_CONTROL_4984_112_2_18_2_6_59_474,"[BRW]W BrwEnterFileNmCbf:brw_info_ptr=0x%x,wap_req_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_4996_112_2_18_2_6_59_475,"[BRW]BrwEnterFileNmCbf,file_name_len=%d")
TRACE_MSG(BRW_CONTROL_5006_112_2_18_2_6_59_476,"[BRW]BrwSetWspDlFileNm,:transfer_encoding = %d")
TRACE_MSG(BRW_CONTROL_5033_112_2_18_2_6_59_477,"[BRW]BrwEnterFileNmCbf,BrwSetHttpDLFileNm failed")
TRACE_MSG(BRW_CONTROL_5045_112_2_18_2_6_59_478,"[BRW]BrwEnterFileNmCbf,BrwSetWspDlFileNm failed")
TRACE_MSG(BRW_CONTROL_5067_112_2_18_2_6_59_479,"[BRW]W BrwEnterFileNmCbf:brw_info_ptr=0x%x,wap_req_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_5073_112_2_18_2_6_59_480,"[BRW]BrwEnterFileNmCbf,file_name_len=%d")
TRACE_MSG(BRW_CONTROL_5117_112_2_18_2_6_59_481,"[BRW]BrwGetCachePath:result =%d")
TRACE_MSG(BRW_CONTROL_5155_112_2_18_2_6_59_482,"[BRW]BrwGetCachePath:result =%d")
TRACE_MSG(BRW_CONTROL_5180_112_2_18_2_6_59_483,"[BRW]BrwGetCookiePath:result =%d")
TRACE_MSG(BRW_CONTROL_5195_112_2_18_2_6_59_484,"BRW BRW_ClearCookie instance =0")
TRACE_MSG(BRW_CONTROL_5201_112_2_18_2_6_59_485,"BRW BRW_ClearCookie brw_info_ptr =0")
TRACE_MSG(BRW_CONTROL_5261_112_2_18_2_7_0_486,"[BRW]BRW_ClearAllCache result_1 = %d")
TRACE_MSG(BRW_CONTROL_5263_112_2_18_2_7_0_487,"[BRW]BRW_ClearAllCache result_2 = %d")
TRACE_MSG(BRW_CONTROL_5283_112_2_18_2_7_0_488,"[BRW] BRW_ClearAllCookie result_1 = %d")
TRACE_MSG(BRW_CONTROL_5285_112_2_18_2_7_0_489,"[BRW] BRW_ClearAllCookie result_2 = %d")
TRACE_MSG(BRW_CONTROL_5307_112_2_18_2_7_0_490,"[BRW]W BrwOMADLConfirmCbf:brw_info_ptr=0x%x,wap_req_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_5313_112_2_18_2_7_0_491,"[BRW]BrwOMADLConfirmCbf,result =%d, file_name_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_5386_112_2_18_2_7_0_492,"[BRW]W BrwProcSSLCert,brw_info_ptr=0x%x")
TRACE_MSG(BRW_CONTROL_5411_112_2_18_2_7_0_493,"BRW_SetScreenProfile:brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_5471_112_2_18_2_7_0_494,"BRW_SetScreenProfile:brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_5637_112_2_18_2_7_0_495,"GetCurDisplayLayout:brw_info_ptr null")
TRACE_MSG(BRW_CONTROL_5644_112_2_18_2_7_0_496,"GetCurDisplayLayout,wap_request_ptr null")
TRACE_MSG(BRW_CONTROL_5648_112_2_18_2_7_0_497,"GetCurDisplayLayout,get snapshot")
TRACE_MSG(BRW_CONTROL_5836_112_2_18_2_7_1_498,"BrwFileCopyForWspDl brw_info_ptr=0")
TRACE_MSG(BRW_CONTROL_5841_112_2_18_2_7_1_499,"[BRW]BrwDecodeFileForDl:decode_result = %d")
TRACE_MSG(BRW_CONTROL_5933_112_2_18_2_7_1_500,"[BRW]W SetPublicProfile:malloc proxy addr failed,len=%d")
TRACE_MSG(BRW_CONTROL_5956_112_2_18_2_7_1_501,"[BRW]W SetPublicProfile:malloc ua_prof failed,len=%d")
TRACE_MSG(BRW_CONTROL_5984_112_2_18_2_7_1_502,"[BRW]W SetPublicProfile:malloc ua_prof failed,len=%d")
TRACE_MSG(BRW_CONTROL_6172_112_2_18_2_7_1_503,"[BRW]StopAndFreeBGsound")
TRACE_MSG(BRW_CONTROL_6176_112_2_18_2_7_1_504,"StopAndFreeBGsound,layout_handle=0x%x")
TRACE_MSG(BRW_CONTROL_6197_112_2_18_2_7_1_505,"StopAndFreeBGsound,wap_request_ptr null")
TRACE_MSG(BRW_CONTROL_6201_112_2_18_2_7_1_506,"StopAndFreeBGsound,get snapshot")
TRACE_MSG(BRW_CONTROL_6212_112_2_18_2_7_1_507,"StopAndFreeBGsound,cur_layout_handle=0x%x")
TRACE_MSG(BRW_CORE_ADAPTER_221_112_2_18_2_7_2_508,"Brw: timer=%dms start id=%d\n")
TRACE_MSG(BRW_CORE_ADAPTER_244_112_2_18_2_7_2_509,"Brw: timer id=%d\n")
TRACE_MSG(BRW_CORE_ADAPTER_323_112_2_18_2_7_2_510,"APP_Task Brw confirm WaitUserConfirm : begin init, time = %d")
TRACE_MSG(BRW_CORE_ADAPTER_431_112_2_18_2_7_3_511,"HandleWmlInputBoxWinMsg ::PNULL ==input_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_453_112_2_18_2_7_3_512,"HandleWmlInputBoxWinMsg ::PNULL ==input_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_763_112_2_18_2_7_3_513,"BrwGraphicsDrawHLine ::PNULL ==region_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_808_112_2_18_2_7_3_514,"BrwGraphicsDrawVLine ::PNULL ==region_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_956_112_2_18_2_7_4_515,"BrwGraphicsDrawLine ::PNULL ==region_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_1221_112_2_18_2_7_4_516,"BrwGraphicsFillRect ::PNULL ==region_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_1269_112_2_18_2_7_4_517,"BrwFillLcdRect ::PNULL ==rect_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_1363_112_2_18_2_7_4_518,"BrwDisplayDownloadingImage ::PNULL ==unicode_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_1501_112_2_18_2_7_5_519,"BrwDisplayDownloadingImage ::PNULL ==rect_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_1560_112_2_18_2_7_5_520,"[BRW]BrwDisplayFailedImage")
TRACE_MSG(BRW_CORE_ADAPTER_1565_112_2_18_2_7_5_521,"BrwDisplayFailedImage ::PNULL ==region_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_1690_112_2_18_2_7_5_522,"[BRW]BrwImageCalc:ret = %d, width =%d,height =%d")
TRACE_MSG(BRW_CORE_ADAPTER_1717_112_2_18_2_7_5_523,"[BRW]W BrwImageDisplayEx input invalid!")
TRACE_MSG(BRW_CORE_ADAPTER_1725_112_2_18_2_7_5_524,"[BRW]W BrwImageDisplayEx:target_width =%d, target_height =%d")
TRACE_MSG(BRW_CORE_ADAPTER_1756_112_2_18_2_7_5_525,"[BRW]W BrwImageDisplayEx:info_ptr->mime_type =%d")
TRACE_MSG(BRW_CORE_ADAPTER_1773_112_2_18_2_7_5_526,"[BRW]W BrwImageDisplayEx:alloc fail, in_param.target_buf_size =%d")
TRACE_MSG(BRW_CORE_ADAPTER_1789_112_2_18_2_7_5_527,"[BRW]BrwImageDisplayEx:is_need_zoom_in = %d")
TRACE_MSG(BRW_CORE_ADAPTER_1820_112_2_18_2_7_5_528,"[BRW]W BrwImageDisplayEx:ZOOM Alloc fail,zoom_in_data.dest_buf_size = %d")
TRACE_MSG(BRW_CORE_ADAPTER_1850_112_2_18_2_7_5_529,"[BRW]W BrwImageDisplayEx:IMG_DEC_GetFrame  decode_ret= %d")
TRACE_MSG(BRW_CORE_ADAPTER_1855_112_2_18_2_7_5_530,"[BRW]W BrwImageDisplayEx:IMG_DEC_Create decode_ret =%d")
TRACE_MSG(BRW_CORE_ADAPTER_1861_112_2_18_2_7_5_531,"[BRW]W BrwImageDisplayEx:IMG_DEC_Destroy decode_ret =%d")
TRACE_MSG(BRW_CORE_ADAPTER_1934_112_2_18_2_7_6_532,"[BRW]BrwImageDisplay:result = %d")
TRACE_MSG(BRW_CORE_ADAPTER_1964_112_2_18_2_7_6_533,"WmlPreSelectListControl ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_1992_112_2_18_2_7_6_534,"SetupWmlListControl ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2063_112_2_18_2_7_6_535,"WmlHandleSelectedFinish ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2073_112_2_18_2_7_6_536,"WmlHandleSelectedFinish ::PNULL ==item_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2129_112_2_18_2_7_6_537,"WmlHandleSelectWinMsg ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2160_112_2_18_2_7_6_538,"WmlHandleSelectWinMsg ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2180_112_2_18_2_7_6_539,"WmlHandleSelectWinMsg ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2213_112_2_18_2_7_6_540,"XhtmlPreSelectListControl ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2240_112_2_18_2_7_6_541,"SetupXhtmlListControl ::PNULL ==layout_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2264_112_2_18_2_7_6_542,"SetupXhtmlListControl ::PNULL ==option_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2314_112_2_18_2_7_6_543,"XhtmlHandleSelectWinMsg ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2324_112_2_18_2_7_6_544,"XhtmlHandleSelectWinMsg ::PNULL ==item_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2372_112_2_18_2_7_6_545,"[BRW]W XhtmlHandleSelectWinMsg ::PNULL==select_win_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2388_112_2_18_2_7_7_546,"XhtmlHandleSelectWinMsg ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2425_112_2_18_2_7_7_547,"XhtmlHandleSelectWinMsg ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2451_112_2_18_2_7_7_548,"XhtmlHandleSelectWinMsg ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2484_112_2_18_2_7_7_549,"ReadInputFormat ::PNULL ==method ||PNULL == input_mode_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2547_112_2_18_2_7_7_550,"HandleWmlInputBoxWinMsg ::PNULL ==input_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2600_112_2_18_2_7_7_551,"HandleWmlInputBoxWinMsg :MSG_CTL_OK:PNULL ==input_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2638_112_2_18_2_7_7_552,"[BRW]W HandleXhtmlInputBoxWinMsg ::PNULL ==input_win_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2683_112_2_18_2_7_7_553,"HandleXhtmlInputBoxWinMsg :MSG_CTL_OK:PNULL ==input_win_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2751_112_2_18_2_7_7_554,"WmlLayoutCreateEditWindow ::PNULL ==input_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_2846_112_2_18_2_7_7_555,"[BRW]W XhtmlLayoutCreateEditWindow:param error, layout_ptr=0x%x")
TRACE_MSG(BRW_CORE_ADAPTER_2852_112_2_18_2_7_7_556,"[BRW]W XhtmlLayoutCreateEditWindow ::input_win_ptr ALLOC FAIL")
TRACE_MSG(BRW_CORE_ADAPTER_2878_112_2_18_2_7_7_557,"[BRW]W XhtmlLayoutCreateEditWindow:handle error")
TRACE_MSG(BRW_CORE_ADAPTER_2950_112_2_18_2_7_8_558,"XhtmlOpenFormFileWinCallBack array size == 0")
TRACE_MSG(BRW_CORE_ADAPTER_2955_112_2_18_2_7_8_559,"XhtmlOpenFormFileWinCallBack failed")
TRACE_MSG(BRW_CORE_ADAPTER_3030_112_2_18_2_7_8_560,"XhtmlLayoutCreateEditWindow ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_3115_112_2_18_2_7_8_561,"XhtmlLayoutCreateSelectWindow ::PNULL ==select_ptr")
TRACE_MSG(BRW_CORE_ADAPTER_3122_112_2_18_2_7_8_562,"[BRW]W XhtmlLayoutCreateSelectWindow ::select_win_ptr ALLOC FAIL")
TRACE_MSG(BRW_CORE_ADAPTER_3139_112_2_18_2_7_8_563,"[BRW]W XhtmlLayoutCreateSelectWindow:handle error")
TRACE_MSG(BRW_CORE_ADAPTER_3160_112_2_18_2_7_8_564,"[BRW]W XhtmlLayoutCreateSelectWindow:GUILIST_CreateListBox error")
TRACE_MSG(BRW_CORE_ADAPTER_3195_112_2_18_2_7_8_565,"[BRW]W BrwPlayGif input invalid!")
TRACE_MSG(BRW_CORE_ADAPTER_3203_112_2_18_2_7_8_566,"[BRW]W BrwPlayGif:target_width =%d, target_height =%d")
TRACE_MSG(BRW_CORE_ADAPTER_3217_112_2_18_2_7_8_567,"[BRW]W BrwPlayGif:IMG_DEC_Create fail")
TRACE_MSG(BRW_CORE_ADAPTER_3233_112_2_18_2_7_8_568,"[BRW]W BrwPlayGif:target_ptr alloc fail")
TRACE_MSG(BRW_CORE_ADAPTER_3262_112_2_18_2_7_8_569,"[BRW]W BrwPlayGif:target_ptr alloc fail2")
TRACE_MSG(BRW_CORE_ADAPTER_3277_112_2_18_2_7_8_570,"[BRW]BrwPlayGif:is_need_zoom_in = %d")
TRACE_MSG(BRW_CORE_ADAPTER_3308_112_2_18_2_7_8_571,"[BRW]W BrwPlayGif:ZOOM Alloc fail,zoom_in_data.dest_buf_size = %d")
TRACE_MSG(BRW_CORE_ADAPTER_3581_112_2_18_2_7_9_572,"BrwZoomImage zoom_in_ptr || zoom_out_ptr is null")
TRACE_MSG(BRW_CORE_ADAPTER_3637_112_2_18_2_7_9_573,"[BRW]BrwZoomImage:zoom result is %d!")
TRACE_MSG(BRW_CORE_ADAPTER_3671_112_2_18_2_7_9_574,"[BRW]BrwGetAudioTypeByMime mime_type=%d")
TRACE_MSG(BRW_CORE_ADAPTER_3715_112_2_18_2_7_9_575,"[BRW]BrwPlayBgsoundCallBack, result=%d")
TRACE_MSG(BRW_CORE_ADAPTER_3732_112_2_18_2_7_9_576,"[BRW]BrwPlayBGsound, loop=%d")
TRACE_MSG(BRW_CORE_ADAPTER_3737_112_2_18_2_7_9_577,"[BRW]BrwPlayBGsound, play_times=%d, audio_type=%d")
TRACE_MSG(BRW_CORE_ADAPTER_3754_112_2_18_2_7_9_578,"[BRW]BrwPlayBGsound, MMIAPISET_IsPermitPlayRing not!")
TRACE_MSG(BRW_CORE_ADAPTER_3759_112_2_18_2_7_9_579,"[BRW]BrwPlayBGsound:ret=%d")
TRACE_MSG(BRW_CORE_ADAPTER_3769_112_2_18_2_7_9_580,"[BRW]BrwStopBGsound")
TRACE_MSG(BRW_DLINTERFACE_285_112_2_18_2_7_10_581,"[BRW]W DLGenFileName ::PNULL != file_name")
TRACE_MSG(BRW_DLINTERFACE_299_112_2_18_2_7_10_582,"[BRW]W DLGenFileName:(ext_name_len=%d) > (BRW_DL_SUFFIX_MAX_LEN=%d)")
TRACE_MSG(BRW_DLINTERFACE_305_112_2_18_2_7_10_583,"[BRW]W DLGenFileName:(ext_name_len=%d),(name_len=%d)")
TRACE_MSG(BRW_DLINTERFACE_311_112_2_18_2_7_10_584,"[BRW]DLGenFileName:name_len=%d")
TRACE_MSG(BRW_DLINTERFACE_515_112_2_18_2_7_11_585,"GetDownloadFileByURL i=%d, rtn_url_ext=%s")
TRACE_MSG(BRW_DLINTERFACE_640_112_2_18_2_7_11_586,"[BRW]GetMimeTypeByhttpHeader mime_type =%d")
TRACE_MSG(BRW_DLINTERFACE_660_112_2_18_2_7_11_587,"[BRW]W Brw_DLGetDefaultFileName:PNULL == file_name")
TRACE_MSG(BRW_DLINTERFACE_666_112_2_18_2_7_11_588,"[BRW]Brw_DLGetDefaultFileName GetFileNameByContentDispositon TURE")
TRACE_MSG(BRW_DLINTERFACE_674_112_2_18_2_7_11_589,"[BRW]Brw_DLGetDefaultFileName type =%d")
TRACE_MSG(BRW_DLINTERFACE_689_112_2_18_2_7_11_590,"[BRW]Brw_DLGetDefaultFileName default_file_wname_ptr ALLOC Fail")
TRACE_MSG(BRW_DLINTERFACE_694_112_2_18_2_7_11_591,"[BRW]Brw_DLGetDefaultFileName url_file_name_len err")
TRACE_MSG(BRW_DLINTERFACE_752_112_2_18_2_7_11_592,"[BRW]BrwDLGetMimeTypeBySuffix mime_type = %d")
TRACE_MSG(BRW_DLINTERFACE_772_112_2_18_2_7_11_593,"BrwGenerateDlTempName length exceed")
TRACE_MSG(BRW_DLINTERFACE_794_112_2_18_2_7_11_594,"[BRW]W BrwStartDL:req_ptr=0x%x,brw_info_ptr=0x%x")
TRACE_MSG(BRW_DLINTERFACE_800_112_2_18_2_7_11_595,"[BRW]BrwStartDL,:transfer_encoding = %d")
TRACE_MSG(BRW_DLINTERFACE_874_112_2_18_2_7_11_596,"[BRW]BrwStartDL:expired_time = %d")
TRACE_MSG(BRW_DLINTERFACE_878_112_2_18_2_7_11_597,"[BRW]BrwStartDL mime_type =%d")
TRACE_MSG(BRW_DLINTERFACE_926_112_2_18_2_7_11_598,"[BRW]BrwStartDL req.mime_type =%d")
TRACE_MSG(BRW_DLINTERFACE_941_112_2_18_2_7_11_599,"[BRW]W--BrwCancelDL:req_ptr=0x%x,brw_info_ptr=0x%x")
TRACE_MSG(BRW_DLINTERFACE_956_112_2_18_2_7_11_600,"[BRW]W--BrwFreeDLResource:req_ptr=0x%x,brw_info_ptr=0x%x")
TRACE_MSG(BRW_DLINTERFACE_968_112_2_18_2_7_11_601,"BrwFreeDLResource:isn't downloading state")
TRACE_MSG(BRW_DLINTERFACE_990_112_2_18_2_7_11_602,"[BRW]W BrwProcDLSuccess:req_ptr=0x%x,brw_info_ptr=0x%x")
TRACE_MSG(BRW_DLINTERFACE_1019_112_2_18_2_7_12_603,"[BRW]W BrwWspDownloadDataIn, invalid input,wap_req_ptr=%d,http_req_ptr=%d")
TRACE_MSG(BRW_DLINTERFACE_1026_112_2_18_2_7_12_604,"[BRW]W BrwWspDownloadDataIn ERR:ffs_handler=%d,buf_ptr=0x%x")
TRACE_MSG(BRW_DLINTERFACE_1055_112_2_18_2_7_12_605,"[BRW]W GetDrmSDExpiredTime http_header_ptr PNULL")
TRACE_MSG(BRW_DLINTERFACE_1061_112_2_18_2_7_12_606,"[BRW]GetDrmSDExpiredTime dest_str1=%s")
TRACE_MSG(BRW_DLINTERFACE_1080_112_2_18_2_7_12_607,"[BRW]W GetDrmSDExpiredTime ALLOC PNULL")
TRACE_MSG(BRW_DLINTERFACE_1085_112_2_18_2_7_12_608,"[BRW]GetDrmSDExpiredTime expired_time=%d")
TRACE_MSG(BRW_DLINTERFACE_1114_112_2_18_2_7_12_609,"[BRW]W BrwStartOMADL:req_ptr=0x%x,brw_info_ptr=0x%x")
TRACE_MSG(BRW_DLINTERFACE_1125_112_2_18_2_7_12_610,"[BRW]W MMIDL_StartOMADownload file_name_ptr PNULL or file_name_len =%d")
TRACE_MSG(BRW_DLINTERFACE_1172_112_2_18_2_7_12_611,"[BRW]W MMIDL_StartOMADownload ODM_StartDownload Failed oma_ret = %d")
TRACE_MSG(BRW_DLINTERFACE_1201_112_2_18_2_7_12_612,"[BRW]W--BrwOMADLResp:wap_req_ptr=0x%x,brw_info_ptr=0x%x")
TRACE_MSG(BRW_DLINTERFACE_1234_112_2_18_2_7_12_613,"[BRW]W BrwOMADLAuthResp:wap_req_ptr=0x%x,brw_info_ptr=0x%x")
TRACE_MSG(BRW_DLINTERFACE_1238_112_2_18_2_7_12_614,"[BRW]BrwOMADLAuthResp:result = %d")
TRACE_MSG(BRW_DLINTERFACE_1290_112_2_18_2_7_12_615,"[BRW]BrwOMADLAuthResp:cnf_ptr->result = %d")
TRACE_MSG(BRW_DLINTERFACE_1404_112_2_18_2_7_12_616,"[BRW] HandleODMDlConfirmationInd mime_type[%d] =%d")
TRACE_MSG(BRW_DLINTERFACE_1445_112_2_18_2_7_12_617,"[BRW] HandleODMDlConfirmationInd mime_type =%d")
TRACE_MSG(BRW_DLINTERFACE_1447_112_2_18_2_7_12_618,"[BRW] HandleODMDlConfirmationInd drm_content_mime_type=%d")
TRACE_MSG(BRW_DLINTERFACE_1481_112_2_18_2_7_12_619,"[BRW]W HandleODMDlConfirmationInd:session_id = 0x%x")
TRACE_MSG(BRW_DLINTERFACE_1535_112_2_18_2_7_13_620,"[BRW]W HandleODMDlAuthInd:session_id = 0x%x")
TRACE_MSG(BRW_DLINTERFACE_1559_112_2_18_2_7_13_621,"[BRW]HandleODMDlProgressInd:stage =%d")
TRACE_MSG(BRW_DLINTERFACE_1596_112_2_18_2_7_13_622,"[BRW]W HandleODMDlProgressInd:session_id = 0x%x")
TRACE_MSG(BRW_DLINTERFACE_1656_112_2_18_2_7_13_623,"[BRW]W HandleODMDlSuccessInd:session_id = 0x%x")
TRACE_MSG(BRW_DLINTERFACE_1681_112_2_18_2_7_13_624,"HandleODMDlErrorInd error_no =%d")
TRACE_MSG(BRW_DLINTERFACE_1700_112_2_18_2_7_13_625,"HandleODMDlErrorInd result =%d")
TRACE_MSG(BRW_DLINTERFACE_1739_112_2_18_2_7_13_626,"[BRW]HandleODMDlHeaderInd:time_out = %d")
TRACE_MSG(BRW_DLINTERFACE_1854_112_2_18_2_7_13_627,"[BRW]W BrwProcODMMsg param is PNULL msg_id = 0x%x")
TRACE_MSG(BRW_DLINTERFACE_1862_112_2_18_2_7_13_628,"[BRW]BrwProcODMMsg receive ODM_START_DOWNLOAD_CNF")
TRACE_MSG(BRW_DLINTERFACE_1867_112_2_18_2_7_13_629,"[BRW]BrwProcODMMsg receive ODM_CANCEL_DOWNLOAD_CNF")
TRACE_MSG(BRW_DLINTERFACE_1872_112_2_18_2_7_13_630,"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_CONFIRMATION_IND")
TRACE_MSG(BRW_DLINTERFACE_1877_112_2_18_2_7_13_631,"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_AUTH_IND")
TRACE_MSG(BRW_DLINTERFACE_1882_112_2_18_2_7_13_632,"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_PROGRESS_IND")
TRACE_MSG(BRW_DLINTERFACE_1887_112_2_18_2_7_13_633,"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_SUCCESS_IND")
TRACE_MSG(BRW_DLINTERFACE_1892_112_2_18_2_7_13_634,"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_ERROR_IND")
TRACE_MSG(BRW_DLINTERFACE_1897_112_2_18_2_7_13_635,"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_ERROR_IND")
TRACE_MSG(BRW_DLINTERFACE_1903_112_2_18_2_7_13_636,"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_GET_SESSION_LIST_CNF")
TRACE_MSG(BRW_DLINTERFACE_1908_112_2_18_2_7_13_637,"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_GET_SESSION_INFO_CNF")
TRACE_MSG(BRW_DLINTERFACE_1913_112_2_18_2_7_13_638,"[BRW]BrwProcODMMsg receive ODM_START_DOWNLOAD_CNF")
TRACE_MSG(BRW_DLINTERFACE_1918_112_2_18_2_7_13_639,"[BRW]BrwProcODMMsg receive ODM_DESUBSCRIBE_INDICATION_CNF")
TRACE_MSG(BRW_DLINTERFACE_1923_112_2_18_2_7_13_640,"[BRW]BrwProcODMMsg receive ODM_MANAGER_PROGRESS_IND")
TRACE_MSG(BRW_DLINTERFACE_1928_112_2_18_2_7_13_641,"[BRW]BrwProcODMMsg receive ODM_MANAGER_SUCCESS_IND")
TRACE_MSG(BRW_DLINTERFACE_1933_112_2_18_2_7_13_642,"[BRW]BrwProcODMMsg receive ODM_MANAGER_ERROR_IND")
TRACE_MSG(BRW_DLINTERFACE_1939_112_2_18_2_7_13_643,"[BRW]BrwProcODMMsg Not handle msg_id = 0x%x")
TRACE_MSG(BRW_DLINTERFACE_1993_112_2_18_2_7_13_644,"[BRW]W BrwSetDlTempFilename err, alloc fail")
TRACE_MSG(BRW_DLINTERFACE_2015_112_2_18_2_7_13_645,"[BRW]BrwDelDlTempFile:result = %d")
TRACE_MSG(BRW_HISTORY_62_112_2_18_2_7_14_646,"BrwNavHisAddCache: cathed request error")
TRACE_MSG(BRW_HISTORY_68_112_2_18_2_7_14_647,"BrwNavHisAddCache: error 2")
TRACE_MSG(BRW_HISTORY_92_112_2_18_2_7_14_648,"brw BrwNavHisAddCache, free cathe")
TRACE_MSG(BRW_HISTORY_124_112_2_18_2_7_14_649,"brw BrwNavClearCache free cathe")
TRACE_MSG(BRW_HISTORY_183_112_2_18_2_7_14_650,"[BRW]NavHisFreeCurCacheReq,BrwDestroyLayoutCtrl: 0x%x")
TRACE_MSG(BRW_HISTORY_186_112_2_18_2_7_14_651,"brw free wap request cathe:%x")
TRACE_MSG(BRW_HISTORY_224_112_2_18_2_7_15_652,"BrwVisitHisAddItem:url len=%d")
TRACE_MSG(BRW_HISTORY_230_112_2_18_2_7_15_653,"[BRW]BrwVisitHisAddItem:s_brw_visit_his_que_ptr null")
TRACE_MSG(BRW_HISTORY_284_112_2_18_2_7_15_654,"[BRW]BRW_VisitHisDelItem:index =%d")
TRACE_MSG(BRW_HISTORY_288_112_2_18_2_7_15_655,"[BRW]BRW_VisitHisDelItem ERROR:index =%d")
TRACE_MSG(BRW_HISTORY_314_112_2_18_2_7_15_656,"[BRW]BRW_VisitHisDel:s_brw_visit_his_que_ptr null")
TRACE_MSG(BRW_HISTORY_364_112_2_18_2_7_15_657,"BRW_VisitHisInit:read_len =%d")
TRACE_MSG(BRW_HISTORY_397_112_2_18_2_7_15_658,"[BRW]W BRW_VisitHisGenIndexArr:index_arr_ptr=0x%x")
TRACE_MSG(BRW_HISTORY_454_112_2_18_2_7_15_659,"[BRW]W BRW_VisitHisGetTotal:error")
TRACE_MSG(BRW_HISTORY_476_112_2_18_2_7_15_660,"BRW]W BRW_VisitHisGetHisInfoByIndex:index=%d")
TRACE_MSG(BRW_HISTORY_497_112_2_18_2_7_15_661,"brw FreeNavHisList free cathe")
TRACE_MSG(BRW_HISTORY_548_112_2_18_2_7_15_662,"BrwNavHisAddItem,%s")
TRACE_MSG(BRW_HISTORY_555_112_2_18_2_7_15_663,"[BRW]BrwNavHisAddItem:url_len=%d")
TRACE_MSG(BRW_HISTORY_569_112_2_18_2_7_15_664,"[BRW]BrwNavHisAddItem:malloc failed")
TRACE_MSG(BRW_HISTORY_595_112_2_18_2_7_16_665,"[BRW]BrwNavHisAddItem:malloc failed")
TRACE_MSG(BRW_HISTORY_685_112_2_18_2_7_16_666,"BrwNavHisForward,total num:%d,cur:%s")
TRACE_MSG(BRW_HISTORY_699_112_2_18_2_7_16_667,"BrwNavHisBackward,total num:%d,cur:%s")
TRACE_MSG(BRW_HISTORY_799_112_2_18_2_7_16_668,"[BRW]BRWUtil_ReadFileSyn:device null")
TRACE_MSG(BRW_HISTORY_819_112_2_18_2_7_16_669,"[BRW]BRW_ClearVisitHis result =%d")
TRACE_MSG(BRW_HTTPINTERFACE_200_112_2_18_2_7_17_670,"BrwInitHttpSession")
TRACE_MSG(BRW_HTTPINTERFACE_231_112_2_18_2_7_17_671,"BRW PASSWORD =%s")
TRACE_MSG(BRW_HTTPINTERFACE_233_112_2_18_2_7_17_672,"BRW user name =%s")
TRACE_MSG(BRW_HTTPINTERFACE_292_112_2_18_2_7_17_673,"BrwCloseHttpSession")
TRACE_MSG(BRW_HTTPINTERFACE_303_112_2_18_2_7_17_674,"[BRW]BrwCloseHttpSession: unsupported protocol")
TRACE_MSG(BRW_HTTPINTERFACE_353_112_2_18_2_7_17_675,"[BRW]BrwProcErrInd")
TRACE_MSG(BRW_HTTPINTERFACE_365_112_2_18_2_7_17_676,"[BRW]BrwProcErrInd http_req_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_394_112_2_18_2_7_17_677,"[BRW]W ProcRequestIDInd http_req_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_400_112_2_18_2_7_17_678,"[BRW]ProcRequestIDInd request_id=0x%x, context_id=0x%x, app_instance=0x%x")
TRACE_MSG(BRW_HTTPINTERFACE_411_112_2_18_2_7_17_679,"[BRW]ProcRequestIDInd profile.protocol =%d")
TRACE_MSG(BRW_HTTPINTERFACE_415_112_2_18_2_7_17_680,"[BRWW ProcRequestIDInd brw_info_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_438_112_2_18_2_7_17_681,"[BRW]ProcNetProgressInd http_req_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_476_112_2_18_2_7_17_682,"[BRW]ProcRedirectInd http_req_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_518_112_2_18_2_7_17_683,"[BRW]ProcNeedAuthInd")
TRACE_MSG(BRW_HTTPINTERFACE_539_112_2_18_2_7_17_684,"[BRW]ProcNeedAuthInd http_req_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_673_112_2_18_2_7_18_685,"[BRW]W BrwSendHttpRequest:file_name_len=%d")
TRACE_MSG(BRW_HTTPINTERFACE_683_112_2_18_2_7_18_686,"SetPostRequestParam,buf:%x,len:%d")
TRACE_MSG(BRW_HTTPINTERFACE_725_112_2_18_2_7_18_687,"[BRW]W SetPostRequestParam:file_name_len=%d")
TRACE_MSG(BRW_HTTPINTERFACE_770_112_2_18_2_7_18_688,"[BRW]W BrwSendHttpRequest:param =0x%x!")
TRACE_MSG(BRW_HTTPINTERFACE_780_112_2_18_2_7_18_689,"[BRW]BrwSendHttpRequest:BRWList_AppendObject req=0x%x!")
TRACE_MSG(BRW_HTTPINTERFACE_784_112_2_18_2_7_18_690,"BrwSendHttpRequest:BRWList_AppendObject failed!")
TRACE_MSG(BRW_HTTPINTERFACE_850_112_2_18_2_7_18_691,"[BRW]W BrwSendHttpRequest:unsupport method=%d!")
TRACE_MSG(BRW_HTTPINTERFACE_858_112_2_18_2_7_18_692,"BrwSendHttpRequest:malloc req failed!")
TRACE_MSG(BRW_HTTPINTERFACE_895_112_2_18_2_7_18_693,"BrwFreeHttpRequest:req null")
TRACE_MSG(BRW_HTTPINTERFACE_911_112_2_18_2_7_18_694,"BrwFreeHttpRequest:req=0x%x")
TRACE_MSG(BRW_HTTPINTERFACE_942_112_2_18_2_7_18_695,"[BRW] req_id:%d")
TRACE_MSG(BRW_HTTPINTERFACE_953_112_2_18_2_7_18_696,"[BRW]wap_req")
TRACE_MSG(BRW_HTTPINTERFACE_958_112_2_18_2_7_18_697,"[BRW]media")
TRACE_MSG(BRW_HTTPINTERFACE_969_112_2_18_2_7_18_698,"ProcHeaderInd ind->rsp_header_info.response_code:%d")
TRACE_MSG(BRW_HTTPINTERFACE_975_112_2_18_2_7_18_699,"ProcDataInd:wap_req_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_985_112_2_18_2_7_18_700,"[BRW]ProcHeaderInd content_type_ptr = %s")
TRACE_MSG(BRW_HTTPINTERFACE_988_112_2_18_2_7_18_701,"[BRW]ProcHeaderInd mime_type =%d")
TRACE_MSG(BRW_HTTPINTERFACE_1021_112_2_18_2_7_18_702,"[BRW]ProcHeaderInd content_length =%d")
TRACE_MSG(BRW_HTTPINTERFACE_1030_112_2_18_2_7_18_703,"[BRW]ProcHeaderInd mime_type(after) =%d, response_code= %d")
TRACE_MSG(BRW_HTTPINTERFACE_1049_112_2_18_2_7_18_704,"[BRW]malloc charset failed")
TRACE_MSG(BRW_HTTPINTERFACE_1131_112_2_18_2_7_19_705,"[BRW]ProcDataInd:http_req_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_1137_112_2_18_2_7_19_706,"ProcDataInd:brw_info_ptr =0x%x")
TRACE_MSG(BRW_HTTPINTERFACE_1144_112_2_18_2_7_19_707,"ProcDataInd:wap_req_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_1172_112_2_18_2_7_19_708,"[BRW]ProcDataInd,now_len=%d,data_len=%d,max_len=%d")
TRACE_MSG(BRW_HTTPINTERFACE_1237_112_2_18_2_7_19_709,"ProcGetCnf:brw_info_ptr =0x%x,http_req_ptr:%x")
TRACE_MSG(BRW_HTTPINTERFACE_1267_112_2_18_2_7_19_710,"ProcGetCnf:brw_info_ptr =0x%x")
TRACE_MSG(BRW_HTTPINTERFACE_1271_112_2_18_2_7_19_711,"brw ProcPostCnf cnf->result=%d")
TRACE_MSG(BRW_HTTPINTERFACE_1464_112_2_18_2_7_19_712,"[BRW]W SendAuthRsp: unsupported protocol")
TRACE_MSG(BRW_HTTPINTERFACE_1492_112_2_18_2_7_19_713,"[BRW]BrwHttpAuthResp:Cancel state =%d")
TRACE_MSG(BRW_HTTPINTERFACE_1524_112_2_18_2_7_19_714,"[BRW]W BrwHttpAuthResp:brw_info_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_1529_112_2_18_2_7_19_715,"[BRW]W BrwHttpAuthResp:http_req_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_1560_112_2_18_2_7_20_716,"BrwClearCache error: unsupported protocol")
TRACE_MSG(BRW_HTTPINTERFACE_1595_112_2_18_2_7_20_717,"BrwClearCookie error: unsupported protocol")
TRACE_MSG(BRW_HTTPINTERFACE_1630_112_2_18_2_7_20_718,"[BRW]ProcSSLCertInd")
TRACE_MSG(BRW_HTTPINTERFACE_1644_112_2_18_2_7_20_719,"[BRW]ProcSSLCertInd http_req_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_1685_112_2_18_2_7_20_720,"[BRW]BrwHttpSSLCertResp: invalid protocol")
TRACE_MSG(BRW_HTTPINTERFACE_1697_112_2_18_2_7_20_721,"[BRW]BrwHttpSSLCertResp:brw_info_ptr null")
TRACE_MSG(BRW_HTTPINTERFACE_1716_112_2_18_2_7_20_722,"[BRW]W ProcHttpErr http err=%d")
TRACE_MSG(BRW_HTTPINTERFACE_1776_112_2_18_2_7_20_723,"BRW ProcInitSessionCnf, the session inited has been destroyed!")
TRACE_MSG(BRW_HTTPINTERFACE_1792_112_2_18_2_7_20_724,"BrwProcHttpMsg HTTP_SIG_INIT_CNF,context_id:%x")
TRACE_MSG(BRW_HTTPINTERFACE_1807_112_2_18_2_7_20_725,"BrwProcHttpMsg HTTP_SIG_CLOSE_CNF")
TRACE_MSG(BRW_HTTPINTERFACE_1828_112_2_18_2_7_20_726,"BrwProcHttpMsg HTTP_SIG_CANCEL_CNF")
TRACE_MSG(BRW_HTTPINTERFACE_1843_112_2_18_2_7_20_727,"BrwProcHttpMsg HTTP_SIG_DATA_IND")
TRACE_MSG(BRW_HTTPINTERFACE_1858_112_2_18_2_7_20_728,"BrwProcHttpMsg HTTP_SIG_ERROR_IND")
TRACE_MSG(BRW_HTTPINTERFACE_1869_112_2_18_2_7_20_729,"BrwProcHttpMsg HTTP_SIG_GET_CNF,context_id:%x")
TRACE_MSG(BRW_HTTPINTERFACE_1874_112_2_18_2_7_20_730,"BrwProcHttpMsg HTTP_SIG_GET_CNF")
TRACE_MSG(BRW_HTTPINTERFACE_1889_112_2_18_2_7_20_731,"BrwProcHttpMsg HTTP_SIG_HEADER_IND")
TRACE_MSG(BRW_HTTPINTERFACE_1904_112_2_18_2_7_20_732,"BrwProcHttpMsg HTTP_SIG_NEED_AUTH_IND")
TRACE_MSG(BRW_HTTPINTERFACE_1936_112_2_18_2_7_20_733,"BrwProcHttpMsg HTTP_SIG_POST_CNF")
TRACE_MSG(BRW_HTTPINTERFACE_1951_112_2_18_2_7_20_734,"BrwProcHttpMsg HTTP_SIG_REDIRECT_IND")
TRACE_MSG(BRW_HTTPINTERFACE_1966_112_2_18_2_7_20_735,"BrwProcHttpMsg HTTP_SIG_REQUEST_ID_IND")
TRACE_MSG(BRW_HTTPINTERFACE_1974_112_2_18_2_7_20_736,"BrwProcHttpMsg HTTP_SIG_SSL_CERT_UNTRUST_IND")
TRACE_MSG(BRW_INPUT_62_112_2_18_2_7_21_737,"BRW_InputHisInit: data error i =%d,his_num=%d")
TRACE_MSG(BRW_INPUT_65_112_2_18_2_7_21_738,"BRW_InputHisInit:read_len =%d")
TRACE_MSG(BRW_INPUT_114_112_2_18_2_7_21_739,"[BRW]BRWUtil_ReadFileSyn:device null")
TRACE_MSG(BRW_INPUT_134_112_2_18_2_7_21_740,"[BRW]BRW_ClearInputHis result =%d")
TRACE_MSG(BRW_INPUT_192_112_2_18_2_7_21_741,"BrwInputHisAddItem:s_brw_input_his_ptr->his_num =%d")
TRACE_MSG(BRW_INPUT_264_112_2_18_2_7_21_742,"BRW_GetMatchInputHisUrl:index =%d, his_num=%d")
TRACE_MSG(BRW_PLINTERFACE_86_112_2_18_2_7_22_743,"BRWPL_IsFolderExist,full_path_len=%d")
TRACE_MSG(BRW_PLINTERFACE_119_112_2_18_2_7_22_744,"SplitFullPath, err full_path_ptr length err")
TRACE_MSG(BRW_PLINTERFACE_137_112_2_18_2_7_22_745,"SplitFullPath, err full_path_ptr path=0")
TRACE_MSG(BRW_PLINTERFACE_166_112_2_18_2_7_22_746,"BRWPL_FileCopy err BRWPL_GetFileSize failed!")
TRACE_MSG(BRW_PLINTERFACE_171_112_2_18_2_7_22_747,"BRWPL_FileCopy err src_file_size=0!")
TRACE_MSG(BRW_PLINTERFACE_174_112_2_18_2_7_22_748,"BRWPL_FileCopy src_file_size=%d")
TRACE_MSG(BRW_PLINTERFACE_179_112_2_18_2_7_22_749,"BRWPL_FileCopy err creat src_file_handle fail!")
TRACE_MSG(BRW_PLINTERFACE_185_112_2_18_2_7_22_750,"BRWPL_FileCopy err creat copyed_file_handle fail!")
TRACE_MSG(BRW_PLINTERFACE_203_112_2_18_2_7_22_751,"BRWPL_FileCopy  err alloc exchange buffer failed!")
TRACE_MSG(BRW_PLINTERFACE_208_112_2_18_2_7_22_752,"BRWPL_FileCopy err SFS_ReadFile fail!")
TRACE_MSG(BRW_PLINTERFACE_213_112_2_18_2_7_22_753,"BRWPL_FileCopy err read_len=0!")
TRACE_MSG(BRW_PLINTERFACE_219_112_2_18_2_7_22_754,"BRWPL_FileCopy err SFS_ReadFile fail!")
TRACE_MSG(BRW_PLINTERFACE_260_112_2_18_2_7_22_755,"BRWPL_CreateDir:full_path_len=%d")
TRACE_MSG(BRW_PLINTERFACE_276_112_2_18_2_7_22_756,"brw BRWPL_DeleteDir full_path_ptr fail!")
TRACE_MSG(BRW_PLINTERFACE_282_112_2_18_2_7_22_757,"brw BRWPL_DeleteDir find_path_ptr fail!")
TRACE_MSG(BRW_PLINTERFACE_306_112_2_18_2_7_22_758,"brw BRWPL_DeleteDir BRWPL_DeleteDir fail!")
TRACE_MSG(BRW_PLINTERFACE_318_112_2_18_2_7_22_759,"brw BRWPL_DeleteDir temp_path_ptr fail!")
TRACE_MSG(BRW_PLINTERFACE_330_112_2_18_2_7_22_760,"brw BRWPL_DeleteDir SFS_DeleteFile fail!")
TRACE_MSG(BRW_PLINTERFACE_445_112_2_18_2_7_23_761,"BRWPL_AppendWriteFileSync err:%d")
TRACE_MSG(BRW_PLINTERFACE_529_112_2_18_2_7_23_762,"BRWPL_IsFileExist,full_path_len=%d")
TRACE_MSG(BRW_PLINTERFACE_575_112_2_18_2_7_23_763,"BRWPL_GetFileSize,full_path_len=%d")
TRACE_MSG(BRW_PLINTERFACE_615_112_2_18_2_7_23_764,"BRWPL_ReadFileSyn,full_path_len=%d,buf_ptr=0x%x,buf_max_len=%d,read_len_ptr=0x%x")
TRACE_MSG(BRW_PLINTERFACE_635_112_2_18_2_7_23_765,"BRWPL_WriteFileSyn,full_path_len=%d,to_write_len=%d,buf_ptr=0x%x")
TRACE_MSG(BRW_PLINTERFACE_658_112_2_18_2_7_23_766,"[BRW]---waning-----")
TRACE_MSG(BRW_PLINTERFACE_675_112_2_18_2_7_23_767,"BRW_Memcpy dest=%x,src=%x,size=%x")
TRACE_MSG(BRW_PLINTERFACE_688_112_2_18_2_7_23_768,"BRW_Memset:dest=%x,size=%x")
TRACE_MSG(BRW_PLINTERFACE_705_112_2_18_2_7_23_769,"BRW_WSTRLEN str null")
TRACE_MSG(BRW_PLINTERFACE_726_112_2_18_2_7_23_770,"[BRW]W BRW_WstrCat :dest_ptr =0x%x")
TRACE_MSG(BRW_PLINTERFACE_755_112_2_18_2_7_23_771,"BRW_Strlen,string null!")
TRACE_MSG(BRW_PLINTERFACE_769_112_2_18_2_7_23_772,"BRW_Strcpy,dst=%x,src=%x")
TRACE_MSG(BRW_PLINTERFACE_783_112_2_18_2_7_23_773,"BRW_Strcpy,dst=%x,src=%x")
TRACE_MSG(BRW_PLINTERFACE_797_112_2_18_2_7_23_774,"BRW_Strncpy,dst=%x,src=%x")
TRACE_MSG(BRW_PLINTERFACE_806_112_2_18_2_7_23_775,"BRW_Strcmp,string=%x,str=%x")
TRACE_MSG(BRW_PLINTERFACE_816_112_2_18_2_7_23_776,"BRW_Strcmp,string=%x,str=%x")
TRACE_MSG(BRW_PLINTERFACE_826_112_2_18_2_7_23_777,"BRW_Stricmp,string=%x,str=%x")
TRACE_MSG(BRW_PLINTERFACE_836_112_2_18_2_7_23_778,"BRW_Strnicmp,string=%x,str=%x")
TRACE_MSG(BRW_PLINTERFACE_848_112_2_18_2_7_23_779,"BRW_Strstr,string=%x,str=%x")
TRACE_MSG(BRW_PLINTERFACE_879_112_2_18_2_7_23_780,"BRW_Strchr,str=%x,c=%c")
TRACE_MSG(BRW_PLINTERFACE_888_112_2_18_2_7_23_781,"BRW_Atoi,invalid param")
TRACE_MSG(BRW_PLINTERFACE_901_112_2_18_2_7_23_782,"BRW_TailStricmp,long_str=%x,tail=%x")
TRACE_MSG(BRW_PLINTERFACE_921_112_2_18_2_7_23_783,"BRW_TailStricmp,long_str=%x,tail=%x")
TRACE_MSG(BRW_PLINTERFACE_954_112_2_18_2_7_24_784,"[BRW]W BRWMem_DeleteMemHandle: delete mem handle fail,err:%x")
TRACE_MSG(BRW_PLINTERFACE_978_112_2_18_2_7_24_785,"BRWMem_DeleteMemHandle:Mem Handle has been deleted!")
TRACE_MSG(BRW_PLINTERFACE_1007_112_2_18_2_7_24_786,"BRWMem_CreatMemHandle: size=%d")
TRACE_MSG(BRW_PLINTERFACE_1045_112_2_18_2_7_24_787,"[BRW]BRWMem_CreatMemHandle:Mem Handle has been initiated!")
TRACE_MSG(BRW_PLINTERFACE_1146_112_2_18_2_7_24_788,"BRW MEM leak print start:\n")
TRACE_MSG(BRW_PLINTERFACE_1151_112_2_18_2_7_24_789,"%d %s/%d 0x%x")
TRACE_MSG(BRW_PLINTERFACE_1156_112_2_18_2_7_24_790,"BRW MEM leak print end:\n")
TRACE_MSG(BRW_PLINTERFACE_1217_112_2_18_2_7_24_791,"BRWMem_MallocEx,malloc failed,size:%d")
TRACE_MSG(BRW_PLINTERFACE_1306_112_2_18_2_7_24_792,"BRWPL_WstrToStr:dst_ptr null")
TRACE_MSG(BRW_PLINTERFACE_1331_112_2_18_2_7_24_793,"[BRW]W BRWPL_WstrStrnCatWithAsciiStr :dest_ptr =0x%x")
TRACE_MSG(BRW_PLINTERFACE_1479_112_2_18_2_7_25_794,"[BRW]W BRWPL_GetValidDevSysBrwRootForWrite:dst_path_name_ptr alloc fail")
TRACE_MSG(BRW_PLINTERFACE_1493_112_2_18_2_7_25_795,"[BRW]BRWPL_GetValidDevSysBrwRoot result =%d, dev =%d")
TRACE_MSG(BRW_SNAPSHOT_272_112_2_18_2_7_25_796,"BrwSnapshotCreatFolder fail")
TRACE_MSG(BRW_SNAPSHOT_459_112_2_18_2_7_26_797,"BRW BrwCallbackSaveResult, delete the folder fail when save is failed!")
TRACE_MSG(BRW_SNAPSHOT_466_112_2_18_2_7_26_798,"BRW BrwCallbackSaveResult, delete the file fail when save is failed!")
TRACE_MSG(BRW_SNAPSHOT_506_112_2_18_2_7_26_799,"[BRW]BRW_SnapshotHandleFfsMsg,msg_id:%d")
TRACE_MSG(BRW_SNAPSHOT_541_112_2_18_2_7_26_800,"BRW_SNAPSHOT_FFS_PAGE_SAVE_CNF,is_succ:%d")
TRACE_MSG(BRW_SNAPSHOT_571_112_2_18_2_7_26_801,"BRW_SNAPSHOT_FFS_CSS_SAVE_CNF,is_succ:%d")
TRACE_MSG(BRW_SNAPSHOT_593_112_2_18_2_7_26_802,"BRW_SNAPSHOT_FFS_IMAGE_SAVE_CNF,is_succ:%d")
TRACE_MSG(BRW_SNAPSHOT_604_112_2_18_2_7_26_803,"BRW_SNAPSHOT_FFS_IMAGE_SAVE_ONE_CNF,is_succ:%d")
TRACE_MSG(BRW_SNAPSHOT_619_112_2_18_2_7_26_804,"BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF,is_succ:%d")
TRACE_MSG(BRW_SNAPSHOT_626_112_2_18_2_7_26_805,"BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF: file_size = %d")
TRACE_MSG(BRW_SNAPSHOT_705_112_2_18_2_7_26_806,"BRW_SNAPSHOT_FFS_IMAGE_OPEN_ONE_CNF,is_succ:%d")
TRACE_MSG(BRW_SNAPSHOT_777_112_2_18_2_7_26_807,"brw snapshot open: %s")
TRACE_MSG(BRW_SNAPSHOT_900_112_2_18_2_7_26_808,"[BRW]BrwParseOfflineWebPage,BrwDestroyLayoutCtrl: 0x%x")
TRACE_MSG(BRW_SNAPSHOT_1249_112_2_18_2_7_27_809,"BRW_SaveCurWebPage,BrwGenerateCssInfo fail")
TRACE_MSG(BRW_SNAPSHOT_1255_112_2_18_2_7_27_810,"BRW_SaveCurWebPage,BrwGenerateImgInfo fail")
TRACE_MSG(BRW_SNAPSHOT_1261_112_2_18_2_7_27_811,"BRW_SaveCurWebPage,BrwSnapshotGenerateAllFileFolder fail")
TRACE_MSG(BRW_SNAPSHOT_1267_112_2_18_2_7_27_812,"BRW_SaveCurWebPage,BrwFfsWritePageContent fail")
TRACE_MSG(BRW_SNAPSHOT_1294_112_2_18_2_7_27_813,"BrwSnapshotDeleteWebPage, read file head len error")
TRACE_MSG(BRW_SNAPSHOT_1299_112_2_18_2_7_27_814,"BrwSnapshotDeleteWebPage, error file_head_len==0")
TRACE_MSG(BRW_SNAPSHOT_1305_112_2_18_2_7_27_815,"BrwSnapshotDeleteWebPage, file_head_buf_ptr alloc error")
TRACE_MSG(BRW_SNAPSHOT_1311_112_2_18_2_7_27_816,"BrwSnapshotDeleteWebPage, read file head error")
TRACE_MSG(BRW_SNAPSHOT_1358_112_2_18_2_7_27_817,"BrwSnapshotDeleteWebPage, no image folder")
TRACE_MSG(BRW_SNAPSHOT_1428_112_2_18_2_7_28_818,"[BRW]BrwDestroyOpenInfo,BrwDestroyLayoutCtrl: 0x%x")
TRACE_MSG(BRW_SNAPSHOT_1452_112_2_18_2_7_28_819,"BrwGetPageMimeType: type = %d")
TRACE_MSG(BRW_SNAPSHOT_1510_112_2_18_2_7_28_820,"BrwGetPageUrl: charset_len = %d")
TRACE_MSG(BRW_SNAPSHOT_1516_112_2_18_2_7_28_821,"BrwGetPageUrl: url_len = %d")
TRACE_MSG(BRW_SNAPSHOT_1538_112_2_18_2_7_28_822,"BrwGetPageCharset: charset_len = %d")
TRACE_MSG(BRW_SNAPSHOT_1589_112_2_18_2_7_28_823,"BRW_SnapshotParseOfflinePage no mem,g_snapshot_open_info.full_file_path_ptr")
TRACE_MSG(BRW_SNAPSHOT_1623_112_2_18_2_7_28_824,"BRW_SnapshotOfflineBrowse,ERROR:file size=0")
TRACE_MSG(BRW_SNAPSHOT_1631_112_2_18_2_7_28_825,"BRW_SnapshotOfflineBrowse,ERROR:alloc file buffer fail")
TRACE_MSG(BRW_SNAPSHOT_1641_112_2_18_2_7_28_826,"BRW_SnapshotOfflineBrowse,ERROR:BRWPL_ReadFileAsync fail")
TRACE_MSG(BRW_SNAPSHOT_1692_112_2_18_2_7_28_827,"BRW_SnapshotOfflineBrowse,ERROR:alloc file buffer fail")
TRACE_MSG(BRW_SNAPSHOT_1703_112_2_18_2_7_28_828,"BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF: file_size = %d")
TRACE_MSG(BRW_SNAPSHOT_2033_112_2_18_2_7_29_829,"BRW_SnapshotRenameFile,rename fail!")
TRACE_MSG(BRW_SNAPSHOT_2118_112_2_18_2_7_29_830,"BRW_DisplayPage,brw_info_ptr null")
TRACE_MSG(BRW_SNAPSHOT_2131_112_2_18_2_7_29_831,"BRW_GetPageImageUrl error: LAYOUT PNULL")
TRACE_MSG(BRW_SNAPSHOT_2178_112_2_18_2_7_29_832,"BRW_DisplayPage,brw_info_ptr null")
TRACE_MSG(BRW_SNAPSHOT_2191_112_2_18_2_7_29_833,"BRW_SavePageImage error: LAYOUT PNULL")
TRACE_MSG(BRW_SNAPSHOT_2196_112_2_18_2_7_29_834,"BRW_SavePageImage error: BrwGetImgBuf error")
TRACE_MSG(BRW_SNAPSHOT_2207_112_2_18_2_7_29_835,"BRW_SavePageImage error: BRWPL_WriteFileAsync error")
TRACE_MSG(BRW_UTILITY_53_112_2_18_2_7_30_836,"BRWList_Construct PNULL == list_ptr")
TRACE_MSG(BRW_UTILITY_203_112_2_18_2_7_30_837,"[BRW]W BRWList_RemoveObject,list_ptr=%x,object_ptr=%x")
TRACE_MSG(BRW_UTILITY_329_112_2_18_2_7_30_838,"[BRW]BRWList_SearchObject list_ptr=%x,object_prt=%x,cmp=%x")
TRACE_MSG(BRW_UTILITY_354_112_2_18_2_7_30_839,"[BRW]BRWList_SearchObject list_ptr=%x")
TRACE_MSG(BRW_UTILITY_680_112_2_18_2_7_31_840,"[BRW]BRWUtil_GetDefaultPath:device null")
TRACE_MSG(BRW_UTILITY_728_112_2_18_2_7_31_841,"[BRW]W BRWUtil_ReadFileSyn:file_name_ptr=0x%x,buf_ptr=0x%x")
TRACE_MSG(BRW_UTILITY_734_112_2_18_2_7_31_842,"[BRW]BRWUtil_ReadFileSyn:device null")
TRACE_MSG(BRW_UTILITY_784_112_2_18_2_7_31_843,"[BRW]W BRWUtil_ReadFileSyn:file_name_ptr=0x%x,buf_ptr=0x%x")
TRACE_MSG(BRW_UTILITY_790_112_2_18_2_7_31_844,"[BRW]BRWUtil_ReadFileSyn:device null")
TRACE_MSG(BRW_UTILITY_835_112_2_18_2_7_31_845,"[BRW]W BRWUtil_GetFileSize:file_name_ptr=0x%x,size_ptr=0x%x")
TRACE_MSG(BRW_UTILITY_842_112_2_18_2_7_31_846,"[BRW]BRWUtil_GetFileSize:device null")
TRACE_MSG(BRW_UTILITY_886_112_2_18_2_7_31_847,"[BRW]W BRWUtil_DelFileSyn:file_name_ptr=0x%x")
TRACE_MSG(BRW_UTILITY_892_112_2_18_2_7_31_848,"[BRW]BRWUtil_DelFileSyn:device null")
TRACE_MSG(BRW_UTILITY_932_112_2_18_2_7_31_849,"[BRW]BRWUtil_CreateDir:device null")
TRACE_MSG(BRW_UTILITY_978_112_2_18_2_7_32_850,"[BRW]BRWUtil_DecodeGzip src_len =0x%x")
TRACE_MSG(BRW_UTILITY_995_112_2_18_2_7_32_851,"[BRW]BRWUtil_DecodeGzip src_len =0x%x, dec_info.src_img_size=0x%x")
TRACE_MSG(BRW_UTILITY_1083_112_2_18_2_7_32_852,"[BRW]W BRWUtil_DecodeGzipByFile:dest_file_name_ptr=0x%x;src_file_name_ptr=0x%x;")
TRACE_MSG(BRW_UTILITY_1100_112_2_18_2_7_32_853,"BRWUtil_DecodeGzipByFile:decode_input.src_size = 0x%x")
TRACE_MSG(BRW_UTILITY_1130_112_2_18_2_7_32_854,"BRWUtil_DecodeGzipByFile:decode_result =%d")
TRACE_MSG(BRW_UTILITY_1178_112_2_18_2_7_32_855,"[BRW]BRWUtil_DecodeGzipRead:buf =0x%x, size =%d")
TRACE_MSG(BRW_UTILITY_1185_112_2_18_2_7_32_856,"[BRW]BRWUtil_DecodeGzipRead:src_ffs_handler =0x%x, data_len =%d, readed =%d")
TRACE_MSG(BRW_UTILITY_1193_112_2_18_2_7_32_857,"[BRW]BRWUtil_DecodeGzipRead:result =%d, bytes_readed =%d, to_read_size =%d")
TRACE_MSG(BRW_UTILITY_1218_112_2_18_2_7_32_858,"[BRW]BRWUtil_DecodeGzipWrite:result =%d, cnt =%d, bytes_written =%d")
TRACE_MSG(BRW_WTAI_62_112_2_18_2_7_32_859,"[BRW]W BrwWPMakeCall:phone_num_len=%d")
TRACE_MSG(BRW_WTAI_86_112_2_18_2_7_32_860,"[BRW]W BrwWPSendDTMF:dtmf_len=%d")
TRACE_MSG(BRW_WTAI_114_112_2_18_2_7_32_861,"[BRW]W BrwWPMakeCall:phone_num_len=%d")
TRACE_MSG(BRW_WTAI_145_112_2_18_2_7_32_862,"[BRW]W BrwWPSendSms: phone_num_len=%d,sms_content_len=%d")
TRACE_MSG(BRW_WTAI_151_112_2_18_2_7_33_863,"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d")
TRACE_MSG(BRW_WTAI_156_112_2_18_2_7_33_864,"[BRW]BrwGetWTAIInfo:err pb sms content length too long!len=%d")
TRACE_MSG(BRW_WTAI_189_112_2_18_2_7_33_865,"[BRW]W BrwWPSendSmsTo: phone_num_len=%d,sms_content_len=%d")
TRACE_MSG(BRW_WTAI_195_112_2_18_2_7_33_866,"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d")
TRACE_MSG(BRW_WTAI_200_112_2_18_2_7_33_867,"[BRW]BrwGetWTAIInfo:err pb sms content length too long!len=%d")
TRACE_MSG(BRW_WTAI_230_112_2_18_2_7_33_868,"[BRW]W BrwWPSendMms:phone_num_len=%d")
TRACE_MSG(BRW_WTAI_235_112_2_18_2_7_33_869,"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d")
TRACE_MSG(BRW_WTAI_259_112_2_18_2_7_33_870,"[BRW]W BrwWPSendMmsTo:phone_num_len=%d")
TRACE_MSG(BRW_WTAI_264_112_2_18_2_7_33_871,"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d")
TRACE_MSG(BRW_WTAI_288_112_2_18_2_7_33_872,"[BRW]W BrwWPSendMail:phone_num_len=%d")
TRACE_MSG(BRW_WTAI_293_112_2_18_2_7_33_873,"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d")
TRACE_MSG(BRW_WTAI_442_112_2_18_2_7_33_874,"[BRW]W BrwGetWTAIInfo:url_ptr=0x%x")
TRACE_MSG(BRW_WTAI_500_112_2_18_2_7_33_875,"[BRW]BrwGetWTAIInfo:err email addr length too long!len=%d")
TRACE_MSG(BRW_WTAI_541_112_2_18_2_7_33_876,"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d")
TRACE_MSG(BRW_WTAI_555_112_2_18_2_7_33_877,"[BRW]BrwGetWTAIInfo:err pb name length too long!len=%d")
TRACE_MSG(BRW_WTAI_563_112_2_18_2_7_33_878,"[BRW]BrwGetWTAIInfo:not support")
END_TRACE_MAP(MMI_APP_BROWSER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_BROWSER_TRC_H_


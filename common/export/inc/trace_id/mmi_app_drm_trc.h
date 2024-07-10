/******************************************************************************
 ** File Name:      mmi_app_drm_trc.h                                         *
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
//trace_id:168
#ifndef _MMI_APP_DRM_TRC_H_
#define _MMI_APP_DRM_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI_DRMFILEMGR_78_112_2_18_2_14_35_0 "[MMIDRM] MMIDRM_IsDRMFile: invalid param!"
#define MMI_DRMFILEMGR_103_112_2_18_2_14_35_1 "[MMIDRM] MMIDRM_IsDRMFileByPath: invalid param!"
#define MMI_DRMFILEMGR_140_112_2_18_2_14_35_2 "[MMIDRM] MMIDRM_IsRightsValid: invalid param!"
#define MMI_DRMFILEMGR_163_112_2_18_2_14_35_3 "[MMIDRM] MMIDRM_IsRightsValidByPath: invalid param!"
#define MMI_DRMFILEMGR_195_112_2_18_2_14_35_4 "[MMIDRM] MMIDRM_IsRsValidNoCarePer: invalid param!"
#define MMI_DRMFILEMGR_217_112_2_18_2_14_35_5 "[MMIDRM] MMIDRM_IsRsValidNoCarePerByPath: invalid param!"
#define MMI_DRMFILEMGR_248_112_2_18_2_14_36_6 "[MMIDRM] MMIDRM_IsRightsValidByRights: invalid param!"
#define MMI_DRMFILEMGR_273_112_2_18_2_14_36_7 "[MMIDRM] MMIDRM_IsRightsExpired: invalid param!"
#define MMI_DRMFILEMGR_300_112_2_18_2_14_36_8 "[MMIDRM] MMIDRM_IsRightsExpiredByPath: invalid param!"
#define MMI_DRMFILEMGR_330_112_2_18_2_14_36_9 "[MMIDRM] MMIDRM_IsRightsExpiredByRights: valid param!"
#define MMI_DRMFILEMGR_354_112_2_18_2_14_36_10 "[MMIDRM] MMIDRM_IsCanForward: invalid param!"
#define MMI_DRMFILEMGR_361_112_2_18_2_14_36_11 "[MMIDRM] MMIDRM_IsCanForward: this isn't drm file, return %d!"
#define MMI_DRMFILEMGR_370_112_2_18_2_14_36_12 "[MMIDRM] MMIDRM_IsCanForward: get rights error = %d!"
#define MMI_DRMFILEMGR_399_112_2_18_2_14_36_13 "[MMIDRM] MMIDRM_IsCanForwardByPath: invalid param!"
#define MMI_DRMFILEMGR_427_112_2_18_2_14_36_14 "[MMIDRM] MMIDRM_IsCanForwardByRights: invalid param!"
#define MMI_DRMFILEMGR_449_112_2_18_2_14_36_15 "[MMIDRM] MMIDRM_GetRightsinfo: invalid param!"
#define MMI_DRMFILEMGR_470_112_2_18_2_14_36_16 "[MMIDRM] MMIDRM_ConsumeRights: invalid param!"
#define MMI_DRMFILEMGR_493_112_2_18_2_14_36_17 "[MMIDRM] MMIDRM_ConsumeRightsByPath: invalid param!"
#define MMI_DRMFILEMGR_500_112_2_18_2_14_36_18 "[MMIDRM] MMIDRM_ConsumeRightsByPath: not drm file suffix!"
#define MMI_DRMFILEMGR_535_112_2_18_2_14_36_19 "[MMIDRM] MMIDRM_GetRightsinfoByPath: invalid param!"
#define MMI_DRMFILEMGR_564_112_2_18_2_14_36_20 "[MMIDRM] MMIDRM_GetFilePermission: invalid param!"
#define MMI_DRMFILEMGR_610_112_2_18_2_14_36_21 "[MMIDRM] MMIDRM_GetFilePermissionByPath: invalid param!"
#define MMI_DRMFILEMGR_640_112_2_18_2_14_36_22 "[MMIDRM] MMIDRM_GetRightsConstraint: invalid param!"
#define MMI_DRMFILEMGR_663_112_2_18_2_14_36_23 "[MMIDRM] MMIDRM_GetDateIntervalTime: invalid param!"
#define MMI_DRMFILEMGR_783_112_2_18_2_14_37_24 "[MMIDRM] MMIDRM_GetDRMFileType: invalid param!"
#define MMI_DRMFILEMGR_808_112_2_18_2_14_37_25 "[MMIDRM] MMIDRM_GetDRMFileTypeByPath: invalid param!"
#define MMI_DRMFILEMGR_837_112_2_18_2_14_37_26 "[MMIDRM] MMIDRM_GetDRMFileMimeType: invalid param!"
#define MMI_DRMFILEMGR_844_112_2_18_2_14_37_27 "[MMIDRM] MMIDRM_GetMediaFileType: Get drm mime type is error=%d, mime_type=%d."
#define MMI_DRMFILEMGR_865_112_2_18_2_14_37_28 "[MMIDRM] MMIDRM_GetDRMFileMimeTypeByPath: invalid param!"
#define MMI_DRMFILEMGR_895_112_2_18_2_14_37_29 "[MMIDRM] MMIDRM_GetMediaFileType: invalid param!"
#define MMI_DRMFILEMGR_909_112_2_18_2_14_37_30 "[MMIDRM] MMIDRM_GetMediaFileType: Get drm media type is error=%d, media_type=%d."
#define MMI_DRMFILEMGR_930_112_2_18_2_14_37_31 "[MMIDRM] MMIDRM_GetMediaFileTypeByPath: invalid param!"
#define MMI_DRMFILEMGR_959_112_2_18_2_14_37_32 "[MMIDRM] MMIDRM_DownloadRightsFile: invalid param!"
#define MMI_DRMFILEMGR_1002_112_2_18_2_14_37_33 "[MMIDRM] MMIDRM_DownloadRightsFileByPath: invalid param!"
#define MMI_DRMFILEMGR_1030_112_2_18_2_14_37_34 "[MMIDRM] MMIDRM_InstallFileReq: invalid param!"
#define MMI_DRMFILEMGR_1036_112_2_18_2_14_37_35 "[MMIDRM] MMIDRM_InstallFileReq: install request is=%d."
#define MMI_DRMFILEMGR_1094_112_2_18_2_14_37_36 "[MMIDRM] MMIDRM_CreateDrmRoDir: create d_drm_ro path success."
#define MMI_DRMFILEMGR_1108_112_2_18_2_14_37_37 "[MMIDRM] MMIDRM_CreateDrmRoDir: create e_drm_ro path success."
#define MMI_DRMFILEMGR_1122_112_2_18_2_14_37_38 "[MMIDRM] MMIDRM_CreateDrmRoDir: create f_drm_ro path success."
#define MMI_DRMFILEMGR_1203_112_2_18_2_14_37_39 "[MMIDRM] MMIDRM_CanDrmFileSetCallRing: invalid param!"
#define MMI_DRMFILEMGR_1211_112_2_18_2_14_38_40 "[MMIDRM] MMIDRM_CanDrmFileSetCallRing create file error!"
#define MMI_DRMFILEMGR_1239_112_2_18_2_14_38_41 "[MMIDRM] MMIDRM_CanDrmFileSetCallRing: drm file is count cons.!"
#define MMI_DRMFILEMGR_1277_112_2_18_2_14_38_42 "[MMIDRM] GetNettimeSysn: invalid param!"
#define MMI_DRMFILEMGR_1311_112_2_18_2_14_38_43 "[MMIDRM] IsRightsValid: invalid param!"
#define MMI_DRMFILEMGR_1317_112_2_18_2_14_38_44 "[MMIDRM] IsRightsValid: this isn't drm file!"
#define MMI_DRMFILEMGR_1326_112_2_18_2_14_38_45 "[MMIDRM] IsRightsValid: can not get rights!"
#define MMI_DRMFILEMGR_1342_112_2_18_2_14_38_46 "[MMIDRM] IsRightsValid return = %d!"
#define MMIDRM_398_112_2_18_2_14_39_47 "[MMIDRM] MMIDRM_InstallRoFile: invalid param!"
#define MMIDRM_425_112_2_18_2_14_39_48 "[MMIDRM] MMIDRM_HandleDrmControlMsg msg_id=%d"
#define MMIDRM_429_112_2_18_2_14_39_49 "[MMIDRM] MMIDRM_HandleDrmControlMsg: invalid param!"
#define MMIDRM_443_112_2_18_2_14_39_50 "[MMIDRM] MMIDRM_HandleDrmControlMsg error_code=%d, install_file_type=%d"
#define MMIDRM_533_112_2_18_2_14_39_51 "[MMIDRM] CreateRoSaveWin: invalid param!"
#define MMIDRM_540_112_2_18_2_14_39_52 "[MMIDRM] CreateRoSaveWin: SCI_ALLOCA win_data_ptr error.!"
#define MMIDRM_548_112_2_18_2_14_39_53 "[MMIDRM] CreateRoSaveWin: SCI_ALLOCA win_data_ptr->rights_buffer_ptr error.!"
#define MMIDRM_584_112_2_18_2_14_39_54 "[MMIDRM] HandleRoFileMsg: invalid param!"
#define MMIDRM_591_112_2_18_2_14_39_55 "[MMIDRM] HandleRoFileMsg: win_data_ptr is null!"
#define MMIDRM_600_112_2_18_2_14_39_56 "[MMIDRM] HandleRoFileMsg: Install request!"
#define MMIDRM_833_112_2_18_2_14_40_57 "[MMIDRM] StartInstallDrm ERROR"
#define MMIDRM_893_112_2_18_2_14_40_58 "[MMIDRM] StartInstallDrm ERROR"
#define MMIDRM_962_112_2_18_2_14_40_59 "[MMIDRM] HandleDrmInstallCnf: error_code =%d"
#define MMIDRM_977_112_2_18_2_14_40_60 "[MMIDRM] HandleDrmInstallCnf BEGIN TO install java"
#define MMIDRM_997_112_2_18_2_14_40_61 "[MMIDRM] is_rights_expired = %d"
#define MMIDRM_1013_112_2_18_2_14_40_62 "[MMIDRM] BEGIN TO install java."
#define MMIDRM_1195_112_2_18_2_14_40_63 "[MMIDRM] MMIAPIDRM_StartToInstallDrm file_name_ptr ERROR"
#define MMIDRM_1202_112_2_18_2_14_41_64 "[MMIDRM] MMIAPIDRM_StartToInstallDrm file_name ERROR"
#define MMIDRM_1209_112_2_18_2_14_41_65 "[MMIDRM] MMIAPIDRM_StartToInstallDrm ALLOC ERROR!!"
#define MMIDRM_1216_112_2_18_2_14_41_66 "[MMIDRM] MMIAPIDRM_StartToInstallDrm mime_type = %d"
#define MMIDRM_1230_112_2_18_2_14_41_67 "[MMIDRM] MMIAPIDRM_StartToInstallDrm download_file_name_ptr PNULL"
#define MMIDRM_1266_112_2_18_2_14_41_68 "[MMIDRM] MMIAPIDRM_StartToInstallDrm url_ptr ALLOR Error, url_len =%d"
#define MMIDRM_1319_112_2_18_2_14_41_69 "[MMIDRM] HandleDrmInstallWaitWinMsg: install_data_ptr is null!"
#define MMIDRM_1336_112_2_18_2_14_41_70 "[MMIDRM] HandleDrmInstallWaitWinMsg: install_data_ptr is null!"
#define MMIDRM_1608_112_2_18_2_14_41_71 "[MMIDRM] DeleteInstallFailFile: delete file is not exist!"
#define MMIDRM_1648_112_2_18_2_14_41_72 "[MMIDRM] HandleDrmRoWaitWinMsg MSG_OPEN_WINDOW: install_data_ptr is null!"
#define MMIDRM_1680_112_2_18_2_14_41_73 "[MMIDRM] HandleDrmRoWaitWinMsg DRM_SIG_DH_INSTALL_FILE_CNF: install_data_ptr is null!"
#define MMIDRM_1761_112_2_18_2_14_42_74 "[MMIDRM] BEGIN TO install java"
#define MMIDRM_1766_112_2_18_2_14_42_75 "[MMIDRM] HandleDrmReceivedRoAlertWinMsg:NULL pubwin_info_ptr = 0x%x"
#define MMIDRM_2138_112_2_18_2_14_42_76 "[MMIDRM] DrmPushNotify dual_sys =%d"
#define MMIDRM_2296_112_2_18_2_14_43_77 "MMIDRM MMIDRM_HandleDLDrmCB mime type = %d"
#define MMIDRM_2339_112_2_18_2_14_43_78 "[MMIDRM] StartInstallDrmRo param error."
#define MMIDRM_EXPORT_70_112_2_18_2_14_43_79 "[MMIDRM] MMIAPIDRM_IsDRMFile: invalid param!"
#define MMIDRM_EXPORT_103_112_2_18_2_14_44_80 "[MMIDRM] MMIAPIDRM_IsRightsExpired: invalid param!"
#define MMIDRM_EXPORT_139_112_2_18_2_14_44_81 "[MMIDRM] MMIAPIDRM_IsCanForward: invalid param!"
#define MMIDRM_EXPORT_172_112_2_18_2_14_44_82 "[MMIDRM] MMIAPIDRM_IsRightsValid: invalid param!"
#define MMIDRM_EXPORT_189_112_2_18_2_14_44_83 "[MMIDRM] MMIAPIDRM_IsRightsValid: invalid param!"
#define MMIDRM_EXPORT_220_112_2_18_2_14_44_84 "[MMIDRM] MMIAPIDRM_GetRightsinfo: invalid param!"
#define MMIDRM_EXPORT_249_112_2_18_2_14_44_85 "[MMIDRM] MMIAPIDRM_GetFilePermission: invalid param!"
#define MMIDRM_EXPORT_277_112_2_18_2_14_44_86 "[MMIDRM] MMIAPIDRM_ConsumeRights: invalid param!"
#define MMIDRM_EXPORT_301_112_2_18_2_14_44_87 "[MMIDRM] MMIAPIDRM_GetRightsConstraint: invalid param!"
#define MMIDRM_EXPORT_325_112_2_18_2_14_44_88 "[MMIDRM] MMIAPIDRM_GetDateIntervalTime: invalid param!"
#define MMIDRM_EXPORT_377_112_2_18_2_14_44_89 "[MMIDRM] MMIAPIDRM_GetDRMFileType: invalid param!"
#define MMIDRM_EXPORT_406_112_2_18_2_14_44_90 "[MMIDRM] MMIAPIDRM_GetDRMFileMimeType: invalid param!"
#define MMIDRM_EXPORT_435_112_2_18_2_14_45_91 "[MMIDRM] MMIAPIDRM_GetMediaFileType: invalid param!"
#define MMIDRM_EXPORT_464_112_2_18_2_14_45_92 "[MMIDRM] MMIAPIDRM_DownloadRightsFile: invalid param!"
#define MMIDRM_EXPORT_526_112_2_18_2_14_45_93 "[MMIDRM] MMIAPIDRM_DownloadRightsFile: invalid param!"
#define MMIDRM_EXPORT_571_112_2_18_2_14_45_94 "[MMIDRM] MMIAPIDRM_UpdateNetworkTime: begin to auto update network time!"
#define MMIDRM_EXPORT_574_112_2_18_2_14_45_95 "[MMIDRM] MMIAPIDRM_UpdateNetworkTime: not support auto update network time!"
#define MMIDRM_EXPORT_579_112_2_18_2_14_45_96 "[MMIDRM] MMIAPIDRM_UpdateNetworkTime: the phone is already update network time!"
#define MMIDRM_EXPORT_701_112_2_18_2_14_45_97 "[MMIDRM] MMIAPIDRM_AlterNetworkTime: alter network time, newtime-oldtime=%d!"
#define MMIDRM_EXPORT_705_112_2_18_2_14_45_98 "[MMIDRM] MMIAPIDRM_AlterNetworkTime: the phone's nitz time is not update, so don't alter!"
#define MMIDRM_EXPORT_735_112_2_18_2_14_45_99 "[MMIDRM] MMIAPIDRM_PreCheckFileStatus: invalid param!"
#define MMIDRM_EXPORT_846_112_2_18_2_14_46_100 "[MMIDRM] MMIAPIDRM_PreCheckFileStatus: invalid param!"
#define MMIDRM_EXPORT_957_112_2_18_2_14_46_101 "[MMIDRM] MMIAPIDRM_PreCheckFileStatusEX: invalid param!"
#define MMIDRM_EXPORT_1062_112_2_18_2_14_46_102 "[MMIDRM] MMIAPIDRM_CanDrmFileSetToAllType: invalid param!"
#define MMIDRM_EXPORT_1069_112_2_18_2_14_46_103 "[MMIDRM] MMIAPIDRM_CanDrmFileSetToAllType: not drm file suffix!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_DRM_TRC)
TRACE_MSG(MMI_DRMFILEMGR_78_112_2_18_2_14_35_0,"[MMIDRM] MMIDRM_IsDRMFile: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_103_112_2_18_2_14_35_1,"[MMIDRM] MMIDRM_IsDRMFileByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_140_112_2_18_2_14_35_2,"[MMIDRM] MMIDRM_IsRightsValid: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_163_112_2_18_2_14_35_3,"[MMIDRM] MMIDRM_IsRightsValidByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_195_112_2_18_2_14_35_4,"[MMIDRM] MMIDRM_IsRsValidNoCarePer: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_217_112_2_18_2_14_35_5,"[MMIDRM] MMIDRM_IsRsValidNoCarePerByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_248_112_2_18_2_14_36_6,"[MMIDRM] MMIDRM_IsRightsValidByRights: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_273_112_2_18_2_14_36_7,"[MMIDRM] MMIDRM_IsRightsExpired: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_300_112_2_18_2_14_36_8,"[MMIDRM] MMIDRM_IsRightsExpiredByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_330_112_2_18_2_14_36_9,"[MMIDRM] MMIDRM_IsRightsExpiredByRights: valid param!")
TRACE_MSG(MMI_DRMFILEMGR_354_112_2_18_2_14_36_10,"[MMIDRM] MMIDRM_IsCanForward: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_361_112_2_18_2_14_36_11,"[MMIDRM] MMIDRM_IsCanForward: this isn't drm file, return %d!")
TRACE_MSG(MMI_DRMFILEMGR_370_112_2_18_2_14_36_12,"[MMIDRM] MMIDRM_IsCanForward: get rights error = %d!")
TRACE_MSG(MMI_DRMFILEMGR_399_112_2_18_2_14_36_13,"[MMIDRM] MMIDRM_IsCanForwardByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_427_112_2_18_2_14_36_14,"[MMIDRM] MMIDRM_IsCanForwardByRights: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_449_112_2_18_2_14_36_15,"[MMIDRM] MMIDRM_GetRightsinfo: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_470_112_2_18_2_14_36_16,"[MMIDRM] MMIDRM_ConsumeRights: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_493_112_2_18_2_14_36_17,"[MMIDRM] MMIDRM_ConsumeRightsByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_500_112_2_18_2_14_36_18,"[MMIDRM] MMIDRM_ConsumeRightsByPath: not drm file suffix!")
TRACE_MSG(MMI_DRMFILEMGR_535_112_2_18_2_14_36_19,"[MMIDRM] MMIDRM_GetRightsinfoByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_564_112_2_18_2_14_36_20,"[MMIDRM] MMIDRM_GetFilePermission: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_610_112_2_18_2_14_36_21,"[MMIDRM] MMIDRM_GetFilePermissionByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_640_112_2_18_2_14_36_22,"[MMIDRM] MMIDRM_GetRightsConstraint: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_663_112_2_18_2_14_36_23,"[MMIDRM] MMIDRM_GetDateIntervalTime: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_783_112_2_18_2_14_37_24,"[MMIDRM] MMIDRM_GetDRMFileType: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_808_112_2_18_2_14_37_25,"[MMIDRM] MMIDRM_GetDRMFileTypeByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_837_112_2_18_2_14_37_26,"[MMIDRM] MMIDRM_GetDRMFileMimeType: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_844_112_2_18_2_14_37_27,"[MMIDRM] MMIDRM_GetMediaFileType: Get drm mime type is error=%d, mime_type=%d.")
TRACE_MSG(MMI_DRMFILEMGR_865_112_2_18_2_14_37_28,"[MMIDRM] MMIDRM_GetDRMFileMimeTypeByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_895_112_2_18_2_14_37_29,"[MMIDRM] MMIDRM_GetMediaFileType: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_909_112_2_18_2_14_37_30,"[MMIDRM] MMIDRM_GetMediaFileType: Get drm media type is error=%d, media_type=%d.")
TRACE_MSG(MMI_DRMFILEMGR_930_112_2_18_2_14_37_31,"[MMIDRM] MMIDRM_GetMediaFileTypeByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_959_112_2_18_2_14_37_32,"[MMIDRM] MMIDRM_DownloadRightsFile: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_1002_112_2_18_2_14_37_33,"[MMIDRM] MMIDRM_DownloadRightsFileByPath: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_1030_112_2_18_2_14_37_34,"[MMIDRM] MMIDRM_InstallFileReq: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_1036_112_2_18_2_14_37_35,"[MMIDRM] MMIDRM_InstallFileReq: install request is=%d.")
TRACE_MSG(MMI_DRMFILEMGR_1094_112_2_18_2_14_37_36,"[MMIDRM] MMIDRM_CreateDrmRoDir: create d_drm_ro path success.")
TRACE_MSG(MMI_DRMFILEMGR_1108_112_2_18_2_14_37_37,"[MMIDRM] MMIDRM_CreateDrmRoDir: create e_drm_ro path success.")
TRACE_MSG(MMI_DRMFILEMGR_1122_112_2_18_2_14_37_38,"[MMIDRM] MMIDRM_CreateDrmRoDir: create f_drm_ro path success.")
TRACE_MSG(MMI_DRMFILEMGR_1203_112_2_18_2_14_37_39,"[MMIDRM] MMIDRM_CanDrmFileSetCallRing: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_1211_112_2_18_2_14_38_40,"[MMIDRM] MMIDRM_CanDrmFileSetCallRing create file error!")
TRACE_MSG(MMI_DRMFILEMGR_1239_112_2_18_2_14_38_41,"[MMIDRM] MMIDRM_CanDrmFileSetCallRing: drm file is count cons.!")
TRACE_MSG(MMI_DRMFILEMGR_1277_112_2_18_2_14_38_42,"[MMIDRM] GetNettimeSysn: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_1311_112_2_18_2_14_38_43,"[MMIDRM] IsRightsValid: invalid param!")
TRACE_MSG(MMI_DRMFILEMGR_1317_112_2_18_2_14_38_44,"[MMIDRM] IsRightsValid: this isn't drm file!")
TRACE_MSG(MMI_DRMFILEMGR_1326_112_2_18_2_14_38_45,"[MMIDRM] IsRightsValid: can not get rights!")
TRACE_MSG(MMI_DRMFILEMGR_1342_112_2_18_2_14_38_46,"[MMIDRM] IsRightsValid return = %d!")
TRACE_MSG(MMIDRM_398_112_2_18_2_14_39_47,"[MMIDRM] MMIDRM_InstallRoFile: invalid param!")
TRACE_MSG(MMIDRM_425_112_2_18_2_14_39_48,"[MMIDRM] MMIDRM_HandleDrmControlMsg msg_id=%d")
TRACE_MSG(MMIDRM_429_112_2_18_2_14_39_49,"[MMIDRM] MMIDRM_HandleDrmControlMsg: invalid param!")
TRACE_MSG(MMIDRM_443_112_2_18_2_14_39_50,"[MMIDRM] MMIDRM_HandleDrmControlMsg error_code=%d, install_file_type=%d")
TRACE_MSG(MMIDRM_533_112_2_18_2_14_39_51,"[MMIDRM] CreateRoSaveWin: invalid param!")
TRACE_MSG(MMIDRM_540_112_2_18_2_14_39_52,"[MMIDRM] CreateRoSaveWin: SCI_ALLOCA win_data_ptr error.!")
TRACE_MSG(MMIDRM_548_112_2_18_2_14_39_53,"[MMIDRM] CreateRoSaveWin: SCI_ALLOCA win_data_ptr->rights_buffer_ptr error.!")
TRACE_MSG(MMIDRM_584_112_2_18_2_14_39_54,"[MMIDRM] HandleRoFileMsg: invalid param!")
TRACE_MSG(MMIDRM_591_112_2_18_2_14_39_55,"[MMIDRM] HandleRoFileMsg: win_data_ptr is null!")
TRACE_MSG(MMIDRM_600_112_2_18_2_14_39_56,"[MMIDRM] HandleRoFileMsg: Install request!")
TRACE_MSG(MMIDRM_833_112_2_18_2_14_40_57,"[MMIDRM] StartInstallDrm ERROR")
TRACE_MSG(MMIDRM_893_112_2_18_2_14_40_58,"[MMIDRM] StartInstallDrm ERROR")
TRACE_MSG(MMIDRM_962_112_2_18_2_14_40_59,"[MMIDRM] HandleDrmInstallCnf: error_code =%d")
TRACE_MSG(MMIDRM_977_112_2_18_2_14_40_60,"[MMIDRM] HandleDrmInstallCnf BEGIN TO install java")
TRACE_MSG(MMIDRM_997_112_2_18_2_14_40_61,"[MMIDRM] is_rights_expired = %d")
TRACE_MSG(MMIDRM_1013_112_2_18_2_14_40_62,"[MMIDRM] BEGIN TO install java.")
TRACE_MSG(MMIDRM_1195_112_2_18_2_14_40_63,"[MMIDRM] MMIAPIDRM_StartToInstallDrm file_name_ptr ERROR")
TRACE_MSG(MMIDRM_1202_112_2_18_2_14_41_64,"[MMIDRM] MMIAPIDRM_StartToInstallDrm file_name ERROR")
TRACE_MSG(MMIDRM_1209_112_2_18_2_14_41_65,"[MMIDRM] MMIAPIDRM_StartToInstallDrm ALLOC ERROR!!")
TRACE_MSG(MMIDRM_1216_112_2_18_2_14_41_66,"[MMIDRM] MMIAPIDRM_StartToInstallDrm mime_type = %d")
TRACE_MSG(MMIDRM_1230_112_2_18_2_14_41_67,"[MMIDRM] MMIAPIDRM_StartToInstallDrm download_file_name_ptr PNULL")
TRACE_MSG(MMIDRM_1266_112_2_18_2_14_41_68,"[MMIDRM] MMIAPIDRM_StartToInstallDrm url_ptr ALLOR Error, url_len =%d")
TRACE_MSG(MMIDRM_1319_112_2_18_2_14_41_69,"[MMIDRM] HandleDrmInstallWaitWinMsg: install_data_ptr is null!")
TRACE_MSG(MMIDRM_1336_112_2_18_2_14_41_70,"[MMIDRM] HandleDrmInstallWaitWinMsg: install_data_ptr is null!")
TRACE_MSG(MMIDRM_1608_112_2_18_2_14_41_71,"[MMIDRM] DeleteInstallFailFile: delete file is not exist!")
TRACE_MSG(MMIDRM_1648_112_2_18_2_14_41_72,"[MMIDRM] HandleDrmRoWaitWinMsg MSG_OPEN_WINDOW: install_data_ptr is null!")
TRACE_MSG(MMIDRM_1680_112_2_18_2_14_41_73,"[MMIDRM] HandleDrmRoWaitWinMsg DRM_SIG_DH_INSTALL_FILE_CNF: install_data_ptr is null!")
TRACE_MSG(MMIDRM_1761_112_2_18_2_14_42_74,"[MMIDRM] BEGIN TO install java")
TRACE_MSG(MMIDRM_1766_112_2_18_2_14_42_75,"[MMIDRM] HandleDrmReceivedRoAlertWinMsg:NULL pubwin_info_ptr = 0x%x")
TRACE_MSG(MMIDRM_2138_112_2_18_2_14_42_76,"[MMIDRM] DrmPushNotify dual_sys =%d")
TRACE_MSG(MMIDRM_2296_112_2_18_2_14_43_77,"MMIDRM MMIDRM_HandleDLDrmCB mime type = %d")
TRACE_MSG(MMIDRM_2339_112_2_18_2_14_43_78,"[MMIDRM] StartInstallDrmRo param error.")
TRACE_MSG(MMIDRM_EXPORT_70_112_2_18_2_14_43_79,"[MMIDRM] MMIAPIDRM_IsDRMFile: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_103_112_2_18_2_14_44_80,"[MMIDRM] MMIAPIDRM_IsRightsExpired: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_139_112_2_18_2_14_44_81,"[MMIDRM] MMIAPIDRM_IsCanForward: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_172_112_2_18_2_14_44_82,"[MMIDRM] MMIAPIDRM_IsRightsValid: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_189_112_2_18_2_14_44_83,"[MMIDRM] MMIAPIDRM_IsRightsValid: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_220_112_2_18_2_14_44_84,"[MMIDRM] MMIAPIDRM_GetRightsinfo: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_249_112_2_18_2_14_44_85,"[MMIDRM] MMIAPIDRM_GetFilePermission: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_277_112_2_18_2_14_44_86,"[MMIDRM] MMIAPIDRM_ConsumeRights: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_301_112_2_18_2_14_44_87,"[MMIDRM] MMIAPIDRM_GetRightsConstraint: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_325_112_2_18_2_14_44_88,"[MMIDRM] MMIAPIDRM_GetDateIntervalTime: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_377_112_2_18_2_14_44_89,"[MMIDRM] MMIAPIDRM_GetDRMFileType: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_406_112_2_18_2_14_44_90,"[MMIDRM] MMIAPIDRM_GetDRMFileMimeType: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_435_112_2_18_2_14_45_91,"[MMIDRM] MMIAPIDRM_GetMediaFileType: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_464_112_2_18_2_14_45_92,"[MMIDRM] MMIAPIDRM_DownloadRightsFile: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_526_112_2_18_2_14_45_93,"[MMIDRM] MMIAPIDRM_DownloadRightsFile: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_571_112_2_18_2_14_45_94,"[MMIDRM] MMIAPIDRM_UpdateNetworkTime: begin to auto update network time!")
TRACE_MSG(MMIDRM_EXPORT_574_112_2_18_2_14_45_95,"[MMIDRM] MMIAPIDRM_UpdateNetworkTime: not support auto update network time!")
TRACE_MSG(MMIDRM_EXPORT_579_112_2_18_2_14_45_96,"[MMIDRM] MMIAPIDRM_UpdateNetworkTime: the phone is already update network time!")
TRACE_MSG(MMIDRM_EXPORT_701_112_2_18_2_14_45_97,"[MMIDRM] MMIAPIDRM_AlterNetworkTime: alter network time, newtime-oldtime=%d!")
TRACE_MSG(MMIDRM_EXPORT_705_112_2_18_2_14_45_98,"[MMIDRM] MMIAPIDRM_AlterNetworkTime: the phone's nitz time is not update, so don't alter!")
TRACE_MSG(MMIDRM_EXPORT_735_112_2_18_2_14_45_99,"[MMIDRM] MMIAPIDRM_PreCheckFileStatus: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_846_112_2_18_2_14_46_100,"[MMIDRM] MMIAPIDRM_PreCheckFileStatus: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_957_112_2_18_2_14_46_101,"[MMIDRM] MMIAPIDRM_PreCheckFileStatusEX: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_1062_112_2_18_2_14_46_102,"[MMIDRM] MMIAPIDRM_CanDrmFileSetToAllType: invalid param!")
TRACE_MSG(MMIDRM_EXPORT_1069_112_2_18_2_14_46_103,"[MMIDRM] MMIAPIDRM_CanDrmFileSetToAllType: not drm file suffix!")
END_TRACE_MAP(MMI_APP_DRM_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_DRM_TRC_H_


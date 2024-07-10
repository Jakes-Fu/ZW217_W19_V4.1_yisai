/******************************************************************************
 ** File Name:      mmi_app_wre_boot_trc.h                                         *
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
//trace_id:242
#ifndef _MMI_APP_WRE_BOOT_TRC_H_
#define _MMI_APP_WRE_BOOT_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIWRE_ADAPTOR_294_112_2_18_3_10_30_0 "WRE_SDCardInsert return %d"
#define MMIWRE_ADAPTOR_367_112_2_18_3_10_30_1 "WARNING!!! update fail!!!"
#define MMIWRE_ADAPTOR_446_112_2_18_3_10_30_2 "enter WRE_InstallKernel!"
#define MMIWRE_ADAPTOR_450_112_2_18_3_10_30_3 "unzip failure!"
#define MMIWRE_ADAPTOR_454_112_2_18_3_10_30_4 "WRE_InstallKernel unzip ok!"
#define MMIWRE_ADAPTOR_460_112_2_18_3_10_30_5 "unpack failure!"
#define MMIWRE_ADAPTOR_464_112_2_18_3_10_30_6 "WRE_InstallKernel unpack ok!"
#define MMIWRE_ADAPTOR_564_112_2_18_3_10_30_7 "WREEntry"
#define MMIWRE_ADAPTOR_669_112_2_18_3_10_30_8 "WREEntry1"
#define MMIWRE_ADAPTOR_673_112_2_18_3_10_30_9 "[WRE]wre is running!"
#define MMIWRE_ADAPTOR_678_112_2_18_3_10_30_10 "[WRE]RestoreMiniWin "
#define MMIWRE_ADAPTOR_703_112_2_18_3_10_31_11 "WREEntry DownloadAndInstallWRE"
#define MMIWRE_ADAPTOR_755_112_2_18_3_10_31_12 "ASYNCSOCK_HandlePsMsg, signal_ptr->msg_id = %lx"
#define MMIWRE_ADAPTOR_983_112_2_18_3_10_31_13 "boot_reboot CALLED1"
#define MMIWRE_ADAPTOR_989_112_2_18_3_10_31_14 "boot_reboot END"
#define MMIWRE_ADAPTOR_1250_112_2_18_3_10_32_15 "enter wre_StoreLastPhotePath  "
#define MMIWRE_ADAPTOR_1269_112_2_18_3_10_32_16 "enter TracePhotoPath  "
#define MMIWRE_ADAPTOR_1273_112_2_18_3_10_32_17 "leave TracePhotoPath %s "
#define MMIWRE_ADAPTOR_1378_112_2_18_3_10_32_18 "Adapt_EntrySNSView"
#define MMIWRE_ADAPTOR_1467_112_2_18_3_10_32_19 "[WRE]Adapt_EntryPhotoView: SetAnimParam Fail! anim_result = %d"
#define MMIWRE_ADAPTOR_1521_112_2_18_3_10_32_20 "[WRE]Adapt_EntryPhotoView: lpFullFileName = %s"
#define MMIWRE_ADAPTOR_1556_112_2_18_3_10_32_21 "[wre]WRE_IsFixAppInstalled return %d"
#define MMIWRE_ADAPTOR_1612_112_2_18_3_10_32_22 "[WRE]WRE_CreateVirtualROMDisk: Create ROM Disk Fail!"
#define MMIWRE_ADAPTOR_1618_112_2_18_3_10_32_23 "[WRE]RegisterDevice :success"
#define MMIWRE_ADAPTOR_1622_112_2_18_3_10_32_24 "[WRE]RegisterDevice:fail, the format device"
#define MMIWRE_ADAPTOR_1648_112_2_18_3_10_32_25 "[WRE]WRE_CreateVirtualRAMDisk:"
#define MMIWRE_ADAPTOR_1652_112_2_18_3_10_32_26 "[WRE]WRE_CreateVirtualRAMDisk: warning! lpHandDisk is already create"
#define MMIWRE_ADAPTOR_1663_112_2_18_3_10_32_27 "[WRE]WRE_CreateVirtualRAMDisk: Alloc memory Fail!"
#define MMIWRE_ADAPTOR_1672_112_2_18_3_10_32_28 "[WRE]WRE_CreateVirtualRAMDisk: DISK_Create Fail!"
#define MMIWRE_ADAPTOR_1682_112_2_18_3_10_32_29 "[WRE]RegisterDevice : RAM disk success"
#define MMIWRE_ADAPTOR_1686_112_2_18_3_10_32_30 "[WRE]RegisterDevice: RAM disk fail, the format device"
#define MMIWRE_ADAPTOR_1701_112_2_18_3_10_33_31 "[WRE]WRE_DestoryVirtualDisk: DISK_Delete!"
#define MMIWRE_ADAPTOR_1708_112_2_18_3_10_33_32 "[WRE]WRE_DestoryVirtualDisk: Delete ROM disk Fail!"
#define MMIWRE_ADAPTOR_1720_112_2_18_3_10_33_33 "[WRE]WRE_DestoryVirtualDisk: Delete RAM disk Fail!"
#define MMIWRE_ADAPTOR_1739_112_2_18_3_10_33_34 "[WRE]WRE_TestVirtualDisk: Alloc memory fail!"
#define MMIWRE_ADAPTOR_1750_112_2_18_3_10_33_35 "[WRE]WRE_TestVirtualDisk: CreateFile Fail!"
#define MMIWRE_ADAPTOR_1758_112_2_18_3_10_33_36 "[WRE]WRE_FILECOPY Fail: WriteFile Error(%lx)"
#define MMIWRE_ADAPTOR_1778_112_2_18_3_10_33_37 "[WRE]WRE_TestVirtualDisk: Alloc memory fail!"
#define MMIWRE_ADAPTOR_1787_112_2_18_3_10_33_38 "[WRE]WRE_TestVirtualDisk: CreateFile Fail!"
#define MMIWRE_ADAPTOR_1795_112_2_18_3_10_33_39 "[WRE]WRE_FILECOPY Fail: WriteFile Error(%lx)"
#define MMIWRE_ADAPTOR_1814_112_2_18_3_10_33_40 "[WRE]WRE_TestVirtualDisk: WriteFile Fail!"
#define MMIWRE_ADAPTOR_1820_112_2_18_3_10_33_41 "[WRE]WRE_TestVirtualDisk: ReadFile Fail!"
#define MMIWRE_CFG_331_112_2_18_3_10_34_42 "WRE_Set_SYS_Dir: sysPathType = %d"
#define MMIWRE_CFG_372_112_2_18_3_10_34_43 "WRE_Get_SYS_Dir_Update: sysPathType = %d"
#define MMIWRE_CFG_384_112_2_18_3_10_34_44 "WRE_Get_SYS_Dir_Update Fail: not support!"
#define MMIWRE_CFG_413_112_2_18_3_10_34_45 "WRE_Get_SYS_Dir_Update Fail:"
#define MMIWRE_CFG_433_112_2_18_3_10_34_46 "WRE_Get_Preload_Dir: sysPathType = %d"
#define MMIWRE_CFG_480_112_2_18_3_10_34_47 "WRE_Get_Preload_Dir: PreloadType = %d"
#define MMIWRE_CFG_746_112_2_18_3_10_35_48 "[WRE]WRE_GetPhoneInfo: virtual disk: len =%d"
#define MMIWRE_CHARSET_39_112_2_18_3_10_35_49 "cp_wcstombs : malloc fail\n"
#define MMIWRE_CHARSET_70_112_2_18_3_10_36_50 "cp_mbstowcs : malloc fail\n"
#define MMIWRE_EXPORT_90_112_2_18_3_10_37_51 "[WRE] HandleWREOpenWaitingWin: msg = 0x%x"
#define MMIWRE_EXPORT_97_112_2_18_3_10_37_52 "[WRE]   WREEntry return 0x%x"
#define MMIWRE_EXPORT_115_112_2_18_3_10_37_53 "[WRE]   WREEntry return 0x%x"
#define MMIWRE_EXPORT_184_112_2_18_3_10_37_54 "[WRE]   WREEntry return 0x%x"
#define MMIWRE_EXPORT_238_112_2_18_3_10_37_55 "[WRE]   WREEntry return 0x%x"
#define MMIWRE_EXPORT_336_112_2_18_3_10_38_56 "[WRE]:MMIWRE_GetAppGuid appid = %d"
#define MMIWRE_EXPORT_349_112_2_18_3_10_38_57 "[WRE]MMIWRE_GetFixAppPath"
#define MMIWRE_EXPORT_381_112_2_18_3_10_38_58 "[WRE]:MMIWRE_StartAppEx, appid = %d, guid = %s, data = %s"
#define MMIWRE_IMAGE_78_112_2_18_3_10_40_59 "IMG_DEC_GetInfo fail!"
#define MMIWRE_IMAGE_86_112_2_18_3_10_40_60 "dec_info.img_type %d != %d!"
#define MMIWRE_IMAGE_103_112_2_18_3_10_40_61 "dec_info.img_type %d != %d!"
#define MMIWRE_IMAGE_114_112_2_18_3_10_40_62 "dec_info.img_type %d != %d!"
#define MMIWRE_IMAGE_237_112_2_18_3_10_41_63 "ScratchImage: NULL == lpszData || PNULL == lpszName"
#define MMIWRE_IMAGE_240_112_2_18_3_10_41_64 ">>>_ScratchImage0=%08x, ptr=%08x, cyDesired=%d, nPriority=%d"
#define MMIWRE_IMAGE_292_112_2_18_3_10_41_65 ">>>_ScratchImage1=%d,%d"
#define MMIWRE_IMAGE_357_112_2_18_3_10_41_66 "IMG_DEC_GetInfo fail!"
#define MMIWRE_MEDIA_229_112_2_18_3_10_45_67 ">>>audio_stream=%d"
#define MMIWRE_MEDIA_240_112_2_18_3_10_45_68 "wre_player_notify, handle = %d"
#define MMIWRE_MEDIA_310_112_2_18_3_10_45_69 "MediaRequest, play, media type error "
#define MMIWRE_MEDIA_329_112_2_18_3_10_45_70 "MediaRequest, record, error = %d"
#define MMIWRE_MEDIA_335_112_2_18_3_10_45_71 "MediaRequest, record, create file error"
#define MMIWRE_MEDIA_382_112_2_18_3_10_45_72 "s_mmPlay, eState = %d, handle = %d"
#define MMIWRE_MEDIA_409_112_2_18_3_10_45_73 "s_mmPlay, MMISRVAUD_Play, fail. ret = %d"
#define MMIWRE_MEDIA_421_112_2_18_3_10_45_74 "s_mmPlay, AUDIO_Play, fail. ret = %d "
#define MMIWRE_MEDIA_461_112_2_18_3_10_45_75 "OpenMedia lpmmd is NULL "
#define MMIWRE_MEDIA_465_112_2_18_3_10_45_76 "OpenMedia, isPlay = %d, eType = %d"
#define MMIWRE_MEDIA_469_112_2_18_3_10_45_77 "OpenMedia hmmd is NULL "
#define MMIWRE_MEDIA_481_112_2_18_3_10_45_78 "OpenMedia play Format error, lpFormat = %d "
#define MMIWRE_MEDIA_491_112_2_18_3_10_45_79 "OpenMedia unReserved0 != 0, lpFormat = %d "
#define MMIWRE_MEDIA_506_112_2_18_3_10_45_80 "OpenMedia record, DataLen = %d, Formate = %d, eType = %d"
#define MMIWRE_MEDIA_513_112_2_18_3_10_45_81 "OpenMedia record pDataOrFn malloc error "
#define MMIWRE_MEDIA_535_112_2_18_3_10_45_82 "OpenMedia play file dwDataLen = 0"
#define MMIWRE_MEDIA_544_112_2_18_3_10_45_83 "OpenMedia play file is not exist "
#define MMIWRE_MEDIA_552_112_2_18_3_10_45_84 "OpenMedia play file pDataOrFn malloc error"
#define MMIWRE_MEDIA_564_112_2_18_3_10_45_85 "OpenMedia play buffer, lpData = NULL or dwDataLen = %d"
#define MMIWRE_MEDIA_599_112_2_18_3_10_45_86 "OpenMedia play stream format error "
#define MMIWRE_MEDIA_609_112_2_18_3_10_45_87 "OpenMedia play error, eType = %d"
#define MMIWRE_MEDIA_657_112_2_18_3_10_45_88 "OpenMedia request handle fail "
#define MMIWRE_MEDIA_683_112_2_18_3_10_45_89 "CloseMedia hmmd is NULL "
#define MMIWRE_MEDIA_686_112_2_18_3_10_45_90 "CloseMedia, hmmd = 0x%x, hmmd->handle = %d"
#define MMIWRE_MEDIA_744_112_2_18_3_10_46_91 "PlayMedia error, hmmd = NULL"
#define MMIWRE_MEDIA_748_112_2_18_3_10_46_92 ">>>PlayMedia=%d,%d,%d"
#define MMIWRE_MEDIA_802_112_2_18_3_10_46_93 "PauseMedia error, hmmd is NULL "
#define MMIWRE_MEDIA_805_112_2_18_3_10_46_94 "PauseMedia, eState = %d, handle = %d, s_hasExtPaused = %d"
#define MMIWRE_MEDIA_820_112_2_18_3_10_46_95 "PauseMedia, eType == TONE"
#define MMIWRE_MEDIA_851_112_2_18_3_10_46_96 "PauseMedia, Pause fail, unReserved0 = %d"
#define MMIWRE_MEDIA_880_112_2_18_3_10_46_97 "ResumeMedia error, hmmd is NULL "
#define MMIWRE_MEDIA_883_112_2_18_3_10_46_98 "ResumeMedia, eState = %d, handle = %d, s_hasExtPaused = %d"
#define MMIWRE_MEDIA_888_112_2_18_3_10_46_99 "ResumeMedia, eType == TONE"
#define MMIWRE_MEDIA_931_112_2_18_3_10_46_100 "ResumeMedia, Resume fail, unReserved0 = %d "
#define MMIWRE_MEDIA_961_112_2_18_3_10_46_101 "CloseMedia error, hmmd is NULL"
#define MMIWRE_MEDIA_964_112_2_18_3_10_46_102 "CloseMedia, hmmd = 0x%x, handle = %d, eState = %d"
#define MMIWRE_MEDIA_975_112_2_18_3_10_46_103 "StopMedia, stop tone"
#define MMIWRE_MEDIA_1086_112_2_18_3_10_46_104 "SetMediaVolume error, hmmd is NULL "
#define MMIWRE_MEDIA_1089_112_2_18_3_10_46_105 "SetMediaVolume, eState = %d, dwValue = %d"
#define MMIWRE_MEDIA_1101_112_2_18_3_10_46_106 "SetMediaVolume, Set volume fail, handle = %d"
#define MMIWRE_MEDIA_1137_112_2_18_3_10_46_107 "GetMediaVolume, invalid param"
#define MMIWRE_MEDIA_1142_112_2_18_3_10_46_108 "GetMediaVolume, curVol = %d"
#define MMIWRE_MEDIA_1333_112_2_18_3_10_47_109 "RecordMedia error, hmmd is NULL"
#define MMIWRE_MEDIA_1336_112_2_18_3_10_47_110 "RecordMedia, eType = %d, eState = %d"
#define MMIWRE_MEDIA_1364_112_2_18_3_10_47_111 "RecordMedia, Record fail, handle = %d "
#define MMIWRE_MEDIA_1475_112_2_18_3_10_47_112 ">>>PauseWREMM=%d,%d,%d"
#define MMIWRE_MEDIA_1525_112_2_18_3_10_47_113 "MMIWRE_ResumeMM, hmmd is NULL "
#define MMIWRE_MEDIA_1528_112_2_18_3_10_47_114 ">>>ResumeWREMM=%d,%d,%d"
#define MMIWRE_MEDIA_1583_112_2_18_3_10_47_115 "MMIWRE_StopMM, hmmd is NULL "
#define MMIWRE_MEDIA_1587_112_2_18_3_10_47_116 "MMIWRE_StopMM=%d, %d, %d, %d"
#define MMIWRE_MEDIA_1856_112_2_18_3_10_48_117 "[WRE] AudioPlayCallback, hAudio=0x%x, notify_info=%d,affix_info=%d"
#define MMIWRE_MEDIA_1916_112_2_18_3_10_48_118 "[WRE] WreGetRingCodec, ring_type=%d, codec=0x%x"
#define MMIWRE_MEDIA_1963_112_2_18_3_10_48_119 "[WRE]GetAudioDev, is_a2dp=%d, ring_fmt=%d"
#define MMIWRE_MEDIA_2052_112_2_18_3_10_48_120 "[WRE]:audiohandle=0x%x,hAudioCodec=0x%x,real_audio_codec=0x%x,hDevice=0x%x,result=%d"
#define MMIWRE_OTHER_679_112_2_18_3_10_50_121 "WreMwinWin_HandleMsg: msg_id = 0x%08x "
#define MMIWRE_OTHER_790_112_2_18_3_10_50_122 "WRE  MSG_KEYDOWN_CANCEL 31"
#define MMIWRE_OTHER_1876_112_2_18_3_10_53_123 "read main file header hFile=%lx err"
#define MMIWRE_OTHER_1884_112_2_18_3_10_53_124 "no enough memory room"
#define MMIWRE_OTHER_1905_112_2_18_3_10_53_125 "set file pointer error hFile=%lx err"
#define MMIWRE_OTHER_1914_112_2_18_3_10_53_126 "read main file branch info hFile=%lx err"
#define MMIWRE_OTHER_1926_112_2_18_3_10_53_127 "[Szip]:file_name_len = %d, file_size = %d, zip_size = %d, reserved = %d"
#define MMIWRE_OTHER_1937_112_2_18_3_10_53_128 "set file pointer error hFile=%lx err"
#define MMIWRE_OTHER_1944_112_2_18_3_10_53_129 "read main file branch info hFile=%lx err"
#define MMIWRE_OTHER_1989_112_2_18_3_10_53_130 "set file pointer error hFile=%lx err"
#define MMIWRE_OTHER_2021_112_2_18_3_10_53_131 "create branch file hFile=%lx err"
#define MMIWRE_OTHER_2155_112_2_18_3_10_53_132 "unzip CreateFile srcfile failure"
#define MMIWRE_OTHER_2161_112_2_18_3_10_53_133 "unzip CreateFile dstfile failure"
#define MMIWRE_OTHER_2171_112_2_18_3_10_53_134 "unzip UnZipcompressfile failure"
#define MMIWRE_OTHER_2235_112_2_18_3_10_53_135 "Invalid install id"
#define MMIWRE_OTHER_2312_112_2_18_3_10_53_136 "idx = %d,appid = %d"
#define MMIWRE_OTHER_2366_112_2_18_3_10_54_137 "DeleteApp: Open Ini File return = %x"
#define MMIWRE_OTHER_2371_112_2_18_3_10_54_138 "DeleteApp: Create Ini File return = %x"
#define MMIWRE_OTHER_2379_112_2_18_3_10_54_139 "DeleteApp: GetFileSize handle= %x, fileSize = %d"
#define MMIWRE_OTHER_2387_112_2_18_3_10_54_140 "DeleteApp: fileSize = 0, malloc WRE_UNINSTALLAPPINI Fail!"
#define MMIWRE_OTHER_2397_112_2_18_3_10_54_141 "DeleteApp: fileSize = 0, malloc WRE_APPGUID Fail!"
#define MMIWRE_OTHER_2415_112_2_18_3_10_54_142 "DeleteApp: fileSize != 0, malloc WRE_UNINSTALLAPPINI Fail!"
#define MMIWRE_OTHER_2424_112_2_18_3_10_54_143 "DeleteApp: fileSize != 0, malloc WreAppTmp Fail!"
#define MMIWRE_OTHER_2433_112_2_18_3_10_54_144 "DeleteApp: ReadFile handle= %x, iByteRead = %d"
#define MMIWRE_OTHER_2453_112_2_18_3_10_54_145 "DeleteApp: WriteFile num return = %x, iByteSize = %d, iByteWrite = %d"
#define MMIWRE_OTHER_2460_112_2_18_3_10_54_146 "DeleteApp: WriteFile data return = %x, iByteSize = %d, iByteWrite = %d"
#define MMIWRE_OTHER_2749_112_2_18_3_10_54_147 "idx = %d,appid = %d"
#define MMIWRE_OTHER_2789_112_2_18_3_10_54_148 "MYW:filepath = %s, SFS_FindFirstFile return %d"
#define MMIWRE_OTHER_3108_112_2_18_3_10_55_149 "WRE_InstallApp: return %d"
#define MMIWRE_OTHER_3117_112_2_18_3_10_55_150 "WRE_InstallApp: user ICON Default return %d"
#define MMIWRE_OTHER_3147_112_2_18_3_10_55_151 "[WRE]: UninstallAPP--Find appName fail!"
#define MMIWRE_OTHER_3158_112_2_18_3_10_55_152 "WRE_UninstallApp %d"
#define MMIWRE_OTHER_3162_112_2_18_3_10_55_153 "RefreshInstallInfo %d"
#define MMIWRE_OTHER_3270_112_2_18_3_10_55_154 "[WRE]   WREEntry return 0x%x"
#define MMIWRE_OTHER_3332_112_2_18_3_10_56_155 "blue tooth event, event no %lx"
#define MMIWRE_OTHER_3424_112_2_18_3_10_56_156 "Set WreSysDir[%d] dir to Hide return %d,  dwAttri = %d"
#define MMIWRE_OTHER_3530_112_2_18_3_10_56_157 "WRE_GetMultiKeyMsg, numkeys %d"
#define MMIWRE_OTHER_3534_112_2_18_3_10_56_158 "WRE_GetMultiKeyMsg, keycode %lx"
#define MMIWRE_OTHER_3578_112_2_18_3_10_56_159 "MMIWRE_InitDebugMode: read debug file (uReadedByte = %d)"
#define MMIWRE_OTHER_3608_112_2_18_3_10_56_160 "[wre]:MMIWRE_InitRuntimePath---"
#define MMIWRE_OTHER_3617_112_2_18_3_10_56_161 "[wre]:MMIWRE_PrintNVInfo---begin"
#define MMIWRE_OTHER_3619_112_2_18_3_10_56_162 "current Path"
#define MMIWRE_OTHER_3624_112_2_18_3_10_56_163 "previous Path"
#define MMIWRE_OTHER_3629_112_2_18_3_10_56_164 "preload Path"
#define MMIWRE_OTHER_3632_112_2_18_3_10_56_165 "type = %lx"
#define MMIWRE_OTHER_3636_112_2_18_3_10_56_166 "type = %lx"
#define MMIWRE_OTHER_3640_112_2_18_3_10_56_167 "type = %lx"
#define MMIWRE_OTHER_3643_112_2_18_3_10_56_168 "[wre]:MMIWRE_PrintNVInfo---end"
#define MMIWRE_OTHER_3648_112_2_18_3_10_56_169 "[wre]:MMIWRE_ResetRuntimePath---"
#define MMIWRE_OTHER_3667_112_2_18_3_10_56_170 "[wre]:MMIWRE_ReadNV---"
#define MMIWRE_OTHER_3702_112_2_18_3_10_56_171 "[wre]:MMIWRE_WriteNV---"
#define MMIWRE_OTHER_3734_112_2_18_3_10_56_172 "[WRE]RestoreWREEnv: kernel fail!"
#define MMIWRE_OTHER_3743_112_2_18_3_10_56_173 "[WRE]RestoreWREEnv: store fail!"
#define MMIWRE_OTHER_3762_112_2_18_3_10_56_174 "[WRE]RestoreFiles: sysPathType = %d"
#define MMIWRE_OTHER_3769_112_2_18_3_10_56_175 "[WRE]RestoreFiles: (kernel)preload path not found!"
#define MMIWRE_OTHER_3783_112_2_18_3_10_56_176 "[WRE]RestoreWREEnv: (kernel)PreloadType is WRE_PRELOAD_EXE"
#define MMIWRE_OTHER_3791_112_2_18_3_10_56_177 "[WRE]RestoreFiles: (store)preload path not found!"
#define MMIWRE_OTHER_3806_112_2_18_3_10_56_178 "[WRE]RestoreWREEnv: (store)PreloadType is WRE_PRELOAD_EXE"
#define MMIWRE_OTHER_3811_112_2_18_3_10_56_179 "[WRE]RestoreFiles: sysPathType not support!"
#define MMIWRE_OTHER_3818_112_2_18_3_10_57_180 "[WRE]RestoreFiles: path not found!"
#define MMIWRE_OTHER_3829_112_2_18_3_10_57_181 "[WRE]RestoreWREEnv: Create WRE dir Fail!"
#define MMIWRE_OTHER_3849_112_2_18_3_10_57_182 "[WRE]RestoreWREEnv: Copy Preload files Fail!"
#define MMIWRE_OTHER_3872_112_2_18_3_10_57_183 "[WRE]RestoreWREEnv: Copy Preload Tree Fail!"
#define MMIWRE_OTHER_3896_112_2_18_3_10_57_184 "[WRE] WRE_FileCopy Fail: src not found"
#define MMIWRE_OTHER_3904_112_2_18_3_10_57_185 "[WRE] WRE_FileCopy Fail: Create dst file fail"
#define MMIWRE_OTHER_3913_112_2_18_3_10_57_186 "[WRE] WRE_FileCopy Fail: file Size == 0"
#define MMIWRE_OTHER_3924_112_2_18_3_10_57_187 "[WRE] WRE_FileCopy Fail: SCI_ALLOC_APP fail"
#define MMIWRE_OTHER_3943_112_2_18_3_10_57_188 "[WRE]WRE_FILECOPY Fail: ReadFile Error(%lx)"
#define MMIWRE_OTHER_3950_112_2_18_3_10_57_189 "[WRE]WRE_FILECOPY Fail: WriteFile Error(%lx)"
#define MMIWRE_OTHER_4093_112_2_18_3_10_57_190 "enter WRE_RotateLCD"
#define MMIWRE_SOCKET_456_112_2_18_3_10_59_191 "ASYNCSOCK_HandlePsMsg, signal_ptr->msg_id = %lx"
#define MMIWRE_SOCKET_699_112_2_18_3_10_59_192 "MWIN TASK ENTRY SOCK, EVENT:%d"
#define MMIWRE_SOCKET_709_112_2_18_3_10_59_193 "DEFAULT:MWIN TASK ENTRY APP, EVENT:%d"
#define MMIWRE_STARTUP_109_112_2_18_3_11_0_194 "[WRE]off sensorautortation:g_old_sensor_open_flag=%d"
#define MMIWRE_STARTUP_138_112_2_18_3_11_0_195 "[WRE]on sensorautortation:g_old_sensor_open_flag=%d"
#define MMIWRE_STARTUP_196_112_2_18_3_11_0_196 "WREMM MMIWRE_MaxmizeAPP 1"
#define MMIWRE_STARTUP_199_112_2_18_3_11_0_197 "WREMM MMIWRE_MaxmizeAPP 2"
#define MMIWRE_STARTUP_201_112_2_18_3_11_0_198 "WREMM MMIWRE_MaxmizeAPP 3"
#define MMIWRE_STARTUP_203_112_2_18_3_11_0_199 "WREMM MMIWRE_MaxmizeAPP 4"
#define MMIWRE_STARTUP_209_112_2_18_3_11_0_200 "WREMM MMIWRE_CloseAPP 1"
#define MMIWRE_STARTUP_211_112_2_18_3_11_0_201 "WREMM MMIWRE_CloseAPP 2"
#define MMIWRE_STARTUP_222_112_2_18_3_11_0_202 "MMIWRE_MaxmizeAPPWithName   count [%d]\n"
#define MMIWRE_STARTUP_286_112_2_18_3_11_0_203 "MMIWRE_MaxmizeAPPWithName   count [%d]\n"
#define MMIWRE_STARTUP_342_112_2_18_3_11_0_204 "k_app_pad2 called "
#define MMIWRE_STARTUP_343_112_2_18_3_11_0_205 "k_app_pad2 called2[%s][%s] "
#define MMIWRE_STARTUP_347_112_2_18_3_11_0_206 "k_app_pad2:%s"
#define MMIWRE_STARTUP_348_112_2_18_3_11_0_207 "snaddr:%x"
#define MMIWRE_STARTUP_349_112_2_18_3_11_0_208 "snaddr:%x"
#define MMIWRE_STARTUP_356_112_2_18_3_11_0_209 "fv:%x"
#define MMIWRE_STARTUP_359_112_2_18_3_11_0_210 "end testvsn:%s"
#define MMIWRE_STARTUP_386_112_2_18_3_11_1_211 "LoadWreLib"
#define MMIWRE_STARTUP_387_112_2_18_3_11_1_212 "LoadWreLib"
#define MMIWRE_STARTUP_388_112_2_18_3_11_1_213 "LoadWreLib"
#define MMIWRE_STARTUP_389_112_2_18_3_11_1_214 "LoadWreLib"
#define MMIWRE_STARTUP_390_112_2_18_3_11_1_215 "LoadWreLib"
#define MMIWRE_STARTUP_391_112_2_18_3_11_1_216 "LoadWreLib"
#define MMIWRE_STARTUP_401_112_2_18_3_11_1_217 "wk wrekernel.wrd not exist!!!!!\n"
#define MMIWRE_STARTUP_415_112_2_18_3_11_1_218 "wk filesize:%d!!!\n"
#define MMIWRE_STARTUP_417_112_2_18_3_11_1_219 "wk address:%x!!!\n"
#define MMIWRE_STARTUP_429_112_2_18_3_11_1_220 "wk file closed!!!\n"
#define MMIWRE_STARTUP_566_112_2_18_3_11_1_221 "[WRE]: startup a application\n"
#define MMIWRE_STARTUP_595_112_2_18_3_11_1_222 "error:WRE_INIT_LOADKERNEL_FALI \n"
#define MMIWRE_STARTUP_614_112_2_18_3_11_1_223 " wre KA_DOWNLOAD_WRE \n"
#define MMIWRE_STARTUP_619_112_2_18_3_11_1_224 " wre KA_NORMAL \n"
#define MMIWRE_STARTUP_638_112_2_18_3_11_1_225 "====== Add wrekernel into check node: %s =======\n"
#define MMIWRE_STARTUP_640_112_2_18_3_11_1_226 "====== wrekernel upload addr: 0x%08x, kernel size: %d =======\n"
#define MMIWRE_STARTUP_650_112_2_18_3_11_1_227 "error:WRE_INIT_NO_ROOM \n"
#define MMIWRE_STARTUP_660_112_2_18_3_11_1_228 " load wk ok \n"
#define MMIWRE_STARTUP_691_112_2_18_3_11_1_229 "WRE boot_reboot HandleMsg 0"
#define MMIWRE_STARTUP_694_112_2_18_3_11_1_230 "WRE boot_reboot HandleMsg 1 ret=%d"
#define MMIWRE_STARTUP_705_112_2_18_3_11_1_231 "[WRE]on sensorautortation:g_old_sensor_open_flag=%d"
#define MMIWRE_STARTUP_809_112_2_18_3_11_1_232 "There is no dpaper file!\n"
#define MMIWRE_STARTUP_813_112_2_18_3_11_1_233 "dpaper file size:%d!!!\n"
#define MMIWRE_STARTUP_816_112_2_18_3_11_1_234 "dpaper address:%x!!!\n"
#define MMIWRE_STARTUP_830_112_2_18_3_11_1_235 "dpaper file closed!!!\n"
#define MMIWRE_TAPI_239_112_2_18_3_11_2_236 "mmiwre Adapt_APPSendSms send fail"
#define MMIWRE_TAPI_279_112_2_18_3_11_2_237 "MMISMS: GetTransmitterDispString number_type = %d, number_plan = %d"
#define MMIWRE_VIDEOPLAYER_769_112_2_18_3_11_6_238 "[Wre Player] CreateMediaPlayer ui callback is null"
#define MMIWRE_VIDEOPLAYER_777_112_2_18_3_11_6_239 "[Wre Player] CreateMediaPlayer create  fail"
#define MMIWRE_VIDEOPLAYER_780_112_2_18_3_11_6_240 "[Wre Player] CreateMediaPlayer create handle = %x"
#define MMIWRE_VIDEOPLAYER_784_112_2_18_3_11_6_241 "[Wre Player] CreateMediaPlayer dispath msg fail"
#define MMIWRE_VIDEOPLAYER_815_112_2_18_3_11_6_242 "[Wre Player] DestroyVideoPlayer hVideo = %x"
#define MMIWRE_VIDEOPLAYER_819_112_2_18_3_11_6_243 "[Wre Player] DestroyVideoPlayer some wrong with hVideo = %x"
#define MMIWRE_VIDEOPLAYER_855_112_2_18_3_11_6_244 "[Wre Player] PlayVideo some wrong with hVideo = %x,playParam = %x"
#define MMIWRE_VIDEOPLAYER_859_112_2_18_3_11_6_245 "[Wre Player] PlayVideo begin"
#define MMIWRE_VIDEOPLAYER_862_112_2_18_3_11_6_246 "[Wre Player] PlayVideo result = %d"
#define MMIWRE_VIDEOPLAYER_883_112_2_18_3_11_6_247 "[Wre Player] StopVideo some wrong with hVideo = %x"
#define MMIWRE_VIDEOPLAYER_887_112_2_18_3_11_6_248 "[Wre Player] StopVideo begin"
#define MMIWRE_VIDEOPLAYER_889_112_2_18_3_11_6_249 "[Wre Player] StopVideo result = %d"
#define MMIWRE_VIDEOPLAYER_906_112_2_18_3_11_6_250 "[Wre Player] PauseVideo some wrong with hVideo = %x"
#define MMIWRE_VIDEOPLAYER_909_112_2_18_3_11_6_251 "[Wre Player] PauseVideo begin"
#define MMIWRE_VIDEOPLAYER_913_112_2_18_3_11_6_252 "[Wre Player] PauseVideo result = %d"
#define MMIWRE_VIDEOPLAYER_934_112_2_18_3_11_6_253 "[Wre Player] ResumeVideo some wrong with hVideo = %x"
#define MMIWRE_VIDEOPLAYER_937_112_2_18_3_11_6_254 "[Wre Player] ResumeVideo begin"
#define MMIWRE_VIDEOPLAYER_940_112_2_18_3_11_6_255 "[Wre Player] ResumeVideo result = %d"
#define MMIWRE_VIDEOPLAYER_959_112_2_18_3_11_6_256 "[Wre Player] SeekVideo some wrong with hVideo = %x"
#define MMIWRE_VIDEOPLAYER_962_112_2_18_3_11_6_257 "[Wre Player] SeekVideo begin"
#define MMIWRE_VIDEOPLAYER_966_112_2_18_3_11_6_258 "[Wre Player] SeekVideo result = %d"
#define MMIWRE_VIDEOPLAYER_983_112_2_18_3_11_7_259 "[Wre Player] ExitVideo the fsm is wrong hVideo = %x"
#define MMIWRE_VIDEOPLAYER_1002_112_2_18_3_11_7_260 "[Wre Player] CompulsivePauseVideo the fsm is wrong hVideo = %x"
#define MMIWRE_VIDEOPLAYER_1006_112_2_18_3_11_7_261 "[Wre Player] CompulsivePauseVideo begin fsm_ptr->is_to_resume = %d"
#define MMIWRE_VIDEOPLAYER_1023_112_2_18_3_11_7_262 "[Wre Player] CompulsivePauseVideo end fsm_ptr->is_to_pause = %d"
#define MMIWRE_VIDEOPLAYER_1041_112_2_18_3_11_7_263 "[Wre Player] CompulsiveResumeVideo the fsm is wrong hVideo = %x"
#define MMIWRE_VIDEOPLAYER_1044_112_2_18_3_11_7_264 "[Wre Player] CompulsiveResumeVideo begin fsm_ptr->is_to_pause = %d"
#define MMIWRE_VIDEOPLAYER_1100_112_2_18_3_11_7_265 "[Wre Player] CompulsiveResumeVideo end fsm_ptr->is_to_resume = %d"
#define MMIWRE_VIDEOPLAYER_1123_112_2_18_3_11_7_266 "[Wre Player] GetMediaPlayerState state = %d"
#define MMIWRE_VIDEOPLAYER_1139_112_2_18_3_11_7_267 "[Wre Player] StopSetRate some wrong with hVideo = %x"
#define MMIWRE_VIDEOPLAYER_1142_112_2_18_3_11_7_268 "[Wre Player] StopSetRate begin"
#define MMIWRE_VIDEOPLAYER_1146_112_2_18_3_11_7_269 "[Wre Player] StopSetRate result = %d"
#define MMIWRE_VIDEOPLAYER_1161_112_2_18_3_11_7_270 "[Wre Player] StartSetRate some wrong with hVideo = %x"
#define MMIWRE_VIDEOPLAYER_1164_112_2_18_3_11_7_271 "[Wre Player] StartSetRate begin"
#define MMIWRE_VIDEOPLAYER_1168_112_2_18_3_11_7_272 "[Wre Player] StartSetRate result = %d"
#define MMIWRE_VIDEOPLAYER_1320_112_2_18_3_11_7_273 "[Wre Player] GetVideoTimeOffset = %x"
#define MMIWRE_VIDEOPLAYER_1349_112_2_18_3_11_7_274 "[Wre Player] GetFrameOffset info_result = %d, *dwFrameIndex = %d, time_offset = %d, media_info.video_info.frame_rate = %d, base = %d"
#define MMIWRE_VIDEOPLAYER_1370_112_2_18_3_11_7_275 "[Wre Player] SwitchWmpScreen the fsm is wrong fsm_ptr = %x"
#define MMIWRE_VIDEOPLAYER_1410_112_2_18_3_11_7_276 "[Wre Player] SwitchPlayDevice the fsm is wrong fsm_ptr = %x"
#define MMIWRE_VIDEOPLAYER_1414_112_2_18_3_11_7_277 "[Wre Player] SwitchPlayDevice is_bt_support = %d, bSupportA2dp = %d"
#define MMIWRE_VIDEOPLAYER_1451_112_2_18_3_11_8_278 "[Wre Player] AuthRsp some wrong with hVideo = %x"
#define MMIWRE_VIDEOPLAYER_1457_112_2_18_3_11_8_279 "[Wre Player] AuthRsp result = %d"
#define MMIWRE_VIDEOPLAYER_1471_112_2_18_3_11_8_280 "[Wre Player] GetMediaInfo param is error"
#define MMIWRE_VIDEOPLAYER_1492_112_2_18_3_11_8_281 "[Wre Player] GetMediaInfo param is error"
#define MMIWRE_VIDEOPLAYER_1611_112_2_18_3_11_8_282 "[Wre Player]: GetMediaInfoFromFullPathName() param error"
#define MMIWRE_VIDEOPLAYER_1621_112_2_18_3_11_8_283 "[Wre Player]: GetMediaInfoFromFullPathName: open dplayer fail! "
#define MMIWRE_VIDEOPLAYER_1629_112_2_18_3_11_8_284 "[Wre Player]: GetMediaInfoFromFullPathName() return = 0x%.2x, full_name_len = %d"
#define MMIWRE_VIDEOPLAYER_1640_112_2_18_3_11_8_285 "[Wre Player]: GetMediaInfoFromFullPathName():ntfile_type = %d,ntlen = %d,ntv_spt = %d,ntf_rate = %d,ntheight = %d,ntwidth = %d,ntvideo_type = %d,ntaudio_type = %d"
#define MMIWRE_VIDEOPLAYER_1662_112_2_18_3_11_8_286 "[Wre Player]: GetMediaInfoFromBuf param error"
#define MMIWRE_VIDEOPLAYER_1666_112_2_18_3_11_8_287 "[Wre Player]: GetMediaInfoFromBuf: video_type = %d, video_length = %d"
#define MMIWRE_VIDEOPLAYER_1723_112_2_18_3_11_8_288 "[Wre Player] CheckSupportedTypeFromMediaInfo param is null"
#define MMIWRE_VIDEOPLAYER_1732_112_2_18_3_11_8_289 "[Wre Player] CheckSupportedTypeFromMediaInfo video_support = %d, audio_support = %d , video_type = %d, audio_type = %d, height = %d, width = %d"
#define MMIWRE_VIDEOPLAYER_1765_112_2_18_3_11_8_290 "[Wre Player] GetSupportedStatus param is null"
#define MMIWRE_VIDEOPLAYER_1828_112_2_18_3_11_8_291 "[Wre Player] FillRectWithTransparenceColor before x= %d, y=%d,width = %d, height = %d"
#define MMIWRE_VIDEOPLAYER_1841_112_2_18_3_11_8_292 "[Wre Player] FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d"
#define MMIWRE_VIDEOPLAYER_1900_112_2_18_3_11_8_293 "[Wre Player] FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d"
#define MMIWRE_VIDEOPLAYER_1992_112_2_18_3_11_9_294 "[Wre Player] DispatchSignalToFsmMsg STRM_INIT_CNF"
#define MMIWRE_VIDEOPLAYER_2010_112_2_18_3_11_9_295 "[Wre Player] DispatchSignalToFsmMsg STRM_PLAY_CNF"
#define MMIWRE_VIDEOPLAYER_2026_112_2_18_3_11_9_296 "[Wre Player] DispatchSignalToFsmMsg STRM_BUFFERING_IND"
#define MMIWRE_VIDEOPLAYER_2034_112_2_18_3_11_9_297 "[Wre Player] DispatchSignalToFsmMsg STRM_BUFFER_END_IND"
#define MMIWRE_VIDEOPLAYER_2042_112_2_18_3_11_9_298 "[Wre Player] DispatchSignalToFsmMsg STRM_DATA_TIMEOUT_IND"
#define MMIWRE_VIDEOPLAYER_2051_112_2_18_3_11_9_299 "[Wre Player] DispatchSignalToFsmMsg strm_buf_process_ptr = %x"
#define MMIWRE_VIDEOPLAYER_2057_112_2_18_3_11_9_300 "[Wre Player] DispatchSignalToFsmMsg process = %d, total = %d"
#define MMIWRE_VIDEOPLAYER_2064_112_2_18_3_11_9_301 "[Wre Player] DispatchSignalToFsmMsg percent = %d"
#define MMIWRE_VIDEOPLAYER_2069_112_2_18_3_11_9_302 "[Wre Player] DispatchSignalToFsmMsg STRM_GOODBYE_IND"
#define MMIWRE_VIDEOPLAYER_2078_112_2_18_3_11_9_303 "[Wre Player] DispatchSignalToFsmMsg STRM_SEEK_CNF"
#define MMIWRE_VIDEOPLAYER_2096_112_2_18_3_11_9_304 "[Wre Player] DispatchSignalToFsmMsg STRM_PAUSE_CNF"
#define MMIWRE_VIDEOPLAYER_2114_112_2_18_3_11_9_305 "[Wre Player] DispatchSignalToFsmMsg STRM_RESUME_CNF"
#define MMIWRE_VIDEOPLAYER_2131_112_2_18_3_11_9_306 "[Wre Player] DispatchSignalToFsmMsg STRM_STOP_CNF"
#define MMIWRE_VIDEOPLAYER_2148_112_2_18_3_11_9_307 "[Wre Player] DispatchSignalToFsmMsg STRM_CLOSE_CNF"
#define MMIWRE_VIDEOPLAYER_2163_112_2_18_3_11_9_308 "[Wre Player] DispatchSignalToFsmMsg APP_WMP_FRAME_END_IND"
#define MMIWRE_VIDEOPLAYER_2172_112_2_18_3_11_9_309 "[Wre Player] DispatchSignalToFsmMsg APP_WMP_END_IND"
#define MMIWRE_VIDEOPLAYER_2185_112_2_18_3_11_9_310 "[Wre Player] DispatchSignalToFsmMsg APP_WMP_PLAY_NEXT_FILE_REQ"
#define MMIWRE_VIDEOPLAYER_2292_112_2_18_3_11_9_311 "[Wre Player] SettingAsScreensaver full_name_ptr = %x, full_name_len = %d"
#define MMIWRE_VIDEOPLAYER_2341_112_2_18_3_11_9_312 "[Wre Player] SettingAsPowerOnOffAnimation is_power_on = %d, full_name_ptr = %x,full_name_len = %d"
#define MMIWRE_VIDEOPLAYER_2424_112_2_18_3_11_9_313 "[Wre Player] CreatMsg play_param_ptr->playType = %d, msg_body_size = %d"
#define MMIWRE_VIDEOPLAYER_2431_112_2_18_3_11_9_314 "[Wre Player] CreatMsg full path ptr is null"
#define MMIWRE_VIDEOPLAYER_2440_112_2_18_3_11_10_315 "[Wre Player] CreatMsg buffer is null"
#define MMIWRE_VIDEOPLAYER_2448_112_2_18_3_11_10_316 "[Wre Player] CreatMsg url is null"
#define MMIWRE_VIDEOPLAYER_2451_112_2_18_3_11_10_317 "[Wre Player] CreatMsg msg_body_size = %d"
#define MMIWRE_VIDEOPLAYER_2453_112_2_18_3_11_10_318 "[Wre Player] CreatMsg msg_body_size = %d"
#define MMIWRE_VIDEOPLAYER_2466_112_2_18_3_11_10_319 "[Wre Player] msg_ptr = %x, len = %d"
#define MMIWRE_VIDEOPLAYER_2478_112_2_18_3_11_10_320 " play_param2_ptr = %x, msg_body_size =%d"
#define MMIWRE_VIDEOPLAYER_2490_112_2_18_3_11_10_321 "[Wre Player] CreatMsg play_param2_ptr->strUrl = %x, len = %x"
#define MMIWRE_VIDEOPLAYER_2494_112_2_18_3_11_10_322 " play_param2_ptr->strFilePath = %x"
#define MMIWRE_VIDEOPLAYER_2496_112_2_18_3_11_10_323 " play_param2_ptr->strFilePath = %x"
#define MMIWRE_VIDEOPLAYER_2497_112_2_18_3_11_10_324 " play_param_ptr->strFilePath = %x, play_param_ptr->uFilePathLen = %d"
#define MMIWRE_VIDEOPLAYER_2499_112_2_18_3_11_10_325 " play_param2_ptr->strFilePath = %x"
#define MMIWRE_VIDEOPLAYER_2581_112_2_18_3_11_10_326 "[Wre Player] CreatMsg frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d"
#define MMIWRE_VIDEOPLAYER_2598_112_2_18_3_11_10_327 "[Wre Player] CreatMsg strm_init_info_ptr->sdp_info.video_param.config_len = %d"
#define MMIWRE_VIDEOPLAYER_2618_112_2_18_3_11_10_328 "[Wre Player] CreatMsg sprop_param_sets_num = %d, enc_type = %d, sprop_sets_arr = %x"
#define MMIWRE_VIDEOPLAYER_2826_112_2_18_3_11_10_329 "[Wre Player] CreateFSM there is a fsm"
#define MMIWRE_VIDEOPLAYER_2834_112_2_18_3_11_10_330 "[Wre Player] CreateFSM no space for fsm"
#define MMIWRE_VIDEOPLAYER_2872_112_2_18_3_11_10_331 "[Wre Player] CreateFSM cur fsm is %x"
#define MMIWRE_VIDEOPLAYER_2895_112_2_18_3_11_10_332 "[Wre Player] CreateFSM cur fsm is %x"
#define MMIWRE_VIDEOPLAYER_2921_112_2_18_3_11_10_333 "[Wre Player] DestroyFSM there is no fsm list"
#define MMIWRE_VIDEOPLAYER_2929_112_2_18_3_11_11_334 "[Wre Player] DestroyFSM current_fsm_ptr = %x, current_fsm_ptr->fsm_info_ptr = %x"
#define MMIWRE_VIDEOPLAYER_2985_112_2_18_3_11_11_335 "[Wre Player] DestroyFSM  fsm hVideo = %x"
#define MMIWRE_VIDEOPLAYER_3034_112_2_18_3_11_11_336 "[Wre Player] FsmDispatch something wrong with fsm_ptr = %x"
#define MMIWRE_VIDEOPLAYER_3054_112_2_18_3_11_11_337 "[Wre Player] FsmStateTrans fsm_ptr = %x, state = %d"
#define MMIWRE_VIDEOPLAYER_3082_112_2_18_3_11_11_338 "[Wre Player] SendPrepareIndToUI param is error"
#define MMIWRE_VIDEOPLAYER_3098_112_2_18_3_11_11_339 "[Wre Player] SendPrepareIndToUI send prepare ind to ui"
#define MMIWRE_VIDEOPLAYER_3114_112_2_18_3_11_11_340 "[Wre Player] SendAUTHIndToUI param is error"
#define MMIWRE_VIDEOPLAYER_3123_112_2_18_3_11_11_341 "[Wre Player] SendAUTHIndToUI send auth ind to ui"
#define MMIWRE_VIDEOPLAYER_3139_112_2_18_3_11_11_342 "[Wre Player] SendFatalErrorIndTOUI param is error"
#define MMIWRE_VIDEOPLAYER_3149_112_2_18_3_11_11_343 "[Wre Player] SendFatalErrorIndTOUI send fatal error to ui errorType = %d"
#define MMIWRE_VIDEOPLAYER_3166_112_2_18_3_11_11_344 "[Wre Player] SendProcessIndToUI param is error"
#define MMIWRE_VIDEOPLAYER_3192_112_2_18_3_11_11_345 "[Wre Player] SendBufferingIndToUI param is error"
#define MMIWRE_VIDEOPLAYER_3201_112_2_18_3_11_11_346 "[Wre Player] SendBufferingIndToUI send VIDEO_NOTIFY_BUFFERING to ui"
#define MMIWRE_VIDEOPLAYER_3217_112_2_18_3_11_11_347 "[Wre Player] SendBufferPercentToUI param is error"
#define MMIWRE_VIDEOPLAYER_3227_112_2_18_3_11_11_348 "[Wre Player]  send VIDEO_NOTIFY_BUFFER_END to ui"
#define MMIWRE_VIDEOPLAYER_3243_112_2_18_3_11_11_349 "[Wre Player] SendBufferEndIndToUI param is error"
#define MMIWRE_VIDEOPLAYER_3252_112_2_18_3_11_11_350 "[Wre Player] send VIDEO_NOTIFY_BUFFER_END to ui"
#define MMIWRE_VIDEOPLAYER_3268_112_2_18_3_11_11_351 "[Wre Player] SendPlayEndToUI param is error"
#define MMIWRE_VIDEOPLAYER_3278_112_2_18_3_11_11_352 "[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_PLAY_END to ui"
#define MMIWRE_VIDEOPLAYER_3294_112_2_18_3_11_11_353 "[Wre Player] SendStopIndToUI param is error"
#define MMIWRE_VIDEOPLAYER_3303_112_2_18_3_11_11_354 "[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_STOP to ui"
#define MMIWRE_VIDEOPLAYER_3319_112_2_18_3_11_11_355 "[Wre Player] SendPlayCnfToUI param is error"
#define MMIWRE_VIDEOPLAYER_3329_112_2_18_3_11_11_356 "[Wre Player] SendPlayCnfToUI send WMP_PLAY_CNFto ui errorType = %d"
#define MMIWRE_VIDEOPLAYER_3345_112_2_18_3_11_11_357 "[Wre Player] SendPauseCnfToUI param is error"
#define MMIWRE_VIDEOPLAYER_3355_112_2_18_3_11_11_358 "[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_PAUSE to ui"
#define MMIWRE_VIDEOPLAYER_3371_112_2_18_3_11_11_359 "[Wre Player] SendSeekCnfToUI param is error"
#define MMIWRE_VIDEOPLAYER_3383_112_2_18_3_11_11_360 "[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_SEEK to ui"
#define MMIWRE_VIDEOPLAYER_3429_112_2_18_3_11_12_361 "[VP]FrameToTime handle = 0x%x, info_result = %d,frame_index = %d, time_offset = 0x%x, time_lenth = %d,base = %d"
#define MMIWRE_VIDEOPLAYER_3455_112_2_18_3_11_12_362 "[Wre Player] CallbackEndNotify i_type = %d, s_current_fsm_ptr->task_id = %d"
#define MMIWRE_VIDEOPLAYER_3503_112_2_18_3_11_12_363 "[Wre Player] PlayLocalFile fsm is error"
#define MMIWRE_VIDEOPLAYER_3509_112_2_18_3_11_12_364 "[Wre Player] PlayLocalFile uFilePathLen = %d, strFilePath = %x, %x, %x, %x"
#define MMIWRE_VIDEOPLAYER_3564_112_2_18_3_11_12_365 "[Wre Player] PlayLocalFile fsm_ptr->dplayer_handle = %x"
#define MMIWRE_VIDEOPLAYER_3586_112_2_18_3_11_12_366 "[Wre Player] PlayLocalFile play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d"
#define MMIWRE_VIDEOPLAYER_3596_112_2_18_3_11_12_367 "[Wre Player] PlayLocalFile total_time = %d, process_time = %d, support_type = %d"
#define MMIWRE_VIDEOPLAYER_3626_112_2_18_3_11_12_368 "[Wre Player] PlayLocalFile  dplayer_result = %d"
#define MMIWRE_VIDEOPLAYER_3650_112_2_18_3_11_12_369 "[Wre Player] PlayLocalFile play error"
#define MMIWRE_VIDEOPLAYER_3686_112_2_18_3_11_12_370 "[Wre Player] PlayLocalBuf param is error"
#define MMIWRE_VIDEOPLAYER_3694_112_2_18_3_11_12_371 "[Wre Player] PlayLocalBuf play_param_ptr->videoBufferInfo.fileYype = %d"
#define MMIWRE_VIDEOPLAYER_3721_112_2_18_3_11_12_372 "[Wre Player] PlayLocalBuf fsm_ptr->dplayer_handle = %x"
#define MMIWRE_VIDEOPLAYER_3736_112_2_18_3_11_12_373 "[Wre Player] PlayLocalBuf vidot_type = %d, vidoe_support = %d"
#define MMIWRE_VIDEOPLAYER_3741_112_2_18_3_11_12_374 "[Wre Player] PlayLocalBuf play_param_ptr->bScalUp = %d, fsm_ptr->display_param.nWidth = %d, p_info.video_info.nWidth = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d"
#define MMIWRE_VIDEOPLAYER_3753_112_2_18_3_11_12_375 "[Wre Player] PlayLocalBuf total_time = %d, process_time = %d, support_type = %d"
#define MMIWRE_VIDEOPLAYER_3780_112_2_18_3_11_12_376 "[Wre Player] PlayLocalBuf dplayer_result = %d"
#define MMIWRE_VIDEOPLAYER_3795_112_2_18_3_11_12_377 "[Wre Player] PlayLocalBuf play error"
#define MMIWRE_VIDEOPLAYER_3826_112_2_18_3_11_12_378 "[Wre Player] PlayFromNet param is error"
#define MMIWRE_VIDEOPLAYER_3834_112_2_18_3_11_12_379 "[Wre Player] PlayFromNet fsm_ptr->dplayer_handle = %x"
#define MMIWRE_VIDEOPLAYER_3859_112_2_18_3_11_12_380 "[Wre Player] PlayFromNet play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d"
#define MMIWRE_VIDEOPLAYER_3871_112_2_18_3_11_12_381 "[Wre Player] PlayFromNet total_time = %d, process_time = %d, support_type = %d"
#define MMIWRE_VIDEOPLAYER_3900_112_2_18_3_11_13_382 "[Wre Player] PlayFromNet play error"
#define MMIWRE_VIDEOPLAYER_3932_112_2_18_3_11_13_383 "[Wre Player] HandlePDPMsg param is error"
#define MMIWRE_VIDEOPLAYER_3936_112_2_18_3_11_13_384 "[Wre Player] HandlePDPMsg msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_3978_112_2_18_3_11_13_385 "[Wre Player] ActivePdp param is error"
#define MMIWRE_VIDEOPLAYER_3983_112_2_18_3_11_13_386 "[Wre Player] ActivePdp dual_sys = %d"
#define MMIWRE_VIDEOPLAYER_3991_112_2_18_3_11_13_387 "[Wre Player] ActivePdp sim casd is not ok dual_sys = %d"
#define MMIWRE_VIDEOPLAYER_4000_112_2_18_3_11_13_388 "[Wre Player] ActivePdp param is setting_index = %d"
#define MMIWRE_VIDEOPLAYER_4023_112_2_18_3_11_13_389 "[Wre Player] ActivePdp sim casd is not ok dual_sys = %d"
#define MMIWRE_VIDEOPLAYER_4056_112_2_18_3_11_13_390 "[Wre Player] FsmIdle param is error"
#define MMIWRE_VIDEOPLAYER_4060_112_2_18_3_11_13_391 "[Wre Player] FsmIdle msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_4068_112_2_18_3_11_13_392 "IDLE, got wrong msg"
#define MMIWRE_VIDEOPLAYER_4088_112_2_18_3_11_13_393 "[Wre Player] InitFsmInfo param is null"
#define MMIWRE_VIDEOPLAYER_4091_112_2_18_3_11_13_394 "[Wre Player] InitFsmInfo state = %d, vpCallBack = %x"
#define MMIWRE_VIDEOPLAYER_4127_112_2_18_3_11_13_395 "[Wre Player] UpdateFsmInfo param is null"
#define MMIWRE_VIDEOPLAYER_4166_112_2_18_3_11_13_396 "[Wre Player] UpdateFsmInfo fsm_ptr->strFilePath = %x, play_param_ptr->strFilePath = %x"
#define MMIWRE_VIDEOPLAYER_4243_112_2_18_3_11_13_397 "[Wre Player] FsmReady play param is null"
#define MMIWRE_VIDEOPLAYER_4247_112_2_18_3_11_13_398 "[Wre Player] FsmReady msg_ptr->msg_id = %x, fsm_ptr->has_play_end_reason = %d, fsm_ptr->is_to_exit = %d"
#define MMIWRE_VIDEOPLAYER_4270_112_2_18_3_11_13_399 "[Wre Player] FsmReady play_param_ptr->playType = %d"
#define MMIWRE_VIDEOPLAYER_4388_112_2_18_3_11_14_400 "[Wre Player] FsmReady ready, got wrong msg"
#define MMIWRE_VIDEOPLAYER_4409_112_2_18_3_11_14_401 "[Wre Player] InitStreaming param is error"
#define MMIWRE_VIDEOPLAYER_4528_112_2_18_3_11_14_402 "[Wre Player] FsmPdpActiving fsm or msg error"
#define MMIWRE_VIDEOPLAYER_4532_112_2_18_3_11_14_403 "[Wre Player] FsmPdpActiving msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_4540_112_2_18_3_11_14_404 "[Wre Player] FsmPdpActiving pdp active error"
#define MMIWRE_VIDEOPLAYER_4595_112_2_18_3_11_14_405 "[Wre Player] FsmInit param is error"
#define MMIWRE_VIDEOPLAYER_4599_112_2_18_3_11_14_406 "[Wre Player] FsmInit msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_4643_112_2_18_3_11_14_407 "[Wre Player] PlayFromNet play error"
#define MMIWRE_VIDEOPLAYER_4690_112_2_18_3_11_14_408 "[Wre Player] FsmInit param is error"
#define MMIWRE_VIDEOPLAYER_4694_112_2_18_3_11_14_409 "[Wre Player] FsmInit msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_4699_112_2_18_3_11_14_410 "[Wre Player] FsmInit str_ini_result = %d, is_success_play = %d"
#define MMIWRE_VIDEOPLAYER_4807_112_2_18_3_11_14_411 "[Wre Player] FsmInitAuth param is error"
#define MMIWRE_VIDEOPLAYER_4811_112_2_18_3_11_14_412 "[Wre Player] FsmInitAuth msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_4873_112_2_18_3_11_15_413 "[Wre Player] FsmPlayPeding param is error"
#define MMIWRE_VIDEOPLAYER_4877_112_2_18_3_11_15_414 "[Wre Player] FsmPlayPeding msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_4882_112_2_18_3_11_15_415 "[Wre Player] FsmPlayPeding strm_result = %d"
#define MMIWRE_VIDEOPLAYER_4999_112_2_18_3_11_15_416 "[Wre Player] FsmPlayAuth param is error"
#define MMIWRE_VIDEOPLAYER_5004_112_2_18_3_11_15_417 "[Wre Player] FsmPlayAuth msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_5020_112_2_18_3_11_15_418 "[Wre Player] FsmPlayAuth auth_info.password_ptr= %x, auth_info.username_ptr = %x"
#define MMIWRE_VIDEOPLAYER_5072_112_2_18_3_11_15_419 "[Wre Player] FsmPlaying param is error"
#define MMIWRE_VIDEOPLAYER_5081_112_2_18_3_11_15_420 "[Wre Player] FsmPlaying FSM_ENTRY_REQ fsm_ptr->is_to_pause %x,fsm_ptr->has_play_end_reason = %d"
#define MMIWRE_VIDEOPLAYER_5105_112_2_18_3_11_15_421 "[Wre Player] FsmPlaying WMP_STRM_BUFFERING_IND"
#define MMIWRE_VIDEOPLAYER_5111_112_2_18_3_11_15_422 "[Wre Player] FsmPlaying FSM_CALLBACK_END_IND"
#define MMIWRE_VIDEOPLAYER_5127_112_2_18_3_11_15_423 "[Wre Player] FsmPlaying fsm_ptr->playType = %d"
#define MMIWRE_VIDEOPLAYER_5181_112_2_18_3_11_15_424 "[Wre Player] FsmPlaying FSM_PAUSE_REQ, fsm_ptr->is_not_notify_ui = %d, fsm_ptr->is_to_pause = %d"
#define MMIWRE_VIDEOPLAYER_5214_112_2_18_3_11_15_425 "[Wre Player] FsmPlaying FSM_SEEK_REQ"
#define MMIWRE_VIDEOPLAYER_5250_112_2_18_3_11_15_426 "[Wre Player] FsmPlaying FSM_STOP_REQ"
#define MMIWRE_VIDEOPLAYER_5264_112_2_18_3_11_15_427 "[Wre Player] FsmPlaying FSM_STRM_DATA_TIMEOUT_IND"
#define MMIWRE_VIDEOPLAYER_5272_112_2_18_3_11_15_428 "[Wre Player] FsmPlaying FSM_RESUME_REQ"
#define MMIWRE_VIDEOPLAYER_5313_112_2_18_3_11_15_429 "[Wre Player] FsmBuffering param is error"
#define MMIWRE_VIDEOPLAYER_5318_112_2_18_3_11_15_430 "[Wre Player] FsmBuffering msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_5446_112_2_18_3_11_16_431 "[Wre Player] FsmSeeking param is error"
#define MMIWRE_VIDEOPLAYER_5450_112_2_18_3_11_16_432 "[Wre Player] FsmSeeking  msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_5463_112_2_18_3_11_16_433 "[Wre Player] FsmSeeking strm_result = %d"
#define MMIWRE_VIDEOPLAYER_5542_112_2_18_3_11_16_434 "[Wre Player] FsmPausing param is error"
#define MMIWRE_VIDEOPLAYER_5546_112_2_18_3_11_16_435 "[Wre Player] FsmPausing msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_5559_112_2_18_3_11_16_436 "[Wre Player] FsmPausing strm_result = %d, fsm_ptr->is_not_notify_ui = %d"
#define MMIWRE_VIDEOPLAYER_5568_112_2_18_3_11_16_437 "[Wre Player] FsmPausing send pause cnf"
#define MMIWRE_VIDEOPLAYER_5631_112_2_18_3_11_16_438 "[Wre Player] FsmPaused param is error"
#define MMIWRE_VIDEOPLAYER_5635_112_2_18_3_11_16_439 "[Wre Player] FsmPaused msg_ptr->msg_id = %x, is_to_pause = %d, is_to_resume = %d"
#define MMIWRE_VIDEOPLAYER_5681_112_2_18_3_11_16_440 "[Wre Player] FsmPaused FSM_SEEK_REQ"
#define MMIWRE_VIDEOPLAYER_5782_112_2_18_3_11_16_441 "[Wre Player] FsmPausedSeeking param is error"
#define MMIWRE_VIDEOPLAYER_5786_112_2_18_3_11_16_442 "[Wre Player] FsmPausedSeeking  msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_5808_112_2_18_3_11_16_443 "[Wre Player] FsmSeeking strm_result = %d"
#define MMIWRE_VIDEOPLAYER_5885_112_2_18_3_11_17_444 "[Wre Player] FsmPaused param is error"
#define MMIWRE_VIDEOPLAYER_5889_112_2_18_3_11_17_445 "[Wre Player] FsmPaused msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_5957_112_2_18_3_11_17_446 "[Wre Player] FsmResuming msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_5969_112_2_18_3_11_17_447 "[Wre Player] FsmResuming strm_result = %d"
#define MMIWRE_VIDEOPLAYER_6041_112_2_18_3_11_17_448 "[Wre Player] HandleInitCnf param is null"
#define MMIWRE_VIDEOPLAYER_6044_112_2_18_3_11_17_449 "[Wre Player] HandleInitCnf strm_init_ptr->result = %d"
#define MMIWRE_VIDEOPLAYER_6133_112_2_18_3_11_17_450 "[Wre Player] HandleInitCnf frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d, fsm_ptr->total_time= %d, strm_cfg.video_cfg.cfg_ptr = %x"
#define MMIWRE_VIDEOPLAYER_6164_112_2_18_3_11_17_451 "[Wre Player] HandleUpdateFrame param is null"
#define MMIWRE_VIDEOPLAYER_6184_112_2_18_3_11_17_452 "[Wre Player] HandlePauseReq param is null"
#define MMIWRE_VIDEOPLAYER_6208_112_2_18_3_11_17_453 "[Wre Player] HandlePauseReq can not pause, so stop"
#define MMIWRE_VIDEOPLAYER_6229_112_2_18_3_11_17_454 "[Wre Player] HandleResumeReq param is null"
#define MMIWRE_VIDEOPLAYER_6261_112_2_18_3_11_17_455 "[Wre Player] HandleSeekReq param is null"
#define MMIWRE_VIDEOPLAYER_6266_112_2_18_3_11_17_456 "[Wre Player] FsmPlaying seek_time = %x, seek_param->nSeekFrame = %x"
#define MMIWRE_VIDEOPLAYER_6291_112_2_18_3_11_17_457 "[Wre Player] HandleSeekReq streaming see error"
#define MMIWRE_VIDEOPLAYER_6305_112_2_18_3_11_17_458 "[Wre Player] HandleStopReq param is null"
#define MMIWRE_VIDEOPLAYER_6309_112_2_18_3_11_17_459 "[Wre Player] HandleStopReq is_begin_play = %d"
#define MMIWRE_VIDEOPLAYER_6344_112_2_18_3_11_18_460 "[Wre Player] FsmStopping param is error"
#define MMIWRE_VIDEOPLAYER_6348_112_2_18_3_11_18_461 "[Wre Player] FsmStopping msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_6379_112_2_18_3_11_18_462 "[Wre Player] FsmStopped param is error"
#define MMIWRE_VIDEOPLAYER_6417_112_2_18_3_11_18_463 "[Wre Player] FsmClosing param is error"
#define MMIWRE_VIDEOPLAYER_6421_112_2_18_3_11_18_464 "[Wre Player] FsmClosing msg_ptr->msg_id = %x"
#define MMIWRE_VIDEOPLAYER_6533_112_2_18_3_11_18_465 "[Wre Player] SendSignalToCurPlayingTask signalType = %d, data = %d, task_id = %d"
#define MMIWRE_VIDEOPLAYER_6576_112_2_18_3_11_18_466 "[Wre Player] SwitchDisplayMode the fsm is wrong fsm_ptr = %x"
#define MMIWRE_VIDEOPLAYER_6580_112_2_18_3_11_18_467 "[Wre Player] SwitchDisplayMode fsm_ptr->state = %d, is_only_rect = %d"
#define MMIWRE_VIDEOPLAYER_6625_112_2_18_3_11_18_468 "[Wre Player] SwitchDisplayMode result = %d"
#define MMIWRE_WINTAB_188_112_2_18_3_11_19_469 "[WRE] MMIWRE_SetSimSelect sim_select = %d"
#define MMIWRE_WINTAB_552_112_2_18_3_11_20_470 "[WRE] HandlePsMsg msg_id=%d, result=%d"
#define MMIWRE_WINTAB_562_112_2_18_3_11_20_471 "[WRE] s_wre_net_id = %d"
#define MMIWRE_WINTAB_791_112_2_18_3_11_20_472 "[WRE]off sensorautortation:g_old_sensor_open_flag=%d"
#define MMIWRE_WINTAB_826_112_2_18_3_11_20_473 "[WRE]s_wre_http_proxy: is_use_proxy = %d, http_port = %d, proxy_host_ptr = %s"
#define MMIWRE_WINTAB_839_112_2_18_3_11_20_474 "[WRE]off sensorautortation:g_old_sensor_open_flag=%d"
#define MMIWRE_WINTAB_1089_112_2_18_3_11_21_475 "[WRE]close download window by timer closed in register"
#define MMIWRE_WINTAB_1130_112_2_18_3_11_21_476 "MOCOR  MSG_KEYDOWN_CANCEL 5"
#define MMIWRE_WINTAB_1132_112_2_18_3_11_21_477 "MOCOR MSG_APP_RED 5 "
#define MMIWRE_WINTAB_1169_112_2_18_3_11_21_478 "[WRE]on sensorautortation:g_old_sensor_open_flag=%d"
#define MMIWRE_WINTAB_1172_112_2_18_3_11_21_479 "[WRE]g_wre_sensor_flag is ture"
#define MMIWRE_WINTAB_1217_112_2_18_3_11_21_480 "[WRE]on sensorautortation:g_old_sensor_open_flag=%d"
#define MMIWRE_WINTAB_1220_112_2_18_3_11_21_481 "[WRE]g_wre_sensor_flag is ture"
#define MMIWRE_WINTAB_1412_112_2_18_3_11_21_482 "[WRE download].IsDevEnoughSpace: free_spaced_high = %d, free_space_low = %d"
#define MMIWRE_WINTAB_1433_112_2_18_3_11_21_483 "[WRE]this is create download window!"
#define MMIWRE_WINTAB_1746_112_2_18_3_11_22_484 "[WRE] %s %s"
#define MMIWRE_WINTAB_1755_112_2_18_3_11_22_485 "[WRE1] %s"
#define MMIWRE_WINTAB_1758_112_2_18_3_11_22_486 "md5 is %s"
#define MMIWRE_WINTAB_1779_112_2_18_3_11_22_487 "[WRE] HTTP TEST no memory \n"
#define MMIWRE_WINTAB_1950_112_2_18_3_11_22_488 "[WRE] TIME OK"
#define MMIWRE_WINTAB_1964_112_2_18_3_11_22_489 "[WRE] TIME OUT"
#define MMIWRE_WINTAB_2049_112_2_18_3_11_22_490 "[WRE]init_param_ptr->proxy: is_use_proxy = %d, http_port = %d, proxy_host_ptr = %s"
#define MMIWRE_WINTAB_2087_112_2_18_3_11_23_491 "[WRE] http test request id = %d\n"
#define MMIWRE_WINTAB_2092_112_2_18_3_11_23_492 "[WRE] http test recv HTTP_SIG_GET_CNF\n"
#define MMIWRE_WINTAB_2108_112_2_18_3_11_23_493 "[WRE] http test recv HTTP_SIG_HEAD_CNF\n"
#define MMIWRE_WINTAB_2114_112_2_18_3_11_23_494 "[WRE] http test data ind rsp code=%d,request_id =%d\n"
#define MMIWRE_WINTAB_2118_112_2_18_3_11_23_495 "[WRE] http test recv HTTP_SIG_HEADER_IND\n"
#define MMIWRE_WINTAB_2134_112_2_18_3_11_23_496 "md5 str is %s"
#define MMIWRE_WINTAB_2158_112_2_18_3_11_23_497 "no find file, or parameter error.."
#define MMIWRE_WINTAB_2162_112_2_18_3_11_23_498 "error code, context_id = %d"
#define MMIWRE_WINTAB_2187_112_2_18_3_11_23_499 "[WRE] http test recv HTTP_SIG_ERROR_IND, ERROR=%d\n"
#define MMIWRE_WINTAB_2215_112_2_18_3_11_23_500 "[WRE] http test recv HTTP_SIG_CLOSE_CNF\n"
#define MMIWRE_WINTAB_2281_112_2_18_3_11_23_501 "[WRE] WRE download total(%d) does match num %d"
#define MMIWRE_WINTAB_2298_112_2_18_3_11_23_502 "[WRE] %d %d %d %d"
#define MMIWRE_WINTAB_2330_112_2_18_3_11_23_503 "[WRE] http test net prog ind,%d,%d,%d\n"
#define MMIWRE_WINTAB_2333_112_2_18_3_11_23_504 "[WRE] http test redirect ind\n"
#define MMIWRE_WINTAB_2337_112_2_18_3_11_23_505 "[WRE] http test recv HTTP_SIG_POST_CNF\n"
#define MMIWRE_WINTAB_2354_112_2_18_3_11_23_506 "[WRE] BEGIN RELOAD!!!"
#define MMIWRE_WINTAB_2382_112_2_18_3_11_23_507 "[WRE] Download end bSucc = %d"
#define MMIWRE_WINTAB_2437_112_2_18_3_11_23_508 "[WRE] WRE_InstallKernel return FALSE"
#define MMIWRE_WINTAB_2452_112_2_18_3_11_23_509 "[WRE] WRE_InstallKernel return TRUE"
#define MMIWRE_WINTAB_2532_112_2_18_3_11_23_510 "[WRE] DeleteApp  begin:"
#define MMIWRE_WINTAB_2535_112_2_18_3_11_23_511 "[WRE] DeleteApp : FindAppGuid Fail!"
#define MMIWRE_WINTAB_2540_112_2_18_3_11_23_512 "[WRE]DeleteApp : WriteGUIDToDeleteIni Fail!"
#define MMIWRE_WINTAB_2544_112_2_18_3_11_23_513 "[WRE]DeleteApp : return TRUE!"
#define MMIWRE_WINTAB_2561_112_2_18_3_11_24_514 "[WRE] WRE_UninstallApp: appid = %d, appname = %s, return %d"
#define MMIWRE_WINTAB_2588_112_2_18_3_11_24_515 "[WRE]WRE_InstalleApp:  alloc dynamic_item_ptr return PNULL"
#define MMIWRE_WINTAB_2627_112_2_18_3_11_24_516 "[WRE] WRE_InstalleApp: appid = %d, appname = %s, return %d"
#define MMIWRE_WINTAB_2677_112_2_18_3_11_24_517 "WRE_NetworkSelectGUI: pWreNetWorkSelFunc is NULL"
#define MMIWRE_WINTAB_2705_112_2_18_3_11_24_518 "WRE_NetworkSelectGUI: not show reg ui"
#define MMIWRE_WINTAB_2756_112_2_18_3_11_24_519 "WRE_NetworkSelectGUICallBack: link_index = %d, simNo = %d, is_net_type = %d, is_wifi_used = %d,cmd = %d"
#define MMIWRE_WINTAB_2776_112_2_18_3_11_24_520 "[WRE]part3 downloading"
#define MMIWRE_WINTAB_2796_112_2_18_3_11_24_521 "[WRE]close download window"
#define MMIWRE_WINTAB_2807_112_2_18_3_11_24_522 "[WRE]create close download window timer"
#define MMIWRE_WINTAB_2821_112_2_18_3_11_24_523 "[WRE]g_download_window_id = 0"
#define MMIWRE_WINTAB_2894_112_2_18_3_11_24_524 "enter alert updata handle msg"
#define MMIWRE_WINTAB_3032_112_2_18_3_11_24_525 "enter YY message hook"
#define MMIWRE_WINTAB_3040_112_2_18_3_11_24_526 "WRE_Get_Window_YY_Message: wre_start_updatas is NULL"
#define MMIWRE_WINTAB_3045_112_2_18_3_11_25_527 "WRE_Get_Window_YY_Message: yy_exit_app is NULL"
#define MMIWRE_WINTAB_3050_112_2_18_3_11_25_528 "WRE_Get_Window_YY_Message: filename is NULL"
#define MMIWRE_WINTAB_3054_112_2_18_3_11_25_529 "WRE_Get_Window_YY_Message: strcode is 0"
#define MMIWRE_WINTAB_3073_112_2_18_3_11_25_530 "[WRE]register device"
#define MMIWRE_WINTAB_3088_112_2_18_3_11_25_531 "[WRE]check version"
#define MMIWRE_WINTAB_3101_112_2_18_3_11_25_532 "WRE_NetworkSelectGUI: SelectNetworkType return FALSE"
#define MMIWRE_WINTAB_3142_112_2_18_3_11_25_533 "[wre]:g_is_lose_focus = TURE"
#define MMIWRE_WINTAB_3153_112_2_18_3_11_25_534 "[wre]:g_is_lose_focus = TURE"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_WRE_BOOT_TRC)
TRACE_MSG(MMIWRE_ADAPTOR_294_112_2_18_3_10_30_0,"WRE_SDCardInsert return %d")
TRACE_MSG(MMIWRE_ADAPTOR_367_112_2_18_3_10_30_1,"WARNING!!! update fail!!!")
TRACE_MSG(MMIWRE_ADAPTOR_446_112_2_18_3_10_30_2,"enter WRE_InstallKernel!")
TRACE_MSG(MMIWRE_ADAPTOR_450_112_2_18_3_10_30_3,"unzip failure!")
TRACE_MSG(MMIWRE_ADAPTOR_454_112_2_18_3_10_30_4,"WRE_InstallKernel unzip ok!")
TRACE_MSG(MMIWRE_ADAPTOR_460_112_2_18_3_10_30_5,"unpack failure!")
TRACE_MSG(MMIWRE_ADAPTOR_464_112_2_18_3_10_30_6,"WRE_InstallKernel unpack ok!")
TRACE_MSG(MMIWRE_ADAPTOR_564_112_2_18_3_10_30_7,"WREEntry")
TRACE_MSG(MMIWRE_ADAPTOR_669_112_2_18_3_10_30_8,"WREEntry1")
TRACE_MSG(MMIWRE_ADAPTOR_673_112_2_18_3_10_30_9,"[WRE]wre is running!")
TRACE_MSG(MMIWRE_ADAPTOR_678_112_2_18_3_10_30_10,"[WRE]RestoreMiniWin ")
TRACE_MSG(MMIWRE_ADAPTOR_703_112_2_18_3_10_31_11,"WREEntry DownloadAndInstallWRE")
TRACE_MSG(MMIWRE_ADAPTOR_755_112_2_18_3_10_31_12,"ASYNCSOCK_HandlePsMsg, signal_ptr->msg_id = %lx")
TRACE_MSG(MMIWRE_ADAPTOR_983_112_2_18_3_10_31_13,"boot_reboot CALLED1")
TRACE_MSG(MMIWRE_ADAPTOR_989_112_2_18_3_10_31_14,"boot_reboot END")
TRACE_MSG(MMIWRE_ADAPTOR_1250_112_2_18_3_10_32_15,"enter wre_StoreLastPhotePath  ")
TRACE_MSG(MMIWRE_ADAPTOR_1269_112_2_18_3_10_32_16,"enter TracePhotoPath  ")
TRACE_MSG(MMIWRE_ADAPTOR_1273_112_2_18_3_10_32_17,"leave TracePhotoPath %s ")
TRACE_MSG(MMIWRE_ADAPTOR_1378_112_2_18_3_10_32_18,"Adapt_EntrySNSView")
TRACE_MSG(MMIWRE_ADAPTOR_1467_112_2_18_3_10_32_19,"[WRE]Adapt_EntryPhotoView: SetAnimParam Fail! anim_result = %d")
TRACE_MSG(MMIWRE_ADAPTOR_1521_112_2_18_3_10_32_20,"[WRE]Adapt_EntryPhotoView: lpFullFileName = %s")
TRACE_MSG(MMIWRE_ADAPTOR_1556_112_2_18_3_10_32_21,"[wre]WRE_IsFixAppInstalled return %d")
TRACE_MSG(MMIWRE_ADAPTOR_1612_112_2_18_3_10_32_22,"[WRE]WRE_CreateVirtualROMDisk: Create ROM Disk Fail!")
TRACE_MSG(MMIWRE_ADAPTOR_1618_112_2_18_3_10_32_23,"[WRE]RegisterDevice :success")
TRACE_MSG(MMIWRE_ADAPTOR_1622_112_2_18_3_10_32_24,"[WRE]RegisterDevice:fail, the format device")
TRACE_MSG(MMIWRE_ADAPTOR_1648_112_2_18_3_10_32_25,"[WRE]WRE_CreateVirtualRAMDisk:")
TRACE_MSG(MMIWRE_ADAPTOR_1652_112_2_18_3_10_32_26,"[WRE]WRE_CreateVirtualRAMDisk: warning! lpHandDisk is already create")
TRACE_MSG(MMIWRE_ADAPTOR_1663_112_2_18_3_10_32_27,"[WRE]WRE_CreateVirtualRAMDisk: Alloc memory Fail!")
TRACE_MSG(MMIWRE_ADAPTOR_1672_112_2_18_3_10_32_28,"[WRE]WRE_CreateVirtualRAMDisk: DISK_Create Fail!")
TRACE_MSG(MMIWRE_ADAPTOR_1682_112_2_18_3_10_32_29,"[WRE]RegisterDevice : RAM disk success")
TRACE_MSG(MMIWRE_ADAPTOR_1686_112_2_18_3_10_32_30,"[WRE]RegisterDevice: RAM disk fail, the format device")
TRACE_MSG(MMIWRE_ADAPTOR_1701_112_2_18_3_10_33_31,"[WRE]WRE_DestoryVirtualDisk: DISK_Delete!")
TRACE_MSG(MMIWRE_ADAPTOR_1708_112_2_18_3_10_33_32,"[WRE]WRE_DestoryVirtualDisk: Delete ROM disk Fail!")
TRACE_MSG(MMIWRE_ADAPTOR_1720_112_2_18_3_10_33_33,"[WRE]WRE_DestoryVirtualDisk: Delete RAM disk Fail!")
TRACE_MSG(MMIWRE_ADAPTOR_1739_112_2_18_3_10_33_34,"[WRE]WRE_TestVirtualDisk: Alloc memory fail!")
TRACE_MSG(MMIWRE_ADAPTOR_1750_112_2_18_3_10_33_35,"[WRE]WRE_TestVirtualDisk: CreateFile Fail!")
TRACE_MSG(MMIWRE_ADAPTOR_1758_112_2_18_3_10_33_36,"[WRE]WRE_FILECOPY Fail: WriteFile Error(%lx)")
TRACE_MSG(MMIWRE_ADAPTOR_1778_112_2_18_3_10_33_37,"[WRE]WRE_TestVirtualDisk: Alloc memory fail!")
TRACE_MSG(MMIWRE_ADAPTOR_1787_112_2_18_3_10_33_38,"[WRE]WRE_TestVirtualDisk: CreateFile Fail!")
TRACE_MSG(MMIWRE_ADAPTOR_1795_112_2_18_3_10_33_39,"[WRE]WRE_FILECOPY Fail: WriteFile Error(%lx)")
TRACE_MSG(MMIWRE_ADAPTOR_1814_112_2_18_3_10_33_40,"[WRE]WRE_TestVirtualDisk: WriteFile Fail!")
TRACE_MSG(MMIWRE_ADAPTOR_1820_112_2_18_3_10_33_41,"[WRE]WRE_TestVirtualDisk: ReadFile Fail!")
TRACE_MSG(MMIWRE_CFG_331_112_2_18_3_10_34_42,"WRE_Set_SYS_Dir: sysPathType = %d")
TRACE_MSG(MMIWRE_CFG_372_112_2_18_3_10_34_43,"WRE_Get_SYS_Dir_Update: sysPathType = %d")
TRACE_MSG(MMIWRE_CFG_384_112_2_18_3_10_34_44,"WRE_Get_SYS_Dir_Update Fail: not support!")
TRACE_MSG(MMIWRE_CFG_413_112_2_18_3_10_34_45,"WRE_Get_SYS_Dir_Update Fail:")
TRACE_MSG(MMIWRE_CFG_433_112_2_18_3_10_34_46,"WRE_Get_Preload_Dir: sysPathType = %d")
TRACE_MSG(MMIWRE_CFG_480_112_2_18_3_10_34_47,"WRE_Get_Preload_Dir: PreloadType = %d")
TRACE_MSG(MMIWRE_CFG_746_112_2_18_3_10_35_48,"[WRE]WRE_GetPhoneInfo: virtual disk: len =%d")
TRACE_MSG(MMIWRE_CHARSET_39_112_2_18_3_10_35_49,"cp_wcstombs : malloc fail\n")
TRACE_MSG(MMIWRE_CHARSET_70_112_2_18_3_10_36_50,"cp_mbstowcs : malloc fail\n")
TRACE_MSG(MMIWRE_EXPORT_90_112_2_18_3_10_37_51,"[WRE] HandleWREOpenWaitingWin: msg = 0x%x")
TRACE_MSG(MMIWRE_EXPORT_97_112_2_18_3_10_37_52,"[WRE]   WREEntry return 0x%x")
TRACE_MSG(MMIWRE_EXPORT_115_112_2_18_3_10_37_53,"[WRE]   WREEntry return 0x%x")
TRACE_MSG(MMIWRE_EXPORT_184_112_2_18_3_10_37_54,"[WRE]   WREEntry return 0x%x")
TRACE_MSG(MMIWRE_EXPORT_238_112_2_18_3_10_37_55,"[WRE]   WREEntry return 0x%x")
TRACE_MSG(MMIWRE_EXPORT_336_112_2_18_3_10_38_56,"[WRE]:MMIWRE_GetAppGuid appid = %d")
TRACE_MSG(MMIWRE_EXPORT_349_112_2_18_3_10_38_57,"[WRE]MMIWRE_GetFixAppPath")
TRACE_MSG(MMIWRE_EXPORT_381_112_2_18_3_10_38_58,"[WRE]:MMIWRE_StartAppEx, appid = %d, guid = %s, data = %s")
TRACE_MSG(MMIWRE_IMAGE_78_112_2_18_3_10_40_59,"IMG_DEC_GetInfo fail!")
TRACE_MSG(MMIWRE_IMAGE_86_112_2_18_3_10_40_60,"dec_info.img_type %d != %d!")
TRACE_MSG(MMIWRE_IMAGE_103_112_2_18_3_10_40_61,"dec_info.img_type %d != %d!")
TRACE_MSG(MMIWRE_IMAGE_114_112_2_18_3_10_40_62,"dec_info.img_type %d != %d!")
TRACE_MSG(MMIWRE_IMAGE_237_112_2_18_3_10_41_63,"ScratchImage: NULL == lpszData || PNULL == lpszName")
TRACE_MSG(MMIWRE_IMAGE_240_112_2_18_3_10_41_64,">>>_ScratchImage0=%08x, ptr=%08x, cyDesired=%d, nPriority=%d")
TRACE_MSG(MMIWRE_IMAGE_292_112_2_18_3_10_41_65,">>>_ScratchImage1=%d,%d")
TRACE_MSG(MMIWRE_IMAGE_357_112_2_18_3_10_41_66,"IMG_DEC_GetInfo fail!")
TRACE_MSG(MMIWRE_MEDIA_229_112_2_18_3_10_45_67,">>>audio_stream=%d")
TRACE_MSG(MMIWRE_MEDIA_240_112_2_18_3_10_45_68,"wre_player_notify, handle = %d")
TRACE_MSG(MMIWRE_MEDIA_310_112_2_18_3_10_45_69,"MediaRequest, play, media type error ")
TRACE_MSG(MMIWRE_MEDIA_329_112_2_18_3_10_45_70,"MediaRequest, record, error = %d")
TRACE_MSG(MMIWRE_MEDIA_335_112_2_18_3_10_45_71,"MediaRequest, record, create file error")
TRACE_MSG(MMIWRE_MEDIA_382_112_2_18_3_10_45_72,"s_mmPlay, eState = %d, handle = %d")
TRACE_MSG(MMIWRE_MEDIA_409_112_2_18_3_10_45_73,"s_mmPlay, MMISRVAUD_Play, fail. ret = %d")
TRACE_MSG(MMIWRE_MEDIA_421_112_2_18_3_10_45_74,"s_mmPlay, AUDIO_Play, fail. ret = %d ")
TRACE_MSG(MMIWRE_MEDIA_461_112_2_18_3_10_45_75,"OpenMedia lpmmd is NULL ")
TRACE_MSG(MMIWRE_MEDIA_465_112_2_18_3_10_45_76,"OpenMedia, isPlay = %d, eType = %d")
TRACE_MSG(MMIWRE_MEDIA_469_112_2_18_3_10_45_77,"OpenMedia hmmd is NULL ")
TRACE_MSG(MMIWRE_MEDIA_481_112_2_18_3_10_45_78,"OpenMedia play Format error, lpFormat = %d ")
TRACE_MSG(MMIWRE_MEDIA_491_112_2_18_3_10_45_79,"OpenMedia unReserved0 != 0, lpFormat = %d ")
TRACE_MSG(MMIWRE_MEDIA_506_112_2_18_3_10_45_80,"OpenMedia record, DataLen = %d, Formate = %d, eType = %d")
TRACE_MSG(MMIWRE_MEDIA_513_112_2_18_3_10_45_81,"OpenMedia record pDataOrFn malloc error ")
TRACE_MSG(MMIWRE_MEDIA_535_112_2_18_3_10_45_82,"OpenMedia play file dwDataLen = 0")
TRACE_MSG(MMIWRE_MEDIA_544_112_2_18_3_10_45_83,"OpenMedia play file is not exist ")
TRACE_MSG(MMIWRE_MEDIA_552_112_2_18_3_10_45_84,"OpenMedia play file pDataOrFn malloc error")
TRACE_MSG(MMIWRE_MEDIA_564_112_2_18_3_10_45_85,"OpenMedia play buffer, lpData = NULL or dwDataLen = %d")
TRACE_MSG(MMIWRE_MEDIA_599_112_2_18_3_10_45_86,"OpenMedia play stream format error ")
TRACE_MSG(MMIWRE_MEDIA_609_112_2_18_3_10_45_87,"OpenMedia play error, eType = %d")
TRACE_MSG(MMIWRE_MEDIA_657_112_2_18_3_10_45_88,"OpenMedia request handle fail ")
TRACE_MSG(MMIWRE_MEDIA_683_112_2_18_3_10_45_89,"CloseMedia hmmd is NULL ")
TRACE_MSG(MMIWRE_MEDIA_686_112_2_18_3_10_45_90,"CloseMedia, hmmd = 0x%x, hmmd->handle = %d")
TRACE_MSG(MMIWRE_MEDIA_744_112_2_18_3_10_46_91,"PlayMedia error, hmmd = NULL")
TRACE_MSG(MMIWRE_MEDIA_748_112_2_18_3_10_46_92,">>>PlayMedia=%d,%d,%d")
TRACE_MSG(MMIWRE_MEDIA_802_112_2_18_3_10_46_93,"PauseMedia error, hmmd is NULL ")
TRACE_MSG(MMIWRE_MEDIA_805_112_2_18_3_10_46_94,"PauseMedia, eState = %d, handle = %d, s_hasExtPaused = %d")
TRACE_MSG(MMIWRE_MEDIA_820_112_2_18_3_10_46_95,"PauseMedia, eType == TONE")
TRACE_MSG(MMIWRE_MEDIA_851_112_2_18_3_10_46_96,"PauseMedia, Pause fail, unReserved0 = %d")
TRACE_MSG(MMIWRE_MEDIA_880_112_2_18_3_10_46_97,"ResumeMedia error, hmmd is NULL ")
TRACE_MSG(MMIWRE_MEDIA_883_112_2_18_3_10_46_98,"ResumeMedia, eState = %d, handle = %d, s_hasExtPaused = %d")
TRACE_MSG(MMIWRE_MEDIA_888_112_2_18_3_10_46_99,"ResumeMedia, eType == TONE")
TRACE_MSG(MMIWRE_MEDIA_931_112_2_18_3_10_46_100,"ResumeMedia, Resume fail, unReserved0 = %d ")
TRACE_MSG(MMIWRE_MEDIA_961_112_2_18_3_10_46_101,"CloseMedia error, hmmd is NULL")
TRACE_MSG(MMIWRE_MEDIA_964_112_2_18_3_10_46_102,"CloseMedia, hmmd = 0x%x, handle = %d, eState = %d")
TRACE_MSG(MMIWRE_MEDIA_975_112_2_18_3_10_46_103,"StopMedia, stop tone")
TRACE_MSG(MMIWRE_MEDIA_1086_112_2_18_3_10_46_104,"SetMediaVolume error, hmmd is NULL ")
TRACE_MSG(MMIWRE_MEDIA_1089_112_2_18_3_10_46_105,"SetMediaVolume, eState = %d, dwValue = %d")
TRACE_MSG(MMIWRE_MEDIA_1101_112_2_18_3_10_46_106,"SetMediaVolume, Set volume fail, handle = %d")
TRACE_MSG(MMIWRE_MEDIA_1137_112_2_18_3_10_46_107,"GetMediaVolume, invalid param")
TRACE_MSG(MMIWRE_MEDIA_1142_112_2_18_3_10_46_108,"GetMediaVolume, curVol = %d")
TRACE_MSG(MMIWRE_MEDIA_1333_112_2_18_3_10_47_109,"RecordMedia error, hmmd is NULL")
TRACE_MSG(MMIWRE_MEDIA_1336_112_2_18_3_10_47_110,"RecordMedia, eType = %d, eState = %d")
TRACE_MSG(MMIWRE_MEDIA_1364_112_2_18_3_10_47_111,"RecordMedia, Record fail, handle = %d ")
TRACE_MSG(MMIWRE_MEDIA_1475_112_2_18_3_10_47_112,">>>PauseWREMM=%d,%d,%d")
TRACE_MSG(MMIWRE_MEDIA_1525_112_2_18_3_10_47_113,"MMIWRE_ResumeMM, hmmd is NULL ")
TRACE_MSG(MMIWRE_MEDIA_1528_112_2_18_3_10_47_114,">>>ResumeWREMM=%d,%d,%d")
TRACE_MSG(MMIWRE_MEDIA_1583_112_2_18_3_10_47_115,"MMIWRE_StopMM, hmmd is NULL ")
TRACE_MSG(MMIWRE_MEDIA_1587_112_2_18_3_10_47_116,"MMIWRE_StopMM=%d, %d, %d, %d")
TRACE_MSG(MMIWRE_MEDIA_1856_112_2_18_3_10_48_117,"[WRE] AudioPlayCallback, hAudio=0x%x, notify_info=%d,affix_info=%d")
TRACE_MSG(MMIWRE_MEDIA_1916_112_2_18_3_10_48_118,"[WRE] WreGetRingCodec, ring_type=%d, codec=0x%x")
TRACE_MSG(MMIWRE_MEDIA_1963_112_2_18_3_10_48_119,"[WRE]GetAudioDev, is_a2dp=%d, ring_fmt=%d")
TRACE_MSG(MMIWRE_MEDIA_2052_112_2_18_3_10_48_120,"[WRE]:audiohandle=0x%x,hAudioCodec=0x%x,real_audio_codec=0x%x,hDevice=0x%x,result=%d")
TRACE_MSG(MMIWRE_OTHER_679_112_2_18_3_10_50_121,"WreMwinWin_HandleMsg: msg_id = 0x%08x ")
TRACE_MSG(MMIWRE_OTHER_790_112_2_18_3_10_50_122,"WRE  MSG_KEYDOWN_CANCEL 31")
TRACE_MSG(MMIWRE_OTHER_1876_112_2_18_3_10_53_123,"read main file header hFile=%lx err")
TRACE_MSG(MMIWRE_OTHER_1884_112_2_18_3_10_53_124,"no enough memory room")
TRACE_MSG(MMIWRE_OTHER_1905_112_2_18_3_10_53_125,"set file pointer error hFile=%lx err")
TRACE_MSG(MMIWRE_OTHER_1914_112_2_18_3_10_53_126,"read main file branch info hFile=%lx err")
TRACE_MSG(MMIWRE_OTHER_1926_112_2_18_3_10_53_127,"[Szip]:file_name_len = %d, file_size = %d, zip_size = %d, reserved = %d")
TRACE_MSG(MMIWRE_OTHER_1937_112_2_18_3_10_53_128,"set file pointer error hFile=%lx err")
TRACE_MSG(MMIWRE_OTHER_1944_112_2_18_3_10_53_129,"read main file branch info hFile=%lx err")
TRACE_MSG(MMIWRE_OTHER_1989_112_2_18_3_10_53_130,"set file pointer error hFile=%lx err")
TRACE_MSG(MMIWRE_OTHER_2021_112_2_18_3_10_53_131,"create branch file hFile=%lx err")
TRACE_MSG(MMIWRE_OTHER_2155_112_2_18_3_10_53_132,"unzip CreateFile srcfile failure")
TRACE_MSG(MMIWRE_OTHER_2161_112_2_18_3_10_53_133,"unzip CreateFile dstfile failure")
TRACE_MSG(MMIWRE_OTHER_2171_112_2_18_3_10_53_134,"unzip UnZipcompressfile failure")
TRACE_MSG(MMIWRE_OTHER_2235_112_2_18_3_10_53_135,"Invalid install id")
TRACE_MSG(MMIWRE_OTHER_2312_112_2_18_3_10_53_136,"idx = %d,appid = %d")
TRACE_MSG(MMIWRE_OTHER_2366_112_2_18_3_10_54_137,"DeleteApp: Open Ini File return = %x")
TRACE_MSG(MMIWRE_OTHER_2371_112_2_18_3_10_54_138,"DeleteApp: Create Ini File return = %x")
TRACE_MSG(MMIWRE_OTHER_2379_112_2_18_3_10_54_139,"DeleteApp: GetFileSize handle= %x, fileSize = %d")
TRACE_MSG(MMIWRE_OTHER_2387_112_2_18_3_10_54_140,"DeleteApp: fileSize = 0, malloc WRE_UNINSTALLAPPINI Fail!")
TRACE_MSG(MMIWRE_OTHER_2397_112_2_18_3_10_54_141,"DeleteApp: fileSize = 0, malloc WRE_APPGUID Fail!")
TRACE_MSG(MMIWRE_OTHER_2415_112_2_18_3_10_54_142,"DeleteApp: fileSize != 0, malloc WRE_UNINSTALLAPPINI Fail!")
TRACE_MSG(MMIWRE_OTHER_2424_112_2_18_3_10_54_143,"DeleteApp: fileSize != 0, malloc WreAppTmp Fail!")
TRACE_MSG(MMIWRE_OTHER_2433_112_2_18_3_10_54_144,"DeleteApp: ReadFile handle= %x, iByteRead = %d")
TRACE_MSG(MMIWRE_OTHER_2453_112_2_18_3_10_54_145,"DeleteApp: WriteFile num return = %x, iByteSize = %d, iByteWrite = %d")
TRACE_MSG(MMIWRE_OTHER_2460_112_2_18_3_10_54_146,"DeleteApp: WriteFile data return = %x, iByteSize = %d, iByteWrite = %d")
TRACE_MSG(MMIWRE_OTHER_2749_112_2_18_3_10_54_147,"idx = %d,appid = %d")
TRACE_MSG(MMIWRE_OTHER_2789_112_2_18_3_10_54_148,"MYW:filepath = %s, SFS_FindFirstFile return %d")
TRACE_MSG(MMIWRE_OTHER_3108_112_2_18_3_10_55_149,"WRE_InstallApp: return %d")
TRACE_MSG(MMIWRE_OTHER_3117_112_2_18_3_10_55_150,"WRE_InstallApp: user ICON Default return %d")
TRACE_MSG(MMIWRE_OTHER_3147_112_2_18_3_10_55_151,"[WRE]: UninstallAPP--Find appName fail!")
TRACE_MSG(MMIWRE_OTHER_3158_112_2_18_3_10_55_152,"WRE_UninstallApp %d")
TRACE_MSG(MMIWRE_OTHER_3162_112_2_18_3_10_55_153,"RefreshInstallInfo %d")
TRACE_MSG(MMIWRE_OTHER_3270_112_2_18_3_10_55_154,"[WRE]   WREEntry return 0x%x")
TRACE_MSG(MMIWRE_OTHER_3332_112_2_18_3_10_56_155,"blue tooth event, event no %lx")
TRACE_MSG(MMIWRE_OTHER_3424_112_2_18_3_10_56_156,"Set WreSysDir[%d] dir to Hide return %d,  dwAttri = %d")
TRACE_MSG(MMIWRE_OTHER_3530_112_2_18_3_10_56_157,"WRE_GetMultiKeyMsg, numkeys %d")
TRACE_MSG(MMIWRE_OTHER_3534_112_2_18_3_10_56_158,"WRE_GetMultiKeyMsg, keycode %lx")
TRACE_MSG(MMIWRE_OTHER_3578_112_2_18_3_10_56_159,"MMIWRE_InitDebugMode: read debug file (uReadedByte = %d)")
TRACE_MSG(MMIWRE_OTHER_3608_112_2_18_3_10_56_160,"[wre]:MMIWRE_InitRuntimePath---")
TRACE_MSG(MMIWRE_OTHER_3617_112_2_18_3_10_56_161,"[wre]:MMIWRE_PrintNVInfo---begin")
TRACE_MSG(MMIWRE_OTHER_3619_112_2_18_3_10_56_162,"current Path")
TRACE_MSG(MMIWRE_OTHER_3624_112_2_18_3_10_56_163,"previous Path")
TRACE_MSG(MMIWRE_OTHER_3629_112_2_18_3_10_56_164,"preload Path")
TRACE_MSG(MMIWRE_OTHER_3632_112_2_18_3_10_56_165,"type = %lx")
TRACE_MSG(MMIWRE_OTHER_3636_112_2_18_3_10_56_166,"type = %lx")
TRACE_MSG(MMIWRE_OTHER_3640_112_2_18_3_10_56_167,"type = %lx")
TRACE_MSG(MMIWRE_OTHER_3643_112_2_18_3_10_56_168,"[wre]:MMIWRE_PrintNVInfo---end")
TRACE_MSG(MMIWRE_OTHER_3648_112_2_18_3_10_56_169,"[wre]:MMIWRE_ResetRuntimePath---")
TRACE_MSG(MMIWRE_OTHER_3667_112_2_18_3_10_56_170,"[wre]:MMIWRE_ReadNV---")
TRACE_MSG(MMIWRE_OTHER_3702_112_2_18_3_10_56_171,"[wre]:MMIWRE_WriteNV---")
TRACE_MSG(MMIWRE_OTHER_3734_112_2_18_3_10_56_172,"[WRE]RestoreWREEnv: kernel fail!")
TRACE_MSG(MMIWRE_OTHER_3743_112_2_18_3_10_56_173,"[WRE]RestoreWREEnv: store fail!")
TRACE_MSG(MMIWRE_OTHER_3762_112_2_18_3_10_56_174,"[WRE]RestoreFiles: sysPathType = %d")
TRACE_MSG(MMIWRE_OTHER_3769_112_2_18_3_10_56_175,"[WRE]RestoreFiles: (kernel)preload path not found!")
TRACE_MSG(MMIWRE_OTHER_3783_112_2_18_3_10_56_176,"[WRE]RestoreWREEnv: (kernel)PreloadType is WRE_PRELOAD_EXE")
TRACE_MSG(MMIWRE_OTHER_3791_112_2_18_3_10_56_177,"[WRE]RestoreFiles: (store)preload path not found!")
TRACE_MSG(MMIWRE_OTHER_3806_112_2_18_3_10_56_178,"[WRE]RestoreWREEnv: (store)PreloadType is WRE_PRELOAD_EXE")
TRACE_MSG(MMIWRE_OTHER_3811_112_2_18_3_10_56_179,"[WRE]RestoreFiles: sysPathType not support!")
TRACE_MSG(MMIWRE_OTHER_3818_112_2_18_3_10_57_180,"[WRE]RestoreFiles: path not found!")
TRACE_MSG(MMIWRE_OTHER_3829_112_2_18_3_10_57_181,"[WRE]RestoreWREEnv: Create WRE dir Fail!")
TRACE_MSG(MMIWRE_OTHER_3849_112_2_18_3_10_57_182,"[WRE]RestoreWREEnv: Copy Preload files Fail!")
TRACE_MSG(MMIWRE_OTHER_3872_112_2_18_3_10_57_183,"[WRE]RestoreWREEnv: Copy Preload Tree Fail!")
TRACE_MSG(MMIWRE_OTHER_3896_112_2_18_3_10_57_184,"[WRE] WRE_FileCopy Fail: src not found")
TRACE_MSG(MMIWRE_OTHER_3904_112_2_18_3_10_57_185,"[WRE] WRE_FileCopy Fail: Create dst file fail")
TRACE_MSG(MMIWRE_OTHER_3913_112_2_18_3_10_57_186,"[WRE] WRE_FileCopy Fail: file Size == 0")
TRACE_MSG(MMIWRE_OTHER_3924_112_2_18_3_10_57_187,"[WRE] WRE_FileCopy Fail: SCI_ALLOC_APP fail")
TRACE_MSG(MMIWRE_OTHER_3943_112_2_18_3_10_57_188,"[WRE]WRE_FILECOPY Fail: ReadFile Error(%lx)")
TRACE_MSG(MMIWRE_OTHER_3950_112_2_18_3_10_57_189,"[WRE]WRE_FILECOPY Fail: WriteFile Error(%lx)")
TRACE_MSG(MMIWRE_OTHER_4093_112_2_18_3_10_57_190,"enter WRE_RotateLCD")
TRACE_MSG(MMIWRE_SOCKET_456_112_2_18_3_10_59_191,"ASYNCSOCK_HandlePsMsg, signal_ptr->msg_id = %lx")
TRACE_MSG(MMIWRE_SOCKET_699_112_2_18_3_10_59_192,"MWIN TASK ENTRY SOCK, EVENT:%d")
TRACE_MSG(MMIWRE_SOCKET_709_112_2_18_3_10_59_193,"DEFAULT:MWIN TASK ENTRY APP, EVENT:%d")
TRACE_MSG(MMIWRE_STARTUP_109_112_2_18_3_11_0_194,"[WRE]off sensorautortation:g_old_sensor_open_flag=%d")
TRACE_MSG(MMIWRE_STARTUP_138_112_2_18_3_11_0_195,"[WRE]on sensorautortation:g_old_sensor_open_flag=%d")
TRACE_MSG(MMIWRE_STARTUP_196_112_2_18_3_11_0_196,"WREMM MMIWRE_MaxmizeAPP 1")
TRACE_MSG(MMIWRE_STARTUP_199_112_2_18_3_11_0_197,"WREMM MMIWRE_MaxmizeAPP 2")
TRACE_MSG(MMIWRE_STARTUP_201_112_2_18_3_11_0_198,"WREMM MMIWRE_MaxmizeAPP 3")
TRACE_MSG(MMIWRE_STARTUP_203_112_2_18_3_11_0_199,"WREMM MMIWRE_MaxmizeAPP 4")
TRACE_MSG(MMIWRE_STARTUP_209_112_2_18_3_11_0_200,"WREMM MMIWRE_CloseAPP 1")
TRACE_MSG(MMIWRE_STARTUP_211_112_2_18_3_11_0_201,"WREMM MMIWRE_CloseAPP 2")
TRACE_MSG(MMIWRE_STARTUP_222_112_2_18_3_11_0_202,"MMIWRE_MaxmizeAPPWithName   count [%d]\n")
TRACE_MSG(MMIWRE_STARTUP_286_112_2_18_3_11_0_203,"MMIWRE_MaxmizeAPPWithName   count [%d]\n")
TRACE_MSG(MMIWRE_STARTUP_342_112_2_18_3_11_0_204,"k_app_pad2 called ")
TRACE_MSG(MMIWRE_STARTUP_343_112_2_18_3_11_0_205,"k_app_pad2 called2[%s][%s] ")
TRACE_MSG(MMIWRE_STARTUP_347_112_2_18_3_11_0_206,"k_app_pad2:%s")
TRACE_MSG(MMIWRE_STARTUP_348_112_2_18_3_11_0_207,"snaddr:%x")
TRACE_MSG(MMIWRE_STARTUP_349_112_2_18_3_11_0_208,"snaddr:%x")
TRACE_MSG(MMIWRE_STARTUP_356_112_2_18_3_11_0_209,"fv:%x")
TRACE_MSG(MMIWRE_STARTUP_359_112_2_18_3_11_0_210,"end testvsn:%s")
TRACE_MSG(MMIWRE_STARTUP_386_112_2_18_3_11_1_211,"LoadWreLib")
TRACE_MSG(MMIWRE_STARTUP_387_112_2_18_3_11_1_212,"LoadWreLib")
TRACE_MSG(MMIWRE_STARTUP_388_112_2_18_3_11_1_213,"LoadWreLib")
TRACE_MSG(MMIWRE_STARTUP_389_112_2_18_3_11_1_214,"LoadWreLib")
TRACE_MSG(MMIWRE_STARTUP_390_112_2_18_3_11_1_215,"LoadWreLib")
TRACE_MSG(MMIWRE_STARTUP_391_112_2_18_3_11_1_216,"LoadWreLib")
TRACE_MSG(MMIWRE_STARTUP_401_112_2_18_3_11_1_217,"wk wrekernel.wrd not exist!!!!!\n")
TRACE_MSG(MMIWRE_STARTUP_415_112_2_18_3_11_1_218,"wk filesize:%d!!!\n")
TRACE_MSG(MMIWRE_STARTUP_417_112_2_18_3_11_1_219,"wk address:%x!!!\n")
TRACE_MSG(MMIWRE_STARTUP_429_112_2_18_3_11_1_220,"wk file closed!!!\n")
TRACE_MSG(MMIWRE_STARTUP_566_112_2_18_3_11_1_221,"[WRE]: startup a application\n")
TRACE_MSG(MMIWRE_STARTUP_595_112_2_18_3_11_1_222,"error:WRE_INIT_LOADKERNEL_FALI \n")
TRACE_MSG(MMIWRE_STARTUP_614_112_2_18_3_11_1_223," wre KA_DOWNLOAD_WRE \n")
TRACE_MSG(MMIWRE_STARTUP_619_112_2_18_3_11_1_224," wre KA_NORMAL \n")
TRACE_MSG(MMIWRE_STARTUP_638_112_2_18_3_11_1_225,"====== Add wrekernel into check node: %s =======\n")
TRACE_MSG(MMIWRE_STARTUP_640_112_2_18_3_11_1_226,"====== wrekernel upload addr: 0x%08x, kernel size: %d =======\n")
TRACE_MSG(MMIWRE_STARTUP_650_112_2_18_3_11_1_227,"error:WRE_INIT_NO_ROOM \n")
TRACE_MSG(MMIWRE_STARTUP_660_112_2_18_3_11_1_228," load wk ok \n")
TRACE_MSG(MMIWRE_STARTUP_691_112_2_18_3_11_1_229,"WRE boot_reboot HandleMsg 0")
TRACE_MSG(MMIWRE_STARTUP_694_112_2_18_3_11_1_230,"WRE boot_reboot HandleMsg 1 ret=%d")
TRACE_MSG(MMIWRE_STARTUP_705_112_2_18_3_11_1_231,"[WRE]on sensorautortation:g_old_sensor_open_flag=%d")
TRACE_MSG(MMIWRE_STARTUP_809_112_2_18_3_11_1_232,"There is no dpaper file!\n")
TRACE_MSG(MMIWRE_STARTUP_813_112_2_18_3_11_1_233,"dpaper file size:%d!!!\n")
TRACE_MSG(MMIWRE_STARTUP_816_112_2_18_3_11_1_234,"dpaper address:%x!!!\n")
TRACE_MSG(MMIWRE_STARTUP_830_112_2_18_3_11_1_235,"dpaper file closed!!!\n")
TRACE_MSG(MMIWRE_TAPI_239_112_2_18_3_11_2_236,"mmiwre Adapt_APPSendSms send fail")
TRACE_MSG(MMIWRE_TAPI_279_112_2_18_3_11_2_237,"MMISMS: GetTransmitterDispString number_type = %d, number_plan = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_769_112_2_18_3_11_6_238,"[Wre Player] CreateMediaPlayer ui callback is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_777_112_2_18_3_11_6_239,"[Wre Player] CreateMediaPlayer create  fail")
TRACE_MSG(MMIWRE_VIDEOPLAYER_780_112_2_18_3_11_6_240,"[Wre Player] CreateMediaPlayer create handle = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_784_112_2_18_3_11_6_241,"[Wre Player] CreateMediaPlayer dispath msg fail")
TRACE_MSG(MMIWRE_VIDEOPLAYER_815_112_2_18_3_11_6_242,"[Wre Player] DestroyVideoPlayer hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_819_112_2_18_3_11_6_243,"[Wre Player] DestroyVideoPlayer some wrong with hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_855_112_2_18_3_11_6_244,"[Wre Player] PlayVideo some wrong with hVideo = %x,playParam = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_859_112_2_18_3_11_6_245,"[Wre Player] PlayVideo begin")
TRACE_MSG(MMIWRE_VIDEOPLAYER_862_112_2_18_3_11_6_246,"[Wre Player] PlayVideo result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_883_112_2_18_3_11_6_247,"[Wre Player] StopVideo some wrong with hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_887_112_2_18_3_11_6_248,"[Wre Player] StopVideo begin")
TRACE_MSG(MMIWRE_VIDEOPLAYER_889_112_2_18_3_11_6_249,"[Wre Player] StopVideo result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_906_112_2_18_3_11_6_250,"[Wre Player] PauseVideo some wrong with hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_909_112_2_18_3_11_6_251,"[Wre Player] PauseVideo begin")
TRACE_MSG(MMIWRE_VIDEOPLAYER_913_112_2_18_3_11_6_252,"[Wre Player] PauseVideo result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_934_112_2_18_3_11_6_253,"[Wre Player] ResumeVideo some wrong with hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_937_112_2_18_3_11_6_254,"[Wre Player] ResumeVideo begin")
TRACE_MSG(MMIWRE_VIDEOPLAYER_940_112_2_18_3_11_6_255,"[Wre Player] ResumeVideo result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_959_112_2_18_3_11_6_256,"[Wre Player] SeekVideo some wrong with hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_962_112_2_18_3_11_6_257,"[Wre Player] SeekVideo begin")
TRACE_MSG(MMIWRE_VIDEOPLAYER_966_112_2_18_3_11_6_258,"[Wre Player] SeekVideo result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_983_112_2_18_3_11_7_259,"[Wre Player] ExitVideo the fsm is wrong hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1002_112_2_18_3_11_7_260,"[Wre Player] CompulsivePauseVideo the fsm is wrong hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1006_112_2_18_3_11_7_261,"[Wre Player] CompulsivePauseVideo begin fsm_ptr->is_to_resume = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1023_112_2_18_3_11_7_262,"[Wre Player] CompulsivePauseVideo end fsm_ptr->is_to_pause = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1041_112_2_18_3_11_7_263,"[Wre Player] CompulsiveResumeVideo the fsm is wrong hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1044_112_2_18_3_11_7_264,"[Wre Player] CompulsiveResumeVideo begin fsm_ptr->is_to_pause = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1100_112_2_18_3_11_7_265,"[Wre Player] CompulsiveResumeVideo end fsm_ptr->is_to_resume = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1123_112_2_18_3_11_7_266,"[Wre Player] GetMediaPlayerState state = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1139_112_2_18_3_11_7_267,"[Wre Player] StopSetRate some wrong with hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1142_112_2_18_3_11_7_268,"[Wre Player] StopSetRate begin")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1146_112_2_18_3_11_7_269,"[Wre Player] StopSetRate result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1161_112_2_18_3_11_7_270,"[Wre Player] StartSetRate some wrong with hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1164_112_2_18_3_11_7_271,"[Wre Player] StartSetRate begin")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1168_112_2_18_3_11_7_272,"[Wre Player] StartSetRate result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1320_112_2_18_3_11_7_273,"[Wre Player] GetVideoTimeOffset = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1349_112_2_18_3_11_7_274,"[Wre Player] GetFrameOffset info_result = %d, *dwFrameIndex = %d, time_offset = %d, media_info.video_info.frame_rate = %d, base = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1370_112_2_18_3_11_7_275,"[Wre Player] SwitchWmpScreen the fsm is wrong fsm_ptr = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1410_112_2_18_3_11_7_276,"[Wre Player] SwitchPlayDevice the fsm is wrong fsm_ptr = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1414_112_2_18_3_11_7_277,"[Wre Player] SwitchPlayDevice is_bt_support = %d, bSupportA2dp = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1451_112_2_18_3_11_8_278,"[Wre Player] AuthRsp some wrong with hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1457_112_2_18_3_11_8_279,"[Wre Player] AuthRsp result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1471_112_2_18_3_11_8_280,"[Wre Player] GetMediaInfo param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1492_112_2_18_3_11_8_281,"[Wre Player] GetMediaInfo param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1611_112_2_18_3_11_8_282,"[Wre Player]: GetMediaInfoFromFullPathName() param error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1621_112_2_18_3_11_8_283,"[Wre Player]: GetMediaInfoFromFullPathName: open dplayer fail! ")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1629_112_2_18_3_11_8_284,"[Wre Player]: GetMediaInfoFromFullPathName() return = 0x%.2x, full_name_len = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1640_112_2_18_3_11_8_285,"[Wre Player]: GetMediaInfoFromFullPathName():ntfile_type = %d,ntlen = %d,ntv_spt = %d,ntf_rate = %d,ntheight = %d,ntwidth = %d,ntvideo_type = %d,ntaudio_type = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1662_112_2_18_3_11_8_286,"[Wre Player]: GetMediaInfoFromBuf param error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1666_112_2_18_3_11_8_287,"[Wre Player]: GetMediaInfoFromBuf: video_type = %d, video_length = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1723_112_2_18_3_11_8_288,"[Wre Player] CheckSupportedTypeFromMediaInfo param is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1732_112_2_18_3_11_8_289,"[Wre Player] CheckSupportedTypeFromMediaInfo video_support = %d, audio_support = %d , video_type = %d, audio_type = %d, height = %d, width = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1765_112_2_18_3_11_8_290,"[Wre Player] GetSupportedStatus param is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1828_112_2_18_3_11_8_291,"[Wre Player] FillRectWithTransparenceColor before x= %d, y=%d,width = %d, height = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1841_112_2_18_3_11_8_292,"[Wre Player] FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1900_112_2_18_3_11_8_293,"[Wre Player] FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_1992_112_2_18_3_11_9_294,"[Wre Player] DispatchSignalToFsmMsg STRM_INIT_CNF")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2010_112_2_18_3_11_9_295,"[Wre Player] DispatchSignalToFsmMsg STRM_PLAY_CNF")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2026_112_2_18_3_11_9_296,"[Wre Player] DispatchSignalToFsmMsg STRM_BUFFERING_IND")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2034_112_2_18_3_11_9_297,"[Wre Player] DispatchSignalToFsmMsg STRM_BUFFER_END_IND")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2042_112_2_18_3_11_9_298,"[Wre Player] DispatchSignalToFsmMsg STRM_DATA_TIMEOUT_IND")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2051_112_2_18_3_11_9_299,"[Wre Player] DispatchSignalToFsmMsg strm_buf_process_ptr = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2057_112_2_18_3_11_9_300,"[Wre Player] DispatchSignalToFsmMsg process = %d, total = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2064_112_2_18_3_11_9_301,"[Wre Player] DispatchSignalToFsmMsg percent = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2069_112_2_18_3_11_9_302,"[Wre Player] DispatchSignalToFsmMsg STRM_GOODBYE_IND")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2078_112_2_18_3_11_9_303,"[Wre Player] DispatchSignalToFsmMsg STRM_SEEK_CNF")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2096_112_2_18_3_11_9_304,"[Wre Player] DispatchSignalToFsmMsg STRM_PAUSE_CNF")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2114_112_2_18_3_11_9_305,"[Wre Player] DispatchSignalToFsmMsg STRM_RESUME_CNF")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2131_112_2_18_3_11_9_306,"[Wre Player] DispatchSignalToFsmMsg STRM_STOP_CNF")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2148_112_2_18_3_11_9_307,"[Wre Player] DispatchSignalToFsmMsg STRM_CLOSE_CNF")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2163_112_2_18_3_11_9_308,"[Wre Player] DispatchSignalToFsmMsg APP_WMP_FRAME_END_IND")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2172_112_2_18_3_11_9_309,"[Wre Player] DispatchSignalToFsmMsg APP_WMP_END_IND")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2185_112_2_18_3_11_9_310,"[Wre Player] DispatchSignalToFsmMsg APP_WMP_PLAY_NEXT_FILE_REQ")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2292_112_2_18_3_11_9_311,"[Wre Player] SettingAsScreensaver full_name_ptr = %x, full_name_len = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2341_112_2_18_3_11_9_312,"[Wre Player] SettingAsPowerOnOffAnimation is_power_on = %d, full_name_ptr = %x,full_name_len = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2424_112_2_18_3_11_9_313,"[Wre Player] CreatMsg play_param_ptr->playType = %d, msg_body_size = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2431_112_2_18_3_11_9_314,"[Wre Player] CreatMsg full path ptr is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2440_112_2_18_3_11_10_315,"[Wre Player] CreatMsg buffer is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2448_112_2_18_3_11_10_316,"[Wre Player] CreatMsg url is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2451_112_2_18_3_11_10_317,"[Wre Player] CreatMsg msg_body_size = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2453_112_2_18_3_11_10_318,"[Wre Player] CreatMsg msg_body_size = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2466_112_2_18_3_11_10_319,"[Wre Player] msg_ptr = %x, len = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2478_112_2_18_3_11_10_320," play_param2_ptr = %x, msg_body_size =%d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2490_112_2_18_3_11_10_321,"[Wre Player] CreatMsg play_param2_ptr->strUrl = %x, len = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2494_112_2_18_3_11_10_322," play_param2_ptr->strFilePath = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2496_112_2_18_3_11_10_323," play_param2_ptr->strFilePath = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2497_112_2_18_3_11_10_324," play_param_ptr->strFilePath = %x, play_param_ptr->uFilePathLen = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2499_112_2_18_3_11_10_325," play_param2_ptr->strFilePath = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2581_112_2_18_3_11_10_326,"[Wre Player] CreatMsg frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2598_112_2_18_3_11_10_327,"[Wre Player] CreatMsg strm_init_info_ptr->sdp_info.video_param.config_len = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2618_112_2_18_3_11_10_328,"[Wre Player] CreatMsg sprop_param_sets_num = %d, enc_type = %d, sprop_sets_arr = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2826_112_2_18_3_11_10_329,"[Wre Player] CreateFSM there is a fsm")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2834_112_2_18_3_11_10_330,"[Wre Player] CreateFSM no space for fsm")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2872_112_2_18_3_11_10_331,"[Wre Player] CreateFSM cur fsm is %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2895_112_2_18_3_11_10_332,"[Wre Player] CreateFSM cur fsm is %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2921_112_2_18_3_11_10_333,"[Wre Player] DestroyFSM there is no fsm list")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2929_112_2_18_3_11_11_334,"[Wre Player] DestroyFSM current_fsm_ptr = %x, current_fsm_ptr->fsm_info_ptr = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_2985_112_2_18_3_11_11_335,"[Wre Player] DestroyFSM  fsm hVideo = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3034_112_2_18_3_11_11_336,"[Wre Player] FsmDispatch something wrong with fsm_ptr = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3054_112_2_18_3_11_11_337,"[Wre Player] FsmStateTrans fsm_ptr = %x, state = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3082_112_2_18_3_11_11_338,"[Wre Player] SendPrepareIndToUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3098_112_2_18_3_11_11_339,"[Wre Player] SendPrepareIndToUI send prepare ind to ui")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3114_112_2_18_3_11_11_340,"[Wre Player] SendAUTHIndToUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3123_112_2_18_3_11_11_341,"[Wre Player] SendAUTHIndToUI send auth ind to ui")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3139_112_2_18_3_11_11_342,"[Wre Player] SendFatalErrorIndTOUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3149_112_2_18_3_11_11_343,"[Wre Player] SendFatalErrorIndTOUI send fatal error to ui errorType = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3166_112_2_18_3_11_11_344,"[Wre Player] SendProcessIndToUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3192_112_2_18_3_11_11_345,"[Wre Player] SendBufferingIndToUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3201_112_2_18_3_11_11_346,"[Wre Player] SendBufferingIndToUI send VIDEO_NOTIFY_BUFFERING to ui")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3217_112_2_18_3_11_11_347,"[Wre Player] SendBufferPercentToUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3227_112_2_18_3_11_11_348,"[Wre Player]  send VIDEO_NOTIFY_BUFFER_END to ui")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3243_112_2_18_3_11_11_349,"[Wre Player] SendBufferEndIndToUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3252_112_2_18_3_11_11_350,"[Wre Player] send VIDEO_NOTIFY_BUFFER_END to ui")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3268_112_2_18_3_11_11_351,"[Wre Player] SendPlayEndToUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3278_112_2_18_3_11_11_352,"[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_PLAY_END to ui")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3294_112_2_18_3_11_11_353,"[Wre Player] SendStopIndToUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3303_112_2_18_3_11_11_354,"[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_STOP to ui")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3319_112_2_18_3_11_11_355,"[Wre Player] SendPlayCnfToUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3329_112_2_18_3_11_11_356,"[Wre Player] SendPlayCnfToUI send WMP_PLAY_CNFto ui errorType = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3345_112_2_18_3_11_11_357,"[Wre Player] SendPauseCnfToUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3355_112_2_18_3_11_11_358,"[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_PAUSE to ui")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3371_112_2_18_3_11_11_359,"[Wre Player] SendSeekCnfToUI param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3383_112_2_18_3_11_11_360,"[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_SEEK to ui")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3429_112_2_18_3_11_12_361,"[VP]FrameToTime handle = 0x%x, info_result = %d,frame_index = %d, time_offset = 0x%x, time_lenth = %d,base = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3455_112_2_18_3_11_12_362,"[Wre Player] CallbackEndNotify i_type = %d, s_current_fsm_ptr->task_id = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3503_112_2_18_3_11_12_363,"[Wre Player] PlayLocalFile fsm is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3509_112_2_18_3_11_12_364,"[Wre Player] PlayLocalFile uFilePathLen = %d, strFilePath = %x, %x, %x, %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3564_112_2_18_3_11_12_365,"[Wre Player] PlayLocalFile fsm_ptr->dplayer_handle = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3586_112_2_18_3_11_12_366,"[Wre Player] PlayLocalFile play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3596_112_2_18_3_11_12_367,"[Wre Player] PlayLocalFile total_time = %d, process_time = %d, support_type = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3626_112_2_18_3_11_12_368,"[Wre Player] PlayLocalFile  dplayer_result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3650_112_2_18_3_11_12_369,"[Wre Player] PlayLocalFile play error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3686_112_2_18_3_11_12_370,"[Wre Player] PlayLocalBuf param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3694_112_2_18_3_11_12_371,"[Wre Player] PlayLocalBuf play_param_ptr->videoBufferInfo.fileYype = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3721_112_2_18_3_11_12_372,"[Wre Player] PlayLocalBuf fsm_ptr->dplayer_handle = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3736_112_2_18_3_11_12_373,"[Wre Player] PlayLocalBuf vidot_type = %d, vidoe_support = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3741_112_2_18_3_11_12_374,"[Wre Player] PlayLocalBuf play_param_ptr->bScalUp = %d, fsm_ptr->display_param.nWidth = %d, p_info.video_info.nWidth = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3753_112_2_18_3_11_12_375,"[Wre Player] PlayLocalBuf total_time = %d, process_time = %d, support_type = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3780_112_2_18_3_11_12_376,"[Wre Player] PlayLocalBuf dplayer_result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3795_112_2_18_3_11_12_377,"[Wre Player] PlayLocalBuf play error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3826_112_2_18_3_11_12_378,"[Wre Player] PlayFromNet param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3834_112_2_18_3_11_12_379,"[Wre Player] PlayFromNet fsm_ptr->dplayer_handle = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3859_112_2_18_3_11_12_380,"[Wre Player] PlayFromNet play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3871_112_2_18_3_11_12_381,"[Wre Player] PlayFromNet total_time = %d, process_time = %d, support_type = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3900_112_2_18_3_11_13_382,"[Wre Player] PlayFromNet play error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3932_112_2_18_3_11_13_383,"[Wre Player] HandlePDPMsg param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3936_112_2_18_3_11_13_384,"[Wre Player] HandlePDPMsg msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3978_112_2_18_3_11_13_385,"[Wre Player] ActivePdp param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3983_112_2_18_3_11_13_386,"[Wre Player] ActivePdp dual_sys = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_3991_112_2_18_3_11_13_387,"[Wre Player] ActivePdp sim casd is not ok dual_sys = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4000_112_2_18_3_11_13_388,"[Wre Player] ActivePdp param is setting_index = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4023_112_2_18_3_11_13_389,"[Wre Player] ActivePdp sim casd is not ok dual_sys = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4056_112_2_18_3_11_13_390,"[Wre Player] FsmIdle param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4060_112_2_18_3_11_13_391,"[Wre Player] FsmIdle msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4068_112_2_18_3_11_13_392,"IDLE, got wrong msg")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4088_112_2_18_3_11_13_393,"[Wre Player] InitFsmInfo param is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4091_112_2_18_3_11_13_394,"[Wre Player] InitFsmInfo state = %d, vpCallBack = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4127_112_2_18_3_11_13_395,"[Wre Player] UpdateFsmInfo param is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4166_112_2_18_3_11_13_396,"[Wre Player] UpdateFsmInfo fsm_ptr->strFilePath = %x, play_param_ptr->strFilePath = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4243_112_2_18_3_11_13_397,"[Wre Player] FsmReady play param is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4247_112_2_18_3_11_13_398,"[Wre Player] FsmReady msg_ptr->msg_id = %x, fsm_ptr->has_play_end_reason = %d, fsm_ptr->is_to_exit = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4270_112_2_18_3_11_13_399,"[Wre Player] FsmReady play_param_ptr->playType = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4388_112_2_18_3_11_14_400,"[Wre Player] FsmReady ready, got wrong msg")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4409_112_2_18_3_11_14_401,"[Wre Player] InitStreaming param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4528_112_2_18_3_11_14_402,"[Wre Player] FsmPdpActiving fsm or msg error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4532_112_2_18_3_11_14_403,"[Wre Player] FsmPdpActiving msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4540_112_2_18_3_11_14_404,"[Wre Player] FsmPdpActiving pdp active error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4595_112_2_18_3_11_14_405,"[Wre Player] FsmInit param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4599_112_2_18_3_11_14_406,"[Wre Player] FsmInit msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4643_112_2_18_3_11_14_407,"[Wre Player] PlayFromNet play error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4690_112_2_18_3_11_14_408,"[Wre Player] FsmInit param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4694_112_2_18_3_11_14_409,"[Wre Player] FsmInit msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4699_112_2_18_3_11_14_410,"[Wre Player] FsmInit str_ini_result = %d, is_success_play = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4807_112_2_18_3_11_14_411,"[Wre Player] FsmInitAuth param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4811_112_2_18_3_11_14_412,"[Wre Player] FsmInitAuth msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4873_112_2_18_3_11_15_413,"[Wre Player] FsmPlayPeding param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4877_112_2_18_3_11_15_414,"[Wre Player] FsmPlayPeding msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4882_112_2_18_3_11_15_415,"[Wre Player] FsmPlayPeding strm_result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_4999_112_2_18_3_11_15_416,"[Wre Player] FsmPlayAuth param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5004_112_2_18_3_11_15_417,"[Wre Player] FsmPlayAuth msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5020_112_2_18_3_11_15_418,"[Wre Player] FsmPlayAuth auth_info.password_ptr= %x, auth_info.username_ptr = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5072_112_2_18_3_11_15_419,"[Wre Player] FsmPlaying param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5081_112_2_18_3_11_15_420,"[Wre Player] FsmPlaying FSM_ENTRY_REQ fsm_ptr->is_to_pause %x,fsm_ptr->has_play_end_reason = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5105_112_2_18_3_11_15_421,"[Wre Player] FsmPlaying WMP_STRM_BUFFERING_IND")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5111_112_2_18_3_11_15_422,"[Wre Player] FsmPlaying FSM_CALLBACK_END_IND")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5127_112_2_18_3_11_15_423,"[Wre Player] FsmPlaying fsm_ptr->playType = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5181_112_2_18_3_11_15_424,"[Wre Player] FsmPlaying FSM_PAUSE_REQ, fsm_ptr->is_not_notify_ui = %d, fsm_ptr->is_to_pause = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5214_112_2_18_3_11_15_425,"[Wre Player] FsmPlaying FSM_SEEK_REQ")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5250_112_2_18_3_11_15_426,"[Wre Player] FsmPlaying FSM_STOP_REQ")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5264_112_2_18_3_11_15_427,"[Wre Player] FsmPlaying FSM_STRM_DATA_TIMEOUT_IND")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5272_112_2_18_3_11_15_428,"[Wre Player] FsmPlaying FSM_RESUME_REQ")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5313_112_2_18_3_11_15_429,"[Wre Player] FsmBuffering param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5318_112_2_18_3_11_15_430,"[Wre Player] FsmBuffering msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5446_112_2_18_3_11_16_431,"[Wre Player] FsmSeeking param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5450_112_2_18_3_11_16_432,"[Wre Player] FsmSeeking  msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5463_112_2_18_3_11_16_433,"[Wre Player] FsmSeeking strm_result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5542_112_2_18_3_11_16_434,"[Wre Player] FsmPausing param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5546_112_2_18_3_11_16_435,"[Wre Player] FsmPausing msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5559_112_2_18_3_11_16_436,"[Wre Player] FsmPausing strm_result = %d, fsm_ptr->is_not_notify_ui = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5568_112_2_18_3_11_16_437,"[Wre Player] FsmPausing send pause cnf")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5631_112_2_18_3_11_16_438,"[Wre Player] FsmPaused param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5635_112_2_18_3_11_16_439,"[Wre Player] FsmPaused msg_ptr->msg_id = %x, is_to_pause = %d, is_to_resume = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5681_112_2_18_3_11_16_440,"[Wre Player] FsmPaused FSM_SEEK_REQ")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5782_112_2_18_3_11_16_441,"[Wre Player] FsmPausedSeeking param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5786_112_2_18_3_11_16_442,"[Wre Player] FsmPausedSeeking  msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5808_112_2_18_3_11_16_443,"[Wre Player] FsmSeeking strm_result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5885_112_2_18_3_11_17_444,"[Wre Player] FsmPaused param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5889_112_2_18_3_11_17_445,"[Wre Player] FsmPaused msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5957_112_2_18_3_11_17_446,"[Wre Player] FsmResuming msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_5969_112_2_18_3_11_17_447,"[Wre Player] FsmResuming strm_result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6041_112_2_18_3_11_17_448,"[Wre Player] HandleInitCnf param is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6044_112_2_18_3_11_17_449,"[Wre Player] HandleInitCnf strm_init_ptr->result = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6133_112_2_18_3_11_17_450,"[Wre Player] HandleInitCnf frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d, fsm_ptr->total_time= %d, strm_cfg.video_cfg.cfg_ptr = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6164_112_2_18_3_11_17_451,"[Wre Player] HandleUpdateFrame param is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6184_112_2_18_3_11_17_452,"[Wre Player] HandlePauseReq param is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6208_112_2_18_3_11_17_453,"[Wre Player] HandlePauseReq can not pause, so stop")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6229_112_2_18_3_11_17_454,"[Wre Player] HandleResumeReq param is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6261_112_2_18_3_11_17_455,"[Wre Player] HandleSeekReq param is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6266_112_2_18_3_11_17_456,"[Wre Player] FsmPlaying seek_time = %x, seek_param->nSeekFrame = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6291_112_2_18_3_11_17_457,"[Wre Player] HandleSeekReq streaming see error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6305_112_2_18_3_11_17_458,"[Wre Player] HandleStopReq param is null")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6309_112_2_18_3_11_17_459,"[Wre Player] HandleStopReq is_begin_play = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6344_112_2_18_3_11_18_460,"[Wre Player] FsmStopping param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6348_112_2_18_3_11_18_461,"[Wre Player] FsmStopping msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6379_112_2_18_3_11_18_462,"[Wre Player] FsmStopped param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6417_112_2_18_3_11_18_463,"[Wre Player] FsmClosing param is error")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6421_112_2_18_3_11_18_464,"[Wre Player] FsmClosing msg_ptr->msg_id = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6533_112_2_18_3_11_18_465,"[Wre Player] SendSignalToCurPlayingTask signalType = %d, data = %d, task_id = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6576_112_2_18_3_11_18_466,"[Wre Player] SwitchDisplayMode the fsm is wrong fsm_ptr = %x")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6580_112_2_18_3_11_18_467,"[Wre Player] SwitchDisplayMode fsm_ptr->state = %d, is_only_rect = %d")
TRACE_MSG(MMIWRE_VIDEOPLAYER_6625_112_2_18_3_11_18_468,"[Wre Player] SwitchDisplayMode result = %d")
TRACE_MSG(MMIWRE_WINTAB_188_112_2_18_3_11_19_469,"[WRE] MMIWRE_SetSimSelect sim_select = %d")
TRACE_MSG(MMIWRE_WINTAB_552_112_2_18_3_11_20_470,"[WRE] HandlePsMsg msg_id=%d, result=%d")
TRACE_MSG(MMIWRE_WINTAB_562_112_2_18_3_11_20_471,"[WRE] s_wre_net_id = %d")
TRACE_MSG(MMIWRE_WINTAB_791_112_2_18_3_11_20_472,"[WRE]off sensorautortation:g_old_sensor_open_flag=%d")
TRACE_MSG(MMIWRE_WINTAB_826_112_2_18_3_11_20_473,"[WRE]s_wre_http_proxy: is_use_proxy = %d, http_port = %d, proxy_host_ptr = %s")
TRACE_MSG(MMIWRE_WINTAB_839_112_2_18_3_11_20_474,"[WRE]off sensorautortation:g_old_sensor_open_flag=%d")
TRACE_MSG(MMIWRE_WINTAB_1089_112_2_18_3_11_21_475,"[WRE]close download window by timer closed in register")
TRACE_MSG(MMIWRE_WINTAB_1130_112_2_18_3_11_21_476,"MOCOR  MSG_KEYDOWN_CANCEL 5")
TRACE_MSG(MMIWRE_WINTAB_1132_112_2_18_3_11_21_477,"MOCOR MSG_APP_RED 5 ")
TRACE_MSG(MMIWRE_WINTAB_1169_112_2_18_3_11_21_478,"[WRE]on sensorautortation:g_old_sensor_open_flag=%d")
TRACE_MSG(MMIWRE_WINTAB_1172_112_2_18_3_11_21_479,"[WRE]g_wre_sensor_flag is ture")
TRACE_MSG(MMIWRE_WINTAB_1217_112_2_18_3_11_21_480,"[WRE]on sensorautortation:g_old_sensor_open_flag=%d")
TRACE_MSG(MMIWRE_WINTAB_1220_112_2_18_3_11_21_481,"[WRE]g_wre_sensor_flag is ture")
TRACE_MSG(MMIWRE_WINTAB_1412_112_2_18_3_11_21_482,"[WRE download].IsDevEnoughSpace: free_spaced_high = %d, free_space_low = %d")
TRACE_MSG(MMIWRE_WINTAB_1433_112_2_18_3_11_21_483,"[WRE]this is create download window!")
TRACE_MSG(MMIWRE_WINTAB_1746_112_2_18_3_11_22_484,"[WRE] %s %s")
TRACE_MSG(MMIWRE_WINTAB_1755_112_2_18_3_11_22_485,"[WRE1] %s")
TRACE_MSG(MMIWRE_WINTAB_1758_112_2_18_3_11_22_486,"md5 is %s")
TRACE_MSG(MMIWRE_WINTAB_1779_112_2_18_3_11_22_487,"[WRE] HTTP TEST no memory \n")
TRACE_MSG(MMIWRE_WINTAB_1950_112_2_18_3_11_22_488,"[WRE] TIME OK")
TRACE_MSG(MMIWRE_WINTAB_1964_112_2_18_3_11_22_489,"[WRE] TIME OUT")
TRACE_MSG(MMIWRE_WINTAB_2049_112_2_18_3_11_22_490,"[WRE]init_param_ptr->proxy: is_use_proxy = %d, http_port = %d, proxy_host_ptr = %s")
TRACE_MSG(MMIWRE_WINTAB_2087_112_2_18_3_11_23_491,"[WRE] http test request id = %d\n")
TRACE_MSG(MMIWRE_WINTAB_2092_112_2_18_3_11_23_492,"[WRE] http test recv HTTP_SIG_GET_CNF\n")
TRACE_MSG(MMIWRE_WINTAB_2108_112_2_18_3_11_23_493,"[WRE] http test recv HTTP_SIG_HEAD_CNF\n")
TRACE_MSG(MMIWRE_WINTAB_2114_112_2_18_3_11_23_494,"[WRE] http test data ind rsp code=%d,request_id =%d\n")
TRACE_MSG(MMIWRE_WINTAB_2118_112_2_18_3_11_23_495,"[WRE] http test recv HTTP_SIG_HEADER_IND\n")
TRACE_MSG(MMIWRE_WINTAB_2134_112_2_18_3_11_23_496,"md5 str is %s")
TRACE_MSG(MMIWRE_WINTAB_2158_112_2_18_3_11_23_497,"no find file, or parameter error..")
TRACE_MSG(MMIWRE_WINTAB_2162_112_2_18_3_11_23_498,"error code, context_id = %d")
TRACE_MSG(MMIWRE_WINTAB_2187_112_2_18_3_11_23_499,"[WRE] http test recv HTTP_SIG_ERROR_IND, ERROR=%d\n")
TRACE_MSG(MMIWRE_WINTAB_2215_112_2_18_3_11_23_500,"[WRE] http test recv HTTP_SIG_CLOSE_CNF\n")
TRACE_MSG(MMIWRE_WINTAB_2281_112_2_18_3_11_23_501,"[WRE] WRE download total(%d) does match num %d")
TRACE_MSG(MMIWRE_WINTAB_2298_112_2_18_3_11_23_502,"[WRE] %d %d %d %d")
TRACE_MSG(MMIWRE_WINTAB_2330_112_2_18_3_11_23_503,"[WRE] http test net prog ind,%d,%d,%d\n")
TRACE_MSG(MMIWRE_WINTAB_2333_112_2_18_3_11_23_504,"[WRE] http test redirect ind\n")
TRACE_MSG(MMIWRE_WINTAB_2337_112_2_18_3_11_23_505,"[WRE] http test recv HTTP_SIG_POST_CNF\n")
TRACE_MSG(MMIWRE_WINTAB_2354_112_2_18_3_11_23_506,"[WRE] BEGIN RELOAD!!!")
TRACE_MSG(MMIWRE_WINTAB_2382_112_2_18_3_11_23_507,"[WRE] Download end bSucc = %d")
TRACE_MSG(MMIWRE_WINTAB_2437_112_2_18_3_11_23_508,"[WRE] WRE_InstallKernel return FALSE")
TRACE_MSG(MMIWRE_WINTAB_2452_112_2_18_3_11_23_509,"[WRE] WRE_InstallKernel return TRUE")
TRACE_MSG(MMIWRE_WINTAB_2532_112_2_18_3_11_23_510,"[WRE] DeleteApp  begin:")
TRACE_MSG(MMIWRE_WINTAB_2535_112_2_18_3_11_23_511,"[WRE] DeleteApp : FindAppGuid Fail!")
TRACE_MSG(MMIWRE_WINTAB_2540_112_2_18_3_11_23_512,"[WRE]DeleteApp : WriteGUIDToDeleteIni Fail!")
TRACE_MSG(MMIWRE_WINTAB_2544_112_2_18_3_11_23_513,"[WRE]DeleteApp : return TRUE!")
TRACE_MSG(MMIWRE_WINTAB_2561_112_2_18_3_11_24_514,"[WRE] WRE_UninstallApp: appid = %d, appname = %s, return %d")
TRACE_MSG(MMIWRE_WINTAB_2588_112_2_18_3_11_24_515,"[WRE]WRE_InstalleApp:  alloc dynamic_item_ptr return PNULL")
TRACE_MSG(MMIWRE_WINTAB_2627_112_2_18_3_11_24_516,"[WRE] WRE_InstalleApp: appid = %d, appname = %s, return %d")
TRACE_MSG(MMIWRE_WINTAB_2677_112_2_18_3_11_24_517,"WRE_NetworkSelectGUI: pWreNetWorkSelFunc is NULL")
TRACE_MSG(MMIWRE_WINTAB_2705_112_2_18_3_11_24_518,"WRE_NetworkSelectGUI: not show reg ui")
TRACE_MSG(MMIWRE_WINTAB_2756_112_2_18_3_11_24_519,"WRE_NetworkSelectGUICallBack: link_index = %d, simNo = %d, is_net_type = %d, is_wifi_used = %d,cmd = %d")
TRACE_MSG(MMIWRE_WINTAB_2776_112_2_18_3_11_24_520,"[WRE]part3 downloading")
TRACE_MSG(MMIWRE_WINTAB_2796_112_2_18_3_11_24_521,"[WRE]close download window")
TRACE_MSG(MMIWRE_WINTAB_2807_112_2_18_3_11_24_522,"[WRE]create close download window timer")
TRACE_MSG(MMIWRE_WINTAB_2821_112_2_18_3_11_24_523,"[WRE]g_download_window_id = 0")
TRACE_MSG(MMIWRE_WINTAB_2894_112_2_18_3_11_24_524,"enter alert updata handle msg")
TRACE_MSG(MMIWRE_WINTAB_3032_112_2_18_3_11_24_525,"enter YY message hook")
TRACE_MSG(MMIWRE_WINTAB_3040_112_2_18_3_11_24_526,"WRE_Get_Window_YY_Message: wre_start_updatas is NULL")
TRACE_MSG(MMIWRE_WINTAB_3045_112_2_18_3_11_25_527,"WRE_Get_Window_YY_Message: yy_exit_app is NULL")
TRACE_MSG(MMIWRE_WINTAB_3050_112_2_18_3_11_25_528,"WRE_Get_Window_YY_Message: filename is NULL")
TRACE_MSG(MMIWRE_WINTAB_3054_112_2_18_3_11_25_529,"WRE_Get_Window_YY_Message: strcode is 0")
TRACE_MSG(MMIWRE_WINTAB_3073_112_2_18_3_11_25_530,"[WRE]register device")
TRACE_MSG(MMIWRE_WINTAB_3088_112_2_18_3_11_25_531,"[WRE]check version")
TRACE_MSG(MMIWRE_WINTAB_3101_112_2_18_3_11_25_532,"WRE_NetworkSelectGUI: SelectNetworkType return FALSE")
TRACE_MSG(MMIWRE_WINTAB_3142_112_2_18_3_11_25_533,"[wre]:g_is_lose_focus = TURE")
TRACE_MSG(MMIWRE_WINTAB_3153_112_2_18_3_11_25_534,"[wre]:g_is_lose_focus = TURE")
END_TRACE_MAP(MMI_APP_WRE_BOOT_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_WRE_BOOT_TRC_H_


/******************************************************************************
 ** File Name:      mmi_app_ikeybackup_trc.h                                         *
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
//trace_id:184
#ifndef _MMI_APP_IKEYBACKUP_TRC_H_
#define _MMI_APP_IKEYBACKUP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIIKEYBACKUP_API_71_112_2_18_2_24_21_0 "[MMIIKB] MMIAPIIKB_InitModule begin."
#define MMIIKEYBACKUP_API_89_112_2_18_2_24_21_1 "[MMIIKB] MMIAPIIKB_Init begin."
#define MMIIKEYBACKUP_API_111_112_2_18_2_24_22_2 "[MMIIKB] MMIAPIIKB_RegTypeFunc param is null."
#define MMIIKEYBACKUP_API_120_112_2_18_2_24_22_3 "[MMIIKB] MMIAPIIKB_RegTypeFunc param is invalid."
#define MMIIKEYBACKUP_API_139_112_2_18_2_24_22_4 "[MMIIKB] MMIAPIIKB_RegTypeFunc type=%d, op=%d, backup=%d,is_ba_asy=%d, restore=%d, is_re_asy=%d, total_fun=%d"
#define MMIIKEYBACKUP_API_200_112_2_18_2_24_22_5 "[MMIIKB] MMIAPIIKB_ApplicationTerminate."
#define MMIIKEYBACKUP_API_218_112_2_18_2_24_22_6 "[MMIIKB] MMIAPIIKB_BackupAllItemReq param is null."
#define MMIIKEYBACKUP_API_239_112_2_18_2_24_22_7 "[MMIIKB] MMIAPIIKB_SendBackupItemDataRes param is null."
#define MMIIKEYBACKUP_API_260_112_2_18_2_24_22_8 "[MMIIKB] MMIAPIIKB_RestoreAllItemReq param is null."
#define MMIIKEYBACKUP_API_281_112_2_18_2_24_22_9 "[MMIIKB] MMIAPIIKB_SendRestoreItemDataRes param is null."
#define MMIIKEYBACKUP_API_304_112_2_18_2_24_22_10 "[MMIIKB] MMIAPIIKB_CancelActionReq param is 0, and task_type=%d."
#define MMIIKEYBACKUP_API_330_112_2_18_2_24_22_11 "[MMIIKB] MMIAPIIKB_RegTypeFunc type=%d, op=%d, backup=%d,is_ba_asy=%d, restore=%d, is_re_asy=%d, total_fun=%d"
#define MMIIKEYBACKUP_API_396_112_2_18_2_24_22_12 "[MMIIKB] MMIIKB_RecoverConfigInfo param is null."
#define MMIIKEYBACKUP_API_411_112_2_18_2_24_22_13 "[MMIIKB] MMIIKB_RecoverConfigInfo is recover."
#define MMIIKEYBACKUP_API_434_112_2_18_2_24_22_14 "[MMIIKB] MMIIKB_GetCurBaseDir param is null."
#define MMIIKEYBACKUP_API_441_112_2_18_2_24_22_15 "[MMIIKB] MMIIKB_GetCurBaseDir: dir value=%d!"
#define MMIIKEYBACKUP_API_468_112_2_18_2_24_22_16 "[MMIIKB] MMIIKB_GetCurBaseDir: dir value=%d!"
#define MMIIKEYBACKUP_API_496_112_2_18_2_24_22_17 "[MMIIKB] MMIIKB_GetTypeProcessParamPtr type=%d wrong."
#define MMIIKEYBACKUP_TASK_590_112_2_18_2_24_25_18 "[MMIIKB] IKeyBackup_StartIKBHandlerTask task_id=%04X."
#define MMIIKEYBACKUP_TASK_718_112_2_18_2_24_26_19 "[MMIIKB] IKeyBackup_GetTotalSizeByType param_ptr is null, type=%d."
#define MMIIKEYBACKUP_TASK_728_112_2_18_2_24_26_20 "[MMIIKB] IKeyBackup_GetTotalSizeByType param_ptr is null, type=%d."
#define MMIIKEYBACKUP_TASK_736_112_2_18_2_24_26_21 "[MMIIKB] IKeyBackup_GetTotalSizeByType call_back is null, type=%d."
#define MMIIKEYBACKUP_TASK_748_112_2_18_2_24_26_22 "[MMIIKB] IKeyBackup_GetTotalSizeByType error_code is %d, type=%d, total_type=%d, total_size=%d."
#define MMIIKEYBACKUP_TASK_767_112_2_18_2_24_26_23 "[MMIIKB] IKeyBackup_BackupAllItemReq param is null."
#define MMIIKEYBACKUP_TASK_776_112_2_18_2_24_26_24 "[MMIIKB] IKeyBackup_BackupAllItemReq memory is out."
#define MMIIKEYBACKUP_TASK_790_112_2_18_2_24_26_25 "[MMIIKB] IKeyBackup_BackupAllItemReq request success."
#define MMIIKEYBACKUP_TASK_812_112_2_18_2_24_26_26 "[MMIIKB] IKeyBackup_SendBackupItemDataRes param is null."
#define MMIIKEYBACKUP_TASK_823_112_2_18_2_24_26_27 "[MMIIKB] IKeyBackup_SendBackupItemDataRes sig_item_data_ptr memory is out."
#define MMIIKEYBACKUP_TASK_860_112_2_18_2_24_26_28 "[MMIIKB] IKeyBackup_SendBackupItemDataRes sig_item_data_ptr->buf memory is out."
#define MMIIKEYBACKUP_TASK_890_112_2_18_2_24_26_29 "[MMIIKB] IKeyBackup_SendBackupItemDataRes sig_item_data_ptr->path_ptr memory is out."
#define MMIIKEYBACKUP_TASK_903_112_2_18_2_24_26_30 "[MMIIKB] IKeyBackup_SendBackupItemDataRes request success."
#define MMIIKEYBACKUP_TASK_925_112_2_18_2_24_26_31 "[MMIIKB] IKeyBackup_RestoreAllItemReq param is null."
#define MMIIKEYBACKUP_TASK_950_112_2_18_2_24_26_32 "[MMIIKB] IKeyBackup_RestoreAllItemReq memory is out."
#define MMIIKEYBACKUP_TASK_962_112_2_18_2_24_26_33 "[MMIIKB] IKeyBackup_RestoreAllItemReq request success."
#define MMIIKEYBACKUP_TASK_984_112_2_18_2_24_26_34 "[MMIIKB] IKeyBackup_SendRestoreItemDataRes param is null."
#define MMIIKEYBACKUP_TASK_995_112_2_18_2_24_26_35 "[MMIIKB] IKeyBackup_SendRestoreItemDataRes sig_item_data_ptr memory is out."
#define MMIIKEYBACKUP_TASK_1019_112_2_18_2_24_26_36 "[MMIIKB] IKeyBackup_SendRestoreItemDataRes request success."
#define MMIIKEYBACKUP_TASK_1042_112_2_18_2_24_26_37 "[MMIIKB] IKeyBackup_SendProgressInfo param is null."
#define MMIIKEYBACKUP_TASK_1050_112_2_18_2_24_26_38 "[MMIIKB] IKeyBackup_SendProgressInfo meory is out."
#define MMIIKEYBACKUP_TASK_1072_112_2_18_2_24_26_39 "[MMIIKB] IKeyBackup_SendProgressInfo type=%d, task_type=%d, state=%d, cur_num=%d, totol_num=%d, cur_size=%d, total_size=%d, cur_cnt=%d total_cnt=%d."
#define MMIIKEYBACKUP_TASK_1096_112_2_18_2_24_26_40 "[MMIIKB] IKeyBackup_CancelActionReq param is 0, and task_type=%d."
#define MMIIKEYBACKUP_TASK_1106_112_2_18_2_24_26_41 "[MMIIKB] IKeyBackup_CancelActionReq memory is out."
#define MMIIKEYBACKUP_TASK_1119_112_2_18_2_24_26_42 "[MMIIKB] IKeyBackup_CancelActionReq request success."
#define MMIIKEYBACKUP_TASK_1162_112_2_18_2_24_26_43 "[MMIIKB] IKeyBackup_CalCrcItemData param is null."
#define MMIIKEYBACKUP_TASK_1255_112_2_18_2_24_27_44 "[MMIIKB] ReadContentHead error_code=%d."
#define MMIIKEYBACKUP_TASK_1274_112_2_18_2_24_27_45 "[MMIIKB] IKeyBackup_IsCrcRight param is null."
#define MMIIKEYBACKUP_TASK_1296_112_2_18_2_24_27_46 "[MMIIKB] IKeyBackup_IsCrcRight error_code=%d."
#define MMIIKEYBACKUP_TASK_1345_112_2_18_2_24_27_47 "[MMIIKB] IKeyBackup_EncBuf param is null."
#define MMIIKEYBACKUP_TASK_1353_112_2_18_2_24_27_48 "[MMIIKB] IKeyBackup_EncBuf cancel by user."
#define MMIIKEYBACKUP_TASK_1393_112_2_18_2_24_27_49 "[MMIIKB] IKeyBackup_DecBuf param is null."
#define MMIIKEYBACKUP_TASK_1401_112_2_18_2_24_27_50 "[MMIIKB] IKeyBackup_DecBuf cancel by user."
#define MMIIKEYBACKUP_TASK_1439_112_2_18_2_24_27_51 "[MMIIKB] IKeyBackup_GetPackContentNumber no memory."
#define MMIIKEYBACKUP_TASK_1455_112_2_18_2_24_27_52 "[MMIIKB] IKeyBackup_GetPackContentNumber no memory."
#define MMIIKEYBACKUP_TASK_1499_112_2_18_2_24_27_53 "[MMIIKB] Command is: MSG_IKB_LOAD_ALL_NODE_REQ"
#define MMIIKEYBACKUP_TASK_1502_112_2_18_2_24_27_54 "[MMIIKB] Command is: MSG_IKB_BACKUP_DATA_REQ"
#define MMIIKEYBACKUP_TASK_1508_112_2_18_2_24_27_55 "[MMIIKB] Command is: MSG_IKB_BACKUP_ITEM_DATA_RES"
#define MMIIKEYBACKUP_TASK_1512_112_2_18_2_24_27_56 "[MMIIKB] Command is: MSG_IKB_RESTORE_DATA_REQ"
#define MMIIKEYBACKUP_TASK_1516_112_2_18_2_24_27_57 "[MMIIKB] Command is: MSG_IKB_RESTORE_ITEM_DATA_RES"
#define MMIIKEYBACKUP_TASK_1521_112_2_18_2_24_27_58 "[MMIIKB] Command is: MSG_IKB_CANCEL_ACTION_REQ"
#define MMIIKEYBACKUP_TASK_1527_112_2_18_2_24_27_59 "[MMIIKB] Command is: IKB_SIG_DEFAULT code=%d"
#define MMIIKEYBACKUP_TASK_1531_112_2_18_2_24_27_60 "[MMIIKB] IKBHandlerMain ret_code = %d\n"
#define MMIIKEYBACKUP_TASK_1550_112_2_18_2_24_27_61 "[MMIIKB] IKBBackupAllItems parma is null."
#define MMIIKEYBACKUP_TASK_1560_112_2_18_2_24_27_62 "[MMIIKB] IKBBackupAllItems s_backup_req_ptr is no-null."
#define MMIIKEYBACKUP_TASK_1572_112_2_18_2_24_27_63 "[MMIIKB] IKBBackupAllItems parma memory is out."
#define MMIIKEYBACKUP_TASK_1689_112_2_18_2_24_28_64 "[MMIIKB] SetContentFileName param is null."
#define MMIIKEYBACKUP_TASK_1697_112_2_18_2_24_28_65 "[MMIIKB] SetContentFileName can't get content name withtype=%d."
#define MMIIKEYBACKUP_TASK_1726_112_2_18_2_24_28_66 "[MMIIKB] SetBackupResParmater param is null."
#define MMIIKEYBACKUP_TASK_1772_112_2_18_2_24_28_67 "[MMIIKB] BackupCircleTask begin."
#define MMIIKEYBACKUP_TASK_1808_112_2_18_2_24_28_68 "[MMIIKB] BackupCircleTask end with error_code=%d, is_asy_ret=%d, has_backup_item=%d."
#define MMIIKEYBACKUP_TASK_1888_112_2_18_2_24_28_69 "[MMIIKB] BackupResProcess send response to app."
#define MMIIKEYBACKUP_TASK_1946_112_2_18_2_24_28_70 "[MMIIKB] InitTaskSeqTable success."
#define MMIIKEYBACKUP_TASK_1986_112_2_18_2_24_28_71 "[MMIIKB] ResetTaskSeqTable."
#define MMIIKEYBACKUP_TASK_2011_112_2_18_2_24_28_72 "[MMIIKB] MMIIKB_GetTaskItemByType param is type=%d."
#define MMIIKEYBACKUP_TASK_2042_112_2_18_2_24_28_73 "[MMIIKB] SetTaskTableMaxValue param is null."
#define MMIIKEYBACKUP_TASK_2082_112_2_18_2_24_28_74 "[MMIIKB] SetTaskTableMaxValue type=%d, error_code=%d, total_type=%d, total_size=%d"
#define MMIIKEYBACKUP_TASK_2109_112_2_18_2_24_28_75 "[MMIIKB] UpdateTaskTable param is type=%d."
#define MMIIKEYBACKUP_TASK_2216_112_2_18_2_24_29_76 "[MMIIKB] BackupItemDataSyn param is null."
#define MMIIKEYBACKUP_TASK_2264_112_2_18_2_24_29_77 "[MMIIKB] BackupItemDataSynByBuf param is null."
#define MMIIKEYBACKUP_TASK_2276_112_2_18_2_24_29_78 "[MMIIKB] BackupItemDataSynByBuf can't get file path."
#define MMIIKEYBACKUP_TASK_2317_112_2_18_2_24_29_79 "[MMIIKB] BackupItemDataSynByBuf get data from app error. type=%d, in.begin=%d, in.len=%d, in.num=%d, in.trans=%d, out.type=%d, out.begin=%d, out.len=%d, out.num=%d, out.fin=%d, error_code=%d"
#define MMIIKEYBACKUP_TASK_2325_112_2_18_2_24_29_80 "[MMIIKB] BackupItemDataSynByBuf get buf is null,type=%d"
#define MMIIKEYBACKUP_TASK_2335_112_2_18_2_24_29_81 "[MMIIKB] BackupItemDataSynByBuf memory is out,type=%d"
#define MMIIKEYBACKUP_TASK_2379_112_2_18_2_24_29_82 "[MMIIKB] BackupItemDataSynByBuf writedata error,type=%d, error_code=%d"
#define MMIIKEYBACKUP_TASK_2438_112_2_18_2_24_29_83 "[MMIIKB] BackupItemDataSynByBuf encryp data error,type=%d"
#define MMIIKEYBACKUP_TASK_2447_112_2_18_2_24_29_84 "[MMIIKB] BackupItemDataSynByBuf cancel by user."
#define MMIIKEYBACKUP_TASK_2454_112_2_18_2_24_29_85 "[MMIIKB] BackupItemDataSynByBuf end with type=%d,error_code=%d."
#define MMIIKEYBACKUP_TASK_2501_112_2_18_2_24_29_86 "[MMIIKB] BackupItemDataAsy param is null."
#define MMIIKEYBACKUP_TASK_2509_112_2_18_2_24_29_87 "[MMIIKB] BackupItemDataAsy task is null type=%d."
#define MMIIKEYBACKUP_TASK_2556_112_2_18_2_24_29_88 "[MMIIKB] BackupHandleItemUnitDataAsy param is null."
#define MMIIKEYBACKUP_TASK_2607_112_2_18_2_24_29_89 "[MMIIKB] BackupHandleUnitDataAsyByBuf param is null."
#define MMIIKEYBACKUP_TASK_2619_112_2_18_2_24_29_90 "[MMIIKB] BackupHandleUnitDataAsyByBuf can't get file path."
#define MMIIKEYBACKUP_TASK_2636_112_2_18_2_24_29_91 "[MMIIKB] BackupHandleUnitDataAsyByBuf app buf is null, type=%d."
#define MMIIKEYBACKUP_TASK_2647_112_2_18_2_24_29_92 "[MMIIKB] BackupHandleUnitDataAsyByBuf memory is out. type=%d."
#define MMIIKEYBACKUP_TASK_2737_112_2_18_2_24_30_93 "[MMIIKB] BackupHandleUnitDataAsyByBuf cancel by user."
#define MMIIKEYBACKUP_TASK_2745_112_2_18_2_24_30_94 "[MMIIKB] BackupHandleUnitDataAsyByBuf end failed,type=%d, error_code=%d."
#define MMIIKEYBACKUP_TASK_2801_112_2_18_2_24_30_95 "[MMIIKB] IKBRestoreAllItems parma is null."
#define MMIIKEYBACKUP_TASK_2811_112_2_18_2_24_30_96 "[MMIIKB] IKBRestoreAllItems s_restore_req_ptr is no-null."
#define MMIIKEYBACKUP_TASK_2823_112_2_18_2_24_30_97 "[MMIIKB] IKBRestoreAllItems memory1 is out."
#define MMIIKEYBACKUP_TASK_2836_112_2_18_2_24_30_98 "[MMIIKB] IKBRestoreAllItems memory2 is out."
#define MMIIKEYBACKUP_TASK_3016_112_2_18_2_24_30_99 "[MMIIKB] RestoreCircleTask cur_type=%d, task_type=%d"
#define MMIIKEYBACKUP_TASK_3035_112_2_18_2_24_30_100 "[MMIIKB] RestoreCircleTask restore syn type=%d fail."
#define MMIIKEYBACKUP_TASK_3044_112_2_18_2_24_30_101 "[MMIIKB] RestoreCircleTask restore syn type=%d suc."
#define MMIIKEYBACKUP_TASK_3056_112_2_18_2_24_30_102 "[MMIIKB] RestoreCircleTask restore asy type=%d fail."
#define MMIIKEYBACKUP_TASK_3066_112_2_18_2_24_30_103 "[MMIIKB] RestoreCircleTask restore asy type=%d suc."
#define MMIIKEYBACKUP_TASK_3076_112_2_18_2_24_31_104 "[MMIIKB] RestoreCircleTask end with error_code=%d,is_asy_ret=%d, has_restore_item=%d."
#define MMIIKEYBACKUP_TASK_3119_112_2_18_2_24_31_105 "[MMIIKB] RestoreResProcess param is null."
#define MMIIKEYBACKUP_TASK_3144_112_2_18_2_24_31_106 "[MMIIKB] RestoreResProcess send response to app."
#define MMIIKEYBACKUP_TASK_3182_112_2_18_2_24_31_107 "[MMIIKB] RestoreItemDataSyn param is null."
#define MMIIKEYBACKUP_TASK_3230_112_2_18_2_24_31_108 "[MMIIKB] RestoreItemDataSynByBuf param is null."
#define MMIIKEYBACKUP_TASK_3243_112_2_18_2_24_31_109 "[MMIIKB] RestoreItemDataSynByBuf can't get file path."
#define MMIIKEYBACKUP_TASK_3250_112_2_18_2_24_31_110 "[MMIIKB] RestoreItemDataSynByBuf file is not exist."
#define MMIIKEYBACKUP_TASK_3291_112_2_18_2_24_31_111 "[MMIIKB] RestoreItemDataSynByBuf task null, type=%d."
#define MMIIKEYBACKUP_TASK_3369_112_2_18_2_24_31_112 "[MMIIKB] RestoreItemDataSynByBuf send unitdata error, restore error by type=%d, error=%dtrans_idx=%d, begin=%d, len=%d"
#define MMIIKEYBACKUP_TASK_3391_112_2_18_2_24_31_113 "[MMIIKB] BackupItemDataSynByBuf cancel by user."
#define MMIIKEYBACKUP_TASK_3406_112_2_18_2_24_31_114 "[MMIIKB] RestoreItemDataSynByBuf end with type=%d,error_code=%d."
#define MMIIKEYBACKUP_TASK_3454_112_2_18_2_24_31_115 "[MMIIKB] RestoreItemDataAsy param is null."
#define MMIIKEYBACKUP_TASK_3463_112_2_18_2_24_31_116 "[MMIIKB] RestoreItemDataAsy task null, type=%d."
#define MMIIKEYBACKUP_TASK_3476_112_2_18_2_24_31_117 "[MMIIKB] RestoreItemDataAsy can't get file path."
#define MMIIKEYBACKUP_TASK_3483_112_2_18_2_24_31_118 "[MMIIKB] RestoreItemDataAsy file is not exist."
#define MMIIKEYBACKUP_TASK_3542_112_2_18_2_24_31_119 "[MMIIKB] SendUnitDataCircle param is null."
#define MMIIKEYBACKUP_TASK_3624_112_2_18_2_24_32_120 "[MMIIKB] RestoreHandleItemUnitDataAsy param is null."
#define MMIIKEYBACKUP_TASK_3669_112_2_18_2_24_32_121 "[MMIIKB] RestoreHandleUnitDataAsyByBuf param is null."
#define MMIIKEYBACKUP_TASK_3683_112_2_18_2_24_32_122 "[MMIIKB] RestoreHandleUnitDataAsyByBuf can't get file path."
#define MMIIKEYBACKUP_TASK_3690_112_2_18_2_24_32_123 "[MMIIKB] RestoreHandleUnitDataAsyByBuf file is not exist."
#define MMIIKEYBACKUP_TASK_3705_112_2_18_2_24_32_124 "[MMIIKB] RestoreHandleUnitDataAsyByBuf this item is end,type=%d, error_code=%d."
#define MMIIKEYBACKUP_TASK_3775_112_2_18_2_24_32_125 "[MMIIKB] RestoreHandleUnitDataAsyByBuf this uint is end,type=%d, error_code=%d."
#define MMIIKEYBACKUP_TASK_3781_112_2_18_2_24_32_126 "[MMIIKB] RestoreHandleUnitDataAsyByBuf cancel by user."
#define MMIIKEYBACKUP_TASK_3838_112_2_18_2_24_32_127 "[MMIIKB] IKBCancelAction param is null."
#define MMIIKEYBACKUP_TASK_3855_112_2_18_2_24_32_128 "[MMIIKB] IKBCancelAction memory is out."
#define MMIIKEYBACKUP_TASK_3892_112_2_18_2_24_32_129 "[MMIIKB] IKBCancelAction task_type=%d, error_code =%dn"
#define MMIIKEYBACKUP_TASK_3907_112_2_18_2_24_32_130 "[MMIIKB] CreateProtectionTimer create timer, begin on=%d."
#define MMIIKEYBACKUP_TASK_3924_112_2_18_2_24_32_131 "[MMIIKB] StopProtectionTimer stop time, end on=%d."
#define MMIIKEYBACKUP_TASK_3945_112_2_18_2_24_32_132 "[MMIIKB] ProcessProtectionTimerCB time out, end on=%d."
#define MMIIKEYBACKUP_TASK_3951_112_2_18_2_24_32_133 "[MMIIKB] ProcessProtectionTimerCB backup time out."
#define MMIIKEYBACKUP_TASK_3956_112_2_18_2_24_32_134 "[MMIIKB] ProcessProtectionTimerCB restore time out."
#define MMIIKEYBACKUP_TASK_3961_112_2_18_2_24_32_135 "[MMIIKB] ProcessProtectionTimerCB all task is unbusy."
#define MMIIKEYBACKUP_TASK_3988_112_2_18_2_24_32_136 "[MMIIKB] GetContentFilePath param is null."
#define MMIIKEYBACKUP_TASK_4025_112_2_18_2_24_32_137 "[MMIIKB] ReceiveUnitData param is null."
#define MMIIKEYBACKUP_TASK_4032_112_2_18_2_24_32_138 "[MMIIKB] ReceiveUnitData backup_call_back is null, type=%d."
#define MMIIKEYBACKUP_TASK_4071_112_2_18_2_24_32_139 "[MMIIKB] WriteUnitData param is null."
#define MMIIKEYBACKUP_TASK_4166_112_2_18_2_24_33_140 "[MMIIKB] WriteContentHead param is null."
#define MMIIKEYBACKUP_TASK_4221_112_2_18_2_24_33_141 "[MMIIKB] SendUnitData param is null."
#define MMIIKEYBACKUP_TASK_4228_112_2_18_2_24_33_142 "[MMIIKB] SendUnitData restore_call_back is null, type=%d."
#define MMIIKEYBACKUP_TASK_4264_112_2_18_2_24_33_143 "[MMIIKB] ReadUnitData param is null."
#define MMIIKEYBACKUP_TASK_4311_112_2_18_2_24_33_144 "[MMIIKB] ReadUnitData offset=%d, error_code=%d."
#define MMIIKEYBACKUP_TASK_4336_112_2_18_2_24_33_145 "[MMIIKB] ReadContentHead param is null."
#define MMIIKEYBACKUP_TASK_4373_112_2_18_2_24_33_146 "[MMIIKB] ReadContentHead error_code=%d."
#define MMIIKEYBACKUP_WINTAB_995_112_2_18_2_24_36_147 "[MMIIKB] MMIAPIIKB_HandleAppSigRes: invalid param!"
#define MMIIKEYBACKUP_WINTAB_1001_112_2_18_2_24_36_148 "[MMIIKB] MMIAPIIKB_HandleAppSigRes msg_id=%d, param=%d"
#define MMIIKEYBACKUP_WINTAB_1279_112_2_18_2_24_36_149 "[MMIIKB] ShowTips: error_code=%d!"
#define MMIIKEYBACKUP_WINTAB_1418_112_2_18_2_24_36_150 "[MMIIKB] InitListUIValue: param is null."
#define MMIIKEYBACKUP_WINTAB_1455_112_2_18_2_24_37_151 "[MMIIKB] GetListItemSelectValues: param is null."
#define MMIIKEYBACKUP_WINTAB_1491_112_2_18_2_24_37_152 "[MMIIKB] GetOneListItemValue: param is null."
#define MMIIKEYBACKUP_WINTAB_1551_112_2_18_2_24_37_153 "[MMIIKB] AppendALLListItemCtrl: param is null."
#define MMIIKEYBACKUP_WINTAB_1592_112_2_18_2_24_37_154 "[MMIIKB] OnClickCheckListItem: param is null."
#define MMIIKEYBACKUP_WINTAB_1638_112_2_18_2_24_37_155 "[MMIIKB] FormatTotalValue: param is null."
#define MMIIKEYBACKUP_WINTAB_1717_112_2_18_2_24_37_156 "[MMIIKB] GetFirstTypeTotalValue: param is null."
#define MMIIKEYBACKUP_WINTAB_1732_112_2_18_2_24_37_157 "[MMIIKB] GetFirstTypeTotalValue types=%d, first total=%d, type=%d"
#define MMIIKEYBACKUP_WINTAB_1758_112_2_18_2_24_37_158 "[MMIIKB] UpdateProcessBar: param is null."
#define MMIIKEYBACKUP_WINTAB_1775_112_2_18_2_24_37_159 "[MMIIKB] UpdateProcessBar: win_id=%04x."
#define MMIIKEYBACKUP_WINTAB_1887_112_2_18_2_24_37_160 "[MMIIKB] CancelCurAction param is 0, task_type=%d."
#define MMIIKEYBACKUP_WINTAB_1933_112_2_18_2_24_37_161 "[MMIIKB] HandleCancelActionQueryMsg param is 0, task_type=%d."
#define MMIIKEYBACKUP_WINTAB_2016_112_2_18_2_24_38_162 "[MMIIKB] DeletePhysicalDir param is null."
#define MMIIKEYBACKUP_WINTAB_2023_112_2_18_2_24_38_163 "[MMIIKB] DeletePhysicalDir no memory."
#define MMIIKEYBACKUP_WINTAB_2031_112_2_18_2_24_38_164 "[MMIIKB] DeletePhysicalDir no memory."
#define MMIIKEYBACKUP_WINTAB_2269_112_2_18_2_24_38_165 "[MMIIKB] OptimizeBackupSelTypes types=%d, opt_types=%d"
#define MMIIKEYBACKUP_WINTAB_2289_112_2_18_2_24_38_166 "[MMIIKB] PreCheckBackupItems param is null, types=%d."
#define MMIIKEYBACKUP_WINTAB_2308_112_2_18_2_24_38_167 "[MMIIKB] PreCheckBackupItems types, opt_types=%d."
#define MMIIKEYBACKUP_WINTAB_2345_112_2_18_2_24_38_168 "[MMIIKB] IsNeedInputPsd param is null, types=0."
#define MMIIKEYBACKUP_WINTAB_2366_112_2_18_2_24_38_169 "[MMIIKB] BackupSelItems param is null."
#define MMIIKEYBACKUP_WINTAB_2371_112_2_18_2_24_38_170 "[MMIIKB] BackupSelItems types=%d."
#define MMIIKEYBACKUP_WINTAB_2416_112_2_18_2_24_38_171 "[MMIIKB] StartBackupItems param is null."
#define MMIIKEYBACKUP_WINTAB_2467_112_2_18_2_24_39_172 "[MMIIKB] StartBackupItems send request, types=%d."
#define MMIIKEYBACKUP_WINTAB_2744_112_2_18_2_24_39_173 "[MMIIKB] OnClickRestorePswSoftkey win_data is null."
#define MMIIKEYBACKUP_WINTAB_2804_112_2_18_2_24_39_174 "[MMIIKB] HandleEnterRestorePwd win_data is null."
#define MMIIKEYBACKUP_WINTAB_3052_112_2_18_2_24_40_175 "[MMIIKB] PreCheckRestoreItems types=%d."
#define MMIIKEYBACKUP_WINTAB_3070_112_2_18_2_24_40_176 "[MMIIKB] RestoreSelItems types=%d."
#define MMIIKEYBACKUP_WINTAB_3115_112_2_18_2_24_40_177 "[MMIIKB] GetPackPswInfo param is null."
#define MMIIKEYBACKUP_WINTAB_3184_112_2_18_2_24_40_178 "[MMIIKB] StartRestoreItems send request, pack_types=%d,sel_types=%d, res_types=%d, index=%d"
#define MMIIKEYBACKUP_WINTAB_3526_112_2_18_2_24_41_179 "[MMIIKB] AppendPackListItem ctrl_id=%d, item_index=%d, types=%d, ptr=%d."
#define MMIIKEYBACKUP_WINTAB_3671_112_2_18_2_24_41_180 "[MMIIKB] GetPackContentNumber no memory."
#define MMIIKEYBACKUP_WINTAB_3679_112_2_18_2_24_41_181 "[MMIIKB] GetPackContentNumber no memory."
#define MMIIKEYBACKUP_WINTAB_3724_112_2_18_2_24_41_182 "[MMIIKB] InitRestoreSelItemList param is null."
#define MMIIKEYBACKUP_WINTAB_4022_112_2_18_2_24_42_183 "[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg menu_id=%x, item_index=%d"
#define MMIIKEYBACKUP_WINTAB_4033_112_2_18_2_24_42_184 "[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg no memory."
#define MMIIKEYBACKUP_WINTAB_4200_112_2_18_2_24_42_185 "[MMIIKB] HandleRestoreListOptMenuWinMsg menu_id=%x, item_index=%d"
#define MMIIKEYBACKUP_WINTAB_4216_112_2_18_2_24_42_186 "[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg no memory."
#define MMIIKEYBACKUP_WINTAB_4237_112_2_18_2_24_42_187 "[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg no memory."
#define MMIIKEYBACKUP_WINTAB_4417_112_2_18_2_24_43_188 "[MMIIKB] InitSettingListCtrl param is null."
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_IKEYBACKUP_TRC)
TRACE_MSG(MMIIKEYBACKUP_API_71_112_2_18_2_24_21_0,"[MMIIKB] MMIAPIIKB_InitModule begin.")
TRACE_MSG(MMIIKEYBACKUP_API_89_112_2_18_2_24_21_1,"[MMIIKB] MMIAPIIKB_Init begin.")
TRACE_MSG(MMIIKEYBACKUP_API_111_112_2_18_2_24_22_2,"[MMIIKB] MMIAPIIKB_RegTypeFunc param is null.")
TRACE_MSG(MMIIKEYBACKUP_API_120_112_2_18_2_24_22_3,"[MMIIKB] MMIAPIIKB_RegTypeFunc param is invalid.")
TRACE_MSG(MMIIKEYBACKUP_API_139_112_2_18_2_24_22_4,"[MMIIKB] MMIAPIIKB_RegTypeFunc type=%d, op=%d, backup=%d,is_ba_asy=%d, restore=%d, is_re_asy=%d, total_fun=%d")
TRACE_MSG(MMIIKEYBACKUP_API_200_112_2_18_2_24_22_5,"[MMIIKB] MMIAPIIKB_ApplicationTerminate.")
TRACE_MSG(MMIIKEYBACKUP_API_218_112_2_18_2_24_22_6,"[MMIIKB] MMIAPIIKB_BackupAllItemReq param is null.")
TRACE_MSG(MMIIKEYBACKUP_API_239_112_2_18_2_24_22_7,"[MMIIKB] MMIAPIIKB_SendBackupItemDataRes param is null.")
TRACE_MSG(MMIIKEYBACKUP_API_260_112_2_18_2_24_22_8,"[MMIIKB] MMIAPIIKB_RestoreAllItemReq param is null.")
TRACE_MSG(MMIIKEYBACKUP_API_281_112_2_18_2_24_22_9,"[MMIIKB] MMIAPIIKB_SendRestoreItemDataRes param is null.")
TRACE_MSG(MMIIKEYBACKUP_API_304_112_2_18_2_24_22_10,"[MMIIKB] MMIAPIIKB_CancelActionReq param is 0, and task_type=%d.")
TRACE_MSG(MMIIKEYBACKUP_API_330_112_2_18_2_24_22_11,"[MMIIKB] MMIAPIIKB_RegTypeFunc type=%d, op=%d, backup=%d,is_ba_asy=%d, restore=%d, is_re_asy=%d, total_fun=%d")
TRACE_MSG(MMIIKEYBACKUP_API_396_112_2_18_2_24_22_12,"[MMIIKB] MMIIKB_RecoverConfigInfo param is null.")
TRACE_MSG(MMIIKEYBACKUP_API_411_112_2_18_2_24_22_13,"[MMIIKB] MMIIKB_RecoverConfigInfo is recover.")
TRACE_MSG(MMIIKEYBACKUP_API_434_112_2_18_2_24_22_14,"[MMIIKB] MMIIKB_GetCurBaseDir param is null.")
TRACE_MSG(MMIIKEYBACKUP_API_441_112_2_18_2_24_22_15,"[MMIIKB] MMIIKB_GetCurBaseDir: dir value=%d!")
TRACE_MSG(MMIIKEYBACKUP_API_468_112_2_18_2_24_22_16,"[MMIIKB] MMIIKB_GetCurBaseDir: dir value=%d!")
TRACE_MSG(MMIIKEYBACKUP_API_496_112_2_18_2_24_22_17,"[MMIIKB] MMIIKB_GetTypeProcessParamPtr type=%d wrong.")
TRACE_MSG(MMIIKEYBACKUP_TASK_590_112_2_18_2_24_25_18,"[MMIIKB] IKeyBackup_StartIKBHandlerTask task_id=%04X.")
TRACE_MSG(MMIIKEYBACKUP_TASK_718_112_2_18_2_24_26_19,"[MMIIKB] IKeyBackup_GetTotalSizeByType param_ptr is null, type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_728_112_2_18_2_24_26_20,"[MMIIKB] IKeyBackup_GetTotalSizeByType param_ptr is null, type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_736_112_2_18_2_24_26_21,"[MMIIKB] IKeyBackup_GetTotalSizeByType call_back is null, type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_748_112_2_18_2_24_26_22,"[MMIIKB] IKeyBackup_GetTotalSizeByType error_code is %d, type=%d, total_type=%d, total_size=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_767_112_2_18_2_24_26_23,"[MMIIKB] IKeyBackup_BackupAllItemReq param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_776_112_2_18_2_24_26_24,"[MMIIKB] IKeyBackup_BackupAllItemReq memory is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_790_112_2_18_2_24_26_25,"[MMIIKB] IKeyBackup_BackupAllItemReq request success.")
TRACE_MSG(MMIIKEYBACKUP_TASK_812_112_2_18_2_24_26_26,"[MMIIKB] IKeyBackup_SendBackupItemDataRes param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_823_112_2_18_2_24_26_27,"[MMIIKB] IKeyBackup_SendBackupItemDataRes sig_item_data_ptr memory is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_860_112_2_18_2_24_26_28,"[MMIIKB] IKeyBackup_SendBackupItemDataRes sig_item_data_ptr->buf memory is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_890_112_2_18_2_24_26_29,"[MMIIKB] IKeyBackup_SendBackupItemDataRes sig_item_data_ptr->path_ptr memory is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_903_112_2_18_2_24_26_30,"[MMIIKB] IKeyBackup_SendBackupItemDataRes request success.")
TRACE_MSG(MMIIKEYBACKUP_TASK_925_112_2_18_2_24_26_31,"[MMIIKB] IKeyBackup_RestoreAllItemReq param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_950_112_2_18_2_24_26_32,"[MMIIKB] IKeyBackup_RestoreAllItemReq memory is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_962_112_2_18_2_24_26_33,"[MMIIKB] IKeyBackup_RestoreAllItemReq request success.")
TRACE_MSG(MMIIKEYBACKUP_TASK_984_112_2_18_2_24_26_34,"[MMIIKB] IKeyBackup_SendRestoreItemDataRes param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_995_112_2_18_2_24_26_35,"[MMIIKB] IKeyBackup_SendRestoreItemDataRes sig_item_data_ptr memory is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1019_112_2_18_2_24_26_36,"[MMIIKB] IKeyBackup_SendRestoreItemDataRes request success.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1042_112_2_18_2_24_26_37,"[MMIIKB] IKeyBackup_SendProgressInfo param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1050_112_2_18_2_24_26_38,"[MMIIKB] IKeyBackup_SendProgressInfo meory is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1072_112_2_18_2_24_26_39,"[MMIIKB] IKeyBackup_SendProgressInfo type=%d, task_type=%d, state=%d, cur_num=%d, totol_num=%d, cur_size=%d, total_size=%d, cur_cnt=%d total_cnt=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1096_112_2_18_2_24_26_40,"[MMIIKB] IKeyBackup_CancelActionReq param is 0, and task_type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1106_112_2_18_2_24_26_41,"[MMIIKB] IKeyBackup_CancelActionReq memory is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1119_112_2_18_2_24_26_42,"[MMIIKB] IKeyBackup_CancelActionReq request success.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1162_112_2_18_2_24_26_43,"[MMIIKB] IKeyBackup_CalCrcItemData param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1255_112_2_18_2_24_27_44,"[MMIIKB] ReadContentHead error_code=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1274_112_2_18_2_24_27_45,"[MMIIKB] IKeyBackup_IsCrcRight param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1296_112_2_18_2_24_27_46,"[MMIIKB] IKeyBackup_IsCrcRight error_code=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1345_112_2_18_2_24_27_47,"[MMIIKB] IKeyBackup_EncBuf param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1353_112_2_18_2_24_27_48,"[MMIIKB] IKeyBackup_EncBuf cancel by user.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1393_112_2_18_2_24_27_49,"[MMIIKB] IKeyBackup_DecBuf param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1401_112_2_18_2_24_27_50,"[MMIIKB] IKeyBackup_DecBuf cancel by user.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1439_112_2_18_2_24_27_51,"[MMIIKB] IKeyBackup_GetPackContentNumber no memory.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1455_112_2_18_2_24_27_52,"[MMIIKB] IKeyBackup_GetPackContentNumber no memory.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1499_112_2_18_2_24_27_53,"[MMIIKB] Command is: MSG_IKB_LOAD_ALL_NODE_REQ")
TRACE_MSG(MMIIKEYBACKUP_TASK_1502_112_2_18_2_24_27_54,"[MMIIKB] Command is: MSG_IKB_BACKUP_DATA_REQ")
TRACE_MSG(MMIIKEYBACKUP_TASK_1508_112_2_18_2_24_27_55,"[MMIIKB] Command is: MSG_IKB_BACKUP_ITEM_DATA_RES")
TRACE_MSG(MMIIKEYBACKUP_TASK_1512_112_2_18_2_24_27_56,"[MMIIKB] Command is: MSG_IKB_RESTORE_DATA_REQ")
TRACE_MSG(MMIIKEYBACKUP_TASK_1516_112_2_18_2_24_27_57,"[MMIIKB] Command is: MSG_IKB_RESTORE_ITEM_DATA_RES")
TRACE_MSG(MMIIKEYBACKUP_TASK_1521_112_2_18_2_24_27_58,"[MMIIKB] Command is: MSG_IKB_CANCEL_ACTION_REQ")
TRACE_MSG(MMIIKEYBACKUP_TASK_1527_112_2_18_2_24_27_59,"[MMIIKB] Command is: IKB_SIG_DEFAULT code=%d")
TRACE_MSG(MMIIKEYBACKUP_TASK_1531_112_2_18_2_24_27_60,"[MMIIKB] IKBHandlerMain ret_code = %d\n")
TRACE_MSG(MMIIKEYBACKUP_TASK_1550_112_2_18_2_24_27_61,"[MMIIKB] IKBBackupAllItems parma is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1560_112_2_18_2_24_27_62,"[MMIIKB] IKBBackupAllItems s_backup_req_ptr is no-null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1572_112_2_18_2_24_27_63,"[MMIIKB] IKBBackupAllItems parma memory is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1689_112_2_18_2_24_28_64,"[MMIIKB] SetContentFileName param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1697_112_2_18_2_24_28_65,"[MMIIKB] SetContentFileName can't get content name withtype=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1726_112_2_18_2_24_28_66,"[MMIIKB] SetBackupResParmater param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1772_112_2_18_2_24_28_67,"[MMIIKB] BackupCircleTask begin.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1808_112_2_18_2_24_28_68,"[MMIIKB] BackupCircleTask end with error_code=%d, is_asy_ret=%d, has_backup_item=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1888_112_2_18_2_24_28_69,"[MMIIKB] BackupResProcess send response to app.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1946_112_2_18_2_24_28_70,"[MMIIKB] InitTaskSeqTable success.")
TRACE_MSG(MMIIKEYBACKUP_TASK_1986_112_2_18_2_24_28_71,"[MMIIKB] ResetTaskSeqTable.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2011_112_2_18_2_24_28_72,"[MMIIKB] MMIIKB_GetTaskItemByType param is type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2042_112_2_18_2_24_28_73,"[MMIIKB] SetTaskTableMaxValue param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2082_112_2_18_2_24_28_74,"[MMIIKB] SetTaskTableMaxValue type=%d, error_code=%d, total_type=%d, total_size=%d")
TRACE_MSG(MMIIKEYBACKUP_TASK_2109_112_2_18_2_24_28_75,"[MMIIKB] UpdateTaskTable param is type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2216_112_2_18_2_24_29_76,"[MMIIKB] BackupItemDataSyn param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2264_112_2_18_2_24_29_77,"[MMIIKB] BackupItemDataSynByBuf param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2276_112_2_18_2_24_29_78,"[MMIIKB] BackupItemDataSynByBuf can't get file path.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2317_112_2_18_2_24_29_79,"[MMIIKB] BackupItemDataSynByBuf get data from app error. type=%d, in.begin=%d, in.len=%d, in.num=%d, in.trans=%dout.type=%d, out.begin=%d, out.len=%d, out.num=%d, out.fin=%derror_code=%d")
TRACE_MSG(MMIIKEYBACKUP_TASK_2325_112_2_18_2_24_29_80,"[MMIIKB] BackupItemDataSynByBuf get buf is null,type=%d")
TRACE_MSG(MMIIKEYBACKUP_TASK_2335_112_2_18_2_24_29_81,"[MMIIKB] BackupItemDataSynByBuf memory is out,type=%d")
TRACE_MSG(MMIIKEYBACKUP_TASK_2379_112_2_18_2_24_29_82,"[MMIIKB] BackupItemDataSynByBuf writedata error,type=%d, error_code=%d")
TRACE_MSG(MMIIKEYBACKUP_TASK_2438_112_2_18_2_24_29_83,"[MMIIKB] BackupItemDataSynByBuf encryp data error,type=%d")
TRACE_MSG(MMIIKEYBACKUP_TASK_2447_112_2_18_2_24_29_84,"[MMIIKB] BackupItemDataSynByBuf cancel by user.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2454_112_2_18_2_24_29_85,"[MMIIKB] BackupItemDataSynByBuf end with type=%d,error_code=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2501_112_2_18_2_24_29_86,"[MMIIKB] BackupItemDataAsy param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2509_112_2_18_2_24_29_87,"[MMIIKB] BackupItemDataAsy task is null type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2556_112_2_18_2_24_29_88,"[MMIIKB] BackupHandleItemUnitDataAsy param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2607_112_2_18_2_24_29_89,"[MMIIKB] BackupHandleUnitDataAsyByBuf param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2619_112_2_18_2_24_29_90,"[MMIIKB] BackupHandleUnitDataAsyByBuf can't get file path.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2636_112_2_18_2_24_29_91,"[MMIIKB] BackupHandleUnitDataAsyByBuf app buf is null, type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2647_112_2_18_2_24_29_92,"[MMIIKB] BackupHandleUnitDataAsyByBuf memory is out. type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2737_112_2_18_2_24_30_93,"[MMIIKB] BackupHandleUnitDataAsyByBuf cancel by user.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2745_112_2_18_2_24_30_94,"[MMIIKB] BackupHandleUnitDataAsyByBuf end failed,type=%d, error_code=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2801_112_2_18_2_24_30_95,"[MMIIKB] IKBRestoreAllItems parma is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2811_112_2_18_2_24_30_96,"[MMIIKB] IKBRestoreAllItems s_restore_req_ptr is no-null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2823_112_2_18_2_24_30_97,"[MMIIKB] IKBRestoreAllItems memory1 is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_2836_112_2_18_2_24_30_98,"[MMIIKB] IKBRestoreAllItems memory2 is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3016_112_2_18_2_24_30_99,"[MMIIKB] RestoreCircleTask cur_type=%d, task_type=%d")
TRACE_MSG(MMIIKEYBACKUP_TASK_3035_112_2_18_2_24_30_100,"[MMIIKB] RestoreCircleTask restore syn type=%d fail.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3044_112_2_18_2_24_30_101,"[MMIIKB] RestoreCircleTask restore syn type=%d suc.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3056_112_2_18_2_24_30_102,"[MMIIKB] RestoreCircleTask restore asy type=%d fail.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3066_112_2_18_2_24_30_103,"[MMIIKB] RestoreCircleTask restore asy type=%d suc.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3076_112_2_18_2_24_31_104,"[MMIIKB] RestoreCircleTask end with error_code=%d,is_asy_ret=%d, has_restore_item=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3119_112_2_18_2_24_31_105,"[MMIIKB] RestoreResProcess param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3144_112_2_18_2_24_31_106,"[MMIIKB] RestoreResProcess send response to app.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3182_112_2_18_2_24_31_107,"[MMIIKB] RestoreItemDataSyn param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3230_112_2_18_2_24_31_108,"[MMIIKB] RestoreItemDataSynByBuf param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3243_112_2_18_2_24_31_109,"[MMIIKB] RestoreItemDataSynByBuf can't get file path.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3250_112_2_18_2_24_31_110,"[MMIIKB] RestoreItemDataSynByBuf file is not exist.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3291_112_2_18_2_24_31_111,"[MMIIKB] RestoreItemDataSynByBuf task null, type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3369_112_2_18_2_24_31_112,"[MMIIKB] RestoreItemDataSynByBuf send unitdata error, restore error by type=%d, error=%dtrans_idx=%d, begin=%d, len=%d")
TRACE_MSG(MMIIKEYBACKUP_TASK_3391_112_2_18_2_24_31_113,"[MMIIKB] BackupItemDataSynByBuf cancel by user.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3406_112_2_18_2_24_31_114,"[MMIIKB] RestoreItemDataSynByBuf end with type=%d,error_code=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3454_112_2_18_2_24_31_115,"[MMIIKB] RestoreItemDataAsy param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3463_112_2_18_2_24_31_116,"[MMIIKB] RestoreItemDataAsy task null, type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3476_112_2_18_2_24_31_117,"[MMIIKB] RestoreItemDataAsy can't get file path.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3483_112_2_18_2_24_31_118,"[MMIIKB] RestoreItemDataAsy file is not exist.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3542_112_2_18_2_24_31_119,"[MMIIKB] SendUnitDataCircle param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3624_112_2_18_2_24_32_120,"[MMIIKB] RestoreHandleItemUnitDataAsy param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3669_112_2_18_2_24_32_121,"[MMIIKB] RestoreHandleUnitDataAsyByBuf param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3683_112_2_18_2_24_32_122,"[MMIIKB] RestoreHandleUnitDataAsyByBuf can't get file path.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3690_112_2_18_2_24_32_123,"[MMIIKB] RestoreHandleUnitDataAsyByBuf file is not exist.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3705_112_2_18_2_24_32_124,"[MMIIKB] RestoreHandleUnitDataAsyByBuf this item is end,type=%d, error_code=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3775_112_2_18_2_24_32_125,"[MMIIKB] RestoreHandleUnitDataAsyByBuf this uint is end,type=%d, error_code=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3781_112_2_18_2_24_32_126,"[MMIIKB] RestoreHandleUnitDataAsyByBuf cancel by user.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3838_112_2_18_2_24_32_127,"[MMIIKB] IKBCancelAction param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3855_112_2_18_2_24_32_128,"[MMIIKB] IKBCancelAction memory is out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3892_112_2_18_2_24_32_129,"[MMIIKB] IKBCancelAction task_type=%d, error_code =%dn")
TRACE_MSG(MMIIKEYBACKUP_TASK_3907_112_2_18_2_24_32_130,"[MMIIKB] CreateProtectionTimer create timer, begin on=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3924_112_2_18_2_24_32_131,"[MMIIKB] StopProtectionTimer stop time, end on=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3945_112_2_18_2_24_32_132,"[MMIIKB] ProcessProtectionTimerCB time out, end on=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3951_112_2_18_2_24_32_133,"[MMIIKB] ProcessProtectionTimerCB backup time out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3956_112_2_18_2_24_32_134,"[MMIIKB] ProcessProtectionTimerCB restore time out.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3961_112_2_18_2_24_32_135,"[MMIIKB] ProcessProtectionTimerCB all task is unbusy.")
TRACE_MSG(MMIIKEYBACKUP_TASK_3988_112_2_18_2_24_32_136,"[MMIIKB] GetContentFilePath param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_4025_112_2_18_2_24_32_137,"[MMIIKB] ReceiveUnitData param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_4032_112_2_18_2_24_32_138,"[MMIIKB] ReceiveUnitData backup_call_back is null, type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_4071_112_2_18_2_24_32_139,"[MMIIKB] WriteUnitData param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_4166_112_2_18_2_24_33_140,"[MMIIKB] WriteContentHead param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_4221_112_2_18_2_24_33_141,"[MMIIKB] SendUnitData param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_4228_112_2_18_2_24_33_142,"[MMIIKB] SendUnitData restore_call_back is null, type=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_4264_112_2_18_2_24_33_143,"[MMIIKB] ReadUnitData param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_4311_112_2_18_2_24_33_144,"[MMIIKB] ReadUnitData offset=%d, error_code=%d.")
TRACE_MSG(MMIIKEYBACKUP_TASK_4336_112_2_18_2_24_33_145,"[MMIIKB] ReadContentHead param is null.")
TRACE_MSG(MMIIKEYBACKUP_TASK_4373_112_2_18_2_24_33_146,"[MMIIKB] ReadContentHead error_code=%d.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_995_112_2_18_2_24_36_147,"[MMIIKB] MMIAPIIKB_HandleAppSigRes: invalid param!")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1001_112_2_18_2_24_36_148,"[MMIIKB] MMIAPIIKB_HandleAppSigRes msg_id=%d, param=%d")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1279_112_2_18_2_24_36_149,"[MMIIKB] ShowTips: error_code=%d!")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1418_112_2_18_2_24_36_150,"[MMIIKB] InitListUIValue: param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1455_112_2_18_2_24_37_151,"[MMIIKB] GetListItemSelectValues: param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1491_112_2_18_2_24_37_152,"[MMIIKB] GetOneListItemValue: param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1551_112_2_18_2_24_37_153,"[MMIIKB] AppendALLListItemCtrl: param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1592_112_2_18_2_24_37_154,"[MMIIKB] OnClickCheckListItem: param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1638_112_2_18_2_24_37_155,"[MMIIKB] FormatTotalValue: param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1717_112_2_18_2_24_37_156,"[MMIIKB] GetFirstTypeTotalValue: param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1732_112_2_18_2_24_37_157,"[MMIIKB] GetFirstTypeTotalValue types=%d, first total=%d, type=%d")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1758_112_2_18_2_24_37_158,"[MMIIKB] UpdateProcessBar: param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1775_112_2_18_2_24_37_159,"[MMIIKB] UpdateProcessBar: win_id=%04x.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1887_112_2_18_2_24_37_160,"[MMIIKB] CancelCurAction param is 0, task_type=%d.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_1933_112_2_18_2_24_37_161,"[MMIIKB] HandleCancelActionQueryMsg param is 0, task_type=%d.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2016_112_2_18_2_24_38_162,"[MMIIKB] DeletePhysicalDir param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2023_112_2_18_2_24_38_163,"[MMIIKB] DeletePhysicalDir no memory.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2031_112_2_18_2_24_38_164,"[MMIIKB] DeletePhysicalDir no memory.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2269_112_2_18_2_24_38_165,"[MMIIKB] OptimizeBackupSelTypes types=%d, opt_types=%d")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2289_112_2_18_2_24_38_166,"[MMIIKB] PreCheckBackupItems param is null, types=%d.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2308_112_2_18_2_24_38_167,"[MMIIKB] PreCheckBackupItems types, opt_types=%d.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2345_112_2_18_2_24_38_168,"[MMIIKB] IsNeedInputPsd param is null, types=0.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2366_112_2_18_2_24_38_169,"[MMIIKB] BackupSelItems param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2371_112_2_18_2_24_38_170,"[MMIIKB] BackupSelItems types=%d.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2416_112_2_18_2_24_38_171,"[MMIIKB] StartBackupItems param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2467_112_2_18_2_24_39_172,"[MMIIKB] StartBackupItems send request, types=%d.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2744_112_2_18_2_24_39_173,"[MMIIKB] OnClickRestorePswSoftkey win_data is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_2804_112_2_18_2_24_39_174,"[MMIIKB] HandleEnterRestorePwd win_data is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_3052_112_2_18_2_24_40_175,"[MMIIKB] PreCheckRestoreItems types=%d.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_3070_112_2_18_2_24_40_176,"[MMIIKB] RestoreSelItems types=%d.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_3115_112_2_18_2_24_40_177,"[MMIIKB] GetPackPswInfo param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_3184_112_2_18_2_24_40_178,"[MMIIKB] StartRestoreItems send request, pack_types=%d,sel_types=%d, res_types=%d, index=%d")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_3526_112_2_18_2_24_41_179,"[MMIIKB] AppendPackListItem ctrl_id=%d, item_index=%d, types=%d, ptr=%d.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_3671_112_2_18_2_24_41_180,"[MMIIKB] GetPackContentNumber no memory.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_3679_112_2_18_2_24_41_181,"[MMIIKB] GetPackContentNumber no memory.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_3724_112_2_18_2_24_41_182,"[MMIIKB] InitRestoreSelItemList param is null.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_4022_112_2_18_2_24_42_183,"[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg menu_id=%x, item_index=%d")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_4033_112_2_18_2_24_42_184,"[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg no memory.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_4200_112_2_18_2_24_42_185,"[MMIIKB] HandleRestoreListOptMenuWinMsg menu_id=%x, item_index=%d")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_4216_112_2_18_2_24_42_186,"[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg no memory.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_4237_112_2_18_2_24_42_187,"[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg no memory.")
TRACE_MSG(MMIIKEYBACKUP_WINTAB_4417_112_2_18_2_24_43_188,"[MMIIKB] InitSettingListCtrl param is null.")
END_TRACE_MAP(MMI_APP_IKEYBACKUP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_IKEYBACKUP_TRC_H_


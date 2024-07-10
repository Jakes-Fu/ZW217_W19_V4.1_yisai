/******************************************************************************
 ** File Name:      mmi_app_fmm_trc.h                                         *
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
//trace_id:179
#ifndef _MMI_APP_FMM_TRC_H_
#define _MMI_APP_FMM_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI_FILEMGR_281_112_2_18_2_19_47_0 "MMIAPIFMM_SplitFullPath ERROR! full_path_len = %d"
#define MMI_FILEMGR_289_112_2_18_2_19_47_1 "MMIAPIFMM_SplitFullPath param ERROR PNULL == full_path_ptr! "
#define MMI_FILEMGR_295_112_2_18_2_19_47_2 "MMIAPIFMM_SplitFullPath param ERROR! "
#define MMI_FILEMGR_321_112_2_18_2_19_47_3 "MMIAPIFMM_SplitFullPath, ':' not found!"
#define MMI_FILEMGR_335_112_2_18_2_19_47_4 "MMIAPIFMM_SplitFullPath, '\' not found after device!"
#define MMI_FILEMGR_417_112_2_18_2_19_47_5 "MMIAPIFMM_SplitFullPathExt param ERROR!"
#define MMI_FILEMGR_438_112_2_18_2_19_47_6 "MMIAPIFMM_SplitFullPathExt ERROR! PNULL == suffix_len_ptr"
#define MMI_FILEMGR_465_112_2_18_2_19_47_7 "MMIAPIFMM_SplitFullPathExt ERROR! PNULL != name_ptr,PNULL == name_len_ptr"
#define MMI_FILEMGR_487_112_2_18_2_19_47_8 "MMIAPIFMM_SplitFullPathExt ERROR! PNULL != path_ptr,PNULL == path_len_ptr"
#define MMI_FILEMGR_520_112_2_18_2_19_48_9 "MMIAPIFMM_SplitFullPath ERROR! full_path_len = %d"
#define MMI_FILEMGR_528_112_2_18_2_19_48_10 "MMIFILE_FileIsInThisFolder param ERROR PNULL == full_path_ptr! "
#define MMI_FILEMGR_534_112_2_18_2_19_48_11 "MMIFILE_FileIsInThisFolder param ERROR! "
#define MMI_FILEMGR_590_112_2_18_2_19_48_12 "[MMIFMM]: MMIAPIFMM_CombineFullPathEx() param error,PNULL == abs_path_ptr!"
#define MMI_FILEMGR_646_112_2_18_2_19_48_13 "MMIAPIFMM_CombineFullPathEx ERROR! abs_path_total_len = %d,abs_path_len = %d"
#define MMI_FILEMGR_682_112_2_18_2_19_48_14 "MMIAPIFMM_CombineFullPath para error!"
#define MMI_FILEMGR_795_112_2_18_2_19_48_15 "MMIAPIFMM_CombineFullPath error! j = %d, full_path_len = %d"
#define MMI_FILEMGR_1046_112_2_18_2_19_49_16 "MMIAPIFMM_RegisterDevice param error !"
#define MMI_FILEMGR_1130_112_2_18_2_19_49_17 "MMIAPIFMM_UnRegisterDevice param ERROR!"
#define MMI_FILEMGR_1147_112_2_18_2_19_49_18 "MMIAPIFMM_UnRegisterDevice unregister device error"
#define MMI_FILEMGR_1186_112_2_18_2_19_49_19 "MMIAPIFMM_FormatDevice param ERROR!"
#define MMI_FILEMGR_1210_112_2_18_2_19_49_20 "MMIAPIFMM_FormatDevice ERROR! fs_format = %d"
#define MMI_FILEMGR_1277_112_2_18_2_19_49_21 "FormatDeviceAsynCallBack param error:PNULL == irp_param"
#define MMI_FILEMGR_1282_112_2_18_2_19_49_22 "MMIAPIFMM_FormatDeviceAsyn: format device callback, error = %d"
#define MMI_FILEMGR_1333_112_2_18_2_19_49_23 "MMIAPIFMM_FormatDeviceAsyn param ERROR!"
#define MMI_FILEMGR_1355_112_2_18_2_19_49_24 "MMIFMM:MMIAPIFMM_FormatDeviceAsyn format = %d is error!"
#define MMI_FILEMGR_1389_112_2_18_2_19_49_25 "MMIFMM:MMIAPIFMM_FormatDeviceAsyn PNULL == callback_ptr is error!"
#define MMI_FILEMGR_1468_112_2_18_2_19_49_26 "MMIAPIFMM_GetDeviceStatus param ERROR!"
#define MMI_FILEMGR_1522_112_2_18_2_19_50_27 "MMIAPIFMM_CreateDir param ERROR!"
#define MMI_FILEMGR_1528_112_2_18_2_19_50_28 "MMIAPIFMM_CreateDir: create dir success"
#define MMI_FILEMGR_1533_112_2_18_2_19_50_29 "MMIAPIFMM_CreateDir: create dir fail"
#define MMI_FILEMGR_1568_112_2_18_2_19_50_30 "[MMIFILE] SetAttr NO memory !"
#define MMI_FILEMGR_1620_112_2_18_2_19_50_31 "MMIAPIFMM_GetFileInfo ERROR:PNULL == full_path_ptr"
#define MMI_FILEMGR_1700_112_2_18_2_19_50_32 "MMIAPIFMM_IsFileExist error:PNULL == full_path_ptr!"
#define MMI_FILEMGR_1721_112_2_18_2_19_50_33 "MMIFMM_IsFilesInFolder: result = %d"
#define MMI_FILEMGR_1748_112_2_18_2_19_50_34 "MMIAPIFMM_GetDeviceFreeSpace param error!"
#define MMI_FILEMGR_1781_112_2_18_2_19_50_35 "MMIAPIFMM_GetDeviceUsedSpace error!"
#define MMI_FILEMGR_1819_112_2_18_2_19_50_36 "MMIAPIFMM_RenameFile full_path_len %d new_file_name_len %d !!"
#define MMI_FILEMGR_1907_112_2_18_2_19_50_37 "StoreFindFileData param error!"
#define MMI_FILEMGR_1964_112_2_18_2_19_50_38 "MMIFMM_FindAndGetFiles param error!"
#define MMI_FILEMGR_1978_112_2_18_2_19_50_39 "MMIFMM_FindAndGetFiles: find first success"
#define MMI_FILEMGR_2016_112_2_18_2_19_51_40 "MMIFMM_FindAndGetFiles: find file number = %d"
#define MMI_FILEMGR_2042_112_2_18_2_19_51_41 "MMIAPIFMM_ReadFilesDataSyn param error!"
#define MMI_FILEMGR_2048_112_2_18_2_19_51_42 "MMIAPIFMM_ReadFilesDataSyn: read file 0"
#define MMI_FILEMGR_2066_112_2_18_2_19_51_43 "MMIAPIFMM_ReadFilesDataSyn: return success"
#define MMI_FILEMGR_2070_112_2_18_2_19_51_44 "MMIAPIFMM_ReadFilesDataSyn: return fail"
#define MMI_FILEMGR_2097_112_2_18_2_19_51_45 "MMIAPIFMM_ReadFilesDataAsynEx: read file 0"
#define MMI_FILEMGR_2113_112_2_18_2_19_51_46 "MMIAPIFMM_ReadFilesDataAsynEx: return success"
#define MMI_FILEMGR_2117_112_2_18_2_19_51_47 "MMIAPIFMM_ReadFilesDataAsynEx: return fail"
#define MMI_FILEMGR_2144_112_2_18_2_19_51_48 "MMIAPIFMM_WriteFilesDataSyn param error!"
#define MMI_FILEMGR_2179_112_2_18_2_19_51_49 "MMIAPIFMM_DeleteFileSyn param error!"
#define MMI_FILEMGR_2205_112_2_18_2_19_51_50 "[MMIFMM] MMIAPIFMM_CopyFile invalid file name !"
#define MMI_FILEMGR_2212_112_2_18_2_19_51_51 "[MMIFMM] MMIAPIFMM_CopyFile no memory!"
#define MMI_FILEMGR_2226_112_2_18_2_19_51_52 "[MMIFMM] MMIAPIFMM_CopyFile open file error !"
#define MMI_FILEMGR_2240_112_2_18_2_19_51_53 "[MMIFILE] MMIAPIFMM_CopyFile MMIAPIFMM_ReadFile copy_ret %d"
#define MMI_FILEMGR_2246_112_2_18_2_19_51_54 "[MMIFILE] MMIAPIFMM_CopyFile MMIAPIFMM_WriteFile copy_ret %d"
#define MMI_FILEMGR_2253_112_2_18_2_19_51_55 "[MMIFILE] MMIAPIFMM_CopyFile copy_ret %d"
#define MMI_FILEMGR_2277_112_2_18_2_19_51_56 "MMIAPIFMM_ReadFileDataByOffsetSyn param error!"
#define MMI_FILEMGR_2282_112_2_18_2_19_51_57 "MMIAPIFMM_ReadFileDataByOffsetSyn: read file 0"
#define MMI_FILEMGR_2303_112_2_18_2_19_51_58 "MMIAPIFMM_ReadFileDataByOffsetSyn: read file data success"
#define MMI_FILEMGR_2307_112_2_18_2_19_51_59 "MMIAPIFMM_ReadFileDataByOffsetSyn: read file data fail"
#define MMI_FILEMGR_2335_112_2_18_2_19_51_60 "MMIFMM_ReadFileDataByOffsetSynEx: read file 0"
#define MMI_FILEMGR_2354_112_2_18_2_19_51_61 "MMIFMM_ReadFileDataByOffsetSynEx: read file data success"
#define MMI_FILEMGR_2358_112_2_18_2_19_51_62 "MMIFMM_ReadFileDataByOffsetSynEx: read file data fail"
#define MMI_FILEMGR_2386_112_2_18_2_19_51_63 "MMIFILE_WriteFileDataByOffsetSyn param error!"
#define MMI_FILEMGR_2407_112_2_18_2_19_51_64 "MMIFILE_WriteFileDataByOffsetSyn: write file data success"
#define MMI_FILEMGR_2411_112_2_18_2_19_51_65 "MMIFILE_WriteFileDataByOffsetSyn: write file data fail"
#define MMI_FILEMGR_2434_112_2_18_2_19_51_66 "ReadFileDataAsynCallback param error:PNULL == irp_param"
#define MMI_FILEMGR_2478_112_2_18_2_19_51_67 "MMIAPIFMM_ReadFilesDataAsyn param error!"
#define MMI_FILEMGR_2483_112_2_18_2_19_51_68 "MMIAPIFMM_ReadFilesDataAsyn param error: win_id = %d,ctrl_id = %d,msg_id = %d"
#define MMI_FILEMGR_2503_112_2_18_2_19_52_69 "MMIAPIFMM_ReadFilesDataAsyn: read file 0"
#define MMI_FILEMGR_2515_112_2_18_2_19_52_70 "MMIFMM:MMIAPIFMM_ReadFilesDataAsyn PNULL == callback_ptr is error!"
#define MMI_FILEMGR_2529_112_2_18_2_19_52_71 "MMIAPIFMM_ReadFilesDataAsyn: read file success"
#define MMI_FILEMGR_2534_112_2_18_2_19_52_72 "MMIAPIFMM_ReadFilesDataAsyn: read file fail"
#define MMI_FILEMGR_2600_112_2_18_2_19_52_73 "MMIAPIFMM_ReadFilesDataAsynEx: read file 0"
#define MMI_FILEMGR_2619_112_2_18_2_19_52_74 "MMIAPIFMM_ReadFilesDataAsynEx: read file success"
#define MMI_FILEMGR_2624_112_2_18_2_19_52_75 "MMIAPIFMM_ReadFilesDataAsynEx: read file fail"
#define MMI_FILEMGR_2649_112_2_18_2_19_52_76 "WriteFileDataAsynCallback param error:PNULL == irp_param"
#define MMI_FILEMGR_2652_112_2_18_2_19_52_77 "WriteFileDataAsynCallback: sfs error = %d"
#define MMI_FILEMGR_2693_112_2_18_2_19_52_78 "MMIAPIFMM_WriteFilesDataAsyn param error!"
#define MMI_FILEMGR_2698_112_2_18_2_19_52_79 "MMIAPIFMM_WriteFilesDataAsyn param error: win_id = %d,msg_id = %d"
#define MMI_FILEMGR_2710_112_2_18_2_19_52_80 "MMIAPIFMM_WriteFilesDataAsyn PNULL == callback_ptr is error!"
#define MMI_FILEMGR_2723_112_2_18_2_19_52_81 "MMIAPIFMM_WriteFilesDataAsyn: write file success"
#define MMI_FILEMGR_2728_112_2_18_2_19_52_82 "MMIAPIFMM_WriteFilesDataAsyn: write file fail"
#define MMI_FILEMGR_2735_112_2_18_2_19_52_83 "MMIAPIFMM_WriteFilesDataAsyn: create file fail"
#define MMI_FILEMGR_2757_112_2_18_2_19_52_84 "ReadFileDataByOffsetAsynCallback param error:PNULL == irp_param"
#define MMI_FILEMGR_2800_112_2_18_2_19_52_85 "MMIFILE_ReadFileDataByOffsetAsyn param error!"
#define MMI_FILEMGR_2805_112_2_18_2_19_52_86 "MMIFILE_ReadFileDataByOffsetAsyn param error:win_id = %d,msg_id = %d"
#define MMI_FILEMGR_2814_112_2_18_2_19_52_87 "MMIFILE_ReadFileDataByOffsetAsyn: read file 0"
#define MMI_FILEMGR_2828_112_2_18_2_19_52_88 "MMIFILE_ReadFileDataByOffsetAsyn PNULL == callback_ptr is error!"
#define MMI_FILEMGR_2857_112_2_18_2_19_52_89 "MMIFILE_ReadFileDataByOffsetAsyn: return success"
#define MMI_FILEMGR_2861_112_2_18_2_19_52_90 "MMIFILE_ReadFileDataByOffsetAsyn: return fail"
#define MMI_FILEMGR_2910_112_2_18_2_19_52_91 "MMIFILE_ReadFileDataByOffsetAsynEx: read file 0"
#define MMI_FILEMGR_2943_112_2_18_2_19_52_92 "MMIFILE_ReadFileDataByOffsetAsynEx: return success"
#define MMI_FILEMGR_2947_112_2_18_2_19_52_93 "MMIFILE_ReadFileDataByOffsetAsynEx: return fail"
#define MMI_FILEMGR_2969_112_2_18_2_19_52_94 "WriteFileDataByOffsetAsynCallback param error:PNULL == irp_param"
#define MMI_FILEMGR_2973_112_2_18_2_19_52_95 "WriteFileDataAsynCallback: sfs error = %d"
#define MMI_FILEMGR_3014_112_2_18_2_19_53_96 "MMIAPIFMM_WriteFileDataByOffsetAsyn param error!"
#define MMI_FILEMGR_3019_112_2_18_2_19_53_97 "MMIAPIFMM_WriteFileDataByOffsetAsyn param error: win_id = %d,msg_id = %d"
#define MMI_FILEMGR_3033_112_2_18_2_19_53_98 "[MMIFMM]:MMIAPIFMM_WriteFileDataByOffsetAsyn PNULL == callback_ptr error!"
#define MMI_FILEMGR_3062_112_2_18_2_19_53_99 "MMIAPIFMM_WriteFileDataByOffsetAsyn: return success"
#define MMI_FILEMGR_3066_112_2_18_2_19_53_100 "MMIAPIFMM_WriteFileDataByOffsetAsyn: return fail"
#define MMI_FILEMGR_3089_112_2_18_2_19_53_101 "MMIAPIFMM_OperationAsynCnf param error: PNULL == param_ptr"
#define MMI_FILEMGR_3114_112_2_18_2_19_53_102 "MMIAPIFMM_OperationAsynCnf: result = %d"
#define MMI_FILEMGR_3136_112_2_18_2_19_53_103 "DeleteFileAsynCallback param error:PNULL == irp_param"
#define MMI_FILEMGR_3141_112_2_18_2_19_53_104 "DeleteFileAsynCallback: error = %d"
#define MMI_FILEMGR_3175_112_2_18_2_19_53_105 "CreateFileAsynCallback param error:PNULL == irp_param"
#define MMI_FILEMGR_3181_112_2_18_2_19_53_106 "CreateFileAsynCallback: error = %d"
#define MMI_FILEMGR_3217_112_2_18_2_19_53_107 "MMIAPIFMM_DeleteFileAsyn param error!"
#define MMI_FILEMGR_3222_112_2_18_2_19_53_108 "MMIAPIFMM_DeleteFileAsyn param error: win_id = %d,msg_id = %d"
#define MMI_FILEMGR_3236_112_2_18_2_19_53_109 "MMIFMM:MMIAPIFMM_DeleteFileAsyn PNULL == callback_ptr error!"
#define MMI_FILEMGR_3249_112_2_18_2_19_53_110 "MMIAPIFMM_DeleteFileAsyn: delete asyn success"
#define MMI_FILEMGR_3283_112_2_18_2_19_53_111 "MMIAPIFMM_CopyOneFile param error!"
#define MMI_FILEMGR_3304_112_2_18_2_19_53_112 "MMIFMM:MMIAPIFMM_CopyOneFile PNULL == s_copy_info.buf_ptr is error!"
#define MMI_FILEMGR_3320_112_2_18_2_19_53_113 "MMIAPIFMM_CopyOneFile: create src file and dest file success"
#define MMI_FILEMGR_3336_112_2_18_2_19_53_114 "MMIAPIFMM_CopyOneFile: create src file and dest file fail"
#define MMI_FILEMGR_3404_112_2_18_2_19_53_115 "MMIAPIFMM_CopyReadSrc: copy one file finished with success"
#define MMI_FILEMGR_3408_112_2_18_2_19_53_116 "MMIAPIFMM_CopyReadSrc: have copied data size = %d, src file size = %d"
#define MMI_FILEMGR_3431_112_2_18_2_19_53_117 "MMIAPIFMM_CopyReadSrc:  fail"
#define MMI_FILEMGR_3499_112_2_18_2_19_53_118 "MMIAPIFMM_CopyWriteDest: read src file success, data size = %d"
#define MMI_FILEMGR_3515_112_2_18_2_19_54_119 "MMIAPIFMM_CopyWriteDest:  fail"
#define MMI_FILEMGR_3576_112_2_18_2_19_54_120 "MMIAPIFMM_DeleteTree pathName 0x%x , fileName 0x%x"
#define MMI_FILEMGR_3687_112_2_18_2_19_54_121 "MMIAPIFMM_CreateFileAsyn param error: PNULL == file_name"
#define MMI_FILEMGR_3709_112_2_18_2_19_54_122 "MMIFMM:MMIAPIFMM_CreateFileAsyn PNULL == callback_ptr is error!"
#define MMI_FILEMGR_3757_112_2_18_2_19_54_123 "MMIAPIFMM_CreateDirectory param error:PNULL == path"
#define MMI_FILEMGR_3838_112_2_18_2_19_54_124 "MMIFILE_SetFileSize param error: PNULL == file_handle"
#define MMI_FILEMGR_3892_112_2_18_2_19_54_125 "MMIAPIFMM_SetFilePointer param error:PNULL == file_handle"
#define MMI_FILEMGR_3919_112_2_18_2_19_54_126 "MMIAPIFMM_GetFilePointer param error:PNULL == file_handle"
#define MMI_FILEMGR_3976_112_2_18_2_19_54_127 "mmi_filemgr.c, MMIAPIFMM_IsEndOfFile(), cur_pos = %X, file_total_size = %X"
#define MMI_FILEMGR_4033_112_2_18_2_19_55_128 "[MMI FILE]: MMIAPIFMM_GetValidateDevice, ret_dev %x"
#define MMI_FILEMGR_4043_112_2_18_2_19_55_129 "[MMIFILE]: MMIAPIFMM_Callback result = %d"
#define MMI_FILEMGR_4094_112_2_18_2_19_55_130 "[mmi_filemgr.c]:MMIAPIFMM_GetDefaultDisk dev_type=%d"
#define MMI_FILEMGR_4143_112_2_18_2_19_55_131 "[mmi_filemgr.c]:GetAvailableDevice available device=%d"
#define MMI_FILEMGR_4173_112_2_18_2_19_55_132 "[mmi_filemgr.c]:MMIAPIFMM_IsEnoughSpace is not enough space!"
#define MMI_FILEMGR_4471_112_2_18_2_19_55_133 "[mmi_filemgr.c]:MMIAPIFMM_CreateSysFileDir sysfile folder not exist!!"
#define MMI_FILEMGR_4478_112_2_18_2_19_55_134 "MMIAPIFMM_CreateSysFileDir: create dir fail"
#define MMI_FILEMGR_4594_112_2_18_2_19_56_135 "[MMIFILE] MMIAPIFMM_AppendFile file name invalid !"
#define MMI_FILEMGR_4820_112_2_18_2_19_56_136 "[MMIFILE] MMIAPIFMM_GetSuitableFileDev user_sel_dev %d min_free_space %d return dev %d file_ret %d"
#define MMI_FILEMGR_5194_112_2_18_2_19_57_137 "MMIFILE_CreateFile param error: PNULL == file_name"
#define MMI_FILEMGR_5230_112_2_18_2_19_57_138 "MMIAPIFMM_FindNextFile param error!"
#define MMI_FILEMGR_5253_112_2_18_2_19_57_139 "MMIAPIFMM_DeleteFile param error:PNULL == name"
#define MMI_FILEMGR_5278_112_2_18_2_19_57_140 "MMIAPIFMM_DeleteDirectory param error:PNULL == path"
#define MMI_FILEMGR_5303_112_2_18_2_19_57_141 "MMIAPIFMM_FindFirstFile param error!"
#define MMI_FILEMGR_5312_112_2_18_2_19_57_142 "MMIAPIFMM_FindFirstFile find_len = %d"
#define MMI_FILEMGR_5348_112_2_18_2_19_57_143 "MMIAPIFMM_GetFileSize param error: PNULL == file_handle"
#define MMI_FILEMGR_5379_112_2_18_2_19_57_144 "MMIAPIFMM_ReadFile param error!"
#define MMI_FILEMGR_5414_112_2_18_2_19_57_145 "MMIAPIFMM_WriteFile param error!"
#define MMI_FILEMGR_5445_112_2_18_2_19_57_146 "MMIAPIFMM_CloseFile file_handle %d !"
#define MMI_FILEMGR_5467_112_2_18_2_19_57_147 "MMIAPIFMM_IsFileExist error:PNULL == full_path_ptr!"
#define MMI_FILEMGR_5480_112_2_18_2_19_57_148 "MMIAPIFMM_IsFileExist: find first sfs_handle = %d"
#define MMI_FILEMGR_5493_112_2_18_2_19_57_149 "MMIAPIFMM_IsFileExist: result = %d"
#define MMI_FILEMGR_5516_112_2_18_2_19_57_150 "MMIAPIFMM_IsFolderExist error:PNULL == full_path_ptr!"
#define MMI_FILEMGR_5540_112_2_18_2_19_58_151 "MMIAPIFMM_IsFolderExist: find first sfs_handle = %d"
#define MMI_FILEMGR_5553_112_2_18_2_19_58_152 "MMIAPIFMM_IsFolderExist: result = %d"
#define MMI_FILETASK_179_112_2_18_2_19_58_153 "APPFILE_CreateTask, file task hasn't been closed!"
#define MMI_FILETASK_195_112_2_18_2_19_58_154 "APPFILE_CreateTask, g_app_file_task_id = %d"
#define MMI_FILETASK_204_112_2_18_2_19_58_155 "APPFILE_HandleSearchFileCnf search_result=%d,s_is_stopfindfile=%d"
#define MMI_FILETASK_288_112_2_18_2_19_58_156 "APP_COPY_Task: SigCode unknown"
#define MMI_FILETASK_317_112_2_18_2_19_58_157 "[MMIFILE] APPFILE_AbortCopy "
#define MMI_FILETASK_330_112_2_18_2_19_58_158 "[MMIFILE] APPFILE_CopyClose "
#define MMI_FILETASK_342_112_2_18_2_19_58_159 "[MMIFILE] MMIFILE_DeleteOpen enter"
#define MMI_FILETASK_345_112_2_18_2_19_58_160 "MMIFILE MMIFILE_DeleteOpen del_file_name 0x&x del_name_len %d !"
#define MMI_FILETASK_353_112_2_18_2_19_58_161 "MMIFILE MMIFILE_DeleteOpen del_handle_ptr 0x%x !"
#define MMI_FILETASK_384_112_2_18_2_19_58_162 "[MMIFILE] MMIFILE_DeleteClose fs_ret %d"
#define MMI_FILETASK_394_112_2_18_2_19_58_163 "[MMIFILE] MMIFILE_AbortDelete enter"
#define MMI_FILETASK_398_112_2_18_2_19_59_164 "MMIFILE_AbortDelete del_handle_ptr 0x&x !"
#define MMI_FILETASK_451_112_2_18_2_19_59_165 "[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_FILES enter"
#define MMI_FILETASK_477_112_2_18_2_19_59_166 "[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_ONE_FILE_IND user stop %d"
#define MMI_FILETASK_549_112_2_18_2_19_59_167 "[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_ONE_FOLDER_IND user stop %d"
#define MMI_FILETASK_585_112_2_18_2_19_59_168 "[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_STOP user stop %d"
#define MMI_FILETASK_633_112_2_18_2_19_59_169 "APPFILE_HandleCopySig param error:PNULL == psig"
#define MMI_FILETASK_1000_112_2_18_2_20_0_170 "APP_COPY_Task: SigCode unknown"
#define MMI_FILETASK_1077_112_2_18_2_20_0_171 "[MMIFILE] MMIFILE_DeleteDirTree enter "
#define MMI_FILETASK_1158_112_2_18_2_20_0_172 "[MMIFILE] MMIFILE_DeleteSubFileAsyn enter "
#define MMI_FILETASK_1162_112_2_18_2_20_0_173 "[MMIFILE] MMIFILE_DeleteAsyn del_handle_ptr 0x%x file_name 0x%x file_name_len %d !"
#define MMI_FILETASK_1212_112_2_18_2_20_0_174 "MMIFILE_DeleteSubFileAsyn: delete asyn success"
#define MMI_FILETASK_1224_112_2_18_2_20_0_175 "MMIFILE_DeleteSubFileAsyn file name too long !"
#define MMI_FILETASK_1263_112_2_18_2_20_0_176 "[MMIFILE] MMIFILE_DeleteSubFolder enter "
#define MMI_FILETASK_1266_112_2_18_2_20_0_177 "[MMIFILE] MMIFILE_DeleteAsyn del_handle_ptr 0x%x !"
#define MMI_FILETASK_1276_112_2_18_2_20_0_178 "MMIFILE_DeleteSubFolder alloc find_data_ptr error:PNULL == find_data_ptr"
#define MMI_FILETASK_1353_112_2_18_2_20_0_179 "[MMIFILE]FileDeleteCallback error %d"
#define MMI_FILETASK_1416_112_2_18_2_20_1_180 "[MMIFILE] FindOnlyFiles enter "
#define MMI_FILETASK_1473_112_2_18_2_20_1_181 "[MMIFILE] FindOnlyFolders enter "
#define MMI_FILETASK_1555_112_2_18_2_20_1_182 "[MMIFILE] MMIFILE_CopyOpen enter"
#define MMI_FILETASK_1558_112_2_18_2_20_1_183 "MMIFILE MMIFILE_CopyOpen src_file_name 0x&x src_name_len %d !"
#define MMI_FILETASK_1564_112_2_18_2_20_1_184 "MMIFILE MMIFILE_CopyOpen dest_file_name 0x&x dest_file_name_len %d !"
#define MMI_FILETASK_1571_112_2_18_2_20_1_185 "MMIFILE MMIFILE_CopyOpen handle_ptr 0x%x !"
#define MMI_FILETASK_1736_112_2_18_2_20_1_186 "[MMIFILE] FileCopy invalid file name !"
#define MMI_FILETASK_1785_112_2_18_2_20_1_187 "[MMIFILE] FileCopy no memory!"
#define MMI_FILETASK_1831_112_2_18_2_20_1_188 "[MMIFILE] FileCopy open file error !"
#define MMI_FILETASK_1925_112_2_18_2_20_2_189 "[MMIFILE] FileCopy copy_ret %d handle_ptr->copy_offset %d filesize %d"
#define MMI_FILETASK_1946_112_2_18_2_20_2_190 "[[MMIFMM]: MMIFILE_MoveOpen enter"
#define MMI_FILETASK_1949_112_2_18_2_20_2_191 "MMIFILE MMIFILE_MoveOpen src_file_name 0x&x src_filename 0x&x !"
#define MMI_FILETASK_1955_112_2_18_2_20_2_192 "[MMIFMM]: MMIFILE_MoveOpen dest_file_name 0x&x dest_filename 0x&x !"
#define MMI_FILETASK_1962_112_2_18_2_20_2_193 "[MMIFMM]: MMIFILE_MoveOpen handle_ptr 0x%x !"
#define MMI_FILETASK_1974_112_2_18_2_20_2_194 "[MMIFMM]: MMIFILE_MoveOpen param error"
#define MMI_FILETASK_2055_112_2_18_2_20_2_195 "[MMIFMM]:APPFILE_HandleMoveSig para error!"
#define MMI_FILETASK_2062_112_2_18_2_20_2_196 "[MMIFMM]:APPFILE_HandleMoveSig PNULL == move_handle_ptr!"
#define MMI_FILETASK_2096_112_2_18_2_20_2_197 "[MMIFMM]:filename too long src_path_len + src_file_len + 1 = %d"
#define MMI_FILETASK_2147_112_2_18_2_20_2_198 "[MMIFMM]: APPFILE_CopyClose "
#define MMI_FILETASK_2168_112_2_18_2_20_2_199 "[MMIFMM]:src_filename_len + dest_full_path_len = %d"
#define MMI_FILETASK_2196_112_2_18_2_20_2_200 "[MMIFMM]: APPFILE_AbortMove "
#define MMI_FILETASK_2199_112_2_18_2_20_2_201 "[MMIFMM]: APPFILE_AbortMove PNULL == handle_ptr"
#define MMIFILEARRAY_112_112_2_18_2_20_3_202 "MMIAPIFILEARRAY_Create, file_array = %x"
#define MMIFILEARRAY_133_112_2_18_2_20_3_203 "MMIAPIFILEARRAY_Destroy, *file_array_ptr = %x"
#define MMIFILEARRAY_197_112_2_18_2_20_3_204 "[FMM] MMIAPIFILEARRAY_Read data_ptr->name_len %d !"
#define MMIFILEARRAY_224_112_2_18_2_20_3_205 "[FMM] MMIAPIFILEARRAY_Read data_ptr->name_len %d !"
#define MMIFILEARRAY_628_112_2_18_2_20_4_206 "CompareNameAsce param error!"
#define MMIFILEARRAY_667_112_2_18_2_20_4_207 "CompareNameDesc param error!"
#define MMIFILEARRAY_778_112_2_18_2_20_4_208 "CompareTypeAsce param error!"
#define MMIFILEARRAY_818_112_2_18_2_20_4_209 "CompareTypeDesc param error!"
#define MMIFILEARRAY_854_112_2_18_2_20_4_210 "CompareTimeAsec param error!"
#define MMIFILEARRAY_921_112_2_18_2_20_4_211 "CompareSizeAsec param error!"
#define MMIFMM_746_112_2_18_2_20_7_212 "MMIAPIFMM_CompareFileName param error!"
#define MMIFMM_779_112_2_18_2_20_7_213 "MMIAPIFMM_CompareFileSize param error!"
#define MMIFMM_818_112_2_18_2_20_7_214 "MMIAPIFMM_CompareFileTime param error!"
#define MMIFMM_859_112_2_18_2_20_7_215 "MMIAPIFMM_CompareFileType param error!"
#define MMIFMM_983_112_2_18_2_20_7_216 "MMIAPIFMM_GetCurrentPathFile param error!"
#define MMIFMM_997_112_2_18_2_20_7_217 "MMIFMM: MMIFMM_GetCurrentPathFile, allocate find_ptr failed!"
#define MMIFMM_1008_112_2_18_2_20_7_218 "MMIFMM: MMIFMM_GetCurrentPathFile, allocate full_file_path_ptr failed!"
#define MMIFMM_1637_112_2_18_2_20_9_219 "MMIFMM: MMIAPIFMM_GetFolderType, path len = %d"
#define MMIFMM_1642_112_2_18_2_20_9_220 "MMIFMM:MMIAPIFMM_GetFolderType PNULL == path_ptr is error!"
#define MMIFMM_1648_112_2_18_2_20_9_221 "MMIFMM:MMIAPIFMM_GetFolderType path_len = %d"
#define MMIFMM_1732_112_2_18_2_20_9_222 "MMIFMM: MMIAPIFMM_FileDataSort, sort =%d."
#define MMIFMM_1736_112_2_18_2_20_9_223 "MMIFMM: MMIAPIFMM_FileDataSort, param error:PNULL == list_data_ptr"
#define MMIFMM_1778_112_2_18_2_20_9_224 "MMIAPIFMM_FileDataSort sort = %d"
#define MMIFMM_1804_112_2_18_2_20_9_225 "MMIFMM_GetFileInfo param error:PNULL == full_path_ptr"
#define MMIFMM_1851_112_2_18_2_20_9_226 "MMIFMM_DeleteFileSyn param error!"
#define MMIFMM_1953_112_2_18_2_20_10_227 "FMM: can't malloc memory "
#define MMIFMM_2139_112_2_18_2_20_10_228 "MMIAPIFMM_IsDevEnoughSpace: free spaced high value is = %d free space low value is = %d"
#define MMIFMM_2243_112_2_18_2_20_10_229 "[MMIFMM]:SetCommonCharacter,param error:PNULL == p_item"
#define MMIFMM_2379_112_2_18_2_20_10_230 "[MMIFMM]: tag_code_type=%d"
#define MMIFMM_2384_112_2_18_2_20_10_231 "[MMIFMM]: SONG title DETAIL!"
#define MMIFMM_2406_112_2_18_2_20_10_232 "[MMIFMM]: artist artist DETAIL!"
#define MMIFMM_2512_112_2_18_2_20_11_233 "AddOneDetailItem fail "
#define MMIFMM_2538_112_2_18_2_20_11_234 "MMIAPIFMM_GetPictureWidthHeight param error!"
#define MMIFMM_2727_112_2_18_2_20_11_235 "OpenSelectMusicWinCbk param error:PNULL == file_data_ptr"
#define MMIFMM_3448_112_2_18_2_20_12_236 "MMIAPIFMM_CloseSelectMusicWin, exit"
#define MMIFMM_3458_112_2_18_2_20_12_237 "MMIAPIFMM_CloseSelectFileWin, exit"
#define MMIFMM_3498_112_2_18_2_20_13_238 "OpenSelectMovWinCbk param error:PNULL == file_data_ptr"
#define MMIFMM_3523_112_2_18_2_20_13_239 "OpenSelectMovWinCbk, supported_type = %d"
#define MMIFMM_3545_112_2_18_2_20_13_240 "OpenSelectMovWinMMSCbk param error:PNULL == file_data_ptr"
#define MMIFMM_3857_112_2_18_2_20_13_241 "MMIAPIFMM_CloseSelectMovieWin, exit"
#define MMIFMM_4131_112_2_18_2_20_14_242 "OpenSelectAllFileWinCbk param error:PNULL == file_data_ptr"
#define MMIFMM_4162_112_2_18_2_20_14_243 "OpenSelectPicWinCbk param error:PNULL == file_data_ptr"
#define MMIFMM_4471_112_2_18_2_20_14_244 "InitMultimFolder param error!"
#define MMIFMM_4641_112_2_18_2_20_15_245 "MMIAPIFMM_CloseWaitSetWallpaper: reset previous wallpaper error!"
#define MMIFMM_4676_112_2_18_2_20_15_246 "MMIAPIFMM_CloseWaitSetWallpaper:wallpaper_type %d is error!"
#define MMIFMM_4715_112_2_18_2_20_15_247 "MMIAPIFMM_CloseWaitSetWallpaper:wallpaper_type %d is error!"
#define MMIFMM_4772_112_2_18_2_20_15_248 "MMIAPIFMM_ChangeWallPaperFileName param error!"
#define MMIFMM_4815_112_2_18_2_20_15_249 "MMIAPIFMM_IsWallPaperFile param error!"
#define MMIFMM_4849_112_2_18_2_20_15_250 "MMIAPIFMM_SetPictureToWallpaper param error!"
#define MMIFMM_4906_112_2_18_2_20_15_251 "MMIAPIFMM_SetPictureToWallpaper:get file info fail!"
#define MMIFMM_4950_112_2_18_2_20_15_252 "MMIAPIFMM_SetPictureToWallpaperEx full_path_ptr is PNULL !!!"
#define MMIFMM_4955_112_2_18_2_20_15_253 "MMIAPIFMM_SetPictureToWallpaperEx full_path_len is PNULL !!!"
#define MMIFMM_5015_112_2_18_2_20_16_254 "MMIAPIFMM_SetMusicToRing param error: PNULL == full_name_ptr"
#define MMIFMM_5119_112_2_18_2_20_16_255 "MMIAPIFMM_SetMusicToRing: create audio file handle error!"
#define MMIFMM_5157_112_2_18_2_20_16_256 "MMIAPIFMM_SetFileNameWinTitle param error!"
#define MMIFMM_5244_112_2_18_2_20_16_257 "MMIAPIFMM_SetRandomNameEditbox param error:PNULL == prefix_ptr"
#define MMIFMM_5277_112_2_18_2_20_16_258 "MMIFMM_GetOpenFileWinTickedFile param error!"
#define MMIFMM_5400_112_2_18_2_20_16_259 "MMIFMM: MMIAPIFMM_SetFolderDetail, detail_data =%x"
#define MMIFMM_5441_112_2_18_2_20_16_260 "MMIFMM: HandleFolderDetailWaitWinMsg, win_id =%d, msg_id = 0x%x."
#define MMIFMM_5527_112_2_18_2_20_17_261 "MMIFMM: MMIAPIFMM_SetFolderDetailCNF, detail_data =%x"
#define MMIFMM_5896_112_2_18_2_20_17_262 "[MMIDRM] MMIFMM_GetSuffixTypeInfo: invalid param!"
#define MMIFMM_5906_112_2_18_2_20_17_263 "[MMIDRM] MMIFMM_GetSuffixTypeInfo: user set all file type is %x!"
#define MMIFMM_5926_112_2_18_2_20_17_264 "[MMIDRM] MMIFMM_GetSuffixTypeInfo: user set file type is %x!"
#define MMIFMM_6033_112_2_18_2_20_18_265 "[MMIDRM] MMIAPIFMM_SetSelDrmLimitValue: invalid param!"
#define MMIFMM_6047_112_2_18_2_20_18_266 "[MMIDRM] MMIAPIFMM_SetSelDrmLimitValue: cons_mode_limit = %x, method_level_limit = %x, is_rights_valid=%d!"
#define MMIFMM_6074_112_2_18_2_20_18_267 "[MMIDRM] IsSelDrmConstraintFile: invalid param!"
#define MMIFMM_6171_112_2_18_2_20_18_268 "[MMIDRM] IsSelDrmConstraintFile: no any limit value!"
#define MMIFMM_INTERFACE_730_112_2_18_2_20_20_269 "MMIFMM_GetXFindCtlStr: xfind_param = %d,ctlstr=%d,ctlstr_len=%d"
#define MMIFMM_INTERFACE_828_112_2_18_2_20_20_270 "MMIXFIND XFindRoutineCallback: inParam = %d,status = %d,outParam1 = %d,outParam2 = %d,"
#define MMIFMM_INTERFACE_839_112_2_18_2_20_20_271 "MMIXFIND XFindRoutineCallback: SFS_X_STOP"
#define MMIFMM_INTERFACE_857_112_2_18_2_20_20_272 "MMIXFIND HandleFindCallback"
#define MMIFMM_INTERFACE_860_112_2_18_2_20_20_273 "MMIXFIND HandleFindCallback PNULL == xfind_info!"
#define MMIFMM_INTERFACE_878_112_2_18_2_20_20_274 "MMIXFIND HandleFindCallback break"
#define MMIFMM_INTERFACE_923_112_2_18_2_20_21_275 "MMIFMM_XFindQuickSearch: result = %d,  find_handle= 0x%x"
#define MMIFMM_INTERFACE_1065_112_2_18_2_20_21_276 "MMIAPIFMM_SearchFileInPath param error!"
#define MMIFMM_INTERFACE_1069_112_2_18_2_20_21_277 "MMIAPIFMM_SearchFileInPath, enter"
#define MMIFMM_INTERFACE_1105_112_2_18_2_20_21_278 "MMIAPIFMM_SearchFileInPath param error!"
#define MMIFMM_INTERFACE_1109_112_2_18_2_20_21_279 "MMIAPIFMM_SearchFileInPath, enter"
#define MMIFMM_INTERFACE_1144_112_2_18_2_20_21_280 "MMIAPIFMM_SearchFileInPath param error!"
#define MMIFMM_INTERFACE_1148_112_2_18_2_20_21_281 "MMIAPIFMM_SearchFileInPathEx, enter"
#define MMIFMM_INTERFACE_1185_112_2_18_2_20_21_282 "MMIAPIFMM_SearchFileInPath param error!"
#define MMIFMM_INTERFACE_1188_112_2_18_2_20_21_283 "MMIAPIFMM_SearchFileInPath, enter"
#define MMIFMM_INTERFACE_1225_112_2_18_2_20_21_284 "MMIFMM_SearchFileInPathByString param error:PNULL == file_array"
#define MMIFMM_INTERFACE_1229_112_2_18_2_20_21_285 "MMIFMM_SearchFileInPathByString, enter"
#define MMIFMM_INTERFACE_1302_112_2_18_2_20_22_286 "MMIAPIFMM_SearchFileInPath param error!"
#define MMIFMM_INTERFACE_1306_112_2_18_2_20_22_287 "MMIAPIFMM_SearchFileInPath, enter"
#define MMIFMM_INTERFACE_1328_112_2_18_2_20_22_288 "MMIFMM_SearchFileInTask, enter"
#define MMIFMM_INTERFACE_1333_112_2_18_2_20_22_289 "MMIFMM_SearchFileInTask param error:NULL == is_stop_ptr"
#define MMIFMM_INTERFACE_1340_112_2_18_2_20_22_290 "MMIFMM_SearchFileInTask, stoped by user !!"
#define MMIFMM_INTERFACE_1380_112_2_18_2_20_22_291 "MMIAPIFMM_SearchFileCallBack, enter"
#define MMIFMM_INTERFACE_1389_112_2_18_2_20_22_292 "MMIAPIFMM_SearchFileCallBack, ignored !!"
#define MMIFMM_INTERFACE_1410_112_2_18_2_20_22_293 "MMIFMM_SearchFileStop, enter"
#define MMIFMM_INTERFACE_1476_112_2_18_2_20_22_294 "MMIFMM: HandleOpenFileByLayerWaitWin PNULL == s_file_array"
#define MMIFMM_INTERFACE_1624_112_2_18_2_20_22_295 "MMIFMM_HandleOpenFileByLayerWin, msg_id = %x"
#define MMIFMM_INTERFACE_1722_112_2_18_2_20_22_296 "MMIFMM_HandleOpenFileByLayerWin softkey STXT_RETURN11"
#define MMIFMM_INTERFACE_1767_112_2_18_2_20_23_297 "MMIFMM_HandleOpenFileByLayerWin softkey STXT_RETURN33"
#define MMIFMM_INTERFACE_1774_112_2_18_2_20_23_298 "MMIFMM_HandleOpenFileByLayerWin softkey STXT_RETURN22"
#define MMIFMM_INTERFACE_1805_112_2_18_2_20_23_299 "MMIFMM_HandleOpenFileByLayerWin cur_dev_type = %d,cur_tab_index = %d"
#define MMIFMM_INTERFACE_1854_112_2_18_2_20_23_300 "MSG_CTL_LIST_NEED_ITEM_DATA,msg_id = %x, param error :PNULL == need_item_data_ptr"
#define MMIFMM_INTERFACE_1871_112_2_18_2_20_23_301 "MSG_CTL_LIST_NEED_ITEM_CONTENT,msg_id = %x, param error :PNULL == need_item_content_ptr"
#define MMIFMM_INTERFACE_2085_112_2_18_2_20_23_302 "[MMIFMM]:MSG_APP_CANCEL path_depth = %d"
#define MMIFMM_INTERFACE_2358_112_2_18_2_20_24_303 "DoSearchInFolderRecurse NULL == file_array !!"
#define MMIFMM_INTERFACE_2366_112_2_18_2_20_24_304 "DoSearchInFolderRecurse create s_foldre_farray fail !"
#define MMIFMM_INTERFACE_2445_112_2_18_2_20_24_305 "[MMIFMM] DoSearchInFolderRecurse read error !"
#define MMIFMM_INTERFACE_2474_112_2_18_2_20_24_306 "DoSearchInFolder NULL == file_array !!"
#define MMIFMM_INTERFACE_2554_112_2_18_2_20_24_307 "SearchFileAndSaveFolder_CallBack param error!"
#define MMIFMM_INTERFACE_2615_112_2_18_2_20_24_308 "SearchFileAndSaveFolder param error!"
#define MMIFMM_INTERFACE_2643_112_2_18_2_20_24_309 "SearchFileAndFolder_CallBack param error!"
#define MMIFMM_INTERFACE_2651_112_2_18_2_20_24_310 "MMIFMM:SearchFileAndFolder_CallBack error! PNULL == f_array_data"
#define MMIFMM_INTERFACE_2815_112_2_18_2_20_25_311 "MMIFMM:SearchFileAndFolder param error!"
#define MMIFMM_INTERFACE_2941_112_2_18_2_20_25_312 "MMIFMM:GetPrevPath param error!"
#define MMIFMM_INTERFACE_3014_112_2_18_2_20_25_313 "LoadFileToList param error:PNULL == file_array"
#define MMIFMM_INTERFACE_3045_112_2_18_2_20_25_314 "MMIFMM:LoadFileToList error ! PNULL == s_file_array"
#define MMIFMM_INTERFACE_3149_112_2_18_2_20_25_315 "LoadItemToList param error!"
#define MMIFMM_INTERFACE_3161_112_2_18_2_20_25_316 "[FMM] LoadItemToList read file array failed !"
#define MMIFMM_INTERFACE_3471_112_2_18_2_20_26_317 "FindAllFiles param error!"
#define MMIFMM_INTERFACE_3480_112_2_18_2_20_26_318 "MMIFMM:FindAllFiles error:PNULL == find_data"
#define MMIFMM_INTERFACE_3489_112_2_18_2_20_26_319 "MMIFMM:FindAllFiles error:PNULL == find"
#define MMIFMM_INTERFACE_3567_112_2_18_2_20_26_320 "MMIAPIFMM_GetFileTypeByFileName param error:PNULL == file_name"
#define MMIFMM_INTERFACE_3634_112_2_18_2_20_26_321 "InitSearchParam param error:PNULL == filter_ptr"
#define MMIFMM_INTERFACE_3695_112_2_18_2_20_26_322 "ClearTempFilesInDevice param error!"
#define MMIFMM_INTERFACE_3788_112_2_18_2_20_27_323 "[MMIFMM]OpenSearchFileWaitWin waitwin already opened !"
#define MMIFMM_INTERFACE_3955_112_2_18_2_20_27_324 "MMIAPIFMM_GetOpenFileWinCurFile param error:PNULL == file_data_ptr"
#define MMIFMM_INTERFACE_3973_112_2_18_2_20_27_325 "MMIFMM_SetOpenFileWinDisp disp_type=%d"
#define MMIFMM_INTERFACE_4047_112_2_18_2_20_27_326 "MMIFMM_GetOpenFileWinFile param error:PNULL == file_data_ptr"
#define MMIFMM_INTERFACE_4088_112_2_18_2_20_27_327 "SetWinTitle param error:PNULL == path_name_ptr"
#define MMIFMM_INTERFACE_4123_112_2_18_2_20_27_328 "MMIFMM_INTERFACE.C MMIFMM_GetIconIdByFileName filename == NULL"
#define MMIFMM_INTERFACE_4178_112_2_18_2_20_27_329 "MMIFMM_OpenFileWinByLayerInternal param error:PNULL == filter_ptr"
#define MMIFMM_INTERFACE_4423_112_2_18_2_20_28_330 "MMIFMM_SetDeviceTable tab_index = %d,fmm_dev= %d"
#define MMIFMM_INTERFACE_4439_112_2_18_2_20_28_331 "MMIFMM_GetDeviceType tab_index = %d"
#define MMIFMM_INTERFACE_4498_112_2_18_2_20_28_332 "SearchPlayMovieFile, item_index = %d"
#define MMIFMM_INTERFACE_4502_112_2_18_2_20_28_333 "SearchPlayMovieFile: GET FILE ERROR, item_index = %d"
#define MMIFMM_INTERFACE_4545_112_2_18_2_20_28_334 "SearchPlayMusicFile, item_index = %d"
#define MMIFMM_INTERFACE_4549_112_2_18_2_20_28_335 "SearchPlayMusicFile: GET FILE ERROR, item_index = %d"
#define MMIFMM_INTERFACE_4599_112_2_18_2_20_28_336 "MMISEARCH_GetFileInfo:PNULL == file_info_ptr"
#define MMIFMM_INTERFACE_4604_112_2_18_2_20_28_337 "MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d"
#define MMIFMM_INTERFACE_4653_112_2_18_2_20_28_338 "MMIAPIFMM_OpenMatchedItem, call relative application!"
#define MMIFMM_INTERFACE_4846_112_2_18_2_20_29_339 "MMISEARCH_LoadFileRstToList: READ file array error!"
#define MMIFMM_WINTAB_2224_112_2_18_2_20_39_340 "MMIFMM: SaveCurrentPath."
#define MMIFMM_WINTAB_2227_112_2_18_2_20_39_341 "MMIFMM:[SaveCurrentPath]:PNULL == current_path_ptr"
#define MMIFMM_WINTAB_2232_112_2_18_2_20_39_342 "MMIFMM:[SaveCurrentPath]:PNULL == path_ptr"
#define MMIFMM_WINTAB_2270_112_2_18_2_20_39_343 "MMIFMM: UpdateRenameData, ctrl_id = %d, new_filename_len = %d."
#define MMIFMM_WINTAB_2275_112_2_18_2_20_39_344 "MMIFMM:[UpdateRenameData]:PNULL == list_data_ptr"
#define MMIFMM_WINTAB_2280_112_2_18_2_20_39_345 "MMIFMM:[UpdateRenameData]:PNULL == new_filename_ptr"
#define MMIFMM_WINTAB_2287_112_2_18_2_20_39_346 "MMIFMM:[UpdateRenameData]:new_filename_len = %d"
#define MMIFMM_WINTAB_2368_112_2_18_2_20_39_347 "MMIFMM: UpdateListData, win_id =%d, ctrl_id = %d, type = %d."
#define MMIFMM_WINTAB_2373_112_2_18_2_20_39_348 "MMIFMM:[UpdateListData]:param error!"
#define MMIFMM_WINTAB_2471_112_2_18_2_20_40_349 "MMIFMM: FmmOpenFont, ctrl_id = %d"
#define MMIFMM_WINTAB_2476_112_2_18_2_20_40_350 "MMIFMM:[FmmOpenFont]:NULL == list_data_ptr"
#define MMIFMM_WINTAB_2481_112_2_18_2_20_40_351 "MMIFMM:[FmmOpenFont]:NULL == current_path_ptr"
#define MMIFMM_WINTAB_2558_112_2_18_2_20_40_352 "MMIFMM:[HandleListItem]:NULL == list_data_ptr"
#define MMIFMM_WINTAB_2563_112_2_18_2_20_40_353 "MMIFMM:[HandleListItem]:NULL == current_path_ptr"
#define MMIFMM_WINTAB_2581_112_2_18_2_20_40_354 "MMIFMM: HandleListItem , ctrl_id = %d, index = %d"
#define MMIFMM_WINTAB_2587_112_2_18_2_20_40_355 "MMIFMM:[HandleListItem]:NULL == ctrl_ptr"
#define MMIFMM_WINTAB_2606_112_2_18_2_20_40_356 "MMIFMM: HandleListItem, Max depth!"
#define MMIFMM_WINTAB_2619_112_2_18_2_20_40_357 "MMIFMM: HandleListItem, enter subdir!"
#define MMIFMM_WINTAB_2636_112_2_18_2_20_40_358 "MMIFMM:[HandleListItem]: total_pathname_len = %d"
#define MMIFMM_WINTAB_2662_112_2_18_2_20_40_359 "MMIFMM: HandleListItem, call relative application!"
#define MMIFMM_WINTAB_2872_112_2_18_2_20_40_360 "MMIFMM: FmmOpenSzipFile, ctrl_id = %d"
#define MMIFMM_WINTAB_2877_112_2_18_2_20_40_361 " PNULL == list_data_ptr"
#define MMIFMM_WINTAB_2882_112_2_18_2_20_40_362 " PNULL == current_path_ptr"
#define MMIFMM_WINTAB_2908_112_2_18_2_20_40_363 "MMIFMM: GetNextIndex."
#define MMIFMM_WINTAB_2913_112_2_18_2_20_40_364 "MMIFMM:[GetNextIndex] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_2918_112_2_18_2_20_40_365 "MMIFMM:[GetNextIndex] PNULL == folder_name_ptr"
#define MMIFMM_WINTAB_2956_112_2_18_2_20_40_366 "MMIFMM: SetupPicIndexData."
#define MMIFMM_WINTAB_2961_112_2_18_2_20_40_367 "MMIFMM:[SetupPicIndexData] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_2966_112_2_18_2_20_41_368 "MMIFMM:[SetupPicIndexData] PNULL == pic_index_data_ptr"
#define MMIFMM_WINTAB_3048_112_2_18_2_20_41_369 "MMIFMM: HandleFmmMainWinMsg, win_id =%d, msg_id = %d."
#define MMIFMM_WINTAB_3204_112_2_18_2_20_41_370 "MMIFMM: FmmInitDevicePath, device =%d."
#define MMIFMM_WINTAB_3386_112_2_18_2_20_41_371 "[VP]: AppendTwoLineAnimTextAndTextListItem FmmVideoThumbnailAdd  !"
#define MMIFMM_WINTAB_3554_112_2_18_2_20_42_372 "MMIFMM: HandleMenuMark."
#define MMIFMM_WINTAB_3558_112_2_18_2_20_42_373 "MMIFMM:[HandleMenuMark]PNULL == list_data_ptr"
#define MMIFMM_WINTAB_3595_112_2_18_2_20_42_374 "MMIFMM: HandleMenuCancelMark."
#define MMIFMM_WINTAB_3599_112_2_18_2_20_42_375 "MMIFMM:[HandleMenuCancelMark]PNULL == list_data_ptr"
#define MMIFMM_WINTAB_3640_112_2_18_2_20_42_376 "MMIFMM: HandleMenuMarkAll."
#define MMIFMM_WINTAB_3644_112_2_18_2_20_42_377 "MMIFMM:[HandleMenuMarkAll]PNULL == list_data_ptr"
#define MMIFMM_WINTAB_3683_112_2_18_2_20_42_378 "MMIFMM: HandleMenuCancelAllMark."
#define MMIFMM_WINTAB_3733_112_2_18_2_20_42_379 "MMIFMM: EnablelMenuItem."
#define MMIFMM_WINTAB_3737_112_2_18_2_20_42_380 "MMIFMM:[DisableInvalidMenuItem] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_3743_112_2_18_2_20_42_381 "MMIFMM:[DisableInvalidMenuItem] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_4002_112_2_18_2_20_43_382 "MMIFMM:[DisableInvalidMenuItem] list_data_ptr->checked = %d"
#define MMIFMM_WINTAB_4055_112_2_18_2_20_43_383 "MMIFMM: FmmFileDetail,  ctrl_id = %d."
#define MMIFMM_WINTAB_4059_112_2_18_2_20_43_384 "MMIFMM:[FmmFileDetail] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_4086_112_2_18_2_20_43_385 "MMIFMM: FmmRenameFile, win_id =%d, ctrl_id = %d."
#define MMIFMM_WINTAB_4090_112_2_18_2_20_43_386 "MMIFMM:[FmmRenameFile] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_4121_112_2_18_2_20_43_387 "[MMIFMM] MMIFMM_MAIN_WIN_ID already closed !"
#define MMIFMM_WINTAB_4126_112_2_18_2_20_43_388 "[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !"
#define MMIFMM_WINTAB_4134_112_2_18_2_20_43_389 "MMIFMM: HandleNormalMenuWinMsg, win_id =0x%x, msg_id = 0x%x."
#define MMIFMM_WINTAB_4188_112_2_18_2_20_43_390 "MMIFMM: FmmSetWallpaper, ctrl_id = %d"
#define MMIFMM_WINTAB_4193_112_2_18_2_20_43_391 "MMIFMM:[FmmSetWallpaper] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_4199_112_2_18_2_20_43_392 "MMIFMM:[FmmSetWallpaper] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_4239_112_2_18_2_20_43_393 "MMIFMM: FmmSetPB, ctrl_id = %d"
#define MMIFMM_WINTAB_4244_112_2_18_2_20_43_394 "MMIFMM:[FmmSetPB] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_4250_112_2_18_2_20_43_395 "MMIFMM:[FmmSetPB] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_4353_112_2_18_2_20_43_396 "MMIFMM: FmmSetCallRing."
#define MMIFMM_WINTAB_4358_112_2_18_2_20_43_397 "MMIFMM:[FmmSetCallRing] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_4364_112_2_18_2_20_43_398 "MMIFMM:[FmmSetCallRing] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_4418_112_2_18_2_20_43_399 "[MMIFMM] MMIFMM_MAIN_WIN_ID already closed !"
#define MMIFMM_WINTAB_4424_112_2_18_2_20_43_400 "[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !"
#define MMIFMM_WINTAB_4436_112_2_18_2_20_43_401 "[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !"
#define MMIFMM_WINTAB_4446_112_2_18_2_20_43_402 "[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !"
#define MMIFMM_WINTAB_4453_112_2_18_2_20_43_403 "MMIFMM: HandleMusicMenuWinMsg, win_id =%d, msg_id = %d."
#define MMIFMM_WINTAB_4517_112_2_18_2_20_44_404 "[VP] HandleVideoScreenSaverMsg msg_ptr->msg_id = %d"
#define MMIFMM_WINTAB_4695_112_2_18_2_20_44_405 "[MMIFMM] MMIFMM_MAIN_WIN_ID already closed !"
#define MMIFMM_WINTAB_4701_112_2_18_2_20_44_406 "[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !"
#define MMIFMM_WINTAB_4713_112_2_18_2_20_44_407 "[MMIFMM] MMIFMM_SDCARD_CHILD_WIN_ID already closed !"
#define MMIFMM_WINTAB_4723_112_2_18_2_20_44_408 "[MMIFMM] MMIFMM_SDCARD_CHILD_WIN_ID already closed !"
#define MMIFMM_WINTAB_4730_112_2_18_2_20_44_409 "MMIFMM: HandleMovieMenuWinMsg, win_id =%d, msg_id = %d."
#define MMIFMM_WINTAB_4975_112_2_18_2_20_44_410 "MMIFMM: HandlePicPreviewMenuWinMsg, win_id =%d, msg_id = %d."
#define MMIFMM_WINTAB_5033_112_2_18_2_20_45_411 "MMIFMM: FmmSendMMS."
#define MMIFMM_WINTAB_5038_112_2_18_2_20_45_412 "MMIFMM:[FmmSendMMS] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_5044_112_2_18_2_20_45_413 "MMIFMM:[FmmSendMMS] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_5124_112_2_18_2_20_45_414 "MMIFMM: FmmBtSendFile."
#define MMIFMM_WINTAB_5127_112_2_18_2_20_45_415 "MMIFMM:[FmmBtSendFile] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_5133_112_2_18_2_20_45_416 "MMIFMM:[FmmBtSendFile] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_5169_112_2_18_2_20_45_417 "[&&&]FmmBtSendFile  send_file_num = %d"
#define MMIFMM_WINTAB_5174_112_2_18_2_20_45_418 "[&&&]FmmBtSendFile  send_file_num=%d"
#define MMIFMM_WINTAB_5220_112_2_18_2_20_45_419 "[&&&]FmmBtSendFile  send_file_index=%d,send_file_num = %d"
#define MMIFMM_WINTAB_5240_112_2_18_2_20_45_420 "MMIFMM: FmmFileDataSort, sort =%d."
#define MMIFMM_WINTAB_5243_112_2_18_2_20_45_421 "MMIFMM:[FmmFileDataSort] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_5284_112_2_18_2_20_45_422 "[MMIFMM] MMIFMM_MAIN_WIN_ID already closed !"
#define MMIFMM_WINTAB_5291_112_2_18_2_20_45_423 "[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !"
#define MMIFMM_WINTAB_5296_112_2_18_2_20_45_424 "MMIFMM: HandleEmptyFolderMenuWinMsg, win_id =%d, msg_id = %d."
#define MMIFMM_WINTAB_5394_112_2_18_2_20_45_425 "MMIFMM: FmmPlayMovieFile, ctrl_id = %d"
#define MMIFMM_WINTAB_5399_112_2_18_2_20_45_426 "MMIFMM:[FmmPlayMovieFile] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_5405_112_2_18_2_20_45_427 "MMIFMM:[FmmPlayMovieFile] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_5456_112_2_18_2_20_46_428 "MMIFMM: FmmOpenVcardFile, ctrl_id = %d"
#define MMIFMM_WINTAB_5461_112_2_18_2_20_46_429 "MMIFMM:[FmmOpenVcardFile] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_5467_112_2_18_2_20_46_430 "MMIFMM:[FmmOpenVcardFile] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_5488_112_2_18_2_20_46_431 "MMIFMM: FmmOpenVcalendarFile, ctrl_id = %d"
#define MMIFMM_WINTAB_5493_112_2_18_2_20_46_432 "MMIFMM:[FmmOpenVcardFile] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_5499_112_2_18_2_20_46_433 "MMIFMM:[FmmOpenVcardFile] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_5528_112_2_18_2_20_46_434 "MMIFMM: FmmPlayMusicFile, ctrl_id = %d"
#define MMIFMM_WINTAB_5533_112_2_18_2_20_46_435 "MMIFMM:[FmmPlayMusicFile] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_5539_112_2_18_2_20_46_436 "MMIFMM:[FmmPlayMusicFile] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_5607_112_2_18_2_20_46_437 "MMIFMM: FmmOpenWWWFile, ctrl_id = %d"
#define MMIFMM_WINTAB_5839_112_2_18_2_20_46_438 "FmmFindStringIsValid PNULL == full_path_wstr_ptr"
#define MMIFMM_WINTAB_6255_112_2_18_2_20_47_439 "HandleCreateFolderWinMsg: create_result = %d"
#define MMIFMM_WINTAB_6322_112_2_18_2_20_47_440 "MMIFMM:[NewFolderNameStringIsValid] PNULL == file_name_ptr"
#define MMIFMM_WINTAB_6369_112_2_18_2_20_47_441 "[MMIFMM] FmmRename create MMIFMM_RENAME_WIN_TAB failed !"
#define MMIFMM_WINTAB_6379_112_2_18_2_20_47_442 "MMIFMM: FmmRename, no memeory!"
#define MMIFMM_WINTAB_6423_112_2_18_2_20_48_443 "MMIFMM: HandleFmmRenameWinMsg, win_id =0x%x, msg_id = 0x%x."
#define MMIFMM_WINTAB_6439_112_2_18_2_20_48_444 "[MMIFMM] HandleFmmRenameWinMsg MSG_OPEN_WINDOW PNULL == rename_param_ptr"
#define MMIFMM_WINTAB_6471_112_2_18_2_20_48_445 "HandleFmmRenameWinMsg, max_file_name_len = %d."
#define MMIFMM_WINTAB_6561_112_2_18_2_20_48_446 "HandleFmmRenameWinMsg: rename_result = %d"
#define MMIFMM_WINTAB_6601_112_2_18_2_20_48_447 "HandleFmmRenameWinMsg: file_is_power_on_anim = %d"
#define MMIFMM_WINTAB_6602_112_2_18_2_20_48_448 "HandleFmmRenameWinMsg: file_is_power_off_anim = %d"
#define MMIFMM_WINTAB_6608_112_2_18_2_20_48_449 "HandleFmmRenameWinMsg: file_is_ssaver = %d"
#define MMIFMM_WINTAB_6656_112_2_18_2_20_48_450 "HandleFmmRenameWinMsg: file_is_ssaver = %d"
#define MMIFMM_WINTAB_6833_112_2_18_2_20_48_451 "CheckIsScreensaverInfo: ssaver_info->more_pic_saver_info.name_len = %d"
#define MMIFMM_WINTAB_6834_112_2_18_2_20_48_452 "CheckIsScreensaverInfo: old_name_len = %d"
#define MMIFMM_WINTAB_6879_112_2_18_2_20_49_453 "CheckIsPowerOnOffAnimInfo: power_onoff_anim_info->more_anim_info.name_len = %d"
#define MMIFMM_WINTAB_6880_112_2_18_2_20_49_454 "CheckIsPowerOnOffAnimInfo: old_name_len = %d"
#define MMIFMM_WINTAB_6947_112_2_18_2_20_49_455 "MMIFMM:[FmmRenameSplitFolder]PNULL == full_path_ptr"
#define MMIFMM_WINTAB_6954_112_2_18_2_20_49_456 "FmmRenameSplitFolder string_len =%d"
#define MMIFMM_WINTAB_6991_112_2_18_2_20_49_457 "MMIFMM:[FmmRenameSplitFile]PNULL == full_path_ptr"
#define MMIFMM_WINTAB_6998_112_2_18_2_20_49_458 "FmmRenameSplitFile string_len =%d"
#define MMIFMM_WINTAB_7051_112_2_18_2_20_49_459 "MMIFMM: MMIAPIFMM_DetailFileData, no memory!"
#define MMIFMM_WINTAB_7157_112_2_18_2_20_49_460 "MMIFMM: HandleFmmFileDetailWinMsg, open option"
#define MMIFMM_WINTAB_7174_112_2_18_2_20_49_461 "MMIFMM: HandleFmmFileDetailWinMsg, Download Rights File"
#define MMIFMM_WINTAB_7232_112_2_18_2_20_49_462 "[MMIFMM] HandleFmmQueryWin win_info_ptr == PNULL !"
#define MMIFMM_WINTAB_7425_112_2_18_2_20_50_463 "FmmCheckMemoryCardState result = %d"
#define MMIFMM_WINTAB_7520_112_2_18_2_20_50_464 "MMIFMM: MMIAPIFMM_OpenExplorer."
#define MMIFMM_WINTAB_7672_112_2_18_2_20_50_465 "MMIFMM: CopySelectFileToFolder"
#define MMIFMM_WINTAB_7679_112_2_18_2_20_50_466 "CopySelectFileToFolder alloc s_copy_del_path_data_ptr fail!"
#define MMIFMM_WINTAB_7786_112_2_18_2_20_51_467 "MMIFMM: DeleteSelectFile"
#define MMIFMM_WINTAB_7828_112_2_18_2_20_51_468 "MMIFMM: SetCopyDeleteState state %d"
#define MMIFMM_WINTAB_7838_112_2_18_2_20_51_469 "MMIFMM: GetCopyDeleteState state= %d"
#define MMIFMM_WINTAB_7856_112_2_18_2_20_51_470 "MMIFMM: HandleWaitCopyWinMsg, win_id =%d, msg_id = 0x%x."
#define MMIFMM_WINTAB_7892_112_2_18_2_20_51_471 "[FMM]HandleWaitCopyWinMsg abort!! Copy"
#define MMIFMM_WINTAB_7930_112_2_18_2_20_51_472 "MMIFMM: HandleCopyFileToFolderWinMsg, win_id =%d, msg_id = 0x%x"
#define MMIFMM_WINTAB_8195_112_2_18_2_20_52_473 "MMIFMM: HandleWaitDeleteWinMsg, win_id =%d, msg_id = %d."
#define MMIFMM_WINTAB_8230_112_2_18_2_20_52_474 "[FMM]HandleWaitDeleteWinMsg abort!! Delete"
#define MMIFMM_WINTAB_8260_112_2_18_2_20_52_475 "MMIFMM: HandleDelFileFolderWinMsg, win_id =%d, msg_id = 0x%x."
#define MMIFMM_WINTAB_8379_112_2_18_2_20_52_476 "FMM_DEL: MSG_FMM_DELETE_END_CNF file_ret = %d"
#define MMIFMM_WINTAB_8412_112_2_18_2_20_52_477 "MMIFMM:MMIAPIFMM_StopOperaSDPlug close relative windows."
#define MMIFMM_WINTAB_8612_112_2_18_2_20_53_478 "[MMIFMM] MMIAPIFMM_CopyFileExistInform enter"
#define MMIFMM_WINTAB_8703_112_2_18_2_20_53_479 "MMIFMM: MMIFMM_GetVcardFileData, ctrl_id = %d"
#define MMIFMM_WINTAB_8708_112_2_18_2_20_53_480 "MMIFMM:[MMIFMM_GetVcardFileData] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_8714_112_2_18_2_20_53_481 "MMIFMM:[MMIFMM_GetVcardFileData] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_8722_112_2_18_2_20_53_482 "MMIFMM_GetVcardFileData list_data_ptr->data[index].file_size %d"
#define MMIFMM_WINTAB_8745_112_2_18_2_20_53_483 "MMIFMM: MMIFMM_GetVcalendarFileData, ctrl_id = %d"
#define MMIFMM_WINTAB_8749_112_2_18_2_20_53_484 "MMIFMM:[MMIFMM_GetVcalendarFileData] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_8755_112_2_18_2_20_53_485 "MMIFMM:[MMIFMM_GetVcalendarFileData] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_8763_112_2_18_2_20_53_486 "MMIFMM_GetVcalendarFileData list_data_ptr->data[index].file_size %d"
#define MMIFMM_WINTAB_8778_112_2_18_2_20_53_487 "MMIFMM: MMIAPIFMM_GetFmmData."
#define MMIFMM_WINTAB_9191_112_2_18_2_20_54_488 "MMIFMM: HandleUdiskChildWindow, win_id =0x%x, msg_id = 0x%x."
#define MMIFMM_WINTAB_9284_112_2_18_2_20_54_489 "[MMIFMM] HandleUdiskChildWindow MSG_CTL_TAB_SWITCH cur_tab %d ,s_cur_dev %d!"
#define MMIFMM_WINTAB_9374_112_2_18_2_20_55_490 "MMIFMM: HandleUdiskChildWindow, call relative menu!"
#define MMIFMM_WINTAB_9639_112_2_18_2_20_55_491 "MMIFMM: HandleUdiskChildWindow, is_videofolder:%d,state:%d"
#define MMIFMM_WINTAB_9644_112_2_18_2_20_55_492 "MMIFMM: HandleUdiskChildWindow,GUILIST_START_SLIDE"
#define MMIFMM_WINTAB_9697_112_2_18_2_20_56_493 "MMIFMM: HandleMovieMenuWinMsg, win_id =%d, msg_id = %d."
#define MMIFMM_WINTAB_9765_112_2_18_2_20_56_494 "MMIFMM: HandleMovieMenuWinMsg, win_id =%d, msg_id = %d."
#define MMIFMM_WINTAB_9834_112_2_18_2_20_56_495 "MMIFMM: HandleOtherMenuWinMsg, win_id =%d, msg_id = %d."
#define MMIFMM_WINTAB_9887_112_2_18_2_20_56_496 "MMIFMM: FmmSetAsScreenSaver,  ctrl_id = %d."
#define MMIFMM_WINTAB_9891_112_2_18_2_20_56_497 "MMIFMM: [FmmSetAsScreenSaver] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_9950_112_2_18_2_20_56_498 "MMIFMM: FmmSetAsVideoWallpaper,  ctrl_id = %d."
#define MMIFMM_WINTAB_9953_112_2_18_2_20_56_499 "MMIFMM: [FmmSetAsVideoWallpaper] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_10021_112_2_18_2_20_56_500 "MMIFMM: FmmSetAsPowerOnOffAnim,  ctrl_id = %d."
#define MMIFMM_WINTAB_10025_112_2_18_2_20_56_501 "MMIFMM: [FmmSetAsPowerOnOffAnim] PNULL == list_data_ptr"
#define MMIFMM_WINTAB_10106_112_2_18_2_20_56_502 "MMIFMM: SetWinTitle, win_id = %d current_path_ptr->path_depth = %d"
#define MMIFMM_WINTAB_10110_112_2_18_2_20_56_503 "MMIFMM: [SetWinTitle] PNULL == current_path_ptr"
#define MMIFMM_WINTAB_10118_112_2_18_2_20_56_504 "MMIFMM: [SetWinTitle] device = %d"
#define MMIFMM_WINTAB_10221_112_2_18_2_20_57_505 "MMIFMM:[SetStrWildcard]PNULL == str"
#define MMIFMM_WINTAB_10258_112_2_18_2_20_57_506 "MMIFMM_COPY_SELECT_FILE_WIN_ID enter"
#define MMIFMM_WINTAB_10272_112_2_18_2_20_57_507 "MMIFMM_DEL_SELECT_FILE_WIN_ID enter"
#define MMIFMM_WINTAB_10316_112_2_18_2_20_57_508 "MMIAPIFMM_IsFileOption return false!"
#define MMIFMM_WINTAB_10368_112_2_18_2_20_57_509 "MMIFMM: GetFileIndex."
#define MMIFMM_WINTAB_10373_112_2_18_2_20_57_510 "MMIFMM:[GetFileIndex]PNULL == list_data_ptr"
#define MMIFMM_WINTAB_10379_112_2_18_2_20_57_511 "MMIFMM:[GetFileIndex]PNULL == file_name_ptr"
#define MMIFMM_WINTAB_10500_112_2_18_2_20_57_512 "MMIFMM: FmmPlaykurFile, ctrl_id = %d"
#define MMIFMM_WINTAB_10505_112_2_18_2_20_57_513 "MMIFMM:[GetFileIndex]PNULL == list_data_ptr"
#define MMIFMM_WINTAB_10511_112_2_18_2_20_57_514 "MMIFMM:[GetFileIndex]PNULL == current_path_ptr"
#define MMIFMM_WINTAB_10540_112_2_18_2_20_57_515 "MMIFMM:[MMIAPIFMM_GetFileInfoFormFullPath]PNULL ==file_info"
#define MMIFMM_WINTAB_10569_112_2_18_2_20_57_516 "MMIFMM: CombineFileName."
#define MMIFMM_WINTAB_10575_112_2_18_2_20_57_517 "MMIFMM:[CombineToFileName]:param error!"
#define MMIFMM_WINTAB_10588_112_2_18_2_20_57_518 "MMIFMM:[CombineToFileName]:total_len = %d"
#define MMIFMM_WINTAB_10708_112_2_18_2_20_58_519 "MMIFMM:[HandleMenuOption]PNULL == list_data_ptr"
#define MMIFMM_WINTAB_10714_112_2_18_2_20_58_520 "MMIFMM:[HandleMenuOption]PNULL == current_path_ptr"
#define MMIFMM_WINTAB_11044_112_2_18_2_20_58_521 "MMIFMM: HandleMarkItem, MSG_CTL_LIST_MARK_ITEM, index = %d, mark = %d"
#define MMIFMM_WINTAB_11067_112_2_18_2_20_58_522 "MMIFMM: HandleSDChildWindow, %d state  error!"
#define MMIFMM_WINTAB_11101_112_2_18_2_20_58_523 "MMIFMM: HandleMarkItem, %d state  error!"
#define MMIFMM_WINTAB_11557_112_2_18_2_20_59_524 "MMIFMM: HandleSDChildWindow, return previous directory!"
#define MMIFMM_WINTAB_11636_112_2_18_2_20_59_525 "MMIFMM:[MMIAPIFMM_HandleMenuOpt] type = %d"
#define MMIFMM_WINTAB_11676_112_2_18_2_21_0_526 "MMIFMM:[MMIFMM_GetFullPathDepth]PNULL == path_ptr"
#define MMIFMM_WINTAB_11687_112_2_18_2_21_0_527 "MMIFMM_GetFullPathDepth path_depth =%d"
#define MMIFMM_WINTAB_11703_112_2_18_2_21_0_528 "MMIFMM:[FmmGetPathDepthLen]PNULL == path_ptr"
#define MMIFMM_WINTAB_11720_112_2_18_2_21_0_529 "FmmGetPathDepthLen path_len =%d"
#define MMIFMM_WINTAB_11735_112_2_18_2_21_0_530 "MMIFMM:[MMIFMM_GetFinallySlashIndex]PNULL == file_name_ptr"
#define MMIFMM_WINTAB_11745_112_2_18_2_21_0_531 "MMIFMM_GetFinallySlashIndex i =%d"
#define MMIFMM_WINTAB_11761_112_2_18_2_21_0_532 "FmmFileIsFolder IsFolder =%d"
#define MMIFMM_WINTAB_11836_112_2_18_2_21_0_533 "MMIFMM:[FmmCopyDelExit]MMI_USE_OLD_SFS type = %d"
#define MMIFMM_WINTAB_11876_112_2_18_2_21_0_534 "MMIFMM:[FmmHandleListAppendData]PNULL == need_item_data_ptr"
#define MMIFMM_WINTAB_11887_112_2_18_2_21_0_535 "MMIFMM:[FmmHandleListAppendData]PNULL == need_item_content_ptr"
#define MMIFMM_WINTAB_11897_112_2_18_2_21_0_536 "MMIFMM:[FmmHandleListAppendData] type = %d"
#define MMIFMM_WINTAB_11918_112_2_18_2_21_0_537 "FmmHandleListAppendData: index = %d file_num = %d,folder_num = %d"
#define MMIFMM_WINTAB_11962_112_2_18_2_21_0_538 "[MMIFMM] MMIFMM_GetCurrentDevice main win not open !"
#define MMIFMM_WINTAB_12027_112_2_18_2_21_0_539 "MMIAPIFMM_UpdateFmmList device = %d"
#define MMIFMM_WINTAB_12112_112_2_18_2_21_0_540 "MMIFMM: HandleFmmCopyRenameWinMsg, win_id =0x%x, msg_id = 0x%x."
#define MMIFMM_WINTAB_12147_112_2_18_2_21_0_541 "HandleFmmCopyRenameWinMsg, max_file_name_len = %d."
#define MMIFMM_WINTAB_12303_112_2_18_2_21_1_542 "CopyFilesCallBack error = %d"
#define MMIFMM_WINTAB_12320_112_2_18_2_21_1_543 "DelFilesCallBack error = %d"
#define MMIFMM_WINTAB_12385_112_2_18_2_21_1_544 "GetTargetPathCallBack array size == 0"
#define MMIFMM_WINTAB_12394_112_2_18_2_21_1_545 "GetTargetPathCallBack failed"
#define MMIFMM_WINTAB_12591_112_2_18_2_21_1_546 "FmmIsCopyDelOK result = %d"
#define MMIFMM_WINTAB_12631_112_2_18_2_21_1_547 "MMIFMM: GetTargetnameFromSrcTarget."
#define MMIFMM_WINTAB_12638_112_2_18_2_21_1_548 "MMIFMM:[GetTargetnameFromSrcTarget] param error!"
#define MMIFMM_WINTAB_12890_112_2_18_2_21_2_549 "[MMIFMM] MMIAPIFMM_CopyNext s_user_stop_copy %d enter"
#define MMIFMM_WINTAB_12922_112_2_18_2_21_2_550 "MMIFMM:[MMIFMM_GetPicName] PNULL == name_len_ptr"
#define MMIFMM_WINTAB_12960_112_2_18_2_21_2_551 "[ATEST][Atest_FMMCreateFolderHori:]%d"
#define MMIFMM_WINTAB_12964_112_2_18_2_21_2_552 "[atest]Atest_FMMCreateFolderHori: entry number error! "
#define MMIFMM_WINTAB_13000_112_2_18_2_21_2_553 "[Atest]Atest_FMMCreateFolderHori: new:%d"
#define MMIFMM_WINTAB_13005_112_2_18_2_21_2_554 "[atest]Atest_FMMCreateFolderHori:  new:%d"
#define MMIFMM_WINTAB_13039_112_2_18_2_21_2_555 "[Atest]Atest_FMMCreateFolderVerti: entry error!"
#define MMIFMM_WINTAB_13062_112_2_18_2_21_2_556 "[atest]Atest_FMMCreateFolderVerti:new:%d"
#define MMIFMM_WINTAB_13067_112_2_18_2_21_2_557 "[atest]Atest_FMMCreateFolderVerti:new:%d"
#define MMIFMM_WINTAB_13092_112_2_18_2_21_2_558 "[MMIFMM:]MMIAPIFMM_DrmOpenDownloadMenuWin = PNULL!"
#define MMIFMM_WINTAB_13453_112_2_18_2_21_3_559 "MMIAPIFMM_OpenFile Enter"
#define MMIFMM_WINTAB_13457_112_2_18_2_21_3_560 "MMIAPIFMM_OpenFile, file name is null"
#define MMIFMM_WINTAB_13474_112_2_18_2_21_3_561 "MMIAPIFMM_OpenFile Fail, no memory"
#define MMIFMM_WINTAB_13611_112_2_18_2_21_4_562 "MMIFMM: HandlePDAMenuMark."
#define MMIFMM_WINTAB_13614_112_2_18_2_21_4_563 "MMIFMM:[HandlePDAMenuMark]PNULL == list_data_ptr"
#define MMIFMM_WINTAB_13841_112_2_18_2_21_4_564 "IsMarkItemContainFolders s_fmm_list_data.mark_num = 0!"
#define MMIFMM_WINTAB_13870_112_2_18_2_21_4_565 "[VP]  FmmVideoThumbnailGetFloder error parameter"
#define MMIFMM_WINTAB_13896_112_2_18_2_21_4_566 "[VP]  FmmVideoThumbnailGetPathname error parameter"
#define MMIFMM_WINTAB_13935_112_2_18_2_21_4_567 "[VP]:FmmVideoThumbnailUpdateIcon  !"
#define MMIFMM_WINTAB_13942_112_2_18_2_21_4_568 "[VP]:FmmVideoThumbnailUpdateIcon  s_fmm_list_data.data[ top_index+index :%d ]->filename_ptr PNULL!"
#define MMIFMM_WINTAB_13953_112_2_18_2_21_4_569 "MMIFMM: FmmVideoThumbnailUpdateIcon,index:%d"
#define MMIFMM_WINTAB_13965_112_2_18_2_21_4_570 "MMIFMM: FmmVideoThumbnailUpdateIcon,GUILIST_ReplaceItem index:%d"
#define MMIFMM_WINTAB_13987_112_2_18_2_21_4_571 "MMIFMM: FmmVideoThumbnailLoadingMovieList,top_index:%d,item_num:%d"
#define MMIFMM_WINTAB_13994_112_2_18_2_21_4_572 "MMIFMM: FmmVideoThumbnailLoadingMovieList,s_fmm_list_data.data[ top_index+index:%d ]->filename_ptr PNULL"
#define MMIFMM_WINTAB_14004_112_2_18_2_21_4_573 "MMIFMM: FmmVideoThumbnailLoadingMovieList,FmmVideoThumbnailAdd,index:%d"
#define MMIFMM_WINTAB_14025_112_2_18_2_21_4_574 "[VP]:FmmVideoThumbnailCallback !"
#define MMIFMM_WINTAB_14032_112_2_18_2_21_4_575 "[VP]:FmmVideoThumbnailCallback MMIAPIFMM_CreateDir !"
#define MMIFMM_WINTAB_14064_112_2_18_2_21_4_576 "[VP]:FmmVideoThumbnailOpen  !"
#define MMIFMM_WINTAB_14068_112_2_18_2_21_4_577 "[VP]:FmmVideoThumbnailOpen  slide now!"
#define MMIFMM_WINTAB_14096_112_2_18_2_21_5_578 "[VP]:FmmVideoThumbnailClose  !"
#define MMIFMM_WINTAB_14111_112_2_18_2_21_5_579 "[VP]:FmmVideoThumbnailAdd  !"
#define MMIFMM_WINTAB_14176_112_2_18_2_21_5_580 "[VP]:FmmVideoThumbnail_Init !"
#define MMIFMM_WINTAB_14284_112_2_18_2_21_5_581 "[MMIFMM]:GetMoveTargetPathCallBack alloc failed"
#define MMIFMM_WINTAB_14302_112_2_18_2_21_5_582 "[MMIFMM]:GetMoveTargetPathCallBack array size == 0"
#define MMIFMM_WINTAB_14311_112_2_18_2_21_5_583 "[MMIFMM]:GetMoveTargetPathCallBack failed"
#define MMIFMM_WINTAB_14350_112_2_18_2_21_5_584 "[MMIFMM]:HandleMoveFileWinMsg PNULL == s_src_data_ptr"
#define MMIFMM_WINTAB_14359_112_2_18_2_21_5_585 "[MMIFMM]:HandleMoveFileWinMsg PNULL == s_src_data_ptr"
#define MMIFMM_WINTAB_14394_112_2_18_2_21_5_586 "[MMIFMM]:HandleMoveFileWinMsg MSG_PROMPTWIN_OK PNULL == s_move_dst_full_path_ptr!"
#define MMIFMM_WINTAB_14404_112_2_18_2_21_5_587 "[MMIFMM]:HandleMoveFileWinMsg PNULL == s_copy_handle_ptr"
#define MMIFMM_WINTAB_14418_112_2_18_2_21_5_588 "[MMIFMM]:HandleMoveFileWinMsg MSG_FMM_COPY_CHOOSE_PATH_RETURN PNULL == s_move_dst_full_path_ptr!"
#define MMIFMM_WINTAB_14470_112_2_18_2_21_5_589 "[MMIFMM]:HandleMoveFileWinMsg MSG_FMM_OPERATE_FILE_BEGIN PNULL == s_move_dst_full_path_ptr!"
#define MMIFMM_WINTAB_14476_112_2_18_2_21_5_590 "[MMIFMM]:HandleMoveFileWinMsg s_move_dst_full_path_ptr len == 0!"
#define MMIFMM_WINTAB_14492_112_2_18_2_21_5_591 "[MMIFMM]:HandleMoveFileWinMsg cur_index = %d"
#define MMIFMM_WINTAB_14497_112_2_18_2_21_5_592 "[MMIFMM]:total name too long %d"
#define MMIFMM_WINTAB_14582_112_2_18_2_21_6_593 "[MMIFMM]:HandleMoveFileWinMsg cur_index = %d"
#define MMIFMM_WINTAB_14617_112_2_18_2_21_6_594 "[MMIFMM]:HandleMoveFileWinMsg stopped by user!"
#define MMIFMM_WINTAB_14639_112_2_18_2_21_6_595 "[MMIFMM]:HandleMoveFileWinMsg case MSG_FMM_MOVE_MARK_FILE 0 == s_fmm_list_data.mark_num"
#define MMIFMM_WINTAB_14783_112_2_18_2_21_6_596 "[MMIFMM] MMIAPIFMM_MoveFileExist enter"
#define MMIFMM_WINTAB_14811_112_2_18_2_21_6_597 "[MMIFMM]: HandleWaitMoveWinMsg, win_id =%d, msg_id = 0x%x."
#define MMIFMM_WINTAB_14829_112_2_18_2_21_6_598 "[MMIFMM]:HandleWaitMoveWinMsg abort!! moveS"
#define MMIVIRTUALARRAY_317_112_2_18_2_21_8_599 "MMIVIRTUALARRAY_Create:MMIVIRTUALARRAY_Create PNULL == v_header_ptr "
#define MMIVIRTUALARRAY_348_112_2_18_2_21_8_600 "MMIVIRTUALARRAY_Create, create virtualarray by no udisk space!"
#define MMIVIRTUALARRAY_367_112_2_18_2_21_8_601 "MMIVIRTUALARRAY_Create, create virtualarray by file!"
#define MMIVIRTUALARRAY_372_112_2_18_2_21_8_602 "MMIVIRTUALARRAY_Create, create virtualarray by no udisk space!"
#define MMIVIRTUALARRAY_1034_112_2_18_2_21_9_603 "MMIFMM:MMIVIRTUALARRAY_Sort PNULL == sort_file_ptr"
#define MMIVIRTUALARRAY_1096_112_2_18_2_21_9_604 "MMIFMM:[UpdateBufferToFile] PNULL == v_header_ptr"
#define MMIVIRTUALARRAY_1100_112_2_18_2_21_9_605 "UpdateBufferToFile, first id = %d, num = %d"
#define MMIVIRTUALARRAY_1205_112_2_18_2_21_9_606 "MMIFMM:[ReadFileToBuffer] PNULL == v_header_ptr"
#define MMIVIRTUALARRAY_1247_112_2_18_2_21_9_607 "ReadFileToBuffer, first_id = %d, num = %d"
#define MMIVIRTUALARRAY_1272_112_2_18_2_21_9_608 "MMIFMM:[DeleteOneDataInFile] PNULL == v_header_ptr"
#define MMIVIRTUALARRAY_1297_112_2_18_2_21_9_609 "MMIFMM:[DeleteOneDataInFile] 1 PNULL == v_header_ptr->del_id_table_ptr"
#define MMIVIRTUALARRAY_1314_112_2_18_2_21_9_610 "MMIFMM:[DeleteOneDataInFile] 2 PNULL == v_header_ptr->del_id_table_ptr"
#define MMIVIRTUALARRAY_1339_112_2_18_2_21_10_611 "MMIFMM:[DeleteAssoFile] PNULL == v_header_ptr"
#define MMIVIRTUALARRAY_1345_112_2_18_2_21_10_612 "DeleteAssoFile, fail!!"
#define MMIVIRTUALARRAY_1360_112_2_18_2_21_10_613 "MMIFMM:[DeleteAssoFile] PNULL == v_header_ptr"
#define MMIVIRTUALARRAY_1385_112_2_18_2_21_10_614 "MMIFMM:[CreateRandFile] PNULL == file_name_ptr"
#define MMIVIRTUALARRAY_1391_112_2_18_2_21_10_615 "MMIFMM:[CreateRandFile] PNULL == file_name_len_ptr"
#define MMIVIRTUALARRAY_1437_112_2_18_2_21_10_616 "MMIFMM:[ReadDataFromBuffer] param error!"
#define MMIVIRTUALARRAY_1465_112_2_18_2_21_10_617 "MMIFMM:[WriteDataToBuffer] param error!"
#define MMIVIRTUALARRAY_1496_112_2_18_2_21_10_618 "MMIFMM:ExternalSortFile param error!"
#define MMIVIRTUALARRAY_1501_112_2_18_2_21_10_619 "MMIFMM:ExternalSortFile PNULL == compare_func"
#define MMIVIRTUALARRAY_1514_112_2_18_2_21_10_620 "ExternalSortFile, not enough memory!"
#define MMIVIRTUALARRAY_1519_112_2_18_2_21_10_621 "ExternalSortFile enter"
#define MMIVIRTUALARRAY_1525_112_2_18_2_21_10_622 "ExternalSortFile, InitMergeFiles fail!"
#define MMIVIRTUALARRAY_1528_112_2_18_2_21_10_623 "ExternalSortFile, InitMergeFiles success!"
#define MMIVIRTUALARRAY_1550_112_2_18_2_21_10_624 "ExternalSortFile, MultiWayMerge success!"
#define MMIVIRTUALARRAY_1563_112_2_18_2_21_10_625 "ExternalSortFile, MultiWayMerge fail!"
#define MMIVIRTUALARRAY_1599_112_2_18_2_21_10_626 "InitMergeFiles v_header_ptr 0x%x"
#define MMIVIRTUALARRAY_1607_112_2_18_2_21_10_627 "MMIFMM:InitMergeFiles ERROR! PNULL == wa_buf"
#define MMIVIRTUALARRAY_1715_112_2_18_2_21_10_628 "MMIFMM:[MultiWayMerge] PNULL == m_fi_ptr"
#define MMIVIRTUALARRAY_1721_112_2_18_2_21_10_629 "MMIFMM:[MultiWayMerge] PNULL == compare_func"
#define MMIVIRTUALARRAY_1811_112_2_18_2_21_10_630 "MMIFMM:[DestroyMergeFiles] PNULL == m_file_ptr"
#define MMIVIRTUALARRAY_1874_112_2_18_2_21_11_631 "MMIFMM:ReplaceSelSort PNULL == wa_num"
#define MMIVIRTUALARRAY_1880_112_2_18_2_21_11_632 "MMIFMM:ReplaceSelSort param error!"
#define MMIVIRTUALARRAY_1888_112_2_18_2_21_11_633 "MMIFMM:ReplaceSelSort error:PNULL == switch_data"
#define MMIVIRTUALARRAY_1948_112_2_18_2_21_11_634 "MMIFMM:GetNextMergeGroup ways = %d"
#define MMIVIRTUALARRAY_1998_112_2_18_2_21_11_635 "MMIFMM:Merge param error!"
#define MMIVIRTUALARRAY_2008_112_2_18_2_21_11_636 "MMIFMM:Merge 0 == data_size"
#define MMIVIRTUALARRAY_2250_112_2_18_2_21_11_637 "MMIFMM:UpdateAssoFile PNULL == v_header_ptr"
#define MMIVIRTUALARRAY_2349_112_2_18_2_21_12_638 "MMIFMM:DeleteBufFromFile PNULL == buffer_ptr"
#define MMIVIRTUALARRAY_SQL_114_112_2_18_2_21_13_639 "GetFilePathBySapce, handle = 0!"
#define MMIVIRTUALARRAY_SQL_154_112_2_18_2_21_13_640 "CreateAssoFile, handle = 0!"
#define MMIVIRTUALARRAY_SQL_217_112_2_18_2_21_13_641 "CreateTable, handle = 0!"
#define MMIVIRTUALARRAY_SQL_224_112_2_18_2_21_13_642 "CreateTable, create talbe fail."
#define MMIVIRTUALARRAY_SQL_234_112_2_18_2_21_13_643 "CreateTable, create index fail."
#define MMIVIRTUALARRAY_SQL_259_112_2_18_2_21_13_644 "GetSize, handle = 0!"
#define MMIVIRTUALARRAY_SQL_298_112_2_18_2_21_13_645 "IfReachLimitCount, handle = 0!"
#define MMIVIRTUALARRAY_SQL_314_112_2_18_2_21_13_646 "IfReachLimitCount, invalid mode!"
#define MMIVIRTUALARRAY_SQL_639_112_2_18_2_21_14_647 "ArrayCombine, sqlite3_exec insert error!"
#define MMIVIRTUALARRAY_SQL_673_112_2_18_2_21_14_648 "MMIVIRTUALARRAY_Create, no enough memory!"
#define MMIVIRTUALARRAY_SQL_682_112_2_18_2_21_14_649 "MMIVIRTUALARRAY_Create, GetFilePathBySapce error!"
#define MMIVIRTUALARRAY_SQL_711_112_2_18_2_21_14_650 "MMIVIRTUALARRAY_Create, error!"
#define MMIVIRTUALARRAY_SQL_777_112_2_18_2_21_14_651 "MMIVIRTUALARRAY_GetArraySize, handle = 0!"
#define MMIVIRTUALARRAY_SQL_838_112_2_18_2_21_14_652 "MMIVIRTUALARRAY_AddItem, handle = 0!"
#define MMIVIRTUALARRAY_SQL_844_112_2_18_2_21_14_653 "MMIVIRTUALARRAY_AddItem, data_ptr = 0!"
#define MMIVIRTUALARRAY_SQL_866_112_2_18_2_21_14_654 "MMIVIRTUALARRAY_AddItem, error:%s."
#define MMIVIRTUALARRAY_SQL_893_112_2_18_2_21_14_655 "MMIVIRTUALARRAY_RemoveItemById, handle = 0!"
#define MMIVIRTUALARRAY_SQL_900_112_2_18_2_21_14_656 "MMIVIRTUALARRAY_RemoveItemById, invalid id."
#define MMIVIRTUALARRAY_SQL_916_112_2_18_2_21_14_657 "MMIVIRTUALARRAY_RemoveItemById, error:%s."
#define MMIVIRTUALARRAY_SQL_948_112_2_18_2_21_14_658 "MMIVIRTUALARRAY_UpdateItemById, handle = 0!"
#define MMIVIRTUALARRAY_SQL_954_112_2_18_2_21_14_659 "MMIVIRTUALARRAY_UpdateItemById, data_ptr = 0!"
#define MMIVIRTUALARRAY_SQL_961_112_2_18_2_21_14_660 "MMIVIRTUALARRAY_UpdateItemById, invalid id."
#define MMIVIRTUALARRAY_SQL_977_112_2_18_2_21_14_661 "MMIVIRTUALARRAY_UpdateItemById, error:%s."
#define MMIVIRTUALARRAY_SQL_1007_112_2_18_2_21_14_662 "MMIVIRTUALARRAY_GetItemById, handle = 0!"
#define MMIVIRTUALARRAY_SQL_1013_112_2_18_2_21_14_663 "MMIVIRTUALARRAY_GetItemById, data_ptr = 0!"
#define MMIVIRTUALARRAY_SQL_1020_112_2_18_2_21_14_664 "MMIVIRTUALARRAY_GetItemById, invalid id."
#define MMIVIRTUALARRAY_SQL_1060_112_2_18_2_21_15_665 "MMIVIRTUALARRAY_GetItemById, error:%s."
#define MMIVIRTUALARRAY_SQL_1145_112_2_18_2_21_15_666 "MMIVIRTUALARRAY_Combine, handle = 0!"
#define MMIVIRTUALARRAY_SQL_1158_112_2_18_2_21_15_667 "MMIVIRTUALARRAY_Combine, array1 reach limit count!"
#define MMIVIRTUALARRAY_SQL_1192_112_2_18_2_21_15_668 "MMIVIRTUALARRAY_SaveToFile, input param error!"
#define MMIVIRTUALARRAY_SQL_1199_112_2_18_2_21_15_669 "MMIVIRTUALARRAY_SaveToFile, create file error!"
#define MMIVIRTUALARRAY_SQL_1230_112_2_18_2_21_15_670 "MMIVIRTUALARRAY_SaveToFile, attach error:%s!"
#define MMIVIRTUALARRAY_SQL_1252_112_2_18_2_21_15_671 "MMIVIRTUALARRAY_LoadFromFile, input param error!"
#define MMIVIRTUALARRAY_SQL_1266_112_2_18_2_21_15_672 "MMIVIRTUALARRAY_LoadFromFile, copy file error!"
#define MMIVIRTUALARRAY_SQL_1279_112_2_18_2_21_15_673 "MMIVIRTUALARRAY_LoadFromFile, FILE_MODE open db error!"
#define MMIVIRTUALARRAY_SQL_1292_112_2_18_2_21_15_674 "MMIVIRTUALARRAY_LoadFromFile, MEMORY_MODE open db error!"
#define MMIVIRTUALARRAY_SQL_1319_112_2_18_2_21_15_675 "MMIVIRTUALARRAY_Transaction, handle = 0!"
#define MMIVIRTUALARRAY_SQL_1346_112_2_18_2_21_15_676 "sqlite_memory_status, nCur = %d,nHigh = %d."
#define MMIVIRTUALARRAY_SQL_1353_112_2_18_2_21_15_677 "sqlite free all."
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_FMM_TRC)
TRACE_MSG(MMI_FILEMGR_281_112_2_18_2_19_47_0,"MMIAPIFMM_SplitFullPath ERROR! full_path_len = %d")
TRACE_MSG(MMI_FILEMGR_289_112_2_18_2_19_47_1,"MMIAPIFMM_SplitFullPath param ERROR PNULL == full_path_ptr! ")
TRACE_MSG(MMI_FILEMGR_295_112_2_18_2_19_47_2,"MMIAPIFMM_SplitFullPath param ERROR! ")
TRACE_MSG(MMI_FILEMGR_321_112_2_18_2_19_47_3,"MMIAPIFMM_SplitFullPath, ':' not found!")
TRACE_MSG(MMI_FILEMGR_335_112_2_18_2_19_47_4,"MMIAPIFMM_SplitFullPath, '\' not found after device!")
TRACE_MSG(MMI_FILEMGR_417_112_2_18_2_19_47_5,"MMIAPIFMM_SplitFullPathExt param ERROR!")
TRACE_MSG(MMI_FILEMGR_438_112_2_18_2_19_47_6,"MMIAPIFMM_SplitFullPathExt ERROR! PNULL == suffix_len_ptr")
TRACE_MSG(MMI_FILEMGR_465_112_2_18_2_19_47_7,"MMIAPIFMM_SplitFullPathExt ERROR! PNULL != name_ptr,PNULL == name_len_ptr")
TRACE_MSG(MMI_FILEMGR_487_112_2_18_2_19_47_8,"MMIAPIFMM_SplitFullPathExt ERROR! PNULL != path_ptr,PNULL == path_len_ptr")
TRACE_MSG(MMI_FILEMGR_520_112_2_18_2_19_48_9,"MMIAPIFMM_SplitFullPath ERROR! full_path_len = %d")
TRACE_MSG(MMI_FILEMGR_528_112_2_18_2_19_48_10,"MMIFILE_FileIsInThisFolder param ERROR PNULL == full_path_ptr! ")
TRACE_MSG(MMI_FILEMGR_534_112_2_18_2_19_48_11,"MMIFILE_FileIsInThisFolder param ERROR! ")
TRACE_MSG(MMI_FILEMGR_590_112_2_18_2_19_48_12,"[MMIFMM]: MMIAPIFMM_CombineFullPathEx() param error,PNULL == abs_path_ptr!")
TRACE_MSG(MMI_FILEMGR_646_112_2_18_2_19_48_13,"MMIAPIFMM_CombineFullPathEx ERROR! abs_path_total_len = %d,abs_path_len = %d")
TRACE_MSG(MMI_FILEMGR_682_112_2_18_2_19_48_14,"MMIAPIFMM_CombineFullPath para error!")
TRACE_MSG(MMI_FILEMGR_795_112_2_18_2_19_48_15,"MMIAPIFMM_CombineFullPath error! j = %d, full_path_len = %d")
TRACE_MSG(MMI_FILEMGR_1046_112_2_18_2_19_49_16,"MMIAPIFMM_RegisterDevice param error !")
TRACE_MSG(MMI_FILEMGR_1130_112_2_18_2_19_49_17,"MMIAPIFMM_UnRegisterDevice param ERROR!")
TRACE_MSG(MMI_FILEMGR_1147_112_2_18_2_19_49_18,"MMIAPIFMM_UnRegisterDevice unregister device error")
TRACE_MSG(MMI_FILEMGR_1186_112_2_18_2_19_49_19,"MMIAPIFMM_FormatDevice param ERROR!")
TRACE_MSG(MMI_FILEMGR_1210_112_2_18_2_19_49_20,"MMIAPIFMM_FormatDevice ERROR! fs_format = %d")
TRACE_MSG(MMI_FILEMGR_1277_112_2_18_2_19_49_21,"FormatDeviceAsynCallBack param error:PNULL == irp_param")
TRACE_MSG(MMI_FILEMGR_1282_112_2_18_2_19_49_22,"MMIAPIFMM_FormatDeviceAsyn: format device callback, error = %d")
TRACE_MSG(MMI_FILEMGR_1333_112_2_18_2_19_49_23,"MMIAPIFMM_FormatDeviceAsyn param ERROR!")
TRACE_MSG(MMI_FILEMGR_1355_112_2_18_2_19_49_24,"MMIFMM:MMIAPIFMM_FormatDeviceAsyn format = %d is error!")
TRACE_MSG(MMI_FILEMGR_1389_112_2_18_2_19_49_25,"MMIFMM:MMIAPIFMM_FormatDeviceAsyn PNULL == callback_ptr is error!")
TRACE_MSG(MMI_FILEMGR_1468_112_2_18_2_19_49_26,"MMIAPIFMM_GetDeviceStatus param ERROR!")
TRACE_MSG(MMI_FILEMGR_1522_112_2_18_2_19_50_27,"MMIAPIFMM_CreateDir param ERROR!")
TRACE_MSG(MMI_FILEMGR_1528_112_2_18_2_19_50_28,"MMIAPIFMM_CreateDir: create dir success")
TRACE_MSG(MMI_FILEMGR_1533_112_2_18_2_19_50_29,"MMIAPIFMM_CreateDir: create dir fail")
TRACE_MSG(MMI_FILEMGR_1568_112_2_18_2_19_50_30,"[MMIFILE] SetAttr NO memory !")
TRACE_MSG(MMI_FILEMGR_1620_112_2_18_2_19_50_31,"MMIAPIFMM_GetFileInfo ERROR:PNULL == full_path_ptr")
TRACE_MSG(MMI_FILEMGR_1700_112_2_18_2_19_50_32,"MMIAPIFMM_IsFileExist error:PNULL == full_path_ptr!")
TRACE_MSG(MMI_FILEMGR_1721_112_2_18_2_19_50_33,"MMIFMM_IsFilesInFolder: result = %d")
TRACE_MSG(MMI_FILEMGR_1748_112_2_18_2_19_50_34,"MMIAPIFMM_GetDeviceFreeSpace param error!")
TRACE_MSG(MMI_FILEMGR_1781_112_2_18_2_19_50_35,"MMIAPIFMM_GetDeviceUsedSpace error!")
TRACE_MSG(MMI_FILEMGR_1819_112_2_18_2_19_50_36,"MMIAPIFMM_RenameFile full_path_len %d new_file_name_len %d !!")
TRACE_MSG(MMI_FILEMGR_1907_112_2_18_2_19_50_37,"StoreFindFileData param error!")
TRACE_MSG(MMI_FILEMGR_1964_112_2_18_2_19_50_38,"MMIFMM_FindAndGetFiles param error!")
TRACE_MSG(MMI_FILEMGR_1978_112_2_18_2_19_50_39,"MMIFMM_FindAndGetFiles: find first success")
TRACE_MSG(MMI_FILEMGR_2016_112_2_18_2_19_51_40,"MMIFMM_FindAndGetFiles: find file number = %d")
TRACE_MSG(MMI_FILEMGR_2042_112_2_18_2_19_51_41,"MMIAPIFMM_ReadFilesDataSyn param error!")
TRACE_MSG(MMI_FILEMGR_2048_112_2_18_2_19_51_42,"MMIAPIFMM_ReadFilesDataSyn: read file 0")
TRACE_MSG(MMI_FILEMGR_2066_112_2_18_2_19_51_43,"MMIAPIFMM_ReadFilesDataSyn: return success")
TRACE_MSG(MMI_FILEMGR_2070_112_2_18_2_19_51_44,"MMIAPIFMM_ReadFilesDataSyn: return fail")
TRACE_MSG(MMI_FILEMGR_2097_112_2_18_2_19_51_45,"MMIAPIFMM_ReadFilesDataAsynEx: read file 0")
TRACE_MSG(MMI_FILEMGR_2113_112_2_18_2_19_51_46,"MMIAPIFMM_ReadFilesDataAsynEx: return success")
TRACE_MSG(MMI_FILEMGR_2117_112_2_18_2_19_51_47,"MMIAPIFMM_ReadFilesDataAsynEx: return fail")
TRACE_MSG(MMI_FILEMGR_2144_112_2_18_2_19_51_48,"MMIAPIFMM_WriteFilesDataSyn param error!")
TRACE_MSG(MMI_FILEMGR_2179_112_2_18_2_19_51_49,"MMIAPIFMM_DeleteFileSyn param error!")
TRACE_MSG(MMI_FILEMGR_2205_112_2_18_2_19_51_50,"[MMIFMM] MMIAPIFMM_CopyFile invalid file name !")
TRACE_MSG(MMI_FILEMGR_2212_112_2_18_2_19_51_51,"[MMIFMM] MMIAPIFMM_CopyFile no memory!")
TRACE_MSG(MMI_FILEMGR_2226_112_2_18_2_19_51_52,"[MMIFMM] MMIAPIFMM_CopyFile open file error !")
TRACE_MSG(MMI_FILEMGR_2240_112_2_18_2_19_51_53,"[MMIFILE] MMIAPIFMM_CopyFile MMIAPIFMM_ReadFile copy_ret %d")
TRACE_MSG(MMI_FILEMGR_2246_112_2_18_2_19_51_54,"[MMIFILE] MMIAPIFMM_CopyFile MMIAPIFMM_WriteFile copy_ret %d")
TRACE_MSG(MMI_FILEMGR_2253_112_2_18_2_19_51_55,"[MMIFILE] MMIAPIFMM_CopyFile copy_ret %d")
TRACE_MSG(MMI_FILEMGR_2277_112_2_18_2_19_51_56,"MMIAPIFMM_ReadFileDataByOffsetSyn param error!")
TRACE_MSG(MMI_FILEMGR_2282_112_2_18_2_19_51_57,"MMIAPIFMM_ReadFileDataByOffsetSyn: read file 0")
TRACE_MSG(MMI_FILEMGR_2303_112_2_18_2_19_51_58,"MMIAPIFMM_ReadFileDataByOffsetSyn: read file data success")
TRACE_MSG(MMI_FILEMGR_2307_112_2_18_2_19_51_59,"MMIAPIFMM_ReadFileDataByOffsetSyn: read file data fail")
TRACE_MSG(MMI_FILEMGR_2335_112_2_18_2_19_51_60,"MMIFMM_ReadFileDataByOffsetSynEx: read file 0")
TRACE_MSG(MMI_FILEMGR_2354_112_2_18_2_19_51_61,"MMIFMM_ReadFileDataByOffsetSynEx: read file data success")
TRACE_MSG(MMI_FILEMGR_2358_112_2_18_2_19_51_62,"MMIFMM_ReadFileDataByOffsetSynEx: read file data fail")
TRACE_MSG(MMI_FILEMGR_2386_112_2_18_2_19_51_63,"MMIFILE_WriteFileDataByOffsetSyn param error!")
TRACE_MSG(MMI_FILEMGR_2407_112_2_18_2_19_51_64,"MMIFILE_WriteFileDataByOffsetSyn: write file data success")
TRACE_MSG(MMI_FILEMGR_2411_112_2_18_2_19_51_65,"MMIFILE_WriteFileDataByOffsetSyn: write file data fail")
TRACE_MSG(MMI_FILEMGR_2434_112_2_18_2_19_51_66,"ReadFileDataAsynCallback param error:PNULL == irp_param")
TRACE_MSG(MMI_FILEMGR_2478_112_2_18_2_19_51_67,"MMIAPIFMM_ReadFilesDataAsyn param error!")
TRACE_MSG(MMI_FILEMGR_2483_112_2_18_2_19_51_68,"MMIAPIFMM_ReadFilesDataAsyn param error: win_id = %d,ctrl_id = %d,msg_id = %d")
TRACE_MSG(MMI_FILEMGR_2503_112_2_18_2_19_52_69,"MMIAPIFMM_ReadFilesDataAsyn: read file 0")
TRACE_MSG(MMI_FILEMGR_2515_112_2_18_2_19_52_70,"MMIFMM:MMIAPIFMM_ReadFilesDataAsyn PNULL == callback_ptr is error!")
TRACE_MSG(MMI_FILEMGR_2529_112_2_18_2_19_52_71,"MMIAPIFMM_ReadFilesDataAsyn: read file success")
TRACE_MSG(MMI_FILEMGR_2534_112_2_18_2_19_52_72,"MMIAPIFMM_ReadFilesDataAsyn: read file fail")
TRACE_MSG(MMI_FILEMGR_2600_112_2_18_2_19_52_73,"MMIAPIFMM_ReadFilesDataAsynEx: read file 0")
TRACE_MSG(MMI_FILEMGR_2619_112_2_18_2_19_52_74,"MMIAPIFMM_ReadFilesDataAsynEx: read file success")
TRACE_MSG(MMI_FILEMGR_2624_112_2_18_2_19_52_75,"MMIAPIFMM_ReadFilesDataAsynEx: read file fail")
TRACE_MSG(MMI_FILEMGR_2649_112_2_18_2_19_52_76,"WriteFileDataAsynCallback param error:PNULL == irp_param")
TRACE_MSG(MMI_FILEMGR_2652_112_2_18_2_19_52_77,"WriteFileDataAsynCallback: sfs error = %d")
TRACE_MSG(MMI_FILEMGR_2693_112_2_18_2_19_52_78,"MMIAPIFMM_WriteFilesDataAsyn param error!")
TRACE_MSG(MMI_FILEMGR_2698_112_2_18_2_19_52_79,"MMIAPIFMM_WriteFilesDataAsyn param error: win_id = %d,msg_id = %d")
TRACE_MSG(MMI_FILEMGR_2710_112_2_18_2_19_52_80,"MMIAPIFMM_WriteFilesDataAsyn PNULL == callback_ptr is error!")
TRACE_MSG(MMI_FILEMGR_2723_112_2_18_2_19_52_81,"MMIAPIFMM_WriteFilesDataAsyn: write file success")
TRACE_MSG(MMI_FILEMGR_2728_112_2_18_2_19_52_82,"MMIAPIFMM_WriteFilesDataAsyn: write file fail")
TRACE_MSG(MMI_FILEMGR_2735_112_2_18_2_19_52_83,"MMIAPIFMM_WriteFilesDataAsyn: create file fail")
TRACE_MSG(MMI_FILEMGR_2757_112_2_18_2_19_52_84,"ReadFileDataByOffsetAsynCallback param error:PNULL == irp_param")
TRACE_MSG(MMI_FILEMGR_2800_112_2_18_2_19_52_85,"MMIFILE_ReadFileDataByOffsetAsyn param error!")
TRACE_MSG(MMI_FILEMGR_2805_112_2_18_2_19_52_86,"MMIFILE_ReadFileDataByOffsetAsyn param error:win_id = %d,msg_id = %d")
TRACE_MSG(MMI_FILEMGR_2814_112_2_18_2_19_52_87,"MMIFILE_ReadFileDataByOffsetAsyn: read file 0")
TRACE_MSG(MMI_FILEMGR_2828_112_2_18_2_19_52_88,"MMIFILE_ReadFileDataByOffsetAsyn PNULL == callback_ptr is error!")
TRACE_MSG(MMI_FILEMGR_2857_112_2_18_2_19_52_89,"MMIFILE_ReadFileDataByOffsetAsyn: return success")
TRACE_MSG(MMI_FILEMGR_2861_112_2_18_2_19_52_90,"MMIFILE_ReadFileDataByOffsetAsyn: return fail")
TRACE_MSG(MMI_FILEMGR_2910_112_2_18_2_19_52_91,"MMIFILE_ReadFileDataByOffsetAsynEx: read file 0")
TRACE_MSG(MMI_FILEMGR_2943_112_2_18_2_19_52_92,"MMIFILE_ReadFileDataByOffsetAsynEx: return success")
TRACE_MSG(MMI_FILEMGR_2947_112_2_18_2_19_52_93,"MMIFILE_ReadFileDataByOffsetAsynEx: return fail")
TRACE_MSG(MMI_FILEMGR_2969_112_2_18_2_19_52_94,"WriteFileDataByOffsetAsynCallback param error:PNULL == irp_param")
TRACE_MSG(MMI_FILEMGR_2973_112_2_18_2_19_52_95,"WriteFileDataAsynCallback: sfs error = %d")
TRACE_MSG(MMI_FILEMGR_3014_112_2_18_2_19_53_96,"MMIAPIFMM_WriteFileDataByOffsetAsyn param error!")
TRACE_MSG(MMI_FILEMGR_3019_112_2_18_2_19_53_97,"MMIAPIFMM_WriteFileDataByOffsetAsyn param error: win_id = %d,msg_id = %d")
TRACE_MSG(MMI_FILEMGR_3033_112_2_18_2_19_53_98,"[MMIFMM]:MMIAPIFMM_WriteFileDataByOffsetAsyn PNULL == callback_ptr error!")
TRACE_MSG(MMI_FILEMGR_3062_112_2_18_2_19_53_99,"MMIAPIFMM_WriteFileDataByOffsetAsyn: return success")
TRACE_MSG(MMI_FILEMGR_3066_112_2_18_2_19_53_100,"MMIAPIFMM_WriteFileDataByOffsetAsyn: return fail")
TRACE_MSG(MMI_FILEMGR_3089_112_2_18_2_19_53_101,"MMIAPIFMM_OperationAsynCnf param error: PNULL == param_ptr")
TRACE_MSG(MMI_FILEMGR_3114_112_2_18_2_19_53_102,"MMIAPIFMM_OperationAsynCnf: result = %d")
TRACE_MSG(MMI_FILEMGR_3136_112_2_18_2_19_53_103,"DeleteFileAsynCallback param error:PNULL == irp_param")
TRACE_MSG(MMI_FILEMGR_3141_112_2_18_2_19_53_104,"DeleteFileAsynCallback: error = %d")
TRACE_MSG(MMI_FILEMGR_3175_112_2_18_2_19_53_105,"CreateFileAsynCallback param error:PNULL == irp_param")
TRACE_MSG(MMI_FILEMGR_3181_112_2_18_2_19_53_106,"CreateFileAsynCallback: error = %d")
TRACE_MSG(MMI_FILEMGR_3217_112_2_18_2_19_53_107,"MMIAPIFMM_DeleteFileAsyn param error!")
TRACE_MSG(MMI_FILEMGR_3222_112_2_18_2_19_53_108,"MMIAPIFMM_DeleteFileAsyn param error: win_id = %d,msg_id = %d")
TRACE_MSG(MMI_FILEMGR_3236_112_2_18_2_19_53_109,"MMIFMM:MMIAPIFMM_DeleteFileAsyn PNULL == callback_ptr error!")
TRACE_MSG(MMI_FILEMGR_3249_112_2_18_2_19_53_110,"MMIAPIFMM_DeleteFileAsyn: delete asyn success")
TRACE_MSG(MMI_FILEMGR_3283_112_2_18_2_19_53_111,"MMIAPIFMM_CopyOneFile param error!")
TRACE_MSG(MMI_FILEMGR_3304_112_2_18_2_19_53_112,"MMIFMM:MMIAPIFMM_CopyOneFile PNULL == s_copy_info.buf_ptr is error!")
TRACE_MSG(MMI_FILEMGR_3320_112_2_18_2_19_53_113,"MMIAPIFMM_CopyOneFile: create src file and dest file success")
TRACE_MSG(MMI_FILEMGR_3336_112_2_18_2_19_53_114,"MMIAPIFMM_CopyOneFile: create src file and dest file fail")
TRACE_MSG(MMI_FILEMGR_3404_112_2_18_2_19_53_115,"MMIAPIFMM_CopyReadSrc: copy one file finished with success")
TRACE_MSG(MMI_FILEMGR_3408_112_2_18_2_19_53_116,"MMIAPIFMM_CopyReadSrc: have copied data size = %d, src file size = %d")
TRACE_MSG(MMI_FILEMGR_3431_112_2_18_2_19_53_117,"MMIAPIFMM_CopyReadSrc:  fail")
TRACE_MSG(MMI_FILEMGR_3499_112_2_18_2_19_53_118,"MMIAPIFMM_CopyWriteDest: read src file success, data size = %d")
TRACE_MSG(MMI_FILEMGR_3515_112_2_18_2_19_54_119,"MMIAPIFMM_CopyWriteDest:  fail")
TRACE_MSG(MMI_FILEMGR_3576_112_2_18_2_19_54_120,"MMIAPIFMM_DeleteTree pathName 0x%x , fileName 0x%x")
TRACE_MSG(MMI_FILEMGR_3687_112_2_18_2_19_54_121,"MMIAPIFMM_CreateFileAsyn param error: PNULL == file_name")
TRACE_MSG(MMI_FILEMGR_3709_112_2_18_2_19_54_122,"MMIFMM:MMIAPIFMM_CreateFileAsyn PNULL == callback_ptr is error!")
TRACE_MSG(MMI_FILEMGR_3757_112_2_18_2_19_54_123,"MMIAPIFMM_CreateDirectory param error:PNULL == path")
TRACE_MSG(MMI_FILEMGR_3838_112_2_18_2_19_54_124,"MMIFILE_SetFileSize param error: PNULL == file_handle")
TRACE_MSG(MMI_FILEMGR_3892_112_2_18_2_19_54_125,"MMIAPIFMM_SetFilePointer param error:PNULL == file_handle")
TRACE_MSG(MMI_FILEMGR_3919_112_2_18_2_19_54_126,"MMIAPIFMM_GetFilePointer param error:PNULL == file_handle")
TRACE_MSG(MMI_FILEMGR_3976_112_2_18_2_19_54_127,"mmi_filemgr.c, MMIAPIFMM_IsEndOfFile(), cur_pos = %X, file_total_size = %X")
TRACE_MSG(MMI_FILEMGR_4033_112_2_18_2_19_55_128,"[MMI FILE]: MMIAPIFMM_GetValidateDevice, ret_dev %x")
TRACE_MSG(MMI_FILEMGR_4043_112_2_18_2_19_55_129,"[MMIFILE]: MMIAPIFMM_Callback result = %d")
TRACE_MSG(MMI_FILEMGR_4094_112_2_18_2_19_55_130,"[mmi_filemgr.c]:MMIAPIFMM_GetDefaultDisk dev_type=%d")
TRACE_MSG(MMI_FILEMGR_4143_112_2_18_2_19_55_131,"[mmi_filemgr.c]:GetAvailableDevice available device=%d")
TRACE_MSG(MMI_FILEMGR_4173_112_2_18_2_19_55_132,"[mmi_filemgr.c]:MMIAPIFMM_IsEnoughSpace is not enough space!")
TRACE_MSG(MMI_FILEMGR_4471_112_2_18_2_19_55_133,"[mmi_filemgr.c]:MMIAPIFMM_CreateSysFileDir sysfile folder not exist!!")
TRACE_MSG(MMI_FILEMGR_4478_112_2_18_2_19_55_134,"MMIAPIFMM_CreateSysFileDir: create dir fail")
TRACE_MSG(MMI_FILEMGR_4594_112_2_18_2_19_56_135,"[MMIFILE] MMIAPIFMM_AppendFile file name invalid !")
TRACE_MSG(MMI_FILEMGR_4820_112_2_18_2_19_56_136,"[MMIFILE] MMIAPIFMM_GetSuitableFileDev user_sel_dev %d min_free_space %d return dev %d file_ret %d")
TRACE_MSG(MMI_FILEMGR_5194_112_2_18_2_19_57_137,"MMIFILE_CreateFile param error: PNULL == file_name")
TRACE_MSG(MMI_FILEMGR_5230_112_2_18_2_19_57_138,"MMIAPIFMM_FindNextFile param error!")
TRACE_MSG(MMI_FILEMGR_5253_112_2_18_2_19_57_139,"MMIAPIFMM_DeleteFile param error:PNULL == name")
TRACE_MSG(MMI_FILEMGR_5278_112_2_18_2_19_57_140,"MMIAPIFMM_DeleteDirectory param error:PNULL == path")
TRACE_MSG(MMI_FILEMGR_5303_112_2_18_2_19_57_141,"MMIAPIFMM_FindFirstFile param error!")
TRACE_MSG(MMI_FILEMGR_5312_112_2_18_2_19_57_142,"MMIAPIFMM_FindFirstFile find_len = %d")
TRACE_MSG(MMI_FILEMGR_5348_112_2_18_2_19_57_143,"MMIAPIFMM_GetFileSize param error: PNULL == file_handle")
TRACE_MSG(MMI_FILEMGR_5379_112_2_18_2_19_57_144,"MMIAPIFMM_ReadFile param error!")
TRACE_MSG(MMI_FILEMGR_5414_112_2_18_2_19_57_145,"MMIAPIFMM_WriteFile param error!")
TRACE_MSG(MMI_FILEMGR_5445_112_2_18_2_19_57_146,"MMIAPIFMM_CloseFile file_handle %d !")
TRACE_MSG(MMI_FILEMGR_5467_112_2_18_2_19_57_147,"MMIAPIFMM_IsFileExist error:PNULL == full_path_ptr!")
TRACE_MSG(MMI_FILEMGR_5480_112_2_18_2_19_57_148,"MMIAPIFMM_IsFileExist: find first sfs_handle = %d")
TRACE_MSG(MMI_FILEMGR_5493_112_2_18_2_19_57_149,"MMIAPIFMM_IsFileExist: result = %d")
TRACE_MSG(MMI_FILEMGR_5516_112_2_18_2_19_57_150,"MMIAPIFMM_IsFolderExist error:PNULL == full_path_ptr!")
TRACE_MSG(MMI_FILEMGR_5540_112_2_18_2_19_58_151,"MMIAPIFMM_IsFolderExist: find first sfs_handle = %d")
TRACE_MSG(MMI_FILEMGR_5553_112_2_18_2_19_58_152,"MMIAPIFMM_IsFolderExist: result = %d")
TRACE_MSG(MMI_FILETASK_179_112_2_18_2_19_58_153,"APPFILE_CreateTask, file task hasn't been closed!")
TRACE_MSG(MMI_FILETASK_195_112_2_18_2_19_58_154,"APPFILE_CreateTask, g_app_file_task_id = %d")
TRACE_MSG(MMI_FILETASK_204_112_2_18_2_19_58_155,"APPFILE_HandleSearchFileCnf search_result=%d,s_is_stopfindfile=%d")
TRACE_MSG(MMI_FILETASK_288_112_2_18_2_19_58_156,"APP_COPY_Task: SigCode unknown")
TRACE_MSG(MMI_FILETASK_317_112_2_18_2_19_58_157,"[MMIFILE] APPFILE_AbortCopy ")
TRACE_MSG(MMI_FILETASK_330_112_2_18_2_19_58_158,"[MMIFILE] APPFILE_CopyClose ")
TRACE_MSG(MMI_FILETASK_342_112_2_18_2_19_58_159,"[MMIFILE] MMIFILE_DeleteOpen enter")
TRACE_MSG(MMI_FILETASK_345_112_2_18_2_19_58_160,"MMIFILE MMIFILE_DeleteOpen del_file_name 0x&x del_name_len %d !")
TRACE_MSG(MMI_FILETASK_353_112_2_18_2_19_58_161,"MMIFILE MMIFILE_DeleteOpen del_handle_ptr 0x%x !")
TRACE_MSG(MMI_FILETASK_384_112_2_18_2_19_58_162,"[MMIFILE] MMIFILE_DeleteClose fs_ret %d")
TRACE_MSG(MMI_FILETASK_394_112_2_18_2_19_58_163,"[MMIFILE] MMIFILE_AbortDelete enter")
TRACE_MSG(MMI_FILETASK_398_112_2_18_2_19_59_164,"MMIFILE_AbortDelete del_handle_ptr 0x&x !")
TRACE_MSG(MMI_FILETASK_451_112_2_18_2_19_59_165,"[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_FILES enter")
TRACE_MSG(MMI_FILETASK_477_112_2_18_2_19_59_166,"[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_ONE_FILE_IND user stop %d")
TRACE_MSG(MMI_FILETASK_549_112_2_18_2_19_59_167,"[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_ONE_FOLDER_IND user stop %d")
TRACE_MSG(MMI_FILETASK_585_112_2_18_2_19_59_168,"[MMIFILE] APPFILE_HandleDeleteSig APP_DEL_STOP user stop %d")
TRACE_MSG(MMI_FILETASK_633_112_2_18_2_19_59_169,"APPFILE_HandleCopySig param error:PNULL == psig")
TRACE_MSG(MMI_FILETASK_1000_112_2_18_2_20_0_170,"APP_COPY_Task: SigCode unknown")
TRACE_MSG(MMI_FILETASK_1077_112_2_18_2_20_0_171,"[MMIFILE] MMIFILE_DeleteDirTree enter ")
TRACE_MSG(MMI_FILETASK_1158_112_2_18_2_20_0_172,"[MMIFILE] MMIFILE_DeleteSubFileAsyn enter ")
TRACE_MSG(MMI_FILETASK_1162_112_2_18_2_20_0_173,"[MMIFILE] MMIFILE_DeleteAsyn del_handle_ptr 0x%x file_name 0x%x file_name_len %d !")
TRACE_MSG(MMI_FILETASK_1212_112_2_18_2_20_0_174,"MMIFILE_DeleteSubFileAsyn: delete asyn success")
TRACE_MSG(MMI_FILETASK_1224_112_2_18_2_20_0_175,"MMIFILE_DeleteSubFileAsyn file name too long !")
TRACE_MSG(MMI_FILETASK_1263_112_2_18_2_20_0_176,"[MMIFILE] MMIFILE_DeleteSubFolder enter ")
TRACE_MSG(MMI_FILETASK_1266_112_2_18_2_20_0_177,"[MMIFILE] MMIFILE_DeleteAsyn del_handle_ptr 0x%x !")
TRACE_MSG(MMI_FILETASK_1276_112_2_18_2_20_0_178,"MMIFILE_DeleteSubFolder alloc find_data_ptr error:PNULL == find_data_ptr")
TRACE_MSG(MMI_FILETASK_1353_112_2_18_2_20_0_179,"[MMIFILE]FileDeleteCallback error %d")
TRACE_MSG(MMI_FILETASK_1416_112_2_18_2_20_1_180,"[MMIFILE] FindOnlyFiles enter ")
TRACE_MSG(MMI_FILETASK_1473_112_2_18_2_20_1_181,"[MMIFILE] FindOnlyFolders enter ")
TRACE_MSG(MMI_FILETASK_1555_112_2_18_2_20_1_182,"[MMIFILE] MMIFILE_CopyOpen enter")
TRACE_MSG(MMI_FILETASK_1558_112_2_18_2_20_1_183,"MMIFILE MMIFILE_CopyOpen src_file_name 0x&x src_name_len %d !")
TRACE_MSG(MMI_FILETASK_1564_112_2_18_2_20_1_184,"MMIFILE MMIFILE_CopyOpen dest_file_name 0x&x dest_file_name_len %d !")
TRACE_MSG(MMI_FILETASK_1571_112_2_18_2_20_1_185,"MMIFILE MMIFILE_CopyOpen handle_ptr 0x%x !")
TRACE_MSG(MMI_FILETASK_1736_112_2_18_2_20_1_186,"[MMIFILE] FileCopy invalid file name !")
TRACE_MSG(MMI_FILETASK_1785_112_2_18_2_20_1_187,"[MMIFILE] FileCopy no memory!")
TRACE_MSG(MMI_FILETASK_1831_112_2_18_2_20_1_188,"[MMIFILE] FileCopy open file error !")
TRACE_MSG(MMI_FILETASK_1925_112_2_18_2_20_2_189,"[MMIFILE] FileCopy copy_ret %d handle_ptr->copy_offset %d filesize %d")
TRACE_MSG(MMI_FILETASK_1946_112_2_18_2_20_2_190,"[[MMIFMM]: MMIFILE_MoveOpen enter")
TRACE_MSG(MMI_FILETASK_1949_112_2_18_2_20_2_191,"MMIFILE MMIFILE_MoveOpen src_file_name 0x&x src_filename 0x&x !")
TRACE_MSG(MMI_FILETASK_1955_112_2_18_2_20_2_192,"[MMIFMM]: MMIFILE_MoveOpen dest_file_name 0x&x dest_filename 0x&x !")
TRACE_MSG(MMI_FILETASK_1962_112_2_18_2_20_2_193,"[MMIFMM]: MMIFILE_MoveOpen handle_ptr 0x%x !")
TRACE_MSG(MMI_FILETASK_1974_112_2_18_2_20_2_194,"[MMIFMM]: MMIFILE_MoveOpen param error")
TRACE_MSG(MMI_FILETASK_2055_112_2_18_2_20_2_195,"[MMIFMM]:APPFILE_HandleMoveSig para error!")
TRACE_MSG(MMI_FILETASK_2062_112_2_18_2_20_2_196,"[MMIFMM]:APPFILE_HandleMoveSig PNULL == move_handle_ptr!")
TRACE_MSG(MMI_FILETASK_2096_112_2_18_2_20_2_197,"[MMIFMM]:filename too long src_path_len + src_file_len + 1 = %d")
TRACE_MSG(MMI_FILETASK_2147_112_2_18_2_20_2_198,"[MMIFMM]: APPFILE_CopyClose ")
TRACE_MSG(MMI_FILETASK_2168_112_2_18_2_20_2_199,"[MMIFMM]:src_filename_len + dest_full_path_len = %d")
TRACE_MSG(MMI_FILETASK_2196_112_2_18_2_20_2_200,"[MMIFMM]: APPFILE_AbortMove ")
TRACE_MSG(MMI_FILETASK_2199_112_2_18_2_20_2_201,"[MMIFMM]: APPFILE_AbortMove PNULL == handle_ptr")
TRACE_MSG(MMIFILEARRAY_112_112_2_18_2_20_3_202,"MMIAPIFILEARRAY_Create, file_array = %x")
TRACE_MSG(MMIFILEARRAY_133_112_2_18_2_20_3_203,"MMIAPIFILEARRAY_Destroy, *file_array_ptr = %x")
TRACE_MSG(MMIFILEARRAY_197_112_2_18_2_20_3_204,"[FMM] MMIAPIFILEARRAY_Read data_ptr->name_len %d !")
TRACE_MSG(MMIFILEARRAY_224_112_2_18_2_20_3_205,"[FMM] MMIAPIFILEARRAY_Read data_ptr->name_len %d !")
TRACE_MSG(MMIFILEARRAY_628_112_2_18_2_20_4_206,"CompareNameAsce param error!")
TRACE_MSG(MMIFILEARRAY_667_112_2_18_2_20_4_207,"CompareNameDesc param error!")
TRACE_MSG(MMIFILEARRAY_778_112_2_18_2_20_4_208,"CompareTypeAsce param error!")
TRACE_MSG(MMIFILEARRAY_818_112_2_18_2_20_4_209,"CompareTypeDesc param error!")
TRACE_MSG(MMIFILEARRAY_854_112_2_18_2_20_4_210,"CompareTimeAsec param error!")
TRACE_MSG(MMIFILEARRAY_921_112_2_18_2_20_4_211,"CompareSizeAsec param error!")
TRACE_MSG(MMIFMM_746_112_2_18_2_20_7_212,"MMIAPIFMM_CompareFileName param error!")
TRACE_MSG(MMIFMM_779_112_2_18_2_20_7_213,"MMIAPIFMM_CompareFileSize param error!")
TRACE_MSG(MMIFMM_818_112_2_18_2_20_7_214,"MMIAPIFMM_CompareFileTime param error!")
TRACE_MSG(MMIFMM_859_112_2_18_2_20_7_215,"MMIAPIFMM_CompareFileType param error!")
TRACE_MSG(MMIFMM_983_112_2_18_2_20_7_216,"MMIAPIFMM_GetCurrentPathFile param error!")
TRACE_MSG(MMIFMM_997_112_2_18_2_20_7_217,"MMIFMM: MMIFMM_GetCurrentPathFile, allocate find_ptr failed!")
TRACE_MSG(MMIFMM_1008_112_2_18_2_20_7_218,"MMIFMM: MMIFMM_GetCurrentPathFile, allocate full_file_path_ptr failed!")
TRACE_MSG(MMIFMM_1637_112_2_18_2_20_9_219,"MMIFMM: MMIAPIFMM_GetFolderType, path len = %d")
TRACE_MSG(MMIFMM_1642_112_2_18_2_20_9_220,"MMIFMM:MMIAPIFMM_GetFolderType PNULL == path_ptr is error!")
TRACE_MSG(MMIFMM_1648_112_2_18_2_20_9_221,"MMIFMM:MMIAPIFMM_GetFolderType path_len = %d")
TRACE_MSG(MMIFMM_1732_112_2_18_2_20_9_222,"MMIFMM: MMIAPIFMM_FileDataSort, sort =%d.")
TRACE_MSG(MMIFMM_1736_112_2_18_2_20_9_223,"MMIFMM: MMIAPIFMM_FileDataSort, param error:PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_1778_112_2_18_2_20_9_224,"MMIAPIFMM_FileDataSort sort = %d")
TRACE_MSG(MMIFMM_1804_112_2_18_2_20_9_225,"MMIFMM_GetFileInfo param error:PNULL == full_path_ptr")
TRACE_MSG(MMIFMM_1851_112_2_18_2_20_9_226,"MMIFMM_DeleteFileSyn param error!")
TRACE_MSG(MMIFMM_1953_112_2_18_2_20_10_227,"FMM: can't malloc memory ")
TRACE_MSG(MMIFMM_2139_112_2_18_2_20_10_228,"MMIAPIFMM_IsDevEnoughSpace: free spaced high value is = %d free space low value is = %d")
TRACE_MSG(MMIFMM_2243_112_2_18_2_20_10_229,"[MMIFMM]:SetCommonCharacter,param error:PNULL == p_item")
TRACE_MSG(MMIFMM_2379_112_2_18_2_20_10_230,"[MMIFMM]: tag_code_type=%d")
TRACE_MSG(MMIFMM_2384_112_2_18_2_20_10_231,"[MMIFMM]: SONG title DETAIL!")
TRACE_MSG(MMIFMM_2406_112_2_18_2_20_10_232,"[MMIFMM]: artist artist DETAIL!")
TRACE_MSG(MMIFMM_2512_112_2_18_2_20_11_233,"AddOneDetailItem fail ")
TRACE_MSG(MMIFMM_2538_112_2_18_2_20_11_234,"MMIAPIFMM_GetPictureWidthHeight param error!")
TRACE_MSG(MMIFMM_2727_112_2_18_2_20_11_235,"OpenSelectMusicWinCbk param error:PNULL == file_data_ptr")
TRACE_MSG(MMIFMM_3448_112_2_18_2_20_12_236,"MMIAPIFMM_CloseSelectMusicWin, exit")
TRACE_MSG(MMIFMM_3458_112_2_18_2_20_12_237,"MMIAPIFMM_CloseSelectFileWin, exit")
TRACE_MSG(MMIFMM_3498_112_2_18_2_20_13_238,"OpenSelectMovWinCbk param error:PNULL == file_data_ptr")
TRACE_MSG(MMIFMM_3523_112_2_18_2_20_13_239,"OpenSelectMovWinCbk, supported_type = %d")
TRACE_MSG(MMIFMM_3545_112_2_18_2_20_13_240,"OpenSelectMovWinMMSCbk param error:PNULL == file_data_ptr")
TRACE_MSG(MMIFMM_3857_112_2_18_2_20_13_241,"MMIAPIFMM_CloseSelectMovieWin, exit")
TRACE_MSG(MMIFMM_4131_112_2_18_2_20_14_242,"OpenSelectAllFileWinCbk param error:PNULL == file_data_ptr")
TRACE_MSG(MMIFMM_4162_112_2_18_2_20_14_243,"OpenSelectPicWinCbk param error:PNULL == file_data_ptr")
TRACE_MSG(MMIFMM_4471_112_2_18_2_20_14_244,"InitMultimFolder param error!")
TRACE_MSG(MMIFMM_4641_112_2_18_2_20_15_245,"MMIAPIFMM_CloseWaitSetWallpaper: reset previous wallpaper error!")
TRACE_MSG(MMIFMM_4676_112_2_18_2_20_15_246,"MMIAPIFMM_CloseWaitSetWallpaper:wallpaper_type %d is error!")
TRACE_MSG(MMIFMM_4715_112_2_18_2_20_15_247,"MMIAPIFMM_CloseWaitSetWallpaper:wallpaper_type %d is error!")
TRACE_MSG(MMIFMM_4772_112_2_18_2_20_15_248,"MMIAPIFMM_ChangeWallPaperFileName param error!")
TRACE_MSG(MMIFMM_4815_112_2_18_2_20_15_249,"MMIAPIFMM_IsWallPaperFile param error!")
TRACE_MSG(MMIFMM_4849_112_2_18_2_20_15_250,"MMIAPIFMM_SetPictureToWallpaper param error!")
TRACE_MSG(MMIFMM_4906_112_2_18_2_20_15_251,"MMIAPIFMM_SetPictureToWallpaper:get file info fail!")
TRACE_MSG(MMIFMM_4950_112_2_18_2_20_15_252,"MMIAPIFMM_SetPictureToWallpaperEx full_path_ptr is PNULL !!!")
TRACE_MSG(MMIFMM_4955_112_2_18_2_20_15_253,"MMIAPIFMM_SetPictureToWallpaperEx full_path_len is PNULL !!!")
TRACE_MSG(MMIFMM_5015_112_2_18_2_20_16_254,"MMIAPIFMM_SetMusicToRing param error: PNULL == full_name_ptr")
TRACE_MSG(MMIFMM_5119_112_2_18_2_20_16_255,"MMIAPIFMM_SetMusicToRing: create audio file handle error!")
TRACE_MSG(MMIFMM_5157_112_2_18_2_20_16_256,"MMIAPIFMM_SetFileNameWinTitle param error!")
TRACE_MSG(MMIFMM_5244_112_2_18_2_20_16_257,"MMIAPIFMM_SetRandomNameEditbox param error:PNULL == prefix_ptr")
TRACE_MSG(MMIFMM_5277_112_2_18_2_20_16_258,"MMIFMM_GetOpenFileWinTickedFile param error!")
TRACE_MSG(MMIFMM_5400_112_2_18_2_20_16_259,"MMIFMM: MMIAPIFMM_SetFolderDetail, detail_data =%x")
TRACE_MSG(MMIFMM_5441_112_2_18_2_20_16_260,"MMIFMM: HandleFolderDetailWaitWinMsg, win_id =%d, msg_id = 0x%x.")
TRACE_MSG(MMIFMM_5527_112_2_18_2_20_17_261,"MMIFMM: MMIAPIFMM_SetFolderDetailCNF, detail_data =%x")
TRACE_MSG(MMIFMM_5896_112_2_18_2_20_17_262,"[MMIDRM] MMIFMM_GetSuffixTypeInfo: invalid param!")
TRACE_MSG(MMIFMM_5906_112_2_18_2_20_17_263,"[MMIDRM] MMIFMM_GetSuffixTypeInfo: user set all file type is %x!")
TRACE_MSG(MMIFMM_5926_112_2_18_2_20_17_264,"[MMIDRM] MMIFMM_GetSuffixTypeInfo: user set file type is %x!")
TRACE_MSG(MMIFMM_6033_112_2_18_2_20_18_265,"[MMIDRM] MMIAPIFMM_SetSelDrmLimitValue: invalid param!")
TRACE_MSG(MMIFMM_6047_112_2_18_2_20_18_266,"[MMIDRM] MMIAPIFMM_SetSelDrmLimitValue: cons_mode_limit = %x, method_level_limit = %x, is_rights_valid=%d!")
TRACE_MSG(MMIFMM_6074_112_2_18_2_20_18_267,"[MMIDRM] IsSelDrmConstraintFile: invalid param!")
TRACE_MSG(MMIFMM_6171_112_2_18_2_20_18_268,"[MMIDRM] IsSelDrmConstraintFile: no any limit value!")
TRACE_MSG(MMIFMM_INTERFACE_730_112_2_18_2_20_20_269,"MMIFMM_GetXFindCtlStr: xfind_param = %d,ctlstr=%d,ctlstr_len=%d")
TRACE_MSG(MMIFMM_INTERFACE_828_112_2_18_2_20_20_270,"MMIXFIND XFindRoutineCallback: inParam = %d,status = %d,outParam1 = %d,outParam2 = %d,")
TRACE_MSG(MMIFMM_INTERFACE_839_112_2_18_2_20_20_271,"MMIXFIND XFindRoutineCallback: SFS_X_STOP")
TRACE_MSG(MMIFMM_INTERFACE_857_112_2_18_2_20_20_272,"MMIXFIND HandleFindCallback")
TRACE_MSG(MMIFMM_INTERFACE_860_112_2_18_2_20_20_273,"MMIXFIND HandleFindCallback PNULL == xfind_info!")
TRACE_MSG(MMIFMM_INTERFACE_878_112_2_18_2_20_20_274,"MMIXFIND HandleFindCallback break")
TRACE_MSG(MMIFMM_INTERFACE_923_112_2_18_2_20_21_275,"MMIFMM_XFindQuickSearch: result = %d,  find_handle= 0x%x")
TRACE_MSG(MMIFMM_INTERFACE_1065_112_2_18_2_20_21_276,"MMIAPIFMM_SearchFileInPath param error!")
TRACE_MSG(MMIFMM_INTERFACE_1069_112_2_18_2_20_21_277,"MMIAPIFMM_SearchFileInPath, enter")
TRACE_MSG(MMIFMM_INTERFACE_1105_112_2_18_2_20_21_278,"MMIAPIFMM_SearchFileInPath param error!")
TRACE_MSG(MMIFMM_INTERFACE_1109_112_2_18_2_20_21_279,"MMIAPIFMM_SearchFileInPath, enter")
TRACE_MSG(MMIFMM_INTERFACE_1144_112_2_18_2_20_21_280,"MMIAPIFMM_SearchFileInPath param error!")
TRACE_MSG(MMIFMM_INTERFACE_1148_112_2_18_2_20_21_281,"MMIAPIFMM_SearchFileInPathEx, enter")
TRACE_MSG(MMIFMM_INTERFACE_1185_112_2_18_2_20_21_282,"MMIAPIFMM_SearchFileInPath param error!")
TRACE_MSG(MMIFMM_INTERFACE_1188_112_2_18_2_20_21_283,"MMIAPIFMM_SearchFileInPath, enter")
TRACE_MSG(MMIFMM_INTERFACE_1225_112_2_18_2_20_21_284,"MMIFMM_SearchFileInPathByString param error:PNULL == file_array")
TRACE_MSG(MMIFMM_INTERFACE_1229_112_2_18_2_20_21_285,"MMIFMM_SearchFileInPathByString, enter")
TRACE_MSG(MMIFMM_INTERFACE_1302_112_2_18_2_20_22_286,"MMIAPIFMM_SearchFileInPath param error!")
TRACE_MSG(MMIFMM_INTERFACE_1306_112_2_18_2_20_22_287,"MMIAPIFMM_SearchFileInPath, enter")
TRACE_MSG(MMIFMM_INTERFACE_1328_112_2_18_2_20_22_288,"MMIFMM_SearchFileInTask, enter")
TRACE_MSG(MMIFMM_INTERFACE_1333_112_2_18_2_20_22_289,"MMIFMM_SearchFileInTask param error:NULL == is_stop_ptr")
TRACE_MSG(MMIFMM_INTERFACE_1340_112_2_18_2_20_22_290,"MMIFMM_SearchFileInTask, stoped by user !!")
TRACE_MSG(MMIFMM_INTERFACE_1380_112_2_18_2_20_22_291,"MMIAPIFMM_SearchFileCallBack, enter")
TRACE_MSG(MMIFMM_INTERFACE_1389_112_2_18_2_20_22_292,"MMIAPIFMM_SearchFileCallBack, ignored !!")
TRACE_MSG(MMIFMM_INTERFACE_1410_112_2_18_2_20_22_293,"MMIFMM_SearchFileStop, enter")
TRACE_MSG(MMIFMM_INTERFACE_1476_112_2_18_2_20_22_294,"MMIFMM: HandleOpenFileByLayerWaitWin PNULL == s_file_array")
TRACE_MSG(MMIFMM_INTERFACE_1624_112_2_18_2_20_22_295,"MMIFMM_HandleOpenFileByLayerWin, msg_id = %x")
TRACE_MSG(MMIFMM_INTERFACE_1722_112_2_18_2_20_22_296,"MMIFMM_HandleOpenFileByLayerWin softkey STXT_RETURN11")
TRACE_MSG(MMIFMM_INTERFACE_1767_112_2_18_2_20_23_297,"MMIFMM_HandleOpenFileByLayerWin softkey STXT_RETURN33")
TRACE_MSG(MMIFMM_INTERFACE_1774_112_2_18_2_20_23_298,"MMIFMM_HandleOpenFileByLayerWin softkey STXT_RETURN22")
TRACE_MSG(MMIFMM_INTERFACE_1805_112_2_18_2_20_23_299,"MMIFMM_HandleOpenFileByLayerWin cur_dev_type = %d,cur_tab_index = %d")
TRACE_MSG(MMIFMM_INTERFACE_1854_112_2_18_2_20_23_300,"MSG_CTL_LIST_NEED_ITEM_DATA,msg_id = %x, param error :PNULL == need_item_data_ptr")
TRACE_MSG(MMIFMM_INTERFACE_1871_112_2_18_2_20_23_301,"MSG_CTL_LIST_NEED_ITEM_CONTENT,msg_id = %x, param error :PNULL == need_item_content_ptr")
TRACE_MSG(MMIFMM_INTERFACE_2085_112_2_18_2_20_23_302,"[MMIFMM]:MSG_APP_CANCEL path_depth = %d")
TRACE_MSG(MMIFMM_INTERFACE_2358_112_2_18_2_20_24_303,"DoSearchInFolderRecurse NULL == file_array !!")
TRACE_MSG(MMIFMM_INTERFACE_2366_112_2_18_2_20_24_304,"DoSearchInFolderRecurse create s_foldre_farray fail !")
TRACE_MSG(MMIFMM_INTERFACE_2445_112_2_18_2_20_24_305,"[MMIFMM] DoSearchInFolderRecurse read error !")
TRACE_MSG(MMIFMM_INTERFACE_2474_112_2_18_2_20_24_306,"DoSearchInFolder NULL == file_array !!")
TRACE_MSG(MMIFMM_INTERFACE_2554_112_2_18_2_20_24_307,"SearchFileAndSaveFolder_CallBack param error!")
TRACE_MSG(MMIFMM_INTERFACE_2615_112_2_18_2_20_24_308,"SearchFileAndSaveFolder param error!")
TRACE_MSG(MMIFMM_INTERFACE_2643_112_2_18_2_20_24_309,"SearchFileAndFolder_CallBack param error!")
TRACE_MSG(MMIFMM_INTERFACE_2651_112_2_18_2_20_24_310,"MMIFMM:SearchFileAndFolder_CallBack error! PNULL == f_array_data")
TRACE_MSG(MMIFMM_INTERFACE_2815_112_2_18_2_20_25_311,"MMIFMM:SearchFileAndFolder param error!")
TRACE_MSG(MMIFMM_INTERFACE_2941_112_2_18_2_20_25_312,"MMIFMM:GetPrevPath param error!")
TRACE_MSG(MMIFMM_INTERFACE_3014_112_2_18_2_20_25_313,"LoadFileToList param error:PNULL == file_array")
TRACE_MSG(MMIFMM_INTERFACE_3045_112_2_18_2_20_25_314,"MMIFMM:LoadFileToList error ! PNULL == s_file_array")
TRACE_MSG(MMIFMM_INTERFACE_3149_112_2_18_2_20_25_315,"LoadItemToList param error!")
TRACE_MSG(MMIFMM_INTERFACE_3161_112_2_18_2_20_25_316,"[FMM] LoadItemToList read file array failed !")
TRACE_MSG(MMIFMM_INTERFACE_3471_112_2_18_2_20_26_317,"FindAllFiles param error!")
TRACE_MSG(MMIFMM_INTERFACE_3480_112_2_18_2_20_26_318,"MMIFMM:FindAllFiles error:PNULL == find_data")
TRACE_MSG(MMIFMM_INTERFACE_3489_112_2_18_2_20_26_319,"MMIFMM:FindAllFiles error:PNULL == find")
TRACE_MSG(MMIFMM_INTERFACE_3567_112_2_18_2_20_26_320,"MMIAPIFMM_GetFileTypeByFileName param error:PNULL == file_name")
TRACE_MSG(MMIFMM_INTERFACE_3634_112_2_18_2_20_26_321,"InitSearchParam param error:PNULL == filter_ptr")
TRACE_MSG(MMIFMM_INTERFACE_3695_112_2_18_2_20_26_322,"ClearTempFilesInDevice param error!")
TRACE_MSG(MMIFMM_INTERFACE_3788_112_2_18_2_20_27_323,"[MMIFMM]OpenSearchFileWaitWin waitwin already opened !")
TRACE_MSG(MMIFMM_INTERFACE_3955_112_2_18_2_20_27_324,"MMIAPIFMM_GetOpenFileWinCurFile param error:PNULL == file_data_ptr")
TRACE_MSG(MMIFMM_INTERFACE_3973_112_2_18_2_20_27_325,"MMIFMM_SetOpenFileWinDisp disp_type=%d")
TRACE_MSG(MMIFMM_INTERFACE_4047_112_2_18_2_20_27_326,"MMIFMM_GetOpenFileWinFile param error:PNULL == file_data_ptr")
TRACE_MSG(MMIFMM_INTERFACE_4088_112_2_18_2_20_27_327,"SetWinTitle param error:PNULL == path_name_ptr")
TRACE_MSG(MMIFMM_INTERFACE_4123_112_2_18_2_20_27_328,"MMIFMM_INTERFACE.C MMIFMM_GetIconIdByFileName filename == NULL")
TRACE_MSG(MMIFMM_INTERFACE_4178_112_2_18_2_20_27_329,"MMIFMM_OpenFileWinByLayerInternal param error:PNULL == filter_ptr")
TRACE_MSG(MMIFMM_INTERFACE_4423_112_2_18_2_20_28_330,"MMIFMM_SetDeviceTable tab_index = %d,fmm_dev= %d")
TRACE_MSG(MMIFMM_INTERFACE_4439_112_2_18_2_20_28_331,"MMIFMM_GetDeviceType tab_index = %d")
TRACE_MSG(MMIFMM_INTERFACE_4498_112_2_18_2_20_28_332,"SearchPlayMovieFile, item_index = %d")
TRACE_MSG(MMIFMM_INTERFACE_4502_112_2_18_2_20_28_333,"SearchPlayMovieFile: GET FILE ERROR, item_index = %d")
TRACE_MSG(MMIFMM_INTERFACE_4545_112_2_18_2_20_28_334,"SearchPlayMusicFile, item_index = %d")
TRACE_MSG(MMIFMM_INTERFACE_4549_112_2_18_2_20_28_335,"SearchPlayMusicFile: GET FILE ERROR, item_index = %d")
TRACE_MSG(MMIFMM_INTERFACE_4599_112_2_18_2_20_28_336,"MMISEARCH_GetFileInfo:PNULL == file_info_ptr")
TRACE_MSG(MMIFMM_INTERFACE_4604_112_2_18_2_20_28_337,"MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d")
TRACE_MSG(MMIFMM_INTERFACE_4653_112_2_18_2_20_28_338,"MMIAPIFMM_OpenMatchedItem, call relative application!")
TRACE_MSG(MMIFMM_INTERFACE_4846_112_2_18_2_20_29_339,"MMISEARCH_LoadFileRstToList: READ file array error!")
TRACE_MSG(MMIFMM_WINTAB_2224_112_2_18_2_20_39_340,"MMIFMM: SaveCurrentPath.")
TRACE_MSG(MMIFMM_WINTAB_2227_112_2_18_2_20_39_341,"MMIFMM:[SaveCurrentPath]:PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_2232_112_2_18_2_20_39_342,"MMIFMM:[SaveCurrentPath]:PNULL == path_ptr")
TRACE_MSG(MMIFMM_WINTAB_2270_112_2_18_2_20_39_343,"MMIFMM: UpdateRenameData, ctrl_id = %d, new_filename_len = %d.")
TRACE_MSG(MMIFMM_WINTAB_2275_112_2_18_2_20_39_344,"MMIFMM:[UpdateRenameData]:PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_2280_112_2_18_2_20_39_345,"MMIFMM:[UpdateRenameData]:PNULL == new_filename_ptr")
TRACE_MSG(MMIFMM_WINTAB_2287_112_2_18_2_20_39_346,"MMIFMM:[UpdateRenameData]:new_filename_len = %d")
TRACE_MSG(MMIFMM_WINTAB_2368_112_2_18_2_20_39_347,"MMIFMM: UpdateListData, win_id =%d, ctrl_id = %d, type = %d.")
TRACE_MSG(MMIFMM_WINTAB_2373_112_2_18_2_20_39_348,"MMIFMM:[UpdateListData]:param error!")
TRACE_MSG(MMIFMM_WINTAB_2471_112_2_18_2_20_40_349,"MMIFMM: FmmOpenFont, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_2476_112_2_18_2_20_40_350,"MMIFMM:[FmmOpenFont]:NULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_2481_112_2_18_2_20_40_351,"MMIFMM:[FmmOpenFont]:NULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_2558_112_2_18_2_20_40_352,"MMIFMM:[HandleListItem]:NULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_2563_112_2_18_2_20_40_353,"MMIFMM:[HandleListItem]:NULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_2581_112_2_18_2_20_40_354,"MMIFMM: HandleListItem , ctrl_id = %d, index = %d")
TRACE_MSG(MMIFMM_WINTAB_2587_112_2_18_2_20_40_355,"MMIFMM:[HandleListItem]:NULL == ctrl_ptr")
TRACE_MSG(MMIFMM_WINTAB_2606_112_2_18_2_20_40_356,"MMIFMM: HandleListItem, Max depth!")
TRACE_MSG(MMIFMM_WINTAB_2619_112_2_18_2_20_40_357,"MMIFMM: HandleListItem, enter subdir!")
TRACE_MSG(MMIFMM_WINTAB_2636_112_2_18_2_20_40_358,"MMIFMM:[HandleListItem]: total_pathname_len = %d")
TRACE_MSG(MMIFMM_WINTAB_2662_112_2_18_2_20_40_359,"MMIFMM: HandleListItem, call relative application!")
TRACE_MSG(MMIFMM_WINTAB_2872_112_2_18_2_20_40_360,"MMIFMM: FmmOpenSzipFile, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_2877_112_2_18_2_20_40_361," PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_2882_112_2_18_2_20_40_362," PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_2908_112_2_18_2_20_40_363,"MMIFMM: GetNextIndex.")
TRACE_MSG(MMIFMM_WINTAB_2913_112_2_18_2_20_40_364,"MMIFMM:[GetNextIndex] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_2918_112_2_18_2_20_40_365,"MMIFMM:[GetNextIndex] PNULL == folder_name_ptr")
TRACE_MSG(MMIFMM_WINTAB_2956_112_2_18_2_20_40_366,"MMIFMM: SetupPicIndexData.")
TRACE_MSG(MMIFMM_WINTAB_2961_112_2_18_2_20_40_367,"MMIFMM:[SetupPicIndexData] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_2966_112_2_18_2_20_41_368,"MMIFMM:[SetupPicIndexData] PNULL == pic_index_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_3048_112_2_18_2_20_41_369,"MMIFMM: HandleFmmMainWinMsg, win_id =%d, msg_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_3204_112_2_18_2_20_41_370,"MMIFMM: FmmInitDevicePath, device =%d.")
TRACE_MSG(MMIFMM_WINTAB_3386_112_2_18_2_20_41_371,"[VP]: AppendTwoLineAnimTextAndTextListItem FmmVideoThumbnailAdd  !")
TRACE_MSG(MMIFMM_WINTAB_3554_112_2_18_2_20_42_372,"MMIFMM: HandleMenuMark.")
TRACE_MSG(MMIFMM_WINTAB_3558_112_2_18_2_20_42_373,"MMIFMM:[HandleMenuMark]PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_3595_112_2_18_2_20_42_374,"MMIFMM: HandleMenuCancelMark.")
TRACE_MSG(MMIFMM_WINTAB_3599_112_2_18_2_20_42_375,"MMIFMM:[HandleMenuCancelMark]PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_3640_112_2_18_2_20_42_376,"MMIFMM: HandleMenuMarkAll.")
TRACE_MSG(MMIFMM_WINTAB_3644_112_2_18_2_20_42_377,"MMIFMM:[HandleMenuMarkAll]PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_3683_112_2_18_2_20_42_378,"MMIFMM: HandleMenuCancelAllMark.")
TRACE_MSG(MMIFMM_WINTAB_3733_112_2_18_2_20_42_379,"MMIFMM: EnablelMenuItem.")
TRACE_MSG(MMIFMM_WINTAB_3737_112_2_18_2_20_42_380,"MMIFMM:[DisableInvalidMenuItem] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_3743_112_2_18_2_20_42_381,"MMIFMM:[DisableInvalidMenuItem] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_4002_112_2_18_2_20_43_382,"MMIFMM:[DisableInvalidMenuItem] list_data_ptr->checked = %d")
TRACE_MSG(MMIFMM_WINTAB_4055_112_2_18_2_20_43_383,"MMIFMM: FmmFileDetail,  ctrl_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_4059_112_2_18_2_20_43_384,"MMIFMM:[FmmFileDetail] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_4086_112_2_18_2_20_43_385,"MMIFMM: FmmRenameFile, win_id =%d, ctrl_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_4090_112_2_18_2_20_43_386,"MMIFMM:[FmmRenameFile] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_4121_112_2_18_2_20_43_387,"[MMIFMM] MMIFMM_MAIN_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_4126_112_2_18_2_20_43_388,"[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_4134_112_2_18_2_20_43_389,"MMIFMM: HandleNormalMenuWinMsg, win_id =0x%x, msg_id = 0x%x.")
TRACE_MSG(MMIFMM_WINTAB_4188_112_2_18_2_20_43_390,"MMIFMM: FmmSetWallpaper, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_4193_112_2_18_2_20_43_391,"MMIFMM:[FmmSetWallpaper] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_4199_112_2_18_2_20_43_392,"MMIFMM:[FmmSetWallpaper] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_4239_112_2_18_2_20_43_393,"MMIFMM: FmmSetPB, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_4244_112_2_18_2_20_43_394,"MMIFMM:[FmmSetPB] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_4250_112_2_18_2_20_43_395,"MMIFMM:[FmmSetPB] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_4353_112_2_18_2_20_43_396,"MMIFMM: FmmSetCallRing.")
TRACE_MSG(MMIFMM_WINTAB_4358_112_2_18_2_20_43_397,"MMIFMM:[FmmSetCallRing] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_4364_112_2_18_2_20_43_398,"MMIFMM:[FmmSetCallRing] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_4418_112_2_18_2_20_43_399,"[MMIFMM] MMIFMM_MAIN_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_4424_112_2_18_2_20_43_400,"[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_4436_112_2_18_2_20_43_401,"[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_4446_112_2_18_2_20_43_402,"[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_4453_112_2_18_2_20_43_403,"MMIFMM: HandleMusicMenuWinMsg, win_id =%d, msg_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_4517_112_2_18_2_20_44_404,"[VP] HandleVideoScreenSaverMsg msg_ptr->msg_id = %d")
TRACE_MSG(MMIFMM_WINTAB_4695_112_2_18_2_20_44_405,"[MMIFMM] MMIFMM_MAIN_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_4701_112_2_18_2_20_44_406,"[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_4713_112_2_18_2_20_44_407,"[MMIFMM] MMIFMM_SDCARD_CHILD_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_4723_112_2_18_2_20_44_408,"[MMIFMM] MMIFMM_SDCARD_CHILD_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_4730_112_2_18_2_20_44_409,"MMIFMM: HandleMovieMenuWinMsg, win_id =%d, msg_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_4975_112_2_18_2_20_44_410,"MMIFMM: HandlePicPreviewMenuWinMsg, win_id =%d, msg_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_5033_112_2_18_2_20_45_411,"MMIFMM: FmmSendMMS.")
TRACE_MSG(MMIFMM_WINTAB_5038_112_2_18_2_20_45_412,"MMIFMM:[FmmSendMMS] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_5044_112_2_18_2_20_45_413,"MMIFMM:[FmmSendMMS] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_5124_112_2_18_2_20_45_414,"MMIFMM: FmmBtSendFile.")
TRACE_MSG(MMIFMM_WINTAB_5127_112_2_18_2_20_45_415,"MMIFMM:[FmmBtSendFile] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_5133_112_2_18_2_20_45_416,"MMIFMM:[FmmBtSendFile] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_5169_112_2_18_2_20_45_417,"[&&&]FmmBtSendFile  send_file_num = %d")
TRACE_MSG(MMIFMM_WINTAB_5174_112_2_18_2_20_45_418,"[&&&]FmmBtSendFile  send_file_num=%d")
TRACE_MSG(MMIFMM_WINTAB_5220_112_2_18_2_20_45_419,"[&&&]FmmBtSendFile  send_file_index=%d,send_file_num = %d")
TRACE_MSG(MMIFMM_WINTAB_5240_112_2_18_2_20_45_420,"MMIFMM: FmmFileDataSort, sort =%d.")
TRACE_MSG(MMIFMM_WINTAB_5243_112_2_18_2_20_45_421,"MMIFMM:[FmmFileDataSort] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_5284_112_2_18_2_20_45_422,"[MMIFMM] MMIFMM_MAIN_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_5291_112_2_18_2_20_45_423,"[MMIFMM] MMIFMM_UDISK_CHILD_WIN_ID already closed !")
TRACE_MSG(MMIFMM_WINTAB_5296_112_2_18_2_20_45_424,"MMIFMM: HandleEmptyFolderMenuWinMsg, win_id =%d, msg_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_5394_112_2_18_2_20_45_425,"MMIFMM: FmmPlayMovieFile, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_5399_112_2_18_2_20_45_426,"MMIFMM:[FmmPlayMovieFile] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_5405_112_2_18_2_20_45_427,"MMIFMM:[FmmPlayMovieFile] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_5456_112_2_18_2_20_46_428,"MMIFMM: FmmOpenVcardFile, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_5461_112_2_18_2_20_46_429,"MMIFMM:[FmmOpenVcardFile] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_5467_112_2_18_2_20_46_430,"MMIFMM:[FmmOpenVcardFile] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_5488_112_2_18_2_20_46_431,"MMIFMM: FmmOpenVcalendarFile, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_5493_112_2_18_2_20_46_432,"MMIFMM:[FmmOpenVcardFile] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_5499_112_2_18_2_20_46_433,"MMIFMM:[FmmOpenVcardFile] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_5528_112_2_18_2_20_46_434,"MMIFMM: FmmPlayMusicFile, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_5533_112_2_18_2_20_46_435,"MMIFMM:[FmmPlayMusicFile] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_5539_112_2_18_2_20_46_436,"MMIFMM:[FmmPlayMusicFile] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_5607_112_2_18_2_20_46_437,"MMIFMM: FmmOpenWWWFile, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_5839_112_2_18_2_20_46_438,"FmmFindStringIsValid PNULL == full_path_wstr_ptr")
TRACE_MSG(MMIFMM_WINTAB_6255_112_2_18_2_20_47_439,"HandleCreateFolderWinMsg: create_result = %d")
TRACE_MSG(MMIFMM_WINTAB_6322_112_2_18_2_20_47_440,"MMIFMM:[NewFolderNameStringIsValid] PNULL == file_name_ptr")
TRACE_MSG(MMIFMM_WINTAB_6369_112_2_18_2_20_47_441,"[MMIFMM] FmmRename create MMIFMM_RENAME_WIN_TAB failed !")
TRACE_MSG(MMIFMM_WINTAB_6379_112_2_18_2_20_47_442,"MMIFMM: FmmRename, no memeory!")
TRACE_MSG(MMIFMM_WINTAB_6423_112_2_18_2_20_48_443,"MMIFMM: HandleFmmRenameWinMsg, win_id =0x%x, msg_id = 0x%x.")
TRACE_MSG(MMIFMM_WINTAB_6439_112_2_18_2_20_48_444,"[MMIFMM] HandleFmmRenameWinMsg MSG_OPEN_WINDOW PNULL == rename_param_ptr")
TRACE_MSG(MMIFMM_WINTAB_6471_112_2_18_2_20_48_445,"HandleFmmRenameWinMsg, max_file_name_len = %d.")
TRACE_MSG(MMIFMM_WINTAB_6561_112_2_18_2_20_48_446,"HandleFmmRenameWinMsg: rename_result = %d")
TRACE_MSG(MMIFMM_WINTAB_6601_112_2_18_2_20_48_447,"HandleFmmRenameWinMsg: file_is_power_on_anim = %d")
TRACE_MSG(MMIFMM_WINTAB_6602_112_2_18_2_20_48_448,"HandleFmmRenameWinMsg: file_is_power_off_anim = %d")
TRACE_MSG(MMIFMM_WINTAB_6608_112_2_18_2_20_48_449,"HandleFmmRenameWinMsg: file_is_ssaver = %d")
TRACE_MSG(MMIFMM_WINTAB_6656_112_2_18_2_20_48_450,"HandleFmmRenameWinMsg: file_is_ssaver = %d")
TRACE_MSG(MMIFMM_WINTAB_6833_112_2_18_2_20_48_451,"CheckIsScreensaverInfo: ssaver_info->more_pic_saver_info.name_len = %d")
TRACE_MSG(MMIFMM_WINTAB_6834_112_2_18_2_20_48_452,"CheckIsScreensaverInfo: old_name_len = %d")
TRACE_MSG(MMIFMM_WINTAB_6879_112_2_18_2_20_49_453,"CheckIsPowerOnOffAnimInfo: power_onoff_anim_info->more_anim_info.name_len = %d")
TRACE_MSG(MMIFMM_WINTAB_6880_112_2_18_2_20_49_454,"CheckIsPowerOnOffAnimInfo: old_name_len = %d")
TRACE_MSG(MMIFMM_WINTAB_6947_112_2_18_2_20_49_455,"MMIFMM:[FmmRenameSplitFolder]PNULL == full_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_6954_112_2_18_2_20_49_456,"FmmRenameSplitFolder string_len =%d")
TRACE_MSG(MMIFMM_WINTAB_6991_112_2_18_2_20_49_457,"MMIFMM:[FmmRenameSplitFile]PNULL == full_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_6998_112_2_18_2_20_49_458,"FmmRenameSplitFile string_len =%d")
TRACE_MSG(MMIFMM_WINTAB_7051_112_2_18_2_20_49_459,"MMIFMM: MMIAPIFMM_DetailFileData, no memory!")
TRACE_MSG(MMIFMM_WINTAB_7157_112_2_18_2_20_49_460,"MMIFMM: HandleFmmFileDetailWinMsg, open option")
TRACE_MSG(MMIFMM_WINTAB_7174_112_2_18_2_20_49_461,"MMIFMM: HandleFmmFileDetailWinMsg, Download Rights File")
TRACE_MSG(MMIFMM_WINTAB_7232_112_2_18_2_20_49_462,"[MMIFMM] HandleFmmQueryWin win_info_ptr == PNULL !")
TRACE_MSG(MMIFMM_WINTAB_7425_112_2_18_2_20_50_463,"FmmCheckMemoryCardState result = %d")
TRACE_MSG(MMIFMM_WINTAB_7520_112_2_18_2_20_50_464,"MMIFMM: MMIAPIFMM_OpenExplorer.")
TRACE_MSG(MMIFMM_WINTAB_7672_112_2_18_2_20_50_465,"MMIFMM: CopySelectFileToFolder")
TRACE_MSG(MMIFMM_WINTAB_7679_112_2_18_2_20_50_466,"CopySelectFileToFolder alloc s_copy_del_path_data_ptr fail!")
TRACE_MSG(MMIFMM_WINTAB_7786_112_2_18_2_20_51_467,"MMIFMM: DeleteSelectFile")
TRACE_MSG(MMIFMM_WINTAB_7828_112_2_18_2_20_51_468,"MMIFMM: SetCopyDeleteState state %d")
TRACE_MSG(MMIFMM_WINTAB_7838_112_2_18_2_20_51_469,"MMIFMM: GetCopyDeleteState state= %d")
TRACE_MSG(MMIFMM_WINTAB_7856_112_2_18_2_20_51_470,"MMIFMM: HandleWaitCopyWinMsg, win_id =%d, msg_id = 0x%x.")
TRACE_MSG(MMIFMM_WINTAB_7892_112_2_18_2_20_51_471,"[FMM]HandleWaitCopyWinMsg abort!! Copy")
TRACE_MSG(MMIFMM_WINTAB_7930_112_2_18_2_20_51_472,"MMIFMM: HandleCopyFileToFolderWinMsg, win_id =%d, msg_id = 0x%x")
TRACE_MSG(MMIFMM_WINTAB_8195_112_2_18_2_20_52_473,"MMIFMM: HandleWaitDeleteWinMsg, win_id =%d, msg_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_8230_112_2_18_2_20_52_474,"[FMM]HandleWaitDeleteWinMsg abort!! Delete")
TRACE_MSG(MMIFMM_WINTAB_8260_112_2_18_2_20_52_475,"MMIFMM: HandleDelFileFolderWinMsg, win_id =%d, msg_id = 0x%x.")
TRACE_MSG(MMIFMM_WINTAB_8379_112_2_18_2_20_52_476,"FMM_DEL: MSG_FMM_DELETE_END_CNF file_ret = %d")
TRACE_MSG(MMIFMM_WINTAB_8412_112_2_18_2_20_52_477,"MMIFMM:MMIAPIFMM_StopOperaSDPlug close relative windows.")
TRACE_MSG(MMIFMM_WINTAB_8612_112_2_18_2_20_53_478,"[MMIFMM] MMIAPIFMM_CopyFileExistInform enter")
TRACE_MSG(MMIFMM_WINTAB_8703_112_2_18_2_20_53_479,"MMIFMM: MMIFMM_GetVcardFileData, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_8708_112_2_18_2_20_53_480,"MMIFMM:[MMIFMM_GetVcardFileData] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_8714_112_2_18_2_20_53_481,"MMIFMM:[MMIFMM_GetVcardFileData] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_8722_112_2_18_2_20_53_482,"MMIFMM_GetVcardFileData list_data_ptr->data[index].file_size %d")
TRACE_MSG(MMIFMM_WINTAB_8745_112_2_18_2_20_53_483,"MMIFMM: MMIFMM_GetVcalendarFileData, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_8749_112_2_18_2_20_53_484,"MMIFMM:[MMIFMM_GetVcalendarFileData] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_8755_112_2_18_2_20_53_485,"MMIFMM:[MMIFMM_GetVcalendarFileData] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_8763_112_2_18_2_20_53_486,"MMIFMM_GetVcalendarFileData list_data_ptr->data[index].file_size %d")
TRACE_MSG(MMIFMM_WINTAB_8778_112_2_18_2_20_53_487,"MMIFMM: MMIAPIFMM_GetFmmData.")
TRACE_MSG(MMIFMM_WINTAB_9191_112_2_18_2_20_54_488,"MMIFMM: HandleUdiskChildWindow, win_id =0x%x, msg_id = 0x%x.")
TRACE_MSG(MMIFMM_WINTAB_9284_112_2_18_2_20_54_489,"[MMIFMM] HandleUdiskChildWindow MSG_CTL_TAB_SWITCH cur_tab %d ,s_cur_dev %d!")
TRACE_MSG(MMIFMM_WINTAB_9374_112_2_18_2_20_55_490,"MMIFMM: HandleUdiskChildWindow, call relative menu!")
TRACE_MSG(MMIFMM_WINTAB_9639_112_2_18_2_20_55_491,"MMIFMM: HandleUdiskChildWindow, is_videofolder:%d,state:%d")
TRACE_MSG(MMIFMM_WINTAB_9644_112_2_18_2_20_55_492,"MMIFMM: HandleUdiskChildWindow,GUILIST_START_SLIDE")
TRACE_MSG(MMIFMM_WINTAB_9697_112_2_18_2_20_56_493,"MMIFMM: HandleMovieMenuWinMsg, win_id =%d, msg_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_9765_112_2_18_2_20_56_494,"MMIFMM: HandleMovieMenuWinMsg, win_id =%d, msg_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_9834_112_2_18_2_20_56_495,"MMIFMM: HandleOtherMenuWinMsg, win_id =%d, msg_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_9887_112_2_18_2_20_56_496,"MMIFMM: FmmSetAsScreenSaver,  ctrl_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_9891_112_2_18_2_20_56_497,"MMIFMM: [FmmSetAsScreenSaver] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_9950_112_2_18_2_20_56_498,"MMIFMM: FmmSetAsVideoWallpaper,  ctrl_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_9953_112_2_18_2_20_56_499,"MMIFMM: [FmmSetAsVideoWallpaper] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_10021_112_2_18_2_20_56_500,"MMIFMM: FmmSetAsPowerOnOffAnim,  ctrl_id = %d.")
TRACE_MSG(MMIFMM_WINTAB_10025_112_2_18_2_20_56_501,"MMIFMM: [FmmSetAsPowerOnOffAnim] PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_10106_112_2_18_2_20_56_502,"MMIFMM: SetWinTitle, win_id = %d current_path_ptr->path_depth = %d")
TRACE_MSG(MMIFMM_WINTAB_10110_112_2_18_2_20_56_503,"MMIFMM: [SetWinTitle] PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_10118_112_2_18_2_20_56_504,"MMIFMM: [SetWinTitle] device = %d")
TRACE_MSG(MMIFMM_WINTAB_10221_112_2_18_2_20_57_505,"MMIFMM:[SetStrWildcard]PNULL == str")
TRACE_MSG(MMIFMM_WINTAB_10258_112_2_18_2_20_57_506,"MMIFMM_COPY_SELECT_FILE_WIN_ID enter")
TRACE_MSG(MMIFMM_WINTAB_10272_112_2_18_2_20_57_507,"MMIFMM_DEL_SELECT_FILE_WIN_ID enter")
TRACE_MSG(MMIFMM_WINTAB_10316_112_2_18_2_20_57_508,"MMIAPIFMM_IsFileOption return false!")
TRACE_MSG(MMIFMM_WINTAB_10368_112_2_18_2_20_57_509,"MMIFMM: GetFileIndex.")
TRACE_MSG(MMIFMM_WINTAB_10373_112_2_18_2_20_57_510,"MMIFMM:[GetFileIndex]PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_10379_112_2_18_2_20_57_511,"MMIFMM:[GetFileIndex]PNULL == file_name_ptr")
TRACE_MSG(MMIFMM_WINTAB_10500_112_2_18_2_20_57_512,"MMIFMM: FmmPlaykurFile, ctrl_id = %d")
TRACE_MSG(MMIFMM_WINTAB_10505_112_2_18_2_20_57_513,"MMIFMM:[GetFileIndex]PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_10511_112_2_18_2_20_57_514,"MMIFMM:[GetFileIndex]PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_10540_112_2_18_2_20_57_515,"MMIFMM:[MMIAPIFMM_GetFileInfoFormFullPath]PNULL ==file_info")
TRACE_MSG(MMIFMM_WINTAB_10569_112_2_18_2_20_57_516,"MMIFMM: CombineFileName.")
TRACE_MSG(MMIFMM_WINTAB_10575_112_2_18_2_20_57_517,"MMIFMM:[CombineToFileName]:param error!")
TRACE_MSG(MMIFMM_WINTAB_10588_112_2_18_2_20_57_518,"MMIFMM:[CombineToFileName]:total_len = %d")
TRACE_MSG(MMIFMM_WINTAB_10708_112_2_18_2_20_58_519,"MMIFMM:[HandleMenuOption]PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_10714_112_2_18_2_20_58_520,"MMIFMM:[HandleMenuOption]PNULL == current_path_ptr")
TRACE_MSG(MMIFMM_WINTAB_11044_112_2_18_2_20_58_521,"MMIFMM: HandleMarkItem, MSG_CTL_LIST_MARK_ITEM, index = %d, mark = %d")
TRACE_MSG(MMIFMM_WINTAB_11067_112_2_18_2_20_58_522,"MMIFMM: HandleSDChildWindow, %d state  error!")
TRACE_MSG(MMIFMM_WINTAB_11101_112_2_18_2_20_58_523,"MMIFMM: HandleMarkItem, %d state  error!")
TRACE_MSG(MMIFMM_WINTAB_11557_112_2_18_2_20_59_524,"MMIFMM: HandleSDChildWindow, return previous directory!")
TRACE_MSG(MMIFMM_WINTAB_11636_112_2_18_2_20_59_525,"MMIFMM:[MMIAPIFMM_HandleMenuOpt] type = %d")
TRACE_MSG(MMIFMM_WINTAB_11676_112_2_18_2_21_0_526,"MMIFMM:[MMIFMM_GetFullPathDepth]PNULL == path_ptr")
TRACE_MSG(MMIFMM_WINTAB_11687_112_2_18_2_21_0_527,"MMIFMM_GetFullPathDepth path_depth =%d")
TRACE_MSG(MMIFMM_WINTAB_11703_112_2_18_2_21_0_528,"MMIFMM:[FmmGetPathDepthLen]PNULL == path_ptr")
TRACE_MSG(MMIFMM_WINTAB_11720_112_2_18_2_21_0_529,"FmmGetPathDepthLen path_len =%d")
TRACE_MSG(MMIFMM_WINTAB_11735_112_2_18_2_21_0_530,"MMIFMM:[MMIFMM_GetFinallySlashIndex]PNULL == file_name_ptr")
TRACE_MSG(MMIFMM_WINTAB_11745_112_2_18_2_21_0_531,"MMIFMM_GetFinallySlashIndex i =%d")
TRACE_MSG(MMIFMM_WINTAB_11761_112_2_18_2_21_0_532,"FmmFileIsFolder IsFolder =%d")
TRACE_MSG(MMIFMM_WINTAB_11836_112_2_18_2_21_0_533,"MMIFMM:[FmmCopyDelExit]MMI_USE_OLD_SFS type = %d")
TRACE_MSG(MMIFMM_WINTAB_11876_112_2_18_2_21_0_534,"MMIFMM:[FmmHandleListAppendData]PNULL == need_item_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_11887_112_2_18_2_21_0_535,"MMIFMM:[FmmHandleListAppendData]PNULL == need_item_content_ptr")
TRACE_MSG(MMIFMM_WINTAB_11897_112_2_18_2_21_0_536,"MMIFMM:[FmmHandleListAppendData] type = %d")
TRACE_MSG(MMIFMM_WINTAB_11918_112_2_18_2_21_0_537,"FmmHandleListAppendData: index = %d file_num = %d,folder_num = %d")
TRACE_MSG(MMIFMM_WINTAB_11962_112_2_18_2_21_0_538,"[MMIFMM] MMIFMM_GetCurrentDevice main win not open !")
TRACE_MSG(MMIFMM_WINTAB_12027_112_2_18_2_21_0_539,"MMIAPIFMM_UpdateFmmList device = %d")
TRACE_MSG(MMIFMM_WINTAB_12112_112_2_18_2_21_0_540,"MMIFMM: HandleFmmCopyRenameWinMsg, win_id =0x%x, msg_id = 0x%x.")
TRACE_MSG(MMIFMM_WINTAB_12147_112_2_18_2_21_0_541,"HandleFmmCopyRenameWinMsg, max_file_name_len = %d.")
TRACE_MSG(MMIFMM_WINTAB_12303_112_2_18_2_21_1_542,"CopyFilesCallBack error = %d")
TRACE_MSG(MMIFMM_WINTAB_12320_112_2_18_2_21_1_543,"DelFilesCallBack error = %d")
TRACE_MSG(MMIFMM_WINTAB_12385_112_2_18_2_21_1_544,"GetTargetPathCallBack array size == 0")
TRACE_MSG(MMIFMM_WINTAB_12394_112_2_18_2_21_1_545,"GetTargetPathCallBack failed")
TRACE_MSG(MMIFMM_WINTAB_12591_112_2_18_2_21_1_546,"FmmIsCopyDelOK result = %d")
TRACE_MSG(MMIFMM_WINTAB_12631_112_2_18_2_21_1_547,"MMIFMM: GetTargetnameFromSrcTarget.")
TRACE_MSG(MMIFMM_WINTAB_12638_112_2_18_2_21_1_548,"MMIFMM:[GetTargetnameFromSrcTarget] param error!")
TRACE_MSG(MMIFMM_WINTAB_12890_112_2_18_2_21_2_549,"[MMIFMM] MMIAPIFMM_CopyNext s_user_stop_copy %d enter")
TRACE_MSG(MMIFMM_WINTAB_12922_112_2_18_2_21_2_550,"MMIFMM:[MMIFMM_GetPicName] PNULL == name_len_ptr")
TRACE_MSG(MMIFMM_WINTAB_12960_112_2_18_2_21_2_551,"[ATEST][Atest_FMMCreateFolderHori:]%d")
TRACE_MSG(MMIFMM_WINTAB_12964_112_2_18_2_21_2_552,"[atest]Atest_FMMCreateFolderHori: entry number error! ")
TRACE_MSG(MMIFMM_WINTAB_13000_112_2_18_2_21_2_553,"[Atest]Atest_FMMCreateFolderHori: new:%d")
TRACE_MSG(MMIFMM_WINTAB_13005_112_2_18_2_21_2_554,"[atest]Atest_FMMCreateFolderHori:  new:%d")
TRACE_MSG(MMIFMM_WINTAB_13039_112_2_18_2_21_2_555,"[Atest]Atest_FMMCreateFolderVerti: entry error!")
TRACE_MSG(MMIFMM_WINTAB_13062_112_2_18_2_21_2_556,"[atest]Atest_FMMCreateFolderVerti:new:%d")
TRACE_MSG(MMIFMM_WINTAB_13067_112_2_18_2_21_2_557,"[atest]Atest_FMMCreateFolderVerti:new:%d")
TRACE_MSG(MMIFMM_WINTAB_13092_112_2_18_2_21_2_558,"[MMIFMM:]MMIAPIFMM_DrmOpenDownloadMenuWin = PNULL!")
TRACE_MSG(MMIFMM_WINTAB_13453_112_2_18_2_21_3_559,"MMIAPIFMM_OpenFile Enter")
TRACE_MSG(MMIFMM_WINTAB_13457_112_2_18_2_21_3_560,"MMIAPIFMM_OpenFile, file name is null")
TRACE_MSG(MMIFMM_WINTAB_13474_112_2_18_2_21_3_561,"MMIAPIFMM_OpenFile Fail, no memory")
TRACE_MSG(MMIFMM_WINTAB_13611_112_2_18_2_21_4_562,"MMIFMM: HandlePDAMenuMark.")
TRACE_MSG(MMIFMM_WINTAB_13614_112_2_18_2_21_4_563,"MMIFMM:[HandlePDAMenuMark]PNULL == list_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_13841_112_2_18_2_21_4_564,"IsMarkItemContainFolders s_fmm_list_data.mark_num = 0!")
TRACE_MSG(MMIFMM_WINTAB_13870_112_2_18_2_21_4_565,"[VP]  FmmVideoThumbnailGetFloder error parameter")
TRACE_MSG(MMIFMM_WINTAB_13896_112_2_18_2_21_4_566,"[VP]  FmmVideoThumbnailGetPathname error parameter")
TRACE_MSG(MMIFMM_WINTAB_13935_112_2_18_2_21_4_567,"[VP]:FmmVideoThumbnailUpdateIcon  !")
TRACE_MSG(MMIFMM_WINTAB_13942_112_2_18_2_21_4_568,"[VP]:FmmVideoThumbnailUpdateIcon  s_fmm_list_data.data[ top_index+index :%d ]->filename_ptr PNULL!")
TRACE_MSG(MMIFMM_WINTAB_13953_112_2_18_2_21_4_569,"MMIFMM: FmmVideoThumbnailUpdateIcon,index:%d")
TRACE_MSG(MMIFMM_WINTAB_13965_112_2_18_2_21_4_570,"MMIFMM: FmmVideoThumbnailUpdateIcon,GUILIST_ReplaceItem index:%d")
TRACE_MSG(MMIFMM_WINTAB_13987_112_2_18_2_21_4_571,"MMIFMM: FmmVideoThumbnailLoadingMovieList,top_index:%d,item_num:%d")
TRACE_MSG(MMIFMM_WINTAB_13994_112_2_18_2_21_4_572,"MMIFMM: FmmVideoThumbnailLoadingMovieList,s_fmm_list_data.data[ top_index+index:%d ]->filename_ptr PNULL")
TRACE_MSG(MMIFMM_WINTAB_14004_112_2_18_2_21_4_573,"MMIFMM: FmmVideoThumbnailLoadingMovieList,FmmVideoThumbnailAdd,index:%d")
TRACE_MSG(MMIFMM_WINTAB_14025_112_2_18_2_21_4_574,"[VP]:FmmVideoThumbnailCallback !")
TRACE_MSG(MMIFMM_WINTAB_14032_112_2_18_2_21_4_575,"[VP]:FmmVideoThumbnailCallback MMIAPIFMM_CreateDir !")
TRACE_MSG(MMIFMM_WINTAB_14064_112_2_18_2_21_4_576,"[VP]:FmmVideoThumbnailOpen  !")
TRACE_MSG(MMIFMM_WINTAB_14068_112_2_18_2_21_4_577,"[VP]:FmmVideoThumbnailOpen  slide now!")
TRACE_MSG(MMIFMM_WINTAB_14096_112_2_18_2_21_5_578,"[VP]:FmmVideoThumbnailClose  !")
TRACE_MSG(MMIFMM_WINTAB_14111_112_2_18_2_21_5_579,"[VP]:FmmVideoThumbnailAdd  !")
TRACE_MSG(MMIFMM_WINTAB_14176_112_2_18_2_21_5_580,"[VP]:FmmVideoThumbnail_Init !")
TRACE_MSG(MMIFMM_WINTAB_14284_112_2_18_2_21_5_581,"[MMIFMM]:GetMoveTargetPathCallBack alloc failed")
TRACE_MSG(MMIFMM_WINTAB_14302_112_2_18_2_21_5_582,"[MMIFMM]:GetMoveTargetPathCallBack array size == 0")
TRACE_MSG(MMIFMM_WINTAB_14311_112_2_18_2_21_5_583,"[MMIFMM]:GetMoveTargetPathCallBack failed")
TRACE_MSG(MMIFMM_WINTAB_14350_112_2_18_2_21_5_584,"[MMIFMM]:HandleMoveFileWinMsg PNULL == s_src_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_14359_112_2_18_2_21_5_585,"[MMIFMM]:HandleMoveFileWinMsg PNULL == s_src_data_ptr")
TRACE_MSG(MMIFMM_WINTAB_14394_112_2_18_2_21_5_586,"[MMIFMM]:HandleMoveFileWinMsg MSG_PROMPTWIN_OK PNULL == s_move_dst_full_path_ptr!")
TRACE_MSG(MMIFMM_WINTAB_14404_112_2_18_2_21_5_587,"[MMIFMM]:HandleMoveFileWinMsg PNULL == s_copy_handle_ptr")
TRACE_MSG(MMIFMM_WINTAB_14418_112_2_18_2_21_5_588,"[MMIFMM]:HandleMoveFileWinMsg MSG_FMM_COPY_CHOOSE_PATH_RETURN PNULL == s_move_dst_full_path_ptr!")
TRACE_MSG(MMIFMM_WINTAB_14470_112_2_18_2_21_5_589,"[MMIFMM]:HandleMoveFileWinMsg MSG_FMM_OPERATE_FILE_BEGIN PNULL == s_move_dst_full_path_ptr!")
TRACE_MSG(MMIFMM_WINTAB_14476_112_2_18_2_21_5_590,"[MMIFMM]:HandleMoveFileWinMsg s_move_dst_full_path_ptr len == 0!")
TRACE_MSG(MMIFMM_WINTAB_14492_112_2_18_2_21_5_591,"[MMIFMM]:HandleMoveFileWinMsg cur_index = %d")
TRACE_MSG(MMIFMM_WINTAB_14497_112_2_18_2_21_5_592,"[MMIFMM]:total name too long %d")
TRACE_MSG(MMIFMM_WINTAB_14582_112_2_18_2_21_6_593,"[MMIFMM]:HandleMoveFileWinMsg cur_index = %d")
TRACE_MSG(MMIFMM_WINTAB_14617_112_2_18_2_21_6_594,"[MMIFMM]:HandleMoveFileWinMsg stopped by user!")
TRACE_MSG(MMIFMM_WINTAB_14639_112_2_18_2_21_6_595,"[MMIFMM]:HandleMoveFileWinMsg case MSG_FMM_MOVE_MARK_FILE 0 == s_fmm_list_data.mark_num")
TRACE_MSG(MMIFMM_WINTAB_14783_112_2_18_2_21_6_596,"[MMIFMM] MMIAPIFMM_MoveFileExist enter")
TRACE_MSG(MMIFMM_WINTAB_14811_112_2_18_2_21_6_597,"[MMIFMM]: HandleWaitMoveWinMsg, win_id =%d, msg_id = 0x%x.")
TRACE_MSG(MMIFMM_WINTAB_14829_112_2_18_2_21_6_598,"[MMIFMM]:HandleWaitMoveWinMsg abort!! moveS")
TRACE_MSG(MMIVIRTUALARRAY_317_112_2_18_2_21_8_599,"MMIVIRTUALARRAY_Create:MMIVIRTUALARRAY_Create PNULL == v_header_ptr ")
TRACE_MSG(MMIVIRTUALARRAY_348_112_2_18_2_21_8_600,"MMIVIRTUALARRAY_Create, create virtualarray by no udisk space!")
TRACE_MSG(MMIVIRTUALARRAY_367_112_2_18_2_21_8_601,"MMIVIRTUALARRAY_Create, create virtualarray by file!")
TRACE_MSG(MMIVIRTUALARRAY_372_112_2_18_2_21_8_602,"MMIVIRTUALARRAY_Create, create virtualarray by no udisk space!")
TRACE_MSG(MMIVIRTUALARRAY_1034_112_2_18_2_21_9_603,"MMIFMM:MMIVIRTUALARRAY_Sort PNULL == sort_file_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1096_112_2_18_2_21_9_604,"MMIFMM:[UpdateBufferToFile] PNULL == v_header_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1100_112_2_18_2_21_9_605,"UpdateBufferToFile, first id = %d, num = %d")
TRACE_MSG(MMIVIRTUALARRAY_1205_112_2_18_2_21_9_606,"MMIFMM:[ReadFileToBuffer] PNULL == v_header_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1247_112_2_18_2_21_9_607,"ReadFileToBuffer, first_id = %d, num = %d")
TRACE_MSG(MMIVIRTUALARRAY_1272_112_2_18_2_21_9_608,"MMIFMM:[DeleteOneDataInFile] PNULL == v_header_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1297_112_2_18_2_21_9_609,"MMIFMM:[DeleteOneDataInFile] 1 PNULL == v_header_ptr->del_id_table_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1314_112_2_18_2_21_9_610,"MMIFMM:[DeleteOneDataInFile] 2 PNULL == v_header_ptr->del_id_table_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1339_112_2_18_2_21_10_611,"MMIFMM:[DeleteAssoFile] PNULL == v_header_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1345_112_2_18_2_21_10_612,"DeleteAssoFile, fail!!")
TRACE_MSG(MMIVIRTUALARRAY_1360_112_2_18_2_21_10_613,"MMIFMM:[DeleteAssoFile] PNULL == v_header_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1385_112_2_18_2_21_10_614,"MMIFMM:[CreateRandFile] PNULL == file_name_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1391_112_2_18_2_21_10_615,"MMIFMM:[CreateRandFile] PNULL == file_name_len_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1437_112_2_18_2_21_10_616,"MMIFMM:[ReadDataFromBuffer] param error!")
TRACE_MSG(MMIVIRTUALARRAY_1465_112_2_18_2_21_10_617,"MMIFMM:[WriteDataToBuffer] param error!")
TRACE_MSG(MMIVIRTUALARRAY_1496_112_2_18_2_21_10_618,"MMIFMM:ExternalSortFile param error!")
TRACE_MSG(MMIVIRTUALARRAY_1501_112_2_18_2_21_10_619,"MMIFMM:ExternalSortFile PNULL == compare_func")
TRACE_MSG(MMIVIRTUALARRAY_1514_112_2_18_2_21_10_620,"ExternalSortFile, not enough memory!")
TRACE_MSG(MMIVIRTUALARRAY_1519_112_2_18_2_21_10_621,"ExternalSortFile enter")
TRACE_MSG(MMIVIRTUALARRAY_1525_112_2_18_2_21_10_622,"ExternalSortFile, InitMergeFiles fail!")
TRACE_MSG(MMIVIRTUALARRAY_1528_112_2_18_2_21_10_623,"ExternalSortFile, InitMergeFiles success!")
TRACE_MSG(MMIVIRTUALARRAY_1550_112_2_18_2_21_10_624,"ExternalSortFile, MultiWayMerge success!")
TRACE_MSG(MMIVIRTUALARRAY_1563_112_2_18_2_21_10_625,"ExternalSortFile, MultiWayMerge fail!")
TRACE_MSG(MMIVIRTUALARRAY_1599_112_2_18_2_21_10_626,"InitMergeFiles v_header_ptr 0x%x")
TRACE_MSG(MMIVIRTUALARRAY_1607_112_2_18_2_21_10_627,"MMIFMM:InitMergeFiles ERROR! PNULL == wa_buf")
TRACE_MSG(MMIVIRTUALARRAY_1715_112_2_18_2_21_10_628,"MMIFMM:[MultiWayMerge] PNULL == m_fi_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1721_112_2_18_2_21_10_629,"MMIFMM:[MultiWayMerge] PNULL == compare_func")
TRACE_MSG(MMIVIRTUALARRAY_1811_112_2_18_2_21_10_630,"MMIFMM:[DestroyMergeFiles] PNULL == m_file_ptr")
TRACE_MSG(MMIVIRTUALARRAY_1874_112_2_18_2_21_11_631,"MMIFMM:ReplaceSelSort PNULL == wa_num")
TRACE_MSG(MMIVIRTUALARRAY_1880_112_2_18_2_21_11_632,"MMIFMM:ReplaceSelSort param error!")
TRACE_MSG(MMIVIRTUALARRAY_1888_112_2_18_2_21_11_633,"MMIFMM:ReplaceSelSort error:PNULL == switch_data")
TRACE_MSG(MMIVIRTUALARRAY_1948_112_2_18_2_21_11_634,"MMIFMM:GetNextMergeGroup ways = %d")
TRACE_MSG(MMIVIRTUALARRAY_1998_112_2_18_2_21_11_635,"MMIFMM:Merge param error!")
TRACE_MSG(MMIVIRTUALARRAY_2008_112_2_18_2_21_11_636,"MMIFMM:Merge 0 == data_size")
TRACE_MSG(MMIVIRTUALARRAY_2250_112_2_18_2_21_11_637,"MMIFMM:UpdateAssoFile PNULL == v_header_ptr")
TRACE_MSG(MMIVIRTUALARRAY_2349_112_2_18_2_21_12_638,"MMIFMM:DeleteBufFromFile PNULL == buffer_ptr")
TRACE_MSG(MMIVIRTUALARRAY_SQL_114_112_2_18_2_21_13_639,"GetFilePathBySapce, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_154_112_2_18_2_21_13_640,"CreateAssoFile, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_217_112_2_18_2_21_13_641,"CreateTable, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_224_112_2_18_2_21_13_642,"CreateTable, create talbe fail.")
TRACE_MSG(MMIVIRTUALARRAY_SQL_234_112_2_18_2_21_13_643,"CreateTable, create index fail.")
TRACE_MSG(MMIVIRTUALARRAY_SQL_259_112_2_18_2_21_13_644,"GetSize, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_298_112_2_18_2_21_13_645,"IfReachLimitCount, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_314_112_2_18_2_21_13_646,"IfReachLimitCount, invalid mode!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_639_112_2_18_2_21_14_647,"ArrayCombine, sqlite3_exec insert error!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_673_112_2_18_2_21_14_648,"MMIVIRTUALARRAY_Create, no enough memory!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_682_112_2_18_2_21_14_649,"MMIVIRTUALARRAY_Create, GetFilePathBySapce error!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_711_112_2_18_2_21_14_650,"MMIVIRTUALARRAY_Create, error!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_777_112_2_18_2_21_14_651,"MMIVIRTUALARRAY_GetArraySize, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_838_112_2_18_2_21_14_652,"MMIVIRTUALARRAY_AddItem, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_844_112_2_18_2_21_14_653,"MMIVIRTUALARRAY_AddItem, data_ptr = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_866_112_2_18_2_21_14_654,"MMIVIRTUALARRAY_AddItem, error:%s.")
TRACE_MSG(MMIVIRTUALARRAY_SQL_893_112_2_18_2_21_14_655,"MMIVIRTUALARRAY_RemoveItemById, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_900_112_2_18_2_21_14_656,"MMIVIRTUALARRAY_RemoveItemById, invalid id.")
TRACE_MSG(MMIVIRTUALARRAY_SQL_916_112_2_18_2_21_14_657,"MMIVIRTUALARRAY_RemoveItemById, error:%s.")
TRACE_MSG(MMIVIRTUALARRAY_SQL_948_112_2_18_2_21_14_658,"MMIVIRTUALARRAY_UpdateItemById, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_954_112_2_18_2_21_14_659,"MMIVIRTUALARRAY_UpdateItemById, data_ptr = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_961_112_2_18_2_21_14_660,"MMIVIRTUALARRAY_UpdateItemById, invalid id.")
TRACE_MSG(MMIVIRTUALARRAY_SQL_977_112_2_18_2_21_14_661,"MMIVIRTUALARRAY_UpdateItemById, error:%s.")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1007_112_2_18_2_21_14_662,"MMIVIRTUALARRAY_GetItemById, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1013_112_2_18_2_21_14_663,"MMIVIRTUALARRAY_GetItemById, data_ptr = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1020_112_2_18_2_21_14_664,"MMIVIRTUALARRAY_GetItemById, invalid id.")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1060_112_2_18_2_21_15_665,"MMIVIRTUALARRAY_GetItemById, error:%s.")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1145_112_2_18_2_21_15_666,"MMIVIRTUALARRAY_Combine, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1158_112_2_18_2_21_15_667,"MMIVIRTUALARRAY_Combine, array1 reach limit count!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1192_112_2_18_2_21_15_668,"MMIVIRTUALARRAY_SaveToFile, input param error!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1199_112_2_18_2_21_15_669,"MMIVIRTUALARRAY_SaveToFile, create file error!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1230_112_2_18_2_21_15_670,"MMIVIRTUALARRAY_SaveToFile, attach error:%s!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1252_112_2_18_2_21_15_671,"MMIVIRTUALARRAY_LoadFromFile, input param error!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1266_112_2_18_2_21_15_672,"MMIVIRTUALARRAY_LoadFromFile, copy file error!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1279_112_2_18_2_21_15_673,"MMIVIRTUALARRAY_LoadFromFile, FILE_MODE open db error!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1292_112_2_18_2_21_15_674,"MMIVIRTUALARRAY_LoadFromFile, MEMORY_MODE open db error!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1319_112_2_18_2_21_15_675,"MMIVIRTUALARRAY_Transaction, handle = 0!")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1346_112_2_18_2_21_15_676,"sqlite_memory_status, nCur = %d,nHigh = %d.")
TRACE_MSG(MMIVIRTUALARRAY_SQL_1353_112_2_18_2_21_15_677,"sqlite free all.")
END_TRACE_MAP(MMI_APP_FMM_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_FMM_TRC_H_


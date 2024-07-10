/******************************************************************************
 ** File Name:      ms_ref_fota_trc.h                                         *
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
//trace_id:48
#ifndef _MS_REF_FOTA_TRC_H_
#define _MS_REF_FOTA_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define NV_UPDATE_80_112_2_18_1_31_22_0 "Failed to load FIXNV partition entry (0x%x)\r\n"
#define NV_UPDATE_87_112_2_18_1_31_22_1 "Failed to load Updated NV partition entry (0x%x)\r\n"
#define NV_UPDATE_122_112_2_18_1_31_22_2 "Failed to read UP parition (0x%x)\r\n"
#define NV_UPDATE_179_112_2_18_1_31_22_3 "Failed to read UP parition (0x%x)\r\n"
#define NV_UPDATE_224_112_2_18_1_31_22_4 "Failed to read UP parition (0x%x)\r\n"
#define NV_UPDATE_281_112_2_18_1_31_22_5 "Failed to read UP parition (0x%x)\r\n"
#define NV_UPDATE_321_112_2_18_1_31_23_6 "Analysis fixnv error0 !!!"
#define NV_UPDATE_367_112_2_18_1_31_23_7 "Failed to BML_MRead\r\n"
#define NV_UPDATE_410_112_2_18_1_31_23_8 "Analysis fixnv error1	!!!!"
#define NV_UPDATE_439_112_2_18_1_31_23_9 "Failed to load DM DATA partition entry (0x%x)\r\n"
#define NV_UPDATE_446_112_2_18_1_31_23_10 "Failed to erase update flag parition (0x%x)\r\n"
#define NV_UPDATE_466_112_2_18_1_31_23_11 "Failed to load DM DATA partition entry (0x%x)\r\n"
#define NV_UPDATE_477_112_2_18_1_31_23_12 "Failed to read UP parition (0x%x)\r\n"
#define NV_UPDATE_501_112_2_18_1_31_23_13 "Failed to load DM DATA partition entry (0x%x)\r\n"
#define NV_UPDATE_511_112_2_18_1_31_23_14 "Failed to BML_MRead\r\n"
#define RB_IMAGEUPDATE_78_112_2_18_1_31_23_15 "RB_GetDelta: Failed to load Delta partition entry (0x%x)\r\n"
#define RB_IMAGEUPDATE_98_112_2_18_1_31_23_16 "RB_GetDelta: copy data failed.\r\n"
#define RB_IMAGEUPDATE_145_112_2_18_1_31_23_17 "RB_ReadBackupBlock:read data failed (0x%x).\r\n"
#define RB_IMAGEUPDATE_181_112_2_18_1_31_23_18 "RB_WriteBackupBlock: Failed to erase backup block (0x%x)\r\n"
#define RB_IMAGEUPDATE_190_112_2_18_1_31_24_19 "Failed to write fota flag parition (0x%x)\r\n"
#define RB_IMAGEUPDATE_227_112_2_18_1_31_24_20 "Failed to read UP parition (0x%x)\r\n"
#define RB_IMAGEUPDATE_234_112_2_18_1_31_24_21 "RB_WriteBlock: Failed to erase image block(0x%x)\r\n"
#define RB_IMAGEUPDATE_243_112_2_18_1_31_24_22 "RB_WriteBlock: Failed to write image (0x%x)\r\n"
#define RB_IMAGEUPDATE_249_112_2_18_1_31_24_23 "Failed to read UP parition (0x%x)\r\n"
#define RB_IMAGEUPDATE_256_112_2_18_1_31_24_24 "RB_WriteBlock: Failed to erase image block(0x%x)\r\n"
#define RB_IMAGEUPDATE_263_112_2_18_1_31_24_25 "RB_WriteBlock: Failed to write image (0x%x)\r\n"
#define RB_IMAGEUPDATE_271_112_2_18_1_31_24_26 "RB_WriteBlock: Failed to erase image block(0x%x)\r\n"
#define RB_IMAGEUPDATE_280_112_2_18_1_31_24_27 "RB_WriteBlock: Failed to write image (0x%x)\r\n"
#define RB_IMAGEUPDATE_329_112_2_18_1_31_24_28 "RB_ReadImage:copy data failed.\r\n"
#define RB_IMAGEUPDATE_361_112_2_18_1_31_24_29 "Error: (M,D) =(0x%x,0x%x); use default CheckDev = 13 \r\n"
#define SPRDFTL_NV_UPDATE_150_112_2_18_1_31_25_30 "_Scan_NV_Buffer_For_Backup: Input para error.\r\n"
#define SPRDFTL_NV_UPDATE_222_112_2_18_1_31_26_31 "Load_FixnvEntry: Open FIXNV partition error (0x%x)\r\n"
#define SPRDFTL_NV_UPDATE_236_112_2_18_1_31_26_32 "Load_FixnvEntry: Open Updated partition error (0x%x)\r\n"
#define SPRDFTL_NV_UPDATE_281_112_2_18_1_31_26_33 "update_fixnv_image: Can not open fixed or update nv partition.\r\n"
#define SPRDFTL_NV_UPDATE_292_112_2_18_1_31_26_34 "update_fixnv_image: Fixed NV size more than g_DownloadNV_Buffer\r\n"
#define SPRDFTL_NV_UPDATE_302_112_2_18_1_31_26_35 "update_fixnv_image: Read fixed nv partition error(0x%x) \r\n"
#define SPRDFTL_NV_UPDATE_312_112_2_18_1_31_26_36 "update_fixnv_image: Update NV size more than g_UpdateNV_Buffer\r\n"
#define SPRDFTL_NV_UPDATE_322_112_2_18_1_31_26_37 "update_fixnv_image: Read update nv partition error(0x%x) \r\n"
#define SPRDFTL_NV_UPDATE_333_112_2_18_1_31_26_38 "update_fixnv_image: Scan fixed nv buffer error\r\n"
#define SPRDFTL_NV_UPDATE_342_112_2_18_1_31_26_39 "update_fixnv_image: Scan update nv buffer error\r\n"
#define SPRDFTL_NV_UPDATE_365_112_2_18_1_31_26_40 "update_fixnv_image: Copy backup items error.\r\n "
#define SPRDFTL_NV_UPDATE_372_112_2_18_1_31_26_41 "update_fixnv_image: Download NV and Update NV incompatible.\r\n"
#define SPRDFTL_NV_UPDATE_393_112_2_18_1_31_26_42 "update_fixnv_image: Get download nv partition info error (0x%x)\r\n"
#define SPRDFTL_NV_UPDATE_401_112_2_18_1_31_26_43 "update_fixnv_image: Erase download nv error (0x%x)\r\n"
#define SPRDFTL_NV_UPDATE_412_112_2_18_1_31_26_44 "update_fixnv_image: Write download nv error (0x%x)\r\n"
#define SPRDFTL_NV_UPDATE_439_112_2_18_1_31_26_45 "erase_running_nv_partition: Failed to open running nv partition, ret(0x%x)\r\n"
#define SPRDFTL_NV_UPDATE_447_112_2_18_1_31_26_46 "erase_running_nv_partition: Get running nv parition info err (0x%x)\r\n"
#define SPRDFTL_NV_UPDATE_453_112_2_18_1_31_26_47 "erase_running_nv_partition: Failed to erase running nv parition (0x%x)\r\n"
#define SPRDFTL_NV_UPDATE_466_112_2_18_1_31_26_48 "erase_running_nv_partition: Failed to write 0xFF into running nv parition (0x%x)\r\n"
#define SPRDFTL_NV_UPDATE_504_112_2_18_1_31_26_49 "Update_Kernel_Head: Open SCI_PARTITION_OSB error (0x%x)\r\n"
#define SPRDFTL_NV_UPDATE_518_112_2_18_1_31_26_50 "Update_Kernel_Head: Read block(%d) error.\r\n"
#define SPRDFTL_NV_UPDATE_565_112_2_18_1_31_26_51 "Update_Kernel_Head: Can not find \"STBP\" signature.\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_89_112_2_18_1_31_27_52 "RB_GetDelta: Input parameter pbBuffer == NULL\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_104_112_2_18_1_31_27_53 "RB_GetDelta: Failed to load Delta partition entry (0x%x)\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_123_112_2_18_1_31_27_54 "RB_GetDelta:Input dwStartAddressOffset error\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_129_112_2_18_1_31_27_55 "RB_GetDelta:Input dwSize error\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_135_112_2_18_1_31_27_56 "RB_GetDelta: g_buffer size error\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_143_112_2_18_1_31_27_57 "RB_GetDelta:Failed to read SCI_PARTITION_DU parition (0x%x)\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_174_112_2_18_1_31_27_58 "RB_ReadBackupBlock: Input parameter pbBuffer == NULL\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_194_112_2_18_1_31_27_59 "RB_ReadBackupBlock: g_buffer size error\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_202_112_2_18_1_31_27_60 "RB_ReadBackupBlock:read data failed (0x%x).\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_231_112_2_18_1_31_27_61 "RB_WriteBackupBlock: Input pbBuffer NULL \r\n"
#define SPRDFTL_RB_IMAGEUPDATE_238_112_2_18_1_31_27_62 "RB_WriteBackupBlock: Input address not align to BLOCK (0x%x)\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_246_112_2_18_1_31_27_63 "RB_WriteBackupBlock: Failed to write backup block (0x%x)\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_274_112_2_18_1_31_27_64 "RB_WriteBlock: Input pbBuffer NULL \r\n"
#define SPRDFTL_RB_IMAGEUPDATE_281_112_2_18_1_31_27_65 "RB_WriteBlock: Input address not align to BLOCK (0x%x)\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_289_112_2_18_1_31_27_66 "RB_WriteBlock: Failed to write backup block (0x%x)\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_319_112_2_18_1_31_27_67 "RB_ReadImage: Input parameter pbBuffer == NULL\r\n"
#define SPRDFTL_RB_IMAGEUPDATE_344_112_2_18_1_31_27_68 "RB_ReadImage:copy data failed.\r\n"
#define SPRDFTL_UA_426_112_2_18_1_31_29_69 "Failed to load update flag entry (0x%x)\r\n"
#define SPRDFTL_UA_435_112_2_18_1_31_29_70 "SetUpdateProcessFlag: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn"
#define SPRDFTL_UA_447_112_2_18_1_31_29_71 "Failed to read SCI_PARTITION_DU parition (0x%x)\r\n"
#define SPRDFTL_UA_467_112_2_18_1_31_29_72 "SetUpdateProcessFlag: SCI_FTL_Erase SCI_PARTITION_DU parition err (0x%x)\r\n"
#define SPRDFTL_UA_475_112_2_18_1_31_29_73 "SetUpdateProcessFlag: SCI_FTL_Write SCI_PARTITION_DU parition err (0x%x)\r\n"
#define SPRDFTL_UA_502_112_2_18_1_31_29_74 "Failed to load update flag entry (0x%x)\r\n"
#define SPRDFTL_UA_511_112_2_18_1_31_29_75 "ClearUpdateFlag: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn"
#define SPRDFTL_UA_523_112_2_18_1_31_29_76 "Failed to read SCI_PARTITION_DU parition (0x%x)\r\n"
#define SPRDFTL_UA_535_112_2_18_1_31_29_77 "ClearUpdateFlag: SCI_FTL_Erase SCI_PARTITION_DU parition err (0x%x)\r\n"
#define SPRDFTL_UA_547_112_2_18_1_31_29_78 "ClearUpdateFlag: SCI_FTL_Write SCI_PARTITION_DU parition err (0x%x)\r\n"
#define SPRDFTL_UA_565_112_2_18_1_31_29_79 "ClearUpdateFlag: SCI_FTL_Write SCI_PARTITION_DU parition err (0x%x)\r\n"
#define SPRDFTL_UA_588_112_2_18_1_31_29_80 "Failed to load update flag entry (0x%x)\r\n"
#define SPRDFTL_UA_597_112_2_18_1_31_29_81 "ReadUpdateProcessFlag: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn"
#define SPRDFTL_UA_610_112_2_18_1_31_29_82 "Failed to read SCI_PARTITION_DU parition (0x%x)\r\n"
#define SPRDFTL_UA_640_112_2_18_1_31_29_83 "Input pointer should not be NULL!"
#define SPRDFTL_UA_648_112_2_18_1_31_29_84 "Failed to open PARTITION(0x%x): Error (0x%x)\r\n"
#define SPRDFTL_UA_680_112_2_18_1_31_29_85 "GetImageInfo() input parameter error\r\n"
#define SPRDFTL_UA_705_112_2_18_1_31_29_86 "GetImageInfo: _get_one_image_map_ionfo (partition 0x%x) get error.rn"
#define SPRDFTL_UA_714_112_2_18_1_31_29_87 "Failed to open PARTITION(0x%x)\r\n"
#define SPRDFTL_UA_731_112_2_18_1_31_29_88 "Failed to read SCI_PARTITION_OSB parition (0x%x)\r\n"
#define SPRDFTL_UA_798_112_2_18_1_31_29_89 "VerifySourceVersion: Failed to open PARTITION(0x%x): Error (0x%x)\r\n"
#define SPRDFTL_UA_807_112_2_18_1_31_29_90 "VerifySourceVersion: UBB partiton's blk(%d) > MAX_UBB_BLK_NUM(%d)"
#define SPRDFTL_UA_855_112_2_18_1_31_29_91 "RB_GetSectionsData failed  \r\n  "
#define SPRDFTL_UA_865_112_2_18_1_31_29_92 "ERR_FOTA_LOST: lost file.\r\n"
#define SPRDFTL_UA_870_112_2_18_1_31_30_93 "ERR_FOTA_SOURCE_NOT_COMP: map[%d] : 0x%08x - 0x%08x\r\n "
#define SPRDFTL_UA_875_112_2_18_1_31_30_94 "ERR_FOTA_FILE_NOT_COMP: map[%d] : 0x%08x - 0x%08x \r\n  "
#define SPRDFTL_UA_935_112_2_18_1_31_30_95 "ImageUpdate: Failed to open PARTITION(0x%x): Error (0x%x)\r\n"
#define SPRDFTL_UA_944_112_2_18_1_31_30_96 "ImageUpdate: UBB partiton's blk(%d) > MAX_UBB_BLK_NUM(%d)"
#define SPRDFTL_UA_1051_112_2_18_1_31_30_97 "main: SCI_FTL_MODULE_Init err(0x%x)\r\n"
#define SPRDFTL_UA_1058_112_2_18_1_31_30_98 "main: SCI_FTL_Load err(0x%x)\r\n"
#define SPRDFTL_UA_1066_112_2_18_1_31_30_99 "main: SCI_FTL_GetDevInfo err(0x%x)\r\n"
#define SPRDFTL_UA_1079_112_2_18_1_31_30_100 "main: Part entry num(%d) more than g_ua_PartI size (%d). rn"
#define SPRDFTL_UA_1100_112_2_18_1_31_30_101 "main: SCI_FTL_Open(ID 0x%x) err(0x%x). rn"
#define SPRDFTL_UA_1113_112_2_18_1_31_30_102 "main: (sizeof(g_ua_mbuf) <  g_Sector_Size * g_SectorsPerBlk_Ua)\r\n"
#define SPRDFTL_UA_1133_112_2_18_1_31_30_103 "Failed to get image start address and end address\r\n"
#define SPRDFTL_UA_1243_112_2_18_1_31_30_104 "change update flag failed(0x%x)\r\n"
#define SPRDFTL_UA_1258_112_2_18_1_31_30_105 "main: SCI_FTL_Close(ID 0x%x) err(0x%x). rn"
#define SPRDFTL_UA_1266_112_2_18_1_31_30_106 "main: SCI_FTL_UnLoad(ID 0x%x) err(0x%x). rn"
#define SPRDFTL_UA_1318_112_2_18_1_31_30_107 "Input dst pointer null! \r\n"
#define SPRDFTL_UA_1354_112_2_18_1_31_30_108 "Failed to read nand flash by sector (0x%x)\r\n"
#define SPRDFTL_UA_1361_112_2_18_1_31_30_109 "ReadNandflashBysector: Can not find partition, Sct_Idx(%d) Sct_Num(%d).rn"
#define SPRDFTL_UA_1389_112_2_18_1_31_31_110 "Input pWBuf pointer null! \r\n"
#define SPRDFTL_UA_1423_112_2_18_1_31_31_111 "WriteNandflashByBlk: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn"
#define SPRDFTL_UA_1437_112_2_18_1_31_31_112 "WriteNandflashByBlk: align error. nWSct_offset(%d) nWSct_num(%d) partInfo.eraseUnit(%d)rn"
#define SPRDFTL_UA_1445_112_2_18_1_31_31_113 "WriteNandflashByBlk: not support erase STL partition(0x%x) in fota rn"
#define SPRDFTL_UA_1452_112_2_18_1_31_31_114 "WriteNandflashByBlk: Failed to erase nand flash (0x%x)\r\n"
#define SPRDFTL_UA_1461_112_2_18_1_31_31_115 "WriteNandflashByBlk: Failed to write nand flash (0x%x)\r\n"
#define SPRDFTL_UA_1468_112_2_18_1_31_31_116 "ReadNandflashBysector: Can not find partition, Blk_Idx(%d) Blk_Num(%d).rn"
#define UA_350_112_2_18_1_31_32_117 "Failed to load update flag entry (0x%x)\r\n"
#define UA_359_112_2_18_1_31_32_118 "Failed to read UP parition (0x%x)\r\n"
#define UA_383_112_2_18_1_31_32_119 "Failed to load update flag entry (0x%x)\r\n"
#define UA_392_112_2_18_1_31_32_120 "Failed to read UP parition (0x%x)\r\n"
#define UA_401_112_2_18_1_31_32_121 "Failed to erase update flag parition (0x%x)\r\n"
#define UA_426_112_2_18_1_31_32_122 "Failed to load update flag entry (0x%x)\r\n"
#define UA_435_112_2_18_1_31_32_123 "Failed to read UP parition (0x%x)\r\n"
#define UA_452_112_2_18_1_31_32_124 "Failed to load update flag entry (0x%x)\r\n"
#define UA_460_112_2_18_1_31_32_125 "part[%d]:  (0x%x, 0x%x), blk_num %d \r\n"
#define UA_483_112_2_18_1_31_32_126 "Failed to load update flag entry (0x%x)\r\n"
#define UA_494_112_2_18_1_31_32_127 "Failed to load update flag entry (0x%x)\r\n"
#define UA_507_112_2_18_1_31_32_128 "Kernel Image:  (0x%x, 0x%x)\r\n"
#define UA_537_112_2_18_1_31_32_129 "Failed to load update backup buffer entry (0x%x)\r\n"
#define UA_583_112_2_18_1_31_32_130 "RB_GetSectionsData failed  \r\n  "
#define UA_592_112_2_18_1_31_32_131 "Err1: lost file.\r\n"
#define UA_597_112_2_18_1_31_32_132 "Err2: map[%d] : 0x%08x - 0x%08x\r\n "
#define UA_602_112_2_18_1_31_32_133 "Err3: map[%d] : 0x%08x - 0x%08x \r\n  "
#define UA_638_112_2_18_1_31_32_134 "Failed to load update backup buffer entry (0x%x)\r\n"
#define UA_682_112_2_18_1_31_32_135 "enter fota process..............\r\n"
#define UA_700_112_2_18_1_31_32_136 "hight %d width %d"
#define UA_713_112_2_18_1_31_32_137 "BML_Init failed(0x%x).\r\n"
#define UA_720_112_2_18_1_31_32_138 "BML_Open failed(0x%x).\r\n"
#define UA_727_112_2_18_1_31_32_139 "BML_Unlock failed(0x%x).\r\n"
#define UA_734_112_2_18_1_31_32_140 "BML_GetVolInfo information failed(0x%x).\r\n"
#define UA_748_112_2_18_1_31_32_141 "Failed to get image start address and end address\r\n"
#define UA_794_112_2_18_1_31_33_142 "change update flag failed(0x%x)\r\n"
#define UA_801_112_2_18_1_31_33_143 "BML_Close failed(0x%x).\r\n"
#define UA_833_112_2_18_1_31_33_144 "Failed to read nand flash by sector (0x%x).\r\n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_FOTA_TRC)
TRACE_MSG(NV_UPDATE_80_112_2_18_1_31_22_0,"Failed to load FIXNV partition entry (0x%x)\r\n")
TRACE_MSG(NV_UPDATE_87_112_2_18_1_31_22_1,"Failed to load Updated NV partition entry (0x%x)\r\n")
TRACE_MSG(NV_UPDATE_122_112_2_18_1_31_22_2,"Failed to read UP parition (0x%x)\r\n")
TRACE_MSG(NV_UPDATE_179_112_2_18_1_31_22_3,"Failed to read UP parition (0x%x)\r\n")
TRACE_MSG(NV_UPDATE_224_112_2_18_1_31_22_4,"Failed to read UP parition (0x%x)\r\n")
TRACE_MSG(NV_UPDATE_281_112_2_18_1_31_22_5,"Failed to read UP parition (0x%x)\r\n")
TRACE_MSG(NV_UPDATE_321_112_2_18_1_31_23_6,"Analysis fixnv error0 !!!")
TRACE_MSG(NV_UPDATE_367_112_2_18_1_31_23_7,"Failed to BML_MRead\r\n")
TRACE_MSG(NV_UPDATE_410_112_2_18_1_31_23_8,"Analysis fixnv error1	!!!!")
TRACE_MSG(NV_UPDATE_439_112_2_18_1_31_23_9,"Failed to load DM DATA partition entry (0x%x)\r\n")
TRACE_MSG(NV_UPDATE_446_112_2_18_1_31_23_10,"Failed to erase update flag parition (0x%x)\r\n")
TRACE_MSG(NV_UPDATE_466_112_2_18_1_31_23_11,"Failed to load DM DATA partition entry (0x%x)\r\n")
TRACE_MSG(NV_UPDATE_477_112_2_18_1_31_23_12,"Failed to read UP parition (0x%x)\r\n")
TRACE_MSG(NV_UPDATE_501_112_2_18_1_31_23_13,"Failed to load DM DATA partition entry (0x%x)\r\n")
TRACE_MSG(NV_UPDATE_511_112_2_18_1_31_23_14,"Failed to BML_MRead\r\n")
TRACE_MSG(RB_IMAGEUPDATE_78_112_2_18_1_31_23_15,"RB_GetDelta: Failed to load Delta partition entry (0x%x)\r\n")
TRACE_MSG(RB_IMAGEUPDATE_98_112_2_18_1_31_23_16,"RB_GetDelta: copy data failed.\r\n")
TRACE_MSG(RB_IMAGEUPDATE_145_112_2_18_1_31_23_17,"RB_ReadBackupBlock:read data failed (0x%x).\r\n")
TRACE_MSG(RB_IMAGEUPDATE_181_112_2_18_1_31_23_18,"RB_WriteBackupBlock: Failed to erase backup block (0x%x)\r\n")
TRACE_MSG(RB_IMAGEUPDATE_190_112_2_18_1_31_24_19,"Failed to write fota flag parition (0x%x)\r\n")
TRACE_MSG(RB_IMAGEUPDATE_227_112_2_18_1_31_24_20,"Failed to read UP parition (0x%x)\r\n")
TRACE_MSG(RB_IMAGEUPDATE_234_112_2_18_1_31_24_21,"RB_WriteBlock: Failed to erase image block(0x%x)\r\n")
TRACE_MSG(RB_IMAGEUPDATE_243_112_2_18_1_31_24_22,"RB_WriteBlock: Failed to write image (0x%x)\r\n")
TRACE_MSG(RB_IMAGEUPDATE_249_112_2_18_1_31_24_23,"Failed to read UP parition (0x%x)\r\n")
TRACE_MSG(RB_IMAGEUPDATE_256_112_2_18_1_31_24_24,"RB_WriteBlock: Failed to erase image block(0x%x)\r\n")
TRACE_MSG(RB_IMAGEUPDATE_263_112_2_18_1_31_24_25,"RB_WriteBlock: Failed to write image (0x%x)\r\n")
TRACE_MSG(RB_IMAGEUPDATE_271_112_2_18_1_31_24_26,"RB_WriteBlock: Failed to erase image block(0x%x)\r\n")
TRACE_MSG(RB_IMAGEUPDATE_280_112_2_18_1_31_24_27,"RB_WriteBlock: Failed to write image (0x%x)\r\n")
TRACE_MSG(RB_IMAGEUPDATE_329_112_2_18_1_31_24_28,"RB_ReadImage:copy data failed.\r\n")
TRACE_MSG(RB_IMAGEUPDATE_361_112_2_18_1_31_24_29,"Error: (M,D) =(0x%x,0x%x); use default CheckDev = 13 \r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_150_112_2_18_1_31_25_30,"_Scan_NV_Buffer_For_Backup: Input para error.\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_222_112_2_18_1_31_26_31,"Load_FixnvEntry: Open FIXNV partition error (0x%x)\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_236_112_2_18_1_31_26_32,"Load_FixnvEntry: Open Updated partition error (0x%x)\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_281_112_2_18_1_31_26_33,"update_fixnv_image: Can not open fixed or update nv partition.\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_292_112_2_18_1_31_26_34,"update_fixnv_image: Fixed NV size more than g_DownloadNV_Buffer\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_302_112_2_18_1_31_26_35,"update_fixnv_image: Read fixed nv partition error(0x%x) \r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_312_112_2_18_1_31_26_36,"update_fixnv_image: Update NV size more than g_UpdateNV_Buffer\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_322_112_2_18_1_31_26_37,"update_fixnv_image: Read update nv partition error(0x%x) \r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_333_112_2_18_1_31_26_38,"update_fixnv_image: Scan fixed nv buffer error\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_342_112_2_18_1_31_26_39,"update_fixnv_image: Scan update nv buffer error\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_365_112_2_18_1_31_26_40,"update_fixnv_image: Copy backup items error.\r\n ")
TRACE_MSG(SPRDFTL_NV_UPDATE_372_112_2_18_1_31_26_41,"update_fixnv_image: Download NV and Update NV incompatible.\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_393_112_2_18_1_31_26_42,"update_fixnv_image: Get download nv partition info error (0x%x)\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_401_112_2_18_1_31_26_43,"update_fixnv_image: Erase download nv error (0x%x)\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_412_112_2_18_1_31_26_44,"update_fixnv_image: Write download nv error (0x%x)\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_439_112_2_18_1_31_26_45,"erase_running_nv_partition: Failed to open running nv partition, ret(0x%x)\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_447_112_2_18_1_31_26_46,"erase_running_nv_partition: Get running nv parition info err (0x%x)\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_453_112_2_18_1_31_26_47,"erase_running_nv_partition: Failed to erase running nv parition (0x%x)\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_466_112_2_18_1_31_26_48,"erase_running_nv_partition: Failed to write 0xFF into running nv parition (0x%x)\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_504_112_2_18_1_31_26_49,"Update_Kernel_Head: Open SCI_PARTITION_OSB error (0x%x)\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_518_112_2_18_1_31_26_50,"Update_Kernel_Head: Read block(%d) error.\r\n")
TRACE_MSG(SPRDFTL_NV_UPDATE_565_112_2_18_1_31_26_51,"Update_Kernel_Head: Can not find \"STBP\" signature.\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_89_112_2_18_1_31_27_52,"RB_GetDelta: Input parameter pbBuffer == NULL\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_104_112_2_18_1_31_27_53,"RB_GetDelta: Failed to load Delta partition entry (0x%x)\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_123_112_2_18_1_31_27_54,"RB_GetDelta:Input dwStartAddressOffset error\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_129_112_2_18_1_31_27_55,"RB_GetDelta:Input dwSize error\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_135_112_2_18_1_31_27_56,"RB_GetDelta: g_buffer size error\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_143_112_2_18_1_31_27_57,"RB_GetDelta:Failed to read SCI_PARTITION_DU parition (0x%x)\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_174_112_2_18_1_31_27_58,"RB_ReadBackupBlock: Input parameter pbBuffer == NULL\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_194_112_2_18_1_31_27_59,"RB_ReadBackupBlock: g_buffer size error\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_202_112_2_18_1_31_27_60,"RB_ReadBackupBlock:read data failed (0x%x).\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_231_112_2_18_1_31_27_61,"RB_WriteBackupBlock: Input pbBuffer NULL \r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_238_112_2_18_1_31_27_62,"RB_WriteBackupBlock: Input address not align to BLOCK (0x%x)\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_246_112_2_18_1_31_27_63,"RB_WriteBackupBlock: Failed to write backup block (0x%x)\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_274_112_2_18_1_31_27_64,"RB_WriteBlock: Input pbBuffer NULL \r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_281_112_2_18_1_31_27_65,"RB_WriteBlock: Input address not align to BLOCK (0x%x)\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_289_112_2_18_1_31_27_66,"RB_WriteBlock: Failed to write backup block (0x%x)\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_319_112_2_18_1_31_27_67,"RB_ReadImage: Input parameter pbBuffer == NULL\r\n")
TRACE_MSG(SPRDFTL_RB_IMAGEUPDATE_344_112_2_18_1_31_27_68,"RB_ReadImage:copy data failed.\r\n")
TRACE_MSG(SPRDFTL_UA_426_112_2_18_1_31_29_69,"Failed to load update flag entry (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_435_112_2_18_1_31_29_70,"SetUpdateProcessFlag: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn")
TRACE_MSG(SPRDFTL_UA_447_112_2_18_1_31_29_71,"Failed to read SCI_PARTITION_DU parition (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_467_112_2_18_1_31_29_72,"SetUpdateProcessFlag: SCI_FTL_Erase SCI_PARTITION_DU parition err (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_475_112_2_18_1_31_29_73,"SetUpdateProcessFlag: SCI_FTL_Write SCI_PARTITION_DU parition err (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_502_112_2_18_1_31_29_74,"Failed to load update flag entry (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_511_112_2_18_1_31_29_75,"ClearUpdateFlag: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn")
TRACE_MSG(SPRDFTL_UA_523_112_2_18_1_31_29_76,"Failed to read SCI_PARTITION_DU parition (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_535_112_2_18_1_31_29_77,"ClearUpdateFlag: SCI_FTL_Erase SCI_PARTITION_DU parition err (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_547_112_2_18_1_31_29_78,"ClearUpdateFlag: SCI_FTL_Write SCI_PARTITION_DU parition err (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_565_112_2_18_1_31_29_79,"ClearUpdateFlag: SCI_FTL_Write SCI_PARTITION_DU parition err (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_588_112_2_18_1_31_29_80,"Failed to load update flag entry (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_597_112_2_18_1_31_29_81,"ReadUpdateProcessFlag: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn")
TRACE_MSG(SPRDFTL_UA_610_112_2_18_1_31_29_82,"Failed to read SCI_PARTITION_DU parition (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_640_112_2_18_1_31_29_83,"Input pointer should not be NULL!")
TRACE_MSG(SPRDFTL_UA_648_112_2_18_1_31_29_84,"Failed to open PARTITION(0x%x): Error (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_680_112_2_18_1_31_29_85,"GetImageInfo() input parameter error\r\n")
TRACE_MSG(SPRDFTL_UA_705_112_2_18_1_31_29_86,"GetImageInfo: _get_one_image_map_ionfo (partition 0x%x) get error.rn")
TRACE_MSG(SPRDFTL_UA_714_112_2_18_1_31_29_87,"Failed to open PARTITION(0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_731_112_2_18_1_31_29_88,"Failed to read SCI_PARTITION_OSB parition (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_798_112_2_18_1_31_29_89,"VerifySourceVersion: Failed to open PARTITION(0x%x): Error (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_807_112_2_18_1_31_29_90,"VerifySourceVersion: UBB partiton's blk(%d) > MAX_UBB_BLK_NUM(%d)")
TRACE_MSG(SPRDFTL_UA_855_112_2_18_1_31_29_91,"RB_GetSectionsData failed  \r\n  ")
TRACE_MSG(SPRDFTL_UA_865_112_2_18_1_31_29_92,"ERR_FOTA_LOST: lost file.\r\n")
TRACE_MSG(SPRDFTL_UA_870_112_2_18_1_31_30_93,"ERR_FOTA_SOURCE_NOT_COMP: map[%d] : 0x%08x - 0x%08x\r\n ")
TRACE_MSG(SPRDFTL_UA_875_112_2_18_1_31_30_94,"ERR_FOTA_FILE_NOT_COMP: map[%d] : 0x%08x - 0x%08x \r\n  ")
TRACE_MSG(SPRDFTL_UA_935_112_2_18_1_31_30_95,"ImageUpdate: Failed to open PARTITION(0x%x): Error (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_944_112_2_18_1_31_30_96,"ImageUpdate: UBB partiton's blk(%d) > MAX_UBB_BLK_NUM(%d)")
TRACE_MSG(SPRDFTL_UA_1051_112_2_18_1_31_30_97,"main: SCI_FTL_MODULE_Init err(0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_1058_112_2_18_1_31_30_98,"main: SCI_FTL_Load err(0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_1066_112_2_18_1_31_30_99,"main: SCI_FTL_GetDevInfo err(0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_1079_112_2_18_1_31_30_100,"main: Part entry num(%d) more than g_ua_PartI size (%d). rn")
TRACE_MSG(SPRDFTL_UA_1100_112_2_18_1_31_30_101,"main: SCI_FTL_Open(ID 0x%x) err(0x%x). rn")
TRACE_MSG(SPRDFTL_UA_1113_112_2_18_1_31_30_102,"main: (sizeof(g_ua_mbuf) <  g_Sector_Size * g_SectorsPerBlk_Ua)\r\n")
TRACE_MSG(SPRDFTL_UA_1133_112_2_18_1_31_30_103,"Failed to get image start address and end address\r\n")
TRACE_MSG(SPRDFTL_UA_1243_112_2_18_1_31_30_104,"change update flag failed(0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_1258_112_2_18_1_31_30_105,"main: SCI_FTL_Close(ID 0x%x) err(0x%x). rn")
TRACE_MSG(SPRDFTL_UA_1266_112_2_18_1_31_30_106,"main: SCI_FTL_UnLoad(ID 0x%x) err(0x%x). rn")
TRACE_MSG(SPRDFTL_UA_1318_112_2_18_1_31_30_107,"Input dst pointer null! \r\n")
TRACE_MSG(SPRDFTL_UA_1354_112_2_18_1_31_30_108,"Failed to read nand flash by sector (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_1361_112_2_18_1_31_30_109,"ReadNandflashBysector: Can not find partition, Sct_Idx(%d) Sct_Num(%d).rn")
TRACE_MSG(SPRDFTL_UA_1389_112_2_18_1_31_31_110,"Input pWBuf pointer null! \r\n")
TRACE_MSG(SPRDFTL_UA_1423_112_2_18_1_31_31_111,"WriteNandflashByBlk: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn")
TRACE_MSG(SPRDFTL_UA_1437_112_2_18_1_31_31_112,"WriteNandflashByBlk: align error. nWSct_offset(%d) nWSct_num(%d) partInfo.eraseUnit(%d)rn")
TRACE_MSG(SPRDFTL_UA_1445_112_2_18_1_31_31_113,"WriteNandflashByBlk: not support erase STL partition(0x%x) in fota rn")
TRACE_MSG(SPRDFTL_UA_1452_112_2_18_1_31_31_114,"WriteNandflashByBlk: Failed to erase nand flash (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_1461_112_2_18_1_31_31_115,"WriteNandflashByBlk: Failed to write nand flash (0x%x)\r\n")
TRACE_MSG(SPRDFTL_UA_1468_112_2_18_1_31_31_116,"ReadNandflashBysector: Can not find partition, Blk_Idx(%d) Blk_Num(%d).rn")
TRACE_MSG(UA_350_112_2_18_1_31_32_117,"Failed to load update flag entry (0x%x)\r\n")
TRACE_MSG(UA_359_112_2_18_1_31_32_118,"Failed to read UP parition (0x%x)\r\n")
TRACE_MSG(UA_383_112_2_18_1_31_32_119,"Failed to load update flag entry (0x%x)\r\n")
TRACE_MSG(UA_392_112_2_18_1_31_32_120,"Failed to read UP parition (0x%x)\r\n")
TRACE_MSG(UA_401_112_2_18_1_31_32_121,"Failed to erase update flag parition (0x%x)\r\n")
TRACE_MSG(UA_426_112_2_18_1_31_32_122,"Failed to load update flag entry (0x%x)\r\n")
TRACE_MSG(UA_435_112_2_18_1_31_32_123,"Failed to read UP parition (0x%x)\r\n")
TRACE_MSG(UA_452_112_2_18_1_31_32_124,"Failed to load update flag entry (0x%x)\r\n")
TRACE_MSG(UA_460_112_2_18_1_31_32_125,"part[%d]:  (0x%x, 0x%x), blk_num %d \r\n")
TRACE_MSG(UA_483_112_2_18_1_31_32_126,"Failed to load update flag entry (0x%x)\r\n")
TRACE_MSG(UA_494_112_2_18_1_31_32_127,"Failed to load update flag entry (0x%x)\r\n")
TRACE_MSG(UA_507_112_2_18_1_31_32_128,"Kernel Image:  (0x%x, 0x%x)\r\n")
TRACE_MSG(UA_537_112_2_18_1_31_32_129,"Failed to load update backup buffer entry (0x%x)\r\n")
TRACE_MSG(UA_583_112_2_18_1_31_32_130,"RB_GetSectionsData failed  \r\n  ")
TRACE_MSG(UA_592_112_2_18_1_31_32_131,"Err1: lost file.\r\n")
TRACE_MSG(UA_597_112_2_18_1_31_32_132,"Err2: map[%d] : 0x%08x - 0x%08x\r\n ")
TRACE_MSG(UA_602_112_2_18_1_31_32_133,"Err3: map[%d] : 0x%08x - 0x%08x \r\n  ")
TRACE_MSG(UA_638_112_2_18_1_31_32_134,"Failed to load update backup buffer entry (0x%x)\r\n")
TRACE_MSG(UA_682_112_2_18_1_31_32_135,"enter fota process..............\r\n")
TRACE_MSG(UA_700_112_2_18_1_31_32_136,"hight %d width %d")
TRACE_MSG(UA_713_112_2_18_1_31_32_137,"BML_Init failed(0x%x).\r\n")
TRACE_MSG(UA_720_112_2_18_1_31_32_138,"BML_Open failed(0x%x).\r\n")
TRACE_MSG(UA_727_112_2_18_1_31_32_139,"BML_Unlock failed(0x%x).\r\n")
TRACE_MSG(UA_734_112_2_18_1_31_32_140,"BML_GetVolInfo information failed(0x%x).\r\n")
TRACE_MSG(UA_748_112_2_18_1_31_32_141,"Failed to get image start address and end address\r\n")
TRACE_MSG(UA_794_112_2_18_1_31_33_142,"change update flag failed(0x%x)\r\n")
TRACE_MSG(UA_801_112_2_18_1_31_33_143,"BML_Close failed(0x%x).\r\n")
TRACE_MSG(UA_833_112_2_18_1_31_33_144,"Failed to read nand flash by sector (0x%x).\r\n")
END_TRACE_MAP(MS_REF_FOTA_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_FOTA_TRC_H_


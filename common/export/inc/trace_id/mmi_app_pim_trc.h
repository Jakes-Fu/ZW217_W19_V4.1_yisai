/******************************************************************************
 ** File Name:      mmi_app_pim_trc.h                                         *
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
//trace_id:212
#ifndef _MMI_APP_PIM_TRC_H_
#define _MMI_APP_PIM_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIPIM_212_112_2_18_2_46_55_0 "enter MMIPIM MMIPIM_InitGlobleRef,time=%d"
#define MMIPIM_332_112_2_18_2_46_55_1 "MMIPIM:HandlePIMPdpMsg enter!"
#define MMIPIM_338_112_2_18_2_46_55_2 "MMIPIM:pim  pdp active success!!!!"
#define MMIPIM_348_112_2_18_2_46_55_3 "MMIPIM:pim pdp active fail!!!"
#define MMIPIM_357_112_2_18_2_46_55_4 "MMIPIM:pim  pdp deactive is over!!!!"
#define MMIPIM_367_112_2_18_2_46_55_5 "MMIPIM:pim  MMIPDP_DEACTIVE_IND msg is come!"
#define MMIPIM_381_112_2_18_2_46_55_6 "MMIPIM:HandlePIMPdpMsg exit!"
#define MMIPIM_417_112_2_18_2_46_55_7 "enter MMIPIM_ConnectNetwork()"
#define MMIPIM_426_112_2_18_2_46_55_8 "MMIPIM_GPRS_DEACTIVE_OK"
#define MMIPIM_433_112_2_18_2_46_55_9 "MMIPIM MNGPRS_SetAndActivePdpContext failure!"
#define MMIPIM_439_112_2_18_2_46_55_10 "MMIPIM  MNGPRS_SetAndActivePdpContext ok!"
#define MMIPIM_453_112_2_18_2_46_55_11 "MMIPIM_GPRS_ACTIVE_OK"
#define MMIPIM_459_112_2_18_2_46_55_12 "==PIM_GPRS_DEACTIVEING "
#define MMIPIM_489_112_2_18_2_46_55_13 "enter MMIPIM PIM_ProcessGprsActiveCnf()"
#define MMIPIM_517_112_2_18_2_46_55_14 "MMIPIM PIM_ProcessGprsActiveCnf, try to connect network again!"
#define MMIPIM_540_112_2_18_2_46_55_15 "enter MMIPIM HandlePIMPsMsg()"
#define MMIPIM_546_112_2_18_2_46_55_16 "enter MMIPIM HandlePIMPsMsg(),pdp_actived_cnf"
#define MMIPIM_558_112_2_18_2_46_55_17 "enter MMIPIM HandlePIMPsMsg(),DEACTIVATE_PDP_CONTEXT_CNF"
#define MMIPIM_567_112_2_18_2_46_55_18 "enter MMIPIM HandlePIMPsMsg(),APP_MN_DEACTIVATE_PDP_CONTEXT_IND"
#define MMIPIM_769_112_2_18_2_46_56_19 "[MMIPIM]  test flag is not consistent with pim config"
#define MMIPIM_PB_126_112_2_18_2_46_58_20 "@@MMIPB_PimAdd, enter! ptr=%d"
#define MMIPIM_PB_137_112_2_18_2_46_58_21 "@@MMIPB_PimAdd, step 1! "
#define MMIPIM_PB_153_112_2_18_2_46_58_22 "@@MMIPB_PimAdd! num_flag=%d"
#define MMIPIM_PB_232_112_2_18_2_46_58_23 "mmipim_pb: MMIPB_GetPimStatus2 ,read sucess! "
#define MMIPIM_PB_239_112_2_18_2_46_58_24 "mmipim_pb: MMIPB_GetPimStatus3 ,read fail! "
#define MMIPIM_PB_287_112_2_18_2_46_58_25 "mmipim_pb: MMIPB_UpdatePimStatus entry_id = %d,old=%d,new=%d"
#define MMIPIM_PB_394_112_2_18_2_46_58_26 "mmipim_pb: MMIPB_UpdatePimStatus entry_id = %d,old=%d,new=%d"
#define MMIPIM_PB_460_112_2_18_2_46_59_27 "@@mmipim MMIPB_GetAllPBItemsCount all_pb_item=%d "
#define MMIPIM_PB_469_112_2_18_2_46_59_28 "@@mmipim MMIPB_GetFreeItemsCount free_num=%d "
#define MMIPIM_PB_528_112_2_18_2_46_59_29 "@@MMIPIM_PB_Add free_id=%d ,time=%d"
#define MMIPIM_PB_577_112_2_18_2_46_59_30 "mmipim @@ vCardData_content=%s "
#define MMIPIM_PB_583_112_2_18_2_46_59_31 "@@MMIPIM_PB_Add 4 "
#define MMIPIM_PB_605_112_2_18_2_46_59_32 "@@MMIPIM_PB_Add£¬receive the large object,first chunk!"
#define MMIPIM_PB_609_112_2_18_2_46_59_33 "@@MMIPIM_PB_Add 5 "
#define MMIPIM_PB_622_112_2_18_2_46_59_34 "@@MMIPIM_PB_Add£¬receive the large object,midile chunk!"
#define MMIPIM_PB_628_112_2_18_2_46_59_35 "@@MMIPIM_PB_Add£¬receive the large object,midile chunk,but the size is invalid!"
#define MMIPIM_PB_644_112_2_18_2_46_59_36 "mmipim @@memory full! in handle add.@@"
#define MMIPIM_PB_650_112_2_18_2_46_59_37 "mmipim @@server process error! in handle add.@@"
#define MMIPIM_PB_677_112_2_18_2_46_59_38 "mmipim @@server process error! in handle add.@@"
#define MMIPIM_PB_688_112_2_18_2_46_59_39 "@@mmipim MMIPIM_PB_Add free_id=%d "
#define MMIPIM_PB_693_112_2_18_2_46_59_40 "mmipim @@add record ok!! in handle add.@@"
#define MMIPIM_PB_739_112_2_18_2_46_59_41 "mmipim @@add record error!! in handle add.@@"
#define MMIPIM_PB_746_112_2_18_2_46_59_42 "mmipim @@analysis vcard error!! in handle add.@@"
#define MMIPIM_PB_754_112_2_18_2_46_59_43 "@@MMIPIM_PB_Add£¬receive the large object,last chunk,but the size is invalid!"
#define MMIPIM_PB_763_112_2_18_2_46_59_44 "@@MMIPIM_PB_Add 6 ,time=%d"
#define MMIPIM_PB_776_112_2_18_2_46_59_45 "mmipim @@memory full! in handle add.@@"
#define MMIPIM_PB_782_112_2_18_2_46_59_46 "mmipim @@server process error! in handle add.@@"
#define MMIPIM_PB_809_112_2_18_2_46_59_47 "mmipim @@server process error! in handle add.@@"
#define MMIPIM_PB_817_112_2_18_2_46_59_48 "@@MMIPIM vcard_parse finished ,time=%d"
#define MMIPIM_PB_828_112_2_18_2_46_59_49 "mmipim @@mmipim add record ok!! in handle add.@@ time=%d"
#define MMIPIM_PB_871_112_2_18_2_46_59_50 "mmipim @@add record error!! in handle add.@@"
#define MMIPIM_PB_877_112_2_18_2_46_59_51 "mmipim @@analysis vcard error!! in handle add.@@"
#define MMIPIM_PB_891_112_2_18_2_47_0_52 "mmipim @@memory full! in handle add.@@"
#define MMIPIM_PB_897_112_2_18_2_47_0_53 "mmipim @@server process error! in handle add.@@"
#define MMIPIM_PB_924_112_2_18_2_47_0_54 "mmipim @@server process error! in handle add.@@"
#define MMIPIM_PB_941_112_2_18_2_47_0_55 "@@mmipim add record ok!! in handle add.@@"
#define MMIPIM_PB_984_112_2_18_2_47_0_56 "mmipim @@add record error!! in handle add.@@"
#define MMIPIM_PB_990_112_2_18_2_47_0_57 "mmipim @@analysis vcard error!! in handle add.@@"
#define MMIPIM_PB_998_112_2_18_2_47_0_58 "mmipim @@ add record finish.@@time=%d"
#define MMIPIM_PB_1050_112_2_18_2_47_0_59 "@@MMIPIM_PB_replace id_srt=%s "
#define MMIPIM_PB_1080_112_2_18_2_47_0_60 "@@MMIPIM_PB_replace id=%d,content=%s "
#define MMIPIM_PB_1107_112_2_18_2_47_0_61 "@@MMIPIM_PB_replace£¬receive the large object,first chunk!"
#define MMIPIM_PB_1111_112_2_18_2_47_0_62 "@@MMIPIM_PB_replace,large object's middle chunk!or normal object!"
#define MMIPIM_PB_1112_112_2_18_2_47_0_63 "@@MMIPIM_PB_replace, g_luri=%s,m_luri=%s "
#define MMIPIM_PB_1117_112_2_18_2_47_0_64 "@@MMIPIM_PB_replace 2 "
#define MMIPIM_PB_1122_112_2_18_2_47_0_65 "@@MMIPIM_PB_replace 3 "
#define MMIPIM_PB_1129_112_2_18_2_47_0_66 "@@MMIPIM_PB_replace£¬receive the large object,midile chunk!"
#define MMIPIM_PB_1133_112_2_18_2_47_0_67 "@@MMIPIM_PB_replace£¬receive the large object,midile chunk,but the size is invalid!"
#define MMIPIM_PB_1163_112_2_18_2_47_0_68 "mmipim@@replace record ok!! in handle replace.@@"
#define MMIPIM_PB_1168_112_2_18_2_47_0_69 "mmipim@@replace record error!! in handle replace 1 .@@"
#define MMIPIM_PB_1176_112_2_18_2_47_0_70 "@@MMIPIM_PB_replace£¬receive the large object,last chunk,but the size is invalid!"
#define MMIPIM_PB_1189_112_2_18_2_47_0_71 "@@MMIPIM_PB_replace 5 "
#define MMIPIM_PB_1207_112_2_18_2_47_0_72 "mmipim@@replace record ok!! in handle replace.@@"
#define MMIPIM_PB_1211_112_2_18_2_47_0_73 "mmipim@@replace record error!! in handle replace 2.@@"
#define MMIPIM_PB_1217_112_2_18_2_47_0_74 "mmipim@@analysis vcard error!! in handle replace. 3@@"
#define MMIPIM_PB_1241_112_2_18_2_47_0_75 "mmipim@@replace record ok!! in handle replace.@@"
#define MMIPIM_PB_1246_112_2_18_2_47_0_76 "mmipim@@replace record error!! in handle replace.@@"
#define MMIPIM_PB_1252_112_2_18_2_47_0_77 "mmipim@@analysis vcard error!! in handle replace.@@"
#define MMIPIM_PB_1304_112_2_18_2_47_0_78 "@@MMIPIM_PB_delete id_srt=%s "
#define MMIPIM_PB_1310_112_2_18_2_47_0_79 "@@MMIPIM_PB_delete id=%d "
#define MMIPIM_PB_1320_112_2_18_2_47_0_80 "mmipim@@delete record ok!! in handle delete.@@"
#define MMIPIM_PB_1326_112_2_18_2_47_0_81 "mmipim@@delete reocrd error!! in handle delete.@@"
#define MMIPIM_PB_1364_112_2_18_2_47_1_82 "@@MMIPIM_GetPBVcardItem id=%d "
#define MMIPIM_PB_1376_112_2_18_2_47_1_83 " enter MMIPIM_GetPBVcardItem  start = %d"
#define MMIPIM_PB_1387_112_2_18_2_47_1_84 " enter MMIPIM_GetPBVcardItem  head ,totalsize= %d"
#define MMIPIM_PB_1411_112_2_18_2_47_1_85 "MMIPIM_GetPBVcardItem entry_ptr->entry_name.wstr_len / 2 = %d"
#define MMIPIM_PB_1428_112_2_18_2_47_1_86 " enter MMIPIM_GetPBVcardItem,name, totalsize = %d"
#define MMIPIM_PB_1450_112_2_18_2_47_1_87 " enter MMIPIM_GetPBVcardItem,tel0, totalsize= %d"
#define MMIPIM_PB_1471_112_2_18_2_47_1_88 " enter MMIPIM_GetPBVcardItem,tel1, totalsize= %d"
#define MMIPIM_PB_1492_112_2_18_2_47_1_89 " enter MMIPIM_GetPBVcardItem,tel2, totalsize= %d"
#define MMIPIM_PB_1513_112_2_18_2_47_1_90 " enter MMIPIM_GetPBVcardItem,tel3, totalsize= %d"
#define MMIPIM_PB_1535_112_2_18_2_47_1_91 " enter MMIPIM_GetPBVcardItem,tel4, totalsize= %d"
#define MMIPIM_PB_1551_112_2_18_2_47_1_92 "mmipim entry_ptr->mail.wstr_len / 2 = %d"
#define MMIPIM_PB_1557_112_2_18_2_47_1_93 "mmipim entry_ptr->mail= %s"
#define MMIPIM_PB_1566_112_2_18_2_47_1_94 " enter MMIPIM_GetPBVcardItem,Email, totalsize = %d"
#define MMIPIM_PB_1582_112_2_18_2_47_1_95 "MMIPIM_GetPBVcardItem entry_ptr->custom_text.wstr_len = %d"
#define MMIPIM_PB_1599_112_2_18_2_47_1_96 " enter MMIPIM_GetPBVcardItem,Note,totalsize = %d"
#define MMIPIM_PB_1612_112_2_18_2_47_1_97 "MMIPIM_GetPBVcardItem entry_ptr->title.wstr_len = %d"
#define MMIPIM_PB_1628_112_2_18_2_47_1_98 " enter MMIPIM_GetPBVcardItem,title,totalsize  = %d"
#define MMIPIM_PB_1639_112_2_18_2_47_1_99 "MMIPIM_GetPBVcardItem entry_ptr->org.wstr_len = %d"
#define MMIPIM_PB_1653_112_2_18_2_47_1_100 " enter MMIPIM_GetPBVcardItem,org,totalsize  = %d"
#define MMIPIM_PB_1664_112_2_18_2_47_1_101 "entry_ptr->work_adr.wstr_len = %d"
#define MMIPIM_PB_1678_112_2_18_2_47_1_102 " enter MMIPIM_GetPBVcardItem,work_adr,totalsize  = %d"
#define MMIPIM_PB_1696_112_2_18_2_47_1_103 " enter MMIPIM_GetPBVcardItem totalsize file_size  = %d"
#define MMIPIM_PB_1717_112_2_18_2_47_1_104 " MMIPIM_GetPBVcardItem ,object is too big,memory is not enough!"
#define MMIPIM_PB_1722_112_2_18_2_47_1_105 " MMIPIM_GetPBVcardItem ,get photo data fail!"
#define MMIPIM_PB_1739_112_2_18_2_47_1_106 " enter MMIPIM_GetPBVcardItem ,end ,totalsize = %d"
#define MMIPIM_PB_1812_112_2_18_2_47_1_107 "^@@^MMIPIM hs_myGetAddedPBItems ,add_is_final,without check all items!"
#define MMIPIM_PB_1845_112_2_18_2_47_1_108 "MMIPIM hs_myGetAddedPBItems,no need updata local contact! "
#define MMIPIM_PB_1935_112_2_18_2_47_2_109 "MMIPIM hs_myGetAddedPBItems,content_len=%d"
#define MMIPIM_PB_2002_112_2_18_2_47_2_110 "@@MMIPIM send_num=%d,current_num_for_add=%d "
#define MMIPIM_PB_2005_112_2_18_2_47_2_111 "@@MMIPIM current_num_for_add=MMIPIM_MAX_PHONEBOOK_RECORDS"
#define MMIPIM_PB_2055_112_2_18_2_47_2_112 "MMIPIM &&& enter hs_myGetReplacedPBItems "
#define MMIPIM_PB_2061_112_2_18_2_47_2_113 "^@@^MMIPIM hs_myGetReplacedPBItems ,replace_is_final,without check all items!"
#define MMIPIM_PB_2125_112_2_18_2_47_2_114 "MMIPIM hs_myGetAddedPBItems,content_len=%d"
#define MMIPIM_PB_2228_112_2_18_2_47_2_115 "^@@^MMIPIM hs_myGetDeletedPBItems ,delete_is_final,without check all items!"
#define MMIPIM_PB_2287_112_2_18_2_47_2_116 "^@@^MMIPIM pMe->g_sync_result.delete_is_final=TRUE"
#define MMIPIM_PB_2318_112_2_18_2_47_2_117 "^@@^MMIPIM hs_myGetAddedVCALItems begin!"
#define MMIPIM_PB_2321_112_2_18_2_47_2_118 "^@@^MMIPIM hs_myGetAddedVCALItems,buffer == NULL!"
#define MMIPIM_PB_2334_112_2_18_2_47_2_119 "^@@^MMIPIM hs_myGetAddedVCALItems ,add_is_final,without check all items!"
#define MMIPIM_PB_2365_112_2_18_2_47_3_120 "MMIPIM hs_myGetAddedPBItems,no need updata local contact! "
#define MMIPIM_PB_2398_112_2_18_2_47_3_121 "MMIPIM hs_myGetAddedVCALItems,change vcalendar finished!data_lenth = %d "
#define MMIPIM_PB_2457_112_2_18_2_47_3_122 "@@MMIPIM send_num=%d,current_num_for_add=%d "
#define MMIPIM_PB_2460_112_2_18_2_47_3_123 "@@MMIPIM current_num_for_add=MMIPIM_MAX_VCAL_NUM"
#define MMIPIM_PB_2498_112_2_18_2_47_3_124 "MMIPIM &&& enter hs_myGetReplacedPBItems "
#define MMIPIM_PB_2504_112_2_18_2_47_3_125 "^@@^MMIPIM hs_myGetReplacedPBItems ,replace_is_final,without check all items!"
#define MMIPIM_PB_2514_112_2_18_2_47_3_126 "MMIPIM hs_myGetReplacedVCALItems,change vcalendar finished!data_lenth = %d "
#define MMIPIM_PB_2603_112_2_18_2_47_3_127 "^@@^MMIPIM hs_myGetDeletedVCALItems ,delete_is_final,without check all items!"
#define MMIPIM_PB_2662_112_2_18_2_47_3_128 "^@@^MMIPIM pMe->g_sync_result.delete_is_final=TRUE"
#define MMIPIM_PB_2723_112_2_18_2_47_3_129 "MMIPIM ----hs_myPrintLogFiles----%d.xml----"
#define MMIPIM_PB_2866_112_2_18_2_47_4_130 "MMIPIM @@handle hs_myHandledelete @@"
#define MMIPIM_PB_2901_112_2_18_2_47_4_131 "MMIPIM @@handle status @@"
#define MMIPIM_PB_2905_112_2_18_2_47_4_132 "MMIPIM @@status is %d@@"
#define MMIPIM_PB_2907_112_2_18_2_47_4_133 "MMIPIM @@status cmd is %s@@"
#define MMIPIM_PB_2921_112_2_18_2_47_4_134 "MMIPIM ** large object,send next block of the object!"
#define MMIPIM_PB_2931_112_2_18_2_47_4_135 "MMIPIM ** send next message of the package"
#define MMIPIM_PB_2950_112_2_18_2_47_4_136 "MMIPIM @@status cmdref equal to %s@@"
#define MMIPIM_PB_2978_112_2_18_2_47_4_137 "MMIPIM @@templist is pnull"
#define MMIPIM_PB_2979_112_2_18_2_47_4_138 "MMIPIM @@pbm modify the NO.%d item's data to SYNCED ok!!!! @@"
#define MMIPIM_PB_3009_112_2_18_2_47_4_139 "MMIPIM @@pbm modify the NO.%d item's data to SYNCED ok!!!! @@"
#define MMIPIM_PB_3023_112_2_18_2_47_4_140 "MMIPIM @@server process error,canot modify the NO.%d item's data to SYNCED ok!!!! @@"
#define MMIPIM_PB_3066_112_2_18_2_47_4_141 "MMIPIM @@status_auth_type is %s@@"
#define MMIPIM_PB_3077_112_2_18_2_47_4_142 "MMIPIM ** hs_myHandlestatus  syncml:auth-md5 AUTH_B64 "
#define MMIPIM_PB_3083_112_2_18_2_47_4_143 "MMIPIM ** hs_myHandlestatus  syncml:auth-md5 AUTH_MD5 authenticate failer"
#define MMIPIM_PB_3089_112_2_18_2_47_4_144 "MMIPIM ** hs_myHandlestatus syncml:auth-md5 g_pim.m_authtype ??"
#define MMIPIM_PB_3102_112_2_18_2_47_4_145 "MMIPIM ** hs_myHandlestatus  syncml:auth-basic AUTH_MD5 "
#define MMIPIM_PB_3106_112_2_18_2_47_4_146 "MMIPIM ** hs_myHandlestatus  syncml:auth-basic AUTH_B64 authenticate failer"
#define MMIPIM_PB_3112_112_2_18_2_47_4_147 "MMIPIM ** hs_myHandlestatus syncml:auth-basic g_pim.m_authtype ??"
#define MMIPIM_PB_3119_112_2_18_2_47_4_148 "MMIPIM ** hs_myHandlestatus  status_auth_type ??"
#define MMIPIM_PB_3170_112_2_18_2_47_4_149 "MMIPIM ** change the sync type to slow sync. "
#define MMIPIM_PB_3246_112_2_18_2_47_4_150 "MMIPIM hs_myLoadCacheMap"
#define MMIPIM_PB_3290_112_2_18_2_47_4_151 "MMIPIM read PimMap[%d] target = %s "
#define MMIPIM_PB_3291_112_2_18_2_47_4_152 "MMIPIM read PimMap[%d] source = %s "
#define MMIPIM_PB_3326_112_2_18_2_47_4_153 "MMIPIM  this item has been modified ,id = %d "
#define MMIPIM_PB_3346_112_2_18_2_47_4_154 "MMIPIM hs_myClearCacheMap"
#define MMIPIM_PB_3384_112_2_18_2_47_5_155 "MMIPIM hs_mySaveCacheMap"
#define MMIPIM_PB_3416_112_2_18_2_47_5_156 "MMIPIM write PimMap target = %s "
#define MMIPIM_PB_3417_112_2_18_2_47_5_157 "MMIPIM write PimMap source = %s "
#define MMIPIM_PB_3435_112_2_18_2_47_5_158 "MMIPIM hs_myGetFlagOfSendingCacheMap TRUE"
#define MMIPIM_PB_3437_112_2_18_2_47_5_159 "MMIPIM hs_myGetFlagOfSendingCacheMap FALSE"
#define MMIPIM_PB_3455_112_2_18_2_47_5_160 "MMIPIM hs_mySetFlagOfSendingCacheMap TRUE"
#define MMIPIM_PB_3457_112_2_18_2_47_5_161 "MMIPIM hs_mySetFlagOfSendingCacheMap FALSE"
#define MMIPIM_PB_3479_112_2_18_2_47_5_162 "MMIPIM &&& step=%s,id=%d "
#define MMIPIM_PB_3492_112_2_18_2_47_5_163 "error occurs! reason: wrong wsmglobal id"
#define MMIPIM_PB_3562_112_2_18_2_47_5_164 "MMIPIM  ^_^hs_Data_Sync_init,time=%d"
#define MMIPIM_PB_3622_112_2_18_2_47_5_165 "MMIPIM \"\"\"\"\"last_anchor = %s\"\"\"\"\""
#define MMIPIM_PB_3628_112_2_18_2_47_5_166 "MMIPIM \"\"\"\"\"vcal_last_anchor = %s\"\"\"\"\""
#define MMIPIM_PB_3673_112_2_18_2_47_5_167 "MMIPIM \"\"\"\"\"recent_add_count = %d\"\"\"\"\""
#define MMIPIM_PB_3674_112_2_18_2_47_5_168 "MMIPIM \"\"\"\"\"recent_replace_count = %d\"\"\"\"\""
#define MMIPIM_PB_3675_112_2_18_2_47_5_169 "MMIPIM \"\"\"\"\"recent_delete_count = %d\"\"\"\"\""
#define MMIPIM_PB_3684_112_2_18_2_47_5_170 "MMIPIM \"\"\"\"\"vcal_recent_add_count = %d\"\"\"\"\""
#define MMIPIM_PB_3685_112_2_18_2_47_5_171 "MMIPIM \"\"\"\"\"vcal_recent_replace_count = %d\"\"\"\"\""
#define MMIPIM_PB_3686_112_2_18_2_47_5_172 "MMIPIM \"\"\"\"\"vcal_recent_delete_count = %d\"\"\"\"\""
#define MMIPIM_PB_3856_112_2_18_2_47_6_173 "MMIPIM  ^_^ hs_syncml_init ok!\n"
#define MMIPIM_PB_3865_112_2_18_2_47_6_174 "MMIPIM  ^_^ syncml communication init ok!"
#define MMIPIM_WINTAB_1535_112_2_18_2_47_9_175 "MMIPIM HandlePIMSyncSettingMenuWinMsg,reset param! "
#define MMIPIM_WINTAB_1714_112_2_18_2_47_9_176 "mmipim_wintab.c:HandlePIMNetSettingAPNWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_1809_112_2_18_2_47_9_177 "mmipim_wintab.c:HandlePIMNetSettingUserWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_1912_112_2_18_2_47_10_178 "mmipim_wintab.c:HandlePIMNetSettingPasswordWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_2080_112_2_18_2_47_10_179 "mmipim_wintab.c:HandlePIMGateWayIpWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_2174_112_2_18_2_47_10_180 "mmipim_wintab.c:HandlePIMGateWayPortWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_2266_112_2_18_2_47_10_181 "mmipim_wintab.c:HandlePIMGateWayUserWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_2361_112_2_18_2_47_11_182 "mmipim_wintab.c:HandlePIMGateWayPasswordWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_2661_112_2_18_2_47_11_183 "mmipim_wintab.c:HandlePIMSyncMLSettingServerWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_2753_112_2_18_2_47_11_184 "mmipim_wintab.c:HandlePIMSyncMLSettingPortWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_2843_112_2_18_2_47_11_185 "mmipim_wintab.c:HandlePIMSyncMLSettingUserWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_2939_112_2_18_2_47_12_186 "mmipim_wintab.c:HandlePIMSyncMLSettingPasswordWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_3033_112_2_18_2_47_12_187 "mmipim_wintab.c:HandlePIMSyncMLSettingPBWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_3126_112_2_18_2_47_12_188 "mmipim_wintab.c:HandlePIMSyncMLSettingVCALWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_3550_112_2_18_2_47_13_189 "MMIDM==> enter PimCustomUASetMenuWinHandleMsg "
#define MMIPIM_WINTAB_3743_112_2_18_2_47_13_190 "MMIPIM PimGetCustomUAParaInfo string=%s"
#define MMIPIM_WINTAB_3779_112_2_18_2_47_13_191 "MMIPIM MMIPIM_GetCustomUAInfo result=%d"
#define MMIPIM_WINTAB_3798_112_2_18_2_47_13_192 "MMIPIM enter HandleDebugSettingMenuWinMsg "
#define MMIPIM_WINTAB_3830_112_2_18_2_47_13_193 "MMIPIM enter HandleDebugSettingMenuWinMsg ,UA_SETTING "
#define MMIPIM_WINTAB_4108_112_2_18_2_47_14_194 "mmipim_wintab.c:HandlePIMWapUrlRegCancelWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_4199_112_2_18_2_47_14_195 "mmipim_wintab.c:HandlePIMWapUrlNetQueryWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_4293_112_2_18_2_47_14_196 "mmipim_wintab.c:HandlePIMWapUrlPayIndexWinMsg() msg_id = %d"
#define MMIPIM_WINTAB_4509_112_2_18_2_47_15_197 "MMIPIM:s_mmipim_chng_freq_req_handler==NULL, init fail!"
#define MMIPIM_WINTAB_4515_112_2_18_2_47_15_198 "MMIPIM  start change freq!"
#define MMIPIM_WINTAB_4520_112_2_18_2_47_15_199 "MMIPIM_GetFreqHandler Fail !!"
#define MMIPIM_WINTAB_4557_112_2_18_2_47_15_200 "MMIPIM  Sync win ,get focus! server_num=%d,rec_num=%d,client_num=%d,send_num=%d"
#define MMIPIM_WINTAB_4627_112_2_18_2_47_15_201 "MMIPIM enter MSG_PDP_ACTIVE_SUCCESS ,time=%d "
#define MMIPIM_WINTAB_4635_112_2_18_2_47_15_202 "MMIPIM enter PIM_MSG_SELECT_TIMER "
#define MMIPIM_WINTAB_4657_112_2_18_2_47_15_203 "MMIPIM enter MSG_PDP_ACTIVE_FAIL "
#define MMIPIM_WINTAB_4762_112_2_18_2_47_15_204 "MMIPIM enter PIM_MSG_SYNC_FAIL "
#define MMIPIM_WINTAB_4772_112_2_18_2_47_15_205 "MMIPIM enter PIM_MSG_COMM_ERROR "
#define MMIPIM_WINTAB_4789_112_2_18_2_47_15_206 "MMIPIM enter PIM_MSG_CONN_EST "
#define MMIPIM_WINTAB_4795_112_2_18_2_47_15_207 "MMIPIM enter PIM_MSG_DATA_SENT "
#define MMIPIM_WINTAB_4803_112_2_18_2_47_15_208 "MMIPIM enter PIM_MSG_RCV_DONE "
#define MMIPIM_WINTAB_4810_112_2_18_2_47_15_209 "MMIPIM enter PIM_MSG_NEXT_STEP "
#define MMIPIM_WINTAB_4842_112_2_18_2_47_15_210 "MMIPIM  ============ malloc_num: %d ================"
#define MMIPIM_WINTAB_4848_112_2_18_2_47_15_211 "MMIPIM  Sync win , server_num=%d,rec_num=%d,client_num=%d,send_num=%d"
#define MMIPIM_WINTAB_4864_112_2_18_2_47_15_212 "MMIPIM display process download!step=%d "
#define MMIPIM_WINTAB_4901_112_2_18_2_47_16_213 "MMIPIM display process upload!step=%d  "
#define MMIPIM_WINTAB_5117_112_2_18_2_47_16_214 "MMIPIM enter MSG_APP_CANCEL "
#define MMIPIM_WINTAB_5134_112_2_18_2_47_16_215 "MMIPIM enter MSG_APP_RED "
#define MMIPIM_WINTAB_5146_112_2_18_2_47_16_216 "MMIPIM confirm cancel! "
#define MMIPIM_WINTAB_5157_112_2_18_2_47_16_217 "MMIPIM enter MSG_APP_OK "
#define MMIPIM_WINTAB_5165_112_2_18_2_47_16_218 "MMIPIM enter MSG_CLOSE_WINDOW "
#define VCALENDAR_PARSE_138_112_2_18_2_47_17_219 "MMIPIM @@ VCAL_ExchangeDelimiter enter!"
#define VCALENDAR_PARSE_144_112_2_18_2_47_17_220 "MMIPIM @@ VCAL_ExchangeDelimiter delimiter exist!"
#define VCALENDAR_PARSE_170_112_2_18_2_47_17_221 "MMIPIM @@ MMIVCAL_BuildFree start!"
#define VCALENDAR_PARSE_174_112_2_18_2_47_17_222 "MMIPIM @@ s_vcalendar_ptr free succeed!"
#define VCALENDAR_PARSE_189_112_2_18_2_47_17_223 "MMIPIM @@ MMIVCAL_BuildInit start!"
#define VCALENDAR_PARSE_234_112_2_18_2_47_17_224 "MMIPIM @@ MMIVCAL_BuildVcal begin!"
#define VCALENDAR_PARSE_244_112_2_18_2_47_18_225 "MMIPIM @@ MMIVCAL_BuildVcal verson add!"
#define VCALENDAR_PARSE_256_112_2_18_2_47_18_226 "MMIPIM @@ MMIVCAL_BuildVcal vevent add!"
#define VCALENDAR_PARSE_297_112_2_18_2_47_18_227 "MMIPIM @@ MMIVCAL_BuildVcal categories add!"
#define VCALENDAR_PARSE_351_112_2_18_2_47_18_228 "MMIPIM @@ MMIVCAL_BuildVcal summery add!"
#define VCALENDAR_PARSE_362_112_2_18_2_47_18_229 "MMIPIM @@ MMIVCAL_BuildVcal description add!vcal_len = %d"
#define VCALENDAR_PARSE_382_112_2_18_2_47_18_230 "MMIPIM @@ MMIVCAL_BuildVcal vevent dtstart add!vcal_len = %d"
#define VCALENDAR_PARSE_398_112_2_18_2_47_18_231 "MMIPIM @@ MMIVCAL_BuildVcal vevent dtend add!vcal_len = %d"
#define VCALENDAR_PARSE_461_112_2_18_2_47_18_232 "MMIPIM @@ MMIVCAL_BuildVcal aalarm add!vcal_len = %d"
#define VCALENDAR_PARSE_517_112_2_18_2_47_18_233 "MMIPIM @@ MMIVCAL_BuildVcal vevent rrule add!vcal_len = %d"
#define VCALENDAR_PARSE_533_112_2_18_2_47_18_234 "MMIPIM @@ MMIVCAL_BuildVcal aalarm add!vcal_len = %d"
#define VCALENDAR_PARSE_553_112_2_18_2_47_18_235 "MMIPIM @@ MMIVCAL_BuildVcal end!vcal_len = %d"
#define VCALENDAR_PARSE_562_112_2_18_2_47_18_236 "MMIPIM @@ MMIVCAL_BuildVcal vcal_lendth is longer than 1000 "
#define VCALENDAR_PARSE_584_112_2_18_2_47_18_237 "MMIPIM @@ MMIPIM_GetVcalItem begin!"
#define VCALENDAR_PARSE_587_112_2_18_2_47_18_238 "MMIPIM @@ MMIPIM_GetVcalItem get event_info finished!"
#define VCALENDAR_PARSE_595_112_2_18_2_47_18_239 "MMIPIM @@ MMIPIM_GetVcalItem change vcalendar buffer finished!"
#define VCALENDAR_PARSE_650_112_2_18_2_47_18_240 "MMIPIM@@ExtractClassFrVcal start!"
#define VCALENDAR_PARSE_681_112_2_18_2_47_19_241 "MMIPIM: ExtractStatusFrVcard:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_715_112_2_18_2_47_19_242 "MMIPIM@@ExtractRruleFrVcal start!"
#define VCALENDAR_PARSE_746_112_2_18_2_47_19_243 "MMIPIM: ExtractStatusFrVcard:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_780_112_2_18_2_47_19_244 "MMIPIM@@ExtractAalarmFrVcal start!"
#define VCALENDAR_PARSE_812_112_2_18_2_47_19_245 "MMIPIM: ExtractAalarmFrVcal:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_839_112_2_18_2_47_19_246 "MMIPIM: ExtractVersionFrVcard buff is NULL!"
#define VCALENDAR_PARSE_843_112_2_18_2_47_19_247 "MMIPIM@@ExtractVersionFrVcal start!"
#define VCALENDAR_PARSE_852_112_2_18_2_47_19_248 "MMIPIM: ExtractVersionFrVcard ret = %d"
#define VCALENDAR_PARSE_878_112_2_18_2_47_19_249 "MMIPIM@@ExtractPriorityFrVcal start!"
#define VCALENDAR_PARSE_909_112_2_18_2_47_19_250 "MMIPIM: ExtractPriorityFrVcard:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_943_112_2_18_2_47_19_251 "MMIPIM@@ExtractStatusFrVcal start!"
#define VCALENDAR_PARSE_974_112_2_18_2_47_19_252 "MMIPIM: ExtractStatusFrVcard:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_1010_112_2_18_2_47_19_253 "MMIPIM@@ExtractCategoriesFrVcal start!"
#define VCALENDAR_PARSE_1047_112_2_18_2_47_19_254 "MMIPIM: ExtractCategoriesFrVcard:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_1091_112_2_18_2_47_20_255 "MMIPIM@@ExtractDescFrVcal start!"
#define VCALENDAR_PARSE_1122_112_2_18_2_47_20_256 "MMIPIM: ExtractDescFrVcard:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_1156_112_2_18_2_47_20_257 "MMIPIM@@ExtractSummaryFrVcal start!"
#define VCALENDAR_PARSE_1187_112_2_18_2_47_20_258 "MMIPIM: ExtractSummaryFrVcard:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_1221_112_2_18_2_47_20_259 "MMIPIM@@ExtractDtstartFrVcal start!"
#define VCALENDAR_PARSE_1252_112_2_18_2_47_20_260 "MMIPIM: ExtractDtstartFrVcard:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_1286_112_2_18_2_47_20_261 "MMIPIM@@ExtractDtendFrVcal start!"
#define VCALENDAR_PARSE_1317_112_2_18_2_47_20_262 "MMIPIM: ExtractDtendFrVcard:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_1351_112_2_18_2_47_20_263 "MMIPIM@@ExtractDueFrVcal start!"
#define VCALENDAR_PARSE_1382_112_2_18_2_47_20_264 "MMIPIM: ExtractDtstartFrVcard:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_1416_112_2_18_2_47_20_265 "MMIPIM@@ExtractCompletedFrVcal start!"
#define VCALENDAR_PARSE_1447_112_2_18_2_47_20_266 "MMIPIM: ExtractDtstartFrVcard:hs_smlLibMalloc FAILED %d\n"
#define VCALENDAR_PARSE_1477_112_2_18_2_47_21_267 "MMIPIM@@MMIVCAL_BuffToVcalinfo start!"
#define VCALENDAR_PARSE_1489_112_2_18_2_47_21_268 "MMIPIM@@MMIVCAL_BuffToVcalinfo extract VEVENT start!"
#define VCALENDAR_PARSE_1495_112_2_18_2_47_21_269 "MMIPIM@@MMIVCAL_BuffToVcalinfo extract VEVENT start!"
#define VCALENDAR_PARSE_1536_112_2_18_2_47_21_270 "MMIPIM: MMIVCAL_VcalToSche vcal_info_ptr or event_info_ptr is null!"
#define VCALENDAR_PARSE_1581_112_2_18_2_47_21_271 "MMIPIM: MMIVCAL_VcalToSche Starttime is invalid! "
#define VCALENDAR_PARSE_1667_112_2_18_2_47_21_272 "MMIPIM: MMIVCAL_VcalToSche the number is invalid! "
#define VCALENDAR_PARSE_1785_112_2_18_2_47_21_273 "MMIPIM: MMIVCAL_VcalToSche the fre_mode is wrong! "
#define VCALENDAR_PARSE_2012_112_2_18_2_47_22_274 "MMIPIM@@MMIVCAL_BufferToSche start!"
#define VCALENDAR_PARSE_2062_112_2_18_2_47_22_275 "MMIPIM@@MMIVCAL_PimAdd! id=%d"
#define VCALENDAR_PARSE_2097_112_2_18_2_47_22_276 "MMIPIM@@MMIVCAL_PimDelete! id=%d"
#define VCALENDAR_PARSE_2134_112_2_18_2_47_22_277 "MMIPIM@@MMIVCAL_PimModify!id = %d "
#define VCALENDAR_PARSE_2168_112_2_18_2_47_22_278 "MMIPIM @@ MMIPIM_VCAL_Add enter!"
#define VCALENDAR_PARSE_2203_112_2_18_2_47_22_279 "MMIPIM @@MMIPIM_VCAL_Add vCalData_content=%s "
#define VCALENDAR_PARSE_2210_112_2_18_2_47_22_280 "mmipim @@memory full! in handle add.@@"
#define VCALENDAR_PARSE_2219_112_2_18_2_47_22_281 "MMIPIM@@MMIPIM_VCAL_Add id=%d ,time=%d"
#define VCALENDAR_PARSE_2224_112_2_18_2_47_22_282 "MMIPIM@@MMIPIM_VCAL_Add add record ok!! in handle add.@@"
#define VCALENDAR_PARSE_2230_112_2_18_2_47_22_283 "MMIPIM@@MMIPIM_VCAL_Add maplist == NULL!!"
#define VCALENDAR_PARSE_2252_112_2_18_2_47_22_284 "MMIPIM@@MMIPIM_VCAL_Add hs_globlemapItemList ==NULL!! "
#define VCALENDAR_PARSE_2269_112_2_18_2_47_22_285 "MMIPIM@@MMIPIM_VCAL_Add add record error!! in handle add.@@"
#define VCALENDAR_PARSE_2275_112_2_18_2_47_22_286 "MMIPIM@@MMIPIM_VCAL_Add analysis vcard error!! in handle add.@@"
#define VCALENDAR_PARSE_2281_112_2_18_2_47_22_287 "MMIPIM@@MMIPIM_VCAL_Add add record finish.@@time=%d"
#define VCALENDAR_PARSE_2304_112_2_18_2_47_22_288 "MMIPIM@@MMIPIM_VCAL_replace enter! "
#define VCALENDAR_PARSE_2330_112_2_18_2_47_23_289 "MMIPIM@@MMIPIM_VCAL_replace id_srt=%s "
#define VCALENDAR_PARSE_2344_112_2_18_2_47_23_290 "MMIPIM@@MMIPIM_VCAL_replace id=%d,content=%s "
#define VCALENDAR_PARSE_2355_112_2_18_2_47_23_291 "MMIPIM@@MMIPIM_VCAL_replace replace record ok!! in handle replace.@@"
#define VCALENDAR_PARSE_2360_112_2_18_2_47_23_292 "MMIPIM@@MMIPIM_VCAL_replace replace record error!! in handle replace.@@"
#define VCALENDAR_PARSE_2366_112_2_18_2_47_23_293 "MMIPIM@@MMIPIM_VCAL_replace analysis vcal error!! in handle replace.@@"
#define VCALENDAR_PARSE_2390_112_2_18_2_47_23_294 "MMIPIM@@MMIPIM_VCAL_delete enter!"
#define VCALENDAR_PARSE_2416_112_2_18_2_47_23_295 "MMIPIM@@MMIPIM_VCAL_delete id_srt=%s "
#define VCALENDAR_PARSE_2422_112_2_18_2_47_23_296 "MMIPIM@@MMIPIM_VCAL_delete id=%d "
#define VCALENDAR_PARSE_2430_112_2_18_2_47_23_297 "MMIPIM@@MMIPIM_VCAL_delete delete record ok!! in handle delete.@@"
#define VCALENDAR_PARSE_2436_112_2_18_2_47_23_298 "MMIPIM@@MMIPIM_VCAL_delete delete reocrd error!! in handle delete.@@"
#define VCALENDAR_PARSE_2460_112_2_18_2_47_23_299 "MMIPIM:MMIPIM_InitVCALStatus,readnvitem = %d"
#define VCALENDAR_PARSE_2502_112_2_18_2_47_23_300 "MMIPIM: MMIVCAL_GetPimStatus2 ,read sucess! "
#define VCALENDAR_PARSE_2509_112_2_18_2_47_23_301 "MMIPIM: MMIVCAL_GetPimStatus2 ,read fail! "
#define VCALENDAR_PARSE_2533_112_2_18_2_47_23_302 "MMIPIM: MMIVCAL_UpdatePimStatus ,error!entry_id = %d,pim_status = %d "
#define VCALENDAR_PARSE_2538_112_2_18_2_47_23_303 "MMIPIM: MMIVCAL_UpdatePimStatus start!entry_id = %d,old_status = %d,pim_status = %d "
#define VCALENDAR_PARSE_2647_112_2_18_2_47_23_304 "MMIPIM: MMIVCAL_UpdatePimStatus end! entry_id = %d,old=%d,new=%d"
#define VCALENDAR_PARSE_2694_112_2_18_2_47_23_305 "MMIPIM: MMIVCAL_GetRecentVCALItemsCount ,pim_status = %d "
#define VCALENDAR_PARSE_2717_112_2_18_2_47_23_306 "MMIPIM: MMIVCAL_GetAllVCALItemsCount all_vcal_item=%d "
#define VCALENDAR_PARSE_2733_112_2_18_2_47_23_307 "MMIPIM: MMIVCAL_GetFreeItemsCount free_num=%d "
#define VCALENDAR_PARSE_2755_112_2_18_2_47_23_308 "MMIPIM @@ MMIPIM_IsScheItemValid=%d "
#define VCARD_PARSE_224_112_2_18_2_47_24_309 "mmipim in_str=%s,len=%d,out_max_len=%d,out_len=%d"
#define VCARD_PARSE_340_112_2_18_2_47_24_310 "bt_vcard_unicode_strlen() return  %d"
#define VCARD_PARSE_353_112_2_18_2_47_24_311 "bt_vcard_unicode_strlen() return  %d"
#define VCARD_PARSE_363_112_2_18_2_47_25_312 "[vcardName] data[%d] = 0x%02x"
#define VCARD_PARSE_573_112_2_18_2_47_25_313 "IdentifyCharSet "
#define VCARD_PARSE_582_112_2_18_2_47_25_314 "IdentifyCharSet  delimiters %d"
#define VCARD_PARSE_609_112_2_18_2_47_25_315 "IdentifyCharSet char= %d,len = %d"
#define VCARD_PARSE_1312_112_2_18_2_47_27_316 "SCI_ALLOC FAILED %d\n"
#define VCARD_PARSE_1443_112_2_18_2_47_27_317 "ExtractNameFrVcard "
#define VCARD_PARSE_1461_112_2_18_2_47_27_318 "ExtractNameFrVcard 22"
#define VCARD_PARSE_1485_112_2_18_2_47_27_319 "ExtractNameFrVcard valueLen = %d "
#define VCARD_PARSE_1499_112_2_18_2_47_27_320 "utf8_len=%d"
#define VCARD_PARSE_1508_112_2_18_2_47_27_321 "ExtractNameFrVcard headPtr  is_CHN =%d"
#define VCARD_PARSE_1524_112_2_18_2_47_27_322 "que_ucs2_len=%d"
#define VCARD_PARSE_1595_112_2_18_2_47_27_323 "ExtractNameFrVcard valueLen = %d "
#define VCARD_PARSE_2765_112_2_18_2_47_30_324 " enter VcardParser "
#define VCARD_PARSE_2770_112_2_18_2_47_30_325 "File is not vCard\n"
#define VCARD_PARSE_2780_112_2_18_2_47_30_326 "tvCard Name [encoding=QUOTED-PRINTABLE]"
#define VCARD_PARSE_2782_112_2_18_2_47_30_327 "tvCard Name [encoding=8BIT]"
#define VCARD_PARSE_2784_112_2_18_2_47_30_328 "tvCard Name [encoding=BASE64]"
#define VCARD_PARSE_2788_112_2_18_2_47_30_329 "tvCard Name [charset=%s]"
#define VCARD_PARSE_2793_112_2_18_2_47_30_330 "\tvCard Name [language=%s]"
#define VCARD_PARSE_2798_112_2_18_2_47_30_331 "field 0, is_ucs2=%d,Name =%s,len=%d"
#define VCARD_PARSE_2812_112_2_18_2_47_30_332 " field1,is_ucs2=%d"
#define VCARD_PARSE_2834_112_2_18_2_47_30_333 "field2"
#define VCARD_PARSE_2839_112_2_18_2_47_30_334 "Prefix Name: %s\n"
#define VCARD_PARSE_2844_112_2_18_2_47_30_335 "Suffix Name: %s\n"
#define VCARD_PARSE_2848_112_2_18_2_47_30_336 "alpha_len = %d"
#define VCARD_PARSE_2934_112_2_18_2_47_30_337 "mmipim tel,type_flag=%x,workType=%d,homeType=%d,voiceType=%d,faxType=%d,cellType=%d"
#define VCARD_PARSE_3021_112_2_18_2_47_30_338 "MMIPIM chenxiang VcardParser tel 2 msg = %s"
#define VCARD_PARSE_3027_112_2_18_2_47_30_339 "tel  npi_ton = %d"
#define VCARD_PARSE_3033_112_2_18_2_47_30_340 "tel  false"
#define VCARD_PARSE_3069_112_2_18_2_47_30_341 "EMAIL "
#define VCARD_PARSE_3072_112_2_18_2_47_30_342 "\t[ENCODING=QUOTED-PRINTABLE]\n"
#define VCARD_PARSE_3074_112_2_18_2_47_30_343 "\t[ENCODING=BASE64]\n"
#define VCARD_PARSE_3076_112_2_18_2_47_30_344 "\n[ENCODING=8BIT]\n"
#define VCARD_PARSE_3080_112_2_18_2_47_30_345 "\t[VALUE=CONTENT-ID]\n"
#define VCARD_PARSE_3082_112_2_18_2_47_30_346 "\t[VALUE=URL]\n"
#define VCARD_PARSE_3084_112_2_18_2_47_30_347 "\t[Type=%s]\n"
#define VCARD_PARSE_3095_112_2_18_2_47_30_348 "MMIPIM chenxiang VcardParser email valueLen = %d "
#define VCARD_PARSE_3096_112_2_18_2_47_30_349 "MMIPIM chenxiang VcardParser email 1 utf8_ptr = %s"
#define VCARD_PARSE_3098_112_2_18_2_47_30_350 "MMIPIM chenxiang VcardParser email utf8_len = %d"
#define VCARD_PARSE_3099_112_2_18_2_47_30_351 "MMIPIM chenxiang VcardParser email 1 utf8_ptr = %s"
#define VCARD_PARSE_3117_112_2_18_2_47_30_352 "MMIPIM chenxiang VcardParser email 2 utf8_ptr = %s"
#define VCARD_PARSE_3169_112_2_18_2_47_30_353 "mmipim , parse note!"
#define VCARD_PARSE_3182_112_2_18_2_47_30_354 "mmipim , parse note!qp ,utf8"
#define VCARD_PARSE_3188_112_2_18_2_47_30_355 "utf8_len=%d"
#define VCARD_PARSE_3235_112_2_18_2_47_31_356 "mmipim , parse title!"
#define VCARD_PARSE_3248_112_2_18_2_47_31_357 "mmipim , parse title!qp ,utf8"
#define VCARD_PARSE_3254_112_2_18_2_47_31_358 "utf8_len=%d"
#define VCARD_PARSE_3303_112_2_18_2_47_31_359 "mmipim , parse org!"
#define VCARD_PARSE_3316_112_2_18_2_47_31_360 "mmipim , parse org!qp ,utf8"
#define VCARD_PARSE_3322_112_2_18_2_47_31_361 "utf8_len=%d"
#define VCARD_PARSE_3358_112_2_18_2_47_31_362 " mmipim vcard_org-unit"
#define VCARD_PARSE_3372_112_2_18_2_47_31_363 "mmipim , parse org!qp ,utf8"
#define VCARD_PARSE_3378_112_2_18_2_47_31_364 "utf8_len=%d"
#define VCARD_PARSE_3438_112_2_18_2_47_31_365 "ADDR "
#define VCARD_PARSE_3454_112_2_18_2_47_31_366 "mmipim , parse addr!"
#define VCARD_PARSE_3467_112_2_18_2_47_31_367 "mmipim , parse org!qp ,utf8"
#define VCARD_PARSE_3473_112_2_18_2_47_31_368 "utf8_len=%d"
#define VCARD_PARSE_3615_112_2_18_2_47_31_369 "cFirst = %d ,cSecond = %d"
#define VCARD_PARSE_3620_112_2_18_2_47_31_370 "pDst = %d , %d,%d"
#define VCARD_PARSE_3672_112_2_18_2_47_31_371 "TempChr1 = %d TempChr2 = %d"
#define VCARD_PARSE_3708_112_2_18_2_47_32_372 "pDst = %d"
#define VCARD_PARSE_3773_112_2_18_2_47_32_373 "MMIPIM wangxiaolin PhotoFile2Base64String"
#define VCARD_PARSE_3869_112_2_18_2_47_32_374 "MMIPIM wangxiaolin Base64String2PhotoFile"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_PIM_TRC)
TRACE_MSG(MMIPIM_212_112_2_18_2_46_55_0,"enter MMIPIM MMIPIM_InitGlobleRef,time=%d")
TRACE_MSG(MMIPIM_332_112_2_18_2_46_55_1,"MMIPIM:HandlePIMPdpMsg enter!")
TRACE_MSG(MMIPIM_338_112_2_18_2_46_55_2,"MMIPIM:pim  pdp active success!!!!")
TRACE_MSG(MMIPIM_348_112_2_18_2_46_55_3,"MMIPIM:pim pdp active fail!!!")
TRACE_MSG(MMIPIM_357_112_2_18_2_46_55_4,"MMIPIM:pim  pdp deactive is over!!!!")
TRACE_MSG(MMIPIM_367_112_2_18_2_46_55_5,"MMIPIM:pim  MMIPDP_DEACTIVE_IND msg is come!")
TRACE_MSG(MMIPIM_381_112_2_18_2_46_55_6,"MMIPIM:HandlePIMPdpMsg exit!")
TRACE_MSG(MMIPIM_417_112_2_18_2_46_55_7,"enter MMIPIM_ConnectNetwork()")
TRACE_MSG(MMIPIM_426_112_2_18_2_46_55_8,"MMIPIM_GPRS_DEACTIVE_OK")
TRACE_MSG(MMIPIM_433_112_2_18_2_46_55_9,"MMIPIM MNGPRS_SetAndActivePdpContext failure!")
TRACE_MSG(MMIPIM_439_112_2_18_2_46_55_10,"MMIPIM  MNGPRS_SetAndActivePdpContext ok!")
TRACE_MSG(MMIPIM_453_112_2_18_2_46_55_11,"MMIPIM_GPRS_ACTIVE_OK")
TRACE_MSG(MMIPIM_459_112_2_18_2_46_55_12,"==PIM_GPRS_DEACTIVEING ")
TRACE_MSG(MMIPIM_489_112_2_18_2_46_55_13,"enter MMIPIM PIM_ProcessGprsActiveCnf()")
TRACE_MSG(MMIPIM_517_112_2_18_2_46_55_14,"MMIPIM PIM_ProcessGprsActiveCnf, try to connect network again!")
TRACE_MSG(MMIPIM_540_112_2_18_2_46_55_15,"enter MMIPIM HandlePIMPsMsg()")
TRACE_MSG(MMIPIM_546_112_2_18_2_46_55_16,"enter MMIPIM HandlePIMPsMsg(),pdp_actived_cnf")
TRACE_MSG(MMIPIM_558_112_2_18_2_46_55_17,"enter MMIPIM HandlePIMPsMsg(),DEACTIVATE_PDP_CONTEXT_CNF")
TRACE_MSG(MMIPIM_567_112_2_18_2_46_55_18,"enter MMIPIM HandlePIMPsMsg(),APP_MN_DEACTIVATE_PDP_CONTEXT_IND")
TRACE_MSG(MMIPIM_769_112_2_18_2_46_56_19,"[MMIPIM]  test flag is not consistent with pim config")
TRACE_MSG(MMIPIM_PB_126_112_2_18_2_46_58_20,"@@MMIPB_PimAdd, enter! ptr=%d")
TRACE_MSG(MMIPIM_PB_137_112_2_18_2_46_58_21,"@@MMIPB_PimAdd, step 1! ")
TRACE_MSG(MMIPIM_PB_153_112_2_18_2_46_58_22,"@@MMIPB_PimAdd! num_flag=%d")
TRACE_MSG(MMIPIM_PB_232_112_2_18_2_46_58_23,"mmipim_pb: MMIPB_GetPimStatus2 ,read sucess! ")
TRACE_MSG(MMIPIM_PB_239_112_2_18_2_46_58_24,"mmipim_pb: MMIPB_GetPimStatus3 ,read fail! ")
TRACE_MSG(MMIPIM_PB_287_112_2_18_2_46_58_25,"mmipim_pb: MMIPB_UpdatePimStatus entry_id = %d,old=%d,new=%d")
TRACE_MSG(MMIPIM_PB_394_112_2_18_2_46_58_26,"mmipim_pb: MMIPB_UpdatePimStatus entry_id = %d,old=%d,new=%d")
TRACE_MSG(MMIPIM_PB_460_112_2_18_2_46_59_27,"@@mmipim MMIPB_GetAllPBItemsCount all_pb_item=%d ")
TRACE_MSG(MMIPIM_PB_469_112_2_18_2_46_59_28,"@@mmipim MMIPB_GetFreeItemsCount free_num=%d ")
TRACE_MSG(MMIPIM_PB_528_112_2_18_2_46_59_29,"@@MMIPIM_PB_Add free_id=%d ,time=%d")
TRACE_MSG(MMIPIM_PB_577_112_2_18_2_46_59_30,"mmipim @@ vCardData_content=%s ")
TRACE_MSG(MMIPIM_PB_583_112_2_18_2_46_59_31,"@@MMIPIM_PB_Add 4 ")
TRACE_MSG(MMIPIM_PB_605_112_2_18_2_46_59_32,"@@MMIPIM_PB_Add£¬receive the large object,first chunk!")
TRACE_MSG(MMIPIM_PB_609_112_2_18_2_46_59_33,"@@MMIPIM_PB_Add 5 ")
TRACE_MSG(MMIPIM_PB_622_112_2_18_2_46_59_34,"@@MMIPIM_PB_Add£¬receive the large object,midile chunk!")
TRACE_MSG(MMIPIM_PB_628_112_2_18_2_46_59_35,"@@MMIPIM_PB_Add£¬receive the large object,midile chunk,but the size is invalid!")
TRACE_MSG(MMIPIM_PB_644_112_2_18_2_46_59_36,"mmipim @@memory full! in handle add.@@")
TRACE_MSG(MMIPIM_PB_650_112_2_18_2_46_59_37,"mmipim @@server process error! in handle add.@@")
TRACE_MSG(MMIPIM_PB_677_112_2_18_2_46_59_38,"mmipim @@server process error! in handle add.@@")
TRACE_MSG(MMIPIM_PB_688_112_2_18_2_46_59_39,"@@mmipim MMIPIM_PB_Add free_id=%d ")
TRACE_MSG(MMIPIM_PB_693_112_2_18_2_46_59_40,"mmipim @@add record ok!! in handle add.@@")
TRACE_MSG(MMIPIM_PB_739_112_2_18_2_46_59_41,"mmipim @@add record error!! in handle add.@@")
TRACE_MSG(MMIPIM_PB_746_112_2_18_2_46_59_42,"mmipim @@analysis vcard error!! in handle add.@@")
TRACE_MSG(MMIPIM_PB_754_112_2_18_2_46_59_43,"@@MMIPIM_PB_Add£¬receive the large object,last chunk,but the size is invalid!")
TRACE_MSG(MMIPIM_PB_763_112_2_18_2_46_59_44,"@@MMIPIM_PB_Add 6 ,time=%d")
TRACE_MSG(MMIPIM_PB_776_112_2_18_2_46_59_45,"mmipim @@memory full! in handle add.@@")
TRACE_MSG(MMIPIM_PB_782_112_2_18_2_46_59_46,"mmipim @@server process error! in handle add.@@")
TRACE_MSG(MMIPIM_PB_809_112_2_18_2_46_59_47,"mmipim @@server process error! in handle add.@@")
TRACE_MSG(MMIPIM_PB_817_112_2_18_2_46_59_48,"@@MMIPIM vcard_parse finished ,time=%d")
TRACE_MSG(MMIPIM_PB_828_112_2_18_2_46_59_49,"mmipim @@mmipim add record ok!! in handle add.@@ time=%d")
TRACE_MSG(MMIPIM_PB_871_112_2_18_2_46_59_50,"mmipim @@add record error!! in handle add.@@")
TRACE_MSG(MMIPIM_PB_877_112_2_18_2_46_59_51,"mmipim @@analysis vcard error!! in handle add.@@")
TRACE_MSG(MMIPIM_PB_891_112_2_18_2_47_0_52,"mmipim @@memory full! in handle add.@@")
TRACE_MSG(MMIPIM_PB_897_112_2_18_2_47_0_53,"mmipim @@server process error! in handle add.@@")
TRACE_MSG(MMIPIM_PB_924_112_2_18_2_47_0_54,"mmipim @@server process error! in handle add.@@")
TRACE_MSG(MMIPIM_PB_941_112_2_18_2_47_0_55,"@@mmipim add record ok!! in handle add.@@")
TRACE_MSG(MMIPIM_PB_984_112_2_18_2_47_0_56,"mmipim @@add record error!! in handle add.@@")
TRACE_MSG(MMIPIM_PB_990_112_2_18_2_47_0_57,"mmipim @@analysis vcard error!! in handle add.@@")
TRACE_MSG(MMIPIM_PB_998_112_2_18_2_47_0_58,"mmipim @@ add record finish.@@time=%d")
TRACE_MSG(MMIPIM_PB_1050_112_2_18_2_47_0_59,"@@MMIPIM_PB_replace id_srt=%s ")
TRACE_MSG(MMIPIM_PB_1080_112_2_18_2_47_0_60,"@@MMIPIM_PB_replace id=%d,content=%s ")
TRACE_MSG(MMIPIM_PB_1107_112_2_18_2_47_0_61,"@@MMIPIM_PB_replace£¬receive the large object,first chunk!")
TRACE_MSG(MMIPIM_PB_1111_112_2_18_2_47_0_62,"@@MMIPIM_PB_replace,large object's middle chunk!or normal object!")
TRACE_MSG(MMIPIM_PB_1112_112_2_18_2_47_0_63,"@@MMIPIM_PB_replace, g_luri=%s,m_luri=%s ")
TRACE_MSG(MMIPIM_PB_1117_112_2_18_2_47_0_64,"@@MMIPIM_PB_replace 2 ")
TRACE_MSG(MMIPIM_PB_1122_112_2_18_2_47_0_65,"@@MMIPIM_PB_replace 3 ")
TRACE_MSG(MMIPIM_PB_1129_112_2_18_2_47_0_66,"@@MMIPIM_PB_replace£¬receive the large object,midile chunk!")
TRACE_MSG(MMIPIM_PB_1133_112_2_18_2_47_0_67,"@@MMIPIM_PB_replace£¬receive the large object,midile chunk,but the size is invalid!")
TRACE_MSG(MMIPIM_PB_1163_112_2_18_2_47_0_68,"mmipim@@replace record ok!! in handle replace.@@")
TRACE_MSG(MMIPIM_PB_1168_112_2_18_2_47_0_69,"mmipim@@replace record error!! in handle replace 1 .@@")
TRACE_MSG(MMIPIM_PB_1176_112_2_18_2_47_0_70,"@@MMIPIM_PB_replace£¬receive the large object,last chunk,but the size is invalid!")
TRACE_MSG(MMIPIM_PB_1189_112_2_18_2_47_0_71,"@@MMIPIM_PB_replace 5 ")
TRACE_MSG(MMIPIM_PB_1207_112_2_18_2_47_0_72,"mmipim@@replace record ok!! in handle replace.@@")
TRACE_MSG(MMIPIM_PB_1211_112_2_18_2_47_0_73,"mmipim@@replace record error!! in handle replace 2.@@")
TRACE_MSG(MMIPIM_PB_1217_112_2_18_2_47_0_74,"mmipim@@analysis vcard error!! in handle replace. 3@@")
TRACE_MSG(MMIPIM_PB_1241_112_2_18_2_47_0_75,"mmipim@@replace record ok!! in handle replace.@@")
TRACE_MSG(MMIPIM_PB_1246_112_2_18_2_47_0_76,"mmipim@@replace record error!! in handle replace.@@")
TRACE_MSG(MMIPIM_PB_1252_112_2_18_2_47_0_77,"mmipim@@analysis vcard error!! in handle replace.@@")
TRACE_MSG(MMIPIM_PB_1304_112_2_18_2_47_0_78,"@@MMIPIM_PB_delete id_srt=%s ")
TRACE_MSG(MMIPIM_PB_1310_112_2_18_2_47_0_79,"@@MMIPIM_PB_delete id=%d ")
TRACE_MSG(MMIPIM_PB_1320_112_2_18_2_47_0_80,"mmipim@@delete record ok!! in handle delete.@@")
TRACE_MSG(MMIPIM_PB_1326_112_2_18_2_47_0_81,"mmipim@@delete reocrd error!! in handle delete.@@")
TRACE_MSG(MMIPIM_PB_1364_112_2_18_2_47_1_82,"@@MMIPIM_GetPBVcardItem id=%d ")
TRACE_MSG(MMIPIM_PB_1376_112_2_18_2_47_1_83," enter MMIPIM_GetPBVcardItem  start = %d")
TRACE_MSG(MMIPIM_PB_1387_112_2_18_2_47_1_84," enter MMIPIM_GetPBVcardItem  head ,totalsize= %d")
TRACE_MSG(MMIPIM_PB_1411_112_2_18_2_47_1_85,"MMIPIM_GetPBVcardItem entry_ptr->entry_name.wstr_len / 2 = %d")
TRACE_MSG(MMIPIM_PB_1428_112_2_18_2_47_1_86," enter MMIPIM_GetPBVcardItem,name, totalsize = %d")
TRACE_MSG(MMIPIM_PB_1450_112_2_18_2_47_1_87," enter MMIPIM_GetPBVcardItem,tel0, totalsize= %d")
TRACE_MSG(MMIPIM_PB_1471_112_2_18_2_47_1_88," enter MMIPIM_GetPBVcardItem,tel1, totalsize= %d")
TRACE_MSG(MMIPIM_PB_1492_112_2_18_2_47_1_89," enter MMIPIM_GetPBVcardItem,tel2, totalsize= %d")
TRACE_MSG(MMIPIM_PB_1513_112_2_18_2_47_1_90," enter MMIPIM_GetPBVcardItem,tel3, totalsize= %d")
TRACE_MSG(MMIPIM_PB_1535_112_2_18_2_47_1_91," enter MMIPIM_GetPBVcardItem,tel4, totalsize= %d")
TRACE_MSG(MMIPIM_PB_1551_112_2_18_2_47_1_92,"mmipim entry_ptr->mail.wstr_len / 2 = %d")
TRACE_MSG(MMIPIM_PB_1557_112_2_18_2_47_1_93,"mmipim entry_ptr->mail= %s")
TRACE_MSG(MMIPIM_PB_1566_112_2_18_2_47_1_94," enter MMIPIM_GetPBVcardItem,Email, totalsize = %d")
TRACE_MSG(MMIPIM_PB_1582_112_2_18_2_47_1_95,"MMIPIM_GetPBVcardItem entry_ptr->custom_text.wstr_len = %d")
TRACE_MSG(MMIPIM_PB_1599_112_2_18_2_47_1_96," enter MMIPIM_GetPBVcardItem,Note,totalsize = %d")
TRACE_MSG(MMIPIM_PB_1612_112_2_18_2_47_1_97,"MMIPIM_GetPBVcardItem entry_ptr->title.wstr_len = %d")
TRACE_MSG(MMIPIM_PB_1628_112_2_18_2_47_1_98," enter MMIPIM_GetPBVcardItem,title,totalsize  = %d")
TRACE_MSG(MMIPIM_PB_1639_112_2_18_2_47_1_99,"MMIPIM_GetPBVcardItem entry_ptr->org.wstr_len = %d")
TRACE_MSG(MMIPIM_PB_1653_112_2_18_2_47_1_100," enter MMIPIM_GetPBVcardItem,org,totalsize  = %d")
TRACE_MSG(MMIPIM_PB_1664_112_2_18_2_47_1_101,"entry_ptr->work_adr.wstr_len = %d")
TRACE_MSG(MMIPIM_PB_1678_112_2_18_2_47_1_102," enter MMIPIM_GetPBVcardItem,work_adr,totalsize  = %d")
TRACE_MSG(MMIPIM_PB_1696_112_2_18_2_47_1_103," enter MMIPIM_GetPBVcardItem totalsize file_size  = %d")
TRACE_MSG(MMIPIM_PB_1717_112_2_18_2_47_1_104," MMIPIM_GetPBVcardItem ,object is too big,memory is not enough!")
TRACE_MSG(MMIPIM_PB_1722_112_2_18_2_47_1_105," MMIPIM_GetPBVcardItem ,get photo data fail!")
TRACE_MSG(MMIPIM_PB_1739_112_2_18_2_47_1_106," enter MMIPIM_GetPBVcardItem ,end ,totalsize = %d")
TRACE_MSG(MMIPIM_PB_1812_112_2_18_2_47_1_107,"^@@^MMIPIM hs_myGetAddedPBItems ,add_is_final,without check all items!")
TRACE_MSG(MMIPIM_PB_1845_112_2_18_2_47_1_108,"MMIPIM hs_myGetAddedPBItems,no need updata local contact! ")
TRACE_MSG(MMIPIM_PB_1935_112_2_18_2_47_2_109,"MMIPIM hs_myGetAddedPBItems,content_len=%d")
TRACE_MSG(MMIPIM_PB_2002_112_2_18_2_47_2_110,"@@MMIPIM send_num=%d,current_num_for_add=%d ")
TRACE_MSG(MMIPIM_PB_2005_112_2_18_2_47_2_111,"@@MMIPIM current_num_for_add=MMIPIM_MAX_PHONEBOOK_RECORDS")
TRACE_MSG(MMIPIM_PB_2055_112_2_18_2_47_2_112,"MMIPIM &&& enter hs_myGetReplacedPBItems ")
TRACE_MSG(MMIPIM_PB_2061_112_2_18_2_47_2_113,"^@@^MMIPIM hs_myGetReplacedPBItems ,replace_is_final,without check all items!")
TRACE_MSG(MMIPIM_PB_2125_112_2_18_2_47_2_114,"MMIPIM hs_myGetAddedPBItems,content_len=%d")
TRACE_MSG(MMIPIM_PB_2228_112_2_18_2_47_2_115,"^@@^MMIPIM hs_myGetDeletedPBItems ,delete_is_final,without check all items!")
TRACE_MSG(MMIPIM_PB_2287_112_2_18_2_47_2_116,"^@@^MMIPIM pMe->g_sync_result.delete_is_final=TRUE")
TRACE_MSG(MMIPIM_PB_2318_112_2_18_2_47_2_117,"^@@^MMIPIM hs_myGetAddedVCALItems begin!")
TRACE_MSG(MMIPIM_PB_2321_112_2_18_2_47_2_118,"^@@^MMIPIM hs_myGetAddedVCALItems,buffer == NULL!")
TRACE_MSG(MMIPIM_PB_2334_112_2_18_2_47_2_119,"^@@^MMIPIM hs_myGetAddedVCALItems ,add_is_final,without check all items!")
TRACE_MSG(MMIPIM_PB_2365_112_2_18_2_47_3_120,"MMIPIM hs_myGetAddedPBItems,no need updata local contact! ")
TRACE_MSG(MMIPIM_PB_2398_112_2_18_2_47_3_121,"MMIPIM hs_myGetAddedVCALItems,change vcalendar finished!data_lenth = %d ")
TRACE_MSG(MMIPIM_PB_2457_112_2_18_2_47_3_122,"@@MMIPIM send_num=%d,current_num_for_add=%d ")
TRACE_MSG(MMIPIM_PB_2460_112_2_18_2_47_3_123,"@@MMIPIM current_num_for_add=MMIPIM_MAX_VCAL_NUM")
TRACE_MSG(MMIPIM_PB_2498_112_2_18_2_47_3_124,"MMIPIM &&& enter hs_myGetReplacedPBItems ")
TRACE_MSG(MMIPIM_PB_2504_112_2_18_2_47_3_125,"^@@^MMIPIM hs_myGetReplacedPBItems ,replace_is_final,without check all items!")
TRACE_MSG(MMIPIM_PB_2514_112_2_18_2_47_3_126,"MMIPIM hs_myGetReplacedVCALItems,change vcalendar finished!data_lenth = %d ")
TRACE_MSG(MMIPIM_PB_2603_112_2_18_2_47_3_127,"^@@^MMIPIM hs_myGetDeletedVCALItems ,delete_is_final,without check all items!")
TRACE_MSG(MMIPIM_PB_2662_112_2_18_2_47_3_128,"^@@^MMIPIM pMe->g_sync_result.delete_is_final=TRUE")
TRACE_MSG(MMIPIM_PB_2723_112_2_18_2_47_3_129,"MMIPIM ----hs_myPrintLogFiles----%d.xml----")
TRACE_MSG(MMIPIM_PB_2866_112_2_18_2_47_4_130,"MMIPIM @@handle hs_myHandledelete @@")
TRACE_MSG(MMIPIM_PB_2901_112_2_18_2_47_4_131,"MMIPIM @@handle status @@")
TRACE_MSG(MMIPIM_PB_2905_112_2_18_2_47_4_132,"MMIPIM @@status is %d@@")
TRACE_MSG(MMIPIM_PB_2907_112_2_18_2_47_4_133,"MMIPIM @@status cmd is %s@@")
TRACE_MSG(MMIPIM_PB_2921_112_2_18_2_47_4_134,"MMIPIM ** large object,send next block of the object!")
TRACE_MSG(MMIPIM_PB_2931_112_2_18_2_47_4_135,"MMIPIM ** send next message of the package")
TRACE_MSG(MMIPIM_PB_2950_112_2_18_2_47_4_136,"MMIPIM @@status cmdref equal to %s@@")
TRACE_MSG(MMIPIM_PB_2978_112_2_18_2_47_4_137,"MMIPIM @@templist is pnull")
TRACE_MSG(MMIPIM_PB_2979_112_2_18_2_47_4_138,"MMIPIM @@pbm modify the NO.%d item's data to SYNCED ok!!!! @@")
TRACE_MSG(MMIPIM_PB_3009_112_2_18_2_47_4_139,"MMIPIM @@pbm modify the NO.%d item's data to SYNCED ok!!!! @@")
TRACE_MSG(MMIPIM_PB_3023_112_2_18_2_47_4_140,"MMIPIM @@server process error,canot modify the NO.%d item's data to SYNCED ok!!!! @@")
TRACE_MSG(MMIPIM_PB_3066_112_2_18_2_47_4_141,"MMIPIM @@status_auth_type is %s@@")
TRACE_MSG(MMIPIM_PB_3077_112_2_18_2_47_4_142,"MMIPIM ** hs_myHandlestatus  syncml:auth-md5 AUTH_B64 ")
TRACE_MSG(MMIPIM_PB_3083_112_2_18_2_47_4_143,"MMIPIM ** hs_myHandlestatus  syncml:auth-md5 AUTH_MD5 authenticate failer")
TRACE_MSG(MMIPIM_PB_3089_112_2_18_2_47_4_144,"MMIPIM ** hs_myHandlestatus syncml:auth-md5 g_pim.m_authtype ??")
TRACE_MSG(MMIPIM_PB_3102_112_2_18_2_47_4_145,"MMIPIM ** hs_myHandlestatus  syncml:auth-basic AUTH_MD5 ")
TRACE_MSG(MMIPIM_PB_3106_112_2_18_2_47_4_146,"MMIPIM ** hs_myHandlestatus  syncml:auth-basic AUTH_B64 authenticate failer")
TRACE_MSG(MMIPIM_PB_3112_112_2_18_2_47_4_147,"MMIPIM ** hs_myHandlestatus syncml:auth-basic g_pim.m_authtype ??")
TRACE_MSG(MMIPIM_PB_3119_112_2_18_2_47_4_148,"MMIPIM ** hs_myHandlestatus  status_auth_type ??")
TRACE_MSG(MMIPIM_PB_3170_112_2_18_2_47_4_149,"MMIPIM ** change the sync type to slow sync. ")
TRACE_MSG(MMIPIM_PB_3246_112_2_18_2_47_4_150,"MMIPIM hs_myLoadCacheMap")
TRACE_MSG(MMIPIM_PB_3290_112_2_18_2_47_4_151,"MMIPIM read PimMap[%d] target = %s ")
TRACE_MSG(MMIPIM_PB_3291_112_2_18_2_47_4_152,"MMIPIM read PimMap[%d] source = %s ")
TRACE_MSG(MMIPIM_PB_3326_112_2_18_2_47_4_153,"MMIPIM  this item has been modified ,id = %d ")
TRACE_MSG(MMIPIM_PB_3346_112_2_18_2_47_4_154,"MMIPIM hs_myClearCacheMap")
TRACE_MSG(MMIPIM_PB_3384_112_2_18_2_47_5_155,"MMIPIM hs_mySaveCacheMap")
TRACE_MSG(MMIPIM_PB_3416_112_2_18_2_47_5_156,"MMIPIM write PimMap target = %s ")
TRACE_MSG(MMIPIM_PB_3417_112_2_18_2_47_5_157,"MMIPIM write PimMap source = %s ")
TRACE_MSG(MMIPIM_PB_3435_112_2_18_2_47_5_158,"MMIPIM hs_myGetFlagOfSendingCacheMap TRUE")
TRACE_MSG(MMIPIM_PB_3437_112_2_18_2_47_5_159,"MMIPIM hs_myGetFlagOfSendingCacheMap FALSE")
TRACE_MSG(MMIPIM_PB_3455_112_2_18_2_47_5_160,"MMIPIM hs_mySetFlagOfSendingCacheMap TRUE")
TRACE_MSG(MMIPIM_PB_3457_112_2_18_2_47_5_161,"MMIPIM hs_mySetFlagOfSendingCacheMap FALSE")
TRACE_MSG(MMIPIM_PB_3479_112_2_18_2_47_5_162,"MMIPIM &&& step=%s,id=%d ")
TRACE_MSG(MMIPIM_PB_3492_112_2_18_2_47_5_163,"error occurs! reason: wrong wsmglobal id")
TRACE_MSG(MMIPIM_PB_3562_112_2_18_2_47_5_164,"MMIPIM  ^_^hs_Data_Sync_init,time=%d")
TRACE_MSG(MMIPIM_PB_3622_112_2_18_2_47_5_165,"MMIPIM \"\"\"\"\"last_anchor = %s\"\"\"\"\"")
TRACE_MSG(MMIPIM_PB_3628_112_2_18_2_47_5_166,"MMIPIM \"\"\"\"\"vcal_last_anchor = %s\"\"\"\"\"")
TRACE_MSG(MMIPIM_PB_3673_112_2_18_2_47_5_167,"MMIPIM \"\"\"\"\"recent_add_count = %d\"\"\"\"\"")
TRACE_MSG(MMIPIM_PB_3674_112_2_18_2_47_5_168,"MMIPIM \"\"\"\"\"recent_replace_count = %d\"\"\"\"\"")
TRACE_MSG(MMIPIM_PB_3675_112_2_18_2_47_5_169,"MMIPIM \"\"\"\"\"recent_delete_count = %d\"\"\"\"\"")
TRACE_MSG(MMIPIM_PB_3684_112_2_18_2_47_5_170,"MMIPIM \"\"\"\"\"vcal_recent_add_count = %d\"\"\"\"\"")
TRACE_MSG(MMIPIM_PB_3685_112_2_18_2_47_5_171,"MMIPIM \"\"\"\"\"vcal_recent_replace_count = %d\"\"\"\"\"")
TRACE_MSG(MMIPIM_PB_3686_112_2_18_2_47_5_172,"MMIPIM \"\"\"\"\"vcal_recent_delete_count = %d\"\"\"\"\"")
TRACE_MSG(MMIPIM_PB_3856_112_2_18_2_47_6_173,"MMIPIM  ^_^ hs_syncml_init ok!\n")
TRACE_MSG(MMIPIM_PB_3865_112_2_18_2_47_6_174,"MMIPIM  ^_^ syncml communication init ok!")
TRACE_MSG(MMIPIM_WINTAB_1535_112_2_18_2_47_9_175,"MMIPIM HandlePIMSyncSettingMenuWinMsg,reset param! ")
TRACE_MSG(MMIPIM_WINTAB_1714_112_2_18_2_47_9_176,"mmipim_wintab.c:HandlePIMNetSettingAPNWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_1809_112_2_18_2_47_9_177,"mmipim_wintab.c:HandlePIMNetSettingUserWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_1912_112_2_18_2_47_10_178,"mmipim_wintab.c:HandlePIMNetSettingPasswordWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_2080_112_2_18_2_47_10_179,"mmipim_wintab.c:HandlePIMGateWayIpWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_2174_112_2_18_2_47_10_180,"mmipim_wintab.c:HandlePIMGateWayPortWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_2266_112_2_18_2_47_10_181,"mmipim_wintab.c:HandlePIMGateWayUserWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_2361_112_2_18_2_47_11_182,"mmipim_wintab.c:HandlePIMGateWayPasswordWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_2661_112_2_18_2_47_11_183,"mmipim_wintab.c:HandlePIMSyncMLSettingServerWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_2753_112_2_18_2_47_11_184,"mmipim_wintab.c:HandlePIMSyncMLSettingPortWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_2843_112_2_18_2_47_11_185,"mmipim_wintab.c:HandlePIMSyncMLSettingUserWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_2939_112_2_18_2_47_12_186,"mmipim_wintab.c:HandlePIMSyncMLSettingPasswordWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_3033_112_2_18_2_47_12_187,"mmipim_wintab.c:HandlePIMSyncMLSettingPBWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_3126_112_2_18_2_47_12_188,"mmipim_wintab.c:HandlePIMSyncMLSettingVCALWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_3550_112_2_18_2_47_13_189,"MMIDM==> enter PimCustomUASetMenuWinHandleMsg ")
TRACE_MSG(MMIPIM_WINTAB_3743_112_2_18_2_47_13_190,"MMIPIM PimGetCustomUAParaInfo string=%s")
TRACE_MSG(MMIPIM_WINTAB_3779_112_2_18_2_47_13_191,"MMIPIM MMIPIM_GetCustomUAInfo result=%d")
TRACE_MSG(MMIPIM_WINTAB_3798_112_2_18_2_47_13_192,"MMIPIM enter HandleDebugSettingMenuWinMsg ")
TRACE_MSG(MMIPIM_WINTAB_3830_112_2_18_2_47_13_193,"MMIPIM enter HandleDebugSettingMenuWinMsg ,UA_SETTING ")
TRACE_MSG(MMIPIM_WINTAB_4108_112_2_18_2_47_14_194,"mmipim_wintab.c:HandlePIMWapUrlRegCancelWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_4199_112_2_18_2_47_14_195,"mmipim_wintab.c:HandlePIMWapUrlNetQueryWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_4293_112_2_18_2_47_14_196,"mmipim_wintab.c:HandlePIMWapUrlPayIndexWinMsg() msg_id = %d")
TRACE_MSG(MMIPIM_WINTAB_4509_112_2_18_2_47_15_197,"MMIPIM:s_mmipim_chng_freq_req_handler==NULL, init fail!")
TRACE_MSG(MMIPIM_WINTAB_4515_112_2_18_2_47_15_198,"MMIPIM  start change freq!")
TRACE_MSG(MMIPIM_WINTAB_4520_112_2_18_2_47_15_199,"MMIPIM_GetFreqHandler Fail !!")
TRACE_MSG(MMIPIM_WINTAB_4557_112_2_18_2_47_15_200,"MMIPIM  Sync win ,get focus! server_num=%d,rec_num=%d,client_num=%d,send_num=%d")
TRACE_MSG(MMIPIM_WINTAB_4627_112_2_18_2_47_15_201,"MMIPIM enter MSG_PDP_ACTIVE_SUCCESS ,time=%d ")
TRACE_MSG(MMIPIM_WINTAB_4635_112_2_18_2_47_15_202,"MMIPIM enter PIM_MSG_SELECT_TIMER ")
TRACE_MSG(MMIPIM_WINTAB_4657_112_2_18_2_47_15_203,"MMIPIM enter MSG_PDP_ACTIVE_FAIL ")
TRACE_MSG(MMIPIM_WINTAB_4762_112_2_18_2_47_15_204,"MMIPIM enter PIM_MSG_SYNC_FAIL ")
TRACE_MSG(MMIPIM_WINTAB_4772_112_2_18_2_47_15_205,"MMIPIM enter PIM_MSG_COMM_ERROR ")
TRACE_MSG(MMIPIM_WINTAB_4789_112_2_18_2_47_15_206,"MMIPIM enter PIM_MSG_CONN_EST ")
TRACE_MSG(MMIPIM_WINTAB_4795_112_2_18_2_47_15_207,"MMIPIM enter PIM_MSG_DATA_SENT ")
TRACE_MSG(MMIPIM_WINTAB_4803_112_2_18_2_47_15_208,"MMIPIM enter PIM_MSG_RCV_DONE ")
TRACE_MSG(MMIPIM_WINTAB_4810_112_2_18_2_47_15_209,"MMIPIM enter PIM_MSG_NEXT_STEP ")
TRACE_MSG(MMIPIM_WINTAB_4842_112_2_18_2_47_15_210,"MMIPIM  ============ malloc_num: %d ================")
TRACE_MSG(MMIPIM_WINTAB_4848_112_2_18_2_47_15_211,"MMIPIM  Sync win , server_num=%d,rec_num=%d,client_num=%d,send_num=%d")
TRACE_MSG(MMIPIM_WINTAB_4864_112_2_18_2_47_15_212,"MMIPIM display process download!step=%d ")
TRACE_MSG(MMIPIM_WINTAB_4901_112_2_18_2_47_16_213,"MMIPIM display process upload!step=%d  ")
TRACE_MSG(MMIPIM_WINTAB_5117_112_2_18_2_47_16_214,"MMIPIM enter MSG_APP_CANCEL ")
TRACE_MSG(MMIPIM_WINTAB_5134_112_2_18_2_47_16_215,"MMIPIM enter MSG_APP_RED ")
TRACE_MSG(MMIPIM_WINTAB_5146_112_2_18_2_47_16_216,"MMIPIM confirm cancel! ")
TRACE_MSG(MMIPIM_WINTAB_5157_112_2_18_2_47_16_217,"MMIPIM enter MSG_APP_OK ")
TRACE_MSG(MMIPIM_WINTAB_5165_112_2_18_2_47_16_218,"MMIPIM enter MSG_CLOSE_WINDOW ")
TRACE_MSG(VCALENDAR_PARSE_138_112_2_18_2_47_17_219,"MMIPIM @@ VCAL_ExchangeDelimiter enter!")
TRACE_MSG(VCALENDAR_PARSE_144_112_2_18_2_47_17_220,"MMIPIM @@ VCAL_ExchangeDelimiter delimiter exist!")
TRACE_MSG(VCALENDAR_PARSE_170_112_2_18_2_47_17_221,"MMIPIM @@ MMIVCAL_BuildFree start!")
TRACE_MSG(VCALENDAR_PARSE_174_112_2_18_2_47_17_222,"MMIPIM @@ s_vcalendar_ptr free succeed!")
TRACE_MSG(VCALENDAR_PARSE_189_112_2_18_2_47_17_223,"MMIPIM @@ MMIVCAL_BuildInit start!")
TRACE_MSG(VCALENDAR_PARSE_234_112_2_18_2_47_17_224,"MMIPIM @@ MMIVCAL_BuildVcal begin!")
TRACE_MSG(VCALENDAR_PARSE_244_112_2_18_2_47_18_225,"MMIPIM @@ MMIVCAL_BuildVcal verson add!")
TRACE_MSG(VCALENDAR_PARSE_256_112_2_18_2_47_18_226,"MMIPIM @@ MMIVCAL_BuildVcal vevent add!")
TRACE_MSG(VCALENDAR_PARSE_297_112_2_18_2_47_18_227,"MMIPIM @@ MMIVCAL_BuildVcal categories add!")
TRACE_MSG(VCALENDAR_PARSE_351_112_2_18_2_47_18_228,"MMIPIM @@ MMIVCAL_BuildVcal summery add!")
TRACE_MSG(VCALENDAR_PARSE_362_112_2_18_2_47_18_229,"MMIPIM @@ MMIVCAL_BuildVcal description add!vcal_len = %d")
TRACE_MSG(VCALENDAR_PARSE_382_112_2_18_2_47_18_230,"MMIPIM @@ MMIVCAL_BuildVcal vevent dtstart add!vcal_len = %d")
TRACE_MSG(VCALENDAR_PARSE_398_112_2_18_2_47_18_231,"MMIPIM @@ MMIVCAL_BuildVcal vevent dtend add!vcal_len = %d")
TRACE_MSG(VCALENDAR_PARSE_461_112_2_18_2_47_18_232,"MMIPIM @@ MMIVCAL_BuildVcal aalarm add!vcal_len = %d")
TRACE_MSG(VCALENDAR_PARSE_517_112_2_18_2_47_18_233,"MMIPIM @@ MMIVCAL_BuildVcal vevent rrule add!vcal_len = %d")
TRACE_MSG(VCALENDAR_PARSE_533_112_2_18_2_47_18_234,"MMIPIM @@ MMIVCAL_BuildVcal aalarm add!vcal_len = %d")
TRACE_MSG(VCALENDAR_PARSE_553_112_2_18_2_47_18_235,"MMIPIM @@ MMIVCAL_BuildVcal end!vcal_len = %d")
TRACE_MSG(VCALENDAR_PARSE_562_112_2_18_2_47_18_236,"MMIPIM @@ MMIVCAL_BuildVcal vcal_lendth is longer than 1000 ")
TRACE_MSG(VCALENDAR_PARSE_584_112_2_18_2_47_18_237,"MMIPIM @@ MMIPIM_GetVcalItem begin!")
TRACE_MSG(VCALENDAR_PARSE_587_112_2_18_2_47_18_238,"MMIPIM @@ MMIPIM_GetVcalItem get event_info finished!")
TRACE_MSG(VCALENDAR_PARSE_595_112_2_18_2_47_18_239,"MMIPIM @@ MMIPIM_GetVcalItem change vcalendar buffer finished!")
TRACE_MSG(VCALENDAR_PARSE_650_112_2_18_2_47_18_240,"MMIPIM@@ExtractClassFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_681_112_2_18_2_47_19_241,"MMIPIM: ExtractStatusFrVcard:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_715_112_2_18_2_47_19_242,"MMIPIM@@ExtractRruleFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_746_112_2_18_2_47_19_243,"MMIPIM: ExtractStatusFrVcard:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_780_112_2_18_2_47_19_244,"MMIPIM@@ExtractAalarmFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_812_112_2_18_2_47_19_245,"MMIPIM: ExtractAalarmFrVcal:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_839_112_2_18_2_47_19_246,"MMIPIM: ExtractVersionFrVcard buff is NULL!")
TRACE_MSG(VCALENDAR_PARSE_843_112_2_18_2_47_19_247,"MMIPIM@@ExtractVersionFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_852_112_2_18_2_47_19_248,"MMIPIM: ExtractVersionFrVcard ret = %d")
TRACE_MSG(VCALENDAR_PARSE_878_112_2_18_2_47_19_249,"MMIPIM@@ExtractPriorityFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_909_112_2_18_2_47_19_250,"MMIPIM: ExtractPriorityFrVcard:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_943_112_2_18_2_47_19_251,"MMIPIM@@ExtractStatusFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_974_112_2_18_2_47_19_252,"MMIPIM: ExtractStatusFrVcard:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_1010_112_2_18_2_47_19_253,"MMIPIM@@ExtractCategoriesFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_1047_112_2_18_2_47_19_254,"MMIPIM: ExtractCategoriesFrVcard:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_1091_112_2_18_2_47_20_255,"MMIPIM@@ExtractDescFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_1122_112_2_18_2_47_20_256,"MMIPIM: ExtractDescFrVcard:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_1156_112_2_18_2_47_20_257,"MMIPIM@@ExtractSummaryFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_1187_112_2_18_2_47_20_258,"MMIPIM: ExtractSummaryFrVcard:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_1221_112_2_18_2_47_20_259,"MMIPIM@@ExtractDtstartFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_1252_112_2_18_2_47_20_260,"MMIPIM: ExtractDtstartFrVcard:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_1286_112_2_18_2_47_20_261,"MMIPIM@@ExtractDtendFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_1317_112_2_18_2_47_20_262,"MMIPIM: ExtractDtendFrVcard:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_1351_112_2_18_2_47_20_263,"MMIPIM@@ExtractDueFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_1382_112_2_18_2_47_20_264,"MMIPIM: ExtractDtstartFrVcard:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_1416_112_2_18_2_47_20_265,"MMIPIM@@ExtractCompletedFrVcal start!")
TRACE_MSG(VCALENDAR_PARSE_1447_112_2_18_2_47_20_266,"MMIPIM: ExtractDtstartFrVcard:hs_smlLibMalloc FAILED %d\n")
TRACE_MSG(VCALENDAR_PARSE_1477_112_2_18_2_47_21_267,"MMIPIM@@MMIVCAL_BuffToVcalinfo start!")
TRACE_MSG(VCALENDAR_PARSE_1489_112_2_18_2_47_21_268,"MMIPIM@@MMIVCAL_BuffToVcalinfo extract VEVENT start!")
TRACE_MSG(VCALENDAR_PARSE_1495_112_2_18_2_47_21_269,"MMIPIM@@MMIVCAL_BuffToVcalinfo extract VEVENT start!")
TRACE_MSG(VCALENDAR_PARSE_1536_112_2_18_2_47_21_270,"MMIPIM: MMIVCAL_VcalToSche vcal_info_ptr or event_info_ptr is null!")
TRACE_MSG(VCALENDAR_PARSE_1581_112_2_18_2_47_21_271,"MMIPIM: MMIVCAL_VcalToSche Starttime is invalid! ")
TRACE_MSG(VCALENDAR_PARSE_1667_112_2_18_2_47_21_272,"MMIPIM: MMIVCAL_VcalToSche the number is invalid! ")
TRACE_MSG(VCALENDAR_PARSE_1785_112_2_18_2_47_21_273,"MMIPIM: MMIVCAL_VcalToSche the fre_mode is wrong! ")
TRACE_MSG(VCALENDAR_PARSE_2012_112_2_18_2_47_22_274,"MMIPIM@@MMIVCAL_BufferToSche start!")
TRACE_MSG(VCALENDAR_PARSE_2062_112_2_18_2_47_22_275,"MMIPIM@@MMIVCAL_PimAdd! id=%d")
TRACE_MSG(VCALENDAR_PARSE_2097_112_2_18_2_47_22_276,"MMIPIM@@MMIVCAL_PimDelete! id=%d")
TRACE_MSG(VCALENDAR_PARSE_2134_112_2_18_2_47_22_277,"MMIPIM@@MMIVCAL_PimModify!id = %d ")
TRACE_MSG(VCALENDAR_PARSE_2168_112_2_18_2_47_22_278,"MMIPIM @@ MMIPIM_VCAL_Add enter!")
TRACE_MSG(VCALENDAR_PARSE_2203_112_2_18_2_47_22_279,"MMIPIM @@MMIPIM_VCAL_Add vCalData_content=%s ")
TRACE_MSG(VCALENDAR_PARSE_2210_112_2_18_2_47_22_280,"mmipim @@memory full! in handle add.@@")
TRACE_MSG(VCALENDAR_PARSE_2219_112_2_18_2_47_22_281,"MMIPIM@@MMIPIM_VCAL_Add id=%d ,time=%d")
TRACE_MSG(VCALENDAR_PARSE_2224_112_2_18_2_47_22_282,"MMIPIM@@MMIPIM_VCAL_Add add record ok!! in handle add.@@")
TRACE_MSG(VCALENDAR_PARSE_2230_112_2_18_2_47_22_283,"MMIPIM@@MMIPIM_VCAL_Add maplist == NULL!!")
TRACE_MSG(VCALENDAR_PARSE_2252_112_2_18_2_47_22_284,"MMIPIM@@MMIPIM_VCAL_Add hs_globlemapItemList ==NULL!! ")
TRACE_MSG(VCALENDAR_PARSE_2269_112_2_18_2_47_22_285,"MMIPIM@@MMIPIM_VCAL_Add add record error!! in handle add.@@")
TRACE_MSG(VCALENDAR_PARSE_2275_112_2_18_2_47_22_286,"MMIPIM@@MMIPIM_VCAL_Add analysis vcard error!! in handle add.@@")
TRACE_MSG(VCALENDAR_PARSE_2281_112_2_18_2_47_22_287,"MMIPIM@@MMIPIM_VCAL_Add add record finish.@@time=%d")
TRACE_MSG(VCALENDAR_PARSE_2304_112_2_18_2_47_22_288,"MMIPIM@@MMIPIM_VCAL_replace enter! ")
TRACE_MSG(VCALENDAR_PARSE_2330_112_2_18_2_47_23_289,"MMIPIM@@MMIPIM_VCAL_replace id_srt=%s ")
TRACE_MSG(VCALENDAR_PARSE_2344_112_2_18_2_47_23_290,"MMIPIM@@MMIPIM_VCAL_replace id=%d,content=%s ")
TRACE_MSG(VCALENDAR_PARSE_2355_112_2_18_2_47_23_291,"MMIPIM@@MMIPIM_VCAL_replace replace record ok!! in handle replace.@@")
TRACE_MSG(VCALENDAR_PARSE_2360_112_2_18_2_47_23_292,"MMIPIM@@MMIPIM_VCAL_replace replace record error!! in handle replace.@@")
TRACE_MSG(VCALENDAR_PARSE_2366_112_2_18_2_47_23_293,"MMIPIM@@MMIPIM_VCAL_replace analysis vcal error!! in handle replace.@@")
TRACE_MSG(VCALENDAR_PARSE_2390_112_2_18_2_47_23_294,"MMIPIM@@MMIPIM_VCAL_delete enter!")
TRACE_MSG(VCALENDAR_PARSE_2416_112_2_18_2_47_23_295,"MMIPIM@@MMIPIM_VCAL_delete id_srt=%s ")
TRACE_MSG(VCALENDAR_PARSE_2422_112_2_18_2_47_23_296,"MMIPIM@@MMIPIM_VCAL_delete id=%d ")
TRACE_MSG(VCALENDAR_PARSE_2430_112_2_18_2_47_23_297,"MMIPIM@@MMIPIM_VCAL_delete delete record ok!! in handle delete.@@")
TRACE_MSG(VCALENDAR_PARSE_2436_112_2_18_2_47_23_298,"MMIPIM@@MMIPIM_VCAL_delete delete reocrd error!! in handle delete.@@")
TRACE_MSG(VCALENDAR_PARSE_2460_112_2_18_2_47_23_299,"MMIPIM:MMIPIM_InitVCALStatus,readnvitem = %d")
TRACE_MSG(VCALENDAR_PARSE_2502_112_2_18_2_47_23_300,"MMIPIM: MMIVCAL_GetPimStatus2 ,read sucess! ")
TRACE_MSG(VCALENDAR_PARSE_2509_112_2_18_2_47_23_301,"MMIPIM: MMIVCAL_GetPimStatus2 ,read fail! ")
TRACE_MSG(VCALENDAR_PARSE_2533_112_2_18_2_47_23_302,"MMIPIM: MMIVCAL_UpdatePimStatus ,error!entry_id = %d,pim_status = %d ")
TRACE_MSG(VCALENDAR_PARSE_2538_112_2_18_2_47_23_303,"MMIPIM: MMIVCAL_UpdatePimStatus start!entry_id = %d,old_status = %d,pim_status = %d ")
TRACE_MSG(VCALENDAR_PARSE_2647_112_2_18_2_47_23_304,"MMIPIM: MMIVCAL_UpdatePimStatus end! entry_id = %d,old=%d,new=%d")
TRACE_MSG(VCALENDAR_PARSE_2694_112_2_18_2_47_23_305,"MMIPIM: MMIVCAL_GetRecentVCALItemsCount ,pim_status = %d ")
TRACE_MSG(VCALENDAR_PARSE_2717_112_2_18_2_47_23_306,"MMIPIM: MMIVCAL_GetAllVCALItemsCount all_vcal_item=%d ")
TRACE_MSG(VCALENDAR_PARSE_2733_112_2_18_2_47_23_307,"MMIPIM: MMIVCAL_GetFreeItemsCount free_num=%d ")
TRACE_MSG(VCALENDAR_PARSE_2755_112_2_18_2_47_23_308,"MMIPIM @@ MMIPIM_IsScheItemValid=%d ")
TRACE_MSG(VCARD_PARSE_224_112_2_18_2_47_24_309,"mmipim in_str=%s,len=%d,out_max_len=%d,out_len=%d")
TRACE_MSG(VCARD_PARSE_340_112_2_18_2_47_24_310,"bt_vcard_unicode_strlen() return  %d")
TRACE_MSG(VCARD_PARSE_353_112_2_18_2_47_24_311,"bt_vcard_unicode_strlen() return  %d")
TRACE_MSG(VCARD_PARSE_363_112_2_18_2_47_25_312,"[vcardName] data[%d] = 0x%02x")
TRACE_MSG(VCARD_PARSE_573_112_2_18_2_47_25_313,"IdentifyCharSet ")
TRACE_MSG(VCARD_PARSE_582_112_2_18_2_47_25_314,"IdentifyCharSet  delimiters %d")
TRACE_MSG(VCARD_PARSE_609_112_2_18_2_47_25_315,"IdentifyCharSet char= %d,len = %d")
TRACE_MSG(VCARD_PARSE_1312_112_2_18_2_47_27_316,"SCI_ALLOC FAILED %d\n")
TRACE_MSG(VCARD_PARSE_1443_112_2_18_2_47_27_317,"ExtractNameFrVcard ")
TRACE_MSG(VCARD_PARSE_1461_112_2_18_2_47_27_318,"ExtractNameFrVcard 22")
TRACE_MSG(VCARD_PARSE_1485_112_2_18_2_47_27_319,"ExtractNameFrVcard valueLen = %d ")
TRACE_MSG(VCARD_PARSE_1499_112_2_18_2_47_27_320,"utf8_len=%d")
TRACE_MSG(VCARD_PARSE_1508_112_2_18_2_47_27_321,"ExtractNameFrVcard headPtr  is_CHN =%d")
TRACE_MSG(VCARD_PARSE_1524_112_2_18_2_47_27_322,"que_ucs2_len=%d")
TRACE_MSG(VCARD_PARSE_1595_112_2_18_2_47_27_323,"ExtractNameFrVcard valueLen = %d ")
TRACE_MSG(VCARD_PARSE_2765_112_2_18_2_47_30_324," enter VcardParser ")
TRACE_MSG(VCARD_PARSE_2770_112_2_18_2_47_30_325,"File is not vCard\n")
TRACE_MSG(VCARD_PARSE_2780_112_2_18_2_47_30_326,"tvCard Name [encoding=QUOTED-PRINTABLE]")
TRACE_MSG(VCARD_PARSE_2782_112_2_18_2_47_30_327,"tvCard Name [encoding=8BIT]")
TRACE_MSG(VCARD_PARSE_2784_112_2_18_2_47_30_328,"tvCard Name [encoding=BASE64]")
TRACE_MSG(VCARD_PARSE_2788_112_2_18_2_47_30_329,"tvCard Name [charset=%s]")
TRACE_MSG(VCARD_PARSE_2793_112_2_18_2_47_30_330,"\tvCard Name [language=%s]")
TRACE_MSG(VCARD_PARSE_2798_112_2_18_2_47_30_331,"field 0, is_ucs2=%d,Name =%s,len=%d")
TRACE_MSG(VCARD_PARSE_2812_112_2_18_2_47_30_332," field1,is_ucs2=%d")
TRACE_MSG(VCARD_PARSE_2834_112_2_18_2_47_30_333,"field2")
TRACE_MSG(VCARD_PARSE_2839_112_2_18_2_47_30_334,"Prefix Name: %s\n")
TRACE_MSG(VCARD_PARSE_2844_112_2_18_2_47_30_335,"Suffix Name: %s\n")
TRACE_MSG(VCARD_PARSE_2848_112_2_18_2_47_30_336,"alpha_len = %d")
TRACE_MSG(VCARD_PARSE_2934_112_2_18_2_47_30_337,"mmipim tel,type_flag=%x,workType=%d,homeType=%d,voiceType=%d,faxType=%d,cellType=%d")
TRACE_MSG(VCARD_PARSE_3021_112_2_18_2_47_30_338,"MMIPIM chenxiang VcardParser tel 2 msg = %s")
TRACE_MSG(VCARD_PARSE_3027_112_2_18_2_47_30_339,"tel  npi_ton = %d")
TRACE_MSG(VCARD_PARSE_3033_112_2_18_2_47_30_340,"tel  false")
TRACE_MSG(VCARD_PARSE_3069_112_2_18_2_47_30_341,"EMAIL ")
TRACE_MSG(VCARD_PARSE_3072_112_2_18_2_47_30_342,"\t[ENCODING=QUOTED-PRINTABLE]\n")
TRACE_MSG(VCARD_PARSE_3074_112_2_18_2_47_30_343,"\t[ENCODING=BASE64]\n")
TRACE_MSG(VCARD_PARSE_3076_112_2_18_2_47_30_344,"\n[ENCODING=8BIT]\n")
TRACE_MSG(VCARD_PARSE_3080_112_2_18_2_47_30_345,"\t[VALUE=CONTENT-ID]\n")
TRACE_MSG(VCARD_PARSE_3082_112_2_18_2_47_30_346,"\t[VALUE=URL]\n")
TRACE_MSG(VCARD_PARSE_3084_112_2_18_2_47_30_347,"\t[Type=%s]\n")
TRACE_MSG(VCARD_PARSE_3095_112_2_18_2_47_30_348,"MMIPIM chenxiang VcardParser email valueLen = %d ")
TRACE_MSG(VCARD_PARSE_3096_112_2_18_2_47_30_349,"MMIPIM chenxiang VcardParser email 1 utf8_ptr = %s")
TRACE_MSG(VCARD_PARSE_3098_112_2_18_2_47_30_350,"MMIPIM chenxiang VcardParser email utf8_len = %d")
TRACE_MSG(VCARD_PARSE_3099_112_2_18_2_47_30_351,"MMIPIM chenxiang VcardParser email 1 utf8_ptr = %s")
TRACE_MSG(VCARD_PARSE_3117_112_2_18_2_47_30_352,"MMIPIM chenxiang VcardParser email 2 utf8_ptr = %s")
TRACE_MSG(VCARD_PARSE_3169_112_2_18_2_47_30_353,"mmipim , parse note!")
TRACE_MSG(VCARD_PARSE_3182_112_2_18_2_47_30_354,"mmipim , parse note!qp ,utf8")
TRACE_MSG(VCARD_PARSE_3188_112_2_18_2_47_30_355,"utf8_len=%d")
TRACE_MSG(VCARD_PARSE_3235_112_2_18_2_47_31_356,"mmipim , parse title!")
TRACE_MSG(VCARD_PARSE_3248_112_2_18_2_47_31_357,"mmipim , parse title!qp ,utf8")
TRACE_MSG(VCARD_PARSE_3254_112_2_18_2_47_31_358,"utf8_len=%d")
TRACE_MSG(VCARD_PARSE_3303_112_2_18_2_47_31_359,"mmipim , parse org!")
TRACE_MSG(VCARD_PARSE_3316_112_2_18_2_47_31_360,"mmipim , parse org!qp ,utf8")
TRACE_MSG(VCARD_PARSE_3322_112_2_18_2_47_31_361,"utf8_len=%d")
TRACE_MSG(VCARD_PARSE_3358_112_2_18_2_47_31_362," mmipim vcard_org-unit")
TRACE_MSG(VCARD_PARSE_3372_112_2_18_2_47_31_363,"mmipim , parse org!qp ,utf8")
TRACE_MSG(VCARD_PARSE_3378_112_2_18_2_47_31_364,"utf8_len=%d")
TRACE_MSG(VCARD_PARSE_3438_112_2_18_2_47_31_365,"ADDR ")
TRACE_MSG(VCARD_PARSE_3454_112_2_18_2_47_31_366,"mmipim , parse addr!")
TRACE_MSG(VCARD_PARSE_3467_112_2_18_2_47_31_367,"mmipim , parse org!qp ,utf8")
TRACE_MSG(VCARD_PARSE_3473_112_2_18_2_47_31_368,"utf8_len=%d")
TRACE_MSG(VCARD_PARSE_3615_112_2_18_2_47_31_369,"cFirst = %d ,cSecond = %d")
TRACE_MSG(VCARD_PARSE_3620_112_2_18_2_47_31_370,"pDst = %d , %d,%d")
TRACE_MSG(VCARD_PARSE_3672_112_2_18_2_47_31_371,"TempChr1 = %d TempChr2 = %d")
TRACE_MSG(VCARD_PARSE_3708_112_2_18_2_47_32_372,"pDst = %d")
TRACE_MSG(VCARD_PARSE_3773_112_2_18_2_47_32_373,"MMIPIM wangxiaolin PhotoFile2Base64String")
TRACE_MSG(VCARD_PARSE_3869_112_2_18_2_47_32_374,"MMIPIM wangxiaolin Base64String2PhotoFile")
END_TRACE_MAP(MMI_APP_PIM_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_PIM_TRC_H_


/******************************************************************************
 ** File Name:      cmmb_trc.h                                         *
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
//trace_id:6
#ifndef _CMMB_TRC_H_
#define _CMMB_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define CMMBDMX_61_112_2_17_23_8_39_0 "%02x%02x%02x%02x\n"
#define CMMBDMX_MISC_195_112_2_17_23_8_44_1 "CMMB_WARN%s slot buff too small, drop %s[%x]\n"
#define DEMOD_103_112_2_17_23_8_46_2 "DEMOD type %d"
#define DEMUX_276_112_2_17_23_8_49_3 "\"\"\"\"#!!!!!!DemuxGetData error!"
#define DEMUX_296_112_2_17_23_8_49_4 "\"\"\"\"#!!!!!!DemuxSendData error!"
#define DTL_602_112_2_17_23_8_56_5 "DTL_DirectRWData RespQ_ptr->resp_id=%x error"
#define DTL_694_112_2_17_23_8_56_6 "DTLSVC_Task()RespQ_ptr[%d].resp_id=%x,parameter0=%x"
#define DTL_805_112_2_17_23_8_56_7 "DTL_SendData() block_num=%d,tail_size=%d, addr = %x"
#define DTL_815_112_2_17_23_8_56_8 "DTL_SendData() block_num=%d error!!!"
#define DTL_841_112_2_17_23_8_56_9 "DTL_SendData() tail_size=%d error!!!"
#define DTL_928_112_2_17_23_8_57_10 "DTL_GetData() block_num=%d,tail_size=%d,src_addr=%x"
#define DTL_937_112_2_17_23_8_57_11 "DTL_GetData() block_num=%d error!!!"
#define DTL_960_112_2_17_23_8_57_12 "DTL_GetData() tail_size=%d error!!!"
#define DTL_1034_112_2_17_23_8_57_13 "write cmd error"
#define DTL_1049_112_2_17_23_8_57_14 "DTL_SendCMD()next_cmd_in=%d,cmd_to_send->cmd_id=%x,p0=%x,p1=%x (%d)"
#define DTL_1060_112_2_17_23_8_57_15 "DTL_SendCMD() write_data error!!"
#define DTL_1131_112_2_17_23_8_57_16 "DTL_SendCMD_NOINT()next_cmd_in=%d,cmd_out=%d,cmd_id=%d"
#define DTL_1142_112_2_17_23_8_57_17 "DTL_SendCMD() write_data error!!"
#define DTL_1225_112_2_17_23_8_57_18 "write cmd error"
#define DTL_1253_112_2_17_23_8_57_19 "DTL_SendCMD_And_MsgParam() data_size = %d error!!!"
#define DTL_1263_112_2_17_23_8_57_20 "DTL_SendCMD_And_MsgParam() tail_size = %d error!!!"
#define DTL_1278_112_2_17_23_8_57_21 "DTL_SendCMD_And_MsgParam() write cmd error!!!"
#define DTL_1344_112_2_17_23_8_57_22 "DTL_SendCMD_NOINT()next_cmd_in=%d,cmd_out=%d,cmd_id=%d"
#define DTL_1355_112_2_17_23_8_57_23 "DTL_SendCMD() write_data error!!"
#define DTL_1504_112_2_17_23_8_58_24 "DTL_GetCmd() tmp_ptr->resp_id=%x"
#define DTL_LOCAL_361_112_2_17_23_9_5_25 "Demod_CallBack ack_id=0x%x"
#define DTL_LOCAL_375_112_2_17_23_9_5_26 "Demod_dmx_callback resp_ptr->resp_id=0x%x"
#define DTL_LOCAL_840_112_2_17_23_9_6_27 "DTL_RegisterMsg,event=%d,i=%d,task_id=0x%x"
#define DTL_LOCAL_897_112_2_17_23_9_6_28 "DTL_UnregisterMsg,event=%d,i=%d,task_id=0x%x"
#define DTL_LOCAL_986_112_2_17_23_9_7_29 "enable channel %d"
#define DTL_LOCAL_992_112_2_17_23_9_7_30 "disable channel %d"
#define DTL_LOCAL_1003_112_2_17_23_9_7_31 "SET DESCRAM KEY"
#define DTL_LOCAL_1008_112_2_17_23_9_7_32 "SET ISMA"
#define LCD_TV_1804_112_2_17_23_9_12_33 "LCDTV_Init,in"
#define LCD_TV_1863_112_2_17_23_9_12_34 "LCDTV_InvalidateRect,in,(left,top)=(%d,%d),(right,bottom)=(%d,%d),lcd(w,h)=(%d,%d)"
#define LCD_TV_1954_112_2_17_23_9_12_35 "LCD_Invalidate_6600V_MP4,lcd_spec,(w,h)=(%d,%d)"
#define MTV_BOOT_775_112_2_17_23_9_16_36 "TVBOOT_TVOpenWqvga() not read BB_ARM_STATUS_DOWNLOAD_ARM_CODE_FINISHED command"
#define MBBMS_GBA_284_112_2_17_23_9_19_37 "MBBMS GBA READY: GBA HAS BE DONE \n"
#define MBBMS_GBA_292_112_2_17_23_9_19_38 "MBBMS GBA READY: RAND LEN: %d\n"
#define MBBMS_GBA_293_112_2_17_23_9_19_39 "MBBMS GBA READY: B-TID LEN: %d\n"
#define MBBMS_GBA_300_112_2_17_23_9_19_40 "MBBMS GBA READY: KS LEN: %d\n"
#define MBBMS_GBA_313_112_2_17_23_9_19_41 "MBBMS GBA READY: MUACMD GET FAIL: UNKOWN\n"
#define MBBMS_GBA_334_112_2_17_23_9_19_42 "MBBMS_GBA_Init: Entering\n"
#define MBBMS_GBA_350_112_2_17_23_9_19_43 "Create gba authorize thread failed!!"
#define MBBMS_GBA_423_112_2_17_23_9_19_44 "MBBMS_GBA_Init:UserAgent =%s \n"
#define MBBMS_GBA_426_112_2_17_23_9_19_45 "MBBMS_GBA_Init: Leaving ret=%d \n"
#define MBBMS_GBA_442_112_2_17_23_9_19_46 "MBBMS_GBA_Exit: Entering\n"
#define MBBMS_GBA_446_112_2_17_23_9_19_47 "MBBMS_GBA_Exit: ABORT, GBA NOT INITED YET \n"
#define MBBMS_GBA_468_112_2_17_23_9_19_48 "MBBMS_GBA_EXIT: Waiting Thread End \n"
#define MBBMS_GBA_487_112_2_17_23_9_19_49 "MBBMS_GBA_Exit Successful, I AM LEAVING...\n"
#define MBBMS_GBA_501_112_2_17_23_9_19_50 "MBBMS_Set_WAP: Entering\n"
#define MBBMS_GBA_511_112_2_17_23_9_19_51 "MBBMS_Set_WAP: wap:%s port:%d Leaving\n"
#define MBBMS_GBA_541_112_2_17_23_9_19_52 "MBBMS_Set_CMMBSN: Entering\n"
#define MBBMS_GBA_550_112_2_17_23_9_19_53 "MBBMS_Set_CMMBSN: CMMBSN:%s Len:%d Leaving\n"
#define MBBMS_GBA_561_112_2_17_23_9_19_54 "MBBMS_Set_User_Agent :%s \n"
#define MBBMS_GBA_566_112_2_17_23_9_19_55 "MBBMS_Get_User_Agent len:%d string:%s \n"
#define MBBMS_GBA_593_112_2_17_23_9_19_56 "MUACMD_MBBMS_GetCMMBSN return FALSE \n"
#define MBBMS_GBA_608_112_2_17_23_9_19_57 "MBBMS_Set_NAF: Entering\n"
#define MBBMS_GBA_619_112_2_17_23_9_19_58 "MBBMS_Set_NAF: naf:%s port:%d Leaving\n"
#define MBBMS_GBA_635_112_2_17_23_9_19_59 "MBBMS_Set_NAF_Domain_Name: Entering\n"
#define MBBMS_GBA_645_112_2_17_23_9_20_60 "MBBMS_Set_NAF_Domain_Name: %s Leaving\n"
#define MBBMS_GBA_691_112_2_17_23_9_20_61 "MBBMS_GBA_Get_State: Entering\n"
#define MBBMS_GBA_696_112_2_17_23_9_20_62 "MBBMS_GBA_Get_State: Leaving, State:GOOD \n"
#define MBBMS_GBA_718_112_2_17_23_9_20_63 "MBBMS_GBA_Get_Data: Entering\n"
#define MBBMS_GBA_726_112_2_17_23_9_20_64 "MBBMS_GBA_Get_Data: Leaving GOOD! \n"
#define MBBMS_GBA_740_112_2_17_23_9_20_65 "MBBMS_MRK_Get_Data: Entering\n"
#define MBBMS_GBA_747_112_2_17_23_9_20_66 "MBBMS_MRK_Get_Data: Failed, MRK not operated \n"
#define MBBMS_GBA_754_112_2_17_23_9_20_67 "MBBMS_MRK_Get_Data: Leaving GOOD! \n"
#define MBBMS_GBA_769_112_2_17_23_9_20_68 "MBBMS_GBAMRK_Done Checking .... \n"
#define MBBMS_GBA_772_112_2_17_23_9_20_69 "MBBMS_GBAMRK_Done module_init not DONE! Over \n"
#define MBBMS_GBA_777_112_2_17_23_9_20_70 "MBBMS_GBAMRK_Done GBA not DONE! Over \n"
#define MBBMS_GBA_782_112_2_17_23_9_20_71 "MBBMS_GBAMRK_Done mrk not DONE! Over \n"
#define MBBMS_GBA_795_112_2_17_23_9_20_72 "MBBMS_SIM_Support_CAS: Entering\n"
#define MBBMS_GBA_804_112_2_17_23_9_20_73 "Enter SendCASChecking to Thread"
#define MBBMS_GBA_821_112_2_17_23_9_20_74 "MBBMS_SIM_Support_CAS: Leaving  \n"
#define MBBMS_GBA_840_112_2_17_23_9_20_75 "MBBMS_GBA_Operation: Entering\n"
#define MBBMS_GBA_845_112_2_17_23_9_20_76 "MBBMS_MRK_Operation: FAILED. not inited, or no naf or wap\n"
#define MBBMS_GBA_850_112_2_17_23_9_20_77 "MBBMS_GBA_Operation: NOT support CAS\n"
#define MBBMS_GBA_855_112_2_17_23_9_20_78 "MBBMS_GBA_Operation: MBBMS_GBA is BUSY!\n"
#define MBBMS_GBA_860_112_2_17_23_9_20_79 "Enter SendGBAWorking to Thread"
#define MBBMS_GBA_870_112_2_17_23_9_20_80 "MBBMS_GBA_Operation: Leaving\n"
#define MBBMS_GBA_886_112_2_17_23_9_20_81 "MBBMS_GBA_Stop_Operation: Entering\n"
#define MBBMS_GBA_893_112_2_17_23_9_20_82 "Enter SendGBAWorking to Thread"
#define MBBMS_GBA_902_112_2_17_23_9_20_83 "MBBMS_GBA_Stop_Operation: Leaving\n"
#define MBBMS_GBA_919_112_2_17_23_9_20_84 "MBBMS_MRK_Operation: Entering\n"
#define MBBMS_GBA_925_112_2_17_23_9_20_85 "MBBMS_MRK_Operation: FAILED. not inited, or no naf or wap\n"
#define MBBMS_GBA_930_112_2_17_23_9_20_86 "MBBMS_MRK_Operation: NOT support CAS\n"
#define MBBMS_GBA_936_112_2_17_23_9_20_87 "MBBMS_MRK_Operation: MBBMS_GBA is BUSY!\n"
#define MBBMS_GBA_940_112_2_17_23_9_20_88 "Enter SendMRKWorking to Thread"
#define MBBMS_GBA_952_112_2_17_23_9_20_89 "MBBMS_MRK_Operation: Leaving\n"
#define MBBMS_GBA_969_112_2_17_23_9_20_90 "MBBMS_MRK_Stop_Operation: Entering\n"
#define MBBMS_GBA_976_112_2_17_23_9_20_91 "Enter SendMRKStop to Thread"
#define MBBMS_GBA_987_112_2_17_23_9_20_92 "MBBMS_MRK_Stop_Operation: Leaving\n"
#define MBBMS_GBA_997_112_2_17_23_9_20_93 "MBBMS GBA sendMMI signal. sig_code:%d Result:%d \n"
#define MBBMS_GBA_1016_112_2_17_23_9_20_94 "MBBMS GBA save MMI signal.  sig_code %d Result:%d \n"
#define MBBMS_GBA_1046_112_2_17_23_9_20_95 "MBBMS Print:%s \n"
#define MBBMS_GBA_1050_112_2_17_23_9_20_96 "MBBMS Print:%s \n"
#define MBBMS_GBA_1067_112_2_17_23_9_20_97 "MBBMS_XML: item:%s value:%s \n"
#define MBBMS_GBA_1083_112_2_17_23_9_20_98 "MBBMS_HTTP RESQN: item:%s value:%s \n"
#define MBBMS_GBA_1096_112_2_17_23_9_20_99 "MBBMS IMPI:%s \n"
#define MBBMS_GBA_1101_112_2_17_23_9_20_100 "MBBMS IMPI description is checking.... \n"
#define MBBMS_GBA_1105_112_2_17_23_9_20_101 "MBBMS IMPI description received:%s \n"
#define MBBMS_GBA_1111_112_2_17_23_9_20_102 "MBBMS IMPI description NOT received \n"
#define MBBMS_GBA_1114_112_2_17_23_9_20_103 "MBBMS IMPI description received len:%d \n"
#define MBBMS_GBA_1146_112_2_17_23_9_21_104 "MBBMS IMPI description NOT received \n"
#define MBBMS_GBA_1148_112_2_17_23_9_21_105 "MBBMS IMPI description received:%s \n"
#define MBBMS_GBA_1177_112_2_17_23_9_21_106 "MBBMS BSF:%s \n"
#define MBBMS_GBA_1202_112_2_17_23_9_21_107 "MBBMS nonce = %s \n"
#define MBBMS_GBA_1205_112_2_17_23_9_21_108 "MBBMS_REG_REQ_BASE64_decode begin....\n"
#define MBBMS_GBA_1208_112_2_17_23_9_21_109 " MBBMS_GBA RAND_AUTN_Other Len:%d \n"
#define MBBMS_GBA_1212_112_2_17_23_9_21_110 "MBBMS_BASE64 RAND 1-8: 0x%02x %02x %02x %02x %02x %02x %02x %02xn"
#define MBBMS_GBA_1214_112_2_17_23_9_21_111 "MBBMS_BASE64 RAND 9-16: 0x%02x %02x %02x %02x %02x %02x %02x %02xn"
#define MBBMS_GBA_1225_112_2_17_23_9_21_112 "MBBMS_BASE64 AUTN len:%d\n"
#define MBBMS_GBA_1227_112_2_17_23_9_21_113 "MBBMS_BASE64 AUTN 1-8: 0x%02x %02x %02x %02x %02x %02x %02x %02xn"
#define MBBMS_GBA_1229_112_2_17_23_9_21_114 "MBBMS_BASE64 AUTN 9-16: 0x%02x %02x %02x %02x %02x %02x %02x %02xn"
#define MBBMS_GBA_1234_112_2_17_23_9_21_115 "MBBMS_GBA_RAND NO AUTN DATA\n"
#define MBBMS_GBA_1249_112_2_17_23_9_21_116 "MBBMS 2G KS_INPUT: 0x%x %x %x %x n"
#define MBBMS_GBA_1255_112_2_17_23_9_21_117 "MBBMS_GBA_RAND NO AUTN DATA\n"
#define MBBMS_GBA_1388_112_2_17_23_9_21_118 "MBBMS btid len:%d str[%s] \n"
#define MBBMS_GBA_1399_112_2_17_23_9_21_119 "MBBMS GBA Lifetime:%d-%d-%d %d:%d:%d n"
#define MBBMS_GBA_1469_112_2_17_23_9_21_120 "MBBMS_Free_Init_Param begin...\n"
#define MBBMS_GBA_1482_112_2_17_23_9_21_121 "MBBMS_Free_Init_Param End\n"
#define MBBMS_GBA_1488_112_2_17_23_9_21_122 "MBBMS_Free_Auth_Param begin...\n"
#define MBBMS_GBA_1498_112_2_17_23_9_21_123 "MBBMS_Free_Auth_Param End\n"
#define MBBMS_GBA_1504_112_2_17_23_9_21_124 "MBBMS_Free_Post_Param begin...\n"
#define MBBMS_GBA_1508_112_2_17_23_9_21_125 "MBBMS_Free_Post_Param accept ptr freed \n"
#define MBBMS_GBA_1511_112_2_17_23_9_21_126 "MBBMS_Free_Post_Param acceptcharsetptr freed \n"
#define MBBMS_GBA_1515_112_2_17_23_9_21_127 "MBBMS_Free_Post_Param useragentptr freed \n"
#define MBBMS_GBA_1519_112_2_17_23_9_21_128 "MBBMS_Free_Post_Param entityptr freed \n"
#define MBBMS_GBA_1523_112_2_17_23_9_21_129 "MBBMS_Free_Post_Param passwordptr freed \n"
#define MBBMS_GBA_1527_112_2_17_23_9_21_130 "MBBMS_Free_Post_Param usernameptr freed \n"
#define MBBMS_GBA_1531_112_2_17_23_9_21_131 "MBBMS_Free_Post_Param refererptr freed\n"
#define MBBMS_GBA_1535_112_2_17_23_9_21_132 "MBBMS_Free_Post_Param uriptr freed \n"
#define MBBMS_GBA_1547_112_2_17_23_9_21_133 "MBBMS_Free_Post_Param otherheaderptr freed \n"
#define MBBMS_GBA_1552_112_2_17_23_9_21_134 "MBBMS_Free_Post_Param postbodytypeptr freed \n"
#define MBBMS_GBA_1556_112_2_17_23_9_21_135 "MBBMS_Free_Post_Param postbufferptr freed \n"
#define MBBMS_GBA_1559_112_2_17_23_9_21_136 "MBBMS_Free_Post_Param end!\n"
#define MBBMS_GBA_1566_112_2_17_23_9_21_137 "MBBMS_Free_Get_Param begin...\n"
#define MBBMS_GBA_1598_112_2_17_23_9_21_138 "MBBMS_Free_Get_Param end!\n"
#define MBBMS_GBA_1621_112_2_17_23_9_21_139 "MBBMS GBA Generate Auth Param password :%s \n"
#define MBBMS_GBA_1644_112_2_17_23_9_22_140 "\n HTTP TEST no memory \n"
#define MBBMS_GBA_1697_112_2_17_23_9_22_141 "MBBMS RESQN IS FALSE, INIT SEND.....\n"
#define MBBMS_GBA_1718_112_2_17_23_9_22_142 "MBBMS RESQN IS TRUE, INIT SEND.....\n"
#define MBBMS_GBA_1740_112_2_17_23_9_22_143 "MBBMS GBA_REGISTER SEND:\n "
#define MBBMS_GBA_1750_112_2_17_23_9_22_144 "MBBMS_GBA_HTTP REG_REQ  url:%s\n"
#define MBBMS_GBA_1760_112_2_17_23_9_22_145 "MBBMS_GBA_HTTP REG_REQ  url:%s\n"
#define MBBMS_GBA_1775_112_2_17_23_9_22_146 "\n HTTP TEST no memory \n"
#define MBBMS_GBA_1861_112_2_17_23_9_22_147 "MBBMS GBA_AUTHORIZE SEND:\n "
#define MBBMS_GBA_1872_112_2_17_23_9_22_148 "MBBMS_GBA_HTTP REG_REQ  url:%s\n"
#define MBBMS_GBA_1880_112_2_17_23_9_22_149 "MBBMS_GBA_HTTP REG_REQ  url:%s\n"
#define MBBMS_GBA_1896_112_2_17_23_9_22_150 "MBBMS RESQN Prepare.....\n"
#define MBBMS_GBA_1899_112_2_17_23_9_22_151 "MBBMS RESQN Auts base64 string:%s \n"
#define MBBMS_GBA_1903_112_2_17_23_9_22_152 "MBBMS RESQN Parse Nonce FAILED\n"
#define MBBMS_GBA_1909_112_2_17_23_9_22_153 "MBBMS RESQN Parse opaque FAILED, Set opaque empty\n"
#define MBBMS_GBA_1925_112_2_17_23_9_22_154 " MBBMS RESQN: MD5(\"\") = %s \n"
#define MBBMS_GBA_1935_112_2_17_23_9_22_155 "MBBMS RESQN: MD5(\"GET:/:MD5()\") = %s \n"
#define MBBMS_GBA_1946_112_2_17_23_9_22_156 " MBBMS RESQN: MD5(%s:%s:)=%s n"
#define MBBMS_GBA_1964_112_2_17_23_9_22_157 "MBBMS RESQN: final response = %s, set string:%s n"
#define MBBMS_GBA_1981_112_2_17_23_9_22_158 "MBBMS_MD5_Auth_Data cnonce base64 string:%s \n"
#define MBBMS_GBA_1985_112_2_17_23_9_22_159 "MBBMS_MD5_Auth_Data: Parse Nonce FAILED\n"
#define MBBMS_GBA_1990_112_2_17_23_9_22_160 "MBBMS_MD5_Auth_Data: Parse realm FAILED\n"
#define MBBMS_GBA_1995_112_2_17_23_9_22_161 "MBBMS_MD5_Auth_Data Parse opaque FAILED, Opaque is empty \n"
#define MBBMS_GBA_2011_112_2_17_23_9_22_162 "MBBMS_MD5_Auth_Data: MD5(\"\") = %s \n"
#define MBBMS_GBA_2021_112_2_17_23_9_22_163 "MBBMS_MD5_Auth_Data: MD5(\"GET:/:MD5()\") = %s \n"
#define MBBMS_GBA_2037_112_2_17_23_9_22_164 "MBBMS_MD5_Auth_Data RES base64 string:%s \n"
#define MBBMS_GBA_2043_112_2_17_23_9_22_165 " MBBMS_MD5_Auth_Data: MD5(%s:%s:RES)=%s n"
#define MBBMS_GBA_2047_112_2_17_23_9_22_166 "MBBMS_MD5_Auth_Data start:%s \n"
#define MBBMS_GBA_2051_112_2_17_23_9_22_167 "MBBMS_MD5_Auth_Data NONCE:%s \n"
#define MBBMS_GBA_2055_112_2_17_23_9_22_168 "MBBMS_MD5_Auth_Data nc_str:%s \n"
#define MBBMS_GBA_2059_112_2_17_23_9_22_169 "MBBMS_MD5_Auth_Data cnonce:%s \n"
#define MBBMS_GBA_2063_112_2_17_23_9_22_170 "MBBMS_MD5_Auth_Data qop:%s \n"
#define MBBMS_GBA_2067_112_2_17_23_9_22_171 "MBBMS_MD5_Auth_Data end:%s \n"
#define MBBMS_GBA_2073_112_2_17_23_9_22_172 "MBBMS_MD5_Auth_Data: final response = %s, set string:%s n"
#define MBBMS_GBA_2092_112_2_17_23_9_22_173 "MBBMS_GBA 3G SIM HandlE_SIM_ALGO OK: USIM \n"
#define MBBMS_GBA_2108_112_2_17_23_9_22_174 "MBBMS SDCARD Result:status len-data: %d-0x%x res %d-0x%x cnonce %d-0x%x n"
#define MBBMS_GBA_2112_112_2_17_23_9_23_175 "MBBMS_GBA_ALGO RES_len:%d\n"
#define MBBMS_GBA_2115_112_2_17_23_9_23_176 "MBBMS_GBA_ALGO RES 0x%x %x %x %x %x %x %x %x n"
#define MBBMS_GBA_2122_112_2_17_23_9_23_177 "MBBMS SD CNONCE:0x%x \n"
#define MBBMS_GBA_2131_112_2_17_23_9_23_178 "MBBMS THREAD_GBA_SEND_AUTHORIZE starting... \n"
#define MBBMS_GBA_2144_112_2_17_23_9_23_179 "MBBMS_GBA SD Card Return RES ERROR \n"
#define MBBMS_GBA_2155_112_2_17_23_9_23_180 "MBBMS_GBA 2G SIM HandlE_SIM_ALGO OK: SIM \n"
#define MBBMS_GBA_2173_112_2_17_23_9_23_181 "MBBMS_GBA_ALGO 2G RES_len:%d\n"
#define MBBMS_GBA_2176_112_2_17_23_9_23_182 "MBBMS_GBA_ALGO 2G RES 0x%x %x %x %x %x %x %x %x n"
#define MBBMS_GBA_2192_112_2_17_23_9_23_183 "MBBMS 2G SD CNONCE:0x%x \n"
#define MBBMS_GBA_2213_112_2_17_23_9_23_184 "MBBMS_GBA SD Card Return RES ERROR \n"
#define MBBMS_GBA_2228_112_2_17_23_9_23_185 "MBBMS_GBA SIM ALGO CNF USIM ERROR: TO RESQN %d\n"
#define MBBMS_GBA_2235_112_2_17_23_9_23_186 "MBBMS GBA RESQN Prepare Failed \n"
#define MBBMS_GBA_2250_112_2_17_23_9_23_187 "MBBMS_GBA SIM ALGO CNF ABNORMAL ERROR: %d\n"
#define MBBMS_GBA_2268_112_2_17_23_9_23_188 "\"#MBBMS GBA HTTP INIT CNF ERROR!!\n"
#define MBBMS_GBA_2277_112_2_17_23_9_23_189 "\n MBBMS GBA HTTP TEST no memory \n"
#define MBBMS_GBA_2360_112_2_17_23_9_23_190 "\"# MBBMS User Agent NOT SET or too short \n"
#define MBBMS_GBA_2381_112_2_17_23_9_23_191 "\"# MBBMS User Agent NOT SET or too short \n"
#define MBBMS_GBA_2390_112_2_17_23_9_23_192 "MBBMS_HTTP gba_init_req send XML:\nMBBMS %s \nMBBMS---end---\n"
#define MBBMS_GBA_2448_112_2_17_23_9_23_193 "MBBMS_Handle_SIG_HEADER_IND rsp:%d  content:%s \n"
#define MBBMS_GBA_2454_112_2_17_23_9_23_194 "MBBMS_GBA_HTTP_SIG_HEADER_IND HTTP FAILED!!!\n"
#define MBBMS_GBA_2461_112_2_17_23_9_23_195 "MBBMS_GBA_HTTP_SIG_HEADER_IND ok!!!\n"
#define MBBMS_GBA_2472_112_2_17_23_9_23_196 "THREAD_GBA_RESQN HEADER:%s \n"
#define MBBMS_GBA_2489_112_2_17_23_9_23_197 "THREAD_GBA_RESQN RunMBBMSAlgo Starting.... \n"
#define MBBMS_GBA_2494_112_2_17_23_9_23_198 "THREAD_GBA_RESQN RunMbbmsAlgo Failed \n"
#define MBBMS_GBA_2505_112_2_17_23_9_23_199 "THREAD_GBA_RESQN HTTP Failed \n"
#define MBBMS_GBA_2521_112_2_17_23_9_23_200 "MBBMS_GBA_HTTP REG RES data:%s \n"
#define MBBMS_GBA_2544_112_2_17_23_9_24_201 "MBBMS_GBA_SIM RunMBBMSAlgo Starting.... \n"
#define MBBMS_GBA_2549_112_2_17_23_9_24_202 "MBBMS_GBA SIM RunMbbmsAlgo Failed \n"
#define MBBMS_GBA_2563_112_2_17_23_9_24_203 " MBBMS REGISTER RAND, AUTN PARSE FAILED\n"
#define MBBMS_GBA_2569_112_2_17_23_9_24_204 "MBBMS gba register response header overflow\n"
#define MBBMS_GBA_2580_112_2_17_23_9_24_205 "MBBMS_GBA_HTTP_SIG_HEADER_IND THREAD_GBA_SEND_AUTHORIZE FAILED!!!\n"
#define MBBMS_GBA_2591_112_2_17_23_9_24_206 "MBBMS_GBA_HTTP_SIG_HEADER_IND THREAD_GBA_SEND_AUTHORIZE data: \n"
#define MBBMS_GBA_2597_112_2_17_23_9_24_207 "gba register response header overflow:length %d\n"
#define MBBMS_GBA_2611_112_2_17_23_9_24_208 "MBBMS GBA HTTP_SIG_DATA_IND Received:resp code%d \n"
#define MBBMS_GBA_2620_112_2_17_23_9_24_209 "MBBMS GBA HTTP_SIG_DATA_IND len:%d body:%s n"
#define MBBMS_GBA_2632_112_2_17_23_9_24_210 "MBBMS_GBA_HTTP_SIG_CONTENT_IND THREAD_GBA_SEND_AUTHORIZE data:%s \n"
#define MBBMS_GBA_2654_112_2_17_23_9_24_211 "[MBBMS GBA] BTID string:%s \n"
#define MBBMS_GBA_2658_112_2_17_23_9_24_212 "MBBMS GBA UPDATE BTID & KS SUCCESSFUL \n"
#define MBBMS_GBA_2660_112_2_17_23_9_24_213 "MBBMS GBA OPERATION FINISHED, HELLO WORLD !!! \n"
#define MBBMS_GBA_2666_112_2_17_23_9_24_214 "MBBMS GBA UPDATE BTID & KS FAIL. GBA FAIL\n"
#define MBBMS_GBA_2678_112_2_17_23_9_24_215 "MBBMS_GBA THREAD_GBA_SET_BTID_RES Over\n"
#define MBBMS_GBA_2683_112_2_17_23_9_24_216 "MBBMS GBA Parse B-TID and Lifetime FAILED!!!"
#define MBBMS_GBA_2703_112_2_17_23_9_24_217 "MBBMS GBA HTTP_SIG_CLOSE_CNF Received....\n"
#define MBBMS_GBA_2724_112_2_17_23_9_24_218 "MBBMS GBA HTTP_AUTH_REQ: RES is not available !!\n"
#define MBBMS_GBA_2740_112_2_17_23_9_24_219 "MBBMS_GBA_HTTP_SIG_GET_DATA_CNF  THREAD_GBA_SEND_INIT!!!\n"
#define MBBMS_GBA_2741_112_2_17_23_9_24_220 "MBBMS_GBA_SEND_INIT_RES: xml: %s\n"
#define MBBMS_GBA_2747_112_2_17_23_9_24_221 "MBBMS_GBA_HTTP_SIG_GET_DATA_CNF  THREAD_GBA_SEND_INIT Parsed Failed\n"
#define MBBMS_GBA_2763_112_2_17_23_9_24_222 "MBBMS  Parse IMPI IS NOT EQUAL WITH IMSI \n"
#define MBBMS_GBA_2776_112_2_17_23_9_24_223 "MBBMS_GBA_HTTP THREAD_GBA_SEND_INIT Parsed OK IMPI:%s bsf:%sn"
#define MBBMS_GBA_2808_112_2_17_23_9_24_224 "MBBMS_GBA_Thread Starting...... \n"
#define MBBMS_GBA_2823_112_2_17_23_9_24_225 "MBBMS GBA Register MN_APP_PHONE_SERVICE Failed \n"
#define MBBMS_GBA_2830_112_2_17_23_9_24_226 "MBBMS_GBA_THread One signal received %d\n"
#define MBBMS_GBA_2835_112_2_17_23_9_24_227 "MBBMS_INNER_GBA_STOP received\n"
#define MBBMS_GBA_2862_112_2_17_23_9_24_228 "MBBMS_INNER_MRK_STOP received\n"
#define MBBMS_GBA_2874_112_2_17_23_9_24_229 "MBBMS_INNER_EXIT received\n"
#define MBBMS_GBA_2880_112_2_17_23_9_24_230 "MBBMS_INNER_GBA received\n"
#define MBBMS_GBA_2892_112_2_17_23_9_24_231 "MBBMS_INNER_GBA: SIM TYPE Return FAILED\n"
#define MBBMS_GBA_2896_112_2_17_23_9_24_232 "MBBMS_INNER_GBA: SIM TYPE is %d n"
#define MBBMS_GBA_2903_112_2_17_23_9_24_233 "MBBMS_INNER_GBA: SIM TYPE GET FAILED\n"
#define MBBMS_GBA_2909_112_2_17_23_9_24_234 "MBBMS_INNER_GBA: Get IMSI 0x%x %x %x %x n"
#define MBBMS_GBA_2915_112_2_17_23_9_24_235 "MBBMS_INNER_GBA:IMSI string:%s \n"
#define MBBMS_GBA_2940_112_2_17_23_9_24_236 "MBBMS GBA HTTP_InitRequest FAILED \n"
#define MBBMS_GBA_2943_112_2_17_23_9_24_237 "MBBMS_INNER_GBA: GBA HTTP_INIT launched...\n"
#define MBBMS_GBA_2949_112_2_17_23_9_24_238 "MBBMS_INNER_MRK received\n"
#define MBBMS_GBA_2969_112_2_17_23_9_24_239 "MBBMS MRK naf domain:%s \n"
#define MBBMS_GBA_2977_112_2_17_23_9_25_240 "MBBMS MRK NAF-ID: %s 0x1 0 0 0 1\n"
#define MBBMS_GBA_2979_112_2_17_23_9_25_241 "MBBMS MRK impi: %s\n"
#define MBBMS_GBA_2998_112_2_17_23_9_25_242 "MBBMS GBA MRK SD CARD OK len:%d 0x%x %x %x %xn"
#define MBBMS_GBA_3001_112_2_17_23_9_25_243 "MBBMS GBA MRK SUCCESSFUL!\n"
#define MBBMS_GBA_3006_112_2_17_23_9_25_244 "MBBMS MRK Result:len:%d ks_ext_naf:0x%2x %2x %2x %2x n"
#define MBBMS_GBA_3008_112_2_17_23_9_25_245 "MBBMS MRK finished Successfully\n"
#define MBBMS_GBA_3017_112_2_17_23_9_25_246 "MBBMS GBA MRK DATA ERROR \n"
#define MBBMS_GBA_3029_112_2_17_23_9_25_247 "MBBMS GBA MRK Return SD Failed\n"
#define MBBMS_GBA_3044_112_2_17_23_9_25_248 "MBBMS_GBA HTT_SIG_INIT_CNF received\n"
#define MBBMS_GBA_3063_112_2_17_23_9_25_249 "MBBMS_GBA HTTP_SIG_NEED_AUTH_IND received\n"
#define MBBMS_GBA_3070_112_2_17_23_9_25_250 "MBBMS_GBA APP_MN_SIM_MBBMS_ALGO_CNF received\n"
#define MBBMS_GBA_3080_112_2_17_23_9_25_251 "MBBMS_GBA HTTP_SIG_REQUEST_ID_IND received\n"
#define MBBMS_GBA_3089_112_2_17_23_9_25_252 "MBBMS_GBA HTTP_SIG_GET_CNF received\n"
#define MBBMS_GBA_3098_112_2_17_23_9_25_253 "MBBMS_GBA HTTP_SIG_HEAD_CNF received\n"
#define MBBMS_GBA_3107_112_2_17_23_9_25_254 "MBBMS_GBA HTTP_SIG_DATA_IND received\n"
#define MBBMS_GBA_3114_112_2_17_23_9_25_255 "MBBMS_GBA HTTP_SIG_HEADER_IND received\n"
#define MBBMS_GBA_3121_112_2_17_23_9_25_256 "MBBMS_GBA HTTP_SIG_ERROR_IND received\n"
#define MBBMS_GBA_3134_112_2_17_23_9_25_257 "MBBMS_GBA HTTP_SIG_CLOSE_CNF received\n"
#define MBBMS_GBA_3150_112_2_17_23_9_25_258 "MBBMS GBA Thread: Freeing gba_sig_ptr \n"
#define MBBMS_GBA_3152_112_2_17_23_9_25_259 "MBBMS GBA Thread: Freed gba_sig_ptr \n"
#define MBBMS_GBA_3158_112_2_17_23_9_25_260 "MBBMS_GBA HTTP_SIG_CANCEL_CNF received\n"
#define MBBMS_GBA_3165_112_2_17_23_9_25_261 "\n http test net prog ind\n"
#define MBBMS_GBA_3168_112_2_17_23_9_25_262 "\n http test redirect ind\n"
#define MBBMS_GBA_3172_112_2_17_23_9_25_263 "MBBMS_GBA HTTP_SIG_POST_CNF received\n"
#define MBBMS_GBA_3202_112_2_17_23_9_25_264 "MBBMS GBA: set callback function\n"
#define MBBMS_GBA_3208_112_2_17_23_9_25_265 "MBBMS GBA: clean callback function\n"
#define MBBMS_GBA_3213_112_2_17_23_9_25_266 "MBBMS GBA: set net id to [%d]\n"
#define MBBMS_GBA_3252_112_2_17_23_9_25_267 "parse life time year:%d month:%d day:%d hour:%d minute:%d second:%d"
#define MBBMS_GBA_3260_112_2_17_23_9_25_268 "mbbms_get_current_time %s, len:%d\n"
#define MBBMS_GBA_3262_112_2_17_23_9_25_269 "MBBMS_get_current_time FAILED!!!\n"
#define MBBMS_GBA_3264_112_2_17_23_9_25_270 "MBBMS_GBA_INIT failed!!!\n"
#define MBBMS_GBA_3266_112_2_17_23_9_25_271 "MBBMS_GBA_Init SUcess!\n"
#define MBBMS_GBA_3274_112_2_17_23_9_25_272 "base64 encode successful!\n len=%d \n %s \n"
#define MBBMS_GBA_3277_112_2_17_23_9_25_273 "base64 decode successful!\n len=%d \n %s \n"
#define MBBMS_GBA_3279_112_2_17_23_9_25_274 "base64 decode FAILED!!!\n"
#define MBBMS_GBA_3282_112_2_17_23_9_25_275 "base64 encode FAILED!!!\n"
#define MBBMS_GBA_3288_112_2_17_23_9_25_276 "MD5 of \"\" :\n"
#define MBBMS_GBA_3291_112_2_17_23_9_25_277 "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
#define MBBMS_GBA_3295_112_2_17_23_9_25_278 "MD5 of \"a\" :\n"
#define MBBMS_GBA_3298_112_2_17_23_9_25_279 "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
#define MBBMS_GBA_3303_112_2_17_23_9_25_280 "MD5 of \"abc\" :\n"
#define MBBMS_GBA_3306_112_2_17_23_9_25_281 "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
#define MBBMS_GBA_3310_112_2_17_23_9_25_282 "mbbms init res ok! bsf:%s impi:%s \n"
#define MBBMS_GBA_3313_112_2_17_23_9_25_283 "mbbms auth response failed!!!\n"
#define MBBMS_GBA_3319_112_2_17_23_9_25_284 "mbbms auth ok! date:%d-%d-%d n"
#define MBBMS_GBA_3321_112_2_17_23_9_25_285 "mbbms auth ok! lifetime:%d time:%d:%d:%d n"
#define MBBMS_GBA_3323_112_2_17_23_9_25_286 "mbbms auth ok! btid:%s \n"
#define MBBMS_GBA_3326_112_2_17_23_9_25_287 "mbbms auth response failed!!!\n"
#define MBBMS_GBA_3332_112_2_17_23_9_25_288 "MBBMS_Set_WAP failed!!!\n"
#define MBBMS_GBA_3334_112_2_17_23_9_25_289 "MBBMS_Set_WAP SUcess!\n"
#define MBBMS_GBA_3336_112_2_17_23_9_25_290 "MBBMS_Get_WAP failed!!!\n"
#define MBBMS_GBA_3338_112_2_17_23_9_25_291 "MBBMS_Get_WAP SUcess! wap:%s, port:%d\n"
#define MBBMS_GBA_3342_112_2_17_23_9_25_292 "MBBMS_Set_NAF failed!!!\n"
#define MBBMS_GBA_3344_112_2_17_23_9_25_293 "MBBMS_Set_NAF SUcess!\n"
#define MBBMS_GBA_3346_112_2_17_23_9_25_294 "MBBMS_Get_NAF failed!!!\n"
#define MBBMS_GBA_3348_112_2_17_23_9_25_295 "MBBMS_Get_NAF SUcess! wap:%s, port:%d\n"
#define MBBMS_GBA_3352_112_2_17_23_9_25_296 "MBBMS_Set_CMMBSN failed!!!\n"
#define MBBMS_GBA_3354_112_2_17_23_9_25_297 "MBBMS_Set_CMMBSN SUcess!\n"
#define MBBMS_GBA_3357_112_2_17_23_9_25_298 "MBBMS_Get_CMMBSN failed!!!\n"
#define MBBMS_GBA_3359_112_2_17_23_9_25_299 "MBBMS_Get_CMMBSN SUcess! cmmbsn:%s, len:%d\n"
#define MBBMS_GBA_3362_112_2_17_23_9_25_300 "MBBMS_SIM_Support_CAS failed!!!\n"
#define MBBMS_GBA_3364_112_2_17_23_9_25_301 "MBBMS_SIM_Support_CAS SUcess!\n"
#define MBBMS_GBA_3367_112_2_17_23_9_25_302 "MBBMS_GBA_Operation failed!!!\n"
#define MBBMS_GBA_3369_112_2_17_23_9_25_303 "MBBMS_GBA_Operation SUcess!\n"
#define MBBMS_GBA_3373_112_2_17_23_9_25_304 "MBBMS_GBA_Stop_Operation failed!!!\n"
#define MBBMS_GBA_3375_112_2_17_23_9_25_305 "MBBMS_GBA_Stop_Operation SUcess!\n"
#define MBBMS_GBA_3378_112_2_17_23_9_25_306 "MBBMS_MRK_Operation failed!!!\n"
#define MBBMS_GBA_3380_112_2_17_23_9_25_307 "MBBMS_MRK_Operation SUcess!\n"
#define MBBMS_GBA_3383_112_2_17_23_9_25_308 "MBBMS_MRK_Stop_Operation failed!!!\n"
#define MBBMS_GBA_3385_112_2_17_23_9_25_309 "MBBMS_MRK_Stop_Operation SUcess!\n"
#define MBBMS_SG_642_112_2_17_23_9_27_310 "[SP] i %d,x%x \n"
#define MBBMS_SG_1002_112_2_17_23_9_27_311 "Esg_InflateData() ,src_handle = %d, dst_handle = %d"
#define MBBMS_SG_1014_112_2_17_23_9_27_312 "Esg_InflateData() ,z_inflateInit2_ = %d!"
#define MBBMS_SG_1049_112_2_17_23_9_28_313 "Esg_InflateData() ,z_inflate = %d!"
#define MBBMS_SG_1060_112_2_17_23_9_28_314 "Esg_InflateData() ,ignore -3 error. now ret = %d!"
#define MBBMS_SG_1070_112_2_17_23_9_28_315 "Esg_InflateData() ,SFS_WriteFile = %d!"
#define MBBMS_SG_1197_112_2_17_23_9_28_316 "[SG] SetAllocFail flag=%d"
#define MBBMS_SG_1212_112_2_17_23_9_28_317 "[SG] SetParseAgain flag=%d"
#define MBBMS_SG_1348_112_2_17_23_9_28_318 "[SG]:  SendPreviewSigToMMI \n"
#define MBBMS_SG_1439_112_2_17_23_9_28_319 "[SG]:SendStopSigToMmi  g_mbbms_stop_flag %d\n"
#define MBBMS_SG_1455_112_2_17_23_9_28_320 "[SG]:SendStopSigToMmi\n"
#define MBBMS_SG_1483_112_2_17_23_9_28_321 "[SG]:InitHttpRequest  SendMemAllocFailToMmi \n"
#define MBBMS_SG_1502_112_2_17_23_9_28_322 "[SG] InitHttpRequest: task_id 0x%x\n "
#define MBBMS_SG_1548_112_2_17_23_9_29_323 "[SG]: InitHttpRequest wap=%s \n, port 0x%x,netid %d"
#define MBBMS_SG_1558_112_2_17_23_9_29_324 "[SG]:InitHttpRequest  HTTP_InitRequest error %d \n"
#define MBBMS_SG_1654_112_2_17_23_9_29_325 "[SG] InitPreviewDataHttpReqest! enter tick %d ,is_success %d \n"
#define MBBMS_SG_1822_112_2_17_23_9_29_326 "[SG] SetProccessFlag state %d\n "
#define MBBMS_SG_1843_112_2_17_23_9_29_327 "[SG] SetState state %d\n "
#define MBBMS_SG_1915_112_2_17_23_9_29_328 "[SG] HandleHTTPCloseCnf\n "
#define MBBMS_SG_1961_112_2_17_23_9_29_329 "[SG] HandleHTTPCloseCnf   sgdd has been downloaded\n"
#define MBBMS_SG_1968_112_2_17_23_9_29_330 "[SG] HandleHTTPCloseCnf   sgdd same with old sgdd\n"
#define MBBMS_SG_2030_112_2_17_23_9_30_331 "[SG]:CloseHttpRequest HTTP_CloseRequest %d\n "
#define MBBMS_SG_2091_112_2_17_23_9_30_332 "[SG] RenamePreviewDataFile!delete old file sfs_err%d\n"
#define MBBMS_SG_2102_112_2_17_23_9_30_333 "[SG] RenamePreviewDataFile!diffrent:fail to rename sfs_err%d\n"
#define MBBMS_SG_2121_112_2_17_23_9_30_334 "[SG] GetFileFormat "
#define MBBMS_SG_2134_112_2_17_23_9_30_335 "[SG] GetFileFormat fail to read File err %d \n"
#define MBBMS_SG_2143_112_2_17_23_9_30_336 "[SG] GetFileFormat format_type 0x%x "
#define MBBMS_SG_2159_112_2_17_23_9_30_337 "[SG] GetFileFormat fail to set File point err%d\n"
#define MBBMS_SG_2212_112_2_17_23_9_30_338 "[SG] ProcessPreviewDataHTTPContentFile! begin inflate \n"
#define MBBMS_SG_2222_112_2_17_23_9_30_339 "[SG]InflateFile  handle = %x!"
#define MBBMS_SG_2234_112_2_17_23_9_30_340 "[SG] ProcessPreviewDataHTTPContentFile inflate_retf %d\n"
#define MBBMS_SG_2238_112_2_17_23_9_30_341 "[SG] ProcessPreviewDataHTTPContentFile fail to Inflate File\n"
#define MBBMS_SG_2247_112_2_17_23_9_30_342 "[SG] ProcessPreviewDataHTTPContentFile! non-gzip \n"
#define MBBMS_SG_2261_112_2_17_23_9_30_343 "[SG] ProcessPreviewDataHTTPContentFile! end tick %d \n"
#define MBBMS_SG_2282_112_2_17_23_9_30_344 "[SG]:HandleHTTPGetCnf  tick %d \n"
#define MBBMS_SG_2286_112_2_17_23_9_30_345 "[SG]:HandleHTTPGetCnf  g_mbbms_stop_flag %d ,data len %d, response_code %dn"
#define MBBMS_SG_2311_112_2_17_23_9_30_346 "[SG]:HandleHTTPDataCnf  HTTP_CloseRequest error %d \n"
#define MBBMS_SG_2342_112_2_17_23_9_30_347 "[SG] HandleHTTPDataCnf   sgdd same with old sgdd\n"
#define MBBMS_SG_2368_112_2_17_23_9_30_348 "[SG] InitHttpRequest: task_id 0x%x ,signalcode 0x%x\n "
#define MBBMS_SG_2497_112_2_17_23_9_30_349 "[SG] SetDeviceName \n "
#define MBBMS_SG_2521_112_2_17_23_9_30_350 "[SG] CleanDeviceName \n "
#define MBBMS_SG_2543_112_2_17_23_9_31_351 "[SG]MBBMS_SGInit full_path_len%d\n "
#define MBBMS_SG_2551_112_2_17_23_9_31_352 "[SG]MBBMS_SGInit  no mem\n "
#define MBBMS_SG_2567_112_2_17_23_9_31_353 "[SG]MBBMS_SGInit  no mem\n "
#define MBBMS_SG_2615_112_2_17_23_9_31_354 "[SG]MBBMS_SGInit %d \n "
#define MBBMS_SG_2658_112_2_17_23_9_31_355 "[SG]MBBMS_SGExit .\n "
#define MBBMS_SG_2730_112_2_17_23_9_31_356 "[SG]: SendHttpPostRequest  sethttpverb \n"
#define MBBMS_SG_2743_112_2_17_23_9_31_357 "[SG]: SendHttpPostRequest  to  delete file ,err  = %d\n"
#define MBBMS_SG_2775_112_2_17_23_9_31_358 "[SG]: SendHttpGetRequest  context_id %d \n"
#define MBBMS_SG_2814_112_2_17_23_9_31_359 "[SG]: SendHttpGetRequest to  delete file ,err  = %d\n"
#define MBBMS_SG_2820_112_2_17_23_9_31_360 "[SG]: SendHttpGetRequest  transport_id %d \n"
#define MBBMS_SG_2840_112_2_17_23_9_31_361 "[SG]:SetPostData  HTTP_PostRequest error  %d\n "
#define MBBMS_SG_2871_112_2_17_23_9_31_362 "[SG]: HandleHTTPInitSessionCnf\n"
#define MBBMS_SG_2924_112_2_17_23_9_31_363 "[SG]: HandleHTTPInitSessionCnf  finish \n"
#define MBBMS_SG_2935_112_2_17_23_9_31_364 "[SG]: InitHTTPContentInfo   \n"
#define MBBMS_SG_2947_112_2_17_23_9_31_365 "[SG]: SetHTTPContentInfo  status code %d,err_type %d,err_code %d\n"
#define MBBMS_SG_2957_112_2_17_23_9_31_366 "[SG]: SetHTTPContentInfo  encoding type %d\n"
#define MBBMS_SG_2966_112_2_17_23_9_31_367 "[SG]: GetHTTPContentInfo  encoding  type %d\n"
#define MBBMS_SG_2981_112_2_17_23_9_31_368 "[SG]:HandleHTTPHeaderInd  tick %d \n"
#define MBBMS_SG_2982_112_2_17_23_9_31_369 "[SG]: HandleHTTPHeaderInd\n"
#define MBBMS_SG_2998_112_2_17_23_9_31_370 "[SG]: HandleHTTPHeaderInd,receive fail %d\n"
#define MBBMS_SG_3015_112_2_17_23_9_31_371 "[SG]: HandleHTTPHeaderInd,receive success\n"
#define MBBMS_SG_3025_112_2_17_23_9_32_372 "[SG]: HandleHTTPHeaderInd,receive  %d\n"
#define MBBMS_SG_3053_112_2_17_23_9_32_373 "[SG]: GetSGDDInfo\n"
#define MBBMS_SG_3074_112_2_17_23_9_32_374 "[SG] GetSGDDInfo version:%d,id:%s\n"
#define MBBMS_SG_3221_112_2_17_23_9_32_375 "[SG]InflateFile  device_name = %x!"
#define MBBMS_SG_3229_112_2_17_23_9_32_376 "[SG]InflateFile SFS_GetDeviceFreeSpace, no enouth space"
#define MBBMS_SG_3235_112_2_17_23_9_32_377 "[SG]InflateFile SFS_GetDeviceFreeSpace! error = %d!"
#define MBBMS_SG_3247_112_2_17_23_9_32_378 "[SG]InflateFile  handle = %x!"
#define MBBMS_SG_3268_112_2_17_23_9_32_379 "[SG]InflateFile create file fail"
#define MBBMS_SG_3287_112_2_17_23_9_32_380 "[SG]: CopyContentString, data_len=%d"
#define MBBMS_SG_3322_112_2_17_23_9_32_381 "[SG] MBBMSSG_DownloadPreviewData! start tick %d\n"
#define MBBMS_SG_3339_112_2_17_23_9_32_382 "[SG]:MBBMSSG_DownloadPreviewData g_mbbms_task_id %d\n "
#define MBBMS_SG_3351_112_2_17_23_9_32_383 "[SG]: MBBMS_FreeBSMSelector"
#define MBBMS_SG_3373_112_2_17_23_9_32_384 "[SG]: MBBMS_GetBSMSelector\n"
#define MBBMS_SG_3505_112_2_17_23_9_32_385 "[SG]: SGDDGetStatus, status=%d"
#define MBBMS_SG_3681_112_2_17_23_9_33_386 "[SG]: SGDDSyntaxHandler Sprd_LinkListCreate\n"
#define MBBMS_SG_3685_112_2_17_23_9_33_387 "[SG]:SGDDSyntaxHandler BSMSelector\n"
#define MBBMS_SG_3701_112_2_17_23_9_33_388 "[SG]:SGDDSyntaxHandler id_BSMSelector len=%d,%s\n"
#define MBBMS_SG_3936_112_2_17_23_9_33_389 "[SG]: SGDDSyntaxHandler, status=%d"
#define MBBMS_SG_3943_112_2_17_23_9_33_390 "[SG]: SGDDSyntaxHandler, Id_SGDD=%s,len="
#define MBBMS_SG_3948_112_2_17_23_9_33_391 "[SG]: SGDDSyntaxHandler, Version_SGDD=%d"
#define MBBMS_SG_4083_112_2_17_23_9_34_392 "[SG]:SGDDSyntaxHandler Fragment use old fragment \n"
#define MBBMS_SG_4220_112_2_17_23_9_34_393 "[SG]: GetSyntaxMappedId failed, element=%s,syntax_id=%d, nest=%d, map_size=%d"
#define MBBMS_SG_4323_112_2_17_23_9_34_394 "[SG]: ServiceFragmentDataHandler, character=%s, len=%d"
#define MBBMS_SG_4351_112_2_17_23_9_34_395 "[SG]: WriteBlockDataToFile, data_len= %d, src_handle =%d,dst_handle %d"
#define MBBMS_SG_4376_112_2_17_23_9_34_396 "[SG]: WriteBlockDataToFile,read file err %d"
#define MBBMS_SG_4389_112_2_17_23_9_34_397 "[SG]: WriteBlockDataToFile,write file err %d"
#define MBBMS_SG_4433_112_2_17_23_9_34_398 "[SG]:SG_SprdCleanSGDDStructure\n"
#define MBBMS_SG_4439_112_2_17_23_9_34_399 "[SG]:SG_SprdCleanSGDDStructure  sgdd_ptr->id\n"
#define MBBMS_SG_4522_112_2_17_23_9_34_400 "[SG]:Esg_ParseFileByHandle() handle%x"
#define MBBMS_SG_4606_112_2_17_23_9_35_401 "[SG]:Parse error "
#define MBBMS_SG_4613_112_2_17_23_9_35_402 "[SG] ParseFileByHandle, alloc mem fail!"
#define MBBMS_SG_4628_112_2_17_23_9_35_403 "[SG]:Parse CFLXML_FreeParser "
#define MBBMS_SG_4637_112_2_17_23_9_35_404 "[SG]:Esg_ParseFileByHandle() handle%x"
#define MBBMS_SG_4650_112_2_17_23_9_35_405 "[SG]:ParseFileByHandle(): create parser fail"
#define MBBMS_SG_4669_112_2_17_23_9_35_406 "[SG]:ParseFileByHandle charsread %d,ffs_error %d\n "
#define MBBMS_SG_4677_112_2_17_23_9_35_407 "[SG]:Parse error at line %dn %sn"
#define MBBMS_SG_4703_112_2_17_23_9_35_408 "[SG] IsExistSgdd \n"
#define MBBMS_SG_4707_112_2_17_23_9_35_409 "[SG] IsExistSgdd, no initial! \n"
#define MBBMS_SG_4723_112_2_17_23_9_35_410 "[SG] IsExistSgdd is exist %d\n"
#define MBBMS_SG_4769_112_2_17_23_9_35_411 "[SG] WriteDataToFile file type %d, data_len 0x%x \n"
#define MBBMS_SG_4784_112_2_17_23_9_35_412 "[SG] WriteDataToFile fail to create file\n"
#define MBBMS_SG_4789_112_2_17_23_9_35_413 "[SG] WriteDataToFile write data method (1) %d\n"
#define MBBMS_SG_4843_112_2_17_23_9_35_414 "[SG] WriteDataToFile write data method (1) %d\n"
#define MBBMS_SG_4853_112_2_17_23_9_35_415 "[SG] WriteDataToFile fail to create file\n"
#define MBBMS_SG_4866_112_2_17_23_9_35_416 "[SG] WriteDataToFile write data method (2) %d\n"
#define MBBMS_SG_4901_112_2_17_23_9_35_417 "[SG] SplitFile is_exist_sgdd=%d, is_gzip = %d\n"
#define MBBMS_SG_4911_112_2_17_23_9_35_418 "[SG] SplitFile delete decomp file ret =%d \n"
#define MBBMS_SG_4924_112_2_17_23_9_35_419 "[SG] SplitFile rename decomp file ret =%d \n"
#define MBBMS_SG_4946_112_2_17_23_9_35_420 "[SG] SpiteFile file size 0x%x \n"
#define MBBMS_SG_4951_112_2_17_23_9_35_421 "[SG] SpiteFile file size 0x%x,data len 0x%x \n"
#define MBBMS_SG_4977_112_2_17_23_9_35_422 "[SG] SpliteFile read  len 0x%x\n"
#define MBBMS_SG_4980_112_2_17_23_9_35_423 "[SG] SpiteFile offset 0x%x,data_ptr 0x%x\n"
#define MBBMS_SG_5011_112_2_17_23_9_35_424 "[SG] WriteDataToFile fail to write sgdd\n"
#define MBBMS_SG_5025_112_2_17_23_9_36_425 "[SG] WriteDataToFile fail to write sgdu\n"
#define MBBMS_SG_5034_112_2_17_23_9_36_426 "[SG] WriteDataToFile fail to write sgdu bcast\n"
#define MBBMS_SG_5074_112_2_17_23_9_36_427 "[SG] ParseSGDD \n"
#define MBBMS_SG_5082_112_2_17_23_9_36_428 "[SG] ParseSGDD fail to open fail\n"
#define MBBMS_SG_5107_112_2_17_23_9_36_429 "[SG] ParseSGDD fail to parse and delete sgdd sfs_error 0x%x \n"
#define MBBMS_SG_5113_112_2_17_23_9_36_430 "[SG] ParseSGDD finish ret=%d\n"
#define MBBMS_SG_5322_112_2_17_23_9_36_431 "[SG] UpdateFragmentFile   transport_id %d\n"
#define MBBMS_SG_5358_112_2_17_23_9_36_432 "[SG] DeleteFragmentFileFromIndexFile  \n"
#define MBBMS_SG_5371_112_2_17_23_9_36_433 "[SG] DeleteFragmentFileFromIndexFile  \n"
#define MBBMS_SG_5395_112_2_17_23_9_36_434 "[SG] DeleteFragmentFileFromIndexFile  fail to alloc mem\n"
#define MBBMS_SG_5406_112_2_17_23_9_36_435 "[SG] DeleteFragmentFileFromIndexFile  fail to alloc mem\n"
#define MBBMS_SG_5574_112_2_17_23_9_37_436 "[SG] DeletePreviewDataFile  transport_id %d \n"
#define MBBMS_SG_5584_112_2_17_23_9_37_437 "[SG] DeletePreviewDataFile fail to delete picture file %d \n"
#define MBBMS_SG_5664_112_2_17_23_9_37_438 "[SG] DeleteFragmentFile  transport_id %d\n"
#define MBBMS_SG_5673_112_2_17_23_9_37_439 "[SG] DeleteFragmentFile fail to delete fragment file \n"
#define MBBMS_SG_5739_112_2_17_23_9_37_440 "[SG] UpdateSGDDFile is_same %d\n"
#define MBBMS_SG_5754_112_2_17_23_9_37_441 "[SG] UpdateSGDDFile :fail to delete sgdd,sfs_err1 %d,\n"
#define MBBMS_SG_5768_112_2_17_23_9_37_442 "[SG] UpdateSGDDFile :fail to rename ,sfs_err1%d\n"
#define MBBMS_SG_5781_112_2_17_23_9_37_443 "[SG] UpdateSGDDFile! same:fail to delete:,sfs_err1\n"
#define MBBMS_SG_5823_112_2_17_23_9_37_444 "[SG]: AddDeltaSGDU enter  tickcount(1)%d"
#define MBBMS_SG_5845_112_2_17_23_9_37_445 "[SG]: AddDeltaSGDU open sgdu file error"
#define MBBMS_SG_5855_112_2_17_23_9_37_446 "[SG]: AddDeltaSGDU get sgdu file size error"
#define MBBMS_SG_5864_112_2_17_23_9_37_447 "[SG]: AddDeltaSGDU get sgdu header error"
#define MBBMS_SG_5920_112_2_17_23_9_37_448 "[SG]: AddDeltaSGDU read   frag id %d, err %d\n"
#define MBBMS_SG_5924_112_2_17_23_9_37_449 "[SG]: AddDeltaSGDU get desp info error"
#define MBBMS_SG_5940_112_2_17_23_9_37_450 "[SG]: AddDeltaSGDU get type error"
#define MBBMS_SG_5949_112_2_17_23_9_37_451 "[SG]: AddDeltaSGDU update fragment transport id %d,act %d"
#define MBBMS_SG_5957_112_2_17_23_9_37_452 "[SG]: AddDeltaSGDU  wrong despinfo_len"
#define MBBMS_SG_5968_112_2_17_23_9_37_453 "[SG]: AddDeltaSGDU write  frag id %d, err %d\n"
#define MBBMS_SG_5973_112_2_17_23_9_37_454 "[SG]: AddDeltaSGDU write desp info error"
#define MBBMS_SG_5990_112_2_17_23_9_37_455 "[SG]: AddDeltaSGDU write fragment index error"
#define MBBMS_SG_6001_112_2_17_23_9_37_456 "[SG]: AddDeltaSGDU unknow encoding type =%d"
#define MBBMS_SG_6028_112_2_17_23_9_38_457 "[SG]: AddDeltaSGDU end tick(2) %d"
#define MBBMS_SG_6120_112_2_17_23_9_38_458 "[SG]: UpdateSGDUFile write desp begin tag error"
#define MBBMS_SG_6152_112_2_17_23_9_38_459 "[SG]: UpdateSGDUFile write desp begin tag error"
#define MBBMS_SG_6182_112_2_17_23_9_38_460 "[SG]: UpdateSGDUFile write desp begin tag error"
#define MBBMS_SG_6232_112_2_17_23_9_38_461 "[SG]: UpdateSGDUFile frag id %d, is_exist %d ,act %d"
#define MBBMS_SG_6288_112_2_17_23_9_38_462 "[SG]: UpdateSGDUFile fail to set point in sgdu "
#define MBBMS_SG_6298_112_2_17_23_9_38_463 "[SG]: UpdateSGDUFile read old  frag id %d, err %d\n"
#define MBBMS_SG_6301_112_2_17_23_9_38_464 "[SG]: UpdateSGDUFile read sgdu error"
#define MBBMS_SG_6309_112_2_17_23_9_38_465 "[SG]: UpdateSGDUFile write sgdu error"
#define MBBMS_SG_6324_112_2_17_23_9_38_466 "[SG]: UpdateSGDUFile write old  frag id %d, err %d\n"
#define MBBMS_SG_6327_112_2_17_23_9_38_467 "[SG]: UpdateSGDUFile write fragment index error"
#define MBBMS_SG_6364_112_2_17_23_9_38_468 "[SG]: UpdateSGDUFile write sgdu error"
#define MBBMS_SG_6382_112_2_17_23_9_38_469 "[SG]: UpdateSGDUFile fail to delete sgdu "
#define MBBMS_SG_6394_112_2_17_23_9_38_470 "[SG]: UpdateSGDUFile fail to rename sgdu err"
#define MBBMS_SG_6519_112_2_17_23_9_39_471 "[SG] CompareSGDD  update transport id \n"
#define MBBMS_SG_6580_112_2_17_23_9_39_472 "[SG] CompareSGDD  fail to delete old frag index\n"
#define MBBMS_SG_6591_112_2_17_23_9_39_473 "[SG] CompareSGDD  fail to rename frag index\n"
#define MBBMS_SG_6617_112_2_17_23_9_39_474 "[SG] HandleAllocFail! \n"
#define MBBMS_SG_6649_112_2_17_23_9_39_475 "[SG] UpdateAllSG  CreateXMLSGDUFile status=%d\n"
#define MBBMS_SG_6652_112_2_17_23_9_39_476 "[SG] ParseHTTPContentfile  CreateXMLSGDUFile status=%d\n"
#define MBBMS_SG_6658_112_2_17_23_9_39_477 "[SG] UpdateAllSG  failt to create sgdd file\n"
#define MBBMS_SG_6666_112_2_17_23_9_39_478 "[SG] UpdateAllSG  receive success\n"
#define MBBMS_SG_6679_112_2_17_23_9_39_479 "[SG] ParseHTTPContentfile  CreateXMLSGDUFile status=%d\n"
#define MBBMS_SG_6686_112_2_17_23_9_39_480 "[SG] UpdateAllSG  imcomplete data\n"
#define MBBMS_SG_6740_112_2_17_23_9_39_481 "[SG] ParseAndCompare! \n"
#define MBBMS_SG_6756_112_2_17_23_9_39_482 "[SG] ParseAndCompare! fail to parse new sgdd\n"
#define MBBMS_SG_6803_112_2_17_23_9_39_483 "[SG] ParseAndCompare! fail to parse old sgdd\n"
#define MBBMS_SG_6839_112_2_17_23_9_39_484 "[SG] ParseAndCompare! they are diffrent \n"
#define MBBMS_SG_6843_112_2_17_23_9_39_485 "[SG] ParseAndCompare! they are same\n"
#define MBBMS_SG_6893_112_2_17_23_9_39_486 "[SG] RenameTempFile!diffrent:fail to delete old file sfs_err %d\n"
#define MBBMS_SG_6903_112_2_17_23_9_39_487 "[SG] RenameTempFile!diffrent:fail to rename sfs_err1%d\n"
#define MBBMS_SG_7011_112_2_17_23_9_39_488 "[SG]: DeleteALLFragmentFile sfs_err %d\n"
#define MBBMS_SG_7053_112_2_17_23_9_40_489 "[SG]: DeleteALLFragmentFile \n"
#define MBBMS_SG_7075_112_2_17_23_9_40_490 "[SG]: DeleteALLFragmentFile fail to alloc mem \n"
#define MBBMS_SG_7099_112_2_17_23_9_40_491 "[SG]: DeleteALLFragmentFile sfs_err %d\n"
#define MBBMS_SG_7141_112_2_17_23_9_40_492 "[SG] DeleteALLFragmentFile!:fail to delete index sfs_err %d \n"
#define MBBMS_SG_7160_112_2_17_23_9_40_493 "[SG] DeleteAllSGFile \n"
#define MBBMS_SG_7171_112_2_17_23_9_40_494 "[SG] DeleteAllSGFile!:fail to delete sfs_err %d,sfs_err1 %d,\n"
#define MBBMS_SG_7200_112_2_17_23_9_40_495 "[SG]: IsCompleteFragmentIndexFile enter"
#define MBBMS_SG_7205_112_2_17_23_9_40_496 "[SG]: IsCompleteFragmentIndexFile handle=%d"
#define MBBMS_SG_7220_112_2_17_23_9_40_497 "[SG]: IsCompleteFragmentIndexFile is_imcomplete =%d"
#define MBBMS_SG_7250_112_2_17_23_9_40_498 "[SG] ParseHTTPContentfile! enter tick %d ,is_success %d \n"
#define MBBMS_SG_7291_112_2_17_23_9_40_499 "[SG] ParseHTTPContentfile ParseAndCompare is ok\n"
#define MBBMS_SG_7298_112_2_17_23_9_40_500 "[SG] ParseHTTPContentfile fail to ParseAndCompare! \n"
#define MBBMS_SG_7322_112_2_17_23_9_40_501 "[SG] ParseHTTPContentfile  CreateXMLSGDUFile status=%d\n"
#define MBBMS_SG_7335_112_2_17_23_9_40_502 "[SG] ParseHTTPContentfile  receive success\n"
#define MBBMS_SG_7360_112_2_17_23_9_40_503 "[SG] ParseHTTPContentfile  imcomplete data\n"
#define MBBMS_SG_7395_112_2_17_23_9_40_504 "[SG] ParseHTTPContentfile!type %d, end tick %d\n"
#define MBBMS_SG_7415_112_2_17_23_9_40_505 "[SG]: GetSGDDStatusCode\n"
#define MBBMS_SG_7436_112_2_17_23_9_40_506 "[SG] GetSGDDStatusCode status:%d\n"
#define MBBMS_SG_7470_112_2_17_23_9_40_507 "[SG] SetStatusCode! enter tick %d ,is_success %d \n"
#define MBBMS_SG_7494_112_2_17_23_9_40_508 "[SG] SetStatusCode fail to get File\n"
#define MBBMS_SG_7506_112_2_17_23_9_40_509 "[SG] SetStatusCode fail to get format \n"
#define MBBMS_SG_7518_112_2_17_23_9_41_510 "[SG] SetStatusCode! begin inflate \n"
#define MBBMS_SG_7523_112_2_17_23_9_41_511 "[SG] SetStatusCode inflate_retf %d\n"
#define MBBMS_SG_7531_112_2_17_23_9_41_512 "[SG] SetStatusCode fail to Inflate File\n"
#define MBBMS_SG_7540_112_2_17_23_9_41_513 "[SG] SetStatusCode! non-gzip \n"
#define MBBMS_SG_7581_112_2_17_23_9_41_514 "[SG] SetStatusCode status:%d\n"
#define MBBMS_SG_7606_112_2_17_23_9_41_515 "[SG]:HandleHTTPDataCnf  tick %d \n"
#define MBBMS_SG_7609_112_2_17_23_9_41_516 "[SG]:HandleHTTPDataCnf  g_mbbms_stop_flag %d ,data len %d, is_success %dn"
#define MBBMS_SG_7646_112_2_17_23_9_41_517 "[SG] HandleHTTPDataCnf   sgdd same with old sgdd\n"
#define MBBMS_SG_7661_112_2_17_23_9_41_518 "[SG]:HandleHTTPDataCnf  HTTP_CloseRequest error %d \n"
#define MBBMS_SG_7681_112_2_17_23_9_41_519 "[SG]:HandleStopSearchSGSig\n"
#define MBBMS_SG_7703_112_2_17_23_9_41_520 "[SG]:HandleStopSearchSGSig  HTTP_CloseRequest error %d \n"
#define MBBMS_SG_7737_112_2_17_23_9_41_521 "[SG]:HandleHTTPErrInd %d\n"
#define MBBMS_SG_7781_112_2_17_23_9_41_522 "[SG]:HandleHTTPDataCnf  HTTP_CloseRequest error %d \n"
#define MBBMS_SG_7850_112_2_17_23_9_41_523 "[SG]:SetPostData  s_sgdd_manual_search_flag %d \n "
#define MBBMS_SG_7874_112_2_17_23_9_41_524 "[SG]:SetPostData  no sgdd  \n "
#define MBBMS_SG_7884_112_2_17_23_9_41_525 "[SG]:SetPostData  broadcastaccess = %s\n "
#define MBBMS_SG_7895_112_2_17_23_9_41_526 "[SG]:SetPostData  mcc %d ,mnc %d,lac 0x%x, cell id 0x%x \n "
#define MBBMS_SG_7899_112_2_17_23_9_41_527 "[SG]:SetPostData  digit_1 %d ,digit_2 %d,digit_3 %d \n "
#define MBBMS_SG_7906_112_2_17_23_9_41_528 "[SG]:SetPostData  digit_1 %d ,digit_2 %d,digit_3 %d \n "
#define MBBMS_SG_7910_112_2_17_23_9_41_529 "[SG]:SetPostData  byte_1 %d ,byte_2 %d,byte_3 %d \n "
#define MBBMS_SG_7921_112_2_17_23_9_41_530 "[SG]:SetPostData  bsdcell = %s\n "
#define MBBMS_SG_7971_112_2_17_23_9_41_531 "[SG]:SetPostData  post data len %d\n "
#define MBBMS_SG_7972_112_2_17_23_9_41_532 "[SG]:SetPostData  post data  %s\n "
#define MBBMS_SG_7980_112_2_17_23_9_41_533 "[SG]:SetPostData  post data  end\n "
#define MBBMS_SG_8026_112_2_17_23_9_42_534 "[SG]:SetPostData  HTTP_PostRequest error  %d\n "
#define MBBMS_SG_8032_112_2_17_23_9_42_535 "[SG] SetPostData! end tick %d\n"
#define MBBMS_SG_8045_112_2_17_23_9_42_536 "[SG]:SendSignalToMMI  info.type %d\n "
#define MBBMS_SG_8113_112_2_17_23_9_42_537 "[SG] CreateMBBMS3SubFolder: create sub  dir success %c\n"
#define MBBMS_SG_8118_112_2_17_23_9_42_538 "[SG] CreateMBBMS3SubFolder: fail to create dir %c \n"
#define MBBMS_SG_8196_112_2_17_23_9_42_539 "[SG] CreateFolder: create dir success\n"
#define MBBMS_SG_8201_112_2_17_23_9_42_540 "[SG] CreateFolder: fail to create dir \n"
#define MBBMS_SG_8227_112_2_17_23_9_42_541 "[SG]:MBBMS_ManualSearchServiceGuide\n "
#define MBBMS_SG_8262_112_2_17_23_9_42_542 "[SG]:MBBMS_SearchServiceGuide\n "
#define MBBMS_SG_8274_112_2_17_23_9_42_543 "[SG]:MBBMS_SearchServiceGuide g_mbbms_task_id %d\n "
#define MBBMS_SG_8298_112_2_17_23_9_42_544 "[SG] MBBMSSG_SearchServiceGuide! start tick %d\n"
#define MBBMS_SG_8335_112_2_17_23_9_42_545 "[SG]:MBBMS_SearchServiceGuide\n "
#define MBBMS_SG_8346_112_2_17_23_9_42_546 "[SG]:MBBMS_SearchServiceGuide g_mbbms_task_id %d\n "
#define MBBMS_SG_8361_112_2_17_23_9_42_547 "[SG]:MBBMS_StopSearchingSG\n "
#define MBBMS_SG_8395_112_2_17_23_9_42_548 "[SG]:MBBMSSG_StopSearchingSG END \n"
#define MBBMS_SG_8406_112_2_17_23_9_42_549 "[SG]:MBBMS_SetSGURI %d[%s]\n "
#define MBBMS_SG_8467_112_2_17_23_9_42_550 "[SG]:MBBMS_GetSGDUFile\n "
#define MBBMS_SG_8519_112_2_17_23_9_43_551 "[SG]:MBBMS_GetSGDUBCASTFile\n "
#define MBBMS_SG_8550_112_2_17_23_9_43_552 "[SG]: MBBMSSG_GetAdFilm\n"
#define MBBMS_SG_8598_112_2_17_23_9_43_553 "[SG] MBBMSSG_GetAdFilm string_len:%d \n"
#define MBBMS_SG_8618_112_2_17_23_9_43_554 "[SG]:MBBMS_SGSetWap wap=%s,port 0x%x\n "
#define MBBMS_SG_8668_112_2_17_23_9_43_555 "[SG]:MBBMSSG_SetSGAreaName name_len 0x%x ,name_ptr 0x%x\n "
#define MBBMS_SG_8708_112_2_17_23_9_43_556 "[SG]:0x%x\n "
#define MBBMS_SG_8837_112_2_17_23_9_43_557 "[SG]:MBBMS_TestParse\n "
#define MBBMS_SG_9088_112_2_17_23_9_44_558 "[SG]: GetSgduHeader read header error"
#define MBBMS_SG_9107_112_2_17_23_9_44_559 "[SG]: GetSgduHeader read desp error"
#define MBBMS_SG_9142_112_2_17_23_9_44_560 "[SG]: WriteFileNameToIndexFile open fragment_idx_handle file error"
#define MBBMS_SG_9153_112_2_17_23_9_44_561 "[SG]: WriteFileNameToIndexFile write fragment index  error"
#define MBBMS_SG_9206_112_2_17_23_9_44_562 "[SG]: OpenIndexFileandOffset fragment_idx_handle error"
#define MBBMS_SG_9216_112_2_17_23_9_44_563 "[SG]: OpenIndexFileandOffset set pointer  error"
#define MBBMS_SG_9241_112_2_17_23_9_44_564 "[SG]: OpenIndexFileandOffset set pointer  error"
#define MBBMS_SG_9284_112_2_17_23_9_44_565 "[SG]: UpdateFragment enter tick %d"
#define MBBMS_SG_9299_112_2_17_23_9_44_566 "[SG]: UpdateFragment open sgdu file error"
#define MBBMS_SG_9306_112_2_17_23_9_44_567 "[SG]: UpdateFragment get sgdu file size error"
#define MBBMS_SG_9312_112_2_17_23_9_44_568 "[SG]: UpdateFragment get sgdu file size error"
#define MBBMS_SG_9350_112_2_17_23_9_44_569 "[SG]: UpdateFragment read_offset %d"
#define MBBMS_SG_9357_112_2_17_23_9_44_570 "[SG]: UpdateFragment fail to set pointer"
#define MBBMS_SG_9386_112_2_17_23_9_44_571 "[SG]: UpdateFragment read sgdu error"
#define MBBMS_SG_9393_112_2_17_23_9_44_572 "[SG]: UpdateFragment unknow type"
#define MBBMS_SG_9401_112_2_17_23_9_44_573 "[SG]: UpdateFragment  wrong despinfo_len"
#define MBBMS_SG_9417_112_2_17_23_9_44_574 "[SG]: UpdateFragment write desp info error"
#define MBBMS_SG_9438_112_2_17_23_9_44_575 "[SG]: UpdateFragment write fragment index error"
#define MBBMS_SG_9482_112_2_17_23_9_44_576 "[SG]: UpdateFragment end tick %d"
#define MBBMS_SG_9534_112_2_17_23_9_45_577 "[SG]: CreateNewFragmentFile open sgdu file error"
#define MBBMS_SG_9541_112_2_17_23_9_45_578 "[SG]: CreateNewFragmentFile get sgdu file size error"
#define MBBMS_SG_9547_112_2_17_23_9_45_579 "[SG]: CreateNewFragmentFile get sgdu file size error"
#define MBBMS_SG_9598_112_2_17_23_9_45_580 "[SG]: CreateNewFragmentFile write desp info error"
#define MBBMS_SG_9627_112_2_17_23_9_45_581 "[SG]: CreateNewFragmentFile read sgdu error"
#define MBBMS_SG_9634_112_2_17_23_9_45_582 "[SG]: CreateNewFragmentFile unknow type"
#define MBBMS_SG_9646_112_2_17_23_9_45_583 "[SG]: CreateNewFragmentFile open fragment_handle file error"
#define MBBMS_SG_9652_112_2_17_23_9_45_584 "[SG]: CreateNewFragmentFile  wrong despinfo_len"
#define MBBMS_SG_9666_112_2_17_23_9_45_585 "[SG]: CreateFragmentFile write desp info error"
#define MBBMS_SG_9825_112_2_17_23_9_45_586 "[SG]: WriteHeaderToIndexFile set pointer  error"
#define MBBMS_SG_9850_112_2_17_23_9_45_587 "[SG]: WriteHeaderToIndexFile write file  error"
#define MBBMS_SG_9903_112_2_17_23_9_45_588 "[SG]: DeleteAllPreviewDataFile \n"
#define MBBMS_SG_9976_112_2_17_23_9_45_589 "[SG]: DeleteALLFragmentFile sfs_err %d\n"
#define MBBMS_SG_10008_112_2_17_23_9_46_590 "[SG]: DeleteSGDUDataFile \n"
#define MBBMS_SG_10019_112_2_17_23_9_46_591 "[SG]: DeleteSGDUDataFile fail  to delete index file err %d"
#define MBBMS_SG_10032_112_2_17_23_9_46_592 "[SG]: DeleteSGDUDataFile fail  to delete sgdu file err %d"
#define MBBMS_SG_10070_112_2_17_23_9_46_593 "[SG]: CreateXMLSGDUFile  tick(1) %d"
#define MBBMS_SG_10089_112_2_17_23_9_46_594 "[SG]: CreateXMLSGDUFile open sgdu file error"
#define MBBMS_SG_10099_112_2_17_23_9_46_595 "[SG]: CreateXMLSGDUFile get sgdu file size error"
#define MBBMS_SG_10111_112_2_17_23_9_46_596 "[SG]: CreateXMLSGDUFile create bcast file error"
#define MBBMS_SG_10121_112_2_17_23_9_46_597 "[SG]: CreateXMLSGDUFile write desp begin tag error"
#define MBBMS_SG_10128_112_2_17_23_9_46_598 "[SG]: CreateXMLSGDUFile get header error"
#define MBBMS_SG_10144_112_2_17_23_9_46_599 "[SG]: CreateXMLSGDUFile sgdd error"
#define MBBMS_SG_10169_112_2_17_23_9_46_600 "[SG]: CreateXMLSGDUFile open index file error"
#define MBBMS_SG_10211_112_2_17_23_9_46_601 "[SG]: CreateXMLSGDUFile get desp info error"
#define MBBMS_SG_10221_112_2_17_23_9_46_602 "[SG]: CreateXMLSGDUFile get type error"
#define MBBMS_SG_10249_112_2_17_23_9_46_603 "[SG]: CreateXMLSGDUFile write fragment error"
#define MBBMS_SG_10264_112_2_17_23_9_46_604 "[SG]: CreateXMLSGDUFile write fragment index error"
#define MBBMS_SG_10272_112_2_17_23_9_46_605 "[SG]: CreateXMLSGDUFile unknow encoding type =%d"
#define MBBMS_SG_10291_112_2_17_23_9_46_606 "[SG]: CreateXMLSGDUFile write desp end tag error"
#define MBBMS_SG_10314_112_2_17_23_9_46_607 "[SG]: CreateXMLSGDUFile write header to index error"
#define MBBMS_SG_10329_112_2_17_23_9_46_608 "[SG]: CreateXMLSGDUFile end tick(2) %d"
#define MBBMS_SG_10359_112_2_17_23_9_46_609 "[SG]: CreateAllFragmentFile enter exist sgdd %d"
#define MBBMS_SG_10380_112_2_17_23_9_46_610 "[SG]: CreateAllFragmentFile open sgdu file error"
#define MBBMS_SG_10390_112_2_17_23_9_46_611 "[SG]: CreateAllFragmentFile get sgdu file size error"
#define MBBMS_SG_10399_112_2_17_23_9_46_612 "[SG]: CreateAllFragmentFile get sgdu file size error"
#define MBBMS_SG_10413_112_2_17_23_9_46_613 "[SG]: CreateAllFragmentFile sgdd error"
#define MBBMS_SG_10433_112_2_17_23_9_46_614 "[SG]: CreateAllFragmentFile open index file error"
#define MBBMS_SG_10487_112_2_17_23_9_47_615 "[SG]: CreateAllFragmentFile get desp info error"
#define MBBMS_SG_10503_112_2_17_23_9_47_616 "[SG]: CreateAllFragmentFile get type error"
#define MBBMS_SG_10517_112_2_17_23_9_47_617 "[SG]: CreateAllFragmentFile open fragment_handle file error"
#define MBBMS_SG_10523_112_2_17_23_9_47_618 "[SG]: CreateAllFragmentFile  wrong despinfo_len"
#define MBBMS_SG_10543_112_2_17_23_9_47_619 "[SG]: CreateAllFragmentFile write desp info error"
#define MBBMS_SG_10555_112_2_17_23_9_47_620 "[SG]: CreateAllFragmentFile write fragment index error"
#define MBBMS_SG_10570_112_2_17_23_9_47_621 "[SG]: CreateAllFragmentFile unknow encoding type =%d"
#define MBBMS_SG_10588_112_2_17_23_9_47_622 "[SG]: CreateAllFragmentFile write header to index error"
#define MBBMS_SG_10614_112_2_17_23_9_47_623 "[SG]: CreateAllFragmentFile end"
#define MBBMS_SG_10695_112_2_17_23_9_47_624 "[SG]: MBBMS_GetFileID fname len=%d"
#define MBBMS_SG_10717_112_2_17_23_9_47_625 "[SG]: MBBMS_GetFileID convert fname fail"
#define MBBMS_SG_10729_112_2_17_23_9_47_626 "[SG]: MBBMS_GetFileID fname invalid"
#define MBBMS_SG_10731_112_2_17_23_9_47_627 "[SG]: MBBMS_GetFileID file id =%d"
#define MBBMS_SG_10775_112_2_17_23_9_47_628 "[SG]: MBBMS_GetIdxFileData enter"
#define MBBMS_SG_10782_112_2_17_23_9_47_629 "[SG]: MBBMS_GetIdxFileData handle=%d"
#define MBBMS_SG_10789_112_2_17_23_9_47_630 "[SG]: MBBMS_GetIdxFileData read=%d sfs err=%d valid=%d"
#define MBBMS_SG_10794_112_2_17_23_9_47_631 "[SG]: MBBMS_GetIdxFileData data_len=%d"
#define MBBMS_SG_10799_112_2_17_23_9_47_632 "[SG]: MBBMS_GetIdxFileData ptr=0x%x"
#define MBBMS_SG_10811_112_2_17_23_9_47_633 "[SG]: MBBMS_GetIdxFileData get data error!"
#define MBBMS_SG_10821_112_2_17_23_9_47_634 "[SG]: MBBMS_GetIdxFileData SFS_GetFileSize error!"
#define MBBMS_SG_10827_112_2_17_23_9_47_635 "[SG]: MBBMS_GetIdxFileData data ptr=0x%x len=%d"
#define MBBMS_SG_10848_112_2_17_23_9_47_636 "[SG]: IsValidFragmentIndexFile enter"
#define MBBMS_SG_10851_112_2_17_23_9_47_637 "[SG]: IsValidFragmentIndexFile handle=%d"
#define MBBMS_SG_10866_112_2_17_23_9_47_638 "[SG]: IsValidFragmentIndexFile valid =%d"
#define MBBMS_SG_10885_112_2_17_23_9_47_639 "[SG]: MBBMSSG_GetSGDDId"
#define MBBMS_SG_10913_112_2_17_23_9_47_640 "[SG]: MBBMS_GetFullPath enter"
#define MBBMS_SG_10952_112_2_17_23_9_47_641 "[SG]: CheckHttpWord data_len =%d, data =%s"
#define MBBMS_SG_10998_112_2_17_23_9_48_642 "[SG]: CheckHttpWord string =%s ,data_len %d"
#define MBBMS_SG_11059_112_2_17_23_9_48_643 "[SG]: CheckHttpWord string =%s ,data_len %d"
#define MBBMS_SG_API_158_112_2_17_23_9_48_644 "[SGAPI]GetList: type = %d\n"
#define MBBMS_SG_API_224_112_2_17_23_9_48_645 "[SGAPI]GetList: list_ptr = 0x%x\n"
#define MBBMS_SG_API_257_112_2_17_23_9_48_646 "[SGAPI]:GetServiceForSearching condition=0x%x,serviceIdRef %s, len=%d n"
#define MBBMS_SG_API_307_112_2_17_23_9_48_647 "[SGAPI]:GetContentForSearching keywordCRC32 %d\n"
#define MBBMS_SG_API_341_112_2_17_23_9_48_648 "[SGAPI]GetServiceForSearching: rt_list_ptr = 0x%x, total_count %d\n"
#define MBBMS_SG_API_359_112_2_17_23_9_49_649 "[SGAPI]:SetSearchInfo  query = %d n"
#define MBBMS_SG_API_409_112_2_17_23_9_49_650 "[SGAPI]:ParseFragment type =%d \n"
#define MBBMS_SG_API_415_112_2_17_23_9_49_651 "[SGAPI]:ParseFragment parse error \n"
#define MBBMS_SG_API_427_112_2_17_23_9_49_652 "[SGAPI]:ParseFragment id =%s \n"
#define MBBMS_SG_API_440_112_2_17_23_9_49_653 "[SGAPI]:ParseFragment id =%s \n"
#define MBBMS_SG_API_496_112_2_17_23_9_49_654 "[SGAPI]:GetContentFragment   id %s,len=%d,pos=%d n"
#define MBBMS_SG_API_562_112_2_17_23_9_49_655 "[SGAPI]:IsSaveContent serviceIdRef %s, len=%d n"
#define MBBMS_SG_API_597_112_2_17_23_9_49_656 "[SGAPI]:IsSaveContent keyword is_find_keyword %d \n"
#define MBBMS_SG_API_625_112_2_17_23_9_49_657 "[SGAPI]:IsSaveContent name is_find_keyword %d \n"
#define MBBMS_SG_API_647_112_2_17_23_9_49_658 "[SGAPI]:IsSaveContent serviceIdRef %s, len=%d n"
#define MBBMS_SG_API_675_112_2_17_23_9_49_659 "[SGAPI]:IsSaveContent serviceIdRef %s, len=%d n"
#define MBBMS_SG_API_703_112_2_17_23_9_49_660 "[SGAPI]:CleanExtentionList  list_ptr = 0x%x n"
#define MBBMS_SG_API_709_112_2_17_23_9_49_661 "[SGAPI]:CleanExtentionList  extension_ptr = 0x%x n"
#define MBBMS_SG_API_745_112_2_17_23_9_49_662 "[SGAPI]:CleanPreviewDataList  list_ptr = 0x%x n"
#define MBBMS_SG_API_752_112_2_17_23_9_49_663 "[SGAPI]:CleanPreviewDataList  ref_ptr = 0x%x n"
#define MBBMS_SG_API_783_112_2_17_23_9_49_664 "[SGAPI]:FreePurchaseDataFragment  data_ptr = 0x%x n"
#define MBBMS_SG_API_821_112_2_17_23_9_49_665 "[SGAPI]:FreeOtherInfoOfPurchaseItemFragment  item_ptr = 0x%x n"
#define MBBMS_SG_API_849_112_2_17_23_9_49_666 "[SGAPI]:FreeOtherInfoOfContentFragment  content_ptr = 0x%x n"
#define MBBMS_SG_API_882_112_2_17_23_9_50_667 "[SGAPI]:FreeOtherInfoOfServiceFragment  service_ptr = 0x%x n"
#define MBBMS_SG_API_971_112_2_17_23_9_50_668 "[SGAPI]:GetComplexInfo  \n"
#define MBBMS_SG_API_1013_112_2_17_23_9_50_669 "[SGAPI]:GetContentDetail  \n"
#define MBBMS_SG_API_1088_112_2_17_23_9_50_670 "[SGAPI]:GetContentId  \n"
#define MBBMS_SG_API_1171_112_2_17_23_9_50_671 "[SGAPI]:SGAPISendSignalToMMI type%d, err_code = %d,is_stop %d ,sig_code %dn"
#define MBBMS_SG_API_1196_112_2_17_23_9_50_672 "[SGMEM]: FakeListCreate ptr=0X%x"
#define MBBMS_SG_API_1291_112_2_17_23_9_50_673 "[SGAPI]:GetQueryData  query = %d n"
#define MBBMS_SG_API_1322_112_2_17_23_9_50_674 "[SGAPI]:GetQueryData  service_list_ptr = 0x%x n"
#define MBBMS_SG_API_1332_112_2_17_23_9_50_675 "[SGAPI]:GetQueryData  content_list_list_ptr = 0x%x n"
#define MBBMS_SG_API_1405_112_2_17_23_9_51_676 "[SGAPI]:GetQueryData  content_list_ptr = 0x%x n"
#define MBBMS_SG_API_1452_112_2_17_23_9_51_677 "[SGAPI]:GetQueryData  QUERY_COMPLEX_INFO  don't match time \n"
#define MBBMS_SG_API_1633_112_2_17_23_9_51_678 "[SGAPI]:GetPreviewDataByRefList  index %s, len=%d,usage=%d,range=%d n"
#define MBBMS_SG_API_1638_112_2_17_23_9_51_679 "[SGAPI]:GetPreviewDataByRefList  usage=%d n"
#define MBBMS_SG_API_1647_112_2_17_23_9_51_680 "[SGAPI]:GetPreviewDataByRefList  preview_data_list_ptr= 0x%x n"
#define MBBMS_SG_API_1895_112_2_17_23_9_52_681 "[SGAPI]:GetPreviewDatafromPurchaseItem  index %s, len=%d,usage=%d,range=%d n"
#define MBBMS_SG_API_1971_112_2_17_23_9_52_682 "[SGAPI]:FreeServiceFragment  fragment_ptr= 0x%x n"
#define MBBMS_SG_API_2000_112_2_17_23_9_52_683 "[SGAPI]:FreeContentFragment  fragment_ptr= 0x%x n"
#define MBBMS_SG_API_2033_112_2_17_23_9_52_684 "[SGAPI]:GetPreviewDatafromService  index %s, len=%d,usage=%d,range=%d n"
#define MBBMS_SG_API_2113_112_2_17_23_9_52_685 "[SGAPI]:GetPreviewDatafromContent  index %s, len=%d,usage=%d,range=%d n"
#define MBBMS_SG_API_2150_112_2_17_23_9_52_686 "[SGAPI]:GetQueryData  service_list_ptr = 0x%x n"
#define MBBMS_SG_API_2168_112_2_17_23_9_52_687 "[SGAPI]:GetQueryData  content_list_list_ptr = 0x%x n"
#define MBBMS_SG_API_2185_112_2_17_23_9_52_688 "[SGAPI]:GetQueryData  content_list_ptr = 0x%x n"
#define MBBMS_SG_API_2265_112_2_17_23_9_52_689 "[SGAPI]:MBBMSSG_FreePreviewData  picture_ptr = 0x%x n"
#define MBBMS_SG_API_2290_112_2_17_23_9_52_690 "[SGAPI]:FreeOtherInfoOfPurchaseChannelFragment  channel_fragment_ptr = 0x%x n"
#define MBBMS_SG_API_2309_112_2_17_23_9_52_691 "[SGAPI]:FreeOtherInfoOfAccessFragment  access_fragment_ptr = 0x%x n"
#define MBBMS_SG_API_2353_112_2_17_23_9_52_692 "[SGAPI]:GetChannelInfoByChannelId  list_ptr = 0x%x n"
#define MBBMS_SG_API_2363_112_2_17_23_9_52_693 "[SGAPI]:MBBMSSG_GetPurchaseItem  header_info.id %s, len=%d n"
#define MBBMS_SG_API_2458_112_2_17_23_9_53_694 "[SGAPI]:MBBMSSG_GetPurchaseItemIndex  list_ptr = 0x%x n"
#define MBBMS_SG_API_2549_112_2_17_23_9_53_695 "[SGAPI]:MBBMSSG_FreePurchaseItemIndex  itemIdList_head_ptr = 0x%x n"
#define MBBMS_SG_API_2599_112_2_17_23_9_53_696 "[SGAPI]:MBBMSSG_GetPurchaseItem  itemId %s, len=%d n"
#define MBBMS_SG_API_2654_112_2_17_23_9_53_697 "[SGAPI]:MBBMSSG_FreePurchaseItem  item_ptr = 0x%x n"
#define MBBMS_SG_API_2692_112_2_17_23_9_53_698 "[SGAPI]:MBBMSSG_GetPurchaseDetail  list_ptr = 0x%x n"
#define MBBMS_SG_API_2716_112_2_17_23_9_53_699 "[SGAPI]:MBBMSSG_GetPurchaseDetail  PurchaseChannelIdRefList = 0x%x n"
#define MBBMS_SG_API_2763_112_2_17_23_9_53_700 "[SGAPI]:MBBMSSG_FreePurchaseDetail  detailData_ptr = 0x%x n"
#define MBBMS_SG_API_2785_112_2_17_23_9_53_701 "[SGAPI]:MBBMSSG_FreePurchaseDetail clear one node \n"
#define MBBMS_SG_API_2813_112_2_17_23_9_53_702 "[SGAPI]:MBBMSSG_GetServiceIndex  serviceIdList_head_ptr = 0x%x n"
#define MBBMS_SG_API_2866_112_2_17_23_9_54_703 "[SGAPI]:MBBMSSG_GetServiceIndex  service_index_ptr->forfree = %d n"
#define MBBMS_SG_API_2890_112_2_17_23_9_54_704 "[SGAPI]:MBBMSSG_GetServiceIndex  serviceid %s  weight = %d n"
#define MBBMS_SG_API_2936_112_2_17_23_9_54_705 "[SGAPI]:MBBMSSG_GetServiceIndex  serviceid %s  weight = %d n"
#define MBBMS_SG_API_2949_112_2_17_23_9_54_706 "[SGAPI]:MBBMSSG_GetServiceIndex  sort by weight : serviceid %s  weight = %d n"
#define MBBMS_SG_API_2980_112_2_17_23_9_54_707 "[SGAPI]:MBBMSSG_FreeServiceIndex  serviceIdList_head_ptr = 0x%x n"
#define MBBMS_SG_API_3019_112_2_17_23_9_54_708 "[SGAPI]:MBBMSSG_GetService  serviceId %s, len=%d n"
#define MBBMS_SG_API_3096_112_2_17_23_9_54_709 "[SGAPI]:MBBMSSG_FreeServiceIndex  service_ptr = 0x%x n"
#define MBBMS_SG_API_3132_112_2_17_23_9_54_710 "[SGAPI]:MBBMSSG_GetContentIndex  serviceId %s, len=%d n"
#define MBBMS_SG_API_3168_112_2_17_23_9_54_711 "[SGAPI]:MBBMSSG_FreeContentIndex  contentIdList_head_ptr = 0x%x n"
#define MBBMS_SG_API_3183_112_2_17_23_9_54_712 "[SGAPI]:MBBMSSG_FreeContentIndex  clear one node \n"
#define MBBMS_SG_API_3215_112_2_17_23_9_54_713 "[SGAPI]:MBBMSSG_GetContent  contentId %s, len=%d n"
#define MBBMS_SG_API_3239_112_2_17_23_9_54_714 "[SGAPI]:MBBMSSG_FreeContent  content_ptr = 0x%x n"
#define MBBMS_SG_API_3286_112_2_17_23_9_54_715 "[SGAPI]:MBBMSSG_GetComplexInfo condition=0x%x,serviceIdRef %s, len=%d,is_stop %d n"
#define MBBMS_SG_API_3308_112_2_17_23_9_54_716 "[SGAPI]:MBBMSSG_GetComplexInfo is_stop=0x%x \n"
#define MBBMS_SG_API_3330_112_2_17_23_9_54_717 "[SGAPI]:MBBMSSG_FreeComplexInfo  complexDataList_head_ptr = 0x%x n"
#define MBBMS_SG_API_3352_112_2_17_23_9_54_718 "[SGAPI]:MBBMSSG_FreeComplexInfo  clear one node \n"
#define MBBMS_SG_API_3383_112_2_17_23_9_55_719 "[SGAPI]:MBBMSSG_GetPurchaseItemByService serviceIdRef %s, len=%d n"
#define MBBMS_SG_API_3407_112_2_17_23_9_55_720 "[SGAPI]:MBBMSSG_GetPurchaseItemByService  serviceid_ref_list_ptr = 0x%x \n"
#define MBBMS_SG_API_3510_112_2_17_23_9_55_721 "[SGAPI]:MBBMSSG_FreePurchaseItemByService  \n"
#define MBBMS_SG_API_3547_112_2_17_23_9_55_722 "[SGAPI]:MBBMSSG_FreePurchaseItemByService  \n"
#define MBBMS_SG_API_3585_112_2_17_23_9_55_723 "[SGAPI]:MBBMSSG_GetPurchaseItemByContent contentId %s, len=%d n"
#define MBBMS_SG_API_3610_112_2_17_23_9_55_724 "[SGAPI]:MBBMSSG_GetPurchaseItemByContent  contentid_ref_list_ptr = 0x%x n"
#define MBBMS_SG_API_3709_112_2_17_23_9_55_725 "[SGAPI]:MBBMSSG_GetPreviewdata  index %s, len=%d,usage=%d,range=%d n"
#define MBBMS_SG_API_3757_112_2_17_23_9_55_726 "[SGAPI]:MBBMSSG_FreePreviewData  data_ptr = 0x%x \n"
#define MBBMS_SG_API_3785_112_2_17_23_9_55_727 "[SGAPI]:MBBMSSG_GetGlobalServiceId  serviceId %s, len=%d n"
#define MBBMS_SG_API_3842_112_2_17_23_9_55_728 "[SGAPI]:MBBMSSG_GetServiceId  globalServiceId %s, len=%d n"
#define MBBMS_SG_API_3891_112_2_17_23_9_55_729 "[SGAPI]:MBBMSSG_GetGlobalContentId  contentId %s, len=%d n"
#define MBBMS_SG_API_3922_112_2_17_23_9_56_730 "[SGAPI]:MBBMSSG_GetContentId  globalContentId %s, len=%d n"
#define MBBMS_SG_API_3956_112_2_17_23_9_56_731 "[SGAPI]:MBBMSSG_GetGlobalPurchaseItemtId  itemId %s, len=%d n"
#define MBBMS_SG_API_4010_112_2_17_23_9_56_732 "[SGAPI]:MBBMSSG_GetPurchaseItemId  globalPurchaseItemId %s, len=%d n"
#define MBBMS_SG_API_4057_112_2_17_23_9_56_733 "[SGAPI]:MBBMSSG_FreeTypeAnyURI  uri_ptr 0x%x n"
#define MBBMS_SG_API_4094_112_2_17_23_9_56_734 "[SGAPI]: MBBMSSG_APITest  \n"
#define MBBMS_SG_API_4235_112_2_17_23_9_56_735 "[SGAPI]:MBBMSSG_GetAccess  serviceId %s, len=%d n"
#define MBBMS_SG_API_4266_112_2_17_23_9_56_736 "[SGAPI]:MBBMSSG_GetAccess  is_encrypted =%d n"
#define MBBMS_SG_API_4302_112_2_17_23_9_56_737 "[[SGAPI]:] MBBMSSG_IsPurchaseItemSubscribed itemId doesn't exist"
#define MBBMS_SG_API_4307_112_2_17_23_9_56_738 "[[SGAPI]:] MBBMSSG_IsPurchaseItemSubscribed itemId doesn't exist"
#define MBBMS_SG_API_4310_112_2_17_23_9_56_739 "[SGAPI]:] MBBMSSG_IsPurchaseItemSubscribed, itemId=%s"
#define MBBMS_SG_API_4317_112_2_17_23_9_56_740 "[SGAPI]:] MBBMSSG_IsPurchaseItemSubscribed, purchaseitem_id=%s, response_code=%d"
#define MBBMS_SG_API_4355_112_2_17_23_9_56_741 "[SGAPI]:MBBMSSG_GetAccess  serviceId %s, len=%d n"
#define MBBMS_SG_API_4402_112_2_17_23_9_56_742 "[SGAPI]:MBBMSSG_GetAccess  cmmb freqNO %d, serviceid=%d n"
#define MBBMS_SG_API_4436_112_2_17_23_9_57_743 "[SGAPI]:FreeKeyManagementSystemListofAccess  list_ptr 0x%x n"
#define MBBMS_SG_API_4479_112_2_17_23_9_57_744 "[SGAPI]:MBBMSSG_FreeAccess  access_ptr 0x%x\n"
#define MBBMS_SG_API_4510_112_2_17_23_9_57_745 "[SGAPI]:MBBMSSG_GetFreeStatusByServiceId  serviceId %s, len=%d n"
#define MBBMS_SG_API_4532_112_2_17_23_9_57_746 "[SGAPI]:MBBMSSG_GetAccess  is_free =%d \n"
#define MBBMS_SG_API_4566_112_2_17_23_9_57_747 "[SGAPI]:MBBMSSG_GetAccess  serviceId %s, len=%d n"
#define MBBMS_SG_API_4713_112_2_17_23_9_57_748 "[SG]: MBBMS_GetFirstBSMSelector"
#define MBBMS_SG_API_4742_112_2_17_23_9_57_749 "[SG]: MBBMS_GetFirstBSMSelector ,first_bsm_id %d"
#define MBBMS_SG_API_4763_112_2_17_23_9_57_750 "[SGAPI]:MBBMSSG_GetContentGenreIndex   \n"
#define MBBMS_SG_API_4770_112_2_17_23_9_57_751 "[SGAPI]:MBBMSSG_GetContentGenreIndex list_ptr 0x%x\n"
#define MBBMS_SG_INTERNAL_965_112_2_17_23_9_59_752 "[SGMEMPOOL]MBBMS_CreateMemory"
#define MBBMS_SG_INTERNAL_969_112_2_17_23_9_59_753 "[SGMEMPOOL]MBBMS_CreateMemory addr=0x%x"
#define MBBMS_SG_INTERNAL_978_112_2_17_23_9_59_754 "[SGMEMPOOL]MBBMS_CreateMemory handle=0x%x"
#define MBBMS_SG_INTERNAL_981_112_2_17_23_9_59_755 "[SGMEMPOOL]MBBMS_CreateMemory Create Mem handle Fail!"
#define MBBMS_SG_INTERNAL_989_112_2_17_23_9_59_756 "[SGMEMPOOL]MBBMS_CreateMemory alloc mem failed!"
#define MBBMS_SG_INTERNAL_995_112_2_17_23_9_59_757 "[SGMEMPOOL]MBBMS_CreateMemory Memory Handle Has been Created!"
#define MBBMS_SG_INTERNAL_1007_112_2_17_23_9_59_758 "[SGMEMPOOL]MBBMS_DestroyMemory ptr=0x%x handle=0x%x"
#define MBBMS_SG_INTERNAL_1047_112_2_17_23_9_59_759 "[SGMEMPOOL]mbbms_Malloc:s_mbbms_pool_handle INVALID!"
#define MBBMS_SG_INTERNAL_1065_112_2_17_23_9_59_760 "[SGMEMPOOL]mbbms_Free:s_mbbms_pool_handle INVALID!"
#define MBBMS_SG_INTERNAL_1095_112_2_17_23_9_59_761 "[SGMEMPOOL]mbbms_PoolAlloc:s_mbbms_pool_handle INVALID!"
#define MBBMS_SG_INTERNAL_1117_112_2_17_23_9_59_762 "[SGMEMPOOL]mbbms_PoolFree:s_mbbms_pool_handle INVALID!"
#define MBBMS_SGDU_PARSE_3596_112_2_17_23_10_7_763 "[SG]: BuildContentGenreIndex genre ptr is NULL"
#define MBBMS_SGDU_PARSE_4584_112_2_17_23_10_9_764 "[SG]: SG_ParseFile fragment type not match org type %d  cur type %d"
#define MBBMS_SGDU_PARSE_5177_112_2_17_23_10_10_765 "[SG]: SG_GetIdxList SG IDX not exit"
#define MBBMS_SGDU_PARSE_5316_112_2_17_23_10_10_766 "[SG]: SG_ParseFragmentIdx enter %d"
#define MBBMS_SGDU_PARSE_5320_112_2_17_23_10_10_767 "[SG]: SG_ParseFragmentIdx alread parsed"
#define MBBMS_SGDU_PARSE_5333_112_2_17_23_10_10_768 "[SG]: SG_ParseFragmentIdx parse service"
#define MBBMS_SGDU_PARSE_5343_112_2_17_23_10_10_769 "[SG]: SG_ParseFragmentIdx parse access"
#define MBBMS_SGDU_PARSE_5365_112_2_17_23_10_10_770 "[SG]: SG_ParseFragmentIdx parse access"
#define MBBMS_SGDU_PARSE_5374_112_2_17_23_10_10_771 "[SG]: SG_ParseFragmentIdx parse purchase item"
#define MBBMS_SGDU_PARSE_5382_112_2_17_23_10_10_772 "[SG]: SG_ParseFragmentIdx parse purchase data"
#define MBBMS_SGDU_PARSE_5390_112_2_17_23_10_10_773 "[SG]: SG_ParseFragmentIdx parse purchase channel"
#define MBBMS_SGDU_PARSE_5398_112_2_17_23_10_10_774 "[SG]: SG_ParseFragmentIdx parse schedule"
#define MBBMS_SGDU_PARSE_5406_112_2_17_23_10_10_775 "[SG]: SG_ParseFragmentIdx parse content"
#define MBBMS_SGDU_PARSE_5414_112_2_17_23_10_11_776 "[SG]: SG_ParseFragmentIdx parse preview data"
#define MBBMS_SGDU_PARSE_5422_112_2_17_23_10_11_777 "[SG]: SG_ParseFragmentIdx parse interactive"
#define MBBMS_SGDU_PARSE_5456_112_2_17_23_10_11_778 "[SG]: SG_ParseFragmentIdx exit ret=%d  count=%d"
#define MBBMS_SUB_239_112_2_17_23_10_12_779 "MBBMS_Free_Service_Request_Ptr ... \n"
#define MBBMS_SUB_254_112_2_17_23_10_12_780 "MBBMS_Free_Service_Request_Ptr END \n"
#define MBBMS_SUB_273_112_2_17_23_10_12_781 "MBBMS_Free_Service_Request_Res_Ptr ...\n"
#define MBBMS_SUB_299_112_2_17_23_10_12_782 "MBBMS_Free_Service_Request_Res_Ptr END \n"
#define MBBMS_SUB_321_112_2_17_23_10_12_783 "MBBMS_Free_SubUpdate_Request_Ptr ... \n"
#define MBBMS_SUB_347_112_2_17_23_10_12_784 "MBBMS_Free_SubUpdate_Request_Ptr END \n"
#define MBBMS_SUB_368_112_2_17_23_10_12_785 "MBBMS_Free_Subupdate_Res_Ptr ... \n"
#define MBBMS_SUB_387_112_2_17_23_10_12_786 "MBBMS_Free_Subupdate_Res_Ptr END \n"
#define MBBMS_SUB_405_112_2_17_23_10_12_787 "MBBMS_Free_Account_Query_Ptr ... \n"
#define MBBMS_SUB_408_112_2_17_23_10_12_788 "MBBMS_Free_Account_Query_Ptr END \n"
#define MBBMS_SUB_430_112_2_17_23_10_12_789 "MBBMS_Free_Account_Query_Res_Ptr ... \n"
#define MBBMS_SUB_461_112_2_17_23_10_12_790 "MBBMS_Free_Account_Query_Res_Ptr END \n"
#define MBBMS_SUB_571_112_2_17_23_10_12_791 "CREATE_MBBMS_FOLDER Error \n"
#define MBBMS_SUB_576_112_2_17_23_10_12_792 "CREATE_MBBMS_FOLDER Error \n"
#define MBBMS_SUB_669_112_2_17_23_10_12_793 "MBBMS GetMSKIDFromMIKEY MSKID = 0x%x MTKID = 0x%x DOMAINID = 0x%x"
#define MBBMS_SUB_680_112_2_17_23_10_12_794 "MBBMS GetMTKIDFromMIKEY MIKEY_Open failed"
#define MBBMS_SUB_695_112_2_17_23_10_13_795 "MBBMS Print:%s \n"
#define MBBMS_SUB_699_112_2_17_23_10_13_796 "MBBMS Print:%s \n"
#define MBBMS_SUB_807_112_2_17_23_10_13_797 "MBBMS_Free_Init_Param...\n"
#define MBBMS_SUB_819_112_2_17_23_10_13_798 "MBBMS_Free_Init_Param END \n"
#define MBBMS_SUB_828_112_2_17_23_10_13_799 "MBBMS_Free_Auth_Param ....\n"
#define MBBMS_SUB_838_112_2_17_23_10_13_800 "MBBMS_Free_Auth_Param END \n"
#define MBBMS_SUB_847_112_2_17_23_10_13_801 "MBBMS_Free_Post_Param...\n"
#define MBBMS_SUB_884_112_2_17_23_10_13_802 "MBBMS_Free_Post_Param END \n"
#define MBBMS_SUB_894_112_2_17_23_10_13_803 "MBBMS_Free_Get_Param ... \n"
#define MBBMS_SUB_927_112_2_17_23_10_13_804 "MBBMS_Free_Get_Param END \n"
#define MBBMS_SUB_960_112_2_17_23_10_13_805 "#ERROR# mbbms HTTP_CloseRequest return ERROR! \n"
#define MBBMS_SUB_969_112_2_17_23_10_13_806 "MBBMS MSK SUB sendMMI Result:%d"
#define MBBMS_SUB_996_112_2_17_23_10_13_807 "MBBMS MSK ONLY Save SUB sendMMI Result:%d"
#define MBBMS_SUB_1043_112_2_17_23_10_13_808 "MBBMS_Sub_MSK_Init done \n"
#define MBBMS_SUB_1063_112_2_17_23_10_13_809 "MBBMS_Sub_MSK_Exit entering \n"
#define MBBMS_SUB_1082_112_2_17_23_10_13_810 "MBBMS_Sub_MSK_Exit Leaving ... \n"
#define MBBMS_SUB_1117_112_2_17_23_10_13_811 "MBBMS_MSK_SUB_Stop entering...\n"
#define MBBMS_SUB_1129_112_2_17_23_10_13_812 "MBBMS_MSK_SUB_Stop leaving... \n"
#define MBBMS_SUB_1152_112_2_17_23_10_13_813 "MBBMS_Save_Msk \"#Error mikey too long:%d \n"
#define MBBMS_SUB_1158_112_2_17_23_10_13_814 "MBBMS_Save_Msk Entering ...msk_id: %x key_domain_id:%x. n"
#define MBBMS_SUB_1206_112_2_17_23_10_14_815 "mbbms_put_msk msk:%d --0x%x \n"
#define MBBMS_SUB_1232_112_2_17_23_10_14_816 "MBBMS_Put_Msk mua SET OK.  \n"
#define MBBMS_SUB_1238_112_2_17_23_10_14_817 "MBBMS MSK Output mikey:0x%x %x %x %x %x %x %x %x n"
#define MBBMS_SUB_1244_112_2_17_23_10_14_818 "MBBMS_Put_Msk mua error \n"
#define MBBMS_SUB_1248_112_2_17_23_10_14_819 "MBBMS_PUT_MSK OK \n"
#define MBBMS_SUB_1288_112_2_17_23_10_14_820 "MBBMS_Set_MSK: GBA TEST MODE: return TRUE anyway \n"
#define MBBMS_SUB_1328_112_2_17_23_10_14_821 "MBBMS_Operation... type:%d \n"
#define MBBMS_SUB_1349_112_2_17_23_10_14_822 "MBBMS_Operation leaving ... \n"
#define MBBMS_SUB_1363_112_2_17_23_10_14_823 "MBBMS_Operation_Status ... %d \n"
#define MBBMS_SUB_1402_112_2_17_23_10_14_824 "MBBMS_Service_Request ...valid:%d global_id:%s n"
#define MBBMS_SUB_1417_112_2_17_23_10_14_825 "MBBMS_Service_Request leaving... \n"
#define MBBMS_SUB_1456_112_2_17_23_10_14_826 "MBBMS_Unsubscribe_Request ... \n"
#define MBBMS_SUB_1471_112_2_17_23_10_14_827 "MBBMS_Unsubscribe_Request leaving.. \n"
#define MBBMS_SUB_1514_112_2_17_23_10_14_828 "MBBMS_Subscription_Update ...\n"
#define MBBMS_SUB_1523_112_2_17_23_10_14_829 "MBBMS_Subscription_Update leaving \n"
#define MBBMS_SUB_1560_112_2_17_23_10_14_830 "MBBMS_Account_Inquiry %d... \n"
#define MBBMS_SUB_1569_112_2_17_23_10_14_831 "MBBMS_Account_Inquiry leaving... \n"
#define MBBMS_SUB_1655_112_2_17_23_10_14_832 "MBBMS mystristr: [%s]  [%s] \n"
#define MBBMS_SUB_1681_112_2_17_23_10_14_833 "MBBMS_SUB mbbms_sub_parse_item: item:%s value:%s \n"
#define MBBMS_SUB_1713_112_2_17_23_10_15_834 "mbbms_Check_Stale_Truth  is TRUE \n"
#define MBBMS_SUB_1716_112_2_17_23_10_15_835 "mbbms_Check_Stale_Truth  is FALSE \n"
#define MBBMS_SUB_1735_112_2_17_23_10_15_836 "MBBMS ADD LTKM Entering.... \n"
#define MBBMS_SUB_1743_112_2_17_23_10_15_837 "MBBMS MSK LEN: %d \n"
#define MBBMS_SUB_1748_112_2_17_23_10_15_838 "MBBMS MSK Input mikey:0x%x %x %x %x %x %x %x %x n"
#define MBBMS_SUB_1758_112_2_17_23_10_15_839 "MBBMS MSK SET OK.  \n"
#define MBBMS_SUB_1764_112_2_17_23_10_15_840 "MBBMS MSK Output mikey:0x%x %x %x %x %x %x %x %x n"
#define MBBMS_SUB_1773_112_2_17_23_10_15_841 "MBBMS MSK MAY SET ACK \n"
#define MBBMS_SUB_1777_112_2_17_23_10_15_842 "MBBMS MSK HAVE ACK \n"
#define MBBMS_SUB_1802_112_2_17_23_10_15_843 "MBBMS MSK SET ERROR \n"
#define MBBMS_SUB_1805_112_2_17_23_10_15_844 "MBBMS ADD LTKM: add ltkm done \n"
#define MBBMS_SUB_1822_112_2_17_23_10_15_845 "MBBMS ADD LTKM Entering.... \n"
#define MBBMS_SUB_1833_112_2_17_23_10_15_846 "MBBMS MSK LEN: %d \n"
#define MBBMS_SUB_1838_112_2_17_23_10_15_847 "MBBMS MSK Input mikey:0x%x %x %x %x %x %x %x %x n"
#define MBBMS_SUB_1848_112_2_17_23_10_15_848 "MBBMS MSK SET OK.  \n"
#define MBBMS_SUB_1854_112_2_17_23_10_15_849 "MBBMS MSK Output mikey:0x%x %x %x %x %x %x %x %x n"
#define MBBMS_SUB_1863_112_2_17_23_10_15_850 "MBBMS MSK MAY SET ACK \n"
#define MBBMS_SUB_1867_112_2_17_23_10_15_851 "MBBMS MSK HAVE ACK \n"
#define MBBMS_SUB_1892_112_2_17_23_10_15_852 "MBBMS MSK SET ERROR \n"
#define MBBMS_SUB_1895_112_2_17_23_10_15_853 "MBBMS ADD LTKM: add ltkm done \n"
#define MBBMS_SUB_1989_112_2_17_23_10_15_854 "\n HTTP TEST no memory \n"
#define MBBMS_SUB_2027_112_2_17_23_10_15_855 "MBBMS MBBMS_SUB_Prepare_Ack_Request Get NAF Failed \n"
#define MBBMS_SUB_2029_112_2_17_23_10_15_856 "MBBMS MBBMS_SUB_Prepare_Ack_Request GET OK naf is %s \n"
#define MBBMS_SUB_2035_112_2_17_23_10_15_857 "MBBMS_SUB_Prepare_Ack_Request URI:%s \n"
#define MBBMS_SUB_2054_112_2_17_23_10_15_858 "MBBMS MSK SET HTTP ACK \n"
#define MBBMS_SUB_2062_112_2_17_23_10_15_859 "MBBMS MSK SET HTTP ACK: ltkmlen:%d ltkm:0x%x %x n"
#define MBBMS_SUB_2063_112_2_17_23_10_15_860 "MBBMS MSK SET HTTP ACK: base64 ltkm:%s \n"
#define MBBMS_SUB_2076_112_2_17_23_10_15_861 "MBBMS ACK  BODY: %s \n"
#define MBBMS_SUB_2080_112_2_17_23_10_15_862 "#ERROR# mbbms HTTP_PostRequest return ERROR!\n"
#define MBBMS_SUB_2145_112_2_17_23_10_15_863 "MBBMS_SUB_SERVICE_REQUEST request_id  VALID \n"
#define MBBMS_SUB_2151_112_2_17_23_10_15_864 "MBBMS_SUB_SERVICE_REQUEST request_id NOT VALID \n"
#define MBBMS_SUB_2168_112_2_17_23_10_15_865 "MBBMS_SUB_SERVICE_REQUEST Entering LOOP"
#define MBBMS_SUB_2173_112_2_17_23_10_15_866 "MBBMS_SUB_SERVICE_REQUEST: global_id:%s \n"
#define MBBMS_SUB_2181_112_2_17_23_10_15_867 "MBBMS_SUB_SERVICE_REQUEST: id_ref:%s \n"
#define MBBMS_SUB_2190_112_2_17_23_10_15_868 "MBBMS_SUB_SERVICE_REQUEST: price %f str:%s \n"
#define MBBMS_SUB_2198_112_2_17_23_10_16_869 "MBBMS_SUB_SERVICE_REQUEST: price SET TO SERO \n"
#define MBBMS_SUB_2210_112_2_17_23_10_16_870 "MBBMS_SUB_SERVICE_REQUEST: id_ref:%s \n"
#define MBBMS_SUB_2217_112_2_17_23_10_16_871 "MBBMS_SUB_SERVICE_REQUEST ADD ERROR IDREF \n"
#define MBBMS_SUB_2227_112_2_17_23_10_16_872 "MBBMS_SUB_SERVICE_REQUEST: price %f\n"
#define MBBMS_SUB_2235_112_2_17_23_10_16_873 "MBBMS_SUB_SERVICE_REQUEST: price SET TO ZERO \n"
#define MBBMS_SUB_2244_112_2_17_23_10_16_874 "MBBMS_SUB_SERVICE_REQUEST LOOP"
#define MBBMS_SUB_2272_112_2_17_23_10_16_875 "MBBMS_SUB_SERVICE_UNSUB Entering LOOP"
#define MBBMS_SUB_2280_112_2_17_23_10_16_876 "MBBMS_SUB_SERVICE_UNSUB  LOOP"
#define MBBMS_SUB_2318_112_2_17_23_10_16_877 "MBBMS Send 2-Step data, sub_state %d \n"
#define MBBMS_SUB_2320_112_2_17_23_10_16_878 "MBBMS XML: %s \n"
#define MBBMS_SUB_2336_112_2_17_23_10_16_879 "\n HTTP TEST no memory \n"
#define MBBMS_SUB_2399_112_2_17_23_10_16_880 "MBBMS HANDLE_MSK_SIG_INIT_CNF Get NAF Failed \n"
#define MBBMS_SUB_2401_112_2_17_23_10_16_881 "MBBMS HANDLE_MSK_SIG_INIT_CNF GET  SUB_UPDATE OK naf is %s \n"
#define MBBMS_SUB_2409_112_2_17_23_10_16_882 "MBBMS HANDLE_MSK_SIG_INIT_CNF Get NAF Failed \n"
#define MBBMS_SUB_2411_112_2_17_23_10_16_883 "MBBMS HANDLE_MSK_SIG_INIT_CNF GET OK naf is %s \n"
#define MBBMS_SUB_2429_112_2_17_23_10_16_884 "MBBMS_SUB_SERVICE_REQUEST request_id  VALID \n"
#define MBBMS_SUB_2437_112_2_17_23_10_16_885 "MBBMS_SUB_SERVICE_REQUEST request_id NOT VALID \n"
#define MBBMS_SUB_2444_112_2_17_23_10_16_886 "MBBMS_SUB_SERVICE_REQUEST valid:%d request_id:%d str:%s n"
#define MBBMS_SUB_2466_112_2_17_23_10_16_887 "\"# MBBMS User Agent NOT SET \n"
#define MBBMS_SUB_2476_112_2_17_23_10_16_888 "MBBMS_SUB_SERVICE_REQUEST Entering LOOP"
#define MBBMS_SUB_2481_112_2_17_23_10_16_889 "MBBMS_SUB_SERVICE_REQUEST: global_id:%s \n"
#define MBBMS_SUB_2489_112_2_17_23_10_16_890 "MBBMS_SUB_SERVICE_REQUEST: id_ref:%s \n"
#define MBBMS_SUB_2498_112_2_17_23_10_16_891 "MBBMS_SUB_SERVICE_REQUEST: price %f str:%s \n"
#define MBBMS_SUB_2506_112_2_17_23_10_16_892 "MBBMS_SUB_SERVICE_REQUEST: price SET TO SERO \n"
#define MBBMS_SUB_2517_112_2_17_23_10_16_893 "MBBMS_SUB_SERVICE_REQUEST: id_ref:%s \n"
#define MBBMS_SUB_2524_112_2_17_23_10_16_894 "MBBMS_SUB_SERVICE_REQUEST ADD ERROR IDREF \n"
#define MBBMS_SUB_2534_112_2_17_23_10_16_895 "MBBMS_SUB_SERVICE_REQUEST: price %f\n"
#define MBBMS_SUB_2542_112_2_17_23_10_16_896 "MBBMS_SUB_SERVICE_REQUEST: price SET TO ZERO \n"
#define MBBMS_SUB_2551_112_2_17_23_10_16_897 "MBBMS_SUB_SERVICE_REQUEST LOOP"
#define MBBMS_SUB_2587_112_2_17_23_10_16_898 "\"# MBBMS User Agent NOT SET \n"
#define MBBMS_SUB_2598_112_2_17_23_10_16_899 "MBBMS_SUB_SERVICE_UNSUB Entering LOOP\n"
#define MBBMS_SUB_2605_112_2_17_23_10_16_900 "MBBMS_SUB_SERVICE_UNSUB Copy OK \n"
#define MBBMS_SUB_2607_112_2_17_23_10_16_901 "MBBMS_SUB_SERVICE_UNSUB  LOOP \n"
#define MBBMS_SUB_2609_112_2_17_23_10_16_902 "MBBMS_SUB_SERVICE_UNSUB Leaving LOOP \n"
#define MBBMS_SUB_2657_112_2_17_23_10_16_903 "\"# MBBMS User Agent NOT SET \n"
#define MBBMS_SUB_2690_112_2_17_23_10_17_904 "\"#MBBMS GET BTID FAILED, Set to EMPTY \n"
#define MBBMS_SUB_2695_112_2_17_23_10_17_905 "\"#MBBMS GET BTID len:%d, string:%s \n"
#define MBBMS_SUB_2709_112_2_17_23_10_17_906 "\"# MBBMS User Agent NOT SET \n"
#define MBBMS_SUB_2722_112_2_17_23_10_17_907 "MBBMS_MSK_GET_MSK Send HTTP Request  \n"
#define MBBMS_SUB_2740_112_2_17_23_10_17_908 "\"# MBBMS User Agent NOT SET \n"
#define MBBMS_SUB_2755_112_2_17_23_10_17_909 "MBBMS MSK REQ keydomainid 0x%x %x %x msk_id:0x%x n"
#define MBBMS_SUB_2760_112_2_17_23_10_17_910 "MBBMS MSK REQ keydomainid base64: %s\n"
#define MBBMS_SUB_2769_112_2_17_23_10_17_911 "MBBMS MSK REQ msk id: %d base64: %s\n"
#define MBBMS_SUB_2786_112_2_17_23_10_17_912 "MBBMS IMSI:%s \n"
#define MBBMS_SUB_2793_112_2_17_23_10_17_913 "MBBMS CMMBSN:%s \n"
#define MBBMS_SUB_2813_112_2_17_23_10_17_914 "#ERROR# mbbms HTTP_PostRequest return ERROR!\n"
#define MBBMS_SUB_2815_112_2_17_23_10_17_915 "MBBMS SUB Send HTTP Request OK \n"
#define MBBMS_SUB_2830_112_2_17_23_10_17_916 "\n HTTP TEST no memory \n"
#define MBBMS_SUB_2873_112_2_17_23_10_17_917 "MBBMS HANDLE_MSK_SIG_INIT_CNF Get NAF Failed \n"
#define MBBMS_SUB_2875_112_2_17_23_10_17_918 "MBBMS HANDLE_MSK_SIG_INIT_CNF GET OK naf is %s \n"
#define MBBMS_SUB_2905_112_2_17_23_10_17_919 "#ERROR# mbbms HTTP_PostRequest return ERROR!\n"
#define MBBMS_SUB_2910_112_2_17_23_10_17_920 " MBBMS MSK SIG_HTTP_INIT_CNF:error sub state%d \n"
#define MBBMS_SUB_2913_112_2_17_23_10_17_921 "MBBMS SendHTTPRequest: sub_state:%d \n"
#define MBBMS_SUB_2914_112_2_17_23_10_17_922 "MBBMS SUB BODY: %s \n"
#define MBBMS_SUB_2916_112_2_17_23_10_17_923 "MBBMS SUB URI: %s \n"
#define MBBMS_SUB_2940_112_2_17_23_10_17_924 "[MBBMS MSK PARSE]username = %s \n"
#define MBBMS_SUB_2947_112_2_17_23_10_17_925 " MBBMS SUB DIGEST: MD5(%s) = %s \n"
#define MBBMS_SUB_2957_112_2_17_23_10_17_926 "MBBMS SUB DIGEST: MD5(\"POST:/:MD5(%s)\") = %s \n"
#define MBBMS_SUB_2968_112_2_17_23_10_17_927 " MBBMS SUB DIGEST: MD5(%s:%s:%s)=%s n"
#define MBBMS_SUB_2986_112_2_17_23_10_17_928 "MBBMS SUB DIGEST: final response = %s, set string:%s n"
#define MBBMS_SUB_3012_112_2_17_23_10_17_929 "[MBBMS SUB]ks_ext_naf base64:%s \n"
#define MBBMS_SUB_3014_112_2_17_23_10_17_930 "[MBBMS MSK PARSE]IMPI = %s \n"
#define MBBMS_SUB_3017_112_2_17_23_10_17_931 "[MBBMS MSK PARSE]IMPI = %s \n"
#define MBBMS_SUB_3021_112_2_17_23_10_17_932 "MBBMS_Generate_Http_Head naf:%s \n"
#define MBBMS_SUB_3025_112_2_17_23_10_17_933 "MBBMS_Generate_Http_Head MBBMS_SUB_Get_MRK len:%d 0x%x %x %x n"
#define MBBMS_SUB_3029_112_2_17_23_10_17_934 "[MBBMS SUB]ks_ext_naf base64:%s \n"
#define MBBMS_SUB_3034_112_2_17_23_10_17_935 "MBBMS_Generate_Http_Head MBBMS_SUB_Get_MRK FAILED \n"
#define MBBMS_SUB_3039_112_2_17_23_10_17_936 "[MBBMS MSK PARSE]nonce = %s \n"
#define MBBMS_SUB_3041_112_2_17_23_10_17_937 "[MBBMS MSK PARSE]realm = %s \n"
#define MBBMS_SUB_3043_112_2_17_23_10_17_938 "[MBBMS MSK PARSE]opaque = %s \n"
#define MBBMS_SUB_3046_112_2_17_23_10_17_939 "[MBBMS MSK PARSE]qop = %s \n"
#define MBBMS_SUB_3054_112_2_17_23_10_17_940 "MBBMS MSK CNONCE Generated: [%s] \n"
#define MBBMS_SUB_3062_112_2_17_23_10_17_941 "[MBBMS MSK PARSE]username = %s \n"
#define MBBMS_SUB_3069_112_2_17_23_10_17_942 " MBBMS SUB DIGEST: MD5(%s) = %s \n"
#define MBBMS_SUB_3079_112_2_17_23_10_17_943 "MBBMS SUB DIGEST: MD5(\"POST:/:MD5(%s)\") = %s \n"
#define MBBMS_SUB_3090_112_2_17_23_10_17_944 " MBBMS SUB DIGEST: MD5(%s:%s:%s)=%s n"
#define MBBMS_SUB_3108_112_2_17_23_10_17_945 "MBBMS SUB DIGEST: final response = %s, set string:%s n"
#define MBBMS_SUB_3147_112_2_17_23_10_17_946 "mbbms_sub_createfolder: create dir success\n"
#define MBBMS_SUB_3152_112_2_17_23_10_17_947 "mbbms_sub_createfolder: fail to create dir \n"
#define MBBMS_SUB_3158_112_2_17_23_10_17_948 "mbbms_sub_createfolder: dir is exist \n"
#define MBBMS_SUB_3194_112_2_17_23_10_18_949 "mbbms_sub_get_file: ok \n"
#define MBBMS_SUB_3223_112_2_17_23_10_18_950 "mbbms_check_isfileexist: ok \n"
#define MBBMS_SUB_3260_112_2_17_23_10_18_951 "MBBMS_Get_Local_IMSI_CMMBSN IMSI read=%d sfs err=%d n"
#define MBBMS_SUB_3264_112_2_17_23_10_18_952 "MBBMS_Get_Local_IMSI_CMMBSN IMSI len read fail \n"
#define MBBMS_SUB_3273_112_2_17_23_10_18_953 "MBBMS_Get_Local_IMSI_CMMBSN IMSI len read error \n"
#define MBBMS_SUB_3277_112_2_17_23_10_18_954 "MBBMS_Get_Local_IMSI_CMMBSN MD5(imsi)=%s \n"
#define MBBMS_SUB_3282_112_2_17_23_10_18_955 "MBBMS_Get_Local_IMSI_CMMBSN IMSI base64 too long \n"
#define MBBMS_SUB_3285_112_2_17_23_10_18_956 "MBBMS_Get_Local_IMSI_CMMBSN IMSI long:%d"
#define MBBMS_SUB_3305_112_2_17_23_10_18_957 "MBBMS_Get_Local_IMSI_CMMBSN CMMBSN read=%d len=%d sfs err=%d n"
#define MBBMS_SUB_3309_112_2_17_23_10_18_958 "MBBMS_Get_Local_IMSI_CMMBSN CMMBSN len read fail \n"
#define MBBMS_SUB_3317_112_2_17_23_10_18_959 "MBBMS_Get_Local_IMSI_CMMBSN CMMBSN read fail \n"
#define MBBMS_SUB_3321_112_2_17_23_10_18_960 "MBBMS_Get_Local_IMSI_CMMBSN MD5(cmmbsn)=%s \n"
#define MBBMS_SUB_3325_112_2_17_23_10_18_961 "MBBMS_Get_Local_IMSI_CMMBSN CMMBSN base64 too long \n"
#define MBBMS_SUB_3328_112_2_17_23_10_18_962 "MBBMS_Get_Local_IMSI_CMMBSN CMMBSN base64 len %d \n"
#define MBBMS_SUB_3361_112_2_17_23_10_18_963 "MBBMS_Save_Local_IMSI_CMMBSN entering.... \n"
#define MBBMS_SUB_3371_112_2_17_23_10_18_964 "MBBMS_Save_Local_IMSI_CMMBSN createFile OK \n"
#define MBBMS_SUB_3375_112_2_17_23_10_18_965 "MBBMS_Save_Local_IMSI_CMMBSN IMSI current is not exist. \n"
#define MBBMS_SUB_3382_112_2_17_23_10_18_966 "MBBMS_Save_Local_IMSI_CMMBSN begin imsi MD5 encode \n"
#define MBBMS_SUB_3387_112_2_17_23_10_18_967 "MBBMS_Get_Local_IMSI_CMMBSN IMSI base64 too long \n"
#define MBBMS_SUB_3390_112_2_17_23_10_18_968 "MBBMS_Save_Local_IMSI_CMMBSN MD5[imsi]=%s \n"
#define MBBMS_SUB_3398_112_2_17_23_10_18_969 "MBBMS_Save_Local_IMSI_CMMBSN begin save cmmbsn \n"
#define MBBMS_SUB_3402_112_2_17_23_10_18_970 "MBBMS_Save_Local_IMSI_CMMBSN get local cmmbsn fail \n"
#define MBBMS_SUB_3412_112_2_17_23_10_18_971 "MBBMS_Get_Local_IMSI_CMMBSN CMMBSN base64 too long \n"
#define MBBMS_SUB_3415_112_2_17_23_10_18_972 "MBBMS_Save_Local_IMSI_CMMBSN MD5[cmmbsn]=%s \n"
#define MBBMS_SUB_3429_112_2_17_23_10_18_973 "MBBMS_Save_Local_IMSI_CMMBSN return ret:%d\n"
#define MBBMS_SUB_3451_112_2_17_23_10_18_974 "MBBMS_Check_IMSI_UAM entering \n"
#define MBBMS_SUB_3455_112_2_17_23_10_18_975 "MBBMS_Check_IMSI_UAM Return FALSE, NO UAM found!! \n"
#define MBBMS_SUB_3461_112_2_17_23_10_18_976 "MBBMS_Check_IMSI_UAM get local imsi, cmmbsn fail \n"
#define MBBMS_SUB_3467_112_2_17_23_10_18_977 "MBBMS_Check_IMSI_UAM IMSI current is not exist. \n"
#define MBBMS_SUB_3473_112_2_17_23_10_18_978 "MBBMS_Check_IMSI_UAM IMSI length is not EQUAL. \n"
#define MBBMS_SUB_3480_112_2_17_23_10_18_979 "MBBMS_Check_IMSI_UAM IMSI is not EQUAL. \n"
#define MBBMS_SUB_3489_112_2_17_23_10_18_980 "MBBMS_Check_IMSI_UAM get local imsi, cmmbsn fail \n"
#define MBBMS_SUB_3495_112_2_17_23_10_18_981 "MBBMS_Check_IMSI_UAM cmmbsn len not equal \n"
#define MBBMS_SUB_3504_112_2_17_23_10_18_982 "MBBMS_Check_IMSI_UAM curcmmbsn is not equal cmmbsn  \n"
#define MBBMS_SUB_3513_112_2_17_23_10_18_983 "MBBMS_Check_IMSI_UAM GBA needed is TRUE \n"
#define MBBMS_SUB_3517_112_2_17_23_10_18_984 "MBBMS_Check_IMSI_UAM leaving %d \n"
#define MBBMS_SUB_3545_112_2_17_23_10_18_985 "MBBMS_Load_UpdateTable IMSI current is not exist. \n"
#define MBBMS_SUB_3552_112_2_17_23_10_18_986 "MBBMS_Load_UpdateTable get local imsi fail \n"
#define MBBMS_SUB_3558_112_2_17_23_10_18_987 "MBBMS_Load_UpdateTable IMSI length is not EQUAL. \n"
#define MBBMS_SUB_3566_112_2_17_23_10_18_988 "MBBMS_Load_UpdateTable IMSI is not EQUAL. \n"
#define MBBMS_SUB_3572_112_2_17_23_10_18_989 "MBBMS_Load_UpdateTable SUB_BuildPurchaseList Return TRUE! \n"
#define MBBMS_SUB_3574_112_2_17_23_10_18_990 "MBBMS_Load_UpdateTable SUB_BuildPurchaseList return FALSE! \n"
#define MBBMS_SUB_3601_112_2_17_23_10_18_991 " MBBMS MSK SUB Launch HTTP Failed for NO WAP Available! \n"
#define MBBMS_SUB_3614_112_2_17_23_10_18_992 "#ERROR# mbbms HTTP_InitRequest return ERROR!\n"
#define MBBMS_SUB_3635_112_2_17_23_10_18_993 "\"#MBBMS GBA HTTP INIT CNF ERROR!!\n"
#define MBBMS_SUB_3637_112_2_17_23_10_18_994 " MBBMS MSK SIG_HTTP_INIT_CNF:Entering... sub state%d \n"
#define MBBMS_SUB_3670_112_2_17_23_10_18_995 "MBBMS_HANDLE_MSK_SIG_NEED_AUTH_IND sub_post_param_ptr XML BODY TOO LONG \n"
#define MBBMS_SUB_3673_112_2_17_23_10_19_996 "MBBMS_HANDLE_MSK_SIG_NEED_AUTH_IND \"# XML BODY IS NULL \"# sub_post_param_ptr is NULL \n"
#define MBBMS_SUB_3676_112_2_17_23_10_19_997 "#ERROR# mbbms HTTP_CancelRequest return ERROR!\n"
#define MBBMS_SUB_3681_112_2_17_23_10_19_998 "MBBMS\"\"\"\"\"\"\"# NEED_AUTH FAILED ONCE MORE\"\"\n"
#define MBBMS_SUB_3735_112_2_17_23_10_19_999 "\n HTTP TEST no memory \n"
#define MBBMS_SUB_3781_112_2_17_23_10_19_1000 "MBBMS MSK CNONCE Generated: [%s] \n"
#define MBBMS_SUB_3818_112_2_17_23_10_19_1001 "MBBMS SUB Digest:[%s] \n"
#define MBBMS_SUB_3822_112_2_17_23_10_19_1002 "MBBMS HANDLE_MSK_SIG_INIT_CNF Get NAF Failed \n"
#define MBBMS_SUB_3824_112_2_17_23_10_19_1003 "MBBMS HANDLE_MSK_SIG_INIT_CNF GET OK naf is %s \n"
#define MBBMS_SUB_3839_112_2_17_23_10_19_1004 "MBBMS Send_Manual_Auth sub_xml_ptr TOO LONG %d \n"
#define MBBMS_SUB_3850_112_2_17_23_10_19_1005 "#ERROR# mbbms HTTP_PostRequest return ERROR!\n"
#define MBBMS_SUB_3882_112_2_17_23_10_19_1006 "MBBMS MSK SIG_DATA_IND Received Result:%d \n"
#define MBBMS_SUB_3884_112_2_17_23_10_19_1007 "MBBMS MSK Received Str:%s \n"
#define MBBMS_SUB_3896_112_2_17_23_10_19_1008 "[MBBMS SUB PARSE]OPERATION sub_state:%d HAS BEEN ACCEPTTED! rst:%d\n"
#define MBBMS_SUB_3904_112_2_17_23_10_19_1009 "MBBMS MBBMS_MTV_CANCEL OK, Clean Data \n"
#define MBBMS_SUB_3913_112_2_17_23_10_19_1010 "[MBBMS SUB PARSE]OPERATION LTKM len:%d dat:0x%x %x %x %x n"
#define MBBMS_SUB_3919_112_2_17_23_10_19_1011 "[MBBMS SUB PARSE]OPERATION ACK HAS BEEN SENT OUT \n"
#define MBBMS_SUB_3925_112_2_17_23_10_19_1012 "[MBBMS SUB PARSE]NOT NEED TO SEND ACK \n"
#define MBBMS_SUB_3933_112_2_17_23_10_19_1013 "MBBMS OPERATION DO NOT HAVE LTKM DATA \n"
#define MBBMS_SUB_3940_112_2_17_23_10_19_1014 "[MBBMS SUB PARSE]OPERATION sub_state:%d HAS BEEN Denied! rst:%d\n"
#define MBBMS_SUB_3952_112_2_17_23_10_19_1015 "[MBBMS SUB PARSE]OPERATION XML PARSE FAILED:%d \n"
#define MBBMS_SUB_3978_112_2_17_23_10_19_1016 "MBBMS GBLID:%s IS SUBSCRIBED \n"
#define MBBMS_SUB_3998_112_2_17_23_10_19_1017 "MBBMS SUB UPDATE: SUB_TABLE subscription done \n"
#define MBBMS_SUB_4003_112_2_17_23_10_19_1018 "MBBMS SUB UPDATE: SUB_TABLE subscription FAIL \n"
#define MBBMS_SUB_4010_112_2_17_23_10_19_1019 "MBBMS_SUB_SERVICE_REQUEST not need to send ACK. OK \n"
#define MBBMS_SUB_4030_112_2_17_23_10_19_1020 "MBBMS_SUB_SERVICE_REQUEST need to send ACK Now...\n"
#define MBBMS_SUB_4054_112_2_17_23_10_19_1021 "MBBMS SUB UPDATE: unsubscription XML Parse ok\n"
#define MBBMS_SUB_4065_112_2_17_23_10_19_1022 "MBBMS GBLID:%s STOP SUBSCRIBED \n"
#define MBBMS_SUB_4087_112_2_17_23_10_19_1023 "MBBMS SUB UPDATE: SUB_TABLE unsubscription done \n"
#define MBBMS_SUB_4092_112_2_17_23_10_19_1024 "MBBMS SUB UPDATE: SUB_TABLE unsubscription ok, BUT other area \n"
#define MBBMS_SUB_4099_112_2_17_23_10_19_1025 "MBBMS_SUB_SERVICE_UNSUB not need to send ACK. OK \n"
#define MBBMS_SUB_4119_112_2_17_23_10_19_1026 "MBBMS_SUB_SERVICE_UNSUB need to send ACK Now...\n"
#define MBBMS_SUB_4141_112_2_17_23_10_19_1027 "MBBMS ACCOUNT QUIRY PARSE OK \n"
#define MBBMS_SUB_4145_112_2_17_23_10_19_1028 "MBBMS Account Quiry Refresh DONE OK \n"
#define MBBMS_SUB_4183_112_2_17_23_10_20_1029 "MBBMS SUB MSK Find activationstatus:%d\n"
#define MBBMS_SUB_4202_112_2_17_23_10_20_1030 "MBBMS_SUB_UPDATEParsing: %s resp_code:%d n"
#define MBBMS_SUB_4203_112_2_17_23_10_20_1031 "   MBBMS_SUB_UPDATE first MSKID: %d \n"
#define MBBMS_SUB_4214_112_2_17_23_10_20_1032 "MBBMS SUB UPDATE: CREATE SUB_TABLE OK \n"
#define MBBMS_SUB_4219_112_2_17_23_10_20_1033 "MBBMS SUB  UPDATE: Create SUB_TABLE FAILED\n"
#define MBBMS_SUB_4239_112_2_17_23_10_20_1034 " MBBMS_SUB_UPDATE FINISHED NOW! \n"
#define MBBMS_SUB_4251_112_2_17_23_10_20_1035 "MBBMS_MSK_GET_MSK status:%d \n"
#define MBBMS_SUB_4264_112_2_17_23_10_20_1036 "MBBMS_MSK_GET_MSK NOT NEED ACK>Send MMI OK\n"
#define MBBMS_SUB_4272_112_2_17_23_10_20_1037 "MBBMS_MSK_GET_MSK need to send ACK Now...\n"
#define MBBMS_SUB_4277_112_2_17_23_10_20_1038 "MBBMS MBBMS_MSK_GET_MSK PARSE FAILED \n"
#define MBBMS_SUB_4299_112_2_17_23_10_20_1039 "MBBMS MSK SIG_HEADER_IND Received  result:%d \n"
#define MBBMS_SUB_4301_112_2_17_23_10_20_1040 "MBBMS MSK SIG_HEADER_IND Received  string:%s \n"
#define MBBMS_SUB_4309_112_2_17_23_10_20_1041 "MBBMS MSK_SIG_HEADER_IND Header Failed \n"
#define MBBMS_SUB_4318_112_2_17_23_10_20_1042 "MBBMS Authenticate & Stale Exist. Re-Send Digest IS Valid \n"
#define MBBMS_SUB_4323_112_2_17_23_10_20_1043 "MBBMS Authenticate Header NOT Exist \n"
#define MBBMS_SUB_4337_112_2_17_23_10_20_1044 "[MBBMS SUB][Save HEAD]:%s \n"
#define MBBMS_SUB_4347_112_2_17_23_10_20_1045 "[MBBMS OPERATION DIGEST] FAILED! !\n"
#define MBBMS_SUB_4352_112_2_17_23_10_20_1046 "MBBMS OPERATION ACK FAILED \n"
#define MBBMS_SUB_4362_112_2_17_23_10_20_1047 "MBBMS Authenticate & Stale Exist. Re-Send Digest IS Valid \n"
#define MBBMS_SUB_4367_112_2_17_23_10_20_1048 "MBBMS Authenticate Header NOT Exist \n"
#define MBBMS_SUB_4378_112_2_17_23_10_20_1049 "[MBBMS SUB][Save HEAD]:%s \n"
#define MBBMS_SUB_4392_112_2_17_23_10_20_1050 "MBBMS SUB MSK ACK FAILED \n"
#define MBBMS_SUB_4405_112_2_17_23_10_20_1051 "MBBMS Authenticate & Stale Exist. Re-Send Digest IS Valid \n"
#define MBBMS_SUB_4410_112_2_17_23_10_20_1052 "MBBMS Authenticate Header NOT Exist \n"
#define MBBMS_SUB_4421_112_2_17_23_10_20_1053 "[MBBMS SUB][Save HEAD]:%s \n"
#define MBBMS_SUB_4434_112_2_17_23_10_20_1054 "MBBMS UNSUB MSK ACK FAILED \n"
#define MBBMS_SUB_4460_112_2_17_23_10_20_1055 "MBBMS Authenticate & Stale Exist. Re-Send Digest IS Valid \n"
#define MBBMS_SUB_4465_112_2_17_23_10_20_1056 "MBBMS Authenticate Header NOT Exist \n"
#define MBBMS_SUB_4475_112_2_17_23_10_20_1057 "MBBMS SUB: Save the HTTP Head for authentication \n"
#define MBBMS_SUB_4486_112_2_17_23_10_20_1058 "MBBMS SUB MBBMS_MSK_GET_MSK: AUTH Failed \n"
#define MBBMS_SUB_4504_112_2_17_23_10_20_1059 "MBBMS Authentication %d OK, and with Authentication Also \n"
#define MBBMS_SUB_4512_112_2_17_23_10_20_1060 "MBBMS Operation substate:%d ACK OK \n"
#define MBBMS_SUB_4535_112_2_17_23_10_20_1061 "MBBMS_SUB_SERVICE_REQUEST HTTP ACK SUCCESSFUL Received \n"
#define MBBMS_SUB_4544_112_2_17_23_10_20_1062 "MBBMS_SUB_SERVICE_UNSUB HTTP ACK SUCCESSFUL Received \n"
#define MBBMS_SUB_4570_112_2_17_23_10_20_1063 "MBBMS_MSK_SUB_Thread sub_xml_ptr addr 0x%x \n"
#define MBBMS_SUB_4571_112_2_17_23_10_20_1064 "MBBMS_MSK_SUB_Thread sub_head_ptr addr 0x%x \n"
#define MBBMS_SUB_4576_112_2_17_23_10_20_1065 "MBBMS_MSK_SUB_THREAD One signal received %d\n"
#define MBBMS_SUB_4587_112_2_17_23_10_20_1066 "MBBMS SUB THREAD: MBBMS_Account_Inquiry_t type:%d \n"
#define MBBMS_SUB_4592_112_2_17_23_10_20_1067 "MBBMS_MSK_INNER_SIGNAL Begin to Launch http \n"
#define MBBMS_SUB_4598_112_2_17_23_10_20_1068 "MBBMS_MSK_INNER_SIGNAL Directly Send HTTP Request\n"
#define MBBMS_SUB_4606_112_2_17_23_10_20_1069 "MBBMS_MSK_SUB_Stop IS STOPPING THE PRCESS :%d \n"
#define MBBMS_SUB_4632_112_2_17_23_10_20_1070 "#ERROR# mbbms HTTP_CloseRequest return ERROR! \n"
#define MBBMS_SUB_4643_112_2_17_23_10_20_1071 "MBBMS_MSK_INNER_EXIT signal received \n"
#define MBBMS_SUB_4653_112_2_17_23_10_21_1072 "[MBBMS_MSK] HTT_SIG_INIT_CNF received\n"
#define MBBMS_SUB_4666_112_2_17_23_10_21_1073 "[MBBMS_MSK] HTTP_SIG_NEED_AUTH_IND received\n"
#define MBBMS_SUB_4672_112_2_17_23_10_21_1074 "[MBBMS_MSK] HTTP_SIG_REQUEST_ID_IND received\n"
#define MBBMS_SUB_4678_112_2_17_23_10_21_1075 "[MBBMS_MSK] HTTP_SIG_GET_CNF received\n"
#define MBBMS_SUB_4684_112_2_17_23_10_21_1076 "[MBBMS_MSK] HTTP_SIG_HEAD_CNF received\n"
#define MBBMS_SUB_4690_112_2_17_23_10_21_1077 "[MBBMS_MSK] HTTP_SIG_DATA_IND received\n"
#define MBBMS_SUB_4696_112_2_17_23_10_21_1078 "[MBBMS_MSK] HTTP_SIG_HEADER_IND received\n"
#define MBBMS_SUB_4702_112_2_17_23_10_21_1079 "[MBBMS_MSK] HTTP_SIG_ERROR_IND received\n"
#define MBBMS_SUB_4712_112_2_17_23_10_21_1080 "[MBBMS_MSK] HTTP_SIG_CLOSE_CNF received\n"
#define MBBMS_SUB_4720_112_2_17_23_10_21_1081 "MBBMS MSK SUB CHANGE sendMMI Result:%d"
#define MBBMS_SUB_4725_112_2_17_23_10_21_1082 "MBBMS MSK SUB sendMMI by subcallback Result:%d"
#define MBBMS_SUB_4728_112_2_17_23_10_21_1083 "MBBMS MSK SUB sendMMI by subcallback ok\n"
#define MBBMS_SUB_4744_112_2_17_23_10_21_1084 "[MBBMS_MSK] HTTP_SIG_CANCEL_CNF received\n"
#define MBBMS_SUB_4754_112_2_17_23_10_21_1085 " [MBBMS-MSK] HTTP_SIG_NET_PROG_IND received \n"
#define MBBMS_SUB_4757_112_2_17_23_10_21_1086 " [MBBMS-MSK] HTTP_SIG_REDIRECT_IND received \n"
#define MBBMS_SUB_4770_112_2_17_23_10_21_1087 "MBBMS SUB THREAD: Signal Freeing..... \n"
#define MBBMS_SUB_4772_112_2_17_23_10_21_1088 "MBBMS SUB THREAD: Signal Free OK \n"
#define MBBMS_SUB_4789_112_2_17_23_10_21_1089 "MBBMS SUB Free sub_head_ptr \n"
#define MBBMS_SUB_4792_112_2_17_23_10_21_1090 "MBBMS SUB Free sub_xml_ptr \n"
#define MBBMS_SUB_4796_112_2_17_23_10_21_1091 "MBBMS SUB Free http_msk_ack_data \n"
#define MBBMS_SUB_4799_112_2_17_23_10_21_1092 "MBBMS SUB FREE OVER, THREAD EXIT \n"
#define MBBMS_SUB_4839_112_2_17_23_10_21_1093 "MBBMS_SUB_Get_MRK WE HAVE ks_ext_naf IN GBA-MRK \n"
#define MBBMS_SUB_4851_112_2_17_23_10_21_1094 "MBBMS_SUB_Get_MRK impi:%s naf:%s \n"
#define MBBMS_SUB_4861_112_2_17_23_10_21_1095 "MBBMS_SUB_Get_MRK naf domain name:%s impi:%s \n"
#define MBBMS_SUB_4883_112_2_17_23_10_21_1096 "MBBMS_SUB_Get_MRK SD CARD OK len:%d 0x%x %x %x %xn"
#define MBBMS_SUB_4890_112_2_17_23_10_21_1097 "MBBMS MRK Result:len:%d ks_ext_naf:0x%2x %2x %2x %2x n"
#define MBBMS_SUB_PARSE_686_112_2_17_23_10_22_1098 "mbbms subBase64binaryToString base64_decode error!\n"
#define MBBMS_SUB_PARSE_722_112_2_17_23_10_22_1099 "mbbms subBase64binaryToUint32 base64_decode error!\n"
#define MBBMS_SUB_PARSE_777_112_2_17_23_10_23_1100 "mbbms subGetUpdateRespTypeTailData ENTER \n"
#define MBBMS_SUB_PARSE_783_112_2_17_23_10_23_1101 "mbbms subGetUpdateRespTypeTailData OVER \n"
#define MBBMS_SUB_PARSE_1009_112_2_17_23_10_23_1102 "[SG]: subServiceResponseSyntaxHandler ServiceResponse"
#define MBBMS_SUB_PARSE_1015_112_2_17_23_10_23_1103 "[SG]: subServiceResponseSyntaxHandler, requestID_ServiceResponse=%d"
#define MBBMS_SUB_PARSE_1029_112_2_17_23_10_23_1104 "[SG]: subServiceResponseSyntaxHandler, globalStatusCode_ServiceResponse=%d"
#define MBBMS_SUB_PARSE_1036_112_2_17_23_10_23_1105 "[SG]: subServiceResponseSyntaxHandler, description_ServiceResponse=%s"
#define MBBMS_SUB_PARSE_1046_112_2_17_23_10_23_1106 "[SG]: subServiceResponseSyntaxHandler PurchaseItem_ServiceResponse"
#define MBBMS_SUB_PARSE_1055_112_2_17_23_10_23_1107 "[SG]: subServiceResponseSyntaxHandler, globalIDRef_PurchaseItem_Service=%s"
#define MBBMS_SUB_PARSE_1071_112_2_17_23_10_23_1108 "[SG]: subServiceResponseSyntaxHandler, itemwiseStatusCode_PurchaseItem=%d"
#define MBBMS_SUB_PARSE_1075_112_2_17_23_10_23_1109 "[SG]: subServiceResponseSyntaxHandler SmartcardProfileSpecificPart_Service"
#define MBBMS_SUB_PARSE_1087_112_2_17_23_10_23_1110 "[SG]: subServiceResponseSyntaxHandler LTKM_SmartcardProfile_Service"
#define MBBMS_SUB_PARSE_1132_112_2_17_23_10_23_1111 "[SG]: subAccountResponseSyntaxHandler AccountResponse"
#define MBBMS_SUB_PARSE_1139_112_2_17_23_10_23_1112 "[SG]: subAccountResponseSyntaxHandler, requestID_AccountResponse=%d"
#define MBBMS_SUB_PARSE_1153_112_2_17_23_10_23_1113 "[SG]: subAccountResponseSyntaxHandler, globalStatusCode_ServiceResponse=%d"
#define MBBMS_SUB_PARSE_1160_112_2_17_23_10_23_1114 "[SG]: subAccountResponseSyntaxHandler Description_AccountResponse=%s"
#define MBBMS_SUB_PARSE_1169_112_2_17_23_10_23_1115 "[SG]: subAccountResponseSyntaxHandler BillingInformation=%s"
#define MBBMS_SUB_PARSE_1173_112_2_17_23_10_23_1116 "[SG]: subAccountResponseSyntaxHandler, ignore BillingInformation 1..N"
#define MBBMS_SUB_PARSE_1203_112_2_17_23_10_23_1117 "[SG]: subAccountResponseSyntaxHandler PurchaseItem_AccountResponse"
#define MBBMS_SUB_PARSE_1212_112_2_17_23_10_23_1118 "[SG]: subAccountResponseSyntaxHandler, globalIDRef_PurchaseItem_Account=%s"
#define MBBMS_SUB_PARSE_1223_112_2_17_23_10_24_1119 "[SG]: subAccountResponseSyntaxHandler BillingInformation=%s"
#define MBBMS_SUB_PARSE_1227_112_2_17_23_10_24_1120 "[SG]: subAccountResponseSyntaxHandler, ignore sub_Description_PurchaseItem 1..N"
#define MBBMS_SUB_PARSE_1253_112_2_17_23_10_24_1121 "[SG]: subAccountResponseSyntaxHandler PurchaseItemFragment_PurchaseItem"
#define MBBMS_SUB_PARSE_1263_112_2_17_23_10_24_1122 "[SG]: subAccountResponseSyntaxHandler, idref_PurchaseData=%s"
#define MBBMS_SUB_PARSE_1304_112_2_17_23_10_24_1123 "[SG]: subActivationResponseSyntaxHandler ActivationResponse"
#define MBBMS_SUB_PARSE_1311_112_2_17_23_10_24_1124 "[SG]: subActivationResponseSyntaxHandler, Result_ActivationResponse=%d"
#define MBBMS_SUB_PARSE_1319_112_2_17_23_10_24_1125 "[SG]: subActivationResponseSyntaxHandler Desc_ActivationResponse=%s"
#define MBBMS_SUB_PARSE_1324_112_2_17_23_10_24_1126 "[SG]: subActivationResponseSyntaxHandler SmartcardProfileSpecificPart_Activation"
#define MBBMS_SUB_PARSE_1336_112_2_17_23_10_24_1127 "[SG]: subActivationResponseSyntaxHandler LTKM_SmartcardProfile_Activation"
#define MBBMS_SUB_PARSE_1393_112_2_17_23_10_24_1128 "[SG]: subSubscriptionUpdateSyntaxHandler mbmsSecurityRegisterResponse"
#define MBBMS_SUB_PARSE_1404_112_2_17_23_10_24_1129 "[SG]: subSubscriptionUpdateSyntaxHandler Response_MSecurityReg"
#define MBBMS_SUB_PARSE_1413_112_2_17_23_10_24_1130 "[SG]: subSubscriptionUpdateSyntaxHandler, serviceID_Response=%s"
#define MBBMS_SUB_PARSE_1423_112_2_17_23_10_24_1131 "[SG]: subSubscriptionUpdateSyntaxHandler, ResponseCode_Response_Security=%d"
#define MBBMS_SUB_PARSE_1428_112_2_17_23_10_24_1132 "[SG]: subSubscriptionUpdateSyntaxHandler SubscriptionUpdateExtension"
#define MBBMS_SUB_PARSE_1436_112_2_17_23_10_24_1133 "[SG]: subSubscriptionUpdateSyntaxHandler Response_description=%s"
#define MBBMS_SUB_PARSE_1445_112_2_17_23_10_24_1134 "[SG]: subSubscriptionUpdateSyntaxHandler, version_subUpdateExtension=%d"
#define MBBMS_SUB_PARSE_1456_112_2_17_23_10_24_1135 "[SG]: subSubscriptionUpdateSyntaxHandler LTKM_subUpdateExtension"
#define MBBMS_SUB_PARSE_1465_112_2_17_23_10_24_1136 "[SG]: subSubscriptionUpdateSyntaxHandler, validFrom_LTKM=%d"
#define MBBMS_SUB_PARSE_1474_112_2_17_23_10_24_1137 "[SG]: subSubscriptionUpdateSyntaxHandler, validUntil_LTKM=%d"
#define MBBMS_SUB_PARSE_1484_112_2_17_23_10_24_1138 "[SG]: subSubscriptionUpdateSyntaxHandler, MSKID_LTKM=%d"
#define MBBMS_SUB_PARSE_1496_112_2_17_23_10_24_1139 "[SG]: subSubscriptionUpdateSyntaxHandler RegistrationResponseExtension"
#define MBBMS_SUB_PARSE_1504_112_2_17_23_10_24_1140 "[SG]: subSubscriptionUpdateSyntaxHandler, delivery_type=%d"
#define MBBMS_SUB_PARSE_1512_112_2_17_23_10_24_1141 "[SG]: subSubscriptionUpdateSyntaxHandler, delivery_type=%d"
#define MBBMS_SUB_PARSE_1556_112_2_17_23_10_24_1142 "[SG]: subMSKResponseSyntaxHandler, MSKRes_description=%s"
#define MBBMS_SUB_PARSE_1560_112_2_17_23_10_24_1143 "[SG]: subMSKResponseSyntaxHandler mbmsMSKResponse"
#define MBBMS_SUB_PARSE_1569_112_2_17_23_10_24_1144 "[SG]: subMSKResponseSyntaxHandler Response_MSKResp"
#define MBBMS_SUB_PARSE_1573_112_2_17_23_10_24_1145 "[SG]: subMSKResponseSyntaxHandler MSK_Response"
#define MBBMS_SUB_PARSE_1588_112_2_17_23_10_24_1146 "[SG]: subMSKResponseSyntaxHandler, keyDomainID_MSK"
#define MBBMS_SUB_PARSE_1597_112_2_17_23_10_24_1147 "[SG]: subMSKResponseSyntaxHandler, MSKID_MSK=%d"
#define MBBMS_SUB_PARSE_1614_112_2_17_23_10_24_1148 "[SG]: subMSKResponseSyntaxHandler, ResponseCode_Response_MSKResp=%d"
#define MBBMS_SUB_PARSE_1619_112_2_17_23_10_24_1149 "[SG]: subMSKResponseSyntaxHandler, LTKMResponseMSKExtension_Response"
#define MBBMS_SUB_PARSE_1627_112_2_17_23_10_24_1150 "[SG]: subMSKResponseSyntaxHandler, version_LTKMExtension=%d"
#define MBBMS_SUB_PARSE_1640_112_2_17_23_10_24_1151 "[SG]: subMSKResponseSyntaxHandler, LTKM_LTKMExtension"
#define MBBMS_SUB_PARSE_1679_112_2_17_23_10_24_1152 "[SG]: subSyntaxHandler none type"
#define MBBMS_SUB_PARSE_1727_112_2_17_23_10_25_1153 "[SG]: subSyntaxHandler none subresp type, from=%d, syntax_name=%s, element=%s"
#define MBBMS_SUB_PARSE_1735_112_2_17_23_10_25_1154 "[SG]: subSyntaxHandler subtype(%d) data is zero"
#define MBBMS_SUB_PARSE_1759_112_2_17_23_10_25_1155 "[SG]: subBuildSGDataList enter"
#define MBBMS_SUB_PARSE_1786_112_2_17_23_10_25_1156 "[SG]: subBuildSGDataList ser idref ptr=0x%x"
#define MBBMS_SUB_PARSE_1787_112_2_17_23_10_25_1157 "[SG]: subBuildSGDataList con idref ptr=0x%x"
#define MBBMS_SUB_PARSE_1788_112_2_17_23_10_25_1158 "[SG]: subBuildSGDataList namelist ptr=0x%x"
#define MBBMS_SUB_PARSE_1789_112_2_17_23_10_25_1159 "[SG]: subBuildSGDataList desc ptr=0x%x"
#define MBBMS_SUB_PARSE_1799_112_2_17_23_10_25_1160 "[SG]: subBuildSGDataList PurchaseItem alloc error"
#define MBBMS_SUB_PARSE_1829_112_2_17_23_10_25_1161 "[SG]: subBuildSGDataList PurchaseData alloc error"
#define MBBMS_SUB_PARSE_1856_112_2_17_23_10_25_1162 "[SG]: subXmlStartElementHandler name=%s"
#define MBBMS_SUB_PARSE_1935_112_2_17_23_10_25_1163 "[SG]: subXmlStartElementHandler exit"
#define MBBMS_SUB_PARSE_1953_112_2_17_23_10_25_1164 "[SG]: subXmlEndElementHandler name=%s"
#define MBBMS_SUB_PARSE_2008_112_2_17_23_10_25_1165 "[SG]: subXmlEndElementHandler exit"
#define MBBMS_SUB_PARSE_2028_112_2_17_23_10_25_1166 "[SG]: subXmlCharacterDataHandler, none type"
#define MBBMS_SUB_PARSE_2098_112_2_17_23_10_26_1167 "[SG]: SUB_ParseData fail"
#define MBBMS_SUB_PARSE_2113_112_2_17_23_10_26_1168 "MBBMS subCleanUpdateRespTypeList ENTERING... \n"
#define MBBMS_SUB_PARSE_2129_112_2_17_23_10_26_1169 "MBBMS subCleanUpdateRespTypeList OVER \n"
#define MBBMS_SUB_PARSE_2179_112_2_17_23_10_26_1170 "sub_CheckMSKID msk_id = %d"
#define MBBMS_SUB_PARSE_2181_112_2_17_23_10_26_1171 "sub_CheckMSKID get purchase_item errcode = %d"
#define MBBMS_SUB_PARSE_2186_112_2_17_23_10_26_1172 "sub_CheckMSKID temp = 0X%x"
#define MBBMS_SUB_PARSE_2190_112_2_17_23_10_26_1173 "sub_CheckMSKID get access errcode = %d"
#define MBBMS_SUB_PARSE_2194_112_2_17_23_10_26_1174 "sub_CheckMSKID get access systemlist_cnt = %d"
#define MBBMS_SUB_PARSE_2199_112_2_17_23_10_26_1175 "sub_CheckMSKID get access keylist_cnt = %d"
#define MBBMS_SUB_PARSE_2203_112_2_17_23_10_26_1176 "sub_CheckMSKID get access keyidlist_ptr = 0X%x"
#define MBBMS_SUB_PARSE_2207_112_2_17_23_10_26_1177 "sub_CheckMSKID get access data_len = %d"
#define MBBMS_SUB_PARSE_2208_112_2_17_23_10_26_1178 "sub_CheckMSKID get access data = %s"
#define MBBMS_SUB_PARSE_2215_112_2_17_23_10_26_1179 "sub_CheckMSKID get access sg_msk_id = %d"
#define MBBMS_SUB_PARSE_2299_112_2_17_23_10_26_1180 "[SG]: SUB_InitSubscription"
#define MBBMS_SUB_PARSE_2304_112_2_17_23_10_26_1181 "[SG]: SUB_InitSubscription id=%s"
#define MBBMS_SUB_PARSE_2310_112_2_17_23_10_26_1182 "[SG]: SUB_InitSubscription CheckPurchaseitemID fail"
#define MBBMS_SUB_PARSE_2315_112_2_17_23_10_26_1183 "[SG]: SUB_InitSubscription response_code fail"
#define MBBMS_SUB_PARSE_2354_112_2_17_23_10_26_1184 "[SG]: SUB_RefreshSubscription"
#define MBBMS_SUB_PARSE_2358_112_2_17_23_10_26_1185 "[SG]: SUB_RefreshSubscription sub_ptr is null"
#define MBBMS_SUB_PARSE_2364_112_2_17_23_10_26_1186 "[SG]: SUB_RefreshSubscription response_code fail"
#define MBBMS_SUB_PARSE_2371_112_2_17_23_10_26_1187 "[SG]: SUB_RefreshSubscription id=%s"
#define MBBMS_SUB_PARSE_2377_112_2_17_23_10_26_1188 "[SG]: SUB_RefreshSubscription CheckPurchaseitemID fail"
#define MBBMS_SUB_PARSE_2416_112_2_17_23_10_26_1189 "[SG]: SUB_UpdateSubscription"
#define MBBMS_SUB_PARSE_2421_112_2_17_23_10_26_1190 "[SG]: SUB_UpdateSubscription CheckPurchaseitemID fail"
#define MBBMS_SUB_PARSE_2426_112_2_17_23_10_26_1191 "[SG]: SUB_UpdateSubscription itemwiseStatusCode fail"
#define MBBMS_SUB_PARSE_2442_112_2_17_23_10_26_1192 "[SG]: SUB_UpdateSubscription, no sub info"
#define MBBMS_SUB_PARSE_2565_112_2_17_23_10_27_1193 "[SG]: sub_SavePurchaseData enter p_sub=0X%x"
#define MBBMS_SUB_PARSE_2572_112_2_17_23_10_27_1194 "[SG]: sub_SavePurchaseData, purchase=%s, response=%d"
#define MBBMS_SUB_PARSE_2576_112_2_17_23_10_27_1195 "[SG]: sub_SavePurchaseData, purchase count=%d"
#define MBBMS_SUB_PARSE_2582_112_2_17_23_10_27_1196 "[SG]: sub_SavePurchaseData, alloc mem fail!!!"
#define MBBMS_SUB_PARSE_2598_112_2_17_23_10_27_1197 "[SG]: sub_SavePurchaseData, write=%d"
#define MBBMS_SUB_PARSE_2601_112_2_17_23_10_27_1198 "[SG]: sub_SavePurchaseData, write file fail!!!"
#define MBBMS_SUB_PARSE_2607_112_2_17_23_10_27_1199 "[SG]: sub_SavePurchaseData, create file fail!!!"
#define MBBMS_SUB_PARSE_2629_112_2_17_23_10_27_1200 "[SG]: SUB_BuildPurchaseList, enter"
#define MBBMS_SUB_PARSE_2638_112_2_17_23_10_27_1201 "[SG]: SUB_BuildPurchaseList, get file size fail!!!"
#define MBBMS_SUB_PARSE_2645_112_2_17_23_10_27_1202 "[SG]: sub_SavePurchaseData, alloc data mem fail!!!"
#define MBBMS_SUB_PARSE_2651_112_2_17_23_10_27_1203 "[SG]: SUB_BuildPurchaseList, read file fail!!!"
#define MBBMS_SUB_PARSE_2661_112_2_17_23_10_27_1204 "[SG]: SUB_BuildPurchaseList, len=%d count=%d"
#define MBBMS_SUB_PARSE_2668_112_2_17_23_10_27_1205 "[SG]: sub_SavePurchaseData, alloc table mem fail!!!"
#define MBBMS_SUB_PARSE_2681_112_2_17_23_10_27_1206 "[SG]: SUB_BuildPurchaseList, open file fail!!!"
#define MBBMS_SUB_PARSE_2692_112_2_17_23_10_27_1207 "[SG]: SUB_BuildPurchaseList, ret=%d"
#define MBBMS_SUB_PARSE_2712_112_2_17_23_10_27_1208 "mbbms_SubPrint Inq ptr=0x%x info ptr=0x%x"
#define MBBMS_SUB_PARSE_2714_112_2_17_23_10_27_1209 "mbbms_SubPrint pur_item=0x%x"
#define MBBMS_SUB_PARSE_2716_112_2_17_23_10_27_1210 "mbbms_SubPrint pur_item count=%d"
#define MBBMS_SUB_PARSE_2720_112_2_17_23_10_27_1211 "mbbms_SubPrint item_ptr=0x%x"
#define MBBMS_SUB_PARSE_2721_112_2_17_23_10_27_1212 "mbbms_SubPrint ser idref ptr=0x%x"
#define MBBMS_SUB_PARSE_2722_112_2_17_23_10_27_1213 "mbbms_SubPrint con idref ptr=0x%x"
#define MBBMS_SUB_PARSE_2723_112_2_17_23_10_27_1214 "mbbms_SubPrint namelist ptr=0x%x"
#define MBBMS_SUB_PARSE_2724_112_2_17_23_10_27_1215 "mbbms_SubPrint desc ptr=0x%x"
#define MBBMS_SUB_PARSE_2726_112_2_17_23_10_27_1216 "mbbms_SubPrint ser_list=0x%x"
#define MBBMS_SUB_PARSE_2728_112_2_17_23_10_27_1217 "mbbms_SubPrint ser_cnt=%d"
#define MBBMS_SUB_PARSE_2732_112_2_17_23_10_27_1218 "mbbms_SubPrint ser_ptr=0x%x"
#define MBBMS_SUB_PARSE_2736_112_2_17_23_10_27_1219 "mbbms_SubPrint pur_data=0x%x"
#define MBBMS_SUB_PARSE_2738_112_2_17_23_10_27_1220 "mbbms_SubPrint pur_data count=%d"
#define MBBMS_SUB_PARSE_2742_112_2_17_23_10_27_1221 "mbbms_SubPrint data ptr=0x%x"
#define MBBMS_SUB_PARSE_2743_112_2_17_23_10_27_1222 "mbbms_SubPrint desc ptr=0x%x"
#define MBBMS_SUB_PARSE_2744_112_2_17_23_10_27_1223 "mbbms_SubPrint price ptr=0x%x"
#define MBBMS_SUB_PARSE_2745_112_2_17_23_10_27_1224 "mbbms_SubPrint itemref ptr=0x%x"
#define MBBMS_SUB_PARSE_2746_112_2_17_23_10_27_1225 "mbbms_SubPrint channel ptr=0x%x"
#define MIKEY_179_112_2_17_23_10_29_1226 "MIKEY start %p"
#define MIKEY_209_112_2_17_23_10_29_1227 "MIKEY_GetCurrentPayloadType: handle is zero"
#define MIKEY_224_112_2_17_23_10_29_1228 "MIKEY_GetNextPayloadType: current_payload_type exceed valid range"
#define MIKEY_230_112_2_17_23_10_29_1229 "MIKEY_GetNextPayloadType: handle is zero"
#define MIKEY_245_112_2_17_23_10_29_1230 "MIKEY_GetPayloadInfo: current_payload_type exceed valid range"
#define MIKEY_251_112_2_17_23_10_29_1231 "MIKEY_GetPayloadInfo: handle is zero"
#define MIKEY_291_112_2_17_23_10_29_1232 "MIKEY_NextPayload: next type %s"
#define MIKEY_297_112_2_17_23_10_29_1233 "MIKEY_NextPayload: next payloadlen %d"
#define MIKEY_302_112_2_17_23_10_29_1234 "MIKEY_NextPayload: last payload"
#define MIKEY_308_112_2_17_23_10_29_1235 "MIKEY_NextPayload: next_payload_type exceed valid range"
#define MUA_CMD_183_112_2_17_23_10_32_1236 "ParseCardSW nStatus=%x"
#define MUA_CMD_306_112_2_17_23_10_32_1237 "Card_SelectFile ret = %d,nStatus=%x"
#define MUA_CMD_347_112_2_17_23_10_32_1238 "Card_SelectFile ret = %d,nStatus=%x"
#define MUA_CMD_408_112_2_17_23_10_32_1239 "Card_GetResp ret=%d,Status = %x"
#define MUA_CMD_426_112_2_17_23_10_32_1240 "Card_Case4_Cmd ret=%d,Status=%x"
#define MUA_CMD_453_112_2_17_23_10_32_1241 "Card_Case4_Cmd Status = %x"
#define MUA_CMD_481_112_2_17_23_10_32_1242 "Card_ReadFixedRec ret=%d,nStatus=%x"
#define MUA_CMD_501_112_2_17_23_10_32_1243 "Card_Switch_2G3G nm=%d"
#define MUA_CMD_526_112_2_17_23_10_32_1244 "Card_Switch_2G3G ret=%d,nStatus=%x"
#define MUA_CMD_532_112_2_17_23_10_32_1245 "Card_Switch_2G3G 2g mode"
#define MUA_CMD_591_112_2_17_23_10_32_1246 "Card_KeepAIDCtx ret = %d,nStatus=%x"
#define MUA_CMD_612_112_2_17_23_10_33_1247 "Card_KeepAIDCtx ret = %d,nStatus=%x"
#define MUA_CMD_892_112_2_17_23_10_33_1248 "MUACMD_GBABS_3G res_len=%d,ck_len=%d,ik_len=%d,rand_len=%d,data_len=%d"
#define MUA_CMD_900_112_2_17_23_10_33_1249 "GBA cmd_data[%d]=0x%x"
#define MUA_CMD_1023_112_2_17_23_10_33_1250 "MUACMD_GBABS_2G res_len=%d,kc_len=%d,ks_input_len=%d,rand_len=%d,data_len=%d"
#define MUA_CMD_1031_112_2_17_23_10_33_1251 "GBA cmd_data[%d]=0x%x"
#define MUA_CMD_1037_112_2_17_23_10_33_1252 "MBBMS MUACMD_GBABS_2G: ret:%d \n"
#define MUA_CMD_1049_112_2_17_23_10_33_1253 "MBBMS MUACMD_GBABS_2G res len %d \n"
#define MUA_CMD_1057_112_2_17_23_10_33_1254 "MBBMS MUACMD_GBABS_2G cnonce len %d \n"
#define MUA_CMD_1121_112_2_17_23_10_34_1255 "MBBMS MUACMD_GBA_NAF First ok \n"
#define MUA_CMD_1164_112_2_17_23_10_34_1256 "MUACMD_GBA_NAF naf_id_len=%d,impi_len=%d,data_len=%d"
#define MUA_CMD_1170_112_2_17_23_10_34_1257 "NAF cmd_data[%d]=0x%x"
#define MUA_CMD_1176_112_2_17_23_10_34_1258 "MBBMS MUACMD_GBA_NAF second ok\n"
#define MUA_CMD_1185_112_2_17_23_10_34_1259 "MUACMD_GBA_NAF data_len=%d"
#define MUA_CMD_1224_112_2_17_23_10_34_1260 "MUACMD_MBMS MAX LEN:%d \n"
#define MUA_CMD_1230_112_2_17_23_10_34_1261 "MUACMD_MBMS SCI_GetMutex ok \n"
#define MUA_CMD_1234_112_2_17_23_10_34_1262 "MUACMD_MBMS mikey len:%d type:%d \n"
#define MUA_CMD_1241_112_2_17_23_10_34_1263 "DATA[%d]: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x n"
#define MUA_CMD_1286_112_2_17_23_10_34_1264 "MBBMS MSK UPDATA CMD RECEIVED \n"
#define MUA_CMD_1291_112_2_17_23_10_34_1265 "MBBMS MTK UPDATA CMD RECEIVED \n"
#define MUA_CMD_1319_112_2_17_23_10_34_1266 "MBMS cmd_data[%d]=0x%x"
#define MUA_CMD_1325_112_2_17_23_10_34_1267 "MUACMD_MBMS() apdu_data=%d,ret=%d,nResLen=%d"
#define MUA_CMD_1331_112_2_17_23_10_34_1268 "MUACMD_MBMS Result len:%d data:0x%x %x %x %x %x n"
#define MUA_CMD_1332_112_2_17_23_10_34_1269 "MUACMD_MBMS input max len:%d \n"
#define MUA_CMD_1789_112_2_17_23_10_35_1270 "MUACMD_IsGBANeeded() resLen = %d"
#define MUA_CMD_2044_112_2_17_23_10_36_1271 "MBBMS MUACMD_GetSDVersion [%s]\n "
#define MUA_TEST_132_112_2_17_23_10_36_1272 "MUACMD_MBBMS_GetCMMBSN()"
#define MUA_TEST_151_112_2_17_23_10_36_1273 "MUACMD_MBBMS_GetCMMBSN()ret=%d"
#define MUA_TEST_154_112_2_17_23_10_36_1274 "sn[%d]=%d"
#define MUA_TEST_158_112_2_17_23_10_36_1275 "MUACMD_GetSDVersion()ret=%d,v0=%x,v1=%x,v2=%x,v3=%x"
#define MUA_TEST_160_112_2_17_23_10_36_1276 "MUACMD_GetSDID()ret=%d,id0=%x,id1=%d,id2=%d,id3=%d,id4=%d,id5=%d"
#define MUA_TEST_204_112_2_17_23_10_36_1277 "DoGBAproc_3G"
#define MUA_TEST_255_112_2_17_23_10_36_1278 "MUACMD_GBABS_3G()ret=%d"
#define MUA_TEST_270_112_2_17_23_10_36_1279 "ME--------->TF  B-TID: %s\r\n"
#define MUA_TEST_279_112_2_17_23_10_36_1280 "ME--------->TF  ks: %s\r\n"
#define MUA_TEST_339_112_2_17_23_10_37_1281 "DoGBAproc_3G_smd"
#define MUA_TEST_370_112_2_17_23_10_37_1282 "DoGBAproc_3G_smd()ret=%d"
#define MUA_TEST_404_112_2_17_23_10_37_1283 "DoGBAproc_2G"
#define MUA_TEST_435_112_2_17_23_10_37_1284 "MUACMD_GBABS_2G()ret=%d"
#define MUA_TEST_468_112_2_17_23_10_37_1285 "MUACMD_GBA_NAF ret=%d,resp_data.data_length=%d"
#define MUA_TEST_497_112_2_17_23_10_37_1286 "MUACMD_MBMS ret=%d,resp_data.data_length=%d"
#define SDP_326_112_2_17_23_10_38_1287 "ParseSDP param error!"
#define SDP_412_112_2_17_23_10_38_1288 "[SG]: SetSdpAttributeValue, cannot value\n"
#define SDP_424_112_2_17_23_10_38_1289 "[SG]: SetSdpAttributeValue, ISMACrypCryptoSuite unknow type"
#define SDP_457_112_2_17_23_10_38_1290 "[SG]: SetSdpAttributeValue, GetSaltKey error"
#define SDP_463_112_2_17_23_10_38_1291 "[SG]: SetSdpAttributeValue, base64 len=%d"
#define SDP_466_112_2_17_23_10_38_1292 "[SG]: SetSdpAttributeValue, base64= %s"
#define SDP_469_112_2_17_23_10_38_1293 "[SG]: SetSdpAttributeValue, ISMACrypSalt decode error"
#define SDP_473_112_2_17_23_10_38_1294 "[SG]: SetSdpAttributeValue, outlen=%d"
#define SDP_482_112_2_17_23_10_38_1295 "[SG]: SetSdpAttributeValue, outbuf[%03d] = %02x %02x %02x %02x %02x %02x %02x %02x"
#define SDP_493_112_2_17_23_10_38_1296 "[SG]: SetSdpAttributeValue, ISMACrypSalt not base64"
#define SDP_501_112_2_17_23_10_38_1297 "[SG]: SetSdpAttributeValue, ISMACrypSalt has no dot"
#define SDP_510_112_2_17_23_10_38_1298 "[SG]: SetSdpAttributeValue, GetSaltKey error"
#define SDP_513_112_2_17_23_10_38_1299 "[SG]: SetSdpAttributeValue, base64 len=%d"
#define SDP_516_112_2_17_23_10_38_1300 "[SG]: SetSdpAttributeValue, base64= %s"
#define SDP_519_112_2_17_23_10_38_1301 "[SG]: SetSdpAttributeValue, USE BASE64 Decode error"
#define SDP_523_112_2_17_23_10_38_1302 "[SG]: SetSdpAttributeValue, outlen=%d"
#define SDP_532_112_2_17_23_10_38_1303 "[SG]: SetSdpAttributeValue, outbuf[%03d] = %02x %02x %02x %02x %02x %02x %02x %02x"
#define SDP_600_112_2_17_23_10_39_1304 "[SG]: MBBMSSDP_CreateIsma enter"
#define SDP_604_112_2_17_23_10_39_1305 "[SG]: MBBMSSDP_CreateIsma, no serviceid, cmmb_frqNo %d,cmmb_serviceId %dn"
#define SDP_612_112_2_17_23_10_39_1306 "[SG]: MBBMSSDP_CreateIsma, no sdp \n"
#define SDP_625_112_2_17_23_10_39_1307 "[SG]: MBBMSSDP_CreateIsma, no sdp content string\n"
#define SDP_634_112_2_17_23_10_39_1308 "[SG]: MBBMSSDP_CreateIsma, no match access, service id %s n"
#define SDP_658_112_2_17_23_10_39_1309 "[SG]: MBBMSSDP_CreateIsma, no serviceid, cmmb_frqNo %d,cmmb_serviceId %dn"
#define SDP_666_112_2_17_23_10_39_1310 "[SG]: MBBMSSDP_CreateIsma, no sdp \n"
#define SDP_679_112_2_17_23_10_39_1311 "[SG]: MBBMSSDP_CreateIsma, no sdp content string\n"
#define SDP_688_112_2_17_23_10_39_1312 "[SG]: MBBMSSDP_CreateIsma, no match access, service id %s n"
#define SDUA_DRV_244_112_2_17_23_10_39_1313 "SDUA_Direct_Read not correct header"
#define SDUA_DRV_252_112_2_17_23_10_39_1314 "SDUA_Direct_Read need retry read"
#define SDUA_DRV_271_112_2_17_23_10_39_1315 "SDUA_Direct_Read return data too large"
#define SDUA_DRV_315_112_2_17_23_10_39_1316 "SDUA_Reset write error\n"
#define SDUA_DRV_325_112_2_17_23_10_39_1317 "SDUA_Reset read error\n"
#define SDUA_DRV_331_112_2_17_23_10_39_1318 "SDUA_Reset return Size error. ret Size = %d\n"
#define SDUA_DRV_337_112_2_17_23_10_39_1319 "out[%d]=%x"
#define SDUA_DRV_351_112_2_17_23_10_40_1320 "SDUA_Reset return nRet=%d"
#define SDUA_DRV_400_112_2_17_23_10_40_1321 "cmd sdua size=0x%x"
#define SDUA_DRV_405_112_2_17_23_10_40_1322 "cmd sdua %02x %02x %02x %02x %02x"
#define SDUA_DRV_411_112_2_17_23_10_40_1323 "cmd sdua %02x"
#define SDUA_DRV_436_112_2_17_23_10_40_1324 "SDUA_IoControl()*o_nSize=%d"
#define SDUA_DRV_443_112_2_17_23_10_40_1325 "resp sdua size=0x%x,resp=0x%x"
#define SDUA_DRV_450_112_2_17_23_10_40_1326 "resp sdua %02x %02x %02x %02x %02x"
#define SDUA_DRV_456_112_2_17_23_10_40_1327 "resp sdua %02x"
#define SDUA_DRV_510_112_2_17_23_10_40_1328 "cmd sipua size=0x%x"
#define SDUA_DRV_515_112_2_17_23_10_40_1329 "cmd sipua %02x %02x %02x %02x %02x"
#define SDUA_DRV_521_112_2_17_23_10_40_1330 "cmd sipua %02x"
#define SDUA_DRV_528_112_2_17_23_10_40_1331 "resp sipua size=0x%x,resp=0x%x"
#define SDUA_DRV_535_112_2_17_23_10_40_1332 "resp sipua %02x %02x %02x %02x %02x"
#define SDUA_DRV_541_112_2_17_23_10_40_1333 "resp sipua %02x"
#define UA_DRV_76_112_2_17_23_10_40_1334 "cmd uam size=0x%x"
#define UA_DRV_81_112_2_17_23_10_40_1335 "cmd uam %02x %02x %02x %02x %02x"
#define UA_DRV_87_112_2_17_23_10_40_1336 "cmd uam %02x"
#define UA_DRV_93_112_2_17_23_10_40_1337 "resp uam size=0x%x,resp=0x%x"
#define UA_DRV_100_112_2_17_23_10_40_1338 "resp uam %02x %02x %02x %02x %02x"
#define UA_DRV_106_112_2_17_23_10_40_1339 "resp uam %02x"
#define CA_ADAPTER_112_112_2_17_23_10_41_1340 "TM_GetSysDate Failed!!!\n"
#define CA_ADAPTER_119_112_2_17_23_10_41_1341 "TM_GetSysTime FAILED!!!\n"
#define CA_ADAPTER_189_112_2_17_23_10_41_1342 "MBBMS GetMTKIDFromMIKEY MSKID = 0x%x MTKID = 0x%x DOMAINID = 0x%x"
#define CA_ADAPTER_200_112_2_17_23_10_41_1343 "MBBMS GetMTKIDFromMIKEY MIKEY_Open failed"
#define CA_ADAPTER_219_112_2_17_23_10_41_1344 "MBBMS GetVerDataFromMIKEY %s"
#define CA_ADAPTER_229_112_2_17_23_10_41_1345 "MBBMS GetVerDataFromMIKEY verdata = 0x%x 0x%x n"
#define CA_ADAPTER_239_112_2_17_23_10_41_1346 "MBBMS GetVerDataFromMIKEY MIKEY_Open failed"
#define CA_ADAPTER_273_112_2_17_23_10_41_1347 "MBBMS GetECMDataType PARSING CAT ......\n"
#define CA_ADAPTER_292_112_2_17_23_10_41_1348 "MBBSM GetECMDataType CA SYSTEM ID:0x%x\n"
#define CA_ADAPTER_298_112_2_17_23_10_41_1349 "MBBMS GetECMDataType data_type:0x%x ecm_transport_type:0x%xn"
#define CA_ADAPTER_340_112_2_17_23_10_41_1350 "KDA CSL:cslFilterSetPatterns MTV_STIMI_DATA_128BYTE_FILTER\n"
#define CA_ADAPTER_352_112_2_17_23_10_41_1351 "KDA CSL:cslFilterSetPatterns MTV_STIMI_DATA_16BYTE_FILTER\n"
#define CA_ADAPTER_376_112_2_17_23_10_41_1352 "MBBMS ecm filter word 0x%X"
#define CA_ADAPTER_399_112_2_17_23_10_41_1353 "MBBMS EnableECMFilter Filter Enable OK filter_id:%d Sub_type:%d data_type:%dn"
#define CA_ADAPTER_419_112_2_17_23_10_41_1354 "MBBMS SetIsma element_id %d\n"
#define CA_ADAPTER_424_112_2_17_23_10_41_1355 "MBBMS SetIsma ismacrypData.salt[%d] %d\n"
#define CA_ADAPTER_434_112_2_17_23_10_41_1356 "MBBMS SetIsma rtpData.auSizeLengthType %x\n"
#define CA_ADAPTER_447_112_2_17_23_10_41_1357 "MBBMS SetIsma unknown type %d"
#define CA_ADAPTER_463_112_2_17_23_10_41_1358 "MBBMS SetIsma isma_data[i] %x\n"
#define CA_ADAPTER_471_112_2_17_23_10_41_1359 "MBBMS CADSX_SetISMA element_type %d\n"
#define CA_ADAPTER_484_112_2_17_23_10_41_1360 "MBBMS CADSX_SetISMA unknown element_type %d"
#define CA_ADAPTER_494_112_2_17_23_10_41_1361 "MBBMS Enter ISMACallback 0x%X"
#define CA_ADAPTER_544_112_2_17_23_10_42_1362 "MBBMS ECMRecv ECM Received"
#define CA_ADAPTER_568_112_2_17_23_10_42_1363 "MBBMS ECMRecv Filter %d len = %d Send Over [%d]"
#define CA_ADAPTER_604_112_2_17_23_10_42_1364 "MBBMS MIKEY RESPONSE RECEIVED cause:%d mikey_type:%d key_len:%d n"
#define CA_ADAPTER_614_112_2_17_23_10_42_1365 "MBBMS MTK DECRYPTED 0x%x %x %x %x %x %x %x %x n"
#define CA_ADAPTER_625_112_2_17_23_10_42_1366 "MBBMS MTK sendMMI signal"
#define CA_ADAPTER_659_112_2_17_23_10_42_1367 "KDA DSX:dsxSetKeys  key_index%d\n"
#define CA_ADAPTER_692_112_2_17_23_10_42_1368 " ---MBBMS CA: ALREADY RUNNING\n"
#define CA_ADAPTER_720_112_2_17_23_10_42_1369 "Create ca adapter thread failed!!"
#define CA_ADAPTER_726_112_2_17_23_10_42_1370 " ---MBBMS CA: BEGIN TO RUN ....   \n"
#define CA_ADAPTER_740_112_2_17_23_10_42_1371 " ---KDA CA:ENTRY TERMINATED!   \n"
#define CA_ADAPTER_751_112_2_17_23_10_42_1372 " ---KDA CA: TERMINATED!   \n"
#define CA_ADAPTER_759_112_2_17_23_10_42_1373 " KDA ADPT: ca_play_callback result:%d \n"
#define CA_ADAPTER_781_112_2_17_23_10_42_1374 "MBBMS Set Media Type:%d \n"
#define CA_ADAPTER_792_112_2_17_23_10_42_1375 "MBBMS ECM content: %s"
#define CA_ADAPTER_796_112_2_17_23_10_42_1376 "MBBMS ECM content: %s"
#define CA_ADAPTER_816_112_2_17_23_10_42_1377 "MBBMS adapter enter thread"
#define CA_ADAPTER_824_112_2_17_23_10_42_1378 "MBBMS adapter receive %d"
#define CA_ADAPTER_832_112_2_17_23_10_42_1379 "MBBMS ECM msg Received %d[%d]"
#define CA_ADAPTER_851_112_2_17_23_10_42_1380 "MBBMS MIKEY %d is NULL"
#define CA_ADAPTER_854_112_2_17_23_10_42_1381 "MBBMS s_current_key_indicator 0x%X key_ind 0x%X[%d]"
#define CA_ADAPTER_896_112_2_17_23_10_42_1382 "MBBMS key indicator %x [%d]"
#define CA_ADAPTER_911_112_2_17_23_10_42_1383 "MBBMS MTK error %d [%d]"
#define CA_ADAPTER_919_112_2_17_23_10_43_1384 "MBBMS mtk_id 0x%X domainid 0x%X[%d]"
#define CA_ADAPTER_922_112_2_17_23_10_43_1385 "MBBMS MTK error %d [%d]"
#define CA_ADAPTER_932_112_2_17_23_10_43_1386 "MBBMS ECM_GetInfo error"
#define CA_ADAPTER_954_112_2_17_23_10_43_1387 "CA_Adapter_Thread: unknown msg id %d!"
#define CA_ADAPTER_981_112_2_17_23_10_43_1388 "MBBMS CAT SHOWING DATA:\n"
#define CA_ADAPTER_983_112_2_17_23_10_43_1389 "MBBMS CAT No:%d Value:0x%x\n"
#define CA_ADAPTER_1042_112_2_17_23_10_43_1390 "MBBMS HandleISMAReqSig \n"
#define CA_ADAPTER_1095_112_2_17_23_10_43_1391 "KDA DSX:dsxConfigure initializationVectorLength %x\n"
#define CA_ADAPTER_1096_112_2_17_23_10_43_1392 "KDA DSX:dsxConfigure deltaInitializationVectorLength %x\n"
#define CA_ADAPTER_1097_112_2_17_23_10_43_1393 "KDA DSX:dsxConfigure keyIndicatorLength %x\n"
#define CA_ADAPTER_1098_112_2_17_23_10_43_1394 "KDA DSX:dsxConfigure useKeyIndicatorPerAu %x\n"
#define CA_ADAPTER_1099_112_2_17_23_10_43_1395 "KDA DSX:dsxConfigure useSelectiveEncryption %x\n"
#define CA_ADAPTER_1101_112_2_17_23_10_43_1396 "KDA DSX:dsxConfigure auSizeLengthType length%x\n"
#define CA_ADAPTER_1133_112_2_17_23_10_43_1397 "KDA DSX:dsxConfigure useRandomAccessFlag %x\n"
#define CA_ADAPTER_1134_112_2_17_23_10_43_1398 "KDA DSX:dsxConfigure streamStateLength %x\n"
#define CA_ADAPTER_1135_112_2_17_23_10_43_1399 "KDA DSX:dsxConfigure indexDeltaLength %x\n"
#define CA_ADAPTER_1136_112_2_17_23_10_43_1400 "KDA DSX:dsxConfigure indexLength%x\n"
#define CA_ADAPTER_1137_112_2_17_23_10_43_1401 "KDA DSX:dsxConfigure auxiliaryDataSizeLength %x\n"
#define CA_ADAPTER_1138_112_2_17_23_10_43_1402 "KDA DSX:dsxConfigure ctsDeltaLength %x\n"
#define CA_ADAPTER_1139_112_2_17_23_10_43_1403 "KDA DSX:dsxConfigure useRandomAccessFlag %x\n"
#define CA_ADAPTER_1145_112_2_17_23_10_43_1404 "KDA DSX:HandleISMAReqSig isma_data[i] %x\n"
#define CA_ADAPTER_1172_112_2_17_23_10_43_1405 " MBBMS HandleDescramKeyReqSig key_select%x,key_id%x \n"
#define CA_ADAPTER_1197_112_2_17_23_10_43_1406 "KDA DSX :HandleDescramKeyReqSig xSetNumKeys%x \n"
#define CA_ADAPTER_1201_112_2_17_23_10_43_1407 "KDA DSX :HandleDescramKeyReqSig key[%d]%x \n"
#define CA_ADAPTER_1221_112_2_17_23_10_43_1408 "KDA DSX :HandleDescramKeyReqSig key index%x \n"
#define CA_ADAPTER_1229_112_2_17_23_10_43_1409 "KDA DSX :HandleDescramKeyReqSig don't get key key_rcv_demux_cmd_ref 0x%x\n"
#define CA_ADAPTER_1246_112_2_17_23_10_43_1410 "MBBMS ProcessDemuxReqCallback \n"
#define CA_ADAPTER_1263_112_2_17_23_10_43_1411 "MBBMS KDA CSL:time ProcessDemuxReqCallback tick%x\n"
#define CA_ADAPTER_1264_112_2_17_23_10_43_1412 "MBBMS_KDA DSX: :ProcessDemuxReqCallback %x\n"
#define CA_ADAPTER_1281_112_2_17_23_10_43_1413 "MBBMS Received UnKNOWN DEMUX REQ CALLBACK \n"
#define CA_ADAPTER_1472_112_2_17_23_10_44_1414 "GetISMA: serviceid(%d) is not 601 or 602"
#define CA_ADAPTER_1494_112_2_17_23_10_44_1415 "CADSX_GetISMA: service_id %d frq_no %d"
#define CA_ADAPTER_1573_112_2_17_23_10_44_1416 "MBBMS ProcessDemuxData,no enough memory %d!"
#define CA_KEY_93_112_2_17_23_10_44_1417 "CA_Key_Push: %X"
#define CA_KEY_113_112_2_17_23_10_44_1418 "CA_Key_Push: error keycount %d"
#define CA_KEY_125_112_2_17_23_10_44_1419 "CA_Key_Get: %X %d"
#define CA_KEY_131_112_2_17_23_10_44_1420 "CA_Key_Get: find key"
#define ECM_40_112_2_17_23_10_44_1421 "MBBMS GetMikeyValidLength %d"
#define ECM_43_112_2_17_23_10_44_1422 "MBBMS GetMikeyValidLength MIKEY_GetCurrentPayloadLength failed"
#define ECM_54_112_2_17_23_10_44_1423 "MBBMS GetMikeyValidLength MIKEY_Open failed"
#define CA_ADAPTER_80_112_2_17_23_10_45_1424 "MBBMS GetECMDataType PARSING CAT ......\n"
#define CA_ADAPTER_95_112_2_17_23_10_45_1425 "MBBMS GetECMDataType PARSING CAT ......\n"
#define CA_ADAPTER_103_112_2_17_23_10_45_1426 "MBBSM GetECMDataType CA SYSTEM ID:0x%x\n"
#define CA_ADAPTER_109_112_2_17_23_10_45_1427 "MBBMS GetECMDataType data_type:0x%x ecm_transport_type:0x%xn"
#define CA_ADAPTER_145_112_2_17_23_10_45_1428 " ---MBBMS CA: ALREADY RUNNING\n"
#define CA_ADAPTER_153_112_2_17_23_10_45_1429 " ---MBBMS CA: BEGIN TO RUN ....   \n"
#define CA_ADAPTER_165_112_2_17_23_10_45_1430 " ---KDA CA:ENTRY TERMINATED!   \n"
#define CA_ADAPTER_172_112_2_17_23_10_45_1431 " ---KDA CA: TERMINATED!   \n"
#define CA_ADAPTER_190_112_2_17_23_10_45_1432 "MBBMS Set Media Type:%d \n"
#define CA_ADAPTER_209_112_2_17_23_10_45_1433 "MBBMS CAT SHOWING DATA:\n"
#define CA_ADAPTER_212_112_2_17_23_10_45_1434 "MBBMS CAT No:%d Value:0x%x\n"
#define CA_ADAPTER_218_112_2_17_23_10_45_1435 "MBBMS: cannot find ecm data type by system id %x"
#define CA_ADAPTER_277_112_2_17_23_10_45_1436 "CADSX_GetISMA: service_id %d frq_no %d"
#define CA_ADAPTER_299_112_2_17_23_10_45_1437 "MBBMS CA_Error_Callback: errcode %d [%d]"
#define CA_ADAPTER_306_112_2_17_23_10_45_1438 "MBBMS CA_Error_Callback: ca_mw_callback is null!"
#define CA_ADAPTER_311_112_2_17_23_10_45_1439 "MBBMS CA_Error_Callback: state error"
#define MULTIMEDIA_AL_108_112_2_17_23_10_46_1440 "Wait MainThread Exit"
#define MULTIMEDIA_AL_130_112_2_17_23_10_46_1441 "MultimediaAL_Play[%d]"
#define MULTIMEDIA_AL_146_112_2_17_23_10_46_1442 "MultimediaAL_Stop[%d]"
#define MULTIMEDIA_AL_191_112_2_17_23_10_46_1443 "MultimediaAL_GetCurrentFrameData: %d %dx%d"
#define MULTIMEDIA_AL_212_112_2_17_23_10_46_1444 "MultimediaAL_GetFrameSize: %d %d"
#define MULTIMEDIA_AL_346_112_2_17_23_10_46_1445 "DPLAYER_StreamRxStart failed!!!"
#define MULTIMEDIA_AL_379_112_2_17_23_10_46_1446 "*******Enter CalbackFrameEndNotify %d %d[%d]"
#define MULTIMEDIA_AL_482_112_2_17_23_10_47_1447 "Display_PARAM: w %d, h %d, angle %d"
#define MULTIMEDIA_AL_559_112_2_17_23_10_47_1448 "Wait FrameUpdateThread Exit"
#define MULTIMEDIA_AL_585_112_2_17_23_10_47_1449 "MMAL_MainThread: Recv %x[%d]"
#define MULTIMEDIA_AL_609_112_2_17_23_10_47_1450 "MMAL_MainThread unknow message %d"
#define MULTIMEDIA_AL_612_112_2_17_23_10_47_1451 "MMALMEM: main clean %p"
#define MULTIMEDIA_AL_633_112_2_17_23_10_47_1452 "MMAL_UpdateThread: Recv %x[%d]"
#define MULTIMEDIA_AL_644_112_2_17_23_10_47_1453 "MMAL_MainThread unknow message %d"
#define MULTIMEDIA_AL_647_112_2_17_23_10_47_1454 "MMALMEM: update clean %p"
#define MULTIMEDIA_AL_681_112_2_17_23_10_47_1455 "*******MultimediaAL Init failed"
#define MULTIMEDIA_AL_686_112_2_17_23_10_47_1456 "*******MultimediaAL Play failed"
#define MULTIMEDIA_AL_694_112_2_17_23_10_47_1457 "*******Open file for read error"
#define MULTIMEDIA_AL_711_112_2_17_23_10_47_1458 "*******Read header error"
#define MULTIMEDIA_AL_716_112_2_17_23_10_47_1459 "*******Read header length error"
#define MULTIMEDIA_AL_727_112_2_17_23_10_47_1460 "*******GetBuffer failed"
#define MULTIMEDIA_AL_736_112_2_17_23_10_47_1461 "*******Read frame error"
#define MULTIMEDIA_AL_740_112_2_17_23_10_47_1462 "*******Read frame length error"
#define MULTIMEDIA_AL_788_112_2_17_23_10_47_1463 "*******Open file for write error"
#define MULTIMEDIA_AL_793_112_2_17_23_10_47_1464 "*******Write file error"
#define MULTIMEDIA_AL_812_112_2_17_23_10_47_1465 "*******CreateThread failed"
#define MULTIMEDIA_IL_23_112_2_17_23_10_47_1466 "*******Open file for write error"
#define MULTIMEDIA_IL_30_112_2_17_23_10_47_1467 "*******Write file error"
#define MULTIMEDIA_IL_34_112_2_17_23_10_47_1468 "*******Write file length error"
#define MULTIMEDIA_IL_203_112_2_17_23_10_48_1469 "MMIL GetBuffer: when write buffer empty, read buffer should be available!!!"
#define MULTIMEDIA_IL_283_112_2_17_23_10_48_1470 "MultimediaIL_GetFrame: should not happen!!!"
#define MULTIMEDIA_IL_337_112_2_17_23_10_48_1471 "MultimediaIL_SetAudioSampleRate %d"
#define MULTIMEDIA_IL_344_112_2_17_23_10_48_1472 "MultimediaIL_GetLastOutputFramePTS %d"
#define SEQUENCERWBUFFER_53_112_2_17_23_10_48_1473 "SequenceRWBuffer: IsReadWriteSameSide null handle"
#define SEQUENCERWBUFFER_68_112_2_17_23_10_48_1474 "SequenceRWBuffer: IsGetWriteSameSide null handle"
#define SEQUENCERWBUFFER_92_112_2_17_23_10_48_1475 "SequenceRWBuffer: GetWritableRange null handle"
#define SEQUENCERWBUFFER_107_112_2_17_23_10_48_1476 "SequenceRWBuffer: FlipReadToStart null handle"
#define SEQUENCERWBUFFER_121_112_2_17_23_10_48_1477 "SequenceRWBuffer: FlipGetToStart null handle"
#define SEQUENCERWBUFFER_135_112_2_17_23_10_48_1478 "SequenceRWBuffer: FlipWriteToStart null handle"
#define SEQUENCERWBUFFER_149_112_2_17_23_10_48_1479 "SequenceRWBuffer: MarkBufferEnd null handle"
#define SEQUENCERWBUFFER_164_112_2_17_23_10_48_1480 "SequenceRWBuffer: IsReadBufferEnd null handle"
#define SEQUENCERWBUFFER_180_112_2_17_23_10_48_1481 "SequenceRWBuffer: IsGetBufferEnd null handle"
#define SEQUENCERWBUFFER_196_112_2_17_23_10_48_1482 "SequenceRWBuffer: IsGetBufferEmpty null handle"
#define SEQUENCERWBUFFER_218_112_2_17_23_10_49_1483 "SequenceRWBuffer: Init alloc fail"
#define SEQUENCERWBUFFER_223_112_2_17_23_10_49_1484 "SequenceRWBuffer: Init null base_ptr"
#define SEQUENCERWBUFFER_239_112_2_17_23_10_49_1485 "SequenceRWBuffer: Exit null handle"
#define SEQUENCERWBUFFER_253_112_2_17_23_10_49_1486 "SequenceRWBuffer: Reset null handle"
#define SEQUENCERWBUFFER_265_112_2_17_23_10_49_1487 "SequenceRWBuffer: GetReadBuffer start %p"
#define SEQUENCERWBUFFER_278_112_2_17_23_10_49_1488 "SequenceRWBuffer: GetReadBuffer %08x"
#define SEQUENCERWBUFFER_291_112_2_17_23_10_49_1489 "SequenceRWBuffer: GetReadBuffer end %p, %d"
#define SEQUENCERWBUFFER_296_112_2_17_23_10_49_1490 "SequenceRWBuffer: GetReadBuffer null handle"
#define SEQUENCERWBUFFER_312_112_2_17_23_10_49_1491 "SequenceRWBuffer: GetWriteBuffer start %p, %d"
#define SEQUENCERWBUFFER_317_112_2_17_23_10_49_1492 "SequenceRWBuffer: range %d"
#define SEQUENCERWBUFFER_350_112_2_17_23_10_49_1493 "SequenceRWBuffer: GetWriteBuffer end %p"
#define SEQUENCERWBUFFER_355_112_2_17_23_10_49_1494 "SequenceRWBuffer: GetWriteBuffer null handle"
#define SEQUENCERWBUFFER_369_112_2_17_23_10_49_1495 "SequenceRWBuffer: FinishUseReadBuffer start %p, %p"
#define SEQUENCERWBUFFER_393_112_2_17_23_10_49_1496 "SequenceRWBuffer: FinishUseReadBuffer end"
#define SEQUENCERWBUFFER_398_112_2_17_23_10_49_1497 "SequenceRWBuffer: FinishUseWriteBuffer null handle"
#define SEQUENCERWBUFFER_409_112_2_17_23_10_49_1498 "SequenceRWBuffer: FinishUseWriteBuffer start %p, %d"
#define SEQUENCERWBUFFER_417_112_2_17_23_10_49_1499 "SequenceRWBuffer: error MAGIC_NUM!!!"
#define SEQUENCERWBUFFER_422_112_2_17_23_10_49_1500 "SequenceRWBuffer: length should less or equal than the writable range!!!"
#define SEQUENCERWBUFFER_431_112_2_17_23_10_49_1501 "SequenceRWBuffer: FinishUseWriteBuffer end"
#define SEQUENCERWBUFFER_436_112_2_17_23_10_49_1502 "SequenceRWBuffer: FinishUseWriteBuffer null handle"
#define SEQUENCERWBUFFER_475_112_2_17_23_10_49_1503 "SequenceRWBuffer: SequenceRWBuffer_RemoveFirstReadBuffer null handle"
#define SEQUENCERWBUFFER_490_112_2_17_23_10_49_1504 "SequenceRWBuffer: SequenceRWBuffer_IsAllReadReleased null handle"
#define APDAC_51_112_2_17_23_10_49_1505 "APDAC_Init"
#define APDAC_74_112_2_17_23_10_49_1506 "APDAC_Open"
#define CA_ADAPTER_108_112_2_17_23_10_50_1507 "@@@@ add enter \n"
#define CA_ADAPTER_113_112_2_17_23_10_50_1508 "@@@@ NOT add enter \n"
#define CA_ADAPTER_157_112_2_17_23_10_50_1509 "TM_GetSysDate Failed!!!\n"
#define CA_ADAPTER_164_112_2_17_23_10_50_1510 "TM_GetSysTime FAILED!!!\n"
#define CA_ADAPTER_216_112_2_17_23_10_50_1511 " KDA number smartcard alarms:%d xNumberOfAccesAlarms:%dn"
#define CA_ADAPTER_219_112_2_17_23_10_50_1512 "KDA pxSmartcardAlarmArray alarm:%d changed:%d iccsessionid:%d"
#define CA_ADAPTER_225_112_2_17_23_10_50_1513 "KDA pxAccessAlarmArray alarm:%d changed:%d elementstreamid:%d,iccsessionid:%d service_id:%d tssession_id:%d"
#define CA_ADAPTER_230_112_2_17_23_10_50_1514 "KDA CAADAPTER: received system notification \n"
#define CA_ADAPTER_236_112_2_17_23_10_50_1515 "KDA CAADAPTER: received termination notification \n"
#define CA_ADAPTER_255_112_2_17_23_10_50_1516 " ---KDA CA: ALREADY RUNNING\n"
#define CA_ADAPTER_274_112_2_17_23_10_50_1517 "KDA ICC: I Know There Be One SD Card \n"
#define CA_ADAPTER_284_112_2_17_23_10_50_1518 " KDA_ICC: mcex_enter return MCEX_ERROR_DEVICE\n"
#define CA_ADAPTER_287_112_2_17_23_10_50_1519 " KDA_ICC: mcex_enter return: ifEnterMcex:%d \n"
#define CA_ADAPTER_295_112_2_17_23_10_50_1520 "KDA ICC: I Know There Be NO SD Card! \n"
#define CA_ADAPTER_301_112_2_17_23_10_50_1521 "--- KDA_ICC nagra card available \n"
#define CA_ADAPTER_306_112_2_17_23_10_50_1522 "--- KDA_ICC nagra card NOT available \n"
#define CA_ADAPTER_314_112_2_17_23_10_50_1523 "caInitialization finished!!"
#define CA_ADAPTER_318_112_2_17_23_10_50_1524 "caInitialization failed!"
#define CA_ADAPTER_372_112_2_17_23_10_50_1525 "Create ca adapter thread failed!!"
#define CA_ADAPTER_393_112_2_17_23_10_50_1526 " ---KDA CA: BEGIN TO RUN ....   \n"
#define CA_ADAPTER_411_112_2_17_23_10_51_1527 " KDA_ICC: FINDING.... T-Card NOT support CA. \n"
#define CA_ADAPTER_416_112_2_17_23_10_51_1528 " KDA_ICC: FINDING.... T-Card support CA. \n"
#define CA_ADAPTER_422_112_2_17_23_10_51_1529 " KDA_ICC: mcex_enter return MCEX_ERROR_DEVICE\n"
#define CA_ADAPTER_429_112_2_17_23_10_51_1530 "KDA ICC: Set Media Type:%d \n"
#define CA_ADAPTER_441_112_2_17_23_10_51_1531 "--- KDA_ICC nagra card available \n"
#define CA_ADAPTER_443_112_2_17_23_10_51_1532 " ---[KDA-CA][SetMediaType]: SMD IS SELECTED \n"
#define CA_ADAPTER_447_112_2_17_23_10_51_1533 "--- KDA_ICC nagra card NOT available \n"
#define CA_ADAPTER_466_112_2_17_23_10_51_1534 " KDA_ICC: mcex_enter return MCEX_ERROR_DEVICE\n"
#define CA_ADAPTER_469_112_2_17_23_10_51_1535 " KDA_ICC: mcex_enter return: ifEnterMcex:%d \n"
#define CA_ADAPTER_472_112_2_17_23_10_51_1536 " ---[KDA-CA][SetMediaType]: MicroSD IS SELECTED \n"
#define CA_ADAPTER_483_112_2_17_23_10_51_1537 " ---[KDA-CA][SetMediaType]: INVALID!  \n"
#define CA_ADAPTER_494_112_2_17_23_10_51_1538 " ---KDA CA:ENTRY TERMINATED!   \n"
#define CA_ADAPTER_497_112_2_17_23_10_51_1539 "---KDA CA: CA_Terminate gCaAdapterInited is FALSE \n"
#define CA_ADAPTER_512_112_2_17_23_10_51_1540 "---KDA CA: CA_Terminate systemrequesttable is in USE \n"
#define CA_ADAPTER_520_112_2_17_23_10_51_1541 "---KDA CA: CA_Terminate smartcardrequesttable is in USE\n"
#define CA_ADAPTER_529_112_2_17_23_10_51_1542 "---KDA CA: CA_Terminate programrequesttable is in USE\n"
#define CA_ADAPTER_542_112_2_17_23_10_51_1543 "---KDA CA: CA_Terminate RegisterationDispose num:%d Failed status:%d n"
#define CA_ADAPTER_553_112_2_17_23_10_51_1544 "---KDA CA: CA_Terminate RegisterationDispose try times = %d Failed status:%d n"
#define CA_ADAPTER_560_112_2_17_23_10_51_1545 "---KDA CA: CA_Terminate RegisterationDispose num:%d OK n"
#define CA_ADAPTER_569_112_2_17_23_10_51_1546 "---KDA CA: CA_Terminate caTermination() return FALSE \n"
#define CA_ADAPTER_584_112_2_17_23_10_51_1547 " ---KDA CA: TERMINATED!   \n"
#define CA_ADAPTER_591_112_2_17_23_10_51_1548 " KDA ADPT: ca_play_callback result:%d \n"
#define CA_ADAPTER_651_112_2_17_23_10_51_1549 "ca system get processing status FAILED subtype:%d!!!\n"
#define CA_ADAPTER_656_112_2_17_23_10_51_1550 "ca system get processing ERROR status  subtype:%d!!!\n"
#define CA_ADAPTER_661_112_2_17_23_10_51_1551 "ca system get object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_665_112_2_17_23_10_51_1552 "---CA ADapter: System get OBJECT num:%d\n"
#define CA_ADAPTER_669_112_2_17_23_10_51_1553 "ca_system_private_t memory alloc FAILED!!!\n"
#define CA_ADAPTER_681_112_2_17_23_10_51_1554 "ca system get CakVersin Failed subtype:%d!!!\n"
#define CA_ADAPTER_684_112_2_17_23_10_51_1555 "---CA ADapter: CB cak_version:%s"
#define CA_ADAPTER_689_112_2_17_23_10_51_1556 "ca system dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_697_112_2_17_23_10_51_1557 "ca system get IRDSrlNO FAILED subtype:%d!!!\n"
#define CA_ADAPTER_705_112_2_17_23_10_51_1558 "ca system Dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_712_112_2_17_23_10_51_1559 "ca system get project info FAILED subtype:%d!!!\n"
#define CA_ADAPTER_719_112_2_17_23_10_51_1560 "ca system dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_734_112_2_17_23_10_51_1561 "ca smartcard get processing status FAILED subtype:%d!!!\n"
#define CA_ADAPTER_739_112_2_17_23_10_51_1562 "ca smartcard get processing ERROR status subtype:%d!!!\n"
#define CA_ADAPTER_744_112_2_17_23_10_51_1563 "ca smartcard get object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_747_112_2_17_23_10_51_1564 "---CA ADapter: Smartcard get OBJECT num:%d\n"
#define CA_ADAPTER_752_112_2_17_23_10_51_1565 "smartcard alloc FAILED\n"
#define CA_ADAPTER_764_112_2_17_23_10_51_1566 "CA adapter ca smartcard get ecm system ID FAILED subtype:%d!!!\n"
#define CA_ADAPTER_770_112_2_17_23_10_51_1567 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_777_112_2_17_23_10_51_1568 "ca smartcard get object data FAILED subtype:%d!!!\n"
#define CA_ADAPTER_783_112_2_17_23_10_51_1569 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_790_112_2_17_23_10_51_1570 "ca smartcard get object data FAILED subtype:%d!!!\n"
#define CA_ADAPTER_796_112_2_17_23_10_51_1571 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_803_112_2_17_23_10_51_1572 "ca smartcard get object data FAILED subtype:%d!!!\n"
#define CA_ADAPTER_809_112_2_17_23_10_51_1573 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_817_112_2_17_23_10_51_1574 "ca smartcard get object data FAILED subtype:%d!!!\n"
#define CA_ADAPTER_823_112_2_17_23_10_51_1575 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_831_112_2_17_23_10_51_1576 "ca smartcard get object data FAILED subtype:%d!!!\n"
#define CA_ADAPTER_840_112_2_17_23_10_51_1577 "---CA ADapter: CB Smartcard Number:0x%2x %2x %2x %2x %2x %2x %2x %2x addr:%xn"
#define CA_ADAPTER_846_112_2_17_23_10_51_1578 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_855_112_2_17_23_10_51_1579 "ca smartcard get object data FAILED subtype:%d!!!\n"
#define CA_ADAPTER_867_112_2_17_23_10_51_1580 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_874_112_2_17_23_10_51_1581 "ca smartcard get object data FAILED subtype:%d!!!\n"
#define CA_ADAPTER_880_112_2_17_23_10_52_1582 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_888_112_2_17_23_10_52_1583 "ca smartcard get object data FAILED subtype:%d!!!\n"
#define CA_ADAPTER_894_112_2_17_23_10_52_1584 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_901_112_2_17_23_10_52_1585 "ca smartcard get object data FAILED subtype:%d!!!\n"
#define CA_ADAPTER_906_112_2_17_23_10_52_1586 "ca smartcard zipcode TOO LONG than 60 bytes. subtype:%d!!!\n"
#define CA_ADAPTER_913_112_2_17_23_10_52_1587 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_920_112_2_17_23_10_52_1588 "ca smartcard get object data  FAILED subtype:%d!!!\n"
#define CA_ADAPTER_926_112_2_17_23_10_52_1589 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_934_112_2_17_23_10_52_1590 "ca smartcard get object data FAILED subtype:%d!!!\n"
#define CA_ADAPTER_940_112_2_17_23_10_52_1591 "ca smartcard dispose object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_955_112_2_17_23_10_52_1592 "ca program get processing status FAILED subtype:%d!!!\n"
#define CA_ADAPTER_960_112_2_17_23_10_52_1593 "ca program get processing ERROR status subtype:%d!!!\n"
#define CA_ADAPTER_965_112_2_17_23_10_52_1594 "ca program get object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_968_112_2_17_23_10_52_1595 "---CA ADapter: program get OBJECT num:%d\n"
#define CA_ADAPTER_972_112_2_17_23_10_52_1596 "program alloc FAILED\n"
#define CA_ADAPTER_983_112_2_17_23_10_52_1597 "ca program get object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_989_112_2_17_23_10_52_1598 "ca program get object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_997_112_2_17_23_10_52_1599 "ca program get object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_1003_112_2_17_23_10_52_1600 "ca program get object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_1010_112_2_17_23_10_52_1601 "ca program get object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_1016_112_2_17_23_10_52_1602 "ca program get object FAILED subtype:%d!!!\n"
#define CA_ADAPTER_1099_112_2_17_23_10_52_1603 "ca system call Wrong subtype:%d!!!"
#define CA_ADAPTER_1105_112_2_17_23_10_52_1604 "ca system call subtype:%d request create failed!!!"
#define CA_ADAPTER_1115_112_2_17_23_10_52_1605 "ca system call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1123_112_2_17_23_10_52_1606 "ca system call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1131_112_2_17_23_10_52_1607 "ca system call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1144_112_2_17_23_10_52_1608 "ca system call subtype:%d send request  failed!!!"
#define CA_ADAPTER_1154_112_2_17_23_10_52_1609 "ca program call Wrong subtype:%d!!!"
#define CA_ADAPTER_1160_112_2_17_23_10_52_1610 "ca program call subtype:%d request create failed!!!"
#define CA_ADAPTER_1169_112_2_17_23_10_52_1611 "ca program call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1177_112_2_17_23_10_52_1612 "ca program call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1185_112_2_17_23_10_52_1613 "ca program call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1198_112_2_17_23_10_52_1614 "ca program call subtype:%d send request  failed!!!"
#define CA_ADAPTER_1208_112_2_17_23_10_52_1615 "ca smartcard call Wrong subtype:%d!!!"
#define CA_ADAPTER_1214_112_2_17_23_10_52_1616 "ca smartcard call subtype:%d request create failed!!!"
#define CA_ADAPTER_1223_112_2_17_23_10_52_1617 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1231_112_2_17_23_10_52_1618 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1239_112_2_17_23_10_52_1619 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1247_112_2_17_23_10_52_1620 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1255_112_2_17_23_10_52_1621 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1263_112_2_17_23_10_52_1622 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1271_112_2_17_23_10_52_1623 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1279_112_2_17_23_10_52_1624 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1287_112_2_17_23_10_52_1625 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1295_112_2_17_23_10_52_1626 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1303_112_2_17_23_10_52_1627 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1311_112_2_17_23_10_52_1628 "ca smartcard call subtype:%d request Set asyn response failed!!!"
#define CA_ADAPTER_1324_112_2_17_23_10_52_1629 "ca smartcard call subtype:%d send request  failed!!!"
#define CA_ADAPTER_1342_112_2_17_23_10_52_1630 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1360_112_2_17_23_10_52_1631 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1378_112_2_17_23_10_53_1632 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1396_112_2_17_23_10_53_1633 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1414_112_2_17_23_10_53_1634 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1431_112_2_17_23_10_53_1635 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1448_112_2_17_23_10_53_1636 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1465_112_2_17_23_10_53_1637 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1483_112_2_17_23_10_53_1638 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1501_112_2_17_23_10_53_1639 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1519_112_2_17_23_10_53_1640 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1537_112_2_17_23_10_53_1641 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1555_112_2_17_23_10_53_1642 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1573_112_2_17_23_10_53_1643 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1591_112_2_17_23_10_53_1644 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1609_112_2_17_23_10_53_1645 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1627_112_2_17_23_10_53_1646 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1645_112_2_17_23_10_53_1647 "ca system cakversion request pointer NOT MATCH!!!"
#define CA_ADAPTER_1664_112_2_17_23_10_53_1648 " ---CA ADapter: CB of CA_Get_Information type:%d subtype:%d return OK\n"
#define CA_ADAPTER_1670_112_2_17_23_10_53_1649 "---CA ADapter: CB cak_version:%s"
#define CA_ADAPTER_1673_112_2_17_23_10_53_1650 "---CA ADapter: CB ird_serial:%s"
#define CA_ADAPTER_1676_112_2_17_23_10_53_1651 "---CA ADapter: CB project_info:%s"
#define CA_ADAPTER_1679_112_2_17_23_10_53_1652 "adapter system CB ERROR subtype\n"
#define CA_ADAPTER_1688_112_2_17_23_10_53_1653 "---CA ADapter: CB ECM_SYSTEM_ID:%d\n"
#define CA_ADAPTER_1691_112_2_17_23_10_53_1654 "---CA ADapter: CB EMM_SYSTEM_ID:%d\n"
#define CA_ADAPTER_1694_112_2_17_23_10_53_1655 "---CA ADapter: CB SmartcardVERSION:%s\n"
#define CA_ADAPTER_1697_112_2_17_23_10_53_1656 "---CA ADapter: CB Smartcard SN:%s\n"
#define CA_ADAPTER_1701_112_2_17_23_10_53_1657 "---CA ADapter: CB Smartcard ID:0x%2x %2x %2x %2x %2x %2x n"
#define CA_ADAPTER_1707_112_2_17_23_10_53_1658 "---CA ADapter: CB Smartcard Number:0x%2x %2x %2x %2x %2x %2x %2x %2xn"
#define CA_ADAPTER_1712_112_2_17_23_10_53_1659 "---CA ADapter: CB Smartcard expir_date: %d-%d-%d -%d:%d:%d n"
#define CA_ADAPTER_1715_112_2_17_23_10_53_1660 "---CA ADapter: CB Smartcard provider_id:%d \n"
#define CA_ADAPTER_1719_112_2_17_23_10_53_1661 "---CA ADapter: CB Smartcard session_id:%d \n"
#define CA_ADAPTER_1722_112_2_17_23_10_53_1662 "---CA ADapter: CB Smartcard zip code:%s \n"
#define CA_ADAPTER_1725_112_2_17_23_10_53_1663 "---CA ADapter: CB Smartcard dela time:%d \n"
#define CA_ADAPTER_1728_112_2_17_23_10_53_1664 "---CA ADapter: CB Smartcard STATE:%d"
#define CA_ADAPTER_1731_112_2_17_23_10_53_1665 "adapter smartcard CB Notify/ERROR subtype\n"
#define CA_ADAPTER_1740_112_2_17_23_10_53_1666 "---CA ADapter: CB Program TS session id:%d\n"
#define CA_ADAPTER_1743_112_2_17_23_10_53_1667 "---CA ADapter: CB program number:%d\n"
#define CA_ADAPTER_1746_112_2_17_23_10_53_1668 "---CA ADapter: CB program access enum:%d\n"
#define CA_ADAPTER_1749_112_2_17_23_10_53_1669 "adapter program CB Notify/ERROR subtype\n"
#define CA_ADAPTER_1767_112_2_17_23_10_53_1670 " ---CA ADapter: call CA_Get_Information type:%d subtype:%d return ERROR\n"
#define CA_ADAPTER_1769_112_2_17_23_10_53_1671 " ---CA ADapter: call CA_Get_Information type:%d subtype:%d return OK\n"
#define CA_ADAPTER_1779_112_2_17_23_10_53_1672 " ---CA ADapter: call CA_Get_Information type:%d subtype:%d return ERROR\n"
#define CA_ADAPTER_1781_112_2_17_23_10_53_1673 " ---CA ADapter: call CA_Get_Information type:%d subtype:%d return OK\n"
#define CA_ADAPTER_1791_112_2_17_23_10_53_1674 " ---CA ADapter: call CA_Get_Information type:%d subtype:%d return ERROR\n"
#define CA_ADAPTER_1793_112_2_17_23_10_53_1675 " ---CA ADapter: call CA_Get_Information type:%d subtype:%d return OK\n"
#define CA_CCL_201_112_2_17_23_10_54_1676 "KDA CCL:TimerCallback  \n"
#define CA_CCL_213_112_2_17_23_10_54_1677 "KDA CCL:CCLActiveWatchTimer  \n"
#define CA_CCL_221_112_2_17_23_10_54_1678 "KDA CCL:CCLActiveWatchTimer  delete old timer\n"
#define CA_CCL_241_112_2_17_23_10_54_1679 "KDA CCL: CACCL_GetChannelStatus \n."
#define CA_CCL_247_112_2_17_23_10_54_1680 "KDA CCL: CACCL_GetChannelStatus error block number 0x%2x  \n."
#define CA_CCL_264_112_2_17_23_10_54_1681 "KDA CCL: CompareService serive_info.xProgramId=%d,setting.programID%d n"
#define CA_CCL_283_112_2_17_23_10_54_1682 "KDA CCL: AddService  s_ccl_service_count %d\n"
#define CA_CCL_288_112_2_17_23_10_54_1683 "CCL: AddService  i %d\n"
#define CA_CCL_308_112_2_17_23_10_54_1684 "KDA CCL: AddService  cat pxData[%d]%d\n"
#define CA_CCL_355_112_2_17_23_10_55_1685 "KDA CCL: AddService  s_ccl_service_count %d\n"
#define CA_CCL_368_112_2_17_23_10_55_1686 "KDA CCL_InitReg  %x\n"
#define CA_CCL_388_112_2_17_23_10_55_1687 "KDA CCL_RegisterCallback fun %x"
#define CA_CCL_403_112_2_17_23_10_55_1688 "KDA CCL_RegisterCallback RegistrationId 0x%x"
#define CA_CCL_439_112_2_17_23_10_55_1689 "KDA CCL_UnregisterCallback xRegistrationId 0x%x"
#define CA_CCL_451_112_2_17_23_10_55_1690 "KDA GetLastCCLReg  \n"
#define CA_CCL_475_112_2_17_23_10_55_1691 "KDA CCL:GetCurReg %d  \n"
#define CA_CCL_479_112_2_17_23_10_55_1692 "KDA CCL:GetCurrentReg  register id %d\n"
#define CA_CCL_506_112_2_17_23_10_55_1693 "KDA CCL GetCallback s_ccl_reg[i]  0x%x"
#define CA_CCL_526_112_2_17_23_10_55_1694 "KDA CCL:CACCL_CmlNotifyCCL \n"
#define CA_CCL_539_112_2_17_23_10_55_1695 "KDA CCL:CACCL_CmlNotifyCCL send signal to task,register_id 0x%x n"
#define CA_CCL_579_112_2_17_23_10_55_1696 "CACCL_PlayProgram tick 0x%x ,s_ccl_call_reg %d\n"
#define CA_CCL_583_112_2_17_23_10_55_1697 "KDA CACCL_PlayProgram pxServiceProvider 0x%x,net_id= 0x%x,ProgramId  0x%x"
#define CA_CCL_584_112_2_17_23_10_55_1698 "KDA CACCL_PlayProgram cat  0x%x,len =%d\n"
#define CA_CCL_618_112_2_17_23_10_55_1699 "KDA CCL CACCL_PlayProgram cat[%d]  = 0x%x\n"
#define CA_CCL_637_112_2_17_23_10_55_1700 "KDA CACCL_PlayProgram element num =%d ,element 0x%x n"
#define CA_CCL_670_112_2_17_23_10_55_1701 "KDA CACCL_PlayProgram xProgramId  %x"
#define CA_CCL_690_112_2_17_23_10_55_1702 "KDA CACCL_PlayProgram element num%d ,element%x n"
#define CA_CCL_748_112_2_17_23_10_56_1703 "KDA CACCL_StopProgram\n"
#define CA_CCL_764_112_2_17_23_10_56_1704 "KDA CACCL_SetPlayCallback func %x"
#define CA_CCL_778_112_2_17_23_10_56_1705 "KDA CACCL_IsPlayFlag %d \n"
#define CA_CCL_801_112_2_17_23_10_56_1706 "KDA CCL NotifyKDA cat= 0x%x,len %d ,tick= 0x%x\n"
#define CA_CCL_802_112_2_17_23_10_56_1707 "KDA CCL NotifyKDA xProgramId=  0x%x,xTransportSessionId= 0x%x,networkid= 0x%x \n"
#define CA_CCL_819_112_2_17_23_10_56_1708 "KDA CCL  NotifyKDA after callback tick 0x%x\n"
#define CA_CCL_832_112_2_17_23_10_56_1709 "KDA IsValidCCLRegistrationId xRegistrationId%d \n"
#define CA_CCL_902_112_2_17_23_10_56_1710 "KDA cclRegister xCallback  %x,tick 0x%x"
#define CA_CCL_909_112_2_17_23_10_56_1711 "KDA cclRegister s_ccl_call_reg  %d\n"
#define CA_CCL_913_112_2_17_23_10_56_1712 "KDA CCL:cmlRegister send signal to task\n"
#define CA_CCL_976_112_2_17_23_10_56_1713 "KDA cclReportAccess xAccess %d,xProgramId %x\n"
#define CA_CCL_985_112_2_17_23_10_56_1714 "KDA cclReportAccess err_block 0x%x \n"
#define CA_CCL_990_112_2_17_23_10_56_1715 "KDA cclReportAccess send denied  msg \n"
#define CA_CCL_996_112_2_17_23_10_56_1716 "KDA cclReportAccess need to verify \n"
#define CA_CCL_1076_112_2_17_23_10_56_1717 "KDA cclCancelRegistration xRegistrationId %d,tick 0x%x\n"
#define CA_CCL_1111_112_2_17_23_10_56_1718 "KDA CCL HandleReportAccessSig status%d,status_ptr%x \n"
#define CA_CCL_1113_112_2_17_23_10_56_1719 "KDA CCL HandleReportAccessSig s_ccl_operation_status%d \n"
#define CA_CCL_1115_112_2_17_23_10_56_1720 "KDA CCL HandleReportAccessSig s_ccl_current_prog.xProgramId %d \n"
#define CA_CCL_1120_112_2_17_23_10_56_1721 "KDA CCL HandleReportAccessSig call appcallback function \n"
#define CA_CCL_1250_112_2_17_23_10_57_1722 "KDA HandleAppAddProgSig programID=%d,is_set=%d,cur_programID=%dn"
#define CA_CCL_1253_112_2_17_23_10_57_1723 "KDA HandleAppAddProgSig element num%d ,element%x n"
#define CA_CCL_1258_112_2_17_23_10_57_1724 "KDA HandleAppAddProgSig s_ccl_abort_flag %d  n"
#define CA_CCL_1274_112_2_17_23_10_57_1725 "KDA HandleAppAddProgSig xTransportSessionId=0x%x ,s_ccl_call_reg %dn"
#define CA_CCL_1285_112_2_17_23_10_57_1726 "KDA HandleAppAddProgSig  no reg or wait for cml\n"
#define CA_CCL_1295_112_2_17_23_10_57_1727 "KDA HandleAppAddProgSig call NotifyKDA s_ccl_is_set_resume = %d\n"
#define CA_CCL_1335_112_2_17_23_10_57_1728 "KDA CCL GetService s_ccl_service_count%d\n"
#define CA_CCL_1345_112_2_17_23_10_57_1729 "KDA CCL GetService programid %d\n"
#define CA_CCL_1361_112_2_17_23_10_57_1730 "KDA CCL DelService s_ccl_service_count%d\n"
#define CA_CCL_1375_112_2_17_23_10_57_1731 "KDA CCL DelService pxData 0x%x\n"
#define CA_CCL_1382_112_2_17_23_10_57_1732 "KDA CCL DelService pxElementaryStreamTable 0x%x\n"
#define CA_CCL_1389_112_2_17_23_10_57_1733 "KDA CCL DelService pxServiceProvider 0x%x\n"
#define CA_CCL_1396_112_2_17_23_10_57_1734 "KDA CCL DelService programid %d\n"
#define CA_CCL_1440_112_2_17_23_10_57_1735 "KDA HandleAppChangeProgSig programID=%d,is_set=%d,cur_programID=%dn"
#define CA_CCL_1443_112_2_17_23_10_57_1736 "KDA HandleAppChangeProgSig element num%d ,element%x n"
#define CA_CCL_1459_112_2_17_23_10_57_1737 "KDA HandleAppChangeProgSig xTransportSessionId= 0x%x n"
#define CA_CCL_1480_112_2_17_23_10_57_1738 "KDA HandleAppChangeProgSig netid=%d \n"
#define CA_CCL_1513_112_2_17_23_10_57_1739 "KDA CACCL_About flag %d\n"
#define CA_CCL_1527_112_2_17_23_10_57_1740 "KDA CACCL_SetPlayFlag is_play %d\n"
#define CA_CCL_1544_112_2_17_23_10_57_1741 "KDA HandleAppRemoveProgSig programID %d ,s_ccl_add_service %d \n"
#define CA_CCL_1591_112_2_17_23_10_57_1742 "KDA HandleAppRemoveProgSig ABORT \n"
#define CA_CCL_1597_112_2_17_23_10_57_1743 "KDA HandleAppRemoveProgSig ABORT %d\n"
#define CA_CCL_1614_112_2_17_23_10_57_1744 "KDA CCL GetReg  reg_id 0x%x \n"
#define CA_CCL_1651_112_2_17_23_10_57_1745 "KDA HandleAppAddProgSig s_ccl_abort_flag %d  n"
#define CA_CCL_1653_112_2_17_23_10_57_1746 "KDA HandleCCLRegisterSig HandleCCLRegisterSig 0x%x,cml_added %d\n"
#define CA_CCL_1658_112_2_17_23_10_57_1747 "KDA HandleCCLRegisterSig wait for cml add  \n"
#define CA_CCL_1668_112_2_17_23_10_57_1748 "KDA HandleCCLRegisterSig no service \n"
#define CA_CCL_1675_112_2_17_23_10_57_1749 "KDA HandleCCLRegisterSig no reg or s_ccl_call_reg %d \n"
#define CA_CCL_1693_112_2_17_23_10_57_1750 "KDA HandleCCLRegisterSig xTransportSessionId 0x%x \n"
#define CA_CCL_1722_112_2_17_23_10_57_1751 "KDA HandleCCLRegisterSig after NotifyKDA xTransportSessionId 0x%x \n"
#define CA_CCL_1744_112_2_17_23_10_58_1752 "KDA CCL CCLThreadEntry task_id%d \n"
#define CA_CCL_1750_112_2_17_23_10_58_1753 "CCL sig_ptr->SignalCode%d \n"
#define CA_CCL_1796_112_2_17_23_10_58_1754 "KDA CCL:CACCL_IsFilterForEcm service_id 0x%x ,s_ccl_play_flag %d ,s_ccl_set_prog.xProgramId 0x%x\n"
#define CA_CCL_1797_112_2_17_23_10_58_1755 "KDA CCL:CACCL_IsFilterForEcm session_id 0x%x ,s_ccl_set_prog.xTransportSessionId 0x%x\n"
#define CA_CCL_1816_112_2_17_23_10_58_1756 "KDA CACCL_Init \n"
#define CA_CCL_1833_112_2_17_23_10_58_1757 "CCL CACCL_Init task_id%d \n"
#define CA_CCL_1863_112_2_17_23_10_58_1758 "KDA CACCL_Exit \n"
#define CA_CCL_1906_112_2_17_23_10_58_1759 "KDA CCL_Exit: fail to delete  thread .\n "
#define CA_CCL_1929_112_2_17_23_10_58_1760 "KDA CACCL_IsExitFlag \n "
#define CA_CCL_1941_112_2_17_23_10_58_1761 "KDA CACCL_IsExitFlag no reg\n "
#define CA_CCL_1950_112_2_17_23_10_58_1762 "KDA CACCL_SetResume is_set_resume %d\n "
#define CA_CCL_1985_112_2_17_23_10_58_1763 "KDA CCL:CACCL_Test  \n"
#define CA_CCL_2023_112_2_17_23_10_58_1764 "KDA CCL:CACCL_Test1  \n"
#define CA_CCL_2039_112_2_17_23_10_58_1765 "KDA CCL:CACCL_Test2  \n"
#define CA_CML_128_112_2_17_23_10_58_1766 "KDA CML:TimerCallback  \n"
#define CA_CML_137_112_2_17_23_10_58_1767 "KDA CML:ActiveWatchTimer  \n"
#define CA_CML_145_112_2_17_23_10_58_1768 "KDA CML:ActiveWatchTimer  delete old timer\n"
#define CA_CML_162_112_2_17_23_10_59_1769 "KDA GetLastReg  \n"
#define CA_CML_188_112_2_17_23_10_59_1770 "KDA CML_InitReg  \n"
#define CA_CML_198_112_2_17_23_10_59_1771 "CML:GetCurReg %d  \n"
#define CA_CML_202_112_2_17_23_10_59_1772 "CML:GetCurrentReg  register id %d\n"
#define CA_CML_227_112_2_17_23_10_59_1773 "CML:CML_RegisterCallback  fun %d\n"
#define CA_CML_241_112_2_17_23_10_59_1774 "CML:CML_RegisterCallback  register id %d\n"
#define CA_CML_270_112_2_17_23_10_59_1775 "KDA CML:CML_UnregisterCallback  xRegistrationId %d\n"
#define CA_CML_305_112_2_17_23_10_59_1776 "KDA CML:GetCallback  s_cml_reg[%d].register_id%d\n"
#define CA_CML_321_112_2_17_23_10_59_1777 "CML: CompareCAT cat_info.xDataSize%d \n"
#define CA_CML_330_112_2_17_23_10_59_1778 "CML: CompareCAT cat_info.pxData%d,cat %d \n"
#define CA_CML_349_112_2_17_23_10_59_1779 "CML: AddCAT  s_cat_total_count%d\n"
#define CA_CML_356_112_2_17_23_10_59_1780 "KDA CML: AddCAT  i%d\n"
#define CA_CML_385_112_2_17_23_10_59_1781 "KDA CML: AddCAT  s_cat_total_count%d\n"
#define CA_CML_404_112_2_17_23_10_59_1782 "KDA CML: GetCat \n"
#define CA_CML_411_112_2_17_23_10_59_1783 "KDA CML: GetCat len %d\n"
#define CA_CML_424_112_2_17_23_10_59_1784 "KDA CML: CmlNotifyKDA  cmd %d,cat len %d,cat 0x%x ,xTransportSessionId 0x%x\n"
#define CA_CML_434_112_2_17_23_10_59_1785 "KDA CML:CmlNotifyKDA callback is PNULL\n"
#define CA_CML_447_112_2_17_23_10_59_1786 "KDA CML:CmlNotifyKDA after callback\n"
#define CA_CML_459_112_2_17_23_10_59_1787 "KDA CML:IsRemoveEMM s_cml_call_reg %d,filter_count %d ,s_cml_wait %d\n"
#define CA_CML_499_112_2_17_23_10_59_1788 "KDA CML:CACML_STOP s_cml_change_plch %d\n"
#define CA_CML_522_112_2_17_23_10_59_1789 "KDA CML:CACML_STOP no reg \n"
#define CA_CML_579_112_2_17_23_10_59_1790 "KDA CML:CACML_ChangePLCH  cat%d,len%d\n"
#define CA_CML_597_112_2_17_23_10_59_1791 "KDA CML:CACML_ChangePLCH  s_cml_call_reg %d\n"
#define CA_CML_627_112_2_17_23_10_59_1792 "KDA CML:CACML_ChangePLCH  xTransportSessionId%d\n"
#define CA_CML_666_112_2_17_23_10_59_1793 "KDA CML:IsValidCMLRegistrationId  xRegistrationId%d\n"
#define CA_CML_724_112_2_17_23_11_0_1794 "KDA CML:cmlRegister  xCallback %d\n"
#define CA_CML_736_112_2_17_23_11_0_1795 "KDA CML:cmlRegister send signal to task\n"
#define CA_CML_795_112_2_17_23_11_0_1796 "KDA CML:cmlCancelRegistration  xRegistrationId %d\n"
#define CA_CML_807_112_2_17_23_11_0_1797 "KDA CML:cmlCancelRegistration  s_cml_is_add %d\n"
#define CA_CML_823_112_2_17_23_11_0_1798 "KDA CML:CML_Test  \n"
#define CA_CML_849_112_2_17_23_11_0_1799 "KDA CML HandleCmlRegisterSig  \n"
#define CA_CML_855_112_2_17_23_11_0_1800 "KDA CML HandleCmlRegisterSig no cat \n"
#define CA_CML_862_112_2_17_23_11_0_1801 "KDA CML HandleCmlRegisterSig no register or s_cml_call_reg %d\n"
#define CA_CML_877_112_2_17_23_11_0_1802 "KDA CML:HandleCmlRegisterSig  xTransportSessionId 0x%x\n"
#define CA_CML_891_112_2_17_23_11_0_1803 "KDA CML:HandleCmlRegisterSig  after CmlNotifyKDA xTransportSessionId 0x%x\n"
#define CA_CML_912_112_2_17_23_11_0_1804 "KDA CML CMLThreadEntry task_id%d \n"
#define CA_CML_918_112_2_17_23_11_0_1805 "KDA CML sig_ptr->SignalCode%d \n"
#define CA_CML_942_112_2_17_23_11_0_1806 "KDA CML CACML_GetAddFlag %d\n"
#define CA_CML_957_112_2_17_23_11_0_1807 "KDA CML CACML_ClearValue\n"
#define CA_CML_970_112_2_17_23_11_0_1808 "KDA CACML_IsExitFlag \n "
#define CA_CML_982_112_2_17_23_11_0_1809 "KDA CACML_IsExitFlag no reg\n "
#define CA_CML_994_112_2_17_23_11_0_1810 "KDA CML: CACML_IsFilterForEmm  s_cml_mutex_flag %d \n"
#define CA_CML_995_112_2_17_23_11_0_1811 "KDA CML: CACML_IsFilterForEmm session_id 0x%x ,s_cml_current_prog.xTransportSessionId 0x%x\n"
#define CA_CML_1013_112_2_17_23_11_0_1812 "KDA CML:CML_Init\n"
#define CA_CML_1031_112_2_17_23_11_0_1813 "KDA CML CACML_Init task_id%d \n"
#define CA_CML_1070_112_2_17_23_11_0_1814 "KDA CML:CML_Exit\n"
#define CA_CML_1084_112_2_17_23_11_0_1815 "KDA CML_Exit: fail to delete  thread .\n "
#define CA_CSL_150_112_2_17_23_11_1_1816 "KDA CSL:CACSL_SetPlayingServiceId xServiceId 0x%x,s_csl_filter_is_start %d n"
#define CA_CSL_162_112_2_17_23_11_1_1817 "KDA CSL:SetFilter xServiceId 0x%x, playing ServiceId 0x%x,is_start %d n"
#define CA_CSL_171_112_2_17_23_11_1_1818 "KDA CSL:SetFilter start s_csl_filter_is_start 0x%xn"
#define CA_CSL_177_112_2_17_23_11_1_1819 "KDA CSL:SetFilter start s_csl_emm_filter_count 0x%xn"
#define CA_CSL_192_112_2_17_23_11_1_1820 "KDA CSL:CACSL_SetFilter s_csl_filter_is_start 0x%x n"
#define CA_CSL_204_112_2_17_23_11_1_1821 "KDA CSL:CACSL_GetEmmFilterCount  s_csl_emm_filter_count 0x%x n"
#define CA_CSL_220_112_2_17_23_11_1_1822 "KDA CSL:CACSL_SetEmmFilterCount  s_csl_emm_filter_count 0x%x n"
#define CA_CSL_234_112_2_17_23_11_1_1823 "KDA CSL:CACSL_GetFilterStartFlag  s_csl_filter_is_start 0x%x n"
#define CA_CSL_248_112_2_17_23_11_1_1824 "KDA CSL:AddFilterToList filter_id%d,xFilterDepth%d \n"
#define CA_CSL_282_112_2_17_23_11_1_1825 "KDA CSL: GetFilterInfo,filter_id%d \n"
#define CA_CSL_289_112_2_17_23_11_1_1826 "KDA CSL: GetFilterInfo filter_ptr%x \n"
#define CA_CSL_302_112_2_17_23_11_1_1827 "KDA CSL: FilterTimeoutCallback filter_ptr= 0x%x \n"
#define CA_CSL_321_112_2_17_23_11_1_1828 "KDA CSL: FilterTimeoutCallback finish \n"
#define CA_CSL_339_112_2_17_23_11_1_1829 "KDA CSL:FilterTimerCallback param%x\n"
#define CA_CSL_395_112_2_17_23_11_1_1830 "KDA CSL:CSL_ClearFilter enable %d, filter_id %d\n"
#define CA_CSL_431_112_2_17_23_11_1_1831 "KDA CSL: CACSL_RcvFilterData filter_id 0x%x ,pxSection 0x%x,xSectionSize 0x%x \n"
#define CA_CSL_439_112_2_17_23_11_1_1832 "KDA CSL:CACSL_SetCompareFlag is_comp =0x%x,\n"
#define CA_CSL_473_112_2_17_23_11_1_1833 "KDA CSL:IsCallFilterCallback \n"
#define CA_CSL_482_112_2_17_23_11_1_1834 "KDA CSL:IsCallFilterCallback return false \n"
#define CA_CSL_501_112_2_17_23_11_1_1835 "KDA CSL:CACSL_FilterCallback filter_id =0x%x,pxSection= 0x%x,xSectionSize =0x%x\n"
#define CA_CSL_502_112_2_17_23_11_1_1836 "KDA CSL:CACSL_FilterCallback xQueryBufferCallback=0x%x,xReceivedSectionCallback= 0x%x\n"
#define CA_CSL_506_112_2_17_23_11_1_1837 "KDA CSL:CACSL_FilterCallback filter_ptr == PNULL\n"
#define CA_CSL_512_112_2_17_23_11_1_1838 "KDA CSL:CACSL_FilterCallback don't call callback \n"
#define CA_CSL_519_112_2_17_23_11_1_1839 "KDA CSL:CACSL_FilterCallback s_csl_comp_ecm.is_comp =0x%x,filter_ptr->filter.xServiceId= 0x%x\n"
#define CA_CSL_543_112_2_17_23_11_2_1840 "KDA CSL:CACSL_FilterCallback save first ecm xSectionSize 0x%x,\n"
#define CA_CSL_553_112_2_17_23_11_2_1841 "KDA CSL:CACSL_FilterCallback same ecm\n"
#define CA_CSL_571_112_2_17_23_11_2_1842 "KDA CSL:CACSL_FilterCallback s_csl_comp_ecm.ecm_len != xSectionSize \n"
#define CA_CSL_579_112_2_17_23_11_2_1843 "KDA CSL:CACSL_FilterCallback different ecm\n"
#define CA_CSL_608_112_2_17_23_11_2_1844 "KDA CSL:CACSL_FilterCallback after xQueryBufferCallback pxBuffer=0x%x,xBufferSize =0x%x\n"
#define CA_CSL_618_112_2_17_23_11_2_1845 "KDA CSL:CACSL_FilterCallback callback tick 0x%x\n"
#define CA_CSL_619_112_2_17_23_11_2_1846 "KDA CSL:CACSL_FilterCallback ecm content :\n"
#define CA_CSL_629_112_2_17_23_11_2_1847 "KDA CSL:CACSL_FilterCallback ecm content >>[0x%04x]:  %02x  %02x  %02x  %02x  %02x  %02x  %02x  %02xn"
#define CA_CSL_645_112_2_17_23_11_2_1848 "KDA CSL:CACSL_FilterCallback ecm content >>[0x%04x]:%s"
#define CA_CSL_662_112_2_17_23_11_2_1849 "KDA CSL:CACSL_FilterCallback size of data very long \n"
#define CA_CSL_674_112_2_17_23_11_2_1850 "KDA CSL:CACSL_FilterCallback finish\n"
#define CA_CSL_723_112_2_17_23_11_2_1851 "KDA CSL:cslFilterOpen pxFilterId 0x%x\n"
#define CA_CSL_737_112_2_17_23_11_2_1852 "KDA CSL:cslFilterOpen filter_id 0x%x\n"
#define CA_CSL_774_112_2_17_23_11_2_1853 "KDA CSL:cslFilterClose filter_id 0x%x\n"
#define CA_CSL_867_112_2_17_23_11_2_1854 "KDA CSL:cslFilterSetPatterns filter_id = 0x%x\n"
#define CA_CSL_892_112_2_17_23_11_2_1855 "KDA CSL:cslFilterSetPatterns data_len 0x%x,sub_type 0x%x\n"
#define CA_CSL_896_112_2_17_23_11_2_1856 "KDA CSL:cslFilterSetPatterns pxMatch[%d] 0x%x\n"
#define CA_CSL_902_112_2_17_23_11_2_1857 "KDA CSL:cslFilterSetPatterns pxEqualMask[%d] 0x%x\n"
#define CA_CSL_908_112_2_17_23_11_2_1858 "KDA CSL:cslFilterSetPatterns pxNotEqualMask[%d] 0x%x\n"
#define CA_CSL_922_112_2_17_23_11_2_1859 "KDA CSL:cslFilterSetPatterns MTV_STIMI_DATA_128BYTE_FILTER\n"
#define CA_CSL_929_112_2_17_23_11_2_1860 "KDA CSL:cslFilterSetPatterns MTV_STIMI_DATA_16BYTE_FILTER\n"
#define CA_CSL_941_112_2_17_23_11_2_1861 "KDA CSL:cslFilterSetPatterns MTV_STIMI_DATA_8BYTE_FILTER\n"
#define CA_CSL_1041_112_2_17_23_11_3_1862 "KDA CSL:cslFilterStart filter_id =0x%x , tick 0x%x ,xTransportSessionId 0x%x\n"
#define CA_CSL_1044_112_2_17_23_11_3_1863 "KDA CSL:cslFilterStart xTimeOut%d\n"
#define CA_CSL_1046_112_2_17_23_11_3_1864 "KDA CSL:cslFilterStart xQueryBufferCallback%x\n"
#define CA_CSL_1047_112_2_17_23_11_3_1865 "KDA CSL:cslFilterStart xReceivedSectionCallback%x\n"
#define CA_CSL_1048_112_2_17_23_11_3_1866 "KDA CSL:cslFilterStart xLoopMode%d\n"
#define CA_CSL_1066_112_2_17_23_11_3_1867 "KDA CSL:cslFilterStart filter_ptr== PNULL\n"
#define CA_CSL_1093_112_2_17_23_11_3_1868 "KDA CSL:cslFilterStart xFilterId->timer== PNULL ,to create timer \n"
#define CA_CSL_1108_112_2_17_23_11_3_1869 "KDA CSL:cslFilterStart xDataType 0x%x\n"
#define CA_CSL_1117_112_2_17_23_11_3_1870 "KDA CSL:cslFilterStart enablefilter tick 0x%x\n"
#define CA_CSL_1157_112_2_17_23_11_3_1871 "KDA CSL:cslFilterStop filter_id 0x%x ,status %d\n"
#define CA_CSL_1175_112_2_17_23_11_3_1872 "KDA CSL:cslFilterStop xTimeout 0x%x,timer 0x%x\n"
#define CA_CSL_1251_112_2_17_23_11_3_1873 "KDA CSL:cslFilterSetPacketId filter_id =0x%x,xServiceId =0x%x\n"
#define CA_CSL_1269_112_2_17_23_11_3_1874 "KDA CSL:cslFilterSetPacketId xDataType = 0x%x\n"
#define CA_CSL_1283_112_2_17_23_11_3_1875 "KDA CSL:cslFilterSetPacketId  successful\n"
#define CA_CSL_1286_112_2_17_23_11_3_1876 "KDA CSL:cslFilterSetPacketId fail \n"
#define CA_CSL_1311_112_2_17_23_11_3_1877 "KDA CSL:CSLThreadEntry  SignalCode%x\n"
#define CA_CSL_1340_112_2_17_23_11_3_1878 "KDA CSL: IsExitFlag \n "
#define CA_CSL_1352_112_2_17_23_11_3_1879 "KDA CSL: IsExitFlag  no filter\n "
#define CA_CSL_1371_112_2_17_23_11_3_1880 "CCL ProcessData buffer_id %d \n"
#define CA_CSL_1384_112_2_17_23_11_3_1881 "KDA CSL: ProcessDemuxData,no enough memory %d!"
#define CA_CSL_1416_112_2_17_23_11_3_1882 "CCL CADataCallback buffer_id %d \n"
#define CA_CSL_1449_112_2_17_23_11_3_1883 "CCL CACSL_IsRegisterCallback is_reg %d \n"
#define CA_CSL_1474_112_2_17_23_11_3_1884 "KDA CACSL_Init \n"
#define CA_CSL_1495_112_2_17_23_11_3_1885 "CCL CACSL_Init task_id%d \n"
#define CA_CSL_1512_112_2_17_23_11_3_1886 "KDA CACSL_Exit \n"
#define CA_CSL_1524_112_2_17_23_11_3_1887 "KDA CACSL_Exit: fail to delete  thread .\n "
#define CA_CSL_2104_112_2_17_23_11_5_1888 "__KDA_TEST_ECM: No of ECM:%d  \n"
#define CA_CSL_2109_112_2_17_23_11_5_1889 "__KDA_TEST_ECM: Filter Available \n"
#define CA_CSL_2114_112_2_17_23_11_5_1890 "__KDA_TEST_ECM: Data has been calledback \n"
#define CA_CSL_2169_112_2_17_23_11_5_1891 "__KDA_TEST_ECM: No Filter Available \n"
#define CA_CSL_2180_112_2_17_23_11_5_1892 "KDA CSL:TestTimerCallback  \n"
#define CA_CSL_2189_112_2_17_23_11_5_1893 "KDA CSL:test_csl  \n"
#define CA_DSX_171_112_2_17_23_11_5_1894 "KDA DSX:SetIsma element_id %d\n"
#define CA_DSX_177_112_2_17_23_11_5_1895 "KDA DSX:SetIsma ismacrypData.salt[%d] %d\n"
#define CA_DSX_198_112_2_17_23_11_5_1896 "KDA DSX:SetIsma rtpData.auSizeLengthType %x\n"
#define CA_DSX_246_112_2_17_23_11_5_1897 "KDA DSX:SetIsma isma_data[i] %x\n"
#define CA_DSX_263_112_2_17_23_11_5_1898 "KDA DSX: :CADSX_GetISMA \n"
#define CA_DSX_300_112_2_17_23_11_5_1899 "KDA DSX: :CADSX_SetISMA element_type %d\n"
#define CA_DSX_341_112_2_17_23_11_5_1900 "KDA DSX :InitObj  \n"
#define CA_DSX_358_112_2_17_23_11_6_1901 "KDA DSX: :CreateObj xProgramId %x \n"
#define CA_DSX_388_112_2_17_23_11_6_1902 "KDA DSX :CreateObj  ProgramId%x,xElementaryStreamId%x,xNumKeys%x,xKeyLength%x\n"
#define CA_DSX_421_112_2_17_23_11_6_1903 "KDA DSX :DeleteObj  %d,pxKeyTable = 0x%x\n"
#define CA_DSX_436_112_2_17_23_11_6_1904 "KDA DSX :DeleteObj pxKeyTable.pValue = 0x%x\n"
#define CA_DSX_468_112_2_17_23_11_6_1905 "KDA DSX :GetCurObj   \n"
#define CA_DSX_479_112_2_17_23_11_6_1906 "KDA DSX :GetCurObj  s_dsx_obj[i].obj.is_set_isma %x , s_dsx_obj[i].is_config_isma %x \n"
#define CA_DSX_486_112_2_17_23_11_6_1907 "KDA DSX :GetCurObj i=%d \n"
#define CA_DSX_513_112_2_17_23_11_6_1908 "KDA DSX :GetObj  pxId %x,xElementaryStreamId = %d\n"
#define CA_DSX_523_112_2_17_23_11_6_1909 "KDA DSX :GetObj i=%d \n"
#define CA_DSX_537_112_2_17_23_11_6_1910 "KDA DSX:IsZeroIndex s_dsx_key_index_len %d ,index =0x%x\n"
#define CA_DSX_595_112_2_17_23_11_6_1911 "KDA DSX:GetSetObj index%d \n"
#define CA_DSX_598_112_2_17_23_11_6_1912 "KDA DSX:GetSetObj  index_to_find%x\n"
#define CA_DSX_615_112_2_17_23_11_6_1913 "KDA DSX:GetSetObj  obj_ptr->obj.pxKeyTable[i]%x\n"
#define CA_DSX_619_112_2_17_23_11_6_1914 "KDA DSX:GetSetObj  ismacrypData.index %x\n"
#define CA_DSX_638_112_2_17_23_11_6_1915 "KDA DSX:GetSetObj  i%x,arr_index %x\n"
#define CA_DSX_664_112_2_17_23_11_6_1916 "KDA DSX :GetKeyIndex key_index%x \n"
#define CA_DSX_671_112_2_17_23_11_6_1917 "KDA DSX :GetKeyIndex arr_index%x \n"
#define CA_DSX_694_112_2_17_23_11_6_1918 "KDA DSX :HandleDescramKeyReqSig key_select%x,key_id%x \n"
#define CA_DSX_716_112_2_17_23_11_6_1919 "KDA DSX :HandleDescramKeyReqSig xSetNumKeys%x \n"
#define CA_DSX_720_112_2_17_23_11_6_1920 "KDA DSX :HandleDescramKeyReqSig key[%d]%x \n"
#define CA_DSX_734_112_2_17_23_11_6_1921 "KDA DSX :HandleDescramKeyReqSig key index%x \n"
#define CA_DSX_748_112_2_17_23_11_6_1922 "KDA DSX :HandleDescramKeyReqSig don't get key key_rcv_demux_cmd_ref 0x%x\n"
#define CA_DSX_761_112_2_17_23_11_6_1923 "KDA DSX:HandleISMAReqSig \n"
#define CA_DSX_799_112_2_17_23_11_6_1924 "KDA DSX: :dsxCreate xProgramId %x \n"
#define CA_DSX_819_112_2_17_23_11_6_1925 "KDA DSX: :dsxCreate success obj.pxId%x \n"
#define CA_DSX_840_112_2_17_23_11_6_1926 "KDA DSX:dsxDispose \n"
#define CA_DSX_872_112_2_17_23_11_6_1927 "KDA DSX:CompareISMAandRTPParam xElementaryStreamId %d\n"
#define CA_DSX_882_112_2_17_23_11_7_1928 "KDA DSX:CompareISMAandRTPParam newsalt 0x%x, oldsalt 0x%xn"
#define CA_DSX_886_112_2_17_23_11_7_1929 "KDA DSX:CompareISMAandRTPParam newsalt 0x%x, oldsalt 0x%xn"
#define CA_DSX_918_112_2_17_23_11_7_1930 "KDA DSX:CompareISMAandRTPParam newsalt[%d] 0x%x, oldsalt[%d] 0x%xn"
#define CA_DSX_945_112_2_17_23_11_7_1931 "KDA DSX:CompareISMAandRTPParam initializationVectorLength %x ,%xn"
#define CA_DSX_948_112_2_17_23_11_7_1932 "KDA DSX:CompareISMAandRTPParam deltaInitializationVectorLength %x, %xn"
#define CA_DSX_951_112_2_17_23_11_7_1933 "KDA DSX:CompareISMAandRTPParam keyIndicatorLength %x ,%xn"
#define CA_DSX_954_112_2_17_23_11_7_1934 "KDA DSX:CompareISMAandRTPParam useKeyIndicatorPerAu %x,%xn"
#define CA_DSX_957_112_2_17_23_11_7_1935 "KDA DSX:CompareISMAandRTPParam useSelectiveEncryption %x,%xn"
#define CA_DSX_963_112_2_17_23_11_7_1936 "KDA DSX:CompareISMAandRTPParam useRandomAccessFlag %x,%xn"
#define CA_DSX_966_112_2_17_23_11_7_1937 "KDA DSX:CompareISMAandRTPParam streamStateLength %x,%xn"
#define CA_DSX_970_112_2_17_23_11_7_1938 "KDA DSX:CompareISMAandRTPParam indexDeltaLength %x,%xn"
#define CA_DSX_974_112_2_17_23_11_7_1939 "KDA DSX:CompareISMAandRTPParam indexLength%x,%xn"
#define CA_DSX_978_112_2_17_23_11_7_1940 "KDA DSX:CompareISMAandRTPParam auxiliaryDataSizeLength %x,%xn"
#define CA_DSX_981_112_2_17_23_11_7_1941 "KDA DSX:CompareISMAandRTPParam ctsDeltaLength %x,%xn"
#define CA_DSX_984_112_2_17_23_11_7_1942 "KDA DSX:CompareISMAandRTPParam useRandomAccessFlag %x,%xn"
#define CA_DSX_991_112_2_17_23_11_7_1943 "KDA DSX:CompareISMAandRTPParam newsalt 0x%x, oldsalt 0x%xn"
#define CA_DSX_1001_112_2_17_23_11_7_1944 "KDA DSX:CompareISMAandRTPParam param is diffrent\n"
#define CA_DSX_1037_112_2_17_23_11_7_1945 "KDA DSX:dsxConfigure xId %d,tick 0x%x,type %d\n"
#define CA_DSX_1051_112_2_17_23_11_7_1946 "KDA DSX:dsxConfigure s_dsx_key_index_len %d \n"
#define CA_DSX_1058_112_2_17_23_11_7_1947 "KDA DSX:dsxConfigure same param \n"
#define CA_DSX_1062_112_2_17_23_11_7_1948 "KDA DSX:dsxConfigure isma_used_count%d,isma_rcv_demux_cmd_ref%d,isma_total_count%d\n"
#define CA_DSX_1067_112_2_17_23_11_7_1949 "KDA DSX:dsxConfigure isma_used_count %d \n"
#define CA_DSX_1092_112_2_17_23_11_7_1950 "KDA DSX:DsxSaveKey index 0x%x\n"
#define CA_DSX_1094_112_2_17_23_11_7_1951 "KDA DSX:DsxSaveKey key_used_count 0x%x, key_rcv_demux_cmd_ref 0x0x\n"
#define CA_DSX_1095_112_2_17_23_11_7_1952 "KDA DSX:DsxSaveKey pxKeyTable 0x%x\n"
#define CA_DSX_1103_112_2_17_23_11_7_1953 "KDA DSX :DsxSaveKey pxKeyTable.pValue = 0x%x\n"
#define CA_DSX_1158_112_2_17_23_11_7_1954 "KDA DSX:DsxSaveKey key index 0x%x\n"
#define CA_DSX_1173_112_2_17_23_11_7_1955 "KDA DSX:dsxSetKeys key_used_count%d,key_rcv_demux_cmd_ref%d,key_total_count%d\n"
#define CA_DSX_1185_112_2_17_23_11_7_1956 "KDA DSX:CompareAndSaveKey pxKeyTable =0x%x,index = %x\n"
#define CA_DSX_1191_112_2_17_23_11_7_1957 "KDA DSX:CompareAndSaveKey begin to set key\n"
#define CA_DSX_1204_112_2_17_23_11_7_1958 "KDA DSX:CompareKey xNumKeys%x,xSetNumKeys%x\n"
#define CA_DSX_1213_112_2_17_23_11_7_1959 "KDA DSX:CompareKey ismacrypData.index %x pxKeyTable.data.ismacrypData.index %x\n"
#define CA_DSX_1227_112_2_17_23_11_7_1960 "KDA DSX:CompareKey need to save key \n"
#define CA_DSX_1233_112_2_17_23_11_7_1961 "KDA DSX:CompareKey don't save key \n"
#define CA_DSX_1249_112_2_17_23_11_7_1962 "KDA DSX:CompareKey finish\n"
#define CA_DSX_1261_112_2_17_23_11_7_1963 "KDA DSX:CompareKeyIndex index 0x%x\n"
#define CA_DSX_1270_112_2_17_23_11_7_1964 "KDA DSX:CompareKeyIndex min0x%x, i key index 0x%x\n"
#define CA_DSX_1278_112_2_17_23_11_7_1965 "KDA DSX:CompareKeyIndex arr index,min key index 0x%x\n"
#define CA_DSX_1293_112_2_17_23_11_7_1966 "KDA DSX:SetKey index 0x%x\n"
#define CA_DSX_1294_112_2_17_23_11_7_1967 "KDA DSX:SetKey key_used_count 0x%x,key_rcv_demux_cmd_ref%x\n"
#define CA_DSX_1295_112_2_17_23_11_7_1968 "KDA DSX:SetKey key_total_count 0x%x,s_is_to_set_key.is_set 0x%x\n"
#define CA_DSX_1305_112_2_17_23_11_7_1969 "KDA DSX :dsxSetKeys xSetNumKeys =0x%x \n"
#define CA_DSX_1316_112_2_17_23_11_7_1970 "KDA DSX:dsxSetKeys  key_index=%d\n"
#define CA_DSX_1332_112_2_17_23_11_7_1971 "KDA DSX:dsxSetKeys  s_is_to_set_key.key_id =%d,s_is_to_set_key.key_select =%dn"
#define CA_DSX_1345_112_2_17_23_11_7_1972 "KDA DSX:dsxSetKeys pxKeyTable[%d].pValue = 0x%x \n"
#define CA_DSX_1353_112_2_17_23_11_7_1973 "KDA DSX:dsxSetKeys  s_is_to_set_key.key_id =%d,s_is_to_set_key.key_select =%dn"
#define CA_DSX_1380_112_2_17_23_11_7_1974 "KDA DSX:dsxSetKeys key[%d]%x \n"
#define CA_DSX_1385_112_2_17_23_11_7_1975 "KDA DSX:dsxSetKeys  key_index%d\n"
#define CA_DSX_1427_112_2_17_23_11_7_1976 "KDA DSX:dsxSetKeys pxKeyTable %x ,tick 0x%x\n"
#define CA_DSX_1435_112_2_17_23_11_7_1977 "KDA DSX:dsxSetKeys s_csl_current_index 0x%x\n"
#define CA_DSX_1441_112_2_17_23_11_7_1978 "KDA DSX:dsxSetKeys pxKeyTable[%d],index=0x%x \n"
#define CA_DSX_1446_112_2_17_23_11_8_1979 "KDA DSX:dsxSetKeys key[%d]=0x%x \n"
#define CA_DSX_1466_112_2_17_23_11_8_1980 "KDA DSX:dsxSetKeys  key_index= 0x%x , is_play %d\n"
#define CA_DSX_1484_112_2_17_23_11_8_1981 "KDA DSX:dsxSetKeys  s_is_to_set_key.is_set =%d,s_is_to_set_key.key_id =%dn"
#define CA_DSX_1496_112_2_17_23_11_8_1982 "KDA DSX:dsxSetKeys  s_is_to_set_key.key_id =%d,s_is_to_set_key.key_select =%dn"
#define CA_DSX_1514_112_2_17_23_11_8_1983 "KDA DSX:dsxSetKeys CompareAndSaveKey \n"
#define CA_DSX_1556_112_2_17_23_11_8_1984 "KDA DSX: :ProcessDemuxReqCallback %x\n"
#define CA_DSX_1563_112_2_17_23_11_8_1985 "KDA CSL:time ProcessDemuxReqCallback tick%x\n"
#define CA_DSX_1584_112_2_17_23_11_8_1986 "KDA DSX: :DSX_ClearValue \n"
#define CA_DSX_1637_112_2_17_23_11_8_1987 "KDA DSX: IsExitFlag \n "
#define CA_DSX_1649_112_2_17_23_11_8_1988 "KDA DSX: IsExitFlag  no obj\n "
#define CA_DSX_1661_112_2_17_23_11_8_1989 "KDA DSX: :DSX_Init \n"
#define CA_DSX_1682_112_2_17_23_11_8_1990 "KDA DSX:DSX_Exit \n"
#define CA_ICC_55_112_2_17_23_11_8_1991 "icc register ok!\n"
#define CA_ICC_144_112_2_17_23_11_8_1992 "***ICC KDA: send card inserted event tick 0x%x\n"
#define CA_ICC_159_112_2_17_23_11_8_1993 "***ICC KDA: CA_Resume tick 0x%x\n"
#define CA_ICC_174_112_2_17_23_11_8_1994 "icc cancel registeration ok!\n"
#define CA_ICC_188_112_2_17_23_11_8_1995 " T1 raw exchange mode not supported !\n"
#define CA_ICC_201_112_2_17_23_11_8_1996 "Calling .......icc T0 send\n"
#define CA_ICC_220_112_2_17_23_11_8_1997 "Calling .......icc T0 receive\n"
#define CA_ICC_247_112_2_17_23_11_9_1998 "**KDA***  T0 exchange sessionid:%d\n"
#define CA_ICC_254_112_2_17_23_11_9_1999 "**KDA*** Calling .......icc T0 exchange \n"
#define CA_ICC_256_112_2_17_23_11_9_2000 "**KDA icc  input: len:%d 0x%x 0x%x 0x%x 0x%x n"
#define CA_ICC_264_112_2_17_23_11_9_2001 "**KDA** Calling .......icc ERROR code:%xn"
#define CA_ICC_270_112_2_17_23_11_9_2002 "**KDA** Calling .......icc T0 exchange ok len:%d data:0x%x 0x%x 0x%xn"
#define CA_ICC_284_112_2_17_23_11_9_2003 "KDA_ICC T0Exchange Error \n"
#define CA_ICC_309_112_2_17_23_11_9_2004 "iccSmartcardReset %d %d"
#define CA_ICC_320_112_2_17_23_11_9_2005 "KDA Termination: ICC TERMINATED!\n"
#define CA_OS_SC6600R_203_112_2_17_23_11_9_2006 "timer CB: entering start:%d stop:%d interval:%d n"
#define CA_OS_SC6600R_236_112_2_17_23_11_9_2007 "***KDA Timer CB: send to task:0x%x event:0x%x n"
#define CA_OS_SC6600R_344_112_2_17_23_11_10_2008 "ESG:IsFolderExist: find first sfs_handle = %d"
#define CA_OS_SC6600R_445_112_2_17_23_11_10_2009 "--KDA- DalOsSetStorageContent.........file:%s \n"
#define CA_OS_SC6600R_452_112_2_17_23_11_10_2010 "---KDA ALoSetStorageContent error,open file failed!\n"
#define CA_OS_SC6600R_458_112_2_17_23_11_10_2011 "---KDA ALoSetStorageContent error,write file failed \n"
#define CA_OS_SC6600R_529_112_2_17_23_11_10_2012 " --KDA DalOsGetStorageContent..........%s \n"
#define CA_OS_SC6600R_536_112_2_17_23_11_10_2013 "---KDA ALoGetStorageContent error,open file failed!"
#define CA_OS_SC6600R_541_112_2_17_23_11_10_2014 "---KDA ALoGetStorageContent error,create file failed!"
#define CA_OS_SC6600R_544_112_2_17_23_11_10_2015 "---KDA ALoSetStorageContent  Create file:%s OK! \n"
#define CA_OS_SC6600R_547_112_2_17_23_11_10_2016 "---KDA ALoSetStorageContent RESET DATA FAILED\n"
#define CA_OS_SC6600R_551_112_2_17_23_11_10_2017 "---KDA ALoSetStorageContent RESET file:%s OK! \n"
#define CA_OS_SC6600R_558_112_2_17_23_11_10_2018 "---KDA ALoGetStorageContent error,read file failed!"
#define CA_OS_SC6600R_629_112_2_17_23_11_10_2019 "****TASK ENTRY OVER: array:%d task_array:0x%x~~~~~~~~"
#define CA_OS_SC6600R_642_112_2_17_23_11_10_2020 "osTaskCreate  xPriority = %d!\n"
#define CA_OS_SC6600R_745_112_2_17_23_11_10_2021 "CREATE TASK OK task_array %d=0x%x pxTaskId=0x%x threadid:0x%x!n"
#define CA_OS_SC6600R_760_112_2_17_23_11_10_2022 "TASK DELETE.....xTaskId=0x%x!\n"
#define CA_OS_SC6600R_800_112_2_17_23_11_10_2023 "***KDA****TASK DELETE OK array:%d thread id:0x%x OVER \n"
#define CA_OS_SC6600R_825_112_2_17_23_11_11_2024 " KDA_OS osTaskIdentify: os task Entering.....thread_id =  0x%x !"
#define CA_OS_SC6600R_838_112_2_17_23_11_11_2025 "KDA_OS os task identify ok pxTaskId = 0x%x thread_id = 0x%x n"
#define CA_OS_SC6600R_907_112_2_17_23_11_11_2026 " _--KDA osCharacterGet: %d %c"
#define CA_OS_SC6600R_1007_112_2_17_23_11_11_2027 "***KDA free event task OK id 0x%x event:0x%x\n"
#define CA_OS_SC6600R_1082_112_2_17_23_11_11_2028 "osEventReceive:WAIT_FOREVER, RECEIVED!!! event:%x"
#define CA_OS_SC6600R_1086_112_2_17_23_11_11_2029 "osEventReceive:Wait_forever, GET SEM BUT NO EVENT!!! event:%x"
#define CA_OS_SC6600R_1095_112_2_17_23_11_11_2030 "osEventReceive:RECEIVED or timeout!!! event:%x"
#define CA_OS_SC6600R_1112_112_2_17_23_11_11_2031 "receive event: received event:%d start:%d stop%d int:%dn"
#define CA_OS_SC6600R_1197_112_2_17_23_11_11_2032 " create timer FAILED:  event:0x%x \n"
#define CA_OS_SC6600R_1201_112_2_17_23_11_11_2033 " create timer ok: timer id:0x%x event:0x%x \n"
#define CA_OS_SC6600R_1552_112_2_17_23_11_12_2034 "***KDA Event THread: Failed to send Event \n"
#define CA_OS_SC6600R_1587_112_2_17_23_11_12_2035 "\"\"\"\"\"#CA KDA Init FAILED \"\"\"\n"
#define CA_OS_SC6600R_1600_112_2_17_23_11_12_2036 "---KDA CA EXIT: deleting sem \n"
#define CA_OS_SC6600R_1610_112_2_17_23_11_12_2037 "---KDA CA EXIT: deleting timer \n"
#define CA_OS_SC6600R_1620_112_2_17_23_11_12_2038 "---KDA CA EXIT: deleting task \n"
#define CA_OS_SC6600R_1623_112_2_17_23_11_12_2039 "---KDA CA EXIT: deleting task-->Delete Semaphore \n"
#define CA_OS_SC6600R_1628_112_2_17_23_11_12_2040 "---KDA CA EXIT: deleting task-->Delete Thread \n"
#define CA_OS_SC6600R_1633_112_2_17_23_11_12_2041 "---KDA CA EXIT: Free Task \n"
#define CMMB_OPERATION_279_112_2_17_23_11_13_2042 "CMMB_Init s_is_set_emm_demux %d"
#define CMMB_OPERATION_281_112_2_17_23_11_13_2043 "CMMB_Init:%d"
#define CMMB_OPERATION_362_112_2_17_23_11_13_2044 "Cleanup: bitmap %p"
#define CMMB_OPERATION_396_112_2_17_23_11_13_2045 "Enter PlayNetwork with %d %d[%d]"
#define CMMB_OPERATION_401_112_2_17_23_11_13_2046 "PlayNetwork state error!"
#define CMMB_OPERATION_407_112_2_17_23_11_13_2047 "PlayNetwork: no ctrl information table"
#define CMMB_OPERATION_423_112_2_17_23_11_13_2048 "PlayNetwork: play_thread_id should be 0!!!"
#define CMMB_OPERATION_444_112_2_17_23_11_13_2049 "Create play thread failed!!"
#define CMMB_OPERATION_453_112_2_17_23_11_13_2050 "Create play timer failed!!"
#define CMMB_OPERATION_458_112_2_17_23_11_13_2051 "create playtimeout timer %p"
#define CMMB_OPERATION_463_112_2_17_23_11_13_2052 "PlayNetwork: wait playparam"
#define CMMB_OPERATION_483_112_2_17_23_11_13_2053 "PlayNetwork: wait to get play param"
#define CMMB_OPERATION_488_112_2_17_23_11_13_2054 "PlayNetwork: cannot get play param"
#define CMMB_OPERATION_519_112_2_17_23_11_13_2055 "Enter PlayNetworkByPlayParam"
#define CMMB_OPERATION_524_112_2_17_23_11_14_2056 "PlayNetwork state error!"
#define CMMB_OPERATION_552_112_2_17_23_11_14_2057 "Create play thread failed!!"
#define CMMB_OPERATION_561_112_2_17_23_11_14_2058 "Create play timer failed!!"
#define CMMB_OPERATION_565_112_2_17_23_11_14_2059 "create playtimeout timer %p"
#define CMMB_OPERATION_570_112_2_17_23_11_14_2060 "PlayNetwork: wait playparam"
#define CMMB_OPERATION_609_112_2_17_23_11_14_2061 "Enter PlayStream @%d"
#define CMMB_OPERATION_614_112_2_17_23_11_14_2062 "PlayNetwork state error!"
#define CMMB_OPERATION_644_112_2_17_23_11_14_2063 "Create play thread failed!!"
#define CMMB_OPERATION_675_112_2_17_23_11_14_2064 "error open play file!"
#define CMMB_OPERATION_683_112_2_17_23_11_14_2065 "file name length %d exceeds MAX length %d!!!"
#define CMMB_OPERATION_697_112_2_17_23_11_14_2066 "error open desp file!"
#define CMMB_OPERATION_709_112_2_17_23_11_14_2067 "Playback: desp file is corrupt, play from begining"
#define CMMB_OPERATION_728_112_2_17_23_11_14_2068 "Create playback thread failed!!"
#define CMMB_OPERATION_747_112_2_17_23_11_14_2069 "Create playbackread thread failed!!"
#define CMMB_OPERATION_762_112_2_17_23_11_14_2070 "Fill initial buffer failed!!"
#define CMMB_OPERATION_812_112_2_17_23_11_14_2071 "Create play timer failed!!"
#define CMMB_OPERATION_816_112_2_17_23_11_14_2072 "create playtimeout timer %p"
#define CMMB_OPERATION_827_112_2_17_23_11_14_2073 "Create playback timer failed!!"
#define CMMB_OPERATION_864_112_2_17_23_11_14_2074 "Enter PlayStream2 @%d"
#define CMMB_OPERATION_869_112_2_17_23_11_14_2075 "PlayStream state error!"
#define CMMB_OPERATION_899_112_2_17_23_11_14_2076 "PlayStream: Create play thread failed!!"
#define CMMB_OPERATION_930_112_2_17_23_11_14_2077 "PlayStream: error open play file!"
#define CMMB_OPERATION_936_112_2_17_23_11_14_2078 "PlayStream: error get mf len of the play file!"
#define CMMB_OPERATION_949_112_2_17_23_11_14_2079 "PlayStream: error get mf pts!"
#define CMMB_OPERATION_956_112_2_17_23_11_14_2080 "PlayStream: error get mf pos by pts!"
#define CMMB_OPERATION_965_112_2_17_23_11_14_2081 "PlayStream: error get mf pts!"
#define CMMB_OPERATION_985_112_2_17_23_11_14_2082 "PlayStream: Create playback thread failed!!"
#define CMMB_OPERATION_1004_112_2_17_23_11_14_2083 "PlayStream: Create playbackread thread failed!!"
#define CMMB_OPERATION_1019_112_2_17_23_11_15_2084 "PlayStream: Fill initial buffer failed!!"
#define CMMB_OPERATION_1073_112_2_17_23_11_15_2085 "PlayStream: error file header %X %X"
#define CMMB_OPERATION_1085_112_2_17_23_11_15_2086 "%02X %02X %02X %02X %02X %02X %02X %02X"
#define CMMB_OPERATION_1121_112_2_17_23_11_15_2087 "Create play timer failed!!"
#define CMMB_OPERATION_1125_112_2_17_23_11_15_2088 "create playtimeout timer %p"
#define CMMB_OPERATION_1131_112_2_17_23_11_15_2089 "Create playback timer failed!!"
#define CMMB_OPERATION_1163_112_2_17_23_11_15_2090 "STOP: call cmmb_stop[%d]"
#define CMMB_OPERATION_1173_112_2_17_23_11_15_2091 "STOP: begin stop"
#define CMMB_OPERATION_1189_112_2_17_23_11_15_2092 "STOP: wait play_thread_id = 0"
#define CMMB_OPERATION_1198_112_2_17_23_11_15_2093 "delete playtimeout timer %p"
#define CMMB_OPERATION_1207_112_2_17_23_11_15_2094 "STOP: wait record stop"
#define CMMB_OPERATION_1218_112_2_17_23_11_15_2095 "STOP: bitmap %p"
#define CMMB_OPERATION_1235_112_2_17_23_11_15_2096 "Create play thread failed!!"
#define CMMB_OPERATION_1243_112_2_17_23_11_15_2097 "STOP: parse MSFH for CA STOP 0x%x\n"
#define CMMB_OPERATION_1285_112_2_17_23_11_15_2098 "Create play timer failed!!"
#define CMMB_OPERATION_1288_112_2_17_23_11_15_2099 "create stoptimeout timer %p"
#define CMMB_OPERATION_1309_112_2_17_23_11_15_2100 "STOP: wrong state!"
#define CMMB_OPERATION_1322_112_2_17_23_11_15_2101 "STOP: begin stop"
#define CMMB_OPERATION_1356_112_2_17_23_11_15_2102 "delete playbacktimeout timer %p"
#define CMMB_OPERATION_1373_112_2_17_23_11_15_2103 "STOP: bitmap %p"
#define CMMB_OPERATION_1391_112_2_17_23_11_15_2104 "Create play thread failed!!"
#define CMMB_OPERATION_1412_112_2_17_23_11_15_2105 "Create play timer failed!!"
#define CMMB_OPERATION_1415_112_2_17_23_11_15_2106 "create stoptimeout timer %p"
#define CMMB_OPERATION_1454_112_2_17_23_11_15_2107 "STOP: wrong state!"
#define CMMB_OPERATION_1460_112_2_17_23_11_15_2108 "STOP: should not be here!!!"
#define CMMB_OPERATION_1477_112_2_17_23_11_15_2109 "ABORT: call cmmb_abort"
#define CMMB_OPERATION_1488_112_2_17_23_11_15_2110 "ABORT: begin stop"
#define CMMB_OPERATION_1494_112_2_17_23_11_16_2111 "delete playtimeout timer %p"
#define CMMB_OPERATION_1503_112_2_17_23_11_16_2112 "ABORT: wait record stop"
#define CMMB_OPERATION_1509_112_2_17_23_11_16_2113 "ABORT: parse MSFH for CA STOP 0x%x\n"
#define CMMB_OPERATION_1532_112_2_17_23_11_16_2114 "ABORT: bitmap %p"
#define CMMB_OPERATION_1540_112_2_17_23_11_16_2115 "ABORT: play_thread_id should be 0!!!"
#define CMMB_OPERATION_1556_112_2_17_23_11_16_2116 "Create play thread failed!!"
#define CMMB_OPERATION_1575_112_2_17_23_11_16_2117 "ABORT: abort finish"
#define CMMB_OPERATION_1579_112_2_17_23_11_16_2118 "ABORT: wrong state!"
#define CMMB_OPERATION_1594_112_2_17_23_11_16_2119 "ABORT: begin abort"
#define CMMB_OPERATION_1623_112_2_17_23_11_16_2120 "delete playbacktimeout timer %p"
#define CMMB_OPERATION_1637_112_2_17_23_11_16_2121 "ABORT: bitmap %p"
#define CMMB_OPERATION_1645_112_2_17_23_11_16_2122 "ABORT: play_thread_id should be 0!!!"
#define CMMB_OPERATION_1661_112_2_17_23_11_16_2123 "Create play thread failed!!"
#define CMMB_OPERATION_1679_112_2_17_23_11_16_2124 "CMMB_StopPlaybackCallback: restore arm clock"
#define CMMB_OPERATION_1681_112_2_17_23_11_16_2125 "ABORT: abort finish"
#define CMMB_OPERATION_1685_112_2_17_23_11_16_2126 "ABORT: wrong state!"
#define CMMB_OPERATION_1691_112_2_17_23_11_16_2127 "ABORT: should not be here!"
#define CMMB_OPERATION_1708_112_2_17_23_11_16_2128 "CMMB_Pause: call CMMB_Pause"
#define CMMB_OPERATION_1748_112_2_17_23_11_16_2129 "CMMB_Pause: should not be here!"
#define CMMB_OPERATION_1765_112_2_17_23_11_16_2130 "CMMB_Resume: call CMMB_Resume"
#define CMMB_OPERATION_1805_112_2_17_23_11_16_2131 "CMMB_Resume: should not be here!"
#define CMMB_OPERATION_1834_112_2_17_23_11_16_2132 "open file error!"
#define CMMB_OPERATION_1840_112_2_17_23_11_16_2133 "read file error!"
#define CMMB_OPERATION_1845_112_2_17_23_11_16_2134 "not a valid recorded file!"
#define CMMB_OPERATION_1874_112_2_17_23_11_16_2135 "enter CMMB_GetStreamLength2"
#define CMMB_OPERATION_1888_112_2_17_23_11_16_2136 "CMMB_GetStreamLength2: zero MF_len"
#define CMMB_OPERATION_1929_112_2_17_23_11_16_2137 "CMMB_GetStreamLength2: start_pts %X last_pts %X"
#define CMMB_OPERATION_1986_112_2_17_23_11_17_2138 "Enter EnableRecord with %d %d"
#define CMMB_OPERATION_1995_112_2_17_23_11_17_2139 "State error!"
#define CMMB_OPERATION_2003_112_2_17_23_11_17_2140 "open file error!"
#define CMMB_OPERATION_2008_112_2_17_23_11_17_2141 "EnableRecord: cannot seek to end!!!"
#define CMMB_OPERATION_2013_112_2_17_23_11_17_2142 "EnableRecord: cannot get file pos!!!"
#define CMMB_OPERATION_2016_112_2_17_23_11_17_2143 "EnableRecord: pos %d"
#define CMMB_OPERATION_2025_112_2_17_23_11_17_2144 "EnableRecord: msfh size %d exceeds MAXMSFHSIZE!!!"
#define CMMB_OPERATION_2040_112_2_17_23_11_17_2145 "EnableRecord: isma error"
#define CMMB_OPERATION_2045_112_2_17_23_11_17_2146 "EnableRecord: isma len %d"
#define CMMB_OPERATION_2047_112_2_17_23_11_17_2147 "EnableRecord: msfh len %d"
#define CMMB_OPERATION_2050_112_2_17_23_11_17_2148 "write file error!"
#define CMMB_OPERATION_2063_112_2_17_23_11_17_2149 "EnableRecord: file name length %d exceeds max length!!!"
#define CMMB_OPERATION_2079_112_2_17_23_11_17_2150 "open file error!"
#define CMMB_OPERATION_2087_112_2_17_23_11_17_2151 "EnableRecord: cannot seek to end!!!"
#define CMMB_OPERATION_2169_112_2_17_23_11_17_2152 "Create record thread failed!!"
#define CMMB_OPERATION_2196_112_2_17_23_11_17_2153 "Create record write thread failed!!"
#define CMMB_OPERATION_2221_112_2_17_23_11_17_2154 "EnableRecord: should not be here!!!"
#define CMMB_OPERATION_2246_112_2_17_23_11_17_2155 "Enter EnableRecord2 with %d %d"
#define CMMB_OPERATION_2255_112_2_17_23_11_17_2156 "State error!"
#define CMMB_OPERATION_2277_112_2_17_23_11_17_2157 "%02X %02X %02X %02X %02X %02X %02X %02X"
#define CMMB_OPERATION_2287_112_2_17_23_11_17_2158 "open file error!"
#define CMMB_OPERATION_2320_112_2_17_23_11_17_2159 "Create record thread failed!!"
#define CMMB_OPERATION_2344_112_2_17_23_11_17_2160 "Create record write thread failed!!"
#define CMMB_OPERATION_2370_112_2_17_23_11_17_2161 "EnableRecord2: should not be here!!!"
#define CMMB_OPERATION_2386_112_2_17_23_11_17_2162 "Enter AbortRecord %d"
#define CMMB_OPERATION_2440_112_2_17_23_11_17_2163 "open file error!"
#define CMMB_OPERATION_2446_112_2_17_23_11_17_2164 "read file error!"
#define CMMB_OPERATION_2452_112_2_17_23_11_17_2165 "not a valid recorded file! %d"
#define CMMB_OPERATION_2458_112_2_17_23_11_17_2166 "not a CMMB recorded file!"
#define CMMB_OPERATION_2487_112_2_17_23_11_18_2167 "enter CMMB_IdentifyStream2"
#define CMMB_OPERATION_2522_112_2_17_23_11_18_2168 "CMMB_EnumAudioLanguages: wrong state %d"
#define CMMB_OPERATION_2527_112_2_17_23_11_18_2169 "CMMB_EnumAudioLanguages: msfh is null"
#define CMMB_OPERATION_2539_112_2_17_23_11_18_2170 "CMMB_EnumAudioLanguages: %d"
#define CMMB_OPERATION_2589_112_2_17_23_11_18_2171 "CMMB_SetAudioLanguage: wrong state %d"
#define CMMB_OPERATION_2594_112_2_17_23_11_18_2172 "CMMB_SetAudioLanguage: msfh null"
#define CMMB_OPERATION_2599_112_2_17_23_11_18_2173 "CMMB_SetAudioLanguage: playparam null"
#define CMMB_OPERATION_2605_112_2_17_23_11_18_2174 "CMMB_SetAudioLanguage: %d id %d"
#define CMMB_OPERATION_2704_112_2_17_23_11_18_2175 "FillBuffer: cannot set file pointer"
#define CMMB_OPERATION_2748_112_2_17_23_11_18_2176 "FillBuffer: desp pos %d"
#define CMMB_OPERATION_2752_112_2_17_23_11_18_2177 "FillBuffer: cannot set file pointer"
#define CMMB_OPERATION_2779_112_2_17_23_11_18_2178 "FillBuffer: data_len %d exceeds MAXMSFSIZE"
#define CMMB_OPERATION_2785_112_2_17_23_11_18_2179 "FillBuffer: FFS_fread error!"
#define CMMB_OPERATION_2802_112_2_17_23_11_18_2180 "Playback: TS %d 0x%X"
#define CMMB_OPERATION_2837_112_2_17_23_11_18_2181 "FillBuffer2: data_offset %d"
#define CMMB_OPERATION_2847_112_2_17_23_11_18_2182 "FillBuffer2: data_len %d exceeds MAXMSFSIZE"
#define CMMB_OPERATION_2853_112_2_17_23_11_18_2183 "FillBuffer2: FFS_fread error!"
#define CMMB_OPERATION_2870_112_2_17_23_11_18_2184 "Playback: TS %d 0x%X"
#define CMMB_OPERATION_2889_112_2_17_23_11_18_2185 "ReqCAInfo: %d %d"
#define CMMB_OPERATION_2895_112_2_17_23_11_18_2186 "CA_Get_Info: error %d"
#define CMMB_OPERATION_2924_112_2_17_23_11_18_2187 "ReqCAInfo: %d %d"
#define CMMB_OPERATION_2976_112_2_17_23_11_19_2188 "CMMB_UpdateMCICallback: should not be here!!!"
#define CMMB_OPERATION_2983_112_2_17_23_11_19_2189 "PlayNetwork: Update mci"
#define CMMB_OPERATION_2999_112_2_17_23_11_19_2190 "CMMB_UpdateMCICallback: should not be here!!!"
#define CMMB_OPERATION_3029_112_2_17_23_11_19_2191 "UpdateMSFHeaderCallback: count %d ind %d"
#define CMMB_OPERATION_3075_112_2_17_23_11_19_2192 "PlayNetwork: len=%d ind=%p"
#define CMMB_OPERATION_3078_112_2_17_23_11_19_2193 "0x%02X"
#define CMMB_OPERATION_3094_112_2_17_23_11_19_2194 "PlayNetwork: audio sample rate %d"
#define CMMB_OPERATION_3130_112_2_17_23_11_19_2195 "Enter CMMB_DataReadyCallback() %d"
#define CMMB_OPERATION_3171_112_2_17_23_11_19_2196 "CMMB_SetPacketIDCallback: service_id %d , this->cmmb_service_id %d ,s_is_set_emm_demux %d\n"
#define CMMB_OPERATION_3195_112_2_17_23_11_19_2197 "CMMB_SetPacketIDCallback: demux_param.mux_subframe_id%d\n"
#define CMMB_OPERATION_3221_112_2_17_23_11_19_2198 "CMMB_SetPacketIDCallback s_is_set_emm_demux %d"
#define CMMB_OPERATION_3227_112_2_17_23_11_19_2199 "CMMB_SetPacketIDCallback: Get CA Param ERROR!"
#define CMMB_OPERATION_3304_112_2_17_23_11_19_2200 "CMMB_DataReadyCallback: about play to end"
#define CMMB_OPERATION_3333_112_2_17_23_11_19_2201 "PLAYBACK: STC %d"
#define CMMB_OPERATION_3346_112_2_17_23_11_19_2202 "PLAYBACK: TIME_IND %d %d %d"
#define CMMB_OPERATION_3352_112_2_17_23_11_19_2203 "PLAYBACK: STREAM_END"
#define CMMB_OPERATION_3396_112_2_17_23_11_19_2204 "Enter CMMB_AVACKCallback"
#define CMMB_OPERATION_3427_112_2_17_23_11_19_2205 "PlayNetwork: enter CMMB_AVACKCallback but state error"
#define CMMB_OPERATION_3431_112_2_17_23_11_19_2206 "Enter CMMB_AVACKCallback"
#define CMMB_OPERATION_3476_112_2_17_23_11_20_2207 "STOP: enter CMMB_StopCallback but state error"
#define CMMB_OPERATION_3484_112_2_17_23_11_20_2208 "Enter CMMB_StopCallback"
#define CMMB_OPERATION_3508_112_2_17_23_11_20_2209 "STOP: enter CMMB_StopSPIAudioCallback but state error"
#define CMMB_OPERATION_3513_112_2_17_23_11_20_2210 "STOP: spi audio thread exit"
#define CMMB_OPERATION_3525_112_2_17_23_11_20_2211 "STOP: receive all stop ack"
#define CMMB_OPERATION_3552_112_2_17_23_11_20_2212 "CMMB_StopPlaybackCallback: receive signal 0x%X"
#define CMMB_OPERATION_3561_112_2_17_23_11_20_2213 "STOP: begin stop"
#define CMMB_OPERATION_3602_112_2_17_23_11_20_2214 "delete playbacktimeout timer %p"
#define CMMB_OPERATION_3624_112_2_17_23_11_20_2215 "STOP: bitmap %p"
#define CMMB_OPERATION_3645_112_2_17_23_11_20_2216 "Create play thread failed!!"
#define CMMB_OPERATION_3666_112_2_17_23_11_20_2217 "Create play timer failed!!"
#define CMMB_OPERATION_3669_112_2_17_23_11_20_2218 "create stoptimeout timer %p"
#define CMMB_OPERATION_3715_112_2_17_23_11_20_2219 "CMMB_CleanupCallback unknown resp_id %d"
#define CMMB_OPERATION_3750_112_2_17_23_11_20_2220 "PlayNetwork: NO CAT"
#define CMMB_OPERATION_3807_112_2_17_23_11_20_2221 "PlayNetwork:  receive CMMB_ABORT %d"
#define CMMB_OPERATION_3850_112_2_17_23_11_20_2222 "CMMB_PlayThread s_is_set_emm_demux %d"
#define CMMB_OPERATION_3914_112_2_17_23_11_20_2223 "PlayNetwork: wait MSFH"
#define CMMB_OPERATION_3940_112_2_17_23_11_20_2224 "PlayNetwork: receive MSFH[%d]"
#define CMMB_OPERATION_3941_112_2_17_23_11_20_2225 "PlayNetwork: len=%d ind=0x%02X"
#define CMMB_OPERATION_3944_112_2_17_23_11_21_2226 "0x%02X"
#define CMMB_OPERATION_3987_112_2_17_23_11_21_2227 "PlayNetwork: wait bitmap %X"
#define CMMB_OPERATION_4015_112_2_17_23_11_21_2228 "PlayNetwork: audio sample rate %d"
#define CMMB_OPERATION_4074_112_2_17_23_11_21_2229 "PlayNetwork: Video play %d"
#define CMMB_OPERATION_4079_112_2_17_23_11_21_2230 "PlayNetwork: codec type %d"
#define CMMB_OPERATION_4126_112_2_17_23_11_21_2231 "PlayNetwork: MMAL play error!"
#define CMMB_OPERATION_4135_112_2_17_23_11_21_2232 "PlayNetwork: wait AVACK"
#define CMMB_OPERATION_4152_112_2_17_23_11_21_2233 "PlayNetwork: receive audio ack %X[%d]"
#define CMMB_OPERATION_4157_112_2_17_23_11_21_2234 "PlayNetwork: receive video ack %X"
#define CMMB_OPERATION_4176_112_2_17_23_11_21_2235 "delete playtimeout timer %p"
#define CMMB_OPERATION_4183_112_2_17_23_11_21_2236 "wrong state in CMMB_PlayThread!!!"
#define CMMB_OPERATION_4237_112_2_17_23_11_21_2237 "STOP: recv dis channel %p"
#define CMMB_OPERATION_4242_112_2_17_23_11_21_2238 "STOP: recv dis vo %p"
#define CMMB_OPERATION_4246_112_2_17_23_11_21_2239 "STOP: recv dis ao %p"
#define CMMB_OPERATION_4252_112_2_17_23_11_21_2240 "STOP: recv dis service %p"
#define CMMB_OPERATION_4257_112_2_17_23_11_21_2241 "STOP: recv dis msfh %p"
#define CMMB_OPERATION_4261_112_2_17_23_11_21_2242 "STOP: recv vid stop %p"
#define CMMB_OPERATION_4265_112_2_17_23_11_21_2243 "STOP: recv aud stop %p"
#define CMMB_OPERATION_4269_112_2_17_23_11_21_2244 "STOP: recv pcm stop %p"
#define CMMB_OPERATION_4274_112_2_17_23_11_21_2245 "STOP: recv pcm abort %p"
#define CMMB_OPERATION_4278_112_2_17_23_11_21_2246 "STOP: recv playback stop %p"
#define CMMB_OPERATION_4282_112_2_17_23_11_21_2247 "STOP: recv ca stop %p"
#define CMMB_OPERATION_4285_112_2_17_23_11_21_2248 "STOP: recv %X"
#define CMMB_OPERATION_4294_112_2_17_23_11_21_2249 "STOP: receive all stop ack"
#define CMMB_OPERATION_4297_112_2_17_23_11_21_2250 "delete stoptimeout timer %p"
#define CMMB_OPERATION_4316_112_2_17_23_11_21_2251 "STOP: send MTV_PLAY_ERROR"
#define CMMB_OPERATION_4321_112_2_17_23_11_21_2252 "STOP: send MTV_STOP_ERROR"
#define CMMB_OPERATION_4326_112_2_17_23_11_21_2253 "STOP: send MTV_PLAY_ERROR_FOR_CA"
#define CMMB_OPERATION_4331_112_2_17_23_11_21_2254 "STOP: send MTV_PLAY_ERROR_FOR_NOAV"
#define CMMB_OPERATION_4336_112_2_17_23_11_21_2255 "STOP: should not be here!!!"
#define CMMB_OPERATION_4341_112_2_17_23_11_21_2256 "STOP: send MTV_STOP_ACK"
#define CMMB_OPERATION_4389_112_2_17_23_11_21_2257 "ABORT: recv pcm stop %p"
#define CMMB_OPERATION_4393_112_2_17_23_11_21_2258 "ABORT: recv playback stop %p"
#define CMMB_OPERATION_4396_112_2_17_23_11_21_2259 "ABORT: recv %X"
#define CMMB_OPERATION_4439_112_2_17_23_11_22_2260 "FindAudioIDByLang: %d"
#define CMMB_OPERATION_4498_112_2_17_23_11_22_2261 " FillInitial: %d %d %d"
#define CMMB_OPERATION_4511_112_2_17_23_11_22_2262 "Fill seine buffer %d"
#define CMMB_OPERATION_4538_112_2_17_23_11_22_2263 "Fill seine buffer desp %p, data %p"
#define CMMB_OPERATION_4539_112_2_17_23_11_22_2264 "BB buffer desp %p, data %p"
#define CMMB_OPERATION_4540_112_2_17_23_11_22_2265 "Data len desp %d, data %d"
#define CMMB_OPERATION_4581_112_2_17_23_11_22_2266 "error sample rate idx %d"
#define CMMB_OPERATION_4607_112_2_17_23_11_22_2267 "BandWidth %d freq %d mfid %d RSMode %d InterleaveMode %dn  LDPCMode %d MappingMode %d ScrambleMode %d TSNumber %d TSStartId %d"
#define CMMB_OPERATION_4660_112_2_17_23_11_22_2268 "MSFH: len=%d"
#define CMMB_OPERATION_4661_112_2_17_23_11_22_2269 "MSFH: st_ind=%d"
#define CMMB_OPERATION_4662_112_2_17_23_11_22_2270 "MSFH: vd_ind=%d"
#define CMMB_OPERATION_4663_112_2_17_23_11_22_2271 "MSFH: au_ind=%d"
#define CMMB_OPERATION_4664_112_2_17_23_11_22_2272 "MSFH: da_ind=%d"
#define CMMB_OPERATION_4665_112_2_17_23_11_22_2273 "MSFH: ex_ind=%d"
#define CMMB_OPERATION_4666_112_2_17_23_11_22_2274 "MSFH: sc_ind=%d"
#define CMMB_OPERATION_4667_112_2_17_23_11_22_2275 "MSFH: en_mod=%d"
#define CMMB_OPERATION_4671_112_2_17_23_11_22_2276 "MSFH: MSFH lens should not be zero!!!"
#define CMMB_OPERATION_4681_112_2_17_23_11_22_2277 "MSFH: st=%d"
#define CMMB_OPERATION_4697_112_2_17_23_11_22_2278 "MSFH: vd_len=%d"
#define CMMB_OPERATION_4698_112_2_17_23_11_22_2279 "MSFH: vd_cnt=%d"
#define CMMB_OPERATION_4714_112_2_17_23_11_22_2280 "MSFH: au_len=%d"
#define CMMB_OPERATION_4715_112_2_17_23_11_22_2281 "MSFH: au_cnt=%d"
#define CMMB_OPERATION_4731_112_2_17_23_11_22_2282 "MSFH: da_len=%d"
#define CMMB_OPERATION_4732_112_2_17_23_11_22_2283 "MSFH: ecm_ind=%d"
#define CMMB_OPERATION_4762_112_2_17_23_11_22_2284 "MSFH: vd_algo=%d"
#define CMMB_OPERATION_4763_112_2_17_23_11_22_2285 "MSFH: rate_ind=%d"
#define CMMB_OPERATION_4764_112_2_17_23_11_22_2286 "MSFH: disp_ind=%d"
#define CMMB_OPERATION_4765_112_2_17_23_11_22_2287 "MSFH: res_ind=%d"
#define CMMB_OPERATION_4766_112_2_17_23_11_22_2288 "MSFH: fps_ind=%d"
#define CMMB_OPERATION_4776_112_2_17_23_11_22_2289 "MSFH: rate=%d"
#define CMMB_OPERATION_4790_112_2_17_23_11_22_2290 "MSFH: x=%d"
#define CMMB_OPERATION_4791_112_2_17_23_11_22_2291 "MSFH: y=%d"
#define CMMB_OPERATION_4792_112_2_17_23_11_22_2292 "MSFH: pri=%d"
#define CMMB_OPERATION_4805_112_2_17_23_11_22_2293 "MSFH: w=%d"
#define CMMB_OPERATION_4806_112_2_17_23_11_22_2294 "MSFH: h=%d"
#define CMMB_OPERATION_4818_112_2_17_23_11_22_2295 "MSFH: fps=%d"
#define CMMB_OPERATION_4847_112_2_17_23_11_22_2296 "MSFH: au_algo=%d"
#define CMMB_OPERATION_4848_112_2_17_23_11_22_2297 "MSFH: rate_ind=%d"
#define CMMB_OPERATION_4849_112_2_17_23_11_22_2298 "MSFH: sample_ind=%d"
#define CMMB_OPERATION_4850_112_2_17_23_11_22_2299 "MSFH: desp_ind=%d"
#define CMMB_OPERATION_4861_112_2_17_23_11_22_2300 "MSFH: rate=%d"
#define CMMB_OPERATION_4873_112_2_17_23_11_22_2301 "MSFH: sample=%d"
#define CMMB_OPERATION_4888_112_2_17_23_11_22_2302 "MSFH: desp=%d"
#define CMMB_OPERATION_5005_112_2_17_23_11_23_2303 "MATCH %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X"
#define CMMB_OPERATION_5014_112_2_17_23_11_23_2304 "Enter SendAbortToPlayThread"
#define CMMB_OPERATION_5034_112_2_17_23_11_23_2305 "Enter PlayTimeOutCallback"
#define CMMB_OPERATION_5050_112_2_17_23_11_23_2306 "Enter StopTimeOutCallback with 0x%X"
#define CMMB_OPERATION_5074_112_2_17_23_11_23_2307 "Enter CAStopCallback with 0x%X"
#define CMMB_OPERATION_5092_112_2_17_23_11_23_2308 "CAInfoCopy: should not be here!!!"
#define CMMB_OPERATION_5131_112_2_17_23_11_23_2309 "CAInfoCopy: should not be here!!!"
#define CMMB_OPERATION_5134_112_2_17_23_11_23_2310 "CAInfoCopy: should not be here!!!"
#define CMMB_OPERATION_5160_112_2_17_23_11_23_2311 "Enter CACallback"
#define CMMB_OPERATION_5174_112_2_17_23_11_23_2312 "CACallback: Detect card not present"
#define CMMB_OPERATION_5187_112_2_17_23_11_23_2313 "CACallback: Detect card not granted"
#define CMMB_OPERATION_5200_112_2_17_23_11_23_2314 "CACallback: Detect card granted"
#define CMMB_OPERATION_5215_112_2_17_23_11_23_2315 "Enter ISMACallback 0x%X"
#define CMMB_OPERATION_5339_112_2_17_23_11_23_2316 "mfsh_pts: mf header crc error"
#define CMMB_OPERATION_5355_112_2_17_23_11_23_2317 "mfsh_pts: msf header crc error"
#define CMMB_OPERATION_5376_112_2_17_23_11_23_2318 "GetStreamTSLen: buffer size less than 32B"
#define CMMB_OPERATION_5382_112_2_17_23_11_23_2319 "GetStreamTSLen: seek to begin failed"
#define CMMB_OPERATION_5394_112_2_17_23_11_23_2320 "GetStreamTSLen: file size less than 32B"
#define CMMB_OPERATION_5400_112_2_17_23_11_23_2321 "GetStreamTSLen: file not start with MF"
#define CMMB_OPERATION_5407_112_2_17_23_11_24_2322 "GetStreamTSLen: first MF is MF0"
#define CMMB_OPERATION_5419_112_2_17_23_11_24_2323 "GetStreamTSLen: first MF is incomplete"
#define CMMB_OPERATION_5425_112_2_17_23_11_24_2324 "GetStreamTSLen: no more data after MF1"
#define CMMB_OPERATION_5437_112_2_17_23_11_24_2325 "Find next MF head @%d"
#define CMMB_OPERATION_5442_112_2_17_23_11_24_2326 "GetStreamTSLen: next MF_id(%d) is not the same with previous one(%d)"
#define CMMB_OPERATION_5492_112_2_17_23_11_24_2327 "GetStreamMFPTS: mfsh_pts get error"
#define CMMB_OPERATION_5517_112_2_17_23_11_24_2328 "GetFirstMFPosByPTS: mf_len %d mf_num %d expectpts %X"
#define CMMB_OPERATION_5524_112_2_17_23_11_24_2329 "GetFirstMFPosByPTS: [%d-%d] %d"
#define CMMB_OPERATION_5529_112_2_17_23_11_24_2330 "GetFirstMFPosByPTS: mid pts %X"
#define CSM_122_112_2_17_23_11_24_2331 "CSM Initialization started!\n"
#define CSM_146_112_2_17_23_11_24_2332 " - state lock initialized!\n"
#define CSM_155_112_2_17_23_11_24_2333 " - timer initialized!\n"
#define CSM_160_112_2_17_23_11_24_2334 " - wait4 semaphore initialized!\n"
#define CSM_175_112_2_17_23_11_24_2335 " - thread created!\n"
#define CSM_181_112_2_17_23_11_24_2336 " - service initialized!\n"
#define CSM_183_112_2_17_23_11_24_2337 "CSM Initialization finished!\n"
#define CSM_199_112_2_17_23_11_24_2338 "CSM clean up started!\n"
#define CSM_203_112_2_17_23_11_24_2339 " - timer released!\n"
#define CSM_217_112_2_17_23_11_24_2340 " - order csm thread to exit!\n"
#define CSM_222_112_2_17_23_11_25_2341 " - csm thread exited!\n"
#define CSM_226_112_2_17_23_11_25_2342 " - wait4 semaphore released!\n"
#define CSM_230_112_2_17_23_11_25_2343 " - state lock released!\n"
#define CSM_234_112_2_17_23_11_25_2344 " - service deleted!\n"
#define CSM_236_112_2_17_23_11_25_2345 "CSM clean up finished!\n"
#define CSM_245_112_2_17_23_11_25_2346 "Enter ChannelStopCallback"
#define CSM_289_112_2_17_23_11_25_2347 "CSM_Set: duplicated set! \n"
#define CSM_293_112_2_17_23_11_25_2348 "CSM_Set: active timer!\n"
#define CSM_323_112_2_17_23_11_25_2349 " sync CSM_Set: frequency %d, bandwidth %dn"
#define CSM_365_112_2_17_23_11_25_2350 "CSM_Set: active timer!\n"
#define CSM_472_112_2_17_23_11_25_2351 " sync CSM_Stop: state %d, last_state %dn"
#define CSM_559_112_2_17_23_11_25_2352 "CSM thread started!\n"
#define CSM_566_112_2_17_23_11_25_2353 " CSMThrd: Registered DTL service !\n"
#define CSM_586_112_2_17_23_11_25_2354 " CSMThrd-timer: state %d, last_state %dn"
#define CSM_665_112_2_17_23_11_26_2355 " CSMThrd-set: state %d, last_state %dn"
#define CSM_702_112_2_17_23_11_26_2356 " CSMThrd-stop: state %d, last_state %dn"
#define CSM_715_112_2_17_23_11_26_2357 " CSMThrd-exit: state %d, last_state %dn"
#define CSM_726_112_2_17_23_11_26_2358 " CSMThrd-demod: state %d, last_state %dn"
#define CSM_749_112_2_17_23_11_26_2359 "CSMThrd: Unknow signal id!\n"
#define CSM_762_112_2_17_23_11_26_2360 " CSMThrd: Unregistered DTL service !\n"
#define CSM_780_112_2_17_23_11_26_2361 " CSMThrd: wakeup the one who closed CSM!\n"
#define CSM_849_112_2_17_23_11_26_2362 "CSMSrv: no callback registered for thread %lu, event %lun"
#define CSM_STRATEGY_112_112_2_17_23_11_26_2363 "CSM WATCHDOG: cur_v[%d], warning_v[%d], cnt[%d]!!!\n"
#define CSM_STRATEGY_153_112_2_17_23_11_26_2364 "CSM WATCHDOG: hey! DSP is died!\n"
#define CSM_STRATEGY_227_112_2_17_23_11_27_2365 "CSM STRATEGY: Locked2Prot!\n"
#define CSM_STRATEGY_282_112_2_17_23_11_27_2366 "CSM STRATEGY: Prot2Locked!\n"
#define CSM_STRATEGY_340_112_2_17_23_11_27_2367 "CSM STRATEGY: Prot2Locking - do relock!\n"
#define CSM_STRATEGY_412_112_2_17_23_11_27_2368 "CSM STRATEGY: Locking2Locked!\n"
#define CSM_STRATEGY_465_112_2_17_23_11_27_2369 "CSM STRATEGY: Locking2Error - relock again!\n"
#define CSM_STRATEGY_512_112_2_17_23_11_27_2370 "CSM STRATEGY: Locking2Error!\n"
#define GDAC_68_112_2_17_23_11_27_2371 "GDAC_Init"
#define GDAC_80_112_2_17_23_11_27_2372 "GDAC_Cleanup"
#define GDAC_92_112_2_17_23_11_27_2373 "GDAC_Open"
#define GDAC_104_112_2_17_23_11_28_2374 "GDAC_Close"
#define GDAC_146_112_2_17_23_11_28_2375 "GDAC type %d"
#define HYDAC_75_112_2_17_23_11_28_2376 "\"\"\"\"\"\"\": HYDAC_Open()"
#define HYDAC_V_64_112_2_17_23_11_28_2377 "\"\"\"\"\"\": HYDAC_Init()"
#define MTV_DBC_398_112_2_17_23_11_31_2378 "AllocReceiveBuf: data_buf is null"
#define MTV_DBC_455_112_2_17_23_11_31_2379 "AddData: (rcv_len(%d) + data_len(%d)) < total_len(%d)"
#define MTV_DBC_609_112_2_17_23_11_31_2380 "ProcessStreamInfo: no stream or callback"
#define MTV_DBC_678_112_2_17_23_11_31_2381 "DownloadXPE: parsing error"
#define MTV_DBC_688_112_2_17_23_11_31_2382 "DownloadXPE: isn't first packet"
#define MTV_DBC_712_112_2_17_23_11_31_2383 "DownloadXPE: not the same packet(ctl_trans_id=%d, xpe_trans_id=%d)"
#define MTV_DBC_724_112_2_17_23_11_31_2384 "DownloadXPE: error in last packet len(rcv_len=%d, total_len=%d)"
#define MTV_DBC_747_112_2_17_23_11_32_2385 "DownloadPacket: unknow packet type"
#define MTV_DBC_755_112_2_17_23_11_32_2386 "DownloadPacket: xpe packet finished"
#define MTV_DBC_766_112_2_17_23_11_32_2387 "DownloadPacket: xpe-fec packet finished"
#define MTV_DBC_794_112_2_17_23_11_32_2388 "DownloadPacket: process stream"
#define MTV_DBC_824_112_2_17_23_11_32_2389 "InitDataSlot: succ=%d"
#define MTV_DBC_881_112_2_17_23_11_32_2390 "ConfigDataFilter: sub_type error"
#define MTV_DBC_911_112_2_17_23_11_32_2391 "ConfigDBCFilter: get data filter error(%d), i"
#define MTV_DBC_1009_112_2_17_23_11_32_2392 "MTVDBC_ParseDBCData: buffer_id=%d"
#define MTV_DBC_1012_112_2_17_23_11_32_2393 "MTVDBC_ParseDBCData: not running(state=%d)"
#define MTV_DBC_1028_112_2_17_23_11_32_2394 "MTVDBC_ParseDBCData: error type=%d"
#define MTV_DBC_1040_112_2_17_23_11_32_2395 "MTVDBC_ParseDBCData: no memory"
#define MTV_DBC_1055_112_2_17_23_11_32_2396 "MTVDBC_ParseDBCData: header=0x%x, 0x%x, 0x%x, 0x%x"
#define MTV_DBC_1072_112_2_17_23_11_32_2397 "DBCRegisterToDDM: is_reg=%d"
#define MTV_DBC_1127_112_2_17_23_11_32_2398 "MTVDBC_StartSession: no network info, frequency=%ld"
#define MTV_DBC_1136_112_2_17_23_11_32_2399 "MTVDBC_StartSession"
#define MTV_DBC_1145_112_2_17_23_11_32_2400 "MTVDBC_StartSession: stream info is max"
#define MTV_DBC_1192_112_2_17_23_11_32_2401 "MTVDBC_StartSession: running"
#define MTV_DBC_1212_112_2_17_23_11_33_2402 "MTVDBC_StopSession: no network info, frequency=%ld"
#define MTV_DBC_1217_112_2_17_23_11_33_2403 "MTVDBC_StopSession"
#define MTV_DBC_1268_112_2_17_23_11_33_2404 "MTVDBC_SetStreamNotifyLen: notify_len=%d not in range(0-%d)"
#define MTV_DBC_1276_112_2_17_23_11_33_2405 "MTVDBC_SetStreamNotifyLen: no network info, frequency=%ld"
#define MTV_DBC_STORAGE_85_112_2_17_23_11_33_2406 "MTVDBC_GetXPEInfo: paramer error"
#define MTV_DBC_STORAGE_106_112_2_17_23_11_33_2407 "MTVDBC_GetXPEInfo: not xpe"
#define MTV_DBC_STORAGE_134_112_2_17_23_11_33_2408 "MTVDBC_GetXPEInfo: temp_sum(%d) != check_sum(%d)"
#define MTV_DBC_STORAGE_151_112_2_17_23_11_33_2409 "MTVDBC_GetXPEInfo: begin_flag=%d, end_flag=%d, fec_ind=%d, service_mode=%d, trans_id=%d, payload_len=%d, total_packets_len=%d"
#define MTV_DBC_STORAGE_169_112_2_17_23_11_33_2410 "MTVDBC_GetXPEFECInfo: paramer error"
#define MTV_DBC_STORAGE_189_112_2_17_23_11_33_2411 "MTVDBC_GetXPEFECInfo: not xpe-fec"
#define MTV_DBC_STORAGE_220_112_2_17_23_11_33_2412 "MTVDBC_GetXPEFECInfo: temp_sum(%d) != check_sum(%d)"
#define MTV_DBC_STORAGE_241_112_2_17_23_11_33_2413 "MTVDBC_GetXPEFECInfo: begin_flag=%d, end_flag=%d, trans_id=%d, payload_len=%d, total_packets_len=%d"
#define MTV_DEMUX_519_112_2_17_23_11_34_2414 "DemuxSlotSendData data_to_send 0x%x"
#define MTV_DEMUX_535_112_2_17_23_11_35_2415 "DemuxSlotSendData data_to_send 0x%x,0x%x,0x%x,0x%x"
#define MTV_DEMUX_556_112_2_17_23_11_35_2416 "LocalDemuxSlotSendData data len 0x%x data_to_send 0x%x,is_fr  0x%x"
#define MTV_DEMUX_575_112_2_17_23_11_35_2417 "LocalDemuxSlotSendData data_to_send 0x%x,0x%x"
#define MTV_DEMUX_597_112_2_17_23_11_35_2418 "DemuxSlotSendData data_len 0x%x,0x%x"
#define MTV_DEMUX_600_112_2_17_23_11_35_2419 "ERR data_len != sizeof(NORMAL_SLOT_INFO_T)"
#define MTV_DEMUX_606_112_2_17_23_11_35_2420 "DemuxSlotGetData start 0x%x,0x%x,0x%x,0x%x"
#define MTV_DEMUX_623_112_2_17_23_11_35_2421 "DemuxSlotGetData finish 0x%x,0x%x,0x%x,0x%x"
#define MTV_DEMUX_1243_112_2_17_23_11_36_2422 "KDA mtvdemux Mutex 3\n"
#define MTV_DEMUX_1267_112_2_17_23_11_36_2423 "KDA mtvdemux Mutex 4\n"
#define MTV_DEMUX_1377_112_2_17_23_11_36_2424 "MTVDEMUX AllocBufferForDemux FreeBuffer: tr %d %d tw %d %d"
#define MTV_DEMUX_1778_112_2_17_23_11_37_2425 "FreeBuffer: tr %d %d tw %d %d"
#define MTV_DEMUX_2056_112_2_17_23_11_38_2426 "KDA mtvdemux Mutex 8\n"
#define MTV_DEMUX_3523_112_2_17_23_11_41_2427 "KDA mtvdemux Mutex 11\n"
#define MTV_OPERATION_226_112_2_17_23_11_42_2428 "MTV_SetChannel: unknown channel param %d"
#define MTV_OPERATION_250_112_2_17_23_11_42_2429 "Record: enter record thread"
#define MTV_OPERATION_253_112_2_17_23_11_42_2430 "Record: not in recording state %d"
#define MTV_OPERATION_262_112_2_17_23_11_42_2431 "Record: GetSignal %d %d"
#define MTV_OPERATION_342_112_2_17_23_11_43_2432 "error delete semaphore!"
#define MTV_OPERATION_372_112_2_17_23_11_43_2433 "wait record stop bit %d"
#define MTV_OPERATION_437_112_2_17_23_11_43_2434 "Record ts: GetSignal %d %d"
#define MTV_OPERATION_444_112_2_17_23_11_43_2435 "Record ts: pos %d"
#define MTV_OPERATION_449_112_2_17_23_11_43_2436 "MTV_RecordWriteThread: FFS_fwrite error! %d"
#define MTV_OPERATION_457_112_2_17_23_11_43_2437 "Record ts: write finish %d"
#define MTV_OPERATION_460_112_2_17_23_11_43_2438 "MTV_RecordWriteThread: put semaphore error!"
#define MTV_OPERATION_492_112_2_17_23_11_43_2439 "Record desp: GetSignal %d %d"
#define MTV_OPERATION_498_112_2_17_23_11_43_2440 "Record desp: pos %d"
#define MTV_OPERATION_502_112_2_17_23_11_43_2441 "Record desp: d_offset %d file_offset %d"
#define MTV_OPERATION_509_112_2_17_23_11_43_2442 "MTV_RecordWriteThread: FFS_fwrite error! %d"
#define MTV_OPERATION_517_112_2_17_23_11_43_2443 "Record desp: write finish %d"
#define MTV_OPERATION_520_112_2_17_23_11_43_2444 "MTV_RecordWriteThread: put semaphore error!"
#define MTV_OPERATION_548_112_2_17_23_11_43_2445 "Record write: GetSignal %d %d"
#define MTV_OPERATION_566_112_2_17_23_11_43_2446 "Record desp: d_offset %d file_offset %d"
#define MTV_OPERATION_575_112_2_17_23_11_43_2447 "MTV_RecordWriteThread: FFS_fwrite error! %d"
#define MTV_OPERATION_585_112_2_17_23_11_43_2448 "Record write desp: finish %d"
#define MTV_OPERATION_594_112_2_17_23_11_43_2449 "MTV_RecordWriteThread: FFS_fwrite error! %d"
#define MTV_OPERATION_613_112_2_17_23_11_43_2450 "MTV_RecordWriteThread: FFS_fwrite error! %d"
#define MTV_OPERATION_627_112_2_17_23_11_43_2451 "Record write ts: finish %d"
#define MTV_OPERATION_633_112_2_17_23_11_43_2452 "MTV_RecordWriteThread: put semaphore error!"
#define MTV_OPERATION_639_112_2_17_23_11_43_2453 "Record write: Saved ts %d desp %d"
#define MTV_OPERATION_685_112_2_17_23_11_44_2454 "Record write2: GetSignal %d %d"
#define MTV_OPERATION_703_112_2_17_23_11_44_2455 "Record ts: mfs_len %d ts_len %d file_offset %d"
#define MTV_OPERATION_722_112_2_17_23_11_44_2456 "MTV_RecordWriteThread: FFS_fwrite error! %d"
#define MTV_OPERATION_741_112_2_17_23_11_44_2457 "MTV_RecordWriteThread: FFS_fwrite error! %d"
#define MTV_OPERATION_756_112_2_17_23_11_44_2458 "Record write2 ts: finish %d"
#define MTV_OPERATION_762_112_2_17_23_11_44_2459 "MTV_RecordWriteThread: put semaphore error!"
#define MTV_OPERATION_768_112_2_17_23_11_44_2460 "Record write2: Saved ts %d"
#define MTV_OPERATION_800_112_2_17_23_11_44_2461 "Record: enter record thread"
#define MTV_OPERATION_812_112_2_17_23_11_44_2462 "Record: GetSignal %d %d"
#define MTV_OPERATION_851_112_2_17_23_11_44_2463 "error delete semaphore!"
#define MTV_OPERATION_853_112_2_17_23_11_44_2464 "Record: buf is written"
#define MTV_OPERATION_916_112_2_17_23_11_44_2465 "Record: mfs_len %d"
#define MTV_OPERATION_972_112_2_17_23_11_44_2466 "Record: mfs_len %d exceeds maxmsfsize %d"
#define MTV_OPERATION_983_112_2_17_23_11_44_2467 "error delete semaphore!"
#define MTV_OPERATION_985_112_2_17_23_11_44_2468 "Record: all transfered"
#define MTV_OPERATION_998_112_2_17_23_11_44_2469 "error delete semaphore!"
#define MTV_OPERATION_1000_112_2_17_23_11_44_2470 "Record: buf is written"
#define MTV_OPERATION_1024_112_2_17_23_11_44_2471 "wait record stop bit %d"
#define MTV_OPERATION_1068_112_2_17_23_11_44_2472 "MTV_RecordTSCallback: wrong state %d!!!"
#define MTV_OPERATION_1073_112_2_17_23_11_44_2473 "MTV_RecordTSCallback: wrong type %d!!!"
#define MTV_OPERATION_1078_112_2_17_23_11_44_2474 "MTV_RecordTSCallback: ts_record_file = NULL!!!"
#define MTV_OPERATION_1090_112_2_17_23_11_44_2475 "MTV_RecordTSCallback: SPITV_Dma_Read error!"
#define MTV_OPERATION_1100_112_2_17_23_11_44_2476 "MTV_RecordTSCallback: FFS_fwrite error!"
#define MTV_OPERATION_1111_112_2_17_23_11_44_2477 "MTV_RecordTSCallback: SPITV_Dma_Read error!"
#define MTV_OPERATION_1120_112_2_17_23_11_44_2478 "MTV_RecordTSCallback: FFS_fwrite error!"
#define MTV_OPERATION_1144_112_2_17_23_11_44_2479 "MTV_RecordDescriptorCallback: wrong state %d!!!"
#define MTV_OPERATION_1149_112_2_17_23_11_45_2480 "MTV_RecordDescriptorCallback: wrong type %d!!!"
#define MTV_OPERATION_1154_112_2_17_23_11_45_2481 "MTV_RecordDescriptorCallback: desp_record_file = NULL!!!"
#define MTV_OPERATION_1165_112_2_17_23_11_45_2482 "MTV_RecordTSCallback: SPITV_Dma_Read error!"
#define MTV_OPERATION_1175_112_2_17_23_11_45_2483 "MTV_RecordTSCallback: FFS_fwrite error!"
#define MTV_OPERATION_1187_112_2_17_23_11_45_2484 "MTV_RecordTSCallback: SPITV_Dma_Read error!"
#define MTV_OPERATION_1196_112_2_17_23_11_45_2485 "MTV_RecordTSCallback: FFS_fwrite error!"
#define MTV_OPERATION_1238_112_2_17_23_11_45_2486 "MTV_PlaybackTSCallback: DTL_PostData error!"
#define MTV_OPERATION_1245_112_2_17_23_11_45_2487 "MTV_PlaybackTSCallback: DTL_PostData error!"
#define MTV_OPERATION_1290_112_2_17_23_11_45_2488 "Playback read: GetSignal %d %d"
#define MTV_OPERATION_1307_112_2_17_23_11_45_2489 "Playback read: %d"
#define MTV_OPERATION_1313_112_2_17_23_11_45_2490 "Playback read: %d finish %d"
#define MTV_OPERATION_1316_112_2_17_23_11_45_2491 "MTV_PlaybackReadThread: put semaphore error!"
#define MTV_OPERATION_1356_112_2_17_23_11_45_2492 "Playback: GetSignal %d %d"
#define MTV_OPERATION_1363_112_2_17_23_11_45_2493 "Playback: len[0]=%d len[1]=%d"
#define MTV_OPERATION_1451_112_2_17_23_11_45_2494 "error delete semaphore!"
#define MTV_OPERATION_1453_112_2_17_23_11_45_2495 "playback: all transfered"
#define MTV_OPERATION_1462_112_2_17_23_11_45_2496 "error delete semaphore!"
#define MTV_OPERATION_1464_112_2_17_23_11_45_2497 "playback: buf is read"
#define MTV_OPERATION_1492_112_2_17_23_11_45_2498 "wait playback stop bit %d"
#define MTV_OPERATION_1504_112_2_17_23_11_45_2499 "STOP: playback thread exit"
#define MTV_OPERATION_1548_112_2_17_23_11_45_2500 "MTV_PlaybackTSCallback: wrong state %d!!!"
#define MTV_OPERATION_1553_112_2_17_23_11_45_2501 "MTV_PlaybackTSCallback: ts_play_file = NULL!!!"
#define MTV_OPERATION_1571_112_2_17_23_11_45_2502 "MTV_PlaybackTSCallback: error resp_id %d"
#define MTV_OPERATION_1633_112_2_17_23_11_45_2503 "MTV_PlaybackTSCallback: FFS_fread error!"
#define MTV_OPERATION_1641_112_2_17_23_11_46_2504 "MTV_PlaybackTSCallback: DTL_PostData error!"
#define MTV_OPERATION_1655_112_2_17_23_11_46_2505 "MTV_PlaybackTSCallback: FFS_fread error!"
#define MTV_OPERATION_1663_112_2_17_23_11_46_2506 "MTV_PlaybackTSCallback: DTL_PostData error!"
#define MTV_OPERATION_1692_112_2_17_23_11_46_2507 "DisableRecordCallback wrong msg %d"
#define MTV_OPERATION_1698_112_2_17_23_11_46_2508 "DisableRecordCallback: record state error!!!"
#define MTV_OPERATION_1776_112_2_17_23_11_46_2509 "Enter RecordSaveEntry()"
#define MTV_OPERATION_1780_112_2_17_23_11_46_2510 "RecordSaveEntry: error get ts file size!!!"
#define MTV_OPERATION_1785_112_2_17_23_11_46_2511 "RecordSaveEntry: error get desp file size!!!"
#define MTV_OPERATION_1792_112_2_17_23_11_46_2512 "RecordSaveEntry: Set to file begin failed!!!"
#define MTV_OPERATION_1798_112_2_17_23_11_46_2513 "RecordSaveEntry: Read file header failed!!!"
#define MTV_OPERATION_1805_112_2_17_23_11_46_2514 "RecordSaveEntry: msfh len %d"
#define MTV_OPERATION_1809_112_2_17_23_11_46_2515 "RecordSaveEntry: len %d %d"
#define MTV_OPERATION_1814_112_2_17_23_11_46_2516 "RecordSaveEntry: Set to file begin failed!!!"
#define MTV_OPERATION_1820_112_2_17_23_11_46_2517 "RecordSaveEntry: Write file header failed!!!"
#define MTV_OPERATION_1853_112_2_17_23_11_46_2518 "MTV_RecordTSCallback: FFS_fwrite error!"
#define MTV_OPERATION_1879_112_2_17_23_11_46_2519 "Leave RecordSaveEntry()"
#define MTV_OPERATION_1896_112_2_17_23_11_46_2520 "Enter RecordSaveEntry()"
#define MTV_OPERATION_1917_112_2_17_23_11_46_2521 "Leave RecordSaveEntry()"
#define MTV_OPERATION_1942_112_2_17_23_11_46_2522 "Data: error occured 0x%X!=0x%X @%d"
#define MTV_OPERATION_1981_112_2_17_23_11_46_2523 "Data: error occured 0x%X!=0x%X @%d/%d"
#define MTV_OPERATION_1995_112_2_17_23_11_46_2524 "Record: TS %d 0x%X"
#define MTV_OPERATION_2016_112_2_17_23_11_46_2525 "MTV_RecordThread: put semaphore error!"
#define MTV_OPERATION_2025_112_2_17_23_11_46_2526 "MTV_PlaybackThread: put semaphore error!"
#define MTV_OPERATIONMAN_157_112_2_17_23_11_47_2527 "MTV_Init: vol=%d"
#define MTV_OPERATIONMAN_172_112_2_17_23_11_47_2528 "MTV_Init: arm clock 5"
#define MTV_OPERATIONMAN_176_112_2_17_23_11_47_2529 "MTV_Init: cannot getReqhandler!!!"
#define MTV_OPERATIONMAN_201_112_2_17_23_11_47_2530 "INIT: cost %dms"
#define MTV_OPERATIONMAN_215_112_2_17_23_11_47_2531 "Cleanup: cost %dms"
#define MTV_OPERATIONMAN_227_112_2_17_23_11_47_2532 "MTV_Cleanup: restore arm clock"
#define MTV_OPERATIONMAN_284_112_2_17_23_11_47_2533 "Cleanup: cost %dms"
#define MTV_OPERATIONMAN_296_112_2_17_23_11_47_2534 "MTV_Cleanup: restore arm clock"
#define MTV_OPERATIONMAN_344_112_2_17_23_11_47_2535 "INIT: receive init finish msg"
#define MTV_OPERATIONMAN_347_112_2_17_23_11_47_2536 "MTV_Init: InitCallback state error!!!"
#define MTV_OPERATIONMAN_363_112_2_17_23_11_47_2537 "Cleanup: receive cleanup finish msg"
#define MTV_OPERATIONMAN_366_112_2_17_23_11_47_2538 "MTV_Cleanup: CleanupCallback state error!!!"
#define MTV_STIMIESG_PARSE_693_112_2_17_23_12_14_2539 "ESG:MTVSTIMI_ESGInit. dir  = %s , %x"
#define MTV_STIMIESG_PARSE_719_112_2_17_23_12_14_2540 "ESG:MTVSTIMI_ESGSearchInit: configure buffer."
#define MTV_STIMIESG_PARSE_875_112_2_17_23_12_14_2541 "ESG:MTVSTIMI_ESGS_TEST"
#define MTV_STIMIESG_PARSE_888_112_2_17_23_12_14_2542 "ESG:MTVSTIMI_ESGS_TEST, updating control table, stop!"
#define MTV_STIMIESG_PARSE_924_112_2_17_23_12_14_2543 "ESG:IsFolderExist: find first sfs_handle = %d, attr = 0x%x"
#define MTV_STIMIESG_PARSE_970_112_2_17_23_12_14_2544 "ESG: MTVSTIMI_SearchESGData, net id  =%d, s_dir_name = %s!"
#define MTV_STIMIESG_PARSE_987_112_2_17_23_12_14_2545 "ESG: MTVSTIMI_SearchESGData, nit (%d)data is empty!"
#define MTV_STIMIESG_PARSE_1001_112_2_17_23_12_14_2546 "ESG: MTVSTIMI_SearchESGData, No Control Table!"
#define MTV_STIMIESG_PARSE_1011_112_2_17_23_12_14_2547 "ESG: MTVSTIMI_SearchESGData, sfs_error = %d"
#define MTV_STIMIESG_PARSE_1018_112_2_17_23_12_14_2548 "ESG: MTVSTIMI_SearchESGData, high_word = %d,  low_word= %d!"
#define MTV_STIMIESG_PARSE_1045_112_2_17_23_12_14_2549 "ESG:MTVSTIMI_SearchESGData, searching!"
#define MTV_STIMIESG_PARSE_1067_112_2_17_23_12_14_2550 "ESG:ESG_SearchESGData, No Control Table!"
#define MTV_STIMIESG_PARSE_1089_112_2_17_23_12_14_2551 "ESG:ESG_SearchESGData, searching!"
#define MTV_STIMIESG_PARSE_1115_112_2_17_23_12_14_2552 "ESG:MTVSTIMI_SearchESGData, no searching!"
#define MTV_STIMIESG_PARSE_1132_112_2_17_23_12_14_2553 "ESG:FreeServiceInfoData!"
#define MTV_STIMIESG_PARSE_1185_112_2_17_23_12_15_2554 "ESG:FreeServiceUnderFrq, service_count =%d!"
#define MTV_STIMIESG_PARSE_1238_112_2_17_23_12_15_2555 "ESG:EsgTimerCallback param = %d\n "
#define MTV_STIMIESG_PARSE_1273_112_2_17_23_12_15_2556 "ESG:EsgDataTimerCallback param = %d\n "
#define MTV_STIMIESG_PARSE_1307_112_2_17_23_12_15_2557 "ESG:ReadyForRcvESGData, net_id = %d, state = %d!"
#define MTV_STIMIESG_PARSE_1314_112_2_17_23_12_15_2558 "ESG:ReadyForRcvESGData, ESG_STATE_WAIT_CTRL_TAB!"
#define MTV_STIMIESG_PARSE_1330_112_2_17_23_12_15_2559 "ESG:ReadyForRcvESGData, ESG_STATE_WAIT_SCT!"
#define MTV_STIMIESG_PARSE_1344_112_2_17_23_12_15_2560 "ESG:ReadyForRcvESGData, ESG_STATE_WAIT_NIT!"
#define MTV_STIMIESG_PARSE_1373_112_2_17_23_12_15_2561 "ESG:ReadyForRcvESGData, ESG_STATE_LOOKUP_SRV_FRQ!"
#define MTV_STIMIESG_PARSE_1391_112_2_17_23_12_15_2562 "ESG:ReadyForRcvESGData, ESG_STATE_WAIT_MCT!"
#define MTV_STIMIESG_PARSE_1406_112_2_17_23_12_15_2563 "ESG:ReadyForRcvESGData, ESG_STATE_CONFIG_SERVICE!"
#define MTV_STIMIESG_PARSE_1415_112_2_17_23_12_15_2564 "ESG:ReadyForRcvESGData, ESG_STATE_CONFIG_FILTER!"
#define MTV_STIMIESG_PARSE_1437_112_2_17_23_12_15_2565 "ESG:ReadyForRcvESGData, default state %d!"
#define MTV_STIMIESG_PARSE_1458_112_2_17_23_12_15_2566 "ESG:DisposeESGResult, result = %d"
#define MTV_STIMIESG_PARSE_1651_112_2_17_23_12_15_2567 "ESG:DisposeESGResult, unknown result = %d"
#define MTV_STIMIESG_PARSE_1688_112_2_17_23_12_16_2568 "ESG: IsEnbleOpenESGD, rcv_segment_count %d, segment_count %d!"
#define MTV_STIMIESG_PARSE_1698_112_2_17_23_12_16_2569 "ESG: IsEnbleOpenESGD, mem (%d)  is empty!"
#define MTV_STIMIESG_PARSE_1729_112_2_17_23_12_16_2570 "ESG: IsEnbleOpenCSCT, rcv_segment_count %d, segment_count %d!"
#define MTV_STIMIESG_PARSE_1739_112_2_17_23_12_16_2571 "ESG: IsEnbleOpenCSCT, mem (%d)  is empty!"
#define MTV_STIMIESG_PARSE_1768_112_2_17_23_12_16_2572 "ESG: IsEnbleOpenNIT, nit (%d)data is empty!"
#define MTV_STIMIESG_PARSE_1778_112_2_17_23_12_16_2573 "ESG: IsEnbleOpenNIT, mem (%d)  is empty!"
#define MTV_STIMIESG_PARSE_1860_112_2_17_23_12_16_2574 "ESG: SetupDataBlock, s_esg_service_net_info.data_type_ptr, not enough memory %d!"
#define MTV_STIMIESG_PARSE_1901_112_2_17_23_12_16_2575 "ESG: SetupDataBlock, s_esg_service_net_info.data_type_num = %d !"
#define MTV_STIMIESG_PARSE_1908_112_2_17_23_12_16_2576 "ESG: SetupDataBlock, GetEsgTypeBlockNum is false !"
#define MTV_STIMIESG_PARSE_1918_112_2_17_23_12_16_2577 "ESG: SetupDataBlock, s_esg_service_net_info.data_block_data_ready_ptr, not enough momery %d!"
#define MTV_STIMIESG_PARSE_1929_112_2_17_23_12_16_2578 "ESG: SetupDataBlock, s_data_ready_block_ptr[%d], not enough space %d!"
#define MTV_STIMIESG_PARSE_2024_112_2_17_23_12_16_2579 "ESG: LookupSCT!"
#define MTV_STIMIESG_PARSE_2079_112_2_17_23_12_16_2580 "ESG: LookupNIT!"
#define MTV_STIMIESG_PARSE_2119_112_2_17_23_12_16_2581 "ESG: LookupNIT, unknown result = %d!"
#define MTV_STIMIESG_PARSE_2146_112_2_17_23_12_16_2582 "ESG: LookupSrvFrq, s_frq_init_pos  %d!"
#define MTV_STIMIESG_PARSE_2153_112_2_17_23_12_16_2583 "ESG: LookupSrvFrq, frq_num  %d!"
#define MTV_STIMIESG_PARSE_2193_112_2_17_23_12_17_2584 "ESG: LookupSrvFrq, center_frequency = %d, bandwidth = %d!"
#define MTV_STIMIESG_PARSE_2214_112_2_17_23_12_17_2585 "ESG: SetupSrvUnderFrq!"
#define MTV_STIMIESG_PARSE_2224_112_2_17_23_12_17_2586 "ESG: SetupSrvUnderFrq, net id =0x%x, frq_num = 0x%x!"
#define MTV_STIMIESG_PARSE_2230_112_2_17_23_12_17_2587 "ESG: SetupSrvUnderFrq, s_esg_service_info_under_frq.service_count = 0x%x!"
#define MTV_STIMIESG_PARSE_2290_112_2_17_23_12_17_2588 "ESG: LookupMCT, service count %d!"
#define MTV_STIMIESG_PARSE_2318_112_2_17_23_12_17_2589 "ESG: LookupMCT, result = %d!"
#define MTV_STIMIESG_PARSE_2357_112_2_17_23_12_17_2590 "ESG: ConfigRcvDataService, current_service_idx %d!"
#define MTV_STIMIESG_PARSE_2383_112_2_17_23_12_17_2591 "ESG: ConfigRcvDataService, Center_frequency = %d, Bandwidth_mode =  %d!"
#define MTV_STIMIESG_PARSE_2390_112_2_17_23_12_17_2592 "ESG: ConfigRcvDataService, Byte_interleave_mode = %d, RS_mode =  %d!"
#define MTV_STIMIESG_PARSE_2391_112_2_17_23_12_17_2593 "ESG: ConfigRcvDataService, LDPC_mode = %d, Mapping_mode =  %d!"
#define MTV_STIMIESG_PARSE_2392_112_2_17_23_12_17_2594 "ESG: ConfigRcvDataService, Mux_frame_ID = %d, Scramble_mode =  %d!"
#define MTV_STIMIESG_PARSE_2395_112_2_17_23_12_17_2595 "ESG: ConfigRcvDataService, TS_number = %d, TS_start_index =  %d!"
#define MTV_STIMIESG_PARSE_2404_112_2_17_23_12_17_2596 "ESG: ConfigRcvDataService, param.mux_subframe_id = %d!"
#define MTV_STIMIESG_PARSE_2410_112_2_17_23_12_17_2597 "ESG: ConfigRcvDataService, param.mux_subframe_id = %d!"
#define MTV_STIMIESG_PARSE_2420_112_2_17_23_12_17_2598 "ESG: ConfigRcvDataService, current_service_id %d, chan_pa = %x,demod_par=0x%x!"
#define MTV_STIMIESG_PARSE_2431_112_2_17_23_12_17_2599 "ESG: MTVSTIMI_EnableESG, current_service_id %d, enable = %d!"
#define MTV_STIMIESG_PARSE_2455_112_2_17_23_12_17_2600 "ESG: SaveESGDataInfo, segment_no %d, data_type %d, data_block %d, service_index %d!"
#define MTV_STIMIESG_PARSE_2510_112_2_17_23_12_17_2601 "ESG: SaveESGDataInfo, cur_block_id_list_ptr->next_ptr = PNULL!!!"
#define MTV_STIMIESG_PARSE_2519_112_2_17_23_12_17_2602 "ESG: SaveESGDataInfo, current_block_num = %d!"
#define MTV_STIMIESG_PARSE_2522_112_2_17_23_12_17_2603 "ESG: SaveESGDataInfo, block id = %d!"
#define MTV_STIMIESG_PARSE_2538_112_2_17_23_12_17_2604 "ESG: SaveESGDataInfo 2, cur_block_id_list_ptr->next_ptr = PNULL!!!"
#define MTV_STIMIESG_PARSE_2547_112_2_17_23_12_17_2605 "ESG: SaveESGDataInfo 2, current_block_num = %d!"
#define MTV_STIMIESG_PARSE_2550_112_2_17_23_12_17_2606 "ESG: SaveESGDataInfo 2, block id = %d!"
#define MTV_STIMIESG_PARSE_2594_112_2_17_23_12_17_2607 "ESG:SaveESGServiceInfo: net id = %d."
#define MTV_STIMIESG_PARSE_2620_112_2_17_23_12_17_2608 "ESG: SaveESGServiceInfo(), open ESGD file failed!"
#define MTV_STIMIESG_PARSE_2633_112_2_17_23_12_17_2609 "ESG: SaveESGServiceInfo(), esg_segment = %d!"
#define MTV_STIMIESG_PARSE_2656_112_2_17_23_12_17_2610 "ESG: SaveESGServiceInfo(), esg_service_number = %d!"
#define MTV_STIMIESG_PARSE_2663_112_2_17_23_12_18_2611 "ESG: SaveESGServiceInfo(), serive i = %d, Not Enough Memory!"
#define MTV_STIMIESG_PARSE_2674_112_2_17_23_12_18_2612 "ESG: SaveESGServiceInfo(), service_id = %d!"
#define MTV_STIMIESG_PARSE_2690_112_2_17_23_12_18_2613 "ESG: SaveESGServiceInfo(), data_type_number = %d!"
#define MTV_STIMIESG_PARSE_2699_112_2_17_23_12_18_2614 "ESG: SaveESGServiceInfo(), segment i = %d, data_type, %d, Not Enough Space!"
#define MTV_STIMIESG_PARSE_2719_112_2_17_23_12_18_2615 "ESG: SaveESGServiceInfo(), data type number j = %d, data block not enough memory %d!"
#define MTV_STIMIESG_PARSE_2736_112_2_17_23_12_18_2616 "ESG: SaveESGServiceInfo(), data_block_id = %d, esg_server_index = %d!"
#define MTV_STIMIESG_PARSE_2802_112_2_17_23_12_18_2617 "ESG: SaveESGServiceData,  net id  %d!"
#define MTV_STIMIESG_PARSE_2831_112_2_17_23_12_18_2618 "ESG: SaveESGServiceData,  service_count = %d!"
#define MTV_STIMIESG_PARSE_2838_112_2_17_23_12_18_2619 "ESG: SaveESGServiceData,  not enough memory %d!"
#define MTV_STIMIESG_PARSE_2875_112_2_17_23_12_18_2620 "ESG: SaveESGServiceFrq(),  net id  %d!"
#define MTV_STIMIESG_PARSE_2915_112_2_17_23_12_18_2621 "ESG: SaveESGServiceFrq(),  not enough memory %d!"
#define MTV_STIMIESG_PARSE_2972_112_2_17_23_12_18_2622 "ESG: GetEsgTypeBlockList type = %d, block count = %d"
#define MTV_STIMIESG_PARSE_2976_112_2_17_23_12_18_2623 "ESG: GetEsgTypeBlockList s_service_segment_count= %d"
#define MTV_STIMIESG_PARSE_2980_112_2_17_23_12_18_2624 "ESG: GetEsgTypeBlockList  service_count = %d"
#define MTV_STIMIESG_PARSE_2983_112_2_17_23_12_18_2625 "ESG: GetEsgTypeBlockList  data_type_num = %d"
#define MTV_STIMIESG_PARSE_2989_112_2_17_23_12_18_2626 "ESG: GetEsgTypeBlockList  count = %d, list_ptr->block_count = %d"
#define MTV_STIMIESG_PARSE_3001_112_2_17_23_12_18_2627 "ESG: GetEsgTypeBlockList  total = %d"
#define MTV_STIMIESG_PARSE_3007_112_2_17_23_12_18_2628 "ESG: GetEsgTypeBlockList  list_ptr->block_id_arr[%d]= %d"
#define MTV_STIMIESG_PARSE_3010_112_2_17_23_12_18_2629 "ESG: GetEsgTypeBlockList  left = %d"
#define MTV_STIMIESG_PARSE_3016_112_2_17_23_12_18_2630 "ESG: GetEsgTypeBlockList block_id_list_ptr->next_ptr = PNULL"
#define MTV_STIMIESG_PARSE_3052_112_2_17_23_12_18_2631 "ESG: TotalESGDataBlock!"
#define MTV_STIMIESG_PARSE_3088_112_2_17_23_12_18_2632 "ESG: TotalESGDataBlock, invalid type %d!"
#define MTV_STIMIESG_PARSE_3165_112_2_17_23_12_19_2633 "ESG: GetESGServiceInfoUnderFrq!"
#define MTV_STIMIESG_PARSE_3182_112_2_17_23_12_19_2634 "ESG: GetESGServiceInfoUnderFrq, service_count = 0!"
#define MTV_STIMIESG_PARSE_3192_112_2_17_23_12_19_2635 "ESG: GetESGServiceInfoUnderFrq, esg_service_info_ptr->esg_service_data_info_ptr , not enough memory %d!"
#define MTV_STIMIESG_PARSE_3221_112_2_17_23_12_19_2636 "ESG: ConfigESGDataTypeFilter s_esg_data_filter_num %d!"
#define MTV_STIMIESG_PARSE_3308_112_2_17_23_12_19_2637 "ESG: EnableESGDataTypeFilter!"
#define MTV_STIMIESG_PARSE_3341_112_2_17_23_12_19_2638 "ESG: DisableAllESGFilter!"
#define MTV_STIMIESG_PARSE_3377_112_2_17_23_12_19_2639 "ESG: EnableAllESGFilter!"
#define MTV_STIMIESG_PARSE_3415_112_2_17_23_12_19_2640 "ESG: ConfigESGDataBlockFilter , type %d, block %d, %segment %d, is segment %d!"
#define MTV_STIMIESG_PARSE_3460_112_2_17_23_12_19_2641 "ESG: EnableESGDataBlockFilter ."
#define MTV_STIMIESG_PARSE_3492_112_2_17_23_12_19_2642 "ESG: DeleteESGDataBlockFilter."
#define MTV_STIMIESG_PARSE_3538_112_2_17_23_12_19_2643 "ESG: AddConfigbuff , index = %d, descriptor_count = %d, s_esg_buffer_used_num = %d."
#define MTV_STIMIESG_PARSE_3560_112_2_17_23_12_19_2644 "ESG: DelConfigbuff, index = %d, s_esg_buffer_used_num = %d."
#define MTV_STIMIESG_PARSE_3604_112_2_17_23_12_19_2645 "ESG: IsDataBlockReady, type %d, block %d."
#define MTV_STIMIESG_PARSE_3666_112_2_17_23_12_20_2646 "ESG: IsDataBlockReady(), file head, reading file head failed! error = %d!"
#define MTV_STIMIESG_PARSE_3714_112_2_17_23_12_20_2647 "ESG: IsBlockLinkNodeExisting, type = %d, block = %d, segment = %d."
#define MTV_STIMIESG_PARSE_3719_112_2_17_23_12_20_2648 "ESG: IsBlockLinkNodeExisting, discarded! "
#define MTV_STIMIESG_PARSE_3728_112_2_17_23_12_20_2649 "ESG: IsBlockLinkNodeExisting, no node!"
#define MTV_STIMIESG_PARSE_3741_112_2_17_23_12_20_2650 "ESG: IsBlockLinkNodeExisting, in file!"
#define MTV_STIMIESG_PARSE_3752_112_2_17_23_12_20_2651 "ESG: IsBlockLinkNodeExisting, in buffer!"
#define MTV_STIMIESG_PARSE_3786_112_2_17_23_12_20_2652 "ESG: IsBlockLinkExisting, type = %d, block = %d."
#define MTV_STIMIESG_PARSE_3794_112_2_17_23_12_20_2653 "ESG: IsBlockLinkExisting, no node."
#define MTV_STIMIESG_PARSE_3867_112_2_17_23_12_20_2654 "ESG: ParseESGData, buff id = %d, descriptor_count = %d."
#define MTV_STIMIESG_PARSE_3883_112_2_17_23_12_20_2655 "ESG: ParseESGData, updating control table, stop receiving!"
#define MTV_STIMIESG_PARSE_3911_112_2_17_23_12_20_2656 "ESG: ParseESGData, no buffer, disable all filter!"
#define MTV_STIMIESG_PARSE_3916_112_2_17_23_12_20_2657 "ESG: ParseESGData, buff data addr = 0x%x, decr addr = 0x%x!"
#define MTV_STIMIESG_PARSE_3920_112_2_17_23_12_20_2658 "ESG: ParseESGData, no empty buffer, s_esg_buffer_used_num= %d!"
#define MTV_STIMIESG_PARSE_3933_112_2_17_23_12_20_2659 "ESG: ParseESGData, no memory %d!"
#define MTV_STIMIESG_PARSE_3997_112_2_17_23_12_20_2660 "ESG: ParseESGData, addr = 0x%x, len= %x,block id= %d, d type = %d, seg no = %d, total =%d!"
#define MTV_STIMIESG_PARSE_4002_112_2_17_23_12_20_2661 "ESG: ParseESGData,  existing!"
#define MTV_STIMIESG_PARSE_4012_112_2_17_23_12_20_2662 "ESG: ParseESGData, added!"
#define MTV_STIMIESG_PARSE_4033_112_2_17_23_12_20_2663 "ESG: ParseESGData, valid  descp_count = %d!"
#define MTV_STIMIESG_PARSE_4080_112_2_17_23_12_20_2664 "ESG: ParseESGData,  unknown result = %d!"
#define MTV_STIMIESG_PARSE_4092_112_2_17_23_12_20_2665 "ESG: ParseESGData, left  descp_count = %d!"
#define MTV_STIMIESG_PARSE_4132_112_2_17_23_12_20_2666 "ESG: ParseESGData, no valid data!"
#define MTV_STIMIESG_PARSE_4196_112_2_17_23_12_21_2667 "ESG: ESG_ProcessDemuxData, buff id = %d, descriptor_len = %d."
#define MTV_STIMIESG_PARSE_4199_112_2_17_23_12_21_2668 "ESG: ESG_ProcessDemuxData, searching is end"
#define MTV_STIMIESG_PARSE_4220_112_2_17_23_12_21_2669 "ESG: ESG_ProcessDemuxData, updating control table, stop receiving!"
#define MTV_STIMIESG_PARSE_4259_112_2_17_23_12_21_2670 "ESG: ESG_ProcessDemuxData, addr = 0x%x, len= %x,block id= %d, d type = %d, seg no = %d, total =%d!"
#define MTV_STIMIESG_PARSE_4264_112_2_17_23_12_21_2671 "ESG: ESG_ProcessDemuxData,  existing!"
#define MTV_STIMIESG_PARSE_4273_112_2_17_23_12_21_2672 "ESG: ESG_ProcessDemuxData, added!"
#define MTV_STIMIESG_PARSE_4309_112_2_17_23_12_21_2673 "ESG: ESG_ProcessDemuxData, error = %d!"
#define MTV_STIMIESG_PARSE_4335_112_2_17_23_12_21_2674 "ESG: ESG_ProcessDemuxData, unknown error = %d!"
#define MTV_STIMIESG_PARSE_4398_112_2_17_23_12_21_2675 "ESG: GetSearchESGServiceInfo."
#define MTV_STIMIESG_PARSE_4404_112_2_17_23_12_21_2676 "ESG: GetSearchESGServiceInfo. block_total_count = %d, rcv_block_count = %d"
#define MTV_STIMIESG_PARSE_4419_112_2_17_23_12_21_2677 "ESG: GetSearchESGServiceInfo. complete."
#define MTV_STIMIESG_PARSE_4421_112_2_17_23_12_21_2678 "ESG: GetSearchESGServiceInfo, current_service_id %d!"
#define MTV_STIMIESG_PARSE_4450_112_2_17_23_12_21_2679 "ESG: GetSearchESGServiceInfo. search next frequency."
#define MTV_STIMIESG_PARSE_4460_112_2_17_23_12_21_2680 "ESG: GetSearchESGServiceInfo. search next service."
#define MTV_STIMIESG_PARSE_4469_112_2_17_23_12_21_2681 "ESG: GetSearchESGServiceInfo. wait for data."
#define MTV_STIMIESG_PARSE_4524_112_2_17_23_12_21_2682 "ESG: SaveConfigData , open file failed!"
#define MTV_STIMIESG_PARSE_4536_112_2_17_23_12_21_2683 "ESG: SaveConfigData, write config file failed %d!"
#define MTV_STIMIESG_PARSE_4558_112_2_17_23_12_21_2684 "ESG: SaveConfigData, segment ,Not enough memory! size = %d!"
#define MTV_STIMIESG_PARSE_4566_112_2_17_23_12_21_2685 "ESG: SaveConfigData, create file failed! error = %d!"
#define MTV_STIMIESG_PARSE_4586_112_2_17_23_12_21_2686 "ESG: SaveConfigData, aloc sect data(%d) file failed!"
#define MTV_STIMIESG_PARSE_4597_112_2_17_23_12_21_2687 "ESG: SaveConfigData, aloc seg data(%d) file failed!"
#define MTV_STIMIESG_PARSE_4631_112_2_17_23_12_21_2688 "ESG: GetESGData, need_save_count %d."
#define MTV_STIMIESG_PARSE_4645_112_2_17_23_12_22_2689 "ESG: GetESGData, stop receiving esg data!"
#define MTV_STIMIESG_PARSE_4669_112_2_17_23_12_22_2690 "ESG: GetESGData, valid data in buffer!"
#define MTV_STIMIESG_PARSE_4672_112_2_17_23_12_22_2691 "ESG: GetESGData, desp_index_ptr->desp_list_head!"
#define MTV_STIMIESG_PARSE_4682_112_2_17_23_12_22_2692 "ESG: GetESGData, desp_list_head!"
#define MTV_STIMIESG_PARSE_4693_112_2_17_23_12_22_2693 "ESG: GetESGData, desp_list!"
#define MTV_STIMIESG_PARSE_4715_112_2_17_23_12_22_2694 "ESG: GetESGData, save data failed!"
#define MTV_STIMIESG_PARSE_4782_112_2_17_23_12_22_2695 "ESG: GetLeftSegment!"
#define MTV_STIMIESG_PARSE_4849_112_2_17_23_12_22_2696 "ESG: ConfigESGDataFilter!"
#define MTV_STIMIESG_PARSE_4866_112_2_17_23_12_22_2697 "ESG: ConfigESGDataFilter, data type  =%d  finshed!"
#define MTV_STIMIESG_PARSE_4891_112_2_17_23_12_22_2698 "ESG: ConfigESGDataFilter filter_id %d!"
#define MTV_STIMIESG_PARSE_4908_112_2_17_23_12_22_2699 "ESG: ConfigESGDataFilter, disable_filter_info  =0x%x  !"
#define MTV_STIMIESG_PARSE_5032_112_2_17_23_12_22_2700 "ESG: SaveESGData(), buff_idx = %d!"
#define MTV_STIMIESG_PARSE_5033_112_2_17_23_12_22_2701 "ESG: SaveESGData(), addr = 0x%x, len= %x,block id= %d, d type = %d, seg no = %d, total =%d!"
#define MTV_STIMIESG_PARSE_5048_112_2_17_23_12_22_2702 "ESG: SaveESGData(), fnot enogh memory ! size = %d!"
#define MTV_STIMIESG_PARSE_5083_112_2_17_23_12_22_2703 "ESG: SaveESGData(), segment_len  = %d,  desp.len = %d!"
#define MTV_STIMIESG_PARSE_5086_112_2_17_23_12_22_2704 "ESG: SaveESGData(), segment_len  %d,  desp.len -ESG_SEGMENT_HEAD_SIZE  %d!"
#define MTV_STIMIESG_PARSE_5108_112_2_17_23_12_22_2705 "ESG: SaveESGData(), high_word = %d,  low_word= %d!"
#define MTV_STIMIESG_PARSE_5114_112_2_17_23_12_22_2706 "ESG: SaveESGData(), SFS_GetDeviceFreeSpace, no space  %d!"
#define MTV_STIMIESG_PARSE_5124_112_2_17_23_12_22_2707 "ESG: SaveESGData(), SFS_GetDeviceFreeSpace! error = %d!"
#define MTV_STIMIESG_PARSE_5135_112_2_17_23_12_22_2708 "ESG: SaveESGData(), file head, reading file head failed! error = %d!"
#define MTV_STIMIESG_PARSE_5151_112_2_17_23_12_23_2709 "ESG: SaveESGData(), file head, reading file head failed! error = %d!"
#define MTV_STIMIESG_PARSE_5161_112_2_17_23_12_23_2710 "ESG: SaveESGData(), write file failed! error = %d!"
#define MTV_STIMIESG_PARSE_5175_112_2_17_23_12_23_2711 "ESG: SaveESGData(), file head, reading file head failed! error = %d!"
#define MTV_STIMIESG_PARSE_5185_112_2_17_23_12_23_2712 "ESG: SaveESGData(), write file failed! error = %d!"
#define MTV_STIMIESG_PARSE_5196_112_2_17_23_12_23_2713 "ESG: SaveESGData(), segment_position, SFS_SetFilePointer! error = %d!"
#define MTV_STIMIESG_PARSE_5207_112_2_17_23_12_23_2714 "ESG: SaveESGData(), write file failed! error = %d!"
#define MTV_STIMIESG_PARSE_5231_112_2_17_23_12_23_2715 "ESG: SaveESGData(), segment ,Not enough memory! size = %d!"
#define MTV_STIMIESG_PARSE_5237_112_2_17_23_12_23_2716 "ESG: SaveESGData(), n high_word = %d,  low_word= %d!"
#define MTV_STIMIESG_PARSE_5243_112_2_17_23_12_23_2717 "ESG: SaveESGData(), SFS_GetDeviceFreeSpace, no space  %d!"
#define MTV_STIMIESG_PARSE_5253_112_2_17_23_12_23_2718 "ESG: SaveESGData(), SFS_GetDeviceFreeSpace! error = %d!"
#define MTV_STIMIESG_PARSE_5262_112_2_17_23_12_23_2719 "ESG: SaveESGData(), create file failed! error = %d!"
#define MTV_STIMIESG_PARSE_5273_112_2_17_23_12_23_2720 "ESG: SaveESGData(), write file head failed! error = %d!"
#define MTV_STIMIESG_PARSE_5286_112_2_17_23_12_23_2721 "ESG: SaveESGData(), write head segment failed! error = %d!"
#define MTV_STIMIESG_PARSE_5298_112_2_17_23_12_23_2722 "ESG: SaveESGData(), write file failed! error = %d!"
#define MTV_STIMIESG_PARSE_5349_112_2_17_23_12_23_2723 "ESG: SaveESGDataToMem(), buff_idx = %d!"
#define MTV_STIMIESG_PARSE_5350_112_2_17_23_12_23_2724 "ESG: SaveESGDataToMem(), addr = 0x%x, len= %x,block id= %d, d type = %d, seg no = %d, total =%d!"
#define MTV_STIMIESG_PARSE_5364_112_2_17_23_12_23_2725 "ESG: SaveESGDataToMem(), fnot enogh memory ! size = %d!"
#define MTV_STIMIESG_PARSE_5385_112_2_17_23_12_23_2726 "ESG: SaveESGDataToMem(), segment_len  = %d,  desp.len = %d!"
#define MTV_STIMIESG_PARSE_5388_112_2_17_23_12_23_2727 "ESG: SaveESGDataToMem(), segment_len  %d,  desp.len -ESG_SEGMENT_HEAD_SIZE  %d!"
#define MTV_STIMIESG_PARSE_5429_112_2_17_23_12_23_2728 "ESG: SaveESGDataToMem(), aloc seg data(%d) file failed!"
#define MTV_STIMIESG_PARSE_5468_112_2_17_23_12_23_2729 "ESG: SaveESGDataToMem(), segment ,Not enough memory! size = %d!"
#define MTV_STIMIESG_PARSE_5477_112_2_17_23_12_23_2730 "ESG: SaveESGDataToMem(), create file failed! error = %d!"
#define MTV_STIMIESG_PARSE_5498_112_2_17_23_12_23_2731 "ESG: SaveESGDataToMem(), aloc ect data(%d) file failed!"
#define MTV_STIMIESG_PARSE_5509_112_2_17_23_12_23_2732 "ESG: SaveESGDataToMem(), aloc seg data(%d) file failed!"
#define MTV_STIMIESG_PARSE_5547_112_2_17_23_12_23_2733 "ESG: DecreaseDescriptorCount, buff_idx = %d, s_esg_buffer_used_num = %d!"
#define MTV_STIMIESG_PARSE_5554_112_2_17_23_12_23_2734 "ESG: DecreaseDescriptorCount, buff_idx = %d, descp_count = %d!"
#define MTV_STIMIESG_PARSE_5591_112_2_17_23_12_23_2735 "ESG: SetFinishMark, type = %d, block = %d!"
#define MTV_STIMIESG_PARSE_5601_112_2_17_23_12_23_2736 "ESG: SetFinishMark, s_esg_service_net_info.data_type_num = %d!"
#define MTV_STIMIESG_PARSE_5612_112_2_17_23_12_23_2737 "ESG: SetFinishMark, block_id_list_ptr->block_count = %d!"
#define MTV_STIMIESG_PARSE_5641_112_2_17_23_12_24_2738 "ESG: SetFinishMark, type = %d, index = %d!"
#define MTV_STIMIESG_PARSE_5653_112_2_17_23_12_24_2739 "ESG: SetFinishMark,data_block_data_ready_ptr[%d][%d] = 0x%x!"
#define MTV_STIMIESG_PARSE_5687_112_2_17_23_12_24_2740 "ESG: IsBlockValid, type = %d, block = %d!"
#define MTV_STIMIESG_PARSE_5696_112_2_17_23_12_24_2741 "ESG: IsBlockValid, s_esg_service_net_info.data_type_num = %d!"
#define MTV_STIMIESG_PARSE_5707_112_2_17_23_12_24_2742 "ESG: IsBlockValid, block_id_list_ptr->block_count = %d!"
#define MTV_STIMIESG_PARSE_5713_112_2_17_23_12_24_2743 "ESG: IsBlockValid, valid!"
#define MTV_STIMIESG_PARSE_5736_112_2_17_23_12_24_2744 "ESG: IsBlockValid, invalid!"
#define MTV_STIMIESG_PARSE_5754_112_2_17_23_12_24_2745 "ESG: InsertESGDespInfoList, buff_idx = %d!"
#define MTV_STIMIESG_PARSE_5797_112_2_17_23_12_24_2746 "ESG: InsertESGDespInfoList, discard desp.segment_no =%d!"
#define MTV_STIMIESG_PARSE_5843_112_2_17_23_12_24_2747 "ESG: InsertESGDespInfoList, invalid desp.segment_no =%d!"
#define MTV_STIMIESG_PARSE_5894_112_2_17_23_12_24_2748 "ESG: CreateESGDespInfoList, buff_idx = %d!"
#define MTV_STIMIESG_PARSE_5955_112_2_17_23_12_24_2749 "ESG: AddESGDespInfoListNode, buff_idx = %d!"
#define MTV_STIMIESG_PARSE_6014_112_2_17_23_12_24_2750 "ESG: AddESGDespInfoListNode, discard buff_idx = %d!"
#define MTV_STIMIESG_PARSE_6055_112_2_17_23_12_24_2751 "ESG:RemoveFolderFile:  net id = %d configure buffer."
#define MTV_STIMIESG_PARSE_6072_112_2_17_23_12_24_2752 "ESG:RemoveFolderFile: CreateDirectory  %s = %d."
#define MTV_STIMIESG_PARSE_6079_112_2_17_23_12_24_2753 "ESG:RemoveFolderFile: CreateDirectory  %s = %d."
#define MTV_STIMIESG_PARSE_6108_112_2_17_23_12_24_2754 "ESG:RmvCtrlTab: ControTable."
#define MTV_STIMIESG_PARSE_6141_112_2_17_23_12_25_2755 "ESG: RemoveLeftFile, full_path_len %d!"
#define MTV_STIMIESG_PARSE_6149_112_2_17_23_12_25_2756 "ESG: RemoveLeftFile, SFS_FindFirstFile ,sfs_handle = %d!"
#define MTV_STIMIESG_PARSE_6154_112_2_17_23_12_25_2757 "ESG: RemoveLeftFile, find_data.attr = 0x%x!"
#define MTV_STIMIESG_PARSE_6174_112_2_17_23_12_25_2758 "ESG: RemoveLeftFile, filename_len = 0x%x!"
#define MTV_STIMIESG_PARSE_6181_112_2_17_23_12_25_2759 "ESG: RemoveLeftFile, SFS_DeleteFile = %d "
#define MTV_STIMIESG_PARSE_6186_112_2_17_23_12_25_2760 "ESG: RemoveLeftFile, filename too long %d!"
#define MTV_STIMIESG_PARSE_6208_112_2_17_23_12_25_2761 "ESG:ProcessStimiDisableDataFilterDoneCmd, cmd id!"
#define MTV_STIMIESG_PARSE_6213_112_2_17_23_12_25_2762 "ESG:ProcessStimiDisableDataFilterDoneCmd, ESG_STATE_IDLE!"
#define MTV_STIMIESG_PARSE_6221_112_2_17_23_12_25_2763 "ESG:ProcessStimiDisableDataFilterDoneCmd, ESG_STATE_WAIT_UPDATE!"
#define MTV_STIMIESG_PARSE_6229_112_2_17_23_12_25_2764 "ESG:ProcessStimiDisableDataFilterDoneCmd, ESG_STATE_LOOKUP_SRV_FRQ!"
#define MTV_STIMIESG_PARSE_6287_112_2_17_23_12_25_2765 "ESG:ProcessStimiDisableDataFilterDoneCmd, default state %d!"
#define MTV_STIMIESG_PARSE_6301_112_2_17_23_12_25_2766 "ESG:ProcessStopESG!"
#define MTV_STIMIESG_PARSE_6305_112_2_17_23_12_25_2767 "ESG:ProcessStopESG, current_service_id %d!"
#define MTV_STIMIESG_PARSE_6346_112_2_17_23_12_25_2768 "ESG:ProcessCtrlTabUpdate, s_esg_state = %d!"
#define MTV_STIMIESG_PARSE_6390_112_2_17_23_12_25_2769 "ESG: ProcessCtrlTabUpdate, updated control table!"
#define MTV_STIMIESG_PARSE_6411_112_2_17_23_12_25_2770 "ESG: ProcessCtrlTabUpdate, default %d!"
#define MTV_STIMIESG_PARSE_6426_112_2_17_23_12_25_2771 "ESG:ProcessStimiDisableTunerDoneCmd, resp id %d!"
#define MTV_STIMIESG_PARSE_6431_112_2_17_23_12_25_2772 "ESG:ProcessStimiDisableTunerDoneCmd, ESG_STATE_IDLE!"
#define MTV_STIMIESG_PARSE_6439_112_2_17_23_12_25_2773 "ESG:ProcessStimiDisableTunerDoneCmd, ESG_STATE_LOOKUP_SRV_FRQ!"
#define MTV_STIMIESG_PARSE_6484_112_2_17_23_12_25_2774 "ESG:ProcessStimiDisableTunerDoneCmd, default state %d!"
#define MTV_STIMIESG_PARSE_6502_112_2_17_23_12_25_2775 "ESG:ProcessStimiDisableDataFilterDoneCmd, resp id %d!"
#define MTV_STIMIESG_PARSE_6507_112_2_17_23_12_25_2776 "ESG:ProcessStimiDisableChannelDoneCmd, ESG_STATE_IDLE!"
#define MTV_STIMIESG_PARSE_6515_112_2_17_23_12_25_2777 "ESG:ProcessStimiDisableChannelDoneCmd, ESG_STATE_CONFIG_SERVICE!"
#define MTV_STIMIESG_PARSE_6541_112_2_17_23_12_25_2778 "ESG:ProcessStimiDisableChannelDoneCmd, default state %d!"
#define MTV_STIMIESG_PARSE_6557_112_2_17_23_12_25_2779 "ESG: ProcessInformEsgEnablePlay, is_enable = %d!"
#define MTV_STIMIESG_PARSE_6586_112_2_17_23_12_25_2780 "ESG: FreeDataSlotBuffer!"
#define MTV_STIMIESG_PARSE_6609_112_2_17_23_12_25_2781 "ESG: FreeDataFilter!"
#define MTV_STIMIESG_PARSE_6615_112_2_17_23_12_25_2782 "ESG: FreeDataFilter s_esg_data_filter_num %d!"
#define MTV_STIMIESG_PARSE_6661_112_2_17_23_12_26_2783 "ESG: ExitESGSearch, open filename (0x%x) failed!"
#define MTV_STIMIESG_PARSE_6669_112_2_17_23_12_26_2784 "ESG: ExitESGSearch, read filename size = %d!"
#define MTV_STIMIESG_PARSE_6672_112_2_17_23_12_26_2785 "ESG: ExitESGSearch, read filename size = %d!"
#define MTV_STIMIESG_PARSE_6675_112_2_17_23_12_26_2786 "ESG: ExitESGSearch, read filename size = %d!"
#define MTV_STIMIESG_PARSE_6678_112_2_17_23_12_26_2787 "ESG: ExitESGSearch, read filename size = %d!"
#define MTV_STIMIESG_PARSE_6681_112_2_17_23_12_26_2788 "ESG: ExitESGSearch, read filename size = %d!"
#define MTV_STIMIESG_PARSE_6694_112_2_17_23_12_26_2789 "ESG: ExitESGSearch!"
#define MTV_STIMIESG_PARSE_6759_112_2_17_23_12_26_2790 "ESG: GetESGCodeCharSet!"
#define MTV_STIMIESG_PARSE_6771_112_2_17_23_12_26_2791 "ESG: GetESGCodeCharSet, char_set = %d!"
#define MTV_STIMIESG_PARSE_6792_112_2_17_23_12_26_2792 "ESG: GetESGCodeCharSet, net_id = %d!"
#define MTV_STIMIESG_PARSE_6810_112_2_17_23_12_26_2793 "ESG: GetESGCodeCharSet, file head, reading file head failed! error = %d!"
#define MTV_STIMIESG_PARSE_6820_112_2_17_23_12_26_2794 "ESG: GetESGCodeCharSet, SFS_SetFilePointer failed! error = %d!"
#define MTV_STIMIESG_PARSE_6830_112_2_17_23_12_26_2795 "ESG: GetESGCodeCharSet, SFS_SetFilePointer failed! error = %d!"
#define MTV_STIMIESG_PARSE_6859_112_2_17_23_12_26_2796 "ESG: ConfigProgHintFilter!"
#define MTV_STIMIESG_PARSE_6923_112_2_17_23_12_26_2797 "ESG: MTVSTIMI_SetProgHintInfo, net_id = %d, is_display %d!"
#define MTV_STIMIESG_PARSE_6929_112_2_17_23_12_26_2798 "ESG: MTVSTIMI_SetProgHintInfo, is_display = %d is identical , !"
#define MTV_STIMIESG_PARSE_6961_112_2_17_23_12_26_2799 "ESG: SetProgHintInfo, net_id = %d, is_display %d!"
#define MTV_STIMIESG_PARSE_6987_112_2_17_23_12_26_2800 "ESG: SetProgHintInfo, No found esgd!"
#define MTV_STIMIESG_PARSE_7059_112_2_17_23_12_26_2801 "ESG: ParseProgHintInfo, buff_id = %d, descriptor_count = %d!"
#define MTV_STIMIESG_PARSE_7076_112_2_17_23_12_26_2802 "ESG: ParseProgHintInfo, descp_buff_ptr no enough memory %d!"
#define MTV_STIMIESG_PARSE_7125_112_2_17_23_12_27_2803 "ESG: ParseProgHintInfo, hint_buff_ptrno enough memory %d!"
#define MTV_STIMIESG_PARSE_7152_112_2_17_23_12_27_2804 "ESG: ParseProgHintInfo, data type%d!"
#define MTV_STIMIESG_PARSE_7166_112_2_17_23_12_27_2805 "ESG: ParseProgHintInfo, hint_buff_ptrno enough memory %d!"
#define MTV_STIMIESG_PARSE_7179_112_2_17_23_12_27_2806 "ESG: ParseProgHintInfo, num = %d invalid!"
#define MTV_STIMIESG_PARSE_7215_112_2_17_23_12_27_2807 "ESG: ParseProgHintInfo, hint language invalid!"
#define MTV_STIMIESG_PARSE_7248_112_2_17_23_12_27_2808 "ESG: Hint_ProcessDemuxData, buff_id = %d, descriptor_len = %d!"
#define MTV_STIMIESG_PARSE_7271_112_2_17_23_12_27_2809 "ESG: ParseProgHintInfo, data_unit.unit_type =  %d error!"
#define MTV_STIMIESG_PARSE_7279_112_2_17_23_12_27_2810 "ESG: Hint_ProcessDemuxData, hint_buff_ptrno enough memory %d!"
#define MTV_STIMIESG_PARSE_7305_112_2_17_23_12_27_2811 "ESG: Hint_ProcessDemuxData, data type%d!"
#define MTV_STIMIESG_PARSE_7312_112_2_17_23_12_27_2812 "ESG: Hint_ProcessDemuxData, hint_buff_ptrno enough memory %d!"
#define MTV_STIMIESG_PARSE_7323_112_2_17_23_12_27_2813 "ESG: Hint_ProcessDemuxData, num = %d invalid!"
#define MTV_STIMIESG_PARSE_7360_112_2_17_23_12_27_2814 "ESG: ParseProgHintInfo, hint language invalid!"
#define MTV_STIMIESG_PARSE_7531_112_2_17_23_12_27_2815 "ESG: MTVSTIMI_GetESGDataInfo , net id =  %d, data type = %d!"
#define MTV_STIMIESG_PARSE_7537_112_2_17_23_12_27_2816 "ESG: MTVSTIMI_GetESGDataInfo , Open file(0x%x) failed!"
#define MTV_STIMIESG_PARSE_7546_112_2_17_23_12_27_2817 "ESG: MTVSTIMI_GetESGDataInfo , read file(0x%x) failed!"
#define MTV_STIMIESG_PARSE_7560_112_2_17_23_12_27_2818 "ESG: MTVSTIMI_GetESGDataInfo, invalid type %d!"
#define MTV_STIMIESG_PARSE_7569_112_2_17_23_12_27_2819 "ESG: MTVSTIMI_GetESGDataInfo, file_info_ptr->file_name_ptr, no memory %d!"
#define MTV_STIMIESG_PARSE_7581_112_2_17_23_12_27_2820 "ESG: filename,  found filename = 0x%x"
#define MTV_STIMIESG_PARSE_7594_112_2_17_23_12_27_2821 "ESG: MTVSTIMI_GetESGDataInfo, file_count %d,  file_num %d!"
#define MTV_STIMIESG_PARSE_7612_112_2_17_23_12_27_2822 "ESG: MTVSTIMI_GetESGDataInfo , net id =  %d, data type = %d!"
#define MTV_STIMIESG_PARSE_7623_112_2_17_23_12_28_2823 "ESG: MTVSTIMI_GetESGDataInfo, not ESG config file!"
#define MTV_STIMIESG_PARSE_7633_112_2_17_23_12_28_2824 "ESG: MTVSTIMI_GetESGDataInfo, read config file failed %d!"
#define MTV_STIMIESG_PARSE_7648_112_2_17_23_12_28_2825 "ESG: MTVSTIMI_GetESGDataInfo, invalid type %d!"
#define MTV_STIMIESG_PARSE_7657_112_2_17_23_12_28_2826 "ESG: MTVSTIMI_GetESGDataInfo, file_info_ptr->file_name_ptr, no memory %d!"
#define MTV_STIMIESG_PARSE_7682_112_2_17_23_12_28_2827 "ESG: MTVSTIMI_GetESGDataInfo, find_data.name[%d] =  %d!"
#define MTV_STIMIESG_PARSE_7696_112_2_17_23_12_28_2828 "ESG: MTVSTIMI_GetESGDataInfo ,finished %d"
#define MTV_STIMIESG_PARSE_7715_112_2_17_23_12_28_2829 "ESG: MTVSTIMI_GetESGDataInfo, file_count %d,  file_num %d!"
#define MTV_STIMIESG_PARSE_7737_112_2_17_23_12_28_2830 "ESG: MTVSTIMI_CreateFile,  file_name = 0x%x,  access_mode =  0x%x!"
#define MTV_STIMIESG_PARSE_7745_112_2_17_23_12_28_2831 "ESG: MTVSTIMI_CreateFile,  found file_name = 0x%x"
#define MTV_STIMIESG_PARSE_7757_112_2_17_23_12_28_2832 "ESG: MTVSTIMI_CreateFile,  can't open  file_name = 0x%x"
#define MTV_STIMIESG_PARSE_7770_112_2_17_23_12_28_2833 "ESG: MTVSTIMI_CreateFile,  first, alloc mem( %d) failed"
#define MTV_STIMIESG_PARSE_7793_112_2_17_23_12_28_2834 "ESG: MTVSTIMI_CreateFile,  add, alloc mem( %d) failed"
#define MTV_STIMIESG_PARSE_7821_112_2_17_23_12_28_2835 "ESG: MTVSTIMI_CreateFile,  can't open  multiply "
#define MTV_STIMIESG_PARSE_7841_112_2_17_23_12_28_2836 "ESG: MTVSTIMI_CreateFile,  create existing  file_name = 0x%x"
#define MTV_STIMIESG_PARSE_7845_112_2_17_23_12_28_2837 "ESG: MTVSTIMI_CreateFile,  can't open  multiply "
#define MTV_STIMIESG_PARSE_7896_112_2_17_23_12_28_2838 "ESG: MTVSTIMI_ReadFile,  handle = 0x%x, bytes_to_read = %d!"
#define MTV_STIMIESG_PARSE_7902_112_2_17_23_12_28_2839 "ESG: MTVSTIMI_ReadFile,  rcv_segment_count = %d!"
#define MTV_STIMIESG_PARSE_7923_112_2_17_23_12_28_2840 "ESG: MTVSTIMI_ReadFile,   cur_sect = %d, offset = %d!"
#define MTV_STIMIESG_PARSE_7924_112_2_17_23_12_28_2841 "ESG: MTVSTIMI_ReadFile,   i = %d, segment_len = %d!"
#define MTV_STIMIESG_PARSE_7953_112_2_17_23_12_28_2842 "ESG: MTVSTIMI_ReadFile,   cur_sect = %d, offset = %d!"
#define MTV_STIMIESG_PARSE_7972_112_2_17_23_12_28_2843 "ESG: MTVSTIMI_CloseFile,  handle = 0x%x!"
#define MTV_STIMIESG_PARSE_7982_112_2_17_23_12_28_2844 "ESG: MTVSTIMI_CloseFile,  found file_name = 0x%x"
#define MTV_STIMIESG_PARSE_8006_112_2_17_23_12_28_2845 "ESG: RemoveAllMemFile , net_id = %d!"
#define MTV_STIMIESG_PARSE_8048_112_2_17_23_12_29_2846 "ESG: RemoveAllMemFile, filename = 0x0x!"
#define MTV_STIMIESG_PARSE_8082_112_2_17_23_12_29_2847 "ESG: MTVSTIMI_SetEsgDataPath , esg_path_ptr = %s, esg_path_len =%d!"
#define MTV_STIMIESG_PARSE_8103_112_2_17_23_12_29_2848 "ESG: MTVSTIMI_GetEsgDataPath , esg_path_addr = %x, esg_path_len =%d!"
#define MTV_STIMIESG_PARSE_8147_112_2_17_23_12_29_2849 "ESG: EsgDataCallback buffer_id %d \n"
#define MTV_STIMIESG_PARSE_8170_112_2_17_23_12_29_2850 "ESG: EsgDataCallback descriptor_ptr is null \n"
#define MTV_STIMIESG_PARSE_8176_112_2_17_23_12_29_2851 "ESG: EsgDataCallback mtv_signal_ptr is null \n"
#define MTV_WRAPPER_153_112_2_17_23_12_29_2852 "fake mmi thread has been created!\n"
#define MTV_WRAPPER_156_112_2_17_23_12_29_2853 "ProxyCallPeer has been initialized!\n"
#define MTV_WRAPPER_253_112_2_17_23_12_29_2854 "gdacman has been initialized!\n"
#define MTV_WRAPPER_269_112_2_17_23_12_30_2855 "Delete Thread ERROR! %s %d"
#define MTV_WRAPPER_352_112_2_17_23_12_30_2856 "PlayNetworkByFrq: frequency (%d) cannot get network id"
#define MTV_WRAPPER_896_112_2_17_23_12_31_2857 "mtv_v:CMMB_GetAllProg() nid = %d sid = %d date %d %d %d"
#define MTV_WRAPPER_904_112_2_17_23_12_31_2858 "mtv_v:CMMB_GetAllProg() ret = %d proglist_ptr = 0X%x"
#define MTV_WRAPPER_911_112_2_17_23_12_31_2859 "mtv_v:CMMB_FreeAllProg_wrapper() enter"
#define MTV_WRAPPER_932_112_2_17_23_12_31_2860 "mtv_v:CMMB_GetEsgDateRange_wrapper"
#define MTV_WRAPPER_949_112_2_17_23_12_31_2861 "mtv_v:CMMB_SetEsgFilter_wrapper"
#define MTV_WRAPPER_1002_112_2_17_23_12_31_2862 "MTVWRAPPER:CMMB_SearchProgEndInd, net_num = %d"
#define MTV_WRAPPER_1131_112_2_17_23_12_31_2863 "MTVWRAPPER:CMMB_SearchEsgEndInd_wrapper, "
#define MTV_WRAPPER_1164_112_2_17_23_12_32_2864 "MTVWRAPPER: CMMB_SearchEsgErrInd_wrapper, error = %d,"
#define MTV_WRAPPER_1448_112_2_17_23_12_32_2865 "MTVWRAPPER:CMMB_SearchEsgEndIndForDetail_wrapper, net_id = %d"
#define MTV_WRAPPER_1450_112_2_17_23_12_32_2866 "MTVWRAPPER:MMIMTV_StartParsing, err = %d"
#define MTV_WRAPPER_1651_112_2_17_23_12_32_2867 "CMMB_AbortESG_wrapper enter"
#define MTV_WRAPPER_1669_112_2_17_23_12_33_2868 "fake_mmi recv: 0x%X 0x%X"
#define MTV_WRAPPER_1883_112_2_17_23_12_33_2869 "unhandled id %d"
#define MTV_WRAPPER_2318_112_2_17_23_12_34_2870 "mtvesg MTV_ESG_START_PARSE_IND result=%d"
#define MTV_WRAPPER_2326_112_2_17_23_12_34_2871 "mtvesg MTV_ESG_START_PARSE_IND result=%d"
#define MTV_WRAPPER_2334_112_2_17_23_12_34_2872 "MW, SignalCode=%d\n"
#define MTVESG_COMMON_172_112_2_17_23_12_34_2873 "[ESG]tm_mday = %d,tm_mon = %d,tm_year = %dn"
#define MTVESG_COMMON_230_112_2_17_23_12_34_2874 "[ESG]mtv_MatchServiceID sid = %d"
#define MTVESG_COMMON_233_112_2_17_23_12_34_2875 "[ESG]p_service->ServiceID = %d"
#define MTVESG_COMMON_275_112_2_17_23_12_35_2876 "[ESG]mtv_GetAllEsgProg() sid = %d"
#define MTVESG_COMMON_276_112_2_17_23_12_35_2877 "input year = %d mon = %d day = %d"
#define MTVESG_COMMON_279_112_2_17_23_12_35_2878 "[ESG]mtv_GetAllEsgProg() proglist_ptr is NULL"
#define MTVESG_COMMON_290_112_2_17_23_12_35_2879 "[ESG]mtv_GetAllEsgProg() p_schedule = 0X%x"
#define MTVESG_COMMON_300_112_2_17_23_12_35_2880 "[ESG]mtv_GetAllEsgProg() count = %d"
#define MTVESG_COMMON_305_112_2_17_23_12_35_2881 "mmiesg -- malloc trace:mtv_GetAllEsgProg size = %d"
#define MTVESG_COMMON_341_112_2_17_23_12_35_2882 "[ESG]mtv_GetAllEsgProg() NumOfProg = %d"
#define MTVESG_COMMON_374_112_2_17_23_12_35_2883 "[ESG]mtv_GetEsgProg() sid = %d"
#define MTVESG_COMMON_376_112_2_17_23_12_35_2884 "year = %d mon = %d day = %d hour = %d min = %d"
#define MTVESG_COMMON_379_112_2_17_23_12_35_2885 "[ESG]mtv_GetEsgProg prog_ptr is NULL!!"
#define MTVESG_COMMON_388_112_2_17_23_12_35_2886 "[ESG]mtv_GetEsgProg() p_schedule = 0X%x"
#define MTVESG_COMMON_392_112_2_17_23_12_35_2887 "[ESG]mtv_GetEsgProg() p_schedule id = %d"
#define MTVESG_COMMON_404_112_2_17_23_12_35_2888 "[ESG]mtv_GetEsgProg() p_content = 0X%x"
#define MTVESG_COMMON_432_112_2_17_23_12_35_2889 "[ESG]mtv_GetEsgProg() is_found = %d"
#define MTVESG_COMMON_542_112_2_17_23_12_35_2890 "[ESG]mtv_CheckSpace SFS_GetDeviceFreeSpace, no enouth space"
#define MTVESG_COMMON_548_112_2_17_23_12_35_2891 "[ESG]mtv_CheckSpace SFS_GetDeviceFreeSpace! error = %d!"
#define MTVESG_COMMON_577_112_2_17_23_12_35_2892 "mtvesg -- malloc trace:MMIMTV_CopyString size = %d"
#define MTVESG_COMMON_592_112_2_17_23_12_35_2893 "mtvesg -- malloc trace:MMIMTV_CopyString size = %d"
#define MTVESG_COMMON_607_112_2_17_23_12_35_2894 "mtvesg -- malloc trace:MTV_CopyString size = %d"
#define MTVESG_COMMON_642_112_2_17_23_12_35_2895 "[ESG]MTV_RunReqHandler(): result = %d"
#define MTVESG_COMMON_663_112_2_17_23_12_35_2896 "[ESG]MTV_RequestProgInfo()"
#define MTVESG_COMMON_694_112_2_17_23_12_35_2897 "mtvesg_common.c.c:MTV_GetAllProg()"
#define MTVESG_COMMON_698_112_2_17_23_12_35_2898 "mtvesg_common.c.c:MTV_GetAllProg() proglist_ptr is NULL"
#define MTVESG_COMMON_748_112_2_17_23_12_36_2899 "[ESG]MTV_GetCurProg() type = %d"
#define MTVESG_COMMON_785_112_2_17_23_12_36_2900 "[ESG]MTV_FreeAllProg() listptr=0x%x sid=%d total=%d progptr=0x%x"
#define MTVESG_COMMON_789_112_2_17_23_12_36_2901 "[ESG]Free ServiceName Done"
#define MTVESG_COMMON_801_112_2_17_23_12_36_2902 "[ESG]Free proglist Done"
#define MTVESG_COMMON_842_112_2_17_23_12_36_2903 "[ESG]MTV_GetServiceName() type = %d"
#define MTVESG_COMMON_845_112_2_17_23_12_36_2904 "[ESG]MTV_GetServiceName() name_ptr is NULL!!"
#define MTVESG_COMMON_878_112_2_17_23_12_36_2905 "[ESG]MTV_GetServiceName() ret = %d"
#define MTVESG_COMMON_893_112_2_17_23_12_36_2906 "[ESG]MTV_GetServiceAddinfo()"
#define MTVESG_COMMON_896_112_2_17_23_12_36_2907 "[ESG]MTV_GetServiceAddinfo() info_ptr is NULL"
#define MTVESG_COMMON_915_112_2_17_23_12_36_2908 "[ESG]MTV_GetServiceAddinfo() ret = %d"
#define MTVESG_COMMON_928_112_2_17_23_12_36_2909 "[ESG]MTV_StartParsingAsyn() id = %d"
#define MTVESG_COMMON_972_112_2_17_23_12_36_2910 "[ESG]MTV_StartParsing() id = %d"
#define MTVESG_COMMON_1008_112_2_17_23_12_36_2911 "[ESG]MTV_ParsingService() id = %d"
#define MTVESG_COMMON_1027_112_2_17_23_12_36_2912 "[ESG]MTV_SetParseCallback()"
#define MTVESG_COMMON_1062_112_2_17_23_12_36_2913 "[ESG]mtv_GetDateRange() sid = %d"
#define MTVESG_COMMON_1078_112_2_17_23_12_36_2914 "[ESG]mtv_GetDateRange() count = %d"
#define MTVESG_COMMON_1094_112_2_17_23_12_36_2915 "[ESG]mtv_GetDateRange() init date"
#define MTVESG_COMMON_1135_112_2_17_23_12_36_2916 "[ESG]mtv_GetDateRange() s %d %d %d  e %d %d %d "
#define MTVESG_COMMON_1160_112_2_17_23_12_36_2917 "[ESG]MTV_GetEsgDateRange()"
#define MTVESG_COMMON_1163_112_2_17_23_12_36_2918 "[ESG]MTV_GetEsgDateRange() param error"
#define MTVESG_COMMON_1187_112_2_17_23_12_36_2919 "[ESG]MTV_SetEsgFilter()"
#define MTVESG_COMMON_1204_112_2_17_23_12_36_2920 "[ESG]MTV_DisableEsgFilter()"
#define MTVESG_PARSER_273_112_2_17_23_12_37_2921 "mtvesg_parser.c:expat_Malloc, size=%d"
#define MTVESG_PARSER_284_112_2_17_23_12_37_2922 "mtvesg_parser.c:expat_Malloc ptr=0x%p malloc=%d"
#define MTVESG_PARSER_296_112_2_17_23_12_37_2923 "mtvesg_parser.c:expat_realloc, new size=%d"
#define MTVESG_PARSER_306_112_2_17_23_12_37_2924 "mtvesg_parser.c:expat_realloc, org size=%d"
#define MTVESG_PARSER_320_112_2_17_23_12_37_2925 "mtvesg_parser.c:expat_realloc ptr=0x%p realloc=%d"
#define MTVESG_PARSER_335_112_2_17_23_12_37_2926 "mtvesg_parser.c:expat_Free ptr=0x%p"
#define MTVESG_PARSER_539_112_2_17_23_12_38_2927 "mtvesg -- malloc trace:Esg_SetString size = %d ptr = 0x%x"
#define MTVESG_PARSER_557_112_2_17_23_12_38_2928 "mtvesg -- malloc trace:Esg_SetString size = %d ptr = 0x%x"
#define MTVESG_PARSER_572_112_2_17_23_12_38_2929 "mtvesg_parser.c:Esg_NewElement() el = %d"
#define MTVESG_PARSER_584_112_2_17_23_12_38_2930 "mtvesg_parser.c:Esg_NewElement() wrong element ID"
#define MTVESG_PARSER_591_112_2_17_23_12_38_2931 "mtvesg -- malloc trace:Esg_NewElement size = %d ptr = 0x%x"
#define MTVESG_PARSER_778_112_2_17_23_12_38_2932 "mtvesg_parser.c:Esg_AddElement() el = %d"
#define MTVESG_PARSER_975_112_2_17_23_12_39_2933 "mtvesg_parser.c:Esg_FreeService()"
#define MTVESG_PARSER_1001_112_2_17_23_12_39_2934 "mtvesg_parser.c:Esg_FreeSchedule()"
#define MTVESG_PARSER_1027_112_2_17_23_12_39_2935 "mtvesg_parser.c:Esg_FreeContent()"
#define MTVESG_PARSER_1060_112_2_17_23_12_39_2936 "mtvesg_parser.c:Esg_FreeElementOnError root el = %d"
#define MTVESG_PARSER_1076_112_2_17_23_12_39_2937 "mtvesg_parser.c:Esg_FreeElementOnError wrong element"
#define MTVESG_PARSER_1093_112_2_17_23_12_39_2938 "mtvesg_parser.c:Esg_FreeGlobalVar()"
#define MTVESG_PARSER_1120_112_2_17_23_12_39_2939 "mtvesg_parser.c:Esg_Zcalloc, size=%d ptr = 0x%x"
#define MTVESG_PARSER_1169_112_2_17_23_12_39_2940 "mtvesg_parser.c:Esg_GetFileHeadLen(): len = %d ret = %d"
#define MTVESG_PARSER_1192_112_2_17_23_12_39_2941 "Esg_InflateData() param error!"
#define MTVESG_PARSER_1195_112_2_17_23_12_39_2942 "Esg_InflateData() ,src_handle = %d, dst_handle = %d"
#define MTVESG_PARSER_1206_112_2_17_23_12_39_2943 "Esg_InflateData() ,z_inflateInit2_ = %d!"
#define MTVESG_PARSER_1241_112_2_17_23_12_39_2944 "Esg_InflateData() ,z_inflate = %d!"
#define MTVESG_PARSER_1251_112_2_17_23_12_39_2945 "Esg_InflateData() ,ignore -3 error. now ret = %d!"
#define MTVESG_PARSER_1258_112_2_17_23_12_39_2946 "Esg_InflateData() ,SFS_WriteFile = %d!"
#define MTVESG_PARSER_1289_112_2_17_23_12_39_2947 "Esg_InflateFile get file name fail"
#define MTVESG_PARSER_1311_112_2_17_23_12_39_2948 "Esg_InflateFile create file fail"
#define MTVESG_PARSER_1333_112_2_17_23_12_39_2949 "mtvesg_parser.c:Esg_ParseFileByHandle()"
#define MTVESG_PARSER_1346_112_2_17_23_12_39_2950 "mtvesg_parser.c:Esg_ParseFileByHandle(): create parser fail"
#define MTVESG_PARSER_1369_112_2_17_23_12_39_2951 "mtvesg_parser.c:Parse error at line %dn %sn"
#define MTVESG_PARSER_1381_112_2_17_23_12_39_2952 "mtvesg_parser.c:Esg_ParseFileByHandle(): abort"
#define MTVESG_PARSER_1390_112_2_17_23_12_39_2953 "mtvesg_parser.c:Esg_ParseFileByHandle() do count = %d"
#define MTVESG_PARSER_1418_112_2_17_23_12_40_2954 "mtvesg_parser.c:MTVESG_ParseEsgByType(): type = %d"
#define MTVESG_PARSER_1432_112_2_17_23_12_40_2955 "Esg_ParseFileByHandle Esg_GetFileHeadLen result = %d"
#define MTVESG_PARSER_1455_112_2_17_23_12_40_2956 "Esg_ParseFileByHandle open temp inflate file fail"
#define MTVESG_PARSER_1460_112_2_17_23_12_40_2957 "Esg_ParseFileByHandle delete temp file"
#define MTVESG_PARSER_1466_112_2_17_23_12_40_2958 "Esg_ParseFileByHandle inflate fail"
#define MTVESG_PARSER_1479_112_2_17_23_12_40_2959 "Esg_ParseFileByHandle close esg file"
#define MTVESG_PARSER_1486_112_2_17_23_12_40_2960 "mtvesg_parser.c:MTVESG_ParseEsgByType(): read file fail"
#define MTVESG_PARSER_1492_112_2_17_23_12_40_2961 "mtvesg_parser.c:MTVESG_ParseEsgByType(): user abort"
#define MTVESG_PARSER_1499_112_2_17_23_12_40_2962 "mtvesg_parser.c:MTVESG_ParseEsgByType(): no ESG files"
#define MTVESG_PARSER_1535_112_2_17_23_12_40_2963 "mtvesg_parser.c:Esg_GetDataHeadLen(): filename = %d"
#define MTVESG_PARSER_1537_112_2_17_23_12_40_2964 "mtvesg_parser.c:Esg_GetDataHeadLen(): handle = 0X%x"
#define MTVESG_PARSER_1549_112_2_17_23_12_40_2965 "mtvesg_parser.c:Esg_GetDataHeadLen(): headlen = %d ret = %d"
#define MTVESG_PARSER_1577_112_2_17_23_12_40_2966 "Esg_InflateAndParseData(),filename = %d"
#define MTVESG_PARSER_1579_112_2_17_23_12_40_2967 "mtvesg_parser.c:Esg_InflateAndParseData(): handle = 0X%x"
#define MTVESG_PARSER_1592_112_2_17_23_12_40_2968 "Esg_InflateAndParseData(),z_inflateInit2_ = %d!"
#define MTVESG_PARSER_1598_112_2_17_23_12_40_2969 "Esg_InflateAndParseData(): create parser fail"
#define MTVESG_PARSER_1613_112_2_17_23_12_40_2970 "Esg_InflateAndParseData(),charsread = %d"
#define MTVESG_PARSER_1635_112_2_17_23_12_40_2971 "Esg_InflateAndParseData(),z_inflate = %d!"
#define MTVESG_PARSER_1645_112_2_17_23_12_40_2972 "Esg_InflateAndParseData(),ignore -3 error. now inf_ret = %d!"
#define MTVESG_PARSER_1651_112_2_17_23_12_40_2973 "Esg_InflateAndParseData(),have = %d"
#define MTVESG_PARSER_1654_112_2_17_23_12_40_2974 "Esg_InflateAndParseData(),read_done = %d parse_done = %d"
#define MTVESG_PARSER_1656_112_2_17_23_12_40_2975 "Esg_InflateAndParseData(),xml_ret = %d!"
#define MTVESG_PARSER_1691_112_2_17_23_12_40_2976 "mtvesg_parser.c:Esg_ParseData() len = %d, off = %d done = %d"
#define MTVESG_PARSER_1701_112_2_17_23_12_40_2977 " msg_ptr[%03d] = %02x %02x %02x %02x %02x %02x %02x %02x"
#define MTVESG_PARSER_1717_112_2_17_23_12_40_2978 "mtvesg_parser.c:Parse error at line %dn %sn"
#define MTVESG_PARSER_1741_112_2_17_23_12_40_2979 "mtvesg_parser.c:Esg_ParseDataByHandle() filename = %d"
#define MTVESG_PARSER_1744_112_2_17_23_12_40_2980 "mtvesg_parser.c:Esg_ParseDataByHandle(): handle = 0X%x"
#define MTVESG_PARSER_1749_112_2_17_23_12_40_2981 "mtvesg_parser.c:Esg_ParseDataByHandle(): create parser fail"
#define MTVESG_PARSER_1795_112_2_17_23_12_40_2982 "mtvesg_parser.c:MTVESG_ParseEsgByTypeInMem(): type = %d"
#define MTVESG_PARSER_1825_112_2_17_23_12_40_2983 "mtvesg_parser.c:MTVESG_ParseEsgByTypeInMem(): no ESG data"
#define MTVESG_PARSER_1856_112_2_17_23_12_40_2984 "mtvesg_parser.c:MTVESG_InitGlobalVar()"
#define MTVESG_PARSER_1909_112_2_17_23_12_41_2985 "MTVESG_ParseThreadInit tid = %d"
#define MTVESG_PARSER_1952_112_2_17_23_12_41_2986 "esg_ParseThreadEntry sig_ptr is NULL!!"
#define MTVESG_PARSER_1959_112_2_17_23_12_41_2987 "esg_ParseThreadEntry ESGPARASER_SIG_PARSE"
#define MTVESG_PARSER_1991_112_2_17_23_12_41_2988 "esg_ParseThreadEntry ESGPARASER_SIG_EXIT"
#define MTVESG_PARSER_1995_112_2_17_23_12_41_2989 "esg_ParseThreadEntry Unknow signal id!"
#define MTVESG_PARSER_2013_112_2_17_23_12_41_2990 "esg_ParseThreadEntry thread exit"
#define MTVESG_PARSER_2023_112_2_17_23_12_41_2991 "mtvesg_parser.c:ESG_IsAbort() = %d"
#define PPBUFFER_75_112_2_17_23_12_41_2992 "MTVAUDIO_Init: allocate memory failed."
#define PPBUFFER_140_112_2_17_23_12_41_2993 "PPBUFFER: in buffer not full"
#define PPBUFFER_142_112_2_17_23_12_41_2994 "PPBUFFER: out buffer not empty"
#define PPBUFFER_162_112_2_17_23_12_41_2995 "PPBUFFER: PrepareInBuffer buffer is not empty"
#define PPBUFFER_170_112_2_17_23_12_41_2996 "PPBUFFER: PrepareInBuffer allocate larger buffer %d=>%d"
#define PPBUFFER_173_112_2_17_23_12_41_2997 "PPBUFFER: PrepareInBuffer buf_size 0x%x larger than 0x2000!!!"
#define PPBUFFER_180_112_2_17_23_12_41_2998 "PPBUFFER: buf ptr is NULL!!!"
#define SPI_AUDIO_530_112_2_17_23_12_43_2999 "SPIAUDIO: W 0x%X[%d]"
#define SYNC_FUNC_99_112_2_17_23_12_44_3000 "SyncFunc recvACK: 0x%X"
#define APDAC_125_112_2_17_23_12_46_3001 "apdac: %d"
#define DTL_RFVT_219_112_2_17_23_12_47_3002 "DTLRFVT_Callback resp_cmd_ptr->resp_id=0x%x"
#define DTL_RFVT_247_112_2_17_23_12_47_3003 "DTLRFVT_Callback wrong resp id"
#define DTL_RFVT_250_112_2_17_23_12_47_3004 "DTLRFVT_Callback subtype = %d"
#define DTL_RFVT_281_112_2_17_23_12_47_3005 "[RFTEST]:DTLRFVT_HandleToolMsg src_ptr = %x, msg_head->subtype=%d, src_len = %d"
#define DTL_RFVT_289_112_2_17_23_12_47_3006 "[RFTEST]:DTLRFVT_HandleToolMsg FREQ = %d"
#define DTL_RFVT_295_112_2_17_23_12_47_3007 "[RFTEST]:DTLRFVT_HandleToolMsg RSSI = %d"
#define DTL_RFVT_308_112_2_17_23_12_47_3008 "[RFTEST]:DTLRFVT_HandleToolMsg ret = %d BER = %d SNR = %d"
#define DTL_RFVT_312_112_2_17_23_12_47_3009 "[RFTEST]:DTLRFVT_HandleToolMsg Reset Mode"
#define DTL_RFVT_320_112_2_17_23_12_47_3010 "[RFTEST]:DTLRFVT_HandleToolMsg Reset Mode wrong mode"
#define DTL_RFVT_327_112_2_17_23_12_47_3011 "[RFTEST]:DTLRFVT_HandleToolMsg Get Mode = %d"
#define DTL_RFVT_332_112_2_17_23_12_47_3012 "DTLRFVT_HandleToolMsg Read NV"
#define DTL_RFVT_360_112_2_17_23_12_47_3013 "DTLRFVT_HandleToolMsg Write NV %d %d %d %d %d %d %d %d %d"
#define DTL_RFVT_366_112_2_17_23_12_47_3014 "[RFTEST]:DTLRFVT_HandleToolMsg dvt test task id = %d"
#define DTL_RFVT_398_112_2_17_23_12_47_3015 "[RFTEST]:DTLRFVT_HandleToolMsg CMMBSN start"
#define DTL_RFVT_400_112_2_17_23_12_47_3016 "[RFTEST]:DTLRFVT_HandleToolMsg Get CMMBSN ret = %d"
#define DTL_RFVT_406_112_2_17_23_12_47_3017 "[RFTEST]:DTLRFVT_HandleToolMsg wrong diag command"
#define DTL_RFVT_437_112_2_17_23_12_47_3018 "[RFTEST]:dtlrfvt_SendData subtype = %d"
#define DTL_RFVT_452_112_2_17_23_12_47_3019 "[RFTEST]: msg_ptr[%03d] = %02x %02x %02x %02x %02x %02x %02x %02x"
#define DTL_RFVT_476_112_2_17_23_12_48_3020 "[RFTEST]:DTLRFVT_ResetMode mode = %d current mode = %d"
#define DTL_RFVT_486_112_2_17_23_12_48_3021 "[RFTEST]:DTLRFVT_ResetMode not support normal mode"
#define DTL_RFVT_514_112_2_17_23_12_48_3022 "[RFTEST]:DTLRFVT_ResetMode wrong command id"
#define DTL_RFVT_531_112_2_17_23_12_48_3023 "[RFTEST]: rftest_Rssi freq = %d tick = %d"
#define DTL_RFVT_536_112_2_17_23_12_48_3024 "[RFTEST]: rftest_Rssi already in play mode  cur mode=%d"
#define DTL_RFVT_542_112_2_17_23_12_48_3025 "[RFTEST]: rftest_Rssi CMMB_SearchCenterFrq error"
#define DTL_RFVT_568_112_2_17_23_12_48_3026 "DTLRFVT_StartPlayNetWork already in play mode, return"
#define DTL_RFVT_573_112_2_17_23_12_48_3027 "DTLRFVT_StartPlayNetWork need to poweroff seine"
#define DTL_RFVT_576_112_2_17_23_12_48_3028 "DTLRFVT_StartPlayNetWork freq = %d, alpha = %d"
#define DTL_RFVT_579_112_2_17_23_12_48_3029 "DTLRFVT_StartPlayNetWork thread_id = %d"
#define DTL_RFVT_598_112_2_17_23_12_48_3030 "DTLRFVT_StartPlayNetWork play init"
#define DTL_RFVT_611_112_2_17_23_12_48_3031 "DTLRFVT_StartPlayNetWork start play"
#define DTL_RFVT_630_112_2_17_23_12_48_3032 "DTLRFVT_StartPlayNetWork play state = %d"
#define DTL_RFVT_635_112_2_17_23_12_48_3033 "DTLRFVT_StartPlayNetWork time out"
#define DTL_RFVT_677_112_2_17_23_12_48_3034 "DTLRFVT_StopPlayNetWork mode = %d"
#define DTL_RFVT_684_112_2_17_23_12_48_3035 "DTLRFVT_StopPlayNetWork stop state = %d"
#define DTL_RFVT_692_112_2_17_23_12_48_3036 "DTLRFVT_StopPlayNetWork not in play mode, return"
#define DTL_RFVT_800_112_2_17_23_12_48_3037 "DTLRFVT_StopPlayBackFile()"
#define DTL_RFVT_815_112_2_17_23_12_48_3038 "DTLRFVT_StopPlayBackFile() END"
#define DTL_RFVT_863_112_2_17_23_12_48_3039 "[RFTEST]:dtl_InitSeine tick = %d"
#define DTL_RFVT_865_112_2_17_23_12_48_3040 "[RFTEST]: create memory fail"
#define DTL_RFVT_868_112_2_17_23_12_48_3041 "[RFTEST]: create memory ok"
#define DTL_RFVT_871_112_2_17_23_12_48_3042 "[RFTEST]: MTV_Initialize ok"
#define DTL_RFVT_873_112_2_17_23_12_48_3043 "[RFTEST]: CMMB_Init ok"
#define DTL_RFVT_877_112_2_17_23_12_48_3044 "[RFTEST]:dtl_InitSeine done tick = %d"
#define DTL_RFVT_881_112_2_17_23_12_48_3045 "[RFTEST]:dtl_InitSeine already in normal mode, return"
#define DTL_RFVT_898_112_2_17_23_12_48_3046 "[RFTEST]:dtl_CloseSeine"
#define DTL_RFVT_908_112_2_17_23_12_48_3047 "[RFTEST]:dtl_CloseSeine already in poweroff mode, return"
#define DTL_RFVT_929_112_2_17_23_12_48_3048 "[RFTEST]:rftest_CallBack ack_id =0x%x,p0 =0x%x, p1 =0x%x\n "
#define DTL_RFVT_933_112_2_17_23_12_48_3049 "[RFTEST]:received MTV_STIMI_SEARCH_PROG_END_CNF msg tick=%d"
#define DTL_RFVT_938_112_2_17_23_12_48_3050 "[RFTEST]: CMMB_GetNetworkInfoList net num=%d."
#define DTL_RFVT_950_112_2_17_23_12_48_3051 "[RFTEST]: CMMB_GetNetworkInfoList service num=%d."
#define DTL_RFVT_955_112_2_17_23_12_49_3052 "[RFTEST]: service = %d  is free:%d"
#define DTL_RFVT_969_112_2_17_23_12_49_3053 "[RFTEST]: CMMB_PlayNetwork %d %d"
#define DTL_RFVT_972_112_2_17_23_12_49_3054 "[RFTEST]: CMMB_PlayNetwork success"
#define DTL_RFVT_982_112_2_17_23_12_49_3055 "[RFTEST]: CMMB_GetNetworkInfoList failed"
#define DTL_RFVT_989_112_2_17_23_12_49_3056 "[RFTEST]: received MTV_STIMI_SEARCH_PROG_ERR_IND msg"
#define DTL_RFVT_1007_112_2_17_23_12_49_3057 "[RFTEST]: received CMMB_PlayNetwork msg ack_id=%d, tick=%d"
#define DTL_RFVT_1011_112_2_17_23_12_49_3058 "[RFTEST]: received MTV_STOP_ACK msg"
#define DTL_RFVT_1039_112_2_17_23_12_49_3059 "[RFTEST]: rftest_GetChannelStatus sync_stat=%d org BER=%d"
#define DTL_RFVT_1048_112_2_17_23_12_49_3060 "[RFTEST]:rftest_GetChannelStatus NOT LOCK"
#define DTL_RFVT_1053_112_2_17_23_12_49_3061 "[RFTEST]:rftest_GetChannelStatus wrong state"
#define DTL_RFVT_1058_112_2_17_23_12_49_3062 "[RFTEST]:rftest_GetChannelStatus parama error"
#define DTL_RFVT_1072_112_2_17_23_12_49_3063 "[RFTEST]:rftest_GetRssiChannelStatus state=%d"
#define DTL_RFVT_1080_112_2_17_23_12_49_3064 "[RFTEST]: rftest_GetRssiChannelStatus ret=%d"
#define DTL_RFVT_1084_112_2_17_23_12_49_3065 "[RFTEST]:rftest_GetRssiChannelStatus wrong state"
#define DTL_RFVT_1089_112_2_17_23_12_49_3066 "[RFTEST]:rftest_GetRssiChannelStatus parama error"
#define DTL_RFVT_1103_112_2_17_23_12_49_3067 "dtl_InitSeineCallback: receive init finish msg"
#define DTL_RFVT_1122_112_2_17_23_12_49_3068 "dtl_SleepSeine current mode = %d"
#define DTL_RFVT_1125_112_2_17_23_12_49_3069 "dtl_SleepSeine need init seine"
#define DTL_RFVT_1128_112_2_17_23_12_49_3070 "dtl_SleepSeine send sleep CMD"
#define DTL_RFVT_1130_112_2_17_23_12_49_3071 "dtl_SleepSeine start waiting"
#define DTL_RFVT_1133_112_2_17_23_12_49_3072 "dtl_SleepSeine end waiting"
#define DTL_RFVT_1187_112_2_17_23_12_49_3073 "[RFTEST]:dtl_DvtSendSignal subtype = %d msg = %d dest id = %d"
#define DTL_RFVT_1219_112_2_17_23_12_49_3074 "[RFTEST]:dtl_handleSearchEndCnf"
#define DTL_RFVT_1222_112_2_17_23_12_49_3075 "[RFTEST]:dtl_handleSearchEndCnf net_number = %d."
#define DTL_RFVT_1235_112_2_17_23_12_49_3076 "[RFTEST]:dtl_handleSearchEndCnf service number = %d"
#define DTL_RFVT_1256_112_2_17_23_12_49_3077 "[RFTEST]:dtl_handleSearchEndCnf no service info"
#define DTL_RFVT_1261_112_2_17_23_12_49_3078 "[RFTEST]:dtl_handleSearchEndCnf Done!!"
#define DTL_RFVT_1267_112_2_17_23_12_49_3079 "[RFTEST]:dtl_handleSearchEndCnf no network info"
#define DTL_RFVT_1281_112_2_17_23_12_49_3080 "[RFTEST]:DTLRFVT_HandleSearchNetwork mem1 = %x mem2 = %x"
#define DTL_RFVT_1285_112_2_17_23_12_49_3081 "[RFTEST]:DTLRFVT_HandleSearchNetwork run in cal mode"
#define DTL_RFVT_1311_112_2_17_23_12_49_3082 "dtl_StartSearchNetwork mem1 = %x, mem2 = %x"
#define DTL_RFVT_1314_112_2_17_23_12_49_3083 "dtl_StartSearchNetwork need to poweroff seine"
#define DTL_RFVT_1318_112_2_17_23_12_49_3084 "dtl_StartSearchNetwork init"
#define DTL_RFVT_1340_112_2_17_23_12_49_3085 "dtl_StartSearchNetwork start search  task = %d"
#define DTL_RFVT_1346_112_2_17_23_12_49_3086 "dtl_StartSearchNetwork search fail"
#define DTL_RFVT_1366_112_2_17_23_12_49_3087 "dtl_exit_tv"
#define DTL_RFVT_1396_112_2_17_23_12_49_3088 "[RFTEST]:DTLRFVT_HandlePlayByParam para 1 = %d 2 = %d 3 = %d mem1 = %x mem2 = %x"
#define DTL_RFVT_1402_112_2_17_23_12_49_3089 "[RFTEST]:DTLRFVT_HandlePlayByParam run in cal mode"
#define DTL_RFVT_1412_112_2_17_23_12_49_3090 "[RFTEST]:DTLRFVT_HandlePlayByParam run in normal mode"
#define DTL_RFVT_1439_112_2_17_23_12_49_3091 "DTLRFVT_PlayNetWorkByParam mem1 = %x, mem2 = %x"
#define DTL_RFVT_1442_112_2_17_23_12_49_3092 "DTLRFVT_PlayNetWorkByParam already in play mode, return"
#define DTL_RFVT_1447_112_2_17_23_12_50_3093 "DTLRFVT_PlayNetWorkByParam need to poweroff seine"
#define DTL_RFVT_1452_112_2_17_23_12_50_3094 "DTLRFVT_PlayNetWorkByParam init_ret = %d"
#define DTL_RFVT_1468_112_2_17_23_12_50_3095 "DTLRFVT_PlayNetWorkByParam net_id = %d service id = %d"
#define DTL_RFVT_1470_112_2_17_23_12_50_3096 "DTLRFVT_PlayNetWorkByParam play init"
#define DTL_RFVT_1493_112_2_17_23_12_50_3097 "DTLRFVT_PlayNetWorkByParam get param error"
#define DTL_RFVT_1499_112_2_17_23_12_50_3098 "DTLRFVT_PlayNetWorkByParam start play"
#define DTL_RFVT_1503_112_2_17_23_12_50_3099 "DTLRFVT_PlayNetWorkByParam play state = %d"
#define DTL_RFVT_1508_112_2_17_23_12_50_3100 "DTLRFVT_PlayNetWorkByParam time out"
#define DTL_RFVT_1533_112_2_17_23_12_50_3101 "DTLRFVT_StopPlayNetWorkByParam stop"
#define DTL_RFVT_1536_112_2_17_23_12_50_3102 "DTLRFVT_StopPlayNetWorkByParam stop state = %d"
#define DTL_RFVT_1543_112_2_17_23_12_50_3103 "DTLRFVT_StopPlayNetWorkByParam not in play mode, return"
#define DTL_RFVT_1596_112_2_17_23_12_50_3104 "[RFTEST]:dvt_ThreadEntry sig_ptr is NULL!!"
#define DTL_RFVT_1600_112_2_17_23_12_50_3105 "[RFTEST]:dvt_ThreadEntry signalcode = %d"
#define DTL_RFVT_1604_112_2_17_23_12_50_3106 "[RFTEST]:dvt_ThreadEntry subtype = %d"
#define DTL_RFVT_1625_112_2_17_23_12_50_3107 "[RFTEST]:dvt_ThreadEntry receive MTV_PLAY_ACK"
#define DTL_RFVT_1632_112_2_17_23_12_50_3108 "[RFTEST]:dvt_ThreadEntry receive MTV_PLAY_ERROR"
#define DTL_RFVT_1639_112_2_17_23_12_50_3109 "[RFTEST]:dvt_ThreadEntry receive MTV_STOP_ACK"
#define DTL_RFVT_1643_112_2_17_23_12_50_3110 "[RFTEST]:dvt_ThreadEntry receive MTV_STOP_ERROR"
#define DTL_RFVT_1648_112_2_17_23_12_50_3111 "[RFTEST]: dvt_ThreadEntry play success start sleep"
#define DTL_RFVT_1651_112_2_17_23_12_50_3112 "[RFTEST]: dvt_ThreadEntry play success sleep end"
#define DTL_RFVT_1654_112_2_17_23_12_50_3113 "[RFTEST]:dvt_ThreadEntry receive MTV_STIMI_SEARCH_PROG_ERR_IND"
#define DTL_RFVT_1685_112_2_17_23_12_50_3114 "[RFTEST]:DTLRFVT_ThreadInit task id = %d"
#define DTL_RFVT_1719_112_2_17_23_12_50_3115 "[RFTEST]:rftest_GetCMMBSN enter"
#define DTL_RFVT_1731_112_2_17_23_12_50_3116 "[RFTEST]:rftest_GetCMMBSN = %s"
#define DTL_RFVT_1735_112_2_17_23_12_50_3117 "[RFTEST]:rftest_GetCMMBSN fail!!!"
#define DTL_RFVT_1740_112_2_17_23_12_50_3118 "[RFTEST]: rftest_GetCMMBSN MUACMD_Open fail!!!"
#define DTL_RFVT_1746_112_2_17_23_12_50_3119 "[RFTEST]:rftest_GetCMMBSN wrong state"
#define DTL_RFVT_1765_112_2_17_23_12_50_3120 "[RFTEST]: rftest_CheckSMD ret=%d"
#define DTL_RFVT_1769_112_2_17_23_12_50_3121 "[RFTEST]:rftest_CheckSMD wrong state"
#define DTL_RFVT_1781_112_2_17_23_12_50_3122 "[RFTEST]: rftest_CheckSMD ret=%d"
#define DTL_RFVT_1805_112_2_17_23_12_50_3123 "dtl_CalcRssiOffset freq = %d"
#define DTL_RFVT_1809_112_2_17_23_12_50_3124 "dtl_CalcRssiOffset index = %d"
#define MTV_COMMON_FUNC_65_112_2_17_23_12_52_3125 "MTV_CombineFullPath param error"
#define MTV_COMMON_FUNC_130_112_2_17_23_12_52_3126 "[SG]: MTV_CombineFullPath, 0x%x,0x%x"
#define MTV_COMMON_FUNC_149_112_2_17_23_12_52_3127 "MTV_IsFolderExist param error"
#define MTV_SETTING_151_112_2_17_23_12_54_3128 "MTV_SetCommConfig V log open"
#define MTV_SETTING_156_112_2_17_23_12_54_3129 "MTV_SetCommConfig V log close"
#define MTV_SETTING_162_112_2_17_23_12_54_3130 "MTV_SetCommConfig V log to BB open"
#define MTV_SETTING_167_112_2_17_23_12_54_3131 "MTV_SetCommConfig V log to BB close"
#define MTV_SETTING_195_112_2_17_23_12_54_3132 "[MTVLOG]MTV_CreateLogfolder ret=%d"
#define MTV_SETTING_210_112_2_17_23_12_54_3133 "[MTVLOG]MTV_CreateLogFile !"
#define MTV_SETTING_213_112_2_17_23_12_54_3134 "[MTVLOG]MTV_CreateLogFile arm handle=%x"
#define TV_FIRMWARE_291_112_2_17_23_12_55_3135 "Parse_FW_Head:%x:%x,%x"
#define TV_FIRMWARE_337_112_2_17_23_12_55_3136 "GZIP_FWID: %d zip_data  0x%x  :%d"
#define TV_FIRMWARE_344_112_2_17_23_12_55_3137 "GZIP_FWID: %d offset  0x%x  :%d"
#define TV_FIRMWARE_347_112_2_17_23_12_55_3138 "GZIP_FW: 0x%x  :%d"
#define TV_FIRMWARE_351_112_2_17_23_12_55_3139 "TVBOOT_TVOpen()download_gzip CODE_IN_SDRAM error!\n"
#define TV_FIRMWARE_379_112_2_17_23_12_55_3140 "NORMAL_FWID %d  0x%x  :%d"
#define TV_FIRMWARE_383_112_2_17_23_12_55_3141 "TVBOOT_TVOpen() normal download %d  error!\n"
#define MTV_LOG_279_112_2_17_23_13_2_3142 "[MTVLOG]mtvlog_SetLogFlag type=%d"
#define MTV_LOG_292_112_2_17_23_13_2_3143 "[MTVLOG]mtvlog_SetLogFlag wrong type"
#define MTV_LOG_339_112_2_17_23_13_2_3144 "mtvlog_DSPTrace src_len = %d"
#define MTV_LOG_351_112_2_17_23_13_2_3145 "mtvlog_DSPTrace src_len = %d send_len = %d, offset = %d"
#define MTV_LOG_353_112_2_17_23_13_2_3146 "mtvlog_DSPTrace status = %d"
#define MTV_LOG_372_112_2_17_23_13_2_3147 " data_ptr[%03d] = %02x %02x %02x %02x %02x %02x %02x %02x"
#define MTV_LOG_392_112_2_17_23_13_2_3148 "[MTVLOG]mtvlog_ProcessData data_ptr = %x, data_len = %d,log_type = %d"
#define MTV_LOG_415_112_2_17_23_13_2_3149 "mtvlog_ProcessData ERROR!! log.log_length = %d, offset = %d"
#define MTV_LOG_440_112_2_17_23_13_2_3150 "[MTVLOG]mtvlog_ProcessData Wrong log_type!!"
#define MTV_LOG_474_112_2_17_23_13_2_3151 "[MTVLOG]mtvlog_LogARMMsg Do nothing!!"
#define MTV_LOG_639_112_2_17_23_13_2_3152 "mtvlog_HandleARM \n"
#define MTV_LOG_643_112_2_17_23_13_2_3153 "mtvlog_HandleARM read_pos = %d, write_pos = %d "
#define MTV_LOG_655_112_2_17_23_13_2_3154 "mtvlog_HandleARM data_length error!!!"
#define MTV_LOG_661_112_2_17_23_13_2_3155 "mtvlog_HandleARM read pos == write pos"
#define MTV_LOG_678_112_2_17_23_13_2_3156 "mtvlog_HandleARM data_ptr = %x, data_length = %d"
#define MTV_LOG_685_112_2_17_23_13_2_3157 "mtvlog_HandleARM start mtvlog_ReadLcmData"
#define MTV_LOG_692_112_2_17_23_13_2_3158 "mtvlog_HandleARM 1st buf adr = %x, len = %d, src adr = %x, 2nd buf adr = %x 2nd len = %d, src adr = %x"
#define MTV_LOG_699_112_2_17_23_13_2_3159 "mtvlog_HandleARM: LCM read time = %d length = %d"
#define MTV_LOG_722_112_2_17_23_13_2_3160 "mtvlog_HandleDsp"
#define MTV_LOG_726_112_2_17_23_13_2_3161 "mtvlog_HandleDsp s_read_pos = %x, org_write_pos = %x "
#define MTV_LOG_729_112_2_17_23_13_2_3162 "mtvlog_HandleDsp write_pos = %x "
#define MTV_LOG_742_112_2_17_23_13_2_3163 "mtvlog_HandleDsp read pos == write pos"
#define MTV_LOG_746_112_2_17_23_13_2_3164 "mtvlog_HandleDsp data_length = %d"
#define MTV_LOG_765_112_2_17_23_13_3_3165 "mtvlog_HandleDsp: LCM read time = %d length = %d speed = %d Kb/s"
#define MTV_LOG_840_112_2_17_23_13_3_3166 "[MTVLOG]mtvlog_CloseLogFile arm=%x"
#define MTV_LOG_874_112_2_17_23_13_3_3167 "[MTVLOG]mvtlog_ThreadEntry exit"
#define MTV_LOG_881_112_2_17_23_13_3_3168 "[MTVLOG]mvtlog_ThreadEntry Unknow signal id!"
#define MTV_LOG_899_112_2_17_23_13_3_3169 "[MTVLOG]mvtlog_ThreadEntry thread exit"
#define MTV_LOG_911_112_2_17_23_13_3_3170 "[MTVLOG]MTVLOG_Init arm log = %d"
#define MTV_LOG_919_112_2_17_23_13_3_3171 "[MTVLOG]MTVLOG_Init create file fail!"
#define MTV_LOG_934_112_2_17_23_13_3_3172 "[MTVLOG]MTVLOG_Init create task=%d"
#define MTV_LOG_950_112_2_17_23_13_3_3173 "[MTVLOG]MTVLOG_Init done!"
#define MTV_LOG_959_112_2_17_23_13_3_3174 "[MTVLOG]MTVLOG_Exit !\n"
#define MTV_LOG_999_112_2_17_23_13_3_3175 "MOBILETVLOG_SetEnableMobileTvArmLogFlag is_enable_arm_log %d\n"
#define MTV_LOG_1023_112_2_17_23_13_3_3176 "MOBILETVLOG_SetEnableMobileTvDspLogFlag is_enable_dsp_log %d\n"
#define MTV_LOG_1059_112_2_17_23_13_3_3177 "test_DSPTrace  status = %d"
#define MTV_LOG_1096_112_2_17_23_13_3_3178 "into test_func s_dsp_total_len = %d"
#define MTV_SERVICE_415_112_2_17_23_13_4_3179 "[MBBMS Service] MTVSERVICE_MBMSInit Entering... \n"
#define MTV_SERVICE_445_112_2_17_23_13_4_3180 "[mbbms service]MTVSERVICE_Init %d\n "
#define MTV_SERVICE_479_112_2_17_23_13_4_3181 "[MBBMS Service] Thread Created Ok \n"
#define MTV_SERVICE_484_112_2_17_23_13_4_3182 "[MBBMS Service] MTVSERVICE_Init  no mem\n "
#define MTV_SERVICE_493_112_2_17_23_13_4_3183 "mbbms MTVSERVICE_Init CMMB init FAILED \n"
#define MTV_SERVICE_513_112_2_17_23_13_4_3184 "[MBBMS Service] MTVSERVICE_MBMSInit CMMB MODE Over \n"
#define MTV_SERVICE_523_112_2_17_23_13_4_3185 "[MBBMS Service] MTVSERVICE_MBMSInit MBBMS Init FAILED \n"
#define MTV_SERVICE_535_112_2_17_23_13_4_3186 "[MBBMS Service] MTVSERVICE_Init  OK Leaving... \n"
#define MTV_SERVICE_546_112_2_17_23_13_4_3187 "[mbbms service]MTVSERVICE_MBMSInit entering... \n"
#define MTV_SERVICE_560_112_2_17_23_13_4_3188 "[MBBMS Service] MTVSERVICE_MBMSInit MBBMS_GBA_Init Failed... \n"
#define MTV_SERVICE_567_112_2_17_23_13_4_3189 "[MBBMS Service] GBA Inited Ok \n"
#define MTV_SERVICE_576_112_2_17_23_13_4_3190 "[MBBMS Service] MTVSERVICE_MBMSInit MBBMS_Sub_MSK_Init Failed... \n"
#define MTV_SERVICE_580_112_2_17_23_13_4_3191 "[MBBMS Service] Sub Msk Inited Ok \n"
#define MTV_SERVICE_582_112_2_17_23_13_4_3192 "[MBBMS Service] SG Inited Ok \n"
#define MTV_SERVICE_607_112_2_17_23_13_5_3193 "[MBBMS Service] MTVSERVICE_MBMSInit fail to  MBBMS_Load_UpdateTable\n"
#define MTV_SERVICE_610_112_2_17_23_13_5_3194 "[MBBMS Service] MTVSERVICE_MBMSInit MBBMS MODE OVER Leaving... \n"
#define MTV_SERVICE_614_112_2_17_23_13_5_3195 "[MBBMS Service] MTVSERVICE_MBMSInit MBBMS Mode Failed... \n"
#define MTV_SERVICE_627_112_2_17_23_13_5_3196 "[mbbms service]MTVSERVICE_ChangeMode %d entering \n"
#define MTV_SERVICE_644_112_2_17_23_13_5_3197 "[mbbms service]MTVSERVICE_ChangeMode MBMS init FAILED\n"
#define MTV_SERVICE_658_112_2_17_23_13_5_3198 "[mbbms service]MTVSERVICE_ChangeMode over \n"
#define MTV_SERVICE_665_112_2_17_23_13_5_3199 "[mbbms service]MTVSERVICE_MBMSExit Entering ...\n"
#define MTV_SERVICE_670_112_2_17_23_13_5_3200 "MTVSERVICE_MBMSExit MTVSERVICE_DeactivePDP failed !! \n"
#define MTV_SERVICE_681_112_2_17_23_13_5_3201 "[mbbms service]MTVSERVICE_MBMSExit: sub type:%d play type:%d gba type:%d sgtype:%dn"
#define MTV_SERVICE_682_112_2_17_23_13_5_3202 "[mbbms service]MTVSERVICE_MBMSExit: some task is running; exit FAIL \n"
#define MTV_SERVICE_696_112_2_17_23_13_5_3203 "[mbbms service]MTVSERVICE_MBMSExit starting ...\n"
#define MTV_SERVICE_701_112_2_17_23_13_5_3204 "[mbbms service]MTVSERVICE_MBMSExit: gba queue is not EMPTY \n"
#define MTV_SERVICE_706_112_2_17_23_13_5_3205 "[mbbms service]MTVSERVICE_MBMSExit: gba queue type:%d sub:%d running:%d n"
#define MTV_SERVICE_716_112_2_17_23_13_5_3206 "[mbbms service]MTVSERVICE_MBMSExit: sub queue is not EMPTY \n"
#define MTV_SERVICE_721_112_2_17_23_13_5_3207 "[mbbms service]MTVSERVICE_MBMSExit: sub queue type:%d sub:%d running:%d n"
#define MTV_SERVICE_732_112_2_17_23_13_5_3208 "[mbbms service]MTVSERVICE_MBMSExit: pdp queue is not EMPTY \n"
#define MTV_SERVICE_737_112_2_17_23_13_5_3209 "[mbbms service]MTVSERVICE_MBMSExit: pdp queue type:%d sub:%d running:%d n"
#define MTV_SERVICE_762_112_2_17_23_13_5_3210 "[mbbms service]MTVSERVICE_MBMSExit leaving ...\n"
#define MTV_SERVICE_790_112_2_17_23_13_5_3211 "[mbbms service ]MTVSERVICE_ServiceRequest starting... \n"
#define MTV_SERVICE_807_112_2_17_23_13_5_3212 "[mbbms service ]MTVSERVICE_ServiceRequest Over... \n"
#define MTV_SERVICE_814_112_2_17_23_13_5_3213 "[mbbms service ]MTVSERVICE_MBMSStopServiceRequest starting... \n"
#define MTV_SERVICE_833_112_2_17_23_13_5_3214 "[mbbms service ]MTVSERVICE_MBMSStopServiceRequest over... \n"
#define MTV_SERVICE_839_112_2_17_23_13_5_3215 "[mbbms service ]MTVSERVICE_UnsubRequest starting... \n"
#define MTV_SERVICE_859_112_2_17_23_13_5_3216 "[mbbms service ]MTVSERVICE_UnsubRequest over... \n"
#define MTV_SERVICE_866_112_2_17_23_13_5_3217 "[mbbms service ]MTVSERVICE_MBMSStopUnsubRequest starting... \n"
#define MTV_SERVICE_885_112_2_17_23_13_5_3218 "[mbbms service ]MTVSERVICE_MBMSStopUnsubRequest over... \n"
#define MTV_SERVICE_891_112_2_17_23_13_5_3219 "[mbbms service ]MTVSERVICE_AccountQuiry starting... \n"
#define MTV_SERVICE_910_112_2_17_23_13_5_3220 "[mbbms service ]MTVSERVICE_AccountQuiry over... \n"
#define MTV_SERVICE_916_112_2_17_23_13_5_3221 "[mbbms service ]MTVSERVICE_MBMSStopAccountQuiry starting... \n"
#define MTV_SERVICE_935_112_2_17_23_13_5_3222 "[mbbms service ]MTVSERVICE_MBMSStopAccountQuiry over... \n"
#define MTV_SERVICE_941_112_2_17_23_13_5_3223 "[mbbms service ]MTVSERVICE_SubUpdate starting... \n"
#define MTV_SERVICE_943_112_2_17_23_13_5_3224 "[mbbms service ]MTVSERVICE_SubUpdate CHECKing INIT \n"
#define MTV_SERVICE_948_112_2_17_23_13_5_3225 "[mbbms service ]MTVSERVICE_SubUpdate CHECKing IDLE \n"
#define MTV_SERVICE_951_112_2_17_23_13_5_3226 "[mbbms service ]MTVSERVICE_SubUpdate GET MUTEX \n"
#define MTV_SERVICE_965_112_2_17_23_13_5_3227 "[mbbms service ]MTVSERVICE_SubUpdate over... \n"
#define MTV_SERVICE_971_112_2_17_23_13_5_3228 "[mbbms service ]MTVSERVICE_MBMSStopSubUpdate starting... \n"
#define MTV_SERVICE_991_112_2_17_23_13_5_3229 "[mbbms service ]MTVSERVICE_MBMSStopSubUpdate over... \n"
#define MTV_SERVICE_998_112_2_17_23_13_5_3230 "[mbbms service ]MTVSERVICE_StartGBA starting... \n"
#define MTV_SERVICE_1020_112_2_17_23_13_5_3231 "[mbbms service ]MTVSERVICE_StartGBA over... \n"
#define MTV_SERVICE_1026_112_2_17_23_13_5_3232 "[mbbms service ]MTVSERVICE_MBMSStopGBA starting... \n"
#define MTV_SERVICE_1045_112_2_17_23_13_5_3233 "[mbbms service ]MTVSERVICE_MBMSStopGBA over... \n"
#define MTV_SERVICE_1065_112_2_17_23_13_5_3234 "mbbms mbms_service_GBA_callback entering..%d %d..\n"
#define MTV_SERVICE_1083_112_2_17_23_13_5_3235 "mbbms mbms_service_GBA_callback send stop for type%d \n"
#define MTV_SERVICE_1124_112_2_17_23_13_6_3236 "[mbbms service]mbms_service_gba_callback stop and restart gba FAIL \n"
#define MTV_SERVICE_1132_112_2_17_23_13_6_3237 "[mbbms service]mbms_service_gba_callback stop and restart gba OK \n"
#define MTV_SERVICE_1162_112_2_17_23_13_6_3238 "mbbms mbms_service_GBA_callback Free ptr for GBA removed \n"
#define MTV_SERVICE_1179_112_2_17_23_13_6_3239 "[mbbms service]mbms_service_gba_callback send signal type:%d result:%d n"
#define MTV_SERVICE_1200_112_2_17_23_13_6_3240 "[mbbms service]mbms_service_gba_callback set next and free current gba \n"
#define MTV_SERVICE_1215_112_2_17_23_13_6_3241 "mbbms mbms_service_GBA_callback END..%d %d..\n"
#define MTV_SERVICE_1232_112_2_17_23_13_6_3242 "mbbms mbms_inner_sub_callback_send entering subtype:%d result:%d... \n"
#define MTV_SERVICE_1236_112_2_17_23_13_6_3243 "mbbms mbms_inner_sub_callback_send sub register queue is EMPTY!! \n"
#define MTV_SERVICE_1242_112_2_17_23_13_6_3244 "mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_ACCOUNT_QUIRY not found-freeptr \n"
#define MTV_SERVICE_1247_112_2_17_23_13_6_3245 "mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_SUBSCRIBE not found-freeptr \n"
#define MTV_SERVICE_1252_112_2_17_23_13_6_3246 "mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_UPDATE_TABLE not found-freeptr \n"
#define MTV_SERVICE_1256_112_2_17_23_13_6_3247 "mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_UNSUBSCRIBE not found-freeptr \n"
#define MTV_SERVICE_1260_112_2_17_23_13_6_3248 "mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_GETMSK not found-freeptr \n"
#define MTV_SERVICE_1290_112_2_17_23_13_6_3249 "[mbbms service]mbms_inner_sub_callback_send FREE type:%d subtype:%dn"
#define MTV_SERVICE_1300_112_2_17_23_13_6_3250 "[mbbms service]mbms_inner_sub_callback_send find type:%d subtype:%dn"
#define MTV_SERVICE_1310_112_2_17_23_13_6_3251 "mbbms mbms_inner_sub_callback_send OK.  NO more task in QUEUE... \n"
#define MTV_SERVICE_1313_112_2_17_23_13_6_3252 "mbbms mbms_inner_sub_callback_send. More Taks is waiting in QUEUE\n"
#define MTV_SERVICE_1321_112_2_17_23_13_6_3253 "[mbbms service]mbms_inner_sub_callback_send TRY run type:%d subtype:%dn"
#define MTV_SERVICE_1328_112_2_17_23_13_6_3254 "mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_ACCOUNT_QUIRY STARTING \n"
#define MTV_SERVICE_1334_112_2_17_23_13_6_3255 "mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_SUBSCRIBE STARTING \n"
#define MTV_SERVICE_1340_112_2_17_23_13_6_3256 "mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_UPDATE_TABLE STARTING \n"
#define MTV_SERVICE_1345_112_2_17_23_13_6_3257 "mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_UNSUBSCRIBE STARTING \n"
#define MTV_SERVICE_1350_112_2_17_23_13_6_3258 "mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_GETMSK STARTING \n"
#define MTV_SERVICE_1361_112_2_17_23_13_6_3259 "[MBBMS Service] mbms_inner_sub_callback_send run OK type:%d subtype:%d \n"
#define MTV_SERVICE_1368_112_2_17_23_13_6_3260 "[MBBMS Service] mbms_inner_sub_callback_send start fail and remove type:%d subtype:%d \n"
#define MTV_SERVICE_1388_112_2_17_23_13_6_3261 "mbbms mbms_service_sub_callback entering..%d %d..\n"
#define MTV_SERVICE_1579_112_2_17_23_13_7_3262 "[MBBMS Service] mbms_service_pdp_callback return NULL!!! \n"
#define MTV_SERVICE_1582_112_2_17_23_13_7_3263 "[MBBMS Service] mbms_service_pdp_callback running.... \n"
#define MTV_SERVICE_1587_112_2_17_23_13_7_3264 "mbms_service_pdp_callback:Get MBMSPDP_ACTIVE_CNF msg  MMIMBBMS_TickCount=%d"
#define MTV_SERVICE_1597_112_2_17_23_13_7_3265 "[MBBMS Service] mbms_service_pdp_callback Ok type:%d\n"
#define MTV_SERVICE_1614_112_2_17_23_13_7_3266 "[MBBMS Service] mbms_service_pdp_callback FAIL type:%d\n"
#define MTV_SERVICE_1625_112_2_17_23_13_7_3267 "mbms_service_pdp_callback:Get MBMSPDP_ACTIVE_CNF msg ERROR!\n"
#define MTV_SERVICE_1631_112_2_17_23_13_7_3268 "mbms_service_pdp_callback:Get MBMSPDP_DEACTIVE_CNF Msg  MMIMBBMS_TickCount=%d"
#define MTV_SERVICE_1639_112_2_17_23_13_7_3269 "[MBBMS Service] mbms_service_pdp_callback DEACTIVE FAIL type:%d\n"
#define MTV_SERVICE_1651_112_2_17_23_13_7_3270 "mbms_service_pdp_callback:Get MBMSPDP_DEACTIVE_IND Msg  MMIMBBMS_TickCount=%d"
#define MTV_SERVICE_1675_112_2_17_23_13_7_3271 "[MBBMS Service] mtvservice_register_gba alloc FAILED \n"
#define MTV_SERVICE_1685_112_2_17_23_13_7_3272 "[MBBMS Service] mtvservice_register_gba First GBA Task \n"
#define MTV_SERVICE_1698_112_2_17_23_13_7_3273 "[MBBMS Service] mtvservice_register_gba GBA return FAIL \n"
#define MTV_SERVICE_1706_112_2_17_23_13_7_3274 "[MBBMS Service] mtvservice_register_gba first GBA BUT Busy!!! \n"
#define MTV_SERVICE_1714_112_2_17_23_13_7_3275 "[MBBMS Service] mtvservice_register_gba add to queue \n"
#define MTV_SERVICE_1724_112_2_17_23_13_7_3276 "[MBBMS Service] mtvservice_register_gba OK type:%d \n"
#define MTV_SERVICE_1744_112_2_17_23_13_7_3277 "[MBBMS Service] mtvservice_register_sub alloc FAILED \n"
#define MTV_SERVICE_1757_112_2_17_23_13_7_3278 "[MBBMS Service]mtvservice_register_sub create queue type%d subtype:%dn"
#define MTV_SERVICE_1764_112_2_17_23_13_7_3279 "[MBBMS Service]mtvservice_register_sub add queue type%d subtype:%dn"
#define MTV_SERVICE_1805_112_2_17_23_13_7_3280 "[MBBMS Service] mtvservice_register_sub run OK type:%d subtype:%d \n"
#define MTV_SERVICE_1816_112_2_17_23_13_7_3281 "[MBBMS Service] mtvservice_register_sub fail and remove type:%d subtype:%d \n"
#define MTV_SERVICE_1824_112_2_17_23_13_7_3282 "[MBBMS Service] mtvservice_register_sub add ok type:%d subtype:%d \n"
#define MTV_SERVICE_1832_112_2_17_23_13_7_3283 "[MBBMS Service] mtvservice_register_sub add ok type:%d subtype:%d \n"
#define MTV_SERVICE_1871_112_2_17_23_13_7_3284 "mbbms mtvservice_unregister_gba entering.... \n"
#define MTV_SERVICE_1884_112_2_17_23_13_7_3285 "[MBBMS Service] mtvservice_unregister_gba ONE OK type:%d \n"
#define MTV_SERVICE_1891_112_2_17_23_13_7_3286 "[MBBMS Service] mtvservice_unregister_gba FREEING... \n"
#define MTV_SERVICE_1893_112_2_17_23_13_7_3287 "[MBBMS Service] mtvservice_unregister_gba FREE OK \n"
#define MTV_SERVICE_1906_112_2_17_23_13_7_3288 "[MBBMS Service] mtvservice_unregister_gba MORE OK type:%d \n"
#define MTV_SERVICE_1911_112_2_17_23_13_7_3289 "[MBBMS Service] mtvservice_unregister_gba MORE FREEING... \n"
#define MTV_SERVICE_1913_112_2_17_23_13_7_3290 "[MBBMS Service] mtvservice_unregister_gba MORE FREE OK \n"
#define MTV_SERVICE_1925_112_2_17_23_13_7_3291 "[MBBMS Service] mtvservice_unregister_gba Stopping .... \n"
#define MTV_SERVICE_1933_112_2_17_23_13_7_3292 "mbbms mtvservice_unregister_gba Over  .... \n"
#define MTV_SERVICE_1942_112_2_17_23_13_7_3293 "mbbms mtvservice_unregister_sub entering .... \n"
#define MTV_SERVICE_1958_112_2_17_23_13_7_3294 "[mbbms service] mtvservice_unregister_sub find type:%d subtype:%dn"
#define MTV_SERVICE_1986_112_2_17_23_13_8_3295 "mbbms mtvservice_unregister_sub Over %d .... \n"
#define MTV_SERVICE_2009_112_2_17_23_13_8_3296 "[mbbms service]mbms_service_deactivepdp_callback received \n"
#define MTV_SERVICE_2010_112_2_17_23_13_8_3297 "[mbbms service] result:%d msg id:%d \n"
#define MTV_SERVICE_2031_112_2_17_23_13_8_3298 "[MBBMS Service] MTVSERVICE_DeactivePDP send MMI Ok \n"
#define MTV_SERVICE_2080_112_2_17_23_13_8_3299 "[MBBMS Service] MTVSERVICE_AddPDP send MMI Ok type:%d\n"
#define MTV_SERVICE_2100_112_2_17_23_13_8_3300 "[MBBMS Service] MTVSERVICE_RemovePDP Ok type:%d\n"
#define MTV_SERVICE_2137_112_2_17_23_13_8_3301 "[mbms] MBBMS_Service_Thread: task_id 0x%x ,signalcode 0x%x ,sig->type %d\n "
#define MTV_SERVICE_2145_112_2_17_23_13_8_3302 "[mbbms service]MBBMS_Service_Thread Exit Begin \n"
#define MTV_SERVICE_2216_112_2_17_23_13_8_3303 "[mbbms service]MBBMS_Service_Thread Exit Over \n"
#define MTV_SERVICE_2272_112_2_17_23_13_8_3304 "[mbbms service ] SetDownloadPicState state %d n"
#define MTV_SERVICE_2287_112_2_17_23_13_8_3305 "[mbbms service ] GetDownloadPicState state %d n"
#define MTV_SERVICE_2301_112_2_17_23_13_8_3306 "[mbbms service ] SetTabState state %d n"
#define MTV_SERVICE_2316_112_2_17_23_13_8_3307 "[mbbms service ] GetTabState state %d n"
#define MTV_SERVICE_2330_112_2_17_23_13_8_3308 "[mbbms service ] SetGBAState state %d n"
#define MTV_SERVICE_2345_112_2_17_23_13_8_3309 "[mbbms service ] GetGBAState state %d n"
#define MTV_SERVICE_2359_112_2_17_23_13_8_3310 "[mbbms service ] StopDownloadPicture type %d n"
#define MTV_SERVICE_2383_112_2_17_23_13_8_3311 "[mbbms service ] HandleRcvSig mode %d ,type %d ,subtype %d \n"
#define MTV_SERVICE_2432_112_2_17_23_13_8_3312 "[mbbms service ] HandleRcvSig err mode %d \n"
#define MTV_SERVICE_2445_112_2_17_23_13_8_3313 "[mbbms service ] IsFinishProcess type %d\n"
#define MTV_SERVICE_2479_112_2_17_23_13_8_3314 "[mbbms service ] FinishProcess type %d, rt %d \n"
#define MTV_SERVICE_2500_112_2_17_23_13_9_3315 "[mbbms service ] HandleRcvStopSig type %d ,subtype %d,mode %d \n"
#define MTV_SERVICE_2557_112_2_17_23_13_9_3316 "[mbbms service ] GetNAFAddr PNULL==naf_addr ||PNULL == naf_addr_len ||PNULL ==port "
#define MTV_SERVICE_2565_112_2_17_23_13_9_3317 "[mbbms service ] GetNAFAddr error_code =%d, service_num=%d\n"
#define MTV_SERVICE_2579_112_2_17_23_13_9_3318 "GetNAFAddr PNULL ==service_index_ptr \n"
#define MTV_SERVICE_2591_112_2_17_23_13_9_3319 "[mbbms service ] GetNAFAddr get access\n"
#define MTV_SERVICE_2611_112_2_17_23_13_9_3320 "[mbbms service ] GetNAFAddr temp =%s\n"
#define MTV_SERVICE_2633_112_2_17_23_13_9_3321 "GetNAFAddr doesn't find"
#define MTV_SERVICE_2648_112_2_17_23_13_9_3322 "GetNAFAddr naf addr =%s\n"
#define MTV_SERVICE_2707_112_2_17_23_13_9_3323 "[mbbms service ] UpdateTab type %d ,subtype %d \n"
#define MTV_SERVICE_2779_112_2_17_23_13_9_3324 "[mbbms service ] HandleDownloadPictureSig \n"
#define MTV_SERVICE_2812_112_2_17_23_13_9_3325 "[mbbms service ] GetSGSearchResult type %d \n"
#define MTV_SERVICE_2851_112_2_17_23_13_9_3326 "[mbbms service ] GetSGSearchResult rt %d \n"
#define MTV_SERVICE_2950_112_2_17_23_13_9_3327 "[mbbms service ] GetMBBMSSGResult type %d \n"
#define MTV_SERVICE_2985_112_2_17_23_13_10_3328 "[mbbms service ] HandleSGParseSig err_code %d \n"
#define MTV_SERVICE_3027_112_2_17_23_13_10_3329 "[mbbms service ] HandleSGParseSig err_code %d \n"
#define MTV_SERVICE_3053_112_2_17_23_13_10_3330 "[mbbms service ] HandleSGParseSig type %d \n"
#define MTV_SERVICE_3073_112_2_17_23_13_10_3331 "[mbbms service ] HandleSGRcvSig info.type %d \n"
#define MTV_SERVICE_3148_112_2_17_23_13_10_3332 "[mbbms service ] HandleUpdateTabSig result %d \n"
#define MTV_SERVICE_3156_112_2_17_23_13_10_3333 "[mbbms service ] HandleUpdateTabSig type %d ,tab_state %d\n"
#define MTV_SERVICE_3173_112_2_17_23_13_10_3334 "[mbbms service ] HandleUpdateTabSig MTVSERVICE_RemoveRegister  rt %d\n"
#define MTV_SERVICE_3197_112_2_17_23_13_10_3335 "[mbbms service ] HandleUpdateTabSig MTVSERVICE_AddRegister gba  \n"
#define MTV_SERVICE_3207_112_2_17_23_13_10_3336 "[mbbms service ] HandleUpdateTabSig MTVSERVICE_AddRegister gba  err\n"
#define MTV_SERVICE_3285_112_2_17_23_13_10_3337 "[mbbms service ] HandleUpdateTabSig state type  %d\n"
#define MTV_SERVICE_3303_112_2_17_23_13_10_3338 "[mbbms service ] HandleGBASig result %d \n"
#define MTV_SERVICE_3325_112_2_17_23_13_10_3339 "[mbbms service ] HandleGBASig MTVSERVICE_RemoveRegister gba  err\n"
#define MTV_SERVICE_3372_112_2_17_23_13_10_3340 "[mbbms service ] HandleGBASig type %d \n"
#define MTV_SERVICE_3434_112_2_17_23_13_10_3341 "[mbbms service ]HandleStopSGRcvSig type %d,subtype %d\n"
#define MTV_SERVICE_3676_112_2_17_23_13_11_3342 "[mbbms service ] SetSGIfState type %d ,subtype %dn"
#define MTV_SERVICE_3693_112_2_17_23_13_11_3343 "[mbbms service ] SetSGIfState finish "
#define MTV_SERVICE_3709_112_2_17_23_13_11_3344 "[mbbms service ] GetSGIfState act_type %d n"
#define MTV_SERVICE_3734_112_2_17_23_13_11_3345 "[mbbms service ]MTVSERVICE_ActiveMBBMS starting\n"
#define MTV_SERVICE_3745_112_2_17_23_13_11_3346 "[mbbms service ]MTVSERVICE_ActiveMBBMS para err !\n"
#define MTV_SERVICE_3757_112_2_17_23_13_11_3347 "[mbbms service ]MTVSERVICE_ActiveMBBMS wrong state %d \n"
#define MTV_SERVICE_3766_112_2_17_23_13_11_3348 "[mbbms service ]MTVSERVICE_ActiveMBBMS fail to active pdp \n"
#define MTV_SERVICE_3773_112_2_17_23_13_11_3349 "[mbbms service ]MTVSERVICE_ActiveMBBMS over \n"
#define MTV_SERVICE_3792_112_2_17_23_13_11_3350 "[mbbms service ]MTVSERVICE_DeactiveMBBMS  \n"
#define MTV_SERVICE_3803_112_2_17_23_13_11_3351 "[mbbms service ]MTVSERVICE_DeactiveMBBMS  type %d, subtype %d\n"
#define MTV_SERVICE_3857_112_2_17_23_13_11_3352 "[mbbms service ]MTVSERVICE_DeactiveMBBMS rt %d\n"
#define MTV_SERVICE_3886_112_2_17_23_13_11_3353 "[mbbms service ]MTVSERVICE_ParseServiceGuide \n"
#define MTV_SERVICE_3930_112_2_17_23_13_11_3354 "[mbbms service ]MTVSERVICE_ParseServiceGuide parse error \n"
#define MTV_SERVICE_3961_112_2_17_23_13_11_3355 "[mbbms service ]MTVSERVICE_StopParseServiceGuide start \n"
#define MTV_SERVICE_4004_112_2_17_23_13_12_3356 "[mbbms service ]IsServiceSubscribed get purchase result=%d\n"
#define MTV_SERVICE_4010_112_2_17_23_13_12_3357 "[mbbms service ]IsServiceSubscribed purchase_itemlist_head_ptr=0x%x\n"
#define MTV_SERVICE_4026_112_2_17_23_13_12_3358 "[mbbms service ]IsServiceSubscribed:%d"
#define MTV_SERVICE_4037_112_2_17_23_13_12_3359 "[mbbms service ] IsServiceSubscribed:Get Purchase Item by sevice failded\n"
#define MTV_SERVICE_4079_112_2_17_23_13_12_3360 "[SG]: Copy2ContentString, fail to alloc memory "
#define MTV_SERVICE_4113_112_2_17_23_13_12_3361 "[SG]: Copy2ContentString, fail to alloc memory "
#define MTV_SERVICE_4175_112_2_17_23_13_12_3362 "[mbbms service ]CopyDataFromlist  type=%d\n"
#define MTV_SERVICE_4298_112_2_17_23_13_12_3363 "[mbbms service ]OrderByWeight  is_increase =%d,total_count =%d\n"
#define MTV_SERVICE_4345_112_2_17_23_13_12_3364 "[mbbms service ]HandlePdpSig sig_ptr->subtype %d\n"
#define MTV_SERVICE_4353_112_2_17_23_13_12_3365 "[mbbms service ]HandlePdpSig   type %d\n"
#define MTV_SERVICE_4428_112_2_17_23_13_12_3366 "[mbbms service ]SetCMMBService  net_id =%d,serviceid =%d\n"
#define MTV_SERVICE_4492_112_2_17_23_13_12_3367 "[mbbms service ]SetServiceList  net_id =%d\n"
#define MTV_SERVICE_4516_112_2_17_23_13_13_3368 "[mbbms service ]SetServiceList  total_count =%d\n"
#define MTV_SERVICE_4530_112_2_17_23_13_13_3369 "[mbbms service ]GetCMMBProgListFromNetInfo  network_number =%d\n"
#define MTV_SERVICE_4534_112_2_17_23_13_13_3370 "[mbbms service ]GetCMMBProgListFromNetInfo  network_number = 0 or list_ptr== PNULL\n"
#define MTV_SERVICE_4676_112_2_17_23_13_13_3371 "[mbbms service ]GetCMMBProgFromFile  \n"
#define MTV_SERVICE_4698_112_2_17_23_13_13_3372 "[mbbms service ]GetCMMBProgFromFile fail to create file \n"
#define MTV_SERVICE_4705_112_2_17_23_13_13_3373 "[mbbms service ]GetCMMBProgFromFile fail to read  file\n"
#define MTV_SERVICE_4714_112_2_17_23_13_13_3374 "[mbbms service ]GetCMMBProgFromFile is_valid%d\n"
#define MTV_SERVICE_4723_112_2_17_23_13_13_3375 "[mbbms service ]GetCMMBProgFromFile network_num %d\n"
#define MTV_SERVICE_4765_112_2_17_23_13_13_3376 "[mbbms service ]GetCMMBProgramFromFile  \n"
#define MTV_SERVICE_4809_112_2_17_23_13_13_3377 "[mbbms service ] SetControlData  \n"
#define MTV_SERVICE_4845_112_2_17_23_13_13_3378 "[mbbms service ]SetControlData is_valid%d\n"
#define MTV_SERVICE_4854_112_2_17_23_13_13_3379 "[mbbms service ]SetControlData network_num %d\n"
#define MTV_SERVICE_4902_112_2_17_23_13_13_3380 "[mbbms service ]GetCMMBControlDataFromFile fail to SetControlTabData \n"
#define MTV_SERVICE_4949_112_2_17_23_13_13_3381 "[mbbms service ] SaveControlDataForTest:name_len %d\n"
#define MTV_SERVICE_4961_112_2_17_23_13_13_3382 "[mbbms service ] SaveControlDataForTest: fail to create file to read \n"
#define MTV_SERVICE_4983_112_2_17_23_13_13_3383 "[mbbms service ] SaveControlDataForTest: fail to create dir \n"
#define MTV_SERVICE_5006_112_2_17_23_13_13_3384 "[mbbms service ] SaveControlDataForTest: fail to create file to write \n"
#define MTV_SERVICE_5020_112_2_17_23_13_13_3385 "[mbbms service ] SaveControlDataForTest: fail to alloc mem \n"
#define MTV_SERVICE_5036_112_2_17_23_13_14_3386 "[mbbms service ] SaveControlDataForTest: read err \n"
#define MTV_SERVICE_5048_112_2_17_23_13_14_3387 "[mbbms service ] SaveControlDataForTest: write err \n"
#define MTV_SERVICE_5086_112_2_17_23_13_14_3388 "[mbbms service ]SaveControlData    \n"
#define MTV_SERVICE_5094_112_2_17_23_13_14_3389 "[mbbms service]: SaveControlData network_number = %d."
#define MTV_SERVICE_5138_112_2_17_23_13_14_3390 "[mbbms service]: SaveControlData write hader err\n"
#define MTV_SERVICE_5156_112_2_17_23_13_14_3391 "[mbbms service]: SaveControlData write data err\n"
#define MTV_SERVICE_5169_112_2_17_23_13_14_3392 "[SG]: SaveProgramList set pointer  error"
#define MTV_SERVICE_5184_112_2_17_23_13_14_3393 "[mbbms service]: SaveProgramList write ctl data info  error"
#define MTV_SERVICE_5198_112_2_17_23_13_14_3394 "[mbbms service]: SaveProgramList set pointer  error"
#define MTV_SERVICE_5211_112_2_17_23_13_14_3395 "[mbbms service]: SaveProgramList write header  error"
#define MTV_SERVICE_5255_112_2_17_23_13_14_3396 "[mbbms service ]SaveProgramList network_number %d  \n"
#define MTV_SERVICE_5303_112_2_17_23_13_14_3397 "[SG]: SaveProgramList set pointer  error"
#define MTV_SERVICE_5342_112_2_17_23_13_14_3398 "[mbbms service ]PrintProgramList count %d  \n"
#define MTV_SERVICE_5347_112_2_17_23_13_14_3399 "[mbbms service ]PrintProgramList program[%d]  \n"
#define MTV_SERVICE_5348_112_2_17_23_13_14_3400 "[mbbms service ] service name len %d\n"
#define MTV_SERVICE_5349_112_2_17_23_13_14_3401 "[mbbms service ] service char_set %d\n"
#define MTV_SERVICE_5350_112_2_17_23_13_14_3402 "[mbbms service ] service net_id %d\n"
#define MTV_SERVICE_5351_112_2_17_23_13_14_3403 "[mbbms service ] service is_forfree %d\n"
#define MTV_SERVICE_5352_112_2_17_23_13_14_3404 "[mbbms service ] service id %d\n"
#define MTV_SERVICE_5353_112_2_17_23_13_14_3405 "[mbbms service ] is_subscribed %d\n"
#define MTV_SERVICE_5354_112_2_17_23_13_14_3406 "[mbbms service ] is_encryped %d\n"
#define MTV_SERVICE_5355_112_2_17_23_13_14_3407 "[mbbms service ]servicetype_count%d\n"
#define MTV_SERVICE_5356_112_2_17_23_13_14_3408 "[mbbms service ]genretype_count%d\n"
#define MTV_SERVICE_5357_112_2_17_23_13_14_3409 "[mbbms service ]weight%d\n"
#define MTV_SERVICE_5358_112_2_17_23_13_14_3410 "[mbbms service ]curcontentname %d\n"
#define MTV_SERVICE_5386_112_2_17_23_13_14_3411 "[mbbms service ]GetCMMBProgramList list_ptr == PNULL"
#define MTV_SERVICE_5389_112_2_17_23_13_14_3412 "[mbbms service ]GetCMMBProgramList is_updated %d  \n"
#define MTV_SERVICE_5451_112_2_17_23_13_14_3413 "[mbbms service ]GetMBBMSProgramList is_updated %d ,language_type %d \n"
#define MTV_SERVICE_5494_112_2_17_23_13_14_3414 "[SL]  MTVSERVICE_GetProgramList serviceid%s  can't get name"
#define MTV_SERVICE_5503_112_2_17_23_13_14_3415 "[mbbms service ]  MTVSERVICE_GetProgramList serviceid=%s  can't get subcribed "
#define MTV_SERVICE_5508_112_2_17_23_13_14_3416 "[mbbms service ]  MTVSERVICE_GetProgramList serviceid=%s  can't get encryption "
#define MTV_SERVICE_5516_112_2_17_23_13_14_3417 "[mbbms service ]  MTVSERVICE_GetProgramList serviceid=%s  can't get SERVICE_GENRE"
#define MTV_SERVICE_5521_112_2_17_23_13_14_3418 "[mbbms service ]  MTVSERVICE_GetProgramList serviceid=%s can't get  SERVICE_TYPE "
#define MTV_SERVICE_5599_112_2_17_23_13_15_3419 "[mbbms service ] MTVSERVICE_GetProgramList is_updated %d ,language_type %d \n"
#define MTV_SERVICE_5682_112_2_17_23_13_15_3420 "[mbbms service ] GetCMMBResult operate_status %d \n"
#define MTV_SERVICE_5744_112_2_17_23_13_15_3421 "[mbbms service ] SetPrivatePtr ptr 0x%x  \n"
#define MTV_SERVICE_5762_112_2_17_23_13_15_3422 "[mbbms service ] WstrToStr   \n"
#define MTV_SERVICE_5802_112_2_17_23_13_15_3423 "[mbbms service ] SetESGPath esg_len %d \n"
#define MTV_SERVICE_5808_112_2_17_23_13_15_3424 "[mbbms service ] SetESGPath  string is not usc2\n"
#define MTV_SERVICE_5816_112_2_17_23_13_15_3425 "[mbbms service ] SetESGPath  fail to WstrToStr\n"
#define MTV_SERVICE_5824_112_2_17_23_13_15_3426 "[mbbms service ] SetESGPath data path %d\n"
#define MTV_SERVICE_5830_112_2_17_23_13_15_3427 "[mbbms service ] SetESGPath  fail to set esg data path\n"
#define MTV_SERVICE_5874_112_2_17_23_13_15_3428 "[mbbms service ] SearchAllEsg \n"
#define MTV_SERVICE_5889_112_2_17_23_13_15_3429 "[mbbms service ] SearchAllEsg fail to alloc mem \n"
#define MTV_SERVICE_5910_112_2_17_23_13_15_3430 "[mbbms service ] SearchAllEsg fail to search esg \n"
#define MTV_SERVICE_5945_112_2_17_23_13_15_3431 "[mbbms service ] CMMB_ManualUpdateSG search_type %d ,para %d ,type %d\n"
#define MTV_SERVICE_6003_112_2_17_23_13_15_3432 "[mbbms service ] CMMB_ManualUpdateSG rt %d \n"
#define MTV_SERVICE_6012_112_2_17_23_13_15_3433 "[mbbms service ] CMMB_ManualUpdateSG type %d ,subtype %d \n"
#define MTV_SERVICE_6036_112_2_17_23_13_15_3434 "[mbbms service ] HandleSearchProgCnf state.private_int %d  \n"
#define MTV_SERVICE_6136_112_2_17_23_13_16_3435 "[mbbms service ] FreeCMMBData state.private_ptr 0x%x  \n"
#define MTV_SERVICE_6174_112_2_17_23_13_16_3436 "[mbbms service ] HandleSearchEsgCnf state.private_int %d  \n"
#define MTV_SERVICE_6193_112_2_17_23_13_16_3437 "[mbbms service ] HandleSearchEsgCnf state.private_ptr  0x%x ,search_index %d ,network_number %d n"
#define MTV_SERVICE_6200_112_2_17_23_13_16_3438 "[mbbms service ] HandleSearchEsgCnf netid %d n"
#define MTV_SERVICE_6341_112_2_17_23_13_16_3439 "[mbbms service ] HandleCMMBSig type %d ,subtype %d \n"
#define MTV_SERVICE_6434_112_2_17_23_13_16_3440 "[mbbms service ] CMMB_StopManualUpdateSG type %d ,subtype %d \n"
#define MTV_SERVICE_6440_112_2_17_23_13_16_3441 "[mbbms service ] CMMB_StopManualUpdateSG type %d ,subtype %d \n"
#define MTV_SERVICE_6485_112_2_17_23_13_16_3442 "[mbbms service ]MBBMSSG_ManualUpdateSG starting\n"
#define MTV_SERVICE_6508_112_2_17_23_13_16_3443 "[mbbms service ]MBBMSSG_ManualUpdateSG over \n"
#define MTV_SERVICE_6527_112_2_17_23_13_16_3444 "[mbbms service ]MBBMSSG_StopManualUpdateSG starting... \n"
#define MTV_SERVICE_6537_112_2_17_23_13_16_3445 "[mbbms service ]  MBBMSSG_StopManualUpdateSG type %d ,subtype %d \n"
#define MTV_SERVICE_6579_112_2_17_23_13_17_3446 "[mbbms service ]MBBMSSG_StopManualUpdateSG rt %d\n"
#define MTV_SERVICE_6604_112_2_17_23_13_17_3447 "[mbbms service ] MTVSERVICE_ManualUpdateSG search_type %d ,para %d \n"
#define MTV_SERVICE_6632_112_2_17_23_13_17_3448 "[mbbms service ]MTVSERVICE_ManualUpdateSG  rt %d\n"
#define MTV_SERVICE_6652_112_2_17_23_13_17_3449 "[mbbms service ]MTVSERVICE_StopManualUpdateSG \n"
#define MTV_SERVICE_6679_112_2_17_23_13_17_3450 "[mbbms service ]MTVSERVICE_StopManualUpdateSG  rt %d\n"
#define MTV_SERVICE_6723_112_2_17_23_13_17_3451 "[mbbms service ]MTVSERVICE_SGSetWap \n"
#define MTV_SERVICE_6744_112_2_17_23_13_17_3452 "[mbbms service ]MTVSERVICE_SetSGURI \n"
#define MTV_SERVICE_6766_112_2_17_23_13_17_3453 "[mbbms service ]MTVSERVICE_SetSGAreaName \n"
#define MTV_SERVICE_6790_112_2_17_23_13_17_3454 "[mbbms service ]GetActType sig_code %d ,stop_sg_process %d\n "
#define MTV_SERVICE_6831_112_2_17_23_13_17_3455 "[mbbms service ]SendSigToMTVService \n "
#define MTV_SERVICE_6839_112_2_17_23_13_17_3456 "[mbbms service ]SendSigToMTVService sig_code %d \n "
#define MTV_SERVICE_6876_112_2_17_23_13_17_3457 "[mbbms service ]SendSigToMTVService fait to get action \n"
#define MTV_SERVICE_6879_112_2_17_23_13_17_3458 "[mbbms service ]SendSigToMTVService act %d\n "
#define MTV_SERVICE_6882_112_2_17_23_13_17_3459 "[mbbms service ]SendSigToMTVService type %d,subtype %d \n"
#define MTV_SERVICE_6901_112_2_17_23_13_17_3460 "[mbbms service ]SendCmmbSigToMTVService ack_id %d,para0 %d \n"
#define MTV_SERVICE_6906_112_2_17_23_13_17_3461 "[mbbms service ]SendCmmbSigToMTVService fail to alloc mem \n"
#define MTV_SERVICE_6919_112_2_17_23_13_17_3462 "[mbbms service ]SendCmmbSigToMTVService type %d,subtype %d \n"
#define MTV_SERVICE_6940_112_2_17_23_13_17_3463 "[mbbms service ]AppCallback type %d,subtype %d rst %dn"
#define MTV_SERVICE_6969_112_2_17_23_13_17_3464 "[mbbms service ]FinishParseESG \n"
#define MTV_SERVICE_6984_112_2_17_23_13_17_3465 "[mbbms service ]MTVSERVICE_ParseESG \n"
#define MTV_SERVICE_7008_112_2_17_23_13_17_3466 "[mbbms service ]MTVSERVICE_ParseESG ret %d \n"
#define MTV_SERVICE_7022_112_2_17_23_13_17_3467 "[mbbms service ]MTVSERVICE_ParseESG rt %d \n"
#define MTV_SERVICE_7042_112_2_17_23_13_17_3468 "[mbbms service ]MTVSERVICE_StopParseESG \n"
#define MTV_SERVICE_7078_112_2_17_23_13_18_3469 "[mbbms service ]MTVSERVICE_StopParseESG rt %d \n"
#define MTV_SERVICE_7097_112_2_17_23_13_18_3470 "[mbbms service] TestPoint type %d\n"
#define MTV_SERVICE_7121_112_2_17_23_13_18_3471 "[mbbms service] TestNextStep  type %d\n"
#define MTV_SERVICE_7143_112_2_17_23_13_18_3472 "[mbbms service] Test_ManualUpdateSG type  %d\n"
#define MTV_SERVICE_7186_112_2_17_23_13_18_3473 "[mbbms service] Test_AllManualUpdateSG   %d\n"
#define MTV_SERVICE_7230_112_2_17_23_13_18_3474 "\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG  test_times %d,g_test_flag %d"
#define MTV_SERVICE_7243_112_2_17_23_13_18_3475 "\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG   test again(1)  \n"
#define MTV_SERVICE_7257_112_2_17_23_13_18_3476 "\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG   test again(2)  \n"
#define MTV_SERVICE_7275_112_2_17_23_13_18_3477 "\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG   test again(3)  \n"
#define MTV_SERVICE_7282_112_2_17_23_13_18_3478 "\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG   test again(4)  \n"
#define MTV_SERVICE_7292_112_2_17_23_13_18_3479 "\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG  finish  \n"
#define MTV_SERVICE_7317_112_2_17_23_13_18_3480 "[mbbms service] Test_StopManualUpdateSG   times %d\n"
#define MTV_SERVICE_7342_112_2_17_23_13_18_3481 "\"\"\"\"\"\"\"\"[mbbms service] Test_ActiveMBBMS\n"
#define MTV_SERVICE_7365_112_2_17_23_13_18_3482 "\"\"\"\"\"\"\"\"[mbbms service] Test_DeactiveMBBMS\n"
#define MTV_SERVICE_7388_112_2_17_23_13_18_3483 "[mbbms service] Test_DeactiveMBBMS   times %d\n"
#define MTV_SERVICE_7432_112_2_17_23_13_18_3484 "[mbbms service] TestThread g_test_count %d, times %d,type %d, mode %d\n"
#define MTV_SERVICE_7513_112_2_17_23_13_18_3485 "\"\"\"\"\"\"\"\"[mbbms service ]  TEST_StopParse\n"
#define MTV_SERVICE_7531_112_2_17_23_13_18_3486 "MBBMS mmi_callback received type:%d  subtype:%d result:%d ptr:%x n"
#define MTV_SERVICE_7594_112_2_17_23_13_19_3487 "\"\"\"\"\"\"\"\"[mbbms service ]  MTVS_Test \n"
#define MTV_SERVICE_SUB_69_112_2_17_23_13_19_3488 "mbbmsservice_handleUnsubscribe received type:%d subtype:%d result:%dn"
#define MTV_SERVICE_SUB_72_112_2_17_23_13_19_3489 "mbmsservice_handleUnsubscribe MBBMS_SERVICE_UNSUBSCRIBE OVER Already!\n"
#define MTV_SERVICE_SUB_84_112_2_17_23_13_19_3490 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Register PDP Fail. \n"
#define MTV_SERVICE_SUB_100_112_2_17_23_13_19_3491 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE starting ... \n"
#define MTV_SERVICE_SUB_105_112_2_17_23_13_19_3492 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE PDP ok, add register failed. \n"
#define MTV_SERVICE_SUB_113_112_2_17_23_13_19_3493 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Send PDP failed. \n"
#define MTV_SERVICE_SUB_122_112_2_17_23_13_19_3494 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE OK. \n"
#define MTV_SERVICE_SUB_131_112_2_17_23_13_19_3495 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE GBA Failed, Starting GBA.... \n"
#define MTV_SERVICE_SUB_139_112_2_17_23_13_19_3496 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Start GBA Failed.. \n"
#define MTV_SERVICE_SUB_148_112_2_17_23_13_19_3497 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE STOP ok recieved. \n"
#define MTV_SERVICE_SUB_156_112_2_17_23_13_19_3498 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Failed result received.. \n"
#define MTV_SERVICE_SUB_163_112_2_17_23_13_19_3499 "[mbbms service]MBBMS_SERVICE_STOP_UNSUBSCRIBE request Received..... \n"
#define MTV_SERVICE_SUB_169_112_2_17_23_13_19_3500 "[mbbms service]MBBMS_SERVICE_STOP_UNSUBSCRIBE remove pdp Fail \n"
#define MTV_SERVICE_SUB_178_112_2_17_23_13_19_3501 "[mbbms service]MTVSERVICE_RemoveRegister remove gba immediately \n"
#define MTV_SERVICE_SUB_194_112_2_17_23_13_19_3502 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Remove Register OK. \n"
#define MTV_SERVICE_SUB_202_112_2_17_23_13_19_3503 "[mbbms service]MTVSERVICE_RemoveRegister MBBMS_SERVICE_UNSUBSCRIBE return FAIL\n"
#define MTV_SERVICE_SUB_218_112_2_17_23_13_19_3504 "[mbbms service]MBBMS_SERVICE_STOP_UNSUBSCRIBE signal: Wrong State, unsub may be done already. \n"
#define MTV_SERVICE_SUB_227_112_2_17_23_13_19_3505 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE GBA OK. \n"
#define MTV_SERVICE_SUB_231_112_2_17_23_13_19_3506 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Start unsubscribe ok. \n"
#define MTV_SERVICE_SUB_237_112_2_17_23_13_19_3507 "[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Start unsubscribe FAIL  END. \n"
#define MTV_SERVICE_SUB_254_112_2_17_23_13_19_3508 "[mbbms service] mbmsservice_handleUnsubscribe UNKOWN subtype :%d \n"
#define MTV_SERVICE_SUB_265_112_2_17_23_13_19_3509 "mbbmsservice_handlesubscribe received type:%d subtype:%d result:%dn"
#define MTV_SERVICE_SUB_268_112_2_17_23_13_19_3510 "mbbmsservice_handlesubscribe MBBMS_SERVICE_SUBSCRIBE OVER Already!\n"
#define MTV_SERVICE_SUB_279_112_2_17_23_13_19_3511 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register PDP fail. \n"
#define MTV_SERVICE_SUB_286_112_2_17_23_13_20_3512 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE register PDP OK. \n"
#define MTV_SERVICE_SUB_298_112_2_17_23_13_20_3513 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register Subscribe OK. \n"
#define MTV_SERVICE_SUB_303_112_2_17_23_13_20_3514 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register Subscribe Fail. \n"
#define MTV_SERVICE_SUB_310_112_2_17_23_13_20_3515 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register PDP Fail. \n"
#define MTV_SERVICE_SUB_319_112_2_17_23_13_20_3516 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE  Subscribe finished. \n"
#define MTV_SERVICE_SUB_330_112_2_17_23_13_20_3517 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register GBA OK. \n"
#define MTV_SERVICE_SUB_335_112_2_17_23_13_20_3518 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register GBA Fail. \n"
#define MTV_SERVICE_SUB_343_112_2_17_23_13_20_3519 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register Subscribe STOP OK. \n"
#define MTV_SERVICE_SUB_349_112_2_17_23_13_20_3520 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register Subscribe Unkown msg. Failed. \n"
#define MTV_SERVICE_SUB_356_112_2_17_23_13_20_3521 "[mbbms service]MBBMS_SERVICE_STOP_SUBSCRIBE request Received..... \n"
#define MTV_SERVICE_SUB_362_112_2_17_23_13_20_3522 "[mbbms service]MBBMS_SERVICE_STOP_SUBSCRIBE remove pdp Fail \n"
#define MTV_SERVICE_SUB_371_112_2_17_23_13_20_3523 "[mbbms service]MBBMS_SERVICE_SUBSCRIBE MTVSERVICE_RemoveRegister remove gba immediately \n"
#define MTV_SERVICE_SUB_384_112_2_17_23_13_20_3524 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE RemoveRegister Subscribe OK. \n"
#define MTV_SERVICE_SUB_396_112_2_17_23_13_20_3525 "[mbbms service]MTVSERVICE_RemoveRegister SUBSCRIBE return FAIL\n"
#define MTV_SERVICE_SUB_401_112_2_17_23_13_20_3526 "[mbbms service]MBBMS_SERVICE_STOP_SUBSCRIBE signal: Wrong State, unsub may be done already. \n"
#define MTV_SERVICE_SUB_414_112_2_17_23_13_20_3527 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE GBA  OK. \n"
#define MTV_SERVICE_SUB_420_112_2_17_23_13_20_3528 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE GBA  OK. \n"
#define MTV_SERVICE_SUB_429_112_2_17_23_13_20_3529 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE STOP GBA OK. Finished. \n"
#define MTV_SERVICE_SUB_436_112_2_17_23_13_20_3530 "[mbbms service] MBBMS_SERVICE_SUBSCRIBE GBA Fail. \n"
#define MTV_SERVICE_SUB_444_112_2_17_23_13_20_3531 "[mbbms service] mbmsservice_handleSUBSCRIBE UNKOWN subtype :%d \n"
#define MTV_SERVICE_SUB_454_112_2_17_23_13_20_3532 "mbbmsservice_handleupdatetable received type:%d subtype:%d result:%dn"
#define MTV_SERVICE_SUB_458_112_2_17_23_13_20_3533 "mbmsservice_handleUpdateTable MBBMS_SERVICE_UPDATE_TABLE OVER Already!\n"
#define MTV_SERVICE_SUB_470_112_2_17_23_13_20_3534 "[mbbms service] MBBMS_SERVICE_UPDATE_TABLE Register PDP fail. \n"
#define MTV_SERVICE_SUB_478_112_2_17_23_13_20_3535 "[mbbms service] MBBMS_SERVICE_UPDATE_TABLE Register PDP ok. \n"
#define MTV_SERVICE_SUB_490_112_2_17_23_13_20_3536 "[mbbms service] MBBMS_SERVICE_UPDATE_TABLE Register updatetable OK. \n"
#define MTV_SERVICE_SUB_497_112_2_17_23_13_20_3537 "[mbbms service] MBBMS_SERVICE_UPDATE_TABLE Register updatetable Fail. Over. \n"
#define MTV_SERVICE_SUB_505_112_2_17_23_13_20_3538 "[mbbms service] MBBMS_SERVICE_UPDATE_TABLE PDP active Fail. \n"
#define MTV_SERVICE_SUB_515_112_2_17_23_13_20_3539 "[mbbms service] MBBMS_SERVICE_UPDATE_TABLE  updatetable OK. \n"
#define MTV_SERVICE_SUB_523_112_2_17_23_13_20_3540 "[mbbms service] MBBMS_SERVICE_UPDATE_TABLE  updatetable Stop OK. \n"
#define MTV_SERVICE_SUB_530_112_2_17_23_13_20_3541 "[mbbms service] MBBMS_SERVICE_UPDATE_TABLE  updatetable Fail. Finished. \n"
#define MTV_SERVICE_SUB_538_112_2_17_23_13_20_3542 "[mbbms service]MBBMS_SERVICE_STOP_UPDATE_TABLE request Received..... \n"
#define MTV_SERVICE_SUB_545_112_2_17_23_13_20_3543 "[mbbms service]MBBMS_SERVICE_STOP_UPDATE_TABLE remove pdp Fail \n"
#define MTV_SERVICE_SUB_558_112_2_17_23_13_20_3544 "[mbbms service] MBBMS_SERVICE_UPDATE_TABLE RemoveRegister updatetable OK. \n"
#define MTV_SERVICE_SUB_568_112_2_17_23_13_20_3545 "[mbbms service]MTVSERVICE_RemoveRegister MBBMS_SERVICE_UPDATE_TABLE return FAIL\n"
#define MTV_SERVICE_SUB_574_112_2_17_23_13_20_3546 "[mbbms service]MBBMS_SERVICE_STOP_UPDATE_TABLE signal: Wrong State, unsub may be done already. \n"
#define MTV_SERVICE_SUB_580_112_2_17_23_13_20_3547 "[mbbms service] mbmsservice_handleUpdateTable UNKOWN subtype :%d \n"
#define MTV_SERVICE_SUB_590_112_2_17_23_13_20_3548 "mbbmsservice_handleaccountquiry received type:%d subtype:%d result:%dn"
#define MTV_SERVICE_SUB_594_112_2_17_23_13_20_3549 "mbmsservice_handleAccountQuiry MBBMS_SERVICE_ACCOUNT_QUIRY OVER Already!\n"
#define MTV_SERVICE_SUB_605_112_2_17_23_13_20_3550 "[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY Register pdp fail. \n"
#define MTV_SERVICE_SUB_613_112_2_17_23_13_20_3551 "[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY Register pdp OK. \n"
#define MTV_SERVICE_SUB_623_112_2_17_23_13_20_3552 "[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY Register accountquiry OK. \n"
#define MTV_SERVICE_SUB_629_112_2_17_23_13_20_3553 "[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY Register accountquiry Fail. \n"
#define MTV_SERVICE_SUB_636_112_2_17_23_13_20_3554 "[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY PDP Active Fail. \n"
#define MTV_SERVICE_SUB_645_112_2_17_23_13_20_3555 "[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY  accountquiry OK. \n"
#define MTV_SERVICE_SUB_652_112_2_17_23_13_20_3556 "[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY  accountquiry STOP OK. \n"
#define MTV_SERVICE_SUB_658_112_2_17_23_13_20_3557 "[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY  accountquiry not OK. Finished. \n"
#define MTV_SERVICE_SUB_665_112_2_17_23_13_20_3558 "[mbbms service]MBBMS_SERVICE_STOP_ACCOUNT_QUIRY request Received..... \n"
#define MTV_SERVICE_SUB_671_112_2_17_23_13_20_3559 "[mbbms service]MBBMS_SERVICE_STOP_ACCOUNT_QUIRY remove pdp Fail \n"
#define MTV_SERVICE_SUB_683_112_2_17_23_13_20_3560 "[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY removeRegister accountquiry OK. \n"
#define MTV_SERVICE_SUB_691_112_2_17_23_13_20_3561 "[mbbms service]MTVSERVICE_RemoveRegister MBBMS_SERVICE_ACCOUNT_QUIRY return FAIL\n"
#define MTV_SERVICE_SUB_700_112_2_17_23_13_20_3562 "[mbbms service]MBBMS_SERVICE_STOP_ACCOUNT_QUIRY signal: Wrong State, unsub may be done already. \n"
#define MTV_SERVICE_SUB_706_112_2_17_23_13_20_3563 "[mbbms service] mbmsservice_handleAccountQuiry UNKOWN subtype :%d \n"
#define MTV_SERVICE_SUB_715_112_2_17_23_13_20_3564 "mbbmsservice_handlegba received type:%d subtype:%d result:%dn"
#define MTV_SERVICE_SUB_718_112_2_17_23_13_20_3565 "mbmsservice_handleGBA MBBMS_SERVICE_GBA OVER Already!\n"
#define MTV_SERVICE_SUB_728_112_2_17_23_13_20_3566 "[mbbms service] MBBMS_SERVICE_GBA Register PDP fail. \n"
#define MTV_SERVICE_SUB_735_112_2_17_23_13_20_3567 "[mbbms service] MBBMS_SERVICE_GBA Register PDP OK. \n"
#define MTV_SERVICE_SUB_746_112_2_17_23_13_20_3568 "[mbbms service] MBBMS_SERVICE_GBA Register GBA OK. \n"
#define MTV_SERVICE_SUB_751_112_2_17_23_13_20_3569 "[mbbms service] MBBMS_SERVICE_GBA Register GBA fail. \n"
#define MTV_SERVICE_SUB_758_112_2_17_23_13_20_3570 "[mbbms service] MBBMS_SERVICE_GBA active PDP fail. \n"
#define MTV_SERVICE_SUB_769_112_2_17_23_13_21_3571 "MBBMS_SERVICE_GBA description len:%d impi:%sn"
#define MTV_SERVICE_SUB_775_112_2_17_23_13_21_3572 "[mbbms service] MBBMS_SERVICE_GBA GBA Stop OK. \n"
#define MTV_SERVICE_SUB_781_112_2_17_23_13_21_3573 "[mbbms service] MBBMS_SERVICE_GBA GBA Not OK. \n"
#define MTV_SERVICE_SUB_789_112_2_17_23_13_21_3574 "[mbbms service]MBBMS_SERVICE_STOP_GBA request Received..... \n"
#define MTV_SERVICE_SUB_795_112_2_17_23_13_21_3575 "[mbbms service]MBBMS_SERVICE_STOP_UNSUBSCRIBE remove pdp Fail \n"
#define MTV_SERVICE_SUB_804_112_2_17_23_13_21_3576 "[mbbms service]MTVSERVICE_RemoveRegister remove gba immediately \n"
#define MTV_SERVICE_SUB_810_112_2_17_23_13_21_3577 "[mbbms service]MBBMS_SERVICE_STOP_GBA signal: Wrong State, unsub may be done already. \n"
#define MTV_SERVICE_SUB_817_112_2_17_23_13_21_3578 "[mbbms service] mbmsservice_handleGBA UNKOWN subtype :%d \n"
#define MTV_SERVICE_SUB_826_112_2_17_23_13_21_3579 "mbbmsservice_handlegetmsk received type:%d subtype:%d result:%dn"
#define MTV_SERVICE_SUB_836_112_2_17_23_13_21_3580 "[mbbms service] MBBMS_SERVICE_GETMSK Register PDP fail. \n"
#define MTV_SERVICE_SUB_843_112_2_17_23_13_21_3581 "[mbbms service] MBBMS_SERVICE_GETMSK Register PDP OK. \n"
#define MTV_SERVICE_SUB_854_112_2_17_23_13_21_3582 "[mbbms service] MBBMS_SERVICE_GETMSK add register getmsk ok. \n"
#define MTV_SERVICE_SUB_860_112_2_17_23_13_21_3583 "[mbbms service] MBBMS_SERVICE_GETMSK add register getmsk Fail. \n"
#define MTV_SERVICE_SUB_867_112_2_17_23_13_21_3584 "[mbbms service] MBBMS_SERVICE_GETMSK PDP active Failed. \n"
#define MTV_SERVICE_SUB_877_112_2_17_23_13_21_3585 "[mbbms service] MBBMS_SERVICE_GETMSK OK. \n"
#define MTV_SERVICE_SUB_885_112_2_17_23_13_21_3586 "[mbbms service] MBBMS_SERVICE_GETMSK GBA Fail. \n"
#define MTV_SERVICE_SUB_889_112_2_17_23_13_21_3587 "[mbbms service] MBBMS_SERVICE_GETMSK GBA running. \n"
#define MTV_SERVICE_SUB_894_112_2_17_23_13_21_3588 "[mbbms service] MBBMS_SERVICE_GETMSK GBA running failed. \n"
#define MTV_SERVICE_SUB_903_112_2_17_23_13_21_3589 "[mbbms service] MBBMS_SERVICE_GETMSK STOP OK. \n"
#define MTV_SERVICE_SUB_910_112_2_17_23_13_21_3590 "[mbbms service] MBBMS_SERVICE_GETMSK Result error. \n"
#define MTV_SERVICE_SUB_918_112_2_17_23_13_21_3591 "[mbbms service]MBBMS_SERVICE_STOP_GETMSK request Received..... \n"
#define MTV_SERVICE_SUB_922_112_2_17_23_13_21_3592 "[mbbms service] MBBMS_SERVICE_GETMSK Remove PDP. \n"
#define MTV_SERVICE_SUB_926_112_2_17_23_13_21_3593 "[mbbms service]MBBMS_SERVICE_STOP_GETMSK remove pdp Fail \n"
#define MTV_SERVICE_SUB_934_112_2_17_23_13_21_3594 "[mbbms service] MBBMS_SERVICE_GETMSK Remove GBA \n"
#define MTV_SERVICE_SUB_937_112_2_17_23_13_21_3595 "[mbbms service]MTVSERVICE_RemoveRegister remove gba immediately \n"
#define MTV_SERVICE_SUB_946_112_2_17_23_13_21_3596 "[mbbms service] MBBMS_SERVICE_GETMSK Remove Get MSK \n"
#define MTV_SERVICE_SUB_951_112_2_17_23_13_21_3597 "[mbbms service] MBBMS_SERVICE_GETMSK RemoveRegister GetMSK OK. \n"
#define MTV_SERVICE_SUB_959_112_2_17_23_13_21_3598 "[mbbms service]MTVSERVICE_RemoveRegister MBBMS_SERVICE_GETMSK return FAIL\n"
#define MTV_SERVICE_SUB_967_112_2_17_23_13_21_3599 "[mbbms service]MBBMS_SERVICE_STOP_GETMSK signal: Wrong State, unsub may be done already. \n"
#define MTV_SERVICE_SUB_976_112_2_17_23_13_21_3600 "[mbbms service] MBBMS_SERVICE_GETMSK GBA Result received. \n"
#define MTV_SERVICE_SUB_982_112_2_17_23_13_21_3601 "[mbbms service] MBBMS_SERVICE_GETMSK Register GBA OK. \n"
#define MTV_SERVICE_SUB_987_112_2_17_23_13_21_3602 "[mbbms service] MBBMS_SERVICE_GETMSK Register GBA fail. \n"
#define MTV_SERVICE_SUB_995_112_2_17_23_13_21_3603 "[mbbms service] MBBMS_SERVICE_GETMSK GBA STOP OK. \n"
#define MTV_SERVICE_SUB_1001_112_2_17_23_13_21_3604 "[mbbms service] MBBMS_SERVICE_GETMSK GBA fail. \n"
#define MTV_SERVICE_SUB_1008_112_2_17_23_13_21_3605 "[mbbms service] mbmsservice_handleGETMSK UNKOWN subtype :%d \n"
#define MTV_SERVICE_SUB_1023_112_2_17_23_13_21_3606 "MBBMS mmi_callback received type:%d  subtype:%d result:%d ptr:%x n"
#define MTV_SERVICE_SUB_1114_112_2_17_23_13_21_3607 "MBBMS_SERVICE_TEST ... \n"
#define MTV_SERVICE_SUB_1288_112_2_17_23_13_22_3608 "MBBMS_SERVICE_TEST Over ... \n"
#define DAC_PEER_500_112_2_17_23_13_37_3609 "HYDAC_AP_Open"
#define MCEX_PEER_74_112_2_17_23_13_38_3610 "Send command failed!\n"
#define MCEX_PEER_80_112_2_17_23_13_38_3611 "Get response failed!\n"
#define MTV_IMP_PEER_176_112_2_17_23_13_39_3612 "CMMB_EnumAudioLanguages_wrapper_Imp: num(%d) != ret(%d)"
#define MTV_IMP_PEER_732_112_2_17_23_13_40_3613 "mtv_v:CMMB_GetServiceName() name_ptr = 0X%x"
#define MTV_IMP_PEER_757_112_2_17_23_13_40_3614 "mtv_v:CMMB_GetAllProg() nid = %d sid = %d date %d %d %d"
#define MTV_IMP_PEER_760_112_2_17_23_13_40_3615 "mtv_v:CMMB_GetAllProg() ret = %d proglist_ptr = 0X%x"
#define MTV_IMP_PEER_776_112_2_17_23_13_40_3616 "mtv_v:CMMB_GetEsgDateRange_Imp() sid = %d"
#define MTV_IMP_PEER_779_112_2_17_23_13_40_3617 "mtv_v:CMMB_GetEsgDateRange_Imp() ret = %d addr = %x  %x cmd=%x"
#define MTV_IMP_PEER_811_112_2_17_23_13_40_3618 "mtv_v:CMMB_SetEsgFilter_Imp() sid = %d date %d %d %d"
#define MUA_PEER_71_112_2_17_23_13_44_3619 "cmd1=%x %x"
#define MUA_PEER_102_112_2_17_23_13_44_3620 "cmd0=%x,%x,%x,%x,%x,%x,%x,%x"
#define MUA_PEER_136_112_2_17_23_13_44_3621 "exchange: %d, %d,%x"
#define MUA_PEER_173_112_2_17_23_13_44_3622 "MUACMD_MBMS_Imp mikey_len %d exceeds buffer len!!!"
#define MUA_PEER_191_112_2_17_23_13_44_3623 "MUACMD_MBMS_Imp resp_len %d exceeds buffer len!!!"
#define MUA_PEER_211_112_2_17_23_13_44_3624 "MBBMSSG_GetIsma_Imp: %d %d %p"
#define MUA_PEER_219_112_2_17_23_13_44_3625 "MBBMSSG_GetIsma_Imp: error %d"
#define MUA_PEER_241_112_2_17_23_13_44_3626 "buf0=%x,%x,%x,%x,%x,%x,%x,%x"
#define MUA_PEER_246_112_2_17_23_13_44_3627 "rspLen=%d,rspsw=0x%x"
#define PROXYCALLPEER_305_112_2_17_23_13_46_3628 "proxycall wait"
#define PROXYCALLPEER_399_112_2_17_23_13_46_3629 "proxycall thread wait %p %x"
#define PROXYCALLPEER_404_112_2_17_23_13_46_3630 "proxycall thread queue %d"
#define PROXYCALLPEER_487_112_2_17_23_13_46_3631 "Proxy_Exec_Thread %d queue %d"
#define PROXYCALLPEER_492_112_2_17_23_13_46_3632 "Proxy_Exec_Thread %d exit %p %d"
#define PROXYCALLPEER_514_112_2_17_23_13_46_3633 "Proxy_Exec_Thread %d recv unknown cmd %X"
#define PROXYCALLPEER_585_112_2_17_23_13_46_3634 "ProxyCall_IsInited: %d"
#define SFS_PEER_242_112_2_17_23_13_48_3635 "ReadFile: %02X %02X %02X %02X - %02X %02X %02X %02Xn"
#define SFS_PEER_654_112_2_17_23_13_49_3636 "ReadFile: %x %x %x %x - %x %x %x %xn"
#define SFS_PEER_781_112_2_17_23_13_49_3637 "ChangFileFdStruEndian: name_len = %d \n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(CMMB_TRC)
TRACE_MSG(CMMBDMX_61_112_2_17_23_8_39_0,"%02x%02x%02x%02x\n")
TRACE_MSG(CMMBDMX_MISC_195_112_2_17_23_8_44_1,"CMMB_WARN%s slot buff too small, drop %s[%x]\n")
TRACE_MSG(DEMOD_103_112_2_17_23_8_46_2,"DEMOD type %d")
TRACE_MSG(DEMUX_276_112_2_17_23_8_49_3,"\"\"\"\"#!!!!!!DemuxGetData error!")
TRACE_MSG(DEMUX_296_112_2_17_23_8_49_4,"\"\"\"\"#!!!!!!DemuxSendData error!")
TRACE_MSG(DTL_602_112_2_17_23_8_56_5,"DTL_DirectRWData RespQ_ptr->resp_id=%x error")
TRACE_MSG(DTL_694_112_2_17_23_8_56_6,"DTLSVC_Task()RespQ_ptr[%d].resp_id=%x,parameter0=%x")
TRACE_MSG(DTL_805_112_2_17_23_8_56_7,"DTL_SendData() block_num=%d,tail_size=%d, addr = %x")
TRACE_MSG(DTL_815_112_2_17_23_8_56_8,"DTL_SendData() block_num=%d error!!!")
TRACE_MSG(DTL_841_112_2_17_23_8_56_9,"DTL_SendData() tail_size=%d error!!!")
TRACE_MSG(DTL_928_112_2_17_23_8_57_10,"DTL_GetData() block_num=%d,tail_size=%d,src_addr=%x")
TRACE_MSG(DTL_937_112_2_17_23_8_57_11,"DTL_GetData() block_num=%d error!!!")
TRACE_MSG(DTL_960_112_2_17_23_8_57_12,"DTL_GetData() tail_size=%d error!!!")
TRACE_MSG(DTL_1034_112_2_17_23_8_57_13,"write cmd error")
TRACE_MSG(DTL_1049_112_2_17_23_8_57_14,"DTL_SendCMD()next_cmd_in=%d,cmd_to_send->cmd_id=%x,p0=%x,p1=%x (%d)")
TRACE_MSG(DTL_1060_112_2_17_23_8_57_15,"DTL_SendCMD() write_data error!!")
TRACE_MSG(DTL_1131_112_2_17_23_8_57_16,"DTL_SendCMD_NOINT()next_cmd_in=%d,cmd_out=%d,cmd_id=%d")
TRACE_MSG(DTL_1142_112_2_17_23_8_57_17,"DTL_SendCMD() write_data error!!")
TRACE_MSG(DTL_1225_112_2_17_23_8_57_18,"write cmd error")
TRACE_MSG(DTL_1253_112_2_17_23_8_57_19,"DTL_SendCMD_And_MsgParam() data_size = %d error!!!")
TRACE_MSG(DTL_1263_112_2_17_23_8_57_20,"DTL_SendCMD_And_MsgParam() tail_size = %d error!!!")
TRACE_MSG(DTL_1278_112_2_17_23_8_57_21,"DTL_SendCMD_And_MsgParam() write cmd error!!!")
TRACE_MSG(DTL_1344_112_2_17_23_8_57_22,"DTL_SendCMD_NOINT()next_cmd_in=%d,cmd_out=%d,cmd_id=%d")
TRACE_MSG(DTL_1355_112_2_17_23_8_57_23,"DTL_SendCMD() write_data error!!")
TRACE_MSG(DTL_1504_112_2_17_23_8_58_24,"DTL_GetCmd() tmp_ptr->resp_id=%x")
TRACE_MSG(DTL_LOCAL_361_112_2_17_23_9_5_25,"Demod_CallBack ack_id=0x%x")
TRACE_MSG(DTL_LOCAL_375_112_2_17_23_9_5_26,"Demod_dmx_callback resp_ptr->resp_id=0x%x")
TRACE_MSG(DTL_LOCAL_840_112_2_17_23_9_6_27,"DTL_RegisterMsg,event=%d,i=%d,task_id=0x%x")
TRACE_MSG(DTL_LOCAL_897_112_2_17_23_9_6_28,"DTL_UnregisterMsg,event=%d,i=%d,task_id=0x%x")
TRACE_MSG(DTL_LOCAL_986_112_2_17_23_9_7_29,"enable channel %d")
TRACE_MSG(DTL_LOCAL_992_112_2_17_23_9_7_30,"disable channel %d")
TRACE_MSG(DTL_LOCAL_1003_112_2_17_23_9_7_31,"SET DESCRAM KEY")
TRACE_MSG(DTL_LOCAL_1008_112_2_17_23_9_7_32,"SET ISMA")
TRACE_MSG(LCD_TV_1804_112_2_17_23_9_12_33,"LCDTV_Init,in")
TRACE_MSG(LCD_TV_1863_112_2_17_23_9_12_34,"LCDTV_InvalidateRect,in,(left,top)=(%d,%d),(right,bottom)=(%d,%d),lcd(w,h)=(%d,%d)")
TRACE_MSG(LCD_TV_1954_112_2_17_23_9_12_35,"LCD_Invalidate_6600V_MP4,lcd_spec,(w,h)=(%d,%d)")
TRACE_MSG(MTV_BOOT_775_112_2_17_23_9_16_36,"TVBOOT_TVOpenWqvga() not read BB_ARM_STATUS_DOWNLOAD_ARM_CODE_FINISHED command")
TRACE_MSG(MBBMS_GBA_284_112_2_17_23_9_19_37,"MBBMS GBA READY: GBA HAS BE DONE \n")
TRACE_MSG(MBBMS_GBA_292_112_2_17_23_9_19_38,"MBBMS GBA READY: RAND LEN: %d\n")
TRACE_MSG(MBBMS_GBA_293_112_2_17_23_9_19_39,"MBBMS GBA READY: B-TID LEN: %d\n")
TRACE_MSG(MBBMS_GBA_300_112_2_17_23_9_19_40,"MBBMS GBA READY: KS LEN: %d\n")
TRACE_MSG(MBBMS_GBA_313_112_2_17_23_9_19_41,"MBBMS GBA READY: MUACMD GET FAIL: UNKOWN\n")
TRACE_MSG(MBBMS_GBA_334_112_2_17_23_9_19_42,"MBBMS_GBA_Init: Entering\n")
TRACE_MSG(MBBMS_GBA_350_112_2_17_23_9_19_43,"Create gba authorize thread failed!!")
TRACE_MSG(MBBMS_GBA_423_112_2_17_23_9_19_44,"MBBMS_GBA_Init:UserAgent =%s \n")
TRACE_MSG(MBBMS_GBA_426_112_2_17_23_9_19_45,"MBBMS_GBA_Init: Leaving ret=%d \n")
TRACE_MSG(MBBMS_GBA_442_112_2_17_23_9_19_46,"MBBMS_GBA_Exit: Entering\n")
TRACE_MSG(MBBMS_GBA_446_112_2_17_23_9_19_47,"MBBMS_GBA_Exit: ABORT, GBA NOT INITED YET \n")
TRACE_MSG(MBBMS_GBA_468_112_2_17_23_9_19_48,"MBBMS_GBA_EXIT: Waiting Thread End \n")
TRACE_MSG(MBBMS_GBA_487_112_2_17_23_9_19_49,"MBBMS_GBA_Exit Successful, I AM LEAVING...\n")
TRACE_MSG(MBBMS_GBA_501_112_2_17_23_9_19_50,"MBBMS_Set_WAP: Entering\n")
TRACE_MSG(MBBMS_GBA_511_112_2_17_23_9_19_51,"MBBMS_Set_WAP: wap:%s port:%d Leaving\n")
TRACE_MSG(MBBMS_GBA_541_112_2_17_23_9_19_52,"MBBMS_Set_CMMBSN: Entering\n")
TRACE_MSG(MBBMS_GBA_550_112_2_17_23_9_19_53,"MBBMS_Set_CMMBSN: CMMBSN:%s Len:%d Leaving\n")
TRACE_MSG(MBBMS_GBA_561_112_2_17_23_9_19_54,"MBBMS_Set_User_Agent :%s \n")
TRACE_MSG(MBBMS_GBA_566_112_2_17_23_9_19_55,"MBBMS_Get_User_Agent len:%d string:%s \n")
TRACE_MSG(MBBMS_GBA_593_112_2_17_23_9_19_56,"MUACMD_MBBMS_GetCMMBSN return FALSE \n")
TRACE_MSG(MBBMS_GBA_608_112_2_17_23_9_19_57,"MBBMS_Set_NAF: Entering\n")
TRACE_MSG(MBBMS_GBA_619_112_2_17_23_9_19_58,"MBBMS_Set_NAF: naf:%s port:%d Leaving\n")
TRACE_MSG(MBBMS_GBA_635_112_2_17_23_9_19_59,"MBBMS_Set_NAF_Domain_Name: Entering\n")
TRACE_MSG(MBBMS_GBA_645_112_2_17_23_9_20_60,"MBBMS_Set_NAF_Domain_Name: %s Leaving\n")
TRACE_MSG(MBBMS_GBA_691_112_2_17_23_9_20_61,"MBBMS_GBA_Get_State: Entering\n")
TRACE_MSG(MBBMS_GBA_696_112_2_17_23_9_20_62,"MBBMS_GBA_Get_State: Leaving, State:GOOD \n")
TRACE_MSG(MBBMS_GBA_718_112_2_17_23_9_20_63,"MBBMS_GBA_Get_Data: Entering\n")
TRACE_MSG(MBBMS_GBA_726_112_2_17_23_9_20_64,"MBBMS_GBA_Get_Data: Leaving GOOD! \n")
TRACE_MSG(MBBMS_GBA_740_112_2_17_23_9_20_65,"MBBMS_MRK_Get_Data: Entering\n")
TRACE_MSG(MBBMS_GBA_747_112_2_17_23_9_20_66,"MBBMS_MRK_Get_Data: Failed, MRK not operated \n")
TRACE_MSG(MBBMS_GBA_754_112_2_17_23_9_20_67,"MBBMS_MRK_Get_Data: Leaving GOOD! \n")
TRACE_MSG(MBBMS_GBA_769_112_2_17_23_9_20_68,"MBBMS_GBAMRK_Done Checking .... \n")
TRACE_MSG(MBBMS_GBA_772_112_2_17_23_9_20_69,"MBBMS_GBAMRK_Done module_init not DONE! Over \n")
TRACE_MSG(MBBMS_GBA_777_112_2_17_23_9_20_70,"MBBMS_GBAMRK_Done GBA not DONE! Over \n")
TRACE_MSG(MBBMS_GBA_782_112_2_17_23_9_20_71,"MBBMS_GBAMRK_Done mrk not DONE! Over \n")
TRACE_MSG(MBBMS_GBA_795_112_2_17_23_9_20_72,"MBBMS_SIM_Support_CAS: Entering\n")
TRACE_MSG(MBBMS_GBA_804_112_2_17_23_9_20_73,"Enter SendCASChecking to Thread")
TRACE_MSG(MBBMS_GBA_821_112_2_17_23_9_20_74,"MBBMS_SIM_Support_CAS: Leaving  \n")
TRACE_MSG(MBBMS_GBA_840_112_2_17_23_9_20_75,"MBBMS_GBA_Operation: Entering\n")
TRACE_MSG(MBBMS_GBA_845_112_2_17_23_9_20_76,"MBBMS_MRK_Operation: FAILED. not inited, or no naf or wap\n")
TRACE_MSG(MBBMS_GBA_850_112_2_17_23_9_20_77,"MBBMS_GBA_Operation: NOT support CAS\n")
TRACE_MSG(MBBMS_GBA_855_112_2_17_23_9_20_78,"MBBMS_GBA_Operation: MBBMS_GBA is BUSY!\n")
TRACE_MSG(MBBMS_GBA_860_112_2_17_23_9_20_79,"Enter SendGBAWorking to Thread")
TRACE_MSG(MBBMS_GBA_870_112_2_17_23_9_20_80,"MBBMS_GBA_Operation: Leaving\n")
TRACE_MSG(MBBMS_GBA_886_112_2_17_23_9_20_81,"MBBMS_GBA_Stop_Operation: Entering\n")
TRACE_MSG(MBBMS_GBA_893_112_2_17_23_9_20_82,"Enter SendGBAWorking to Thread")
TRACE_MSG(MBBMS_GBA_902_112_2_17_23_9_20_83,"MBBMS_GBA_Stop_Operation: Leaving\n")
TRACE_MSG(MBBMS_GBA_919_112_2_17_23_9_20_84,"MBBMS_MRK_Operation: Entering\n")
TRACE_MSG(MBBMS_GBA_925_112_2_17_23_9_20_85,"MBBMS_MRK_Operation: FAILED. not inited, or no naf or wap\n")
TRACE_MSG(MBBMS_GBA_930_112_2_17_23_9_20_86,"MBBMS_MRK_Operation: NOT support CAS\n")
TRACE_MSG(MBBMS_GBA_936_112_2_17_23_9_20_87,"MBBMS_MRK_Operation: MBBMS_GBA is BUSY!\n")
TRACE_MSG(MBBMS_GBA_940_112_2_17_23_9_20_88,"Enter SendMRKWorking to Thread")
TRACE_MSG(MBBMS_GBA_952_112_2_17_23_9_20_89,"MBBMS_MRK_Operation: Leaving\n")
TRACE_MSG(MBBMS_GBA_969_112_2_17_23_9_20_90,"MBBMS_MRK_Stop_Operation: Entering\n")
TRACE_MSG(MBBMS_GBA_976_112_2_17_23_9_20_91,"Enter SendMRKStop to Thread")
TRACE_MSG(MBBMS_GBA_987_112_2_17_23_9_20_92,"MBBMS_MRK_Stop_Operation: Leaving\n")
TRACE_MSG(MBBMS_GBA_997_112_2_17_23_9_20_93,"MBBMS GBA sendMMI signal. sig_code:%d Result:%d \n")
TRACE_MSG(MBBMS_GBA_1016_112_2_17_23_9_20_94,"MBBMS GBA save MMI signal.  sig_code %d Result:%d \n")
TRACE_MSG(MBBMS_GBA_1046_112_2_17_23_9_20_95,"MBBMS Print:%s \n")
TRACE_MSG(MBBMS_GBA_1050_112_2_17_23_9_20_96,"MBBMS Print:%s \n")
TRACE_MSG(MBBMS_GBA_1067_112_2_17_23_9_20_97,"MBBMS_XML: item:%s value:%s \n")
TRACE_MSG(MBBMS_GBA_1083_112_2_17_23_9_20_98,"MBBMS_HTTP RESQN: item:%s value:%s \n")
TRACE_MSG(MBBMS_GBA_1096_112_2_17_23_9_20_99,"MBBMS IMPI:%s \n")
TRACE_MSG(MBBMS_GBA_1101_112_2_17_23_9_20_100,"MBBMS IMPI description is checking.... \n")
TRACE_MSG(MBBMS_GBA_1105_112_2_17_23_9_20_101,"MBBMS IMPI description received:%s \n")
TRACE_MSG(MBBMS_GBA_1111_112_2_17_23_9_20_102,"MBBMS IMPI description NOT received \n")
TRACE_MSG(MBBMS_GBA_1114_112_2_17_23_9_20_103,"MBBMS IMPI description received len:%d \n")
TRACE_MSG(MBBMS_GBA_1146_112_2_17_23_9_21_104,"MBBMS IMPI description NOT received \n")
TRACE_MSG(MBBMS_GBA_1148_112_2_17_23_9_21_105,"MBBMS IMPI description received:%s \n")
TRACE_MSG(MBBMS_GBA_1177_112_2_17_23_9_21_106,"MBBMS BSF:%s \n")
TRACE_MSG(MBBMS_GBA_1202_112_2_17_23_9_21_107,"MBBMS nonce = %s \n")
TRACE_MSG(MBBMS_GBA_1205_112_2_17_23_9_21_108,"MBBMS_REG_REQ_BASE64_decode begin....\n")
TRACE_MSG(MBBMS_GBA_1208_112_2_17_23_9_21_109," MBBMS_GBA RAND_AUTN_Other Len:%d \n")
TRACE_MSG(MBBMS_GBA_1212_112_2_17_23_9_21_110,"MBBMS_BASE64 RAND 1-8: 0x%02x %02x %02x %02x %02x %02x %02x %02xn")
TRACE_MSG(MBBMS_GBA_1214_112_2_17_23_9_21_111,"MBBMS_BASE64 RAND 9-16: 0x%02x %02x %02x %02x %02x %02x %02x %02xn")
TRACE_MSG(MBBMS_GBA_1225_112_2_17_23_9_21_112,"MBBMS_BASE64 AUTN len:%d\n")
TRACE_MSG(MBBMS_GBA_1227_112_2_17_23_9_21_113,"MBBMS_BASE64 AUTN 1-8: 0x%02x %02x %02x %02x %02x %02x %02x %02xn")
TRACE_MSG(MBBMS_GBA_1229_112_2_17_23_9_21_114,"MBBMS_BASE64 AUTN 9-16: 0x%02x %02x %02x %02x %02x %02x %02x %02xn")
TRACE_MSG(MBBMS_GBA_1234_112_2_17_23_9_21_115,"MBBMS_GBA_RAND NO AUTN DATA\n")
TRACE_MSG(MBBMS_GBA_1249_112_2_17_23_9_21_116,"MBBMS 2G KS_INPUT: 0x%x %x %x %x n")
TRACE_MSG(MBBMS_GBA_1255_112_2_17_23_9_21_117,"MBBMS_GBA_RAND NO AUTN DATA\n")
TRACE_MSG(MBBMS_GBA_1388_112_2_17_23_9_21_118,"MBBMS btid len:%d str[%s] \n")
TRACE_MSG(MBBMS_GBA_1399_112_2_17_23_9_21_119,"MBBMS GBA Lifetime:%d-%d-%d %d:%d:%d n")
TRACE_MSG(MBBMS_GBA_1469_112_2_17_23_9_21_120,"MBBMS_Free_Init_Param begin...\n")
TRACE_MSG(MBBMS_GBA_1482_112_2_17_23_9_21_121,"MBBMS_Free_Init_Param End\n")
TRACE_MSG(MBBMS_GBA_1488_112_2_17_23_9_21_122,"MBBMS_Free_Auth_Param begin...\n")
TRACE_MSG(MBBMS_GBA_1498_112_2_17_23_9_21_123,"MBBMS_Free_Auth_Param End\n")
TRACE_MSG(MBBMS_GBA_1504_112_2_17_23_9_21_124,"MBBMS_Free_Post_Param begin...\n")
TRACE_MSG(MBBMS_GBA_1508_112_2_17_23_9_21_125,"MBBMS_Free_Post_Param accept ptr freed \n")
TRACE_MSG(MBBMS_GBA_1511_112_2_17_23_9_21_126,"MBBMS_Free_Post_Param acceptcharsetptr freed \n")
TRACE_MSG(MBBMS_GBA_1515_112_2_17_23_9_21_127,"MBBMS_Free_Post_Param useragentptr freed \n")
TRACE_MSG(MBBMS_GBA_1519_112_2_17_23_9_21_128,"MBBMS_Free_Post_Param entityptr freed \n")
TRACE_MSG(MBBMS_GBA_1523_112_2_17_23_9_21_129,"MBBMS_Free_Post_Param passwordptr freed \n")
TRACE_MSG(MBBMS_GBA_1527_112_2_17_23_9_21_130,"MBBMS_Free_Post_Param usernameptr freed \n")
TRACE_MSG(MBBMS_GBA_1531_112_2_17_23_9_21_131,"MBBMS_Free_Post_Param refererptr freed\n")
TRACE_MSG(MBBMS_GBA_1535_112_2_17_23_9_21_132,"MBBMS_Free_Post_Param uriptr freed \n")
TRACE_MSG(MBBMS_GBA_1547_112_2_17_23_9_21_133,"MBBMS_Free_Post_Param otherheaderptr freed \n")
TRACE_MSG(MBBMS_GBA_1552_112_2_17_23_9_21_134,"MBBMS_Free_Post_Param postbodytypeptr freed \n")
TRACE_MSG(MBBMS_GBA_1556_112_2_17_23_9_21_135,"MBBMS_Free_Post_Param postbufferptr freed \n")
TRACE_MSG(MBBMS_GBA_1559_112_2_17_23_9_21_136,"MBBMS_Free_Post_Param end!\n")
TRACE_MSG(MBBMS_GBA_1566_112_2_17_23_9_21_137,"MBBMS_Free_Get_Param begin...\n")
TRACE_MSG(MBBMS_GBA_1598_112_2_17_23_9_21_138,"MBBMS_Free_Get_Param end!\n")
TRACE_MSG(MBBMS_GBA_1621_112_2_17_23_9_21_139,"MBBMS GBA Generate Auth Param password :%s \n")
TRACE_MSG(MBBMS_GBA_1644_112_2_17_23_9_22_140,"\n HTTP TEST no memory \n")
TRACE_MSG(MBBMS_GBA_1697_112_2_17_23_9_22_141,"MBBMS RESQN IS FALSE, INIT SEND.....\n")
TRACE_MSG(MBBMS_GBA_1718_112_2_17_23_9_22_142,"MBBMS RESQN IS TRUE, INIT SEND.....\n")
TRACE_MSG(MBBMS_GBA_1740_112_2_17_23_9_22_143,"MBBMS GBA_REGISTER SEND:\n ")
TRACE_MSG(MBBMS_GBA_1750_112_2_17_23_9_22_144,"MBBMS_GBA_HTTP REG_REQ  url:%s\n")
TRACE_MSG(MBBMS_GBA_1760_112_2_17_23_9_22_145,"MBBMS_GBA_HTTP REG_REQ  url:%s\n")
TRACE_MSG(MBBMS_GBA_1775_112_2_17_23_9_22_146,"\n HTTP TEST no memory \n")
TRACE_MSG(MBBMS_GBA_1861_112_2_17_23_9_22_147,"MBBMS GBA_AUTHORIZE SEND:\n ")
TRACE_MSG(MBBMS_GBA_1872_112_2_17_23_9_22_148,"MBBMS_GBA_HTTP REG_REQ  url:%s\n")
TRACE_MSG(MBBMS_GBA_1880_112_2_17_23_9_22_149,"MBBMS_GBA_HTTP REG_REQ  url:%s\n")
TRACE_MSG(MBBMS_GBA_1896_112_2_17_23_9_22_150,"MBBMS RESQN Prepare.....\n")
TRACE_MSG(MBBMS_GBA_1899_112_2_17_23_9_22_151,"MBBMS RESQN Auts base64 string:%s \n")
TRACE_MSG(MBBMS_GBA_1903_112_2_17_23_9_22_152,"MBBMS RESQN Parse Nonce FAILED\n")
TRACE_MSG(MBBMS_GBA_1909_112_2_17_23_9_22_153,"MBBMS RESQN Parse opaque FAILED, Set opaque empty\n")
TRACE_MSG(MBBMS_GBA_1925_112_2_17_23_9_22_154," MBBMS RESQN: MD5(\"\") = %s \n")
TRACE_MSG(MBBMS_GBA_1935_112_2_17_23_9_22_155,"MBBMS RESQN: MD5(\"GET:/:MD5()\") = %s \n")
TRACE_MSG(MBBMS_GBA_1946_112_2_17_23_9_22_156," MBBMS RESQN: MD5(%s:%s:)=%s n")
TRACE_MSG(MBBMS_GBA_1964_112_2_17_23_9_22_157,"MBBMS RESQN: final response = %s, set string:%s n")
TRACE_MSG(MBBMS_GBA_1981_112_2_17_23_9_22_158,"MBBMS_MD5_Auth_Data cnonce base64 string:%s \n")
TRACE_MSG(MBBMS_GBA_1985_112_2_17_23_9_22_159,"MBBMS_MD5_Auth_Data: Parse Nonce FAILED\n")
TRACE_MSG(MBBMS_GBA_1990_112_2_17_23_9_22_160,"MBBMS_MD5_Auth_Data: Parse realm FAILED\n")
TRACE_MSG(MBBMS_GBA_1995_112_2_17_23_9_22_161,"MBBMS_MD5_Auth_Data Parse opaque FAILED, Opaque is empty \n")
TRACE_MSG(MBBMS_GBA_2011_112_2_17_23_9_22_162,"MBBMS_MD5_Auth_Data: MD5(\"\") = %s \n")
TRACE_MSG(MBBMS_GBA_2021_112_2_17_23_9_22_163,"MBBMS_MD5_Auth_Data: MD5(\"GET:/:MD5()\") = %s \n")
TRACE_MSG(MBBMS_GBA_2037_112_2_17_23_9_22_164,"MBBMS_MD5_Auth_Data RES base64 string:%s \n")
TRACE_MSG(MBBMS_GBA_2043_112_2_17_23_9_22_165," MBBMS_MD5_Auth_Data: MD5(%s:%s:RES)=%s n")
TRACE_MSG(MBBMS_GBA_2047_112_2_17_23_9_22_166,"MBBMS_MD5_Auth_Data start:%s \n")
TRACE_MSG(MBBMS_GBA_2051_112_2_17_23_9_22_167,"MBBMS_MD5_Auth_Data NONCE:%s \n")
TRACE_MSG(MBBMS_GBA_2055_112_2_17_23_9_22_168,"MBBMS_MD5_Auth_Data nc_str:%s \n")
TRACE_MSG(MBBMS_GBA_2059_112_2_17_23_9_22_169,"MBBMS_MD5_Auth_Data cnonce:%s \n")
TRACE_MSG(MBBMS_GBA_2063_112_2_17_23_9_22_170,"MBBMS_MD5_Auth_Data qop:%s \n")
TRACE_MSG(MBBMS_GBA_2067_112_2_17_23_9_22_171,"MBBMS_MD5_Auth_Data end:%s \n")
TRACE_MSG(MBBMS_GBA_2073_112_2_17_23_9_22_172,"MBBMS_MD5_Auth_Data: final response = %s, set string:%s n")
TRACE_MSG(MBBMS_GBA_2092_112_2_17_23_9_22_173,"MBBMS_GBA 3G SIM HandlE_SIM_ALGO OK: USIM \n")
TRACE_MSG(MBBMS_GBA_2108_112_2_17_23_9_22_174,"MBBMS SDCARD Result:status len-data: %d-0x%x res %d-0x%x cnonce %d-0x%x n")
TRACE_MSG(MBBMS_GBA_2112_112_2_17_23_9_23_175,"MBBMS_GBA_ALGO RES_len:%d\n")
TRACE_MSG(MBBMS_GBA_2115_112_2_17_23_9_23_176,"MBBMS_GBA_ALGO RES 0x%x %x %x %x %x %x %x %x n")
TRACE_MSG(MBBMS_GBA_2122_112_2_17_23_9_23_177,"MBBMS SD CNONCE:0x%x \n")
TRACE_MSG(MBBMS_GBA_2131_112_2_17_23_9_23_178,"MBBMS THREAD_GBA_SEND_AUTHORIZE starting... \n")
TRACE_MSG(MBBMS_GBA_2144_112_2_17_23_9_23_179,"MBBMS_GBA SD Card Return RES ERROR \n")
TRACE_MSG(MBBMS_GBA_2155_112_2_17_23_9_23_180,"MBBMS_GBA 2G SIM HandlE_SIM_ALGO OK: SIM \n")
TRACE_MSG(MBBMS_GBA_2173_112_2_17_23_9_23_181,"MBBMS_GBA_ALGO 2G RES_len:%d\n")
TRACE_MSG(MBBMS_GBA_2176_112_2_17_23_9_23_182,"MBBMS_GBA_ALGO 2G RES 0x%x %x %x %x %x %x %x %x n")
TRACE_MSG(MBBMS_GBA_2192_112_2_17_23_9_23_183,"MBBMS 2G SD CNONCE:0x%x \n")
TRACE_MSG(MBBMS_GBA_2213_112_2_17_23_9_23_184,"MBBMS_GBA SD Card Return RES ERROR \n")
TRACE_MSG(MBBMS_GBA_2228_112_2_17_23_9_23_185,"MBBMS_GBA SIM ALGO CNF USIM ERROR: TO RESQN %d\n")
TRACE_MSG(MBBMS_GBA_2235_112_2_17_23_9_23_186,"MBBMS GBA RESQN Prepare Failed \n")
TRACE_MSG(MBBMS_GBA_2250_112_2_17_23_9_23_187,"MBBMS_GBA SIM ALGO CNF ABNORMAL ERROR: %d\n")
TRACE_MSG(MBBMS_GBA_2268_112_2_17_23_9_23_188,"\"#MBBMS GBA HTTP INIT CNF ERROR!!\n")
TRACE_MSG(MBBMS_GBA_2277_112_2_17_23_9_23_189,"\n MBBMS GBA HTTP TEST no memory \n")
TRACE_MSG(MBBMS_GBA_2360_112_2_17_23_9_23_190,"\"# MBBMS User Agent NOT SET or too short \n")
TRACE_MSG(MBBMS_GBA_2381_112_2_17_23_9_23_191,"\"# MBBMS User Agent NOT SET or too short \n")
TRACE_MSG(MBBMS_GBA_2390_112_2_17_23_9_23_192,"MBBMS_HTTP gba_init_req send XML:\nMBBMS %s \nMBBMS---end---\n")
TRACE_MSG(MBBMS_GBA_2448_112_2_17_23_9_23_193,"MBBMS_Handle_SIG_HEADER_IND rsp:%d  content:%s \n")
TRACE_MSG(MBBMS_GBA_2454_112_2_17_23_9_23_194,"MBBMS_GBA_HTTP_SIG_HEADER_IND HTTP FAILED!!!\n")
TRACE_MSG(MBBMS_GBA_2461_112_2_17_23_9_23_195,"MBBMS_GBA_HTTP_SIG_HEADER_IND ok!!!\n")
TRACE_MSG(MBBMS_GBA_2472_112_2_17_23_9_23_196,"THREAD_GBA_RESQN HEADER:%s \n")
TRACE_MSG(MBBMS_GBA_2489_112_2_17_23_9_23_197,"THREAD_GBA_RESQN RunMBBMSAlgo Starting.... \n")
TRACE_MSG(MBBMS_GBA_2494_112_2_17_23_9_23_198,"THREAD_GBA_RESQN RunMbbmsAlgo Failed \n")
TRACE_MSG(MBBMS_GBA_2505_112_2_17_23_9_23_199,"THREAD_GBA_RESQN HTTP Failed \n")
TRACE_MSG(MBBMS_GBA_2521_112_2_17_23_9_23_200,"MBBMS_GBA_HTTP REG RES data:%s \n")
TRACE_MSG(MBBMS_GBA_2544_112_2_17_23_9_24_201,"MBBMS_GBA_SIM RunMBBMSAlgo Starting.... \n")
TRACE_MSG(MBBMS_GBA_2549_112_2_17_23_9_24_202,"MBBMS_GBA SIM RunMbbmsAlgo Failed \n")
TRACE_MSG(MBBMS_GBA_2563_112_2_17_23_9_24_203," MBBMS REGISTER RAND, AUTN PARSE FAILED\n")
TRACE_MSG(MBBMS_GBA_2569_112_2_17_23_9_24_204,"MBBMS gba register response header overflow\n")
TRACE_MSG(MBBMS_GBA_2580_112_2_17_23_9_24_205,"MBBMS_GBA_HTTP_SIG_HEADER_IND THREAD_GBA_SEND_AUTHORIZE FAILED!!!\n")
TRACE_MSG(MBBMS_GBA_2591_112_2_17_23_9_24_206,"MBBMS_GBA_HTTP_SIG_HEADER_IND THREAD_GBA_SEND_AUTHORIZE data: \n")
TRACE_MSG(MBBMS_GBA_2597_112_2_17_23_9_24_207,"gba register response header overflow:length %d\n")
TRACE_MSG(MBBMS_GBA_2611_112_2_17_23_9_24_208,"MBBMS GBA HTTP_SIG_DATA_IND Received:resp code%d \n")
TRACE_MSG(MBBMS_GBA_2620_112_2_17_23_9_24_209,"MBBMS GBA HTTP_SIG_DATA_IND len:%d body:%s n")
TRACE_MSG(MBBMS_GBA_2632_112_2_17_23_9_24_210,"MBBMS_GBA_HTTP_SIG_CONTENT_IND THREAD_GBA_SEND_AUTHORIZE data:%s \n")
TRACE_MSG(MBBMS_GBA_2654_112_2_17_23_9_24_211,"[MBBMS GBA] BTID string:%s \n")
TRACE_MSG(MBBMS_GBA_2658_112_2_17_23_9_24_212,"MBBMS GBA UPDATE BTID & KS SUCCESSFUL \n")
TRACE_MSG(MBBMS_GBA_2660_112_2_17_23_9_24_213,"MBBMS GBA OPERATION FINISHED, HELLO WORLD !!! \n")
TRACE_MSG(MBBMS_GBA_2666_112_2_17_23_9_24_214,"MBBMS GBA UPDATE BTID & KS FAIL. GBA FAIL\n")
TRACE_MSG(MBBMS_GBA_2678_112_2_17_23_9_24_215,"MBBMS_GBA THREAD_GBA_SET_BTID_RES Over\n")
TRACE_MSG(MBBMS_GBA_2683_112_2_17_23_9_24_216,"MBBMS GBA Parse B-TID and Lifetime FAILED!!!")
TRACE_MSG(MBBMS_GBA_2703_112_2_17_23_9_24_217,"MBBMS GBA HTTP_SIG_CLOSE_CNF Received....\n")
TRACE_MSG(MBBMS_GBA_2724_112_2_17_23_9_24_218,"MBBMS GBA HTTP_AUTH_REQ: RES is not available !!\n")
TRACE_MSG(MBBMS_GBA_2740_112_2_17_23_9_24_219,"MBBMS_GBA_HTTP_SIG_GET_DATA_CNF  THREAD_GBA_SEND_INIT!!!\n")
TRACE_MSG(MBBMS_GBA_2741_112_2_17_23_9_24_220,"MBBMS_GBA_SEND_INIT_RES: xml: %s\n")
TRACE_MSG(MBBMS_GBA_2747_112_2_17_23_9_24_221,"MBBMS_GBA_HTTP_SIG_GET_DATA_CNF  THREAD_GBA_SEND_INIT Parsed Failed\n")
TRACE_MSG(MBBMS_GBA_2763_112_2_17_23_9_24_222,"MBBMS  Parse IMPI IS NOT EQUAL WITH IMSI \n")
TRACE_MSG(MBBMS_GBA_2776_112_2_17_23_9_24_223,"MBBMS_GBA_HTTP THREAD_GBA_SEND_INIT Parsed OK IMPI:%s bsf:%sn")
TRACE_MSG(MBBMS_GBA_2808_112_2_17_23_9_24_224,"MBBMS_GBA_Thread Starting...... \n")
TRACE_MSG(MBBMS_GBA_2823_112_2_17_23_9_24_225,"MBBMS GBA Register MN_APP_PHONE_SERVICE Failed \n")
TRACE_MSG(MBBMS_GBA_2830_112_2_17_23_9_24_226,"MBBMS_GBA_THread One signal received %d\n")
TRACE_MSG(MBBMS_GBA_2835_112_2_17_23_9_24_227,"MBBMS_INNER_GBA_STOP received\n")
TRACE_MSG(MBBMS_GBA_2862_112_2_17_23_9_24_228,"MBBMS_INNER_MRK_STOP received\n")
TRACE_MSG(MBBMS_GBA_2874_112_2_17_23_9_24_229,"MBBMS_INNER_EXIT received\n")
TRACE_MSG(MBBMS_GBA_2880_112_2_17_23_9_24_230,"MBBMS_INNER_GBA received\n")
TRACE_MSG(MBBMS_GBA_2892_112_2_17_23_9_24_231,"MBBMS_INNER_GBA: SIM TYPE Return FAILED\n")
TRACE_MSG(MBBMS_GBA_2896_112_2_17_23_9_24_232,"MBBMS_INNER_GBA: SIM TYPE is %d n")
TRACE_MSG(MBBMS_GBA_2903_112_2_17_23_9_24_233,"MBBMS_INNER_GBA: SIM TYPE GET FAILED\n")
TRACE_MSG(MBBMS_GBA_2909_112_2_17_23_9_24_234,"MBBMS_INNER_GBA: Get IMSI 0x%x %x %x %x n")
TRACE_MSG(MBBMS_GBA_2915_112_2_17_23_9_24_235,"MBBMS_INNER_GBA:IMSI string:%s \n")
TRACE_MSG(MBBMS_GBA_2940_112_2_17_23_9_24_236,"MBBMS GBA HTTP_InitRequest FAILED \n")
TRACE_MSG(MBBMS_GBA_2943_112_2_17_23_9_24_237,"MBBMS_INNER_GBA: GBA HTTP_INIT launched...\n")
TRACE_MSG(MBBMS_GBA_2949_112_2_17_23_9_24_238,"MBBMS_INNER_MRK received\n")
TRACE_MSG(MBBMS_GBA_2969_112_2_17_23_9_24_239,"MBBMS MRK naf domain:%s \n")
TRACE_MSG(MBBMS_GBA_2977_112_2_17_23_9_25_240,"MBBMS MRK NAF-ID: %s 0x1 0 0 0 1\n")
TRACE_MSG(MBBMS_GBA_2979_112_2_17_23_9_25_241,"MBBMS MRK impi: %s\n")
TRACE_MSG(MBBMS_GBA_2998_112_2_17_23_9_25_242,"MBBMS GBA MRK SD CARD OK len:%d 0x%x %x %x %xn")
TRACE_MSG(MBBMS_GBA_3001_112_2_17_23_9_25_243,"MBBMS GBA MRK SUCCESSFUL!\n")
TRACE_MSG(MBBMS_GBA_3006_112_2_17_23_9_25_244,"MBBMS MRK Result:len:%d ks_ext_naf:0x%2x %2x %2x %2x n")
TRACE_MSG(MBBMS_GBA_3008_112_2_17_23_9_25_245,"MBBMS MRK finished Successfully\n")
TRACE_MSG(MBBMS_GBA_3017_112_2_17_23_9_25_246,"MBBMS GBA MRK DATA ERROR \n")
TRACE_MSG(MBBMS_GBA_3029_112_2_17_23_9_25_247,"MBBMS GBA MRK Return SD Failed\n")
TRACE_MSG(MBBMS_GBA_3044_112_2_17_23_9_25_248,"MBBMS_GBA HTT_SIG_INIT_CNF received\n")
TRACE_MSG(MBBMS_GBA_3063_112_2_17_23_9_25_249,"MBBMS_GBA HTTP_SIG_NEED_AUTH_IND received\n")
TRACE_MSG(MBBMS_GBA_3070_112_2_17_23_9_25_250,"MBBMS_GBA APP_MN_SIM_MBBMS_ALGO_CNF received\n")
TRACE_MSG(MBBMS_GBA_3080_112_2_17_23_9_25_251,"MBBMS_GBA HTTP_SIG_REQUEST_ID_IND received\n")
TRACE_MSG(MBBMS_GBA_3089_112_2_17_23_9_25_252,"MBBMS_GBA HTTP_SIG_GET_CNF received\n")
TRACE_MSG(MBBMS_GBA_3098_112_2_17_23_9_25_253,"MBBMS_GBA HTTP_SIG_HEAD_CNF received\n")
TRACE_MSG(MBBMS_GBA_3107_112_2_17_23_9_25_254,"MBBMS_GBA HTTP_SIG_DATA_IND received\n")
TRACE_MSG(MBBMS_GBA_3114_112_2_17_23_9_25_255,"MBBMS_GBA HTTP_SIG_HEADER_IND received\n")
TRACE_MSG(MBBMS_GBA_3121_112_2_17_23_9_25_256,"MBBMS_GBA HTTP_SIG_ERROR_IND received\n")
TRACE_MSG(MBBMS_GBA_3134_112_2_17_23_9_25_257,"MBBMS_GBA HTTP_SIG_CLOSE_CNF received\n")
TRACE_MSG(MBBMS_GBA_3150_112_2_17_23_9_25_258,"MBBMS GBA Thread: Freeing gba_sig_ptr \n")
TRACE_MSG(MBBMS_GBA_3152_112_2_17_23_9_25_259,"MBBMS GBA Thread: Freed gba_sig_ptr \n")
TRACE_MSG(MBBMS_GBA_3158_112_2_17_23_9_25_260,"MBBMS_GBA HTTP_SIG_CANCEL_CNF received\n")
TRACE_MSG(MBBMS_GBA_3165_112_2_17_23_9_25_261,"\n http test net prog ind\n")
TRACE_MSG(MBBMS_GBA_3168_112_2_17_23_9_25_262,"\n http test redirect ind\n")
TRACE_MSG(MBBMS_GBA_3172_112_2_17_23_9_25_263,"MBBMS_GBA HTTP_SIG_POST_CNF received\n")
TRACE_MSG(MBBMS_GBA_3202_112_2_17_23_9_25_264,"MBBMS GBA: set callback function\n")
TRACE_MSG(MBBMS_GBA_3208_112_2_17_23_9_25_265,"MBBMS GBA: clean callback function\n")
TRACE_MSG(MBBMS_GBA_3213_112_2_17_23_9_25_266,"MBBMS GBA: set net id to [%d]\n")
TRACE_MSG(MBBMS_GBA_3252_112_2_17_23_9_25_267,"parse life time year:%d month:%d day:%d hour:%d minute:%d second:%d")
TRACE_MSG(MBBMS_GBA_3260_112_2_17_23_9_25_268,"mbbms_get_current_time %s, len:%d\n")
TRACE_MSG(MBBMS_GBA_3262_112_2_17_23_9_25_269,"MBBMS_get_current_time FAILED!!!\n")
TRACE_MSG(MBBMS_GBA_3264_112_2_17_23_9_25_270,"MBBMS_GBA_INIT failed!!!\n")
TRACE_MSG(MBBMS_GBA_3266_112_2_17_23_9_25_271,"MBBMS_GBA_Init SUcess!\n")
TRACE_MSG(MBBMS_GBA_3274_112_2_17_23_9_25_272,"base64 encode successful!\n len=%d \n %s \n")
TRACE_MSG(MBBMS_GBA_3277_112_2_17_23_9_25_273,"base64 decode successful!\n len=%d \n %s \n")
TRACE_MSG(MBBMS_GBA_3279_112_2_17_23_9_25_274,"base64 decode FAILED!!!\n")
TRACE_MSG(MBBMS_GBA_3282_112_2_17_23_9_25_275,"base64 encode FAILED!!!\n")
TRACE_MSG(MBBMS_GBA_3288_112_2_17_23_9_25_276,"MD5 of \"\" :\n")
TRACE_MSG(MBBMS_GBA_3291_112_2_17_23_9_25_277,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x")
TRACE_MSG(MBBMS_GBA_3295_112_2_17_23_9_25_278,"MD5 of \"a\" :\n")
TRACE_MSG(MBBMS_GBA_3298_112_2_17_23_9_25_279,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x")
TRACE_MSG(MBBMS_GBA_3303_112_2_17_23_9_25_280,"MD5 of \"abc\" :\n")
TRACE_MSG(MBBMS_GBA_3306_112_2_17_23_9_25_281,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x")
TRACE_MSG(MBBMS_GBA_3310_112_2_17_23_9_25_282,"mbbms init res ok! bsf:%s impi:%s \n")
TRACE_MSG(MBBMS_GBA_3313_112_2_17_23_9_25_283,"mbbms auth response failed!!!\n")
TRACE_MSG(MBBMS_GBA_3319_112_2_17_23_9_25_284,"mbbms auth ok! date:%d-%d-%d n")
TRACE_MSG(MBBMS_GBA_3321_112_2_17_23_9_25_285,"mbbms auth ok! lifetime:%d time:%d:%d:%d n")
TRACE_MSG(MBBMS_GBA_3323_112_2_17_23_9_25_286,"mbbms auth ok! btid:%s \n")
TRACE_MSG(MBBMS_GBA_3326_112_2_17_23_9_25_287,"mbbms auth response failed!!!\n")
TRACE_MSG(MBBMS_GBA_3332_112_2_17_23_9_25_288,"MBBMS_Set_WAP failed!!!\n")
TRACE_MSG(MBBMS_GBA_3334_112_2_17_23_9_25_289,"MBBMS_Set_WAP SUcess!\n")
TRACE_MSG(MBBMS_GBA_3336_112_2_17_23_9_25_290,"MBBMS_Get_WAP failed!!!\n")
TRACE_MSG(MBBMS_GBA_3338_112_2_17_23_9_25_291,"MBBMS_Get_WAP SUcess! wap:%s, port:%d\n")
TRACE_MSG(MBBMS_GBA_3342_112_2_17_23_9_25_292,"MBBMS_Set_NAF failed!!!\n")
TRACE_MSG(MBBMS_GBA_3344_112_2_17_23_9_25_293,"MBBMS_Set_NAF SUcess!\n")
TRACE_MSG(MBBMS_GBA_3346_112_2_17_23_9_25_294,"MBBMS_Get_NAF failed!!!\n")
TRACE_MSG(MBBMS_GBA_3348_112_2_17_23_9_25_295,"MBBMS_Get_NAF SUcess! wap:%s, port:%d\n")
TRACE_MSG(MBBMS_GBA_3352_112_2_17_23_9_25_296,"MBBMS_Set_CMMBSN failed!!!\n")
TRACE_MSG(MBBMS_GBA_3354_112_2_17_23_9_25_297,"MBBMS_Set_CMMBSN SUcess!\n")
TRACE_MSG(MBBMS_GBA_3357_112_2_17_23_9_25_298,"MBBMS_Get_CMMBSN failed!!!\n")
TRACE_MSG(MBBMS_GBA_3359_112_2_17_23_9_25_299,"MBBMS_Get_CMMBSN SUcess! cmmbsn:%s, len:%d\n")
TRACE_MSG(MBBMS_GBA_3362_112_2_17_23_9_25_300,"MBBMS_SIM_Support_CAS failed!!!\n")
TRACE_MSG(MBBMS_GBA_3364_112_2_17_23_9_25_301,"MBBMS_SIM_Support_CAS SUcess!\n")
TRACE_MSG(MBBMS_GBA_3367_112_2_17_23_9_25_302,"MBBMS_GBA_Operation failed!!!\n")
TRACE_MSG(MBBMS_GBA_3369_112_2_17_23_9_25_303,"MBBMS_GBA_Operation SUcess!\n")
TRACE_MSG(MBBMS_GBA_3373_112_2_17_23_9_25_304,"MBBMS_GBA_Stop_Operation failed!!!\n")
TRACE_MSG(MBBMS_GBA_3375_112_2_17_23_9_25_305,"MBBMS_GBA_Stop_Operation SUcess!\n")
TRACE_MSG(MBBMS_GBA_3378_112_2_17_23_9_25_306,"MBBMS_MRK_Operation failed!!!\n")
TRACE_MSG(MBBMS_GBA_3380_112_2_17_23_9_25_307,"MBBMS_MRK_Operation SUcess!\n")
TRACE_MSG(MBBMS_GBA_3383_112_2_17_23_9_25_308,"MBBMS_MRK_Stop_Operation failed!!!\n")
TRACE_MSG(MBBMS_GBA_3385_112_2_17_23_9_25_309,"MBBMS_MRK_Stop_Operation SUcess!\n")
TRACE_MSG(MBBMS_SG_642_112_2_17_23_9_27_310,"[SP] i %d,x%x \n")
TRACE_MSG(MBBMS_SG_1002_112_2_17_23_9_27_311,"Esg_InflateData() ,src_handle = %d, dst_handle = %d")
TRACE_MSG(MBBMS_SG_1014_112_2_17_23_9_27_312,"Esg_InflateData() ,z_inflateInit2_ = %d!")
TRACE_MSG(MBBMS_SG_1049_112_2_17_23_9_28_313,"Esg_InflateData() ,z_inflate = %d!")
TRACE_MSG(MBBMS_SG_1060_112_2_17_23_9_28_314,"Esg_InflateData() ,ignore -3 error. now ret = %d!")
TRACE_MSG(MBBMS_SG_1070_112_2_17_23_9_28_315,"Esg_InflateData() ,SFS_WriteFile = %d!")
TRACE_MSG(MBBMS_SG_1197_112_2_17_23_9_28_316,"[SG] SetAllocFail flag=%d")
TRACE_MSG(MBBMS_SG_1212_112_2_17_23_9_28_317,"[SG] SetParseAgain flag=%d")
TRACE_MSG(MBBMS_SG_1348_112_2_17_23_9_28_318,"[SG]:  SendPreviewSigToMMI \n")
TRACE_MSG(MBBMS_SG_1439_112_2_17_23_9_28_319,"[SG]:SendStopSigToMmi  g_mbbms_stop_flag %d\n")
TRACE_MSG(MBBMS_SG_1455_112_2_17_23_9_28_320,"[SG]:SendStopSigToMmi\n")
TRACE_MSG(MBBMS_SG_1483_112_2_17_23_9_28_321,"[SG]:InitHttpRequest  SendMemAllocFailToMmi \n")
TRACE_MSG(MBBMS_SG_1502_112_2_17_23_9_28_322,"[SG] InitHttpRequest: task_id 0x%x\n ")
TRACE_MSG(MBBMS_SG_1548_112_2_17_23_9_29_323,"[SG]: InitHttpRequest wap=%s \n, port 0x%x,netid %d")
TRACE_MSG(MBBMS_SG_1558_112_2_17_23_9_29_324,"[SG]:InitHttpRequest  HTTP_InitRequest error %d \n")
TRACE_MSG(MBBMS_SG_1654_112_2_17_23_9_29_325,"[SG] InitPreviewDataHttpReqest! enter tick %d ,is_success %d \n")
TRACE_MSG(MBBMS_SG_1822_112_2_17_23_9_29_326,"[SG] SetProccessFlag state %d\n ")
TRACE_MSG(MBBMS_SG_1843_112_2_17_23_9_29_327,"[SG] SetState state %d\n ")
TRACE_MSG(MBBMS_SG_1915_112_2_17_23_9_29_328,"[SG] HandleHTTPCloseCnf\n ")
TRACE_MSG(MBBMS_SG_1961_112_2_17_23_9_29_329,"[SG] HandleHTTPCloseCnf   sgdd has been downloaded\n")
TRACE_MSG(MBBMS_SG_1968_112_2_17_23_9_29_330,"[SG] HandleHTTPCloseCnf   sgdd same with old sgdd\n")
TRACE_MSG(MBBMS_SG_2030_112_2_17_23_9_30_331,"[SG]:CloseHttpRequest HTTP_CloseRequest %d\n ")
TRACE_MSG(MBBMS_SG_2091_112_2_17_23_9_30_332,"[SG] RenamePreviewDataFile!delete old file sfs_err%d\n")
TRACE_MSG(MBBMS_SG_2102_112_2_17_23_9_30_333,"[SG] RenamePreviewDataFile!diffrent:fail to rename sfs_err%d\n")
TRACE_MSG(MBBMS_SG_2121_112_2_17_23_9_30_334,"[SG] GetFileFormat ")
TRACE_MSG(MBBMS_SG_2134_112_2_17_23_9_30_335,"[SG] GetFileFormat fail to read File err %d \n")
TRACE_MSG(MBBMS_SG_2143_112_2_17_23_9_30_336,"[SG] GetFileFormat format_type 0x%x ")
TRACE_MSG(MBBMS_SG_2159_112_2_17_23_9_30_337,"[SG] GetFileFormat fail to set File point err%d\n")
TRACE_MSG(MBBMS_SG_2212_112_2_17_23_9_30_338,"[SG] ProcessPreviewDataHTTPContentFile! begin inflate \n")
TRACE_MSG(MBBMS_SG_2222_112_2_17_23_9_30_339,"[SG]InflateFile  handle = %x!")
TRACE_MSG(MBBMS_SG_2234_112_2_17_23_9_30_340,"[SG] ProcessPreviewDataHTTPContentFile inflate_retf %d\n")
TRACE_MSG(MBBMS_SG_2238_112_2_17_23_9_30_341,"[SG] ProcessPreviewDataHTTPContentFile fail to Inflate File\n")
TRACE_MSG(MBBMS_SG_2247_112_2_17_23_9_30_342,"[SG] ProcessPreviewDataHTTPContentFile! non-gzip \n")
TRACE_MSG(MBBMS_SG_2261_112_2_17_23_9_30_343,"[SG] ProcessPreviewDataHTTPContentFile! end tick %d \n")
TRACE_MSG(MBBMS_SG_2282_112_2_17_23_9_30_344,"[SG]:HandleHTTPGetCnf  tick %d \n")
TRACE_MSG(MBBMS_SG_2286_112_2_17_23_9_30_345,"[SG]:HandleHTTPGetCnf  g_mbbms_stop_flag %d ,data len %d, response_code %dn")
TRACE_MSG(MBBMS_SG_2311_112_2_17_23_9_30_346,"[SG]:HandleHTTPDataCnf  HTTP_CloseRequest error %d \n")
TRACE_MSG(MBBMS_SG_2342_112_2_17_23_9_30_347,"[SG] HandleHTTPDataCnf   sgdd same with old sgdd\n")
TRACE_MSG(MBBMS_SG_2368_112_2_17_23_9_30_348,"[SG] InitHttpRequest: task_id 0x%x ,signalcode 0x%x\n ")
TRACE_MSG(MBBMS_SG_2497_112_2_17_23_9_30_349,"[SG] SetDeviceName \n ")
TRACE_MSG(MBBMS_SG_2521_112_2_17_23_9_30_350,"[SG] CleanDeviceName \n ")
TRACE_MSG(MBBMS_SG_2543_112_2_17_23_9_31_351,"[SG]MBBMS_SGInit full_path_len%d\n ")
TRACE_MSG(MBBMS_SG_2551_112_2_17_23_9_31_352,"[SG]MBBMS_SGInit  no mem\n ")
TRACE_MSG(MBBMS_SG_2567_112_2_17_23_9_31_353,"[SG]MBBMS_SGInit  no mem\n ")
TRACE_MSG(MBBMS_SG_2615_112_2_17_23_9_31_354,"[SG]MBBMS_SGInit %d \n ")
TRACE_MSG(MBBMS_SG_2658_112_2_17_23_9_31_355,"[SG]MBBMS_SGExit .\n ")
TRACE_MSG(MBBMS_SG_2730_112_2_17_23_9_31_356,"[SG]: SendHttpPostRequest  sethttpverb \n")
TRACE_MSG(MBBMS_SG_2743_112_2_17_23_9_31_357,"[SG]: SendHttpPostRequest  to  delete file ,err  = %d\n")
TRACE_MSG(MBBMS_SG_2775_112_2_17_23_9_31_358,"[SG]: SendHttpGetRequest  context_id %d \n")
TRACE_MSG(MBBMS_SG_2814_112_2_17_23_9_31_359,"[SG]: SendHttpGetRequest to  delete file ,err  = %d\n")
TRACE_MSG(MBBMS_SG_2820_112_2_17_23_9_31_360,"[SG]: SendHttpGetRequest  transport_id %d \n")
TRACE_MSG(MBBMS_SG_2840_112_2_17_23_9_31_361,"[SG]:SetPostData  HTTP_PostRequest error  %d\n ")
TRACE_MSG(MBBMS_SG_2871_112_2_17_23_9_31_362,"[SG]: HandleHTTPInitSessionCnf\n")
TRACE_MSG(MBBMS_SG_2924_112_2_17_23_9_31_363,"[SG]: HandleHTTPInitSessionCnf  finish \n")
TRACE_MSG(MBBMS_SG_2935_112_2_17_23_9_31_364,"[SG]: InitHTTPContentInfo   \n")
TRACE_MSG(MBBMS_SG_2947_112_2_17_23_9_31_365,"[SG]: SetHTTPContentInfo  status code %d,err_type %d,err_code %d\n")
TRACE_MSG(MBBMS_SG_2957_112_2_17_23_9_31_366,"[SG]: SetHTTPContentInfo  encoding type %d\n")
TRACE_MSG(MBBMS_SG_2966_112_2_17_23_9_31_367,"[SG]: GetHTTPContentInfo  encoding  type %d\n")
TRACE_MSG(MBBMS_SG_2981_112_2_17_23_9_31_368,"[SG]:HandleHTTPHeaderInd  tick %d \n")
TRACE_MSG(MBBMS_SG_2982_112_2_17_23_9_31_369,"[SG]: HandleHTTPHeaderInd\n")
TRACE_MSG(MBBMS_SG_2998_112_2_17_23_9_31_370,"[SG]: HandleHTTPHeaderInd,receive fail %d\n")
TRACE_MSG(MBBMS_SG_3015_112_2_17_23_9_31_371,"[SG]: HandleHTTPHeaderInd,receive success\n")
TRACE_MSG(MBBMS_SG_3025_112_2_17_23_9_32_372,"[SG]: HandleHTTPHeaderInd,receive  %d\n")
TRACE_MSG(MBBMS_SG_3053_112_2_17_23_9_32_373,"[SG]: GetSGDDInfo\n")
TRACE_MSG(MBBMS_SG_3074_112_2_17_23_9_32_374,"[SG] GetSGDDInfo version:%d,id:%s\n")
TRACE_MSG(MBBMS_SG_3221_112_2_17_23_9_32_375,"[SG]InflateFile  device_name = %x!")
TRACE_MSG(MBBMS_SG_3229_112_2_17_23_9_32_376,"[SG]InflateFile SFS_GetDeviceFreeSpace, no enouth space")
TRACE_MSG(MBBMS_SG_3235_112_2_17_23_9_32_377,"[SG]InflateFile SFS_GetDeviceFreeSpace! error = %d!")
TRACE_MSG(MBBMS_SG_3247_112_2_17_23_9_32_378,"[SG]InflateFile  handle = %x!")
TRACE_MSG(MBBMS_SG_3268_112_2_17_23_9_32_379,"[SG]InflateFile create file fail")
TRACE_MSG(MBBMS_SG_3287_112_2_17_23_9_32_380,"[SG]: CopyContentString, data_len=%d")
TRACE_MSG(MBBMS_SG_3322_112_2_17_23_9_32_381,"[SG] MBBMSSG_DownloadPreviewData! start tick %d\n")
TRACE_MSG(MBBMS_SG_3339_112_2_17_23_9_32_382,"[SG]:MBBMSSG_DownloadPreviewData g_mbbms_task_id %d\n ")
TRACE_MSG(MBBMS_SG_3351_112_2_17_23_9_32_383,"[SG]: MBBMS_FreeBSMSelector")
TRACE_MSG(MBBMS_SG_3373_112_2_17_23_9_32_384,"[SG]: MBBMS_GetBSMSelector\n")
TRACE_MSG(MBBMS_SG_3505_112_2_17_23_9_32_385,"[SG]: SGDDGetStatus, status=%d")
TRACE_MSG(MBBMS_SG_3681_112_2_17_23_9_33_386,"[SG]: SGDDSyntaxHandler Sprd_LinkListCreate\n")
TRACE_MSG(MBBMS_SG_3685_112_2_17_23_9_33_387,"[SG]:SGDDSyntaxHandler BSMSelector\n")
TRACE_MSG(MBBMS_SG_3701_112_2_17_23_9_33_388,"[SG]:SGDDSyntaxHandler id_BSMSelector len=%d,%s\n")
TRACE_MSG(MBBMS_SG_3936_112_2_17_23_9_33_389,"[SG]: SGDDSyntaxHandler, status=%d")
TRACE_MSG(MBBMS_SG_3943_112_2_17_23_9_33_390,"[SG]: SGDDSyntaxHandler, Id_SGDD=%s,len=")
TRACE_MSG(MBBMS_SG_3948_112_2_17_23_9_33_391,"[SG]: SGDDSyntaxHandler, Version_SGDD=%d")
TRACE_MSG(MBBMS_SG_4083_112_2_17_23_9_34_392,"[SG]:SGDDSyntaxHandler Fragment use old fragment \n")
TRACE_MSG(MBBMS_SG_4220_112_2_17_23_9_34_393,"[SG]: GetSyntaxMappedId failed, element=%s,syntax_id=%d, nest=%d, map_size=%d")
TRACE_MSG(MBBMS_SG_4323_112_2_17_23_9_34_394,"[SG]: ServiceFragmentDataHandler, character=%s, len=%d")
TRACE_MSG(MBBMS_SG_4351_112_2_17_23_9_34_395,"[SG]: WriteBlockDataToFile, data_len= %d, src_handle =%d,dst_handle %d")
TRACE_MSG(MBBMS_SG_4376_112_2_17_23_9_34_396,"[SG]: WriteBlockDataToFile,read file err %d")
TRACE_MSG(MBBMS_SG_4389_112_2_17_23_9_34_397,"[SG]: WriteBlockDataToFile,write file err %d")
TRACE_MSG(MBBMS_SG_4433_112_2_17_23_9_34_398,"[SG]:SG_SprdCleanSGDDStructure\n")
TRACE_MSG(MBBMS_SG_4439_112_2_17_23_9_34_399,"[SG]:SG_SprdCleanSGDDStructure  sgdd_ptr->id\n")
TRACE_MSG(MBBMS_SG_4522_112_2_17_23_9_34_400,"[SG]:Esg_ParseFileByHandle() handle%x")
TRACE_MSG(MBBMS_SG_4606_112_2_17_23_9_35_401,"[SG]:Parse error ")
TRACE_MSG(MBBMS_SG_4613_112_2_17_23_9_35_402,"[SG] ParseFileByHandle, alloc mem fail!")
TRACE_MSG(MBBMS_SG_4628_112_2_17_23_9_35_403,"[SG]:Parse CFLXML_FreeParser ")
TRACE_MSG(MBBMS_SG_4637_112_2_17_23_9_35_404,"[SG]:Esg_ParseFileByHandle() handle%x")
TRACE_MSG(MBBMS_SG_4650_112_2_17_23_9_35_405,"[SG]:ParseFileByHandle(): create parser fail")
TRACE_MSG(MBBMS_SG_4669_112_2_17_23_9_35_406,"[SG]:ParseFileByHandle charsread %d,ffs_error %d\n ")
TRACE_MSG(MBBMS_SG_4677_112_2_17_23_9_35_407,"[SG]:Parse error at line %dn %sn")
TRACE_MSG(MBBMS_SG_4703_112_2_17_23_9_35_408,"[SG] IsExistSgdd \n")
TRACE_MSG(MBBMS_SG_4707_112_2_17_23_9_35_409,"[SG] IsExistSgdd, no initial! \n")
TRACE_MSG(MBBMS_SG_4723_112_2_17_23_9_35_410,"[SG] IsExistSgdd is exist %d\n")
TRACE_MSG(MBBMS_SG_4769_112_2_17_23_9_35_411,"[SG] WriteDataToFile file type %d, data_len 0x%x \n")
TRACE_MSG(MBBMS_SG_4784_112_2_17_23_9_35_412,"[SG] WriteDataToFile fail to create file\n")
TRACE_MSG(MBBMS_SG_4789_112_2_17_23_9_35_413,"[SG] WriteDataToFile write data method (1) %d\n")
TRACE_MSG(MBBMS_SG_4843_112_2_17_23_9_35_414,"[SG] WriteDataToFile write data method (1) %d\n")
TRACE_MSG(MBBMS_SG_4853_112_2_17_23_9_35_415,"[SG] WriteDataToFile fail to create file\n")
TRACE_MSG(MBBMS_SG_4866_112_2_17_23_9_35_416,"[SG] WriteDataToFile write data method (2) %d\n")
TRACE_MSG(MBBMS_SG_4901_112_2_17_23_9_35_417,"[SG] SplitFile is_exist_sgdd=%d, is_gzip = %d\n")
TRACE_MSG(MBBMS_SG_4911_112_2_17_23_9_35_418,"[SG] SplitFile delete decomp file ret =%d \n")
TRACE_MSG(MBBMS_SG_4924_112_2_17_23_9_35_419,"[SG] SplitFile rename decomp file ret =%d \n")
TRACE_MSG(MBBMS_SG_4946_112_2_17_23_9_35_420,"[SG] SpiteFile file size 0x%x \n")
TRACE_MSG(MBBMS_SG_4951_112_2_17_23_9_35_421,"[SG] SpiteFile file size 0x%x,data len 0x%x \n")
TRACE_MSG(MBBMS_SG_4977_112_2_17_23_9_35_422,"[SG] SpliteFile read  len 0x%x\n")
TRACE_MSG(MBBMS_SG_4980_112_2_17_23_9_35_423,"[SG] SpiteFile offset 0x%x,data_ptr 0x%x\n")
TRACE_MSG(MBBMS_SG_5011_112_2_17_23_9_35_424,"[SG] WriteDataToFile fail to write sgdd\n")
TRACE_MSG(MBBMS_SG_5025_112_2_17_23_9_36_425,"[SG] WriteDataToFile fail to write sgdu\n")
TRACE_MSG(MBBMS_SG_5034_112_2_17_23_9_36_426,"[SG] WriteDataToFile fail to write sgdu bcast\n")
TRACE_MSG(MBBMS_SG_5074_112_2_17_23_9_36_427,"[SG] ParseSGDD \n")
TRACE_MSG(MBBMS_SG_5082_112_2_17_23_9_36_428,"[SG] ParseSGDD fail to open fail\n")
TRACE_MSG(MBBMS_SG_5107_112_2_17_23_9_36_429,"[SG] ParseSGDD fail to parse and delete sgdd sfs_error 0x%x \n")
TRACE_MSG(MBBMS_SG_5113_112_2_17_23_9_36_430,"[SG] ParseSGDD finish ret=%d\n")
TRACE_MSG(MBBMS_SG_5322_112_2_17_23_9_36_431,"[SG] UpdateFragmentFile   transport_id %d\n")
TRACE_MSG(MBBMS_SG_5358_112_2_17_23_9_36_432,"[SG] DeleteFragmentFileFromIndexFile  \n")
TRACE_MSG(MBBMS_SG_5371_112_2_17_23_9_36_433,"[SG] DeleteFragmentFileFromIndexFile  \n")
TRACE_MSG(MBBMS_SG_5395_112_2_17_23_9_36_434,"[SG] DeleteFragmentFileFromIndexFile  fail to alloc mem\n")
TRACE_MSG(MBBMS_SG_5406_112_2_17_23_9_36_435,"[SG] DeleteFragmentFileFromIndexFile  fail to alloc mem\n")
TRACE_MSG(MBBMS_SG_5574_112_2_17_23_9_37_436,"[SG] DeletePreviewDataFile  transport_id %d \n")
TRACE_MSG(MBBMS_SG_5584_112_2_17_23_9_37_437,"[SG] DeletePreviewDataFile fail to delete picture file %d \n")
TRACE_MSG(MBBMS_SG_5664_112_2_17_23_9_37_438,"[SG] DeleteFragmentFile  transport_id %d\n")
TRACE_MSG(MBBMS_SG_5673_112_2_17_23_9_37_439,"[SG] DeleteFragmentFile fail to delete fragment file \n")
TRACE_MSG(MBBMS_SG_5739_112_2_17_23_9_37_440,"[SG] UpdateSGDDFile is_same %d\n")
TRACE_MSG(MBBMS_SG_5754_112_2_17_23_9_37_441,"[SG] UpdateSGDDFile :fail to delete sgdd,sfs_err1 %d,\n")
TRACE_MSG(MBBMS_SG_5768_112_2_17_23_9_37_442,"[SG] UpdateSGDDFile :fail to rename ,sfs_err1%d\n")
TRACE_MSG(MBBMS_SG_5781_112_2_17_23_9_37_443,"[SG] UpdateSGDDFile! same:fail to delete:,sfs_err1\n")
TRACE_MSG(MBBMS_SG_5823_112_2_17_23_9_37_444,"[SG]: AddDeltaSGDU enter  tickcount(1)%d")
TRACE_MSG(MBBMS_SG_5845_112_2_17_23_9_37_445,"[SG]: AddDeltaSGDU open sgdu file error")
TRACE_MSG(MBBMS_SG_5855_112_2_17_23_9_37_446,"[SG]: AddDeltaSGDU get sgdu file size error")
TRACE_MSG(MBBMS_SG_5864_112_2_17_23_9_37_447,"[SG]: AddDeltaSGDU get sgdu header error")
TRACE_MSG(MBBMS_SG_5920_112_2_17_23_9_37_448,"[SG]: AddDeltaSGDU read   frag id %d, err %d\n")
TRACE_MSG(MBBMS_SG_5924_112_2_17_23_9_37_449,"[SG]: AddDeltaSGDU get desp info error")
TRACE_MSG(MBBMS_SG_5940_112_2_17_23_9_37_450,"[SG]: AddDeltaSGDU get type error")
TRACE_MSG(MBBMS_SG_5949_112_2_17_23_9_37_451,"[SG]: AddDeltaSGDU update fragment transport id %d,act %d")
TRACE_MSG(MBBMS_SG_5957_112_2_17_23_9_37_452,"[SG]: AddDeltaSGDU  wrong despinfo_len")
TRACE_MSG(MBBMS_SG_5968_112_2_17_23_9_37_453,"[SG]: AddDeltaSGDU write  frag id %d, err %d\n")
TRACE_MSG(MBBMS_SG_5973_112_2_17_23_9_37_454,"[SG]: AddDeltaSGDU write desp info error")
TRACE_MSG(MBBMS_SG_5990_112_2_17_23_9_37_455,"[SG]: AddDeltaSGDU write fragment index error")
TRACE_MSG(MBBMS_SG_6001_112_2_17_23_9_37_456,"[SG]: AddDeltaSGDU unknow encoding type =%d")
TRACE_MSG(MBBMS_SG_6028_112_2_17_23_9_38_457,"[SG]: AddDeltaSGDU end tick(2) %d")
TRACE_MSG(MBBMS_SG_6120_112_2_17_23_9_38_458,"[SG]: UpdateSGDUFile write desp begin tag error")
TRACE_MSG(MBBMS_SG_6152_112_2_17_23_9_38_459,"[SG]: UpdateSGDUFile write desp begin tag error")
TRACE_MSG(MBBMS_SG_6182_112_2_17_23_9_38_460,"[SG]: UpdateSGDUFile write desp begin tag error")
TRACE_MSG(MBBMS_SG_6232_112_2_17_23_9_38_461,"[SG]: UpdateSGDUFile frag id %d, is_exist %d ,act %d")
TRACE_MSG(MBBMS_SG_6288_112_2_17_23_9_38_462,"[SG]: UpdateSGDUFile fail to set point in sgdu ")
TRACE_MSG(MBBMS_SG_6298_112_2_17_23_9_38_463,"[SG]: UpdateSGDUFile read old  frag id %d, err %d\n")
TRACE_MSG(MBBMS_SG_6301_112_2_17_23_9_38_464,"[SG]: UpdateSGDUFile read sgdu error")
TRACE_MSG(MBBMS_SG_6309_112_2_17_23_9_38_465,"[SG]: UpdateSGDUFile write sgdu error")
TRACE_MSG(MBBMS_SG_6324_112_2_17_23_9_38_466,"[SG]: UpdateSGDUFile write old  frag id %d, err %d\n")
TRACE_MSG(MBBMS_SG_6327_112_2_17_23_9_38_467,"[SG]: UpdateSGDUFile write fragment index error")
TRACE_MSG(MBBMS_SG_6364_112_2_17_23_9_38_468,"[SG]: UpdateSGDUFile write sgdu error")
TRACE_MSG(MBBMS_SG_6382_112_2_17_23_9_38_469,"[SG]: UpdateSGDUFile fail to delete sgdu ")
TRACE_MSG(MBBMS_SG_6394_112_2_17_23_9_38_470,"[SG]: UpdateSGDUFile fail to rename sgdu err")
TRACE_MSG(MBBMS_SG_6519_112_2_17_23_9_39_471,"[SG] CompareSGDD  update transport id \n")
TRACE_MSG(MBBMS_SG_6580_112_2_17_23_9_39_472,"[SG] CompareSGDD  fail to delete old frag index\n")
TRACE_MSG(MBBMS_SG_6591_112_2_17_23_9_39_473,"[SG] CompareSGDD  fail to rename frag index\n")
TRACE_MSG(MBBMS_SG_6617_112_2_17_23_9_39_474,"[SG] HandleAllocFail! \n")
TRACE_MSG(MBBMS_SG_6649_112_2_17_23_9_39_475,"[SG] UpdateAllSG  CreateXMLSGDUFile status=%d\n")
TRACE_MSG(MBBMS_SG_6652_112_2_17_23_9_39_476,"[SG] ParseHTTPContentfile  CreateXMLSGDUFile status=%d\n")
TRACE_MSG(MBBMS_SG_6658_112_2_17_23_9_39_477,"[SG] UpdateAllSG  failt to create sgdd file\n")
TRACE_MSG(MBBMS_SG_6666_112_2_17_23_9_39_478,"[SG] UpdateAllSG  receive success\n")
TRACE_MSG(MBBMS_SG_6679_112_2_17_23_9_39_479,"[SG] ParseHTTPContentfile  CreateXMLSGDUFile status=%d\n")
TRACE_MSG(MBBMS_SG_6686_112_2_17_23_9_39_480,"[SG] UpdateAllSG  imcomplete data\n")
TRACE_MSG(MBBMS_SG_6740_112_2_17_23_9_39_481,"[SG] ParseAndCompare! \n")
TRACE_MSG(MBBMS_SG_6756_112_2_17_23_9_39_482,"[SG] ParseAndCompare! fail to parse new sgdd\n")
TRACE_MSG(MBBMS_SG_6803_112_2_17_23_9_39_483,"[SG] ParseAndCompare! fail to parse old sgdd\n")
TRACE_MSG(MBBMS_SG_6839_112_2_17_23_9_39_484,"[SG] ParseAndCompare! they are diffrent \n")
TRACE_MSG(MBBMS_SG_6843_112_2_17_23_9_39_485,"[SG] ParseAndCompare! they are same\n")
TRACE_MSG(MBBMS_SG_6893_112_2_17_23_9_39_486,"[SG] RenameTempFile!diffrent:fail to delete old file sfs_err %d\n")
TRACE_MSG(MBBMS_SG_6903_112_2_17_23_9_39_487,"[SG] RenameTempFile!diffrent:fail to rename sfs_err1%d\n")
TRACE_MSG(MBBMS_SG_7011_112_2_17_23_9_39_488,"[SG]: DeleteALLFragmentFile sfs_err %d\n")
TRACE_MSG(MBBMS_SG_7053_112_2_17_23_9_40_489,"[SG]: DeleteALLFragmentFile \n")
TRACE_MSG(MBBMS_SG_7075_112_2_17_23_9_40_490,"[SG]: DeleteALLFragmentFile fail to alloc mem \n")
TRACE_MSG(MBBMS_SG_7099_112_2_17_23_9_40_491,"[SG]: DeleteALLFragmentFile sfs_err %d\n")
TRACE_MSG(MBBMS_SG_7141_112_2_17_23_9_40_492,"[SG] DeleteALLFragmentFile!:fail to delete index sfs_err %d \n")
TRACE_MSG(MBBMS_SG_7160_112_2_17_23_9_40_493,"[SG] DeleteAllSGFile \n")
TRACE_MSG(MBBMS_SG_7171_112_2_17_23_9_40_494,"[SG] DeleteAllSGFile!:fail to delete sfs_err %d,sfs_err1 %d,\n")
TRACE_MSG(MBBMS_SG_7200_112_2_17_23_9_40_495,"[SG]: IsCompleteFragmentIndexFile enter")
TRACE_MSG(MBBMS_SG_7205_112_2_17_23_9_40_496,"[SG]: IsCompleteFragmentIndexFile handle=%d")
TRACE_MSG(MBBMS_SG_7220_112_2_17_23_9_40_497,"[SG]: IsCompleteFragmentIndexFile is_imcomplete =%d")
TRACE_MSG(MBBMS_SG_7250_112_2_17_23_9_40_498,"[SG] ParseHTTPContentfile! enter tick %d ,is_success %d \n")
TRACE_MSG(MBBMS_SG_7291_112_2_17_23_9_40_499,"[SG] ParseHTTPContentfile ParseAndCompare is ok\n")
TRACE_MSG(MBBMS_SG_7298_112_2_17_23_9_40_500,"[SG] ParseHTTPContentfile fail to ParseAndCompare! \n")
TRACE_MSG(MBBMS_SG_7322_112_2_17_23_9_40_501,"[SG] ParseHTTPContentfile  CreateXMLSGDUFile status=%d\n")
TRACE_MSG(MBBMS_SG_7335_112_2_17_23_9_40_502,"[SG] ParseHTTPContentfile  receive success\n")
TRACE_MSG(MBBMS_SG_7360_112_2_17_23_9_40_503,"[SG] ParseHTTPContentfile  imcomplete data\n")
TRACE_MSG(MBBMS_SG_7395_112_2_17_23_9_40_504,"[SG] ParseHTTPContentfile!type %d, end tick %d\n")
TRACE_MSG(MBBMS_SG_7415_112_2_17_23_9_40_505,"[SG]: GetSGDDStatusCode\n")
TRACE_MSG(MBBMS_SG_7436_112_2_17_23_9_40_506,"[SG] GetSGDDStatusCode status:%d\n")
TRACE_MSG(MBBMS_SG_7470_112_2_17_23_9_40_507,"[SG] SetStatusCode! enter tick %d ,is_success %d \n")
TRACE_MSG(MBBMS_SG_7494_112_2_17_23_9_40_508,"[SG] SetStatusCode fail to get File\n")
TRACE_MSG(MBBMS_SG_7506_112_2_17_23_9_40_509,"[SG] SetStatusCode fail to get format \n")
TRACE_MSG(MBBMS_SG_7518_112_2_17_23_9_41_510,"[SG] SetStatusCode! begin inflate \n")
TRACE_MSG(MBBMS_SG_7523_112_2_17_23_9_41_511,"[SG] SetStatusCode inflate_retf %d\n")
TRACE_MSG(MBBMS_SG_7531_112_2_17_23_9_41_512,"[SG] SetStatusCode fail to Inflate File\n")
TRACE_MSG(MBBMS_SG_7540_112_2_17_23_9_41_513,"[SG] SetStatusCode! non-gzip \n")
TRACE_MSG(MBBMS_SG_7581_112_2_17_23_9_41_514,"[SG] SetStatusCode status:%d\n")
TRACE_MSG(MBBMS_SG_7606_112_2_17_23_9_41_515,"[SG]:HandleHTTPDataCnf  tick %d \n")
TRACE_MSG(MBBMS_SG_7609_112_2_17_23_9_41_516,"[SG]:HandleHTTPDataCnf  g_mbbms_stop_flag %d ,data len %d, is_success %dn")
TRACE_MSG(MBBMS_SG_7646_112_2_17_23_9_41_517,"[SG] HandleHTTPDataCnf   sgdd same with old sgdd\n")
TRACE_MSG(MBBMS_SG_7661_112_2_17_23_9_41_518,"[SG]:HandleHTTPDataCnf  HTTP_CloseRequest error %d \n")
TRACE_MSG(MBBMS_SG_7681_112_2_17_23_9_41_519,"[SG]:HandleStopSearchSGSig\n")
TRACE_MSG(MBBMS_SG_7703_112_2_17_23_9_41_520,"[SG]:HandleStopSearchSGSig  HTTP_CloseRequest error %d \n")
TRACE_MSG(MBBMS_SG_7737_112_2_17_23_9_41_521,"[SG]:HandleHTTPErrInd %d\n")
TRACE_MSG(MBBMS_SG_7781_112_2_17_23_9_41_522,"[SG]:HandleHTTPDataCnf  HTTP_CloseRequest error %d \n")
TRACE_MSG(MBBMS_SG_7850_112_2_17_23_9_41_523,"[SG]:SetPostData  s_sgdd_manual_search_flag %d \n ")
TRACE_MSG(MBBMS_SG_7874_112_2_17_23_9_41_524,"[SG]:SetPostData  no sgdd  \n ")
TRACE_MSG(MBBMS_SG_7884_112_2_17_23_9_41_525,"[SG]:SetPostData  broadcastaccess = %s\n ")
TRACE_MSG(MBBMS_SG_7895_112_2_17_23_9_41_526,"[SG]:SetPostData  mcc %d ,mnc %d,lac 0x%x, cell id 0x%x \n ")
TRACE_MSG(MBBMS_SG_7899_112_2_17_23_9_41_527,"[SG]:SetPostData  digit_1 %d ,digit_2 %d,digit_3 %d \n ")
TRACE_MSG(MBBMS_SG_7906_112_2_17_23_9_41_528,"[SG]:SetPostData  digit_1 %d ,digit_2 %d,digit_3 %d \n ")
TRACE_MSG(MBBMS_SG_7910_112_2_17_23_9_41_529,"[SG]:SetPostData  byte_1 %d ,byte_2 %d,byte_3 %d \n ")
TRACE_MSG(MBBMS_SG_7921_112_2_17_23_9_41_530,"[SG]:SetPostData  bsdcell = %s\n ")
TRACE_MSG(MBBMS_SG_7971_112_2_17_23_9_41_531,"[SG]:SetPostData  post data len %d\n ")
TRACE_MSG(MBBMS_SG_7972_112_2_17_23_9_41_532,"[SG]:SetPostData  post data  %s\n ")
TRACE_MSG(MBBMS_SG_7980_112_2_17_23_9_41_533,"[SG]:SetPostData  post data  end\n ")
TRACE_MSG(MBBMS_SG_8026_112_2_17_23_9_42_534,"[SG]:SetPostData  HTTP_PostRequest error  %d\n ")
TRACE_MSG(MBBMS_SG_8032_112_2_17_23_9_42_535,"[SG] SetPostData! end tick %d\n")
TRACE_MSG(MBBMS_SG_8045_112_2_17_23_9_42_536,"[SG]:SendSignalToMMI  info.type %d\n ")
TRACE_MSG(MBBMS_SG_8113_112_2_17_23_9_42_537,"[SG] CreateMBBMS3SubFolder: create sub  dir success %c\n")
TRACE_MSG(MBBMS_SG_8118_112_2_17_23_9_42_538,"[SG] CreateMBBMS3SubFolder: fail to create dir %c \n")
TRACE_MSG(MBBMS_SG_8196_112_2_17_23_9_42_539,"[SG] CreateFolder: create dir success\n")
TRACE_MSG(MBBMS_SG_8201_112_2_17_23_9_42_540,"[SG] CreateFolder: fail to create dir \n")
TRACE_MSG(MBBMS_SG_8227_112_2_17_23_9_42_541,"[SG]:MBBMS_ManualSearchServiceGuide\n ")
TRACE_MSG(MBBMS_SG_8262_112_2_17_23_9_42_542,"[SG]:MBBMS_SearchServiceGuide\n ")
TRACE_MSG(MBBMS_SG_8274_112_2_17_23_9_42_543,"[SG]:MBBMS_SearchServiceGuide g_mbbms_task_id %d\n ")
TRACE_MSG(MBBMS_SG_8298_112_2_17_23_9_42_544,"[SG] MBBMSSG_SearchServiceGuide! start tick %d\n")
TRACE_MSG(MBBMS_SG_8335_112_2_17_23_9_42_545,"[SG]:MBBMS_SearchServiceGuide\n ")
TRACE_MSG(MBBMS_SG_8346_112_2_17_23_9_42_546,"[SG]:MBBMS_SearchServiceGuide g_mbbms_task_id %d\n ")
TRACE_MSG(MBBMS_SG_8361_112_2_17_23_9_42_547,"[SG]:MBBMS_StopSearchingSG\n ")
TRACE_MSG(MBBMS_SG_8395_112_2_17_23_9_42_548,"[SG]:MBBMSSG_StopSearchingSG END \n")
TRACE_MSG(MBBMS_SG_8406_112_2_17_23_9_42_549,"[SG]:MBBMS_SetSGURI %d[%s]\n ")
TRACE_MSG(MBBMS_SG_8467_112_2_17_23_9_42_550,"[SG]:MBBMS_GetSGDUFile\n ")
TRACE_MSG(MBBMS_SG_8519_112_2_17_23_9_43_551,"[SG]:MBBMS_GetSGDUBCASTFile\n ")
TRACE_MSG(MBBMS_SG_8550_112_2_17_23_9_43_552,"[SG]: MBBMSSG_GetAdFilm\n")
TRACE_MSG(MBBMS_SG_8598_112_2_17_23_9_43_553,"[SG] MBBMSSG_GetAdFilm string_len:%d \n")
TRACE_MSG(MBBMS_SG_8618_112_2_17_23_9_43_554,"[SG]:MBBMS_SGSetWap wap=%s,port 0x%x\n ")
TRACE_MSG(MBBMS_SG_8668_112_2_17_23_9_43_555,"[SG]:MBBMSSG_SetSGAreaName name_len 0x%x ,name_ptr 0x%x\n ")
TRACE_MSG(MBBMS_SG_8708_112_2_17_23_9_43_556,"[SG]:0x%x\n ")
TRACE_MSG(MBBMS_SG_8837_112_2_17_23_9_43_557,"[SG]:MBBMS_TestParse\n ")
TRACE_MSG(MBBMS_SG_9088_112_2_17_23_9_44_558,"[SG]: GetSgduHeader read header error")
TRACE_MSG(MBBMS_SG_9107_112_2_17_23_9_44_559,"[SG]: GetSgduHeader read desp error")
TRACE_MSG(MBBMS_SG_9142_112_2_17_23_9_44_560,"[SG]: WriteFileNameToIndexFile open fragment_idx_handle file error")
TRACE_MSG(MBBMS_SG_9153_112_2_17_23_9_44_561,"[SG]: WriteFileNameToIndexFile write fragment index  error")
TRACE_MSG(MBBMS_SG_9206_112_2_17_23_9_44_562,"[SG]: OpenIndexFileandOffset fragment_idx_handle error")
TRACE_MSG(MBBMS_SG_9216_112_2_17_23_9_44_563,"[SG]: OpenIndexFileandOffset set pointer  error")
TRACE_MSG(MBBMS_SG_9241_112_2_17_23_9_44_564,"[SG]: OpenIndexFileandOffset set pointer  error")
TRACE_MSG(MBBMS_SG_9284_112_2_17_23_9_44_565,"[SG]: UpdateFragment enter tick %d")
TRACE_MSG(MBBMS_SG_9299_112_2_17_23_9_44_566,"[SG]: UpdateFragment open sgdu file error")
TRACE_MSG(MBBMS_SG_9306_112_2_17_23_9_44_567,"[SG]: UpdateFragment get sgdu file size error")
TRACE_MSG(MBBMS_SG_9312_112_2_17_23_9_44_568,"[SG]: UpdateFragment get sgdu file size error")
TRACE_MSG(MBBMS_SG_9350_112_2_17_23_9_44_569,"[SG]: UpdateFragment read_offset %d")
TRACE_MSG(MBBMS_SG_9357_112_2_17_23_9_44_570,"[SG]: UpdateFragment fail to set pointer")
TRACE_MSG(MBBMS_SG_9386_112_2_17_23_9_44_571,"[SG]: UpdateFragment read sgdu error")
TRACE_MSG(MBBMS_SG_9393_112_2_17_23_9_44_572,"[SG]: UpdateFragment unknow type")
TRACE_MSG(MBBMS_SG_9401_112_2_17_23_9_44_573,"[SG]: UpdateFragment  wrong despinfo_len")
TRACE_MSG(MBBMS_SG_9417_112_2_17_23_9_44_574,"[SG]: UpdateFragment write desp info error")
TRACE_MSG(MBBMS_SG_9438_112_2_17_23_9_44_575,"[SG]: UpdateFragment write fragment index error")
TRACE_MSG(MBBMS_SG_9482_112_2_17_23_9_44_576,"[SG]: UpdateFragment end tick %d")
TRACE_MSG(MBBMS_SG_9534_112_2_17_23_9_45_577,"[SG]: CreateNewFragmentFile open sgdu file error")
TRACE_MSG(MBBMS_SG_9541_112_2_17_23_9_45_578,"[SG]: CreateNewFragmentFile get sgdu file size error")
TRACE_MSG(MBBMS_SG_9547_112_2_17_23_9_45_579,"[SG]: CreateNewFragmentFile get sgdu file size error")
TRACE_MSG(MBBMS_SG_9598_112_2_17_23_9_45_580,"[SG]: CreateNewFragmentFile write desp info error")
TRACE_MSG(MBBMS_SG_9627_112_2_17_23_9_45_581,"[SG]: CreateNewFragmentFile read sgdu error")
TRACE_MSG(MBBMS_SG_9634_112_2_17_23_9_45_582,"[SG]: CreateNewFragmentFile unknow type")
TRACE_MSG(MBBMS_SG_9646_112_2_17_23_9_45_583,"[SG]: CreateNewFragmentFile open fragment_handle file error")
TRACE_MSG(MBBMS_SG_9652_112_2_17_23_9_45_584,"[SG]: CreateNewFragmentFile  wrong despinfo_len")
TRACE_MSG(MBBMS_SG_9666_112_2_17_23_9_45_585,"[SG]: CreateFragmentFile write desp info error")
TRACE_MSG(MBBMS_SG_9825_112_2_17_23_9_45_586,"[SG]: WriteHeaderToIndexFile set pointer  error")
TRACE_MSG(MBBMS_SG_9850_112_2_17_23_9_45_587,"[SG]: WriteHeaderToIndexFile write file  error")
TRACE_MSG(MBBMS_SG_9903_112_2_17_23_9_45_588,"[SG]: DeleteAllPreviewDataFile \n")
TRACE_MSG(MBBMS_SG_9976_112_2_17_23_9_45_589,"[SG]: DeleteALLFragmentFile sfs_err %d\n")
TRACE_MSG(MBBMS_SG_10008_112_2_17_23_9_46_590,"[SG]: DeleteSGDUDataFile \n")
TRACE_MSG(MBBMS_SG_10019_112_2_17_23_9_46_591,"[SG]: DeleteSGDUDataFile fail  to delete index file err %d")
TRACE_MSG(MBBMS_SG_10032_112_2_17_23_9_46_592,"[SG]: DeleteSGDUDataFile fail  to delete sgdu file err %d")
TRACE_MSG(MBBMS_SG_10070_112_2_17_23_9_46_593,"[SG]: CreateXMLSGDUFile  tick(1) %d")
TRACE_MSG(MBBMS_SG_10089_112_2_17_23_9_46_594,"[SG]: CreateXMLSGDUFile open sgdu file error")
TRACE_MSG(MBBMS_SG_10099_112_2_17_23_9_46_595,"[SG]: CreateXMLSGDUFile get sgdu file size error")
TRACE_MSG(MBBMS_SG_10111_112_2_17_23_9_46_596,"[SG]: CreateXMLSGDUFile create bcast file error")
TRACE_MSG(MBBMS_SG_10121_112_2_17_23_9_46_597,"[SG]: CreateXMLSGDUFile write desp begin tag error")
TRACE_MSG(MBBMS_SG_10128_112_2_17_23_9_46_598,"[SG]: CreateXMLSGDUFile get header error")
TRACE_MSG(MBBMS_SG_10144_112_2_17_23_9_46_599,"[SG]: CreateXMLSGDUFile sgdd error")
TRACE_MSG(MBBMS_SG_10169_112_2_17_23_9_46_600,"[SG]: CreateXMLSGDUFile open index file error")
TRACE_MSG(MBBMS_SG_10211_112_2_17_23_9_46_601,"[SG]: CreateXMLSGDUFile get desp info error")
TRACE_MSG(MBBMS_SG_10221_112_2_17_23_9_46_602,"[SG]: CreateXMLSGDUFile get type error")
TRACE_MSG(MBBMS_SG_10249_112_2_17_23_9_46_603,"[SG]: CreateXMLSGDUFile write fragment error")
TRACE_MSG(MBBMS_SG_10264_112_2_17_23_9_46_604,"[SG]: CreateXMLSGDUFile write fragment index error")
TRACE_MSG(MBBMS_SG_10272_112_2_17_23_9_46_605,"[SG]: CreateXMLSGDUFile unknow encoding type =%d")
TRACE_MSG(MBBMS_SG_10291_112_2_17_23_9_46_606,"[SG]: CreateXMLSGDUFile write desp end tag error")
TRACE_MSG(MBBMS_SG_10314_112_2_17_23_9_46_607,"[SG]: CreateXMLSGDUFile write header to index error")
TRACE_MSG(MBBMS_SG_10329_112_2_17_23_9_46_608,"[SG]: CreateXMLSGDUFile end tick(2) %d")
TRACE_MSG(MBBMS_SG_10359_112_2_17_23_9_46_609,"[SG]: CreateAllFragmentFile enter exist sgdd %d")
TRACE_MSG(MBBMS_SG_10380_112_2_17_23_9_46_610,"[SG]: CreateAllFragmentFile open sgdu file error")
TRACE_MSG(MBBMS_SG_10390_112_2_17_23_9_46_611,"[SG]: CreateAllFragmentFile get sgdu file size error")
TRACE_MSG(MBBMS_SG_10399_112_2_17_23_9_46_612,"[SG]: CreateAllFragmentFile get sgdu file size error")
TRACE_MSG(MBBMS_SG_10413_112_2_17_23_9_46_613,"[SG]: CreateAllFragmentFile sgdd error")
TRACE_MSG(MBBMS_SG_10433_112_2_17_23_9_46_614,"[SG]: CreateAllFragmentFile open index file error")
TRACE_MSG(MBBMS_SG_10487_112_2_17_23_9_47_615,"[SG]: CreateAllFragmentFile get desp info error")
TRACE_MSG(MBBMS_SG_10503_112_2_17_23_9_47_616,"[SG]: CreateAllFragmentFile get type error")
TRACE_MSG(MBBMS_SG_10517_112_2_17_23_9_47_617,"[SG]: CreateAllFragmentFile open fragment_handle file error")
TRACE_MSG(MBBMS_SG_10523_112_2_17_23_9_47_618,"[SG]: CreateAllFragmentFile  wrong despinfo_len")
TRACE_MSG(MBBMS_SG_10543_112_2_17_23_9_47_619,"[SG]: CreateAllFragmentFile write desp info error")
TRACE_MSG(MBBMS_SG_10555_112_2_17_23_9_47_620,"[SG]: CreateAllFragmentFile write fragment index error")
TRACE_MSG(MBBMS_SG_10570_112_2_17_23_9_47_621,"[SG]: CreateAllFragmentFile unknow encoding type =%d")
TRACE_MSG(MBBMS_SG_10588_112_2_17_23_9_47_622,"[SG]: CreateAllFragmentFile write header to index error")
TRACE_MSG(MBBMS_SG_10614_112_2_17_23_9_47_623,"[SG]: CreateAllFragmentFile end")
TRACE_MSG(MBBMS_SG_10695_112_2_17_23_9_47_624,"[SG]: MBBMS_GetFileID fname len=%d")
TRACE_MSG(MBBMS_SG_10717_112_2_17_23_9_47_625,"[SG]: MBBMS_GetFileID convert fname fail")
TRACE_MSG(MBBMS_SG_10729_112_2_17_23_9_47_626,"[SG]: MBBMS_GetFileID fname invalid")
TRACE_MSG(MBBMS_SG_10731_112_2_17_23_9_47_627,"[SG]: MBBMS_GetFileID file id =%d")
TRACE_MSG(MBBMS_SG_10775_112_2_17_23_9_47_628,"[SG]: MBBMS_GetIdxFileData enter")
TRACE_MSG(MBBMS_SG_10782_112_2_17_23_9_47_629,"[SG]: MBBMS_GetIdxFileData handle=%d")
TRACE_MSG(MBBMS_SG_10789_112_2_17_23_9_47_630,"[SG]: MBBMS_GetIdxFileData read=%d sfs err=%d valid=%d")
TRACE_MSG(MBBMS_SG_10794_112_2_17_23_9_47_631,"[SG]: MBBMS_GetIdxFileData data_len=%d")
TRACE_MSG(MBBMS_SG_10799_112_2_17_23_9_47_632,"[SG]: MBBMS_GetIdxFileData ptr=0x%x")
TRACE_MSG(MBBMS_SG_10811_112_2_17_23_9_47_633,"[SG]: MBBMS_GetIdxFileData get data error!")
TRACE_MSG(MBBMS_SG_10821_112_2_17_23_9_47_634,"[SG]: MBBMS_GetIdxFileData SFS_GetFileSize error!")
TRACE_MSG(MBBMS_SG_10827_112_2_17_23_9_47_635,"[SG]: MBBMS_GetIdxFileData data ptr=0x%x len=%d")
TRACE_MSG(MBBMS_SG_10848_112_2_17_23_9_47_636,"[SG]: IsValidFragmentIndexFile enter")
TRACE_MSG(MBBMS_SG_10851_112_2_17_23_9_47_637,"[SG]: IsValidFragmentIndexFile handle=%d")
TRACE_MSG(MBBMS_SG_10866_112_2_17_23_9_47_638,"[SG]: IsValidFragmentIndexFile valid =%d")
TRACE_MSG(MBBMS_SG_10885_112_2_17_23_9_47_639,"[SG]: MBBMSSG_GetSGDDId")
TRACE_MSG(MBBMS_SG_10913_112_2_17_23_9_47_640,"[SG]: MBBMS_GetFullPath enter")
TRACE_MSG(MBBMS_SG_10952_112_2_17_23_9_47_641,"[SG]: CheckHttpWord data_len =%d, data =%s")
TRACE_MSG(MBBMS_SG_10998_112_2_17_23_9_48_642,"[SG]: CheckHttpWord string =%s ,data_len %d")
TRACE_MSG(MBBMS_SG_11059_112_2_17_23_9_48_643,"[SG]: CheckHttpWord string =%s ,data_len %d")
TRACE_MSG(MBBMS_SG_API_158_112_2_17_23_9_48_644,"[SGAPI]GetList: type = %d\n")
TRACE_MSG(MBBMS_SG_API_224_112_2_17_23_9_48_645,"[SGAPI]GetList: list_ptr = 0x%x\n")
TRACE_MSG(MBBMS_SG_API_257_112_2_17_23_9_48_646,"[SGAPI]:GetServiceForSearching condition=0x%x,serviceIdRef %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_307_112_2_17_23_9_48_647,"[SGAPI]:GetContentForSearching keywordCRC32 %d\n")
TRACE_MSG(MBBMS_SG_API_341_112_2_17_23_9_48_648,"[SGAPI]GetServiceForSearching: rt_list_ptr = 0x%x, total_count %d\n")
TRACE_MSG(MBBMS_SG_API_359_112_2_17_23_9_49_649,"[SGAPI]:SetSearchInfo  query = %d n")
TRACE_MSG(MBBMS_SG_API_409_112_2_17_23_9_49_650,"[SGAPI]:ParseFragment type =%d \n")
TRACE_MSG(MBBMS_SG_API_415_112_2_17_23_9_49_651,"[SGAPI]:ParseFragment parse error \n")
TRACE_MSG(MBBMS_SG_API_427_112_2_17_23_9_49_652,"[SGAPI]:ParseFragment id =%s \n")
TRACE_MSG(MBBMS_SG_API_440_112_2_17_23_9_49_653,"[SGAPI]:ParseFragment id =%s \n")
TRACE_MSG(MBBMS_SG_API_496_112_2_17_23_9_49_654,"[SGAPI]:GetContentFragment   id %s,len=%d,pos=%d n")
TRACE_MSG(MBBMS_SG_API_562_112_2_17_23_9_49_655,"[SGAPI]:IsSaveContent serviceIdRef %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_597_112_2_17_23_9_49_656,"[SGAPI]:IsSaveContent keyword is_find_keyword %d \n")
TRACE_MSG(MBBMS_SG_API_625_112_2_17_23_9_49_657,"[SGAPI]:IsSaveContent name is_find_keyword %d \n")
TRACE_MSG(MBBMS_SG_API_647_112_2_17_23_9_49_658,"[SGAPI]:IsSaveContent serviceIdRef %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_675_112_2_17_23_9_49_659,"[SGAPI]:IsSaveContent serviceIdRef %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_703_112_2_17_23_9_49_660,"[SGAPI]:CleanExtentionList  list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_709_112_2_17_23_9_49_661,"[SGAPI]:CleanExtentionList  extension_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_745_112_2_17_23_9_49_662,"[SGAPI]:CleanPreviewDataList  list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_752_112_2_17_23_9_49_663,"[SGAPI]:CleanPreviewDataList  ref_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_783_112_2_17_23_9_49_664,"[SGAPI]:FreePurchaseDataFragment  data_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_821_112_2_17_23_9_49_665,"[SGAPI]:FreeOtherInfoOfPurchaseItemFragment  item_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_849_112_2_17_23_9_49_666,"[SGAPI]:FreeOtherInfoOfContentFragment  content_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_882_112_2_17_23_9_50_667,"[SGAPI]:FreeOtherInfoOfServiceFragment  service_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_971_112_2_17_23_9_50_668,"[SGAPI]:GetComplexInfo  \n")
TRACE_MSG(MBBMS_SG_API_1013_112_2_17_23_9_50_669,"[SGAPI]:GetContentDetail  \n")
TRACE_MSG(MBBMS_SG_API_1088_112_2_17_23_9_50_670,"[SGAPI]:GetContentId  \n")
TRACE_MSG(MBBMS_SG_API_1171_112_2_17_23_9_50_671,"[SGAPI]:SGAPISendSignalToMMI type%d, err_code = %d,is_stop %d ,sig_code %dn")
TRACE_MSG(MBBMS_SG_API_1196_112_2_17_23_9_50_672,"[SGMEM]: FakeListCreate ptr=0X%x")
TRACE_MSG(MBBMS_SG_API_1291_112_2_17_23_9_50_673,"[SGAPI]:GetQueryData  query = %d n")
TRACE_MSG(MBBMS_SG_API_1322_112_2_17_23_9_50_674,"[SGAPI]:GetQueryData  service_list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_1332_112_2_17_23_9_50_675,"[SGAPI]:GetQueryData  content_list_list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_1405_112_2_17_23_9_51_676,"[SGAPI]:GetQueryData  content_list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_1452_112_2_17_23_9_51_677,"[SGAPI]:GetQueryData  QUERY_COMPLEX_INFO  don't match time \n")
TRACE_MSG(MBBMS_SG_API_1633_112_2_17_23_9_51_678,"[SGAPI]:GetPreviewDataByRefList  index %s, len=%d,usage=%d,range=%d n")
TRACE_MSG(MBBMS_SG_API_1638_112_2_17_23_9_51_679,"[SGAPI]:GetPreviewDataByRefList  usage=%d n")
TRACE_MSG(MBBMS_SG_API_1647_112_2_17_23_9_51_680,"[SGAPI]:GetPreviewDataByRefList  preview_data_list_ptr= 0x%x n")
TRACE_MSG(MBBMS_SG_API_1895_112_2_17_23_9_52_681,"[SGAPI]:GetPreviewDatafromPurchaseItem  index %s, len=%d,usage=%d,range=%d n")
TRACE_MSG(MBBMS_SG_API_1971_112_2_17_23_9_52_682,"[SGAPI]:FreeServiceFragment  fragment_ptr= 0x%x n")
TRACE_MSG(MBBMS_SG_API_2000_112_2_17_23_9_52_683,"[SGAPI]:FreeContentFragment  fragment_ptr= 0x%x n")
TRACE_MSG(MBBMS_SG_API_2033_112_2_17_23_9_52_684,"[SGAPI]:GetPreviewDatafromService  index %s, len=%d,usage=%d,range=%d n")
TRACE_MSG(MBBMS_SG_API_2113_112_2_17_23_9_52_685,"[SGAPI]:GetPreviewDatafromContent  index %s, len=%d,usage=%d,range=%d n")
TRACE_MSG(MBBMS_SG_API_2150_112_2_17_23_9_52_686,"[SGAPI]:GetQueryData  service_list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2168_112_2_17_23_9_52_687,"[SGAPI]:GetQueryData  content_list_list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2185_112_2_17_23_9_52_688,"[SGAPI]:GetQueryData  content_list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2265_112_2_17_23_9_52_689,"[SGAPI]:MBBMSSG_FreePreviewData  picture_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2290_112_2_17_23_9_52_690,"[SGAPI]:FreeOtherInfoOfPurchaseChannelFragment  channel_fragment_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2309_112_2_17_23_9_52_691,"[SGAPI]:FreeOtherInfoOfAccessFragment  access_fragment_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2353_112_2_17_23_9_52_692,"[SGAPI]:GetChannelInfoByChannelId  list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2363_112_2_17_23_9_52_693,"[SGAPI]:MBBMSSG_GetPurchaseItem  header_info.id %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_2458_112_2_17_23_9_53_694,"[SGAPI]:MBBMSSG_GetPurchaseItemIndex  list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2549_112_2_17_23_9_53_695,"[SGAPI]:MBBMSSG_FreePurchaseItemIndex  itemIdList_head_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2599_112_2_17_23_9_53_696,"[SGAPI]:MBBMSSG_GetPurchaseItem  itemId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_2654_112_2_17_23_9_53_697,"[SGAPI]:MBBMSSG_FreePurchaseItem  item_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2692_112_2_17_23_9_53_698,"[SGAPI]:MBBMSSG_GetPurchaseDetail  list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2716_112_2_17_23_9_53_699,"[SGAPI]:MBBMSSG_GetPurchaseDetail  PurchaseChannelIdRefList = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2763_112_2_17_23_9_53_700,"[SGAPI]:MBBMSSG_FreePurchaseDetail  detailData_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2785_112_2_17_23_9_53_701,"[SGAPI]:MBBMSSG_FreePurchaseDetail clear one node \n")
TRACE_MSG(MBBMS_SG_API_2813_112_2_17_23_9_53_702,"[SGAPI]:MBBMSSG_GetServiceIndex  serviceIdList_head_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_2866_112_2_17_23_9_54_703,"[SGAPI]:MBBMSSG_GetServiceIndex  service_index_ptr->forfree = %d n")
TRACE_MSG(MBBMS_SG_API_2890_112_2_17_23_9_54_704,"[SGAPI]:MBBMSSG_GetServiceIndex  serviceid %s  weight = %d n")
TRACE_MSG(MBBMS_SG_API_2936_112_2_17_23_9_54_705,"[SGAPI]:MBBMSSG_GetServiceIndex  serviceid %s  weight = %d n")
TRACE_MSG(MBBMS_SG_API_2949_112_2_17_23_9_54_706,"[SGAPI]:MBBMSSG_GetServiceIndex  sort by weight : serviceid %s  weight = %d n")
TRACE_MSG(MBBMS_SG_API_2980_112_2_17_23_9_54_707,"[SGAPI]:MBBMSSG_FreeServiceIndex  serviceIdList_head_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_3019_112_2_17_23_9_54_708,"[SGAPI]:MBBMSSG_GetService  serviceId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_3096_112_2_17_23_9_54_709,"[SGAPI]:MBBMSSG_FreeServiceIndex  service_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_3132_112_2_17_23_9_54_710,"[SGAPI]:MBBMSSG_GetContentIndex  serviceId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_3168_112_2_17_23_9_54_711,"[SGAPI]:MBBMSSG_FreeContentIndex  contentIdList_head_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_3183_112_2_17_23_9_54_712,"[SGAPI]:MBBMSSG_FreeContentIndex  clear one node \n")
TRACE_MSG(MBBMS_SG_API_3215_112_2_17_23_9_54_713,"[SGAPI]:MBBMSSG_GetContent  contentId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_3239_112_2_17_23_9_54_714,"[SGAPI]:MBBMSSG_FreeContent  content_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_3286_112_2_17_23_9_54_715,"[SGAPI]:MBBMSSG_GetComplexInfo condition=0x%x,serviceIdRef %s, len=%d,is_stop %d n")
TRACE_MSG(MBBMS_SG_API_3308_112_2_17_23_9_54_716,"[SGAPI]:MBBMSSG_GetComplexInfo is_stop=0x%x \n")
TRACE_MSG(MBBMS_SG_API_3330_112_2_17_23_9_54_717,"[SGAPI]:MBBMSSG_FreeComplexInfo  complexDataList_head_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_3352_112_2_17_23_9_54_718,"[SGAPI]:MBBMSSG_FreeComplexInfo  clear one node \n")
TRACE_MSG(MBBMS_SG_API_3383_112_2_17_23_9_55_719,"[SGAPI]:MBBMSSG_GetPurchaseItemByService serviceIdRef %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_3407_112_2_17_23_9_55_720,"[SGAPI]:MBBMSSG_GetPurchaseItemByService  serviceid_ref_list_ptr = 0x%x \n")
TRACE_MSG(MBBMS_SG_API_3510_112_2_17_23_9_55_721,"[SGAPI]:MBBMSSG_FreePurchaseItemByService  \n")
TRACE_MSG(MBBMS_SG_API_3547_112_2_17_23_9_55_722,"[SGAPI]:MBBMSSG_FreePurchaseItemByService  \n")
TRACE_MSG(MBBMS_SG_API_3585_112_2_17_23_9_55_723,"[SGAPI]:MBBMSSG_GetPurchaseItemByContent contentId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_3610_112_2_17_23_9_55_724,"[SGAPI]:MBBMSSG_GetPurchaseItemByContent  contentid_ref_list_ptr = 0x%x n")
TRACE_MSG(MBBMS_SG_API_3709_112_2_17_23_9_55_725,"[SGAPI]:MBBMSSG_GetPreviewdata  index %s, len=%d,usage=%d,range=%d n")
TRACE_MSG(MBBMS_SG_API_3757_112_2_17_23_9_55_726,"[SGAPI]:MBBMSSG_FreePreviewData  data_ptr = 0x%x \n")
TRACE_MSG(MBBMS_SG_API_3785_112_2_17_23_9_55_727,"[SGAPI]:MBBMSSG_GetGlobalServiceId  serviceId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_3842_112_2_17_23_9_55_728,"[SGAPI]:MBBMSSG_GetServiceId  globalServiceId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_3891_112_2_17_23_9_55_729,"[SGAPI]:MBBMSSG_GetGlobalContentId  contentId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_3922_112_2_17_23_9_56_730,"[SGAPI]:MBBMSSG_GetContentId  globalContentId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_3956_112_2_17_23_9_56_731,"[SGAPI]:MBBMSSG_GetGlobalPurchaseItemtId  itemId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_4010_112_2_17_23_9_56_732,"[SGAPI]:MBBMSSG_GetPurchaseItemId  globalPurchaseItemId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_4057_112_2_17_23_9_56_733,"[SGAPI]:MBBMSSG_FreeTypeAnyURI  uri_ptr 0x%x n")
TRACE_MSG(MBBMS_SG_API_4094_112_2_17_23_9_56_734,"[SGAPI]: MBBMSSG_APITest  \n")
TRACE_MSG(MBBMS_SG_API_4235_112_2_17_23_9_56_735,"[SGAPI]:MBBMSSG_GetAccess  serviceId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_4266_112_2_17_23_9_56_736,"[SGAPI]:MBBMSSG_GetAccess  is_encrypted =%d n")
TRACE_MSG(MBBMS_SG_API_4302_112_2_17_23_9_56_737,"[[SGAPI]:] MBBMSSG_IsPurchaseItemSubscribed itemId doesn't exist")
TRACE_MSG(MBBMS_SG_API_4307_112_2_17_23_9_56_738,"[[SGAPI]:] MBBMSSG_IsPurchaseItemSubscribed itemId doesn't exist")
TRACE_MSG(MBBMS_SG_API_4310_112_2_17_23_9_56_739,"[SGAPI]:] MBBMSSG_IsPurchaseItemSubscribed, itemId=%s")
TRACE_MSG(MBBMS_SG_API_4317_112_2_17_23_9_56_740,"[SGAPI]:] MBBMSSG_IsPurchaseItemSubscribed, purchaseitem_id=%s, response_code=%d")
TRACE_MSG(MBBMS_SG_API_4355_112_2_17_23_9_56_741,"[SGAPI]:MBBMSSG_GetAccess  serviceId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_4402_112_2_17_23_9_56_742,"[SGAPI]:MBBMSSG_GetAccess  cmmb freqNO %d, serviceid=%d n")
TRACE_MSG(MBBMS_SG_API_4436_112_2_17_23_9_57_743,"[SGAPI]:FreeKeyManagementSystemListofAccess  list_ptr 0x%x n")
TRACE_MSG(MBBMS_SG_API_4479_112_2_17_23_9_57_744,"[SGAPI]:MBBMSSG_FreeAccess  access_ptr 0x%x\n")
TRACE_MSG(MBBMS_SG_API_4510_112_2_17_23_9_57_745,"[SGAPI]:MBBMSSG_GetFreeStatusByServiceId  serviceId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_4532_112_2_17_23_9_57_746,"[SGAPI]:MBBMSSG_GetAccess  is_free =%d \n")
TRACE_MSG(MBBMS_SG_API_4566_112_2_17_23_9_57_747,"[SGAPI]:MBBMSSG_GetAccess  serviceId %s, len=%d n")
TRACE_MSG(MBBMS_SG_API_4713_112_2_17_23_9_57_748,"[SG]: MBBMS_GetFirstBSMSelector")
TRACE_MSG(MBBMS_SG_API_4742_112_2_17_23_9_57_749,"[SG]: MBBMS_GetFirstBSMSelector ,first_bsm_id %d")
TRACE_MSG(MBBMS_SG_API_4763_112_2_17_23_9_57_750,"[SGAPI]:MBBMSSG_GetContentGenreIndex   \n")
TRACE_MSG(MBBMS_SG_API_4770_112_2_17_23_9_57_751,"[SGAPI]:MBBMSSG_GetContentGenreIndex list_ptr 0x%x\n")
TRACE_MSG(MBBMS_SG_INTERNAL_965_112_2_17_23_9_59_752,"[SGMEMPOOL]MBBMS_CreateMemory")
TRACE_MSG(MBBMS_SG_INTERNAL_969_112_2_17_23_9_59_753,"[SGMEMPOOL]MBBMS_CreateMemory addr=0x%x")
TRACE_MSG(MBBMS_SG_INTERNAL_978_112_2_17_23_9_59_754,"[SGMEMPOOL]MBBMS_CreateMemory handle=0x%x")
TRACE_MSG(MBBMS_SG_INTERNAL_981_112_2_17_23_9_59_755,"[SGMEMPOOL]MBBMS_CreateMemory Create Mem handle Fail!")
TRACE_MSG(MBBMS_SG_INTERNAL_989_112_2_17_23_9_59_756,"[SGMEMPOOL]MBBMS_CreateMemory alloc mem failed!")
TRACE_MSG(MBBMS_SG_INTERNAL_995_112_2_17_23_9_59_757,"[SGMEMPOOL]MBBMS_CreateMemory Memory Handle Has been Created!")
TRACE_MSG(MBBMS_SG_INTERNAL_1007_112_2_17_23_9_59_758,"[SGMEMPOOL]MBBMS_DestroyMemory ptr=0x%x handle=0x%x")
TRACE_MSG(MBBMS_SG_INTERNAL_1047_112_2_17_23_9_59_759,"[SGMEMPOOL]mbbms_Malloc:s_mbbms_pool_handle INVALID!")
TRACE_MSG(MBBMS_SG_INTERNAL_1065_112_2_17_23_9_59_760,"[SGMEMPOOL]mbbms_Free:s_mbbms_pool_handle INVALID!")
TRACE_MSG(MBBMS_SG_INTERNAL_1095_112_2_17_23_9_59_761,"[SGMEMPOOL]mbbms_PoolAlloc:s_mbbms_pool_handle INVALID!")
TRACE_MSG(MBBMS_SG_INTERNAL_1117_112_2_17_23_9_59_762,"[SGMEMPOOL]mbbms_PoolFree:s_mbbms_pool_handle INVALID!")
TRACE_MSG(MBBMS_SGDU_PARSE_3596_112_2_17_23_10_7_763,"[SG]: BuildContentGenreIndex genre ptr is NULL")
TRACE_MSG(MBBMS_SGDU_PARSE_4584_112_2_17_23_10_9_764,"[SG]: SG_ParseFile fragment type not match org type %d  cur type %d")
TRACE_MSG(MBBMS_SGDU_PARSE_5177_112_2_17_23_10_10_765,"[SG]: SG_GetIdxList SG IDX not exit")
TRACE_MSG(MBBMS_SGDU_PARSE_5316_112_2_17_23_10_10_766,"[SG]: SG_ParseFragmentIdx enter %d")
TRACE_MSG(MBBMS_SGDU_PARSE_5320_112_2_17_23_10_10_767,"[SG]: SG_ParseFragmentIdx alread parsed")
TRACE_MSG(MBBMS_SGDU_PARSE_5333_112_2_17_23_10_10_768,"[SG]: SG_ParseFragmentIdx parse service")
TRACE_MSG(MBBMS_SGDU_PARSE_5343_112_2_17_23_10_10_769,"[SG]: SG_ParseFragmentIdx parse access")
TRACE_MSG(MBBMS_SGDU_PARSE_5365_112_2_17_23_10_10_770,"[SG]: SG_ParseFragmentIdx parse access")
TRACE_MSG(MBBMS_SGDU_PARSE_5374_112_2_17_23_10_10_771,"[SG]: SG_ParseFragmentIdx parse purchase item")
TRACE_MSG(MBBMS_SGDU_PARSE_5382_112_2_17_23_10_10_772,"[SG]: SG_ParseFragmentIdx parse purchase data")
TRACE_MSG(MBBMS_SGDU_PARSE_5390_112_2_17_23_10_10_773,"[SG]: SG_ParseFragmentIdx parse purchase channel")
TRACE_MSG(MBBMS_SGDU_PARSE_5398_112_2_17_23_10_10_774,"[SG]: SG_ParseFragmentIdx parse schedule")
TRACE_MSG(MBBMS_SGDU_PARSE_5406_112_2_17_23_10_10_775,"[SG]: SG_ParseFragmentIdx parse content")
TRACE_MSG(MBBMS_SGDU_PARSE_5414_112_2_17_23_10_11_776,"[SG]: SG_ParseFragmentIdx parse preview data")
TRACE_MSG(MBBMS_SGDU_PARSE_5422_112_2_17_23_10_11_777,"[SG]: SG_ParseFragmentIdx parse interactive")
TRACE_MSG(MBBMS_SGDU_PARSE_5456_112_2_17_23_10_11_778,"[SG]: SG_ParseFragmentIdx exit ret=%d  count=%d")
TRACE_MSG(MBBMS_SUB_239_112_2_17_23_10_12_779,"MBBMS_Free_Service_Request_Ptr ... \n")
TRACE_MSG(MBBMS_SUB_254_112_2_17_23_10_12_780,"MBBMS_Free_Service_Request_Ptr END \n")
TRACE_MSG(MBBMS_SUB_273_112_2_17_23_10_12_781,"MBBMS_Free_Service_Request_Res_Ptr ...\n")
TRACE_MSG(MBBMS_SUB_299_112_2_17_23_10_12_782,"MBBMS_Free_Service_Request_Res_Ptr END \n")
TRACE_MSG(MBBMS_SUB_321_112_2_17_23_10_12_783,"MBBMS_Free_SubUpdate_Request_Ptr ... \n")
TRACE_MSG(MBBMS_SUB_347_112_2_17_23_10_12_784,"MBBMS_Free_SubUpdate_Request_Ptr END \n")
TRACE_MSG(MBBMS_SUB_368_112_2_17_23_10_12_785,"MBBMS_Free_Subupdate_Res_Ptr ... \n")
TRACE_MSG(MBBMS_SUB_387_112_2_17_23_10_12_786,"MBBMS_Free_Subupdate_Res_Ptr END \n")
TRACE_MSG(MBBMS_SUB_405_112_2_17_23_10_12_787,"MBBMS_Free_Account_Query_Ptr ... \n")
TRACE_MSG(MBBMS_SUB_408_112_2_17_23_10_12_788,"MBBMS_Free_Account_Query_Ptr END \n")
TRACE_MSG(MBBMS_SUB_430_112_2_17_23_10_12_789,"MBBMS_Free_Account_Query_Res_Ptr ... \n")
TRACE_MSG(MBBMS_SUB_461_112_2_17_23_10_12_790,"MBBMS_Free_Account_Query_Res_Ptr END \n")
TRACE_MSG(MBBMS_SUB_571_112_2_17_23_10_12_791,"CREATE_MBBMS_FOLDER Error \n")
TRACE_MSG(MBBMS_SUB_576_112_2_17_23_10_12_792,"CREATE_MBBMS_FOLDER Error \n")
TRACE_MSG(MBBMS_SUB_669_112_2_17_23_10_12_793,"MBBMS GetMSKIDFromMIKEY MSKID = 0x%x MTKID = 0x%x DOMAINID = 0x%x")
TRACE_MSG(MBBMS_SUB_680_112_2_17_23_10_12_794,"MBBMS GetMTKIDFromMIKEY MIKEY_Open failed")
TRACE_MSG(MBBMS_SUB_695_112_2_17_23_10_13_795,"MBBMS Print:%s \n")
TRACE_MSG(MBBMS_SUB_699_112_2_17_23_10_13_796,"MBBMS Print:%s \n")
TRACE_MSG(MBBMS_SUB_807_112_2_17_23_10_13_797,"MBBMS_Free_Init_Param...\n")
TRACE_MSG(MBBMS_SUB_819_112_2_17_23_10_13_798,"MBBMS_Free_Init_Param END \n")
TRACE_MSG(MBBMS_SUB_828_112_2_17_23_10_13_799,"MBBMS_Free_Auth_Param ....\n")
TRACE_MSG(MBBMS_SUB_838_112_2_17_23_10_13_800,"MBBMS_Free_Auth_Param END \n")
TRACE_MSG(MBBMS_SUB_847_112_2_17_23_10_13_801,"MBBMS_Free_Post_Param...\n")
TRACE_MSG(MBBMS_SUB_884_112_2_17_23_10_13_802,"MBBMS_Free_Post_Param END \n")
TRACE_MSG(MBBMS_SUB_894_112_2_17_23_10_13_803,"MBBMS_Free_Get_Param ... \n")
TRACE_MSG(MBBMS_SUB_927_112_2_17_23_10_13_804,"MBBMS_Free_Get_Param END \n")
TRACE_MSG(MBBMS_SUB_960_112_2_17_23_10_13_805,"#ERROR# mbbms HTTP_CloseRequest return ERROR! \n")
TRACE_MSG(MBBMS_SUB_969_112_2_17_23_10_13_806,"MBBMS MSK SUB sendMMI Result:%d")
TRACE_MSG(MBBMS_SUB_996_112_2_17_23_10_13_807,"MBBMS MSK ONLY Save SUB sendMMI Result:%d")
TRACE_MSG(MBBMS_SUB_1043_112_2_17_23_10_13_808,"MBBMS_Sub_MSK_Init done \n")
TRACE_MSG(MBBMS_SUB_1063_112_2_17_23_10_13_809,"MBBMS_Sub_MSK_Exit entering \n")
TRACE_MSG(MBBMS_SUB_1082_112_2_17_23_10_13_810,"MBBMS_Sub_MSK_Exit Leaving ... \n")
TRACE_MSG(MBBMS_SUB_1117_112_2_17_23_10_13_811,"MBBMS_MSK_SUB_Stop entering...\n")
TRACE_MSG(MBBMS_SUB_1129_112_2_17_23_10_13_812,"MBBMS_MSK_SUB_Stop leaving... \n")
TRACE_MSG(MBBMS_SUB_1152_112_2_17_23_10_13_813,"MBBMS_Save_Msk \"#Error mikey too long:%d \n")
TRACE_MSG(MBBMS_SUB_1158_112_2_17_23_10_13_814,"MBBMS_Save_Msk Entering ...msk_id: %x key_domain_id:%x. n")
TRACE_MSG(MBBMS_SUB_1206_112_2_17_23_10_14_815,"mbbms_put_msk msk:%d --0x%x \n")
TRACE_MSG(MBBMS_SUB_1232_112_2_17_23_10_14_816,"MBBMS_Put_Msk mua SET OK.  \n")
TRACE_MSG(MBBMS_SUB_1238_112_2_17_23_10_14_817,"MBBMS MSK Output mikey:0x%x %x %x %x %x %x %x %x n")
TRACE_MSG(MBBMS_SUB_1244_112_2_17_23_10_14_818,"MBBMS_Put_Msk mua error \n")
TRACE_MSG(MBBMS_SUB_1248_112_2_17_23_10_14_819,"MBBMS_PUT_MSK OK \n")
TRACE_MSG(MBBMS_SUB_1288_112_2_17_23_10_14_820,"MBBMS_Set_MSK: GBA TEST MODE: return TRUE anyway \n")
TRACE_MSG(MBBMS_SUB_1328_112_2_17_23_10_14_821,"MBBMS_Operation... type:%d \n")
TRACE_MSG(MBBMS_SUB_1349_112_2_17_23_10_14_822,"MBBMS_Operation leaving ... \n")
TRACE_MSG(MBBMS_SUB_1363_112_2_17_23_10_14_823,"MBBMS_Operation_Status ... %d \n")
TRACE_MSG(MBBMS_SUB_1402_112_2_17_23_10_14_824,"MBBMS_Service_Request ...valid:%d global_id:%s n")
TRACE_MSG(MBBMS_SUB_1417_112_2_17_23_10_14_825,"MBBMS_Service_Request leaving... \n")
TRACE_MSG(MBBMS_SUB_1456_112_2_17_23_10_14_826,"MBBMS_Unsubscribe_Request ... \n")
TRACE_MSG(MBBMS_SUB_1471_112_2_17_23_10_14_827,"MBBMS_Unsubscribe_Request leaving.. \n")
TRACE_MSG(MBBMS_SUB_1514_112_2_17_23_10_14_828,"MBBMS_Subscription_Update ...\n")
TRACE_MSG(MBBMS_SUB_1523_112_2_17_23_10_14_829,"MBBMS_Subscription_Update leaving \n")
TRACE_MSG(MBBMS_SUB_1560_112_2_17_23_10_14_830,"MBBMS_Account_Inquiry %d... \n")
TRACE_MSG(MBBMS_SUB_1569_112_2_17_23_10_14_831,"MBBMS_Account_Inquiry leaving... \n")
TRACE_MSG(MBBMS_SUB_1655_112_2_17_23_10_14_832,"MBBMS mystristr: [%s]  [%s] \n")
TRACE_MSG(MBBMS_SUB_1681_112_2_17_23_10_14_833,"MBBMS_SUB mbbms_sub_parse_item: item:%s value:%s \n")
TRACE_MSG(MBBMS_SUB_1713_112_2_17_23_10_15_834,"mbbms_Check_Stale_Truth  is TRUE \n")
TRACE_MSG(MBBMS_SUB_1716_112_2_17_23_10_15_835,"mbbms_Check_Stale_Truth  is FALSE \n")
TRACE_MSG(MBBMS_SUB_1735_112_2_17_23_10_15_836,"MBBMS ADD LTKM Entering.... \n")
TRACE_MSG(MBBMS_SUB_1743_112_2_17_23_10_15_837,"MBBMS MSK LEN: %d \n")
TRACE_MSG(MBBMS_SUB_1748_112_2_17_23_10_15_838,"MBBMS MSK Input mikey:0x%x %x %x %x %x %x %x %x n")
TRACE_MSG(MBBMS_SUB_1758_112_2_17_23_10_15_839,"MBBMS MSK SET OK.  \n")
TRACE_MSG(MBBMS_SUB_1764_112_2_17_23_10_15_840,"MBBMS MSK Output mikey:0x%x %x %x %x %x %x %x %x n")
TRACE_MSG(MBBMS_SUB_1773_112_2_17_23_10_15_841,"MBBMS MSK MAY SET ACK \n")
TRACE_MSG(MBBMS_SUB_1777_112_2_17_23_10_15_842,"MBBMS MSK HAVE ACK \n")
TRACE_MSG(MBBMS_SUB_1802_112_2_17_23_10_15_843,"MBBMS MSK SET ERROR \n")
TRACE_MSG(MBBMS_SUB_1805_112_2_17_23_10_15_844,"MBBMS ADD LTKM: add ltkm done \n")
TRACE_MSG(MBBMS_SUB_1822_112_2_17_23_10_15_845,"MBBMS ADD LTKM Entering.... \n")
TRACE_MSG(MBBMS_SUB_1833_112_2_17_23_10_15_846,"MBBMS MSK LEN: %d \n")
TRACE_MSG(MBBMS_SUB_1838_112_2_17_23_10_15_847,"MBBMS MSK Input mikey:0x%x %x %x %x %x %x %x %x n")
TRACE_MSG(MBBMS_SUB_1848_112_2_17_23_10_15_848,"MBBMS MSK SET OK.  \n")
TRACE_MSG(MBBMS_SUB_1854_112_2_17_23_10_15_849,"MBBMS MSK Output mikey:0x%x %x %x %x %x %x %x %x n")
TRACE_MSG(MBBMS_SUB_1863_112_2_17_23_10_15_850,"MBBMS MSK MAY SET ACK \n")
TRACE_MSG(MBBMS_SUB_1867_112_2_17_23_10_15_851,"MBBMS MSK HAVE ACK \n")
TRACE_MSG(MBBMS_SUB_1892_112_2_17_23_10_15_852,"MBBMS MSK SET ERROR \n")
TRACE_MSG(MBBMS_SUB_1895_112_2_17_23_10_15_853,"MBBMS ADD LTKM: add ltkm done \n")
TRACE_MSG(MBBMS_SUB_1989_112_2_17_23_10_15_854,"\n HTTP TEST no memory \n")
TRACE_MSG(MBBMS_SUB_2027_112_2_17_23_10_15_855,"MBBMS MBBMS_SUB_Prepare_Ack_Request Get NAF Failed \n")
TRACE_MSG(MBBMS_SUB_2029_112_2_17_23_10_15_856,"MBBMS MBBMS_SUB_Prepare_Ack_Request GET OK naf is %s \n")
TRACE_MSG(MBBMS_SUB_2035_112_2_17_23_10_15_857,"MBBMS_SUB_Prepare_Ack_Request URI:%s \n")
TRACE_MSG(MBBMS_SUB_2054_112_2_17_23_10_15_858,"MBBMS MSK SET HTTP ACK \n")
TRACE_MSG(MBBMS_SUB_2062_112_2_17_23_10_15_859,"MBBMS MSK SET HTTP ACK: ltkmlen:%d ltkm:0x%x %x n")
TRACE_MSG(MBBMS_SUB_2063_112_2_17_23_10_15_860,"MBBMS MSK SET HTTP ACK: base64 ltkm:%s \n")
TRACE_MSG(MBBMS_SUB_2076_112_2_17_23_10_15_861,"MBBMS ACK  BODY: %s \n")
TRACE_MSG(MBBMS_SUB_2080_112_2_17_23_10_15_862,"#ERROR# mbbms HTTP_PostRequest return ERROR!\n")
TRACE_MSG(MBBMS_SUB_2145_112_2_17_23_10_15_863,"MBBMS_SUB_SERVICE_REQUEST request_id  VALID \n")
TRACE_MSG(MBBMS_SUB_2151_112_2_17_23_10_15_864,"MBBMS_SUB_SERVICE_REQUEST request_id NOT VALID \n")
TRACE_MSG(MBBMS_SUB_2168_112_2_17_23_10_15_865,"MBBMS_SUB_SERVICE_REQUEST Entering LOOP")
TRACE_MSG(MBBMS_SUB_2173_112_2_17_23_10_15_866,"MBBMS_SUB_SERVICE_REQUEST: global_id:%s \n")
TRACE_MSG(MBBMS_SUB_2181_112_2_17_23_10_15_867,"MBBMS_SUB_SERVICE_REQUEST: id_ref:%s \n")
TRACE_MSG(MBBMS_SUB_2190_112_2_17_23_10_15_868,"MBBMS_SUB_SERVICE_REQUEST: price %f str:%s \n")
TRACE_MSG(MBBMS_SUB_2198_112_2_17_23_10_16_869,"MBBMS_SUB_SERVICE_REQUEST: price SET TO SERO \n")
TRACE_MSG(MBBMS_SUB_2210_112_2_17_23_10_16_870,"MBBMS_SUB_SERVICE_REQUEST: id_ref:%s \n")
TRACE_MSG(MBBMS_SUB_2217_112_2_17_23_10_16_871,"MBBMS_SUB_SERVICE_REQUEST ADD ERROR IDREF \n")
TRACE_MSG(MBBMS_SUB_2227_112_2_17_23_10_16_872,"MBBMS_SUB_SERVICE_REQUEST: price %f\n")
TRACE_MSG(MBBMS_SUB_2235_112_2_17_23_10_16_873,"MBBMS_SUB_SERVICE_REQUEST: price SET TO ZERO \n")
TRACE_MSG(MBBMS_SUB_2244_112_2_17_23_10_16_874,"MBBMS_SUB_SERVICE_REQUEST LOOP")
TRACE_MSG(MBBMS_SUB_2272_112_2_17_23_10_16_875,"MBBMS_SUB_SERVICE_UNSUB Entering LOOP")
TRACE_MSG(MBBMS_SUB_2280_112_2_17_23_10_16_876,"MBBMS_SUB_SERVICE_UNSUB  LOOP")
TRACE_MSG(MBBMS_SUB_2318_112_2_17_23_10_16_877,"MBBMS Send 2-Step data, sub_state %d \n")
TRACE_MSG(MBBMS_SUB_2320_112_2_17_23_10_16_878,"MBBMS XML: %s \n")
TRACE_MSG(MBBMS_SUB_2336_112_2_17_23_10_16_879,"\n HTTP TEST no memory \n")
TRACE_MSG(MBBMS_SUB_2399_112_2_17_23_10_16_880,"MBBMS HANDLE_MSK_SIG_INIT_CNF Get NAF Failed \n")
TRACE_MSG(MBBMS_SUB_2401_112_2_17_23_10_16_881,"MBBMS HANDLE_MSK_SIG_INIT_CNF GET  SUB_UPDATE OK naf is %s \n")
TRACE_MSG(MBBMS_SUB_2409_112_2_17_23_10_16_882,"MBBMS HANDLE_MSK_SIG_INIT_CNF Get NAF Failed \n")
TRACE_MSG(MBBMS_SUB_2411_112_2_17_23_10_16_883,"MBBMS HANDLE_MSK_SIG_INIT_CNF GET OK naf is %s \n")
TRACE_MSG(MBBMS_SUB_2429_112_2_17_23_10_16_884,"MBBMS_SUB_SERVICE_REQUEST request_id  VALID \n")
TRACE_MSG(MBBMS_SUB_2437_112_2_17_23_10_16_885,"MBBMS_SUB_SERVICE_REQUEST request_id NOT VALID \n")
TRACE_MSG(MBBMS_SUB_2444_112_2_17_23_10_16_886,"MBBMS_SUB_SERVICE_REQUEST valid:%d request_id:%d str:%s n")
TRACE_MSG(MBBMS_SUB_2466_112_2_17_23_10_16_887,"\"# MBBMS User Agent NOT SET \n")
TRACE_MSG(MBBMS_SUB_2476_112_2_17_23_10_16_888,"MBBMS_SUB_SERVICE_REQUEST Entering LOOP")
TRACE_MSG(MBBMS_SUB_2481_112_2_17_23_10_16_889,"MBBMS_SUB_SERVICE_REQUEST: global_id:%s \n")
TRACE_MSG(MBBMS_SUB_2489_112_2_17_23_10_16_890,"MBBMS_SUB_SERVICE_REQUEST: id_ref:%s \n")
TRACE_MSG(MBBMS_SUB_2498_112_2_17_23_10_16_891,"MBBMS_SUB_SERVICE_REQUEST: price %f str:%s \n")
TRACE_MSG(MBBMS_SUB_2506_112_2_17_23_10_16_892,"MBBMS_SUB_SERVICE_REQUEST: price SET TO SERO \n")
TRACE_MSG(MBBMS_SUB_2517_112_2_17_23_10_16_893,"MBBMS_SUB_SERVICE_REQUEST: id_ref:%s \n")
TRACE_MSG(MBBMS_SUB_2524_112_2_17_23_10_16_894,"MBBMS_SUB_SERVICE_REQUEST ADD ERROR IDREF \n")
TRACE_MSG(MBBMS_SUB_2534_112_2_17_23_10_16_895,"MBBMS_SUB_SERVICE_REQUEST: price %f\n")
TRACE_MSG(MBBMS_SUB_2542_112_2_17_23_10_16_896,"MBBMS_SUB_SERVICE_REQUEST: price SET TO ZERO \n")
TRACE_MSG(MBBMS_SUB_2551_112_2_17_23_10_16_897,"MBBMS_SUB_SERVICE_REQUEST LOOP")
TRACE_MSG(MBBMS_SUB_2587_112_2_17_23_10_16_898,"\"# MBBMS User Agent NOT SET \n")
TRACE_MSG(MBBMS_SUB_2598_112_2_17_23_10_16_899,"MBBMS_SUB_SERVICE_UNSUB Entering LOOP\n")
TRACE_MSG(MBBMS_SUB_2605_112_2_17_23_10_16_900,"MBBMS_SUB_SERVICE_UNSUB Copy OK \n")
TRACE_MSG(MBBMS_SUB_2607_112_2_17_23_10_16_901,"MBBMS_SUB_SERVICE_UNSUB  LOOP \n")
TRACE_MSG(MBBMS_SUB_2609_112_2_17_23_10_16_902,"MBBMS_SUB_SERVICE_UNSUB Leaving LOOP \n")
TRACE_MSG(MBBMS_SUB_2657_112_2_17_23_10_16_903,"\"# MBBMS User Agent NOT SET \n")
TRACE_MSG(MBBMS_SUB_2690_112_2_17_23_10_17_904,"\"#MBBMS GET BTID FAILED, Set to EMPTY \n")
TRACE_MSG(MBBMS_SUB_2695_112_2_17_23_10_17_905,"\"#MBBMS GET BTID len:%d, string:%s \n")
TRACE_MSG(MBBMS_SUB_2709_112_2_17_23_10_17_906,"\"# MBBMS User Agent NOT SET \n")
TRACE_MSG(MBBMS_SUB_2722_112_2_17_23_10_17_907,"MBBMS_MSK_GET_MSK Send HTTP Request  \n")
TRACE_MSG(MBBMS_SUB_2740_112_2_17_23_10_17_908,"\"# MBBMS User Agent NOT SET \n")
TRACE_MSG(MBBMS_SUB_2755_112_2_17_23_10_17_909,"MBBMS MSK REQ keydomainid 0x%x %x %x msk_id:0x%x n")
TRACE_MSG(MBBMS_SUB_2760_112_2_17_23_10_17_910,"MBBMS MSK REQ keydomainid base64: %s\n")
TRACE_MSG(MBBMS_SUB_2769_112_2_17_23_10_17_911,"MBBMS MSK REQ msk id: %d base64: %s\n")
TRACE_MSG(MBBMS_SUB_2786_112_2_17_23_10_17_912,"MBBMS IMSI:%s \n")
TRACE_MSG(MBBMS_SUB_2793_112_2_17_23_10_17_913,"MBBMS CMMBSN:%s \n")
TRACE_MSG(MBBMS_SUB_2813_112_2_17_23_10_17_914,"#ERROR# mbbms HTTP_PostRequest return ERROR!\n")
TRACE_MSG(MBBMS_SUB_2815_112_2_17_23_10_17_915,"MBBMS SUB Send HTTP Request OK \n")
TRACE_MSG(MBBMS_SUB_2830_112_2_17_23_10_17_916,"\n HTTP TEST no memory \n")
TRACE_MSG(MBBMS_SUB_2873_112_2_17_23_10_17_917,"MBBMS HANDLE_MSK_SIG_INIT_CNF Get NAF Failed \n")
TRACE_MSG(MBBMS_SUB_2875_112_2_17_23_10_17_918,"MBBMS HANDLE_MSK_SIG_INIT_CNF GET OK naf is %s \n")
TRACE_MSG(MBBMS_SUB_2905_112_2_17_23_10_17_919,"#ERROR# mbbms HTTP_PostRequest return ERROR!\n")
TRACE_MSG(MBBMS_SUB_2910_112_2_17_23_10_17_920," MBBMS MSK SIG_HTTP_INIT_CNF:error sub state%d \n")
TRACE_MSG(MBBMS_SUB_2913_112_2_17_23_10_17_921,"MBBMS SendHTTPRequest: sub_state:%d \n")
TRACE_MSG(MBBMS_SUB_2914_112_2_17_23_10_17_922,"MBBMS SUB BODY: %s \n")
TRACE_MSG(MBBMS_SUB_2916_112_2_17_23_10_17_923,"MBBMS SUB URI: %s \n")
TRACE_MSG(MBBMS_SUB_2940_112_2_17_23_10_17_924,"[MBBMS MSK PARSE]username = %s \n")
TRACE_MSG(MBBMS_SUB_2947_112_2_17_23_10_17_925," MBBMS SUB DIGEST: MD5(%s) = %s \n")
TRACE_MSG(MBBMS_SUB_2957_112_2_17_23_10_17_926,"MBBMS SUB DIGEST: MD5(\"POST:/:MD5(%s)\") = %s \n")
TRACE_MSG(MBBMS_SUB_2968_112_2_17_23_10_17_927," MBBMS SUB DIGEST: MD5(%s:%s:%s)=%s n")
TRACE_MSG(MBBMS_SUB_2986_112_2_17_23_10_17_928,"MBBMS SUB DIGEST: final response = %s, set string:%s n")
TRACE_MSG(MBBMS_SUB_3012_112_2_17_23_10_17_929,"[MBBMS SUB]ks_ext_naf base64:%s \n")
TRACE_MSG(MBBMS_SUB_3014_112_2_17_23_10_17_930,"[MBBMS MSK PARSE]IMPI = %s \n")
TRACE_MSG(MBBMS_SUB_3017_112_2_17_23_10_17_931,"[MBBMS MSK PARSE]IMPI = %s \n")
TRACE_MSG(MBBMS_SUB_3021_112_2_17_23_10_17_932,"MBBMS_Generate_Http_Head naf:%s \n")
TRACE_MSG(MBBMS_SUB_3025_112_2_17_23_10_17_933,"MBBMS_Generate_Http_Head MBBMS_SUB_Get_MRK len:%d 0x%x %x %x n")
TRACE_MSG(MBBMS_SUB_3029_112_2_17_23_10_17_934,"[MBBMS SUB]ks_ext_naf base64:%s \n")
TRACE_MSG(MBBMS_SUB_3034_112_2_17_23_10_17_935,"MBBMS_Generate_Http_Head MBBMS_SUB_Get_MRK FAILED \n")
TRACE_MSG(MBBMS_SUB_3039_112_2_17_23_10_17_936,"[MBBMS MSK PARSE]nonce = %s \n")
TRACE_MSG(MBBMS_SUB_3041_112_2_17_23_10_17_937,"[MBBMS MSK PARSE]realm = %s \n")
TRACE_MSG(MBBMS_SUB_3043_112_2_17_23_10_17_938,"[MBBMS MSK PARSE]opaque = %s \n")
TRACE_MSG(MBBMS_SUB_3046_112_2_17_23_10_17_939,"[MBBMS MSK PARSE]qop = %s \n")
TRACE_MSG(MBBMS_SUB_3054_112_2_17_23_10_17_940,"MBBMS MSK CNONCE Generated: [%s] \n")
TRACE_MSG(MBBMS_SUB_3062_112_2_17_23_10_17_941,"[MBBMS MSK PARSE]username = %s \n")
TRACE_MSG(MBBMS_SUB_3069_112_2_17_23_10_17_942," MBBMS SUB DIGEST: MD5(%s) = %s \n")
TRACE_MSG(MBBMS_SUB_3079_112_2_17_23_10_17_943,"MBBMS SUB DIGEST: MD5(\"POST:/:MD5(%s)\") = %s \n")
TRACE_MSG(MBBMS_SUB_3090_112_2_17_23_10_17_944," MBBMS SUB DIGEST: MD5(%s:%s:%s)=%s n")
TRACE_MSG(MBBMS_SUB_3108_112_2_17_23_10_17_945,"MBBMS SUB DIGEST: final response = %s, set string:%s n")
TRACE_MSG(MBBMS_SUB_3147_112_2_17_23_10_17_946,"mbbms_sub_createfolder: create dir success\n")
TRACE_MSG(MBBMS_SUB_3152_112_2_17_23_10_17_947,"mbbms_sub_createfolder: fail to create dir \n")
TRACE_MSG(MBBMS_SUB_3158_112_2_17_23_10_17_948,"mbbms_sub_createfolder: dir is exist \n")
TRACE_MSG(MBBMS_SUB_3194_112_2_17_23_10_18_949,"mbbms_sub_get_file: ok \n")
TRACE_MSG(MBBMS_SUB_3223_112_2_17_23_10_18_950,"mbbms_check_isfileexist: ok \n")
TRACE_MSG(MBBMS_SUB_3260_112_2_17_23_10_18_951,"MBBMS_Get_Local_IMSI_CMMBSN IMSI read=%d sfs err=%d n")
TRACE_MSG(MBBMS_SUB_3264_112_2_17_23_10_18_952,"MBBMS_Get_Local_IMSI_CMMBSN IMSI len read fail \n")
TRACE_MSG(MBBMS_SUB_3273_112_2_17_23_10_18_953,"MBBMS_Get_Local_IMSI_CMMBSN IMSI len read error \n")
TRACE_MSG(MBBMS_SUB_3277_112_2_17_23_10_18_954,"MBBMS_Get_Local_IMSI_CMMBSN MD5(imsi)=%s \n")
TRACE_MSG(MBBMS_SUB_3282_112_2_17_23_10_18_955,"MBBMS_Get_Local_IMSI_CMMBSN IMSI base64 too long \n")
TRACE_MSG(MBBMS_SUB_3285_112_2_17_23_10_18_956,"MBBMS_Get_Local_IMSI_CMMBSN IMSI long:%d")
TRACE_MSG(MBBMS_SUB_3305_112_2_17_23_10_18_957,"MBBMS_Get_Local_IMSI_CMMBSN CMMBSN read=%d len=%d sfs err=%d n")
TRACE_MSG(MBBMS_SUB_3309_112_2_17_23_10_18_958,"MBBMS_Get_Local_IMSI_CMMBSN CMMBSN len read fail \n")
TRACE_MSG(MBBMS_SUB_3317_112_2_17_23_10_18_959,"MBBMS_Get_Local_IMSI_CMMBSN CMMBSN read fail \n")
TRACE_MSG(MBBMS_SUB_3321_112_2_17_23_10_18_960,"MBBMS_Get_Local_IMSI_CMMBSN MD5(cmmbsn)=%s \n")
TRACE_MSG(MBBMS_SUB_3325_112_2_17_23_10_18_961,"MBBMS_Get_Local_IMSI_CMMBSN CMMBSN base64 too long \n")
TRACE_MSG(MBBMS_SUB_3328_112_2_17_23_10_18_962,"MBBMS_Get_Local_IMSI_CMMBSN CMMBSN base64 len %d \n")
TRACE_MSG(MBBMS_SUB_3361_112_2_17_23_10_18_963,"MBBMS_Save_Local_IMSI_CMMBSN entering.... \n")
TRACE_MSG(MBBMS_SUB_3371_112_2_17_23_10_18_964,"MBBMS_Save_Local_IMSI_CMMBSN createFile OK \n")
TRACE_MSG(MBBMS_SUB_3375_112_2_17_23_10_18_965,"MBBMS_Save_Local_IMSI_CMMBSN IMSI current is not exist. \n")
TRACE_MSG(MBBMS_SUB_3382_112_2_17_23_10_18_966,"MBBMS_Save_Local_IMSI_CMMBSN begin imsi MD5 encode \n")
TRACE_MSG(MBBMS_SUB_3387_112_2_17_23_10_18_967,"MBBMS_Get_Local_IMSI_CMMBSN IMSI base64 too long \n")
TRACE_MSG(MBBMS_SUB_3390_112_2_17_23_10_18_968,"MBBMS_Save_Local_IMSI_CMMBSN MD5[imsi]=%s \n")
TRACE_MSG(MBBMS_SUB_3398_112_2_17_23_10_18_969,"MBBMS_Save_Local_IMSI_CMMBSN begin save cmmbsn \n")
TRACE_MSG(MBBMS_SUB_3402_112_2_17_23_10_18_970,"MBBMS_Save_Local_IMSI_CMMBSN get local cmmbsn fail \n")
TRACE_MSG(MBBMS_SUB_3412_112_2_17_23_10_18_971,"MBBMS_Get_Local_IMSI_CMMBSN CMMBSN base64 too long \n")
TRACE_MSG(MBBMS_SUB_3415_112_2_17_23_10_18_972,"MBBMS_Save_Local_IMSI_CMMBSN MD5[cmmbsn]=%s \n")
TRACE_MSG(MBBMS_SUB_3429_112_2_17_23_10_18_973,"MBBMS_Save_Local_IMSI_CMMBSN return ret:%d\n")
TRACE_MSG(MBBMS_SUB_3451_112_2_17_23_10_18_974,"MBBMS_Check_IMSI_UAM entering \n")
TRACE_MSG(MBBMS_SUB_3455_112_2_17_23_10_18_975,"MBBMS_Check_IMSI_UAM Return FALSE, NO UAM found!! \n")
TRACE_MSG(MBBMS_SUB_3461_112_2_17_23_10_18_976,"MBBMS_Check_IMSI_UAM get local imsi, cmmbsn fail \n")
TRACE_MSG(MBBMS_SUB_3467_112_2_17_23_10_18_977,"MBBMS_Check_IMSI_UAM IMSI current is not exist. \n")
TRACE_MSG(MBBMS_SUB_3473_112_2_17_23_10_18_978,"MBBMS_Check_IMSI_UAM IMSI length is not EQUAL. \n")
TRACE_MSG(MBBMS_SUB_3480_112_2_17_23_10_18_979,"MBBMS_Check_IMSI_UAM IMSI is not EQUAL. \n")
TRACE_MSG(MBBMS_SUB_3489_112_2_17_23_10_18_980,"MBBMS_Check_IMSI_UAM get local imsi, cmmbsn fail \n")
TRACE_MSG(MBBMS_SUB_3495_112_2_17_23_10_18_981,"MBBMS_Check_IMSI_UAM cmmbsn len not equal \n")
TRACE_MSG(MBBMS_SUB_3504_112_2_17_23_10_18_982,"MBBMS_Check_IMSI_UAM curcmmbsn is not equal cmmbsn  \n")
TRACE_MSG(MBBMS_SUB_3513_112_2_17_23_10_18_983,"MBBMS_Check_IMSI_UAM GBA needed is TRUE \n")
TRACE_MSG(MBBMS_SUB_3517_112_2_17_23_10_18_984,"MBBMS_Check_IMSI_UAM leaving %d \n")
TRACE_MSG(MBBMS_SUB_3545_112_2_17_23_10_18_985,"MBBMS_Load_UpdateTable IMSI current is not exist. \n")
TRACE_MSG(MBBMS_SUB_3552_112_2_17_23_10_18_986,"MBBMS_Load_UpdateTable get local imsi fail \n")
TRACE_MSG(MBBMS_SUB_3558_112_2_17_23_10_18_987,"MBBMS_Load_UpdateTable IMSI length is not EQUAL. \n")
TRACE_MSG(MBBMS_SUB_3566_112_2_17_23_10_18_988,"MBBMS_Load_UpdateTable IMSI is not EQUAL. \n")
TRACE_MSG(MBBMS_SUB_3572_112_2_17_23_10_18_989,"MBBMS_Load_UpdateTable SUB_BuildPurchaseList Return TRUE! \n")
TRACE_MSG(MBBMS_SUB_3574_112_2_17_23_10_18_990,"MBBMS_Load_UpdateTable SUB_BuildPurchaseList return FALSE! \n")
TRACE_MSG(MBBMS_SUB_3601_112_2_17_23_10_18_991," MBBMS MSK SUB Launch HTTP Failed for NO WAP Available! \n")
TRACE_MSG(MBBMS_SUB_3614_112_2_17_23_10_18_992,"#ERROR# mbbms HTTP_InitRequest return ERROR!\n")
TRACE_MSG(MBBMS_SUB_3635_112_2_17_23_10_18_993,"\"#MBBMS GBA HTTP INIT CNF ERROR!!\n")
TRACE_MSG(MBBMS_SUB_3637_112_2_17_23_10_18_994," MBBMS MSK SIG_HTTP_INIT_CNF:Entering... sub state%d \n")
TRACE_MSG(MBBMS_SUB_3670_112_2_17_23_10_18_995,"MBBMS_HANDLE_MSK_SIG_NEED_AUTH_IND sub_post_param_ptr XML BODY TOO LONG \n")
TRACE_MSG(MBBMS_SUB_3673_112_2_17_23_10_19_996,"MBBMS_HANDLE_MSK_SIG_NEED_AUTH_IND \"# XML BODY IS NULL \"# sub_post_param_ptr is NULL \n")
TRACE_MSG(MBBMS_SUB_3676_112_2_17_23_10_19_997,"#ERROR# mbbms HTTP_CancelRequest return ERROR!\n")
TRACE_MSG(MBBMS_SUB_3681_112_2_17_23_10_19_998,"MBBMS\"\"\"\"\"\"\"# NEED_AUTH FAILED ONCE MORE\"\"\n")
TRACE_MSG(MBBMS_SUB_3735_112_2_17_23_10_19_999,"\n HTTP TEST no memory \n")
TRACE_MSG(MBBMS_SUB_3781_112_2_17_23_10_19_1000,"MBBMS MSK CNONCE Generated: [%s] \n")
TRACE_MSG(MBBMS_SUB_3818_112_2_17_23_10_19_1001,"MBBMS SUB Digest:[%s] \n")
TRACE_MSG(MBBMS_SUB_3822_112_2_17_23_10_19_1002,"MBBMS HANDLE_MSK_SIG_INIT_CNF Get NAF Failed \n")
TRACE_MSG(MBBMS_SUB_3824_112_2_17_23_10_19_1003,"MBBMS HANDLE_MSK_SIG_INIT_CNF GET OK naf is %s \n")
TRACE_MSG(MBBMS_SUB_3839_112_2_17_23_10_19_1004,"MBBMS Send_Manual_Auth sub_xml_ptr TOO LONG %d \n")
TRACE_MSG(MBBMS_SUB_3850_112_2_17_23_10_19_1005,"#ERROR# mbbms HTTP_PostRequest return ERROR!\n")
TRACE_MSG(MBBMS_SUB_3882_112_2_17_23_10_19_1006,"MBBMS MSK SIG_DATA_IND Received Result:%d \n")
TRACE_MSG(MBBMS_SUB_3884_112_2_17_23_10_19_1007,"MBBMS MSK Received Str:%s \n")
TRACE_MSG(MBBMS_SUB_3896_112_2_17_23_10_19_1008,"[MBBMS SUB PARSE]OPERATION sub_state:%d HAS BEEN ACCEPTTED! rst:%d\n")
TRACE_MSG(MBBMS_SUB_3904_112_2_17_23_10_19_1009,"MBBMS MBBMS_MTV_CANCEL OK, Clean Data \n")
TRACE_MSG(MBBMS_SUB_3913_112_2_17_23_10_19_1010,"[MBBMS SUB PARSE]OPERATION LTKM len:%d dat:0x%x %x %x %x n")
TRACE_MSG(MBBMS_SUB_3919_112_2_17_23_10_19_1011,"[MBBMS SUB PARSE]OPERATION ACK HAS BEEN SENT OUT \n")
TRACE_MSG(MBBMS_SUB_3925_112_2_17_23_10_19_1012,"[MBBMS SUB PARSE]NOT NEED TO SEND ACK \n")
TRACE_MSG(MBBMS_SUB_3933_112_2_17_23_10_19_1013,"MBBMS OPERATION DO NOT HAVE LTKM DATA \n")
TRACE_MSG(MBBMS_SUB_3940_112_2_17_23_10_19_1014,"[MBBMS SUB PARSE]OPERATION sub_state:%d HAS BEEN Denied! rst:%d\n")
TRACE_MSG(MBBMS_SUB_3952_112_2_17_23_10_19_1015,"[MBBMS SUB PARSE]OPERATION XML PARSE FAILED:%d \n")
TRACE_MSG(MBBMS_SUB_3978_112_2_17_23_10_19_1016,"MBBMS GBLID:%s IS SUBSCRIBED \n")
TRACE_MSG(MBBMS_SUB_3998_112_2_17_23_10_19_1017,"MBBMS SUB UPDATE: SUB_TABLE subscription done \n")
TRACE_MSG(MBBMS_SUB_4003_112_2_17_23_10_19_1018,"MBBMS SUB UPDATE: SUB_TABLE subscription FAIL \n")
TRACE_MSG(MBBMS_SUB_4010_112_2_17_23_10_19_1019,"MBBMS_SUB_SERVICE_REQUEST not need to send ACK. OK \n")
TRACE_MSG(MBBMS_SUB_4030_112_2_17_23_10_19_1020,"MBBMS_SUB_SERVICE_REQUEST need to send ACK Now...\n")
TRACE_MSG(MBBMS_SUB_4054_112_2_17_23_10_19_1021,"MBBMS SUB UPDATE: unsubscription XML Parse ok\n")
TRACE_MSG(MBBMS_SUB_4065_112_2_17_23_10_19_1022,"MBBMS GBLID:%s STOP SUBSCRIBED \n")
TRACE_MSG(MBBMS_SUB_4087_112_2_17_23_10_19_1023,"MBBMS SUB UPDATE: SUB_TABLE unsubscription done \n")
TRACE_MSG(MBBMS_SUB_4092_112_2_17_23_10_19_1024,"MBBMS SUB UPDATE: SUB_TABLE unsubscription ok, BUT other area \n")
TRACE_MSG(MBBMS_SUB_4099_112_2_17_23_10_19_1025,"MBBMS_SUB_SERVICE_UNSUB not need to send ACK. OK \n")
TRACE_MSG(MBBMS_SUB_4119_112_2_17_23_10_19_1026,"MBBMS_SUB_SERVICE_UNSUB need to send ACK Now...\n")
TRACE_MSG(MBBMS_SUB_4141_112_2_17_23_10_19_1027,"MBBMS ACCOUNT QUIRY PARSE OK \n")
TRACE_MSG(MBBMS_SUB_4145_112_2_17_23_10_19_1028,"MBBMS Account Quiry Refresh DONE OK \n")
TRACE_MSG(MBBMS_SUB_4183_112_2_17_23_10_20_1029,"MBBMS SUB MSK Find activationstatus:%d\n")
TRACE_MSG(MBBMS_SUB_4202_112_2_17_23_10_20_1030,"MBBMS_SUB_UPDATEParsing: %s resp_code:%d n")
TRACE_MSG(MBBMS_SUB_4203_112_2_17_23_10_20_1031,"   MBBMS_SUB_UPDATE first MSKID: %d \n")
TRACE_MSG(MBBMS_SUB_4214_112_2_17_23_10_20_1032,"MBBMS SUB UPDATE: CREATE SUB_TABLE OK \n")
TRACE_MSG(MBBMS_SUB_4219_112_2_17_23_10_20_1033,"MBBMS SUB  UPDATE: Create SUB_TABLE FAILED\n")
TRACE_MSG(MBBMS_SUB_4239_112_2_17_23_10_20_1034," MBBMS_SUB_UPDATE FINISHED NOW! \n")
TRACE_MSG(MBBMS_SUB_4251_112_2_17_23_10_20_1035,"MBBMS_MSK_GET_MSK status:%d \n")
TRACE_MSG(MBBMS_SUB_4264_112_2_17_23_10_20_1036,"MBBMS_MSK_GET_MSK NOT NEED ACK>Send MMI OK\n")
TRACE_MSG(MBBMS_SUB_4272_112_2_17_23_10_20_1037,"MBBMS_MSK_GET_MSK need to send ACK Now...\n")
TRACE_MSG(MBBMS_SUB_4277_112_2_17_23_10_20_1038,"MBBMS MBBMS_MSK_GET_MSK PARSE FAILED \n")
TRACE_MSG(MBBMS_SUB_4299_112_2_17_23_10_20_1039,"MBBMS MSK SIG_HEADER_IND Received  result:%d \n")
TRACE_MSG(MBBMS_SUB_4301_112_2_17_23_10_20_1040,"MBBMS MSK SIG_HEADER_IND Received  string:%s \n")
TRACE_MSG(MBBMS_SUB_4309_112_2_17_23_10_20_1041,"MBBMS MSK_SIG_HEADER_IND Header Failed \n")
TRACE_MSG(MBBMS_SUB_4318_112_2_17_23_10_20_1042,"MBBMS Authenticate & Stale Exist. Re-Send Digest IS Valid \n")
TRACE_MSG(MBBMS_SUB_4323_112_2_17_23_10_20_1043,"MBBMS Authenticate Header NOT Exist \n")
TRACE_MSG(MBBMS_SUB_4337_112_2_17_23_10_20_1044,"[MBBMS SUB][Save HEAD]:%s \n")
TRACE_MSG(MBBMS_SUB_4347_112_2_17_23_10_20_1045,"[MBBMS OPERATION DIGEST] FAILED! !\n")
TRACE_MSG(MBBMS_SUB_4352_112_2_17_23_10_20_1046,"MBBMS OPERATION ACK FAILED \n")
TRACE_MSG(MBBMS_SUB_4362_112_2_17_23_10_20_1047,"MBBMS Authenticate & Stale Exist. Re-Send Digest IS Valid \n")
TRACE_MSG(MBBMS_SUB_4367_112_2_17_23_10_20_1048,"MBBMS Authenticate Header NOT Exist \n")
TRACE_MSG(MBBMS_SUB_4378_112_2_17_23_10_20_1049,"[MBBMS SUB][Save HEAD]:%s \n")
TRACE_MSG(MBBMS_SUB_4392_112_2_17_23_10_20_1050,"MBBMS SUB MSK ACK FAILED \n")
TRACE_MSG(MBBMS_SUB_4405_112_2_17_23_10_20_1051,"MBBMS Authenticate & Stale Exist. Re-Send Digest IS Valid \n")
TRACE_MSG(MBBMS_SUB_4410_112_2_17_23_10_20_1052,"MBBMS Authenticate Header NOT Exist \n")
TRACE_MSG(MBBMS_SUB_4421_112_2_17_23_10_20_1053,"[MBBMS SUB][Save HEAD]:%s \n")
TRACE_MSG(MBBMS_SUB_4434_112_2_17_23_10_20_1054,"MBBMS UNSUB MSK ACK FAILED \n")
TRACE_MSG(MBBMS_SUB_4460_112_2_17_23_10_20_1055,"MBBMS Authenticate & Stale Exist. Re-Send Digest IS Valid \n")
TRACE_MSG(MBBMS_SUB_4465_112_2_17_23_10_20_1056,"MBBMS Authenticate Header NOT Exist \n")
TRACE_MSG(MBBMS_SUB_4475_112_2_17_23_10_20_1057,"MBBMS SUB: Save the HTTP Head for authentication \n")
TRACE_MSG(MBBMS_SUB_4486_112_2_17_23_10_20_1058,"MBBMS SUB MBBMS_MSK_GET_MSK: AUTH Failed \n")
TRACE_MSG(MBBMS_SUB_4504_112_2_17_23_10_20_1059,"MBBMS Authentication %d OK, and with Authentication Also \n")
TRACE_MSG(MBBMS_SUB_4512_112_2_17_23_10_20_1060,"MBBMS Operation substate:%d ACK OK \n")
TRACE_MSG(MBBMS_SUB_4535_112_2_17_23_10_20_1061,"MBBMS_SUB_SERVICE_REQUEST HTTP ACK SUCCESSFUL Received \n")
TRACE_MSG(MBBMS_SUB_4544_112_2_17_23_10_20_1062,"MBBMS_SUB_SERVICE_UNSUB HTTP ACK SUCCESSFUL Received \n")
TRACE_MSG(MBBMS_SUB_4570_112_2_17_23_10_20_1063,"MBBMS_MSK_SUB_Thread sub_xml_ptr addr 0x%x \n")
TRACE_MSG(MBBMS_SUB_4571_112_2_17_23_10_20_1064,"MBBMS_MSK_SUB_Thread sub_head_ptr addr 0x%x \n")
TRACE_MSG(MBBMS_SUB_4576_112_2_17_23_10_20_1065,"MBBMS_MSK_SUB_THREAD One signal received %d\n")
TRACE_MSG(MBBMS_SUB_4587_112_2_17_23_10_20_1066,"MBBMS SUB THREAD: MBBMS_Account_Inquiry_t type:%d \n")
TRACE_MSG(MBBMS_SUB_4592_112_2_17_23_10_20_1067,"MBBMS_MSK_INNER_SIGNAL Begin to Launch http \n")
TRACE_MSG(MBBMS_SUB_4598_112_2_17_23_10_20_1068,"MBBMS_MSK_INNER_SIGNAL Directly Send HTTP Request\n")
TRACE_MSG(MBBMS_SUB_4606_112_2_17_23_10_20_1069,"MBBMS_MSK_SUB_Stop IS STOPPING THE PRCESS :%d \n")
TRACE_MSG(MBBMS_SUB_4632_112_2_17_23_10_20_1070,"#ERROR# mbbms HTTP_CloseRequest return ERROR! \n")
TRACE_MSG(MBBMS_SUB_4643_112_2_17_23_10_20_1071,"MBBMS_MSK_INNER_EXIT signal received \n")
TRACE_MSG(MBBMS_SUB_4653_112_2_17_23_10_21_1072,"[MBBMS_MSK] HTT_SIG_INIT_CNF received\n")
TRACE_MSG(MBBMS_SUB_4666_112_2_17_23_10_21_1073,"[MBBMS_MSK] HTTP_SIG_NEED_AUTH_IND received\n")
TRACE_MSG(MBBMS_SUB_4672_112_2_17_23_10_21_1074,"[MBBMS_MSK] HTTP_SIG_REQUEST_ID_IND received\n")
TRACE_MSG(MBBMS_SUB_4678_112_2_17_23_10_21_1075,"[MBBMS_MSK] HTTP_SIG_GET_CNF received\n")
TRACE_MSG(MBBMS_SUB_4684_112_2_17_23_10_21_1076,"[MBBMS_MSK] HTTP_SIG_HEAD_CNF received\n")
TRACE_MSG(MBBMS_SUB_4690_112_2_17_23_10_21_1077,"[MBBMS_MSK] HTTP_SIG_DATA_IND received\n")
TRACE_MSG(MBBMS_SUB_4696_112_2_17_23_10_21_1078,"[MBBMS_MSK] HTTP_SIG_HEADER_IND received\n")
TRACE_MSG(MBBMS_SUB_4702_112_2_17_23_10_21_1079,"[MBBMS_MSK] HTTP_SIG_ERROR_IND received\n")
TRACE_MSG(MBBMS_SUB_4712_112_2_17_23_10_21_1080,"[MBBMS_MSK] HTTP_SIG_CLOSE_CNF received\n")
TRACE_MSG(MBBMS_SUB_4720_112_2_17_23_10_21_1081,"MBBMS MSK SUB CHANGE sendMMI Result:%d")
TRACE_MSG(MBBMS_SUB_4725_112_2_17_23_10_21_1082,"MBBMS MSK SUB sendMMI by subcallback Result:%d")
TRACE_MSG(MBBMS_SUB_4728_112_2_17_23_10_21_1083,"MBBMS MSK SUB sendMMI by subcallback ok\n")
TRACE_MSG(MBBMS_SUB_4744_112_2_17_23_10_21_1084,"[MBBMS_MSK] HTTP_SIG_CANCEL_CNF received\n")
TRACE_MSG(MBBMS_SUB_4754_112_2_17_23_10_21_1085," [MBBMS-MSK] HTTP_SIG_NET_PROG_IND received \n")
TRACE_MSG(MBBMS_SUB_4757_112_2_17_23_10_21_1086," [MBBMS-MSK] HTTP_SIG_REDIRECT_IND received \n")
TRACE_MSG(MBBMS_SUB_4770_112_2_17_23_10_21_1087,"MBBMS SUB THREAD: Signal Freeing..... \n")
TRACE_MSG(MBBMS_SUB_4772_112_2_17_23_10_21_1088,"MBBMS SUB THREAD: Signal Free OK \n")
TRACE_MSG(MBBMS_SUB_4789_112_2_17_23_10_21_1089,"MBBMS SUB Free sub_head_ptr \n")
TRACE_MSG(MBBMS_SUB_4792_112_2_17_23_10_21_1090,"MBBMS SUB Free sub_xml_ptr \n")
TRACE_MSG(MBBMS_SUB_4796_112_2_17_23_10_21_1091,"MBBMS SUB Free http_msk_ack_data \n")
TRACE_MSG(MBBMS_SUB_4799_112_2_17_23_10_21_1092,"MBBMS SUB FREE OVER, THREAD EXIT \n")
TRACE_MSG(MBBMS_SUB_4839_112_2_17_23_10_21_1093,"MBBMS_SUB_Get_MRK WE HAVE ks_ext_naf IN GBA-MRK \n")
TRACE_MSG(MBBMS_SUB_4851_112_2_17_23_10_21_1094,"MBBMS_SUB_Get_MRK impi:%s naf:%s \n")
TRACE_MSG(MBBMS_SUB_4861_112_2_17_23_10_21_1095,"MBBMS_SUB_Get_MRK naf domain name:%s impi:%s \n")
TRACE_MSG(MBBMS_SUB_4883_112_2_17_23_10_21_1096,"MBBMS_SUB_Get_MRK SD CARD OK len:%d 0x%x %x %x %xn")
TRACE_MSG(MBBMS_SUB_4890_112_2_17_23_10_21_1097,"MBBMS MRK Result:len:%d ks_ext_naf:0x%2x %2x %2x %2x n")
TRACE_MSG(MBBMS_SUB_PARSE_686_112_2_17_23_10_22_1098,"mbbms subBase64binaryToString base64_decode error!\n")
TRACE_MSG(MBBMS_SUB_PARSE_722_112_2_17_23_10_22_1099,"mbbms subBase64binaryToUint32 base64_decode error!\n")
TRACE_MSG(MBBMS_SUB_PARSE_777_112_2_17_23_10_23_1100,"mbbms subGetUpdateRespTypeTailData ENTER \n")
TRACE_MSG(MBBMS_SUB_PARSE_783_112_2_17_23_10_23_1101,"mbbms subGetUpdateRespTypeTailData OVER \n")
TRACE_MSG(MBBMS_SUB_PARSE_1009_112_2_17_23_10_23_1102,"[SG]: subServiceResponseSyntaxHandler ServiceResponse")
TRACE_MSG(MBBMS_SUB_PARSE_1015_112_2_17_23_10_23_1103,"[SG]: subServiceResponseSyntaxHandler, requestID_ServiceResponse=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1029_112_2_17_23_10_23_1104,"[SG]: subServiceResponseSyntaxHandler, globalStatusCode_ServiceResponse=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1036_112_2_17_23_10_23_1105,"[SG]: subServiceResponseSyntaxHandler, description_ServiceResponse=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1046_112_2_17_23_10_23_1106,"[SG]: subServiceResponseSyntaxHandler PurchaseItem_ServiceResponse")
TRACE_MSG(MBBMS_SUB_PARSE_1055_112_2_17_23_10_23_1107,"[SG]: subServiceResponseSyntaxHandler, globalIDRef_PurchaseItem_Service=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1071_112_2_17_23_10_23_1108,"[SG]: subServiceResponseSyntaxHandler, itemwiseStatusCode_PurchaseItem=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1075_112_2_17_23_10_23_1109,"[SG]: subServiceResponseSyntaxHandler SmartcardProfileSpecificPart_Service")
TRACE_MSG(MBBMS_SUB_PARSE_1087_112_2_17_23_10_23_1110,"[SG]: subServiceResponseSyntaxHandler LTKM_SmartcardProfile_Service")
TRACE_MSG(MBBMS_SUB_PARSE_1132_112_2_17_23_10_23_1111,"[SG]: subAccountResponseSyntaxHandler AccountResponse")
TRACE_MSG(MBBMS_SUB_PARSE_1139_112_2_17_23_10_23_1112,"[SG]: subAccountResponseSyntaxHandler, requestID_AccountResponse=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1153_112_2_17_23_10_23_1113,"[SG]: subAccountResponseSyntaxHandler, globalStatusCode_ServiceResponse=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1160_112_2_17_23_10_23_1114,"[SG]: subAccountResponseSyntaxHandler Description_AccountResponse=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1169_112_2_17_23_10_23_1115,"[SG]: subAccountResponseSyntaxHandler BillingInformation=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1173_112_2_17_23_10_23_1116,"[SG]: subAccountResponseSyntaxHandler, ignore BillingInformation 1..N")
TRACE_MSG(MBBMS_SUB_PARSE_1203_112_2_17_23_10_23_1117,"[SG]: subAccountResponseSyntaxHandler PurchaseItem_AccountResponse")
TRACE_MSG(MBBMS_SUB_PARSE_1212_112_2_17_23_10_23_1118,"[SG]: subAccountResponseSyntaxHandler, globalIDRef_PurchaseItem_Account=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1223_112_2_17_23_10_24_1119,"[SG]: subAccountResponseSyntaxHandler BillingInformation=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1227_112_2_17_23_10_24_1120,"[SG]: subAccountResponseSyntaxHandler, ignore sub_Description_PurchaseItem 1..N")
TRACE_MSG(MBBMS_SUB_PARSE_1253_112_2_17_23_10_24_1121,"[SG]: subAccountResponseSyntaxHandler PurchaseItemFragment_PurchaseItem")
TRACE_MSG(MBBMS_SUB_PARSE_1263_112_2_17_23_10_24_1122,"[SG]: subAccountResponseSyntaxHandler, idref_PurchaseData=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1304_112_2_17_23_10_24_1123,"[SG]: subActivationResponseSyntaxHandler ActivationResponse")
TRACE_MSG(MBBMS_SUB_PARSE_1311_112_2_17_23_10_24_1124,"[SG]: subActivationResponseSyntaxHandler, Result_ActivationResponse=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1319_112_2_17_23_10_24_1125,"[SG]: subActivationResponseSyntaxHandler Desc_ActivationResponse=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1324_112_2_17_23_10_24_1126,"[SG]: subActivationResponseSyntaxHandler SmartcardProfileSpecificPart_Activation")
TRACE_MSG(MBBMS_SUB_PARSE_1336_112_2_17_23_10_24_1127,"[SG]: subActivationResponseSyntaxHandler LTKM_SmartcardProfile_Activation")
TRACE_MSG(MBBMS_SUB_PARSE_1393_112_2_17_23_10_24_1128,"[SG]: subSubscriptionUpdateSyntaxHandler mbmsSecurityRegisterResponse")
TRACE_MSG(MBBMS_SUB_PARSE_1404_112_2_17_23_10_24_1129,"[SG]: subSubscriptionUpdateSyntaxHandler Response_MSecurityReg")
TRACE_MSG(MBBMS_SUB_PARSE_1413_112_2_17_23_10_24_1130,"[SG]: subSubscriptionUpdateSyntaxHandler, serviceID_Response=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1423_112_2_17_23_10_24_1131,"[SG]: subSubscriptionUpdateSyntaxHandler, ResponseCode_Response_Security=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1428_112_2_17_23_10_24_1132,"[SG]: subSubscriptionUpdateSyntaxHandler SubscriptionUpdateExtension")
TRACE_MSG(MBBMS_SUB_PARSE_1436_112_2_17_23_10_24_1133,"[SG]: subSubscriptionUpdateSyntaxHandler Response_description=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1445_112_2_17_23_10_24_1134,"[SG]: subSubscriptionUpdateSyntaxHandler, version_subUpdateExtension=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1456_112_2_17_23_10_24_1135,"[SG]: subSubscriptionUpdateSyntaxHandler LTKM_subUpdateExtension")
TRACE_MSG(MBBMS_SUB_PARSE_1465_112_2_17_23_10_24_1136,"[SG]: subSubscriptionUpdateSyntaxHandler, validFrom_LTKM=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1474_112_2_17_23_10_24_1137,"[SG]: subSubscriptionUpdateSyntaxHandler, validUntil_LTKM=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1484_112_2_17_23_10_24_1138,"[SG]: subSubscriptionUpdateSyntaxHandler, MSKID_LTKM=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1496_112_2_17_23_10_24_1139,"[SG]: subSubscriptionUpdateSyntaxHandler RegistrationResponseExtension")
TRACE_MSG(MBBMS_SUB_PARSE_1504_112_2_17_23_10_24_1140,"[SG]: subSubscriptionUpdateSyntaxHandler, delivery_type=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1512_112_2_17_23_10_24_1141,"[SG]: subSubscriptionUpdateSyntaxHandler, delivery_type=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1556_112_2_17_23_10_24_1142,"[SG]: subMSKResponseSyntaxHandler, MSKRes_description=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1560_112_2_17_23_10_24_1143,"[SG]: subMSKResponseSyntaxHandler mbmsMSKResponse")
TRACE_MSG(MBBMS_SUB_PARSE_1569_112_2_17_23_10_24_1144,"[SG]: subMSKResponseSyntaxHandler Response_MSKResp")
TRACE_MSG(MBBMS_SUB_PARSE_1573_112_2_17_23_10_24_1145,"[SG]: subMSKResponseSyntaxHandler MSK_Response")
TRACE_MSG(MBBMS_SUB_PARSE_1588_112_2_17_23_10_24_1146,"[SG]: subMSKResponseSyntaxHandler, keyDomainID_MSK")
TRACE_MSG(MBBMS_SUB_PARSE_1597_112_2_17_23_10_24_1147,"[SG]: subMSKResponseSyntaxHandler, MSKID_MSK=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1614_112_2_17_23_10_24_1148,"[SG]: subMSKResponseSyntaxHandler, ResponseCode_Response_MSKResp=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1619_112_2_17_23_10_24_1149,"[SG]: subMSKResponseSyntaxHandler, LTKMResponseMSKExtension_Response")
TRACE_MSG(MBBMS_SUB_PARSE_1627_112_2_17_23_10_24_1150,"[SG]: subMSKResponseSyntaxHandler, version_LTKMExtension=%d")
TRACE_MSG(MBBMS_SUB_PARSE_1640_112_2_17_23_10_24_1151,"[SG]: subMSKResponseSyntaxHandler, LTKM_LTKMExtension")
TRACE_MSG(MBBMS_SUB_PARSE_1679_112_2_17_23_10_24_1152,"[SG]: subSyntaxHandler none type")
TRACE_MSG(MBBMS_SUB_PARSE_1727_112_2_17_23_10_25_1153,"[SG]: subSyntaxHandler none subresp type, from=%d, syntax_name=%s, element=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1735_112_2_17_23_10_25_1154,"[SG]: subSyntaxHandler subtype(%d) data is zero")
TRACE_MSG(MBBMS_SUB_PARSE_1759_112_2_17_23_10_25_1155,"[SG]: subBuildSGDataList enter")
TRACE_MSG(MBBMS_SUB_PARSE_1786_112_2_17_23_10_25_1156,"[SG]: subBuildSGDataList ser idref ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_1787_112_2_17_23_10_25_1157,"[SG]: subBuildSGDataList con idref ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_1788_112_2_17_23_10_25_1158,"[SG]: subBuildSGDataList namelist ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_1789_112_2_17_23_10_25_1159,"[SG]: subBuildSGDataList desc ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_1799_112_2_17_23_10_25_1160,"[SG]: subBuildSGDataList PurchaseItem alloc error")
TRACE_MSG(MBBMS_SUB_PARSE_1829_112_2_17_23_10_25_1161,"[SG]: subBuildSGDataList PurchaseData alloc error")
TRACE_MSG(MBBMS_SUB_PARSE_1856_112_2_17_23_10_25_1162,"[SG]: subXmlStartElementHandler name=%s")
TRACE_MSG(MBBMS_SUB_PARSE_1935_112_2_17_23_10_25_1163,"[SG]: subXmlStartElementHandler exit")
TRACE_MSG(MBBMS_SUB_PARSE_1953_112_2_17_23_10_25_1164,"[SG]: subXmlEndElementHandler name=%s")
TRACE_MSG(MBBMS_SUB_PARSE_2008_112_2_17_23_10_25_1165,"[SG]: subXmlEndElementHandler exit")
TRACE_MSG(MBBMS_SUB_PARSE_2028_112_2_17_23_10_25_1166,"[SG]: subXmlCharacterDataHandler, none type")
TRACE_MSG(MBBMS_SUB_PARSE_2098_112_2_17_23_10_26_1167,"[SG]: SUB_ParseData fail")
TRACE_MSG(MBBMS_SUB_PARSE_2113_112_2_17_23_10_26_1168,"MBBMS subCleanUpdateRespTypeList ENTERING... \n")
TRACE_MSG(MBBMS_SUB_PARSE_2129_112_2_17_23_10_26_1169,"MBBMS subCleanUpdateRespTypeList OVER \n")
TRACE_MSG(MBBMS_SUB_PARSE_2179_112_2_17_23_10_26_1170,"sub_CheckMSKID msk_id = %d")
TRACE_MSG(MBBMS_SUB_PARSE_2181_112_2_17_23_10_26_1171,"sub_CheckMSKID get purchase_item errcode = %d")
TRACE_MSG(MBBMS_SUB_PARSE_2186_112_2_17_23_10_26_1172,"sub_CheckMSKID temp = 0X%x")
TRACE_MSG(MBBMS_SUB_PARSE_2190_112_2_17_23_10_26_1173,"sub_CheckMSKID get access errcode = %d")
TRACE_MSG(MBBMS_SUB_PARSE_2194_112_2_17_23_10_26_1174,"sub_CheckMSKID get access systemlist_cnt = %d")
TRACE_MSG(MBBMS_SUB_PARSE_2199_112_2_17_23_10_26_1175,"sub_CheckMSKID get access keylist_cnt = %d")
TRACE_MSG(MBBMS_SUB_PARSE_2203_112_2_17_23_10_26_1176,"sub_CheckMSKID get access keyidlist_ptr = 0X%x")
TRACE_MSG(MBBMS_SUB_PARSE_2207_112_2_17_23_10_26_1177,"sub_CheckMSKID get access data_len = %d")
TRACE_MSG(MBBMS_SUB_PARSE_2208_112_2_17_23_10_26_1178,"sub_CheckMSKID get access data = %s")
TRACE_MSG(MBBMS_SUB_PARSE_2215_112_2_17_23_10_26_1179,"sub_CheckMSKID get access sg_msk_id = %d")
TRACE_MSG(MBBMS_SUB_PARSE_2299_112_2_17_23_10_26_1180,"[SG]: SUB_InitSubscription")
TRACE_MSG(MBBMS_SUB_PARSE_2304_112_2_17_23_10_26_1181,"[SG]: SUB_InitSubscription id=%s")
TRACE_MSG(MBBMS_SUB_PARSE_2310_112_2_17_23_10_26_1182,"[SG]: SUB_InitSubscription CheckPurchaseitemID fail")
TRACE_MSG(MBBMS_SUB_PARSE_2315_112_2_17_23_10_26_1183,"[SG]: SUB_InitSubscription response_code fail")
TRACE_MSG(MBBMS_SUB_PARSE_2354_112_2_17_23_10_26_1184,"[SG]: SUB_RefreshSubscription")
TRACE_MSG(MBBMS_SUB_PARSE_2358_112_2_17_23_10_26_1185,"[SG]: SUB_RefreshSubscription sub_ptr is null")
TRACE_MSG(MBBMS_SUB_PARSE_2364_112_2_17_23_10_26_1186,"[SG]: SUB_RefreshSubscription response_code fail")
TRACE_MSG(MBBMS_SUB_PARSE_2371_112_2_17_23_10_26_1187,"[SG]: SUB_RefreshSubscription id=%s")
TRACE_MSG(MBBMS_SUB_PARSE_2377_112_2_17_23_10_26_1188,"[SG]: SUB_RefreshSubscription CheckPurchaseitemID fail")
TRACE_MSG(MBBMS_SUB_PARSE_2416_112_2_17_23_10_26_1189,"[SG]: SUB_UpdateSubscription")
TRACE_MSG(MBBMS_SUB_PARSE_2421_112_2_17_23_10_26_1190,"[SG]: SUB_UpdateSubscription CheckPurchaseitemID fail")
TRACE_MSG(MBBMS_SUB_PARSE_2426_112_2_17_23_10_26_1191,"[SG]: SUB_UpdateSubscription itemwiseStatusCode fail")
TRACE_MSG(MBBMS_SUB_PARSE_2442_112_2_17_23_10_26_1192,"[SG]: SUB_UpdateSubscription, no sub info")
TRACE_MSG(MBBMS_SUB_PARSE_2565_112_2_17_23_10_27_1193,"[SG]: sub_SavePurchaseData enter p_sub=0X%x")
TRACE_MSG(MBBMS_SUB_PARSE_2572_112_2_17_23_10_27_1194,"[SG]: sub_SavePurchaseData, purchase=%s, response=%d")
TRACE_MSG(MBBMS_SUB_PARSE_2576_112_2_17_23_10_27_1195,"[SG]: sub_SavePurchaseData, purchase count=%d")
TRACE_MSG(MBBMS_SUB_PARSE_2582_112_2_17_23_10_27_1196,"[SG]: sub_SavePurchaseData, alloc mem fail!!!")
TRACE_MSG(MBBMS_SUB_PARSE_2598_112_2_17_23_10_27_1197,"[SG]: sub_SavePurchaseData, write=%d")
TRACE_MSG(MBBMS_SUB_PARSE_2601_112_2_17_23_10_27_1198,"[SG]: sub_SavePurchaseData, write file fail!!!")
TRACE_MSG(MBBMS_SUB_PARSE_2607_112_2_17_23_10_27_1199,"[SG]: sub_SavePurchaseData, create file fail!!!")
TRACE_MSG(MBBMS_SUB_PARSE_2629_112_2_17_23_10_27_1200,"[SG]: SUB_BuildPurchaseList, enter")
TRACE_MSG(MBBMS_SUB_PARSE_2638_112_2_17_23_10_27_1201,"[SG]: SUB_BuildPurchaseList, get file size fail!!!")
TRACE_MSG(MBBMS_SUB_PARSE_2645_112_2_17_23_10_27_1202,"[SG]: sub_SavePurchaseData, alloc data mem fail!!!")
TRACE_MSG(MBBMS_SUB_PARSE_2651_112_2_17_23_10_27_1203,"[SG]: SUB_BuildPurchaseList, read file fail!!!")
TRACE_MSG(MBBMS_SUB_PARSE_2661_112_2_17_23_10_27_1204,"[SG]: SUB_BuildPurchaseList, len=%d count=%d")
TRACE_MSG(MBBMS_SUB_PARSE_2668_112_2_17_23_10_27_1205,"[SG]: sub_SavePurchaseData, alloc table mem fail!!!")
TRACE_MSG(MBBMS_SUB_PARSE_2681_112_2_17_23_10_27_1206,"[SG]: SUB_BuildPurchaseList, open file fail!!!")
TRACE_MSG(MBBMS_SUB_PARSE_2692_112_2_17_23_10_27_1207,"[SG]: SUB_BuildPurchaseList, ret=%d")
TRACE_MSG(MBBMS_SUB_PARSE_2712_112_2_17_23_10_27_1208,"mbbms_SubPrint Inq ptr=0x%x info ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2714_112_2_17_23_10_27_1209,"mbbms_SubPrint pur_item=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2716_112_2_17_23_10_27_1210,"mbbms_SubPrint pur_item count=%d")
TRACE_MSG(MBBMS_SUB_PARSE_2720_112_2_17_23_10_27_1211,"mbbms_SubPrint item_ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2721_112_2_17_23_10_27_1212,"mbbms_SubPrint ser idref ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2722_112_2_17_23_10_27_1213,"mbbms_SubPrint con idref ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2723_112_2_17_23_10_27_1214,"mbbms_SubPrint namelist ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2724_112_2_17_23_10_27_1215,"mbbms_SubPrint desc ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2726_112_2_17_23_10_27_1216,"mbbms_SubPrint ser_list=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2728_112_2_17_23_10_27_1217,"mbbms_SubPrint ser_cnt=%d")
TRACE_MSG(MBBMS_SUB_PARSE_2732_112_2_17_23_10_27_1218,"mbbms_SubPrint ser_ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2736_112_2_17_23_10_27_1219,"mbbms_SubPrint pur_data=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2738_112_2_17_23_10_27_1220,"mbbms_SubPrint pur_data count=%d")
TRACE_MSG(MBBMS_SUB_PARSE_2742_112_2_17_23_10_27_1221,"mbbms_SubPrint data ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2743_112_2_17_23_10_27_1222,"mbbms_SubPrint desc ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2744_112_2_17_23_10_27_1223,"mbbms_SubPrint price ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2745_112_2_17_23_10_27_1224,"mbbms_SubPrint itemref ptr=0x%x")
TRACE_MSG(MBBMS_SUB_PARSE_2746_112_2_17_23_10_27_1225,"mbbms_SubPrint channel ptr=0x%x")
TRACE_MSG(MIKEY_179_112_2_17_23_10_29_1226,"MIKEY start %p")
TRACE_MSG(MIKEY_209_112_2_17_23_10_29_1227,"MIKEY_GetCurrentPayloadType: handle is zero")
TRACE_MSG(MIKEY_224_112_2_17_23_10_29_1228,"MIKEY_GetNextPayloadType: current_payload_type exceed valid range")
TRACE_MSG(MIKEY_230_112_2_17_23_10_29_1229,"MIKEY_GetNextPayloadType: handle is zero")
TRACE_MSG(MIKEY_245_112_2_17_23_10_29_1230,"MIKEY_GetPayloadInfo: current_payload_type exceed valid range")
TRACE_MSG(MIKEY_251_112_2_17_23_10_29_1231,"MIKEY_GetPayloadInfo: handle is zero")
TRACE_MSG(MIKEY_291_112_2_17_23_10_29_1232,"MIKEY_NextPayload: next type %s")
TRACE_MSG(MIKEY_297_112_2_17_23_10_29_1233,"MIKEY_NextPayload: next payloadlen %d")
TRACE_MSG(MIKEY_302_112_2_17_23_10_29_1234,"MIKEY_NextPayload: last payload")
TRACE_MSG(MIKEY_308_112_2_17_23_10_29_1235,"MIKEY_NextPayload: next_payload_type exceed valid range")
TRACE_MSG(MUA_CMD_183_112_2_17_23_10_32_1236,"ParseCardSW nStatus=%x")
TRACE_MSG(MUA_CMD_306_112_2_17_23_10_32_1237,"Card_SelectFile ret = %d,nStatus=%x")
TRACE_MSG(MUA_CMD_347_112_2_17_23_10_32_1238,"Card_SelectFile ret = %d,nStatus=%x")
TRACE_MSG(MUA_CMD_408_112_2_17_23_10_32_1239,"Card_GetResp ret=%d,Status = %x")
TRACE_MSG(MUA_CMD_426_112_2_17_23_10_32_1240,"Card_Case4_Cmd ret=%d,Status=%x")
TRACE_MSG(MUA_CMD_453_112_2_17_23_10_32_1241,"Card_Case4_Cmd Status = %x")
TRACE_MSG(MUA_CMD_481_112_2_17_23_10_32_1242,"Card_ReadFixedRec ret=%d,nStatus=%x")
TRACE_MSG(MUA_CMD_501_112_2_17_23_10_32_1243,"Card_Switch_2G3G nm=%d")
TRACE_MSG(MUA_CMD_526_112_2_17_23_10_32_1244,"Card_Switch_2G3G ret=%d,nStatus=%x")
TRACE_MSG(MUA_CMD_532_112_2_17_23_10_32_1245,"Card_Switch_2G3G 2g mode")
TRACE_MSG(MUA_CMD_591_112_2_17_23_10_32_1246,"Card_KeepAIDCtx ret = %d,nStatus=%x")
TRACE_MSG(MUA_CMD_612_112_2_17_23_10_33_1247,"Card_KeepAIDCtx ret = %d,nStatus=%x")
TRACE_MSG(MUA_CMD_892_112_2_17_23_10_33_1248,"MUACMD_GBABS_3G res_len=%d,ck_len=%d,ik_len=%d,rand_len=%d,data_len=%d")
TRACE_MSG(MUA_CMD_900_112_2_17_23_10_33_1249,"GBA cmd_data[%d]=0x%x")
TRACE_MSG(MUA_CMD_1023_112_2_17_23_10_33_1250,"MUACMD_GBABS_2G res_len=%d,kc_len=%d,ks_input_len=%d,rand_len=%d,data_len=%d")
TRACE_MSG(MUA_CMD_1031_112_2_17_23_10_33_1251,"GBA cmd_data[%d]=0x%x")
TRACE_MSG(MUA_CMD_1037_112_2_17_23_10_33_1252,"MBBMS MUACMD_GBABS_2G: ret:%d \n")
TRACE_MSG(MUA_CMD_1049_112_2_17_23_10_33_1253,"MBBMS MUACMD_GBABS_2G res len %d \n")
TRACE_MSG(MUA_CMD_1057_112_2_17_23_10_33_1254,"MBBMS MUACMD_GBABS_2G cnonce len %d \n")
TRACE_MSG(MUA_CMD_1121_112_2_17_23_10_34_1255,"MBBMS MUACMD_GBA_NAF First ok \n")
TRACE_MSG(MUA_CMD_1164_112_2_17_23_10_34_1256,"MUACMD_GBA_NAF naf_id_len=%d,impi_len=%d,data_len=%d")
TRACE_MSG(MUA_CMD_1170_112_2_17_23_10_34_1257,"NAF cmd_data[%d]=0x%x")
TRACE_MSG(MUA_CMD_1176_112_2_17_23_10_34_1258,"MBBMS MUACMD_GBA_NAF second ok\n")
TRACE_MSG(MUA_CMD_1185_112_2_17_23_10_34_1259,"MUACMD_GBA_NAF data_len=%d")
TRACE_MSG(MUA_CMD_1224_112_2_17_23_10_34_1260,"MUACMD_MBMS MAX LEN:%d \n")
TRACE_MSG(MUA_CMD_1230_112_2_17_23_10_34_1261,"MUACMD_MBMS SCI_GetMutex ok \n")
TRACE_MSG(MUA_CMD_1234_112_2_17_23_10_34_1262,"MUACMD_MBMS mikey len:%d type:%d \n")
TRACE_MSG(MUA_CMD_1241_112_2_17_23_10_34_1263,"DATA[%d]: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x n")
TRACE_MSG(MUA_CMD_1286_112_2_17_23_10_34_1264,"MBBMS MSK UPDATA CMD RECEIVED \n")
TRACE_MSG(MUA_CMD_1291_112_2_17_23_10_34_1265,"MBBMS MTK UPDATA CMD RECEIVED \n")
TRACE_MSG(MUA_CMD_1319_112_2_17_23_10_34_1266,"MBMS cmd_data[%d]=0x%x")
TRACE_MSG(MUA_CMD_1325_112_2_17_23_10_34_1267,"MUACMD_MBMS() apdu_data=%d,ret=%d,nResLen=%d")
TRACE_MSG(MUA_CMD_1331_112_2_17_23_10_34_1268,"MUACMD_MBMS Result len:%d data:0x%x %x %x %x %x n")
TRACE_MSG(MUA_CMD_1332_112_2_17_23_10_34_1269,"MUACMD_MBMS input max len:%d \n")
TRACE_MSG(MUA_CMD_1789_112_2_17_23_10_35_1270,"MUACMD_IsGBANeeded() resLen = %d")
TRACE_MSG(MUA_CMD_2044_112_2_17_23_10_36_1271,"MBBMS MUACMD_GetSDVersion [%s]\n ")
TRACE_MSG(MUA_TEST_132_112_2_17_23_10_36_1272,"MUACMD_MBBMS_GetCMMBSN()")
TRACE_MSG(MUA_TEST_151_112_2_17_23_10_36_1273,"MUACMD_MBBMS_GetCMMBSN()ret=%d")
TRACE_MSG(MUA_TEST_154_112_2_17_23_10_36_1274,"sn[%d]=%d")
TRACE_MSG(MUA_TEST_158_112_2_17_23_10_36_1275,"MUACMD_GetSDVersion()ret=%d,v0=%x,v1=%x,v2=%x,v3=%x")
TRACE_MSG(MUA_TEST_160_112_2_17_23_10_36_1276,"MUACMD_GetSDID()ret=%d,id0=%x,id1=%d,id2=%d,id3=%d,id4=%d,id5=%d")
TRACE_MSG(MUA_TEST_204_112_2_17_23_10_36_1277,"DoGBAproc_3G")
TRACE_MSG(MUA_TEST_255_112_2_17_23_10_36_1278,"MUACMD_GBABS_3G()ret=%d")
TRACE_MSG(MUA_TEST_270_112_2_17_23_10_36_1279,"ME--------->TF  B-TID: %s\r\n")
TRACE_MSG(MUA_TEST_279_112_2_17_23_10_36_1280,"ME--------->TF  ks: %s\r\n")
TRACE_MSG(MUA_TEST_339_112_2_17_23_10_37_1281,"DoGBAproc_3G_smd")
TRACE_MSG(MUA_TEST_370_112_2_17_23_10_37_1282,"DoGBAproc_3G_smd()ret=%d")
TRACE_MSG(MUA_TEST_404_112_2_17_23_10_37_1283,"DoGBAproc_2G")
TRACE_MSG(MUA_TEST_435_112_2_17_23_10_37_1284,"MUACMD_GBABS_2G()ret=%d")
TRACE_MSG(MUA_TEST_468_112_2_17_23_10_37_1285,"MUACMD_GBA_NAF ret=%d,resp_data.data_length=%d")
TRACE_MSG(MUA_TEST_497_112_2_17_23_10_37_1286,"MUACMD_MBMS ret=%d,resp_data.data_length=%d")
TRACE_MSG(SDP_326_112_2_17_23_10_38_1287,"ParseSDP param error!")
TRACE_MSG(SDP_412_112_2_17_23_10_38_1288,"[SG]: SetSdpAttributeValue, cannot value\n")
TRACE_MSG(SDP_424_112_2_17_23_10_38_1289,"[SG]: SetSdpAttributeValue, ISMACrypCryptoSuite unknow type")
TRACE_MSG(SDP_457_112_2_17_23_10_38_1290,"[SG]: SetSdpAttributeValue, GetSaltKey error")
TRACE_MSG(SDP_463_112_2_17_23_10_38_1291,"[SG]: SetSdpAttributeValue, base64 len=%d")
TRACE_MSG(SDP_466_112_2_17_23_10_38_1292,"[SG]: SetSdpAttributeValue, base64= %s")
TRACE_MSG(SDP_469_112_2_17_23_10_38_1293,"[SG]: SetSdpAttributeValue, ISMACrypSalt decode error")
TRACE_MSG(SDP_473_112_2_17_23_10_38_1294,"[SG]: SetSdpAttributeValue, outlen=%d")
TRACE_MSG(SDP_482_112_2_17_23_10_38_1295,"[SG]: SetSdpAttributeValue, outbuf[%03d] = %02x %02x %02x %02x %02x %02x %02x %02x")
TRACE_MSG(SDP_493_112_2_17_23_10_38_1296,"[SG]: SetSdpAttributeValue, ISMACrypSalt not base64")
TRACE_MSG(SDP_501_112_2_17_23_10_38_1297,"[SG]: SetSdpAttributeValue, ISMACrypSalt has no dot")
TRACE_MSG(SDP_510_112_2_17_23_10_38_1298,"[SG]: SetSdpAttributeValue, GetSaltKey error")
TRACE_MSG(SDP_513_112_2_17_23_10_38_1299,"[SG]: SetSdpAttributeValue, base64 len=%d")
TRACE_MSG(SDP_516_112_2_17_23_10_38_1300,"[SG]: SetSdpAttributeValue, base64= %s")
TRACE_MSG(SDP_519_112_2_17_23_10_38_1301,"[SG]: SetSdpAttributeValue, USE BASE64 Decode error")
TRACE_MSG(SDP_523_112_2_17_23_10_38_1302,"[SG]: SetSdpAttributeValue, outlen=%d")
TRACE_MSG(SDP_532_112_2_17_23_10_38_1303,"[SG]: SetSdpAttributeValue, outbuf[%03d] = %02x %02x %02x %02x %02x %02x %02x %02x")
TRACE_MSG(SDP_600_112_2_17_23_10_39_1304,"[SG]: MBBMSSDP_CreateIsma enter")
TRACE_MSG(SDP_604_112_2_17_23_10_39_1305,"[SG]: MBBMSSDP_CreateIsma, no serviceid, cmmb_frqNo %d,cmmb_serviceId %dn")
TRACE_MSG(SDP_612_112_2_17_23_10_39_1306,"[SG]: MBBMSSDP_CreateIsma, no sdp \n")
TRACE_MSG(SDP_625_112_2_17_23_10_39_1307,"[SG]: MBBMSSDP_CreateIsma, no sdp content string\n")
TRACE_MSG(SDP_634_112_2_17_23_10_39_1308,"[SG]: MBBMSSDP_CreateIsma, no match access, service id %s n")
TRACE_MSG(SDP_658_112_2_17_23_10_39_1309,"[SG]: MBBMSSDP_CreateIsma, no serviceid, cmmb_frqNo %d,cmmb_serviceId %dn")
TRACE_MSG(SDP_666_112_2_17_23_10_39_1310,"[SG]: MBBMSSDP_CreateIsma, no sdp \n")
TRACE_MSG(SDP_679_112_2_17_23_10_39_1311,"[SG]: MBBMSSDP_CreateIsma, no sdp content string\n")
TRACE_MSG(SDP_688_112_2_17_23_10_39_1312,"[SG]: MBBMSSDP_CreateIsma, no match access, service id %s n")
TRACE_MSG(SDUA_DRV_244_112_2_17_23_10_39_1313,"SDUA_Direct_Read not correct header")
TRACE_MSG(SDUA_DRV_252_112_2_17_23_10_39_1314,"SDUA_Direct_Read need retry read")
TRACE_MSG(SDUA_DRV_271_112_2_17_23_10_39_1315,"SDUA_Direct_Read return data too large")
TRACE_MSG(SDUA_DRV_315_112_2_17_23_10_39_1316,"SDUA_Reset write error\n")
TRACE_MSG(SDUA_DRV_325_112_2_17_23_10_39_1317,"SDUA_Reset read error\n")
TRACE_MSG(SDUA_DRV_331_112_2_17_23_10_39_1318,"SDUA_Reset return Size error. ret Size = %d\n")
TRACE_MSG(SDUA_DRV_337_112_2_17_23_10_39_1319,"out[%d]=%x")
TRACE_MSG(SDUA_DRV_351_112_2_17_23_10_40_1320,"SDUA_Reset return nRet=%d")
TRACE_MSG(SDUA_DRV_400_112_2_17_23_10_40_1321,"cmd sdua size=0x%x")
TRACE_MSG(SDUA_DRV_405_112_2_17_23_10_40_1322,"cmd sdua %02x %02x %02x %02x %02x")
TRACE_MSG(SDUA_DRV_411_112_2_17_23_10_40_1323,"cmd sdua %02x")
TRACE_MSG(SDUA_DRV_436_112_2_17_23_10_40_1324,"SDUA_IoControl()*o_nSize=%d")
TRACE_MSG(SDUA_DRV_443_112_2_17_23_10_40_1325,"resp sdua size=0x%x,resp=0x%x")
TRACE_MSG(SDUA_DRV_450_112_2_17_23_10_40_1326,"resp sdua %02x %02x %02x %02x %02x")
TRACE_MSG(SDUA_DRV_456_112_2_17_23_10_40_1327,"resp sdua %02x")
TRACE_MSG(SDUA_DRV_510_112_2_17_23_10_40_1328,"cmd sipua size=0x%x")
TRACE_MSG(SDUA_DRV_515_112_2_17_23_10_40_1329,"cmd sipua %02x %02x %02x %02x %02x")
TRACE_MSG(SDUA_DRV_521_112_2_17_23_10_40_1330,"cmd sipua %02x")
TRACE_MSG(SDUA_DRV_528_112_2_17_23_10_40_1331,"resp sipua size=0x%x,resp=0x%x")
TRACE_MSG(SDUA_DRV_535_112_2_17_23_10_40_1332,"resp sipua %02x %02x %02x %02x %02x")
TRACE_MSG(SDUA_DRV_541_112_2_17_23_10_40_1333,"resp sipua %02x")
TRACE_MSG(UA_DRV_76_112_2_17_23_10_40_1334,"cmd uam size=0x%x")
TRACE_MSG(UA_DRV_81_112_2_17_23_10_40_1335,"cmd uam %02x %02x %02x %02x %02x")
TRACE_MSG(UA_DRV_87_112_2_17_23_10_40_1336,"cmd uam %02x")
TRACE_MSG(UA_DRV_93_112_2_17_23_10_40_1337,"resp uam size=0x%x,resp=0x%x")
TRACE_MSG(UA_DRV_100_112_2_17_23_10_40_1338,"resp uam %02x %02x %02x %02x %02x")
TRACE_MSG(UA_DRV_106_112_2_17_23_10_40_1339,"resp uam %02x")
TRACE_MSG(CA_ADAPTER_112_112_2_17_23_10_41_1340,"TM_GetSysDate Failed!!!\n")
TRACE_MSG(CA_ADAPTER_119_112_2_17_23_10_41_1341,"TM_GetSysTime FAILED!!!\n")
TRACE_MSG(CA_ADAPTER_189_112_2_17_23_10_41_1342,"MBBMS GetMTKIDFromMIKEY MSKID = 0x%x MTKID = 0x%x DOMAINID = 0x%x")
TRACE_MSG(CA_ADAPTER_200_112_2_17_23_10_41_1343,"MBBMS GetMTKIDFromMIKEY MIKEY_Open failed")
TRACE_MSG(CA_ADAPTER_219_112_2_17_23_10_41_1344,"MBBMS GetVerDataFromMIKEY %s")
TRACE_MSG(CA_ADAPTER_229_112_2_17_23_10_41_1345,"MBBMS GetVerDataFromMIKEY verdata = 0x%x 0x%x n")
TRACE_MSG(CA_ADAPTER_239_112_2_17_23_10_41_1346,"MBBMS GetVerDataFromMIKEY MIKEY_Open failed")
TRACE_MSG(CA_ADAPTER_273_112_2_17_23_10_41_1347,"MBBMS GetECMDataType PARSING CAT ......\n")
TRACE_MSG(CA_ADAPTER_292_112_2_17_23_10_41_1348,"MBBSM GetECMDataType CA SYSTEM ID:0x%x\n")
TRACE_MSG(CA_ADAPTER_298_112_2_17_23_10_41_1349,"MBBMS GetECMDataType data_type:0x%x ecm_transport_type:0x%xn")
TRACE_MSG(CA_ADAPTER_340_112_2_17_23_10_41_1350,"KDA CSL:cslFilterSetPatterns MTV_STIMI_DATA_128BYTE_FILTER\n")
TRACE_MSG(CA_ADAPTER_352_112_2_17_23_10_41_1351,"KDA CSL:cslFilterSetPatterns MTV_STIMI_DATA_16BYTE_FILTER\n")
TRACE_MSG(CA_ADAPTER_376_112_2_17_23_10_41_1352,"MBBMS ecm filter word 0x%X")
TRACE_MSG(CA_ADAPTER_399_112_2_17_23_10_41_1353,"MBBMS EnableECMFilter Filter Enable OK filter_id:%d Sub_type:%d data_type:%dn")
TRACE_MSG(CA_ADAPTER_419_112_2_17_23_10_41_1354,"MBBMS SetIsma element_id %d\n")
TRACE_MSG(CA_ADAPTER_424_112_2_17_23_10_41_1355,"MBBMS SetIsma ismacrypData.salt[%d] %d\n")
TRACE_MSG(CA_ADAPTER_434_112_2_17_23_10_41_1356,"MBBMS SetIsma rtpData.auSizeLengthType %x\n")
TRACE_MSG(CA_ADAPTER_447_112_2_17_23_10_41_1357,"MBBMS SetIsma unknown type %d")
TRACE_MSG(CA_ADAPTER_463_112_2_17_23_10_41_1358,"MBBMS SetIsma isma_data[i] %x\n")
TRACE_MSG(CA_ADAPTER_471_112_2_17_23_10_41_1359,"MBBMS CADSX_SetISMA element_type %d\n")
TRACE_MSG(CA_ADAPTER_484_112_2_17_23_10_41_1360,"MBBMS CADSX_SetISMA unknown element_type %d")
TRACE_MSG(CA_ADAPTER_494_112_2_17_23_10_41_1361,"MBBMS Enter ISMACallback 0x%X")
TRACE_MSG(CA_ADAPTER_544_112_2_17_23_10_42_1362,"MBBMS ECMRecv ECM Received")
TRACE_MSG(CA_ADAPTER_568_112_2_17_23_10_42_1363,"MBBMS ECMRecv Filter %d len = %d Send Over [%d]")
TRACE_MSG(CA_ADAPTER_604_112_2_17_23_10_42_1364,"MBBMS MIKEY RESPONSE RECEIVED cause:%d mikey_type:%d key_len:%d n")
TRACE_MSG(CA_ADAPTER_614_112_2_17_23_10_42_1365,"MBBMS MTK DECRYPTED 0x%x %x %x %x %x %x %x %x n")
TRACE_MSG(CA_ADAPTER_625_112_2_17_23_10_42_1366,"MBBMS MTK sendMMI signal")
TRACE_MSG(CA_ADAPTER_659_112_2_17_23_10_42_1367,"KDA DSX:dsxSetKeys  key_index%d\n")
TRACE_MSG(CA_ADAPTER_692_112_2_17_23_10_42_1368," ---MBBMS CA: ALREADY RUNNING\n")
TRACE_MSG(CA_ADAPTER_720_112_2_17_23_10_42_1369,"Create ca adapter thread failed!!")
TRACE_MSG(CA_ADAPTER_726_112_2_17_23_10_42_1370," ---MBBMS CA: BEGIN TO RUN ....   \n")
TRACE_MSG(CA_ADAPTER_740_112_2_17_23_10_42_1371," ---KDA CA:ENTRY TERMINATED!   \n")
TRACE_MSG(CA_ADAPTER_751_112_2_17_23_10_42_1372," ---KDA CA: TERMINATED!   \n")
TRACE_MSG(CA_ADAPTER_759_112_2_17_23_10_42_1373," KDA ADPT: ca_play_callback result:%d \n")
TRACE_MSG(CA_ADAPTER_781_112_2_17_23_10_42_1374,"MBBMS Set Media Type:%d \n")
TRACE_MSG(CA_ADAPTER_792_112_2_17_23_10_42_1375,"MBBMS ECM content: %s")
TRACE_MSG(CA_ADAPTER_796_112_2_17_23_10_42_1376,"MBBMS ECM content: %s")
TRACE_MSG(CA_ADAPTER_816_112_2_17_23_10_42_1377,"MBBMS adapter enter thread")
TRACE_MSG(CA_ADAPTER_824_112_2_17_23_10_42_1378,"MBBMS adapter receive %d")
TRACE_MSG(CA_ADAPTER_832_112_2_17_23_10_42_1379,"MBBMS ECM msg Received %d[%d]")
TRACE_MSG(CA_ADAPTER_851_112_2_17_23_10_42_1380,"MBBMS MIKEY %d is NULL")
TRACE_MSG(CA_ADAPTER_854_112_2_17_23_10_42_1381,"MBBMS s_current_key_indicator 0x%X key_ind 0x%X[%d]")
TRACE_MSG(CA_ADAPTER_896_112_2_17_23_10_42_1382,"MBBMS key indicator %x [%d]")
TRACE_MSG(CA_ADAPTER_911_112_2_17_23_10_42_1383,"MBBMS MTK error %d [%d]")
TRACE_MSG(CA_ADAPTER_919_112_2_17_23_10_43_1384,"MBBMS mtk_id 0x%X domainid 0x%X[%d]")
TRACE_MSG(CA_ADAPTER_922_112_2_17_23_10_43_1385,"MBBMS MTK error %d [%d]")
TRACE_MSG(CA_ADAPTER_932_112_2_17_23_10_43_1386,"MBBMS ECM_GetInfo error")
TRACE_MSG(CA_ADAPTER_954_112_2_17_23_10_43_1387,"CA_Adapter_Thread: unknown msg id %d!")
TRACE_MSG(CA_ADAPTER_981_112_2_17_23_10_43_1388,"MBBMS CAT SHOWING DATA:\n")
TRACE_MSG(CA_ADAPTER_983_112_2_17_23_10_43_1389,"MBBMS CAT No:%d Value:0x%x\n")
TRACE_MSG(CA_ADAPTER_1042_112_2_17_23_10_43_1390,"MBBMS HandleISMAReqSig \n")
TRACE_MSG(CA_ADAPTER_1095_112_2_17_23_10_43_1391,"KDA DSX:dsxConfigure initializationVectorLength %x\n")
TRACE_MSG(CA_ADAPTER_1096_112_2_17_23_10_43_1392,"KDA DSX:dsxConfigure deltaInitializationVectorLength %x\n")
TRACE_MSG(CA_ADAPTER_1097_112_2_17_23_10_43_1393,"KDA DSX:dsxConfigure keyIndicatorLength %x\n")
TRACE_MSG(CA_ADAPTER_1098_112_2_17_23_10_43_1394,"KDA DSX:dsxConfigure useKeyIndicatorPerAu %x\n")
TRACE_MSG(CA_ADAPTER_1099_112_2_17_23_10_43_1395,"KDA DSX:dsxConfigure useSelectiveEncryption %x\n")
TRACE_MSG(CA_ADAPTER_1101_112_2_17_23_10_43_1396,"KDA DSX:dsxConfigure auSizeLengthType length%x\n")
TRACE_MSG(CA_ADAPTER_1133_112_2_17_23_10_43_1397,"KDA DSX:dsxConfigure useRandomAccessFlag %x\n")
TRACE_MSG(CA_ADAPTER_1134_112_2_17_23_10_43_1398,"KDA DSX:dsxConfigure streamStateLength %x\n")
TRACE_MSG(CA_ADAPTER_1135_112_2_17_23_10_43_1399,"KDA DSX:dsxConfigure indexDeltaLength %x\n")
TRACE_MSG(CA_ADAPTER_1136_112_2_17_23_10_43_1400,"KDA DSX:dsxConfigure indexLength%x\n")
TRACE_MSG(CA_ADAPTER_1137_112_2_17_23_10_43_1401,"KDA DSX:dsxConfigure auxiliaryDataSizeLength %x\n")
TRACE_MSG(CA_ADAPTER_1138_112_2_17_23_10_43_1402,"KDA DSX:dsxConfigure ctsDeltaLength %x\n")
TRACE_MSG(CA_ADAPTER_1139_112_2_17_23_10_43_1403,"KDA DSX:dsxConfigure useRandomAccessFlag %x\n")
TRACE_MSG(CA_ADAPTER_1145_112_2_17_23_10_43_1404,"KDA DSX:HandleISMAReqSig isma_data[i] %x\n")
TRACE_MSG(CA_ADAPTER_1172_112_2_17_23_10_43_1405," MBBMS HandleDescramKeyReqSig key_select%x,key_id%x \n")
TRACE_MSG(CA_ADAPTER_1197_112_2_17_23_10_43_1406,"KDA DSX :HandleDescramKeyReqSig xSetNumKeys%x \n")
TRACE_MSG(CA_ADAPTER_1201_112_2_17_23_10_43_1407,"KDA DSX :HandleDescramKeyReqSig key[%d]%x \n")
TRACE_MSG(CA_ADAPTER_1221_112_2_17_23_10_43_1408,"KDA DSX :HandleDescramKeyReqSig key index%x \n")
TRACE_MSG(CA_ADAPTER_1229_112_2_17_23_10_43_1409,"KDA DSX :HandleDescramKeyReqSig don't get key key_rcv_demux_cmd_ref 0x%x\n")
TRACE_MSG(CA_ADAPTER_1246_112_2_17_23_10_43_1410,"MBBMS ProcessDemuxReqCallback \n")
TRACE_MSG(CA_ADAPTER_1263_112_2_17_23_10_43_1411,"MBBMS KDA CSL:time ProcessDemuxReqCallback tick%x\n")
TRACE_MSG(CA_ADAPTER_1264_112_2_17_23_10_43_1412,"MBBMS_KDA DSX: :ProcessDemuxReqCallback %x\n")
TRACE_MSG(CA_ADAPTER_1281_112_2_17_23_10_43_1413,"MBBMS Received UnKNOWN DEMUX REQ CALLBACK \n")
TRACE_MSG(CA_ADAPTER_1472_112_2_17_23_10_44_1414,"GetISMA: serviceid(%d) is not 601 or 602")
TRACE_MSG(CA_ADAPTER_1494_112_2_17_23_10_44_1415,"CADSX_GetISMA: service_id %d frq_no %d")
TRACE_MSG(CA_ADAPTER_1573_112_2_17_23_10_44_1416,"MBBMS ProcessDemuxData,no enough memory %d!")
TRACE_MSG(CA_KEY_93_112_2_17_23_10_44_1417,"CA_Key_Push: %X")
TRACE_MSG(CA_KEY_113_112_2_17_23_10_44_1418,"CA_Key_Push: error keycount %d")
TRACE_MSG(CA_KEY_125_112_2_17_23_10_44_1419,"CA_Key_Get: %X %d")
TRACE_MSG(CA_KEY_131_112_2_17_23_10_44_1420,"CA_Key_Get: find key")
TRACE_MSG(ECM_40_112_2_17_23_10_44_1421,"MBBMS GetMikeyValidLength %d")
TRACE_MSG(ECM_43_112_2_17_23_10_44_1422,"MBBMS GetMikeyValidLength MIKEY_GetCurrentPayloadLength failed")
TRACE_MSG(ECM_54_112_2_17_23_10_44_1423,"MBBMS GetMikeyValidLength MIKEY_Open failed")
TRACE_MSG(CA_ADAPTER_80_112_2_17_23_10_45_1424,"MBBMS GetECMDataType PARSING CAT ......\n")
TRACE_MSG(CA_ADAPTER_95_112_2_17_23_10_45_1425,"MBBMS GetECMDataType PARSING CAT ......\n")
TRACE_MSG(CA_ADAPTER_103_112_2_17_23_10_45_1426,"MBBSM GetECMDataType CA SYSTEM ID:0x%x\n")
TRACE_MSG(CA_ADAPTER_109_112_2_17_23_10_45_1427,"MBBMS GetECMDataType data_type:0x%x ecm_transport_type:0x%xn")
TRACE_MSG(CA_ADAPTER_145_112_2_17_23_10_45_1428," ---MBBMS CA: ALREADY RUNNING\n")
TRACE_MSG(CA_ADAPTER_153_112_2_17_23_10_45_1429," ---MBBMS CA: BEGIN TO RUN ....   \n")
TRACE_MSG(CA_ADAPTER_165_112_2_17_23_10_45_1430," ---KDA CA:ENTRY TERMINATED!   \n")
TRACE_MSG(CA_ADAPTER_172_112_2_17_23_10_45_1431," ---KDA CA: TERMINATED!   \n")
TRACE_MSG(CA_ADAPTER_190_112_2_17_23_10_45_1432,"MBBMS Set Media Type:%d \n")
TRACE_MSG(CA_ADAPTER_209_112_2_17_23_10_45_1433,"MBBMS CAT SHOWING DATA:\n")
TRACE_MSG(CA_ADAPTER_212_112_2_17_23_10_45_1434,"MBBMS CAT No:%d Value:0x%x\n")
TRACE_MSG(CA_ADAPTER_218_112_2_17_23_10_45_1435,"MBBMS: cannot find ecm data type by system id %x")
TRACE_MSG(CA_ADAPTER_277_112_2_17_23_10_45_1436,"CADSX_GetISMA: service_id %d frq_no %d")
TRACE_MSG(CA_ADAPTER_299_112_2_17_23_10_45_1437,"MBBMS CA_Error_Callback: errcode %d [%d]")
TRACE_MSG(CA_ADAPTER_306_112_2_17_23_10_45_1438,"MBBMS CA_Error_Callback: ca_mw_callback is null!")
TRACE_MSG(CA_ADAPTER_311_112_2_17_23_10_45_1439,"MBBMS CA_Error_Callback: state error")
TRACE_MSG(MULTIMEDIA_AL_108_112_2_17_23_10_46_1440,"Wait MainThread Exit")
TRACE_MSG(MULTIMEDIA_AL_130_112_2_17_23_10_46_1441,"MultimediaAL_Play[%d]")
TRACE_MSG(MULTIMEDIA_AL_146_112_2_17_23_10_46_1442,"MultimediaAL_Stop[%d]")
TRACE_MSG(MULTIMEDIA_AL_191_112_2_17_23_10_46_1443,"MultimediaAL_GetCurrentFrameData: %d %dx%d")
TRACE_MSG(MULTIMEDIA_AL_212_112_2_17_23_10_46_1444,"MultimediaAL_GetFrameSize: %d %d")
TRACE_MSG(MULTIMEDIA_AL_346_112_2_17_23_10_46_1445,"DPLAYER_StreamRxStart failed!!!")
TRACE_MSG(MULTIMEDIA_AL_379_112_2_17_23_10_46_1446,"*******Enter CalbackFrameEndNotify %d %d[%d]")
TRACE_MSG(MULTIMEDIA_AL_482_112_2_17_23_10_47_1447,"Display_PARAM: w %d, h %d, angle %d")
TRACE_MSG(MULTIMEDIA_AL_559_112_2_17_23_10_47_1448,"Wait FrameUpdateThread Exit")
TRACE_MSG(MULTIMEDIA_AL_585_112_2_17_23_10_47_1449,"MMAL_MainThread: Recv %x[%d]")
TRACE_MSG(MULTIMEDIA_AL_609_112_2_17_23_10_47_1450,"MMAL_MainThread unknow message %d")
TRACE_MSG(MULTIMEDIA_AL_612_112_2_17_23_10_47_1451,"MMALMEM: main clean %p")
TRACE_MSG(MULTIMEDIA_AL_633_112_2_17_23_10_47_1452,"MMAL_UpdateThread: Recv %x[%d]")
TRACE_MSG(MULTIMEDIA_AL_644_112_2_17_23_10_47_1453,"MMAL_MainThread unknow message %d")
TRACE_MSG(MULTIMEDIA_AL_647_112_2_17_23_10_47_1454,"MMALMEM: update clean %p")
TRACE_MSG(MULTIMEDIA_AL_681_112_2_17_23_10_47_1455,"*******MultimediaAL Init failed")
TRACE_MSG(MULTIMEDIA_AL_686_112_2_17_23_10_47_1456,"*******MultimediaAL Play failed")
TRACE_MSG(MULTIMEDIA_AL_694_112_2_17_23_10_47_1457,"*******Open file for read error")
TRACE_MSG(MULTIMEDIA_AL_711_112_2_17_23_10_47_1458,"*******Read header error")
TRACE_MSG(MULTIMEDIA_AL_716_112_2_17_23_10_47_1459,"*******Read header length error")
TRACE_MSG(MULTIMEDIA_AL_727_112_2_17_23_10_47_1460,"*******GetBuffer failed")
TRACE_MSG(MULTIMEDIA_AL_736_112_2_17_23_10_47_1461,"*******Read frame error")
TRACE_MSG(MULTIMEDIA_AL_740_112_2_17_23_10_47_1462,"*******Read frame length error")
TRACE_MSG(MULTIMEDIA_AL_788_112_2_17_23_10_47_1463,"*******Open file for write error")
TRACE_MSG(MULTIMEDIA_AL_793_112_2_17_23_10_47_1464,"*******Write file error")
TRACE_MSG(MULTIMEDIA_AL_812_112_2_17_23_10_47_1465,"*******CreateThread failed")
TRACE_MSG(MULTIMEDIA_IL_23_112_2_17_23_10_47_1466,"*******Open file for write error")
TRACE_MSG(MULTIMEDIA_IL_30_112_2_17_23_10_47_1467,"*******Write file error")
TRACE_MSG(MULTIMEDIA_IL_34_112_2_17_23_10_47_1468,"*******Write file length error")
TRACE_MSG(MULTIMEDIA_IL_203_112_2_17_23_10_48_1469,"MMIL GetBuffer: when write buffer empty, read buffer should be available!!!")
TRACE_MSG(MULTIMEDIA_IL_283_112_2_17_23_10_48_1470,"MultimediaIL_GetFrame: should not happen!!!")
TRACE_MSG(MULTIMEDIA_IL_337_112_2_17_23_10_48_1471,"MultimediaIL_SetAudioSampleRate %d")
TRACE_MSG(MULTIMEDIA_IL_344_112_2_17_23_10_48_1472,"MultimediaIL_GetLastOutputFramePTS %d")
TRACE_MSG(SEQUENCERWBUFFER_53_112_2_17_23_10_48_1473,"SequenceRWBuffer: IsReadWriteSameSide null handle")
TRACE_MSG(SEQUENCERWBUFFER_68_112_2_17_23_10_48_1474,"SequenceRWBuffer: IsGetWriteSameSide null handle")
TRACE_MSG(SEQUENCERWBUFFER_92_112_2_17_23_10_48_1475,"SequenceRWBuffer: GetWritableRange null handle")
TRACE_MSG(SEQUENCERWBUFFER_107_112_2_17_23_10_48_1476,"SequenceRWBuffer: FlipReadToStart null handle")
TRACE_MSG(SEQUENCERWBUFFER_121_112_2_17_23_10_48_1477,"SequenceRWBuffer: FlipGetToStart null handle")
TRACE_MSG(SEQUENCERWBUFFER_135_112_2_17_23_10_48_1478,"SequenceRWBuffer: FlipWriteToStart null handle")
TRACE_MSG(SEQUENCERWBUFFER_149_112_2_17_23_10_48_1479,"SequenceRWBuffer: MarkBufferEnd null handle")
TRACE_MSG(SEQUENCERWBUFFER_164_112_2_17_23_10_48_1480,"SequenceRWBuffer: IsReadBufferEnd null handle")
TRACE_MSG(SEQUENCERWBUFFER_180_112_2_17_23_10_48_1481,"SequenceRWBuffer: IsGetBufferEnd null handle")
TRACE_MSG(SEQUENCERWBUFFER_196_112_2_17_23_10_48_1482,"SequenceRWBuffer: IsGetBufferEmpty null handle")
TRACE_MSG(SEQUENCERWBUFFER_218_112_2_17_23_10_49_1483,"SequenceRWBuffer: Init alloc fail")
TRACE_MSG(SEQUENCERWBUFFER_223_112_2_17_23_10_49_1484,"SequenceRWBuffer: Init null base_ptr")
TRACE_MSG(SEQUENCERWBUFFER_239_112_2_17_23_10_49_1485,"SequenceRWBuffer: Exit null handle")
TRACE_MSG(SEQUENCERWBUFFER_253_112_2_17_23_10_49_1486,"SequenceRWBuffer: Reset null handle")
TRACE_MSG(SEQUENCERWBUFFER_265_112_2_17_23_10_49_1487,"SequenceRWBuffer: GetReadBuffer start %p")
TRACE_MSG(SEQUENCERWBUFFER_278_112_2_17_23_10_49_1488,"SequenceRWBuffer: GetReadBuffer %08x")
TRACE_MSG(SEQUENCERWBUFFER_291_112_2_17_23_10_49_1489,"SequenceRWBuffer: GetReadBuffer end %p, %d")
TRACE_MSG(SEQUENCERWBUFFER_296_112_2_17_23_10_49_1490,"SequenceRWBuffer: GetReadBuffer null handle")
TRACE_MSG(SEQUENCERWBUFFER_312_112_2_17_23_10_49_1491,"SequenceRWBuffer: GetWriteBuffer start %p, %d")
TRACE_MSG(SEQUENCERWBUFFER_317_112_2_17_23_10_49_1492,"SequenceRWBuffer: range %d")
TRACE_MSG(SEQUENCERWBUFFER_350_112_2_17_23_10_49_1493,"SequenceRWBuffer: GetWriteBuffer end %p")
TRACE_MSG(SEQUENCERWBUFFER_355_112_2_17_23_10_49_1494,"SequenceRWBuffer: GetWriteBuffer null handle")
TRACE_MSG(SEQUENCERWBUFFER_369_112_2_17_23_10_49_1495,"SequenceRWBuffer: FinishUseReadBuffer start %p, %p")
TRACE_MSG(SEQUENCERWBUFFER_393_112_2_17_23_10_49_1496,"SequenceRWBuffer: FinishUseReadBuffer end")
TRACE_MSG(SEQUENCERWBUFFER_398_112_2_17_23_10_49_1497,"SequenceRWBuffer: FinishUseWriteBuffer null handle")
TRACE_MSG(SEQUENCERWBUFFER_409_112_2_17_23_10_49_1498,"SequenceRWBuffer: FinishUseWriteBuffer start %p, %d")
TRACE_MSG(SEQUENCERWBUFFER_417_112_2_17_23_10_49_1499,"SequenceRWBuffer: error MAGIC_NUM!!!")
TRACE_MSG(SEQUENCERWBUFFER_422_112_2_17_23_10_49_1500,"SequenceRWBuffer: length should less or equal than the writable range!!!")
TRACE_MSG(SEQUENCERWBUFFER_431_112_2_17_23_10_49_1501,"SequenceRWBuffer: FinishUseWriteBuffer end")
TRACE_MSG(SEQUENCERWBUFFER_436_112_2_17_23_10_49_1502,"SequenceRWBuffer: FinishUseWriteBuffer null handle")
TRACE_MSG(SEQUENCERWBUFFER_475_112_2_17_23_10_49_1503,"SequenceRWBuffer: SequenceRWBuffer_RemoveFirstReadBuffer null handle")
TRACE_MSG(SEQUENCERWBUFFER_490_112_2_17_23_10_49_1504,"SequenceRWBuffer: SequenceRWBuffer_IsAllReadReleased null handle")
TRACE_MSG(APDAC_51_112_2_17_23_10_49_1505,"APDAC_Init")
TRACE_MSG(APDAC_74_112_2_17_23_10_49_1506,"APDAC_Open")
TRACE_MSG(CA_ADAPTER_108_112_2_17_23_10_50_1507,"@@@@ add enter \n")
TRACE_MSG(CA_ADAPTER_113_112_2_17_23_10_50_1508,"@@@@ NOT add enter \n")
TRACE_MSG(CA_ADAPTER_157_112_2_17_23_10_50_1509,"TM_GetSysDate Failed!!!\n")
TRACE_MSG(CA_ADAPTER_164_112_2_17_23_10_50_1510,"TM_GetSysTime FAILED!!!\n")
TRACE_MSG(CA_ADAPTER_216_112_2_17_23_10_50_1511," KDA number smartcard alarms:%d xNumberOfAccesAlarms:%dn")
TRACE_MSG(CA_ADAPTER_219_112_2_17_23_10_50_1512,"KDA pxSmartcardAlarmArray alarm:%d changed:%d iccsessionid:%d")
TRACE_MSG(CA_ADAPTER_225_112_2_17_23_10_50_1513,"KDA pxAccessAlarmArray alarm:%d changed:%d elementstreamid:%d,iccsessionid:%d service_id:%d tssession_id:%d")
TRACE_MSG(CA_ADAPTER_230_112_2_17_23_10_50_1514,"KDA CAADAPTER: received system notification \n")
TRACE_MSG(CA_ADAPTER_236_112_2_17_23_10_50_1515,"KDA CAADAPTER: received termination notification \n")
TRACE_MSG(CA_ADAPTER_255_112_2_17_23_10_50_1516," ---KDA CA: ALREADY RUNNING\n")
TRACE_MSG(CA_ADAPTER_274_112_2_17_23_10_50_1517,"KDA ICC: I Know There Be One SD Card \n")
TRACE_MSG(CA_ADAPTER_284_112_2_17_23_10_50_1518," KDA_ICC: mcex_enter return MCEX_ERROR_DEVICE\n")
TRACE_MSG(CA_ADAPTER_287_112_2_17_23_10_50_1519," KDA_ICC: mcex_enter return: ifEnterMcex:%d \n")
TRACE_MSG(CA_ADAPTER_295_112_2_17_23_10_50_1520,"KDA ICC: I Know There Be NO SD Card! \n")
TRACE_MSG(CA_ADAPTER_301_112_2_17_23_10_50_1521,"--- KDA_ICC nagra card available \n")
TRACE_MSG(CA_ADAPTER_306_112_2_17_23_10_50_1522,"--- KDA_ICC nagra card NOT available \n")
TRACE_MSG(CA_ADAPTER_314_112_2_17_23_10_50_1523,"caInitialization finished!!")
TRACE_MSG(CA_ADAPTER_318_112_2_17_23_10_50_1524,"caInitialization failed!")
TRACE_MSG(CA_ADAPTER_372_112_2_17_23_10_50_1525,"Create ca adapter thread failed!!")
TRACE_MSG(CA_ADAPTER_393_112_2_17_23_10_50_1526," ---KDA CA: BEGIN TO RUN ....   \n")
TRACE_MSG(CA_ADAPTER_411_112_2_17_23_10_51_1527," KDA_ICC: FINDING.... T-Card NOT support CA. \n")
TRACE_MSG(CA_ADAPTER_416_112_2_17_23_10_51_1528," KDA_ICC: FINDING.... T-Card support CA. \n")
TRACE_MSG(CA_ADAPTER_422_112_2_17_23_10_51_1529," KDA_ICC: mcex_enter return MCEX_ERROR_DEVICE\n")
TRACE_MSG(CA_ADAPTER_429_112_2_17_23_10_51_1530,"KDA ICC: Set Media Type:%d \n")
TRACE_MSG(CA_ADAPTER_441_112_2_17_23_10_51_1531,"--- KDA_ICC nagra card available \n")
TRACE_MSG(CA_ADAPTER_443_112_2_17_23_10_51_1532," ---[KDA-CA][SetMediaType]: SMD IS SELECTED \n")
TRACE_MSG(CA_ADAPTER_447_112_2_17_23_10_51_1533,"--- KDA_ICC nagra card NOT available \n")
TRACE_MSG(CA_ADAPTER_466_112_2_17_23_10_51_1534," KDA_ICC: mcex_enter return MCEX_ERROR_DEVICE\n")
TRACE_MSG(CA_ADAPTER_469_112_2_17_23_10_51_1535," KDA_ICC: mcex_enter return: ifEnterMcex:%d \n")
TRACE_MSG(CA_ADAPTER_472_112_2_17_23_10_51_1536," ---[KDA-CA][SetMediaType]: MicroSD IS SELECTED \n")
TRACE_MSG(CA_ADAPTER_483_112_2_17_23_10_51_1537," ---[KDA-CA][SetMediaType]: INVALID!  \n")
TRACE_MSG(CA_ADAPTER_494_112_2_17_23_10_51_1538," ---KDA CA:ENTRY TERMINATED!   \n")
TRACE_MSG(CA_ADAPTER_497_112_2_17_23_10_51_1539,"---KDA CA: CA_Terminate gCaAdapterInited is FALSE \n")
TRACE_MSG(CA_ADAPTER_512_112_2_17_23_10_51_1540,"---KDA CA: CA_Terminate systemrequesttable is in USE \n")
TRACE_MSG(CA_ADAPTER_520_112_2_17_23_10_51_1541,"---KDA CA: CA_Terminate smartcardrequesttable is in USE\n")
TRACE_MSG(CA_ADAPTER_529_112_2_17_23_10_51_1542,"---KDA CA: CA_Terminate programrequesttable is in USE\n")
TRACE_MSG(CA_ADAPTER_542_112_2_17_23_10_51_1543,"---KDA CA: CA_Terminate RegisterationDispose num:%d Failed status:%d n")
TRACE_MSG(CA_ADAPTER_553_112_2_17_23_10_51_1544,"---KDA CA: CA_Terminate RegisterationDispose try times = %d Failed status:%d n")
TRACE_MSG(CA_ADAPTER_560_112_2_17_23_10_51_1545,"---KDA CA: CA_Terminate RegisterationDispose num:%d OK n")
TRACE_MSG(CA_ADAPTER_569_112_2_17_23_10_51_1546,"---KDA CA: CA_Terminate caTermination() return FALSE \n")
TRACE_MSG(CA_ADAPTER_584_112_2_17_23_10_51_1547," ---KDA CA: TERMINATED!   \n")
TRACE_MSG(CA_ADAPTER_591_112_2_17_23_10_51_1548," KDA ADPT: ca_play_callback result:%d \n")
TRACE_MSG(CA_ADAPTER_651_112_2_17_23_10_51_1549,"ca system get processing status FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_656_112_2_17_23_10_51_1550,"ca system get processing ERROR status  subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_661_112_2_17_23_10_51_1551,"ca system get object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_665_112_2_17_23_10_51_1552,"---CA ADapter: System get OBJECT num:%d\n")
TRACE_MSG(CA_ADAPTER_669_112_2_17_23_10_51_1553,"ca_system_private_t memory alloc FAILED!!!\n")
TRACE_MSG(CA_ADAPTER_681_112_2_17_23_10_51_1554,"ca system get CakVersin Failed subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_684_112_2_17_23_10_51_1555,"---CA ADapter: CB cak_version:%s")
TRACE_MSG(CA_ADAPTER_689_112_2_17_23_10_51_1556,"ca system dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_697_112_2_17_23_10_51_1557,"ca system get IRDSrlNO FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_705_112_2_17_23_10_51_1558,"ca system Dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_712_112_2_17_23_10_51_1559,"ca system get project info FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_719_112_2_17_23_10_51_1560,"ca system dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_734_112_2_17_23_10_51_1561,"ca smartcard get processing status FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_739_112_2_17_23_10_51_1562,"ca smartcard get processing ERROR status subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_744_112_2_17_23_10_51_1563,"ca smartcard get object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_747_112_2_17_23_10_51_1564,"---CA ADapter: Smartcard get OBJECT num:%d\n")
TRACE_MSG(CA_ADAPTER_752_112_2_17_23_10_51_1565,"smartcard alloc FAILED\n")
TRACE_MSG(CA_ADAPTER_764_112_2_17_23_10_51_1566,"CA adapter ca smartcard get ecm system ID FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_770_112_2_17_23_10_51_1567,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_777_112_2_17_23_10_51_1568,"ca smartcard get object data FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_783_112_2_17_23_10_51_1569,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_790_112_2_17_23_10_51_1570,"ca smartcard get object data FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_796_112_2_17_23_10_51_1571,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_803_112_2_17_23_10_51_1572,"ca smartcard get object data FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_809_112_2_17_23_10_51_1573,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_817_112_2_17_23_10_51_1574,"ca smartcard get object data FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_823_112_2_17_23_10_51_1575,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_831_112_2_17_23_10_51_1576,"ca smartcard get object data FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_840_112_2_17_23_10_51_1577,"---CA ADapter: CB Smartcard Number:0x%2x %2x %2x %2x %2x %2x %2x %2x addr:%xn")
TRACE_MSG(CA_ADAPTER_846_112_2_17_23_10_51_1578,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_855_112_2_17_23_10_51_1579,"ca smartcard get object data FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_867_112_2_17_23_10_51_1580,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_874_112_2_17_23_10_51_1581,"ca smartcard get object data FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_880_112_2_17_23_10_52_1582,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_888_112_2_17_23_10_52_1583,"ca smartcard get object data FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_894_112_2_17_23_10_52_1584,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_901_112_2_17_23_10_52_1585,"ca smartcard get object data FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_906_112_2_17_23_10_52_1586,"ca smartcard zipcode TOO LONG than 60 bytes. subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_913_112_2_17_23_10_52_1587,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_920_112_2_17_23_10_52_1588,"ca smartcard get object data  FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_926_112_2_17_23_10_52_1589,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_934_112_2_17_23_10_52_1590,"ca smartcard get object data FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_940_112_2_17_23_10_52_1591,"ca smartcard dispose object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_955_112_2_17_23_10_52_1592,"ca program get processing status FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_960_112_2_17_23_10_52_1593,"ca program get processing ERROR status subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_965_112_2_17_23_10_52_1594,"ca program get object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_968_112_2_17_23_10_52_1595,"---CA ADapter: program get OBJECT num:%d\n")
TRACE_MSG(CA_ADAPTER_972_112_2_17_23_10_52_1596,"program alloc FAILED\n")
TRACE_MSG(CA_ADAPTER_983_112_2_17_23_10_52_1597,"ca program get object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_989_112_2_17_23_10_52_1598,"ca program get object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_997_112_2_17_23_10_52_1599,"ca program get object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_1003_112_2_17_23_10_52_1600,"ca program get object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_1010_112_2_17_23_10_52_1601,"ca program get object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_1016_112_2_17_23_10_52_1602,"ca program get object FAILED subtype:%d!!!\n")
TRACE_MSG(CA_ADAPTER_1099_112_2_17_23_10_52_1603,"ca system call Wrong subtype:%d!!!")
TRACE_MSG(CA_ADAPTER_1105_112_2_17_23_10_52_1604,"ca system call subtype:%d request create failed!!!")
TRACE_MSG(CA_ADAPTER_1115_112_2_17_23_10_52_1605,"ca system call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1123_112_2_17_23_10_52_1606,"ca system call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1131_112_2_17_23_10_52_1607,"ca system call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1144_112_2_17_23_10_52_1608,"ca system call subtype:%d send request  failed!!!")
TRACE_MSG(CA_ADAPTER_1154_112_2_17_23_10_52_1609,"ca program call Wrong subtype:%d!!!")
TRACE_MSG(CA_ADAPTER_1160_112_2_17_23_10_52_1610,"ca program call subtype:%d request create failed!!!")
TRACE_MSG(CA_ADAPTER_1169_112_2_17_23_10_52_1611,"ca program call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1177_112_2_17_23_10_52_1612,"ca program call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1185_112_2_17_23_10_52_1613,"ca program call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1198_112_2_17_23_10_52_1614,"ca program call subtype:%d send request  failed!!!")
TRACE_MSG(CA_ADAPTER_1208_112_2_17_23_10_52_1615,"ca smartcard call Wrong subtype:%d!!!")
TRACE_MSG(CA_ADAPTER_1214_112_2_17_23_10_52_1616,"ca smartcard call subtype:%d request create failed!!!")
TRACE_MSG(CA_ADAPTER_1223_112_2_17_23_10_52_1617,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1231_112_2_17_23_10_52_1618,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1239_112_2_17_23_10_52_1619,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1247_112_2_17_23_10_52_1620,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1255_112_2_17_23_10_52_1621,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1263_112_2_17_23_10_52_1622,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1271_112_2_17_23_10_52_1623,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1279_112_2_17_23_10_52_1624,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1287_112_2_17_23_10_52_1625,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1295_112_2_17_23_10_52_1626,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1303_112_2_17_23_10_52_1627,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1311_112_2_17_23_10_52_1628,"ca smartcard call subtype:%d request Set asyn response failed!!!")
TRACE_MSG(CA_ADAPTER_1324_112_2_17_23_10_52_1629,"ca smartcard call subtype:%d send request  failed!!!")
TRACE_MSG(CA_ADAPTER_1342_112_2_17_23_10_52_1630,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1360_112_2_17_23_10_52_1631,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1378_112_2_17_23_10_53_1632,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1396_112_2_17_23_10_53_1633,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1414_112_2_17_23_10_53_1634,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1431_112_2_17_23_10_53_1635,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1448_112_2_17_23_10_53_1636,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1465_112_2_17_23_10_53_1637,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1483_112_2_17_23_10_53_1638,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1501_112_2_17_23_10_53_1639,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1519_112_2_17_23_10_53_1640,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1537_112_2_17_23_10_53_1641,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1555_112_2_17_23_10_53_1642,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1573_112_2_17_23_10_53_1643,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1591_112_2_17_23_10_53_1644,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1609_112_2_17_23_10_53_1645,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1627_112_2_17_23_10_53_1646,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1645_112_2_17_23_10_53_1647,"ca system cakversion request pointer NOT MATCH!!!")
TRACE_MSG(CA_ADAPTER_1664_112_2_17_23_10_53_1648," ---CA ADapter: CB of CA_Get_Information type:%d subtype:%d return OK\n")
TRACE_MSG(CA_ADAPTER_1670_112_2_17_23_10_53_1649,"---CA ADapter: CB cak_version:%s")
TRACE_MSG(CA_ADAPTER_1673_112_2_17_23_10_53_1650,"---CA ADapter: CB ird_serial:%s")
TRACE_MSG(CA_ADAPTER_1676_112_2_17_23_10_53_1651,"---CA ADapter: CB project_info:%s")
TRACE_MSG(CA_ADAPTER_1679_112_2_17_23_10_53_1652,"adapter system CB ERROR subtype\n")
TRACE_MSG(CA_ADAPTER_1688_112_2_17_23_10_53_1653,"---CA ADapter: CB ECM_SYSTEM_ID:%d\n")
TRACE_MSG(CA_ADAPTER_1691_112_2_17_23_10_53_1654,"---CA ADapter: CB EMM_SYSTEM_ID:%d\n")
TRACE_MSG(CA_ADAPTER_1694_112_2_17_23_10_53_1655,"---CA ADapter: CB SmartcardVERSION:%s\n")
TRACE_MSG(CA_ADAPTER_1697_112_2_17_23_10_53_1656,"---CA ADapter: CB Smartcard SN:%s\n")
TRACE_MSG(CA_ADAPTER_1701_112_2_17_23_10_53_1657,"---CA ADapter: CB Smartcard ID:0x%2x %2x %2x %2x %2x %2x n")
TRACE_MSG(CA_ADAPTER_1707_112_2_17_23_10_53_1658,"---CA ADapter: CB Smartcard Number:0x%2x %2x %2x %2x %2x %2x %2x %2xn")
TRACE_MSG(CA_ADAPTER_1712_112_2_17_23_10_53_1659,"---CA ADapter: CB Smartcard expir_date: %d-%d-%d -%d:%d:%d n")
TRACE_MSG(CA_ADAPTER_1715_112_2_17_23_10_53_1660,"---CA ADapter: CB Smartcard provider_id:%d \n")
TRACE_MSG(CA_ADAPTER_1719_112_2_17_23_10_53_1661,"---CA ADapter: CB Smartcard session_id:%d \n")
TRACE_MSG(CA_ADAPTER_1722_112_2_17_23_10_53_1662,"---CA ADapter: CB Smartcard zip code:%s \n")
TRACE_MSG(CA_ADAPTER_1725_112_2_17_23_10_53_1663,"---CA ADapter: CB Smartcard dela time:%d \n")
TRACE_MSG(CA_ADAPTER_1728_112_2_17_23_10_53_1664,"---CA ADapter: CB Smartcard STATE:%d")
TRACE_MSG(CA_ADAPTER_1731_112_2_17_23_10_53_1665,"adapter smartcard CB Notify/ERROR subtype\n")
TRACE_MSG(CA_ADAPTER_1740_112_2_17_23_10_53_1666,"---CA ADapter: CB Program TS session id:%d\n")
TRACE_MSG(CA_ADAPTER_1743_112_2_17_23_10_53_1667,"---CA ADapter: CB program number:%d\n")
TRACE_MSG(CA_ADAPTER_1746_112_2_17_23_10_53_1668,"---CA ADapter: CB program access enum:%d\n")
TRACE_MSG(CA_ADAPTER_1749_112_2_17_23_10_53_1669,"adapter program CB Notify/ERROR subtype\n")
TRACE_MSG(CA_ADAPTER_1767_112_2_17_23_10_53_1670," ---CA ADapter: call CA_Get_Information type:%d subtype:%d return ERROR\n")
TRACE_MSG(CA_ADAPTER_1769_112_2_17_23_10_53_1671," ---CA ADapter: call CA_Get_Information type:%d subtype:%d return OK\n")
TRACE_MSG(CA_ADAPTER_1779_112_2_17_23_10_53_1672," ---CA ADapter: call CA_Get_Information type:%d subtype:%d return ERROR\n")
TRACE_MSG(CA_ADAPTER_1781_112_2_17_23_10_53_1673," ---CA ADapter: call CA_Get_Information type:%d subtype:%d return OK\n")
TRACE_MSG(CA_ADAPTER_1791_112_2_17_23_10_53_1674," ---CA ADapter: call CA_Get_Information type:%d subtype:%d return ERROR\n")
TRACE_MSG(CA_ADAPTER_1793_112_2_17_23_10_53_1675," ---CA ADapter: call CA_Get_Information type:%d subtype:%d return OK\n")
TRACE_MSG(CA_CCL_201_112_2_17_23_10_54_1676,"KDA CCL:TimerCallback  \n")
TRACE_MSG(CA_CCL_213_112_2_17_23_10_54_1677,"KDA CCL:CCLActiveWatchTimer  \n")
TRACE_MSG(CA_CCL_221_112_2_17_23_10_54_1678,"KDA CCL:CCLActiveWatchTimer  delete old timer\n")
TRACE_MSG(CA_CCL_241_112_2_17_23_10_54_1679,"KDA CCL: CACCL_GetChannelStatus \n.")
TRACE_MSG(CA_CCL_247_112_2_17_23_10_54_1680,"KDA CCL: CACCL_GetChannelStatus error block number 0x%2x  \n.")
TRACE_MSG(CA_CCL_264_112_2_17_23_10_54_1681,"KDA CCL: CompareService serive_info.xProgramId=%d,setting.programID%d n")
TRACE_MSG(CA_CCL_283_112_2_17_23_10_54_1682,"KDA CCL: AddService  s_ccl_service_count %d\n")
TRACE_MSG(CA_CCL_288_112_2_17_23_10_54_1683,"CCL: AddService  i %d\n")
TRACE_MSG(CA_CCL_308_112_2_17_23_10_54_1684,"KDA CCL: AddService  cat pxData[%d]%d\n")
TRACE_MSG(CA_CCL_355_112_2_17_23_10_55_1685,"KDA CCL: AddService  s_ccl_service_count %d\n")
TRACE_MSG(CA_CCL_368_112_2_17_23_10_55_1686,"KDA CCL_InitReg  %x\n")
TRACE_MSG(CA_CCL_388_112_2_17_23_10_55_1687,"KDA CCL_RegisterCallback fun %x")
TRACE_MSG(CA_CCL_403_112_2_17_23_10_55_1688,"KDA CCL_RegisterCallback RegistrationId 0x%x")
TRACE_MSG(CA_CCL_439_112_2_17_23_10_55_1689,"KDA CCL_UnregisterCallback xRegistrationId 0x%x")
TRACE_MSG(CA_CCL_451_112_2_17_23_10_55_1690,"KDA GetLastCCLReg  \n")
TRACE_MSG(CA_CCL_475_112_2_17_23_10_55_1691,"KDA CCL:GetCurReg %d  \n")
TRACE_MSG(CA_CCL_479_112_2_17_23_10_55_1692,"KDA CCL:GetCurrentReg  register id %d\n")
TRACE_MSG(CA_CCL_506_112_2_17_23_10_55_1693,"KDA CCL GetCallback s_ccl_reg[i]  0x%x")
TRACE_MSG(CA_CCL_526_112_2_17_23_10_55_1694,"KDA CCL:CACCL_CmlNotifyCCL \n")
TRACE_MSG(CA_CCL_539_112_2_17_23_10_55_1695,"KDA CCL:CACCL_CmlNotifyCCL send signal to task,register_id 0x%x n")
TRACE_MSG(CA_CCL_579_112_2_17_23_10_55_1696,"CACCL_PlayProgram tick 0x%x ,s_ccl_call_reg %d\n")
TRACE_MSG(CA_CCL_583_112_2_17_23_10_55_1697,"KDA CACCL_PlayProgram pxServiceProvider 0x%x,net_id= 0x%x,ProgramId  0x%x")
TRACE_MSG(CA_CCL_584_112_2_17_23_10_55_1698,"KDA CACCL_PlayProgram cat  0x%x,len =%d\n")
TRACE_MSG(CA_CCL_618_112_2_17_23_10_55_1699,"KDA CCL CACCL_PlayProgram cat[%d]  = 0x%x\n")
TRACE_MSG(CA_CCL_637_112_2_17_23_10_55_1700,"KDA CACCL_PlayProgram element num =%d ,element 0x%x n")
TRACE_MSG(CA_CCL_670_112_2_17_23_10_55_1701,"KDA CACCL_PlayProgram xProgramId  %x")
TRACE_MSG(CA_CCL_690_112_2_17_23_10_55_1702,"KDA CACCL_PlayProgram element num%d ,element%x n")
TRACE_MSG(CA_CCL_748_112_2_17_23_10_56_1703,"KDA CACCL_StopProgram\n")
TRACE_MSG(CA_CCL_764_112_2_17_23_10_56_1704,"KDA CACCL_SetPlayCallback func %x")
TRACE_MSG(CA_CCL_778_112_2_17_23_10_56_1705,"KDA CACCL_IsPlayFlag %d \n")
TRACE_MSG(CA_CCL_801_112_2_17_23_10_56_1706,"KDA CCL NotifyKDA cat= 0x%x,len %d ,tick= 0x%x\n")
TRACE_MSG(CA_CCL_802_112_2_17_23_10_56_1707,"KDA CCL NotifyKDA xProgramId=  0x%x,xTransportSessionId= 0x%x,networkid= 0x%x \n")
TRACE_MSG(CA_CCL_819_112_2_17_23_10_56_1708,"KDA CCL  NotifyKDA after callback tick 0x%x\n")
TRACE_MSG(CA_CCL_832_112_2_17_23_10_56_1709,"KDA IsValidCCLRegistrationId xRegistrationId%d \n")
TRACE_MSG(CA_CCL_902_112_2_17_23_10_56_1710,"KDA cclRegister xCallback  %x,tick 0x%x")
TRACE_MSG(CA_CCL_909_112_2_17_23_10_56_1711,"KDA cclRegister s_ccl_call_reg  %d\n")
TRACE_MSG(CA_CCL_913_112_2_17_23_10_56_1712,"KDA CCL:cmlRegister send signal to task\n")
TRACE_MSG(CA_CCL_976_112_2_17_23_10_56_1713,"KDA cclReportAccess xAccess %d,xProgramId %x\n")
TRACE_MSG(CA_CCL_985_112_2_17_23_10_56_1714,"KDA cclReportAccess err_block 0x%x \n")
TRACE_MSG(CA_CCL_990_112_2_17_23_10_56_1715,"KDA cclReportAccess send denied  msg \n")
TRACE_MSG(CA_CCL_996_112_2_17_23_10_56_1716,"KDA cclReportAccess need to verify \n")
TRACE_MSG(CA_CCL_1076_112_2_17_23_10_56_1717,"KDA cclCancelRegistration xRegistrationId %d,tick 0x%x\n")
TRACE_MSG(CA_CCL_1111_112_2_17_23_10_56_1718,"KDA CCL HandleReportAccessSig status%d,status_ptr%x \n")
TRACE_MSG(CA_CCL_1113_112_2_17_23_10_56_1719,"KDA CCL HandleReportAccessSig s_ccl_operation_status%d \n")
TRACE_MSG(CA_CCL_1115_112_2_17_23_10_56_1720,"KDA CCL HandleReportAccessSig s_ccl_current_prog.xProgramId %d \n")
TRACE_MSG(CA_CCL_1120_112_2_17_23_10_56_1721,"KDA CCL HandleReportAccessSig call appcallback function \n")
TRACE_MSG(CA_CCL_1250_112_2_17_23_10_57_1722,"KDA HandleAppAddProgSig programID=%d,is_set=%d,cur_programID=%dn")
TRACE_MSG(CA_CCL_1253_112_2_17_23_10_57_1723,"KDA HandleAppAddProgSig element num%d ,element%x n")
TRACE_MSG(CA_CCL_1258_112_2_17_23_10_57_1724,"KDA HandleAppAddProgSig s_ccl_abort_flag %d  n")
TRACE_MSG(CA_CCL_1274_112_2_17_23_10_57_1725,"KDA HandleAppAddProgSig xTransportSessionId=0x%x ,s_ccl_call_reg %dn")
TRACE_MSG(CA_CCL_1285_112_2_17_23_10_57_1726,"KDA HandleAppAddProgSig  no reg or wait for cml\n")
TRACE_MSG(CA_CCL_1295_112_2_17_23_10_57_1727,"KDA HandleAppAddProgSig call NotifyKDA s_ccl_is_set_resume = %d\n")
TRACE_MSG(CA_CCL_1335_112_2_17_23_10_57_1728,"KDA CCL GetService s_ccl_service_count%d\n")
TRACE_MSG(CA_CCL_1345_112_2_17_23_10_57_1729,"KDA CCL GetService programid %d\n")
TRACE_MSG(CA_CCL_1361_112_2_17_23_10_57_1730,"KDA CCL DelService s_ccl_service_count%d\n")
TRACE_MSG(CA_CCL_1375_112_2_17_23_10_57_1731,"KDA CCL DelService pxData 0x%x\n")
TRACE_MSG(CA_CCL_1382_112_2_17_23_10_57_1732,"KDA CCL DelService pxElementaryStreamTable 0x%x\n")
TRACE_MSG(CA_CCL_1389_112_2_17_23_10_57_1733,"KDA CCL DelService pxServiceProvider 0x%x\n")
TRACE_MSG(CA_CCL_1396_112_2_17_23_10_57_1734,"KDA CCL DelService programid %d\n")
TRACE_MSG(CA_CCL_1440_112_2_17_23_10_57_1735,"KDA HandleAppChangeProgSig programID=%d,is_set=%d,cur_programID=%dn")
TRACE_MSG(CA_CCL_1443_112_2_17_23_10_57_1736,"KDA HandleAppChangeProgSig element num%d ,element%x n")
TRACE_MSG(CA_CCL_1459_112_2_17_23_10_57_1737,"KDA HandleAppChangeProgSig xTransportSessionId= 0x%x n")
TRACE_MSG(CA_CCL_1480_112_2_17_23_10_57_1738,"KDA HandleAppChangeProgSig netid=%d \n")
TRACE_MSG(CA_CCL_1513_112_2_17_23_10_57_1739,"KDA CACCL_About flag %d\n")
TRACE_MSG(CA_CCL_1527_112_2_17_23_10_57_1740,"KDA CACCL_SetPlayFlag is_play %d\n")
TRACE_MSG(CA_CCL_1544_112_2_17_23_10_57_1741,"KDA HandleAppRemoveProgSig programID %d ,s_ccl_add_service %d \n")
TRACE_MSG(CA_CCL_1591_112_2_17_23_10_57_1742,"KDA HandleAppRemoveProgSig ABORT \n")
TRACE_MSG(CA_CCL_1597_112_2_17_23_10_57_1743,"KDA HandleAppRemoveProgSig ABORT %d\n")
TRACE_MSG(CA_CCL_1614_112_2_17_23_10_57_1744,"KDA CCL GetReg  reg_id 0x%x \n")
TRACE_MSG(CA_CCL_1651_112_2_17_23_10_57_1745,"KDA HandleAppAddProgSig s_ccl_abort_flag %d  n")
TRACE_MSG(CA_CCL_1653_112_2_17_23_10_57_1746,"KDA HandleCCLRegisterSig HandleCCLRegisterSig 0x%x,cml_added %d\n")
TRACE_MSG(CA_CCL_1658_112_2_17_23_10_57_1747,"KDA HandleCCLRegisterSig wait for cml add  \n")
TRACE_MSG(CA_CCL_1668_112_2_17_23_10_57_1748,"KDA HandleCCLRegisterSig no service \n")
TRACE_MSG(CA_CCL_1675_112_2_17_23_10_57_1749,"KDA HandleCCLRegisterSig no reg or s_ccl_call_reg %d \n")
TRACE_MSG(CA_CCL_1693_112_2_17_23_10_57_1750,"KDA HandleCCLRegisterSig xTransportSessionId 0x%x \n")
TRACE_MSG(CA_CCL_1722_112_2_17_23_10_57_1751,"KDA HandleCCLRegisterSig after NotifyKDA xTransportSessionId 0x%x \n")
TRACE_MSG(CA_CCL_1744_112_2_17_23_10_58_1752,"KDA CCL CCLThreadEntry task_id%d \n")
TRACE_MSG(CA_CCL_1750_112_2_17_23_10_58_1753,"CCL sig_ptr->SignalCode%d \n")
TRACE_MSG(CA_CCL_1796_112_2_17_23_10_58_1754,"KDA CCL:CACCL_IsFilterForEcm service_id 0x%x ,s_ccl_play_flag %d ,s_ccl_set_prog.xProgramId 0x%x\n")
TRACE_MSG(CA_CCL_1797_112_2_17_23_10_58_1755,"KDA CCL:CACCL_IsFilterForEcm session_id 0x%x ,s_ccl_set_prog.xTransportSessionId 0x%x\n")
TRACE_MSG(CA_CCL_1816_112_2_17_23_10_58_1756,"KDA CACCL_Init \n")
TRACE_MSG(CA_CCL_1833_112_2_17_23_10_58_1757,"CCL CACCL_Init task_id%d \n")
TRACE_MSG(CA_CCL_1863_112_2_17_23_10_58_1758,"KDA CACCL_Exit \n")
TRACE_MSG(CA_CCL_1906_112_2_17_23_10_58_1759,"KDA CCL_Exit: fail to delete  thread .\n ")
TRACE_MSG(CA_CCL_1929_112_2_17_23_10_58_1760,"KDA CACCL_IsExitFlag \n ")
TRACE_MSG(CA_CCL_1941_112_2_17_23_10_58_1761,"KDA CACCL_IsExitFlag no reg\n ")
TRACE_MSG(CA_CCL_1950_112_2_17_23_10_58_1762,"KDA CACCL_SetResume is_set_resume %d\n ")
TRACE_MSG(CA_CCL_1985_112_2_17_23_10_58_1763,"KDA CCL:CACCL_Test  \n")
TRACE_MSG(CA_CCL_2023_112_2_17_23_10_58_1764,"KDA CCL:CACCL_Test1  \n")
TRACE_MSG(CA_CCL_2039_112_2_17_23_10_58_1765,"KDA CCL:CACCL_Test2  \n")
TRACE_MSG(CA_CML_128_112_2_17_23_10_58_1766,"KDA CML:TimerCallback  \n")
TRACE_MSG(CA_CML_137_112_2_17_23_10_58_1767,"KDA CML:ActiveWatchTimer  \n")
TRACE_MSG(CA_CML_145_112_2_17_23_10_58_1768,"KDA CML:ActiveWatchTimer  delete old timer\n")
TRACE_MSG(CA_CML_162_112_2_17_23_10_59_1769,"KDA GetLastReg  \n")
TRACE_MSG(CA_CML_188_112_2_17_23_10_59_1770,"KDA CML_InitReg  \n")
TRACE_MSG(CA_CML_198_112_2_17_23_10_59_1771,"CML:GetCurReg %d  \n")
TRACE_MSG(CA_CML_202_112_2_17_23_10_59_1772,"CML:GetCurrentReg  register id %d\n")
TRACE_MSG(CA_CML_227_112_2_17_23_10_59_1773,"CML:CML_RegisterCallback  fun %d\n")
TRACE_MSG(CA_CML_241_112_2_17_23_10_59_1774,"CML:CML_RegisterCallback  register id %d\n")
TRACE_MSG(CA_CML_270_112_2_17_23_10_59_1775,"KDA CML:CML_UnregisterCallback  xRegistrationId %d\n")
TRACE_MSG(CA_CML_305_112_2_17_23_10_59_1776,"KDA CML:GetCallback  s_cml_reg[%d].register_id%d\n")
TRACE_MSG(CA_CML_321_112_2_17_23_10_59_1777,"CML: CompareCAT cat_info.xDataSize%d \n")
TRACE_MSG(CA_CML_330_112_2_17_23_10_59_1778,"CML: CompareCAT cat_info.pxData%d,cat %d \n")
TRACE_MSG(CA_CML_349_112_2_17_23_10_59_1779,"CML: AddCAT  s_cat_total_count%d\n")
TRACE_MSG(CA_CML_356_112_2_17_23_10_59_1780,"KDA CML: AddCAT  i%d\n")
TRACE_MSG(CA_CML_385_112_2_17_23_10_59_1781,"KDA CML: AddCAT  s_cat_total_count%d\n")
TRACE_MSG(CA_CML_404_112_2_17_23_10_59_1782,"KDA CML: GetCat \n")
TRACE_MSG(CA_CML_411_112_2_17_23_10_59_1783,"KDA CML: GetCat len %d\n")
TRACE_MSG(CA_CML_424_112_2_17_23_10_59_1784,"KDA CML: CmlNotifyKDA  cmd %d,cat len %d,cat 0x%x ,xTransportSessionId 0x%x\n")
TRACE_MSG(CA_CML_434_112_2_17_23_10_59_1785,"KDA CML:CmlNotifyKDA callback is PNULL\n")
TRACE_MSG(CA_CML_447_112_2_17_23_10_59_1786,"KDA CML:CmlNotifyKDA after callback\n")
TRACE_MSG(CA_CML_459_112_2_17_23_10_59_1787,"KDA CML:IsRemoveEMM s_cml_call_reg %d,filter_count %d ,s_cml_wait %d\n")
TRACE_MSG(CA_CML_499_112_2_17_23_10_59_1788,"KDA CML:CACML_STOP s_cml_change_plch %d\n")
TRACE_MSG(CA_CML_522_112_2_17_23_10_59_1789,"KDA CML:CACML_STOP no reg \n")
TRACE_MSG(CA_CML_579_112_2_17_23_10_59_1790,"KDA CML:CACML_ChangePLCH  cat%d,len%d\n")
TRACE_MSG(CA_CML_597_112_2_17_23_10_59_1791,"KDA CML:CACML_ChangePLCH  s_cml_call_reg %d\n")
TRACE_MSG(CA_CML_627_112_2_17_23_10_59_1792,"KDA CML:CACML_ChangePLCH  xTransportSessionId%d\n")
TRACE_MSG(CA_CML_666_112_2_17_23_10_59_1793,"KDA CML:IsValidCMLRegistrationId  xRegistrationId%d\n")
TRACE_MSG(CA_CML_724_112_2_17_23_11_0_1794,"KDA CML:cmlRegister  xCallback %d\n")
TRACE_MSG(CA_CML_736_112_2_17_23_11_0_1795,"KDA CML:cmlRegister send signal to task\n")
TRACE_MSG(CA_CML_795_112_2_17_23_11_0_1796,"KDA CML:cmlCancelRegistration  xRegistrationId %d\n")
TRACE_MSG(CA_CML_807_112_2_17_23_11_0_1797,"KDA CML:cmlCancelRegistration  s_cml_is_add %d\n")
TRACE_MSG(CA_CML_823_112_2_17_23_11_0_1798,"KDA CML:CML_Test  \n")
TRACE_MSG(CA_CML_849_112_2_17_23_11_0_1799,"KDA CML HandleCmlRegisterSig  \n")
TRACE_MSG(CA_CML_855_112_2_17_23_11_0_1800,"KDA CML HandleCmlRegisterSig no cat \n")
TRACE_MSG(CA_CML_862_112_2_17_23_11_0_1801,"KDA CML HandleCmlRegisterSig no register or s_cml_call_reg %d\n")
TRACE_MSG(CA_CML_877_112_2_17_23_11_0_1802,"KDA CML:HandleCmlRegisterSig  xTransportSessionId 0x%x\n")
TRACE_MSG(CA_CML_891_112_2_17_23_11_0_1803,"KDA CML:HandleCmlRegisterSig  after CmlNotifyKDA xTransportSessionId 0x%x\n")
TRACE_MSG(CA_CML_912_112_2_17_23_11_0_1804,"KDA CML CMLThreadEntry task_id%d \n")
TRACE_MSG(CA_CML_918_112_2_17_23_11_0_1805,"KDA CML sig_ptr->SignalCode%d \n")
TRACE_MSG(CA_CML_942_112_2_17_23_11_0_1806,"KDA CML CACML_GetAddFlag %d\n")
TRACE_MSG(CA_CML_957_112_2_17_23_11_0_1807,"KDA CML CACML_ClearValue\n")
TRACE_MSG(CA_CML_970_112_2_17_23_11_0_1808,"KDA CACML_IsExitFlag \n ")
TRACE_MSG(CA_CML_982_112_2_17_23_11_0_1809,"KDA CACML_IsExitFlag no reg\n ")
TRACE_MSG(CA_CML_994_112_2_17_23_11_0_1810,"KDA CML: CACML_IsFilterForEmm  s_cml_mutex_flag %d \n")
TRACE_MSG(CA_CML_995_112_2_17_23_11_0_1811,"KDA CML: CACML_IsFilterForEmm session_id 0x%x ,s_cml_current_prog.xTransportSessionId 0x%x\n")
TRACE_MSG(CA_CML_1013_112_2_17_23_11_0_1812,"KDA CML:CML_Init\n")
TRACE_MSG(CA_CML_1031_112_2_17_23_11_0_1813,"KDA CML CACML_Init task_id%d \n")
TRACE_MSG(CA_CML_1070_112_2_17_23_11_0_1814,"KDA CML:CML_Exit\n")
TRACE_MSG(CA_CML_1084_112_2_17_23_11_0_1815,"KDA CML_Exit: fail to delete  thread .\n ")
TRACE_MSG(CA_CSL_150_112_2_17_23_11_1_1816,"KDA CSL:CACSL_SetPlayingServiceId xServiceId 0x%x,s_csl_filter_is_start %d n")
TRACE_MSG(CA_CSL_162_112_2_17_23_11_1_1817,"KDA CSL:SetFilter xServiceId 0x%x, playing ServiceId 0x%x,is_start %d n")
TRACE_MSG(CA_CSL_171_112_2_17_23_11_1_1818,"KDA CSL:SetFilter start s_csl_filter_is_start 0x%xn")
TRACE_MSG(CA_CSL_177_112_2_17_23_11_1_1819,"KDA CSL:SetFilter start s_csl_emm_filter_count 0x%xn")
TRACE_MSG(CA_CSL_192_112_2_17_23_11_1_1820,"KDA CSL:CACSL_SetFilter s_csl_filter_is_start 0x%x n")
TRACE_MSG(CA_CSL_204_112_2_17_23_11_1_1821,"KDA CSL:CACSL_GetEmmFilterCount  s_csl_emm_filter_count 0x%x n")
TRACE_MSG(CA_CSL_220_112_2_17_23_11_1_1822,"KDA CSL:CACSL_SetEmmFilterCount  s_csl_emm_filter_count 0x%x n")
TRACE_MSG(CA_CSL_234_112_2_17_23_11_1_1823,"KDA CSL:CACSL_GetFilterStartFlag  s_csl_filter_is_start 0x%x n")
TRACE_MSG(CA_CSL_248_112_2_17_23_11_1_1824,"KDA CSL:AddFilterToList filter_id%d,xFilterDepth%d \n")
TRACE_MSG(CA_CSL_282_112_2_17_23_11_1_1825,"KDA CSL: GetFilterInfo,filter_id%d \n")
TRACE_MSG(CA_CSL_289_112_2_17_23_11_1_1826,"KDA CSL: GetFilterInfo filter_ptr%x \n")
TRACE_MSG(CA_CSL_302_112_2_17_23_11_1_1827,"KDA CSL: FilterTimeoutCallback filter_ptr= 0x%x \n")
TRACE_MSG(CA_CSL_321_112_2_17_23_11_1_1828,"KDA CSL: FilterTimeoutCallback finish \n")
TRACE_MSG(CA_CSL_339_112_2_17_23_11_1_1829,"KDA CSL:FilterTimerCallback param%x\n")
TRACE_MSG(CA_CSL_395_112_2_17_23_11_1_1830,"KDA CSL:CSL_ClearFilter enable %d, filter_id %d\n")
TRACE_MSG(CA_CSL_431_112_2_17_23_11_1_1831,"KDA CSL: CACSL_RcvFilterData filter_id 0x%x ,pxSection 0x%x,xSectionSize 0x%x \n")
TRACE_MSG(CA_CSL_439_112_2_17_23_11_1_1832,"KDA CSL:CACSL_SetCompareFlag is_comp =0x%x,\n")
TRACE_MSG(CA_CSL_473_112_2_17_23_11_1_1833,"KDA CSL:IsCallFilterCallback \n")
TRACE_MSG(CA_CSL_482_112_2_17_23_11_1_1834,"KDA CSL:IsCallFilterCallback return false \n")
TRACE_MSG(CA_CSL_501_112_2_17_23_11_1_1835,"KDA CSL:CACSL_FilterCallback filter_id =0x%x,pxSection= 0x%x,xSectionSize =0x%x\n")
TRACE_MSG(CA_CSL_502_112_2_17_23_11_1_1836,"KDA CSL:CACSL_FilterCallback xQueryBufferCallback=0x%x,xReceivedSectionCallback= 0x%x\n")
TRACE_MSG(CA_CSL_506_112_2_17_23_11_1_1837,"KDA CSL:CACSL_FilterCallback filter_ptr == PNULL\n")
TRACE_MSG(CA_CSL_512_112_2_17_23_11_1_1838,"KDA CSL:CACSL_FilterCallback don't call callback \n")
TRACE_MSG(CA_CSL_519_112_2_17_23_11_1_1839,"KDA CSL:CACSL_FilterCallback s_csl_comp_ecm.is_comp =0x%x,filter_ptr->filter.xServiceId= 0x%x\n")
TRACE_MSG(CA_CSL_543_112_2_17_23_11_2_1840,"KDA CSL:CACSL_FilterCallback save first ecm xSectionSize 0x%x,\n")
TRACE_MSG(CA_CSL_553_112_2_17_23_11_2_1841,"KDA CSL:CACSL_FilterCallback same ecm\n")
TRACE_MSG(CA_CSL_571_112_2_17_23_11_2_1842,"KDA CSL:CACSL_FilterCallback s_csl_comp_ecm.ecm_len != xSectionSize \n")
TRACE_MSG(CA_CSL_579_112_2_17_23_11_2_1843,"KDA CSL:CACSL_FilterCallback different ecm\n")
TRACE_MSG(CA_CSL_608_112_2_17_23_11_2_1844,"KDA CSL:CACSL_FilterCallback after xQueryBufferCallback pxBuffer=0x%x,xBufferSize =0x%x\n")
TRACE_MSG(CA_CSL_618_112_2_17_23_11_2_1845,"KDA CSL:CACSL_FilterCallback callback tick 0x%x\n")
TRACE_MSG(CA_CSL_619_112_2_17_23_11_2_1846,"KDA CSL:CACSL_FilterCallback ecm content :\n")
TRACE_MSG(CA_CSL_629_112_2_17_23_11_2_1847,"KDA CSL:CACSL_FilterCallback ecm content >>[0x%04x]:  %02x  %02x  %02x  %02x  %02x  %02x  %02x  %02xn")
TRACE_MSG(CA_CSL_645_112_2_17_23_11_2_1848,"KDA CSL:CACSL_FilterCallback ecm content >>[0x%04x]:%s")
TRACE_MSG(CA_CSL_662_112_2_17_23_11_2_1849,"KDA CSL:CACSL_FilterCallback size of data very long \n")
TRACE_MSG(CA_CSL_674_112_2_17_23_11_2_1850,"KDA CSL:CACSL_FilterCallback finish\n")
TRACE_MSG(CA_CSL_723_112_2_17_23_11_2_1851,"KDA CSL:cslFilterOpen pxFilterId 0x%x\n")
TRACE_MSG(CA_CSL_737_112_2_17_23_11_2_1852,"KDA CSL:cslFilterOpen filter_id 0x%x\n")
TRACE_MSG(CA_CSL_774_112_2_17_23_11_2_1853,"KDA CSL:cslFilterClose filter_id 0x%x\n")
TRACE_MSG(CA_CSL_867_112_2_17_23_11_2_1854,"KDA CSL:cslFilterSetPatterns filter_id = 0x%x\n")
TRACE_MSG(CA_CSL_892_112_2_17_23_11_2_1855,"KDA CSL:cslFilterSetPatterns data_len 0x%x,sub_type 0x%x\n")
TRACE_MSG(CA_CSL_896_112_2_17_23_11_2_1856,"KDA CSL:cslFilterSetPatterns pxMatch[%d] 0x%x\n")
TRACE_MSG(CA_CSL_902_112_2_17_23_11_2_1857,"KDA CSL:cslFilterSetPatterns pxEqualMask[%d] 0x%x\n")
TRACE_MSG(CA_CSL_908_112_2_17_23_11_2_1858,"KDA CSL:cslFilterSetPatterns pxNotEqualMask[%d] 0x%x\n")
TRACE_MSG(CA_CSL_922_112_2_17_23_11_2_1859,"KDA CSL:cslFilterSetPatterns MTV_STIMI_DATA_128BYTE_FILTER\n")
TRACE_MSG(CA_CSL_929_112_2_17_23_11_2_1860,"KDA CSL:cslFilterSetPatterns MTV_STIMI_DATA_16BYTE_FILTER\n")
TRACE_MSG(CA_CSL_941_112_2_17_23_11_2_1861,"KDA CSL:cslFilterSetPatterns MTV_STIMI_DATA_8BYTE_FILTER\n")
TRACE_MSG(CA_CSL_1041_112_2_17_23_11_3_1862,"KDA CSL:cslFilterStart filter_id =0x%x , tick 0x%x ,xTransportSessionId 0x%x\n")
TRACE_MSG(CA_CSL_1044_112_2_17_23_11_3_1863,"KDA CSL:cslFilterStart xTimeOut%d\n")
TRACE_MSG(CA_CSL_1046_112_2_17_23_11_3_1864,"KDA CSL:cslFilterStart xQueryBufferCallback%x\n")
TRACE_MSG(CA_CSL_1047_112_2_17_23_11_3_1865,"KDA CSL:cslFilterStart xReceivedSectionCallback%x\n")
TRACE_MSG(CA_CSL_1048_112_2_17_23_11_3_1866,"KDA CSL:cslFilterStart xLoopMode%d\n")
TRACE_MSG(CA_CSL_1066_112_2_17_23_11_3_1867,"KDA CSL:cslFilterStart filter_ptr== PNULL\n")
TRACE_MSG(CA_CSL_1093_112_2_17_23_11_3_1868,"KDA CSL:cslFilterStart xFilterId->timer== PNULL ,to create timer \n")
TRACE_MSG(CA_CSL_1108_112_2_17_23_11_3_1869,"KDA CSL:cslFilterStart xDataType 0x%x\n")
TRACE_MSG(CA_CSL_1117_112_2_17_23_11_3_1870,"KDA CSL:cslFilterStart enablefilter tick 0x%x\n")
TRACE_MSG(CA_CSL_1157_112_2_17_23_11_3_1871,"KDA CSL:cslFilterStop filter_id 0x%x ,status %d\n")
TRACE_MSG(CA_CSL_1175_112_2_17_23_11_3_1872,"KDA CSL:cslFilterStop xTimeout 0x%x,timer 0x%x\n")
TRACE_MSG(CA_CSL_1251_112_2_17_23_11_3_1873,"KDA CSL:cslFilterSetPacketId filter_id =0x%x,xServiceId =0x%x\n")
TRACE_MSG(CA_CSL_1269_112_2_17_23_11_3_1874,"KDA CSL:cslFilterSetPacketId xDataType = 0x%x\n")
TRACE_MSG(CA_CSL_1283_112_2_17_23_11_3_1875,"KDA CSL:cslFilterSetPacketId  successful\n")
TRACE_MSG(CA_CSL_1286_112_2_17_23_11_3_1876,"KDA CSL:cslFilterSetPacketId fail \n")
TRACE_MSG(CA_CSL_1311_112_2_17_23_11_3_1877,"KDA CSL:CSLThreadEntry  SignalCode%x\n")
TRACE_MSG(CA_CSL_1340_112_2_17_23_11_3_1878,"KDA CSL: IsExitFlag \n ")
TRACE_MSG(CA_CSL_1352_112_2_17_23_11_3_1879,"KDA CSL: IsExitFlag  no filter\n ")
TRACE_MSG(CA_CSL_1371_112_2_17_23_11_3_1880,"CCL ProcessData buffer_id %d \n")
TRACE_MSG(CA_CSL_1384_112_2_17_23_11_3_1881,"KDA CSL: ProcessDemuxData,no enough memory %d!")
TRACE_MSG(CA_CSL_1416_112_2_17_23_11_3_1882,"CCL CADataCallback buffer_id %d \n")
TRACE_MSG(CA_CSL_1449_112_2_17_23_11_3_1883,"CCL CACSL_IsRegisterCallback is_reg %d \n")
TRACE_MSG(CA_CSL_1474_112_2_17_23_11_3_1884,"KDA CACSL_Init \n")
TRACE_MSG(CA_CSL_1495_112_2_17_23_11_3_1885,"CCL CACSL_Init task_id%d \n")
TRACE_MSG(CA_CSL_1512_112_2_17_23_11_3_1886,"KDA CACSL_Exit \n")
TRACE_MSG(CA_CSL_1524_112_2_17_23_11_3_1887,"KDA CACSL_Exit: fail to delete  thread .\n ")
TRACE_MSG(CA_CSL_2104_112_2_17_23_11_5_1888,"__KDA_TEST_ECM: No of ECM:%d  \n")
TRACE_MSG(CA_CSL_2109_112_2_17_23_11_5_1889,"__KDA_TEST_ECM: Filter Available \n")
TRACE_MSG(CA_CSL_2114_112_2_17_23_11_5_1890,"__KDA_TEST_ECM: Data has been calledback \n")
TRACE_MSG(CA_CSL_2169_112_2_17_23_11_5_1891,"__KDA_TEST_ECM: No Filter Available \n")
TRACE_MSG(CA_CSL_2180_112_2_17_23_11_5_1892,"KDA CSL:TestTimerCallback  \n")
TRACE_MSG(CA_CSL_2189_112_2_17_23_11_5_1893,"KDA CSL:test_csl  \n")
TRACE_MSG(CA_DSX_171_112_2_17_23_11_5_1894,"KDA DSX:SetIsma element_id %d\n")
TRACE_MSG(CA_DSX_177_112_2_17_23_11_5_1895,"KDA DSX:SetIsma ismacrypData.salt[%d] %d\n")
TRACE_MSG(CA_DSX_198_112_2_17_23_11_5_1896,"KDA DSX:SetIsma rtpData.auSizeLengthType %x\n")
TRACE_MSG(CA_DSX_246_112_2_17_23_11_5_1897,"KDA DSX:SetIsma isma_data[i] %x\n")
TRACE_MSG(CA_DSX_263_112_2_17_23_11_5_1898,"KDA DSX: :CADSX_GetISMA \n")
TRACE_MSG(CA_DSX_300_112_2_17_23_11_5_1899,"KDA DSX: :CADSX_SetISMA element_type %d\n")
TRACE_MSG(CA_DSX_341_112_2_17_23_11_5_1900,"KDA DSX :InitObj  \n")
TRACE_MSG(CA_DSX_358_112_2_17_23_11_6_1901,"KDA DSX: :CreateObj xProgramId %x \n")
TRACE_MSG(CA_DSX_388_112_2_17_23_11_6_1902,"KDA DSX :CreateObj  ProgramId%x,xElementaryStreamId%x,xNumKeys%x,xKeyLength%x\n")
TRACE_MSG(CA_DSX_421_112_2_17_23_11_6_1903,"KDA DSX :DeleteObj  %d,pxKeyTable = 0x%x\n")
TRACE_MSG(CA_DSX_436_112_2_17_23_11_6_1904,"KDA DSX :DeleteObj pxKeyTable.pValue = 0x%x\n")
TRACE_MSG(CA_DSX_468_112_2_17_23_11_6_1905,"KDA DSX :GetCurObj   \n")
TRACE_MSG(CA_DSX_479_112_2_17_23_11_6_1906,"KDA DSX :GetCurObj  s_dsx_obj[i].obj.is_set_isma %x , s_dsx_obj[i].is_config_isma %x \n")
TRACE_MSG(CA_DSX_486_112_2_17_23_11_6_1907,"KDA DSX :GetCurObj i=%d \n")
TRACE_MSG(CA_DSX_513_112_2_17_23_11_6_1908,"KDA DSX :GetObj  pxId %x,xElementaryStreamId = %d\n")
TRACE_MSG(CA_DSX_523_112_2_17_23_11_6_1909,"KDA DSX :GetObj i=%d \n")
TRACE_MSG(CA_DSX_537_112_2_17_23_11_6_1910,"KDA DSX:IsZeroIndex s_dsx_key_index_len %d ,index =0x%x\n")
TRACE_MSG(CA_DSX_595_112_2_17_23_11_6_1911,"KDA DSX:GetSetObj index%d \n")
TRACE_MSG(CA_DSX_598_112_2_17_23_11_6_1912,"KDA DSX:GetSetObj  index_to_find%x\n")
TRACE_MSG(CA_DSX_615_112_2_17_23_11_6_1913,"KDA DSX:GetSetObj  obj_ptr->obj.pxKeyTable[i]%x\n")
TRACE_MSG(CA_DSX_619_112_2_17_23_11_6_1914,"KDA DSX:GetSetObj  ismacrypData.index %x\n")
TRACE_MSG(CA_DSX_638_112_2_17_23_11_6_1915,"KDA DSX:GetSetObj  i%x,arr_index %x\n")
TRACE_MSG(CA_DSX_664_112_2_17_23_11_6_1916,"KDA DSX :GetKeyIndex key_index%x \n")
TRACE_MSG(CA_DSX_671_112_2_17_23_11_6_1917,"KDA DSX :GetKeyIndex arr_index%x \n")
TRACE_MSG(CA_DSX_694_112_2_17_23_11_6_1918,"KDA DSX :HandleDescramKeyReqSig key_select%x,key_id%x \n")
TRACE_MSG(CA_DSX_716_112_2_17_23_11_6_1919,"KDA DSX :HandleDescramKeyReqSig xSetNumKeys%x \n")
TRACE_MSG(CA_DSX_720_112_2_17_23_11_6_1920,"KDA DSX :HandleDescramKeyReqSig key[%d]%x \n")
TRACE_MSG(CA_DSX_734_112_2_17_23_11_6_1921,"KDA DSX :HandleDescramKeyReqSig key index%x \n")
TRACE_MSG(CA_DSX_748_112_2_17_23_11_6_1922,"KDA DSX :HandleDescramKeyReqSig don't get key key_rcv_demux_cmd_ref 0x%x\n")
TRACE_MSG(CA_DSX_761_112_2_17_23_11_6_1923,"KDA DSX:HandleISMAReqSig \n")
TRACE_MSG(CA_DSX_799_112_2_17_23_11_6_1924,"KDA DSX: :dsxCreate xProgramId %x \n")
TRACE_MSG(CA_DSX_819_112_2_17_23_11_6_1925,"KDA DSX: :dsxCreate success obj.pxId%x \n")
TRACE_MSG(CA_DSX_840_112_2_17_23_11_6_1926,"KDA DSX:dsxDispose \n")
TRACE_MSG(CA_DSX_872_112_2_17_23_11_6_1927,"KDA DSX:CompareISMAandRTPParam xElementaryStreamId %d\n")
TRACE_MSG(CA_DSX_882_112_2_17_23_11_7_1928,"KDA DSX:CompareISMAandRTPParam newsalt 0x%x, oldsalt 0x%xn")
TRACE_MSG(CA_DSX_886_112_2_17_23_11_7_1929,"KDA DSX:CompareISMAandRTPParam newsalt 0x%x, oldsalt 0x%xn")
TRACE_MSG(CA_DSX_918_112_2_17_23_11_7_1930,"KDA DSX:CompareISMAandRTPParam newsalt[%d] 0x%x, oldsalt[%d] 0x%xn")
TRACE_MSG(CA_DSX_945_112_2_17_23_11_7_1931,"KDA DSX:CompareISMAandRTPParam initializationVectorLength %x ,%xn")
TRACE_MSG(CA_DSX_948_112_2_17_23_11_7_1932,"KDA DSX:CompareISMAandRTPParam deltaInitializationVectorLength %x, %xn")
TRACE_MSG(CA_DSX_951_112_2_17_23_11_7_1933,"KDA DSX:CompareISMAandRTPParam keyIndicatorLength %x ,%xn")
TRACE_MSG(CA_DSX_954_112_2_17_23_11_7_1934,"KDA DSX:CompareISMAandRTPParam useKeyIndicatorPerAu %x,%xn")
TRACE_MSG(CA_DSX_957_112_2_17_23_11_7_1935,"KDA DSX:CompareISMAandRTPParam useSelectiveEncryption %x,%xn")
TRACE_MSG(CA_DSX_963_112_2_17_23_11_7_1936,"KDA DSX:CompareISMAandRTPParam useRandomAccessFlag %x,%xn")
TRACE_MSG(CA_DSX_966_112_2_17_23_11_7_1937,"KDA DSX:CompareISMAandRTPParam streamStateLength %x,%xn")
TRACE_MSG(CA_DSX_970_112_2_17_23_11_7_1938,"KDA DSX:CompareISMAandRTPParam indexDeltaLength %x,%xn")
TRACE_MSG(CA_DSX_974_112_2_17_23_11_7_1939,"KDA DSX:CompareISMAandRTPParam indexLength%x,%xn")
TRACE_MSG(CA_DSX_978_112_2_17_23_11_7_1940,"KDA DSX:CompareISMAandRTPParam auxiliaryDataSizeLength %x,%xn")
TRACE_MSG(CA_DSX_981_112_2_17_23_11_7_1941,"KDA DSX:CompareISMAandRTPParam ctsDeltaLength %x,%xn")
TRACE_MSG(CA_DSX_984_112_2_17_23_11_7_1942,"KDA DSX:CompareISMAandRTPParam useRandomAccessFlag %x,%xn")
TRACE_MSG(CA_DSX_991_112_2_17_23_11_7_1943,"KDA DSX:CompareISMAandRTPParam newsalt 0x%x, oldsalt 0x%xn")
TRACE_MSG(CA_DSX_1001_112_2_17_23_11_7_1944,"KDA DSX:CompareISMAandRTPParam param is diffrent\n")
TRACE_MSG(CA_DSX_1037_112_2_17_23_11_7_1945,"KDA DSX:dsxConfigure xId %d,tick 0x%x,type %d\n")
TRACE_MSG(CA_DSX_1051_112_2_17_23_11_7_1946,"KDA DSX:dsxConfigure s_dsx_key_index_len %d \n")
TRACE_MSG(CA_DSX_1058_112_2_17_23_11_7_1947,"KDA DSX:dsxConfigure same param \n")
TRACE_MSG(CA_DSX_1062_112_2_17_23_11_7_1948,"KDA DSX:dsxConfigure isma_used_count%d,isma_rcv_demux_cmd_ref%d,isma_total_count%d\n")
TRACE_MSG(CA_DSX_1067_112_2_17_23_11_7_1949,"KDA DSX:dsxConfigure isma_used_count %d \n")
TRACE_MSG(CA_DSX_1092_112_2_17_23_11_7_1950,"KDA DSX:DsxSaveKey index 0x%x\n")
TRACE_MSG(CA_DSX_1094_112_2_17_23_11_7_1951,"KDA DSX:DsxSaveKey key_used_count 0x%x, key_rcv_demux_cmd_ref 0x0x\n")
TRACE_MSG(CA_DSX_1095_112_2_17_23_11_7_1952,"KDA DSX:DsxSaveKey pxKeyTable 0x%x\n")
TRACE_MSG(CA_DSX_1103_112_2_17_23_11_7_1953,"KDA DSX :DsxSaveKey pxKeyTable.pValue = 0x%x\n")
TRACE_MSG(CA_DSX_1158_112_2_17_23_11_7_1954,"KDA DSX:DsxSaveKey key index 0x%x\n")
TRACE_MSG(CA_DSX_1173_112_2_17_23_11_7_1955,"KDA DSX:dsxSetKeys key_used_count%d,key_rcv_demux_cmd_ref%d,key_total_count%d\n")
TRACE_MSG(CA_DSX_1185_112_2_17_23_11_7_1956,"KDA DSX:CompareAndSaveKey pxKeyTable =0x%x,index = %x\n")
TRACE_MSG(CA_DSX_1191_112_2_17_23_11_7_1957,"KDA DSX:CompareAndSaveKey begin to set key\n")
TRACE_MSG(CA_DSX_1204_112_2_17_23_11_7_1958,"KDA DSX:CompareKey xNumKeys%x,xSetNumKeys%x\n")
TRACE_MSG(CA_DSX_1213_112_2_17_23_11_7_1959,"KDA DSX:CompareKey ismacrypData.index %x pxKeyTable.data.ismacrypData.index %x\n")
TRACE_MSG(CA_DSX_1227_112_2_17_23_11_7_1960,"KDA DSX:CompareKey need to save key \n")
TRACE_MSG(CA_DSX_1233_112_2_17_23_11_7_1961,"KDA DSX:CompareKey don't save key \n")
TRACE_MSG(CA_DSX_1249_112_2_17_23_11_7_1962,"KDA DSX:CompareKey finish\n")
TRACE_MSG(CA_DSX_1261_112_2_17_23_11_7_1963,"KDA DSX:CompareKeyIndex index 0x%x\n")
TRACE_MSG(CA_DSX_1270_112_2_17_23_11_7_1964,"KDA DSX:CompareKeyIndex min0x%x, i key index 0x%x\n")
TRACE_MSG(CA_DSX_1278_112_2_17_23_11_7_1965,"KDA DSX:CompareKeyIndex arr index,min key index 0x%x\n")
TRACE_MSG(CA_DSX_1293_112_2_17_23_11_7_1966,"KDA DSX:SetKey index 0x%x\n")
TRACE_MSG(CA_DSX_1294_112_2_17_23_11_7_1967,"KDA DSX:SetKey key_used_count 0x%x,key_rcv_demux_cmd_ref%x\n")
TRACE_MSG(CA_DSX_1295_112_2_17_23_11_7_1968,"KDA DSX:SetKey key_total_count 0x%x,s_is_to_set_key.is_set 0x%x\n")
TRACE_MSG(CA_DSX_1305_112_2_17_23_11_7_1969,"KDA DSX :dsxSetKeys xSetNumKeys =0x%x \n")
TRACE_MSG(CA_DSX_1316_112_2_17_23_11_7_1970,"KDA DSX:dsxSetKeys  key_index=%d\n")
TRACE_MSG(CA_DSX_1332_112_2_17_23_11_7_1971,"KDA DSX:dsxSetKeys  s_is_to_set_key.key_id =%d,s_is_to_set_key.key_select =%dn")
TRACE_MSG(CA_DSX_1345_112_2_17_23_11_7_1972,"KDA DSX:dsxSetKeys pxKeyTable[%d].pValue = 0x%x \n")
TRACE_MSG(CA_DSX_1353_112_2_17_23_11_7_1973,"KDA DSX:dsxSetKeys  s_is_to_set_key.key_id =%d,s_is_to_set_key.key_select =%dn")
TRACE_MSG(CA_DSX_1380_112_2_17_23_11_7_1974,"KDA DSX:dsxSetKeys key[%d]%x \n")
TRACE_MSG(CA_DSX_1385_112_2_17_23_11_7_1975,"KDA DSX:dsxSetKeys  key_index%d\n")
TRACE_MSG(CA_DSX_1427_112_2_17_23_11_7_1976,"KDA DSX:dsxSetKeys pxKeyTable %x ,tick 0x%x\n")
TRACE_MSG(CA_DSX_1435_112_2_17_23_11_7_1977,"KDA DSX:dsxSetKeys s_csl_current_index 0x%x\n")
TRACE_MSG(CA_DSX_1441_112_2_17_23_11_7_1978,"KDA DSX:dsxSetKeys pxKeyTable[%d],index=0x%x \n")
TRACE_MSG(CA_DSX_1446_112_2_17_23_11_8_1979,"KDA DSX:dsxSetKeys key[%d]=0x%x \n")
TRACE_MSG(CA_DSX_1466_112_2_17_23_11_8_1980,"KDA DSX:dsxSetKeys  key_index= 0x%x , is_play %d\n")
TRACE_MSG(CA_DSX_1484_112_2_17_23_11_8_1981,"KDA DSX:dsxSetKeys  s_is_to_set_key.is_set =%d,s_is_to_set_key.key_id =%dn")
TRACE_MSG(CA_DSX_1496_112_2_17_23_11_8_1982,"KDA DSX:dsxSetKeys  s_is_to_set_key.key_id =%d,s_is_to_set_key.key_select =%dn")
TRACE_MSG(CA_DSX_1514_112_2_17_23_11_8_1983,"KDA DSX:dsxSetKeys CompareAndSaveKey \n")
TRACE_MSG(CA_DSX_1556_112_2_17_23_11_8_1984,"KDA DSX: :ProcessDemuxReqCallback %x\n")
TRACE_MSG(CA_DSX_1563_112_2_17_23_11_8_1985,"KDA CSL:time ProcessDemuxReqCallback tick%x\n")
TRACE_MSG(CA_DSX_1584_112_2_17_23_11_8_1986,"KDA DSX: :DSX_ClearValue \n")
TRACE_MSG(CA_DSX_1637_112_2_17_23_11_8_1987,"KDA DSX: IsExitFlag \n ")
TRACE_MSG(CA_DSX_1649_112_2_17_23_11_8_1988,"KDA DSX: IsExitFlag  no obj\n ")
TRACE_MSG(CA_DSX_1661_112_2_17_23_11_8_1989,"KDA DSX: :DSX_Init \n")
TRACE_MSG(CA_DSX_1682_112_2_17_23_11_8_1990,"KDA DSX:DSX_Exit \n")
TRACE_MSG(CA_ICC_55_112_2_17_23_11_8_1991,"icc register ok!\n")
TRACE_MSG(CA_ICC_144_112_2_17_23_11_8_1992,"***ICC KDA: send card inserted event tick 0x%x\n")
TRACE_MSG(CA_ICC_159_112_2_17_23_11_8_1993,"***ICC KDA: CA_Resume tick 0x%x\n")
TRACE_MSG(CA_ICC_174_112_2_17_23_11_8_1994,"icc cancel registeration ok!\n")
TRACE_MSG(CA_ICC_188_112_2_17_23_11_8_1995," T1 raw exchange mode not supported !\n")
TRACE_MSG(CA_ICC_201_112_2_17_23_11_8_1996,"Calling .......icc T0 send\n")
TRACE_MSG(CA_ICC_220_112_2_17_23_11_8_1997,"Calling .......icc T0 receive\n")
TRACE_MSG(CA_ICC_247_112_2_17_23_11_9_1998,"**KDA***  T0 exchange sessionid:%d\n")
TRACE_MSG(CA_ICC_254_112_2_17_23_11_9_1999,"**KDA*** Calling .......icc T0 exchange \n")
TRACE_MSG(CA_ICC_256_112_2_17_23_11_9_2000,"**KDA icc  input: len:%d 0x%x 0x%x 0x%x 0x%x n")
TRACE_MSG(CA_ICC_264_112_2_17_23_11_9_2001,"**KDA** Calling .......icc ERROR code:%xn")
TRACE_MSG(CA_ICC_270_112_2_17_23_11_9_2002,"**KDA** Calling .......icc T0 exchange ok len:%d data:0x%x 0x%x 0x%xn")
TRACE_MSG(CA_ICC_284_112_2_17_23_11_9_2003,"KDA_ICC T0Exchange Error \n")
TRACE_MSG(CA_ICC_309_112_2_17_23_11_9_2004,"iccSmartcardReset %d %d")
TRACE_MSG(CA_ICC_320_112_2_17_23_11_9_2005,"KDA Termination: ICC TERMINATED!\n")
TRACE_MSG(CA_OS_SC6600R_203_112_2_17_23_11_9_2006,"timer CB: entering start:%d stop:%d interval:%d n")
TRACE_MSG(CA_OS_SC6600R_236_112_2_17_23_11_9_2007,"***KDA Timer CB: send to task:0x%x event:0x%x n")
TRACE_MSG(CA_OS_SC6600R_344_112_2_17_23_11_10_2008,"ESG:IsFolderExist: find first sfs_handle = %d")
TRACE_MSG(CA_OS_SC6600R_445_112_2_17_23_11_10_2009,"--KDA- DalOsSetStorageContent.........file:%s \n")
TRACE_MSG(CA_OS_SC6600R_452_112_2_17_23_11_10_2010,"---KDA ALoSetStorageContent error,open file failed!\n")
TRACE_MSG(CA_OS_SC6600R_458_112_2_17_23_11_10_2011,"---KDA ALoSetStorageContent error,write file failed \n")
TRACE_MSG(CA_OS_SC6600R_529_112_2_17_23_11_10_2012," --KDA DalOsGetStorageContent..........%s \n")
TRACE_MSG(CA_OS_SC6600R_536_112_2_17_23_11_10_2013,"---KDA ALoGetStorageContent error,open file failed!")
TRACE_MSG(CA_OS_SC6600R_541_112_2_17_23_11_10_2014,"---KDA ALoGetStorageContent error,create file failed!")
TRACE_MSG(CA_OS_SC6600R_544_112_2_17_23_11_10_2015,"---KDA ALoSetStorageContent  Create file:%s OK! \n")
TRACE_MSG(CA_OS_SC6600R_547_112_2_17_23_11_10_2016,"---KDA ALoSetStorageContent RESET DATA FAILED\n")
TRACE_MSG(CA_OS_SC6600R_551_112_2_17_23_11_10_2017,"---KDA ALoSetStorageContent RESET file:%s OK! \n")
TRACE_MSG(CA_OS_SC6600R_558_112_2_17_23_11_10_2018,"---KDA ALoGetStorageContent error,read file failed!")
TRACE_MSG(CA_OS_SC6600R_629_112_2_17_23_11_10_2019,"****TASK ENTRY OVER: array:%d task_array:0x%x~~~~~~~~")
TRACE_MSG(CA_OS_SC6600R_642_112_2_17_23_11_10_2020,"osTaskCreate  xPriority = %d!\n")
TRACE_MSG(CA_OS_SC6600R_745_112_2_17_23_11_10_2021,"CREATE TASK OK task_array %d=0x%x pxTaskId=0x%x threadid:0x%x!n")
TRACE_MSG(CA_OS_SC6600R_760_112_2_17_23_11_10_2022,"TASK DELETE.....xTaskId=0x%x!\n")
TRACE_MSG(CA_OS_SC6600R_800_112_2_17_23_11_10_2023,"***KDA****TASK DELETE OK array:%d thread id:0x%x OVER \n")
TRACE_MSG(CA_OS_SC6600R_825_112_2_17_23_11_11_2024," KDA_OS osTaskIdentify: os task Entering.....thread_id =  0x%x !")
TRACE_MSG(CA_OS_SC6600R_838_112_2_17_23_11_11_2025,"KDA_OS os task identify ok pxTaskId = 0x%x thread_id = 0x%x n")
TRACE_MSG(CA_OS_SC6600R_907_112_2_17_23_11_11_2026," _--KDA osCharacterGet: %d %c")
TRACE_MSG(CA_OS_SC6600R_1007_112_2_17_23_11_11_2027,"***KDA free event task OK id 0x%x event:0x%x\n")
TRACE_MSG(CA_OS_SC6600R_1082_112_2_17_23_11_11_2028,"osEventReceive:WAIT_FOREVER, RECEIVED!!! event:%x")
TRACE_MSG(CA_OS_SC6600R_1086_112_2_17_23_11_11_2029,"osEventReceive:Wait_forever, GET SEM BUT NO EVENT!!! event:%x")
TRACE_MSG(CA_OS_SC6600R_1095_112_2_17_23_11_11_2030,"osEventReceive:RECEIVED or timeout!!! event:%x")
TRACE_MSG(CA_OS_SC6600R_1112_112_2_17_23_11_11_2031,"receive event: received event:%d start:%d stop%d int:%dn")
TRACE_MSG(CA_OS_SC6600R_1197_112_2_17_23_11_11_2032," create timer FAILED:  event:0x%x \n")
TRACE_MSG(CA_OS_SC6600R_1201_112_2_17_23_11_11_2033," create timer ok: timer id:0x%x event:0x%x \n")
TRACE_MSG(CA_OS_SC6600R_1552_112_2_17_23_11_12_2034,"***KDA Event THread: Failed to send Event \n")
TRACE_MSG(CA_OS_SC6600R_1587_112_2_17_23_11_12_2035,"\"\"\"\"\"#CA KDA Init FAILED \"\"\"\n")
TRACE_MSG(CA_OS_SC6600R_1600_112_2_17_23_11_12_2036,"---KDA CA EXIT: deleting sem \n")
TRACE_MSG(CA_OS_SC6600R_1610_112_2_17_23_11_12_2037,"---KDA CA EXIT: deleting timer \n")
TRACE_MSG(CA_OS_SC6600R_1620_112_2_17_23_11_12_2038,"---KDA CA EXIT: deleting task \n")
TRACE_MSG(CA_OS_SC6600R_1623_112_2_17_23_11_12_2039,"---KDA CA EXIT: deleting task-->Delete Semaphore \n")
TRACE_MSG(CA_OS_SC6600R_1628_112_2_17_23_11_12_2040,"---KDA CA EXIT: deleting task-->Delete Thread \n")
TRACE_MSG(CA_OS_SC6600R_1633_112_2_17_23_11_12_2041,"---KDA CA EXIT: Free Task \n")
TRACE_MSG(CMMB_OPERATION_279_112_2_17_23_11_13_2042,"CMMB_Init s_is_set_emm_demux %d")
TRACE_MSG(CMMB_OPERATION_281_112_2_17_23_11_13_2043,"CMMB_Init:%d")
TRACE_MSG(CMMB_OPERATION_362_112_2_17_23_11_13_2044,"Cleanup: bitmap %p")
TRACE_MSG(CMMB_OPERATION_396_112_2_17_23_11_13_2045,"Enter PlayNetwork with %d %d[%d]")
TRACE_MSG(CMMB_OPERATION_401_112_2_17_23_11_13_2046,"PlayNetwork state error!")
TRACE_MSG(CMMB_OPERATION_407_112_2_17_23_11_13_2047,"PlayNetwork: no ctrl information table")
TRACE_MSG(CMMB_OPERATION_423_112_2_17_23_11_13_2048,"PlayNetwork: play_thread_id should be 0!!!")
TRACE_MSG(CMMB_OPERATION_444_112_2_17_23_11_13_2049,"Create play thread failed!!")
TRACE_MSG(CMMB_OPERATION_453_112_2_17_23_11_13_2050,"Create play timer failed!!")
TRACE_MSG(CMMB_OPERATION_458_112_2_17_23_11_13_2051,"create playtimeout timer %p")
TRACE_MSG(CMMB_OPERATION_463_112_2_17_23_11_13_2052,"PlayNetwork: wait playparam")
TRACE_MSG(CMMB_OPERATION_483_112_2_17_23_11_13_2053,"PlayNetwork: wait to get play param")
TRACE_MSG(CMMB_OPERATION_488_112_2_17_23_11_13_2054,"PlayNetwork: cannot get play param")
TRACE_MSG(CMMB_OPERATION_519_112_2_17_23_11_13_2055,"Enter PlayNetworkByPlayParam")
TRACE_MSG(CMMB_OPERATION_524_112_2_17_23_11_14_2056,"PlayNetwork state error!")
TRACE_MSG(CMMB_OPERATION_552_112_2_17_23_11_14_2057,"Create play thread failed!!")
TRACE_MSG(CMMB_OPERATION_561_112_2_17_23_11_14_2058,"Create play timer failed!!")
TRACE_MSG(CMMB_OPERATION_565_112_2_17_23_11_14_2059,"create playtimeout timer %p")
TRACE_MSG(CMMB_OPERATION_570_112_2_17_23_11_14_2060,"PlayNetwork: wait playparam")
TRACE_MSG(CMMB_OPERATION_609_112_2_17_23_11_14_2061,"Enter PlayStream @%d")
TRACE_MSG(CMMB_OPERATION_614_112_2_17_23_11_14_2062,"PlayNetwork state error!")
TRACE_MSG(CMMB_OPERATION_644_112_2_17_23_11_14_2063,"Create play thread failed!!")
TRACE_MSG(CMMB_OPERATION_675_112_2_17_23_11_14_2064,"error open play file!")
TRACE_MSG(CMMB_OPERATION_683_112_2_17_23_11_14_2065,"file name length %d exceeds MAX length %d!!!")
TRACE_MSG(CMMB_OPERATION_697_112_2_17_23_11_14_2066,"error open desp file!")
TRACE_MSG(CMMB_OPERATION_709_112_2_17_23_11_14_2067,"Playback: desp file is corrupt, play from begining")
TRACE_MSG(CMMB_OPERATION_728_112_2_17_23_11_14_2068,"Create playback thread failed!!")
TRACE_MSG(CMMB_OPERATION_747_112_2_17_23_11_14_2069,"Create playbackread thread failed!!")
TRACE_MSG(CMMB_OPERATION_762_112_2_17_23_11_14_2070,"Fill initial buffer failed!!")
TRACE_MSG(CMMB_OPERATION_812_112_2_17_23_11_14_2071,"Create play timer failed!!")
TRACE_MSG(CMMB_OPERATION_816_112_2_17_23_11_14_2072,"create playtimeout timer %p")
TRACE_MSG(CMMB_OPERATION_827_112_2_17_23_11_14_2073,"Create playback timer failed!!")
TRACE_MSG(CMMB_OPERATION_864_112_2_17_23_11_14_2074,"Enter PlayStream2 @%d")
TRACE_MSG(CMMB_OPERATION_869_112_2_17_23_11_14_2075,"PlayStream state error!")
TRACE_MSG(CMMB_OPERATION_899_112_2_17_23_11_14_2076,"PlayStream: Create play thread failed!!")
TRACE_MSG(CMMB_OPERATION_930_112_2_17_23_11_14_2077,"PlayStream: error open play file!")
TRACE_MSG(CMMB_OPERATION_936_112_2_17_23_11_14_2078,"PlayStream: error get mf len of the play file!")
TRACE_MSG(CMMB_OPERATION_949_112_2_17_23_11_14_2079,"PlayStream: error get mf pts!")
TRACE_MSG(CMMB_OPERATION_956_112_2_17_23_11_14_2080,"PlayStream: error get mf pos by pts!")
TRACE_MSG(CMMB_OPERATION_965_112_2_17_23_11_14_2081,"PlayStream: error get mf pts!")
TRACE_MSG(CMMB_OPERATION_985_112_2_17_23_11_14_2082,"PlayStream: Create playback thread failed!!")
TRACE_MSG(CMMB_OPERATION_1004_112_2_17_23_11_14_2083,"PlayStream: Create playbackread thread failed!!")
TRACE_MSG(CMMB_OPERATION_1019_112_2_17_23_11_15_2084,"PlayStream: Fill initial buffer failed!!")
TRACE_MSG(CMMB_OPERATION_1073_112_2_17_23_11_15_2085,"PlayStream: error file header %X %X")
TRACE_MSG(CMMB_OPERATION_1085_112_2_17_23_11_15_2086,"%02X %02X %02X %02X %02X %02X %02X %02X")
TRACE_MSG(CMMB_OPERATION_1121_112_2_17_23_11_15_2087,"Create play timer failed!!")
TRACE_MSG(CMMB_OPERATION_1125_112_2_17_23_11_15_2088,"create playtimeout timer %p")
TRACE_MSG(CMMB_OPERATION_1131_112_2_17_23_11_15_2089,"Create playback timer failed!!")
TRACE_MSG(CMMB_OPERATION_1163_112_2_17_23_11_15_2090,"STOP: call cmmb_stop[%d]")
TRACE_MSG(CMMB_OPERATION_1173_112_2_17_23_11_15_2091,"STOP: begin stop")
TRACE_MSG(CMMB_OPERATION_1189_112_2_17_23_11_15_2092,"STOP: wait play_thread_id = 0")
TRACE_MSG(CMMB_OPERATION_1198_112_2_17_23_11_15_2093,"delete playtimeout timer %p")
TRACE_MSG(CMMB_OPERATION_1207_112_2_17_23_11_15_2094,"STOP: wait record stop")
TRACE_MSG(CMMB_OPERATION_1218_112_2_17_23_11_15_2095,"STOP: bitmap %p")
TRACE_MSG(CMMB_OPERATION_1235_112_2_17_23_11_15_2096,"Create play thread failed!!")
TRACE_MSG(CMMB_OPERATION_1243_112_2_17_23_11_15_2097,"STOP: parse MSFH for CA STOP 0x%x\n")
TRACE_MSG(CMMB_OPERATION_1285_112_2_17_23_11_15_2098,"Create play timer failed!!")
TRACE_MSG(CMMB_OPERATION_1288_112_2_17_23_11_15_2099,"create stoptimeout timer %p")
TRACE_MSG(CMMB_OPERATION_1309_112_2_17_23_11_15_2100,"STOP: wrong state!")
TRACE_MSG(CMMB_OPERATION_1322_112_2_17_23_11_15_2101,"STOP: begin stop")
TRACE_MSG(CMMB_OPERATION_1356_112_2_17_23_11_15_2102,"delete playbacktimeout timer %p")
TRACE_MSG(CMMB_OPERATION_1373_112_2_17_23_11_15_2103,"STOP: bitmap %p")
TRACE_MSG(CMMB_OPERATION_1391_112_2_17_23_11_15_2104,"Create play thread failed!!")
TRACE_MSG(CMMB_OPERATION_1412_112_2_17_23_11_15_2105,"Create play timer failed!!")
TRACE_MSG(CMMB_OPERATION_1415_112_2_17_23_11_15_2106,"create stoptimeout timer %p")
TRACE_MSG(CMMB_OPERATION_1454_112_2_17_23_11_15_2107,"STOP: wrong state!")
TRACE_MSG(CMMB_OPERATION_1460_112_2_17_23_11_15_2108,"STOP: should not be here!!!")
TRACE_MSG(CMMB_OPERATION_1477_112_2_17_23_11_15_2109,"ABORT: call cmmb_abort")
TRACE_MSG(CMMB_OPERATION_1488_112_2_17_23_11_15_2110,"ABORT: begin stop")
TRACE_MSG(CMMB_OPERATION_1494_112_2_17_23_11_16_2111,"delete playtimeout timer %p")
TRACE_MSG(CMMB_OPERATION_1503_112_2_17_23_11_16_2112,"ABORT: wait record stop")
TRACE_MSG(CMMB_OPERATION_1509_112_2_17_23_11_16_2113,"ABORT: parse MSFH for CA STOP 0x%x\n")
TRACE_MSG(CMMB_OPERATION_1532_112_2_17_23_11_16_2114,"ABORT: bitmap %p")
TRACE_MSG(CMMB_OPERATION_1540_112_2_17_23_11_16_2115,"ABORT: play_thread_id should be 0!!!")
TRACE_MSG(CMMB_OPERATION_1556_112_2_17_23_11_16_2116,"Create play thread failed!!")
TRACE_MSG(CMMB_OPERATION_1575_112_2_17_23_11_16_2117,"ABORT: abort finish")
TRACE_MSG(CMMB_OPERATION_1579_112_2_17_23_11_16_2118,"ABORT: wrong state!")
TRACE_MSG(CMMB_OPERATION_1594_112_2_17_23_11_16_2119,"ABORT: begin abort")
TRACE_MSG(CMMB_OPERATION_1623_112_2_17_23_11_16_2120,"delete playbacktimeout timer %p")
TRACE_MSG(CMMB_OPERATION_1637_112_2_17_23_11_16_2121,"ABORT: bitmap %p")
TRACE_MSG(CMMB_OPERATION_1645_112_2_17_23_11_16_2122,"ABORT: play_thread_id should be 0!!!")
TRACE_MSG(CMMB_OPERATION_1661_112_2_17_23_11_16_2123,"Create play thread failed!!")
TRACE_MSG(CMMB_OPERATION_1679_112_2_17_23_11_16_2124,"CMMB_StopPlaybackCallback: restore arm clock")
TRACE_MSG(CMMB_OPERATION_1681_112_2_17_23_11_16_2125,"ABORT: abort finish")
TRACE_MSG(CMMB_OPERATION_1685_112_2_17_23_11_16_2126,"ABORT: wrong state!")
TRACE_MSG(CMMB_OPERATION_1691_112_2_17_23_11_16_2127,"ABORT: should not be here!")
TRACE_MSG(CMMB_OPERATION_1708_112_2_17_23_11_16_2128,"CMMB_Pause: call CMMB_Pause")
TRACE_MSG(CMMB_OPERATION_1748_112_2_17_23_11_16_2129,"CMMB_Pause: should not be here!")
TRACE_MSG(CMMB_OPERATION_1765_112_2_17_23_11_16_2130,"CMMB_Resume: call CMMB_Resume")
TRACE_MSG(CMMB_OPERATION_1805_112_2_17_23_11_16_2131,"CMMB_Resume: should not be here!")
TRACE_MSG(CMMB_OPERATION_1834_112_2_17_23_11_16_2132,"open file error!")
TRACE_MSG(CMMB_OPERATION_1840_112_2_17_23_11_16_2133,"read file error!")
TRACE_MSG(CMMB_OPERATION_1845_112_2_17_23_11_16_2134,"not a valid recorded file!")
TRACE_MSG(CMMB_OPERATION_1874_112_2_17_23_11_16_2135,"enter CMMB_GetStreamLength2")
TRACE_MSG(CMMB_OPERATION_1888_112_2_17_23_11_16_2136,"CMMB_GetStreamLength2: zero MF_len")
TRACE_MSG(CMMB_OPERATION_1929_112_2_17_23_11_16_2137,"CMMB_GetStreamLength2: start_pts %X last_pts %X")
TRACE_MSG(CMMB_OPERATION_1986_112_2_17_23_11_17_2138,"Enter EnableRecord with %d %d")
TRACE_MSG(CMMB_OPERATION_1995_112_2_17_23_11_17_2139,"State error!")
TRACE_MSG(CMMB_OPERATION_2003_112_2_17_23_11_17_2140,"open file error!")
TRACE_MSG(CMMB_OPERATION_2008_112_2_17_23_11_17_2141,"EnableRecord: cannot seek to end!!!")
TRACE_MSG(CMMB_OPERATION_2013_112_2_17_23_11_17_2142,"EnableRecord: cannot get file pos!!!")
TRACE_MSG(CMMB_OPERATION_2016_112_2_17_23_11_17_2143,"EnableRecord: pos %d")
TRACE_MSG(CMMB_OPERATION_2025_112_2_17_23_11_17_2144,"EnableRecord: msfh size %d exceeds MAXMSFHSIZE!!!")
TRACE_MSG(CMMB_OPERATION_2040_112_2_17_23_11_17_2145,"EnableRecord: isma error")
TRACE_MSG(CMMB_OPERATION_2045_112_2_17_23_11_17_2146,"EnableRecord: isma len %d")
TRACE_MSG(CMMB_OPERATION_2047_112_2_17_23_11_17_2147,"EnableRecord: msfh len %d")
TRACE_MSG(CMMB_OPERATION_2050_112_2_17_23_11_17_2148,"write file error!")
TRACE_MSG(CMMB_OPERATION_2063_112_2_17_23_11_17_2149,"EnableRecord: file name length %d exceeds max length!!!")
TRACE_MSG(CMMB_OPERATION_2079_112_2_17_23_11_17_2150,"open file error!")
TRACE_MSG(CMMB_OPERATION_2087_112_2_17_23_11_17_2151,"EnableRecord: cannot seek to end!!!")
TRACE_MSG(CMMB_OPERATION_2169_112_2_17_23_11_17_2152,"Create record thread failed!!")
TRACE_MSG(CMMB_OPERATION_2196_112_2_17_23_11_17_2153,"Create record write thread failed!!")
TRACE_MSG(CMMB_OPERATION_2221_112_2_17_23_11_17_2154,"EnableRecord: should not be here!!!")
TRACE_MSG(CMMB_OPERATION_2246_112_2_17_23_11_17_2155,"Enter EnableRecord2 with %d %d")
TRACE_MSG(CMMB_OPERATION_2255_112_2_17_23_11_17_2156,"State error!")
TRACE_MSG(CMMB_OPERATION_2277_112_2_17_23_11_17_2157,"%02X %02X %02X %02X %02X %02X %02X %02X")
TRACE_MSG(CMMB_OPERATION_2287_112_2_17_23_11_17_2158,"open file error!")
TRACE_MSG(CMMB_OPERATION_2320_112_2_17_23_11_17_2159,"Create record thread failed!!")
TRACE_MSG(CMMB_OPERATION_2344_112_2_17_23_11_17_2160,"Create record write thread failed!!")
TRACE_MSG(CMMB_OPERATION_2370_112_2_17_23_11_17_2161,"EnableRecord2: should not be here!!!")
TRACE_MSG(CMMB_OPERATION_2386_112_2_17_23_11_17_2162,"Enter AbortRecord %d")
TRACE_MSG(CMMB_OPERATION_2440_112_2_17_23_11_17_2163,"open file error!")
TRACE_MSG(CMMB_OPERATION_2446_112_2_17_23_11_17_2164,"read file error!")
TRACE_MSG(CMMB_OPERATION_2452_112_2_17_23_11_17_2165,"not a valid recorded file! %d")
TRACE_MSG(CMMB_OPERATION_2458_112_2_17_23_11_17_2166,"not a CMMB recorded file!")
TRACE_MSG(CMMB_OPERATION_2487_112_2_17_23_11_18_2167,"enter CMMB_IdentifyStream2")
TRACE_MSG(CMMB_OPERATION_2522_112_2_17_23_11_18_2168,"CMMB_EnumAudioLanguages: wrong state %d")
TRACE_MSG(CMMB_OPERATION_2527_112_2_17_23_11_18_2169,"CMMB_EnumAudioLanguages: msfh is null")
TRACE_MSG(CMMB_OPERATION_2539_112_2_17_23_11_18_2170,"CMMB_EnumAudioLanguages: %d")
TRACE_MSG(CMMB_OPERATION_2589_112_2_17_23_11_18_2171,"CMMB_SetAudioLanguage: wrong state %d")
TRACE_MSG(CMMB_OPERATION_2594_112_2_17_23_11_18_2172,"CMMB_SetAudioLanguage: msfh null")
TRACE_MSG(CMMB_OPERATION_2599_112_2_17_23_11_18_2173,"CMMB_SetAudioLanguage: playparam null")
TRACE_MSG(CMMB_OPERATION_2605_112_2_17_23_11_18_2174,"CMMB_SetAudioLanguage: %d id %d")
TRACE_MSG(CMMB_OPERATION_2704_112_2_17_23_11_18_2175,"FillBuffer: cannot set file pointer")
TRACE_MSG(CMMB_OPERATION_2748_112_2_17_23_11_18_2176,"FillBuffer: desp pos %d")
TRACE_MSG(CMMB_OPERATION_2752_112_2_17_23_11_18_2177,"FillBuffer: cannot set file pointer")
TRACE_MSG(CMMB_OPERATION_2779_112_2_17_23_11_18_2178,"FillBuffer: data_len %d exceeds MAXMSFSIZE")
TRACE_MSG(CMMB_OPERATION_2785_112_2_17_23_11_18_2179,"FillBuffer: FFS_fread error!")
TRACE_MSG(CMMB_OPERATION_2802_112_2_17_23_11_18_2180,"Playback: TS %d 0x%X")
TRACE_MSG(CMMB_OPERATION_2837_112_2_17_23_11_18_2181,"FillBuffer2: data_offset %d")
TRACE_MSG(CMMB_OPERATION_2847_112_2_17_23_11_18_2182,"FillBuffer2: data_len %d exceeds MAXMSFSIZE")
TRACE_MSG(CMMB_OPERATION_2853_112_2_17_23_11_18_2183,"FillBuffer2: FFS_fread error!")
TRACE_MSG(CMMB_OPERATION_2870_112_2_17_23_11_18_2184,"Playback: TS %d 0x%X")
TRACE_MSG(CMMB_OPERATION_2889_112_2_17_23_11_18_2185,"ReqCAInfo: %d %d")
TRACE_MSG(CMMB_OPERATION_2895_112_2_17_23_11_18_2186,"CA_Get_Info: error %d")
TRACE_MSG(CMMB_OPERATION_2924_112_2_17_23_11_18_2187,"ReqCAInfo: %d %d")
TRACE_MSG(CMMB_OPERATION_2976_112_2_17_23_11_19_2188,"CMMB_UpdateMCICallback: should not be here!!!")
TRACE_MSG(CMMB_OPERATION_2983_112_2_17_23_11_19_2189,"PlayNetwork: Update mci")
TRACE_MSG(CMMB_OPERATION_2999_112_2_17_23_11_19_2190,"CMMB_UpdateMCICallback: should not be here!!!")
TRACE_MSG(CMMB_OPERATION_3029_112_2_17_23_11_19_2191,"UpdateMSFHeaderCallback: count %d ind %d")
TRACE_MSG(CMMB_OPERATION_3075_112_2_17_23_11_19_2192,"PlayNetwork: len=%d ind=%p")
TRACE_MSG(CMMB_OPERATION_3078_112_2_17_23_11_19_2193,"0x%02X")
TRACE_MSG(CMMB_OPERATION_3094_112_2_17_23_11_19_2194,"PlayNetwork: audio sample rate %d")
TRACE_MSG(CMMB_OPERATION_3130_112_2_17_23_11_19_2195,"Enter CMMB_DataReadyCallback() %d")
TRACE_MSG(CMMB_OPERATION_3171_112_2_17_23_11_19_2196,"CMMB_SetPacketIDCallback: service_id %d , this->cmmb_service_id %d ,s_is_set_emm_demux %d\n")
TRACE_MSG(CMMB_OPERATION_3195_112_2_17_23_11_19_2197,"CMMB_SetPacketIDCallback: demux_param.mux_subframe_id%d\n")
TRACE_MSG(CMMB_OPERATION_3221_112_2_17_23_11_19_2198,"CMMB_SetPacketIDCallback s_is_set_emm_demux %d")
TRACE_MSG(CMMB_OPERATION_3227_112_2_17_23_11_19_2199,"CMMB_SetPacketIDCallback: Get CA Param ERROR!")
TRACE_MSG(CMMB_OPERATION_3304_112_2_17_23_11_19_2200,"CMMB_DataReadyCallback: about play to end")
TRACE_MSG(CMMB_OPERATION_3333_112_2_17_23_11_19_2201,"PLAYBACK: STC %d")
TRACE_MSG(CMMB_OPERATION_3346_112_2_17_23_11_19_2202,"PLAYBACK: TIME_IND %d %d %d")
TRACE_MSG(CMMB_OPERATION_3352_112_2_17_23_11_19_2203,"PLAYBACK: STREAM_END")
TRACE_MSG(CMMB_OPERATION_3396_112_2_17_23_11_19_2204,"Enter CMMB_AVACKCallback")
TRACE_MSG(CMMB_OPERATION_3427_112_2_17_23_11_19_2205,"PlayNetwork: enter CMMB_AVACKCallback but state error")
TRACE_MSG(CMMB_OPERATION_3431_112_2_17_23_11_19_2206,"Enter CMMB_AVACKCallback")
TRACE_MSG(CMMB_OPERATION_3476_112_2_17_23_11_20_2207,"STOP: enter CMMB_StopCallback but state error")
TRACE_MSG(CMMB_OPERATION_3484_112_2_17_23_11_20_2208,"Enter CMMB_StopCallback")
TRACE_MSG(CMMB_OPERATION_3508_112_2_17_23_11_20_2209,"STOP: enter CMMB_StopSPIAudioCallback but state error")
TRACE_MSG(CMMB_OPERATION_3513_112_2_17_23_11_20_2210,"STOP: spi audio thread exit")
TRACE_MSG(CMMB_OPERATION_3525_112_2_17_23_11_20_2211,"STOP: receive all stop ack")
TRACE_MSG(CMMB_OPERATION_3552_112_2_17_23_11_20_2212,"CMMB_StopPlaybackCallback: receive signal 0x%X")
TRACE_MSG(CMMB_OPERATION_3561_112_2_17_23_11_20_2213,"STOP: begin stop")
TRACE_MSG(CMMB_OPERATION_3602_112_2_17_23_11_20_2214,"delete playbacktimeout timer %p")
TRACE_MSG(CMMB_OPERATION_3624_112_2_17_23_11_20_2215,"STOP: bitmap %p")
TRACE_MSG(CMMB_OPERATION_3645_112_2_17_23_11_20_2216,"Create play thread failed!!")
TRACE_MSG(CMMB_OPERATION_3666_112_2_17_23_11_20_2217,"Create play timer failed!!")
TRACE_MSG(CMMB_OPERATION_3669_112_2_17_23_11_20_2218,"create stoptimeout timer %p")
TRACE_MSG(CMMB_OPERATION_3715_112_2_17_23_11_20_2219,"CMMB_CleanupCallback unknown resp_id %d")
TRACE_MSG(CMMB_OPERATION_3750_112_2_17_23_11_20_2220,"PlayNetwork: NO CAT")
TRACE_MSG(CMMB_OPERATION_3807_112_2_17_23_11_20_2221,"PlayNetwork:  receive CMMB_ABORT %d")
TRACE_MSG(CMMB_OPERATION_3850_112_2_17_23_11_20_2222,"CMMB_PlayThread s_is_set_emm_demux %d")
TRACE_MSG(CMMB_OPERATION_3914_112_2_17_23_11_20_2223,"PlayNetwork: wait MSFH")
TRACE_MSG(CMMB_OPERATION_3940_112_2_17_23_11_20_2224,"PlayNetwork: receive MSFH[%d]")
TRACE_MSG(CMMB_OPERATION_3941_112_2_17_23_11_20_2225,"PlayNetwork: len=%d ind=0x%02X")
TRACE_MSG(CMMB_OPERATION_3944_112_2_17_23_11_21_2226,"0x%02X")
TRACE_MSG(CMMB_OPERATION_3987_112_2_17_23_11_21_2227,"PlayNetwork: wait bitmap %X")
TRACE_MSG(CMMB_OPERATION_4015_112_2_17_23_11_21_2228,"PlayNetwork: audio sample rate %d")
TRACE_MSG(CMMB_OPERATION_4074_112_2_17_23_11_21_2229,"PlayNetwork: Video play %d")
TRACE_MSG(CMMB_OPERATION_4079_112_2_17_23_11_21_2230,"PlayNetwork: codec type %d")
TRACE_MSG(CMMB_OPERATION_4126_112_2_17_23_11_21_2231,"PlayNetwork: MMAL play error!")
TRACE_MSG(CMMB_OPERATION_4135_112_2_17_23_11_21_2232,"PlayNetwork: wait AVACK")
TRACE_MSG(CMMB_OPERATION_4152_112_2_17_23_11_21_2233,"PlayNetwork: receive audio ack %X[%d]")
TRACE_MSG(CMMB_OPERATION_4157_112_2_17_23_11_21_2234,"PlayNetwork: receive video ack %X")
TRACE_MSG(CMMB_OPERATION_4176_112_2_17_23_11_21_2235,"delete playtimeout timer %p")
TRACE_MSG(CMMB_OPERATION_4183_112_2_17_23_11_21_2236,"wrong state in CMMB_PlayThread!!!")
TRACE_MSG(CMMB_OPERATION_4237_112_2_17_23_11_21_2237,"STOP: recv dis channel %p")
TRACE_MSG(CMMB_OPERATION_4242_112_2_17_23_11_21_2238,"STOP: recv dis vo %p")
TRACE_MSG(CMMB_OPERATION_4246_112_2_17_23_11_21_2239,"STOP: recv dis ao %p")
TRACE_MSG(CMMB_OPERATION_4252_112_2_17_23_11_21_2240,"STOP: recv dis service %p")
TRACE_MSG(CMMB_OPERATION_4257_112_2_17_23_11_21_2241,"STOP: recv dis msfh %p")
TRACE_MSG(CMMB_OPERATION_4261_112_2_17_23_11_21_2242,"STOP: recv vid stop %p")
TRACE_MSG(CMMB_OPERATION_4265_112_2_17_23_11_21_2243,"STOP: recv aud stop %p")
TRACE_MSG(CMMB_OPERATION_4269_112_2_17_23_11_21_2244,"STOP: recv pcm stop %p")
TRACE_MSG(CMMB_OPERATION_4274_112_2_17_23_11_21_2245,"STOP: recv pcm abort %p")
TRACE_MSG(CMMB_OPERATION_4278_112_2_17_23_11_21_2246,"STOP: recv playback stop %p")
TRACE_MSG(CMMB_OPERATION_4282_112_2_17_23_11_21_2247,"STOP: recv ca stop %p")
TRACE_MSG(CMMB_OPERATION_4285_112_2_17_23_11_21_2248,"STOP: recv %X")
TRACE_MSG(CMMB_OPERATION_4294_112_2_17_23_11_21_2249,"STOP: receive all stop ack")
TRACE_MSG(CMMB_OPERATION_4297_112_2_17_23_11_21_2250,"delete stoptimeout timer %p")
TRACE_MSG(CMMB_OPERATION_4316_112_2_17_23_11_21_2251,"STOP: send MTV_PLAY_ERROR")
TRACE_MSG(CMMB_OPERATION_4321_112_2_17_23_11_21_2252,"STOP: send MTV_STOP_ERROR")
TRACE_MSG(CMMB_OPERATION_4326_112_2_17_23_11_21_2253,"STOP: send MTV_PLAY_ERROR_FOR_CA")
TRACE_MSG(CMMB_OPERATION_4331_112_2_17_23_11_21_2254,"STOP: send MTV_PLAY_ERROR_FOR_NOAV")
TRACE_MSG(CMMB_OPERATION_4336_112_2_17_23_11_21_2255,"STOP: should not be here!!!")
TRACE_MSG(CMMB_OPERATION_4341_112_2_17_23_11_21_2256,"STOP: send MTV_STOP_ACK")
TRACE_MSG(CMMB_OPERATION_4389_112_2_17_23_11_21_2257,"ABORT: recv pcm stop %p")
TRACE_MSG(CMMB_OPERATION_4393_112_2_17_23_11_21_2258,"ABORT: recv playback stop %p")
TRACE_MSG(CMMB_OPERATION_4396_112_2_17_23_11_21_2259,"ABORT: recv %X")
TRACE_MSG(CMMB_OPERATION_4439_112_2_17_23_11_22_2260,"FindAudioIDByLang: %d")
TRACE_MSG(CMMB_OPERATION_4498_112_2_17_23_11_22_2261," FillInitial: %d %d %d")
TRACE_MSG(CMMB_OPERATION_4511_112_2_17_23_11_22_2262,"Fill seine buffer %d")
TRACE_MSG(CMMB_OPERATION_4538_112_2_17_23_11_22_2263,"Fill seine buffer desp %p, data %p")
TRACE_MSG(CMMB_OPERATION_4539_112_2_17_23_11_22_2264,"BB buffer desp %p, data %p")
TRACE_MSG(CMMB_OPERATION_4540_112_2_17_23_11_22_2265,"Data len desp %d, data %d")
TRACE_MSG(CMMB_OPERATION_4581_112_2_17_23_11_22_2266,"error sample rate idx %d")
TRACE_MSG(CMMB_OPERATION_4607_112_2_17_23_11_22_2267,"BandWidth %d freq %d mfid %d RSMode %d InterleaveMode %dn  LDPCMode %d MappingMode %d ScrambleMode %d TSNumber %d TSStartId %d")
TRACE_MSG(CMMB_OPERATION_4660_112_2_17_23_11_22_2268,"MSFH: len=%d")
TRACE_MSG(CMMB_OPERATION_4661_112_2_17_23_11_22_2269,"MSFH: st_ind=%d")
TRACE_MSG(CMMB_OPERATION_4662_112_2_17_23_11_22_2270,"MSFH: vd_ind=%d")
TRACE_MSG(CMMB_OPERATION_4663_112_2_17_23_11_22_2271,"MSFH: au_ind=%d")
TRACE_MSG(CMMB_OPERATION_4664_112_2_17_23_11_22_2272,"MSFH: da_ind=%d")
TRACE_MSG(CMMB_OPERATION_4665_112_2_17_23_11_22_2273,"MSFH: ex_ind=%d")
TRACE_MSG(CMMB_OPERATION_4666_112_2_17_23_11_22_2274,"MSFH: sc_ind=%d")
TRACE_MSG(CMMB_OPERATION_4667_112_2_17_23_11_22_2275,"MSFH: en_mod=%d")
TRACE_MSG(CMMB_OPERATION_4671_112_2_17_23_11_22_2276,"MSFH: MSFH lens should not be zero!!!")
TRACE_MSG(CMMB_OPERATION_4681_112_2_17_23_11_22_2277,"MSFH: st=%d")
TRACE_MSG(CMMB_OPERATION_4697_112_2_17_23_11_22_2278,"MSFH: vd_len=%d")
TRACE_MSG(CMMB_OPERATION_4698_112_2_17_23_11_22_2279,"MSFH: vd_cnt=%d")
TRACE_MSG(CMMB_OPERATION_4714_112_2_17_23_11_22_2280,"MSFH: au_len=%d")
TRACE_MSG(CMMB_OPERATION_4715_112_2_17_23_11_22_2281,"MSFH: au_cnt=%d")
TRACE_MSG(CMMB_OPERATION_4731_112_2_17_23_11_22_2282,"MSFH: da_len=%d")
TRACE_MSG(CMMB_OPERATION_4732_112_2_17_23_11_22_2283,"MSFH: ecm_ind=%d")
TRACE_MSG(CMMB_OPERATION_4762_112_2_17_23_11_22_2284,"MSFH: vd_algo=%d")
TRACE_MSG(CMMB_OPERATION_4763_112_2_17_23_11_22_2285,"MSFH: rate_ind=%d")
TRACE_MSG(CMMB_OPERATION_4764_112_2_17_23_11_22_2286,"MSFH: disp_ind=%d")
TRACE_MSG(CMMB_OPERATION_4765_112_2_17_23_11_22_2287,"MSFH: res_ind=%d")
TRACE_MSG(CMMB_OPERATION_4766_112_2_17_23_11_22_2288,"MSFH: fps_ind=%d")
TRACE_MSG(CMMB_OPERATION_4776_112_2_17_23_11_22_2289,"MSFH: rate=%d")
TRACE_MSG(CMMB_OPERATION_4790_112_2_17_23_11_22_2290,"MSFH: x=%d")
TRACE_MSG(CMMB_OPERATION_4791_112_2_17_23_11_22_2291,"MSFH: y=%d")
TRACE_MSG(CMMB_OPERATION_4792_112_2_17_23_11_22_2292,"MSFH: pri=%d")
TRACE_MSG(CMMB_OPERATION_4805_112_2_17_23_11_22_2293,"MSFH: w=%d")
TRACE_MSG(CMMB_OPERATION_4806_112_2_17_23_11_22_2294,"MSFH: h=%d")
TRACE_MSG(CMMB_OPERATION_4818_112_2_17_23_11_22_2295,"MSFH: fps=%d")
TRACE_MSG(CMMB_OPERATION_4847_112_2_17_23_11_22_2296,"MSFH: au_algo=%d")
TRACE_MSG(CMMB_OPERATION_4848_112_2_17_23_11_22_2297,"MSFH: rate_ind=%d")
TRACE_MSG(CMMB_OPERATION_4849_112_2_17_23_11_22_2298,"MSFH: sample_ind=%d")
TRACE_MSG(CMMB_OPERATION_4850_112_2_17_23_11_22_2299,"MSFH: desp_ind=%d")
TRACE_MSG(CMMB_OPERATION_4861_112_2_17_23_11_22_2300,"MSFH: rate=%d")
TRACE_MSG(CMMB_OPERATION_4873_112_2_17_23_11_22_2301,"MSFH: sample=%d")
TRACE_MSG(CMMB_OPERATION_4888_112_2_17_23_11_22_2302,"MSFH: desp=%d")
TRACE_MSG(CMMB_OPERATION_5005_112_2_17_23_11_23_2303,"MATCH %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X")
TRACE_MSG(CMMB_OPERATION_5014_112_2_17_23_11_23_2304,"Enter SendAbortToPlayThread")
TRACE_MSG(CMMB_OPERATION_5034_112_2_17_23_11_23_2305,"Enter PlayTimeOutCallback")
TRACE_MSG(CMMB_OPERATION_5050_112_2_17_23_11_23_2306,"Enter StopTimeOutCallback with 0x%X")
TRACE_MSG(CMMB_OPERATION_5074_112_2_17_23_11_23_2307,"Enter CAStopCallback with 0x%X")
TRACE_MSG(CMMB_OPERATION_5092_112_2_17_23_11_23_2308,"CAInfoCopy: should not be here!!!")
TRACE_MSG(CMMB_OPERATION_5131_112_2_17_23_11_23_2309,"CAInfoCopy: should not be here!!!")
TRACE_MSG(CMMB_OPERATION_5134_112_2_17_23_11_23_2310,"CAInfoCopy: should not be here!!!")
TRACE_MSG(CMMB_OPERATION_5160_112_2_17_23_11_23_2311,"Enter CACallback")
TRACE_MSG(CMMB_OPERATION_5174_112_2_17_23_11_23_2312,"CACallback: Detect card not present")
TRACE_MSG(CMMB_OPERATION_5187_112_2_17_23_11_23_2313,"CACallback: Detect card not granted")
TRACE_MSG(CMMB_OPERATION_5200_112_2_17_23_11_23_2314,"CACallback: Detect card granted")
TRACE_MSG(CMMB_OPERATION_5215_112_2_17_23_11_23_2315,"Enter ISMACallback 0x%X")
TRACE_MSG(CMMB_OPERATION_5339_112_2_17_23_11_23_2316,"mfsh_pts: mf header crc error")
TRACE_MSG(CMMB_OPERATION_5355_112_2_17_23_11_23_2317,"mfsh_pts: msf header crc error")
TRACE_MSG(CMMB_OPERATION_5376_112_2_17_23_11_23_2318,"GetStreamTSLen: buffer size less than 32B")
TRACE_MSG(CMMB_OPERATION_5382_112_2_17_23_11_23_2319,"GetStreamTSLen: seek to begin failed")
TRACE_MSG(CMMB_OPERATION_5394_112_2_17_23_11_23_2320,"GetStreamTSLen: file size less than 32B")
TRACE_MSG(CMMB_OPERATION_5400_112_2_17_23_11_23_2321,"GetStreamTSLen: file not start with MF")
TRACE_MSG(CMMB_OPERATION_5407_112_2_17_23_11_24_2322,"GetStreamTSLen: first MF is MF0")
TRACE_MSG(CMMB_OPERATION_5419_112_2_17_23_11_24_2323,"GetStreamTSLen: first MF is incomplete")
TRACE_MSG(CMMB_OPERATION_5425_112_2_17_23_11_24_2324,"GetStreamTSLen: no more data after MF1")
TRACE_MSG(CMMB_OPERATION_5437_112_2_17_23_11_24_2325,"Find next MF head @%d")
TRACE_MSG(CMMB_OPERATION_5442_112_2_17_23_11_24_2326,"GetStreamTSLen: next MF_id(%d) is not the same with previous one(%d)")
TRACE_MSG(CMMB_OPERATION_5492_112_2_17_23_11_24_2327,"GetStreamMFPTS: mfsh_pts get error")
TRACE_MSG(CMMB_OPERATION_5517_112_2_17_23_11_24_2328,"GetFirstMFPosByPTS: mf_len %d mf_num %d expectpts %X")
TRACE_MSG(CMMB_OPERATION_5524_112_2_17_23_11_24_2329,"GetFirstMFPosByPTS: [%d-%d] %d")
TRACE_MSG(CMMB_OPERATION_5529_112_2_17_23_11_24_2330,"GetFirstMFPosByPTS: mid pts %X")
TRACE_MSG(CSM_122_112_2_17_23_11_24_2331,"CSM Initialization started!\n")
TRACE_MSG(CSM_146_112_2_17_23_11_24_2332," - state lock initialized!\n")
TRACE_MSG(CSM_155_112_2_17_23_11_24_2333," - timer initialized!\n")
TRACE_MSG(CSM_160_112_2_17_23_11_24_2334," - wait4 semaphore initialized!\n")
TRACE_MSG(CSM_175_112_2_17_23_11_24_2335," - thread created!\n")
TRACE_MSG(CSM_181_112_2_17_23_11_24_2336," - service initialized!\n")
TRACE_MSG(CSM_183_112_2_17_23_11_24_2337,"CSM Initialization finished!\n")
TRACE_MSG(CSM_199_112_2_17_23_11_24_2338,"CSM clean up started!\n")
TRACE_MSG(CSM_203_112_2_17_23_11_24_2339," - timer released!\n")
TRACE_MSG(CSM_217_112_2_17_23_11_24_2340," - order csm thread to exit!\n")
TRACE_MSG(CSM_222_112_2_17_23_11_25_2341," - csm thread exited!\n")
TRACE_MSG(CSM_226_112_2_17_23_11_25_2342," - wait4 semaphore released!\n")
TRACE_MSG(CSM_230_112_2_17_23_11_25_2343," - state lock released!\n")
TRACE_MSG(CSM_234_112_2_17_23_11_25_2344," - service deleted!\n")
TRACE_MSG(CSM_236_112_2_17_23_11_25_2345,"CSM clean up finished!\n")
TRACE_MSG(CSM_245_112_2_17_23_11_25_2346,"Enter ChannelStopCallback")
TRACE_MSG(CSM_289_112_2_17_23_11_25_2347,"CSM_Set: duplicated set! \n")
TRACE_MSG(CSM_293_112_2_17_23_11_25_2348,"CSM_Set: active timer!\n")
TRACE_MSG(CSM_323_112_2_17_23_11_25_2349," sync CSM_Set: frequency %d, bandwidth %dn")
TRACE_MSG(CSM_365_112_2_17_23_11_25_2350,"CSM_Set: active timer!\n")
TRACE_MSG(CSM_472_112_2_17_23_11_25_2351," sync CSM_Stop: state %d, last_state %dn")
TRACE_MSG(CSM_559_112_2_17_23_11_25_2352,"CSM thread started!\n")
TRACE_MSG(CSM_566_112_2_17_23_11_25_2353," CSMThrd: Registered DTL service !\n")
TRACE_MSG(CSM_586_112_2_17_23_11_25_2354," CSMThrd-timer: state %d, last_state %dn")
TRACE_MSG(CSM_665_112_2_17_23_11_26_2355," CSMThrd-set: state %d, last_state %dn")
TRACE_MSG(CSM_702_112_2_17_23_11_26_2356," CSMThrd-stop: state %d, last_state %dn")
TRACE_MSG(CSM_715_112_2_17_23_11_26_2357," CSMThrd-exit: state %d, last_state %dn")
TRACE_MSG(CSM_726_112_2_17_23_11_26_2358," CSMThrd-demod: state %d, last_state %dn")
TRACE_MSG(CSM_749_112_2_17_23_11_26_2359,"CSMThrd: Unknow signal id!\n")
TRACE_MSG(CSM_762_112_2_17_23_11_26_2360," CSMThrd: Unregistered DTL service !\n")
TRACE_MSG(CSM_780_112_2_17_23_11_26_2361," CSMThrd: wakeup the one who closed CSM!\n")
TRACE_MSG(CSM_849_112_2_17_23_11_26_2362,"CSMSrv: no callback registered for thread %lu, event %lun")
TRACE_MSG(CSM_STRATEGY_112_112_2_17_23_11_26_2363,"CSM WATCHDOG: cur_v[%d], warning_v[%d], cnt[%d]!!!\n")
TRACE_MSG(CSM_STRATEGY_153_112_2_17_23_11_26_2364,"CSM WATCHDOG: hey! DSP is died!\n")
TRACE_MSG(CSM_STRATEGY_227_112_2_17_23_11_27_2365,"CSM STRATEGY: Locked2Prot!\n")
TRACE_MSG(CSM_STRATEGY_282_112_2_17_23_11_27_2366,"CSM STRATEGY: Prot2Locked!\n")
TRACE_MSG(CSM_STRATEGY_340_112_2_17_23_11_27_2367,"CSM STRATEGY: Prot2Locking - do relock!\n")
TRACE_MSG(CSM_STRATEGY_412_112_2_17_23_11_27_2368,"CSM STRATEGY: Locking2Locked!\n")
TRACE_MSG(CSM_STRATEGY_465_112_2_17_23_11_27_2369,"CSM STRATEGY: Locking2Error - relock again!\n")
TRACE_MSG(CSM_STRATEGY_512_112_2_17_23_11_27_2370,"CSM STRATEGY: Locking2Error!\n")
TRACE_MSG(GDAC_68_112_2_17_23_11_27_2371,"GDAC_Init")
TRACE_MSG(GDAC_80_112_2_17_23_11_27_2372,"GDAC_Cleanup")
TRACE_MSG(GDAC_92_112_2_17_23_11_27_2373,"GDAC_Open")
TRACE_MSG(GDAC_104_112_2_17_23_11_28_2374,"GDAC_Close")
TRACE_MSG(GDAC_146_112_2_17_23_11_28_2375,"GDAC type %d")
TRACE_MSG(HYDAC_75_112_2_17_23_11_28_2376,"\"\"\"\"\"\"\": HYDAC_Open()")
TRACE_MSG(HYDAC_V_64_112_2_17_23_11_28_2377,"\"\"\"\"\"\": HYDAC_Init()")
TRACE_MSG(MTV_DBC_398_112_2_17_23_11_31_2378,"AllocReceiveBuf: data_buf is null")
TRACE_MSG(MTV_DBC_455_112_2_17_23_11_31_2379,"AddData: (rcv_len(%d) + data_len(%d)) < total_len(%d)")
TRACE_MSG(MTV_DBC_609_112_2_17_23_11_31_2380,"ProcessStreamInfo: no stream or callback")
TRACE_MSG(MTV_DBC_678_112_2_17_23_11_31_2381,"DownloadXPE: parsing error")
TRACE_MSG(MTV_DBC_688_112_2_17_23_11_31_2382,"DownloadXPE: isn't first packet")
TRACE_MSG(MTV_DBC_712_112_2_17_23_11_31_2383,"DownloadXPE: not the same packet(ctl_trans_id=%d, xpe_trans_id=%d)")
TRACE_MSG(MTV_DBC_724_112_2_17_23_11_31_2384,"DownloadXPE: error in last packet len(rcv_len=%d, total_len=%d)")
TRACE_MSG(MTV_DBC_747_112_2_17_23_11_32_2385,"DownloadPacket: unknow packet type")
TRACE_MSG(MTV_DBC_755_112_2_17_23_11_32_2386,"DownloadPacket: xpe packet finished")
TRACE_MSG(MTV_DBC_766_112_2_17_23_11_32_2387,"DownloadPacket: xpe-fec packet finished")
TRACE_MSG(MTV_DBC_794_112_2_17_23_11_32_2388,"DownloadPacket: process stream")
TRACE_MSG(MTV_DBC_824_112_2_17_23_11_32_2389,"InitDataSlot: succ=%d")
TRACE_MSG(MTV_DBC_881_112_2_17_23_11_32_2390,"ConfigDataFilter: sub_type error")
TRACE_MSG(MTV_DBC_911_112_2_17_23_11_32_2391,"ConfigDBCFilter: get data filter error(%d), i")
TRACE_MSG(MTV_DBC_1009_112_2_17_23_11_32_2392,"MTVDBC_ParseDBCData: buffer_id=%d")
TRACE_MSG(MTV_DBC_1012_112_2_17_23_11_32_2393,"MTVDBC_ParseDBCData: not running(state=%d)")
TRACE_MSG(MTV_DBC_1028_112_2_17_23_11_32_2394,"MTVDBC_ParseDBCData: error type=%d")
TRACE_MSG(MTV_DBC_1040_112_2_17_23_11_32_2395,"MTVDBC_ParseDBCData: no memory")
TRACE_MSG(MTV_DBC_1055_112_2_17_23_11_32_2396,"MTVDBC_ParseDBCData: header=0x%x, 0x%x, 0x%x, 0x%x")
TRACE_MSG(MTV_DBC_1072_112_2_17_23_11_32_2397,"DBCRegisterToDDM: is_reg=%d")
TRACE_MSG(MTV_DBC_1127_112_2_17_23_11_32_2398,"MTVDBC_StartSession: no network info, frequency=%ld")
TRACE_MSG(MTV_DBC_1136_112_2_17_23_11_32_2399,"MTVDBC_StartSession")
TRACE_MSG(MTV_DBC_1145_112_2_17_23_11_32_2400,"MTVDBC_StartSession: stream info is max")
TRACE_MSG(MTV_DBC_1192_112_2_17_23_11_32_2401,"MTVDBC_StartSession: running")
TRACE_MSG(MTV_DBC_1212_112_2_17_23_11_33_2402,"MTVDBC_StopSession: no network info, frequency=%ld")
TRACE_MSG(MTV_DBC_1217_112_2_17_23_11_33_2403,"MTVDBC_StopSession")
TRACE_MSG(MTV_DBC_1268_112_2_17_23_11_33_2404,"MTVDBC_SetStreamNotifyLen: notify_len=%d not in range(0-%d)")
TRACE_MSG(MTV_DBC_1276_112_2_17_23_11_33_2405,"MTVDBC_SetStreamNotifyLen: no network info, frequency=%ld")
TRACE_MSG(MTV_DBC_STORAGE_85_112_2_17_23_11_33_2406,"MTVDBC_GetXPEInfo: paramer error")
TRACE_MSG(MTV_DBC_STORAGE_106_112_2_17_23_11_33_2407,"MTVDBC_GetXPEInfo: not xpe")
TRACE_MSG(MTV_DBC_STORAGE_134_112_2_17_23_11_33_2408,"MTVDBC_GetXPEInfo: temp_sum(%d) != check_sum(%d)")
TRACE_MSG(MTV_DBC_STORAGE_151_112_2_17_23_11_33_2409,"MTVDBC_GetXPEInfo: begin_flag=%d, end_flag=%d, fec_ind=%d, service_mode=%d, trans_id=%d, payload_len=%d, total_packets_len=%d")
TRACE_MSG(MTV_DBC_STORAGE_169_112_2_17_23_11_33_2410,"MTVDBC_GetXPEFECInfo: paramer error")
TRACE_MSG(MTV_DBC_STORAGE_189_112_2_17_23_11_33_2411,"MTVDBC_GetXPEFECInfo: not xpe-fec")
TRACE_MSG(MTV_DBC_STORAGE_220_112_2_17_23_11_33_2412,"MTVDBC_GetXPEFECInfo: temp_sum(%d) != check_sum(%d)")
TRACE_MSG(MTV_DBC_STORAGE_241_112_2_17_23_11_33_2413,"MTVDBC_GetXPEFECInfo: begin_flag=%d, end_flag=%d, trans_id=%d, payload_len=%d, total_packets_len=%d")
TRACE_MSG(MTV_DEMUX_519_112_2_17_23_11_34_2414,"DemuxSlotSendData data_to_send 0x%x")
TRACE_MSG(MTV_DEMUX_535_112_2_17_23_11_35_2415,"DemuxSlotSendData data_to_send 0x%x,0x%x,0x%x,0x%x")
TRACE_MSG(MTV_DEMUX_556_112_2_17_23_11_35_2416,"LocalDemuxSlotSendData data len 0x%x data_to_send 0x%x,is_fr  0x%x")
TRACE_MSG(MTV_DEMUX_575_112_2_17_23_11_35_2417,"LocalDemuxSlotSendData data_to_send 0x%x,0x%x")
TRACE_MSG(MTV_DEMUX_597_112_2_17_23_11_35_2418,"DemuxSlotSendData data_len 0x%x,0x%x")
TRACE_MSG(MTV_DEMUX_600_112_2_17_23_11_35_2419,"ERR data_len != sizeof(NORMAL_SLOT_INFO_T)")
TRACE_MSG(MTV_DEMUX_606_112_2_17_23_11_35_2420,"DemuxSlotGetData start 0x%x,0x%x,0x%x,0x%x")
TRACE_MSG(MTV_DEMUX_623_112_2_17_23_11_35_2421,"DemuxSlotGetData finish 0x%x,0x%x,0x%x,0x%x")
TRACE_MSG(MTV_DEMUX_1243_112_2_17_23_11_36_2422,"KDA mtvdemux Mutex 3\n")
TRACE_MSG(MTV_DEMUX_1267_112_2_17_23_11_36_2423,"KDA mtvdemux Mutex 4\n")
TRACE_MSG(MTV_DEMUX_1377_112_2_17_23_11_36_2424,"MTVDEMUX AllocBufferForDemux FreeBuffer: tr %d %d tw %d %d")
TRACE_MSG(MTV_DEMUX_1778_112_2_17_23_11_37_2425,"FreeBuffer: tr %d %d tw %d %d")
TRACE_MSG(MTV_DEMUX_2056_112_2_17_23_11_38_2426,"KDA mtvdemux Mutex 8\n")
TRACE_MSG(MTV_DEMUX_3523_112_2_17_23_11_41_2427,"KDA mtvdemux Mutex 11\n")
TRACE_MSG(MTV_OPERATION_226_112_2_17_23_11_42_2428,"MTV_SetChannel: unknown channel param %d")
TRACE_MSG(MTV_OPERATION_250_112_2_17_23_11_42_2429,"Record: enter record thread")
TRACE_MSG(MTV_OPERATION_253_112_2_17_23_11_42_2430,"Record: not in recording state %d")
TRACE_MSG(MTV_OPERATION_262_112_2_17_23_11_42_2431,"Record: GetSignal %d %d")
TRACE_MSG(MTV_OPERATION_342_112_2_17_23_11_43_2432,"error delete semaphore!")
TRACE_MSG(MTV_OPERATION_372_112_2_17_23_11_43_2433,"wait record stop bit %d")
TRACE_MSG(MTV_OPERATION_437_112_2_17_23_11_43_2434,"Record ts: GetSignal %d %d")
TRACE_MSG(MTV_OPERATION_444_112_2_17_23_11_43_2435,"Record ts: pos %d")
TRACE_MSG(MTV_OPERATION_449_112_2_17_23_11_43_2436,"MTV_RecordWriteThread: FFS_fwrite error! %d")
TRACE_MSG(MTV_OPERATION_457_112_2_17_23_11_43_2437,"Record ts: write finish %d")
TRACE_MSG(MTV_OPERATION_460_112_2_17_23_11_43_2438,"MTV_RecordWriteThread: put semaphore error!")
TRACE_MSG(MTV_OPERATION_492_112_2_17_23_11_43_2439,"Record desp: GetSignal %d %d")
TRACE_MSG(MTV_OPERATION_498_112_2_17_23_11_43_2440,"Record desp: pos %d")
TRACE_MSG(MTV_OPERATION_502_112_2_17_23_11_43_2441,"Record desp: d_offset %d file_offset %d")
TRACE_MSG(MTV_OPERATION_509_112_2_17_23_11_43_2442,"MTV_RecordWriteThread: FFS_fwrite error! %d")
TRACE_MSG(MTV_OPERATION_517_112_2_17_23_11_43_2443,"Record desp: write finish %d")
TRACE_MSG(MTV_OPERATION_520_112_2_17_23_11_43_2444,"MTV_RecordWriteThread: put semaphore error!")
TRACE_MSG(MTV_OPERATION_548_112_2_17_23_11_43_2445,"Record write: GetSignal %d %d")
TRACE_MSG(MTV_OPERATION_566_112_2_17_23_11_43_2446,"Record desp: d_offset %d file_offset %d")
TRACE_MSG(MTV_OPERATION_575_112_2_17_23_11_43_2447,"MTV_RecordWriteThread: FFS_fwrite error! %d")
TRACE_MSG(MTV_OPERATION_585_112_2_17_23_11_43_2448,"Record write desp: finish %d")
TRACE_MSG(MTV_OPERATION_594_112_2_17_23_11_43_2449,"MTV_RecordWriteThread: FFS_fwrite error! %d")
TRACE_MSG(MTV_OPERATION_613_112_2_17_23_11_43_2450,"MTV_RecordWriteThread: FFS_fwrite error! %d")
TRACE_MSG(MTV_OPERATION_627_112_2_17_23_11_43_2451,"Record write ts: finish %d")
TRACE_MSG(MTV_OPERATION_633_112_2_17_23_11_43_2452,"MTV_RecordWriteThread: put semaphore error!")
TRACE_MSG(MTV_OPERATION_639_112_2_17_23_11_43_2453,"Record write: Saved ts %d desp %d")
TRACE_MSG(MTV_OPERATION_685_112_2_17_23_11_44_2454,"Record write2: GetSignal %d %d")
TRACE_MSG(MTV_OPERATION_703_112_2_17_23_11_44_2455,"Record ts: mfs_len %d ts_len %d file_offset %d")
TRACE_MSG(MTV_OPERATION_722_112_2_17_23_11_44_2456,"MTV_RecordWriteThread: FFS_fwrite error! %d")
TRACE_MSG(MTV_OPERATION_741_112_2_17_23_11_44_2457,"MTV_RecordWriteThread: FFS_fwrite error! %d")
TRACE_MSG(MTV_OPERATION_756_112_2_17_23_11_44_2458,"Record write2 ts: finish %d")
TRACE_MSG(MTV_OPERATION_762_112_2_17_23_11_44_2459,"MTV_RecordWriteThread: put semaphore error!")
TRACE_MSG(MTV_OPERATION_768_112_2_17_23_11_44_2460,"Record write2: Saved ts %d")
TRACE_MSG(MTV_OPERATION_800_112_2_17_23_11_44_2461,"Record: enter record thread")
TRACE_MSG(MTV_OPERATION_812_112_2_17_23_11_44_2462,"Record: GetSignal %d %d")
TRACE_MSG(MTV_OPERATION_851_112_2_17_23_11_44_2463,"error delete semaphore!")
TRACE_MSG(MTV_OPERATION_853_112_2_17_23_11_44_2464,"Record: buf is written")
TRACE_MSG(MTV_OPERATION_916_112_2_17_23_11_44_2465,"Record: mfs_len %d")
TRACE_MSG(MTV_OPERATION_972_112_2_17_23_11_44_2466,"Record: mfs_len %d exceeds maxmsfsize %d")
TRACE_MSG(MTV_OPERATION_983_112_2_17_23_11_44_2467,"error delete semaphore!")
TRACE_MSG(MTV_OPERATION_985_112_2_17_23_11_44_2468,"Record: all transfered")
TRACE_MSG(MTV_OPERATION_998_112_2_17_23_11_44_2469,"error delete semaphore!")
TRACE_MSG(MTV_OPERATION_1000_112_2_17_23_11_44_2470,"Record: buf is written")
TRACE_MSG(MTV_OPERATION_1024_112_2_17_23_11_44_2471,"wait record stop bit %d")
TRACE_MSG(MTV_OPERATION_1068_112_2_17_23_11_44_2472,"MTV_RecordTSCallback: wrong state %d!!!")
TRACE_MSG(MTV_OPERATION_1073_112_2_17_23_11_44_2473,"MTV_RecordTSCallback: wrong type %d!!!")
TRACE_MSG(MTV_OPERATION_1078_112_2_17_23_11_44_2474,"MTV_RecordTSCallback: ts_record_file = NULL!!!")
TRACE_MSG(MTV_OPERATION_1090_112_2_17_23_11_44_2475,"MTV_RecordTSCallback: SPITV_Dma_Read error!")
TRACE_MSG(MTV_OPERATION_1100_112_2_17_23_11_44_2476,"MTV_RecordTSCallback: FFS_fwrite error!")
TRACE_MSG(MTV_OPERATION_1111_112_2_17_23_11_44_2477,"MTV_RecordTSCallback: SPITV_Dma_Read error!")
TRACE_MSG(MTV_OPERATION_1120_112_2_17_23_11_44_2478,"MTV_RecordTSCallback: FFS_fwrite error!")
TRACE_MSG(MTV_OPERATION_1144_112_2_17_23_11_44_2479,"MTV_RecordDescriptorCallback: wrong state %d!!!")
TRACE_MSG(MTV_OPERATION_1149_112_2_17_23_11_45_2480,"MTV_RecordDescriptorCallback: wrong type %d!!!")
TRACE_MSG(MTV_OPERATION_1154_112_2_17_23_11_45_2481,"MTV_RecordDescriptorCallback: desp_record_file = NULL!!!")
TRACE_MSG(MTV_OPERATION_1165_112_2_17_23_11_45_2482,"MTV_RecordTSCallback: SPITV_Dma_Read error!")
TRACE_MSG(MTV_OPERATION_1175_112_2_17_23_11_45_2483,"MTV_RecordTSCallback: FFS_fwrite error!")
TRACE_MSG(MTV_OPERATION_1187_112_2_17_23_11_45_2484,"MTV_RecordTSCallback: SPITV_Dma_Read error!")
TRACE_MSG(MTV_OPERATION_1196_112_2_17_23_11_45_2485,"MTV_RecordTSCallback: FFS_fwrite error!")
TRACE_MSG(MTV_OPERATION_1238_112_2_17_23_11_45_2486,"MTV_PlaybackTSCallback: DTL_PostData error!")
TRACE_MSG(MTV_OPERATION_1245_112_2_17_23_11_45_2487,"MTV_PlaybackTSCallback: DTL_PostData error!")
TRACE_MSG(MTV_OPERATION_1290_112_2_17_23_11_45_2488,"Playback read: GetSignal %d %d")
TRACE_MSG(MTV_OPERATION_1307_112_2_17_23_11_45_2489,"Playback read: %d")
TRACE_MSG(MTV_OPERATION_1313_112_2_17_23_11_45_2490,"Playback read: %d finish %d")
TRACE_MSG(MTV_OPERATION_1316_112_2_17_23_11_45_2491,"MTV_PlaybackReadThread: put semaphore error!")
TRACE_MSG(MTV_OPERATION_1356_112_2_17_23_11_45_2492,"Playback: GetSignal %d %d")
TRACE_MSG(MTV_OPERATION_1363_112_2_17_23_11_45_2493,"Playback: len[0]=%d len[1]=%d")
TRACE_MSG(MTV_OPERATION_1451_112_2_17_23_11_45_2494,"error delete semaphore!")
TRACE_MSG(MTV_OPERATION_1453_112_2_17_23_11_45_2495,"playback: all transfered")
TRACE_MSG(MTV_OPERATION_1462_112_2_17_23_11_45_2496,"error delete semaphore!")
TRACE_MSG(MTV_OPERATION_1464_112_2_17_23_11_45_2497,"playback: buf is read")
TRACE_MSG(MTV_OPERATION_1492_112_2_17_23_11_45_2498,"wait playback stop bit %d")
TRACE_MSG(MTV_OPERATION_1504_112_2_17_23_11_45_2499,"STOP: playback thread exit")
TRACE_MSG(MTV_OPERATION_1548_112_2_17_23_11_45_2500,"MTV_PlaybackTSCallback: wrong state %d!!!")
TRACE_MSG(MTV_OPERATION_1553_112_2_17_23_11_45_2501,"MTV_PlaybackTSCallback: ts_play_file = NULL!!!")
TRACE_MSG(MTV_OPERATION_1571_112_2_17_23_11_45_2502,"MTV_PlaybackTSCallback: error resp_id %d")
TRACE_MSG(MTV_OPERATION_1633_112_2_17_23_11_45_2503,"MTV_PlaybackTSCallback: FFS_fread error!")
TRACE_MSG(MTV_OPERATION_1641_112_2_17_23_11_46_2504,"MTV_PlaybackTSCallback: DTL_PostData error!")
TRACE_MSG(MTV_OPERATION_1655_112_2_17_23_11_46_2505,"MTV_PlaybackTSCallback: FFS_fread error!")
TRACE_MSG(MTV_OPERATION_1663_112_2_17_23_11_46_2506,"MTV_PlaybackTSCallback: DTL_PostData error!")
TRACE_MSG(MTV_OPERATION_1692_112_2_17_23_11_46_2507,"DisableRecordCallback wrong msg %d")
TRACE_MSG(MTV_OPERATION_1698_112_2_17_23_11_46_2508,"DisableRecordCallback: record state error!!!")
TRACE_MSG(MTV_OPERATION_1776_112_2_17_23_11_46_2509,"Enter RecordSaveEntry()")
TRACE_MSG(MTV_OPERATION_1780_112_2_17_23_11_46_2510,"RecordSaveEntry: error get ts file size!!!")
TRACE_MSG(MTV_OPERATION_1785_112_2_17_23_11_46_2511,"RecordSaveEntry: error get desp file size!!!")
TRACE_MSG(MTV_OPERATION_1792_112_2_17_23_11_46_2512,"RecordSaveEntry: Set to file begin failed!!!")
TRACE_MSG(MTV_OPERATION_1798_112_2_17_23_11_46_2513,"RecordSaveEntry: Read file header failed!!!")
TRACE_MSG(MTV_OPERATION_1805_112_2_17_23_11_46_2514,"RecordSaveEntry: msfh len %d")
TRACE_MSG(MTV_OPERATION_1809_112_2_17_23_11_46_2515,"RecordSaveEntry: len %d %d")
TRACE_MSG(MTV_OPERATION_1814_112_2_17_23_11_46_2516,"RecordSaveEntry: Set to file begin failed!!!")
TRACE_MSG(MTV_OPERATION_1820_112_2_17_23_11_46_2517,"RecordSaveEntry: Write file header failed!!!")
TRACE_MSG(MTV_OPERATION_1853_112_2_17_23_11_46_2518,"MTV_RecordTSCallback: FFS_fwrite error!")
TRACE_MSG(MTV_OPERATION_1879_112_2_17_23_11_46_2519,"Leave RecordSaveEntry()")
TRACE_MSG(MTV_OPERATION_1896_112_2_17_23_11_46_2520,"Enter RecordSaveEntry()")
TRACE_MSG(MTV_OPERATION_1917_112_2_17_23_11_46_2521,"Leave RecordSaveEntry()")
TRACE_MSG(MTV_OPERATION_1942_112_2_17_23_11_46_2522,"Data: error occured 0x%X!=0x%X @%d")
TRACE_MSG(MTV_OPERATION_1981_112_2_17_23_11_46_2523,"Data: error occured 0x%X!=0x%X @%d/%d")
TRACE_MSG(MTV_OPERATION_1995_112_2_17_23_11_46_2524,"Record: TS %d 0x%X")
TRACE_MSG(MTV_OPERATION_2016_112_2_17_23_11_46_2525,"MTV_RecordThread: put semaphore error!")
TRACE_MSG(MTV_OPERATION_2025_112_2_17_23_11_46_2526,"MTV_PlaybackThread: put semaphore error!")
TRACE_MSG(MTV_OPERATIONMAN_157_112_2_17_23_11_47_2527,"MTV_Init: vol=%d")
TRACE_MSG(MTV_OPERATIONMAN_172_112_2_17_23_11_47_2528,"MTV_Init: arm clock 5")
TRACE_MSG(MTV_OPERATIONMAN_176_112_2_17_23_11_47_2529,"MTV_Init: cannot getReqhandler!!!")
TRACE_MSG(MTV_OPERATIONMAN_201_112_2_17_23_11_47_2530,"INIT: cost %dms")
TRACE_MSG(MTV_OPERATIONMAN_215_112_2_17_23_11_47_2531,"Cleanup: cost %dms")
TRACE_MSG(MTV_OPERATIONMAN_227_112_2_17_23_11_47_2532,"MTV_Cleanup: restore arm clock")
TRACE_MSG(MTV_OPERATIONMAN_284_112_2_17_23_11_47_2533,"Cleanup: cost %dms")
TRACE_MSG(MTV_OPERATIONMAN_296_112_2_17_23_11_47_2534,"MTV_Cleanup: restore arm clock")
TRACE_MSG(MTV_OPERATIONMAN_344_112_2_17_23_11_47_2535,"INIT: receive init finish msg")
TRACE_MSG(MTV_OPERATIONMAN_347_112_2_17_23_11_47_2536,"MTV_Init: InitCallback state error!!!")
TRACE_MSG(MTV_OPERATIONMAN_363_112_2_17_23_11_47_2537,"Cleanup: receive cleanup finish msg")
TRACE_MSG(MTV_OPERATIONMAN_366_112_2_17_23_11_47_2538,"MTV_Cleanup: CleanupCallback state error!!!")
TRACE_MSG(MTV_STIMIESG_PARSE_693_112_2_17_23_12_14_2539,"ESG:MTVSTIMI_ESGInit. dir  = %s , %x")
TRACE_MSG(MTV_STIMIESG_PARSE_719_112_2_17_23_12_14_2540,"ESG:MTVSTIMI_ESGSearchInit: configure buffer.")
TRACE_MSG(MTV_STIMIESG_PARSE_875_112_2_17_23_12_14_2541,"ESG:MTVSTIMI_ESGS_TEST")
TRACE_MSG(MTV_STIMIESG_PARSE_888_112_2_17_23_12_14_2542,"ESG:MTVSTIMI_ESGS_TEST, updating control table, stop!")
TRACE_MSG(MTV_STIMIESG_PARSE_924_112_2_17_23_12_14_2543,"ESG:IsFolderExist: find first sfs_handle = %d, attr = 0x%x")
TRACE_MSG(MTV_STIMIESG_PARSE_970_112_2_17_23_12_14_2544,"ESG: MTVSTIMI_SearchESGData, net id  =%d, s_dir_name = %s!")
TRACE_MSG(MTV_STIMIESG_PARSE_987_112_2_17_23_12_14_2545,"ESG: MTVSTIMI_SearchESGData, nit (%d)data is empty!")
TRACE_MSG(MTV_STIMIESG_PARSE_1001_112_2_17_23_12_14_2546,"ESG: MTVSTIMI_SearchESGData, No Control Table!")
TRACE_MSG(MTV_STIMIESG_PARSE_1011_112_2_17_23_12_14_2547,"ESG: MTVSTIMI_SearchESGData, sfs_error = %d")
TRACE_MSG(MTV_STIMIESG_PARSE_1018_112_2_17_23_12_14_2548,"ESG: MTVSTIMI_SearchESGData, high_word = %d,  low_word= %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_1045_112_2_17_23_12_14_2549,"ESG:MTVSTIMI_SearchESGData, searching!")
TRACE_MSG(MTV_STIMIESG_PARSE_1067_112_2_17_23_12_14_2550,"ESG:ESG_SearchESGData, No Control Table!")
TRACE_MSG(MTV_STIMIESG_PARSE_1089_112_2_17_23_12_14_2551,"ESG:ESG_SearchESGData, searching!")
TRACE_MSG(MTV_STIMIESG_PARSE_1115_112_2_17_23_12_14_2552,"ESG:MTVSTIMI_SearchESGData, no searching!")
TRACE_MSG(MTV_STIMIESG_PARSE_1132_112_2_17_23_12_14_2553,"ESG:FreeServiceInfoData!")
TRACE_MSG(MTV_STIMIESG_PARSE_1185_112_2_17_23_12_15_2554,"ESG:FreeServiceUnderFrq, service_count =%d!")
TRACE_MSG(MTV_STIMIESG_PARSE_1238_112_2_17_23_12_15_2555,"ESG:EsgTimerCallback param = %d\n ")
TRACE_MSG(MTV_STIMIESG_PARSE_1273_112_2_17_23_12_15_2556,"ESG:EsgDataTimerCallback param = %d\n ")
TRACE_MSG(MTV_STIMIESG_PARSE_1307_112_2_17_23_12_15_2557,"ESG:ReadyForRcvESGData, net_id = %d, state = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_1314_112_2_17_23_12_15_2558,"ESG:ReadyForRcvESGData, ESG_STATE_WAIT_CTRL_TAB!")
TRACE_MSG(MTV_STIMIESG_PARSE_1330_112_2_17_23_12_15_2559,"ESG:ReadyForRcvESGData, ESG_STATE_WAIT_SCT!")
TRACE_MSG(MTV_STIMIESG_PARSE_1344_112_2_17_23_12_15_2560,"ESG:ReadyForRcvESGData, ESG_STATE_WAIT_NIT!")
TRACE_MSG(MTV_STIMIESG_PARSE_1373_112_2_17_23_12_15_2561,"ESG:ReadyForRcvESGData, ESG_STATE_LOOKUP_SRV_FRQ!")
TRACE_MSG(MTV_STIMIESG_PARSE_1391_112_2_17_23_12_15_2562,"ESG:ReadyForRcvESGData, ESG_STATE_WAIT_MCT!")
TRACE_MSG(MTV_STIMIESG_PARSE_1406_112_2_17_23_12_15_2563,"ESG:ReadyForRcvESGData, ESG_STATE_CONFIG_SERVICE!")
TRACE_MSG(MTV_STIMIESG_PARSE_1415_112_2_17_23_12_15_2564,"ESG:ReadyForRcvESGData, ESG_STATE_CONFIG_FILTER!")
TRACE_MSG(MTV_STIMIESG_PARSE_1437_112_2_17_23_12_15_2565,"ESG:ReadyForRcvESGData, default state %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_1458_112_2_17_23_12_15_2566,"ESG:DisposeESGResult, result = %d")
TRACE_MSG(MTV_STIMIESG_PARSE_1651_112_2_17_23_12_15_2567,"ESG:DisposeESGResult, unknown result = %d")
TRACE_MSG(MTV_STIMIESG_PARSE_1688_112_2_17_23_12_16_2568,"ESG: IsEnbleOpenESGD, rcv_segment_count %d, segment_count %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_1698_112_2_17_23_12_16_2569,"ESG: IsEnbleOpenESGD, mem (%d)  is empty!")
TRACE_MSG(MTV_STIMIESG_PARSE_1729_112_2_17_23_12_16_2570,"ESG: IsEnbleOpenCSCT, rcv_segment_count %d, segment_count %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_1739_112_2_17_23_12_16_2571,"ESG: IsEnbleOpenCSCT, mem (%d)  is empty!")
TRACE_MSG(MTV_STIMIESG_PARSE_1768_112_2_17_23_12_16_2572,"ESG: IsEnbleOpenNIT, nit (%d)data is empty!")
TRACE_MSG(MTV_STIMIESG_PARSE_1778_112_2_17_23_12_16_2573,"ESG: IsEnbleOpenNIT, mem (%d)  is empty!")
TRACE_MSG(MTV_STIMIESG_PARSE_1860_112_2_17_23_12_16_2574,"ESG: SetupDataBlock, s_esg_service_net_info.data_type_ptr, not enough memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_1901_112_2_17_23_12_16_2575,"ESG: SetupDataBlock, s_esg_service_net_info.data_type_num = %d !")
TRACE_MSG(MTV_STIMIESG_PARSE_1908_112_2_17_23_12_16_2576,"ESG: SetupDataBlock, GetEsgTypeBlockNum is false !")
TRACE_MSG(MTV_STIMIESG_PARSE_1918_112_2_17_23_12_16_2577,"ESG: SetupDataBlock, s_esg_service_net_info.data_block_data_ready_ptr, not enough momery %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_1929_112_2_17_23_12_16_2578,"ESG: SetupDataBlock, s_data_ready_block_ptr[%d], not enough space %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2024_112_2_17_23_12_16_2579,"ESG: LookupSCT!")
TRACE_MSG(MTV_STIMIESG_PARSE_2079_112_2_17_23_12_16_2580,"ESG: LookupNIT!")
TRACE_MSG(MTV_STIMIESG_PARSE_2119_112_2_17_23_12_16_2581,"ESG: LookupNIT, unknown result = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2146_112_2_17_23_12_16_2582,"ESG: LookupSrvFrq, s_frq_init_pos  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2153_112_2_17_23_12_16_2583,"ESG: LookupSrvFrq, frq_num  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2193_112_2_17_23_12_17_2584,"ESG: LookupSrvFrq, center_frequency = %d, bandwidth = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2214_112_2_17_23_12_17_2585,"ESG: SetupSrvUnderFrq!")
TRACE_MSG(MTV_STIMIESG_PARSE_2224_112_2_17_23_12_17_2586,"ESG: SetupSrvUnderFrq, net id =0x%x, frq_num = 0x%x!")
TRACE_MSG(MTV_STIMIESG_PARSE_2230_112_2_17_23_12_17_2587,"ESG: SetupSrvUnderFrq, s_esg_service_info_under_frq.service_count = 0x%x!")
TRACE_MSG(MTV_STIMIESG_PARSE_2290_112_2_17_23_12_17_2588,"ESG: LookupMCT, service count %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2318_112_2_17_23_12_17_2589,"ESG: LookupMCT, result = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2357_112_2_17_23_12_17_2590,"ESG: ConfigRcvDataService, current_service_idx %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2383_112_2_17_23_12_17_2591,"ESG: ConfigRcvDataService, Center_frequency = %d, Bandwidth_mode =  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2390_112_2_17_23_12_17_2592,"ESG: ConfigRcvDataService, Byte_interleave_mode = %d, RS_mode =  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2391_112_2_17_23_12_17_2593,"ESG: ConfigRcvDataService, LDPC_mode = %d, Mapping_mode =  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2392_112_2_17_23_12_17_2594,"ESG: ConfigRcvDataService, Mux_frame_ID = %d, Scramble_mode =  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2395_112_2_17_23_12_17_2595,"ESG: ConfigRcvDataService, TS_number = %d, TS_start_index =  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2404_112_2_17_23_12_17_2596,"ESG: ConfigRcvDataService, param.mux_subframe_id = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2410_112_2_17_23_12_17_2597,"ESG: ConfigRcvDataService, param.mux_subframe_id = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2420_112_2_17_23_12_17_2598,"ESG: ConfigRcvDataService, current_service_id %d, chan_pa = %x,demod_par=0x%x!")
TRACE_MSG(MTV_STIMIESG_PARSE_2431_112_2_17_23_12_17_2599,"ESG: MTVSTIMI_EnableESG, current_service_id %d, enable = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2455_112_2_17_23_12_17_2600,"ESG: SaveESGDataInfo, segment_no %d, data_type %d, data_block %d, service_index %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2510_112_2_17_23_12_17_2601,"ESG: SaveESGDataInfo, cur_block_id_list_ptr->next_ptr = PNULL!!!")
TRACE_MSG(MTV_STIMIESG_PARSE_2519_112_2_17_23_12_17_2602,"ESG: SaveESGDataInfo, current_block_num = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2522_112_2_17_23_12_17_2603,"ESG: SaveESGDataInfo, block id = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2538_112_2_17_23_12_17_2604,"ESG: SaveESGDataInfo 2, cur_block_id_list_ptr->next_ptr = PNULL!!!")
TRACE_MSG(MTV_STIMIESG_PARSE_2547_112_2_17_23_12_17_2605,"ESG: SaveESGDataInfo 2, current_block_num = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2550_112_2_17_23_12_17_2606,"ESG: SaveESGDataInfo 2, block id = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2594_112_2_17_23_12_17_2607,"ESG:SaveESGServiceInfo: net id = %d.")
TRACE_MSG(MTV_STIMIESG_PARSE_2620_112_2_17_23_12_17_2608,"ESG: SaveESGServiceInfo(), open ESGD file failed!")
TRACE_MSG(MTV_STIMIESG_PARSE_2633_112_2_17_23_12_17_2609,"ESG: SaveESGServiceInfo(), esg_segment = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2656_112_2_17_23_12_17_2610,"ESG: SaveESGServiceInfo(), esg_service_number = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2663_112_2_17_23_12_18_2611,"ESG: SaveESGServiceInfo(), serive i = %d, Not Enough Memory!")
TRACE_MSG(MTV_STIMIESG_PARSE_2674_112_2_17_23_12_18_2612,"ESG: SaveESGServiceInfo(), service_id = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2690_112_2_17_23_12_18_2613,"ESG: SaveESGServiceInfo(), data_type_number = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2699_112_2_17_23_12_18_2614,"ESG: SaveESGServiceInfo(), segment i = %d, data_type, %d, Not Enough Space!")
TRACE_MSG(MTV_STIMIESG_PARSE_2719_112_2_17_23_12_18_2615,"ESG: SaveESGServiceInfo(), data type number j = %d, data block not enough memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2736_112_2_17_23_12_18_2616,"ESG: SaveESGServiceInfo(), data_block_id = %d, esg_server_index = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2802_112_2_17_23_12_18_2617,"ESG: SaveESGServiceData,  net id  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2831_112_2_17_23_12_18_2618,"ESG: SaveESGServiceData,  service_count = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2838_112_2_17_23_12_18_2619,"ESG: SaveESGServiceData,  not enough memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2875_112_2_17_23_12_18_2620,"ESG: SaveESGServiceFrq(),  net id  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2915_112_2_17_23_12_18_2621,"ESG: SaveESGServiceFrq(),  not enough memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_2972_112_2_17_23_12_18_2622,"ESG: GetEsgTypeBlockList type = %d, block count = %d")
TRACE_MSG(MTV_STIMIESG_PARSE_2976_112_2_17_23_12_18_2623,"ESG: GetEsgTypeBlockList s_service_segment_count= %d")
TRACE_MSG(MTV_STIMIESG_PARSE_2980_112_2_17_23_12_18_2624,"ESG: GetEsgTypeBlockList  service_count = %d")
TRACE_MSG(MTV_STIMIESG_PARSE_2983_112_2_17_23_12_18_2625,"ESG: GetEsgTypeBlockList  data_type_num = %d")
TRACE_MSG(MTV_STIMIESG_PARSE_2989_112_2_17_23_12_18_2626,"ESG: GetEsgTypeBlockList  count = %d, list_ptr->block_count = %d")
TRACE_MSG(MTV_STIMIESG_PARSE_3001_112_2_17_23_12_18_2627,"ESG: GetEsgTypeBlockList  total = %d")
TRACE_MSG(MTV_STIMIESG_PARSE_3007_112_2_17_23_12_18_2628,"ESG: GetEsgTypeBlockList  list_ptr->block_id_arr[%d]= %d")
TRACE_MSG(MTV_STIMIESG_PARSE_3010_112_2_17_23_12_18_2629,"ESG: GetEsgTypeBlockList  left = %d")
TRACE_MSG(MTV_STIMIESG_PARSE_3016_112_2_17_23_12_18_2630,"ESG: GetEsgTypeBlockList block_id_list_ptr->next_ptr = PNULL")
TRACE_MSG(MTV_STIMIESG_PARSE_3052_112_2_17_23_12_18_2631,"ESG: TotalESGDataBlock!")
TRACE_MSG(MTV_STIMIESG_PARSE_3088_112_2_17_23_12_18_2632,"ESG: TotalESGDataBlock, invalid type %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_3165_112_2_17_23_12_19_2633,"ESG: GetESGServiceInfoUnderFrq!")
TRACE_MSG(MTV_STIMIESG_PARSE_3182_112_2_17_23_12_19_2634,"ESG: GetESGServiceInfoUnderFrq, service_count = 0!")
TRACE_MSG(MTV_STIMIESG_PARSE_3192_112_2_17_23_12_19_2635,"ESG: GetESGServiceInfoUnderFrq, esg_service_info_ptr->esg_service_data_info_ptr , not enough memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_3221_112_2_17_23_12_19_2636,"ESG: ConfigESGDataTypeFilter s_esg_data_filter_num %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_3308_112_2_17_23_12_19_2637,"ESG: EnableESGDataTypeFilter!")
TRACE_MSG(MTV_STIMIESG_PARSE_3341_112_2_17_23_12_19_2638,"ESG: DisableAllESGFilter!")
TRACE_MSG(MTV_STIMIESG_PARSE_3377_112_2_17_23_12_19_2639,"ESG: EnableAllESGFilter!")
TRACE_MSG(MTV_STIMIESG_PARSE_3415_112_2_17_23_12_19_2640,"ESG: ConfigESGDataBlockFilter , type %d, block %d, %segment %d, is segment %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_3460_112_2_17_23_12_19_2641,"ESG: EnableESGDataBlockFilter .")
TRACE_MSG(MTV_STIMIESG_PARSE_3492_112_2_17_23_12_19_2642,"ESG: DeleteESGDataBlockFilter.")
TRACE_MSG(MTV_STIMIESG_PARSE_3538_112_2_17_23_12_19_2643,"ESG: AddConfigbuff , index = %d, descriptor_count = %d, s_esg_buffer_used_num = %d.")
TRACE_MSG(MTV_STIMIESG_PARSE_3560_112_2_17_23_12_19_2644,"ESG: DelConfigbuff, index = %d, s_esg_buffer_used_num = %d.")
TRACE_MSG(MTV_STIMIESG_PARSE_3604_112_2_17_23_12_19_2645,"ESG: IsDataBlockReady, type %d, block %d.")
TRACE_MSG(MTV_STIMIESG_PARSE_3666_112_2_17_23_12_20_2646,"ESG: IsDataBlockReady(), file head, reading file head failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_3714_112_2_17_23_12_20_2647,"ESG: IsBlockLinkNodeExisting, type = %d, block = %d, segment = %d.")
TRACE_MSG(MTV_STIMIESG_PARSE_3719_112_2_17_23_12_20_2648,"ESG: IsBlockLinkNodeExisting, discarded! ")
TRACE_MSG(MTV_STIMIESG_PARSE_3728_112_2_17_23_12_20_2649,"ESG: IsBlockLinkNodeExisting, no node!")
TRACE_MSG(MTV_STIMIESG_PARSE_3741_112_2_17_23_12_20_2650,"ESG: IsBlockLinkNodeExisting, in file!")
TRACE_MSG(MTV_STIMIESG_PARSE_3752_112_2_17_23_12_20_2651,"ESG: IsBlockLinkNodeExisting, in buffer!")
TRACE_MSG(MTV_STIMIESG_PARSE_3786_112_2_17_23_12_20_2652,"ESG: IsBlockLinkExisting, type = %d, block = %d.")
TRACE_MSG(MTV_STIMIESG_PARSE_3794_112_2_17_23_12_20_2653,"ESG: IsBlockLinkExisting, no node.")
TRACE_MSG(MTV_STIMIESG_PARSE_3867_112_2_17_23_12_20_2654,"ESG: ParseESGData, buff id = %d, descriptor_count = %d.")
TRACE_MSG(MTV_STIMIESG_PARSE_3883_112_2_17_23_12_20_2655,"ESG: ParseESGData, updating control table, stop receiving!")
TRACE_MSG(MTV_STIMIESG_PARSE_3911_112_2_17_23_12_20_2656,"ESG: ParseESGData, no buffer, disable all filter!")
TRACE_MSG(MTV_STIMIESG_PARSE_3916_112_2_17_23_12_20_2657,"ESG: ParseESGData, buff data addr = 0x%x, decr addr = 0x%x!")
TRACE_MSG(MTV_STIMIESG_PARSE_3920_112_2_17_23_12_20_2658,"ESG: ParseESGData, no empty buffer, s_esg_buffer_used_num= %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_3933_112_2_17_23_12_20_2659,"ESG: ParseESGData, no memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_3997_112_2_17_23_12_20_2660,"ESG: ParseESGData, addr = 0x%x, len= %x,block id= %d, d type = %d, seg no = %d, total =%d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4002_112_2_17_23_12_20_2661,"ESG: ParseESGData,  existing!")
TRACE_MSG(MTV_STIMIESG_PARSE_4012_112_2_17_23_12_20_2662,"ESG: ParseESGData, added!")
TRACE_MSG(MTV_STIMIESG_PARSE_4033_112_2_17_23_12_20_2663,"ESG: ParseESGData, valid  descp_count = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4080_112_2_17_23_12_20_2664,"ESG: ParseESGData,  unknown result = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4092_112_2_17_23_12_20_2665,"ESG: ParseESGData, left  descp_count = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4132_112_2_17_23_12_20_2666,"ESG: ParseESGData, no valid data!")
TRACE_MSG(MTV_STIMIESG_PARSE_4196_112_2_17_23_12_21_2667,"ESG: ESG_ProcessDemuxData, buff id = %d, descriptor_len = %d.")
TRACE_MSG(MTV_STIMIESG_PARSE_4199_112_2_17_23_12_21_2668,"ESG: ESG_ProcessDemuxData, searching is end")
TRACE_MSG(MTV_STIMIESG_PARSE_4220_112_2_17_23_12_21_2669,"ESG: ESG_ProcessDemuxData, updating control table, stop receiving!")
TRACE_MSG(MTV_STIMIESG_PARSE_4259_112_2_17_23_12_21_2670,"ESG: ESG_ProcessDemuxData, addr = 0x%x, len= %x,block id= %d, d type = %d, seg no = %d, total =%d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4264_112_2_17_23_12_21_2671,"ESG: ESG_ProcessDemuxData,  existing!")
TRACE_MSG(MTV_STIMIESG_PARSE_4273_112_2_17_23_12_21_2672,"ESG: ESG_ProcessDemuxData, added!")
TRACE_MSG(MTV_STIMIESG_PARSE_4309_112_2_17_23_12_21_2673,"ESG: ESG_ProcessDemuxData, error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4335_112_2_17_23_12_21_2674,"ESG: ESG_ProcessDemuxData, unknown error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4398_112_2_17_23_12_21_2675,"ESG: GetSearchESGServiceInfo.")
TRACE_MSG(MTV_STIMIESG_PARSE_4404_112_2_17_23_12_21_2676,"ESG: GetSearchESGServiceInfo. block_total_count = %d, rcv_block_count = %d")
TRACE_MSG(MTV_STIMIESG_PARSE_4419_112_2_17_23_12_21_2677,"ESG: GetSearchESGServiceInfo. complete.")
TRACE_MSG(MTV_STIMIESG_PARSE_4421_112_2_17_23_12_21_2678,"ESG: GetSearchESGServiceInfo, current_service_id %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4450_112_2_17_23_12_21_2679,"ESG: GetSearchESGServiceInfo. search next frequency.")
TRACE_MSG(MTV_STIMIESG_PARSE_4460_112_2_17_23_12_21_2680,"ESG: GetSearchESGServiceInfo. search next service.")
TRACE_MSG(MTV_STIMIESG_PARSE_4469_112_2_17_23_12_21_2681,"ESG: GetSearchESGServiceInfo. wait for data.")
TRACE_MSG(MTV_STIMIESG_PARSE_4524_112_2_17_23_12_21_2682,"ESG: SaveConfigData , open file failed!")
TRACE_MSG(MTV_STIMIESG_PARSE_4536_112_2_17_23_12_21_2683,"ESG: SaveConfigData, write config file failed %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4558_112_2_17_23_12_21_2684,"ESG: SaveConfigData, segment ,Not enough memory! size = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4566_112_2_17_23_12_21_2685,"ESG: SaveConfigData, create file failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4586_112_2_17_23_12_21_2686,"ESG: SaveConfigData, aloc sect data(%d) file failed!")
TRACE_MSG(MTV_STIMIESG_PARSE_4597_112_2_17_23_12_21_2687,"ESG: SaveConfigData, aloc seg data(%d) file failed!")
TRACE_MSG(MTV_STIMIESG_PARSE_4631_112_2_17_23_12_21_2688,"ESG: GetESGData, need_save_count %d.")
TRACE_MSG(MTV_STIMIESG_PARSE_4645_112_2_17_23_12_22_2689,"ESG: GetESGData, stop receiving esg data!")
TRACE_MSG(MTV_STIMIESG_PARSE_4669_112_2_17_23_12_22_2690,"ESG: GetESGData, valid data in buffer!")
TRACE_MSG(MTV_STIMIESG_PARSE_4672_112_2_17_23_12_22_2691,"ESG: GetESGData, desp_index_ptr->desp_list_head!")
TRACE_MSG(MTV_STIMIESG_PARSE_4682_112_2_17_23_12_22_2692,"ESG: GetESGData, desp_list_head!")
TRACE_MSG(MTV_STIMIESG_PARSE_4693_112_2_17_23_12_22_2693,"ESG: GetESGData, desp_list!")
TRACE_MSG(MTV_STIMIESG_PARSE_4715_112_2_17_23_12_22_2694,"ESG: GetESGData, save data failed!")
TRACE_MSG(MTV_STIMIESG_PARSE_4782_112_2_17_23_12_22_2695,"ESG: GetLeftSegment!")
TRACE_MSG(MTV_STIMIESG_PARSE_4849_112_2_17_23_12_22_2696,"ESG: ConfigESGDataFilter!")
TRACE_MSG(MTV_STIMIESG_PARSE_4866_112_2_17_23_12_22_2697,"ESG: ConfigESGDataFilter, data type  =%d  finshed!")
TRACE_MSG(MTV_STIMIESG_PARSE_4891_112_2_17_23_12_22_2698,"ESG: ConfigESGDataFilter filter_id %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_4908_112_2_17_23_12_22_2699,"ESG: ConfigESGDataFilter, disable_filter_info  =0x%x  !")
TRACE_MSG(MTV_STIMIESG_PARSE_5032_112_2_17_23_12_22_2700,"ESG: SaveESGData(), buff_idx = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5033_112_2_17_23_12_22_2701,"ESG: SaveESGData(), addr = 0x%x, len= %x,block id= %d, d type = %d, seg no = %d, total =%d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5048_112_2_17_23_12_22_2702,"ESG: SaveESGData(), fnot enogh memory ! size = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5083_112_2_17_23_12_22_2703,"ESG: SaveESGData(), segment_len  = %d,  desp.len = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5086_112_2_17_23_12_22_2704,"ESG: SaveESGData(), segment_len  %d,  desp.len -ESG_SEGMENT_HEAD_SIZE  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5108_112_2_17_23_12_22_2705,"ESG: SaveESGData(), high_word = %d,  low_word= %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5114_112_2_17_23_12_22_2706,"ESG: SaveESGData(), SFS_GetDeviceFreeSpace, no space  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5124_112_2_17_23_12_22_2707,"ESG: SaveESGData(), SFS_GetDeviceFreeSpace! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5135_112_2_17_23_12_22_2708,"ESG: SaveESGData(), file head, reading file head failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5151_112_2_17_23_12_23_2709,"ESG: SaveESGData(), file head, reading file head failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5161_112_2_17_23_12_23_2710,"ESG: SaveESGData(), write file failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5175_112_2_17_23_12_23_2711,"ESG: SaveESGData(), file head, reading file head failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5185_112_2_17_23_12_23_2712,"ESG: SaveESGData(), write file failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5196_112_2_17_23_12_23_2713,"ESG: SaveESGData(), segment_position, SFS_SetFilePointer! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5207_112_2_17_23_12_23_2714,"ESG: SaveESGData(), write file failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5231_112_2_17_23_12_23_2715,"ESG: SaveESGData(), segment ,Not enough memory! size = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5237_112_2_17_23_12_23_2716,"ESG: SaveESGData(), n high_word = %d,  low_word= %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5243_112_2_17_23_12_23_2717,"ESG: SaveESGData(), SFS_GetDeviceFreeSpace, no space  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5253_112_2_17_23_12_23_2718,"ESG: SaveESGData(), SFS_GetDeviceFreeSpace! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5262_112_2_17_23_12_23_2719,"ESG: SaveESGData(), create file failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5273_112_2_17_23_12_23_2720,"ESG: SaveESGData(), write file head failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5286_112_2_17_23_12_23_2721,"ESG: SaveESGData(), write head segment failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5298_112_2_17_23_12_23_2722,"ESG: SaveESGData(), write file failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5349_112_2_17_23_12_23_2723,"ESG: SaveESGDataToMem(), buff_idx = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5350_112_2_17_23_12_23_2724,"ESG: SaveESGDataToMem(), addr = 0x%x, len= %x,block id= %d, d type = %d, seg no = %d, total =%d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5364_112_2_17_23_12_23_2725,"ESG: SaveESGDataToMem(), fnot enogh memory ! size = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5385_112_2_17_23_12_23_2726,"ESG: SaveESGDataToMem(), segment_len  = %d,  desp.len = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5388_112_2_17_23_12_23_2727,"ESG: SaveESGDataToMem(), segment_len  %d,  desp.len -ESG_SEGMENT_HEAD_SIZE  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5429_112_2_17_23_12_23_2728,"ESG: SaveESGDataToMem(), aloc seg data(%d) file failed!")
TRACE_MSG(MTV_STIMIESG_PARSE_5468_112_2_17_23_12_23_2729,"ESG: SaveESGDataToMem(), segment ,Not enough memory! size = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5477_112_2_17_23_12_23_2730,"ESG: SaveESGDataToMem(), create file failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5498_112_2_17_23_12_23_2731,"ESG: SaveESGDataToMem(), aloc ect data(%d) file failed!")
TRACE_MSG(MTV_STIMIESG_PARSE_5509_112_2_17_23_12_23_2732,"ESG: SaveESGDataToMem(), aloc seg data(%d) file failed!")
TRACE_MSG(MTV_STIMIESG_PARSE_5547_112_2_17_23_12_23_2733,"ESG: DecreaseDescriptorCount, buff_idx = %d, s_esg_buffer_used_num = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5554_112_2_17_23_12_23_2734,"ESG: DecreaseDescriptorCount, buff_idx = %d, descp_count = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5591_112_2_17_23_12_23_2735,"ESG: SetFinishMark, type = %d, block = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5601_112_2_17_23_12_23_2736,"ESG: SetFinishMark, s_esg_service_net_info.data_type_num = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5612_112_2_17_23_12_23_2737,"ESG: SetFinishMark, block_id_list_ptr->block_count = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5641_112_2_17_23_12_24_2738,"ESG: SetFinishMark, type = %d, index = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5653_112_2_17_23_12_24_2739,"ESG: SetFinishMark,data_block_data_ready_ptr[%d][%d] = 0x%x!")
TRACE_MSG(MTV_STIMIESG_PARSE_5687_112_2_17_23_12_24_2740,"ESG: IsBlockValid, type = %d, block = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5696_112_2_17_23_12_24_2741,"ESG: IsBlockValid, s_esg_service_net_info.data_type_num = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5707_112_2_17_23_12_24_2742,"ESG: IsBlockValid, block_id_list_ptr->block_count = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5713_112_2_17_23_12_24_2743,"ESG: IsBlockValid, valid!")
TRACE_MSG(MTV_STIMIESG_PARSE_5736_112_2_17_23_12_24_2744,"ESG: IsBlockValid, invalid!")
TRACE_MSG(MTV_STIMIESG_PARSE_5754_112_2_17_23_12_24_2745,"ESG: InsertESGDespInfoList, buff_idx = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5797_112_2_17_23_12_24_2746,"ESG: InsertESGDespInfoList, discard desp.segment_no =%d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5843_112_2_17_23_12_24_2747,"ESG: InsertESGDespInfoList, invalid desp.segment_no =%d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5894_112_2_17_23_12_24_2748,"ESG: CreateESGDespInfoList, buff_idx = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_5955_112_2_17_23_12_24_2749,"ESG: AddESGDespInfoListNode, buff_idx = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6014_112_2_17_23_12_24_2750,"ESG: AddESGDespInfoListNode, discard buff_idx = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6055_112_2_17_23_12_24_2751,"ESG:RemoveFolderFile:  net id = %d configure buffer.")
TRACE_MSG(MTV_STIMIESG_PARSE_6072_112_2_17_23_12_24_2752,"ESG:RemoveFolderFile: CreateDirectory  %s = %d.")
TRACE_MSG(MTV_STIMIESG_PARSE_6079_112_2_17_23_12_24_2753,"ESG:RemoveFolderFile: CreateDirectory  %s = %d.")
TRACE_MSG(MTV_STIMIESG_PARSE_6108_112_2_17_23_12_24_2754,"ESG:RmvCtrlTab: ControTable.")
TRACE_MSG(MTV_STIMIESG_PARSE_6141_112_2_17_23_12_25_2755,"ESG: RemoveLeftFile, full_path_len %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6149_112_2_17_23_12_25_2756,"ESG: RemoveLeftFile, SFS_FindFirstFile ,sfs_handle = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6154_112_2_17_23_12_25_2757,"ESG: RemoveLeftFile, find_data.attr = 0x%x!")
TRACE_MSG(MTV_STIMIESG_PARSE_6174_112_2_17_23_12_25_2758,"ESG: RemoveLeftFile, filename_len = 0x%x!")
TRACE_MSG(MTV_STIMIESG_PARSE_6181_112_2_17_23_12_25_2759,"ESG: RemoveLeftFile, SFS_DeleteFile = %d ")
TRACE_MSG(MTV_STIMIESG_PARSE_6186_112_2_17_23_12_25_2760,"ESG: RemoveLeftFile, filename too long %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6208_112_2_17_23_12_25_2761,"ESG:ProcessStimiDisableDataFilterDoneCmd, cmd id!")
TRACE_MSG(MTV_STIMIESG_PARSE_6213_112_2_17_23_12_25_2762,"ESG:ProcessStimiDisableDataFilterDoneCmd, ESG_STATE_IDLE!")
TRACE_MSG(MTV_STIMIESG_PARSE_6221_112_2_17_23_12_25_2763,"ESG:ProcessStimiDisableDataFilterDoneCmd, ESG_STATE_WAIT_UPDATE!")
TRACE_MSG(MTV_STIMIESG_PARSE_6229_112_2_17_23_12_25_2764,"ESG:ProcessStimiDisableDataFilterDoneCmd, ESG_STATE_LOOKUP_SRV_FRQ!")
TRACE_MSG(MTV_STIMIESG_PARSE_6287_112_2_17_23_12_25_2765,"ESG:ProcessStimiDisableDataFilterDoneCmd, default state %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6301_112_2_17_23_12_25_2766,"ESG:ProcessStopESG!")
TRACE_MSG(MTV_STIMIESG_PARSE_6305_112_2_17_23_12_25_2767,"ESG:ProcessStopESG, current_service_id %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6346_112_2_17_23_12_25_2768,"ESG:ProcessCtrlTabUpdate, s_esg_state = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6390_112_2_17_23_12_25_2769,"ESG: ProcessCtrlTabUpdate, updated control table!")
TRACE_MSG(MTV_STIMIESG_PARSE_6411_112_2_17_23_12_25_2770,"ESG: ProcessCtrlTabUpdate, default %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6426_112_2_17_23_12_25_2771,"ESG:ProcessStimiDisableTunerDoneCmd, resp id %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6431_112_2_17_23_12_25_2772,"ESG:ProcessStimiDisableTunerDoneCmd, ESG_STATE_IDLE!")
TRACE_MSG(MTV_STIMIESG_PARSE_6439_112_2_17_23_12_25_2773,"ESG:ProcessStimiDisableTunerDoneCmd, ESG_STATE_LOOKUP_SRV_FRQ!")
TRACE_MSG(MTV_STIMIESG_PARSE_6484_112_2_17_23_12_25_2774,"ESG:ProcessStimiDisableTunerDoneCmd, default state %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6502_112_2_17_23_12_25_2775,"ESG:ProcessStimiDisableDataFilterDoneCmd, resp id %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6507_112_2_17_23_12_25_2776,"ESG:ProcessStimiDisableChannelDoneCmd, ESG_STATE_IDLE!")
TRACE_MSG(MTV_STIMIESG_PARSE_6515_112_2_17_23_12_25_2777,"ESG:ProcessStimiDisableChannelDoneCmd, ESG_STATE_CONFIG_SERVICE!")
TRACE_MSG(MTV_STIMIESG_PARSE_6541_112_2_17_23_12_25_2778,"ESG:ProcessStimiDisableChannelDoneCmd, default state %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6557_112_2_17_23_12_25_2779,"ESG: ProcessInformEsgEnablePlay, is_enable = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6586_112_2_17_23_12_25_2780,"ESG: FreeDataSlotBuffer!")
TRACE_MSG(MTV_STIMIESG_PARSE_6609_112_2_17_23_12_25_2781,"ESG: FreeDataFilter!")
TRACE_MSG(MTV_STIMIESG_PARSE_6615_112_2_17_23_12_25_2782,"ESG: FreeDataFilter s_esg_data_filter_num %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6661_112_2_17_23_12_26_2783,"ESG: ExitESGSearch, open filename (0x%x) failed!")
TRACE_MSG(MTV_STIMIESG_PARSE_6669_112_2_17_23_12_26_2784,"ESG: ExitESGSearch, read filename size = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6672_112_2_17_23_12_26_2785,"ESG: ExitESGSearch, read filename size = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6675_112_2_17_23_12_26_2786,"ESG: ExitESGSearch, read filename size = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6678_112_2_17_23_12_26_2787,"ESG: ExitESGSearch, read filename size = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6681_112_2_17_23_12_26_2788,"ESG: ExitESGSearch, read filename size = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6694_112_2_17_23_12_26_2789,"ESG: ExitESGSearch!")
TRACE_MSG(MTV_STIMIESG_PARSE_6759_112_2_17_23_12_26_2790,"ESG: GetESGCodeCharSet!")
TRACE_MSG(MTV_STIMIESG_PARSE_6771_112_2_17_23_12_26_2791,"ESG: GetESGCodeCharSet, char_set = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6792_112_2_17_23_12_26_2792,"ESG: GetESGCodeCharSet, net_id = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6810_112_2_17_23_12_26_2793,"ESG: GetESGCodeCharSet, file head, reading file head failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6820_112_2_17_23_12_26_2794,"ESG: GetESGCodeCharSet, SFS_SetFilePointer failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6830_112_2_17_23_12_26_2795,"ESG: GetESGCodeCharSet, SFS_SetFilePointer failed! error = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6859_112_2_17_23_12_26_2796,"ESG: ConfigProgHintFilter!")
TRACE_MSG(MTV_STIMIESG_PARSE_6923_112_2_17_23_12_26_2797,"ESG: MTVSTIMI_SetProgHintInfo, net_id = %d, is_display %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6929_112_2_17_23_12_26_2798,"ESG: MTVSTIMI_SetProgHintInfo, is_display = %d is identical , !")
TRACE_MSG(MTV_STIMIESG_PARSE_6961_112_2_17_23_12_26_2799,"ESG: SetProgHintInfo, net_id = %d, is_display %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_6987_112_2_17_23_12_26_2800,"ESG: SetProgHintInfo, No found esgd!")
TRACE_MSG(MTV_STIMIESG_PARSE_7059_112_2_17_23_12_26_2801,"ESG: ParseProgHintInfo, buff_id = %d, descriptor_count = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7076_112_2_17_23_12_26_2802,"ESG: ParseProgHintInfo, descp_buff_ptr no enough memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7125_112_2_17_23_12_27_2803,"ESG: ParseProgHintInfo, hint_buff_ptrno enough memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7152_112_2_17_23_12_27_2804,"ESG: ParseProgHintInfo, data type%d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7166_112_2_17_23_12_27_2805,"ESG: ParseProgHintInfo, hint_buff_ptrno enough memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7179_112_2_17_23_12_27_2806,"ESG: ParseProgHintInfo, num = %d invalid!")
TRACE_MSG(MTV_STIMIESG_PARSE_7215_112_2_17_23_12_27_2807,"ESG: ParseProgHintInfo, hint language invalid!")
TRACE_MSG(MTV_STIMIESG_PARSE_7248_112_2_17_23_12_27_2808,"ESG: Hint_ProcessDemuxData, buff_id = %d, descriptor_len = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7271_112_2_17_23_12_27_2809,"ESG: ParseProgHintInfo, data_unit.unit_type =  %d error!")
TRACE_MSG(MTV_STIMIESG_PARSE_7279_112_2_17_23_12_27_2810,"ESG: Hint_ProcessDemuxData, hint_buff_ptrno enough memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7305_112_2_17_23_12_27_2811,"ESG: Hint_ProcessDemuxData, data type%d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7312_112_2_17_23_12_27_2812,"ESG: Hint_ProcessDemuxData, hint_buff_ptrno enough memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7323_112_2_17_23_12_27_2813,"ESG: Hint_ProcessDemuxData, num = %d invalid!")
TRACE_MSG(MTV_STIMIESG_PARSE_7360_112_2_17_23_12_27_2814,"ESG: ParseProgHintInfo, hint language invalid!")
TRACE_MSG(MTV_STIMIESG_PARSE_7531_112_2_17_23_12_27_2815,"ESG: MTVSTIMI_GetESGDataInfo , net id =  %d, data type = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7537_112_2_17_23_12_27_2816,"ESG: MTVSTIMI_GetESGDataInfo , Open file(0x%x) failed!")
TRACE_MSG(MTV_STIMIESG_PARSE_7546_112_2_17_23_12_27_2817,"ESG: MTVSTIMI_GetESGDataInfo , read file(0x%x) failed!")
TRACE_MSG(MTV_STIMIESG_PARSE_7560_112_2_17_23_12_27_2818,"ESG: MTVSTIMI_GetESGDataInfo, invalid type %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7569_112_2_17_23_12_27_2819,"ESG: MTVSTIMI_GetESGDataInfo, file_info_ptr->file_name_ptr, no memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7581_112_2_17_23_12_27_2820,"ESG: filename,  found filename = 0x%x")
TRACE_MSG(MTV_STIMIESG_PARSE_7594_112_2_17_23_12_27_2821,"ESG: MTVSTIMI_GetESGDataInfo, file_count %d,  file_num %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7612_112_2_17_23_12_27_2822,"ESG: MTVSTIMI_GetESGDataInfo , net id =  %d, data type = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7623_112_2_17_23_12_28_2823,"ESG: MTVSTIMI_GetESGDataInfo, not ESG config file!")
TRACE_MSG(MTV_STIMIESG_PARSE_7633_112_2_17_23_12_28_2824,"ESG: MTVSTIMI_GetESGDataInfo, read config file failed %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7648_112_2_17_23_12_28_2825,"ESG: MTVSTIMI_GetESGDataInfo, invalid type %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7657_112_2_17_23_12_28_2826,"ESG: MTVSTIMI_GetESGDataInfo, file_info_ptr->file_name_ptr, no memory %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7682_112_2_17_23_12_28_2827,"ESG: MTVSTIMI_GetESGDataInfo, find_data.name[%d] =  %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7696_112_2_17_23_12_28_2828,"ESG: MTVSTIMI_GetESGDataInfo ,finished %d")
TRACE_MSG(MTV_STIMIESG_PARSE_7715_112_2_17_23_12_28_2829,"ESG: MTVSTIMI_GetESGDataInfo, file_count %d,  file_num %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7737_112_2_17_23_12_28_2830,"ESG: MTVSTIMI_CreateFile,  file_name = 0x%x,  access_mode =  0x%x!")
TRACE_MSG(MTV_STIMIESG_PARSE_7745_112_2_17_23_12_28_2831,"ESG: MTVSTIMI_CreateFile,  found file_name = 0x%x")
TRACE_MSG(MTV_STIMIESG_PARSE_7757_112_2_17_23_12_28_2832,"ESG: MTVSTIMI_CreateFile,  can't open  file_name = 0x%x")
TRACE_MSG(MTV_STIMIESG_PARSE_7770_112_2_17_23_12_28_2833,"ESG: MTVSTIMI_CreateFile,  first, alloc mem( %d) failed")
TRACE_MSG(MTV_STIMIESG_PARSE_7793_112_2_17_23_12_28_2834,"ESG: MTVSTIMI_CreateFile,  add, alloc mem( %d) failed")
TRACE_MSG(MTV_STIMIESG_PARSE_7821_112_2_17_23_12_28_2835,"ESG: MTVSTIMI_CreateFile,  can't open  multiply ")
TRACE_MSG(MTV_STIMIESG_PARSE_7841_112_2_17_23_12_28_2836,"ESG: MTVSTIMI_CreateFile,  create existing  file_name = 0x%x")
TRACE_MSG(MTV_STIMIESG_PARSE_7845_112_2_17_23_12_28_2837,"ESG: MTVSTIMI_CreateFile,  can't open  multiply ")
TRACE_MSG(MTV_STIMIESG_PARSE_7896_112_2_17_23_12_28_2838,"ESG: MTVSTIMI_ReadFile,  handle = 0x%x, bytes_to_read = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7902_112_2_17_23_12_28_2839,"ESG: MTVSTIMI_ReadFile,  rcv_segment_count = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7923_112_2_17_23_12_28_2840,"ESG: MTVSTIMI_ReadFile,   cur_sect = %d, offset = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7924_112_2_17_23_12_28_2841,"ESG: MTVSTIMI_ReadFile,   i = %d, segment_len = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7953_112_2_17_23_12_28_2842,"ESG: MTVSTIMI_ReadFile,   cur_sect = %d, offset = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_7972_112_2_17_23_12_28_2843,"ESG: MTVSTIMI_CloseFile,  handle = 0x%x!")
TRACE_MSG(MTV_STIMIESG_PARSE_7982_112_2_17_23_12_28_2844,"ESG: MTVSTIMI_CloseFile,  found file_name = 0x%x")
TRACE_MSG(MTV_STIMIESG_PARSE_8006_112_2_17_23_12_28_2845,"ESG: RemoveAllMemFile , net_id = %d!")
TRACE_MSG(MTV_STIMIESG_PARSE_8048_112_2_17_23_12_29_2846,"ESG: RemoveAllMemFile, filename = 0x0x!")
TRACE_MSG(MTV_STIMIESG_PARSE_8082_112_2_17_23_12_29_2847,"ESG: MTVSTIMI_SetEsgDataPath , esg_path_ptr = %s, esg_path_len =%d!")
TRACE_MSG(MTV_STIMIESG_PARSE_8103_112_2_17_23_12_29_2848,"ESG: MTVSTIMI_GetEsgDataPath , esg_path_addr = %x, esg_path_len =%d!")
TRACE_MSG(MTV_STIMIESG_PARSE_8147_112_2_17_23_12_29_2849,"ESG: EsgDataCallback buffer_id %d \n")
TRACE_MSG(MTV_STIMIESG_PARSE_8170_112_2_17_23_12_29_2850,"ESG: EsgDataCallback descriptor_ptr is null \n")
TRACE_MSG(MTV_STIMIESG_PARSE_8176_112_2_17_23_12_29_2851,"ESG: EsgDataCallback mtv_signal_ptr is null \n")
TRACE_MSG(MTV_WRAPPER_153_112_2_17_23_12_29_2852,"fake mmi thread has been created!\n")
TRACE_MSG(MTV_WRAPPER_156_112_2_17_23_12_29_2853,"ProxyCallPeer has been initialized!\n")
TRACE_MSG(MTV_WRAPPER_253_112_2_17_23_12_29_2854,"gdacman has been initialized!\n")
TRACE_MSG(MTV_WRAPPER_269_112_2_17_23_12_30_2855,"Delete Thread ERROR! %s %d")
TRACE_MSG(MTV_WRAPPER_352_112_2_17_23_12_30_2856,"PlayNetworkByFrq: frequency (%d) cannot get network id")
TRACE_MSG(MTV_WRAPPER_896_112_2_17_23_12_31_2857,"mtv_v:CMMB_GetAllProg() nid = %d sid = %d date %d %d %d")
TRACE_MSG(MTV_WRAPPER_904_112_2_17_23_12_31_2858,"mtv_v:CMMB_GetAllProg() ret = %d proglist_ptr = 0X%x")
TRACE_MSG(MTV_WRAPPER_911_112_2_17_23_12_31_2859,"mtv_v:CMMB_FreeAllProg_wrapper() enter")
TRACE_MSG(MTV_WRAPPER_932_112_2_17_23_12_31_2860,"mtv_v:CMMB_GetEsgDateRange_wrapper")
TRACE_MSG(MTV_WRAPPER_949_112_2_17_23_12_31_2861,"mtv_v:CMMB_SetEsgFilter_wrapper")
TRACE_MSG(MTV_WRAPPER_1002_112_2_17_23_12_31_2862,"MTVWRAPPER:CMMB_SearchProgEndInd, net_num = %d")
TRACE_MSG(MTV_WRAPPER_1131_112_2_17_23_12_31_2863,"MTVWRAPPER:CMMB_SearchEsgEndInd_wrapper, ")
TRACE_MSG(MTV_WRAPPER_1164_112_2_17_23_12_32_2864,"MTVWRAPPER: CMMB_SearchEsgErrInd_wrapper, error = %d,")
TRACE_MSG(MTV_WRAPPER_1448_112_2_17_23_12_32_2865,"MTVWRAPPER:CMMB_SearchEsgEndIndForDetail_wrapper, net_id = %d")
TRACE_MSG(MTV_WRAPPER_1450_112_2_17_23_12_32_2866,"MTVWRAPPER:MMIMTV_StartParsing, err = %d")
TRACE_MSG(MTV_WRAPPER_1651_112_2_17_23_12_32_2867,"CMMB_AbortESG_wrapper enter")
TRACE_MSG(MTV_WRAPPER_1669_112_2_17_23_12_33_2868,"fake_mmi recv: 0x%X 0x%X")
TRACE_MSG(MTV_WRAPPER_1883_112_2_17_23_12_33_2869,"unhandled id %d")
TRACE_MSG(MTV_WRAPPER_2318_112_2_17_23_12_34_2870,"mtvesg MTV_ESG_START_PARSE_IND result=%d")
TRACE_MSG(MTV_WRAPPER_2326_112_2_17_23_12_34_2871,"mtvesg MTV_ESG_START_PARSE_IND result=%d")
TRACE_MSG(MTV_WRAPPER_2334_112_2_17_23_12_34_2872,"MW, SignalCode=%d\n")
TRACE_MSG(MTVESG_COMMON_172_112_2_17_23_12_34_2873,"[ESG]tm_mday = %d,tm_mon = %d,tm_year = %dn")
TRACE_MSG(MTVESG_COMMON_230_112_2_17_23_12_34_2874,"[ESG]mtv_MatchServiceID sid = %d")
TRACE_MSG(MTVESG_COMMON_233_112_2_17_23_12_34_2875,"[ESG]p_service->ServiceID = %d")
TRACE_MSG(MTVESG_COMMON_275_112_2_17_23_12_35_2876,"[ESG]mtv_GetAllEsgProg() sid = %d")
TRACE_MSG(MTVESG_COMMON_276_112_2_17_23_12_35_2877,"input year = %d mon = %d day = %d")
TRACE_MSG(MTVESG_COMMON_279_112_2_17_23_12_35_2878,"[ESG]mtv_GetAllEsgProg() proglist_ptr is NULL")
TRACE_MSG(MTVESG_COMMON_290_112_2_17_23_12_35_2879,"[ESG]mtv_GetAllEsgProg() p_schedule = 0X%x")
TRACE_MSG(MTVESG_COMMON_300_112_2_17_23_12_35_2880,"[ESG]mtv_GetAllEsgProg() count = %d")
TRACE_MSG(MTVESG_COMMON_305_112_2_17_23_12_35_2881,"mmiesg -- malloc trace:mtv_GetAllEsgProg size = %d")
TRACE_MSG(MTVESG_COMMON_341_112_2_17_23_12_35_2882,"[ESG]mtv_GetAllEsgProg() NumOfProg = %d")
TRACE_MSG(MTVESG_COMMON_374_112_2_17_23_12_35_2883,"[ESG]mtv_GetEsgProg() sid = %d")
TRACE_MSG(MTVESG_COMMON_376_112_2_17_23_12_35_2884,"year = %d mon = %d day = %d hour = %d min = %d")
TRACE_MSG(MTVESG_COMMON_379_112_2_17_23_12_35_2885,"[ESG]mtv_GetEsgProg prog_ptr is NULL!!")
TRACE_MSG(MTVESG_COMMON_388_112_2_17_23_12_35_2886,"[ESG]mtv_GetEsgProg() p_schedule = 0X%x")
TRACE_MSG(MTVESG_COMMON_392_112_2_17_23_12_35_2887,"[ESG]mtv_GetEsgProg() p_schedule id = %d")
TRACE_MSG(MTVESG_COMMON_404_112_2_17_23_12_35_2888,"[ESG]mtv_GetEsgProg() p_content = 0X%x")
TRACE_MSG(MTVESG_COMMON_432_112_2_17_23_12_35_2889,"[ESG]mtv_GetEsgProg() is_found = %d")
TRACE_MSG(MTVESG_COMMON_542_112_2_17_23_12_35_2890,"[ESG]mtv_CheckSpace SFS_GetDeviceFreeSpace, no enouth space")
TRACE_MSG(MTVESG_COMMON_548_112_2_17_23_12_35_2891,"[ESG]mtv_CheckSpace SFS_GetDeviceFreeSpace! error = %d!")
TRACE_MSG(MTVESG_COMMON_577_112_2_17_23_12_35_2892,"mtvesg -- malloc trace:MMIMTV_CopyString size = %d")
TRACE_MSG(MTVESG_COMMON_592_112_2_17_23_12_35_2893,"mtvesg -- malloc trace:MMIMTV_CopyString size = %d")
TRACE_MSG(MTVESG_COMMON_607_112_2_17_23_12_35_2894,"mtvesg -- malloc trace:MTV_CopyString size = %d")
TRACE_MSG(MTVESG_COMMON_642_112_2_17_23_12_35_2895,"[ESG]MTV_RunReqHandler(): result = %d")
TRACE_MSG(MTVESG_COMMON_663_112_2_17_23_12_35_2896,"[ESG]MTV_RequestProgInfo()")
TRACE_MSG(MTVESG_COMMON_694_112_2_17_23_12_35_2897,"mtvesg_common.c.c:MTV_GetAllProg()")
TRACE_MSG(MTVESG_COMMON_698_112_2_17_23_12_35_2898,"mtvesg_common.c.c:MTV_GetAllProg() proglist_ptr is NULL")
TRACE_MSG(MTVESG_COMMON_748_112_2_17_23_12_36_2899,"[ESG]MTV_GetCurProg() type = %d")
TRACE_MSG(MTVESG_COMMON_785_112_2_17_23_12_36_2900,"[ESG]MTV_FreeAllProg() listptr=0x%x sid=%d total=%d progptr=0x%x")
TRACE_MSG(MTVESG_COMMON_789_112_2_17_23_12_36_2901,"[ESG]Free ServiceName Done")
TRACE_MSG(MTVESG_COMMON_801_112_2_17_23_12_36_2902,"[ESG]Free proglist Done")
TRACE_MSG(MTVESG_COMMON_842_112_2_17_23_12_36_2903,"[ESG]MTV_GetServiceName() type = %d")
TRACE_MSG(MTVESG_COMMON_845_112_2_17_23_12_36_2904,"[ESG]MTV_GetServiceName() name_ptr is NULL!!")
TRACE_MSG(MTVESG_COMMON_878_112_2_17_23_12_36_2905,"[ESG]MTV_GetServiceName() ret = %d")
TRACE_MSG(MTVESG_COMMON_893_112_2_17_23_12_36_2906,"[ESG]MTV_GetServiceAddinfo()")
TRACE_MSG(MTVESG_COMMON_896_112_2_17_23_12_36_2907,"[ESG]MTV_GetServiceAddinfo() info_ptr is NULL")
TRACE_MSG(MTVESG_COMMON_915_112_2_17_23_12_36_2908,"[ESG]MTV_GetServiceAddinfo() ret = %d")
TRACE_MSG(MTVESG_COMMON_928_112_2_17_23_12_36_2909,"[ESG]MTV_StartParsingAsyn() id = %d")
TRACE_MSG(MTVESG_COMMON_972_112_2_17_23_12_36_2910,"[ESG]MTV_StartParsing() id = %d")
TRACE_MSG(MTVESG_COMMON_1008_112_2_17_23_12_36_2911,"[ESG]MTV_ParsingService() id = %d")
TRACE_MSG(MTVESG_COMMON_1027_112_2_17_23_12_36_2912,"[ESG]MTV_SetParseCallback()")
TRACE_MSG(MTVESG_COMMON_1062_112_2_17_23_12_36_2913,"[ESG]mtv_GetDateRange() sid = %d")
TRACE_MSG(MTVESG_COMMON_1078_112_2_17_23_12_36_2914,"[ESG]mtv_GetDateRange() count = %d")
TRACE_MSG(MTVESG_COMMON_1094_112_2_17_23_12_36_2915,"[ESG]mtv_GetDateRange() init date")
TRACE_MSG(MTVESG_COMMON_1135_112_2_17_23_12_36_2916,"[ESG]mtv_GetDateRange() s %d %d %d  e %d %d %d ")
TRACE_MSG(MTVESG_COMMON_1160_112_2_17_23_12_36_2917,"[ESG]MTV_GetEsgDateRange()")
TRACE_MSG(MTVESG_COMMON_1163_112_2_17_23_12_36_2918,"[ESG]MTV_GetEsgDateRange() param error")
TRACE_MSG(MTVESG_COMMON_1187_112_2_17_23_12_36_2919,"[ESG]MTV_SetEsgFilter()")
TRACE_MSG(MTVESG_COMMON_1204_112_2_17_23_12_36_2920,"[ESG]MTV_DisableEsgFilter()")
TRACE_MSG(MTVESG_PARSER_273_112_2_17_23_12_37_2921,"mtvesg_parser.c:expat_Malloc, size=%d")
TRACE_MSG(MTVESG_PARSER_284_112_2_17_23_12_37_2922,"mtvesg_parser.c:expat_Malloc ptr=0x%p malloc=%d")
TRACE_MSG(MTVESG_PARSER_296_112_2_17_23_12_37_2923,"mtvesg_parser.c:expat_realloc, new size=%d")
TRACE_MSG(MTVESG_PARSER_306_112_2_17_23_12_37_2924,"mtvesg_parser.c:expat_realloc, org size=%d")
TRACE_MSG(MTVESG_PARSER_320_112_2_17_23_12_37_2925,"mtvesg_parser.c:expat_realloc ptr=0x%p realloc=%d")
TRACE_MSG(MTVESG_PARSER_335_112_2_17_23_12_37_2926,"mtvesg_parser.c:expat_Free ptr=0x%p")
TRACE_MSG(MTVESG_PARSER_539_112_2_17_23_12_38_2927,"mtvesg -- malloc trace:Esg_SetString size = %d ptr = 0x%x")
TRACE_MSG(MTVESG_PARSER_557_112_2_17_23_12_38_2928,"mtvesg -- malloc trace:Esg_SetString size = %d ptr = 0x%x")
TRACE_MSG(MTVESG_PARSER_572_112_2_17_23_12_38_2929,"mtvesg_parser.c:Esg_NewElement() el = %d")
TRACE_MSG(MTVESG_PARSER_584_112_2_17_23_12_38_2930,"mtvesg_parser.c:Esg_NewElement() wrong element ID")
TRACE_MSG(MTVESG_PARSER_591_112_2_17_23_12_38_2931,"mtvesg -- malloc trace:Esg_NewElement size = %d ptr = 0x%x")
TRACE_MSG(MTVESG_PARSER_778_112_2_17_23_12_38_2932,"mtvesg_parser.c:Esg_AddElement() el = %d")
TRACE_MSG(MTVESG_PARSER_975_112_2_17_23_12_39_2933,"mtvesg_parser.c:Esg_FreeService()")
TRACE_MSG(MTVESG_PARSER_1001_112_2_17_23_12_39_2934,"mtvesg_parser.c:Esg_FreeSchedule()")
TRACE_MSG(MTVESG_PARSER_1027_112_2_17_23_12_39_2935,"mtvesg_parser.c:Esg_FreeContent()")
TRACE_MSG(MTVESG_PARSER_1060_112_2_17_23_12_39_2936,"mtvesg_parser.c:Esg_FreeElementOnError root el = %d")
TRACE_MSG(MTVESG_PARSER_1076_112_2_17_23_12_39_2937,"mtvesg_parser.c:Esg_FreeElementOnError wrong element")
TRACE_MSG(MTVESG_PARSER_1093_112_2_17_23_12_39_2938,"mtvesg_parser.c:Esg_FreeGlobalVar()")
TRACE_MSG(MTVESG_PARSER_1120_112_2_17_23_12_39_2939,"mtvesg_parser.c:Esg_Zcalloc, size=%d ptr = 0x%x")
TRACE_MSG(MTVESG_PARSER_1169_112_2_17_23_12_39_2940,"mtvesg_parser.c:Esg_GetFileHeadLen(): len = %d ret = %d")
TRACE_MSG(MTVESG_PARSER_1192_112_2_17_23_12_39_2941,"Esg_InflateData() param error!")
TRACE_MSG(MTVESG_PARSER_1195_112_2_17_23_12_39_2942,"Esg_InflateData() ,src_handle = %d, dst_handle = %d")
TRACE_MSG(MTVESG_PARSER_1206_112_2_17_23_12_39_2943,"Esg_InflateData() ,z_inflateInit2_ = %d!")
TRACE_MSG(MTVESG_PARSER_1241_112_2_17_23_12_39_2944,"Esg_InflateData() ,z_inflate = %d!")
TRACE_MSG(MTVESG_PARSER_1251_112_2_17_23_12_39_2945,"Esg_InflateData() ,ignore -3 error. now ret = %d!")
TRACE_MSG(MTVESG_PARSER_1258_112_2_17_23_12_39_2946,"Esg_InflateData() ,SFS_WriteFile = %d!")
TRACE_MSG(MTVESG_PARSER_1289_112_2_17_23_12_39_2947,"Esg_InflateFile get file name fail")
TRACE_MSG(MTVESG_PARSER_1311_112_2_17_23_12_39_2948,"Esg_InflateFile create file fail")
TRACE_MSG(MTVESG_PARSER_1333_112_2_17_23_12_39_2949,"mtvesg_parser.c:Esg_ParseFileByHandle()")
TRACE_MSG(MTVESG_PARSER_1346_112_2_17_23_12_39_2950,"mtvesg_parser.c:Esg_ParseFileByHandle(): create parser fail")
TRACE_MSG(MTVESG_PARSER_1369_112_2_17_23_12_39_2951,"mtvesg_parser.c:Parse error at line %dn %sn")
TRACE_MSG(MTVESG_PARSER_1381_112_2_17_23_12_39_2952,"mtvesg_parser.c:Esg_ParseFileByHandle(): abort")
TRACE_MSG(MTVESG_PARSER_1390_112_2_17_23_12_39_2953,"mtvesg_parser.c:Esg_ParseFileByHandle() do count = %d")
TRACE_MSG(MTVESG_PARSER_1418_112_2_17_23_12_40_2954,"mtvesg_parser.c:MTVESG_ParseEsgByType(): type = %d")
TRACE_MSG(MTVESG_PARSER_1432_112_2_17_23_12_40_2955,"Esg_ParseFileByHandle Esg_GetFileHeadLen result = %d")
TRACE_MSG(MTVESG_PARSER_1455_112_2_17_23_12_40_2956,"Esg_ParseFileByHandle open temp inflate file fail")
TRACE_MSG(MTVESG_PARSER_1460_112_2_17_23_12_40_2957,"Esg_ParseFileByHandle delete temp file")
TRACE_MSG(MTVESG_PARSER_1466_112_2_17_23_12_40_2958,"Esg_ParseFileByHandle inflate fail")
TRACE_MSG(MTVESG_PARSER_1479_112_2_17_23_12_40_2959,"Esg_ParseFileByHandle close esg file")
TRACE_MSG(MTVESG_PARSER_1486_112_2_17_23_12_40_2960,"mtvesg_parser.c:MTVESG_ParseEsgByType(): read file fail")
TRACE_MSG(MTVESG_PARSER_1492_112_2_17_23_12_40_2961,"mtvesg_parser.c:MTVESG_ParseEsgByType(): user abort")
TRACE_MSG(MTVESG_PARSER_1499_112_2_17_23_12_40_2962,"mtvesg_parser.c:MTVESG_ParseEsgByType(): no ESG files")
TRACE_MSG(MTVESG_PARSER_1535_112_2_17_23_12_40_2963,"mtvesg_parser.c:Esg_GetDataHeadLen(): filename = %d")
TRACE_MSG(MTVESG_PARSER_1537_112_2_17_23_12_40_2964,"mtvesg_parser.c:Esg_GetDataHeadLen(): handle = 0X%x")
TRACE_MSG(MTVESG_PARSER_1549_112_2_17_23_12_40_2965,"mtvesg_parser.c:Esg_GetDataHeadLen(): headlen = %d ret = %d")
TRACE_MSG(MTVESG_PARSER_1577_112_2_17_23_12_40_2966,"Esg_InflateAndParseData(),filename = %d")
TRACE_MSG(MTVESG_PARSER_1579_112_2_17_23_12_40_2967,"mtvesg_parser.c:Esg_InflateAndParseData(): handle = 0X%x")
TRACE_MSG(MTVESG_PARSER_1592_112_2_17_23_12_40_2968,"Esg_InflateAndParseData(),z_inflateInit2_ = %d!")
TRACE_MSG(MTVESG_PARSER_1598_112_2_17_23_12_40_2969,"Esg_InflateAndParseData(): create parser fail")
TRACE_MSG(MTVESG_PARSER_1613_112_2_17_23_12_40_2970,"Esg_InflateAndParseData(),charsread = %d")
TRACE_MSG(MTVESG_PARSER_1635_112_2_17_23_12_40_2971,"Esg_InflateAndParseData(),z_inflate = %d!")
TRACE_MSG(MTVESG_PARSER_1645_112_2_17_23_12_40_2972,"Esg_InflateAndParseData(),ignore -3 error. now inf_ret = %d!")
TRACE_MSG(MTVESG_PARSER_1651_112_2_17_23_12_40_2973,"Esg_InflateAndParseData(),have = %d")
TRACE_MSG(MTVESG_PARSER_1654_112_2_17_23_12_40_2974,"Esg_InflateAndParseData(),read_done = %d parse_done = %d")
TRACE_MSG(MTVESG_PARSER_1656_112_2_17_23_12_40_2975,"Esg_InflateAndParseData(),xml_ret = %d!")
TRACE_MSG(MTVESG_PARSER_1691_112_2_17_23_12_40_2976,"mtvesg_parser.c:Esg_ParseData() len = %d, off = %d done = %d")
TRACE_MSG(MTVESG_PARSER_1701_112_2_17_23_12_40_2977," msg_ptr[%03d] = %02x %02x %02x %02x %02x %02x %02x %02x")
TRACE_MSG(MTVESG_PARSER_1717_112_2_17_23_12_40_2978,"mtvesg_parser.c:Parse error at line %dn %sn")
TRACE_MSG(MTVESG_PARSER_1741_112_2_17_23_12_40_2979,"mtvesg_parser.c:Esg_ParseDataByHandle() filename = %d")
TRACE_MSG(MTVESG_PARSER_1744_112_2_17_23_12_40_2980,"mtvesg_parser.c:Esg_ParseDataByHandle(): handle = 0X%x")
TRACE_MSG(MTVESG_PARSER_1749_112_2_17_23_12_40_2981,"mtvesg_parser.c:Esg_ParseDataByHandle(): create parser fail")
TRACE_MSG(MTVESG_PARSER_1795_112_2_17_23_12_40_2982,"mtvesg_parser.c:MTVESG_ParseEsgByTypeInMem(): type = %d")
TRACE_MSG(MTVESG_PARSER_1825_112_2_17_23_12_40_2983,"mtvesg_parser.c:MTVESG_ParseEsgByTypeInMem(): no ESG data")
TRACE_MSG(MTVESG_PARSER_1856_112_2_17_23_12_40_2984,"mtvesg_parser.c:MTVESG_InitGlobalVar()")
TRACE_MSG(MTVESG_PARSER_1909_112_2_17_23_12_41_2985,"MTVESG_ParseThreadInit tid = %d")
TRACE_MSG(MTVESG_PARSER_1952_112_2_17_23_12_41_2986,"esg_ParseThreadEntry sig_ptr is NULL!!")
TRACE_MSG(MTVESG_PARSER_1959_112_2_17_23_12_41_2987,"esg_ParseThreadEntry ESGPARASER_SIG_PARSE")
TRACE_MSG(MTVESG_PARSER_1991_112_2_17_23_12_41_2988,"esg_ParseThreadEntry ESGPARASER_SIG_EXIT")
TRACE_MSG(MTVESG_PARSER_1995_112_2_17_23_12_41_2989,"esg_ParseThreadEntry Unknow signal id!")
TRACE_MSG(MTVESG_PARSER_2013_112_2_17_23_12_41_2990,"esg_ParseThreadEntry thread exit")
TRACE_MSG(MTVESG_PARSER_2023_112_2_17_23_12_41_2991,"mtvesg_parser.c:ESG_IsAbort() = %d")
TRACE_MSG(PPBUFFER_75_112_2_17_23_12_41_2992,"MTVAUDIO_Init: allocate memory failed.")
TRACE_MSG(PPBUFFER_140_112_2_17_23_12_41_2993,"PPBUFFER: in buffer not full")
TRACE_MSG(PPBUFFER_142_112_2_17_23_12_41_2994,"PPBUFFER: out buffer not empty")
TRACE_MSG(PPBUFFER_162_112_2_17_23_12_41_2995,"PPBUFFER: PrepareInBuffer buffer is not empty")
TRACE_MSG(PPBUFFER_170_112_2_17_23_12_41_2996,"PPBUFFER: PrepareInBuffer allocate larger buffer %d=>%d")
TRACE_MSG(PPBUFFER_173_112_2_17_23_12_41_2997,"PPBUFFER: PrepareInBuffer buf_size 0x%x larger than 0x2000!!!")
TRACE_MSG(PPBUFFER_180_112_2_17_23_12_41_2998,"PPBUFFER: buf ptr is NULL!!!")
TRACE_MSG(SPI_AUDIO_530_112_2_17_23_12_43_2999,"SPIAUDIO: W 0x%X[%d]")
TRACE_MSG(SYNC_FUNC_99_112_2_17_23_12_44_3000,"SyncFunc recvACK: 0x%X")
TRACE_MSG(APDAC_125_112_2_17_23_12_46_3001,"apdac: %d")
TRACE_MSG(DTL_RFVT_219_112_2_17_23_12_47_3002,"DTLRFVT_Callback resp_cmd_ptr->resp_id=0x%x")
TRACE_MSG(DTL_RFVT_247_112_2_17_23_12_47_3003,"DTLRFVT_Callback wrong resp id")
TRACE_MSG(DTL_RFVT_250_112_2_17_23_12_47_3004,"DTLRFVT_Callback subtype = %d")
TRACE_MSG(DTL_RFVT_281_112_2_17_23_12_47_3005,"[RFTEST]:DTLRFVT_HandleToolMsg src_ptr = %x, msg_head->subtype=%d, src_len = %d")
TRACE_MSG(DTL_RFVT_289_112_2_17_23_12_47_3006,"[RFTEST]:DTLRFVT_HandleToolMsg FREQ = %d")
TRACE_MSG(DTL_RFVT_295_112_2_17_23_12_47_3007,"[RFTEST]:DTLRFVT_HandleToolMsg RSSI = %d")
TRACE_MSG(DTL_RFVT_308_112_2_17_23_12_47_3008,"[RFTEST]:DTLRFVT_HandleToolMsg ret = %d BER = %d SNR = %d")
TRACE_MSG(DTL_RFVT_312_112_2_17_23_12_47_3009,"[RFTEST]:DTLRFVT_HandleToolMsg Reset Mode")
TRACE_MSG(DTL_RFVT_320_112_2_17_23_12_47_3010,"[RFTEST]:DTLRFVT_HandleToolMsg Reset Mode wrong mode")
TRACE_MSG(DTL_RFVT_327_112_2_17_23_12_47_3011,"[RFTEST]:DTLRFVT_HandleToolMsg Get Mode = %d")
TRACE_MSG(DTL_RFVT_332_112_2_17_23_12_47_3012,"DTLRFVT_HandleToolMsg Read NV")
TRACE_MSG(DTL_RFVT_360_112_2_17_23_12_47_3013,"DTLRFVT_HandleToolMsg Write NV %d %d %d %d %d %d %d %d %d")
TRACE_MSG(DTL_RFVT_366_112_2_17_23_12_47_3014,"[RFTEST]:DTLRFVT_HandleToolMsg dvt test task id = %d")
TRACE_MSG(DTL_RFVT_398_112_2_17_23_12_47_3015,"[RFTEST]:DTLRFVT_HandleToolMsg CMMBSN start")
TRACE_MSG(DTL_RFVT_400_112_2_17_23_12_47_3016,"[RFTEST]:DTLRFVT_HandleToolMsg Get CMMBSN ret = %d")
TRACE_MSG(DTL_RFVT_406_112_2_17_23_12_47_3017,"[RFTEST]:DTLRFVT_HandleToolMsg wrong diag command")
TRACE_MSG(DTL_RFVT_437_112_2_17_23_12_47_3018,"[RFTEST]:dtlrfvt_SendData subtype = %d")
TRACE_MSG(DTL_RFVT_452_112_2_17_23_12_47_3019,"[RFTEST]: msg_ptr[%03d] = %02x %02x %02x %02x %02x %02x %02x %02x")
TRACE_MSG(DTL_RFVT_476_112_2_17_23_12_48_3020,"[RFTEST]:DTLRFVT_ResetMode mode = %d current mode = %d")
TRACE_MSG(DTL_RFVT_486_112_2_17_23_12_48_3021,"[RFTEST]:DTLRFVT_ResetMode not support normal mode")
TRACE_MSG(DTL_RFVT_514_112_2_17_23_12_48_3022,"[RFTEST]:DTLRFVT_ResetMode wrong command id")
TRACE_MSG(DTL_RFVT_531_112_2_17_23_12_48_3023,"[RFTEST]: rftest_Rssi freq = %d tick = %d")
TRACE_MSG(DTL_RFVT_536_112_2_17_23_12_48_3024,"[RFTEST]: rftest_Rssi already in play mode  cur mode=%d")
TRACE_MSG(DTL_RFVT_542_112_2_17_23_12_48_3025,"[RFTEST]: rftest_Rssi CMMB_SearchCenterFrq error")
TRACE_MSG(DTL_RFVT_568_112_2_17_23_12_48_3026,"DTLRFVT_StartPlayNetWork already in play mode, return")
TRACE_MSG(DTL_RFVT_573_112_2_17_23_12_48_3027,"DTLRFVT_StartPlayNetWork need to poweroff seine")
TRACE_MSG(DTL_RFVT_576_112_2_17_23_12_48_3028,"DTLRFVT_StartPlayNetWork freq = %d, alpha = %d")
TRACE_MSG(DTL_RFVT_579_112_2_17_23_12_48_3029,"DTLRFVT_StartPlayNetWork thread_id = %d")
TRACE_MSG(DTL_RFVT_598_112_2_17_23_12_48_3030,"DTLRFVT_StartPlayNetWork play init")
TRACE_MSG(DTL_RFVT_611_112_2_17_23_12_48_3031,"DTLRFVT_StartPlayNetWork start play")
TRACE_MSG(DTL_RFVT_630_112_2_17_23_12_48_3032,"DTLRFVT_StartPlayNetWork play state = %d")
TRACE_MSG(DTL_RFVT_635_112_2_17_23_12_48_3033,"DTLRFVT_StartPlayNetWork time out")
TRACE_MSG(DTL_RFVT_677_112_2_17_23_12_48_3034,"DTLRFVT_StopPlayNetWork mode = %d")
TRACE_MSG(DTL_RFVT_684_112_2_17_23_12_48_3035,"DTLRFVT_StopPlayNetWork stop state = %d")
TRACE_MSG(DTL_RFVT_692_112_2_17_23_12_48_3036,"DTLRFVT_StopPlayNetWork not in play mode, return")
TRACE_MSG(DTL_RFVT_800_112_2_17_23_12_48_3037,"DTLRFVT_StopPlayBackFile()")
TRACE_MSG(DTL_RFVT_815_112_2_17_23_12_48_3038,"DTLRFVT_StopPlayBackFile() END")
TRACE_MSG(DTL_RFVT_863_112_2_17_23_12_48_3039,"[RFTEST]:dtl_InitSeine tick = %d")
TRACE_MSG(DTL_RFVT_865_112_2_17_23_12_48_3040,"[RFTEST]: create memory fail")
TRACE_MSG(DTL_RFVT_868_112_2_17_23_12_48_3041,"[RFTEST]: create memory ok")
TRACE_MSG(DTL_RFVT_871_112_2_17_23_12_48_3042,"[RFTEST]: MTV_Initialize ok")
TRACE_MSG(DTL_RFVT_873_112_2_17_23_12_48_3043,"[RFTEST]: CMMB_Init ok")
TRACE_MSG(DTL_RFVT_877_112_2_17_23_12_48_3044,"[RFTEST]:dtl_InitSeine done tick = %d")
TRACE_MSG(DTL_RFVT_881_112_2_17_23_12_48_3045,"[RFTEST]:dtl_InitSeine already in normal mode, return")
TRACE_MSG(DTL_RFVT_898_112_2_17_23_12_48_3046,"[RFTEST]:dtl_CloseSeine")
TRACE_MSG(DTL_RFVT_908_112_2_17_23_12_48_3047,"[RFTEST]:dtl_CloseSeine already in poweroff mode, return")
TRACE_MSG(DTL_RFVT_929_112_2_17_23_12_48_3048,"[RFTEST]:rftest_CallBack ack_id =0x%x,p0 =0x%x, p1 =0x%x\n ")
TRACE_MSG(DTL_RFVT_933_112_2_17_23_12_48_3049,"[RFTEST]:received MTV_STIMI_SEARCH_PROG_END_CNF msg tick=%d")
TRACE_MSG(DTL_RFVT_938_112_2_17_23_12_48_3050,"[RFTEST]: CMMB_GetNetworkInfoList net num=%d.")
TRACE_MSG(DTL_RFVT_950_112_2_17_23_12_48_3051,"[RFTEST]: CMMB_GetNetworkInfoList service num=%d.")
TRACE_MSG(DTL_RFVT_955_112_2_17_23_12_49_3052,"[RFTEST]: service = %d  is free:%d")
TRACE_MSG(DTL_RFVT_969_112_2_17_23_12_49_3053,"[RFTEST]: CMMB_PlayNetwork %d %d")
TRACE_MSG(DTL_RFVT_972_112_2_17_23_12_49_3054,"[RFTEST]: CMMB_PlayNetwork success")
TRACE_MSG(DTL_RFVT_982_112_2_17_23_12_49_3055,"[RFTEST]: CMMB_GetNetworkInfoList failed")
TRACE_MSG(DTL_RFVT_989_112_2_17_23_12_49_3056,"[RFTEST]: received MTV_STIMI_SEARCH_PROG_ERR_IND msg")
TRACE_MSG(DTL_RFVT_1007_112_2_17_23_12_49_3057,"[RFTEST]: received CMMB_PlayNetwork msg ack_id=%d, tick=%d")
TRACE_MSG(DTL_RFVT_1011_112_2_17_23_12_49_3058,"[RFTEST]: received MTV_STOP_ACK msg")
TRACE_MSG(DTL_RFVT_1039_112_2_17_23_12_49_3059,"[RFTEST]: rftest_GetChannelStatus sync_stat=%d org BER=%d")
TRACE_MSG(DTL_RFVT_1048_112_2_17_23_12_49_3060,"[RFTEST]:rftest_GetChannelStatus NOT LOCK")
TRACE_MSG(DTL_RFVT_1053_112_2_17_23_12_49_3061,"[RFTEST]:rftest_GetChannelStatus wrong state")
TRACE_MSG(DTL_RFVT_1058_112_2_17_23_12_49_3062,"[RFTEST]:rftest_GetChannelStatus parama error")
TRACE_MSG(DTL_RFVT_1072_112_2_17_23_12_49_3063,"[RFTEST]:rftest_GetRssiChannelStatus state=%d")
TRACE_MSG(DTL_RFVT_1080_112_2_17_23_12_49_3064,"[RFTEST]: rftest_GetRssiChannelStatus ret=%d")
TRACE_MSG(DTL_RFVT_1084_112_2_17_23_12_49_3065,"[RFTEST]:rftest_GetRssiChannelStatus wrong state")
TRACE_MSG(DTL_RFVT_1089_112_2_17_23_12_49_3066,"[RFTEST]:rftest_GetRssiChannelStatus parama error")
TRACE_MSG(DTL_RFVT_1103_112_2_17_23_12_49_3067,"dtl_InitSeineCallback: receive init finish msg")
TRACE_MSG(DTL_RFVT_1122_112_2_17_23_12_49_3068,"dtl_SleepSeine current mode = %d")
TRACE_MSG(DTL_RFVT_1125_112_2_17_23_12_49_3069,"dtl_SleepSeine need init seine")
TRACE_MSG(DTL_RFVT_1128_112_2_17_23_12_49_3070,"dtl_SleepSeine send sleep CMD")
TRACE_MSG(DTL_RFVT_1130_112_2_17_23_12_49_3071,"dtl_SleepSeine start waiting")
TRACE_MSG(DTL_RFVT_1133_112_2_17_23_12_49_3072,"dtl_SleepSeine end waiting")
TRACE_MSG(DTL_RFVT_1187_112_2_17_23_12_49_3073,"[RFTEST]:dtl_DvtSendSignal subtype = %d msg = %d dest id = %d")
TRACE_MSG(DTL_RFVT_1219_112_2_17_23_12_49_3074,"[RFTEST]:dtl_handleSearchEndCnf")
TRACE_MSG(DTL_RFVT_1222_112_2_17_23_12_49_3075,"[RFTEST]:dtl_handleSearchEndCnf net_number = %d.")
TRACE_MSG(DTL_RFVT_1235_112_2_17_23_12_49_3076,"[RFTEST]:dtl_handleSearchEndCnf service number = %d")
TRACE_MSG(DTL_RFVT_1256_112_2_17_23_12_49_3077,"[RFTEST]:dtl_handleSearchEndCnf no service info")
TRACE_MSG(DTL_RFVT_1261_112_2_17_23_12_49_3078,"[RFTEST]:dtl_handleSearchEndCnf Done!!")
TRACE_MSG(DTL_RFVT_1267_112_2_17_23_12_49_3079,"[RFTEST]:dtl_handleSearchEndCnf no network info")
TRACE_MSG(DTL_RFVT_1281_112_2_17_23_12_49_3080,"[RFTEST]:DTLRFVT_HandleSearchNetwork mem1 = %x mem2 = %x")
TRACE_MSG(DTL_RFVT_1285_112_2_17_23_12_49_3081,"[RFTEST]:DTLRFVT_HandleSearchNetwork run in cal mode")
TRACE_MSG(DTL_RFVT_1311_112_2_17_23_12_49_3082,"dtl_StartSearchNetwork mem1 = %x, mem2 = %x")
TRACE_MSG(DTL_RFVT_1314_112_2_17_23_12_49_3083,"dtl_StartSearchNetwork need to poweroff seine")
TRACE_MSG(DTL_RFVT_1318_112_2_17_23_12_49_3084,"dtl_StartSearchNetwork init")
TRACE_MSG(DTL_RFVT_1340_112_2_17_23_12_49_3085,"dtl_StartSearchNetwork start search  task = %d")
TRACE_MSG(DTL_RFVT_1346_112_2_17_23_12_49_3086,"dtl_StartSearchNetwork search fail")
TRACE_MSG(DTL_RFVT_1366_112_2_17_23_12_49_3087,"dtl_exit_tv")
TRACE_MSG(DTL_RFVT_1396_112_2_17_23_12_49_3088,"[RFTEST]:DTLRFVT_HandlePlayByParam para 1 = %d 2 = %d 3 = %d mem1 = %x mem2 = %x")
TRACE_MSG(DTL_RFVT_1402_112_2_17_23_12_49_3089,"[RFTEST]:DTLRFVT_HandlePlayByParam run in cal mode")
TRACE_MSG(DTL_RFVT_1412_112_2_17_23_12_49_3090,"[RFTEST]:DTLRFVT_HandlePlayByParam run in normal mode")
TRACE_MSG(DTL_RFVT_1439_112_2_17_23_12_49_3091,"DTLRFVT_PlayNetWorkByParam mem1 = %x, mem2 = %x")
TRACE_MSG(DTL_RFVT_1442_112_2_17_23_12_49_3092,"DTLRFVT_PlayNetWorkByParam already in play mode, return")
TRACE_MSG(DTL_RFVT_1447_112_2_17_23_12_50_3093,"DTLRFVT_PlayNetWorkByParam need to poweroff seine")
TRACE_MSG(DTL_RFVT_1452_112_2_17_23_12_50_3094,"DTLRFVT_PlayNetWorkByParam init_ret = %d")
TRACE_MSG(DTL_RFVT_1468_112_2_17_23_12_50_3095,"DTLRFVT_PlayNetWorkByParam net_id = %d service id = %d")
TRACE_MSG(DTL_RFVT_1470_112_2_17_23_12_50_3096,"DTLRFVT_PlayNetWorkByParam play init")
TRACE_MSG(DTL_RFVT_1493_112_2_17_23_12_50_3097,"DTLRFVT_PlayNetWorkByParam get param error")
TRACE_MSG(DTL_RFVT_1499_112_2_17_23_12_50_3098,"DTLRFVT_PlayNetWorkByParam start play")
TRACE_MSG(DTL_RFVT_1503_112_2_17_23_12_50_3099,"DTLRFVT_PlayNetWorkByParam play state = %d")
TRACE_MSG(DTL_RFVT_1508_112_2_17_23_12_50_3100,"DTLRFVT_PlayNetWorkByParam time out")
TRACE_MSG(DTL_RFVT_1533_112_2_17_23_12_50_3101,"DTLRFVT_StopPlayNetWorkByParam stop")
TRACE_MSG(DTL_RFVT_1536_112_2_17_23_12_50_3102,"DTLRFVT_StopPlayNetWorkByParam stop state = %d")
TRACE_MSG(DTL_RFVT_1543_112_2_17_23_12_50_3103,"DTLRFVT_StopPlayNetWorkByParam not in play mode, return")
TRACE_MSG(DTL_RFVT_1596_112_2_17_23_12_50_3104,"[RFTEST]:dvt_ThreadEntry sig_ptr is NULL!!")
TRACE_MSG(DTL_RFVT_1600_112_2_17_23_12_50_3105,"[RFTEST]:dvt_ThreadEntry signalcode = %d")
TRACE_MSG(DTL_RFVT_1604_112_2_17_23_12_50_3106,"[RFTEST]:dvt_ThreadEntry subtype = %d")
TRACE_MSG(DTL_RFVT_1625_112_2_17_23_12_50_3107,"[RFTEST]:dvt_ThreadEntry receive MTV_PLAY_ACK")
TRACE_MSG(DTL_RFVT_1632_112_2_17_23_12_50_3108,"[RFTEST]:dvt_ThreadEntry receive MTV_PLAY_ERROR")
TRACE_MSG(DTL_RFVT_1639_112_2_17_23_12_50_3109,"[RFTEST]:dvt_ThreadEntry receive MTV_STOP_ACK")
TRACE_MSG(DTL_RFVT_1643_112_2_17_23_12_50_3110,"[RFTEST]:dvt_ThreadEntry receive MTV_STOP_ERROR")
TRACE_MSG(DTL_RFVT_1648_112_2_17_23_12_50_3111,"[RFTEST]: dvt_ThreadEntry play success start sleep")
TRACE_MSG(DTL_RFVT_1651_112_2_17_23_12_50_3112,"[RFTEST]: dvt_ThreadEntry play success sleep end")
TRACE_MSG(DTL_RFVT_1654_112_2_17_23_12_50_3113,"[RFTEST]:dvt_ThreadEntry receive MTV_STIMI_SEARCH_PROG_ERR_IND")
TRACE_MSG(DTL_RFVT_1685_112_2_17_23_12_50_3114,"[RFTEST]:DTLRFVT_ThreadInit task id = %d")
TRACE_MSG(DTL_RFVT_1719_112_2_17_23_12_50_3115,"[RFTEST]:rftest_GetCMMBSN enter")
TRACE_MSG(DTL_RFVT_1731_112_2_17_23_12_50_3116,"[RFTEST]:rftest_GetCMMBSN = %s")
TRACE_MSG(DTL_RFVT_1735_112_2_17_23_12_50_3117,"[RFTEST]:rftest_GetCMMBSN fail!!!")
TRACE_MSG(DTL_RFVT_1740_112_2_17_23_12_50_3118,"[RFTEST]: rftest_GetCMMBSN MUACMD_Open fail!!!")
TRACE_MSG(DTL_RFVT_1746_112_2_17_23_12_50_3119,"[RFTEST]:rftest_GetCMMBSN wrong state")
TRACE_MSG(DTL_RFVT_1765_112_2_17_23_12_50_3120,"[RFTEST]: rftest_CheckSMD ret=%d")
TRACE_MSG(DTL_RFVT_1769_112_2_17_23_12_50_3121,"[RFTEST]:rftest_CheckSMD wrong state")
TRACE_MSG(DTL_RFVT_1781_112_2_17_23_12_50_3122,"[RFTEST]: rftest_CheckSMD ret=%d")
TRACE_MSG(DTL_RFVT_1805_112_2_17_23_12_50_3123,"dtl_CalcRssiOffset freq = %d")
TRACE_MSG(DTL_RFVT_1809_112_2_17_23_12_50_3124,"dtl_CalcRssiOffset index = %d")
TRACE_MSG(MTV_COMMON_FUNC_65_112_2_17_23_12_52_3125,"MTV_CombineFullPath param error")
TRACE_MSG(MTV_COMMON_FUNC_130_112_2_17_23_12_52_3126,"[SG]: MTV_CombineFullPath, 0x%x,0x%x")
TRACE_MSG(MTV_COMMON_FUNC_149_112_2_17_23_12_52_3127,"MTV_IsFolderExist param error")
TRACE_MSG(MTV_SETTING_151_112_2_17_23_12_54_3128,"MTV_SetCommConfig V log open")
TRACE_MSG(MTV_SETTING_156_112_2_17_23_12_54_3129,"MTV_SetCommConfig V log close")
TRACE_MSG(MTV_SETTING_162_112_2_17_23_12_54_3130,"MTV_SetCommConfig V log to BB open")
TRACE_MSG(MTV_SETTING_167_112_2_17_23_12_54_3131,"MTV_SetCommConfig V log to BB close")
TRACE_MSG(MTV_SETTING_195_112_2_17_23_12_54_3132,"[MTVLOG]MTV_CreateLogfolder ret=%d")
TRACE_MSG(MTV_SETTING_210_112_2_17_23_12_54_3133,"[MTVLOG]MTV_CreateLogFile !")
TRACE_MSG(MTV_SETTING_213_112_2_17_23_12_54_3134,"[MTVLOG]MTV_CreateLogFile arm handle=%x")
TRACE_MSG(TV_FIRMWARE_291_112_2_17_23_12_55_3135,"Parse_FW_Head:%x:%x,%x")
TRACE_MSG(TV_FIRMWARE_337_112_2_17_23_12_55_3136,"GZIP_FWID: %d zip_data  0x%x  :%d")
TRACE_MSG(TV_FIRMWARE_344_112_2_17_23_12_55_3137,"GZIP_FWID: %d offset  0x%x  :%d")
TRACE_MSG(TV_FIRMWARE_347_112_2_17_23_12_55_3138,"GZIP_FW: 0x%x  :%d")
TRACE_MSG(TV_FIRMWARE_351_112_2_17_23_12_55_3139,"TVBOOT_TVOpen()download_gzip CODE_IN_SDRAM error!\n")
TRACE_MSG(TV_FIRMWARE_379_112_2_17_23_12_55_3140,"NORMAL_FWID %d  0x%x  :%d")
TRACE_MSG(TV_FIRMWARE_383_112_2_17_23_12_55_3141,"TVBOOT_TVOpen() normal download %d  error!\n")
TRACE_MSG(MTV_LOG_279_112_2_17_23_13_2_3142,"[MTVLOG]mtvlog_SetLogFlag type=%d")
TRACE_MSG(MTV_LOG_292_112_2_17_23_13_2_3143,"[MTVLOG]mtvlog_SetLogFlag wrong type")
TRACE_MSG(MTV_LOG_339_112_2_17_23_13_2_3144,"mtvlog_DSPTrace src_len = %d")
TRACE_MSG(MTV_LOG_351_112_2_17_23_13_2_3145,"mtvlog_DSPTrace src_len = %d send_len = %d, offset = %d")
TRACE_MSG(MTV_LOG_353_112_2_17_23_13_2_3146,"mtvlog_DSPTrace status = %d")
TRACE_MSG(MTV_LOG_372_112_2_17_23_13_2_3147," data_ptr[%03d] = %02x %02x %02x %02x %02x %02x %02x %02x")
TRACE_MSG(MTV_LOG_392_112_2_17_23_13_2_3148,"[MTVLOG]mtvlog_ProcessData data_ptr = %x, data_len = %d,log_type = %d")
TRACE_MSG(MTV_LOG_415_112_2_17_23_13_2_3149,"mtvlog_ProcessData ERROR!! log.log_length = %d, offset = %d")
TRACE_MSG(MTV_LOG_440_112_2_17_23_13_2_3150,"[MTVLOG]mtvlog_ProcessData Wrong log_type!!")
TRACE_MSG(MTV_LOG_474_112_2_17_23_13_2_3151,"[MTVLOG]mtvlog_LogARMMsg Do nothing!!")
TRACE_MSG(MTV_LOG_639_112_2_17_23_13_2_3152,"mtvlog_HandleARM \n")
TRACE_MSG(MTV_LOG_643_112_2_17_23_13_2_3153,"mtvlog_HandleARM read_pos = %d, write_pos = %d ")
TRACE_MSG(MTV_LOG_655_112_2_17_23_13_2_3154,"mtvlog_HandleARM data_length error!!!")
TRACE_MSG(MTV_LOG_661_112_2_17_23_13_2_3155,"mtvlog_HandleARM read pos == write pos")
TRACE_MSG(MTV_LOG_678_112_2_17_23_13_2_3156,"mtvlog_HandleARM data_ptr = %x, data_length = %d")
TRACE_MSG(MTV_LOG_685_112_2_17_23_13_2_3157,"mtvlog_HandleARM start mtvlog_ReadLcmData")
TRACE_MSG(MTV_LOG_692_112_2_17_23_13_2_3158,"mtvlog_HandleARM 1st buf adr = %x, len = %d, src adr = %x, 2nd buf adr = %x 2nd len = %d, src adr = %x")
TRACE_MSG(MTV_LOG_699_112_2_17_23_13_2_3159,"mtvlog_HandleARM: LCM read time = %d length = %d")
TRACE_MSG(MTV_LOG_722_112_2_17_23_13_2_3160,"mtvlog_HandleDsp")
TRACE_MSG(MTV_LOG_726_112_2_17_23_13_2_3161,"mtvlog_HandleDsp s_read_pos = %x, org_write_pos = %x ")
TRACE_MSG(MTV_LOG_729_112_2_17_23_13_2_3162,"mtvlog_HandleDsp write_pos = %x ")
TRACE_MSG(MTV_LOG_742_112_2_17_23_13_2_3163,"mtvlog_HandleDsp read pos == write pos")
TRACE_MSG(MTV_LOG_746_112_2_17_23_13_2_3164,"mtvlog_HandleDsp data_length = %d")
TRACE_MSG(MTV_LOG_765_112_2_17_23_13_3_3165,"mtvlog_HandleDsp: LCM read time = %d length = %d speed = %d Kb/s")
TRACE_MSG(MTV_LOG_840_112_2_17_23_13_3_3166,"[MTVLOG]mtvlog_CloseLogFile arm=%x")
TRACE_MSG(MTV_LOG_874_112_2_17_23_13_3_3167,"[MTVLOG]mvtlog_ThreadEntry exit")
TRACE_MSG(MTV_LOG_881_112_2_17_23_13_3_3168,"[MTVLOG]mvtlog_ThreadEntry Unknow signal id!")
TRACE_MSG(MTV_LOG_899_112_2_17_23_13_3_3169,"[MTVLOG]mvtlog_ThreadEntry thread exit")
TRACE_MSG(MTV_LOG_911_112_2_17_23_13_3_3170,"[MTVLOG]MTVLOG_Init arm log = %d")
TRACE_MSG(MTV_LOG_919_112_2_17_23_13_3_3171,"[MTVLOG]MTVLOG_Init create file fail!")
TRACE_MSG(MTV_LOG_934_112_2_17_23_13_3_3172,"[MTVLOG]MTVLOG_Init create task=%d")
TRACE_MSG(MTV_LOG_950_112_2_17_23_13_3_3173,"[MTVLOG]MTVLOG_Init done!")
TRACE_MSG(MTV_LOG_959_112_2_17_23_13_3_3174,"[MTVLOG]MTVLOG_Exit !\n")
TRACE_MSG(MTV_LOG_999_112_2_17_23_13_3_3175,"MOBILETVLOG_SetEnableMobileTvArmLogFlag is_enable_arm_log %d\n")
TRACE_MSG(MTV_LOG_1023_112_2_17_23_13_3_3176,"MOBILETVLOG_SetEnableMobileTvDspLogFlag is_enable_dsp_log %d\n")
TRACE_MSG(MTV_LOG_1059_112_2_17_23_13_3_3177,"test_DSPTrace  status = %d")
TRACE_MSG(MTV_LOG_1096_112_2_17_23_13_3_3178,"into test_func s_dsp_total_len = %d")
TRACE_MSG(MTV_SERVICE_415_112_2_17_23_13_4_3179,"[MBBMS Service] MTVSERVICE_MBMSInit Entering... \n")
TRACE_MSG(MTV_SERVICE_445_112_2_17_23_13_4_3180,"[mbbms service]MTVSERVICE_Init %d\n ")
TRACE_MSG(MTV_SERVICE_479_112_2_17_23_13_4_3181,"[MBBMS Service] Thread Created Ok \n")
TRACE_MSG(MTV_SERVICE_484_112_2_17_23_13_4_3182,"[MBBMS Service] MTVSERVICE_Init  no mem\n ")
TRACE_MSG(MTV_SERVICE_493_112_2_17_23_13_4_3183,"mbbms MTVSERVICE_Init CMMB init FAILED \n")
TRACE_MSG(MTV_SERVICE_513_112_2_17_23_13_4_3184,"[MBBMS Service] MTVSERVICE_MBMSInit CMMB MODE Over \n")
TRACE_MSG(MTV_SERVICE_523_112_2_17_23_13_4_3185,"[MBBMS Service] MTVSERVICE_MBMSInit MBBMS Init FAILED \n")
TRACE_MSG(MTV_SERVICE_535_112_2_17_23_13_4_3186,"[MBBMS Service] MTVSERVICE_Init  OK Leaving... \n")
TRACE_MSG(MTV_SERVICE_546_112_2_17_23_13_4_3187,"[mbbms service]MTVSERVICE_MBMSInit entering... \n")
TRACE_MSG(MTV_SERVICE_560_112_2_17_23_13_4_3188,"[MBBMS Service] MTVSERVICE_MBMSInit MBBMS_GBA_Init Failed... \n")
TRACE_MSG(MTV_SERVICE_567_112_2_17_23_13_4_3189,"[MBBMS Service] GBA Inited Ok \n")
TRACE_MSG(MTV_SERVICE_576_112_2_17_23_13_4_3190,"[MBBMS Service] MTVSERVICE_MBMSInit MBBMS_Sub_MSK_Init Failed... \n")
TRACE_MSG(MTV_SERVICE_580_112_2_17_23_13_4_3191,"[MBBMS Service] Sub Msk Inited Ok \n")
TRACE_MSG(MTV_SERVICE_582_112_2_17_23_13_4_3192,"[MBBMS Service] SG Inited Ok \n")
TRACE_MSG(MTV_SERVICE_607_112_2_17_23_13_5_3193,"[MBBMS Service] MTVSERVICE_MBMSInit fail to  MBBMS_Load_UpdateTable\n")
TRACE_MSG(MTV_SERVICE_610_112_2_17_23_13_5_3194,"[MBBMS Service] MTVSERVICE_MBMSInit MBBMS MODE OVER Leaving... \n")
TRACE_MSG(MTV_SERVICE_614_112_2_17_23_13_5_3195,"[MBBMS Service] MTVSERVICE_MBMSInit MBBMS Mode Failed... \n")
TRACE_MSG(MTV_SERVICE_627_112_2_17_23_13_5_3196,"[mbbms service]MTVSERVICE_ChangeMode %d entering \n")
TRACE_MSG(MTV_SERVICE_644_112_2_17_23_13_5_3197,"[mbbms service]MTVSERVICE_ChangeMode MBMS init FAILED\n")
TRACE_MSG(MTV_SERVICE_658_112_2_17_23_13_5_3198,"[mbbms service]MTVSERVICE_ChangeMode over \n")
TRACE_MSG(MTV_SERVICE_665_112_2_17_23_13_5_3199,"[mbbms service]MTVSERVICE_MBMSExit Entering ...\n")
TRACE_MSG(MTV_SERVICE_670_112_2_17_23_13_5_3200,"MTVSERVICE_MBMSExit MTVSERVICE_DeactivePDP failed !! \n")
TRACE_MSG(MTV_SERVICE_681_112_2_17_23_13_5_3201,"[mbbms service]MTVSERVICE_MBMSExit: sub type:%d play type:%d gba type:%d sgtype:%dn")
TRACE_MSG(MTV_SERVICE_682_112_2_17_23_13_5_3202,"[mbbms service]MTVSERVICE_MBMSExit: some task is running; exit FAIL \n")
TRACE_MSG(MTV_SERVICE_696_112_2_17_23_13_5_3203,"[mbbms service]MTVSERVICE_MBMSExit starting ...\n")
TRACE_MSG(MTV_SERVICE_701_112_2_17_23_13_5_3204,"[mbbms service]MTVSERVICE_MBMSExit: gba queue is not EMPTY \n")
TRACE_MSG(MTV_SERVICE_706_112_2_17_23_13_5_3205,"[mbbms service]MTVSERVICE_MBMSExit: gba queue type:%d sub:%d running:%d n")
TRACE_MSG(MTV_SERVICE_716_112_2_17_23_13_5_3206,"[mbbms service]MTVSERVICE_MBMSExit: sub queue is not EMPTY \n")
TRACE_MSG(MTV_SERVICE_721_112_2_17_23_13_5_3207,"[mbbms service]MTVSERVICE_MBMSExit: sub queue type:%d sub:%d running:%d n")
TRACE_MSG(MTV_SERVICE_732_112_2_17_23_13_5_3208,"[mbbms service]MTVSERVICE_MBMSExit: pdp queue is not EMPTY \n")
TRACE_MSG(MTV_SERVICE_737_112_2_17_23_13_5_3209,"[mbbms service]MTVSERVICE_MBMSExit: pdp queue type:%d sub:%d running:%d n")
TRACE_MSG(MTV_SERVICE_762_112_2_17_23_13_5_3210,"[mbbms service]MTVSERVICE_MBMSExit leaving ...\n")
TRACE_MSG(MTV_SERVICE_790_112_2_17_23_13_5_3211,"[mbbms service ]MTVSERVICE_ServiceRequest starting... \n")
TRACE_MSG(MTV_SERVICE_807_112_2_17_23_13_5_3212,"[mbbms service ]MTVSERVICE_ServiceRequest Over... \n")
TRACE_MSG(MTV_SERVICE_814_112_2_17_23_13_5_3213,"[mbbms service ]MTVSERVICE_MBMSStopServiceRequest starting... \n")
TRACE_MSG(MTV_SERVICE_833_112_2_17_23_13_5_3214,"[mbbms service ]MTVSERVICE_MBMSStopServiceRequest over... \n")
TRACE_MSG(MTV_SERVICE_839_112_2_17_23_13_5_3215,"[mbbms service ]MTVSERVICE_UnsubRequest starting... \n")
TRACE_MSG(MTV_SERVICE_859_112_2_17_23_13_5_3216,"[mbbms service ]MTVSERVICE_UnsubRequest over... \n")
TRACE_MSG(MTV_SERVICE_866_112_2_17_23_13_5_3217,"[mbbms service ]MTVSERVICE_MBMSStopUnsubRequest starting... \n")
TRACE_MSG(MTV_SERVICE_885_112_2_17_23_13_5_3218,"[mbbms service ]MTVSERVICE_MBMSStopUnsubRequest over... \n")
TRACE_MSG(MTV_SERVICE_891_112_2_17_23_13_5_3219,"[mbbms service ]MTVSERVICE_AccountQuiry starting... \n")
TRACE_MSG(MTV_SERVICE_910_112_2_17_23_13_5_3220,"[mbbms service ]MTVSERVICE_AccountQuiry over... \n")
TRACE_MSG(MTV_SERVICE_916_112_2_17_23_13_5_3221,"[mbbms service ]MTVSERVICE_MBMSStopAccountQuiry starting... \n")
TRACE_MSG(MTV_SERVICE_935_112_2_17_23_13_5_3222,"[mbbms service ]MTVSERVICE_MBMSStopAccountQuiry over... \n")
TRACE_MSG(MTV_SERVICE_941_112_2_17_23_13_5_3223,"[mbbms service ]MTVSERVICE_SubUpdate starting... \n")
TRACE_MSG(MTV_SERVICE_943_112_2_17_23_13_5_3224,"[mbbms service ]MTVSERVICE_SubUpdate CHECKing INIT \n")
TRACE_MSG(MTV_SERVICE_948_112_2_17_23_13_5_3225,"[mbbms service ]MTVSERVICE_SubUpdate CHECKing IDLE \n")
TRACE_MSG(MTV_SERVICE_951_112_2_17_23_13_5_3226,"[mbbms service ]MTVSERVICE_SubUpdate GET MUTEX \n")
TRACE_MSG(MTV_SERVICE_965_112_2_17_23_13_5_3227,"[mbbms service ]MTVSERVICE_SubUpdate over... \n")
TRACE_MSG(MTV_SERVICE_971_112_2_17_23_13_5_3228,"[mbbms service ]MTVSERVICE_MBMSStopSubUpdate starting... \n")
TRACE_MSG(MTV_SERVICE_991_112_2_17_23_13_5_3229,"[mbbms service ]MTVSERVICE_MBMSStopSubUpdate over... \n")
TRACE_MSG(MTV_SERVICE_998_112_2_17_23_13_5_3230,"[mbbms service ]MTVSERVICE_StartGBA starting... \n")
TRACE_MSG(MTV_SERVICE_1020_112_2_17_23_13_5_3231,"[mbbms service ]MTVSERVICE_StartGBA over... \n")
TRACE_MSG(MTV_SERVICE_1026_112_2_17_23_13_5_3232,"[mbbms service ]MTVSERVICE_MBMSStopGBA starting... \n")
TRACE_MSG(MTV_SERVICE_1045_112_2_17_23_13_5_3233,"[mbbms service ]MTVSERVICE_MBMSStopGBA over... \n")
TRACE_MSG(MTV_SERVICE_1065_112_2_17_23_13_5_3234,"mbbms mbms_service_GBA_callback entering..%d %d..\n")
TRACE_MSG(MTV_SERVICE_1083_112_2_17_23_13_5_3235,"mbbms mbms_service_GBA_callback send stop for type%d \n")
TRACE_MSG(MTV_SERVICE_1124_112_2_17_23_13_6_3236,"[mbbms service]mbms_service_gba_callback stop and restart gba FAIL \n")
TRACE_MSG(MTV_SERVICE_1132_112_2_17_23_13_6_3237,"[mbbms service]mbms_service_gba_callback stop and restart gba OK \n")
TRACE_MSG(MTV_SERVICE_1162_112_2_17_23_13_6_3238,"mbbms mbms_service_GBA_callback Free ptr for GBA removed \n")
TRACE_MSG(MTV_SERVICE_1179_112_2_17_23_13_6_3239,"[mbbms service]mbms_service_gba_callback send signal type:%d result:%d n")
TRACE_MSG(MTV_SERVICE_1200_112_2_17_23_13_6_3240,"[mbbms service]mbms_service_gba_callback set next and free current gba \n")
TRACE_MSG(MTV_SERVICE_1215_112_2_17_23_13_6_3241,"mbbms mbms_service_GBA_callback END..%d %d..\n")
TRACE_MSG(MTV_SERVICE_1232_112_2_17_23_13_6_3242,"mbbms mbms_inner_sub_callback_send entering subtype:%d result:%d... \n")
TRACE_MSG(MTV_SERVICE_1236_112_2_17_23_13_6_3243,"mbbms mbms_inner_sub_callback_send sub register queue is EMPTY!! \n")
TRACE_MSG(MTV_SERVICE_1242_112_2_17_23_13_6_3244,"mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_ACCOUNT_QUIRY not found-freeptr \n")
TRACE_MSG(MTV_SERVICE_1247_112_2_17_23_13_6_3245,"mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_SUBSCRIBE not found-freeptr \n")
TRACE_MSG(MTV_SERVICE_1252_112_2_17_23_13_6_3246,"mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_UPDATE_TABLE not found-freeptr \n")
TRACE_MSG(MTV_SERVICE_1256_112_2_17_23_13_6_3247,"mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_UNSUBSCRIBE not found-freeptr \n")
TRACE_MSG(MTV_SERVICE_1260_112_2_17_23_13_6_3248,"mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_GETMSK not found-freeptr \n")
TRACE_MSG(MTV_SERVICE_1290_112_2_17_23_13_6_3249,"[mbbms service]mbms_inner_sub_callback_send FREE type:%d subtype:%dn")
TRACE_MSG(MTV_SERVICE_1300_112_2_17_23_13_6_3250,"[mbbms service]mbms_inner_sub_callback_send find type:%d subtype:%dn")
TRACE_MSG(MTV_SERVICE_1310_112_2_17_23_13_6_3251,"mbbms mbms_inner_sub_callback_send OK.  NO more task in QUEUE... \n")
TRACE_MSG(MTV_SERVICE_1313_112_2_17_23_13_6_3252,"mbbms mbms_inner_sub_callback_send. More Taks is waiting in QUEUE\n")
TRACE_MSG(MTV_SERVICE_1321_112_2_17_23_13_6_3253,"[mbbms service]mbms_inner_sub_callback_send TRY run type:%d subtype:%dn")
TRACE_MSG(MTV_SERVICE_1328_112_2_17_23_13_6_3254,"mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_ACCOUNT_QUIRY STARTING \n")
TRACE_MSG(MTV_SERVICE_1334_112_2_17_23_13_6_3255,"mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_SUBSCRIBE STARTING \n")
TRACE_MSG(MTV_SERVICE_1340_112_2_17_23_13_6_3256,"mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_UPDATE_TABLE STARTING \n")
TRACE_MSG(MTV_SERVICE_1345_112_2_17_23_13_6_3257,"mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_UNSUBSCRIBE STARTING \n")
TRACE_MSG(MTV_SERVICE_1350_112_2_17_23_13_6_3258,"mbbms mbms_inner_sub_callback_send.MBBMS_SERVICE_GETMSK STARTING \n")
TRACE_MSG(MTV_SERVICE_1361_112_2_17_23_13_6_3259,"[MBBMS Service] mbms_inner_sub_callback_send run OK type:%d subtype:%d \n")
TRACE_MSG(MTV_SERVICE_1368_112_2_17_23_13_6_3260,"[MBBMS Service] mbms_inner_sub_callback_send start fail and remove type:%d subtype:%d \n")
TRACE_MSG(MTV_SERVICE_1388_112_2_17_23_13_6_3261,"mbbms mbms_service_sub_callback entering..%d %d..\n")
TRACE_MSG(MTV_SERVICE_1579_112_2_17_23_13_7_3262,"[MBBMS Service] mbms_service_pdp_callback return NULL!!! \n")
TRACE_MSG(MTV_SERVICE_1582_112_2_17_23_13_7_3263,"[MBBMS Service] mbms_service_pdp_callback running.... \n")
TRACE_MSG(MTV_SERVICE_1587_112_2_17_23_13_7_3264,"mbms_service_pdp_callback:Get MBMSPDP_ACTIVE_CNF msg  MMIMBBMS_TickCount=%d")
TRACE_MSG(MTV_SERVICE_1597_112_2_17_23_13_7_3265,"[MBBMS Service] mbms_service_pdp_callback Ok type:%d\n")
TRACE_MSG(MTV_SERVICE_1614_112_2_17_23_13_7_3266,"[MBBMS Service] mbms_service_pdp_callback FAIL type:%d\n")
TRACE_MSG(MTV_SERVICE_1625_112_2_17_23_13_7_3267,"mbms_service_pdp_callback:Get MBMSPDP_ACTIVE_CNF msg ERROR!\n")
TRACE_MSG(MTV_SERVICE_1631_112_2_17_23_13_7_3268,"mbms_service_pdp_callback:Get MBMSPDP_DEACTIVE_CNF Msg  MMIMBBMS_TickCount=%d")
TRACE_MSG(MTV_SERVICE_1639_112_2_17_23_13_7_3269,"[MBBMS Service] mbms_service_pdp_callback DEACTIVE FAIL type:%d\n")
TRACE_MSG(MTV_SERVICE_1651_112_2_17_23_13_7_3270,"mbms_service_pdp_callback:Get MBMSPDP_DEACTIVE_IND Msg  MMIMBBMS_TickCount=%d")
TRACE_MSG(MTV_SERVICE_1675_112_2_17_23_13_7_3271,"[MBBMS Service] mtvservice_register_gba alloc FAILED \n")
TRACE_MSG(MTV_SERVICE_1685_112_2_17_23_13_7_3272,"[MBBMS Service] mtvservice_register_gba First GBA Task \n")
TRACE_MSG(MTV_SERVICE_1698_112_2_17_23_13_7_3273,"[MBBMS Service] mtvservice_register_gba GBA return FAIL \n")
TRACE_MSG(MTV_SERVICE_1706_112_2_17_23_13_7_3274,"[MBBMS Service] mtvservice_register_gba first GBA BUT Busy!!! \n")
TRACE_MSG(MTV_SERVICE_1714_112_2_17_23_13_7_3275,"[MBBMS Service] mtvservice_register_gba add to queue \n")
TRACE_MSG(MTV_SERVICE_1724_112_2_17_23_13_7_3276,"[MBBMS Service] mtvservice_register_gba OK type:%d \n")
TRACE_MSG(MTV_SERVICE_1744_112_2_17_23_13_7_3277,"[MBBMS Service] mtvservice_register_sub alloc FAILED \n")
TRACE_MSG(MTV_SERVICE_1757_112_2_17_23_13_7_3278,"[MBBMS Service]mtvservice_register_sub create queue type%d subtype:%dn")
TRACE_MSG(MTV_SERVICE_1764_112_2_17_23_13_7_3279,"[MBBMS Service]mtvservice_register_sub add queue type%d subtype:%dn")
TRACE_MSG(MTV_SERVICE_1805_112_2_17_23_13_7_3280,"[MBBMS Service] mtvservice_register_sub run OK type:%d subtype:%d \n")
TRACE_MSG(MTV_SERVICE_1816_112_2_17_23_13_7_3281,"[MBBMS Service] mtvservice_register_sub fail and remove type:%d subtype:%d \n")
TRACE_MSG(MTV_SERVICE_1824_112_2_17_23_13_7_3282,"[MBBMS Service] mtvservice_register_sub add ok type:%d subtype:%d \n")
TRACE_MSG(MTV_SERVICE_1832_112_2_17_23_13_7_3283,"[MBBMS Service] mtvservice_register_sub add ok type:%d subtype:%d \n")
TRACE_MSG(MTV_SERVICE_1871_112_2_17_23_13_7_3284,"mbbms mtvservice_unregister_gba entering.... \n")
TRACE_MSG(MTV_SERVICE_1884_112_2_17_23_13_7_3285,"[MBBMS Service] mtvservice_unregister_gba ONE OK type:%d \n")
TRACE_MSG(MTV_SERVICE_1891_112_2_17_23_13_7_3286,"[MBBMS Service] mtvservice_unregister_gba FREEING... \n")
TRACE_MSG(MTV_SERVICE_1893_112_2_17_23_13_7_3287,"[MBBMS Service] mtvservice_unregister_gba FREE OK \n")
TRACE_MSG(MTV_SERVICE_1906_112_2_17_23_13_7_3288,"[MBBMS Service] mtvservice_unregister_gba MORE OK type:%d \n")
TRACE_MSG(MTV_SERVICE_1911_112_2_17_23_13_7_3289,"[MBBMS Service] mtvservice_unregister_gba MORE FREEING... \n")
TRACE_MSG(MTV_SERVICE_1913_112_2_17_23_13_7_3290,"[MBBMS Service] mtvservice_unregister_gba MORE FREE OK \n")
TRACE_MSG(MTV_SERVICE_1925_112_2_17_23_13_7_3291,"[MBBMS Service] mtvservice_unregister_gba Stopping .... \n")
TRACE_MSG(MTV_SERVICE_1933_112_2_17_23_13_7_3292,"mbbms mtvservice_unregister_gba Over  .... \n")
TRACE_MSG(MTV_SERVICE_1942_112_2_17_23_13_7_3293,"mbbms mtvservice_unregister_sub entering .... \n")
TRACE_MSG(MTV_SERVICE_1958_112_2_17_23_13_7_3294,"[mbbms service] mtvservice_unregister_sub find type:%d subtype:%dn")
TRACE_MSG(MTV_SERVICE_1986_112_2_17_23_13_8_3295,"mbbms mtvservice_unregister_sub Over %d .... \n")
TRACE_MSG(MTV_SERVICE_2009_112_2_17_23_13_8_3296,"[mbbms service]mbms_service_deactivepdp_callback received \n")
TRACE_MSG(MTV_SERVICE_2010_112_2_17_23_13_8_3297,"[mbbms service] result:%d msg id:%d \n")
TRACE_MSG(MTV_SERVICE_2031_112_2_17_23_13_8_3298,"[MBBMS Service] MTVSERVICE_DeactivePDP send MMI Ok \n")
TRACE_MSG(MTV_SERVICE_2080_112_2_17_23_13_8_3299,"[MBBMS Service] MTVSERVICE_AddPDP send MMI Ok type:%d\n")
TRACE_MSG(MTV_SERVICE_2100_112_2_17_23_13_8_3300,"[MBBMS Service] MTVSERVICE_RemovePDP Ok type:%d\n")
TRACE_MSG(MTV_SERVICE_2137_112_2_17_23_13_8_3301,"[mbms] MBBMS_Service_Thread: task_id 0x%x ,signalcode 0x%x ,sig->type %d\n ")
TRACE_MSG(MTV_SERVICE_2145_112_2_17_23_13_8_3302,"[mbbms service]MBBMS_Service_Thread Exit Begin \n")
TRACE_MSG(MTV_SERVICE_2216_112_2_17_23_13_8_3303,"[mbbms service]MBBMS_Service_Thread Exit Over \n")
TRACE_MSG(MTV_SERVICE_2272_112_2_17_23_13_8_3304,"[mbbms service ] SetDownloadPicState state %d n")
TRACE_MSG(MTV_SERVICE_2287_112_2_17_23_13_8_3305,"[mbbms service ] GetDownloadPicState state %d n")
TRACE_MSG(MTV_SERVICE_2301_112_2_17_23_13_8_3306,"[mbbms service ] SetTabState state %d n")
TRACE_MSG(MTV_SERVICE_2316_112_2_17_23_13_8_3307,"[mbbms service ] GetTabState state %d n")
TRACE_MSG(MTV_SERVICE_2330_112_2_17_23_13_8_3308,"[mbbms service ] SetGBAState state %d n")
TRACE_MSG(MTV_SERVICE_2345_112_2_17_23_13_8_3309,"[mbbms service ] GetGBAState state %d n")
TRACE_MSG(MTV_SERVICE_2359_112_2_17_23_13_8_3310,"[mbbms service ] StopDownloadPicture type %d n")
TRACE_MSG(MTV_SERVICE_2383_112_2_17_23_13_8_3311,"[mbbms service ] HandleRcvSig mode %d ,type %d ,subtype %d \n")
TRACE_MSG(MTV_SERVICE_2432_112_2_17_23_13_8_3312,"[mbbms service ] HandleRcvSig err mode %d \n")
TRACE_MSG(MTV_SERVICE_2445_112_2_17_23_13_8_3313,"[mbbms service ] IsFinishProcess type %d\n")
TRACE_MSG(MTV_SERVICE_2479_112_2_17_23_13_8_3314,"[mbbms service ] FinishProcess type %d, rt %d \n")
TRACE_MSG(MTV_SERVICE_2500_112_2_17_23_13_9_3315,"[mbbms service ] HandleRcvStopSig type %d ,subtype %d,mode %d \n")
TRACE_MSG(MTV_SERVICE_2557_112_2_17_23_13_9_3316,"[mbbms service ] GetNAFAddr PNULL==naf_addr ||PNULL == naf_addr_len ||PNULL ==port ")
TRACE_MSG(MTV_SERVICE_2565_112_2_17_23_13_9_3317,"[mbbms service ] GetNAFAddr error_code =%d, service_num=%d\n")
TRACE_MSG(MTV_SERVICE_2579_112_2_17_23_13_9_3318,"GetNAFAddr PNULL ==service_index_ptr \n")
TRACE_MSG(MTV_SERVICE_2591_112_2_17_23_13_9_3319,"[mbbms service ] GetNAFAddr get access\n")
TRACE_MSG(MTV_SERVICE_2611_112_2_17_23_13_9_3320,"[mbbms service ] GetNAFAddr temp =%s\n")
TRACE_MSG(MTV_SERVICE_2633_112_2_17_23_13_9_3321,"GetNAFAddr doesn't find")
TRACE_MSG(MTV_SERVICE_2648_112_2_17_23_13_9_3322,"GetNAFAddr naf addr =%s\n")
TRACE_MSG(MTV_SERVICE_2707_112_2_17_23_13_9_3323,"[mbbms service ] UpdateTab type %d ,subtype %d \n")
TRACE_MSG(MTV_SERVICE_2779_112_2_17_23_13_9_3324,"[mbbms service ] HandleDownloadPictureSig \n")
TRACE_MSG(MTV_SERVICE_2812_112_2_17_23_13_9_3325,"[mbbms service ] GetSGSearchResult type %d \n")
TRACE_MSG(MTV_SERVICE_2851_112_2_17_23_13_9_3326,"[mbbms service ] GetSGSearchResult rt %d \n")
TRACE_MSG(MTV_SERVICE_2950_112_2_17_23_13_9_3327,"[mbbms service ] GetMBBMSSGResult type %d \n")
TRACE_MSG(MTV_SERVICE_2985_112_2_17_23_13_10_3328,"[mbbms service ] HandleSGParseSig err_code %d \n")
TRACE_MSG(MTV_SERVICE_3027_112_2_17_23_13_10_3329,"[mbbms service ] HandleSGParseSig err_code %d \n")
TRACE_MSG(MTV_SERVICE_3053_112_2_17_23_13_10_3330,"[mbbms service ] HandleSGParseSig type %d \n")
TRACE_MSG(MTV_SERVICE_3073_112_2_17_23_13_10_3331,"[mbbms service ] HandleSGRcvSig info.type %d \n")
TRACE_MSG(MTV_SERVICE_3148_112_2_17_23_13_10_3332,"[mbbms service ] HandleUpdateTabSig result %d \n")
TRACE_MSG(MTV_SERVICE_3156_112_2_17_23_13_10_3333,"[mbbms service ] HandleUpdateTabSig type %d ,tab_state %d\n")
TRACE_MSG(MTV_SERVICE_3173_112_2_17_23_13_10_3334,"[mbbms service ] HandleUpdateTabSig MTVSERVICE_RemoveRegister  rt %d\n")
TRACE_MSG(MTV_SERVICE_3197_112_2_17_23_13_10_3335,"[mbbms service ] HandleUpdateTabSig MTVSERVICE_AddRegister gba  \n")
TRACE_MSG(MTV_SERVICE_3207_112_2_17_23_13_10_3336,"[mbbms service ] HandleUpdateTabSig MTVSERVICE_AddRegister gba  err\n")
TRACE_MSG(MTV_SERVICE_3285_112_2_17_23_13_10_3337,"[mbbms service ] HandleUpdateTabSig state type  %d\n")
TRACE_MSG(MTV_SERVICE_3303_112_2_17_23_13_10_3338,"[mbbms service ] HandleGBASig result %d \n")
TRACE_MSG(MTV_SERVICE_3325_112_2_17_23_13_10_3339,"[mbbms service ] HandleGBASig MTVSERVICE_RemoveRegister gba  err\n")
TRACE_MSG(MTV_SERVICE_3372_112_2_17_23_13_10_3340,"[mbbms service ] HandleGBASig type %d \n")
TRACE_MSG(MTV_SERVICE_3434_112_2_17_23_13_10_3341,"[mbbms service ]HandleStopSGRcvSig type %d,subtype %d\n")
TRACE_MSG(MTV_SERVICE_3676_112_2_17_23_13_11_3342,"[mbbms service ] SetSGIfState type %d ,subtype %dn")
TRACE_MSG(MTV_SERVICE_3693_112_2_17_23_13_11_3343,"[mbbms service ] SetSGIfState finish ")
TRACE_MSG(MTV_SERVICE_3709_112_2_17_23_13_11_3344,"[mbbms service ] GetSGIfState act_type %d n")
TRACE_MSG(MTV_SERVICE_3734_112_2_17_23_13_11_3345,"[mbbms service ]MTVSERVICE_ActiveMBBMS starting\n")
TRACE_MSG(MTV_SERVICE_3745_112_2_17_23_13_11_3346,"[mbbms service ]MTVSERVICE_ActiveMBBMS para err !\n")
TRACE_MSG(MTV_SERVICE_3757_112_2_17_23_13_11_3347,"[mbbms service ]MTVSERVICE_ActiveMBBMS wrong state %d \n")
TRACE_MSG(MTV_SERVICE_3766_112_2_17_23_13_11_3348,"[mbbms service ]MTVSERVICE_ActiveMBBMS fail to active pdp \n")
TRACE_MSG(MTV_SERVICE_3773_112_2_17_23_13_11_3349,"[mbbms service ]MTVSERVICE_ActiveMBBMS over \n")
TRACE_MSG(MTV_SERVICE_3792_112_2_17_23_13_11_3350,"[mbbms service ]MTVSERVICE_DeactiveMBBMS  \n")
TRACE_MSG(MTV_SERVICE_3803_112_2_17_23_13_11_3351,"[mbbms service ]MTVSERVICE_DeactiveMBBMS  type %d, subtype %d\n")
TRACE_MSG(MTV_SERVICE_3857_112_2_17_23_13_11_3352,"[mbbms service ]MTVSERVICE_DeactiveMBBMS rt %d\n")
TRACE_MSG(MTV_SERVICE_3886_112_2_17_23_13_11_3353,"[mbbms service ]MTVSERVICE_ParseServiceGuide \n")
TRACE_MSG(MTV_SERVICE_3930_112_2_17_23_13_11_3354,"[mbbms service ]MTVSERVICE_ParseServiceGuide parse error \n")
TRACE_MSG(MTV_SERVICE_3961_112_2_17_23_13_11_3355,"[mbbms service ]MTVSERVICE_StopParseServiceGuide start \n")
TRACE_MSG(MTV_SERVICE_4004_112_2_17_23_13_12_3356,"[mbbms service ]IsServiceSubscribed get purchase result=%d\n")
TRACE_MSG(MTV_SERVICE_4010_112_2_17_23_13_12_3357,"[mbbms service ]IsServiceSubscribed purchase_itemlist_head_ptr=0x%x\n")
TRACE_MSG(MTV_SERVICE_4026_112_2_17_23_13_12_3358,"[mbbms service ]IsServiceSubscribed:%d")
TRACE_MSG(MTV_SERVICE_4037_112_2_17_23_13_12_3359,"[mbbms service ] IsServiceSubscribed:Get Purchase Item by sevice failded\n")
TRACE_MSG(MTV_SERVICE_4079_112_2_17_23_13_12_3360,"[SG]: Copy2ContentString, fail to alloc memory ")
TRACE_MSG(MTV_SERVICE_4113_112_2_17_23_13_12_3361,"[SG]: Copy2ContentString, fail to alloc memory ")
TRACE_MSG(MTV_SERVICE_4175_112_2_17_23_13_12_3362,"[mbbms service ]CopyDataFromlist  type=%d\n")
TRACE_MSG(MTV_SERVICE_4298_112_2_17_23_13_12_3363,"[mbbms service ]OrderByWeight  is_increase =%d,total_count =%d\n")
TRACE_MSG(MTV_SERVICE_4345_112_2_17_23_13_12_3364,"[mbbms service ]HandlePdpSig sig_ptr->subtype %d\n")
TRACE_MSG(MTV_SERVICE_4353_112_2_17_23_13_12_3365,"[mbbms service ]HandlePdpSig   type %d\n")
TRACE_MSG(MTV_SERVICE_4428_112_2_17_23_13_12_3366,"[mbbms service ]SetCMMBService  net_id =%d,serviceid =%d\n")
TRACE_MSG(MTV_SERVICE_4492_112_2_17_23_13_12_3367,"[mbbms service ]SetServiceList  net_id =%d\n")
TRACE_MSG(MTV_SERVICE_4516_112_2_17_23_13_13_3368,"[mbbms service ]SetServiceList  total_count =%d\n")
TRACE_MSG(MTV_SERVICE_4530_112_2_17_23_13_13_3369,"[mbbms service ]GetCMMBProgListFromNetInfo  network_number =%d\n")
TRACE_MSG(MTV_SERVICE_4534_112_2_17_23_13_13_3370,"[mbbms service ]GetCMMBProgListFromNetInfo  network_number = 0 or list_ptr== PNULL\n")
TRACE_MSG(MTV_SERVICE_4676_112_2_17_23_13_13_3371,"[mbbms service ]GetCMMBProgFromFile  \n")
TRACE_MSG(MTV_SERVICE_4698_112_2_17_23_13_13_3372,"[mbbms service ]GetCMMBProgFromFile fail to create file \n")
TRACE_MSG(MTV_SERVICE_4705_112_2_17_23_13_13_3373,"[mbbms service ]GetCMMBProgFromFile fail to read  file\n")
TRACE_MSG(MTV_SERVICE_4714_112_2_17_23_13_13_3374,"[mbbms service ]GetCMMBProgFromFile is_valid%d\n")
TRACE_MSG(MTV_SERVICE_4723_112_2_17_23_13_13_3375,"[mbbms service ]GetCMMBProgFromFile network_num %d\n")
TRACE_MSG(MTV_SERVICE_4765_112_2_17_23_13_13_3376,"[mbbms service ]GetCMMBProgramFromFile  \n")
TRACE_MSG(MTV_SERVICE_4809_112_2_17_23_13_13_3377,"[mbbms service ] SetControlData  \n")
TRACE_MSG(MTV_SERVICE_4845_112_2_17_23_13_13_3378,"[mbbms service ]SetControlData is_valid%d\n")
TRACE_MSG(MTV_SERVICE_4854_112_2_17_23_13_13_3379,"[mbbms service ]SetControlData network_num %d\n")
TRACE_MSG(MTV_SERVICE_4902_112_2_17_23_13_13_3380,"[mbbms service ]GetCMMBControlDataFromFile fail to SetControlTabData \n")
TRACE_MSG(MTV_SERVICE_4949_112_2_17_23_13_13_3381,"[mbbms service ] SaveControlDataForTest:name_len %d\n")
TRACE_MSG(MTV_SERVICE_4961_112_2_17_23_13_13_3382,"[mbbms service ] SaveControlDataForTest: fail to create file to read \n")
TRACE_MSG(MTV_SERVICE_4983_112_2_17_23_13_13_3383,"[mbbms service ] SaveControlDataForTest: fail to create dir \n")
TRACE_MSG(MTV_SERVICE_5006_112_2_17_23_13_13_3384,"[mbbms service ] SaveControlDataForTest: fail to create file to write \n")
TRACE_MSG(MTV_SERVICE_5020_112_2_17_23_13_13_3385,"[mbbms service ] SaveControlDataForTest: fail to alloc mem \n")
TRACE_MSG(MTV_SERVICE_5036_112_2_17_23_13_14_3386,"[mbbms service ] SaveControlDataForTest: read err \n")
TRACE_MSG(MTV_SERVICE_5048_112_2_17_23_13_14_3387,"[mbbms service ] SaveControlDataForTest: write err \n")
TRACE_MSG(MTV_SERVICE_5086_112_2_17_23_13_14_3388,"[mbbms service ]SaveControlData    \n")
TRACE_MSG(MTV_SERVICE_5094_112_2_17_23_13_14_3389,"[mbbms service]: SaveControlData network_number = %d.")
TRACE_MSG(MTV_SERVICE_5138_112_2_17_23_13_14_3390,"[mbbms service]: SaveControlData write hader err\n")
TRACE_MSG(MTV_SERVICE_5156_112_2_17_23_13_14_3391,"[mbbms service]: SaveControlData write data err\n")
TRACE_MSG(MTV_SERVICE_5169_112_2_17_23_13_14_3392,"[SG]: SaveProgramList set pointer  error")
TRACE_MSG(MTV_SERVICE_5184_112_2_17_23_13_14_3393,"[mbbms service]: SaveProgramList write ctl data info  error")
TRACE_MSG(MTV_SERVICE_5198_112_2_17_23_13_14_3394,"[mbbms service]: SaveProgramList set pointer  error")
TRACE_MSG(MTV_SERVICE_5211_112_2_17_23_13_14_3395,"[mbbms service]: SaveProgramList write header  error")
TRACE_MSG(MTV_SERVICE_5255_112_2_17_23_13_14_3396,"[mbbms service ]SaveProgramList network_number %d  \n")
TRACE_MSG(MTV_SERVICE_5303_112_2_17_23_13_14_3397,"[SG]: SaveProgramList set pointer  error")
TRACE_MSG(MTV_SERVICE_5342_112_2_17_23_13_14_3398,"[mbbms service ]PrintProgramList count %d  \n")
TRACE_MSG(MTV_SERVICE_5347_112_2_17_23_13_14_3399,"[mbbms service ]PrintProgramList program[%d]  \n")
TRACE_MSG(MTV_SERVICE_5348_112_2_17_23_13_14_3400,"[mbbms service ] service name len %d\n")
TRACE_MSG(MTV_SERVICE_5349_112_2_17_23_13_14_3401,"[mbbms service ] service char_set %d\n")
TRACE_MSG(MTV_SERVICE_5350_112_2_17_23_13_14_3402,"[mbbms service ] service net_id %d\n")
TRACE_MSG(MTV_SERVICE_5351_112_2_17_23_13_14_3403,"[mbbms service ] service is_forfree %d\n")
TRACE_MSG(MTV_SERVICE_5352_112_2_17_23_13_14_3404,"[mbbms service ] service id %d\n")
TRACE_MSG(MTV_SERVICE_5353_112_2_17_23_13_14_3405,"[mbbms service ] is_subscribed %d\n")
TRACE_MSG(MTV_SERVICE_5354_112_2_17_23_13_14_3406,"[mbbms service ] is_encryped %d\n")
TRACE_MSG(MTV_SERVICE_5355_112_2_17_23_13_14_3407,"[mbbms service ]servicetype_count%d\n")
TRACE_MSG(MTV_SERVICE_5356_112_2_17_23_13_14_3408,"[mbbms service ]genretype_count%d\n")
TRACE_MSG(MTV_SERVICE_5357_112_2_17_23_13_14_3409,"[mbbms service ]weight%d\n")
TRACE_MSG(MTV_SERVICE_5358_112_2_17_23_13_14_3410,"[mbbms service ]curcontentname %d\n")
TRACE_MSG(MTV_SERVICE_5386_112_2_17_23_13_14_3411,"[mbbms service ]GetCMMBProgramList list_ptr == PNULL")
TRACE_MSG(MTV_SERVICE_5389_112_2_17_23_13_14_3412,"[mbbms service ]GetCMMBProgramList is_updated %d  \n")
TRACE_MSG(MTV_SERVICE_5451_112_2_17_23_13_14_3413,"[mbbms service ]GetMBBMSProgramList is_updated %d ,language_type %d \n")
TRACE_MSG(MTV_SERVICE_5494_112_2_17_23_13_14_3414,"[SL]  MTVSERVICE_GetProgramList serviceid%s  can't get name")
TRACE_MSG(MTV_SERVICE_5503_112_2_17_23_13_14_3415,"[mbbms service ]  MTVSERVICE_GetProgramList serviceid=%s  can't get subcribed ")
TRACE_MSG(MTV_SERVICE_5508_112_2_17_23_13_14_3416,"[mbbms service ]  MTVSERVICE_GetProgramList serviceid=%s  can't get encryption ")
TRACE_MSG(MTV_SERVICE_5516_112_2_17_23_13_14_3417,"[mbbms service ]  MTVSERVICE_GetProgramList serviceid=%s  can't get SERVICE_GENRE")
TRACE_MSG(MTV_SERVICE_5521_112_2_17_23_13_14_3418,"[mbbms service ]  MTVSERVICE_GetProgramList serviceid=%s can't get  SERVICE_TYPE ")
TRACE_MSG(MTV_SERVICE_5599_112_2_17_23_13_15_3419,"[mbbms service ] MTVSERVICE_GetProgramList is_updated %d ,language_type %d \n")
TRACE_MSG(MTV_SERVICE_5682_112_2_17_23_13_15_3420,"[mbbms service ] GetCMMBResult operate_status %d \n")
TRACE_MSG(MTV_SERVICE_5744_112_2_17_23_13_15_3421,"[mbbms service ] SetPrivatePtr ptr 0x%x  \n")
TRACE_MSG(MTV_SERVICE_5762_112_2_17_23_13_15_3422,"[mbbms service ] WstrToStr   \n")
TRACE_MSG(MTV_SERVICE_5802_112_2_17_23_13_15_3423,"[mbbms service ] SetESGPath esg_len %d \n")
TRACE_MSG(MTV_SERVICE_5808_112_2_17_23_13_15_3424,"[mbbms service ] SetESGPath  string is not usc2\n")
TRACE_MSG(MTV_SERVICE_5816_112_2_17_23_13_15_3425,"[mbbms service ] SetESGPath  fail to WstrToStr\n")
TRACE_MSG(MTV_SERVICE_5824_112_2_17_23_13_15_3426,"[mbbms service ] SetESGPath data path %d\n")
TRACE_MSG(MTV_SERVICE_5830_112_2_17_23_13_15_3427,"[mbbms service ] SetESGPath  fail to set esg data path\n")
TRACE_MSG(MTV_SERVICE_5874_112_2_17_23_13_15_3428,"[mbbms service ] SearchAllEsg \n")
TRACE_MSG(MTV_SERVICE_5889_112_2_17_23_13_15_3429,"[mbbms service ] SearchAllEsg fail to alloc mem \n")
TRACE_MSG(MTV_SERVICE_5910_112_2_17_23_13_15_3430,"[mbbms service ] SearchAllEsg fail to search esg \n")
TRACE_MSG(MTV_SERVICE_5945_112_2_17_23_13_15_3431,"[mbbms service ] CMMB_ManualUpdateSG search_type %d ,para %d ,type %d\n")
TRACE_MSG(MTV_SERVICE_6003_112_2_17_23_13_15_3432,"[mbbms service ] CMMB_ManualUpdateSG rt %d \n")
TRACE_MSG(MTV_SERVICE_6012_112_2_17_23_13_15_3433,"[mbbms service ] CMMB_ManualUpdateSG type %d ,subtype %d \n")
TRACE_MSG(MTV_SERVICE_6036_112_2_17_23_13_15_3434,"[mbbms service ] HandleSearchProgCnf state.private_int %d  \n")
TRACE_MSG(MTV_SERVICE_6136_112_2_17_23_13_16_3435,"[mbbms service ] FreeCMMBData state.private_ptr 0x%x  \n")
TRACE_MSG(MTV_SERVICE_6174_112_2_17_23_13_16_3436,"[mbbms service ] HandleSearchEsgCnf state.private_int %d  \n")
TRACE_MSG(MTV_SERVICE_6193_112_2_17_23_13_16_3437,"[mbbms service ] HandleSearchEsgCnf state.private_ptr  0x%x ,search_index %d ,network_number %d n")
TRACE_MSG(MTV_SERVICE_6200_112_2_17_23_13_16_3438,"[mbbms service ] HandleSearchEsgCnf netid %d n")
TRACE_MSG(MTV_SERVICE_6341_112_2_17_23_13_16_3439,"[mbbms service ] HandleCMMBSig type %d ,subtype %d \n")
TRACE_MSG(MTV_SERVICE_6434_112_2_17_23_13_16_3440,"[mbbms service ] CMMB_StopManualUpdateSG type %d ,subtype %d \n")
TRACE_MSG(MTV_SERVICE_6440_112_2_17_23_13_16_3441,"[mbbms service ] CMMB_StopManualUpdateSG type %d ,subtype %d \n")
TRACE_MSG(MTV_SERVICE_6485_112_2_17_23_13_16_3442,"[mbbms service ]MBBMSSG_ManualUpdateSG starting\n")
TRACE_MSG(MTV_SERVICE_6508_112_2_17_23_13_16_3443,"[mbbms service ]MBBMSSG_ManualUpdateSG over \n")
TRACE_MSG(MTV_SERVICE_6527_112_2_17_23_13_16_3444,"[mbbms service ]MBBMSSG_StopManualUpdateSG starting... \n")
TRACE_MSG(MTV_SERVICE_6537_112_2_17_23_13_16_3445,"[mbbms service ]  MBBMSSG_StopManualUpdateSG type %d ,subtype %d \n")
TRACE_MSG(MTV_SERVICE_6579_112_2_17_23_13_17_3446,"[mbbms service ]MBBMSSG_StopManualUpdateSG rt %d\n")
TRACE_MSG(MTV_SERVICE_6604_112_2_17_23_13_17_3447,"[mbbms service ] MTVSERVICE_ManualUpdateSG search_type %d ,para %d \n")
TRACE_MSG(MTV_SERVICE_6632_112_2_17_23_13_17_3448,"[mbbms service ]MTVSERVICE_ManualUpdateSG  rt %d\n")
TRACE_MSG(MTV_SERVICE_6652_112_2_17_23_13_17_3449,"[mbbms service ]MTVSERVICE_StopManualUpdateSG \n")
TRACE_MSG(MTV_SERVICE_6679_112_2_17_23_13_17_3450,"[mbbms service ]MTVSERVICE_StopManualUpdateSG  rt %d\n")
TRACE_MSG(MTV_SERVICE_6723_112_2_17_23_13_17_3451,"[mbbms service ]MTVSERVICE_SGSetWap \n")
TRACE_MSG(MTV_SERVICE_6744_112_2_17_23_13_17_3452,"[mbbms service ]MTVSERVICE_SetSGURI \n")
TRACE_MSG(MTV_SERVICE_6766_112_2_17_23_13_17_3453,"[mbbms service ]MTVSERVICE_SetSGAreaName \n")
TRACE_MSG(MTV_SERVICE_6790_112_2_17_23_13_17_3454,"[mbbms service ]GetActType sig_code %d ,stop_sg_process %d\n ")
TRACE_MSG(MTV_SERVICE_6831_112_2_17_23_13_17_3455,"[mbbms service ]SendSigToMTVService \n ")
TRACE_MSG(MTV_SERVICE_6839_112_2_17_23_13_17_3456,"[mbbms service ]SendSigToMTVService sig_code %d \n ")
TRACE_MSG(MTV_SERVICE_6876_112_2_17_23_13_17_3457,"[mbbms service ]SendSigToMTVService fait to get action \n")
TRACE_MSG(MTV_SERVICE_6879_112_2_17_23_13_17_3458,"[mbbms service ]SendSigToMTVService act %d\n ")
TRACE_MSG(MTV_SERVICE_6882_112_2_17_23_13_17_3459,"[mbbms service ]SendSigToMTVService type %d,subtype %d \n")
TRACE_MSG(MTV_SERVICE_6901_112_2_17_23_13_17_3460,"[mbbms service ]SendCmmbSigToMTVService ack_id %d,para0 %d \n")
TRACE_MSG(MTV_SERVICE_6906_112_2_17_23_13_17_3461,"[mbbms service ]SendCmmbSigToMTVService fail to alloc mem \n")
TRACE_MSG(MTV_SERVICE_6919_112_2_17_23_13_17_3462,"[mbbms service ]SendCmmbSigToMTVService type %d,subtype %d \n")
TRACE_MSG(MTV_SERVICE_6940_112_2_17_23_13_17_3463,"[mbbms service ]AppCallback type %d,subtype %d rst %dn")
TRACE_MSG(MTV_SERVICE_6969_112_2_17_23_13_17_3464,"[mbbms service ]FinishParseESG \n")
TRACE_MSG(MTV_SERVICE_6984_112_2_17_23_13_17_3465,"[mbbms service ]MTVSERVICE_ParseESG \n")
TRACE_MSG(MTV_SERVICE_7008_112_2_17_23_13_17_3466,"[mbbms service ]MTVSERVICE_ParseESG ret %d \n")
TRACE_MSG(MTV_SERVICE_7022_112_2_17_23_13_17_3467,"[mbbms service ]MTVSERVICE_ParseESG rt %d \n")
TRACE_MSG(MTV_SERVICE_7042_112_2_17_23_13_17_3468,"[mbbms service ]MTVSERVICE_StopParseESG \n")
TRACE_MSG(MTV_SERVICE_7078_112_2_17_23_13_18_3469,"[mbbms service ]MTVSERVICE_StopParseESG rt %d \n")
TRACE_MSG(MTV_SERVICE_7097_112_2_17_23_13_18_3470,"[mbbms service] TestPoint type %d\n")
TRACE_MSG(MTV_SERVICE_7121_112_2_17_23_13_18_3471,"[mbbms service] TestNextStep  type %d\n")
TRACE_MSG(MTV_SERVICE_7143_112_2_17_23_13_18_3472,"[mbbms service] Test_ManualUpdateSG type  %d\n")
TRACE_MSG(MTV_SERVICE_7186_112_2_17_23_13_18_3473,"[mbbms service] Test_AllManualUpdateSG   %d\n")
TRACE_MSG(MTV_SERVICE_7230_112_2_17_23_13_18_3474,"\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG  test_times %d,g_test_flag %d")
TRACE_MSG(MTV_SERVICE_7243_112_2_17_23_13_18_3475,"\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG   test again(1)  \n")
TRACE_MSG(MTV_SERVICE_7257_112_2_17_23_13_18_3476,"\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG   test again(2)  \n")
TRACE_MSG(MTV_SERVICE_7275_112_2_17_23_13_18_3477,"\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG   test again(3)  \n")
TRACE_MSG(MTV_SERVICE_7282_112_2_17_23_13_18_3478,"\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG   test again(4)  \n")
TRACE_MSG(MTV_SERVICE_7292_112_2_17_23_13_18_3479,"\"\"\"\"\"\"\"\"[mbbms service] Test_StopManualUpdateSG  finish  \n")
TRACE_MSG(MTV_SERVICE_7317_112_2_17_23_13_18_3480,"[mbbms service] Test_StopManualUpdateSG   times %d\n")
TRACE_MSG(MTV_SERVICE_7342_112_2_17_23_13_18_3481,"\"\"\"\"\"\"\"\"[mbbms service] Test_ActiveMBBMS\n")
TRACE_MSG(MTV_SERVICE_7365_112_2_17_23_13_18_3482,"\"\"\"\"\"\"\"\"[mbbms service] Test_DeactiveMBBMS\n")
TRACE_MSG(MTV_SERVICE_7388_112_2_17_23_13_18_3483,"[mbbms service] Test_DeactiveMBBMS   times %d\n")
TRACE_MSG(MTV_SERVICE_7432_112_2_17_23_13_18_3484,"[mbbms service] TestThread g_test_count %d, times %d,type %d, mode %d\n")
TRACE_MSG(MTV_SERVICE_7513_112_2_17_23_13_18_3485,"\"\"\"\"\"\"\"\"[mbbms service ]  TEST_StopParse\n")
TRACE_MSG(MTV_SERVICE_7531_112_2_17_23_13_18_3486,"MBBMS mmi_callback received type:%d  subtype:%d result:%d ptr:%x n")
TRACE_MSG(MTV_SERVICE_7594_112_2_17_23_13_19_3487,"\"\"\"\"\"\"\"\"[mbbms service ]  MTVS_Test \n")
TRACE_MSG(MTV_SERVICE_SUB_69_112_2_17_23_13_19_3488,"mbbmsservice_handleUnsubscribe received type:%d subtype:%d result:%dn")
TRACE_MSG(MTV_SERVICE_SUB_72_112_2_17_23_13_19_3489,"mbmsservice_handleUnsubscribe MBBMS_SERVICE_UNSUBSCRIBE OVER Already!\n")
TRACE_MSG(MTV_SERVICE_SUB_84_112_2_17_23_13_19_3490,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Register PDP Fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_100_112_2_17_23_13_19_3491,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE starting ... \n")
TRACE_MSG(MTV_SERVICE_SUB_105_112_2_17_23_13_19_3492,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE PDP ok, add register failed. \n")
TRACE_MSG(MTV_SERVICE_SUB_113_112_2_17_23_13_19_3493,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Send PDP failed. \n")
TRACE_MSG(MTV_SERVICE_SUB_122_112_2_17_23_13_19_3494,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_131_112_2_17_23_13_19_3495,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE GBA Failed, Starting GBA.... \n")
TRACE_MSG(MTV_SERVICE_SUB_139_112_2_17_23_13_19_3496,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Start GBA Failed.. \n")
TRACE_MSG(MTV_SERVICE_SUB_148_112_2_17_23_13_19_3497,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE STOP ok recieved. \n")
TRACE_MSG(MTV_SERVICE_SUB_156_112_2_17_23_13_19_3498,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Failed result received.. \n")
TRACE_MSG(MTV_SERVICE_SUB_163_112_2_17_23_13_19_3499,"[mbbms service]MBBMS_SERVICE_STOP_UNSUBSCRIBE request Received..... \n")
TRACE_MSG(MTV_SERVICE_SUB_169_112_2_17_23_13_19_3500,"[mbbms service]MBBMS_SERVICE_STOP_UNSUBSCRIBE remove pdp Fail \n")
TRACE_MSG(MTV_SERVICE_SUB_178_112_2_17_23_13_19_3501,"[mbbms service]MTVSERVICE_RemoveRegister remove gba immediately \n")
TRACE_MSG(MTV_SERVICE_SUB_194_112_2_17_23_13_19_3502,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Remove Register OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_202_112_2_17_23_13_19_3503,"[mbbms service]MTVSERVICE_RemoveRegister MBBMS_SERVICE_UNSUBSCRIBE return FAIL\n")
TRACE_MSG(MTV_SERVICE_SUB_218_112_2_17_23_13_19_3504,"[mbbms service]MBBMS_SERVICE_STOP_UNSUBSCRIBE signal: Wrong State, unsub may be done already. \n")
TRACE_MSG(MTV_SERVICE_SUB_227_112_2_17_23_13_19_3505,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE GBA OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_231_112_2_17_23_13_19_3506,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Start unsubscribe ok. \n")
TRACE_MSG(MTV_SERVICE_SUB_237_112_2_17_23_13_19_3507,"[mbbms service] MBBMS_SERVICE_UNSUBSCRIBE Start unsubscribe FAIL  END. \n")
TRACE_MSG(MTV_SERVICE_SUB_254_112_2_17_23_13_19_3508,"[mbbms service] mbmsservice_handleUnsubscribe UNKOWN subtype :%d \n")
TRACE_MSG(MTV_SERVICE_SUB_265_112_2_17_23_13_19_3509,"mbbmsservice_handlesubscribe received type:%d subtype:%d result:%dn")
TRACE_MSG(MTV_SERVICE_SUB_268_112_2_17_23_13_19_3510,"mbbmsservice_handlesubscribe MBBMS_SERVICE_SUBSCRIBE OVER Already!\n")
TRACE_MSG(MTV_SERVICE_SUB_279_112_2_17_23_13_19_3511,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register PDP fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_286_112_2_17_23_13_20_3512,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE register PDP OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_298_112_2_17_23_13_20_3513,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register Subscribe OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_303_112_2_17_23_13_20_3514,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register Subscribe Fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_310_112_2_17_23_13_20_3515,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register PDP Fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_319_112_2_17_23_13_20_3516,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE  Subscribe finished. \n")
TRACE_MSG(MTV_SERVICE_SUB_330_112_2_17_23_13_20_3517,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register GBA OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_335_112_2_17_23_13_20_3518,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register GBA Fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_343_112_2_17_23_13_20_3519,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register Subscribe STOP OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_349_112_2_17_23_13_20_3520,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE Register Subscribe Unkown msg. Failed. \n")
TRACE_MSG(MTV_SERVICE_SUB_356_112_2_17_23_13_20_3521,"[mbbms service]MBBMS_SERVICE_STOP_SUBSCRIBE request Received..... \n")
TRACE_MSG(MTV_SERVICE_SUB_362_112_2_17_23_13_20_3522,"[mbbms service]MBBMS_SERVICE_STOP_SUBSCRIBE remove pdp Fail \n")
TRACE_MSG(MTV_SERVICE_SUB_371_112_2_17_23_13_20_3523,"[mbbms service]MBBMS_SERVICE_SUBSCRIBE MTVSERVICE_RemoveRegister remove gba immediately \n")
TRACE_MSG(MTV_SERVICE_SUB_384_112_2_17_23_13_20_3524,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE RemoveRegister Subscribe OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_396_112_2_17_23_13_20_3525,"[mbbms service]MTVSERVICE_RemoveRegister SUBSCRIBE return FAIL\n")
TRACE_MSG(MTV_SERVICE_SUB_401_112_2_17_23_13_20_3526,"[mbbms service]MBBMS_SERVICE_STOP_SUBSCRIBE signal: Wrong State, unsub may be done already. \n")
TRACE_MSG(MTV_SERVICE_SUB_414_112_2_17_23_13_20_3527,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE GBA  OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_420_112_2_17_23_13_20_3528,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE GBA  OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_429_112_2_17_23_13_20_3529,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE STOP GBA OK. Finished. \n")
TRACE_MSG(MTV_SERVICE_SUB_436_112_2_17_23_13_20_3530,"[mbbms service] MBBMS_SERVICE_SUBSCRIBE GBA Fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_444_112_2_17_23_13_20_3531,"[mbbms service] mbmsservice_handleSUBSCRIBE UNKOWN subtype :%d \n")
TRACE_MSG(MTV_SERVICE_SUB_454_112_2_17_23_13_20_3532,"mbbmsservice_handleupdatetable received type:%d subtype:%d result:%dn")
TRACE_MSG(MTV_SERVICE_SUB_458_112_2_17_23_13_20_3533,"mbmsservice_handleUpdateTable MBBMS_SERVICE_UPDATE_TABLE OVER Already!\n")
TRACE_MSG(MTV_SERVICE_SUB_470_112_2_17_23_13_20_3534,"[mbbms service] MBBMS_SERVICE_UPDATE_TABLE Register PDP fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_478_112_2_17_23_13_20_3535,"[mbbms service] MBBMS_SERVICE_UPDATE_TABLE Register PDP ok. \n")
TRACE_MSG(MTV_SERVICE_SUB_490_112_2_17_23_13_20_3536,"[mbbms service] MBBMS_SERVICE_UPDATE_TABLE Register updatetable OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_497_112_2_17_23_13_20_3537,"[mbbms service] MBBMS_SERVICE_UPDATE_TABLE Register updatetable Fail. Over. \n")
TRACE_MSG(MTV_SERVICE_SUB_505_112_2_17_23_13_20_3538,"[mbbms service] MBBMS_SERVICE_UPDATE_TABLE PDP active Fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_515_112_2_17_23_13_20_3539,"[mbbms service] MBBMS_SERVICE_UPDATE_TABLE  updatetable OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_523_112_2_17_23_13_20_3540,"[mbbms service] MBBMS_SERVICE_UPDATE_TABLE  updatetable Stop OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_530_112_2_17_23_13_20_3541,"[mbbms service] MBBMS_SERVICE_UPDATE_TABLE  updatetable Fail. Finished. \n")
TRACE_MSG(MTV_SERVICE_SUB_538_112_2_17_23_13_20_3542,"[mbbms service]MBBMS_SERVICE_STOP_UPDATE_TABLE request Received..... \n")
TRACE_MSG(MTV_SERVICE_SUB_545_112_2_17_23_13_20_3543,"[mbbms service]MBBMS_SERVICE_STOP_UPDATE_TABLE remove pdp Fail \n")
TRACE_MSG(MTV_SERVICE_SUB_558_112_2_17_23_13_20_3544,"[mbbms service] MBBMS_SERVICE_UPDATE_TABLE RemoveRegister updatetable OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_568_112_2_17_23_13_20_3545,"[mbbms service]MTVSERVICE_RemoveRegister MBBMS_SERVICE_UPDATE_TABLE return FAIL\n")
TRACE_MSG(MTV_SERVICE_SUB_574_112_2_17_23_13_20_3546,"[mbbms service]MBBMS_SERVICE_STOP_UPDATE_TABLE signal: Wrong State, unsub may be done already. \n")
TRACE_MSG(MTV_SERVICE_SUB_580_112_2_17_23_13_20_3547,"[mbbms service] mbmsservice_handleUpdateTable UNKOWN subtype :%d \n")
TRACE_MSG(MTV_SERVICE_SUB_590_112_2_17_23_13_20_3548,"mbbmsservice_handleaccountquiry received type:%d subtype:%d result:%dn")
TRACE_MSG(MTV_SERVICE_SUB_594_112_2_17_23_13_20_3549,"mbmsservice_handleAccountQuiry MBBMS_SERVICE_ACCOUNT_QUIRY OVER Already!\n")
TRACE_MSG(MTV_SERVICE_SUB_605_112_2_17_23_13_20_3550,"[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY Register pdp fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_613_112_2_17_23_13_20_3551,"[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY Register pdp OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_623_112_2_17_23_13_20_3552,"[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY Register accountquiry OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_629_112_2_17_23_13_20_3553,"[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY Register accountquiry Fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_636_112_2_17_23_13_20_3554,"[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY PDP Active Fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_645_112_2_17_23_13_20_3555,"[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY  accountquiry OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_652_112_2_17_23_13_20_3556,"[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY  accountquiry STOP OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_658_112_2_17_23_13_20_3557,"[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY  accountquiry not OK. Finished. \n")
TRACE_MSG(MTV_SERVICE_SUB_665_112_2_17_23_13_20_3558,"[mbbms service]MBBMS_SERVICE_STOP_ACCOUNT_QUIRY request Received..... \n")
TRACE_MSG(MTV_SERVICE_SUB_671_112_2_17_23_13_20_3559,"[mbbms service]MBBMS_SERVICE_STOP_ACCOUNT_QUIRY remove pdp Fail \n")
TRACE_MSG(MTV_SERVICE_SUB_683_112_2_17_23_13_20_3560,"[mbbms service] MBBMS_SERVICE_ACCOUNT_QUIRY removeRegister accountquiry OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_691_112_2_17_23_13_20_3561,"[mbbms service]MTVSERVICE_RemoveRegister MBBMS_SERVICE_ACCOUNT_QUIRY return FAIL\n")
TRACE_MSG(MTV_SERVICE_SUB_700_112_2_17_23_13_20_3562,"[mbbms service]MBBMS_SERVICE_STOP_ACCOUNT_QUIRY signal: Wrong State, unsub may be done already. \n")
TRACE_MSG(MTV_SERVICE_SUB_706_112_2_17_23_13_20_3563,"[mbbms service] mbmsservice_handleAccountQuiry UNKOWN subtype :%d \n")
TRACE_MSG(MTV_SERVICE_SUB_715_112_2_17_23_13_20_3564,"mbbmsservice_handlegba received type:%d subtype:%d result:%dn")
TRACE_MSG(MTV_SERVICE_SUB_718_112_2_17_23_13_20_3565,"mbmsservice_handleGBA MBBMS_SERVICE_GBA OVER Already!\n")
TRACE_MSG(MTV_SERVICE_SUB_728_112_2_17_23_13_20_3566,"[mbbms service] MBBMS_SERVICE_GBA Register PDP fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_735_112_2_17_23_13_20_3567,"[mbbms service] MBBMS_SERVICE_GBA Register PDP OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_746_112_2_17_23_13_20_3568,"[mbbms service] MBBMS_SERVICE_GBA Register GBA OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_751_112_2_17_23_13_20_3569,"[mbbms service] MBBMS_SERVICE_GBA Register GBA fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_758_112_2_17_23_13_20_3570,"[mbbms service] MBBMS_SERVICE_GBA active PDP fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_769_112_2_17_23_13_21_3571,"MBBMS_SERVICE_GBA description len:%d impi:%sn")
TRACE_MSG(MTV_SERVICE_SUB_775_112_2_17_23_13_21_3572,"[mbbms service] MBBMS_SERVICE_GBA GBA Stop OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_781_112_2_17_23_13_21_3573,"[mbbms service] MBBMS_SERVICE_GBA GBA Not OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_789_112_2_17_23_13_21_3574,"[mbbms service]MBBMS_SERVICE_STOP_GBA request Received..... \n")
TRACE_MSG(MTV_SERVICE_SUB_795_112_2_17_23_13_21_3575,"[mbbms service]MBBMS_SERVICE_STOP_UNSUBSCRIBE remove pdp Fail \n")
TRACE_MSG(MTV_SERVICE_SUB_804_112_2_17_23_13_21_3576,"[mbbms service]MTVSERVICE_RemoveRegister remove gba immediately \n")
TRACE_MSG(MTV_SERVICE_SUB_810_112_2_17_23_13_21_3577,"[mbbms service]MBBMS_SERVICE_STOP_GBA signal: Wrong State, unsub may be done already. \n")
TRACE_MSG(MTV_SERVICE_SUB_817_112_2_17_23_13_21_3578,"[mbbms service] mbmsservice_handleGBA UNKOWN subtype :%d \n")
TRACE_MSG(MTV_SERVICE_SUB_826_112_2_17_23_13_21_3579,"mbbmsservice_handlegetmsk received type:%d subtype:%d result:%dn")
TRACE_MSG(MTV_SERVICE_SUB_836_112_2_17_23_13_21_3580,"[mbbms service] MBBMS_SERVICE_GETMSK Register PDP fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_843_112_2_17_23_13_21_3581,"[mbbms service] MBBMS_SERVICE_GETMSK Register PDP OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_854_112_2_17_23_13_21_3582,"[mbbms service] MBBMS_SERVICE_GETMSK add register getmsk ok. \n")
TRACE_MSG(MTV_SERVICE_SUB_860_112_2_17_23_13_21_3583,"[mbbms service] MBBMS_SERVICE_GETMSK add register getmsk Fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_867_112_2_17_23_13_21_3584,"[mbbms service] MBBMS_SERVICE_GETMSK PDP active Failed. \n")
TRACE_MSG(MTV_SERVICE_SUB_877_112_2_17_23_13_21_3585,"[mbbms service] MBBMS_SERVICE_GETMSK OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_885_112_2_17_23_13_21_3586,"[mbbms service] MBBMS_SERVICE_GETMSK GBA Fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_889_112_2_17_23_13_21_3587,"[mbbms service] MBBMS_SERVICE_GETMSK GBA running. \n")
TRACE_MSG(MTV_SERVICE_SUB_894_112_2_17_23_13_21_3588,"[mbbms service] MBBMS_SERVICE_GETMSK GBA running failed. \n")
TRACE_MSG(MTV_SERVICE_SUB_903_112_2_17_23_13_21_3589,"[mbbms service] MBBMS_SERVICE_GETMSK STOP OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_910_112_2_17_23_13_21_3590,"[mbbms service] MBBMS_SERVICE_GETMSK Result error. \n")
TRACE_MSG(MTV_SERVICE_SUB_918_112_2_17_23_13_21_3591,"[mbbms service]MBBMS_SERVICE_STOP_GETMSK request Received..... \n")
TRACE_MSG(MTV_SERVICE_SUB_922_112_2_17_23_13_21_3592,"[mbbms service] MBBMS_SERVICE_GETMSK Remove PDP. \n")
TRACE_MSG(MTV_SERVICE_SUB_926_112_2_17_23_13_21_3593,"[mbbms service]MBBMS_SERVICE_STOP_GETMSK remove pdp Fail \n")
TRACE_MSG(MTV_SERVICE_SUB_934_112_2_17_23_13_21_3594,"[mbbms service] MBBMS_SERVICE_GETMSK Remove GBA \n")
TRACE_MSG(MTV_SERVICE_SUB_937_112_2_17_23_13_21_3595,"[mbbms service]MTVSERVICE_RemoveRegister remove gba immediately \n")
TRACE_MSG(MTV_SERVICE_SUB_946_112_2_17_23_13_21_3596,"[mbbms service] MBBMS_SERVICE_GETMSK Remove Get MSK \n")
TRACE_MSG(MTV_SERVICE_SUB_951_112_2_17_23_13_21_3597,"[mbbms service] MBBMS_SERVICE_GETMSK RemoveRegister GetMSK OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_959_112_2_17_23_13_21_3598,"[mbbms service]MTVSERVICE_RemoveRegister MBBMS_SERVICE_GETMSK return FAIL\n")
TRACE_MSG(MTV_SERVICE_SUB_967_112_2_17_23_13_21_3599,"[mbbms service]MBBMS_SERVICE_STOP_GETMSK signal: Wrong State, unsub may be done already. \n")
TRACE_MSG(MTV_SERVICE_SUB_976_112_2_17_23_13_21_3600,"[mbbms service] MBBMS_SERVICE_GETMSK GBA Result received. \n")
TRACE_MSG(MTV_SERVICE_SUB_982_112_2_17_23_13_21_3601,"[mbbms service] MBBMS_SERVICE_GETMSK Register GBA OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_987_112_2_17_23_13_21_3602,"[mbbms service] MBBMS_SERVICE_GETMSK Register GBA fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_995_112_2_17_23_13_21_3603,"[mbbms service] MBBMS_SERVICE_GETMSK GBA STOP OK. \n")
TRACE_MSG(MTV_SERVICE_SUB_1001_112_2_17_23_13_21_3604,"[mbbms service] MBBMS_SERVICE_GETMSK GBA fail. \n")
TRACE_MSG(MTV_SERVICE_SUB_1008_112_2_17_23_13_21_3605,"[mbbms service] mbmsservice_handleGETMSK UNKOWN subtype :%d \n")
TRACE_MSG(MTV_SERVICE_SUB_1023_112_2_17_23_13_21_3606,"MBBMS mmi_callback received type:%d  subtype:%d result:%d ptr:%x n")
TRACE_MSG(MTV_SERVICE_SUB_1114_112_2_17_23_13_21_3607,"MBBMS_SERVICE_TEST ... \n")
TRACE_MSG(MTV_SERVICE_SUB_1288_112_2_17_23_13_22_3608,"MBBMS_SERVICE_TEST Over ... \n")
TRACE_MSG(DAC_PEER_500_112_2_17_23_13_37_3609,"HYDAC_AP_Open")
TRACE_MSG(MCEX_PEER_74_112_2_17_23_13_38_3610,"Send command failed!\n")
TRACE_MSG(MCEX_PEER_80_112_2_17_23_13_38_3611,"Get response failed!\n")
TRACE_MSG(MTV_IMP_PEER_176_112_2_17_23_13_39_3612,"CMMB_EnumAudioLanguages_wrapper_Imp: num(%d) != ret(%d)")
TRACE_MSG(MTV_IMP_PEER_732_112_2_17_23_13_40_3613,"mtv_v:CMMB_GetServiceName() name_ptr = 0X%x")
TRACE_MSG(MTV_IMP_PEER_757_112_2_17_23_13_40_3614,"mtv_v:CMMB_GetAllProg() nid = %d sid = %d date %d %d %d")
TRACE_MSG(MTV_IMP_PEER_760_112_2_17_23_13_40_3615,"mtv_v:CMMB_GetAllProg() ret = %d proglist_ptr = 0X%x")
TRACE_MSG(MTV_IMP_PEER_776_112_2_17_23_13_40_3616,"mtv_v:CMMB_GetEsgDateRange_Imp() sid = %d")
TRACE_MSG(MTV_IMP_PEER_779_112_2_17_23_13_40_3617,"mtv_v:CMMB_GetEsgDateRange_Imp() ret = %d addr = %x  %x cmd=%x")
TRACE_MSG(MTV_IMP_PEER_811_112_2_17_23_13_40_3618,"mtv_v:CMMB_SetEsgFilter_Imp() sid = %d date %d %d %d")
TRACE_MSG(MUA_PEER_71_112_2_17_23_13_44_3619,"cmd1=%x %x")
TRACE_MSG(MUA_PEER_102_112_2_17_23_13_44_3620,"cmd0=%x,%x,%x,%x,%x,%x,%x,%x")
TRACE_MSG(MUA_PEER_136_112_2_17_23_13_44_3621,"exchange: %d, %d,%x")
TRACE_MSG(MUA_PEER_173_112_2_17_23_13_44_3622,"MUACMD_MBMS_Imp mikey_len %d exceeds buffer len!!!")
TRACE_MSG(MUA_PEER_191_112_2_17_23_13_44_3623,"MUACMD_MBMS_Imp resp_len %d exceeds buffer len!!!")
TRACE_MSG(MUA_PEER_211_112_2_17_23_13_44_3624,"MBBMSSG_GetIsma_Imp: %d %d %p")
TRACE_MSG(MUA_PEER_219_112_2_17_23_13_44_3625,"MBBMSSG_GetIsma_Imp: error %d")
TRACE_MSG(MUA_PEER_241_112_2_17_23_13_44_3626,"buf0=%x,%x,%x,%x,%x,%x,%x,%x")
TRACE_MSG(MUA_PEER_246_112_2_17_23_13_44_3627,"rspLen=%d,rspsw=0x%x")
TRACE_MSG(PROXYCALLPEER_305_112_2_17_23_13_46_3628,"proxycall wait")
TRACE_MSG(PROXYCALLPEER_399_112_2_17_23_13_46_3629,"proxycall thread wait %p %x")
TRACE_MSG(PROXYCALLPEER_404_112_2_17_23_13_46_3630,"proxycall thread queue %d")
TRACE_MSG(PROXYCALLPEER_487_112_2_17_23_13_46_3631,"Proxy_Exec_Thread %d queue %d")
TRACE_MSG(PROXYCALLPEER_492_112_2_17_23_13_46_3632,"Proxy_Exec_Thread %d exit %p %d")
TRACE_MSG(PROXYCALLPEER_514_112_2_17_23_13_46_3633,"Proxy_Exec_Thread %d recv unknown cmd %X")
TRACE_MSG(PROXYCALLPEER_585_112_2_17_23_13_46_3634,"ProxyCall_IsInited: %d")
TRACE_MSG(SFS_PEER_242_112_2_17_23_13_48_3635,"ReadFile: %02X %02X %02X %02X - %02X %02X %02X %02Xn")
TRACE_MSG(SFS_PEER_654_112_2_17_23_13_49_3636,"ReadFile: %x %x %x %x - %x %x %x %xn")
TRACE_MSG(SFS_PEER_781_112_2_17_23_13_49_3637,"ChangFileFdStruEndian: name_len = %d \n")
END_TRACE_MAP(CMMB_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _CMMB_TRC_H_


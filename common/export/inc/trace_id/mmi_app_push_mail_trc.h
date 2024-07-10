/******************************************************************************
 ** File Name:      mmi_app_push_mail_trc.h                                         *
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
//trace_id:214
#ifndef _MMI_APP_PUSH_MAIL_TRC_H_
#define _MMI_APP_PUSH_MAIL_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MAIL_EXPORT_134_112_2_18_2_47_37_0 "Mail:username id : %d:%s password: %s"
#define MAIL_EXPORT_285_112_2_18_2_47_37_1 "Mail:invalid account id:%d"
#define MAIL_EXPORT_306_112_2_18_2_47_37_2 "Mail:invalid account id:%d"
#define MAIL_EXPORT_325_112_2_18_2_47_37_3 "Mail: %s"
#define MAIL_EXPORT_329_112_2_18_2_47_37_4 "Mail:start receive msg-err"
#define MAIL_EXPORT_335_112_2_18_2_47_37_5 "Mail:start receive msg-id:%d, name:%s"
#define MAIL_EXPORT_340_112_2_18_2_47_37_6 "Mail:invalid account id:%d"
#define MAIL_EXPORT_346_112_2_18_2_47_37_7 "Mail:mail_retrievePushMsg-retring \n"
#define MAIL_EXPORT_352_112_2_18_2_47_37_8 "Mail:mail_retrievePushMsg-sending \n"
#define MAIL_EXPORT_378_112_2_18_2_47_37_9 "Mail:mail_RetrAttachStart-retring"
#define MAIL_EXPORT_394_112_2_18_2_47_38_10 "Mail:mail_RetrAttachStart-retring"
#define MAIL_EXPORT_410_112_2_18_2_47_38_11 "Mail:invalid account id:%d"
#define MAIL_EXPORT_416_112_2_18_2_47_38_12 "Mail:mail_RetrAttachStart-retring"
#define MAIL_EXPORT_452_112_2_18_2_47_38_13 "Mail:invalid account id:%d"
#define MAIL_EXPORT_459_112_2_18_2_47_38_14 "Mail:invalid uidl"
#define MAIL_EXPORT_482_112_2_18_2_47_38_15 "Mail:is mail, do not allow u disk"
#define MAIL_EXPORT_489_112_2_18_2_47_38_16 "Mail:is receiving mail, do not allow u disk"
#define MAIL_EXPORT_496_112_2_18_2_47_38_17 "Mail:is writing mail, do not allow u disk"
#define MAIL_EXPORT_659_112_2_18_2_47_38_18 "Mail:MAIL_ListInit no sim card or switch sim card"
#define MAIL_EXPORT_841_112_2_18_2_47_39_19 "MAIL_InsertMsgToSMS:msgId = %d"
#define MAIL_EXPORT_842_112_2_18_2_47_39_20 "MAIL_InsertMsgToSMS:record_id = %d"
#define MAIL_EXPORT_956_112_2_18_2_47_39_21 "mail_GetMailLockFlag:index < 0"
#define MAIL_EXPORT_982_112_2_18_2_47_39_22 "mail_GetMailSendingFlag:index < 0"
#define MAIL_EXPORT_1008_112_2_18_2_47_39_23 "mail_GetMailSecurityFlag:index < 0"
#define MAIL_EXPORT_1034_112_2_18_2_47_39_24 "mail_GetMailSecurityFlag:index < 0"
#define MAIL_EXPORT_1075_112_2_18_2_47_39_25 "mail_fldr mail_delMessageByMsgIdOtherApp del msg(%d)start! \n"
#define MAIL_EXPORT_1081_112_2_18_2_47_39_26 "mail_fldr mail_delMessageByMsgIdOtherApp:index-msgId not matched"
#define MAIL_EXPORT_1088_112_2_18_2_47_39_27 "mail_fldr mail_delMessageByMsgIdOtherApp:delete msg not found or msg list not found"
#define MAIL_EXPORT_1092_112_2_18_2_47_39_28 "mail_fldr mail_delMessageByMsgIdOtherApp delete msg accountId = %d \n"
#define MAIL_EXPORT_1093_112_2_18_2_47_39_29 "mail_fldr mail_delMessageByMsgIdOtherApp cur accountId = \n"
#define MAIL_EXPORT_1096_112_2_18_2_47_39_30 "mail_fldr mail_delMessageByMsgIdOtherApp mail->uidl=%s \n"
#define MAIL_EXPORT_1320_112_2_18_2_47_40_31 "mail_MoveMailToSecurityBoxEx!"
#define MAIL_EXPORT_1343_112_2_18_2_47_40_32 "mail_delSMSMessageListById:msg_id = %d"
#define MAIL_EXPORT_1344_112_2_18_2_47_40_33 "mail_delSMSMessageListById:record_id = %d"
#define MAIL_FILE_340_112_2_18_2_47_41_34 "Mail: mail_FileOperationWriteBuf: buffer %s \n"
#define MAIL_FILE_356_112_2_18_2_47_41_35 "Mail: mail_FileOperationWriteBuf: written %d \n"
#define MAIL_FILE_690_112_2_18_2_47_42_36 "MMIMail mail_createFolder full_path_len <= 0"
#define MAIL_FILE_736_112_2_18_2_47_42_37 "mail_fldr mail_FileOperationRead set pointer fail\n"
#define MAIL_FILE_743_112_2_18_2_47_42_38 "mail_fldr mail_FileOperationRead error=%d\n"
#define MAIL_FILE_744_112_2_18_2_47_42_39 "mail_fldr mail_FileOperationRead read_Size=%d\n"
#define MAIL_FILE_745_112_2_18_2_47_42_40 "mail_fldr mail_FileOperationRead bufSize=%d\n"
#define MAIL_FILE_753_112_2_18_2_47_42_41 "mail_fldr mail_FileOperationRead  bufsize=%d,read_size=%d\n"
#define MAIL_FILE_757_112_2_18_2_47_42_42 "mail_fldr mail_FileOperationRead error=%d\n"
#define MAIL_FILE_758_112_2_18_2_47_42_43 "mail_fldr mail_FileOperationRead read_Size=%d\n"
#define MAIL_FILE_759_112_2_18_2_47_42_44 "mail_fldr mail_FileOperationRead bufSize=%d\n"
#define MAIL_FILE_807_112_2_18_2_47_42_45 "mail_fldr mail_FileOperationWrite bufsize=%d,buf_write=%d \n"
#define MAIL_FILE_937_112_2_18_2_47_42_46 "mail_RenameFileByWchar srcName=%d,dstName =%d"
#define MAIL_FILE_995_112_2_18_2_47_42_47 "mail_RenameFileByWchar oldName=%d,newName =%d"
#define MAIL_FILE_1036_112_2_18_2_47_43_48 "mail_fldr mail_RenameFile del old fail!\n"
#define MAIL_FILE_1049_112_2_18_2_47_43_49 "mail_fldr mail_RenameFile res=%d\n"
#define MAIL_FILE_1082_112_2_18_2_47_43_50 "mail_FileNameCmp FileName1=%d, FileName2 =%d"
#define MAIL_FILE_1150_112_2_18_2_47_43_51 "mail_GetFileNumOfDir"
#define MAIL_FILE_1198_112_2_18_2_47_43_52 "mail_RemoveFile FileName=%d"
#define MAIL_FILE_1235_112_2_18_2_47_43_53 "mail_fldr mail_sentMailWriteFile start write file(%s) buffer_size=%d \n"
#define MAIL_FILE_1243_112_2_18_2_47_43_54 "mail_fldr mail_sentMailWriteFile create file(%s)fail!\n"
#define MAIL_FILE_1316_112_2_18_2_47_43_55 "Mail:delet folder exit\n"
#define MAIL_FILE_1367_112_2_18_2_47_43_56 "Mail:mail_GetDirFileNum %d\n"
#define MAIL_MAIN_159_112_2_18_2_47_44_57 "MMIMail mail_SendSignaltoMailTask PNULL == sig_ptr"
#define MAIL_MAIN_169_112_2_18_2_47_44_58 "MAIL: mail_SendSignaltoMailTask sender:%d cur: %d .event: %x\n"
#define MAIL_MAIN_199_112_2_18_2_47_44_59 "MAIL: mail_SendSignaltoMailTask sender:%d cur: %d .event: %x\n"
#define MAIL_MAIN_225_112_2_18_2_47_44_60 "MMIMail mail_SendSignaltoMailParseTask PNULL == sig_ptr"
#define MAIL_MAIN_234_112_2_18_2_47_44_61 "MAIL: mail_SendSignaltoMailParseTask sender:%d cur: %d .event: %x\n"
#define MAIL_MAIN_258_112_2_18_2_47_44_62 "MMIMail mail_handleTaskEvent PNULL == sig_ptr"
#define MAIL_MAIN_262_112_2_18_2_47_44_63 "Mail: mail_handleTaskEvent signal id: %x "
#define MAIL_MAIN_284_112_2_18_2_47_44_64 "Mail:start resend!!!!\n"
#define MAIL_MAIN_316_112_2_18_2_47_44_65 "Mail: MSG_MAIL_TIME_OUT mail recv timeout!"
#define MAIL_MAIN_336_112_2_18_2_47_44_66 "Mail: MSG_MAIL_RETR_START is retring "
#define MAIL_MAIN_340_112_2_18_2_47_44_67 "Mail: MSG_MAIL_RETR_START Reset g_mail_is_recv_timeout to FALSE!"
#define MAIL_MAIN_374_112_2_18_2_47_44_68 "Mail: comm_NetTask data arrival "
#define MAIL_MAIN_381_112_2_18_2_47_44_69 "Mail: MSG_MAIL_DATA_ARRIVAL "
#define MAIL_MAIN_388_112_2_18_2_47_44_70 "Mail: comm_NetTask data can send "
#define MAIL_MAIN_395_112_2_18_2_47_44_71 "Mail: MSG_MAIL_DATA_SEND "
#define MAIL_MAIN_404_112_2_18_2_47_44_72 "Mail: comm_NetTask connect error = %d "
#define MAIL_MAIN_413_112_2_18_2_47_44_73 "Mail: comm_NetTask server close this connection err: %d\n "
#define MAIL_MAIN_424_112_2_18_2_47_44_74 "Mail:comm_NetTask unkonwn event :%d"
#define MAIL_MAIN_457_112_2_18_2_47_44_75 "MMIMail mail_handleParseTaskEvent PNULL == sig_ptr"
#define MAIL_MAIN_461_112_2_18_2_47_44_76 "Mail: mail_handleParseTaskEvent signal id: %x "
#define MAIL_MAIN_471_112_2_18_2_47_44_77 "Mail: MSG_MAIL_HEADER_START \n "
#define MAIL_MAIN_482_112_2_18_2_47_44_78 "Mail: MSG_MAIL_HEADER_FINISH  \n"
#define MAIL_MAIN_578_112_2_18_2_47_44_79 "Mail: MSG_MAIL_OPERATE_SEND_SAVE_OUTBOX \n "
#define MAIL_MAIN_590_112_2_18_2_47_44_80 "Mail: MSG_MAIL_SEND_NEXT \n "
#define MAIL_MAIN_595_112_2_18_2_47_44_81 "Mail: MSG_MAIL_OPERATE_SEND_MSG \n "
#define MAIL_MAIN_605_112_2_18_2_47_44_82 "Mail: MSG_MAIL_OPERATE_READ_SORT \n "
#define MAIL_MAIN_611_112_2_18_2_47_44_83 "Mail: MSG_MAIL_OPERATE_SAVE_OUTBOX \n "
#define MAIL_MAIN_620_112_2_18_2_47_45_84 "Mail: MSG_MAIL_OPERATE_DEL_ACCOUNT_MSG \n "
#define MAIL_MAIN_625_112_2_18_2_47_45_85 "Mail: MSG_MAIL_OPERATE_DEL_ALL_ACCOUNT_MSG \n "
#define MAIL_MAIN_630_112_2_18_2_47_45_86 "Mail: MSG_MAIL_OPERATE_BACK_FACTORY \n "
#define MAIL_MAIN_640_112_2_18_2_47_45_87 "Mail:comm_NetTask unkonwn event :%d"
#define MAIL_MAIN_646_112_2_18_2_47_45_88 "Mail: mail_handleParseTaskEvent free event memory \n "
#define MAIL_MAIN_797_112_2_18_2_47_45_89 "MMIMail mail_SendRetrMsgReq PNULL == retr_mode"
#define MAIL_MAIN_842_112_2_18_2_47_45_90 "MMIMail mail_SendRetrEnableReq PNULL == retr_mode"
#define MAIL_MAIN_880_112_2_18_2_47_45_91 "Mail: network invalid"
#define MAIL_MAIN_911_112_2_18_2_47_45_92 "Mail: free space invalid"
#define MAIL_MAIN_933_112_2_18_2_47_45_93 "Mail: box is full 200 mail"
#define MAIL_MAIN_1018_112_2_18_2_47_45_94 "Mail:mail_StartRecvParseMail fldr is not initial"
#define MAIL_MAIN_1370_112_2_18_2_47_46_95 "Mail: mail_SetSendingState state: %d"
#define MAIL_MAIN_1383_112_2_18_2_47_46_96 "mail_GetSendingState: is_sending = %d"
#define MAIL_MAIN_1575_112_2_18_2_47_46_97 "Mail num: total mail:%d  un parsed mail: %d parsing mail: %d"
#define MAIL_MAIN_1588_112_2_18_2_47_46_98 "Mail num: mail is full: %d"
#define MAIL_MAIN_1612_112_2_18_2_47_46_99 "Mail FreeSpace: %d"
#define MAIL_MAIN_1726_112_2_18_2_47_47_100 "Mail: mail_GetNextMailToParse is parsing! num : %d"
#define MAIL_MAIN_1733_112_2_18_2_47_47_101 "Mail: parsing! num : %d "
#define MAIL_MAIN_1747_112_2_18_2_47_47_102 "Mail: unparsed list error"
#define MAIL_MAIN_1780_112_2_18_2_47_47_103 "Mail: mail_parseNextMail\n"
#define MAIL_MAIN_1791_112_2_18_2_47_47_104 "Mail: mail_parseNextMail start parser next msg! \n"
#define MAIL_MAIN_1796_112_2_18_2_47_47_105 "Mail: mail_parseNextMail parser finish!"
#define MAIL_MAIN_1812_112_2_18_2_47_47_106 "Mail: receive attachment num: %d err: %d"
#define MAIL_MAIN_1837_112_2_18_2_47_47_107 "Mail:mail_RetrAttachStartaccount:%d uidl:%s retr_num %d,cont:%s, size:%d"
#define MAIL_MAIN_1851_112_2_18_2_47_47_108 "Mail:mail_RetrAttachStart-no space"
#define MAIL_MAIN_1864_112_2_18_2_47_47_109 "Mail:mail_RetrAttachStart-para error"
#define MAIL_MAIN_1884_112_2_18_2_47_47_110 "Mail:mail_RetrAttachStart-para error"
#define MAIL_MAIN_1898_112_2_18_2_47_47_111 "Mail:mail_RetrAttachStart- att too large!"
#define MAIL_MAIN_1912_112_2_18_2_47_47_112 "Mail:mail_RetrAttachStart- att too large!"
#define MAIL_MAIN_1928_112_2_18_2_47_47_113 "Mail:mail_RetrAttachStart-para error"
#define MAIL_MAIN_1959_112_2_18_2_47_47_114 "Mail:mail_RetrAttachStart-retring"
#define MAIL_MAIN_1973_112_2_18_2_47_47_115 "Mail:mail_RetrAttachStart-retring"
#define MAIL_MAIN_1986_112_2_18_2_47_47_116 "MMIMail mail_RetrAttachStart PNULL == retr_mode"
#define MAIL_MAIN_2010_112_2_18_2_47_47_117 "Mail:mail_CheckPasswordResult result:%d"
#define MAIL_MAIN_2028_112_2_18_2_47_47_118 "Mail:mail_CheckPassWord:%d "
#define MAIL_MAIN_2041_112_2_18_2_47_47_119 "Mail:mail_CheckPassWord-password error"
#define MAIL_MAIN_2048_112_2_18_2_47_47_120 "Mail:mail_CheckPassWord-retring"
#define MAIL_MAIN_2055_112_2_18_2_47_47_121 "Mail:mail_CheckPassWord-retring"
#define MAIL_MAIN_2064_112_2_18_2_47_47_122 "MMIMail mail_CheckPassWord PNULL == retr_mode"
#define MAIL_MAIN_2071_112_2_18_2_47_47_123 "MMIMail mail_CheckPassWord PNULL == pass_word"
#define MAIL_MAIN_2096_112_2_18_2_47_47_124 "MAIL:mail_CancelRetringManual not retring \n"
#define MAIL_MAIN_2116_112_2_18_2_47_47_125 "Mail: mail_CancelSendingManual \n"
#define MAIL_MAIN_2126_112_2_18_2_47_47_126 "Mail: mail_CancelSendingManual  not sending\n"
#define MAIL_MAIN_2217_112_2_18_2_47_48_127 "Mail: mail_AutoRetrNotify retr num: %d \n"
#define MAIL_MAIN_2247_112_2_18_2_47_48_128 "Mail:mail_progressbar %d/%d -%d/%d  - %d"
#define MAIL_MAIN_2281_112_2_18_2_47_48_129 "Mail: mail_SetUserCancelSendingState state: %d"
#define MAIL_SETTING_352_112_2_18_2_47_49_130 "Mail:save settings error!!!!!!!!!!!!!!!!!!"
#define MAIL_SETTING_365_112_2_18_2_47_49_131 "Mail:write settings error!!!!!!!!!!!!!!!!!!"
#define MAIL_SETTING_376_112_2_18_2_47_49_132 "Mail:write settings error len = %d\n"
#define MAIL_SETTING_382_112_2_18_2_47_49_133 "Mail:There no account settings to save"
#define MAIL_SETTING_409_112_2_18_2_47_49_134 "Mail:save settings error!!!!!!!!!!!!!!!!!!"
#define MAIL_SETTING_420_112_2_18_2_47_49_135 "Mail:mail_WriteMailPartCfgfile write settings error"
#define MAIL_SETTING_569_112_2_18_2_47_49_136 "Mail:There no mail settings"
#define MAIL_SETTING_582_112_2_18_2_47_49_137 "Mail:init file read error"
#define MAIL_SETTING_591_112_2_18_2_47_49_138 "Mail:mail settings num:%d-default:%d"
#define MAIL_SETTING_600_112_2_18_2_47_49_139 "Mail:mail_settingInit:%d %d"
#define MAIL_SETTING_611_112_2_18_2_47_49_140 "Mail:file account read:fail %d %d"
#define MAIL_SETTING_624_112_2_18_2_47_49_141 "Mail:mail settings id:%d-user:%s-pass:%s-apn:%s"
#define MAIL_SETTING_628_112_2_18_2_47_49_142 "Mail:There no account settings"
#define MAIL_SETTING_647_112_2_18_2_47_49_143 "Mail:mail_settingFinal"
#define MAIL_SETTING_683_112_2_18_2_47_49_144 "MMIMAIL==> mail_GetDefaultAccount: default account = %d"
#define MAIL_SETTING_688_112_2_18_2_47_49_145 "MMIMAIL==> mail_GetDefaultAccount: can't get default account!"
#define MAIL_SETTING_808_112_2_18_2_47_50_146 "mail_GetDefAccount account_info=%d"
#define MAIL_SETTING_845_112_2_18_2_47_50_147 "mail_GetAccountByIndex account_info=%d"
#define MAIL_SETTING_878_112_2_18_2_47_50_148 "Mail:mail_SetDefAccount invalid account %d"
#define MAIL_SETTING_943_112_2_18_2_47_50_149 "Mail:mail_DeleteAccount account do not exit %d"
#define MAIL_SETTING_999_112_2_18_2_47_50_150 "Mail:mail_DeleteAccount index do not exit %d"
#define MAIL_SETTING_1056_112_2_18_2_47_50_151 "mail_SetAccountInfo account_info=%d"
#define MAIL_SETTING_1090_112_2_18_2_47_50_152 "Mail:mail_SetAccountInfo password %s"
#define MAIL_SETTING_1123_112_2_18_2_47_50_153 "Mail:mail_SetAccountInfo password %s"
#define MAIL_SETTING_1155_112_2_18_2_47_50_154 "Mail:imsi new %s"
#define MAIL_SETTING_1161_112_2_18_2_47_50_155 "Mail:imsi old %s"
#define MAIL_SETTING_1165_112_2_18_2_47_50_156 "Mail:imsi recover %s"
#define MAIL_SETTING_1475_112_2_18_2_47_51_157 "Mail:now is not silence time "
#define MAIL_SETTING_1480_112_2_18_2_47_51_158 "Mail:now is silence time "
#define MAIL_SETTING_1788_112_2_18_2_47_52_159 "Mail:mail_AccountIsSSL ssl = %d\n"
#define MAIL_SETTING_1789_112_2_18_2_47_52_160 "Mail:mail_AccountIsSSL is_mas_account = %d\n"
#define MAIL_SETTING_1790_112_2_18_2_47_52_161 "Mail:mail_AccountIsSSL is_adc_account = %d\n"
#define MAIL_SETTING_1803_112_2_18_2_47_52_162 "Mail:mail_AccountIsSSL  account not exit ssl FALSE\n"
#define MAIL_SETTING_1828_112_2_18_2_47_52_163 "Mail:mail_AccountIsGroupAccount is_mas_account = %d\n"
#define MAIL_SETTING_1829_112_2_18_2_47_52_164 "Mail:mail_AccountIsGroupAccount is_adc_account = %d\n"
#define MAIL_SETTING_1835_112_2_18_2_47_52_165 "Mail:mail_AccountIsGroupAccount  account not exit!accountid = %d"
#define MAIL_SETTING_1864_112_2_18_2_47_52_166 "Mail:imsi no imsi\n"
#define MAIL_SETTING_1874_112_2_18_2_47_52_167 "Mail:imsi change old %s new %s"
#define MAIL_SETTING_1880_112_2_18_2_47_52_168 "Mail:imsi c %s"
#define MAIL_SETTING_1922_112_2_18_2_47_52_169 "MAIL:mail_ReceivingNewMsg account: %d"
#define MAIL_SETTING_1961_112_2_18_2_47_52_170 "Mail:mail_SaveSingleAccount This is a single mas account!"
#define MAIL_SETTING_1969_112_2_18_2_47_52_171 "Mail:mail_SaveSingleAccount This is a single adc account!"
#define MAIL_SETTING_1975_112_2_18_2_47_52_172 "Mail:mail_SaveSingleAccount This is a single 139 mail account!"
#define MAIL_SETTING_2029_112_2_18_2_47_52_173 "Mail:mail_SaveMultiAccount This is a mas account!account_id = %d"
#define MAIL_SETTING_2037_112_2_18_2_47_52_174 "Mail:mail_SaveMultiAccount This is a adc account!account_id = %d"
#define MAIL_SETTING_2043_112_2_18_2_47_52_175 "Mail:mail_SaveMultiAccount This is a 139 mail account!account_id = %d"
#define MAIL_SETTING_2065_112_2_18_2_47_52_176 "MAIL:mail_AccountSettingsArrival new setting"
#define MAIL_SETTING_2069_112_2_18_2_47_52_177 "MAIL:mail_AccountSettingsArrival error"
#define MAIL_SETTING_2118_112_2_18_2_47_52_178 "MAIL:mail_AccountSettingsArrival change card"
#define MAIL_SETTING_2189_112_2_18_2_47_52_179 "MAIL: mail_WriteAllSettings "
#define MAIL_SETTING_2192_112_2_18_2_47_52_180 "Mail: no account num"
#define MAIL_SETTING_2222_112_2_18_2_47_52_181 "Mail:imsi new %s"
#define MAIL_SETTING_2242_112_2_18_2_47_53_182 "Mail: mail_SettinginfoRecved %d"
#define MAIL_SETTING_2260_112_2_18_2_47_53_183 "Mail: mail_SettinginfoRecved %d"
#define MAIL_SETTING_2275_112_2_18_2_47_53_184 "Mail: mail_SettinginfoRecved %d"
#define MAIL_SETTING_2310_112_2_18_2_47_53_185 "Mail:mail_AccountNeedPassword password %s"
#define MAIL_UTIL_222_112_2_18_2_47_53_186 "mail_GetDefAccount address is NULL"
#define MAIL_UTIL_1115_112_2_18_2_47_55_187 "Mail:mail_SearchDigitStr error"
#define MAIL_UTIL_1177_112_2_18_2_47_55_188 "%s %s\n"
#define MAIL_UTIL_1189_112_2_18_2_47_55_189 "%s %s\n"
#define MAIL_UTIL_2754_112_2_18_2_47_59_190 "mail_DateTimeToStr date_str is alloc fail"
#define MAIL_UTIL_2916_112_2_18_2_47_59_191 "mail_DateTimeToStr wstr_ptr||utf8_ptr is NULL "
#define MAIL_UTIL_3030_112_2_18_2_47_59_192 "GUI_UTF82UCS2B: i %d char %x"
#define MAIL_UTIL_3187_112_2_18_2_48_0_193 "mail_StrnToWstr: dst is NULL!"
#define MAIL_WELCOM_57_112_2_18_2_48_5_194 "\"\"\"\"\"\"\"\"\"\"1"
#define MAIL_WELCOM_75_112_2_18_2_48_5_195 "\"\"\"\"\"\"\"\"\"\""
#define MAIL_WELCOM_77_112_2_18_2_48_5_196 "\"\"\"\"\"\"\"\"\"\"%d"
#define MMIMAIL_EDIT_616_112_2_18_2_48_6_197 "MMIMAIL==> MMIAPIMAIL_OpenEditWin: s_mail_edit_global.editbody_ptr is not NULL"
#define MMIMAIL_EDIT_633_112_2_18_2_48_6_198 "MMIMAIL==> MMIAPIMAIL_OpenEditWin: malloc MAIL_MESSAGE_T fail!"
#define MMIMAIL_EDIT_664_112_2_18_2_48_6_199 "MMIMAIL==> MMIAPIMAIL_SendMail: whole_mail_ptr is NULL"
#define MMIMAIL_EDIT_670_112_2_18_2_48_6_200 "MMIMAIL==> MMIAPIMAIL_SendMail: sender is NULL"
#define MMIMAIL_EDIT_674_112_2_18_2_48_7_201 "MMIMAIL==> MMIAPIMAIL_SendMail: Enter send mail! is_inc_org_text is %d"
#define MMIMAIL_EDIT_712_112_2_18_2_48_7_202 "MMIMAIL==> MMIAPIMAIL_SendMail: realloc success"
#define MMIMAIL_EDIT_718_112_2_18_2_48_7_203 "MMIMAIL==> MMIAPIMAIL_SendMail: realloc size [%d] fail"
#define MMIMAIL_EDIT_737_112_2_18_2_48_7_204 "MMIMAIL==> MMIAPIMAIL_SendMail: cur_disp_mail is NULL"
#define MMIMAIL_EDIT_757_112_2_18_2_48_7_205 "MMIMAIL==> MMIAPIMAIL_SendMail: no dest address"
#define MMIMAIL_EDIT_823_112_2_18_2_48_7_206 "MMIMAIL==> MMIAPIMAIL_CancelSendById: current mail id = [%d], state [%d] is valid!"
#define MMIMAIL_EDIT_829_112_2_18_2_48_7_207 "MMIMAIL==> MMIAPIMAIL_CancelSendById: current mail id = [%d], state [%d] is invalid!"
#define MMIMAIL_EDIT_849_112_2_18_2_48_7_208 "MMIMail MMIAPIMAIL_EditMailFromFile full_path_len <= 0 OR full_path_name is NULL"
#define MMIMAIL_EDIT_889_112_2_18_2_48_7_209 "MMIMAIL==> MMIMAIL_SaveFinished: current operate type is %d"
#define MMIMAIL_EDIT_959_112_2_18_2_48_7_210 "MMIMAIL_SendFinished Mail sent succeeded!---MMIMAIL_EDIT_OPER_SEND"
#define MMIMAIL_EDIT_964_112_2_18_2_48_7_211 "MMIMAIL_SendFinished Mail sent failed!---MMIMAIL_EDIT_OPER_SEND"
#define MMIMAIL_EDIT_993_112_2_18_2_48_7_212 "MMIMAIL_SendFinished Mail sent succeeded!---Sending by ID"
#define MMIMAIL_EDIT_998_112_2_18_2_48_7_213 "MMIMAIL_SendFinished Mail sent failed!---Sending by ID"
#define MMIMAIL_EDIT_1028_112_2_18_2_48_7_214 "MMIMail==> MMIMAIL_DestroyEditBody "
#define MMIMAIL_EDIT_1043_112_2_18_2_48_7_215 "MMIMail MMIMAIL_EditInit PNULL == editbody_ptr"
#define MMIMAIL_EDIT_1047_112_2_18_2_48_7_216 "MMIMAIL==> MMIMAIL_EditInit: Open type is [%d]"
#define MMIMAIL_EDIT_1108_112_2_18_2_48_7_217 "MMIMAIL==> MMIMAIL_EditInit: Open type [%d] is invalid!"
#define MMIMAIL_EDIT_1283_112_2_18_2_48_8_218 "MMIMail MMIMAIL_SaveMail PNULL == editbody_ptr"
#define MMIMAIL_EDIT_1295_112_2_18_2_48_8_219 "MMIMAIL==> MMIMAIL_SaveMail: Enter save mail! is_inc_org_text is %d"
#define MMIMAIL_EDIT_1305_112_2_18_2_48_8_220 "MMIMAIL==> MMIMAIL_SaveMail: sender is NULL"
#define MMIMAIL_EDIT_1343_112_2_18_2_48_8_221 "MMIMAIL==> MMIMAIL_SaveMail: realloc success"
#define MMIMAIL_EDIT_1349_112_2_18_2_48_8_222 "MMIMAIL==> MMIMAIL_SaveMail: realloc size [%d] fail"
#define MMIMAIL_EDIT_1354_112_2_18_2_48_8_223 "MMIMAIL==> MMIMAIL_SaveMail: new content size [%d] include org content is larger than 65535"
#define MMIMAIL_EDIT_1386_112_2_18_2_48_8_224 "MMIMAIL==> MMIMAIL_SaveMail: mail is empty, need not save!"
#define MMIMAIL_EDIT_1402_112_2_18_2_48_8_225 "MMIMAIL==> CreateMailEditBody: malloc MAIL_MESSAGE_T"
#define MMIMAIL_EDIT_1409_112_2_18_2_48_8_226 "MMIMAIL==> CreateMailEditBody: malloc from"
#define MMIMAIL_EDIT_1417_112_2_18_2_48_8_227 "MMIMAIL==> CreateMailEditBody: malloc to"
#define MMIMAIL_EDIT_1428_112_2_18_2_48_8_228 "MMIMAIL==> CreateMailEditBody: malloc cc"
#define MMIMAIL_EDIT_1439_112_2_18_2_48_8_229 "MMIMAIL==> CreateMailEditBody: malloc bcc"
#define MMIMAIL_EDIT_1451_112_2_18_2_48_8_230 "MMIMAIL==> CreateMailEditBody: malloc reply to"
#define MMIMAIL_EDIT_1460_112_2_18_2_48_8_231 "MMIMAIL==> CreateMailEditBody: malloc subject"
#define MMIMAIL_EDIT_1464_112_2_18_2_48_8_232 "MMIMAIL==> CreateMailEditBody: malloc content"
#define MMIMAIL_EDIT_1468_112_2_18_2_48_8_233 "MMIMAIL==> CreateMailEditBody: malloc acc"
#define MMIMAIL_EDIT_1479_112_2_18_2_48_8_234 "MMIMAIL==> CreateMailEditBody: total malloc size = %d"
#define MMIMAIL_EDIT_1497_112_2_18_2_48_8_235 "MMIMail==> DestroyMailEditBody"
#define MMIMAIL_EDIT_1694_112_2_18_2_48_9_236 "MMIMAIL==> AddMailSign:content_ptr or content_ptr->wstr_ptr is NULL"
#define MMIMAIL_EDIT_1705_112_2_18_2_48_9_237 "MMIMAIL==> AddMailSign:mail_GetAddSignature return TRUE"
#define MMIMAIL_EDIT_1745_112_2_18_2_48_9_238 "MMIMAIL==> AddMailOrgText:dest_content_ptr or dest_content_ptr->wstr_ptr is NULL"
#define MMIMAIL_EDIT_1751_112_2_18_2_48_9_239 "MMIMAIL==> AddMailOrgText:src_content_ptr or src_content_ptr->wstr_ptr is NULL"
#define MMIMAIL_EDIT_1767_112_2_18_2_48_9_240 "MMIMAIL==> AddMailOrgText: mail_GetReplyWithOriginalCont return TRUE"
#define MMIMAIL_EDIT_1808_112_2_18_2_48_9_241 "MMIMAIL==> CopyMailAcc:edit_mail_ptr OR browser_mail_ptr is NULL"
#define MMIMAIL_EDIT_1811_112_2_18_2_48_9_242 "MMIMAIL==> CopyMailAcc: acc num = %d"
#define MMIMAIL_EDIT_1850_112_2_18_2_48_9_243 "MMIMAIL==> NewMail:edit_mail_ptr is NULL"
#define MMIMAIL_EDIT_1872_112_2_18_2_48_9_244 "MMIMAIL==> CopyExistMail:edit_mail_ptr OR browser_mail_ptr is NULL"
#define MMIMAIL_EDIT_1942_112_2_18_2_48_9_245 "MMIMAIL==> ReplyMail:edit_mail_ptr OR browser_mail_ptr is NULL"
#define MMIMAIL_EDIT_1968_112_2_18_2_48_9_246 "MMIMAIL==> ReplyMail: The original mail subject has include Re: string!"
#define MMIMAIL_EDIT_1991_112_2_18_2_48_9_247 "MMIMAIL==> ReplyMail: malloc temp_str fail!"
#define MMIMAIL_EDIT_2024_112_2_18_2_48_9_248 "MMIMAIL==> ReplyAllMail:edit_mail_ptr OR browser_mail_ptr is NULL"
#define MMIMAIL_EDIT_2088_112_2_18_2_48_9_249 "MMIMAIL==> ReplyAllMail: The original mail subject has include Re: string!"
#define MMIMAIL_EDIT_2111_112_2_18_2_48_9_250 "MMIMAIL==> ReplyAllMail: malloc temp_str fail!"
#define MMIMAIL_EDIT_2140_112_2_18_2_48_9_251 "MMIMAIL==> ForwardAll:edit_mail_ptr OR browser_mail_ptr is NULL"
#define MMIMAIL_EDIT_2166_112_2_18_2_48_9_252 "MMIMAIL==> ForwardAll: malloc temp_str fail!"
#define MMIMAIL_EDIT_2195_112_2_18_2_48_10_253 "MMIMAIL==> ForwardContent:edit_mail_ptr OR browser_mail_ptr is NULL"
#define MMIMAIL_EDIT_2221_112_2_18_2_48_10_254 "MMIMAIL==> ForwardContent: malloc temp_str fail!"
#define MMIMAIL_EDIT_2247_112_2_18_2_48_10_255 "MMIMAIL==> ServerForwardAcc:edit_mail_ptr OR browser_mail_ptr is NULL"
#define MMIMAIL_EDIT_2273_112_2_18_2_48_10_256 "MMIMAIL==> ServerForwardAll: malloc temp_str fail!"
#define MMIMAIL_EDIT_2296_112_2_18_2_48_10_257 "MMIMAIL==> ServerForwardAll:edit_mail_ptr OR browser_mail_ptr is NULL"
#define MMIMAIL_EDIT_2322_112_2_18_2_48_10_258 "MMIMAIL==> ServerForwardAll: malloc temp_str fail!"
#define MMIMAIL_EDIT_2357_112_2_18_2_48_10_259 "MMIMAIL==> GetMailEditCurOper: s_mail_edit_global.cur_oper = %d"
#define MMIMAIL_EDIT_2394_112_2_18_2_48_10_260 "MMIMAIL==> AddEditContact:contact_addr OR index_ptr is NULL"
#define MMIMAIL_EDIT_2399_112_2_18_2_48_10_261 "MMIMail AddEditContact MMIMAIL_RICHTEXT_OP_MAX <= is_add"
#define MMIMAIL_EDIT_2407_112_2_18_2_48_10_262 "MMIMAIL==> AddEditContact: total num = %d"
#define MMIMAIL_EDIT_2411_112_2_18_2_48_10_263 "MMIMAIL==> AddEditContact: receiver is empty!"
#define MMIMAIL_EDIT_2457_112_2_18_2_48_10_264 "MMIMAIL==> AddEditContact: ==error== Email addr [%s] len > 128"
#define MMIMAIL_EDIT_2483_112_2_18_2_48_10_265 "MMIMAIL==> AddEditContact: receiver is empty!"
#define MMIMAIL_EDIT_2570_112_2_18_2_48_10_266 "MMIMAIL==> AddEditContact: ==error== Email addr [%s] len > 128"
#define MMIMAIL_EDIT_2608_112_2_18_2_48_10_267 "MMIMAIL==> AddEditContact: malloc memory fail, malloc size = %d"
#define MMIMAIL_EDIT_2613_112_2_18_2_48_10_268 "MMIMAIL==> AddEditContact: receiver is empty!"
#define MMIMAIL_EDIT_2652_112_2_18_2_48_10_269 "MMIMAIL==> AddEditTo:editbody_ptr OR index_ptr is NULL"
#define MMIMAIL_EDIT_2657_112_2_18_2_48_10_270 "MMIMail AddEditTo MMIMAIL_RICHTEXT_OP_MAX <= is_add"
#define MMIMAIL_EDIT_2661_112_2_18_2_48_10_271 "MMIMAIL==> AddEditTo enter"
#define MMIMAIL_EDIT_2679_112_2_18_2_48_10_272 "MMIMAIL==> AddEditCc:editbody_ptr OR index_ptr is NULL"
#define MMIMAIL_EDIT_2684_112_2_18_2_48_10_273 "MMIMail AddEditCc MMIMAIL_RICHTEXT_OP_MAX <= is_add"
#define MMIMAIL_EDIT_2688_112_2_18_2_48_10_274 "MMIMAIL==> AddEditCc enter"
#define MMIMAIL_EDIT_2706_112_2_18_2_48_11_275 "MMIMAIL==> AddEditBcc:editbody_ptr OR index_ptr is NULL"
#define MMIMAIL_EDIT_2711_112_2_18_2_48_11_276 "MMIMail AddEditBcc MMIMAIL_RICHTEXT_OP_MAX <= is_add"
#define MMIMAIL_EDIT_2715_112_2_18_2_48_11_277 "MMIMAIL==> AddEditBcc enter"
#define MMIMAIL_EDIT_2736_112_2_18_2_48_11_278 "MMIMAIL==> AddEditSubject:editbody_ptr OR index_ptr is NULL"
#define MMIMAIL_EDIT_2741_112_2_18_2_48_11_279 "MMIMail AddEditSubject MMIMAIL_RICHTEXT_OP_MAX <= is_add"
#define MMIMAIL_EDIT_2756_112_2_18_2_48_11_280 "MMIMAIL==> AddEditSubject: subject is empty!"
#define MMIMAIL_EDIT_2801_112_2_18_2_48_11_281 "MMIMAIL==> AddEditContent:editbody_ptr OR index_ptr is NULL"
#define MMIMAIL_EDIT_2806_112_2_18_2_48_11_282 "MMIMail AddEditContent MMIMAIL_RICHTEXT_OP_MAX <= is_add"
#define MMIMAIL_EDIT_2820_112_2_18_2_48_11_283 "MMIMAIL==> AddEditContent: content is empty!"
#define MMIMAIL_EDIT_2932_112_2_18_2_48_11_284 "MMIMAIL==> AddEditContent:editbody_ptr OR index_ptr is NULL"
#define MMIMAIL_EDIT_2937_112_2_18_2_48_11_285 "MMIMail AddEditAcc MMIMAIL_RICHTEXT_OP_MAX <= is_add"
#define MMIMAIL_EDIT_2954_112_2_18_2_48_11_286 "MMIMAIL==> AddEditAcc: acc is empty!"
#define MMIMAIL_EDIT_2993_112_2_18_2_48_11_287 "MMIMAIL==> MailEditEntry:editbody_ptr is NULL"
#define MMIMAIL_EDIT_3044_112_2_18_2_48_11_288 "MMIMAIL==> MailEditEntry: focus_index [%d] is invalid!"
#define MMIMAIL_EDIT_3093_112_2_18_2_48_11_289 "MMIMAIL==> MailEditEnterCorrespondItem: richedit_index [%d] is invalid!"
#define MMIMAIL_EDIT_3111_112_2_18_2_48_11_290 "MMIMail MailInitAccInfo PNULL == editbody_ptr"
#define MMIMAIL_EDIT_3154_112_2_18_2_48_11_291 "MMIMail HandleMailEditMainWinMsg PNULL == editbody_ptr"
#define MMIMAIL_EDIT_3158_112_2_18_2_48_11_292 "MMIMAIL==> HandleMailEditMainWinMsg msg_id = 0x%x"
#define MMIMAIL_EDIT_3274_112_2_18_2_48_12_293 "MMIMail==> HandleMailEditMainWinMsg: GetMailEditCurOper()=%d "
#define MMIMAIL_EDIT_3279_112_2_18_2_48_12_294 "MMIMail==> HandleMailEditMainWinMsg: close window , is_other_free is TRUE"
#define MMIMAIL_EDIT_3284_112_2_18_2_48_12_295 "MMIMail==> HandleMailEditMainWinMsg: close window , is_other_free is FALSE"
#define MMIMAIL_EDIT_3333_112_2_18_2_48_12_296 "MMIMAIL==> HandleMailEditOptWinMsg, msg_id = 0x%x"
#define MMIMAIL_EDIT_3436_112_2_18_2_48_12_297 "MMIMAIL==> HandleMailEditOptWinMsg, menu_id = %d is invalid"
#define MMIMAIL_EDIT_3622_112_2_18_2_48_12_298 "MMIMail SetRecverList PNULL == editbody_ptr"
#define MMIMAIL_EDIT_3644_112_2_18_2_48_12_299 "MMIMAIL==> SetRecverList: current edit item [%d] is invalid"
#define MMIMAIL_EDIT_3650_112_2_18_2_48_12_300 "MMIMAIL==> SetRecverList: mail_addr_list:[%d],addr_count:[%d]"
#define MMIMAIL_EDIT_3683_112_2_18_2_48_12_301 "MMIMail SaveRecverList PNULL == editbody_ptr"
#define MMIMAIL_EDIT_3705_112_2_18_2_48_12_302 "MMIMAIL==> SaveRecverList: current edit item [%d] is invalid"
#define MMIMAIL_EDIT_3724_112_2_18_2_48_13_303 "MMIMail SaveRecverList PNULL == item_str"
#define MMIMAIL_EDIT_3764_112_2_18_2_48_13_304 "MMIMAIL==> SaveRecverList: list_num = %d, clean all receivers address"
#define MMIMAIL_EDIT_3870_112_2_18_2_48_13_305 "MMIMAIL==> GetPbSelReturn: malloc all_number_wstr failed"
#define MMIMAIL_EDIT_3884_112_2_18_2_48_13_306 "MMIMAIL==> GetPbSelReturn: malloc num_str_ptr failed"
#define MMIMAIL_EDIT_3914_112_2_18_2_48_13_307 "MMIMAIL==> GetPbSelReturn: malloc email_addr failed"
#define MMIMAIL_EDIT_3979_112_2_18_2_48_13_308 "MMIMail HandleMailEditRecverListWinMsg PNULL == editbody_ptr"
#define MMIMAIL_EDIT_4042_112_2_18_2_48_13_309 "MMIMAIL==> HandleMailEditRecverListWinMsg: alloc GUIEDIT_LIST_ITEM_STR_T fail"
#define MMIMAIL_EDIT_4093_112_2_18_2_48_14_310 "MMIMail SetMailText PNULL == editbody_ptr"
#define MMIMAIL_EDIT_4123_112_2_18_2_48_14_311 "MMIMAIL==> SetMailText: current edit item [%d] is invalid"
#define MMIMAIL_EDIT_4150_112_2_18_2_48_14_312 "MMIMail SaveMailText PNULL == editbody_ptr"
#define MMIMAIL_EDIT_4167_112_2_18_2_48_14_313 "MMIMAIL==> SaveMailText: current edit item [%d] is invalid"
#define MMIMAIL_EDIT_4196_112_2_18_2_48_14_314 "MMIMail HandleMailEditTextWinMsg PNULL == editbody_ptr"
#define MMIMAIL_EDIT_4200_112_2_18_2_48_14_315 "MMIMAIL==> HandleMailEditTextWinMsg msg_id = 0x%x"
#define MMIMAIL_EDIT_4255_112_2_18_2_48_14_316 "MMIMail AppendEditAccListItem filename_len <= 0"
#define MMIMAIL_EDIT_4261_112_2_18_2_48_14_317 "MMIMail AppendEditAccListItem PNULL == filename"
#define MMIMAIL_EDIT_4302_112_2_18_2_48_14_318 "MMIMail SetEditAccList PNULL == editbody_ptr"
#define MMIMAIL_EDIT_4419_112_2_18_2_48_14_319 "MMIMail SaveSelectAccFile PNULL == editbody_ptr"
#define MMIMAIL_EDIT_4425_112_2_18_2_48_14_320 "MMIMAIL==> SaveSelectAccFile: selected_info is NULL"
#define MMIMAIL_EDIT_4479_112_2_18_2_48_14_321 "MMIMAIL==> SaveSelectAccFile: free_index = %d"
#define MMIMAIL_EDIT_4501_112_2_18_2_48_14_322 "MMIMail DelSelectAcc PNULL == editbody_ptr"
#define MMIMAIL_EDIT_4582_112_2_18_2_48_14_323 "MMIMAIL==> DelSelectAcc: index = %d is invalid"
#define MMIMAIL_EDIT_4588_112_2_18_2_48_15_324 "MMIMAIL==> DelSelectAcc: current edit acc is null!"
#define MMIMAIL_EDIT_4589_112_2_18_2_48_15_325 "MMIMAIL==> DelSelectAcc: editbody_ptr->partcount = %d"
#define MMIMAIL_EDIT_4614_112_2_18_2_48_15_326 "MMIMail HandleEditAccListWinMsg PNULL == editbody_ptr"
#define MMIMAIL_EDIT_4685_112_2_18_2_48_15_327 "MMIMAIL==> HandleEditAccListWinMsg: param is NULL"
#define MMIMAIL_EDIT_4765_112_2_18_2_48_15_328 "MMIMail HandleEditAccMenuOptWinMsg node_id = %d"
#define MMIMAIL_READ_691_112_2_18_2_48_17_329 "MMIMAIL==> MMIMAIL_ReadFinished: post MMIMAIL_MAILBOX_MAIN_WIN_ID ==> MSG_MAIL_FINISH_READ"
#define MMIMAIL_READ_698_112_2_18_2_48_17_330 "MMIMAIL==> MMIMAIL_ReadFinished: MMIMAIL_MAILBOX_MAIN_WIN_ID is not open"
#define MMIMAIL_READ_712_112_2_18_2_48_17_331 "MMIMAIL==> MMIMAIL_ReadFinished: open_type : %d, record_id : %d"
#define MMIMAIL_READ_718_112_2_18_2_48_18_332 "MMIMAIL==> MMIMAIL_ReadFinished: get mail ptr fail!"
#define MMIMAIL_READ_746_112_2_18_2_48_18_333 "MMIMAIL==> MMIMAIL_ReadFinished: mail open type is %d"
#define MMIMAIL_READ_752_112_2_18_2_48_18_334 "MMIMAIL==> MMIMAIL_ReadFinished: whole_mail_ptr is NULL!"
#define MMIMAIL_READ_776_112_2_18_2_48_18_335 "MMIMAIL==> MMIMAIL_GetIconByType: type [%d] is invalid"
#define MMIMAIL_READ_816_112_2_18_2_48_18_336 "MMIMAIL==> HandleMailReadWinMsg, msg_id = 0x%x"
#define MMIMAIL_READ_989_112_2_18_2_48_18_337 "MMIMAIL==> HandleReadMailOptWinMsg, msg_id = 0x%x"
#define MMIMAIL_READ_1005_112_2_18_2_48_18_338 "MMIMAIL==> HandleReadMailOptWinMsg: node_id = %d"
#define MMIMAIL_READ_1037_112_2_18_2_48_18_339 "MMIMail HandleReadMailOptMenuId PNULL == whole_mail_ptr"
#define MMIMAIL_READ_1100_112_2_18_2_48_18_340 "MMIMail HandleReadInboxOptMenuId PNULL == whole_mail_ptr"
#define MMIMAIL_READ_1163_112_2_18_2_48_18_341 "MMIMail HandleReadOutboxOptMenuId PNULL == whole_mail_ptr"
#define MMIMAIL_READ_1179_112_2_18_2_48_18_342 "MMIMAIL==> HandleReadOutboxOptMenuId: current operation mail is NULL!"
#define MMIMAIL_READ_1220_112_2_18_2_48_19_343 "MMIMail HandleReadSentboxOptMenuId PNULL == whole_mail_ptr"
#define MMIMAIL_READ_1260_112_2_18_2_48_19_344 "MMIMAIL==> HandleMailReadAllContentWinMsg, msg_id = 0x%x"
#define MMIMAIL_READ_1296_112_2_18_2_48_19_345 "MMIMAIL==> HandleReadWaitingWinMsg, msg_id = 0x%x"
#define MMIMAIL_READ_1334_112_2_18_2_48_19_346 "MMIMAIL==> HandleAccPicurePreview, msg_id = 0x%x"
#define MMIMAIL_READ_1417_112_2_18_2_48_19_347 "MMIMAIL==> HandleAccessoryOpen enter\n"
#define MMIMAIL_READ_1423_112_2_18_2_48_19_348 "MMIMAIL==> HandleAccessoryOpen full_path_name = %s\n"
#define MMIMAIL_READ_1437_112_2_18_2_48_19_349 "MMIMAIL==> HandleAccessoryOpen file_suffix = %s\n"
#define MMIMAIL_READ_1544_112_2_18_2_48_19_350 "MMIMAIL==> HandleAccessoryOpen ptr=%x\n"
#define MMIMAIL_READ_1586_112_2_18_2_48_19_351 "MMIMAIL==> HandleAccessoryOpen entry_para.url_ptr=%s\n"
#define MMIMAIL_READ_1614_112_2_18_2_48_19_352 "MMIMAIL==> HandleAccessoryOpen file type not support!\n"
#define MMIMAIL_READ_1639_112_2_18_2_48_19_353 "MMIMail HandleReadAccessoryListWinMsg PNULL == whole_mail_ptr"
#define MMIMAIL_READ_1643_112_2_18_2_48_19_354 "MMIMAIL==> HandleReadAccessoryListWinMsg msg_id = 0x%x, have_items = %d\n"
#define MMIMAIL_READ_1728_112_2_18_2_48_19_355 "MMIMAIL==> HandleReadAccessoryListWinMsg: full_path_name is NULL"
#define MMIMAIL_READ_1795_112_2_18_2_48_20_356 "MMIMail HandleReadAccExpandListWinMsg PNULL == whole_mail_ptr"
#define MMIMAIL_READ_1799_112_2_18_2_48_20_357 "MMIMAIL==> HandleReadAccExpandListWinMsg, msg_id = 0x%x"
#define MMIMAIL_READ_1839_112_2_18_2_48_20_358 "MMIMail HandleReadAccExpandListWinMsg PNULL == cur_acc_ptr"
#define MMIMAIL_READ_1862_112_2_18_2_48_20_359 "MMIMail HandleReadAccExpandListWinMsg PNULL == cur_acc_ptr"
#define MMIMAIL_READ_1910_112_2_18_2_48_20_360 "MMIMAIL==> HandleReadAccMenuOptWinMsg, msg_id = 0x%x"
#define MMIMAIL_READ_1931_112_2_18_2_48_20_361 "MMIMail HandleReadAccMenuOptWinMsg node_id = %d"
#define MMIMAIL_READ_1964_112_2_18_2_48_20_362 "MMIMail HandleSaveOrDlAccessory PNULL == whole_mail_ptr"
#define MMIMAIL_READ_1970_112_2_18_2_48_20_363 "MMIMail HandleSaveOrDlAccessory PNULL == acc_ptr"
#define MMIMAIL_READ_2019_112_2_18_2_48_20_364 "MMIMAIL==> HandleDlWaitingWinMsg, msg_id = 0x%x"
#define MMIMAIL_READ_2063_112_2_18_2_48_20_365 "PUSH MAIL  AddMailSender PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2107_112_2_18_2_48_20_366 "MMIMAIL==> AddMailSender: sender is empty!"
#define MMIMAIL_READ_2134_112_2_18_2_48_20_367 "PUSH MAIL  AddMailAccount PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2186_112_2_18_2_48_20_368 "PUSH MAIL  AddMailContact: PNULL == contact_addr ||PNULL == index_ptr "
#define MMIMAIL_READ_2193_112_2_18_2_48_20_369 "MMIMAIL==> AddMailContact: total num = %d"
#define MMIMAIL_READ_2197_112_2_18_2_48_20_370 "MMIMAIL==> AddMailContact: receiver is empty!"
#define MMIMAIL_READ_2237_112_2_18_2_48_21_371 "MMIMAIL==> AddMailContact: ==error== Email addr [%s] len > 128"
#define MMIMAIL_READ_2329_112_2_18_2_48_21_372 "MMIMAIL==> AddMailContact: ==error== Email addr [%s] len > 128"
#define MMIMAIL_READ_2355_112_2_18_2_48_21_373 "MMIMAIL==> AddMailContact: malloc memory fail, malloc size = %d"
#define MMIMAIL_READ_2360_112_2_18_2_48_21_374 "MMIMAIL==> AddMailContact: receiver is empty!"
#define MMIMAIL_READ_2391_112_2_18_2_48_21_375 "PUSH MAIL  AddMailTo: PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2395_112_2_18_2_48_21_376 "MMIMAIL==> AddMailTo enter"
#define MMIMAIL_READ_2412_112_2_18_2_48_21_377 "PUSH MAIL  AddMailCc: PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2416_112_2_18_2_48_21_378 "MMIMAIL==> AddMailCc enter"
#define MMIMAIL_READ_2433_112_2_18_2_48_21_379 "PUSH MAIL  AddMailBcc: PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2437_112_2_18_2_48_21_380 "MMIMAIL==> AddMailBcc enter"
#define MMIMAIL_READ_2460_112_2_18_2_48_21_381 "PUSH MAIL  AddMailTime: PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2487_112_2_18_2_48_21_382 "MMIMAIL==> AddMailTime: time is empty!"
#define MMIMAIL_READ_2506_112_2_18_2_48_21_383 "PUSH MAIL  AddMailSubject: PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2527_112_2_18_2_48_21_384 "MMIMAIL==> AddMailSubject: subject is empty!"
#define MMIMAIL_READ_2564_112_2_18_2_48_21_385 "PUSH MAIL  AddMailAccessory: PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2629_112_2_18_2_48_21_386 "MMIMAIL==> AddMailAccessory: malloc memory fail, malloc size = %d"
#define MMIMAIL_READ_2634_112_2_18_2_48_21_387 "MMIMAIL==> AddMailAccessory: accessory is empty!"
#define MMIMAIL_READ_2653_112_2_18_2_48_21_388 "PUSH MAIL  AddMailSeperator: PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2684_112_2_18_2_48_21_389 "PUSH MAIL  AddMailContent: PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2726_112_2_18_2_48_21_390 "MMIMAIL==> AddMailContent: content is empty!"
#define MMIMAIL_READ_2744_112_2_18_2_48_22_391 "PUSH MAIL  ShowMailContent: PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2790_112_2_18_2_48_22_392 "PUSH MAIL  ShowAllMailContent: PNULL == whole_mail_ptr ||PNULL == index_ptr "
#define MMIMAIL_READ_2929_112_2_18_2_48_22_393 "MMIMAIL==> OpenMailReadOptWin:current box type is invalid!"
#define MMIMAIL_READ_2948_112_2_18_2_48_22_394 "MMIMail OpenAccessoryOptWin PNULL == acc_ptr"
#define MMIMAIL_READ_3046_112_2_18_2_48_22_395 "MMIMAIL==> SwitchToNextMail: no next mail!"
#define MMIMAIL_READ_3051_112_2_18_2_48_22_396 "MMIMAIL==> SwitchToNextMail: cur_mail_ptr is NULL!"
#define MMIMAIL_READ_3075_112_2_18_2_48_22_397 "MMIMail AppendAccessoryListItem filename_len <= 0 OR PNULL == filename"
#define MMIMAIL_READ_3126_112_2_18_2_48_22_398 "MMIMail ReplaceAccessoryListItem filename_len <=  0 OR  PNULL == filename"
#define MMIMAIL_READ_3274_112_2_18_2_48_23_399 "MMIMAIL==> UpdateMailReadState: current operation mail ptr is NULL"
#define MMIMAIL_READ_3278_112_2_18_2_48_23_400 "MMIMAIL==> UpdateMailReadState: record_id = %d, cur_mail->record_id = %d"
#define MMIMAIL_READ_3318_112_2_18_2_48_23_401 "MMIMAIL==> MMIMAIL_GetCurOperExpandAcc: Error!"
#define MMIMAIL_READ_3361_112_2_18_2_48_23_402 "MMIMail MMIMAIL_IsAccessoryDownloaded PNULL == whole_mail_ptr OR PNULL == accessory_ptr"
#define MMIMAIL_READ_3370_112_2_18_2_48_23_403 "MMIMAIL==> MMIMAIL_IsAccessoryDownloaded: result = %d!"
#define MMIMAIL_READ_3390_112_2_18_2_48_23_404 "MMIMail MMIMAIL_IsAccessoryAutoChanged PNULL == whole_mail_ptr OR PNULL == accessory_ptr"
#define MMIMAIL_READ_3399_112_2_18_2_48_23_405 "MMIMAIL==> MMIMAIL_IsAccessoryAutoChanged: changed num = %d!"
#define MMIMAIL_READ_3434_112_2_18_2_48_23_406 "MMIMAIL_LoadAccessoryList whole_mail_ptr is NULL"
#define MMIMAIL_READ_3479_112_2_18_2_48_23_407 "MMIMAIL_LoadAccExpandList whole_mail_ptr || acc_ptr is NULL"
#define MMIMAIL_READ_3491_112_2_18_2_48_23_408 "MMIMail MMIMAIL_LoadAccExpandList PNULL == list_ptr"
#define MMIMAIL_READ_3550_112_2_18_2_48_23_409 "AppendRecverListItem mail_addr is NULL"
#define MMIMAIL_READ_3593_112_2_18_2_48_23_410 "MMIMAIL_LoadRecverList whole_mail_ptr is NULL"
#define MMIMAIL_READ_3689_112_2_18_2_48_23_411 "MMIMail HandleReadAllRecverListWinMsg PNULL == whole_mail_ptr"
#define MMIMAIL_READ_3693_112_2_18_2_48_23_412 "MMIMAIL==> HandleReadAllRecverListWinMsg, msg_id = 0x%x"
#define MMIMAIL_READ_3715_112_2_18_2_48_23_413 "MMIMail HandleReadAllRecverListWinMsg PNULL == s_cur_recver_list"
#define MMIMAIL_READ_3793_112_2_18_2_48_24_414 "MMIMAIL==> HandleReadRecvListOptWin, msg_id = 0x%x"
#define MMIMAIL_READ_3827_112_2_18_2_48_24_415 "MMIMAIL==> HandleReadRecvListOptWin, menu_id = %d is invalid"
#define MMIMAIL_READ_3861_112_2_18_2_48_24_416 "MMIMAIL==> HandleReadSelGroupWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_1677_112_2_18_2_48_28_417 "Mail:MMIAPIMAIL_Enter change sim and no new settings"
#define MMIMAIL_WINTAB_1757_112_2_18_2_48_28_418 "Mail: MMIAPIMAIL_GetAccountCallback cur sim is a white sim card!"
#define MMIMAIL_WINTAB_1839_112_2_18_2_48_28_419 "MMIMAIL==> MMIAPIMAIL_IsHaveUnreadMail: unread_num = %d"
#define MMIMAIL_WINTAB_1868_112_2_18_2_48_28_420 "MMIMAIL==> MMIAPIMAIL_SetSmsOk: is_sms_ok = %d"
#define MMIMAIL_WINTAB_1878_112_2_18_2_48_28_421 "MMIMAIL==> MMIMAIL_IsReady: return = %d"
#define MMIMAIL_WINTAB_1888_112_2_18_2_48_28_422 "MMIMAIL==> MMIMAIL_GetOpenType: return = %d"
#define MMIMAIL_WINTAB_1900_112_2_18_2_48_28_423 "MMIMAIL==> MMIMAIL_GetOpenParam: return = %d"
#define MMIMAIL_WINTAB_1904_112_2_18_2_48_28_424 "MMIMAIL==> MMIMAIL_GetOpenParam: return 0"
#define MMIMAIL_WINTAB_1915_112_2_18_2_48_28_425 "MMIMAIL==> MMIMAIL_GetDefAccountId: return = %d"
#define MMIMAIL_WINTAB_2121_112_2_18_2_48_28_426 "MMIMAIL==> MMIMAIL_GetMailState: mail_ptr is NULL"
#define MMIMAIL_WINTAB_2142_112_2_18_2_48_28_427 "MMIMAIL==> MMIMAIL_SetMailState: mail_ptr is NULL"
#define MMIMAIL_WINTAB_2200_112_2_18_2_48_29_428 "MMIMAIL==> MMIMAIL_OpenDelteQueryWin: delete_mail_num = 0,no need delete!"
#define MMIMAIL_WINTAB_2257_112_2_18_2_48_29_429 "MMIMAIL==> MMIMAIL_OpenDelteQueryWin: delete_mail_num = 0,no need delete!"
#define MMIMAIL_WINTAB_2330_112_2_18_2_48_29_430 "MMIMAIL==> MMIMAIL_GetMailPtrByRecordId: record_id = %d"
#define MMIMAIL_WINTAB_2364_112_2_18_2_48_29_431 "MMIMAIL==> MMIAPIMAIL_IsSmsOpen: is_sms_open = %d"
#define MMIMAIL_WINTAB_2613_112_2_18_2_48_29_432 "MMIMAIL_GetDefAccount account_info is null"
#define MMIMAIL_WINTAB_2638_112_2_18_2_48_29_433 "MMIMAIL_GetDefAccount account_info OR mailbox_info is null"
#define MMIMAIL_WINTAB_2798_112_2_18_2_48_30_434 "MMIMAIL==> MMIMAIL_CloseInitWaitingWindow: s_mmimail_info.enter_type = %d"
#define MMIMAIL_WINTAB_2829_112_2_18_2_48_30_435 "MMIMAIL==> HandleWaitGetAccountWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_2885_112_2_18_2_48_30_436 "MMIMAIL==> HandleOverwriteQueryWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_2924_112_2_18_2_48_30_437 "MMIMAIL==> HandleDelExistAccountQueryWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_2964_112_2_18_2_48_30_438 "MMIMAIL==> HandleInitWaitingWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3017_112_2_18_2_48_30_439 "MMIMAIL==> HandleDelQueryWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3054_112_2_18_2_48_30_440 "MMIMAIL==> HandleDelAllInCurBoxQueryWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3093_112_2_18_2_48_30_441 "MMIMAIL==> HandleDelWaitWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3155_112_2_18_2_48_31_442 "HandleMailTaskMsg app_ptr is null"
#define MMIMAIL_WINTAB_3159_112_2_18_2_48_31_443 "MMIMAIL==> HandleMailTaskMsg: msg_id = 0x%x"
#define MMIMAIL_WINTAB_3288_112_2_18_2_48_31_444 "MMIMAIL==> HandleMailWelcomeWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3352_112_2_18_2_48_31_445 "MMIMAIL==> HandleMailApplyAlertWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3393_112_2_18_2_48_31_446 "MMIMAIL==> HandleMailMainMenuWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3497_112_2_18_2_48_31_447 "MMIMAIL==> HandleMailMainMenuOptWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3532_112_2_18_2_48_31_448 "MMIMail HandleMailMainMenuOptWinMsg menu_id = %d"
#define MMIMAIL_WINTAB_3563_112_2_18_2_48_31_449 "MMIMAIL==> HandleMailBoxMainWindow, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3634_112_2_18_2_48_31_450 "MMIMAIL==> HandleInBoxChildWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3666_112_2_18_2_48_32_451 "MMIMail HandleInBoxChildWinMsg PNULL == need_item_data_ptr"
#define MMIMAIL_WINTAB_3766_112_2_18_2_48_32_452 "MMIMAIL==> HandleOutBoxChildWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3798_112_2_18_2_48_32_453 "MMIMail HandleOutBoxChildWinMsg PNULL == need_item_data_ptr"
#define MMIMAIL_WINTAB_3896_112_2_18_2_48_32_454 "MMIMAIL==> HandleSentBoxChildWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_3928_112_2_18_2_48_32_455 "MMIMail HandleSentBoxChildWinMsg PNULL == need_item_data_ptr"
#define MMIMAIL_WINTAB_4025_112_2_18_2_48_32_456 "MMIMAIL==> HandleDraftChildWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_4057_112_2_18_2_48_32_457 "MMIMail HandleDraftChildWinMsg PNULL == need_item_data_ptr"
#define MMIMAIL_WINTAB_4292_112_2_18_2_48_33_458 "MMIMail HandleDraftChildWinMsg PNULL == cur_mail_ptr"
#define MMIMAIL_WINTAB_4338_112_2_18_2_48_33_459 "MMIMAIL==> HandleMailListOkMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_4391_112_2_18_2_48_33_460 "HandleMailListOkMsg: Get current operating mail fail!"
#define MMIMAIL_WINTAB_4477_112_2_18_2_48_33_461 "MMIMail HandleMailBoxOptMenuId PNULL == cur_mail_ptr"
#define MMIMAIL_WINTAB_4512_112_2_18_2_48_33_462 "MMIMail HandleMailBoxOptMenuId PNULL == cur_mail_ptr"
#define MMIMAIL_WINTAB_4670_112_2_18_2_48_34_463 "MMIMAIL==> HandleOutboxOptMenuId: current operation mail is NULL!"
#define MMIMAIL_WINTAB_4735_112_2_18_2_48_34_464 "MMIMail HandleSentboxOptMenuId PNULL == cur_mail_ptr"
#define MMIMAIL_WINTAB_4956_112_2_18_2_48_34_465 "MMIMAIL==> HandleInboxMenuOptWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_4982_112_2_18_2_48_34_466 "MMIMAIL==> HandleInboxMenuOptWinMsg: menu_id = %d"
#define MMIMAIL_WINTAB_5015_112_2_18_2_48_34_467 "MMIMAIL==> HandleOutboxMenuOptWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_5041_112_2_18_2_48_34_468 "MMIMAIL==> HandleOutboxMenuOptWinMsg: menu_id = %d"
#define MMIMAIL_WINTAB_5074_112_2_18_2_48_34_469 "MMIMAIL==> HandleOutboxMenuOptExtWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_5099_112_2_18_2_48_34_470 "MMIMAIL==> HandleOutboxMenuOptExtWinMsg: menu_id = %d"
#define MMIMAIL_WINTAB_5132_112_2_18_2_48_34_471 "MMIMAIL==> HandleSentboxMenuOptWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_5158_112_2_18_2_48_34_472 "MMIMAIL==> HandleSentboxMenuOptWinMsg: menu_id = %d"
#define MMIMAIL_WINTAB_5191_112_2_18_2_48_35_473 "MMIMAIL==> HandleDraftboxMenuOptWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_5217_112_2_18_2_48_35_474 "MMIMAIL==> HandleDraftboxMenuOptWinMsg: menu_id = %d"
#define MMIMAIL_WINTAB_5270_112_2_18_2_48_35_475 "MMIMAIL==> ShowWelcomeInfo: charge_info->wstr_ptr = %d, charge_info->wstr_len = %d"
#define MMIMAIL_WINTAB_5275_112_2_18_2_48_35_476 "MMIMAIL==> ShowWelcomeInfo: charge_info is null"
#define MMIMAIL_WINTAB_5294_112_2_18_2_48_35_477 "MMIMail ShowWelcomeInfo MMIMAIL_WELCOME_CONTENT_LEN <= uint16_str_len"
#define MMIMAIL_WINTAB_5315_112_2_18_2_48_35_478 "MMIMAIL==> StartMailMenuWinTimer: the mainmenu win timer has started!"
#define MMIMAIL_WINTAB_5332_112_2_18_2_48_35_479 "MMIMAIL==> StopMailMenuWinTimer: the mainmenu win timer has stop!"
#define MMIMAIL_WINTAB_5683_112_2_18_2_48_35_480 "MMIMAIL==> EnterMailCorrespondingMenu node_id = %d"
#define MMIMAIL_WINTAB_5725_112_2_18_2_48_36_481 "StartReadMail ctrl_id is 0"
#define MMIMAIL_WINTAB_5762_112_2_18_2_48_36_482 "MMIMAIL==> MMIMAIL_Enter: %s"
#define MMIMAIL_WINTAB_5766_112_2_18_2_48_36_483 "MMIMAIL==> MMIMAIL_Enter: no local account!"
#define MMIMAIL_WINTAB_5852_112_2_18_2_48_36_484 "MMIMAIL==> MMIMAIL_Enter: enter type [%d] is invalid!"
#define MMIMAIL_WINTAB_5911_112_2_18_2_48_36_485 "MMIMAIL==> MMIMAIL_GetBoxTotalNumber: box_type = %d is invalid"
#define MMIMAIL_WINTAB_5940_112_2_18_2_48_36_486 "MMIMAIL==> MMIMAIL_GetInboxBoxUnreadNum: unread_num = %d"
#define MMIMAIL_WINTAB_5959_112_2_18_2_48_36_487 "MMIMAIL==> MMIMAIL_LoadMailList the count of list box is %d"
#define MMIMAIL_WINTAB_6019_112_2_18_2_48_36_488 "MMIMAIL==> MMIMAIL_GetMailListHead: the box_type is %d"
#define MMIMAIL_WINTAB_6118_112_2_18_2_48_36_489 "MMIMAIL_AppendListItem string_ptr is NULL"
#define MMIMAIL_WINTAB_6525_112_2_18_2_48_37_490 "MMIMAIL==> MMIMAIL_GetCurMailIsMarked: cur_mail_ptr is null !"
#define MMIMAIL_WINTAB_6548_112_2_18_2_48_37_491 "MMIMAIL==> MMIMAIL_SetCurMailMarked: cur_mail_ptr is null!"
#define MMIMAIL_WINTAB_6687_112_2_18_2_48_38_492 "MMIMAIL==> MMIMAIL_GetMailboxDeleteAvailNum delete_mail_num=%d"
#define MMIMAIL_WINTAB_6754_112_2_18_2_48_38_493 "MMIMAIL==> MMIMAIL_HandleRetrAttachResult: retr_result = %d"
#define MMIMAIL_WINTAB_6790_112_2_18_2_48_38_494 "MMIMAIL==> MMIMAIL_HandleRetrAttachResult: MMIMAIL_DL_ACCESSORY_WAITING_WIN_ID and MMIMAIL_RECV_MAIL_PROGRESS_WIN_ID is not open!"
#define MMIMAIL_WINTAB_6791_112_2_18_2_48_38_495 "MMIMAIL==> MMIMAIL_HandleRetrAttachResult: retr_result = %d"
#define MMIMAIL_WINTAB_6794_112_2_18_2_48_38_496 "MMIMAIL==> MMIMAIL_HandleRetrAttachResult Full Paint ACCESSORY WIN!"
#define MMIMAIL_WINTAB_6811_112_2_18_2_48_38_497 "MMIMAIL==> MMIMAIL_CheckPwFinished: is_success = %d"
#define MMIMAIL_WINTAB_6818_112_2_18_2_48_38_498 "MMIMAIL==> MMIMAIL_CheckPwFinished: edit password window"
#define MMIMAIL_WINTAB_6824_112_2_18_2_48_38_499 "MMIMAIL==> MMIMAIL_CheckPwFinished: switch account window"
#define MMIMAIL_WINTAB_6830_112_2_18_2_48_38_500 "MMIMAIL==> MMIMAIL_CheckPwFinished: enter app"
#define MMIMAIL_WINTAB_6841_112_2_18_2_48_38_501 "MMIMAIL==> MMIMAIL_CheckPwFinished: account num = %d"
#define MMIMAIL_WINTAB_6846_112_2_18_2_48_38_502 "MMIMAIL==> MMIMAIL_CheckPwFinished: edit password window"
#define MMIMAIL_WINTAB_6852_112_2_18_2_48_38_503 "MMIMAIL==> MMIMAIL_CheckPwFinished: switch account window"
#define MMIMAIL_WINTAB_6858_112_2_18_2_48_38_504 "MMIMAIL==> MMIMAIL_CheckPwFinished: enter app"
#define MMIMAIL_WINTAB_6876_112_2_18_2_48_38_505 "MMIMAIL==> MMIMAIL_DelFinished: Account del finish by Bomb!"
#define MMIMAIL_WINTAB_6922_112_2_18_2_48_38_506 "MMIMAIL==> MMIMAIL_DelFinished: Account logoff finish!"
#define MMIMAIL_WINTAB_6930_112_2_18_2_48_38_507 "MMIMAIL==> MMIMAIL_DelFinished: Account del finish by Bomb!"
#define MMIMAIL_WINTAB_7041_112_2_18_2_48_38_508 "MMIMAIL==> MMIMAIL_ProgressUpdateCallBack: cur_progress = %d, total_progress = %d"
#define MMIMAIL_WINTAB_7053_112_2_18_2_48_38_509 "MMIMAIL==> MMIMAIL_ProgressUpdateCallBack: progress_info is PNULL"
#define MMIMAIL_WINTAB_7088_112_2_18_2_48_38_510 "MMIMAIL_IsHaveReceiver cur_mail_ptr is NULL"
#define MMIMAIL_WINTAB_7178_112_2_18_2_48_39_511 "MMIMAIL==> MMIMAIL_LoadAccountList: def_acc_id = %d error!"
#define MMIMAIL_WINTAB_7500_112_2_18_2_48_39_512 "MMIMAIL==> HandleSetAccountWinMsg: current select account id = %d"
#define MMIMAIL_WINTAB_7526_112_2_18_2_48_39_513 "MMIMAIL==> HandleSetAccountWinMsg: receive MSG_MAIL_CHECK_PASSWD_RESULT"
#define MMIMAIL_WINTAB_7640_112_2_18_2_48_40_514 "MMIMAIL==> HandleSetAccountOptWinMsg, msg_id = 0x%x"
#define MMIMAIL_WINTAB_7940_112_2_18_2_48_40_515 "MMIMAIL==> MMIMAIL_LoadAccountEditItem: focus item is %d"
#define MMIMAIL_WINTAB_8149_112_2_18_2_48_41_516 "MMIMAIL==> HandleMailEditAccountPwWinMsg: receive MSG_MAIL_CHECK_PASSWD_RESULT, is_success = %d"
#define MMIMAIL_WINTAB_8155_112_2_18_2_48_41_517 "MMIMAIL==> HandleMailEditAccountPwWinMsg: tmp_str.wstr_len = %d"
#define MMIMAIL_WINTAB_8210_112_2_18_2_48_41_518 "MMIMAIL==> HandleMailInputPwWinMsg: add_data = %d, acc_id = %d"
#define MMIMAIL_WINTAB_8528_112_2_18_2_48_41_519 "AddSilentTime index_ptr || time is NULL"
#define MMIMAIL_WINTAB_8605_112_2_18_2_48_41_520 "MMIMAIL==> SetSilentTime: focus item is %d"
#define MMIMAIL_WINTAB_8728_112_2_18_2_48_42_521 "SaveSilentTime:hour is %d,minute is %d is invalid!"
#define MMIMAIL_WINTAB_9454_112_2_18_2_48_44_522 "Mail:MMIMAIL_HandleNewMsgWin do not allow in mail"
#define MMIMAIL_WINTAB_9505_112_2_18_2_48_44_523 "[MMISET]:MMIAPISET_AlertRingPlayResult: result = %d"
#define MMIMAIL_WINTAB_9583_112_2_18_2_48_44_524 "MAIL: MMIMAIL_PlayMailSound now silence mode"
#define MMIMAIL_WINTAB_9600_112_2_18_2_48_44_525 "MAIL: PlaymailRing vt mode"
#define MMIMAIL_WINTAB_9605_112_2_18_2_48_44_526 "MAIL: PlaymailRing normal call mode"
#define MMIMAIL_WINTAB_9610_112_2_18_2_48_44_527 "MAIL: PlaymailRing ring_type=%d"
#define MMIMAIL_WINTAB_9769_112_2_18_2_48_45_528 "MMIMail_NewMailArrival No space "
#define MMIMAIL_WINTAB_9777_112_2_18_2_48_45_529 "MMIMail_NewMailArrival box is full"
#define MMIMAIL_WINTAB_9784_112_2_18_2_48_45_530 "MMIMail_NewMailArrival recv 0 mail "
#define MMIMAIL_WINTAB_9799_112_2_18_2_48_45_531 "MMIMail_NewMailArrival in mail box "
#define MMIMAIL_WINTAB_9854_112_2_18_2_48_45_532 "MMIMail_NewMailArrival "
#define MMIMAIL_WINTAB_9981_112_2_18_2_48_45_533 "HandleCopyAccWaitWinMsg  copy_result=%d"
#define MMIMAIL_WINTAB_10045_112_2_18_2_48_45_534 "select_foler_full_path = "
#define MMIMAIL_WINTAB_10048_112_2_18_2_48_45_535 "%d:%x "
#define MMIMAIL_WINTAB_10052_112_2_18_2_48_45_536 "file_size = "
#define MMIMAIL_WINTAB_10091_112_2_18_2_48_45_537 "MMIAPIFMM_CopyOneFile ret=%d"
#define MMIMAIL_WINTAB_10167_112_2_18_2_48_45_538 "MMIMAIL==> MMIAPIFMM_CopyAccToFolder: Enter"
#define MMIMAIL_WINTAB_10174_112_2_18_2_48_45_539 "MMIMAIL==> MMIAPIFMM_CopyAccToFolder: alloc s_selected_file_ptr fail!"
#define MMIMAIL_WINTAB_10184_112_2_18_2_48_45_540 "s_acc_file_ptr = "
#define MMIMAIL_WINTAB_10187_112_2_18_2_48_45_541 "%d:%x "
#define MMIMAIL_WINTAB_10239_112_2_18_2_48_46_542 "MMIMAIL==> MMIAPIFMM_CopyAccToFolder: alloc s_copy_del_path_data_ptr fail!"
#define MMIMAIL_WINTAB_10279_112_2_18_2_48_46_543 "MMIMAIL==> MMIAPIFMM_FreeAccTmpSpace: Enter"
#define MMIMAIL_WINTAB_10315_112_2_18_2_48_46_544 "Mail:MMIMAILPDP_Active apn:%s"
#define MMIMAIL_WINTAB_10383_112_2_18_2_48_46_545 "[MMISMS ATC] MMIAPIMAIL_InsertMessage result %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_PUSH_MAIL_TRC)
TRACE_MSG(MAIL_EXPORT_134_112_2_18_2_47_37_0,"Mail:username id : %d:%s password: %s")
TRACE_MSG(MAIL_EXPORT_285_112_2_18_2_47_37_1,"Mail:invalid account id:%d")
TRACE_MSG(MAIL_EXPORT_306_112_2_18_2_47_37_2,"Mail:invalid account id:%d")
TRACE_MSG(MAIL_EXPORT_325_112_2_18_2_47_37_3,"Mail: %s")
TRACE_MSG(MAIL_EXPORT_329_112_2_18_2_47_37_4,"Mail:start receive msg-err")
TRACE_MSG(MAIL_EXPORT_335_112_2_18_2_47_37_5,"Mail:start receive msg-id:%d, name:%s")
TRACE_MSG(MAIL_EXPORT_340_112_2_18_2_47_37_6,"Mail:invalid account id:%d")
TRACE_MSG(MAIL_EXPORT_346_112_2_18_2_47_37_7,"Mail:mail_retrievePushMsg-retring \n")
TRACE_MSG(MAIL_EXPORT_352_112_2_18_2_47_37_8,"Mail:mail_retrievePushMsg-sending \n")
TRACE_MSG(MAIL_EXPORT_378_112_2_18_2_47_37_9,"Mail:mail_RetrAttachStart-retring")
TRACE_MSG(MAIL_EXPORT_394_112_2_18_2_47_38_10,"Mail:mail_RetrAttachStart-retring")
TRACE_MSG(MAIL_EXPORT_410_112_2_18_2_47_38_11,"Mail:invalid account id:%d")
TRACE_MSG(MAIL_EXPORT_416_112_2_18_2_47_38_12,"Mail:mail_RetrAttachStart-retring")
TRACE_MSG(MAIL_EXPORT_452_112_2_18_2_47_38_13,"Mail:invalid account id:%d")
TRACE_MSG(MAIL_EXPORT_459_112_2_18_2_47_38_14,"Mail:invalid uidl")
TRACE_MSG(MAIL_EXPORT_482_112_2_18_2_47_38_15,"Mail:is mail, do not allow u disk")
TRACE_MSG(MAIL_EXPORT_489_112_2_18_2_47_38_16,"Mail:is receiving mail, do not allow u disk")
TRACE_MSG(MAIL_EXPORT_496_112_2_18_2_47_38_17,"Mail:is writing mail, do not allow u disk")
TRACE_MSG(MAIL_EXPORT_659_112_2_18_2_47_38_18,"Mail:MAIL_ListInit no sim card or switch sim card")
TRACE_MSG(MAIL_EXPORT_841_112_2_18_2_47_39_19,"MAIL_InsertMsgToSMS:msgId = %d")
TRACE_MSG(MAIL_EXPORT_842_112_2_18_2_47_39_20,"MAIL_InsertMsgToSMS:record_id = %d")
TRACE_MSG(MAIL_EXPORT_956_112_2_18_2_47_39_21,"mail_GetMailLockFlag:index < 0")
TRACE_MSG(MAIL_EXPORT_982_112_2_18_2_47_39_22,"mail_GetMailSendingFlag:index < 0")
TRACE_MSG(MAIL_EXPORT_1008_112_2_18_2_47_39_23,"mail_GetMailSecurityFlag:index < 0")
TRACE_MSG(MAIL_EXPORT_1034_112_2_18_2_47_39_24,"mail_GetMailSecurityFlag:index < 0")
TRACE_MSG(MAIL_EXPORT_1075_112_2_18_2_47_39_25,"mail_fldr mail_delMessageByMsgIdOtherApp del msg(%d)start! \n")
TRACE_MSG(MAIL_EXPORT_1081_112_2_18_2_47_39_26,"mail_fldr mail_delMessageByMsgIdOtherApp:index-msgId not matched")
TRACE_MSG(MAIL_EXPORT_1088_112_2_18_2_47_39_27,"mail_fldr mail_delMessageByMsgIdOtherApp:delete msg not found or msg list not found")
TRACE_MSG(MAIL_EXPORT_1092_112_2_18_2_47_39_28,"mail_fldr mail_delMessageByMsgIdOtherApp delete msg accountId = %d \n")
TRACE_MSG(MAIL_EXPORT_1093_112_2_18_2_47_39_29,"mail_fldr mail_delMessageByMsgIdOtherApp cur accountId = \n")
TRACE_MSG(MAIL_EXPORT_1096_112_2_18_2_47_39_30,"mail_fldr mail_delMessageByMsgIdOtherApp mail->uidl=%s \n")
TRACE_MSG(MAIL_EXPORT_1320_112_2_18_2_47_40_31,"mail_MoveMailToSecurityBoxEx!")
TRACE_MSG(MAIL_EXPORT_1343_112_2_18_2_47_40_32,"mail_delSMSMessageListById:msg_id = %d")
TRACE_MSG(MAIL_EXPORT_1344_112_2_18_2_47_40_33,"mail_delSMSMessageListById:record_id = %d")
TRACE_MSG(MAIL_FILE_340_112_2_18_2_47_41_34,"Mail: mail_FileOperationWriteBuf: buffer %s \n")
TRACE_MSG(MAIL_FILE_356_112_2_18_2_47_41_35,"Mail: mail_FileOperationWriteBuf: written %d \n")
TRACE_MSG(MAIL_FILE_690_112_2_18_2_47_42_36,"MMIMail mail_createFolder full_path_len <= 0")
TRACE_MSG(MAIL_FILE_736_112_2_18_2_47_42_37,"mail_fldr mail_FileOperationRead set pointer fail\n")
TRACE_MSG(MAIL_FILE_743_112_2_18_2_47_42_38,"mail_fldr mail_FileOperationRead error=%d\n")
TRACE_MSG(MAIL_FILE_744_112_2_18_2_47_42_39,"mail_fldr mail_FileOperationRead read_Size=%d\n")
TRACE_MSG(MAIL_FILE_745_112_2_18_2_47_42_40,"mail_fldr mail_FileOperationRead bufSize=%d\n")
TRACE_MSG(MAIL_FILE_753_112_2_18_2_47_42_41,"mail_fldr mail_FileOperationRead  bufsize=%d,read_size=%d\n")
TRACE_MSG(MAIL_FILE_757_112_2_18_2_47_42_42,"mail_fldr mail_FileOperationRead error=%d\n")
TRACE_MSG(MAIL_FILE_758_112_2_18_2_47_42_43,"mail_fldr mail_FileOperationRead read_Size=%d\n")
TRACE_MSG(MAIL_FILE_759_112_2_18_2_47_42_44,"mail_fldr mail_FileOperationRead bufSize=%d\n")
TRACE_MSG(MAIL_FILE_807_112_2_18_2_47_42_45,"mail_fldr mail_FileOperationWrite bufsize=%d,buf_write=%d \n")
TRACE_MSG(MAIL_FILE_937_112_2_18_2_47_42_46,"mail_RenameFileByWchar srcName=%d,dstName =%d")
TRACE_MSG(MAIL_FILE_995_112_2_18_2_47_42_47,"mail_RenameFileByWchar oldName=%d,newName =%d")
TRACE_MSG(MAIL_FILE_1036_112_2_18_2_47_43_48,"mail_fldr mail_RenameFile del old fail!\n")
TRACE_MSG(MAIL_FILE_1049_112_2_18_2_47_43_49,"mail_fldr mail_RenameFile res=%d\n")
TRACE_MSG(MAIL_FILE_1082_112_2_18_2_47_43_50,"mail_FileNameCmp FileName1=%d, FileName2 =%d")
TRACE_MSG(MAIL_FILE_1150_112_2_18_2_47_43_51,"mail_GetFileNumOfDir")
TRACE_MSG(MAIL_FILE_1198_112_2_18_2_47_43_52,"mail_RemoveFile FileName=%d")
TRACE_MSG(MAIL_FILE_1235_112_2_18_2_47_43_53,"mail_fldr mail_sentMailWriteFile start write file(%s) buffer_size=%d \n")
TRACE_MSG(MAIL_FILE_1243_112_2_18_2_47_43_54,"mail_fldr mail_sentMailWriteFile create file(%s)fail!\n")
TRACE_MSG(MAIL_FILE_1316_112_2_18_2_47_43_55,"Mail:delet folder exit\n")
TRACE_MSG(MAIL_FILE_1367_112_2_18_2_47_43_56,"Mail:mail_GetDirFileNum %d\n")
TRACE_MSG(MAIL_MAIN_159_112_2_18_2_47_44_57,"MMIMail mail_SendSignaltoMailTask PNULL == sig_ptr")
TRACE_MSG(MAIL_MAIN_169_112_2_18_2_47_44_58,"MAIL: mail_SendSignaltoMailTask sender:%d cur: %d .event: %x\n")
TRACE_MSG(MAIL_MAIN_199_112_2_18_2_47_44_59,"MAIL: mail_SendSignaltoMailTask sender:%d cur: %d .event: %x\n")
TRACE_MSG(MAIL_MAIN_225_112_2_18_2_47_44_60,"MMIMail mail_SendSignaltoMailParseTask PNULL == sig_ptr")
TRACE_MSG(MAIL_MAIN_234_112_2_18_2_47_44_61,"MAIL: mail_SendSignaltoMailParseTask sender:%d cur: %d .event: %x\n")
TRACE_MSG(MAIL_MAIN_258_112_2_18_2_47_44_62,"MMIMail mail_handleTaskEvent PNULL == sig_ptr")
TRACE_MSG(MAIL_MAIN_262_112_2_18_2_47_44_63,"Mail: mail_handleTaskEvent signal id: %x ")
TRACE_MSG(MAIL_MAIN_284_112_2_18_2_47_44_64,"Mail:start resend!!!!\n")
TRACE_MSG(MAIL_MAIN_316_112_2_18_2_47_44_65,"Mail: MSG_MAIL_TIME_OUT mail recv timeout!")
TRACE_MSG(MAIL_MAIN_336_112_2_18_2_47_44_66,"Mail: MSG_MAIL_RETR_START is retring ")
TRACE_MSG(MAIL_MAIN_340_112_2_18_2_47_44_67,"Mail: MSG_MAIL_RETR_START Reset g_mail_is_recv_timeout to FALSE!")
TRACE_MSG(MAIL_MAIN_374_112_2_18_2_47_44_68,"Mail: comm_NetTask data arrival ")
TRACE_MSG(MAIL_MAIN_381_112_2_18_2_47_44_69,"Mail: MSG_MAIL_DATA_ARRIVAL ")
TRACE_MSG(MAIL_MAIN_388_112_2_18_2_47_44_70,"Mail: comm_NetTask data can send ")
TRACE_MSG(MAIL_MAIN_395_112_2_18_2_47_44_71,"Mail: MSG_MAIL_DATA_SEND ")
TRACE_MSG(MAIL_MAIN_404_112_2_18_2_47_44_72,"Mail: comm_NetTask connect error = %d ")
TRACE_MSG(MAIL_MAIN_413_112_2_18_2_47_44_73,"Mail: comm_NetTask server close this connection err: %d\n ")
TRACE_MSG(MAIL_MAIN_424_112_2_18_2_47_44_74,"Mail:comm_NetTask unkonwn event :%d")
TRACE_MSG(MAIL_MAIN_457_112_2_18_2_47_44_75,"MMIMail mail_handleParseTaskEvent PNULL == sig_ptr")
TRACE_MSG(MAIL_MAIN_461_112_2_18_2_47_44_76,"Mail: mail_handleParseTaskEvent signal id: %x ")
TRACE_MSG(MAIL_MAIN_471_112_2_18_2_47_44_77,"Mail: MSG_MAIL_HEADER_START \n ")
TRACE_MSG(MAIL_MAIN_482_112_2_18_2_47_44_78,"Mail: MSG_MAIL_HEADER_FINISH  \n")
TRACE_MSG(MAIL_MAIN_578_112_2_18_2_47_44_79,"Mail: MSG_MAIL_OPERATE_SEND_SAVE_OUTBOX \n ")
TRACE_MSG(MAIL_MAIN_590_112_2_18_2_47_44_80,"Mail: MSG_MAIL_SEND_NEXT \n ")
TRACE_MSG(MAIL_MAIN_595_112_2_18_2_47_44_81,"Mail: MSG_MAIL_OPERATE_SEND_MSG \n ")
TRACE_MSG(MAIL_MAIN_605_112_2_18_2_47_44_82,"Mail: MSG_MAIL_OPERATE_READ_SORT \n ")
TRACE_MSG(MAIL_MAIN_611_112_2_18_2_47_44_83,"Mail: MSG_MAIL_OPERATE_SAVE_OUTBOX \n ")
TRACE_MSG(MAIL_MAIN_620_112_2_18_2_47_45_84,"Mail: MSG_MAIL_OPERATE_DEL_ACCOUNT_MSG \n ")
TRACE_MSG(MAIL_MAIN_625_112_2_18_2_47_45_85,"Mail: MSG_MAIL_OPERATE_DEL_ALL_ACCOUNT_MSG \n ")
TRACE_MSG(MAIL_MAIN_630_112_2_18_2_47_45_86,"Mail: MSG_MAIL_OPERATE_BACK_FACTORY \n ")
TRACE_MSG(MAIL_MAIN_640_112_2_18_2_47_45_87,"Mail:comm_NetTask unkonwn event :%d")
TRACE_MSG(MAIL_MAIN_646_112_2_18_2_47_45_88,"Mail: mail_handleParseTaskEvent free event memory \n ")
TRACE_MSG(MAIL_MAIN_797_112_2_18_2_47_45_89,"MMIMail mail_SendRetrMsgReq PNULL == retr_mode")
TRACE_MSG(MAIL_MAIN_842_112_2_18_2_47_45_90,"MMIMail mail_SendRetrEnableReq PNULL == retr_mode")
TRACE_MSG(MAIL_MAIN_880_112_2_18_2_47_45_91,"Mail: network invalid")
TRACE_MSG(MAIL_MAIN_911_112_2_18_2_47_45_92,"Mail: free space invalid")
TRACE_MSG(MAIL_MAIN_933_112_2_18_2_47_45_93,"Mail: box is full 200 mail")
TRACE_MSG(MAIL_MAIN_1018_112_2_18_2_47_45_94,"Mail:mail_StartRecvParseMail fldr is not initial")
TRACE_MSG(MAIL_MAIN_1370_112_2_18_2_47_46_95,"Mail: mail_SetSendingState state: %d")
TRACE_MSG(MAIL_MAIN_1383_112_2_18_2_47_46_96,"mail_GetSendingState: is_sending = %d")
TRACE_MSG(MAIL_MAIN_1575_112_2_18_2_47_46_97,"Mail num: total mail:%d  un parsed mail: %d parsing mail: %d")
TRACE_MSG(MAIL_MAIN_1588_112_2_18_2_47_46_98,"Mail num: mail is full: %d")
TRACE_MSG(MAIL_MAIN_1612_112_2_18_2_47_46_99,"Mail FreeSpace: %d")
TRACE_MSG(MAIL_MAIN_1726_112_2_18_2_47_47_100,"Mail: mail_GetNextMailToParse is parsing! num : %d")
TRACE_MSG(MAIL_MAIN_1733_112_2_18_2_47_47_101,"Mail: parsing! num : %d ")
TRACE_MSG(MAIL_MAIN_1747_112_2_18_2_47_47_102,"Mail: unparsed list error")
TRACE_MSG(MAIL_MAIN_1780_112_2_18_2_47_47_103,"Mail: mail_parseNextMail\n")
TRACE_MSG(MAIL_MAIN_1791_112_2_18_2_47_47_104,"Mail: mail_parseNextMail start parser next msg! \n")
TRACE_MSG(MAIL_MAIN_1796_112_2_18_2_47_47_105,"Mail: mail_parseNextMail parser finish!")
TRACE_MSG(MAIL_MAIN_1812_112_2_18_2_47_47_106,"Mail: receive attachment num: %d err: %d")
TRACE_MSG(MAIL_MAIN_1837_112_2_18_2_47_47_107,"Mail:mail_RetrAttachStartaccount:%d uidl:%s retr_num %d,cont:%s, size:%d")
TRACE_MSG(MAIL_MAIN_1851_112_2_18_2_47_47_108,"Mail:mail_RetrAttachStart-no space")
TRACE_MSG(MAIL_MAIN_1864_112_2_18_2_47_47_109,"Mail:mail_RetrAttachStart-para error")
TRACE_MSG(MAIL_MAIN_1884_112_2_18_2_47_47_110,"Mail:mail_RetrAttachStart-para error")
TRACE_MSG(MAIL_MAIN_1898_112_2_18_2_47_47_111,"Mail:mail_RetrAttachStart- att too large!")
TRACE_MSG(MAIL_MAIN_1912_112_2_18_2_47_47_112,"Mail:mail_RetrAttachStart- att too large!")
TRACE_MSG(MAIL_MAIN_1928_112_2_18_2_47_47_113,"Mail:mail_RetrAttachStart-para error")
TRACE_MSG(MAIL_MAIN_1959_112_2_18_2_47_47_114,"Mail:mail_RetrAttachStart-retring")
TRACE_MSG(MAIL_MAIN_1973_112_2_18_2_47_47_115,"Mail:mail_RetrAttachStart-retring")
TRACE_MSG(MAIL_MAIN_1986_112_2_18_2_47_47_116,"MMIMail mail_RetrAttachStart PNULL == retr_mode")
TRACE_MSG(MAIL_MAIN_2010_112_2_18_2_47_47_117,"Mail:mail_CheckPasswordResult result:%d")
TRACE_MSG(MAIL_MAIN_2028_112_2_18_2_47_47_118,"Mail:mail_CheckPassWord:%d ")
TRACE_MSG(MAIL_MAIN_2041_112_2_18_2_47_47_119,"Mail:mail_CheckPassWord-password error")
TRACE_MSG(MAIL_MAIN_2048_112_2_18_2_47_47_120,"Mail:mail_CheckPassWord-retring")
TRACE_MSG(MAIL_MAIN_2055_112_2_18_2_47_47_121,"Mail:mail_CheckPassWord-retring")
TRACE_MSG(MAIL_MAIN_2064_112_2_18_2_47_47_122,"MMIMail mail_CheckPassWord PNULL == retr_mode")
TRACE_MSG(MAIL_MAIN_2071_112_2_18_2_47_47_123,"MMIMail mail_CheckPassWord PNULL == pass_word")
TRACE_MSG(MAIL_MAIN_2096_112_2_18_2_47_47_124,"MAIL:mail_CancelRetringManual not retring \n")
TRACE_MSG(MAIL_MAIN_2116_112_2_18_2_47_47_125,"Mail: mail_CancelSendingManual \n")
TRACE_MSG(MAIL_MAIN_2126_112_2_18_2_47_47_126,"Mail: mail_CancelSendingManual  not sending\n")
TRACE_MSG(MAIL_MAIN_2217_112_2_18_2_47_48_127,"Mail: mail_AutoRetrNotify retr num: %d \n")
TRACE_MSG(MAIL_MAIN_2247_112_2_18_2_47_48_128,"Mail:mail_progressbar %d/%d -%d/%d  - %d")
TRACE_MSG(MAIL_MAIN_2281_112_2_18_2_47_48_129,"Mail: mail_SetUserCancelSendingState state: %d")
TRACE_MSG(MAIL_SETTING_352_112_2_18_2_47_49_130,"Mail:save settings error!!!!!!!!!!!!!!!!!!")
TRACE_MSG(MAIL_SETTING_365_112_2_18_2_47_49_131,"Mail:write settings error!!!!!!!!!!!!!!!!!!")
TRACE_MSG(MAIL_SETTING_376_112_2_18_2_47_49_132,"Mail:write settings error len = %d\n")
TRACE_MSG(MAIL_SETTING_382_112_2_18_2_47_49_133,"Mail:There no account settings to save")
TRACE_MSG(MAIL_SETTING_409_112_2_18_2_47_49_134,"Mail:save settings error!!!!!!!!!!!!!!!!!!")
TRACE_MSG(MAIL_SETTING_420_112_2_18_2_47_49_135,"Mail:mail_WriteMailPartCfgfile write settings error")
TRACE_MSG(MAIL_SETTING_569_112_2_18_2_47_49_136,"Mail:There no mail settings")
TRACE_MSG(MAIL_SETTING_582_112_2_18_2_47_49_137,"Mail:init file read error")
TRACE_MSG(MAIL_SETTING_591_112_2_18_2_47_49_138,"Mail:mail settings num:%d-default:%d")
TRACE_MSG(MAIL_SETTING_600_112_2_18_2_47_49_139,"Mail:mail_settingInit:%d %d")
TRACE_MSG(MAIL_SETTING_611_112_2_18_2_47_49_140,"Mail:file account read:fail %d %d")
TRACE_MSG(MAIL_SETTING_624_112_2_18_2_47_49_141,"Mail:mail settings id:%d-user:%s-pass:%s-apn:%s")
TRACE_MSG(MAIL_SETTING_628_112_2_18_2_47_49_142,"Mail:There no account settings")
TRACE_MSG(MAIL_SETTING_647_112_2_18_2_47_49_143,"Mail:mail_settingFinal")
TRACE_MSG(MAIL_SETTING_683_112_2_18_2_47_49_144,"MMIMAIL==> mail_GetDefaultAccount: default account = %d")
TRACE_MSG(MAIL_SETTING_688_112_2_18_2_47_49_145,"MMIMAIL==> mail_GetDefaultAccount: can't get default account!")
TRACE_MSG(MAIL_SETTING_808_112_2_18_2_47_50_146,"mail_GetDefAccount account_info=%d")
TRACE_MSG(MAIL_SETTING_845_112_2_18_2_47_50_147,"mail_GetAccountByIndex account_info=%d")
TRACE_MSG(MAIL_SETTING_878_112_2_18_2_47_50_148,"Mail:mail_SetDefAccount invalid account %d")
TRACE_MSG(MAIL_SETTING_943_112_2_18_2_47_50_149,"Mail:mail_DeleteAccount account do not exit %d")
TRACE_MSG(MAIL_SETTING_999_112_2_18_2_47_50_150,"Mail:mail_DeleteAccount index do not exit %d")
TRACE_MSG(MAIL_SETTING_1056_112_2_18_2_47_50_151,"mail_SetAccountInfo account_info=%d")
TRACE_MSG(MAIL_SETTING_1090_112_2_18_2_47_50_152,"Mail:mail_SetAccountInfo password %s")
TRACE_MSG(MAIL_SETTING_1123_112_2_18_2_47_50_153,"Mail:mail_SetAccountInfo password %s")
TRACE_MSG(MAIL_SETTING_1155_112_2_18_2_47_50_154,"Mail:imsi new %s")
TRACE_MSG(MAIL_SETTING_1161_112_2_18_2_47_50_155,"Mail:imsi old %s")
TRACE_MSG(MAIL_SETTING_1165_112_2_18_2_47_50_156,"Mail:imsi recover %s")
TRACE_MSG(MAIL_SETTING_1475_112_2_18_2_47_51_157,"Mail:now is not silence time ")
TRACE_MSG(MAIL_SETTING_1480_112_2_18_2_47_51_158,"Mail:now is silence time ")
TRACE_MSG(MAIL_SETTING_1788_112_2_18_2_47_52_159,"Mail:mail_AccountIsSSL ssl = %d\n")
TRACE_MSG(MAIL_SETTING_1789_112_2_18_2_47_52_160,"Mail:mail_AccountIsSSL is_mas_account = %d\n")
TRACE_MSG(MAIL_SETTING_1790_112_2_18_2_47_52_161,"Mail:mail_AccountIsSSL is_adc_account = %d\n")
TRACE_MSG(MAIL_SETTING_1803_112_2_18_2_47_52_162,"Mail:mail_AccountIsSSL  account not exit ssl FALSE\n")
TRACE_MSG(MAIL_SETTING_1828_112_2_18_2_47_52_163,"Mail:mail_AccountIsGroupAccount is_mas_account = %d\n")
TRACE_MSG(MAIL_SETTING_1829_112_2_18_2_47_52_164,"Mail:mail_AccountIsGroupAccount is_adc_account = %d\n")
TRACE_MSG(MAIL_SETTING_1835_112_2_18_2_47_52_165,"Mail:mail_AccountIsGroupAccount  account not exit!accountid = %d")
TRACE_MSG(MAIL_SETTING_1864_112_2_18_2_47_52_166,"Mail:imsi no imsi\n")
TRACE_MSG(MAIL_SETTING_1874_112_2_18_2_47_52_167,"Mail:imsi change old %s new %s")
TRACE_MSG(MAIL_SETTING_1880_112_2_18_2_47_52_168,"Mail:imsi c %s")
TRACE_MSG(MAIL_SETTING_1922_112_2_18_2_47_52_169,"MAIL:mail_ReceivingNewMsg account: %d")
TRACE_MSG(MAIL_SETTING_1961_112_2_18_2_47_52_170,"Mail:mail_SaveSingleAccount This is a single mas account!")
TRACE_MSG(MAIL_SETTING_1969_112_2_18_2_47_52_171,"Mail:mail_SaveSingleAccount This is a single adc account!")
TRACE_MSG(MAIL_SETTING_1975_112_2_18_2_47_52_172,"Mail:mail_SaveSingleAccount This is a single 139 mail account!")
TRACE_MSG(MAIL_SETTING_2029_112_2_18_2_47_52_173,"Mail:mail_SaveMultiAccount This is a mas account!account_id = %d")
TRACE_MSG(MAIL_SETTING_2037_112_2_18_2_47_52_174,"Mail:mail_SaveMultiAccount This is a adc account!account_id = %d")
TRACE_MSG(MAIL_SETTING_2043_112_2_18_2_47_52_175,"Mail:mail_SaveMultiAccount This is a 139 mail account!account_id = %d")
TRACE_MSG(MAIL_SETTING_2065_112_2_18_2_47_52_176,"MAIL:mail_AccountSettingsArrival new setting")
TRACE_MSG(MAIL_SETTING_2069_112_2_18_2_47_52_177,"MAIL:mail_AccountSettingsArrival error")
TRACE_MSG(MAIL_SETTING_2118_112_2_18_2_47_52_178,"MAIL:mail_AccountSettingsArrival change card")
TRACE_MSG(MAIL_SETTING_2189_112_2_18_2_47_52_179,"MAIL: mail_WriteAllSettings ")
TRACE_MSG(MAIL_SETTING_2192_112_2_18_2_47_52_180,"Mail: no account num")
TRACE_MSG(MAIL_SETTING_2222_112_2_18_2_47_52_181,"Mail:imsi new %s")
TRACE_MSG(MAIL_SETTING_2242_112_2_18_2_47_53_182,"Mail: mail_SettinginfoRecved %d")
TRACE_MSG(MAIL_SETTING_2260_112_2_18_2_47_53_183,"Mail: mail_SettinginfoRecved %d")
TRACE_MSG(MAIL_SETTING_2275_112_2_18_2_47_53_184,"Mail: mail_SettinginfoRecved %d")
TRACE_MSG(MAIL_SETTING_2310_112_2_18_2_47_53_185,"Mail:mail_AccountNeedPassword password %s")
TRACE_MSG(MAIL_UTIL_222_112_2_18_2_47_53_186,"mail_GetDefAccount address is NULL")
TRACE_MSG(MAIL_UTIL_1115_112_2_18_2_47_55_187,"Mail:mail_SearchDigitStr error")
TRACE_MSG(MAIL_UTIL_1177_112_2_18_2_47_55_188,"%s %s\n")
TRACE_MSG(MAIL_UTIL_1189_112_2_18_2_47_55_189,"%s %s\n")
TRACE_MSG(MAIL_UTIL_2754_112_2_18_2_47_59_190,"mail_DateTimeToStr date_str is alloc fail")
TRACE_MSG(MAIL_UTIL_2916_112_2_18_2_47_59_191,"mail_DateTimeToStr wstr_ptr||utf8_ptr is NULL ")
TRACE_MSG(MAIL_UTIL_3030_112_2_18_2_47_59_192,"GUI_UTF82UCS2B: i %d char %x")
TRACE_MSG(MAIL_UTIL_3187_112_2_18_2_48_0_193,"mail_StrnToWstr: dst is NULL!")
TRACE_MSG(MAIL_WELCOM_57_112_2_18_2_48_5_194,"\"\"\"\"\"\"\"\"\"\"1")
TRACE_MSG(MAIL_WELCOM_75_112_2_18_2_48_5_195,"\"\"\"\"\"\"\"\"\"\"")
TRACE_MSG(MAIL_WELCOM_77_112_2_18_2_48_5_196,"\"\"\"\"\"\"\"\"\"\"%d")
TRACE_MSG(MMIMAIL_EDIT_616_112_2_18_2_48_6_197,"MMIMAIL==> MMIAPIMAIL_OpenEditWin: s_mail_edit_global.editbody_ptr is not NULL")
TRACE_MSG(MMIMAIL_EDIT_633_112_2_18_2_48_6_198,"MMIMAIL==> MMIAPIMAIL_OpenEditWin: malloc MAIL_MESSAGE_T fail!")
TRACE_MSG(MMIMAIL_EDIT_664_112_2_18_2_48_6_199,"MMIMAIL==> MMIAPIMAIL_SendMail: whole_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_670_112_2_18_2_48_6_200,"MMIMAIL==> MMIAPIMAIL_SendMail: sender is NULL")
TRACE_MSG(MMIMAIL_EDIT_674_112_2_18_2_48_7_201,"MMIMAIL==> MMIAPIMAIL_SendMail: Enter send mail! is_inc_org_text is %d")
TRACE_MSG(MMIMAIL_EDIT_712_112_2_18_2_48_7_202,"MMIMAIL==> MMIAPIMAIL_SendMail: realloc success")
TRACE_MSG(MMIMAIL_EDIT_718_112_2_18_2_48_7_203,"MMIMAIL==> MMIAPIMAIL_SendMail: realloc size [%d] fail")
TRACE_MSG(MMIMAIL_EDIT_737_112_2_18_2_48_7_204,"MMIMAIL==> MMIAPIMAIL_SendMail: cur_disp_mail is NULL")
TRACE_MSG(MMIMAIL_EDIT_757_112_2_18_2_48_7_205,"MMIMAIL==> MMIAPIMAIL_SendMail: no dest address")
TRACE_MSG(MMIMAIL_EDIT_823_112_2_18_2_48_7_206,"MMIMAIL==> MMIAPIMAIL_CancelSendById: current mail id = [%d], state [%d] is valid!")
TRACE_MSG(MMIMAIL_EDIT_829_112_2_18_2_48_7_207,"MMIMAIL==> MMIAPIMAIL_CancelSendById: current mail id = [%d], state [%d] is invalid!")
TRACE_MSG(MMIMAIL_EDIT_849_112_2_18_2_48_7_208,"MMIMail MMIAPIMAIL_EditMailFromFile full_path_len <= 0 OR full_path_name is NULL")
TRACE_MSG(MMIMAIL_EDIT_889_112_2_18_2_48_7_209,"MMIMAIL==> MMIMAIL_SaveFinished: current operate type is %d")
TRACE_MSG(MMIMAIL_EDIT_959_112_2_18_2_48_7_210,"MMIMAIL_SendFinished Mail sent succeeded!---MMIMAIL_EDIT_OPER_SEND")
TRACE_MSG(MMIMAIL_EDIT_964_112_2_18_2_48_7_211,"MMIMAIL_SendFinished Mail sent failed!---MMIMAIL_EDIT_OPER_SEND")
TRACE_MSG(MMIMAIL_EDIT_993_112_2_18_2_48_7_212,"MMIMAIL_SendFinished Mail sent succeeded!---Sending by ID")
TRACE_MSG(MMIMAIL_EDIT_998_112_2_18_2_48_7_213,"MMIMAIL_SendFinished Mail sent failed!---Sending by ID")
TRACE_MSG(MMIMAIL_EDIT_1028_112_2_18_2_48_7_214,"MMIMail==> MMIMAIL_DestroyEditBody ")
TRACE_MSG(MMIMAIL_EDIT_1043_112_2_18_2_48_7_215,"MMIMail MMIMAIL_EditInit PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_1047_112_2_18_2_48_7_216,"MMIMAIL==> MMIMAIL_EditInit: Open type is [%d]")
TRACE_MSG(MMIMAIL_EDIT_1108_112_2_18_2_48_7_217,"MMIMAIL==> MMIMAIL_EditInit: Open type [%d] is invalid!")
TRACE_MSG(MMIMAIL_EDIT_1283_112_2_18_2_48_8_218,"MMIMail MMIMAIL_SaveMail PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_1295_112_2_18_2_48_8_219,"MMIMAIL==> MMIMAIL_SaveMail: Enter save mail! is_inc_org_text is %d")
TRACE_MSG(MMIMAIL_EDIT_1305_112_2_18_2_48_8_220,"MMIMAIL==> MMIMAIL_SaveMail: sender is NULL")
TRACE_MSG(MMIMAIL_EDIT_1343_112_2_18_2_48_8_221,"MMIMAIL==> MMIMAIL_SaveMail: realloc success")
TRACE_MSG(MMIMAIL_EDIT_1349_112_2_18_2_48_8_222,"MMIMAIL==> MMIMAIL_SaveMail: realloc size [%d] fail")
TRACE_MSG(MMIMAIL_EDIT_1354_112_2_18_2_48_8_223,"MMIMAIL==> MMIMAIL_SaveMail: new content size [%d] include org content is larger than 65535")
TRACE_MSG(MMIMAIL_EDIT_1386_112_2_18_2_48_8_224,"MMIMAIL==> MMIMAIL_SaveMail: mail is empty, need not save!")
TRACE_MSG(MMIMAIL_EDIT_1402_112_2_18_2_48_8_225,"MMIMAIL==> CreateMailEditBody: malloc MAIL_MESSAGE_T")
TRACE_MSG(MMIMAIL_EDIT_1409_112_2_18_2_48_8_226,"MMIMAIL==> CreateMailEditBody: malloc from")
TRACE_MSG(MMIMAIL_EDIT_1417_112_2_18_2_48_8_227,"MMIMAIL==> CreateMailEditBody: malloc to")
TRACE_MSG(MMIMAIL_EDIT_1428_112_2_18_2_48_8_228,"MMIMAIL==> CreateMailEditBody: malloc cc")
TRACE_MSG(MMIMAIL_EDIT_1439_112_2_18_2_48_8_229,"MMIMAIL==> CreateMailEditBody: malloc bcc")
TRACE_MSG(MMIMAIL_EDIT_1451_112_2_18_2_48_8_230,"MMIMAIL==> CreateMailEditBody: malloc reply to")
TRACE_MSG(MMIMAIL_EDIT_1460_112_2_18_2_48_8_231,"MMIMAIL==> CreateMailEditBody: malloc subject")
TRACE_MSG(MMIMAIL_EDIT_1464_112_2_18_2_48_8_232,"MMIMAIL==> CreateMailEditBody: malloc content")
TRACE_MSG(MMIMAIL_EDIT_1468_112_2_18_2_48_8_233,"MMIMAIL==> CreateMailEditBody: malloc acc")
TRACE_MSG(MMIMAIL_EDIT_1479_112_2_18_2_48_8_234,"MMIMAIL==> CreateMailEditBody: total malloc size = %d")
TRACE_MSG(MMIMAIL_EDIT_1497_112_2_18_2_48_8_235,"MMIMail==> DestroyMailEditBody")
TRACE_MSG(MMIMAIL_EDIT_1694_112_2_18_2_48_9_236,"MMIMAIL==> AddMailSign:content_ptr or content_ptr->wstr_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_1705_112_2_18_2_48_9_237,"MMIMAIL==> AddMailSign:mail_GetAddSignature return TRUE")
TRACE_MSG(MMIMAIL_EDIT_1745_112_2_18_2_48_9_238,"MMIMAIL==> AddMailOrgText:dest_content_ptr or dest_content_ptr->wstr_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_1751_112_2_18_2_48_9_239,"MMIMAIL==> AddMailOrgText:src_content_ptr or src_content_ptr->wstr_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_1767_112_2_18_2_48_9_240,"MMIMAIL==> AddMailOrgText: mail_GetReplyWithOriginalCont return TRUE")
TRACE_MSG(MMIMAIL_EDIT_1808_112_2_18_2_48_9_241,"MMIMAIL==> CopyMailAcc:edit_mail_ptr OR browser_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_1811_112_2_18_2_48_9_242,"MMIMAIL==> CopyMailAcc: acc num = %d")
TRACE_MSG(MMIMAIL_EDIT_1850_112_2_18_2_48_9_243,"MMIMAIL==> NewMail:edit_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_1872_112_2_18_2_48_9_244,"MMIMAIL==> CopyExistMail:edit_mail_ptr OR browser_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_1942_112_2_18_2_48_9_245,"MMIMAIL==> ReplyMail:edit_mail_ptr OR browser_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_1968_112_2_18_2_48_9_246,"MMIMAIL==> ReplyMail: The original mail subject has include Re: string!")
TRACE_MSG(MMIMAIL_EDIT_1991_112_2_18_2_48_9_247,"MMIMAIL==> ReplyMail: malloc temp_str fail!")
TRACE_MSG(MMIMAIL_EDIT_2024_112_2_18_2_48_9_248,"MMIMAIL==> ReplyAllMail:edit_mail_ptr OR browser_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2088_112_2_18_2_48_9_249,"MMIMAIL==> ReplyAllMail: The original mail subject has include Re: string!")
TRACE_MSG(MMIMAIL_EDIT_2111_112_2_18_2_48_9_250,"MMIMAIL==> ReplyAllMail: malloc temp_str fail!")
TRACE_MSG(MMIMAIL_EDIT_2140_112_2_18_2_48_9_251,"MMIMAIL==> ForwardAll:edit_mail_ptr OR browser_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2166_112_2_18_2_48_9_252,"MMIMAIL==> ForwardAll: malloc temp_str fail!")
TRACE_MSG(MMIMAIL_EDIT_2195_112_2_18_2_48_10_253,"MMIMAIL==> ForwardContent:edit_mail_ptr OR browser_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2221_112_2_18_2_48_10_254,"MMIMAIL==> ForwardContent: malloc temp_str fail!")
TRACE_MSG(MMIMAIL_EDIT_2247_112_2_18_2_48_10_255,"MMIMAIL==> ServerForwardAcc:edit_mail_ptr OR browser_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2273_112_2_18_2_48_10_256,"MMIMAIL==> ServerForwardAll: malloc temp_str fail!")
TRACE_MSG(MMIMAIL_EDIT_2296_112_2_18_2_48_10_257,"MMIMAIL==> ServerForwardAll:edit_mail_ptr OR browser_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2322_112_2_18_2_48_10_258,"MMIMAIL==> ServerForwardAll: malloc temp_str fail!")
TRACE_MSG(MMIMAIL_EDIT_2357_112_2_18_2_48_10_259,"MMIMAIL==> GetMailEditCurOper: s_mail_edit_global.cur_oper = %d")
TRACE_MSG(MMIMAIL_EDIT_2394_112_2_18_2_48_10_260,"MMIMAIL==> AddEditContact:contact_addr OR index_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2399_112_2_18_2_48_10_261,"MMIMail AddEditContact MMIMAIL_RICHTEXT_OP_MAX <= is_add")
TRACE_MSG(MMIMAIL_EDIT_2407_112_2_18_2_48_10_262,"MMIMAIL==> AddEditContact: total num = %d")
TRACE_MSG(MMIMAIL_EDIT_2411_112_2_18_2_48_10_263,"MMIMAIL==> AddEditContact: receiver is empty!")
TRACE_MSG(MMIMAIL_EDIT_2457_112_2_18_2_48_10_264,"MMIMAIL==> AddEditContact: ==error== Email addr [%s] len > 128")
TRACE_MSG(MMIMAIL_EDIT_2483_112_2_18_2_48_10_265,"MMIMAIL==> AddEditContact: receiver is empty!")
TRACE_MSG(MMIMAIL_EDIT_2570_112_2_18_2_48_10_266,"MMIMAIL==> AddEditContact: ==error== Email addr [%s] len > 128")
TRACE_MSG(MMIMAIL_EDIT_2608_112_2_18_2_48_10_267,"MMIMAIL==> AddEditContact: malloc memory fail, malloc size = %d")
TRACE_MSG(MMIMAIL_EDIT_2613_112_2_18_2_48_10_268,"MMIMAIL==> AddEditContact: receiver is empty!")
TRACE_MSG(MMIMAIL_EDIT_2652_112_2_18_2_48_10_269,"MMIMAIL==> AddEditTo:editbody_ptr OR index_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2657_112_2_18_2_48_10_270,"MMIMail AddEditTo MMIMAIL_RICHTEXT_OP_MAX <= is_add")
TRACE_MSG(MMIMAIL_EDIT_2661_112_2_18_2_48_10_271,"MMIMAIL==> AddEditTo enter")
TRACE_MSG(MMIMAIL_EDIT_2679_112_2_18_2_48_10_272,"MMIMAIL==> AddEditCc:editbody_ptr OR index_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2684_112_2_18_2_48_10_273,"MMIMail AddEditCc MMIMAIL_RICHTEXT_OP_MAX <= is_add")
TRACE_MSG(MMIMAIL_EDIT_2688_112_2_18_2_48_10_274,"MMIMAIL==> AddEditCc enter")
TRACE_MSG(MMIMAIL_EDIT_2706_112_2_18_2_48_11_275,"MMIMAIL==> AddEditBcc:editbody_ptr OR index_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2711_112_2_18_2_48_11_276,"MMIMail AddEditBcc MMIMAIL_RICHTEXT_OP_MAX <= is_add")
TRACE_MSG(MMIMAIL_EDIT_2715_112_2_18_2_48_11_277,"MMIMAIL==> AddEditBcc enter")
TRACE_MSG(MMIMAIL_EDIT_2736_112_2_18_2_48_11_278,"MMIMAIL==> AddEditSubject:editbody_ptr OR index_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2741_112_2_18_2_48_11_279,"MMIMail AddEditSubject MMIMAIL_RICHTEXT_OP_MAX <= is_add")
TRACE_MSG(MMIMAIL_EDIT_2756_112_2_18_2_48_11_280,"MMIMAIL==> AddEditSubject: subject is empty!")
TRACE_MSG(MMIMAIL_EDIT_2801_112_2_18_2_48_11_281,"MMIMAIL==> AddEditContent:editbody_ptr OR index_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2806_112_2_18_2_48_11_282,"MMIMail AddEditContent MMIMAIL_RICHTEXT_OP_MAX <= is_add")
TRACE_MSG(MMIMAIL_EDIT_2820_112_2_18_2_48_11_283,"MMIMAIL==> AddEditContent: content is empty!")
TRACE_MSG(MMIMAIL_EDIT_2932_112_2_18_2_48_11_284,"MMIMAIL==> AddEditContent:editbody_ptr OR index_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_2937_112_2_18_2_48_11_285,"MMIMail AddEditAcc MMIMAIL_RICHTEXT_OP_MAX <= is_add")
TRACE_MSG(MMIMAIL_EDIT_2954_112_2_18_2_48_11_286,"MMIMAIL==> AddEditAcc: acc is empty!")
TRACE_MSG(MMIMAIL_EDIT_2993_112_2_18_2_48_11_287,"MMIMAIL==> MailEditEntry:editbody_ptr is NULL")
TRACE_MSG(MMIMAIL_EDIT_3044_112_2_18_2_48_11_288,"MMIMAIL==> MailEditEntry: focus_index [%d] is invalid!")
TRACE_MSG(MMIMAIL_EDIT_3093_112_2_18_2_48_11_289,"MMIMAIL==> MailEditEnterCorrespondItem: richedit_index [%d] is invalid!")
TRACE_MSG(MMIMAIL_EDIT_3111_112_2_18_2_48_11_290,"MMIMail MailInitAccInfo PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_3154_112_2_18_2_48_11_291,"MMIMail HandleMailEditMainWinMsg PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_3158_112_2_18_2_48_11_292,"MMIMAIL==> HandleMailEditMainWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMAIL_EDIT_3274_112_2_18_2_48_12_293,"MMIMail==> HandleMailEditMainWinMsg: GetMailEditCurOper()=%d ")
TRACE_MSG(MMIMAIL_EDIT_3279_112_2_18_2_48_12_294,"MMIMail==> HandleMailEditMainWinMsg: close window , is_other_free is TRUE")
TRACE_MSG(MMIMAIL_EDIT_3284_112_2_18_2_48_12_295,"MMIMail==> HandleMailEditMainWinMsg: close window , is_other_free is FALSE")
TRACE_MSG(MMIMAIL_EDIT_3333_112_2_18_2_48_12_296,"MMIMAIL==> HandleMailEditOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_EDIT_3436_112_2_18_2_48_12_297,"MMIMAIL==> HandleMailEditOptWinMsg, menu_id = %d is invalid")
TRACE_MSG(MMIMAIL_EDIT_3622_112_2_18_2_48_12_298,"MMIMail SetRecverList PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_3644_112_2_18_2_48_12_299,"MMIMAIL==> SetRecverList: current edit item [%d] is invalid")
TRACE_MSG(MMIMAIL_EDIT_3650_112_2_18_2_48_12_300,"MMIMAIL==> SetRecverList: mail_addr_list:[%d],addr_count:[%d]")
TRACE_MSG(MMIMAIL_EDIT_3683_112_2_18_2_48_12_301,"MMIMail SaveRecverList PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_3705_112_2_18_2_48_12_302,"MMIMAIL==> SaveRecverList: current edit item [%d] is invalid")
TRACE_MSG(MMIMAIL_EDIT_3724_112_2_18_2_48_13_303,"MMIMail SaveRecverList PNULL == item_str")
TRACE_MSG(MMIMAIL_EDIT_3764_112_2_18_2_48_13_304,"MMIMAIL==> SaveRecverList: list_num = %d, clean all receivers address")
TRACE_MSG(MMIMAIL_EDIT_3870_112_2_18_2_48_13_305,"MMIMAIL==> GetPbSelReturn: malloc all_number_wstr failed")
TRACE_MSG(MMIMAIL_EDIT_3884_112_2_18_2_48_13_306,"MMIMAIL==> GetPbSelReturn: malloc num_str_ptr failed")
TRACE_MSG(MMIMAIL_EDIT_3914_112_2_18_2_48_13_307,"MMIMAIL==> GetPbSelReturn: malloc email_addr failed")
TRACE_MSG(MMIMAIL_EDIT_3979_112_2_18_2_48_13_308,"MMIMail HandleMailEditRecverListWinMsg PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_4042_112_2_18_2_48_13_309,"MMIMAIL==> HandleMailEditRecverListWinMsg: alloc GUIEDIT_LIST_ITEM_STR_T fail")
TRACE_MSG(MMIMAIL_EDIT_4093_112_2_18_2_48_14_310,"MMIMail SetMailText PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_4123_112_2_18_2_48_14_311,"MMIMAIL==> SetMailText: current edit item [%d] is invalid")
TRACE_MSG(MMIMAIL_EDIT_4150_112_2_18_2_48_14_312,"MMIMail SaveMailText PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_4167_112_2_18_2_48_14_313,"MMIMAIL==> SaveMailText: current edit item [%d] is invalid")
TRACE_MSG(MMIMAIL_EDIT_4196_112_2_18_2_48_14_314,"MMIMail HandleMailEditTextWinMsg PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_4200_112_2_18_2_48_14_315,"MMIMAIL==> HandleMailEditTextWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMAIL_EDIT_4255_112_2_18_2_48_14_316,"MMIMail AppendEditAccListItem filename_len <= 0")
TRACE_MSG(MMIMAIL_EDIT_4261_112_2_18_2_48_14_317,"MMIMail AppendEditAccListItem PNULL == filename")
TRACE_MSG(MMIMAIL_EDIT_4302_112_2_18_2_48_14_318,"MMIMail SetEditAccList PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_4419_112_2_18_2_48_14_319,"MMIMail SaveSelectAccFile PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_4425_112_2_18_2_48_14_320,"MMIMAIL==> SaveSelectAccFile: selected_info is NULL")
TRACE_MSG(MMIMAIL_EDIT_4479_112_2_18_2_48_14_321,"MMIMAIL==> SaveSelectAccFile: free_index = %d")
TRACE_MSG(MMIMAIL_EDIT_4501_112_2_18_2_48_14_322,"MMIMail DelSelectAcc PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_4582_112_2_18_2_48_14_323,"MMIMAIL==> DelSelectAcc: index = %d is invalid")
TRACE_MSG(MMIMAIL_EDIT_4588_112_2_18_2_48_15_324,"MMIMAIL==> DelSelectAcc: current edit acc is null!")
TRACE_MSG(MMIMAIL_EDIT_4589_112_2_18_2_48_15_325,"MMIMAIL==> DelSelectAcc: editbody_ptr->partcount = %d")
TRACE_MSG(MMIMAIL_EDIT_4614_112_2_18_2_48_15_326,"MMIMail HandleEditAccListWinMsg PNULL == editbody_ptr")
TRACE_MSG(MMIMAIL_EDIT_4685_112_2_18_2_48_15_327,"MMIMAIL==> HandleEditAccListWinMsg: param is NULL")
TRACE_MSG(MMIMAIL_EDIT_4765_112_2_18_2_48_15_328,"MMIMail HandleEditAccMenuOptWinMsg node_id = %d")
TRACE_MSG(MMIMAIL_READ_691_112_2_18_2_48_17_329,"MMIMAIL==> MMIMAIL_ReadFinished: post MMIMAIL_MAILBOX_MAIN_WIN_ID ==> MSG_MAIL_FINISH_READ")
TRACE_MSG(MMIMAIL_READ_698_112_2_18_2_48_17_330,"MMIMAIL==> MMIMAIL_ReadFinished: MMIMAIL_MAILBOX_MAIN_WIN_ID is not open")
TRACE_MSG(MMIMAIL_READ_712_112_2_18_2_48_17_331,"MMIMAIL==> MMIMAIL_ReadFinished: open_type : %d, record_id : %d")
TRACE_MSG(MMIMAIL_READ_718_112_2_18_2_48_18_332,"MMIMAIL==> MMIMAIL_ReadFinished: get mail ptr fail!")
TRACE_MSG(MMIMAIL_READ_746_112_2_18_2_48_18_333,"MMIMAIL==> MMIMAIL_ReadFinished: mail open type is %d")
TRACE_MSG(MMIMAIL_READ_752_112_2_18_2_48_18_334,"MMIMAIL==> MMIMAIL_ReadFinished: whole_mail_ptr is NULL!")
TRACE_MSG(MMIMAIL_READ_776_112_2_18_2_48_18_335,"MMIMAIL==> MMIMAIL_GetIconByType: type [%d] is invalid")
TRACE_MSG(MMIMAIL_READ_816_112_2_18_2_48_18_336,"MMIMAIL==> HandleMailReadWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_READ_989_112_2_18_2_48_18_337,"MMIMAIL==> HandleReadMailOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_READ_1005_112_2_18_2_48_18_338,"MMIMAIL==> HandleReadMailOptWinMsg: node_id = %d")
TRACE_MSG(MMIMAIL_READ_1037_112_2_18_2_48_18_339,"MMIMail HandleReadMailOptMenuId PNULL == whole_mail_ptr")
TRACE_MSG(MMIMAIL_READ_1100_112_2_18_2_48_18_340,"MMIMail HandleReadInboxOptMenuId PNULL == whole_mail_ptr")
TRACE_MSG(MMIMAIL_READ_1163_112_2_18_2_48_18_341,"MMIMail HandleReadOutboxOptMenuId PNULL == whole_mail_ptr")
TRACE_MSG(MMIMAIL_READ_1179_112_2_18_2_48_18_342,"MMIMAIL==> HandleReadOutboxOptMenuId: current operation mail is NULL!")
TRACE_MSG(MMIMAIL_READ_1220_112_2_18_2_48_19_343,"MMIMail HandleReadSentboxOptMenuId PNULL == whole_mail_ptr")
TRACE_MSG(MMIMAIL_READ_1260_112_2_18_2_48_19_344,"MMIMAIL==> HandleMailReadAllContentWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_READ_1296_112_2_18_2_48_19_345,"MMIMAIL==> HandleReadWaitingWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_READ_1334_112_2_18_2_48_19_346,"MMIMAIL==> HandleAccPicurePreview, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_READ_1417_112_2_18_2_48_19_347,"MMIMAIL==> HandleAccessoryOpen enter\n")
TRACE_MSG(MMIMAIL_READ_1423_112_2_18_2_48_19_348,"MMIMAIL==> HandleAccessoryOpen full_path_name = %s\n")
TRACE_MSG(MMIMAIL_READ_1437_112_2_18_2_48_19_349,"MMIMAIL==> HandleAccessoryOpen file_suffix = %s\n")
TRACE_MSG(MMIMAIL_READ_1544_112_2_18_2_48_19_350,"MMIMAIL==> HandleAccessoryOpen ptr=%x\n")
TRACE_MSG(MMIMAIL_READ_1586_112_2_18_2_48_19_351,"MMIMAIL==> HandleAccessoryOpen entry_para.url_ptr=%s\n")
TRACE_MSG(MMIMAIL_READ_1614_112_2_18_2_48_19_352,"MMIMAIL==> HandleAccessoryOpen file type not support!\n")
TRACE_MSG(MMIMAIL_READ_1639_112_2_18_2_48_19_353,"MMIMail HandleReadAccessoryListWinMsg PNULL == whole_mail_ptr")
TRACE_MSG(MMIMAIL_READ_1643_112_2_18_2_48_19_354,"MMIMAIL==> HandleReadAccessoryListWinMsg msg_id = 0x%x, have_items = %d\n")
TRACE_MSG(MMIMAIL_READ_1728_112_2_18_2_48_19_355,"MMIMAIL==> HandleReadAccessoryListWinMsg: full_path_name is NULL")
TRACE_MSG(MMIMAIL_READ_1795_112_2_18_2_48_20_356,"MMIMail HandleReadAccExpandListWinMsg PNULL == whole_mail_ptr")
TRACE_MSG(MMIMAIL_READ_1799_112_2_18_2_48_20_357,"MMIMAIL==> HandleReadAccExpandListWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_READ_1839_112_2_18_2_48_20_358,"MMIMail HandleReadAccExpandListWinMsg PNULL == cur_acc_ptr")
TRACE_MSG(MMIMAIL_READ_1862_112_2_18_2_48_20_359,"MMIMail HandleReadAccExpandListWinMsg PNULL == cur_acc_ptr")
TRACE_MSG(MMIMAIL_READ_1910_112_2_18_2_48_20_360,"MMIMAIL==> HandleReadAccMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_READ_1931_112_2_18_2_48_20_361,"MMIMail HandleReadAccMenuOptWinMsg node_id = %d")
TRACE_MSG(MMIMAIL_READ_1964_112_2_18_2_48_20_362,"MMIMail HandleSaveOrDlAccessory PNULL == whole_mail_ptr")
TRACE_MSG(MMIMAIL_READ_1970_112_2_18_2_48_20_363,"MMIMail HandleSaveOrDlAccessory PNULL == acc_ptr")
TRACE_MSG(MMIMAIL_READ_2019_112_2_18_2_48_20_364,"MMIMAIL==> HandleDlWaitingWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_READ_2063_112_2_18_2_48_20_365,"PUSH MAIL  AddMailSender PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2107_112_2_18_2_48_20_366,"MMIMAIL==> AddMailSender: sender is empty!")
TRACE_MSG(MMIMAIL_READ_2134_112_2_18_2_48_20_367,"PUSH MAIL  AddMailAccount PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2186_112_2_18_2_48_20_368,"PUSH MAIL  AddMailContact: PNULL == contact_addr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2193_112_2_18_2_48_20_369,"MMIMAIL==> AddMailContact: total num = %d")
TRACE_MSG(MMIMAIL_READ_2197_112_2_18_2_48_20_370,"MMIMAIL==> AddMailContact: receiver is empty!")
TRACE_MSG(MMIMAIL_READ_2237_112_2_18_2_48_21_371,"MMIMAIL==> AddMailContact: ==error== Email addr [%s] len > 128")
TRACE_MSG(MMIMAIL_READ_2329_112_2_18_2_48_21_372,"MMIMAIL==> AddMailContact: ==error== Email addr [%s] len > 128")
TRACE_MSG(MMIMAIL_READ_2355_112_2_18_2_48_21_373,"MMIMAIL==> AddMailContact: malloc memory fail, malloc size = %d")
TRACE_MSG(MMIMAIL_READ_2360_112_2_18_2_48_21_374,"MMIMAIL==> AddMailContact: receiver is empty!")
TRACE_MSG(MMIMAIL_READ_2391_112_2_18_2_48_21_375,"PUSH MAIL  AddMailTo: PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2395_112_2_18_2_48_21_376,"MMIMAIL==> AddMailTo enter")
TRACE_MSG(MMIMAIL_READ_2412_112_2_18_2_48_21_377,"PUSH MAIL  AddMailCc: PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2416_112_2_18_2_48_21_378,"MMIMAIL==> AddMailCc enter")
TRACE_MSG(MMIMAIL_READ_2433_112_2_18_2_48_21_379,"PUSH MAIL  AddMailBcc: PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2437_112_2_18_2_48_21_380,"MMIMAIL==> AddMailBcc enter")
TRACE_MSG(MMIMAIL_READ_2460_112_2_18_2_48_21_381,"PUSH MAIL  AddMailTime: PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2487_112_2_18_2_48_21_382,"MMIMAIL==> AddMailTime: time is empty!")
TRACE_MSG(MMIMAIL_READ_2506_112_2_18_2_48_21_383,"PUSH MAIL  AddMailSubject: PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2527_112_2_18_2_48_21_384,"MMIMAIL==> AddMailSubject: subject is empty!")
TRACE_MSG(MMIMAIL_READ_2564_112_2_18_2_48_21_385,"PUSH MAIL  AddMailAccessory: PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2629_112_2_18_2_48_21_386,"MMIMAIL==> AddMailAccessory: malloc memory fail, malloc size = %d")
TRACE_MSG(MMIMAIL_READ_2634_112_2_18_2_48_21_387,"MMIMAIL==> AddMailAccessory: accessory is empty!")
TRACE_MSG(MMIMAIL_READ_2653_112_2_18_2_48_21_388,"PUSH MAIL  AddMailSeperator: PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2684_112_2_18_2_48_21_389,"PUSH MAIL  AddMailContent: PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2726_112_2_18_2_48_21_390,"MMIMAIL==> AddMailContent: content is empty!")
TRACE_MSG(MMIMAIL_READ_2744_112_2_18_2_48_22_391,"PUSH MAIL  ShowMailContent: PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2790_112_2_18_2_48_22_392,"PUSH MAIL  ShowAllMailContent: PNULL == whole_mail_ptr ||PNULL == index_ptr ")
TRACE_MSG(MMIMAIL_READ_2929_112_2_18_2_48_22_393,"MMIMAIL==> OpenMailReadOptWin:current box type is invalid!")
TRACE_MSG(MMIMAIL_READ_2948_112_2_18_2_48_22_394,"MMIMail OpenAccessoryOptWin PNULL == acc_ptr")
TRACE_MSG(MMIMAIL_READ_3046_112_2_18_2_48_22_395,"MMIMAIL==> SwitchToNextMail: no next mail!")
TRACE_MSG(MMIMAIL_READ_3051_112_2_18_2_48_22_396,"MMIMAIL==> SwitchToNextMail: cur_mail_ptr is NULL!")
TRACE_MSG(MMIMAIL_READ_3075_112_2_18_2_48_22_397,"MMIMail AppendAccessoryListItem filename_len <= 0 OR PNULL == filename")
TRACE_MSG(MMIMAIL_READ_3126_112_2_18_2_48_22_398,"MMIMail ReplaceAccessoryListItem filename_len <=  0 OR  PNULL == filename")
TRACE_MSG(MMIMAIL_READ_3274_112_2_18_2_48_23_399,"MMIMAIL==> UpdateMailReadState: current operation mail ptr is NULL")
TRACE_MSG(MMIMAIL_READ_3278_112_2_18_2_48_23_400,"MMIMAIL==> UpdateMailReadState: record_id = %d, cur_mail->record_id = %d")
TRACE_MSG(MMIMAIL_READ_3318_112_2_18_2_48_23_401,"MMIMAIL==> MMIMAIL_GetCurOperExpandAcc: Error!")
TRACE_MSG(MMIMAIL_READ_3361_112_2_18_2_48_23_402,"MMIMail MMIMAIL_IsAccessoryDownloaded PNULL == whole_mail_ptr OR PNULL == accessory_ptr")
TRACE_MSG(MMIMAIL_READ_3370_112_2_18_2_48_23_403,"MMIMAIL==> MMIMAIL_IsAccessoryDownloaded: result = %d!")
TRACE_MSG(MMIMAIL_READ_3390_112_2_18_2_48_23_404,"MMIMail MMIMAIL_IsAccessoryAutoChanged PNULL == whole_mail_ptr OR PNULL == accessory_ptr")
TRACE_MSG(MMIMAIL_READ_3399_112_2_18_2_48_23_405,"MMIMAIL==> MMIMAIL_IsAccessoryAutoChanged: changed num = %d!")
TRACE_MSG(MMIMAIL_READ_3434_112_2_18_2_48_23_406,"MMIMAIL_LoadAccessoryList whole_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_READ_3479_112_2_18_2_48_23_407,"MMIMAIL_LoadAccExpandList whole_mail_ptr || acc_ptr is NULL")
TRACE_MSG(MMIMAIL_READ_3491_112_2_18_2_48_23_408,"MMIMail MMIMAIL_LoadAccExpandList PNULL == list_ptr")
TRACE_MSG(MMIMAIL_READ_3550_112_2_18_2_48_23_409,"AppendRecverListItem mail_addr is NULL")
TRACE_MSG(MMIMAIL_READ_3593_112_2_18_2_48_23_410,"MMIMAIL_LoadRecverList whole_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_READ_3689_112_2_18_2_48_23_411,"MMIMail HandleReadAllRecverListWinMsg PNULL == whole_mail_ptr")
TRACE_MSG(MMIMAIL_READ_3693_112_2_18_2_48_23_412,"MMIMAIL==> HandleReadAllRecverListWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_READ_3715_112_2_18_2_48_23_413,"MMIMail HandleReadAllRecverListWinMsg PNULL == s_cur_recver_list")
TRACE_MSG(MMIMAIL_READ_3793_112_2_18_2_48_24_414,"MMIMAIL==> HandleReadRecvListOptWin, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_READ_3827_112_2_18_2_48_24_415,"MMIMAIL==> HandleReadRecvListOptWin, menu_id = %d is invalid")
TRACE_MSG(MMIMAIL_READ_3861_112_2_18_2_48_24_416,"MMIMAIL==> HandleReadSelGroupWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_1677_112_2_18_2_48_28_417,"Mail:MMIAPIMAIL_Enter change sim and no new settings")
TRACE_MSG(MMIMAIL_WINTAB_1757_112_2_18_2_48_28_418,"Mail: MMIAPIMAIL_GetAccountCallback cur sim is a white sim card!")
TRACE_MSG(MMIMAIL_WINTAB_1839_112_2_18_2_48_28_419,"MMIMAIL==> MMIAPIMAIL_IsHaveUnreadMail: unread_num = %d")
TRACE_MSG(MMIMAIL_WINTAB_1868_112_2_18_2_48_28_420,"MMIMAIL==> MMIAPIMAIL_SetSmsOk: is_sms_ok = %d")
TRACE_MSG(MMIMAIL_WINTAB_1878_112_2_18_2_48_28_421,"MMIMAIL==> MMIMAIL_IsReady: return = %d")
TRACE_MSG(MMIMAIL_WINTAB_1888_112_2_18_2_48_28_422,"MMIMAIL==> MMIMAIL_GetOpenType: return = %d")
TRACE_MSG(MMIMAIL_WINTAB_1900_112_2_18_2_48_28_423,"MMIMAIL==> MMIMAIL_GetOpenParam: return = %d")
TRACE_MSG(MMIMAIL_WINTAB_1904_112_2_18_2_48_28_424,"MMIMAIL==> MMIMAIL_GetOpenParam: return 0")
TRACE_MSG(MMIMAIL_WINTAB_1915_112_2_18_2_48_28_425,"MMIMAIL==> MMIMAIL_GetDefAccountId: return = %d")
TRACE_MSG(MMIMAIL_WINTAB_2121_112_2_18_2_48_28_426,"MMIMAIL==> MMIMAIL_GetMailState: mail_ptr is NULL")
TRACE_MSG(MMIMAIL_WINTAB_2142_112_2_18_2_48_28_427,"MMIMAIL==> MMIMAIL_SetMailState: mail_ptr is NULL")
TRACE_MSG(MMIMAIL_WINTAB_2200_112_2_18_2_48_29_428,"MMIMAIL==> MMIMAIL_OpenDelteQueryWin: delete_mail_num = 0,no need delete!")
TRACE_MSG(MMIMAIL_WINTAB_2257_112_2_18_2_48_29_429,"MMIMAIL==> MMIMAIL_OpenDelteQueryWin: delete_mail_num = 0,no need delete!")
TRACE_MSG(MMIMAIL_WINTAB_2330_112_2_18_2_48_29_430,"MMIMAIL==> MMIMAIL_GetMailPtrByRecordId: record_id = %d")
TRACE_MSG(MMIMAIL_WINTAB_2364_112_2_18_2_48_29_431,"MMIMAIL==> MMIAPIMAIL_IsSmsOpen: is_sms_open = %d")
TRACE_MSG(MMIMAIL_WINTAB_2613_112_2_18_2_48_29_432,"MMIMAIL_GetDefAccount account_info is null")
TRACE_MSG(MMIMAIL_WINTAB_2638_112_2_18_2_48_29_433,"MMIMAIL_GetDefAccount account_info OR mailbox_info is null")
TRACE_MSG(MMIMAIL_WINTAB_2798_112_2_18_2_48_30_434,"MMIMAIL==> MMIMAIL_CloseInitWaitingWindow: s_mmimail_info.enter_type = %d")
TRACE_MSG(MMIMAIL_WINTAB_2829_112_2_18_2_48_30_435,"MMIMAIL==> HandleWaitGetAccountWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_2885_112_2_18_2_48_30_436,"MMIMAIL==> HandleOverwriteQueryWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_2924_112_2_18_2_48_30_437,"MMIMAIL==> HandleDelExistAccountQueryWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_2964_112_2_18_2_48_30_438,"MMIMAIL==> HandleInitWaitingWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3017_112_2_18_2_48_30_439,"MMIMAIL==> HandleDelQueryWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3054_112_2_18_2_48_30_440,"MMIMAIL==> HandleDelAllInCurBoxQueryWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3093_112_2_18_2_48_30_441,"MMIMAIL==> HandleDelWaitWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3155_112_2_18_2_48_31_442,"HandleMailTaskMsg app_ptr is null")
TRACE_MSG(MMIMAIL_WINTAB_3159_112_2_18_2_48_31_443,"MMIMAIL==> HandleMailTaskMsg: msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3288_112_2_18_2_48_31_444,"MMIMAIL==> HandleMailWelcomeWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3352_112_2_18_2_48_31_445,"MMIMAIL==> HandleMailApplyAlertWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3393_112_2_18_2_48_31_446,"MMIMAIL==> HandleMailMainMenuWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3497_112_2_18_2_48_31_447,"MMIMAIL==> HandleMailMainMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3532_112_2_18_2_48_31_448,"MMIMail HandleMailMainMenuOptWinMsg menu_id = %d")
TRACE_MSG(MMIMAIL_WINTAB_3563_112_2_18_2_48_31_449,"MMIMAIL==> HandleMailBoxMainWindow, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3634_112_2_18_2_48_31_450,"MMIMAIL==> HandleInBoxChildWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3666_112_2_18_2_48_32_451,"MMIMail HandleInBoxChildWinMsg PNULL == need_item_data_ptr")
TRACE_MSG(MMIMAIL_WINTAB_3766_112_2_18_2_48_32_452,"MMIMAIL==> HandleOutBoxChildWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3798_112_2_18_2_48_32_453,"MMIMail HandleOutBoxChildWinMsg PNULL == need_item_data_ptr")
TRACE_MSG(MMIMAIL_WINTAB_3896_112_2_18_2_48_32_454,"MMIMAIL==> HandleSentBoxChildWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_3928_112_2_18_2_48_32_455,"MMIMail HandleSentBoxChildWinMsg PNULL == need_item_data_ptr")
TRACE_MSG(MMIMAIL_WINTAB_4025_112_2_18_2_48_32_456,"MMIMAIL==> HandleDraftChildWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_4057_112_2_18_2_48_32_457,"MMIMail HandleDraftChildWinMsg PNULL == need_item_data_ptr")
TRACE_MSG(MMIMAIL_WINTAB_4292_112_2_18_2_48_33_458,"MMIMail HandleDraftChildWinMsg PNULL == cur_mail_ptr")
TRACE_MSG(MMIMAIL_WINTAB_4338_112_2_18_2_48_33_459,"MMIMAIL==> HandleMailListOkMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_4391_112_2_18_2_48_33_460,"HandleMailListOkMsg: Get current operating mail fail!")
TRACE_MSG(MMIMAIL_WINTAB_4477_112_2_18_2_48_33_461,"MMIMail HandleMailBoxOptMenuId PNULL == cur_mail_ptr")
TRACE_MSG(MMIMAIL_WINTAB_4512_112_2_18_2_48_33_462,"MMIMail HandleMailBoxOptMenuId PNULL == cur_mail_ptr")
TRACE_MSG(MMIMAIL_WINTAB_4670_112_2_18_2_48_34_463,"MMIMAIL==> HandleOutboxOptMenuId: current operation mail is NULL!")
TRACE_MSG(MMIMAIL_WINTAB_4735_112_2_18_2_48_34_464,"MMIMail HandleSentboxOptMenuId PNULL == cur_mail_ptr")
TRACE_MSG(MMIMAIL_WINTAB_4956_112_2_18_2_48_34_465,"MMIMAIL==> HandleInboxMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_4982_112_2_18_2_48_34_466,"MMIMAIL==> HandleInboxMenuOptWinMsg: menu_id = %d")
TRACE_MSG(MMIMAIL_WINTAB_5015_112_2_18_2_48_34_467,"MMIMAIL==> HandleOutboxMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_5041_112_2_18_2_48_34_468,"MMIMAIL==> HandleOutboxMenuOptWinMsg: menu_id = %d")
TRACE_MSG(MMIMAIL_WINTAB_5074_112_2_18_2_48_34_469,"MMIMAIL==> HandleOutboxMenuOptExtWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_5099_112_2_18_2_48_34_470,"MMIMAIL==> HandleOutboxMenuOptExtWinMsg: menu_id = %d")
TRACE_MSG(MMIMAIL_WINTAB_5132_112_2_18_2_48_34_471,"MMIMAIL==> HandleSentboxMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_5158_112_2_18_2_48_34_472,"MMIMAIL==> HandleSentboxMenuOptWinMsg: menu_id = %d")
TRACE_MSG(MMIMAIL_WINTAB_5191_112_2_18_2_48_35_473,"MMIMAIL==> HandleDraftboxMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_5217_112_2_18_2_48_35_474,"MMIMAIL==> HandleDraftboxMenuOptWinMsg: menu_id = %d")
TRACE_MSG(MMIMAIL_WINTAB_5270_112_2_18_2_48_35_475,"MMIMAIL==> ShowWelcomeInfo: charge_info->wstr_ptr = %d, charge_info->wstr_len = %d")
TRACE_MSG(MMIMAIL_WINTAB_5275_112_2_18_2_48_35_476,"MMIMAIL==> ShowWelcomeInfo: charge_info is null")
TRACE_MSG(MMIMAIL_WINTAB_5294_112_2_18_2_48_35_477,"MMIMail ShowWelcomeInfo MMIMAIL_WELCOME_CONTENT_LEN <= uint16_str_len")
TRACE_MSG(MMIMAIL_WINTAB_5315_112_2_18_2_48_35_478,"MMIMAIL==> StartMailMenuWinTimer: the mainmenu win timer has started!")
TRACE_MSG(MMIMAIL_WINTAB_5332_112_2_18_2_48_35_479,"MMIMAIL==> StopMailMenuWinTimer: the mainmenu win timer has stop!")
TRACE_MSG(MMIMAIL_WINTAB_5683_112_2_18_2_48_35_480,"MMIMAIL==> EnterMailCorrespondingMenu node_id = %d")
TRACE_MSG(MMIMAIL_WINTAB_5725_112_2_18_2_48_36_481,"StartReadMail ctrl_id is 0")
TRACE_MSG(MMIMAIL_WINTAB_5762_112_2_18_2_48_36_482,"MMIMAIL==> MMIMAIL_Enter: %s")
TRACE_MSG(MMIMAIL_WINTAB_5766_112_2_18_2_48_36_483,"MMIMAIL==> MMIMAIL_Enter: no local account!")
TRACE_MSG(MMIMAIL_WINTAB_5852_112_2_18_2_48_36_484,"MMIMAIL==> MMIMAIL_Enter: enter type [%d] is invalid!")
TRACE_MSG(MMIMAIL_WINTAB_5911_112_2_18_2_48_36_485,"MMIMAIL==> MMIMAIL_GetBoxTotalNumber: box_type = %d is invalid")
TRACE_MSG(MMIMAIL_WINTAB_5940_112_2_18_2_48_36_486,"MMIMAIL==> MMIMAIL_GetInboxBoxUnreadNum: unread_num = %d")
TRACE_MSG(MMIMAIL_WINTAB_5959_112_2_18_2_48_36_487,"MMIMAIL==> MMIMAIL_LoadMailList the count of list box is %d")
TRACE_MSG(MMIMAIL_WINTAB_6019_112_2_18_2_48_36_488,"MMIMAIL==> MMIMAIL_GetMailListHead: the box_type is %d")
TRACE_MSG(MMIMAIL_WINTAB_6118_112_2_18_2_48_36_489,"MMIMAIL_AppendListItem string_ptr is NULL")
TRACE_MSG(MMIMAIL_WINTAB_6525_112_2_18_2_48_37_490,"MMIMAIL==> MMIMAIL_GetCurMailIsMarked: cur_mail_ptr is null !")
TRACE_MSG(MMIMAIL_WINTAB_6548_112_2_18_2_48_37_491,"MMIMAIL==> MMIMAIL_SetCurMailMarked: cur_mail_ptr is null!")
TRACE_MSG(MMIMAIL_WINTAB_6687_112_2_18_2_48_38_492,"MMIMAIL==> MMIMAIL_GetMailboxDeleteAvailNum delete_mail_num=%d")
TRACE_MSG(MMIMAIL_WINTAB_6754_112_2_18_2_48_38_493,"MMIMAIL==> MMIMAIL_HandleRetrAttachResult: retr_result = %d")
TRACE_MSG(MMIMAIL_WINTAB_6790_112_2_18_2_48_38_494,"MMIMAIL==> MMIMAIL_HandleRetrAttachResult: MMIMAIL_DL_ACCESSORY_WAITING_WIN_ID and MMIMAIL_RECV_MAIL_PROGRESS_WIN_ID is not open!")
TRACE_MSG(MMIMAIL_WINTAB_6791_112_2_18_2_48_38_495,"MMIMAIL==> MMIMAIL_HandleRetrAttachResult: retr_result = %d")
TRACE_MSG(MMIMAIL_WINTAB_6794_112_2_18_2_48_38_496,"MMIMAIL==> MMIMAIL_HandleRetrAttachResult Full Paint ACCESSORY WIN!")
TRACE_MSG(MMIMAIL_WINTAB_6811_112_2_18_2_48_38_497,"MMIMAIL==> MMIMAIL_CheckPwFinished: is_success = %d")
TRACE_MSG(MMIMAIL_WINTAB_6818_112_2_18_2_48_38_498,"MMIMAIL==> MMIMAIL_CheckPwFinished: edit password window")
TRACE_MSG(MMIMAIL_WINTAB_6824_112_2_18_2_48_38_499,"MMIMAIL==> MMIMAIL_CheckPwFinished: switch account window")
TRACE_MSG(MMIMAIL_WINTAB_6830_112_2_18_2_48_38_500,"MMIMAIL==> MMIMAIL_CheckPwFinished: enter app")
TRACE_MSG(MMIMAIL_WINTAB_6841_112_2_18_2_48_38_501,"MMIMAIL==> MMIMAIL_CheckPwFinished: account num = %d")
TRACE_MSG(MMIMAIL_WINTAB_6846_112_2_18_2_48_38_502,"MMIMAIL==> MMIMAIL_CheckPwFinished: edit password window")
TRACE_MSG(MMIMAIL_WINTAB_6852_112_2_18_2_48_38_503,"MMIMAIL==> MMIMAIL_CheckPwFinished: switch account window")
TRACE_MSG(MMIMAIL_WINTAB_6858_112_2_18_2_48_38_504,"MMIMAIL==> MMIMAIL_CheckPwFinished: enter app")
TRACE_MSG(MMIMAIL_WINTAB_6876_112_2_18_2_48_38_505,"MMIMAIL==> MMIMAIL_DelFinished: Account del finish by Bomb!")
TRACE_MSG(MMIMAIL_WINTAB_6922_112_2_18_2_48_38_506,"MMIMAIL==> MMIMAIL_DelFinished: Account logoff finish!")
TRACE_MSG(MMIMAIL_WINTAB_6930_112_2_18_2_48_38_507,"MMIMAIL==> MMIMAIL_DelFinished: Account del finish by Bomb!")
TRACE_MSG(MMIMAIL_WINTAB_7041_112_2_18_2_48_38_508,"MMIMAIL==> MMIMAIL_ProgressUpdateCallBack: cur_progress = %d, total_progress = %d")
TRACE_MSG(MMIMAIL_WINTAB_7053_112_2_18_2_48_38_509,"MMIMAIL==> MMIMAIL_ProgressUpdateCallBack: progress_info is PNULL")
TRACE_MSG(MMIMAIL_WINTAB_7088_112_2_18_2_48_38_510,"MMIMAIL_IsHaveReceiver cur_mail_ptr is NULL")
TRACE_MSG(MMIMAIL_WINTAB_7178_112_2_18_2_48_39_511,"MMIMAIL==> MMIMAIL_LoadAccountList: def_acc_id = %d error!")
TRACE_MSG(MMIMAIL_WINTAB_7500_112_2_18_2_48_39_512,"MMIMAIL==> HandleSetAccountWinMsg: current select account id = %d")
TRACE_MSG(MMIMAIL_WINTAB_7526_112_2_18_2_48_39_513,"MMIMAIL==> HandleSetAccountWinMsg: receive MSG_MAIL_CHECK_PASSWD_RESULT")
TRACE_MSG(MMIMAIL_WINTAB_7640_112_2_18_2_48_40_514,"MMIMAIL==> HandleSetAccountOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMIMAIL_WINTAB_7940_112_2_18_2_48_40_515,"MMIMAIL==> MMIMAIL_LoadAccountEditItem: focus item is %d")
TRACE_MSG(MMIMAIL_WINTAB_8149_112_2_18_2_48_41_516,"MMIMAIL==> HandleMailEditAccountPwWinMsg: receive MSG_MAIL_CHECK_PASSWD_RESULT, is_success = %d")
TRACE_MSG(MMIMAIL_WINTAB_8155_112_2_18_2_48_41_517,"MMIMAIL==> HandleMailEditAccountPwWinMsg: tmp_str.wstr_len = %d")
TRACE_MSG(MMIMAIL_WINTAB_8210_112_2_18_2_48_41_518,"MMIMAIL==> HandleMailInputPwWinMsg: add_data = %d, acc_id = %d")
TRACE_MSG(MMIMAIL_WINTAB_8528_112_2_18_2_48_41_519,"AddSilentTime index_ptr || time is NULL")
TRACE_MSG(MMIMAIL_WINTAB_8605_112_2_18_2_48_41_520,"MMIMAIL==> SetSilentTime: focus item is %d")
TRACE_MSG(MMIMAIL_WINTAB_8728_112_2_18_2_48_42_521,"SaveSilentTime:hour is %d,minute is %d is invalid!")
TRACE_MSG(MMIMAIL_WINTAB_9454_112_2_18_2_48_44_522,"Mail:MMIMAIL_HandleNewMsgWin do not allow in mail")
TRACE_MSG(MMIMAIL_WINTAB_9505_112_2_18_2_48_44_523,"[MMISET]:MMIAPISET_AlertRingPlayResult: result = %d")
TRACE_MSG(MMIMAIL_WINTAB_9583_112_2_18_2_48_44_524,"MAIL: MMIMAIL_PlayMailSound now silence mode")
TRACE_MSG(MMIMAIL_WINTAB_9600_112_2_18_2_48_44_525,"MAIL: PlaymailRing vt mode")
TRACE_MSG(MMIMAIL_WINTAB_9605_112_2_18_2_48_44_526,"MAIL: PlaymailRing normal call mode")
TRACE_MSG(MMIMAIL_WINTAB_9610_112_2_18_2_48_44_527,"MAIL: PlaymailRing ring_type=%d")
TRACE_MSG(MMIMAIL_WINTAB_9769_112_2_18_2_48_45_528,"MMIMail_NewMailArrival No space ")
TRACE_MSG(MMIMAIL_WINTAB_9777_112_2_18_2_48_45_529,"MMIMail_NewMailArrival box is full")
TRACE_MSG(MMIMAIL_WINTAB_9784_112_2_18_2_48_45_530,"MMIMail_NewMailArrival recv 0 mail ")
TRACE_MSG(MMIMAIL_WINTAB_9799_112_2_18_2_48_45_531,"MMIMail_NewMailArrival in mail box ")
TRACE_MSG(MMIMAIL_WINTAB_9854_112_2_18_2_48_45_532,"MMIMail_NewMailArrival ")
TRACE_MSG(MMIMAIL_WINTAB_9981_112_2_18_2_48_45_533,"HandleCopyAccWaitWinMsg  copy_result=%d")
TRACE_MSG(MMIMAIL_WINTAB_10045_112_2_18_2_48_45_534,"select_foler_full_path = ")
TRACE_MSG(MMIMAIL_WINTAB_10048_112_2_18_2_48_45_535,"%d:%x ")
TRACE_MSG(MMIMAIL_WINTAB_10052_112_2_18_2_48_45_536,"file_size = ")
TRACE_MSG(MMIMAIL_WINTAB_10091_112_2_18_2_48_45_537,"MMIAPIFMM_CopyOneFile ret=%d")
TRACE_MSG(MMIMAIL_WINTAB_10167_112_2_18_2_48_45_538,"MMIMAIL==> MMIAPIFMM_CopyAccToFolder: Enter")
TRACE_MSG(MMIMAIL_WINTAB_10174_112_2_18_2_48_45_539,"MMIMAIL==> MMIAPIFMM_CopyAccToFolder: alloc s_selected_file_ptr fail!")
TRACE_MSG(MMIMAIL_WINTAB_10184_112_2_18_2_48_45_540,"s_acc_file_ptr = ")
TRACE_MSG(MMIMAIL_WINTAB_10187_112_2_18_2_48_45_541,"%d:%x ")
TRACE_MSG(MMIMAIL_WINTAB_10239_112_2_18_2_48_46_542,"MMIMAIL==> MMIAPIFMM_CopyAccToFolder: alloc s_copy_del_path_data_ptr fail!")
TRACE_MSG(MMIMAIL_WINTAB_10279_112_2_18_2_48_46_543,"MMIMAIL==> MMIAPIFMM_FreeAccTmpSpace: Enter")
TRACE_MSG(MMIMAIL_WINTAB_10315_112_2_18_2_48_46_544,"Mail:MMIMAILPDP_Active apn:%s")
TRACE_MSG(MMIMAIL_WINTAB_10383_112_2_18_2_48_46_545,"[MMISMS ATC] MMIAPIMAIL_InsertMessage result %d")
END_TRACE_MAP(MMI_APP_PUSH_MAIL_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_PUSH_MAIL_TRC_H_


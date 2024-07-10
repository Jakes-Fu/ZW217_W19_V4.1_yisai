/******************************************************************************
 ** File Name:      mmi_app_dcd_trc.h                                         *
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
//trace_id:165
#ifndef _MMI_APP_DCD_TRC_H_
#define _MMI_APP_DCD_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIDCD_EXPORT_98_112_2_18_2_12_39_0 "[MMIDCD]: MMIAPIDCD_AppInit !!!! "
#define MMIDCD_EXPORT_234_112_2_18_2_12_40_1 "[MMIDCD]: MMIAPIDCD_HandFlyModeSetChange enter"
#define MMIDCD_EXPORT_237_112_2_18_2_12_40_2 "[MMIDCD]: DCD in updating but phone is set in fly mode now!"
#define MMIDCD_FILE_214_112_2_18_2_12_41_3 "MMIDCD pageIndex = %d, item index = %d"
#define MMIDCD_FILE_234_112_2_18_2_12_41_4 "MMIDCD SetItemAlreadyRead pageIndex = %d, item index = %d"
#define MMIDCD_FILE_275_112_2_18_2_12_41_5 "MMIDCD GetXMLItemsCountPNULL != xml_ptr"
#define MMIDCD_FILE_297_112_2_18_2_12_41_6 "[MMIDCD]:MMIDCDFILE_HasFeedIdInXmlFile ptr PNULL error!!!!!!!!!!!!!!"
#define MMIDCD_FILE_303_112_2_18_2_12_41_7 "[MMIDCD] xml_feed_num = %d"
#define MMIDCD_FILE_335_112_2_18_2_12_41_8 "[MMIDCD]:MMIDCDFILE_HasReadEntryIdInXmlFile ptr PNULL ERROR!!!!!!!!!!!!!!"
#define MMIDCD_FILE_341_112_2_18_2_12_41_9 "[MMIDCD] feed_entry_num = %d"
#define MMIDCD_FILE_342_112_2_18_2_12_41_10 "[MMIDCD] entryId_ptr = %s"
#define MMIDCD_FILE_346_112_2_18_2_12_41_11 "[MMIDCD] old entryId_ptr = %s"
#define MMIDCD_FILE_354_112_2_18_2_12_41_12 "[MMIDCD] find readed item"
#define MMIDCD_FILE_358_112_2_18_2_12_41_13 "[MMIDCD]  same entry id "
#define MMIDCD_FILE_385_112_2_18_2_12_41_14 "[MMIDCD]:MMIDCD MMIDCDFILE_CmpTwoXmlFile has file null !!!!!!!!"
#define MMIDCD_FILE_394_112_2_18_2_12_41_15 "[MMIDCD] DCD_CmpTwoXmlFile enterd time = %d"
#define MMIDCD_FILE_397_112_2_18_2_12_41_16 "[MMIDCD] new_fend_num = %d"
#define MMIDCD_FILE_422_112_2_18_2_12_41_17 "[MMIDCD] find entry exist"
#define MMIDCD_FILE_431_112_2_18_2_12_41_18 "MMIDCD DCD_CmpTwoXmlFile find entry id = NULL !!!!"
#define MMIDCD_FILE_442_112_2_18_2_12_41_19 "MMIDCD new feedId = Null"
#define MMIDCD_FILE_448_112_2_18_2_12_41_20 "[MMIDCD] DCD_CmpTwoXmlFile EXIT time = %d"
#define MMIDCD_FILE_462_112_2_18_2_12_41_21 "MMIDCD pageIndex = %d, item index = %d"
#define MMIDCD_FILE_489_112_2_18_2_12_41_22 "Create xml File Fail"
#define MMIDCD_FILE_494_112_2_18_2_12_41_23 "WriteXMLNodeState get file path fail"
#define MMIDCD_FILE_525_112_2_18_2_12_42_24 "MMIDCD MMIDCDFILE_RenameXMLFile, index xml file name fail"
#define MMIDCD_FILE_531_112_2_18_2_12_42_25 "MMIDCD MMIDCDFILE_RenameXMLFile, ole xml file name fail"
#define MMIDCD_FILE_554_112_2_18_2_12_42_26 "MMIDCD Rename error dest file type!!"
#define MMIDCD_FILE_560_112_2_18_2_12_42_27 "MMIDCD Rename old file is not esist"
#define MMIDCD_FILE_565_112_2_18_2_12_42_28 "MMIDCD Rename two file name same !!"
#define MMIDCD_FILE_593_112_2_18_2_12_42_29 "[MMIDCD]:MMIDCDFFS_FileName2CtxtId ptr PNULL ERROR!!!!!!!!!!!!!!"
#define MMIDCD_FILE_668_112_2_18_2_12_42_30 "MMIDCD GetOnePictureFullPath get path fail"
#define MMIDCD_FILE_699_112_2_18_2_12_42_31 "MMIDCD DeletePictures Get path fail"
#define MMIDCD_FILE_784_112_2_18_2_12_42_32 "MMIDCD create pitcute dir fail"
#define MMIDCD_FILE_788_112_2_18_2_12_42_33 "MMIDCD create local pictute dir = %d"
#define MMIDCD_FILE_794_112_2_18_2_12_42_34 "MMIDCD create dcd main dir fail"
#define MMIDCD_FILE_844_112_2_18_2_12_42_35 "[MMIDCD]:GetDCDFileName default ERROR!!!!!!!!!!!!!!"
#define MMIDCD_FILE_851_112_2_18_2_12_42_36 "[MMIDCD]:GetDCDFileName ptr PNULL ERROR!!!!!!!!!!!!!!"
#define MMIDCD_FILE_886_112_2_18_2_12_42_37 "MMIDCD GetindexFilePathForRead file name is NULL"
#define MMIDCD_FILE_891_112_2_18_2_12_42_38 "MMIDCD PARA error!!!!!!!!!!"
#define MMIDCD_FILE_936_112_2_18_2_12_42_39 "MMIDCD PARA error!!!!!!!!!!!!"
#define MMIDCD_FILE_973_112_2_18_2_12_42_40 "MMIDCD GetindexFilePathForWrite file name is NULL"
#define MMIDCD_FILE_978_112_2_18_2_12_42_41 "MMIDCD FilePathForWrite PARA error!!!!!!!!!!!!"
#define MMIDCD_FILE_997_112_2_18_2_12_42_42 "MMIDCD get file name fail"
#define MMIDCD_FILE_1016_112_2_18_2_12_43_43 "MMIDCDFFS_CreateDefaultXMLFile bytes_written != default_dcd_xml_len"
#define MMIDCD_FILE_1024_112_2_18_2_12_43_44 "Create XML File Failed\n"
#define MMIDCD_FILE_1168_112_2_18_2_12_43_45 "MMIDCD DeleteLocalPictures Get path fail"
#define MMIDCD_FILE_1214_112_2_18_2_12_43_46 "[MMIDCD]:pageindex = %d, real_item_index=%d"
#define MMIDCD_FILE_1222_112_2_18_2_12_43_47 "[MMIDCD]:DeleteRealIndexItem pageindex = %d, real_item_index=%d"
#define MMIDCD_FILE_1283_112_2_18_2_12_43_48 "[MMIDCD]:DeletePageMultItem pageindex = %d, del_num=%d"
#define MMIDCD_FILE_1322_112_2_18_2_12_43_49 "[MMIDCD]:MMIDCD_GetRealItemIndex NOT find item"
#define MMIDCD_FILE_1387_112_2_18_2_12_43_50 "[MMIDCD]:MMIDCDFILE_GetPictureInfo ptr PNULL ERROR!!!!!!!!!!!!!!"
#define MMIDCD_FILE_1404_112_2_18_2_12_43_51 "MMIDCDFILE_GetPictureInfo readed_size = %d"
#define MMIDCD_FILE_1410_112_2_18_2_12_43_52 "MMIDCDFILE_GetPictureInfo read file error"
#define MMIDCD_FILE_1415_112_2_18_2_12_43_53 "MMIDCDFILE_GetPictureInfo get picture alloc fail"
#define MMIDCD_FILE_1420_112_2_18_2_12_43_54 "MMIDCDFILE_GetPictureInfo get picture size is zero , no data"
#define MMIDCD_FILE_1456_112_2_18_2_12_43_55 "MMIDCD MMIDCDFILE_GetPictureSize para error!!!!!!!!"
#define MMIDCD_FILE_1477_112_2_18_2_12_43_56 "MMIDCD RealIndexToShowIndex page_index = %d, real_item_index = %d"
#define MMIDCD_FILE_1492_112_2_18_2_12_43_57 "[MMIDCD]: MMIDCDFILE_RealIndexToShowIndex: not find !"
#define MMIDCD_FILE_1496_112_2_18_2_12_43_58 "[MMIDCD]: display_index =%d"
#define MMIDCD_FILE_1581_112_2_18_2_12_44_59 "MMIDCD InitalizeLocalFileHeaderInfo write_size = %d"
#define MMIDCD_FILE_1586_112_2_18_2_12_44_60 "MMIDCD InitalizeLocalFileHeaderInfo write file error"
#define MMIDCD_FILE_1610_112_2_18_2_12_44_61 "MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
#define MMIDCD_FILE_1617_112_2_18_2_12_44_62 "MMIDCD init header fail"
#define MMIDCD_FILE_1635_112_2_18_2_12_44_63 "MMIDCD init header fail less size than normal"
#define MMIDCD_FILE_1642_112_2_18_2_12_44_64 "MMIDCD GetLocalFileHeaderInfo readed_size = %d"
#define MMIDCD_FILE_1647_112_2_18_2_12_44_65 "MMIDCD GetLocalFileHeaderInfo read file error"
#define MMIDCD_FILE_1656_112_2_18_2_12_44_66 "MMIDCD GetLocalFileHeaderInfo create header fail, will init it"
#define MMIDCD_FILE_1675_112_2_18_2_12_44_67 "[MMIDCD]:MMIDCDFILE_WriteLocalFileHeaderInfo ptr PNULL ERROR!!!!!!!!!!!!!!"
#define MMIDCD_FILE_1681_112_2_18_2_12_44_68 "MMIDCD WriteLocalFileHeaderInfo, local xml file name fail"
#define MMIDCD_FILE_1694_112_2_18_2_12_44_69 "MMIDCD WriteLocalFileHeaderInfo write_size = %d"
#define MMIDCD_FILE_1699_112_2_18_2_12_44_70 "MMIDCD WriteLocalFileHeaderInfo write file error"
#define MMIDCD_FILE_1707_112_2_18_2_12_44_71 "MMIDCD WriteLocalFileHeaderInfo create handle = %d"
#define MMIDCD_FILE_1735_112_2_18_2_12_44_72 "MMIDCD init HeaderInfo write file error"
#define MMIDCD_FILE_1755_112_2_18_2_12_44_73 "MMIDCD MMIDCDFILE_GetLocalItemCount fail"
#define MMIDCD_FILE_1786_112_2_18_2_12_44_74 "MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
#define MMIDCD_FILE_1814_112_2_18_2_12_44_75 "MMIDCD GetFirstFreeIndex = %d"
#define MMIDCD_FILE_1839_112_2_18_2_12_44_76 "WriteItemInfoToLocal write_size = %d,offset = %d"
#define MMIDCD_FILE_1845_112_2_18_2_12_44_77 "MMIDCD WriteItemInfoToLocal !!!= %d"
#define MMIDCD_FILE_1856_112_2_18_2_12_44_78 "MMIDCD WriteItemTextToLocal write_size = %d"
#define MMIDCD_FILE_1895_112_2_18_2_12_44_79 "MMIDCD MMIDCDFILE_ReadLocalItemInfo, item_index = %d"
#define MMIDCD_FILE_1906_112_2_18_2_12_44_80 "MMIDCD MMIDCDFILE_ReadLocalItemInfo, local xml file name fail"
#define MMIDCD_FILE_1920_112_2_18_2_12_44_81 "MMIDCD ReadLocalItemInfo readed_size = %d,offset = %d"
#define MMIDCD_FILE_1925_112_2_18_2_12_44_82 "MMIDCD ReadLocalItemInfo read file error"
#define MMIDCD_FILE_1930_112_2_18_2_12_44_83 "ReadLocalItemInfo set file pointer error offset = %d"
#define MMIDCD_FILE_1961_112_2_18_2_12_44_84 "MMIDCD DeleteLocalItem write file error"
#define MMIDCD_FILE_1970_112_2_18_2_12_44_85 "MMIDCD DeleteLocalItem read file error"
#define MMIDCD_FILE_2012_112_2_18_2_12_44_86 "MMIDCD DeleteLocalItem write file error"
#define MMIDCD_FILE_2017_112_2_18_2_12_45_87 "MMIDCD DeleteLocalItem read file error"
#define MMIDCD_FILE_2067_112_2_18_2_12_45_88 "MMIDCD LocalIndexRelativeToAbs read file error"
#define MMIDCD_FILE_2084_112_2_18_2_12_45_89 "MMIDCD ReadItemsState, node xml file name fail"
#define MMIDCD_FILE_2193_112_2_18_2_12_45_90 "[MMIDCD]: MMIDCDFILE_DeleteUpdateInfo enter.."
#define MMIDCD_FILE_2198_112_2_18_2_12_45_91 "[MMIDCD]: MMIDCDFILE_DeleteUpdateInfo; the local.txt file delete ok!"
#define MMIDCD_FILE_2203_112_2_18_2_12_45_92 "[MMIDCD]: MMIDCDFILE_DeleteUpdateInfo; the local.txt file do not exist!"
#define MMIDCD_FILE_2244_112_2_18_2_12_45_93 "[MMIDCD]: MMIDCDFILE_WriteUpdateInfo write file error"
#define MMIDCD_FILE_2257_112_2_18_2_12_45_94 "[MMIDCD]: MMIDCDFILE_WriteUpdateInfo open file error"
#define MMIDCD_FILE_2284_112_2_18_2_12_45_95 "[MMIDCD]: MMIDCDFILE_WriteUpdateInfo read file error"
#define MMIDCD_FILE_2301_112_2_18_2_12_45_96 "[MMIDCD]: MMIDCDFILE_WriteUpdateInfo write file error"
#define MMIDCD_FILE_2306_112_2_18_2_12_45_97 "[MMIDCD]: MMIDCDFILE_WriteUpdateInfo SetFilePointer error!!"
#define MMIDCD_FILE_2310_112_2_18_2_12_45_98 "[MMIDCD]: MMIDCDFILE_WriteUpdateInfo !!!= %d"
#define MMIDCD_FILE_2336_112_2_18_2_12_45_99 "WriteItemInfoToLocal write_size = %d,offset = %d"
#define MMIDCD_FILE_2341_112_2_18_2_12_45_100 "WriteItemInfoToLocal write_size file error"
#define MMIDCD_FILE_2347_112_2_18_2_12_45_101 "[MMIDCD]: MMIDCDFILE_WriteUpdateInfo SetFilePointer error!!"
#define MMIDCD_FILE_2354_112_2_18_2_12_45_102 "[MMIDCD]: MMIDCDFILE_WriteUpdateInfo total info num out of range !!!"
#define MMIDCD_HTTP_204_112_2_18_2_12_46_103 "[MMIDCD]:DCD_HttpSessionInit"
#define MMIDCD_HTTP_250_112_2_18_2_12_46_104 "[MMIDCD]:MMIDCD_HttpSessionClose"
#define MMIDCD_HTTP_298_112_2_18_2_12_46_105 "[MMIDCD]:Http_ReqInit ptr PNULL ERROR!!!!!!!!!!!!!!"
#define MMIDCD_HTTP_651_112_2_18_2_12_47_106 "[MMIDCD]:Http_SendReq() enter"
#define MMIDCD_HTTP_686_112_2_18_2_12_47_107 "MMIDCD name = %s, index = %d"
#define MMIDCD_HTTP_739_112_2_18_2_12_47_108 "Http_SendReq() exit"
#define MMIDCD_HTTP_754_112_2_18_2_12_47_109 "Http_ContentDownloadProcess() enter"
#define MMIDCD_HTTP_777_112_2_18_2_12_47_110 "Http_ContentDownloadProcess() exit"
#define MMIDCD_HTTP_790_112_2_18_2_12_47_111 "Http_DownloadErrorHander() HTTP_REQ_TYPE_XML"
#define MMIDCD_HTTP_800_112_2_18_2_12_47_112 "Http_DownloadErrorHander() DCD_HTTP_REQ_TYPE_CONTENT"
#define MMIDCD_HTTP_813_112_2_18_2_12_47_113 "Http_DownloadErrorHander() handle unkown http error!!"
#define MMIDCD_HTTP_834_112_2_18_2_12_47_114 "Http_ContentInterpreteProcess() enter"
#define MMIDCD_HTTP_851_112_2_18_2_12_47_115 "[DCD] Return ,if invalid"
#define MMIDCD_HTTP_861_112_2_18_2_12_47_116 "[DCD]  Write file error!"
#define MMIDCD_HTTP_870_112_2_18_2_12_47_117 "[DCD]  Create file error!"
#define MMIDCD_HTTP_877_112_2_18_2_12_47_118 "[MMIDCD]:Http_ContentInterpreteProcess data_ptr pnull!!!!!!!"
#define MMIDCD_HTTP_885_112_2_18_2_12_47_119 "[DCD]  Interprete Over!"
#define MMIDCD_HTTP_908_112_2_18_2_12_47_120 "MMIDCD Http_ContentInterpreteProcess entered time = %d"
#define MMIDCD_HTTP_942_112_2_18_2_12_47_121 "[MMIDCD]  Http_ContentInterpreteProcess; READ FILE  ERROR!"
#define MMIDCD_HTTP_957_112_2_18_2_12_47_122 "MMIDCD  MMIAPIFMM_WriteFile error = %d"
#define MMIDCD_HTTP_973_112_2_18_2_12_47_123 "MMIDCD   Http_ContentInterpreteProcess exit time = %d"
#define MMIDCD_HTTP_987_112_2_18_2_12_47_124 "[MMIDCD]:Http_ErrorHandler; errorcode = %d"
#define MMIDCD_HTTP_1064_112_2_18_2_12_48_125 "[MMIDCD]:Http_DownloadXML() enter"
#define MMIDCD_HTTP_1070_112_2_18_2_12_48_126 "[MMIDCD]:Http_DownloadXML:Http_ReqListNum(s_dcd_http_req_list_needed)==0"
#define MMIDCD_HTTP_1080_112_2_18_2_12_48_127 "Http_DownloadXML() exit"
#define MMIDCD_HTTP_1180_112_2_18_2_12_48_128 "Http_DownloadXMLPre() enter"
#define MMIDCD_HTTP_1198_112_2_18_2_12_48_129 "MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
#define MMIDCD_HTTP_1236_112_2_18_2_12_48_130 "[MMIDCD]:DCD REQ TYPE :%s"
#define MMIDCD_HTTP_1237_112_2_18_2_12_48_131 "[MMIDCD]:DCD UA :%s"
#define MMIDCD_HTTP_1238_112_2_18_2_12_48_132 "[MMIDCD]:DCD URL :%s"
#define MMIDCD_HTTP_1280_112_2_18_2_12_48_133 "DCD_SetReqype is man"
#define MMIDCD_HTTP_1285_112_2_18_2_12_48_134 "DCD_SetReqype is ttl"
#define MMIDCD_HTTP_1290_112_2_18_2_12_48_135 "DCD_SetReqype is svr"
#define MMIDCD_HTTP_1295_112_2_18_2_12_48_136 "DCD_SetReqype is mc"
#define MMIDCD_HTTP_1300_112_2_18_2_12_48_137 "DCD_SetReqype is mr"
#define MMIDCD_HTTP_1305_112_2_18_2_12_48_138 "DCD_SetReqype is start"
#define MMIDCD_HTTP_1310_112_2_18_2_12_48_139 "DCD_SetReqype is retry"
#define MMIDCD_HTTP_1315_112_2_18_2_12_48_140 "DCD_SetReqype is data"
#define MMIDCD_HTTP_1320_112_2_18_2_12_48_141 "DCD_SetReqype is data2"
#define MMIDCD_HTTP_1325_112_2_18_2_12_48_142 "DCD_SetReqype is data3"
#define MMIDCD_HTTP_1328_112_2_18_2_12_48_143 "DCD_SetReqype is unknown"
#define MMIDCD_HTTP_1339_112_2_18_2_12_48_144 "[MMIDCD]:MMIDCD_SetReqCommonPara enter "
#define MMIDCD_HTTP_1436_112_2_18_2_12_48_145 "[MMIDCD]:Http_ContentReqHeaderInit ptr PNULL ERROR!!!!!!!!!!!!!!"
#define MMIDCD_HTTP_1496_112_2_18_2_12_48_146 "[MMIDCD]:MIMEDecodeByBuf; buf_ptr or len is NULL!!!!!!"
#define MMIDCD_HTTP_1506_112_2_18_2_12_48_147 " [MMIDCD]:Content-Type: can not be found, the file is invalid"
#define MMIDCD_HTTP_1514_112_2_18_2_12_48_148 " [MMIDCD]:image: can not be found, the file is invalid"
#define MMIDCD_HTTP_1532_112_2_18_2_12_48_149 " [MMIDCD]:Content-Id: can not be found, the file is invalid"
#define MMIDCD_HTTP_1553_112_2_18_2_12_49_150 " [MMIDCD]:Content-Length: can not be found, the file is invalid"
#define MMIDCD_HTTP_1572_112_2_18_2_12_49_151 "RULL 2:file len < *len_ptr, the file is invalid"
#define MMIDCD_MAIN_233_112_2_18_2_12_49_152 "[MMIDCD]: MMIDCD_GetLatestUpdateState; dcd latest error_type is %d"
#define MMIDCD_MAIN_306_112_2_18_2_12_49_153 "[MMIDCD] OUT OF RANGE OF DCD MSG"
#define MMIDCD_MAIN_352_112_2_18_2_12_50_154 "[MMIDCD]: DCD phone is in fly mode now"
#define MMIDCD_MAIN_358_112_2_18_2_12_50_155 "[MMIDCD]:  DCD sim card is not ok"
#define MMIDCD_MAIN_364_112_2_18_2_12_50_156 "[MMIDCD]:  DCD u disk is running"
#define MMIDCD_MAIN_374_112_2_18_2_12_50_157 "[MMIDCD]:  DCD mem size is not enough!!"
#define MMIDCD_MAIN_382_112_2_18_2_12_50_158 "[MMIDCD]:  current network is not cmcc "
#define MMIDCD_MAIN_388_112_2_18_2_12_50_159 "[MMIDCD]:  network is not valid"
#define MMIDCD_MAIN_396_112_2_18_2_12_50_160 "[MMIDCD]:  DCD service is not open"
#define MMIDCD_MAIN_402_112_2_18_2_12_50_161 "[MMIDCD]:  DCD is busy now"
#define MMIDCD_MAIN_407_112_2_18_2_12_50_162 "[MMIDCD]:  DCD status is ok now!"
#define MMIDCD_MAIN_437_112_2_18_2_12_50_163 "[MMIDCD]:DCD_Idle_ProcessMsg enter"
#define MMIDCD_MAIN_455_112_2_18_2_12_50_164 "[MMIDCD]: http init !!"
#define MMIDCD_MAIN_462_112_2_18_2_12_50_165 "[MMIDCD]: DCD_SIG_DL_CANCLE_REQ"
#define MMIDCD_MAIN_471_112_2_18_2_12_50_166 "[MMIDCD]: default enter"
#define MMIDCD_MAIN_481_112_2_18_2_12_50_167 "[MMIDCD]:DCD_XML_ProcessMsg enter"
#define MMIDCD_MAIN_488_112_2_18_2_12_50_168 "[MMIDCD]: http init cnf and xml download start!!"
#define MMIDCD_MAIN_575_112_2_18_2_12_50_169 "[MMIDCD]:DCD_MIME_ProcessMsg enter"
#define MMIDCD_MAIN_583_112_2_18_2_12_50_170 "MMIDCD Http_ContentDownloadProcess finish time = %d"
#define MMIDCD_MAIN_624_112_2_18_2_12_50_171 "[MMIDCD]: HTTP_SIG_GET_CNF, parse fail"
#define MMIDCD_MAIN_679_112_2_18_2_12_50_172 "[MMIDCD]:DCD_Null_ProcessMsg enter"
#define MMIDCD_MAIN_703_112_2_18_2_12_50_173 "[MMIDCD]: msg %s"
#define MMIDCD_MAIN_733_112_2_18_2_12_50_174 "[MMIDCD]: MMIDCD_SimErrorInd(),  sim_status= %d"
#define MMIDCD_MAIN_759_112_2_18_2_12_50_175 "HandleDCDProcessMsg() enter msg_id = %d"
#define MMIDCD_MAIN_769_112_2_18_2_12_50_176 "HandleDCDProcessMsg() DCD_SIG_XML_DL_REQ error_type = %d"
#define MMIDCD_MAIN_798_112_2_18_2_12_50_177 "[MMIDCD]:dcd pdp active fail!!!"
#define MMIDCD_MAIN_830_112_2_18_2_12_50_178 "[MMIDCD]: http init cnf and error!!"
#define MMIDCD_MAIN_835_112_2_18_2_12_50_179 "[MMIDCD]: http init cnf and ok!!!"
#define MMIDCD_MAIN_882_112_2_18_2_12_51_180 "[MMIDCD]: xml download is end!!"
#define MMIDCD_MAIN_906_112_2_18_2_12_51_181 "[MMIDCD]: receive DCD_SIG_DEFALUT_RETRY_TIMER_IND!"
#define MMIDCD_MAIN_985_112_2_18_2_12_51_182 "HandleDCDProcessMsg()  exit"
#define MMIDCD_MAIN_1035_112_2_18_2_12_51_183 "[MMIDCD] feed Ctxt_Id repeat = %s"
#define MMIDCD_MAIN_1074_112_2_18_2_12_51_184 "[MMIDCD] entry Ctxt_Id repeat = %s"
#define MMIDCD_MAIN_1107_112_2_18_2_12_51_185 "[MMIDCD]:ParseDCDXMLFileWhenPowerOn enter!!"
#define MMIDCD_MAIN_1118_112_2_18_2_12_51_186 "MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
#define MMIDCD_MAIN_1125_112_2_18_2_12_51_187 "[MMIDCD]:MMIDCD_Parse() case xml file parse return TRUE"
#define MMIDCD_MAIN_1144_112_2_18_2_12_51_188 "[MMIDCD]:MMIAPIDCD_Parse() case old xml file DCDXML_ParseXMLFile return FALSE"
#define MMIDCD_MAIN_1163_112_2_18_2_12_51_189 "[MMIDCD]:MMIDCD_Parse:global_meta_ttl= %d\n"
#define MMIDCD_MAIN_1166_112_2_18_2_12_51_190 "[MMIDCD]:MMIDCD_Parse() exit!!"
#define MMIDCD_MAIN_1190_112_2_18_2_12_51_191 "[MMIDCD]:MMIDCD_Parse() enter!!"
#define MMIDCD_MAIN_1201_112_2_18_2_12_51_192 "MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
#define MMIDCD_MAIN_1207_112_2_18_2_12_51_193 "[MMIDCD]:MMIDCD_Parse() case default xml parse!"
#define MMIDCD_MAIN_1225_112_2_18_2_12_51_194 "[MMIDCD]:MMIDCD_Parse() case xml file parse return TRUE"
#define MMIDCD_MAIN_1254_112_2_18_2_12_51_195 "[MMIDCD] local xml file parse fail"
#define MMIDCD_MAIN_1259_112_2_18_2_12_51_196 "[MMIDCD]MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
#define MMIDCD_MAIN_1275_112_2_18_2_12_52_197 "[MMIDCD]:MMIDCD_Parse() case xml file parse return FALSE"
#define MMIDCD_MAIN_1290_112_2_18_2_12_52_198 "[MMIDCD]:MMIAPIDCD_Parse() case old xml file DCDXML_ParseXMLFile return TRUE"
#define MMIDCD_MAIN_1296_112_2_18_2_12_52_199 "[MMIDCD]:MMIAPIDCD_Parse() case old xml file DCDXML_ParseXMLFile return FALSE"
#define MMIDCD_MAIN_1303_112_2_18_2_12_52_200 "[MMIDCD]:has parse fail or pasrse success but has zero feed, so use default xml"
#define MMIDCD_MAIN_1323_112_2_18_2_12_52_201 "[MMIDCD]:MMIDCD_Parse:global_meta_ttl= %d\n"
#define MMIDCD_MAIN_1326_112_2_18_2_12_52_202 "[MMIDCD]:MMIDCD_Parse() exit!!"
#define MMIDCD_MAIN_1347_112_2_18_2_12_52_203 "MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
#define MMIDCD_MAIN_1352_112_2_18_2_12_52_204 "[MMIDCD]: the index.xml file delete fail!"
#define MMIDCD_MAIN_1360_112_2_18_2_12_52_205 "MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
#define MMIDCD_MAIN_1366_112_2_18_2_12_52_206 "[MMIDCD]: the local.xml file delete fail!"
#define MMIDCD_MAIN_1374_112_2_18_2_12_52_207 "MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
#define MMIDCD_MAIN_1379_112_2_18_2_12_52_208 "[MMIDCD]: the old.xml file delete fail!"
#define MMIDCD_MAIN_1403_112_2_18_2_12_52_209 "[MMIDCD]:DCD_ActivatePDPcontext enter!"
#define MMIDCD_MAIN_1422_112_2_18_2_12_52_210 "Read fail!! return_val = %d"
#define MMIDCD_MAIN_1438_112_2_18_2_12_52_211 "[MMIDCD]:DCD_DeactivatePDPcontext enter!"
#define MMIDCD_MAIN_1465_112_2_18_2_12_52_212 "[MMIDCD]:DcdHandlePDPMsg enter!"
#define MMIDCD_MAIN_1475_112_2_18_2_12_52_213 "[MMIDCD]:dcd  pdp active success!!!!"
#define MMIDCD_MAIN_1480_112_2_18_2_12_52_214 "[MMIDCD]:dcd pdp active fail!!!"
#define MMIDCD_MAIN_1486_112_2_18_2_12_52_215 "[MMIDCD]:dcd  is busy , but accapt MMIPDP_ACTIVE_CNF message!!status = %d "
#define MMIDCD_MAIN_1492_112_2_18_2_12_52_216 "[MMIDCD]:dcd  pdp deactive is come!!!!"
#define MMIDCD_MAIN_1499_112_2_18_2_12_52_217 "[MMIDCD]:dcd  MMIPDP_DEACTIVE_IND msg is come!"
#define MMIDCD_MAIN_1507_112_2_18_2_12_52_218 "[MMIDCD]:DcdHandlePDPMsg exit!"
#define MMIDCD_MAIN_1542_112_2_18_2_12_52_219 "[MMIDCD]: MMIDCD_NetWorkIsValid sim is not ok"
#define MMIDCD_MAIN_1548_112_2_18_2_12_52_220 "[MMIDCD]: MMIDCD_NetWorkIsValid net type is not ok"
#define MMIDCD_MAIN_1554_112_2_18_2_12_52_221 "[MMIDCD]:  MMIDCD_NetWorkIsValid net is not valid"
#define MMIDCD_MAIN_1598_112_2_18_2_12_52_222 "[MMIDCD]:Current net mnc is: %d "
#define MMIDCD_MAIN_1666_112_2_18_2_12_52_223 "[MMIDCD]:PNULL != sync_pkg_ptr!!!!!!"
#define MMIDCD_MAIN_1671_112_2_18_2_12_52_224 "[MMIDCD]:MMIDCD_IsSynchronize: at present dcd ttl=%d\n"
#define MMIDCD_MAIN_1675_112_2_18_2_12_52_225 "[MMIDCD]:MMIDCD_IsSynchronize:dcd disable dcd ttl update!!\n"
#define MMIDCD_MAIN_1682_112_2_18_2_12_52_226 "[MMIDCD]:MMIDCD_IsSynchronize:ttl update is coming, resume next ttl: %d\n"
#define MMIDCD_MAIN_1714_112_2_18_2_12_52_227 "[MMIDCD]:DCD error retry update is the %d times, next timer is %d min !!"
#define MMIDCD_MAIN_1742_112_2_18_2_12_52_228 "[MMIDCD]: phone is in roaming status: %d"
#define MMIDCD_MAIN_1772_112_2_18_2_12_53_229 "[MMIDCD]MMIDCD_NotifyEventCallback dual_sys=%d, notify_event=%d"
#define MMIDCD_MAIN_1782_112_2_18_2_12_53_230 "[MMIDCD]:MMIDCD_NotifyEventCallback,  start retry update now! "
#define MMIDCD_MAIN_1813_112_2_18_2_12_53_231 "[MMIDCD]:DCD Retry start!"
#define MMIDCD_MAIN_1823_112_2_18_2_12_53_232 "[MMIDCD]:DCD TTL start!"
#define MMIDCD_MAIN_1835_112_2_18_2_12_53_233 "[MMIDCD] start timer, req_type = %d, second = %d"
#define MMIDCD_MAIN_1839_112_2_18_2_12_53_234 "[MMIDCD] start ttl timer begin, timer_id = %d"
#define MMIDCD_MAIN_1855_112_2_18_2_12_53_235 "[MMIDCD] start ttl timer end, timer_id = %d"
#define MMIDCD_MAIN_1859_112_2_18_2_12_53_236 "[MMIDCD] start retry timer begin, timer_id = %d"
#define MMIDCD_MAIN_1877_112_2_18_2_12_53_237 "[MMIDCD]:DCD error retry update is the %d times, next timer is %d second !!"
#define MMIDCD_MAIN_1881_112_2_18_2_12_53_238 "[MMIDCD] start retry timer end, timer_id = %d"
#define MMIDCD_MAIN_1894_112_2_18_2_12_53_239 "[MMIDCD] MMIDCD_SetDefaultRetryTime enter!"
#define MMIDCD_MAIN_1897_112_2_18_2_12_53_240 "[MMIDCD] MMIDCD_SetDefaultRetryTime, delete s_retry_timer"
#define MMIDCD_SETTING_67_112_2_18_2_12_53_241 "[MMIDCD] MMIAPIDCD_WriteSettingToNv is_dcd_on = %d"
#define MMIDCD_SETTING_76_112_2_18_2_12_54_242 "[MMIDCD]:: MMIDCD_SetSettingDefault enter! "
#define MMIDCD_SETTING_110_112_2_18_2_12_54_243 "[MMIDCD]:MMIDCD_InitSetting"
#define MMIDCD_SETTING_141_112_2_18_2_12_54_244 "[MMIDCD]:MMIDCD_Setting_GetNetSettingIndex,dual_sys=%d,index=%d"
#define MMIDCD_SETTING_156_112_2_18_2_12_54_245 "[MMIDCD]:MMIDCD_Setting_SetNetSettingIndex dual_sys=%d index=%d"
#define MMIDCD_SETTING_184_112_2_18_2_12_54_246 "MMIDCD_Setting_ConnectionCallback,index=%d, dual_sys=%d"
#define MMIDCD_WINTAB_893_112_2_18_2_12_56_247 "MMIDCD Read fail!!!"
#define MMIDCD_WINTAB_902_112_2_18_2_12_56_248 "[MMIDCD]:MMIAPIDCD_SetData() enter"
#define MMIDCD_WINTAB_922_112_2_18_2_12_56_249 "[MMIDCD]:MMIAPIDCD_SetData() exit"
#define MMIDCD_WINTAB_986_112_2_18_2_12_56_250 "MMIDCD Get index fail!!!"
#define MMIDCD_WINTAB_1088_112_2_18_2_12_56_251 "[MMIDCD]: MMIDCD_UpdateStart enter, dcd update start: %d"
#define MMIDCD_WINTAB_1135_112_2_18_2_12_56_252 "[MMIDCD]:DCD_AppendListItem invalid ctrl_id = %d"
#define MMIDCD_WINTAB_1215_112_2_18_2_12_56_253 "[MMIDCD] pageindex = %d, reaindex = %d"
#define MMIDCD_WINTAB_1297_112_2_18_2_12_57_254 "MMIDCD read item info fail, local index = %d"
#define MMIDCD_WINTAB_1464_112_2_18_2_12_57_255 "MMIDCD delete fail"
#define MMIDCD_WINTAB_1589_112_2_18_2_12_57_256 "MMIDCD please check list ctrl!!"
#define MMIDCD_WINTAB_1624_112_2_18_2_12_57_257 "[MMIDCD]:CreateDcdMarkMenu Get list data error!!!!!!!!!!!"
#define MMIDCD_WINTAB_1929_112_2_18_2_12_58_258 "MMIDCD anim has start"
#define MMIDCD_WINTAB_2254_112_2_18_2_12_59_259 "[MMIDCD]:MMIDCD_Error_Handler() enter MMIDCD_Error_Handler = %d"
#define MMIDCD_WINTAB_2320_112_2_18_2_12_59_260 "[MMIDCD]:MMIDCD_IdleWinWakeupInd() enter "
#define MMIDCD_WINTAB_2323_112_2_18_2_12_59_261 "[MMIDCD]:dcd will be wake up !! "
#define MMIDCD_WINTAB_2339_112_2_18_2_12_59_262 "[MMIDCD]:MMIDCD_Wakeup() enter: "
#define MMIDCD_WINTAB_2364_112_2_18_2_12_59_263 "[MMIDCD]:MMIDCD_IndicationShow() enter: "
#define MMIDCD_WINTAB_2402_112_2_18_2_12_59_264 "[MMIDCD]:start retry update mechanism!!! "
#define MMIDCD_WINTAB_2433_112_2_18_2_12_59_265 "[MMIDCD]:MMIDCD_IndicationShow Please check here error!!!!!!!!!"
#define MMIDCD_WINTAB_2484_112_2_18_2_12_59_266 "[MMIDCD]:s_dcd_gui_struct.total_page_num out of max range  error!!!!!!!! "
#define MMIDCD_WINTAB_2520_112_2_18_2_12_59_267 "DCD_AddTabsAndChildWindows: tab id = %d, page_num = %d!"
#define MMIDCD_WINTAB_2535_112_2_18_2_12_59_268 "DCD_AddTabsAndChildWindows: idle tab!"
#define MMIDCD_WINTAB_2542_112_2_18_2_12_59_269 "DCD_AddTabsAndChildWindows: not idle tab!"
#define MMIDCD_WINTAB_2672_112_2_18_2_13_0_270 "MMIDCD_SetAnimateCtrlState state = %d"
#define MMIDCD_WINTAB_2697_112_2_18_2_13_0_271 "MMIDCD SetDCDAnimateCtrlState has entered"
#define MMIDCD_WINTAB_2702_112_2_18_2_13_0_272 "MMIDCD pauseanim has done"
#define MMIDCD_WINTAB_2732_112_2_18_2_13_0_273 "[MMIDCD]: the invalid pic file delete ok!"
#define MMIDCD_WINTAB_2736_112_2_18_2_13_0_274 "[MMIDCD]: the invalid pic file do not exist!"
#define MMIDCD_WINTAB_2763_112_2_18_2_13_0_275 "[MMIDCD]: the invalid pic file delete ok!"
#define MMIDCD_WINTAB_2767_112_2_18_2_13_0_276 "[MMIDCD]: the invalid pic file do not exist!"
#define MMIDCD_WINTAB_2943_112_2_18_2_13_0_277 "[MMIDCD]:MMIDCD_UpdateFrame() enter"
#define MMIDCD_WINTAB_2965_112_2_18_2_13_0_278 "[MMIDCD]:MMIDCD_UpdateFrame() exit"
#define MMIDCD_WINTAB_3003_112_2_18_2_13_0_279 "MMICD will show file"
#define MMIDCD_WINTAB_3179_112_2_18_2_13_1_280 "MMICD will show file"
#define MMIDCD_WINTAB_3184_112_2_18_2_13_1_281 "MMIDCD find no picture "
#define MMIDCD_WINTAB_3355_112_2_18_2_13_1_282 "MMICD will show file"
#define MMIDCD_WINTAB_3360_112_2_18_2_13_1_283 "MMIDCD find no picture "
#define MMIDCD_WINTAB_4134_112_2_18_2_13_2_284 "[MMIDCD] MMIDCDAPISET_FuncFinishedInd; MMIDCD_STOP_UPDATING_WAITING_WIN_ID is open!"
#define MMIDCD_WINTAB_4139_112_2_18_2_13_2_285 "[MMIDCD] MMIDCDAPISET_FuncFinishedInd; MMIDCD_RESET_FACTORY_WAITING_WIN_ID is open!"
#define MMIDCD_WINTAB_4160_112_2_18_2_13_3_286 "[MMIDCD]: MSG_DCD_UPDATE_FINISH is come!"
#define MMIDCD_WINTAB_4674_112_2_18_2_13_4_287 "MMIDCD GetDcdItemLinkAdr read  fail"
#define MMIDCD_WINTAB_4696_112_2_18_2_13_4_288 "MMIDCD find no link_ptr"
#define MMIDCD_WINTAB_4821_112_2_18_2_13_4_289 "[MMIDCD] curitem,  item_num= %d"
#define MMIDCD_WINTAB_4942_112_2_18_2_13_4_290 "[MMIDCD] curitem,  item_num= %d"
#define MMIDCD_WINTAB_4948_112_2_18_2_13_4_291 "MMIDCD HandDCDPrewItemSwitch DATA has changed"
#define MMIDCD_WINTAB_6075_112_2_18_2_13_6_292 "ResetDCDApplet_HandleEvent msg_id = 0x%04x"
#define MMIDCD_WINTAB_6081_112_2_18_2_13_6_293 "ResetDCDApplet_HandleEventMSG_APPLET_START"
#define MMIDCD_WINTAB_6098_112_2_18_2_13_6_294 "[MMIDCD]: MSG_SET_RESET_FACTORY_OVER_IND received!"
#define MMIDCD_WINTAB_6136_112_2_18_2_13_6_295 "[MMIDCD]:MMIDCD_ShowUpdateCompelete req_type = %d, error_code = %d"
#define MMIDCD_WINTAB_6140_112_2_18_2_13_6_296 "[MMIDCD]: this not need process "
#define MMIDCD_WINTAB_6166_112_2_18_2_13_6_297 "MMIDCD s_data_has_changed = %d"
#define MMIDCD_WINTAB_6174_112_2_18_2_13_6_298 "MMIDCD GetDataChangeFlag s_data_has_changed = %d"
#define MMIDCD_XML_PARSE_785_112_2_18_2_13_8_299 "[MMIDCD]:UTF82UCS2B ERROR!!!!!!!!!!!!!!"
#define MMIDCD_XML_PARSE_919_112_2_18_2_13_9_300 "GetATTRIBUTE_AttributeID() str='%s'	"
#define MMIDCD_XML_PARSE_953_112_2_18_2_13_9_301 "DCD XML INVALID DCDXML_EL_ID_TITLE"
#define MMIDCD_XML_PARSE_972_112_2_18_2_13_9_302 "DCD XML INVALID DCDXML_EL_ID_SUMMARY"
#define MMIDCD_XML_PARSE_987_112_2_18_2_13_9_303 "DCD XML INVALID DCDXML_EL_ID_ISSUED"
#define MMIDCD_XML_PARSE_1002_112_2_18_2_13_9_304 "DCD XML INVALID DCDXML_EL_ID_ACTIVATION"
#define MMIDCD_XML_PARSE_1017_112_2_18_2_13_9_305 "DCD XML INVALID DCDXML_EL_ID_EXPIRATION"
#define MMIDCD_XML_PARSE_1032_112_2_18_2_13_9_306 "DCD XML INVALID DCDXML_EL_ID_AUTHOR"
#define MMIDCD_XML_PARSE_1048_112_2_18_2_13_9_307 "DCD XML INVALID DCDXML_EL_ID_NAME"
#define MMIDCD_XML_PARSE_1063_112_2_18_2_13_9_308 "DCD XML INVALID DCDXML_EL_ID_URL"
#define MMIDCD_XML_PARSE_1078_112_2_18_2_13_9_309 "DCD XML INVALID DCDXML_EL_ID_EMAIL"
#define MMIDCD_XML_PARSE_1084_112_2_18_2_13_9_310 "DCD XML INVALID chardata = %d"
#define MMIDCD_XML_PARSE_1142_112_2_18_2_13_9_311 "DCD XML INVALID DCDXML_EL_ID_INVALID_HTML"
#define MMIDCD_XML_PARSE_1191_112_2_18_2_13_9_312 "DCD XML INVALID  MAX ID MetaParser "
#define MMIDCD_XML_PARSE_1197_112_2_18_2_13_9_313 "DCD XML INVALID  MetaParser"
#define MMIDCD_XML_PARSE_1242_112_2_18_2_13_9_314 "DCD XML INVALID  MAX ID FeedParser "
#define MMIDCD_XML_PARSE_1248_112_2_18_2_13_9_315 "DCD XML INVALID  FeedParser"
#define MMIDCD_XML_PARSE_1304_112_2_18_2_13_10_316 "DCD XML INVALID  MAX ID EntryParser "
#define MMIDCD_XML_PARSE_1310_112_2_18_2_13_10_317 "DCD XML INVALID  EntryParser"
#define MMIDCD_XML_PARSE_1356_112_2_18_2_13_10_318 "DCD XML INVALID  MAX ID MetaParser "
#define MMIDCD_XML_PARSE_1404_112_2_18_2_13_10_319 "DCD XML INVALID  parser_status = %d"
#define MMIDCD_XML_PARSE_1478_112_2_18_2_13_10_320 "MMIDCD_XmlParse() enter"
#define MMIDCD_XML_PARSE_1492_112_2_18_2_13_10_321 "MMIDCD_XmlParse(): parsed_stream fail"
#define MMIDCD_XML_PARSE_1501_112_2_18_2_13_10_322 "MMIDCD_XmlParse(): parsed_stream fail"
#define MMIDCD_XML_PARSE_1510_112_2_18_2_13_10_323 "MMIDCD_XmlParse(): create xml parser fail"
#define MMIDCD_XML_PARSE_1570_112_2_18_2_13_10_324 "DCDXML_ParseStaticFile() enter"
#define MMIDCD_XML_PARSE_1584_112_2_18_2_13_10_325 "create xml parser fail"
#define MMIDCD_XML_PARSE_1638_112_2_18_2_13_10_326 "DCDXML_ParseStaticFile():Parse error at line %dn %sn"
#define MMIDCD_XML_PARSE_1660_112_2_18_2_13_10_327 "GetLinkRel() str='%s'	"
#define MMIDCD_XML_PARSE_1676_112_2_18_2_13_10_328 "GetLinkType() str='%s'	"
#define MMIDCD_XML_PARSE_1690_112_2_18_2_13_10_329 "GetContentMode() str='%s'	"
#define MMIDCD_XML_PARSE_1705_112_2_18_2_13_10_330 "GetContentType() str='%s'	"
#define MMIDCD_XML_PARSE_1719_112_2_18_2_13_10_331 "GetContentName() str='%s'	"
#define MMIDCD_XML_PARSE_1733_112_2_18_2_13_10_332 "GetTemplateId() str='%s'	"
#define MMIDCD_XML_PARSE_1752_112_2_18_2_13_10_333 "GetEntryType() str='%s'	"
#define MMIDCD_XML_PARSE_1769_112_2_18_2_13_10_334 "GetFeedType() str='%s'	"
#define MMIDCD_XML_PARSE_1785_112_2_18_2_13_11_335 "GetFeedType() str='%s'	"
#define MMIDCD_XML_PARSE_1800_112_2_18_2_13_11_336 "GetTimeType() str='%s'	"
#define MMIDCD_XML_PARSE_2775_112_2_18_2_13_13_337 "DCD_FeedMetaListAdd() enter"
#define MMIDCD_XML_PARSE_2832_112_2_18_2_13_13_338 "DCD_ContentListAdd() enter"
#define MMIDCD_XML_PARSE_2888_112_2_18_2_13_13_339 "DCD_EntryListAdd() enter"
#define MMIDCD_XML_PARSE_2945_112_2_18_2_13_13_340 "[MMIDCD]:FeedAdd2List ptr PNULL ERROR!!!!!!!!!!!!!!"
#define MMIDCD_XML_PARSE_2949_112_2_18_2_13_13_341 "DCD_FeedListAdd() enter"
#define MMIDCD_XML_PARSE_3007_112_2_18_2_13_13_342 "[MMIDCD]:ContentIsExistInEntry ptr PNULL ERROR!!!!!!!!!!!!!!"
#define MMIDCD_XML_PARSE_3085_112_2_18_2_13_13_343 "MMIDCD nend not download picutre"
#define MMIDCD_XML_PARSE_3858_112_2_18_2_13_15_344 "[MMIDCD]:MMIDCD_GetFeedCTXTId ptr PNULL ERROR!!!!!!!!!!!!!!"
#define MMIDCD_XML_PARSE_3874_112_2_18_2_13_15_345 "MMI DCD this index has no chanel"
#define MMIDCD_XML_PARSE_4063_112_2_18_2_13_15_346 "MMIDCD content_ptr no cmp"
#define MMIDCD_XML_PARSE_4069_112_2_18_2_13_15_347 "MMIDCD content_ptr is null now"
#define MMIDCD_XML_PARSE_4074_112_2_18_2_13_15_348 "MMIDCD feed_ptr is null now"
#define MMIDCD_XML_PARSE_4114_112_2_18_2_13_15_349 "[MMIDCD]:MMIAPIDCD_GetEntryContentInfo type dont matching ERROR!!!!!!!!!!"
#define MMIDCD_XML_PARSE_4150_112_2_18_2_13_15_350 "[MMIDCD]:MMIDCD_GetEntryLink xml type dont matching ERROR!!!!!!!!!!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_DCD_TRC)
TRACE_MSG(MMIDCD_EXPORT_98_112_2_18_2_12_39_0,"[MMIDCD]: MMIAPIDCD_AppInit !!!! ")
TRACE_MSG(MMIDCD_EXPORT_234_112_2_18_2_12_40_1,"[MMIDCD]: MMIAPIDCD_HandFlyModeSetChange enter")
TRACE_MSG(MMIDCD_EXPORT_237_112_2_18_2_12_40_2,"[MMIDCD]: DCD in updating but phone is set in fly mode now!")
TRACE_MSG(MMIDCD_FILE_214_112_2_18_2_12_41_3,"MMIDCD pageIndex = %d, item index = %d")
TRACE_MSG(MMIDCD_FILE_234_112_2_18_2_12_41_4,"MMIDCD SetItemAlreadyRead pageIndex = %d, item index = %d")
TRACE_MSG(MMIDCD_FILE_275_112_2_18_2_12_41_5,"MMIDCD GetXMLItemsCountPNULL != xml_ptr")
TRACE_MSG(MMIDCD_FILE_297_112_2_18_2_12_41_6,"[MMIDCD]:MMIDCDFILE_HasFeedIdInXmlFile ptr PNULL error!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_FILE_303_112_2_18_2_12_41_7,"[MMIDCD] xml_feed_num = %d")
TRACE_MSG(MMIDCD_FILE_335_112_2_18_2_12_41_8,"[MMIDCD]:MMIDCDFILE_HasReadEntryIdInXmlFile ptr PNULL ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_FILE_341_112_2_18_2_12_41_9,"[MMIDCD] feed_entry_num = %d")
TRACE_MSG(MMIDCD_FILE_342_112_2_18_2_12_41_10,"[MMIDCD] entryId_ptr = %s")
TRACE_MSG(MMIDCD_FILE_346_112_2_18_2_12_41_11,"[MMIDCD] old entryId_ptr = %s")
TRACE_MSG(MMIDCD_FILE_354_112_2_18_2_12_41_12,"[MMIDCD] find readed item")
TRACE_MSG(MMIDCD_FILE_358_112_2_18_2_12_41_13,"[MMIDCD]  same entry id ")
TRACE_MSG(MMIDCD_FILE_385_112_2_18_2_12_41_14,"[MMIDCD]:MMIDCD MMIDCDFILE_CmpTwoXmlFile has file null !!!!!!!!")
TRACE_MSG(MMIDCD_FILE_394_112_2_18_2_12_41_15,"[MMIDCD] DCD_CmpTwoXmlFile enterd time = %d")
TRACE_MSG(MMIDCD_FILE_397_112_2_18_2_12_41_16,"[MMIDCD] new_fend_num = %d")
TRACE_MSG(MMIDCD_FILE_422_112_2_18_2_12_41_17,"[MMIDCD] find entry exist")
TRACE_MSG(MMIDCD_FILE_431_112_2_18_2_12_41_18,"MMIDCD DCD_CmpTwoXmlFile find entry id = NULL !!!!")
TRACE_MSG(MMIDCD_FILE_442_112_2_18_2_12_41_19,"MMIDCD new feedId = Null")
TRACE_MSG(MMIDCD_FILE_448_112_2_18_2_12_41_20,"[MMIDCD] DCD_CmpTwoXmlFile EXIT time = %d")
TRACE_MSG(MMIDCD_FILE_462_112_2_18_2_12_41_21,"MMIDCD pageIndex = %d, item index = %d")
TRACE_MSG(MMIDCD_FILE_489_112_2_18_2_12_41_22,"Create xml File Fail")
TRACE_MSG(MMIDCD_FILE_494_112_2_18_2_12_41_23,"WriteXMLNodeState get file path fail")
TRACE_MSG(MMIDCD_FILE_525_112_2_18_2_12_42_24,"MMIDCD MMIDCDFILE_RenameXMLFile, index xml file name fail")
TRACE_MSG(MMIDCD_FILE_531_112_2_18_2_12_42_25,"MMIDCD MMIDCDFILE_RenameXMLFile, ole xml file name fail")
TRACE_MSG(MMIDCD_FILE_554_112_2_18_2_12_42_26,"MMIDCD Rename error dest file type!!")
TRACE_MSG(MMIDCD_FILE_560_112_2_18_2_12_42_27,"MMIDCD Rename old file is not esist")
TRACE_MSG(MMIDCD_FILE_565_112_2_18_2_12_42_28,"MMIDCD Rename two file name same !!")
TRACE_MSG(MMIDCD_FILE_593_112_2_18_2_12_42_29,"[MMIDCD]:MMIDCDFFS_FileName2CtxtId ptr PNULL ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_FILE_668_112_2_18_2_12_42_30,"MMIDCD GetOnePictureFullPath get path fail")
TRACE_MSG(MMIDCD_FILE_699_112_2_18_2_12_42_31,"MMIDCD DeletePictures Get path fail")
TRACE_MSG(MMIDCD_FILE_784_112_2_18_2_12_42_32,"MMIDCD create pitcute dir fail")
TRACE_MSG(MMIDCD_FILE_788_112_2_18_2_12_42_33,"MMIDCD create local pictute dir = %d")
TRACE_MSG(MMIDCD_FILE_794_112_2_18_2_12_42_34,"MMIDCD create dcd main dir fail")
TRACE_MSG(MMIDCD_FILE_844_112_2_18_2_12_42_35,"[MMIDCD]:GetDCDFileName default ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_FILE_851_112_2_18_2_12_42_36,"[MMIDCD]:GetDCDFileName ptr PNULL ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_FILE_886_112_2_18_2_12_42_37,"MMIDCD GetindexFilePathForRead file name is NULL")
TRACE_MSG(MMIDCD_FILE_891_112_2_18_2_12_42_38,"MMIDCD PARA error!!!!!!!!!!")
TRACE_MSG(MMIDCD_FILE_936_112_2_18_2_12_42_39,"MMIDCD PARA error!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_FILE_973_112_2_18_2_12_42_40,"MMIDCD GetindexFilePathForWrite file name is NULL")
TRACE_MSG(MMIDCD_FILE_978_112_2_18_2_12_42_41,"MMIDCD FilePathForWrite PARA error!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_FILE_997_112_2_18_2_12_42_42,"MMIDCD get file name fail")
TRACE_MSG(MMIDCD_FILE_1016_112_2_18_2_12_43_43,"MMIDCDFFS_CreateDefaultXMLFile bytes_written != default_dcd_xml_len")
TRACE_MSG(MMIDCD_FILE_1024_112_2_18_2_12_43_44,"Create XML File Failed\n")
TRACE_MSG(MMIDCD_FILE_1168_112_2_18_2_12_43_45,"MMIDCD DeleteLocalPictures Get path fail")
TRACE_MSG(MMIDCD_FILE_1214_112_2_18_2_12_43_46,"[MMIDCD]:pageindex = %d, real_item_index=%d")
TRACE_MSG(MMIDCD_FILE_1222_112_2_18_2_12_43_47,"[MMIDCD]:DeleteRealIndexItem pageindex = %d, real_item_index=%d")
TRACE_MSG(MMIDCD_FILE_1283_112_2_18_2_12_43_48,"[MMIDCD]:DeletePageMultItem pageindex = %d, del_num=%d")
TRACE_MSG(MMIDCD_FILE_1322_112_2_18_2_12_43_49,"[MMIDCD]:MMIDCD_GetRealItemIndex NOT find item")
TRACE_MSG(MMIDCD_FILE_1387_112_2_18_2_12_43_50,"[MMIDCD]:MMIDCDFILE_GetPictureInfo ptr PNULL ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_FILE_1404_112_2_18_2_12_43_51,"MMIDCDFILE_GetPictureInfo readed_size = %d")
TRACE_MSG(MMIDCD_FILE_1410_112_2_18_2_12_43_52,"MMIDCDFILE_GetPictureInfo read file error")
TRACE_MSG(MMIDCD_FILE_1415_112_2_18_2_12_43_53,"MMIDCDFILE_GetPictureInfo get picture alloc fail")
TRACE_MSG(MMIDCD_FILE_1420_112_2_18_2_12_43_54,"MMIDCDFILE_GetPictureInfo get picture size is zero , no data")
TRACE_MSG(MMIDCD_FILE_1456_112_2_18_2_12_43_55,"MMIDCD MMIDCDFILE_GetPictureSize para error!!!!!!!!")
TRACE_MSG(MMIDCD_FILE_1477_112_2_18_2_12_43_56,"MMIDCD RealIndexToShowIndex page_index = %d, real_item_index = %d")
TRACE_MSG(MMIDCD_FILE_1492_112_2_18_2_12_43_57,"[MMIDCD]: MMIDCDFILE_RealIndexToShowIndex: not find !")
TRACE_MSG(MMIDCD_FILE_1496_112_2_18_2_12_43_58,"[MMIDCD]: display_index =%d")
TRACE_MSG(MMIDCD_FILE_1581_112_2_18_2_12_44_59,"MMIDCD InitalizeLocalFileHeaderInfo write_size = %d")
TRACE_MSG(MMIDCD_FILE_1586_112_2_18_2_12_44_60,"MMIDCD InitalizeLocalFileHeaderInfo write file error")
TRACE_MSG(MMIDCD_FILE_1610_112_2_18_2_12_44_61,"MMIDCD GetLocalFileHeaderInfo, local xml file name fail")
TRACE_MSG(MMIDCD_FILE_1617_112_2_18_2_12_44_62,"MMIDCD init header fail")
TRACE_MSG(MMIDCD_FILE_1635_112_2_18_2_12_44_63,"MMIDCD init header fail less size than normal")
TRACE_MSG(MMIDCD_FILE_1642_112_2_18_2_12_44_64,"MMIDCD GetLocalFileHeaderInfo readed_size = %d")
TRACE_MSG(MMIDCD_FILE_1647_112_2_18_2_12_44_65,"MMIDCD GetLocalFileHeaderInfo read file error")
TRACE_MSG(MMIDCD_FILE_1656_112_2_18_2_12_44_66,"MMIDCD GetLocalFileHeaderInfo create header fail, will init it")
TRACE_MSG(MMIDCD_FILE_1675_112_2_18_2_12_44_67,"[MMIDCD]:MMIDCDFILE_WriteLocalFileHeaderInfo ptr PNULL ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_FILE_1681_112_2_18_2_12_44_68,"MMIDCD WriteLocalFileHeaderInfo, local xml file name fail")
TRACE_MSG(MMIDCD_FILE_1694_112_2_18_2_12_44_69,"MMIDCD WriteLocalFileHeaderInfo write_size = %d")
TRACE_MSG(MMIDCD_FILE_1699_112_2_18_2_12_44_70,"MMIDCD WriteLocalFileHeaderInfo write file error")
TRACE_MSG(MMIDCD_FILE_1707_112_2_18_2_12_44_71,"MMIDCD WriteLocalFileHeaderInfo create handle = %d")
TRACE_MSG(MMIDCD_FILE_1735_112_2_18_2_12_44_72,"MMIDCD init HeaderInfo write file error")
TRACE_MSG(MMIDCD_FILE_1755_112_2_18_2_12_44_73,"MMIDCD MMIDCDFILE_GetLocalItemCount fail")
TRACE_MSG(MMIDCD_FILE_1786_112_2_18_2_12_44_74,"MMIDCD GetLocalFileHeaderInfo, local xml file name fail")
TRACE_MSG(MMIDCD_FILE_1814_112_2_18_2_12_44_75,"MMIDCD GetFirstFreeIndex = %d")
TRACE_MSG(MMIDCD_FILE_1839_112_2_18_2_12_44_76,"WriteItemInfoToLocal write_size = %d,offset = %d")
TRACE_MSG(MMIDCD_FILE_1845_112_2_18_2_12_44_77,"MMIDCD WriteItemInfoToLocal !!!= %d")
TRACE_MSG(MMIDCD_FILE_1856_112_2_18_2_12_44_78,"MMIDCD WriteItemTextToLocal write_size = %d")
TRACE_MSG(MMIDCD_FILE_1895_112_2_18_2_12_44_79,"MMIDCD MMIDCDFILE_ReadLocalItemInfo, item_index = %d")
TRACE_MSG(MMIDCD_FILE_1906_112_2_18_2_12_44_80,"MMIDCD MMIDCDFILE_ReadLocalItemInfo, local xml file name fail")
TRACE_MSG(MMIDCD_FILE_1920_112_2_18_2_12_44_81,"MMIDCD ReadLocalItemInfo readed_size = %d,offset = %d")
TRACE_MSG(MMIDCD_FILE_1925_112_2_18_2_12_44_82,"MMIDCD ReadLocalItemInfo read file error")
TRACE_MSG(MMIDCD_FILE_1930_112_2_18_2_12_44_83,"ReadLocalItemInfo set file pointer error offset = %d")
TRACE_MSG(MMIDCD_FILE_1961_112_2_18_2_12_44_84,"MMIDCD DeleteLocalItem write file error")
TRACE_MSG(MMIDCD_FILE_1970_112_2_18_2_12_44_85,"MMIDCD DeleteLocalItem read file error")
TRACE_MSG(MMIDCD_FILE_2012_112_2_18_2_12_44_86,"MMIDCD DeleteLocalItem write file error")
TRACE_MSG(MMIDCD_FILE_2017_112_2_18_2_12_45_87,"MMIDCD DeleteLocalItem read file error")
TRACE_MSG(MMIDCD_FILE_2067_112_2_18_2_12_45_88,"MMIDCD LocalIndexRelativeToAbs read file error")
TRACE_MSG(MMIDCD_FILE_2084_112_2_18_2_12_45_89,"MMIDCD ReadItemsState, node xml file name fail")
TRACE_MSG(MMIDCD_FILE_2193_112_2_18_2_12_45_90,"[MMIDCD]: MMIDCDFILE_DeleteUpdateInfo enter..")
TRACE_MSG(MMIDCD_FILE_2198_112_2_18_2_12_45_91,"[MMIDCD]: MMIDCDFILE_DeleteUpdateInfo; the local.txt file delete ok!")
TRACE_MSG(MMIDCD_FILE_2203_112_2_18_2_12_45_92,"[MMIDCD]: MMIDCDFILE_DeleteUpdateInfo; the local.txt file do not exist!")
TRACE_MSG(MMIDCD_FILE_2244_112_2_18_2_12_45_93,"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo write file error")
TRACE_MSG(MMIDCD_FILE_2257_112_2_18_2_12_45_94,"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo open file error")
TRACE_MSG(MMIDCD_FILE_2284_112_2_18_2_12_45_95,"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo read file error")
TRACE_MSG(MMIDCD_FILE_2301_112_2_18_2_12_45_96,"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo write file error")
TRACE_MSG(MMIDCD_FILE_2306_112_2_18_2_12_45_97,"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo SetFilePointer error!!")
TRACE_MSG(MMIDCD_FILE_2310_112_2_18_2_12_45_98,"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo !!!= %d")
TRACE_MSG(MMIDCD_FILE_2336_112_2_18_2_12_45_99,"WriteItemInfoToLocal write_size = %d,offset = %d")
TRACE_MSG(MMIDCD_FILE_2341_112_2_18_2_12_45_100,"WriteItemInfoToLocal write_size file error")
TRACE_MSG(MMIDCD_FILE_2347_112_2_18_2_12_45_101,"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo SetFilePointer error!!")
TRACE_MSG(MMIDCD_FILE_2354_112_2_18_2_12_45_102,"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo total info num out of range !!!")
TRACE_MSG(MMIDCD_HTTP_204_112_2_18_2_12_46_103,"[MMIDCD]:DCD_HttpSessionInit")
TRACE_MSG(MMIDCD_HTTP_250_112_2_18_2_12_46_104,"[MMIDCD]:MMIDCD_HttpSessionClose")
TRACE_MSG(MMIDCD_HTTP_298_112_2_18_2_12_46_105,"[MMIDCD]:Http_ReqInit ptr PNULL ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_HTTP_651_112_2_18_2_12_47_106,"[MMIDCD]:Http_SendReq() enter")
TRACE_MSG(MMIDCD_HTTP_686_112_2_18_2_12_47_107,"MMIDCD name = %s, index = %d")
TRACE_MSG(MMIDCD_HTTP_739_112_2_18_2_12_47_108,"Http_SendReq() exit")
TRACE_MSG(MMIDCD_HTTP_754_112_2_18_2_12_47_109,"Http_ContentDownloadProcess() enter")
TRACE_MSG(MMIDCD_HTTP_777_112_2_18_2_12_47_110,"Http_ContentDownloadProcess() exit")
TRACE_MSG(MMIDCD_HTTP_790_112_2_18_2_12_47_111,"Http_DownloadErrorHander() HTTP_REQ_TYPE_XML")
TRACE_MSG(MMIDCD_HTTP_800_112_2_18_2_12_47_112,"Http_DownloadErrorHander() DCD_HTTP_REQ_TYPE_CONTENT")
TRACE_MSG(MMIDCD_HTTP_813_112_2_18_2_12_47_113,"Http_DownloadErrorHander() handle unkown http error!!")
TRACE_MSG(MMIDCD_HTTP_834_112_2_18_2_12_47_114,"Http_ContentInterpreteProcess() enter")
TRACE_MSG(MMIDCD_HTTP_851_112_2_18_2_12_47_115,"[DCD] Return ,if invalid")
TRACE_MSG(MMIDCD_HTTP_861_112_2_18_2_12_47_116,"[DCD]  Write file error!")
TRACE_MSG(MMIDCD_HTTP_870_112_2_18_2_12_47_117,"[DCD]  Create file error!")
TRACE_MSG(MMIDCD_HTTP_877_112_2_18_2_12_47_118,"[MMIDCD]:Http_ContentInterpreteProcess data_ptr pnull!!!!!!!")
TRACE_MSG(MMIDCD_HTTP_885_112_2_18_2_12_47_119,"[DCD]  Interprete Over!")
TRACE_MSG(MMIDCD_HTTP_908_112_2_18_2_12_47_120,"MMIDCD Http_ContentInterpreteProcess entered time = %d")
TRACE_MSG(MMIDCD_HTTP_942_112_2_18_2_12_47_121,"[MMIDCD]  Http_ContentInterpreteProcess; READ FILE  ERROR!")
TRACE_MSG(MMIDCD_HTTP_957_112_2_18_2_12_47_122,"MMIDCD  MMIAPIFMM_WriteFile error = %d")
TRACE_MSG(MMIDCD_HTTP_973_112_2_18_2_12_47_123,"MMIDCD   Http_ContentInterpreteProcess exit time = %d")
TRACE_MSG(MMIDCD_HTTP_987_112_2_18_2_12_47_124,"[MMIDCD]:Http_ErrorHandler; errorcode = %d")
TRACE_MSG(MMIDCD_HTTP_1064_112_2_18_2_12_48_125,"[MMIDCD]:Http_DownloadXML() enter")
TRACE_MSG(MMIDCD_HTTP_1070_112_2_18_2_12_48_126,"[MMIDCD]:Http_DownloadXML:Http_ReqListNum(s_dcd_http_req_list_needed)==0")
TRACE_MSG(MMIDCD_HTTP_1080_112_2_18_2_12_48_127,"Http_DownloadXML() exit")
TRACE_MSG(MMIDCD_HTTP_1180_112_2_18_2_12_48_128,"Http_DownloadXMLPre() enter")
TRACE_MSG(MMIDCD_HTTP_1198_112_2_18_2_12_48_129,"MMIDCD GetLocalFileHeaderInfo, local xml file name fail")
TRACE_MSG(MMIDCD_HTTP_1236_112_2_18_2_12_48_130,"[MMIDCD]:DCD REQ TYPE :%s")
TRACE_MSG(MMIDCD_HTTP_1237_112_2_18_2_12_48_131,"[MMIDCD]:DCD UA :%s")
TRACE_MSG(MMIDCD_HTTP_1238_112_2_18_2_12_48_132,"[MMIDCD]:DCD URL :%s")
TRACE_MSG(MMIDCD_HTTP_1280_112_2_18_2_12_48_133,"DCD_SetReqype is man")
TRACE_MSG(MMIDCD_HTTP_1285_112_2_18_2_12_48_134,"DCD_SetReqype is ttl")
TRACE_MSG(MMIDCD_HTTP_1290_112_2_18_2_12_48_135,"DCD_SetReqype is svr")
TRACE_MSG(MMIDCD_HTTP_1295_112_2_18_2_12_48_136,"DCD_SetReqype is mc")
TRACE_MSG(MMIDCD_HTTP_1300_112_2_18_2_12_48_137,"DCD_SetReqype is mr")
TRACE_MSG(MMIDCD_HTTP_1305_112_2_18_2_12_48_138,"DCD_SetReqype is start")
TRACE_MSG(MMIDCD_HTTP_1310_112_2_18_2_12_48_139,"DCD_SetReqype is retry")
TRACE_MSG(MMIDCD_HTTP_1315_112_2_18_2_12_48_140,"DCD_SetReqype is data")
TRACE_MSG(MMIDCD_HTTP_1320_112_2_18_2_12_48_141,"DCD_SetReqype is data2")
TRACE_MSG(MMIDCD_HTTP_1325_112_2_18_2_12_48_142,"DCD_SetReqype is data3")
TRACE_MSG(MMIDCD_HTTP_1328_112_2_18_2_12_48_143,"DCD_SetReqype is unknown")
TRACE_MSG(MMIDCD_HTTP_1339_112_2_18_2_12_48_144,"[MMIDCD]:MMIDCD_SetReqCommonPara enter ")
TRACE_MSG(MMIDCD_HTTP_1436_112_2_18_2_12_48_145,"[MMIDCD]:Http_ContentReqHeaderInit ptr PNULL ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_HTTP_1496_112_2_18_2_12_48_146,"[MMIDCD]:MIMEDecodeByBuf; buf_ptr or len is NULL!!!!!!")
TRACE_MSG(MMIDCD_HTTP_1506_112_2_18_2_12_48_147," [MMIDCD]:Content-Type: can not be found, the file is invalid")
TRACE_MSG(MMIDCD_HTTP_1514_112_2_18_2_12_48_148," [MMIDCD]:image: can not be found, the file is invalid")
TRACE_MSG(MMIDCD_HTTP_1532_112_2_18_2_12_48_149," [MMIDCD]:Content-Id: can not be found, the file is invalid")
TRACE_MSG(MMIDCD_HTTP_1553_112_2_18_2_12_49_150," [MMIDCD]:Content-Length: can not be found, the file is invalid")
TRACE_MSG(MMIDCD_HTTP_1572_112_2_18_2_12_49_151,"RULL 2:file len < *len_ptr, the file is invalid")
TRACE_MSG(MMIDCD_MAIN_233_112_2_18_2_12_49_152,"[MMIDCD]: MMIDCD_GetLatestUpdateState; dcd latest error_type is %d")
TRACE_MSG(MMIDCD_MAIN_306_112_2_18_2_12_49_153,"[MMIDCD] OUT OF RANGE OF DCD MSG")
TRACE_MSG(MMIDCD_MAIN_352_112_2_18_2_12_50_154,"[MMIDCD]: DCD phone is in fly mode now")
TRACE_MSG(MMIDCD_MAIN_358_112_2_18_2_12_50_155,"[MMIDCD]:  DCD sim card is not ok")
TRACE_MSG(MMIDCD_MAIN_364_112_2_18_2_12_50_156,"[MMIDCD]:  DCD u disk is running")
TRACE_MSG(MMIDCD_MAIN_374_112_2_18_2_12_50_157,"[MMIDCD]:  DCD mem size is not enough!!")
TRACE_MSG(MMIDCD_MAIN_382_112_2_18_2_12_50_158,"[MMIDCD]:  current network is not cmcc ")
TRACE_MSG(MMIDCD_MAIN_388_112_2_18_2_12_50_159,"[MMIDCD]:  network is not valid")
TRACE_MSG(MMIDCD_MAIN_396_112_2_18_2_12_50_160,"[MMIDCD]:  DCD service is not open")
TRACE_MSG(MMIDCD_MAIN_402_112_2_18_2_12_50_161,"[MMIDCD]:  DCD is busy now")
TRACE_MSG(MMIDCD_MAIN_407_112_2_18_2_12_50_162,"[MMIDCD]:  DCD status is ok now!")
TRACE_MSG(MMIDCD_MAIN_437_112_2_18_2_12_50_163,"[MMIDCD]:DCD_Idle_ProcessMsg enter")
TRACE_MSG(MMIDCD_MAIN_455_112_2_18_2_12_50_164,"[MMIDCD]: http init !!")
TRACE_MSG(MMIDCD_MAIN_462_112_2_18_2_12_50_165,"[MMIDCD]: DCD_SIG_DL_CANCLE_REQ")
TRACE_MSG(MMIDCD_MAIN_471_112_2_18_2_12_50_166,"[MMIDCD]: default enter")
TRACE_MSG(MMIDCD_MAIN_481_112_2_18_2_12_50_167,"[MMIDCD]:DCD_XML_ProcessMsg enter")
TRACE_MSG(MMIDCD_MAIN_488_112_2_18_2_12_50_168,"[MMIDCD]: http init cnf and xml download start!!")
TRACE_MSG(MMIDCD_MAIN_575_112_2_18_2_12_50_169,"[MMIDCD]:DCD_MIME_ProcessMsg enter")
TRACE_MSG(MMIDCD_MAIN_583_112_2_18_2_12_50_170,"MMIDCD Http_ContentDownloadProcess finish time = %d")
TRACE_MSG(MMIDCD_MAIN_624_112_2_18_2_12_50_171,"[MMIDCD]: HTTP_SIG_GET_CNF, parse fail")
TRACE_MSG(MMIDCD_MAIN_679_112_2_18_2_12_50_172,"[MMIDCD]:DCD_Null_ProcessMsg enter")
TRACE_MSG(MMIDCD_MAIN_703_112_2_18_2_12_50_173,"[MMIDCD]: msg %s")
TRACE_MSG(MMIDCD_MAIN_733_112_2_18_2_12_50_174,"[MMIDCD]: MMIDCD_SimErrorInd(),  sim_status= %d")
TRACE_MSG(MMIDCD_MAIN_759_112_2_18_2_12_50_175,"HandleDCDProcessMsg() enter msg_id = %d")
TRACE_MSG(MMIDCD_MAIN_769_112_2_18_2_12_50_176,"HandleDCDProcessMsg() DCD_SIG_XML_DL_REQ error_type = %d")
TRACE_MSG(MMIDCD_MAIN_798_112_2_18_2_12_50_177,"[MMIDCD]:dcd pdp active fail!!!")
TRACE_MSG(MMIDCD_MAIN_830_112_2_18_2_12_50_178,"[MMIDCD]: http init cnf and error!!")
TRACE_MSG(MMIDCD_MAIN_835_112_2_18_2_12_50_179,"[MMIDCD]: http init cnf and ok!!!")
TRACE_MSG(MMIDCD_MAIN_882_112_2_18_2_12_51_180,"[MMIDCD]: xml download is end!!")
TRACE_MSG(MMIDCD_MAIN_906_112_2_18_2_12_51_181,"[MMIDCD]: receive DCD_SIG_DEFALUT_RETRY_TIMER_IND!")
TRACE_MSG(MMIDCD_MAIN_985_112_2_18_2_12_51_182,"HandleDCDProcessMsg()  exit")
TRACE_MSG(MMIDCD_MAIN_1035_112_2_18_2_12_51_183,"[MMIDCD] feed Ctxt_Id repeat = %s")
TRACE_MSG(MMIDCD_MAIN_1074_112_2_18_2_12_51_184,"[MMIDCD] entry Ctxt_Id repeat = %s")
TRACE_MSG(MMIDCD_MAIN_1107_112_2_18_2_12_51_185,"[MMIDCD]:ParseDCDXMLFileWhenPowerOn enter!!")
TRACE_MSG(MMIDCD_MAIN_1118_112_2_18_2_12_51_186,"MMIDCD GetLocalFileHeaderInfo, local xml file name fail")
TRACE_MSG(MMIDCD_MAIN_1125_112_2_18_2_12_51_187,"[MMIDCD]:MMIDCD_Parse() case xml file parse return TRUE")
TRACE_MSG(MMIDCD_MAIN_1144_112_2_18_2_12_51_188,"[MMIDCD]:MMIAPIDCD_Parse() case old xml file DCDXML_ParseXMLFile return FALSE")
TRACE_MSG(MMIDCD_MAIN_1163_112_2_18_2_12_51_189,"[MMIDCD]:MMIDCD_Parse:global_meta_ttl= %d\n")
TRACE_MSG(MMIDCD_MAIN_1166_112_2_18_2_12_51_190,"[MMIDCD]:MMIDCD_Parse() exit!!")
TRACE_MSG(MMIDCD_MAIN_1190_112_2_18_2_12_51_191,"[MMIDCD]:MMIDCD_Parse() enter!!")
TRACE_MSG(MMIDCD_MAIN_1201_112_2_18_2_12_51_192,"MMIDCD GetLocalFileHeaderInfo, local xml file name fail")
TRACE_MSG(MMIDCD_MAIN_1207_112_2_18_2_12_51_193,"[MMIDCD]:MMIDCD_Parse() case default xml parse!")
TRACE_MSG(MMIDCD_MAIN_1225_112_2_18_2_12_51_194,"[MMIDCD]:MMIDCD_Parse() case xml file parse return TRUE")
TRACE_MSG(MMIDCD_MAIN_1254_112_2_18_2_12_51_195,"[MMIDCD] local xml file parse fail")
TRACE_MSG(MMIDCD_MAIN_1259_112_2_18_2_12_51_196,"[MMIDCD]MMIDCD GetLocalFileHeaderInfo, local xml file name fail")
TRACE_MSG(MMIDCD_MAIN_1275_112_2_18_2_12_52_197,"[MMIDCD]:MMIDCD_Parse() case xml file parse return FALSE")
TRACE_MSG(MMIDCD_MAIN_1290_112_2_18_2_12_52_198,"[MMIDCD]:MMIAPIDCD_Parse() case old xml file DCDXML_ParseXMLFile return TRUE")
TRACE_MSG(MMIDCD_MAIN_1296_112_2_18_2_12_52_199,"[MMIDCD]:MMIAPIDCD_Parse() case old xml file DCDXML_ParseXMLFile return FALSE")
TRACE_MSG(MMIDCD_MAIN_1303_112_2_18_2_12_52_200,"[MMIDCD]:has parse fail or pasrse success but has zero feed, so use default xml")
TRACE_MSG(MMIDCD_MAIN_1323_112_2_18_2_12_52_201,"[MMIDCD]:MMIDCD_Parse:global_meta_ttl= %d\n")
TRACE_MSG(MMIDCD_MAIN_1326_112_2_18_2_12_52_202,"[MMIDCD]:MMIDCD_Parse() exit!!")
TRACE_MSG(MMIDCD_MAIN_1347_112_2_18_2_12_52_203,"MMIDCD GetLocalFileHeaderInfo, local xml file name fail")
TRACE_MSG(MMIDCD_MAIN_1352_112_2_18_2_12_52_204,"[MMIDCD]: the index.xml file delete fail!")
TRACE_MSG(MMIDCD_MAIN_1360_112_2_18_2_12_52_205,"MMIDCD GetLocalFileHeaderInfo, local xml file name fail")
TRACE_MSG(MMIDCD_MAIN_1366_112_2_18_2_12_52_206,"[MMIDCD]: the local.xml file delete fail!")
TRACE_MSG(MMIDCD_MAIN_1374_112_2_18_2_12_52_207,"MMIDCD GetLocalFileHeaderInfo, local xml file name fail")
TRACE_MSG(MMIDCD_MAIN_1379_112_2_18_2_12_52_208,"[MMIDCD]: the old.xml file delete fail!")
TRACE_MSG(MMIDCD_MAIN_1403_112_2_18_2_12_52_209,"[MMIDCD]:DCD_ActivatePDPcontext enter!")
TRACE_MSG(MMIDCD_MAIN_1422_112_2_18_2_12_52_210,"Read fail!! return_val = %d")
TRACE_MSG(MMIDCD_MAIN_1438_112_2_18_2_12_52_211,"[MMIDCD]:DCD_DeactivatePDPcontext enter!")
TRACE_MSG(MMIDCD_MAIN_1465_112_2_18_2_12_52_212,"[MMIDCD]:DcdHandlePDPMsg enter!")
TRACE_MSG(MMIDCD_MAIN_1475_112_2_18_2_12_52_213,"[MMIDCD]:dcd  pdp active success!!!!")
TRACE_MSG(MMIDCD_MAIN_1480_112_2_18_2_12_52_214,"[MMIDCD]:dcd pdp active fail!!!")
TRACE_MSG(MMIDCD_MAIN_1486_112_2_18_2_12_52_215,"[MMIDCD]:dcd  is busy , but accapt MMIPDP_ACTIVE_CNF message!!status = %d ")
TRACE_MSG(MMIDCD_MAIN_1492_112_2_18_2_12_52_216,"[MMIDCD]:dcd  pdp deactive is come!!!!")
TRACE_MSG(MMIDCD_MAIN_1499_112_2_18_2_12_52_217,"[MMIDCD]:dcd  MMIPDP_DEACTIVE_IND msg is come!")
TRACE_MSG(MMIDCD_MAIN_1507_112_2_18_2_12_52_218,"[MMIDCD]:DcdHandlePDPMsg exit!")
TRACE_MSG(MMIDCD_MAIN_1542_112_2_18_2_12_52_219,"[MMIDCD]: MMIDCD_NetWorkIsValid sim is not ok")
TRACE_MSG(MMIDCD_MAIN_1548_112_2_18_2_12_52_220,"[MMIDCD]: MMIDCD_NetWorkIsValid net type is not ok")
TRACE_MSG(MMIDCD_MAIN_1554_112_2_18_2_12_52_221,"[MMIDCD]:  MMIDCD_NetWorkIsValid net is not valid")
TRACE_MSG(MMIDCD_MAIN_1598_112_2_18_2_12_52_222,"[MMIDCD]:Current net mnc is: %d ")
TRACE_MSG(MMIDCD_MAIN_1666_112_2_18_2_12_52_223,"[MMIDCD]:PNULL != sync_pkg_ptr!!!!!!")
TRACE_MSG(MMIDCD_MAIN_1671_112_2_18_2_12_52_224,"[MMIDCD]:MMIDCD_IsSynchronize: at present dcd ttl=%d\n")
TRACE_MSG(MMIDCD_MAIN_1675_112_2_18_2_12_52_225,"[MMIDCD]:MMIDCD_IsSynchronize:dcd disable dcd ttl update!!\n")
TRACE_MSG(MMIDCD_MAIN_1682_112_2_18_2_12_52_226,"[MMIDCD]:MMIDCD_IsSynchronize:ttl update is coming, resume next ttl: %d\n")
TRACE_MSG(MMIDCD_MAIN_1714_112_2_18_2_12_52_227,"[MMIDCD]:DCD error retry update is the %d times, next timer is %d min !!")
TRACE_MSG(MMIDCD_MAIN_1742_112_2_18_2_12_52_228,"[MMIDCD]: phone is in roaming status: %d")
TRACE_MSG(MMIDCD_MAIN_1772_112_2_18_2_12_53_229,"[MMIDCD]MMIDCD_NotifyEventCallback dual_sys=%d, notify_event=%d")
TRACE_MSG(MMIDCD_MAIN_1782_112_2_18_2_12_53_230,"[MMIDCD]:MMIDCD_NotifyEventCallback,  start retry update now! ")
TRACE_MSG(MMIDCD_MAIN_1813_112_2_18_2_12_53_231,"[MMIDCD]:DCD Retry start!")
TRACE_MSG(MMIDCD_MAIN_1823_112_2_18_2_12_53_232,"[MMIDCD]:DCD TTL start!")
TRACE_MSG(MMIDCD_MAIN_1835_112_2_18_2_12_53_233,"[MMIDCD] start timer, req_type = %d, second = %d")
TRACE_MSG(MMIDCD_MAIN_1839_112_2_18_2_12_53_234,"[MMIDCD] start ttl timer begin, timer_id = %d")
TRACE_MSG(MMIDCD_MAIN_1855_112_2_18_2_12_53_235,"[MMIDCD] start ttl timer end, timer_id = %d")
TRACE_MSG(MMIDCD_MAIN_1859_112_2_18_2_12_53_236,"[MMIDCD] start retry timer begin, timer_id = %d")
TRACE_MSG(MMIDCD_MAIN_1877_112_2_18_2_12_53_237,"[MMIDCD]:DCD error retry update is the %d times, next timer is %d second !!")
TRACE_MSG(MMIDCD_MAIN_1881_112_2_18_2_12_53_238,"[MMIDCD] start retry timer end, timer_id = %d")
TRACE_MSG(MMIDCD_MAIN_1894_112_2_18_2_12_53_239,"[MMIDCD] MMIDCD_SetDefaultRetryTime enter!")
TRACE_MSG(MMIDCD_MAIN_1897_112_2_18_2_12_53_240,"[MMIDCD] MMIDCD_SetDefaultRetryTime, delete s_retry_timer")
TRACE_MSG(MMIDCD_SETTING_67_112_2_18_2_12_53_241,"[MMIDCD] MMIAPIDCD_WriteSettingToNv is_dcd_on = %d")
TRACE_MSG(MMIDCD_SETTING_76_112_2_18_2_12_54_242,"[MMIDCD]:: MMIDCD_SetSettingDefault enter! ")
TRACE_MSG(MMIDCD_SETTING_110_112_2_18_2_12_54_243,"[MMIDCD]:MMIDCD_InitSetting")
TRACE_MSG(MMIDCD_SETTING_141_112_2_18_2_12_54_244,"[MMIDCD]:MMIDCD_Setting_GetNetSettingIndex,dual_sys=%d,index=%d")
TRACE_MSG(MMIDCD_SETTING_156_112_2_18_2_12_54_245,"[MMIDCD]:MMIDCD_Setting_SetNetSettingIndex dual_sys=%d index=%d")
TRACE_MSG(MMIDCD_SETTING_184_112_2_18_2_12_54_246,"MMIDCD_Setting_ConnectionCallback,index=%d, dual_sys=%d")
TRACE_MSG(MMIDCD_WINTAB_893_112_2_18_2_12_56_247,"MMIDCD Read fail!!!")
TRACE_MSG(MMIDCD_WINTAB_902_112_2_18_2_12_56_248,"[MMIDCD]:MMIAPIDCD_SetData() enter")
TRACE_MSG(MMIDCD_WINTAB_922_112_2_18_2_12_56_249,"[MMIDCD]:MMIAPIDCD_SetData() exit")
TRACE_MSG(MMIDCD_WINTAB_986_112_2_18_2_12_56_250,"MMIDCD Get index fail!!!")
TRACE_MSG(MMIDCD_WINTAB_1088_112_2_18_2_12_56_251,"[MMIDCD]: MMIDCD_UpdateStart enter, dcd update start: %d")
TRACE_MSG(MMIDCD_WINTAB_1135_112_2_18_2_12_56_252,"[MMIDCD]:DCD_AppendListItem invalid ctrl_id = %d")
TRACE_MSG(MMIDCD_WINTAB_1215_112_2_18_2_12_56_253,"[MMIDCD] pageindex = %d, reaindex = %d")
TRACE_MSG(MMIDCD_WINTAB_1297_112_2_18_2_12_57_254,"MMIDCD read item info fail, local index = %d")
TRACE_MSG(MMIDCD_WINTAB_1464_112_2_18_2_12_57_255,"MMIDCD delete fail")
TRACE_MSG(MMIDCD_WINTAB_1589_112_2_18_2_12_57_256,"MMIDCD please check list ctrl!!")
TRACE_MSG(MMIDCD_WINTAB_1624_112_2_18_2_12_57_257,"[MMIDCD]:CreateDcdMarkMenu Get list data error!!!!!!!!!!!")
TRACE_MSG(MMIDCD_WINTAB_1929_112_2_18_2_12_58_258,"MMIDCD anim has start")
TRACE_MSG(MMIDCD_WINTAB_2254_112_2_18_2_12_59_259,"[MMIDCD]:MMIDCD_Error_Handler() enter MMIDCD_Error_Handler = %d")
TRACE_MSG(MMIDCD_WINTAB_2320_112_2_18_2_12_59_260,"[MMIDCD]:MMIDCD_IdleWinWakeupInd() enter ")
TRACE_MSG(MMIDCD_WINTAB_2323_112_2_18_2_12_59_261,"[MMIDCD]:dcd will be wake up !! ")
TRACE_MSG(MMIDCD_WINTAB_2339_112_2_18_2_12_59_262,"[MMIDCD]:MMIDCD_Wakeup() enter: ")
TRACE_MSG(MMIDCD_WINTAB_2364_112_2_18_2_12_59_263,"[MMIDCD]:MMIDCD_IndicationShow() enter: ")
TRACE_MSG(MMIDCD_WINTAB_2402_112_2_18_2_12_59_264,"[MMIDCD]:start retry update mechanism!!! ")
TRACE_MSG(MMIDCD_WINTAB_2433_112_2_18_2_12_59_265,"[MMIDCD]:MMIDCD_IndicationShow Please check here error!!!!!!!!!")
TRACE_MSG(MMIDCD_WINTAB_2484_112_2_18_2_12_59_266,"[MMIDCD]:s_dcd_gui_struct.total_page_num out of max range  error!!!!!!!! ")
TRACE_MSG(MMIDCD_WINTAB_2520_112_2_18_2_12_59_267,"DCD_AddTabsAndChildWindows: tab id = %d, page_num = %d!")
TRACE_MSG(MMIDCD_WINTAB_2535_112_2_18_2_12_59_268,"DCD_AddTabsAndChildWindows: idle tab!")
TRACE_MSG(MMIDCD_WINTAB_2542_112_2_18_2_12_59_269,"DCD_AddTabsAndChildWindows: not idle tab!")
TRACE_MSG(MMIDCD_WINTAB_2672_112_2_18_2_13_0_270,"MMIDCD_SetAnimateCtrlState state = %d")
TRACE_MSG(MMIDCD_WINTAB_2697_112_2_18_2_13_0_271,"MMIDCD SetDCDAnimateCtrlState has entered")
TRACE_MSG(MMIDCD_WINTAB_2702_112_2_18_2_13_0_272,"MMIDCD pauseanim has done")
TRACE_MSG(MMIDCD_WINTAB_2732_112_2_18_2_13_0_273,"[MMIDCD]: the invalid pic file delete ok!")
TRACE_MSG(MMIDCD_WINTAB_2736_112_2_18_2_13_0_274,"[MMIDCD]: the invalid pic file do not exist!")
TRACE_MSG(MMIDCD_WINTAB_2763_112_2_18_2_13_0_275,"[MMIDCD]: the invalid pic file delete ok!")
TRACE_MSG(MMIDCD_WINTAB_2767_112_2_18_2_13_0_276,"[MMIDCD]: the invalid pic file do not exist!")
TRACE_MSG(MMIDCD_WINTAB_2943_112_2_18_2_13_0_277,"[MMIDCD]:MMIDCD_UpdateFrame() enter")
TRACE_MSG(MMIDCD_WINTAB_2965_112_2_18_2_13_0_278,"[MMIDCD]:MMIDCD_UpdateFrame() exit")
TRACE_MSG(MMIDCD_WINTAB_3003_112_2_18_2_13_0_279,"MMICD will show file")
TRACE_MSG(MMIDCD_WINTAB_3179_112_2_18_2_13_1_280,"MMICD will show file")
TRACE_MSG(MMIDCD_WINTAB_3184_112_2_18_2_13_1_281,"MMIDCD find no picture ")
TRACE_MSG(MMIDCD_WINTAB_3355_112_2_18_2_13_1_282,"MMICD will show file")
TRACE_MSG(MMIDCD_WINTAB_3360_112_2_18_2_13_1_283,"MMIDCD find no picture ")
TRACE_MSG(MMIDCD_WINTAB_4134_112_2_18_2_13_2_284,"[MMIDCD] MMIDCDAPISET_FuncFinishedInd; MMIDCD_STOP_UPDATING_WAITING_WIN_ID is open!")
TRACE_MSG(MMIDCD_WINTAB_4139_112_2_18_2_13_2_285,"[MMIDCD] MMIDCDAPISET_FuncFinishedInd; MMIDCD_RESET_FACTORY_WAITING_WIN_ID is open!")
TRACE_MSG(MMIDCD_WINTAB_4160_112_2_18_2_13_3_286,"[MMIDCD]: MSG_DCD_UPDATE_FINISH is come!")
TRACE_MSG(MMIDCD_WINTAB_4674_112_2_18_2_13_4_287,"MMIDCD GetDcdItemLinkAdr read  fail")
TRACE_MSG(MMIDCD_WINTAB_4696_112_2_18_2_13_4_288,"MMIDCD find no link_ptr")
TRACE_MSG(MMIDCD_WINTAB_4821_112_2_18_2_13_4_289,"[MMIDCD] curitem,  item_num= %d")
TRACE_MSG(MMIDCD_WINTAB_4942_112_2_18_2_13_4_290,"[MMIDCD] curitem,  item_num= %d")
TRACE_MSG(MMIDCD_WINTAB_4948_112_2_18_2_13_4_291,"MMIDCD HandDCDPrewItemSwitch DATA has changed")
TRACE_MSG(MMIDCD_WINTAB_6075_112_2_18_2_13_6_292,"ResetDCDApplet_HandleEvent msg_id = 0x%04x")
TRACE_MSG(MMIDCD_WINTAB_6081_112_2_18_2_13_6_293,"ResetDCDApplet_HandleEventMSG_APPLET_START")
TRACE_MSG(MMIDCD_WINTAB_6098_112_2_18_2_13_6_294,"[MMIDCD]: MSG_SET_RESET_FACTORY_OVER_IND received!")
TRACE_MSG(MMIDCD_WINTAB_6136_112_2_18_2_13_6_295,"[MMIDCD]:MMIDCD_ShowUpdateCompelete req_type = %d, error_code = %d")
TRACE_MSG(MMIDCD_WINTAB_6140_112_2_18_2_13_6_296,"[MMIDCD]: this not need process ")
TRACE_MSG(MMIDCD_WINTAB_6166_112_2_18_2_13_6_297,"MMIDCD s_data_has_changed = %d")
TRACE_MSG(MMIDCD_WINTAB_6174_112_2_18_2_13_6_298,"MMIDCD GetDataChangeFlag s_data_has_changed = %d")
TRACE_MSG(MMIDCD_XML_PARSE_785_112_2_18_2_13_8_299,"[MMIDCD]:UTF82UCS2B ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_XML_PARSE_919_112_2_18_2_13_9_300,"GetATTRIBUTE_AttributeID() str='%s'	")
TRACE_MSG(MMIDCD_XML_PARSE_953_112_2_18_2_13_9_301,"DCD XML INVALID DCDXML_EL_ID_TITLE")
TRACE_MSG(MMIDCD_XML_PARSE_972_112_2_18_2_13_9_302,"DCD XML INVALID DCDXML_EL_ID_SUMMARY")
TRACE_MSG(MMIDCD_XML_PARSE_987_112_2_18_2_13_9_303,"DCD XML INVALID DCDXML_EL_ID_ISSUED")
TRACE_MSG(MMIDCD_XML_PARSE_1002_112_2_18_2_13_9_304,"DCD XML INVALID DCDXML_EL_ID_ACTIVATION")
TRACE_MSG(MMIDCD_XML_PARSE_1017_112_2_18_2_13_9_305,"DCD XML INVALID DCDXML_EL_ID_EXPIRATION")
TRACE_MSG(MMIDCD_XML_PARSE_1032_112_2_18_2_13_9_306,"DCD XML INVALID DCDXML_EL_ID_AUTHOR")
TRACE_MSG(MMIDCD_XML_PARSE_1048_112_2_18_2_13_9_307,"DCD XML INVALID DCDXML_EL_ID_NAME")
TRACE_MSG(MMIDCD_XML_PARSE_1063_112_2_18_2_13_9_308,"DCD XML INVALID DCDXML_EL_ID_URL")
TRACE_MSG(MMIDCD_XML_PARSE_1078_112_2_18_2_13_9_309,"DCD XML INVALID DCDXML_EL_ID_EMAIL")
TRACE_MSG(MMIDCD_XML_PARSE_1084_112_2_18_2_13_9_310,"DCD XML INVALID chardata = %d")
TRACE_MSG(MMIDCD_XML_PARSE_1142_112_2_18_2_13_9_311,"DCD XML INVALID DCDXML_EL_ID_INVALID_HTML")
TRACE_MSG(MMIDCD_XML_PARSE_1191_112_2_18_2_13_9_312,"DCD XML INVALID  MAX ID MetaParser ")
TRACE_MSG(MMIDCD_XML_PARSE_1197_112_2_18_2_13_9_313,"DCD XML INVALID  MetaParser")
TRACE_MSG(MMIDCD_XML_PARSE_1242_112_2_18_2_13_9_314,"DCD XML INVALID  MAX ID FeedParser ")
TRACE_MSG(MMIDCD_XML_PARSE_1248_112_2_18_2_13_9_315,"DCD XML INVALID  FeedParser")
TRACE_MSG(MMIDCD_XML_PARSE_1304_112_2_18_2_13_10_316,"DCD XML INVALID  MAX ID EntryParser ")
TRACE_MSG(MMIDCD_XML_PARSE_1310_112_2_18_2_13_10_317,"DCD XML INVALID  EntryParser")
TRACE_MSG(MMIDCD_XML_PARSE_1356_112_2_18_2_13_10_318,"DCD XML INVALID  MAX ID MetaParser ")
TRACE_MSG(MMIDCD_XML_PARSE_1404_112_2_18_2_13_10_319,"DCD XML INVALID  parser_status = %d")
TRACE_MSG(MMIDCD_XML_PARSE_1478_112_2_18_2_13_10_320,"MMIDCD_XmlParse() enter")
TRACE_MSG(MMIDCD_XML_PARSE_1492_112_2_18_2_13_10_321,"MMIDCD_XmlParse(): parsed_stream fail")
TRACE_MSG(MMIDCD_XML_PARSE_1501_112_2_18_2_13_10_322,"MMIDCD_XmlParse(): parsed_stream fail")
TRACE_MSG(MMIDCD_XML_PARSE_1510_112_2_18_2_13_10_323,"MMIDCD_XmlParse(): create xml parser fail")
TRACE_MSG(MMIDCD_XML_PARSE_1570_112_2_18_2_13_10_324,"DCDXML_ParseStaticFile() enter")
TRACE_MSG(MMIDCD_XML_PARSE_1584_112_2_18_2_13_10_325,"create xml parser fail")
TRACE_MSG(MMIDCD_XML_PARSE_1638_112_2_18_2_13_10_326,"DCDXML_ParseStaticFile():Parse error at line %dn %sn")
TRACE_MSG(MMIDCD_XML_PARSE_1660_112_2_18_2_13_10_327,"GetLinkRel() str='%s'	")
TRACE_MSG(MMIDCD_XML_PARSE_1676_112_2_18_2_13_10_328,"GetLinkType() str='%s'	")
TRACE_MSG(MMIDCD_XML_PARSE_1690_112_2_18_2_13_10_329,"GetContentMode() str='%s'	")
TRACE_MSG(MMIDCD_XML_PARSE_1705_112_2_18_2_13_10_330,"GetContentType() str='%s'	")
TRACE_MSG(MMIDCD_XML_PARSE_1719_112_2_18_2_13_10_331,"GetContentName() str='%s'	")
TRACE_MSG(MMIDCD_XML_PARSE_1733_112_2_18_2_13_10_332,"GetTemplateId() str='%s'	")
TRACE_MSG(MMIDCD_XML_PARSE_1752_112_2_18_2_13_10_333,"GetEntryType() str='%s'	")
TRACE_MSG(MMIDCD_XML_PARSE_1769_112_2_18_2_13_10_334,"GetFeedType() str='%s'	")
TRACE_MSG(MMIDCD_XML_PARSE_1785_112_2_18_2_13_11_335,"GetFeedType() str='%s'	")
TRACE_MSG(MMIDCD_XML_PARSE_1800_112_2_18_2_13_11_336,"GetTimeType() str='%s'	")
TRACE_MSG(MMIDCD_XML_PARSE_2775_112_2_18_2_13_13_337,"DCD_FeedMetaListAdd() enter")
TRACE_MSG(MMIDCD_XML_PARSE_2832_112_2_18_2_13_13_338,"DCD_ContentListAdd() enter")
TRACE_MSG(MMIDCD_XML_PARSE_2888_112_2_18_2_13_13_339,"DCD_EntryListAdd() enter")
TRACE_MSG(MMIDCD_XML_PARSE_2945_112_2_18_2_13_13_340,"[MMIDCD]:FeedAdd2List ptr PNULL ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_XML_PARSE_2949_112_2_18_2_13_13_341,"DCD_FeedListAdd() enter")
TRACE_MSG(MMIDCD_XML_PARSE_3007_112_2_18_2_13_13_342,"[MMIDCD]:ContentIsExistInEntry ptr PNULL ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_XML_PARSE_3085_112_2_18_2_13_13_343,"MMIDCD nend not download picutre")
TRACE_MSG(MMIDCD_XML_PARSE_3858_112_2_18_2_13_15_344,"[MMIDCD]:MMIDCD_GetFeedCTXTId ptr PNULL ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIDCD_XML_PARSE_3874_112_2_18_2_13_15_345,"MMI DCD this index has no chanel")
TRACE_MSG(MMIDCD_XML_PARSE_4063_112_2_18_2_13_15_346,"MMIDCD content_ptr no cmp")
TRACE_MSG(MMIDCD_XML_PARSE_4069_112_2_18_2_13_15_347,"MMIDCD content_ptr is null now")
TRACE_MSG(MMIDCD_XML_PARSE_4074_112_2_18_2_13_15_348,"MMIDCD feed_ptr is null now")
TRACE_MSG(MMIDCD_XML_PARSE_4114_112_2_18_2_13_15_349,"[MMIDCD]:MMIAPIDCD_GetEntryContentInfo type dont matching ERROR!!!!!!!!!!")
TRACE_MSG(MMIDCD_XML_PARSE_4150_112_2_18_2_13_15_350,"[MMIDCD]:MMIDCD_GetEntryLink xml type dont matching ERROR!!!!!!!!!!")
END_TRACE_MAP(MMI_APP_DCD_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_DCD_TRC_H_


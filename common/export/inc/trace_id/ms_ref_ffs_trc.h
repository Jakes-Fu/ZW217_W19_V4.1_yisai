/******************************************************************************
 ** File Name:      ms_ref_ffs_trc.h                                         *
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
//trace_id:47
#ifndef _MS_REF_FFS_TRC_H_
#define _MS_REF_FFS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define FFDEV_79_112_2_18_1_30_27_0 "FFS name is:"
#define FFDEV_88_112_2_18_1_30_27_1 "%s"
#define FFDEV_94_112_2_18_1_30_27_2 "%s"
#define FFDEV_302_112_2_18_1_30_27_3 "FFS FAT_Create DEV NO %x,bMode = %x"
#define FFDEV_345_112_2_18_1_30_27_4 "FFS FAT_Create:result = %x"
#define FFDEV_400_112_2_18_1_30_27_5 "FFS FAT_Close Fhandle = %x,dev NO %x"
#define FFDEV_711_112_2_18_1_30_28_6 "FFS FAT_chsize DEV NO %x"
#define FFDEV_712_112_2_18_1_30_28_7 "FFS FAT_chsize:fhande %x,size %x"
#define FFDEV_737_112_2_18_1_30_28_8 "FFS FAT_GetLength Fhandle:%x,DEV NO %x"
#define FFDEV_741_112_2_18_1_30_28_9 "FFS FAT_GetLength result = 0"
#define FFDEV_745_112_2_18_1_30_28_10 "FFS FAT_GetLength result = %x"
#define FFDEV_827_112_2_18_1_30_28_11 "FFS FAT_Read: Fhandle %x,buf %x ,len %x,overlapped:%x"
#define FFDEV_984_112_2_18_1_30_29_12 "FFS FAT_Write: Fhandle %x,buf %x ,len %x,overlapped:%x"
#define FFDEV_1101_112_2_18_1_30_29_13 "FFS FAT_Delete: DEV NO %x,overlapped %x"
#define FFDEV_1191_112_2_18_1_30_29_14 "FFS FAT_Rename Device No %d"
#define FFDEV_1347_112_2_18_1_30_29_15 "FFS FAT_FindNext: fhandle %x"
#define FFDEV_1402_112_2_18_1_30_29_16 "FFS FAT_CreateDir DEV NO %x"
#define FFDEV_1463_112_2_18_1_30_30_17 "FFS FAT_DeleteDir DEV NO %x"
#define FFDEV_1521_112_2_18_1_30_30_18 "FFS FAT_OpenDir DEV NO %x"
#define FFDEV_1572_112_2_18_1_30_30_19 "FFS FAT_Seek:Fhandle = %x,dev NO %x,offset %x ,origin %x"
#define FFDEV_1745_112_2_18_1_30_30_20 "FFS FFS_init dev No %x"
#define FFDEV_1752_112_2_18_1_30_30_21 "FFS_init,ud is null"
#define FFDEV_1761_112_2_18_1_30_30_22 "FFS_init,sd is null"
#define FFDEV_1776_112_2_18_1_30_30_23 "FFS_Init: file_dev_type(%d) error"
#define FFDEV_1878_112_2_18_1_30_30_24 "FFS FFS_Shutdown DEV NO %x,overlapped,%x"
#define FFDEV_1955_112_2_18_1_30_31_25 "FFS FFS_Close"
#define FFDEV_1980_112_2_18_1_30_31_26 "FFS FFS_GetLength"
#define FFDEV_2018_112_2_18_1_30_31_27 "FFS_Create"
#define FFDEV_2032_112_2_18_1_30_31_28 "FFS_Create,ud is null"
#define FFDEV_2042_112_2_18_1_30_31_29 "FFS_Create,sd is null"
#define FFDEV_2068_112_2_18_1_30_31_30 "FFS FFS_Seek"
#define FFDEV_2545_112_2_18_1_30_32_31 "FFS,freespace,ud is null"
#define FFDEV_2554_112_2_18_1_30_32_32 "FFS,freespace,sd is null"
#define FFDEV_2584_112_2_18_1_30_32_33 "FFS,usedspace,ud is null"
#define FFDEV_2593_112_2_18_1_30_32_34 "FFS,usedspace,sd is null"
#define FFDEV_2735_112_2_18_1_30_32_35 "FFS_fopen mode = %x"
#define FFDEV_2749_112_2_18_1_30_32_36 "FFS,fopen,ud is null"
#define FFDEV_2758_112_2_18_1_30_32_37 "FFS,fopen,sd is null"
#define FFDEV_3166_112_2_18_1_30_33_38 "FFS,format,ud is null"
#define FFDEV_3174_112_2_18_1_30_33_39 "FFS,format,sd is null"
#define FFS_CREATE_148_112_2_18_1_30_34_40 "FFS add_entry,root"
#define FFS_CREATE_151_112_2_18_1_30_34_41 "FFS add_entry,root,short"
#define FFS_CREATE_167_112_2_18_1_30_34_42 "FFS add_entry,root,short,get entryID fail"
#define FFS_CREATE_172_112_2_18_1_30_34_43 "FFS add_entry,root,long"
#define FFS_CREATE_193_112_2_18_1_30_34_44 "FFS add_entry,long,beg for"
#define FFS_CREATE_211_112_2_18_1_30_35_45 "FFS add_entry,long,write entries end "
#define FFS_CREATE_219_112_2_18_1_30_35_46 "FFS add_entry,in cluster"
#define FFS_CREATE_224_112_2_18_1_30_35_47 "FFS add_entry,in cluster,short"
#define FFS_CREATE_229_112_2_18_1_30_35_48 "FFS add_entry,in cluster,short,have entry"
#define FFS_CREATE_251_112_2_18_1_30_35_49 "FFS add_entry,in cluster,short,have no entry"
#define FFS_CREATE_259_112_2_18_1_30_35_50 "FFS add_entry,in cluster,long"
#define FFS_CREATE_269_112_2_18_1_30_35_51 "FFS add_entry,in cluster,long,have entry"
#define FFS_CREATE_321_112_2_18_1_30_35_52 "FFS add_entry,in cluster,long,in else"
#define FFS_CREATE_339_112_2_18_1_30_35_53 "FFS add_entry,in cluster,long,new clu"
#define FFS_CREATE_372_112_2_18_1_30_35_54 "FFS add_entry,in cluster,long,retn true"
#define FFS_CREATE_379_112_2_18_1_30_35_55 "FFS add_entry,in cluster,long,last,no entry"
#define FFS_CREATE_458_112_2_18_1_30_35_56 "FFS del_entry,root"
#define FFS_CREATE_465_112_2_18_1_30_35_57 "FFS del_entry,root,long"
#define FFS_CREATE_476_112_2_18_1_30_35_58 "FFS del_entry,in clu"
#define FFS_CREATE_483_112_2_18_1_30_35_59 "FFS del_entry,in clu,long"
#define FFS_CREATE_486_112_2_18_1_30_35_60 "FFS del_entry,in clu,long,lclu!=clu"
#define FFS_CREATE_500_112_2_18_1_30_35_61 "FFS del_entry,in clu,get nxt clu"
#define FFS_CREATE_506_112_2_18_1_30_35_62 "FFS del_entry,in clu,nxt clu"
#define FFS_CREATE_531_112_2_18_1_30_35_63 "FFS del_entry,in clu,long,for,end"
#define FFS_CREATE_534_112_2_18_1_30_35_64 "FFS del_entry,in clu,retn right"
#define FFS_CREATE_538_112_2_18_1_30_35_65 "FFS del_entry,in clu,lclu=clu"
#define FFS_CREATE_545_112_2_18_1_30_35_66 "FFS del_entry,in clu,back true"
#define FFS_CREATE_549_112_2_18_1_30_35_67 "FFS del_entry,in clu,short"
#define FFS_CREATE_620_112_2_18_1_30_35_68 "FFS,Create,it's a absolute path"
#define FFS_CREATE_626_112_2_18_1_30_35_69 "FFS,Create,only a /"
#define FFS_CREATE_668_112_2_18_1_30_35_70 "FFS create sech1 in clu retn 0x3"
#define FFS_CREATE_675_112_2_18_1_30_35_71 "FFS create have name1"
#define FFS_CREATE_679_112_2_18_1_30_35_72 "FFS create have name,dir"
#define FFS_CREATE_683_112_2_18_1_30_35_73 "FFS create have name not dir"
#define FFS_CREATE_696_112_2_18_1_30_35_74 "FFS create have name,file"
#define FFS_CREATE_699_112_2_18_1_30_35_75 "FFS create have name,but dir"
#define FFS_CREATE_712_112_2_18_1_30_36_76 "FFS create ,check access"
#define FFS_CREATE_721_112_2_18_1_30_36_77 "FFS create ,check open"
#define FFS_CREATE_729_112_2_18_1_30_36_78 "FFS create ,not opened"
#define FFS_CREATE_772_112_2_18_1_30_36_79 "FFS create ,overwrite"
#define FFS_CREATE_786_112_2_18_1_30_36_80 "FFS create ,clean1 end"
#define FFS_CREATE_823_112_2_18_1_30_36_81 "FFS create ,overwrite,new fcb"
#define FFS_CREATE_848_112_2_18_1_30_36_82 "FFS create ,not have name"
#define FFS_CREATE_859_112_2_18_1_30_36_83 "FFS create ,not have name,dir"
#define FFS_CREATE_891_112_2_18_1_30_36_84 "FFS create ,not have name,add entry"
#define FFS_CREATE_895_112_2_18_1_30_36_85 "FFS create ,add entry fail"
#define FFS_CREATE_898_112_2_18_1_30_36_86 "FFS create ,add entry suc"
#define FFS_CREATE_915_112_2_18_1_30_36_87 "FFS create ,add entry suc,write back suc"
#define FFS_CREATE_920_112_2_18_1_30_36_88 "FFS create ,write dot in dir beg"
#define FFS_CREATE_966_112_2_18_1_30_36_89 "FFS create ,write dot in dir suc"
#define FFS_CREATE_971_112_2_18_1_30_36_90 "FFS create ,write dot in dir suc,dir"
#define FFS_CREATE_986_112_2_18_1_30_36_91 "FFS create ,write dot in dir suc,file"
#define FFS_CREATE_998_112_2_18_1_30_36_92 "FFS create ,write dot in dir suc,sech in dir suc end"
#define FFS_CREATE_1016_112_2_18_1_30_36_93 "FFS create ,into while"
#define FFS_CREATE_1033_112_2_18_1_30_36_94 "FFS create ,still have /"
#define FFS_CREATE_1048_112_2_18_1_30_36_95 "FFS create ,have no /"
#define FFS_CREATE_1071_112_2_18_1_30_36_96 "FFS create ,add_entry2"
#define FFS_CREATE_1077_112_2_18_1_30_36_97 "FFS create ,add_entry end"
#define FFS_CREATE_1103_112_2_18_1_30_36_98 "FFS create ,add_entry2 & write back end"
#define FFS_CREATE_1108_112_2_18_1_30_36_99 "FFS create ,add_entry2,dir"
#define FFS_CREATE_1138_112_2_18_1_30_36_100 "FFS create ,add_entry2,end"
#define FFS_CREATE_1142_112_2_18_1_30_36_101 "FFS create ,add_entry2,file"
#define FFS_CREATE_1162_112_2_18_1_30_37_102 "FFS create ,in while end ,last"
#define FFS_DELETE_139_112_2_18_1_30_37_103 "FFS dodel,have name"
#define FFS_DELETE_142_112_2_18_1_30_37_104 "FFS dodel,have name,dir"
#define FFS_DELETE_154_112_2_18_1_30_37_105 "FFS dodel,have dir,continue"
#define FFS_DELETE_160_112_2_18_1_30_37_106 "FFS dodel,have,file"
#define FFS_DELETE_163_112_2_18_1_30_37_107 "FFS dodel,have,file,last"
#define FFS_DELETE_166_112_2_18_1_30_37_108 "FFS dodel,del beg"
#define FFS_DELETE_168_112_2_18_1_30_37_109 "FFS dodel,del end"
#define FFS_DELETE_177_112_2_18_1_30_37_110 "FFS dodel,clean end"
#define FFS_DELETE_185_112_2_18_1_30_37_111 "FFS dodel,not last but file"
#define FFS_DELETE_193_112_2_18_1_30_37_112 "FFS dodel,have no name"
#define FFS_DIR_265_112_2_18_1_30_38_113 "FFS,DoCreateDir,only a /"
#define FFS_DIR_271_112_2_18_1_30_38_114 "FFS,DoCreateDir,not root"
#define FFS_DIR_282_112_2_18_1_30_38_115 "FFS,DoCreateDir,root"
#define FFS_DIR_303_112_2_18_1_30_38_116 "FFS,DoCreateDir,have name"
#define FFS_DIR_306_112_2_18_1_30_38_117 "FFS,DoCreateDir,have name,last"
#define FFS_DIR_315_112_2_18_1_30_38_118 "FFS,DoCreateDir,have,dir"
#define FFS_DIR_317_112_2_18_1_30_38_119 "FFS,DoCreateDir,clusterID=%d"
#define FFS_DIR_327_112_2_18_1_30_38_120 "FFS,DoCreateDir,have,but file"
#define FFS_DIR_336_112_2_18_1_30_38_121 "FFS,DoCreateDir,not have"
#define FFS_DIR_574_112_2_18_1_30_39_122 "FFS,DoDeleteDir,only a /"
#define FFS_DIR_615_112_2_18_1_30_39_123 "FFS,DoDeleteDir,have name,dir"
#define FFS_DIR_619_112_2_18_1_30_39_124 "FFS,DoDeleteDir,have name,last"
#define FFS_DIR_667_112_2_18_1_30_39_125 "FFS,DoDeleteDir,have name,not last"
#define FFS_DIR_677_112_2_18_1_30_39_126 "FFS,DoDeleteDir,have but file,return"
#define FFS_DIR_686_112_2_18_1_30_39_127 "FFS,DoDeleteDir,not have name,return"
#define FFS_DIR_740_112_2_18_1_30_39_128 "FFS,DoCD,2f00"
#define FFS_DIR_746_112_2_18_1_30_39_129 "FFS,DoCD,root,return"
#define FFS_DIR_777_112_2_18_1_30_39_130 "FFS,DoCD,truncate=%x,%x,%x,%x,%x,%x"
#define FFS_DIR_791_112_2_18_1_30_39_131 "FFS,DoCD,have name,clusterID=%d"
#define FFS_DIR_802_112_2_18_1_30_39_132 "FFS,DoCD,have name,dir"
#define FFS_DIR_805_112_2_18_1_30_39_133 "FFS,DoCD,have name,last dir"
#define FFS_DIR_817_112_2_18_1_30_39_134 "FFS,DoCD,have name,not last dir"
#define FFS_DIR_823_112_2_18_1_30_39_135 "FFS,DoCD,have name,not dir"
#define FFS_DIR_832_112_2_18_1_30_39_136 "FFS,DoCD,not have name"
#define FFS_RW_116_112_2_18_1_30_47_137 "!!!!!!!FFS DoWrite() in write_to_cluster_buf,0x0000, offset=%d,len=%d"
#define FFS_RW_154_112_2_18_1_30_47_138 "!!!!!!!FFS DoWrite() in write_to_cluster_buf,0x0001, offset=%d,len=%d,fcb->cluster_pos= %d"
#define FFS_RW_222_112_2_18_1_30_47_139 "!!!!!!!FFS Do read read_from_cluster_buf 0x0000 , offset=%d,len=%d,fcb->cluster_pos= %d"
#define FFS_RW_260_112_2_18_1_30_47_140 "FFS read from clu buf ,not in fcb"
#define FFS_RW_305_112_2_18_1_30_47_141 "!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset,0x0000,cluster_id=%d"
#define FFS_RW_321_112_2_18_1_30_47_142 "FFS read:read clu buf by,fail1"
#define FFS_RW_348_112_2_18_1_30_47_143 "!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset,go ahead,cluster_id=%d"
#define FFS_RW_361_112_2_18_1_30_47_144 "!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset, back search,cluster_id=%d"
#define FFS_RW_370_112_2_18_1_30_47_145 "[read_cluster_buf_by_offset] cluster_id_buf is valid"
#define FFS_RW_382_112_2_18_1_30_47_146 "!!!!!!!!FFS DoWrite() in read_cluster_buf_offset, go ahead, cluster_id = %d"
#define FFS_RW_414_112_2_18_1_30_48_147 "[read_cluster_buf_by_offset] set up cluster_id_buf: invalid cluster_id = %d"
#define FFS_RW_435_112_2_18_1_30_48_148 "[read_cluster_buf_by_offset] invalid cluster_id = %d"
#define FFS_RW_465_112_2_18_1_30_48_149 "FFS read:read clu buf by,fail2"
#define FFS_RW_509_112_2_18_1_30_48_150 "!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset,0x0000,cluster_id=%d"
#define FFS_RW_548_112_2_18_1_30_48_151 "!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset,go ahead,cluster_id=%d"
#define FFS_RW_561_112_2_18_1_30_48_152 "!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset, back search,cluster_id=%d"
#define FFS_RW_703_112_2_18_1_30_48_153 "FFS read firclu=0x0"
#define FFS_RW_709_112_2_18_1_30_48_154 "FFS read offset +len>length"
#define FFS_RW_725_112_2_18_1_30_48_155 "FFS read clu buf by,0"
#define FFS_RW_732_112_2_18_1_30_48_156 "FFS read fro clu buf ,0"
#define FFS_RW_812_112_2_18_1_30_48_157 "FFS write apply fail"
#define FFS_RW_827_112_2_18_1_30_48_158 "FFS write:read clu buf by,0"
#define FFS_RW_834_112_2_18_1_30_48_159 "FFS write:write to clu buf ,0"
#define FFS_TASK_139_112_2_18_1_30_49_160 "FFS cmd:%d is not legal in FFS_ERROR_INVALID_PARAM state"
#define FFS_TASK_143_112_2_18_1_30_49_161 "FFS cmd :%d done  in FFS_ERROR_INVALID_PARAM state,error_code %x,result %x"
#define FFS_TASK_159_112_2_18_1_30_49_162 "FFS cmd:%d is not legal in FFS_ERROR_SYSTEM state"
#define FFS_TASK_163_112_2_18_1_30_49_163 "FFS cmd :%d done  in FFS_ERROR_NOT_EXIST|FFS_ERROR_SYSTEM state,error_code %x,result %x"
#define FFS_TASK_169_112_2_18_1_30_49_164 "!!!!!!!FFS_ProcessIRP cmd:%d"
#define FFS_TASK_263_112_2_18_1_30_50_165 "FFS cmd:%d is not legal in FFS_NO_ERROR state"
#define FFS_TASK_269_112_2_18_1_30_50_166 "FFS cmd :%d done  in FFS_NO_ERROR state,error_code %x,result %x"
#define FFS_TASK_302_112_2_18_1_30_50_167 "FFS Start Reclaim Sector"
#define FFS_TASK_308_112_2_18_1_30_50_168 "End Reclaim Sector(Tick: %d)"
#define FFS_TASK_346_112_2_18_1_30_50_169 "FFS FAT_Init: UdiskCreate ERROR!"
#define FFS_TASK_400_112_2_18_1_30_50_170 "FFS_Init:UdiskFormat error"
#define FFS_TASK_406_112_2_18_1_30_50_171 "FFS_Init:UdiskInit error"
#define FFS_TASK_410_112_2_18_1_30_50_172 "FFS FAT_Init: FFS_DoFormat SUCCESS!"
#define FFS_TASK_506_112_2_18_1_30_50_173 "FFS,bsd_compaction"
#define UDISKDEV_608_112_2_18_1_31_7_174 "enter udiskcreate"
#define UDISKDEV_617_112_2_18_1_31_7_175 "[FFS:UdiskCreate] Unknown device %u\r\n"
#define UDISKDEV_623_112_2_18_1_31_7_176 "[FFS:UdiskCreate] BSD_Open failed(0x%x)\r\n"
#define UDISKDEV_673_112_2_18_1_31_8_177 "enter DevPtr->Init();"
#define UDISKDEV_675_112_2_18_1_31_8_178 "left DevPtr->Init();"
#define UDISKDEV_885_112_2_18_1_31_8_179 "udiskInit read first sector failure!!! "
#define UDISKDEV_899_112_2_18_1_31_8_180 "udiskinit,basic signature doesn't fit, format!"
#define UDISKDEV_903_112_2_18_1_31_8_181 "udiskinit format fail"
#define UDISKDEV_907_112_2_18_1_31_8_182 "udiskinit format suc,but we still didn't read bpb"
#define UDISKDEV_913_112_2_18_1_31_8_183 "udiskInit 2 read first sector failure!!! "
#define UDISKDEV_924_112_2_18_1_31_8_184 "udiskInit ,signature cause format,then read bpb fit!!"
#define UDISKDEV_930_112_2_18_1_31_8_185 "udiskinit,the signature fit!"
#define UDISKDEV_936_112_2_18_1_31_8_186 "udiskInit ,no MBR on disk!!! "
#define UDISKDEV_945_112_2_18_1_31_8_187 "udiskInit ,no MBR on disk,bpb fit!! "
#define UDISKDEV_959_112_2_18_1_31_8_188 "udiskinit,relative_start=%d,dptsectors=%d"
#define UDISKDEV_962_112_2_18_1_31_8_189 "udiskInit read relative_start sector failure!!! "
#define UDISKDEV_970_112_2_18_1_31_8_190 "udiskinit,1,DevPtr->Disk_size=%d"
#define UDISKDEV_983_112_2_18_1_31_8_191 "udiskInit,the sector is not a DBR,and not a MBR,format !!!"
#define UDISKDEV_985_112_2_18_1_31_8_192 "udiskinit,2,DevPtr->Disk_size=%d"
#define UDISKDEV_988_112_2_18_1_31_8_193 "udiskInit,not a DBRand MBR,format fail!!!"
#define UDISKDEV_997_112_2_18_1_31_8_194 "udiskInit 2 read first sector failure!!! "
#define UDISKDEV_1008_112_2_18_1_31_8_195 "udiskInit ,not a DBRand MBR,format,then read bpb fit!!"
#define UDISKDEV_1017_112_2_18_1_31_8_196 "FAT16format:after readBPB,and find not fat16!!!"
#define UDISKDEV_1019_112_2_18_1_31_8_197 "udiskinit,3,DevPtr->Disk_size=%d"
#define UDISKDEV_1023_112_2_18_1_31_8_198 "FAT16format:after readBPB,and find not fat16,format it failure,set IsReady FALSE!!!"
#define UDISKDEV_1037_112_2_18_1_31_8_199 "udisk is formated!!!!"
#define UDISKDEV_1079_112_2_18_1_31_8_200 "FAT16format:fat16_read_in_normal_fat_table failure!!!"
#define UDISKDEV_1085_112_2_18_1_31_8_201 "FAT16format:fat16_read_in_root-dir failure!!!"
#define UDISKDEV_1090_112_2_18_1_31_8_202 "FAT16format:after read infattable and root dir!!!"
#define UDISKDEV_1170_112_2_18_1_31_9_203 "udiskInit read first sector failure!!! "
#define UDISKDEV_1192_112_2_18_1_31_9_204 "udiskinit,the signature fit!"
#define UDISKDEV_1199_112_2_18_1_31_9_205 "udiskInit ,no MBR on disk!!! "
#define UDISKDEV_1209_112_2_18_1_31_9_206 "udiskInit ,no MBR on disk,bpb fit!! "
#define UDISKDEV_1227_112_2_18_1_31_9_207 "udiskinit,relative_start=%d,dptsectors=%d"
#define UDISKDEV_1230_112_2_18_1_31_9_208 "udiskInit read relative_start sector failure!!! "
#define UDISKDEV_1238_112_2_18_1_31_9_209 "udiskinit,1,DevPtr->Disk_size=%d"
#define UDISKDEV_1268_112_2_18_1_31_9_210 "udisk is formated!!!!"
#define UDISKDEV_1310_112_2_18_1_31_9_211 "FAT16format:fat16_read_in_normal_fat_table failure!!!"
#define UDISKDEV_1317_112_2_18_1_31_9_212 "FAT16format:fat16_read_in_root-dir failure!!!"
#define UDISKDEV_1323_112_2_18_1_31_9_213 "FAT16format:after read infattable and root dir!!!"
#define UDISKDEV_1515_112_2_18_1_31_9_214 "format: DevPtr->size = %d, count = %d"
#define UDISKDEV_1524_112_2_18_1_31_9_215 "FAT16format disk size not in count!!!"
#define UDISKDEV_1532_112_2_18_1_31_9_216 "FAT16format begin!!!"
#define UDISKDEV_1601_112_2_18_1_31_9_217 "FAT16format enter into!!!"
#define UDISKDEV_1623_112_2_18_1_31_10_218 "FAT16format count = %d, disk_size = %d"
#define UDISKDEV_1664_112_2_18_1_31_10_219 "FFS!!!bsd delete fail1"
#define UDISKDEV_1669_112_2_18_1_31_10_220 "FFS!!!bsd delete fail2"
#define UDISKDEV_1676_112_2_18_1_31_10_221 "FFS!!!bsd delete fail1"
#define UDISKDEV_1681_112_2_18_1_31_10_222 "FFS!!!bsd delete fail2"
#define UDISKDEV_1687_112_2_18_1_31_10_223 "FFS!!!bsd delete fail3"
#define UDISKDEV_1734_112_2_18_1_31_10_224 "FAT16format enter into!!!"
#define UDISKDEV_1778_112_2_18_1_31_10_225 "udiskfat16format suc!"
#define UDISKDEV_3239_112_2_18_1_31_13_226 "FFS clean:nxt_clu=%x"
#define UDISKDEV_3254_112_2_18_1_31_13_227 "FFS!!!bsd delete fail2"
#define UDISKDEV_3271_112_2_18_1_31_13_228 "FFS clean:back true"
#define UDISKDEV_3277_112_2_18_1_31_13_229 "FFS : this file has wrong tail ,it is a bad file,but we just think wrong tail is a tail"
#define UDISKDEV_3284_112_2_18_1_31_13_230 "FFS clean:cur_sec!=nxt_sec"
#define UDISKDEV_3930_112_2_18_1_31_14_231 "FFS expand fail"
#define UDISKDEV_3972_112_2_18_1_31_14_232 "FFS get_next_cluster_id < 0x2"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_FFS_TRC)
TRACE_MSG(FFDEV_79_112_2_18_1_30_27_0,"FFS name is:")
TRACE_MSG(FFDEV_88_112_2_18_1_30_27_1,"%s")
TRACE_MSG(FFDEV_94_112_2_18_1_30_27_2,"%s")
TRACE_MSG(FFDEV_302_112_2_18_1_30_27_3,"FFS FAT_Create DEV NO %x,bMode = %x")
TRACE_MSG(FFDEV_345_112_2_18_1_30_27_4,"FFS FAT_Create:result = %x")
TRACE_MSG(FFDEV_400_112_2_18_1_30_27_5,"FFS FAT_Close Fhandle = %x,dev NO %x")
TRACE_MSG(FFDEV_711_112_2_18_1_30_28_6,"FFS FAT_chsize DEV NO %x")
TRACE_MSG(FFDEV_712_112_2_18_1_30_28_7,"FFS FAT_chsize:fhande %x,size %x")
TRACE_MSG(FFDEV_737_112_2_18_1_30_28_8,"FFS FAT_GetLength Fhandle:%x,DEV NO %x")
TRACE_MSG(FFDEV_741_112_2_18_1_30_28_9,"FFS FAT_GetLength result = 0")
TRACE_MSG(FFDEV_745_112_2_18_1_30_28_10,"FFS FAT_GetLength result = %x")
TRACE_MSG(FFDEV_827_112_2_18_1_30_28_11,"FFS FAT_Read: Fhandle %x,buf %x ,len %x,overlapped:%x")
TRACE_MSG(FFDEV_984_112_2_18_1_30_29_12,"FFS FAT_Write: Fhandle %x,buf %x ,len %x,overlapped:%x")
TRACE_MSG(FFDEV_1101_112_2_18_1_30_29_13,"FFS FAT_Delete: DEV NO %x,overlapped %x")
TRACE_MSG(FFDEV_1191_112_2_18_1_30_29_14,"FFS FAT_Rename Device No %d")
TRACE_MSG(FFDEV_1347_112_2_18_1_30_29_15,"FFS FAT_FindNext: fhandle %x")
TRACE_MSG(FFDEV_1402_112_2_18_1_30_29_16,"FFS FAT_CreateDir DEV NO %x")
TRACE_MSG(FFDEV_1463_112_2_18_1_30_30_17,"FFS FAT_DeleteDir DEV NO %x")
TRACE_MSG(FFDEV_1521_112_2_18_1_30_30_18,"FFS FAT_OpenDir DEV NO %x")
TRACE_MSG(FFDEV_1572_112_2_18_1_30_30_19,"FFS FAT_Seek:Fhandle = %x,dev NO %x,offset %x ,origin %x")
TRACE_MSG(FFDEV_1745_112_2_18_1_30_30_20,"FFS FFS_init dev No %x")
TRACE_MSG(FFDEV_1752_112_2_18_1_30_30_21,"FFS_init,ud is null")
TRACE_MSG(FFDEV_1761_112_2_18_1_30_30_22,"FFS_init,sd is null")
TRACE_MSG(FFDEV_1776_112_2_18_1_30_30_23,"FFS_Init: file_dev_type(%d) error")
TRACE_MSG(FFDEV_1878_112_2_18_1_30_30_24,"FFS FFS_Shutdown DEV NO %x,overlapped,%x")
TRACE_MSG(FFDEV_1955_112_2_18_1_30_31_25,"FFS FFS_Close")
TRACE_MSG(FFDEV_1980_112_2_18_1_30_31_26,"FFS FFS_GetLength")
TRACE_MSG(FFDEV_2018_112_2_18_1_30_31_27,"FFS_Create")
TRACE_MSG(FFDEV_2032_112_2_18_1_30_31_28,"FFS_Create,ud is null")
TRACE_MSG(FFDEV_2042_112_2_18_1_30_31_29,"FFS_Create,sd is null")
TRACE_MSG(FFDEV_2068_112_2_18_1_30_31_30,"FFS FFS_Seek")
TRACE_MSG(FFDEV_2545_112_2_18_1_30_32_31,"FFS,freespace,ud is null")
TRACE_MSG(FFDEV_2554_112_2_18_1_30_32_32,"FFS,freespace,sd is null")
TRACE_MSG(FFDEV_2584_112_2_18_1_30_32_33,"FFS,usedspace,ud is null")
TRACE_MSG(FFDEV_2593_112_2_18_1_30_32_34,"FFS,usedspace,sd is null")
TRACE_MSG(FFDEV_2735_112_2_18_1_30_32_35,"FFS_fopen mode = %x")
TRACE_MSG(FFDEV_2749_112_2_18_1_30_32_36,"FFS,fopen,ud is null")
TRACE_MSG(FFDEV_2758_112_2_18_1_30_32_37,"FFS,fopen,sd is null")
TRACE_MSG(FFDEV_3166_112_2_18_1_30_33_38,"FFS,format,ud is null")
TRACE_MSG(FFDEV_3174_112_2_18_1_30_33_39,"FFS,format,sd is null")
TRACE_MSG(FFS_CREATE_148_112_2_18_1_30_34_40,"FFS add_entry,root")
TRACE_MSG(FFS_CREATE_151_112_2_18_1_30_34_41,"FFS add_entry,root,short")
TRACE_MSG(FFS_CREATE_167_112_2_18_1_30_34_42,"FFS add_entry,root,short,get entryID fail")
TRACE_MSG(FFS_CREATE_172_112_2_18_1_30_34_43,"FFS add_entry,root,long")
TRACE_MSG(FFS_CREATE_193_112_2_18_1_30_34_44,"FFS add_entry,long,beg for")
TRACE_MSG(FFS_CREATE_211_112_2_18_1_30_35_45,"FFS add_entry,long,write entries end ")
TRACE_MSG(FFS_CREATE_219_112_2_18_1_30_35_46,"FFS add_entry,in cluster")
TRACE_MSG(FFS_CREATE_224_112_2_18_1_30_35_47,"FFS add_entry,in cluster,short")
TRACE_MSG(FFS_CREATE_229_112_2_18_1_30_35_48,"FFS add_entry,in cluster,short,have entry")
TRACE_MSG(FFS_CREATE_251_112_2_18_1_30_35_49,"FFS add_entry,in cluster,short,have no entry")
TRACE_MSG(FFS_CREATE_259_112_2_18_1_30_35_50,"FFS add_entry,in cluster,long")
TRACE_MSG(FFS_CREATE_269_112_2_18_1_30_35_51,"FFS add_entry,in cluster,long,have entry")
TRACE_MSG(FFS_CREATE_321_112_2_18_1_30_35_52,"FFS add_entry,in cluster,long,in else")
TRACE_MSG(FFS_CREATE_339_112_2_18_1_30_35_53,"FFS add_entry,in cluster,long,new clu")
TRACE_MSG(FFS_CREATE_372_112_2_18_1_30_35_54,"FFS add_entry,in cluster,long,retn true")
TRACE_MSG(FFS_CREATE_379_112_2_18_1_30_35_55,"FFS add_entry,in cluster,long,last,no entry")
TRACE_MSG(FFS_CREATE_458_112_2_18_1_30_35_56,"FFS del_entry,root")
TRACE_MSG(FFS_CREATE_465_112_2_18_1_30_35_57,"FFS del_entry,root,long")
TRACE_MSG(FFS_CREATE_476_112_2_18_1_30_35_58,"FFS del_entry,in clu")
TRACE_MSG(FFS_CREATE_483_112_2_18_1_30_35_59,"FFS del_entry,in clu,long")
TRACE_MSG(FFS_CREATE_486_112_2_18_1_30_35_60,"FFS del_entry,in clu,long,lclu!=clu")
TRACE_MSG(FFS_CREATE_500_112_2_18_1_30_35_61,"FFS del_entry,in clu,get nxt clu")
TRACE_MSG(FFS_CREATE_506_112_2_18_1_30_35_62,"FFS del_entry,in clu,nxt clu")
TRACE_MSG(FFS_CREATE_531_112_2_18_1_30_35_63,"FFS del_entry,in clu,long,for,end")
TRACE_MSG(FFS_CREATE_534_112_2_18_1_30_35_64,"FFS del_entry,in clu,retn right")
TRACE_MSG(FFS_CREATE_538_112_2_18_1_30_35_65,"FFS del_entry,in clu,lclu=clu")
TRACE_MSG(FFS_CREATE_545_112_2_18_1_30_35_66,"FFS del_entry,in clu,back true")
TRACE_MSG(FFS_CREATE_549_112_2_18_1_30_35_67,"FFS del_entry,in clu,short")
TRACE_MSG(FFS_CREATE_620_112_2_18_1_30_35_68,"FFS,Create,it's a absolute path")
TRACE_MSG(FFS_CREATE_626_112_2_18_1_30_35_69,"FFS,Create,only a /")
TRACE_MSG(FFS_CREATE_668_112_2_18_1_30_35_70,"FFS create sech1 in clu retn 0x3")
TRACE_MSG(FFS_CREATE_675_112_2_18_1_30_35_71,"FFS create have name1")
TRACE_MSG(FFS_CREATE_679_112_2_18_1_30_35_72,"FFS create have name,dir")
TRACE_MSG(FFS_CREATE_683_112_2_18_1_30_35_73,"FFS create have name not dir")
TRACE_MSG(FFS_CREATE_696_112_2_18_1_30_35_74,"FFS create have name,file")
TRACE_MSG(FFS_CREATE_699_112_2_18_1_30_35_75,"FFS create have name,but dir")
TRACE_MSG(FFS_CREATE_712_112_2_18_1_30_36_76,"FFS create ,check access")
TRACE_MSG(FFS_CREATE_721_112_2_18_1_30_36_77,"FFS create ,check open")
TRACE_MSG(FFS_CREATE_729_112_2_18_1_30_36_78,"FFS create ,not opened")
TRACE_MSG(FFS_CREATE_772_112_2_18_1_30_36_79,"FFS create ,overwrite")
TRACE_MSG(FFS_CREATE_786_112_2_18_1_30_36_80,"FFS create ,clean1 end")
TRACE_MSG(FFS_CREATE_823_112_2_18_1_30_36_81,"FFS create ,overwrite,new fcb")
TRACE_MSG(FFS_CREATE_848_112_2_18_1_30_36_82,"FFS create ,not have name")
TRACE_MSG(FFS_CREATE_859_112_2_18_1_30_36_83,"FFS create ,not have name,dir")
TRACE_MSG(FFS_CREATE_891_112_2_18_1_30_36_84,"FFS create ,not have name,add entry")
TRACE_MSG(FFS_CREATE_895_112_2_18_1_30_36_85,"FFS create ,add entry fail")
TRACE_MSG(FFS_CREATE_898_112_2_18_1_30_36_86,"FFS create ,add entry suc")
TRACE_MSG(FFS_CREATE_915_112_2_18_1_30_36_87,"FFS create ,add entry suc,write back suc")
TRACE_MSG(FFS_CREATE_920_112_2_18_1_30_36_88,"FFS create ,write dot in dir beg")
TRACE_MSG(FFS_CREATE_966_112_2_18_1_30_36_89,"FFS create ,write dot in dir suc")
TRACE_MSG(FFS_CREATE_971_112_2_18_1_30_36_90,"FFS create ,write dot in dir suc,dir")
TRACE_MSG(FFS_CREATE_986_112_2_18_1_30_36_91,"FFS create ,write dot in dir suc,file")
TRACE_MSG(FFS_CREATE_998_112_2_18_1_30_36_92,"FFS create ,write dot in dir suc,sech in dir suc end")
TRACE_MSG(FFS_CREATE_1016_112_2_18_1_30_36_93,"FFS create ,into while")
TRACE_MSG(FFS_CREATE_1033_112_2_18_1_30_36_94,"FFS create ,still have /")
TRACE_MSG(FFS_CREATE_1048_112_2_18_1_30_36_95,"FFS create ,have no /")
TRACE_MSG(FFS_CREATE_1071_112_2_18_1_30_36_96,"FFS create ,add_entry2")
TRACE_MSG(FFS_CREATE_1077_112_2_18_1_30_36_97,"FFS create ,add_entry end")
TRACE_MSG(FFS_CREATE_1103_112_2_18_1_30_36_98,"FFS create ,add_entry2 & write back end")
TRACE_MSG(FFS_CREATE_1108_112_2_18_1_30_36_99,"FFS create ,add_entry2,dir")
TRACE_MSG(FFS_CREATE_1138_112_2_18_1_30_36_100,"FFS create ,add_entry2,end")
TRACE_MSG(FFS_CREATE_1142_112_2_18_1_30_36_101,"FFS create ,add_entry2,file")
TRACE_MSG(FFS_CREATE_1162_112_2_18_1_30_37_102,"FFS create ,in while end ,last")
TRACE_MSG(FFS_DELETE_139_112_2_18_1_30_37_103,"FFS dodel,have name")
TRACE_MSG(FFS_DELETE_142_112_2_18_1_30_37_104,"FFS dodel,have name,dir")
TRACE_MSG(FFS_DELETE_154_112_2_18_1_30_37_105,"FFS dodel,have dir,continue")
TRACE_MSG(FFS_DELETE_160_112_2_18_1_30_37_106,"FFS dodel,have,file")
TRACE_MSG(FFS_DELETE_163_112_2_18_1_30_37_107,"FFS dodel,have,file,last")
TRACE_MSG(FFS_DELETE_166_112_2_18_1_30_37_108,"FFS dodel,del beg")
TRACE_MSG(FFS_DELETE_168_112_2_18_1_30_37_109,"FFS dodel,del end")
TRACE_MSG(FFS_DELETE_177_112_2_18_1_30_37_110,"FFS dodel,clean end")
TRACE_MSG(FFS_DELETE_185_112_2_18_1_30_37_111,"FFS dodel,not last but file")
TRACE_MSG(FFS_DELETE_193_112_2_18_1_30_37_112,"FFS dodel,have no name")
TRACE_MSG(FFS_DIR_265_112_2_18_1_30_38_113,"FFS,DoCreateDir,only a /")
TRACE_MSG(FFS_DIR_271_112_2_18_1_30_38_114,"FFS,DoCreateDir,not root")
TRACE_MSG(FFS_DIR_282_112_2_18_1_30_38_115,"FFS,DoCreateDir,root")
TRACE_MSG(FFS_DIR_303_112_2_18_1_30_38_116,"FFS,DoCreateDir,have name")
TRACE_MSG(FFS_DIR_306_112_2_18_1_30_38_117,"FFS,DoCreateDir,have name,last")
TRACE_MSG(FFS_DIR_315_112_2_18_1_30_38_118,"FFS,DoCreateDir,have,dir")
TRACE_MSG(FFS_DIR_317_112_2_18_1_30_38_119,"FFS,DoCreateDir,clusterID=%d")
TRACE_MSG(FFS_DIR_327_112_2_18_1_30_38_120,"FFS,DoCreateDir,have,but file")
TRACE_MSG(FFS_DIR_336_112_2_18_1_30_38_121,"FFS,DoCreateDir,not have")
TRACE_MSG(FFS_DIR_574_112_2_18_1_30_39_122,"FFS,DoDeleteDir,only a /")
TRACE_MSG(FFS_DIR_615_112_2_18_1_30_39_123,"FFS,DoDeleteDir,have name,dir")
TRACE_MSG(FFS_DIR_619_112_2_18_1_30_39_124,"FFS,DoDeleteDir,have name,last")
TRACE_MSG(FFS_DIR_667_112_2_18_1_30_39_125,"FFS,DoDeleteDir,have name,not last")
TRACE_MSG(FFS_DIR_677_112_2_18_1_30_39_126,"FFS,DoDeleteDir,have but file,return")
TRACE_MSG(FFS_DIR_686_112_2_18_1_30_39_127,"FFS,DoDeleteDir,not have name,return")
TRACE_MSG(FFS_DIR_740_112_2_18_1_30_39_128,"FFS,DoCD,2f00")
TRACE_MSG(FFS_DIR_746_112_2_18_1_30_39_129,"FFS,DoCD,root,return")
TRACE_MSG(FFS_DIR_777_112_2_18_1_30_39_130,"FFS,DoCD,truncate=%x,%x,%x,%x,%x,%x")
TRACE_MSG(FFS_DIR_791_112_2_18_1_30_39_131,"FFS,DoCD,have name,clusterID=%d")
TRACE_MSG(FFS_DIR_802_112_2_18_1_30_39_132,"FFS,DoCD,have name,dir")
TRACE_MSG(FFS_DIR_805_112_2_18_1_30_39_133,"FFS,DoCD,have name,last dir")
TRACE_MSG(FFS_DIR_817_112_2_18_1_30_39_134,"FFS,DoCD,have name,not last dir")
TRACE_MSG(FFS_DIR_823_112_2_18_1_30_39_135,"FFS,DoCD,have name,not dir")
TRACE_MSG(FFS_DIR_832_112_2_18_1_30_39_136,"FFS,DoCD,not have name")
TRACE_MSG(FFS_RW_116_112_2_18_1_30_47_137,"!!!!!!!FFS DoWrite() in write_to_cluster_buf,0x0000, offset=%d,len=%d")
TRACE_MSG(FFS_RW_154_112_2_18_1_30_47_138,"!!!!!!!FFS DoWrite() in write_to_cluster_buf,0x0001, offset=%d,len=%d,fcb->cluster_pos= %d")
TRACE_MSG(FFS_RW_222_112_2_18_1_30_47_139,"!!!!!!!FFS Do read read_from_cluster_buf 0x0000 , offset=%d,len=%d,fcb->cluster_pos= %d")
TRACE_MSG(FFS_RW_260_112_2_18_1_30_47_140,"FFS read from clu buf ,not in fcb")
TRACE_MSG(FFS_RW_305_112_2_18_1_30_47_141,"!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset,0x0000,cluster_id=%d")
TRACE_MSG(FFS_RW_321_112_2_18_1_30_47_142,"FFS read:read clu buf by,fail1")
TRACE_MSG(FFS_RW_348_112_2_18_1_30_47_143,"!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset,go ahead,cluster_id=%d")
TRACE_MSG(FFS_RW_361_112_2_18_1_30_47_144,"!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset, back search,cluster_id=%d")
TRACE_MSG(FFS_RW_370_112_2_18_1_30_47_145,"[read_cluster_buf_by_offset] cluster_id_buf is valid")
TRACE_MSG(FFS_RW_382_112_2_18_1_30_47_146,"!!!!!!!!FFS DoWrite() in read_cluster_buf_offset, go ahead, cluster_id = %d")
TRACE_MSG(FFS_RW_414_112_2_18_1_30_48_147,"[read_cluster_buf_by_offset] set up cluster_id_buf: invalid cluster_id = %d")
TRACE_MSG(FFS_RW_435_112_2_18_1_30_48_148,"[read_cluster_buf_by_offset] invalid cluster_id = %d")
TRACE_MSG(FFS_RW_465_112_2_18_1_30_48_149,"FFS read:read clu buf by,fail2")
TRACE_MSG(FFS_RW_509_112_2_18_1_30_48_150,"!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset,0x0000,cluster_id=%d")
TRACE_MSG(FFS_RW_548_112_2_18_1_30_48_151,"!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset,go ahead,cluster_id=%d")
TRACE_MSG(FFS_RW_561_112_2_18_1_30_48_152,"!!!!!!!FFS DoWrite() in read_cluster_buf_by_offset, back search,cluster_id=%d")
TRACE_MSG(FFS_RW_703_112_2_18_1_30_48_153,"FFS read firclu=0x0")
TRACE_MSG(FFS_RW_709_112_2_18_1_30_48_154,"FFS read offset +len>length")
TRACE_MSG(FFS_RW_725_112_2_18_1_30_48_155,"FFS read clu buf by,0")
TRACE_MSG(FFS_RW_732_112_2_18_1_30_48_156,"FFS read fro clu buf ,0")
TRACE_MSG(FFS_RW_812_112_2_18_1_30_48_157,"FFS write apply fail")
TRACE_MSG(FFS_RW_827_112_2_18_1_30_48_158,"FFS write:read clu buf by,0")
TRACE_MSG(FFS_RW_834_112_2_18_1_30_48_159,"FFS write:write to clu buf ,0")
TRACE_MSG(FFS_TASK_139_112_2_18_1_30_49_160,"FFS cmd:%d is not legal in FFS_ERROR_INVALID_PARAM state")
TRACE_MSG(FFS_TASK_143_112_2_18_1_30_49_161,"FFS cmd :%d done  in FFS_ERROR_INVALID_PARAM state,error_code %x,result %x")
TRACE_MSG(FFS_TASK_159_112_2_18_1_30_49_162,"FFS cmd:%d is not legal in FFS_ERROR_SYSTEM state")
TRACE_MSG(FFS_TASK_163_112_2_18_1_30_49_163,"FFS cmd :%d done  in FFS_ERROR_NOT_EXIST|FFS_ERROR_SYSTEM state,error_code %x,result %x")
TRACE_MSG(FFS_TASK_169_112_2_18_1_30_49_164,"!!!!!!!FFS_ProcessIRP cmd:%d")
TRACE_MSG(FFS_TASK_263_112_2_18_1_30_50_165,"FFS cmd:%d is not legal in FFS_NO_ERROR state")
TRACE_MSG(FFS_TASK_269_112_2_18_1_30_50_166,"FFS cmd :%d done  in FFS_NO_ERROR state,error_code %x,result %x")
TRACE_MSG(FFS_TASK_302_112_2_18_1_30_50_167,"FFS Start Reclaim Sector")
TRACE_MSG(FFS_TASK_308_112_2_18_1_30_50_168,"End Reclaim Sector(Tick: %d)")
TRACE_MSG(FFS_TASK_346_112_2_18_1_30_50_169,"FFS FAT_Init: UdiskCreate ERROR!")
TRACE_MSG(FFS_TASK_400_112_2_18_1_30_50_170,"FFS_Init:UdiskFormat error")
TRACE_MSG(FFS_TASK_406_112_2_18_1_30_50_171,"FFS_Init:UdiskInit error")
TRACE_MSG(FFS_TASK_410_112_2_18_1_30_50_172,"FFS FAT_Init: FFS_DoFormat SUCCESS!")
TRACE_MSG(FFS_TASK_506_112_2_18_1_30_50_173,"FFS,bsd_compaction")
TRACE_MSG(UDISKDEV_608_112_2_18_1_31_7_174,"enter udiskcreate")
TRACE_MSG(UDISKDEV_617_112_2_18_1_31_7_175,"[FFS:UdiskCreate] Unknown device %u\r\n")
TRACE_MSG(UDISKDEV_623_112_2_18_1_31_7_176,"[FFS:UdiskCreate] BSD_Open failed(0x%x)\r\n")
TRACE_MSG(UDISKDEV_673_112_2_18_1_31_8_177,"enter DevPtr->Init();")
TRACE_MSG(UDISKDEV_675_112_2_18_1_31_8_178,"left DevPtr->Init();")
TRACE_MSG(UDISKDEV_885_112_2_18_1_31_8_179,"udiskInit read first sector failure!!! ")
TRACE_MSG(UDISKDEV_899_112_2_18_1_31_8_180,"udiskinit,basic signature doesn't fit, format!")
TRACE_MSG(UDISKDEV_903_112_2_18_1_31_8_181,"udiskinit format fail")
TRACE_MSG(UDISKDEV_907_112_2_18_1_31_8_182,"udiskinit format suc,but we still didn't read bpb")
TRACE_MSG(UDISKDEV_913_112_2_18_1_31_8_183,"udiskInit 2 read first sector failure!!! ")
TRACE_MSG(UDISKDEV_924_112_2_18_1_31_8_184,"udiskInit ,signature cause format,then read bpb fit!!")
TRACE_MSG(UDISKDEV_930_112_2_18_1_31_8_185,"udiskinit,the signature fit!")
TRACE_MSG(UDISKDEV_936_112_2_18_1_31_8_186,"udiskInit ,no MBR on disk!!! ")
TRACE_MSG(UDISKDEV_945_112_2_18_1_31_8_187,"udiskInit ,no MBR on disk,bpb fit!! ")
TRACE_MSG(UDISKDEV_959_112_2_18_1_31_8_188,"udiskinit,relative_start=%d,dptsectors=%d")
TRACE_MSG(UDISKDEV_962_112_2_18_1_31_8_189,"udiskInit read relative_start sector failure!!! ")
TRACE_MSG(UDISKDEV_970_112_2_18_1_31_8_190,"udiskinit,1,DevPtr->Disk_size=%d")
TRACE_MSG(UDISKDEV_983_112_2_18_1_31_8_191,"udiskInit,the sector is not a DBR,and not a MBR,format !!!")
TRACE_MSG(UDISKDEV_985_112_2_18_1_31_8_192,"udiskinit,2,DevPtr->Disk_size=%d")
TRACE_MSG(UDISKDEV_988_112_2_18_1_31_8_193,"udiskInit,not a DBRand MBR,format fail!!!")
TRACE_MSG(UDISKDEV_997_112_2_18_1_31_8_194,"udiskInit 2 read first sector failure!!! ")
TRACE_MSG(UDISKDEV_1008_112_2_18_1_31_8_195,"udiskInit ,not a DBRand MBR,format,then read bpb fit!!")
TRACE_MSG(UDISKDEV_1017_112_2_18_1_31_8_196,"FAT16format:after readBPB,and find not fat16!!!")
TRACE_MSG(UDISKDEV_1019_112_2_18_1_31_8_197,"udiskinit,3,DevPtr->Disk_size=%d")
TRACE_MSG(UDISKDEV_1023_112_2_18_1_31_8_198,"FAT16format:after readBPB,and find not fat16,format it failure,set IsReady FALSE!!!")
TRACE_MSG(UDISKDEV_1037_112_2_18_1_31_8_199,"udisk is formated!!!!")
TRACE_MSG(UDISKDEV_1079_112_2_18_1_31_8_200,"FAT16format:fat16_read_in_normal_fat_table failure!!!")
TRACE_MSG(UDISKDEV_1085_112_2_18_1_31_8_201,"FAT16format:fat16_read_in_root-dir failure!!!")
TRACE_MSG(UDISKDEV_1090_112_2_18_1_31_8_202,"FAT16format:after read infattable and root dir!!!")
TRACE_MSG(UDISKDEV_1170_112_2_18_1_31_9_203,"udiskInit read first sector failure!!! ")
TRACE_MSG(UDISKDEV_1192_112_2_18_1_31_9_204,"udiskinit,the signature fit!")
TRACE_MSG(UDISKDEV_1199_112_2_18_1_31_9_205,"udiskInit ,no MBR on disk!!! ")
TRACE_MSG(UDISKDEV_1209_112_2_18_1_31_9_206,"udiskInit ,no MBR on disk,bpb fit!! ")
TRACE_MSG(UDISKDEV_1227_112_2_18_1_31_9_207,"udiskinit,relative_start=%d,dptsectors=%d")
TRACE_MSG(UDISKDEV_1230_112_2_18_1_31_9_208,"udiskInit read relative_start sector failure!!! ")
TRACE_MSG(UDISKDEV_1238_112_2_18_1_31_9_209,"udiskinit,1,DevPtr->Disk_size=%d")
TRACE_MSG(UDISKDEV_1268_112_2_18_1_31_9_210,"udisk is formated!!!!")
TRACE_MSG(UDISKDEV_1310_112_2_18_1_31_9_211,"FAT16format:fat16_read_in_normal_fat_table failure!!!")
TRACE_MSG(UDISKDEV_1317_112_2_18_1_31_9_212,"FAT16format:fat16_read_in_root-dir failure!!!")
TRACE_MSG(UDISKDEV_1323_112_2_18_1_31_9_213,"FAT16format:after read infattable and root dir!!!")
TRACE_MSG(UDISKDEV_1515_112_2_18_1_31_9_214,"format: DevPtr->size = %d, count = %d")
TRACE_MSG(UDISKDEV_1524_112_2_18_1_31_9_215,"FAT16format disk size not in count!!!")
TRACE_MSG(UDISKDEV_1532_112_2_18_1_31_9_216,"FAT16format begin!!!")
TRACE_MSG(UDISKDEV_1601_112_2_18_1_31_9_217,"FAT16format enter into!!!")
TRACE_MSG(UDISKDEV_1623_112_2_18_1_31_10_218,"FAT16format count = %d, disk_size = %d")
TRACE_MSG(UDISKDEV_1664_112_2_18_1_31_10_219,"FFS!!!bsd delete fail1")
TRACE_MSG(UDISKDEV_1669_112_2_18_1_31_10_220,"FFS!!!bsd delete fail2")
TRACE_MSG(UDISKDEV_1676_112_2_18_1_31_10_221,"FFS!!!bsd delete fail1")
TRACE_MSG(UDISKDEV_1681_112_2_18_1_31_10_222,"FFS!!!bsd delete fail2")
TRACE_MSG(UDISKDEV_1687_112_2_18_1_31_10_223,"FFS!!!bsd delete fail3")
TRACE_MSG(UDISKDEV_1734_112_2_18_1_31_10_224,"FAT16format enter into!!!")
TRACE_MSG(UDISKDEV_1778_112_2_18_1_31_10_225,"udiskfat16format suc!")
TRACE_MSG(UDISKDEV_3239_112_2_18_1_31_13_226,"FFS clean:nxt_clu=%x")
TRACE_MSG(UDISKDEV_3254_112_2_18_1_31_13_227,"FFS!!!bsd delete fail2")
TRACE_MSG(UDISKDEV_3271_112_2_18_1_31_13_228,"FFS clean:back true")
TRACE_MSG(UDISKDEV_3277_112_2_18_1_31_13_229,"FFS : this file has wrong tail ,it is a bad file,but we just think wrong tail is a tail")
TRACE_MSG(UDISKDEV_3284_112_2_18_1_31_13_230,"FFS clean:cur_sec!=nxt_sec")
TRACE_MSG(UDISKDEV_3930_112_2_18_1_31_14_231,"FFS expand fail")
TRACE_MSG(UDISKDEV_3972_112_2_18_1_31_14_232,"FFS get_next_cluster_id < 0x2")
END_TRACE_MAP(MS_REF_FFS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_FFS_TRC_H_


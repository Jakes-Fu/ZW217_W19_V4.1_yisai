/******************************************************************************
 ** File Name:      ms_ref_video_security_trc.h                                         *
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
//trace_id:78
#ifndef _MS_REF_VIDEO_SECURITY_TRC_H_
#define _MS_REF_VIDEO_SECURITY_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define SPRDKMV_SECURITY_PROCESS_152_112_2_18_1_55_29_0 "SPRDKMV_VDEC_SEC_Open,argc=%d,argv=0x%x."
#define SPRDKMV_SECURITY_PROCESS_157_112_2_18_1_55_29_1 "SPRDKMV_VDEC_SEC_Open,alloc failed."
#define SPRDKMV_SECURITY_PROCESS_169_112_2_18_1_55_29_2 "VPlayerSecurityPlug_Open,file_open failed."
#define SPRDKMV_SECURITY_PROCESS_178_112_2_18_1_55_29_3 "VPlayerSecurityPlug_Open,init_table failed."
#define SPRDKMV_SECURITY_PROCESS_193_112_2_18_1_55_30_4 "SPRDKMV_VDEC_SEC_Close,handle=0x%x"
#define SPRDKMV_SECURITY_PROCESS_213_112_2_18_1_55_30_5 "SPRDKMV_VDEC_SEC_GetInfo."
#define SPRDKMV_SECURITY_PROCESS_278_112_2_18_1_55_30_6 "SPRDKMV_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_INFO, data_len=%d"
#define SPRDKMV_SECURITY_PROCESS_304_112_2_18_1_55_30_7 "SPRDKMV_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_DATA,0x%x,%d"
#define SPRDKMV_SECURITY_PROCESS_328_112_2_18_1_55_30_8 "vdec_sec_file_open"
#define SPRDKMV_SECURITY_PROCESS_350_112_2_18_1_55_30_9 "vdec_sec_file_open,SFS_CreateFile failed"
#define SPRDKMV_SECURITY_PROCESS_366_112_2_18_1_55_30_10 "vdec_sec_file_open,file_header[4-7]=0x[%x,%x,%x,%x]"
#define SPRDKMV_SECURITY_PROCESS_401_112_2_18_1_55_30_11 "vdec_sec_file_close"
#define SPRDKMV_SECURITY_PROCESS_533_112_2_18_1_55_30_12 "vdec_sec_get_next_STCO_sample,pos_base=0x%x,next_pos=0x%x,entry_off=%d"
#define SPRDKMV_SECURITY_PROCESS_572_112_2_18_1_55_30_13 "vdec_sec_get_next_STSZ_sample,pos_base=0x%x,next_pos=0x%x,entry_off=%d,size=%d"
#define SPRDKMV_SECURITY_PROCESS_607_112_2_18_1_55_30_14 "vdec_sec_get_next_MAP_sample,pos_base=0x%x,next_pos=0x%x,size2read=%d"
#define SPRDKMV_SECURITY_PROCESS_681_112_2_18_1_55_30_15 "vdec_sec_init_table, in"
#define SPRDKMV_SECURITY_PROCESS_696_112_2_18_1_55_31_16 "vdec_sec_init_table, search STCO, box_size==0, data error!"
#define SPRDKMV_SECURITY_PROCESS_728_112_2_18_1_55_31_17 "vdec_sec_init_table, STCO OK"
#define SPRDKMV_SECURITY_PROCESS_754_112_2_18_1_55_31_18 "vdec_sec_init_table, read STSZ failed"
#define SPRDKMV_SECURITY_PROCESS_758_112_2_18_1_55_31_19 "vdec_sec_init_table, STSZ OK"
#define SPRDKMV_SECURITY_PROCESS_781_112_2_18_1_55_31_20 "vdec_sec_init_table, read SECMAP failed"
#define SPRDKMV_SECURITY_PROCESS_785_112_2_18_1_55_31_21 "vdec_sec_init_table, SECMAP OK"
#define SPRDKMV_SECURITY_PROCESS_797_112_2_18_1_55_31_22 "vdec_sec_init_table, read KEY failed"
#define SPRDKMV_SECURITY_PROCESS_803_112_2_18_1_55_31_23 "vdec_sec_init_table, read key head failed"
#define SPRDKMV_SECURITY_PROCESS_816_112_2_18_1_55_31_24 "vdec_sec_init_table, read key body failed"
#define SPRDKMV_SECURITY_PROCESS_820_112_2_18_1_55_31_25 "vdec_sec_init_table, KEY OK"
#define SPRDKMV_SECURITY_PROCESS_828_112_2_18_1_55_31_26 "vdec_sec_init_table, search udta, box_size==0, data error!"
#define SPRDKMV_SECURITY_PROCESS_883_112_2_18_1_55_31_27 "vdec_sec_init_table,return,STCO/STSZ/MAP/thumbnail/key,is_ok=[%d,%d,%d,%d,%d],pos=[%x,%x,%x,%x,%x]"
#define SPRDKMV_SECURITY_PROCESS_1060_112_2_18_1_55_31_28 "vdec_sec_get_matrix,frame_num=%d,cur_frame_id=%d, mb_matrix=0x%x, size=%d"
#define SPRDKMV_SECURITY_PROCESS_1077_112_2_18_1_55_31_29 "vdec_sec_get_matrix,map_size=0"
#define SPRDKMV_SECURITY_PROCESS_1444_112_2_18_1_55_32_30 "SPRDKMV_VDEC_SEC_FrmProc,in,frame_num=%d,f_in=0x[%x,%x,%x],f_out=0x[%x,%x,%x],mb_matrix=0x%x"
#define SPRDKMV_SECURITY_PROCESS_1450_112_2_18_1_55_32_31 "SPRDKMV_VDEC_SEC_FrmProc, get_matrix failed"
#define SPRDKMV_SECURITY_PROCESS_1867_112_2_18_1_55_33_32 "vdec_sec_init_table, get q, time=%d"
#define SPRDKMV_SECURITY_PROCESS_2023_112_2_18_1_55_33_33 "SPRDKMV_VDEC_SEC_StrmProc,in=[0x%x,%d],out=0x%x"
#define VIDEO_SECURITY_75_112_2_18_1_55_34_34 "hook_vdec_sec_plug_obj, security_FID=%d, invalid"
#define VIDEO_SECURITY_79_112_2_18_1_55_34_35 "hook_vdec_sec_plug_obj,ret=%d."
#define VIDEO_SECURITY_87_112_2_18_1_55_34_36 "hook_vdec_sec_plug_obj,ctx=0x%x."
#define VIDEO_SECURITY_98_112_2_18_1_55_34_37 "unhook_vdec_sec_plug_obj, security_FID=%d, invalid"
#define VIDEO_SECURITY_112_112_2_18_1_55_34_38 "VPlayerSecurity_Open."
#define VIDEO_SECURITY_118_112_2_18_1_55_34_39 "VPlayerSecurity_Open,alloc failed."
#define VIDEO_SECURITY_132_112_2_18_1_55_34_40 "VPlayerSecurity_Open, Success, VPLAYER_SECURITY_FID_SPRDKMV"
#define VIDEO_SECURITY_136_112_2_18_1_55_34_41 "VPlayerSecurity_Open, failed!"
#define VIDEO_SECURITY_147_112_2_18_1_55_34_42 "VPlayerSecurity_Close."
#define VIDEO_SECURITY_166_112_2_18_1_55_34_43 "VPlayerSecurity_GetFID."
#define VIDEO_SECURITY_188_112_2_18_1_55_34_44 "VPlayerSecurity_GetVDecSecPlugInfo."
#define VIDEO_SECURITY_198_112_2_18_1_55_34_45 "VPlayerSecurity_GetVDecSecPlugInfo,p_info=0x%x,strm_proc=0x%x,frame_proc=0x%x"
#define VIDEO_SECURITY_213_112_2_18_1_55_34_46 "VPlayerSecurity_GetThumbnailInfo."
#define VIDEO_SECURITY_242_112_2_18_1_55_34_47 "VPlayerSecurity_GetThumbnailInfo, invalid FID:%d"
#define VIDEO_SECURITY_258_112_2_18_1_55_34_48 "VPlayerSecurity_GetThumbnailData.p_buffer=0x%x,len=%d"
#define VIDEO_SECURITY_288_112_2_18_1_55_34_49 "VPlayerSecurity_GetThumbnailData, invalid FID:%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_VIDEO_SECURITY_TRC)
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_152_112_2_18_1_55_29_0,"SPRDKMV_VDEC_SEC_Open,argc=%d,argv=0x%x.")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_157_112_2_18_1_55_29_1,"SPRDKMV_VDEC_SEC_Open,alloc failed.")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_169_112_2_18_1_55_29_2,"VPlayerSecurityPlug_Open,file_open failed.")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_178_112_2_18_1_55_29_3,"VPlayerSecurityPlug_Open,init_table failed.")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_193_112_2_18_1_55_30_4,"SPRDKMV_VDEC_SEC_Close,handle=0x%x")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_213_112_2_18_1_55_30_5,"SPRDKMV_VDEC_SEC_GetInfo.")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_278_112_2_18_1_55_30_6,"SPRDKMV_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_INFO, data_len=%d")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_304_112_2_18_1_55_30_7,"SPRDKMV_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_DATA,0x%x,%d")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_328_112_2_18_1_55_30_8,"vdec_sec_file_open")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_350_112_2_18_1_55_30_9,"vdec_sec_file_open,SFS_CreateFile failed")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_366_112_2_18_1_55_30_10,"vdec_sec_file_open,file_header[4-7]=0x[%x,%x,%x,%x]")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_401_112_2_18_1_55_30_11,"vdec_sec_file_close")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_533_112_2_18_1_55_30_12,"vdec_sec_get_next_STCO_sample,pos_base=0x%x,next_pos=0x%x,entry_off=%d")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_572_112_2_18_1_55_30_13,"vdec_sec_get_next_STSZ_sample,pos_base=0x%x,next_pos=0x%x,entry_off=%d,size=%d")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_607_112_2_18_1_55_30_14,"vdec_sec_get_next_MAP_sample,pos_base=0x%x,next_pos=0x%x,size2read=%d")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_681_112_2_18_1_55_30_15,"vdec_sec_init_table, in")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_696_112_2_18_1_55_31_16,"vdec_sec_init_table, search STCO, box_size==0, data error!")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_728_112_2_18_1_55_31_17,"vdec_sec_init_table, STCO OK")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_754_112_2_18_1_55_31_18,"vdec_sec_init_table, read STSZ failed")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_758_112_2_18_1_55_31_19,"vdec_sec_init_table, STSZ OK")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_781_112_2_18_1_55_31_20,"vdec_sec_init_table, read SECMAP failed")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_785_112_2_18_1_55_31_21,"vdec_sec_init_table, SECMAP OK")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_797_112_2_18_1_55_31_22,"vdec_sec_init_table, read KEY failed")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_803_112_2_18_1_55_31_23,"vdec_sec_init_table, read key head failed")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_816_112_2_18_1_55_31_24,"vdec_sec_init_table, read key body failed")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_820_112_2_18_1_55_31_25,"vdec_sec_init_table, KEY OK")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_828_112_2_18_1_55_31_26,"vdec_sec_init_table, search udta, box_size==0, data error!")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_883_112_2_18_1_55_31_27,"vdec_sec_init_table,return,STCO/STSZ/MAP/thumbnail/key,is_ok=[%d,%d,%d,%d,%d],pos=[%x,%x,%x,%x,%x]")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_1060_112_2_18_1_55_31_28,"vdec_sec_get_matrix,frame_num=%d,cur_frame_id=%d, mb_matrix=0x%x, size=%d")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_1077_112_2_18_1_55_31_29,"vdec_sec_get_matrix,map_size=0")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_1444_112_2_18_1_55_32_30,"SPRDKMV_VDEC_SEC_FrmProc,in,frame_num=%d,f_in=0x[%x,%x,%x],f_out=0x[%x,%x,%x],mb_matrix=0x%x")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_1450_112_2_18_1_55_32_31,"SPRDKMV_VDEC_SEC_FrmProc, get_matrix failed")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_1867_112_2_18_1_55_33_32,"vdec_sec_init_table, get q, time=%d")
TRACE_MSG(SPRDKMV_SECURITY_PROCESS_2023_112_2_18_1_55_33_33,"SPRDKMV_VDEC_SEC_StrmProc,in=[0x%x,%d],out=0x%x")
TRACE_MSG(VIDEO_SECURITY_75_112_2_18_1_55_34_34,"hook_vdec_sec_plug_obj, security_FID=%d, invalid")
TRACE_MSG(VIDEO_SECURITY_79_112_2_18_1_55_34_35,"hook_vdec_sec_plug_obj,ret=%d.")
TRACE_MSG(VIDEO_SECURITY_87_112_2_18_1_55_34_36,"hook_vdec_sec_plug_obj,ctx=0x%x.")
TRACE_MSG(VIDEO_SECURITY_98_112_2_18_1_55_34_37,"unhook_vdec_sec_plug_obj, security_FID=%d, invalid")
TRACE_MSG(VIDEO_SECURITY_112_112_2_18_1_55_34_38,"VPlayerSecurity_Open.")
TRACE_MSG(VIDEO_SECURITY_118_112_2_18_1_55_34_39,"VPlayerSecurity_Open,alloc failed.")
TRACE_MSG(VIDEO_SECURITY_132_112_2_18_1_55_34_40,"VPlayerSecurity_Open, Success, VPLAYER_SECURITY_FID_SPRDKMV")
TRACE_MSG(VIDEO_SECURITY_136_112_2_18_1_55_34_41,"VPlayerSecurity_Open, failed!")
TRACE_MSG(VIDEO_SECURITY_147_112_2_18_1_55_34_42,"VPlayerSecurity_Close.")
TRACE_MSG(VIDEO_SECURITY_166_112_2_18_1_55_34_43,"VPlayerSecurity_GetFID.")
TRACE_MSG(VIDEO_SECURITY_188_112_2_18_1_55_34_44,"VPlayerSecurity_GetVDecSecPlugInfo.")
TRACE_MSG(VIDEO_SECURITY_198_112_2_18_1_55_34_45,"VPlayerSecurity_GetVDecSecPlugInfo,p_info=0x%x,strm_proc=0x%x,frame_proc=0x%x")
TRACE_MSG(VIDEO_SECURITY_213_112_2_18_1_55_34_46,"VPlayerSecurity_GetThumbnailInfo.")
TRACE_MSG(VIDEO_SECURITY_242_112_2_18_1_55_34_47,"VPlayerSecurity_GetThumbnailInfo, invalid FID:%d")
TRACE_MSG(VIDEO_SECURITY_258_112_2_18_1_55_34_48,"VPlayerSecurity_GetThumbnailData.p_buffer=0x%x,len=%d")
TRACE_MSG(VIDEO_SECURITY_288_112_2_18_1_55_34_49,"VPlayerSecurity_GetThumbnailData, invalid FID:%d")
END_TRACE_MAP(MS_REF_VIDEO_SECURITY_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_VIDEO_SECURITY_TRC_H_


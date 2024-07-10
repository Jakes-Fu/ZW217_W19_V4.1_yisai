/******************************************************************************
 ** File Name:      ms_ref_video_security_kmv_trc.h                                         *
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
//trace_id:79
#ifndef _MS_REF_VIDEO_SECURITY_KMV_TRC_H_
#define _MS_REF_VIDEO_SECURITY_KMV_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define KMVTEST_SECURITY_PROCESS_129_112_2_18_1_55_35_0 "KMVTEST_VDEC_SEC_Open,argc=%d,argv=0x%x."
#define KMVTEST_SECURITY_PROCESS_134_112_2_18_1_55_35_1 "KMVTEST_VDEC_SEC_Open,alloc failed."
#define KMVTEST_SECURITY_PROCESS_146_112_2_18_1_55_35_2 "VPlayerSecurityPlug_Open,file_open failed."
#define KMVTEST_SECURITY_PROCESS_155_112_2_18_1_55_35_3 "VPlayerSecurityPlug_Open,init_table failed."
#define KMVTEST_SECURITY_PROCESS_170_112_2_18_1_55_35_4 "KMVTEST_VDEC_SEC_Close,handle=0x%x"
#define KMVTEST_SECURITY_PROCESS_190_112_2_18_1_55_35_5 "KMVTEST_VDEC_SEC_GetInfo."
#define KMVTEST_SECURITY_PROCESS_255_112_2_18_1_55_36_6 "KMVTEST_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_INFO, data_len=%d"
#define KMVTEST_SECURITY_PROCESS_281_112_2_18_1_55_36_7 "KMVTEST_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_DATA,0x%x,%d"
#define KMVTEST_SECURITY_PROCESS_320_112_2_18_1_55_36_8 "KMVTEST_VDEC_SEC_StrmProc,in=[0x%x,%d],out=0x%x"
#define KMVTEST_SECURITY_PROCESS_396_112_2_18_1_55_36_9 "vdec_sec_file_open"
#define KMVTEST_SECURITY_PROCESS_418_112_2_18_1_55_36_10 "vdec_sec_file_open,SFS_CreateFile failed"
#define KMVTEST_SECURITY_PROCESS_434_112_2_18_1_55_36_11 "vdec_sec_file_open,file_header[4-7]=0x[%x,%x,%x,%x]"
#define KMVTEST_SECURITY_PROCESS_469_112_2_18_1_55_36_12 "vdec_sec_file_close"
#define KMVTEST_SECURITY_PROCESS_600_112_2_18_1_55_36_13 "vdec_sec_get_next_STCO_sample,pos_base=0x%x,next_pos=0x%x,entry_off=%d"
#define KMVTEST_SECURITY_PROCESS_637_112_2_18_1_55_36_14 "vdec_sec_get_next_STSZ_sample,pos_base=0x%x,next_pos=0x%x,entry_off=%d,size=%d"
#define KMVTEST_SECURITY_PROCESS_672_112_2_18_1_55_36_15 "vdec_sec_get_next_MAP_sample,pos_base=0x%x,next_pos=0x%x,size2read=%d"
#define KMVTEST_SECURITY_PROCESS_745_112_2_18_1_55_37_16 "vdec_sec_init_table, in"
#define KMVTEST_SECURITY_PROCESS_759_112_2_18_1_55_37_17 "vdec_sec_init_table, search STCO, box_size==0, data error!"
#define KMVTEST_SECURITY_PROCESS_791_112_2_18_1_55_37_18 "vdec_sec_init_table, STCO OK"
#define KMVTEST_SECURITY_PROCESS_816_112_2_18_1_55_37_19 "vdec_sec_init_table, STSZ OK"
#define KMVTEST_SECURITY_PROCESS_840_112_2_18_1_55_37_20 "vdec_sec_init_table, SECMAP OK"
#define KMVTEST_SECURITY_PROCESS_848_112_2_18_1_55_37_21 "vdec_sec_init_table, search udta, box_size==0, data error!"
#define KMVTEST_SECURITY_PROCESS_903_112_2_18_1_55_37_22 "vdec_sec_init_table,return,STCO/STSZ/MAP/thumbnail,is_ok=[%d,%d,%d,%d],pos=[%x,%x,%x,%x]"
#define KMVTEST_SECURITY_PROCESS_1131_112_2_18_1_55_37_23 "vdec_sec_get_matrix,frame_num=%d,cur_frame_id=%d"
#define KMVTEST_SECURITY_PROCESS_1148_112_2_18_1_55_37_24 "vdec_sec_get_matrix,map_size=0"
#define KMVTEST_SECURITY_PROCESS_1514_112_2_18_1_55_38_25 "KMVTEST_VDEC_SEC_FrmProc,in,frame_num=%d,f_in=0x[%x,%x,%x],f_out=0x[%x,%x,%x]"
#define VIDEO_SECURITY_KMV_61_112_2_18_1_55_38_26 "hook_vdec_sec_plug_obj, security_FID=%d, invalid"
#define VIDEO_SECURITY_KMV_65_112_2_18_1_55_38_27 "hook_vdec_sec_plug_obj,ret=%d."
#define VIDEO_SECURITY_KMV_73_112_2_18_1_55_38_28 "hook_vdec_sec_plug_obj,ctx=0x%x."
#define VIDEO_SECURITY_KMV_84_112_2_18_1_55_38_29 "unhook_vdec_sec_plug_obj, security_FID=%d, invalid"
#define VIDEO_SECURITY_KMV_99_112_2_18_1_55_38_30 "VPlayerSecurity_Open."
#define VIDEO_SECURITY_KMV_105_112_2_18_1_55_38_31 "VPlayerSecurity_Open,alloc failed."
#define VIDEO_SECURITY_KMV_120_112_2_18_1_55_38_32 "VPlayerSecurity_Open, Success, VPLAYER_SECURITY_FID_KMVTEST"
#define VIDEO_SECURITY_KMV_124_112_2_18_1_55_39_33 "VPlayerSecurity_Open, failed!"
#define VIDEO_SECURITY_KMV_135_112_2_18_1_55_39_34 "VPlayerSecurity_Close."
#define VIDEO_SECURITY_KMV_154_112_2_18_1_55_39_35 "VPlayerSecurity_GetFID."
#define VIDEO_SECURITY_KMV_176_112_2_18_1_55_39_36 "VPlayerSecurity_GetVDecSecPlugInfo."
#define VIDEO_SECURITY_KMV_186_112_2_18_1_55_39_37 "VPlayerSecurity_GetVDecSecPlugInfo,p_info=0x%x,strm_proc=0x%x,frame_proc=0x%x"
#define VIDEO_SECURITY_KMV_201_112_2_18_1_55_39_38 "VPlayerSecurity_GetThumbnailInfo."
#define VIDEO_SECURITY_KMV_230_112_2_18_1_55_39_39 "VPlayerSecurity_GetThumbnailInfo, invalid FID:%d"
#define VIDEO_SECURITY_KMV_246_112_2_18_1_55_39_40 "VPlayerSecurity_GetThumbnailData.p_buffer=0x%x,len=%d"
#define VIDEO_SECURITY_KMV_276_112_2_18_1_55_39_41 "VPlayerSecurity_GetThumbnailData, invalid FID:%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_VIDEO_SECURITY_KMV_TRC)
TRACE_MSG(KMVTEST_SECURITY_PROCESS_129_112_2_18_1_55_35_0,"KMVTEST_VDEC_SEC_Open,argc=%d,argv=0x%x.")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_134_112_2_18_1_55_35_1,"KMVTEST_VDEC_SEC_Open,alloc failed.")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_146_112_2_18_1_55_35_2,"VPlayerSecurityPlug_Open,file_open failed.")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_155_112_2_18_1_55_35_3,"VPlayerSecurityPlug_Open,init_table failed.")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_170_112_2_18_1_55_35_4,"KMVTEST_VDEC_SEC_Close,handle=0x%x")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_190_112_2_18_1_55_35_5,"KMVTEST_VDEC_SEC_GetInfo.")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_255_112_2_18_1_55_36_6,"KMVTEST_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_INFO, data_len=%d")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_281_112_2_18_1_55_36_7,"KMVTEST_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_DATA,0x%x,%d")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_320_112_2_18_1_55_36_8,"KMVTEST_VDEC_SEC_StrmProc,in=[0x%x,%d],out=0x%x")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_396_112_2_18_1_55_36_9,"vdec_sec_file_open")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_418_112_2_18_1_55_36_10,"vdec_sec_file_open,SFS_CreateFile failed")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_434_112_2_18_1_55_36_11,"vdec_sec_file_open,file_header[4-7]=0x[%x,%x,%x,%x]")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_469_112_2_18_1_55_36_12,"vdec_sec_file_close")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_600_112_2_18_1_55_36_13,"vdec_sec_get_next_STCO_sample,pos_base=0x%x,next_pos=0x%x,entry_off=%d")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_637_112_2_18_1_55_36_14,"vdec_sec_get_next_STSZ_sample,pos_base=0x%x,next_pos=0x%x,entry_off=%d,size=%d")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_672_112_2_18_1_55_36_15,"vdec_sec_get_next_MAP_sample,pos_base=0x%x,next_pos=0x%x,size2read=%d")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_745_112_2_18_1_55_37_16,"vdec_sec_init_table, in")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_759_112_2_18_1_55_37_17,"vdec_sec_init_table, search STCO, box_size==0, data error!")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_791_112_2_18_1_55_37_18,"vdec_sec_init_table, STCO OK")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_816_112_2_18_1_55_37_19,"vdec_sec_init_table, STSZ OK")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_840_112_2_18_1_55_37_20,"vdec_sec_init_table, SECMAP OK")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_848_112_2_18_1_55_37_21,"vdec_sec_init_table, search udta, box_size==0, data error!")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_903_112_2_18_1_55_37_22,"vdec_sec_init_table,return,STCO/STSZ/MAP/thumbnail,is_ok=[%d,%d,%d,%d],pos=[%x,%x,%x,%x]")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_1131_112_2_18_1_55_37_23,"vdec_sec_get_matrix,frame_num=%d,cur_frame_id=%d")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_1148_112_2_18_1_55_37_24,"vdec_sec_get_matrix,map_size=0")
TRACE_MSG(KMVTEST_SECURITY_PROCESS_1514_112_2_18_1_55_38_25,"KMVTEST_VDEC_SEC_FrmProc,in,frame_num=%d,f_in=0x[%x,%x,%x],f_out=0x[%x,%x,%x]")
TRACE_MSG(VIDEO_SECURITY_KMV_61_112_2_18_1_55_38_26,"hook_vdec_sec_plug_obj, security_FID=%d, invalid")
TRACE_MSG(VIDEO_SECURITY_KMV_65_112_2_18_1_55_38_27,"hook_vdec_sec_plug_obj,ret=%d.")
TRACE_MSG(VIDEO_SECURITY_KMV_73_112_2_18_1_55_38_28,"hook_vdec_sec_plug_obj,ctx=0x%x.")
TRACE_MSG(VIDEO_SECURITY_KMV_84_112_2_18_1_55_38_29,"unhook_vdec_sec_plug_obj, security_FID=%d, invalid")
TRACE_MSG(VIDEO_SECURITY_KMV_99_112_2_18_1_55_38_30,"VPlayerSecurity_Open.")
TRACE_MSG(VIDEO_SECURITY_KMV_105_112_2_18_1_55_38_31,"VPlayerSecurity_Open,alloc failed.")
TRACE_MSG(VIDEO_SECURITY_KMV_120_112_2_18_1_55_38_32,"VPlayerSecurity_Open, Success, VPLAYER_SECURITY_FID_KMVTEST")
TRACE_MSG(VIDEO_SECURITY_KMV_124_112_2_18_1_55_39_33,"VPlayerSecurity_Open, failed!")
TRACE_MSG(VIDEO_SECURITY_KMV_135_112_2_18_1_55_39_34,"VPlayerSecurity_Close.")
TRACE_MSG(VIDEO_SECURITY_KMV_154_112_2_18_1_55_39_35,"VPlayerSecurity_GetFID.")
TRACE_MSG(VIDEO_SECURITY_KMV_176_112_2_18_1_55_39_36,"VPlayerSecurity_GetVDecSecPlugInfo.")
TRACE_MSG(VIDEO_SECURITY_KMV_186_112_2_18_1_55_39_37,"VPlayerSecurity_GetVDecSecPlugInfo,p_info=0x%x,strm_proc=0x%x,frame_proc=0x%x")
TRACE_MSG(VIDEO_SECURITY_KMV_201_112_2_18_1_55_39_38,"VPlayerSecurity_GetThumbnailInfo.")
TRACE_MSG(VIDEO_SECURITY_KMV_230_112_2_18_1_55_39_39,"VPlayerSecurity_GetThumbnailInfo, invalid FID:%d")
TRACE_MSG(VIDEO_SECURITY_KMV_246_112_2_18_1_55_39_40,"VPlayerSecurity_GetThumbnailData.p_buffer=0x%x,len=%d")
TRACE_MSG(VIDEO_SECURITY_KMV_276_112_2_18_1_55_39_41,"VPlayerSecurity_GetThumbnailData, invalid FID:%d")
END_TRACE_MAP(MS_REF_VIDEO_SECURITY_KMV_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_VIDEO_SECURITY_KMV_TRC_H_


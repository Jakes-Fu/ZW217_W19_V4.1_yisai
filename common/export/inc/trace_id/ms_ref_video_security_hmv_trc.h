/******************************************************************************
 ** File Name:      ms_ref_video_security_hmv_trc.h                                         *
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
//trace_id:82
#ifndef _MS_REF_VIDEO_SECURITY_HMV_TRC_H_
#define _MS_REF_VIDEO_SECURITY_HMV_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define SPRDHMV_SECURITY_PROCESS_140_112_2_18_1_58_4_0 "SPRDHMV_VDEC_SEC_Open,argc=%d,argv=0x%x."
#define SPRDHMV_SECURITY_PROCESS_145_112_2_18_1_58_4_1 "SPRDHMV_VDEC_SEC_Open,alloc failed."
#define SPRDHMV_SECURITY_PROCESS_157_112_2_18_1_58_4_2 "VPlayerSecurityPlug_Open,file_open failed."
#define SPRDHMV_SECURITY_PROCESS_166_112_2_18_1_58_4_3 "VPlayerSecurityPlug_Open,init_table failed."
#define SPRDHMV_SECURITY_PROCESS_181_112_2_18_1_58_4_4 "SPRDHMV_VDEC_SEC_Close,handle=0x%x"
#define SPRDHMV_SECURITY_PROCESS_201_112_2_18_1_58_4_5 "SPRDHMV_VDEC_SEC_GetInfo."
#define SPRDHMV_SECURITY_PROCESS_266_112_2_18_1_58_4_6 "SPRDHMV_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_INFO, data_len=%d"
#define SPRDHMV_SECURITY_PROCESS_292_112_2_18_1_58_5_7 "SPRDHMV_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_DATA,0x%x,%d"
#define SPRDHMV_SECURITY_PROCESS_316_112_2_18_1_58_5_8 "vdec_sec_file_open"
#define SPRDHMV_SECURITY_PROCESS_338_112_2_18_1_58_5_9 "vdec_sec_file_open,SFS_CreateFile failed"
#define SPRDHMV_SECURITY_PROCESS_354_112_2_18_1_58_5_10 "vdec_sec_file_open,file_header[4-7]=0x[%x,%x,%x,%x]"
#define SPRDHMV_SECURITY_PROCESS_389_112_2_18_1_58_5_11 "vdec_sec_file_close"
#define SPRDHMV_SECURITY_PROCESS_551_112_2_18_1_58_5_12 "vdec_sec_init_table, in"
#define SPRDHMV_SECURITY_PROCESS_563_112_2_18_1_58_5_13 "vdec_sec_init_table, search KEY, box_size==0, data error!"
#define SPRDHMV_SECURITY_PROCESS_574_112_2_18_1_58_5_14 "vdec_sec_init_table, read key body file seek failed"
#define SPRDHMV_SECURITY_PROCESS_588_112_2_18_1_58_5_15 "vdec_sec_init_table, read key body failed"
#define SPRDHMV_SECURITY_PROCESS_602_112_2_18_1_58_5_16 "vdec_sec_init_table, vdec_sec_file_readmp4box error"
#define SPRDHMV_SECURITY_PROCESS_608_112_2_18_1_58_5_17 "vdec_sec_init_table, KEY OK"
#define SPRDHMV_SECURITY_PROCESS_617_112_2_18_1_58_5_18 "vdec_sec_init_table, search udta, box_size==0, data error!"
#define SPRDHMV_SECURITY_PROCESS_670_112_2_18_1_58_5_19 "vdec_sec_init_table,return,thumbnail/key,is_ok=[%d,%d],pos=[%x,%x]"
#define SPRDHMV_SECURITY_PROCESS_702_112_2_18_1_58_5_20 "SPRDHMV_VDEC_SEC_FrmProc,in,frame_num=%d,WxH=[%d, %d]f_in=0x[%x,%x,%x],f_out=0x[%x,%x,%x]"
#define SPRDHMV_SECURITY_PROCESS_1046_112_2_18_1_58_6_21 "vdec_sec_init_table, get q, time=%d"
#define SPRDHMV_SECURITY_PROCESS_1202_112_2_18_1_58_6_22 "SPRDHMV_VDEC_SEC_StrmProc,in=[0x%x,%d],out=0x%x"
#define VIDEO_SECURITY_HMV_74_112_2_18_1_58_7_23 "hook_vdec_sec_plug_obj, security_FID=%d, invalid"
#define VIDEO_SECURITY_HMV_78_112_2_18_1_58_7_24 "hook_vdec_sec_plug_obj,ret=%d."
#define VIDEO_SECURITY_HMV_86_112_2_18_1_58_7_25 "hook_vdec_sec_plug_obj,ctx=0x%x."
#define VIDEO_SECURITY_HMV_97_112_2_18_1_58_7_26 "unhook_vdec_sec_plug_obj, security_FID=%d, invalid"
#define VIDEO_SECURITY_HMV_111_112_2_18_1_58_7_27 "VPlayerSecurity_Open_Hmv."
#define VIDEO_SECURITY_HMV_117_112_2_18_1_58_7_28 "VPlayerSecurity_Open_Hmv,alloc failed."
#define VIDEO_SECURITY_HMV_131_112_2_18_1_58_7_29 "VPlayerSecurity_Open_Hmv, Success, VPLAYER_SECURITY_FID_SPRDHMV"
#define VIDEO_SECURITY_HMV_135_112_2_18_1_58_7_30 "VPlayerSecurity_Open_Hmv, failed!"
#define VIDEO_SECURITY_HMV_146_112_2_18_1_58_7_31 "VPlayerSecurity_Close_Hmv."
#define VIDEO_SECURITY_HMV_165_112_2_18_1_58_7_32 "VPlayerSecurity_GetFID_Hmv."
#define VIDEO_SECURITY_HMV_187_112_2_18_1_58_7_33 "VPlayerSecurity_GetVDecSecPlugInfo_Hmv."
#define VIDEO_SECURITY_HMV_197_112_2_18_1_58_7_34 "VPlayerSecurity_GetVDecSecPlugInfo_Hmv,p_info=0x%x,strm_proc=0x%x,frame_proc=0x%x"
#define VIDEO_SECURITY_HMV_212_112_2_18_1_58_7_35 "VPlayerSecurity_GetThumbnailInfo_Hmv."
#define VIDEO_SECURITY_HMV_241_112_2_18_1_58_7_36 "VPlayerSecurity_GetThumbnailInfo_Hmv, invalid FID:%d"
#define VIDEO_SECURITY_HMV_257_112_2_18_1_58_7_37 "VPlayerSecurity_GetThumbnailData_Hmv.p_buffer=0x%x,len=%d"
#define VIDEO_SECURITY_HMV_287_112_2_18_1_58_7_38 "VPlayerSecurity_GetThumbnailData_Hmv, invalid FID:%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_VIDEO_SECURITY_HMV_TRC)
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_140_112_2_18_1_58_4_0,"SPRDHMV_VDEC_SEC_Open,argc=%d,argv=0x%x.")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_145_112_2_18_1_58_4_1,"SPRDHMV_VDEC_SEC_Open,alloc failed.")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_157_112_2_18_1_58_4_2,"VPlayerSecurityPlug_Open,file_open failed.")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_166_112_2_18_1_58_4_3,"VPlayerSecurityPlug_Open,init_table failed.")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_181_112_2_18_1_58_4_4,"SPRDHMV_VDEC_SEC_Close,handle=0x%x")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_201_112_2_18_1_58_4_5,"SPRDHMV_VDEC_SEC_GetInfo.")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_266_112_2_18_1_58_4_6,"SPRDHMV_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_INFO, data_len=%d")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_292_112_2_18_1_58_5_7,"SPRDHMV_VDEC_SEC_IO_CONTROL,GET_THUMBNAIL_DATA,0x%x,%d")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_316_112_2_18_1_58_5_8,"vdec_sec_file_open")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_338_112_2_18_1_58_5_9,"vdec_sec_file_open,SFS_CreateFile failed")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_354_112_2_18_1_58_5_10,"vdec_sec_file_open,file_header[4-7]=0x[%x,%x,%x,%x]")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_389_112_2_18_1_58_5_11,"vdec_sec_file_close")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_551_112_2_18_1_58_5_12,"vdec_sec_init_table, in")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_563_112_2_18_1_58_5_13,"vdec_sec_init_table, search KEY, box_size==0, data error!")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_574_112_2_18_1_58_5_14,"vdec_sec_init_table, read key body file seek failed")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_588_112_2_18_1_58_5_15,"vdec_sec_init_table, read key body failed")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_602_112_2_18_1_58_5_16,"vdec_sec_init_table, vdec_sec_file_readmp4box error")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_608_112_2_18_1_58_5_17,"vdec_sec_init_table, KEY OK")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_617_112_2_18_1_58_5_18,"vdec_sec_init_table, search udta, box_size==0, data error!")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_670_112_2_18_1_58_5_19,"vdec_sec_init_table,return,thumbnail/key,is_ok=[%d,%d],pos=[%x,%x]")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_702_112_2_18_1_58_5_20,"SPRDHMV_VDEC_SEC_FrmProc,in,frame_num=%d,WxH=[%d, %d]f_in=0x[%x,%x,%x],f_out=0x[%x,%x,%x]")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_1046_112_2_18_1_58_6_21,"vdec_sec_init_table, get q, time=%d")
TRACE_MSG(SPRDHMV_SECURITY_PROCESS_1202_112_2_18_1_58_6_22,"SPRDHMV_VDEC_SEC_StrmProc,in=[0x%x,%d],out=0x%x")
TRACE_MSG(VIDEO_SECURITY_HMV_74_112_2_18_1_58_7_23,"hook_vdec_sec_plug_obj, security_FID=%d, invalid")
TRACE_MSG(VIDEO_SECURITY_HMV_78_112_2_18_1_58_7_24,"hook_vdec_sec_plug_obj,ret=%d.")
TRACE_MSG(VIDEO_SECURITY_HMV_86_112_2_18_1_58_7_25,"hook_vdec_sec_plug_obj,ctx=0x%x.")
TRACE_MSG(VIDEO_SECURITY_HMV_97_112_2_18_1_58_7_26,"unhook_vdec_sec_plug_obj, security_FID=%d, invalid")
TRACE_MSG(VIDEO_SECURITY_HMV_111_112_2_18_1_58_7_27,"VPlayerSecurity_Open_Hmv.")
TRACE_MSG(VIDEO_SECURITY_HMV_117_112_2_18_1_58_7_28,"VPlayerSecurity_Open_Hmv,alloc failed.")
TRACE_MSG(VIDEO_SECURITY_HMV_131_112_2_18_1_58_7_29,"VPlayerSecurity_Open_Hmv, Success, VPLAYER_SECURITY_FID_SPRDHMV")
TRACE_MSG(VIDEO_SECURITY_HMV_135_112_2_18_1_58_7_30,"VPlayerSecurity_Open_Hmv, failed!")
TRACE_MSG(VIDEO_SECURITY_HMV_146_112_2_18_1_58_7_31,"VPlayerSecurity_Close_Hmv.")
TRACE_MSG(VIDEO_SECURITY_HMV_165_112_2_18_1_58_7_32,"VPlayerSecurity_GetFID_Hmv.")
TRACE_MSG(VIDEO_SECURITY_HMV_187_112_2_18_1_58_7_33,"VPlayerSecurity_GetVDecSecPlugInfo_Hmv.")
TRACE_MSG(VIDEO_SECURITY_HMV_197_112_2_18_1_58_7_34,"VPlayerSecurity_GetVDecSecPlugInfo_Hmv,p_info=0x%x,strm_proc=0x%x,frame_proc=0x%x")
TRACE_MSG(VIDEO_SECURITY_HMV_212_112_2_18_1_58_7_35,"VPlayerSecurity_GetThumbnailInfo_Hmv.")
TRACE_MSG(VIDEO_SECURITY_HMV_241_112_2_18_1_58_7_36,"VPlayerSecurity_GetThumbnailInfo_Hmv, invalid FID:%d")
TRACE_MSG(VIDEO_SECURITY_HMV_257_112_2_18_1_58_7_37,"VPlayerSecurity_GetThumbnailData_Hmv.p_buffer=0x%x,len=%d")
TRACE_MSG(VIDEO_SECURITY_HMV_287_112_2_18_1_58_7_38,"VPlayerSecurity_GetThumbnailData_Hmv, invalid FID:%d")
END_TRACE_MAP(MS_REF_VIDEO_SECURITY_HMV_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_VIDEO_SECURITY_HMV_TRC_H_


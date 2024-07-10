/******************************************************************************
 ** File Name:      media_file_demux.h                                        *
 ** Author:                                                                   *
 ** Date:           2012/1/15                                                 *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    file demux class definition.                              * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2012/1/15                       Create.                                   *
 ******************************************************************************/
#ifndef MEDIA_FILE_DEMUX_H
#define MEDIA_FILE_DEMUX_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "sci_types.h" 

// --------Packet type mask. bit[3:0]-------
#define PACKET_FLAG_TYP_MASK          0x0000000F
// normal packet.
#define PACKET_FLAG_TYP_NORMAL        0x0000
// Invalid packet.
#define PACKET_FLAG_TYP_INVALID       0x0001
// The packet doesn't follow the last packet, or is probably broken.
#define PACKET_FLAG_TYP_BROKEN        0x0002

// --------Position mask. bit[7:4]-------
#define PACKET_FLAG_POS_MASK          0x000000F0
// pos is unknow.
#define PACKET_FLAG_POS_UNKNOW        0x0000
// The header packet of one frame.
#define PACKET_FLAG_POS_HEADER        0x0010
// The tail packet of one frame.
#define PACKET_FLAG_POS_TAIL          0x0020

// --------Frame type mask. bit[11:8]-------
#define PACKET_FLAG_FRM_MASK     0x00000F00
// unknow frame.
#define PACKET_FLAG_FRM_UNKNOW   0x0000
// Intra frame.
#define PACKET_FLAG_FRM_I        0x0100
// Inter frame with backward reference only.
#define PACKET_FLAG_FRM_P        0x0200
// Inter frame with backward and forward reference.
#define PACKET_FLAG_FRM_B        0x0300
// For inter frame when you don't know the real type.
#define PACKET_FLAG_FRM_PB       0x0400


//
#define FDEMUX_SEEK_ORIGIN_SET 0
#define FDEMUX_SEEK_ORIGIN_CUR 1
#define FDEMUX_SEEK_ORIGIN_END 2


//
#define MAX_SEGMENTS_IN_PACKET  64


// the return value of 
#define DEMUX_IO_BUF_STATUS_E  0x1  // empty.
#define DEMUX_IO_BUF_STATUS_AE 0x2  // almost empty.
#define DEMUX_IO_BUF_STATUS_AF 0x3  // almost full.
#define DEMUX_IO_BUF_STATUS_F  0x4  // full.


//
typedef void*  DEMUX_IN_HANDLE;
typedef void*  DEMUX_HANDLE;
typedef void*  DEMUX_STRM_HANDLE;


typedef struct
{
    DEMUX_IN_HANDLE p_file;
    //
    int (*pf_file_read)(DEMUX_IN_HANDLE p_in_src, unsigned char *p_buf, uint32 size);
    int (*pf_file_peek)(DEMUX_IN_HANDLE p_in_src, unsigned char **pp_buf, uint32 size);
    //
    int (*pf_file_seek)(DEMUX_IN_HANDLE p_in_src, uint32 offset, uint32 ref_pos);
    //
}DEMUX_IN_SRC_T;


typedef struct
{
    uint32 mem_max_size_total;
    uint32 mem_max_size_onetime;
    uint32 mem_max_size_for_data;   // the max cache buffer size for demux data.
    //
    void *p_mem_owner;
    char (*pf_mem_malloc)(void *p_mem_owner,uint32 property, uint32 size);
    int  (*pf_mem_free)(void *p_mem_owner,char *p_buf);
    //
}DEMUX_MEM_OP_T;


typedef struct
{
    void *p_callback_owner;
    //
    int (*pf_report_callback)(uint32 data_check, uint32 err_code, void *p_err_msg);
    //
}DEMUX_CALLBACK_OP_T;


typedef struct
{
    BOOLEAN is_valid;
    uint32  offset;
}DEMUX_SEGMENT_T;


typedef struct DEMUX_STRM_PACKET_TAG
{
    void   *p_owner;

    // the param need by APP.
    unsigned char   *p_data;
    uint32 data_len;
    // one packet may contain several segments.
    uint32 seg_num;
    DEMUX_SEGMENT_T segments[MAX_SEGMENTS_IN_PACKET];

    // one frame may contain several packets.
    uint32 time_stamp;  // ms.
    uint32 frm_num;
    uint32 is_last_frm;
    uint32 packet_num;
    uint32 is_last_packet;
    //
    uint32 flag;    // packet flag.

    // the count for multi outputs. when send the packet to one receiver,used_cnt+=1;
    // and when free callback, used_cnt-=1.
    uint32 used_cnt;
    // free port. when the receiver has used the packet,free it by callback.
    int (*pf_freeport)(struct DEMUX_STRM_PACKET_TAG *p_packet);

    // other information.
    uint32 distance_to_nextkeyfrm; // Distance(ms) to next key frame. 0xffffffff means unkonw.
    
}DEMUX_STRM_PACKET_T;


typedef struct
{
    void *p_receiver;
    //
    int (*pf_rx_callback)(void *p_receiver, DEMUX_STRM_PACKET_T *p_packet);
    int (*pf_check_rx_buf_status)(void *p_receiver); // return DEMUX_IO_BUF_STATUS_x.
}DEMUX_STRM_RX_SLAVE_T;


typedef struct
{
    void *p_sender;
    //
    int (*pf_tx_callback)(void *p_sender, DEMUX_STRM_PACKET_T *p_packet);
    int (*pf_check_tx_buf_status)(void *p_sender);// return DEMUX_IO_BUF_STATUS_x.
}DEMUX_STRM_TX_SLAVE_T;


typedef struct
{
    uint32 samplerate;
    uint32 channels;
    //
    uint32 is_cbr;
    uint32 avg_bitrate; //bps.
    //
    uint32 bitspersample;
    uint32 frame_period_ms;
    //
    // Bytes used by one compressed frame, depends on bitrate.
    uint32 bytes_per_compressfrm;
    // Number of sampleframes contained in one compressed frame.
    uint32 samplefrms_per_compressfrm;
    //
    uint32 blk_align_size;
}DEMUX_AUDIO_FMT_T;


typedef struct
{
    uint32 width;
    uint32 height;
    //
    uint32 frm_rate;
    uint32 frm_rate_base;
    //
    uint32 is_cbr;
    uint32 avg_bitrate; //bps.
}DEMUX_VIDEO_FMT_T;


typedef union
{
    // according to strm type.
    DEMUX_AUDIO_FMT_T a_fmt;
    DEMUX_VIDEO_FMT_T v_fmt;
}DEMUX_STRM_FMT_U;


typedef struct
{
    // strm/track id.
    uint32 strm_id;         // 0,1,2...
    
    // strm/codec type
    uint32 strm_main_typ;   // "aud " or "vid ", 4c string.
    uint32 strm_sub_typ;    // "mp4a" or "mp4v", 4c string.

    // strm size/time;
    uint32 strm_size;
    uint32 strm_duration_ms;
    // user data.
    uint32 codec_cfg_data_len;
    uint32 userdata_len;

    //
    DEMUX_STRM_FMT_U strm_fmt;
}DEMUX_STRM_INFO_T;


typedef struct DEMUX_STRM_INFO_NODE_TAG
{
    DEMUX_STRM_INFO_T strm_info;
    struct DEMUX_STRM_INFO_NODE_TAG *p_strm_node_pre;
    struct DEMUX_STRM_INFO_NODE_TAG *p_strm_node_next;
}DEMUX_STRM_INFO_NODE_T;

typedef struct
{
    uint32 node_cnt;
    DEMUX_STRM_INFO_NODE_T *p_first;
}DEMUX_STRM_INFO_LIST_T;


typedef struct
{
    char *p_buf;
    uint32 len;
}DEMUX_DATA_BUF_T;


typedef struct
{
    // file type.
    uint32 file_main_typ;
    uint32 file_sub_typ;
    // file size;
    uint32 file_size;
    uint32 media_duration_ms;
    //
    uint32 strm_num;
    uint32 strm_data_interval;   // 0 means AV stream is seperate, 1 means av is interval.
    DEMUX_STRM_INFO_LIST_T strm_info_lst;
    //
    uint32 userdata_len;
    //
}DEMUX_FILE_INFO_T;




typedef struct
{
	// open a demux and check the format.
	DEMUX_HANDLE (*pf_open)(DEMUX_IN_SRC_T        *p_in_src,
	                          DEMUX_MEM_OP_T        *p_mem_op,
	                          DEMUX_CALLBACK_OP_T   *p_callback_op
	                         );
	// close demux.
	int (*pf_close)(DEMUX_HANDLE *p_demux);

    // get the media file information/user data.
    int (*pf_get_file_info)(DEMUX_HANDLE *p_demux, DEMUX_FILE_INFO_T **pp_file_info );
    int (*pf_get_file_udata)(DEMUX_HANDLE *p_demux, DEMUX_DATA_BUF_T *p_des_buf );

    // create/destroy the stream.
    DEMUX_STRM_HANDLE (*pf_create_strm)(DEMUX_HANDLE *p_demux, uint32 strm_id);
    int (*pf_destroy_strm)(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm);

    // add/del the stream outport.
    int (*pf_add_strm_outport)(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm, DEMUX_STRM_RX_SLAVE_T *p_rx );
    int (*pf_del_strm_outport)(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm, DEMUX_STRM_RX_SLAVE_T *p_rx );

    // get stream config/user data.
    int (*pf_get_strm_cfgdata)(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm, DEMUX_DATA_BUF_T *p_des_buf );
    int (*pf_get_strm_udata)(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm, DEMUX_DATA_BUF_T *p_des_buf );

    // file/stream seek, ms.
    int (*pf_file_seek)(DEMUX_HANDLE *p_demux, uint32 new_time);
    int (*pf_strm_seek)(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm, uint32 new_time);

    // get the current time of stream, ms.
    int (*pf_strm_gettime)(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm);

    // demux the packet one by one. for AV invterval file. 
    // packet/frame data will send to output port by callback func.
    int (*pf_demux_file_packet)(DEMUX_HANDLE *p_demux,uint32 packet_num);
    int (*pf_demux_file_period)(DEMUX_HANDLE *p_demux,uint32 period_ms);
    // demux the steam seperately. for AV seperate file.
    int (*pf_demux_strm_packet)(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm, uint32 packet_num);
    int (*pf_demux_strm_period)(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm,uint32 period_ms);

    // IOControl for externtion.
    int (*pf_IOControl)(DEMUX_HANDLE *p_demux, uint32 cmd, void *p_param);

}FILE_DEMUX_ENTRY_T;


typedef struct
{
    // demux type.
    char *p_main_typ;    // e.g, "mp4 ","avi ","flv ", "rmvb"
    char *p_sub_typ;
    // version.
    uint32 main_ver;
    uint32 sub_ver;
    //
    uint32 reserve0;

    // entries.
    FILE_DEMUX_ENTRY_T *p_demux_entry;
    //

}FILE_DEMUX_OBJ_T;


#if 0
// for example.
FILE_DEMUX_OBJ_T s_demux_module_regist_table[]
{
        {"rmvb","",  0,0,  0, PNULL},
        {0},
        {0}
};
#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     							   *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    }
#endif

#endif /*#ifndef MEDIA_FILE_DEMUX_H*/ 

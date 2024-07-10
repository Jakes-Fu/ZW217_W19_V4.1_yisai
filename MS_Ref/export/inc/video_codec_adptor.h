/******************************************************************************
 ** File Name:      video_codec_adptor.h                                      *
 ** Author:                                                                   *
 ** Date:           2012/1/15                                                 *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    video codec adptor class definition.                      * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2012/1/15                       Create.                                   *
 ******************************************************************************/
#ifndef VIDEO_CODEC_ADPTOR_H
#define VIDEO_CODEC_ADPTOR_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "sci_types.h" 




/**---------------------------------------------------------------------------*
 **                         Macro define                                      *
 **---------------------------------------------------------------------------*/
// return value.
#define VCODEC_ADP_RET_SUCCESS      0x00
#define VCODEC_ADP_RET_FRM_RDY      0x01
//
#define VCODEC_ADP_RET_PARAM_ERR    0x11
#define VCODEC_ADP_RET_NO_MEM       0x12
#define VCODEC_ADP_RET_UNSUPPORT    0x13
//
#define VCODEC_ADP_RET_BUSY         0x20
#define VCODEC_ADP_RET_STRM_ERR     0x21
#define VCODEC_ADP_RET_HARDWAR_ERR  0x22
//
#define VCODEC_ADP_RET_UNKNOW_ERR   0xF0

// VOP type.
#define VCODEC_ADP_VOP_UNKON    0x0
#define VCODEC_ADP_VOP_I        0x1
#define VCODEC_ADP_VOP_P        0x2
#define VCODEC_ADP_VOP_B        0x3

// IoControl command.
#define VCODEC_ADP_CMD_SEEK_KEY_FRM     0x10
#define VCODEC_ADP_CMD_DISP_SKIP        0x11
#define VCODEC_ADP_CMD_SCALE_DOWN       0x12


/**---------------------------------------------------------------------------*
 **                         Struct define                                     *
 **---------------------------------------------------------------------------*/
typedef void*  VCODEC_ADP_HANDLE;


// ------------YUV420_3PLANE-----------------------//
// 1. Y plane.
// {
//   y[0][0],y[0][1],...,y[0][w-1];
//   y[1][0],y[1][1],...,y[1][w-1];
//   ...
//   y[h-1][0],y[h-1][1],...y[h-1][w-1];
// }

// 2. U plane.
// {
//   u[0][0],u[0][2],...,u[0][w-2];
//   u[2][0],u[2][2],...,u[2][w-2];
//   ...
//   u[h-2][0],u[h-2][2],...,u[h-2][w-2];
// }

// 3. V plane.
// {
//   v[1][1],v[1][3],...,v[1][w-1];
//   v[3][1],v[3][3],...,v[3][w-1];
//   ...
//   v[h-1][1],v[h-1][1],...,v[h-1][w-1];
// }
// ------------------------------------------------//

// ------------YUV420_2PLANE-----------------------//
// 1. Y plane. same with YUV420_3PLANE.

// 2. UV plane.
// {
//   u[0][0],u[0][2],...,u[0][w-2];
//   v[1][1],v[1][3],...,v[1][w-1];
//   ...
//   u[h-2][0],u[h-2][2],...,u[h-2][w-2];
//   v[h-1][1],v[h-1][1],...,v[h-1][w-1];
// }
// ------------------------------------------------//

// ------------YUV422_3PLANE-----------------------//
// 1. Y plane. same with YUV420_3PLANE.

// 2. U plane.
// {
//   u[0][0],u[0][2],...,u[0][w-2];
//   u[1][0],u[1][2],...,u[1][w-2];
//   ...
//   u[h-1][0],u[h-1][2],...,u[h-1][w-2];
// }

// 3. V plane.
// {
//   v[0][1],v[0][3],...,v[0][w-1];
//   v[1][1],v[1][3],...,v[1][w-1];
//   ...
//   v[h-1][1],v[h-1][1],...,v[h-1][w-1];
// }
// ------------------------------------------------//

// ------------YUV422_2PLANE-----------------------//
// 1. Y plane. same with YUV420_3PLANE.

// 2. UV plane.
// {
//   u[0][0],v[0][1],...,u[0][w-2],v[0][w-1];
//   u[1][0],v[1][1],...,u[1][w-2],v[1][w-1];
//   ...
//   u[h-1][1],v[h-1][1],...,u[h-1][w-2],v[h-1][w-1];
// }
// ------------------------------------------------//

typedef enum
{
    VCODEC_ADP_DATATYP_NONE     = 0x0,      // no data.
    VCODEC_ADP_DATATYP_INVALID  = 0x1,      // invalid data.
    // component.
    VCODEC_ADP_DATATYP_COMPONENT  = 0x10,
    VCODEC_ADP_DATATYP_YUV420_MCU,          // YUV420, MCU.
    VCODEC_ADP_DATATYP_YUV420_2PLANE,       // YUV420, Y plane, UV plane.
    VCODEC_ADP_DATATYP_YUV420_3PLANE,       // YUV420, Y plane, U plane.
    VCODEC_ADPP_DATATYP_YUV422_2PLANE,      // YUV422, Y plane, UV plane.
    VCODEC_ADP_DATATYP_YUV422_3PPLANE,      // YUV422, Y plane, U plane,V plane.
    VCODEC_ADP_DATATYP_RGB565,              // RGB565.
    // compressed.
    VCODEC_ADP_DATATYP_STRM   = 0x20,
    //
    VCODEC_ADP_DATATYP_ERR_INFO,
    //
    VCODEC_ADP_DATATYP_MAX
}VCODEC_ADP_DATATYP_E;


typedef union
{
    struct
    {
        uint8* p_data; // data point for JPEG,MCU,RGB565.
        uint32 len;
    }oneplane;
    
    struct
    {
        uint8 *p_y;
        uint8 *p_uv;
    }yuv2plane; // data point for yuv 2 planes.
    
    struct
    {
        uint8 *p_y;
        uint8 *p_u;
        uint8 *p_v;
    }yuv3plane; // data point for yuv 3 planes.

}VCODEC_ADP_COMPONENT_DATA_U;


typedef struct
{
    uint32 data_type;     // data type.refer to VCODEC_ADP_DATATYP_E.
    //
    uint32 data_w;   // data width and height,general 16pixels aligned.
    uint32 data_h;
    //
    uint32 trim_x;   // start x/y of valid image in data image.
    uint32 trim_y;
    uint32 trim_w;   // valid image width and height(<=data w/h).
    uint32 trim_h;

    // other property of the frame.
    uint32 rotate_angle;    // clowise, 0-360 degree.
    uint32 mirror_type;     // 0: no mirror, 1: Horizontal mirror, 2: vertical mirror.
}VCODEC_ADP_IMG_FMT_T;


typedef struct VCODEC_ADP_COMPONENT_FRAME_TAG
{
    VCODEC_ADP_IMG_FMT_T        img_fmt;
    VCODEC_ADP_COMPONENT_DATA_U img_data;
}VCODEC_ADP_COMP_FRM_T;


typedef struct
{
    void *p_owner;
    int (*pf_callback)(void *p_owner, uint32 msg_id, void *p_msg);
    //
}VCODEC_ADP_CALLBACK_PORT_T;


typedef struct VCODEC_ADP_DATA_PACKET_TAG
{
    uint8* p_data;
    uint32 len;
    //
    uint32 data_type;
    uint32 flag;
    //
    struct VCODEC_ADP_DATA_PACKET_TAG *p_pre;
    struct VCODEC_ADP_DATA_PACKET_TAG *p_next;

}VCODEC_ADP_DATA_PACKET_T;


typedef struct
{
    uint32 list_len;
    VCODEC_ADP_DATA_PACKET_T *p_first;
}VCODEC_ADP_DATA_PACKET_LIST_T;


typedef struct
{
    // one frame may contain several packets.
    VCODEC_ADP_DATA_PACKET_LIST_T packet_list;
    //
    uint32 is_last_one;

    // free port. when the receiver has used the packet,free it by callback.
    VCODEC_ADP_CALLBACK_PORT_T  free_port;
    // other information.
}VCODEC_ADP_STRM_T;


typedef struct
{
    char   codec_name[8];    // e.g. "mp4v","h263","h264"...
    uint32 profile;
    uint32 level;
    //
    uint32 width;
    uint32 height;
    //
    uint32 frm_rate;
    uint32 frm_rate_base;

    //
    VCODEC_ADP_DATA_PACKET_LIST_T cfg_data;

}VCODEC_ADP_STRM_FMT_T;


typedef struct
{
    char   codec_name[8];    // e.g. "mp4v","h263","h264"...
    uint32 profile;
    uint32 level;
    //
    uint32 width;
    uint32 height;
    //
    uint32 vop_type;    // I,P,B frame/slice.
    uint32 flag;
}VCODEC_ADP_FRM_INFO_T;


typedef struct
{
	// create a decoder and check the format.
	VCODEC_ADP_HANDLE (*pf_open)( VCODEC_ADP_STRM_FMT_T       *p_strm_fmt,
	                              VCODEC_ADP_CALLBACK_PORT_T  *p_err_port
	                             );
	// close decoder.
	int (*pf_close)(VCODEC_ADP_HANDLE *p_decoder);

    // set/get the codec format of this stream.
    int (*pf_set_strm_fmt)(VCODEC_ADP_HANDLE *p_decoder,VCODEC_ADP_STRM_FMT_T *p_strm_fmt);
    int (*pf_get_strm_fmt)(VCODEC_ADP_HANDLE *p_decoder,VCODEC_ADP_STRM_FMT_T *p_strm_fmt);

    // parse the stream data.
    int (*pf_get_frm_info)( VCODEC_ADP_HANDLE *p_decoder, 
                            VCODEC_ADP_STRM_T *p_frm,
                            VCODEC_ADP_FRM_INFO_T *p_frm_info
                          );

    // set the output image fmt.
    int (*pf_set_output_fmt)(VCODEC_ADP_HANDLE *p_decoder,VCODEC_ADP_IMG_FMT_T *p_img_fmt);

    // decde the input stream/packet. 
    // if one frame decoded out(ready), get the frame throug "pf_get_frm".
    int (*pf_dec_strm)( VCODEC_ADP_HANDLE *p_decoder, VCODEC_ADP_STRM_T *p_strm);
    // get/free one image frame.
    int (*pf_get_frm) (VCODEC_ADP_HANDLE *p_decoder, VCODEC_ADP_COMP_FRM_T **pp_frm);
    int (*pf_free_frm)(VCODEC_ADP_HANDLE *p_decoder, VCODEC_ADP_COMP_FRM_T *p_frm);

    // IOControl for externding.
    int (*pf_IOControl)(VCODEC_ADP_HANDLE *p_decoder, uint32 cmd, void *p_param);

}VCODEC_ADP_DEC_ENTRY_T;


typedef struct
{
    // codec type.
    char *p_codec_name;    // e.g, "mpeg4 ","h263 ","h264", "rmvb"...
    // version.
    uint32 profile;
    uint32 level;
    uint32 reserve0;

    // entries.
    VCODEC_ADP_DEC_ENTRY_T *p_dec_entry;

}VCODEC_ADP_DEC_OBJ_T;



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     							   *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    }
#endif

#endif /*#ifndef VIDEO_CODEC_ADPTOR_H*/ 

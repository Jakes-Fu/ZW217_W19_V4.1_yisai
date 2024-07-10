/******************************************************************************
 ** File Name   :video_security.h                                             *
 ** Author      :                                                             *
 ** Date        :2011/07/18                                                   *
 ** Copyright   :2011 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description :
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2011/07/18                      Create.                                   *
 ******************************************************************************/
#ifndef VIDEOPLAYER_SECURITY_H
#define VIDEOPLAYER_SECURITY_H

#include "sci_types.h"

typedef void * VPLAYER_SECURITY_HANDLE;
typedef void * VDEC_SECURITY_PLUG_HANDLE;


// FID(high 16bits) and subID(low 16bits).
#define VPLAYER_SECURITY_FID_INVALID (0x00000000 & 0xffff0000)
#define VPLAYER_SECURITY_FID_KMVTEST (0x10000000 & 0xffff0000)
#define VPLAYER_SECURITY_FID_SPRDKMV (0x01000000 & 0xffff0000)
#define VPLAYER_SECURITY_FID_SPRDHMV (0x00100000 & 0xffff0000)
//
#define VPLAYER_SECURITY_SUBID_FIRST (0x00000001 & 0x0000ffff)
// TODO: add other security FID and process functions.

// IO Control command.
#define VPLAYER_SECURITY_IO_CMD_GET_PLUG_INFO           0x0010
//
#define VPLAYER_SECURITY_IO_CMD_GET_THUMBNAIL_INFO      0x0020
#define VPLAYER_SECURITY_IO_CMD_GET_THUMBNAIL_DATA      0x0021


typedef enum
{
    VP_SEC_IMG_FMT_BMP,
    VP_SEC_IMG_FMT_JPG,
    VP_SEC_IMG_FMT_RGB565,
    VP_SEC_IMG_FMT_YUV420_2PLANE,
    VP_SEC_IMG_FMT_YUV420_3PLANE,
    VP_SEC_IMG_FMT_MAX
}VPLAYER_SEC_IMG_FMT_E;

typedef struct
{
    uint32 data_fmt;
    uint32 data_len;
    //
    uint32 w;            // width.
    uint32 h;            // height.
    // other information.
    uint32 argc;
    void *argv;
}VPLAYER_SEC_THUMBNAIL_INFO_T;


typedef struct
{
    uint32 frame_num;       // frame num.
    uint32 packet_num;      // packet num in the frame.
    uint32 start_offset;    // start offset in the frame.
    //
    uint32 strm_len;
    uint8 *p_strm;
    // other information.
    uint32 argc;
    void *argv;
}VIDEO_SECURITY_STREAM_T;


typedef struct
{
    uint32 frame_num;       // frame num.
    uint32 frame_size_mb;   // the mb num in the frame.
    uint32 mb_w;            // width of mb.
    uint32 mb_h;            // height of mb.
    uint32 x_mb;    // mb_offset in the frame at x direction.
    uint32 y_mb;    // mb_offset in the frame at y direction.
    //
    uint32 data_fmt;    //0x420,0x422,0x444,etc.
    uint8 *p_y;
    uint8 *p_u;
    uint8 *p_v;
}VIDEO_SECURITY_MB_T;

typedef struct
{
    uint32 frame_num;       // frame num.
    uint32 w;            // width.
    uint32 h;            // height.
    //
    uint32 data_fmt;    //0x420,0x422,0x444,etc.
    uint8 *p_y;
    uint8 *p_u;
    uint8 *p_v;
}VIDEO_SECURITY_FRAME_T;


typedef struct
{
    VDEC_SECURITY_PLUG_HANDLE vdec_sec_handle;
    // security FID(factory ID) and SUBID, they determin how to use the secrity process.
    uint32 FID;
    uint32 SUBID;

    // stream process.
    int (*pf_strm_proc)(VDEC_SECURITY_PLUG_HANDLE sec_handle,VIDEO_SECURITY_STREAM_T *p_strm_in, VIDEO_SECURITY_STREAM_T *p_strm_out);
    // mb process.
    int (*pf_mb_proc)(VDEC_SECURITY_PLUG_HANDLE sec_handle,VIDEO_SECURITY_MB_T *p_mb_in, VIDEO_SECURITY_MB_T *p_mb_out);
    //  frame process.
    int (*pf_frm_proc)(VDEC_SECURITY_PLUG_HANDLE sec_handle,VIDEO_SECURITY_FRAME_T *p_frame_in,VIDEO_SECURITY_FRAME_T *p_frame_out);

}VDEC_SECURITY_PLUG_INFO_T;



typedef void (*USER_SECKEY_PROCESS_FUN)(
    uint8 *,   /*!<input  src data.*/
    uint8 *,   /*!<output dec  data.*/
    uint16    /*!<length of data.*/
    );

extern USER_SECKEY_PROCESS_FUN mvdec_usersec_process;	/*!<process of express.*/

/*****************************************************************************/
//  Description: reg mv user seckey funcation
//  Author:
//  Input:  user key process funcation
//              
//  Return:  No 
//          
//	Note:           
/*****************************************************************************/

PUBLIC void SPRDMV_UserSecFun_RegCallback(USER_SECKEY_PROCESS_FUN userkey_process);

PUBLIC void SPRDMV_UserSecFun_UnRegCallback(void);


/*****************************************************************************/
//  Description: Open security module
//  Author:
//  Input:   argc=1
//              argv: pointer of file name
//  Return: VPLAYER_SECURITY_HANDLE   successful
//              0    failed
//	Note:           
/*****************************************************************************/
PUBLIC VPLAYER_SECURITY_HANDLE VPlayerSecurity_Open(uint32 argc,void **argv);

/*****************************************************************************/
//  Description: Close security module
//  Author:     
//  Input:  VPLAYER_SECURITY_HANDLE
//  Return: SCI_SUCCESS
//	Note:           
/*****************************************************************************/
PUBLIC int VPlayerSecurity_Close(VPLAYER_SECURITY_HANDLE vplayer_sec_handle);

/*****************************************************************************/
//  Description: Get security FID
//  Author:        
//  Input:  VPLAYER_SECURITY_HANDLE
//  Return: security_FID    successful
//              VPLAYER_SECURITY_FID_INVALID    failed
//	Note:           
/*****************************************************************************/
PUBLIC uint32 VPlayerSecurity_GetFID(VPLAYER_SECURITY_HANDLE vplayer_sec_handle);

/*****************************************************************************/
//  Description: Get security plug info form handle
//  Author:   
//  Input:  VPLAYER_SECURITY_HANDLE
//             address of VDEC_SECURITY_PLUG_INFO_T pointer
//  Return: SCI_SUCCESS
//	Note:  *p_info will point to VDEC_SECURITY_PLUG_INFO_T in VPLAYER_SECURITY_HANDLE
/*****************************************************************************/
PUBLIC int VPlayerSecurity_GetVDecSecPlugInfo(
                                VPLAYER_SECURITY_HANDLE vplayer_sec_handle,
                                VDEC_SECURITY_PLUG_INFO_T **p_info
                                );
                                
/*****************************************************************************/
//  Description: Get thumbnail infomation
//  Author:       
//  Input:  VPLAYER_SECURITY_HANDLE
//              VPLAYER_SEC_THUMBNAIL_INFO_T*
//  Return: SCI_SUCCESS   successful
//              SCI_ERROR   failed
//	Note:           
/*****************************************************************************/
PUBLIC int VPlayerSecurity_GetThumbnailInfo(VPLAYER_SECURITY_HANDLE vplayer_sec_handle,
                                            VPLAYER_SEC_THUMBNAIL_INFO_T *p_thumbnail_info
                                            );
                                            
/*****************************************************************************/
//  Description: Get thumbnal data
//  Author:        
//  Input:  VPLAYER_SECURITY_HANDLE
//              p_buffer: point to the memory storing thumbnail data
//              len: thumbnail data you want
//  Return: SCI_SUCCESS   successful
//              SCI_ERROR   failed
//	Note:           
/*****************************************************************************/
PUBLIC int VPlayerSecurity_GetThumbnailData(VPLAYER_SECURITY_HANDLE vplayer_sec_handle,
                                            uint8 *p_buffer,uint32 len
                                            );




#endif

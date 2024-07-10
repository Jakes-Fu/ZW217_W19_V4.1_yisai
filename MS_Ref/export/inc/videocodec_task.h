/***************************************************************************************
** File Name:      videoCodec_task.h                                         		 			   *
** DATE:           2007/06/05                                                		   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         		   *
** Description:    																       *
****************************************************************************************

****************************************************************************************
**                         Edit History                                       		   *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                               				   *
** 2007/06/05     	       Create.
** 2009/10/10              modify.
****************************************************************************************/

#ifndef _VIDEOCODEC_TASK_H_
#define _VIDEOCODEC_TASK_H_


/**------------------------------------------------------------------------------------*
 **                         Dependencies           			                           *
 **------------------------------------------------------------------------------------*/
#include "os_api.h"
#include "tasks_id.h"
#include "user_tasks_id.h"


/**------------------------------------------------------------------------------------*
 **							Compiler Flag											   *
 **------------------------------------------------------------------------------------*/
#ifdef __cpluslus
extern "C"
{
#endif





/***************************************************************************************************/
/****************************************  ENCODE & DECODE  ****************************************/
/***************************************************************************************************/



/* colorspace values */
typedef enum
{
    /* colorspace YUV */
    CAL_CSP_I420 = 0x001,       /* 12-bit YUV420 three planars */
    CAL_CSP_NV12,               /* 12-bit YUV420 two planars */
    CAL_CSP_NV16,		        /* 16-bit YUV422 two planars */
    CAL_CSP_YV12,	            /* 12-bit YUV420 three planars */
    CAL_CSP_YV16,		        /* 16-bit YUV422 three planars */

    /* colorspace RGB */
    CAL_CSP_RGB  = 0x101,		/* 24-bit    RGB packed */
    
    /* colorspace CMYK */
    CAL_CSP_CMYK = 0x201		/* 32-bit   CMYK planar */
} CAL_CSP;

/* colorspace vflip */
#define CAL_CSP_VFLIP    (1<<31) /* vertical flip mask */



/* CAL_IMAGE_T */
	/* for packed colorspaces use only plane[0] and stride[0] */
	/* for planar colorspaces, if unwanted planar, then the fourth plane reserved for alpha */
typedef struct 
{
	CAL_CSP csp;			/* [in] colorspace */
	void* plane[4];			/* [in] image plane ptrs */
	int stride[4];			/* [in] image stride; "bytes per row"*/
} CAL_IMAGE_T;



/* frame type flags */
typedef enum
{
    CAL_VOP_TYPE_VOL  = -1,     /* decoder only: vol was decoded */
    CAL_VOP_TYPE_AUTO =  0,     /* encoder: automatically determine coding type */
    CAL_VOP_TYPE_IVOP =  1,     /* intra frame */
    CAL_VOP_TYPE_PVOP =  2,     /* predicted frame */
    CAL_VOP_TYPE_BVOP =  3,     /* bidirectionally encoded frame */
    CAL_VOP_TYPE_SVOP =  4     /* predicted+sprite frame */
} CAL_VOP_TYPE;



/* video type */
typedef enum
{
    CAL_VIDEO_TYPE_NOVIDEO = 0, /* Value when coding is N/A */
    CAL_VIDEO_TYPE_MJPEG,       /* Motion JPEG */
    CAL_VIDEO_TYPE_H263,        /* H.263 */
    CAL_VIDEO_TYPE_H263_FLV,  /* FLV_H.263 */
    CAL_VIDEO_TYPE_MPEG4,	    /* MPEG4 Video */
    CAL_VIDEO_TYPE_H264,        /* H.264/AVC */
    CAL_VIDEO_TYPE_WMV,         /* all versions of Windows Media Video */
    CAL_VIDEO_TYPE_RV           /* all versions of Real Video */
} CAL_VIDEO_TYPE;



/* memory for encoder or decoder */
typedef struct 
{
    uint8*  buffer;    		    /* [in] Pointer to buffer used for codec */
    uint32  size;            	    /* [in] Number of bytes of buffer */
} CAL_VIDEO_CODEC_MEMORY;



/* error packets info in video stream. */
#define CAL_STRM_ERR_PKT_NUM_MAX  0x3

typedef struct 
{
    uint16 start_pos;
    uint16 end_pos;
}CAL_STRM_ERR_POS_T;



/******************************************************************************************************************************************/
/****************************************************************  ENCODE  ****************************************************************/
/******************************************************************************************************************************************/



typedef enum
{
    CAL_VIDEO_ENC_OK = 0,
    CAL_VIDEO_ENC_ERROR = -1,
    CAL_VIDEO_ENC_PARAM_ERROR = -2,
    CAL_VIDEO_ENC_MEMORY_ERROR = -3,
    CAL_VIDEO_ENC_INVALID_STATUS = -4,
    CAL_VIDEO_ENC_OUTPUT_BUFFER_OVERFLOW = -5,
    CAL_VIDEO_ENC_HW_ERROR = -6 
} CAL_VIDEO_ENC_RET;



typedef struct 
{
    CAL_VIDEO_TYPE	VideoType;		/* [in] Video Type in CAL_VIDEO_TYPE */

    int32	frame_width;            /* [in] frame width in pixel */
    int32	frame_height;           /* [in] frame height in pixel */
    
    int32	time_scale;
} CAL_VIDEO_ENC_FORMAT;





/* video encoding config parameter */
typedef struct
{
    uint32  FrameRate;              /* [in] target framerate */
    
    uint32	RateCtrlEnable;         /* [in] rate control, 0 : disable  1: enable */
    uint32	BitRate;                /* [in] target bitrate, valid if the RateControl is enabled */
    uint32	QP_Intra;     			/* [in] Intra frame QP; 1 ~ 32, valid if the RateControl is disabled */
    uint32	QP_Inter;     			/* [in] Inter frame QP; 1 ~ 32, valid if the RateControl is disabled */
    //--for rate control in wirless channel.
    uint32	vbv_buf_size;           //bit size,vbv buffer size, to determine the max transfer delay

    uint32 h263En;
    uint32	profileAndLevel;
} CAL_VIDEO_ENC_CONFIG;



typedef struct
{
    CAL_VOP_TYPE vopType;           /* [in] VOP Type in CAL_VOP_TYPE */
    
    CAL_IMAGE_T frame;              /* [in] current frame data */
    uint32 frame_width;             /* [in] frame width in pixel */
    uint32 frame_height;            /* [in] frame height in pixel */
    
    int32 time_stamp;
    //--for dynamic rate control in wirless channel.
    int32   bs_remain_len;				//bit len,remained bitstream length
    int32 	channel_quality;			//0: good, 1: ok, 2: poor
} CAL_VIDEO_ENC_IN;



typedef struct
{
    uint8*      bitstream_buffer;	/* bitstream buffer encoded */
    uint32		bitstream_size;		/* bitstream size encoded */
} CAL_VIDEO_ENC_OUT;






/******************************************************************************************************************************************/
/****************************************************************  DECODE  ****************************************************************/
/******************************************************************************************************************************************/



typedef enum
{
    CAL_VIDEO_DEC_OK = 0,	//解码成功，该帧可以用于显示
    CAL_VIDEO_DEC_ERROR = -1, //解码失败
    CAL_VIDEO_DEC_PARAM_ERROR = -2,
    CAL_VIDEO_DEC_MEMORY_ERROR = -3,
    CAL_VIDEO_DEC_INVALID_STATUS = -4,
    CAL_VIDEO_DEC_STREAM_ERROR = -5,//码流错误，解码失败
    CAL_VIDEO_DEC_OUTPUT_BUFFER_OVERFLOW = -6,
    CAL_VIDEO_DEC_HW_ERROR = -7,
    CAL_VIDEO_DEC_NOT_SUPPORTED = -8,
    CAL_VIDEO_DEC_FRAME_SEEK_IVOP = -9//该帧被丢掉，寻找下个I帧
} CAL_VIDEO_DEC_RET;



typedef struct 
{
	CAL_VIDEO_TYPE	VideoType;		/* [in] Video Type in CAL_VIDEO_TYPE */
	
	int32	frame_width;            /* [in] frame width in pixel */
	int32	frame_height;           /* [in] frame height in pixel */
    
	int32	i_extra;
	void 	*p_extra;
} CAL_VIDEO_DEC_FORMAT;



typedef struct
{
    uint8   *bitstream_buffer;  /* bitstream buffer to be decoded */
    uint32  bitstream_size;     /* bitstream size to be decoded */

    uint32  pts;                /* [in] current frame presentation timestamp */
    int     transpose;          /* [in] current frame need to do transpose or not, 1: need transpose, 0: no need */	
    int32   frameskip;          /* [in] indicate to skip to next IVOP */
    int     framedrop;	        /* [in] current frame need to skip display or not, 1: display skipped, 0: display */
    uint32 frame_num;
    // error packets info in stream.
    int32                  err_pkt_num;
    CAL_STRM_ERR_POS_T     err_pkt_pos[CAL_STRM_ERR_PKT_NUM_MAX];

} CAL_VIDEO_DEC_IN;



typedef struct
{
    CAL_VOP_TYPE voptype;       /* [out] VOP Type in CAL_VOP_TYPE */
    
    CAL_IMAGE_T frame;          /* [out] current frame data */
    uint32 frame_width;         /* [out] frame width in pixel */
    uint32 frame_height;        /* [out] frame height in pixel */
 
    int     is_transposed;      /* [out] current frame is transposed or not, 1: transposed, 0: not transposed */
    uint32  pts;                /* [out] current frame presentation timestamp */
    int32   frameEffective;     /* [out] current frame is effective for display or not, 1: effective for display, 0: not effective  */
    int32   err_MB_num;         /* [out] the error MBs checked in the stream.*/
}CAL_VIDEO_DEC_OUT;






/***************************************************************************************************************************************/
/****************************************************************  API  ****************************************************************/
/***************************************************************************************************************************************/



/************************************************/
// Description:	Create Video Codec Task
// Global resource dependence:
// Author: Jianping.wang
// Note:
/************************************************/
PUBLIC void VideoCodecTask_Init(void);


/************************************************/
// Description:	Destroy Video Codec Task
// Global resource dependence:
// Author: 
// Note:
/************************************************/
PUBLIC void VideoCodecTask_Destroy(void);



/*****************************************************************************/
//  Description: Initialize the video encoder
//	Global resource dependence: 
//  Author: 
//  Parameter:
//      CAL_VIDEO_CODEC_MEMORY *pBuffer: encoder memory
//      CAL_VIDEO_ENC_FORMAT * pformat:  encoder format
//  Return:
//      return 0 if OK, else return others
//	Note:
/*****************************************************************************/
PUBLIC int32 CAL_VideoEncInit(CAL_VIDEO_CODEC_MEMORY *pBuffer, CAL_VIDEO_ENC_FORMAT * pformat);


/*****************************************************************************/
//  Description: Get the video encoder config
//	Global resource dependence: 
//  Author: 
//  Parameter:
//      CAL_VIDEO_ENC_CONFIG *pConfig:  pointers to the video encoder config
//  Return:
//      return 0 if OK, else return others
//	Note: pConfig depend on the CAL_VIDEO_TYPE
/*****************************************************************************/
PUBLIC int32 CAL_VideoEncGetConfig(CAL_VIDEO_ENC_CONFIG *pConfig);


/*****************************************************************************/
//  Description: Set the video encoder config
//	Global resource dependence: 
//  Author: 
//  Parameter:
//      CAL_VIDEO_ENC_CONFIG *pConfig:  pointers to the video encoder config
//  Return:
//      return 0 if OK, else return others
//	Note: pConfig depend on the CAL_VIDEO_TYPE
/*****************************************************************************/
PUBLIC int32 CAL_VideoEncSetConfig(CAL_VIDEO_ENC_CONFIG *pConfig);


/*****************************************************************************/
//  Description: Generate the video stream header
//	Global resource dependence: 
//  Author: 
//  Parameter:
//      CAL_VIDEO_ENC_OUT *pOutput:
//  Return:
//      return 0 if OK, else return others
//	Note:
/*****************************************************************************/
PUBLIC int32 CAL_VideoEncGenHeader(CAL_VIDEO_ENC_OUT *pOutput);


/*****************************************************************************/
//  Description: Encode the video frame to bitstream
//	Global resource dependence: 
//  Author: 
//  Parameter:
//      CAL_VIDEO_ENC_IN *pInput:
//      CAL_VIDEO_ENC_OUT *pOutput:
//  Return:
//      refer to the CAL_VIDEO_ENC_RET
//	Note:
/*****************************************************************************/
PUBLIC int32 CAL_VideoEncEncode(CAL_VIDEO_ENC_IN *pInput, CAL_VIDEO_ENC_OUT *pOutput);


/*****************************************************************************/
//  Description: Release the video encoder
//	Global resource dependence: 
//  Author: 
//  Parameter:  none
//  Return: none
//	Note:
/*****************************************************************************/
PUBLIC void CAL_VideoEncRelease(void);



/*****************************************************************************/
//  Description: Initialize the video decoder
//	Global resource dependence: 
//  Author: 
//  Parameter:
//	  CAL_VIDEO_CODEC_MEMORY *pBuffer
//      CAL_VIDEO_DEC_FORMAT *pFormat:  decoder format
//  Return:
//      return 0 if OK, else return others
//	Note:
/*****************************************************************************/
PUBLIC int32 CAL_VideoDecInit(CAL_VIDEO_CODEC_MEMORY *pBuffer, CAL_VIDEO_DEC_FORMAT *pFormat);


/*****************************************************************************/
//  Description: malloc frame buffer for decoded frame
//	Global resource dependence: 
//  Author: 
//  Parameter:
//      CAL_VIDEO_CODEC_MEMORY *pBuffer
//  Return:
//      return 0 if OK, else return others
//	Note:
/*****************************************************************************/
PUBLIC int32 CAL_VideoDecMallocBuffer(CAL_VIDEO_CODEC_MEMORY *pBuffer);


/*****************************************************************************/
//  Description: Decode the video bitstream to frame and lock the frame for display
//	Global resource dependence: 
//  Author: 
//  Parameter:
//      CAL_VIDEO_DEC_IN *pInput:
//      CAL_VIDEO_DEC_OUT *pFrame:
//  Return:
//      refer to the CAL_VIDEO_DEC_RET
//	Note:
/*****************************************************************************/
PUBLIC int32 CAL_VideoDecDecodeAndLockFrame(CAL_VIDEO_DEC_IN *pInput, CAL_VIDEO_DEC_OUT *pFrame);


/*****************************************************************************/
//  Description: unlock the frame
//	Global resource dependence: 
//  Author: 
//  Parameter:
//      CAL_VIDEO_DEC_OUT *pFrame:
//  Return:
//      refer to the CAL_VIDEO_DEC_RET
//	Note:
/*****************************************************************************/
PUBLIC int32 CAL_VideoDecUnlockFrame(CAL_VIDEO_DEC_OUT *pFrame);


/*****************************************************************************/
//  Description: free frame buffer of decoded frame
//	Global resource dependence: 
//  Author: 
//  Parameter:
//  Return:	none
//	Note:
/*****************************************************************************/
PUBLIC void CAL_VideoDecFreeBuffer(void);

/*****************************************************************************/
//  Description: Release the video decoder
//	Global resource dependence: 
//  Author: 
//  Parameter:  none
//  Return: none
//	Note:
/*****************************************************************************/
PUBLIC void CAL_VideoDecRelease(void);





/**------------------------------------------------------------------------------------*
 **								Compiler Flag										   *
 **------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


#endif //_VIDEOCODEC_TASK_H_

//end of videoCodec_task.h


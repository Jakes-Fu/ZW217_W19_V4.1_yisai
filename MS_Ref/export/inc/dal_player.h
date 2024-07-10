/******************************************************************************
 ** File Name:      digitalvideo.h                                            *
 ** Author:        					                                          *
 ** Date:           2007/02/05                                                 *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    define spreadtrum multimedia player/recorder interface    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/

#ifndef _DAL_PLAYER_H_
#define _DAL_PLAYER_H_

#include "sci_types.h"
#include "os_api.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
/*if use mplayer or mrecorder whose file system interface is FFS, 
   please do not define this macor*/
/*this macor is defined in mcp project file*/
//#define _DV_FILESYSTEM_SFS

#define DPLAYER_NORMAL_HANDLE								(1)
#define DPLAYER_GENERAL_ERROR_HANDLE						(0)
#define DPLAYER_MEM_MALLOC_ERROR_HANDLE						(-1)
#define DPLAYER_PARAMATER_ERROR_HANDLE						(-2)
#define DPLAYER_OPEN_FILE_ERROR_HANDLE						(-3)
#define DPLAYER_BITSTREAM_ERROR_HANDLE						(-4)
#define DPLAYER_NO_VIDEO_TRACK_ERROR_HANDLE					(-5)
#define DPLAYER_DECAME_OPEN_ERROR_HANDLE					(-6)


// the command for DPLAYER_ExternalPlugControl.
#define DPLAYER_EXTPLUG_CTL_ADD_DEC_SECURITY    (0x0001)
#define DPLAYER_EXTPLUG_CTL_DEL_DEC_SECURITY    (0x0002)

/**---------------------------------------------------------------------------*
 **                       Common  Type Declaration                            *
 **---------------------------------------------------------------------------*/
typedef enum
{
    DPLAYER_FILETYPE_3GP = 0,
    DPLAYER_FILETYPE_MP4,
    DPLAYER_FILETYPE_AVI,
    DPLAYER_FILETYPE_FLV,
    DPLAYER_FILETYPE_RMVB,
    DPLAYER_FILETYPE_MAX
}DPLAYER_FILE_TYPE_E;


typedef  int32	DPLAYER_HANDLE;

typedef enum
{
	AUDIO_TYPE_AMR = 0,
	AUDIO_TYPE_AAC,
	AUDIO_TYPE_PCM,
	AUDIO_TYPE_IMAADPCM,
	AUDIO_TYPE_MP3,
	AUDIO_TYPE_DRA,
	AUDIO_TYPE_RA,
	AUDIO_TYPE_MAX
}MEDIA_AUDIO_TYPE_E;

typedef enum
{
    DPLAYER_AUD_DEV_NONE = 0,   // discard the audio,mute.
    DPLAYER_AUD_DEV_SPEAKER,    // send to speaker or ear.
    DPLAYER_AUD_DEV_BT,         // send to blue tooth.
    DPLAYER_AUD_DEV_NET,        // send to NET.
    DPLAYER_AUD_DEV_FILE,       // send to file.
    DPLAYER_AUD_DEV_MAX         // invalid.
}DPLAYER_AUD_DEV_E;

typedef struct
{
    BOOLEAN b_a2dp_support;
    uint8   res0;
    uint16  res1;
}DPLAYER_AUD_DEV_BT_T;

typedef union
{
    DPLAYER_AUD_DEV_BT_T bt_info;
}DPLAYER_AUD_DEV_INFO_U;

typedef enum
{
	VIDEO_TYPE_H263 = 0,
	VIDEO_TYPE_MP4V,
	VIDEO_TYPE_MJPEG,
	VIDEO_TYPE_H264,
	VIDEO_TYPE_RV,
	VIDEO_TYPE_MAX
}MEDIA_VIDEO_TYPE_E;

typedef struct
{
    uint32              audio_support;  //0 refer to no audio or unsupport,valid value:0,1
    MEDIA_AUDIO_TYPE_E  audio_type;
    uint32              channels;       //audio channel,valid value:1,2
    uint32              sample_rate;    //8000Hz for AMR
    uint32              bitrate;        //bps.
}MEDIA_AUDIO_INFO_T;


typedef struct
{
    uint32              video_support;  //0 refer to no video or unsupport,valid value:0,1
    MEDIA_VIDEO_TYPE_E  video_type;
    uint32              width;          //(96-1024)This value must be multiple of 16
    uint32              height;         //(96-768)This value must be multiple of 16	
    uint32              frame_rate;     //frame_rate div frame_rate_base(if !=0) will be real frame_rate.
    uint32              frame_rate_base;
    uint32              bitrate;        //bps.
}MEDIA_VIDEO_INFO_T;


typedef struct
{
    uint32                  media_length;   //media length(milli-second)
    DPLAYER_FILE_TYPE_E     file_type;
    MEDIA_VIDEO_INFO_T      video_info;
    MEDIA_AUDIO_INFO_T      audio_info;
    uint32                  stream_bitrate; //bps. bitrate of all the tracks.
   	int                     b_is_seekable;      /*incicate if the movie is seekable, default not seekable*/
}DPLAYER_MEDIA_INFO_T;


typedef struct
{
    DPLAYER_AUD_DEV_E       output_device;
    DPLAYER_AUD_DEV_INFO_U  output_device_info;
}DPLAYER_AUDIO_PARAM_T;


typedef enum
{
	DPLAYER_DISP_FULLVIEW=0x00, // remain rate of width/height , no trim to fill target_rect,maybe not full
	DPLAYER_DISP_FULLSCREEN,    // remain rate of width/height,  trim to fill target_rect fullly
	DPLAYER_DISP_CUSTOMIZE,     // the rate of w/h maybe varied, no trim to to fill target_rect fully
	DPLAYER_DISP_MODE_MAX
}DPLAYER_DISP_MODE_E;

typedef struct
{
    uint32               x;
    uint32               y;	
    uint32               dx;
    uint32               dy;
}DPLAYER_DISP_RECT_T;

typedef enum
{
    DPLAYER_DISP_DATA_OP_NONE = 0x0,
    DPLAYER_DISP_DATA_GET,
    DPLAYER_DISP_DATA_OP_MAX
}DPLAYER_DISP_DATA_OPERATION_E;

typedef struct
{	
    DPLAYER_DISP_RECT_T disp_rect;      //image & osd display area
    DPLAYER_DISP_RECT_T target_rect;    //image display area
    DPLAYER_DISP_MODE_E disp_mode;	                                
    uint32  RotateAngle;        // 0：0o； 1：90o； 2：180o； 3：270o；
    uint32  audioSupport;       // 1:support audio;0:not support audio
    uint32  videoSupport;       // 1:support video;0:not support video
    uint32  LcdId;              //LCD ID；如果是TV_ID，则表示要tv-out。
    uint32  LcdBlkId;           //LCD block ID
    int32   b_support_seek;
    DPLAYER_DISP_DATA_OPERATION_E  disp_data_op;
    uint32  b_no_osd;    // 1:needn't osd;  0:need osd
}DPLAYER_DISPLAY_PARAM_T;


typedef enum
{
	DPLAYER_SUCCESS = SCI_SUCCESS,
	DPLAYER_ERROR,
 	DPLAYER_ERROR_FILE_NOT_FOUND  			= -1,	/* file to be opened not found */
 	DPLAYER_ERROR_INVALID_PARAMETER			= -2,	/* invalid value to set for parameter */
	DPLAYER_ERROR_MEMORY_ALLOCATION			= -3,	/* memory allocation failed */
	DPLAYER_ERROR_AUDIO_PLAYBACK			= -4,	/* error in audio playback / initialization */
	DPLAYER_ERROR_READING_DATA 				= -6,	/* error reading video / audio frames from stream */
	DPLAYER_BUFFERS_EMPTY					= -7,	/* streaming buffers empty */
	DPLAYER_BUFFERS_READY					= -8,	/* streaming buffers ready */
	DPLAYER_ERROR_DECODING_VIDEO			= -9,	/* error in video decoding */
	DPLAYER_END_OF_STREAM					= -10,	/* end of stream reached */
	DPLAYER_ERROR_UNSUPPORTED_FORMAT		= -11,	/* no supported audio/video formats in file */
	DPLAYER_ERROR_INVALID_COMMAND			= -12,	/* not ready for requested operation in current state */
	DPLAYER_ERROR_UNSUPPORTED_IMAGE_SIZE	= -13,	/* image size unsupported */
	DPLAYER_ERROR_NETWORK					= -14,	/* error in streaming */
	DPLAYER_UNAVAILABLE						= -15,	/* mediainfo currently not available */
	DPLAYER_ERROR_BITSTREAM					= -16,  /* bitstream is error or file is corrupted*/
	DPLAYER_ERROR_NO_VIDEO_TRACK			= -17   /* file not include any video track*/
}DPLAYER_RETURN_E;

typedef enum
{
	DPLAYER_OPEN_FROM_FILE = 0,
	DPLAYER_OPEN_FROM_MEM,
	DPLAYER_OPEN_FROM_NET,
	DPLAYER_OPEN_FROM_MAX
}DPLAYER_OPEN_TYPE_E;

// when open from file, argv[0] point to file_name, (uint32)argv[1] means the access_mode.
typedef enum
{
	DPLAYER_FA_MOD_RB = 0,   // The file is readonly.
	DPLAYER_FA_MOD_ABPLUSE,  // The file may be read and written at the tail.
	DPLAYER_FA_MOD_MAX
}DPLAYER_FILE_ACCESS_MODE_E;


typedef struct
{
	void 		*p_handle;
	uint32		frame_num;
	uint8		data_type;
	uint8 *		buffer_ptr;
	uint32 *	buffersize_ptr;	//[In] buffer size; [out] frame size
	int i_width;
    int i_height;
} DPLAYER_FRAME_DATA_T;


typedef enum
{
    DPLAYER_IMG_FMT_NONE = 0,       // means the data is invalid.
    DPLAYER_IMG_FMT_JPG,            // JPEG.
    DPLAYER_IMG_FMT_BMP,
    DPLAYER_IMG_FMT_YUV420_MCU,
    DPLAYER_IMG_FMT_YUV420_2PLANE,  // YUV420, Y plane, UV plane.
    DPLAYER_IMG_FMT_YUV420_3PLANE,  // YUV420, Y plane, U plane,V plane.
    DPLAYER_IMG_FMT_YUV422_2PLANE,  // YUV422, Y plane, UV plane.
    DPLAYER_IMG_FMT_YUV422_3PLANE,  // YUV422, Y plane, U plane,V plane.
    DPLAYER_IMG_FMT_RGB565,         // RGB565(16bits).
    DPLAYER_IMG_FMT_ARGB888,        // ARGB888(24bits).
    DPLAYER_IMG_FMT_MAX
}DPLAYER_IMAGE_FORMAT_E;


typedef struct
{
    uint32  w;
    uint32  h;
    uint32  trim_x;
    uint32  trim_y;
    uint32  trim_w;
    uint32  trim_h;
    //
    uint32  rotate_angle;   // closewise, 0/1/2/3 means 0/90/180/270 degree.
}DPLAYER_IMAGE_COORDINATE_T;

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
    }yuv2plane;         // data point for yuv 2 pannels.
    
    struct
    {
        uint8 *p_y;
        uint8 *p_u;
        uint8 *p_v;
    }yuv3plane;         // data point for yuv 3 pannels.

}DPLAYER_IMAGE_BUFFER_U;


typedef struct
{
    DPLAYER_IMAGE_FORMAT_E      data_fmt;
    DPLAYER_IMAGE_COORDINATE_T  coordinate;
    DPLAYER_IMAGE_BUFFER_U      buf;
    //
    void    *owner_ptr;
}DPLAYER_IMAGE_DATA_T;


typedef  enum
{
	DPLAYER_PLAY_RATE_NORMAL = 0,
	DPLAYER_PLAY_RATE_X8,
	DPLAYER_PLAY_RATE_X4,
	DPLAYER_PLAY_RATE_X2,
	DPLAYER_PLAY_RATE_DIV2,
	DPLAYER_PLAY_RATE_DIV4,
	DPLAYER_PLAY_RATE_DIV8,
	DPLAYER_PLAY_RATE_BACKWARD_X8,
	DPLAYER_PLAY_RATE_BACKWARD_X4,
	DPLAYER_PLAY_RATE_BACKWARD_X2,
	DPLAYER_PLAY_RATE_BACKWARD_DIV2,
	DPLAYER_PLAY_RATE_BACKWARD_DIV4,
	DPLAYER_PLAY_RATE_BACKWARD_DIV8,
	DPLAYER_PLAY_RATE_MAX
} DPLAYER_PLAY_RATE_E;


typedef enum
{
	DPLY_STRM_POS_TIME = 0,			/* stream position in time */
	DPLY_STRM_POS_BYTE,				/* stream position in byte */
	DPLY_STRM_POS_TYPE_MAX
} DPLAYER_STRM_POS_TYPE_E;


typedef struct
{
	uint16	*audio_codec_type_ptr;		/* audio codec type */
	uint8	*cfg_ptr;					/* audio config pointer */
	uint32	cfg_len;						/* audio config length */
	uint32	duration;					/* audio duration in ms */
	uint32	sample_rate;				/* audio sample rate */
	uint32	channel_num;				/* audio channel number */
} DPLAYER_STRM_AUD_CFG_T;

typedef struct
{
	uint16	*video_codec_type_ptr;		/* video codec type */
	uint8	*cfg_ptr;					/* video config pointer */
	uint32	cfg_len;						/* video config length */
	uint8	*cfg_enhance_ptr;			/* video enhancement layer config pointer for H.264 */
	uint32	cfg_enhance_len;			/* video enhancement layer config length for H.264 */
	uint32	nal_header_length_size;		/* nal header length for H.264 */
	uint32	duration;					/* video duration */
	uint32	sample_rate;				/* video sample rate */

	int 		frame_width;				/* video frame width in pixel */
	int		frame_height;				/* video frame height in pixel */

	uint32 	frame_rate;					/* video framerate numerator */
	uint32	frame_rate_base;			/* video framerate denominator */
} DPLAYER_STRM_VID_CFG_T;

typedef struct
{
	uint32 						media_length;			/* media duration in ms */
	
	DPLAYER_FILE_TYPE_E 			file_type;

	DPLAYER_STRM_AUD_CFG_T 	audio_cfg;
	DPLAYER_STRM_VID_CFG_T		video_cfg;
} DPLAYER_STRM_CFG_T;


typedef struct
{
	BOOLEAN						is_pos_valid;
	DPLAYER_STRM_POS_TYPE_E	pos_type;
	uint32						pos_low32;
	uint32						pos_up32;
} DPLAYER_STRM_POS_T;

typedef enum
{
	DPLY_STRM_AUD_FRAME,				/* audio frame */
	DPLY_STRM_VID_FRAME,				/* video frame */
	DPLY_STRM_FILE_SCICE,
	DPLY_STRM_DATA_TYPE_MAX
} DPLAYER_STRM_DATA_TYPE_E;


typedef struct
{
	DPLAYER_STRM_DATA_TYPE_E 	data_type;
	BOOLEAN						is_pre_data_lost;
	BOOLEAN						is_end_of_stream;
	BOOLEAN						is_key_frame;
	uint8						channel_id;			/* for audio: 0 - mono, 1 - left channel of stereo, 2 - right channel of stereo */

	DPLAYER_STRM_POS_T			data_pos;

	uint8						*data_ptr;
	uint32						data_len;
} DPLAYER_STRM_DATA_T;


typedef enum
{
	PLAYBACK_MODE_SYNC = 0,			/* AV sync */
	PLAYBACK_MODE_RATE				/*video priority*/
} DPLAYER_PLAYBACK_MODE_E;


typedef DPLAYER_RETURN_E (*DPLAYER_STRM_GETFRAME_PTR)(DPLAYER_HANDLE player_handle, DPLAYER_STRM_DATA_TYPE_E data_type, DPLAYER_STRM_DATA_T *frame_data_ptr);
typedef void (*DPLAYER_STRM_FREEFRAME_PTR)(DPLAYER_HANDLE player_handle, DPLAYER_STRM_DATA_T *frame_data_ptr);


typedef DPLAYER_RETURN_E (*PF_FRM_RDY_CALLBACK)(DPLAYER_IMAGE_DATA_T *src_frame_ptr);



/******************************************************************************
 *   player interface
 ******************************************************************************/

/******************************************************************************/
// Description: Init DPlayer
// Author:
// Input:      void
// Output:     none
// Return:     DPLAYER_RETURN_E
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Init(void);

/******************************************************************************/
// Description: Release DPlayer
// Author:
// Input:      void                        
// Output:     none
// Return:     DPLAYER_RETURN_E
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Release(void);

/******************************************************************************/
// Description: open a movie file, prepare it to play 
// Author:
// Input:     
//                          
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_HANDLE  DPLAYER_Open(
	DPLAYER_OPEN_TYPE_E   	open_type,
	int32 				argc, 
	void *				argv[]);

/******************************************************************************/
// Description: stop the player
// Author:
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Close(DPLAYER_HANDLE	player_handle);

/******************************************************************************/
// Description: play an interval
// Author:
// Input:      
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Play(DPLAYER_HANDLE	player_handle);

/******************************************************************************/
// Description: stop
// Author:
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Stop(DPLAYER_HANDLE	player_handle);

/******************************************************************************/
// Description: pause
// Author:
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E  DPLAYER_Pause(DPLAYER_HANDLE	player_handle);
DPLAYER_RETURN_E  DPLAYER_PauseEx(DPLAYER_HANDLE	player_handle);

/******************************************************************************/
// Description: pause
// Author:
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Resume(DPLAYER_HANDLE	player_handle);
DPLAYER_RETURN_E DPLAYER_ResumeEx(DPLAYER_HANDLE	player_handle);

/******************************************************************************/
// Description: get the length (duration)
// Author:
// Input:    
// Output:     none
// Return:     <0         failed
//             others     total length of the movie
// Note:       none
/******************************************************************************/
uint32 DPLAYER_GetLength(DPLAYER_HANDLE	player_handle);

/******************************************************************************/
// Description: set next time to play
// Author:
// Input:      i_time     next play point, milli-second
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_SetTime(
 					DPLAYER_HANDLE	player_handle,uint32 i_time);


/******************************************************************************/
// Description: get current playing time
// Author:
// Input:      
// Output:     none
// Return:     <0         failed
//             others     total length of the movie
// Note:       none
/******************************************************************************/
uint32  DPLAYER_GetTime(DPLAYER_HANDLE	player_handle);

/******************************************************************************/
// Description: set play rate
// Author:
// Input:      none
// Output:     none
// Return:     <0         failed
//             others     total length of the movie
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_SetPlayRate(DPLAYER_HANDLE		player_handle,
		DPLAYER_PLAY_RATE_E	rate);

/******************************************************************************/
// Description: update current frame
// Author:
// Input:      none
// Output:     none
// Return:     <0         failed
//             others     total length of the movie
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_UpdateCurrentFrame(DPLAYER_HANDLE	player_handle);

/******************************************************************************/
// Description: update next frame
// Author:
// Input:      
// Output:     none
// Return:     <0         failed
//             others     total length of the movie
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_UpdateNextFrame(DPLAYER_HANDLE	player_handle);


/******************************************************************************/
// Description: get movie info
// Author:     
// Input:      p_info      point to movie information structure
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_GetMovieInfo(DPLAYER_HANDLE	player_handle,
	DPLAYER_MEDIA_INFO_T *p_info);

/******************************************************************************/
// Description: register display notify function
// Author:     
// Input:      pf_notify   point to notify function
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_SetFrameEndNotify(DPLAYER_HANDLE	player_handle,
	void (*pf_notify)(int32 frame_num));

/******************************************************************************/
// Description: register display end notify function
// Author:     
// Input:      pf_notify   point to notify function
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_SetEndNotify(DPLAYER_HANDLE	player_handle,
	void (*pf_notify)(int32 i_type));



/******************************************************************************/
// Description: GET notify function after showing a frame
// Author:     Zhemin.Lin
// Input:      pf_notify   point to notify function
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_GetFrameEndNotify(
								DPLAYER_HANDLE	player_handle,
								void (**pf_notify)(int32 frame_num));
/******************************************************************************/
// Description: set display parameter
// Author:     
// Input:      display_param_t *pParam pointer to display parameter strcture
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_SetDisplayParam(DPLAYER_HANDLE			player_handle,
	DPLAYER_DISPLAY_PARAM_T const *param_ptr);


// Description: get current frame data for display or other application.
// Input      : 1. dest_frame_ptr, the destin frame offered by APP.
//                 now it's null.
//              2. pf_frm_rdy_callback, if =NULL, it means sync process,otherwise
//                 async process. Now only support sync process.
// Output     : 1. src_frame_ptr, the src frame output from lowlevel.
// Return     : 
// Note       : when dest_frame_ptr is null, the buffer of src_frame_ptr will 
//              allocate by lowlevel, otherwise will use the buffer in 
//              dest_frame_ptr.
/******************************************************************************/
PUBLIC DPLAYER_RETURN_E DPLAYER_GetCurrentFrameData(
					DPLAYER_HANDLE			 player_handle,
                    DPLAYER_IMAGE_DATA_T     *des_frame_ptr,
					DPLAYER_IMAGE_DATA_T     *src_frame_ptr,
					PF_FRM_RDY_CALLBACK      pf_frm_rdy_callback
					);


/******************************************************************************/
// Description: get next frame data for display or other application.
// Input      : 1. dest_frame_ptr, the destin frame offered by APP.
//                 now it's null.
// Output     : 1. src_frame_ptr, the src frame output from lowlevel.
//              2. pf_frm_rdy_callback, if =NULL, it means sync process,otherwise
//                 async process.
// Return     : 
// Note       : when dest_frame_ptr is null, the buffer of src_frame_ptr will 
//              allocate by lowlevel, otherwise will use the buffer in 
//              dest_frame_ptr.
/******************************************************************************/
PUBLIC DPLAYER_RETURN_E DPLAYER_GetNextFrameData(
                    DPLAYER_HANDLE              player_handle,
                    DPLAYER_IMAGE_DATA_T        *des_frame_ptr,
                    DPLAYER_IMAGE_DATA_T        *src_frame_ptr,
                    PF_FRM_RDY_CALLBACK         pf_frm_rdy_callback
                    );


/******************************************************************************/
// Description: Force Audio/Video Sync
// Author:     
// Input:      support_skip 0,not support; 1,support
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_ForceAVSync(DPLAYER_HANDLE	player_handle,
	BOOLEAN		support_skip );


/******************************************************************************/
// Description: Set video playback mode
// Author:     
// Input:      DPLAYER_HANDLE  DPLAYER_PLAYBACK_MODE_E
// Output:     none
// Return:     DPLAYER_SUCCESS       ok
//             other                 fail
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_SetPlaybackMode(
				 		DPLAYER_HANDLE	player_handle,
						DPLAYER_PLAYBACK_MODE_E	playback_mode);

/******************************************************************************/
// Description: Register data path function
// Author:     
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_StreamRegistRxDataPath(DPLAYER_HANDLE player_handle, DPLAYER_STRM_GETFRAME_PTR pf_getframe_ptr, DPLAYER_STRM_FREEFRAME_PTR pf_freeframe_ptr);


/******************************************************************************/
// Description: start streaming
// Author:     
// Input:      start_play_time in ms
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_StreamRxStart(DPLAYER_HANDLE player_handle, uint32 start_play_time);


/******************************************************************************/
// Description: stop streaming
// Author:     
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_StreamRxStop(DPLAYER_HANDLE player_handle);

/******************************************************************************/
// Description: set audio parameter
// Author:     
// Input:      
// Output:     none
// Return:     
//             other: error
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_SetAudioParam(
                                        DPLAYER_HANDLE  player_handle,
                                        DPLAYER_AUDIO_PARAM_T const *param_ptr);

                                        

/******************************************************************************/
// Description  : external plug control, e.g, add/del one external plugger.
// Author       :
// Input        :
// Output       : none.
// Return       : DPLAYER_SUCCESS or error.
// Note         : none.
/******************************************************************************/
PUBLIC DPLAYER_RETURN_E DPLAYER_ExternalPlugControl(
                    DPLAYER_HANDLE  player_handle,
                    uint32          plug_command,
                    uint32          argc,
                    void            *argv[]
                    );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /*_DAL_DV_SC6800AD_H_*/


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

#ifndef _DAL_RECORDER_H_
#define _DAL_RECORDER_H_
#include "sci_types.h"
#include "dal_player.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

typedef enum
{
    DRECORDER_OP_DV = 0,
    DRECORDER_OP_ATV,
    DRECORDER_OP_MINIDV,
    DRECORDER_OP_MAX
}DRECORDER_OP_MODE_E;

typedef enum
{
	DRECORDER_SUCCESS = SCI_SUCCESS,
	DRECORDER_ERROR,
	DRECORDER_ERROR_FILE_OPENING				= -1,	/* file to be opened not found */
	DRECORDER_ERROR_INVALID_PARAMETER			= -2,	/* invalid value to set for parameter */
	DRECORDER_ERROR_MEMORY_ALLOCATION			= -3,	/* memory allocation failed */
	DRECORDER_WRITING_DATA_ERROR				= -4,	/* error reading video / audio frames from stream */
	DRECORDER_RECORDING_AUDIO_ERROR				= -5,	/* error in audio playback */
	DRECORDER_ERROR_ENCODING_VIDEO				= -6,	/* error in video decoding */
	DRECORDER_END_OF_STREAM						= -7,	/* end of stream reached */
	DRECORDER_UNSUPPORTED_FORMAT_ERROR			= -8,	/* no supported audio/video formats in file */
	DRECORDER_INVALID_COMMAND_ERROR				= -9,	/* not ready for requested operation in current state */
	DRECORDER_ERROR_ENGINE_INITIAL              = -10,	/* error in camera */
	DRECORDER_ERROR_LIMIT_FILE_SIZE				= -11	/* error in limit file size */
}DRECORDER_RETURN_E;

typedef enum
{
	DRECORDER_FILETYPE_3GP = 0,
	DRECORDER_FILETYPE_MP4,
	DRECORDER_FILETYPE_AVI,
	DRECORDER_FILETYPE_MAX
}DRECORDER_FILE_TYPE_E; 

typedef enum
{
    DRECORDER_VIDEO_DATATYPE_JPEG = 0,
    DRECORDER_VIDEO_DATATYPE_YUV420,
    DRECORDER_VIDEO_DATATYPE_YUV422,
    DRECORDER_VIDEO_DATATYPE_RAWRGB,
    DRECORDER_VIDEO_DATATYPE_MAX
}DRECORDER_VIDEO_DATATYPE_E;

typedef enum
{
	DRECORDER_VIDEO_QUALITY_LOW = 0,
	DRECORDER_VIDEO_QUALITY_NORMAL,
	DRECORDER_VIDEO_QUALITY_HIGH,
	DRECORDER_VIDEO_QUALITY_MAX
}DRECORDER_PARAM_VIDEO_QUALITY_E;

typedef enum
{
	DRECORDER_AUDIO_QUALITY_LOW = 0,
	DRECORDER_AUDIO_QUALITY_NORMAL,
	DRECORDER_AUDIO_QUALITY_HIGH,
	DRECORDER_AUDIO_QUALITY_MAX
}DRECORDER_PARAM_AUDIO_QUALITY_E;


// control for CAMERA/sensor.
typedef enum
{
	DRECORDER_EFFECT_NORMAL = 0,
	DRECORDER_EFFECT_BLACKWHITE,
	DRECORDER_EFFECT_RED,
	DRECORDER_EFFECT_GREEN,
	DRECORDER_EFFECT_BLUE,
	DRECORDER_EFFECT_YELLOW,
	DRECORDER_EFFECT_NEGATIVE,		// 底片
	DRECORDER_EFFECT_CANVAS,		// 帆布
	DRECORDER_EFFECT_RELIEVOS,		// 浮雕	
	DRECORDER_EFFECT_MAX
}DRECORDER_PARAM_EFFECT_E;

typedef enum
{
	DRECORDER_ENVIROMENT_NORMAL = 0,
	DRECORDER_ENVIROMENT_NIGHT,
	DRECORDER_ENVIROMENT_SUNNY,
	DRECORDER_ENVIROMENT_MAX
}DRECORDER_PARAM_ENVIROMENT_E;

typedef enum
{
	DRECORDER_WB_MODE_AUTO = 0,			//自动
	DRECORDER_WB_MODE_INCANDESCENCE,		//白炽灯
	DRECORDER_WB_MODE_U30,				//商用光源
	DRECORDER_WB_MODE_CWF,				//冷荧光
	DRECORDER_WB_MODE_FLUORESCENT,		//日光灯
	DRECORDER_WB_MODE_SUN,				//晴天
	DRECORDER_WB_MODE_CLOUD,			//阴天
	DRECORDER_WB_MODE_MAX
}DRECORDER_PARAM_WB_MODE_E;

typedef enum
{
    // for camera and common.
	DRECORDER_INFO_NONE = 0,
	DRECORDER_INFO_BRIGHTNESS,
	DRECORDER_INFO_CONTRAST,
	DRECORDER_INFO_SHARPNESS,
	DRECORDER_INFO_SATURATION,
	DRECORDER_INFO_ZOOM,
	DRECORDER_INFO_EFFECT,
	DRECORDER_INFO_PRVIEW_MODE,	
	DRECORDER_INFO_WB_MODE,	
	DRECORDER_INFO_VIDEO_QUALITY,
	DRECORDER_INFO_AUDIO_QUALITY,
	DRECORDER_INFO_VIDEO_MODE,

    // only for ATV.
    DRECORDER_INFO_ATV_CH_ALL_NUM,  // get the num of channel in current region.
    DRECORDER_INFO_PREVIEW_ROT,  // get the num of channel in current region.    
    
	DRECORDER_INFO_MAX
}DRECORDER_INFO_CMD_E;

typedef enum
{
    // for camera and common.
	DRECORDER_PARAM_CTRL_NONE = 0,
	DRECORDER_PARAM_CTRL_BRIGHTNESS,
	DRECORDER_PARAM_CTRL_CONTRAST,
	DRECORDER_PARAM_CTRL_SHARPNESS,
	DRECORDER_PARAM_CTRL_SATURATION,
	DRECORDER_PARAM_CTRL_ZOOM,
	DRECORDER_PARAM_CTRL_EFFECT,
	DRECORDER_PARAM_CTRL_PRVIEW_MODE,	
	DRECORDER_PARAM_CTRL_WB_MODE,	
	DRECORDER_PARAM_CTRL_VIDEO_QUALITY,
	DRECORDER_PARAM_CTRL_AUDIO_QUALITY,
	DRECORDER_PARAM_CTRL_FLICKER_COMPENSATION,		
	DRECORDER_PARAM_CTRL_VIDEO_MODE,

    // only for ATV.
    DRECORDER_PARAM_CTRL_ATV_CHN,       // set current ATV channel ID.
    DRECORDER_PARAM_CTRL_ATV_REGION,    // set current ATV region.
    
	DRECORDER_PARAM_CTRL_MAX
}DRECORDER_PARAM_CTRL_CMD_E;


typedef enum
{
	DRECORDER_DISP_FULLVIEW=0x00,		//remain width height rate, no trim to fill Rect,maybe not full
	DRECORDER_DISP_FULLSCREEN,			//remain width height rate, and Trim to fill target_rect fullly
	DRECORDER_DISP_CUSTOMIZE,		    //scale or zoom to fill target_rect fully
	DRECORDER_DISP_MODE_MAX
}DRECORDER_DISP_MODE_E;


typedef enum
{
	DRECORDER_SRC_DEV_DCAMERA = 0,
	DRECORDER_SRC_DEV_ATV,
	DRECORDER_SRC_DEV_ATV_FACTORY,
	DRECORDER_SRC_DEV_STREAMING,
	DRECORDER_SRC_DEV_MAX
}DRECORDER_SOURCE_DEVICE_E;

typedef enum
{
    // audio output device.
    DRECORDER_AUD_DEV_NONE = 0,   // discard the audio,mute.
    DRECORDER_AUD_DEV_SPEAKER,    // send to speaker or ear.
    DRECORDER_AUD_DEV_BT,         // send to blue tooth.
    DRECORDER_AUD_DEV_NET,        // send to NET.reserved now.
    DRECORDER_AUD_DEV_FILE,       // send to file.reserved now.
    // audio input device.
    DRECORDER_AUD_DEV_MAX         // invalid.
}DRECORDER_AUD_DEV_E;

typedef enum
{
    // for ATV.
    DRECORDER_REPORT_ATV_SCAN_CHN     = 0x01,
    DRECORDER_REPORT_ATV_SCAN_CHN_END,
    DRECORDER_REPORT_ATV_SCAN_CHN_STOP,
    DRECORDER_REPORT_ATV_RSSI,
    DRECORDER_REPORT_ATV_ERR,

    // for other.
    DRECORDER_REPORT_TYP_MAX
}DRECORDER_REPORT_TYPE_E;


typedef struct
{
    uint32               x;
    uint32               y;	
    uint32               dx;
    uint32               dy;
}DRECORDER_DISP_RECT_T;


/* report info structure in callback function for demon.*/
typedef struct
{
    uint32 report_type;
    uint32 report_data[3];
}DRECORDER_DEMON_REPORT_DATA_T;

/* report info structure in callback function for ATV. */
typedef struct
{
    DRECORDER_REPORT_TYPE_E report_typ;             // sub type.
    uint32 chn_is_valid;    // data[0:3]
    uint32 chn_id;
    uint32 rssi;
    uint32 rtn;
}DRECORDER_ATV_REPORT_DATA_T;

typedef union
{
    DRECORDER_DEMON_REPORT_DATA_T demon_report;
    DRECORDER_ATV_REPORT_DATA_T   atv_report;
}DRECORDER_REPORT_DATA_U;

typedef uint32 (*DRECORDER_REPORT_CALLBACK)(DRECORDER_REPORT_DATA_U *report_ptr);


typedef struct
{
	DRECORDER_DISP_RECT_T disp_rect;//image & osd display area	
    DRECORDER_DISP_RECT_T target_rect;//image display area
	uint32  DisplayRotateAngle;	//0,0;1,90;2,180;3,270;
	uint32	EncRotateAngle;		//0,0;1,90;2,180;3,270;
	uint32	LcdId; 				//LCD ID，如果为TV ID，则为TV-OUT状态。
	uint32  LcdBlkId;			//LCD block ID
	uint32	image_format;		// 0: default; 1: RAW; 2: YUV422
	DRECORDER_DISP_MODE_E	display_mode;

    DRECORDER_REPORT_CALLBACK report_callback;

}DRECORDER_PREVIEW_PARAM_T; 

/* callback for parameter control */
typedef uint32 (* DRECORDER_PARAM_CTRL_CALL_BACK)(void* param_ptr, uint32 param_size);

typedef	void (*DRECORDER_FRAME_CALLBACK)(void);
typedef void (*DRECORDER_END_CALLBACK)(int32 i_type);


typedef union
{
    struct
    {
        uint8* p_data; // data point for JPEG,MCU,RGB565.
        uint32 len;
    }onepannel;
    
    struct
    {
        uint8 *p_y;
        uint8 *p_uv;
    }yuv2pannel; // data point for yuv 2 pannels.
    
    struct
    {
        uint8 *p_y;
        uint8 *p_u;
        uint8 *p_v;
    }yuv3pannel; // data point for yuv 3 pannels.

}DRECORDER_VIDEO_DATAT_U;

typedef struct
{
    uint32 width;               //  width and height.
    uint32 height;

    DRECORDER_VIDEO_DATATYPE_E data_type;     // data type.
    DRECORDER_VIDEO_DATAT_U    data;
}DRECORDER_VIDEO_FRAME_T;

typedef struct
{
    uint32 width;               //  width and height.
    uint32 height;

    DRECORDER_VIDEO_DATATYPE_E data_type;     // data type.
}DRECORDER_THUMBNAIL_PARAM_T;

// when APP calls DRECORDER_StopRecord() to stop recording,the callback func will 
// be callback to notify APP to get the last frame data.
typedef void (*DRECORDER_GETLASTFRAME_CALLBACK)(DRECORDER_VIDEO_FRAME_T *p_lastframe);


typedef  struct
{
	MEDIA_AUDIO_INFO_T *			audio_info_ptr;
	MEDIA_VIDEO_INFO_T *			video_info_ptr; 
	DRECORDER_FRAME_CALLBACK		frame_callback;
	DRECORDER_END_CALLBACK			end_callback;
	uint8 *							file_name_ptr; 
	uint32							file_name_byte_len;
	uint32							file_limit_size;
	DRECORDER_FILE_TYPE_E			file_type;
}DRECORDER_START_RECORD_T;

typedef struct
{
    uint32 data_size;               // size of the recorded data in file.unit:byte.
    uint32 media_time;              // time of the recorder.unit:millisecond.
    uint32 video_frame_count;       // frame count of video recorded.
}DRECORDER_RECORD_INFO_T;

typedef struct
{
    BOOLEAN b_a2dp_support;
    uint8   res0;
    uint16  res1;
}DRECORDER_AUD_BT_INFO_T;

typedef union
{
    DRECORDER_AUD_BT_INFO_T bt_info;
}DRECORDER_AUD_DEV_INFO_U;

typedef struct
{
    DRECORDER_AUD_DEV_E         aud_dev;    // audio device.
    DRECORDER_AUD_DEV_INFO_U    info;       // reserved now.
}DRECORDER_AUD_DEV_PARAM_T;


/******************************************************************************/
// Description: Drecorder Open
// Author:     
// Input:      none
// Output:     none
// Return:     DRECORDER_RETURN_E
// Note:       none
/******************************************************************************/ 
DRECORDER_RETURN_E DRECORDER_Open (void);
/******************************************************************************/
// Description: Drecorder Open
// Author:     
// Input:      DRECORDER_OP_MODE_E
// Output:     none
// Return:     DRECORDER_RETURN_E
// Note:       none
/******************************************************************************/ 
DRECORDER_RETURN_E DRECORDER_OpenEx(DRECORDER_OP_MODE_E mode);

/******************************************************************************/
// Description: Drecorder Close
// Author:     
// Input:      none
// Output:     none
// Return:     DRECORDER_RETURN_E
// Note:       none
/******************************************************************************/
DRECORDER_RETURN_E DRECORDER_Close (void);


/******************************************************************************/
// Description: trigger the recorder to start recording
// Author:     
// Input:      none
// Output:     none
// Return:     DRECORDER_RETURN_E
// Note:       none
/******************************************************************************/
DRECORDER_RETURN_E DRECORDER_StartRecord(DRECORDER_START_RECORD_T const * record_ptr);


/******************************************************************************/
// Description: stop recording
// Author:     
// Input:      none
// Output:     none
// Return:     DRECORDER_RETURN_E
// Note:       none
/******************************************************************************/
DRECORDER_RETURN_E DRECORDER_StopRecord (void);

/******************************************************************************/
// Description: Start Preview
// Author:     
// Input:      record_param_t *pParam pointer to record parameter strcture
// Output:     none
// Return:     DRECORDER_RETURN_E
// Note:       none
/******************************************************************************/
DRECORDER_RETURN_E DRECORDER_StartPreview(DRECORDER_PREVIEW_PARAM_T const * param_ptr);

/******************************************************************************/
// Description: Stop Preview
// Author:     
// Input:      none
// Output:     none
// Return:     DRECORDER_RETURN_E
// Note:       none
/******************************************************************************/
DRECORDER_RETURN_E DRECORDER_StopPreview (void);


/***************************************************************************************/
// Description: Control Preview effect
// Global resource dependence:
// Author:     
// Note:
//		input:  cmd, command, enum DC_PREVIEW_CTL_CMD_E
//				arg, argument, enum DC_PREVIEW_CTL_PARAM_E
// 		output: none
// 		Return: DRECORDER_RETURN_E
/***************************************************************************************/
DRECORDER_RETURN_E DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_CMD_E param_cmd, uint32  arg,
                                                DRECORDER_PARAM_CTRL_CALL_BACK callback);

/***************************************************************************************/
// Description: pause or resume recording
// Global resource dependence:
// Author: Jianping.wang
// Note:
// 		input:
//			none
// 		output:
//			none
// 		return:
// 			DRECORDER_RETURN_E
/***************************************************************************************/									             
DRECORDER_RETURN_E DRECORDER_Pause (void);

/***************************************************************************************/
// Description: resume recording
// Global resource dependence:
// Author:
// Note:
// 		input:
//			none
// 		output:
//			none
// 		return:
// 			DRECORDER_RETURN_E
/***************************************************************************************/
DRECORDER_RETURN_E DRECORDER_Resume (void);


/******************************************************************************/
// Description  :Get the scope of CMD's parameter.It'll be called before "ParamCtrl".
// Author   :
// Input    :param_cmd,Command enum.
// Output   :info_ptr, the max value according to the CMD that MMI can set. 
// Return   :     
// Note     :none
/******************************************************************************/
DRECORDER_RETURN_E DRECORDER_GetInfo(DRECORDER_INFO_CMD_E	param_cmd, uint32*	info_ptr);


/******************************************************************************/
// Description:when APP calls DRECORDER_StopRecord() to stop recording,the callback
//             func will be callback to notify app to get the last frame data.
// Author:     
// Input:      getlastframe_callback,the callback func addr.
//             data_format, yuv or rgb.
// Output:     none
// Return:     
// Note:       none
/******************************************************************************/
PUBLIC DRECORDER_RETURN_E DRECORDER_RegisterGetLastFrameCallback(
                                DRECORDER_GETLASTFRAME_CALLBACK getlastframe_callback,
                                DRECORDER_VIDEO_DATATYPE_E data_type);
                                
/******************************************************************************/
// Description:when APP calls DRECORDER_StopRecord() to stop recording,the callback
//             func will be callback to notify app to get the last thumbnail data.
// Author:     
// Input:      getlastthumbnail_callback,the callback func addr.
//             DRECORDER_THUMBNAIL_PARAM_T
// Output:     none
// Return:     
// Note:       none
/******************************************************************************/
PUBLIC DRECORDER_RETURN_E DRECORDER_RegisterGetLastThumbnailCallback(
                                DRECORDER_GETLASTFRAME_CALLBACK getlastthumbnail_callback,
                                DRECORDER_THUMBNAIL_PARAM_T* thumbnail_param);

/******************************************************************************/
// Description: Get the recorded information when recording.
// Author:     
// Input:      p_record_info, address to receive the recorded info.
// Output:     none
// Return:     
// Note:       none
/******************************************************************************/
PUBLIC DRECORDER_RETURN_E DRECORDER_GetRecordInfo(
                            DRECORDER_RECORD_INFO_T *p_record_info
                            );


/******************************************************************************/
// Description:Select source device for drecorder.Optional API.
// Author:     
// Input:      none
// Output:     none
// Return:     
// Note:       This function should be called flowing DRECORDER_Open(). 
//             If the API hasn't be called, source type will be
//             the default( = DRECORDER_SRC_TYP_DCAMERA).
/******************************************************************************/
PUBLIC DRECORDER_RETURN_E DRECORDER_SelectSrcDevice(
                                           DRECORDER_SOURCE_DEVICE_E  src_dev);


/******************************************************************************/
// Description:Set the display param for video.Optional API.
// Author:     
// Input:      none
// Output:     none
// Return:     
// Note:       This function should be called between DRECORDER_Open() and 
//             DRECORDER_Close().If the API hasn't be called, display parameter
//             will be equal to the parameter in DRECORDER_StartPreview.
/******************************************************************************/
PUBLIC DRECORDER_RETURN_E DRECORDER_SetVideoDispParam(
                            DRECORDER_PREVIEW_PARAM_T const *p_disp_param);


/******************************************************************************/
// Description:Set the adio output param for ATV.
// Author:     
// Input:      none
// Output:     none
// Return:     
// Note:       none
/******************************************************************************/
PUBLIC DRECORDER_RETURN_E DRECORDER_ATV_SetAudioOutParam(
                            DRECORDER_AUD_DEV_PARAM_T const *p_aud_param);


/******************************************************************************/
// Description:Start to scan all the available channels.When one channel has been
//             scanned,DRECORDER will report the info of the channel to APP throug
//             the report_callback function.
// Author:     
// Input:      none
// Output:     none
// Return:     
// Note:       none
/******************************************************************************/
PUBLIC DRECORDER_RETURN_E DRECORDER_ATV_StartScanChn(
                            DRECORDER_PREVIEW_PARAM_T const *p_preview_param);


/******************************************************************************/
// Description:Stop scaning all the available channels.
// Author:     
// Input:      none
// Output:     none
// Return:     
// Note:       none
/******************************************************************************/
PUBLIC DRECORDER_RETURN_E DRECORDER_ATV_StopScanChn(void);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /*_DAL_RECORDER_H_*/


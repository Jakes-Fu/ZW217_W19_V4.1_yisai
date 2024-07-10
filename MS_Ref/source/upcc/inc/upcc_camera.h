/******************************************************************************
 ** File Name:    UPCC_camera.h                                               *
 ** Author:       yuhua.shi                                                   *
 ** DATE:         3/23/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
 /*****************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/23/2006     yuhua.shi       Create.                                     *
 ******************************************************************************/
#ifndef _UPCC_CAMERA_H_
#define _UPCC_CAMERA_H_
    
/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */           
#include "upcc_common.h"
#include "dal_dcamera.h"
#include "upcc_api.h"


/**---------------------------------------------------------------------------*
 **                         COMPILER FLAG                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {    
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/   


#define UPCC_JPEG_WIDTH_160         0x00A0
#define UPCC_JPEG_HEIGHT_128        0x0080
#define UPCC_JPEG_WIDTH_320         0x0140
#define UPCC_JPEG_HEIGHT_240        0x00F0


//video frame rate constant
#define UPCC_FRAME_RATE_NUM         UPCC_VIDEO_FRAME_RATE_MAX
#define UPCC_MJPEG_HIGH_FRAME_RATE	30//30//15	//15FPS
#define UPCC_MJPEG_MIDD_FRAME_RATE	15//10	//10FPS
#define UPCC_MJPEG_LOW_FRAME_RATE	10//6	//6FPS
   
//video frame size constant
#define UPCC_FRAME_SIZE_NUM        UPCC_VIDEO_SIZE_MAX



//default value
#define UPCC_BRIGHTNESS_DEF       (UPCC_BRIGHTNESS_MAX/2)
#define UPCC_CONTRAST_DEF         (UPCC_CONTRAST_MAX/2)
#define UPCC_HUE_DEF              UPCC_EFFECT_NORMAL
#define UPCC_FRAME_RATE_DEF       UPCC_VIDEO_FRAME_RATE_HIGH
#define UPCC_FRAME_SIZE_DEF       UPCC_VIDEO_SIZE_320X240



#define	UPCC_JPEG_BUF_MAX_SIZE		    (32 * 1024)

#define UPCC_MAINSCREEN_WIDTH       128
#define UPCC_MAINSCREEN_HEIGHT      160

#define UPCC_VC_CONTROL_INIT_LEN    sizeof(UPCC_INIT_CONTROL_INFO_T)

#define UPCC_BLACK_COLOR		        0x0000

typedef struct
{
    uint16 				  	   	left;        	   	/*!<left up point coordinate x */
    uint16 					   	top;           	   	/*!<left up point coordinate y */
    uint16 					   	right;         	   	/*!<right down point coordinate x */
    uint16 					   	bottom;       	   	/*!<right down point coordinate y */
}UPCC_RECT_T;



//@fly add 2006-09-21
typedef struct upcc_jpg_frame_tag
{
	uint8 	*p_head;	//jpeg file buffer head
	uint32	len;		//jpeg file len
}UPCC_JPG_FRAME_T;

typedef __packed struct upcc_get_more_value_tag
{
    uint16 def_value;//default value
    uint16 cur_value;//current value
    uint16 min_value;//minimum value
    uint16 max_value;//maximum value
}UPCC_GET_MORE_VALUE_T;

typedef __packed struct upcc_more_init_value_tag
{
    uint16 def_value;//default value
    uint16 min_value;//minimum value
    uint16 max_value;//maximum value
}UPCC_MORE_INIT_VALUE_T;

typedef __packed struct upcc_frame_rate_info_tag
{
    uint16 def_value;//default value
    uint16 num;      //number
    uint16 size_array[UPCC_FRAME_RATE_NUM];
}UPCC_FRAME_RATE_INFO_T;

typedef __packed struct upcc_frame_size_info_tag
{
    uint16 def_value;//default value
    uint16 num;      //number
    uint16 size_array[UPCC_FRAME_SIZE_NUM*2];
}UPCC_FRAME_SIZE_INFO_T;

//struct of init control request
typedef __packed struct upcc_init_control_tag
{
    uint16                      length;
    UPCC_MORE_INIT_VALUE_T      brightness;
    UPCC_MORE_INIT_VALUE_T      contrast;
    UPCC_MORE_INIT_VALUE_T      hue;
    UPCC_FRAME_RATE_INFO_T      frame_rate;
    UPCC_FRAME_SIZE_INFO_T      frame_size; 
}UPCC_INIT_CONTROL_INFO_T;


/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef struct upcc_setting_info_tag
{
	UPCC_GET_MORE_VALUE_T   st_contrast;	//对比度
	UPCC_GET_MORE_VALUE_T   st_brightness;	//亮度
	UPCC_VIDEO_SIZE_E		e_video_size;	//录像大小
	UPCC_VIDEO_FRAME_RATE_E	e_frame_rate;	//帧率（高、低）
	UPCC_PHOTO_EFFECT_E	    e_effect;		//效果（正常、黑白、蓝色、绿色、红色、帆布、底片）
	UPCC_VIDEO_QUALITY_E	e_quality;		//质量（高级、普通）
	UPCC_VIDEO_ROTATION_E   e_rotation;
	
}UPCC_SETTING_INFO_T;


/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get pc camera setting info
//	Global resource dependence : 
//  Author: yuhua.shi
//	Note:
/*****************************************************************************/
PUBLIC UPCC_SETTING_INFO_T* UPCC_GetSettingInfo(void);

/*****************************************************************************/
// 	Description : init dc setting
//	Global resource dependence : 
//  Author: yuhua.shi
//	Note:
/*****************************************************************************/
PUBLIC void UPCC_SettingInit(void);

/*****************************************************************************/
// 	Description : set the start flag of video started by host
//	Global resource dependence : 
//  Author: yuhua.shi
//	Note:
/*****************************************************************************/
PUBLIC void UPCC_SetVideoStartFlagByHost(BOOLEAN flag);

/*****************************************************************************/
// 	Description : get the start flag of video started by host
//	Global resource dependence : 
//  Author: yuhua.shi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_IsVideoStartByHost(void);

/*****************************************************************************/
//  Description: set the init control value for pc
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_SetInitControl(UPCC_INIT_SET_INFO_T *init_set_prt);

/*****************************************************************************/
//  Description: start the video streaming of M-JPEG
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_StartVideoStreaming(void);

/*****************************************************************************/
//  Description: stop the video streaming of M-JPEG
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_StopVideoStreaming(void);

/*****************************************************************************/
//  Description: set the brightness value to dc
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_SetBrightnessValue(uint16 value);

/*****************************************************************************/
//  Description: get the brightness value from dc
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_GetBrightnessValue(uint16* value, uint8 bRequest);

/*****************************************************************************/
//  Description: set the contrast value to dc
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_SetContrastValue(uint16 value);

/*****************************************************************************/
//  Description: get the contrast value from dc
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_GetContrastValue(uint16* value, uint8 bRequest);

/*****************************************************************************/
//  Description: set the hue value to dc
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_SetHueValue(uint16 value);

/*****************************************************************************/
//  Description: get the hue value from dc
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_GetHueValue(uint16* value);

/*****************************************************************************/
//  Description: set the frame size pattern of M-JPEG
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_SetFrameSizePattern(UPCC_VIDEO_SIZE_E type);

/*****************************************************************************/
//  Description: get the frame size pattern of M-JPEG
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_GetFrameSizePattern(UPCC_VIDEO_SIZE_E* type);

/*****************************************************************************/
//  Description: set the frame rate of M-JPEG
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_SetFrameRate(uint8 type);

/*****************************************************************************/
//  Description: get the frame rate of M-JPEG
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_GetFrameRate(uint8* type);

/*****************************************************************************/
// 	Description : get a frame and send it
//	Global resource dependence : 
//  Author: feifei.gu
//	Note:
/*****************************************************************************/
PUBLIC void UPCC_GetAndSendFrame();

/**---------------------------------------------------------------------------*
**                          COMPILER FLAG                                     *
**----------------------------------------------------------------------------*/    
#ifdef __cplusplus
    }
#endif    


/**---------------------------------------------------------------------------*/   
#endif //_UPCC_CAMERA_H_
//end UPCC_camera.h

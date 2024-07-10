/******************************************************************************
 ** File Name:    UPCC_api.h                                                  *
 ** Author:       yuhua.shi                                                   *
 ** DATE:         3/24/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
 /*****************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/24/2006     yuhua.shi       Create.                                     *
 ******************************************************************************/
#ifndef _UPCC_API_H_
    #define _UPCC_API_H_
    
/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */          
#include "gadget.h"

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
//brightness constant
#define UPCC_BRIGHTNESS_MAX    6
#define UPCC_BRIGHTNESS_MIN    0

//contrast constant
#define UPCC_CONTRAST_MAX    6
#define UPCC_CONTRAST_MIN    0

//effect constant
#define UPCC_HUE_MIN         UPCC_EFFECT_NORMAL
#define UPCC_HUE_MAX         (UPCC_EFFECT_MAX-1)

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
	UPCC_VIDEO_QUALITY_HIGH=6,	//高级
	UPCC_VIDEO_QUALITY_NORMAL=4,//普通
	UPCC_VIDEO_QUALITY_LOW=2,	//简洁
    UPCC_VIDEO_QUALITY_MAX
}UPCC_VIDEO_QUALITY_E;//拍摄质量

typedef enum
{
	UPCC_VIDEO_FRAME_RATE_LOW = 1,
	UPCC_VIDEO_FRAME_RATE_MID = 0,
	UPCC_VIDEO_FRAME_RATE_HIGH= 2,
	UPCC_VIDEO_FRAME_RATE_MAX
}UPCC_VIDEO_FRAME_RATE_E;	//拍摄帧率


typedef enum
{
	UPCC_EFFECT_NORMAL = 0,	//正常
	UPCC_EFFECT_BLACKWHITE,	//黑白
	UPCC_EFFECT_RED,		//红色
	UPCC_EFFECT_GREEN,		//绿色
	UPCC_EFFECT_BLUE,		//蓝色
	UPCC_EFFECT_YELLOW,		//黄色
	UPCC_EFFECT_SEPIA,		//怀旧
	UPCC_EFFECT_NEGATIVE,	//底片
	UPCC_EFFECT_MAX
}UPCC_PHOTO_EFFECT_E;	//照片效果

typedef enum
{
	UPCC_VIDEO_SIZE_160X128 = 0,
	UPCC_VIDEO_SIZE_320X240,
	UPCC_VIDEO_SIZE_240X320,
	UPCC_VIDEO_SIZE_MAX
}UPCC_VIDEO_SIZE_E;	//录像的尺寸

typedef enum
{
	UPCC_IMAGE_FORMAT_DEFAULT = 0x00,	// depend on low level driver
	UPCC_IMAGE_FORMAT_RAW,				// 0x01
	UPCC_IMAGE_FORMAT_YUV422,			// 0x02
	UPCC_IMAGE_FORMAT_MAX

}UPCC_SENSOR_IMAGE_FORMAT_E;

typedef enum
{
	UPCC_VIDEO_ROTATION_0 = 0,
	UPCC_VIDEO_ROTATION_90,
	UPCC_VIDEO_ROTATION_180,
	UPCC_VIDEO_ROTATION_270,
	UPCC_VIDEO_ROTATION_MIRROR,
	UPCC_VIDEO_ROTATION_FLIP,
	UPCC_VIDEO_ROTATION_MAX
}UPCC_VIDEO_ROTATION_E;

//struct of init control request
typedef __packed struct upcc_init_set_info_tag
{
    uint16                      brightness;
    uint16                      contrast;
    UPCC_PHOTO_EFFECT_E         hue;
    UPCC_VIDEO_FRAME_RATE_E     frame_rate;
    UPCC_VIDEO_SIZE_E           frame_size; 
}UPCC_INIT_SET_INFO_T;

/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC const USB_GADGET_DRIVER_T * UPCC_GetHandler(void);

/*****************************************************************************/
//  Description: init the pc camera
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_InitPCCamera(UPCC_INIT_SET_INFO_T *init_set_prt);

/*****************************************************************************/
//  Description: start the pc camera
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_OpenPCCamera(void);

/*****************************************************************************/
//  Description: stop the pc camera
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_ClosePCCamera(void);

/*****************************************************************************/
//  Description: stop the video streaming 
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_StartVideoStreaming(void);

PUBLIC BOOLEAN UPCC_StopVideoStreaming(void);

PUBLIC uint32 UPCC_GetCOnfigDescriptor (uint8 **para);

/**---------------------------------------------------------------------------*
**                          COMPILER FLAG                                     *
**----------------------------------------------------------------------------*/    
#ifdef __cplusplus
    }
#endif    


/**---------------------------------------------------------------------------*/   
#endif //_UPCC_API_H_
//end UPCC_api.h

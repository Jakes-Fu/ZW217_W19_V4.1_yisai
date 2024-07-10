/*****************************************************************************
** File Name:      mmi_pccamera.h                                                *
** Author:                                                                   *
** Date:           17/05/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe pc camera                       *          
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2006       gang.tong         Create
******************************************************************************/


#ifndef _MMI_PC_CAMERA_H_
#define _MMI_PC_CAMERA_H_


//#define USB_PCC

#ifndef WIN32
#include "upcc_api.h"
#else

//#include "isp_service.h"
typedef enum
{
    ISP_IMAGE_FORMAT_YUV422 = 0,
        ISP_IMAGE_FORMAT_YUV420,
        ISP_IMAGE_FORMAT_RAW,
        ISP_IMAGE_FORMAT_RGB565,
        ISP_IMAGE_FORMAT_RGB666,
        ISP_IMAGE_FORMAT_RGB888,
        ISP_IMAGE_FORMAT_CCIR656,
        ISP_IMAGE_FORMAT_MAX
}ISP_IMAGE_FORMAT;

typedef enum
{
    UPCC_VIDEO_FORMAT_MJPEG,	//Motion JPEG
        UPCC_VIDEO_FORMAT_MAX
}UPCC_VIDEO_FORMAT_E;

typedef enum
{
    UPCC_VIDEO_QUALITY_HIGH=8,	//高级
        UPCC_VIDEO_QUALITY_NORMAL=5,//普通
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

//struct of init control request

typedef struct upcc_init_set_info_tag
{
    uint16                      brightness;
    uint16                      contrast;
    UPCC_PHOTO_EFFECT_E         hue;
    UPCC_VIDEO_FRAME_RATE_E     frame_rate;
    UPCC_VIDEO_SIZE_E           frame_size; 
    BOOLEAN                     is_lcd_display_enabled;
    BOOLEAN						vflip_enable;//vertical flip
    ISP_IMAGE_FORMAT						image_format;
}UPCC_INIT_SET_INFO_T;

#endif
#endif



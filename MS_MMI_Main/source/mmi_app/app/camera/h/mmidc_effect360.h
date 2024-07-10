/*****************************************************************************
** File Name:      mmidc_effect360.h                                         *
** Author:                                                                   *
** Date:           02/02/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to effect 360                           *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012       robert.wang         Create
******************************************************************************/

#ifndef _MMIDC_EFFECT360_H_
#define _MMIDC_EFFECT360_H_ 

#if defined(MMIDC_F_EFFECT_360)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmidc_ucamera_internal.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/ 
#define MMIDC_FISH_EYE_BG_COLOR 0x80808080

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct
{
    DC_IMG_SIZE_T img_size;
    MMIDC_BUFFER_DATA_T osd_data;
    MMIDC_DATA_ADDR_T osd_yuv;
}MMIDC_EFFECT_360_OSD_INFO_T;

typedef struct
{
    IMGPROC_HANDLE handle;
    uint32 cur_index;
    IMG_PROC_HDR_T hdr_param;
}MMIDC_HDR_INFO_T;

typedef struct
{
    IMGPROC_HANDLE handle;
    uint32 cur_index;
    IMG_PROC_FISHEYE_T fisheye_param;
}MMIDC_FISHEYE_INFO_T;

typedef struct
{
    IMGPROC_HANDLE handle;
    uint32 cur_index;
    IMG_PROC_LOMO_PARAM_T lomo_param;
}MMIDC_LOMO_INFO_T;

typedef struct
{
    IMGPROC_HANDLE handle;
    uint32 cur_index;
    IMG_PROC_PENCILSKETCH_T black_white_drawing_param;
}MMIDC_BLACK_WHITE_DRAWING_INFO_T;

typedef struct
{
    IMGPROC_HANDLE handle;
    uint32 cur_index;
}MMIDC_X_RAY_INFO_T;

typedef struct
{
    IMGPROC_HANDLE handle;
    uint32 cur_index;
}MMIDC_INFRARED_INFO_T;

typedef struct
{
    IMGPROC_HANDLE handle;
    uint32 cur_index;
}MMIDC_EXPOSAL_INFO_T;

typedef struct
{
    IMGPROC_HANDLE handle;
    uint32 cur_index;
}MMIDC_NEGATIVE_INFO_T;

typedef struct
{
    BOOLEAN is_not_comon_buf;
    MMIDC_IMAGE_PROCESS_TYPE_E cur_common_type;
    MMIDC_HDR_INFO_T hdr_info;
    MMIDC_FISHEYE_INFO_T fisheye_info;
    MMIDC_LOMO_INFO_T lomo_info;
    MMIDC_BLACK_WHITE_DRAWING_INFO_T black_white_drawing_info;
    MMIDC_X_RAY_INFO_T x_ray_info;
    MMIDC_INFRARED_INFO_T infrared_info;
    MMIDC_EXPOSAL_INFO_T exposal_info;
    MMIDC_NEGATIVE_INFO_T negative_info;
    MMIDC_EFFECT_360_OSD_INFO_T osd_info;
    MMIDC_U_CAMERA_PHOTO_INFO_T photo_info;
}MMIDC_EFFECT_360_COMMON_INFO_T;

typedef struct
{
    BOOLEAN is_enable;
    MMIDC_IMAGE_PROCESS_TYPE_E cur_type;
}MMIDC_EFFECT_360_INFO_T;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open effect 360
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenEffect360(void);
/*****************************************************************************/
//  Description : close effect 360
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseEffect360(void);

/*****************************************************************************/
//Description : get effect 360 image process parameter
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetEffect360ImageProcessParam(MMIDC_IMAGE_PROCESS_PARAM_T *img_param_ptr /*out*/
                                                               );
                                                               
/*****************************************************************************/
//Description : is effect 360 enable 
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsEffect360Enable(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //#if defined(MMIDC_F_EFFECT_360)

#endif //#ifndef _MMIDC_EFFECT360_H_





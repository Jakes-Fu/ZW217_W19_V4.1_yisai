/*****************************************************************************
** File Name:      mmidc_ucamera_internal.h                                  *
** Author:                                                                   *
** Date:           02/14/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to u camera                             *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012       robert.wang         Create
******************************************************************************/

#ifndef _MMIDC_UCAMERA_INTERNAL_H_
#define _MMIDC_UCAMERA_INTERNAL_H_

#if defined(MMIDC_F_U_CAMERA)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "img_proc.h"

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
#define MMIDC_IMAGE_PROCESS_TYPE_E IMG_PROC_ALG_CMD_E
#define MMIDC_U_CAMERA_DEFAULT_PHOTO_SIZE PHOTO_SIZE_640X480

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

typedef struct
{
    uint32 w;
    uint32 h;
}DC_IMG_SIZE_T;

typedef struct
{
    BOOLEAN is_enable;
    uint8 *buffer_ptr;//
    uint32 buffer_size;
}MMIDC_BUFFER_DATA_T;

typedef struct
{
    uint8 *chn1_ptr;
    uint32 chn1_size;
    uint8 *chn2_ptr;
    uint32 chn2_size;
    uint8 *chn3_ptr;
    uint32 chn3_size;
}MMIDC_DATA_ADDR_T;

typedef enum
{
    UCAMERA_ERR_NONE,
    UCAMERA_ERR_NO_SD_CARD,
    UCAMERA_ERR_MAX
}MMIDC_U_CAMERA_ERR_E;

typedef enum
{
    UCAMERA_DATA_ROUTE_PREVIEW,
    UCAMERA_DATA_ROUTE_CAPTURE,
    UCAMERA_DATA_ROUTE_RES,//resource
    UCAMERA_DATA_ROUTE_MAX
}MMIDC_U_CAMERA_DATA_ROUTE_E;

typedef struct
{
    MMIDC_U_CAMERA_DATA_ROUTE_E data_from;
    DC_IMG_SIZE_T data_size;
    DCAMERA_DATA_TYPE_E data_type;
    MMIDC_DATA_ADDR_T data_addr;
    wchar *file_name_ptr;
    uint16 file_name_len;
}MMIDC_IMAGE_PROCESS_INFO_T;


typedef struct
{
    MMIDC_DATA_ADDR_T data_addr;
    BOOLEAN is_skip_image_process;//skip image process or not
}MMIDC_IMAGE_PROCESS_OUT_INFO_T;

typedef int32 (*UCAMERA_SYNC_CALLBACK)(IMG_PROC_PROCESS_RTN_T* param_ptr /*in*/, 
                                        MMIDC_IMAGE_PROCESS_INFO_T *src_ptr /*in*/, 
                                        MMIDC_IMAGE_PROCESS_OUT_INFO_T *dst_ptr /*out*/
                                        );

typedef struct
{
    IMGPROC_HANDLE handle;
    IMG_PROC_DATA_T src_data;
    IMG_PROC_PROCESS_T img_proc_param;//for img proc task
    UCAMERA_SYNC_CALLBACK sync_callback;
    MMIDC_IMAGE_PROCESS_INFO_T *src_info_ptr;//mmi image process
    MMIDC_IMAGE_PROCESS_OUT_INFO_T *dst_info_ptr;//mmi image process
}MMIDC_IMAGE_PROCESS_PARAM_T;


typedef struct
{
    MMIDC_BUFFER_DATA_T photo_data;
    DC_IMG_SIZE_T img_size;
}MMIDC_U_CAMERA_PHOTO_INFO_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //#if defined(MMIDC_F_U_CAMERA)

#endif //#ifndef _MMIDC_UCAMERA_INTERNAL_H_






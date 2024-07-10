/******************************************************************************
 ** File Name:      rotate_software.c                                         *
 ** Author:         Shan.He                                                   *
 ** DATE:           2010-10-09                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    software image rotation functions                         *
 ** Note:           Olny used in simulator                                    *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2010-10-09         Shan.He         Create                                 *
 *****************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "graphics_draw.h"

typedef struct _rotation_size_tag
{
    uint16 w;
    uint16 h;
} ROTATION_SIZE_T,*ROTATION_SIZE_T_PTR;

typedef struct _ROTATION_data_addr_tag
{
    uint32 y_addr;
    uint32 uv_addr;
    uint32 v_addr;
} ROTATION_DATA_ADDR_T,*ROTATION_DATA_ADDR_T_PTR;

typedef enum
{
    ROTATION_YUV422 = 0,
    ROTATION_YUV420,
    ROTATION_YUV400,
    ROTATION_RGB888,
    ROTATION_RGB666,
    ROTATION_RGB565,
    ROTATION_RGB555,
    ROTATION_MAX
} ROATION_DATA_FORMAT_E;

typedef enum
{
    ROTATION_90 = 0,
    ROTATION_270,
    ROTATION_180,
    ROTATION_MIRROR,
    ROTATION_DIR_MAX
} ROTATION_MODE_E;

typedef struct _rotation_tag
{
    ROTATION_SIZE_T         img_size;
    ROATION_DATA_FORMAT_E   data_format;
    ROTATION_MODE_E         rotation_mode; 
    ROTATION_DATA_ADDR_T    src_addr;
    ROTATION_DATA_ADDR_T    dst_addr;     
}ROTATION_PARAM_T, *ROTATION_PARAM_T_PTR;

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
 /******************************************************************************/
//  Description: rotate image
//  Author: Shan.He 
//  Input:   
//          param_ptr:	pointer of the input parameters.
//  Output: 
//  Return:	None
//  Note:   the caller should makesure the safety of the input parameters
/******************************************************************************/
PUBLIC uint32 Rotation_AppStart(ROTATION_PARAM_T* param_ptr);

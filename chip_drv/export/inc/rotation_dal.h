/******************************************************************************
 ** File Name:    rotation_dal.h                                       *
 ** Author:       Tim.zhu                                               *
 ** DATE:         04/13/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 04/13/2010    Tim.zhu   Create.                                     *
 ******************************************************************************/
#ifndef _ROTATION_DAL_H_
#define _ROTATION_DAL_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
typedef void (*ROTATION_IRQ_FUNC) (uint32);

/**----------------------------------------------------------------------------*
 **                         Emun                                               *
 **---------------------------------------------------------------------------*/
typedef enum
{
    ROTATION_INT_ROTATION_DONE = 0,
    ROTATION_INT_MAX
} ROTATON_INT_TYPE_E;

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
    ROTATION_DATA_YUV = 0,
    ROTATION_DATA_RGB,
    ROTATION_DATA_MAX
} ROTATION_DATA_MODE_E;

typedef enum
{
    ROTATION_ONE_BYTE = 0,
    ROTATION_TWO_BYTES,
    ROTATION_FOUR_BYTES,
    ROTATION_BYTE_MAX
} ROTATION_PIXEL_FORMAT_E;

typedef enum
{
    ROTATION_90 = 0,
    ROTATION_270,
    ROTATION_180,
    ROTATION_MIRROR,
    ROTATION_DIR_MAX
} ROTATION_MODE_E;

typedef enum
{
    ROTATION_NORMAL = 0,
    ROTATION_UV422,
    ROTATION_DATA_FORMAT_MAX
} ROTATION_UV_MODE_E;

/**------------------------------------------------------------------------------------*
 **                         Struct                                             *
 **------------------------------------------------------------------------------------*/
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

typedef struct _ROTATION_data_len_tag
{
    uint32 y_len;
    uint32 uv_len;
    uint32 v_len;
} ROTATION_DATA_LEN_T,*ROTATION_DATA_LEN_T_PTR;

typedef struct _rotation_ori_param_tag
{
    uint32 ori_width;
    uint32 offset_x;
    uint32 offset_y;
}ROTATION_ORI_PARAM_T;

/**----------------------------------------------------------------------------*
**                         extern function                                     **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/


#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End


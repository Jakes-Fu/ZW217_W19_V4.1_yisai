
/******************************************************************************
 ** File Name:    img_cascade.h                                       *
 ** Author:       Tim.zhu                                              *
 ** DATE:         09/15/2009                                                  *
 ** Copyright:    2009 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 09/15/2009    Tim.zhu   Create.                                     *
 ******************************************************************************/
#ifndef _IMG_CASCADE_H_
#define _IMG_CASCADE_H_
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

/**------------------------------------------------------------------------------------*
 **                         Macro define                                		       *
 **------------------------------------------------------------------------------------*/
#define MAX_COMBINE_NUM    4

/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
typedef enum 
{
    PANORAMA_HORIZONTAL_LEFT = 0, 
    PANORAMA_HORIZONTAL_RIGHT,
    PANORAMA_VERTICAL_UP,
    PANORAMA_VERTICAL_DOWN,
    PANORAMA_DIRECTION_MAX
} IMG_PANORAMA_DIRECTION_E;

typedef enum 
{
    PANORAMA_STAT_HORIZONTAL= 0, 
    PANORAMA_STAT_VERTICAL,
    PANORAMA_STAT_MAX
} IMG_PANORAMA_STAT_DIR_E;

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct _dc_size_tag
{
    uint16 width;
    uint16 height;
}IMG_SIZE_T; 

typedef struct _dc_addr_tag
{
    uint32 y_addr;
    uint32 uv_addr;
    //uint32 v_addr;    
}IMG_ADDR_T; 

typedef struct
{
    uint16  x0_point;
    uint16  y0_point;
    uint16  x1_point;
    uint16  y1_point;
}IMG_PANORAMA_MEATHED_T;

typedef struct
{
    IMG_PANORAMA_DIRECTION_E                 dir;             		/*全景拍方向*/    
    IMG_SIZE_T 							input_size;    		//单张图像大小
    uint32 								input_num;       	//全景拍照所用单张图像的张数
    IMG_ADDR_T*      				              input_img[MAX_COMBINE_NUM];	//单张图像的地址列表
    uint32                                                      swap_buf_addr;
    uint32                                                      swap_buf_len;    
    IMG_ADDR_T							output_addr;       //全景拍照输出图像的地址
    uint32 								output_buf_len;   	//全景拍照输出图像的buffer大小
    IMG_SIZE_T							output_size;  		//全景拍照输出图像的大小	
}IMG_PANORAMA_PARAM_T;

/******************************************************************************
// Description:	Panorama Mosaic 
// Author:		img_panorama_param_ptr: the point of panorama parameter
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
PUBLIC uint32 Img_PanoramaMosaic(IMG_PANORAMA_PARAM_T* img_panorama_param_ptr);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 


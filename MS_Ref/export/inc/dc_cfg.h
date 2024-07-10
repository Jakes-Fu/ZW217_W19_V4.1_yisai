/******************************************************************************
 ** File Name:      dc_cfg.h                                           *
 ** Author:         Tim.zhu                                             *
 ** DATE:           11/16/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the product configure dc parameter    *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/16/2009     Tim.zhu	  Create.                         		  *
 ******************************************************************************/

#ifndef DC_CFG_H_
#define DC_CFG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "dal_dcamera.h"
#include "jpeg_interface.h"
#include "dc_product_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
extern   "C" 
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro define         	                          *
 **---------------------------------------------------------------------------*/
#define DC_WIDTH_TO_ADD_THUM     640
#define DC_THUMBNAIL_WIDTH       160
#define DC_THUMBNAIL_HEIGHT      120
#define DC_DISPLAY_ROUND_BUFFER  4
#define DC_ENCODER_ROUND_BUFFER  4
#define DC_SLICE_HEIGHT          8
#define DC_JPEG_TMP_BUF_LEN      (48*1024)
#define DC_JPEG_SLICE_BS_LEN     (10*1024)
#define DC_JPEG_COMPRESS_LEVEL   4
#define DC_JPEG_YUV_MIN_INTERVAL (10*1024)
/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:  This function is set dc exif data time info
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC void DC_SetExifImageDataTime(void);


PUBLIC void DC_SetExifImagePixel(uint32 width, uint32 height);
/*****************************************************************************/
//  Description:  This function is used to get dc exif Param    
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
PUBLIC JINF_EXIF_INFO_T* DC_GetExifParameter(void);

/******************************************************************************
// Description: get the size struct  pointer of specific type
// Author:      Tim.zhu
// Input:       uint32 type: img size
// Output:      none
// Return:      sensor size struct pointer
******************************************************************************/ 
PUBLIC DCAMERA_SIZE_T* DC_GetSensorSizeStPtr(uint32 type);

/*****************************************************************************/
//  Description:  This function is used to get dc custom cfg
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
PUBLIC DC_PRODUCT_CFG_T_PTR DC_GeProductCfgPtr(void);


/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                             					        *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // end of dc_cfg.h    

 #endif

/****************************************************************************
** File Name:      mmidc_photodate.h                                        *
** Author:                                                                  *
** Date:           06/11/2011                                               *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the dc photo date          *
                   application.                                             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 06/2011     robert.wang           Create                                 *
****************************************************************************/

#ifndef _MMIDC_PHOTODATE_H
#define _MMIDC_PHOTODATE_H


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef MMI_CAMERA_F_PHOTO_DATE
#include "mmi_image.h"
#include "dal_dcamera.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define PHOTO_DATE_OSD_SPACE 2
#define OSD_INTERVAL_STRING_NUMBER 2
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    OSD_LAYOUT_ONE_LINE,
    OSD_LAYOUT_TWO_LINE,
    OSD_LAYOUT_MAX
}DATE_OSD_LAYOUT_E;

typedef enum
{
    OSD_LAYER_PHOTO,
    OSD_LAYER_THUMBNAIL,
    OSD_LAYER_MAX
}OSD_LAYER_ID_E;

typedef struct
{
    MMIDC_PHOTO_SIZE_E photo_size;
    int16 x;
    int16 y;
    int16 x_offset;
    int16 y_offset;
    GUI_FONT_T font;
}DATE_OSD_ATTRIBUTE_T;

typedef struct
{
    BOOLEAN is_enable;
    uint8 *buffer_ptr;
    uint16 layer_width;
    uint16 layer_height;
    GUI_LCD_DEV_INFO dev_info;
}PHOTO_DATE_LAYER_INFO_T;

typedef struct
{
    OSD_LAYER_ID_E cur_layer_id;
    PHOTO_DATE_LAYER_INFO_T layer[OSD_LAYER_MAX];
    PHOTO_DATE_LAYER_INFO_T display_layer[OSD_LAYER_MAX];
    DATE_OSD_ATTRIBUTE_T osd_attribute;
    GUI_COLOR_T font_color;
}MMIDC_PHOTO_DATE_INFO_T;
/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
 
/*****************************************************************************/
// 	Description : get photo date parameter
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetPhotoDateParameter(DCAMERA_CAPTURE_DATE_T *capture_date_ptr //out
                                        );
                                        
/*****************************************************************************/
//Description : get photo date dev info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO  MMIDC_GetPhotoDateDevInfo(void);

/*****************************************************************************/
//Description : get photo date OSD font color
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMIDC_GetPhotoDateOSDFontColor(void) ;

/*****************************************************************************/
//Description : create photo date data
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CreatePhotoDateData(void);

/*****************************************************************************/
//Description : release photo date data
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_ReleasePhotoDateData(void);

/*****************************************************************************/
//Description : get photo date display layer info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC PHOTO_DATE_LAYER_INFO_T MMIDC_GetPhotoDateDisplayLayerInfo(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  //MMI_CAMERA_F_PHOTO_DATE
#endif  //_MMIDC_EXPORT_H

 


/*****************************************************************************
** File Name:      mmidc_smileautophoto.h                                            *
** Author:                                                                   *
** Date:           02/06/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to camera work mode                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012       chunyou         Create
******************************************************************************/

#ifndef _MMIDC_SMILEAUTOPHOTO_H_
#define _MMIDC_SMILEAUTOPHOTO_H_ 

#if defined(MMIDC_F_SMILE_AUTO_PHOTO)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmidc_ucamera_internal.h"
#include "dal_dcamera.h"

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
#define MMIDC_SMILE_AUTO_PHOTO_SMILE_SCORE 75

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct
{
    BOOLEAN is_enable;
    uint32 cur_index;
    IMGPROC_HANDLE handle;
    GUI_LCD_DEV_INFO osd_dev_info;
    DCAMERA_RECT_T dc_disp_rect;
}MMIDC_SMILE_AUTO_PHOTO_INFO_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//Description : check if smile auto photo function opened
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsSmileAutoPhotoEnable(void);

/*****************************************************************************/
//Description : display smile auto photo image
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DisplaySmileAutoPhoto(void);

/*****************************************************************************/
//  Description : close smile auto photo
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseSmileAutoPhoto(void);

/*****************************************************************************/
//Description : change smile auto photo function enable or disable
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ChangeSmileAutoPhotoEnable(void);

/*****************************************************************************/
//Description : get smile auto photo image process parameter
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetSmileAutoPhotoImageProcessParam(MMIDC_IMAGE_PROCESS_PARAM_T *img_param_ptr /*out*/
                                                            );

/*****************************************************************************/
//  Description : open smile auto photo
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenSmileAutoPhoto(void);

/*****************************************************************************/
//  Description : create smile auto photo handle
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CreateSmileAutoPhotoHandle(void);

/*****************************************************************************/
//  Description : destroy smile auto photo handle
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DestroySmileAutoPhotoHandle(void);

/*****************************************************************************/
//Description : clear smile auto photo osd layer
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearSmileAutoPhotoOSDLayer(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //#if defined(MMIDC_F_SMILE_AUTO_PHOTO)

#endif //#ifndef _MMIDC_SMILEAUTOPHOTO_H_




/*****************************************************************************
** File Name:      mmidc_ninone.h                                            *
** Author:                                                                   *
** Date:           01/26/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to camera work mode                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2012       robert.wang         Create
******************************************************************************/

#ifndef _MMIDC_NINONE_H_
#define _MMIDC_NINONE_H_ 

#if defined(MMIDC_F_N_IN_ONE)

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
#define MMIDC_N_IN_ONE_IMG_COLOR 0x2124
#define MMIDC_N_IN_ONE_BORDER_COLOR 0x9CF3
#define MMIDC_N_IN_ONE_BORDER_WIDTH 1
#define MMIDC_N_IN_ONE_TOTAL_NUM 4
#define MMIDC_N_IN_ONE_LINE_NUM 2

#define MMIDC_N_IN_ONE_PHOTO_WIDTH 640
#define MMIDC_N_IN_ONE_PHOTO_HEIGHT 480

#if defined(MAINLCD_SIZE_320X480)
    #define MMIDC_N_IN_ONE_IMG_WIDTH  48
    #define MMIDC_N_IN_ONE_IMG_HEIGHT 36
    
    #define MMIDC_N_IN_ONE_H_X 374
    #define MMIDC_N_IN_ONE_H_Y 239
    
    #define MMIDC_N_IN_ONE_V_X 7
    #define MMIDC_N_IN_ONE_V_Y 398
#elif defined(MAINLCD_SIZE_240X400)
    #define MMIDC_N_IN_ONE_IMG_WIDTH  32
    #define MMIDC_N_IN_ONE_IMG_HEIGHT 24
    
    #define MMIDC_N_IN_ONE_H_X 329
    #define MMIDC_N_IN_ONE_H_Y 184
    
    #define MMIDC_N_IN_ONE_V_X 5
    #define MMIDC_N_IN_ONE_V_Y 344
#else
    #define MMIDC_N_IN_ONE_IMG_WIDTH  32
    #define MMIDC_N_IN_ONE_IMG_HEIGHT 24
    
    #define MMIDC_N_IN_ONE_H_X 249
    #define MMIDC_N_IN_ONE_H_Y 184
    
    #define MMIDC_N_IN_ONE_V_X 5
    #define MMIDC_N_IN_ONE_V_Y 264
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


typedef struct
{
    uint8 cur_index;
    DC_IMG_SIZE_T img_size;
    MMIDC_BUFFER_DATA_T osd_data[MMIDC_N_IN_ONE_TOTAL_NUM];
}MMIDC_N_IN_ONE_OSD_INFO_T;

typedef struct
{
    MMIDC_BUFFER_DATA_T photo_data;
    DC_IMG_SIZE_T img_size;
}MMIDC_N_IN_ONE_PHOTO_INFO_T;

typedef struct
{
    BOOLEAN is_enable;
    IMGPROC_HANDLE handle;
    uint8 total_num;
    uint8 hor_num; //horizontal number
    MMIDC_N_IN_ONE_OSD_INFO_T osd_info;
    MMIDC_N_IN_ONE_PHOTO_INFO_T photo_info;
    IMG_PROC_NINONE_T n_in_one_param;
    GUI_POINT_T start_point;
	BOOLEAN is_start_display_frame;
}MMIDC_N_IN_ONE_INFO_T;



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//Description : check if four in one function opened
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsNInOneEnable(void);

/*****************************************************************************/
//Description : display n in one image
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DisplayNInOne(void);

/*****************************************************************************/
//Description : get n in one start point
//Global resource dependence : 
//Author: jinju.ma
//Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMIDC_GetNInOneRectStartPoint(void);
/*****************************************************************************/
//  Description : open n in one
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenNInOne(void);

/*****************************************************************************/
//  Description : close n in one
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseNInOne(void);

/*****************************************************************************/
//Description : get n in one image process parameter
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetNInOneImageProcessParam(MMIDC_IMAGE_PROCESS_PARAM_T *img_param_ptr /*out*/
                                        );
/*****************************************************************************/
//Description : get n in one image number
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint8 MMIDC_GetNInOneImageNumber(void);

/*****************************************************************************/
//Description : set n in one start point
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNInOneStartPoint(GUI_POINT_T point);

/*****************************************************************************/
//Description : get n in one display width and height
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetNInOneDisplayWidthAndHeight(int16 *width, int16 *height);

/*****************************************************************************/
//Description : is n in one start display frame
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsNInOneStartDisplayFrame(void);

/*****************************************************************************/
//Description : set n in one start display frame
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNInOneStartDisplayFrame(BOOLEAN flag);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //#if defined(MMIDC_F_N_IN_ONE)

#endif //#ifndef _MMIDC_NINONE_H_




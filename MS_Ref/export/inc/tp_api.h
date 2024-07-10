/******************************************************************************
 ** File Name:      tp_main.h                                                *
 ** Author:         tao.feng                                            *
 ** DATE:           06/10/2008                                              *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic data structure for Touch Panel module  *
 **                                                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/10/2008     tao.feng         Create.                                  *                                              *
 ******************************************************************************/

#ifndef _TP_API_H
#define _TP_API_H

#include "sci_types.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

/* calibration flag */
#define CALIBRATION_FLAG                        0x5555aaaa

//@vine.yuan 2010.1.5
typedef enum _TP_MODE_
{
    TP_MODE_NORMAL = 0,
    TP_MODE_XW,
    TP_MODE_YW,
    TP_MODE_XYW,
    TP_MODE_XYE,
    TP_MODE_XYE_XW,
    TP_MODE_XYE_YW,
    TP_MODE_XYE_XYW,
    TP_MODE_MAX
}TP_MODE_E;

typedef struct
{   
	uint16 x_adc;  /*x adc*/
	uint16 y_adc;  /*y adc*/ 
}TP_ADC_T; 

typedef struct TP_CALIBRATION_TAG
{    //struct about calibraton       //@David.Jia    2005.12.15
    uint32 is_valid;            //toutch panel is calibrated effectually ok is 0x5555aaaa
    uint16 x_top_factor;      //(x_max_top-x_min_top)/TP_WIDTH*1000
    uint16 x_top_constant;    //    added by feng tao 
    uint16 x_bottom_factor;  //(x_max_bottom-x_min_bottom)/TP_WIDTH*1000
    uint16 x_bottom_constant;    //    added by feng tao 
    uint16 y_left_factor;        //(y_max_left-y_min_left)/TP_HEIGHT*1000
    uint16 y_left_constant;    //    added by feng tao 
    uint16 y_right_factor;   //(y_max_right-y_min_right)/TP_HEIGHT*1000
    uint16 y_right_constant;    //    added by feng tao 
    uint16 x_min_top;            //top panel min x adc value     
    uint16 x_max_top;          //top panel max x adc value
    uint16 x_min_bottom;       //bottom panel min x adc value
    uint16 x_max_bottom;       //bottom panel max x adc value
    uint16 y_min_left;         //left panel min y adc value
    uint16 y_max_left;        //left panel max y adc value
    uint16 y_min_right;        //right panel min y adc value        
    uint16 y_max_right;        //right panel min y adc value        
    uint16 x_center;         //center point x adc value
    uint16 y_center;         //center point y adc value
    uint16 x_min_up;           //x adc value below it as pen up detect
    uint16 y_min_up;           //y adc value below it as pen up detect

    int32 x_slope;
    int32 x_offset;
    int32 y_slope;
    int32 y_offset;
}TP_CALIBRATION_T;

typedef struct _liear_obj
{
   int32 slope;    //
   int32 offset;   //
}  LINEAR_OBJ_T, *LINEAR_OBJ_PTR;


typedef struct _tp_info_tag
{
    uint16 tp_width;
    uint16 tp_height;
    uint16 tp_lcd_height;
    uint16 dummy_1;
    uint16 adc_min_x;
    uint16 adc_min_y;
    uint16 adc_max_x;
    uint16 adc_max_y;
    uint16 exchange_x;      /* xl and xr exchanged */    
    uint16 exchange_y;      /* yd and yu exchanged */    
    uint16 exchange_xy;    /* xl and yd exchanged */        
}TP_INFO_T;

PUBLIC void TP_SetRawDataMode(BOOLEAN is_raw_mode);
PUBLIC TP_INFO_T* TP_GetInfo(void);
PUBLIC void TP_Disable(void);
PUBLIC void TP_Enable(void);
PUBLIC BOOLEAN TP_UpdateCalibration(TP_CALIBRATION_T * param);

#ifdef __cplusplus
    }
#endif

#endif


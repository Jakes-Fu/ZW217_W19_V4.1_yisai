/******************************************************************************
 ** File Name:     tpd_drv.h                                                *
 ** Author:         gang.tong                                             *
 ** DATE:           02/05/2007                                                *
 ** Copyright:     2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:   This file defines the basic operation interfaces of       *
 **                      touch panel device.                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/05/2007     gang.tong         Create.                                  *
 ******************************************************************************/

#ifndef _TPD_DRV_WIN32_SIMU_H
#define _TPD_DRV_WIN32_SIMU_H

/**----------------------------------------------------------------------------*
 **  compare switch
 **-----------------------------------------------------------------------------*/

//#define NV_WR_OK

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"


#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define TPDDRV_DEBUG
#ifdef TPDDRV_DEBUG
    #define TPDRV_PRINT	SCI_TRACE_LOW
#else
    #define TPDRV_PRINT	
#endif

#define     CALIBRATION_FLAG        0x5555aaaa

#define     TP_MEASURE_COUNT        1           //sampling number of eatch ADC chanel 

/**---------------------------------------------------------------------------*/
//                               struct dfeine
/**---------------------------------------------------------------------------*/
typedef struct TP_DESHADING_TAG{//struct about toutch filter
    uint32 ct;                  //conter of data collect
    uint16 x[TP_MEASURE_COUNT];
    uint16 y[TP_MEASURE_COUNT];
}TP_DESHADING_T;
//typedef enum TP_STATE_ENUM{//struct of pen state
//    TP_NONE_S,
 //   TP_UP_S,
 //   TP_DOWN_S
//}TP_STATE_E;
typedef struct TP_PIPE_NODE_TAG{
	uint32 num;
	uint32 valu;
	struct TP_PIPE_NODE_TAG  *next;
}TP_PIPE_NODE_T;

typedef struct TP_CALIBRATION_TAG {	//struct about calibraton       //@David.Jia    2005.12.15
    uint32       is_valid;    		//toutch panel is calibrated effectually ok is 0x5555aaaa
    uint16       x_top_factor;      //(x_max_top-x_min_top)/TP_WIDTH*1000
    uint16 	      x_bottom_factor;  //(x_max_bottom-x_min_bottom)/TP_WIDTH*1000
    uint16       y_left_factor;		//(y_max_left-y_min_left)/TP_HEIGHT*1000
    uint16 	      y_right_factor;   //(y_max_right-y_min_right)/TP_HEIGHT*1000
    uint16       x_min_top;			//top panel min x adc value     
    uint16      x_max_top;          //top panel max x adc value
    uint16      x_min_bottom;       //bottom panel min x adc value
    uint16      x_max_bottom;   	//bottom panel max x adc value
    uint16      y_min_left;         //left panel min y adc value
    uint16       y_max_left;		//left panel max y adc value
    uint16       y_min_right;		//right panel min y adc value		
    uint16      y_max_right;        //right panel min y adc value		
    uint16	      x_center;         //center point x adc value
    uint16	      y_center;         //center point y adc value
    uint16      x_min_up;           //x adc value below it as pen up detect
    uint16      y_min_up;           //y adc value below it as pen up detect
}TP_CALIBRATION_T;

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
	
	uint16 exchange_x;
	uint16 exchange_y;	

}TP_INFO_T;

#endif
/******************************************************************************
 ** File Name:      Tp_cfg.h                                                 *
 ** Author:         changde.li                                                *
 ** DATE:           06/10/2011                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic structure of TP's configuration*
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/10/2011                                                                *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#ifndef _TP_CFG_H_
#define _TP_CFG_H_

#include "sci_types.h"



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/

//Copy from tpc_phy.h
typedef float FLOAT_TYPE;


#define MSG_TP_WIDTH            0x200
#define MSG_TP_HEIGHT           0x201
#define MSG_TP_REF_RES          0x202
#define MSG_TP_REF_UOL          0x203
#define MSG_TP_DPOINT_XLIMIT    0X204
#define MSG_TP_DPOINT_YLIMIT    0X205
#define MSG_TP_SPOINT_RT_MAX    0X206
#define MSG_TP_SPOINT_RT_MIN    0X207

#define MSG_TP_DPOINT_RT_MAX    0X208
#define MSG_TP_DPOINT_RT_MIN    0X209
#define MSG_TP_DPOINT_ENABLE    0X210

#define	TPC_PAD_RES_200OHM		200
#define	TPC_PAD_RES_400OHM		400
#define	TPC_PAD_RES_600OHM		600
#define	TPC_PAD_RES_800OHM		800

//#define	TPC_MAX_ADC_VAL			1023
#define	TPC_MAX_ADC_VAL			4095
/*
    Data to be watched in debug window.
*/
#define WATCH_DELTA_X   0
#define WATCH_DELTA_Y   1
#define WATCH_RT        2
#define WATCH_ARRAY_SIZE 8
extern FLOAT_TYPE g_watch_data[WATCH_ARRAY_SIZE];

/******************** Touch panel Physical factor *****************************/

/**---------------------------------------------------------------------------*
 **                      DATA STRUCTURE Definition                            *
 **---------------------------------------------------------------------------*/

 // define configuration for double-point tpc
typedef struct _TP_DP_CFG_T
{
    uint32 tp_width;                  //tp_width, Unit:mm            
    uint32 tp_height;                 //tp_height

    uint32 ref_r;                     //reference_resistor, Unit:ohm
    uint32 ref_v;                     //reference_voltage

    FLOAT_TYPE dpoint_x_limit;        //Unit:mm
    FLOAT_TYPE dpoint_y_limit;        //
    
    uint32 spoint_rt_max;             //Unit:ohm
    uint32 dpoint_rt_max; 
    uint32 dpoint_rt_min;
    
    uint32 data_period;               //uint:ms
    uint32 exchange_xy;               //
} TP_DP_CFG_T, *TP_DP_CFG_PTR;

 // define in tp configure layer
typedef struct _TP_CFG_INFO_T
{
    uint32 exchange_x;                //If XL1<->XR2,XR1<->XL2    
	uint32 exchange_y;                //If YU1<->YD2,YD1<->YU2
	uint32 exchange_xy;               //If (XL1,XR1)<=>(YU2,YD2) and (YU1,YD1)<=>(XL2,XR2)

	 int32 delta_data_limit;          //The sum of delta_x and delta_y between two points,
	                                  //If the sum is big than the limit_value, the point will be discarded.
	uint32 pressure_factor_limit;     //If pressure is less than this limit, the points will be recongnized as a invalidate data.        
	uint32 data_period;               //The period of sending data to TP server.     

    TP_DP_CFG_T* dpoint_cfg_ptr;      //DoblePoint custom configuration
} TP_CFG_INFO_T, *TP_CFG_INFO_PTR;

/*****************************************************************************/
//  Description:    This function is used to get TP configuration
//  Author:         Changde.li
//  Note:           
/*****************************************************************************/
PUBLIC TP_CFG_INFO_PTR TP_GetCfgInfo( void );

PUBLIC void   TP_SetParam(uint32 msg_id, uint32 data);
PUBLIC uint32 TP_GetParam(uint32 msg_id);    
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of tp_cfg.H

#endif  // End of _TP_CFG_H_



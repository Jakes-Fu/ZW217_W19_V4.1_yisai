/******************************************************************************
 ** File Name:      Tp_cfg.c                                                 *
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

#include "Tp_cfg.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifndef COMPILER_RVCT
#undef __MODULE__ 
#define __MODULE__ "Tp_cfg.c"
#endif

/**---------------------------------------------------------------------------*
 **                         Macro define                                      *
 **---------------------------------------------------------------------------*/
/******************** Touch panel ADC value CFG *******************************/

/************************** EXCHANGE X/Y *************************************/
/* Not used. */
#define  EXCHANGE_X       0
#define  EXCHANGE_Y       0
#define  EXCHANGE_XY      0
#define TPC_DELTA_DATA                  15
#define TPC_PRESSURE_FACTOR             100

/************************** other factor *************************************/
/* 
   Customer Configuration: according to the special product. 
   You can modify this parameter to improve the tp characters.
*/
#define  TP_FILTER_TRACE_COUNT       12
#define  DPOINT_DITHER_LEN           3
#define  TP_SMOOTH_WIN_SIZE          0

#define  MOVING_X_LIMIT  6 
#define  MOVING_Y_LIMIT	 10

#define  FLYINGSPOT_X_LIMIT  50  
#define  FLYINGSPOT_Y_LIMIT	 60

#define TPC_DATA_PERIOD              12

/**---------------------------------------------------------------------------*
 **                         Struct Define                                     *
 **---------------------------------------------------------------------------*/
//For New Tp RefPhone V1.0.1
LOCAL TP_DP_CFG_T s_tp_dp_cfg =
{
    48,   //tp_width, Unit:mm            
    36,   //tp_height

    800,  //reference_resistor, Unit:ohm
    3823,  //reference_voltage

    15,   //dpoint_x_limit, Unit:mm
    10,   //dpoint_y_limit
    
    1200, //spoint_rt_max
    900,   //dpoint_rt_max
    100,   //dpoint_rt_min
    
	20,   //data_period
	0     //xy_exchange   
};

LOCAL TP_CFG_INFO_T s_tp_cfg_info =
{
    EXCHANGE_X,
    EXCHANGE_Y,
    EXCHANGE_XY,
    TPC_DELTA_DATA,
    TPC_PRESSURE_FACTOR,
    TPC_DATA_PERIOD,
    &s_tp_dp_cfg,
};

/*
//EVB hx8347d 320*240
LOCAL TP_DP_CFG_T s_tp_dp_cfg =
{
    45,   //tp_width, Unit:mm            
    64,  //tp_height

    550,  //reference_resistor, Unit:ohm
    963,  //reference_voltage

    9,   //dpoint_x_limit, Unit:mm
    15,   //dpoint_y_limit
    
    1500, //spoint_rt_limit
    150,   //dpoint_rt_limit

	20,   //data_period
};
*/
/**---------------------------------------------------------------------------*
 **                     Pblic Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to get TP configuration
//  Author:         Changde.li
//  Note:           
/*****************************************************************************/
PUBLIC TP_CFG_INFO_PTR TP_GetCfgInfo( void )
{
    return (TP_CFG_INFO_PTR)(&s_tp_cfg_info);
}


PUBLIC void TP_SetParam(uint32 msg_id, uint32 data)
{
    switch( msg_id )
    {
        case MSG_TP_WIDTH:
			s_tp_dp_cfg.tp_width =data;
			break;
		case MSG_TP_HEIGHT:
			s_tp_dp_cfg.tp_height= data;
			break;
		case MSG_TP_REF_RES:
			s_tp_dp_cfg.ref_r= data;
			break;
		case MSG_TP_REF_UOL:
			s_tp_dp_cfg.ref_v= data;
			break;
		case MSG_TP_DPOINT_XLIMIT:
			s_tp_dp_cfg.dpoint_x_limit= data;
			break;
		case MSG_TP_DPOINT_YLIMIT:
			s_tp_dp_cfg.dpoint_y_limit= data;
			break;
		case MSG_TP_SPOINT_RT_MAX:
			s_tp_dp_cfg.spoint_rt_max= data;
			break;			
		case MSG_TP_DPOINT_RT_MAX:
			s_tp_dp_cfg.dpoint_rt_max= data;
			break;					
		case MSG_TP_DPOINT_RT_MIN:
			s_tp_dp_cfg.dpoint_rt_min= data;
			break;					
		default:
			break;
	}	
}

PUBLIC uint32 TP_GetParam(uint32 msg_id)
{
    uint32 result=0;
    
    switch( msg_id )
    {
        case MSG_TP_WIDTH:
			result =s_tp_dp_cfg.tp_width;
			break;
		case MSG_TP_HEIGHT:
			result =s_tp_dp_cfg.tp_height;
			break;
		case MSG_TP_REF_RES:
			result =s_tp_dp_cfg.ref_r;
			break;
		case MSG_TP_REF_UOL:
			result=s_tp_dp_cfg.ref_v;
			break;
		case MSG_TP_DPOINT_XLIMIT:
			result =(uint32)(s_tp_dp_cfg.dpoint_x_limit);
			break;
		case MSG_TP_DPOINT_YLIMIT:
			result =(uint32)(s_tp_dp_cfg.dpoint_y_limit);
			break;
		case MSG_TP_SPOINT_RT_MAX:
			result =(uint32)(s_tp_dp_cfg.spoint_rt_max);
			break;			
		case MSG_TP_DPOINT_RT_MAX:
			result =(uint32)(s_tp_dp_cfg.dpoint_rt_max);
			break;					
		case MSG_TP_DPOINT_RT_MIN:
			result =(uint32)(s_tp_dp_cfg.dpoint_rt_min);
			break;	
		default:
			result =111;
			break;
	}
    return (result);
}
/******************************************************************************
 ** File Name:      dualbat_prod_cfg.c                                        *
 ** Author:         Mingwei.Zhang                                             *
 ** DATE:           21/06/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic dual battery operation process.*
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME                     DESCRIPTION                  *
 ** 21/06/2011          Mingwei.Zhang            Create.                      *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "dualbat_drvapi.h"
#include "dualbat_prod.h"

LOCAL const DBAT_PROD_CFG_T	dbat_cfg_tab = 
{	
    {	    //DBAT_DISCHARGE_PARAM_T
        3640,                       //warning voltage 3.64v
        3500,	                    //shutdown voltage 3.35v
        3000,	                    //deadline voltage 3.1v
        24,	                        //warning count
        3600,	                    //discharge switch voltage 3.6V
        100		                    //switch gate 100mV
    },
    {		//DBAT_CHARGE_PARAM_T
        4160,                       //recharge		4.16V
        4210,	                    // charge end	4.21V
        4330,                       //bat_safety_vol
        {400,400},	                    //standard charge current;0:400mA,1:600mA;main--400mA and aux--400mA
        {300,300},                      //usb charge current;0:300mA,1:400mA,2:500mA;main--300mA and aux--300mA
        {300,300},                         //nonstandard current 
        18000,                      //chg_timeout
        3400,                      //charge switch,3.3V
        100,	                   //chg_switch_gate	100mV
        4150	                   //idle vbat full	4.15
    },
    {       //DBAT_OVP_PARAM_T
        0,                          //	ovp_type;
        6500,                       //	ovp_over_vol;
        6000,                       //  ovp_resume_vol;
    },
    {   //DBAT_OTP_PARAM_T
    	1,                          // otp_type;	///0:don't support,1:environment temp,2:vbat temp	
    	995,                        // over_low;    -5C,otp_over_low = 1000 + temperatue,low limit temperature
    	1063,                       // over_high;   63C,otp_over_high = 1000 + temperatue,high limit temperature
    	1000,                       // resume_low;  0C,otp_resume_low = 1000 + temperatue,low resume temperatue after over temperature occur
    	1058,                       // resume_high; 58C,otp_resume_high = 1000 + temperatue,high resume temperatue after over temperature occur
    },
        
    {        //DBAT_BATDETECT_PARAM_T	
        DBAT_BAT_DETECT_ADC,        //detect_mode
        2000,                       //detect_vol 1.0V	
        10	                        //detect_vol_gate  	
    },
    {        //DBAT_HW_PARAM_T	
        {DBAT_ADC_CHANNEL_MAIN,DBAT_ADC_CHANNEL_AUX},    //adc_channel[DBAT_MAX_BAT]		
        {800,800},                  //bat_cap[DBAT_MAX_BAT] reserved
        150,                        //adc_resistor_vbat 150K	
        51,                         //adc_resistor_gnd 51K	
        DBAT_BIDIRECTIONAL_CHG,     //mutual_chg_mode	
        DBAT_UNKNOW_BATTERY_MODE,   //prod_type reserved
        DBAT_DISCHG_M_CHG_A	        //work_mode	
    },
};

/*****************************************************************************/
//  Description:    Get product config table,when system power on.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC DBAT_PROD_CFG_T * DBAT_Prod_GetCfg(void)
{
	return (DBAT_PROD_CFG_T *)&dbat_cfg_tab;
}



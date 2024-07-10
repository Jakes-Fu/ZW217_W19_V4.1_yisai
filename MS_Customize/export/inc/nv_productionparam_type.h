/******************************************************************************
 ** File Name:      nv_productionparam_type.h                                               *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           30/12/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contain some NV production parameters.                     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 30/12/2004     Benjamin.Wang    Create.                                   *
 ******************************************************************************/
#ifndef _NV_PRODUCTIONPARAM_TYPE_H
#define _NV_PRODUCTIONPARAM_TYPE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h" 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define BAT_TABLE_SIZE    	(13) 
#define PROD_HW_CONFIG          (20)
#define CHR_PARAM_RESERVE_SIZE 	(48 -(PROD_HW_CONFIG + 2*BAT_TABLE_SIZE ))

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

// All the voltage unit is mV
typedef struct
{
    struct dischg_param_t
    {
        uint16 voltage_warning;     // when low than the voltage, send the warning msg to client.
        uint16 voltage_shutdown;    // when low than the voltage, send the shutdown msg to client.
        uint16 voltage_deadline;    //adjust_backlight;    //
        uint16 warning_count;  		// when reach the warning count, will send the voltage warning to client.    		
    } dischg_param;
    struct chg_param_t
    {
        uint16 recharge_voltage;
        uint16 charge_end_voltage;
        uint16 bat_safety_vol;
        uint16 standard_current_type;      //standard adapter charge current type
        uint16 usb_current_type;           //pc charge current type
        uint16 nonstandard_current_type;    //nonstandard adapter charge current select,0:usb 1:standard adapter
        uint16 chg_timeout;
    }chg_param;	
    struct ovp_param_t
    {
        uint16 ovp_type;       //ovp type
        uint16 ovp_over_vol;
        uint16 ovp_resume_vol;
    }ovp_param;
    struct
    {
    	uint16 otp_type;	///0:don't support,1:environment temp,2:vbat temp	
    	uint16 over_low;
    	uint16 over_high;
    	uint16 resume_low;
    	uint16 resume_high;
    }otp_param;
    uint16 hw_switch_point;		//set the lowest hardware switchover point between cc-cv    
    // The table define as following:
    //  If the voltage >= the low WORD of table[n], the capacity is the high WORD of table[n].
    // The last one item of the table must be 0xFFFFFFFF
    uint16 dischg_bat_capacity_table[ BAT_TABLE_SIZE ];
    uint16 chg_bat_capacity_table[ BAT_TABLE_SIZE ];
    uint16 reseved[CHR_PARAM_RESERVE_SIZE]; 
}CHGMNG_PARAM_PROD_T;   // Size is 112 bytes

typedef struct prod_peripherial_tag
{
    uint16 main_lcd_index;
    uint16 sub_lcd_index;
    uint16 main_sensor_index;
    uint16 sub_sensor_index;
    uint16 fm_index;
    uint16 af_index;
    uint16 sd_card_slot;	//0:T Flash; 1:mini SD; 2 standard SD(MMC);0xFX("X"´ú±íSPI CSºÅ)
	uint16 gps_flag;   		//BIT_0: 1--have download SiRF firmware, other--not downloaded.
}PROD_PERIPHERAL_T;

typedef struct
{
    uint32 tuning_step;     //step between tunings
    uint32 tuning_interval; //delay between tuning steps(uint: ms)
}ANA_DEV_PARAM_PROD_T;

typedef struct PROD_PARAM_tag
{
    CHGMNG_PARAM_PROD_T chr_param;          //96bytes
    PROD_PERIPHERAL_T   peripheral_param;   //16bytes
    ANA_DEV_PARAM_PROD_T lcm_param; // 8 bytes
    ANA_DEV_PARAM_PROD_T kpd_param; // 8 bytes
} PROD_PARAM_T;

typedef enum
{
	PROD_NV_ID_MAIN_LCD = 0x00,
	PROD_NV_ID_SUB_LCD,
	PROD_NV_ID_MAIN_SENSOR,
	PROD_NV_ID_SUB_SENSOR,
	PROD_NV_ID_FM,
	PROD_NV_ID_AUTO_FOCUS,
	PROD_NV_ID_SD_SLOT,
	PROD_NV_ID_GPS,

	PROD_NV_ID_MAX
	
}PROD_NV_PERIPHERAL_ID_E;

/*****************************************************************************/
//  Description:    This function get peripheral nv parameter.
//  Author:         Liangwen.Zhen
//	Note:           
/*****************************************************************************/
PUBLIC uint16 PROD_GetPeripheralNVParam(PROD_NV_PERIPHERAL_ID_E id);

/*****************************************************************************/
//  Description:    This function set peripheral nv parameter.
//  Author:         Liangwen.Zhen
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN PROD_SetPeripheralNVParam(
					PROD_NV_PERIPHERAL_ID_E id,
					uint16 					param
				);
    
/*****************************************************************************/
//  Description:    The function gets charge nvitem parameters.
//  Global resource dependence: 
//  Author:         Benjamin.Wang
//  Note:           
/*****************************************************************************/
PUBLIC CHGMNG_PARAM_PROD_T* PROD_GetChargeParam(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _NV_PRODUCTIONPARAM_TYPE_H

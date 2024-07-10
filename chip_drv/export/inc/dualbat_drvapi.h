/******************************************************************************
 ** File Name:      dualbat_drvapi.h                                          *
 ** Author:         Mingwei.Zhang                                             *
 ** DATE:           21/06/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 dual battery module.                                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 21/06/2011     Mingwei.Zhang     Create.                                  *
 ******************************************************************************/

#ifndef _DUALBAT_DRVAPI_H_
#define _DUALBAT_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "chg_drvapi.h"

#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
#define DETECT_WAIT_VOL_DROP_TIME   1000

/**---------------------------------------------------------------------------*
 **                         Type Definations                                  *
 **---------------------------------------------------------------------------*/

typedef enum {
    DBAT_CHARGER_PLUGIN_MSG = 1,
    DBAT_CHARGER_PLUGOUT_MSG,
    DBAT_M_BAT_PLUGOUT_MSG,
    DBAT_M_BAT_PLUGIN_MSG,
    DBAT_A_BAT_PLUGOUT_MSG,
    DBAT_A_BAT_PLUGIN_MSG,

    DBAT_SWITCH_BAT_REQ_MSG,

    DBAT_BAT_MONITOR_MSG,
    DBAT_CHR_CHARGE_START_IND,   
    DBAT_CHR_CHARGE_END_IND,     
    DBAT_CHR_WARNING_IND,       
    DBAT_CHR_SHUTDOWN_IND,      
    DBAT_CHR_CHARGE_FINISH,      
    DBAT_CHR_CHARGE_DISCONNECT,  
    DBAT_CHR_CHARGE_FAULT,       
    DBAT_CHR_CHARGE_RESETDONE,
    DBAT_MAX_MSG
}DBAT_MSG_E;

typedef enum
{
    DBAT_UPDATE_STATE_IND = 0x1,
        
    DBAT_BAT_PLUGIN_IND,
    DBAT_BAT_PLUGOUT_IND,

    DBAT_SWITCH_BAT_IND,

    DBAT_CHARGE_START_IND, 
    DBAT_WARNING_IND,     
    DBAT_SHUTDOWN_IND,       
    DBAT_CHARGE_FINISH,      
    DBAT_CHARGE_DISCONNECT,  
    DBAT_CHARGE_FAULT,
    DBAT_CHARGERCONNECT_VOL_VERYLOW,
    DBAT_MSG_MAX_NUM
}DBAT_MSG_SERVICE_E;

typedef enum
{
	DBAT_MAIN_BAT,
	DBAT_AUX_BAT,
	DBAT_MAX_BAT
}DBAT_BAT_E;

typedef enum
{
    DBAT_DISCONNECT = 0,
    DBAT_CONNECT_CHARGING,
    DBAT_CONNECT_DISCHARGING,
    DBAT_EXCEPTION,
}DBAT_STATE_E;

//D:discharge,C:charge,N:non assign,M:main,A:AUX
typedef enum
{
    DBAT_DISCHG_M_CHG_M =   0x00,
    DBAT_DISCHG_M_CHG_A =   0x01,   //main battery discharge first,and aux battery charge first.
    DBAT_DISCHG_M_CHG_N =   0x02,
    DBAT_DISCHG_N_CHG_M =   0x20,
    DBAT_DISCHG_N_CHG_N =   0x22,   //charge and dischagre don't assign
}DBAT_WORK_MODE_E;

typedef enum
{
    DBAT_ONE_BATTERY_BUILT_IN,
    DBAT_TWO_BATTERY_EQUALITY,
    DBAT_ONE_BATTERY_COVER,
    DBAT_UNKNOW_BATTERY_MODE,
}DBAT_PROD_TYPE_E;

typedef enum
{
    DBAT_BIDIRECTIONAL_CHG,         //support bidirectional charge
    DBAT_UNIDIRECTIONAL_M_TO_A,     //only MAIN charge AUX is allowed
    DBAT_UNIDIRECTIONAL_A_TO_M,     //only AUX charge MAIN is allowed
    DBAT_NOT_SUPPORT_MUTUAL,        //don't support mutual charge
}DBAT_MUTUAL_CHG_MODE_E;

typedef enum
{
    DBAT_BAT_DETECT_INT,        //detect battery through INT mode
    DBAT_BAT_DETECT_ADC,        //detect battery through ADC mode
}DBAT_BATDETECT_MODE_E;


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct
{
    uint32  vol;
    uint32  remain_cap;
}DBAT_BAT_INFO_T;

typedef struct
{
    uint16  warning_vol;
    uint16	shutdown_vol;
    uint16  deadline_vol;
    uint16	warning_count;
    uint16	dischg_switch_vol;
    uint16  dischg_switch_gate;
}DBAT_DISCHARGE_PARAM_T;

typedef struct		
{
    uint16	rechg_vol;
    uint16	chg_end_vol;
    uint16  bat_safety_vol;
    uint16	standard_chg_current[DBAT_MAX_BAT];
    uint16	usb_chg_current[DBAT_MAX_BAT];
    uint16	nonstandard_chg_current[DBAT_MAX_BAT];
    uint16  chg_timeout;
    uint16	chg_switch_vol;
    uint16	chg_switch_gate;
    uint16	idle_bat_full_vol;	
}DBAT_CHARGE_PARAM_T;

typedef struct
{
    uint16	ovp_type;
    uint16	ovp_over_vol;
    uint16  ovp_resume_vol;
}DBAT_OVP_PARAM_T;

typedef struct ////mingwei
{
	uint16 otp_type;	///0:don't support,1:environment temp,2:vbat temp	
	uint16 over_low;
	uint16 over_high;
	uint16 resume_low;
	uint16 resume_high;
}DBAT_OTP_PARAM_T;

typedef struct
{
    DBAT_BATDETECT_MODE_E	detect_mode;
    uint32	                detect_vol;
    uint32	                detect_vol_gate;
}DBAT_BATDETECT_PARAM_T;

typedef struct
{
    uint32                  adc_channel[DBAT_MAX_BAT];
    uint32                  bat_cap[DBAT_MAX_BAT];
    uint32                  adc_resistor_vbat;
    uint32                  adc_resistor_gnd;
    DBAT_MUTUAL_CHG_MODE_E  mutual_chg_mode;
    DBAT_PROD_TYPE_E        prod_type;
    DBAT_WORK_MODE_E        work_mode;
}DBAT_HW_PARAM_T;

typedef struct
{
    DBAT_DISCHARGE_PARAM_T  dischg_param;
    DBAT_CHARGE_PARAM_T     chg_param;
    DBAT_OVP_PARAM_T        ovp_param;
    DBAT_OTP_PARAM_T        otp_param;
    DBAT_BATDETECT_PARAM_T  detct_param;
    DBAT_HW_PARAM_T         hw_param;
}DBAT_PROD_CFG_T;

typedef struct
{
    void                    (*Init)(void);                  //Callback when system power on
    void		            (*PowerOff)(void);              //Callback when system power off,reserved
    BOOLEAN                 (*MutualCharge)(DBAT_BAT_E need_chg_bat,BOOLEAN on); //DBAT_MAX_BAT:all battery need on all off
    uint32                  (*GetBatADC)(DBAT_BAT_E bat);   //Get ADC value of battery voltage through ADC module API
    BOOLEAN                 (*SelectBat)( DBAT_BAT_E bat);  //select one battery to be used.
    BOOLEAN	                (*GetBatPresent)( DBAT_BAT_E bat);  //Query battery present state,reserved
    DBAT_BAT_E              (*GetCurBat)(void);                 //Get current used battery number,reserved
    void                    (*BatPlugCallBack)(DBAT_BAT_E bat,BOOLEAN plugin);  // Callback when battery plugin or plugout.
    void                    (*ChargeCallBack)(BOOLEAN chg);     //Callback when charge start or stop .
    void                    (*CalibrationModeInit)(void);       //Calibration mode callback when system init.
}DBAT_PROD_FUN_T;

typedef struct	
{
    CHGMNG_STATE_INFO_T     chgmng_info;
    DBAT_STATE_E            module_state;           //dual battery module state
    DBAT_BAT_E              cur_bat;                //current use battery number
    DBAT_PROD_TYPE_E        prod_type;              //product type,reserved
    DBAT_WORK_MODE_E        work_mode;              //dual battery work mode,determine by manufactory
    DBAT_BAT_INFO_T*        p_bat[DBAT_MAX_BAT];    //if p_bat[DBAT_MAIN_BAT] == NULL,it means main battery isn't present
}DBAT_STATE_INFO_T;


/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Get dual battery module information.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC DBAT_STATE_INFO_T* DBAT_GetState(void);

/*****************************************************************************/
//  Description:    Send message to dual battery task.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void DBAT_SendMsgToDBatTask (DBAT_MSG_E msg);

/*****************************************************************************/
//  Description:    Set battery calibration data.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void DBAT_SetBatAdcCal(DBAT_BAT_E bat, CHGMNG_ADC_CAL_T *cal_info);

/*****************************************************************************/
//  Description:    dual battery module Init function,be called when system setup.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void DBAT_Init(DBAT_PROD_FUN_T* p_fun, DBAT_PROD_CFG_T* p_param);

#ifdef __cplusplus
}
#endif

#endif  // _DUALBAT_DRVAPI_H_


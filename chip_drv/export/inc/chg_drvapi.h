/******************************************************************************
 ** File Name:      chg_drvapi.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           24/11/2004                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 charger.                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 24/11/2004     Benjamin.Wang     Create.                                  *
 ******************************************************************************/

#ifndef _CHG_DRVAPI_H_
#define _CHG_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sci_api.h"
#include "adc_drvapi.h"

#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
//#define CHG_SHARP_FEATRUE
//#define CHGMNG_PSE_SUPPORT

#define CAL_TYPR_NO 0
#define CAL_TYPE_NV 1
#define CAL_TYPE_EFUSE 2

#define CHARGER_CURRENT_300MA   300//136
#define CHARGER_CURRENT_350MA   350//158
#define CHARGER_CURRENT_400MA   400//181
#define CHARGER_CURRENT_450MA   450//204
#define CHARGER_CURRENT_500MA   500//227
#define CHARGER_CURRENT_550MA   550//250
#define CHARGER_CURRENT_600MA   600//272
#define CHARGER_CURRENT_650MA   650//295
#define CHARGER_CURRENT_700MA   700//317
#define CHARGER_CURRENT_750MA   750//340
#define CHARGER_CURRENT_800MA   800//363
//#define CHARGER_CURRENT_850MA   850//385
#define CHARGER_CURRENT_900MA   900//408
//#define CHARGER_CURRENT_950MA   950//430
#define CHARGER_CURRENT_1000MA  1000//453
//#define CHARGER_CURRENT_1050MA  1050//476
#define CHARGER_CURRENT_1100MA  1100//498
#define CHARGER_CURRENT_1200MA  1200//544
#define CHARGER_CURRENT_MAX     1300//589




/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Type Definations                                   *
 **---------------------------------------------------------------------------*/
typedef enum CHGMNG_StateCode_enum
{
    CHGMNG_IDLE = 0,
    CHGMNG_STARTING,
    CHGMNG_CHARGING,
    CHGMNG_PULSECHARGING,
    CHGMNG_STOPPING
} CHGMNG_STATE_E;

typedef enum CHGMNG_StopReason_enum
{
    CHGMNG_INVALIDREASON = 0,
    CHGMNG_CHARGERUNPLUG = 1,
    CHGMNG_TIMEOUT,
    CHGMNG_VBATEND,
    CHGMNG_CHARGEDONE,
    CHGMNG_OVERVOLTAGE,  //add by paul for charge:over voltage
    CHGMNG_OVERTEMP,
    CHGMNG_PESUDO_CHARGERUNPLUG
} CHGMNG_STOPREASON_E;

typedef enum {
    CHGMNG_VBAT_MONITOR_MSG = 1 ,
    CHGMNG_CHARGER_MONITOR_MSG,
    CHGMNG_CHARGER_PLUG_IN_MSG,
    CHGMNG_CHARGER_PLUG_OUT_MSG,
    CHGMNG_MODULE_RESET_MSG,
    CHGMNG_CHARGER_PESUDO_PLUG_OUT_MSG,
#ifdef BATTERY_DETECT_SUPPORT
    CHGMNG_BATTERY_OFF_MSG,
#endif
	CHGMNG_USB_HOST_PLUG_IN_MSG,
    CHGMNG_MAX_MSG
} CHGMNG_MSG_E;

typedef struct CHGMNGSVR_SIG_tag
{
    SIGNAL_VARS
    uint32  sig_param;
}CHGMNGSVR_SIG_T;

//
//the param of the callback.
//   typedef void (* REG_CALLBACK) (uint32 id, uint32 argc, void *argv);

//   id : the id that provided when register.
//   argc : the event; one of CHR_SVR_MSG_SERVICE_E;
//   argv : capacity. you can convert to uint32,
//

// The message notify to MMI
typedef enum
{
    // Charge message.
    CHR_CAP_IND = 0x1,      // Notify the battery's capacity

    CHR_CHARGE_START_IND,   // start the charge process.
    CHR_CHARGE_END_IND,     // the charge ended.

    CHR_WARNING_IND,        // the capacity is low, should charge.
    CHR_SHUTDOWN_IND,       // the capacity is very low and must shutdown.
#ifdef BATTERY_DETECT_SUPPORT
    CHR_BATTERY_OFF_IND,      // the battery is off
#endif

    CHR_CHARGE_FINISH,      // the charge has been completed.
    CHR_CHARGE_DISCONNECT,  // the charge be disconnect
    CHR_CHARGE_FAULT,       // the charge fault, maybe the voltage of charge is too low.
    CHR_MSG_MAX_NUM
} CHR_SVR_MSG_SERVICE_E;


/*This enum defines the lowest switchover point between constant-current and
constant-volatage.*/
typedef enum CHG_CCtoCVSwitchoverPoint_enum
{
    CHG_SWITPOINT_LOWEST = 0x0F,
    CHG_SWITPOINT_1 = 0x0E,
    CHG_SWITPOINT_2 = 0x0D,
    CHG_SWITPOINT_3 = 0x0C,
    CHG_SWITPOINT_4 = 0x0B,
    CHG_SWITPOINT_5 = 0x0A,
    CHG_SWITPOINT_6 = 0x09,
    CHG_SWITPOINT_7 = 0x08,
    CHG_SWITPOINT_8 = 0x07,
    CHG_SWITPOINT_9 = 0x06,
    CHG_SWITPOINT_10 = 0x05,
    CHG_SWITPOINT_11 = 0x04,
    CHG_SWITPOINT_12 = 0x03,
    CHG_SWITPOINT_13 = 0x02,
    CHG_SWITPOINT_14 = 0x01,
    CHG_SWITPOINT_15 = 0x00,
    CHG_SWITPOINT_16 = 0x10,
    CHG_SWITPOINT_17 = 0x11,
    CHG_SWITPOINT_18 = 0x12,
    CHG_SWITPOINT_19 = 0x13,
    CHG_SWITPOINT_20 = 0x14,
    CHG_SWITPOINT_21 = 0x15,
    CHG_SWITPOINT_22 = 0x16,
    CHG_SWITPOINT_23 = 0x17,
    CHG_SWITPOINT_24 = 0x18,
    CHG_SWITPOINT_25 = 0x19,
    CHG_SWITPOINT_26 = 0x1A,
    CHG_SWITPOINT_27 = 0x1B,
    CHG_SWITPOINT_28 = 0x1C,
    CHG_SWITPOINT_29 = 0x1D,
    CHG_SWITPOINT_30 = 0x1E,
    CHG_SWITPOINT_HIGHEST = 0x1F
}
CHG_SWITPOINT_E;

typedef enum
{
    CHGMNG_ADP_UNKNOW,
    CHGMNG_ADP_STANDARD,
    CHGMNG_ADP_NONSTANDARD,
    CHGMNG_ADP_USB,
}
CHGMNG_ADAPTER_TYPE_E;


////temp code for new charge

typedef struct
{
    int32   x0_adc;
    int32   y0_vol;
    int32   slope;
    int32   zoom_in;
    uint32  cal_flag;
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
	//must be placed in this position
	int32   x0_adc_small_scale;
    int32   y0_vol_small_scale;
	int32	slope_small_scale;
#endif
}CHGMNG_ADC_CAL_T;

typedef struct
{
    uint16  warning_vol;
    uint16	shutdown_vol;
    uint16  deadline_vol;
    uint16	warning_count;
}CHGMNG_DISCHARGE_PARAM_T;

typedef struct
{
    uint16	rechg_vol;
    uint16	chg_end_vol;
    uint16  bat_safety_vol;
    uint16	standard_chg_current;
    uint16	usb_chg_current;
    uint16	nonstandard_chg_current;
    uint16  chg_timeout;
}CHGMNG_CHARGE_PARAM_T;

typedef struct
{
    uint16	ovp_type;
    uint16	ovp_over_vol;
    uint16  ovp_resume_vol;
}CHGMNG_OVP_PARAM_T;

#ifdef CHGMNG_PSE_SUPPORT
typedef enum
{
    /*! Battery is cold              */
    PSE_BATTERY_TEMP_REGION__COLD = 0,
    /*! Battery is low               */
    PSE_BATTERY_TEMP_REGION__LOW,
    /*! Battery is normal specific   */
    PSE_BATTERY_TEMP_REGION__SPECIFIC,
    /*! Battery is standard          */
    PSE_BATTERY_TEMP_REGION__STANDARD,
    /*! Battery is high              */
    PSE_BATTERY_TEMP_REGION__HIGH,
    /*! Battery is hot              */
    PSE_BATTERY_TEMP_REGION__HOT,
}CHGMNG_TEMP_REGION_T;

typedef struct
{
    uint16 cold_temp;
    uint16 low_temp;
    uint16 specific_temp;
    uint16 standard_temp;
    uint16 high_temp;
    uint16 hot_temp;
}CHGMNG_TEMP_PARAM_T;

typedef struct
{
    uint16 chg_end_vol;
	uint16 rechg_vol;
	uint16 chg_current;
    CHG_SWITPOINT_E switch_point;
}CHGMNG_PSE_PARAM_T;

typedef struct
{
    uint16 hot_chg_end_vol;
    uint16 hot_rechg_vol;
    uint16 high_chg_end_vol;
    uint16 high_rechg_vol;
    uint16 standard_chg_end_vol;
    uint16 standard_rechg_vol;
    uint16 specific_chg_end_vol;
    uint16 specific_rechg_vol;
    uint16 low_chg_end_vol;
    uint16 low_rechg_vol;
    uint16 cold_chg_end_vol;
    uint16 cold_rechg_vol;
    uint16 hot_chg_current;
    uint16 high_chg_current;
    uint16 standard_chg_current;
    uint16 specific_chg_current;
    uint16 low_chg_current;
    uint16 cold_chg_current;
    uint16 chg_timeout;
    uint16 chg_end_current;
}CHGMNG_PSE_CHARGE_PARAM_T;
#endif

typedef struct ////mingwei
{
	uint16 otp_type;	///0:don't support,1:environment temp,2:vbat temp
	uint16 over_low;
	uint16 over_high;
	uint16 resume_low;
	uint16 resume_high;
}CHGMNG_OTP_PARAM_T;

typedef struct
{
    CHGMNG_STATE_E          chgmng_state;          //charge module state
    uint32                  bat_statistic_vol;          //statistic voltage,
    uint32                  bat_cur_vol;                //current voltage, twinkling value
    uint32                  bat_remain_cap;         //remain battery capacity
    uint32                  charging_current;       //charging current value,reserved
    uint32                  charging_temperature;	///statistic vbat temperature.
    CHGMNG_ADAPTER_TYPE_E   adp_type;               //adapter type when chargeing,reserved
    CHGMNG_STOPREASON_E     charging_stop_reason;
    uint32                  chg_vol;
}CHGMNG_STATE_INFO_T;

typedef enum
{
    CHR_BATT_MIN_VOLTAGE,
    CHR_BATT_MAX_VOLTAGE,
    CHR_BATT_IN_CHARGE,
    CHR_BATT_OUT_OF_CHARGE
}CHR_BATT_STATE_E;


typedef enum
{
    CHG_DEFAULT_MODE = 0,
    CHG_NORMAL_ADAPTER,
    CHG_USB_ADAPTER
}CHG_ADAPTER_MODE_E;

typedef enum
{
    CHG_USB_300MA = 0,
    CHG_USB_400MA = 0x1,
    CHG_USB_500MA = 0x3
}CHG_USB_CHARGE_CURRENT_E;

typedef enum
{
    CHG_NOR_300MA = 0,
    CHG_NOR_400MA = 0x1,
    CHG_NOR_500MA = 0x2,
    CHG_NOR_600MA = 0x3,
    CHG_NOR_800MA = 0x4,
    CHG_NOR_1000MA = 0x5
}CHG_NOR_CHARGE_CURRENT_E;

typedef enum {          //this enum group are onle used to serve FSM in charger module
    CHGMNG_FSM_EVENT_INIT = 0,          //init event
    CHGMNG_FSM_EVENT_START_CHG,         //charger plug in
    CHGMNG_FSM_EVENT_STOP_CHG,          //charging timeout
    CHGMNG_FSM_EVENT_MAX                //don't use
} CHGMNG_FSM_EVENT_E;

#define CHR_BATTERY_NONE_S          0x1    // No battery
#define CHR_BATTERY_NORMAL_S        0x2    // battery is in normal model( not in charge )
#define CHR_BATTERY_CHARGING_S      0x3    // is charging.

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// Check the charge is connect (return TRUE if the charge is connected)
// Author: lin.liu
/*****************************************************************************/
BOOLEAN CHGMNG_IsChargeConnect (void);
/*****************************************************************************/
//  Description:    This function initialize the Charge manager.
//  Return:         CHGMNG_STARTSUCCESS - normal
//                      CHGMNG_STARTINCHARGING - t is charging when CHGMNG_Start
//  Author:         Benjamin.Wang
//  Note:           This module should be initialized after GPIO_Init.
/*****************************************************************************/
PUBLIC uint32  CHGMNG_Init (void);
PUBLIC void  _CHGMNG_EIC_Init (void);


#if 0
PUBLIC uint32 CHGMNG_GetChargeTimerInfo(void);
#endif

/*****************************************************************************/
//  Description:    This function is used to close charge manager.
//  Author:         Benjamin.Wang
//  Note:           It can't be called in int handler!
/*****************************************************************************/
PUBLIC void CHGMNG_Close (void);
/*****************************************************************************/
//  Description:    This function sets charge otp parameter.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC void CHGMNG_SetOtpParam(CHGMNG_OTP_PARAM_T* otp );

/*****************************************************************************/
//  Description:    This function sets the charging parameter.
//  Retrun:
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetChgParam (CHGMNG_CHARGE_PARAM_T* chg);

/*****************************************************************************/
//  Description:    This function gets the charging parameter.
//  Retrun:         recharge_voltage value.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC CHGMNG_CHARGE_PARAM_T CHGMNG_GetChgParam (void);

/*****************************************************************************/
//  Description:    Get the hardware switch point which is from cc to cv.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CHGMNG_GetHWSwitchPoint (void);

/*****************************************************************************/
//  Description:    Set the hardware switch point which is from cc to cv.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetHWSwitchPoint (CHG_SWITPOINT_E value);

/*****************************************************************************/
//  Description:    Set the discharge parameter.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetDischgParam (CHGMNG_DISCHARGE_PARAM_T* dischg);

/*****************************************************************************/
//  Description:    Get the discharge parameter.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC CHGMNG_DISCHARGE_PARAM_T CHGMNG_GetDischgParam (void);

/*****************************************************************************/
//  Description:    Set the vbat calibration table.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetVBatAdcCal(CHGMNG_ADC_CAL_T *cal_info);

/*****************************************************************************/
//  Description:    Set the voltage capacity table.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetVBatCapTable (uint16 *dischg_ptable, uint16 *chg_ptable);

/*****************************************************************************/
//  Description:    This function get the Ovp parameter
//  Retrun:
//  Note:
/*****************************************************************************/
PUBLIC CHGMNG_OVP_PARAM_T CHGMNG_GetOvpParam (void);

/*****************************************************************************/
//  Description:    This function set the Ovp parameter
//  Retrun:
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetOvpParam (CHGMNG_OVP_PARAM_T* ovp);

/*****************************************************************************/
//  Description:    This function is used to set the adapter mode, adapter or usb.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetChargeAdapter (CHGMNG_ADAPTER_TYPE_E mode);

/*****************************************************************************/
//  Description:    This function is used to convert voltage value to ADC value.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CHGMNG_VoltageToAdcvalue (uint16 votage);

/*****************************************************************************/
//  Description:    This function is used to convert ADC value to voltage value.
//  Author:         Benjamin.Wang
//  Note:           big voltage & No voltage ratio
/*****************************************************************************/
PUBLIC uint16 CHGMNG_AdcvalueToVoltage (uint32 adcvalue);

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
/*****************************************************************************/
//  Description:    This function is used to convert ADC value to voltage value.
//  Author:         Tony
//  Note:			small scale & No voltage ratio
/*****************************************************************************/
PUBLIC uint16 CHGMNG_AdcvalueToVoltageforSmallScale (uint32 adc);

/*****************************************************************************/
//  Description:    This function is used to convert ADC value to voltage value.
//  Author:         Tao.shi
//  Note:           support scale choose
/*****************************************************************************/
PUBLIC uint16 CHGMNG_AdcvalueToVoltage_baseScale(uint32 adc_value, ADC_SCALE_TYPE_E scale);

/*****************************************************************************/
//  Description:    convert adc to Vbus voltage.
//  Author:         Tony
//  Note:			have voltage ratio
/*****************************************************************************/
PUBLIC uint32 _CHGMNG_AdcValueToVChgVol(uint32 adc);
#endif

/*****************************************************************************/
//  Description:    This function is used to get VBAT ADC value.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHGMNG_GetVBATADCValue (void);

PUBLIC int32 CHGMNG_GetVbatTempVolValue (void);

PUBLIC int32 CHGMNG_GetNTC_TempVolValue(void);

/*****************************************************************************/
//  Description:    When charger pulgin/unplug interrupt happened, this function is called.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_ChargerPlugInHandler (uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    send massage to charge task.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC void CHGMNG_SendMsgToChgTask(CHGMNG_MSG_E sig,uint32 sig_param);

/*****************************************************************************/
//  Description:    identify charger type.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC CHGMNG_ADAPTER_TYPE_E CHGMNG_IdentifyAdpType(void);

/*****************************************************************************/
//  Description:    Get charge module state information.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC CHGMNG_STATE_INFO_T* CHGMNG_GetModuleState(void);

/*****************************************************************************/
//  Description:    Convert ADCVoltage to percentrum for dual battery.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHGMNG_VoltagetoPercentumDBat (uint32 bat, uint32 voltage,BOOLEAN init);
PUBLIC void CHGMNG_FSM_TAKE_THERMAL(CHGMNG_FSM_EVENT_E event, uint32 condition);
///dummy
uint32 CHR_GetBatCapacity (void);
uint32   CHR_GetBattState (void);
void CHR_StopCharge (void);
void CHR_StartCharge (void);
uint32 CHR_GetVoltage (void);
uint32 CHR_GetCurVoltage (void);
uint32 CHR_SetBattIntRes (uint32 level);
uint32 CHR_SetTPulseOutCharge (uint32 level);
uint32 CHR_SetTPulseInCharge (uint32 level);
uint32 CHR_SetBattLevelMax (uint32 level);
uint32 CHR_SetBattLevelMin (uint32 level);
uint32 CHR_CheckBatteryStaus (void);

#ifdef __cplusplus
}
#endif

#endif  // _CHG_DRVAPI_H_


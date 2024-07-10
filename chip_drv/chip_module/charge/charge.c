/******************************************************************************
 ** File Name:      charge.c                                                 *
 ** Author:         Benjamin.Wang                                                   *
 ** DATE:           24/11/2004                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic charging and power manage operation process.   *
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME                     DESCRIPTION                               *
 ** 24/11/2004      Benjamin.Wang       Create.                                   *
 ** 26/08/2011      Mingwei.Zhang       Modify
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "tasks_id.h"
#include "chg_drvapi.h"
#include "dal_power.h"
#include "adc_drvapi.h"
#include "doidleonpage.h"
#include "charge_phy.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
//#define CHGMNG_OTP_SUPPORT
//#define SC6531_CHG_CUR_SEARCH   //for chip of 6531
#define DEBUG_CHGMNG

#ifdef DEBUG_CHGMNG
#define CHGMNG_PRINT( _format_string )   SCI_TRACE_LOW _format_string
#else
#define CHGMNG_PRINT( _format_string )
#endif

#define   TASK_CHGMNG_SRV_PRI    PRI_CHGMNG_SRV_TASK   //SCI_PRIORITY_NORMAL
/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/
#define CHARGER_NORMAL_TYPE    0
#define CHARGER_NK_TYPE        1

//when the phone is staying in busy state(for example, talking, play games or play music, etc.),we will stop
//the state timer until it is not busy.
#define BUSYSTATE               1   
#define CHARGING_TIMER_INTERVAL 1000

#define VOL_TO_CUR_PARAM	    576			///CC charge current covert voltage,that is 576mV in 8800G and 6800H
#define VBAT_VOL_DIV_P1			266			///voltage divider 0.268,268/1000
#define VBAT_VOL_DIV_P2			1000		///voltage divider 0.268,268/1000  

#define VCHG_DIV_P1             75         ///voltage divider 0.0755,7555/10000
#define VCHG_DIV_P2             1000

#define DISCHG_VBAT_BUFF_SIZE   128
#define CHG_VBAT_BUFF_SIZE      16
#define CHG_CURRENT_BUFF_SIZE   (CHG_VBAT_BUFF_SIZE >> 1)

#define CHGMNG_PLUS_TIMES       3

#define CHG_TEMP_BUFF_SIZE      5		//mingwei
#define CHG_NV_TEMP_OFFSET	    1000	//NV value must be positive,e.g. nv = -5C + CHG_NV_TEMP_OFFSET

#define VBAT_RESULT_NUM         7       //ADC sampling number
#define VPROG_RESULT_NUM        7       //ADC sampling number

#define CHGMNG_STACK_SIZE       (1024*2)
#define CHGMNG_QUEUE_NUM        24

#define OVP_DETECT_TIMES        20
#define OVP_DETECT_VALID_TIMES  10
#define OVP_TIMER_INTERVAL      117

#ifdef CHG_SHARP_FEATRUE
#define ADC_CHANNEL_VCHGBG  9
#define PLUS_TIME_CNT   (60*60)
uint32 plus_cnt = PLUS_TIME_CNT;
#endif

typedef enum {          //this enum group are onle used to serve FSM in charger module
    CHGMNG_FSM_EVENT_INIT = 0,          //init event
    CHGMNG_FSM_EVENT_START_CHG,            //charger plug in
    CHGMNG_FSM_EVENT_STOP_CHG,           //charging timeout
    CHGMNG_FSM_EVENT_PULSE_TO_CHG,       //pulsecharging to charging
    CHGMNG_FSM_EVENT_CHG_TO_PULSE,          //charging to pulsecharging
    CHGMNG_FSM_EVENT_MAX        //don't use
} CHGMNG_FSM_EVENT_E;

typedef enum {
    CHGMNG_ADP_PLUGIN,
    CHGMNG_RECHARGE,
    CHGMNG_FAULT_RESUME
}CHGMNG_CHG_START_TYPE_E;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

LOCAL BLOCK_ID chg_task_id;

/*CHGMNG_CHARGING state use*/
LOCAL SCI_TIMER_PTR g_charging_timer = NULL;      //charge state timer
LOCAL SCI_TIMER_PTR g_ovp_timer = NULL;

LOCAL BOOLEAN ischgmng_start = SCI_FALSE;     //charge manager start flag.
LOCAL BOOLEAN isshutting_down = SCI_FALSE;    //shutdowe flag
LOCAL CHG_SWITPOINT_E hw_switch_point = CHG_SWITPOINT_15; //The lowest switchover point between cc and cv modes.
LOCAL uint16 ovp_detect_counter = 0;
LOCAL uint16 charge_endtime_counter = 0;   
LOCAL uint16 warning_counter = 0;

/* recent_message_flag: Record the recent message which has been send before client registes.*/
LOCAL uint32 recent_message[10] = {0};     // Ten is enough!

#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
LOCAL CHGMNG_ADC_CAL_T adc_cal = {3505,4200,1207,1000,CAL_TYPR_NO};
#else
LOCAL CHGMNG_ADC_CAL_T adc_cal = {931,4200,4487,1000,CAL_TYPR_NO};
#endif

#define BAT_CAPACITY_STEP   12
LOCAL uint16 dischg_bat_capacity_table[BAT_CAPACITY_STEP][2] = 
{
    {4120,  100},
    {4060,  90},
    {3979,  80},
    {3900,  70},
    {3840,  60},
    {3800,  50},
    {3760,  40},
    {3730,  30},
    {3700,  20},
    {3650,  15},
    {3600,  5},
    {3501,  0},
};

LOCAL uint16 chg_bat_capacity_table[BAT_CAPACITY_STEP][2]=
{
    {4200,  100},
    {4180,  90},
    {4119,  80},
    {4080,  70},
    {4020,  60},
    {3970,  50},
    {3920,  40},
    {3880,  30},
    {3860,  20},
    {3830,  15},
    {3730,  5},
    {3251,  0},
};

#ifdef CHG_SHARP_FEATRUE
LOCAL uint16    bgvol_map_cccv[32][2] = 
{
    {955,  0x1F},
    {958,	0x1E},
    {962,	0x1D},
    {966,	0x1C},
    {969,	0x1B},
    {973,	0x1A},
    {977,	0x19},
    {981,	0x18},
    {984,	0x17},
    {988,	0x16},
    {992,	0x15},
    {995,	0x14},
    {1000,0x13},
    {1003,0x12},
    {1006,0x11},
    {1011,0x10},
    {1014,0x0},
    {1018,0x1},
    {1022,0x2},
    {1025,0x3},
    {1029,0x4},
    {1033,0x5},
    {1037,0x6},
    {1040,0x7},
    {1044,0x8},
    {1048,0x9},
    {1051,0xA},
    {1055,0xB},
    {1059,0xC},
    {1062,0xD},
    {1066,0xE},
    {1070,0xF},
};
#endif

LOCAL CHGMNG_DISCHARGE_PARAM_T dischg_param = 
{
    3440,     //warning_vol
    3380,     //shutdown_vol
    3000,     //deadline_vol
    24,       //warning_count,warning interval
};

LOCAL CHGMNG_CHARGE_PARAM_T chg_param = 
{
    4150,       //rechg_vol
    4210,         //chg_end_vol  
    4330,
    CHARGER_CURRENT_400MA,      //standard_chg_current
    CHARGER_CURRENT_300MA,      //usb_chg_current
    CHARGER_CURRENT_300MA,  //nonstandard_current_sel,0:usb charge current,1:normal charge current
    18000,      //18000S
};

LOCAL CHGMNG_OVP_PARAM_T ovp_param = 
{
    CHARGER_NORMAL_TYPE,//ovp_type
    0,                  //ovp_over_vol
    0,                  //ovp_resume_vol
};

LOCAL CHGMNG_STATE_INFO_T module_state =
{
    CHGMNG_IDLE,        //chgmng_state
    0,                  //bat_statistic_vol
    0,                  //bat_cur_vol
    0,                  //bat_remain_cap
    0,                  //charging_current
    0,                  //charging_temperature
    CHGMNG_ADP_UNKNOW,  //adp_type
    CHGMNG_INVALIDREASON, //charging_stop_reason
    0,                  //chg_vol
};
LOCAL CHGMNG_STATE_INFO_T module_state_to_app;

typedef struct 
{
    uint32 queue[DISCHG_VBAT_BUFF_SIZE];
    uint32 pointer;
    uint32 sum;
    uint32 queue_len;
} VBAT_QUEUE_INFO_T;
LOCAL VBAT_QUEUE_INFO_T vbat_queue;

typedef struct 
{
    uint32 queue[CHG_CURRENT_BUFF_SIZE];
    uint32 pointer;
    uint32 sum;
} CURRENT_QUEUE_INFO_T;
LOCAL CURRENT_QUEUE_INFO_T current_queue;

#ifdef CHGMNG_OTP_SUPPORT
typedef struct 
{
    uint32 queue[CHG_TEMP_BUFF_SIZE];
    uint32 pointer;
    uint32 sum;
} TEMP_QUEUE_INFO_T;
LOCAL TEMP_QUEUE_INFO_T temp_queue;
LOCAL CHGMNG_OTP_PARAM_T otp_param;
#endif

#ifdef SC6531_CHG_CUR_SEARCH
#define SEARCH_COUNTER        20
#define TRIGGER_COUNTER       5
#define SEARCH_TIMER_INTERVAL 71
#define SC6531_FIX_CURRENT  800

LOCAL uint32 charger_plugin_flag = 0;
LOCAL SCI_TIMER_PTR g_6531_search_cur_timer = NULL;
LOCAL uint32 charging_current_type = SC6531_FIX_CURRENT;

LOCAL uint32 charger_resistor_param=172;  //((0.62*100)/(0.36*100))

LOCAL void _CHGMNG_SearchCurEnd(void);
LOCAL void _CHGMNG_SearchTimerHandler (uint32 state);
LOCAL void _CHGMNG_SC6531_ChgCurSearch(void);
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void _CHGMNG_SrvDefault (BLOCK_ID id, uint32 argc, void *argv);
LOCAL uint32 _CHGMNG_VoltagetoPercentum (uint32 voltage, CHGMNG_STATE_E  is_charging, BOOLEAN update);
LOCAL void _CHGMNG_StartCharge (CHGMNG_CHG_START_TYPE_E start_type);
LOCAL void _CHGMNG_SendMessagetoClient (CHR_SVR_MSG_SERVICE_E  msg, uint32 param);
LOCAL uint32 _CHGMNG_GetPhoneState (void);
LOCAL void _CHGMNG_VbatMonitorRoutine (void);
LOCAL void _CHGMNG_VBatQueueInit (uint32 vbat_vol,uint32 queue_len);
LOCAL void _CHGMNG_ChargeTimerHandler (uint32 state);
LOCAL uint32 _CHGMNG_VBatTimerHandler (uint32 param);
LOCAL void _CHGMNG_StopCharge (CHGMNG_STOPREASON_E reason);
LOCAL void _CHGMNG_CheckVbatPresent (void);
LOCAL void _CHGMNG_ChargeMonitorRoutine(void);
LOCAL void _CHGMNG_SrvThread (uint32 argc, void *argv);
LOCAL void _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_E fsm_event,uint32 condition);
LOCAL uint32 _CHGMNG_GetVprogADCResult (void);
LOCAL void _CHGMNG_SetChargeCurrent (CHGMNG_ADAPTER_TYPE_E mode);
LOCAL void _CHGMNG_OvpTimerHandler (uint32 state);
LOCAL uint32 _CHGMNG_GetVBATADCResult (void);
LOCAL uint32 _CHGMNG_GetVbatVol(void);
LOCAL uint32 _CHGMNG_GetCurrentTemp(void);
LOCAL void _CHGMNG_TempQueueInit(uint32 temp);
LOCAL void _CHGMNG_TempQueueUpdate(uint32 temp);
LOCAL void _CHGMNG_CalVchgBg(void);

#ifdef DUAL_BATTERY_SUPPORT
LOCAL void _CHGMNG_ModuleReset(uint32 voltage);
#endif
extern BOOLEAN PROD_SetChargeNVParam (CHG_SWITPOINT_E    point);

/**---------------------------------------------------------------------------*
 **                         Local Function Definitions                        *
 **---------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description:    Charger module task entry.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_PrintLogMsg(void)
{
    //CHGMNG_PRINT:"CHGMNG:module_state.chgmng_state:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_236_112_2_17_23_3_25_387,(uint8*)"d",module_state.chgmng_state);
    //CHGMNG_PRINT:"CHGMNG:module_state.bat_statistic_vol:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_237_112_2_17_23_3_25_388,(uint8*)"d",module_state.bat_statistic_vol);
    //CHGMNG_PRINT:"CHGMNG:module_state.bat_cur_vol:%d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_238_112_2_17_23_3_25_389,(uint8*)"d",module_state.bat_cur_vol);
    //CHGMNG_PRINT:"CHGMNG:module_state.bat_remain_cap:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_239_112_2_17_23_3_25_390,(uint8*)"d",module_state.bat_remain_cap);
    //CHGMNG_PRINT:"CHGMNG:x0_adc:%d,y0_vol:%d,slope:%d,zoom_in:%d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_240_112_2_17_23_3_25_391,(uint8*)"dddd",adc_cal.x0_adc,adc_cal.y0_vol,adc_cal.slope,adc_cal.zoom_in);
    
    if (CHG_PHY_IsChargerPresent())
    {
        //CHGMNG_PRINT:"CHGMNG:module_state.charging_current:%d"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_244_112_2_17_23_3_25_392,(uint8*)"d",module_state.charging_current);
        //CHGMNG_PRINT:"CHGMNG:module_state.adp_type:%d"
       // SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_245_112_2_17_23_3_25_393,(uint8*)"d",module_state.adp_type);
        //CHGMNG_PRINT:"CHGMNG:module_state.charging_stop_reason:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_246_112_2_17_23_3_25_394,(uint8*)"d",module_state.charging_stop_reason);
        //CHGMNG_PRINT:"CHGMNG:hw_switch_point:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_247_112_2_17_23_3_25_395,(uint8*)"d",hw_switch_point);
        //CHGMNG_PRINT:"CHGMNG:chg_end_vol:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_248_112_2_17_23_3_25_396,(uint8*)"d",chg_param.chg_end_vol);
#ifdef CHGMNG_OTP_SUPPORT
    	//CHGMNG_PRINT:"CHGMNG:otp_type:%d, over_low:%d, over_high:%d,resume_low:%d,resume_high:%d,charging_temperature:%d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_252_112_2_17_23_3_25_397,(uint8*)"dddddd", otp_param.otp_type, otp_param.over_low, otp_param.over_high,otp_param.resume_low, otp_param.resume_high,module_state.charging_temperature);
#endif
    }
    else
    {
        //CHGMNG_PRINT:"CHGMNG:module_state.bat_remain_cap:%d"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_257_112_2_17_23_3_25_398,(uint8*)"d",module_state.bat_remain_cap);
        //CHGMNG_PRINT:"CHGMNG:dischg_param.shutdown_vol:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_258_112_2_17_23_3_25_399,(uint8*)"d",dischg_param.shutdown_vol);
        //CHGMNG_PRINT:"CHGMNG:dischg_param.deadline_vol:%d"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_259_112_2_17_23_3_25_400,(uint8*)"d",dischg_param.deadline_vol);
        //CHGMNG_PRINT:"CHGMNG:dischg_param.warning_vol:%d"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_260_112_2_17_23_3_25_401,(uint8*)"d",dischg_param.warning_vol);
    }
    //CHGMNG_PRINT:"CHGMNG:....................vbat_queue.pointer:%d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_262_112_2_17_23_3_25_402,(uint8*)"d",vbat_queue.pointer);

}

/*****************************************************************************/
//  Description:    charge task entry.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_SrvThread (uint32 argc, void *argv)
{
    CHGMNGSVR_SIG_T *	signal;

    SCI_SLEEP(1500);	//wait for some client
	
    if (CHG_PHY_IsChargerPresent())         //If we hadn't detected charger is pluged in in interrupts, this operator can ensure charging is started.
    {
        if (module_state.chgmng_state == CHGMNG_IDLE)
        {
#ifdef SC6531_CHG_CUR_SEARCH
            CHG_PHY_TurnOn();
            CHG_PHY_SetRecharge();//pulse high
            charger_plugin_flag = 1;
            charging_current_type =SC6531_FIX_CURRENT;
            CHG_PHY_SetChgCurrent (charging_current_type);
            _CHGMNG_SendMessagetoClient (CHR_CHARGE_START_IND, 0);
            _CHGMNG_SC6531_ChgCurSearch();
#else
            CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PLUG_IN_MSG,0);
#endif
        }
    }

    DoIdle_RegisterCallback ( (DOIDLE_CALLBACK_FUNC) _CHGMNG_VBatTimerHandler);
    
    for(;;)
    {
        signal = (CHGMNGSVR_SIG_T *) SCI_GetSignal (chg_task_id);
        
        switch(signal->SignalCode)
        {
            case CHGMNG_CHARGER_PLUG_IN_MSG:
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG,CHGMNG_ADP_PLUGIN);
            break;
            case CHGMNG_CHARGER_PLUG_OUT_MSG:
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_CHARGERUNPLUG);
            break;
            case CHGMNG_CHARGER_MONITOR_MSG:
                _CHGMNG_ChargeMonitorRoutine();
            break;
            case CHGMNG_VBAT_MONITOR_MSG:
                _CHGMNG_VbatMonitorRoutine ();
            break;
#ifdef DUAL_BATTERY_SUPPORT
            case CHGMNG_MODULE_RESET_MSG:
                _CHGMNG_ModuleReset(signal->sig_param);      
            break;
#endif
            default:
                SCI_PASSERT (0,("Error msg!!!"));   /*assert verified*/
            break;    
        }
        
        SCI_FREE(signal);
        signal = NULL;
    }
}

/*****************************************************************************/
//  Description:    This function is used to process Finite State Machine of charge module.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_E fsm_event, uint32 condition)
{
    //CHGMNG_PRINT:"CHGMNG:_CHGMNG_FSMProcess fsm_event:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_321_112_2_17_23_3_25_403,(uint8*)"d",fsm_event);
    
    switch(fsm_event)
    {
        case CHGMNG_FSM_EVENT_INIT:
            module_state.chgmng_state= CHGMNG_IDLE;
        break;
              
        case CHGMNG_FSM_EVENT_START_CHG:
            module_state.chgmng_state = CHGMNG_STARTING;
            _CHGMNG_StartCharge(condition);
            module_state.chgmng_state = CHGMNG_CHARGING; ///should be changed in future 
        break;
        
        case CHGMNG_FSM_EVENT_STOP_CHG:
            if(CHGMNG_CHARGERUNPLUG == condition)
            {
                module_state.chgmng_state = CHGMNG_IDLE;
            }
            else
            {
                module_state.chgmng_state = CHGMNG_STOPPING;
            }                
            _CHGMNG_StopCharge (condition);
            
            if(CHGMNG_CHARGEDONE == condition)
            {
#ifndef CHG_SHARP_FEATRUE           
                hw_switch_point = (CHG_SWITPOINT_E) CHG_PHY_UpdateSwitchoverPoint (FALSE);
                //Set CC-CV point to nv
                PROD_SetChargeNVParam (hw_switch_point);
#endif
            }
        break;
        
        case CHGMNG_FSM_EVENT_PULSE_TO_CHG:
            module_state.chgmng_state = CHGMNG_CHARGING;
        break;
        
        case CHGMNG_FSM_EVENT_CHG_TO_PULSE:
            module_state.chgmng_state = CHGMNG_PULSECHARGING;
        break;
        
        default:            
            SCI_PASSERT (0,("Error CHGMNG_FSM_EVENT_E!!!"));   /*assert verified*/
        break;
    }
}

//end modify by paul for charge manage
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    if vbat don't present,should poweroff by hardware
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_CheckVbatPresent (void)
{
    uint32 old_time,new_time;

    CHG_PHY_ShutDown();

    ////wait 10ms, if vbat don't present,should poweroff by hardware
    old_time = new_time = SCI_GetTickCount();

    while ( (new_time - old_time) < 10)
    {
        new_time = SCI_GetTickCount();
    }

    CHG_PHY_TurnOn();
    CHG_PHY_SetRecharge();
}
/*****************************************************************************/
//  Description:    convert adc to Vbus voltage
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL uint32 _CHGMNG_AdcValueToVChgVol(uint32 adc)
{
    uint32 result;
    uint32 vbat_vol = CHGMNG_AdcvalueToVoltage (adc);
    uint32 m,n;
    
    ///v1 = vbat_vol*0.268 = vol_bat_m * r2 /(r1+r2)
    n = VBAT_VOL_DIV_P2*VCHG_DIV_P1;
    m = vbat_vol*VBAT_VOL_DIV_P1*(VCHG_DIV_P2);
    result = (m + n/2)/n;
    return result;
}

/*****************************************************************************/
//  Description:    This function is used to get the result of Vcharge ADC value.
//                  Return: the Vcharge adc value.
//  Author:         paul.luo
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetVChgVol (void)
{
    return _CHGMNG_AdcValueToVChgVol(ADC_GetResultDirectly (ADC_CHANNEL_VCHG, SCI_FALSE));
}

/*****************************************************************************/
//  Description:    This function is used to convert VProg ADC value to charge current value.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL uint16 _CHGMNG_ADCValueToCurrent(uint32 adcvalue)
{
	uint32 current_type= 300;
	uint32 voltage = (uint32)CHGMNG_AdcvalueToVoltage (adcvalue);
	uint32 temp;
	
	if(module_state.adp_type == CHGMNG_ADP_STANDARD)
	{
        current_type = chg_param.standard_chg_current;
	}
	else if(module_state.adp_type == CHGMNG_ADP_USB)
	{
        current_type = chg_param.usb_chg_current;
	}
    else if(module_state.adp_type == CHGMNG_ADP_NONSTANDARD)
    {
        current_type = chg_param.nonstandard_chg_current;
    }
    else if(module_state.adp_type == CHGMNG_ADP_UNKNOW)
    {
        current_type = CHARGER_CURRENT_300MA;
    }
	
	/*
		1.internal chip voltage for ADC measure:v1---->v1 = voltage*0.268 = voltage*VOL_DIV_P1/VOL_DIV_P2
		2.current convert voltage expressions: VOL_TO_CUR_PARAM*current/curret_type = v1 = voltage*VOL_DIV_P1/VOL_DIV_P2
		   ---->current = ((current_type*voltage*VOL_DIV_P1)/VOL_TO_CUR_PARAM)/VOL_DIV_P2;
	*/
	temp = ((current_type*voltage*VBAT_VOL_DIV_P1)/VOL_TO_CUR_PARAM)/VBAT_VOL_DIV_P2;
	
	return (uint16)temp;
	
}

/*****************************************************************************/
//  Description:    This function gets the virtual Vprog value. Because the Iprog is not steady
//                  enough when it is larger than 500mA, we have to calculate its average. The
//                  sampling times perhaps will be adjusted in the eventual project.
//                  Return: the virtual result.
//  Author:         Benjamin.Wang
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetVprogADCResult (void)
{
    int i,j, temp;
    int vprog_result[VPROG_RESULT_NUM];
    
    for (i = 0; i < VPROG_RESULT_NUM; i++)
    {
        vprog_result[i] =
            ADC_GetResultDirectly (ADC_CHANNEL_PROG, SCI_FALSE);
    }

    for (j=1; j<=VPROG_RESULT_NUM-1; j++)
    {
        for (i=0; i<VPROG_RESULT_NUM -j; i++)
        {
            if (vprog_result[i] > vprog_result[i+1])
            {
                temp = vprog_result[i];
                vprog_result[i] = vprog_result[i+1];
                vprog_result[i+1] = temp;
            }
        }
    }

    return vprog_result[VPROG_RESULT_NUM/2];
}
//#define CURRENT_FROM_ISENSE
#define ADC_CHANNEL_ISENSE  12
/*****************************************************************************/
//  Description:    Get charging current
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetChgCurrent(void)
{
#ifdef  CURRENT_FROM_ISENSE
    {
        uint32 isense,vbatsense,i;
        uint32 temp;
        uint32 cnt = 4;
        
        for(i=0;i < cnt; i++)
        {
            isense = CHGMNG_AdcvalueToVoltage (ADC_GetResultDirectly (ADC_CHANNEL_ISENSE, SCI_FALSE));
            vbatsense = CHGMNG_AdcvalueToVoltage (ADC_GetResultDirectly (ADC_CHANNEL_VBAT, SCI_FALSE));
            CHGMNG_PRINT(("CHGMNG:isense:%d,vbatsense:%d\n",isense,vbatsense));
            if(isense > vbatsense)
            {
                break;
            }
        }
        if(isense > vbatsense)
        {
            temp = ((isense - vbatsense)*100)/36;
            temp = temp + (temp/10);
            return temp;
        }
        else
         {
            return module_state.charging_current;
         }
        
    }
#else
    return _CHGMNG_ADCValueToCurrent (_CHGMNG_GetVprogADCResult());
#endif
}

/*****************************************************************************/
//  Description:    This function is used to get the result of VBAT ADC.
//                  Return: the VBAT value.
//  Author:         Benjamin.Wang
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetVBATADCResult (void)
{
    int i,j, temp;
    
    int vbat_result[VBAT_RESULT_NUM];
    
    for (i = 0; i < VBAT_RESULT_NUM; i++)
    {
        vbat_result[i] =
            ADC_GetResultDirectly (ADC_CHANNEL_VBAT, SCI_FALSE);
#ifdef PLATFORM_SC8800G
        if (ischgmng_start)
        {
            extern int8 check_f_GSM_TX_on (void);
            if (check_f_GSM_TX_on())
            {
                //CHGMNG_PRINT:"CHGMNG:check_f_GSM_TX_on!"
                //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_507_112_2_17_23_3_26_404,(uint8*)"");
                return CHGMNG_VoltageToAdcvalue (module_state.bat_statistic_vol);
            }
        }
#endif
    }
#if 0
    for (i = 0; i < VBAT_RESULT_NUM; i++)
    {
       //CHGMNG_PRINT:"CHGMNG:vbat_result[%d]:%d"
       //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_516_112_2_17_23_3_26_405,(uint8*)"dd",i,vbat_result[i]);
    }
#endif
    for (j=1; j<=VBAT_RESULT_NUM-1; j++)
    {
        for (i=0; i<VBAT_RESULT_NUM -j; i++)
        {
            if (vbat_result[i] > vbat_result[i+1])
            {
                temp = vbat_result[i];
                vbat_result[i] = vbat_result[i+1];
                vbat_result[i+1] = temp;
            }
        }
    }


    //CHGMNG_PRINT:"CHGMNG:_CHGMNG_GetVBATADCResult:%d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_533_112_2_17_23_3_26_406,(uint8*)"d",vbat_result[VBAT_RESULT_NUM/2]);
    return vbat_result[VBAT_RESULT_NUM/2];
}

/*****************************************************************************/
//  Description:    Get Vbat voltage
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetVbatVol(void)
{
    uint32 vol = CHGMNG_AdcvalueToVoltage (_CHGMNG_GetVBATADCResult());
    //CHGMNG_PRINT:"CHGMNG:_CHGMNG_GetVbatVol:%d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_542_112_2_17_23_3_26_407,(uint8*)"d",vol);
    return vol;
}

/*****************************************************************************/
//  Description:    This function return current temperature.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetCurrentTemp(void)
{
	int32 result = 0;
    
#ifdef CHGMNG_OTP_SUPPORT	
	if(otp_param.otp_type == 1)	////environment temperature for otp
	{
		result = CHG_NV_TEMP_OFFSET + ADC_GetEnvTemperature();
	}
	else if(otp_param.otp_type == 2)		///Vbat temperature for otp
	{
		result = CHG_NV_TEMP_OFFSET + ADC_GetVbatTemperature();
	}
	
	SCI_PASSERT ((result >= 0), ("Temperature is very low!"));/*assert to do*/
#endif
	
	return (uint32)result;
}
/*****************************************************************************/
//  Description:    Update Statistic Temperature.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_TempQueueInit(uint32 temp)
{
#ifdef CHGMNG_OTP_SUPPORT
    uint32 i;
    
    temp_queue.sum = temp * CHG_TEMP_BUFF_SIZE;
    temp_queue.pointer = 0;
    
    for (i = 0; i < CHG_TEMP_BUFF_SIZE ; i++)
    {
        temp_queue.queue[i] = temp;
    }
    module_state.charging_temperature = temp;
#endif    
}
/*****************************************************************************/
//  Description:    Update Statistic Temperature.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_TempQueueUpdate(uint32 temp)
{
#ifdef CHGMNG_OTP_SUPPORT
    temp_queue.sum += temp;
    temp_queue.sum -= temp_queue.queue[temp_queue.pointer];    
    module_state.charging_temperature = temp_queue.sum/CHG_TEMP_BUFF_SIZE;
    
    temp_queue.queue[temp_queue.pointer++] = temp;
    
    if (temp_queue.pointer == CHG_TEMP_BUFF_SIZE)
    {
        temp_queue.pointer = 0;
    }
#endif    
}
/*****************************************************************************/
//  Description:    Charge current queue init
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_CurrentQueueInit (uint32 current)
{
    uint32 i;
    
    current_queue.sum = current * CHG_CURRENT_BUFF_SIZE;
    current_queue.pointer = 0;
    
    for (i = 0; i < CHG_CURRENT_BUFF_SIZE ; i++)
    {
        current_queue.queue[i] = current;
    }
    module_state.charging_current = current;
}
/*****************************************************************************/
//  Description:    update charge current queue
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_CurrentQueueUpdate (uint32 current)
{
    current_queue.sum += current;
    current_queue.sum -= current_queue.queue[current_queue.pointer];    
    module_state.charging_current = current_queue.sum/CHG_CURRENT_BUFF_SIZE;
    
    current_queue.queue[current_queue.pointer++] = current;
    
    if (current_queue.pointer == CHG_CURRENT_BUFF_SIZE)
    {
        current_queue.pointer = 0;
    }
}
/*****************************************************************************/
//  Description:    The function initializes the Vbat queue
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_VBatQueueInit (uint32 vbat_vol,uint32 queue_len)
{
    uint32 i;
    
    SCI_DisableIRQ();   //must do it
    vbat_queue.sum = vbat_vol * queue_len;
    vbat_queue.pointer = 0;
    vbat_queue.queue_len = queue_len;
    SCI_RestoreIRQ();
    
    for (i = 0; i < queue_len ; i++)               //init vbat queue
    {
        vbat_queue.queue[i] = vbat_vol;
    }
    
    module_state.bat_statistic_vol = vbat_vol;
}
/*****************************************************************************/
//  Description:    update vbat queue
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_VbatQueueUpdate (uint32 vbat)
{
    SCI_DisableIRQ();   //must do it
    vbat_queue.sum += vbat;
    vbat_queue.sum -= vbat_queue.queue[vbat_queue.pointer];    
    module_state.bat_statistic_vol = vbat_queue.sum/vbat_queue.queue_len;
    SCI_RestoreIRQ();
    
    vbat_queue.queue[vbat_queue.pointer++] = vbat;
    
    if (vbat_queue.pointer == vbat_queue.queue_len)
    {
        vbat_queue.pointer = 0;
    }
}

/*****************************************************************************/
//  Description:    Message send function.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
LOCAL void _CHGMNG_SendMessagetoClient (CHR_SVR_MSG_SERVICE_E  msg, uint32 param)
{
    static uint32 msg_count = 1;

    if (CHR_MSG_MAX_NUM == recent_message[0])
    {
        int32 i = 1;

        while (i < 10)
        {
            if ( (recent_message[i] != 0))
            {
                if (SCI_SUCCESS != SCI_SendEventToClient (CHR_SERVICE, recent_message[i], 0))
                {
                    if ( (msg != CHR_CAP_IND) && (msg != CHR_WARNING_IND))
                    {
                        recent_message[msg_count++] = msg;

                        if (10 == msg_count)
                        {
                            msg_count = 1;
                        }
                    }

                    return;
                }
                else
                {
                    //CHGMNG_PRINT:"CHGMNG:%d has been send000!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_699_112_2_17_23_3_26_408,(uint8*)"d", recent_message[i]);
                    recent_message[i] = 0;
                }
            }

            i++;
        }

        msg_count = 1;
        recent_message[0] = 0;
    }

    // send to client.
    // The percent is valid only when msg == CHR_CAP_IND
    //CHGMNG_PRINT:"CHGMNG:%d has been send2222!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_713_112_2_17_23_3_26_409,(uint8*)"d", msg);

    if (SCI_SUCCESS != SCI_SendEventToClient (CHR_SERVICE, (uint32) msg, (void *) param))
    {
        //CHGMNG_PRINT:"CHGMNG:%d has been saved111!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_717_112_2_17_23_3_26_410,(uint8*)"d", msg);

        if ( (msg != CHR_CAP_IND) && (msg != CHR_WARNING_IND))
        {
            recent_message[0] = CHR_MSG_MAX_NUM;        //This is used to indicate that we have had messages to handle.
            recent_message[msg_count++] = msg;

            if (10 == msg_count)
            {
                msg_count = 1;
            }
        }
    }
}

/*****************************************************************************/
//  Description:    Convert ADCVoltage to percentrum.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
uint32 _CHGMNG_VoltagetoPercentum (uint32 voltage, CHGMNG_STATE_E  is_charging, BOOLEAN update)
{
    uint16 percentum;
    int32 temp = 0;
    int pos = 0;
    static uint16 pre_percentum = 0xffff;

    if(update){
        pre_percentum = 0xffff;
        return 0;
    }

    if((is_charging == CHGMNG_CHARGING) || (is_charging == CHGMNG_PULSECHARGING)){
        for(pos = 0; pos < BAT_CAPACITY_STEP -1; pos++){
            if(voltage > chg_bat_capacity_table[pos][0])
                break;
        }
        if(pos == 0) {
            percentum = 100;
        }else{
            temp = chg_bat_capacity_table[pos][1]-chg_bat_capacity_table[pos-1][1];
            temp = temp*(int32)(voltage - chg_bat_capacity_table[pos][0]);
            temp = temp/(chg_bat_capacity_table[pos][0] - chg_bat_capacity_table[pos-1][0]);
            temp = temp + chg_bat_capacity_table[pos][1];
            if(temp < 0){
                temp = 0;
            }
            percentum = temp;
        }
        
       // CHGMNG_PRINT(("temp : %d,chg_bat_capacity_table[2][0]:%d,chg_bat_capacity_table[11][0]:%d",temp,chg_bat_capacity_table[2][0],chg_bat_capacity_table[11][0] ));
        
        if(pre_percentum == 0xffff){    
            pre_percentum = percentum;
        }else if(pre_percentum > percentum){   
            percentum = pre_percentum;
        }else{    
            pre_percentum = percentum;
        }
    }else{
        for(pos = 0; pos < BAT_CAPACITY_STEP -1; pos++){
            if(voltage > dischg_bat_capacity_table[pos][0])
                break;
        }
        if(pos == 0) {
            percentum = 100;
        }else{
            temp = dischg_bat_capacity_table[pos][1]-dischg_bat_capacity_table[pos-1][1];
            temp = temp*(int32)(voltage - dischg_bat_capacity_table[pos][0]);
            temp = temp/(dischg_bat_capacity_table[pos][0] - dischg_bat_capacity_table[pos-1][0]);
            temp = temp + dischg_bat_capacity_table[pos][1];
            if(temp < 0){
                temp = 0;
            }
            percentum = temp;
        }
       // CHGMNG_PRINT(("temp : %d,dischg_bat_capacity_table[2][0]:%d,dischg_bat_capacity_table[11][0]:%d",temp,dischg_bat_capacity_table[2][0],dischg_bat_capacity_table[11][0] ));
    
        if(pre_percentum == 0xffff){
            pre_percentum = percentum;
        }else if(pre_percentum < percentum){
            percentum = pre_percentum;
        }else{
            pre_percentum = percentum;
        }
    }

    return percentum;
}

/*****************************************************************************/
//  Description:    Get phone state(busy or no busy).
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetPhoneState (void)
{
    /*Improve later.*/
    return 0;
}

/*****************************************************************************/
//  Description:    The function monitor vbat status,Be called by DoIdle_Callback.
//  Author:         Mingwei.Zhang
//    Note:
/*****************************************************************************/
LOCAL uint32 _CHGMNG_VBatTimerHandler (uint32 param)
{
    CHGMNG_SendMsgToChgTask(CHGMNG_VBAT_MONITOR_MSG,0);
    
    return 0;
}
/*****************************************************************************/
//  Description:    The function calculates the vbat every 2 seconds.
//  Author:         Benjamin.Wang
//    Note:
/*****************************************************************************/
LOCAL void _CHGMNG_VbatMonitorRoutine ()
{
    /*If we had inform the upper layer to shutdown, we will not send any other messages
    because too many messages can block the message queue.*/
    if (isshutting_down)
    {
        return;
    }
    
    if ( (module_state.chgmng_state == CHGMNG_IDLE) && (!CHG_PHY_IsChargerPresent()))
    {        
        module_state.bat_cur_vol = _CHGMNG_GetVbatVol();
        _CHGMNG_VbatQueueUpdate (module_state.bat_cur_vol);
        
        _CHGMNG_PrintLogMsg();   
        if (module_state.bat_statistic_vol < dischg_param.warning_vol)
        {
            if (0 == warning_counter)
            {
                //CHGMNG_PRINT:"CHGMNG:CHR_WARNING_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!"
                //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_831_112_2_17_23_3_26_411,(uint8*)"");
                _CHGMNG_SendMessagetoClient (CHR_WARNING_IND, 0);
            }

            warning_counter++;

            if (warning_counter > dischg_param.warning_count)
            {
                warning_counter = 0;
            }
        }

        if ( (module_state.bat_statistic_vol < dischg_param.shutdown_vol) || (module_state.bat_cur_vol < dischg_param.deadline_vol))
        {
            if (0 == warning_counter)
            {
                //CHGMNG_PRINT:"CHGMNG:CHR_WARNING_IND Before shutdown!!!!!!!!!!!!!!!!!!!!!!!!!!!"
                //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_847_112_2_17_23_3_26_412,(uint8*)"");
                _CHGMNG_SendMessagetoClient (CHR_WARNING_IND, 0);
                warning_counter++;
            }
            else
            {
                //CHGMNG_PRINT:"CHGMNG:CHR_SHUTDOWN_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!"
                //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_853_112_2_17_23_3_26_413,(uint8*)"");
                _CHGMNG_SendMessagetoClient (CHR_SHUTDOWN_IND, 0);

                if (recent_message[0] == 0) ///msg is send
                {
                    isshutting_down = SCI_TRUE;
                }
            }
        }
    }
    else
    {
        warning_counter = 0;
    }

    module_state.bat_remain_cap = _CHGMNG_VoltagetoPercentum (module_state.bat_statistic_vol,module_state.chgmng_state,SCI_FALSE);
    _CHGMNG_SendMessagetoClient (CHR_CAP_IND, module_state.bat_remain_cap);
}
/*****************************************************************************/
//  Description:    over voltage protect timer
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_OvpTimerHandler (uint32 state)
{
    uint32 vchg_vol, vprog;
    static uint32 tick= 0;
    uint32 curr_tick = 0;

    if (CHG_PHY_IsChargerPresent())
    {
        if (ovp_param.ovp_type == CHARGER_NORMAL_TYPE)
        {
            vchg_vol = _CHGMNG_GetVChgVol ();
            module_state.chg_vol = vchg_vol;
		//CHGMNG_PRINT(("CHGMNG:vchg_vol:%d,\n",vchg_vol));
            /* over volatage */
            if (vchg_vol > (ovp_param.ovp_over_vol))
            {
                if (ovp_detect_counter == 0)
                {
                    tick = SCI_GetTickCount();
                }

                ovp_detect_counter++;
            }

            if ( (ovp_detect_counter >= OVP_DETECT_VALID_TIMES) && (module_state.charging_stop_reason != CHGMNG_OVERVOLTAGE))
            {
                curr_tick = SCI_GetTickCount();

                if (curr_tick - tick < (OVP_TIMER_INTERVAL*OVP_DETECT_TIMES))
                {
                    //SCI_TRACE_LOW:"CHGMNG_CHARGING: ovp"
                    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_902_112_2_17_23_3_27_414,(uint8*)"");

                    _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_OVERVOLTAGE);
                }
                ovp_detect_counter = 0;
            }
            else if ( (module_state.charging_stop_reason == CHGMNG_OVERVOLTAGE) 
                && (vchg_vol  < (ovp_param.ovp_resume_vol)))
            {
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG,CHGMNG_FAULT_RESUME);
            }
        }
        else if (ovp_param.ovp_type == CHARGER_NK_TYPE)
        {
            vchg_vol = _CHGMNG_GetVChgVol ();
            module_state.chg_vol = vchg_vol;

            /* over volatage */
            if (vchg_vol > (ovp_param.ovp_over_vol))
            {
                if (ovp_detect_counter == 0)
                {
                    tick = SCI_GetTickCount();
                }

                ovp_detect_counter++;
            }

            if ( (ovp_detect_counter >= OVP_DETECT_VALID_TIMES)  && (module_state.charging_stop_reason != CHGMNG_OVERVOLTAGE))
            {
                curr_tick = SCI_GetTickCount();

                if (curr_tick - tick < (OVP_TIMER_INTERVAL*OVP_DETECT_TIMES))
                {
                    CHG_PHY_SetChgCurrent (CHARGER_CURRENT_300MA);

                    vprog = _CHGMNG_GetVprogADCResult();
                    vchg_vol = _CHGMNG_GetVChgVol();
                    /* over power, 509 -> 8V */
                    if ( (vchg_vol > 8000) && (vprog > 200))
                    {
                        //SCI_TRACE_LOW:"CHGMNG_CHARGING: charger over power"
                        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_942_112_2_17_23_3_27_415,(uint8*)"");

                        _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_OVERVOLTAGE);
                    }
                }
                ovp_detect_counter = 0;
            }
            else if ( (module_state.charging_stop_reason == CHGMNG_OVERVOLTAGE) && (vchg_vol < (ovp_param.ovp_resume_vol)))
            {
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG,CHGMNG_FAULT_RESUME);
            }
        }
        else
        {
            SCI_PASSERT (SCI_FALSE, ("CHGMNG:Wrong charger type, please check nv parameter!"));/*assert verified*/
        }
    }
}
/*****************************************************************************/
//  Description:    timeout function of charge timer
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_ChargeTimerHandler (uint32 state)
{
    CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_MONITOR_MSG,0);
}

/*****************************************************************************/
//  Description:    When charger connect start charger monitor,Be called by charger task.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_ChargeMonitorRoutine(void)
{
    uint32 chg_current = 0;
    static uint32 charge_pulse_times = 0; //high pulse times
    static BOOLEAN pulse_status = SCI_TRUE;
    
    if(CHGMNG_IDLE == module_state.chgmng_state)
    {
        return;
    }

    _CHGMNG_TempQueueUpdate(_CHGMNG_GetCurrentTemp());

    _CHGMNG_PrintLogMsg(); 

    if ( (CHGMNG_CHARGING == module_state.chgmng_state) || (CHGMNG_PULSECHARGING == module_state.chgmng_state))
    {   
        chg_current = _CHGMNG_GetChgCurrent();
        
        //CHGMNG_PRINT:"CHGMNG:chg_current:%d"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_988_112_2_17_23_3_27_416,(uint8*)"d",chg_current);
        
        if ( (_CHGMNG_GetVbatVol() > chg_param.bat_safety_vol) && (chg_current < CHGMNG_STOP_VPROG))
        {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_VBATEND);
            //CHGMNG_PRINT:"CHGMNG:STOP CHGMNG_VBATEND!"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_993_112_2_17_23_3_27_417,(uint8*)"");
            return;
        }

        //CHG_PHY_ShutDown();
        module_state.bat_cur_vol = _CHGMNG_GetVbatVol();
        //CHG_PHY_TurnOn();
#if 0
        if ((module_state.bat_statistic_vol >= (chg_param.chg_end_vol + 60)) )
        {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_CHARGEDONE);
            CHGMNG_PRINT(("CHGMNG:STOP CHGMNG_VBATEND! mingwei !!!"));
            return;
        }
#endif        
        
        if (charge_endtime_counter == 0)
        {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_TIMEOUT);
            //CHGMNG_PRINT:"CHGMNG:STOP CHGMNG_TIMEOUT!"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1004_112_2_17_23_3_27_418,(uint8*)"");
            return;
        }
        else
        {
            if (_CHGMNG_GetPhoneState() != BUSYSTATE) //When phone is in busy, we will stop timing.
            {
                charge_endtime_counter--;
            }
        }

#ifdef CHGMNG_OTP_SUPPORT
		if((module_state.charging_temperature <= otp_param.over_low)
           ||(module_state.charging_temperature >= otp_param.over_high))
		{
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_OVERTEMP);
        	return;
		}
#endif 
    }

    switch (module_state.chgmng_state)
    {
        case CHGMNG_STARTING:
            break;

        case CHGMNG_CHARGING:
            _CHGMNG_VbatQueueUpdate (module_state.bat_cur_vol);
            _CHGMNG_CurrentQueueUpdate (chg_current);
#ifdef CHG_SHARP_FEATRUE
            if (module_state.charging_current <= 200)
            {
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_CHG_TO_PULSE,0);
                plus_cnt = PLUS_TIME_CNT;
            }
#else
            if ( (module_state.bat_statistic_vol < chg_param.chg_end_vol) && (0 == vbat_queue.pointer))
            {
                if (module_state.charging_current < CHGMNG_SWITCH_CV_VPROG)
                {
                    hw_switch_point = (CHG_SWITPOINT_E) CHG_PHY_UpdateSwitchoverPoint (TRUE);
                    //CHGMNG_PRINT:"CHGMNG:UpdateSwitchoverPoint!"
                    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1039_112_2_17_23_3_27_419,(uint8*)"");
                    
                    _CHGMNG_VBatQueueInit (module_state.bat_statistic_vol,CHG_VBAT_BUFF_SIZE);
                }
            }
            else if ( (module_state.bat_statistic_vol >= chg_param.chg_end_vol) && (0 == vbat_queue.pointer))
            {
                if (module_state.charging_current >= CHGMNG_STOP_VPROG)
                {
                    _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_CHG_TO_PULSE,0);
                    
                    _CHGMNG_VBatQueueInit (module_state.bat_statistic_vol,CHG_VBAT_BUFF_SIZE/(CHGMNG_PLUS_TIMES + 1));
                }
                else if (module_state.charging_current  < CHGMNG_STOP_VPROG)
                {
                    _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_CHARGEDONE);                    
                    return;
                }
            }
#endif
            CHG_PHY_SetRecharge();
            break;

        case CHGMNG_PULSECHARGING:
#ifdef CHG_SHARP_FEATRUE		
            _CHGMNG_VbatQueueUpdate (module_state.bat_cur_vol);
            _CHGMNG_CurrentQueueUpdate (chg_current);
            if(plus_cnt == 0)
             {
                    _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_CHARGEDONE);                    
                    return;
             }
            plus_cnt--;
            CHG_PHY_SetRecharge();
#else
            if (pulse_status == SCI_TRUE)
            {
                charge_pulse_times++;
                _CHGMNG_CurrentQueueUpdate (chg_current);
                
                if (charge_pulse_times == CHGMNG_PLUS_TIMES)
                {
                    CHG_PHY_ShutDown();
                    pulse_status = SCI_FALSE;
                    charge_pulse_times = 0;
                }
            }
            else
            {
                _CHGMNG_VbatQueueUpdate (module_state.bat_cur_vol);
                CHG_PHY_TurnOn();
                CHG_PHY_SetRecharge();//pulse high
                pulse_status = SCI_TRUE;

                if (0 == vbat_queue.pointer)
                {
                    if ( (module_state.bat_statistic_vol >= chg_param.chg_end_vol))
                    {
                        _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_CHARGEDONE);
                    }
                    else
                    {
                        _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_PULSE_TO_CHG,0);
                        _CHGMNG_VBatQueueInit (module_state.bat_statistic_vol,CHG_VBAT_BUFF_SIZE);
                    }
                }
            }
#endif                
            break;
        case CHGMNG_STOPPING:
            {
                //CHGMNG_PRINT:"CHGMNG:STOPPING! recharge:%d"
                //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1098_112_2_17_23_3_27_420,(uint8*)"d",chg_param.rechg_vol);

                module_state.bat_cur_vol = _CHGMNG_GetVbatVol();
                _CHGMNG_VbatQueueUpdate (module_state.bat_cur_vol);
                
                if ((module_state.charging_stop_reason != CHGMNG_OVERVOLTAGE)
                    &&((module_state.charging_stop_reason != CHGMNG_OVERTEMP)))
                {
                    if (module_state.bat_statistic_vol < chg_param.rechg_vol)
                    {
                        _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG,CHGMNG_RECHARGE);
                    }
                }
                
#ifdef CHGMNG_OTP_SUPPORT                	
				if((module_state.charging_stop_reason == CHGMNG_OVERTEMP)
                    &&(module_state.charging_temperature >= otp_param.resume_low)
                    &&(module_state.charging_temperature <= otp_param.resume_high))
				{
					//CHGMNG_PRINT:"CHGMNG:OTP enable charger!"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1117_112_2_17_23_3_27_421,(uint8*)"");
                    _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG,CHGMNG_FAULT_RESUME);
				}
#endif                
            }
            break;
        default:
            break;
    }
   
}

LOCAL void _CHGMNG_CalVchgBg(void)
{
#ifdef CHG_SHARP_FEATRUE    
    if(adc_cal.cal_flag != CAL_TYPR_NO)
    {
        uint32 i,bg_vol;
        uint32 adc_value = 0;

        for(i = 0; i < 16; i++)
        {
        	adc_value += ADC_GetResultDirectly (ADC_CHANNEL_VCHGBG, SCI_TRUE);
        }
        adc_value = adc_value >> 4;

        bg_vol = CHGMNG_AdcvalueToVoltage (adc_value);

        bg_vol = (VBAT_VOL_DIV_P1*bg_vol)/VBAT_VOL_DIV_P2;

        for(i = 0; i < 32; i++){
            if(bg_vol < bgvol_map_cccv[i][0])
                break;
        }
        if(i == 0) {
            hw_switch_point = bgvol_map_cccv[i][1];
        }else{
            hw_switch_point = bgvol_map_cccv[i - 1][1];
        }
        CHG_PHY_SetSwitchoverPoint (hw_switch_point);
    }
#endif	    
}
/*****************************************************************************/
//  Description:    Enter Charge start state.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
LOCAL void _CHGMNG_StartCharge (CHGMNG_CHG_START_TYPE_E start_type)
{
    //to detect charge unplug.
    //CHGMNG_PRINT:"CHGMNG:CHGMNG_ChargeStartHandler!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1134_112_2_17_23_3_27_422,(uint8*)"");

    _CHGMNG_VBatQueueInit (module_state.bat_statistic_vol,CHG_VBAT_BUFF_SIZE);
#ifndef CHG_SHARP_FEATRUE
    CHG_PHY_SetSwitchoverPoint (hw_switch_point);
#endif

    CHG_PHY_TurnOn();       //Ensure that charge module can be turned on always.

    if (start_type != CHGMNG_RECHARGE)
    {
#ifdef SC6531_CHG_CUR_SEARCH
        if(!charger_plugin_flag){
#endif
        /*If it is not a restart recharging, send start message.*/
        //CHGMNG_PRINT:"CHGMNG:CHR_CHARGE_START_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1145_112_2_17_23_3_27_423,(uint8*)"");

        _CHGMNG_SendMessagetoClient (CHR_CHARGE_START_IND, 0); 
#ifdef SC6531_CHG_CUR_SEARCH
        }
#endif
    }
    
#ifdef SC6531_CHG_CUR_SEARCH
    if(charger_plugin_flag)
    {
        charger_plugin_flag = 0;
    }
#endif

    if (start_type == CHGMNG_ADP_PLUGIN)
    {
        if (g_ovp_timer)
        {
            SCI_ChangeTimer (g_ovp_timer, _CHGMNG_OvpTimerHandler, OVP_TIMER_INTERVAL);
            if (SCI_ActiveTimer (g_ovp_timer))
            {
                //CHGMNG_PRINT:"CHGMNG:Invalid g_ovp_timer timer pointer!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1157_112_2_17_23_3_27_424,(uint8*)"");
            }
        }

        ovp_detect_counter = 0;
        if (g_charging_timer)
        {
            SCI_ChangeTimer (g_charging_timer, _CHGMNG_ChargeTimerHandler, CHARGING_TIMER_INTERVAL);
            if (SCI_ActiveTimer (g_charging_timer))
            {
                //CHGMNG_PRINT:"CHGMNG:Invalid application timer pointer!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1167_112_2_17_23_3_27_425,(uint8*)"");
            }
        }
    }

    module_state.charging_stop_reason = CHGMNG_INVALIDREASON;
    
    charge_endtime_counter = (chg_param.chg_timeout * 1000)/CHARGING_TIMER_INTERVAL;     //set end time
    
    _CHGMNG_CurrentQueueInit (_CHGMNG_GetChgCurrent());
    _CHGMNG_TempQueueInit(_CHGMNG_GetCurrentTemp());

    _CHGMNG_CalVchgBg();
}


/*****************************************************************************/
//  Description:    Stop charge.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_StopCharge (CHGMNG_STOPREASON_E reason)
{
    module_state.charging_stop_reason = reason;

    _CHGMNG_VBatQueueInit (module_state.bat_statistic_vol,DISCHG_VBAT_BUFF_SIZE);
    _CHGMNG_CurrentQueueInit (0);
    CHG_PHY_StopRecharge();
    CHG_PHY_ShutDown();
    
    switch (reason)
    {
        case CHGMNG_CHARGERUNPLUG:

            if (g_charging_timer)
            {
                if (SCI_DeactiveTimer (g_charging_timer))
                {
                    //CHGMNG_PRINT:"CHGMNG:Invalid application timer pointer!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1200_112_2_17_23_3_27_426,(uint8*)"");
                }
            }

            //add by paul begin
            if (g_ovp_timer)
            {
                if (SCI_DeactiveTimer (g_ovp_timer))
                {
                    //CHGMNG_PRINT:"[Vcharge]CHGMNG:Invalid application timer pointer!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1209_112_2_17_23_3_27_427,(uint8*)"");
                }
            }

            //CHGMNG_PRINT:"CHGMNG:CHR_CHARGE_DISCONNECT1!!!!!!!!!!!!!!!!!!!!!!!!!!!"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1213_112_2_17_23_3_27_428,(uint8*)"");
            _CHGMNG_SendMessagetoClient (CHR_CHARGE_DISCONNECT, 0);     
            break;

        case CHGMNG_TIMEOUT:
        case CHGMNG_VBATEND:
        case CHGMNG_CHARGEDONE:
            //CHGMNG_PRINT:"CHGMNG:CHR_CHARGE_FINISH!!!!!!!!!!!!!!!!!!!!!!!!!!!"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1220_112_2_17_23_3_27_429,(uint8*)"");
            _CHGMNG_SendMessagetoClient (CHR_CHARGE_FINISH, 0);
            break;

        case CHGMNG_OVERVOLTAGE:
        case CHGMNG_OVERTEMP:
            _CHGMNG_SendMessagetoClient (CHR_CHARGE_FAULT, 0); //add by paul
            break;
        default:
            break;
    }
}

/*****************************************************************************/
//  Description:    This function is used to set charge current and charger mode.
//  Author:         Benjamin.Wang
/*****************************************************************************/
LOCAL void _CHGMNG_SetChargeCurrent (CHGMNG_ADAPTER_TYPE_E mode)
{
    switch(mode)
    {
        case CHGMNG_ADP_STANDARD:
            CHG_PHY_SetChgCurrent (chg_param.standard_chg_current);
        break;
        case CHGMNG_ADP_NONSTANDARD:
            CHG_PHY_SetChgCurrent (chg_param.nonstandard_chg_current);
        break;
        case CHGMNG_ADP_USB:
            CHG_PHY_SetChgCurrent (chg_param.usb_chg_current);
        break;
        case CHGMNG_ADP_UNKNOW:
        default:
            CHG_PHY_SetChgCurrent (CHARGER_CURRENT_300MA);
    }
}
/*****************************************************************************/
//  Description:    Default callback function. Called if client has no callback function..
//                      This is a dummy function.
//  Author:         Benjamin.Wang
/*****************************************************************************/
LOCAL void _CHGMNG_SrvDefault (BLOCK_ID id, uint32 argc, void *argv)
{
}

/**---------------------------------------------------------------------------*
 **                     PUBLIC Function Definitions                           *
 **---------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description:    This function initialize the Charge manager. Before starting charge
//                      manager, you'd better set the essential parameters according to your need.
//                      Otherwise, it will use the default parameters.
//  Return:      
//  Author:         Benjamin.Wang
//  Note:           This module should be initialized after GPIO_Init.
/*****************************************************************************/
PUBLIC uint32  CHGMNG_Init (void)
{
    // create client list.
    SCI_CreateClientList (CHR_SERVICE, (CHR_MSG_MAX_NUM & 0x0ff),
                          (REG_CALLBACK) _CHGMNG_SrvDefault);

    CHG_PHY_Init();     //Charger control initial setup

    CHG_PHY_SetSwitchoverPoint (hw_switch_point);   //Set hardware cc-cv switch point

    CHG_PHY_SetChgCurrent(CHARGER_CURRENT_300MA);
    
    /*Registe timers*/
    if (NULL == g_charging_timer)    //stop state timer
    {
        g_charging_timer = SCI_CreatePeriodTimer ("CHGTIMER",
                           _CHGMNG_ChargeTimerHandler,
                           module_state.chgmng_state,
                           CHARGING_TIMER_INTERVAL,
                           SCI_NO_ACTIVATE);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE, ("CHGMNG:Stop timer has been created!"));/*assert verified*/
    }

    if (NULL == g_ovp_timer)    //stop state timer
    {
        g_ovp_timer = SCI_CreatePeriodTimer ("OVPTIMERDECT",
                            _CHGMNG_OvpTimerHandler,
                            module_state.chgmng_state,
                            OVP_TIMER_INTERVAL,
                            SCI_NO_ACTIVATE);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE, ("CHGMNG:Stop timer has been created!"));/*assert verified*/
    }
#ifdef SC6531_CHG_CUR_SEARCH
    if (NULL == g_6531_search_cur_timer)    //stop state timer
    {
        g_6531_search_cur_timer = SCI_CreatePeriodTimer ("SEARCHCHGCUR",
                            _CHGMNG_SearchTimerHandler,
                            module_state.chgmng_state,
                            SEARCH_TIMER_INTERVAL,
                            SCI_NO_ACTIVATE);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE, ("CHGMNG:Stop timer has been created!"));/*assert verified*/
    }
#endif
    _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_INIT,0);

    chg_task_id  = SCI_CreateThread("CHGMNG_SRV",
									"Q_CHGMNG_SRV",
									_CHGMNG_SrvThread ,
									0,
									0,
									CHGMNG_STACK_SIZE ,
									CHGMNG_QUEUE_NUM,
									TASK_CHGMNG_SRV_PRI ,
									SCI_PREEMPT,
									SCI_AUTO_START);
    
    _CHGMNG_CheckVbatPresent();

    module_state.bat_cur_vol = _CHGMNG_GetVbatVol();

    _CHGMNG_VBatQueueInit(module_state.bat_cur_vol,DISCHG_VBAT_BUFF_SIZE);     //init statistic program
    
 #ifndef DUAL_BATTERY_SUPPORT   
    if(module_state.bat_cur_vol < dischg_param.shutdown_vol)
    {
        if(!CHG_PHY_IsChargerPresent())
    	 {
            POWER_PowerOff();
         }
    }
#endif

    ischgmng_start = SCI_TRUE;      //Charge management has started.
    
    return 0;
}

/*****************************************************************************/
//  Description:    This function is used to close charge manager.
//  Author:         Benjamin.Wang
//  Note:           It can't be called in int handler!
/*****************************************************************************/
PUBLIC void CHGMNG_Close (void)
{
    if (g_charging_timer != NULL)
    {
        SCI_DeactiveTimer (g_charging_timer);
    }
    if (g_ovp_timer != NULL)
    {
        SCI_DeactiveTimer (g_ovp_timer);
    }

    DoIdle_UnRegisterCallback((DOIDLE_CALLBACK_FUNC) _CHGMNG_VBatTimerHandler);
    
    CHG_PHY_ShutDown();
    
    if (ischgmng_start)
    {
        ischgmng_start = SCI_FALSE;
    }
}

/*****************************************************************************/
//  Description:    This function sets charge otp parameter.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC void CHGMNG_SetOtpParam(CHGMNG_OTP_PARAM_T* otp )
{
#ifdef CHGMNG_OTP_SUPPORT
	SCI_PASSERT ((((CHG_NV_TEMP_OFFSET - 100) <= otp->over_low)&& /*assert to do*/
				(otp->over_low <= otp->resume_low)&&
				(otp->resume_low < otp->resume_high)&&
				(otp->resume_high <= otp->over_high)&&
				(otp->over_high< (CHG_NV_TEMP_OFFSET + 200))), 
				("otp parameter error!"));
	
	otp_param = *otp;
#endif	
	return;
}
/*****************************************************************************/
//  Description:    This function set the charger type.
//  Retrun:
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetOvpParam (CHGMNG_OVP_PARAM_T* ovp)
{
    SCI_PASSERT ((ovp != NULL),  ("CHGMNG_SetOvpParam error"));    /*assert verified*/
    SCI_PASSERT ( (ovp->ovp_type== CHARGER_NK_TYPE || ovp->ovp_type == CHARGER_NORMAL_TYPE),  /*assert verified*/
                  ("Ovp type error"));
    ovp_param = *ovp;
}


/*****************************************************************************/
//  Description:    This function get the charger type.
//  Retrun:
//  Note:
/*****************************************************************************/
PUBLIC CHGMNG_OVP_PARAM_T CHGMNG_GetOvpParam (void)
{
    return ovp_param;
}

/*****************************************************************************/
//  Description:    This function sets the charging parameter.
//  Retrun:
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetChgParam (CHGMNG_CHARGE_PARAM_T* chg)
{
    SCI_PASSERT ((chg != NULL), ("CHGMNG_SetChgParam err!"));   /*assert verified*/

    chg_param = *chg;
}

/*****************************************************************************/
//  Description:    This function gets the charging parameter.
//  Retrun:         recharge_voltage value.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC CHGMNG_CHARGE_PARAM_T CHGMNG_GetChgParam (void)
{
    return chg_param;
}

/*****************************************************************************/
//  Description:    Set the discharge parameter.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetDischgParam (CHGMNG_DISCHARGE_PARAM_T* dischg)
{
    SCI_PASSERT ((dischg != NULL), ("WarningVoltage Setting is too low!"));/*assert verified*/

    dischg_param = *dischg;
}

/*****************************************************************************/
//  Description:    Get the discharge parameter.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC CHGMNG_DISCHARGE_PARAM_T CHGMNG_GetDischgParam (void)
{
    return dischg_param;
}

/*****************************************************************************/
//  Description:    Get the hardware switch point which is from cc to cv.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CHGMNG_GetHWSwitchPoint (void)
{
    return hw_switch_point;
}

/*****************************************************************************/
//  Description:    Set the hardware switch point which is from cc to cv.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetHWSwitchPoint (CHG_SWITPOINT_E value)
{
#ifndef CHG_SHARP_FEATRUE
    hw_switch_point = value;
    CHG_PHY_SetSwitchoverPoint (value);
#endif
}

/*****************************************************************************/
//  Description:    Set the vbat calibration table.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetVBatAdcCal(CHGMNG_ADC_CAL_T *cal_info)
{
    if(cal_info != NULL)
    {
        adc_cal = *cal_info;
    }
}

/*****************************************************************************/
//  Description:    Set the voltage capacity table.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetVBatCapTable (uint16 *dischg_ptable, uint16 *chg_ptable)
{
    uint32 i;
    SCI_PASSERT (((dischg_ptable != NULL) && (chg_ptable != NULL)), ("dischg_ptable != NULL!"));/*assert verified*/
    SCI_PASSERT (((dischg_ptable[0] > 0) && (chg_ptable[0] > 0)), ("dischg_ptable[0] > 0"));/*assert verified*/

    for(i = 0; i < BAT_CAPACITY_STEP; i++)
    {
        dischg_bat_capacity_table[i][0] = dischg_ptable[i];
        chg_bat_capacity_table[i][0] = chg_ptable[i];
    }
    
    CHGMNG_PRINT(("dischg_bat_capacity_table[5][0]:%d\n",dischg_bat_capacity_table[5][0] ));
    CHGMNG_PRINT(("dischg_bat_capacity_table[6][0]:%d\n",dischg_bat_capacity_table[6][0] ));
    CHGMNG_PRINT(("chg_bat_capacity_table[5][0]:%d\n",chg_bat_capacity_table[5][0] ));
    CHGMNG_PRINT(("chg_bat_capacity_table[6][0]:%d\n",chg_bat_capacity_table[6][0] ));
}

/*****************************************************************************/
//  Description:    This function is used to set the adapter mode, adapter or usb.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetChargeAdapter (CHGMNG_ADAPTER_TYPE_E mode)
{
    module_state.adp_type = mode;
#ifndef SC6531_CHG_CUR_SEARCH
    _CHGMNG_SetChargeCurrent (mode);
#endif
}

/*****************************************************************************/
//  Description:    This function is used to convert voltage value to ADC value.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CHGMNG_VoltageToAdcvalue (uint16 votage)
{
    int32 result;
    
    result = ((votage - adc_cal.y0_vol)*adc_cal.zoom_in)/adc_cal.slope + adc_cal.x0_adc;
    if(result < 0)
    {
        result = 0;
    }

    //CHGMNG_PRINT:"CHGMNG:CHGMNG_VoltageToAdcvalue votage:%d adc:%d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1510_112_2_17_23_3_28_430,(uint8*)"dd",votage,(uint16) (result));
    return (uint16) (result);
}

/*****************************************************************************/
//  Description:    This function is used to convert ADC value to voltage value.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CHGMNG_AdcvalueToVoltage (uint32 adc)
{
    int32 result;
    int32 adcvalue = (int32)adc;
    
    result = ((adcvalue - adc_cal.x0_adc)*adc_cal.slope)/adc_cal.zoom_in + adc_cal.y0_vol;
    
    if(result < 0)
    {
        result = 0;
    }
    
    return result;
}

#ifdef SC6531_CHG_CUR_SEARCH
LOCAL void _CHGMNG_SearchCurEnd(void)
{
    //CHGMNG_PRINT(("mingwei......_CHGMNG_SearchCurEnd.....charging_current_type:%d\n",charging_current_type ));
    if (g_6531_search_cur_timer)
    {
        if (SCI_DeactiveTimer (g_6531_search_cur_timer))
        {
            CHGMNG_PRINT(("[Vcharge]CHGMNG:Invalid application timer pointer!"));
        }
    }
    CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PLUG_IN_MSG,0);
}
LOCAL void _CHGMNG_SearchTimerHandler (uint32 state)
{
    uint32 vprog_adc;
    static uint32 search_counter = 0;
    static uint32 trigger_couter = 0;

    vprog_adc = ADC_GetResultDirectly (ADC_CHANNEL_PROG, SCI_FALSE);
    //CHGMNG_PRINT(("mingwei...........vprog_adc:%d\n",vprog_adc ));
    search_counter++; 
    if (vprog_adc < (30))
    {
        trigger_couter++;
    }
    if(search_counter > SEARCH_COUNTER)
    {
        if(trigger_couter > TRIGGER_COUNTER)    //chg current low
        {
            if(charging_current_type <= 300)    //search end
            {
 	              chg_param.standard_chg_current = (charging_current_type*100)/charger_resistor_param;    
                  chg_param.usb_chg_current = (charging_current_type*100)/charger_resistor_param; 
		          chg_param.nonstandard_chg_current = (charging_current_type*100)/charger_resistor_param;
                _CHGMNG_SearchCurEnd();
            }
            else
            {
                charging_current_type -= 100;
                CHG_PHY_SetChgCurrent (charging_current_type);
                //CHGMNG_PRINT(("mingwei...........charging_current_type:%d\n",charging_current_type ));
            }
        }
        else    //search end
        {
            if(charging_current_type > 300)
            {
                charging_current_type -= 50;
                CHG_PHY_SetChgCurrent (charging_current_type);  
            }
            chg_param.standard_chg_current = (charging_current_type*100)/charger_resistor_param;    
            chg_param.usb_chg_current = (charging_current_type*100)/charger_resistor_param; 
     		chg_param.nonstandard_chg_current = (charging_current_type*100)/charger_resistor_param;
			_CHGMNG_SearchCurEnd();
        }
        search_counter = 0;
        trigger_couter = 0;
    }
}

LOCAL void _CHGMNG_SC6531_ChgCurSearch(void)
{
    if (g_6531_search_cur_timer)
    {
        SCI_ChangeTimer (g_6531_search_cur_timer, _CHGMNG_SearchTimerHandler, SEARCH_TIMER_INTERVAL);
        if (SCI_ActiveTimer (g_6531_search_cur_timer))
        {
            CHGMNG_PRINT(("CHGMNG:Invalid g_ovp_timer timer pointer 6531!"));
        }
    }    
}
#endif

/*****************************************************************************/
//  Description:    When charger pulgin/unplug interrupt happened, this function is called.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_ChargerPlugInHandler (uint32 gpio_id, uint32 gpio_state)
{
    if (!ischgmng_start)
    {
        return;
    }
    
    if (CHG_PHY_IsChargerPresent())
    {
#ifdef SC6531_CHG_CUR_SEARCH
        CHG_PHY_TurnOn();
        CHG_PHY_SetRecharge();//pulse high
        charger_plugin_flag = 1;
        charging_current_type = SC6531_FIX_CURRENT;
        CHG_PHY_SetChgCurrent (charging_current_type);

        _CHGMNG_SendMessagetoClient (CHR_CHARGE_START_IND, 0);
        _CHGMNG_SC6531_ChgCurSearch();
#else
        CHG_PHY_SetChgCurrent(CHARGER_CURRENT_300MA);
        CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PLUG_IN_MSG,0);
#endif
    }
    else
    {
        CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PLUG_OUT_MSG,0);
    }
  
    module_state.charging_stop_reason = CHGMNG_INVALIDREASON;// when charge plug out ,to make sure charge can check voltage right
}

/*****************************************************************************/
//  Description:    This function is used to get VBAT ADC value.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHGMNG_GetVBATADCValue (void)
{
    return _CHGMNG_GetVBATADCResult();
}
/*****************************************************************************/
//  Description:    Check charger connect.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
BOOLEAN CHGMNG_IsChargeConnect (void)
{
#if defined(FPGA_SUPPORT_SC6530) || defined(FPGA_SUPPORT_SC6531) || defined(FPGA_SUPPORT_SC6531EFM)  || defined(FPGA_SUPPORT_UIX8910)
    return SCI_FALSE;
#else
    return CHG_PHY_IsChargerPresent();
#endif
}
/*****************************************************************************/
//  Description:    identify charger type.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC CHGMNG_ADAPTER_TYPE_E CHGMNG_IdentifyAdpType(void)
{
    return CHG_PHY_IdentifyAdpType();
}

/*****************************************************************************/
//  Description:    Get charge module state information.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC CHGMNG_STATE_INFO_T* CHGMNG_GetModuleState(void)
{
    SCI_DisableIRQ(); 
    module_state_to_app = module_state;
    SCI_RestoreIRQ();

    if(!ischgmng_start)
    {
        module_state_to_app.bat_cur_vol = _CHGMNG_GetVbatVol();
    }
    
    return &module_state_to_app;
}
/*****************************************************************************/
//  Description:    send massage to charge task.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC void CHGMNG_SendMsgToChgTask(CHGMNG_MSG_E sig,uint32 sig_param)
{
    CHGMNGSVR_SIG_T *signal = PNULL;
    
    SCI_CREATE_SIGNAL(
        signal,
        sig,
        sizeof (CHGMNGSVR_SIG_T),
        SCI_IDENTIFY_THREAD());
    
    signal->sig_param = sig_param;

    SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, chg_task_id);   
}

#ifdef DUAL_BATTERY_SUPPORT

////dual battery
/*****************************************************************************/
//  Description:    reset charge module for dual battery module.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_ModuleReset(uint32 voltage)
{

    //CHGMNG_PRINT:"CHGMNG:CHGMNG_ModuleReset!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1619_112_2_17_23_3_28_431,(uint8*)"");

    _CHGMNG_CurrentQueueInit (_CHGMNG_GetChgCurrent());
    
    if(CHGMNG_CHARGING == module_state.chgmng_state)
    {
        _CHGMNG_VBatQueueInit (voltage,CHG_VBAT_BUFF_SIZE);
    }
    else if(CHGMNG_PULSECHARGING == module_state.chgmng_state)
    {
        _CHGMNG_VBatQueueInit (voltage,CHG_VBAT_BUFF_SIZE/(CHGMNG_PLUS_TIMES + 1));
    }
    else
    {
        _CHGMNG_VBatQueueInit (voltage,DISCHG_VBAT_BUFF_SIZE);
    }

     module_state.bat_remain_cap = _CHGMNG_VoltagetoPercentum (module_state.bat_statistic_vol,module_state.chgmng_state,SCI_TRUE);

    warning_counter = 0;
    
    _CHGMNG_SetChargeCurrent (module_state.adp_type);
    isshutting_down = SCI_FALSE; 
}

/*****************************************************************************/
//  Description:    Convert ADCVoltage to percentrum for dual battery.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHGMNG_VoltagetoPercentumDBat (uint32 bat, uint32 voltage,BOOLEAN init)
{
    uint16 percentum;
    int32 temp;
    int pos = 0;

    for(pos = 0; pos < BAT_CAPACITY_STEP -1; pos++){
        if(voltage > dischg_bat_capacity_table[pos][0])
            break;
    }
    if(pos == 0) {
        percentum = 100;
    }else{
        temp = dischg_bat_capacity_table[pos][1]-dischg_bat_capacity_table[pos-1][1];
        temp = temp*(voltage - dischg_bat_capacity_table[pos][0]);
        temp = temp/(dischg_bat_capacity_table[pos][0] - dischg_bat_capacity_table[pos-1][0]);
        temp = temp + dischg_bat_capacity_table[pos][1];
        if(temp < 0){
            temp = 0;
        }
        percentum = temp;
    }
    
    return percentum;
}

#endif


/////old function dummy
uint32 CHR_GetVoltage (void){return 0;}
uint32 CHR_GetBatCapacity (void){ return 0; }
uint32 CHR_GetBattState (void){ return 0;}
uint32 CHR_SetBattIntRes (uint32 level){ return 0;}
uint32 CHR_SetTPulseOutCharge (uint32 level){ return 0;}
uint32 CHR_SetTPulseInCharge (uint32 level){ return 0;}
uint32 CHR_SetBattLevelMax (uint32 level){ return 0;}
uint32 CHR_SetBattLevelMin (uint32 level){ return 0;}
uint32 CHR_CheckBatteryStaus (void){ return 0;}
void CHR_StopCharge (void){}
void CHR_StartCharge (void){}
uint32 CHR_GetCurVoltage (void)
{
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();

    return p_chgmng_info->bat_cur_vol / 10;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of charge.c


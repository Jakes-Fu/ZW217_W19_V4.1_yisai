/******************************************************************************
** File Name:    	Thermal_phy.c                                                				*
** Author:         	Tianyu.Yang                                                   				*
** DATE:           	22/02/2023                                               					*
** Copyright:      	2023 Spreadtrum, Incoporated. All Rights Reserved.         		*
** Description:    	This file defines the basic thermalr manage operation process.   	*
*******************************************************************************
*******************************************************************************
**                        Edit History                                       						*
** ------------------------------------------------------------------------- 
** DATE                NAME              	DESCRIPTION                               			*
** 22/02/2023      Tianyu.Yang       	Create.                                   				*
** 22/02/2023      Tianyu.Yang       	Modify									*
******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "Thermal.h"
#include "sci_types.h"
#include "sci_api.h"
#include "doidleonpage.h"
#include "priority_system.h"
#include "adc_drvapi.h"
#include "uws6121e_module_config.h"
#include "sci_service.h"
#include "chg_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    						*
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     						*
 **---------------------------------------------------------------------------*/
PUBLIC void _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_E fsm_event,uint32 condition);

#define	THERMAL_STACK_SIZE	1.5*1024
#define	THERMAL_QUEUE_NUM	10
#define   TASK_THERMAL_SRV_PRI    PRI_THERMAL_SRV_TASK   //SCI_PRIORITY_NORMAL
#define	CLOSE_BUSINESS		43
#define	REOPEN_BUSINESS	40
#define	SKIN_POWEROFF 	58
#define	CURRENT_OFF 	45
#define	CURRENT_ON 	44

LOCAL BLOCK_ID thermal_task_id = NULL;
LOCAL uint32 body_temperature_flag = 0;
uint8 body_temperature_shutdown_ctrl = 1;
extern CHGMNG_STATE_INFO_T module_state;

PUBLIC void thermal_init(void)
{
    	// Create Client List.
	SCI_CreateClientList (THERMAL_SERVICE, (THERMAL_MSG_MAX_NUM & 0x0ff),(REG_CALLBACK) THERMAL_SrvDefault);

	if(thermal_task_id == NULL)
	{
	    thermal_task_id  = SCI_CreateThread("THERMAL_SRV",
			"Q_THERMAL_SRV",
			_THERMAL_SrvThread ,
			0,
			0,
			THERMAL_STACK_SIZE ,
			THERMAL_QUEUE_NUM,
			68 ,
			SCI_PREEMPT,
			SCI_AUTO_START);
	}
}

LOCAL void _THERMAL_SrvThread (uint32 argc, void *argv)
{
	THERMALSVR_SIG_T *	signal;
	DoIdle_RegisterCallback ( (DOIDLE_CALLBACK_FUNC) _THERMAL_TimerHandler);
	for(;;) {
		signal = (THERMALSVR_SIG_T *) SCI_GetSignal (thermal_task_id);

		switch (signal->SignalCode) {
		case THERMAL_MONITOR_MSG:
			THERMAL_Read_Temperature();
			break;
		default:
			SCI_PASSERT (0,("Error msg!!!"));   /*assert verified*/
			break;
		}

		SCI_FREE(signal);
		signal = NULL;
		}
}

LOCAL uint32 _THERMAL_TimerHandler (uint32 param)
{
	if(THERMAL_Ctrl_Get())
	{
		THERMAL_SendMsgToTHMTask(THERMAL_MONITOR_MSG,0);
	}
	else
	{
		thermal_module_state.thermal_state = STOP_TERMAL;
		SCI_TRACE_LOW("[THERMAL]:STOP_TERMAL_From_AT, status=%d\n", thermal_module_state.thermal_state);
	}
	return 0;
}

PUBLIC void THERMAL_SendMsgToTHMTask(THERMAL_MSG_E sig,uint32 sig_param)
{
	THERMALSVR_SIG_T *signal = PNULL;

	SCI_CREATE_SIGNAL(
		signal,
		sig,
		sizeof (THERMALSVR_SIG_T),
		SCI_IDENTIFY_THREAD());

	signal->sig_param = sig_param;

	SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, thermal_task_id);
}

LOCAL void THERMAL_Read_Temperature (void)
{
	thermal_module_state.skin_temp = THERMAL_Skin_Temp_Read();
	THERMAL_Skin_Temp_Action(thermal_module_state.skin_temp);
}

PUBLIC int32 THERMAL_Skin_Temp_Read(void)
{
    uint32          vol = 0;
    uint32          i = 0;
    int32           temp = 0;
    uint32          table_size = sizeof(skin_temp_table)/sizeof(skin_temp_table[0]);

    vol =  _CHGMNG_GetChannelVol(ADC_CHANNEL_ADCI2, ADC_SCALE_2V444);  //Tianyu.Yang

    for  (i = 0; i < table_size; i++)
    {
        if (vol >= skin_temp_table[i][0])
        {
            break;
        }
    }

    if ( i <  table_size)
    {
        temp = skin_temp_table[i][1];
    }
    else
    {
        temp = skin_temp_table[table_size-1][1];
    }

    return temp;
}

LOCAL void THERMAL_Skin_Temp_Action (int32 skin_temp)
{
	if((skin_temp >= CLOSE_BUSINESS) && (IDLE_TEMP == thermal_module_state.thermal_state))
	{
		SCI_SendEventToClient (THERMAL_SERVICE, THERMAL_CLOSE_BUSINESS, 0);
		SCI_TRACE_LOW("[THERMAL]:Send to MMI CLOSE BUSINESS:%d\n", skin_temp);
		thermal_module_state.thermal_state = SKIN_TEMP;
	}

	if((skin_temp < REOPEN_BUSINESS) && (SKIN_TEMP == thermal_module_state.thermal_state))
	{
		SCI_SendEventToClient (THERMAL_SERVICE, THERMAL_REOPEN_BUSINESS, 0);
		SCI_TRACE_LOW("[THERMAL]:Send to MMI REOPEN BUSINESS:%d\n", skin_temp);
		thermal_module_state.thermal_state = IDLE_TEMP;
	}

	if((skin_temp >= SKIN_POWEROFF))
	{
		SCI_TRACE_LOW("[THERMAL]:Send to MMI POWER_OFF:%d\n", skin_temp);
		SCI_SendEventToClient (THERMAL_SERVICE, THERMAL_POWER_OFF, 0);
	}
	else if((skin_temp >= CURRENT_OFF) && (module_state.chgmng_state == CHGMNG_CHARGING))
	{
		SCI_TRACE_LOW("[Thermal]: body temperature is high stop charging = %d", skin_temp);
		CHGMNG_FSM_TAKE_THERMAL(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_OVERTEMP);
		body_temperature_flag = 1;
	}
	else if((skin_temp < CURRENT_ON) && (module_state.chgmng_state == CHGMNG_IDLE) && (body_temperature_flag == 1))
	{
		SCI_TRACE_LOW("[Thermal]: body temperature is recovery resume charging = %d", skin_temp);
		CHGMNG_FSM_TAKE_THERMAL(CHGMNG_FSM_EVENT_START_CHG, 0);
		body_temperature_flag = 0;
	}
	SCI_TRACE_LOW("[THERMAL]:thermal_state=%d,skin_temp=%d\n",thermal_module_state.thermal_state, skin_temp);
}

PUBLIC uint32 THERMAL_Ctrl_Get (void)
{
	return body_temperature_shutdown_ctrl;
}

PUBLIC void THERMAL_Ctrl_Set (uint32 value)
{
	if(value != 0)
	{
		value = 1;
		thermal_module_state.thermal_state = IDLE_TEMP;
	}
	body_temperature_shutdown_ctrl = value;
}

LOCAL void THERMAL_SrvDefault (BLOCK_ID id, uint32 argc, void *argv)
{
}
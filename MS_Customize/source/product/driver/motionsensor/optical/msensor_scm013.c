/******************************************************************************
 ** File Name:      proximity_sensor.c                                         *
 ** Author:         vine.yuan                                             *
 ** DATE:           06/29/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/29/2011     vine.yuan     Create. 
 *
 ******************************************************************************/

 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "os_api.h"
#include "adc_drvapi.h"
#include "msensor_drv.h"
#include "msensor_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif

#define TIMER_PERIOD_PROXSENSOR   (200)
#define PROX_SENSOR_ADC_THRETHOLD  (70)
#define PROX_SENSOR_ADC_FLUCTANT_GAP  (8)


LOCAL SCI_TIMER_PTR 		s_proxsensor_timer	= PNULL;

extern void GPIO_ProximitySensorTXEn (BOOLEAN b_on);
extern void GPIO_ProximitySensorRXEn (BOOLEAN b_on);

LOCAL uint32 scm013_init(void);
LOCAL uint32 scm013_open(void);
LOCAL uint32 scm013_close(void);

LOCAL uint32 scm013_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr);

PUBLIC MSENSOR_CFG_T g_scm013_msensor_info = 
{
    "scm013",
	MSENSOR_TYPE_OPTICAL,
    MSENSOR_STATUS_NOT_INIT,
	scm013_init,
	scm013_open,
	scm013_close,
	scm013_ioctl
};


LOCAL void scm013_TimerCallback(uint32 param)
{
    SCI_ASSERT(s_proxsensor_timer != PNULL);   /*assert verified*/
    
    MSensor_TimerHandler(param);
	SCI_ChangeTimer(s_proxsensor_timer, scm013_TimerCallback, TIMER_PERIOD_PROXSENSOR);
	SCI_ActiveTimer(s_proxsensor_timer);
	return ;
}

/*****************************************************************************/
//  Description:    Init g-sensor chip
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 scm013_init(void)
{
	return MSENSOR_SUCCESS;
}


// base on testing result,here select 400ms is appropriate

#define PROXSENSOR_OPEN_DELAY (200)

LOCAL void scm013_delay(uint sleep_delay)
{
   uint32 cur_ticks = 0;
   cur_ticks = SCI_GetTickCount();
   while ((SCI_GetTickCount() - cur_ticks) < sleep_delay) ;
}

LOCAL uint32 scm013_open(void)
{

	GPIO_ProximitySensorRXEn(TRUE);
	SCI_Sleep(PROXSENSOR_OPEN_DELAY);

	if(PNULL == s_proxsensor_timer)
	{
		s_proxsensor_timer = SCI_CreateTimer("scm013_Timer", scm013_TimerCallback,
			MSENSOR_TYPE_OPTICAL, TIMER_PERIOD_PROXSENSOR, SCI_AUTO_ACTIVATE);
	}
	else
	{
		SCI_ChangeTimer(s_proxsensor_timer, scm013_TimerCallback, TIMER_PERIOD_PROXSENSOR);
		SCI_ActiveTimer(s_proxsensor_timer);
	}
	return MSENSOR_SUCCESS;
}


LOCAL uint32 scm013_close(void)
{
	if(s_proxsensor_timer != PNULL)
	{
		SCI_DeactiveTimer(s_proxsensor_timer);
	}

    GPIO_ProximitySensorTXEn(FALSE);
    GPIO_ProximitySensorRXEn(FALSE);

    return MSENSOR_SUCCESS;
}



#define PROXSENSOR_CLOSE_LIMIT (45)
#define PROXSENSOR_FARAWAY_LIMIT (30)
#define PROXSENSOR_SLEEP_TIME (30)


LOCAL BOOLEAN scm013_proc(void * data_ptr)
{
    uint32 Ambent_Adc =0;
    uint32 Work_Adc = 0;
    static uint32 cur_status = MSENSOR_OPTICAL_NONE; 

    SCI_ASSERT(data_ptr != PNULL);  /*assert verified*/
    *(uint32 *)data_ptr = MSENSOR_OPTICAL_NONE;

    GPIO_ProximitySensorTXEn(FALSE);
    scm013_delay(PROXSENSOR_SLEEP_TIME);
    Ambent_Adc = ADC_GetResultDirectly(ADIN_1, FALSE);

    GPIO_ProximitySensorTXEn(TRUE);
    scm013_delay(PROXSENSOR_SLEEP_TIME);
    Work_Adc = ADC_GetResultDirectly(ADIN_1, FALSE);

    GPIO_ProximitySensorTXEn(FALSE);
    //SCI_TraceLow:"[Kui Wang]Work_Adc=%d\t,Ambent_Adc=%d\t,Work_Adc - Ambent_Adc=%d\t,cur_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_SCM013_148_112_2_18_0_34_25_1944,(uint8*)"dddd",Work_Adc,Ambent_Adc,Work_Adc - Ambent_Adc,cur_status);

    if(MSENSOR_OPTICAL_NONE == cur_status)
    {
        if(Work_Adc  > Ambent_Adc + PROXSENSOR_CLOSE_LIMIT)
        {
            cur_status = MSENSOR_OPTICAL_STH_DETECTED;
            *(int32 *)data_ptr = cur_status;	
        }
        else
        {
            cur_status = MSENSOR_OPTICAL_NO_DETECTED;
            *(int32 *)data_ptr = cur_status;	
        }
    }
    else if(MSENSOR_OPTICAL_NO_DETECTED == cur_status)
    {
        if(Work_Adc  > Ambent_Adc + PROXSENSOR_CLOSE_LIMIT)
        {
            cur_status = MSENSOR_OPTICAL_STH_DETECTED;
            *(int32 *)data_ptr = cur_status;
        }
    }
    else if(MSENSOR_OPTICAL_STH_DETECTED == cur_status)
    {
        if(Work_Adc  < Ambent_Adc + PROXSENSOR_FARAWAY_LIMIT)
        {
            cur_status = MSENSOR_OPTICAL_NO_DETECTED;
            *(int32 *)data_ptr = cur_status;
        }
    }
    else
    {
        *(uint32 *)data_ptr = MSENSOR_OPTICAL_NONE;
    }
    return MSENSOR_SUCCESS;
}

LOCAL uint32 scm013_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr)
{
    uint32 ret = MSENSOR_SUCCESS;
    SCI_ASSERT(data_ptr != PNULL);  /*assert verified*/
    
    switch(cmd)
    {
        case MSENSOR_IOCTL_GET_DATA:
            break;
        case MSENSOR_IOCTL_SET_TIMER:
            break;
        case MSENSOR_IOCTL_GET_EVENT:
            ret = scm013_proc(data_ptr);
            break;
        default:
            break;
    }

    return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif  // End of file

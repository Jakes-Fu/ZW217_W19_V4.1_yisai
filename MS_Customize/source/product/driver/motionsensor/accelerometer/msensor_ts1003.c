/******************************************************************************
 ** File Name:      msensor_ts1003.c                                     	  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           10/28/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    driver for msensor_ts1003                                 *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/28/2009     Liangwen.Zhen    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "os_api.h"
#include "msensor_drv.h"
#include "msensor_cfg.h"
#include "power_cfg.h"
#include "gpio_drvapi.h"
#include "gpio_prod_cfg.h"
#include "gpio_prod_api.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define TS1003_USE_AVDDCAMA // NOTE: check it with your specific HW, it deponeds on HW
#define TS1003_DEBUG

#ifdef TS1003_DEBUG
#define  TS1003_PRINT       SCI_TRACE_LOW
#define  MSENSOR_LOG        "[DRV_MSENSOR] "
#else
#define  TS1003_PRINT(_x_)
#endif


/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 TS1003_Init(void);
LOCAL uint32 TS1003_Open(void);
LOCAL uint32 TS1003_Close(void);
LOCAL uint32 TS1003_GetEvent(MSENSOR_MOTION_TYPE_E * event_ptr);
LOCAL uint32 TS1003_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr);
LOCAL void TS1003_TimerCallback(uint32 param);
LOCAL uint32 TS1003_ChangeTimer(uint32 timer_expire);
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
 #define TIMER_PERIOD_TS1003                  500
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL SCI_TIMER_PTR 		s_ts1003_timer	= PNULL;
#define  GPIO_DIR_SENSOR1 69
#define  GPIO_DIR_SENSOR2 72

PUBLIC MSENSOR_CFG_T g_ts1003_msensor_info = 
{
   	"edison ts1003",
	MSENSOR_TYPE_ACCELEROMETER,
    MSENSOR_STATUS_NOT_INIT,
	TS1003_Init,
	TS1003_Open,
	TS1003_Close,
	TS1003_ioctl
};

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description:    This function is used to check if meye module is existed .    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 TS1003_Init(void)
{
	return MSENSOR_SUCCESS;

}

/*****************************************************************************/
//  Description:    This function is used to open meye module    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 TS1003_Open(void)
{

    #ifdef TS1003_USE_AVDDCAMA
        Power_SetSensorVoltage(POWER_SENSOR_MAIN, POWER_VDD_UNUSED, POWER_VDD_2800MV, POWER_VDD_UNUSED);
    #endif
	
	if(s_ts1003_timer == NULL)
	{
        s_ts1003_timer = SCI_CreateTimer("ts1003_Timer", TS1003_TimerCallback,
			MSENSOR_TYPE_ACCELEROMETER, TIMER_PERIOD_TS1003, SCI_AUTO_ACTIVATE);
	}
	else
	{
		SCI_ChangeTimer(s_ts1003_timer, TS1003_TimerCallback, TIMER_PERIOD_TS1003);
		SCI_ActiveTimer(s_ts1003_timer);	
	}
	return MSENSOR_SUCCESS;

} 

/*****************************************************************************/
//  Description:    This function is used to close meye module 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 TS1003_Close(void)
{ 
	if(s_ts1003_timer != NULL)
	{
		SCI_DeactiveTimer(s_ts1003_timer);
	}
    #ifdef TS1003_USE_AVDDCAMA
        Power_SetSensorVoltage(POWER_SENSOR_MAIN, POWER_VDD_UNUSED, POWER_VDD_CLOSED, POWER_VDD_UNUSED);
    #endif	
	
	return MSENSOR_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is used to close meye module 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/

LOCAL uint32 TS1003_GetEvent(MSENSOR_MOTION_TYPE_E * event_ptr)
{
	uint8 E1 = 0;
	uint8 E2 = 0;
    GPIO_CFG_INFO_T_PTR E1_gpio_cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_E1_ID);
    GPIO_CFG_INFO_T_PTR E2_gpio_cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_E2_ID);
   	MSENSOR_MOTION_TYPE_E event_result = MSENSOR_MOTION_TYPE_NONE;
	static MSENSOR_MOTION_TYPE_E s_last_event_result = MSENSOR_MOTION_TYPE_NONE;
   	
   	E1 = GPIO_GetValue(E1_gpio_cfg_ptr->gpio_num);
  	E2 = GPIO_GetValue(E2_gpio_cfg_ptr->gpio_num);
   	
	if      ((E2==0)&&(E1==1))
	{
		event_result = MSENSOR_MOTION_TYPE_RIGHT_SIDE;
	}
	else if ((E2==0)&&(E1==0))
	{
		event_result = MSENSOR_MOTION_TYPE_UP_SIDE;
	}
	else if ((E2==1)&&(E1==0))
	{
		event_result = MSENSOR_MOTION_TYPE_LEFT_SIDE;
	}
	else if ((E2==1)&&(E1==1))
	{
		event_result = MSENSOR_MOTION_TYPE_DOWN_SIDE;
	}
	
    if(s_last_event_result != event_result)
    {   // always report at New Event
     	*event_ptr  = event_result;      
    }
	else
	{	// do NOT report the same event
        *event_ptr = MSENSOR_EVENT_NONE;  
	}

					
    s_last_event_result = event_result;

	return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   TS1003_GetCapability
// Note:          
/******************************************************************************/
LOCAL uint32 TS1003_GetCapability(uint32 * Capability)
{
    *Capability = MSENSOR_MOTION_CAP_XY_AXIS;
    return MSENSOR_SUCCESS;
}
/*****************************************************************************/
//  Description:    This function is used to judge motion type of meye.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 TS1003_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr)
{
	uint32 ret = MSENSOR_SUCCESS;
	

    switch(cmd)
    {
        case MSENSOR_IOCTL_GET_DATA:
        {
            ret = MSENSOR_ERROR_NOT_SUPPORT;
			break;
        }
        
        case MSENSOR_IOCTL_GET_EVENT:
        {
			ret = TS1003_GetEvent((MSENSOR_MOTION_TYPE_E *)data_ptr);
			break;
        }
        case MSENSOR_IOCTL_SET_SENSI:
        {
            ret = MSENSOR_ERROR_NOT_SUPPORT;
			break;
        }        
        case MSENSOR_IOCTL_SET_TIMER:
        {
			ret = TS1003_ChangeTimer(*(uint32 *)data_ptr);
			break;
        }
		case MSENSOR_IOCTL_GET_CAP:
        {
            ret = TS1003_GetCapability((uint32 *)data_ptr);
			break;			
        }   
        default:
            ret = MSENSOR_ERROR_NOT_SUPPORT;
            break;
    }

    return ret;

}

/*****************************************************************************/
//  Description:    This function is used to close meye module 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/

LOCAL void TS1003_TimerCallback(uint32 param)
{
 	MSensor_TimerHandler(param);
	SCI_ChangeTimer(s_ts1003_timer, TS1003_TimerCallback, TIMER_PERIOD_TS1003);
	SCI_ActiveTimer(s_ts1003_timer);
	return ;
}

/******************************************************************************/
// Description:   lis35DE_SetSensi
// Note:          
/******************************************************************************/
LOCAL uint32 TS1003_ChangeTimer(uint32 timer_expire)
{
	SCI_ChangeTimer(s_ts1003_timer, TS1003_TimerCallback, timer_expire);
	SCI_ActiveTimer(s_ts1003_timer);
    return MSENSOR_SUCCESS;
}



/******************************************************************************
 ** File Name:      msensor_drv.c                                             *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           08/31/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of Motion*
 **                 Sensor.                                                   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/31/2009     Liangwen.Zhen    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "ms_customize_trc.h"
#include "os_api.h"
#include "sci_api.h"
#include "gpio_prod_cfg.h"
#include "msensor_drv.h"
#include "msensor_cfg.h"
#include "block_mem_def.h"
#include "dal_keypad.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#define DEBUG_MSENSOR

#ifdef DEBUG_MSENSOR
#define MSENSOR_PRINT   SCI_TRACE_LOW
#else
#define MSENSOR_PRINT(x)  
#endif

#define MSENSOR_SUPORT_DIAG // support msensor diag simulate
#ifdef  MSENSOR_SUPORT_DIAG
    #include "diag.h"
#endif 
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

#define MSENSOR_STACK_SIZE			2000
#define MSENSOR_QUEUE_NUM			20
#define MSENSOR_TASK_PORI			PRI_MSENSOR_TASK  //  28
#define MSENSOR_MEM_SIZE_MAX		(50*1024)

#define MSENSOR_SIGANL				xSignalHeaderRec
#define MSENSOR_SIGANL_TYPE_MASK    0xFF
#define MSENSOR_SIGANL_TYPE_BIT_LEN 8
typedef enum
{
	MSENSOR_MSG_INIT = 0x0,
	MSENSOR_MSG_OPEN,
	MSENSOR_MSG_CLOSE,
	MSENSOR_MSG_TIMER,
	MSENSOR_MSG_INT,
	MSENSOR_MSG_MAX
	
}MSENSOR_MSG_E;


/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
 LOCAL uint32 				s_msensor_thread_id 	= SCI_INVALID_BLOCK_ID;
 LOCAL SCI_EVENT_GROUP_PTR  s_msensor_event 		= PNULL;
 LOCAL SCI_SEMAPHORE_PTR    s_msensor_sm_module     = NULL;
 LOCAL MSENSOR_CFG_T_PRT    s_msensor_tab[MSENSOR_TYPE_MAX] = {0};
 LOCAL uint32               s_msensor_cnt[MSENSOR_TYPE_MAX] = {0}; // opened counter
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description:    This function is used to create MSENSOR event.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _MSensor_CreateEvent( void )
{
    if(PNULL == s_msensor_event)
    {
        s_msensor_event = SCI_CreateEvent("MOTION_SENSOR_EVENT");

        //MSENSOR_PRINT:"_MSensor_CreateEvent : event =0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_100_112_2_18_0_34_22_1916,(uint8*)"d", s_msensor_event);
    }
}

/*****************************************************************************/
//  Description:    This function is used to get MSENSOR event.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _MSensor_GetEvent(uint32 flag)
{
    uint32 actual_event = 0;
    uint32 status = MSENSOR_ERROR;

    if(PNULL != s_msensor_event)
    {    
        status = SCI_GetEvent(
								s_msensor_event, 
	                            (0x01<<flag), 
	                            SCI_OR_CLEAR, 
	                            &actual_event, 
	                            5000);
    }
    
    return status;
}

/*****************************************************************************/
//  Description:    This function is used to set MSENSOR event.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _MSensor_SetEvent(uint32 flag)
{
    if(PNULL != s_msensor_event)
    {   
        SCI_SetEvent(s_msensor_event, (0x01<<flag), SCI_OR);
    }
}

/*****************************************************************************/
//  Description:    This function is used to send message to task    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _MSensor_SendMSG( MSENSOR_MSG_E msg, uint32 para, 
                                        BOOLEAN is_sync)
{
    MSENSOR_SIGANL *signal_ptr;
	uint32			ret_val = MSENSOR_SUCCESS;

    //MSENSOR_PRINT:"_MSensor_SendMSG : msg = %d, para = %d, is_sync = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_146_112_2_18_0_34_22_1917,(uint8*)"ddd",msg, para, is_sync);
		
    // 20110103: will.jiang_cr221372 : avoid Q full assert
    if(SCI_QUEUE_FULL == SCI_IsThreadQueueAvilable(s_msensor_thread_id))
    {
    	//MSENSOR_PRINT:"MSensor_TimerHandler: msensor thread Q Full !!"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_151_112_2_18_0_34_22_1918,(uint8*)"");
        return MSENSOR_ERROR;
    }
	signal_ptr = SCI_ALLOC_APP(sizeof(MSENSOR_SIGANL));
	if(signal_ptr == NULL)
	{
		return MSENSOR_ERROR;
	}
	
    signal_ptr->SignalCode = (msg << MSENSOR_SIGANL_TYPE_BIT_LEN) | 
                             (para & MSENSOR_SIGANL_TYPE_MASK);
    signal_ptr->SignalSize = sizeof(MSENSOR_SIGANL);
    signal_ptr->Sender     = 0;

    if(SCI_SUCCESS == SCI_SendSignal((xSignalHeader)signal_ptr, 
                                      s_msensor_thread_id))
    {
        if(is_sync)
        {
            ret_val = _MSensor_GetEvent(msg);
        }		
    }
	else
	{
		SCI_FREE(signal_ptr);
		ret_val = MSENSOR_ERROR;
	}

	return ret_val;
}

/*****************************************************************************/
//  Description:    This function is used to init motion sensor    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _MSensor_Init(void)
{
    int32 tab_length = MSensor_GetCfgTabLength();
    MSENSOR_CFG_T_PRT * msensor_cfg_ptr = MSensor_GetCfgTab();   
    int32 i=0;

	SCI_MEMSET(s_msensor_tab, 0, sizeof(s_msensor_tab));
	SCI_MEMSET(s_msensor_cnt, 0, sizeof(s_msensor_cnt));
	// 20100927: willjiang: use for() instead of while()
	for(i=0; i<tab_length; i++)
	{   // 20100927: willjiang_cr203009 : only one device per type
		if( msensor_cfg_ptr[i] && msensor_cfg_ptr[i]->init &&
		    (msensor_cfg_ptr[i]->init() == SCI_SUCCESS) )
		{
			s_msensor_tab[msensor_cfg_ptr[i]->type] = msensor_cfg_ptr[i];
            s_msensor_tab[msensor_cfg_ptr[i]->type]->status = MSENSOR_STATUS_INIT;
		}
		else
		{
			//MSENSOR_PRINT:"DRV_MSENSOR : init: name=%s type = %d init failed!rn"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_205_112_2_18_0_34_22_1919,(uint8*)"sd",msensor_cfg_ptr[i]->name, msensor_cfg_ptr[i]->type);
		}
	}
    return MSENSOR_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is used to handle task message    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _MSensor_HandleRoutine(uint32 argc, void *argv)
{
    MSENSOR_SIGANL 		*signal_ptr = {0};
	MSENSOR_MSG_E   	msg_id;
	uint32          	sensor_type;
	MSENSOR_CFG_T_PRT 	msensor_ptr;
	
    while(SCI_TRUE)/*lint !e716 */
    {
        signal_ptr = SCI_GetSignal( s_msensor_thread_id );
        SCI_ASSERT(PNULL != signal_ptr);/*assert verified*/

		msg_id       = signal_ptr->SignalCode >> MSENSOR_SIGANL_TYPE_BIT_LEN;
		sensor_type  = signal_ptr->SignalCode &  MSENSOR_SIGANL_TYPE_MASK;
		SCI_FREE(signal_ptr);
        //MSENSOR_PRINT:"DRV_MSENSOR : HandleRoutine: msg_id = %d, sensor_type = %drn"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_231_112_2_18_0_34_22_1920,(uint8*)"dd", msg_id, sensor_type);
		msensor_ptr = s_msensor_tab[sensor_type];

        switch(msg_id)
        {
            case MSENSOR_MSG_INIT:
				_MSensor_Init();
				break;
	
            case MSENSOR_MSG_OPEN:
				if(msensor_ptr && msensor_ptr->open)
				{
					msensor_ptr->open();
					msensor_ptr->status = MSENSOR_STATUS_OPEN;
				}
				else
				{
					//MSENSOR_PRINT:"DRV_MSENSOR : HandleRoutine: invalid open()"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_248_112_2_18_0_34_22_1921,(uint8*)"");
				}
				_MSensor_SetEvent(msg_id);
                break;

            case MSENSOR_MSG_CLOSE:
                if(msensor_ptr && msensor_ptr->close)
                {
                    msensor_ptr->close();
                    msensor_ptr->status = MSENSOR_STATUS_CLOSE;
                }
                _MSensor_SetEvent(msg_id);	
                // send a signal to turn on light after close motion sensor, to avoid close action is executed when backlight is closed
                if ((MSENSOR_TYPE_OPTICAL == sensor_type) && msensor_ptr && (MSENSOR_STATUS_CLOSE == msensor_ptr->status))
                {
                    SCI_SendEventToClient(MSENSOR_SERVICE, sensor_type, (void*)MSENSOR_OPTICAL_NO_DETECTED);
                }
                break;

            case MSENSOR_MSG_TIMER:	//NO break!
			case MSENSOR_MSG_INT:	/*lint !e825*/	
				if(msensor_ptr && msensor_ptr->ioctl &&
				   (MSENSOR_STATUS_OPEN == msensor_ptr->status) )
				{
					uint32 event = 0;
					msensor_ptr->ioctl(MSENSOR_IOCTL_GET_EVENT, (void *)&event);
                    //MSENSOR_PRINT:"DRV_MSENSOR : HandleRoutine: event = 0x%08X\r\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_274_112_2_18_0_34_22_1922,(uint8*)"d", event);
					if(event & MSG_MISC_SENSOR_MASK)
					{
					    if((msensor_ptr->type == MSENSOR_TYPE_ACCELEROMETER)
#ifdef MOTION_SENSOR_TYPE_OPTICAL
					    ||(msensor_ptr->type == MSENSOR_TYPE_OPTICAL)
#endif
					    )
					    {
						    SCI_SendEventToClient(MSENSOR_SERVICE, sensor_type, 
						        (void*)event);
						}
						else if(msensor_ptr->type == MSENSOR_TYPE_OFN)
						{
						    SCI_SendEventToClient(KEYPAD_SERVICE, KPD_DOWN, (void *)event);
						    SCI_SendEventToClient(KEYPAD_SERVICE, KPD_UP, (void *)event);
						}
					}
				}				
                break;

            default:
                break;
        }

    }

}

/*****************************************************************************/
//  Description:    This function is used to create task    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _MSensor_CreateTask(uint32 priority)
{
    //MSENSOR_PRINT:"MSensor:  _MSensor_CreateTask priority=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_304_112_2_18_0_34_22_1923,(uint8*)"d", priority);
    
    if(SCI_INVALID_BLOCK_ID == s_msensor_thread_id)
    {
        s_msensor_thread_id = SCI_CreateAppThread(
                                                    "T_MSENSOR_TASK",
                                                    "T_MSENSOR_QUEUE",
                                                    _MSensor_HandleRoutine,
                                                    0,
                                                    0,
                                                    MSENSOR_STACK_SIZE,
                                                    MSENSOR_QUEUE_NUM,
                                                    priority,
                                                    SCI_PREEMPT,
                                                    SCI_AUTO_START);
        //MSENSOR_PRINT:"_MSensor_CreateTask : task id 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_319_112_2_18_0_34_22_1924,(uint8*)"d", s_msensor_thread_id);
    }
}
/*****************************************************************************/
//  Description:    ChangeTaskPriority() dynamically 
//  Note:           
/*****************************************************************************/
LOCAL void _MSensor_ChangeTaskPriority(uint32 priority)
{   uint32 old_priority = 0;
    //MSENSOR_PRINT:"MSensor:  _MSensor_ChangeTask new Priority = %dn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_327_112_2_18_0_34_22_1925,(uint8*)"d", priority);

    if(SCI_INVALID_BLOCK_ID == s_msensor_thread_id)
    {
        SCI_ChangeThreadPriority(s_msensor_thread_id, 
                                 priority, 
                                 &old_priority);
    
        //MSENSOR_PRINT:"MSensor:  _MSensor_ChangeTask old Priority = %dn"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_336_112_2_18_0_34_22_1926,(uint8*)"d", old_priority);
    }
    else
        //MSENSOR_PRINT:"MSensor:  _MSensor_ChangeTaskpPriority SCI_INVALID_BLOCK_ID"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_339_112_2_18_0_34_22_1927,(uint8*)"");
      
}

/******************************************************************************/
// Description:   The Default function of MSENSOR SERVER.
// Dependence: 
// Author:        Haifeng.Yang
// Note:          This function just prints some debug information.
/******************************************************************************/
LOCAL void _MSensor_DefaultServiceCB (uint32 id, uint32 argc, void *argv)
{
	//MSENSOR_PRINT:"Msensor: _MSensor_DefaultServiceCB: id = %d, argc = %d argv = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_350_112_2_18_0_34_22_1928,(uint8*)"ddd",id, argc, (uint32)argv);
}

/******************************************************************************/
// Description: support msensor diag simulate 
// Note:
/******************************************************************************/
#ifdef  MSENSOR_SUPORT_DIAG
LOCAL uint32 _MSensor_diag_handler (
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    typedef struct
    {

        uint32  type;
        uint32  event;    
    }MSENSOR_MSG_T;
    
    MSENSOR_MSG_T msensor_msg = {0}, * msensor_msg_ptr = PNULL;

    msensor_msg_ptr   = (MSENSOR_MSG_T *) (src_ptr + sizeof (MSG_HEAD_T));
    msensor_msg.type  = msensor_msg_ptr->type; 
    msensor_msg.event = msensor_msg_ptr->event;
    //MSENSOR_PRINT:"Msensor: DIAG, type = %d, event = 0x%08X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_375_112_2_18_0_34_22_1929,(uint8*)"dd",msensor_msg.type, msensor_msg.event);
    // Build response to channel server
    *dest_ptr = SCI_ALLOC_APP (sizeof (MSG_HEAD_T));
    *dest_len_ptr = sizeof (MSG_HEAD_T);
    memcpy (*dest_ptr, src_ptr, sizeof (MSG_HEAD_T));
    ( (MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
    
	SCI_SendEventToClient(MSENSOR_SERVICE, msensor_msg.type, (void *) msensor_msg.event);
	
    return SCI_TRUE;
}
#endif

/******************************************************************************/
// Description:   Create a semaphore to proect OPEN and CLOSE operations
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
PUBLIC uint32 MSensor_InitModule(void)
{
	static BOOLEAN  s_init_flag = SCI_FALSE;

	if(s_init_flag)
	{
		return MSENSOR_SUCCESS;
	}
	s_init_flag = SCI_TRUE;
	
	s_msensor_sm_module = SCI_CreateSemaphore ("MSENSOR SEMAPHORE MODULE", 1);
	SCI_ASSERT(s_msensor_sm_module != NULL);/*assert verified*/

	
    SCI_CreateClientList(MSENSOR_SERVICE, 
                    (uint8)(MSENSOR_TYPE_MAX & 0xff), 
                    (REG_CALLBACK) _MSensor_DefaultServiceCB);

    _MSensor_CreateTask(MSENSOR_TASK_PORI);
	SCI_ASSERT(s_msensor_thread_id != SCI_INVALID_BLOCK_ID);/*assert verified*/
    _MSensor_CreateEvent();
    #ifdef MSENSOR_SUPORT_DIAG
        DIAG_RegisterCmdRoutine(DIAG_MSENSOR_F, _MSensor_diag_handler);
    #endif
	_MSensor_SendMSG(MSENSOR_MSG_INIT, 0, SCI_FALSE);
	return MSENSOR_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to open MSENSOR .    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 MSensor_Open(MSENSOR_TYPE_E sensor_type)
{
	uint32 ret_val = MSENSOR_SUCCESS;

	if(sensor_type >= MSENSOR_TYPE_MAX)
		return MSENSOR_ERROR_PARA;
	if(PNULL == s_msensor_tab[sensor_type])	
	{
		//MSENSOR_PRINT:"MSensor_Open : failed, sensor_type %d not support"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_431_112_2_18_0_34_23_1930,(uint8*)"d",sensor_type);
	    return MSENSOR_ERROR_NOT_SUPPORT;
	}	
    // protect
    SCI_GetSemaphore(s_msensor_sm_module, SCI_WAIT_FOREVER);
    // if sensor has been already opened, count up
 	if(s_msensor_cnt[sensor_type])
	{
		s_msensor_cnt[sensor_type]++;
	}  
	else if(MSENSOR_STATUS_NOT_INIT == s_msensor_tab[sensor_type]->status)
	{
		ret_val = MSENSOR_ERROR_NOT_INITED;
	}
    // check state, if sensor has been already opened, return directly
	else if(MSENSOR_STATUS_OPEN != s_msensor_tab[sensor_type]->status)
	{  
	   ret_val = _MSensor_SendMSG(MSENSOR_MSG_OPEN, (uint32)sensor_type, SCI_TRUE);
       if(MSENSOR_SUCCESS == ret_val)
   	   {
		   s_msensor_cnt[sensor_type]++;
   	   }
        
	}
    SCI_PutSemaphore(s_msensor_sm_module);

	//MSENSOR_PRINT:"MSensor_Open : s_msensor_tab[%d]->name= %s, status = %d, "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_461_112_2_18_0_34_23_1931,(uint8*)"dsddd", sensor_type, s_msensor_tab[sensor_type]->name, s_msensor_tab[sensor_type]->status,s_msensor_cnt[sensor_type], ret_val);
	
    return ret_val;

}

/*****************************************************************************/
//  Description:    This function is used to close MSENSOR.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 MSensor_Close(MSENSOR_TYPE_E sensor_type)
{   
    uint32 ret_val = MSENSOR_SUCCESS;
    
	if(sensor_type >= MSENSOR_TYPE_MAX)
		return MSENSOR_ERROR_PARA;
	if(PNULL == s_msensor_tab[sensor_type])	
	{
		//MSENSOR_PRINT:"MSensor_Close : failed, sensor_type %d not support"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_478_112_2_18_0_34_23_1932,(uint8*)"d",sensor_type);
	    return MSENSOR_ERROR_NOT_SUPPORT;
	}	
    // protect
    SCI_GetSemaphore(s_msensor_sm_module, SCI_WAIT_FOREVER);
	if(MSENSOR_STATUS_NOT_INIT == s_msensor_tab[sensor_type]->status)
	{
		ret_val = MSENSOR_ERROR_NOT_INITED;
	}
	else if(s_msensor_cnt[sensor_type] >= 1)
	{
	    s_msensor_cnt[sensor_type]--;
		if(s_msensor_cnt[sensor_type] == 0)
		{		
	    	ret_val = _MSensor_SendMSG(MSENSOR_MSG_CLOSE, (uint32)sensor_type, SCI_TRUE);
	    }
	}
    SCI_PutSemaphore(s_msensor_sm_module);
	//MSENSOR_PRINT:"MSensor_Close : s_msensor_tab[%d]->name= %s, status = %d, "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_500_112_2_18_0_34_23_1933,(uint8*)"dsddd", sensor_type, s_msensor_tab[sensor_type]->name, s_msensor_tab[sensor_type]->status, s_msensor_cnt[sensor_type], ret_val);
    return ret_val;
}

/*****************************************************************************/
//  Description:    This function is used to handler interrupt    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC uint32 MSensor_IntHandler(uint32 id)
{
	
    if(s_msensor_thread_id 	== SCI_INVALID_BLOCK_ID)
    {
    	//MSENSOR_PRINT:"MSensor_IntHandler: The thread is NOT running!"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_512_112_2_18_0_34_23_1934,(uint8*)"");
    	return MSENSOR_ERROR_NOT_INITED;
    }
    
    
    // 20101012: Will.Jiang_cr203009 : one int handler support multi-int-source
	if(GPIO_PROD_GSENSOR_INT_ID == id)
	{  // GPIO_PROD_GSENSOR_INT_ID ==> MSENSOR_TYPE_ACCELEROMETER
       _MSensor_SendMSG(MSENSOR_MSG_INT, MSENSOR_TYPE_ACCELEROMETER, SCI_FALSE);
	}
	#ifdef MOTION_SENSOR_TYPE_OFN
    else if(GPIO_PROD_OFN_INT_ID == id)
	{  
       _MSensor_SendMSG(MSENSOR_MSG_INT, MSENSOR_TYPE_OFN, SCI_FALSE);
	}
	#endif
	else
	    return MSENSOR_ERROR_PARA;
	    
    return MSENSOR_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to SENSOR timer    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC uint32 MSensor_TimerHandler(MSENSOR_TYPE_E sensor_type)
{
    if(s_msensor_thread_id 	== SCI_INVALID_BLOCK_ID)
    {
    	//MSENSOR_PRINT:"MSensor_TimerHandler: The thread is NOT running!"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_541_112_2_18_0_34_23_1935,(uint8*)"");
    	return MSENSOR_ERROR_NOT_INITED;
    }
    
	_MSensor_SendMSG(MSENSOR_MSG_TIMER, (uint32)sensor_type, SCI_FALSE);

    return MSENSOR_SUCCESS;
}
/*****************************************************************************/
//  Description: msensor ioctl
//  Note:        data_ptr should be ready before call it       
/*****************************************************************************/
PUBLIC uint32 MSensor_ioctl(
    MSENSOR_TYPE_E sensor_type,
    MSENSOR_IOCTL_E cmd,
	void * data_ptr)
{
	MSENSOR_CFG_T_PRT    msensor_ptr = s_msensor_tab[sensor_type];
	uint32 ret = MSENSOR_ERROR;
	
	if(data_ptr == NULL || sensor_type >= MSENSOR_TYPE_MAX)
	{
		return MSENSOR_ERROR_PARA;
	}
	if(PNULL == s_msensor_tab[sensor_type])	
	{
		//MSENSOR_PRINT:"MSensor_ioctl : failed, sensor_type %d not support"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DRV_565_112_2_18_0_34_23_1936,(uint8*)"d",sensor_type);
	    return MSENSOR_ERROR_NOT_SUPPORT;
	}
	// only one task avilaible, whatever sensor type is
    if(MSENSOR_IOCTL_SET_PRIO == cmd)
    {
        _MSensor_ChangeTaskPriority(*(uint32 *)data_ptr);
    }
    // MSENSOR_IOCTL_GET_CAP whenever it is OPEN or not
    else if( (MSENSOR_IOCTL_GET_CAP == cmd) &&
              msensor_ptr && msensor_ptr->ioctl)
	{
	    ret = msensor_ptr->ioctl(cmd, data_ptr);
    }
    // related to specific sensor
    else if(msensor_ptr && msensor_ptr->ioctl &&
		   (MSENSOR_STATUS_OPEN == msensor_ptr->status) )
	{
	
        // protect
        SCI_GetSemaphore(s_msensor_sm_module, SCI_WAIT_FOREVER);
	    ret = msensor_ptr->ioctl(cmd, data_ptr);
        SCI_PutSemaphore(s_msensor_sm_module);
	}

	return ret;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of msensor_drv.c


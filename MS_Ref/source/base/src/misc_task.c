/******************************************************************************
 ** File Name:      misc_task.c                                               *
 ** Author:         Eric.zhou                                                 *
 ** DATE:           06/01/2003                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/15/2003    Zhemin.Lin    Modify SCI_SetArmClk, SCI_GetArmClk *
 ** 29/08/2005     weihua.wang      modify for  CR29225                       *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_ref_base_trc.h"
#include "os_api.h"
#include "tb_dal.h"
#include "tasks_id.h"
#include "misc_ext.h"
#include "sys_time_ext.h"
#include "arm_reg.h"
#include "sc_reg.h"
#include "ref_param.h"
#include "tb_hal.h"
#include "sci_service.h"
#include "chip.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define MISC_DEBUG

#ifdef MISC_DEBUG
	#define MISC_PRINT		SCI_TRACE_LOW
#else
	#define MISC_PRINT	
#endif	// MISC_DEBUG

/**---------------------------------------------------------------------------*
 **                         Local constant                                    *
 **---------------------------------------------------------------------------*/
 #ifdef SIMULATE_MIN_INT
LOCAL SCI_TIMER_PTR s_simulate_minute_int_timer;

LOCAL BOOLEAN   s_is_real_minute_int;
#endif
LOCAL uint32    s_last_minute_int_signal_timestamp;

LOCAL RTC_CALIBRATION_INFO_T	s_rtc_cal_info;

/*****************************************************************************/
//  Description:    This function is init misc_task. 
//  Author:         Eric.zhou
//	Note:           
/*****************************************************************************/
LOCAL void InitTask(void);

/*****************************************************************************/
//  Description:    This function is used to send signal to client. It is a
//                  callback function.
//  Author:         Eric.zhou
//	Note:           
/*****************************************************************************/
LOCAL void MISC_SendToClient(BLOCK_ID id, uint32 argc, void *argv);

/*****************************************************************************/
// 	Description :   Handle simulate minute interrupt timer.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
/*****************************************************************************/
#ifdef SIMULATE_MIN_INT
LOCAL void SimulateMinIntTimerFunc(uint32 param);
#endif
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Main thread of diagnostic task. It manages the command 
//                  messages from client application tools and send response
//                  back to channel manager. 
//	Global resource dependence: 
//  Author:         Eric.zhou
//	Note:			It recever messages from MISC.
/*****************************************************************************/
THREAD_ENTRY(TIMESVR)/*lint -esym(765,thread_entry_TIMESVR)*/
{
    uint32      cur_time;
    TB_RTC_TIME_T   rtc_time;
	MISC_SIG_T	*sig_ptr = SCI_NULL;
	
	SCI_CreateClientList(TIME_SERVICE, MISC_MSG_MAX_NUM, (REG_CALLBACK)MISC_SendToClient);
    
    InitTask();

	//The more conventional form of
    //  infinite loop prefix is for(;;)
	//while(1)
	for(;;)
	{
        sig_ptr = (MISC_SIG_T *)SCI_GetSignal(TIMESVR);
        //MISC_PRINT:"thread_misc_task_entry: Received signal: 0x%04x!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MISC_TASK_101_112_2_18_1_5_25_46,(uint8*)"d", sig_ptr->SignalCode);
        
        switch (sig_ptr->SignalCode)
		{
		    case MISC_MSG_RTC_MIN:
		    	#ifdef SIMULATE_MIN_INT
		        if (s_is_real_minute_int)
		        {
		            SCI_AdjustMiniuteIntTimer(60);
		        }
		        else
		        {
		            s_is_real_minute_int = SCI_TRUE;
		        }
		        #endif
		        
		        cur_time = SCI_GetTickCount();
		        if ((cur_time - s_last_minute_int_signal_timestamp) > 55000 || 0 == s_last_minute_int_signal_timestamp )
		        {  
#ifdef _REF_SC8800D_
	                TB_MSG          rtc_msg;
#endif
	                
                    // Get RTC time to check if need to update date.
	                RTC_Get(&rtc_time);
                    #ifdef SIMULATE_MIN_INT
	                if (s_is_real_minute_int)
	                {
	                	if (30 == rtc_time.min)
	                	{
	                		SCI_UpdateRtcCalibration(0);
	                	}
	                }
	               #else
	                if (30 == rtc_time.min)
	                {
	                	SCI_UpdateRtcCalibration(0);
	                }	               
	               #endif
	                s_last_minute_int_signal_timestamp = cur_time;
		            SCI_SendEventToClient(TIME_SERVICE, MISC_MSG_RTC_MIN, 0);
#ifdef _REF_SC8800D_		    
		            if (rtc_time.day > 0)
                	{
                        rtc_msg.message = TB_RTC_INT_DAY;
                        SCI_RtcCallback(&rtc_msg);
                    }
#endif   
            
		        }		        
		        break;

		    case MISC_MSG_RTC_HOUR:
		        SCI_SendEventToClient(TIME_SERVICE, MISC_MSG_RTC_HOUR, 0);
		        break;
		        
		    case MISC_MSG_RTC_DAY:
#ifdef _REF_SC8800D_		    
	            RTC_Get(&rtc_time);
                if (rtc_time.day > 0)
                {
    	            TM_UpdateDate();
    	            SCI_SendEventToClient(TIME_SERVICE, MISC_MSG_RTC_DAY, 0);
                }
#endif           
		        break;

		    case MISC_MSG_RTC_ALARM:
		        SCI_SendEventToClient(TIME_SERVICE, MISC_MSG_RTC_ALARM, 0);
		        break;

		    default:
   		        break;
		}
	    SCI_FREE(sig_ptr);
    }
}

/*****************************************************************************/
//  Description:    This function is init misc_task. 
//  Author:         Eric.zhou
//	Note:           
/*****************************************************************************/
LOCAL void InitTask(void)
{

#ifndef WIN32
    // Register RTC callback function to TBIOS
    
    HAL_RegCallback(TB_RTC_INT, SCI_RtcCallback);
	
    TM_Init();
    
    REFPARAM_GetRtcCalibrationInfo(&s_rtc_cal_info);
	
	s_last_minute_int_signal_timestamp = 0;
	
#ifdef SIMULATE_MIN_INT
	s_is_real_minute_int = SCI_TRUE;
    // Create a timer to simulate minute interrupt.
    s_simulate_minute_int_timer = SCI_CreatePeriodTimer("Min_INT", SimulateMinIntTimerFunc, 0, 60000, 1);
    SCI_ASSERT(s_simulate_minute_int_timer != PNULL);/*assert verified*/
#endif // SIMULATE_MIN_INT
    
#else //WIN32

#define X_RTC_SIMULATE	
#ifdef X_RTC_SIMULATE
	HAL_RegCallback(TB_RTC_INT, SCI_RtcCallback);	
	RTC_Init();
#endif // X_RTC_SIMULATE
	
	TM_Init();
    
    REFPARAM_GetRtcCalibrationInfo(&s_rtc_cal_info);
	
    s_last_minute_int_signal_timestamp = 0;  
#ifndef	X_RTC_SIMULATE
#ifdef SIMULATE_MIN_INT
	s_is_real_minute_int = SCI_TRUE;  	
	s_simulate_minute_int_timer = SCI_CreatePeriodTimer("Min_INT", SimulateMinIntTimerFunc, 0, 60000, 1);
	SCI_ASSERT(s_simulate_minute_int_timer != PNULL);/*assert verified*/
#endif
#endif //X_RTC_SIMULATE
    
#endif

}

/*****************************************************************************/
//  Description:    This function is used to send signal to client. It is a
//                  callback function.
//  Author:         Eric.zhou
//	Note:           
/*****************************************************************************/
LOCAL void MISC_SendToClient(BLOCK_ID id, uint32 argc, void *argv)
{
    MISC_SIG_T    *rtc_sig_ptr    = SCI_NULL;
    
    rtc_sig_ptr = (MISC_SIG_T *)SCI_ALLOC_APP(sizeof(MISC_SIG_T));
    SCI_ASSERT(SCI_NULL != rtc_sig_ptr);/*assert verified*/
    /*lint -esym(613, rtc_sig_ptr) */
    rtc_sig_ptr->SignalSize  = sizeof(MISC_SIG_T);
    rtc_sig_ptr->Sender      = TIMESVR;
    rtc_sig_ptr->SignalCode  = (uint16)(argc & 0xFFFF);
    
    // Send signal to client of client.
    SCI_SendSignal((xSignalHeader)rtc_sig_ptr, id);
}

/*****************************************************************************/
//  Description:    This function is used to register to TBIOS. It will be
//                  called to handle the message from lower layer.
//  Author:         Eric.zhou
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_RtcCallback(
    TB_MSG * msg_ptr    // Message to be handled.
    )
{  
    MISC_SIG_T  *rtc_sig_ptr;
    
    rtc_sig_ptr = (MISC_SIG_T *)SCI_ALLOC_APP(sizeof(MISC_SIG_T));    
    SCI_ASSERT(SCI_NULL != rtc_sig_ptr);/*assert verified*/
    
    rtc_sig_ptr->SignalSize = sizeof(MISC_SIG_T);
    rtc_sig_ptr->Sender     = TIMESVR;
    
    // General signal and send it to MISC.
    if (TB_RTC_INT_MIN == msg_ptr->message)
    {   
        rtc_sig_ptr->SignalCode = MISC_MSG_RTC_MIN;
        SCI_SendSignal((xSignalHeader)rtc_sig_ptr, TIMESVR);
    }
    else if (TB_RTC_INT_HOUR == msg_ptr->message)
    {   
        rtc_sig_ptr->SignalCode = MISC_MSG_RTC_HOUR;
        SCI_SendSignal((xSignalHeader)rtc_sig_ptr, TIMESVR);
    }
    else if (TB_RTC_INT_DAY == msg_ptr->message)
    {   
        rtc_sig_ptr->SignalCode = MISC_MSG_RTC_DAY;
        SCI_SendSignal((xSignalHeader)rtc_sig_ptr, TIMESVR);
    }        
    else if (TB_RTC_INT_ALARM == msg_ptr->message)
    {   
        rtc_sig_ptr->SignalCode = MISC_MSG_RTC_ALARM;
        SCI_SendSignal((xSignalHeader)rtc_sig_ptr, TIMESVR);
    }
    else
    {
        SCI_FREE(rtc_sig_ptr);
    }    
}

/*****************************************************************************/
// 	Description :   Handle minute interrupt timer.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
/*****************************************************************************/
#ifdef SIMULATE_MIN_INT
LOCAL void SimulateMinIntTimerFunc(uint32 param)
{
    TB_MSG          rtc_msg;
            
    s_is_real_minute_int = SCI_FALSE;
	rtc_msg.message = TB_RTC_INT_MIN;
    SCI_RtcCallback(&rtc_msg);
}
#endif

/*****************************************************************************/
// 	Description :   Adjust simulate minute interrupt timer.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
/*****************************************************************************/
#ifdef SIMULATE_MIN_INT
PUBLIC void SCI_AdjustMiniuteIntTimer(
    uint32 sec_num
    )
{
    SCI_ASSERT(s_simulate_minute_int_timer != PNULL);/*assert verified*/
    
    if (sec_num > 60)
    {
        sec_num = 60;
    }    
    
    if (SCI_IsTimerActive(s_simulate_minute_int_timer))
    {
        SCI_DeactiveTimer(s_simulate_minute_int_timer);
        SCI_ChangeTimer(s_simulate_minute_int_timer, SimulateMinIntTimerFunc, sec_num * 1000);
        SCI_ActiveTimer(s_simulate_minute_int_timer);
    }
}
#endif

/*****************************************************************************/
// 	Description :   Adjust RTC time.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
/*****************************************************************************/
#define RTC_CAL_STANDARD			0x108798
#define RTC_CAL_DENOMINATOR			1625000		// 3250000/2
#define RTC_UPDATE_RTC_REG_SEC_NUM	30			// [10, 60]
#define RTC_CAL_MAX_INTERVAL_SEC	1728000		// 20 Days

PUBLIC void SCI_RtcCalibration(uint32 cal_result)/*lint -esym(765,SCI_RtcCalibration)*/
{
	int32  new_cal_numerator;
	uint32 cur_time = TM_GetTotalSeconds();
	
	// Get new calibration result
	cal_result = cal_result >> 5;
	
	// If current time is earlier than last time, clear the calibration information.
	if (cur_time <= s_rtc_cal_info.last_cal_time)
	{
		s_rtc_cal_info.last_cal_result = 0;
	}

	if (s_rtc_cal_info.last_cal_result == 0)
	{
		// First time calibration.
		s_rtc_cal_info.total_delta_seconds	= 0;
		s_rtc_cal_info.last_cal_numerator	= 0;
		s_rtc_cal_info.is_update_nv			= 1;
	}
	else
	{
		// Not first time.
		
		int32 tmp_time = (cur_time - s_rtc_cal_info.last_cal_time);
		if (tmp_time > RTC_CAL_MAX_INTERVAL_SEC)
		{
			// To avoid overflowing
			tmp_time = RTC_CAL_MAX_INTERVAL_SEC;
		}		
		new_cal_numerator = s_rtc_cal_info.last_cal_numerator + (int32)(cal_result - RTC_CAL_STANDARD) * 3 * tmp_time;
		
		s_rtc_cal_info.last_cal_numerator 	= new_cal_numerator % RTC_CAL_DENOMINATOR;
		s_rtc_cal_info.is_update_nv			= new_cal_numerator / RTC_CAL_DENOMINATOR;
		s_rtc_cal_info.total_delta_seconds += s_rtc_cal_info.is_update_nv;
	}
	s_rtc_cal_info.last_cal_time	= cur_time;
	s_rtc_cal_info.last_cal_result 	= cal_result;
}

/*****************************************************************************/
// 	Description :   Update RTC calibration result.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
/*****************************************************************************/
PUBLIC void SCI_UpdateRtcCalibration(BOOLEAN is_clear)
{
	BOOLEAN	is_update = SCI_FALSE;
	int32	delta_sec = 0;	
	RTC_CALIBRATION_INFO_T	tmp_info = {0};
	
	//SCI_TRACE_LOW:"SCI_UpdateRtcCalibration: last_cal_time=%d, last_cal_result=%d,total_delta_seconds=%d, last_cal_numerator=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MISC_TASK_392_112_2_18_1_5_26_47,(uint8*)"dddd", s_rtc_cal_info.last_cal_time, s_rtc_cal_info.last_cal_result, s_rtc_cal_info.total_delta_seconds, s_rtc_cal_info.last_cal_numerator);

	SCI_DisableIRQ();
	
	// Update s_rtc_cal_info and flags.
	if (is_clear)
	{
		// Clear RTC calibration infomation.
		s_rtc_cal_info.last_cal_numerator 	= 0;
		s_rtc_cal_info.is_update_nv			= 0;
		s_rtc_cal_info.total_delta_seconds  = 0;
		s_rtc_cal_info.last_cal_time		= 0;
		s_rtc_cal_info.last_cal_result 		= 0;
		
		tmp_info 	= s_rtc_cal_info;
		is_update 	= SCI_TRUE;
	}
	else
	{
		// If delta time is bigger than RTC_UPDATE_RTC_REG_SEC_NUM, then update.
		if ((s_rtc_cal_info.total_delta_seconds >= RTC_UPDATE_RTC_REG_SEC_NUM) || (s_rtc_cal_info.total_delta_seconds <= (-RTC_UPDATE_RTC_REG_SEC_NUM)))
		{			
			delta_sec	= s_rtc_cal_info.total_delta_seconds;
			s_rtc_cal_info.total_delta_seconds 	= 0;
			
			s_rtc_cal_info.last_cal_numerator  += s_rtc_cal_info.reserved1 * (RTC_CAL_DENOMINATOR / 100);
			
			s_rtc_cal_info.is_update_nv = delta_sec;
		}
		
		if (s_rtc_cal_info.is_update_nv != 0)
		{			
			tmp_info 	= s_rtc_cal_info;
			is_update 	= SCI_TRUE;			
		}
		else
		{
			is_update = SCI_FALSE;
		}
			
	}
	
	SCI_RestoreIRQ();

	if (delta_sec != 0)
	{
		// Update RTC registers.
		TB_RTC_TIME_T   rtc_time;
		uint32			tmp_sec;
		RTC_Get(&rtc_time);
			
		tmp_sec = rtc_time.min * 60 + rtc_time.sec + delta_sec;
			
		rtc_time.min = tmp_sec / 60;
		rtc_time.sec = tmp_sec % 60;
		RTC_Set(rtc_time);
	}
		
	if (is_update)
	{
		// Update it to NV.
		REFPARAM_SetRtcCalibrationInfo(tmp_info);
	}
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#define _MV_TIMER_API_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "sfs.h"
#include "mv_debug.h"
#include "sci_api.h"
#include "mv_timer_api.h"
#ifdef WIN32
#include "tx_api_win32.h"
#else
#include "tx_api_arm.h"
#endif
#include "caf.h"
#include "chng_freq.h"
#include "socket_types.h"
#include "mv_net_api.h"
#include "mv_player_api.h"
#include "priority_app.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL _UINT32 s_mv_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
LOCAL uint32 s_mv_task_id =SCI_INVALID_BLOCK_ID;
LOCAL uint32 s_mv_socket_task_id =SCI_INVALID_BLOCK_ID;
LOCAL TX_EVENT_FLAGS_GROUP_PTR    s_mv_event_flags = SCI_NULL;
static timer_param 		s_mv_globle_param = {0}; 
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : mv_task
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void mv_task(uint32 argc, void* argv);

/*****************************************************************************/
//  Description : mv_task
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void mv_timer(uint32 param);

LOCAL _INT s_mv_asyn_sig[4] = {0};
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : mv_task
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void mv_task(uint32 argc, void* argv)
{
    UINT     status;	
    uint32 callback_start_tick;
    uint32 callback_end_tick;
    uint32  actual_flags;

	while(1)/*lint !e716*/
    {
        if ( s_mv_asyn_sig[3] )
        {
           CM_Key_CallBack(s_mv_asyn_sig[0], s_mv_asyn_sig[1], s_mv_asyn_sig[2]);
           s_mv_asyn_sig[3] = 0;
        }
        else
        {
        status = SCI_GetEvent(s_mv_event_flags,0x1,TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);

        if( s_mv_globle_param.mv_callback != NULL )
        {
            callback_start_tick = SCI_GetTickCount();

            s_mv_globle_param.mv_callback( s_mv_globle_param.mv_handle, s_mv_globle_param.mv_pParam);

            callback_end_tick = SCI_GetTickCount();
            //MV_TRACE_LOW:"--WD[mv_timer] call mv_callback,tick=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_TIMER_API_91_112_2_18_2_39_21_186,(uint8*)"d",callback_end_tick - callback_start_tick    );
        }
        }
   
 	}

 }
 	
/*****************************************************************************/
//  Description : mv_task
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void mv_timer(uint32 param)
{  

    if(SCI_NULL != s_mv_event_flags)
    {
        SCI_SetEvent(s_mv_event_flags, 0x1, TX_OR);
    }

 }
 

/*****************************************************************************/
//  Description : mv_init
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_init(uint32 nElapse)
{
    SCI_TIMER_PTR  handle = 0;
    
    if(SCI_NULL == s_mv_globle_param.mv_handle )
    {
        memset(&s_mv_globle_param,0,sizeof(s_mv_globle_param));
        s_mv_globle_param.mv_callback  = NULL;
        s_mv_globle_param.mv_pParam   = NULL;
        handle = SCI_CreateTimer( "mobile_video", mv_timer, (uint32)&s_mv_globle_param, nElapse, SCI_NO_ACTIVATE);
        s_mv_globle_param.mv_handle  = handle;  
    }
    //MV_TRACE_LOW:"[MV]--WD[mv_init] s_mv_chng_freq_req_handler = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_TIMER_API_131_112_2_18_2_39_21_187,(uint8*)"d",s_mv_chng_freq_req_handler);

    if (s_mv_chng_freq_req_handler == CHNG_FREQ_REQ_HANDLER_NULL)
    {
#ifndef WIN32
        s_mv_chng_freq_req_handler =  CHNG_FREQ_GetReqHandler("mv");
        CHNG_FREQ_SetArmClk(s_mv_chng_freq_req_handler,FREQ_INDEX_MP4_PLAY_LVL4);
#endif
    }
  
}

/*****************************************************************************/
//  Description : MV_GetFreqHandler
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MV_GetFreqHandler(void)
{
    return s_mv_chng_freq_req_handler;
}

 /*****************************************************************************/
//  Description : MV_SetFreqHandler
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_SetFreqHandler(uint32 req_handler)
{
    s_mv_chng_freq_req_handler = req_handler;

    //MV_TRACE_LOW:"[MV]--WD[MV_SetFreqHandler] s_mv_chng_freq_req_handler = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_TIMER_API_162_112_2_18_2_39_21_188,(uint8*)"d",s_mv_chng_freq_req_handler);

}
 
 /*****************************************************************************/
//  Description : CM_SetTimer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_HANDLE CM_SetTimer(_INT nElapse,TimerCallbackProc pTimerProc,_VOID* pParam)
{   
    static uint32 last_set_timer_tick = 0;
    uint32 cur_set_timer_tick = SCI_GetTickCount();
    uint32 actual_elapse = nElapse;

    if (MV_IsPlaying())
    {
        actual_elapse = 400;//150;
    }
    else
    {
        actual_elapse = 80;//40;
    }


    //MV_TRACE_LOW:"--WD[CM_SetTimer],delta_tick=%d;nElapse=%d,%d;callback=0x%x,0x%x,task=0x%x,%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_TIMER_API_191_112_2_18_2_39_21_189,(uint8*)"ddddddd",(cur_set_timer_tick - last_set_timer_tick ),nElapse,actual_elapse,(uint32)pTimerProc,(uint32)pParam,SCI_IdentifyThread(),s_mv_task_id);
    last_set_timer_tick = cur_set_timer_tick;

    s_mv_globle_param.mv_callback  = pTimerProc;
    s_mv_globle_param.mv_pParam   = pParam;


    SCI_ChangeTimer(s_mv_globle_param.mv_handle,mv_timer ,actual_elapse);
    SCI_ActiveTimer( s_mv_globle_param.mv_handle);

    return (_HANDLE)s_mv_globle_param.mv_handle;

}


/*****************************************************************************/
//  Description : CM_CancelTimer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_CancelTimer(_HANDLE hTimer)
{
    SCI_TIMER_PTR timer_ptr = PNULL;
    int ret = 0;
    timer_ptr = hTimer;

    //MV_TRACE_LOW:"[MV]--WD[CM_CancelTimer] cancel mv timer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_TIMER_API_216_112_2_18_2_39_21_190,(uint8*)"");
    if( SCI_DeactiveTimer( timer_ptr) == SCI_SUCCESS)
    {
	      ret = _TRUE;
    }
    else
    {
	   	ret = _FALSE;
    }

    SCI_DeleteTimer( (SCI_TIMER_PTR)(s_mv_globle_param.mv_handle) );
    s_mv_globle_param.mv_handle = SCI_NULL;
    s_mv_globle_param.mv_callback=NULL;
    return ret;
}


/*****************************************************************************/
// 	Description : mobile video init
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MV_inittask(void)
{
#ifndef WIN32
    if(SCI_NULL == s_mv_event_flags)
    {
        //tx_event_flags_create(&mv_event_flags, "mv event flags");
        s_mv_event_flags = SCI_CreateEvent("mv event flags");
    }

    if( SCI_INVALID_BLOCK_ID == s_mv_task_id)
    {
        s_mv_task_id = SCI_CreateAppThread( 
                        "mv_task",    		//thread name
                         "mv_q", 	      	// Name of the queue
                         mv_task,     		//the task entry
                         0, 		      	//First parameter for entry function,
                         0, 		      	//Second parameter for entry function,
                         MV_STACK_SIZE,     //Size of the thread stack in bytes
                         10,
                         PRI_MV_TASK, 				// Prority of the thread
    	                 SCI_PREEMPT,       // Indicates if the thread is preemptable.
                         SCI_AUTO_START     // Specifies whether the thread starts 
                         );
    }

    if(SCI_INVALID_BLOCK_ID == s_mv_socket_task_id)
    {
        s_mv_socket_task_id = SCI_CreateAppThread( 
                        "mv_socket_task",    	//thread name
                         "mv_socket_q", 	    // Name of the queue
                         MV_socket_task,     	//the task entry
                         0, 		      		//First parameter for entry function,
                         0, 		      		//Second parameter for entry function,
                         MV_SOCKET_STACK_SIZE,  //Size of the thread stack in bytes
                         10,
                         PRI_MV_SOCKET_TASK, 					// Prority of the thread
    	                 SCI_PREEMPT,      	    // Indicates if the thread is preemptable.
                         SCI_AUTO_START 	    // Specifies whether the thread starts 
                         );
    }
   
#endif
}

/*****************************************************************************/
// 	Description : get mv task id
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32 MV_GetMvTaskID(void)
{
    //MV_TRACE_LOW:"[MV] MV_GetMvTaskID s_mv_task_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_TIMER_API_287_112_2_18_2_39_22_191,(uint8*)"d",s_mv_task_id);
    return s_mv_task_id;
}

/*****************************************************************************/
// 	Description : get mv socket task id
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32 MV_GetMVSocketTaskID(void)
{
    return s_mv_socket_task_id;
}

/*****************************************************************************/
// 	Description : key callback useing asynchronism mode
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
_INT CM_Key_Asyn_CallBack(_INT keyCode, _INT Digital, _INT TypeOfPress)
{
	s_mv_asyn_sig[0] = keyCode;
	s_mv_asyn_sig[1] = Digital;
	s_mv_asyn_sig[2] = TypeOfPress;
	s_mv_asyn_sig[3] = 1;
	
    return 0;
}

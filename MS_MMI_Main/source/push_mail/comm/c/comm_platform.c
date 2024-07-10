/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_platform.c
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/


#include "comm_platform.h"
#include "comm_memory.h"


const comm_uint32 comm_time_interval = 946684800;


/*==========================================================
 * Function 	: comm_GetSysTime
 * Author		: sunguochen      
 * Description : 获取系统时间
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
void comm_GetSysTime(comm_time_t* time)
{
    SCI_DATE_T  date_ptr = {0};
    SCI_TIME_T  time_ptr = {0};

    if(!time)
    {
        return;
    }
    
    TM_GetSysDate(&date_ptr);
    TM_GetSysTime(&time_ptr);

    time->tm_sec = time_ptr.sec;
    time->tm_min = time_ptr.min;
    time->tm_hour = time_ptr.hour;

    time->tm_mday = date_ptr.mday;    
    time->tm_mon = date_ptr.mon;
    time->tm_year = date_ptr.year;    
    
}


/*==========================================================
 * Function 	: comm_GetSysTimeStep
 * Author		: sunguochen      
 * Description : 将时间按yyyymmddhhmmss的格式打印为字符串
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
void comm_GetSysTimeStep(char* timestep)
{

    comm_uint32 len = 0;
    comm_time_t time = {0};

    comm_GetSysTime(&time);

    if(!timestep)
    {
        return;
    }

    memset(timestep, 0, COMM_TIMESTEP_LEN);

    sprintf(timestep, "%4ld", time.tm_year);

    len = strlen(timestep);
    sprintf(timestep+len, "%2ld", time.tm_mon);

    len = strlen(timestep);    
    sprintf(timestep+len, "%2ld", time.tm_mday);

    len = strlen(timestep);    
    sprintf(timestep+len, "%2ld", time.tm_hour);

    len = strlen(timestep);    
    sprintf(timestep+len, "%2ld", time.tm_hour);

    len = strlen(timestep);    
    sprintf(timestep+len, "%2ld", time.tm_sec);

    
}


/*==========================================================
 * Function 	: comm_GetUserAgent
 * Author		: sunguochen      
 * Description : 获取手机ua
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
char* comm_GetUserAgent(void)
{

    return MMIAPICOM_GetUserAgent();

}


//thread api function list

/*==========================================================
 * Function 	: comm_CreateThread
 * Author		: sunguochen      
 * Description : create thread
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/6/2010 
 ==========================================================*/
comm_task_id comm_CreateThread(       // If successful, returns the thread ID
                                                                                                    // of the new created thread, 
                                                                                                    // else returns SCI_INVALID_BLOCK_ID
                                                                const char  *thread_name,           // Name string of the thread
                                                                const char  *queue_name,            // Queue name string of the thread
                                                                void    (*entry)(uint32, void*),    // Entry function of the thread
                                                                uint32  argc,                       // First parameter for entry function,
                                                                void    *argv,                      // Second parameter for entry function,
                                                                uint32  stack_size,                 // Size of the thread stack in bytes
                                                                uint32  queue_num,          // Number of messages which can be enqueued
                                                                uint32  priority,           // Prority of the thread.
                                                                uint32  preempt,            // Indicates if the thread is preemptable.
                                                                uint32  auto_start          // Specifies whether the thread starts 
                                                                                            // immediately or stays in a pure suspended
                                                                                            // state. Legal options are SCI_AUTO_START
                                                                                            // and SCI_DONT_START.
                                                                )
{

    return SCI_CreateThread(
                thread_name,           
                queue_name,
                entry,
                argc,
                argv,
                stack_size,
                queue_num,
                priority,
                preempt,
                auto_start
                );

}


/*==========================================================
 * Function 	: comm_DeleteThread
 * Author		: sunguochen      
 * Description : delete thread
 * Parameter	: type          parameter       Description
 * Return		: If delete successful,return SCI_SUCCESS;
                      else return SCI_ERROR
 * Effect		: NO
 * History	: created  4/6/2010 
 ==========================================================*/
comm_uint32 comm_DeleteThread(comm_task_id task_id)
{

    return SCI_DeleteThread(task_id);

}


/*==========================================================
 * Function 	: comm_GetSignal
 * Author		: sunguochen      
 * Description : The function gets a signal from a thread.
                      BLOCK_ID thread_id    -- ID of the thread which receives signal
 * Parameter	: type          parameter       Description
 * Return		: Return received signal
 * Effect		: NO
 * History	: created  4/6/2010 
 ==========================================================*/
comm_thread_signal comm_GetSignal(comm_task_id task_id)
{

    return  SCI_GetSignal(task_id);

}


/*==========================================================
 * Function 	: comm_SendSignal
 * Author		: sunguochen      
 * Description : The function send signal to proper thread.
 * Parameter	: type          parameter       Description
                      xSignalHeader   signal_ptr---Signal pointer to be sended
                      BLOCK_ID        dest_id   --- Dest thread ID
 * Return		: If send successfully, return SCI_SUCCESS,
                      else return SCI_ERROR
 * Effect		: NO
 * History	: created  4/6/2010 
 ==========================================================*/
comm_uint32 comm_SendSignal(comm_thread_signal signal_ptr,comm_task_id dest_task_id)
{

    return  SCI_SendSignal(signal_ptr,dest_task_id);

}


/*==========================================================
 * Function 	: comm_IdentifyThread
 * Author		: sunguochen      
 * Description : returns the current inused thread ID
 * Parameter	: type          parameter       Description
 * Return		: If the thread is created by user, return thread id; 
                      else return SCI_INVALID_BLOCK_ID.
 * Effect		: NO
 * History	: created  4/6/2010 
 ==========================================================*/
comm_task_id comm_IdentifyThread(void)
{

    return  SCI_IdentifyThread();

}


/*==========================================================
 * Function 	: comm_SendCmdToApp
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/6/2010 
 ==========================================================*/
int comm_SendCmdToApp(comm_uint32 cmd)
{

    comm_uint32 ret = 0;
    comm_uint16                signal_size = 0;  
    comm_uint16                signal_code = 0;
    comm_thread_signal_struct      *signal_ptr = 0;
    
    signal_code = (comm_uint16)cmd;//3 
    signal_size = sizeof(comm_thread_signal_struct);
    
    MMI_CREATE_SIGNAL(signal_ptr, signal_code, signal_size, COMM_P_APP_ID);
    
    // Send the signal to the dedicated task's queue
    ret = MMI_SEND_SIGNAL((comm_thread_signal_struct*)signal_ptr, COMM_P_APP_ID);

    return ret;
	
}




/*==========================================================
 * Function 	: comm_CurrentTime
 * Author		: sunguochen      
 * Description : 获取当前时间-单位为秒
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
comm_uint32 comm_CurrentTime(void)
{
    comm_uint32 seconds = 0;

    seconds = MMIAPICOM_GetCurTime();//TM_GetTotalSeconds() + comm_time_interval - COMM_TIME_ZONE * 3600;

    return seconds;
    
}

//thread api list end


/*==========================================================
 * Function 	: comm_str2wstr
 * Author		: sunguochen      
 * Description : char to wchar
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void comm_str2wstr(uint16 *wstr, char *str)
{
    char str_value = 0;

    str_value = *str++;
    while('\0' != str_value)
    {
        *wstr++ = str_value;
        str_value = *str++;
    }

}


/*==========================================================
 * Function 	: comm_CurrentTick
 * Author		: sunguochen      
 * Description  : 获取开机后的tick
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
comm_uint32 comm_CurrentTick(void)
{
    comm_uint32 seconds = 0;

    seconds = SCI_GetTickCount();

    return seconds;
    
}


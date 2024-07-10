/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_platform.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef _COMM_PLATFORM_H_
#define _COMM_PLATFORM_H_

#include "mmk_type.h"
#include "sci_types.h"
#include "os_api.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "dal_time.h"
#include "comm_feature.h"
#include "comm_debug.h"
#include "sci_api.h"
#include "mmifmm_export.h"

#define COMM_TIMESTEP_LEN    15

//type
typedef BOOLEAN comm_boolean;
typedef uint8       comm_uint8;
typedef uint16     comm_uint16;
typedef uint32     comm_uint32;

typedef int8        comm_int8;
typedef int16      comm_int16;
typedef int32      comm_int32;

typedef unsigned short comm_wchar;

//字符串-同MMI_STRING_T
typedef struct
{
    wchar*   wstr_ptr;
    uint16    wstr_len;
    
}comm_string_t;

//字符串-扩充用来保存大的数据
typedef struct
{
    wchar*   wstr_ptr;
    int      wstr_len;
    
}comm_long_string_t;

#define                COMM_TIME_ZONE                     8
//thread start

#define COMM_THREAD_SUCCESS  SCI_SUCCESS
#define COMM_THREAD_ERR         SCI_ERROR

typedef BLOCK_ID comm_task_id;

typedef xSignalHeaderRec comm_thread_signal_struct;//xSignalHeaderRec
typedef xSignalHeader      comm_thread_signal;//xSignalHeaderRec *

#define COMM_INVALID_BLOCK_ID SCI_INVALID_BLOCK_ID

#define COMM_P_APP_ID P_APP


//thread end

#define COMM_ASSERT(exp)  SCI_ASSERT(exp)

#define COMM_MIN(a,b)        ( ((a) < (b)) ? (a) : (b))


typedef struct _comm_time_t
{
    comm_uint32 tm_sec;     // secondsafter the minute - [0,59] 
    comm_uint32 tm_min;     // minutesafter the hour - [0,59] 
    comm_uint32 tm_hour;    // hours since midnight - [0,23] 
    comm_uint32 tm_mday;    // day of the month - [1,31] 
    comm_uint32 tm_mon;     // months since January - [1,12] 
    comm_uint32 tm_year;    // years 

}comm_time_t;




typedef struct
{
    SIGNAL_VARS
    int32 data;

} MAIL_SIG_T;



#define COMM_SendSignal               SCI_SendSignal
#define COMM_CREATE_SIGNAL      SCI_CREATE_SIGNAL

/*==========================================================
 * Function 	: comm_GetSysTime
 * Author		: sunguochen      
 * Description : 获取系统时间
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
void comm_GetSysTime(comm_time_t* time);


/*==========================================================
 * Function 	: comm_GetSysTimeStep
 * Author		: sunguochen      
 * Description : 将时间按yyyymmddhhmmss的格式打印为字符串
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
void comm_GetSysTimeStep(char* timestep);


/*==========================================================
 * Function 	: comm_GetUserAgent
 * Author		: sunguochen      
 * Description : 获取手机ua
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
char* comm_GetUserAgent(void);


//thread api list

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
                                                                );


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
comm_uint32 comm_DeleteThread(comm_task_id task_id);


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
comm_thread_signal comm_GetSignal(comm_task_id task_id);


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
comm_uint32 comm_SendSignal(comm_thread_signal signal_ptr,comm_task_id dest_task_id);


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
comm_task_id comm_IdentifyThread(void);


/*==========================================================
 * Function 	: comm_SendCmdToApp
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/6/2010 
 ==========================================================*/
int comm_SendCmdToApp(comm_uint32 cmd);





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
void comm_str2wstr(uint16 *wstr, char *str);



/*==========================================================
 * Function 	: comm_CurrentTime
 * Author		: sunguochen      
 * Description : 获取当前时间-单位为秒
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
comm_uint32 comm_CurrentTime(void);



/*==========================================================
 * Function 	: comm_CurrentTick
 * Author		: sunguochen      
 * Description  : 获取开机后的tick
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
comm_uint32 comm_CurrentTick(void);



#endif


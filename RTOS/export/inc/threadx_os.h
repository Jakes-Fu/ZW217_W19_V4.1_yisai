/******************************************************************************
 ** File Name:      threadx_os.h                                              *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           09/08/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 RTOS. Including thread control functions, message control *
 **					fucntions, event, memory, timer etc. 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/08/2002     Xueliang.Wang    Create                                    *  
 ** 09/12/2002     Zhemin.Lin       Modify according to CR:MS00004678         *  
 ******************************************************************************/
#ifndef _THREADX_OS_H
#define _THREADX_OS_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
// Define basic proportypes.
#include "sci_types.h"

// Define RTOS kernel API.
#ifdef WIN32
    #include "tx_api_win32.h"
#else
	#ifndef _FEATURE_ARM
    	#include "tx_api_thumb.h"
	#else
    	#include "tx_api_arm.h"
	#endif
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
//---------------------------------------------
// RTOS control block define.
//---------------------------------------------
typedef TX_BLOCK_POOL           SCI_BLOCK_POOL_T;
typedef TX_BYTE_POOL            SCI_BYTE_POOL_T;
typedef TX_EVENT_FLAGS_GROUP    SCI_EVENT_GROUP_T;
//typedef TX_QUEUE                SCI_QUEUE_T;
typedef TX_TIMER                SCI_TIMER_T;

//@Zhemin.Lin, add, 09/12/2003, CR:MS00004678
typedef TX_MUTEX        SCI_MUTEX_T;
typedef TX_SEMAPHORE    SCI_SEMAPHORE_T;

typedef struct SCI_THREAD_tag
{
    uint32          block_type;     // Thread type:
                                    // SCI_STATIC_THREAD; SCI_DYNAMIC_THREAD
	uint32			thread_id;		// Static thread ID.
    TX_THREAD       thread;         // Thread's thread control block
    TX_QUEUE        queue;          // Thread's queue control block
    SCI_EVENT_GROUP_T   event;      // Event notification for system server,
                                    // we need to set event back in application 
                                    // calls to realize sync, so that the function
                                    // call can get the event and return.
    void            *save_queue;    // Thread's SaveQ, used to save signals 
                                    // which can not de disposed right now
    uint32          last_error;     // save last error code of this thread.
} SCI_THREAD_T;

#define SCI_THREAD_GAP  8		// 2 * sizeof(uint32)
#define SCI_QUEUE_GAP   (SCI_THREAD_GAP + sizeof(TX_THREAD))
#define SCI_EVENT_GAP   (SCI_QUEUE_GAP + sizeof(TX_QUEUE))

typedef TX_BLOCK_POOL           RAL_BLOCK_POOL_T;
typedef TX_BYTE_POOL            RAL_BYTE_POOL_T;
typedef TX_EVENT_FLAGS_GROUP    RAL_EVENT_GROUP_T;
typedef TX_QUEUE                RAL_QUEUE_T;
typedef TX_TIMER                RAL_TIMER_T;
typedef TX_MUTEX                RAL_MUTEX_T;
typedef TX_SEMAPHORE            RAL_SEMAPHORE_T;

typedef struct RAL_THREAD_tag
{
    uint32          block_type;     // Thread type:
                                    // SCI_STATIC_THREAD; SCI_DYNAMIC_THREAD
    uint32			thread_id;		// Static thread ID.
    TX_THREAD       thread;         // Thread's thread control block
    TX_QUEUE        queue;          // Thread's queue control block
    RAL_EVENT_GROUP_T   event;      // Event notification for system server,
                                    // we need to set event back in application 
                                    // calls to realize sync, so that the function
                                    // call can get the event and return.
    void            *save_queue;    // Thread's SaveQ, used to save signals 
                                    // which can not de disposed right now
    uint32          last_error;     // save last error code of this thread.
} RAL_THREAD_T;
//---------------------------------------------
// Convert macroes.
//---------------------------------------------
// Convert return value.
#define  MAX_ERROR_STRING_LEN    120
extern TX_THREAD * _tx_thread_current_ptr;
#define SAVE_RETURN_VAL_AND_ASSERT_ERROR(_status) \
        if (TX_SUCCESS == (_status)) \
        {   (_status) = SCI_SUCCESS; } \
        else \
        { \
            char    err_str[MAX_ERROR_STRING_LEN + 1]; \
            int32  str_len = 0; \
            int32  info_len = 0;\
            RETURN_VALUE_T  *info_ptr = SCI_GetInfoByKernelErr(_status); \
            (_status) = info_ptr->api_value; \
            if ((*(uint32 *)((uint32)_tx_thread_current_ptr - SCI_THREAD_GAP) == SCI_STATIC_THREAD) \
                || (*(uint32 *)((uint32)_tx_thread_current_ptr - SCI_THREAD_GAP) == SCI_DYNAMIC_THREAD)) \
        	{   ((SCI_THREAD_T *)((uint32)_tx_thread_current_ptr - SCI_THREAD_GAP))->last_error = (_status); } \
        	str_len = sprintf(err_str, "ASSERT: Error 0x%lx,", (_status)); \
        	info_len = strlen(info_ptr->value_str); \
            info_len = (info_len <= (MAX_ERROR_STRING_LEN - str_len)) ? info_len : (MAX_ERROR_STRING_LEN - str_len); \
			SCI_MEMCPY(&err_str[str_len], info_ptr->value_str, info_len); \
			str_len += info_len; \
        	err_str[str_len] = 0; \
            SCI_Assert(err_str, __FILE__, __LINE__);  /*assert verified*/ \
        }   


#define SAVE_RETURN_VAL_AND_ASSERT_ERROR_EX(_status, _file_name, _file_line,param) \
        if (TX_SUCCESS == (_status)) \
        {   (_status) = SCI_SUCCESS; } \
        else \
        { \
            char    err_str[MAX_ERROR_STRING_LEN + 1]; \
            int32  str_len = 0;  \
            int32  name_len = 0; \
            int32  info_len = 0; \
            RETURN_VALUE_T  *info_ptr = SCI_GetInfoByKernelErr(_status); \
            if(!info_ptr)  \
            { \
                    str_len = sprintf(err_str, "ASSERT: Error 0x%lx,", (_status));\
                    name_len = strlen(_file_name); \
			        name_len = (name_len <= (MAX_ERROR_STRING_LEN - str_len - 30)) ? name_len : (MAX_ERROR_STRING_LEN - str_len - 30); \
					SCI_MEMCPY(&err_str[str_len], _file_name, name_len);  \
					str_len += name_len; \
					str_len += sprintf(&err_str[str_len], ", line=%lu, param=0x%lX", _file_line, param);\
		        	err_str[str_len] = 0; \
					SCI_Assert(err_str, __FILE__, __LINE__ ); /*assert verified*/ \
            } \
            else \
            {  \
                (_status) = info_ptr->api_value; \
                if ((*(uint32 *)((uint32)_tx_thread_current_ptr - SCI_THREAD_GAP) == SCI_STATIC_THREAD) \
                    || (*(uint32 *)((uint32)_tx_thread_current_ptr - SCI_THREAD_GAP) == SCI_DYNAMIC_THREAD)) \
            	{   ((SCI_THREAD_T *)((uint32)_tx_thread_current_ptr - SCI_THREAD_GAP))->last_error = (_status); } \
					 str_len = sprintf(err_str, "ASSERT: Error 0x%lx,", (_status));\
					 info_len = strlen(info_ptr->value_str); \
				     info_len = (info_len <= (MAX_ERROR_STRING_LEN >> 1)) ? info_len : (MAX_ERROR_STRING_LEN >> 1);\
				     SCI_MEMCPY(&err_str[str_len], info_ptr->value_str, info_len); \
				     str_len += info_len; err_str[str_len] = ','; str_len++; \
				     name_len = strlen(_file_name); \
			         name_len = (name_len <= (MAX_ERROR_STRING_LEN - str_len - 30)) ? name_len : (MAX_ERROR_STRING_LEN - str_len - 30); \
					 SCI_MEMCPY(&err_str[str_len], _file_name, name_len);  \
					 str_len += name_len; \
					 str_len += sprintf(&err_str[str_len], ", line=%lu, param=0x%lX", _file_line, param);\
		        	 err_str[str_len] = 0; \
 				     SCI_Assert(err_str, __FILE__, __LINE__ );  /*assert verified*/ \
            } \
       }   

#define SAVE_RETURN_VAL(_status) \
        if (TX_SUCCESS == (_status)) \
        {   (_status) = SCI_SUCCESS; } \
        else \
        { \
            RETURN_VALUE_T  *info_ptr = SCI_GetInfoByKernelErr(_status); \
            (_status) = info_ptr->api_value; \
            if ((*(uint32 *)((uint32)_tx_thread_current_ptr - SCI_THREAD_GAP) == SCI_STATIC_THREAD) \
                || (*(uint32 *)((uint32)_tx_thread_current_ptr - SCI_THREAD_GAP) == SCI_DYNAMIC_THREAD)) \
        	{   ((SCI_THREAD_T *)((uint32)_tx_thread_current_ptr - SCI_THREAD_GAP))->last_error = (_status); } \
        }   
        
// Option used to set/get event
extern const uint32 event_option_table[];
#define CONVERT_EVENT_OPTION(_OPTION)   event_option_table[_OPTION]

// Auto start option on thread
extern const uint32 thread_option_table[];
#define CONVERT_THREAD_OPTION(_OPTION)  thread_option_table[_OPTION]

// Indicates if the thread is preemptable.
//extern const uint32 thread_preempt_option_table[];
#define CONVERT_THREAD_PREEMPT_OPTION(_OPTION)  \
        ((_OPTION) == SCI_NO_PREEMPT) ? PRI_RTOS_KERNEL : priority;
        
// Auto start option on timer
extern const uint32 timer_option_table[];
#define CONVERT_TIMER_OPTION(_OPTION)   timer_option_table[_OPTION]

// Macro for queue item size.
extern const uint32 queue_option_table[];
#define CONVERT_QUEUE_OPTION(_OPTION)   queue_option_table[_OPTION]

#if defined(PLATFORM_UWS6121E)
// Wait option when send message or receive message.
#define CONVERT_WAIT_OPTION(_TIME) \
        (_TIME) == SCI_NO_WAIT ? TX_NO_WAIT : \
        (_TIME) == SCI_WAIT_FOREVER ? TX_WAIT_FOREVER : (uint32)((uint64)_TIME * 16384 / 1000)
#else
// Wait option when send message or receive message.
#define CONVERT_WAIT_OPTION(_TIME) \
        (_TIME) == SCI_NO_WAIT ? TX_NO_WAIT : \
        (_TIME) == SCI_WAIT_FOREVER ? TX_WAIT_FOREVER : (_TIME)
#endif

//@Zhemin.Lin, add, 09/12/2003, CR:MS00004678
#define CONVERT_INHERITY_OPTION(_OPTION) \
	    ((_OPTION) == SCI_INHERIT)?TX_INHERIT : TX_NO_INHERIT

// Time convert macro.

//UEIT: optimize interrupt function in tx_ill.c , so keep the tick as the same as ARM for GSM timer.
#ifdef WIN32
#ifdef UEIT_MOCOR
    #define SCI_MILLISEONCDTOTICK(_MILLISEC)    (_MILLISEC)
    #define SCI_TICKTOMILLISECOND(_TICK)        (_TICK)
#else
    #define MILLISECONDS_PER_TICK               20
    #define SCI_MILLISEONCDTOTICK(_MILLISEC)    ((_MILLISEC + 19) / (MILLISECONDS_PER_TICK))
    #define SCI_TICKTOMILLISECOND(_TICK)        ((_TICK) * (MILLISECONDS_PER_TICK))
#endif
#else
    #if defined(PLATFORM_UWS6121E)
    #define SCI_MILLISEONCDTOTICK(_MILLISEC)    (uint32)((uint64)_MILLISEC * 16384 / 1000)
    #define SCI_TICKTOMILLISECOND(_TICK)        (uint32)((uint64)_TICK * 1000 / 16384)
    #else
    #define SCI_MILLISEONCDTOTICK(_MILLISEC)    (_MILLISEC)
    #define SCI_TICKTOMILLISECOND(_TICK)        (_TICK)
    #endif
#endif  // End of WIN32


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // End of _THREADX_OS_H


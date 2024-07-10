/**
 ***********************************************************************************************************************
 * Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * @file        oneos_types.h
 *
 * @brief       OneOS most common data type definitions
 *
 * @revision
 * Date         Author          Notes
 * 2020-02-12   OneOS Team      First version
 ***********************************************************************************************************************
 */

#ifndef __OS_TYPES_H__
#define __OS_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum value of base type */
#define OS_UINT8_MAX                    0xffU                   /* Maxium value of UINT8 */
#define OS_UINT16_MAX                   0xffffU                 /* Maxium value of UINT16 */
#define OS_UINT32_MAX                   0xffffffffU             /* Maxium value of UINT32 */
#define OS_UINT64_MAX                   0xffffffffffffffffULL   /* Maxium value of UINT64 */

#ifdef OS_ARCH_CPU_64BIT
#define OS_TICK_MAX                     OS_UINT64_MAX           /* Maxium value of tick */
#else
#define OS_TICK_MAX                     OS_UINT32_MAX           /* Maxium value of tick */
#endif

/* Alignment size for CPU architecture data access */
#define OS_ALIGN_SIZE                   sizeof(os_base_t)

/* Basic data type definitions */
typedef signed char                     os_int8_t;              /* 8bit integer type */
typedef signed short                    os_int16_t;             /* 16bit integer type */
typedef signed int                      os_int32_t;             /* 32bit integer type */
#ifndef WIN32
typedef signed long long                os_int64_t;             /* 64bit integer type */
#endif
typedef unsigned char                   os_uint8_t;             /* 8bit unsigned integer type */
typedef unsigned short                  os_uint16_t;            /* 16bit unsigned integer type */
typedef unsigned int                    os_uint32_t;            /* 32bit unsigned integer type */
#ifndef WIN32
typedef unsigned long long              os_uint64_t;            /* 64bit unsigned integer type */
#endif
typedef signed int                      os_bool_t;              /* Boolean type */

/* Represent Nbit CPU */
typedef signed long                     os_base_t;              /* Nbit CPU related date type */
typedef unsigned long                   os_ubase_t;             /* Nbit unsigned CPU related data type */

/* Else data type definitions */
typedef os_int32_t                      os_err_t;               /* Type for error number */
typedef os_ubase_t                      os_tick_t;              /* Type for tick count */
typedef os_ubase_t                      os_size_t;              /* Type for size */
typedef os_base_t                       os_off_t;               /* Type for offset */

typedef int mode_t;
typedef long off_t;
#if OS_TASK_PRIORITY_MAX > 255
typedef os_uint16_t                     os_priority_t;              /* Type for priority */
#else
typedef os_uint8_t                      os_priority_t;              /* Type for priority */
#endif


// add for adpating sci api
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
// Invalid state value.
#define OS_INVALID_STATE           ~0

//---------------------------------------------
// API return values.
//---------------------------------------------
//Success, no error.
#define OS_SUCCESS                 0x00
//Object was deleted.
#define OS_DELETED                 0x01
//Invalid pool pointer.
#define OS_POOL_ERROR              0x02
//Invalid pointer.
#define OS_PTR_ERROR               0x03
//A wait option other than TX_NO_WAIT was specified on call from a non-thread.
#define OS_WAIT_ERROR              0x04
//Size is invalid.
#define OS_SIZE_ERROR              0x05
//Invalid event group pointer.
#define OS_GROUP_ERROR             0x06
//Service was unable to get the specified events.
#define OS_NO_EVENTS               0x07
//Invalid option was specified.
#define OS_OPTION_ERROR            0x08
//Invalid queue pointer.
#define OS_QUEUE_ERROR             0x09
//Service was unable to retrieve a message because the queue was empty.
#define OS_QUEUE_EMPTY             0x0A
//Service was unable to send message because the queue was full.
#define OS_QUEUE_FULL              0x0B
//Invalid counting semaphore pointer.
#define OS_SEMAPHORE_ERROR         0x0C
//Service was unable to retrieve an instance of the counting semaphore (semaphore count is zero).
#define OS_NO_INSTANCE             0x0D
//Invalid thread control pointer.
#define OS_THREAD_ERROR            0x0E
//Invalid thread priority, which is a value outside the range of 0-31.
#define OS_PRIORITY_ERROR          0x0F
//Service was unable to allocate memory.
#define OS_NO_MEMORY               0x10
//Specified thread is not in a terminated or completed state.
#define OS_DELETE_ERROR            0x11
//Specified thread is not suspended.
#define OS_RESUME_ERROR            0x12
//Invalid caller of this service.
#define OS_CALLER_ERROR            0x13
//Specified thread is in a terminated or completed state.
#define SCI_SUSPEND_ERROR           0x14
//Invalid application timer pointer.
#define OS_TIMER_ERROR             0x15
//Invalid value (a zero) supplied for initial ticks.
#define OS_TICK_ERROR              0x16
//Timer was already active./Invalid activation selected.
#define OS_ACTIVATE_ERROR          0x17
//Invalid preemption threshold specified.
//This value must be a valid priority less than or equal to the initial priority of the thread.
#define OS_THRESH_ERROR            0x18
//Previously set delayed suspension was lifted.
#define OS_SUSPEND_LIFTED          0x19
//Suspension was aborted by another thread, timer, or ISR.
#define OS_WAIT_ABORTED            0x1A
//Specified thread is not in a waiting state.
#define OS_WAIT_ABORT_ERROR        0x1B
//Invalid mutex pointer.
#define OS_MUTEX_ERROR             0x1C
//Service was unable to get ownership of the mutex.
#define OS_NOT_AVAILABLE           0x1D
//Mutex is not owned by caller.
#define OS_NOT_OWNED               0x1E
//Invalid priority inherit parameter.
#define OS_INHERIT_ERROR           0x1F
//Invalid auto-start selection.
#define OS_START_ERROR             0x20
//Parameter is invalid.
#define OS_PARAM_ERROR             0x21
//Normal error.
#define OS_ERROR                   0xFF

//---------------------------------------------
// API input parameters.
//---------------------------------------------
// Boolean value
#define OS_TRUE                     1       // Boolean true value
#define OS_FALSE                    0       // Boolean false value

// Wait option.
#define OS_NO_WAIT                 0U
#define OS_WAIT_FOREVER            OS_TICK_MAX

// Option used to set/get event.
#define OS_EVENT_OPTION_OR          0
#define OS_EVENT_OPTION_OR_CLEAR    1
#define OS_EVENT_OPTION_AND         2
#define OS_EVENT_OPTION_AND_CLEAR   3

// Auto start option on thread
#define OS_DONT_START              0
#define OS_AUTO_START              1

// Indicates if the thread is preemptable.
#define OS_NO_PREEMPT              0
#define OS_PREEMPT                 1

// Auto start option on timer.
#define OS_NO_ACTIVATE             0
#define OS_AUTO_ACTIVATE           1

//priority inherit mode for mutex
#define OS_NO_INHERIT 0
#define OS_INHERIT 1

//---------------------------------------------
// General constants.
//---------------------------------------------
// Max ID of static thread MUST be less than this value.
#define OS_MAX_STATIC_TASK_ID    100
// Number of all static threads should less than this value.
#define OS_MAX_STATIC_TASK_NUM   100

#define OS_NULL                    0x0
#define OS_INVALID_BLOCK_ID        0xFFFFFFFF

// Name size of thread, timer, queue, etc...
#define OS_MAX_NAME_SIZE           32

#ifdef __cplusplus
}
#endif

#endif /* __OS_TYPES_H__ */

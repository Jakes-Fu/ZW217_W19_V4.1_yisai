/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef _PTHREAD_H_
#define _PTHREAD_H_

//#include <time.h>
//#include <signal.h>
//#include <sched.h>
//#include <limits.h>
//#include <sys/types.h>
#include "time.h"
#include "sci_types.h"
#include "fake.h"
#include "os_api.h"

/*
 * Types
 */
typedef struct
{
    int volatile value;
} pthread_mutex_t;

typedef struct
{
    int volatile value;
} pthread_rwlock_t;

#define  PTHREAD_MUTEX_INITIALIZER             {0}
#define  PTHREAD_RECURSIVE_MUTEX_INITIALIZER   {0x4000}
#define  PTHREAD_ERRORCHECK_MUTEX_INITIALIZER  {0x8000}

#define SIG_BLOCK 0
#define SIGRTMIN  35

typedef long pthread_mutexattr_t;
typedef long pthread_rwlockattr_t;
typedef long pthread_condattr_t;
typedef int pthread_key_t;
typedef long pthread_t;
typedef int clockid_t;
typedef volatile int  pthread_once_t;

/**
 * @brief timer
 */
struct itimerspec
{
    struct timespec it_interval; /**< Timer period. */
    struct timespec it_value;    /**< Timer expiration. */
};

enum {
    PTHREAD_MUTEX_NORMAL = 0,
    PTHREAD_MUTEX_RECURSIVE = 1,
    PTHREAD_MUTEX_ERRORCHECK = 2,

    PTHREAD_MUTEX_ERRORCHECK_NP = PTHREAD_MUTEX_ERRORCHECK,
    PTHREAD_MUTEX_RECURSIVE_NP  = PTHREAD_MUTEX_RECURSIVE,

    PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL
};



/**
 * @brief Condition variable.
 */
typedef struct
{
        BOOLEAN xIsInitialized;            /**< Set to pdTRUE if this condition variable is initialized, pdFALSE otherwise. */
        SCI_SEMAPHORE_PTR xCondWaitSemaphore; /**< Threads block on this semaphore in pthread_cond_wait. */
        unsigned iWaitingThreads;             /**< The number of threads currently waiting on this condition variable. */
    } pthread_cond_internal_t;

/**
 * @brief Compile-time initializer of pthread_cond_internal_t.
 */

    #define FREERTOS_POSIX_COND_INITIALIZER \
    ( ( ( pthread_cond_internal_t )         \
    {                                       \
        .xIsInitialized = FALSE,          \
        .xCondWaitSemaphore = 0,    \
        .iWaitingThreads = 0                \
    }                                       \
        )                                   \
    )
typedef  pthread_cond_internal_t pthread_cond_t;

#define PTHREAD_RWLOCK_INITIALIZER \
    ( ( ( pthread_rwlock_t )         \
    {                                       \
        .value = FALSE,          \
    }                                       \
        )                                   \
    )
typedef struct
{
    uint32_t flags;
    void * stack_base;
    size_t stack_size;
    size_t guard_size;
    int32_t sched_policy;
    int32_t sched_priority;
} pthread_attr_t;

/*
 * Defines
 */
#define PTHREAD_COND_INITIALIZER  {0}

#define PTHREAD_STACK_MIN (2 * PAGE_SIZE)

#define PTHREAD_CREATE_DETACHED  0x00000000
#define PTHREAD_CREATE_JOINABLE  0x00000001

#define PTHREAD_ONCE_INIT    0

#define PTHREAD_PROCESS_PRIVATE  0
#define PTHREAD_PROCESS_SHARED   1

#define PTHREAD_SCOPE_SYSTEM     0
#define PTHREAD_SCOPE_PROCESS    1

/*
 * Prototypes
 */
#if __cplusplus
extern "C" {
#endif

pthread_t dm_pthread_self( void );
int dm_pthread_attr_init(pthread_attr_t * attr);
int dm_pthread_attr_setdetachstate( pthread_attr_t * attr, int detachstate);
int dm_pthread_create( pthread_t * thread, const pthread_attr_t * attr, void *( *startroutine )( void * ), void * arg );
void dm_pthread_exit( void * value_ptr );

#if __cplusplus
} /* extern "C" */
#endif

/************ TO FIX ************/

#define LONG_LONG_MAX __LONG_LONG_MAX__
#define LONG_LONG_MIN (-__LONG_LONG_MAX__ - 1)

#endif // _PTHREAD_H_

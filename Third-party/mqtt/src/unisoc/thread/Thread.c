/*******************************************************************************
 * Copyright (c) 2009, 2018 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial implementation
 *    Ian Craggs, Allan Stockdill-Mander - async client updates
 *    Ian Craggs - bug #415042 - start Linux thread as disconnected
 *    Ian Craggs - fix for bug #420851
 *    Ian Craggs - change MacOS semaphore implementation
 *    Ian Craggs - fix for clock #284
 *******************************************************************************/

/**
 * @file
 * \brief Threading related functions
 *
 * Used to create platform independent threading functions
 */

#include "Thread.h"

#include "Log.h"
#include "StackTrace.h"

#undef malloc
#undef realloc
#undef free

#if !defined(WIN32) && !defined(WIN64)
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#endif
#include <stdlib.h>

#include "OsWrapper.h"

#define PAHOMQTT_PRIORITY (231)
#define PAHOMQTT_STACK_SIZE (1024 * 16)
#define PAHOMQTT_EVENT_QUEUE_SIZE (32)

BLOCK_ID Thread_start(void (*entry)(uint32, void*), void *argv, BOOLEAN flag)
{
    if(flag == 1)
        return SCI_CreateThread("[P_send Mqtt]", "[Q_send Mqtt]", entry, 1, argv, PAHOMQTT_STACK_SIZE, PAHOMQTT_EVENT_QUEUE_SIZE, 
                                                PAHOMQTT_PRIORITY, SCI_PREEMPT, SCI_AUTO_START);
    else
        return SCI_CreateThread("[P_receive Mqtt]", "[Q_receive Mqtt]", entry, 1, argv, PAHOMQTT_STACK_SIZE, PAHOMQTT_EVENT_QUEUE_SIZE, 
                                                PAHOMQTT_PRIORITY, SCI_PREEMPT, SCI_AUTO_START);
}

int Thread_stop(void)
{
    SCI_ThreadExit();
    return 0;
}

SCI_MUTEX_PTR Thread_create_mutex(const char *name_ptr)
{
    return SCI_CreateMutex(name_ptr, SCI_INHERIT);
}

uint32 Thread_lock_mutex(SCI_MUTEX_PTR mutex_ptr)
{
    return SCI_GetMutex(mutex_ptr, SCI_WAIT_FOREVER);
}

uint32 Thread_unlock_mutex(SCI_MUTEX_PTR mutex_ptr)
{
    return SCI_PutMutex(mutex_ptr);
}

uint32 Thread_destroy_mutex(SCI_MUTEX_PTR mutex_ptr)
{
    return SCI_DeleteMutex(mutex_ptr);
}

BLOCK_ID Thread_getid(void)
{
    return SCI_IdentifyThread();
}

SCI_SEMAPHORE_PTR Thread_create_cond(const char *name_ptr)
{
    return SCI_CreateSemaphore(name_ptr, 0);
}

uint32 Thread_signal_cond(SCI_SEMAPHORE_PTR sem_ptr)
{
    return SCI_PutSemaphore(sem_ptr);
}

uint32 Thread_wait_cond(SCI_SEMAPHORE_PTR sem_ptr, int timeout)
{
    return SCI_GetSemaphore(sem_ptr, timeout * 1000);
}

uint32 Thread_destroy_cond(SCI_SEMAPHORE_PTR sem_ptr)
{
    return SCI_DeleteSemaphore(sem_ptr);
}

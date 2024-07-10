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
 *    Ian Craggs - fix for bug #420851
 *    Ian Craggs - change MacOS semaphore implementation
 *******************************************************************************/
//#include "UniTime.h"
//#include "MQTTClient.h"
#include "os_api.h"
#if !defined(__THREAD_H__)
#define __THREAD_H__

//#define thread_id_type void *


BLOCK_ID Thread_start(void (*entry)(uint32, void*), void *argv,BOOLEAN flag);
int Thread_stop(void);
SCI_MUTEX_PTR Thread_create_mutex(const char *name_ptr);
uint32 Thread_lock_mutex(SCI_MUTEX_PTR mutex_ptr);
uint32 Thread_unlock_mutex(SCI_MUTEX_PTR mutex_ptr);
uint32 Thread_destroy_mutex(SCI_MUTEX_PTR mutex_ptr);
BLOCK_ID Thread_getid(void);
SCI_SEMAPHORE_PTR Thread_create_cond(const char *name_ptr);
uint32 Thread_signal_cond(SCI_SEMAPHORE_PTR sem_ptr);
uint32 Thread_wait_cond(SCI_SEMAPHORE_PTR sem_ptr, int timeout);
uint32 Thread_destroy_cond(SCI_SEMAPHORE_PTR sem_ptr);

#endif

/*******************************************************************************
 *
 * Copyright (c) 2013, 2014, 2015 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *******************************************************************************/

#include <liblwm2m.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
//#include "osi_api.h"   //modify by xiameng.yang
#include "internals.h"
//#include "osi_log.h"   //modify by xiameng.yang


#include "liblwm2m.h"

#include "lwm2m_api.h"
#ifndef LWM2M_MEMORY_TRACE

void * lwm2m_malloc(size_t s)
{
    char * ret;
    
    ret = malloc(s);
    if(ret == NULL){
        LOG("malloc failed");
        return NULL;
    }
    memset(ret,0,s);
        
    return ret;
}

void lwm2m_free(void * p)
{
    free(p);
}

char * lwm2m_strdup(const char * str)
{
    char *cpStrBuf;
    cpStrBuf = (char *)lwm2m_malloc(strlen(str)+1);
    if ( NULL != cpStrBuf ) {
        strcpy(cpStrBuf, str);
    }
    return cpStrBuf;
}

#endif

int lwm2m_strncmp(const char * s1,
                     const char * s2,
                     size_t n)
{
    return strncmp(s1, s2, n);
}

time_t lwm2m_gettime(void)
{
    struct timeval tv;

    if (0 != gettimeofday(&tv, NULL))
    {
        return -1;
    }

    return tv.tv_sec;
}
#if 0
void lwm2m_printf(const char * format, ...)
{
    
    char uart_buf[350];
    va_list ap;

    va_start (ap, format);
    vsnprintf(uart_buf, sizeof(uart_buf), format, ap);
    va_end (ap);

    OSI_LOGI(0x0, uart_buf);
}
#endif

SCI_MUTEX_PTR lwm2m_lockcreate(char * mutex_name)
{
    //(*mutex) = (void *)osiMutexCreate();
    return SCI_CreateMutex(mutex_name , SCI_INHERIT);
}

void lwm2m_lockdestory(SCI_MUTEX_PTR mutex)
{
    //osiMutexDelete((osiMutex_t *)(mutex))
    SCI_DeleteMutex(mutex);
}

void lwm2m_lock(SCI_MUTEX_PTR mutex,uint32_t ms)
{
    //osiMutexTryLock(mutex, ms);
    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);
    
}

void lwm2m_unlock(SCI_MUTEX_PTR mutex)
{
    SCI_PutMutex(mutex);
}



int lwm2m_Thread_stop(void)
{
    SCI_ThreadExit();
    return 0;
}
BLOCK_ID lwm2m_Thread_start(char * name,void (*entry)(uint32, void*), void *argv)
{ 
    char   q_name[24 +2];
    char * temp = "Q_";
    if(strlen(name)>24)
    {
        SCI_TRACE_LOW(" [ LWM2MDEBUG]   thread name too long [%s %d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    snprintf((char *)q_name, strlen(name) +2+1,"%s%s",temp,name);

    SCI_TRACE_LOW(" [ LWM2MDEBUG]   name :%s , q_name:%s  [%s %d]\n",name,q_name,__FUNCTION__,__LINE__);
    
    return SCI_CreateThread(name, q_name, entry, 1, argv, LWM2MTASK_STACK_SIZE, LWM2MTASK_EVENT_QUEUE_SIZE, 
                                                LWM2MTASK_PRIORITY, SCI_PREEMPT, SCI_AUTO_START);
}
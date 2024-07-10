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
#if 0 //modify by unisoc
#include <sys/time.h>
#else
#include "time.h"
#include "os_api.h"
#include "dal_time.h"
#endif

char *strdup(const char* str);

char *strdup(const char* str)   //add from frmode.c
{
    size_t len;
    char *newstr;

    if(!str)
        return (char*)NULL;

    len = strlen(str);
    if(len >= ((size_t)-1) / sizeof(char))
        return (char *)NULL;

    newstr = malloc((len+1)*sizeof(char));
    if (!newstr)
        return (char*)NULL;

    memcpy(newstr, str, (len+1)*sizeof(char));

    return newstr;
}

#ifndef LWM2M_MEMORY_TRACE

void * lwm2m_malloc(size_t s)
{
    return malloc(s);
}

void lwm2m_free(void * p)
{
#if 0 //modify by unisoc
    return free(p);
#else
    free(p);
#endif
}

char * lwm2m_strdup(const char * str)
{
    return strdup(str);
}

#endif

int lwm2m_strncmp(const char * s1,
                     const char * s2,
                     size_t n)
{
    return strncmp(s1, s2, n);
}
#if 0
int gettimeofday_c(struct timeval *tv, void *zone)
{    
    /* Silence warnings about unused parameters. */
    ( void ) zone;
        
    if (tv == NULL)
    {
        return -1;
    }
    
    tv->tv_sec = TM_GetTotalSeconds();
    tv->tv_usec = 0;
    
    return 0;
}
#endif
time_t lwm2m_gettime(void)
{
    struct timeval tv = {0, 0};

    if (0 != gettimeofday_c(&tv, NULL))
    {
        return -1;
    }

    return tv.tv_sec;
}

#if 0 //modify by unisoc
void lwm2m_printf(const char * format, ...)
{
    va_list ap;

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
}
#endif

char *lwm2m_getcurtime()
{
    static char s[20];
#if 0 //modify by unisoc
    time_t t;
    struct tm* ltime;

    time(&t);
    ltime = localtime(&t);
    strftime(s, 20, "%Y-%m-%d-%H-%M-%S", ltime);
#else
    SCI_DATE_T  tmp_date;
    SCI_TIME_T  tmp_time;

    TM_GetSysDate(&tmp_date);
    TM_GetSysTime(&tmp_time);

    snprintf(s, 20, "%d-%d-%d-%d-%d-%d", tmp_date.year, tmp_date.mon, tmp_date.mday, tmp_time.hour, tmp_time.min, tmp_time.sec);
#endif
    return s;
}

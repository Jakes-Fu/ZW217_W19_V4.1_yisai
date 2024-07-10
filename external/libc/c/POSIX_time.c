/*
 * Amazon FreeRTOS POSIX V1.1.0
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file POSIX_time.c
 * @brief Implementation of clock functions in time.h
 */

/* C standard library includes. */
#include <stddef.h>
#include <string.h>

/* FreeRTOS+POSIX includes. */
#include "errno-base.h"
#include "pthread.h"
#include "time.h"


#include "POSIX_utils.h"
#include "dal_time.h"

#define     SEC_PER_DAY                  86400
#define     SEC_PER_HOUR                 3600
#define     SEC_PER_MIN                  60
#define     DAY_PER_YEAR                 365
#define     DATE_MAX_YEAR                2099   //时间设置最大年份
#define     DATE_MIN_YEAR                1980   //时间设置最小年份
//#define     DAY_PER_YEAR                 365
#define     DATE_MAX_MONTH               12

static uint16 day_per_mon[DATE_MAX_MONTH] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static struct tm s_time = {0};

static BOOLEAN IsLeapYear(uint32 year)
{
	BOOLEAN ret_val = FALSE;

	ret_val = (BOOLEAN)((!(year % 4) && (year % 100)) || !(year % 400));

	return ret_val;
}

static uint16 DaysPerMonth(uint8 month, uint16 year)
{
    if (month != 2)
    {
        return day_per_mon[month - 1];
    }
    else
    {
        return day_per_mon[1] + IsLeapYear(year);
    }
}

static struct tm Sec2Tm(uint32 second)
{
    struct tm tmptime = {0};
    int32 sec = 0;
    int32 day =0;
    uint16 y = 0;
    uint8 m = 0;
    uint16 d = 0;
    /* hour, min, sec */
    /* hour */
    sec = second % SEC_PER_DAY;
    tmptime.tm_hour = sec / SEC_PER_HOUR;

    /* min */
    sec %= SEC_PER_HOUR;
    tmptime.tm_min = sec / SEC_PER_MIN;

    /* sec */
    tmptime.tm_sec = sec % SEC_PER_MIN;

    /* year, month, day */
    /* year */
    /* year */
    day = second / SEC_PER_DAY;
    for (y = DATE_MIN_YEAR; day > 0; y++)
    {
        d = (DAY_PER_YEAR + IsLeapYear(y));
        if (day >= d)
        {
            day -= d;
        }
        else
        {
            break;
        }
    }
    tmptime.tm_year = y;

    for (m = 1; m < DATE_MAX_MONTH; m++)
    {
        d = DaysPerMonth(m, y);
        if (day >= d)
        {
            day -= d;
        }
        else
        {
            break;
        }
    }
   
    tmptime.tm_mon = m;
    tmptime.tm_mday = (uint8)(day + 1);

    return tmptime;
}

/*-----------------------------------------------------------*/
clock_t clock( void )
{
    /* This function is currently unsupported. It will always return -1. */
	SCI_TraceLow("\n===[%s, %d] Don't support the api.\n", __FUNCTION__, __LINE__);
    return ( clock_t ) -1;
}

/*-----------------------------------------------------------*/

int clock_getcpuclockid( pid_t pid,
                         clockid_t * clock_id )
{
    /* Silence warnings about unused parameters. */
    ( void ) pid;
    ( void ) clock_id;

    /* This function is currently unsupported. It will always return EPERM. */
	
	SCI_TraceLow("\n===[%s, %d] Don't support the api.\n", __FUNCTION__, __LINE__);
    return EPERM;
}

/*-----------------------------------------------------------*/

int clock_getres( clockid_t clock_id,
                  struct timespec * res )
{
    /* Silence warnings about unused parameters. */
    ( void ) clock_id;
    ( void ) res;
    
	SCI_TraceLow("\n===[%s, %d] Don't support the api.\n", __FUNCTION__, __LINE__);
    return EPERM;
}

/*-----------------------------------------------------------*/

int clock_gettime( clockid_t clock_id,
                   struct timespec * tp )
{
    uint32 xCurrentTime = 0;

    /* Silence warnings about unused parameters. */
    ( void ) clock_id;
    
    if (tp == NULL)
    {
        return -1;
    }

    if  (clock_id == CLOCK_REALTIME)
    {
        tp->tv_sec = TM_GetTotalSeconds();
        tp->tv_nsec = 0;
    }
    else //CLOCK_MONOTONIC
    {
        xCurrentTime = SCI_GetTickCount();
        tp->tv_sec = xCurrentTime/MICROSECONDS_PER_SECOND;
        tp->tv_nsec = (xCurrentTime%MICROSECONDS_PER_SECOND)*NANOSECONDS_PER_MICROSECONDS;
    }
    SCI_TraceLow("\n===[%s, %d] Type=%d, tp->tv_sec=%d, tp->tv_nsec=%d.\n", __FUNCTION__, __LINE__, tp->tv_sec, tp->tv_nsec);

    return 0;
}

/*-----------------------------------------------------------*/

int clock_nanosleep( clockid_t clock_id,
                     int flags,
                     const struct timespec * rqtp,
                     struct timespec * rmtp )
{
    int iStatus = 0;
    TickType_t xSleepTime = 0;
    struct timespec xCurrentTime = { 0 };

    /* Silence warnings about unused parameters. */
    ( void ) clock_id;
    ( void ) rmtp;
    ( void ) flags; /* This is only ignored if INCLUDE_vTaskDelayUntil is 0. */
    
    /* Check rqtp. */
    if( UTILS_ValidateTimespec( rqtp ) == false )
    {
        iStatus = EINVAL;
    }

    /* Get current time */
    if( ( iStatus == 0 ) && ( clock_gettime( CLOCK_REALTIME, &xCurrentTime ) != 0 ) )
    {
        iStatus = EINVAL;
    }

    if( iStatus == 0 )
    {
        /* Check for absolute time sleep. */
        if( ( flags & TIMER_ABSTIME ) == TIMER_ABSTIME )
        {
            /* Get current time */
            if( clock_gettime( CLOCK_REALTIME, &xCurrentTime ) != 0 )
            {
                iStatus = EINVAL;
            }

            /* Get number of ticks until absolute time. */
            if( ( iStatus == 0 ) && ( UTILS_AbsoluteTimespecToDeltaTicks( rqtp, &xCurrentTime, &xSleepTime ) == 0 ) )
            {
                SCI_Sleep( xSleepTime );
            }
        }
        else
        {
            /* If TIMER_ABSTIME isn't specified, convert rqtp to ticks and
             * sleep for a relative time. */
            if( UTILS_TimespecToTicks( rqtp, &xSleepTime ) == 0 )
            {
                SCI_Sleep( xSleepTime );
            }
        }
    }

    return iStatus;
}

/*-----------------------------------------------------------*/

int clock_settime( clockid_t clock_id,
                   const struct timespec * tp )
{
    /* Silence warnings about unused parameters. */
    ( void ) clock_id;
    ( void ) tp;

    /* This function is currently unsupported. It will always return -1 and
     * set errno to EPERM. */
	SCI_TraceLow("\n===[%s, %d] Don't support the api.\n", __FUNCTION__, __LINE__);
    return -1;
}

/*-----------------------------------------------------------*/

int nanosleep( const struct timespec * rqtp,
               struct timespec * rmtp )
{
    int iStatus = 0;
    TickType_t xSleepTime = 0;

    /* Silence warnings about unused parameters. */
    ( void ) rmtp;
    
    /* Check rqtp. */
    if( UTILS_ValidateTimespec( rqtp ) == false )
    {
        iStatus = -1;
    }

    if( iStatus == 0 )
    {
        /* Convert rqtp to ticks and delay. */
        if( UTILS_TimespecToTicks( rqtp, &xSleepTime ) == 0 )
        {
            SCI_Sleep( xSleepTime );
        }
    }

    return iStatus;
}

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

struct tm *gmtime(const time_t *timer)
{
    time_t sec = 0;
    if (timer == NULL)
    {
        return NULL;
    }
    memset(&s_time, 0, sizeof(struct tm));
    sec = *timer;
    s_time = Sec2Tm(sec); //sec from 1980s
    if (s_time.tm_year > 1900)
    {
            s_time.tm_year  -= 1900;
    }
    return &s_time;
}

struct tm *gmtime_r(const time_t *timer, struct tm *result)
{
    if (timer == NULL || result == NULL)
    {
        return NULL;
    }
    result = gmtime(timer);
    return result;
}

time_t  sci_time(time_t *seconds)
{
    struct timeval tv;
    time_t sec;

    gettimeofday(&tv, NULL);
	if (seconds)
	{
        *seconds = tv.tv_sec;
    }

    return sec;
}

void timeradd(struct timeval *a , struct timeval *b , struct timeval *res)
{
    if(a == NULL || b == NULL || res == NULL)
    {
        return;    
    }
    res->tv_sec = a->tv_sec + b->tv_sec;			      
    res->tv_usec = a->tv_usec + b->tv_usec;			      
    if (res->tv_usec >= 1000000)					      
    {									      
        ++res->tv_sec;						      
        res->tv_usec -= 1000000;					      
    }
}


void timersub(struct timeval *a , struct timeval *b , struct timeval *res)
{
    if(a == NULL || b == NULL || res == NULL)
    {
        return;    
    }
    res->tv_sec = a->tv_sec - b->tv_sec;			      
    res->tv_usec = a->tv_usec - b->tv_usec;			      
    if (res->tv_usec < 0) 
    {					      
        --res->tv_sec;						      
        res->tv_usec += 1000000;		
    }
}
/*-----------------------------------------------------------*/

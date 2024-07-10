/*
 * Amazon FreeRTOS POSIX V1.1.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * @file time.h
 * @brief Time types.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/time.h.html
 */

#ifndef _TIME_H_
#define _TIME_H_

#include "sci_types.h"
#include "fake.h"

/**
 * @name Unit conversion constants.
 */
/**@{ */
#define NANOSECONDS_PER_MICROSECONDS    ( 1000000LL )                    /**< Nanoseconds per Microseconds. */
#define MICROSECONDS_PER_SECOND    ( 1000LL )                                   /**< Microseconds per second. */
#define NANOSECONDS_PER_SECOND     ( 1000000000LL )                          /**< Nanoseconds per second. */
/**@} */

/**
 * @name Clock identifiers.
 */
/**@{ */
#define CLOCK_REALTIME     0     /**< The identifier of the system-wide clock measuring real time. */
#define CLOCK_MONOTONIC    1     /**< The identifier for the system-wide monotonic clock.*/


/**
 * @name Flag indicating time is absolute.
 *
 * For functions taking timer objects, this refers to the clock associated with the timer.
 */
/**@{ */
#define TIMER_ABSTIME    0x01
/**@} */

#ifndef _TM_DEFINED
/*
 * A structure for storing all kinds of useful information about the
 * current (or another) time.
 */
struct tm
{
	int	tm_sec;		/* Seconds: 0-59 (K&R says 0-61?) */
	int	tm_min;		/* Minutes: 0-59 */
	int	tm_hour;	/* Hours since midnight: 0-23 */
	int	tm_mday;	/* Day of the month: 1-31 */
	int	tm_mon;		/* Months *since* january: 0-11 */
	int	tm_year;	/* Years since 1900 */
	int	tm_wday;	/* Days since Sunday (0-6) */
	int	tm_yday;	/* Days since Jan. 1: 0-365 */
	int	tm_isdst;	/* +1 Daylight Savings Time, 0 No DST,
				 * -1 don't know */
};
#define _TM_DEFINED
#endif

#ifndef _TIME_T
#define _TIME_T
typedef long time_t;
#endif

#ifndef __cplusplus//unisoc modified
#ifndef _TIMESPEC
#define _TIMESPEC
struct timespec
{
    time_t tv_sec; /**< Seconds. */
    long tv_nsec; /**< Nanoseconds. */
};
#endif

/* A time value that is accurate to the nearest
   microsecond but also has a range of years.  */
#ifndef _TIMEVAL
#define _TIMEVAL
struct timeval
{
    time_t tv_sec;		/* Seconds.  */
    long tv_usec;	/* Microseconds.  */
};
#endif
#else
#include <arm-tpl.h>
#endif

typedef uint32 clock_t;
typedef uint32 TickType_t;
#ifndef _CLOCKID_T
#define _CLOCKID_T
typedef int32 clockid_t;
#endif

#endif 

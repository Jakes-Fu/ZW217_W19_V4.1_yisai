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

#if 0//def WIN32
#include <time.h>
#else
#include_next <time.h>
#endif

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
#define CLOCK_REALTIME      0     /**< The identifier of the system-wide clock measuring real time. */
#define CLOCK_MONOTONIC     1     /**< The identifier for the system-wide monotonic clock.*/
#define CLOCK_MONOTONIC_RAW 3     // FIXME, check impl


/**
 * @name Flag indicating time is absolute.
 *
 * For functions taking timer objects, this refers to the clock associated with the timer.
 */
/**@{ */
#define TIMER_ABSTIME    0x01
/**@} */

#ifndef _TIME_T_DEFINED
#define _TIME_T_DEFINED
typedef unsigned int time_t;
#endif

typedef uint32 TickType_t;
#ifndef _CLOCKID_T
#define _CLOCKID_T
typedef int32 clockid_t;
#endif

#ifndef __cplusplus//unisoc modified
#ifndef _TIMEVAL_DEFINED
#define _TIMEVAL_DEFINED
/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
struct timeval {
    long    tv_sec;     /* seconds */
    long    tv_usec;    /* and microseconds */
};
#endif /* _TIMEVAL_DEFINED */

#ifndef _TIMESPEC_DEFINED	 
#define _TIMESPEC_DEFINED	 
/*	 
* Structure defined by POSIX.1b to be like a timeval.	 
*/	 
struct timespec {	 
   time_t  tv_sec;     /* seconds */	 
   long    tv_nsec;    /* and nanoseconds */	 
};	 
#endif /* _TIMESPEC_DEFINED */ 

#if 0//ndef _TM_DEFINED
typedef uint32 clock_t;

#define _TM_DEFINED
/*
 * A structure for storing all kinds of useful information about the
 * current (or another) time.
 */
struct tm {
    int tm_sec;   /* seconds after the minute, 0 to 60
                     (0 - 60 allows for the occasional leap second) */
    int tm_min;   /* minutes after the hour, 0 to 59 */
    int tm_hour;  /* hours since midnight, 0 to 23 */
    int tm_mday;  /* day of the month, 1 to 31 */
    int tm_mon;   /* months since January, 0 to 11 */
    int tm_year;  /* years since 1900 */
    int tm_wday;  /* days since Sunday, 0 to 6 */
    int tm_yday;  /* days since January 1, 0 to 365 */
    int tm_isdst; /* Daylight Savings Time flag */
    union {       /* ABI-required extra fields, in a variety of types */
        struct {
            int __extra_1, __extra_2;
        };
        struct {
            long __extra_1_long, __extra_2_long;
        };
        struct {
            char *__extra_1_cptr, *__extra_2_cptr;
        };
        struct {
            void *__extra_1_vptr, *__extra_2_vptr;
        };
    };
};
#endif
#endif

#if __cplusplus
extern "C" {
#endif
/**
 * @brief Report CPU time used.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/clock.html
 *
 * @return  The number of FreeRTOS ticks since the scheduler
 * was started minus the ticks spent in the idle task.
 *
 * @note This function does NOT report the number of ticks spent by the calling thread.
 */
unsigned int clock( void );

/**
 * @brief Access a process CPU-time clock.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/clock_getcpuclockid.html
 *
 * @retval EPERM
 *
 * @note This function is currently unsupported.
 *
 */
int clock_getcpuclockid( pid_t pid,
                         clockid_t * clock_id );

/**
 * @brief Returns the resolution of a clock.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/clock_getres.html
 *
 * @note clock_id is ignored
 * @note This function stores the resolution of the FreeRTOS tick count in the object res points to.
 *
 * @retval 0 - Upon successful execution
 */
int clock_getres( clockid_t clock_id,
                  struct timespec * res );

/**
 * @brief Returns the current value for the specified clock, clock_id.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/clock_gettime.html
 *
 * @note clock_id is ignored
 * @note  this function does not check for overflows of time_t.
 *
 * @retval 0 - Upon successful completion.
 */
int clock_gettime( clockid_t clock_id,
                   struct timespec * tp );

/**
 * @brief High resolution sleep with specifiable clock.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/clock_nanosleep.html
 *
 * @note clock_id is ignored, as this function uses the FreeRTOS tick count as its clock.
 * @note flags is ignored, if INCLUDE_vTaskDelayUntil is 0. i.e. the FreeRTOS function vTaskDelayUntil isn't available.
 * @note rmtp is also ignored, as signals are not implemented.
 *
 * @retval 0 - Upon successful completion.
 * @retval EINVAL - The rqtp argument specified a nanosecond value less than zero or greater than or equal to 1000 million.
 */
int clock_nanosleep( clockid_t clock_id,
                     int flags,
                     const struct timespec * rqtp,
                     struct timespec * rmtp );

/**
 * @brief Sets the time for the specified clock.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/clock_settime.html
 *
 * @retval -1 with errno set to EPERM.
 *
 * @note This function is currently unsupported, as FreeRTOS does not provide a function to modify the tick count.
 */
int clock_settime( clockid_t clock_id,
                   const struct timespec * tp );

/**
 * @brief High resolution sleep.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/nanosleep.html
 *
 * @note rmtp is ignored, as signals are not implemented.
 *
 * @retval 0 - Upon successful completion.
 * @retval -1 - The rqtp argument is invalid OR the rqtp argument specified a nanosecond value less than zero or greater than or equal to 1000 million.
 *
 */
int nanosleep( const struct timespec * rqtp,
               struct timespec * rmtp );

/**
 * @brief Get the current precise time(from January 1, 1980 to the present).
 *
 *
 * @note zone is ignored, as signals are not implemented.
 *
 * @retval 0 - Upon successful completion.
 * @retval -1 - The tv argument is invalid.
 *
 */
int gettimeofday_c(struct timeval *tv, void *zone);

#define gettimeofday gettimeofday_c

struct tm *localtime_r(const time_t *t, struct tm *tm);



/**
 * @brief Convert seconds to GMT time.
 *
 * @retval address of GMT time.
 */
struct tm *gmtime(const time_t *timer);

/**
 * @brief Convert seconds to GMT time.
 *
 * @note result, the user space to store GMT time.
 *
 * @retval address of GMT time.
 *
 */
struct tm *gmtime_r(const time_t *timer, struct tm *result);
#define time(x) sci_time(x)

#if __cplusplus
} /* extern "C" */
#endif

#endif

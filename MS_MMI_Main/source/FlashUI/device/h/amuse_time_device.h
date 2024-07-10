/*
* amuse_time_device.h
* Copyright (C) 2011 Microrapid Inc
* Author: Tim <tim@Microrapid.com>
* 
* This file contains time interface that need to 
* be implemented on different platforms.
*/

#ifndef _AMUSE_TIME_DEVICE_H_
#define _AMUSE_TIME_DEVICE_H_

#ifdef HTW_SDK
#ifndef USE_GCC
#define USE_GCC 1
#endif
#endif

#ifdef __cplusplus
extern "C"{
#endif

typedef unsigned int amuse_time_t;

struct amuse_tm {
	int tm_sec;     /* seconds after the minute - [0,59] */
	int tm_min;     /* minutes after the hour - [0,59] */
	int tm_hour;    /* hours since midnight - [0,23] */
	int tm_mday;    /* day of the month - [1,31] */
	int tm_mon;     /* months since January - [0,11] */
	int tm_year;    /* years since 1900 */
	int tm_wday;    /* days since Sunday - [0,6] */
	int tm_yday;    /* days since January 1 - [0,365] */
	int tm_isdst;   /* daylight savings time flag */
#if USE_GCC
	long int tm_gmtoff;           /* Seconds east of UTC.  */
	char *tm_zone;
#endif
};

/*
* Retrieves the number of milliseconds 
* that have elapsed since the system was started.
* 
* Parameters: none
* 
* Returns: the number of milliseconds that have elapsed since the system was started.
*/
unsigned int amuse_getTickCount(void);

/*
* Get the system time.
* 
* Parameters: 
* result: pointer to the storage location for time
* 
* Returns: the time as seconds elapsed since midnight, January 1, 1970
*/
amuse_time_t amuse_time(amuse_time_t* result);

/*
 * This function converts a time value and correct for the local time zone.
 *
 * Parameters: 
 * timer: pointer to stored time
 *
 * Returns: a pointer to the amuse_tm structure result, or NULL if timer is invalid
 */
struct amuse_tm* amuse_localtime(const amuse_time_t* timer);

/*
* This function converts a time value to a amuse_tm structure.
*
* Parameters: 
* timer: pointer to stored time
*
* Returns: a pointer to the amuse_tm structure result, or NULL if timer is invalid
*/
struct amuse_tm* amuse_gmtime(const amuse_time_t* timer);

/*
 * This function converts the local time to a calendar value.
 *
 * Parameters: 
 * timeptr: pointer to amuse_tm structure
 *
 * Returns: the specified calendar time encoded as a value of type time_t, 
 *			or ¨C1 cast to type time_t if timeptr references an invalid date.
 */
amuse_time_t amuse_mktime(struct amuse_tm* timeptr);

/*
 * This function finds the difference between two times.
 *
 * Parameters: 
 * time1: ending time
 * time0: beginning time
 *
 * Returns: the elapsed time in seconds from timer0 to timer1, or 0 to indicate an error.
 */
double amuse_difftime(amuse_time_t time1, amuse_time_t time0);

#ifdef __cplusplus
}
#endif

#endif

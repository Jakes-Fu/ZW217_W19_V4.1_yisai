/*
* amuse_time_device.c
* Copyright (C) 2011 Microrapid Inc
* Author: Tim <tim@Microrapid.com>
* 
* This file contains time interface implemented on win32 platforms.
*/
#ifndef FLASHUI_WIN32_SIM_DLL
#include "Os_api.h"
#include "dal_time.h"
#include "mmicom_time.h"
#else
#include "amuse.h"
#endif
#include "amuse_time_device.h"
#include <time.h>


unsigned int 
amuse_getTickCount(void)
{
	unsigned int ntime;
	ntime = (unsigned int)SCI_GetTickCount();
	return ntime;
}

amuse_time_t
amuse_time(amuse_time_t* t)
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    uint32      second = 0;

    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);
    second = MMIAPICOM_Tm2Second( sys_time.sec, 
                            sys_time.min, 
                            sys_time.hour,
                            sys_date.mday,
                            sys_date.mon, 
                            sys_date.year);
 return second;
}

struct amuse_tm* 
amuse_localtime(const amuse_time_t* timer)
{
	return (struct amuse_tm*)localtime((time_t*)timer);
}

struct amuse_tm* 
amuse_gmtime(const amuse_time_t* timer)
{
	return (struct amuse_tm*)gmtime((time_t*)timer);
}

amuse_time_t 
amuse_mktime(struct amuse_tm* timeptr)
{
	return (amuse_time_t)mktime((struct tm*)timeptr);
}

double 
amuse_difftime(amuse_time_t time1, amuse_time_t time0)
{
	return difftime(time1, time0);
}

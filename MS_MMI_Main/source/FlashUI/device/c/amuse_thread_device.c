/*
* amuse_thread_device.c
* Copyright (C) 2011 Microrapid Inc
* Author: Tim <tim@Microrapid.com>
* 
* This file implementes thread interface for win32 platform.
*/

#include "amuse_thread_device.h"
#ifndef WIN32
#include "Os_api.h"
#else
#include "amuse.h"
#endif


amuse_thread_handle
amuse_create_thread(amuse_thread_func pThreadFunction, void *pParameters, int prio)
{
	return 0;
}

int 
amuse_suspend_thread(amuse_thread_handle h)
{
	return 0;
}

int
amuse_resume_thread(amuse_thread_handle h)
{
	return 0;
}


void 
amuse_sleep(unsigned int duration)
{
	SCI_Sleep(duration);
	return;
}

int 
amuse_set_thread_priority(amuse_thread_handle h, int priority)
{
	return 0;
}

amuse_thread_locker 
amuse_lock_get()
{
	return 0;
}

void 
amuse_lock(amuse_thread_locker plocker)
{
	return;
}

void 
amuse_unlock(amuse_thread_locker plocker)
{
	return;
}

void 
amuse_lock_release(amuse_thread_locker plocker)
{
	return;
}

long 
amuse_atom_increment(volatile long* addend)
{
	return 0;
}

long 
amuse_atom_decrement(volatile long* addend)
{
	return 0;
}
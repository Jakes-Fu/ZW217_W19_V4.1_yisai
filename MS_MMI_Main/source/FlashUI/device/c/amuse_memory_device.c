/*
 * amuse_memory_device.c
 * Copyright (C) 2011 Microrapid Inc
 * Author: Tim <tim@Microrapid.com>
 * 
 * This file contains device related memory implementation for WIN32.
 */
#ifdef WIN32
#include "amuse.h"
#endif

#ifndef FLASHUI_WIN32_SIM_DLL
#include "os_api.h"
#endif
#include <setjmp.h>

jmp_buf amuse_mark;

jmp_buf * amuse_getJmp()
{
	return &amuse_mark;
}

void amuse_long_jump(int code)
{
	longjmp(amuse_mark, code);
}

void* 
amuse_malloc_device_wrapper(unsigned int size, char* fn, int ln)
{
	void *p = (void*)malloc( size);
	SCI_ASSERT(p);/*assert verified*/
	return p;
}

void* 
amuse_malloc_device(unsigned int size)
{
	void *p = (void*)malloc(size);
	SCI_ASSERT(p);/*assert verified*/
	return p;
}

void* amuse_realloc_device_wrapper(void *p,size_t size, char* fn, int ln)
{
	void * pp=NULL;
	if (size == 0)	return NULL;
	return pp;
}

void 
amuse_free_device(void* p)
{
	if(p)
		free(p);
}

void* 
amuse_safe_malloc(unsigned int size)
{
	return amuse_malloc_device(size);
}

void*
amuse_safe_calloc(unsigned int nmemb, unsigned int size)
{
	return 0;
}

void*
amuse_safe_realloc(void *p, unsigned int size)
{
	return 0;
}

void 
amuse_safe_free(void *p)
{
	amuse_free_device(p);
}


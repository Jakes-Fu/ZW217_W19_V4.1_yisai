/*
* amuse_memory_device.c
* Copyright (C) 2011 Microrapid Inc
* Author: Tim <tim@Microrapid.com>
* 
* This file contains device related memory interface.
*/

#ifndef _AMUSE_MEMORY_DEVICE_H_
#define _AMUSE_MEMORY_DEVICE_H_

#ifndef  IMPORT_C
#if WIN32
#define IMPORT_C	__declspec(dllimport)
#define EXPORT_C   __declspec(dllexport)
#else
#define IMPORT_C extern
#define EXPORT_C
#endif
#endif

/*
 *When memory is limitted, whether to exit?
 */
#if !ENABLE_FOR_RITU
#define EXIT_WHEN_MEMORY_LIMIT 1
#endif
/*
 * This macro determines how much memory can amuse use. when exceeds, the player will destroy
 *
 * Difference with amuse_memory_size:
 *		amuse_memory_size is heap1 size
 *		MAX_HEAP_TOTAL_SIZE is total heaps size
 */
//#define MAX_HEAPS_TOTAL_SIZE 0x3000000

#ifdef __cplusplus
extern "C"{
#endif

/*
 * This function allocates memory by using alloc API of device.
 *
 * Parameters: 
 * size: bytes to allocate
 *
 * Returns: a void pointer to the allocated space, or NULL if there is
 *			insufficient memory available or size is zero of negative.
 */
void* amuse_malloc_device(unsigned int size);

/*
 * This function allocates an array in memory with elements initialized to 0.
 *
 * Parameters: 
 * nmemb: number of elements
 * size: length in bytes of each element
 *
 * Returns: a void pointer to the allocated space, or NULL if there is
 *			insufficient memory available or size is zero of negative.
 */
void* amuse_calloc_device(unsigned int nmemb, unsigned int size);

/*
 * This function reallocate memory blocks.
 *
 * Parameters: 
 * p: pointer to previously allocated memory block
 * size: new size in bytes
 *
 * Returns: a void pointer to the allocated space, or NULL if there is
 *			insufficient memory available or size is zero of negative.
 */
void* amuse_realloc_device(void *p, unsigned int size);

/*
 * This function frees a memory block.
 *
 * Parameters: 
 * p: previously allocated memory block to be freed
 *
 * Returns: none
 */
void amuse_free_device(void* p);


/** Thread Safe Memory Allocation Interfaces **/

/**
 * The following interfaces do the same thing as above except
 * that they are thread-safe ones.
 */
void* amuse_safe_malloc(unsigned int size);
void* amuse_safe_calloc(unsigned int nmemb, unsigned int size);
void* amuse_safe_realloc(void *p, unsigned int size);
void amuse_safe_free(void *p);
EXPORT_C jmp_buf * amuse_getJmp(void);

#ifdef __cplusplus
}
#endif
#endif

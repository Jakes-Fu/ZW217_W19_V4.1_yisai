/*

Copyright (c) 2005-2008, Simon Howard

Permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

 */

/**
 * @file alloc-testing.h
 *
 * @brief Memory allocation testing framework.
 *
 * This file uses the preprocessor to redefine the standard C dynamic memory
 * allocation functions for testing purposes.  This allows checking that
 * code under test correctly frees back all memory allocated, as well as
 * the ability to impose artificial limits on allocation, to test that
 * code correctly handles out-of-memory scenarios.
 */

#ifndef ALLOC_TESTING_H
#define ALLOC_TESTING_H
#include "system_hal.h"
/* Don't redefine the functions in the alloc-testing.c, as we need the
 * standard malloc/free functions. */

#ifndef ALLOC_TESTING_C
/*
#undef malloc
#define malloc   alloc_test_malloc
#undef free
#define free     alloc_test_free
#undef realloc
#define realloc  alloc_test_realloc
#undef calloc
#define calloc   alloc_test_calloc
#undef strdup
#define strdup   alloc_test_strdup
*/
#undef HALO_Malloc(size)
#define HALO_Malloc(size) alloc_test_malloc(size)
#undef HALO_MallocAPP(size)
#define HALO_MallocAPP(size) alloc_test_malloc(size)
#undef HALO_Free(ptr) 
#define HALO_Free(ptr) alloc_test_free(ptr)
#undef HALO_ALLOCA(_SIZE)
#define HALO_ALLOCA(_SIZE)  alloc_test_malloc(size) 
#undef HALO_ASSERT(_EXP)
#define HALO_ASSERT(_EXP) (void)((!!(_EXP))||(PRTF(#_EXP " is Failed in function: [%s] line [%d]",__func__,__LINE__)))


extern void Printf_Buf(uint8* str, uint8* address, int len);

#undef PRTF_BLOCK(_EXP,PTR,LEN)
#define PRTF_BLOCK(_EXP,PTR,LEN) Printf_Buf(_EXP,PTR,LEN)

					
#undef HALO_ASSERT(_EXP)
#define HALO_ASSERT(_EXP) { \
							 if(!!(_EXP)) \
							 PRTF(#_EXP " is [OK]\n"); \
							 else \
							 { \
								 PRTF(#_EXP " is [FAIL] \n"); \
								 } \
						  }
#undef  HALO_PASSERT(_EXP,PRINT_STR)
#define HALO_PASSERT(_EXP,PRINT_STR) { \
							 PRTF(#PRINT_STR "\n"); \
							 if(!!(_EXP)) \
							 PRTF(#_EXP " is [OK]\n"); \
							 else \
							 { \
								 PRTF(#_EXP " is [FAIL]\n"); \
								 } \
						  }	
#define HALO_ASSERTRT(_EXP) { \
							 if(!!(_EXP)) \
							 PRTF(#_EXP " is [OK]\n"); \
							 else \
							 { \
								 PRTF(#_EXP " is [FAIL]\n"); \
								  return 0; \
								 } \
						  }
#define HALO_JUDGE(_EXP,VAL) {HALO_ASSERT(_EXP); JUDGE(_EXP,VAL);}
#define HALO_JUDGE_STR(PRINT_STR,_EXP,VAL) {HALO_PASSERT(_EXP,PRINT_STR); JUDGE(_EXP,VAL);}

#define JUDGE(_EXP,VAL) (!!(_EXP))?(VAL=1):(VAL=0)

#endif

/**
 * Allocate a block of memory.
 *
 * @param bytes          Number of bytes to allocate.
 * @return               Pointer to the new block, or NULL if it was not
 *                       possible to allocate the new block.
 */

void *alloc_test_malloc(size_t bytes);

/**
 * Free a block of memory.
 *
 * @param ptr            Pointer to the block to free.
 */

void alloc_test_free(void *ptr);

/**
 * Reallocate a previously-allocated block to a new size, preserving
 * contents.
 *
 * @param ptr            Pointer to the existing block.
 * @param bytes          Size of the new block, in bytes.
 * @return               Pointer to the new block, or NULL if it was not
 *                       possible to allocate the new block.
 */

void *alloc_test_realloc(void *ptr, size_t bytes);

/**
 * Allocate a block of memory for an array of structures, initialising
 * the contents to zero.
 *
 * @param nmemb          Number of structures to allocate for.
 * @param bytes          Size of each structure, in bytes.
 * @return               Pointer to the new memory block for the array,
 *                       or NULL if it was not possible to allocate the
 *                       new block.
 */

void *alloc_test_calloc(size_t nmemb, size_t bytes);

/**
 * Allocate a block of memory containing a copy of a string.
 *
 * @param string         The string to copy.
 * @return               Pointer to the new memory block containing the
 *                       copied string, or NULL if it was not possible
 *                       to allocate the new block.
 */

char *alloc_test_strdup(const char *string);

/**
 * Set an artificial limit on the amount of memory that can be
 * allocated.
 *
 * @param alloc_count    Number of allocations that are possible after
 *                       this call.  For example, if this has a value
 *                       of 3, malloc() can be called successfully
 *                       three times, but all allocation attempts
 *                       after this will fail.  If this has a negative
 *                       value, the allocation limit is disabled.
 */

void alloc_test_set_limit(signed int alloc_count);

/**
 * Get a count of the number of bytes currently allocated.
 *
 * @return               The number of bytes currently allocated by
 *                       the allocation system.
 */

size_t alloc_test_get_allocated(void);

#endif /* #ifndef ALLOC_TESTING_H */


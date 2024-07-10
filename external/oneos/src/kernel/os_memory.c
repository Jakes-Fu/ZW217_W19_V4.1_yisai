/**
 ***********************************************************************************************************************
 * Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * @file        os_memory.c
 *
 * @brief       This file implements system memory management and mulitiheap management.
 *
 * @revision
 * Date         Author          Notes
 * 2020-11-18   OneOS team      First version.
 ***********************************************************************************************************************
 */
#include "os_memory.h"
#include "os_assert.h"
#include "os_api.h"

/**
 ***********************************************************************************************************************
 * @brief           This function allocates 'size' bytes from system_memory_heap, returns pointer of the allocated
 *                  memory.
 *
 * @param[in]       size            The size of the requested block in bytes.
 *
 * @return          The pointer to allocated memory or OS_NULL if no free memory was found.
 ***********************************************************************************************************************
 */
void *os_malloc(os_size_t size)
{
    return SCI_ALLOCA(size);
}
/**
 ***********************************************************************************************************************
 * @brief           This function changes the size of the memory block pointed to by 'ptr' to 'newsize' bytes. If size
 *                  is equal to 0, it works in the same way as os_free(). If 'ptr' is OS_NULL, it works in the same way
 *                  as os_malloc().
 *
 * @param[in]       ptr             The pointer of memory block to change.
 * @param[in]       size            The size of new memory block.
 *
 * @return          The pointer to newly allocated memory or NULL.
 ***********************************************************************************************************************
 */
void *os_realloc(void *ptr, os_size_t size)
{
    OS_ASSERT(ptr);
    return SCI_REALLOC(ptr, size);
}

/**
 ***********************************************************************************************************************
 * @brief           This function allocates memory for an array of 'count' elements of 'size' bytes each and returns a
 *                  pointer to the allocated memory. The memory is set to zero.
 *
 * @param[in]       count           Number of array to allocate.
 * @param[in]       size            Size of each element to allocate.
 *
 * @return          The pointer to allocated memory or OS_NULL if no free memory was found.
 ***********************************************************************************************************************
 */
void *os_calloc(os_size_t count, os_size_t size)
{
    return SCI_CALLOC(count, size);
}

/**
 ***********************************************************************************************************************
 * @brief           This function frees the memory space pointed to by 'ptr', which allocated by os_malloc(), or
 *                  os_realloc() or os_calloc() or os_aligned_malloc().
 *
 * @param[in]       ptr             The pointer to memory space.
 *
 * @return          None.
 ***********************************************************************************************************************
 */
void os_free(void *ptr)
{
    OS_ASSERT(ptr);
    SCI_FREE(ptr);
}

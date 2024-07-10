

#ifndef RM_MEMORY_H
#define RM_MEMORY_H

#include "helix_types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

typedef void* (*rm_malloc_func_ptr) (void* pUserMem, UINT32 ulSize);
typedef void  (*rm_free_func_ptr)   (void* pUserMem, void* ptr);

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef RM_MEMORY_H */

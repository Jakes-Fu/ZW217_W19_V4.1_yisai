

#ifndef RM_MEMORY_DEFAULT_H
#define RM_MEMORY_DEFAULT_H

#include "helix_types.h"

void* rm_memory_default_malloc(void* pUserMem, UINT32 ulSize);
void rm_memory_default_free(void* pUserMem, void* ptr);

#endif /* RM_MEMORY_DEFAULT_H */

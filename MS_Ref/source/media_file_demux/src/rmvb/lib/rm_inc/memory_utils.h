

#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include "helix_types.h"
#include "helix_result.h"
#include "rm_memory.h"

HX_RESULT rm_enforce_buffer_min_size(void*              pUserMem,
                                     rm_malloc_func_ptr fpMalloc,
                                     rm_free_func_ptr   fpFree,
                                     BYTE**             ppBuf,
                                     UINT32*            pulCurLen,
                                     UINT32             ulReqLen);

BYTE* copy_buffer(void*              pUserMem,
                  rm_malloc_func_ptr fpMalloc,
                  BYTE*              pBuf,
                  UINT32             ulLen);

#endif /* #ifndef MEMORY_UTILS_H */

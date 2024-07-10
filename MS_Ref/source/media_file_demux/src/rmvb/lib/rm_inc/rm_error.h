

#ifndef RM_ERROR_H
#define RM_ERROR_H

#include "helix_types.h"
#include "helix_result.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

typedef void (*rm_error_func_ptr) (void* pUserError, HX_RESULT err, const char* msg);

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef RM_ERROR_H */

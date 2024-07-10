

#ifndef RM_ERROR_DEFAULT_H
#define RM_ERROR_DEFAULT_H

#include "helix_types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

void rm_error_default(void* pUserError, HX_RESULT err, const char* msg);

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef RM_ERROR_DEFAULT_H */



#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "helix_types.h"
#include "rm_memory.h"
/* Create and copy a string */
char* copy_string(void*              pUserMem,
                  rm_malloc_func_ptr fpMalloc,
                  const char*        pszStr);

/* Free a string */
void free_string(void*            pUserMem,
                 rm_free_func_ptr fpFree,
                 char**           ppszStr);


#if defined(_UNIX)
#include <strings.h>
#else
int strcasecmp(const char* pszStr1, const char* pszStr2);
int strncasecmp(const char* pszStr1, const char* pszStr2, int len);
#endif

#endif /* #ifndef STRING_UTILS_H */

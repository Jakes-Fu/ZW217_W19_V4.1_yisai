

#include <string.h>
#include <ctype.h>
#include "string_utils.h"

char* copy_string(void*              pUserMem,
                  rm_malloc_func_ptr fpMalloc,
                  const char*        pszStr)
{
    char* pRet = HXNULL;

    if (fpMalloc && pszStr)
    {
        /* Allocate space for string */
        pRet = (char*) fpMalloc(pUserMem, strlen(pszStr) + 1);
        if (pRet)
        {
            /* Copy the string */
            strcpy(pRet, pszStr);
        }
    }

    return pRet;
}

void free_string(void*            pUserMem,
                 rm_free_func_ptr fpFree,
                 char**           ppszStr)
{
    if (fpFree && ppszStr && *ppszStr)
    {
        fpFree(pUserMem, *ppszStr);
        *ppszStr = HXNULL;
    }
}

#if defined(WIN32)

#if !defined(_WINCE)

int strcasecmp(const char* pszStr1, const char* pszStr2)
{
    return _stricmp(pszStr1, pszStr2);
}

#endif /* #if !defined(_WINCE) */

int strncasecmp(const char* pszStr1, const char* pszStr2, int len)
{
    return _strnicmp(pszStr1, pszStr2, (size_t) len);
}
#else
int strncasecmp(const char* pszStr1, const char* pszStr2, int len)
{
    while (--len >=0 && toupper((unsigned char)*pszStr1) == toupper((unsigned char)*pszStr2++))
    	if (*pszStr1++ == '\0') return 0;
    return (len<0? 0: toupper((unsigned char)*pszStr1) - toupper((unsigned char)*--pszStr2));
}
#endif /* #if defined(_WINDOWS) */

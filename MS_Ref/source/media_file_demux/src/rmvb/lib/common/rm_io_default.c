

#include <stdio.h>
#include "helix_types.h"
#include "rm_io_default.h"


UINT32 rm_io_default_read(void* pUserRead, BYTE* pBuf, UINT32 ulBytesToRead)
{
    UINT32 ulRet = 0;

    if (pUserRead && pBuf && ulBytesToRead)
    {
        /* For default, the void* is a FILE* */
        FILE* fp = (FILE*) pUserRead;
        /* Read the number of bytes requested */
        ulRet = (UINT32) fread(pBuf, 1, ulBytesToRead, fp);
    }

    return ulRet;
}

void rm_io_default_seek(void* pUserRead, UINT32 ulOffset, UINT32 ulOrigin)
{
    if (pUserRead)
    {
        /* For default, the void* is a FILE* */
        FILE* fp = (FILE*) pUserRead;
        /* Do the seek */
        fseek(fp, ulOffset, ulOrigin);
    }
}


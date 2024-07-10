

//#include <memory.h>
#include <string.h>
#include "helix_types.h"
#include "helix_result.h"
#include "rm_memory.h"
#include "memory_utils.h"

HX_RESULT rm_enforce_buffer_min_size(void*              pUserMem,
                                     rm_malloc_func_ptr fpMalloc,
                                     rm_free_func_ptr   fpFree,
                                     BYTE**             ppBuf,
                                     UINT32*            pulCurLen,
                                     UINT32             ulReqLen)
{
    HX_RESULT retVal = HXR_OUTOFMEMORY;

    if (fpMalloc && fpFree && ppBuf && pulCurLen)
    {
        if (ulReqLen > *pulCurLen)
        {
            /* Allocate a new buffer */
            BYTE* pBuf = fpMalloc(pUserMem, ulReqLen);
            if (pBuf)
            {
                /* Was the old buffer allocated? */
                if (*ppBuf && *pulCurLen > 0)
                {
                    /* Copy the old buffer into the new one */
                    memcpy(pBuf, *ppBuf, *pulCurLen);
                    /* Free the old buffer */
                    fpFree(pUserMem, *ppBuf);
                }
                /* Assign the buffer out parameter */
                *ppBuf = pBuf;
                /* Change the current size to the requested size */
                *pulCurLen = ulReqLen;
                /* Clear the return value */
                retVal = HXR_OK;
            }
        }
        else
        {
            /* Current buffer size is adequate - don't
             * have to do anything here.
             */
            retVal = HXR_OK;
        }
    }

    return retVal;
}

BYTE* copy_buffer(void*              pUserMem,
                  rm_malloc_func_ptr fpMalloc,
                  BYTE*              pBuf,
                  UINT32             ulLen)
{
    BYTE* pRet = HXNULL;

    if (fpMalloc && pBuf && ulLen)
    {
        /* Allocate a buffer */
        pRet = (BYTE*) fpMalloc(pUserMem, ulLen);
        if (pRet)
        {
            /* Copy the buffer */
            memcpy(pRet, pBuf, ulLen);
        }
    }

    return pRet;
}

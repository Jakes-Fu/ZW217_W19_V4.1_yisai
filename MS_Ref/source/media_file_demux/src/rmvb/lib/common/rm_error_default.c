

//#include <stdio.h>
#include "rm_error.h"
#include "rm_error_default.h"

void rm_error_default(void* pUserError, HX_RESULT err, const char* msg)
{
    if (msg)
    {
        //fprintf(stderr,"rm_error_default(pUserError=0x%08x,err=0x%08x,msg=%s\n",pUserError, err, msg);
    }
    else
    {
        //fprintf(stderr,"rm_error_default(pUserError=0x%08x,err=0x%08x\n",pUserError, err);
    }
}

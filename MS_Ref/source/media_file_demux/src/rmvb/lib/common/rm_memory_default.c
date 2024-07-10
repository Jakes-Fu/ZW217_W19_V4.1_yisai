

//#include <malloc.h>
#include <stdlib.h>
#include "rm_memory.h"
#include "rm_memory_default.h"


void* rm_memory_default_malloc(void* pUserMem, UINT32 ulSize)
{
    return malloc(ulSize);
}

void rm_memory_default_free(void* pUserMem, void* ptr)
{
    free(ptr);
}

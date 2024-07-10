
#include <stdlib.h>


#include "rs_mem.h"

#include "os_api.h"



void* rs_malloc_porting(rs_u32 allocSize)
{
	void *returnPointer;
	

	returnPointer = SCI_ALLOC(allocSize);


	return(returnPointer);
}

void  rs_free_porting(void* memBlock)
{


	SCI_Free(memBlock);
}



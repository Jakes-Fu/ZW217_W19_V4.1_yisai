#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "test.h"

int test_api1()
{
	printf("test_api1 \r\n");
	return 0;
}

int test_api2()
{
	printf("test_api1 \r\n");
	return 1;
}
#if 0
void* jmem_heap_alloc_block_null_on_error(const size_t size)
{
	printf("jmem_heap_alloc_block_null_on_error \r\n");
	return 0;
}
#endif
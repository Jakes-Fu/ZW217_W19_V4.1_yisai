
/*

Copyright (c) 2005-2008, Simon Howard

Permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

 */

/* Tests for the allocation testing framework. */

#include <stdio.h>
#include <string.h>

#include "alloc-testing.h"
#include "framework.h"

static unsigned char test_malloc_free(void)
{
	void *block, *block2, *block3, *block4;
	unsigned char *ptr;
	int i;

	/* Allocate a block and check that the counters increase */

	HALO_ASSERTRT(alloc_test_get_allocated() == 0);

	block = HALO_Malloc(1024);

	HALO_ASSERTRT(block != NULL);
	HALO_ASSERTRT(alloc_test_get_allocated() == 1024);

	/* Check that the block is initialised with garbage */

	ptr = block;

	for (i=0; i<1024; ++i) {
		HALO_ASSERTRT(ptr[i] != 0);
	}

	/* Free the block back and check the counters decrease */

	HALO_Free(block);

	HALO_ASSERTRT(alloc_test_get_allocated() == 0);

	/* Try setting a limit */

	alloc_test_set_limit(3);

	block = HALO_Malloc(1024);
	HALO_ASSERTRT(block != NULL);
	block2 = HALO_Malloc(1024);
	HALO_ASSERTRT(block2 != NULL);
	block3 = HALO_Malloc(1024);
	HALO_ASSERTRT(block3 != NULL);
	block4 = HALO_Malloc(1024);
	HALO_ASSERTRT(block4 == NULL);
	HALO_Free(block);
	HALO_Free(block2);
	HALO_Free(block3);
	HALO_Free(block4);
	return 1;
}

static unsigned char test_memcpy_menset(void)
{
	HALO_PASSERT(0,this test need to be impliment);
	return 1;
}

static Unit_Test_T tests[] = {
	TEST(test_malloc_free),
	TEST(test_memcpy_menset),
	{NULL,NULL}
};



int test_memory(void)
{
	run_tests(tests);

	return 0;
}

